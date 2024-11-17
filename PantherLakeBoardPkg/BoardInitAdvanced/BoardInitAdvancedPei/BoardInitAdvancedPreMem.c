/** @file
  Source code file for Board Init Advanced Pre-Memory PEI module

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

#include <PiPei.h>
#include <Ppi/Reset.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <IntelRcStatusCode.h>
#include <Library/TimerLib.h>
#include <Guid/TcoWdtHob.h>
#include <Library/PmcLib.h>
#include <Library/TcoLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <PlatformNvRamHookLib.h>
#include <Library/EcMiscLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/PmcRegs.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <SetupVariable.h>
#include <CustomOptionControl.h>
#include <FirwmareConfigurations.h>
#include <Library/PeiPlatformLib.h>
#include <PlatformBoardId.h>
#include <Library/PeiMeLib.h>
#include <Register/PttPtpRegs.h>
#include <Library/BootGuardLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/GpioV2WrapperLib.h>
#include <Library/FspWrapperApiLib.h>
#include <Library/PeiUsbcRetimerConfigLib.h>
#include <Library/PeiServicesLib.h>
#if FixedPcdGetBool (PcdOverclockEnable) == 1
#include <Library/PlatformVoltageControlLib.h>
#endif
#if FixedPcdGetBool (PcdDTbtEnable) == 1
#include <Library/PeiDTbtPolicyLib.h>
#endif
#include <Library/PcdInfoLib.h>
#include <AcmDataHob.h>
#include <Library/BaseBpmAccessLib.h>
#include <Library/MemoryAllocationLib.h>

///
/// Reset Generator I/O Port
///
#define RESET_GENERATOR_PORT           0xCF9

//
//  Global Data
//
UINTN                 Length = sizeof (UINT8);


EFI_STATUS
EFIAPI
PlatformInitPreMem (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

EFI_STATUS
EFIAPI
PchReset (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mPreMemNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiReadOnlyVariable2PpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) PlatformInitPreMem
};

static EFI_PEI_RESET_PPI mResetPpi = {
  PchReset
};

static EFI_PEI_PPI_DESCRIPTOR mPreMemPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI| EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiResetPpiGuid,
    &mResetPpi
  }
};


EFI_STATUS
EFIAPI
PeiGuidForward (
  VOID
  );

/**
  Configures GPIO

  @param[in]  GpioTable       Point to Platform Gpio table
  @param[in]  GpioTableCount  Number of Gpio table entries

**/
VOID
ConfigureGpio (
  IN GPIOV2_INIT_CONFIG                 *GpioTable,
  IN UINT16                             GpioTableCount
  )
{
  EFI_STATUS          Status;

  DEBUG ((DEBUG_INFO, "ConfigureGpio() Start\n"));

  Status = GpioV2ConfigurePads (GpioTable, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  Status = GpioV2SetConflictCheckData (GpioTable, GpioTableCount);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "ConfigureGpio() End\n"));
}

/**
  Create ACM Data HOB

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
CreateAcmDataHob (
  )
{
  BOOT_POLICY_MANIFEST_HEADER *Bpm;
  CNBS_SEGMENT                *CnbsElement;
  UINT32                      CommunicationBufferBase;
  UINT32                      CommunicationBufferSize;
  UINT8                       *Buffer;
  UINT8                       *IBBDigestBuffer;
  UINT8                       *ACMDigestBuffer;
  UINT8                       *ACMVersionBuffer;
  UINT8                       *ACMBenchmarkBuffer;
  RECORD                      *ACMBenchmarkRecordBuffer;
  UINT32                      BaseMsr;
  UINT32                      MaskMsr;
  UINT32                      IBB_Digest_Uuid[4];
  UINT32                      ACM_Digest_Uuid[4];
  UINT32                      ACM_Version_Uuid[4];
  UINT32                      ACM_Benchmark_Uuid[4];
  VOID                        *HobPtr;
  ACM_DATA_HOB                AcmDataHob;
  UINT32                      Count;
  UINT32                      StructureSize;
  BOOLEAN                     IBBDigestFound;
  BOOLEAN                     ACMDigestFound;
  BOOLEAN                     ACMVersionFound;
  BOOLEAN                     ACMBenchmarkFound;
  STARTUP_ACM_BENCHMARK       *ACMBenchmarkBufferStruct;

  Bpm                         = NULL;
  CnbsElement                 = NULL;
  Count                       = 0;
  IBBDigestBuffer             = NULL;
  ACMDigestBuffer             = NULL;
  ACMVersionBuffer            = NULL;
  ACMBenchmarkBuffer          = NULL;
  ACMBenchmarkRecordBuffer    = NULL;
  IBBDigestFound              = FALSE;
  ACMDigestFound              = FALSE;
  ACMVersionFound             = FALSE;
  ACMBenchmarkFound           = FALSE;

  IBB_Digest_Uuid[0]          = ACM_NEM_IBB_DIGESTS_UUID1;
  IBB_Digest_Uuid[1]          = ACM_NEM_IBB_DIGESTS_UUID2;
  IBB_Digest_Uuid[2]          = ACM_NEM_IBB_DIGESTS_UUID3;
  IBB_Digest_Uuid[3]          = ACM_NEM_IBB_DIGESTS_UUID4;

  ACM_Digest_Uuid[0]          = ACM_NEM_ACM_DIGESTS_UUID1;
  ACM_Digest_Uuid[1]          = ACM_NEM_ACM_DIGESTS_UUID2;
  ACM_Digest_Uuid[2]          = ACM_NEM_ACM_DIGESTS_UUID3;
  ACM_Digest_Uuid[3]          = ACM_NEM_ACM_DIGESTS_UUID4;

  ACM_Version_Uuid[0]         = ACM_NEM_ACM_VER_UUID1;
  ACM_Version_Uuid[1]         = ACM_NEM_ACM_VER_UUID2;
  ACM_Version_Uuid[2]         = ACM_NEM_ACM_VER_UUID3;
  ACM_Version_Uuid[3]         = ACM_NEM_ACM_VER_UUID4;

  ACM_Benchmark_Uuid[0]       = ACM_NEM_ACM_BENCHMARK_UUID1;
  ACM_Benchmark_Uuid[1]       = ACM_NEM_ACM_BENCHMARK_UUID2;
  ACM_Benchmark_Uuid[2]       = ACM_NEM_ACM_BENCHMARK_UUID3;
  ACM_Benchmark_Uuid[3]       = ACM_NEM_ACM_BENCHMARK_UUID4;

  BaseMsr                     = ACM_NEM_MTRR_MSR_BASE_9;
  MaskMsr                     = ACM_NEM_MTRR_MSR_MASK_9;

  Bpm = FindBpm ();
  ASSERT (Bpm != NULL);
  if (Bpm == NULL) return EFI_NOT_FOUND;

  CnbsElement = (CNBS_SEGMENT*) FindBpmElement (Bpm, BPM_CNBS_ELEMENT_STRUCTURE_ID);
  ASSERT (CnbsElement != NULL);
  if (CnbsElement == NULL) return EFI_NOT_FOUND;

  CommunicationBufferBase = CnbsElement->BufferData.Base;
  CommunicationBufferSize = CnbsElement->BufferData.Size;
  DEBUG ((DEBUG_INFO, "CommunicationBufferBase = 0x%x\n", CommunicationBufferBase));
  DEBUG ((DEBUG_INFO, "CommunicationBufferSize = 0x%x\n", CommunicationBufferSize));

  Buffer = (UINT8 *)(UINTN) CommunicationBufferBase;
  while (Count < CommunicationBufferSize) {
    StructureSize = *(UINT32 *) (Buffer + 4);
    if (StructureSize == 0) {
      break;
    }
    if (((*(UINT32 *) (Buffer + 8))  == IBB_Digest_Uuid[0]) ||
        ((*(UINT32 *) (Buffer + 12)) == IBB_Digest_Uuid[1]) ||
        ((*(UINT32 *) (Buffer + 16)) == IBB_Digest_Uuid[2]) ||
        ((*(UINT32 *) (Buffer + 20)) == IBB_Digest_Uuid[3])) {
      IBBDigestBuffer = Buffer + 24; // This reaches the Data section of the stucture once the UUID is found to match (Type + Size + UUID)
      IBBDigestFound = TRUE;
    } else if (((*(UINT32 *) (Buffer + 8))  == ACM_Digest_Uuid[0]) ||
               ((*(UINT32 *) (Buffer + 12)) == ACM_Digest_Uuid[1]) ||
               ((*(UINT32 *) (Buffer + 16)) == ACM_Digest_Uuid[2]) ||
               ((*(UINT32 *) (Buffer + 20)) == ACM_Digest_Uuid[3])) {
      ACMDigestBuffer = Buffer + 24; // This reaches the Data section of the stucture once the UUID is found to match (Type + Size + UUID)
      ACMDigestFound = TRUE;
    } else if (((*(UINT32 *) (Buffer + 8))  == ACM_Version_Uuid[0]) ||
               ((*(UINT32 *) (Buffer + 12)) == ACM_Version_Uuid[1]) ||
               ((*(UINT32 *) (Buffer + 16)) == ACM_Version_Uuid[2]) ||
               ((*(UINT32 *) (Buffer + 20)) == ACM_Version_Uuid[3])) {
      ACMVersionBuffer = Buffer + 24; // This reaches the Data section of the stucture once the UUID is found to match (Type + Size + UUID)
      ACMVersionFound = TRUE;
    } else if (((*(UINT32 *) (Buffer + 8))  == ACM_Benchmark_Uuid[0]) ||
               ((*(UINT32 *) (Buffer + 12)) == ACM_Benchmark_Uuid[1]) ||
               ((*(UINT32 *) (Buffer + 16)) == ACM_Benchmark_Uuid[2]) ||
               ((*(UINT32 *) (Buffer + 20)) == ACM_Benchmark_Uuid[3])) {
      ACMBenchmarkBuffer = Buffer + 28; // This reaches the Count and Data sections of the stucture once the UUID is found to match (Type + Size + UUID + ModuleID)
      ACMBenchmarkFound = TRUE;
      ACMBenchmarkBufferStruct = (STARTUP_ACM_BENCHMARK*) ACMBenchmarkBuffer;
    }
    Buffer += StructureSize;
    Count+= StructureSize;
  }

  DEBUG ((DEBUG_INFO, "IBBDigestFound = 0x%x\n", IBBDigestFound));
  DEBUG ((DEBUG_INFO, "ACMDigestFound = 0x%x\n", ACMDigestFound));
  DEBUG ((DEBUG_INFO, "ACMVersionFound = 0x%x\n", ACMVersionFound));
  DEBUG ((DEBUG_INFO, "ACMBenchmarkFound = 0x%x\n", ACMBenchmarkFound));

  if (!IBBDigestFound  &&
      !ACMDigestFound  &&
      !ACMVersionFound &&
      !ACMBenchmarkFound) {
    DEBUG ((DEBUG_INFO, "ACM Data not found within communication buffer\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "ACM Data is found within communication buffer\n"));

  ///
  /// Initialize default HOB data
  ///
  ZeroMem (&AcmDataHob, sizeof (ACM_DATA_HOB));

  //
  // Get data from ACM Communication Buffer
  //
  if (IBBDigestFound)  CopyMem (&(AcmDataHob.IBBDigests), IBBDigestBuffer,  sizeof(IBB_DIGEST));
  if (ACMDigestFound)  CopyMem (&(AcmDataHob.ACMDigests), ACMDigestBuffer,  sizeof(ACM_DIGEST));
  if (ACMVersionFound) CopyMem (&(AcmDataHob.ACMVersion), ACMVersionBuffer, sizeof(ACM_VERSION));
  if (ACMBenchmarkFound) {
    ACMBenchmarkRecordBuffer = (RECORD*) AllocatePool (sizeof(RECORD) * ACMBenchmarkBufferStruct->Count);
    if (ACMBenchmarkRecordBuffer == NULL) {
      DEBUG ((DEBUG_INFO, "ACM Data HOB: Failed to allocate space for HOB! \n"));
      return EFI_OUT_OF_RESOURCES;
    }
    CopyMem (ACMBenchmarkRecordBuffer, (void*) &(ACMBenchmarkBufferStruct->Record), sizeof(RECORD) * ACMBenchmarkBufferStruct->Count);
    AcmDataHob.StartupACMBenchmark.Record = ACMBenchmarkRecordBuffer;
    AcmDataHob.StartupACMBenchmark.Count = ACMBenchmarkBufferStruct->Count;
  }

  ///
  /// Create HOB for ACM Info
  ///
  HobPtr = BuildGuidDataHob (
                 &gAcmDataHobGuid,
                 &AcmDataHob,
                 sizeof (ACM_DATA_HOB)
                 );
  ASSERT (HobPtr != NULL);

  // MTRR 9 shall be disabled to not interfere with further IBB functionality.
  AsmWriteMsr64(BaseMsr, 0);
  AsmWriteMsr64(MaskMsr, 0);

  return EFI_SUCCESS;
}

//@todo it should be performed in Si Pkg.
/**
  Provide hard reset PPI service.
  To generate full hard reset, write 0x0E to PCH RESET_GENERATOR_PORT (0xCF9).

  @param[in]  PeiServices       General purpose services available to every PEIM.

  @retval     Not return        System reset occured.
  @retval     EFI_DEVICE_ERROR  Device error, could not reset the system.
**/
EFI_STATUS
EFIAPI
PchReset (
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  DEBUG ((DEBUG_INFO, "Perform Cold Reset\n"));
  IoWrite8 (RESET_GENERATOR_PORT, 0x0E);

  CpuDeadLoop ();

  ///
  /// System reset occured, should never reach at this line.
  ///
  ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);

  return EFI_DEVICE_ERROR;
}

/**
  Early Platform PCH initialization
**/
VOID
EFIAPI
EarlyPlatformPchInit (
  VOID
  )
{
  UINT8        TcoRebootHappened;
  TCO_WDT_HOB  *TcoWdtHobPtr;
  EFI_STATUS   Status;

  //
  // Halt the TCO timer as early as possible
  //
  TcoHaltTimer (PcdGet16 (PcdTcoBaseAddress));

  if (TcoSecondToHappened (PcdGet16 (PcdTcoBaseAddress))) {
    TcoRebootHappened = 1;
    DEBUG ((DEBUG_INFO, "PlatformInitPreMem - TCO Second TO status bit is set. This might be a TCO reboot\n"));
  } else {
    TcoRebootHappened = 0;
  }

  ///
  /// Create HOB
  ///
  Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, sizeof (TCO_WDT_HOB), (VOID **) &TcoWdtHobPtr);
  if (!EFI_ERROR (Status)) {
    TcoWdtHobPtr->Header.Name  = gTcoWdtHobGuid;
    TcoWdtHobPtr->TcoRebootHappened = TcoRebootHappened;
  }

  TcoClearSecondToStatus (PcdGet16 (PcdTcoBaseAddress));

}

// @todo: It should be moved Policy Init.
/**
  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers

  @param[in] VariableServices  A pointer to this instance of the EFI_PEI_READ_ONLY_VARIABLE2_PPI.
**/
VOID
EFIAPI
PlatformPchInit (
  IN EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices
  )
{

  ///
  /// Enable the LPC I/O decoding for 0x6A0~0x6A7 as EC's extra I/O port, where 0x6A0 is the Data port
  /// and 0x6A4 is the Command/Status port.
  ///
#if FixedPcdGetBool (PcdEcEnable) == 1
  PchLpcGenIoRangeSet (PcdGet16 (PcdEcExtraIoBase), 0x10); //PTLMOD+
#else
#endif

}

/**
  This function handles PlatformInit task after PeiReadOnlyVariable2 PPI produced

  @param[in]  PeiServices  Pointer to PEI Services Table.
  @param[in]  NotifyDesc   Pointer to the descriptor for the Notification event that
                           caused this function to execute.
  @param[in]  Ppi          Pointer to the PPI data associated with this function.

  @retval     EFI_SUCCESS  The function completes successfully
  @retval     others
**/
EFI_STATUS
EFIAPI
PlatformInitPreMem (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                        Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI   *VariableServices;
  EFI_BOOT_MODE                     BootMode;
  UINT16                            ABase;
  SETUP_DATA                        SystemConfiguration;
  UINTN                             VariableSize;

#if FixedPcdGetBool(PcdTpmEnable) == 1
  UINT8                             TpmInterfaceId;
  UINT32                            PttFtifReg;
#if (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)
  BOOT_GUARD_INFO                   BootGuardInfo;
#endif // (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)
#endif // FixedPcdGetBool(PcdTpmEnable) == 1

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_ENTRY); //PostCode (BOARD_INIT_PREMEM_ENTRY);

  if (PcdGetBool (PcdPlatformInitPreMem)) {
    return EFI_SUCCESS;
  } else {
    Status = PcdSetBoolS (PcdPlatformInitPreMem, TRUE);
    ASSERT_EFI_ERROR (Status);
  }

  VariableServices = (EFI_PEI_READ_ONLY_VARIABLE2_PPI *) Ppi;

  ///
  /// EDK/FSP dispatch mode switches PcdAcpiS3Enable dynamically on gSiPkgTokenSpaceGuid.PcdS3Enable setting.
  /// FSP API mode has a default PcdAcpiS3Enable setting but does not turn the PCD on/off here.
  ///
#if FixedPcdGetBool (PcdFspModeSelection) == 0
#if FixedPcdGetBool (PcdS3Enable) == 1
  PcdSetBoolS (PcdAcpiS3Enable, TRUE);
#else
  PcdSetBoolS (PcdAcpiS3Enable, FALSE);
#endif
#endif

  PeiServicesGetBootMode (&BootMode);

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SystemConfiguration
                               );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Fail to get System Configuration and set the configuration to production mode!\n"));
  } else {
  }

#if FixedPcdGetBool(PcdDTbtEnable) == 1
  if (SystemConfiguration.DiscreteTbtSupport) {
    InstallPeiDTbtPolicy ();
  }
#endif

  UsbcRetimerComplianceModeConfig ();


  PcdSetBoolS (PcdGuidForward, SystemConfiguration.GuidForward);

  if (PcdGetBool (PcdGuidForward)) {
    Status = PeiGuidForward ();
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Status-%r\n",Status));
    }
  }

#if FixedPcdGetBool (PcdBootGuardPerformanceEnable) == 1
  Status = CreateAcmDataHob ();
#endif // FixedPcdGetBool (PcdBootGuardPerformanceEnable) == 1

  ///
  /// Do basic PCH init
  ///
  PlatformPchInit (VariableServices);

  ABase = PmcGetAcpiBase ();

  ///
  /// Clear all pending SMI. On S3 clear power button enable so it will not generate an SMI.
  ///
  IoWrite16 (ABase + R_ACPI_IO_PM1_EN, 0);
  IoWrite32 (ABase + R_ACPI_IO_GPE0_EN_127_96, 0);
  IoWrite32 (ABase + R_ACPI_IO_GPE1_EN_31_0, 0);
  ///----------------------------------------------------------------------------------
  ///
  /// BIOS should check the wake status before memory initialization to determine
  /// if ME has reset the system while the host was in a sleep state. If platform was not in a sleep state,
  /// BIOS should ensure a non-sleep exit path is taken by forcing an s5 exit.
  ///
  if (PmcGetSleepTypeAfterWake () == PmcNotASleepState) {
    PmcSetSleepState (PmcS5SleepState);
  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    //
    // A platform PEIM should enable R/W access to E/F segment in the S3 boot path
    // otherwise, this AP wakeup buffer can't be accessed during CPU S3 operation.
    //
    SetHostBridgeRegisterData (HostBridgePam0CfgReg, HostBridgePam0WriteData, 0);
    SetHostBridgeRegisterData (HostBridgePam5CfgReg, HostBridgePam5WriteData, 0);
    SetHostBridgeRegisterData (HostBridgePam6CfgReg, HostBridgePam6WriteData, 0);
  }

#if FixedPcdGetBool(PcdTpmEnable) == 1

#if (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)
  //
  // TPM Initialized by Boot Guard
  //
  if (IsBootGuardSupported ()) {
    GetBootGuardInfo (&BootGuardInfo);
    DEBUG ((DEBUG_INFO, "PlatformInitAdvancedPreMem: BootGuard supported\n"));
    if (BootGuardInfo.BypassTpmInit == TRUE) {
      DEBUG ((DEBUG_INFO, "PlatformInitAdvancedPreMem: Bypass TPM init\n"));
      PcdSet8S (PcdTpm2InitializationPolicy, 0);
    }
  }
#endif // (FixedPcdGetBool (PcdBootGuardEnable) == 1) || (FixedPcdGetBool (PcdTxtEnable) == 1)

  //
  // If PTT presence detected, disable TPM self test policy
  //
  TpmInterfaceId = MmioRead8 ((UINTN)PcdGet64 (PcdTpmBaseAddress) + R_TPM_INTERFACE_ID);
  PttFtifReg = MmioRead32 (R_PTT_TXT_STS_FTIF);

  if ((TpmInterfaceId != TIS_13_TPM_INTERFACE) &&
      ((PttFtifReg & V_FTIF_FTPM_PRESENT) == V_FTIF_FTPM_PRESENT)) {

    DEBUG ((DEBUG_INFO, "PttFtifReg: %x\n", PttFtifReg));
    DEBUG ((DEBUG_INFO, "PTT detected\n"));
    DEBUG ((DEBUG_INFO, "PlatformInitAdvancedPreMem: Disabling TPM Self Test policy for PTT\n"));
    PcdSet8S (PcdTpm2SelfTestPolicy, 0);

  }

  if (BootMode == BOOT_ON_S3_RESUME) {
    PcdSet8S (PcdCRBIdleByPass, 0x0);
  }
#endif // FixedPcdGetBool (PcdTpmEnable)
  ///
  /// Install Pre Memory PPIs
  ///
  Status = PeiServicesInstallPpi (&mPreMemPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_EXIT); //PostCode (BOARD_INIT_PREMEM_EXIT);

  return Status;
}

/**
  Platform Init before memory PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
BoardInitAdvancedPreMemEntryPoint(
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS     Status;

  ///
  /// Initialize Nvram to default when checksum computation is failed
  ///
  SecondaryNvRamInit ();

  ///@todo it should be moved to Si Pkg.
  ///
  /// Do Early PCH init
  ///
  EarlyPlatformPchInit ();

  //
  // Install ME reset call back function.
  //
  RegisterMeReset ();

#if FixedPcdGetBool (PcdOverclockEnable) == 1
  PlatformVoltageInit ();
#endif

  ///
  /// Performing PlatformInitPreMem after PeiReadOnlyVariable2 PPI produced
  ///
  Status = PeiServicesNotifyPpi (&mPreMemNotifyList);

  return Status;
}
