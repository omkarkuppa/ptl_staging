/** @file
  CPU feature control module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuInitPostMem.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MeInitLib.h>
#include <Library/MsrFruLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PmcLib.h>
#include <Library/PmcPrivateLib.h>
#include <IntelRcStatusCode.h>
#include <Library/RegisterCpuFeaturesLib.h>
#include <Library/CpuInitPreMem.h>
#include <Library/CpuMailboxLib.h>
#include <Register/Msr.h>
#include <Register/GenerationMsr.h>
#include <Library/MicrocodeLib.h>
#include <Guid/MicrocodePatchHob.h>
#include <Ppi/MpServices2.h>
#include <Library/PcdLib.h>
#include <Library/PeiPmcPolicyCommonLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/LocalApicLib.h>
#include <Library/PeiCpuInitFruLib.h>
#include <Library/SmmRelocationLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TdxDataHob.h>
#include <Library/TdxInfoLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/TmeLib.h>
#include <Library/TmeInfoLib.h>
#include <Library/PeiVtdInitFruLib.h>

  //
  //  MCHECK_FLOW_STATUS MMIO register
  //
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32 Mcheck_Skipped_OR_Done   : 1;   ///< Bits[0]
    UINT32 Reserved                 : 31;  ///< Bits[31:1]
  } Fields;
  UINT32 Data;
} MCHECK_FLOW_STATUS_MMIO_REGISTER;

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED EDKII_PEI_MP_SERVICES2_PPI   *mMpServices2Ppi            = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED SI_POLICY_PPI                *mSiPolicyPpi               = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED CPU_TEST_CONFIG              *mCpuTestConfig             = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED CPU_INIT_CONFIG              *mCpuInitConfig             = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED CPU_INIT_PREMEM_CONFIG       *mCpuInitPreMemConfig       = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED VTD_CONFIG                   *mVtdConfig                 = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED SI_PREMEM_CONFIG             *mSiPreMemConfig            = NULL;

//
// MCHECK BIOS Parameter Structure per MCHECK 0.5.002 CORE SAS
//
#define MCHECK_BIOS_PARAM_INFO_UUID    {0x13, 0x43, 0xF1, 0x5C, 0xAA, 0x00, 0x42, 0x07, 0xA2, 0x14, 0xA8, 0x4F, 0x35, 0x58, 0x22, 0xA2}
#define MCHECK_STRUCTURE_VERSION       5
#define KEYLOCKER_ENABLE               BIT3
#define SEAM_ENABLE                    BIT1
#define MCHECK_MAX_PACKAGES            8

typedef union {
  UINT8               Uuid[16];
  UINT64              Uuid64[2];
} MCHECK_UUID;

typedef struct {
  MCHECK_UUID         Uuid;
  UINT16              Size;
  UINT16              Version;
  UINT8               Reserved[12];
} MCHECK_HEADER;

typedef struct {
  VOID   *Address;
  UINT64 Size;
} MCHECK_BUFFER; // Total: 16 bytes

typedef struct {
  MCHECK_BUFFER Cmr; // CMR_LIST
  MCHECK_BUFFER TioSocketInfo;
} MCHECK_SEAM_INFO; // Total 32 bytes

typedef struct {
    UINT8         NumSockets;                            // Number of currently populated sockets
    UINT8         Master;                                // Node ID of Master socket
    UINT8         Alg;                                   // Algorithm for PRMRR key
    UINT8         Rfu;                                   // RFU, must be 0
    UINT8         Flags[4];
    UINT64        PlatformConfig[2];                     // RFU, must be 0
    UINT8         NodeIds[MCHECK_MAX_PACKAGES];          // Node IDs (1-byte each).
    MCHECK_BUFFER ActmDimmManifest[MCHECK_MAX_PACKAGES]; // Array of node-specific DIMM manifest structures
    MCHECK_BUFFER WorkingArea;                           // WorkingArea
} MCHECK_TOPOLOGY_INFO; // Total: 176 bytes

typedef struct {
  MCHECK_HEADER       Header;
  UINT64              Features;
  MCHECK_BUFFER       SgxInfo;
  MCHECK_BUFFER       SeamInfo;
  MCHECK_BUFFER       SafInfo;
  MCHECK_BUFFER       TopologyInfo;
} MCHECK_BIOS_PARAM_INFO;

typedef struct {
  UINT32    ApicId;
  UINT64    PmLogicId;
  //
  // To add more
  //
} DUMP_INFO_PER_CORE;

///
/// Values for Trace Hub Acpi Base Address
///
UINT64   mTraceHubAcpiBaseAddress = 0;
BOOLEAN  mTraceHubFeatureEnable   = FALSE;

UINT8    mSmallCoreCount          = 0;
UINT64   mPrmrrBase;
UINT64   mPrmrrMask;
UINT64   mSeamrrBase;
UINT64   mSeamrrMask;

STATIC EFI_PEI_PPI_DESCRIPTOR   mPeiCpuFeaturesSetDonePpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiCpuFeaturesSetDoneGuid,
  NULL
};

/**
  Set up flags in CR4 for XMM instruction enabling
**/
VOID
XmmInit (
  VOID
  );

//
// Setting for AP init.
//
BOOLEAN  mPmTimerEmulationEnable  = FALSE;

/**
  This function is to check if CPU support XSAVE or not.
  @param[in]

  @retval TRUE    XSAVE is supported.
  @retval FALSE   XSAVE is unsupported.
**/
BOOLEAN
IsCpuSupportXsave (
  VOID
  )
{
  CPUID_VERSION_INFO_ECX  Ecx;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);

  ///
  /// Check if cpu supported XSAVE.
  ///
  return (BOOLEAN) (Ecx.Bits.XSAVE == 1);
}

/**
  This function is to update BIOS PARAM INFO with SEAM INFO and TOPOLOGY INFO
  @param[in] BiosParamInfoPtr   A pointer to the MCHECK_BIOS_PARAM_INFO

  @retval VOID                  No value to return
**/

VOID
EFIAPI
UpdateBiosParamInfo (
  IN MCHECK_BIOS_PARAM_INFO *BiosParamInfoPtr
 )
{
  MCHECK_SEAM_INFO          *SeamInfoPtr = NULL;
  MCHECK_TOPOLOGY_INFO      *TopologyInfoPtr = NULL;
  TDX_DATA_HOB              *TdxDataHob;
  UINT32                    Index;
  UINT32                    MaxCmrEntries = 0;

  // Check if Tdx is enabled
  if (IsTdxEnabled ()) {

    TdxDataHob = (TDX_DATA_HOB *) GetFirstGuidHob (&gTdxDataHobGuid);
    if (TdxDataHob == NULL) {
      DEBUG ((DEBUG_ERROR, "Error while locating TDX Data HOB!\n"));
      return;
    }

    // Allocate the zero pool for SEAM_INFO Structure
    SeamInfoPtr = (MCHECK_SEAM_INFO *) AllocateZeroPool (sizeof (MCHECK_SEAM_INFO));
    if (SeamInfoPtr == NULL) {
      DEBUG ((DEBUG_INFO, "Error: Cannot allocate memory for SeamInfoPtr\n"));
      ASSERT (FALSE);
      return;
    }

    // Allocate the zero pool for TOPOLOGY_INFO Structure
    TopologyInfoPtr = (MCHECK_TOPOLOGY_INFO *) AllocateZeroPool (sizeof (MCHECK_TOPOLOGY_INFO));
    if (TopologyInfoPtr == NULL) {
      DEBUG ((DEBUG_INFO, "Error: Cannot allocate memory for TopologyInfoPtr\n"));
      ASSERT (FALSE);

      if (SeamInfoPtr) {
        FreePool (SeamInfoPtr);
      }

      return;
    }

    // Update the Seam Info
    BiosParamInfoPtr->SeamInfo.Address = SeamInfoPtr;
    BiosParamInfoPtr->SeamInfo.Size = sizeof(MCHECK_SEAM_INFO);

    SeamInfoPtr->Cmr.Address = TdxDataHob->CmrList.Cmr;
    SeamInfoPtr->Cmr.Size = (TdxDataHob->CmrList.NumCmrArrayEntries) * sizeof(TdxDataHob->CmrList.Cmr[0]);
    MaxCmrEntries = TdxDataHob->CmrList.NumCmrArrayEntries;

    if ((MaxCmrEntries == 0) || (MaxCmrEntries > MCHECK_MAX_CMR_ENTRIES)) {
      DEBUG ((DEBUG_INFO, "MaxCmrEntries is not in boundaries! %d\n", MaxCmrEntries));

      if (SeamInfoPtr) {
        FreePool (SeamInfoPtr);
      }

      if (TopologyInfoPtr) {
        FreePool (SeamInfoPtr);
      }
    }

    SeamInfoPtr->TioSocketInfo.Address = NULL;
    SeamInfoPtr->TioSocketInfo.Size = 0;

    // Update the Topology info
    BiosParamInfoPtr->TopologyInfo.Address = TopologyInfoPtr;
    BiosParamInfoPtr->TopologyInfo.Size = sizeof(MCHECK_TOPOLOGY_INFO);

    TopologyInfoPtr->NumSockets = 1;
    TopologyInfoPtr->Master = 0;
    TopologyInfoPtr->Alg = 0;
    TopologyInfoPtr->Rfu = 0;

    // Update MCHECK_TOPOLOGY_INFO Flags
    for (Index = 0; Index < 4; Index++) {
      TopologyInfoPtr->Flags[Index] = 0;
    }

    // Update MCHECK_TOPOLOGY_INFO PlatformConfig
    for (Index = 0; Index < 2; Index++) {
      TopologyInfoPtr->PlatformConfig[Index] = 0;
    }

    // Update MCHECK_TOPOLOGY_INFO NodeIds
    for (Index = 0; Index < MCHECK_MAX_PACKAGES; Index++) {
      TopologyInfoPtr->NodeIds[Index] = 0;
    }

    // Update MCHECK_TOPOLOGY_INFO ActmDimmManifest
    TopologyInfoPtr->ActmDimmManifest[0].Address = &(TdxDataHob->ActmDimmManifest);
    TopologyInfoPtr->ActmDimmManifest[0].Size = sizeof(TdxDataHob->ActmDimmManifest);
    for (Index = 1; Index < MCHECK_MAX_PACKAGES; Index++) {
      TopologyInfoPtr->ActmDimmManifest[Index].Address = NULL;
      TopologyInfoPtr->ActmDimmManifest[Index].Size = 0;
    }

    // Update MCHECK_TOPOLOGY_INFO WorkingArea
    TopologyInfoPtr->WorkingArea.Address = NULL;
    TopologyInfoPtr->WorkingArea.Size = 0;

    DEBUG ((DEBUG_INFO, "SeamInfo Address = 0x%016x \n", BiosParamInfoPtr->SeamInfo.Address));
    DEBUG ((DEBUG_INFO, "SeamInfo Cmr Address = 0x%016x \n", SeamInfoPtr->Cmr.Address));
    DEBUG ((DEBUG_INFO, "CMR table:\n"));
    for (Index = 0; Index < MaxCmrEntries; Index++) {
      DEBUG ((DEBUG_INFO, "CMR[%d] Base = 0x%016llx \n", Index, TdxDataHob->CmrList.Cmr[Index].CmrBase));
      DEBUG ((DEBUG_INFO, "CMR[%d] Size = 0x%016llx \n", Index, TdxDataHob->CmrList.Cmr[Index].CmrSize));
    }
    DEBUG ((DEBUG_INFO, "TopologyInfo Address = 0x%016x \n", BiosParamInfoPtr->TopologyInfo.Address));
    DEBUG ((DEBUG_INFO, "TopologyInfo Dimm Manifest Address = 0x%016x \n", TopologyInfoPtr->ActmDimmManifest[0].Address));

  }
}

/**
  Perform BIOS uCode PM_TMR Emulation Configuration by configuring MSR 121h and setting
  the ACPI Timer Disable Bit

  The uCode emulation of the ACPI Timer allows disabling of the ACPI Timer to have no
  impact on the system, with the exception that TMR_STS will not be set.  All aligned
  32-bit reads   to the ACPI Timer port are valid and will behave as if the ACPI timer
  remains enabled.

  @param[in] IsBsp - TRUE indicates the function is called from BSP.
**/
VOID
BiosUcodePmTmrEmulationMsrCfg (
  BOOLEAN                IsBsp
  )
{
  MSR_BIOS_UCODE_PM_TMR_EMULATION_CFG_REGISTER UcodePmTmrEmulation;
  UINT32 CorrectionFactor;
  UINT32 Data32;

  // EPOC {21:20,17}
  // 000 24MHz
  // 001 19.2MHz
  // 010 38.4MHz
  Data32 = PmcGetCpuEpoc ();
  if ((Data32 >> N_PMC_PWRM_CPU_EPOC_XTAL_FREQ_LSB) & 0x1) {
    CorrectionFactor = V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_19_MHZ;
  } else {
    if ((Data32 >> N_PMC_PWRM_CPU_EPOC_XTAL_FREQ_MSB) & 0x1){
      CorrectionFactor = V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_38_MHZ;
    } else {
      CorrectionFactor = V_BIOS_UCODE_PM_TMR_EMULATION_CFG_CORRECTION_FACTOR_24_MHZ;
    }
  }

  UcodePmTmrEmulation.Uint64 = 0;
  UcodePmTmrEmulation.Bits.CorrectionFactor = CorrectionFactor;
  UcodePmTmrEmulation.Bits.RspDelayLoopCount = V_BIOS_UCODE_PM_TMR_EMULATION_CFG_DELAY_VALUE;
  UcodePmTmrEmulation.Bits.Valid = 1;
  UcodePmTmrEmulation.Bits.EmulatedIoPort = PmcGetAcpiBase () + R_ACPI_IO_PM1_TMR;
  if (IsBsp) {
    DEBUG ((DEBUG_INFO, "ACPI Timer Emulation Config, writing 0x%lx to MSR 121h.\n", UcodePmTmrEmulation.Uint64));
  }
  AsmWriteMsr64 (MSR_BIOS_UCODE_PM_TMR_EMULATION_CFG, UcodePmTmrEmulation.Uint64);
}

/**
  X2APIC need VT-d support, check VT-d value to enable X2APIC mode.

  @retval 0x01  - X2APIC mode enabled.
  @retval 0x00  - X2APIC mode disabled.
**/
static
UINT8
IsX2apicModeEnabled (
  VOID
  )
{
  static UINT8 IsX2apicMode = 0xFF;
  if (IsX2apicMode != 0xFF) {
    return IsX2apicMode;
  }
  if ((mVtdConfig != NULL) && (!IsVtdDisabled(mVtdConfig)) && (mCpuInitConfig->X2ApicEnable)) {
    IsX2apicMode = 0x01;
  } else {
    IsX2apicMode = 0x00;
  }
  return IsX2apicMode;
}

/**
  Worker function to add CPU feature to User Configuration.

  @param[in]      Feature                       CPU feature index.
  @param[in, out] CpuFeaturesUserSetting        Pointer to CPU features user setting.
  @param[in]      CpuFeaturesUserSettingSize    CPU features user setting size.
**/
STATIC
VOID
AddCpuFeaturesConfiguration (
  IN UINT32              Feature,
  IN UINT8               *CpuConfiguration,
  IN UINTN               CpuConfigurationSize
  )
{
  ASSERT ((Feature >> 3) < CpuConfigurationSize);
  CpuConfiguration[Feature >> 3] |= (1 << (Feature & 0x07));
}

/**
  Function that uses DEBUG() macros to display the contents of a a CPU feature bit mask.

  @param[in]  FeatureMask  A pointer to the CPU feature bit mask.
  @param[in]  BitMaskSize  CPU feature bits mask buffer size.

**/
VOID
DumpCpuFeatureMask (
  IN UINT8               *FeatureMask,
  IN UINTN               BitMaskSize
  )
{
  UINTN                  Index;
  UINT8                  *Data8;

  Data8       = (UINT8 *) FeatureMask;
  for (Index = 0; Index < BitMaskSize; Index++) {
    DEBUG ((DEBUG_INFO, " %02x ", *Data8++));
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  Get Trace Hub Acpi Base address for BSP
**/
VOID
GetTraceHubAcpiBaseAddressForBsp (
  VOID
  )
{
  ///
  /// Get Trace Hub ACPI base address from BSP
  ///
  mTraceHubAcpiBaseAddress = AsmReadMsr64 (MSR_NPK_STH_ACPIBAR_BASE);
  DEBUG ((DEBUG_INFO, "TraceHubAcpiBaseAddress = %x\n", mTraceHubAcpiBaseAddress));

  ///
  /// Check the pass in Trace Hub ACPI base if equal zero.
  ///
  if (mTraceHubAcpiBaseAddress != 0) {
    mTraceHubFeatureEnable = TRUE;
  }
  return;
}

/**
  This function is to set MemoryControlSplitLock Disable
**/
VOID
DisableMemoryControlSplitLock (
  VOID
  )
{
  MSR_MEMORY_CONTROL_REGISTER  MsrMemoryControl;
  MsrMemoryControl.Uint64 = AsmReadMsr64 (MSR_MEMORY_CONTROL);
  MsrMemoryControl.Bits.SplitLockDisable = 1;
  AsmWriteMsr64 (MSR_MEMORY_CONTROL, MsrMemoryControl.Uint64);
}

/**
  Set PcdCpuFeaturesSetting and install the gEdkiiCpuFeaturesSetDoneGuid
  PPI which is depended by CpuFeaturesPei driver.
**/
VOID
SetPcdCpuFeaturesSetting (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT8                     *CpuConfiguration;
  UINTN                     CpuConfigurationSize;
  Status = EFI_SUCCESS;

  CpuConfiguration = (UINT8 *)PcdGetPtr (PcdCpuFeaturesSetting);
  if (CpuConfiguration == NULL) {
    DEBUG ((DEBUG_ERROR, "Get PcdCpuFeaturesSetting Fail\n "));
    ASSERT (FALSE);
    return;
  }

  CpuConfigurationSize = PcdGetSize (PcdCpuFeaturesSetting);
  DEBUG ((DEBUG_INFO, "CpuConfiguration = "));
  DumpCpuFeatureMask (CpuConfiguration, CpuConfigurationSize);
  if (mCpuTestConfig->MachineCheckEnable) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_MCE, CpuConfiguration, CpuConfigurationSize);
    AddCpuFeaturesConfiguration (CPU_FEATURE_MCA, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuInitConfig->AesEnable) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_AESNI, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuTestConfig->MonitorMwaitEnable) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_MWAIT, CpuConfiguration, CpuConfigurationSize);
  }
  if (IsX2apicModeEnabled () == 0x01) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_X2APIC, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuTestConfig->MlcStreamerPrefetcher) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_MLC_STREAMER_PREFETCHER, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuTestConfig->MlcSpatialPrefetcher) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_MLC_SPATIAL_PREFETCHER, CpuConfiguration, CpuConfigurationSize);
  }
  AddCpuFeaturesConfiguration (CPU_FEATURE_APIC_TPR_UPDATE_MESSAGE, CpuConfiguration, CpuConfigurationSize);
  if (mCpuTestConfig->ThreeStrikeCounter) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_THREE_STRIKE_COUNTER, CpuConfiguration, CpuConfigurationSize);
  }
  AddCpuFeaturesConfiguration (CPU_FEATURE_LOCK_FEATURE_CONTROL_REGISTER, CpuConfiguration, CpuConfigurationSize);
  if (mCpuInitPreMemConfig->VmxEnable) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_VMX, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuInitConfig->TxtEnable) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_SMX, CpuConfiguration, CpuConfigurationSize);
  }
  ///
  /// Identify if EndOfManufacturing (EOM).
  ///
  if ((mCpuInitConfig->PpinSupport == 1) || ((mCpuInitConfig->PpinSupport == 2) && (!IsEom ()))) {
    AddCpuFeaturesConfiguration (CPU_FEATURE_PPIN, CpuConfiguration, CpuConfigurationSize);
  }
  if (mCpuTestConfig->ProcessorTraceEnable) {
    PcdSet32S (PcdCpuProcTraceMemSize, mCpuTestConfig->ProcessorTraceMemSize);
    PcdSet8S (PcdCpuProcTraceOutputScheme, (UINT8)mCpuTestConfig->ProcessorTraceOutputScheme);
    PcdSetBoolS (PcdCpuProcTraceBspOnly, (BOOLEAN)mCpuTestConfig->ProcessorTraceBspOnly);
    PcdSetBoolS (PcdCpuProcTracePerformanceCollecting, (BOOLEAN)mCpuTestConfig->ProcessorTraceTimingPacket);
    AddCpuFeaturesConfiguration (CPU_FEATURE_PROC_TRACE, CpuConfiguration, CpuConfigurationSize);
  }
  //
  // Set CPU Configuration PCD
  //
  Status = PcdSetPtrS (
             PcdCpuFeaturesSetting,
             &CpuConfigurationSize,
             CpuConfiguration
             );
  ASSERT_EFI_ERROR (Status);
  //
  // CpuFeatures Pei driver could be dispatched after this PPI is installed.
  //
  Status = PeiServicesInstallPpi (&mPeiCpuFeaturesSetDonePpiList);
  ASSERT_EFI_ERROR (Status);
  DEBUG ((DEBUG_INFO, "InstallPpi gEdkiiCpuFeaturesSetDoneGuid : %r\n", Status));
  return;
}

/**
  This function syncs the PRMRR MSR settings to remaining cores.

  @param[in]      IsBspInt      Check to see if the executing thread is BSP
**/
VOID
SyncPrmrrMsr (
  IN  BOOLEAN IsBspInt
  )
{
  UINT32 ThreadIndex;
  if (IsBspInt) {
    //
    // Read PRMRR MSRs that were set by MRC
    //
    mPrmrrBase = AsmReadMsr64 (MSR_PRMRR_BASE_0);
    mPrmrrMask = AsmReadMsr64 (MSR_PRMRR_MASK);
    DEBUG ((DEBUG_INFO, "PRMRR MSR: %016lx, %016lx\n", mPrmrrBase, mPrmrrMask));
  }
  //
  // Sync the BSP PRMRR MSR values to other cores.
  //
  GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
  if (!IsBspInt && (ThreadIndex == 0)) {
    AsmWriteMsr64 (MSR_PRMRR_BASE_0, mPrmrrBase);
    AsmWriteMsr64 (MSR_PRMRR_MASK,   mPrmrrMask);
  }
}

/**
  This function syncs the SEAMRR MSR settings to remaining cores.

  @param[in]      IsBspInt      Check to see if the executing thread is BSP
**/
VOID
SyncSeamrrMsr (
  IN  BOOLEAN IsBspInt
  )
{
  UINT32 ThreadIndex;
  if (IsBspInt) {
    //
    // Read SEAMRR MSRs that were set by MRC
    //
    mSeamrrBase = AsmReadMsr64 (MSR_SEAMRR_BASE);
    mSeamrrMask = AsmReadMsr64 (MSR_SEAMRR_MASK);
    DEBUG ((DEBUG_INFO, "SEAMRR MSRs: %016lx, %016lx\n", mSeamrrBase, mSeamrrMask));
  }
  //
  // Sync the BSP SEAMRR MSR values to other cores.
  //
  GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
  if (!IsBspInt && (ThreadIndex == 0)) {
    AsmWriteMsr64 (MSR_SEAMRR_BASE, mSeamrrBase);
    AsmWriteMsr64 (MSR_SEAMRR_MASK, mSeamrrMask);
  }
}

/**
  Configure CORE_MKTME_ACTIVATION on all cores
**/
VOID
ConfigureTmeCoreActivation (
  VOID
  )
{
  UINT32 ThreadIndex;
  GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
  if (ThreadIndex == 0) {
    MsrConfigureTmeCoreActivate ();
  }
}

/**
  This function is to enable/disable AVX Instructions

  @param[in]  AvxDisable
**/
VOID
MsrSetAvx (
  IN BOOLEAN AvxDisable
  )
{
  MSR_BIOS_AVX_DIS_REGISTER    BiosAvxDisableMsr;

  UINT32    Index = 0;
  UINT64    Value = 0;

  if (IsCpuSupportXsave () == TRUE) {
    BiosAvxDisableMsr.Uint64 = AsmReadMsr64 (MSR_BIOS_AVX_DIS);
    //
    // Set MSR when setup setting of AvxDisable was changed
    //
    if (BiosAvxDisableMsr.Bits.AvxDis != AvxDisable) {
      if (AvxDisable == TRUE) {
        ///
        /// Set OSXSAVE bit in CR4 to enable xgetbv/xsetbv instruction
        ///
        AsmWriteCr4 (AsmReadCr4 () | BIT18);

        Value = AsmXGetBv (Index);
        ///
        /// Clear Xcr0 bit 2 for disable AVX/AVX2
        /// Clear Xcr0 bit 7,6,5 for disable AVX3
        ///
        Value &= (UINT64) ~(BIT2 | BIT5 | BIT6 | BIT7);
        AsmXSetBv (Index, Value);
        ///
        /// Clear OSXSAVE bit in CR4 to disable xgetbv/xsetbv instruction
        ///
        AsmWriteCr4 (AsmReadCr4 () & (~BIT18));

        BiosAvxDisableMsr.Bits.AvxDis = 1;
      } else {
        BiosAvxDisableMsr.Bits.AvxDis = 0;
      }
      AsmWriteMsr64 (MSR_BIOS_AVX_DIS, BiosAvxDisableMsr.Uint64);
    }
  }
}

/**
  This function is to enable/disable AVX3 Instructions

  @param[in]  Avx3Disable
**/
VOID
MsrSetAvx3 (
  IN BOOLEAN Avx3Disable
  )
{
  MSR_BIOS_AVX_DIS_REGISTER    BiosAvxDisableMsr;

  UINT32    Index = 0;
  UINT64    Value = 0;

  if (IsCpuSupportXsave () == TRUE) {
    BiosAvxDisableMsr.Uint64 = AsmReadMsr64 (MSR_BIOS_AVX_DIS);
    //
    // If all Avx was disabled, skip to set Avx3
    //
    if (BiosAvxDisableMsr.Bits.AvxDis == 0) {
      //
      // Set MSR when setup setting of Avx3Disable was changed
      //
      if (BiosAvxDisableMsr.Bits.Avx3Dis != Avx3Disable) {
        if (Avx3Disable == TRUE) {
          ///
          /// Set OSXSAVE bit in CR4 to enable xgetbv/xsetbv instruction
          ///
          AsmWriteCr4 (AsmReadCr4 () | BIT18);

          Value = AsmXGetBv (Index);
          ///
          /// Clear Xcr0 bit 7,6,5 for disable AVX3
          ///
          Value &= (UINT64) ~(BIT5 | BIT6 | BIT7);
          AsmXSetBv (Index, Value);
          ///
          /// Clear OSXSAVE bit in CR4 to disable xgetbv/xsetbv instruction
          ///
          AsmWriteCr4 (AsmReadCr4 () & (~BIT18));

          BiosAvxDisableMsr.Bits.Avx3Dis = 1;
        } else {
          BiosAvxDisableMsr.Bits.Avx3Dis = 0;
        }
        AsmWriteMsr64 (MSR_BIOS_AVX_DIS, BiosAvxDisableMsr.Uint64);
      }
    }
  }
}

/**
  Disable or enable CPU Crashlog dump feature by MSR.

  @param[in] BOOLEAN      Policy for CrashLog
**/
VOID
EnableCpuCrashLog (
  IN  BOOLEAN                 CrashLogEnable
  )
{
  MSR_CRASHLOG_CONTROL_REGISTER    CurrentValue;
  MSR_CRASHLOG_CONTROL_REGISTER    ExpectedValue;

  CurrentValue.Uint64 = AsmReadMsr64 (MSR_CRASHLOG_CONTROL);

  ExpectedValue.Uint64 = CurrentValue.Uint64;

  if (CrashLogEnable) {
    ExpectedValue.Bits.Cddis = 0;
  } else {
    ExpectedValue.Bits.Cddis = 1;
  }

  if (CurrentValue.Uint64 != ExpectedValue.Uint64) {
    //
    // MSR value does not match policy.
    // Write MSR with the expected value.
    //
    AsmWriteMsr64 (MSR_CRASHLOG_CONTROL, ExpectedValue.Uint64);
  }
}

/**
  Check if the user selected output scheme is supported or not

  @param[in] OutputSchemeTopa - TRUE:  Topa Output,
                                FALSE: Single Range Output.

  @retval TRUE  - Output scheme is supported.
  @retval FALSE - Output scheme is not supported.
**/
BOOLEAN
ProcTraceOutputSupported (
  IN BOOLEAN OutputSchemeTopa
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX  Ebx;
  CPUID_INTEL_PROCESSOR_TRACE_MAIN_LEAF_ECX    Ecx;
  BOOLEAN                                      IsTopaSupported;
  BOOLEAN                                      IsSingleRangeSupported;

  //
  // Check if Processor Trace is supported
  //
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, &Ebx.Uint32, NULL, NULL);
  if (Ebx.Bits.IntelProcessorTrace == 0) {
    return FALSE;
  }
  //
  // Check if the output scheme is supported
  //
  AsmCpuidEx (CPUID_INTEL_PROCESSOR_TRACE, CPUID_INTEL_PROCESSOR_TRACE_MAIN_LEAF, NULL, NULL, &Ecx.Uint32, NULL);
  IsTopaSupported = (Ecx.Bits.RTIT == 1) ? TRUE : FALSE;
  IsSingleRangeSupported = (Ecx.Bits.SingleRangeOutput == 1) ? TRUE : FALSE;

  if (!(OutputSchemeTopa && IsTopaSupported) &&
      !(!OutputSchemeTopa && IsSingleRangeSupported))
  {
    return FALSE;
  }
  return TRUE;
}

/**
  Assign the aligned memory

  @param[in] MemorySize        - Size of the memory needed.
  @param[in] ProcessorNumber   - Total number of processor need assign memory.

  @retval VOID * - Return allocated memory address.
**/
VOID *
GetMemoryTable (
  IN  UINT32  MemorySize,
  IN  UINTN   ProcessorNumber
  )
{
  UINTN                        *MemRegionTable;
  UINTN                        MemRegionBaseAddr;
  UINTN                        Pages;
  UINTN                        Alignment;
  UINTN                        Index;

  MemRegionTable = (UINTN *)AllocateZeroPool (ProcessorNumber * sizeof (UINTN));
  if (MemRegionTable == NULL) {
    DEBUG ((DEBUG_ERROR, "ProcTrace: Allocate ProcTrace MemRegionTable Failed\n"));
    return NULL;
  }

  for (Index = 0; Index < ProcessorNumber; Index++) {
    Pages = EFI_SIZE_TO_PAGES (MemorySize);
    if (MemorySize > 0x1000) {
      Alignment = MemorySize;
    } else {
      Alignment = 0x1000;
    }
    MemRegionBaseAddr = (UINTN)AllocateAlignedReservedPages (Pages, Alignment);
    if (MemRegionBaseAddr == 0) {
      DEBUG ((DEBUG_ERROR, "ProcTrace: Out of mem, allocated only for %d threads\n", Index));
      if (Index == 0) {
        //
        // Could not allocate for BSP even
        //
        FreePool ((VOID *)MemRegionTable);
        MemRegionTable = NULL;
        return NULL;
      }

      break;
    }

    MemRegionTable[Index] = MemRegionBaseAddr;
  }

  DEBUG ((DEBUG_INFO, "ProcTrace: allocated memory %p for %d threads\n", MemRegionTable, Index));
  return MemRegionTable;
}

/**
  Program Output base and mask for processor trace

  @param[in] CpuTestConfig - Pointer to CPU_TEST_CONFIG.
  @param[in] IsBsp         - TRUE indicates the function is called from BSP.
**/
VOID
SetProcTraceOutput (
  IN CPU_TEST_CONFIG *CpuTestConfig,
  IN BOOLEAN         IsBsp
  )
{
  EFI_STATUS                               Status;
  UINT32                                   MemRegionSize;
  UINTN                                    MemRegionBaseAddr;
  STATIC UINTN                             *MemRegionTable;
  STATIC UINTN                             *TopaMemArray;
  UINTN                                    TopaTableBaseAddr;
  UINTN                                    ProcessorNumber;
  UINTN                                    NumberOfProcessors;
  UINTN                                    NumberOfEnabledProcessors;
  BOOLEAN                                  SetMsr;

  MemRegionBaseAddr = 0;
  TopaTableBaseAddr = 0;

  if ((CpuTestConfig == NULL)
  || ((CpuTestConfig->ProcessorTraceBspOnly != 0) && !IsBsp)) {
    return;
  }

  Status = mMpServices2Ppi->WhoAmI (mMpServices2Ppi, &ProcessorNumber);
  if (EFI_ERROR (Status)) {
    return;
  }

  if (!ProcTraceOutputSupported ((BOOLEAN)CpuTestConfig->ProcessorTraceOutputScheme)) {
    return;
  }

  ///
  /// Refer to PROC_TRACE_MEM_SIZE Table for Size Encoding
  ///
  MemRegionSize = (UINT32)(1 << (CpuTestConfig->ProcessorTraceMemSize + 12));
  if (IsBsp) {
    NumberOfEnabledProcessors = 1;
    if (CpuTestConfig->ProcessorTraceBspOnly == 0) {
      Status = mMpServices2Ppi->GetNumberOfProcessors (mMpServices2Ppi, (UINTN *)&NumberOfProcessors, (UINTN *)&NumberOfEnabledProcessors);
      if (EFI_ERROR (Status)) {
        return;
      }
    }
    MemRegionTable = GetMemoryTable (MemRegionSize, NumberOfEnabledProcessors);
  }
  if ((MemRegionTable != NULL)) {
    MemRegionBaseAddr = MemRegionTable[ProcessorNumber];
  }
  if (MemRegionBaseAddr == 0) {
    return;
  }

  if (CpuTestConfig->ProcessorTraceOutputScheme != 0) {
    if (IsBsp) {
      TopaMemArray = GetMemoryTable (sizeof (RTIT_TOPA_TABLE_ENTRY) * 2, NumberOfEnabledProcessors);
    }
    if (TopaMemArray != NULL) {
      TopaTableBaseAddr = TopaMemArray[ProcessorNumber];
      if (TopaTableBaseAddr == 0) {
        return;
      }
    } else {
      FreePool (MemRegionTable);
      MemRegionTable = NULL;
      return;
    }
  } else {
    TopaTableBaseAddr = 0;
  }

  SetMsr = MsrSetProcTraceOutput (
            (BOOLEAN)CpuTestConfig->ProcessorTraceOutputScheme,
            MemRegionBaseAddr,
            TopaTableBaseAddr,
            CpuTestConfig->ProcessorTraceMemSize);
  if (!SetMsr && IsBsp) {
    if (MemRegionTable != NULL) {
      FreePool (MemRegionTable);
      MemRegionTable = NULL;
    }
    if (TopaMemArray != NULL) {
      FreePool (TopaMemArray);
      TopaMemArray = NULL;
    }
  }
}

/**
  Program all processor features basing on desired settings

  @param[in] IsBsp - TRUE indicates the function is called from BSP.
**/
VOID
EFIAPI
ProgramProcessorFeature (
  BOOLEAN                        IsBsp
  )
{
  MSR_MTRRCAP_REGISTER           MtrrCap;

  ///
  /// Get Trace Hub Acpi Base address for BSP
  ///
  if (IsBsp) {
    GetTraceHubAcpiBaseAddressForBsp ();
  }

  if (!IsBsp) {
    ///
    /// This is to program Trace Hub ACPI base address for all the threads
    ///
    if (mTraceHubFeatureEnable) {
      AsmWriteMsr64 (MSR_NPK_STH_ACPIBAR_BASE, mTraceHubAcpiBaseAddress);
    }
  }

  //
  // Set PMON Address to MSR
  //
  if ((BOOLEAN)mCpuInitConfig->PmonEnable) {
    MsrSetPmonAddress (mCpuInitConfig->PmonAddress);
  }
  //
  // Enable or disable Crashlog, Gprs dump on CPU Core by MSR
  //
  EnableCpuCrashLog ((BOOLEAN)(mCpuInitPreMemConfig->CrashLogEnable));
  MsrSetCrashLogGprsMask ((BOOLEAN)(mCpuInitPreMemConfig->CrashLogEnable), (UINT8)(mCpuInitPreMemConfig->CrashLogGprs));

  //
  // Same MSR offset definition MSR_IA32_MTRRCAP 0x000000FE in ArchitecturalMsr.h
  //
  MtrrCap.Uint64 = AsmReadMsr64 (MSR_MTRRCAP);
  if (MtrrCap.Bits.Prmrr != 0) {
    ///
    /// Initialize Core PRMRRs.
    ///
    SyncPrmrrMsr (IsBsp);
  }

  if (MtrrCap.Bits.Seamrr != 0) {
    ///
    /// Initialize Core SEAMRRs.
    ///
    SyncSeamrrMsr (IsBsp);
  }

  if (IsTmeEnabled () == TRUE) {
    if (IsBsp) {
      DEBUG ((DEBUG_INFO, "Configuring MKTME_CORE_ACTIVATION\n"));
    }
    ConfigureTmeCoreActivation ();
  }
  ///
  /// Configure BIOS uCode PM_TMR Emulation MSR 121h on all logical processors (BSP & APs).
  /// Only enable this when PCH PM Timer is disabled.
  ///
  if (mPmTimerEmulationEnable) {
    BiosUcodePmTmrEmulationMsrCfg (IsBsp);
  }

  if (IsBsp) {
    DEBUG ((DEBUG_INFO, "Initialize AVX Instructions\n"));
  }
  //
  // Enable/Disable AVX Instructions
  //
  MsrSetAvx ((BOOLEAN) mCpuInitConfig->AvxDisable);

  if ((mCpuTestConfig->ProcessorTraceEnable == 0)
    && mCpuTestConfig->ProcessorTraceMemSize <= RtitTopaMemorySize128M) {
    SetProcTraceOutput (mCpuTestConfig, IsBsp);
  }

  return;
}

/**
  Set the BIOS DONE MSR
**/
VOID
EFIAPI
ApSafeSetBiosDone (
  VOID
  )
{
  MSR_BIOS_DONE_REGISTER  MsrBiosDone;
  MsrBiosDone.Uint64 = AsmReadMsr64 (MSR_BIOS_DONE);
  MsrBiosDone.Bits.EnableIaUntrusted = 1;
  AsmWriteMsr64 (MSR_BIOS_DONE, MsrBiosDone.Uint64);
  return;
}

/**
  Set the BIOS DONE MSR for all Enabled Cores
**/
VOID
EFIAPI
SetBiosDone (
  VOID
  )
{
  EFI_STATUS   Status;
  ///
  /// Set the BIOS DONE MSR before second patch load
  ///
  DEBUG ((DEBUG_INFO, "SetBiosDone Start\n"));
  ApSafeSetBiosDone ();
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApSafeSetBiosDone,
                             FALSE,
                             0,
                             NULL
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
    ASSERT (FALSE);
  }
  return;
}

/**
  Return the latest microcode.

  @param[in]  Microcode        The address of the microcode update binary (in memory).
  @param[in]  MicrocodeSize    Size of microcode region (in memory).

  @return  The latest microcode.
**/
CPU_MICROCODE_HEADER *
LocateLatestMicrocode (
  IN  CPU_MICROCODE_HEADER *Microcode,
  IN  UINT32               MicrocodeSize
  )
{
  UINT32                        LatestRevision;
  UINTN                         MicrocodeEnd;
  CPU_MICROCODE_HEADER          *LatestMicrocode;
  EDKII_PEI_MICROCODE_CPU_ID    MicrocodeCpuId;
  MSR_IA32_PLATFORM_ID_REGISTER Msr;

  Msr.Uint64 = AsmReadMsr64 (MSR_IA32_PLATFORM_ID);
  MicrocodeCpuId.PlatformId = (UINT8) Msr.Bits.PlatformId;
  AsmCpuid (CPUID_VERSION_INFO, &MicrocodeCpuId.ProcessorSignature, NULL, NULL, NULL);

  LatestRevision  = 0;
  LatestMicrocode = NULL;
  MicrocodeEnd = (UINTN) Microcode + MicrocodeSize;

  do {
    if (!IsValidMicrocode (Microcode, MicrocodeEnd - (UINTN) Microcode, LatestRevision, &MicrocodeCpuId, 1, TRUE)) {
      //
      // Microcode is aligned in 1KB. Check next 1KB when invalid header is met.
      //
      Microcode = (CPU_MICROCODE_HEADER *) ((UINTN) Microcode + SIZE_1KB);
      continue;
    }
    LatestMicrocode = Microcode;
    LatestRevision  = LatestMicrocode->UpdateRevision;
    Microcode = (CPU_MICROCODE_HEADER *) (((UINTN) Microcode) + GetMicrocodeLength (Microcode));
  } while ((UINTN) Microcode < MicrocodeEnd);

  return LatestMicrocode;
}

/**
  Re-load microcode patch.

  @param[in] Buffer      - A pointer to buffer which need to append the element
**/
VOID
ReloadMicrocodePatch (
  VOID
  )
{
  UINT32                     CurrentRevision;
  EFI_HOB_GUID_TYPE          *GuidHob;
  EDKII_MICROCODE_PATCH_HOB  *MicrocodePatchHob;
  CPU_MICROCODE_HEADER       *LatestMicrocode;
  CPU_MICROCODE_HEADER       *MicrocodeInMem;

  DEBUG ((DEBUG_INFO, "ReloadMicrocodePatch: second patch load started\n"));

  ///
  /// Init XMM
  ///
  XmmInit ();

  LatestMicrocode = NULL;
  MicrocodeInMem  = NULL;
  CurrentRevision = 0;
  //
  // Use the information from HOB first.
  //
  GuidHob = GetFirstGuidHob (&gEdkiiMicrocodePatchHobGuid);
  if (GuidHob != NULL) {
    MicrocodePatchHob = GET_GUID_HOB_DATA (GuidHob);

    DEBUG ((DEBUG_INFO, " MicrocodePatchHob->MicrocodePatchAddress = 0x%x\n", MicrocodePatchHob->MicrocodePatchAddress));
    DEBUG ((DEBUG_INFO, " MicrocodePatchHob->ProcessorSpecificPatchOffset[0] = 0x%x\n", MicrocodePatchHob->ProcessorSpecificPatchOffset[0]));

    if (MicrocodePatchHob->ProcessorSpecificPatchOffset[0] != MAX_UINT64) {
      LatestMicrocode = (CPU_MICROCODE_HEADER *) (UINTN) (MicrocodePatchHob->MicrocodePatchAddress + MicrocodePatchHob->ProcessorSpecificPatchOffset[0]);
    }
  } else {
    //
    // Copy the microcode from flash to memory.
    //
    DEBUG ((DEBUG_INFO, " Microcode Region Address = %p, Size = %d\n", (UINTN) mCpuInitConfig->MicrocodePatchAddress, mCpuInitConfig->MicrocodePatchRegionSize));
    if ((mCpuInitConfig->MicrocodePatchRegionSize != 0) && (mCpuInitConfig->MicrocodePatchAddress != 0)) {
      MicrocodeInMem = AllocatePages (EFI_SIZE_TO_PAGES (mCpuInitConfig->MicrocodePatchRegionSize));
      if (MicrocodeInMem != NULL) {
        CopyMem (MicrocodeInMem, (VOID *) (UINTN) mCpuInitConfig->MicrocodePatchAddress, mCpuInitConfig->MicrocodePatchRegionSize);
        LatestMicrocode = LocateLatestMicrocode (MicrocodeInMem, mCpuInitConfig->MicrocodePatchRegionSize);
      } else {
        DEBUG ((DEBUG_WARN, "Page allocation for Microcode failed, locate latest Microcode from flash instead.\n"));
        LatestMicrocode = LocateLatestMicrocode ((VOID *) (UINTN) mCpuInitConfig->MicrocodePatchAddress, mCpuInitConfig->MicrocodePatchRegionSize);
      }
    }
  }

  if (LatestMicrocode != NULL) {
    CurrentRevision = GetProcessorMicrocodeSignature ();
    DEBUG ((DEBUG_INFO, "Before load patch [revision = 0x%x], current revision = 0x%x\n", LatestMicrocode->UpdateRevision, CurrentRevision));
    if (LatestMicrocode->UpdateRevision >= CurrentRevision) {
      LoadMicrocode (LatestMicrocode);
      DEBUG ((DEBUG_INFO, "After load patch [revision = 0x%x], current revision = 0x%x\n", LatestMicrocode->UpdateRevision, GetProcessorMicrocodeSignature ()));
    }
  }

  if (MicrocodeInMem != NULL) {
    FreePages (MicrocodeInMem, EFI_SIZE_TO_PAGES (mCpuInitConfig->MicrocodePatchRegionSize));
  }

  DEBUG_CODE_BEGIN ();
  if ((LatestMicrocode != NULL) && (LatestMicrocode->UpdateRevision >= CurrentRevision) && MsrReadMcu2ndPatchStatusFru()) {
    DEBUG ((DEBUG_INFO, "ReloadMicrocodePatch: Second patch load success.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "ReloadMicrocodePatch: Second patch didn't load.\n"));
  }
  DEBUG_CODE_END ();
}

/**
  This function gets the total number of Small cores
**/
STATIC
VOID
GetNumberOfSmallCores (
  VOID
  )
{
  UINT8   CoreType;
  CoreType = 0;
  GetCoreType (&CoreType);
  if (CoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
    mSmallCoreCount++;
  }
}

/**
  Activate the Key Locker MSR
**/
STATIC
VOID
ActivateKeyLocker (
  VOID
  )
{
  MSR_PRMRR_MASK_REGISTER      MsrPrmrrMask;
  MSR_BIOS_UPGD_TRIG_REGISTER  MsrBiosUpgdTrig;

  //
  // Activate the keylocker only when PRMRR MASK VLD Bit is set.
  //
  MsrPrmrrMask.Uint64 = AsmReadMsr64 (MSR_PRMRR_MASK);
  //
  // Check if PRMRR Valid bit is set
  //
  if (MsrPrmrrMask.Bits.Vld != 0) {
    //
    // BIOS Should do read and write back the MSR to do a Keylocker Activation from ucode.
    //
    MsrBiosUpgdTrig.Uint64 = AsmReadMsr64 (MSR_BIOS_UPGD_TRIG);
    AsmWriteMsr64 (MSR_BIOS_UPGD_TRIG, MsrBiosUpgdTrig.Uint64);
  }
}

/**
  Finalization of settings post Microcode patch reload
**/
VOID
ApSafePostMicrocodePatchInit (
  VOID
  )
{
  if (IsKeyLockerSupported () && (mSmallCoreCount != 0)) {
    ActivateKeyLocker ();
  }
  return;
}

/**
  This function is to set MCHECK_SKIPPED_OR_DONE in MCHECK_FLOW_STATUS Register
**/
VOID
SetMcheckFlowStatus (
  VOID
  )
{
  UINT64                            MchBar;
  MCHECK_FLOW_STATUS_MMIO_REGISTER  McheckFlowStatusRegister;

  McheckFlowStatusRegister.Data = 0;
  //
  // Program registers and Mchbar related to MCHEK_FLOW_STATUS registers.
  //
  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  McheckFlowStatusRegister.Fields.Mcheck_Skipped_OR_Done = 1;
  //
  // Update Mcheck status to MCHECK_FLOW_STATUS register MMIO
  //
  MmioWrite32 ((MchBar + MMIO_MCHECK_FLOW_STATUS_0_0_0_MCHBAR), McheckFlowStatusRegister.Data);
}

/**
  This function handles Cpu Initialization routine at the end of PEI

  @retval EFI_STATUS       - Always return EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuInitAtEndOfPei (
  VOID
  )
{
  MSR_TEE_ERR_CODE_REGISTER MsrTeeErrCode;
  EFI_STATUS                Status;

  DEBUG ((DEBUG_INFO, "ReloadMicrocodePatch Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_START); //PostCode (0xC60)

  if (IsSimicsEnvironment ()) {
    AsmWriteMsr64 (MSR_PROBE_SEG_REG_1, 0x02);
  }

  ///
  /// Reload Microcode Patch
  ///
  ReloadMicrocodePatch ();

  DEBUG ((DEBUG_INFO, "ReloadMicrocodePatch Done\n"));

  MsrTeeErrCode.Uint64 = AsmReadMsr64 (MSR_TEE_ERR_CODE);
  DEBUG ((DEBUG_INFO, "MCheck Error code post second patch load is: 0x%X\n", MsrTeeErrCode.Bits.McheckErrorCode));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_RELOAD_UCODEPATCH_DONE); //PostCode (0xC61)

  ///
  /// Program Mcheck_status_Flow
  ///
  SetMcheckFlowStatus ();

  DEBUG ((DEBUG_INFO, "ApSafePostMicrocodePatchInit Start\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_POST_UCODEPATCH_INIT_START); //PostCode (0xC62)

  ApSafePostMicrocodePatchInit ();
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ApSafePostMicrocodePatchInit,
                             FALSE,
                             0,
                             NULL
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "ApSafePostMicrocodePatchInit: StartupAllAps(): %r\n", Status));
    ASSERT (FALSE);
  }

  DEBUG ((DEBUG_INFO, "ApSafePostMicrocodePatchInit Done\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_UCODEPATCH_INIT_DONE); //PostCode (0xC6F)

  return EFI_SUCCESS;
}

/**
  Program the BIOS Parameters required for MCHECK module.
**/
VOID
EFIAPI
ProgramBiosParamsForMcheck (
  VOID
  )
{
  MCHECK_BIOS_PARAM_INFO    *BiosParamInfoPtr = NULL;
  UINT8                     BiosParamInfoUuid[] = MCHECK_BIOS_PARAM_INFO_UUID;

  //
  // BIOS_INFO Structure should be 4K Aligned
  //
  BiosParamInfoPtr = (MCHECK_BIOS_PARAM_INFO *) AllocatePages (EFI_SIZE_TO_PAGES (sizeof (MCHECK_BIOS_PARAM_INFO)));
  if (BiosParamInfoPtr == NULL) {
    DEBUG ((DEBUG_INFO, "Error: Cannot allocate memory for BiosParamInfoPtr\n"));
    ASSERT (FALSE);
    return;
  } else {
    ZeroMem (BiosParamInfoPtr, sizeof (MCHECK_BIOS_PARAM_INFO));
  }

  CopyMem (BiosParamInfoPtr->Header.Uuid.Uuid, BiosParamInfoUuid, sizeof (MCHECK_UUID));
  BiosParamInfoPtr->Header.Size = sizeof (MCHECK_BIOS_PARAM_INFO) - sizeof (MCHECK_HEADER);
  BiosParamInfoPtr->Header.Version = MCHECK_STRUCTURE_VERSION;


  //
  // Bit Map of features
  //
  // SGX         BIT0
  // SEAM        BIT1
  // C6DRAM      BIT2
  // Key Locker  BIT3
  // FuSa SAF    BIT4
  //

  //
  // Update the KL feature flag if it is supported
  //
  if (IsKeyLockerSupported ()) {
    BiosParamInfoPtr->Features |= KEYLOCKER_ENABLE;
  }
  //
  // Update the SEAM feature flag if SEAM Mode is supported
  //
  if (IsTdxEnabled ()) {
    BiosParamInfoPtr->Features |= SEAM_ENABLE;
    UpdateBiosParamInfo(BiosParamInfoPtr);
  }

  AsmWriteMsr64 (MSR_TEE_INPUT_PARAM, (UINTN) BiosParamInfoPtr);
  return;
}

/**
  Get the information needed to be dumped for one core.

  @param[in] Buffer - Buffer to contains the information.
**/
VOID
EFIAPI
GetDumpInfoPerCore (
  DUMP_INFO_PER_CORE           *Buffer
  )
{
  EFI_STATUS                   Status;
  UINTN                        ProcessorNumber;
  Status = mMpServices2Ppi->WhoAmI (mMpServices2Ppi, &ProcessorNumber);
  if (EFI_ERROR (Status)) {
    return;
  }
  Buffer[ProcessorNumber].ApicId = GetApicId();
  Buffer[ProcessorNumber].PmLogicId = GetPmLogicId ();
}

/**
  Initialize processor features, performance and power management features, BIOS Guard, and Overclocking etc features before RESET_CPL at post-memory phase.

  @param[in] SiPolicyPpi      The SI Policy PPI instance

  @retval EFI_SUCCESS     The driver installed/initialized correctly.
**/
EFI_STATUS
EFIAPI
CpuInit (
  IN  SI_POLICY_PPI           *SiPolicyPpi
  )
{
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi;
  UINTN                        NumberOfProcessors;
  UINTN                        NumberOfEnabledProcessors;
  DUMP_INFO_PER_CORE           *Buffer;
  UINTN                        Index;

  DEBUG ((DEBUG_INFO, "CpuInit Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INIT_START_POSTMEM); //PostCode (0xC10)

  mSiPolicyPpi = SiPolicyPpi;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &mCpuInitConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuTestConfigGuid, (VOID *) &mCpuTestConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &mCpuInitPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &mVtdConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Locate CpuMpCpu MpService Ppi
  ///
  Status = PeiServicesLocatePpi (
              &gEdkiiPeiMpServices2PpiGuid,
              0,
              NULL,
              (VOID **) &mMpServices2Ppi
              );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Fill Cpu MP system data
  ///
  mMpServices2Ppi->GetNumberOfProcessors (mMpServices2Ppi, (UINTN *)&NumberOfProcessors, (UINTN *)&NumberOfEnabledProcessors);

  DEBUG_CODE_BEGIN ();
  //
  // Dump Cpu Register in all cores
  //
  Buffer = (DUMP_INFO_PER_CORE *)AllocatePages (EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (DUMP_INFO_PER_CORE)));
  DEBUG ((DEBUG_INFO, "Dump Cpu Register in all cores:\n"));
  if (Buffer == NULL) {
    DEBUG ((DEBUG_INFO, "Error: Cannot allocate memory\n"));
    ASSERT (FALSE);
  } else {
    Status = mMpServices2Ppi->StartupAllCPUs (
                              mMpServices2Ppi,
                              (EFI_AP_PROCEDURE) GetDumpInfoPerCore,
                              0,
                              (VOID *) Buffer
                              );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "GetDumpInfoPerCore: StartupAllCPUs(): %r\n", Status));
      ASSERT (FALSE);
    }
    for (Index = 0; Index < NumberOfProcessors; Index++) {
      DEBUG ((DEBUG_INFO, "Dump Cpu Register for core %d\n", Index));
      DEBUG ((DEBUG_INFO, "   APIC ID = 0x%x\n", Buffer[Index].ApicId));
      DEBUG ((DEBUG_INFO, "   PmLogicId = 0x%llx\n", Buffer[Index].PmLogicId));
    }
  }
  FreePages(Buffer, EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (DUMP_INFO_PER_CORE)));
  DEBUG_CODE_END ();


  DEBUG ((DEBUG_INFO, "Set the Cpu Features for CpuFeaturesPei driver\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_SET_PCD_CPUFEATURE); //PostCode (0xC11)
  SetPcdCpuFeaturesSetting ();

  ///
  /// Check if PM_TMR emulation is required.
  ///
  if (PmcIsPchPmTimerEnabled () == FALSE) {
      mPmTimerEmulationEnable = TRUE;
  }

  ///
  /// Program processor supported features on BSP and on all APs
  ///
  DEBUG ((DEBUG_INFO, "ProgramProcessorFeature Start \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_START); //PostCode (0xC12)
  ProgramProcessorFeature (TRUE);
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) ProgramProcessorFeature,
                             FALSE,
                             0,
                             (VOID *) FALSE
                             );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "ProgramProcessorFeature: StartupAllAps(): %r\n", Status));
    ASSERT (FALSE);
  }

  DEBUG ((DEBUG_INFO, "ProgramProcessorFeature Done\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_PROGRAM_PROCFEATURE_DONE); //PostCode (0xC13)

  ///
  /// Get total number of small cores present in platform
  ///
  GetNumberOfSmallCores ();
  Status = mMpServices2Ppi->StartupAllAPs (
                             mMpServices2Ppi,
                             (EFI_AP_PROCEDURE) GetNumberOfSmallCores,
                             TRUE,
                             0,
                             NULL
                             );

  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "GetNumberOfSmallCores: StartupAllAps(): %r\n", Status));
  }
  DEBUG ((DEBUG_INFO, "Small Core Count - %x\n", mSmallCoreCount));

  //
  // Program the BIOS Params needed for MCHECK.
  //
  ProgramBiosParamsForMcheck ();

  //
  // Smm Relocation Initialize.
  //
  Status = SmmRelocationInit (mMpServices2Ppi);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "CpuInit: Not able to execute Smm Relocation Init.  Status: %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "CpuInit Done \n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_INIT_DONE); //PostCode (0xC1F)

  return EFI_SUCCESS;
}
