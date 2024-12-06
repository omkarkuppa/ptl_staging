/** @file
  Header file for PchPsfPrivateLib.

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

#ifndef _PSF_PRIVATE_LIB_H_
#define _PSF_PRIVATE_LIB_H_

#include <RegisterAccess.h>

typedef struct {
  UINT32           Id;
  REGISTER_ACCESS  *Access;
} PSF_DEV;

typedef struct {
  UINT32       Size;
  PSF_DEV      *Data;
} PSF_DEV_TABLE;

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
  );

//
// Structure describes layout of MCTP and EOI configuration registers
//
typedef struct {
  UINT32  PsfId;
  UINT16  TargetBase;
  UINT16  ControlBase;
  UINT32  MaxTargets;
  UINT8   NumOfEnabledTargets;
  BOOLEAN HasParent;
  UINT32  ParentPsfId;
} PSF_MCAST_REG_DATA;

typedef struct {
  UINT32              Size;
  PSF_MCAST_REG_DATA  *Data;
} PSF_MCAST_REG_DATA_TABLE;

#define PSF_PORT_NULL ((PSF_PORT){NULL,0})
#define PSF_IS_PORT_NULL(PsfPort) ((PsfPort.PsfDev == NULL) || (PsfPort.RegBase == 0))

//
// Structure for storing data on both PSF SideBand Port ID and
// PSF port register offset for specific device
//
typedef struct {
  PSF_DEV      *PsfDev;
  UINT16       RegBase;
} PSF_PORT;

typedef struct {
  UINT32       PsfNumber;
  UINT16       RegBase;
} PSF_REG_BASE;

typedef struct {
  UINT32    PsfNumber;
  UINT32    SecondLevelPsfNumber;
  UINT16    RootPciePort;
  UINT16    SecondLevelPort;
  UINT16    RootFunctionConfigPort;
  UINT16    RootPortFunctionConfig2ndLvlPort;
} PSF_PCIE_PORT_DATA;

typedef struct {
  UINT32              Size;
  PSF_PCIE_PORT_DATA  *Data;
} PSF_PCIE_PORT_DATA_TABLE;

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  *PsfPort
  );

/**
  Check if PCIe Root Port is enabled at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
BOOLEAN
PsfIsPcieRootPortEnabled (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  );

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  *PsfPort
  );

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  *PsfPort
  );

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  Pointer to PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  *PsfPort
  );

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
  );

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
  );

/**
  Set PMC ABASE value in PSF

  @param[in] PsfPort     Pointer to PSF PMC Port
  @param[in] Address     Address for ACPI base address.
**/
VOID
PsfSetPmcAbase (
  IN  PSF_PORT     *PsfPort,
  IN  UINT16       Address
  );

/**
  Get PMC PWRMBASE value from PSF

  @param[in] PsfPort     Pointer to PSF PMC Port

  @retval Address     Address for PWRM base.
**/
UINT32
PsfGetPmcPwrmBase (
  IN  PSF_PORT     *PsfPort
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
VOID
PsfDisablePcieRootPort (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  );

/**
  Enable PCIe Root Port at PSF level

  @param[in] PsfTable            PSF Dev Table
  @param[in] PciePortData        PCIe Root Port Data
**/
VOID
PsfEnablePcieRootPort (
  IN PSF_DEV_TABLE         *PsfTable,
  IN PSF_PCIE_PORT_DATA    *PciePortData
  );

//
// Structure for PSF Port Destination ID
//
typedef union {
  UINT32 RegVal;
  struct {
    UINT32  ChannelId   : 8;  // Channel ID
    UINT32  PortId      : 7;  // Port ID
    UINT32  PortGroupId : 1;  // Port Group ID
    UINT32  PsfId       : 8;  // PSF ID
    UINT32  Rsvd        : 7;  // Reserved
    UINT32  ChanMap     : 1;  // Channel map
  } Fields;
} PSF_PORT_DEST_ID;

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  This function configures parity error checking for all PSF devices.

  @param[in] PsfTable       Table of supported PSF devices
**/
VOID
PsfConfigureParityChecking (
  IN PSF_DEV_TABLE  *PsfTable
  );

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
  );

/**
  Enable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     Pointer to PSF PORT data structure
**/
VOID
PsfEnableDeviceMemSpace (
  IN PSF_PORT  *PsfPort
  );

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
  );

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
  );

/**
  Reset R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS0 and
  R_PCH_PSF_PCR_ROOTSPACE_CONFIG_RS3 registers for all PSFs
  to HW default for one array

  @param[in]  RootspaceConfigsRegsArray       Rootspace config regs array
  @param[in]  RootspaceConfigsRegsArraySize   Rootspace config regs array size
  @param[in]  PsfTable                        Pointer to PSF_DEV_TABLE
**/
VOID
PsfResetRootspaceConfig (
  PSF_DEV_TABLE     *PsfTable,
  PSF_REG_BASE      *RootspaceConfigsRegsArray,
  UINT32            RootspaceConfigsRegsArraySize
  );

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
  );

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
  );

/**
  Specifies the root port configuration of the
  PCIe controller. The number on the left of x
  signifies the number of root ports in the controller
  while value on the right is link width. N stands for
  the number of PCIe lanes per root port instance.
**/
typedef enum {
  PsfPcieCtrl4xn,
  PsfPcieCtrl1x2n_2xn,
  PsfPcieCtrl2xn_1x2n,
  PsfPcieCtrl2x2n,
  PsfPcieCtrl1x4n,
  PsfPcieCtrl1x16n = 7,
  PsfPcieCtrlUndefined
} PSF_PCIE_CTRL_CONFIG;

typedef struct {
  PSF_REG_BASE  PsfRegBase;
  UINT32        Value;
} REG_WITH_VALUE;

typedef struct {
  REG_WITH_VALUE *DevGc;
  REG_WITH_VALUE *TgtGcUpstream;
  REG_WITH_VALUE *TgtGcDownstream;
} GC_REG;

typedef struct {
  GC_REG GcRegs1stLvl;
  GC_REG GcRegs2ndLvl;
} GC_REGS_FOR_RP;

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
  );

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
  );

#define PSF_PCIE_CONTROLLER_MAX_CHANNELS 4
typedef struct {
  UINT8    Bifurcation;
  UINT32   RpCount;
  UINT32   LaneNum;
  UINT8    GcArray[PSF_PCIE_CONTROLLER_MAX_CHANNELS];
} BIFURCATION_TO_GRANT_COUNT;
#endif
