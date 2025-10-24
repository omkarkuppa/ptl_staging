/** @file
    Initilize TSE in PEI

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

#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Register/TseIocRegs.h>
#include <Register/TseIocceRegs.h>
#include <Ppi/SiPolicy.h>
#include <Library/TseInfoLib.h>
#include <Library/TseDataHob.h>
#include <Library/PeiTseLib.h>
#include <TseCpuId.h>

EFI_STATUS
EFIAPI
TseDramAllocationPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

//
// Notify Descriptor used to register a callback
//
static EFI_PEI_NOTIFY_DESCRIPTOR  mPpiPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  TseDramAllocationPpiNotifyCallback
};

/**
  This function will be called when MRC is done and the TSE DRAM memory has
  been carved from the memory map and IMRs have been programmed.

  The reserved TSE DRAM base will be programmed into the IOCCE MMIO

  @param  PeiServices General purpose services available to every PEIM.

  @param  NotifyDescriptor Information about the notify event..

  @param  Ppi The notify context.

  @retval EFI_SUCCESS If the function completed successfully.
  @retval EFI_NOT_FOUND If the TSE_DATA_HOB was not located.
**/
EFI_STATUS
EFIAPI
TseDramAllocationPpiNotifyCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "TSE DRAM Table configuration.\n"));

  UINT64        MchBar = 0;
  UINT64        TseDramMemBaseAddr = 0;
  TSE_DATA_HOB  *TseDataHobPtr = NULL;
  TSE_TABLE_OFFSET_MCHBAR_STRUCT TseTableOffsetStruct;

  ZeroMem (&TseTableOffsetStruct, sizeof (TSE_TABLE_OFFSET_MCHBAR_STRUCT));

  TseDataHobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);
  if (TseDataHobPtr == NULL) {
    return EFI_NOT_FOUND;
  }

  TseDramMemBaseAddr = (TseDataHobPtr->TseDramMemBase) << 20;  // Convert DRAM Address from MB to B units
  DEBUG ((DEBUG_INFO, "TseDramMemBaseAddr: 0x%016llX\n", TseDramMemBaseAddr & V_TSE_TABLE_OFFSET_MASK));

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  TseTableOffsetStruct.Data = MmioRead64 (MchBar + R_IOCCE_TSE_BASE + R_IOCCE_TSE_TABLE_OFFSET_MCHBAR);
  DEBUG ((DEBUG_INFO, "IOCCE TSE Table offset before programming = 0x%016llX \n", TseTableOffsetStruct.Data));

  TseTableOffsetStruct.Bits.TseOffset = RShiftU64 ((TseDramMemBaseAddr & V_TSE_TABLE_OFFSET_MASK), 12);

  MmioWrite64 ((MchBar + R_IOCCE_TSE_BASE + R_IOCCE_TSE_TABLE_OFFSET_MCHBAR), TseTableOffsetStruct.Data);
  TseTableOffsetStruct.Data = MmioRead64 (MchBar + R_IOCCE_TSE_BASE + R_IOCCE_TSE_TABLE_OFFSET_MCHBAR);
  DEBUG ((DEBUG_INFO, "IOCCE TSE Table offset after programming = 0x%016llX \n", TseTableOffsetStruct.Data));

  return EFI_SUCCESS;
}

/**
   Create TSE DATA HOB

   @param[out] TseDataHobPtr  - Pointer to the TSE Data HOB structure

   @retval  EFI_SUCCESS  TseDataHobPtr has been initialized successfully.
   @retval  Others       HOB creation failed.
**/
EFI_STATUS
TseHobInit (
  OUT TSE_DATA_HOB **TseDataHobPtr
  )
{
  TSE_DATA_HOB *HobPtr;
  EFI_STATUS    Status;

  Status = EFI_SUCCESS;
  *TseDataHobPtr = NULL;

  DEBUG ((DEBUG_INFO, "Total Storage Encryption (TSE) HOB Initialization\n"));

  //
  // Get TseDataHob, if it is already present, reload the hob and exit
  //
  HobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);
  if (HobPtr != NULL) {
    *TseDataHobPtr = HobPtr;
    return Status;
  }

  //
  // Create hob for storing TSE data
  //
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TSE_DATA_HOB),
             (VOID **) &HobPtr
             );

  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  ZeroMem (&(HobPtr->Revision), sizeof (TSE_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  HobPtr->EfiHobGuidType.Name = gTseDataHobGuid;
  HobPtr->Revision = TSE_DATA_HOB_REVISION;
  DEBUG ((DEBUG_INFO, "TseDataHob->EfiHobGuidType.Name: %g\n", &HobPtr->EfiHobGuidType.Name));

  *TseDataHobPtr = HobPtr;
  return Status;
}

/**
   Reads the TSE FIPS self test status register and return if a failure is reported.

   If the Self-test fuse is enabled, this function will read and return if a failure status
   is reported.

   If the Self-test fuse is disabled, this function will always return FALSE.

   @retval TRUE              If the TSE FIPS self test reports a failed status.
   @retval FALSE             If the TSE FIPS self test reports a passing status OR the self test fuse is disabled.
**/
BOOLEAN
TseSelfTestFailed (
  VOID
  )
{
  UINT64 MchBar = 0;
  TSE_IOCCE_TSE_FIPS_SELF_TEST_STS_MCHBAR_STRUCT TseFipsSelfTestStsStruct;

  ZeroMem (&TseFipsSelfTestStsStruct, sizeof (TSE_IOCCE_TSE_FIPS_SELF_TEST_STS_MCHBAR_STRUCT));

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  TseFipsSelfTestStsStruct.Data = MmioRead32 (MchBar + R_IOCCE_TSE_BASE + B_IOCCE_TSE_SELF_TEST_STS);

  DEBUG ((DEBUG_INFO, "IOCCE TSE Fuse_SelfTest_EN: %x\n", TseFipsSelfTestStsStruct.Bits.Fuse_SelfTest_EN));
  DEBUG ((DEBUG_INFO, "IOCCE TSE XTS_AES_SELFTEST_PASS: %x\n", TseFipsSelfTestStsStruct.Bits.XTS_AES_SELFTEST_PASS));
  DEBUG ((DEBUG_INFO, "IOCCE TSE XTS_AES_SELFTEST_FAIL: %x\n", TseFipsSelfTestStsStruct.Bits.XTS_AES_SELFTEST_FAIL));

  if (TseFipsSelfTestStsStruct.Bits.Fuse_SelfTest_EN == FALSE) {
    return FALSE;
  }

  if ((TseFipsSelfTestStsStruct.Bits.XTS_AES_SELFTEST_PASS == TRUE) &&
      (TseFipsSelfTestStsStruct.Bits.XTS_AES_SELFTEST_FAIL == FALSE)) {
    return FALSE;
  }

  return TRUE;
}

/**
   Perform Total Storage Encryption initialization.

   This function needs to be called once the TseDataHob has been created and populated with the
   TSE policy and HW capabilities state.

   @param[in] CpuSecurityPreMemConfig Pointer to CPU_SECURITY_PREMEM_CONFIG instance

   @retval EFI_SUCCESS       The TSE initialization completed successfully.
   @retval EFI_NOT_FOUND     The TSE HOB was not found, initialization can not be performed.
**/
EFI_STATUS
EFIAPI
TseInit (
  IN CPU_SECURITY_PREMEM_CONFIG *CpuSecurityPreMemConfig
  )
{
  EFI_STATUS                  Status = EFI_SUCCESS;
  TSE_DATA_HOB                *TseDataHobPtr;
  UINT64                      TseCapabilityMsr        = 0;
  BOOLEAN                     TseCapabilityMsrPresent = FALSE;
  BOOLEAN                     TsePconfigPresent       = FALSE;
  BOOLEAN                     TseSupported            = FALSE;
  BOOLEAN                     TseEnabled              = FALSE;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  TseDataHobPtr = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);

  //
  // Get TseDataHob if it is already present, then exit.
  //
  if (TseDataHobPtr != NULL) {
    return EFI_SUCCESS;
  }

  if (IsTseCapabilityPresent () == FALSE) {
    DEBUG ((DEBUG_INFO, "IA32_TSE_CAPABILITY MSR not present\n"));
  } else {
    TseCapabilityMsrPresent = TRUE;
  }

  if (IsTsePconfigSupported () == FALSE) {
    DEBUG ((DEBUG_INFO, "PCONFIG ISA support not present\n"));
  } else {
    TsePconfigPresent = TRUE;
  }

  TseEnabled = CpuSecurityPreMemConfig->TseEnable == 1 ? TRUE : FALSE;
  DEBUG ((DEBUG_INFO, "CpuSecurityPreMemConfig->TseEnable: %s\n", (TseEnabled == TRUE) ? (L"TRUE") : (L"FALSE")));

  Status = TseHobInit (
             &TseDataHobPtr
             );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Error creating TSE HOB, status = %r\n", Status));
    return Status;
  }

  //
  // If HW supports TSE, then enable based on BIOS TSE Policy value
  //
  if ((TseCapabilityMsrPresent == TRUE) &&
      (TsePconfigPresent == TRUE)) {
    TseCapabilityMsr = AsmReadMsr64 (MSR_TSE_CAPABILITY);
    DEBUG ((DEBUG_INFO, "MSR_TSE_CAPABILITY: 0x%016llx\n", TseCapabilityMsr));

    if (IsAesXts256Supported () == TRUE) {
      TseSupported = TRUE;
    } else {
      DEBUG ((DEBUG_INFO, "AES-XTS not supported. Skipping TSE flows!\n"));
    }
  } else {
    DEBUG ((DEBUG_WARN, "Not supported by HW. Forcing TSE to be disabled!\n"));
  }

  //
  // Check TSE self test result, if failure is reported force disable TSE.
  //
  if (TseSelfTestFailed () == TRUE) {
    DEBUG ((DEBUG_ERROR, "Self Test failure reported. Forcing TSE to be disabled!\n"));
    TseEnabled = FALSE;
  }

  //
  // Update HOB values
  //
  TseDataHobPtr->TseSupported = TseSupported;
  TseDataHobPtr->TseEnabled = TseEnabled;

  DEBUG ((DEBUG_INFO, "TseSupported: 0x%X\n", TseDataHobPtr->TseSupported));
  DEBUG ((DEBUG_INFO, "TseEnabled: 0x%X\n", TseDataHobPtr->TseEnabled));

  //
  // Install callback to program TSE DRAM table on Memory Discovered event
  //
  if ((TseSupported == TRUE) &&
      (TseEnabled == TRUE)) {
    Status = PeiServicesNotifyPpi (&mPpiPeiNotifyList);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot register TseDramAllocationPpiNotifyCallback, status = %r\n", Status));
    }
  }

  return Status;
}

