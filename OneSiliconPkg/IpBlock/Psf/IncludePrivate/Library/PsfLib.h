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
  REGISTER_ACCESS  *Access;
  BOOLEAN          CsmeSegment;
} PSF_DEV;

typedef struct {
  UINT32       Id;
  PSF_DEV      *PsfDev;
} PSF_SEGMENT;

typedef struct {
  UINT32       Size;
  PSF_SEGMENT  Data[];
} PSF_SEGMENT_TABLE;

#define PSF_SEGMENT_TABLE_INIT(...) \
{ \
  (sizeof((PSF_SEGMENT[]){__VA_ARGS__})/sizeof(PSF_SEGMENT)), \
  { __VA_ARGS__ } \
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
} PSF_REG_DATA;

typedef struct {
  UINT32            Size;
  PSF_REG_DATA      Data[];
} PSF_REG_DATA_TABLE;

#define PSF_REG_DATA_TABLE_INIT(...) \
{ \
  (sizeof((PSF_REG_DATA[]){__VA_ARGS__})/sizeof(PSF_REG_DATA)), \
  { __VA_ARGS__ } \
}

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
  UINT16    RootRs3Port;
  UINT16    SecondLevelPort;
  UINT16    RootFunctionConfigPort;
  UINT16    RootRs3FunctionConfigPort;
  UINT16    RootPortFunctionConfig2ndLvlPort;
} PSF_PCIE_PORT_DATA;

typedef struct {
  UINT32              Size;
  PSF_PCIE_PORT_DATA  Data[];
} PSF_PCIE_PORT_DATA_TABLE;

#define PSF_PCIE_PORT_DATA_TABLE_INIT(...) \
{ \
  (sizeof((PSF_PCIE_PORT_DATA[]){__VA_ARGS__})/sizeof(PSF_PCIE_PORT_DATA)), \
  { __VA_ARGS__ } \
}

/**
  Disable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Check if bridge (e.g. PCIe Root Port) is enabled at PSF level

  @param[in] PsfPort  PSF PORT data structure

  @retval TRUE        Bridge behind PSF Port is enabled
          FALSE       Bridge behind PSF Port is disabled
**/
BOOLEAN
PsfIsBridgeEnabled (
  IN PSF_PORT  PsfPort
  );

/**
  Enable device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfEnableDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Hide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfHideDevice (
  IN PSF_PORT  PsfPort
  );

/**
  Unhide PciCfgSpace of device at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfUnhideDevice (
  IN PSF_PORT  PsfPort
  );

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
  );

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
  );

/**
  Set PMC ABASE value in PSF

  @param[in] PsfPort     PSF PMC Port
  @param[in] Address     Address for ACPI base address.
**/
VOID
PsfSetPmcAbase (
  IN  PSF_PORT     PsfPort,
  IN  UINT16       Address
  );

/**
  Get PMC PWRMBASE value from PSF

  @param[in] PsfPort     PSF PMC Port

  @retval Address     Address for PWRM base.
**/
UINTN
PsfGetPmcPwrmBase (
  IN  PSF_PORT     PsfPort
  );

/**
  Disable PCIe Root Port at PSF level

  @param[in] PciePortData        PCIe Root Port Data
  @param[in] PsfTable            PSF Segments Table
**/
VOID
PsfDisablePcieRootPort (
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN PSF_SEGMENT_TABLE     *PsfTable
  );

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfDisableBridge (
  IN PSF_PORT  PsfPort
  );

/**
  Disable bridge (e.g. PCIe Root Port) at PSF level in RS3

  @param[in] PsfPort  PSF PORT data structure
**/
VOID
PsfRs3DisableBridge (
  IN PSF_PORT  PsfPort
  );

/**
  Enable PCIe Root Port at PSF level

  @param[in] PciePortData        PCIe Root Port Data
  @param[in] PsfTable            PSF Segments Table
**/
VOID
PsfEnablePcieRootPort (
  IN PSF_PCIE_PORT_DATA    *PciePortData,
  IN PSF_SEGMENT_TABLE     *PsfTable
  );

typedef struct {
  UINT32  DgcrNo;
  UINT32  PgTgtNo;
} PSF_GNTCNT_NUM;

typedef struct {
  UINT32         Channels;
  PSF_GNTCNT_NUM Data[];
} PSF_GRANT_COUNT_NUMBER;

#define PSF_GRANT_COUNT_NUMBER_INIT(...) \
{ \
  (sizeof((PSF_GNTCNT_NUM[]){__VA_ARGS__})/sizeof(PSF_GNTCNT_NUM)), \
  { __VA_ARGS__ } \
}
//
// Structure for storing information on location in PSF topology
// Every PSF node is identified by PsfID and PsfPortId
//
typedef struct {
  UINT8         PsfId;
  UINT8         PortId;
} PSF_TOPO_PORT;

#define PSF_TOPO_PORT_NULL ((PSF_TOPO_PORT){0, 0})
#define PSF_IS_TOPO_PORT_NULL(PsfTopoPort) (((PsfTopoPort).PsfId == 0) && ((PsfTopoPort).PortId == 0))

typedef struct {
  UINT16                  DevGntCnt0Base;
  UINT16                  TargetGntCntPg1Tgt0Base;
  PSF_GRANT_COUNT_NUMBER  *GrantCountNum;
} PSF_GRANT_COUNT_REG_DATA;

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

//
// This is optional field containing PSF port specific data
//
typedef union {
  UINT32  PcieCtrlIndex;
} PSF_TOPO_PORT_DATA;

//
// Type of enpoint connected to PSF port.
// PsfNullPort is used for ports which do not exist
//
typedef enum {
  PsfNullPort,
  PsfToPsfPort,
  PsfPcieCtrlPort
} PSF_TOPO_PORT_TYPE;

//
// Structure representing PSF port in PSF topology
// If port is of PsfToPsfPort type Child will point to the first
// port of sub PSF segment.
//
typedef struct PSF_TOPOLOGY {
  PSF_TOPO_PORT              PsfPort;
  PSF_TOPO_PORT_TYPE         PortType;
  CONST struct PSF_TOPOLOGY  *Child;
  PSF_GRANT_COUNT_REG_DATA   *GrantCountData;
  PSF_TOPO_PORT_DATA         PortData;
} PSF_TOPOLOGY;

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

  @param[in] TargetId            Target ID
  @param[in] PsfSegmentTable     Table of PSF Segments
  @param[in] PsfEoiRegDataTable  Table of EOI Registry Data
**/
VOID
PsfEnableEoiTarget (
  IN PSF_PORT_DEST_ID     TargetId,
  IN PSF_SEGMENT_TABLE    *PsfSegmentTable,
  IN PSF_REG_DATA_TABLE   *PsfEoiRegDataTable
  );

typedef struct {
  PSF_PCIE_PORT_DATA_TABLE  *PciePortDataTable;
  PSF_REG_DATA_TABLE        *EoiRegDataTable;
  PSF_REG_DATA_TABLE        *MctpRegDataTable;
  BOOLEAN                   MctpSupported;
} PSF_EARLY_INIT_DATA;

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
  );

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
  );

typedef struct {
  PSF_DEV      *PsfDev;
  UINT32       RegisterAddress;
  UINT8        Fro;
} PSF_PORT_RELAXED_ORDERING_CONFIG_REG;

typedef struct {
  UINT32                                RegsTableSize;
  UINT32                                RegsPchTypeSpecificTableSize;
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG* RegsTable;
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG* RegsPchTypeSpecific;
} PSF_RELAXED_ORDER_REGS;

/**
  This function enables PCIe Relaxed Order in PSF

  @param[in] PsfRelaxedOrderRegs      struct containing tables of registers for programming of Relaxed Ordering
**/
VOID
PsfEnablePcieRelaxedOrder (
  PSF_RELAXED_ORDER_REGS* PsfRelaxedOrderRegs
  );

/**
  Enable PCIE Relaxed order for Port Relaxed Ordering Table

  @param[in] PortRelaxedOrderingConfigTable        Port Relaxed Ordering Config Table
  @param[in] TableSize                             Port Relaxed Ordering Config Table Size
**/
VOID
PsfEnablePcieRelaxedOrderForTable (
  PSF_PORT_RELAXED_ORDERING_CONFIG_REG    *PortRelaxedOrderingConfigTable,
  UINT32                                  TableSize
  );

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
  );

/**
  This function configures parity error checking for all PSF segments.

  @param[in] PsfTable       Table of supported PSF segments
**/
VOID
PsfConfigureParityChecking (
  IN PSF_SEGMENT_TABLE  *PsfTable
  );

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
  );

/**
  Enable device Memory Space at PSF level
  Method not for bridges (e.g. PCIe Root Port)

  @param[in] PsfPort     PSF PORT data structure
**/
VOID
PsfEnableDeviceMemSpace (
  IN PSF_PORT  PsfPort
  );

typedef struct {
  UINT32  PsfId;
  UINT16  RegisterAddress;
  UINT32  Value;
} PSF_TC_VC_MAPPING;

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

#endif
