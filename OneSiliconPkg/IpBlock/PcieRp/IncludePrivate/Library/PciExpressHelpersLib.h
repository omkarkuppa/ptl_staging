/** @file
  Header file for PCI Express helpers library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _PCI_EXPRESS_HELPERS_LIB_H_
#define _PCI_EXPRESS_HELPERS_LIB_H_

#include <PcieConfig.h>

#define VC_CAP_ID     0x0002  // Virtual Channel capability ID
#define MFVC_CAP_ID   0x0009  // Optional Multi Functional Virtual Channel capability ID
#define VC_CAP_REG    0x04    // Virtual Channel capability register
#define VC_MAX_NUM    8
#define VC0_CTRL_REG  0x14    // VC0 control register
#define VCX_CTRL_REG_OFFSET  0xC
#define VC_ID         BIT27 | BIT26 | BIT25 | BIT24
#define VC_EN         BIT31

typedef enum {
  TpoScale2us,
  TpoScale10us,
  TpoScale100us,
  TpoScaleMax
} T_PO_SCALE;

typedef struct {
  UINT32     Value;
  T_PO_SCALE Scale;
} T_POWER_ON;

//
// This structure keeps segment:bus:device:function coordinates of a PCIe device
// in a single variable. PcieCap is offset to PCI Express capabilities.
//
typedef struct {
  UINT32 Seg : 8;
  UINT32 Bus : 8;
  UINT32 Dev : 5;
  UINT32 Func : 3;
  UINT32 PcieCap : 8;
} SBDF;

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxSnoopLatencyRequirement   : 1;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 MaxNoSnoopLatencyRequirement : 1;
  UINT32 ForceOverride                : 1;
  UINT32 Reserved                     : 7;
} LTR_OVERRIDE;

typedef struct {
  UINT8  TcVcMap[VC_MAX_NUM];
} TC_VC_MAP;

typedef enum {
  DevTypePci,
  DevTypePcieEndpoint,
  DevTypePcieUpstream,
  DevTypePcieDownstream,
  DevTypeMax
} PCI_DEV_TYPE;

typedef struct {
  UINT32                          Size;
  const PCIE_DEVICE_OVERRIDE*     Table;
} OVERRIDE_TABLE;

/**
  Enable Root Port as multicast EOI target

  @param[in] RpBase  Address of PCIe device
**/
typedef
VOID
(EFIAPI *PCIE_ENABLE_RP_EOI_TARGET) (
  IN  UINT64        RpBase
  );

typedef struct {
  PCIE_ENABLE_RP_EOI_TARGET  EnableRpEoiTarget;
} PCIE_FEATURE_CONFIGURATION_CALLBACK;

typedef struct {
  UINT8     Mrrs;
  UINT8     ConfigureMrrs;
  UINT8     ConfigureDynamicLinkThrottling;
  PCIE_FEATURE_CONFIGURATION_CALLBACK Callback;
} PCIE_FEATURE_CONFIGURATION;

/*
  Returns Tpower_on capability of device

  @param[in] DeviceBase       device's PCI segment base address
  @param[in] L1ssCapOffset    offset to L1substates capability in device's extended config space

  @retval                     structure containing Tpoweron scale and value
*/
T_POWER_ON
GetTpoCapability (
  UINT64 DeviceBase,
  UINT32 L1ssCapOffset
  );

/*
  Converts Tpower_on from value:scale notation to microseconds

  @param[in] TpoScale   T power on scale
  @param[in] TpoValue   T power on value

  @retval    number of microseconds
*/
UINT32
TpoToUs (
  UINT32 TpoScale,
  UINT32 TpoValue
  );

/**
  Converts L1 latency from enumerated register value to microseconds

  @param[in] L1Latency     latency value retrieved from register; see PCIE specification for encoding

  @retval    L1 latency converted to microseconds
**/
UINT32
L1LatencyToUs (
  UINT32 L1Latency
  );

/**
  Checks if given PCI device is capable of Latency Tolerance Reporting

  @param[in] Sbdf            device's segment:bus:device:function coordinates

  @retval TRUE if yes
**/
BOOLEAN
IsLtrCapable (
  SBDF Sbdf
  );

/**
PCIe controller Sku.
**/

typedef enum {
  EnumPchPcie = 0,
  EnumiTbtPcie = 1,
  EnumCpuPcie = 2,
  EnumPcdPcie = 3,
  EnumPciSkuMax = 4
} PCI_SKU;

/*
  Initializes the following features in rootport and devices behind it:
  Maximum Payload Size (generic)
  Rootport packet split (proprietary)
  EonOfInterrupt forwarding (proprietary)
  Common Clock Configuration (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  CCC requires link retrain, which takes a while. CCC must happen before L0s/L1 programming.
  If there was guarantee no code would access PCI while links retrain, it would be possible to skip this waiting

  @param[in] RpSegment               address of rootport on PCIe
  @param[in] RpBus                   address of rootport on PCIe
  @param[in] RpDevice                address of rootport on PCIe
  @param[in] RpFunction              address of rootport on PCIe
  @param[in] BusMin                  minimum Bus number that can be assigned below this rootport
  @param[in] BusMax                  maximum Bus number that can be assigned below this rootport
  @param[in] PciSku                  PCIe controller type
  @param[in] FeatureConfiguration    pointer to status of MRRS feature configuration
*/
VOID
RootportDownstreamConfiguration (
  UINT8                       RpSegment,
  UINT8                       RpBus,
  UINT8                       RpDevice,
  UINT8                       RpFunction,
  UINT8                       BusMin,
  UINT8                       BusMax,
  PCI_SKU                     PciSku,
  PCIE_FEATURE_CONFIGURATION  *FeatureConfiguration
  );

/*
  Configures the following power-management related features in rootport and devices behind it:
  LTR limit (generic)
  LTR override (proprietary)
  Clock Power Management (generic)
  L1 substates (generic except for the override table)
  L1.LOW substate (proprietary)
  L0s and L1 (generic)

  Generic: any code written according to PCIE Express base specification can do that.
  Proprietary: code uses registers and features that are specific to Intel silicon
  and probably only this Reference Code knows how to handle that.

  If OEM implemented generic feature enabling in his platform code or trusts Operating System
  to do it, then those features can be deleted from here.

  @param[in] RpSegment                address of rootport on PCIe
  @param[in] RpBus                    address of rootport on PCIe
  @param[in] RpDevice                 address of rootport on PCIe
  @param[in] RpFunction               address of rootport on PCIe
  @param[in] BusMin                   minimal Bus number that can be assigned below this rootport
  @param[in] BusMax                   maximum Bus number that can be assigned below this rootport
  @param[in] PcieRpCommonConfig       a pointer to Pcie Root Port Common Config
  @param[in] AspmOverrideTableSize    size of override array
  @param[in] AspmOverrideTable        array of device that need exceptions in configuration
  @param[in] FeatureConfiguration     pointer to status of PCIe features configuration
*/
VOID
RootportDownstreamPmConfiguration (
  UINT8                               RpSegment,
  UINT8                               RpBus,
  UINT8                               RpDevice,
  UINT8                               RpFunction,
  UINT8                               BusMin,
  UINT8                               BusMax,
  PCIE_ROOT_PORT_COMMON_CONFIG        *PcieRpCommonConfig,
  UINT32                              AspmOverrideTableSize,
  PCIE_DEVICE_OVERRIDE                *AspmOverrideTable,
  PCIE_FEATURE_CONFIGURATION          *FeatureConfiguration
  );

typedef struct {
  UINT32 MaxSnoopLatencyValue         : 10;
  UINT32 MaxSnoopLatencyScale         : 3;
  UINT32 MaxNoSnoopLatencyValue       : 10;
  UINT32 MaxNoSnoopLatencyScale       : 3;
  UINT32 Reserved                     : 6;
} LTR_LIMIT;

/**
Configures the following power-management related features in rootport and devices behind it:
LTR limit (generic)
L0s and L1 (generic)

Generic: any code written according to PCIE Express base specification can do that.

@param[in] RpSegment                address of rootport on PCIe
@param[in] RpBus                    address of rootport on PCIe
@param[in] RpDevice                 address of rootport on PCIe
@param[in] RpFunction               address of rootport on PCIe
@param[in] PolicyLtr                table with MaxSnoop and MaxNoSnoop Latency values to be programmed
**/
VOID
TcssRootportDownstreamPmConfiguration(
  UINT8                     RpSegment,
  UINT8                     RpBus,
  UINT8                     RpDevice,
  UINT8                     RpFunction,
  LTR_LIMIT                 PolicyLtr
);

/**
  Get max PCIe link speed supported by the root port.

  @param[in] RpBase    Root Port pci segment base address
  @return Max link speed
**/
UINT32
GetMaxLinkSpeed (
  UINT64 RpBase
  );

/**
  Checks if given PCI device is capable of Latency Tolerance Reporting

  @param[in] Sbdf            device's segment:bus:device:function coordinates

  @retval TRUE if yes
**/
BOOLEAN
IsLtrCapable (
  SBDF Sbdf
  );

/**
  Finds next PCIe (not legacy PCI) device behind given device
  If device/function number == 0x1F/0x7, this function searches for children from scratch
  Any other device number means Dev:Func contain address of last found child device
  and this function should search for next one

  @param[in]     ParentDevType  type of bridge who's partent of this bus
  @param[in,out] Sbdf           On entry: location returned previously from this function
                                          Dev:Func value of 0x1F:0x07 means search should start from the beginning
                                On exit:  if PCIe device was found, its SBDF coordinates are returned
                                          otherwise, Dev:Func are initialized to 0x1F:0x07 for convenience
  @retval TRUE when next PCIe device was found; FALSE otherwise
**/
BOOLEAN
FindNextPcieChild (
  IN     PCI_DEV_TYPE ParentDevType,
  IN OUT SBDF   *Sbdf
  );

/**
  Checks if given device is an IoAPIC

  @param[in] Base            device's base address

  @retval TRUE if it's an IoAPIC
**/
BOOLEAN
IsIoApicDevice (
  UINT64 Base
  );

/**
  Returns combination of two LTR override values
  The resulting LTR override separately chooses stricter limits for snoop and nosnoop

  @param[in] LtrA      LTR override values to be combined
  @param[in] LtrB      LTR override values to be combined

  @retval LTR override value
**/
LTR_OVERRIDE
CombineLtr (
  LTR_OVERRIDE LtrA,
  LTR_OVERRIDE LtrB
  );

/**
  Returns LTR override value for given device
  The value is extracted from Device Override table. If the device is not found,
  the returned value will have Requirement bits clear

  @param[in] Base            device's base address
  @param[in] Override        device override table

  @retval LTR override value
**/
LTR_OVERRIDE
GetOverrideLtr (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  );

/**
  In accordance with PCIe spec, devices with no LTR support are considered to have no LTR requirements
  which means infinite latency tolerance. This was found to cause problems with HID and Audio devices without LTR
  support placed behind PCIe switches with LTR support, as Switch's upstream link would be allowed to enter L1.2
  and cause large latency downstream. To work around such issues and to fix some devices with broken
  LTR reporting, Device Override table was introduced.
  This function scans PCIe tree for devices mentioned in override table and calculates the strictest
  LTR requirement between them. That value will be programmed into rootport's LTR override register

  This function expects that bridges have bus numbers already configured

  @param[in] BusLimit                       maximum Bus number that can be assigned below this port
  @param[in] Segment,Bus,Device,Function    address of currently visited PCIe device
  @param[in] AspmOverride                   Device specific ASPM policy override items

  @retval MaxLTR programmed in this device
**/
LTR_OVERRIDE
RecursiveLtrOverrideCheck (
  SBDF           Sbdf,
  OVERRIDE_TABLE *AspmOverride
  );

/**
  Extended Virtual Channel Configuration
**/
typedef struct {
  UINT16 CapOffset;
  UINT8  ExtVcCount;
} MULTI_VC_SUPPORT;

/**
  Multi VC configuration.

  @param[in]  Sbdf     SBDF
  @param[in]  BusMin   minimum bus
  @param[in]  BusMax   maximum bus
  @param[in]  TcVcMap  TC/VC map
**/
VOID
MultiVcConfiguration (
  IN SBDF       Sbdf,
  UINT8         BusMin,
  UINT8         BusMax,
  IN TC_VC_MAP  *TcVcMap
  );

/**
  Program registers to Support 10-bit Tag
  Only if given Root Port and tree below it supports 10BitTag Completer/Requester

  @param[in] Segment,Bus,Device,Function  address of currently visited PCIe device
**/
VOID
Rootport10BitTagConfiguration (
  SBDF  RpSbdf
  );

/**
  There are some devices which support L1 substates, but due to silicon bugs the corresponding register
  cannot be found by scanning PCIe capabilities. This function checks list of such devices and if one
  is found, returns its L1ss capability register offset

  @param[in] Base       base address of device
  @param[in] Override   table of devices that need override

  @retval               offset to L1ss capability register
**/
UINT16
GetOverrideL1ssCapsOffset (
  UINT64         Base,
  OVERRIDE_TABLE *Override
  );

/**
  Converts device's segment:bus:device:function coordinates to flat address

  @param[in] Sbdf   device's segment:bus:device:function coordinates

  @retval    address of device's PCI cfg space
**/
UINT64
SbdfToBase (
  SBDF Sbdf
  );

/**
  Returns information about type of device.

  @param[out] Sbdf            device's segment:bus:device:function coordinates

  @retval     one of: not a PCIe device (legacy PCI), PCIe endpoint, PCIe upstream port or PCIe downstream port (including rootport)
**/
PCI_DEV_TYPE
GetDeviceType (
  SBDF Sbdf
  );

#define MAX_SBDF_TABLE_SIZE 50 //arbitrary table size; big enough to accomodate even full length TBT chain.

#pragma pack(push, 1)
typedef struct {
  UINT32 Count;
  SBDF   Entry [MAX_SBDF_TABLE_SIZE];
} SBDF_TABLE;
#pragma pack(pop)

/**
  Attempts to assign secondary and subordinate bus numbers to uninitialized bridges in PCIe tree
  If the device is a bridge and already has bus numbers assigned, they won't be changed
  Otherwise new bus number will be assigned below this bridge.
  This function can be called from SMM, where BIOS must not modify bus numbers to prevent
  conflict with OS enumerator. To prevent this, this function returns list of bridges whose
  bus numbers were changed. All devices from that list must have buses cleared afterwards.

  @param[in] Sbdf                segment:bus:device:function coordinates of device to be added to table
  @param[in] MinBus              minimum Bus number that can be assigned below this port
  @param[in] MaxBus              maximum Bus number that can be assigned below this port
  @param[in] BridgeCleanupList   list of bridges where bus numbers were modified

  @retval    maximum bus number assigned anywhere below this device
**/
UINT8
RecursiveBusAssignment (
  SBDF       Sbdf,
  UINT8      MinBus,
  UINT8      MaxBus,
  SBDF_TABLE *BridgeCleanupList
  );

/**
  Checks if device at given address exists and is a PCI Express device.
  PCI express devices are distinguished from PCI by having Capability ID 0x10
  If the device is PCI express then its SDBF structure gets updated with pointer to
  the PCIe Capability. This is an optimization feature. It greatly decreases the number
  of bus accesses, since most features configured by this library depend on registers
  whose location is relative to PCIe capability.

  @param[in,out] Sbdf   on entry, segment:bus:device:function coordinates
                        on exit, PcieCap offset is updated

  @retval               TRUE when PCIe device exists; FALSE if it's not PCIe or there's no device at all
**/
BOOLEAN
IsPcieDevice (
  SBDF *Sbdf
  );

/**
  BIOS to support Host Rx L0s only (Host Tx L0s disabled)
  HSD:https://hsdes.intel.com/appstore/article/#/13010408261

  @param[in] RpBase   base address of rootport
**/
VOID
HostL0sTxDisable (
  UINT64  RpBase
  );
#endif // _PCI_EXPRESS_HELPERS_LIB_H_
