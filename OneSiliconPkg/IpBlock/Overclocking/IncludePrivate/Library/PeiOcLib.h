/** @file
  Header file for overclocking initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _PEI_OC_LIB_H_
#define _PEI_OC_LIB_H_

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/OcCommonLib.h>
#include <Ppi/SiPolicy.h>
#include <Ppi/Wdt.h>
#include <Ppi/MpServices2.h>
#include <Library/OcMailboxLib.h>

///
///  High Level Voltage/Frequency data required for setting v/f requests on all CPU domains
///
typedef struct {
  UINT8  MaxOcRatio; ///< Max overclocking ratio limit for given CPU domain id
  UINT8  VoltageTargetMode; ///< Voltage Mode selection; <b>0: Adaptive</b>, 1: Override
  UINT16 VoltageTarget; ///< Voltage target specified in millivolts
  INT16  VoltageOffset; ///< Voltage offset specified in millivolts. Can be positive or negative offset.
  UINT8  VfPointIndex;  ///< Selected VF Point Index.
} VOLTAGE_FREQUENCY_SETTINGS;

///
///  Expanded Voltage Frequency mailbox information in the mailbox command format
///
typedef struct {
  UINT32 MaxOcRatio       : 8;  ///< Max overclocking ratio limit for given CPU domain id
  UINT32 VoltageTargetU12 : 12; ///< Voltage target represented in unsigned fixed point U12.2.10V format
  UINT32 TargetMode       : 1;  ///< Voltage mode selection; <b>0: Adaptive</b>, 1: Override
  UINT32 VoltageOffsetS11 : 11; ///< Voltage offset represented in signed fixed point S11.0.10V format
} VF_MAILBOX_COMMAND_DATA;

///
///  High Level DDR capabilities data used to pass into the overclocking library for final
///  mailbox message creation.
///
typedef struct {
  UINT8   QclkRatio; ///< Qclk Ratio
  BOOLEAN McReferenceClk; ///< Memory Controller Reference Clock 0: 133 MHz, 1: 100 MHz
  UINT8   NumDdrChannels; ///< Number of DDR Channels
} DDR_CAPABILITIES_ITEM;

///
///  High level Voltage/Frequency information used to pass into the overclocking library
///  for the final mailbox message creation.
///
typedef struct {
  VOLTAGE_FREQUENCY_SETTINGS VfSettings; ///< Structure containing the voltage/frequency information
  UINT8                      DomainId; ///< CPU Domain ID
} VOLTAGE_FREQUENCY_ITEM;

///
/// High level per core ratio limit information used to pass into the overclocking library
/// for the final mailbox message creation.
///
typedef struct {
  UINT8 MaxOcRatioLimit1C; ///< Max 1-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit2C; ///< Max 2-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit3C; ///< Max 3-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit4C; ///< Max 4-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit5C; ///< Max 5-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit6C; ///< Max 6-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit7C; ///< Max 7-Core overclocking ratio limit
  UINT8 MaxOcRatioLimit8C; ///< Max 8-Core overclocking ratio limit
  UINT8 CoreGroupSelector; ///< Core group selector; 0: Group 0-3, 1(1C to 4C): Group 4-7(5C to 8C)
} CORE_RATIO_LIMITS_ITEM;

///
/// PVD Override command offset information
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 PvdRatioThreshold    :  6; ///< [5:0]  Ratio Threshold value for PVD
    UINT32 PvdMode              :  2; ///< [7:6]  PVD Overclock Mode
    UINT32 Reserved             : 24; ///< [31:8] Reserved
  }Fields;
} PVD_OVERRIDE;

///
/// FLL Override command offset information
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 FllOverclockMode     :  2; ///< [1:0]  FLL Overclock Mode
    UINT32 Reserved             : 30; ///< [31:2] Reserved
  }Fields;
} FLL_OVERRIDE;

///
/// UnderVolt Protection command details
///
typedef union {
  UINT32 Data;
  struct {
    UINT32 UnderVoltProtection  :  2; ///< [1:0]
    UINT32 Reserved             : 30; ///< [31:2] Reserved
  }Fields;
} UNDERVOLT_PROTECTION_CMD_DATA;


///
/// OC PERSISTENT OVERRIDES
/// For the overrides, request will take effect after warm reset.
/// Each bit set will persist until the next cold boot.
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Reserved1                         : 5;  ///< [4:0] Reserved
    UINT32   TscDisableHwFixup                 : 1;  ///< [5:5] Core HW Fixup disable during TSC copy from PMA to APIC
    UINT32   DisableProcessVmaxLimit           : 1;  ///< [6:6] Disabling Process Vmax Limit will allow user to set any voltage
    UINT32   NegativeTemperatureReporting      : 1;  ///< [7:7] Enable Negative Temperature Reporting
    UINT32   Reserved2                         : 24; ///< [31:8] Reserved
  } Fields;
} OC_PERSISTENT_OVERRIDES_DATA;

///
/// OC MISC Config Data
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Reserved1                    : 1; ///Bit[0] < Temperature Threshold
    UINT32   DynamicMemTimeChangeEn       : 1; ///Bit[1] < Enables Dynamic Memory Timing Changes
    UINT32   CoreOpPointReportingEn       : 1; ///Bit[2] < Enables Core Operating point reporting
    UINT32   PcorePowerDensityThrottleDis : 1; ///Bit[3] < Disable P-core throttling for power density protection
    UINT32   EcorePowerDensityThrottleDis : 1; ///Bit[4] < Disable E-core throttling for power density protection
    UINT32   Reserved2                    :27; ///Bit[31:5] Reserved
  } Fields;
} OC_MISC_CONFIG_DATA;

//
// OCMB Command
//

//
// OC Capability data for OCMB command 0x01
//
typedef union {
  UINT32 Data;
  struct {
    UINT32 MaxOcRatioLimit              :  8; // BIT[7:0]   Max OC Ratio Limit
    UINT32 RatioOcSupported             :  1; // BIT[8:8]   OC Ratio Support
    UINT32 VoltageOverridesSupported    :  1; // BIT[9:9]   OC Voltage Override Support
    UINT32 VoltageOffsetSupported       :  1; // BIT[10:10] OC Voltage Offset Support
    UINT32 Reserved                     : 21; // BIT[11:31] Reserved Bits
  } Fields;
} OC_CAPABILITIES_ITEM;

///
/// OC VR Topology data for OCMB Command 0x04
///
typedef union {
  UINT32 Data;
  struct {
    UINT32  Reserved1            : 8; /// [07:00] Reserved
    UINT32  IaVrAddress          : 4; /// [11:08] IA VR Address
    UINT32  IaVrType             : 1; /// [12]    IA VR Type
    UINT32  GtVrAddress          : 4; /// [16:13] GT VR Address
    UINT32  GtVrType             : 1; /// [17]    GT VR Type
    UINT32  Reserved2            : 7; /// [24:18] Reserved
    UINT32  SaVrAddress          : 4; /// [28:25] SA VR Address
    UINT32  SaVrType             : 1; /// [29]    SA VR Type
    UINT32  Reserved3            : 1; /// [30]    Reserved
    UINT32  PsysDisable          : 1; /// [31]    PSYS Disable
  } Fields;
} OCMB_VR_TOPOLOGY_DATA;

///
/// MISC Global Config Data for OCMB Command 0x14/0x15
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Reserved                   :  2; ///< Bit[1:0]  Reserved bits
    UINT32   BclkAdaptiveVoltage        :  1; ///< Bit[2]    BCLK Change will result in VF Curve
    UINT32   PerCoreVfOverride          :  1; ///< Bit[3]    Per Core VF Override for OCMB command 0x10/0x11
    UINT32   Reserved1                  : 28; ///< Bit[31:4] Reserved bits
  } Fields;
} MISC_GLOBAL_CONFIG;

///
/// OC ICC MAX data for OCMB command 0x16/0x17
///
typedef union {
  UINT32 Data;
  struct {
    UINT32  IccMaxValue          :11; /// Sets/Gets the maximum desired current
    UINT32  Reserved             :20; /// Reserved
    UINT32  UnlimitedIccMaxMode  : 1; ///  Unlimited ICCMAX mode
  } Fields;
} OCMB_ICCMAX_DATA;

///
/// MISC Turbo Control Data for OCMB Command 0x18/0x19
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Reserved                   :  1; ///< Bit[0:0]  Reserved bits
    UINT32   RingDownbinDisable         :  1; ///< Bit[1:1]  Push Ring Downbin Disable
    UINT32   ThermalTurboDisable        :  1; ///< Bit[2:2]  Thermal Turbo Disable
    UINT32   NegativeItdDisable         :  1; ///< Bit[3:3]  Negative ITD Disable
    UINT32   Reserved1                  : 28; ///< Bit[31:4] Reserved bits
  } Fields;
} MISC_TURBO_CONTROL;

///
/// AVX Control Data for OCMB Command 0x1A/0x1B
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Reserved                   :  5; ///< Bit[4:0]   Reserved bits
    UINT32   AVX2RatioOffset            :  5; ///< Bit[9:5]   AVX2 Ratio Offset
    UINT32   Reserved1                  : 22; ///< Bit[31:10] Reserved bits
  } Fields;
} AVX_CONTROL;

///
/// AVX Voltage Guardband Scale Factor Data for OCMB Command 0x20/0x21
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   Avx2VoltageScaleFactor     :  8; ///< Bit[7:0]   AVX2 Voltage Guardband Scale Factor in U1.7 format
    UINT32   Reserved1                  : 24; ///< Bit[31:8]  Reserved bits
  } Fields;
} AVX_VOLTAGE_SCALE;

///
/// OC TVB Config data for OCMB command 0x24/0x25
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   TvbTempThreshold           : 8;  ///< Bit[7:0]   Temperature Threshold
    UINT32   TvbDownBins                : 8;  ///< Bit[15:8]  Down Bins (delta) for Temperature Threshold
    UINT32   Reserved                   : 16; ///< Bit[31:16] Reserved
  } Fields;
} OCMB_TVB_DATA;

///
/// Granular Ratio Config Data for OCMB Command 0x26/0x27
///
typedef union {
  UINT32 Data;
  struct {
    UINT32   GranularityUnits         : 3; ///Bit[2:0]   < Granularity Units
    UINT32   Reserved1                : 5; ///Bit[7:3]   < Reserved bits
    UINT32   GranularityBins          : 8; ///Bit[15:8]  < Granularity Bins for Module
    UINT32   Reserved2                :16; ///Bit[31:16] < Reserved bits
  } Fields;
} OC_GRANULAR_RATIO_DATA;

///
/// Module related information
///
typedef struct {
    UINT8                                  LpId;
    UINT8                                  CoreType;
    UINT16                                 InstanceId;
} OC_MODULE_INFO;

///
/// Instance related information for each node
///
typedef struct {
    UINT8                                  DieId;
    UINT8                                  ModuleId;
    UINT16                                 InstanceId;
    UINT8                                  CoreType;
    VOID                                   *NextNodePtr;
} OC_INSTANCE_NODE;


/// OC Library Function Prototypes
///
/**
  Gets the Voltage and Frequency information for a given CPU domain

  @param[out] *VfSettings
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetVoltageFrequencyItem (
  OUT VOLTAGE_FREQUENCY_ITEM *VfSettings,
  OUT UINT32                 *LibStatus
  );

/**
  Sets the Voltage and Frequency information for a given CPU domain

  @param[in]  *VfSettings
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
SetVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM VfSettings,
  OUT UINT32                *LibStatus
  );

/**
  Update existing OC Voltage & Frequency item to the requested OC configuration.

  @param[in]  CurrentVfItem      The current domain VfSettings.
  @param[in]  RequestedVfItem    The requested domain VfSettings.
  @param[in]  WdtPei             watchdog timer for OC changes.

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
UpdateVoltageFrequencyItem (
  IN VOLTAGE_FREQUENCY_ITEM  *CurrentVfItem,
  IN VOLTAGE_FREQUENCY_ITEM  *RequestedVfItem,
  IN WDT_PPI                 *WdtPei           OPTIONAL
  );

/**
  Get the overclocking capabilities for a given CPU Domain

  @param[out] *OcCapabilities
  @param[in]  DomainId
  @param[out] *LibStatus

  @retval EFI_STATUS
**/
EFI_STATUS
EFIAPI
GetOcCapabilities (
  OUT OC_CAPABILITIES_ITEM *OcCapabilities,
  IN  UINT8                DomainId,
  OUT UINT32               *LibStatus
  );

/**
  Converts the input votlage Offset to the fixed point S11.0.10 Volt format or
  to Binary millivolts representation based on the ConversionType.

  @param[in]  InputVoltageOffset
  @param[out] *OutputVoltageOffset
  @param[in]  ConversionType - 0:fixed point, 1:Signed Binary millivolts
**/
VOID
EFIAPI
ConvertVoltageOffset (
  IN INT16  InputVoltageOffset,
  OUT INT16 *OutputVoltageOffset,
  IN UINT8  ConversionType
  );

/**
  Converts the input data to valid mailbox command format based on CommandID

  @param[in]  InputData      Specific input data per command ID.
  @param[out] *MailboxData   Return the constructed mailbox data.
  @param[in]  CommandId      Command ID.

  @retval  EFI_UNSUPPORTED   Unsupported command ID.
  @retval  EFI_SUCCESS       Mailbox data is successfully constructed.
**/
EFI_STATUS
EFIAPI
ConvertToMailboxFormat (
  IN VOID             *InputData,
  OUT OC_MAILBOX_FULL *MailboxData,
  IN UINT32           CommandId
  );

/**
  Initializes pre-memory Overclocking settings in the processor.

  @param[in] *SiPreMemPolicyPpi - SiPreMemPolicyPpi Pointer to Si Policy Ppi instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPreMem (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  );

/**
  Initializes post-memory Overclocking settings in the processor.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
  @param[in] MpServices2Ppi            Pointer to this instance of the MpServices.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPostMem (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig,
  IN EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi
  );
#endif
