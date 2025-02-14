/** @file
  CPU Power Management Test Config Block.

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
#ifndef _CPU_POWER_MGMT_TEST_CONFIG_H_
#define _CPU_POWER_MGMT_TEST_CONFIG_H_

extern EFI_GUID gCpuPowerMgmtTestConfigGuid;

#pragma pack (push,1)

///
/// PPM Package C State Limit
///
typedef enum {
  PkgC0C1                 = 0,
  PkgC2,
  PkgC3,
  PkgC6,
  PkgC7,
  PkgC7s,
  PkgC8,
  PkgC9,
  PkgC10,
  PkgCMax,
  PkgCpuDefault = 254,
  PkgAuto = 255
} MAX_PKG_C_STATE;

///
/// Custom Power Units. User can choose to enter in watts or 125 milliwatt increments.
///
typedef enum {
  PowerUnitWatts = 0,     ///< in Watts.
  PowerUnit125MilliWatts, ///< in 125 milliwatt increments. Example: 90 power units times 125 mW equals 11.250 W.
  PowerUnitMax
} CUSTOM_POWER_UNIT;

///
/// PPM Interrupt Redirection Mode Selection
///
typedef enum {
  PpmIrmFixedPriority     = 0,
  PpmIrmRoundRobin,
  PpmIrmHashVector,
  PpmIrmReserved1,
  PpmIrmReserved2,
  PpmIrmReserved3,
  PpmIrmReserved4,
  PpmIrmNoChange
} PPM_IRM_SETTING;

/**
  CPU Power Management Test Configuration Structure.

  <b>Revision 1</b>:
  - Initial version.
**/

#define CPU_POWER_MGMT_TEST_CONFIG_REVISION 1

typedef struct {
  CONFIG_BLOCK_HEADER   Header;                    ///< Offset 0-27  Config Block Header
  UINT32 Eist                          : 1;        ///< Offset 28-31 Enable or Disable Intel SpeedStep Technology. 0: Disable; <b>1: Enable</b>
  UINT32 EnergyEfficientPState         : 1;        ///<              Enable or Disable Energy Efficient P-state will be applied in Turbo mode. Disable; <b>1: Enable</b>
  UINT32 EnergyEfficientTurbo          : 1;        ///<              Enable or Disable Energy Efficient Turbo, will be applied in Turbo mode.[Desktop Sku] <b>0: Disable</b>; 1: Enable [Mobile Sku] 0: Disable; <b>1: Enable</b>
  UINT32 TStates                       : 1;        ///<              Enable or Disable T states; <b>0: Disable</b>; 1: Enable.
  UINT32 EnableAllThermalFunctions     : 1;        ///<              Enable or Disable Thermal Reporting through ACPI tables; 0: Disable; <b>1: Enable</b>.
  UINT32 Cx                            : 1;        ///<              Enable or Disable CPU power states (C-states). 0: Disable; <b>1: Enable</b>
  UINT32 PmgCstCfgCtrlLock             : 1;        ///<              If enabled, sets MSR 0xE2[15]; 0: Disable; <b>1: Enable</b>.
  UINT32 C1AutoDemotion                : 1;        ///<              Enable or Disable C6/C7 auto demotion to C1. 0: Disabled; <b>1: C1 Auto demotion</b>
  UINT32 C1UnDemotion                  : 1;        ///<              Enable or Disable C1UnDemotion. 0: Disabled; <b>1: C1 Auto undemotion</b>
  UINT32 PkgCStateDemotion             : 1;        ///<              Enable or Disable Package Cstate Demotion. Disable; <b>1: Enable</b> [WhiskeyLake] <b>Disable</b>; 1: Enable
  UINT32 PkgCStateUnDemotion           : 1;        ///<              Enable or Disable Package Cstate UnDemotion. Disable; <b>1: Enable</b> [WhiskeyLake] <b>Disable</b>; 1: Enable
  UINT32 CStatePreWake                 : 1;        ///<              Enable or Disable CState-Pre wake. Disable; <b>1: Enable</b>
  UINT32 TimedMwait                    : 1;        ///<              Enable or Disable TimedMwait Support. <b>Disable</b>; 1: Enable
  UINT32 RaceToHalt                    : 1;        ///<              Enable or Disable Race To Halt feature; 0: Disable; <b>1: Enable </b>. RTH will dynamically increase CPU frequency in order to enter pkg C-State faster to reduce overall power. (RTH is controlled through MSR 1FC bit 20)
  UINT32 ForcePrDemotion               : 1;        ///<              FORCEPR Demotion Algorithm configuration; 0: Disable; <b> 1: Hardware Default</b>
  UINT32 VrAlertDemotion               : 1;        ///<              VR Alert Demotion Algorithm configuration; 0: Disable; <b> 1: Enable</b>
  UINT32 RsvdBits                      : 16;       ///<              Reserved for future use.
  MAX_PKG_C_STATE   PkgCStateLimit;                ///< Offset 32    This field is used to set the Max Pkg Cstate. Default set to Auto which limits the Max Pkg Cstate to deep C-state.
  /**
  Offset 36  Interrupt Redirection Mode Select.
   - 0: Fixed priority.             //Default under CNL.
   - 1: Round robin.
   - 2: Hash vector.
   - 4: PAIR with fixed priority.   //Default under KBL, not available under CNL.
   - 5: PAIR with round robin.      //Not available under CNL.
   - 6: PAIR with hash vector.      //Not available under CNL.
   - 7: No change.
  **/
  PPM_IRM_SETTING      PpmIrmSetting;
  // Move the padding to previous offset to align the structure at 32-bit address.
  UINT8  Rsvd[16];                                 ///< Offset 40-56 Reserved for future use and config block alignment
} CPU_POWER_MGMT_TEST_CONFIG;

#pragma pack (pop)

#endif // _CPU_POWER_MGMT_TEST_CONFIG_H_
