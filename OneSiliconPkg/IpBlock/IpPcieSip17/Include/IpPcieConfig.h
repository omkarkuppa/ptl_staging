/** @file
  Header file for Pcie IP Config Block

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

#ifndef _IP_PCIE_CONFIG_H_
#define _IP_PCIE_CONFIG_H_

#include <IpCsi.h>

#define IP_PCIE_LINK_EQ_COEFFICIENTS_MAX 10
#define IP_PCIE_LINK_EQ_PRESETS_MAX 11

#define IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17  14
#define IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP16  11
#define IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP14  5
#define IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS        IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17

#define IP_PCIE_HWEQ_COEFFS_MAX    5
#pragma pack(push, 1)
//
// Stores all platform data needed for PCIE IP Initialization
//

// @TODO: When all API ready, rename the enum member. Now we modify the name to fix the build error.
typedef enum {
  IpPcieUndefinedIntegration,
  IpPchPcie,
  IpCpuPcie,
  IpPcdPcie,
  IpTbtPcie,
  IpUpDmi,
  IpDpDmi
} IP_PCIE_INTEGRATION;

// @TODO: When all API ready, rename the enum member. Now we modify the name to fix the build error.
typedef enum {
  IpPcieSip14 = 0,
  IpPcieSip15 = 1,
  IpPcieSip16 = 2,
  IpPcieSip17 = 3,
  IpPcieSip17_2 = 4,
  IpPcieSip17_4 = 5,
  IpPcieSip17_5 = 6,
  IpPcieSip17_6 = 7,
  IpPcieSipUnknown
} IP_PCIE_SIP_VERSION;

// @TODO: When all API ready, rename the enum member. Now we modify the name to fix the build error.
typedef enum {
  IP_SE16 = 0,
  IP_SC16,
  IP_SE32,
  IP_SC20,
  IP_HSPHY
} IP_PCIE_SIP_PHY_VERSION;

typedef struct {
  UINT32  PtmPipeStageDelay[IP_MAX_PTM_STAGE_DELAY_CONFIG_REGS];
  UINT32  PtmConfig;
} IP_PTM_CONFIGURATION;

typedef struct {
  BOOLEAN  NoSnoopEnable;
  UINT32   NoSnoopScale;
  UINT32   NoSnoopValue;
  BOOLEAN  SnoopEnable;
  UINT32   SnoopScale;
  UINT32   SnoopValue;
} IP_LTR_SUBTRACTION_CONFIG;

typedef struct {
  UINT32  PreCursor;    ///< Pre-cursor coefficient
  UINT32  PostCursor;   ///< Post-cursor coefficient
} IP_PCIE_LINK_EQ_COEFFICIENTS;

typedef enum {
  IpPcieMps128B = 0,
  IpPcieMps256B,
  IpPcieMps512B,
  IpPcieMps2048B,
  IpPcieMps4096B,
  IpPcieMpsUnknown,
} IP_PCIE_MPS;

/**
  PCIe Link EQ Platform Settings
**/
typedef struct {
  UINT8                      PcieLinkEqMethod;               ///< Tells BIOS which link EQ method should be used for this port. Please refer to PCIE_LINK_EQ_METHOD for details of supported methods. Default: PcieLinkHardwareEq
  UINT8                      PcieLinkEqMode;                 ///< Tells BIOS which mode should be used for PCIe link EQ. Please refer to PCIE_LINK_EQ_MODE for details of supported modes. Default: depends on SoC
  
  /**
    Specifies if BIOS should perform local transmitter override during phase 2 of EQ process.
    If enabled value in Ph2LocalTransmitterOverridePreset must be valid.
    <b>0: Disabled</b>; 1: Enabled
  **/
  UINT8                      LocalTransmitterOverrideEnable;
  /**
    Tells BIOS how many presets/coefficients should be used during link EQ.
    Entries in the Ph3CoefficientsList or Ph3PresetList(depending on chosen mode) need to be valid up to the number specified in this field.
  **/
  UINT8                      Ph3NumberOfPresetsOrCoefficients;

  IP_PCIE_LINK_EQ_COEFFICIENTS  Ph3CoefficientsList[IP_PCIE_LINK_EQ_COEFFICIENTS_MAX];  ///< List of the PCIe coefficients to be used during equalization process. Only valid if PcieLinkEqMode is PcieLinkEqCoefficientMode
  UINT32                     Ph3PresetList[IP_PCIE_LINK_EQ_PRESETS_MAX];             ///< List of the PCIe preset values to be used during equalization process. Only valid if PcieLinkEqMode is PcieLinkEqPresetMode
  UINT32                     Ph1DownstreamPortTransmitterPreset;  ///< Specifies the value of the downstream port transmitter preset to be used during phase 1 of the equalization process. Will be applied to all lanes
  UINT32                     Ph1UpstreamPortTransmitterPreset;    ///< Specifies the value of the upstream port transmitter preset to be used during phase 1 of the equalization process. Will be applied to all lanes

  /**
    Specifies the preset that should be used during local transmitter override during phase 2 of EQ process.
    Used only if LocalTransmitterOverrideEnable is TRUE. Will be applied to all PCIe lanes of the root port.
    Valid up to the PCIE_LINK_EQ_PRESET_MAX value. <b>Default: 0<\b>
  **/
  UINT32                     Ph2LocalTransmitterOverridePreset;
  UINT32                     PCETTimer;                  ///< PCET Timer value for single PCIe speed.
  UINT8                      RemotePresetCoeffoverride;  ///< Remote Transmitter Preset Coefficient Override for single PCIe speed.
  /**
  PCIe Equalization Phase 3 Enable Control
    - <b>Disabled</b>       (0x0) : Disable phase 3 (Default)
     - Enabled               (0x1) : Enable phase 3
  **/
  UINT8                      EqPhBypass;
  UINT8                      EqPh3Bypass;
  UINT8                      LocalTxOverrideEn;
  /**
  PCIe Equalization Phase 2-3 Enable Control
  - <b>Disabled</b>       (0x0) : Disable Phase 2 - Phase 3 (Default)
  - Enabled               (0x1) : Enable Phase 2 - Phase 3
  **/
  UINT8                      EqPh23Bypass;
  UINT8                      TsLockTimer;                 ///< 8.0GT/s Training Sequence Wait Latency For Presets / Coefficients Evaluation - Gen3 TS Lock Timer
  UINT8                      Rsvd[3];
} IP_PCIE_LINK_EQ_PLATFORM_SETTINGS;

typedef struct {
  UINT32  PostCursorCoefficient;
  UINT32  CursorCoefficient;
  UINT32  PreCursorCoefficient;
} IP_PCIE_PRESET_TO_COEFFICIENT_MAPPING;

typedef struct {
  //
  // This flag indicates if project's PCIe PIPE implementation supports
  // get preset to coefficient mapping query in the phy. If this query is supported project
  // doesn't have to specify preset to coefficient mapping in the BIOS as root port
  // can get it from phy.
  //
  BOOLEAN                                PipePresetToCoefficientQuerySupported;
  UINT32                                 LowFrequencyValue; // Only valid if query to PIPE is unsupported
  UINT32                                 FullSwingValue; // Only valid if query to PIPE is unsupported
  IP_PCIE_PRESET_TO_COEFFICIENT_MAPPING  PresetToCoefficient[IP_PCIE_LINK_EQ_PRESETS_MAX]; // Only valid if query to PIPE is unsupported
  UINT32                                 PresetCoefficientEvaluationTimeMs;
} IP_PCIE_LINK_EQ_PROJECT_SETTINGS;

typedef struct {
  IP_PCIE_LINK_EQ_PLATFORM_SETTINGS  PlatformSettings;
  IP_PCIE_LINK_EQ_PROJECT_SETTINGS   ProjectSettings;
} IP_PCIE_LINK_EQ_SETTINGS;

typedef enum {
  IpPcieLinkHardwareEq = 0,  ///< Hardware is responsible for performing coefficient/preset search.
  IpPcieLinkFixedEq          ///< No coefficient/preset search is performed. Fixed values are used.
} IP_PCIE_LINK_EQ_METHOD;

typedef enum {
  IpPcieLinkEqPresetMode = 0,   ///< Use presets during PCIe link equalization
  IpPcieLinkEqCoefficientMode   ///< Use coefficients during PCIe link equalization
} IP_PCIE_LINK_EQ_MODE;

typedef enum {
  IpPcieAuto,
  IpPcieGen1,
  IpPcieGen2,
  IpPcieGen3,
  IpPcieGen4,
  IpPcieGen5
} IP_PCIE_SPEED;

typedef struct {
  UINT8  Unrs;
  UINT8  Uprs;
  UINT8  Imrs;
  UINT8  Imps;
  UINT8  Upsd;
  UINT8  Unsd;
} IP_PCIE_PAYLOAD_CONFIG;

//
// SIP version
//
#define PCIE_SIP16       0
#define PCIE_SIP17       1


typedef struct {
  UINT32                  RootPortIndexInController;
  UINT8                   RootPortsInController;
  UINT8                   LanesPerRootPort;
  BOOLEAN                 ClkReqAssigned;
  BOOLEAN                 VnnRemovalSupported;
  BOOLEAN                 VtdEnabled;
  UINT8                   BusMin;
  UINT8                   BusMax;
  BOOLEAN                 ImrEnabled;
  UINT64                  PciImrBase;
  UINT32                  PciImrSize;
  UINT8                   Rs3Bus;
  IP_PTM_CONFIGURATION    PtmConfig;
  UINT8                   InterruptPin;
  BOOLEAN                 EnablePort80Decode;
  UINT8                   TPowerOn;
  BOOLEAN                 ParityCheckEnable;
  BOOLEAN                 SquelchDirectionOverrideDisable;
  BOOLEAN                 DoNotHangOnLinkDown;
  UINT32                  AddressOffset;
  BOOLEAN                 TriggerDmiTraining;
  IP_LTR_SUBTRACTION_CONFIG  L1StandardConfig;
  IP_LTR_SUBTRACTION_CONFIG  L1p1Config;
  IP_LTR_SUBTRACTION_CONFIG  L1p2Config;
  IP_LTR_SUBTRACTION_CONFIG  LtrSubL11Npg;
  IP_PCIE_LINK_EQ_SETTINGS   Gen3EqSettings;
  IP_PCIE_LINK_EQ_SETTINGS   Gen4EqSettings;
  IP_PCIE_LINK_EQ_SETTINGS   Gen5EqSettings;
  BOOLEAN                    AtomicOpEnabled;
  BOOLEAN                    RootPortDisable;
  BOOLEAN                    ClkReqEnable;
  BOOLEAN                    LinkRetrainInProgress;
  BOOLEAN                    MultiVcEnabled;
  BOOLEAN                    ClockDcgEnable;
  UINT8                      NumOfVcs;
  UINT8                      BlockChainTimer;
  UINT8                      ChainTimersValue;
  BOOLEAN                    ConfigureMrrs;
  BOOLEAN                    ConfigureAspmAtDpDmiInit;
  BOOLEAN                    ConfigureDynamicLinkThrottling;
  IP_PCIE_PAYLOAD_CONFIG     PayLoadConfig;
  BOOLEAN                    L0StickyBits;
  BOOLEAN                    CompletionChaining;
  BOOLEAN                    PmcRequestEnable;
  UINT8                      PresentDetectStatePolicy;
  BOOLEAN                    L1PgAutoPowerGateEnable;
  BOOLEAN                    ChainBitGenerationMode;
  UINT8                      Gen3ElasticBufferMode;
  UINT8                      Gen4ElasticBufferMode;
  UINT8                      Gen5ElasticBufferMode;
  BOOLEAN                    ReceiveSbemCapDecodeEnable;
  BOOLEAN                    MarginIndependentLeftRightTimingSupport;
  UINT8                      MarginMaxTimingOffsetSupport;
  UINT8                      MarginNumberOfTimingStepsSupport;
  UINT8                      MarginMaxNumberOfLanesSupport;
  UINT8                      MarginSampleReportingMethod;
  UINT8                      MarginNumberOfVoltageStepsSupport;
  UINT8                      MarginSamplingRateTimingSupport;
  BOOLEAN                    DevEnAndLockBit;
  BOOLEAN                    DisableDmiPg;
  BOOLEAN                    ApmsL0sSupported;
  UINT8                      DefaultVc0Mapping;
} IP_PCIE_ROOT_PORT_PRIVATE_CONFIG;

/**
  PCIe device table entry entry

  The PCIe device table is being used to override PCIe device ASPM settings.
  To take effect table consisting of such entries must be instelled as PPI
  on gPchPcieDeviceTablePpiGuid.
  Last entry VendorId must be 0.
**/
typedef struct {
  UINT16  VendorId;                    ///< The vendor Id of Pci Express card ASPM setting override, 0xFFFF means any Vendor ID
  UINT16  DeviceId;                    ///< The Device Id of Pci Express card ASPM setting override, 0xFFFF means any Device ID
  UINT8   RevId;                       ///< The Rev Id of Pci Express card ASPM setting override, 0xFF means all steppings
  UINT8   BaseClassCode;               ///< The Base Class Code of Pci Express card ASPM setting override, 0xFF means all base class
  UINT8   SubClassCode;                ///< The Sub Class Code of Pci Express card ASPM setting override, 0xFF means all sub class
  UINT8   EndPointAspm;                ///< Override device ASPM (see: PCH_PCIE_ASPM_CONTROL)
                                       ///< Bit 1 must be set in OverrideConfig for this field to take effect
  UINT16  OverrideConfig;              ///< The override config bitmap (see: PCH_PCIE_OVERRIDE_CONFIG).
  /**
    The L1Substates Capability Offset Override. (applicable if bit 2 is set in OverrideConfig)
    This field can be zero if only the L1 Substate value is going to be override.
  **/
  UINT16  L1SubstatesCapOffset;
  /**
    L1 Substate Capability Mask. (applicable if bit 2 is set in OverrideConfig)
    Set to zero then the L1 Substate Capability [3:0] is ignored, and only L1s values are override.
    Only bit [3:0] are applicable. Other bits are ignored.
  **/
  UINT8   L1SubstatesCapMask;
  /**
    L1 Substate Port Common Mode Restore Time Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sCommonModeRestoreTime;
  /**
    L1 Substate Port Tpower_on Scale Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sTpowerOnScale;
  /**
    L1 Substate Port Tpower_on Value Override. (applicable if bit 2 is set in OverrideConfig)
    L1sCommonModeRestoreTime and L1sTpowerOnScale can have a valid value of 0, but not the L1sTpowerOnValue.
    If L1sTpowerOnValue is zero, all L1sCommonModeRestoreTime, L1sTpowerOnScale, and L1sTpowerOnValue are ignored,
    and only L1SubstatesCapOffset is override.
  **/
  UINT8   L1sTpowerOnValue;

  /**
    SnoopLatency bit definition
    Note: All Reserved bits must be set to 0

    BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                  When clear values in bits 9:0 will be ignored
    BITS[14:13] - Reserved
    BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                  000b - 1 ns
                  001b - 32 ns
                  010b - 1024 ns
                  011b - 32,768 ns
                  100b - 1,048,576 ns
                  101b - 33,554,432 ns
                  110b - Reserved
                  111b - Reserved
    BITS[9:0]   - Snoop Latency Value. The value in these bits will be multiplied with
                  the scale in bits 12:10

    This field takes effect only if bit 3 is set in OverrideConfig.
  **/
  UINT16  SnoopLatency;
  /**
    NonSnoopLatency bit definition
    Note: All Reserved bits must be set to 0

    BIT[15]     - When set to 1b, indicates that the values in bits 9:0 are valid
                  When clear values in bits 9:0 will be ignored
    BITS[14:13] - Reserved
    BITS[12:10] - Value in bits 9:0 will be multiplied with the scale in these bits
                  000b - 1 ns
                  001b - 32 ns
                  010b - 1024 ns
                  011b - 32,768 ns
                  100b - 1,048,576 ns
                  101b - 33,554,432 ns
                  110b - Reserved
                  111b - Reserved
    BITS[9:0]   - Non Snoop Latency Value. The value in these bits will be multiplied with
                  the scale in bits 12:10

    This field takes effect only if bit 3 is set in OverrideConfig.
  **/
  UINT16  NonSnoopLatency;

  /**
    Forces LTR override to be permanent
    The default way LTR override works is:
      rootport uses LTR override values provided by BIOS until connected device sends an LTR message, then it will use values from the message
    This settings allows force override of LTR mechanism. If it's enabled, then:
      rootport will use LTR override values provided by BIOS forever; LTR messages sent from connected device will be ignored
  **/
  UINT8  ForceLtrOverride;
  UINT8  Reserved[3];
} IP_PCIE_DEVICE_OVERRIDE;

/**
  Represent lane specific PCIe Gen3 equalization parameters.
**/
typedef struct {
  UINT8   Cm;                 ///< Coefficient C-1
  UINT8   Cp;                 ///< Coefficient C+1
  UINT8   Rsvd0[2];           ///< Reserved bytes
} IP_PCIE_EQ_PARAM;

typedef struct {
  UINT16  LtrMaxSnoopLatency;                     ///< <b>(Test)</b> Latency Tolerance Reporting, Max Snoop Latency.
  UINT16  LtrMaxNoSnoopLatency;                   ///< <b>(Test)</b> Latency Tolerance Reporting, Max Non-Snoop Latency.
  UINT8   SnoopLatencyOverrideMode;               ///< <b>(Test)</b> Latency Tolerance Reporting, Snoop Latency Override Mode.
  UINT8   SnoopLatencyOverrideMultiplier;         ///< <b>(Test)</b> Latency Tolerance Reporting, Snoop Latency Override Multiplier.
  UINT16  SnoopLatencyOverrideValue;              ///< <b>(Test)</b> Latency Tolerance Reporting, Snoop Latency Override Value.
  UINT8   NonSnoopLatencyOverrideMode;            ///< <b>(Test)</b> Latency Tolerance Reporting, Non-Snoop Latency Override Mode.
  UINT8   NonSnoopLatencyOverrideMultiplier;      ///< <b>(Test)</b> Latency Tolerance Reporting, Non-Snoop Latency Override Multiplier.
  UINT16  NonSnoopLatencyOverrideValue;           ///< <b>(Test)</b> Latency Tolerance Reporting, Non-Snoop Latency Override Value.
  UINT8   LtrConfigLock;                          ///< <b>0: Disable</b>; 1: Enable.
  UINT8   ForceLtrOverride;
  UINT8   LtrOverrideSpecCompliant;
  UINT8   RsvdByte1;
} IP_PCIE_LTR_CONFIG;


///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  IpPchPcieAspmDisabled,
  IpPchPcieAspmL0s,
  IpPchPcieAspmL1,
  IpPchPcieAspmL0sL1,
  IpPchPcieAspmAutoConfig,
  IpPchPcieAspmMax
} IP_PCH_PCIE_ASPM_CONTROL;

/**
  Refer to PCH EDS for the PCH implementation values corresponding
  to below PCI-E spec defined ranges
**/
typedef enum {
  IpPchPcieL1SubstatesDisabled,
  IpPchPcieL1SubstatesL1_1,
  IpPchPcieL1SubstatesL1_1_2,
  IpPchPcieL1SubstatesMax
} IP_PCH_PCIE_L1SUBSTATES_CONTROL;

enum IP_PCH_PCIE_MAX_PAYLOAD {
  IpPchPcieMaxPayload128 = 0,
  IpPchPcieMaxPayload256,
  IpPchPcieMaxPayload512,
  IpPchPcieMaxPayload64 = 7,
  IpPchPcieMaxPayloadMax
};

enum IP_PCIE_COMPLETION_TIMEOUT {
  IpPcieCompletionTO_Default   = 0x0,
  IpPcieCompletionTO_50_100us  = 0x01,
  IpPcieCompletionTO_1_10ms    = 0x02,
  IpPcieCompletionTO_16_55ms   = 0x05,
  IpPcieCompletionTO_65_210ms  = 0x06,
  IpPcieCompletionTO_260_900ms = 0x09,
  IpPcieCompletionTO_1_3P5s    = 0x0A,
  IpPcieCompletionTO_4_13s     = 0x0D,
  IpPcieCompletionTO_17_64s    = 0x0E,
  IpPcieCompletionTO_Disabled  = 0x010
};

typedef enum IP_PCIE_DTR_STAT {
  IpPcieDtrNotNeed    = 0x0,
  IpPcieDtrNotReady   = 0x01,
  IpPcieDtrReady      = 0x02,
  IpPcieDtrBusy       = 0x03
} IP_PCIE_DTR_STAT;

//Note: This structure will be expanded to hold all common PCIe policies between SA and PCH RootPort
typedef struct {
  UINT32  HotPlug                         :  1;   ///< Indicate whether the root port is hot plug available. <b>0: Disable</b>; 1: Enable.
  UINT32  PmSci                           :  1;   ///< Indicate whether the root port power manager SCI is enabled. 0: Disable; <b>1: Enable</b>.
  UINT32  TransmitterHalfSwing            :  1;   ///< Indicate whether the Transmitter Half Swing is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  AcsEnabled                      :  1;   ///< Indicate whether the ACS is enabled. 0: Disable; <b>1: Enable</b>.
  //
  // Error handlings
  //
  UINT32  AdvancedErrorReporting          :  1;   ///< Indicate whether the Advanced Error Reporting is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  UnsupportedRequestReport        :  1;   ///< Indicate whether the Unsupported Request Report is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  FatalErrorReport                :  1;   ///< Indicate whether the Fatal Error Report is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  NoFatalErrorReport              :  1;   ///< Indicate whether the No Fatal Error Report is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  CorrectableErrorReport          :  1;   ///< Indicate whether the Correctable Error Report is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  SystemErrorOnFatalError         :  1;   ///< Indicate whether the System Error on Fatal Error is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  SystemErrorOnNonFatalError      :  1;   ///< Indicate whether the System Error on Non Fatal Error is enabled. <b>0: Disable</b>; 1: Enable.
  UINT32  SystemErrorOnCorrectableError   :  1;   ///< Indicate whether the System Error on Correctable Error is enabled. <b>0: Disable</b>; 1: Enable.
  /**
    Max Payload Size supported, Default <b>128B</b>, see enum CPU_PCIE_MAX_PAYLOAD
    Changes Max Payload Size Supported field in Device Capabilities of the root port.
  **/
  UINT32  MaxPayload                      :  3;
  UINT32  DpcEnabled                      :  1;   ///< Downstream Port Containment. 0: Disable; <b>1: Enable</b>
  UINT32  RpDpcExtensionsEnabled          :  1;   ///< RP Extensions for Downstream Port Containment. 0: Disable; <b>1: Enable</b>
  /**
    Indicates how this root port is connected to endpoint. 0: built-in device; <b>1: slot</b>
    Built-in is incompatible with hotplug-capable ports.
  **/
  UINT32  SlotImplemented                 :  1;
  UINT32  PtmEnabled                      :  1;   ///< Enables PTM capability
  UINT32  SlotPowerLimitScale             :  2;   ///< <b>(Test)</b> Specifies scale used for slot power limit value. Leave as 0 to set to default. Default is <b>zero</b>.
  UINT32  SlotPowerLimitValue             : 12;   //< <b>(Test)</b> Specifies upper limit on power supplies by slot. Leave as 0 to set to default. Default is <b>zero</b>.
  /**
    Probe CLKREQ# signal before enabling CLKREQ# based power management.
    Conforming device shall hold CLKREQ# low until CPM is enabled. This feature attempts
    to verify CLKREQ# signal is connected by testing pad state before enabling CPM.
    In particular this helps to avoid issues with open-ended PCIe slots.
    This is only applicable to non hot-plug ports.
    <b>0: Disable</b>; 1: Enable.
  **/
  UINT32  ClkReqDetect                    :  1;
  /**
    Set if the slot supports manually operated retention latch.
  **/
  UINT32  MrlSensorPresent                :  1;
  UINT32  RelaxedOrder                    :  1;
  UINT32  NoSnoop                         :  1;
  ///
  /// This member describes whether Peer Memory Writes are enabled on the platform. <b>0: Disable</b>; 1: Enable.
  ///
  UINT32  EnablePeerMemoryWrite           :  1;
  ///
  /// This member describes whether the PCI Express Clock Gating for each root port
  /// is enabled by platform modules. <b>0: Disable</b>; 1: Enable.
  ///
  UINT32  ClockGating                     :  1;
  ///
  /// This member describes whether the PCI Express Power Gating for each root port
  /// is enabled by platform modules. <b>0: Disable</b>; 1: Enable.
  ///
  UINT32  PowerGating                     :  1;
  UINT32  PcieFomsCp                      :  3;    ///< FOM Score Board Control Policy
  UINT32  LedManagement                   :  1;   ///< Indicate whether the VMD Led Management is enabled in root port. <b>0: Disable</b>; 1: Enable.
  UINT32  RsvdBits0                       :  20;   ///< Reserved bits.
  UINT16  PhysicalSlotNumber;                     ///< Indicates the slot number for the root port. Default is the value as root port index.
  /**
    PCIe Gen3 Equalization Phase 3 Method (see CPU_PCIE_EQ_METHOD).
    0: DEPRECATED, hardware equalization; <b>1: hardware equalization</b>; 4: Fixed Coefficients
  **/
  UINT8   Gen3EqPh3Method;
  UINT8   CompletionTimeout;                      ///< The completion timeout configuration of the root port (see: CPU_PCIE_COMPLETION_TIMEOUT). Default is <b>PchPcieCompletionTO_Default</b>.
  //
  // Power Management
  //
  UINT8   Aspm;                                   ///< The ASPM configuration of the root port (see: CPU_PCIE_ASPM_CONTROL). Default is <b>PchPcieAspmAutoConfig</b>.
  UINT8   L1Substates;                            ///< The L1 Substates configuration of the root port (see: CPU_PCIE_L1SUBSTATES_CONTROL). Default is <b>PchPcieL1SubstatesL1_1_2</b>.
  UINT8   LtrEnable;                              ///< Latency Tolerance Reporting Mechanism. <b>0: Disable</b>; 1: Enable.
  UINT8   EnableCpm;                              ///< Enables Clock Power Management; even if disabled, CLKREQ# signal can still be controlled by L1 PM substates mechanism
  UINT8   PcieSpeed;                              ///< Contains speed of PCIe bus (see: PCIE_SPEED)
  UINT8   RsvdBytes2[3];                          ///< Reserved bytes
  /**
  <b>(Test)</b>
  Forces LTR override to be permanent
  The default way LTR override works is:
  rootport uses LTR override values provided by BIOS until connected device sends an LTR message, then it will use values from the message
  This settings allows force override of LTR mechanism. If it's enabled, then:
  rootport will use LTR override values provided by BIOS forever; LTR messages sent from connected device will be ignored
  **/
  IP_PCIE_LTR_CONFIG               PcieRpLtrConfig;            ///< <b>(Test)</b> Latency Tolerance Reporting Policies including LTR limit and Override
    /**
    The number of milliseconds reference code will wait for link to exit Detect state for enabled ports
    before assuming there is no device and potentially disabling the port.
    It's assumed that the link will exit detect state before root port initialization (sufficient time
    elapsed since PLTRST de-assertion) therefore default timeout is zero. However this might be useful
    if device power-up seqence is controlled by BIOS or a specific device requires more time to detect.
    In case of non-common clock enabled the default timout is 15ms.
    <b>Default: 0</b>
  **/
  UINT16  DetectTimeoutMs;
  UINT8   FormFactor; // Please check PCIE_FORM_FACTOR for supported values
  UINT8   L1Low;                                  ///< L1.LOW enable/disable. <b>0: Disable</b>; 1: Enable.
  UINT8   LinkDownGpios;
  ///
  /// <b>0: Use project default equalization settings</b>; 1: Use equalization settings from PcieLinkEqPlatformSettings
  ///
  UINT8   OverrideEqualizationDefaults;
  UINT8   PcieCfgDump;
  /**
  PCIe Equalization Phase Enable Control
  - <b>Disabled</b>       (0x0) : Disable Phase (Default)
  - Enabled               (0x1) : Enable Phase
  **/
  UINT8   EqPhBypass;
  IP_PCIE_LINK_EQ_PLATFORM_SETTINGS    PcieGen3LinkEqPlatformSettings;  ///< Global PCIe Gen3 link EQ settings that BIOS will use during PCIe link EQ for every port.
  IP_PCIE_LINK_EQ_PLATFORM_SETTINGS    PcieGen4LinkEqPlatformSettings;  ///< Global PCIe Gen4 link EQ settings that BIOS will use during PCIe link EQ for every port.
  IP_PCIE_LINK_EQ_PLATFORM_SETTINGS    PcieGen5LinkEqPlatformSettings;  ///< Global PCIe Gen5 link EQ settings that BIOS will use during PCIe link EQ for every port.
  /**
    RpFunctionSwap allows BIOS to use root port function number swapping when root port of function 0 is disabled.
    A PCIE device can have higher functions only when Function0 exists. To satisfy this requirement,
    BIOS will always enable Function0 of a device that contains more than 0 enabled root ports.
    - <b>Enabled: One of enabled root ports get assigned to Function0.</b>
      This offers no guarantee that any particular root port will be available at a specific DevNr:FuncNr location
    - Disabled: Root port that corresponds to Function0 will be kept visible even though it might be not used.
      That way rootport - to - DevNr:FuncNr assignment is constant. This option will impact ports 1, 9, 17.
      NOTE: This option will not work if ports 1, 9, 17 are fused or configured for RST PCIe storage or disabled through policy
            In other words, it only affects ports that would become hidden because they have no device connected.
      NOTE: Disabling function swap may have adverse impact on power management. This option should ONLY
            be used when each one of root ports 1, 9, 17:
        - is configured as PCIe and has correctly configured ClkReq signal, or
        - does not own any mPhy lanes (they are configured as SATA or USB)
  **/
  UINT32  RpFunctionSwap                   :  1;
  /**
    Compliance Test Mode shall be enabled when using Compliance Load Board.
    <b>0: Disable</b>, 1: Enable
  **/
  UINT32  ComplianceTestMode               :  1;
  UINT32  RsvdBits1                        : 30;   ///< Reserved bits
  ///
  /// <b>(Test)</b> This member describes whether PCIE root port Port 8xh Decode is enabled. <b>0: Disable</b>; 1: Enable.
  ///
  UINT8  TestAspmOptionalityCompliance;
  UINT8  EnablePort8xhDecode;
  UINT8  TPowerOn;
  UINT8  EpErrorReporting;
  UINT8  EnableDtr;
  //                        ///< Reserved bytes
  UINT32  VisaClockGating;
  UINT32  AutoPowerGating;
} IP_PCIE_ROOT_PORT_COMMON_CONFIG;
#pragma pack(pop)

#endif
