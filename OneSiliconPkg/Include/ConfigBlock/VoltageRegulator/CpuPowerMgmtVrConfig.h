/** @file
  CPU Power Management VR Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _CPU_POWER_MGMT_VR_CONFIG_H_
#define _CPU_POWER_MGMT_VR_CONFIG_H_

extern EFI_GUID gCpuPowerMgmtVrConfigGuid;
#include <ConfigBlock.h>
#pragma pack (push,1)

///
/// Defines the maximum number of VR domains supported.
/// @warning: Changing this define would cause DWORD alignment issues in policy structures.
///
#define MAX_NUM_VRS         6

/**
  CPU Power Management VR Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/

#define CPU_POWER_MGMT_VR_CONFIG_REVISION 1

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  UINT32 AcousticNoiseMitigation        : 1;      ///< Enable or Disable Acoustic Noise Mitigation feature. <b>0: Disabled</b>; 1: Enabled

  UINT32 RfiMitigation                  : 1;      ///< Enable or Disable RFI Mitigation. <b>0: Disable - DCM is the IO_N default</b>; 1: Enable - Enable IO_N DCM/CCM switching as RFI mitigation.
  UINT32 RsvdBits                       : 30;     ///< Reserved for future use.
  UINT8  PsysSlope;                               ///< PCODE MMIO Mailbox: Platform Psys slope correction. <b>0: Auto</b> Specified in 1/100 increment values. Range is 0-200. 125 = 1.25.
  UINT8  Rsvd;
  UINT16 PsysPmax;                                ///< PCODE MMIO Mailbox: Platform Power Pmax. <b>0 - Auto</b> Specified in 1/8 Watt increments. 0-1024 Watts. Value of 800 = 100W.

  /** @name VR Settings
  The VR related settings are sorted in an array where each index maps to the VR domain as defined below:
   - 0 = System Agent VR
   - 1 = IA Core VR
   - 2 = Ring Vr
   - 3 = GT VR
   - 4 = FIVR VR

  The VR settings for a given domain must be populated in the appropriate index.
  **/
  ///@{
  UINT16 TdcCurrentLimit[MAX_NUM_VRS];            ///< PCODE MMIO Mailbox: Thermal Design Current current limit. Specified in 1/8A units. Range is 0-4095. 1000 = 125A. <b>0: 0 Amps</b>
  UINT16 AcLoadline[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: AcLoadline in 1/100 mOhms (ie. 1250 = 12.50 mOhm); Range is 0-6249. <b>Intel Recommended Defaults vary by domain and SKU.</b>
  UINT16 DcLoadline[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: DcLoadline in 1/100 mOhms (ie. 1250 = 12.50 mOhm); Range is 0-6249.<b>Intel Recommended Defaults vary by domain and SKU.</b>
  UINT16 Ps1Threshold[MAX_NUM_VRS];               ///< PCODE MMIO Mailbox: Power State 1 current cuttof in 1/4 Amp increments. Range is 0-128A.
  UINT16 Ps2Threshold[MAX_NUM_VRS];               ///< PCODE MMIO Mailbox: Power State 2 current cuttof in 1/4 Amp increments. Range is 0-128A.
  UINT16 Ps3Threshold[MAX_NUM_VRS];               ///< PCODE MMIO Mailbox: Power State 3 current cuttof in 1/4 Amp increments. Range is 0-128A.
  INT32  ImonOffset[MAX_NUM_VRS];                 ///< PCODE MMIO Mailbox: Imon offset correction. Value is a 2's complement signed integer. Units 1/1000, Range is [-128000, 127999]. For an offset = 12.580, use 12580. <b>0: Auto</b>
  UINT16 IccMax[MAX_NUM_VRS];                     ///< PCODE MMIO Mailbox: VR Icc Max limit. 0-255A in 1/4 A units. 400 = 100A. <b>Default: 0 - Auto, no override</b>
  UINT16 VrVoltageLimit[MAX_NUM_VRS];             ///< PCODE MMIO Mailbox: VR Voltage Limit. Range is 1-7999mV.
  UINT16 ImonSlope[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Imon slope correction. Specified in 1/100 increment values. Range is 0-200. 125 = 1.25. <b>0: Auto</b>
  UINT8  Ps3Enable[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Power State 3 enable/disable; 0: Disable; <b>1: Enable</b>.
  UINT8  Ps4Enable[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Power State 4 enable/disable; 0: Disable; <b>1: Enable</b>.
  UINT8  VrConfigEnable[MAX_NUM_VRS];             ///< Enable/Disable BIOS configuration of VR; 0: Disable; <b>1: Enable.</b>
  UINT8  TdcEnable[MAX_NUM_VRS];                  ///< PCODE MMIO Mailbox: Thermal Design Current enable/disable; <b>0: Disable; </b>1: Enable
  UINT8  TdcLock[MAX_NUM_VRS];                    ///< PCODE MMIO Mailbox: Thermal Design Current Lock; <b>0: Disable</b>; 1: Enable.
  UINT8  FastPkgCRampDisable[MAX_NUM_VRS];        ///< Disable Fast Slew Rate for Deep Package C States for VR IA,GT domain. <b>0: False</b>; 1: True
  UINT8  SlowSlewRate[MAX_NUM_VRS];               ///< Slew Rate configuration for Deep Package C States for VR IA,GT domain. <b>0: Fast/2</b>; 1: Fast/4; 2: Fast/8; 3: Fast/16
  ///@}
  UINT8  Rsvd1[2];                                ///  Make sure nature alignment
  INT32  PsysOffset;                              ///< PCODE MMIO Mailbox: Platform Psys offset correction. <b>0: Auto</b> 32-bit signed value (2's complement). Units 1/1000, Range is [-128000, 127999]. For an offset of 25.348, enter 25348.
  UINT32 TdcTimeWindow[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: Thermal Design Current time window. Defined in milli seconds. <b>1ms default</b>
  UINT8  TdcMode[MAX_NUM_VRS];                    ///< PCODE MMIO Mailbox:TDC Mode based on IRMS supported bit from Mailbox. <b>0: iPL2</b>; 1: IRMS.
  UINT8  FivrSpectrumEnable;                      ///< Enable or Disable FIVR Spread Spectrum 0: Disable; <b> 1: Enable.</b>
  UINT8  DlvrSpreadSpectrumPercentage;            ///< PCODE MMIO Mailbox: Encoded DLVR SSC value. 0x0 = Disabled. u3.2 value from 0% - 7.75%.
  UINT8  DlvrRfiEnable;                           ///< PCODE MMIO Mailbox: Enable/Disable RFI frequency hopping. 0: Disable; <b>1: Enable.</b>
  UINT8  PcoreHysteresisWindow;                   ///< PCODE MMIO Mailbox: Pcore VR Hysteresis time window. This can be programmed only if AcousticNoiseMitigation is enabled.<b>Default Value = 0 </b> Range is 0-50ms.
  UINT8  EcoreHysteresisWindow;                   ///< PCODE MMIO Mailbox: Ecore VR Hysteresis time window. This can be programmed only if AcousticNoiseMitigation is enabled.<b>Default Value = 0 </b> Range is 0-50ms.
  UINT16 DlvrRfiFrequency;                        ///< PCODE MMIO Mailbox: DLVR clock frequency in MHz.
  UINT8  PS1toPS0DynamicCutoffEnable[MAX_NUM_VRS];///< Enable/Disable PS1 to PS0 Dynamic Cutoff; <b>0: Disable</b>; 1: Enable.
  UINT8  PS2toPS1DynamicCutoffEnable[MAX_NUM_VRS];///< Enable/Disable:PS2 to PS1 Dynamic Cutoff; <b>0: Disable</b>; 1: Enable.
  UINT16 PS1toPS0MCoef[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: PS1 to PS0 M Coef. This number is in M*100 units. <b>Default: 100</b> Range: 0-4096.
  INT16  PS1toPS0CCoef[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: PS1 to PS0 C Coef. This number is in C*100 units. <b>Default: 2000</b> Range: 0-4096.
  UINT16 PS2toPS1MCoef[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: PS2 to PS1 M Coef. This number is in M*100 units. <b>Default: 100</b> Range: 0-4096.
  INT16  PS2toPS1CCoef[MAX_NUM_VRS];              ///< PCODE MMIO Mailbox: PS2 to PS1 C Coef. This number is in C*100 units. <b>Default: 500</b> Range: 0-4096.
  /**
    This field is required to be non-zero on Desktop platforms.

    Used to communicate the power delivery design capability of the board. This value is an
    enum of the available power delivery segments that are defined in the Platform Design Guide.
    This is required due to CPU socket compatibility across different board designs. Without
    this field populated, the system may experience VR OCP/OVP shutdowns, hangs, thermal and
    performance loss.
    <b> 0 - Disable </b>
  **/
  UINT8  VrPowerDeliveryDesign;
  /**
  Enables / Disables DLVR's PHASE_SSC feature.
  **/
  UINT8  DlvrPhaseSsc;                           ///< Enable/Disable DLVR PHASE_SSC. <b>0: Disable.</b> 1:Disable
  /**
    Enable VCC Demotion.
      0: Disable;
      <b>1: Enable with default threshold</b>;
      2: Enable with user configured threshold.
  **/
  UINT8  VccInDemotionEnable[MAX_NUM_VRS];
  /**
    User configured platform quiescent threshold in milli-watt
    when VCC Demotion is enabled with user configured threshold.
    Range: 0-255.
  **/
  UINT8  VccInDemotionQuiescentPowerInMw[MAX_NUM_VRS];
  /**
    User configured VR's quiescent power threshold in micro-farad
    when VCC Demotion is enabled with user configured threshold.
    Range: 0-2000.
  **/
  UINT32 VccInDemotionCapacitanceInUf[MAX_NUM_VRS];

  UINT8  EnableVsysCritical;                     ///< PCODE MMIO Mailbox: Enable/Disable Vsys Critical; <b>0: Disable</b>; 1: Enable.
  /**
    Set Assertion Deglitch [0x4F] and De-Assertion Deglitch [0x49]
    Provide 2 fields in BIOS Menu for Assertion/De-Assertion Deglitch: Mantissa and Exponent.
    The format for Assertion/De-Assertion Deglitch: 0.002ms * Mantissa * 2^(Exponent).
  **/
  UINT8  VsysAssertionDeglitchMantissa;
  UINT8  VsysAssertionDeglitchExponent;
  UINT8  VsysDeassertionDeglitchMantissa;
  UINT8  VsysDeassertionDeglitchExponent;
  UINT8  Rsvd2[16];
  UINT8  CepEnable[MAX_NUM_VRS];
  /**
    PCODE VR Mailbox: Voltage Regulator Fast Vmode ICC Limit.
    This value represents the current threshold where the VR would initiate reactive protection if Fast Vmode is enabled.
    A value of 0 corresponds to feature disabled (no reactive protection).
    <b>Default: 0 - Disable, no override</b>
  **/
  UINT16 IccLimit[MAX_NUM_VRS];
  /**
    VR Fast Vmode.
    Use to control Fast Vmode Enable/Disable by VR domain.
    <b>Default: 0 - Disable, no override</b>
  **/
  UINT8  EnableFastVmode[MAX_NUM_VRS];
  /**
    Set Vsys/Pysy Critical [0x4A]
    Write to 0x4A to program Vsys Critical Threshold which is a linear fractional value of the Vsys input full scale voltage
    where minimun is 0 and maximum is 255000mv
    For example: Vsys_Crit (0x4A) = (Critical Threshold/Full Scale) * 0xFF
  **/
  UINT32 VsysFullScale;
  UINT32 VsysCriticalThreshold;
  UINT32 PsysFullScale;
  UINT32 PsysCriticalThreshold;
} CPU_POWER_MGMT_VR_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_VR_CONFIG_H_
