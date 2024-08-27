/** @file
  CPU Common Lib implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuLib.h>
#include <Library/TimerLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/BootGuardLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/MtrrLib.h>
#include <Library/PmcPrivateLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Register/LocalApic.h>
#include <Register/GenerationMsr.h>
#include <CpuRegs.h>
#include <Txt.h>
#include <Register/CpuGenRegs.h>
#include <Register/B2pMailbox.h>
#include <Guid/MpInformation2.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>

#define INTERRUPT_VECTOR_NUMBER 256

/**
  This function returns Max Turbo Ratio.

  @return Max Turbo Ratio
**/
UINT8
EFIAPI
GetMaxTurboRatio (
  VOID
  )
{
   MSR_TURBO_RATIO_LIMIT_REGISTER MsrTurboRatioLimitRatio;

  //
  // Read BIGCORE turbo ratio limit ratio
  //
  MsrTurboRatioLimitRatio.Uint64 = AsmReadMsr64 (MSR_TURBO_RATIO_LIMIT);
  return (UINT8) MsrTurboRatioLimitRatio.Bits.RatioLimit0;
}

/**
  Return CPU Identifier used to identify various CPU types

  @retval CPU_IDENTIFIER           CPU Identifier
**/
UINT32
EFIAPI
GetCpuIdentifier (
  VOID
  )
{
  return (UINT32) GetCpuSkuIdentifier (GetCpuSku (), GetCpuSteppingId ());
}

/**
  This function is to program Trace Hub ACPI base address to processor's MSR TRACE_HUB_STH_ACPIBAR_BASE.

  @param[in]  TraceHubAcpiBase - Base address of Trace Hub ACPI Base address
**/
VOID
EFIAPI
CpuWriteTraceHubAcpiBase (
  IN UINT64  TraceHubAcpiBase
  )
{
  MSR_NPK_STH_ACPIBAR_BASE_REGISTER BaseAddressMask;
  MSR_NPK_STH_ACPIBAR_BASE_REGISTER MsrTraceHubAcpiBase;

  //
  // Make up Trace Hub ACPI Base address mask.
  //
  BaseAddressMask.Uint64 = MAX_UINT64;
  BaseAddressMask.Bits.Lock = 0;
  BaseAddressMask.Bits.Rsvd = 0;

  //
  // Check the pass in Trace Hub ACPI base if 256KB alignment.
  //
  if ((TraceHubAcpiBase & ~BaseAddressMask.Uint64) != 0) {
    ASSERT ((TraceHubAcpiBase & ~BaseAddressMask.Uint64) == 0);
    return;
  }

  ///
  /// Set MSR TRACE_HUB_STH_ACPIBAR_BASE[0] LOCK bit for the AET packets to be directed to NPK MMIO.
  ///
  MsrTraceHubAcpiBase.Uint64 = TraceHubAcpiBase;
  MsrTraceHubAcpiBase.Bits.Lock = 1;
  AsmWriteMsr64 (MSR_NPK_STH_ACPIBAR_BASE, MsrTraceHubAcpiBase.Uint64);

  return;
}

/**
  Private helper function to convert various Turbo Power Limit Time from milli Seconds to CPU units

  @param[in] TimeInMilliSeconds       Time in milli seconds

  @retval UINT8 Converted time in CPU units
**/
UINT8
EFIAPI
GetConvertedTime (
  IN UINT32          TimeInMilliSeconds
  )
{
  //
  // Assume Cpu time unit is 1/1024s
  //
  UINT64  TimeInCpuUnit;

  //
  // AccurateTime unit is 1/(1024*1000)s
  //
  UINT64  AccurateTime;
  UINT64  AccurateTimeFloor;
  UINT64  AccurateTimeCeil;
  INTN    HighBit;
  //
  // Time Window = (float) ((1+(X/4)) * (2 ^ Y))
  // Where:
  // X = OUTPUT[6:5]
  // Y = OUTPUT[4:0]
  //
  UINT8   X;
  UINT8   Y;

  TimeInCpuUnit = (UINT64) TimeInMilliSeconds * 1024 / 1000;
  AccurateTime  = (UINT64) TimeInMilliSeconds * 1024;

  HighBit = HighBitSet64 (TimeInCpuUnit);
  if (HighBit < 0) {
    return 0;
  }
  ASSERT ((UINTN)(UINT8) HighBit == (UINTN) HighBit);
  //
  // Y = log2(TimeInCpuUnit)
  // X = ((TimeInCpuUnit/(2^Y))-1)*4
  // Below X,Y is the Floor(TimeInCpuUnit)
  //
  Y = (UINT8) HighBit;
  X = (UINT8) (RShiftU64(TimeInCpuUnit * 4, Y) - 4);
  ASSERT (X <= 3);
  AccurateTimeFloor = RShiftU64(LShiftU64(4 + X, Y) * 1000, 2);

  //
  // Get the ceil value of TimeInCpuUnit
  // use (X+1, Y) or (0, Y+1) to get the ceil value
  //
  if (X == 3) {
    AccurateTimeCeil = RShiftU64(LShiftU64(4, Y + 1) * 1000, 2);
  } else {
    AccurateTimeCeil = RShiftU64(LShiftU64(4 + X + 1, Y) * 1000, 2);
  }
  ASSERT (AccurateTimeCeil >= AccurateTime);
  ASSERT (AccurateTime >= AccurateTimeFloor);

  //
  // Compare to determine if the ceil or floor value is more accurate
  //
  if ((AccurateTimeCeil - AccurateTime) <= (AccurateTime - AccurateTimeFloor)) {
    if (X == 3) {
      X = 0;
      Y += 1;
    } else {
      X += 1;
    }
  }
  if (Y >= (1<<5)) {
    //
    // Too large input, use max value
    //
    X = 3;
    Y = 31;
  }

  return (X<<5) + Y;
}

/**
  Check on the processor if PRMRR is supported.

  @param[in]  IsBspInt  Check to see if the executing thread is BSP.

  @retval TRUE  if PRMRR supported
  @retval FALSE if PRMRR is not supported
**/
BOOLEAN
IsPrmrrSupported (
  BOOLEAN    IsBspInt
  )
{
  ///
  /// PRMRR configuration enabled, MSR IA32_MTRRCAP (FEh) [12] == 1
  ///
  if ((AsmReadMsr64 (MSR_IA32_MTRRCAP) & BIT12) != 0) {
    return TRUE;
  }
  //
  // Continue patch load without PRMRR initialization.
  //
  if (IsBspInt) {
    DEBUG ((DEBUG_INFO, "PRMRR configuration not supported.\n"));
  }
  return FALSE;
}

/**
  Set the PRMRR MSRs.

  @param PrmrrBase PRMRR base address
  @param PrmrrSize PRMRR size

  @retval RETURN_SUCCESS       PRMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   PRMRR is not supported.
  @retval RETURN_ACCESS_DENIED PRMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
SetPrmrrMsr (
  UINT64 PrmrrBase,
  UINT32 PrmrrSize
  )
{
  UINT64                          ValidPrmrrBitsMask;
  UINT64                          PrmrrMask;
  UINT32                          MaxExtendedFunction;
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX  VirPhyAddressSize;
  MSR_PRMRR_MASK_REGISTER         MsrPrmrrMask;
  MSR_PRMRR_BASE_0_REGISTER       MsrPrmrrBase0;

  if (!IsPrmrrSupported (TRUE)) {
    return RETURN_UNSUPPORTED;
  }

  MsrPrmrrMask.Uint64 = AsmReadMsr64 (MSR_PRMRR_MASK);
  ASSERT (MsrPrmrrMask.Bits.L == 0);
  if (MsrPrmrrMask.Bits.L != 0) {
    DEBUG ((DEBUG_ERROR, "PRMRR region is already locked and cannot be changed!\n"));
    return RETURN_ACCESS_DENIED;
  }

  //
  // If PRMRR size is 0, only need to set bit lock on MSR 0x1f5
  //
  if (PrmrrSize == 0) {
    //
    // Need to lock mask MSRs even if PRMRR size is zero
    //
    DEBUG ((DEBUG_INFO, "PRMRR size is zero, only Locking PRMRR MASK MSR\n"));
    MsrPrmrrMask.Bits.L = 1;
    AsmWriteMsr64 (MSR_PRMRR_MASK, MsrPrmrrMask.Uint64);
  } else {

    VirPhyAddressSize.Bits.PhysicalAddressBits = 36;
    //
    // Check whether CPU supports to return max physical address size
    //
    AsmCpuid (CPUID_EXTENDED_FUNCTION, &MaxExtendedFunction, NULL, NULL, NULL);
    if (MaxExtendedFunction >= CPUID_VIR_PHY_ADDRESS_SIZE) {
      //
      // PRMRR Mask should always use the Physical Address bits returned from SoC Capabilities - CPUID.80000008H:EAX
      //
      AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &VirPhyAddressSize.Uint32, NULL, NULL, NULL);
    }

    ValidPrmrrBitsMask = (LShiftU64 (1, VirPhyAddressSize.Bits.PhysicalAddressBits) - 1) & 0xfffffffffffff000;
    PrmrrMask          = ValidPrmrrBitsMask & (~((UINT64)PrmrrSize - 1));
    //
    // Set the PRMRR Base MSR with base, memory type, and configured status.Set PRMRR Mask MSR with the mask and lock bit.
    // The valid bit on PRMRR Mask MSR will be set later by the microcode.
    //
    MsrPrmrrBase0.Uint64          = PrmrrBase;
    MsrPrmrrBase0.Bits.Memtype    = MTRR_CACHE_WRITE_BACK;
    MsrPrmrrBase0.Bits.Configured = 1;
    AsmWriteMsr64 (MSR_PRMRR_BASE_0, MsrPrmrrBase0.Uint64);
    MsrPrmrrMask.Uint64 = PrmrrMask;
    MsrPrmrrMask.Bits.L = 1;
    AsmWriteMsr64 (MSR_PRMRR_MASK, MsrPrmrrMask.Uint64);
  }
  return RETURN_SUCCESS;
}

/**
  Set the SEAMRR MSRs.
  @param SeamrrBase SEAMRR base address
  @param SeamrrSize SEAMRR size
  @retval RETURN_SUCCESS       SEAMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   SEAMRR is not supported.
  @retval RETURN_ACCESS_DENIED SEAMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
SetSeamrrMsr (
  UINT64 SeamrrBase,
  UINT32 SeamrrSize
  )
{
  UINT64                          ValidSeamrrBitsMask;
  UINT64                          SeamrrMask;
  UINT32                          MaxExtendedFunction;
  CPUID_VIR_PHY_ADDRESS_SIZE_EAX  VirPhyAddressSize;
  MSR_SEAMRR_MASK_REGISTER        MsrSeamrrMask;
  MSR_SEAMRR_BASE_REGISTER        MsrSeamrrBase;

  DEBUG ((DEBUG_INFO, "[TDX] Inside programming SEAMRR BASE and MASK\n"));
  if (IsSeamModeSupported () == FALSE) {
    return RETURN_UNSUPPORTED;
  }

  MsrSeamrrMask.Uint64 = AsmReadMsr64 (MSR_SEAMRR_MASK);
  ASSERT (MsrSeamrrMask.Bits.L == 0);
  if (MsrSeamrrMask.Bits.L != 0) {
    DEBUG ((DEBUG_ERROR, "[TDX] SEAMRR region is already locked and cannot be changed!\n"));
    return RETURN_ACCESS_DENIED;
  }

  //
  // If SEAMRR size is 0, only need to set bit lock on MSR 0x1401
  //
  if (SeamrrSize == 0) {
    //
    // Need to lock mask MSRs even if SEAMRR size is zeros
    //
    DEBUG ((DEBUG_INFO, "[TDX] SEAMRR size is zero, not configuring the SEAMRR BASE and Locking SEAMRR MASK MSR\n"));
    MsrSeamrrBase.Bits.Configured = 0;
    MsrSeamrrMask.Bits.L = 1;
    AsmWriteMsr64 (MSR_SEAMRR_MASK, MsrSeamrrMask.Uint64);
  } else {
    VirPhyAddressSize.Bits.PhysicalAddressBits = 36;
    //
    // Check whether CPU supports to return max physical address size
    //
    AsmCpuid (CPUID_EXTENDED_FUNCTION, &MaxExtendedFunction, NULL, NULL, NULL);
    if (MaxExtendedFunction >= CPUID_VIR_PHY_ADDRESS_SIZE) {
      //
      // SEAMRR Mask should always use the Physical Address bits returned from SoC Capabilities - CPUID.80000008H:EAX
      //
      AsmCpuid (CPUID_VIR_PHY_ADDRESS_SIZE, &VirPhyAddressSize.Uint32, NULL, NULL, NULL);
    }
    ValidSeamrrBitsMask = (LShiftU64 (1, VirPhyAddressSize.Bits.PhysicalAddressBits) - 1) & 0xfffffffffffff000;
    SeamrrMask          = ValidSeamrrBitsMask & (~((UINT64)SeamrrSize - 1));
    //
    // Set the SEAMRR Base MSR with base and configured status.
    //
    MsrSeamrrBase.Uint64          = SeamrrBase;
    MsrSeamrrBase.Bits.Configured = 1;
    AsmWriteMsr64 (MSR_SEAMRR_BASE, MsrSeamrrBase.Uint64);
    //
    // Set SEAMRR Mask MSR with the mask and lock bit.
    //
    MsrSeamrrMask.Uint64 = SeamrrMask;
    MsrSeamrrMask.Bits.L = 1;
    AsmWriteMsr64 (MSR_SEAMRR_MASK, MsrSeamrrMask.Uint64);

    MsrSeamrrMask.Uint64 = AsmReadMsr64 (MSR_SEAMRR_MASK);
    MsrSeamrrBase.Uint64 = AsmReadMsr64 (MSR_SEAMRR_BASE);

    DEBUG ((DEBUG_INFO, "[TDX] MSR Read Seamrr Base %X \n", MsrSeamrrBase.Uint64));
    DEBUG ((DEBUG_INFO, "[TDX] MSR Read Seamrr Mask %X \n", MsrSeamrrMask.Uint64));
  }
  return RETURN_SUCCESS;
}

/**
  Lock the SEAMRR MSRs.
  @retval RETURN_SUCCESS       SEAMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   SEAMRR is not supported.
  @retval RETURN_ACCESS_DENIED SEAMRR MSRs are already locked and cannot be changed.
*/
RETURN_STATUS
EFIAPI
LockSeamrrMsr (
  VOID
  )
{
  MSR_SEAMRR_MASK_REGISTER        MsrSeamrrMask;

  if (IsSeamModeSupported () == FALSE) {
    return RETURN_UNSUPPORTED;
  }
  MsrSeamrrMask.Uint64 = AsmReadMsr64 (MSR_SEAMRR_MASK);
  ASSERT (MsrSeamrrMask.Bits.L == 0);
  if (MsrSeamrrMask.Bits.L != 0) {
    DEBUG ((DEBUG_ERROR, "SEAMRR region is already locked \n"));
    return RETURN_ACCESS_DENIED;
  }

  MsrSeamrrMask.Bits.L = 1;
  AsmWriteMsr64 (MSR_SEAMRR_MASK, MsrSeamrrMask.Uint64);

  DEBUG ((DEBUG_INFO, "Seamrr Mask = %X \n", MsrSeamrrMask.Uint64));

  return RETURN_SUCCESS;
}

/**
  Configures SE_SVN_MSR.

  @param SeSvnMsr                   Value of SeSvn Msr to program.

  @retval RETURN_SUCCESS            SeSvn MSR are set successfully.
  @retval RETURN_SECURITY_VIOLATION SeSvn MSR cannot be programmed.
*/
RETURN_STATUS
EFIAPI
SetBiosSeSvnMsr (
  UINT64 SeSvn
  )
{
  MSR_BIOS_SE_SVN_REGISTER MsrSeSvnToProgram;
  MSR_BIOS_SE_SVN_REGISTER MsrSeSvn;

  MsrSeSvn.Uint64 = SeSvn;
  MsrSeSvnToProgram.Uint64 = AsmReadMsr64 (MSR_BIOS_SE_SVN);

  // Checking now is only for SeamldrSvn
  if ((MsrSeSvnToProgram.Bits.SeamldrSeSvn != 0xFF) && (MsrSeSvn.Bits.SeamldrSeSvn < MsrSeSvnToProgram.Bits.SeamldrSeSvn)) {
    DEBUG ((DEBUG_ERROR, "SeamldrSeSvn in msr (%x) is bigger than value to program (%x) Aborting!\n", MsrSeSvnToProgram.Bits.SeamldrSeSvn, MsrSeSvn.Bits.SeamldrSeSvn));
    return RETURN_SECURITY_VIOLATION;
  }

  MsrSeSvnToProgram.Bits.SeamldrSeSvn = MsrSeSvn.Bits.SeamldrSeSvn;
  AsmWriteMsr64 (MSR_BIOS_SE_SVN, MsrSeSvnToProgram.Uint64);

  DEBUG ((DEBUG_INFO, "MsrSeSvn = %llx \n", MsrSeSvnToProgram.Uint64));

  return RETURN_SUCCESS;
}

/**
  Detect if Hetero Core is supported.

  @retval TRUE - Processor support HeteroCore
  @retval FALSE - Processor doesnt support HeteroCore
**/
BOOLEAN
EFIAPI
IsHeteroCoreSupported (
  VOID
  )
{
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EDX Edx;

  ///
  /// Check Hetero feature is supported
  /// with CPUID.(EAX=7,ECX=0):EDX[15]=1
  ///
  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, NULL, NULL, &Edx.Uint32);
  if (Edx.Bits.Hybrid == 1) {
    return TRUE;
  }
  return FALSE;
}

/**
  Detect the type of core, whether it is Big/Small Core.

  @param[out]    CoreType      Output pointer that get CPUID_NATIVE_MODEL_ID_INFO data
                 10h - Quark
                 20h - Atom
                 30H - Knights
                 40H - Core
**/
VOID
EFIAPI
GetCoreType (
  OUT  UINT8   *CoreType
  )
{
  UINT32   Eax;

  if (IsHeteroCoreSupported ()) {
    //
    // Check which is the running core by reading CPUID.(EAX=1AH, ECX=00H):EAX
    //
    AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax, NULL, NULL, NULL);
    *CoreType = (UINT8)((Eax & 0xFF000000) >> 24);
  } else {
    *CoreType = CPUID_CORE_TYPE_INTEL_CORE;
  }
}

/**
  Get 32-bit CPU Soft Strap data by each set.

  @param[in]  CpuStrapSetNum    CPU strap set number
  @param[out] CpuStrapSetData   CPU strap set value

  @retval EFI_SUCCESS           Get CPU Soft Strap data successfully or do not need to get data.
  @retval Others                Failed to get CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
GetCpuStrapDataBySet (
  IN  UINT8   CpuStrapSetNum,
  OUT UINT32  *CpuStrapSetData
  )
{
  EFI_STATUS                 Status;
  PCODE_MAILBOX_INTERFACE    MailboxCommand;
  UINT32                     MailboxData;
  UINT32                     MailboxStatus;

  if (CpuStrapSetData == NULL) {
    return EFI_SUCCESS;
  }

  //
  // Get CPU Soft Strap data specified by set number.
  //
  MailboxCommand.InterfaceData  = 0;
  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_READ_SOFT_STRAPS;
  MailboxCommand.Fields.Param1  = CpuStrapSetNum;
  MailboxCommand.Fields.Param2  = V_STRAP_RESOLVED_VALUE;
  MailboxData = 0;
  Status = MailboxRead (MailboxCommand.InterfaceData, (UINT32*) &MailboxData, &MailboxStatus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PMC: Failed to read CPU Soft Strap data. EFI_STATUS = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS) {
    *CpuStrapSetData = MailboxData;
  } else {
    *CpuStrapSetData = 0;
  }
  return EFI_SUCCESS;
}

/**
  Get 32-bit CPU Soft Strap data of multi sets.

  @param[out] CpuStrapSet1High  CPU strap Set1 High value
  @param[out] CpuStrapSet2Low   CPU strap Set2 Low value
  @param[out] CpuStrapSet2High  CPU strap Set2 High value
  @param[out] CpuStrapSet3Low   CPU strap Set3 Low value
  @param[out] CpuStrapSet3High  CPU strap Set3 High value

  @retval EFI_SUCCESS           Get CPU Soft Strap data successfully.
  @retval EFI_INVALID_PARAMETER All the parameters are NULL.
  @retval Others                Failed to get CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
GetCpuStrapData (
  OUT UINT32  *CpuStrapSet1High OPTIONAL,
  OUT UINT32  *CpuStrapSet2Low  OPTIONAL,
  OUT UINT32  *CpuStrapSet2High OPTIONAL,
  OUT UINT32  *CpuStrapSet3Low  OPTIONAL,
  OUT UINT32  *CpuStrapSet3High OPTIONAL
  )
{
  EFI_STATUS              Status;

  DEBUG ((DEBUG_INFO, "GetCpuStrapData ...\n"));

  if (CpuStrapSet1High == NULL && CpuStrapSet2Low == NULL && CpuStrapSet2High == NULL
        && CpuStrapSet3Low == NULL && CpuStrapSet3High == NULL) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  Status = GetCpuStrapDataBySet (CPU_SOFTSTRAP_SET1_HIGH, CpuStrapSet1High);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GetCpuStrapDataBySet (CPU_SOFTSTRAP_SET2_LOW,  CpuStrapSet2Low);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GetCpuStrapDataBySet (CPU_SOFTSTRAP_SET2_HIGH, CpuStrapSet2High);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GetCpuStrapDataBySet (CPU_SOFTSTRAP_SET3_LOW,  CpuStrapSet3Low);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = GetCpuStrapDataBySet (CPU_SOFTSTRAP_SET3_HIGH, CpuStrapSet3High);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Set 32-bit CPU Soft Strap data of multi sets.

  @param[in] CpuStrapSet1High  CPU strap Set1 High value
  @param[in] CpuStrapSet2Low   CPU strap Set2 Low value
  @param[in] CpuStrapSet2High  CPU strap Set2 High value
  @param[in] CpuStrapSet3Low   CPU strap Set3 Low value
  @param[in] CpuStrapSet3High  CPU strap Set3 High value

  @retval EFI_SUCCESS           Set CPU Soft Strap data successfully
  @retval EFI_INVALID_PARAMETER The parameters - CpuStrapSet1High, CpuStrapSet2Low, CpuStrapSet2High
                                and CpuStrapSet3Low are inconsistent(should be all NULL or non-NULL).
  @retval Others                Failed to set CPU Soft Strap data.
**/
EFI_STATUS
EFIAPI
SetCpuStrapData (
  IN  UINT32  *CpuStrapSet1High OPTIONAL,
  IN  UINT32  *CpuStrapSet2Low  OPTIONAL,
  IN  UINT32  *CpuStrapSet2High OPTIONAL,
  IN  UINT32  *CpuStrapSet3Low  OPTIONAL,
  IN  UINT32  *CpuStrapSet3High OPTIONAL
  )
{
  EFI_STATUS              Status;
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  DEBUG ((DEBUG_INFO, "SetCpuStrapData ...\n"));

  if ((CpuStrapSet1High != NULL || CpuStrapSet2Low != NULL || CpuStrapSet2High != NULL || CpuStrapSet3Low != NULL) &&
      (CpuStrapSet1High == NULL || CpuStrapSet2Low == NULL || CpuStrapSet2High == NULL || CpuStrapSet3Low == NULL)) {
    ASSERT_EFI_ERROR (EFI_INVALID_PARAMETER);
    return EFI_INVALID_PARAMETER;
  }

  if (CpuStrapSet1High != NULL) {
    Wbuf.Buf0 = *CpuStrapSet1High;
    Wbuf.Buf1 = *CpuStrapSet2Low;
    Wbuf.Buf2 = *CpuStrapSet2High;
    Wbuf.Buf3 = *CpuStrapSet3Low;

    Status = PmcSendCommand (
              PmcGetPwrmBase (),
              V_PMC_PWRM_IPC_CMD_COMMAND_CPU_STRAP,
              V_PMC_PWRM_IPC_CMD_CMD_ID_CPU_STRAP_WRITE_DW1_4,
              0, &Wbuf, NULL
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PMC: Failed to write CPU Soft Strap data DW1_4. EFI_STATUS = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  if (CpuStrapSet3High != NULL) {
    ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
    Wbuf.Buf0 = *CpuStrapSet3High;

    Status = PmcSendCommand (
              PmcGetPwrmBase (),
              V_PMC_PWRM_IPC_CMD_COMMAND_CPU_STRAP,
              V_PMC_PWRM_IPC_CMD_CMD_ID_CPU_STRAP_WRITE_DW5_8,
              0, &Wbuf, NULL
              );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "PMC: Failed to write CPU Soft Strap data DW5_8. EFI_STATUS = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function returns ratio

  @param[in]  ConfigTdpLevel  Config Tdp Level

  @retval intended ratio
**/
UINT8
MsrGetSelectCtdpRatio (
  IN UINT8 ConfigTdpLevel
  )
{
  MSR_CONFIG_TDP_NOMINAL_REGISTER       CtdpNominal;
  MSR_CONFIG_TDP_LEVEL1_REGISTER        CtdpLevel1Msr;
  MSR_CONFIG_TDP_LEVEL2_REGISTER        CtdpLevel2Msr;

  if (ConfigTdpLevel == 0) {
    CtdpNominal.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_NOMINAL);
    return (UINT8) CtdpNominal.Bits.TdpRatio;
  }

  if (ConfigTdpLevel == 1) {
    CtdpLevel1Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL1);
    return (UINT8) CtdpLevel1Msr.Bits.TdpRatio;
  }
  if (ConfigTdpLevel == 2) {
    CtdpLevel2Msr.Uint64 = AsmReadMsr64 (MSR_CONFIG_TDP_LEVEL2);
    return (UINT8) CtdpLevel2Msr.Bits.TdpRatio;
  }
  return 0;
}

/**
  Get Processor Power Unit in Watts

  @retval Processpr power unit
**/
UINT8
EFIAPI
GetPowerUnit (
  VOID
  )
{
  UINT8    ProcessorPowerUnit;
  MSR_PACKAGE_POWER_SKU_UNIT_REGISTER   PackagePowerSkuUnitMsr;

  PackagePowerSkuUnitMsr.Uint64  = AsmReadMsr64 (MSR_PACKAGE_POWER_SKU_UNIT);
  ProcessorPowerUnit = (UINT8) PackagePowerSkuUnitMsr.Bits.PwrUnit;
  if (ProcessorPowerUnit == 0) {
    ProcessorPowerUnit = 1;
  } else {
    ProcessorPowerUnit = (UINT8) LShiftU64 (2, (ProcessorPowerUnit - 1));
  }

  return ProcessorPowerUnit;
}

/**
  Function to compare 2 MP_INFORMATION2_HOB_DATA pointer based on ProcessorIndex.
  @param[in] Buffer1            pointer to MP_INFORMATION2_HOB_DATA poiner to compare
  @param[in] Buffer2            pointer to second MP_INFORMATION2_HOB_DATA pointer to compare
  @retval 0                     Buffer1 equal to Buffer2
  @retval <0                    Buffer1 is less than Buffer2
  @retval >0                    Buffer1 is greater than Buffer2
**/
INTN
EFIAPI
CompareMpInformation2Hob (
  IN  CONST VOID  *Buffer1,
  IN  CONST VOID  *Buffer2
  )
{
  return (*(MP_INFORMATION2_HOB_DATA **)Buffer1)->ProcessorIndex - (*(MP_INFORMATION2_HOB_DATA **)Buffer2)->ProcessorIndex;
}

/**
  Get processor information and core type.
  This function retrieves processor information and core type for all processors
  in the system. This information is obtained from the HOB characterized by
  gMpInformation2HobGuid.
  @param[in out] ProcessorInfo            A pointer to an array of EFI_PROCESSOR_INFORMATION structures.
                                          One entry for each processor.
  @param[in out] CoreType                 A pointer to an array of CoreType.
                                          One entry for each processor.
  @param[in]    NumberOfProcessors        Number Of Processors
**/
VOID
EFIAPI
GetProcessorInfoAndCoreType (
  IN OUT  EFI_PROCESSOR_INFORMATION  *ProcessorInfo,
  IN OUT  UINT8                      *CoreType,
  IN      UINTN                      NumberOfProcessors
  )
{
  EFI_HOB_GUID_TYPE         *GuidHob;
  MP_INFORMATION2_HOB_DATA  *MpInformation2HobData;
  UINTN                     HobCount;
  UINTN                     HobIndex;
  UINTN                     ProcessorIndex;
  UINT64                    PrevProcessorIndex;
  MP_INFORMATION2_ENTRY     *MpInformation2Entry;
  MP_INFORMATION2_HOB_DATA  **MpInfo2Hobs;
  UINTN                     SortBuffer;
  UINTN                     CountedNumberOfProcessors;

  GuidHob                   = NULL;
  MpInformation2HobData     = NULL;
  HobIndex                  = 0;
  HobCount                  = 0;
  CountedNumberOfProcessors = 0;
  GuidHob                   = GetFirstGuidHob (&gMpInformation2HobGuid);

  if ((CoreType == NULL) && (ProcessorInfo == NULL)) {
    return;
  }
  ASSERT (GuidHob != NULL);

  // Count the number of processors and HOBs
  for (HobCount = 0; GuidHob != NULL; HobCount++) {
    MpInformation2HobData      = GET_GUID_HOB_DATA (GuidHob);
    CountedNumberOfProcessors += MpInformation2HobData->NumberOfProcessors;
    //
    // This is the last MpInformationHob in the HOB list.
    //
    if (MpInformation2HobData->NumberOfProcessors == 0) {
      ASSERT (HobCount != 0);
      break;
    }

    GuidHob = GetNextGuidHob (&gMpInformation2HobGuid, GET_NEXT_HOB (GuidHob));
  }

  ASSERT (CountedNumberOfProcessors == NumberOfProcessors);

  MpInfo2Hobs = AllocatePool (sizeof (MP_INFORMATION2_HOB_DATA *) * HobCount);
  ASSERT (MpInfo2Hobs != NULL);
  if (MpInfo2Hobs == NULL) {
    return;
  }

  GuidHob = GetFirstGuidHob (&gMpInformation2HobGuid);
  while (HobIndex < HobCount) {
    MpInfo2Hobs[HobIndex++] = GET_GUID_HOB_DATA (GuidHob);
    GuidHob                 = GetNextGuidHob (&gMpInformation2HobGuid, GET_NEXT_HOB (GuidHob));
  }

  // Sort MpInfo2Hobs
  QuickSort (MpInfo2Hobs, HobCount, sizeof (MP_INFORMATION2_HOB_DATA *), (BASE_SORT_COMPARE)CompareMpInformation2Hob, &SortBuffer);

  PrevProcessorIndex = 0;
  for (HobIndex = 0; HobIndex < HobCount; HobIndex++) {
    //
    // Make sure no overlap and no gap in the CPU range covered by each HOB
    //
    ASSERT (MpInfo2Hobs[HobIndex]->ProcessorIndex == PrevProcessorIndex);

    //
    // Cache each EFI_PROCESSOR_INFORMATION in order.
    //
    for (ProcessorIndex = 0; ProcessorIndex < MpInfo2Hobs[HobIndex]->NumberOfProcessors; ProcessorIndex++) {
      MpInformation2Entry = GET_MP_INFORMATION_ENTRY (MpInfo2Hobs[HobIndex], ProcessorIndex);
      if (ProcessorInfo != NULL) {
        CopyMem (
          &ProcessorInfo[PrevProcessorIndex + ProcessorIndex],
          &MpInformation2Entry->ProcessorInfo,
          sizeof (EFI_PROCESSOR_INFORMATION)
          );
      }

      if (CoreType != NULL) {
        CoreType[PrevProcessorIndex + ProcessorIndex] = MpInformation2Entry->CoreType;
      }
    }

    PrevProcessorIndex += MpInfo2Hobs[HobIndex]->NumberOfProcessors;
  }

  FreePool (MpInfo2Hobs);
}
