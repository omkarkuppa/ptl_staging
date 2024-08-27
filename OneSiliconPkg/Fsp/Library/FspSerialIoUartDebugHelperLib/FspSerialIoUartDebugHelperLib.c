/** @file
  Implementation of FspSerialIoUartDebugHelperLib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include <PiPei.h>
#include <FspEas.h>
#include <FsptUpd.h>
#include <FspmUpd.h>
#include <Library/FspCommonLib.h>
#include <Library/LpssUartDebugPropertyPcdLib.h>
#include <Library/LpssUartLib.h>



/**
  Return FSPT UPD Location Ppi

**/
FSPT_UPD *
EFIAPI
FspSerialIoUartDebugGetFsptUpdLocPpi (
  VOID
  )
{
  CONST EFI_PEI_SERVICES    **PeiServices;
  IA32_DESCRIPTOR           Idtr;
  EFI_STATUS                Status;
  FSPT_UPD                  *FsptUpdLocationPpi;

  Status = EFI_SUCCESS;
  PeiServices = NULL;
  FsptUpdLocationPpi = NULL;

  if (PcdGetBool (PcdFspValidatePeiServiceTablePointer)) {  // New PCD to control do PeiServiceTablepointer check or not. For SEC, this PCD is configured as FALSE. Its default value is TRUE
    AsmReadIdtr (&Idtr);
    PeiServices = (CONST EFI_PEI_SERVICES **) (*(UINTN*)(Idtr.Base - sizeof (UINTN)));
  }
  if (PeiServices != NULL) {
    Status =  (*PeiServices)->LocatePpi (
                                PeiServices,
                                &gFsptUpdLocationPpiGuid,
                                0,
                                NULL,
                                (VOID **) &FsptUpdLocationPpi
                                );
    if (EFI_ERROR (Status)) {
      FsptUpdLocationPpi = NULL;
    }
  }
  return FsptUpdLocationPpi;
}

/**
  Get FSPT Config

  @param[in] FsptConfig        FSPT Config

  @retval TRUE if FSPT exists
  @retval FALSE unable to locate FSP
**/
BOOLEAN
STATIC
GetFsptConfig (
  OUT FSP_T_CONFIG  **Fspt
  )
{
  FSPT_UPD        *FsptUpd;

  FsptUpd = FspSerialIoUartDebugGetFsptUpdLocPpi ();
  if (FsptUpd != NULL) {
    *Fspt = &FsptUpd->FsptConfig;
    return TRUE;
  }
  return FALSE;
}

/**
  Get FSPM Config

  @param[out] Fspm        FSPM Config

  @retval TRUE if FSPM exists
  @retval FALSE unable to locate FSP
**/
BOOLEAN
STATIC
GetFspmConfig (
  OUT FSP_M_CONFIG  **Fspm
  )
{
  FSP_GLOBAL_DATA *FspData;
  FSPM_UPD        *FspmUpd;

  FspData = GetFspGlobalDataPointer ();

  if (((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) ||
     (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    return FALSE;
  } else if ((FspData->FspMode == FSP_IN_API_MODE) && (FspData->MemoryInitUpdPtr != NULL)) { // FSP_MODE_CHECK
    FspmUpd = FspData->MemoryInitUpdPtr;
    *Fspm = &FspmUpd->FspmConfig;
    return TRUE;
  }
  return FALSE;
}

/**
  Returns Serial Io UART MMIO Base Address in Debug mode

  @retval  MMIO Base Address by default in PCI Mode
**/
UINTN
STATIC
FspSerialIoUartDebugGetPciDefaultMmioBase (
  VOID
  )
{
  FSP_M_CONFIG    *FspmConfig;
  FSP_T_CONFIG    *FsptConfig;
  UINTN           MmioBase;

  FspmConfig = NULL;
  FsptConfig = NULL;

  if (GetFsptConfig (&FsptConfig)) {
    MmioBase = FsptConfig->PcdSerialIoUartDebugMmioBase;
  } else if (GetFspmConfig (&FspmConfig)) {
    MmioBase = FspmConfig->SerialIoUartDebugMmioBase;
  } else {
    return LpssUartDebugPcdGetPciDefaultMmioBase ();
  }
  return MmioBase;
}

/**
  Initialize SerialIo UART for debug.

**/
VOID
EFIAPI
SerialIoUartDebugInit (
  VOID
  )
{
  //
  // Skip the initialization for Debug Mode.
  //
}

/**
  Write data to serial device.

  If the buffer is NULL, then return 0;
  if NumberOfBytes is zero, then return 0.

  @param[in]  Buffer           Data pointer
  @param[in]  NumberOfBytes    Number of output bytes which are cached in Buffer.

  @retval                      Actual number of bytes written to serial device.
**/
UINTN
EFIAPI
SerialIoUartDebugWrite (
  IN UINT8            *Buffer,
  IN UINTN            NumberOfBytes
  )
{
  return LpssUartWrite((UINTN)FspSerialIoUartDebugGetPciDefaultMmioBase (), Buffer, NumberOfBytes);
}
