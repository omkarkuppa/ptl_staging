/** @file
  This file contains internal Fusa routines for Fusa lib
  usage

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/SaRegsHostBridge.h>
#include <Register/Intel/ArchitecturalMsr.h>
#include <Library/IGpuInfoLib.h>

#include "PeiFusaPrivateLibInternal.h"
#include "PeiFusaResultReportingLib.h"

/**
  Enable MCA on the targetted bank index for error reporting
  purpose and clear the rest. if TargetMcaBankNum is 0xFF
  (outside of the supported MCA bank count), then effectively
  disable all MCA reporting

  @param[in] TargetMcaBankNum - Target MCA bank number
**/
VOID
McaReportingEnable(
  IN UINT32 TargetMcaBankNum
  )
{
  MSR_IA32_MCG_CAP_REGISTER  McgCap;
  UINT32 BankIndex;

  McgCap.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
  for (BankIndex = 0; BankIndex < (UINT32) McgCap.Bits.Count; BankIndex++) {
    if (TargetMcaBankNum == BankIndex) {
      AsmWriteMsr64 (MSR_IA32_MC0_CTL + BankIndex * 4, MAX_UINT64);
    } else {
      AsmWriteMsr64 (MSR_IA32_MC0_CTL + BankIndex * 4, 0ULL);
    }
  }
}

/**
  Read the related MCA bank status and compare against the
  expected McaStatus values (auto check for valid bit)

  @param[in] McaBankNum - MCA bank number
  @param[in] McaStatusMask - Mask value to be applied on
        McaStatus before comparison
  @param[in] ExpectedMcaStatus - Expected McaStatus value after
        masked.

  @retval  FUSA_TEST_PASS - if matches expected values
  @retval  FUSA_TEST_FAIL - if not matches expected values
**/

UINT8
McaBankStatusCheck(
  IN UINT8  McaBankNum,
  IN UINT64 McaStatusMask,
  IN UINT64 ExpectedMcaStatus
  )
{
  UINT8 CheckResult;
  UINT64 McaStatus;

  McaStatus = AsmReadMsr64 (MSR_IA32_MC0_STATUS + McaBankNum * 4);

  if ((McaStatus & (McaStatusMask | MCASTATUS_VALID)) == (ExpectedMcaStatus | MCASTATUS_VALID)) {
    CheckResult = FUSA_TEST_PASS;
  } else {
    CheckResult = FUSA_TEST_FAIL;
  }

  return CheckResult;
}

/**
  Clear all MCA bank status
**/
VOID
McaBankStatusClearAll(
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER  McgCap;
  UINT32 McaBankNum;

  McgCap.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
  for (McaBankNum = 0; McaBankNum < (UINT32) McgCap.Bits.Count; McaBankNum++) {
      AsmWriteMsr64 (MSR_IA32_MC0_STATUS + McaBankNum * 4, 0ULL);
  }
}

/**
  Dump all MCA bank status in debug build
**/
VOID
McaBankStatusDumpAll(
  VOID
  )
{
  MSR_IA32_MCG_CAP_REGISTER  McgCap;
  UINT32 McaBankNum;

  McgCap.Uint64 = AsmReadMsr64 (MSR_IA32_MCG_CAP);
  for (McaBankNum = 0; McaBankNum < (UINT32) McgCap.Bits.Count; McaBankNum++) {
      DEBUG ((DEBUG_INFO, "MCA status for bank %d is is 0x%lx\n",
              McaBankNum,
              AsmReadMsr64 (MSR_IA32_MC0_STATUS + McaBankNum * 4)));
  }
}

/**
  Clear the related MCA bank status, address and misc details

  @param[in] McaBankNum - MCA bank number
**/
VOID
McaBankInfoClear(
  IN UINT8 McaBankNum
  )
{
  AsmWriteMsr64 (MSR_IA32_MC0_STATUS + McaBankNum * 4, 0ULL);
  AsmWriteMsr64 (MSR_IA32_MC0_ADDR + McaBankNum * 4, 0ULL);
  AsmWriteMsr64 (MSR_IA32_MC0_MISC + McaBankNum * 4, 0ULL);
}

/**
  Dump the related MCA bank status, address and misc details in
  debug build.

  @param[in] McaBankNum - MCA bank number
**/
VOID
McaBankInfoDump(
  IN UINT8 McaBankNum
  )
{
  DEBUG_CODE_BEGIN();
    UINT64 Value64 = AsmReadMsr64 (MSR_IA32_MC0_STATUS + McaBankNum * 4);
    DEBUG ((DEBUG_INFO, "MCA status for bank %d is is 0x%lx\n", McaBankNum, Value64));
    Value64 = AsmReadMsr64 (MSR_IA32_MC0_ADDR + McaBankNum * 4);
    DEBUG ((DEBUG_INFO, "MCA address for bank %d is is 0x%lx\n", McaBankNum, Value64));
    Value64 = AsmReadMsr64 (MSR_IA32_MC0_MISC + McaBankNum * 4);
    DEBUG ((DEBUG_INFO, "MCA misc for bank %d is is 0x%lx\n", McaBankNum, Value64));
  DEBUG_CODE_END();
}

/**
  Write the parity error injection control payload into the Fusa
  IP indexed by the IpIndex

  @param[in] IpIndex - IP Index for the Fusa MSR for the error
        injection
  @param[in] InjectionControlPayload - Injection control payload
        for the error injection

  @note Once the Fusa MSR is used, the ucode will disable the
        prefetcher for preventing from messing with the CTC
        mechanism. This step will not be undone because it is
        not expected the system to boot to OS without a reset
        for the CTC path.

**/
VOID
FuSaMsrWrite (
  IN UINT32 IpIndex,
  IN UINT32 InjectionControlPayload
  )
{
  AsmWriteMsr32 (MSR_PERRINJ_AT_IP, IpIndex | BIT31);
  AsmWriteMsr32 (MSR_PERRINJ_CTRL, InjectionControlPayload);
}

/**
  Read the parity error injection control payload status from
  the Fusa IP indexed by the IpIndex

  @param[in] IpIndex - IP Index for the Fusa MSR for the error
        injection

  @retval  Parity error injection control payload status
**/
UINT32
FuSaMsrRead (
  IN UINT32 IpIndex
  )
{
  AsmWriteMsr32 (MSR_PERRINJ_AT_IP, IpIndex | BIT31);
  return AsmReadMsr32 (MSR_PERRINJ_CTRL);
}

/**
  Get MCH Base address

  @param  MchBaseAddress - MCH Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
MchBarAddressGet (
  OUT UINTN *MchBaseAddress
  )
{
  UINT64          McD0BaseAddress;
  UINT64          MchBar;
  FUSA_LIB_STATUS LibStatus;

  McD0BaseAddress    = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  PciSegmentReadBuffer (McD0BaseAddress + R_SA_MCHBAR, sizeof (MchBar), &MchBar);
  DEBUG ((DEBUG_INFO, "MchBar is 0x%lx\n", MchBar));
  if (MchBar & BIT0) {
    MchBar &= ((UINT64) ~BIT0);
    *MchBaseAddress = (UINTN) MchBar;

    if (0ULL == MchBar) {
      LibStatus = FusaDeviceError;
    }
    //32 bit address mode cannot perform 64 bit MMIO addressing that exceeds 4G
    else if ((sizeof(UINTN) < sizeof(UINT64))
              && MchBar >= 0x100000000ULL )
    {
      LibStatus = FusaDeviceError;
    } else {
      LibStatus = FusaNoError;
    }
  } else {
    LibStatus = FusaDeviceError; //MchBar is not enabled
  }

  return LibStatus;
}

/**
  Get DMI Base address

  @param  DmiBaseAddress - DMI Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
DmiBarAddressGet (
  OUT UINTN *DmiBaseAddress
  )
{
  UINT64          McD0BaseAddress;
  UINT64          DmiBar;
  FUSA_LIB_STATUS LibStatus;

  McD0BaseAddress    = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 0, 0, 0);
  PciSegmentReadBuffer (McD0BaseAddress + R_SA_DMIBAR, sizeof (DmiBar), &DmiBar);
  DEBUG ((DEBUG_INFO, "DmiBar is 0x%lx\n", DmiBar));
  if (DmiBar & BIT0) {
    DmiBar &= ((UINT64) ~BIT0);
    *DmiBaseAddress = (UINTN) DmiBar;

    if (0ULL == DmiBar) {
      LibStatus = FusaDeviceError;
    }
    //32 bit address mode cannot perform 64 bit MMIO addressing that exceeds 4G
    else if ((sizeof(UINTN) < sizeof(UINT64))
              && DmiBar >= 0x100000000ULL )
    {
      LibStatus = FusaDeviceError;
    } else {
      LibStatus = FusaNoError;
    }
  } else {
    LibStatus = FusaDeviceError; //DmiBar is not enabled
  }

  return LibStatus;
}

/**
  Get GTTMM Base address

  @param  GttMmBaseAdr - GTTMM Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
GttMmBarAddressGet (
  OUT UINTN *GttMmBaseAdr
  )
{
  UINT64          McD2BaseAddress;
  UINT64          GttMmAdr;
  FUSA_LIB_STATUS LibStatus;

  GttMmAdr = IGpuGetGttMmAdrBase ();
  DEBUG ((DEBUG_INFO, "GttMmAdr is 0x%lx\n", GttMmAdr));
  

  if (0ULL == GttMmAdr) {
    LibStatus = FusaDeviceError;
  } else {
    LibStatus = FusaNoError;
  }

  return LibStatus;
}

/**
  Get GM Base address

  @param  GmBaseAdr - GM Base address

  @retval  fusaDeviceError - for unexpected address value
  @retval  fusaNoError - no error otherwise
**/
FUSA_LIB_STATUS
GmBarAddressGet (
  OUT UINTN *GmBaseAdr
  )
{
  UINT64          McD2BaseAddress;
  UINT64          GmAdr;
  UINT32          Cmd;
  FUSA_LIB_STATUS LibStatus;

  GmAdr = IGpuGetLMemBar ();
  DEBUG ((DEBUG_INFO, "GmAdr is 0x%lx\n", GmAdr));

  if (0ULL == GmAdr) {
    LibStatus = FusaDeviceError;
  } else {
    LibStatus = FusaNoError;
  }

  return LibStatus;
}

/**
  Get PCH Device D31 F0 Base address

  @param  PchD31F0BaseAddress - PCH Device D31 F0 Base address

  @note   VOID return because no error possible
**/
VOID
PchD31F0BaseAddressGet (
  OUT UINTN *PchD31F0BaseAddress
  )
{
  *PchD31F0BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, 31, 0, 0);
}

/**
  Generic error injection flow using Fusa MSR

  @param[in] IpIndexInput - IP Index for the Fusa MSR for the
        error injection
  @param[in] InjectionPayload - Injection payload for the error
      injection
  @param[in] InjectionAndTraffic - function pointer for
        performing injection and related traffic
  @param[in] McaBankNum - The MCA Bank that reflect the test
        status
  @param[in] McaStatusMask - MCA status mask to be applied
        before comparing for the ExpectedMcaStatus
  @param[in] ExpectedMcaStatus - Expected MCA status value
  @param[in] CheckNum - check number assigned to this check
  @param[in,out] pFusaTestResult - pointer to the test result
        buffer for result to be updated
  @param[in] DebugFlag - to control the debug print. Main
        purpose is to allow AP to avoid the debug print that
        would cause hang.

  @retval  lib routine calling status from UpdateResults

  @note - combine injection and traffic in one routine so that
        there is not done through function calling and return in
        which the error itself may mess up with function return
        or calling.
**/
FUSA_LIB_STATUS
FusaMsrCtcTestGeneric (
  IN UINT32                       IpIndexInput,
  IN UINT32                       InjectionPayload,
  IN FUSA_INJECTION_TRAFFIC_FUNC  InjectionAndTraffic,
  IN UINT8                        McaBankNum,
  IN UINT64                       McaStatusMask,
  IN UINT64                       ExpectedMcaStatus,
  IN UINT8                        CheckNum,
  IN OUT FUSA_TEST_RESULT         *pFusaTestResult,
  IN BOOLEAN                      DebugFlag
  )
{
  UINT32 Data32;
  UINT32 Count;
  UINT8 CheckResult;
  FUSA_LIB_STATUS LibStatus;
  UINT32 IpIndex = IpIndexInput | BIT31; //make sure BIT31 is set

  //effectively disable all MCA reporting because target 0xFF does not exist,
  //it does not prevent the MCA status from reflecting the error routed to MCA
  McaReportingEnable (0xFF);
  McaBankInfoClear (McaBankNum);

  Data32 = InjectionAndTraffic (IpIndex, InjectionPayload);

  Count = 1;
  while (Data32 == InjectionPayload
         && Count <= 10U) //seen one time needed for the core related test
  {
    Data32 = FuSaMsrRead (IpIndex);
    Count++;
  }

  CheckResult = McaBankStatusCheck(McaBankNum, McaStatusMask, ExpectedMcaStatus);
  if (FUSA_TEST_FAIL == CheckResult) {
    FuSaMsrWrite (IpIndex, 0U);
  }

  LibStatus = UpdateResults(CheckResult, CheckNum, pFusaTestResult);

  if (DebugFlag) {
    DEBUG ((DEBUG_INFO, "Repeat %d times reading the contol, its value is 0x%x\n", Count, Data32));
    McaBankInfoDump (McaBankNum);
  }

  McaBankInfoClear (McaBankNum);

  return LibStatus;
}

