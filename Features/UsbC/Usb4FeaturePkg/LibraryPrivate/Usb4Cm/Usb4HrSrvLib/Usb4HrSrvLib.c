/** @file
  USB4 host router service routines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4Common.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HrInst.h>
#include <Library/CmUtilsLib.h>
#include <Library/Usb4HcMem.h>

/**
  Allocates pages that are suitable for USB4 Tx/Rx ring DMA access.

  @param[in]  Usb4Hr   - Pointer to host router instance.
  @param[in]  BufPages - The buffer pages to be allocated.
  @param[out] BufAddr  - A pointer of pointer to store the allocated DMA buffer address.

  @retval EFI_SUCCESS           - The requested memory pages were allocated.
  @retval EFI_OUT_OF_RESOURCES  - Insufficient resources.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
PrePciUsb4DmaBufAlloc (
  IN  USB4_HR_INSTANCE    *Usb4Hr,
  IN  UINT32              BufPages,
  OUT VOID                **BufAddr
  )
{
  EFI_STATUS              Status;
  PHYSICAL_ADDRESS        DmaBufAddr;

  DEBUG ((DEBUG_INFO, "PrePciUsb4DmaBufAlloc entry - BufPages = %d\n", BufPages));

  Status = EFI_SUCCESS;

  if ((Usb4Hr == NULL) || (BufAddr == NULL)) {
    DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufAlloc: Invalid (%p, %p)\n", Usb4Hr, BufAddr));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Get DMA buffer from PEI reserved DMA area if IOMMU is enabled and PeiDmaBufInUse flag is FALSE.
  // Get DMA buffer by boot service if IOMMU is not enabled.
  //
  if (Usb4Hr->HrInfo.PeiDmaBufBase == 0) {
    DEBUG ((DEBUG_INFO, "Iommu is not enabled\n"));
    Status = gBS->AllocatePages (AllocateAnyPages, EfiBootServicesData, BufPages, &DmaBufAddr);
    if (!EFI_ERROR (Status)) {
      *BufAddr = (VOID *) DmaBufAddr;
      DEBUG ((DEBUG_INFO, "PrePciUsb4DmaBufAlloc: Allocated DMA buffer address = 0x%ll0X\n", DmaBufAddr));
    } else {
      DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufAlloc: Allocate buffer failure, %r\n", Status));
      goto Exit;
    }
  } else {
    DEBUG ((DEBUG_INFO, "Iommu is enabled\n"));

    if (!Usb4Hr->PeiDmaBufInUse) {
      DEBUG ((DEBUG_INFO, "DmaBufferBase = 0x%ll0X\n", Usb4Hr->HrInfo.PeiDmaBufBase));
      DEBUG ((DEBUG_INFO, "DmaBufferSize = 0x%08X\n", Usb4Hr->HrInfo.PeiDmaBufSize));

      if (Usb4Hr->HrInfo.PeiDmaBufSize < EFI_PAGES_TO_SIZE (BufPages)) {
        DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufAlloc: PEI reserved DMA buffer is too small, required size = 0x%0x\n", EFI_PAGES_TO_SIZE (BufPages)));
        Status = EFI_OUT_OF_RESOURCES;
        goto Exit;
      }
      *BufAddr = (VOID *) Usb4Hr->HrInfo.PeiDmaBufBase;

      //
      // Set PeiDmaBufInUse flag to TRUE to avoid the reserved DMA buffer is re-assigned.
      //
      Usb4Hr->PeiDmaBufInUse = TRUE;
      DEBUG ((DEBUG_INFO, "Allocated buffer address = 0x%ll0X\n", *BufAddr));
    } else {
      DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufAlloc: PEI reserved DMA buffer has been in use, no more DMA buffer is available\n"));
      Status = EFI_OUT_OF_RESOURCES;
      goto Exit;
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "PrePciUsb4DmaBufAlloc exit\n"));
  return Status;
}

/**
  Frees memory that was allocated with PrePciUsb4DmaBufAlloc().

  @param[in] HrContext   - USB4 host router context.
  @param[in] HostAddress - The system memory address of DMA buffer to free.
  @param[in] BufPages    - The number of pages to free.
**/
VOID
PrePciUsb4DmaBufFree (
  IN USB4_HR_CONTEXT    HrContext,
  IN VOID               *HostAddress,
  IN UINT32             BufPages
  )
{
  EFI_STATUS              Status;
  USB4_HR_INSTANCE        *Usb4Hr;

  DEBUG ((DEBUG_INFO, "PrePciUsb4DmaBufFree entry\n"));

  Usb4Hr = (USB4_HR_INSTANCE *) HrContext;

  if ((Usb4Hr == NULL) || (HostAddress == NULL)) {
    DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufFree: Invalid (%p, %p)\n", Usb4Hr, HostAddress));
    goto Exit;
  }

  //
  // Set PeiDmaBufInUse flag to FALSE to indicate PEI reserved DMA buffer available if IOMMU is enabled.
  // Free DMA buffer by boot service if IOMMU is not enabled.
  //
  if (Usb4Hr->HrInfo.PeiDmaBufBase == 0) {
    DEBUG ((DEBUG_INFO, "Iommu is not enabled\n"));
    DEBUG ((DEBUG_INFO, "Buffer address = 0x%ll0x, Pages=0x%x\n", HostAddress, BufPages));

    Status = gBS->FreePages ((PHYSICAL_ADDRESS) HostAddress, BufPages);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PrePciUsb4DmaBufFree: Free buffer failure, %r\n", Status));
      goto Exit;
    }
  } else {
    DEBUG ((DEBUG_INFO, "Iommu is enabled\n"));
    Usb4Hr->PeiDmaBufInUse = FALSE;
  }

Exit:
  DEBUG ((DEBUG_INFO, "PrePciUsb4DmaBufFree exit\n"));
  return;
}

/**
  Allocates pages that are suitable for NHI DMA access.

  @param[in]  HrContext     - USB4 host router context for DMA buffer allocation.
  @param[in]  BufPages      - The buffer size to be allocated.
  @param[out] HostAddress   - Pointer of pointer to the system memory address of the allocated DMA buffer.
  @param[out] DeviceAddress - Pointer to the resulting map address for the bus master PCI controller.
  @param[out] Mapping       - A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           - The requested memory pages were allocated.
  @retval EFI_OUT_OF_RESOURCES  - Buffer allocation failure.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvDmaBufAlloc (
  IN  USB4_HR_CONTEXT     HrContext,
  IN  UINT32              BufPages,
  OUT VOID                **HostAddress,
  OUT PHYSICAL_ADDRESS    *DeviceAddress,
  OUT VOID                **Mapping
  )
{
  EFI_STATUS          Status;
  USB4_HR_INSTANCE    *Usb4Hr;

  DEBUG ((DEBUG_INFO, "HrSrvDmaBufAlloc entry - BufPages = %d\n", BufPages));

  Usb4Hr = (USB4_HR_INSTANCE *) HrContext;

  if ((Usb4Hr == NULL) || (HostAddress == NULL) || (DeviceAddress == NULL) || (Mapping == NULL)) {
    DEBUG ((DEBUG_ERROR, "HrSrvDmaBufAlloc: Invalid (%p, %p, %p, %p)\n", Usb4Hr, HostAddress, DeviceAddress, Mapping));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->Signature != USB4_HR_INSTANCE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "HrSrvDmaBufAlloc: Invalid USB4 host router signature 0x%0x\n", Usb4Hr->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->PciIo == NULL) {
    Status = PrePciUsb4DmaBufAlloc (HrContext, BufPages, HostAddress);
    if (!EFI_ERROR (Status)) {
      *DeviceAddress = (PHYSICAL_ADDRESS) *HostAddress;
      *Mapping       = NULL;
    }
  } else {
    Status = UsbHcAllocateAlignedPages (Usb4Hr->PciIo, BufPages, EFI_PAGE_SIZE, HostAddress, DeviceAddress, Mapping);
  }

Exit:
  DEBUG ((DEBUG_INFO, "HrSrvDmaBufAlloc exit\n"));
  return Status;
}

/**
  Free the DMA buffer that is allocated by HrSrvDmaBufAlloc.

  @param[in] HrContext   - USB4 host router context.
  @param[in] HostAddress - The system memory address of DMA buffer to free.
  @param[in] BufPages    - The number of pages to free.
  @param[in] Mapping     - The mapping value returned from Map().
**/
VOID
HrSrvDmaBufFree (
  IN USB4_HR_CONTEXT     HrContext,
  IN VOID                *HostAddress,
  IN UINT32              BufPages,
  IN VOID                *Mapping
  )
{
  USB4_HR_INSTANCE    *Usb4Hr;

  DEBUG ((DEBUG_INFO, "HrSrvDmaBufFree entry - BufPages = %d\n", BufPages));

  Usb4Hr = (USB4_HR_INSTANCE *) HrContext;

  if ((Usb4Hr == NULL) || (HostAddress == NULL)) {
    DEBUG ((DEBUG_ERROR, "HrSrvDmaBufFree: Invalid (%p, %p)\n", Usb4Hr, HostAddress));
    goto Exit;
  }

  if (Usb4Hr->Signature != USB4_HR_INSTANCE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "HrSrvDmaBufFree: Invalid USB4 host router signature 0x%0x\n", Usb4Hr->Signature));
    goto Exit;
  }

  if (Usb4Hr->PciIo == NULL) {
    PrePciUsb4DmaBufFree (HrContext, HostAddress, BufPages);
  } else {
    UsbHcFreeAlignedPages (Usb4Hr->PciIo, HostAddress, BufPages, Mapping);
  }

Exit:
  DEBUG ((DEBUG_INFO, "HrSrvDmaBufFree exit\n"));
}

/**
  Read MMIO data from Host Router NHI device via BAR0.

  @param[in]  HrContext - Host router context for MMIO read.
  @param[in]  Offset    - The offset to start the memory operation.
  @param[in]  Count     - The number in DWORDs of memory operations to perform.
  @param[out] Buffer    - Data buffer for MMIO read.

  @retval EFI_SUCCESS           - MMIO data read success.
  @retval EFI_NOT_READY         - MMIO space is not assigned to NHI device.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvMmioRead (
  IN  USB4_HR_CONTEXT    HrContext,
  IN  UINT32             Offset,
  IN  UINT32             Count,
  OUT VOID               *Buffer
  )
{
  EFI_STATUS          Status;
  USB4_HR_INSTANCE    *Usb4Hr;
  UINT32              Index;
  UINTN               MmioReg;
  UINT32              *Data32;

  Usb4Hr = (USB4_HR_INSTANCE *) HrContext;

  if ((Usb4Hr == NULL) || (Buffer == NULL)) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioRead: Invalid parameter - (%p, %p)\n", Usb4Hr, Buffer));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->Signature != USB4_HR_INSTANCE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioRead: Invalid USB4 host router signature 0x%0x\n", Usb4Hr->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->MmioBase == 0) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioRead: MMIO base is not present!\n"));
    Status = EFI_NOT_READY;
    goto Exit;
  }

  MmioReg = (UINTN)(Usb4Hr->MmioBase + Offset);
  Data32 = (UINT32 *)Buffer;
  for(Index = 0; Index < Count; Index++) {
    Data32[Index] = MmioRead32 (MmioReg);
    MmioReg += 4;
  }

  Status = EFI_SUCCESS;
Exit:
  return Status;
}

/**
  Write MMIO data to Host Router NHI device via BAR0.

  @param[in]  HrContext - Host router context for MMIO write.
  @param[in]  Offset    - The offset to start the memory operation.
  @param[in]  Count     - The number in DWORDs of memory operations to perform.
  @param[in]  Buffer    - Data buffer for MMIO write.

  @retval EFI_SUCCESS           - MMIO data write success.
  @retval EFI_NOT_READY         - MMIO space is not assigned to NHI device.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
HrSrvMmioWrite (
  IN  USB4_HR_CONTEXT    HrContext,
  IN  UINT32             Offset,
  IN  UINT32             Count,
  IN  VOID               *Buffer
  )
{
  EFI_STATUS          Status;
  USB4_HR_INSTANCE    *Usb4Hr;
  UINT32              Index;
  UINTN               MmioReg;
  UINT32              *Data32;

  Usb4Hr = (USB4_HR_INSTANCE *) HrContext;

  if ((Usb4Hr == NULL) || (Buffer == NULL)) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioWrite: Invalid parameter - (%p, %p)\n", Usb4Hr, Buffer));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->Signature != USB4_HR_INSTANCE_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioWrite: Invalid USB4 host router signature 0x%0x\n", Usb4Hr->Signature));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4Hr->MmioBase == 0) {
    DEBUG ((DEBUG_ERROR, "HrSrvMmioWrite: MMIO base is not present!\n"));
    Status = EFI_NOT_READY;
    goto Exit;
  }

  MmioReg = (UINTN)(Usb4Hr->MmioBase + Offset);
  Data32 = (UINT32 *)Buffer;
  for(Index = 0; Index < Count; Index++) {
    MmioWrite32 (MmioReg, Data32[Index]);
    MmioReg += 4;
  }

  Status = EFI_SUCCESS;

Exit:
  return Status;
}
