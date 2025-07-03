## @file
#  Platform description.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification
##

  ################################################################################
  #
  # UPDs consumed in FspSiliconInit Api
  #
  ################################################################################
  # !BSF FIND:{PTLUPD_S}
  # !HDR COMMENT:{FSP_UPD_HEADER:FSP UPD Header}
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:START}
  # FspsUpdSignature: {PTLUPD_S}
  gPlatformFspPkgTokenSpaceGuid.Signature                   | * | 0x08 | 0x535F4450554C5450
  # !BSF NAME:{FspsUpdRevision}  TYPE:{None}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  # !HDR EMBED:{FSP_UPD_HEADER:FspUpdHeader:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x17 | {0x00}

  # !HDR COMMENT:{FSPS_ARCH2_UPD:FSPS_ARCH_UPD}
  # !HDR EMBED:{FSPS_ARCH2_UPD:FspsArchUpd:START}
  gPlatformFspPkgTokenSpaceGuid.Revision                    | * | 0x01 | 0x02
  gPlatformFspPkgTokenSpaceGuid.Reserved                    | * | 0x03 | {0x00}
  gPlatformFspPkgTokenSpaceGuid.Length                      | * | 0x04 | 0x00000020
  gPlatformFspPkgTokenSpaceGuid.FspEventHandler             | * | 0x08 | 0x00000000

  # !HDR EMBED:{FSPS_ARCH2_UPD:FspsArchUpd:END}
  gPlatformFspPkgTokenSpaceGuid.Reserved1                   | * | 0x10 | {0x00}

  # !HDR COMMENT:{FSP_S_CONFIG:Fsp S Configuration}
  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:START}

  ## BIOS_GUARD_CONFIG Start

  # !BSF NAME:{BgpdtHash[6]} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT64}
  # !BSF HELP:{BgpdtHash values}
  gPlatformFspPkgTokenSpaceGuid.BgpdtHash                   | * | 0x30 | {0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000, 0x0000000000000000}

  # !BSF NAME:{BiosGuardAttr} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{BiosGuardAttr default values}
  gPlatformFspPkgTokenSpaceGuid.BiosGuardAttr               | * | 0x4 | 0xFFFFFFFF
  # Added reserved space  UnusedUpdSpace0[4]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd0                | * | 0x4 | {0x00}

  # !BSF NAME:{BiosGuardModulePtr} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{BiosGuardModulePtr default values}
  gPlatformFspPkgTokenSpaceGuid.BiosGuardModulePtr          | * | 0x8 | 0xFFFFFFFFFFFFFFFF

  # !BSF NAME:{EcProvisionEav} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{EcProvisionEav function pointer. \n @code typedef EFI_STATUS (EFIAPI *EC_PROVISION_EAV) (IN UINT32 Eav, OUT UINT8  *ReturnValue); @endcode}
  gPlatformFspPkgTokenSpaceGuid.EcProvisionEav              | * | 0x8 | 0xFFFFFFFFFFFFFFFF

  # !BSF NAME:{EcBiosGuardCmdLock} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{EcBiosGuardCmdLock function pointer. \n @code typedef EFI_STATUS (EFIAPI *EC_CMD_LOCK) (OUT UINT8 *ReturnValue); @endcode}
  gPlatformFspPkgTokenSpaceGuid.EcBiosGuardCmdLock          | * | 0x8 | 0xFFFFFFFFFFFFFFFF

  ## BIOS_GUARD_CONFIG End

  ## PCH_ESPI_CONFIG Start

  # !BSF NAME:{PCH eSPI Host and Device BME enabled} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{PCH eSPI Host and Device BME enabled}
  gPlatformFspPkgTokenSpaceGuid.PchEspiBmeHostDeviceEnabled        | * | 0x01 | 0x01

  # !BSF NAME:{PCH eSPI Link Configuration Lock (SBLCL)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable lock of communication through SET_CONFIG/GET_CONFIG to eSPI target addresseses from range 0x0 - 0x7FF}
  gPlatformFspPkgTokenSpaceGuid.PchEspiLockLinkConfiguration        | * | 0x01 | 0x01

  # !BSF NAME:{Enable Host C10 reporting through eSPI} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Host C10 reporting to Device via eSPI Virtual Wire.}
  gPlatformFspPkgTokenSpaceGuid.PchEspiHostC10ReportEnable  | * | 0x01 | 0x0

  # !BSF NAME:{Espi Lgmr Memory Range decode } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This option enables or disables espi lgmr }
  gPlatformFspPkgTokenSpaceGuid.PchEspiLgmrEnable           | * | 0x01 | 0x00

  # !BSF NAME:{PCH eSPI PmHAE} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Hardware Autonomous Enable (HAE)
  #If set to TRUE, then the IP may request a PG whenever it is idle}
  gPlatformFspPkgTokenSpaceGuid.PchEspiPmHAE        | * | 0x01 | 0x01

  # !BSF NAME:{PCH eSPI HideNonFatalErrors } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable Non-Fatal Error Reporting (PCNFEE)
  #If set to TRUE, then NFE bit of PCERR_SLV0 and PCERR_SLV1 is cleared which disables non-fatal error reporting}
  gPlatformFspPkgTokenSpaceGuid.PchEspiHideNonFatalErrors  | * | 0x01 | 0x00

  # !BSF NAME:{PCH eSPI NmiEnableCs1 } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set this bit to enable eSPI NMI VW events to be processed by the SOC}
  gPlatformFspPkgTokenSpaceGuid.PchEspiNmiEnableCs1  | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace1
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd1                  | * | 0x1 | 0x00

  ## PCH_ESPI_CONFIG End

  #
  # CPU Post-Mem Block Start
  #
  # !BSF PAGE:{CPU2}

  # !BSF NAME:{CpuBistData} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Pointer CPU BIST Data}
  gPlatformFspPkgTokenSpaceGuid.CpuBistData                 | * | 0x8 | 0

  # !BSF NAME:{CpuMpPpi} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{<b>Optional</b> pointer to the boot loader's implementation of EFI_PEI_MP_SERVICES_PPI. If not NULL, FSP will use the boot loader's implementation of multiprocessing. See section 5.1.4 of the FSP Integration Guide for more details.}
  gPlatformFspPkgTokenSpaceGuid.CpuMpPpi                     | * | 0x8 | 0

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  # !BSF NAME:{AC Split Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable #AC check on split lock. <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.AcSplitLock                  | * | 0x1 | 0x0

  # !BSF NAME:{TccMode enable/disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{DEPRECATED.Enable will activate TCC Mode and disable will deactivate TCC Mode .This is temporary to skip some of the TCC related register access in Non-RT SKU's}
  gPlatformFspPkgTokenSpaceGuid.TccMode | * | 0x01 | 0x00
!endif
  ## TELEMETRY_PEI_PREMEM_CONFIG Start

  # !BSF NAME:{Enable/Disable CrashLog} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable CPU CrashLog, Disable: Disable CPU CrashLog}
  gPlatformFspPkgTokenSpaceGuid.CpuCrashLogEnable           | * | 0x01 | 0x1
  # Added reserved space  UnusedUpdSpace2[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd2                 | * | 0x3 | {0x00}

  # !BSF NAME:{StreamTracer Mode} TYPE:{Combo}
  # !BSF OPTION:{0: Disable (Default), 524288: Advanced Tracing , 8192: Auto , 3: User input}
  # !BSF HELP:{Disable: Disable StreamTracer, Advanced Tracing: StreamTracer size 512MB - Recommended when all groups in high verbosity are traced in 'red', Auto: StreamTracer size 8MB - Recommended when using up to 8 groups red or up to 16 groups in green in med verbosity, User input: Allow User to enter a size in the range of 64KB-512MB}
  gPlatformFspPkgTokenSpaceGuid.StreamTracerMode             | * | 0x04 | 0x00

  ## TELEMETRY_PEI_PREMEM_CONFIG End

  ## CPU_INIT_CONFIG Start

  # !BSF NAME:{MicrocodeRegionBase} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Memory Base of Microcode Updates}
  gPlatformFspPkgTokenSpaceGuid.MicrocodeRegionBase         | * | 0x08 | 0x0

  # !BSF NAME:{MicrocodeRegionSize} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Size of Microcode Updates}
  gPlatformFspPkgTokenSpaceGuid.MicrocodeRegionSize         | * | 0x08 | 0x0

  # !BSF NAME:{Enable or Disable TXT} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables utilization of additional hardware capabilities provided by Intel (R) Trusted Execution Technology. Changes require a full power cycle to take effect. <b>0: Disable</b>, 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.TxtEnable                   | * | 0x01 | 0

  # !BSF NAME:{PpinSupport to view Protected Processor Inventory Number}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1: Enable, 2: Auto}
  # !BSF HELP:{PPIN Feature Support to view Protected Processor Inventory Number. Disable to turn off this feature. When 'PPIN Enable Mode' is selected, this shows second option where feature can be enabled based on EOM (End of Manufacturing) flag or it is always enabled}
  gPlatformFspPkgTokenSpaceGuid.PpinSupport                 | * | 0x1 | 0x00

  # !BSF NAME:{Advanced Encryption Standard (AES) feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Advanced Encryption Standard (AES) feature; </b>0: Disable; <b>1: Enable}
  gPlatformFspPkgTokenSpaceGuid.AesEnable                   | * | 0x01 | 0x01

  # !BSF NAME:{AvxDisable}
  # !BSF TYPE:{Combo} OPTION:{0: Enable, 1: Disable}
  # !BSF HELP:{Enable/Disable the AVX and AVX2 Instructions}
  gPlatformFspPkgTokenSpaceGuid.AvxDisable                  | * | 0x1 | 0x00

  # !BSF NAME:{X2ApicEnable}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable X2APIC Operating Mode. When this option is configured as 'Enabled', 'VT-d' option must be 'Enabled'.}
  gPlatformFspPkgTokenSpaceGuid.X2ApicEnable                | * | 0x1 | 0x01

  ## CPU_INIT_CONFIG End

  ## CPU_POWER_MGMT_TEST_CONFIG Start

  # !BSF NAME:{P-state ratios for max 16 version of custom P-state table} TYPE:{EditNum, HEX, (0x00,0x7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{P-state ratios for max 16 version of custom P-state table. This table is used for OS versions limited to a max of 16 P-States. If the first entry of this table is 0, or if Number of Entries is 16 or less, then this table will be ignored, and up to the top 16 values of the StateRatio table will be used instead. Valid Range of each entry is 0 to 0x7F}
  gPlatformFspPkgTokenSpaceGuid.StateRatioMax16             | * | 0x10 | {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}

  # !BSF NAME:{Enable or Disable Intel SpeedStep Technology} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Allows more than two frequency ranges to be supported. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.Eist                        | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable Energy Efficient P-state} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Energy Efficient P-state feature. When set to 0, will disable access to ENERGY_PERFORMANCE_BIAS MSR and CPUID Function will read 0 indicating no support for Energy Efficient policy setting. When set to 1 will enable access to ENERGY_PERFORMANCE_BIAS MSR and CPUID Function will read 1 indicating Energy Efficient policy setting is supported. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.EnergyEfficientPState       | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable Energy Efficient Turbo} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Energy Efficient Turbo Feature. This feature will opportunistically lower the turbo frequency to increase efficiency. Recommended only to disable in overclocking situations where turbo frequency must remain constant. Otherwise, leave enabled. <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.EnergyEfficientTurbo        | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable T states} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable T states; <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.TStates                     | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable Thermal Reporting} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Thermal Reporting through ACPI tables; 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.EnableAllThermalFunctions        | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable CPU power states (C-states)} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CPU Power Management. Allows CPU to go to C states when it's not 100% utilized. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.Cx                          | * | 0x01 | 0x01

  # !BSF NAME:{Configure C-State Configuration Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Configure MSR to CFG Lock bit. 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.PmgCstCfgCtrlLock           | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable Package Cstate Demotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Package C-State Demotion. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.PkgCStateDemotion           | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable Package Cstate UnDemotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Package C-State Un-Demotion. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.PkgCStateUnDemotion         | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable CState-Pre wake} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable - to disable the Cstate Pre-Wake. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.CStatePreWake               | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable TimedMwait Support.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable TimedMwait Support. <b>0: Disable</b>; 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.TimedMwait                  | * | 0x01 | 0x00

  # !BSF NAME:{Set the Max Pkg Cstate}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Maximum Package C State Limit Setting. Cpu Default: Leaves to Factory default value. Auto: Initializes to deepest available Package C State Limit. Valid values 0 - C0/C1, 1 - C2, 2 - C3, 3 - C6, 4 - C7, 5 - C7S, 6 - C8, 7 - C9, 8 - C10, 254 - CPU Default, <b>255 - Auto</b>}
  gPlatformFspPkgTokenSpaceGuid.PkgCStateLimit              | * | 0x01 | 0xFF

  # !BSF NAME:{ForcePr Demotion Algorithm configuration}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1: Enable}
  # !BSF HELP:{ForcePr Demotion Algorithm configuration. 0: Disable;<b> 1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.ForcePrDemotion              | * | 0x01 | 0x01

  # !BSF NAME:{VrAlert Demotion Algorithm configuration}
  # !BSF TYPE:{Combo} OPTION:{0: Disable, 1: Enable}
  # !BSF HELP:{VrAlert Demotion Algorithm configuration. 0: Disable;<b> 1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.VrAlertDemotion              | * | 0x01 | 0x01

  # !BSF NAME:{Interrupt Redirection Mode Select}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7)}
  # !BSF HELP:{Interrupt Redirection Mode Select for Logical Interrupts. 0: Fixed priority; 1: Round robin; 2: Hash vector; 7: No change.}
  gPlatformFspPkgTokenSpaceGuid.PpmIrmSetting               | * | 0x01 | 0x00

  ## CPU_POWER_MGMT_TEST_CONFIG End

  ## CPU_POWER_MGMT_BASIC_CONFIG Start

  # !BSF NAME:{Turbo Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable processor Turbo Mode. 0:disable, <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.TurboMode                   | * | 0x01 | 0x1

  # !BSF NAME:{Power Floor PCIe Gen Downgrade} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{SoC can downgrade PCIe gen speed to lower SoC floor power (Default enabled).  0: Disable: Reduction in PCIe gen speed will not be used by SoC., <b>1: Enable </b>}
  gPlatformFspPkgTokenSpaceGuid.PowerFloorPcieGenDowngrade | * | 0x01 | 0x01

  # !BSF NAME:{P-state ratios for custom P-state table} TYPE:{EditNum, HEX, (0x00,0x7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F7F)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{P-state ratios for custom P-state table. NumberOfEntries has valid range between 0 to 40. For no. of P-States supported(NumberOfEntries) , StateRatio[NumberOfEntries] are configurable. Valid Range of each entry is 0 to 0x7F}
  gPlatformFspPkgTokenSpaceGuid.StateRatio                  | * | 0x28 | {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0}

  # !BSF NAME:{Custom Ratio State Entries}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x28)}
  # !BSF HELP:{The number of custom ratio state entries, ranges from 0 to 40 for a valid custom ratio table. Sets the number of custom P-states. At least 2 states must be present}
  gPlatformFspPkgTokenSpaceGuid.NumberOfEntries             | * | 0x01 | 0x00

  # !BSF NAME:{Max P-State Ratio}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x7F)}
  # !BSF HELP:{Maximum P-state ratio to use in the custom P-state table. Valid Range 0 to 0x7F}
  gPlatformFspPkgTokenSpaceGuid.MaxRatio                    | * | 0x01 | 0x00

  # !BSF NAME:{Boot frequency}
  # !BSF TYPE:{Combo} OPTION:{0:0, 1:1, 2:2}
  # !BSF HELP:{Select the performance state that the BIOS will set starting from reset vector. 0: Maximum battery performance. 1: Maximum non-turbo performance. <b>2: Turbo performance </b>}
  gPlatformFspPkgTokenSpaceGuid.BootFrequency                 | * | 0x01 | 0x02

  # !BSF NAME:{Turbo settings Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable locking of Package Power Limit settings. When enabled, PACKAGE_POWER_LIMIT MSR will be locked and a reset will be required to unlock the register. <b>0: Disable; </b> 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.TurboPowerLimitLock         | * | 0x01 | 0x00

  # !BSF NAME:{FastMsrHwpReq} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable; <b> 1: Enable;</b>}
  gPlatformFspPkgTokenSpaceGuid.EnableFastMsrHwpReq      | * | 0x01 | 0x00

  #
  # Turbo Ratio Limit Block Start
  #

  # !BSF NAME:{Turbo Ratio Limit Ratio array} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Performance-core Turbo Ratio Limit Ratio0-7 (TRLR) defines the turbo ratio (max is 85 in normal mode and 120 in core extension mode). Ratio[0]: This Turbo Ratio Limit Ratio0 must be greater than or equal all other ratio values. If this value is invalid, thn set all other active cores to minimum. Otherwise, align the Ratio Limit to 0. Please check each active cores. Ratio[1~7]: This Turbo Ratio Limit Ratio1 must be <= to Turbo Ratio Limit Ratio0~6.}
  gPlatformFspPkgTokenSpaceGuid.TurboRatioLimitRatio        | * | 0x8 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Turbo Ratio Limit Num Core array} TTYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Performance-core Turbo Ratio Limit Core0~7 defines the core range, the turbo ratio is defined in Turbo Ratio Limit Ratio0~7. If value is zero, this entry is ignored.}
  gPlatformFspPkgTokenSpaceGuid.TurboRatioLimitNumCore      | * | 0x8 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{ATOM Turbo Ratio Limit Ratio array} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Efficient-core Turbo Ratio Limit Ratio0-7 defines the turbo ratio (max is 85 irrespective of the core extension mode), the core range is defined in E-core Turbo Ratio Limit CoreCount0-7.}
  gPlatformFspPkgTokenSpaceGuid.AtomTurboRatioLimitRatio    | * | 0x8 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{ATOM Turbo Ratio Limit Num Core array} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Efficient-core Turbo Ratio Limit CoreCount0-7 defines the core range, the turbo ratio is defined in E-core Turbo Ratio Limit Ratio0-7. If value is zero, this entry is ignored.}
  gPlatformFspPkgTokenSpaceGuid.AtomTurboRatioLimitNumCore  | * | 0x8 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  #
  # Turbo Ratio Limit Block End
  #

  # !BSF NAME:{Race To Halt} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Race To Halt feature. RTH will dynamically increase CPU frequency in order to enter pkg C-State faster to reduce overall power. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.RaceToHalt                  | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable C1 Cstate Demotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable C1 Cstate Auto Demotion. Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.C1StateAutoDemotion           | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable C1 Cstate UnDemotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable C1 Cstate Un-Demotion. Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.C1StateUnDemotion         | * | 0x01 | 0x01

  # !BSF NAME:{Minimum Ring ratio limit override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x53)}
  # !BSF HELP:{Minimum Ring ratio limit override. <b>0: Hardware defaults.</b> Range: 0 - Max turbo ratio limit}
  gPlatformFspPkgTokenSpaceGuid.MinRingRatioLimit           | * | 0x01 | 0x00

  # !BSF NAME:{Maximum Ring ratio limit override}
  # !BSF TYPE:{EditNum, HEX, (0x00, 0x53)}
  # !BSF HELP:{Maximum Ring ratio limit override. <b>0: Hardware defaults.</b> Range: 0 - Max turbo ratio limit}
  gPlatformFspPkgTokenSpaceGuid.MaxRingRatioLimit           | * | 0x01 | 0x00

  # !BSF NAME:{Resource Priority Feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Resource Priority Feature. Enable/Disable; 0: Disable, <b>1: Enable </b>}
  gPlatformFspPkgTokenSpaceGuid.EnableRp                    | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable HWP} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Intel(R) Speed Shift Technology support. Enabling will expose the CPPC v2 interface to allow for hardware controlled P-states. 0: Disable; <b>1: Enable;</b>}
  gPlatformFspPkgTokenSpaceGuid.Hwp                         | * | 0x01 | 0x01

  # !BSF NAME:{Set HW P-State Interrupts Enabled for for MISC_PWR_MGMT} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set HW P-State Interrupts Enabled for for MISC_PWR_MGMT; <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.HwpInterruptControl         | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable HwP Autonomous Per Core P State OS control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable Autonomous PCPS Autonomous will request the same value for all cores all the time. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.EnableHwpAutoPerCorePstate | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable HwP Autonomous EPP Grouping} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable EPP grouping Autonomous will request the same values for all cores with same EPP. Disable EPP grouping autonomous will not necessarily request same values for all cores with same EPP. 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.EnableHwpAutoEppGrouping | * | 0x01 | 0x01

  # !BSF NAME:{Dynamic Efficiency Control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable SoC to control energy efficiency targets autonomously, regardless of EPP, EPB and other SW inputs. 0: Disable; </b>1: Enable <b>}
  gPlatformFspPkgTokenSpaceGuid.EnableDynamicEfficiencyControl | * | 0x01 | 0x01

  # !BSF NAME:{Misc Power Management MSR Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable HWP Lock support in Misc Power Management MSR. 0: Disable, <b>1: Enable </b>}
  gPlatformFspPkgTokenSpaceGuid.HwpLock                    | * | 0x01 | 0x01

  # !BSF NAME:{Power Floor Managment for SOC} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Option to disable Power Floor Managment for SOC. Disabling this might effectively raise power floor of the SoC and may lead to stability issues. 0: Disable, <b>1: Enable </b>}
  gPlatformFspPkgTokenSpaceGuid.PowerFloorManagement        | * | 0x01 | 0x01

  # !BSF NAME:{Power Floor Disaplay Disconnect} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{SoC can disconnect secondary/external display to lower SoC floor power (Default disabled).  0: Disable: Display disconnect will not be used by SoC., </b>1: Enable <b>}
  gPlatformFspPkgTokenSpaceGuid.PowerFloorDisplayDisconnect | * | 0x01 | 0x00

  ## CPU_POWER_MGMT_BASIC_CONFIG End


  ## CPU_TEST_CONFIG Start

  # !BSF NAME:{Memory size per thread allocated for Processor Trace}
  # !BSF TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Memory size per thread for Processor Trace. Processor Trace requires 2^N alignment and size in bytes per thread, from 4KB to 128MB.\n<b> 0xff:none </b>, 0:4k, 0x1:8k, 0x2:16k, 0x3:32k, 0x4:64k, 0x5:128k, 0x6:256k, 0x7:512k, 0x8:1M, 0x9:2M, 0xa:4M. 0xb:8M, 0xc:16M, 0xd:32M, 0xe:64M, 0xf:128M}
  gPlatformFspPkgTokenSpaceGuid.ProcessorTraceMemSize       | * | 0x1 | 0xff

  # !BSF NAME:{Enable or Disable MLC Streamer Prefetcher} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable MLC Streamer Prefetcher; 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.MlcStreamerPrefetcher       | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable MLC Spatial Prefetcher} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable MLC Spatial Prefetcher; 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.MlcSpatialPrefetcher        | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable Monitor /MWAIT instructions} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable MonitorMWait, if Disable MonitorMwait, the AP threads Idle Manner should not set in MWAIT Loop. 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.MonitorMwaitEnable          | * | 0x01 | 0x01

  # !BSF NAME:{Enable or Disable initialization of machine check registers} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable initialization of machine check registers; 0: Disable; <b>1: Enable</b>.}
  gPlatformFspPkgTokenSpaceGuid.MachineCheckEnable          | * | 0x01 | 0x01

  # !BSF NAME:{Control on Processor Trace output scheme}
  # !BSF TYPE:{Combo} OPTION:{0: Single Range Output, 1: ToPA Output}
  # !BSF HELP:{Control on Processor Trace output scheme; <b>0: Single Range Output</b>; 1: ToPA Output.}
  gPlatformFspPkgTokenSpaceGuid.ProcessorTraceOutputScheme  | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable Processor Trace feature} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable or Disable Processor Trace feature; <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.ProcessorTraceEnable        | * | 0x01 | 0x00

  # !BSF NAME:{Processor trace enabled for Bsp only or all cores}
  # !BSF TYPE:{Combo} OPTION:{0: all cores, 1: Bsp only}
  # !BSF HELP:{Processor trace enabled for Bsp only or all cores; <b>0: all cores</b>; 1: Bsp only.}
  gPlatformFspPkgTokenSpaceGuid.ProcessorTraceBspOnly       | * | 0x01 | 0x00

  # !BSF NAME:{Enable/Disable processor trace Timing Packet} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable collocting processor trace performance (CYC, TSC); <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.ProcessorTraceTimingPacket  | * | 0x01 | 0x00

  # !BSF NAME:{Enable or Disable Three Strike Counter} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable (default): Three Strike counter will be incremented. Disable: Prevents Three Strike counter from incrementing; 0: Disable; <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.ThreeStrikeCounter          | * | 0x01 | 0x01

  ## CPU_TEST_CONFIG End

  #
  # CPU Post-Mem Block End
  #

  ## SCS_UFS_CONFIG Start

  # !BSF NAME:{UFS enable/disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable UFS controller, One byte for each Controller - (1,0) to enable controller 0 and (0,1) to enable controller 1}
  gPlatformFspPkgTokenSpaceGuid.UfsEnable                   | * | 0x02 | {0, 0}

  # !BSF NAME:{UFS Inline Encryption enable/disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable UFS Inline Encryption feature, One byte for each Controller - (1,0) to enable Inline Encryption for controller 0 and (0, 1) to enable Inline Encryption for controller 1}
  gPlatformFspPkgTokenSpaceGuid.UfsInlineEncryption         | * | 0x02 | {0, 0}

  # !BSF NAME:{UFS Connection Status} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{UFS Connection Status, One byte for each Controller - (1,0) to UFS connected to controller 0 and (0,1) to UFS connected to controller 1}
  gPlatformFspPkgTokenSpaceGuid.UfsDeviceConnected         | * | 0x02 | {0x1, 0x1}

  ## SCS_UFS_CONFIG End

  ## PEI_ITBT_CONFIG Start

  # !BSF NAME:{Enable/Disable PCIe tunneling for USB4} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable PCIe tunneling for USB4, default is enable}
  gPlatformFspPkgTokenSpaceGuid.ITbtPcieTunnelingForUsb4    | * | 0x01 | 0x01
  # Added reserved space  UnusedUpdSpace3
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd3                 | * | 0x2 | {0x00}

  # !BSF NAME:{ITBTForcePowerOn Timeout value} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{ITBTForcePowerOn value. Specified increment values in miliseconds. Range is 0-1000. 100 = 100 ms.}
  gPlatformFspPkgTokenSpaceGuid.ITbtForcePowerOnTimeoutInMs | * | 0x02 | 0x1F4

  # !BSF NAME:{ITbtConnectTopology Timeout value} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{ITbtConnectTopologyTimeout value. Specified increment values in miliseconds. Range is 0-10000. 100 = 100 ms.}
  gPlatformFspPkgTokenSpaceGuid.ITbtConnectTopologyTimeoutInMs  | * | 0x02 | 0x1388

  # !BSF NAME:{ITBT DMA LTR}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{TCSS DMA1, DMA2 LTR value}
  gPlatformFspPkgTokenSpaceGuid.ITbtDmaLtr                     | * | 0x04 | {0x97FF, 0x97FF}

  # !BSF NAME:{ITbt Usb4CmMode value} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{ITbt Usb4CmMode value. 0:Firmware CM, 1:Software CM}
  gPlatformFspPkgTokenSpaceGuid.Usb4CmMode                  | * | 0x01 | 0x01
  # Added reserved space  UnusedUpdSpace4[5]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd4                 | * | 0xB | {0x00}

  ## PEI_ITBT_CONFIG End

  #
  # FspV Pre-Mem Test Config Start
  #

  # !BSF NAME:{FSPS Validation} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Point to FSPS Validation configuration structure}
  gPlatformFspPkgTokenSpaceGuid.FspsValidationPtr   | * | 0x08 | 0x00

  #
  # FspV Pre-Mem Test Config End
  #

  ## IEH_CONFIG Start

  # !BSF NAME:{IEH Mode} TYPE:{Combo} OPTION:{0: Bypass, 1:Enable}
  # !BSF HELP:{Integrated Error Handler Mode, 0: Bypass, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.IehMode | * | 0x01 | 0x00

!if (gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1) OR (gSiPkgTokenSpaceGuid.PcdFusaSupport == 0x1)
  # !BSF NAME:{Fusa Configuration} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Fusa (Functional Safety) Enable Fusa Feature, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaConfigEnable            | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Module 0 Lockstep Configuration}
  # !BSF TYPE:{Combo} OPTION:{0: Disable lockstep, 1: Enable lockstep for Core 0 with Core 1 and Core 2 with Core 3, 2: Enable lockstep for Core 0 with Core 1, 3: Enable lockstep for Core 2 with Core 3}
  # !BSF HELP:{Enable/Disable Lockstep for Atom module 0, which has 4 cores; 0: Disable lockstep; 1: Enable lockstep for Core 0 with Core 1, Core 2 with Core 3; 2: Enable lockstep for Core 0 with Core 1; 3: Enable lockstep for Core 2 with Core 3}
  gPlatformFspPkgTokenSpaceGuid.Module0Lockstep             | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Module 1 Lockstep Configuration}
  # !BSF TYPE:{Combo} OPTION:{0: Disable lockstep, 1: Enable lockstep for Core 0 with Core 1 and Core 2 with Core 3, 2: Enable lockstep for Core 0 with Core 1, 3: Enable lockstep for Core 2 with Core 3}
  # !BSF HELP:{Enable/Disable Lockstep for Atom module 1, which has 4 cores; 0: Disable lockstep; 1: Enable lockstep for Core 0 with Core 1, Core 2 with Core 3; 2: Enable lockstep for Core 0 with Core 1; 3: Enable lockstep for Core 2 with Core 3}
  gPlatformFspPkgTokenSpaceGuid.Module1Lockstep             | * | 0x01 | 0x00
  # !BSF NAME:{Fusa LPC Lockstep Configuration}
  # !BSF TYPE:{Combo} OPTION:{0: Disable lockstep, 1: Enable lockstep for Core 0 with Core 1 and Core 2 with Core 3, 2: Enable lockstep for Core 0 with Core 1, 3: Enable lockstep for Core 2 with Core 3}
  # !BSF HELP:{Enable/Disable Lockstep for Atom module 1, which has 4 cores; 0: Disable lockstep; 1: Enable lockstep for Core 0 with Core 1, Core 2 with Core 3; 2: Enable lockstep for Core 0 with Core 1; 3: Enable lockstep for Core 2 with Core 3}
  gPlatformFspPkgTokenSpaceGuid.LpcLockstep                 | * | 0x01 | 0x00
  # !BSF NAME:{Opio Recentering Ctrl} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Opio Recentering Disabling for Pcie Latency Improvement, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.OpioRecenter                | * | 0x01 | 0x01
    # !BSF NAME:{Fusa Run Start Up Array BIST for Module 0}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute startup array test for Module 0 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunStartupArrayBistMod0     | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Start Up Array BIST for Module 1}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute startup array test for Module 1 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunStartupArrayBistMod1     | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Start Up Scan BIST for Module 0}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute startup scan test for module 0 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunStartupScanBistMod0      | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Start Up Scan BIST for Module 1}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute startup scan test for module 1 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunStartupScanBistMod1      | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Periodic Array BIST for Module 0}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute periodic array test for module 0 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunPeriodicArrayBistMod0     | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Periodic Array BIST for Module 1}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute periodic array test for module 1 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunPeriodicArrayBistMod1     | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Periodic Scan BIST for Module 0}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute periodic scan test for module 0 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunPeriodicScanBistMod0     | * | 0x01 | 0x00
  # !BSF NAME:{Fusa Run Periodic Scan BIST for Module 1}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enabling this will execute periodic scan test for module 1 during boot, 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.FusaRunPeriodicScanBistMod1     | * | 0x01 | 0x00

  gPlatformFspPkgTokenSpaceGuid.FusaReserved2FspsUpd         | * | 0x0F | {0x00}
  # !BSF NAME:{Fusa Startup Pattern File Base Address} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Fusa (Functional Safety) Startup Pattern File Base Address}
  gPlatformFspPkgTokenSpaceGuid.FusaStartupPatternAddr | * | 0x04 | 0x00000000
  # !BSF NAME:{Fusa Periodic Pattern File Base Address} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Fusa (Functional Safety) Periodic Pattern File Base Address}
  gPlatformFspPkgTokenSpaceGuid.FusaPeriodicPatternAddr | * | 0x04 | 0x00000000
!endif

  ## IEH_CONFIG End

  ## RTC_CONFIG Start

  # !BSF NAME:{RTC BIOS Interface Lock}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable RTC BIOS interface lock. When set, prevents RTC TS (BUC.TS) from being changed.}
  gPlatformFspPkgTokenSpaceGuid.RtcBiosInterfaceLock        | * | 0x01 | 0x01

  # !BSF NAME:{RTC Cmos Memory Lock}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable RTC lower and upper 128 byte Lock bits to lock Bytes 38h-3Fh in the upper and and lower 128-byte bank of RTC RAM.}
  gPlatformFspPkgTokenSpaceGuid.RtcMemoryLock               | * | 0x01 | 0x01

  ## RTC_CONFIG End

  ## AMT_PEI_CONFIG Start

  # !BSF NAME:{AMT Switch} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, 1: enable, Enable or disable AMT functionality.}
  gPlatformFspPkgTokenSpaceGuid.AmtEnabled                  | * | 0x01 | 0x01

  # !BSF NAME:{SOL Switch} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, 1: enable, Serial Over Lan enable/disable state by Mebx. Setting is invalid if AmtEnabled is 0.}
  gPlatformFspPkgTokenSpaceGuid.AmtSolEnabled               | * | 0x01 | 0x0

  # !BSF NAME:{WatchDog Timer Switch} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, 1: enable, Enable or disable WatchDog timer. Setting is invalid if AmtEnabled is 0.}
  gPlatformFspPkgTokenSpaceGuid.WatchDogEnabled             | * | 0x01 | 0x0

  # !BSF NAME:{OS Timer} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{16 bits Value, Set OS watchdog timer. Setting is invalid if AmtEnabled is 0.}
  gPlatformFspPkgTokenSpaceGuid.WatchDogTimerOs             | * | 0x02 | 0x0

  # !BSF NAME:{BIOS Timer} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{16 bits Value, Set BIOS watchdog timer. Setting is invalid if AmtEnabled is 0.}
  gPlatformFspPkgTokenSpaceGuid.WatchDogTimerBios           | * | 0x02 | 0x0

  ## AMT_PEI_CONFIG End

  ## IAX_PEI_CONFIG Start

  # !BSF NAME:{Iax Switch} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable. 0: Disable, 1: enable, Enable or disable Iax functionality.}
  gPlatformFspPkgTokenSpaceGuid.IaxEnable                  | * | 0x01 | 0x00
  # Added reserved space  UnusedUpdSpace5[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd5                 | * | 0x1 | 0x00

  ## IAX_PEI_CONFIG End

  ## ISH_CONFIG Start

  # !BSF NAME:{ISH GP GPIO Pin Muxing} TYPE:{EditNum, HEX, (0, 0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Determines ISH GP GPIO Pin muxing. See GPIO_*_MUXING_ISH_GP_x_GPIO_*. 'x' are GP_NUMBER}
  gPlatformFspPkgTokenSpaceGuid.IshGpGpioPinMuxing          | * | 0x30 | { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{ISH UART Rx Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Determines ISH UART Rx Pin muxing. See GPIO_*_MUXING_ISH_UARTx_TXD_*}
  gPlatformFspPkgTokenSpaceGuid.IshUartRxPinMuxing          | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Tx Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Determines ISH UART Tx Pin muxing. See GPIO_*_MUXING_ISH_UARTx_RXD_*}
  gPlatformFspPkgTokenSpaceGuid.IshUartTxPinMuxing          | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Rts Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH UART Rts Pin muxing. Refer to GPIO_*_MUXING_ISH_UARTx_RTS_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshUartRtsPinMuxing         | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Rts Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH UART Cts Pin muxing. Refer to GPIO_*_MUXING_ISH_UARTx_CTS_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshUartCtsPinMuxing         | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH I2C SDA Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH I2C SDA Pin muxing. Refer to GPIO_*_MUXING_ISH_I2Cx_SDA_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshI2cSdaPinMuxing          | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH I2C SCL Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH I2C SCL Pin muxing. Refer to GPIO_*_MUXING_ISH_I2Cx_SCL_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshI2cSclPinMuxing          | * | 0xC | { 0, 0, 0 }

  # !BSF NAME:{ISH SPI MOSI Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH SPI MOSI Pin muxing. Refer to GPIO_*_MUXING_ISH_SPIx_MOSI_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiMosiPinMuxing         | * | 0x8 | { 0, 0 }

  # !BSF NAME:{ISH SPI MISO Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH SPI MISO Pin muxing. Refer to GPIO_*_MUXING_ISH_SPIx_MISO_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiMisoPinMuxing         | * | 0x8 | { 0, 0 }

  # !BSF NAME:{ISH SPI CLK Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH SPI CLK Pin muxing. Refer to GPIO_*_MUXING_ISH_SPIx_CLK_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiClkPinMuxing          | * | 0x8 | { 0, 0 }

  # !BSF NAME:{ISH SPI CS#N Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select ISH SPI CS#N Pin muxing. Refer to GPIO_*_MUXING_ISH_SPIx_CS<N>_* for possible values. N-SPI number, 0-1.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiCsPinMuxing           | * | 0x10 | { 0, 0 , 0, 0}

  # !BSF NAME:{ISH GP GPIO Pad termination} TYPE:{EditNum, HEX, (0, 0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo GP#N GPIO pads termination respectively. #N are GP_NUMBER, not strictly relate to indexes of this table. Index 0-23 -> ISH_GP_0-23, Index 24-25 -> ISH_GP_30-31}
  gPlatformFspPkgTokenSpaceGuid.IshGpGpioPadTermination          | * | 0x0C | { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{ISH UART Rx Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo UART#N Rx pads termination respectively. #N-byte for each controller, byte0 for UART0 Rx, byte1 for UART1 Rx, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshUartRxPadTermination          | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Tx Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo UART#N Tx pads termination respectively. #N-byte for each controller, byte0 for UART0 Tx, byte1 for UART1 Tx, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshUartTxPadTermination          | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Rts Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo UART#N Rts pads termination respectively. #N-byte for each controller, byte0 for UART0 Rts, byte1 for UART1 Rts, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshUartRtsPadTermination         | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH UART Rts Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo UART#N Cts pads termination respectively. #N-byte for each controller, byte0 for UART0 Cts, byte1 for UART1 Cts, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshUartCtsPadTermination         | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH I2C SDA Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C#N Sda pads termination respectively. #N-byte for each controller, byte0 for I2C0 Sda, byte1 for I2C1 Sda, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshI2cSdaPadTermination          | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH I2C SCL Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C#N Scl pads termination respectively. #N-byte for each controller, byte0 for I2C0 Scl, byte1 for I2C1 Scl, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshI2cSclPadTermination          | * | 0x3 | { 0, 0, 0 }

  # !BSF NAME:{ISH SPI MOSI Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo SPI#N Mosi pads termination respectively. #N-byte for each controller, byte0 for SPI0 Mosi, byte1 for SPI1 Mosi, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiMosiPadTermination         | * | 0x2 | { 0, 0 }

  # !BSF NAME:{ISH SPI MISO Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo SPI#N Miso pads termination respectively. #N-byte for each controller, byte0 for SPI0 Miso, byte1 for SPI1 Miso, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiMisoPadTermination         | * | 0x2 | { 0, 0 }

  # !BSF NAME:{ISH SPI CLK Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo SPI#N Clk pads termination respectively. #N-byte for each controller, byte0 for SPI0 Clk, byte1 for SPI1 Clk, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshSpiClkPadTermination          | * | 0x2 | { 0, 0 }

  # !BSF NAME:{ISH SPI CS#N Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo SPI#N Cs#M pads termination respectively. N*M-byte for each controller, byte0 for SPI0 Cs0, byte1 for SPI1 Cs1, SPI1 Cs0, byte2, SPI1 Cs1, byte3}
  gPlatformFspPkgTokenSpaceGuid.IshSpiCsPadTermination           | * | 0x4 | { 0, 0, 0, 0 }

  # !BSF NAME:{Enable PCH ISH SPI Cs#N pins assigned} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set if ISH SPI Cs#N pins are to be enabled by BIOS. 0: Disable; 1: Enable. N-Cs number: 0-1}
  gPlatformFspPkgTokenSpaceGuid.PchIshSpiCsEnable           | * | 0x04 | { 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH ISH SPI Cs0 pins assigned} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Set if ISH SPI Cs0 pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshSpiCs0Enable          | * | 0x01 | { 0x00 }

  # !BSF NAME:{Enable PCH ISH SPI pins assigned} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Set if ISH SPI native pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshSpiEnable             | * | 0x01 | { 0x00 }

  # !BSF NAME:{Enable PCH ISH UART pins assigned} TYPE:{EditNum, HEX, (0x0,0xFFFF)}
  # !BSF HELP:{Set if ISH UART native pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshUartEnable            | * | 0x02 | { 0x00, 0x00 }

  # !BSF NAME:{Enable PCH ISH I2C pins assigned} TYPE:{EditNum, HEX, (0x0,0xFFFFFF)}
  # !BSF HELP:{Set if ISH I2C native pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshI2cEnable             | * | 0x03 | { 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCH ISH GP pins assigned} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set if ISH GP native pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshGpEnable              | * | 0x0C | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCH ISH PDT Unlock Msg} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: False; 1: True.}
  gPlatformFspPkgTokenSpaceGuid.PchIshPdtUnlock             | * | 0x01 | 0x00

  # !BSF NAME:{PCH ISH MSI Interrupts} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: False; 1: True.}
  gPlatformFspPkgTokenSpaceGuid.PchIshMsiInterrupt          | * | 0x01 | 0x01

  ## ISH_CONFIG End

  #
  # ME Post-Mem Block Start
  #
  # !BSF PAGE:{ME2}

  ## ME_PEI_CONFIG Start

  # !BSF NAME:{End of Post message} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Send in PEI, 2:Send in DXE, 3:Reserved}
  # !BSF HELP:{Test, Send End of Post message. Disable(0x0): Disable EOP message, Send in PEI(0x1): EOP send in PEI, Send in DXE(0x2)(Default): EOP send in DXE}
  gPlatformFspPkgTokenSpaceGuid.EndOfPostMessage            | * | 0x01 | 0x2

  # !BSF NAME:{D0I3 Setting for HECI Disable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: disable, 1: enable, Setting this option disables setting D0I3 bit for all HECI devices}
  gPlatformFspPkgTokenSpaceGuid.DisableD0I3SettingForHeci   | * | 0x01 | 0x0

  # !BSF NAME:{Mctp Broadcast Cycle} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, Determine if MCTP Broadcast is enabled <b>0: Disable</b>; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.MctpBroadcastCycle          | * | 0x01 | 0x0

  # !BSF NAME:{ME Unconfig on RTC clear}
  # !BSF TYPE:{Combo} OPTION:{0: Disable ME Unconfig On Rtc Clear, 1: Enable ME Unconfig On Rtc Clear, 2: Cmos is clear, 3: Reserved}
  # !BSF HELP:{0: Disable ME Unconfig On Rtc Clear. <b>1: Enable ME Unconfig On Rtc Clear</b>. 2: Cmos is clear, status unkonwn. 3: Reserved}
  gPlatformFspPkgTokenSpaceGuid.MeUnconfigOnRtcClear        | * | 0x01 | 0x01

  # !BSF NAME:{CSE Data Resilience Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable CSE Data Resilience Support. <b>1: Enable CSE Data Resilience Support.</b> 2: Enable CSE Data Resilience but defer to DXE.}
  gPlatformFspPkgTokenSpaceGuid.CseDataResilience           | * | 0x01 | 0x01

  # !BSF NAME:{PSE EOM Flow Control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable PSE EOM Flow. <b>1: Enable PSE EOM Flow.</b>}
  gPlatformFspPkgTokenSpaceGuid.PseEomFlowEnable            | * | 0x01 | 0x00

  ## ME_PEI_CONFIG End
  ## ME Post-Mem Block End

  ## ISH_I3C_CONFIG Start

  # !BSF NAME:{ISH I3C SDA Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select ISH I3C SDA Pin muxing. Refer to GPIO_*_MUXING_ISH_I3Cx_SDA_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshI3cSdaPinMuxing          | * | 0x8 | { 0, 0 }

  # !BSF NAME:{ISH I3C SCL Pin Muxing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Select ISH I3C SCL Pin muxing. Refer to GPIO_*_MUXING_ISH_I3Cx_SCL_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.IshI3cSclPinMuxing          | * | 0x8 | { 0, 0 }

  # !BSF NAME:{ISH I3C SDA Pad termination} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C#N Sda pads termination respectively. #N-byte for each controller, byte0 for I2C0 Sda, byte1 for I2C1 Sda, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshI3cSdaPadTermination          | * | 0x2 | { 0, 0 }

  # !BSF NAME:{ISH I3C SCL Pad termination} TYPE:{EditNum, HEX, (0x00,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C#N Scl pads termination respectively. #N-byte for each controller, byte0 for I2C0 Scl, byte1 for I2C1 Scl, and so on.}
  gPlatformFspPkgTokenSpaceGuid.IshI3cSclPadTermination          | * | 0x2 | { 0, 0 }

  # !BSF NAME:{Enable PCH ISH I3C pins assigned} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Set if ISH I3C native pins are to be enabled by BIOS. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIshI3cEnable             | * | 0x02 | { 0x00, 0x00}
  # Added reserved space   UnusedUpdSpace6[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd6                | * | 0x3 | {0x00}

  ## ISH_I3C_CONFIG End

  ## PCH_PM_CONFIG Start

  # !BSF NAME:{Power button debounce configuration} TYPE:{EditNum, HEX, (0x00,0x009C4000)}
  # !BSF HELP:{Debounce time for PWRBTN in microseconds. For values not supported by HW, they will be rounded down to closest supported on. 0: disable, 250-1024000us: supported range}
  gPlatformFspPkgTokenSpaceGuid.PmcPowerButtonDebounce              | * | 0x04 | 0x00

  # !BSF NAME:{PCH USB2 PHY Power Gating enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{1: Will enable USB2 PHY SUS Well Power Gating, 0: Will not enable PG of USB2 PHY Sus Well PG}
  gPlatformFspPkgTokenSpaceGuid.PmcUsb2PhySusPgEnable       | * | 0x01 | 0x01

  # !BSF NAME:{VRAlert# Pin} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When VRAlert# feature pin is enabled and its state is '0', the PMC requests throttling to a T3 Tstate to the PCH throttling unit.. 0: disable, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PchPmVrAlert                | * | 0x01 | 0x00

  # !BSF NAME:{ModPHY SUS Power Domain Dynamic Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable ModPHY SUS Power Domain Dynamic Gating. Setting not supported on PCH-H. 0: disable, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PmcModPhySusPgEnable        | * | 0x01 | 0x00

  # !BSF NAME:{V1p05-PHY supply external FET control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable control using EXT_PWR_GATE# pin of external FET to power gate v1p05-PHY supply. 0: disable, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PmcV1p05PhyExtFetControlEn  | * | 0x01 | 0x01

  # !BSF NAME:{V1p05-IS supply external FET control} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable control using EXT_PWR_GATE2# pin of external FET to power gate v1p05-IS supply. 0: disable, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PmcV1p05IsExtFetControlEn   | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm PME_B0_S5_DIS} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When cleared (default), wake events from PME_B0_STS are allowed in S5 if PME_B0_EN = 1.}
  gPlatformFspPkgTokenSpaceGuid.PchPmPmeB0S5Dis             | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm Wol Enable Override} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Corresponds to the WOL Enable Override bit in the General PM Configuration B (GEN_PMCON_B) register.}
  gPlatformFspPkgTokenSpaceGuid.PchPmWolEnableOverride      | * | 0x01 | 0x01

  # !BSF NAME:{PCH Pm WoW lan Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Determine if WLAN wake from Sx, corresponds to the HOST_WLAN_PP_EN bit in the PWRM_CFG3 register.}
  gPlatformFspPkgTokenSpaceGuid.PchPmWoWlanEnable           | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm Slp S3 Min Assert} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{SLP_S3 Minimum Assertion Width Policy. Default is PchSlpS350ms.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpS3MinAssert         | * | 0x01 | 0x03

  # !BSF NAME:{PCH Pm Slp S4 Min Assert} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{SLP_S4 Minimum Assertion Width Policy. Default is PchSlpS44s.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpS4MinAssert         | * | 0x01 | 0x01

  # !BSF NAME:{PCH Pm Slp Sus Min Assert} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{SLP_SUS Minimum Assertion Width Policy. Default is PchSlpSus4s.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpSusMinAssert        | * | 0x01 | 0x04

  # !BSF NAME:{PCH Pm Slp A Min Assert} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{SLP_A Minimum Assertion Width Policy. Default is PchSlpA2s.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpAMinAssert          | * | 0x01 | 0x04

  # !BSF NAME:{PCH Pm Slp Strch Sus Up} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable SLP_X Stretching After SUS Well Power Up.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpStrchSusUp          | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm Slp Lan Low Dc} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable SLP_LAN# Low on DC Power.}
  gPlatformFspPkgTokenSpaceGuid.PchPmSlpLanLowDc            | * | 0x01 | 0x01

  # !BSF NAME:{PCH Pm Pwr Btn Override Period} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{PCH power button override period. 000b-4s, 001b-6s, 010b-8s, 011b-10s, 100b-12s, 101b-14s.}
  gPlatformFspPkgTokenSpaceGuid.PchPmPwrBtnOverridePeriod   | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm Disable Native Power Button} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Power button native mode disable.}
  gPlatformFspPkgTokenSpaceGuid.PchPmDisableNativePowerButton  | * | 0x01 | 0x00

  # !BSF NAME:{PCH Pm ME_WAKE_STS} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Clear the ME_WAKE_STS bit in the Power and Reset Status (PRSTS) register.}
  gPlatformFspPkgTokenSpaceGuid.PchPmMeWakeSts              | * | 0x01 | 0x01

  # !BSF NAME:{PCH Pm WOL_OVR_WK_STS} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Clear the WOL_OVR_WK_STS bit in the Power and Reset Status (PRSTS) register.}
  gPlatformFspPkgTokenSpaceGuid.PchPmWolOvrWkSts            | * | 0x01 | 0x01

  # !BSF NAME:{PCH Pm Reset Power Cycle Duration} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Could be customized in the unit of second. Please refer to EDS for all support settings. 0 is default, 1 is 1 second, 2 is 2 seconds, ...}
  gPlatformFspPkgTokenSpaceGuid.PchPmPwrCycDur              | * | 0x01 | 0xC7

  # !BSF NAME:{PCH Pm Pcie Pll Ssc} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Specifies the Pcie Pll Spread Spectrum Percentage. The default is 0xFF: AUTO - No BIOS override.}
  gPlatformFspPkgTokenSpaceGuid.PchPmPciePllSsc             | * | 0x01 | 0xFF

  # !BSF NAME:{Enable TCO timer.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When FALSE, it disables PCH ACPI timer, and stops TCO timer. NOTE: This will have huge power impact when it's enabled. If TCO timer is disabled, uCode ACPI timer emulation must be enabled, and WDAT table must not be exposed to the OS.}
  gPlatformFspPkgTokenSpaceGuid.EnableTcoTimer              | * | 0x01 | 0x00

!if (gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1)
  # !BSF NAME:{Enable Timed GPIO0} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Timed GPIO0. When disabled, it disables cross time stamp time-synchronization as extension of Hammock Harbor time synchronization.}
  gPlatformFspPkgTokenSpaceGuid.EnableTimedGpio0             | * | 0x01 | 0x00

  # !BSF NAME:{Enable Timed GPIO1} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable Timed GPIO1. When disabled, it disables cross time stamp time-synchronization as extension of Hammock Harbor time synchronization.}
  gPlatformFspPkgTokenSpaceGuid.EnableTimedGpio1             | * | 0x01 | 0x00
!endif

  # !BSF NAME:{Enable PS_ON.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{PS_ON is a new C10 state from the CPU on desktop SKUs that enables a lower power target that will be required by the California Energy Commission (CEC). When FALSE, PS_ON is to be disabled.}
  gPlatformFspPkgTokenSpaceGuid.PsOnEnable                  | * | 0x01 | 0x00

  # !BSF NAME:{Pmc Cpu C10 Gate Pin Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable platform support for CPU_C10_GATE# pin to control gating of CPU VccIO and VccSTG rails instead of SLP_S0# pin.}
  gPlatformFspPkgTokenSpaceGuid.PmcCpuC10GatePinEnable      | * | 0x01 | 0x01

  # !BSF NAME:{OS IDLE Mode Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable OS Idle Mode}
  gPlatformFspPkgTokenSpaceGuid.PmcOsIdleEnable             | * | 0x01 | 0x01

  # !BSF NAME:{S0ix Auto-Demotion} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable the Low Power Mode Auto-Demotion Host Control feature.}
  gPlatformFspPkgTokenSpaceGuid.PchS0ixAutoDemotion         | * | 0x01 | 0x01

  # !BSF NAME:{Latch Events C10 Exit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When this bit is set to 1, SLP_S0# entry events in SLP_S0_DEBUG_REGx registers are captured on C10 exit (instead of C10 entry which is default)}
  gPlatformFspPkgTokenSpaceGuid.PchPmLatchEventsC10Exit     | * | 0x01 | 0x00

  # !BSF NAME:{PCH Energy Reporting} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable PCH to CPU energy report feature.}
  gPlatformFspPkgTokenSpaceGuid.PchPmDisableEnergyReport    | * | 0x01 | 0x00

  # !BSF NAME:{Low Power Mode Enable/Disable config mask} TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Configure if respective S0i2/3 sub-states are to be supported. Each bit corresponds to one sub-state (LPMx - BITx): LPM0-s0i2.0, LPM1-s0i2.1, LPM2-s0i2.2, LPM3-s0i3.0, LPM4-s0i3.1, LPM5-s0i3.2, LPM6-s0i3.3, LPM7-s0i3.4.}
  gPlatformFspPkgTokenSpaceGuid.PmcLpmS0ixSubStateEnableMask | * | 0x01 | 0xFF

  # !BSF NAME:{PCH PMC ER Debug mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable/Enable Energy Reporting Debug Mode.}
  gPlatformFspPkgTokenSpaceGuid.PchPmErDebugMode             | * | 0x01 | 0x01

  # !BSF NAME:{PMC C10 dynamic threshold dajustment enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set if you want to enable PMC C10 dynamic threshold adjustment. Only works on supported SKUs}
  gPlatformFspPkgTokenSpaceGuid.PmcC10DynamicThresholdAdjustment | * | 0x1 | 0x0

  ## PCH_PM_CONFIG End

  ## PCH_LOCK_DOWN_CONFIG Start

  # !BSF NAME:{Enable LOCKDOWN BIOS LOCK} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable the BIOS Lock feature and set EISS bit (D31:F5:RegDCh[5]) for the BIOS region protection.}
  gPlatformFspPkgTokenSpaceGuid.PchLockDownBiosLock         | * | 0x01 | 0x01

  # !BSF NAME:{Enable LOCKDOWN SMI} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable SMI_LOCK bit to prevent writes to the Global SMI Enable bit.}
  gPlatformFspPkgTokenSpaceGuid.PchLockDownGlobalSmi        | * | 0x01 | 0x01

  # !BSF NAME:{Enable LOCKDOWN BIOS Interface} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable BIOS Interface Lock Down bit to prevent writes to the Backup Control Register.}
  gPlatformFspPkgTokenSpaceGuid.PchLockDownBiosInterface    | * | 0x01 | 0x01

  # !BSF NAME:{Unlock all GPIO pads} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Force all GPIO pads to be unlocked for debug purpose.}
  gPlatformFspPkgTokenSpaceGuid.PchUnlockGpioPads           | * | 0x01 | 0x00

  ## PCH_LOCK_DOWN_CONFIG End

  ## PCH_FLASH_PROTECTION_CONFIG Start

  # !BSF NAME:{PCH Flash Protection Ranges Write Enble} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{Write or erase is blocked by hardware.}
  gPlatformFspPkgTokenSpaceGuid.PchWriteProtectionEnable    | * | 0x05 | { 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH Flash Protection Ranges Read Enble} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFF)}
  # !BSF HELP:{Read is blocked by hardware.}
  gPlatformFspPkgTokenSpaceGuid.PchReadProtectionEnable     | * | 0x05 | { 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH Protect Range Limit} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Left shifted address by 12 bits with address bits 11:0 are assumed to be FFFh for limit comparison.}
  gPlatformFspPkgTokenSpaceGuid.PchProtectedRangeLimit      | * | 0x0A | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  # !BSF NAME:{PCH Protect Range Base} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Left shifted address by 12 bits with address bits 11:0 are assumed to be 0.}
  gPlatformFspPkgTokenSpaceGuid.PchProtectedRangeBase       | * | 0x0A | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  ## PCH_FLASH_PROTECTION_CONFIG End

  ## PCH_PCIE_CONFIG Start

  # !BSF NAME:{PCIe PTM enable/disable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable Precision Time Measurement for PCIE Root Ports.}
  gPlatformFspPkgTokenSpaceGuid.PciePtm                    | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCH PCIe root port connection type} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: built-in device, 1:slot}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSlotImplemented       | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Access Control Services Extended Capability} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PCIE RP Access Control Services Extended Capability}
  gPlatformFspPkgTokenSpaceGuid.PcieRpAcsEnabled            | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Clock Power Management} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable PCIE RP Clock Power Management, even if disabled, CLKREQ# signal can still be controlled by L1 PM substates mechanism}
  gPlatformFspPkgTokenSpaceGuid.PcieRpEnableCpm             | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Detect Timeout Ms} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{The number of milliseconds within 0~65535 in reference code will wait for link to exit Detect state for enabled ports before assuming there is no device and potentially disabling the port.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpDetectTimeoutMs       | * | 0x30 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCIE RP HotPlug} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the root port is hot plug available.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpHotPlug               | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCIE RP Pm Sci} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the root port power manager SCI is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpPmSci                 | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCIE RP Transmitter Half Swing} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the Transmitter Half Swing is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpTransmitterHalfSwing  | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Enable PCIE RP Clk Req Detect} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Probe CLKREQ# signal before enabling CLKREQ# based power management.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpClkReqDetect          | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Advanced Error Report} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the Advanced Error Reporting is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpAdvancedErrorReporting   | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Unsupported Request Report} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the Unsupported Request Report is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpUnsupportedRequestReport | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Fatal Error Report} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the Fatal Error Report is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpFatalErrorReport      | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP No Fatal Error Report} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the No Fatal Error Report is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpNoFatalErrorReport    | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Correctable Error Report} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the Correctable Error Report is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpCorrectableErrorReport   | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP System Error On Fatal Error} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the System Error on Fatal Error is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSystemErrorOnFatalError  | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP System Error On Non Fatal Error} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the System Error on Non Fatal Error is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSystemErrorOnNonFatalError | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP System Error On Correctable Error} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicate whether the System Error on Correctable Error is enabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSystemErrorOnCorrectableError | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Max Payload} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Max Payload Size supported, Default 256B, see enum PCH_PCIE_MAX_PAYLOAD.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpMaxPayload            | * | 0x1C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x2, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
  # !BSF NAME:{PCIE RP Pcie Speed} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Determines each PCIE Port speed capability. 0: Auto; 1: Gen1; 2: Gen2; 3: Gen3 (see: PCIE_SPEED).}
  gPlatformFspPkgTokenSpaceGuid.PcieRpPcieSpeed             | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Physical Slot Number} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Indicates the slot number for the root port. Default is the value as root port index.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpPhysicalSlotNumber    | * | 0x1C | { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B }

  # !BSF NAME:{PCIE RP Completion Timeout} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{The root port completion timeout(see: PCIE_COMPLETION_TIMEOUT). Default is PchPcieCompletionTO_Default.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpCompletionTimeout     | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Aspm} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{The ASPM configuration of the root port (see: PCH_PCIE_ASPM_CONTROL). Default is PchPcieAspmAutoConfig.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpAspm                  | * | 0x1C | { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x04 }

  # !BSF NAME:{HostL0sTxDis} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Disable Host L0 transmission state}
  gPlatformFspPkgTokenSpaceGuid.HostL0sTxDis                | * |  0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP L1 Substates} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{The L1 Substates configuration of the root port (see: PCH_PCIE_L1SUBSTATES_CONTROL). Default is PchPcieL1SubstatesL1_1_2.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpL1Substates           | * | 0x1C | { 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03 }

  # !BSF NAME:{PCIE RP Ltr Enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting Mechanism.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLtrEnable             | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Ltr Config Lock} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLtrConfigLock         | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP override default settings for EQ} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Choose PCIe EQ method}
  gPlatformFspPkgTokenSpaceGuid.PcieEqOverrideDefault       | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ method} TYPE:{Combo} OPTION:{0: HardwareEq, 1: FixedEq}
  # !BSF HELP:{Choose PCIe EQ method}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqMethod                | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ mode} TYPE:{Combo} OPTION:{0: PresetEq, 1: CoefficientEq}
  # !BSF HELP:{Choose PCIe EQ mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqMode                  | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ local transmitter override} Type:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqLocalTxOverrideEn     | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCI RP number of valid list entries} TYPE:{EditNum, HEX, (0, 11)}
  # !BSF HELP:{Select number of presets or coefficients depending on the mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3NoOfPresetOrCoeff  | * | 0x0C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor0List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor0List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor1List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor1List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor2List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor2List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIR RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor3List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor3List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor4List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor4List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor5List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor5List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor6List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIe post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor6List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor7List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor7List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor8List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor8List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PreCursor9List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3PostCursor9List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset0List        | * | 0xC | {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset1List        | * | 0xC | {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset2List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset3List        | * | 0xC | {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset4List        | * | 0xC | {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset5List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset6List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset7List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset8List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset9List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh3Preset10List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe EQ phase 1 downstream transmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the downstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh1DpTxPreset  | * | 0x0C | { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ phase 1 upstream tranmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the upstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh1UpTxPreset  | * | 0x0C | {  0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ phase 2 local transmitter override preset} Type:{EditNum, DEC, (0, 10)}
  # !BSF HELP:{Allows to select the value of the preset used during phase 2 local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3EqPh2LocalTxOverridePreset  | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ method} TYPE:{Combo} OPTION:{0: HardwareEq, 1: FixedEq}
  # !BSF HELP:{Choose PCIe EQ method}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqMethod                | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ mode} TYPE:{Combo} OPTION:{0: PresetEq, 1: CoefficientEq}
  # !BSF HELP:{Choose PCIe EQ mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqMode                  | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ local transmitter override} Type:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqLocalTxOverrideEn     | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCI RP number of valid list entries} TYPE:{EditNum, HEX, (0, 11)}
  # !BSF HELP:{Select number of presets or coefficients depending on the mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3NoOfPresetOrCoeff | * | 0x0C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor0List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor0List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor1List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor1List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor2List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor2List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIR RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor3List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor3List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor4List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor4List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor5List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor5List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor6List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIe post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor6List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor7List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor7List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor8List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor8List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PreCursor9List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3PostCursor9List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset0List        | * | 0xC | {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset1List        | * | 0xC | {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset2List        | * | 0xC | {0x06, 0x06, 0x06, 0x06, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset3List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset4List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset5List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset6List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset7List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset8List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset9List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh3Preset10List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe EQ phase 1 downstream transmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the downstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh1DpTxPreset  | * | 0x0C | { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ phase 1 upstream tranmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the upstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh1UpTxPreset    | * | 0x0C | { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ phase 2 local transmitter override preset} Type:{EditNum, DEC, (0, 10)}
  # !BSF HELP:{Allows to select the value of the preset used during phase 2 local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4EqPh2LocalTxOverridePreset | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ method} TYPE:{Combo} OPTION:{0: HardwareEq, 1: FixedEq}
  # !BSF HELP:{Choose PCIe EQ method}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqMethod                | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP choose EQ mode} TYPE:{Combo} OPTION:{0: PresetEq, 1: CoefficientEq}
  # !BSF HELP:{Choose PCIe EQ mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqMode                  | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ local transmitter override} Type:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqLocalTxOverrideEn  | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCI RP number of valid list entries} TYPE:{EditNum, HEX, (0, 11)}
  # !BSF HELP:{Select number of presets or coefficients depending on the mode}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3NoOfPresetOrCoeff  | * | 0x0C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor0List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor0List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor1List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor1List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor2List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor2List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIR RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor3List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor3List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor4List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor4List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor5List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor5List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor6List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIe post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor6List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor7List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor7List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor8List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor8List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP pre-cursor coefficient list} TYPE:{EditNum, HEX, (0x0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of pre-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PreCursor9List         | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP post-cursor coefficient list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of post-cursor coefficients to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3PostCursor9List        | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset0List        | * | 0xC | {0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset1List        | * | 0xC | {0x05, 0x05, 0x05, 0x05, 0x05, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset2List        | * | 0xC | {0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset3List        | * | 0xC | {0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset4List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset5List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset6List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset7List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset8List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset9List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIE RP preset list} TYPE:{EditNum, HEX, (0, 0x3F3F3F3F3F3F3F3F3F3F3F)}
  # !BSF HELP:{Provide a list of presets to be used during phase 3 EQ}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh3Preset10List        | * | 0xC | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{PCIe EQ phase 1 downstream transmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the downstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh1DpTxPreset          | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x000 }

  # !BSF NAME:{PCIE RP EQ phase 1 upstream tranmitter port preset} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Allows to select the upstream port preset value that will be used during phase 1 of equalization}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh1UpTxPreset          | * | 0x0C | { 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP EQ phase 2 local transmitter override preset} Type:{EditNum, DEC, (0, 10)}
  # !BSF HELP:{Allows to select the value of the preset used during phase 2 local transmitter override}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5EqPh2LocalTxOverridePreset | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase3 RP Gen3 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase3 Gen3 EQ enable. Disabled(0x0)(Default): Disable phase 3, Enabled(0x1): Enable phase 3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen3EqPh3Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase3 RP Gen4 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase3 Gen4 EQ enable. Disabled(0x0)(Default): Disable phase 3, Enabled(0x1): Enable phase 3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen4EqPh3Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase3 RP Gen5 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase3 Gen5 EQ enable. Disabled(0x0)(Default): Disable phase 3, Enabled(0x1): Enable phase 3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen5EqPh3Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase2-3 RP Gen3 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase2-3 Gen3 EQ enable. Disabled(0x0)(Default): Disable Phase2-3, Enabled(0x1): Enable Phase2-3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen3EqPh23Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase2-3 RP Gen4 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase2-3 Gen4 EQ enable. Disabled(0x0)(Default): Disable Phase2-3, Enabled(0x1): Enable Phase2-3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen4EqPh23Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Phase2-3 RP Gen5 EQ enable} TYPE:{Combo}
  # !BSF OPTION:{0:Disable, 1:Enable, 2:Auto}
  # !BSF HELP:{Phase2-3 Gen5 EQ enable. Disabled(0x0)(Default): Disable Phase2-3, Enabled(0x1): Enable Phase2-3}
  gPlatformFspPkgTokenSpaceGuid.PcieRpGen5EqPh23Bypass         | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCET Timer} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Preset/Coefficient Evaluation Timeout Gen3 PCET Timer. See PCIE_GEN3_PCET. Default is 0x0(2ms)}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3PcetTimer                 | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Gen4 PCET Timer} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Preset/Coefficient Evaluation Timeout - Gen4 PCET Timer. See PCIE_GEN4_PCET. Default is 0x0(2ms)}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4PcetTimer                 | * | 0xC | { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Gen5 PCET Timer} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Preset/Coefficient Evaluation Timeout - Gen5 PCET Timer. See PCIE_GEN5_PCET. Default is 0x0(2ms)}}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5PcetTimer                 | * | 0xC | { 0x04, 0x04, 0x04, 0x04, 0x04, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{TS Lock Timer for Gen3} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Training Sequence Wait Latency For Presets/Coefficients Evaluation - Gen3 TS Lock Timer. See PCIE_GEN3_TS_LOCK_TIMER. Default is 0x0}
  gPlatformFspPkgTokenSpaceGuid.PcieGen3TsLockTimer                     | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PTS Lock Timer for Gen4} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Training Sequence Wait Latency For Presets/Coefficients Evaluation - Gen4 TS Lock Timer. See PCIE_GEN4_TS_LCOK_TIMER. Default is 0x0}
  gPlatformFspPkgTokenSpaceGuid.PcieGen4TsLockTimer                    | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PTS Lock Timer for Gen5} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Training Sequence Wait Latency For Presets/Coefficients Evaluation - Gen5 TS Lock Timer. See PCIE_GEN5_TS_LCOK_TIMER. Default is 0x0}
  gPlatformFspPkgTokenSpaceGuid.PcieGen5TsLockTimer                    | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE Secure Register Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes whether Secure Register Lock is enaled or disabled. When it will be enbaled, load PcieRpSetSecuredRegisterLock recipe. 0: Disable(Default); 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PcieSetSecuredRegisterLock             | * | 0x01 | 0x01

  # !BSF NAME:{Enable/Disable ASPM Optionality Compliance} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/Disable ASPM Optionality Compliance.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpTestAspmOc            | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Enable Peer Memory Write} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This member describes whether Peer Memory Writes are enabled on the platform.}
  gPlatformFspPkgTokenSpaceGuid.PcieEnablePeerMemoryWrite   | * | 0xC | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Assertion on Link Down GPIOs} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{GPIO Assertion on Link Down. Disabled(0x0)(Default): Disable assertion on Link Down GPIOs, Enabled(0x1): Enable assertion on Link Down GPIOs}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLinkDownGpios   | * | 0x0C | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE Compliance Test Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Compliance Test Mode shall be enabled when using Compliance Load Board.}
  gPlatformFspPkgTokenSpaceGuid.PcieComplianceTestMode      | * | 0x01 | 0x00

  # !BSF NAME:{PCIE Rp Function Swap} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Allows BIOS to use root port function number swapping when root port of function 0 is disabled.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpFunctionSwap          | * | 0x01 | 0x01

  # !BSF NAME:{PCIe RootPort Clock Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes whether the PCI Express Clock Gating for each root port is enabled by platform modules. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PcieClockGating | * | 0x0C | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{PCIe RootPort Power Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes whether the PCI Express Power Gating for each root port is enabled by platform modules. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PciePowerGating | * | 0x0C | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{PCIe RootPort VISA Clock Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes whether the PCI Express VISA Clock Gating. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PcieVisaClockGating | * | 0x0C | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{PCIe RootPort AutoPower Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes the Auto Power Gating for per controller. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PcieAutoPowerGating | * | 0x0C | {0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{PCIe RootPort PHY AutoPower Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Describes the PHY Auto Power Gating for per controller. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PciePhyAutoPowerGating | * | 0x01 | 0x01

  # !BSF NAME:{FOMS Control Policy} TYPE:{Combo}
  # !BSF OPTION:{0: Auto, 1: Gen3 Foms, 2: Gen4 Foms, 3: Gen3 and Gen4 Foms}
  # !BSF HELP:{Choose the Foms Control Policy, <b>Default = 0 </b>}
  gPlatformFspPkgTokenSpaceGuid.PcieFomsCp | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{EqPhBypass Control Policy} TYPE:{Combo}
  # !BSF OPTION:{0: Auto, 1: Gen3 Foms, 2: Gen4 Foms, 3: Gen3 and Gen4 Foms}
  # !BSF HELP:{PCIe Equalization Phase Enable Control, <b>Disabled</b>       (0x0) : Disable Phase (Default), Enabled (0x1) : Enable Phase}
  gPlatformFspPkgTokenSpaceGuid.PcieEqPhBypass | * | 0x0C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Ltr Max Snoop Latency} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Max Snoop Latency.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLtrMaxSnoopLatency    | * | 0x30 | { 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F }

  # !BSF NAME:{PCIE RP Ltr Max No Snoop Latency} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Max Non-Snoop Latency.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLtrMaxNoSnoopLatency  | * | 0x30 | { 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F, 0x100F }

  # !BSF NAME:{PCIE RP Snoop Latency Override Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Mode.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideMode          | * | 0x1C | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE RP Snoop Latency Override Multiplier} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Multiplier.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideMultiplier    | * | 0x1C | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE RP Snoop Latency Override Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Value.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSnoopLatencyOverrideValue         | * | 0x30 | { 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Mode.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideMode       | * | 0x1C | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Multiplier} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Multiplier.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideMultiplier | * | 0x1C | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Value.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpNonSnoopLatencyOverrideValue      | * | 0x30 | { 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C, 0x003C }

  # !BSF NAME:{PCIE RP Slot Power Limit Scale} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Specifies scale used for slot power limit value. Leave as 0 to set to default.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSlotPowerLimitScale   | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Slot Power Limit Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Specifies upper limit on power supplie by slot. Leave as 0 to set to default.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpSlotPowerLimitValue   | * | 0x30 | { 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000 }

  # !BSF NAME:{PCIE RP Enable Port8xh Decode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This member describes whether PCIE root port Port 8xh Decode is enabled. 0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PcieEnablePort8xhDecode     | * | 0x01 | 0x00

  # !BSF NAME:{PCIE Port8xh Decode Port Index} TYPE:{EditNum, HEX, (0x0,0x6)}
  # !BSF HELP:{The Index of PCIe Port that is selected for Port8xh Decode (1 Based).}
  gPlatformFspPkgTokenSpaceGuid.PchPciePort8xhDecodePortIndex  | * | 0x01 | 0x01


  # !BSF NAME:{PCIE RP LTR Override Spec Compliant} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFF)}
  # !BSF HELP:{Override LTR based on Ep capability.}
  gPlatformFspPkgTokenSpaceGuid.PcieRpLtrOverrideSpecCompliant      | * | 0x1C | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Global PCIe AER} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF OPTION:{0:Disable, 1:Enable}
  # !BSF HELP:{Enable/Disable Global PCIe Advanced Error Reporting}
  gPlatformFspPkgTokenSpaceGuid.GlobalPcieAer                | * | 0x01 | 0x01

  # Added reserved space   UnusedUpdSpace7[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd7                 | * | 0x01 | {0x00}

  ## PCH_PCIE_CONFIG End

  ## SERIAL_IO_CONFIG Start

  # !BSF NAME:{Serial IO SPI CLK Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo LPSS SPI CS pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CLK* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiClkPinMux     | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{Serial IO SPI CS Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo  LPSS SPI CS pin muxing. Refer to GPIO_*_MUXING_SERIALIO_SPIx_CS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiCsPinMux     | * | 0x38 | { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{SPIn Device Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Selects SPI operation mode. N represents controller index: SPI0, SPI1, ... Available modes: 0:LpssSpiDisabled, 1:LpssSpiPci, 2:LpssSpiHidden}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiMode             | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
  # Added reserved space   UnusedUpdSpace8[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd8                 | * | 0x1 | 0x00

  # !BSF NAME:{LPSS SPI MOSI Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select LPSS SPI MOSI pin muxing. Refer to GPIO_*_MUXING_LPSS_SPIx_MOSI* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiMosiPinMux     | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{LPSS SPI MISO Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select Lpss SPI MISO pin muxing. Refer to GPIO_*_MUXING_LPSS_SPIx_MISO* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiMisoPinMux     | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{SPI<N> Chip Select Polarity} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets polarity for each chip Select. Available options: 0:LpssSpiCsActiveLow, 1:LpssSpiCsActiveHigh}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiCsPolarity       | * | 0x0E | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

  # !BSF NAME:{SPI<N> Chip Select Enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0:Disabled, 1:Enabled. Enables GPIO for CS0 or CS1 if it is Enabled}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiCsEnable         | * | 0x0E | { 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00}

  # !BSF NAME:{SPIn Default Chip Select Mode HW/SW} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets Default CS Mode Hardware or Software. N represents controller index: SPI0, SPI1, ... Available options: 0:HW, 1:SW}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiCsMode           | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{SPIn Default Chip Select State Low/High} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Sets Default CS State Low or High. N represents controller index: SPI0, SPI1, ... Available options: 0:Low, 1:High}
  gPlatformFspPkgTokenSpaceGuid.SerialIoLpssSpiCsState          | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{UARTn Device Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Selects Uart operation mode. N represents controller index: Uart0, Uart1, ... Available modes: 0:SerialIoUartDisabled, 1:SerialIoUartPci, 2:SerialIoUartHidden, 3:SerialIoUartCom, 4:SerialIoUartSkipInit}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartMode            | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  # Added reserved space    UnusedUpdSpace9[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd9                | * | 0x3 | {0x00}

  # !BSF NAME:{Default BaudRate for each Serial IO UART} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Set default BaudRate Supported from 0 - default to 6000000}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartBaudRate        | * | 0x1C | { 115200, 115200, 115200, 115200, 115200, 115200, 115200 }

  # !BSF NAME:{Default ParityType for each Serial IO UART} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set default Parity. 0: DefaultParity, 1: NoParity, 2: EvenParity, 3: OddParity}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartParity          | * | 0x07 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Default DataBits for each Serial IO UART} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set default word length. 0: Default, 5,6,7,8}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDataBits        | * | 0x07 | { 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08 }

  # !BSF NAME:{Default StopBits for each Serial IO UART} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set default stop bits. 0: DefaultStopBits, 1: OneStopBit, 2: OneFiveStopBits, 3: TwoStopBits}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartStopBits        | * | 0x07 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{Power Gating mode for each Serial IO UART that works in COM mode} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set Power Gating. 0: Disabled, 1: Enabled, 2: Auto}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartPowerGating     | * | 0x07 | { 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{Enable Dma for each Serial IO UART that supports it} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set DMA/PIO mode. 0: Disabled, 1: Enabled}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDmaEnable       | * | 0x07 | { 0x01, 0x01, 0x00, 0x01, 0x01, 0x00, 0x00 }

  # !BSF NAME:{Enables UART hardware flow control, CTS and RTS lines} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enables UART hardware flow control, CTS and RTS lines.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartAutoFlow        | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
  # Added reserved space    UnusedUpdSpace10[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd10                 | * | 0x2 | {0x00}

  # !BSF NAME:{SerialIoUartRtsPinMuxPolicy} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo Uart Rts pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RTS* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartRtsPinMuxPolicy | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{SerialIoUartRxPinMuxPolicy} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo Uart Rx pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_RX* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartRxPinMuxPolicy  | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{SerialIoUartTxPinMuxPolicy} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo Uart Tx pin muxing. Refer to GPIO_*_MUXING_SERIALIO_UARTx_TX* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartTxPinMuxPolicy  | * | 0x1C | { 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{Serial IO UART DBG2 table} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable or disable Serial Io UART DBG2 table, default is Disable; <b>0: Disable;</b> 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartDbg2            | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Serial IO UART PG DBG2 table} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable or disable Serial Io UART PG DBG2 table, default is Disable; <b>0: Disable;</b> 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoUartPgDbg2          | * | 0x07 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{I2Cn Device Mode} TYPE:{EditNum, HEX,  (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Selects I2c operation mode. N represents controller index: I2c0, I2c1, ... Available modes: 0:SerialIoI2cDisabled, 1:SerialIoI2cPci, 2:SerialIoI2cHidden}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI2cMode             | * | 0x08 | { 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }
  # Added reserved space    UnusedUpdSpace11[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd11                 | * | 0x2 | {0x00}

  # !BSF NAME:{Serial IO I2C SDA Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo I2c Sda pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SDA* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PchSerialIoI2cSdaPinMux     | * | 0x20 | { 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{Serial IO I2C SCL Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo I2c Scl pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I2Cx_SCL* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.PchSerialIoI2cSclPinMux     | * | 0x20 | { 0, 0, 0, 0, 0, 0, 0, 0 }

  # !BSF NAME:{PCH SerialIo I2C Pads Termination} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I2C0,I2C1,... pads termination respectively. One byte for each controller, byte0 for I2C0, byte1 for I2C1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PchSerialIoI2cPadsTermination | * | 0x08 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  }

  # !BSF NAME:{I3C Device Mode} TYPE:{EditNum, HEX,  (0x00,0xFFFFFF)}
  # !BSF HELP:{Selects I3c operation mode. Available modes: 0:SerialIoI3cDisabled, 1:SerialIoI3cPci, 2:SerialIoI3cPhantom (only applicable to I3C1, controlls GPIO enabling)}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cMode             | * | 0x03 | {0, 0, 0}
  # Added reserved space    UnusedUpdSpace12[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd12               | * | 0x1 | 0x00

  # !BSF NAME:{Serial IO I3C SDA Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo I3c Sda pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I3Cx_SDA* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSdaPinMux     | * | 0x0C | { 0, 0, 0 }

  # !BSF NAME:{Serial IO I3C SDA Pad Termination} TYPE:{EditNum, HEX, (0,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I3C0,I3C1,... pads termination respectively. One byte for each controller, byte0 for I3C0, byte1 for I3C1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSdaPadTermination     | * | 0x3 | { 0, 0, 0 }
  # Added reserved space    UnusedUpdSpace13[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd13                 | * | 0x1 | 0x00

  # !BSF NAME:{Serial IO I3C SCL Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo I3c Scl pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I3Cx_SCL* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSclPinMux     | * | 0x0C | { 0, 0, 0 }

  # !BSF NAME:{Serial IO I3C SCL Pad Termination} TYPE:{EditNum, HEX, (0,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I3C0,I3C1,... pads termination respectively. One byte for each controller, byte0 for I3C0, byte1 for I3C1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSclPadTermination     | * | 0x3 | { 0, 0, 0 }
  # Added reserved space    UnusedUpdSpace14[1]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd14                 | * | 0x1 | 0x00

  # !BSF NAME:{Serial IO I3C SCL FB Pin Muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Select SerialIo I3c SclFb pin muxing. Refer to GPIO_*_MUXING_SERIALIO_I3Cx_SCL FB* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSclFbPinMux     | * | 0x0C | { 0, 0, 0 }

  # !BSF NAME:{Serial IO I3C SCL FB Pad Termination} TYPE:{EditNum, HEX, (0,0xFFFFFF)}
  # !BSF HELP:{0x0: Hardware default, 0x1: None, 0x13: 1kOhm weak pull-up, 0x15: 5kOhm weak pull-up, 0x19: 20kOhm weak pull-up - Enable/disable SerialIo I3C0,I3C1,... pads termination respectively. One byte for each controller, byte0 for I3C0, byte1 for I3C1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.SerialIoI3cSclFbPadTermination     | * | 0x3 | { 0, 0, 0 }

  ## SERIAL_IO_CONFIG End

  #
  #VMD post-mem config
  #
  # !BSF NAME:{Enable VMD controller} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable to VMD controller.0: Disable; 1: Enable(Default)}
  gPlatformFspPkgTokenSpaceGuid.VmdEnable                 | * | 0x01 | 0x01

  # !BSF NAME:{Enable VMD Global Mapping} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable to VMD controller.0: Disable(Default); 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.VmdGlobalMapping          | * | 0x01 | 0x00

  # !BSF NAME:{Map port under VMD} TYPE:{Combo} OPTION:{$EN_DIS}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Map/UnMap port under VMD}
  gPlatformFspPkgTokenSpaceGuid.VmdPort         | * | 0x1F | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{VMD Port Bus} TYPE:{EditNum, DEC, (0,255)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{VMD Root port bus number.}
  gPlatformFspPkgTokenSpaceGuid.VmdPortBus      | * | 0x1F | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{VMD Port Device} TYPE:{EditNum, DEC, (0,31)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{VMD Root port device number.}
  gPlatformFspPkgTokenSpaceGuid.VmdPortDev      | * | 0x1F | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{VMD Port Func} TYPE:{EditNum, DEC, (0,7)}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{VMD Root port function number.}
  gPlatformFspPkgTokenSpaceGuid.VmdPortFunc     | * | 0x1F | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
  # Added reserved space  UnusedUpdSpace0[7]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd37     | * | 0x7 | {0x00}

  # !BSF NAME:{VMD Variable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{VMD Variable Pointer.}
  gPlatformFspPkgTokenSpaceGuid.VmdVariablePtr  | * | 0x8 | 0x00

  # !BSF NAME:{Temporary CfgBar address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary Cfg Bar address space to access Cfg space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (CfgBarAddr + 32MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdCfgBarBase                    | * | 0x4 | 0xA0000000
  # !BSF NAME:{Temporary MemBar1 address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MemBar1 address space to access MMIO space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (MemBar1Addr + 32MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdMemBar1Base                    | * | 0x4 | 0xA2000000

  # !BSF NAME:{Temporary MemBar2 address for VMD} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The reference code will use this as Temporary MemBar2 address space to access MMIO space of mapped devices. Platform should provide conflict free Temporary MMIO Range to (MemBar2Addr + 1MB MMIO).
  gPlatformFspPkgTokenSpaceGuid.VmdMemBar2Base                    | * | 0x4 | 0xA4000000

  ## TCSS_PEI_CONFIG Start

  # !BSF NAME:{Enable D3 Hot in TCSS } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This policy will enable/disable D3 hot support in IOM}
  gPlatformFspPkgTokenSpaceGuid.D3HotEnable                 | * | 0x01 | 0x00
  # Added reserved space    UnusedUpdSpace15[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd15               | * | 0x3 | {0x00}

  # !BSF NAME:{TypeC port GPIO setting} TYPE:{EditNum, HEX, (0, 0xFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{GPIO Ping number for Type C Aux orientation setting, use the GpioPad that is defined in GpioPinsXXXH.h and GpioPinsXXXLp.h as argument.(XXX is platform name, Ex: Lnl = LunarLake)}
  gPlatformFspPkgTokenSpaceGuid.IomTypeCPortPadCfg          | * | 0x30 | {0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000}

  # !BSF NAME:{CPU USB3 Port Over Current Pin} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Describe the specific over current pin number of USBC Port N.}
  gPlatformFspPkgTokenSpaceGuid.CpuUsb3OverCurrentPin       | * | 0xA | { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF }

  # !BSF NAME:{Enable D3 Cold in TCSS } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This policy will enable/disable D3 cold support in IOM}
  gPlatformFspPkgTokenSpaceGuid.D3ColdEnable                | * | 0x01 | 0x01

  # !BSF NAME:{TC State in TCSS } TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{This TC C-State Limit in IOM}
  gPlatformFspPkgTokenSpaceGuid.TcCstateLimit               | * | 0x01 | 0x0A

  # !BSF NAME:{TC Notify Igd } TYPE:{EditNum, HEX, (0x00,0xFF)}
  # !BSF HELP:{Tc Notify Igd}
  gPlatformFspPkgTokenSpaceGuid.TcNotifyIgd    | * | 0x01 | 0x00

  # !BSF NAME:{TCSS CPU USB PDO Programming} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PDO programming for TCSS CPU USB in PEI phase. Disabling will allow for programming during later phase. 1: enable, 0: disable}
  gPlatformFspPkgTokenSpaceGuid.TcssCpuUsbPdoProgramming    | * | 0x01 | 0x01

  # !BSF NAME:{Enable/Disable PMC-PD Solution } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This policy will enable/disable PMC-PD Solution vs EC-TCPC Solution}
  gPlatformFspPkgTokenSpaceGuid.PmcPdEnable                 | * | 0x01 | 0x01
  # Added reserved space    UnusedUpdSpace16
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd16               | * | 0x1 | 0x00

  # !BSF NAME:{TCSS Aux Orientation Override Enable} TYPE:{EditNum, HEX, (0x0,0x0FFF)}
  # !BSF HELP:{Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides}
  gPlatformFspPkgTokenSpaceGuid.TcssAuxOri                  | * | 0x02 | 0x0000

  # !BSF NAME:{TCSS HSL Orientation Override Enable} TYPE:{EditNum, HEX, (0x0,0x0FFF)}
  # !BSF HELP:{Bits 0, 2, ... 10 control override enables, bits 1, 3, ... 11 control overrides}
  gPlatformFspPkgTokenSpaceGuid.TcssHslOri                  | * | 0x02 | 0x0000

  # !BSF NAME:{TCSS USB Port Enable} TYPE:{EditNum, HEX, (0x0,0x000F)}
  # !BSF HELP:{Bits 0, 1, ... max Type C port control enables}
  gPlatformFspPkgTokenSpaceGuid.UsbTcPortEn                 | * | 0x01 | 0x00

  # !BSF NAME:{VCCST request for IOM } TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This policy will enable/disable VCCST and also decides if message would be replayed in S4/S5}
  gPlatformFspPkgTokenSpaceGuid.VccSt                       | * | 0x01 | 0x01

  # !BSF NAME:{Enable/Disable PTM} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This policy will enable/disable Precision Time Measurement for TCSS PCIe Root Ports}
  gPlatformFspPkgTokenSpaceGuid.PtmEnabled                     | * | 0x04 | {1, 1, 1, 1}

  # !BSF NAME:{PCIE RP Ltr Enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting Mechanism.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpLtrEnable                         | * | 0x4 | { 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Snoop Latency Override Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Mode.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpSnoopLatencyOverrideMode          | * | 0x4 | { 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Snoop Latency Override Multiplier} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Multiplier.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpSnoopLatencyOverrideMultiplier    | * | 0x4 | { 0x02, 0x02, 0x02, 0x02 }
  # !BSF NAME:{PCIE RP Snoop Latency Override Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Snoop Latency Override Value.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpSnoopLatencyOverrideValue         | * | 0x8 | { 0x00C8, 0x00C8, 0x00C8, 0x00C8 }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Mode.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpNonSnoopLatencyOverrideMode       | * | 0x4 | { 0x01, 0x01, 0x01, 0x01 }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Multiplier} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Multiplier.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpNonSnoopLatencyOverrideMultiplier | * | 0x4 | { 0x02, 0x02, 0x02, 0x02 }

  # !BSF NAME:{PCIE RP Non Snoop Latency Override Value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Latency Tolerance Reporting, Non-Snoop Latency Override Value.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpNonSnoopLatencyOverrideValue      | * | 0x8 | { 0x00C8, 0x00C8, 0x00C8, 0x00C8 }

  # !BSF NAME:{Force LTR Override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Force LTR Override.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpForceLtrOverride   | * | 0x4 | { 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCIE RP Ltr Config Lock} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.SaPcieItbtRpLtrConfigLock      | * | 0x4 | { 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{Type C Port x Convert to TypeA} TYPE:{Combo} OPTION:{$EN_DIS}
  # !HDR STRUCT:{UINT8}
  # !BSF HELP:{Enable / Disable(default) Type C Port x Convert to TypeA}
  gPlatformFspPkgTokenSpaceGuid.EnableTcssCovTypeA     | * | 0x4 | {0x00, 0x00, 0x00, 0x00}

  ## TCSS_PEI_CONFIG End

  ## THC_CONFIG Start

  # !BSF NAME:{Touch Host Controller Assignment} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Assign THC 0x0:ThcAssignmentNone, 0x1:ThcAssignmentThc0, 0x2:ThcAssignmentThc1}
  gPlatformFspPkgTokenSpaceGuid.ThcAssignment          | * | 0x02 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Interrupt Pin Mux} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Set THC Pin Muxing Value if signal can be enabled on multiple pads. Refer to GPIO_*_MUXING_THC_SPIx_INTB_* for possible values.}
  gPlatformFspPkgTokenSpaceGuid.ThcInterruptPinMuxing  | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Mode} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Switch between Intel THC protocol and Industry standard HID Over SPI protocol. 0x0:Thc, 0x1:Hid}
  gPlatformFspPkgTokenSpaceGuid.ThcMode                | * | 0x02 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Wake On Touch} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Based on this setting vGPIO for given THC will be in native mode, and additional _CRS for wake will be exposed in ACPI}
  gPlatformFspPkgTokenSpaceGuid.ThcWakeOnTouch         | * | 0x02 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Active Ltr} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Expose Active Ltr for OS driver to set}
  gPlatformFspPkgTokenSpaceGuid.ThcActiveLtr           | * | 0x08 | {0xFFFFFFFF, 0xFFFFFFFF}

  # !BSF NAME:{Touch Host Controller Idle Ltr} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Expose Idle Ltr for OS driver to set}
  gPlatformFspPkgTokenSpaceGuid.ThcIdleLtr             | * | 0x08 | {0xFFFFFFFF, 0xFFFFFFFF}

  # !BSF NAME:{Touch Host Controller Timestamp timer behavior in D0i2} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Timestamp timer behavior in D0i2. 1 = Timer resets to 0 when entering D0i2 0 = Timer is paused instead of reset to 0 when entering D0i2}
  gPlatformFspPkgTokenSpaceGuid.TimestampTimerMode         | * | 0x02 | {0x0, 0x0}
  # Added reserved space    UnusedUpdSpace17[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd17                 | * | 0x2 | {0x00}

  # !BSF NAME:{Touch Host Controller Display Frame Sync Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Period of the emulated display frame sync [ms] The minimum period is 2ms, maximum period is 100ms}
  gPlatformFspPkgTokenSpaceGuid.DisplayFrameSyncPeriod             | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller ResetPad} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{ResetPad}
  gPlatformFspPkgTokenSpaceGuid.ThcResetPad         | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller ResetPad Trigger} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Reset Pad Trigger 0x0:Low, 0x1:High}
  gPlatformFspPkgTokenSpaceGuid.ThcResetPadTrigger  | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller DYSync} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Based on this setting GPIO for given THC will be in native mode}
  gPlatformFspPkgTokenSpaceGuid.ThcDsyncPad         | * | 0x02 | {0x0, 0x0}
  # Added reserved space    UnusedUpdSpace18[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd18                 | * | 0x2 | {0x00}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Connection Speed} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Connection Speed - SPI Frequency}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiConnectionSpeed              | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Limit PacketSize} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{When set, limits SPI read & write packet size to 64B. Otherwise, THC uses Max Soc packet size for SPI Read and Write 0x0- Max Soc Packet Size,  0x11 - 64 Bytes}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiLimitPacketSize                 | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Limit PacketSize} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Minimum amount of delay the THC/QUICKSPI driver must wait between end of write operation and begin of read operation. This value shall be in 10us multiples 0x0: Disabled, 1-65535 (0xFFFF) - up to 655350 us}
  gPlatformFspPkgTokenSpaceGuid.ThcPerformanceLimitation           | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Input Report Header Address} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Input Report Header Address}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiInputReportHeaderAddress     | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Input Report Body Address} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Input Report Body Address}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiInputReportBodyAddress       | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Output Report Address} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Output Report Address}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiOutputReportAddress          | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Read Opcode} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Read Opcode}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiReadOpcode                   | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Write Opcode} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Write Opcode}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiWriteOpcode                  | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over Spi Flags} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over Spi Flags 0x0:Single SPI Mode, 0x4000:Dual SPI Mode, 0x8000:Quad SPI Mode}
  gPlatformFspPkgTokenSpaceGuid.ThcHidSpiFlags                        | * | 0x08 |  {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Reset Sequencing Delay [ms]} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !HDR STRUCT:{UINT16}
  # !BSF HELP:{Policy control for reset sequencing delay (ACPI _INI, _RST) default 300ms}
  gPlatformFspPkgTokenSpaceGuid.ThcResetSequencingDelay          | * | 0x04 |  {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Device Address} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over I2c Device Address}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cDeviceAddress                 | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Connection Speed} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over I2c Connection Speed [Hz]}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cConnectionSpeed               | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Addressing Mode} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Hid Over I2c Addressing Mode - 0x1: The connection uses 10-bit addressing. 0x0: The connection uses 7-bit addressing.}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cAddressingMode                | * | 0x2 | {0x0, 0x0}
  # Added reserved space    UnusedUpdSpace19[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd19                          | * | 0x2 | {0x00}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Device Descriptor Address} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  # !BSF HELP:{Hid Over I2c Device Descriptor Address}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cDeviceDescriptorAddress       | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Serial Clock Line High Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cStandardModeSerialClockLineHighPeriod              | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Standard Mode Serial Clock Line Low Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cStandardModeSerialClockLineLowPeriod               | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Standard Mode Serial Data Line Transmit Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod       | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Standard Mode Serial Data Line Receive Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod       | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Serial Clock Line High Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModeSerialClockLineHighPeriod                  | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Serial Clock Line Low Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModeSerialClockLineLowPeriod                   | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Serial Data Line Transmit Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod           | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Serial Data Line Receive Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod            | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Maximum Length Of Suppressed Spikes In Std Mode Fast Mode And Fast Mode Plus} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cMaxSuppressedSpikesSMFMFMP               | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Plus Serial Clock Line High Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModePlusSerialClockLineHighPeriod              | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Plus Serial Clock Line Low Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModePlusSerialClockLineLowPeriod               | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Plus Serial Data Line Transmit Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod       | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Fast Mode Plus Serial Data Line Receive Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod        | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c High Speed Mode Plus Serial Clock Line High Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod         | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c High Speed Mode Plus Serial Clock Line Low Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod          | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c High Speed Mode Plus Serial Data Line Transmit Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod  | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c High Speed Mode Plus Serial Data Line Receive Hold Period} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod   | * | 0x08 | {0x0, 0x0}

  # !BSF NAME:{Touch Host Controller Hid Over I2c Maximum Length Of Suppressed Spikes In High Speed Mode} TYPE:{EditNum, HEX, (0,0xFFFFFFFFFFFFFFFF)}
  # !HDR STRUCT:{UINT32}
  gPlatformFspPkgTokenSpaceGuid.ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode     | * | 0x08 | {0x0, 0x0}

  ## THC_CONFIG End

  # !BSF NAME:{THC Wake On Touch GPIO resource Edge or Level} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Definition of GPIO resource configuration of Edge or Level}
  gPlatformFspPkgTokenSpaceGuid.ThcWotEdgeLevel             | * | 0x02 | {0x0, 0x0}

  # !BSF NAME:{THC Wake On Touch GPIO resource of Active Level} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Definition of GPIO resource configuration of Active Level}
  gPlatformFspPkgTokenSpaceGuid.ThcWotActiveLevel             | * | 0x02 | {0x1, 0x1}

  # !BSF NAME:{THC Wake On Touch GPIO resource of pin configuration} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !BSF HELP:{Definition of GPIO resource configuration of pin configuration}
  gPlatformFspPkgTokenSpaceGuid.ThcWotPinConfig             | * | 0x02 | {0x0, 0x0}

  # !BSF NAME:{THC customized SubSytem ID for Port} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !HDR STRUCT:{UINT16}
  gPlatformFspPkgTokenSpaceGuid.ThcCustomizedSsid             | * | 0x04 | {0x7270, 0x7270}

  # !BSF NAME:{THC Sets Customized SubSytem Vendor ID for Port} TYPE:{EditNum, HEX, (0,0xFFFF)}
  # !HDR STRUCT:{UINT16}
  gPlatformFspPkgTokenSpaceGuid.ThcCustomizedSvid             | * | 0x04 | {0x8086, 0x8086}

  # Added reserved space  UnusedUpdSpace38[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd38                | * | 0x2 | {0x00}

  # !BSF NAME:{USB 3.1 Speed Selection} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Choose USB 3.1 Port Speed Selection. Each bit represents a port. 1: Gen1, 0: Gen2}
  gPlatformFspPkgTokenSpaceGuid.Usb31PortSpeed              | * | 0x04 | 0x00000000

  # !BSF NAME:{PchPostMemRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for PCH Post-Mem}
  gPlatformFspPkgTokenSpaceGuid.PchPostMemRsvd              | * | 0x15 | {0x00}

  ## THERMAL_CONFIG Start

  # !BSF NAME:{PCHHOT# pin} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable PCHHOT# pin assertion when temperature is higher than PchHotLevel. 0: disable, 1: enable}
  gPlatformFspPkgTokenSpaceGuid.PchHotEnable                | * | 0x01 | 0x00
  # !BSF NAME:{Thermal Throttling Custimized T0Level Value} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Custimized T0Level value.}
  gPlatformFspPkgTokenSpaceGuid.PchT0Level                  | * | 0x02 | 0x0000

  # !BSF NAME:{Thermal Throttling Custimized T1Level Value} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Custimized T1Level value.}
  gPlatformFspPkgTokenSpaceGuid.PchT1Level                  | * | 0x02 | 0x0000

  # !BSF NAME:{Thermal Throttling Custimized T2Level Value} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Custimized T2Level value.}
  gPlatformFspPkgTokenSpaceGuid.PchT2Level                  | * | 0x02 | 0x0000

  # !BSF NAME:{Enable The Thermal Throttle} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable the thermal throttle function.}
  gPlatformFspPkgTokenSpaceGuid.PchTTEnable                 | * | 0x01 | 0x00

  # !BSF NAME:{PMSync State 13} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When set to 1 and the programmed GPIO pin is a 1, then PMSync state 13 will force at least T2 state.}
  gPlatformFspPkgTokenSpaceGuid.PchTTState13Enable          | * | 0x01 | 0x00

  # !BSF NAME:{Thermal Throttle Lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Thermal Throttle Lock.}
  gPlatformFspPkgTokenSpaceGuid.PchTTLock                   | * | 0x01 | 0x00

  # !BSF NAME:{Thermal Throttling Suggested Setting} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Thermal Throttling Suggested Setting.}
  gPlatformFspPkgTokenSpaceGuid.TTSuggestedSetting          | * | 0x01 | 0x01

  # !BSF NAME:{Thermal Device Temperature} TYPE:{EditNum, HEX, (0x00,0xFFFF)}
  # !BSF HELP:{Decides the temperature.}
  gPlatformFspPkgTokenSpaceGuid.PchTemperatureHotLevel      | * | 0x02 | 0x0073

  ## THERMAL_CONFIG End

  ## TSN_CONFIG Start
  # !BSF NAME:{Enable PCH TSN} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable TSN on the PCH.}
  gPlatformFspPkgTokenSpaceGuid.PchTsnEnable                | * | 0x04 | {0x01, 0x01, 0x01, 0x01}

  # Added reserved space    UnusedUpdSpace20
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd20               | * | 0x2 | 0x0

  # !BSF NAME:{PCH TSN MAC Address High Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN MAC Address High.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn1MacAddressHigh       | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN MAC Address Low Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN MAC Address Low.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn1MacAddressLow        | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN2 MAC Address High Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN2 MAC Address High.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn2MacAddressHigh       | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN2 MAC Address Low Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN2 MAC Address Low.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn2MacAddressLow        | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN3 MAC Address High Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN3 MAC Address High.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn3MacAddressHigh       | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN3 MAC Address Low Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN3 MAC Address Low.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn3MacAddressLow        | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN4 MAC Address High Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN4 MAC Address High.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn4MacAddressHigh       | * | 0x04 | 0x00000000

  # !BSF NAME:{PCH TSN MAC4 Address Low Bits} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Set TSN MAC4 Address Low.}
  gPlatformFspPkgTokenSpaceGuid.PchTsn4MacAddressLow        | * | 0x04 | 0x00000000

  ## TSN_CONFIG End

  ## USB_CONFIG Start

  # !BSF NAME:{Enable USB2 ports} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable per USB2 ports. One byte for each port, byte0 for port0, byte1 for port1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PortUsb20Enable             | * | 0x10 | { 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}

!if (gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1)
  # !BSF NAME:{Enable USB2 SW Device Mode} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable SW device mode per USB2 ports. One byte for each port, byte0 for port0, byte1 for port1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PortUsb20SwDeviceModeEnable | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB3 compatible port} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{For the USB2 port, whether it also can be used as USB3 port. 0: no; 1: yes.}
  gPlatformFspPkgTokenSpaceGuid.Usb3CompatiblePort          | * | 0x10 | { 0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB3 port number of compatible port} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB3 port number if the USB2 port can be used as USB3. 0: port1; 1: port2; 2: port3. Usb3CompatiblePort=1 is  the premise.}
  gPlatformFspPkgTokenSpaceGuid.Usb3CompatiblePortNum       | * | 0x10 | { 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
!endif

  # !BSF NAME:{Enable USB3 ports} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Enable/disable per USB3 ports. One byte for each port, byte0 for port0, byte1 for port1, and so on.}
  gPlatformFspPkgTokenSpaceGuid.PortUsb30Enable             | * | 0x0A | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

!if gSiPkgTokenSpaceGuid.PcdEmbeddedEnable == 0x1
  # !BSF NAME:{USB 3.1 Speed Selection} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Choose USB 3.1 Speed Selection. 1: Gen1, 0: Gen2}
  gPlatformFspPkgTokenSpaceGuid.PortUsb31Speed              | * | 0x01 | 0x00
!endif

  # !BSF NAME:{Enable xDCI controller} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable to xDCI controller.}
  gPlatformFspPkgTokenSpaceGuid.XdciEnable                  | * | 0x01 | 0x00

  # !BSF NAME:{USB PDO Programming} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PDO programming for USB in PEI phase. Disabling will allow for programming during later phase. 1: enable, 0: disable}
  gPlatformFspPkgTokenSpaceGuid.UsbPdoProgramming                   | * | 0x01 | 0x01

  # !BSF NAME:{USB Audio Offload enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable USB Audio Offload capabilites. 0: disabled, 1: enabled (default)}
  gPlatformFspPkgTokenSpaceGuid.PchXhciUaolEnable | * | 0x01 | 0x01

  # !BSF NAME:{PCH USB OverCurrent mapping enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{1: Will program USB OC pin mapping in xHCI controller memory, 0: Will clear OC pin mapping allow for NOA usage of OC pins}
  gPlatformFspPkgTokenSpaceGuid.PchUsbOverCurrentEnable     | * | 0x01 | 0x01

  # !BSF NAME:{USB2 Port Over Current Pin} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Describe the specific over current pin number of USB 2.0 Port N.}
  gPlatformFspPkgTokenSpaceGuid.Usb2OverCurrentPin          | * | 0x10 | { 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04, 0x05, 0x05, 0x06, 0x06, 0x07, 0x07 }

  # !BSF NAME:{USB3 Port Over Current Pin} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{Describe the specific over current pin number of USB 3.0 Port N.}
  gPlatformFspPkgTokenSpaceGuid.Usb3OverCurrentPin          | * | 0x0A | { 0x00, 0x00, 0x01, 0x01, 0x02, 0x02, 0x03, 0x03, 0x04, 0x04 }

  # !BSF NAME:{Enable xHCI LTR override} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables override of recommended LTR values for xHCI}
  gPlatformFspPkgTokenSpaceGuid.PchUsbLtrOverrideEnable         | * | 0x01 | 0x00

  # !BSF NAME:{USB DWB enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable USB DWB. 0: disabled, 1: enabled (default)}
  gPlatformFspPkgTokenSpaceGuid.PchXhciDwbEnable | * | 0x01 | 0x01

  # Added reserved space    UnusedUpdSpace21[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd21                   | * | 0x2 | {0x00}

  # !BSF NAME:{xHCI High Idle Time LTR override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Value used for overriding LTR recommendation for xHCI High Idle Time LTR setting}
  gPlatformFspPkgTokenSpaceGuid.PchUsbLtrHighIdleTimeOverride   | * | 0x04 | 0x00000000

  # !BSF NAME:{xHCI Medium Idle Time LTR override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Value used for overriding LTR recommendation for xHCI Medium Idle Time LTR setting}
  gPlatformFspPkgTokenSpaceGuid.PchUsbLtrMediumIdleTimeOverride | * | 0x04 | 0x00000000

  # !BSF NAME:{xHCI Low Idle Time LTR override} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Value used for overriding LTR recommendation for xHCI Low Idle Time LTR setting}
  gPlatformFspPkgTokenSpaceGuid.PchUsbLtrLowIdleTimeOverride    | * | 0x04 | 0x00000000

  # !BSF NAME:{USB2 Port Reset Message Enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{0: Disable USB2 Port Reset Message; 1: Enable USB2 Port Reset Message; This must be enable for USB2 Port those are paired with CPU XHCI Port}
  gPlatformFspPkgTokenSpaceGuid.PortResetMessageEnable      | * | 0x10 | { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }

  # !BSF NAME:{PCH USB OverCurrent mapping lock enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{If this policy option is enabled then BIOS will program OCCFDONE bit in xHCI meaning that OC mapping data will be consumed by xHCI and OC mapping registers will be locked.}
  gPlatformFspPkgTokenSpaceGuid.PchXhciOcLock               | * | 0x01 | 0x01

  ## USB_CONFIG End

  ## USB2_PHY_CONFIG Start

  # !BSF NAME:{USB Per Port HS Preemphasis Bias} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB Per Port HS Preemphasis Bias. 000b-0mV, 001b-11.25mV, 010b-16.9mV, 011b-28.15mV, 100b-28.15mV, 101b-39.35mV, 110b-45mV, 111b-56.3mV. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb2PhyPetxiset             | * | 0x10 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB Per Port HS Transmitter Bias} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB Per Port HS Transmitter Bias. 000b-0mV, 001b-11.25mV, 010b-16.9mV, 011b-28.15mV, 100b-28.15mV, 101b-39.35mV, 110b-45mV, 111b-56.3mV, One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb2PhyTxiset               | * | 0x10 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB Per Port HS Transmitter Emphasis} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB Per Port HS Transmitter Emphasis. 00b - Emphasis OFF, 01b - De-emphasis ON, 10b - Pre-emphasis ON, 11b - Pre-emphasis & De-emphasis ON. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb2PhyPredeemp             | * | 0x10 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB Per Port Half Bit Pre-emphasis} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB Per Port Half Bit Pre-emphasis. 1b - half-bit pre-emphasis, 0b - full-bit pre-emphasis. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb2PhyPehalfbit            | * | 0x10 | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  ## USB2_PHY_CONFIG End

  ## USB3_HSIO_CONFIG Start

  # !BSF NAME:{Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output -3.5dB De-Emphasis Adjustment. Each value in arrary can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxDeEmphEnable      | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output -3.5dB De-Emphasis Adjustment Setting, HSIO_TX_DWORD5[21:16], <b>Default = 29h</b> (approximately -3.5dB De-Emphasis). One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxDeEmph            | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output Downscale Amplitude Adjustment, Each value in arrary can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxDownscaleAmpEnable  | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output Downscale Amplitude Adjustment} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output Downscale Amplitude Adjustment, HSIO_TX_DWORD8[21:16], <b>Default = 00h</b>. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxDownscaleAmp      | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 adapted linear equalization parameters programming enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {After enabling BIOS will program adapter linear equalization parameters <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioCtrlAdaptOffsetCfgEnable   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 LFPS sensitivity levels parameter programming enable N} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Enabling this option will enable LFPS parameters programming<b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioFilterSelNEnable   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 LFPS sensitivity levels parameter programming enable P} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Enabling this option will enable LFPS parameters programming<b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioFilterSelPEnable   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 input offset control enable} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Enables programming of input offset parameter, <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioOlfpsCfgPullUpDwnResEnable   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 adapted linear equalization parameters value} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {This will change the actual adapted linear equalization value CTLE_ADAPT, <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioCtrlAdaptOffsetCfg   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 Controls the input offset} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Controls the input offset, works in conjunction with LFPS sensitivity level, <b>Default = 3h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioOlfpsCfgPullUpDwnRes   | * | 0xA | {0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03}

  # !BSF NAME: {USB 3.0 LFPS sensitivity level for N} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Sensitivity level for LFPS circuitry, <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioFilterSelN   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME: {USB 3.0 LFPS sensitivity level for P} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP: {Sensitivity level for LFPS circuitry, <b>Default = 0h</b>.}
  gPlatformFspPkgTokenSpaceGuid.PchUsb3HsioFilterSelP   | * | 0xA | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 3} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 3, Each value in array can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate3UniqTranEnable  | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output Unique Transition Bit Scale for rate 3} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output Unique Transition Bit Scale for rate 3, HSIO_TX_DWORD9[6:0], <b>Default = 4Ch</b>. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate3UniqTran      | * | 0x0A | {0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C}

  # !BSF NAME:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 2} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 2, Each value in array can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate2UniqTranEnable  | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output Unique Transition Bit Scale for rate 2} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output Unique Transition Bit Scale for rate 2, HSIO_TX_DWORD9[14:8], <b>Default = 4Ch</b>. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate2UniqTran      | * | 0x0A | {0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C}

  # !BSF NAME:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 1} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 1, Each value in array can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate1UniqTranEnable  | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output Unique Transition Bit Scale for rate 1} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output Unique Transition Bit Scale for rate 1, HSIO_TX_DWORD9[22:16], <b>Default = 4Ch</b>. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate1UniqTran      | * | 0x0A | {0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C}

  # !BSF NAME:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 0} TYPE:{EditNum, HEX, (0x00,0x01010101010101010101)}
  # !BSF HELP:{Enable the write to USB 3.0 TX Output Unique Transition Bit Mode for rate 0, Each value in array can be between 0-1. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate0UniqTranEnable  | * | 0x0A | {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}

  # !BSF NAME:{USB 3.0 TX Output Unique Transition Bit Scale for rate 0} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFFFFFFFFFFFFFF)}
  # !BSF HELP:{USB 3.0 TX Output Unique Transition Bit Scale for rate 0, HSIO_TX_DWORD9[30:24], <b>Default = 4Ch</b>. One byte for each port.}
  gPlatformFspPkgTokenSpaceGuid.Usb3HsioTxRate0UniqTran      | * | 0x0A | {0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C, 0x4C}

  ## USB3_HSIO_CONFIG End

  ## FIA_CONFIG Start

  # !BSF NAME:{PCIe Fia Programming} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Load Fia configuration if enable. 0: Disable; 1: Enable(Default).}
  gPlatformFspPkgTokenSpaceGuid.PcieFiaProgramming | * | 0x01 | 0x01

  ## FIA_CONFIG End

  #
  # ME Post-Mem Restricted Block Start
  #

  # !BSF NAME:{Enable SSE Device} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Test, 0: POR, 1: enable, 2: disable, Enable/Disable SSE/SSE++ Devices from PCI config space}
  gPlatformFspPkgTokenSpaceGuid.SseCommunication             | * | 0x01 | 0x00

  # !BSF NAME:{MePostMemRestrictedRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for ME Post-Mem Restricted}
  gPlatformFspPkgTokenSpaceGuid.MePostMemRestrictedRsvd      | * | 0x02 | {0x00}

  #
  # ME Post-Mem Restricted Block End
  #

  ## NPU_PEI_CONFIG Start

  # !BSF NAME:{Enable/Disable NPU Device} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable NPU Device, Disable: Disable NPU Device}
  gPlatformFspPkgTokenSpaceGuid.NpuEnable                  | * | 0x01 | 0x1

  ## NPU_PEI_CONFIG End

  ## GBE_CONFIG Start

  # !BSF NAME:{Enable LAN} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable LAN controller.}
  gPlatformFspPkgTokenSpaceGuid.PchLanEnable                | * | 0x01 | 0x01

  # !BSF NAME:{Enable PCH Lan LTR capabilty of PCH internal LAN} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchLanLtrEnable             | * | 0x01 | 0x01

  # !BSF NAME:{PCH Lan WOL Fast Support} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enables bit B_PCH_ACPI_GPE0_EN_127_96_PME_B0 during PchLanSxCallback in PchLanSxSmm.}
  gPlatformFspPkgTokenSpaceGuid.PchLanWOLFastSupport | * | 0x01 | 0x00

  ## GBE_CONFIG End

  #
  # SI Post-Mem Block Start
  #
  # !BSF PAGE:{SI2}

  ## SI_CONFIG Start

  # !BSF NAME:{Skip Ssid Programming.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When set to TRUE, silicon code will not do any SSID programming and platform code needs to handle that by itself properly.}
  gPlatformFspPkgTokenSpaceGuid.SiSkipSsidProgramming    | * | 0x01 | 0x00

  # !BSF NAME:{Change Default SVID} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Change the default SVID used in FSP to programming internal devices. This is only valid when SkipSsidProgramming is FALSE.}
  gPlatformFspPkgTokenSpaceGuid.SiCustomizedSvid    | * | 0x02 | 0x0000

  # !BSF NAME:{Change Default SSID} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{Change the default SSID used in FSP to programming internal devices. This is only valid when SkipSsidProgramming is FALSE.}
  gPlatformFspPkgTokenSpaceGuid.SiCustomizedSsid    | * | 0x02 | 0x0000

  # !BSF NAME:{CAN Configurations} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CAN Controllers.0: Disable, <b>1: Enable</b>}
  gPlatformFspPkgTokenSpaceGuid.PchCanEnable                | * | 0x02 | {0x01, 0x01}

  # Added reserved space    UnusedUpdSpace22[4]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd22               | * | 0x4 | {0x00}

  #
  # typedef struct {
  #   UINT16  SubSystemVendorId; // If giving 0, FSP will program default for the device.
  #   UINT16  SubSystemId;       // If giving 0, FSP will program default for the device.
  # } SVID_SID_VALUE;
  #
  # typedef struct {
  #   union {
  #     struct {
  #       UINT64  Register:12;   // SSID register offset
  #       UINT64  Function:3;
  #       UINT64  Device:5;
  #       UINT64  Bus:8;
  #       UINT64  Reserved1:4;
  #       UINT64  Segment:16;
  #       UINT64  Reserved2:16;
  #     } Bits;
  #     UINT64    SegBusDevFuncRegister;
  #   } Address;
  #   SVID_SID_VALUE SvidSidValue;
  #   UINT32 Reserved;
  # } SVID_SID_INIT_ENTRY;
  #
  # Example table structure:
  # SVID_SID_INIT_ENTRY mSsidTablePtr[] = {
  #   {{{Register, Function, Device, Bus, Reserved1, Segment, Reserved2}}, {SubSystemVendorId, SubSystemId}, Reserved},
  #   {{{       0,        5,     31,   0,         0,       0,         0}}, {           0x1234,      0x5678}        ,0}
  # }
  #
  # Note:
  # In the table, only Device, Function, SubSystemVendorId and SubSystemId fields are required.
  # When both SiSsidTablePtr and SiNumberOfSsidTableEntry are non-zero, FSP will follow the table to program SSID for corresponding devices (either given value or default value).
  #
  # !BSF NAME:{SVID SDID table Poniter.} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The address of the table of SVID SDID to customize each SVID SDID entry. This is only valid when SkipSsidProgramming is FALSE.}
  gPlatformFspPkgTokenSpaceGuid.SiSsidTablePtr              | * | 0x08 | 0x00000000

  # !BSF NAME:{Number of ssid table.} TYPE:{EditNum, HEX, (0x00, 0xFFFF)}
  # !BSF HELP:{SiNumberOfSsidTableEntry should match the table entries created in SiSsidTablePtr. This is only valid when SkipSsidProgramming is FALSE.}
  gPlatformFspPkgTokenSpaceGuid.SiNumberOfSsidTableEntry    | * | 0x02 | 0x0000

  # !BSF NAME:{Skip DFX.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Skip DFX.}
  gPlatformFspPkgTokenSpaceGuid.DfxSkipBiosDone | * | 0x01 | 0x00

  ## SI_CONFIG End

  # !BSF NAME:{SiPostMemRsvd} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Reserved for SI Post-Mem}
  gPlatformFspPkgTokenSpaceGuid.SiPostMemRsvd               | * | 0x06 | {0x00}
  # Added reserved space    UnusedUpdSpace23[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd23               | * | 0x3 | {0x00}

  #
  # SI Post-Mem Block End
  #

  ## GRAPHICS_PEI_CONFIG Start

  # !BSF PAGE:{SA2}
  # !BSF NAME:{LogoPixelHeight Address}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Address of LogoPixelHeight}
  gPlatformFspPkgTokenSpaceGuid.LogoPixelHeight             | * | 0x04 | 0x00000000

  # !BSF NAME:{LogoPixelWidth Address}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Address of LogoPixelWidth}
  gPlatformFspPkgTokenSpaceGuid.LogoPixelWidth              | * | 0x04 | 0x00000000
  # Added reserved space    UnusedUpdSpace24[4]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd24               | * | 0x4 | {0x00}

  # !BSF NAME:{Blt Buffer Address}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Address of Blt buffer}
  gPlatformFspPkgTokenSpaceGuid.BltBufferAddress            | * | 0x08 | 0x00000000

  # !BSF NAME:{Graphics Configuration Ptr}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Points to VBT}
  gPlatformFspPkgTokenSpaceGuid.GraphicsConfigPtr           | * | 0x08 | 0x00000000

  # !BSF NAME:{Enable/Disable SkipFspGop} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: Skip FSP provided GOP driver, Disable(Default): Use FSP provided GOP driver}
  gPlatformFspPkgTokenSpaceGuid.SkipFspGop                  | * | 0x01 | 0x0

  # !BSF NAME:{Enable/Disable Media Configuration} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Configure Media for use, Disable: Skip Media Configuration}
  gPlatformFspPkgTokenSpaceGuid.ConfigureMedia              | * | 0x01 | 0x1

  # !BSF NAME:{Enable/Disable IGFX RenderStandby} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable IGFX RenderStandby, Disable: Disable IGFX RenderStandby}
  gPlatformFspPkgTokenSpaceGuid.RenderStandby               | * | 0x01 | 0x1

  # !BSF NAME:{Enable/Disable GT Configuration} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Configure GT for use, Disable: Skip GT Configuration}
  gPlatformFspPkgTokenSpaceGuid.ConfigureGT                 | * | 0x01 | 0x1

  # !BSF NAME:{Enable RC1p GT frequency request to PMA (provided all other conditions are met)}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.RC1pGtFreqEnable            | * | 0x01 | 0x0

  # !BSF NAME:{Enable RC1p Media frequency request to PMA (provided all other conditions are met)}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0(Default)=Disable, 1=Enable}
  gPlatformFspPkgTokenSpaceGuid.RC1pMediaFreqEnable         | * | 0x01 | 0x0

  # !BSF NAME:{Enable/Disable PavpEnable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable PavpEnable, Disable: Disable PavpEnable}
  gPlatformFspPkgTokenSpaceGuid.PavpEnable                  | * | 0x01 | 0x1

  # !BSF NAME:{Enable/Disable PeiGraphicsPeimInit} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{<b>Enable(Default):</b> FSP will initialize the framebuffer and provide it via EFI_PEI_GRAPHICS_INFO_HOB. Disable: FSP will NOT initialize the framebuffer.}
  gPlatformFspPkgTokenSpaceGuid.PeiGraphicsPeimInit         | * | 0x01 | 0x1

  # !BSF NAME:{Enable/Disable IGFX Media Standby} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable(Default): Enable IGFX Media Standby, Disable: Disable IGFX MediaStandby}
  gPlatformFspPkgTokenSpaceGuid.MediaStandby               | * | 0x01 | 0x1
  # Added reserved space    UnusedUpdSpace25[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd25              | * | 0x3 | {0x00}

  # !BSF NAME:{Intel Graphics VBT (Video BIOS Table) Size}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Size of Internal Graphics VBT Image}
  gPlatformFspPkgTokenSpaceGuid.VbtSize                     | * | 0x04 | 0x00000000

  # !BSF NAME:{Platform LID Status for LFP Displays.} TYPE:{Combo}
  # !BSF OPTION:{0: LidClosed, 1: LidOpen}
  # !BSF HELP:{LFP Display Lid Status (LID_STATUS enum): 0 (Default): LidClosed, 1: LidOpen.}
  gPlatformFspPkgTokenSpaceGuid.LidStatus                   | * | 0x01 | 0x0
  # Added reserved space    UnusedUpdSpace26[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd26               | * | 0x3 | {0x00}

  # !BSF NAME:{HorizontalResolution for PEI Logo}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{HorizontalResolution from PEIm Gfx for PEI Logo}
  gPlatformFspPkgTokenSpaceGuid.HorizontalResolution        | * | 0x04 | 0x00000000

  # !BSF NAME:{VerticalResolution for PEI Logo}
  # !BSF TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{VerticalResolution from PEIm Gfx for PEI Logo}
  gPlatformFspPkgTokenSpaceGuid.VerticalResolution          | * | 0x04 | 0x00000000

  # Added reserved space FspsUpdRsvd38[56]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd388              | * | 0x38 | {0x00}

  ## GRAPHICS_PEI_CONFIG End

  ## PCH_INTERRUPT_CONFIG Start

  # !BSF NAME:{Address of PCH_DEVICE_INTERRUPT_CONFIG table.} TYPE:{EditNum, HEX, (0x00, 0xFFFFFFFF)}
  # !BSF HELP:{The address of the table of PCH_DEVICE_INTERRUPT_CONFIG.}
  gPlatformFspPkgTokenSpaceGuid.DevIntConfigPtr             | * | 0x04 | 0x00

  # !BSF NAME:{Number of DevIntConfig Entry} TYPE:{EditNum, HEX, (0x00,0x40)}
  # !BSF HELP:{Number of Device Interrupt Configuration Entry. If this is not zero, the DevIntConfigPtr must not be NULL.}
  gPlatformFspPkgTokenSpaceGuid.NumOfDevIntConfig           | * | 0x01 | 0x00

  # !BSF NAME:{Select GPIO IRQ Route}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{GPIO IRQ Select. The valid value is 14 or 15.}
  gPlatformFspPkgTokenSpaceGuid.GpioIrqRoute                | * | 0x01 | 0x0E

  # !BSF NAME:{Select SciIrqSelect}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x17)}
  # !BSF HELP:{SCI IRQ Select. The valid value is 9, 10, 11, and 20, 21, 22, 23 for APIC only.}
  gPlatformFspPkgTokenSpaceGuid.SciIrqSelect                | * | 0x01 | 0x09

  # !BSF NAME:{Select TcoIrqSelect}
  # !BSF TYPE:{EditNum, HEX, (0x00,0x17)}
  # !BSF HELP:{TCO IRQ Select. The valid value is 9, 10, 11, 20, 21, 22, 23.}
  gPlatformFspPkgTokenSpaceGuid.TcoIrqSelect                | * | 0x01 | 0x09

  # !BSF NAME:{Enable/Disable Tco IRQ}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}}
  # !BSF HELP:{Enable/disable TCO IRQ}
  gPlatformFspPkgTokenSpaceGuid.TcoIrqEnable                | * | 0x01 | 0x00

  ## PCH_INTERRUPT_CONFIG End

  ## ADR_CONFIG Start

  # !BSF NAME:{PMC ADR enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable asynchronous DRAM refresh}
  gPlatformFspPkgTokenSpaceGuid.PmcAdrEn                    | * | 0x1  | 0x00

  # !BSF NAME:{PMC ADR timer configuration enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable ADR timer configuration}
  gPlatformFspPkgTokenSpaceGuid.PmcAdrTimerEn               | * | 0x1  | 0x00

  # !BSF NAME:{PMC ADR phase 1 timer value} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:[Specify the timer value phase 1 ADR timer}
  gPlatformFspPkgTokenSpaceGuid.PmcAdrTimer1Val             | * | 0x1  | 0x00

  # !BSF NAME:{PMC ADR phase 1 timer multiplier value} TYPE:{EditNum, HEX, (0x00, 0xFF)}
  # !BSF HELP:{Specify the multiplier value for phase 1 ADR timer}
  gPlatformFspPkgTokenSpaceGuid.PmcAdrMultiplier1Val        | * | 0x1  | 0x00

  # !BSF NAME:{PMC ADR host reset partition enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Specify whether PMC should set ADR_RST_STS bit after receiving Reset_Warn_Ack DMI message}
  gPlatformFspPkgTokenSpaceGuid.PmcAdrHostPartitionReset    | * | 0x1  | 0x00

  ## ADR_CONFIG End

  ## PCH_FIVR_CONFIG Start

  # !BSF NAME:{Mask to enable the usage of external V1p05 VR rail in specific S0ix or Sx states} TYPE:{EditNum, HEX, (0x00,0x1F)}
  # !BSF HELP:{Enable External V1P05 Rail in: BIT0:S0i1/S0i2, BIT1:S0i3, BIT2:S3, BIT3:S4, BIT5:S5}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailEnabledStates    | * | 0x1 | 0x0

  # !BSF NAME:{Mask to enable the platform configuration of external V1p05 VR rail} TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{External V1P05 Rail Supported Configuration}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailSupportedVoltageStates  | * | 0x1 | 0x0

  # !BSF NAME:{External V1P05 Voltage Value that will be used in S0i2/S0i3 states} TYPE:{EditNum, HEX, (0x0,0x07FF)}
  # !BSF HELP:{Value is given in 2.5mV increments (0=0mV, 1=2.5mV, 2=5mV...)}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailVoltage          | * | 0x2 | 0x01A4

  # !BSF NAME:{External V1P05 Icc Max Value} TYPE:{EditNum, HEX, (0x0,0xC8)}
  # !BSF HELP:{Granularity of this setting is 1mA and maximal possible value is 200mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailIccMax           | * | 0x1 | 0x64

  # !BSF NAME:{Mask to enable the usage of external Vnn VR rail in specific S0ix or Sx states} TYPE:{EditNum, HEX, (0x00,0x1F)}
  # !BSF HELP:{Enable External Vnn Rail in: BIT0:S0i1/S0i2, BIT1:S0i3, BIT2:S3, BIT3:S4, BIT5:S5}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailEnabledStates      | * | 0x1 | 0x0

  # !BSF NAME:{Mask to enable the platform configuration of external Vnn VR rail} TYPE:{EditNum, HEX, (0x00,0x0F)}
  # !BSF HELP:{External Vnn Rail Supported Configuration}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailSupportedVoltageStates  | * | 0x1 | 0x0
  # Added reserved space    UnusedUpdSpace27
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd27                       | * | 0x1 | 0x00

  # !BSF NAME:{External Vnn Voltage Value that will be used in S0ix/Sx states} TYPE:{EditNum, HEX, (0x0,0x07FF)}
  # !BSF HELP:{Value is given in 2.5mV increments (0=0mV, 1=2.5mV, 2=5mV...), Default is set to 420}}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailVoltage            | * | 0x2 | 0x01A4

  # !BSF NAME:{External Vnn Icc Max Value that will be used in S0ix/Sx states} TYPE:{EditNum, HEX, (0x0,0xC8)}
  # !BSF HELP:{Granularity of this setting is 1mA and maximal possible value is 200mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailIccMax             | * | 0x1 | 0xC8

  # !BSF NAME:{Mask to enable the usage of external Vnn VR rail in Sx states} TYPE:{EditNum, HEX, (0x00,0x1F)}
  # !BSF HELP:{Use only if Ext Vnn Rail config is different in Sx. Enable External Vnn Rail in Sx: BIT0-1:Reserved, BIT2:S3, BIT3:S4, BIT5:S5}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailSxEnabledStates    | * | 0x1 | 0x0

  # !BSF NAME:{External Vnn Voltage Value that will be used in Sx states} TYPE:{EditNum, HEX, (0x0,0x07FF)}
  # !BSF HELP:{Use only if Ext Vnn Rail config is different in Sx. Value is given in 2.5mV increments (0=0mV, 1=2.5mV, 2=5mV...)}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailSxVoltage          | * | 0x2 | 0x01A4

  # !BSF NAME:{External Vnn Icc Max Value that will be used in Sx states} TYPE:{EditNum, HEX, (0x0,0xC8)}
  # !BSF HELP:{Use only if Ext Vnn Rail config is different in Sx. Granularity of this setting is 1mA and maximal possible value is 200mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailSxIccMax           | * | 0x1 | 0xC8

  # !BSF NAME:{Transition time in microseconds from Low Current Mode Voltage to High Current Mode Voltage} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{This field has 1us resolution. When value is 0 PCH will not transition VCCIN_AUX to low current mode voltage.}
  gPlatformFspPkgTokenSpaceGuid.PchFivrVccinAuxLowToHighCurModeVolTranTime  | * | 0x1 | 0x0C

  # !BSF NAME:{Transition time in microseconds from Retention Mode Voltage to High Current Mode Voltage} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{This field has 1us resolution. When value is 0 PCH will not transition VCCIN_AUX to retention mode voltage.}
  gPlatformFspPkgTokenSpaceGuid.PchFivrVccinAuxRetToHighCurModeVolTranTime  | * | 0x1 | 0x036

  # !BSF NAME:{Transition time in microseconds from Retention Mode Voltage to Low Current Mode Voltage} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{This field has 1us resolution. When value is 0 PCH will not transition VCCIN_AUX to retention mode voltage.}
  gPlatformFspPkgTokenSpaceGuid.PchFivrVccinAuxRetToLowCurModeVolTranTime   | * | 0x1 | 0x2B

  # !BSF NAME:{Transition time in microseconds from Off (0V) to High Current Mode Voltage} TYPE:{EditNum, HEX, (0x0,0x7FF)}
  # !BSF HELP:{This field has 1us resolution. When value is 0 Transition to 0V is disabled.}
  gPlatformFspPkgTokenSpaceGuid.PchFivrVccinAuxOffToHighCurModeVolTranTime  | * | 0x2 | 0x0096

  # !BSF NAME:{FIVR Dynamic Power Management} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable FIVR Dynamic Power Management.}
  gPlatformFspPkgTokenSpaceGuid.PchFivrDynPm                            | * | 0x01 | 0x01
  # Added reserved space    UnusedUpdSpace28
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd28                           | * | 0x1 | 0x00

  # !BSF NAME:{External V1P05 Icc Max Value} TYPE:{EditNum, HEX, (0x0,0x1F4)}
  # !BSF HELP:{Granularity of this setting is 1mA and maximal possible value is 500mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailIccMaximum           | * | 0x2 | 0x1F4

  # !BSF NAME:{External Vnn Icc Max Value that will be used in S0ix/Sx states} TYPE:{EditNum, HEX, (0x0,0x1F4)}
  # !BSF HELP:{Granularity of this setting is 1mA and maximal possible value is 500mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailIccMaximum             | * | 0x2 | 0x1F4

  # !BSF NAME:{External Vnn Icc Max Value that will be used in Sx states} TYPE:{EditNum, HEX, (0x0,0x1F4)}
  # !BSF HELP:{Use only if Ext Vnn Rail config is different in Sx. Granularity of this setting is 1mA and maximal possible value is 500mA}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailSxIccMaximum           | * | 0x2 | 0x1F4

  # !BSF NAME:{External V1P05 Control Ramp Timer value} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Hold off time to be used when changing the v1p05_ctrl for external bypass value in us}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtV1p05RailCtrlRampTmr           | * | 0x1 | 0x1

  # !BSF NAME:{External VNN Control Ramp Timer value} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Hold off time to be used when changing the vnn_ctrl for external bypass value in us}
  gPlatformFspPkgTokenSpaceGuid.PchFivrExtVnnRailCtrlRampTmr           | * | 0x1 | 0x1

  ## PCH_FIVR_CONFIG End

  ## PCH_GENERAL_CONFIG Start

  # !BSF NAME:{PCH Compatibility Revision ID} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This member describes whether or not the CRID feature of PCH should be enabled.}
  gPlatformFspPkgTokenSpaceGuid.PchCrid                     | * | 0x01 | 0x00

  # !BSF NAME:{PCH Legacy IO Low Latency Enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set to enable low latency of legacy IO. <b>0: Disable</b>, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchLegacyIoLowLatency       | * | 0x01 | 0x00

  ## PCH_GENERAL_CONFIG End

  ## PCH_P2SB_CONFIG Start

  # !BSF NAME:{PCH P2SB} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{PCH P2SB}
  gPlatformFspPkgTokenSpaceGuid.SvTestUnhideP2sb  | * | 0x01 | 0x00

  # !BSF NAME:{PCH Unlock SideBand access} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{The SideBand PortID mask for certain end point (e.g. PSFx) will be locked before 3rd party code execution. 0: Lock SideBand access; 1: Unlock SideBand access.}
  gPlatformFspPkgTokenSpaceGuid.PchSbAccessUnlock           | * | 0x01 | 0x00

  ## PCH_P2SB_CONFIG End

  ## PCH_IOAPIC_CONFIG Start

  # !BSF NAME:{Enable 8254 Static Clock Gating} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Set 8254CGE=1 is required for SLP_S0 support. However, set 8254CGE=1 in POST time might fail to boot legacy OS using 8254 timer. Make sure it is disabled to support legacy OS using 8254 timer. Also enable this while S0ix is enabled.}
  gPlatformFspPkgTokenSpaceGuid.Enable8254ClockGating       | * | 0x01 | 0x01

  # !BSF NAME:{Enable 8254 Static Clock Gating On S3} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{This is only applicable when Enable8254ClockGating is disabled. FSP will do the 8254 CGE programming on S3 resume when Enable8254ClockGatingOnS3 is enabled. This avoids the SMI requirement for the programming.}
  gPlatformFspPkgTokenSpaceGuid.Enable8254ClockGatingOnS3   | * | 0x01 | 0x01

  # !BSF NAME:{Enable PCH Io Apic Entry 24-119} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{0: Disable; 1: Enable.}
  gPlatformFspPkgTokenSpaceGuid.PchIoApicEntry24_119        | * | 0x01 | 0x01

  # !BSF NAME:{PCH Io Apic ID} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{This member determines IOAPIC ID. Default is 0x02.}
  gPlatformFspPkgTokenSpaceGuid.PchIoApicId                 | * | 0x01 | 0x02

  ## PCH_IOAPIC_CONFIG End

  ## CNVI_CONFIG Start

  # !BSF NAME:{CNVi Configuration} TYPE:{Combo} OPTION:{0:Disable, 1:Auto}
  # !BSF HELP:{This option allows for automatic detection of Connectivity Solution. [Auto Detection] assumes that CNVi will be enabled when available, [Disable] allows for disabling CNVi.}
  gPlatformFspPkgTokenSpaceGuid.CnviMode                    | * | 0x01 | 0x01

  # !BSF NAME:{CNVi Wi-Fi Core} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CNVi Wi-Fi Core, Default is ENABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.CnviWifiCore                | * | 0x01 | 0x01

  # !BSF NAME:{CNVi BT Core} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable CNVi BT Core, Default is ENABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.CnviBtCore                  | * | 0x01 | 0x01

  # !BSF NAME:{CNVi BT Interface} TYPE:{Combo} OPTION:{1:USB, 2:PCI}
  # !BSF HELP:{This option configures BT device interface to either USB/PCI}
  gPlatformFspPkgTokenSpaceGuid.CnviBtInterface             | * | 0x01 | 0x01

  # !BSF NAME:{CNVi BT Audio Offload} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable BT Audio Offload, Default is ENABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.CnviBtAudioOffload          | * | 0x01 | 0x01

  # !BSF NAME:{WWAN Coex} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{WWAN Coex is getting updated from UEFI variable}
  gPlatformFspPkgTokenSpaceGuid.CnviWwanCoex                | * | 0x01 | 0x00

  # Added reserved space    UnusedUpdSpace29[2]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd29               | * | 0x2 | {0x00}

  # !BSF NAME:{CNVi RF_RESET pin muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select CNVi RF_RESET# pin depending on board routing. LP/P/M: GPP_A8 = 0x2942E408(default) or GPP_F4 = 0x194CE404. H/S: 0. Refer to GPIO_*_MUXING_CNVI_RF_RESET_* in GpioPins*.h.}
  gPlatformFspPkgTokenSpaceGuid.CnviRfResetPinMux           | * | 0x04 | 0x0

  # !BSF NAME:{CNVi CLKREQ pin muxing} TYPE:{EditNum, HEX, (0,0xFFFFFFFF)}
  # !BSF HELP:{Select CNVi CLKREQ pin depending on board routing. LP/P/M: GPP_A9 = 0x3942E609(default) or GPP_F5 = 0x394CE605. H/S: 0. Refer to GPIO_*_MUXING_CNVI_CRF_XTAL_CLKREQ_* in GpioPins*.h.}
  gPlatformFspPkgTokenSpaceGuid.CnviClkreqPinMux            | * | 0x04 | 0x0

  # !BSF NAME:{CNVi BT Audio OffOffloadInterfaceload} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/Disable BT Audio OffloadInterface, Default is ENABLE. 0: DISABLE, 1: ENABLE}
  gPlatformFspPkgTokenSpaceGuid.CnviBtAudioOffloadInterface          | * | 0x01 | 0x00

  ## CNVI_CONFIG End

  ## HOST_BRIDGE_PEI_CONFIG Start

  # !BSF NAME:{Enable Device 4}
  # !BSF TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable Device 4}
  gPlatformFspPkgTokenSpaceGuid.Device4Enable               | * | 0x01 | 0x00

  # !BSF NAME:{Skip PAM regsiter lock} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable: PAM register will not be locked by RC, platform code should lock it, Disable(Default): PAM registers will be locked by RC}
  gPlatformFspPkgTokenSpaceGuid.SkipPamLock                 | * | 0x01 | 0x0

  # !BSF NAME:{TCSS LSx OE Enable} TYPE:{EditNum, HEX, (0x0,0x000F)}
  # !BSF HELP:{Bits 0, 1, ... max Type C Rettimerless port LSx OE enables}
  gPlatformFspPkgTokenSpaceGuid.TcssLsxOe                  | * | 0x01 | 0x00

  ## HOST_BRIDGE_PEI_CONFIG End

  ## HDAUDIO_CONFIG Start
  # !BSF NAME:{PCH HDA Verb Table Entry Number} TYPE:{EditNum, HEX, (0x0,0xFF)}
  # !BSF HELP:{Number of Entries in Verb Table.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaVerbTableEntryNum     | * | 1 | 0
  # Added reserved space UnusedUpdSpace30[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd30               | * | 0x3 | {0x00}

  # !BSF NAME:{PCH HDA Verb Table Pointer} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{Pointer to Array of pointers to Verb Table.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaVerbTablePtr          | * | 0x8 | 0

  # !BSF NAME:{PCH HDA Codec Sx Wake Capability} TYPE:{$EN_DIS}
  # !BSF HELP:{Capability to detect wake initiated by a codec in Sx}
  gPlatformFspPkgTokenSpaceGuid.PchHdaCodecSxWakeCapability | * | 0x01 | 0x00

  # !BSF NAME:{Enable Pme} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable Azalia wake-on-ring.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaPme                   | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Link Frequency} TYPE:{Combo} OPTION:{0: 6MHz, 1: 12MHz, 2: 24MHz}
  # !BSF HELP:{HDA Link Freq (PCH_HDAUDIO_LINK_FREQUENCY enum): 0: 6MHz, 1: 12MHz, 2: 24MHz.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaLinkFrequency         | * | 0x01 | 0x02

  # !BSF NAME:{HD Audio Microphone Privacy Mode} TYPE:{Combo} OPTION:{0: No Microphone Privacy Support, 1: HW Managed Microphone Privacy, 2: FW Managed Microphone Privacy, 3: Force Microphone Mute}
  # !BSF HELP:{HD Audio Microphone Privacy Mode: 0: No Microphone Privacy Support; 1: HW Managed Microphone Privacy; 2: FW Managed Microphone Privacy; 3: Force Microphone Mute}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyMode        | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy Deglitch} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy Deglitch: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyDeglitch    | * | 0x01 | 0x01

  # !BSF NAME:{HD Audio Microphone Privacy applied for SoundWire Link number 0 in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for SoundWire Link number 0 in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeSoundWire0    | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy applied for SoundWire Link number 1 in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for SoundWire Link number 1 in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeSoundWire1    | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy applied for SoundWire Link number 2 in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for SoundWire Link number 2 in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeSoundWire2    | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy applied for SoundWire Link number 3 in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for SoundWire Link number 3 in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeSoundWire3    | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy applied for SoundWire Link number 4 in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for SoundWire Link number 4 in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeSoundWire4    | * | 0x01 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy applied for Dmic in HW Mode} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{HD Audio Microphone Privacy applied for Dmic in HW Mode: 0: Disable, 1: Enable}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyHwModeDmic    | * | 0x01 | 0x00
  # Added reserved space    UnusedUpdSpace31
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd31                 | * | 0x1 | 0x00

  # !BSF NAME:{HD Audio Microphone Privacy Timeout. Indicates the time-out duration to wait before forcing the actual microphone privacy DMA data zeroing.} TYPE:{EditNum, HEX, (0x0,0xFFFFFFFF)}
  # !BSF HELP:{HD Audio Microphone Privacy Timeout. Indicates the time-out duration to wait before forcing the actual microphone privacy DMA data zeroing.}
  gPlatformFspPkgTokenSpaceGuid.PchHdaMicPrivacyTimeout    | * | 0x04 | 0x00000064

  # Reserved for alignment checking and it can be used when new item is added.
  gPlatformFspPkgTokenSpaceGuid.PchHdaRsvd    | * | 0x05 | { 0x00, 0x00, 0x00, 000 }
  # Added reserved space    UnusedUpdSpace32[3]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd32               | * | 0x3 | {0x00}

  ## HDAUDIO_CONFIG End

  ## PCH_HSIO_CONFIG Start

  # !BSF NAME:{Pointer to ChipsetInit Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{ChipsetInit Binary Pointer.}
  gPlatformFspPkgTokenSpaceGuid.ChipsetInitBinPtr           | * | 0x08 | 0x00000000

  # !BSF NAME:{Length of ChipsetInit Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{ChipsetInit Binary Length.}
  gPlatformFspPkgTokenSpaceGuid.ChipsetInitBinLen           | * | 0x04 | 0x00000000
  # Added reserved space    UnusedUpdSpace33[4]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd33               | * | 0x4 | {0x00}

  # !BSF NAME:{Pointer to NPHY Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Nphy Binary Pointer.}
  gPlatformFspPkgTokenSpaceGuid.NphyBinPtr                  | * | 0x08 | 0x00000000

  # !BSF NAME:{Length of NPHY Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Nphy Binary Length.}
  gPlatformFspPkgTokenSpaceGuid.NphyBinLen                  | * | 0x04 | 0x00000000
  # Added reserved space    UnusedUpdSpace34[4]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd34               | * | 0x4 | {0x00}

  # !BSF NAME:{Pointer to SYNPS PHY Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Synps Binary Pointer.}
  gPlatformFspPkgTokenSpaceGuid.SynpsPhyBinPtr              | * | 0x08 | 0x00000000

  # !BSF NAME:{Length of SYNPS PHY Binary} TYPE:{EditNum, HEX, (0x00,0xFFFFFFFF)}
  # !BSF HELP:{Synps Binary Length.}
  gPlatformFspPkgTokenSpaceGuid.SynpsPhyBinLen              | * | 0x04 | 0x00000000

  # !BSF NAME:{Skip setting BIOS_DONE When Fw Update.} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{When set to TRUE and boot mode is BOOT_ON_FLASH_UPDATE,skip setting BIOS_DONE MSR at EndofPei. Note: BIOS_DONE MSR should be set in later phase before executing 3rd party code if SiSkipBiosDoneWhenFwUpdate set to TRUE.}
  gPlatformFspPkgTokenSpaceGuid.SiSkipBiosDoneWhenFwUpdate  | * | 0x01 | 0x00

  ## PCH_HSIO_CONFIG End

  # !BSF NAME:{PMC WDT enable} TYPE:{Combo} OPTION:{$EN_DIS}
  # !BSF HELP:{Enable/disable PMC WDT configuration}
  gPlatformFspPkgTokenSpaceGuid.PmcWdtTimerEn               | * | 0x1  | 0x01
  # Added reserved space    UnusedUpdSpace35
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd35               | * | 0x1 | 0x00

  # !HDR EMBED:{FSP_S_CONFIG:FspsConfig:END}
  gPlatformFspPkgTokenSpaceGuid.ReservedFspsUpd             | * | 0x1 | 0x00

  # Added reserved space  UnusedUpdSpace[6]
  gPlatformFspPkgTokenSpaceGuid.FspsUpdRsvd36               | * | 0x06 | {0x00}

  # Note please keep "UpdTerminator" at the end of each UPD region.
  # The tool will use this field to determine the actual end of the UPD data
  # structure.

  gPlatformFspPkgTokenSpaceGuid.UpdTerminator               | * | 0x02 | 0x55AA
