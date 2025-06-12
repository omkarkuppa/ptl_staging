/** @file
  Memory Initialization PEIM.

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

@par Specification Reference:
**/

#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Guid/SmramMemoryReserve.h>
#include <Guid/MemoryTypeInformation.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/TseInfoLib.h>
#include <Library/TseDataHob.h>
#include <Library/MemorySubSystemInitLib.h>
#include <Library/NguInfoLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/RngLib.h>
#include <Library/SmbusLib.h>
#include <MemorySubSystemConfig.h>
#include <Library/PeiImrInitLib.h>
#include <Library/CpuInitPreMem.h>
#include "MrcDdr5.h"
#include "MrcDdr5Settings.h"
#include "CMrcStatsTracker.h"
#include <ConfigBlock/SiPreMemConfig.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/MemoryInstallLib.h>
#include <Library/DomainIGpuInit.h>
#include <Library/PeiTraceHubInitLib.h>
#include "MrcDdrIoComp.h"
#include "CPlatformData.h"  // for PLATFORM_DATA and BDAT definitionss
#include "MrcAmt.h" // For AMT_PPR_ENABLE
#include "MrcPpr.h" // For MrcIsPprEnabled
#include <Library/PeiVmdInitFruLib.h>
#include <BupMsgs.h>

//
// Definition in EDK Foundation, used in this driver
//
#ifndef MDEPKG_NDEBUG
#define BOOT_IN_RECOVERY_MODE_MASK                    0x40
#define BOOT_SPECIAL_MASK                             0x80
#endif

#include <Register/GenerationMsr.h>
#include <Library/PeiCpuInitFruLib.h>
#include "CMemoryInit.h"
#include "MrcGeneral.h"
#include "MrcPostCodes.h"
#include "MrcOemPlatform.h"
#include <Library/PerformanceLib.h>
#include <CMemoryTest.h>
#include <IndustryStandard/SmBios.h>
#include <Library/PeiHostBridgeInitLib.h>
#include <Library/PeiTelemetryInitLib.h>
#include <Library/TxtLib.h>
#include <Register/Cpuid.h>

//
// Driver Dependent PPI Prototypes
//
#include <Ppi/MasterBootMode.h>

#include <Library/FspCommonLib.h>
#include <Library/FspSwitchStackLib.h>
#include <Ppi/FspmArchConfigPpi.h>
#include <Library/FspErrorInfoLib.h>

//
// Driver PPI Definitions
//

//
// Private Library Definitions
//
#include <Library/TxtPeiLib.h>
#include <Library/MeInitLib.h>
#include <Library/PsmiInitLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/IGpuInfoLib.h>

#include <Library/MemorySubSystemInfoLib.h>
#include <Library/CapsulePpiProcessLib.h>

#define CMOS_ADDR_PORT  0x70  // Defined in CmosMap.h
#define CMOS_DATA_PORT  0x71  // Defined in CmosMap.h

#ifndef MDEPKG_NDEBUG
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringFc[]  = "WITH_FULL_CONFIGURATION";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringMc[]  = "WITH_MINIMAL_CONFIGURATION";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringNc[]  = "ASSUMING_NO_CONFIGURATION_CHANGES";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringFcd[] = "WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringDs[]  = "WITH_DEFAULT_SETTINGS";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringS4[]  = "ON_S4_RESUME";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringS5[]  = "ON_S5_RESUME";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringS2[]  = "ON_S2_RESUME";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringS3[]  = "ON_S3_RESUME";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringFu[]  = "ON_FLASH_UPDATE";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringRm[]  = "IN_RECOVERY_MODE";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringRmm[] = "IN_RECOVERY_MODE_MASK";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringSm[]  = "SPECIAL_MASK";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 BootStringUnk[] = "MODE_UNKNOWN";
#endif

///
/// The following table drives the SPD fields that are read from the DIMM.
///
GLOBAL_REMOVE_IF_UNREFERENCED static const SPD_OFFSET_TABLE SpdDdr5Table[] = {
  {   0,               1,           (1 << bmCold),},
  {   2,               3,           (1 << bmCold) | (1 << bmFast),},
  {   4,              47,           (1 << bmCold),},
  { 126,             127,           (1 << bmCold),},
  { 192,             213,           (1 << bmCold),},
  { 230,             235,           (1 << bmCold),},
  { 240,             243,           (1 << bmCold) | (1 << bmFast),},
  { SPD5_MANUF_START, SPD5_MANUF_END, (1 << bmCold) | (1 << bmFast),},
  { 521,             550,           (1 << bmCold),},
  { SPD5_DRAM_MFG_START, SPD5_DRAM_MFG_END, (1 << bmCold) | (1 << bmFast),},
  // XMP 3.0 spec
  // Below XMP Global Section
  { 640,             703,             (1 << bmCold),},
  // Below XMP PROFILE1
  { 704,             767,             (1 << bmCold),},
  // Below USER_PROFILE4/5
  { 896,             959,             (1 << bmCold),},
  { 960,             1023,            (1 << bmCold),},
  // Below XMP PROFILE2/3
  { 768,             831,             (1 << bmCold),},
  { 832,             895,             (1 << bmCold),}
};
GLOBAL_REMOVE_IF_UNREFERENCED const SPD_OFFSET_TABLE SpdLpddrTable[] = {
  {   0,               1,               (1 << SpdCold),},
  {   2,               3,               (1 << SpdCold) | (1 << SpdFast),},
  {   4,              32,               (1 << SpdCold),},
  { 120,             130,               (1 << SpdCold),},
  { 230,             232,               (1 << SpdCold),},
  { 234,             235,               (1 << SpdCold),},
  { SPDLP_MANUF_START, SPDLP_MANUF_END, (1 << SpdCold) | (1 << SpdFast),},
  { 329,             352,               (1 << SpdCold) | (1 << SpdFast),},
  { SPDLP_JEDEC_SPEC_MANUF_START, SPDLP_JEDEC_SPEC_MANUF_END, (1 << SpdCold) | (1 << SpdFast),},
  { 521,             554,               (1 << SpdCold),}
};

///
/// Configure default no swizzle for Simics
///
// DQS byte swizzling between CPU and DRAM
const UINT8 DqsMapCpu2DramNoSwizzle[MAX_CHANNEL][MAX_BYTE_IN_DDR5_CHANNEL] = {
  // Ch 0       1             2             3
  {0,1,2,3},  {0,1,2,3},  {0,1,2,3},  {0,1,2,3} // Controller 0
};

// DQ bit swizzling between CPU and DRAM
UINT8 DqMapCpu2DramNoSwizzle[MAX_BYTE_IN_DDR5_CHANNEL][MAX_BITS] = {
  {  0,  1,  2,  3,  4,  5,  6,  7 }, // Byte 0
  {  8,  9, 10, 11, 12, 13, 14, 15 }, // Byte 1
  { 16, 17, 18, 19, 20, 21, 22, 23 }, // Byte 2
  { 24, 25, 26, 27, 28, 29, 30, 31 }, // Byte 3
};

CONST EFI_PEI_PPI_DESCRIPTOR gMrcColdBootRequiredPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gMrcColdBootRequiredPpiGuid,
  NULL
};

GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 MrcDataStringConst[] = "MRCD";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 MrcSpdStringConst[]  = "SPD ";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 Ch2CkdQckString[]    = "Channel To CKD QCK";
GLOBAL_REMOVE_IF_UNREFERENCED const UINT8 PhyClk2CkdString[]   = "DDRIO To CKD Clock";

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mTsegMemoryTestInitPpi = {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gEfiPeiTsegMemoryTestPpiGuid,
    NULL
};

/**
  Entry point of MemoryInit library
  This function will invoke main memory detection code after the following dependencies
  have been satisfied at right timing.
  - gSiPreMemPolicyPpiGuid
  - gSiPolicyPpiGuid
  - gWdtPpiGuid
  - gSaPeiInitPpiGuid
  - gEfiPeiMasterBootModePpiGuid
  Note:\n

  @retval EFI_SUCCESS          - MRC callback has been registered successfully
  @retval EFI_OUT_OF_RESOURCES - Fail to allocate required buffer for MRC callback control.
**/
EFI_STATUS
EFIAPI
InstallMrcCallback (
  VOID
  )
{
// Exists strictly to build MRC-excluded BIOS for size check
#ifndef NO_MRC
  EFI_STATUS                    Status;
  MRC_INSTANCE                  *MrcInstance;
  SI_PREMEM_POLICY              *SiPreMemPolicy;
  TXT_PREMEM_CONFIG             *TxtPreMemConfig;

  DEBUG ((DEBUG_INFO, "[InstallMrcCallback] MRC Entry...\n"));
  //
  // Obtain policy settings.
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  ASSERT_EFI_ERROR (Status);

  TxtPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  //
  // Initialize MRC Instance for callbacks
  //
  MrcInstance = AllocateZeroPool (sizeof (MRC_INSTANCE));
  ASSERT (MrcInstance != NULL);
  if (MrcInstance == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  MrcInstance->Signature                = MRC_INSTANCE_SIGNATURE;
  MrcInstance->NotifyDescriptor.Flags   = (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  MrcInstance->NotifyDescriptor.Notify  = PrememoryInitCallBack;
  MrcInstance->FileHandle               = NULL;
  MrcInstance->MrcExecuted              = FALSE;
  MrcInstance->NotifyDescriptor.Guid    = &gEfiPeiMasterBootModePpiGuid;
  Status = PeiServicesNotifyPpi (&MrcInstance->NotifyDescriptor);
  ASSERT_EFI_ERROR (Status);
  return Status;
#else // NO_MRC
  return EFI_SUCCESS;
#endif // NO_MRC
}

/**
  A notify callback function to determine if main memory detection function can be executed or not

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.  Not used.

  @retval EFI_SUCCESS          - MRC has been executed successfully
  @retval EFI_NOT_READY        - One of the dependency not ready yet for MRC execution.
**/
EFI_STATUS
EFIAPI
PrememoryInitCallBack (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  EFI_STATUS                    Status;
  MRC_INSTANCE                  *MrcInstance;

  DEBUG ((DEBUG_INFO, "[PrememoryInitCallBack]\n"));
  MrcInstance = MRC_INSTANCE_FROM_NOTIFY_THIS (NotifyDescriptor);

  //
  // If MRC has been executed, rest of callbacks should return directly.
  //
  if (MrcInstance->MrcExecuted == TRUE) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (
             &gEfiPeiMasterBootModePpiGuid,
             0,
             NULL,
             NULL
             );
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_WARN, "[PrememoryInitCallBack] PeiMasterBootModePpi is not installed yet.\n"));
    return EFI_NOT_READY;
  }

  //
  // Here we will execute MemoryInit and set MrcExecuted to TRUE
  //
  MrcInstance->MrcExecuted = TRUE;
  Status = PeimMemoryInit (MrcInstance->FileHandle, (CONST EFI_PEI_SERVICES **) PeiServices, MrcInstance);

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "FspMemoryInitApi() - Error Initialization Failure - 0x%x\n", Status));
    //
    // Report FSP error information to bootloader.
    // This is required by both API and Dispatch mode.
    //
    SendFspErrorInfo (
      gMrcFspErrorTypeCallerId,
      gMrcFspErrorTypeMemoryInit,
      EfiStatusToUint32 (Status)
    );

    //
    // Return after converting to API return status as per FSP EAS
    //
    if ((Status != EFI_INVALID_PARAMETER) && (Status != EFI_UNSUPPORTED) &&
        (Status != EFI_DEVICE_ERROR) && (Status != EFI_OUT_OF_RESOURCES)) {
      Status = EFI_DEVICE_ERROR;  // Remap to API return status as per FSP EAS
    }
    FspApiReturnCheck (Status);
  }

  return Status;
}

/**
  Translate SerialDebugLevel from the ConfigBlock into MRC Levels
**/
VOID
SetSerialDebugLevel (
  IN OUT MrcParameters *const MrcData,
  IN     MrcDebugMsgLevel     SerialDebugLevel
)
{
  MrcInput    *Inputs;

  Inputs      = &MrcData->Inputs;
  /**
  Sets the serial debug message level
  0x00 = Disabled
  0x01 = Errors only
  0x02 = Errors and Warnings
  0x03 = Errors, Warnings, and Info
  0x04 = Errors, Warnings, Info, and Events
  0x05 = Errors, Warnings, Info, Events, and Verbose (Includes Execution Time)
  **/
  switch (SerialDebugLevel) {
    case 0:
      Inputs->SerialDebugLevel = MSG_LEVEL_NONE;
      break;
    case 1:
      Inputs->SerialDebugLevel = MSG_LEVEL_COMBO_1;
      break;
    case 2:
      Inputs->SerialDebugLevel = MSG_LEVEL_COMBO_2;
      break;
    case 3:
      Inputs->SerialDebugLevel = MSG_LEVEL_COMBO_3;
      break;
    case 4:
      Inputs->SerialDebugLevel = MSG_LEVEL_COMBO_4;
      break;
    case 5:
      Inputs->SerialDebugLevel = MSG_LEVEL_COMBO_5;
      break;
    default:
      Inputs->SerialDebugLevel = SerialDebugLevel;
      break;
  }
}

/**
  Check Status of RetrainToWorkingChannel and disable failing channel

  @param[in out]  MrcData - All the MRC global data.
  @param[in]      SskpdSaveRestore - If set to TRUE, saves disabled channel status to scratchpad register.
                                     If set to FALSE, restores disabled channel status from scratchpad register.

  @retval         TRUE if All Channels are passing,
                  FALSE if all Channels are not passing and MRC needs reset after disabling failing channels
**/
BOOLEAN
MrcDisableFailingChannels (
  IN OUT  MrcParameters        *const MrcData,
  IN      BOOLEAN              SskpdSaveRestore
  )
{
  MrcOutput  *Outputs;
  MrcInput   *Inputs;
  BOOLEAN    RetrainToWorkingChannelDone;
  UINT8      McChIndex;
  UINT8      NumOfFailingCh;
  UINT8      Controller;
  UINT8      Channel;
  UINT8      SubChannel;
  UINT8      Dimm;
  UINT8      DisableChannelWrite;
  UINT8      DisableChannelRead;

  M_PCU_CR_SSKPD_PCU_STRUCT SskpdData64;
  MRC_EXT_INPUTS_TYPE       *ExtInputs;

  Outputs                     = &MrcData->Outputs;
  Inputs                      = &MrcData->Inputs;
  ExtInputs                   = Inputs->ExtInputs.Ptr;
  RetrainToWorkingChannelDone = TRUE;
  NumOfFailingCh              = 0;
  DisableChannelRead          = 0;
  DisableChannelWrite         = 0;

  if (!ExtInputs->RetrainToWorkingChannel) {
    return RetrainToWorkingChannelDone;
  }

  //
  // Read DisableChannel Value from ScratchPad Register
  //
  if (SskpdSaveRestore == READ_RESTORE_FROM_SCRATCHPAD) {
    DEBUG ((DEBUG_INFO, "Reading ScratchPad Register\n"));
    SskpdData64.Data = MrcWmRegGet (MrcData);
    DisableChannelRead = (UINT8) SskpdData64.Bits.MrcDisableChannel;
    DEBUG ((DEBUG_INFO, "DisableChannelRead 0x%x\n", DisableChannelRead));
    if (DisableChannelRead != 0x0) {
      for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
        for (Channel= 0;  Channel < MAX_CHANNEL; Channel++) {
          if ((DisableChannelRead >> (Controller * MAX_CHANNEL + Channel)) & MRC_DIMM_STATUS_ERROR) {
            ExtInputs->DisableChannel[Controller][Channel] = CHANNEL_DISABLED;
            DEBUG ((DEBUG_INFO, "!! MC:%d CH:%d DISABLE !!\n", Controller, Channel));
          }
        } // Channel
      } // Controller
      RetrainToWorkingChannelDone = FALSE;
    }
  }

  //
  // Store FailingChannelBitMask to DisableChannel and Write to ScratchPad Register
  //
  if (SskpdSaveRestore == WRITE_SAVE_TO_SCRATCHPAD) {
    DEBUG ((DEBUG_INFO, "Outputs->FailingChannelBitMask.Data16:0x%x\n", Outputs->FailingChannelBitMask.Data16));
    // Iterate through Failing Channel bitmask and update ExtInputs Setup option
    // Detect Failing channel and Disable them
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      for (Channel= 0;  Channel< MAX_CHANNEL; Channel++) {
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          McChIndex = MRC_DIMM_STATUS_INDEX(Controller, Channel, Dimm);
          if ((Outputs->FailingChannelBitMask.Data16 >> McChIndex) & MRC_DIMM_STATUS_ERROR) {
            DisableChannelWrite = DisableChannelWrite | (1 << ((Controller * MAX_CHANNEL) + Channel));
            DEBUG ((DEBUG_INFO, "!!MC:%d CH:%d FAILING CHANNEL DETECTED!!\n", Controller, Channel));
            RetrainToWorkingChannelDone = FALSE;
            //
            // Mc SubChannel should also be disabled in case of Lpddr
            //
            if (Outputs->IsLpddr) {
              SubChannel = (Channel % 2 == 0) ? (Channel + 1) : (Channel - 1);
              DisableChannelWrite = DisableChannelWrite | (1 << ((Controller * MAX_CHANNEL) + SubChannel));
              DEBUG ((DEBUG_INFO, "!!MC:%d CH:%d SUB-CHANNEL DISABLED!!\n", Controller, SubChannel));
            }
          }
        } // Dimm
      } // Channel
    } // Controller

    //
    // Write DisableChannel Value to ScratchPad Register
    //
    DEBUG ((DEBUG_INFO, "DisableChannelWrite 0x%x\n", DisableChannelWrite));
    SskpdData64.Data = MrcWmRegGet (MrcData);
    SskpdData64.Bits.MrcDisableChannel |= DisableChannelWrite;
    DEBUG ((DEBUG_INFO, "Writing ScratchPad Register\n"));
    MrcWmRegSetBits (MrcData, SskpdData64.Data);

    DisableChannelWrite = (UINT8) SskpdData64.Bits.MrcDisableChannel;
    while (DisableChannelWrite != 0) {
      if (DisableChannelWrite & MRC_BIT0) {
          NumOfFailingCh++;
        }
        DisableChannelWrite = DisableChannelWrite >> MRC_BIT0;
    }

    if (((Outputs->IsLpddr) && (NumOfFailingCh > 2)) || ((Outputs->IsDdr5) && (NumOfFailingCh > 1))) {
      RetrainToWorkingChannelDone = TRUE;
      DEBUG ((DEBUG_INFO, "More than 1 failing channel [%d] detected, halting retrain event.\n", NumOfFailingCh));
    }
  }

  return RetrainToWorkingChannelDone;
}

/**
  Main starting point for system memory initialization.
    - 1. Get SysBootMode and MrcBootMode
    - 2. Locate SiPreMemPolicy
    - 3. Locate S3DataPtr from MiscPeiPreMemConfig.
    - 4. SaveDataValid := TRUE if S3DataPtr is not NULL.
    - 5. If SysBootMode is BOOT_ON_S3_RESUME and S3Data is not valid:
      -> ASSERT.
    - 6. If MrcBootMode is Warm boot, but S3 data is not valid :
        -> change MrcBootMode to Cold boot.
    - 7. If MrcBootMode is Cold boot:
        -> Run MRC code
        -> Save S3 Restore Data
      Else
        -> Run MRC_S3Resume
    - 8. Run MRC_Done().
    - 9. Install EFI memory HOBs.

  @param[in] FfsHeader (or FileHandle) - Pointer to Firmware File System file header
  @param[in] PeiServices               - General purpose services available to every PEIM.
  @param[in] MrcInstance               - MRC instance structure for callback implementation

  @retval EFI_SUCCESS      - Memory initialization completed successfully.
  @retval EFI_NOT_READY    - Cannot locate SA Policy.
  @retval EFI_NOT_FOUND    - No S3 data in S3 Boot Mode.
  @retval EFI_DEVICE_ERROR - MemoryInit failed or IOSAV Memory test failed.
**/
EFI_STATUS
PeimMemoryInit (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  MRC_INSTANCE                 *MrcInstance
  )
{
  SI_PREMEM_POLICY             *SiPreMemPolicy;
  MEMORY_CONFIGURATION         *MemConfig;
  MEMORY_CONFIG_NO_CRC         *MemConfigNoCrc;
  MEMORY_PLATFORM_DATA_HOB     *Hob;
  CPU_MEMORY_INIT_CONFIG       CpuMemoryInitConfig;
  PLATFORM_DATA                PlatformData;
  MRC_BDAT_SCHEMA_LIST_HOB     *BdatSchemaListPointer;
  // The next line is telling Coverity to ignore the STACK_USE checker failure due to MrcParameters size
  // coverity [stack_use_local_overflow]
  MrcParameters                *MrcData;
  MrcInput                     *Inputs;
  MrcOutput                    *Outputs;
  MrcSave                      *SaveSys;
  MrcSave                      *Save;
  MrcSaveData                  *SaveData;
  MrcSaveHeader                *SaveHeader;
  MRC_FUNCTION                 CallTable;
  MRC_FUNCTION                 *MrcCall;
  EFI_STATUS                   Status;
  EFI_BOOT_MODE                SysBootMode;
  MrcStatus                    MrcStatus;
  MRC_BOOT_MODE                MrcBootMode;
  BOOLEAN                      SaveDataValid;
  UINT32                       Crc32;
  MrcDebug                     *Debug;
  MRC_EXT_INPUTS_TYPE          *ExtInputs;
#ifndef MDEPKG_NDEBUG
  const UINT8                  *Str;
#endif
  UINT8                        InitStat;
  UINT8                        ForceFullTraining;
  UINT8                        TotalDprSizeMB;
  UINT16                       HobTotalSize;
  MrcParameters                MrcGlobalData;
  BOOLEAN                      InterpeterTrainingDone;
  M_PCU_CR_SSKPD_PCU_STRUCT    SskpdValue;
  VOID                         *S3DataPtr;
  BOOLEAN                      IsLastBasicMemoryTestPass;
  BOOLEAN                      DidPreviousTrainingFail;
  UINT8                        Controller;
  UINT8                        Channel;
  BOOLEAN                      IsEfiResetColdRequired;
  UINT32                       EfiStatusErrorCode;

  UINT32                       ImrsSize;
  UINT32                       Alignment;
  VOID                         *PayloadAddress;
  UINTN                        PayloadFvInstance;
  EFI_PEI_FV_HANDLE            PayloadFvHandle;
  EFI_PEI_FILE_HANDLE          PayloadFileHandle;
  SI_PREMEM_CONFIG             *SiPreMemConfig;
  UINT32                       SavedTsegSize;
  IGPU_PEI_PREMEM_CONFIG       *IGpuPreMemConfig;

  DEBUG ((DEBUG_INFO, "[PeimMemoryInit]\n"));

  ZeroMem (&MrcGlobalData, sizeof (MrcParameters));
  ZeroMem (&CallTable, sizeof (CallTable));
  ZeroMem (&PlatformData, sizeof (PlatformData));

  MrcData                                  = &MrcGlobalData;
  Inputs                                   = &MrcData->Inputs;
  Inputs->Call.Func                        = &CallTable;
  MrcCall                                  = Inputs->Call.Func;
  Outputs                                  = &MrcData->Outputs;
  MrcData->MrcDataSize                     = sizeof (MrcParameters);
  MrcData->Save.Size                       = sizeof (MrcSave);
  Outputs->Size                            = sizeof (MrcOutput);
  SaveData                                 = &MrcData->Save.Data;
  SaveHeader                               = &MrcData->Save.Header;
  *((UINT32 *) &MrcData->MrcDataString[0]) = *((UINT32 *) MrcDataStringConst);
  InterpeterTrainingDone                   = FALSE;
  IsLastBasicMemoryTestPass                = TRUE;
  DidPreviousTrainingFail                  = FALSE;
  IsEfiResetColdRequired                   = FALSE;

  // Print MRC Interface Structure Sizes before the execution of the MRC.
  DEBUG_CODE_BEGIN();
    DEBUG ((
      DEBUG_INFO,
      "\nsizeof MrcInput: 0x%x, MrcOutput: 0x%x, MrcSaveData: 0x%x, MrcDebug: 0x%x\n",
      sizeof (MrcInput),
      sizeof (MrcOutput),
      sizeof (MrcSaveData),
      sizeof (MrcDebug)
      ));
  DEBUG_CODE_END();

  //
  // Obtain policy settings.
  //
  SiPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
  if (SiPreMemPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);
  if (MemConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MemConfigNoCrc = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);
  if (MemConfigNoCrc == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SiPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (SiPreMemConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  IGpuPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  if (IGpuPreMemConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CpuMemoryInitConfig, sizeof (CPU_MEMORY_INIT_CONFIG));
  CpuGetMemoryInitConfig (&CpuMemoryInitConfig);

  Inputs->ExtInputs.Ptr = &MemConfig->ExternalInputs;

  ASSERT (Inputs->ExtInputs.Ptr != NULL);

  ExtInputs         = Inputs->ExtInputs.Ptr;
  ExtInputs->Size   = sizeof (MRC_EXT_INPUTS_TYPE);

  DEBUG ((DEBUG_INFO, "SafeLoadingBiosEnableState: %x, PprRecoveryStatusEnable: %x\n",
    MemConfigNoCrc->SafeLoadingBiosEnableState, MemConfigNoCrc->PprRecoveryStatusEnable == 1));
  if ((MemConfigNoCrc->SafeLoadingBiosEnableState == 1) && (MemConfigNoCrc->PprRecoveryStatusEnable == 1)) {
    // Trigger to get Status of MRC PPR
    MemConfigNoCrc->PprTestType.Bits.XMarch = 1;
    MemConfigNoCrc->PprRepairType = hPPR;
  }

  //
  // Save MEMORY_CONFIGURATION crc at the beginning before MemConfig is overwritten.
  // Input->ExtInputs points to the MemConfig and will break the raw data of MemConfig
  //
  Inputs->SaMemCfgCrc = MrcCalculateCrc32((UINT8 *)MemConfig, sizeof (MEMORY_CONFIGURATION));

  Inputs->SaMemCfgCrcForSave = MrcCalculateCrc32((UINT8 *)MemConfig, sizeof (MEMORY_CONFIGURATION));

  Inputs->PciEBaseAddress   = PcdGet64 (PcdSiPciExpressBaseAddress);

  MrcSetupOem (MrcData, MemConfig);

  //
  // Obtain boot mode.
  //
  Status = PeiServicesGetBootMode (&SysBootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Locate FSP-M Arch Config PPI for Memory Configuration Save Data.
  //
  S3DataPtr         = NULL;
  Status = GetS3DataPtr (&S3DataPtr);
  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  Inputs->PlatformDataPtr.Pointer = &PlatformData;
  if (SysBootMode != BOOT_ON_S3_RESUME) {
    //
    // Calculate total HOB size needed for MRC
    //
    HobTotalSize = sizeof (EFI_HOB_GUID_TYPE) + sizeof (MEMORY_PLATFORM_DATA);
    MrcStatus = MrcGetHobForDataStorage ((VOID **) &Hob, HobTotalSize, &gSiMemoryPlatformDataGuid);
    if (MrcStatus != mrcSuccess) {
      DEBUG ((DEBUG_ERROR, "Cannot create MEMORY_PLATFORM_DATA_HOB\n"));
      ASSERT_EFI_ERROR (EFI_OUT_OF_RESOURCES);
      return EFI_OUT_OF_RESOURCES;
    }
    MrcStatus = MrcGetHobForDataStorage ((VOID **) &PlatformData.BdatMemoryHob[0], sizeof (BDAT_MEMORY_DATA_HOB), &gEfiMemorySchemaGuid);
    if (MrcStatus != mrcSuccess) {
      PlatformData.BdatMemoryHob[0].Pointer = NULL;
    }

    MrcStatus = MrcGetBdatSchemaListHob (&BdatSchemaListPointer);
    if (MrcStatus != mrcSuccess) {
      PlatformData.BdatSchemasHob.Pointer = NULL;
    } else {
      PlatformData.BdatSchemasHob.Pointer = BdatSchemaListPointer;
    }
  } else {
    Hob = NULL;
    PlatformData.BdatMemoryHob[0].Pointer = NULL;
  }

  Debug = &Outputs->Debug;

  if (ExtInputs->MrcTimeMeasure > 0) {
    Inputs->SerialDebugLevel = MSG_LEVEL_TIME;
  } else {
    SetSerialDebugLevel (MrcData, MemConfigNoCrc->SerialDebugLevel);
  }
  Inputs->DebugStream = (UINT32)(UINTN) PeiServices;
  //
  // Top of stack at the beginning of MRC - for stack usage calculations.
  //
  Inputs->MrcStackTop.Ptr = &FileHandle;

  MrcCall->MrcDebugHook (MrcData, MRC_INITIALIZATION_START);
  InitStat = BIOS_MSG_DID_SUCCESS;

  ForceFullTraining = 0;

  DEBUG ((DEBUG_INFO, "Inputs->SerialDebugLevel: 0x%X\n", Inputs->SerialDebugLevel));

  MRC_DEBUG_MSG_OPEN (MrcData, Inputs->SerialDebugLevel, Inputs->DebugStream);
  MrcStatsInitAndPause (MrcData);
  MrcStatus = mrcSuccess;

  //
  // Unlock memory if it is necessary.
  //
  UnlockMemory (MrcData);

  //
  // Get MRC BootMode
  //
  MrcBootMode = (SysBootMode == BOOT_ON_S3_RESUME) ? bmS3 : MrcGetBootMode (MrcData);

DEBUG_CODE_BEGIN();
  if ((SysBootMode == BOOT_ON_S3_RESUME) && (bmCold == MrcGetBootMode (MrcData))) {
    DEBUG ((
      DEBUG_ERROR,
      "ERROR: SysBootMode = %Xh and MrcBootMode = %d - Check PCH MEM_SR bit\n",
      SysBootMode,
      MrcBootMode
      ));
  }
DEBUG_CODE_END();

  Inputs->MchBarBaseAddress = (UINT32) GetHostBridgeRegisterData(HostBridgeCfgReg, MchBarCfgBaseLow);
  //
  // Set Cpu Information
  //
  if (MrcSetCpuInformation (MrcData) != mrcSuccess) {
      DEBUG ((DEBUG_ERROR, "ERROR: DDRIO IP VERSION/SEGMENT/STEPPING %Xh.%Xh.%Xh is not supported:\n",
        Inputs->DdrIoIpVersion.Bits.Version, Inputs->DdrIoIpVersion.Bits.Segment, Inputs->DdrIoIpVersion.Bits.Stepping));
      ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
  }

  if (Inputs->IsDdrphyx64) {
    Controller = 1;
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ExtInputs->DisableChannel[Controller][Channel] = 1;
    }
  }

#ifndef MDEPKG_NDEBUG
  switch (SysBootMode) {
    case BOOT_WITH_FULL_CONFIGURATION:                  Str = BootStringFc;   break;
    case BOOT_WITH_MINIMAL_CONFIGURATION:               Str = BootStringMc;   break;
    case BOOT_ASSUMING_NO_CONFIGURATION_CHANGES:        Str = BootStringNc;   break;
    case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS: Str = BootStringFcd;  break;
    case BOOT_WITH_DEFAULT_SETTINGS:                    Str = BootStringDs;   break;
    case BOOT_ON_S4_RESUME:                             Str = BootStringS4;   break;
    case BOOT_ON_S5_RESUME:                             Str = BootStringS5;   break;
    case BOOT_ON_S2_RESUME:                             Str = BootStringS2;   break;
    case BOOT_ON_S3_RESUME:                             Str = BootStringS3;   break;
    case BOOT_ON_FLASH_UPDATE:                          Str = BootStringFu;   break;
    case BOOT_IN_RECOVERY_MODE:                         Str = BootStringRm;   break;
    case BOOT_IN_RECOVERY_MODE_MASK:                    Str = BootStringRmm;  break;
    case BOOT_SPECIAL_MASK:                             Str = BootStringSm;   break;
    default:                                            Str = BootStringUnk;  break;
  }
  DEBUG ((DEBUG_INFO, "System boot mode: %Xh - BOOT_%a\n", SysBootMode, Str));
#endif

  //
  // Locate and determine if memory configuration save data is valid.
  //
  SaveDataValid = FALSE;
  DEBUG ((
           DEBUG_INFO,
           "Restore data of size %d from 0x%X\nGlobal data of size %d is at 0x%X\n",
           (S3DataPtr == NULL) ? 0 : ((MrcSave *) S3DataPtr)->Size,
           S3DataPtr,
           sizeof (MrcParameters),
           &MrcData
           ));
  DEBUG ((DEBUG_INFO, "S3DataPtr: %x check completed, SysBootMode: %x\n", S3DataPtr, SysBootMode));
  if ((S3DataPtr != NULL) && (SysBootMode != BOOT_WITH_DEFAULT_SETTINGS  || MemConfigNoCrc->DisableMrcRetraining == 1)) {
    SaveSys = (MrcSave *) S3DataPtr;
    Save    = SaveSys;
    Crc32 = MrcCalculateCrc32 ((UINT8 *) (&Save->Data), sizeof (MrcSaveData));
    DEBUG ((DEBUG_INFO, "Calc. crc = 0x%x, Header crc = 0x%x\n", Crc32, Save->Header.Crc));
    if (Crc32 == Save->Header.Crc) {
      DEBUG ((DEBUG_INFO, "Saved memory configuration data is valid\n"));
      ((*PeiServices)->CopyMem) ((VOID *) &MrcData->Save, (VOID *) SaveSys, sizeof (MrcSave));
      SaveDataValid = TRUE;
    }
  }
  //
  // We must have memory configuration save data in order to resume from S3.
  //
  if ((SysBootMode == BOOT_ON_S3_RESUME) && (!SaveDataValid)) {
    DEBUG ((DEBUG_ERROR, "Unable to resume from S3 without valid saved memory configuration data\n"));
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }

  if (!(MrcBootMode == bmS3 || MrcBootMode == bmWarm)) {
    //
    // Check CPU Replaced Status, if so a system non-power cycle reset will be required.
    //
    DEBUG ((DEBUG_INFO, "Calling CpuReplacementCheck\n"));
    CpuReplacementCheck(FileHandle, &ForceFullTraining);
    DEBUG ((DEBUG_INFO, "ForceFullTraining %x\n", ForceFullTraining));
    if (ForceFullTraining == 0x1) {
      SaveDataValid = FALSE;
    }
  }

  SskpdValue.Data = MrcWmRegGet (MrcData);

  // Telemetry Health Report
  // If sticky scratchpad SSKPD_PCU_SKPD_MEM_BASICMEMORYTEST_FAIL bit is set,
  // there was a BasicMemoryTest failure during the previous flow.
  // Report the health check result to Telemetry Health Driver,
  // and then clear the bit.
  if (SskpdValue.Bits.MEM_BASICMEMORYTEST_FAIL != 0) {
    if (ExtInputs->MrcFastBoot > 0) {
        IsLastBasicMemoryTestPass = FALSE;
    }
    MrcWmRegClrBits (MrcData, SSKPD_PCU_SKPD_MEM_BASICMEMORYTEST_FAIL);
    SskpdValue.Bits.MEM_BASICMEMORYTEST_FAIL = 0;
  }

  Inputs->TxtClean = IsTxtSecretsSet ();

  // In case of Txt Enabled, Ibecc is in non-protective mode (bypass), and will be enabled after TxtACheck is done
  Inputs->IsIbeccEnabled = ExtInputs->Ibecc && !IsTxtACheckRequested();

  //
  // MrcBootMode can ONLY be bmCold, bmWarm or bmS3 at this point.
  //
  switch (MrcBootMode) {
    case bmCold:
      // Advance the MRC boot mode to fast boot if the following condition is met.
      // If sticky scratchpad is set, there was a failure during previous Fast flow, so use Cold.
      if ((SaveDataValid == TRUE) &&
          (ExtInputs->MrcFastBoot > 0) &&
          (SskpdValue.Bits.MRC_RUNNING == 0) &&
          (ColdBootRequired (MrcData, MemConfig, &CpuMemoryInitConfig, MrcBootMode, &IsEfiResetColdRequired) == FALSE)
          ) {
        MrcBootMode = bmFast;
        DEBUG ((DEBUG_INFO, "Fast boot is possible, so forcing it\n"));
      } else {
        DEBUG ((DEBUG_INFO, "Cold boot\n"));
        SaveDataValid = FALSE;
        if (SskpdValue.Bits.MRC_RUNNING == 1) {
          /*
           * If MRC training fails at XMP profile when DMB=TRUE,
           * MRC triggers WarmReset. After reset, MrcGetBootMode
           * returns _ColdBoot_ and SskpdValue.Bits.MRC_RUNNING equals non-zero.
           */
          DidPreviousTrainingFail = TRUE;
        }
      }
      break;

    case bmWarm:
    case bmS3:
      if (SaveDataValid == FALSE) {
        DEBUG ((DEBUG_WARN, "Saved memory configuration data is not valid, forcing a cold boot\n"));
        MrcBootMode = bmCold;
        break;
      } else {
        if (ColdBootRequired (MrcData, MemConfig, &CpuMemoryInitConfig, MrcBootMode, &IsEfiResetColdRequired) == TRUE) {
          DEBUG ((
            DEBUG_WARN,
            "Platform settings or configuration have changed, forcing a cold boot\n"
            ));
          MrcBootMode = bmCold;
          SaveDataValid = FALSE;
          break;
        }
        if ((MemConfigNoCrc->CleanMemory == TRUE) && (MrcBootMode == bmWarm)) {
          DEBUG ((
            DEBUG_WARN,
            "TCG MOR request, forcing fast boot flow to scrub memory\n"
            ));
          MrcBootMode = bmFast;
          break;
        }
        if (Inputs->TxtClean && (MrcBootMode == bmWarm)) {
          DEBUG ((
            DEBUG_WARN,
            "TXT Clean detected in Warm flow, forcing fast boot flow to scrub memory\n"
            ));
          MrcBootMode = bmFast;
          break;
        }
        //
        // Do not force cold boot when BOOT_ON_FLASH_UPDATE since the memory content should be retained.
        //
        if (SysBootMode != BOOT_ON_FLASH_UPDATE) {
          /*
            Check SSKPD register to determine if Warm Reset occurred before MRC was reached during a cold boot.
            If so, we need to choose Fast or Cold boot path.
          */
          if ((MrcBootMode == bmWarm) && (SskpdValue.Bits.MRC_RUNNING == 0)) {
            DEBUG ((
              DEBUG_WARN,
              "Reset occurred in the cold boot path before reaching MRC.\n"
              ));
            if ((ExtInputs->MrcFastBoot > 0) && IsLastBasicMemoryTestPass) {
              DEBUG ((DEBUG_INFO, "Fast boot is possible, so forcing it\n"));
              MrcBootMode = bmFast;
            } else {
              DEBUG ((DEBUG_INFO, "Previous boot cycle recorded Memory Test Failure forcing Cold boot\n"));
              MrcBootMode = bmCold;
              SaveDataValid = FALSE;
            }
            break;
          }
        }
        Inputs->LastIbeccOperationMode = GetLastIbeccOpMode (MrcData);
        if (Inputs->IsIbeccEnabled && Inputs->LastIbeccOperationMode == IbeccNonProtect) {
          DEBUG ((DEBUG_INFO, "Previous boot cycle recorded IBECC IP in Bypass Mode, change to Fast Boot\n"));
          // System Transitioned from IBECC IP Enabled in Bypass Mode to Enabled in FullProtectMode on warm boot
          MrcBootMode = bmFast;
          break;
        }
      }
      DEBUG ((DEBUG_INFO, (MrcBootMode == bmS3) ? "Resume from S3\n" : "Warm reset\n"));
      break;

    default:
      DEBUG ((DEBUG_INFO, "Cold boot\n"));
      MrcBootMode = bmCold;
      SaveDataValid = FALSE;
      break;
  }

  if (IsEfiResetColdRequired) {
    DEBUG ((DEBUG_WARN, "Forcing a power cycle..\n"));
    (*PeiServices)->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
  }
  if (MrcBootMode == bmFast && ExtInputs->MrcFastBoot == 0) {
    DEBUG ((DEBUG_WARN, "Fast Boot Possible But Disabled\n"));
    MrcBootMode = bmCold;
  }
  if (MrcBootMode == bmFast && !IsLastBasicMemoryTestPass) {
    DEBUG ((DEBUG_WARN, "Previous boot cycle recorded Memory Test Failure forcing Cold boot\n"));
    MrcBootMode = bmCold;
  }
  if (MrcBootMode == bmCold) {
    // MRC Cold Boot Required signal
    Status = PeiServicesInstallPpi (&gMrcColdBootRequiredPpi);
    ASSERT_EFI_ERROR (Status);
  }

  // Set sticky bit in case system hangs during training
  MrcWmRegSetBits (MrcData, SSKPD_PCU_SKPD_MRC_RUNNING);

  //
  // Clear MrcSave if not valid saved data. We don't want to end up with Ghost DIMMs
  //
  if (SaveDataValid == FALSE) {
    ZeroMem (&MrcData->Save, sizeof (MrcSave));
  }

  //
  // Calculate Total DPR Size
  //
  CalculateTotalDprMemorySize (&TotalDprSizeMB);
  if (TotalDprSizeMB != 0) {
    Inputs->DprSize = (UINT32) TotalDprSizeMB;
  }

  //
  // Set up the MRC input data structure.
  //
  PERF_INMODULE_BEGIN ("MrcSetupMrcData");
  Inputs->BootMode = MrcSetupMrcData (
                       SysBootMode,
                       MrcBootMode,
                       MrcData,
                       MemConfig,
                       MemConfigNoCrc,
                       SiPreMemPolicy,
                       DidPreviousTrainingFail
                       );
  PERF_INMODULE_END ("MrcSetupMrcData");

  //  This shall be evaluated after MrcSetupMrcData is done
  Inputs->PprEnable = MrcIsPprEnabled (MrcData);

  //
  // Initialize MeStolenSize to 0 before we retrieving from ME FW.
  //
  Inputs->MeStolenSize = 0;

  if (MrcBootMode != bmS3) {
    ImrsSize       = 0;
    Alignment      = 0;
    Status = MeImrConfig (&ImrsSize, &Alignment);
    if (Status == EFI_OUT_OF_RESOURCES) {
      InitStat = BIOS_MSG_DID_NO_MEMORY;
      Outputs->MemoryMapData.MeStolenBase = 0x0;
    }
    Inputs->MeStolenSize = ImrsSize;
    Inputs->ImrAlignment = Alignment;
  }

  Inputs->UcPayloadAddress = 0;
  PayloadFvHandle = NULL;
  PayloadFvInstance = 0;
  PayloadAddress = NULL;
  while (TRUE) {
    //
    // Traverse all firmware volume instances.
    //
    Status = PeiServicesFfsFindNextVolume (PayloadFvInstance, &PayloadFvHandle);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "MRC UC payload not found!\n"));
      break;
    }

    FileHandle = NULL;
    Status = PeiServicesFfsFindFileByName (&gMemoryUcPayloadGuid, PayloadFvHandle, &PayloadFileHandle);
    if (!EFI_ERROR (Status)) {
      //
      // Search RAW section.
      //
      Status = PeiServicesFfsFindSectionData (EFI_SECTION_RAW, PayloadFileHandle, &PayloadAddress);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "MRC UC payload at %x\n", (UINTN) PayloadAddress));
        Inputs->UcPayloadAddress = (UINTN) PayloadAddress;
        break;
      }
    }

    //
    // Search the next volume.
    //
    PayloadFvInstance++;
  }

  // Regular SAGV case - Read SAGV point for Warm / S3 resume flow
  if ((MrcBootMode == bmWarm) || (MrcBootMode == bmS3)) {
    MrcGetSaGvPointBeforeReset (MrcData);
  }

  if (MrcBootMode == bmCold) {
    if (IS_VGA_INIT_ON_MRC_ONLY (IGpuPreMemConfig->VgaInitControl)) {
      IGpuVgaInit (IGpuPreMemConfig);
    }
  }

  do {
    if ((Inputs->BootMode == bmCold) || (Inputs->BootMode == bmFast)) {
      //
      // Clear DRAM Init Bit if we are doing a cold boot, to prevent hang if a warm reset occurs in the training flow
      // where an old memory config is saved.
      //
      MrcResetDISB (MrcData);
    }
    MrcStatus = mrcSuccess;
    if ((mrcSuccess == MrcStatus) && (!InterpeterTrainingDone)) {
      DEBUG ((DEBUG_INFO, "MRC Start Memory Configuration\n"));
      PERF_INMODULE_BEGIN ("MrcStartMemoryConfig");
      MrcStatus = MrcStartMemoryConfiguration (MrcData, 0, NULL, TRUE);
      PERF_INMODULE_END ("MrcStartMemoryConfig");
    }

    if (MrcStatus == mrcColdBootRequired) {
      // MRC Cold Boot Required signal
      Status = PeiServicesInstallPpi (&gMrcColdBootRequiredPpi);
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_INFO, "\n Install MRC Boot Required PPI. MrcStatus: %d \n", MrcStatus));
    }

    EfiStatusErrorCode = EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NON_SPECIFIC;
    switch (MrcStatus) {
      case mrcSuccess:
        // Clean-up after progress bar-related calculations
        SaveData->PostCodesDone = 0;
        SaveData->PostCodesTotal = 0;

        if (ExtInputs->RetrainToWorkingChannel && (Inputs->BootMode == bmCold)) {
          SaveData->SaMemCfgCrc = MrcCalculateCrc32((UINT8 *)MemConfig, sizeof (MEMORY_CONFIGURATION));
          SaveHeader->Crc       = MrcCalculateCrc32 ((UINT8 *) SaveData, sizeof (MrcSaveData));
          MrcData->Save.Size    = sizeof (MrcSave);
          DEBUG ((DEBUG_INFO, "Updated Mem Cfg CRC = 0x%x\n", SaveData->SaMemCfgCrc));
          DEBUG ((DEBUG_INFO, "Updated Saved Data CRC = 0x%x\n", SaveHeader->Crc));
        }
        break;

      case mrcColdBootRequired:
        // The MrcSave data is no longer valid, clear it to avoid Ghost DIMMs and SAGV points on the next MRC call.
        ZeroMem (&MrcData->Save, sizeof (MrcSave));
        if (Inputs->BootMode == bmFast) {
          // At this point, input structure has limited data.
          // We need to initialize the input structure for the cold boot.
          MrcBootMode = bmCold;
          Inputs->BootMode = MrcSetupMrcData (
                               SysBootMode,
                               MrcBootMode,
                               MrcData,
                               MemConfig,
                               MemConfigNoCrc,
                               SiPreMemPolicy,
                               DidPreviousTrainingFail
                               );
        } else {
          Inputs->BootMode = bmCold;
        }
        break;

      case mrcDramNotSupportEdvfsc:
        // Clear MrcSave and MrcOutputs as we are going to restart the MRC core with ExtInputs->DvfscEnabled = 0
        ZeroMem (&MrcData->Save, sizeof (MrcSave));
        ZeroMem (&MrcData->Outputs, sizeof (MrcOutput));
        MRC_DEBUG_MSG_OPEN (MrcData, Inputs->SerialDebugLevel, Inputs->DebugStream);
        ExtInputs->DvfscEnabled = FALSE;
        DEBUG ((DEBUG_ERROR, "Re-run MRC with E-DVFSC disabled\n"));
        break;

      case mrcResetFullTrain:
        // Margin Check failed, reset and do full training
        REPORT_STATUS_CODE (EFI_ERROR_CODE, EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_CORRECTABLE);
        MrcWmRegSetBits (MrcData, SSKPD_PCU_SKPD_MEM_BASICMEMORYTEST_FAIL);
        IoWrite16 (0x80, 0);  // Clear 16-bit port80
        (*PeiServices)->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
        break;

      case mrcUnsupportedTechnology:
      case mrcDimmNotExist:
        MrcCall->MrcDebugHook (MrcData, MRC_NO_MEMORY_DETECTED);
        if (MrcStatus == mrcUnsupportedTechnology) {
          DEBUG ((DEBUG_ERROR, "DIMM is present but not supported\n"));
          EfiStatusErrorCode = EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_INVALID_TYPE;
        } else {
          DEBUG ((DEBUG_ERROR, "There are no DIMMs present in the system\n"));
          EfiStatusErrorCode = EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_EC_NONE_DETECTED;
        }
        // no break;

      default:
        if ((ExtInputs->RetrainToWorkingChannel) && (MrcStatus != mrcDimmNotExist) && (MrcStatus != mrcUnsupportedTechnology)) {
          DEBUG ((DEBUG_INFO, "MRC ReTrain to Working Channel Enabled\n"));
          if (MrcDisableFailingChannels (MrcData, WRITE_SAVE_TO_SCRATCHPAD) != TRUE) {
            IoWrite16 (0x80, 0);  // Clear 16-bit port80
            (*PeiServices)->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
            break;
          }
        }

        DEBUG ((DEBUG_ERROR, "Memory initialization has failed\n"));
        REPORT_STATUS_CODE (EFI_ERROR_CODE, EfiStatusErrorCode);
        // REPORT_STATUS_CODE will write an EFI status code to the post code IO port 80
        // Write back the last MRC post code so that the last post code seen is the MRC post code
        (MrcCall->MrcIoWrite16)(0x80, Debug->PostCode[MRC_POST_CODE]);

        if ((MrcStatus == mrcDimmNotExist) || (MrcStatus == mrcUnsupportedTechnology)) {
          (MrcCall->MrcIoWrite16)(0x80, MRC_NO_MEMORY_DETECTED);
          InitStat = BIOS_MSG_DID_NO_MEMORY;
        } else {
          // Replace the upper byte (0x81) with MRC_FAILURE_INDICATION
          (MrcCall->MrcIoWrite8) (0x81, MRC_FAILURE_INDICATION);
          InitStat = BIOS_MSG_DID_INIT_ERROR;
        }
        //
        // Send DRAM Init Done to ME FW, indicating either 'no DRAM' or 'MRC failure'
        //
        MeConfigDidReg (
          FileHandle,
          InitStat,
          (Outputs->MemoryMapData.MeStolenBase & 0xFFF) << 20, // Low DWORD
          Outputs->MemoryMapData.MeStolenBase >> 12 // High DWORD
          );
        ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
        return EFI_DEVICE_ERROR;
    }
  } while ((MrcStatus == mrcColdBootRequired) || (MrcStatus == mrcDramNotSupportEdvfsc));

  DEBUG ((
    DEBUG_INFO,
    "Lowest StackAddr: 0x%x\tTop StackAddr: 0x%x\tDepth: %d\n",
    Debug->LowestStackAddr.DataN,
    Debug->TopStackAddr.DataN,
    Debug->TopStackAddr.DataN - Debug->LowestStackAddr.DataN
    ));

  //
  // Intel Silicon View Technology (ISVT) IO Reading port with AH = 0x10 for End of MRC
  //
  MrcCall->MrcReturnFromSmc(MrcData, MrcStatus);

  //
  // Update Inputs->TsegSize before calling PeiMemorySubSystemInit
  //
  SavedTsegSize = Inputs->TsegSize;
  if ((Inputs->TsegSize > 0) && (MemConfigNoCrc != NULL)) {
    if ((MemConfigNoCrc->SafeLoadingBiosEnableState == 1) || (MemConfigNoCrc->TsegMemoryTestStatus == mrcSuccess)) {
      if ((MemConfigNoCrc->RetryCount >= 1) && (MemConfigNoCrc->RetryCount <= 3) && (Outputs->MrcPprStatus != mrcSuccess)) {
        // Reserve Failed Tesg in Tesg region
        Inputs->TsegSize = SavedTsegSize * (MemConfigNoCrc->RetryCount + 1);
        DEBUG ((DEBUG_INFO, "Reserving Failed Tseg in Tseg region, Inputs->TsegSize = 0x%X\n", Inputs->TsegSize));
      }
    }
  }

  // Set the last IBECC operation mode
  SetLastIbeccOpMode (Outputs->FinalIbeccOperationMode);

  if (Inputs->TxtClean) {
    if (Outputs->TxtScrubSuccess) {
      // Clear the sticky scratchpad to indicate that MRC is finished,
      // so that we can run Fast flow on the next boot
      MrcWmRegClrBits (MrcData, SSKPD_PCU_SKPD_MRC_RUNNING);
    }
  }

  //
  // Set MEM_CONFIG_DONE
  //
  PeiMemorySubSystemInit (SiPreMemPolicy, MrcData);
  // Restore Inputs->TsegSize
  Inputs->TsegSize = SavedTsegSize;

  if (MemConfigNoCrc->SafeLoadingBiosEnableState == 1) {
    MemConfigNoCrc->MrcPprStatus = Outputs->MrcPprStatus;
    DEBUG ((DEBUG_INFO, "MrcPprStatus: %x\n",MemConfigNoCrc->MrcPprStatus));
    Status = PeiServicesInstallPpi(&mTsegMemoryTestInitPpi);
    ASSERT_EFI_ERROR(Status);
  }

  if (Inputs->BootMode != bmS3) {
    // Polling For IBECC_MEMORY_INIT_CONTROL.INIT_ALL_RANGES
    MrcStatus = MrcPollIbeccFSMInit (MrcData);
    if (Status != mrcSuccess) {
      ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
      return EFI_DEVICE_ERROR;
    }
  }

  PERF_INMODULE_BEGIN ("MrcBasicMemoryTest");
  if ((MrcBootMode == bmCold) || (MrcBootMode == bmFast)) {
    //
    // Perform simple memory test.
    //
    if (mrcFail == BasicMemoryTest (MrcData)) {
      if (MrcBootMode == bmFast) {
        if (ExtInputs->RetrainOnFastFail) {
          //
          // BasicMemoryTest failed in Fast flow - fall back to Cold boot flow.
          //
          DEBUG ((DEBUG_ERROR, "BasicMemoryTest failed in Fast flow - reset to run MRC in Cold flow !!\n\n\n"));
          REPORT_STATUS_CODE (EFI_ERROR_CODE, EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NON_SPECIFIC);
          MrcWmRegSetBits (MrcData, SSKPD_PCU_SKPD_MEM_BASICMEMORYTEST_FAIL);
          IoWrite16 (0x80, 0);  // Clear 16-bit port80
          (*PeiServices)->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
        }
      }
      MrcCall->MrcDebugHook (MrcData, MRC_MEM_INIT_DONE_WITH_ERRORS);
      PERF_INMODULE_END ("MrcBasicMemoryTest");
      //
      // Send DRAM Init Done to ME FW, indicating 'MRC failure'
      //
      InitStat = BIOS_MSG_DID_INIT_ERROR;
      MeConfigDidReg (
        FileHandle,
        InitStat,
        (Outputs->MemoryMapData.MeStolenBase & 0xFFF) << 20, // Low DWORD
        Outputs->MemoryMapData.MeStolenBase >> 12 // High DWORD
        );
      ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
      return EFI_DEVICE_ERROR;
    }
  } else {
    //
    // Perform simple non-destructive memory test for Warm / S3 resume.
    // On S3 Resume run this test only for Debug builds, to avoid S3 exit latency increase for Release builds.
    //
#ifdef MDEPKG_NDEBUG
    if (MrcBootMode != bmS3) {
#endif // MDEPKG_NDEBUG
      if (mrcFail == BasicMemoryTestS3 (MrcData)) {
        MrcCall->MrcDebugHook (MrcData, MRC_MEM_INIT_DONE_WITH_ERRORS);
        PERF_INMODULE_END ("MrcBasicMemoryTest");
        //
        // Send DRAM Init Done to ME FW, indicating 'MRC failure'
        //
        InitStat = BIOS_MSG_DID_INIT_ERROR;
        MeConfigDidReg (
          FileHandle,
          InitStat,
          (Outputs->MemoryMapData.MeStolenBase & 0xFFF) << 20, // Low DWORD
          Outputs->MemoryMapData.MeStolenBase >> 12 // High DWORD
          );
        ASSERT_EFI_ERROR (EFI_DEVICE_ERROR);
        return EFI_DEVICE_ERROR;
      }
#ifdef MDEPKG_NDEBUG
    }
#endif // MDEPKG_NDEBUG
  }
  PERF_INMODULE_END ("MrcBasicMemoryTest");

  //
  // This Function will Configure the PSMI Handler Base and Trace Regions
  //
  ConfigurePsmiHandler ();

  //
  // Poll MAILBOX_BIOS_CMD_MISC.Path_to_memory_available to be set before DID flow.
  // This bit indicates Path to memory available.
  //
  PollPathToMemAvailable ();

  DEBUG ((DEBUG_INFO, "Calling MeConfigDidReg\n"));
  MeConfigDidReg (
    FileHandle,
    InitStat,
    (Outputs->MemoryMapData.MeStolenBase & 0xFFF) << 20, // Low DWORD
    Outputs->MemoryMapData.MeStolenBase >> 12 // High DWORD
    );
  DEBUG ((DEBUG_INFO, "CSE IMR Base Low  = 0x%08X\n", (Outputs->MemoryMapData.MeStolenBase & 0xFFF) << 20));
  DEBUG ((DEBUG_INFO, "CSE IMR Base High = 0x%08X\n", Outputs->MemoryMapData.MeStolenBase >> 12));
  DEBUG ((DEBUG_INFO, "MeDramInitDone Complete.\n"));

  //
  // If memory is not preserved across reset. Based on SkipOverrideBootModeWhenFwUpdate policy.
  // 0: Proceed through normal boot (with override boot mode).
  // 1: Proceed through boot on flash update (without override boot mode).
  //
  if (((MrcBootMode == bmCold) || (MrcBootMode == bmFast)) && (SysBootMode == BOOT_ON_FLASH_UPDATE)) {
    if (SiPreMemConfig->SkipOverrideBootModeWhenFwUpdate) {
      DEBUG ((DEBUG_INFO, "Skip override boot mode when updating flash.\n"));
    } else {
      DEBUG ((DEBUG_INFO, "Proceed normal boot instead of boot on flash update.\n"));
      SysBootMode = BOOT_WITH_FULL_CONFIGURATION;
      Status = PeiServicesSetBootMode (SysBootMode);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        return Status;
      }
    }
  }

  //  MRC is responsible to report the result of BasicMemoryTest to Telemetry as MRC health indicator.
  Outputs->MrcBasicMemoryTestPass = IsLastBasicMemoryTestPass;

  //
  // Install EFI memory HOBs
  //
  if (SysBootMode != BOOT_ON_S3_RESUME) {
#ifdef MRC_DEBUG_PRINT
    Debug->Current.DataN = 0;
#endif // MRC_DEBUG_PRINT
    BuildMemoryPlatformDataHob (MrcData, Hob);
  }
  BuildMemoryInfoDataHob (MrcData);

  Status = InstallEfiMemory(MrcData, PeiServices, SysBootMode);
  ASSERT_EFI_ERROR(Status);

  if (SysBootMode != BOOT_ON_S3_RESUME) {
    BuildMemoryS3DataHob (MrcData);
  }

  //
  // Set IMR Exclusion Registers
  //
  PeiImrExclusionInit ();

  //
  // This function Print Memory Map.
  //
  PrintMemoryMap (MrcData);
  CheckMemoryMapOverlap ();

  DEBUG ((DEBUG_INFO, "MemoryInit Complete.\n"));
  MrcCall->MrcDebugHook (MrcData, MRC_MEM_INIT_DONE);

  return Status;
}

/**
  This function installs memory for all paths except S3 resume.

  @param[in] MrcData     - Mrc data structure
  @param[in] PeiServices - PEI Services table.
  @param[in] SysBootMode - The specific boot path that is being followed.

  @retval EFI_SUCCESS          The function completed successfully.
  @retval EFI_OUT_OF_RESOURCES Out of Resources.
**/
EFI_STATUS
InstallEfiMemory (
  IN MrcParameters *CONST      MrcData,
  IN CONST EFI_PEI_SERVICES    **PeiServices,
  IN EFI_BOOT_MODE             SysBootMode
  )
{
  IN CONST MrcInput                     *Inputs;
  EFI_STATUS                            Status;
  UINT8                                 Index;
  UINT8                                 NumRanges;
  UINT8                                 SmramIndex;
  UINT8                                 SmramRanges;
  UINT64                                PeiMemoryLength;
  UINT64                                InstalledPeiMemoryLength;
  UINT64                                FspReportedHobMemoryLength;
  UINT64                                RangeLength;
  UINTN                                 BufferSize;
  UINTN                                 PeiMemoryIndex;
  EFI_PEI_HOB_POINTERS                  Hob;
  EFI_PHYSICAL_ADDRESS                  PeiMemoryBaseAddress;
  EFI_PHYSICAL_ADDRESS                  InstalledPeiMemoryBaseAddress;
  UINTN                                 RequiredMemSize;
  EFI_PHYSICAL_ADDRESS                  TopUseableMemAddr;
  MEMORY_TEST_LEVEL                     MemoryTestLevel;
  EFI_PHYSICAL_ADDRESS                  SeamRrBaseAddress;
  UINT32                                StreamTracerMemSize;
  UINT64                                StreamTracerBaseAddr;
  EFI_SMRAM_HOB_DESCRIPTOR_BLOCK        *SmramHobDescriptorBlock;
  EFI_SMRAM_DESCRIPTOR                  *SmramDescriptor;
  PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE MemoryMap[MAX_RANGES];
  EFI_PHYSICAL_ADDRESS                  BadMemoryAddress;
  EFI_RESOURCE_TYPE                     ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE           ResourceAttribute;
  SI_PREMEM_POLICY                      *SiPreMemPolicy;
  MEMORY_CONFIG_NO_CRC                  *MemConfigNoCrc;
  EFI_RESOURCE_ATTRIBUTE_TYPE           ResourceAttributeTested;
  UINT64                                TopMemSize;
  UINT64                                Touud;
  UINT32                                BootLoaderTolumSize;
  BOOLEAN                               IsTsegRegion;
  MEMORY_SUB_SYSTEM_CONFIG              *MemorySubSystemConfig;
  TSE_DATA_HOB                          *TseDataHob;
  UINT32                                TseDramSizeInBytes;
  UINT64                                TseDramMemBaseAddr;
  MrcOutput                             *Outputs;
  MrcMemoryMap                          *MemoryMapData;
  EFI_PHYSICAL_ADDRESS                  RemapBase;
  EFI_PHYSICAL_ADDRESS                  RemapLimit;
  UINT64                                Tom;
  PEI_DDR_MAIN_MEMORY_MAP_RANGE         MainMemoryMap[MAX_RANGES];
  UINT8                                 MainMemoryIndex;
  SI_PREMEM_CONFIG                      *SiPreMemConfig;
  UINT64                                StaticContentSize;
  UINT64                                NocImrExclusionLimit;
  UINT64                                NocImrExclusionSize;
  UINT64                                NocImrExclusionDelta;

  TseDataHob = NULL;
  MemorySubSystemConfig = NULL;

  Inputs        = &MrcData->Inputs;
  Outputs       = &MrcData->Outputs;
  MemoryMapData = &Outputs->MemoryMapData;

  DEBUG ((DEBUG_INFO, "Install EFI memory.\n"));

  //
  // Get the Memory Map
  //
  NumRanges = MAX_RANGES;
  ZeroMem (MemoryMap, sizeof (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE) * NumRanges);
  Status = GetMemoryMap (MrcData, (PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE *) MemoryMap, &NumRanges);
  ASSERT_EFI_ERROR (Status);

  //
  // Find the highest memory range in processor native address space to give to
  // PEI. Then take the top.
  //
  PeiMemoryBaseAddress = 0;
  InstalledPeiMemoryBaseAddress = 0;
  InstalledPeiMemoryLength = 0;
  ResourceAttributeTested = 0;
  ResourceAttribute = 0;
  StaticContentSize = 0;

  FspReportedHobMemoryLength = 0;
  //
  // Query the platform for the minimum memory size.
  //
  PeiMemoryLength = 0;
  RequiredMemSize = 0;
  SiPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );

  SiPreMemConfig = NULL;
  if (SiPreMemPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gSiPreMemConfigGuid, (VOID *) &SiPreMemConfig);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Unable to Get gSiPreMemConfigGuid block\n"));
      ASSERT_EFI_ERROR (Status);
    }
  }

  MemConfigNoCrc = NULL;
  if (SiPreMemPolicy != NULL) {
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Unable to Get gMemoryConfigNoCrcGuid block\n"));
      MemConfigNoCrc = NULL;
      ASSERT_EFI_ERROR (Status);
    }
    if (MemConfigNoCrc != NULL) {
      PeiMemoryLength = MemConfigNoCrc->PlatformMemorySize;
    }
  }
  RetrieveMemorySizeInfo (PeiMemoryLength, &RequiredMemSize, &BootLoaderTolumSize);
  if (!RequiredMemSize)
  {
    RetrieveRequiredMemorySize (&RequiredMemSize);
  }
  DEBUG ((DEBUG_INFO, "Required memory size = 0x%x bytes\n", RequiredMemSize));
  PeiMemoryIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    DEBUG ((DEBUG_INFO, "Found %016Xh bytes at ", MemoryMap[Index].RangeLength));
    DEBUG ((DEBUG_INFO, "%016Xh\n", MemoryMap[Index].PhysicalAddress));
    if ((MemoryMap[Index].Type == DualChannelDdrMainMemory) &&
        (MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength < MAX_ADDRESS) &&
        (MemoryMap[Index].PhysicalAddress >= PeiMemoryBaseAddress) &&
        (MemoryMap[Index].RangeLength >= PeiMemoryLength)) {
      PeiMemoryBaseAddress = MemoryMap[Index].PhysicalAddress + MemoryMap[Index].RangeLength - PeiMemoryLength;
      PeiMemoryIndex       = Index;
    }
  }

  //
  // Test only the PEI memory if necessary. Some platforms do not require the
  // Base Memory PEIM to be present.
  //
  switch (SysBootMode) {

  case BOOT_WITH_FULL_CONFIGURATION:
    MemoryTestLevel = Quick;
    break;

  case BOOT_WITH_FULL_CONFIGURATION_PLUS_DIAGNOSTICS:
    MemoryTestLevel = Extensive;
    break;

  default:
    MemoryTestLevel = Ignore;
    break;
  }
  if (SysBootMode != BOOT_ON_S3_RESUME) {
    ResourceAttributeTested = EFI_RESOURCE_ATTRIBUTE_TESTED;

   (*PeiServices)->ReportStatusCode (
             PeiServices,
             EFI_PROGRESS_CODE,                                  // Type
             EFI_COMPUTING_UNIT_MEMORY | EFI_CU_MEMORY_PC_TEST,  // Value
             0,    // Instance
             NULL, // *CallerId OPTIONAL
             NULL  // *Data OPTIONAL
             );
  }

  if (MemConfigNoCrc != NULL) {
    if ((MrcData->Inputs.BootMode == bmWarm) && (MemConfigNoCrc->MemTestOnWarmBoot == 0)) {
      MemoryTestLevel = Ignore;
    }
  }

  Status = BaseMemoryTest (
             MrcData,
             PeiMemoryBaseAddress,
             PeiMemoryLength,
             MemoryTestLevel,
             &BadMemoryAddress
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Memory test failure at %lXh.\n", BadMemoryAddress));
    REPORT_STATUS_CODE (EFI_ERROR_CODE, EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NON_SPECIFIC);
  }

  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
  }
  if (SysBootMode != BOOT_ON_S3_RESUME) {
      InstalledPeiMemoryBaseAddress = PeiMemoryBaseAddress;
      //
      // Carve out the top memory reserved for ACPI.
      //
      InstalledPeiMemoryLength = PeiMemoryLength - RequiredMemSize;
      FspReportedHobMemoryLength = PeiMemoryLength;
  } else {
    InstalledPeiMemoryBaseAddress = MemorySubSystemConfig->AcpiReservedMemoryBase;
    InstalledPeiMemoryLength = MemorySubSystemConfig->AcpiReservedMemorySize;
    FspReportedHobMemoryLength = 0;
  }
  GetInstalledPeiMemoryInfo (
    PeiMemoryLength,
    PeiMemoryBaseAddress,
    BootLoaderTolumSize,
    RequiredMemSize,
    &InstalledPeiMemoryBaseAddress,
    &InstalledPeiMemoryLength,
    &FspReportedHobMemoryLength
  );

  //
  // Once PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE is moved to MemorySubSystem, this
  // preprocessing can be removed and passing whole PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE table
  // to PeiMemoryBeforeInstallingEfiMemory() rather than PEI_DDR_MAIN_MEMORY_MAP_RANGE.
  //
  ZeroMem (MainMemoryMap, sizeof (PEI_DDR_MAIN_MEMORY_MAP_RANGE) * MAX_RANGES);
  MainMemoryIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if (MemoryMap[Index].Type == DualChannelDdrMainMemory) {
      MainMemoryMap[MainMemoryIndex].PhysicalAddress = MemoryMap[Index].PhysicalAddress;
      MainMemoryMap[MainMemoryIndex].RangeLength     = MemoryMap[Index].RangeLength;
      MainMemoryIndex++;
    }
  }

  //
  // Give a chance for Capsule pre-processing before installing EFI memory.
  //
  CoalesceCapsulesIfNeeded (
    MainMemoryMap,
    MainMemoryIndex,
    InstalledPeiMemoryBaseAddress,
    PeiMemoryLength
    );

  Status = PeiServicesInstallPeiMemory (InstalledPeiMemoryBaseAddress, InstalledPeiMemoryLength);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Building RESOURCE_SYSTEM_MEMORY Hob: \n"));
  DEBUG ((DEBUG_INFO, "PeiMemoryBaseAddress = %lXh, PeiMemoryLength = %lXh\n", InstalledPeiMemoryBaseAddress, InstalledPeiMemoryLength));

  Status = InstallFspHobs (
             PeiMemoryBaseAddress + PeiMemoryLength - BootLoaderTolumSize,
             BootLoaderTolumSize,
             InstalledPeiMemoryBaseAddress,
             InstalledPeiMemoryLength,
             ResourceAttributeTested
           );

  if (Status == EFI_UNSUPPORTED) { // FSP Dispatch Mode
    if (SysBootMode != BOOT_ON_S3_RESUME) {
      BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          ResourceAttributeTested |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
       PeiMemoryBaseAddress,
       PeiMemoryLength
       );
    }
  }

  //
  // Install physical memory descriptor hobs for each memory range.
  //
  SmramRanges = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    IsTsegRegion = FALSE;
    if (MemoryMap[Index].Type == DualChannelDdrMainMemory) {
      if (Index == PeiMemoryIndex) {
        //
        // This is a partially tested Main Memory range, give it to EFI
        //
        RangeLength = MemoryMap[Index].RangeLength - FspReportedHobMemoryLength;
      } else {
        //
        // This is an untested Main Memory range, give it to EFI.
        //
        RangeLength = MemoryMap[Index].RangeLength;
      }
      BuildResourceDescriptorHob (
        EFI_RESOURCE_SYSTEM_MEMORY,
        (
          EFI_RESOURCE_ATTRIBUTE_PRESENT |
          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
          //
          // RC Override: mark <4G available memory as tested to give DXE enough memory space, so that default
          // memory allocations won't occupy the bins for specific memory types.
          //
          ResourceAttributeTested |
          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE
          ),
        MemoryMap[Index].PhysicalAddress,
        RangeLength
        );

      //
      // Test this memory range
      //
      Status = BaseMemoryTest (
                 MrcData,
                 MemoryMap[Index].PhysicalAddress,
                 RangeLength,
                 MemoryTestLevel,
                 &BadMemoryAddress
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Memory test failure at %lXh.\n", BadMemoryAddress));
        REPORT_STATUS_CODE (EFI_ERROR_CODE, EFI_COMPUTING_UNIT_MEMORY | EFI_CU_EC_NON_SPECIFIC);
        ASSERT_EFI_ERROR (Status);
      }
    } else {
      ResourceAttribute = 0;
      if ((
            (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
            (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
            ) &&
          (MemoryMap[Index].PhysicalAddress != MC_ABSEG_HSEG_PHYSICAL_START)) {
        //
        // Only count and report TSEG.
        //
        IsTsegRegion = TRUE;
        SmramRanges++;
      } else if (MemoryMap[Index].Type == DualChannelDdrReservedWriteBackMemory) {
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                            ResourceAttributeTested |
                            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;
      }
      //
      // Make sure non-system memory is marked as reserved.
      //
      if (IsTsegRegion){
        //
        // Build an Tseg resource Hob for Fsp
        //
        BuildResourceDescriptorWithOwnerHob (
          EFI_RESOURCE_MEMORY_RESERVED,          // MemoryType,
          ResourceAttribute,                     // MemoryAttribute
          MemoryMap[Index].PhysicalAddress,      // MemoryBegin
          MemoryMap[Index].RangeLength,          // MemoryLength
          &gFspReservedMemoryResourceHobTsegGuid // OwnerGUID
        );
      } else {
        BuildResourceDescriptorHob (
          EFI_RESOURCE_MEMORY_RESERVED,     // MemoryType,
          ResourceAttribute,                // MemoryAttribute
          MemoryMap[Index].PhysicalAddress, // MemoryBegin
          MemoryMap[Index].RangeLength      // MemoryLength
        );
      }
    }
  }

  BufferSize = sizeof (EFI_SMRAM_HOB_DESCRIPTOR_BLOCK);
  if (SmramRanges > 0) {
    BufferSize += ((SmramRanges - 1) * sizeof (EFI_SMRAM_DESCRIPTOR));
  }

  Hob.Raw = BuildGuidHob (&gEfiSmmSmramMemoryGuid, BufferSize);
  if (Hob.Raw == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  SmramHobDescriptorBlock = (void *) (Hob.Raw);
  SmramHobDescriptorBlock->NumberOfSmmReservedRegions = SmramRanges;

  SmramIndex = 0;
  for (Index = 0; Index < NumRanges; Index++) {
    if ((
          (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) ||
          (MemoryMap[Index].Type == DualChannelDdrSmramNonCacheable)
          ) &&
        (MemoryMap[Index].PhysicalAddress != MC_ABSEG_HSEG_PHYSICAL_START)) {
      //
      // This is an SMRAM range (not reporting AB_SEG or H_SEG), create an SMRAM descriptor.
      //
      SmramDescriptor = &SmramHobDescriptorBlock->Descriptor[SmramIndex];
      SmramDescriptor->PhysicalStart = MemoryMap[Index].PhysicalAddress;
      SmramDescriptor->CpuStart      = MemoryMap[Index].CpuAddress;

      //
      // RangeLength includes alignment adjustment.
      //
      if (SmramDescriptor->PhysicalStart < 0x100000) {
        SmramDescriptor->PhysicalSize  = MemoryMap[Index].RangeLength;
      } else {
        SmramDescriptor->PhysicalSize  = Inputs->TsegSize * 0x100000;
      }
      DEBUG ((DEBUG_INFO, "TSeg base is %Xh\n", SmramDescriptor->PhysicalStart));
      DEBUG ((DEBUG_INFO, "TSeg SMRAM size is %Xh\n", SmramDescriptor->PhysicalSize));

      if (MemoryMap[Index].Type == DualChannelDdrSmramCacheable) {
        SmramDescriptor->RegionState = EFI_SMRAM_CLOSED | EFI_CACHEABLE;
      } else {
        SmramDescriptor->RegionState = EFI_SMRAM_CLOSED;
      }
      SmramIndex++;
    }
  }

  TseDataHob = (TSE_DATA_HOB *) GetFirstGuidHob (&gTseDataHobGuid);
  if ((TseDataHob != NULL) &&
    (IsTseSupported() && IsTseEnabled())) {

    TseDramSizeInBytes = (TseDataHob->TseDramMemSize) << 20;      // Value in MB
    TseDramMemBaseAddr = (TseDataHob->TseDramMemBase) << 20;      // Value in MB

    DEBUG ((DEBUG_INFO, "TSE DRAM memory base = 0x%lx Size = 0x%x\n", TseDramMemBaseAddr, TseDramSizeInBytes));

    ResourceType = EFI_RESOURCE_MEMORY_RESERVED;

    ResourceAttribute = \
      EFI_RESOURCE_ATTRIBUTE_PRESENT |
      ResourceAttributeTested |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE;

    BuildResourceDescriptorHob (
      ResourceType,                           // MemoryType,
      ResourceAttribute,                      // MemoryAttribute
      TseDramMemBaseAddr,                     // MemoryBegin
      TseDramSizeInBytes                      // MemoryLength
      );

    BuildMemoryAllocationHob (
      TseDramMemBaseAddr,                     // MemoryBegin
      TseDramSizeInBytes,                     // MemoryLength
      EfiReservedMemoryType
      );

    //
    // Program TSE IMR.
    //
    Status = SetImr (IMR_TSE, TseDramMemBaseAddr, TseDramSizeInBytes);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Fail to program TSE IMR!\n"));
    }
  }
  if (MemoryMapData->TotalPhysicalMemorySize <= 0x1000) {
    PeiTdxProgramImrBelow4Gb ();
  }
  //
  // Get the current "Top of Upper Usable Memory" address from TOUUD.
  // If TOUUD > 4G, it means memory is re-mapped.
  //
  Touud = GetHostBridgeRegisterData (HostBridgeTouud, HostBridgeTouudBase);

  RemapBase = GetRemapBase ();
  RemapLimit = GetRemapLimit ();
  Tom = GetHostBridgeRegisterData (HostBridgeTom, HostBridgeTomInMb);

  DEBUG ((DEBUG_INFO, "RemapBase 0x%lx\n, RemapLimit 0x%lx\n, Tom 0x%lx\n", RemapBase, RemapLimit, Tom));
  DEBUG((DEBUG_INFO, "[Touud=0x%llX]\n", Touud));

  if (Touud > MEM_EQU_4GB) {
    TopUseableMemAddr = MEM_EQU_4GB;
    Touud -= TopUseableMemAddr;
    NocImrExclusionLimit = MEM_EQU_4GB;

    DEBUG((DEBUG_INFO, "[TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // Memory Allocation @4GB
    //
    if(SiPreMemConfig != NULL) {
      if (SiPreMemConfig->StaticContentSizeAt4Gb != 0) {
        StaticContentSize = MultU64x32(SiPreMemConfig->StaticContentSizeAt4Gb, SIZE_1MB);
        DEBUG ((DEBUG_INFO, "StaticContent Base = 0x%lx Size = 0x%lx\n", TopUseableMemAddr, StaticContentSize));
        ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                          EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                          ResourceAttributeTested |
                          EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                          EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;
        BuildResourceDescriptorHob (
          ResourceType,                           // MemoryType,
          ResourceAttribute,                      // MemoryAttribute
          TopUseableMemAddr,                      // MemoryBegin
          StaticContentSize                       // MemoryLength
        );
        BuildMemoryAllocationHob (
          TopUseableMemAddr,
          StaticContentSize,
          EfiReservedMemoryType
          );
        TopUseableMemAddr += StaticContentSize;
        Touud -= StaticContentSize;
      }
    }
    DEBUG((DEBUG_INFO, "[Post Static Memory Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // Memory Allocation for Prmrr
    //
    PeiPrmrrMemoryAllocation(&TopUseableMemAddr, &Touud);
    DEBUG((DEBUG_INFO, "[Post Prmrr Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // Memory Allocation for Trace Hub
    //
    PeiTraceHubMemoryAllocation(&TopUseableMemAddr, &Touud);
    DEBUG((DEBUG_INFO, "[Post Trace Hub Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // Memory Allocation for Telemetry
    //
    StreamTracerMemSize = 0;
    StreamTracerBaseAddr = 0;
    PeiTelemetryMemoryAllocation(&TopUseableMemAddr, &Touud,
                                              ResourceAttributeTested,
                                              &StreamTracerMemSize,
                                              &StreamTracerBaseAddr);
    MemoryMapData->StreamTracerBase = (UINT32)StreamTracerBaseAddr;
    MemoryMapData->StreamTracerSize = StreamTracerMemSize;
    DEBUG((DEBUG_INFO, "[Post Telemetry Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    if (MemoryMapData->TotalPhysicalMemorySize > 0x1000) {
      //
      // Memory Allocation for Tdx
      //
      SeamRrBaseAddress = 0;
      PeiTdxMemoryAllocation(&TopUseableMemAddr, &Touud,
                                            ResourceAttributeTested,
                                            &SeamRrBaseAddress);
      MemoryMapData->SeamrrBase = SeamRrBaseAddress;
      DEBUG((DEBUG_INFO, "[Post Tdx Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));
    }

    NocImrExclusionLimit = TopUseableMemAddr;

    if (NocImrExclusionLimit != MEM_EQU_4GB) {
      NocImrExclusionSize = NocImrExclusionLimit - BASE_4GB;
      if (NocImrExclusionSize > GetPowerOfTwo64 (NocImrExclusionSize)) {
        NocImrExclusionSize = 2 * GetPowerOfTwo64 (NocImrExclusionSize);
      }

      NocImrExclusionDelta = NocImrExclusionSize - (NocImrExclusionLimit - BASE_4GB);
      DEBUG ((DEBUG_INFO, "NocImrExclusionLimit = 0x%llX NocImrExclusionDelta = 0x%llX NocImrExclusionSize = 0x%llX\n", NocImrExclusionLimit, NocImrExclusionDelta, NocImrExclusionSize));

      if (NocImrExclusionDelta != 0) {
        ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                            ResourceAttributeTested |
                            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;

        BuildResourceDescriptorHob (
                                    ResourceType,         // MemoryType,
                                    ResourceAttribute,    // MemoryAttribute
                                    NocImrExclusionLimit, // MemoryBegin
                                    NocImrExclusionDelta  // MemoryLength
                                    );

        BuildMemoryAllocationHob (
                                  NocImrExclusionLimit,
                                  NocImrExclusionDelta,
                                  EfiReservedMemoryType
                                  );

        TopUseableMemAddr += NocImrExclusionDelta;
        Touud             -= NocImrExclusionDelta;
        DEBUG ((DEBUG_INFO, "After Aligning the BASE 2 IMR TopUseableMemAddr = 0x%016lX Touud=0x%llX\n", TopUseableMemAddr, Touud));
      }
    }

    //
    // Memory Allocation for FlatCcs
    //
    IGpuMemoryAllocation(&TopUseableMemAddr, &Touud, ResourceAttributeTested);
    DEBUG((DEBUG_INFO, "[Post FlatCcs Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // Memory Allocation for psmi
    //
    PeiPsmiMemoryAllocation(&TopUseableMemAddr, &Touud, ResourceAttributeTested);
    DEBUG((DEBUG_INFO, "[Post Psmi Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // GSM2 allocation
    //
    IGpuGsm2Allocation(SiPreMemPolicy, &TopUseableMemAddr, &Touud, ResourceAttributeTested);
    DEBUG((DEBUG_INFO, "[Post GSM2 Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // VMD allocation
    //
    VmdMemoryAllocation(&TopUseableMemAddr, &Touud, ResourceAttributeTested);
    DEBUG((DEBUG_INFO, "[Post Vmd Allocation: TopUseableMemAddr=0x%llX Touud=0x%llX]\n", TopUseableMemAddr, Touud));

    //
    // This is above PSMI memory space, give it to EFI.
    //
    TopMemSize = Touud;

    DEBUG ((DEBUG_INFO, "Found 0x%016lX bytes at 0x%016lX\n", TopMemSize, TopUseableMemAddr));
    ResourceType      = EFI_RESOURCE_SYSTEM_MEMORY;
    ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_COMBINEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_THROUGH_CACHEABLE |
                        EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;
    BuildResourceDescriptorHob (
      ResourceType,                           // MemoryType,
      ResourceAttribute,                      // MemoryAttribute
      TopUseableMemAddr,                      // MemoryBegin
      TopMemSize                              // MemoryLength
      );
    TopUseableMemAddr += TopMemSize;
  } // end of above 4GB

  //
  // MMIO allocation
  //
  PeiIOMemoryAllocation ();
  ReserveMmioBelowPciConfig ();

  //
  // Create SMRAM ACPI Variable Hob
  //
  CreateS3AcpiVariableHob ();

  return EFI_SUCCESS;
}

/**
  Determine the memory size desired based on HOB memory information.

  @param[in, out] Size        - The memory size to return.
**/
void
RetrieveRequiredMemorySize (
  IN OUT    UINTN             *Size
  )
{
  EFI_STATUS                  Status;
  EFI_PEI_HOB_POINTERS        Hob;
  EFI_MEMORY_TYPE_INFORMATION *MemoryData;
  UINT32                      Index;
  UINTN                       TempPageNum;
  UINT32                      NoOfEntries;

  *Size      = 0;
  MemoryData = NULL;
  Status     = PeiServicesGetHobList ((VOID **) &Hob.Raw);
  ASSERT_EFI_ERROR (Status);

  while (!END_OF_HOB_LIST (Hob)) {
    if (Hob.Header->HobType == EFI_HOB_TYPE_GUID_EXTENSION &&
      CompareGuid (&Hob.Guid->Name, &gEfiMemoryTypeInformationGuid)) {
      MemoryData = (EFI_MEMORY_TYPE_INFORMATION *) (Hob.Raw + sizeof (EFI_HOB_GENERIC_HEADER) + sizeof (EFI_GUID));
      break;
    }

    Hob.Raw = GET_NEXT_HOB (Hob);
  }
  //
  // Platform PEIM should supply the information. Generic PEIM doesn't assume any default value.
  //
  if (MemoryData == NULL) {
    return;
  }

  TempPageNum = 0;
  NoOfEntries = GET_GUID_HOB_DATA_SIZE (Hob.Raw) / sizeof (EFI_MEMORY_TYPE_INFORMATION);
  for (Index = 0; Index < NoOfEntries; Index++) {
    //
    // Accumulate default memory size requirements
    //
    TempPageNum += MemoryData[Index].NumberOfPages;
  }

  if (TempPageNum == 0) {
    return;
  }
  //
  // 16 additional pages are used by DXE memory manager.
  //
  *Size = (TempPageNum + 16) * EFI_PAGE_SIZE;

  return;
}

/**
  This function returns the memory ranges to be enabled, along with information
  describing how the range should be used. The MemoryMap buffer will be filled in and
  NumRanges will contain the actual number of memory ranges that are to be enabled.

  @param[in]      MrcData     - Mrc data structure
  @param[in, out] MemoryMap   - Buffer to record details of the memory ranges to be enabled.
  @param[in, out] NumRanges   - On input, this contains the maximum number of memory ranges that
                                can be described in the MemoryMap buffer.

  @retval EFI_SUCCESS          - The function completed successfully.
  @retval EFI_BUFFER_TOO_SMALL - The specified number of ranges is too large.
**/
EFI_STATUS
GetMemoryMap (
  IN MrcParameters  *CONST                        MrcData,
  IN OUT   PEI_DUAL_CHANNEL_DDR_MEMORY_MAP_RANGE  *MemoryMap,
  IN OUT   UINT8                                  *NumRanges
  )
{
  const MrcInput               *Inputs;
  MrcOutput                    *Outputs;
  MrcMemoryMap                 *MemoryMapData;
  EFI_STATUS                   Status;
  UINT32                       SmramMask;
  UINT8                        Index;
  SI_PREMEM_POLICY             *SiPreMemPolicy;
  MEMORY_CONFIGURATION         *MemConfig;
  IGPU_PEI_PREMEM_CONFIG       *IGpuPreMemConfig;
  UINT32                       GraphicsStolenSizeInMb;
  UINT32                       GraphicsGttSizeInMb;
  UINT8                        MemoryHoleIndex;
  UINT32                       SeamrrSize;
  MRC_EXT_INPUTS_TYPE          *ExtInputs;
  UINT32                       TsegFailedSize;
  UINT32                       TsegFailedBase;
  MEMORY_CONFIG_NO_CRC         *MemConfigNoCrc;

  if ((*NumRanges) < MAX_RANGES) {
    return EFI_BUFFER_TOO_SMALL;
  }

  *NumRanges  = 0;
  GraphicsStolenSizeInMb = 0;
  GraphicsGttSizeInMb = 0;

  Inputs    = &MrcData->Inputs;
  Outputs   = &MrcData->Outputs;
  MemoryMapData = &Outputs->MemoryMapData;
  ExtInputs = Inputs->ExtInputs.Ptr;

  //
  // Get platform memory range service
  //
  SmramMask = 0;
  SiPreMemPolicy = NULL;
  IGpuPreMemConfig = NULL;
  MemConfigNoCrc = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  ASSERT_EFI_ERROR (Status);

  if (SiPreMemPolicy != NULL) {
    MemConfig = NULL;
    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigGuid, (VOID *) &MemConfig);
    ASSERT_EFI_ERROR (Status);

    Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
    ASSERT_EFI_ERROR (Status);

    if (MemConfig != NULL) {
      //
      // Find out which memory ranges to reserve on this platform
      //
      SmramMask = ExtInputs->SmramMask;
    }

    if (IGpuIsSupported ()) {
      Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
      ASSERT_EFI_ERROR (Status);

      if (IGpuPreMemConfig != NULL) {
        GraphicsStolenSizeInMb = IGpuGetDsmSizeInBytes ();
        GraphicsGttSizeInMb  = IGpuGetGsmSizeInBytes ();
      }
    }
  }

  //
  // Generate memory ranges for the memory map.
  //
  Index           = 0;

  //
  // Get the current "max usable memory" address from TOLUD because we will not
  // support any memory above 4Gig.  Will ignore the memory between 4G and TOUUD.
  //

  //
  // System is very unlikely to work with less than 32MB
  //
  ASSERT ((MemoryMapData->ToludBase << 20) >= (32 * 1024 * 1024));

  //
  // Add memory below 640KB to the memory map. Make sure memory between
  // 640KB and 1MB are reserved, even if not used for SMRAM
  //
  MemoryMap[*NumRanges].RowNumber       = Index;
  MemoryMap[*NumRanges].PhysicalAddress = ADDRESS_SPACE_START;
  MemoryMap[*NumRanges].CpuAddress      = ADDRESS_SPACE_START;
  MemoryMap[*NumRanges].RangeLength     = 0xA0000;
  MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
  (*NumRanges)++;

  //
  // Reserve ABSEG or HSEG SMRAM if needed
  //
  if (SmramMask & (PEI_MR_SMRAM_ABSEG_MASK | PEI_MR_SMRAM_HSEG_MASK)) {
    MemoryMap[*NumRanges].PhysicalAddress = MC_ABSEG_HSEG_PHYSICAL_START;
    MemoryMap[*NumRanges].RangeLength     = MC_ABSEG_HSEG_LENGTH;
    MemoryMap[*NumRanges].CpuAddress      = (SmramMask & PEI_MR_SMRAM_ABSEG_MASK) ?
      MC_ABSEG_CPU_START :
      MC_HSEG_CPU_START;
    //
    // Chipset only supports cacheable SMRAM.
    //
    MemoryMap[*NumRanges].Type = DualChannelDdrSmramNonCacheable;
  } else {
    //
    // Just mark this range reserved.
    //
    MemoryMap[*NumRanges].PhysicalAddress = MC_ABSEG_HSEG_PHYSICAL_START;
    MemoryMap[*NumRanges].CpuAddress      = MC_ABSEG_CPU_START;
    MemoryMap[*NumRanges].RangeLength     = 0x60000;
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
  }
  MemoryMap[*NumRanges].RowNumber = Index;
  (*NumRanges)++;

  //
  // Add remaining memory to the memory map based on new LowestBase
  //
  MemoryMap[*NumRanges].RowNumber       = Index;
  MemoryMap[*NumRanges].PhysicalAddress = LOWER_MEMORY_START;
  MemoryMap[*NumRanges].CpuAddress      = LOWER_MEMORY_START;
  MemoryMap[*NumRanges].RangeLength     = MemoryMapData->LowestBase - LOWER_MEMORY_START;
  MemoryMap[*NumRanges].Type            = DualChannelDdrMainMemory;
  (*NumRanges)++;


  //
  // IMR
  //
  if (MemoryMapData->MeStolenSize > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = (MemoryMapData->MeStolenBase << 20);
    MemoryMap[*NumRanges].CpuAddress      = (MemoryMapData->MeStolenBase << 20);
    MemoryMap[*NumRanges].RangeLength     = (MemoryMapData->MeStolenSize << 20);
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedWriteBackMemory;
    (*NumRanges)++;
  }

  //
  // PRMRR
  //
  if (Inputs->PrmrrSize > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = LShiftU64 (MemoryMapData->PrmrrBase, 20);
    MemoryMap[*NumRanges].CpuAddress      = LShiftU64 (MemoryMapData->PrmrrBase, 20);
    MemoryMap[*NumRanges].RangeLength     = (Inputs->PrmrrSize << 20);
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedWriteBackMemory;
    (*NumRanges)++;
  }

  //
  // DPR
  //
  if (Inputs->DprSize > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = ((MemoryMapData->TsegBase - Inputs->DprSize) << 20);
    MemoryMap[*NumRanges].CpuAddress      = ((MemoryMapData->TsegBase - Inputs->DprSize) << 20);
    MemoryMap[*NumRanges].RangeLength     = (Inputs->DprSize << 20);
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;

    UpdateDprHobInfo (MemoryMap[*NumRanges].PhysicalAddress, (UINT8)Inputs->DprSize);

    (*NumRanges)++;
  }

  //
  // TSEG
  //
  if (Inputs->TsegSize > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = (MemoryMapData->TsegBase << 20);
    MemoryMap[*NumRanges].CpuAddress      = (MemoryMapData->TsegBase << 20);
    MemoryMap[*NumRanges].RangeLength     = (Inputs->TsegSize << 20);
    MemoryMap[*NumRanges].Type            = DualChannelDdrSmramCacheable;
    (*NumRanges)++;
  }

  //
  // BGSM (GFX GTT)
  //
  if (GraphicsGttSizeInMb > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = (MemoryMapData->GttBase << 20);
    MemoryMap[*NumRanges].CpuAddress      = (MemoryMapData->GttBase << 20);
    MemoryMap[*NumRanges].RangeLength     = (GraphicsGttSizeInMb);
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
    (*NumRanges)++;
  }

  //
  // BDSM (GFX UMA)
  //
  if (GraphicsStolenSizeInMb > 0) {
    MemoryMap[*NumRanges].RowNumber       = Index;
    MemoryMap[*NumRanges].PhysicalAddress = (MemoryMapData->BdsmBase << 20);
    MemoryMap[*NumRanges].CpuAddress      = (MemoryMapData->BdsmBase << 20);
    MemoryMap[*NumRanges].RangeLength     = (GraphicsStolenSizeInMb);
    MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
    (*NumRanges)++;
  }

  //
  // SEAMRR
  //
  if (IsTdxEnabled ()) {
    Status = GetSeamrrSize (&SeamrrSize);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Cannot execute GetSeamrrSize() properly %r\n", Status));
    }
    if (SeamrrSize > 0) {
      MemoryMap[*NumRanges].RowNumber       = Index;
      MemoryMap[*NumRanges].PhysicalAddress = LShiftU64 (MemoryMapData->SeamrrBase, 20);
      MemoryMap[*NumRanges].CpuAddress      = LShiftU64 (MemoryMapData->SeamrrBase, 20);
      MemoryMap[*NumRanges].RangeLength     = (SeamrrSize << 20);
      MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
      (*NumRanges)++;
    }
  }

  //
  // TXT space (Private / Public / TPM / DbC TraceHub / other)
  //
  MemoryMap[*NumRanges].RowNumber       = Index;
  MemoryMap[*NumRanges].PhysicalAddress = TXT_PRIVATE_BASE;
  MemoryMap[*NumRanges].CpuAddress      = TXT_PRIVATE_BASE;
  MemoryMap[*NumRanges].RangeLength     = TXT_CONFIG_SPACE_LENGTH;
  MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
  (*NumRanges)++;

  //
  // Record the memory range Information of Failed TSEG Region
  // if TSEG New Region Reallocated and  default TSEG Region failed
  //
  TsegFailedSize = 0;
  TsegFailedBase = 0;
  if ((Inputs->TsegSize > 0) && (MemConfigNoCrc != NULL)) {
    if ((MemConfigNoCrc->SafeLoadingBiosEnableState == 1) || (MemConfigNoCrc->TsegMemoryTestStatus == mrcSuccess)) {
      if ((MemConfigNoCrc->RetryCount >= 1) && (MemConfigNoCrc->RetryCount <= 3) && (MrcData->Outputs.MrcPprStatus!= mrcSuccess)) {
        TsegFailedBase = MemoryMapData->TsegBase + Inputs->TsegSize;
        TsegFailedSize = Inputs->TsegSize * MemConfigNoCrc->RetryCount;
        DEBUG ((DEBUG_INFO, "Reserving Tseg Failed BaseAddress = 0x%x Size = 0x%x\n", TsegFailedBase, TsegFailedSize));
        MemoryMap[*NumRanges].RowNumber       = Index;
        MemoryMap[*NumRanges].PhysicalAddress = (TsegFailedBase << 20);
        MemoryMap[*NumRanges].CpuAddress      = (TsegFailedBase << 20);
        MemoryMap[*NumRanges].RangeLength     = (TsegFailedSize << 20);
        MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
        (*NumRanges)++;
      }
    }
  }

  for (MemoryHoleIndex = 0; MemoryHoleIndex < MEMORY_MAP_MAX_HOLE; MemoryHoleIndex++) {
    DEBUG ((DEBUG_INFO, "Memory Map Hole Check Base = %X Size = %X\n", MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress, MemoryMapData->MemoryHoles[MemoryHoleIndex].Length));
    if ((MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress != MemoryMapData->MemoryHoles[MemoryHoleIndex].Length) && (MemoryMapData->MemoryHoles[MemoryHoleIndex].Length != 0) && (MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress != 0)) {
      DEBUG ((DEBUG_INFO, "Reserving the Memory Map hole BaseAddress = 0x%x Limit = 0x%x\n", (MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress << 20), (MemoryMapData->MemoryHoles[MemoryHoleIndex].Length << 20)));
      MemoryMap[*NumRanges].RowNumber       = Index;
      MemoryMap[*NumRanges].PhysicalAddress = (MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress << 20);
      MemoryMap[*NumRanges].CpuAddress      = (MemoryMapData->MemoryHoles[MemoryHoleIndex].BaseAddress << 20);
      MemoryMap[*NumRanges].RangeLength     = (MemoryMapData->MemoryHoles[MemoryHoleIndex].Length << 20);
      MemoryMap[*NumRanges].Type            = DualChannelDdrReservedMemory;
      (*NumRanges)++;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function returns a pointer to the allocated hand off buffer.

  @param[in, out] Hob         - A pointer to where to store the pointer to the allocated data buffer.
  @param[in]      BlockSize   - The size of the buffer to get.
  @param[in]      Guid        - The GUID to assign to the HOB.

  @retval mrcSuccess  - Hob is successfully built.
  @retval Others      - Error occurred while creating the Hob.
**/
MrcStatus
MrcGetHobForDataStorage (
  IN OUT VOID      **Hob,
  IN     UINT16    BlockSize,
  IN     EFI_GUID  *Guid
  )
{
  EFI_HOB_GUID_TYPE *Buffer;
  EFI_STATUS        Status;

  Status = PeiServicesCreateHob (EFI_HOB_TYPE_GUID_EXTENSION, BlockSize, Hob);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return mrcFail;
  }

  Buffer = (EFI_HOB_GUID_TYPE *) *Hob;
  CopyMem (&(Buffer->Name), Guid, sizeof (EFI_GUID));
  ZeroMem (&Buffer[1], BlockSize - sizeof (EFI_HOB_GUID_TYPE));

  return mrcSuccess;
}

/**
  Finds the BDAT Schema List HOB if it exists.  If it does not exist, creates it

  @param[out] BdatSchemaListHob - Pointer to Schema List Hob, by reference

  @retval mrcSuccess  - Hob was successfully found or built.
  @retval Others      - Error occurred while creating or finding the Hob.
**/
MrcStatus
MrcGetBdatSchemaListHob (
  OUT  MRC_BDAT_SCHEMA_LIST_HOB  **BdatSchemaListHob
  )
{
  CONST EFI_GUID           SchemaListGuid = gMrcSchemaListHobGuid;
  MrcStatus                Status;
  MRC_BDAT_SCHEMA_LIST_HOB *Buffer;

  Status = mrcSuccess;
  Buffer = (MRC_BDAT_SCHEMA_LIST_HOB *) GetFirstGuidHob (&SchemaListGuid);

  if (Buffer != NULL) {
    DEBUG ((DEBUG_INFO, "BDAT Schema List HOB already exists\n"));
    *BdatSchemaListHob = Buffer;
  } else {
    DEBUG ((DEBUG_INFO, "Creating BDAT Schema List HOB: "));
    Status = MrcGetHobForDataStorage (
               (VOID **) &Buffer,
               sizeof (MRC_BDAT_SCHEMA_LIST_HOB),
               (EFI_GUID*)&SchemaListGuid
               );
    if (Status != mrcSuccess) {
      DEBUG ((DEBUG_INFO, "%r\n", Status));
      return mrcFail;
    }

    *BdatSchemaListHob = Buffer;
  }

  return Status;
}

/**
  This function returns a pointer to the allocated hand off buffer for the BDAT GUID.

  @param[in out] BdatMemoryHob   - Pointer to Schema List Hob, by reference
  @param[in]     BdatSchemaSize  - Size of the Bdat Schema
  @param[in]     ResultsGuid     - Pointer to GUID associated with the BDAT Memory HOB

  @retval mrcSuccess  - Hob was successfully found or built.
  @retval Others      - Error occurred while creating or finding the Hob.
**/
MrcStatus
MrcGetBdatMemoryHob (
  IN OUT VOID      **BdatMemoryHob,
  IN     UINT32    BdatSchemaSize,
  IN     EFI_GUID  *ResultsGuid
  )
{
  MrcStatus  Status;
  UINT16     BdatHobSize;

  BdatHobSize = (UINT16) (BdatSchemaSize + sizeof (MRC_BDAT_SCHEMA_HEADER_STRUCTURE) + sizeof (EFI_HOB_GUID_TYPE));

  Status = MrcGetHobForDataStorage (
            BdatMemoryHob,
            BdatHobSize,
            ResultsGuid
            );

  return Status;
}

/**
  This function updates the schema IDs with the Bdat GUID value.

  @param[in out] BdatSchemaHobGuidPtr  - Pointer to Schema List in a Hob, by reference
  @param[in]     BdatSchemasIdPtr      - Pointer to BdatMemoryHob Schema Id in a Hob, by reference
  @param[in]     ResultsGuid           - Pointer to GUID associated with the BDAT Memory HOB

  @retval None
**/
VOID
MrcUpdateBdatHobIds (
  IN OUT  MrcParameters *const MrcData,
  IN OUT  VOID                 *BdatSchemaHobGuidPtr,
  IN OUT  VOID                 *BdatSchemasIdPtr,
  IN EFI_GUID                  *ResultsGuid
  )
{
  const MrcInput       *Inputs;
  const MRC_FUNCTION   *MrcCall;

  Inputs         = &MrcData->Inputs;
  MrcCall        = Inputs->Call.Func;

  // Housekeeping for the list of schema IDs
  MrcCall->MrcCopyMem ((UINT8 *) BdatSchemaHobGuidPtr, (UINT8 *) ResultsGuid, sizeof (EFI_GUID));
  MrcCall->MrcCopyMem ((UINT8 *) BdatSchemasIdPtr, (UINT8 *) ResultsGuid, sizeof (EFI_GUID));
}

/**
  Determines whether or not the platform has executed a TXT launch by
  examining the TPM Establishment bit.
  @param[in] VOID
  @retval TRUE        - If the TPM establishment bit is asserted.
  @retval FALSE       - If the TPM establishment bit is unasserted.
**/
BOOLEAN
IsEstablishmentBitAsserted (
  VOID
  )
{
  UINT8             Access;
  UINT16            TimeOutCount;

  //
  // Set TPM.ACCESS polling timeout about 750ms.
  //
  TimeOutCount = TPM_TIME_OUT;
  do {
    //
    // Read TPM status register
    //
    Access = MmioRead8 (TPM_STATUS_REG_ADDRESS);

    //
    // if TPM.Access == 0xFF, TPM is not present.
    //
    if (Access == 0xFF) {
      return FALSE;
    }
    //
    // Check tpmRegValidSts bit before checking establishment bit.
    //
    if ((Access & 0x80) == 0x80) {
      //
      // tpmRegValidSts set, we can check establishment bit now.
      //
      break;
    } else {
      //
      // Delay 1ms
      //
      MicroSecondDelay (1000);
    }

    TimeOutCount--;
  } while (TimeOutCount != 0);

  //
  // ValidSts is not set.
  //
  if ((Access & 0x80) != 0x80) {
    return FALSE;
  }
  //
  // The bit we're interested in uses negative logic:
  // If bit 0 == 1 then return False,
  // Else return True.
  //
  return (BOOLEAN) ((Access & 0x1) ? FALSE : TRUE);
}

/**
  Unlock memory when security is set and TxT is not enabled.
  @param[in] MrcData     - Mrc global data.
**/
void
UnlockMemory (
  IN CONST MrcParameters    *CONST  MrcData
  )
{
  CPUID_VERSION_INFO_ECX  CpuidVersionInfoEcx;
  UINT32                  Data32;
  const MrcInput          *Inputs;
  MRC_FUNCTION            *MrcCall;
  MSR_LTCTRLSTS_REGISTER  MsrLtctrlsts;

  Inputs = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  Data32 = 0;

  AsmCpuid (
      CPUID_VERSION_INFO,
      NULL,
      NULL,
      &CpuidVersionInfoEcx.Uint32,
      NULL
      );
  if ((CpuidVersionInfoEcx.Uint32 & BIT6)) {
    DEBUG ((DEBUG_INFO, "Processor supports TXT\n"));

    Data32 = CheckSmxCapabilities();

    if (Data32 & BIT0) {
      DEBUG ((DEBUG_INFO, "Platform / PCH supports TXT\n"));
      MsrLtctrlsts.Uint64 = MrcCall->MrcReadMsr64 (MSR_LTCTRLSTS);
      if (MsrLtctrlsts.Bits.Memlockcpu == 0 && MsrLtctrlsts.Bits.Memlockdev == 0) {
        return;
      }

      if (!(IsEstablishmentBitAsserted ())) {
        DEBUG ((DEBUG_INFO, "Unlock memory\n"));
        MrcCall->MrcWriteMsr64 (MSR_LT_UNLOCK_MEMORY, 0);
      }
    } else {
      DEBUG ((DEBUG_INFO, "Platform / PCH does not support TxT\n"));
    }
  } else {
    DEBUG ((DEBUG_INFO, "Processor does not support TxT\n"));
  }
}

/**
  Determine the current valid state of PmaCce.

  @param[in] CpuMemoryInitConfig   - The Cpu Memory Init config.
  @param[in] CurrTmeEnable         - TmeEnable state.

  @retval PmaCce status.
**/
MRC_CCE_CONFIG
GetPmaCceConfig (
  IN CPU_MEMORY_INIT_CONFIG* CpuMemoryInitConfig,
  IN UINT32                  CurrTmeEnable
  )
{
  MRC_CCE_CONFIG  CurrPmaCceConfig;
  if (CurrTmeEnable) {
    // Check which controller is populated and program CCE accordingly
    CurrPmaCceConfig = MrcCceEnablePerMc; // This case handles setting either CCE0 or CCE1 or CCE0+CCE1
  } else if (CpuMemoryInitConfig->PrmrrSize > 0 && (IsKeyLockerSupported () || CpuMemoryInitConfig->TdxEnable)) {
    CurrPmaCceConfig = MrcCceEnSingle;
  } else {
    // Disable CCE
    CurrPmaCceConfig = MrcCceDisable;
  }
  return CurrPmaCceConfig;
}

/**
  Determine whether a cold reset of the platform is required.
  Note that the memory configuration saved data must be valid.

  @param[in] MrcData                 - The MRC "global data" area.
  @param[in] MemConfig               - MEMORY_CONFIGURATION structure.
  @param[in] CpuMemoryInitConfig     - The Cpu Memory Init config.
  @param[in] MrcBootMode             - Current MRC boot mode.
  @param[out] IsEfiResetColdRequired - TRUE if a power cycle is required.

  @retval TRUE if cold reset is required, otherwise returns FALSE.
**/
BOOLEAN
ColdBootRequired (
  IN MrcParameters                *CONST  MrcData,
  IN MEMORY_CONFIGURATION         *MemConfig,
  IN CPU_MEMORY_INIT_CONFIG       *CpuMemoryInitConfig,
  IN MRC_BOOT_MODE                MrcBootMode,
  OUT BOOLEAN                     *IsEfiResetColdRequired
  )
{
  MrcInput             *Inputs;
  MrcSaveData          *SaveData;
  MrcVersion           Version;
  UINT32               CurrPmaCceConfig;
  UINT32               CurrTmeEnable;
  BOOLEAN              RetVal;
  BOOLEAN              IsPowerCycleNeeded;

  Inputs    = &MrcData->Inputs;
  SaveData  = &MrcData->Save.Data;
  RetVal = FALSE;

  MrcVersionGet (MrcData, &Version);
  if ((Version.Major != SaveData->Version.Major) ||
      (Version.Minor != SaveData->Version.Minor) ||
      (Version.Rev   != SaveData->Version.Rev)   ||
      (Version.Build != SaveData->Version.Build)) {
    DEBUG ((
      DEBUG_INFO,
      "MRC change detected, prev. ver. %d.%d.%d.%d, curr. ver. %d.%d.%d.%d\n",
      SaveData->Version.Major,
      SaveData->Version.Minor,
      SaveData->Version.Rev,
      SaveData->Version.Build,
      Version.Major,
      Version.Minor,
      Version.Rev,
      Version.Build
      ));
    RetVal = TRUE;
  }
  if (Inputs->DdrIoIpVersion.Data != SaveData->DdrIoIpVersion.Data) {
    DEBUG ((
      DEBUG_INFO,
      "CPU change detected, DDRIO IP Version prev. %x.%x.%x.%x, curr. %x.%x.%x.%x\n",
      SaveData->DdrIoIpVersion.Bits.Version,
      SaveData->DdrIoIpVersion.Bits.Derivative,
      SaveData->DdrIoIpVersion.Bits.Segment,
      SaveData->DdrIoIpVersion.Bits.Stepping,
      Inputs->DdrIoIpVersion.Bits.Version,
      Inputs->DdrIoIpVersion.Bits.Derivative,
      Inputs->DdrIoIpVersion.Bits.Segment,
      Inputs->DdrIoIpVersion.Bits.Stepping
      ));
    RetVal = TRUE;
  }
  if (MrcBootMode != bmS3) {
    if (MrcData->Inputs.SaMemCfgCrc != SaveData->SaMemCfgCrc) {
      DEBUG ((
        DEBUG_INFO,
        "System Agent input parameter change detected, prev. CRC %xh, curr. CRC %xh.\n",
        SaveData->SaMemCfgCrc,
        MrcData->Inputs.SaMemCfgCrc
        ));
      RetVal = TRUE;
    } // CRC mismatch
  } // MrcBootMode != bmS3

  // If IBECC / CCE / TME policy is changed on a non-Cold reset, we need to invoke a power cycle
  IsPowerCycleNeeded = FALSE;

  if (MemConfig->ExternalInputs.Ibecc != SaveData->Ibecc) {
    DEBUG ((DEBUG_INFO, "%a policy has changed: old: %u, new: %u\n", "Ibecc", SaveData->Ibecc, MemConfig->ExternalInputs.Ibecc));
    RetVal = TRUE;
    if (MrcBootMode != bmCold) {
      IsPowerCycleNeeded = TRUE;
    }
  }

  if (CpuMemoryInitConfig != NULL) {
    CurrTmeEnable = (CpuMemoryInitConfig->TmeEnable == 1);
    if (CurrTmeEnable != SaveData->TmeEnable) {
      DEBUG ((DEBUG_INFO, "%a policy has changed: old: %u, new: %u\n", "TmeEnable", SaveData->TmeEnable, CurrTmeEnable));
      RetVal = TRUE;
      if (MrcBootMode != bmCold) {
        IsPowerCycleNeeded = TRUE;
      }
    }

    CurrPmaCceConfig = GetPmaCceConfig (CpuMemoryInitConfig, CurrTmeEnable);
    if (CurrPmaCceConfig != SaveData->PmaCceConfig) {
      DEBUG ((DEBUG_INFO, "%a policy has changed: old: %u, new: %u\n", "PmaCceConfig", SaveData->PmaCceConfig, CurrPmaCceConfig));
      RetVal = TRUE;
      if (MrcBootMode != bmCold) {
        IsPowerCycleNeeded = TRUE;
      }
    }
  }

  if (IsEfiResetColdRequired != NULL) {
    *IsEfiResetColdRequired = IsPowerCycleNeeded;
    DEBUG ((DEBUG_INFO, "EfiResetCold is%a required\n", (IsPowerCycleNeeded) ? "": " not"));
  }
  return RetVal;
}

/**
  Set up the MRC OEM data structure.

  @param[in, out] MrcData             - Pointer to the MRC global data structure.
  @param[in]      MemConfig           - MEMORY_CONFIGURATION structure.
**/
void
MrcSetupOem (
  IN OUT MrcParameters          *CONST MrcData,
  IN    MEMORY_CONFIGURATION    *MemConfig
  )
{
  MrcInput      *Inputs;
  MRC_FUNCTION  *MrcCall;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;

  // Populate MRC_FUNCTION pointers
  MrcCall->MrcIoRead8              = (MRC_IO_READ_8) &IoRead8;
  MrcCall->MrcIoRead16             = (MRC_IO_READ_16) &IoRead16;
  MrcCall->MrcIoRead32             = (MRC_IO_READ_32) &IoRead32;
  MrcCall->MrcIoWrite8             = (MRC_IO_WRITE_8) &IoWrite8;
  MrcCall->MrcIoWrite16            = (MRC_IO_WRITE_16) &IoWrite16;
  MrcCall->MrcIoWrite32            = (MRC_IO_WRITE_32) &IoWrite32;
  MrcCall->MrcMmioRead8            = (MRC_MMIO_READ_8) &MmioRead8;
  MrcCall->MrcMmioRead16           = (MRC_MMIO_READ_16) &MmioRead16;
  MrcCall->MrcMmioRead32           = (MRC_MMIO_READ_32) &MmioRead32;
  MrcCall->MrcMmioRead64           = (MRC_MMIO_READ_64) &SaMmioRead64;
  MrcCall->MrcMmioWrite8           = (MRC_MMIO_WRITE_8) &MmioWrite8;
  MrcCall->MrcMmioWrite16          = (MRC_MMIO_WRITE_16) &MmioWrite16;
  MrcCall->MrcMmioWrite32          = (MRC_MMIO_WRITE_32) &MmioWrite32;
  MrcCall->MrcMmioWrite64          = (MRC_MMIO_WRITE_64) &SaMmioWrite64;
  MrcCall->MrcSmbusRead8           = (MRC_SMBUS_READ_8) &SmBusReadDataByte;
  MrcCall->MrcSmbusRead16          = (MRC_SMBUS_READ_16) &SmBusReadDataWord;
  MrcCall->MrcSmbusWrite8          = (MRC_SMBUS_WRITE_8) &SmBusWriteDataByte;
  MrcCall->MrcSmbusWrite16         = (MRC_SMBUS_WRITE_16) &SmBusWriteDataWord;
  MrcCall->MrcGetPciDeviceAddress  = (MRC_GET_PCI_DEVICE_ADDRESS) &GetPciDeviceAddress;
  MrcCall->MrcGetPcieDeviceAddress = (MRC_GET_PCIE_DEVICE_ADDRESS) &GetPcieDeviceAddress;
  MrcCall->MrcGetRtcTime           = (MRC_GET_RTC_TIME) &GetRtcTime;
  MrcCall->MrcGetCpuTime           = (MRC_GET_CPU_TIME) &GetCpuTime;
  MrcCall->MrcCopyMem              = (MRC_MEMORY_COPY) &CopyMem;
  MrcCall->MrcSetMem               = (MRC_MEMORY_SET_BYTE) &SetMem;
  MrcCall->MrcSetMemWord           = (MRC_MEMORY_SET_WORD) &SetMemWord;
  MrcCall->MrcSetMemDword          = (MRC_MEMORY_SET_DWORD) &SetMemDword;
  MrcCall->MrcLeftShift64          = (MRC_LEFT_SHIFT_64) &LShiftU64;
  MrcCall->MrcRightShift64         = (MRC_RIGHT_SHIFT_64) &RShiftU64;
  MrcCall->MrcMultU64x32           = (MRC_MULT_U64_U32) &MultU64x32;
  MrcCall->MrcDivU64x64            = (MRC_DIV_U64_U64) &DivU64x64Remainder;
  MrcCall->MrcGetSpdData           = (MRC_GET_SPD_DATA) &GetSpdData;
  MrcCall->MrcGetRandomNumber      = (MRC_GET_RANDOM_NUMBER) &GetRandomNumber32;
  MrcCall->MrcCpuMailboxRead       = (MRC_CPU_MAILBOX_READ) &MailboxRead;
  MrcCall->MrcCpuMailboxWrite      = (MRC_CPU_MAILBOX_WRITE) &MailboxWrite;
  MrcCall->MrcCheckpoint           = (MRC_CHECKPOINT) &CheckPoint;
  MrcCall->MrcDebugHook            = (MRC_DEBUG_HOOK) &DebugHook;
  MrcCall->MrcPrintString          = (MRC_PRINT_STRING) &SaDebugPrint;
  MrcCall->MrcRtcCmos              = (MRC_GET_RTC_CMOS) &PeiRtcRead;
  MrcCall->MrcReadMsr64            = (MRC_MSR_READ_64) &AsmReadMsr64;
  MrcCall->MrcWriteMsr64           = (MRC_MSR_WRITE_64) &AsmWriteMsr64;
  MrcCall->MrcReturnFromSmc        = (MRC_RETURN_FROM_SMC) &ReturnFromSmc;
  MrcCall->MrcDelayNs              = (MRC_DELAY_NS) &DelayNs;
  MrcCall->MrcSetLockPrmrr         = (MRC_SET_LOCK_PRMRR) &PeiCpuSetPrmrrRegion;

  return;
}

/**
  Detects and returns the SoC SKU type based on the PCH configuration.
  Returns MrcSkuTypeUnspecified if the configuration is not recognized.

  @param None

  @return The detected platform SKU type.

**/
MrcSkuType
MrcGetSkuType (
  VOID
  )
{
  MrcSkuType SkuType;
  UINT16     CpuDid;

  CpuDid = (UINT16) GetHostBridgeRegisterData (HostBridgeDeviceId, HostBridgeDeviceIdData);

  switch (CpuDid) {
      case PTL_U_SA_DEVICE_ID_4C:
      case PTL_U_SA_DEVICE_ID_2C:
      case PTL_U_NEX_SA_DEVICE_ID_4C:
      case PTL_U_NEX_SA_DEVICE_ID_2C:
        SkuType = MrcSkuTypeU;
        break;

      case PTL_H_12XE_SA_DEVICE_ID_4C_8A:
      case PTL_H_12XE_SA_DEVICE_ID_4C_4A:
      case PTL_H_12XE_SA_DEVICE_ID_2C_8A:
      case PTL_H_12XE_SA_DEVICE_ID_2C_4A:
        SkuType = MrcSkuTypeH12Xe;
        break;

      case PTL_H_4XE_SA_DEVICE_ID_4C_8A:
      case PTL_H_4XE_SA_DEVICE_ID_4C_4A:
      case PTL_H_4XE_SA_DEVICE_ID_2C_8A:
      case PTL_H_4XE_SA_DEVICE_ID_2C_4A:
      case PTL_H_4XE_SA_DEVICE_ID_4C:
        SkuType = MrcSkuTypeH4Xe;
        break;

      default:
        SkuType = MrcSkuTypeUnspecified;
        break;
  }

  DEBUG ((DEBUG_INFO, "MrcGetSkuType 0x%x\n", SkuType));

  return SkuType;
}

/**
  Set up the MRC input data structure.

  @param[in]  SysBootMode         - Boot mode of the system.
  @param[in]  BootMode            - Boot mode of the MRC.
  @param[in]  MrcData             - Pointer to the MRC global data structure
  @param[in]  MemConfig           - MEMORY_CONFIGURATION structure.
  @param[in]  MemConfigNoCrc      - MEMORY_CONFIG_NO_CRC structure.
  @param[in]  SiPreMemPolicy      - The SI Pre-Mem Policy instance.

  @retval  Updated MRC_BOOT_MODE
**/
MRC_BOOT_MODE
MrcSetupMrcData (
  IN CONST EFI_BOOT_MODE              SysBootMode,
  IN CONST MRC_BOOT_MODE              BootMode,
  OUT      MrcParameters              *CONST MrcData,
  IN       MEMORY_CONFIGURATION       *CONST MemConfig,
  IN       MEMORY_CONFIG_NO_CRC       *CONST MemConfigNoCrc,
  IN       SI_PREMEM_POLICY           *CONST SiPreMemPolicy,
  IN       BOOLEAN                    DidPreviousTrainingFail
  )
{
  MrcInput                            *Inputs;
  MRC_EXT_INPUTS_TYPE                 *ExtInputs;
  MrcSaveData                         *SaveData;
  MRC_FUNCTION                        *MrcCall;
  MrcControllerIn                     *ControllerIn;
  MrcChannelIn                        *ChannelIn;
  UINT8                               Dimm;
  MrcDimmIn                           *DimmIn;
  UINT8                               SpdNum;
  UINT8                               McNum;
  UINT8                               ChNum;
  UINT8                               DimmNum;
  BOOLEAN                             IsSpdMatched;
  UINT8                               Index;
  UINT8                               CkdIndex;
  BOOLEAN                             Ddr5DoubleSize1Dpc;
  BOOLEAN                             Ddr5DoubleSize2Dpc;
  BOOLEAN                             Lpddr5CammPresent;
  MRC_SPD_MATCHED                     SpdMatched[MAX_SYS_CHANNEL];
  UINT8                               SpdCount;
  UINT8                               Controller;
  UINT8                               Channel;
  UINT8                               Rank;
  PCU_CR_PLATFORM_INFO_STRUCT         PlatformInfoMsr;
  SIMICS_MEMFLOW_STRUCT               Memflows1;
  SIMICS_MEMFLOW2_STRUCT              Memflows2;
  CPU_MEMORY_INIT_CONFIG              CpuMemoryInitConfig;

  Inputs  = &MrcData->Inputs;
  MrcCall = Inputs->Call.Func;
  SaveData  = &MrcData->Save.Data;
  ExtInputs = Inputs->ExtInputs.Ptr;
  Ddr5DoubleSize1Dpc = FALSE;
  Ddr5DoubleSize2Dpc = FALSE;
  Lpddr5CammPresent  = FALSE;

  if (ExtInputs->RetrainToWorkingChannel && (BootMode == bmCold)) {
    if (MrcDisableFailingChannels (MrcData, READ_RESTORE_FROM_SCRATCHPAD)) {
      DEBUG ((DEBUG_INFO, "All Channels are Enabled\n"));
    }
  }

  // ASSERT if the Config block is not DWORD Aligned.
  ASSERT (sizeof(MEMORY_CONFIGURATION) % sizeof(UINT32) == 0);

  // ASSERT if the RcompTarget Array is not UINT16 Aligned.
  DEBUG ((DEBUG_INFO, "ExtInputs: 0x%lx, RcompTarget: 0x%lx\n", (UINTN)ExtInputs, (UINTN)ExtInputs->RcompTarget));
  ASSERT ((OFFSET_OF(MRC_EXT_INPUTS_TYPE, RcompTarget) % sizeof(UINT16)) == 0);
  DEBUG ((DEBUG_INFO, "Outputs:   0x%lx, RcompTarget: 0x%lx\n", (UINTN)&MrcData->Outputs, (UINTN)MrcData->Outputs.RcompTarget));
  ASSERT ((OFFSET_OF(MrcOutput, RcompTarget) % sizeof(UINT16)) == 0);

  Inputs->MaxVrefSamplesOvrd  = 0;

  //
  // Force standard profile when system boot mode indicates. Usually means some has cleared CMOS.
  //
  if (SysBootMode == BOOT_WITH_DEFAULT_SETTINGS) {
    ExtInputs->MemoryProfile  = STD_PROFILE;
    ExtInputs->Ratio          = 0;
    ExtInputs->VddVoltage     = VDD_INVALID;
    ExtInputs->VddqVoltage    = VDD_INVALID;
    ExtInputs->VppVoltage     = VDD_INVALID;
    ExtInputs->DynamicMemoryBoost        = FALSE;
    ExtInputs->RealtimeMemoryFrequency   = FALSE;
  }

  Inputs->MemoryProfileSave = ExtInputs->MemoryProfile;

  //
  // Setup the base addresses.
  //
  Inputs->MchBarBaseAddress = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  Inputs->SmbusBaseAddress  = (UINT32) PcdGet16 (PcdSmbusBaseAddress);

  //
  // MMIO size in MB units (below 4GB)
  //
  Inputs->MmioSize = GetPreMemMmioSize ();

  //
  // DDR maximum frequency
  //
  Inputs->FreqMax = ExtInputs->DdrFreqLimit;

  ZeroMem (&CpuMemoryInitConfig, sizeof (CPU_MEMORY_INIT_CONFIG));
  ///
  /// Obtain Memory Init Config
  ///
  CpuGetMemoryInitConfig (&CpuMemoryInitConfig);

  /**
    TSEG Size in MB units
    Also TSEG size must be power of 2.
    @note When re-calculating we do not enlarge TSEG to next power of 2 size because the minimal TSEG size is recommended.
  **/
  Inputs->TsegSize = CpuMemoryInitConfig.TsegSize;

  Inputs->TmeEnable = CpuMemoryInitConfig.TmeEnable;
  Inputs->PrmrrSize = CpuMemoryInitConfig.PrmrrSize;
  ///
  /// Base physical address to be excluded from TME encryption.
  ///
  Inputs->TmeExcludeBase = CpuMemoryInitConfig.TmeExcludeBase;
  ///
  /// Size of range to be excluded from TME encryption.
  ///
  Inputs->TmeExcludeSize = CpuMemoryInitConfig.TmeExcludeSize;
  //
  // whether to generate a new TME key
  //
  Inputs->GenerateNewTmeKey = CpuMemoryInitConfig.GenerateNewTmeKey;

  Inputs->PmaCceConfig = GetPmaCceConfig (&CpuMemoryInitConfig, Inputs->TmeEnable);

  Inputs->SkuType   = MrcGetSkuType ();

  Inputs->EnterInterpreterOnPostCode = 0;

DEBUG_CODE_BEGIN();
  ExtInputs->TrainingEnables.RMT         = 1;      // Enable RMT in debug build regardless of the setup option
  ExtInputs->TrainingEnables2.RMTBIT     = 1;      // Enable RMT per Bit in debug build regardless of the setup option
  //Inputs->RmtPerTask                  = FALSE;  // Run RMT after major training steps
DEBUG_CODE_END();

  PlatformInfoMsr.Data = MrcCall->MrcReadMsr64 (PCU_CR_PLATFORM_INFO);
  Inputs->TscTimeBase = (BCLK_DEFAULT / 1000 / 1000) * PlatformInfoMsr.Bits.MAX_NON_TURBO_LIM_RATIO; // In microsec

  Inputs->BdatEnable        = (BOOLEAN) (MemConfigNoCrc->MrcBdatEnable);
  Inputs->BdatTestType      = MemConfigNoCrc->MrcBdatTestType;
  Inputs->ErrorCountForFail = MRC_BER_ERROR_COUNTER_FOR_FAILURE;
  Inputs->BER = 1;
  ExtInputs->TrainTrace     = FALSE;

  for (Index = 0; Index < PPR_REQUEST_MAX; Index++) {
    Inputs->PprEntryInfo[Index] = MemConfigNoCrc->PprEntryInfo[Index];
    Inputs->PprEntryAddress[Index] = MemConfigNoCrc->PprEntryAddress[Index];
  }
  Inputs->PprTestType.Value = MemConfigNoCrc->PprTestType.Value;
  Inputs->PprRepairType     = MemConfigNoCrc->PprRepairType;
  Inputs->PprRetryLimit     = MemConfigNoCrc->PprRetryLimit;
  Inputs->PprRunOnce        = MemConfigNoCrc->PprRunOnce;
  Inputs->PprErrorInjection = MemConfigNoCrc->PprErrorInjection;
  Inputs->PprForceRepair    = MemConfigNoCrc->PprForceRepair;

  Inputs->OffsetKnobs.CAVrefCtlOffset = MemConfigNoCrc->CAVrefCtlOffset;
  Inputs->OffsetKnobs.VrefCtlOffset   = MemConfigNoCrc->VrefCtlOffset;
  Inputs->OffsetKnobs.ClkPiCodeOffset = MemConfigNoCrc->ClkPiCodeOffset;
  Inputs->OffsetKnobs.RcvEnOffset     = MemConfigNoCrc->RcvEnOffset;
  Inputs->OffsetKnobs.RxDqsOffset     = MemConfigNoCrc->RxDqsOffset;
  Inputs->OffsetKnobs.TxDqOffset      = MemConfigNoCrc->TxDqOffset;
  Inputs->OffsetKnobs.TxDqsOffset     = MemConfigNoCrc->TxDqsOffset;
  Inputs->OffsetKnobs.VrefOffset      = MemConfigNoCrc->VrefOffset;
  Inputs->OffsetKnobs.CntrlrMask      = MemConfigNoCrc->CntrlrMask;
  Inputs->OffsetKnobs.ChMask          = MemConfigNoCrc->ChMask;

  // LPDDR4: Bitmask of ranks that have CA bus terminated. Rank0 is terminating and Rank1 is non-terminating
  ExtInputs->CmdRanksTerminated = 0x01;

  Inputs->CleanMemory = (UINT8) MemConfigNoCrc->CleanMemory;

  //
  // Besides STD profile, XMP3.0 supports XMP1, XMP2, XMP3, USER4, USER5 profiles
  // Dynamic Memory Boost only supports certain POR configurations:
  // - Only allowed if using one of XMP1/2/3 profile
  // - Else if, and using any XMP profile, then disable SAGV.
  //
  if ((ExtInputs->MemoryProfile < XMP_PROFILE1) || (ExtInputs->MemoryProfile > XMP_PROFILE3)) {
    ExtInputs->DynamicMemoryBoost = FALSE;
  }
  if (ExtInputs->RealtimeMemoryFrequency && (ExtInputs->MemoryProfile != XMP_PROFILE1)) {
    ExtInputs->RealtimeMemoryFrequency = FALSE;
  }

  /*
   * When both DynamicMemoryBoost and RealtimeMemoryFrequency are TRUE,
   * disable RealtimeMemoryFrequency
   */
  if (ExtInputs->DynamicMemoryBoost && ExtInputs->RealtimeMemoryFrequency) {
    ExtInputs->RealtimeMemoryFrequency = FALSE;
  }
  if (ExtInputs->DynamicMemoryBoost || ExtInputs->RealtimeMemoryFrequency) {
    // Force to enable SAGV
    ExtInputs->SaGv = MRC_SAGV_ENABLED;
    ExtInputs->SaGvWpMask = 0x3; // SAGV point 0 and 1
  } else if ((ExtInputs->MemoryProfile >= XMP_PROFILE1) && (ExtInputs->MemoryProfile <= XMP_PROFILE3)) {
    ExtInputs->SaGv = MRC_SAGV_DISABLED;
    ExtInputs->SaGvWpMask = 0x1; // SAGV point 0
  }

  Inputs->LpFreqSwitch = FALSE;
  Inputs->ReadMprVA = FALSE;
  Inputs->Mars = FALSE;
  Inputs->Lp58BankMode = 0; // Lp5 8-Bank Mode
  Inputs->EnableOdtMatrix = TRUE;
  Inputs->IsCaDeselectStress    = FALSE;
  Inputs->IsIOTestAddressRandom = FALSE; // Enabled on the fly inside RMT; do not enable here
  Inputs->RxDqVrefPerBit = TRUE; // Enable the RX DQ VREF PER BIT.
  Inputs->IsKeepUcssPostMrc = 0;

  Inputs->FourToggleReadPreamble = TRUE;
  Inputs->InitPerDeviceNnFlex = FALSE;
  Inputs->LockUiDiv6Flow = FALSE;

  Inputs->EnablePda = TRUE;
  Inputs->Lp5NZQCount = 0;
  Inputs->IsXtensaEccDisabled = TRUE;
  Inputs->SenseAtRxDll = TRUE;
  Inputs->MinAllowedNModeOvrd = CA_2_NMODE;

  Inputs->MptuPropagationErrorFlow = FALSE;

  Inputs->RloadTarget = 1000;

  Inputs->IsOneDpcSplitBgEnabled = FALSE;

  ExtInputs->MemoryProfile = STD_PROFILE;

  SpdCount = 0;
  MrcCall->MrcSetMem ((UINT8 *) SpdMatched, sizeof (SpdMatched), 0);
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerIn = &Inputs->Controller[Controller];
    ControllerIn->ChannelCount = 0;
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelIn = &ControllerIn->Channel[Channel];
      DEBUG ((DEBUG_ERROR, "!!MC:%d CH:%d DisabledChannel %d!!\n", Controller, Channel, ExtInputs->DisableChannel[Controller][Channel]));
      {
        // Decide which channels are disabled.
        if (ExtInputs->DisableChannel[Controller][Channel]) {
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            ChannelIn->Dimm[Dimm].Status = DIMM_DISABLED;
          }
          ChannelIn->Status         = CHANNEL_DISABLED;
          ChannelIn->DimmCount      = 0;
        } else {
          for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
            ChannelIn->Dimm[Dimm].Status = DIMM_ENABLED;
          }
          ChannelIn->Status         = CHANNEL_PRESENT;
          ControllerIn->ChannelCount++;
          ChannelIn->DimmCount      = 2;
        }
      }

      if (ExtInputs->SimicsFlag) {
        MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqsMapCpu2Dram, (UINT8 *) DqsMapCpu2DramNoSwizzle[0], sizeof (ChannelIn->DqsMapCpu2Dram));
        for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
          MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqMapCpu2Dram[Rank], (UINT8 *) DqMapCpu2DramNoSwizzle, sizeof (ChannelIn->DqMapCpu2Dram[rRank0]));
        }
      } else {
        // Only LP5 should be supporting Input Dqs/Dq swizzle mapping since DDR5 supports Swizzle Training
        MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqsMapCpu2Dram, (UINT8 *) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[Controller][Channel], MAX_BYTE_IN_LP_CHANNEL);
        // LP5 Rank0 and Rank1 should have same swizzle info
        MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqMapCpu2Dram[rRank0], (UINT8 *) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[Controller][Channel], (MAX_BYTE_IN_LP_CHANNEL * MAX_BITS));
        MrcCall->MrcCopyMem ((UINT8 *) ChannelIn->DqMapCpu2Dram[rRank1], (UINT8 *) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[Controller][Channel], (MAX_BYTE_IN_LP_CHANNEL * MAX_BITS));
      }

      //
      // Get DIMM SpdBaseAddresses.
      //
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmIn = &ChannelIn->Dimm[Dimm];
        // Using 2 instead of MAX_DIMMS_IN_CHANNEL as MEMORY_CONFIG_NO_CRC.SpdAddressTable assumes up to 2DPC regardless of MRC definition of MAX_DIMMS_IN_CHANNEL
        Index  = (Controller * MAX_CHANNEL * 2) + (Channel * 2) + Dimm;
        DimmIn->SpdAddress = MemConfigNoCrc->SpdAddressTable[Index];
        //DimmIn->CkdAddress = MemConfigNoCrc->CkdAddressTable[Index];
        MrcCall->MrcCopyMem ((UINT8 *) &DimmIn->Spd.MrcSpdString[0], (UINT8 *) MrcSpdStringConst, sizeof (DimmIn->Spd.MrcSpdString));
        ZeroMem (&DimmIn->Spd.Data, sizeof (MrcSpd));
        DimmIn->Spd.Flag.Bit.DimmNumber    = Dimm;
        DimmIn->Spd.Flag.Bit.ChannelNumber = Channel;
        DimmIn->Spd.Flag.Bit.MdSocket      = (DimmIn->SpdAddress > 0) ? 1 : 0;
        DEBUG ((DEBUG_INFO, "Spd Address for Mc %d Channel %d Dimm %d: %x\n", Controller, Channel, Dimm, DimmIn->SpdAddress));
        //DEBUG ((DEBUG_INFO, "Ckd Address for Mc %d Channel %d Dimm %d: %x\n", Controller, Channel, Dimm, DimmIn->CkdAddress));
        if ((DIMM_ENABLED == DimmIn->Status) || (DIMM_DISABLED == DimmIn->Status)) {
          IsSpdMatched = FALSE;
          McNum = 0;
          ChNum = 0;
          DimmNum = 0;
          // Check if SPD address already exists
          for (SpdNum = 0; SpdNum < SpdCount; SpdNum++) {
            if (DimmIn->SpdAddress == SpdMatched[SpdNum].SpdAddress) {
              IsSpdMatched = TRUE;
              McNum = SpdMatched[SpdNum].Controller;
              ChNum = SpdMatched[SpdNum].Channel;
              DimmNum = SpdMatched[SpdNum].Dimm;
              break;
            }
          }

          if (IsSpdMatched) {
            // Copy SPD if SPD Address are match, so don't have to read it twice
            MrcCall->MrcCopyMem ((UINT8*) &DimmIn->Spd.Data, (UINT8*) &Inputs->Controller[McNum].Channel[ChNum].Dimm[DimmNum].Spd.Data, sizeof (MrcSpd));
          } else if ((Controller == 1) && (Ddr5DoubleSize1Dpc || Ddr5DoubleSize2Dpc|| Lpddr5CammPresent)) {
            // DDR5 x128 module has a single SPD EEPROM for four x32 channels
            // Copy SPD from Controller 0 into Controller 1, for all channels / DIMMs
            MrcCall->MrcCopyMem ((UINT8 *) &DimmIn->Spd.Data, (UINT8 *) &Inputs->Controller[0].Channel[Channel].Dimm[Dimm].Spd.Data, sizeof (MrcSpd));
          } else if ((Dimm == 1) && Ddr5DoubleSize2Dpc) {
            // DDR5 2DPC x128 module has a single SPD EEPROM for four x32 channels
            // Copy SPD from DIMM1 into DIMM0
            MrcCall->MrcCopyMem ((UINT8 *) &DimmIn->Spd.Data, (UINT8 *) &ControllerIn->Channel[Channel].Dimm[0].Spd.Data, sizeof (MrcSpd));
          } else if ((Lpddr5CammPresent) && ((Channel == 1) || (Channel == 2) || (Channel == 3))) {
            // Copy SPD from Channel 0 into Channel 1, Channel 2, Channel 3
            MrcCall->MrcCopyMem ((UINT8 *) &DimmIn->Spd.Data, (UINT8 *) &ControllerIn->Channel[0].Dimm[Dimm].Spd.Data, sizeof (MrcSpd));
          } else {
            if (DimmIn->SpdAddress > 0) {
              if ((bmCold == BootMode) || (bmFast == BootMode)) {
                MrcCall->MrcGetSpdData (BootMode, DimmIn->SpdAddress, (UINT8*) &DimmIn->Spd.Data, (UINT8*) &SpdDdr5Table, sizeof (SpdDdr5Table), (UINT8*) &SpdLpddrTable, sizeof (SpdLpddrTable));
                // Update SPD list
                SpdMatched[SpdCount].SpdAddress = DimmIn->SpdAddress;
                SpdMatched[SpdCount].Controller = Controller;
                SpdMatched[SpdCount].Channel = Channel;
                SpdMatched[SpdCount].Dimm = Dimm;
                SpdCount++;
              }
            } else {
              MrcCall->MrcCopyMem ((UINT8*) &DimmIn->Spd.Data, (UINT8*) &MemConfigNoCrc->SpdData->SpdData[Controller][Channel][Dimm][0], sizeof (MrcSpd));
            }
            if (DimmIn->Spd.Data.Ddr5.Base.DramDeviceType.Bits.Type == MRC_SPD_DDR5_SDRAM_TYPE_NUMBER) {
              if ((DimmIn->Spd.Data.Ddr5.Base.ModuleType.Bits.ModuleType == CammMemoryPackage) &&
                 (DimmIn->Spd.Data.Ddr5.ModuleCommon.ModuleOrganization.Bits.RankCount <= 1)) {
                Ddr5DoubleSize1Dpc = TRUE;
              }
              if ((DimmIn->Spd.Data.Ddr5.Base.ModuleType.Bits.ModuleType == CammMemoryPackage) &&
                 (DimmIn->Spd.Data.Ddr5.ModuleCommon.ModuleOrganization.Bits.RankCount == 3)) {
                Ddr5DoubleSize2Dpc = TRUE;
              }
            } else if ((DimmIn->Spd.Data.Ddr5.Base.DramDeviceType.Bits.Type == MRC_SPD_LPDDR5_SDRAM_TYPE_NUMBER) ||
                       (DimmIn->Spd.Data.Ddr5.Base.DramDeviceType.Bits.Type == MRC_SPD_LPDDR5X_SDRAM_TYPE_NUMBER)) {
               if (DimmIn->Spd.Data.Ddr5.Base.ModuleType.Bits.ModuleType == CammMemoryPackage) {
                 // SPD byte 3 is 8
                 Lpddr5CammPresent = TRUE;
                 DEBUG ((DEBUG_INFO, "Lpddr5CammPresent = TRUE\n"));
               }
            }
          }
        }
        if (DimmIn->Spd.Data.Ddr5.Base.DramDeviceType.Bits.Type == MRC_SPD_DDR5_SDRAM_TYPE_NUMBER && (Channel < MAX_DDR5_CHANNEL)) {
          CkdIndex = (Controller * MAX_DDR5_CHANNEL * MAX_DIMMS_IN_CHANNEL) + (Channel * MAX_DIMMS_IN_CHANNEL) + Dimm;
          DimmIn->ChannelToCkdQckMapping = MemConfigNoCrc->ChannelToCkdQckMapping[CkdIndex];
          DimmIn->PhyClockToCkdDimm      = MemConfigNoCrc->PhyClockToCkdDimm[CkdIndex];
          DEBUG ((DEBUG_INFO, "MC%dCH%dD%d:\n\t%a = %d\n\t%a = %d\n", Controller, Channel, Dimm, Ch2CkdQckString, DimmIn->ChannelToCkdQckMapping, PhyClk2CkdString, DimmIn->PhyClockToCkdDimm));
        }
      } // for Dimm
    } // for Channel
  } // for Controller

  Inputs->Lpddr5Camm = Lpddr5CammPresent;

  CheckForTimingOverride (Inputs, MemConfig);

  // Read Simics provided MSR to set training steps
  if (ExtInputs->SimicsFlag) {
    Memflows1.Data = MrcCall->MrcReadMsr64 (MSR_SIMICS_MEMFLOWS_LOW);
    Memflows2.Data = MrcCall->MrcReadMsr64 (MSR_SIMICS_MEMFLOWS_HIGH);

    // If Simics reserved bit is set, then use mrc_mem_flow overrides
    if ((Memflows1.Data & MRC_BIT63) != 0) {
      MrcCall->MrcCopyMem ((UINT8 *)&ExtInputs->TrainingEnables,  (UINT8 *) &Memflows1.Bits.Low,  sizeof(TrainingStepsEn));
      MrcCall->MrcCopyMem ((UINT8 *)&ExtInputs->TrainingEnables2, (UINT8 *) &Memflows1.Bits.High, sizeof(TrainingStepsEn2));
      MrcCall->MrcCopyMem ((UINT8 *)&ExtInputs->TrainingEnables3, (UINT8 *) &Memflows2.Bits.Low,  sizeof(TrainingStepsEn3));
    } else {
      // Else, use Bios Knob setting
      // Disable trainings not supported in simics currently
      MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables,  sizeof (TrainingStepsEn),  0);
      MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables2, sizeof (TrainingStepsEn2), 0);
      MrcCall->MrcSetMem ((UINT8 *) &ExtInputs->TrainingEnables3, sizeof (TrainingStepsEn3), 0);
    }
  }

  if ( (DidPreviousTrainingFail && ExtInputs->DynamicMemoryBoost && (BootMode == bmCold))
       || (SaveData->DynamicMemoryBoostTrainingFailed && ExtInputs->DynamicMemoryBoost && (BootMode != bmCold))
     ) {
    //
    // Dynamic Memory Boost training failed, fallback to Dynamic Memory Boost disabled, using Standard Profile with SAGV disabled.
    //
    DEBUG ((DEBUG_INFO, "Previous full training failed - disable DMB, disable SAGV and set MemoryProfile to Standard Profile\n"));
    ExtInputs->DynamicMemoryBoost              = FALSE;
    SaveData->DynamicMemoryBoostTrainingFailed = TRUE;
    ExtInputs->MemoryProfile                   = STD_PROFILE;
    /*
     * Some XMP memory might not work well at max speed at STD_PROFILE.
     * Restrict DdrFreqLimit to low speed in case failover can't boot.
     */
    Inputs->FreqMax                            = f4000;
    ExtInputs->SaGv                            = MRC_SAGV_DISABLED;
  } else if (BootMode == bmCold) {
    SaveData->DynamicMemoryBoostTrainingFailed = FALSE;
  }

  switch (BootMode) {
    case bmWarm:
    case bmS3:
    case bmFast:
      return BootMode;

    default:
      DEBUG ((DEBUG_ERROR, "ERROR: Invalid flow specified, defaulting to cold flow\n"));
      // No break. Note that the boot mode changes to bmCold.
    case bmCold:
      break;
  }
  return bmCold;
}

/**
  Check to see if user defined profile is selected and if it is, then copy the
  timing settings for this profile to the timing override structure. If user
  defined profile is not selected, then set the timing override structure to 0.

  @note Even though we set timings on a DIMM by DIMM basis, the controller
  may force DIMM timings to be the same for all DIMMs in a channel.

  @param[in, out] Inputs              - The MRC Input data structure.
  @param[in]      MemConfig           - MEMORY_CONFIGURATION structure.
**/
void
CheckForTimingOverride (
  IN OUT MrcInput *const          Inputs,
  IN     MEMORY_CONFIGURATION     *MemConfig
  )
{
  MRC_EXT_INPUTS_TYPE         *ExtInputs;
  MrcControllerIn             *ControllerIn;
  MrcChannelIn                *ChannelIn;
  MrcTiming                   *Timing;
  UINT8                       Controller;
  UINT8                       Channel;
  UINT8                       Dimm;

  ExtInputs = Inputs->ExtInputs.Ptr;

  //
  // Override DIMM timing settings for customer profile setting.
  //
  if (ExtInputs->MemoryProfile == CUSTOM_PROFILE1) {
    for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
      ControllerIn = &Inputs->Controller[Controller];
      for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
        ChannelIn = &ControllerIn->Channel[Channel];
        for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
          Timing = &ChannelIn->Dimm[Dimm].Timing;
          Timing->NMode   = ExtInputs->NModeSupport;
          Timing->tCL     = ExtInputs->tCL;
          Timing->tCWL    = ExtInputs->tCWL;
          Timing->tFAW    = ExtInputs->tFAW;
          Timing->tRAS    = ExtInputs->tRAS;
          Timing->tRCDtRP = ExtInputs->tRCDtRP;
          Timing->tREFI   = ExtInputs->tREFI;
          Timing->tRFC    = ExtInputs->tRFC;
          Timing->tRRD    = ExtInputs->tRRD;
          Timing->tRTP    = ExtInputs->tRTP;
          Timing->tWR     = ExtInputs->tWR;
          Timing->tWTR    = ExtInputs->tWTR;
          Timing->tRFCpb  = ExtInputs->tRFCpb;
          Timing->tRFC2   = ExtInputs->tRFC2;
          Timing->tRFC4   = ExtInputs->tRFC4;
          Timing->tRRD_L  = ExtInputs->tRRD_L;
          Timing->tRRD_S  = ExtInputs->tRRD_S;
          Timing->tWTR_L  = ExtInputs->tWTR_L;
          Timing->tCCD_L  = ExtInputs->tCCD_L;
          Timing->tWTR_S  = ExtInputs->tWTR_S;
          Timing->tCCD_L_WR = ExtInputs->tCCD_L_WR;
        }
      }
    }
  }

  return;
}

/**
  Build S3 memory data HOB

  @param[in] MrcData          Pointer to Mrc Parameters
**/
VOID
BuildMemoryS3DataHob (
  IN MrcParameters            *MrcData
  )
{
  EFI_STATUS                       Status;
  EFI_PHYSICAL_ADDRESS             Buffer;
  UINT64                           FspNvsHob2Data[2];
  VOID                             *HobPtr;

  //
  // Allocate permanent memory for MrcData.
  //
  Status = PeiServicesAllocatePages (
             EfiBootServicesData,
             EFI_SIZE_TO_PAGES (MrcData->Save.Size),
             &Buffer
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Copy MrcData to memory
  //
  CopyMem ((VOID *)(UINTN) Buffer, (VOID *) &(MrcData->Save), MrcData->Save.Size);

  FspNvsHob2Data[0] = Buffer;
  FspNvsHob2Data[1] = MrcData->Save.Size;

  DEBUG ((
          DEBUG_INFO,
          "BuildMemoryS3DataHob.\nMemoryS3DataHob Base : 0x%lx\nMemoryS3DataHob Size : 0x%lx\n",
          FspNvsHob2Data[0],
          FspNvsHob2Data[1]
          ));

  HobPtr = BuildGuidDataHob (
             &gSiMemoryS3Data2Guid,
             (VOID *) &(FspNvsHob2Data),
             (UINTN) sizeof(FspNvsHob2Data)
           );
  ASSERT (HobPtr != NULL);
}

/**
  Build SMBIOS and OC memory data HOB

  @param[in] MrcData          Pointer to Mrc Parameters
**/
VOID
BuildMemoryInfoDataHob (
  IN MrcParameters            *MrcData
  )
{
  MEMORY_INFO_DATA_HOB *MemoryInfo;
  CONTROLLER_INFO      *ControllerInfo;
  CHANNEL_INFO         *ChannelInfo;
  DIMM_INFO            *DimmInfo;
  MRC_FUNCTION         *MrcCall;
  MrcInput             *Inputs;
  MRC_EXT_INPUTS_TYPE  *ExtInputs;
  MrcSaveData          *SaveData;
  MrcContSave          *ControllerSave;
  MrcChannelSave       *ChannelSave;
  MrcDimmSpdSave       *DimmSave;
  MrcOutput            *Outputs;
  MrcControllerOut     *ControllerOut;
  MrcChannelOut        *ChannelOut;
  MrcDimmOut           *DimmOut;
  MrcSpd               *Spd;
  MrcProfile           Profile;
  UINT8                Controller;
  UINT8                Channel;
  UINT8                Dimm;
  UINT8                OdtDimmMask;
  UINT8                PartNumberOffset;
  UINT8                ModulePartLength;
  UINT8                NumChannelsPopulated;
  UINT8                NumSubChannels;
  UINT32               PartNumberLength;
  BOOLEAN              IsChannelPopulated;
  BOOLEAN              IsDdr5;
  SPD5_MODULE_MEMORY_BUS_WIDTH  MemoryBusWidthDdr5;
  UINT16               SaGvPoint;
  TOdtValueDqDdr5      DqOdtTableIndex;
  TOdtValueCccDdr5     *CccOdtTableIndex;
  MrcStatus            Status;
  const UINT16         *RcompTarget;
  UINT8                BankGroup;
  ROW_FAIL_RANGE       *FailRangePtr;
  UINT8                Rank;
  BOOLEAN              PprDoneFillingErrorInfo;
  AMT_PPR_ENABLE       AmtPprRanInLastBoot;

  SaveData = &MrcData->Save.Data;
  Outputs  = &MrcData->Outputs;
  Inputs   = &MrcData->Inputs;
  MrcCall  = Inputs->Call.Func;
  ExtInputs = Inputs->ExtInputs.Ptr;
  NumChannelsPopulated = 0;
  NumSubChannels       = 0;
  IsChannelPopulated = FALSE;
  IsDdr5             = FALSE;

  DEBUG ((DEBUG_INFO, "BuildMemoryInfoDataHob.\n"));

  //
  // Build HOB for MemoryInfoData
  //
  MemoryInfo = BuildGuidHob (
                 &gSiMemoryInfoDataGuid,
                 sizeof (MEMORY_INFO_DATA_HOB)
                 );
  ASSERT (MemoryInfo != NULL);
  if (MemoryInfo == NULL) {
    return;
  }
  ZeroMem ((VOID *) MemoryInfo, sizeof (MEMORY_INFO_DATA_HOB));

  MrcVersionGet (MrcData, (MrcVersion *) &MemoryInfo->Version);
  MemoryInfo->Revision = 0x03;
  OdtDimmMask = 0;
  switch (Outputs->DdrType) {
    case MRC_DDR_TYPE_DDR5:
      MemoryInfo->MemoryType = MemoryTypeDdr5;
      OdtDimmMask = (1 << MAX_DIMMS_IN_CHANNEL) - 1;
      IsDdr5 = TRUE;
      break;

    case MRC_DDR_TYPE_LPDDR5:
      MemoryInfo->MemoryType = MemoryTypeLpddr5;
      break;

    default:
      MemoryInfo->MemoryType = MemoryTypeUnknown;
      break;
  }
  MemoryInfo->ConfiguredMemoryClockSpeed = 0;
  for (SaGvPoint = 0; SaGvPoint < MAX_SAGV_POINTS; SaGvPoint++) {
    MemoryInfo->ConfiguredMemoryClockSpeed = MAX (MemoryInfo->ConfiguredMemoryClockSpeed, (UINT16) SaveData->Frequency[SaGvPoint]);
  }
  MemoryInfo->MaximumMemoryClockSpeed    = (UINT16) Outputs->FreqMax;
  if (Outputs->EccSupport) {
    MemoryInfo->ErrorCorrectionType = MemoryErrorCorrectionSingleBitEcc;
  } else {
    MemoryInfo->ErrorCorrectionType = MemoryErrorCorrectionNone;
  }
  MemoryInfo->EccSupport              = Outputs->EccSupport;
  MemoryInfo->TotalPhysicalMemorySize = Outputs->MemoryMapData.TotalPhysicalMemorySize;
  // Max Rank Capacity comes from MC HAS: 16GB in DDR5 and 8GB in LP5
  MemoryInfo->MaxRankCapacity = Outputs->IsDdr5 ? 16 : 8;
  MrcCall->MrcSetMem ((UINT8 *) &MemoryInfo->RcompTarget, sizeof (MemoryInfo->RcompTarget), 0);
  for (Profile = STD_PROFILE; Profile < MAX_PROFILE_NUM; Profile++) {
    MemoryInfo->VddVoltage[Profile]   = Outputs->VddVoltage[Profile];
    MemoryInfo->VddqVoltage[Profile]  = Outputs->VddqVoltage[Profile];
    MemoryInfo->VppVoltage[Profile]   = Outputs->VppVoltage[Profile];
    RcompTarget = MrcGetDefaultRcompTarget(MrcData, Profile);
    if (NULL != RcompTarget) {
      MrcCall->MrcCopyMem ((UINT8 *) &MemoryInfo->RcompTarget[Profile], (UINT8 *) RcompTarget, sizeof (MemoryInfo->RcompTarget[Profile]));
    }
  }

  MemoryInfo->MemoryProfile = ExtInputs->MemoryProfile;

  MemoryInfo->XmpProfileEnable = SaveData->XmpProfileEnable;
  MemoryInfo->XmpConfigWarning = SaveData->XmpConfigWarning;
  MemoryInfo->DynamicMemoryBoostTrainingFailed = SaveData->DynamicMemoryBoostTrainingFailed;
  MemoryInfo->Ratio = SaveData->Ratio;
  MemoryInfo->NumPopulatedChannels = Outputs->NumPopChannel;
  MemoryInfo->IsIbeccEnabled = Inputs->IsIbeccPmaEnabled;
  DEBUG ((DEBUG_INFO, "IsIbeccEnabled: %u\n", MemoryInfo->IsIbeccEnabled));

  AmtPprRanInLastBoot.Data = 0;
  if ((Outputs->PprRunningState == PPR_IS_DONE) && ((Inputs->PprTestType.Value & MRC_PPR_ADV_ALGORITHM_TEST_MASK) != 0)) {
    AmtPprRanInLastBoot.Bits.AmtEnabled = 1;
  }
  if ((Inputs->PprRepairType != 0) && (Outputs->PprNumDetectedErrors != 0)) {
    AmtPprRanInLastBoot.Bits.PprEnabled = 1;
  }
  MemoryInfo->PprRanInLastBoot = AmtPprRanInLastBoot.Data;

  MemoryInfo->PprDetectedErrors       = Outputs->PprNumDetectedErrors;
  MemoryInfo->PprRepairFails          = Outputs->PprRepairFails;
  MemoryInfo->PprForceRepairStatus    = Outputs->PprForceRepairStatus;
  MemoryInfo->PprRepairsSuccessful    = Outputs->PprNumSuccessfulRepairs;
  MemoryInfo->PprFailingChannelBitMask = Outputs->PprFailingChannelBitMask;
  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      for (Rank = 0; Rank < MAX_RANK_IN_CHANNEL; Rank++) {
        for (Dimm = 0; Dimm < MAX_SDRAM_IN_DIMM; Dimm++) {
          MemoryInfo->PprAvailableResources[Controller][Channel][Rank][Dimm] = Outputs->PprAvailableResources[Controller][Channel][Rank][Dimm];
        }
      }
    }
  }
  if (Outputs->PprNumDetectedErrors > 0) {
    PprDoneFillingErrorInfo = FALSE;
    for (Controller = 0; Controller < MAX_CONTROLLER && !PprDoneFillingErrorInfo; Controller++) {
      for (Channel = 0; Channel < MAX_CHANNEL && !PprDoneFillingErrorInfo; Channel++) {
        FailRangePtr = &Outputs->FailRange[Controller][Channel][0];
        if ((Outputs->FailMax[Controller][Channel] > 0) && FailRangePtr->Addr.Bits.Valid) {
          // Determine BankGroup
          for (BankGroup = 0; BankGroup < MAX_BANK_GROUP_CNT && !PprDoneFillingErrorInfo; BankGroup++) {
            if (!(FailRangePtr->BankGroupMask & (1 << BankGroup))) {
              continue;
            }
            MemoryInfo->PprErrorInfo.PprRowRepairsSuccessful = (FailRangePtr->BankGroupRepairedMask & (1 << BankGroup)) != 0;
            MemoryInfo->PprErrorInfo.Controller = Controller;
            MemoryInfo->PprErrorInfo.Channel = Channel;
            MemoryInfo->PprErrorInfo.Rank = (UINT8) FailRangePtr->Addr.Bits.Rank;
            MemoryInfo->PprErrorInfo.BankGroup = BankGroup;
            MemoryInfo->PprErrorInfo.Bank = (UINT8) FailRangePtr->Addr.Bits.BankPair;
            MemoryInfo->PprErrorInfo.Row = FailRangePtr->Addr.Bits.Row;
            MemoryInfo->PprErrorInfo.Device = FailRangePtr->Device;

            // Stop after filling info for one row
            PprDoneFillingErrorInfo = TRUE;
          }
        }
      } // Channel
    } // Controller
  }

  PartNumberOffset = sizeof (SPD_MANUFACTURER_ID_CODE) + sizeof (SPD_MANUFACTURING_LOCATION) + sizeof (SPD_MANUFACTURING_DATE) + sizeof (SPD_MANUFACTURER_SERIAL_NUMBER);

  MrcCall->MrcSetMem ((UINT8 *) &MemoryInfo->DimmOdt, sizeof (MemoryInfo->DimmOdt), 0);

  for (Controller = 0; Controller < MAX_CONTROLLER; Controller++) {
    ControllerSave               = &SaveData->Controller[Controller];
    ControllerOut                = &Outputs->Controller[Controller];
    ControllerInfo               = &MemoryInfo->Controller[Controller];
    ControllerInfo->Status       = ControllerSave->Status;
    ControllerInfo->DeviceId     = ControllerOut->DeviceId;
    ControllerInfo->RevisionId   = ControllerOut->RevisionId;
    ControllerInfo->ChannelCount = ControllerSave->ChannelCount;
    for (Channel = 0; Channel < MAX_CHANNEL; Channel++) {
      ChannelSave                   = &ControllerSave->Channel[Channel];
      ChannelOut                    = &ControllerOut->Channel[Channel];
      ChannelInfo                   = &ControllerInfo->ChannelInfo[Channel];
      ChannelInfo->Status           = ChannelSave->Status;
      ChannelInfo->ChannelId        = Channel;
      ChannelInfo->DimmCount        = (UINT8) ChannelSave->DimmCount;

      for (Profile = STD_PROFILE; Profile < MAX_PROFILE_NUM; Profile++) {
        MrcCall->MrcCopyMem ((UINT8 *) &ChannelInfo->Timing[Profile], (UINT8 *) &Outputs->Timing[Profile], sizeof (MrcTiming));
      }
      IsChannelPopulated = FALSE;
      for (Dimm = 0; Dimm < MAX_DIMMS_IN_CHANNEL; Dimm++) {
        DimmSave = &ChannelSave->DimmSpdSave[Dimm];
        DimmOut  = &ChannelOut->Dimm[Dimm];
        DimmInfo = &ChannelInfo->DimmInfo[Dimm];
        DimmInfo->Status = DimmOut->Status;
        DimmInfo->DimmId = Dimm;
        DimmInfo->DimmCapacity = DimmOut->DimmCapacity;
        DimmInfo->MdSocket = Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Flag.Bit.MdSocket;
        DimmInfo->Banks = DimmOut->Banks;
        DimmInfo->BankGroups = DimmOut->BankGroups;
        DimmInfo->DeviceDensity = DimmOut->DeviceDensity;

        Spd = &Inputs->Controller[Controller].Channel[Channel].Dimm[Dimm].Spd.Data;
        if (Outputs->IsDdr5) {
          DimmInfo->CkdMfgID.Data  = Spd->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.ManufacturerId.Data;
          DimmInfo->CkdDeviceRev   = Spd->Ddr5.ModuleCommon.ModuleSpecific.Unbuffered.DeviceInfoRegister.DeviceRevision;
          DimmInfo->DramMfgID.Data = Spd->Ddr5.ManufactureInfo.DramIdCode.Data;
          DimmInfo->SerialNumber   = Spd->Ddr5.ManufactureInfo.ModuleId.SerialNumber.Data;
          PartNumberLength         = sizeof (SPD5_MODULE_PART_NUMBER);
        } else if (Outputs->IsLP5Camm2) {
          DimmInfo->DramMfgID.Data = Spd->JedecLpddr5.ManufactureInfo.DramIdCode.Data;
          DimmInfo->SerialNumber   = Spd->JedecLpddr5.ManufactureInfo.ModuleId.SerialNumber.Data;
          PartNumberLength         = sizeof (SPD5_MODULE_PART_NUMBER);
        } else {
          // No clock driver for LP devices
          DimmInfo->DramMfgID.Data = Spd->Lpddr.ManufactureInfo.DramIdCode.Data;
          DimmInfo->SerialNumber   = Spd->Lpddr.ManufactureInfo.ModuleId.SerialNumber.Data;
          PartNumberLength         = sizeof (SPD4_MODULE_PART_NUMBER);
        }

        // Copy the SPD before DIMM_PRESENT check, to show Vendor ID in Setup also for disabled DIMMs
        MrcCall->MrcCopyMem (&DimmInfo->SpdSave[0], &DimmSave->SpdSave[0], sizeof (DimmInfo->SpdSave));
        DimmInfo->MfgId.Data8[0] = DimmSave->SpdSave[0];
        DimmInfo->MfgId.Data8[1] = DimmSave->SpdSave[1];
        if ((DimmInfo->Status == DIMM_PRESENT) && (DimmInfo->DimmCapacity > 0)) {
          if (Channel == 0 || !IsDdr5) {
            DimmInfo->RankInDimm = DimmOut->RankInDimm;
            MrcCall->MrcCopyMem ((UINT8 *) &DimmInfo->ModulePartNum[0],
                                (UINT8 *) &DimmSave->SpdSave[PartNumberOffset],
                                PartNumberLength);
            // Pad with spaces in case of LP5 memory down, which only uses 20 bytes
            if (PartNumberLength < sizeof (SPD5_MODULE_PART_NUMBER)) {
              MrcCall->MrcSetMem ((UINT8 *) &DimmInfo->ModulePartNum[sizeof (SPD4_MODULE_PART_NUMBER)], sizeof (SPD5_MODULE_PART_NUMBER) - PartNumberLength, ' ');
            }
            DimmInfo->SpdDramDeviceType       = DimmSave->SpdDramDeviceType;
            DimmInfo->SpdModuleType           = DimmSave->SpdModuleType;
            DimmInfo->Speed                   = (UINT16) DimmOut->Speed;
            MemoryBusWidthDdr5.Data           = DimmSave->SpdModuleMemoryBusWidth;
            NumSubChannels                    = MemoryBusWidthDdr5.Bits.NumberOfChannels + 1;       // SPD encoding: 0: one channel, 1: two channels
            // Double the bus width / bus width extension (ECC) if we have two channels - this only happens in case of DDR5
            DimmInfo->DataWidth               = 8 * (1 << MemoryBusWidthDdr5.Bits.PrimaryBusWidth) * NumSubChannels;
            DimmInfo->TotalWidth              = DimmInfo->DataWidth + (MemoryBusWidthDdr5.Bits.BusWidthExtension * 4 * NumSubChannels);
            if (Channel == 0 && IsDdr5) {
              //
              // In order to report the combined BusWidth and Capacity in Smbios Type17 table and Setup menu,
              // modify the dimm info in HOB according to the number of subchannels in DDR5 module
              //
              DimmInfo->DimmCapacity *= NumSubChannels;  // Double the capacity if we have two channels
            }
            DEBUG ((DEBUG_INFO, "MC%u C%u D%u:\n MfgId: 0x%X, DramMfgID: 0x%X\n NumSubChannels: %u, DataWidth: %u, TotalWidth: %u\n",
              Controller, Channel, Dimm, DimmInfo->MfgId.Data, DimmInfo->DramMfgID.Data, NumSubChannels, DimmInfo->DataWidth, DimmInfo->TotalWidth));
            DEBUG ((DEBUG_INFO, " DeviceDensity: %uGb, DimmCapacity: %u, CkdMfgID: 0x%X, CkdDeviceRev: %u, SerialNumber: 0x%X\n",
              DimmInfo->DeviceDensity, DimmInfo->DimmCapacity, DimmInfo->CkdMfgID.Data, DimmInfo->CkdDeviceRev, DimmInfo->SerialNumber));
            DEBUG ((DEBUG_INFO, " Module Part Number: "));
            for (ModulePartLength = 0; ModulePartLength < sizeof (SPD5_MODULE_PART_NUMBER); ModulePartLength++) {
              DEBUG ((DEBUG_INFO, "%c", DimmInfo->ModulePartNum[ModulePartLength]));
            }
            DEBUG ((DEBUG_INFO, "\n"));
            if (!IsChannelPopulated) {
              IsChannelPopulated = TRUE;
              NumChannelsPopulated += 1;
            }
          } else {// DDR5 Channel 1
            // Mark DDR5 DIMM on channel 1 as not present and zero size, it is reported above as part of ch0
            DimmInfo->Status = DIMM_NOT_PRESENT;
            DimmInfo->DimmCapacity = 0;
          }
          MemoryInfo->TotalMemWidth += DimmInfo->DataWidth; // Track the total number of data bits in the system (without ECC)

          if (OdtDimmMask & (1 << Dimm)) {
            for (Profile = STD_PROFILE; Profile < MAX_PROFILE_NUM; Profile++) {
              if (Outputs->DdrType != MRC_DDR_TYPE_DDR5) {
                continue;
              }

              Status = GetProfileOdtTableIndex (MrcData, Profile, Controller, Channel, Dimm, &DqOdtTableIndex);
              if (mrcSuccess == Status) {
                MrcCall->MrcCopyMem ((UINT8 *) &MemoryInfo->DimmOdt[Profile][Dimm], (UINT8 *) &DqOdtTableIndex, sizeof (DqOdtTableIndex));
              }

              CccOdtTableIndex = (TOdtValueCccDdr5 *) SelectCccTableDdr5 (MrcData, Profile, Controller, Channel, Dimm);
              if (NULL != CccOdtTableIndex) {
                // Copy both GroupA and GroupB
                MrcCall->MrcCopyMem ((UINT8 *) &MemoryInfo->DimmOdt[Profile][Dimm][OFFSET_RTTCA_GROUPA], (UINT8 *) &CccOdtTableIndex[0], sizeof (TOdtValueCccDdr5) * 2);
              }
            }
            OdtDimmMask &= ~(1 << Dimm);
          }
        }
      }
#if MAX_DIMMS_IN_CHANNEL == 1
      // No second DIMM in MRC host data; mark DIMM1 as "not present" in the HOB accordingly
      DimmInfo = &ChannelInfo->DimmInfo[dDIMM1];
      DimmInfo->Status = DIMM_NOT_PRESENT;
#endif
    }
  }
  DEBUG ((DEBUG_INFO, "NumPopulatedChannels: %u, TotalMemWidth(bits): %u\n", MemoryInfo->NumPopulatedChannels, MemoryInfo->TotalMemWidth));

  // Copy Over the SaGvOutputs Data
  // Make sure the the source and destination structures match in size
  ASSERT (sizeof (HOB_SAGV_INFO) == sizeof (MrcSaGvOutput));

  // Make sure the structure is UINT32 aligned
  ASSERT ((sizeof (MrcSaGvOutput) % sizeof (UINT32)) == 0);

  CopyMem (&MemoryInfo->SagvConfigInfo, &SaveData->SaGvOutputs, sizeof (HOB_SAGV_INFO));
  DEBUG ((DEBUG_INFO, "NumSaGvPointsEnabled: %u, SaGvPointMask: 0x%x\n", SaveData->SaGvOutputs.NumSaGvPointsEnabled, SaveData->SaGvOutputs.SaGvPointMask));
}

/**
  Build platform memory data HOB

  @param[in]      MrcData             Pointer to Mrc Parameters
  @param[in, out] MemoryPlatformData  Hob to pass memory data for platform code
**/
VOID
BuildMemoryPlatformDataHob (
  IN     MrcParameters            *MrcData,
  IN OUT MEMORY_PLATFORM_DATA_HOB *MemoryPlatformData
  )
{
  MrcInput      *Inputs;
  MrcOutput     *Outputs;
  MrcMemoryMap  *MemoryMapData;

  Outputs = &MrcData->Outputs;
  Inputs  = &MrcData->Inputs;
  MemoryMapData = &Outputs->MemoryMapData;

  DEBUG ((DEBUG_INFO, "BuildMemoryPlatformDataHob.\n"));
  MemoryPlatformData->Data.Revision        = 0x02;
  MemoryPlatformData->Data.BootMode        = Inputs->BootMode;
  MemoryPlatformData->Data.TsegSize        = Inputs->TsegSize;
  MemoryPlatformData->Data.TsegBase        = MemoryMapData->TsegBase;
  MemoryPlatformData->Data.PrmrrSize       = Inputs->PrmrrSize;
  MemoryPlatformData->Data.PrmrrBase       = MemoryMapData->PrmrrBase;
  MemoryPlatformData->Data.GttBase         = MemoryMapData->GttBase;
  MemoryPlatformData->Data.MmioSize        = Inputs->MmioSize;
  MemoryPlatformData->Data.PciEBaseAddress = (UINT32) Inputs->PciEBaseAddress;
  MemoryPlatformData->Data.MrcBasicMemoryTestPass = Outputs->MrcBasicMemoryTestPass;
}


/**
  Read the last SAGV point before Warm / S3 entry from a PMC register

  @param[in, out]  MrcData - All the MRC global data.

**/
VOID
MrcGetSaGvPointBeforeReset (
  IN OUT  MrcParameters *const MrcData
  )
{
  MrcInput  *Inputs;

  Inputs = &MrcData->Inputs;

  PmcReadSaGvPointBeforeReset (&Inputs->SaGvBeforeReset);
}

/**
  Check Status of the last IBECC operation mode

  @param[in, out]  MrcData - All the MRC global data.

  @return The last IBECC operation mode setting
**/
UINT8
GetLastIbeccOpMode (
  IN OUT  MrcParameters *const MrcData
  )
{
  M_PCU_CR_SSKPD_PCU_STRUCT SskpdData64 = {0};

  SskpdData64.Data = MrcWmRegGet (MrcData);
  return (UINT8) SskpdData64.Bits.LastIbeccOpMode;
}

/**
  Set the status of the current IBECC operation mode to the SSKPD_PCU register.
  This status will survive a warm reset.

  @param[in]  IbeccOpMode - The last configured IBECC Operation Mode
**/
VOID
SetLastIbeccOpMode (
  IN UINT8  IbeccOpMode
  )
{
  M_PCU_CR_SSKPD_PCU_STRUCT SskpdData64 = {0};
  UINT64                    MchBar;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  SskpdData64.Data = MmioRead64 (MchBar + SSKPD_PCU_REG);
  DEBUG ((DEBUG_INFO, "%a SSKPD_PCU R = 0x%llx\n", __FUNCTION__, SskpdData64.Data));
  SskpdData64.Bits.LastIbeccOpMode = IbeccOpMode;
  MmioWrite64 (
    MchBar + SSKPD_PCU_REG,
    SskpdData64.Data
    );
  DEBUG ((DEBUG_INFO, "%a SSKPD_PCU W = 0x%llx\n", __FUNCTION__, SskpdData64.Data));
}

/**
  Update the progress bar showing the MRC completion status.

  @param[in out]  MrcData - All the MRC global data.
**/
VOID
MrcUpdateProgressBar (
  IN OUT MrcParameters *CONST MrcData
  )
{
  MrcDebug    *Debug;
  MrcSaveData *SaveData;
  UINT32      Percentage;

  Debug = &MrcData->Outputs.Debug;
  SaveData = &MrcData->Save.Data;

  SaveData->PostCodesDone += 1;

  if (SaveData->PostCodesTotal == 0) {
    UpdateProgressBar (0);
  } else {
    if (Debug->PostCode[MRC_POST_CODE] != MRC_MEM_INIT_DONE) {
      Percentage = UDIVIDEROUND (SaveData->PostCodesDone * 100, SaveData->PostCodesTotal);
      UpdateProgressBar ((UINT8) Percentage);
    } else {
      UpdateProgressBar (100);
    }
  }
}
