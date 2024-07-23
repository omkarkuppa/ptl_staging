/** @file
  UBS3 IP Instance structure

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

@par Specification
**/

#ifndef IP_USB3_H_
#define IP_USB3_H_

#include <IpCsi.h>

#define MAX_USB2_PORTS  16
#define MAX_USB3_LANES  10

typedef struct _IP_USB3_INST IP_USB3_INST;

//
// USB2 port configuration structure
//
typedef struct {
  BOOLEAN    Enable;                 // Port enable/disable control
  UINT8      OverCurrentPin;         // OverCurrent Pin number assigned to port
  UINT8      PortResetMessageEnable; // Port reset message enable/disable control
  UINT8      SwDeviceModeEnable;     // Switch port to device mode
  BOOLEAN    HasInboxDevice;         // Flag for setting whether there's inbox device on a port
} IP_USB3_USB2_PORT_CONFIG;

//
// USB3 port configuration structure
//
typedef struct {
  BOOLEAN    Enable;         // Port enable/disable control
  UINT8      OverCurrentPin; // OverCurrent Pin number assigned to port
  BOOLEAN    HasInboxDevice; // Flag for setting whether there's inbox device on a port
} IP_USB3_USB3_PORT_CONFIG;

//
// Supported feature list enumeration
//
typedef enum {
  IpUsb3FeatIdUnknown = 0,                    // Unknown feature
  IpUsb3FeatIdAudioOffload,                   // USB Audio Offload feature
  IpUsb3FeatIdPortResetMessaging,             // Port Reset Messaging
  IpUsb3FeatIdResetPrepOverride,              // Reset Prep Override
  IpUsb3FeatIdUsb3ProgrammingLock,            // USB3 Programming Lock bit
  IpUsb3FeatIdPowerControl,                   // Power Control related configuration
  IpUsb3FeatIdAccessControlLock,              // Access Control Lock setting
  IpUsb3FeatIdU2MappingToP3State,             // USB3 U2 mapping to P3 state
  IpUsb3FeatIdCreditRedeem,                   // Controls credit redeem when port is in NC state
  IpUsb3FeatIdBackToBackBTOHandlingEnable,    // USB2 Back to Back BTO Handling Enable
  IpUsb3FeatIdHhhFlushFix,                    // USB2 Back to Back BTO Handling Enable
  IpUsb3FeatIdB2bRetryFix,                    // Controls B2B retry fix in Data Align block
  IpUsb3FeatIdReadAfterWriteDelay,            // Controls delay of MMIO read after MMIO write
  IpUsb3FeatIdHSAsyncActiveExposure,          // Controls if HS Async Active Exposure is propagated to PMC
  IpUsb3FeatIdSoftErrorTriggerRecalibration,  // Controls if xHCI uses soft error as an indicator for recalibration
  IpUsb3FeatIdDbcTraceInEp                    // Controls inclusion of DbC.Trace into PMREQ Response and Wake Wire
} IP_USB3_FEAT_ID;

//
// USB3 IPSD feature values enumeration
//
typedef enum {
  IpUsb3FeatValDis = 0,

  // IpUsb3FeatIdAudioOffload
  IpUsb3FeatValAudioOffloadDis = 0,
  IpUsb3FeatValAudioOffloadEn  = 1,

  // IpUsb3FeatIdPowerControl
  IpUsb3FeatValPowerControlDis = 0,
  IpUsb3FeatValPowerControlEn  = 1,

  // IpUsb3FeatIdResetPrepOverride
  IpUsb3FeatValResetPrepOverrideDis = 0,
  IpUsb3FeatValResetPrepOverrideEn  = 1,

  // IpUsb3FeatIdU2MappingToP3State
  IpUsb3FeatValU2MappingToP3StateDis = 0,
  IpUsb3FeatValU2MappingToP3StateEn  = 1,

  // IpUsb3FeatIdCreditRedeem
  IpUsb3FeatValCreditRedeemDis = 0,
  IpUsb3FeatValCreditRedeemEn  = 1,

  // IpUsb3FeatIdBackToBackBTOHandlingEnable
  IpUsb3FeatValBackToBackBTOHandlingEnableDis = 0,
  IpUsb3FeatValBackToBackBTOHandlingEnableEn  = 1,

  // IpUsb3FeatIdHhhFlushFix
  IpUsb3FeatValHhhFlushFixDis = 0,
  IpUsb3FeatValHhhFlushFixEn  = 1,

  // IpUsb3FeatIdB2bRetryFix
  IpUsb3FeatValB2bRetryFixDis = 0,
  IpUsb3FeatValB2bRetryFixEn  = 1,

  // IpUsb3FeatIdReadAfterWriteDelay
  IpUsb3FeatValReadAfterWriteDelayDis = 0,
  IpUsb3FeatValReadAfterWriteDelayEn  = 1,

  // IpUsb3FeatIdHSAsyncActiveExposure
  IpUsb3FeatValHSAsyncActiveExposureDis = 0,
  IpUsb3FeatValHSAsyncActiveExposureEn  = 1,

  // IpUsb3FeatIdSoftErrorTriggerRecalibration
  IpUsb3FeatValSoftErrorTriggerRecalibrationDis = 0,
  IpUsb3FeatValSoftErrorTriggerRecalibrationEn  = 1,

  // IpUsb3FeatIdDbcTraceInEp
  IpUsb3FeatValDbcTraceInEpDis = 0,
  IpUsb3FeatValDbcTraceInEpEn  = 1,
} IP_USB3_FEAT_VAL;

//
// USB3 IPSD feature list entry structure
//
typedef struct {
  IP_USB3_FEAT_ID     FeatId;
  IP_USB3_FEAT_VAL    FeatVal;
} IP_USB3_FEATURE_LIST_ENTRY;

//
// USB3 IP tuning parameters
//
typedef enum {
  IpUsb3TuningParamIdUnknown = 0,               // Unknown tuning parameter
  IpUsb3TuningParamIdCp13Deemphasis,            // Tuning of CP13 Deemphasis value
  IpUsb3TuningParamIdCp14Deemphasis,            // Tuning of CP14 Deemphasis value
  IpUsb3TuningParamIdCp15Deemphasis,            // Tuning of CP15 Deemphasis value
  IpUsb3TuningParamIdCp16Deemphasis,            // Tuning of CP16 Deemphasis value
  IpUsb3TuningParamIdLinkPollingMinTime,        // Minimum time for the link to stay in Polling.Active and Recovery.Active from U3
  IpUsb3TuningParamIdGapTimeAfterLsTx,          // GAP time after LS TX thru FS hub
  IpUsb3TuningParamIdU2ExitLatency,             // U2 Device Exit Latency
  IpUsb3TuningParamIdHbufWaterMark,             // HBUF Water Mark
  IpUsb3TuningParamIdSsBackbonePxpTrunkClkGtEn, // SS Backbone PXP Trunk Clock Gate Enable
} IP_USB3_TUNING_PARAM_ID;

//
// Supported IP versions of USB3 IP
//
typedef enum {
  IpUsb3IpVersionUnknown = 0, // Unknown IP version
  IpUsb3IpVersion19p2,        // IP Version 19.2
  IpUsb3IpVersion19p3,        // IP Version 19.3
  IpUsb3IpVersion19p4         // IP Version 19.4
} IP_USB3_VERSION;

//
// Supported IP integration locations
//
typedef enum {
  IpUsb3IntegrationUnknown = 0, // Unknown integration
  IpUsb3IntegrationUsb4ss,      // IP located in USB4 Subsystem
  IpUsb3IntegrationStandalone,  // IP as a Standalone
  IpUsb3IntegrationPch          // IP used in PCH
} IP_USB3_INTEGRATION;

/**
  Configures GPIO pins for USB OverCurrent detection

  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  OvercurrentPin    Index of OverCurrent Pin to be enabled
**/
typedef
void
(*IP_USB3_ENABLE_OVERCURRENT_PIN) (
  IP_USB3_INST  *pInst,
  UINT8         OverCurrentPin
  );

/**
  Checks in FIA is given lane is USB owned

  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  LaneNumber        Index of lane to check
**/
typedef
BOOLEAN
(*IP_USB3_LANE_OWNED) (
  IP_USB3_INST  *pInst,
  UINT32        LaneNumber
  );

/**
  Calculates Overcurrent pin number to match USB3 OCM register bit map

  @param[in]  pInst             IP_USB3_INST structure pointer
  @param[in]  OcPin             Index of lane to check
**/
typedef
UINT32
(*IP_USB3_CALCULATE_OC_PIN) (
  IP_USB3_INST  *pInst,
  UINT32        OcPin
  );

//
// USB3 IP instance
//
struct _IP_USB3_INST {
  IP_CSI_INST_PREFIX            Prefix;
  IP_WR_VSC_CNTXT               IpWrVscCntxt;

  // CSI Errors
  IP_WR_ERROR_CNTXT             ErrorCntxt;
  IP_WR_ERROR_CNTXT             ErrorCntxtWarning;

  // CSI Print
  IP_WR_PRINT_CNTXT             PrintError;
  IP_WR_PRINT_CNTXT             PrintWarning;
  IP_WR_PRINT_CNTXT             PrintLevel1;

  // Register Access contexts
  IP_WR_REG_CNTXT               RegCntxtPci;       // PCI Config space register access context for USB Host controller
  IP_WR_REG_CNTXT               RegCntxtMem;       // MMIO space register access context for USB Host controller
  IP_WR_REG_CNTXT               RegCntxtSbMmio;    // Sideband space register MMIO access context for USB Host controller
  IP_WR_REG_CNTXT               RegCntxtDciSbMmio; // Sideband space register MMIO access context for DCI

  // Time contexts
  IP_WR_TIME_CNTXT              TimeCntxt;

  // Public configuration
  UINT64                        MmioBaseAddress;
  IP_USB3_VERSION               IpVersion;                      // Version of the IP that this instance describes
  IP_USB3_INTEGRATION           Integration;                    // Integration location of the IP
  UINT8                         Usb2PortCount;                  // Number of USB2 ports
  UINT8                         UsbRPortCount;                  // Number of USBr ports
  UINT8                         Usb3LanesCount;                 // Number of USB3 lanes
  BOOLEAN                       PdoProgramming;                 // Program PDO based on port configuration
  BOOLEAN                       OverCurrentEnable;              // Program OverCurrent registers based on port configuration
  BOOLEAN                       OverCurrentLock;                // Lock OverCurrent configuration after programming
  BOOLEAN                       UsbAudioOffloadEnable;          // Enable/disable control of USB Audio Offload feature
  UINT16                        FabricPllAdvanceWake;           // Value for Fabric PLL Advance wake programming (FPSAW)
  BOOLEAN                       Usb31SpeedAdvertise;            // Advertise ports as USB 3.1 speed
  BOOLEAN                       SkipD3HE;                       // If set code will not enable D3 Hot Enable in PCE
  BOOLEAN                       ExternalPowerControl;           // Flag for IPSD to not program Power Control
  BOOLEAN                       LtrEnable;                      // Enable LTR if set to FALSE will use NDE instead
  BOOLEAN                       NdeSbEnable;                    // Enable NDE side band if set to FALSE will use PMREQ instead
  BOOLEAN                       HsiiEnable;                     // Enabling of HSII feature
  UINT32                        LtvLimit;                       // Value of LTV Limit to set if NDE is not enabled
  UINT32                        LtrHighIdleTime;                // Value of High Idle Time if LTR is enabled
  UINT32                        LtrMedIdleTime;                 // Value of Medium Idle Time if LTR is enabled
  UINT32                        LtrLowIdleTime;                 // Value of Low Idle Time if LTR is enabled
  UINT8                         InterruptPin;                   // Interrupt pin that will be used for interrupt programming
  BOOLEAN                       DisableHostController;          // Controls whether to disable controller or not during SoC configuration
  UINT8                         MaxPayloadSize;                 // Control max size of DMA payload
  UINT8                         RequestBoundaryCrossingControl; // Controls size of boundary crossing limit of each R/W request
  IP_CSI_TUNING_PARAM_ENTRY     *TuningList;                    // Tuning list table pointer
  UINT32                        TuningListEntryCount;           // Tuning list table entry count
  IP_USB3_FEATURE_LIST_ENTRY    *FeatureList;                   // Feature list table pointer
  UINT32                        FeatureListEntryCount;          // Feature list table entry count
  BOOLEAN                       IsDciEnabled;                   // If TRUE USB2 DAP SPR debug mode is enabled, debug disabled if FALSE.

  // Per port configurations
  IP_USB3_USB2_PORT_CONFIG      Usb2PortConfig[MAX_USB2_PORTS];
  IP_USB3_USB3_PORT_CONFIG      Usb3PortConfig[MAX_USB3_LANES];

  /**
    USB callbacks
    List of upper layer function pointers (callbacks)
    that can be passed to IP initialization library.
    Used for cross IP communication
    and SoC related operations.
  **/
  IP_USB3_ENABLE_OVERCURRENT_PIN    cEnableOvercurrentPin;
  IP_USB3_LANE_OWNED                cIsUsbOwningLane;
  IP_USB3_CALCULATE_OC_PIN          cCalculateOcPin;
};

/**
  Get version information

  @param[in]      pInst USB3 IP instance
  @param[in]      VerId version ID

  @retval         0 is returned upon any error or unavailable version ID
  @retval         return value define by VerId
**/
UINT32
IpUsb3GetVersion (
  IP_USB3_INST   *pInst,
  IP_CSI_VER_ID  VerId
  );

/**
  Get Control

  @param[in]      pInst       USB3 IP instance
  @param[in]      IpTarget    IP defined value for target within the IP (for USB3 only 0 is accepted)
  @param[in]      FeatureId   Feature ID
  @param[in,out]  pCsiSts     Optional may be NULL. If non-NULL return IP_CSI_STATUS
    IpCsiStsSuccess           completed without error
    IpCsiStsErrorUnknownId    unknown Feature ID was provided
    IpCsiStsErrorNullPtr      null pointer to IP Instance provided
    IpCsiStsErrorBadParam     IpTarget value provided is different than 0

  @retval         UINT32      defined by IP_USB3_FEATURE_ID
**/
UINT32
IpUsb3GetControl (
  IP_USB3_INST     *pInst,
  UINT32           IpTarget,
  IP_USB3_FEAT_ID  FeatureId,
  IP_CSI_STATUS    *pCsiSts
  );

/**
  Set Control. Based on provided FeatureId it controls that feature and programming
  that comes with this.

  @param[in]  pInst       USB3 IP instance
  @param[in]  IpTarget    IP defined value for target within the IP (for USB3 only 0 is accepted)
  @param[in]  FeatureId   Feature ID
  @param[in]  FeatureVal  Value based on FeatureId

  @retval     IpCsiStsSuccess          completed without error
  @retval     IpCsiStsErrorUnknownId   unknown Feature ID was provided
  @retval     IpCsiStsErrorNullPtr     null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam    IpTarget value provided is different than 0
**/
IP_CSI_STATUS
IpUsb3SetControl (
  IP_USB3_INST     *pInst,
  UINT32           IpTarget,
  IP_USB3_FEAT_ID  FeatureId,
  UINT32           FeatureVal
  );

/**
  USB3 IP Instance Initialization

  @param[in]  pInst  USB3 IP instance

  @retval     IpCsiStsSuccess        completed without error
  @retval     IpCsiStsErrorNullPtr   null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3InstInit (
  IP_USB3_INST  *pInst
  );

/**
  USB3 IP initialization procedure entry point

  @param[in]  pInst   USB3 IP instance

  @retval     IpCsiStsSuccess        completed without error
  @retval     IpCsiStsErrorNullPtr   null pointer to IP Instance provide
**/
IP_CSI_STATUS
IpUsb3IpInit (
  IP_USB3_INST  *pInst
  );

/**
  Enables USB3 ports by enabling SuperSpeed terminations

  @param[in]  pInst             USB3 IP Instance
  @param[in]  EnablePortsMask   Bitmask of enabled USB3 ports

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3EnableUsb3Ports (
  IP_USB3_INST  *pInst,
  UINT32        EnablePortsMask
  );

/**
  This goes through general USB3 controller initialization programming
  requirements described in USB3 Programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3HostControllerInit (
  IP_USB3_INST  *pInst
  );

/**
  Programs interrupt pin in xHCI controller

  @param[in]  pInst         USB3 IP Instance
  @param[in]  IntPin        Interrupt Pin

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorBadParam   IntPin value must be between 0 and 3
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3InterruptMapping (
  IP_USB3_INST  *pInst,
  UINT8         IntPin
  );

/**
  Programs Over Current registers based on data present in IP Instance

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3OvercurrentMapping (
  IP_USB3_INST  *pInst
  );

/**
  Programs latency related parameters in xHCI.
  Based on provided parameters will either enable NDE support with side band or PMREQ
  or program LTR related values based on data in IP instance.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  LtrEnable     Flag whether to use LTR or NDE
  @param[in]  NdeSbEnable   Flag whether to use side band or PMREQ for NDE
  @param[in]  HsiiEnable    Flag whether to enable HS Interrupt IN alarm

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3LatencyTuning (
  IP_USB3_INST  *pInst,
  BOOLEAN       LtrEnable,
  BOOLEAN       NdeSbEnable,
  BOOLEAN       HsiiEnable
  );

/**
  Enables function disable functionality in xHCI

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3FunctionDisable (
  IP_USB3_INST  *pInst
  );

/**
  Disables USB2 ports using Port Disable Override feature

  @param[in]  pInst               USB3 IP Instance
  @param[in]  DisabledPortsMask   Bitmask of disabled ports
  @param[in]  PortCount           Number of USB2 port count

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           actual register values differ from expected ones
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3DisableUsb2Ports (
  IP_USB3_INST  *pInst,
  UINT32        DisabledPortsMask,
  UINT8         PortCount
  );

/**
  Disables USB3 ports using Port Disable Override feature

  @param[in]  pInst               USB3 IP Instance
  @param[in]  DisabledPortsMask   Bitmask of disabled ports
  @param[in]  PortCount           Number of USB3 port count

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           actual register values differ from expected ones
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3DisableUsb3Ports (
  IP_USB3_INST  *pInst,
  UINT32        DisabledPortsMask,
  UINT8         PortCount
  );

/**
  Shall go through SOC specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst                           USB3 IP Instance
  @param[in]  RequestBoundaryCrossingControl  Crossing boundary control
  @param[in]  MaxPayloadSize                  Max payload size

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam   unsupported payload size provided
**/
IP_CSI_STATUS
IpUsb3FabricConfiguration (
  IP_USB3_INST  *pInst,
  UINT8         RequestBoundaryCrossingControl,
  UINT8         MaxPayloadSize
  );

/**
  Shall go through SOC specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorBadParam   one of the configuration parameters in IP Instance was incorrect
  @retval     IpCsiStsError           actual register values differ from expected ones
**/
IP_CSI_STATUS
IpUsb3SocConfiguration (
  IP_USB3_INST  *pInst
  );

/**
  Shall go through platform specific programming flows and requirements described in
  USB3 programming guide.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3PlatformConfiguration (
  IP_USB3_INST  *pInst
  );

/**
  This interface will apply the tuning values provided in the tuning list to the IP.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  pTuningList   List of Parameter Ids and Parameter Values to be applied.
  @param[in]  EntryCount    The number of tuning list entries in the pTuningList.

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsErrorUnknownId  bad IP_CSI_TUNING_PARAM_ENTRY
**/
IP_CSI_STATUS
IpUsb3ApplyTuningList (
  IP_USB3_INST               *pInst,
  IP_CSI_TUNING_PARAM_ENTRY  *pTuningList,
  UINT32                     EntryCount
  );

/**
  Enables MMIO access to xHCI registers by programming MBAR value in xHCI PCI
  config space and enabling Memory Space Enable bit in PCI.CMD

  @param[in]  pInst         USB3 IP Instance
  @param[in]  Address       MBAR address value

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3MmioEnable (
  IP_USB3_INST  *pInst,
  UINT64        Address
  );

/**
  Disables MMIO access to xHCI registers by clearing Memory Space Enable bit in
  PCI.CMD register and clearing MBAR register in xHCI PCI config space.

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3MmioDisable (
  IP_USB3_INST  *pInst
  );

/**
  Performs full Host Controller initialization based on USB3 IP Instance data

  @param[in]  pInst         USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsError           error occured during call to any of underlying functions
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3HostControllerFullInit (
  IP_USB3_INST  *pInst
  );

/**
  Goes through all USB2 ports reported by controller and switches them to functional
  ports from DbC

  @param[in]  pInst                   USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3ChangeUsb2DbcPortsToFunctional (
  IP_USB3_INST  *pInst
  );

/**
  Goes through all USB2 ports reported by the controller and switches to device mocde
  based on configuration in IP_USB3_USB2_PORT_CONFIG

  @param[in]  pInst                   USB3 IP Instance

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
**/
IP_CSI_STATUS
IpUsb3UsbSwDeviceModeConfig (
  IP_USB3_INST  *pInst
  );

#endif // IP_USB3_H_
