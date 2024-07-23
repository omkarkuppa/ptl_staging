/** @file
  This file contains PSF routines for RC usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PsfLib.h>
#include <Register/PchRegsPsf.h>

//
// Do not override PSF Grant Count value and leave HW default setting
//
#define DEFAULT_PCIE_GRANT_COUNT          0xFF
#define PSF_PCIE_CHANNELS_PER_CONTROLLER  4u

/**
  This function is the method for PSF_SEGMENT_TABLE structure
  Function returns PSF_SEGMENT structure from PSF_SEGMENT_TABLE
  for corresponding Psf Id

  @param[in] PsfSegmentTable  PSF Segment Table
  @param[in] PsfId            PSF ID
**/
STATIC
PSF_SEGMENT*
PsfGetSegment (
  IN PSF_SEGMENT_TABLE  *PsfSegmentTable,
  IN UINT32             PsfId
  )
{
  UINT32  Index;

  for (Index = 0; Index < PsfSegmentTable->Size; Index++) {
    if (PsfSegmentTable->Data[Index].Id == PsfId) {
      return &PsfSegmentTable->Data[Index];
    }
  }
  return NULL;
}

/**
  This function is the method for PSF_SEGMENT_TABLE structure
  Function returns PSF_DEV object from PSF_SEGMENT_TABLE
  for corresponding Psf Id

  @param[in] PsfSegmentTable  PSF Segment Table
  @param[in] PsfId            PSF ID
**/
PSF_DEV*
PsfGetDev (
  IN PSF_SEGMENT_TABLE  *PsfSegmentTable,
  IN UINT32             PsfId
  )
{
  UINT32  Index;

  for (Index = 0; Index < PsfSegmentTable->Size; Index++) {
    if (PsfSegmentTable->Data[Index].Id == PsfId) {
      return PsfSegmentTable->Data[Index].PsfDev;
    }
  }
  return NULL;
}

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableBridge (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS
    );
}

/**
  Enable Bridge (e.g. PCIe Root port) at PSF level

  @param[in] PsfPort  PSF PORT data structure
**/
STATIC
VOID
PsfEnableBridge (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN,
    (UINT32)~B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS,
    0
    );
}

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level in RS3

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfRs3DisableBridge (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Enable bridge (e.g. PCIe Root Port) at PSF level in RS3

  @param[in] PsfPort  PSF PORT data structure
**/
STATIC
VOID
PsfRs3EnableBridge (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    (UINT32)~B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS,
    0
    );
}

/**
  Check if bridge (e.g. PCIe Root Port) is enabled at PSF level

  @param[in] PsfPort  PSF PORT data structure

  @retval TRUE        Bridge behind PSF Port is enabled
          FALSE       Bridge behind PSF Port is disabled
**/
BOOLEAN
PsfIsBridgeEnabled (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return FALSE;
  }

  return ((PsfPort.PsfDev->Access->Read32 (PsfPort.PsfDev->Access, PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN)
           & B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS) == 0);
}

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    (UINT32) ~(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS),
    0
    );
}

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS,
    (UINT32) ~(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS_CFGDIS),
    0
    );
}

/**
  Disable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarDisMask  BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfDisableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarDisMask
  )
{
  if (PSF_IS_PORT_NULL (PsfPort) || !(BarDisMask < BIT6)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    BarDisMask << N_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BARXDIS
    );
}

/**
  Enable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarEnMask   BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfEnableDeviceBar (
  IN PSF_PORT  PsfPort,
  IN UINT32    BarEnMask
  )
{
  if (PSF_IS_PORT_NULL (PsfPort) || !(BarEnMask < BIT6)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    (UINT32)~(BarEnMask << N_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BARXDIS),
    0
    );
}

/**
  Disable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfDisableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~(UINT32)(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_IOEN),
    0
    );
}

/**
  Enable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
STATIC
VOID
PsfEnableDeviceIoSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_IOEN
    );
}

/**
  Disable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfDisableDeviceMemSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~(UINT32)(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_MEMEN),
    0
    );
}

/**
  Enable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfEnableDeviceMemSpace (
  IN PSF_PORT  PsfPort
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_MEMEN
    );
}

/**
  Set device BARx address at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PsfSetDeviceBarValue (
  IN PSF_PORT  PsfPort,
  IN UINT8     BarNum,
  IN UINT32    BarValue
  )
{
  if (PSF_IS_PORT_NULL (PsfPort) || !(BarNum < 6)) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR0 + BarNum * 0x4,
    0x0,
    BarValue
    );
}

/**
  Set Function number for PSF_PORT
  Method works both in RS0 and RS3 for devices and bridges

  @param[in] PsfPort        PSF PORT data structure
  @param[in] FunctionNumber Function Number
**/
STATIC
VOID
PsfSetFunctionNumber (
  IN PSF_PORT  PsfPort,
  IN UINT32    FunctionNumber
  )
{
  if (PSF_IS_PORT_NULL (PsfPort) || FunctionNumber > 7) {
    return;
  }

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase,
    (UINT32)~B_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION,
    FunctionNumber << N_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION
    );
}

/**
  Set PMC ABASE value in PSF

  @param[in] PsfPort     PSF PMC Port
  @param[in] Address     Address for ACPI base address.
**/
VOID
PsfSetPmcAbase (
  IN  PSF_PORT     PsfPort,
  IN  UINT16       Address
  )
{
  if (PSF_IS_PORT_NULL (PsfPort)) {
    return;
  }
  //
  // Disable IOSpace before changing the address
  //
  PsfDisableDeviceIoSpace (PsfPort);
  //
  // Program ABASE in PSF PMC space BAR4
  //
  PsfSetDeviceBarValue (PsfPort, 4, Address);
  //
  // Enable IOSpace
  //
  PsfEnableDeviceIoSpace (PsfPort);
}

/**
  Get PMC PWRMBASE value from PSF

  @param[in] PsfPort     PSF PMC Port

  @retval Address     Address for PWRM base.
**/
UINTN
PsfGetPmcPwrmBase (
  IN  PSF_PORT     PsfPort
  )
{
  UINTN    Address;

  if (PSF_IS_PORT_NULL (PsfPort)) {
    return 0;
  }

  Address = PsfPort.PsfDev->Access->Read32 (
              PsfPort.PsfDev->Access,
              PsfPort.RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR0
              );

  if (Address == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "PsfGetPmcPwrmBase (): cannot read PMC PWRM BASE!\n"));
    return 0;
  }

  return Address;
}

/**
  Disable PCIe Root Port at PSF level

  @param[in] PciePortData        PCIe Root Port Data
  @param[in] PsfTable            PSF Segments Table
**/
VOID
PsfDisablePcieRootPort (
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN PSF_SEGMENT_TABLE     *PsfTable
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootPciePort;
  PSF_PORT RootRs3Port;

  if (PciePortData == NULL || PsfTable == NULL) {
    return;
  }

  SecondLevelPort.RegBase = PciePortData->SecondLevelPort;
  SecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPsfNumber);
  if (!PSF_IS_PORT_NULL (SecondLevelPort)) {
    //
    // Disable PCIe Root Port also in second level PSF segment.
    // In such 2-level configuration this is the one to which
    // PCIe Root Port is directly connected
    //
    PsfDisableBridge (SecondLevelPort);
  }
  //
  // Disable PCIe Root Ports at PSF root level
  // for RS0
  //
  RootPciePort.RegBase = PciePortData->RootPciePort;
  RootPciePort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootPciePort)) {
    PsfDisableBridge (RootPciePort);
  }

  //
  // Some projects don't support access to root port in root space 3.
  // In such event PSF_PORT is going to be NULL and we have to skip RS3 access disabling
  //
  RootRs3Port.RegBase = PciePortData->RootRs3Port;
  RootRs3Port.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootRs3Port)) {
    //
    // Disable PCIe Root Ports at PSF root level
    // for RS3
    //
    PsfRs3DisableBridge (RootRs3Port);
  }
}

/**
  Enable PCIe Root Port at PSF level

  @param[in] PciePortData        PCIe Root Port Data
  @param[in] PsfTable            PSF Segments Table
**/
VOID
PsfEnablePcieRootPort (
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN PSF_SEGMENT_TABLE     *PsfTable
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootPciePort;
  PSF_PORT RootRs3Port;

  if (PciePortData == NULL || PsfTable == NULL) {
    return;
  }

  SecondLevelPort.RegBase = PciePortData->SecondLevelPort;
  SecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPort);
  if (!PSF_IS_PORT_NULL (SecondLevelPort)) {
    //
    // Enable PCIe Root Port also in second level PSF segment.
    // In such 2-level configuration this is the one to which
    // PCIe Root Port is directly connected
    //
    PsfEnableBridge (SecondLevelPort);
  }
  //
  // Disable PCIe Root Ports at PSF root level
  // for RS0
  //
  RootPciePort.RegBase = PciePortData->RootPciePort;
  RootPciePort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootPciePort)) {
    PsfEnableBridge (RootPciePort);
  }

  //
  // Some projects don't support access to root port in root space 3.
  // In such event PSF_PORT is going to be NULL and we have to skip RS3 access enabling
  //
  RootRs3Port.RegBase = PciePortData->RootRs3Port;
  RootRs3Port.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootRs3Port)) {
    //
    // Disable PCIe Root Ports at PSF root level
    // for RS3
    //
    PsfRs3EnableBridge (RootRs3Port);
  }
}

/**
  PSF PCIe channel grant counts

  @param[in]  PsfPcieCtrlConfig        PCIe controller configuration
  @param[out] ChannelGrant             4 item array with PCIe Channel Grant Count values
**/
STATIC
VOID
PsfPcieChannelGrantCounts (
  IN  PSF_PCIE_CTRL_CONFIG  PsfPcieCtrlConfig,
  OUT UINT32                *ChannelGrant
  )
{
  switch (PsfPcieCtrlConfig) {
    case PsfPcieCtrl4xn:
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PsfPcieCtrl1x2n_2xn:
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 1;
      ChannelGrant[3] = 1;
      break;
    case PsfPcieCtrl2xn_1x2n:
      ChannelGrant[0] = 1;
      ChannelGrant[1] = 1;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PsfPcieCtrl2x2n:
      ChannelGrant[0] = 2;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = 2;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    case PsfPcieCtrl1x4n:
      ChannelGrant[0] = 4;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
    default:
      ChannelGrant[0] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[1] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[2] = DEFAULT_PCIE_GRANT_COUNT;
      ChannelGrant[3] = DEFAULT_PCIE_GRANT_COUNT;
      break;
  }
}

/**
  Set Grant Count

  @param[in]  PsfDev                   Psf Device
  @param[in]  DevGntCnt0Base           Device Grant Count Base
  @param[in]  TargetGntCntPg1Tgt0Base  Target Grant Count Base
  @param[in]  DgcrNo                   Device Grant Count Number
  @param[in]  PgTgtNo                  Target Grant Count Number
  @param[in]  GrantCount               Grant Count
**/
STATIC
VOID
PsfSetGrantCount (
  PSF_DEV      *PsfDev,
  UINT16       DevGntCnt0Base,
  UINT16       TargetGntCntPg1Tgt0Base,
  UINT32       DgcrNo,
  UINT32       PgTgtNo,
  UINT32       GrantCount
  )
{
  if (PsfDev == NULL) {
    return;
  }

  PsfDev->Access->AndThenOr32 (
    PsfDev->Access,
    (UINT16) (DevGntCnt0Base + (DgcrNo * S_PSF_PCR_PSF_X_DEV_GNTCNT_RELOAD_DGCRN_GNTCNTRELOAD)),
    (UINT32) ~B_PSF_PCR_PSF_X_DEV_GNTCNT_RELOAD_DGCRN_GNTCNTRELOAD,
    GrantCount
    );

  PsfDev->Access->AndThenOr32 (
    PsfDev->Access,
    (UINT16) (TargetGntCntPg1Tgt0Base + (PgTgtNo * S_PSF_PCR_PSF_X_TARGET_GNTCNT_RELOAD_PGN_TGTM_GNTCNTRELOAD)),
    (UINT32) ~B_PSF_PCR_PSF_X_TARGET_GNTCNT_RELOAD_PGN_TGTM_GNTCNTRELOAD,
    GrantCount
    );
}

/**
  Program PSF grant counts for PCI express PSF ports

  @param[in] GrantCountData           Grant Count Data
  @param[in] ChannelGrant             Array of Grant Counts per channel on controller
  @param[in] PsfDev                   PSF Device
  @param[in] PsfPortTable             Array of PSF_PORTs for PCIE Controller

  @retval GrantCount  GrantCount value that was programmed for given PSF Port (PsfPort)
**/
STATIC
UINT32
PsfSetPcieControllerGrantCount (
  IN PSF_GRANT_COUNT_REG_DATA    *GrantCountData,
  IN UINT32                      *ChannelGrant, // 4-elements array
  IN PSF_DEV                     *PsfDev,
  IN PSF_PORT                    *PsfPortTable // 4-elements array
  )
{
  UINT32       Channel;
  UINT32       TotalGrantCount;

  if (GrantCountData == NULL ||
      PsfDev == NULL ||
      ChannelGrant == NULL ||
      PsfPortTable == NULL) {
    return 0;
  }

  TotalGrantCount = 0;

  for (Channel = 0; Channel < GrantCountData->GrantCountNum->Channels && Channel < PSF_PCIE_CHANNELS_PER_CONTROLLER; Channel++) {
    if (ChannelGrant[Channel] == DEFAULT_PCIE_GRANT_COUNT) {
      //
      // If HW default move to next channel
      //
      continue;
    }

    PsfSetGrantCount (
      PsfDev,
      GrantCountData->DevGntCnt0Base,
      GrantCountData->TargetGntCntPg1Tgt0Base,
      GrantCountData->GrantCountNum->Data[Channel].DgcrNo,
      GrantCountData->GrantCountNum->Data[Channel].PgTgtNo,
      ChannelGrant[Channel]
      );

    if (PsfIsBridgeEnabled (PsfPortTable[Channel])) {
      TotalGrantCount += ChannelGrant[Channel];
    }
  }
  return TotalGrantCount;
}

/**
  Program PSF grant counts for PSF to PSF port

  @param[in] GrantCountData        GrantCount Data
  @param[in] GrantCount            GrantCount value that is to be programmed for given PSF Port\

  @retval GrantCount  GrantCount value that was programmed for given PSF Port (PsfPort)
**/
STATIC
UINT32
PsfSetSegmentGrantCounts (
  IN PSF_GRANT_COUNT_REG_DATA    *GrantCountRegData,
  IN UINT32                      GrantCount,
  IN PSF_DEV                     *PsfDev
  )
{
  UINT32               GrantCountMax;

  if (GrantCountRegData == NULL || PsfDev == NULL) {
    return 0;
  }
  GrantCountMax = MIN (V_PCH_PSFX_PCR_DEV_GNTCNT_MAX, GrantCount);

  PsfSetGrantCount (
    PsfDev,
    GrantCountRegData->DevGntCnt0Base,
    GrantCountRegData->TargetGntCntPg1Tgt0Base,
    GrantCountRegData->GrantCountNum->Data[0].DgcrNo,
    GrantCountRegData->GrantCountNum->Data[0].PgTgtNo,
    GrantCountMax
    );

  return GrantCountMax;
}

/**
  Program PSF grant counts for PCI express tree in PSF topology. This tree consists of PSF ports
  for PCIe controllers and PSF segments under which PCIe controllers reside indirectly
  through other PSF segments.

  @param[in] PsfPcieCtrlConfigTable   Table with PCIe controllers configuration
  @param[in] NumberOfPcieControllers  Number of PCIe controllers. This is also the size of PsfPcieCtrlConfig table
  @param[in] PsfTopology              PSF Topology for which grant counts are to be programmed
  @param[in] PsfTable                 PSF Segments Table
  @param[in] PcieRpPorts              Table of PSF_PORT for Pcie Root Ports

  @retval GrantCount  GrantCount value that was programmed for given PSF Port (PsfPort)
**/
STATIC
UINT32
PsfTopologyConfigurePcieGrantCounts (
  IN PSF_PCIE_CTRL_CONFIG       *PsfPcieCtrlConfigTable,
  IN UINT32                     NumberOfPcieControllers,
  IN CONST PSF_TOPOLOGY         *PsfTopology,
  IN PSF_SEGMENT_TABLE          *PsfTable,
  IN PSF_REG_BASE               *PcieRpPorts
  )
{
  UINT32               GrantCount;
  CONST PSF_TOPOLOGY   *ChildSegment;
  UINT32               ChannelGrant[PSF_PCIE_CHANNELS_PER_CONTROLLER];
  PSF_DEV              *PsfDev;
  PSF_PORT             PsfPortsInPcieController[PSF_PCIE_CHANNELS_PER_CONTROLLER];
  UINT32               Index;

  if (PsfPcieCtrlConfigTable == NULL || PsfTopology == NULL || PsfTable == NULL) {
    return 0;
  }
  GrantCount = 0;

  if (PsfTopology->PortType == PsfPcieCtrlPort && PsfTopology->PortData.PcieCtrlIndex < NumberOfPcieControllers) {
    PsfPcieChannelGrantCounts (PsfPcieCtrlConfigTable[PsfTopology->PortData.PcieCtrlIndex], &ChannelGrant[0]);

    for (Index = 0; Index < PSF_PCIE_CHANNELS_PER_CONTROLLER; Index++) {
      PsfPortsInPcieController[Index].RegBase = PcieRpPorts[PsfTopology->PortData.PcieCtrlIndex * PSF_PCIE_CHANNELS_PER_CONTROLLER + Index].RegBase;
      PsfPortsInPcieController[Index].PsfDev = PsfGetDev (PsfTable, PcieRpPorts[PsfTopology->PortData.PcieCtrlIndex * PSF_PCIE_CHANNELS_PER_CONTROLLER + Index].PsfNumber);
    }
    PsfDev = PsfGetDev (PsfTable, PsfTopology->PsfPort.PsfId);
    if (PsfDev == NULL) {
      return 0;
    }
    GrantCount = PsfSetPcieControllerGrantCount (
                   PsfTopology->GrantCountData,
                   ChannelGrant,
                   PsfDev,
                   PsfPortsInPcieController
                   );
  } else if (PsfTopology->PortType == PsfToPsfPort) {

    ChildSegment = PsfTopology->Child;

    while (!PSF_IS_TOPO_PORT_NULL (ChildSegment->PsfPort)) {
      GrantCount += PsfTopologyConfigurePcieGrantCounts (
                      PsfPcieCtrlConfigTable,
                      NumberOfPcieControllers,
                      ChildSegment,
                      PsfTable,
                      PcieRpPorts
                      );
      ChildSegment++;
    }

    if (!PSF_IS_TOPO_PORT_NULL (PsfTopology->PsfPort) && GrantCount > 0) {
      PsfDev = PsfGetDev (PsfTable, PsfTopology->PsfPort.PsfId);
      if (PsfDev == NULL) {
        return 0;
      }
      GrantCount = PsfSetSegmentGrantCounts (
                     PsfTopology->GrantCountData,
                     GrantCount,
                     PsfDev
                     );
    }
  }
  return GrantCount;
}

/**
  Program PSF grant counts for PCI express depending on controllers configuration

  @param[in] PsfPcieCtrlConfigTable   Table with PCIe controllers configuration
  @param[in] NumberOfPcieControllers  Number of PCIe controllers. This is also the size of PsfPcieCtrlConfig table
  @param[in] PsfTopology              PSF Topology for which grant counts are to be programmed
  @param[in] PsfTable                 PSF Segments Table
  @param[in] PcieRpPorts              PSF Root Ports table
**/
VOID
PsfConfigurePcieGrantCounts (
  IN PSF_PCIE_CTRL_CONFIG       *PsfPcieCtrlConfigTable,
  IN UINT32                     NumberOfPcieControllers,
  IN CONST PSF_TOPOLOGY         *PsfTopology,
  IN PSF_SEGMENT_TABLE          *PsfTable,
  IN PSF_REG_BASE               *PcieRpPorts
  )
{
  DEBUG ((DEBUG_INFO, "PsfConfigurePcieGrantCounts() Start\n"));
  PsfTopologyConfigurePcieGrantCounts (
    PsfPcieCtrlConfigTable,
    NumberOfPcieControllers,
    PsfTopology,
    PsfTable,
    PcieRpPorts
    );
  DEBUG ((DEBUG_INFO, "PsfConfigurePcieGrantCounts() End\n"));
}

/**
  Reload default RP PSF function number.
  The PSF register doesn't got reset after system reset, which will result in mismatch between
  PSF register setting and PCIE PCR PCD register setting. Reset PSF register in early phase
  to avoid cycle decoding confusing.

  @param[in] PciePortDataTable   PCIE Root Port Data table
  @param[in] PsfTable            Table of PSF Segments
  @param[in] PcieRpFuncNumTable  PCIE RP Function number array
  @param[in] PortNumber          Size of PcieRpFuncNumTable
**/
STATIC
VOID
PsfReloadDefaultPcieRpFunc (
  IN  PSF_PCIE_PORT_DATA_TABLE  *PciePortDataTable,
  IN  PSF_SEGMENT_TABLE         *PsfTable,
  IN  UINT8                     *PcieRpFuncNumTable,
  IN  UINT32                    PortNumber
  )
{
  UINT32   PortIndex;
  UINT32   RootPortFunction;
  PSF_PORT PsfSecondLevelPort;
  PSF_PORT PsfRootRs3Port;
  PSF_PORT PsfRootRs3FunctionConfigPort;
  PSF_PORT PsfRootFunctionConfigPort;
  PSF_PORT PsfRpFunctionConfig2ndLevelPort;

  if (PciePortDataTable == NULL || PsfTable == NULL || PcieRpFuncNumTable == NULL) {
    return;
  }

  //
  // Reload PSF function config registers to restore default function numbers for PCIe Root Ports
  //
  for (PortIndex = 0; (PortIndex < PciePortDataTable->Size) && (PortIndex < PortNumber); PortIndex++) {

    RootPortFunction = PcieRpFuncNumTable[PortIndex];

    PsfSecondLevelPort.RegBase = PciePortDataTable->Data[PortIndex].SecondLevelPort;
    PsfSecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortDataTable->Data[PortIndex].SecondLevelPsfNumber);

    if (!PSF_IS_PORT_NULL (PsfSecondLevelPort)) {
      //
      // Configure PCIe Root Port also in second level PSF segment.
      // In such 2-level configuration this is the one to which
      // PCIe Root Port is directly connected
      //
      PsfRpFunctionConfig2ndLevelPort.RegBase = PciePortDataTable->Data[PortIndex].RootPortFunctionConfig2ndLvlPort;
      PsfRpFunctionConfig2ndLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortDataTable->Data[PortIndex].SecondLevelPsfNumber);
      PsfSetFunctionNumber (
        PsfRpFunctionConfig2ndLevelPort,
        RootPortFunction
        );
    }

    //
    // Configure PSF (Root level) in RS0
    //
    PsfRootFunctionConfigPort.RegBase = PciePortDataTable->Data[PortIndex].RootFunctionConfigPort;
    PsfRootFunctionConfigPort.PsfDev = PsfGetDev (PsfTable, PciePortDataTable->Data[PortIndex].PsfNumber);
    PsfSetFunctionNumber (
      PsfRootFunctionConfigPort,
      RootPortFunction
      );

    //
    // Some projects don't support access to root port in root space 3.
    // In such event PSF_PORT is going to be NULL and we have to skip resetting
    // RS3 root port function number to default value
    //
    PsfRootRs3Port.RegBase = PciePortDataTable->Data[PortIndex].RootRs3Port;
    PsfRootRs3Port.PsfDev = PsfGetDev (PsfTable, PciePortDataTable->Data[PortIndex].PsfNumber);
    if (!PSF_IS_PORT_NULL (PsfRootRs3Port)) {
      //
      // Configure PSF (Root level) in RS3
      //
      PsfRootRs3FunctionConfigPort.RegBase = PciePortDataTable->Data[PortIndex].RootRs3FunctionConfigPort;
      PsfRootRs3FunctionConfigPort.PsfDev = PsfGetDev (PsfTable, PciePortDataTable->Data[PortIndex].PsfNumber);
      PsfSetFunctionNumber (
        PsfRootRs3FunctionConfigPort,
        RootPortFunction
        );
    }
  }
}

/**
  Assign new function number for PCIe Port Number.

  @param[in] PciePortData   PCIe Root Port Data
  @param[in] NewFunction    New Function number
  @param[in] PsfTable       Table of PSF Segments
**/
VOID
PsfSetPcieFunction (
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN UINT32                NewFunction,
  IN PSF_SEGMENT_TABLE     *PsfTable
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootFunctionConfigPort;
  PSF_PORT RootRs3FunctionConfigPort;
  PSF_PORT PsfRpFunctionConfig2ndLevelPort;

  if (PciePortData == NULL || PsfTable == NULL) {
    return;
  }

  SecondLevelPort.RegBase = PciePortData->SecondLevelPort;
  SecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPsfNumber);
  if (!PSF_IS_PORT_NULL (SecondLevelPort)) {
    //
    // Configure PCIe Root Port also in second level PSF segment.
    // In such 2-level configuration this is the one to which
    // PCIe Root Port is directly connected
    //
    PsfRpFunctionConfig2ndLevelPort.RegBase = PciePortData->RootPortFunctionConfig2ndLvlPort;
    PsfRpFunctionConfig2ndLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPsfNumber);
    PsfSetFunctionNumber (
      PsfRpFunctionConfig2ndLevelPort,
      NewFunction
      );
  }

  //
  // Configure PSF (Root level) in RS0
  //
  RootFunctionConfigPort.RegBase = PciePortData->RootFunctionConfigPort;
  RootFunctionConfigPort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  PsfSetFunctionNumber (
    RootFunctionConfigPort,
    NewFunction
    );

  //
  // Some projects don't support access to root port in root space 3.
  // In such event PSF_PORT is going to be NULL and we have to skip setting
  // new function number for RS3 root port
  //
  if (PciePortData->RootRs3Port != 0) {
    //
    // Configure PSF (Root level) in RS3
    //
    RootRs3FunctionConfigPort.RegBase = PciePortData->RootRs3FunctionConfigPort;
    RootRs3FunctionConfigPort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
    PsfSetFunctionNumber (
      RootRs3FunctionConfigPort,
      NewFunction
      );
  }
}

/**
  API for PSF_REG_DATA_TABLE
  This function extracts single PSF_REG_DATA entity from PSF_REG_DATA_TABLE
  for corresponding PsfId. Function is useful both for EOI and MCTP Reg Data.

  @param[in] RegDataTable  Table with EOI or MCTP data for supported PSFs
  @param[in] PsfId         PSF ID of Reg Data we want to get from RegDataTable
**/
STATIC
PSF_REG_DATA*
PsfGetRegData (
  IN PSF_REG_DATA_TABLE  *RegDataTable,
  IN UINT32              PsfId
)
{
  UINT32  Index;
  if (RegDataTable != NULL) {
    for (Index = 0; Index < RegDataTable->Size; Index++) {
      if (RegDataTable->Data[Index].PsfId == PsfId) {
        return &(RegDataTable->Data[Index]);
      }
    }
  }
  return NULL;
}

/**
  Add EOI Target in a given PSF

  @param[in] PsfId             PSF Device
  @param[in] TargetId          EOI Target ID
  @param[in] EoiRegData        EOI Register Data
**/
STATIC
VOID
PsfAddEoiTarget (
  PSF_DEV          *PsfDev,
  PSF_PORT_DEST_ID TargetId,
  PSF_REG_DATA     *EoiRegData
  )
{
  UINT8       NumOfEnabledTargets;
  UINT32      Data32;
  UINT8       TargetIndex;

  if (EoiRegData == NULL || PsfDev == NULL) {
    return;
  }

  //
  // Get number of enabled agents from PSF_x_PSF_MC_CONTROL_MCAST0_RS0_EOI register
  //
  Data32 = PsfDev->Access->Read32 (PsfDev->Access, EoiRegData->ControlBase);
  NumOfEnabledTargets = (UINT8) (Data32 >> N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC);

  //
  // Check if target was not already enabled
  // Targets from a different PSF segment are aggregated into single destination on
  // current PSF segment.
  //
  for (TargetIndex = 0; TargetIndex < NumOfEnabledTargets; TargetIndex++) {
    Data32 = PsfDev->Access->Read32 (PsfDev->Access, EoiRegData->TargetBase + TargetIndex * 4);
    //
    // If target already added don't add it again
    //
    if (Data32 == TargetId.RegVal) {
      return;
    }
    //
    // If target is from different PSF segment than currently being analyzed
    // it is enough that its PsfID is matching
    //
    if ((Data32 & B_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID) >> N_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID == TargetId.Fields.PsfId) {
      return;
    }
  }

  //
  // Check if next one can be added
  //
  if (NumOfEnabledTargets >= EoiRegData->MaxTargets) {
    return;
  }

  //
  // Add next target
  // Configure Multicast Destination ID register with target device on PSF.
  // Configuration must be done in next available PSF_MC_AGENT_MCAST0_RS0_TGT<x>_EOI register
  // so that other targets  are not overridden. <x> is known from the number of multicast agents
  // in Multicast Control Register. Value programmed is based on
  // PsfID, PortGroupID, PortID and ChannelID of the target
  //
  PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->TargetBase + NumOfEnabledTargets * 4, TargetId.RegVal);

  //
  // Enable new target
  // Configure PSF_x_PSF_MC_CONTROL_MCAST0_RS0_EOI, increase NumMc and set MultCEn
  //
  NumOfEnabledTargets++;
  Data32 = (NumOfEnabledTargets << N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC) | B_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_MULTCEN;
  PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->ControlBase, Data32);
}

/**
  Enable EOI Target

  @param[in] TargetId            Target ID
  @param[in] PsfSegmentTable     Table of PSF Segments
  @param[in] PsfEoiRegDataTable  Table of EOI Registry Data
**/
VOID
PsfEnableEoiTarget (
  IN PSF_PORT_DEST_ID     TargetId,
  IN PSF_SEGMENT_TABLE    *PsfSegmentTable,
  IN PSF_REG_DATA_TABLE   *EoiRegDataTable
  )
{
  PSF_REG_DATA  *EoiRegData;
  PSF_SEGMENT   *RootPsfSegment;
  PSF_SEGMENT   *PsfSegment;

  //
  // Enable EOI target in root PSF
  //
  RootPsfSegment = &PsfSegmentTable->Data[0];
  EoiRegData = PsfGetRegData (EoiRegDataTable, RootPsfSegment->Id);
  PsfAddEoiTarget (RootPsfSegment->PsfDev, TargetId, EoiRegData);

  //
  // Enable EOI target on other PSF segment if target
  // is not located on root PSF
  //
  if (TargetId.Fields.PsfId != RootPsfSegment->Id) {
    PsfSegment = PsfGetSegment (PsfSegmentTable, TargetId.Fields.PsfId);
    EoiRegData = PsfGetRegData (EoiRegDataTable, TargetId.Fields.PsfId);
    if (PsfSegment != NULL) {
      PsfAddEoiTarget (PsfSegment->PsfDev, TargetId, EoiRegData);
    }
  }
}

/**
  This function disables all EOI targets and restores back HW default configuration.
  Function is needed because MCAST_CONTROL_EOI and MCAST_TARGET_EOI registers do not
  get back to HW default in all types of resets

  @param[in]  PsfSegmentTable     Table of PSF Segments
  @param[in]  PsfEoiRegDataTable  Table of EOI Registry Data
**/
STATIC
VOID
PsfResetEoiTargets (
  IN  PSF_SEGMENT_TABLE  *PsfSegmentTable,
  IN  PSF_REG_DATA_TABLE *PsfEoiRegDataTable
  )
{
  UINT8           NumOfEnabledTargets;
  UINT8           TargetIndex;
  UINT32          Data32;
  UINT32          PsfTableIndex;
  PSF_DEV         *PsfDev;
  PSF_REG_DATA    *EoiRegData;

  if (PsfSegmentTable == NULL || PsfEoiRegDataTable == NULL) {
    return;
  }

  for (PsfTableIndex = 0; PsfTableIndex < PsfSegmentTable->Size; PsfTableIndex++) {
    EoiRegData = PsfGetRegData (PsfEoiRegDataTable, PsfSegmentTable->Data[PsfTableIndex].Id);
    if (EoiRegData != NULL) {
      //
      // Get number of enabled agents from EOI control register on this PSF
      //
      PsfDev = PsfSegmentTable->Data[PsfTableIndex].PsfDev;
      Data32 = PsfDev->Access->Read32 (PsfDev->Access, EoiRegData->ControlBase);
      NumOfEnabledTargets = (UINT8) (Data32 >> N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC);

      if (NumOfEnabledTargets > 0) {
        //
        // Disable EOI targets
        //
        PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->ControlBase, 0);

        for (TargetIndex = 0; TargetIndex < NumOfEnabledTargets; TargetIndex++) {
          //
          // Clear EOI target configuration
          //
          PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->TargetBase + TargetIndex * 4, 0);
        }
      }
    }
  }
}

/**
  Perform registers programming required for
  Management Component Transport Protocol Broadcast Cycle.

  Agent Destination Addresses are being programmed only when adequate
  PCIe root port controllers are function enabled.

  Function sets CSME PMT as a message broadcaster and programs the targets
  of the message in registers only if adequate PCIe root port controllers
  are function enabled. Conditionally, if the CPU PEG exist and is function
  enabled, DMI is also a target.

  @param[in] TargetIdTable  Array of MCTP Target IDs
  @param[in] TargetNumber   TargetIdTable array size
  @param[in] PsfTable       Table od PSF segments
  @param[in] MctpRegTable   Table of MCTP registers
  @param[in] RcOwner        RC Owner value
**/
VOID
PsfConfigureMctpCycle (
  IN  PSF_PORT_DEST_ID   *TargetIdTable,
  IN  UINT32             TargetNumber,
  IN  PSF_SEGMENT_TABLE  *PsfTable,
  IN  PSF_REG_DATA_TABLE *MctpRegTable,
  IN  UINT32             RcOwner
  )
{
  UINT32            PsfId;
  PSF_PORT_DEST_ID  TargetId;
  UINT32            Index;
  PSF_SEGMENT       *PsfSegment;
  PSF_SEGMENT       *ParentPsfSegment;
  PSF_REG_DATA      *MctpReg;
  PSF_REG_DATA      *ParentMctpReg;


  for (Index = 0; Index < MctpRegTable->Size; Index++) {
    MctpRegTable->Data[Index].NumOfEnabledTargets = 0;
  }


  //
  // Configure each target under direct PSF
  //
  for (Index = 0; Index < TargetNumber; Index++) {

    TargetId = TargetIdTable[Index];

    PsfId = TargetId.Fields.PsfId;
    MctpReg = PsfGetRegData (MctpRegTable, PsfId);
    PsfSegment = PsfGetSegment (PsfTable, PsfId);
    if (PsfSegment == NULL || MctpReg == NULL) {
      continue;
    }
    //
    // Add next target in PSF segment directly connected to this endpoint
    // Configure Multicast Destination ID register with target device on PSF.
    //
    PsfSegment->PsfDev->Access->Write32 (
      PsfSegment->PsfDev->Access,
      MctpReg->TargetBase + MctpReg->NumOfEnabledTargets * 4,
      TargetId.RegVal
      );

    MctpReg->NumOfEnabledTargets++;
  }

  //
  // Program PSF segments as targets on their parent PSFs
  //
  for (Index = 0; Index < MctpRegTable->Size; Index++) {
    MctpReg = &MctpRegTable->Data[Index];
    if ((MctpReg->NumOfEnabledTargets > 0) && MctpReg->HasParent) {
      ParentPsfSegment = PsfGetSegment (PsfTable, MctpReg->ParentPsfId);
      ParentMctpReg = PsfGetRegData (MctpRegTable, MctpReg->ParentPsfId);
      ParentPsfSegment->PsfDev->Access->Write32 (
        ParentPsfSegment->PsfDev->Access,
        ParentMctpReg->TargetBase + ParentMctpReg->NumOfEnabledTargets * 4,
        B_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTGROUPID | (MctpReg->PsfId << N_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID)
        );

      ParentMctpReg->NumOfEnabledTargets++;
    }
  }

  //
  // Configure MCTP Control with a number of targets on a given PSF segment
  // and assign MCTP ownership to PMT
  //
  for (Index = 0; Index < MctpRegTable->Size; Index++) {
    MctpReg = &MctpRegTable->Data[Index];

    if (MctpReg->NumOfEnabledTargets > 0) {
      PsfSegment = PsfGetSegment (PsfTable, MctpReg->PsfId);
      if (PsfSegment == NULL) {
        return;
      }

      PsfSegment->PsfDev->Access->Write32 (
        PsfSegment->PsfDev->Access,
        MctpReg->ControlBase,
        (MctpReg->NumOfEnabledTargets << N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC) | B_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_MULTCEN
        );

      PsfSegment->PsfDev->Access->Write32 (
        PsfSegment->PsfDev->Access,
        R_PSF_PCR_PSF_X_RC_OWNER_RS0,
        RcOwner
        );
    }
  }
}

/**
  This function disables all MCTP targets and restores back HW default for MCTP configuration.
  Function is needed because MCAST_CONTROL_MCTP and MCAST_TARGET_MCTP registers do not
  get back to HW default in all types of resets

  @param[in] PsfTable        Table of PSF segments
  @param[in] MctpRegTable    MCTP Registers table
**/
VOID
PsfResetMctpTargets (
  IN  PSF_SEGMENT_TABLE   *PsfTable,
  IN  PSF_REG_DATA_TABLE  *MctpRegTable
  )
{
  UINT32          PsfTableIndex;
  UINT32           TargetIndex;
  PSF_DEV         *PsfDev;
  PSF_REG_DATA    *MctpReg;

  for (PsfTableIndex = 0; PsfTableIndex < PsfTable->Size; PsfTableIndex++) {
    MctpReg = PsfGetRegData (MctpRegTable, PsfTable->Data[PsfTableIndex].Id);
    if (MctpReg == NULL) {
      continue;
    }
    if (MctpReg->MaxTargets > 0) {
      PsfDev = PsfTable->Data[PsfTableIndex].PsfDev;

      //
      // Clear MCTP targets
      //
      PsfDev->Access->Write32 (PsfDev->Access, MctpReg->ControlBase, 0);

      //
      // Clear MCTP bus ownership
      //
      PsfDev->Access->Write32 (PsfDev->Access, R_PSF_PCR_PSF_X_RC_OWNER_RS0, 0);

      for (TargetIndex = 0; TargetIndex < MctpReg->MaxTargets; TargetIndex++) {
        //
        // Clear MCTP target configuration
        //
        PsfDev->Access->Write32 (PsfDev->Access, MctpReg->TargetBase + TargetIndex * 4, 0);
      }
    }
  }
}

/**
  PSF early initialization.

  @param[in] EarlyInitData       Early Init Data
  @param[in] PsfTable            Table of Psf Segments
  @param[in] PcieRpFuncNumTable  PCIE RP Function number array
  @param[in] PortNumber          Size of PcieRpFuncNumTable
**/
VOID
PsfEarlyInit (
  IN  PSF_EARLY_INIT_DATA       *EarlyInitData,
  IN  PSF_SEGMENT_TABLE         *PsfTable,
  IN  UINT8                     *PcieRpFuncNumTable,
  IN  UINT32                    PortNumber
  )
{
  PsfReloadDefaultPcieRpFunc (EarlyInitData->PciePortDataTable, PsfTable, PcieRpFuncNumTable, PortNumber);
  PsfResetEoiTargets (PsfTable, EarlyInitData->EoiRegDataTable);
  if (EarlyInitData->MctpSupported) {
    PsfResetMctpTargets (PsfTable, EarlyInitData->MctpRegDataTable);
  }
}

/**
  Enable PCIE Relaxed order for Port Relaxed Ordering Table

  @param[in] PortRelaxedOrderingConfigTable        Port Relaxed Ordering Config Table
  @param[in] TableSize                             Port Relaxed Ordering Config Table Size
**/
VOID
PsfEnablePcieRelaxedOrderForTable (
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG    *PortRelaxedOrderingConfigTable,
  UINT32                                  TableSize
  )
{
  UINT32   Index;
  PSF_DEV  *PsfDev;

  if (PortRelaxedOrderingConfigTable == NULL) {
    DEBUG ((DEBUG_INFO, "PortRelaxedOrderingConfigTable is NULL\n"));
    return;
  }

  for (Index = 0; Index < TableSize; Index++) {
    PsfDev = PortRelaxedOrderingConfigTable[Index].PsfDev;
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      PortRelaxedOrderingConfigTable[Index].RegisterAddress,
      (UINT32)~(B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO | B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO),
      PortRelaxedOrderingConfigTable[Index].Fro
      );
  }
}

/**
  This function enables PCIe Relaxed Order in PSF

  @param[in] PsfRelaxedOrderRegs      struct containing tables of registers for programming of Relaxed Ordering
**/
VOID
PsfEnablePcieRelaxedOrder (
  PSF_RELAXED_ORDER_REGS* PsfRelaxedOrderRegs
  )
{
  //
  // PCH BIOS Spec Section 8.2.9
  //
  if (PsfRelaxedOrderRegs == NULL) {
    return;
  }
  PsfEnablePcieRelaxedOrderForTable (PsfRelaxedOrderRegs->RegsTable, PsfRelaxedOrderRegs->RegsTableSize);
  PsfEnablePcieRelaxedOrderForTable (PsfRelaxedOrderRegs->RegsPchTypeSpecific, PsfRelaxedOrderRegs->RegsPchTypeSpecificTableSize);
}

/**
  This function configures parity error checking for all PSF segments.

  @param[in] PsfTable       Table of supported PSF segments
**/
VOID
PsfConfigureParityChecking (
  IN PSF_SEGMENT_TABLE  *PsfTable
  )
{
  UINT32 Index;
  PSF_DEV *PsfDev;

  for(Index = 0; Index < PsfTable->Size; Index++) {
    PsfDev = PsfTable->Data[Index].PsfDev;
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG,
      (UINT32) ~0,
      B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENCMDPARDETINGRESS
    );
  }
}

/**
  For platforms that support multi-VC,
  this function configures TC-based channel mapping.

  @param[in] TcVcMappingArray      Array of 3-value structure objects,
                                   contains PsfId, RegisterAddress to write
                                   and desired Value to be written into the register.
                                   Value represents TC-VC bitmask that encodes which
                                   Source Channel (SC [BitNumber]) should be mapped to
                                   which Traffic Class (TC [BitNumber])
  @param[in] TcVcMappingArraySize  Size of the array
  @param[in] PsfTable              Table of PSF Segments available on current platform
**/
VOID
PsfConfigureTcVcMapping (
  IN  PSF_TC_VC_MAPPING  TcVcMappingArray[],
  IN  UINT32             TcVcMappingArraySize,
  IN  PSF_SEGMENT_TABLE  *PsfTable
  )
{
  UINT32   Index;
  PSF_DEV  *PsfDev;

  if (PsfTable == NULL) {
    return;
  }

  for (Index = 0; Index < TcVcMappingArraySize; Index++) {
    PsfDev = PsfGetDev (PsfTable, TcVcMappingArray[Index].PsfId);
    if (PsfDev == NULL || PsfDev->Access == NULL) {
      continue;
    }

    PsfDev->Access->Write32 (
      PsfDev->Access,
      TcVcMappingArray[Index].RegisterAddress,
      TcVcMappingArray[Index].Value
      );
  }
}

/**
  Enable VTd for one PSF_REG_BASE array

  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
  @param[in]  PsfSegmentTable                 Pointer to PSF_SEGMENT_TABLE
**/
VOID
PsfLibEnableVtd (
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize,
  PSF_SEGMENT_TABLE *PsfSegmentTable
  )
{
  UINT32  Index;
  PSF_DEV *PsfDev;

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfSegmentTable,
      RootspaceConfigsRegsArray[Index].PsfNumber
      );
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      RootspaceConfigsRegsArray[Index].RegBase,
      ~0u,
      B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN
      );
  }
}

/**
  Disable PSF address-based peer-to-peer decoding for one PSF_PORT array

  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
  @param[in]  PsfSegmentTable                 Pointer to PSF_SEGMENT_TABLE
**/
VOID
PsfDisableP2pDecoding (
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize,
  PSF_SEGMENT_TABLE *PsfSegmentTable
  )
{
  UINT32 Index;
  PSF_DEV *PsfDev;

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfSegmentTable,
      RootspaceConfigsRegsArray[Index].PsfNumber
      );
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      RootspaceConfigsRegsArray[Index].RegBase,
      (UINT32)~B_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P,
      0
      );
  }
}

/**
  Reset R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS0 and
  R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS3 registers for all PSFs
  to HW default for one array

  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
  @param[in]  PsfSegmentTable                 Pointer to PSF_SEGMENT_TABLE
**/
VOID
PsfResetRootspaceConfig (
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize,
  PSF_SEGMENT_TABLE *PsfSegmentTable
  )
{
  UINT32  Index;
  UINT32  Data32;
  PSF_DEV *PsfDev;

  Data32 = (UINT32)(
    (0 << N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P) |
    (0 << N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN)
    );

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfSegmentTable,
      RootspaceConfigsRegsArray[Index].PsfNumber
      );
    PsfDev->Access->Write32 (
      PsfDev->Access,
      RootspaceConfigsRegsArray[Index].RegBase,
      Data32
      );
  }
}

/**
  Program Deferred Write Buffer

  @param[in] PsfPort             PSF_PORT of DWB register offset
  @param[in] DwbFlushThreshold   DWB Flush Threshold value
  @param[in] NonxHCIEn           Non xHCI Enable
  @param[in] OBFFEn              OBFF Enable
  @param[in] DWBEn               DWB Enable
**/
VOID
PsfProgramDWB (
  IN  PSF_PORT    PsfPort,
  IN  UINT32      DwbFlushThreshold,
  IN  UINT32      NonxHCIEn,
  IN  UINT32      OBFFEn,
  IN  UINT32      DWBEn
  )
{
  UINT32      Data32And;
  UINT32      Data32Or;

  Data32And = (UINT32)~(
    B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD |
    B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN |
    B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN |
    B_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN
    );
  Data32Or = (
    (DwbFlushThreshold << N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_FLUSHTHRESHHOLD) |
    (NonxHCIEn         << N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_NONXHCIEN) |
    (OBFFEn            << N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_OBFFEN) |
    (DWBEn             << N_PSF_X_DWB_CONFIG_PG0_PORT0_CHANNEL0_DWBEN)
    );

  PsfPort.PsfDev->Access->AndThenOr32 (
    PsfPort.PsfDev->Access,
    PsfPort.RegBase,
    Data32And,
    Data32Or
    );
}