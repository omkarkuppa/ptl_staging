/** @file
  PTL PCD ME Init Common Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmcLib.h>
#include <Library/MeInfoLib.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <Register/PchRegs.h>
#include <Register/HeciRegs.h>
#include <MeHandle.h>
#include <Library/P2SbSocLib.h>
#include <PcdSbPortIds.h>
#include <PciSbdf.h>

/**
  Get CSME HECI devices sideband registers access

  @param[in]      This                 The ME_HANDLE instance.
  @param[in]      DeviceSel            Device to be configured
  @param[in, out] P2SbController       P2SB controller pointer
  @param[in, out] HeciPcrAccess        P2SB register access to ME devices
**/
VOID
EFIAPI
PtlPcdGetCsmeHeciSbAccess (
  IN     ME_HANDLE                     *This,
  IN     HECI_DEVICE                   DeviceSel,
  IN OUT P2SB_CONTROLLER               *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS *HeciPcrAccess
  )
{
  P2SB_PORT_16_ID                      P2SbPid;
  PCI_SBDF                             HeciSbdf;

  HeciSbdf.PciSegmentLibAddress = This->Callbacks->GetCsmeHeciPciCfgBase (DeviceSel);

  P2SbPid.Pid16bit = (HeciSbdf.Fields.Func == SOL) ? PTL_SID_F2_PID_CSME12 : PTL_SID_F2_PID_CSME0;
  PtlPcdGetP2SbController (P2SbController, P2SbPid);
  BuildP2SbSidebandAccess (
    P2SbController,
    P2SbPid.PortId.LocalPid,
    (UINT16) ((HeciSbdf.Fields.Dev << 3) + HeciSbdf.Fields.Func),
    P2SbPrivateConfig,
    P2SbMsgAccess,
    FALSE,
    HeciPcrAccess
    );
}

/**
  Get CSME HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval CSME HECI controller address in PCI Segment Library representation
**/
UINT64
EFIAPI
PtlPcdGetCsmeHeciPciCfgBase (
  IN HECI_DEVICE   HeciFunc
  )
{
  return MeHeciPciCfgBase (HeciFunc);
}

/**
  Get PSE HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc     PSE HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
EFIAPI
PtlPcdGetPseHeciPciCfgBase (
  IN PSE_HECI_DEVICE    HeciFunc
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_PSE_HECI1,
           HeciFunc,
           0
           );
}

/**
  Get Xhci controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PtlPcdGetXhciPciCfgBase (
  VOID
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_XHCI,
           PCI_FUNCTION_NUMBER_PCH_XHCI,
           0
           );
}
