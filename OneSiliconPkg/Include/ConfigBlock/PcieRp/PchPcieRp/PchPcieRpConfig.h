/** @file
  PCH Pcie root port policy

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
#ifndef _PCH_PCIERP_CONFIG_H_
#define _PCH_PCIERP_CONFIG_H_

#include <Library/PchLimits.h>
#include <PcieConfig.h>
#include <ConfigBlock.h>

/*
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n

<b>Revision 1</b>:  - Initial version.
<b>Revision 2</b>:
FomsCp - Deprecated
<b>Revision 3</b>:
Added PCIE_EQ_PARAM  HwEqGen3CoeffList for all CPU_PCIE_MAX_ROOT_PORTS
Added PCIE_EQ_PARAM  HwEqGen4CoeffList for all CPU_PCIE_MAX_ROOT_PORTS
Added PCIE_EQ_PARAM  HwEqGen5CoeffList for all CPU_PCIE_MAX_ROOT_PORTS
<b>Revision 4</b>:
AllowL0sWithGen3 - Deprecated
Moved EnablePeerMemoryWrite policy from PCIE_COMMON_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Moved ClockGating policy from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Moved PowerGating policy from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Moved EnablePort8xhDecode policy form PCH_PCIE_CONFIG to PCIE_COMMON_CONFIG
Added LinkDownGpios policy to PCIE_ROOT_PORT_COMMON_CONFIG
<b>Revision 5</b>:
Deprecated PCIE_EQ_PARAM  HwEqGen3CoeffList for all CPU_PCIE_MAX_ROOT_PORTS
<b>Revision 6</b>:
Move PcieGen3LinkEqPlatformSettings policies from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Move PcieGen4LinkEqPlatformSettings policies from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Move PcieGen5LinkEqPlatformSettings policies from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
Move OverrideEqualizationDefaults policy from CPU_PCIE_CONFIG to PCIE_ROOT_PORT_COMMON_CONFIG
<b>Revision 7</b>:
Added EqPhBypass polices to PCIE_ROOT_PORT_COMMON_CONFIG
<b>Revision 8</b>:
Deprecated EqPhBypass policies from PCIE_ROOT_PORT_COMMON_CONFIG
*/
#define PCH_PCIE_CONFIG_REVISION  8

/*
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n

<b>Revision 1</b>:  - Initial version.
*/
#define PCH_PCIE_RP_PREMEM_CONFIG_REVISION 2

/*
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n

<b>Revision 1</b>:  - Initial version.
*/
#define PCIE_RP_DXE_CONFIG_REVISION 1

extern EFI_GUID gPchPcieConfigGuid;
extern EFI_GUID gPcieRpPreMemConfigGuid;

#pragma pack (push,1)

typedef enum {
  PchPcieOverrideDisabled             = 0,
  PchPcieL1L2Override                 = 0x01,
  PchPcieL1SubstatesOverride          = 0x02,
  PchPcieL1L2AndL1SubstatesOverride   = 0x03,
  PchPcieLtrOverride                  = 0x04
} PCH_PCIE_OVERRIDE_CONFIG;

///
/// The values before AutoConfig match the setting of PCI Express Base Specification 1.1, please be careful for adding new feature
///
typedef enum {
  PchPcieAspmDisabled,
  PchPcieAspmL0s,
  PchPcieAspmL1,
  PchPcieAspmL0sL1,
  PchPcieAspmAutoConfig,
  PchPcieAspmMax
} PCH_PCIE_ASPM_CONTROL;

/**
  Refer to PCH EDS for the PCH implementation values corresponding
  to below PCI-E spec defined ranges
**/
typedef enum {
  PchPcieL1SubstatesDisabled,
  PchPcieL1SubstatesL1_1,
  PchPcieL1SubstatesL1_1_2,
  PchPcieL1SubstatesMax
} PCH_PCIE_L1SUBSTATES_CONTROL;

enum PCH_PCIE_MAX_PAYLOAD {
  PchPcieMaxPayload128 = 0,
  PchPcieMaxPayload256,
  PchPcieMaxPayload512,
  PchPcieMaxPayload64 = 7,
  PchPcieMaxPayloadMax
};

#define PCH_PCIE_NO_SUCH_CLOCK  0xFF
#define PCH_PCIE_NO_GPIO_MUXING 0x00

typedef enum {
  PchClockUsagePchPcie0      = 0,
  PchClockUsagePchPcie1      = 1,
  PchClockUsagePchPcie2      = 2,
  PchClockUsagePchPcie3      = 3,
  PchClockUsagePchPcie4      = 4,
  PchClockUsagePchPcie5      = 5,
  PchClockUsagePchPcie6      = 6,
  PchClockUsagePchPcie7      = 7,
  PchClockUsagePchPcie8      = 8,
  PchClockUsagePchPcie9      = 9,
  PchClockUsagePchPcie10     = 10,
  PchClockUsagePchPcie11     = 11,
  PchClockUsagePchPcie12     = 12,
  PchClockUsagePchPcie13     = 13,
  PchClockUsagePchPcie14     = 14,
  PchClockUsagePchPcie15     = 15,
  PchClockUsagePchPcie16     = 16,
  PchClockUsagePchPcie17     = 17,
  PchClockUsagePchPcie18     = 18,
  PchClockUsagePchPcie19     = 19,
  PchClockUsagePchPcie20     = 20,
  PchClockUsagePchPcie21     = 21,
  PchClockUsagePchPcie22     = 22,
  PchClockUsagePchPcie23     = 23,
  /**
    Quantity of PCH and CPU PCIe ports, as well as their encoding in this enum, may change between
    silicon generations and series. Do not assume that PCH port 0 will be always encoded by 0.
    Instead, it is recommended to use (PchClockUsagePchPcie0 + PchPortIndex) style to be forward-compatible
  **/
  PchClockUsageCpuPcie0      = 0x40,
  PchClockUsageCpuPcie1      = 0x41,
  PchClockUsageCpuPcie2      = 0x42,
  PchClockUsageCpuPcie3      = 0x43,

  PchClockUsageLan           = 0x70,
  PchClockUsageUnspecified   = 0x80, ///< In use for a purpose not listed above
  PchClockUsageNotUsed       = 0xFF
} PCH_PCIE_CLOCK_USAGE;

/**
  PCH_PCIE_CLOCK describes PCIe source clock generated by PCH.
**/
typedef struct {
  UINT8   Usage;             ///< Purpose of given clock (see PCH_PCIE_CLOCK_USAGE). Default: Unused, 0xFF
  UINT8   ClkReq;            ///< ClkSrc - ClkReq mapping. Default: 1:1 mapping with Clock numbers
  UINT8   RsvdBytes[2];      ///< Reserved byte
  UINT32  ClkReqGpioPinMux;  /// Muxed GPIO details. Refer GPIO_*_MUXING_SRC_CLKREQ_x*
} PCH_PCIE_CLOCK;

///
/// The PCH_DMI_LANE_CONFIG block describes specific lane configuration
///
typedef struct {
  UINT32   DmiTranCoOverEn            : 2;    ///< Enable/Disable Lane Transmitter Coefficient
  UINT32   Rsvd1                      : 6;    ///< Reserved
  UINT32   DmiTranCoOverPostCur       : 6;    ///< Lane Transmitter Post-Cursor Coefficient Override
  UINT32   Rsvd2                      : 2;    ///< Reserved
  UINT32   DmiTranCoOverPreCur        : 6;    ///< Lane Transmitter Pre-Cursor Coefficient Override
  UINT32   Rsvd3                      : 2;    ///< Reserved
  UINT32   DmiUpPortTranPreset        : 4;    ///< Upstream Port Lane Transmitter Preset
  UINT32   Rsvd4                      : 4;    ///< Reserved
} DMI_LANE_CONFIG;

typedef struct {
  DMI_LANE_CONFIG   Lane[PCH_MAX_DMI_LANES];               ///< DMI Lanes configuration
  UINT32            DmiUpPortTranPresetEn  :  2;           ///< 0: POR setting, 1: force enable, 2: force disable.
  UINT32            DmiRtlepceb            :  2;           ///< DMI Remote Transmit Link Equalization Preset/Coefficient Evaluation Bypass (RTLEPCEB)
  /**
   When CNP is paired with SKL CPU, TX SAI Filtering Check performed for cycles
   targeting MEUMA (ie. VCm upstream). As part of this check,
   IOSF Root Space attribute is check to ensure that it is RS3.
   Setting valid only when DMI VCm is enabled.
   0: POR setting, 1: force enable, 2: force disable.
   */
  UINT32            TestDmiMeUmaRootSpaceCheck :  2;
  UINT32            Rsvdbits                   : 26;
  UINT8             MvcEnabled;                            ///< Enable/Disable MultiVC Option. 0: Disable; 1: Enable
  UINT8             Reserved[3];
} DMI_PREMEM_CONFIG;

/**
  The PCH_PCI_EXPRESS_ROOT_PORT_CONFIG describe the feature and capability of each PCH PCIe root port.
**/
typedef struct {
  PCIE_ROOT_PORT_COMMON_CONFIG  PcieRpCommonConfig; ///an instance of Pcie Common Config
  UINT8  ExtSync;              ///< Indicate whether the extended synch is enabled. <b>0: Disable</b>; 1: Enable.
  //
  // Error handlings
  //
  UINT8  SystemErrorEnable;    ///< Indicate whether the System Error is enabled. <b>0: Disable</b>; 1: Enable.
  /**
    The Multiple VC (MVC) supports hardware to avoid HoQ block for latency sensitive TC.
    Currently it is only applicable to Root Ports with 2pX4 port configuration with 2 VCs,or
    DMI port configuration with 3 VCs. For Root Ports 2pX4 configuration, two RPs (RP0,
    RP2) shall support two PCIe VCs (VC0 & VC1) and the other RPs (RP1, RP3) shall be
    disabled.
    <b>0: Disable</b>; 1: Enable
  **/
  UINT8  MvcEnabled;
  /**
    Virtual Pin Port is industry standard introduced to PCIe Hot Plug support in systems
    when GPIO pins expansion is needed. It is server specific feature.
    <b>0x00: Default</b>; 0xFF: Disabled
  **/
  UINT8   VppPort;
  UINT8   VppAddress;                               ///< PCIe Hot Plug VPP SMBus Address. Default is zero.
  UINT8   MultiVc;
  UINT8   Vc1Ctrl;
  UINT8   VcMCtrl;
  UINT8   MeUmaRootSpaceCheck;
  UINT8   RsvdBytes237[3];
} PCH_PCIE_ROOT_PORT_CONFIG;

/**
  The PCH_PCIE_CONFIG block describes the expected configuration of the PCH PCI Express controllers
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Moved EnablePort8xhDecode policy to PCIE_COMMON_CONFIG
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  ///
  /// These members describe the configuration of each PCH PCIe root port.
  ///
  PCIE_COMMON_CONFIG                PcieCommonConfig;
  PCH_PCIE_ROOT_PORT_CONFIG         RootPort[PCH_MAX_PCIE_ROOT_PORTS];
  PCH_PCIE_ROOT_PORT_CONFIG         DmiPort;

  ///
  /// <b>(Test)</b> The Index of PCIe Port that is selected for Port8xh Decode (0 Based)
  ///
  UINT8                             PchPciePort8xhDecodePortIndex;
  UINT8                             DmiPowerReduction;
  UINT8                             RsvdBytes0[2];

  ///
  /// This member describes whether the PCI Express Clock Gating for each root port
  /// is enabled by platform modules. <b>O: POR</b>; 1: Enable; 2: Disable
  ///
  UINT32                            PcieClockGating;

} PCH_PCIE_CONFIG;

/**
  The PCH_PCIE_RP_PREMEM_CONFIG block describes early configuration of the PCH PCI Express controllers
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                                ///< Config Block Header
  /**
    Root Port enabling mask.
    Bit0 presents RP1, Bit1 presents RP2, and so on.
    0: Disable; <b>1: Enable</b>.
  **/
  UINT32                RpEnabledMask;
  /// Configuration of PCIe source clocks
  ///
  PCH_PCIE_CLOCK        PcieClock[PCH_MAX_PCIE_CLOCKS];

  /**
    Per Controller Bifurcation Configuration
    <b>0: Disabled</b>; 1: 4x1; 2: 1x2_2x1; 3: 2x2; 4: 1x4; 5: 4x2; 6: 1x4_2x2; 7: 2x2_1x4; 8: 2x4; 9: 1x8 (see: PCIE_BIFURCATION_CONFIG)
  **/
  UINT8                 Bifurcation[PCH_MAX_PCIE_CONTROLLERS];
  UINT8                 Rsvd[(4 - PCH_MAX_PCIE_CONTROLLERS % 4)];
  DMI_PREMEM_CONFIG     PchDmiPrememConfig;
  UINT8                 PcieResizableBarSupport;
  UINT8                 SkipExtGfxScan;                       ///< <b>(Test)</b> :1=Skip External Gfx Device Scan; <b>0=Scan for external graphics devices</b>. Set this policy to skip External Graphics card scanning if the platform uses Internal Graphics only.
  UINT8                 Rsvd2[2];
} PCH_PCIE_RP_PREMEM_CONFIG;

/**
  The PCIE_RP_DXE_CONFIG block describes the expected configuration of the PCH PCI Express controllers in DXE phase

  <b>Revision 1</b>:
  - Init version
**/
typedef struct {
  CONFIG_BLOCK_HEADER      Header;                     ///< Config Block Header
  UINT8                    RsvdBytes323[4];            ///< Reserved
  /**
    PCIe device override table
    The PCIe device table is being used to override PCIe device ASPM settings.
    And it's only used in DXE phase.
    Please refer to PCIE_DEVICE_OVERRIDE structure for the table.
    Last entry VendorId must be 0.
  **/
  PCIE_DEVICE_OVERRIDE     *PcieDeviceOverrideTablePtr;
} PCIE_RP_DXE_CONFIG;

#pragma pack (pop)

#endif // _PCH_PCIERP_CONFIG_H_
