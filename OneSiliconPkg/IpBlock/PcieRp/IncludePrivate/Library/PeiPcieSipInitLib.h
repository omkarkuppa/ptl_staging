/** @file
  Header file for PCIe SIP init library.

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

#ifndef _PCIE_SIP_INIT_LIB_H_
#define _PCIE_SIP_INIT_LIB_H_

#include <PchPcieRpConfig.h>
#include <ThermalConfig.h>
#include <RegisterAccess.h>
#include <Library/PciExpressHelpersLib.h>

//
// PCIe Equalization implementation related definitions
//
#define B_PCIE_CFG_L01EC_TP                           0xF ///< Bits used for each transmitter preset value
#define S_PCIE_CFG_L01EC_TP                           8 ///< Each of the transmitter presets configuration fields
#define S_PCIE_CFG_PL16L01EC_TP                       4 ///< Each of the transmitter presets configuration fields
#define S_PCIE_CFG_SINGLE_LANE_PRESETS                0x20
#define R_PCIE_CFG_LFFS_OFFSET_FROM_LANE_BASE         0x1C   ///< This register offset is taken from base of the lane presets configuration
                                                             ///< starting with R_PCIE_CFG_L0P0P1PCM register.
#define N_PCIE_CFG_LFFS_FS                            8      ///< Offset of LFFS being 0x51C
#define B_PCIE_CFG_FOMS_FOMSV0                        0x000000FF
#define N_PCIE_CFG_FOMS_FOMSV0                        0
#define B_PCIE_CFG_FOMS_FOMSV1                        0x0000FF00
#define N_PCIE_CFG_FOMS_FOMSV1                        8
#define B_PCIE_CFG_FOMS_FOMSV2                        0x00FF0000
#define N_PCIE_CFG_FOMS_FOMSV2                        16
//
// Monitor signals
//
#define R_PCIE_SIP_MONITOR_CAT2                       0x2
#define R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN3       0xF
#define R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN4       0x10
#define R_PCIE_SIP_MONITOR_CAT2_BEST4COEFF_GEN5       0x11

typedef enum {
  PcieSip14 = 14,
  PcieSip15,
  PcieSip16,
  PcieSip17,
  PcieSip17_2,
  PcieSip17_4,
  PcieSip17_5,
  PcieSip17_6,
  PcieSip18
} PCIE_SIP_VERSION;

typedef enum {
  PcieUndefinedIntegration,
  PchPcie,
  CpuPcie,
  TbtPcie,
  UpDmi,
  DpDmi,
  UpDmi3
} PCIE_INTEGRATION;

typedef struct {
  UINT16  Segment;
  UINT16  Bus;
  UINT16  Device;
  UINT16  Function;
} PCIE_SBDF;

typedef struct {
  UINT16  Vid;
  UINT16  Did;
  UINT8   MaxLinkSpeed;
} PCIE_DEVICE_INFO;

typedef enum {
  SE16 = 0,
  SC16,
  SE32,
  SC20,
  HSPHY
} PCIE_SIP_PHY_VERSION;

typedef enum {
  PcieIpTrainD2,
  PcieIpTrainD3,
  PcieIpTrainD4,
  PcieIpTrainD6,
  PcieIpTrainMax
} PCIE_IP_TRAIN;

typedef struct _PCIE_ROOT_PORT_DEV  PCIE_ROOT_PORT_DEV;

/**
  Returns end point register access. Caller is responsible
  to make sure that end point is actually attached.

  @param[in]  RpDev        Root port device.
  @param[out] EpPciAccess  On output a pointer to the end point register access.

  @retval TRUE   End point access initialized successfuly.
  @retval FALSE  End point access failed to initialize.
**/
typedef
BOOLEAN
(*PCIE_SIP_GET_PCI_ACCESS) (
  IN  PCIE_ROOT_PORT_DEV  *RpDev,
  IN  PCIE_SBDF           *EpSbdf,
  OUT REGISTER_ACCESS     **EpPciAccess
  );

/**
  Called when SIP library disables the PCIe root port.

  @param[in] RpDev  Pointer to root port device which is being disabled.
**/
typedef
VOID
(*PCIE_SIP_DISABLE_ROOT_PORT) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library to check if root port's lane
  is connected to PHY.

  @param[in] RpDev        Pointer to root port device.
  @param[in] RpLaneIndex  Index of the root port's lane which to check.

  @retval TRUE   PCIe lane connected to PHY.
  @retval FALSE  PCIe lane not connected to PHY.
**/
typedef
BOOLEAN
(*PCIE_SIP_IS_PCIE_LANE_CONNECTED) (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN UINT32              RpLaneIndex
  );

/**
  Called by SIP library to enable port 80 decode.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
VOID
(*PCIE_SIP_ENABLE_PORT_80_DECODE) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library to check if CLKREQ signal is being pulled down.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
BOOLEAN
(*PCIE_SIP_IS_CLKREQ_PULLED_DOWN) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library to enable CLKREQ pin for root port.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
VOID
(*PCIE_SIP_ENABLE_CLKREQ) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library when initializing NCC root port.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
VOID
(*PCIE_SIP_INIT_NCC_PORT) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library when swapping the root port function number.

  @param[in] RpDev        Pointer to root port device.
  @param[in] NewFunction  New function number.
**/
typedef
VOID
(*PCIE_SIP_SWAP_FUNCTION_NUM) (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN UINT16              NewFunction
  );

/**
  Called by SIP library when it wants to force enable the
  PCI config decode for given root port.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
VOID
(*PCIE_SIP_FORCE_ENABLE_PCI_CONFIG) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library when PCI config access to root port
  should be disabled.

  @param[in] RpDev  Pointer to root port device.
**/
typedef
VOID
(*PCIE_SIP_DISABLE_PCI_CONFIG) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Configure Completion Timeout

  @param[in] RpDev  Pointer to root port device.

**/
typedef
VOID
(*PCIE_SIP_POROGRAM_COMPLETION_TIMEOUT) (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Called by SIP library to enable PCIE LINK DOWN functionality
  on platform specifc GPIO
**/
typedef
VOID
(*PCIE_SIP_ENABLE_GPIO_LINK_DOWN) (
  VOID
  );

/**
  A set of callbacks which are called during various points during
  root port init by SIP library.
**/
typedef struct {
  PCIE_SIP_GET_PCI_ACCESS               GetPciAccess;
  PCIE_SIP_DISABLE_ROOT_PORT            DisableRootPort;
  PCIE_SIP_IS_PCIE_LANE_CONNECTED       IsPcieLaneConnected;
  PCIE_SIP_ENABLE_PORT_80_DECODE        EnablePort80Decode;
  PCIE_SIP_IS_CLKREQ_PULLED_DOWN        IsClkReqPulledDown;
  PCIE_SIP_ENABLE_CLKREQ                EnableClkReq;
  PCIE_SIP_INIT_NCC_PORT                InitNccPort;
  PCIE_SIP_SWAP_FUNCTION_NUM            SwapFunctionNumber;
  PCIE_SIP_FORCE_ENABLE_PCI_CONFIG      ForceEnablePciConfig;
  PCIE_SIP_DISABLE_PCI_CONFIG           DisablePciConfig;
  PCIE_SIP_POROGRAM_COMPLETION_TIMEOUT  ProgramCompletionTimeoutValue;
  PCIE_SIP_ENABLE_GPIO_LINK_DOWN        EnableGpioLinkDown;
} PCIE_ROOT_PORT_CALLBACKS;

#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17  14
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP16  11
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP14  5
#define MAX_PTM_STAGE_DELAY_CONFIG_REGS        MAX_PTM_STAGE_DELAY_CONFIG_REGS_SIP17

typedef struct {
  UINT32  PtmPipeStageDelay[MAX_PTM_STAGE_DELAY_CONFIG_REGS];
  UINT32  PtmConfig;
} PTM_CONFIGURATION;

typedef struct {
  BOOLEAN  NoSnoopEnable;
  UINT32   NoSnoopScale;
  UINT32   NoSnoopValue;
  BOOLEAN  SnoopEnable;
  UINT32   SnoopScale;
  UINT32   SnoopValue;
} LTR_SUBTRACTION_CONFIG;

typedef struct {
  UINT32  PostCursorCoefficient;
  UINT32  CursorCoefficient;
  UINT32  PreCursorCoefficient;
} PCIE_PRESET_TO_COEFFICIENT_MAPPING;

typedef struct {
  //
  // This flag indicates if project's PCIe PIPE implementation supports
  // get preset to coefficient mapping query in the phy. If this query is supported project
  // doesn't have to specify preset to coefficient mapping in the BIOS as root port
  // can get it from phy.
  //
  BOOLEAN                             PipePresetToCoefficientQuerySupported;
  UINT32                              LowFrequencyValue; // Only valid if query to PIPE is unsupported
  UINT32                              FullSwingValue; // Only valid if query to PIPE is unsupported
  PCIE_PRESET_TO_COEFFICIENT_MAPPING  PresetToCoefficient[PCIE_LINK_EQ_PRESETS_MAX]; // Only valid if query to PIPE is unsupported
  UINT32                              PresetCoefficientEvaluationTimeMs;
} PCIE_LINK_EQ_PROJECT_SETTINGS;

typedef struct {
  PCIE_LINK_EQ_PLATFORM_SETTINGS  PlatformSettings;
  PCIE_LINK_EQ_PROJECT_SETTINGS   ProjectSettings;
} PCIE_LINK_EQ_SETTINGS;

typedef struct {
  UINT8  Unrs;
  UINT8  Uprs;
  UINT8  Imrs;
  UINT8  Imps;
  UINT8  Upsd;
  UINT8  Unsd;
} PCIE_PAYLOAD_CONFIG;

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
  PTM_CONFIGURATION       PtmConfig;
  UINT8                   InterruptPin;
  BOOLEAN                 EnablePort80Decode;
  UINT8                   TPowerOn;
  BOOLEAN                 ParityCheckEnable;
  BOOLEAN                 SquelchDirectionOverrideDisable;
  BOOLEAN                 DoNotHangOnLinkDown;
  UINT32                  AddressOffset;
  BOOLEAN                 TriggerDmiTraining;
  LTR_SUBTRACTION_CONFIG  L1StandardConfig;
  LTR_SUBTRACTION_CONFIG  L1p1Config;
  LTR_SUBTRACTION_CONFIG  L1p2Config;
  LTR_SUBTRACTION_CONFIG  LtrSubL11Npg;
  PCIE_LINK_EQ_SETTINGS   Gen3EqSettings;
  PCIE_LINK_EQ_SETTINGS   Gen4EqSettings;
  PCIE_LINK_EQ_SETTINGS   Gen5EqSettings;
  BOOLEAN                 AtomicOpEnabled;
  BOOLEAN                 MultiVcEnabled;
  UINT8                   NumOfVcs;
  BOOLEAN                 ConfigureRpPacketSplit;
  BOOLEAN                 ConfigureMrrs;
  BOOLEAN                 ConfigureAspmAtDpDmiInit;
  BOOLEAN                 ConfigureDynamicLinkThrottling;
  PCIE_PAYLOAD_CONFIG     PayLoadConfig;
  BOOLEAN                 L0StickyBits;
  BOOLEAN                 CompletionChaining;
  BOOLEAN                 PmcRequestEnable;
  UINT8                   PresentDetectStatePolicy;
  BOOLEAN                 L1PgAutoPowerGateEnable;
  BOOLEAN                 ChainBitGenerationMode;
  UINT8                   Gen3ElasticBufferMode;
  UINT8                   Gen4ElasticBufferMode;
  UINT8                   Gen5ElasticBufferMode;
  BOOLEAN                 ReceiveSbemCapDecodeEnable;
  BOOLEAN                 MarginIndependentLeftRightTimingSupport;
  UINT8                   MarginMaxTimingOffsetSupport;
  UINT8                   MarginNumberOfTimingStepsSupport;
  UINT8                   MarginMaxNumberOfLanesSupport;
  UINT8                   MarginSampleReportingMethod;
  UINT8                   MarginNumberOfVoltageStepsSupport;
  UINT8                   MarginSamplingRateTimingSupport;
  BOOLEAN                 DisableDmiPg;
  BOOLEAN                 ApmsL0sSupported;
  UINT8                   DefaultVc0Mapping;
  UINT8                   IncreaseChainTimersValue;
  BOOLEAN                 ClockDcgEnable;
} PCIE_ROOT_PORT_PRIVATE_CONFIG;

typedef struct {
  BOOLEAN  RootPortDisable;
  BOOLEAN  ClkReqEnable;
  BOOLEAN  LinkRetrainInProgress;
} PCIE_ROOT_PORT_STATUS;

struct _PCIE_ROOT_PORT_DEV {
  PCIE_SIP_VERSION  SipVersion;
  PCIE_INTEGRATION  Integration;
  PCIE_IP_TRAIN     IpTrain;

  //
  // This member describes the root port location on PCI.
  // Note that depending on the integration root port might
  // not be accessible via the PCI config so library will not
  // attempt to construct memory addresses from this Sbdf.
  //
  PCIE_SBDF         Sbdf;

  //
  // Unique ID used to identify root port in debug prints
  //
  UINT32            Id;

  //
  // TRUE if this root port device is a main controller root port.
  // For instance in controllers with 4 root ports root port 0 is a main
  // root port which implements registers controlling behavior of the whole
  // controller.
  //
  BOOLEAN           IsController;

  //
  //  PCIe root port private configuration.
  //
  PCIE_ROOT_PORT_PRIVATE_CONFIG  PrivateConfig;

  //
  // PCIe root port status. This structure holds internal PCIe RP
  // status that is used by SIP library during init flows.
  //
  PCIE_ROOT_PORT_STATUS  Status;

  //
  // Pointer to PCIe public configuration.
  //
  PCH_PCIE_CONFIG  *PcieConfig;

  //
  // PCIe root port public configuration.
  //
  PCH_PCIE_ROOT_PORT_CONFIG  *PcieRootPortConfig;

  //
  // Set of callbacks to SoC.
  //
  PCIE_ROOT_PORT_CALLBACKS  Callbacks;

  //
  // This member allows the SIP library to access
  // the IP PCI config register bank. Please note that
  // depending on the integration the PCI config bank might
  // not be accessible via the standard PCI access mechanism
  // such as PCI Express BAR.
  //
  REGISTER_ACCESS  *PciCfgAccess;

  //
  // This member allows the SIP library to access the
  // IP PCI config registers by sending the SBI message
  // on the sideband.
  //
  REGISTER_ACCESS  *PciSbiMsgCfgAccess;

  //
  // This member allows the SIP library to access the
  // IP memory registers via the SBI message on the
  // sideband.
  //
  REGISTER_ACCESS  *PciSbiMsgMemAccess;

  //
  // This member allows the SIP library to access the
  // IP private configuration registers.
  //
  REGISTER_ACCESS  *PciPcrAccess;
  PCIE_SIP_PHY_VERSION  SipPhyVersion;
};

typedef struct _PCIE_ROOT_PORT_LIST  PCIE_ROOT_PORT_LIST;

/**
  Get next root port in the list. When called this
  function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_NEXT_ROOT_PORT) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Get next controller in the list. When called this
  function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next controller device.

  @retval EFI_SUCCESS    Next controller device found.
  @retval EFI_NOT_FOUND  Didn't find next controller device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_NEXT_CONTROLLER) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Get next root port belonging to current controller.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_NEXT_ROOT_PORT_IN_THIS_CONTROLLER) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Get controller device to which current root port
  belongs. When called this function does not
  progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to controller device.

  @retval EFI_SUCCESS    Controller device found.
  @retval EFI_NOT_FOUND  Didn't find controller device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_THIS_CONTROLLER) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Get root port device located on next PCI device number.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_ROOT_PORT_ON_NEXT_DEVICE) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Get root port device located on next PCI function number.
  When called this function progresses iteration.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_GET_ROOT_PORT_ON_NEXT_FUNCTION) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  Reset the iteration to the first root port.

  @param[in]  RpList  Pointer to root port list.
  @param[out] RpDev   Pointer to next root port device.

  @retval EFI_SUCCESS    Next root port device found.
  @retval EFI_NOT_FOUND  Didn't find next root port device.
**/
typedef
EFI_STATUS
(*PCIE_LIST_RESET_TO_FIRST) (
  IN  PCIE_ROOT_PORT_LIST  *RpList,
  OUT PCIE_ROOT_PORT_DEV   **RpDev
  );

/**
  This structure implements an interface which SIP library will use to initialize
  a collection of root ports at a single time.
**/
struct _PCIE_ROOT_PORT_LIST {
  PCIE_LIST_GET_NEXT_ROOT_PORT                     GetNextRootPort;
  PCIE_LIST_GET_NEXT_CONTROLLER                    GetNextController;
  PCIE_LIST_GET_NEXT_ROOT_PORT_IN_THIS_CONTROLLER  GetNextRootPortInThisController;
  PCIE_LIST_GET_THIS_CONTROLLER                    GetThisController;
  PCIE_LIST_GET_ROOT_PORT_ON_NEXT_DEVICE           GetRootPortOnNextDevice;
  PCIE_LIST_GET_ROOT_PORT_ON_NEXT_FUNCTION         GetRootPortOnNextFunction;
  PCIE_LIST_RESET_TO_FIRST                         ResetToFirst;
};

/**
  Configures PTM settings on the PCIe controller.

  @param[in] RpDev      Pointer to the root port device
  @param[in] PtmConfig  Pointer to the PTM configuration structure
**/
VOID
PcieSipConfigurePtm (
  IN PCIE_ROOT_PORT_DEV  *RpDev,
  IN PTM_CONFIGURATION   *PtmConfig
  );
  
/**
  Configure 10-Bit Tag

  @param[in] RpDev               Pointer to the root port device
  @param[in] CompleterSupported  If TRUE it indicates 10-Bit Tag Completer is supported.
  @param[in] RequesterSupported  If TRUE it indicates 10-Bit Tag Requester is supported.

**/
VOID
PcieSipConfigure10BitTag (
  IN PCIE_ROOT_PORT_DEV      *RpDev,
  IN BOOLEAN                 CompleterSupported,
  IN BOOLEAN                 RequesterSupported
  );

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  PcieSipConfigurePresetToCoefficientMapping has been called.

  @param[in]  RpDev         Pointer to the root port device
  @param[in]  LaneIndex     PCIe lane for which to get coefficients
  @param[in]  PresetIndex   Preset index for given lane
  @param[out] Coefficients  Value of the coefficients extracted from RP HW
**/
VOID
GetSipGen3PresetToCoefficientMapping (
  IN  PCIE_ROOT_PORT_DEV           *RpDev,
  IN  UINT32                       LaneIndex,
  IN  UINT32                       PresetIndex,
  OUT PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  );

/**
  Get coefficients values corresponding to given preset.
  This function will return valid values only after
  PcieSipConfigurePresetToCoefficientMapping has been called.

  @param[in]  RpDev         Pointer to the root port device
  @param[in]  LaneIndex     PCIe lane for which to get coefficients
  @param[in]  PresetIndex   Preset index for given lane
  @param[out] Coefficients  Value of the coefficients extracted from RP HW
**/
VOID
GetSipGen4PresetToCoefficientMapping (
  IN  PCIE_ROOT_PORT_DEV           *RpDev,
  IN  UINT32                       LaneIndex,
  IN  UINT32                       PresetIndex,
  OUT PCIE_LINK_EQ_COEFFICIENTS    *Coefficients
  );

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen3.

  @param[in] RpDev                 Pointer to the root port device
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
VOID
PcieSipSetGen3TransmitterLanePreset (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                LaneIndex,
  IN UINT32                DownstreamPortPreset,
  IN UINT32                UpstreamPortPreset
  );

/**
  Sets transmitter lane presets for both upstream and downstream port for Gen4.

  @param[in] RpBase                PCI base of the RP
  @param[in] LaneIndex             Index of the transmitter lane
  @param[in] DownstreamPortPreset  Preset value for the downstream port
  @param[in] UpstreamPortPreset    Preset value for the upstream port
**/
VOID
PcieSipSetGen4TransmitterLanePreset (
  IN PCIE_ROOT_PORT_DEV    *RpDev,
  IN UINT32                LaneIndex,
  IN UINT32                DownstreamPortPreset,
  IN UINT32                UpstreamPortPreset
  );

/**
  Sets local transmitter override coefficients for a
  given lane for Gen3.

  @param[in] RpDev             Pointer to the root port device
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
VOID
PcieSipSetGen3LocalTransmitterOverrideCoefficients (
  IN PCIE_ROOT_PORT_DEV           *RpDev,
  IN UINT32                       LaneIndex,
  IN PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  );

/**
  Sets local transmitter override coefficients for a
  given lane for Gen4.

  @param[in] RpDev             Pointer to the root port device
  @param[in] LaneIndex         Index of the PCI lane
  @param[in] LaneCoefficients  Coefficients to be programed
**/
VOID
PcieSipSetGen4LocalTransmitterOverrideCoefficients (
  IN PCIE_ROOT_PORT_DEV           *RpDev,
  IN UINT32                       LaneIndex,
  IN PCIE_LINK_EQ_COEFFICIENTS    *LaneCoefficients
  );

/**
  Set preset coefficients.

  @param[in] RpDev                Pointer to the root port device
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
VOID
SetSipGen3PresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV                    *RpDev,
  IN UINT32                                LaneIndex,
  IN UINT32                                PresetIndex,
  IN UINT32                                LowFrequencyValue,
  IN UINT32                                FullSwingValue,
  IN PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  );

/**
  Set Gen4 preset coefficients.

  @param[in] RpDev                Pointer to the root port device
  @param[in] LaneIndex            PCIe lane for which to set coefficients
  @param[in] PresetIndex          Preset index for the given lane
  @param[in] LowFrequencyValue    Low frequency value for the given lane
  @param[in] FullSwingValue       Full swing value for the given lane
  @param[in] PresetToCoefficient  Structure with information about coefficients for the preset
**/
VOID
SetSipGen4PresetToCoefficientMapping (
  IN PCIE_ROOT_PORT_DEV                    *RpDev,
  IN UINT32                                PresetIndex,
  IN UINT32                                LowFrequencyValue,
  IN UINT32                                FullSwingValue,
  IN PCIE_PRESET_TO_COEFFICIENT_MAPPING    PresetToCoefficient
  );

/**
  Performs the initialization for hardware equalization to Gen3.
  Please make sure this function is called before programming any
  power management register in PCIe Root Port

  @param[in] RpDev                   Pointer to the root port device
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
**/
VOID
PcieSipLinkEqualizeInit (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings
  );

/**
  Performs PCIe link equalization according to platform settings.
  Please make sure that link is in gen1 or gen2 before calling this
  procedure.

  @param[in] RpDev                   Pointer to the root port device
  @param[in] EndPointAttached        Is End Point device connected to the Root Port
  @param[in] PcieGen3LinkEqSettings  PCIe Gen3 link equalization settings to be used during EQ
  @param[in] PcieGen4LinkEqSettings  PCIe Gen4 link equalization settings to be used during EQ
  @param[in] PcieGen5LinkEqSettings  PCIe Gen5 link equalization settings to be used during EQ
  @param[in] RootPortPcieSpeed       PCIe speed of Root Port
  @param[in] EndpointPcieSpeed       PCIe speed of Endpoint connected to corresponding Root Port
**/
VOID
PcieSipLinkEqualize (
  IN PCIE_ROOT_PORT_DEV       *RpDev,
  IN BOOLEAN                  EndPointAttached,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen3LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen4LinkEqSettings,
  IN PCIE_LINK_EQ_SETTINGS    *PcieGen5LinkEqSettings,
  IN PCIE_SPEED               RootPortPcieSpeed,
  IN PCIE_SPEED               EndpointPcieSpeed
  );

/**
  Get max link width.

  @param[in] RpDev  Pointer to the root port device
  @retval           Max link width
**/
UINT8
PcieSipGetMaxLinkWidth (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Get PCIe port number for enabled port.
  @param[in] RpDev     Pointer to the root port device

  @retval Root Port number (1 based)
**/
UINT32
PcieSipGetPortNum (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Get the negotiated link width

  @param[in] RpDev     Pointer to the root port device

  @return negotiated link width
**/
UINT8
PcieSipGetNegotiatedLinkWidth (
  PCIE_ROOT_PORT_DEV  *RpDev
  );
  
/**
  Initialize PCIe root ports.

  @param[in] RpList                Pointer to the root port list.
  @param[in] PciSku                Type of Root Port.
  @param[in] FeatureConfiguration  PCIe Feature configuration
**/
VOID
PcieSipRpInit (
  IN PCIE_ROOT_PORT_LIST       *RpList,
  PCI_SKU                      PciSku,
  PCIE_FEATURE_CONFIGURATION   *FeatureConfiguration
  );

/**
  Perform some early Pcie Root port configuration to enable RCRB and program decoder control registers.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipEarlyDecodeEnable (
  IN PCIE_ROOT_PORT_LIST  *RpList
 );

/**
  Initializes ports with NonCommonClock and SSC configuration.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipInitNccRootPorts (
  IN PCIE_ROOT_PORT_LIST  *RpList
  );

/**
  Configure root port function number mapping.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipConfigureRpfnMapping (
  IN PCIE_ROOT_PORT_LIST  *RpList
  );

/**
  Hides root ports that have been disabled by straps
  or by the controller config.

  @param[in] RpList  Pointer to the root port list.
**/
VOID
PcieSipHideDisableRootPorts (
  IN PCIE_ROOT_PORT_LIST  *RpList
  );

/**
  Returns the PCIe controller bifurcation

  @param[in] RpDev  Pointer to root port device.

  @retval PCIe controller bifurcation
**/
UINT8
PcieSipGetControllerBifurcation (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Verify whether the PCIe port does own all lanes according to the port width.

  @param[in] ControllerDev  Pointer to the controller dev.
  @param[in] RpDev          Pointer to root port device.

  @retval TRUE   All lanes are owned by PCIe controller.
  @retval FALSE  Not all lanes are owned by PCIe root port.
**/
BOOLEAN
PcieSipIsRpOwningLanes (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Checks if root port is disabled in controller.

  @param[in] ControllerDev        Pointer to controller device.
  @param[in] RpIndexInController  Which root port in controller to disable.

  @retval TRUE   Root port is enabled.
  @retval FALSE  Root port is disabled.
**/
BOOLEAN
PcieSipIsRootPortEnabledInController (
  IN PCIE_ROOT_PORT_DEV  *ControllerDev,
  IN UINT32              RpIndexInController
  );

/**
  Configure PCIe power down mapping

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipConfigurePowerDownMapping (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Perform reset prep programming

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipConfigureResetPrep (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Program Rx primary Cycle Decode Registers for SIP17

  @param[in] RpDev  Pointer to root port device.
**/
VOID
PcieSipRxPrimaryCycleDecode (
  IN  PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Get max PCIe link speed supported by the root port.

  @param[in]  RpDev  Pointer to root port device.

  @retval Max link speed
**/
UINT32
PcieSipGetMaxLinkSpeed (
  IN PCIE_ROOT_PORT_DEV  *RpDev
  );

/**
  Dump PCIe HW EQ results. For internal use only.

  @param[in] RpDev     Pointer to the root port device
**/
VOID
PcieSipDumpHwEqResults (
  IN PCIE_ROOT_PORT_DEV    *RpDev
  );

/**
  Reset and enable Recovery Entry and Idle Framing Error Count

  @param[in] RpDev    Pointer to the root port device
**/
VOID
PcieSipResetErrorCounts (
  PCIE_ROOT_PORT_DEV    *RpDev
  );
#endif
