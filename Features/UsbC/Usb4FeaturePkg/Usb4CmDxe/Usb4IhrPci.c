/** @file
  Integrated USB4 host router PCI routines.

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
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4HrSrvLib.h>
#include "Usb4IhrPci.h"

/**
  Dump PCI registers of USB4 host router

  @param[in] HrInfo - USB4 host router information
**/
VOID
Iusb4HrPciDump (
  PUSB4_HR_INFO    HrInfo
  )
{
  UINT64         DevAddr;
  PCI_DEV_BDF    *Bdf;


  if (HrInfo != NULL) {
    DEBUG ((DEBUG_INFO, "Integrated USB4 Host Router PCI Information:\n"));
    DEBUG ((DEBUG_INFO, "NHI Device:\n"));
    Bdf = &(HrInfo->NhiBdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bdf->Bus, Bdf->Dev, Bdf->Func, 0);
    HrSrvDumpPciDevRegs (DevAddr);
  }
}

/**
  Dump integrated USB4 host router Control information.

  @param[in] Usb4HrInst   - Pointer to USB4 host router instance.
**/
VOID
Iusb4HrDump (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  PCI_DEV_BDF      *Bdf;
  PUSB4_HR_INFO    HrInfo;

  if (Usb4HrInst != NULL) {
    HrInfo = &(Usb4HrInst->HrInfo);
    DEBUG ((DEBUG_INFO, "Integrated USB4 Host Router Info:\n"));
    DEBUG ((DEBUG_INFO, "    MbCmdReg = 0x%0x, MbStatusReg = 0x%0x\n", HrInfo->MbCmdReg, HrInfo->MbStatusReg));
    Bdf = &(HrInfo->MailboxBdf);
    DEBUG ((DEBUG_INFO, "    Mailbox B/D/F (Hex) = (%x/%x/%x)\n", Bdf->Bus,  Bdf->Dev, Bdf->Func));
    Bdf = &(HrInfo->NhiBdf);
    DEBUG ((DEBUG_INFO, "    NHI B/D/F (Hex) = (%x/%x/%x)\n", Bdf->Bus, Bdf->Dev, Bdf->Func));
    DEBUG ((DEBUG_INFO, "    MMIO base = %016llx, MMIO length = 0x%x\n", Usb4HrInst->MmioBase, Usb4HrInst->MmioLen));
    DEBUG ((DEBUG_INFO, "    Iommu DmaBuf base = 0x%0x, size = %d\n", HrInfo->PeiDmaBufBase, HrInfo->PeiDmaBufSize));
  }
}

/**
  Force power to USB4 host router.

  @param[in] Usb4Hr      - Pointer to USB4 host router instance.
  @param[in] SetForcePwr - TRUE = Set force power, FALSE = Un-power.

  @retval EFI_SUCCESS           - Force power success.
  @retval EFI_UNSUPPORTED       - Device is not ready for force power.
  @retval EFI_TIMEOUT           - Device is not ready after timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4HrForcePwr (
  IN USB4_HR_INSTANCE    *Usb4Hr,
  IN BOOLEAN             SetForcePwr
  )
{
  EFI_STATUS    Status;
  PCI_DEV_BDF   *NhiBdf;
  UINT32        Index;
  UINT32        RegVal;
  UINT64        DevAddr;

  DEBUG ((DEBUG_INFO, "Iusb4HrForcePwr entry - Usb4Hr %p\n", Usb4Hr));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_INFO, "Iusb4HrForcePwr: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  NhiBdf = &(Usb4Hr->HrInfo.NhiBdf);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (
              0,
              (UINT32)NhiBdf->Bus,
              (UINT32)NhiBdf->Dev,
              (UINT32)NhiBdf->Func,
              0
              );

  if (SetForcePwr) {
    DEBUG ((DEBUG_INFO, "iUSB4 HR set force power\n"));
    //
    // Set force power
    //
    PciSegmentAndThenOr32 (
      DevAddr + R_HOST_DMA_RTD3_FORCEPOWER,
      (UINT32) ~(B_HOST_DMA_DMA_ACTIVE_DELAY_MASK),
      B_HOST_DMA_FORCEPOWER_EN | (V_HOST_DMA_DMA_ACTIVE_DELAY_VALUE << N_HOST_DMA_DMA_ACTIVE_DELAY_OFFSET)
    );

    //
    //  Polling NVM FW version
    //
    for (Index = 0; Index < HR_FORCE_POWER_POLLING_MAX; Index++) {
      RegVal = PciSegmentRead32 (DevAddr + R_HOST_DMA_ITBT_NVM_FW_REVISION);
      if (RegVal == 0xFFFFFFFF) {
        DEBUG ((DEBUG_ERROR, "Device is not ready!\n"));
        Status = EFI_UNSUPPORTED;
        goto Exit;
      }
      if ((RegVal & BIT31) == BIT31) {
        DEBUG ((DEBUG_INFO, "iUSB4 HR NVM FW version = 0x%08X, retry = %d\n", RegVal, Index));
        break;
      }
      MicroSecondDelay (HR_FORCE_POWER_POLLING_US);
    }

    if (Index >= HR_FORCE_POWER_POLLING_MAX) {
      DEBUG ((DEBUG_ERROR, "iUSB4 HR is not ready, regval=0x%x\n", RegVal));
      Status = EFI_TIMEOUT;
      goto Exit;
    }
  } else {
    DEBUG ((DEBUG_INFO, "iUSB4 HR un-power\n"));
    //
    // Un-power
    //
    PciSegmentAnd8 (DevAddr + R_HOST_DMA_RTD3_FORCEPOWER, (UINT8) (~B_HOST_DMA_FORCEPOWER_EN));
  }

  Status = EFI_SUCCESS;
Exit:
  DEBUG ((DEBUG_INFO, "Iusb4HrForcePwr exit\n"));
  return Status;
}

/**
  Initialize PCI ocnfiguration space of integrated USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS            - PCI initialization success.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
  @retval EFI_UNSUPPORTED        - Can't initialize PCI configuration space of USB4 host router.
**/
EFI_STATUS
Iusb4HrPciInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  EFI_STATUS    Status;
  PCI_DEV_BDF   *NhiBdf;
  UINT64        DevAddr;

  DEBUG ((DEBUG_INFO, "Iusb4HrPciInit entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciInit: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  if (Usb4HrInst->MmioBase == 0) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciInit: MMIO resource is not present!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Get devce address of NHI device
  //
  NhiBdf = &(Usb4HrInst->HrInfo.NhiBdf);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (
              0,
              (UINT32)NhiBdf->Bus,
              (UINT32)NhiBdf->Dev,
              (UINT32)NhiBdf->Func,
              0
              );

  //
  // Check if the NHI device is USB4 host router.
  //
  Status = HrSrvPciDetectUsb4Hr (DevAddr);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Iusb4HrPciInit: USB4 host router is not present!\n"));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Set force power
  //
  Status = Iusb4HrForcePwr (Usb4HrInst, TRUE);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciInit: Set force power failure, %r\n", Status));
    goto Exit;
  }

  //
  // Initialize NHI device
  //
  Status = HrSrvPciDevInit (DevAddr, Usb4HrInst->MmioBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciInit: Fail to initialize NHI, %r\n", Status));
    goto Exit;
  }
  Usb4HrInst->PciInitDone = TRUE;

  Iusb4HrDump (Usb4HrInst);
  Iusb4HrPciDump (&(Usb4HrInst->HrInfo));

Exit:
  DEBUG ((DEBUG_INFO, "Iusb4HrPciInit exit\n"));
  return Status;
}

/**
  Un-Initialize PCI configuration space of integrated USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Iusb4HrPciUnInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  EFI_STATUS    Status;
  PCI_DEV_BDF   *NhiBdf;
  UINT64        DevAddr;

  DEBUG ((DEBUG_INFO, "Iusb4HrPciUnInit entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciUnInit: Invalid parameter\n"));
    goto Exit;
  }

  NhiBdf = &(Usb4HrInst->HrInfo.NhiBdf);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (
              0,
              (UINT32)NhiBdf->Bus,
              (UINT32)NhiBdf->Dev,
              (UINT32)NhiBdf->Func,
              0
              );

  //
  // Un-Init NHI device
  //
  if (Usb4HrInst->PciInitDone) {
    Status = HrSrvPciDevUninit (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Iusb4HrPciUnInit: Fail to un-initialize NHI, %r\n", Status));
    }
    Usb4HrInst->PciInitDone = FALSE;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Iusb4HrPciUnInit exit\n"));
}

/**
  Query PCI information of integrated USB4 host router.
  This function should be called after PCI enumeration.

  @param[in, out] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Query PCI information successfully.
  @retval EFI_UNSUPPORTED       - Fail to query PCI information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4HrPciQuery (
  IN OUT USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS     Status;
  PCI_DEV_BDF    *NhiBdf;
  UINT64         DevAddr;

  DEBUG ((DEBUG_INFO, "Iusb4HrPciQuery entry\n"));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciQuery: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  // Set host router information.
  //
  NhiBdf = &(Usb4Hr->HrInfo.NhiBdf);

  //
  // Query PCI I/O protocol of host router.
  //
  Status = HrSrvPciQueryIoProtocol (NhiBdf->Bus, NhiBdf->Dev, NhiBdf->Func, &(Usb4Hr->PciIo));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciQuery: Query PCI I/O protocol failure, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query MMIO space for the programming interface of Tx/Rx Ring access.
  //
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (
              0,
              (UINT32)NhiBdf->Bus,
              (UINT32)NhiBdf->Dev,
              (UINT32)NhiBdf->Func,
              0
              );
  Status = HrSrvPciQueryMmio (DevAddr, &(Usb4Hr->MmioBase));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Iusb4HrPciQuery: Query MMIO failure, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Iusb4HrPciQuery exit\n"));
  return Status;
}