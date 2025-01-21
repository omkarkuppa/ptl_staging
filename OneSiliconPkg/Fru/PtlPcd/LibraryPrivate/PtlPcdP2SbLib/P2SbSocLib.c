/** @file
  P2SB SoC library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/SocInfoLib.h>
#include <Library/P2SbSocLib.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Defines/PchReservedResources.h>
#include <PcieRegs.h>
#include <PcrDefine.h>
#include <PcdSbPortIds.h>

/**
  Get P2SB instance for SOC

  @param[in, out] P2SbController        P2SB controller pointer
  @param[in]      Pid                   P2SB 16 bits port ID

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL or wrong Segment
**/
EFI_STATUS
PtlPcdGetP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController,
  IN     P2SB_PORT_16_ID         Pid
  )
{
  if (P2SbController == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (Pid.PortId.SegmentId == PTL_SID_F2_P2SB_2) {
    return PtlPcdGetFirstP2SbController (P2SbController);
  } else if (Pid.PortId.SegmentId == PTL_SID_F3_P2SB_1) {
    return PtlPcdGetSecondP2SbController (P2SbController);
  }

  return EFI_INVALID_PARAMETER;
}

/**
  Get P2SB first instance of P2SB
  First instance corresponds to segment ID 0xF3

  @param[in, out] P2SbController        P2SB controller pointer

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL
**/
EFI_STATUS
PtlPcdGetFirstP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController
  )
{
  UINT8   Dev;
  UINT8   Fun;

  if (P2SbController == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = PCI_DEVICE_NUMBER_PCH_P2SB;
  Fun = PCI_FUNCTION_NUMBER_PCH_P2SB;

  ZeroMem (P2SbController, sizeof (P2SB_CONTROLLER));
  P2SbController->PciCfgBaseAddr = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, Dev, Fun, 0);
  P2SbController->Mmio           = PCH_PCR_BASE_ADDRESS;
  P2SbController->P2sb20bPcrSupported = IsP2sb20bPcrSupported ();

  return EFI_SUCCESS;
}

/**
  Get P2SB seccond instance of P2SB
  Seccond instance corresponds to segment ID 0xF2

  @param[in, out] P2SbController        P2SB controller pointer

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - P2SbController NULL
**/
EFI_STATUS
PtlPcdGetSecondP2SbController (
  IN OUT P2SB_CONTROLLER         *P2SbController
  )
{
  UINT8   Dev;
  UINT8   Fun;

  if (P2SbController == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Dev = PCI_DEVICE_NUMBER_SECOND_P2SB;
  Fun = PCI_FUNCTION_NUMBER_SECOND_P2SB;

  ZeroMem (P2SbController, sizeof (P2SB_CONTROLLER));
  P2SbController->PciCfgBaseAddr = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, Dev, Fun, 0);
  P2SbController->Mmio           = PCH_PCR_SECOND_BASE_ADDRESS;
  P2SbController->P2sb20bPcrSupported = IsP2sb20bPcrSupported ();

  return EFI_SUCCESS;
}

/**
  Checks if second instance of P2SB is hidden.

  @return TRUE if P2SB is hidden.
**/
BOOLEAN
PtlPcdIsSecondP2SbHidden (
  VOID
  )
{
  UINT64  PciAddress;

  PciAddress = PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH, DEFAULT_PCI_BUS_NUMBER_PCH, PCI_DEVICE_NUMBER_SECOND_P2SB, PCI_FUNCTION_NUMBER_SECOND_P2SB, 0);
  if (PciSegmentRead16 (PciAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    return TRUE;
  } else {
    return FALSE;
  }
}
