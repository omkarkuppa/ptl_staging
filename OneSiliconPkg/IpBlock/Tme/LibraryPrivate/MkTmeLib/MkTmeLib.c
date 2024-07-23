/** @file
  Initialize TME in PEI

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
#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/TmeLib.h>
#include <Library/MsrFruLib.h>
#include <Library/PcdLib.h>
#include <Library/PmcLib.h>
#include <Library/IoLib.h>
#include <Library/CpuPlatformLib.h>
#include <CpuRegs.h>
#include <Register/Cpuid.h>
#include <Register/GenerationMsr.h>
#include <Register/PmcRegs.h>
#include <Ppi/MpServices2.h>
#include <Library/TmeInfoLib.h>
#include <Library/PeiVtdInitFruLib.h>

#define TME_ACTIVATE_RETRY_LIMIT  10
#define TDX_KEY_ID_BITS            1

typedef struct {
  UINT64 TmeEnable          :1;
  UINT64 MkTmeMaxKeyIdBits  :4;
  UINT64 MkTmeMaxKey        :15;
  UINT64 TmePolicy          :4;
  UINT64 KeySelect          :1;
  UINT64 SaveKeyForStandby  :1;
  UINT64 MkTmeKeyIdBits     :4;
  UINT64 MkTmeCryptoAlgs    :16;
  UINT64 TmeBypassCapability:1;
  UINT64 TdxKeyIdBits       :4;
  UINT64 Rsvd               :13;
} TME_CONFIG;

/**
  Determines maximum physical address width using CPUID command.

  @retval MaxPhyAddrBits - maximum physical address width that can be used on the system.
**/
UINT32
GetMaxPhyAddrBits ()
{
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX               CpuIdEax;

  AsmCpuidEx (CPUID_VIR_PHY_ADDRESS_SIZE, 0, &CpuIdEax.Uint32, NULL, NULL, NULL);

  return CpuIdEax.Bits.PhysicalAddressBits;
}

/**
  Calculates memory region that shouldn't be encrypted

  @param[in] Base      Base address (must be >= Size)
  @param[in] Size      Size value (must be power of 2)
  @param[out] BaseReg  Contains value to be written to Exclude Base Reg
  @param[out] MaskReg  Contains value to be written to Exclude Mask Reg

  @retval none
**/
VOID
CalculateTmeExcludeBaseMask (
  IN   UINT64                        Base,
  IN   UINT64                        Size,
  OUT  MSR_TME_EXCLUDE_BASE_REGISTER *BaseReg,
  OUT  MSR_TME_EXCLUDE_MASK_REGISTER *MaskReg
  )
{
  UINT64 BitMask = 0;
  UINT32 MaxPhyAddrBits = 0;

  // Set upper 63:MAXPHYS bits to 0
  // MAXPHYS includes keyID bits
  MaxPhyAddrBits = GetMaxPhyAddrBits ();
  BitMask = (LShiftU64 (1, MaxPhyAddrBits) - 1) & ~(0xFFF);
  BaseReg->Uint64 = Base & BitMask;
  Size = ~(Size - 1);
  MaskReg->Uint64 = Size & BitMask;
  MaskReg->Bits.Enable = 1;
}

/**
  This function is to configure TME Activate MSR.

  @param[in]  TmeEnable
  @param[in]  KeySelect
  @param[in]  SaveKeyForStandby
  @param[in]  TmePolicy
  @param[in]  MkTmeKeyIdBits
  @param[in]  TdxKeyIdBits
  @param[in]  MkTmeCryptoAlgs
  @param[in]  TmeBypassCapability
**/
VOID
EFIAPI
MsrConfigureTmeActivate (
  IN UINT8   TmeEnable,
  IN UINT8   KeySelect,
  IN UINT8   SaveKeyForStandby,
  IN UINT8   TmePolicy,
  IN UINT8   MkTmeKeyIdBits,
  IN UINT8   TdxKeyIdBits,
  IN UINT16  MkTmeCryptoAlgs,
  IN UINT8   TmeBypassCapability
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64                 = 0;
    TmeActivate.Bits.TmeEnable         = TmeEnable;
    TmeActivate.Bits.KeySelect         = KeySelect;
    TmeActivate.Bits.SaveKeyForStandby = SaveKeyForStandby;
    TmeActivate.Bits.TmePolicy         = TmePolicy;
    TmeActivate.Bits.MkTmeKeyidBits    = MkTmeKeyIdBits;
    TmeActivate.Bits.TdxReservedKeyidBits = TdxKeyIdBits;
    TmeActivate.Bits.MkTmeCryptoAlgs   = MkTmeCryptoAlgs;
    TmeActivate.Bits.TmeBypassMode     = TmeBypassCapability;

    AsmWriteMsr64 (MSR_TME_ACTIVATE, TmeActivate.Uint64);
  }
}

/**
  This function write-lock the TME Activate MSR.

  @param[in]  TmeEnable
**/
VOID
EFIAPI
MsrLockTmeActivate (
  IN UINT32  TmeEnable
  )
{
  MSR_TME_ACTIVATE_REGISTER      TmeActivate;

  ///
  /// MK-TME MSRs can only be accessed when hardware
  /// supports it.  Otherwise, system will throw exception.
  ///
  if (IsTmeSupported () == TRUE) {
    TmeActivate.Uint64                 = 0;
    TmeActivate.Bits.TmeEnable         = TmeEnable;
    AsmWriteMsr64 (MSR_TME_ACTIVATE, TmeActivate.Uint64);
  }
}

/**
  Set the TME Activate MSR to the specified input parameter

  @param[in] TmeConfig     TME Activate config

  @retval TRUE             If TME Activate MSR is locked
  @retval FALSE            If TME Activate MSR is not locked
**/
BOOLEAN
SetTmeActivateMsr (
  IN TME_CONFIG TmeConfig
  )
{
  MSR_TME_ACTIVATE_REGISTER          TmeValidate;
  UINT8                              Index;

  ZeroMem (&TmeValidate, sizeof (MSR_TME_ACTIVATE_REGISTER));
  ///
  /// Configure MSR_TME_ACTIVATE (982H) with TME Enable, Key Select, Save Key, and TME Policy
  /// Lock bit will be set upon successful WRMSR for MSR_TME_ACTIVATE.
  ///   - First SMI will also set the Lock
  ///   - This will also lock MSR_TME_EXCLUDE_MASK and MSR_TME_EXCLUDE_BASE
  ///
  DEBUG ((DEBUG_INFO, "MK-TME: Initialize MSR_TME_ACTIVATE\n"));
  DEBUG ((DEBUG_INFO, " TmeEnable           = 0x%X\n", TmeConfig.TmeEnable));
  DEBUG ((DEBUG_INFO, " KeySelect           = 0x%X\n", TmeConfig.KeySelect));
  DEBUG ((DEBUG_INFO, " SaveKeyForStandby   = 0x%X\n", TmeConfig.SaveKeyForStandby));
  DEBUG ((DEBUG_INFO, " TmePolicy           = 0x%X\n", TmeConfig.TmePolicy));
  DEBUG ((DEBUG_INFO, " MkTmeKeyIdBits      = 0x%X\n", TmeConfig.MkTmeKeyIdBits));
  DEBUG ((DEBUG_INFO, " TdxKeyIdBits        = 0x%X\n", TmeConfig.TdxKeyIdBits));
  DEBUG ((DEBUG_INFO, " MkTmeCryptoAlgs     = 0x%X\n", TmeConfig.MkTmeCryptoAlgs));
  DEBUG ((DEBUG_INFO, " TmeBypassCapability = 0x%X\n", TmeConfig.TmeBypassCapability));

  // TME activate retry limit is 10
  for (Index = 0; Index < TME_ACTIVATE_RETRY_LIMIT; Index++) {
    MsrConfigureTmeActivate ((UINT8) TmeConfig.TmeEnable,
                             (UINT8) TmeConfig.KeySelect,
                             (UINT8) TmeConfig.SaveKeyForStandby,
                             (UINT8) TmeConfig.TmePolicy,
                             (UINT8) TmeConfig.MkTmeKeyIdBits,
                             (UINT8) TmeConfig.TdxKeyIdBits,
                             (UINT16) TmeConfig.MkTmeCryptoAlgs,
                             (UINT8) TmeConfig.TmeBypassCapability
                            );
    if (IsTmeActivateLockSet ()) {
      TmeValidate.Uint64 = AsmReadMsr64 (MSR_TME_ACTIVATE);
      DEBUG ((DEBUG_INFO, "MK-TME: MSR_TME_ACTIVATE was initialized and locked: %016llx\n", TmeValidate));
      return TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "MK-TME: Failed to initialize MSR_TME_ACTIVATE: %016llx\n", TmeValidate));

  return FALSE;
}

/**
  Configure TME Core Activation MSR.

  @retval EFI_UNSUPPORTED When Tme is not enabled nor supported in the system
  @retval EFI_SUCCESS     Successfully write to Core Activation Msr
**/
EFI_STATUS
EFIAPI
MsrConfigureTmeCoreActivate (
  VOID
  )
{
  if (IsTmeSupported () == TRUE) {
    if (IsTmeEnabled () == TRUE) {
      ///
      /// write zeroes in the TME Core Activate MSR
      ///
      AsmWriteMsr64 (MSR_CORE_MKTME_ACTIVATION, 0);
      return EFI_SUCCESS;
    }
  }

  return EFI_UNSUPPORTED;
}

/**
  Perform Multi-Key Total Memory Encryption initialization.

  @param[in] TmePolicy      - TME policy

  @retval VOID - No value to return
**/
VOID
TmeInit (
  IN VOID *TmePolicy
  )
{

  MSR_TME_EXCLUDE_MASK_REGISTER      ExcludeMask;
  MSR_TME_EXCLUDE_BASE_REGISTER      ExcludeBase;
  EFI_BOOT_MODE                      BootMode;
  EFI_STATUS                         Status;
  TME_CONFIG                         TmeConfig;
  TME_POLICY                         *LocalTmePolicy;
  UINT32                             RegisterVal32;
  UINT8                              MkTmeKeyIdBits;
  BOOLEAN                            WarmReset;
  BOOLEAN                            IsLocked;

  BootMode  = 0;
  Status    = 0;
  WarmReset = FALSE;
  IsLocked  = FALSE;


  DEBUG ((DEBUG_INFO, "Multi-Key Total Memory Encryption (MK-TME) Initialization\n"));

  ///
  /// Detect MK-TME IP is supported in the hardware
  ///
  if (IsMkTmeSupported () == FALSE) {
    return;
  }

  if (TmePolicy == NULL) {
    DEBUG ((DEBUG_ERROR, "TmePolicy is null, lock ACTIVATE MSR then exit\n"));
    MsrLockTmeActivate(0);
    return;
  } else {
    LocalTmePolicy = (TME_POLICY *) TmePolicy;
  }

  if (LocalTmePolicy->VtdEnable == FALSE) {
    DEBUG ((DEBUG_INFO, "MK-TME: Skip MK-TME initialization due to VT-D is disabled.\n"));
    ///
    /// Lock MSR before returning
    ///
    MsrLockTmeActivate(0);
    return;
  }

  ///
  /// Detect MK-TME capabilities are supported in the hardware
  ///
  if (IsTmeCapabilityPresent () == FALSE) {
    DEBUG ((DEBUG_INFO, "MK-TME: Skip MK-TME initialization due to not supported in hardware.\n"));
    ///
    /// Lock MSR before returning
    ///
    MsrLockTmeActivate(0);
    return;
  }

  ///
  /// Detect BIOS policy
  ///
  if (LocalTmePolicy->TmeEnable == 0) {
    DEBUG ((DEBUG_INFO, "MK-TME: Policy is disabled in BIOS Setup.\n"));
    DEBUG ((DEBUG_INFO, "MK-TME: Skip MK-TME initialization.\n"));

    ///
    /// Ensure Lock bit of MSR_TME_ACTIVATE (982H) is set
    /// Lock bit will not be set if creation of ephemeral number using DRNG action failed.
    ///
    DEBUG ((DEBUG_INFO, "MK-TME: TME was not enabled. Locking MSR_TME_ACTIVATE.\n"));
    MsrLockTmeActivate(0);
    return;
  }

  ZeroMem (&TmeConfig, sizeof (TME_CONFIG));

  ///
  /// Set TME policy
  ///
  TmeConfig.TmePolicy = V_TME_ACTIVATE_TME_POLICY_AES_XTS_256;

  ///
  /// Read Max supported Keys
  ///
  TmeConfig.MkTmeMaxKeyIdBits = (UINT8) MsrGetMkTmeMaxKeyidBits ();
  TmeConfig.MkTmeMaxKey       = (UINT16) MsrGetMkTmeMaxKey ();

  ///
  /// Configure MSR_TME_EXCLUDE_MASK (983H) with TME Mask and Enable bit
  ///
  /// Configure MSR_TME_EXCLUDE_BASE (984H) with TME Base
  ///
  if (LocalTmePolicy->TmeExcludeSize != 0 && LocalTmePolicy->TmeExcludeBase != 0) {
    CalculateTmeExcludeBaseMask(
      LocalTmePolicy->TmeExcludeBase,
      LocalTmePolicy->TmeExcludeSize,
      &ExcludeBase,
      &ExcludeMask
    );

    AsmWriteMsr64 (MSR_TME_EXCLUDE_MASK, ExcludeMask.Uint64);
    AsmWriteMsr64 (MSR_TME_EXCLUDE_BASE, ExcludeBase.Uint64);
  }

  ExcludeBase.Uint64 = AsmReadMsr64 (MSR_TME_EXCLUDE_BASE);
  DEBUG ((DEBUG_INFO, "TME_EXCLUDE_BASE (0x984)\n"));
  DEBUG ((DEBUG_INFO, " [BIT 31:12] ExcludeBase0: 0x%X\n", ExcludeBase.Bits.Tmebase0));
  DEBUG ((DEBUG_INFO, " [BIT 52:32] ExcludeBase1: 0x%X\n", ExcludeBase.Bits.Tmebase1));
  ExcludeMask.Uint64 = AsmReadMsr64(MSR_TME_EXCLUDE_MASK);
  DEBUG ((DEBUG_INFO, "TME_EXCLUDE_MASK (0x983)\n"));
  DEBUG ((DEBUG_INFO, " [BIT 11]    Enable:       0x%X\n", ExcludeMask.Bits.Enable));
  DEBUG ((DEBUG_INFO, " [BIT 31:12] ExcludeMask0: 0x%X\n", ExcludeMask.Bits.Tmemask0));
  DEBUG ((DEBUG_INFO, " [BIT 52:31] ExcludeMask1: 0x%X\n", ExcludeMask.Bits.Tmemask1));

  ///
  /// Set TME Key Select and Save Key for Warm Reset, Standby and Flash Update Mode
  /// Key Select
  ///   - Set for Warm Reset, S3 resume flow and flash update flow to restore
  ///     TME keys from PCH
  ///   - Clear for cold boot to create new TME keys
  /// Save Key
  ///   - Clear for S3 resume flow and flash update flow
  ///   - Set for cold boot to save key into storage
  ///
  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Detect Warm Reset boot
  ///
  RegisterVal32 = MmioRead32 (PmcGetPwrmBase () + R_PMC_PWRM_GEN_PMCON_A);
  if ((((RegisterVal32 & B_PMC_PWRM_GEN_PMCON_A_MEM_SR) != 0) &&
      ((RegisterVal32 & B_PMC_PWRM_GEN_PMCON_A_DISB) != 0))) {
    WarmReset = TRUE;
  }

  if (((BootMode == BOOT_ON_S3_RESUME) ||
      (BootMode == BOOT_ON_FLASH_UPDATE) ||
      (WarmReset == TRUE)) && (LocalTmePolicy->GenerateNewTmeKey == FALSE)) {
    TmeConfig.KeySelect = 1;
    TmeConfig.SaveKeyForStandby = 0;
  } else {
    TmeConfig.KeySelect = 0;
    TmeConfig.SaveKeyForStandby = 1;
  }

  ///
  ///  Support for key split between MKTME & TDx KeyIds
  ///
  MkTmeKeyIdBits = PcdGet8 (PcdMkTmeKeyIdBits);
  if (MkTmeKeyIdBits > TmeConfig.MkTmeMaxKeyIdBits) {
    ///
    /// if Pcd is greater than max then configure KeyId to max supported
    ///
    TmeConfig.MkTmeKeyIdBits  = TmeConfig.MkTmeMaxKeyIdBits;
  } else {
    TmeConfig.MkTmeKeyIdBits = MkTmeKeyIdBits;
  }

  // Check if TDx is supported
  if (LocalTmePolicy->TdxEnable) {
    // Set tdx keyid bits. The default value is 1.
    TmeConfig.TdxKeyIdBits = PcdGet8 (PcdTdxKeyIdBits);

    DEBUG ((DEBUG_INFO, " Key split between MKTME & TDx KeyIds.\n"));
    DEBUG ((DEBUG_INFO, " MktmeKeyIdBits = 0x%X\n", TmeConfig.MkTmeKeyIdBits));
    DEBUG ((DEBUG_INFO, " TdxKeyIdBits   = 0x%X\n", TmeConfig.TdxKeyIdBits));
  }

  TmeConfig.MkTmeCryptoAlgs = 0x4;  // value 0x4 equates to MSR_TME_ACTIVATE (982H) [BIT50] set

  ///
  /// set the TME Bypass Capability
  ///
  TmeConfig.TmeBypassCapability = LocalTmePolicy->TmeBypassCapability;

  ///
  /// Set TME enable
  /// TME capability and encryption policy is supported
  ///
  TmeConfig.TmeEnable = 1;

  ///
  /// Call to set TME_ACTIVATE_MSR
  ///
  IsLocked = SetTmeActivateMsr(TmeConfig);

  ///
  /// If TME_ACTIVATE_MSR failed to lock and code tried to restore previous key
  ///
  if (IsLocked == FALSE && TmeConfig.KeySelect == 1){
    ///
    /// Attempt to set TME_ACTIVATE_MSR with new key
    ///
    TmeConfig.KeySelect = 0;
    TmeConfig.SaveKeyForStandby = 1;
    IsLocked = SetTmeActivateMsr(TmeConfig);
  }

  if (IsLocked == FALSE) {
    /// If previous attempts to set TME_ACTIVATE failed, lock TME_ACTIVATE to 0
    MsrLockTmeActivate(0);
  }

  return;
}