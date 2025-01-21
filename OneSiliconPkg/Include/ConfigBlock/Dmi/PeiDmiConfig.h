/** @file
  Config block defination for DMI

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
#ifndef __PEI_DMI_CONFIG_H__
#define __PEI_DMI_CONFIG_H__

#include <ConfigBlock.h>
#include <Defines/HostBridgeDefines.h>

#define PEI_DMI_CONFIG_REVISION 1

#define DMI_HWEQ_COEFFS_MAX    8

#pragma pack (push,1)
///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
 typedef enum {
   DmiAspmDisabled,
   DmiAspmL0s,
   DmiAspmL1,
   DmiAspmL0sL1,
   DmiAspmAutoConfig,
   DmiAspmMax
 } DMI_ASPM;

/**
  Represent lane specific Dmi Gen3 equalization parameters.
**/
typedef struct {
  UINT8   Cm;                 ///< Coefficient C-1
  UINT8   Cp;                 ///< Coefficient C+1
  UINT8   Rsvd0[2];           ///< Reserved bytes
} DMI_EQ_PARAM;

/**
  The PEI_DMI_CONFIG block describes the expected configuration of the DMI.
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header

/**
  - <b>Auto</b> (0x0)  : Maximum possible link speed (Default)
  - Gen1        (0x1)  : Limit Link to Gen1 Speed
  - Gen2        (0x2)  : Limit Link to Gen2 Speed
  - Gen3        (0x3)  : Limit Link to Gen3 Speed
  **/
  UINT8                  DmiMaxLinkSpeed;
  /**
   <b>(Test)</b> DMI Equalization Phase 2 Enable Control
  - Disabled       (0x0) : Disable phase 2
  - Enabled        (0x1) : Enable phase 2
  - <b>Auto</b>    (0x2) : Use the current default method (Default)
  **/
  UINT8                  DmiGen3EqPh2Enable;
  /**
   <b>(Test)</b> Selects the method for performing Phase3 of Gen3 Equalization on DMI
  - <b>Auto</b> (0x0)  : Use the current default method (Default)
  - HwEq        (0x1)  : Use Adaptive Hardware Equalization
  - SwEq        (0x2)  : Use Adaptive Software Equalization (Implemented in BIOS Reference Code)
  - Static      (0x3)  : Use the Static EQs provided in DmiGen3EndPointPreset array for Phase1 AND Phase3 (Instead of just Phase1)
  - Disabled    (0x4)  : Bypass Equalization Phase 3
  **/
  UINT8                  DmiGen3EqPh3Method;
  /**
   <b>(Test)</b> Program DMI Gen3 EQ Phase1 Static Presets
  - Disabled        (0x0)  : Disable EQ Phase1 Static Presets Programming
  - <b>Enabled</b>  (0x1)  : Enable  EQ Phase1 Static Presets Programming (Default)
  **/
  UINT8                  DmiGen3ProgramStaticEq;
  UINT8                  DmiDeEmphasis;                               ///< DeEmphasis control for DMI (-6 dB and -3.5 dB are the options)
  UINT8                  DmiAspm;
  UINT8                  DmiAspmCtrl;                                 ///< ASPM configuration on the DP side of the DMI/OPI Link. Default is <b>DmiAspmAutoConfig</b>
  UINT8                  DmiAspmL1ExitLatency;                        ///< ASPM configuration on the DP side of the DMI/OPI Link. Default is <b>DmiAspmAutoConfig</b>
  UINT8                  DmiGen3RootPortPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3EndPointPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 7 is default for each lane
  UINT8                  DmiGen3EndPointHint[SA_DMI_MAX_LANE];        ///< Hint value per lane for the DMI Gen3 End Point. Range: 0-6, 2 is default for each lane

  /**
   DMI Gen3 RxCTLEp per-Bundle control. The range of the setting is (0-15). This setting
   has to be specified based upon platform design and must follow the guideline. Default is 12.
  **/

  UINT8                  DmiGen3RxCtlePeaking[SA_DMI_MAX_BUNDLE];
  UINT8                  DmiGen3DsPortRxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3DsPortTxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3UsPortRxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3UsPortTxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  Gen3EqPhase23Bypass;
  UINT8                  Gen3EqPhase3Bypass;
  UINT8                  Gen3LtcoEnable;
  UINT8                  Gen3RtcoRtpoEnable;
  UINT8                  DmiGen3Ltcpre[SA_DMI_MAX_LANE];              ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3Ltcpo[SA_DMI_MAX_LANE];               ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3RtcoCpre[SA_DMI_MAX_LANE];            ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen3RtcoCpo[SA_DMI_MAX_LANE];             ///< Used for programming DMI Gen3 preset values per lane. Range: 0-9, 8 is default for each lane
  DMI_EQ_PARAM           DmiHwEqGen3CoeffList[DMI_HWEQ_COEFFS_MAX];
  UINT8                  DmiHweq;
  UINT8                  DmiGen3DsPresetEnable;
  UINT8                  DmiGen3UsPresetEnable;
  UINT8                  DmiL1ssEnable;
  DMI_EQ_PARAM           DmiHwEqGen4CoeffList[DMI_HWEQ_COEFFS_MAX];
  UINT8                  Gen4EqPhase23Bypass;
  UINT8                  Gen4EqPhase3Bypass;
  UINT8                  DmiGen4DsPresetEnable;
  UINT8                  DmiGen4UsPresetEnable;
  UINT8                  Gen4RtcoRtpoEnable;
  UINT8                  Gen4LtcoEnable;
  UINT8                  DmiGen4Ltcpre[SA_DMI_MAX_LANE];              ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen4Ltcpo[SA_DMI_MAX_LANE];               ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen4DsPortTxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen4UsPortTxPreset[SA_DMI_MAX_LANE];      ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen4RtcoCpre[SA_DMI_MAX_LANE];            ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiGen4RtcoCpo[SA_DMI_MAX_LANE];             ///< Used for programming DMI Gen4 preset values per lane. Range: 0-9, 8 is default for each lane
  UINT8                  DmiNewFom;                                   ///< Used for programming DMI New FOM
  UINT8                  DmiCdrRelock;                                ///< Used for programming DMI CDR Relock
  UINT32                 DmiBaseAddress;
} PEI_DMI_CONFIG;

#pragma pack (pop)

#endif // __PEI_DMI_CONFIG_H__
