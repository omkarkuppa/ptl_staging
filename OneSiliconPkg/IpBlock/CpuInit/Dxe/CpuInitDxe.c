/** @file
  Cpu driver, which initializes CPU and implements CPU Architecture
  Protocol as defined in Framework specification.

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

#include "BiosGuard.h"
#include "CpuInitDxe.h"
#include <Library/BaseLib.h>
#include <Library/CpuCacheInfoLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/HobLib.h>
#include <Library/LocalApicLib.h>
#include <PowerMgmtNvsStruct.h>
#include <Protocol/CpuInfo.h>
#include <Protocol/DxeSmmReadyToLock.h>
#include <Protocol/MpService.h>
#include <Protocol/CpuNvsArea.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <Library/PtlCpuInfoLib.h>

//
// Private GUIDs for BIOS Guard initializes
//
EFI_MP_SERVICES_PROTOCOL  *mMpService;

UINT32  mCoreTypeBitMap            = 0;
UINT8   mNumBigCore                = 0;
UINT8   mNumSmallCore              = 0;
UINT8   mBigCoreThreads            = 0;
UINT8   mSmallCoreThreads          = 0;
UINT8   *mDieModuleIndex           = NULL;
UINTN   mNumberOfProcessors        = 0;
UINTN   mNumberOfEnabledProcessors = 0;
///
/// The Cpu Init Data Hob
///
GLOBAL_REMOVE_IF_UNREFERENCED CPU_INFO_PROTOCOL                      mCpuInfo;

/**
  Set all APs to deepest C-State before ready to boot for better power saving
**/
VOID
EFIAPI
RequestDeepestCStateForAps (
  IN  EFI_EVENT Event,
  IN  VOID      *Context
)
{
  CPUID_MONITOR_MWAIT_ECX                      MonitorMwaitEcx;
  CPUID_MONITOR_MWAIT_EDX                      MonitorMwaitEdx;
  UINT8                                         MaxCstate;
  MSR_BROADWELL_PKG_CST_CONFIG_CONTROL_REGISTER Msr;
  UINT32                                        SubStates;

  ///
  /// APs should be at deepest C-State before ready to boot for better power saving,
  /// if boot to DOS/EFI_SHARE or any operating system that running only single thread.
  ///
  /// BIOS should use CPUID.(EAX=5) Monitor/Mwait Leaf and also check MSR E2h[3:0] Package C-state limit to determine
  /// if the processor supports MONITOR/MWAIT extensions for various C-states and sub C-states.
  ///
  Msr.Uint64 = AsmReadMsr64 (MSR_BROADWELL_PKG_CST_CONFIG_CONTROL);
  GetSubCStateSupported ((UINT32 *)&MonitorMwaitEcx, (UINT32 *)&MonitorMwaitEdx);
  MaxCstate = 0;
  SubStates = 0;
  if (MonitorMwaitEcx.Bits.ExtensionsSupported) {
    switch (Msr.Bits.Limit) {
      case V_CSTATE_LIMIT_C10:
        SubStates = MonitorMwaitEdx.Bits.C7States;
        MaxCstate = 0x60;
        break;

      case V_CSTATE_LIMIT_C9:
        SubStates = MonitorMwaitEdx.Bits.C6States;
        MaxCstate = 0x50;
        break;

      case V_CSTATE_LIMIT_C8:
        SubStates = MonitorMwaitEdx.Bits.C5States;
        MaxCstate = 0x40;
        break;

      case V_CSTATE_LIMIT_C7S:
        SubStates = MonitorMwaitEdx.Bits.C4States;
        MaxCstate = 0x30;
        break;

      case V_CSTATE_LIMIT_C7:
        SubStates = MonitorMwaitEdx.Bits.C4States;
        MaxCstate = 0x30;
        break;

      case V_CSTATE_LIMIT_C6:
        SubStates = MonitorMwaitEdx.Bits.C3States;
        MaxCstate = 0x20;
        break;

      case V_CSTATE_LIMIT_C3:
        SubStates = MonitorMwaitEdx.Bits.C2States;
        MaxCstate = 0x10;
        break;

      case V_CSTATE_LIMIT_C1:
        SubStates = MonitorMwaitEdx.Bits.C1States;
        MaxCstate = 0x00;
        break;

      default:
        break;
    }
  }

  ///
  /// If Substates opcode is greater than 1 than add that info to the MaxCstate Opcode.
  ///
  if (SubStates > 1) {
    MaxCstate |= (UINT8) SubStates - 1;
  }

  ///
  /// Update Target C State EAX[7:4] value alone in PcdCpuApTargetCstate
  ///
  PcdSet8S (PcdCpuApTargetCstate, MaxCstate >> 4);
}

/**
  CpuInitOnReadyToLockCallback - Callback will be triggered when OnReadyToLock event is signaled
   - Create SMBIOS Table type - FviSmbiosType
   - Drop into SMM to register IOTRAP for BIOS Guard tools interface

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CpuInitOnReadyToLockCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  BIOSGUARD_HOB                 *BiosGuardHobPtr;
  MSR_BIOS_INFO_FLAGS_REGISTER  MsrBiosInfoFlags;
  EFI_STATUS                    Status;
  VOID                          *DxeSmmReadyToLock;

  ///
  /// Account for the initial call from EfiCreateProtocolNotifyEvent
  ///
  Status = gBS->LocateProtocol (
                  &gEfiDxeSmmReadyToLockProtocolGuid,
                  NULL,
                  &DxeSmmReadyToLock
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  ///
  /// Close the event
  ///
  gBS->CloseEvent (Event);

  ///
  /// Enable BIOS Guard Flash Wear-Out Protection mitigation.
  ///
  BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (BiosGuardHobPtr != NULL) {
    if (BiosGuardHobPtr->Bgpdt.BiosGuardAttr & EnumFlashwearoutProtection) {
      MsrBiosInfoFlags.Uint64 = AsmReadMsr64 (MSR_BIOS_INFO_FLAGS);
      MsrBiosInfoFlags.Bits.BiosInfoFlagsData = 1;
      AsmWriteMsr64 (MSR_BIOS_INFO_FLAGS, MsrBiosInfoFlags.Uint64);
    }
  }

  return;
}

/**
  CpuNvsAreaInitCallback - Initialize some fields of CpuNvsArea.

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
VOID
EFIAPI
CpuNvsAreaInitCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                    Status;
  CPU_NVS_AREA_PROTOCOL         *CpuNvsAreaProtocol;
  UINTN                         Index;
  UINTN                         Index2;

  Status = gBS->LocateProtocol (
                  &gCpuNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &CpuNvsAreaProtocol
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  CpuNvsAreaProtocol->Area->HeteroStatus = IsHeteroCoreSupported ();

  for (Index = 0; Index < mNumberOfProcessors; Index++) {
    CpuNvsAreaProtocol->Area->DomainIndex [Index] = mDieModuleIndex [Index];
    CpuNvsAreaProtocol->Area->CoreCountInDomain [Index] = 0;
    for (Index2 = 0; Index2 < mNumberOfEnabledProcessors; Index2 ++) {
      if (mDieModuleIndex[Index] == mDieModuleIndex[Index2]) {
        CpuNvsAreaProtocol->Area->CoreCountInDomain [Index] ++;
      }
    }
    DEBUG ((DEBUG_INFO, "Processor - 0x%x  DomainIndex - 0x%x  CoreCountInDomain - 0x%x \n",
           Index,
           CpuNvsAreaProtocol->Area->DomainIndex[Index],
           CpuNvsAreaProtocol->Area->CoreCountInDomain[Index]));
  }

  gBS->CloseEvent (Event);

  return;
}

/**
  Collect Cpu Cache Infomation

  @param[in] CPU_INFO_PROTOCOL *CpuInfo - Pointer to the CPU_INFO_PROTOCOL Structure

  @retval EFI_SUCCESS           - Collect information Successfully.
  @retval EFI_NOT_FOUND         - Specified HOB can not be found.
  @retval EFI_INVALID_PARAMETER - Input buffer is invalid.
**/
EFI_STATUS
CollectCpuCacheInfo (
  IN CPU_INFO_PROTOCOL    *CpuInfo
  )
{
  EFI_HOB_GUID_TYPE       *CacheInfoHob;
  CPU_CACHE_INFO          *CacheInfoData;
  UINTN                   CacheInfoDataSize;
  UINT8                   CoreType;
  UINT8                   CacheCount;
  UINT8                   SmallCoreCacheCount;
  UINT8                   BigCoreCacheCount;
  UINT8                   Index;

  CacheInfoHob = GetFirstGuidHob (&gCpuCacheInfoHobGuid);
  if (CacheInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU Cache info HOB not available\n"));
    return EFI_NOT_FOUND;
  }

  CacheInfoData     = GET_GUID_HOB_DATA (CacheInfoHob);
  CacheInfoDataSize = GET_GUID_HOB_DATA_SIZE (CacheInfoHob);

  CoreType = 0;
  CacheCount = 0;
  SmallCoreCacheCount = 0;
  BigCoreCacheCount = 0;

  for (Index = 0; Index < CacheInfoDataSize / sizeof (*CacheInfoData); Index++) {
    switch (CacheInfoData[Index].CoreType) {
      case CORE_TYPE_NON_HYBRID:
        if (mNumBigCore != 0 && mNumSmallCore == 0) {
          CoreType = BIG_CORE;
          CacheCount = BigCoreCacheCount;
          BigCoreCacheCount++;
        } else if (mNumBigCore == 0 && mNumSmallCore != 0) {
          CoreType = SMALL_CORE;
          CacheCount = SmallCoreCacheCount;
          SmallCoreCacheCount ++;
        } else {
          ASSERT (FALSE);
        }
        break;

      case CPUID_CORE_TYPE_INTEL_ATOM:
        CoreType = SMALL_CORE;
        CacheCount = SmallCoreCacheCount;
        SmallCoreCacheCount++;
        break;

      case CPUID_CORE_TYPE_INTEL_CORE:
        CoreType = BIG_CORE;
        CacheCount = BigCoreCacheCount;
        BigCoreCacheCount++;
        break;

      default:
        DEBUG ((DEBUG_ERROR, "CPU Core type not available\n"));
        ASSERT (FALSE);
        break;
    }

    CpuInfo->CpuInfo[CoreType].CacheInfo[CacheCount].Type          = (UINT8) CacheInfoData[Index].CacheType;
    CpuInfo->CpuInfo[CoreType].CacheInfo[CacheCount].Level         = (UINT8) CacheInfoData[Index].CacheLevel;
    CpuInfo->CpuInfo[CoreType].CacheInfo[CacheCount].Associativity = CacheInfoData[Index].CacheWays + 1;
    CpuInfo->CpuInfo[CoreType].CacheInfo[CacheCount].Size          = CacheInfoData[Index].CacheSizeinKB * CacheInfoData[Index].CacheCount;
  }

  //
  // Subtract 1 because there are 2 counts for L1 (Data and Code).
  //
  if (SmallCoreCacheCount > 0) {
    CpuInfo->CpuInfo[SMALL_CORE].MaxCacheSupported = SmallCoreCacheCount - 1;
  }
  if (BigCoreCacheCount > 0) {
    CpuInfo->CpuInfo[BIG_CORE].MaxCacheSupported = BigCoreCacheCount - 1;
  }

  return EFI_SUCCESS;
}

/**
  Collect Core Type Cpu Infomation

  @param[in] VOID *Buffer      - Pointer to the CORE_INFORMATION Structure

  @retval EFI_SUCCESS           - Successfully prepared.
  @retval EFI_INVALID_PARAMETER - Input Buffer is invalid.
**/
EFI_STATUS
CollectCoreTypeCpuInfo (
  IN VOID       *Buffer
  )
{
  CPU_INFO                              *CoreInfo;
  CHAR8                                 *BrandString;
  UINT32                                CpuSignature;
  CPUID_BRAND_STRING_DATA               BrandStringEax;
  CPUID_BRAND_STRING_DATA               BrandStringEbx;
  CPUID_BRAND_STRING_DATA               BrandStringEcx;
  CPUID_BRAND_STRING_DATA               BrandStringEdx;
  VOID                                  *Hob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;

  CoreInfo = (CPU_INFO *) Buffer;
  BrandString = CoreInfo->BrandString;

  ///
  /// Get Brand string
  ///
  AsmCpuid (CPUID_BRAND_STRING1, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
  *(UINT32*) BrandString = BrandStringEax.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEbx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEcx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEdx.Uint32; BrandString += 4;

  AsmCpuid (CPUID_BRAND_STRING2, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
  *(UINT32*) BrandString = BrandStringEax.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEbx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEcx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEdx.Uint32; BrandString += 4;

  AsmCpuid (CPUID_BRAND_STRING3, &BrandStringEax.Uint32, &BrandStringEbx.Uint32, &BrandStringEcx.Uint32, &BrandStringEdx.Uint32);
  *(UINT32*) BrandString = BrandStringEax.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEbx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEcx.Uint32; BrandString += 4;
  *(UINT32*) BrandString = BrandStringEdx.Uint32; BrandString += 4;
  *BrandString = '\0';

  ///
  /// Remove leading spaces.
  ///
  BrandString = CoreInfo->BrandString;
  while (*BrandString == ' ') {
    ++BrandString;
  }
  AsciiStrCpyS (CoreInfo->BrandString, sizeof (CoreInfo->BrandString), BrandString);

  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU power management private data hob not available\n"));
    return EFI_NOT_FOUND;
  }

  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);

  ///
  /// Gather CPU info
  ///
  AsmCpuid (CPUID_VERSION_INFO, &CpuSignature, NULL, NULL, NULL);
  CoreInfo->CpuSignature               = CpuSignature;
  if (CpuPowerMgmtPrivateDataHob->CtdpLevelsSupported > 1) {
    CoreInfo->IntendedFreq             = MsrGetSelectCtdpRatio (CpuPowerMgmtPrivateDataHob->ConfigTdpBootModeIndex) * 100;
  } else {
    CoreInfo->IntendedFreq             = GetMaxNonTurboRatio () * 100;
  }

  return EFI_SUCCESS;
}

/**
  Update the Core Infomation for Small Core and Big Core
  This is executed across all threads
**/
VOID
UpdateCoreInformation (
  VOID
  )
{
  EFI_STATUS                   Status;
  UINT8                        CoreType;
  UINT8                        DieModuleIndex;
  UINTN                        ApNumber;
  UINT32                       ThreadIndex;
  UINT32                       ApicId;
  UINT32                       Index;
  UINT32                       MaxStandardCpuIdIndex;
  CPUID_EXTENDED_TOPOLOGY_EAX  ExtendedTopologyEax;
  CPUID_EXTENDED_TOPOLOGY_ECX  ExtendedTopologyEcx;
  UINTN                        LevelType;
  UINTN                        LastLevelType;
  UINTN                        ShiftBitsTmp;
  UINTN                        ShiftBits;

  Status = mMpService->WhoAmI (mMpService, &ApNumber);
  ASSERT_EFI_ERROR (Status);

  CoreType = 0;
  GetCoreType (&CoreType);
  ApicId = GetApicId ();

  //
  // For XAPIC case or CPU 0x1f not supports Core and module level, use legacy way to get Module ID
  //
  ShiftBits = 3;

  //
  // Get max index of CPUID
  //
  AsmCpuid (CPUID_SIGNATURE, &MaxStandardCpuIdIndex, NULL, NULL, NULL);

  if ((GetApicMode () == LOCAL_APIC_MODE_X2APIC) && (MaxStandardCpuIdIndex >= CPUID_V2_EXTENDED_TOPOLOGY)) {
    ShiftBitsTmp = 0;
    LastLevelType = CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID;
    for (Index = 0; ; Index++) {
      AsmCpuidEx (
        CPUID_V2_EXTENDED_TOPOLOGY,
        Index,
        &ExtendedTopologyEax.Uint32,
        NULL,
        &ExtendedTopologyEcx.Uint32,
        NULL
        );

      LevelType = ExtendedTopologyEcx.Bits.LevelType;
      if (LevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_INVALID) {
        break;
      }
      if ((LevelType == CPUID_V2_EXTENDED_TOPOLOGY_LEVEL_TYPE_MODULE) &&
          (LastLevelType == CPUID_EXTENDED_TOPOLOGY_LEVEL_TYPE_CORE)) {
        ShiftBits = ShiftBitsTmp;
        break;
      }
      ShiftBitsTmp  = ExtendedTopologyEax.Bits.ApicIdShift;
      LastLevelType = LevelType;
    }
  }

  DieModuleIndex = (UINT8)(ApicId >> ShiftBits);
  //
  // Now we assume DieModuleIndex can be present in 8 bits.
  //
  ASSERT ((UINT32)DieModuleIndex == (ApicId >> ShiftBits));

  mDieModuleIndex [ApNumber] = DieModuleIndex;

  if (CoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
    if (mNumSmallCore == 0) {
      CollectCoreTypeCpuInfo (&mCpuInfo.CpuInfo[SMALL_CORE]);
    }
    mSmallCoreThreads ++;
    GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
    if (ThreadIndex == 0) {
      mNumSmallCore ++;
    }
  } else if (CoreType == CPUID_CORE_TYPE_INTEL_CORE) {
    if (mNumBigCore == 0) {
      CollectCoreTypeCpuInfo (&mCpuInfo.CpuInfo[BIG_CORE]);
    }
    mBigCoreThreads ++;
    GetProcessorLocationByApicId (GetInitialApicId (), NULL, NULL, &ThreadIndex);
    if (ThreadIndex == 0) {
      mNumBigCore ++;
    }
    //
    // If detected Core is BIG, then respective BIT position will be SET.
    //
    mCoreTypeBitMap |= (1 << ApNumber);
  }
}

/**
  Initialize CPU info.

  @retval EFI_SUCCESS          - Successfully prepared.
  @retval EFI_OUT_OF_RESOURCES - Not enough memory to complete the function.
**/
EFI_STATUS
InitCpuInfo (
  VOID
  )
{
  EFI_STATUS                    Status;
  EFI_HANDLE                    Handle;
  VOID                          *Registration;
  EFI_EVENT                     Event;

  Handle = NULL;

  ZeroMem (&mCpuInfo, sizeof (mCpuInfo));

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &mMpService
                  );
  ASSERT_EFI_ERROR (Status);

  Status = mMpService->GetNumberOfProcessors (mMpService, &mNumberOfProcessors, &mNumberOfEnabledProcessors);
  DEBUG ((DEBUG_INFO, "Total num of Processors - 0x%x. Enabled Processors - 0x%x\n", mNumberOfProcessors, mNumberOfEnabledProcessors));
  mDieModuleIndex = AllocateZeroPool (mNumberOfProcessors);
  if (mDieModuleIndex == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  UpdateCoreInformation ();
  mMpService->StartupAllAPs (
                mMpService,         // This
                (EFI_AP_PROCEDURE) UpdateCoreInformation, // Procedure
                TRUE,               // SingleThread
                NULL,               // WaitEvent
                0,                  // TimeoutInMicrosecsond
                NULL,               // ProcedureArgument
                NULL                // FailedCpuList
                );
  DEBUG ((DEBUG_INFO, "Total Enabled Big Core - 0x%x, Total Enabled Small Core - 0x%x\n", mNumBigCore, mNumSmallCore));
  DEBUG ((DEBUG_INFO, "Total Enabled Big Core Threads - 0x%x, Total Enabled Small Core Threads- 0x%x\n",mBigCoreThreads, mSmallCoreThreads));

  CollectCpuCacheInfo (&mCpuInfo);

  if (mNumSmallCore != 0) {
    mCpuInfo.CpuInfo[SMALL_CORE].NumHts   = mSmallCoreThreads / mNumSmallCore;
    mCpuInfo.CpuInfo[SMALL_CORE].NumCores = mNumSmallCore;
  }
  else {
    mCpuInfo.CpuInfo[SMALL_CORE].NumHts   = 0;
    mCpuInfo.CpuInfo[SMALL_CORE].NumCores = 0;
  }

  if (mNumBigCore != 0) {
    mCpuInfo.CpuInfo[BIG_CORE].NumHts     = mBigCoreThreads / mNumBigCore;
    mCpuInfo.CpuInfo[BIG_CORE].NumCores   = mNumBigCore;
  }
  else {
    mCpuInfo.CpuInfo[BIG_CORE].NumHts     = 0;
    mCpuInfo.CpuInfo[BIG_CORE].NumCores   = 0;
  }

  mCpuInfo.Revision                       = CPU_INFO_PROTOCOL_REVISION;

  ///
  /// Install CPU info protocol
  ///
  gBS->InstallMultipleProtocolInterfaces (
         &Handle,
         &gCpuInfoProtocolGuid,
         &mCpuInfo,
         NULL
         );

  Event = EfiCreateProtocolNotifyEvent (
            &gCpuNvsAreaProtocolGuid,
            TPL_CALLBACK,
            CpuNvsAreaInitCallback,
            NULL,
            &Registration
            );
  ASSERT (Event != NULL);

  return EFI_SUCCESS;
}


/**
  Initialize the state information for the CPU Architectural Protocol

  @param[in] ImageHandle - Image handle of the loaded driver
  @param[in] SystemTable - Pointer to the System Table

  @retval EFI_SUCCESS           - Thread was successfully created
  @retval EFI_OUT_OF_RESOURCES  - Can not allocate protocol data structure
  @retval EFI_DEVICE_ERROR      - Can not create the thread
  @retval EFI_NOT_FOUND         - Can not locate CPU Data HOB
**/
EFI_STATUS
EFIAPI
InitializeCpu (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_EVENT                             ExitBootServicesEvent;
  VOID                                  *Registration;
#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
  BIOSGUARD_HOB                         *BiosGuardHobPtr;
#endif

  ///
  /// Initialize DxeCpuInfo protocol instance and gather CPU information
  ///
  Status = InitCpuInfo ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to initialize DxeCpuInfo\n"));
  }

  ///
  /// Create an OnReadyToLock protocol callback event for BIOS Guard.
  /// This function causes CpuInitOnReadyToLockCallback() to be executed,
  /// Ensure CpuInitOnReadyToLockCallback() accounts for this initial call.
  ///
  EfiCreateProtocolNotifyEvent (
    &gEfiDxeSmmReadyToLockProtocolGuid,
    TPL_CALLBACK,
    CpuInitOnReadyToLockCallback,
    NULL,
    &Registration
    );

#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
  ///
  /// Get Bios Guard Config Hob
  ///
  BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (BiosGuardHobPtr != NULL) {
    ///
    /// Determine if BGUP is to be placed in TSEG
    /// If the size allocated to Bios Guard in DPR is 0, BGUP will be stored in TSEG
    /// Otherwise, BGUP will use the memory allocated within DPR and
    /// will initialize the Bios Guard NVS related variables
    ///
    DEBUG ((DEBUG_INFO, "\n BiosGuardMemSize :%x \n",BiosGuardHobPtr->BiosGuardMemSize));
    if (BiosGuardHobPtr->BiosGuardMemSize != 0) {
      BiosGuardAcpiInit (ImageHandle);
    }
  } else {
    DEBUG ((DEBUG_ERROR, "Bios Guard Config HOB not available, Bios Guard NVS variables not initialized\n"));
  }
#endif

  //
  // Set PCD to request deepest C-states in monitor/mwait right before boot.
  // Use TPL_NOTIFY signal to trigger callback before TPL_CALLBACK signal used by
  // UefiCpuPkg to put APs in deep sleep before giving control to OS.
  //

  Status = gBS->CreateEvent (
                  EVT_SIGNAL_EXIT_BOOT_SERVICES,
                  TPL_NOTIFY,
                  RequestDeepestCStateForAps,
                  NULL,
                  &ExitBootServicesEvent
                  );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Install CPU die info protocol
  ///
  Status = CpuInstallDieInfo ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}
