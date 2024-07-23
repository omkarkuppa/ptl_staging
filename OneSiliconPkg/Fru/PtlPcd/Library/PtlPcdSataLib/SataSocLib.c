/** @file
  SATA SoC library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PchInfoLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/SataSocLib.h>
#include <Register/PchRegs.h>

/**
  Get SATA controller instance

  @param[in]  SataCtrlIndex       SATA controller index
  @param[out] SataController      Pointer to SATA controller structure

  @retval     EFI_SUCCESS           - Completted successfully
              EFI_INVALID_PARAMETER - SataController NULL or SataCtrlIndex out of the range
**/
EFI_STATUS
SataGetController (
  IN  UINT32              SataCtrlIndex,
  OUT SATA_CONTROLLER     *SataController
  )
{
  if (SataController == NULL) {
    DEBUG ((DEBUG_WARN, "%a: Null pointer at SataController!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }
  if (SataCtrlIndex >= MaxSataControllerNum ()) {
    DEBUG ((DEBUG_WARN, "%a: Controller number out of the range!\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  SataController->SataCtrlIndex     = SataCtrlIndex;
  SataController->Segment           = DEFAULT_PCI_SEGMENT_NUMBER_PCH;
  SataController->Bus               = DEFAULT_PCI_BUS_NUMBER_PCH;
  SataController->Device            = SataDevNumber (SataCtrlIndex);
  SataController->Function          = SataFuncNumber (SataCtrlIndex);
  SataController->PciCfgBaseAddr    = SataPciCfgBase (SataCtrlIndex);
  SataController->TotalCtrlPortNum  = MaxSataPortNum (SataCtrlIndex);

  return EFI_SUCCESS;
}

/**
  Get Maximum Sata Controller Number

  @retval Maximum Sata Controller Number
**/
UINT8
MaxSataControllerNum (
  VOID
  )
{
  //
  // Defined in SoC integration header
  //
  return 0;
}

/**
  Get Maximum Sata Port Number

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     Maximum Sata Port Number
**/
UINT8
MaxSataPortNum (
  IN UINT32      SataCtrlIndex
  )
{
  return 0;
}

/**
  Check if SATA controller supports RST remapping

  @param[in]  SataCtrlIndex       SATA controller index

  @retval     TRUE                Controller supports remapping
  @retval     FALSE               Controller does not support remapping

**/
BOOLEAN
IsRemappingSupportedOnSata (
  IN UINT32  SataCtrlIndex
  )
{
  return FALSE;
}

/**
  Checks if SoC supports the SATA PGD power down on given
  SATA controller.

  @param[in] SataCtrlIndex  SATA controller index

  @retval TRUE   SATA PGD power down supported
  @retval FALSE  SATA PGD power down not supported
**/
BOOLEAN
IsSataPowerGatingSupported (
  IN UINT32 SataCtrlIndex
  )
{
  return TRUE;
}
