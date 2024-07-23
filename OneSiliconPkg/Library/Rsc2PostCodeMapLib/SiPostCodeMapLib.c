/** @file
  Silicon Rsc2PostCodeMap Library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#include <Base.h>
#include <Uefi.h>

#include <Pi/PiStatusCode.h>
#include <IntelRcStatusCode.h>

#define MRC_POSTCODE_MASK    0x0000FFFF

STATUS_CODE_TO_POST_CODE_MAP mSiPostCodeProgressMap[] = {
//
// CPU
//
 { INTEL_RC_STATUS_CODE_CPU_POST_MEM_ENTRY, 0x9C00 },                 // [CPU] CPU Post-Mem Entry
 { INTEL_RC_STATUS_CODE_CPU_ADD_POSTMEM_CONFIGBLOCKS_DONE, 0x9C0F },  // [CPU] Adding CpuPostMemConfigBlocks Done
 { INTEL_RC_STATUS_CODE_CPU_INIT_START_POSTMEM, 0x9C10 },             // [CPU] CpuInit Start
 { INTEL_RC_STATUS_CODE_CPU_SET_PCD_CPUFEATURE, 0x9C11 },             // [CPU] Set the Cpu Features for CpuFeaturesPei driver
 { INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_START, 0x9C12 },      // [CPU] ProgramProcessorFeature Start
 { INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_DONE,  0x9C13 },       // [CPU] ProgramProcessorFeature Done
 { INTEL_RC_STATUS_CODE_CPU_INIT_DONE,  0x9C1F },                      // [CPU] CpuInit Done
 { INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_START, 0x9C20 },           // [CPU] BiosGuardInit Start
 { INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_RESET_START,  0x9C21 },     // [CPU] BiosGuardInit Reset review
 { INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_RESET_DONE, 0x9C29 },      // [CPU] BiosGuardInit Done review
 { INTEL_RC_STATUS_CODE_CPU_BIOSGUARD_INIT_EXIT, 0x9C2F },            // [CPU] BiosGuardInit Done
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_START, 0x9C30 },      // [CPU] PowerMgmtPreResetCpl Start
 { INTEL_RC_STATUS_CODE_CPU_INITRATL_START, 0xDC31 },                  // [CPU] InitRatl Start
 { INTEL_RC_STATUS_CODE_CPU_INITRATL_DONE, 0xDC32 },                   // [CPU] InitRatl Done
 { INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_START, 0x9C33 },         // [CPU] SetBootFrequency Start
 { INTEL_RC_STATUS_CODE_CPU_SETBOOTFREQUENCY_DONE, 0x9C34 },          // [CPU] SetBootFrequency Done
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PRERESETCPL_DONE, 0x9C3F },       // [CPU] PowerMgmtPreResetCpl Done
 { INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_START, 0xDC40 },               // [CPU] ConfigureSvidVrs Start
 { INTEL_RC_STATUS_CODE_CPU_VR_UPDATE_OVEERRIDES, 0xDC41 },            // [CPU] VR: Update VR overrides
 { INTEL_RC_STATUS_CODE_CPU_GETCPU_VR_TOPOLOGY, 0xDC42 },              // [CPU] VR: Get CPU VR Topology
 { INTEL_RC_STATUS_CODE_CPU_CFG_SVIDVRS_DONE, 0xDC4E },                // [CPU] ConfigureSvidVrs Done
 { INTEL_RC_STATUS_CODE_CPU_SEND_EARLY_PM_DONE, 0x9C4F },             // [CPU] Send early PM done
 { INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_START, 0x9C60 },        // [CPU] ReloadMicrocodePatch Start
 { INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_DONE, 0x9C61 },         // [CPU] ReloadMicrocodePatch Done
 { INTEL_RC_STATUS_CODE_CPU_POST_UCODEPATCH_INIT_START, 0x9C62 },     // [CPU] ApSafePostMicrocodePatchInit Start
 { INTEL_RC_STATUS_CODE_CPU_UCODEPATCH_INIT_DONE, 0x9C6F },           // [CPU] ApSafePostMicrocodePatchInit Done
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMTINIT_START, 0x9C70 },              // [CPU] CpuPowerMgmtInit Start
 { INTEL_RC_STATUS_CODE_CPU_PPM_INIT_START,  0x9C71 },                 // [CPU] InitPpm Start
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_START,  0xDC72 },        // [CPU] InitializeCpuPowerMgmtPrivateDataHob Start
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_PVTDATAHOB_DONE,  0xDC73 },         // [CPU] InitializeCpuPowerMgmtPrivateDataHob Done
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_START,  0x9C74 },          // [CPU] InitializeCpuPowerMgmtDataHob Start
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_DATAHOB_DONE, 0x9C75 },           // [CPU] InitializeCpuPowerMgmtDataHob Done
 { INTEL_RC_STATUS_CODE_CPU_PPM_INIT_DONE, 0x9C76 },                  // [CPU] InitPpm Done
 { INTEL_RC_STATUS_CODE_CPU_PWRMGMT_ADDCFGBLKS, 0x9C7D },             // [CPU] CpuPowerManagementAddConfigBlocks Done
 { INTEL_RC_STATUS_CODE_CPU_POST_MEM_EXIT, 0x9C7F },                  // [CPU] CPU Post-Mem Exit
 { INTEL_RC_STATUS_CODE_CPU_PRE_MEM_ENTRY, 0xDC00 },                   // [CPU] CPU Pre-Mem Entry
 { INTEL_RC_STATUS_CODE_CPU_ADD_PREMEM_CONFIGBLOCKS_DONE, 0xDC0F },    // [CPU] CpuAddPreMemConfigBlocks Done
 { INTEL_RC_STATUS_CODE_CPU_INIT_START_PREMEM, 0xDC10 },               // [CPU] CpuOnPolicyInstalled Start
 { INTEL_RC_STATUS_CODE_CPU_XMM_INIT_START, 0xDC11 },                  // [CPU] XmmInit Start
 { INTEL_RC_STATUS_CODE_CPU_CPUSTRAPS_INIT_PREMEM, 0xDC12 },           // [CPU] Init CPU Straps
 { INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_START, 0xDC13 }, // [CPU] SetCpuStrapAndEarlyPowerOnConfig Start
 { INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_RESET, 0xDC14 }, // [CPU] SetCpuStrapAndEarlyPowerOnConfig Reset
 { INTEL_RC_STATUS_CODE_CPU_SET_CPUSTRAP_AND_EARLYPOWERONCONFIG_DONE, 0xDC15 },  // [CPU] SetCpuStrapAndEarlyPowerOnConfig Done
 { INTEL_RC_STATUS_CODE_CPU_DEBUG_MSRSET, 0xDC16 },                    // [CPU] Invoking LockEnableDisableCpuDebug
 { INTEL_RC_STATUS_CODE_CPU_INIT_DONE_PREMEM, 0xDC1F },                // [CPU] CPU Pre-Mem Exit
 { INTEL_RC_STATUS_CODE_CPU_OCINIT_START_PREMEM, 0xDC20 },             // [CPU] (OC) CpuOcInitPreMem Start
 { INTEL_RC_STATUS_CODE_CPU_OCINIT_SET_RESET_FLAG, 0xDC21 },           // [CPU] CpuOcInitPreMem Reset Flag Set
 { INTEL_RC_STATUS_CODE_CPU_OCINIT_END_PREMEM, 0xDC2F },               // [CPU] (OC) CpuOcInitPreMem End
//
// SA
//
 { INTEL_RC_STATUS_CODE_SA_DEVICE_CONFIG_START, 0x9A01 },             // [SA] DeviceConfigure Start
 { INTEL_RC_STATUS_CODE_SA_PEI_DISPLAY_INIT, 0x9A03 },                // [SA] Initializing Pei Display
 { INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_ENTRY, 0x9A04 },          // [SA] PeiGraphicsNotifyCallback Entry
 { INTEL_RC_STATUS_CODE_SA_CALLPPI_AND_FILLFRAMEBUFFER, 0x9A05 },     // [SA] CallPpiAndFillFrameBuffer
 { INTEL_RC_STATUS_CODE_SA_IGPU_PPI_INIT_START, 0x9A06 },              // [SA] GraphicsPpiInit
 { INTEL_RC_STATUS_CODE_SA_GRPAHICS_PPIGETMODE_START, 0x9A07 },       // [SA] GraphicsPpiGetMode
 { INTEL_RC_STATUS_CODE_SA_DISPLAY_LOGO, 0x9A08 },                    // [SA] FillFrameBufferAndShowLogo
 { INTEL_RC_STATUS_CODE_SA_PEI_IGPU_NTFYCBK_EXIT, 0x9A0F },           // [SA] PeiGraphicsNotifyCallback Exit
 { INTEL_RC_STATUS_CODE_SA_STREAMTRACER_INIT, 0x9A13},                // [SA] Initializing Streamtracer
 { INTEL_RC_STATUS_CODE_SA_IPUINIT_BEGIN, 0x9A14 },                   // [SA] Initializing SA IPU device
 { INTEL_RC_STATUS_CODE_SA_NPUINIT_BEGIN, 0x9A15 },                   // [SA] Initializing NPU device
 { INTEL_RC_STATUS_CODE_SA_TCSS_INIT, 0x9A31 },                       // [SA] Initializing TCSS
 { INTEL_RC_STATUS_CODE_SA_PAVP_INIT, 0x9A32 },                       // [SA] Initializing VMD
 { INTEL_RC_STATUS_CODE_SA_VMD_INIT, 0x9A33 },                        // [SA] Initializing Pavp
 { INTEL_RC_STATUS_CODE_SA_CPUCRASHLOG_DISABLE, 0x9A33 },             // [SA] Disable CpuCrashLog
 { INTEL_RC_STATUS_CODE_SA_SET_BIOS_RESET_CPL, 0x9A61 },              // [SA] Set BIOS_RESET_CPL to indicate all configurations complete
 { INTEL_RC_STATUS_CODE_SA_IGPU_PM_INIT_START, 0x9A63 },              // [SA] IGpuPmInit Start
 { INTEL_RC_STATUS_CODE_SA_SAPCIPRINT_START, 0x9A64 },                // [SA] SaPciPrint Start
 { INTEL_RC_STATUS_CODE_SA_LOCK_PAM_ENDOFPEI, 0x9A70 },               // [SA] SaS3ResumeAtEndOfPei Callback Entry
 { INTEL_RC_STATUS_CODE_SA_S3_RESUME_CALLBACK_EXIT, 0x9A7F },               // [SA] SaS3ResumeAtEndOfPei Callback Exit
 { INTEL_RC_STATUS_CODE_SA_PROGRAM_SA_BARS, 0xDA06 },                 // [SA] Programming SA Bars
 { INTEL_RC_STATUS_CODE_SA_INSTALL_SA_HOBS, 0xDA08 },                 // [SA] Install SA HOBs
 { INTEL_RC_STATUS_CODE_SA_TCSS_INIT_PREMEM, 0xDA09 },                // [SA] TCSS Init
 { INTEL_RC_STATUS_CODE_SA_DISPLAY_INIT_PREMEM, 0xDA0B },             // [SA] Initializing Pre-Memory Display
 { INTEL_RC_STATUS_CODE_SA_CHK_OFFBOARD_PCIEVGA, 0xDA42 },            // [SA] CheckOffboardPcieVga
 { INTEL_RC_STATUS_CODE_SA_GFX_INIT, 0xDA50 },                        // [SA] Initializing Graphics
 { INTEL_RC_STATUS_CODE_SA_VTD_INIT, 0xDA51 },                        // [SA] Initializing Vtd
//
// PCH/PCD
//
 { INTEL_RC_STATUS_CODE_PCH_SMM_RESOURCE_ACCESS_VIOLATION, 0x0B23 }, // [PCH] Represent SMM Resource Access Violation
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_START,         0x9B00 }, // [PCH] Post-Mem PchInit Entry
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_ACE,           0x9B01 }, // [PCH] Post-Mem Ace configuration start
 { PC_INST_PCD | PC_ENDOFPEI_CTRL_RESET_ACE,        0x9B02 }, // [PCH] Post-Mem Ace controller reset
 { PC_INST_PCD | PC_ENDOFPEI_INIT_LINKS_ACE,        0x9B03 }, // [PCH] Post-Mem Ace configure audio links
 { PC_INST_PCD | PC_ENDOFPEI_INIT_HDA_CODECS,       0x9B04 }, // [PCH] Post-Mem Ace Hda codecs initialization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_USB_HOST,      0x9B07 }, // [PCH] Post-Mem Configures PCH USB Host controller (xHCI)
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_USB_DEVICE,    0x9B08 }, // [PCH] Post-Mem Configures PCH USB device (xDCI)
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_P2SB,          0x9B0B }, // [PCH] Post-Mem Initialize P2SB controller
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_ITSS,          0x9B0C }, // [PCH] Post-Mem IOAPIC initialization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_CNVI,          0x9B0D }, // [PCH] Post-Mem PCH devices interrupt configuration
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_ISH,           0x9B14 }, // [PCH] Post-Mem ISH initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SMBUS,         0x9B15 }, // [PCH] Post-Mem Configure SMBUS power management review prefix
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_GBE,           0x9B05 }, // [PCH] Post-Mem GBE initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_PCIE_RP,       0x9B06 }, // [PCH] Post-Mem PCIE Root Port initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_PSTH,          0x9B09 }, // [PCH] Post-Mem PSTH initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SMBUS,         0x9B0A }, // [PCH] Post-Mem SMBUS initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_THC,           0x9B0E }, // [PCH] Post-Mem THC initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SCS,           0x9B10 }, // [PCH] Post-Mem SCS initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_GPIO,          0x9B11 }, // [PCH] Post-Mem GPIO initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_RTC,           0x9B12 }, // [PCH] Post-Mem RTC initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_LPSS,          0x9B13 }, // [PCH] Post-Mem LPSS initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SPI,           0x9B16 }, // [PCH] Post-Mem SPI initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_ESPI,          0x9B17 }, // [PCH] Post-Mem eSPI initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_IEH,           0x9B18 }, // [PCH] Post-Mem IEH initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_PMC,           0x9B19 }, // [PCH] Post-Mem PMC initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_CHIPSETINIT,   0x9B1A }, // [PCH] Post-Mem ChipsetInit
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SSE,           0x9B1B }, // [PCH] Post-Mem SSE initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_SATA,          0x9B1C }, // [PCH] Post-Mem SATA initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_DMI,           0x9B1D }, // [PCH] Post-Mem DMI initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_EAH,           0x9B1E }, // [PCH] Post-Mem EAH initizalization
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_THERMAL,       0x9B1F }, // [PCH] Post-Mem Initialize Thermal controller
 { PC_INST_PCD | PC_ENDOFPEI_START,                 0x9B40 }, // [PCH] Post-Mem OnEndOfPEI Entry
 { PC_INST_PCD | PC_ENDOFPEI_INIT_THERMAL,          0x9B41 }, // [PCH] Post-Mem OnEndOfPEI Initialize Thermal controller
 { PC_INST_PCD | PC_ENDOFPEI_INIT_ACE,              0x9B42 }, // [PCH] Post-Mem OnEndOfPEI ACE
 { PC_INST_PCD | PC_ENDOFPEI_DONE,                  0x9B47 }, // [PCH] Post-Mem OnEndOfPEI Exit
 { PC_INST_PCD | PC_PEI_POSTMEM_INIT_DONE,          0x9B7F }, // [PCH] Post-Mem PchInit Exit
 { PC_INST_PCD | PC_PEI_PREPOLICY_START,            0xDB00 }, // [PCH] Pre-Mem PchInit Entry
 { PC_INST_PCD | PC_PEI_PREPOLICY_INIT_SERVICE_SPI, 0xDB02 }, // [PCH] Pre-Mem Disable PCH fused controllers
 { PC_INST_PCD | PC_PEI_PREMEM_INIT_SMBUS,          0xDB15 }, // [PCH] Pre-Mem SMBUS configuration review prefix
 { PC_INST_PCD | PC_PEI_PREMEM_START,               0xDB48 }, // [PCH] Pre-Mem PchOnPolicyInstalled Entry
 { PC_INST_PCD | PC_PEI_PREMEM_INIT_DCI,            0xDB4A }, // [PCH] Pre-Mem DCI configuration
 { PC_INST_PCD | PC_PEI_PREMEM_RESET_DCI,           0xDB4B }, // [PCH] Pre-Mem DCI configuration - Trigger Global reset for opt-in debug state change
 { PC_INST_PCD | PC_PEI_PREMEM_INIT_TRACEHUB,       0xDB4D }, // [PCH] Pre-Mem Trace Hub - Early configuration
 { PC_INST_PCD | PC_PEI_PREMEM_DISABLE_TRACEHUB,    0xDB50 }, // [PCH] Pre-Mem Trace Hub - Power gating configuration
 { PC_INST_PCD | PC_PEI_PREMEM_ENABLE_TRACEHUB,     0xDB51 }, // [PCH] Pre-Mem Trace Hub - Power gating Trace Hub device and locking HSWPGCR1 register
 { PC_INST_PCD | PC_PEI_PREMEM_INIT_HPET,           0xDB52 }, // [PCH] Pre-Mem Initialize HPET timer
 { PC_INST_PCD | PC_PEI_PREMEM_DONE,                0xDB55 }, // [PCH] Pre-Mem PchOnPolicyInstalled Exit
 { PC_INST_PCD | PC_PEI_PREMEM_INIT_CNVI,           0xDB56 }, // [PCH] Pre-Mem Cnvi Init
 { PC_INST_PCD | PC_PEI_PREMEM_DONE,                0xDB7F }, // [PCH] Pre-Mem PchInit Exit

//
// ME
//
 { INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_ENTRY, 0xDE03 },          // [ME] ME-BIOS: HECI Control PPI Entry
 { INTEL_RC_STATUS_CODE_ME_HECI_INIT_START, 0x0E04 },                 // [ME] ME-BIOS: HECI Protocol Entry
 { INTEL_RC_STATUS_CODE_ME_EOP_ENTRY_START, 0x0E05 },                 // [ME] ME-BIOS: ME-BIOS: EOP Entry
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_ENTRY_START, 0x0E06 },            // [ME] ME-BIOS: Global Reset Request Entry
 { INTEL_RC_STATUS_CODE_ME_MEBX_ENTRY, 0x0E07 },                      // [ME] ME-BIOS: MEBx Entry
 { INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_START, 0xDE08 },               // [ME] ME-BIOS: Get IMR Size Entry
 { INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ENTRY, 0x0E09 },           // [ME] ME-BIOS: AsfGetBootOptions Entry
 { INTEL_RC_STATUS_CODE_ME_MEDEV_INIT_ENTRY, 0x9E0A },                // [ME] ME-BIOS: ME Device Init Entry
 { INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_SUCCESS, 0xDE23 },   // [ME] ME-BIOS: HECI Control PPI Exit - Success.
 { INTEL_RC_STATUS_CODE_ME_HECI_INIT_EXIT, 0x0E24 },                  // [ME] ME-BIOS: HECI Protocol Exit - Success
 { INTEL_RC_STATUS_CODE_ME_EOP_EXIT_SUCCESS, 0x0E25 },                // [ME] ME-BIOS: EOP Exit - Success
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_SUCCESS, 0x0E26 },           // [ME] ME-BIOS: Global Reset Request Exit - Success
 { INTEL_RC_STATUS_CODE_ME_MEBX_EXIT, 0x0E27 },                       // [ME] ME-BIOS: MEBx Exit - Success
 { INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_SUCCESS, 0x0E28 },        // [ME] ME-BIOS: Get IMR Size Exit - Success
 { INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_EXIT, 0x0E29 },            // [ME] ME-BIOS: AsfGetBootOptions Exit - Success
 { INTEL_RC_STATUS_CODE_ME_DEV_INIT_EXIT_SUCCESS, 0x0E2A },           // [ME] ME-BIOS: ME Device Init Exit - Success
 { INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_GETMBPMSGFAIL, 0x9E82 },      // [ME] ME-BIOS: MBP Exit - Error by PrepareMeBiosPayload fail
 { INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_LACKOFMEM, 0x0E84 },         // [ME] ME-BIOS: HECI Protocol Exit - Error by lack of memory for HECI_INSTANCE
 { INTEL_RC_STATUS_CODE_ME_EOP_EXIT_ERR_EOPMSGSNDFAIL, 0x0E85 },      // [ME] ME-BIOS: EOP Exit - Error by sending EOP message. Forcing HECI interface closure
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSGNA, 0x0E86 },         // [ME] ME-BIOS: Global Reset Request Exit - Error by message is not allowed
 { INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NOHECI, 0xDE88 },     // [ME] ME-BIOS: Get IMR Size Exit - Error by HECI service not found
 { INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_HECI_NOSRVC, 0x0E89 }, // [ME] ME-BIOS: AsfGetBootOptions Exit - Error by HECI service not found
 { INTEL_RC_STATUS_CODE_ME_MEDEV_EXIT_ERR_INVALIDPARAM, 0x0E8A },     // [ME] ME-BIOS: ME Device Init Exit - Error by Invalid Parameter
 { INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_INITFAIL, 0x0EA4 },          // [ME] ME-BIOS: HECI Protocol Exit - Error by HeciInitialize fail
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_NOHECI, 0x0EA6 },        // [ME] ME-BIOS: Global Reset Request Exit - Error by HECI service not found
 { INTEL_RC_STATUS_CODE_ME_HECI_GETIMR_EXIT_ERR_NORESPONSE, 0xDEA8 }, // [ME] ME-BIOS: Get IMR Size Exit - Error by by no response
 { INTEL_RC_STATUS_CODE_ME_HECI_EXIT_ERR_INSTALPROTOFAIL, 0x0EC4 },   // [ME] ME-BIOS: HECI Protocol Exit - Error by install HECI protocol fail
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_SENTFAIL, 0x0EC6 },  // [ME] ME-BIOS: Global Reset Request Exit - Error by message sent fail
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_SENTFAIL, 0x0EC6 },  // [ME] ME-BIOS: Global Reset Request Exit - Error by message sent fail
 { INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_CLRBOOTOPT, 0x0EC9 },  // [ME] ME-BIOS: AsfGetBootOptions Exit - Error by ClearBootOptions
 { INTEL_RC_STATUS_CODE_ME_RESETREQ_EXIT_ERR_MSG_ACKERR, 0x0EE6 },    // [ME] ME-BIOS: Global Reset Request Exit - Error by message ack error
 { INTEL_RC_STATUS_CODE_RESETREQ_EXIT_ERR_MSG_ACKERR, 0x0EE6 },       // [ME] ME-BIOS: Global Reset Request Exit - Error by message ack error
 { INTEL_RC_STATUS_CODE_ME_ASF_GETBOOTOPTN_ERR_HECI_ACSFAIL, 0x0EE9 },// [ME] ME-BIOS: AsfGetBootOptions Exit - Error by HECI access fail
 { INTEL_RC_STATUS_CODE_ME_MBP_ENTRY_START, 0x9E02 },                 // [ME] ME-BIOS: MBP Entry
 { INTEL_RC_STATUS_CODE_ME_MBP_EXIT_SUCCESS, 0x9E22 },                // [ME] ME-BIOS: MBP Exit - Success
 { INTEL_RC_STATUS_CODE_ME_MBP_EXIT_ERR_PAYLOAD_FAIL, 0x9EA2 },       // [ME] ME-BIOS: MBP Exit - Error by MeBiosPayloadHob creation has failed
 { INTEL_RC_STATUS_CODE_ME_DID_ENTRY_START, 0xDE01 },                 // [ME] ME-BIOS: DID Entry
 { INTEL_RC_STATUS_CODE_ME_DID_EXIT_SUCCESS, 0xDE21 },                // [ME] ME-BIOS: DID Exit - Success
 { INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_SIPREMEMPPIFAIL, 0xDE81 },    // [ME] ME-BIOS: DID Exit - Error by SiPreMemPolicyPpi not found
 { INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_HECIDEV, 0xDE83 },       // [ME] ME-BIOS: HECI Control PPI Exit - Error by HECI device error
 { INTEL_RC_STATUS_CODE_ME_DID_EXIT_ERR_DIDMSGSENDFAIL, 0xDEA1 },     // [ME] // [ME] ME-BIOS: DID Exit - Error by fail to send DID message
 { INTEL_RC_STATUS_CODE_ME_HECI_CONTROL_PPI_EXIT_ERR_INSTALLPPIFAIL, 0xDEA3 },// [ME] ME-BIOS: HECI Control PPI Exit - Error by install HeciControlPpi fail
//
// FSP
//
 { INTEL_RC_STATUS_CODE_FSP_SILICON_INIT_API_ENTRY, 0x9800 },           // [FSP] SiliconInt Api Entry
 { INTEL_RC_STATUS_CODE_FSP_MEM_TEMPRAMEXIT_API_ENTRY, 0xB800 },        // [FSP] TempRamExit Api Entry
 { INTEL_RC_STATUS_CODE_FSP_MEM_TEMPRAMEXIT_API_EXIT, 0xB87F },         // [FSP] TempRamExit Api Exit
 { INTEL_RC_STATUS_CODE_FSP_MEM_INIT_API_ENTRY, 0xD800 },               // [FSP] Memory Init Api Entry
 { INTEL_RC_STATUS_CODE_FSP_MEM_INIT_API_EXIT, 0xD87F },                // [FSP] MemoryInit API Exit
//
// Security
//
 { INTEL_RC_STATUS_CODE_TXT_ACM_ENTRY, 0x9901 },  // [Security] ACM Entry
 { INTEL_RC_STATUS_CODE_TXT_ACM_EXIT, 0x9902 },   // [Security] ACM Exit
 { INTEL_RC_STATUS_CODE_TDX_ACM_ENTRY, 0xD903 },  // [Security] ACM Entry
 { INTEL_RC_STATUS_CODE_TDX_ACM_EXIT, 0xD904 },   // [Security] ACM Exit
 {0,0}
 };

//
// MRC Rsc2PostCode Table
//
STATUS_CODE_TO_POST_CODE_MAP mMrcPostCodeProgressMap[] = {
//
// MRC.
// If PostCode Value corresponding to INTEL_RC_STATUS_CODE_MRC_POST_CODE value is not present in the table, the Last 4 Nibbles of the RSC value will be sent to PostCode Interfaces.
//
// Ex: { REPORT_STATUS_CODE_VALUE, 0xABCD },                           // [MRC] New
 {0,0}
 };

STATUS_CODE_TO_POST_CODE_MAP *mSiPostCodeRscMap[] = {
  mSiPostCodeProgressMap,
  mMrcPostCodeProgressMap
};

/**
  Find the post code data from status code value.

  @param  Map              The map used to find in.
  @param  Value            The status code value.

  @return PostCode         0 for not found.

**/
UINT32
FindRscPostCodeData (
  IN STATUS_CODE_TO_POST_CODE_MAP *Map,
  IN EFI_STATUS_CODE_VALUE   Value
  )
{
  while (Map->Value != 0) {
    if (Map->Value == Value) {
      return Map->Data;
    }
    Map++;
  }
  return 0;     // If value = 0 => No corresponding PostCode value exists for the RSC value.
}

/**
  Get MRC Reference Code PostCode from status code value.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
GetMrcRscPostCodeData (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  UINT32               PostCodeValue=0;

  // Check if PostCode value is available in the MRC Map table.
  PostCodeValue = FindRscPostCodeData (mSiPostCodeRscMap[1], Value);
  // If value not present in the table, return the Last 4 Nibbles.
  if (PostCodeValue == 0) {
    PostCodeValue = Value & MRC_POSTCODE_MASK;
  }
  return PostCodeValue;
}

/**
  Get Silicon Reference Code PostCode from status code value.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
GetSiRscPostCodeData (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  return FindRscPostCodeData (mSiPostCodeRscMap[0], Value);
}