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
  This function is the method for PSF_DEV_TABLE structure
  Function returns PSF_DEV object from PSF_DEV_TABLE
  for corresponding Psf Id

  @param[in] PsfTable         PSF Devices Table
  @param[in] PsfId            PSF ID
**/
PSF_DEV*
PsfGetDev (
  IN PSF_DEV_TABLE      *PsfTable,
  IN UINT32             PsfId
  )
{
  UINT32  Index;

  if (PsfTable != NULL) {
    for (Index = 0; Index < PsfTable->Size; Index++) {
      if (PsfTable->Data[Index].Id == PsfId) {
        return &PsfTable->Data[Index];
      }
    }
  }
  return NULL;
}

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS
    );
}

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
STATIC
VOID
PsfDisableBridge (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS
    );
}

/**
  Enable Bridge (e.g. PCIe Root port) at PSF level

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
STATIC
VOID
PsfEnableBridge (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN,
    (UINT32)~B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS,
    0
    );
}

/**
  Check if bridge (e.g. PCIe Root Port) is enabled at PSF level

  @param[in] PsfPort  Pointer to PSF PORT data structure

  @retval TRUE        Bridge behind PSF Port is enabled
          FALSE       Bridge behind PSF Port is disabled
**/
STATIC
BOOLEAN
PsfIsBridgeEnabled (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return FALSE;
  }

  return (
    (PsfPort->PsfDev->Access->Read32 (
      PsfPort->PsfDev->Access,
      PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN
      ) & B_PSF_PCR_PSF_X_AGNT_T1_SHDW_PCIEN_FUNDIS) == 0);
}

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    (UINT32) ~(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_FUNDIS),
    0
    );
}

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS_CFGDIS
    );
}

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS,
    (UINT32) ~(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_CFG_DIS_CFGDIS),
    0
    );
}

/**
  Disable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
  @param[in] BarDisMask  BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfDisableDeviceBar (
  IN PSF_PORT  *PsfPort,
  IN UINT32    BarDisMask
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  if (!(BarDisMask < BIT6)) {
    DEBUG ((DEBUG_WARN, "%a - BarDisMask exceeds allowed range, max: 0x%X, given: 0x%X, abort\n", __FUNCTION__, BIT6 - 1, BarDisMask));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    BarDisMask << N_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BARXDIS
    );
}

/**
  Enable device BARs at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
  @param[in] BarEnMask   BIT0-BAR0, BIT1-BAR1,...
                         Mask corresponds to 32bit wide BARs
**/
VOID
PsfEnableDeviceBar (
  IN PSF_PORT  *PsfPort,
  IN UINT32    BarEnMask
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  if (!(BarEnMask < BIT6)) {
    DEBUG ((DEBUG_WARN, "%a - BarEnMask exceeds allowed range, max: 0x%X, given: 0x%X, abort\n", __FUNCTION__, BIT6 - 1, BarEnMask));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    (UINT32)~(BarEnMask << N_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_BARXDIS),
    0
    );
}

/**
  Disable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
**/
STATIC
VOID
PsfDisableDeviceIoSpace (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~(UINT32)(B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_IOEN),
    0
    );
}

/**
  Enable device IOSpace at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
**/
STATIC
VOID
PsfEnableDeviceIoSpace (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_IOEN
    );
}

/**
  Enable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
**/
VOID
PsfEnableDeviceMemSpace (
  IN PSF_PORT  *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN,
    ~0u,
    B_PSF_PCR_PSF_X_AGNT_T0_SHDW_PCIEN_MEMEN
    );
}

/**
  Set device BARx address at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
  @param[in] BarNum      BAR Number (0:BAR0, 1:BAR1, ...)
  @param[in] BarValue    32bit BAR value
**/
VOID
PsfSetDeviceBarValue (
  IN PSF_PORT  *PsfPort,
  IN UINT8     BarNum,
  IN UINT32    BarValue
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  if (!(BarNum < 6)) {
    DEBUG ((DEBUG_WARN, "%a - wrong BarNum, max: %d, given: %d, abort\n", __FUNCTION__, 5, BarNum));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR0 + BarNum * 0x4,
    0x0,
    BarValue
    );
}

/**
  Set Function number for PSF_PORT
  Method works both for devices and bridges

  @param[in] PsfPort        Pointer to PSF PORT data structure
  @param[in] FunctionNumber Function Number
**/
VOID
PsfSetFunctionNumber (
  IN PSF_PORT  *PsfPort,
  IN UINT32    FunctionNumber
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  if (FunctionNumber > 7) {
    DEBUG ((DEBUG_WARN, "%a - illegal FunctionNumber, max: %d, given: %d, abort\n", __FUNCTION__, 7, FunctionNumber));
    return;
  }

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase,
    (UINT32)~B_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION,
    FunctionNumber << N_PCH_PSFX_PCR_TX_AGENT_FUNCTION_CONFIG_FUNCTION
    );
}

/**
  Set PMC ABASE value in PSF

  @param[in] PsfPort     Pointer to PSF PMC Port
  @param[in] Address     Address for ACPI base address.
**/
VOID
PsfSetPmcAbase (
  IN  PSF_PORT     *PsfPort,
  IN  UINT16       Address
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
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

  @param[in] PsfPort     Pointer to PSF PMC Port

  @retval Address     Address for PWRM base.
**/
UINT32
PsfGetPmcPwrmBase (
  IN  PSF_PORT     *PsfPort
  )
{
  if ((PsfPort == NULL) || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, return 0x%X\n", __FUNCTION__, ~(UINT32)0));
    return ~(UINT32)0;
  }

  return PsfPort->PsfDev->Access->Read32 (
           PsfPort->PsfDev->Access,
           PsfPort->RegBase + R_PSF_PCR_PSF_X_AGNT_T0_SHDW_BAR0
           );
}

/**
  Disable PCIe Root Port at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
VOID
PsfDisablePcieRootPort (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootPciePort;

  if (PciePortData == NULL || PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
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
    PsfDisableBridge (&SecondLevelPort);
  }
  //
  // Disable PCIe Root Ports at PSF root level
  // for RS0
  //
  RootPciePort.RegBase = PciePortData->RootPciePort;
  RootPciePort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootPciePort)) {
    PsfDisableBridge (&RootPciePort);
  }
}

/**
  Enable PCIe Root Port at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
VOID
PsfEnablePcieRootPort (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootPciePort;

  if (PciePortData == NULL || PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  SecondLevelPort.RegBase = PciePortData->SecondLevelPort;
  SecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPsfNumber);
  if (!PSF_IS_PORT_NULL (SecondLevelPort)) {
    //
    // Enable PCIe Root Port also in second level PSF segment.
    // In such 2-level configuration this is the one to which
    // PCIe Root Port is directly connected
    //
    PsfEnableBridge (&SecondLevelPort);
  }
  //
  // Disable PCIe Root Ports at PSF root level
  // for RS0
  //
  RootPciePort.RegBase = PciePortData->RootPciePort;
  RootPciePort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootPciePort)) {
    PsfEnableBridge (&RootPciePort);
  }
}

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
BOOLEAN
PsfIsPcieRootPortEnabled (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootPciePort;
  BOOLEAN  Enabled = FALSE;

  if (PciePortData == NULL || PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return FALSE;
  }

  RootPciePort.RegBase = PciePortData->RootPciePort;
  RootPciePort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  if (!PSF_IS_PORT_NULL (RootPciePort)) {
    Enabled = PsfIsBridgeEnabled (&RootPciePort);
  }

  //
  // If Root Port have second level port,
  // Root Port should be also enabled/disabled on it.
  // Root Port should be enabled on both ports or disabled on both ports.
  //
  SecondLevelPort.RegBase = PciePortData->SecondLevelPort;
  SecondLevelPort.PsfDev = PsfGetDev (PsfTable, PciePortData->SecondLevelPsfNumber);
  if (!PSF_IS_PORT_NULL (SecondLevelPort)) {
    if (Enabled != PsfIsBridgeEnabled (&SecondLevelPort)) {
      DEBUG ((DEBUG_ERROR, "%a - PCIe Root Port should have the same Enabled or Disabled status on both PSF levels!\n", __FUNCTION__));
      ASSERT (FALSE);
    }
  }
  return Enabled;
}

/**
  Assign new function number for PCIe Port Number.

  @param[in] PsfTable       Table of PSF Devices
  @param[in] PciePortData   PCIe Root Port Data
  @param[in] NewFunction    New Function number
**/
VOID
PsfSetPcieFunction (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN UINT32                NewFunction
  )
{
  PSF_PORT SecondLevelPort;
  PSF_PORT RootFunctionConfigPort;
  PSF_PORT PsfRpFunctionConfig2ndLevelPort;

  if (PciePortData == NULL || PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
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
      &PsfRpFunctionConfig2ndLevelPort,
      NewFunction
      );
  }

  //
  // Configure PSF (Root level) in RS0
  //
  RootFunctionConfigPort.RegBase = PciePortData->RootFunctionConfigPort;
  RootFunctionConfigPort.PsfDev = PsfGetDev (PsfTable, PciePortData->PsfNumber);
  PsfSetFunctionNumber (
    &RootFunctionConfigPort,
    NewFunction
    );
}

/**
  API for PSF_MCAST_REG_DATA_TABLE
  This function extracts single PSF_MCAST_REG_DATA entity from PSF_MCAST_REG_DATA_TABLE
  for corresponding PsfId. Function is useful both for EOI and MCTP Reg Data.

  @param[in] RegDataTable  Table with EOI or MCTP data for supported PSFs
  @param[in] PsfId         PSF ID of Reg Data we want to get from RegDataTable
**/
STATIC
PSF_MCAST_REG_DATA*
PsfGetMcastRegData (
  IN PSF_MCAST_REG_DATA_TABLE  *RegDataTable,
  IN UINT32                    PsfId
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
  PSF_DEV              *PsfDev,
  PSF_PORT_DEST_ID     TargetId,
  PSF_MCAST_REG_DATA   *EoiRegData
  )
{
  UINT8       NumOfEnabledTargets;
  UINT32      Data32;
  UINT8       TargetIndex;

  if (EoiRegData == NULL || PsfDev == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
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
  }

  //
  // Check if next one can be added
  //
  if (NumOfEnabledTargets >= (UINT8)EoiRegData->MaxTargets) {
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

  @param[in] PsfTable            Table of PSF Devices
  @param[in] PsfEoiRegDataTable  Table of EOI Registry Data
  @param[in] TargetId            Target ID
**/
VOID
PsfEnableEoiTarget (
  IN PSF_DEV_TABLE              *PsfTable,
  IN PSF_MCAST_REG_DATA_TABLE   *EoiRegDataTable,
  IN PSF_PORT_DEST_ID           TargetId
  )
{
  PSF_MCAST_REG_DATA  *EoiRegData;
  PSF_MCAST_REG_DATA  *ParentEoiRegData;
  PSF_DEV             *ParentPsfDev;
  PSF_DEV             *PsfDev;
  PSF_PORT_DEST_ID    ChildPsfTargetId;

  if (PsfTable == NULL || EoiRegDataTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  //
  // Enable EOI target in parent PSF
  //
  EoiRegData = PsfGetMcastRegData (EoiRegDataTable, TargetId.Fields.PsfId);
  if (EoiRegData->HasParent) {
    ParentPsfDev = PsfGetDev (PsfTable, EoiRegData->ParentPsfId);
    if (ParentPsfDev != NULL) {
      ParentEoiRegData = PsfGetMcastRegData (EoiRegDataTable, EoiRegData->ParentPsfId);
      ChildPsfTargetId.RegVal = 0;
      ChildPsfTargetId.Fields.PsfId = TargetId.Fields.PsfId;
      PsfAddEoiTarget (ParentPsfDev, ChildPsfTargetId, ParentEoiRegData);
    }
  }

  //
  // Enable EOI target on direct PSF segment
  //
  PsfDev = PsfGetDev (PsfTable, TargetId.Fields.PsfId);
  EoiRegData = PsfGetMcastRegData (EoiRegDataTable, TargetId.Fields.PsfId);
  if (PsfDev != NULL) {
    PsfAddEoiTarget (PsfDev, TargetId, EoiRegData);
  }
}

/**
  This function disables all EOI targets and restores back HW default configuration.
  Function is needed because MCAST_CONTROL_EOI and MCAST_TARGET_EOI registers do not
  get back to HW default in all types of resets

  @param[in]  PsfTable            Table of PSF Devices
  @param[in]  PsfEoiRegDataTable  Table of EOI Registry Data
**/
VOID
PsfResetEoiTargets (
  IN  PSF_DEV_TABLE            *PsfTable,
  IN  PSF_MCAST_REG_DATA_TABLE *PsfEoiRegDataTable
  )
{
  UINT32              TargetIndex;
  UINT32              PsfTableIndex;
  PSF_DEV             *PsfDev;
  PSF_MCAST_REG_DATA  *EoiRegData;

  if (PsfTable == NULL || PsfEoiRegDataTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (PsfTableIndex = 0; PsfTableIndex < PsfTable->Size; PsfTableIndex++) {
    EoiRegData = PsfGetMcastRegData (PsfEoiRegDataTable, PsfTable->Data[PsfTableIndex].Id);
    if (EoiRegData != NULL) {
      //
      // Get number of enabled agents from EOI control register on this PSF
      //
      PsfDev = &PsfTable->Data[PsfTableIndex];
      //
      // Disable EOI targets
      //
      PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->ControlBase, 0);

      for (TargetIndex = 0; TargetIndex < EoiRegData->MaxTargets; TargetIndex++) {
        //
        // Clear EOI target configuration
        //
        PsfDev->Access->Write32 (PsfDev->Access, EoiRegData->TargetBase + TargetIndex * 4, 0);
      }
    }
  }
}

/**
  Perform registers programming required for
  Management Component Transport Protocol Broadcast Cycle.

  Agent Destination Addresses are being programmed only when adequate
  PCIe root port controllers are function enabled.

  Function sets RcOwner as a message broadcaster and programs the targets
  of the message in registers only if adequate PCIe root port controllers
  are function enabled.

  @param[in] PsfTable       Table od PSF devices
  @param[in] MctpRegTable   Table of MCTP registers
  @param[in] TargetIdTable  Array of MCTP Target IDs
  @param[in] TargetNumber   TargetIdTable array size
  @param[in] RcOwner        RC Owner value
**/
VOID
PsfConfigureMctpCycle (
  IN  PSF_DEV_TABLE            *PsfTable,
  IN  PSF_MCAST_REG_DATA_TABLE *MctpRegTable,
  IN  PSF_PORT_DEST_ID         *TargetIdTable,
  IN  UINT32                   TargetNumber,
  IN  UINT32                   RcOwner
  )
{
  UINT32              PsfId;
  PSF_PORT_DEST_ID    TargetId;
  UINT32              Index;
  PSF_DEV             *PsfDev;
  PSF_DEV             *ParentPsfDev;
  PSF_MCAST_REG_DATA  *MctpReg;
  PSF_MCAST_REG_DATA  *ParentMctpReg;

  if (PsfTable == NULL || MctpRegTable == NULL || TargetIdTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < MctpRegTable->Size; Index++) {
    MctpRegTable->Data[Index].NumOfEnabledTargets = 0;
  }

  //
  // Configure each target under direct PSF
  //
  for (Index = 0; Index < TargetNumber; Index++) {

    TargetId = TargetIdTable[Index];

    PsfId = TargetId.Fields.PsfId;
    MctpReg = PsfGetMcastRegData (MctpRegTable, PsfId);
    PsfDev = PsfGetDev (PsfTable, PsfId);
    if (PsfDev == NULL || MctpReg == NULL) {
      continue;
    }
    //
    // Add next target in PSF segment directly connected to this endpoint
    // Configure Multicast Destination ID register with target device on PSF.
    //
    PsfDev->Access->Write32 (
      PsfDev->Access,
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
      ParentPsfDev = PsfGetDev (PsfTable, MctpReg->ParentPsfId);
      ParentMctpReg = PsfGetMcastRegData (MctpRegTable, MctpReg->ParentPsfId);
      ParentPsfDev->Access->Write32 (
        ParentPsfDev->Access,
        ParentMctpReg->TargetBase + ParentMctpReg->NumOfEnabledTargets * 4,
        B_PSF_PCR_PSF_X_RC_OWNER_RSN_PORTGROUPID | (MctpReg->PsfId << N_PSF_PCR_PSF_X_RC_OWNER_RSN_PSFID)
        );

      ParentMctpReg->NumOfEnabledTargets++;
    }
  }

  //
  // Configure MCTP Control with a number of targets on a given PSF segment
  // and assign MCTP ownership to given RcOwner
  //
  for (Index = 0; Index < MctpRegTable->Size; Index++) {
    MctpReg = &MctpRegTable->Data[Index];

    if (MctpReg->NumOfEnabledTargets > 0) {
      PsfDev = PsfGetDev (PsfTable, MctpReg->PsfId);
      if (PsfDev == NULL) {
        return;
      }

      PsfDev->Access->Write32 (
        PsfDev->Access,
        MctpReg->ControlBase,
        (MctpReg->NumOfEnabledTargets << N_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_NUMMC) | B_PSF_PCR_PSF_X_PSF_MC_CONTROL_MCAST0_RSN_EOI_MULTCEN
        );

      PsfDev->Access->Write32 (
        PsfDev->Access,
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

  @param[in] PsfTable        Table of PSF devices
  @param[in] MctpRegTable    MCTP Registers table
**/
VOID
PsfResetMctpTargets (
  IN  PSF_DEV_TABLE             *PsfTable,
  IN  PSF_MCAST_REG_DATA_TABLE  *MctpRegTable
  )
{
  UINT32              PsfTableIndex;
  UINT32              TargetIndex;
  PSF_DEV             *PsfDev;
  PSF_MCAST_REG_DATA  *MctpReg;

  if (PsfTable == NULL || MctpRegTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (PsfTableIndex = 0; PsfTableIndex < PsfTable->Size; PsfTableIndex++) {
    MctpReg = PsfGetMcastRegData (MctpRegTable, PsfTable->Data[PsfTableIndex].Id);
    if (MctpReg == NULL) {
      continue;
    }
    if (MctpReg->MaxTargets > 0) {
      PsfDev = &PsfTable->Data[PsfTableIndex];

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
  Set Ingress/Egress Force Relaxed Ordering bit for PSF_REG_BASE array

  @param[in] PsfTable                   PSF Devices Table
  @param[in] PsfPortConfigRegBases      Array of PSF_REG_BASE for PSF_PORT_CONFIG_PG<N>_PORT<M> registers
  @param[in] PsfPortConfigRegBasesSize  Size of PsfPortConfigRegBases array
  @param[in] BitMask                    BitMask for Ingress or Engress FRO bit
**/
STATIC
VOID
PsfSetFroBit (
  PSF_DEV_TABLE  *PsfTable,
  PSF_REG_BASE   *PsfPortConfigRegBases,
  UINT32         PsfPortConfigRegBasesSize,
  UINT32         BitMask
  )
{
  UINT32   Index;
  PSF_DEV  *PsfDev;

  if (PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < PsfPortConfigRegBasesSize; Index++) {
    PsfDev = PsfGetDev (PsfTable, PsfPortConfigRegBases[Index].PsfNumber);
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      PsfPortConfigRegBases[Index].RegBase,
      (UINT32)~(B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO | B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO),
      BitMask
      );
  }
}

/**
  Set Ingress Force Relaxed Ordering bit for PSF_REG_BASE array

  @param[in] PsfTable                  PSF Devices Table
  @param[in] PsfPortConfigRegBases     Array of PSF_REG_BASE for PSF_PORT_CONFIG_PG<N>_PORT<M> registers
  @param[in] PsfPortConfigRegBasesSize Size of PsfPortConfigRegBases array
**/
VOID
PsfSetIngressFro (
  PSF_DEV_TABLE  *PsfTable,
  PSF_REG_BASE   *PsfPortConfigRegBases,
  UINT32         PsfPortConfigRegBasesSize
  )
{
  if (PsfTable == NULL || PsfPortConfigRegBases == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfSetFroBit (
    PsfTable,
    PsfPortConfigRegBases,
    PsfPortConfigRegBasesSize,
    B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_INGRESS_FRO
    );
}

/**
  Set Egress Force Relaxed Ordering bit for PSF_REG_BASE array

  @param[in] PsfTable                  PSF Devices Table
  @param[in] PsfPortConfigRegBases     Array of PSF_REG_BASE for PSF_PORT_CONFIG_PG<N>_PORT<M> registers
  @param[in] PsfPortConfigRegBasesSize Size of PsfPortConfigRegBases array
**/
VOID
PsfSetEgressFro (
  PSF_DEV_TABLE  *PsfTable,
  PSF_REG_BASE   *PsfPortConfigRegBases,
  UINT32         PsfPortConfigRegBasesSize
  )
{
  if (PsfTable == NULL || PsfPortConfigRegBases == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  PsfSetFroBit (
    PsfTable,
    PsfPortConfigRegBases,
    PsfPortConfigRegBasesSize,
    B_PSF_PCR_PSF_X_PSF_PORT_CONFIG_PGN_PORTM_EGRESS_FRO
    );
}

/**
  This function configures parity error checking for all PSF devices.

  @param[in] PsfTable       Table of supported PSF devices
**/
VOID
PsfConfigureParityChecking (
  IN PSF_DEV_TABLE  *PsfTable
  )
{
  UINT32 Index;
  PSF_DEV *PsfDev;

  if (PsfTable == NULL) {
    DEBUG ((DEBUG_WARN, "%a - PsfTable argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for(Index = 0; Index < PsfTable->Size; Index++) {
    PsfDev = &PsfTable->Data[Index];
    PsfDev->Access->AndThenOr32 (
      PsfDev->Access,
      R_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG,
      (UINT32) ~0,
      B_PSF_PCR_PSF_X_PSF_GLOBAL_CONFIG_ENCMDPARDETINGRESS
    );
  }
}

/**
  Enable VTd for one PSF_REG_BASE array

  @param[in]  PsfTable                        Pointer to PSF_DEV_TABLE
  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
**/
VOID
PsfLibEnableVtd (
  PSF_DEV_TABLE     *PsfTable,
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize
  )
{
  UINT32  Index;
  PSF_DEV *PsfDev;

  if (PsfTable == NULL || RootspaceConfigsRegsArray == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfTable,
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

  @param[in]  PsfTable                        Pointer to PSF_DEV_TABLE
  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
**/
VOID
PsfDisableP2pDecoding (
  PSF_DEV_TABLE     *PsfTable,
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize
  )
{
  UINT32 Index;
  PSF_DEV *PsfDev;

  if (PsfTable == NULL || RootspaceConfigsRegsArray == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfTable,
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

  @param[in]  PsfTable                        Pointer to PSF_DEV_TABLE
  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
**/
VOID
PsfResetRootspaceConfig (
  PSF_DEV_TABLE     *PsfTable,
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize
  )
{
  UINT32  Index;
  UINT32  Data32;
  PSF_DEV *PsfDev;

  if (PsfTable == NULL || RootspaceConfigsRegsArray == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  Data32 = (UINT32)(
    (0 << N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_ENADDRP2P) |
    (0 << N_PSF_PCR_PSF_X_ROOTSPACE_CONFIG_RSN_VTDEN)
    );

  for (Index = 0; Index < RootspaceConfigsRegsArraySize; Index++) {
    PsfDev = PsfGetDev (
      PsfTable,
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

  @param[in] PsfPort             Pointer to PSF_PORT of DWB register offset
  @param[in] DwbFlushThreshold   DWB Flush Threshold value
  @param[in] NonxHCIEn           Non xHCI Enable
  @param[in] OBFFEn              OBFF Enable
  @param[in] DWBEn               DWB Enable
**/
VOID
PsfProgramDWB (
  IN  PSF_PORT    *PsfPort,
  IN  UINT32      DwbFlushThreshold,
  IN  UINT32      NonxHCIEn,
  IN  UINT32      OBFFEn,
  IN  UINT32      DWBEn
  )
{
  UINT32      Data32And;
  UINT32      Data32Or;

  if (PsfPort == NULL || PSF_IS_PORT_NULL ((*PsfPort))) {
    DEBUG ((DEBUG_WARN, "%a - PsfPort argument is NULL, abort\n", __FUNCTION__));
    return;
  }

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

  PsfPort->PsfDev->Access->AndThenOr32 (
    PsfPort->PsfDev->Access,
    PsfPort->RegBase,
    Data32And,
    Data32Or
    );
}

#define PSF_GRANT_COUNT_MAX_VALUE 63
/**
  Program Grant Count Registers

  @param[in]  PsfTable               PSF_DEV_TABLE pointer
  @param[in]  RegWithValueArray      Array of REG_WITH_VALUEs to program
  @param[in]  RegWithValueArraySize  RegWithValueArray size
*/
VOID
PsfProgramGrantCountRegisters (
  IN PSF_DEV_TABLE  *PsfTable,
  IN REG_WITH_VALUE **RegWithValueArray,
  IN UINT32         RegWithValueArraySize
  )
{
  UINT32  Index;
  PSF_DEV *PsfDev;

  if (PsfTable == NULL || RegWithValueArray == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < RegWithValueArraySize; Index++) {
    if (RegWithValueArray[Index]->Value != 0) {
      PsfDev = PsfGetDev (PsfTable, RegWithValueArray[Index]->PsfRegBase.PsfNumber);
      if (RegWithValueArray[Index]->Value >= PSF_GRANT_COUNT_MAX_VALUE) {
        RegWithValueArray[Index]->Value = PSF_GRANT_COUNT_MAX_VALUE;
      }
      PsfDev->Access->Write32 (
        PsfDev->Access,
        RegWithValueArray[Index]->PsfRegBase.RegBase,
        RegWithValueArray[Index]->Value
      );
    }
  }
}

/**
  Set Grant Count values for register corresponds with Root Port number.
  Array index represents Root Port number (0 based).

  @param[in, out]  GcRegsForRpArray    Array of Grant Count registers representation for every Root Port number (0 based)
  @param[in]       GcValuesArray       Array of Grant Count values for every Root Port number (0 based)
  @param[in]       ArraySize           Size of GcRegsForRpArray which is equal to size of GcValuesArray
**/
VOID
PsfSetGrantCountForRegs (
  IN OUT  GC_REGS_FOR_RP  *GcRegsForRpArray,
  IN      UINT8           *GcValuesArray,
  IN      UINT32          ArraySize
  )
{
  UINT32 Index;

  if (GcRegsForRpArray == NULL || GcValuesArray == NULL) {
    DEBUG ((DEBUG_WARN, "%a - argument is NULL, abort\n", __FUNCTION__));
    return;
  }

  for (Index = 0; Index < ArraySize; Index++) {
    if (GcRegsForRpArray[Index].GcRegs1stLvl.DevGc != NULL) {
      GcRegsForRpArray[Index].GcRegs1stLvl.DevGc->Value += GcValuesArray[Index];
    }
    if (GcRegsForRpArray[Index].GcRegs1stLvl.TgtGcUpstream != NULL) {
      GcRegsForRpArray[Index].GcRegs1stLvl.TgtGcUpstream->Value += GcValuesArray[Index];
    }
    if (GcRegsForRpArray[Index].GcRegs1stLvl.TgtGcDownstream != NULL) {
      GcRegsForRpArray[Index].GcRegs1stLvl.TgtGcDownstream->Value += GcValuesArray[Index];
    }
    if (GcRegsForRpArray[Index].GcRegs2ndLvl.DevGc != NULL) {
      GcRegsForRpArray[Index].GcRegs2ndLvl.DevGc->Value += GcValuesArray[Index];
    }
    if (GcRegsForRpArray[Index].GcRegs2ndLvl.TgtGcUpstream != NULL) {
      GcRegsForRpArray[Index].GcRegs2ndLvl.TgtGcUpstream->Value += GcValuesArray[Index];
    }
    if (GcRegsForRpArray[Index].GcRegs2ndLvl.TgtGcDownstream != NULL) {
      GcRegsForRpArray[Index].GcRegs2ndLvl.TgtGcDownstream->Value += GcValuesArray[Index];
    }
  }
}