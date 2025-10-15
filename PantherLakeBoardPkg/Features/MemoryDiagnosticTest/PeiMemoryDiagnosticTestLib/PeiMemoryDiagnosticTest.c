/** @file
  PEI Driver to Perform Memory Diagnostic Test for TSEG Region

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <SetupVariable.h>
#include <Setup.h>
#include <Library/PcdLib.h>
#include <Ppi/SiPolicy.h>
#include <CmosMap.h>
#include <Library/CmosAccessLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Include/BootStateLib.h>
#include <TishDataHob.h>
#include <MemoryConfig.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

#define PASS  0
#define FAIL  1

#define B_SA_TSEGMB_TSEGMB_MASK  (0xfff00000)

///
/// Used in memory test routines
///
#define  MEMORY_TEST_COVER_SPAN  0x40000
#define  MEMORY_TEST_PATTERN     0x5A5A5A5A

typedef enum {
  Ignore,
  Quick,
  Sparse,
  Extensive,
  MemTestMax
} MEMORY_TEST_LEVEL;

/**
  Installs Memory Test Info Structure Data Hob
  @param[in] UINT8      Ppr Recovery Status Enable
  @param[in] UINT8      Tseg Reallocate Enable Count

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallTishHob (
  IN UINT8  TsegReallocateEnableCount
  );

/**
  Get the BIOS Setup Menu Data

  @param[out] SetupData      Return the BIOS Setup Menu Data

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
GetBiosSetupMenuData (
  OUT SETUP_DATA  *SetupData
  );

/**
  This function checks the memory range.

  @param[in]  MrcData           Pointer to the MRC global data structure
  @param[in]  BeginAddress      Beginning of the memory address to be checked.
  @param[in]  MemoryLength      Bytes of memory range to be checked.
  @param[in]  MemoryTestLevel   Type of memory check level to be performed.
  @param[out] ErrorAddress      Return the address of the error memory address.

  @retval     EFI_SUCCESS       The operation completed successfully.
  @retval     EFI_DEVICE_ERROR  Memory test failed. It's not reliable to use this range of memory.
**/
EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN       EFI_PHYSICAL_ADDRESS  BeginAddress,
  IN       UINT64                MemoryLength,
  IN       MEMORY_TEST_LEVEL     MemoryTestLevel,
  OUT      EFI_PHYSICAL_ADDRESS  *ErrorAddress
  );

/**
  Perform Tseg Memory Test

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PerformTsegRegionMemoryTest (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR  mTsegMemoryTestNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiTsegMemoryTestPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT)PerformTsegRegionMemoryTest
};

/**
  This function checks the memory range.

  @param[in]  MrcData           Pointer to the MRC global data structure
  @param[in]  BeginAddress      Beginning of the memory address to be checked.
  @param[in]  MemoryLength      Bytes of memory range to be checked.
  @param[in]  MemoryTestLevel   Type of memory check level to be performed.
  @param[out] ErrorAddress      Return the address of the error memory address.

  @retval     EFI_SUCCESS       The operation completed successfully.
  @retval     EFI_DEVICE_ERROR  Memory test failed. It's not reliable to use this range of memory.
**/
EFI_STATUS
EFIAPI
BaseMemoryTest (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN       EFI_PHYSICAL_ADDRESS  BeginAddress,
  IN       UINT64                MemoryLength,
  IN       MEMORY_TEST_LEVEL     MemoryTestLevel,
  OUT      EFI_PHYSICAL_ADDRESS  *ErrorAddress
  )
{
  UINT32                TestPattern;
  UINT32                SpanSize;
  EFI_PHYSICAL_ADDRESS  TempAddress;

  TestPattern = MEMORY_TEST_PATTERN;
  SpanSize    = 0;

  //
  // Make sure we don't try and test anything above the max physical address range
  //
  ASSERT ((BeginAddress + MemoryLength) < MAX_ADDRESS);

  switch (MemoryTestLevel) {
    case Extensive:
      SpanSize = 0x4;
      break;
    case Sparse:
    case Quick:
      SpanSize = MEMORY_TEST_COVER_SPAN;
      break;
    case Ignore:
      goto Done;
      break;
    default:
      DEBUG ((DEBUG_ERROR, "Make sure we don't try and test anything above the max physical address range\n"));
      ASSERT (FALSE);
      break;
  }

  DEBUG ((DEBUG_INFO, "Base memory test started.\n"));

  //
  // Write the test pattern into memory range
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    (*(UINT32 *)(UINTN)TempAddress) = TestPattern;
    TempAddress                    += SpanSize;
  }

  //
  // Read pattern from memory and compare it
  //
  TempAddress = BeginAddress;
  while (TempAddress < BeginAddress + MemoryLength) {
    if ((*(UINT32 *)(UINTN)TempAddress) != TestPattern) {
      *ErrorAddress = TempAddress;
      DEBUG ((DEBUG_ERROR, "!!! Base memory test FAILED !!!\n"));
      return EFI_DEVICE_ERROR;
    }

    TempAddress += SpanSize;
  }

Done:
  return EFI_SUCCESS;
}

/**
  Perform Tseg Memory Test

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
PerformTsegRegionMemoryTest (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  TsegBaseAddress;
  TISH_CONFIG_HOB       *TishDataHob;
  UINT32                TsegSize;
  MEMORY_TEST_LEVEL     MemoryTestLevel;
  EFI_PHYSICAL_ADDRESS  BadMemoryAddress;
  UINT8                 RetryCount;
  UINT8                 MemoryTestInfo;
  MEMORY_CONFIG_NO_CRC  *MemConfigNoCrc;
  SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi;

  DEBUG ((DEBUG_INFO, "PerformTsegRegionMemoryTest Start\n"));
  SiPreMemPolicyPpi = NULL;
  Status            = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **)&SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || (SiPreMemPolicyPpi == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  MemConfigNoCrc    = NULL;
  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *)&MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status) || (MemConfigNoCrc == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // Get TSEG Region Memory Address from TSEG Register TSEGMB_HOSTBRIDGE_CFG_REG(0xb8)
  //
  TsegBaseAddress = (EFI_PHYSICAL_ADDRESS)GetHostBridgeRegisterData (HostBridgeTsegMb, HostBridgeTsegMbBase);
  DEBUG ((DEBUG_INFO, "TsegBaseAddress = 0x%x\n", TsegBaseAddress));
  TsegSize         = PcdGet32 (PcdTsegSize);
  MemoryTestLevel  = Quick;
  BadMemoryAddress = 0;
  RetryCount       = 0;
  MemoryTestInfo   = 0;

  //
  // Perform TSEG memory test
  //
  Status = BaseMemoryTest (
             PeiServices,
             TsegBaseAddress,
             TsegSize - 1,
             MemoryTestLevel,
             &BadMemoryAddress
             );
  //
  // Update the TISH_Data_HOB with TsegRegionAddress and TsegMemFailureAddress
  //
  TishDataHob = (TISH_CONFIG_HOB *)GetFirstGuidHob (&gTishDataHobGuid);
  if (TishDataHob != NULL) {
    // Update Mrc PPR Status BIT3 in CMOS_MEM_TEST_INFO_OFFSET(0x94)
    if (TishDataHob->PprRecoveryStatusEnable == 1) {
      MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
      MemoryTestInfo &= 0xF7;
      MemoryTestInfo |= ((MemConfigNoCrc->MrcPprStatus & 0x1) << 3);
      CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
    }

    if (TishDataHob->TsegReallocateEnableCount >= 1) {
      if (((!EFI_ERROR (Status)) && (TishDataHob->RetryCount <= 3) && (TishDataHob->TsegReallocateEnableCount == TishDataHob->RetryCount)) ||
          (!EFI_ERROR (Status)))
      {
        DEBUG ((DEBUG_INFO, "TishDataHob->TsegReallocateEnableCount[%d]\n", TishDataHob->TsegReallocateEnableCount));
        goto MemTestPass;
      } else {
        goto MemTestFail;
      }
    }

MemTestPass:
    if (((!EFI_ERROR (Status)) && (TishDataHob->RetryCount <= 3)) ||
        (!EFI_ERROR (Status)))
    {
      DEBUG ((DEBUG_INFO, "Memory Diagnostic Test for TSEG Memory Region  at 0x%x - Passed\n", TsegBaseAddress));
      DEBUG ((DEBUG_INFO, "TSEG RetryCount[%d]\n", TishDataHob->RetryCount));
      TishDataHob->TsegRegionAddress    = TsegBaseAddress;
      TishDataHob->TsegMemoryTestStatus = PASS;
      //
      // Clear TSEG Memory Test status  in CMOS_MEM_TEST_INFO_OFFSET(0x94), if TSEG MemTest is PASS
      //
      MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
      MemoryTestInfo &= ((UINT8) ~BIT4);
      CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
      DEBUG ((DEBUG_INFO, "PerformTsegRegionMemoryTest End\n"));
      return Status;
    } else {
      goto MemTestFail;
    }

MemTestFail:
    DEBUG ((DEBUG_INFO, "Memory Diagnostic Test for TSEG Memory Region  at 0x%x - Failed\n", TsegBaseAddress));
    DEBUG ((DEBUG_INFO, "TSEG RetryCount[%d]\n", TishDataHob->RetryCount));
    TishDataHob->TsegMemFailureAddress = BadMemoryAddress;
    TishDataHob->TsegMemoryTestStatus  = FAIL;
    //
    // Update the RetryCount[BIT0: BIT2] value in CMOS_MEM_TEST_INFO_OFFSET(0x94)
    //
    RetryCount      = TishDataHob->RetryCount +1;
    MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
    MemoryTestInfo &= 0xF8;
    MemoryTestInfo |= (RetryCount & 0x07);
    CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
    //
    // Set TSEG Memory Test status  in CMOS_MEM_TEST_INFO_OFFSET(0x94), if TSEG MemTest is FAIL
    //
    MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
    MemoryTestInfo |= ((UINT8)BIT4);
    CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
    if (RetryCount > 3 ) {
      //
      // Clear TSEG Memory Test status  in CMOS_MEM_TEST_INFO_OFFSET(0x94), if TSEG MemTest is PASS
      //
      MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
      MemoryTestInfo &= ((UINT8) ~BIT4);
      CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
      DEBUG ((DEBUG_INFO, "Assert TSEG Mem Test Failed and reached maximum RetryCount  = %d\n", RetryCount));
      ASSERT (RetryCount <= 3);
    }

    //
    // Perform Warm reset after update the TSEG RetryCount value in NVS UEFI Variable
    //
    (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    return Status;
  }

  return Status;
}

/**
  Get the BIOS Setup Menu Data

  @param[out] SetupData      Return the BIOS Setup Menu Data

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
GetBiosSetupMenuData (
  OUT SETUP_DATA  *SetupData
  )
{
  EFI_STATUS                       Status;
  UINTN                            VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;

  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **)&VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    return Status;
  }

  VarSize = sizeof (SETUP_DATA);
  Status  = VariableServices->GetVariable (
                                VariableServices,
                                L"Setup",
                                &gSetupVariableGuid,
                                NULL,
                                &VarSize,
                                SetupData
                                );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (SetupData) failed\n"));
  }

  return Status;
}

/**
  Installs Memory Test Info Structure Data Hob
  @param[in] UINT8      Ppr Recovery Status Enable
  @param[in] UINT8      Tseg Reallocate Enable Count

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallTishHob (
  IN UINT8  TsegReallocateEnableCount
  )
{
  EFI_STATUS       Status;
  TISH_CONFIG_HOB  *TishDataHob;

  //
  // Build HOB for TISH Data
  //
  TishDataHob = NULL;
  Status      = PeiServicesCreateHob (
                  EFI_HOB_TYPE_GUID_EXTENSION,
                  sizeof (TISH_CONFIG_HOB),
                  (VOID **)&TishDataHob
                  );
  ASSERT_EFI_ERROR (Status);
  //
  // Initialize default HOB data
  //
  ZeroMem (&(TishDataHob->TsegRegionAddress), sizeof (TISH_CONFIG_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  TishDataHob->EfiHobGuidType.Name        = gTishDataHobGuid;
  TishDataHob->TsegMemFailureAddress      = 0;
  TishDataHob->SafeLoadingBiosEnableState = 1;
  TishDataHob->TsegSize                   = PcdGet32 (PcdTsegSize);
  TishDataHob->RetryCount                 = 0;
  TishDataHob->TsegMemoryTestStatus       = FAIL;
  TishDataHob->PprRecoveryStatusEnable    = 0;
  if (TsegReallocateEnableCount != 0) {
    TishDataHob->TsegReallocateEnableCount = TsegReallocateEnableCount;
  } else {
    TishDataHob->TsegReallocateEnableCount = 0;
  }

  return Status;
}

/**
  PEI Driver to Perform Memory Diagnostic Test for TSEG Region.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
PeiMemoryDiagnosticTestLibContructor (
  )
{
  EFI_STATUS       Status;
  TISH_CONFIG_HOB  *TishDataHob;
  UINT8            RetryCount;
  SETUP_DATA       SetupData;
  UINT8            MemoryTestInfo;
  UINT8            TsegMemoryTestStatus;

  TishDataHob          = NULL;
  RetryCount           = 0;
  MemoryTestInfo       = 0;
  TsegMemoryTestStatus = 0;

  //
  // Get value for SafeLoadingBiosEnable from BIOS Setup
  //
  Status = GetBiosSetupMenuData (&SetupData);
  //
  // If Boot state is  first boot after reflashing, clear RetryCount and
  // Set Memory Test status in CMOS
  //
  if (!IsBootStatePresent ()) {
    if ((CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET) & BIT4) == 0) {
      CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, 0x18);
    }
  }

  //
  //  Get TSEG Memory Test Status
  //
  TsegMemoryTestStatus = (((CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET) & BIT4) >> 4) == 1) ? 1 : 0;
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "SetupData.SafeLoadingBiosEnable: %x\n", SetupData.SafeLoadingBiosEnable));
    if ((SetupData.SafeLoadingBiosEnable == 1) || (TsegMemoryTestStatus == PASS)) {
      //
      //  Installing Memory Test Info Structure Hob
      //
      DEBUG ((DEBUG_INFO, "Install Test Info Structure  Hob (TISH)\n"));
      Status = InstallTishHob (SetupData.TsegReallocateEnableCount);
      //
      // Performing TSEG Region Memory Test after TsegMemoryTestInit PPI Installed
      //
      Status      = PeiServicesNotifyPpi (&mTsegMemoryTestNotifyList);
      TishDataHob = (TISH_CONFIG_HOB *)GetFirstGuidHob (&gTishDataHobGuid);
      if (TishDataHob != NULL) {
        RetryCount = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET) & 0x7;
        if ((TishDataHob->TsegReallocateEnableCount != 0) && (TishDataHob->TsegReallocateEnableCount < RetryCount) && (RetryCount <= 3)) {
          //
          // Initialize the RetryCount[BIT0: BIT2] value as 0 in CMOS_MEM_TEST_INFO_OFFSET(0x94)
          // if TsegReallocateEnableCount less than Retrycount
          //
          RetryCount      = 0;
          MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
          MemoryTestInfo &= 0xF8;
          MemoryTestInfo |= (RetryCount & 0x07);
          CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
        }

        if ((TishDataHob->TsegReallocateEnableCount != 0) && (TishDataHob->TsegReallocateEnableCount != RetryCount)) {
          //
          // Set TSEG Memory Test status as FAIL  in CMOS_MEM_TEST_INFO_OFFSET(0x94)
          // if TsegReallocateEnableCount is not equal to Retrycount
          //
          MemoryTestInfo  = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET);
          MemoryTestInfo |= ((UINT8)BIT4);
          CmosWrite8 (CMOS_MEM_TEST_INFO_OFFSET, MemoryTestInfo);
        }

        //
        //  Update the RetryCount and PPR Status to TISH Hob from CMOS Memory
        //
        RetryCount           = CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET) & 0x7;
        TsegMemoryTestStatus = (((CmosRead8 (CMOS_MEM_TEST_INFO_OFFSET) & BIT4) >> 4) == 1) ? 1 : 0;
        if ((TsegMemoryTestStatus == PASS) && (RetryCount != 4)) {
          TishDataHob->SafeLoadingBiosEnableState = 0;
        }

        TishDataHob->RetryCount           = RetryCount;
        TishDataHob->TsegMemoryTestStatus = TsegMemoryTestStatus;
        DEBUG ((DEBUG_INFO, "Updated RetryCount: %x\n", TishDataHob->RetryCount));
        DEBUG ((DEBUG_INFO, "TSEG Memory Status: %x\n", TishDataHob->TsegMemoryTestStatus));
      }
    }
  }

  return Status;
}
