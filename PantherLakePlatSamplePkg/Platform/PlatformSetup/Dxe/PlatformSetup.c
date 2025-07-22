/** @file
  Platform Policy Initialization Driver

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

@par Specification Reference:
**/

#include <Library/S3BootScriptLib.h>
#include <Library/PchInfoLib.h>
#include <PlatformSetup.h>
#include <OemSetup.h>
#include <Library/MemoryAllocationLib.h>
#include <PlatformBoardConfig.h>
#include <Library/TseInfoLib.h>
#include <Library/LocalApicLib.h>
#include <Register/GenerationMsr.h>
#include <Library/TmeInfoLib.h>
#include <Library/DgrInfoLib.h>
#include <Library/TdxInfoLib.h>
#if FixedPcdGetBool (PcdOverclockEnable) == 1
#include <OcLimits.h>
#endif

//
// Global variables
//
GLOBAL_REMOVE_IF_UNREFERENCED CPU_SETUP                       mCpuSetup;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_PCI_ROOT_BRIDGE_IO_PROTOCOL *mPciRootBridgeIo;
GLOBAL_REMOVE_IF_UNREFERENCED CHAR16                          HexDigit[17] = L"0123456789ABCDEF";

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PS2_POLICY_PROTOCOL         mPs2PolicyData = {
  EFI_KEYBOARD_NUMLOCK,
  (EFI_PS2_INIT_HARDWARE) Ps2InitHardware
};

#if FixedPcdGetBool (PcdOverclockEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                           mBigCoreCount;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                           mSmallCoreCLusterCount;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                           mAtomClusterFlag[MAX_OC_ATOM_CLUSTERS_ID] = {0};
#endif

#define V_BIG_CORE_TYPE             0x40
#define V_ATOM_CORE_TYPE            0x20

/**
  CPUID Native Model ID Information

  @param   EAX  CPUID_NATIVE_MODEL_ID_INFO (0x1A)

  @retval  EAX  Value of bits [23:0] gives Native Model ID
                Value of bits [31:24] gives Core Type. 0x40 - Core, 0x20 - Atom
  @retval  EBX  Reserved.
  @retval  ECX  Reserved.
  @retval  EDX  Reserved.

  <b>Example usage</b>
  @code
  UINT32  Eax;

  AsmCpuid (CPUID_NATIVE_MODEL_ID_INFO, &Eax, NULL, NULL, NULL);
  @endcode
**/
#define CPUID_NATIVE_MODEL_ID_INFO              0x1A

/**
  CPUID Native Model ID Information returned in EAX for CPUID leaf
  #CPUID_NATIVE_MODEL_ID_INFO.
**/
typedef union {
  ///
  /// Individual bit fields
  ///
  struct {
    UINT32  NativeModelId:24;        ///< [Bits   23:0] Native Model ID
    UINT32  CoreType:8;              ///< [Bits   31:24] Core Type
  } Bits;
  ///
  /// All bit fields as a 32-bit value
  ///
  UINT32    Uint32;
} CPUID_NATIVE_MODEL_ID_INFO_EAX;

GLOBAL_REMOVE_IF_UNREFERENCED SETUP_VOLATILE_DATA   mSetupVolatileData = {0};
GLOBAL_REMOVE_IF_UNREFERENCED UINTN                 mNumberOfCPUs;

//
// Functions
//
/**
  Get the cpu signature.

  @retval Cpu Signature

  @todo remove and use CpuPlatformLib
**/
UINT32 GetCpuSignature (
  VOID
  )
{
  CPUID_VERSION_INFO_EAX  CpuidVersionInfoEax;
  AsmCpuid (
          CPUID_VERSION_INFO,
          &CpuidVersionInfoEax.Uint32,
          NULL,
          NULL,
          NULL
          );

  return CpuidVersionInfoEax.Uint32;
}

VOID
HexToString (
  CHAR16  *String,
  UINTN   Value,
  UINTN   Digits
  )
{
  for (; Digits > 0; Digits--, String++) {
    *String = HexDigit[((Value >> (4*(Digits-1))) & 0x0f)];
  }
}

/**
  Displays BIST result on Console.

  @param[in] Core         Number of Code
  @param[in] Thread       Number of Code
  @param[in] Eax          Value of Register Eax
**/
VOID
DisplaySelfTestBistResult (
  IN UINT32 Core,
  IN UINT32 Thread,
  IN UINT32 Eax
  )
{
  CHAR16                    String[20];
  CHAR16                    StrBuffer[10];

  gST->ConOut->OutputString (gST->ConOut, L"*******************BIST FAILED*****************\r\n");

  //
  // Display  Core detail
  //
  StrCpyS (String, sizeof (String) / sizeof (CHAR16), L"CORE = 0x");
  HexToString(StrBuffer, Core, 2);
  StrCpyS (String + StrLen(String), sizeof (String) / sizeof (CHAR16) - StrLen (String), StrBuffer);
  gST->ConOut->OutputString (gST->ConOut,String);

  //
  // Display  Thread detail
  //
  StrCpyS (String, sizeof (String) / sizeof (CHAR16), L" Thread = 0x");
  HexToString(StrBuffer, Thread, 2);
  StrCpyS (String + StrLen(String), sizeof (String) / sizeof (CHAR16) - StrLen (String), StrBuffer);
  gST->ConOut->OutputString (gST->ConOut,String);

  //
  // Display  Eax detail
  //
  StrCpyS (String, sizeof (String) / sizeof (CHAR16), L" EAX = 0x");
  HexToString(StrBuffer, Eax, 4);
  StrCpyS (String + StrLen(String), sizeof (String) / sizeof (CHAR16) - StrLen (String), StrBuffer);
  gST->ConOut->OutputString (gST->ConOut,String);
  gST->ConOut->OutputString (gST->ConOut,L"\r\n");
}

#if FixedPcdGetBool (PcdOverclockEnable) == 1
/**
  This function gets number of different cores enabled in the platform.

**/
STATIC
VOID
GetNumberOfHybridCores (
  VOID
  )
{
  CPUID_NATIVE_MODEL_ID_INFO_EAX     Eax;
  UINT32                             ThreadIndex;
  UINT32                             ModuleIndex;

  //
  // GetProcessorLocation2ByApicId(InitialApicId,Package,Die,Tile,Module,Core,Thread)
  //
  GetProcessorLocation2ByApicId (GetInitialApicId (), NULL, NULL, NULL, &ModuleIndex, NULL, &ThreadIndex);
  ASSERT (ModuleIndex <= MAX_OC_ATOM_CLUSTERS_ID);

  if (ThreadIndex != 0) {
    return;
  }

  //
  // Check which is the running core by reading CPUID.(EAX=1AH, ECX=00H):EAX
  //
  AsmCpuid (CPUID_NATIVE_MODEL_ID_INFO, &Eax.Uint32, NULL, NULL, NULL);

  if ((UINT8) Eax.Bits.CoreType == V_BIG_CORE_TYPE) {
    mBigCoreCount++;
  } else {
    if (mAtomClusterFlag[ModuleIndex] == 0) {
      mAtomClusterFlag[ModuleIndex] = 1;
      mSmallCoreCLusterCount++;
    }
  }
}
#endif
/**
  Self Test BIST result computed.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
SelfTestBistErrMsg (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  EFI_STATUS                          Status;
  EFI_PROCESSOR_INFORMATION           MpContext;
  EFI_MP_SERVICES_PROTOCOL            *MpServices;
  UINT32                              Index;
  BOOLEAN                             BistFailed = FALSE;

  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpServices
                  );
  ASSERT_EFI_ERROR (Status);

  gST->ConOut->ClearScreen (gST->ConOut);

  for (Index = 0; Index < mNumberOfCPUs; Index++) {
    Status = MpServices->GetProcessorInfo (MpServices, Index, &MpContext);
    ASSERT_EFI_ERROR (Status);
    if (MpContext.ProcessorId > 255) {
      break;
    }
    if ((MpContext.StatusFlag & PROCESSOR_HEALTH_STATUS_BIT) == 0) {
      BistFailed = TRUE;
      DEBUG ((DEBUG_ERROR, "BIST FAILED CORE=%x THREAD=%x EAX=%x\n", (UINT32)MpContext.Location.Core, (UINT32)MpContext.Location.Thread, MpContext.StatusFlag));
      DisplaySelfTestBistResult(
                   (UINT32)MpContext.Location.Core,
                   (UINT32)MpContext.Location.Thread,
                   MpContext.StatusFlag
                   );
    }
  }

  if (BistFailed) {
    DEBUG ((DEBUG_ERROR, "BIST Failed!!\n"));
    CpuDeadLoop();
  } else {
    DEBUG ((DEBUG_INFO, "BIST passed\n"));
  }
  gBS->CloseEvent(Event);

}

VOID
EFIAPI
CpuSetupInitCallback (
  IN EFI_EVENT     Event,
  IN VOID          *Context
  )
{
  EFI_STATUS                                   Status;
  SETUP_CPU_FEATURES                           SetupCpuFeatures;
  UINTN                                        Index;
  EFI_PROCESSOR_INFORMATION                    MpContext;
  VOID                                         *VariableWrite;
  EFI_EVENT                                    ReadyToBootEvent;
  EFI_MP_SERVICES_PROTOCOL                     *MpServices;
  UINTN                                        NumberOfEnabledCPUs;
  UINT32                                       SetupCpuFeaturesAttr;
  UINTN                                        SetupCpuFeaturesSize;
  CPUID_MONITOR_MWAIT_EDX                      MonitorMwaitEdx;
  CPUID_VERSION_INFO_ECX                       CpuIdVersionInfoEcx;


  SetupCpuFeaturesAttr = 0;
  SetupCpuFeaturesSize = 0;
  Status = gBS->LocateProtocol (
                  &gEfiMpServiceProtocolGuid,
                  NULL,
                  (VOID **) &MpServices
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  Status = gBS->LocateProtocol (
                  &gEfiVariableWriteArchProtocolGuid,
                  NULL,
                  &VariableWrite
                  );
  if (EFI_ERROR(Status)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Running CpuSetupInitCallback...\n"));

  GetSupportedCpuFeatures ((UINT32 *) &CpuIdVersionInfoEcx);

  //
  // Only show as supported if all are supported.
  //
  ZeroMem(&SetupCpuFeatures, sizeof(SetupCpuFeatures));
  SetupCpuFeaturesSize = sizeof (SETUP_CPU_FEATURES);
  Status = gRT->GetVariable (
                  L"SetupCpuFeatures",
                  &gSetupVariableGuid,
                  &SetupCpuFeaturesAttr,
                  &SetupCpuFeaturesSize,
                  &SetupCpuFeatures
                  );

  if (EFI_ERROR (Status)) {
    SetupCpuFeaturesAttr = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  }
  SetupCpuFeatures.TurboModeAvailable = (UINT8) IsTurboModeSupported ();
  SetupCpuFeatures.XETdcTdpLimitAvailable = (UINT8) IsXETdcTdpLimitSupported ();
  SetupCpuFeatures.CoreRatioLimitAvailable = (UINT8) IsCoreRatioLimitSupported ();
  SetupCpuFeatures.ConfigTdpAvailable = (UINT8) IsConfigTdpSupported ();
  SetupCpuFeatures.LpmAvailable = (UINT8) IsLpmSupported ();
  SetupCpuFeatures.VTAvailable = (UINT8) CpuIdVersionInfoEcx.Bits.VMX;        // if VMX =1 , VT enable
  SetupCpuFeatures.AESAvailable = (UINT8) CpuIdVersionInfoEcx.Bits.AESNI;     // if AESNI =1, AES supported
  SetupCpuFeatures.MultiCoreAvailable = 1;
  SetupCpuFeatures.MultiSocketAvailable = 0;
  SetupCpuFeatures.MultiSocketPopulated = 0;
  SetupCpuFeatures.LocalX2ApicAvailable = (UINT8) CpuIdVersionInfoEcx.Bits.x2APIC;    // If X2APIC=1 , X2APIC supported
  SetupCpuFeatures.EnergyEfficientTurboAvailable = (UINT8) IsEnergyEfficientTurboSupported ();
  SetupCpuFeatures.TimedMwaitSupported = (UINT8) IsTimedMwaitSupported ();
  SetupCpuFeatures.HwpAvailable = (UINT8) IsHwpSupported ();
  GetConfigTdpLevelsSupported(&SetupCpuFeatures.CtdpLevel1Supported, &SetupCpuFeatures.CtdpLevel2Supported);
  SetupCpuFeatures.PpinFeatureAvailable = IsPpinFeatureAvailable ();
  DEBUG((DEBUG_INFO, "SetupCpuFeatures.PpinFeatureAvailable = 0x%x \n", SetupCpuFeatures.PpinFeatureAvailable));

  ///
  /// Resource Priority feature is supported only on LNL and later.
  /// with CPUID.(EAX=7):EBX[15] = 1
  ///
  SetupCpuFeatures.RpSupported = (UINT8) IsResourcePriorityFeatureSupported ();

  ///
  /// Since current platform doesn't support
  ///
  SetupCpuFeatures.FclkSupported = 0;

  SetupCpuFeatures.ApicIdFlag = 0;

  Status = MpServices->GetNumberOfProcessors (MpServices, &mNumberOfCPUs, &NumberOfEnabledCPUs);
  ASSERT_EFI_ERROR (Status);

  for (Index = 0; Index < mNumberOfCPUs; Index++) {
    Status = MpServices->GetProcessorInfo (MpServices, Index, &MpContext);
    ASSERT_EFI_ERROR (Status);
    if (MpContext.ProcessorId > 255) {
      SetupCpuFeatures.ApicIdFlag = 1;
      break;
    }
  }

  //
  // Setup For CPU PPM
  //
  SetupCpuFeatures.CxAvailable = 0;
  SetupCpuFeatures.C1Available = 0;
  SetupCpuFeatures.C3Available = 0;
  SetupCpuFeatures.C6Available = 0;
  SetupCpuFeatures.C7Available = 0;
  SetupCpuFeatures.C8Available = 0;
  SetupCpuFeatures.C9Available = 0;
  SetupCpuFeatures.C10Available = 0;

  if (CpuIdVersionInfoEcx.Bits.MONITOR) {
    GetSubCStateSupported (NULL, (UINT32 *)&MonitorMwaitEdx);
    //
    // Don't use mwait for now to halt CPU in BIOS.
    //
    //if (RegEcx & 0x01) {
    //  UseMwait = TRUE;
    //}
    if (MonitorMwaitEdx.Bits.C1States) {
      SetupCpuFeatures.C1Available = 1;
    }
    if (MonitorMwaitEdx.Bits.C2States) {
      SetupCpuFeatures.C3Available = 1;
    }
    if (MonitorMwaitEdx.Bits.C3States) {
      SetupCpuFeatures.C6Available = 1;
    }
    if (MonitorMwaitEdx.Bits.C4States) {
      SetupCpuFeatures.C7Available = 1;
    }
     if (PcdGet8 (PcdPlatformType) == TypeUltUlx) {
      if (MonitorMwaitEdx.Bits.C5States) {
        SetupCpuFeatures.C8Available = 1;
      }
      if (MonitorMwaitEdx.Bits.C6States) {
        SetupCpuFeatures.C9Available = 1;
      }
      if (MonitorMwaitEdx.Bits.C7States) {
        SetupCpuFeatures.C10Available = 1;
      }
    }
  }

  //
  // This allows the setup question to limit package C-states.
  // Enable after further testing.
  //
  SetupCpuFeatures.CxAvailable =
      SetupCpuFeatures.C1Available |
      SetupCpuFeatures.C3Available |
      SetupCpuFeatures.C6Available |
      SetupCpuFeatures.C7Available;

  //
  // Checking TXT capability after all CPU initialized
  //
  SetupCpuFeatures.TXTAvailable = (UINT8) CpuIdVersionInfoEcx.Bits.SMX;    //if SMX= 1, SMX supported

  SetupCpuFeatures.EISTAvailable = (UINT8) CpuIdVersionInfoEcx.Bits.EIST;

#if FixedPcdGetBool (PcdOverclockEnable) == 1
  //
  // Getting Number of Big Cores & Small Cores.
  //
  mBigCoreCount          = 0;
  mSmallCoreCLusterCount = 0;
  ZeroMem (&mAtomClusterFlag, sizeof (mAtomClusterFlag));
  GetNumberOfHybridCores ();
  Status = MpServices->StartupAllAPs (
                        MpServices,
                        (EFI_AP_PROCEDURE) GetNumberOfHybridCores,
                        TRUE,
                        NULL,
                        0,
                        NULL,
                        NULL
                        );
  if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
    DEBUG ((DEBUG_ERROR, "GetNumberOfHybridCores: StartupAllAps(): %r\n", Status));
  }

  SetupCpuFeatures.NumCores             = mBigCoreCount;
  SetupCpuFeatures.NumSmallCoresCLuster = mSmallCoreCLusterCount;
  DEBUG ((DEBUG_INFO, "Number of cores enabled in platform: BigCores = %d mSmallCoreCLusterCount = %d\n", mBigCoreCount, mSmallCoreCLusterCount));
#endif

  ///
  /// Debug interface is supported if CPUID (EAX=1): ECX[11] = 1
  ///
  SetupCpuFeatures.DebugInterfaceSupported = (UINT8) CpuIdVersionInfoEcx.Bits.SDBG;

  ///
  /// set DLVR Phase SSC support
  ///
  SetupCpuFeatures.DlvrPhaseSscSupported = IsDlvrPhaseSscSupported ();
  ///
  /// Processor Trace feature is supported only on BDW and later,
  /// with CPUID.(EAX=7):EBX[25] = 1
  ///
  SetupCpuFeatures.ProcTraceSupported = (UINT8) IsIntelProcessorTraceSupported ();
  ///
  /// Set TSE supported
  ///
  SetupCpuFeatures.TseSupported = (UINT8) IsTseSupported ();
  ///
  /// Set TME supported
  ///
  SetupCpuFeatures.TmeSupported = (UINT8) IsMkTmeSupported ();

  ///
  /// Set TME Bypass Supported
  ///
  SetupCpuFeatures.TmeBypassSupported = (UINT8) IsTmeBypassSupported ();

  ///
  /// Set TDX supported
  ///
  SetupCpuFeatures.TdxSupported = (UINT8) IsTdxSupported ();

  Status = gRT->SetVariable (
                  L"SetupCpuFeatures",
                  &gSetupVariableGuid,
                  SetupCpuFeaturesAttr,
                  sizeof (SETUP_CPU_FEATURES),
                  &SetupCpuFeatures
                  );
  ASSERT_EFI_ERROR(Status);

  //
  // Display Bist information to the user if Bist is enabled in Setup
  //
  if (mCpuSetup.BistOnReset) {
    //
    // Create an Ready to Boot event.
    //
    Status = EfiCreateEventReadyToBootEx (
               TPL_CALLBACK,
               SelfTestBistErrMsg,
               NULL,
               &ReadyToBootEvent
               );
    ASSERT_EFI_ERROR (Status);
  }

  gBS->CloseEvent (Event);
}

EFI_STATUS
CpuSetupInit (
  VOID
  )
{
  VOID                    *Registration1;
  VOID                    *Registration2;

  EfiCreateProtocolNotifyEvent (
    &gEfiMpServiceProtocolGuid,
    TPL_CALLBACK,
    CpuSetupInitCallback,
    NULL,
    &Registration1
    );

  EfiCreateProtocolNotifyEvent (
    &gEfiVariableWriteArchProtocolGuid,
    TPL_CALLBACK,
    CpuSetupInitCallback,
    NULL,
    &Registration2
    );

  return EFI_SUCCESS;
}

VOID
EFIAPI
PlatformPcdInit (
  VOID
  )
{
  UINTN                 VarSize;
  EFI_STATUS            Status;
  CPU_SETUP             CpuSetup;
  SETUP_DATA            SetupData;

  VarSize = sizeof (SETUP_DATA);
  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get setup variable\n"));
    return;
  }

  VarSize = sizeof (CPU_SETUP);
  Status = gRT->GetVariable (
                  L"CpuSetup",
                  &gCpuSetupVariableGuid,
                  NULL,
                  &VarSize,
                  &CpuSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Failed to get CPU Setup variable\n"));
    return;
  }

  if (SetupData.FastBoot) {
    //
    // Disable MdeModulePkg core PcdAtaSmartEnable to skip HDD SMART enabling
    //
    PcdSetBoolS (PcdAtaSmartEnable, FALSE);
    //
    // Enable gEfiMdeModulePkgTokenSpaceGuid PcdFastPS2Detection to enable quick PS2 device detection
    //
    PcdSetBoolS (PcdFastPS2Detection, TRUE);
  }
  return;
}


/**
  This is the standard  driver entry point for the Platform Setup Driver.
  This driver is responsible for setting up any platform specific policy or
  initialization information.

  @param[in] ImageHandle     - Handle for the image of this driver
  @param[in] SystemTable     - Pointer to the EFI System Table

  @retval EFI_SUCCESS        - Policy decisions are set successfully
**/
EFI_STATUS
EFIAPI
PlatformSetupEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                         Status;
  EFI_HANDLE                         Handle;
  UINTN                              VariableSize;
  UINT32                             SetupVolAttributes;
  MEM_INFO_PROTOCOL                  *MemoryInfoProtocol;
  Handle        = NULL;

  Status = gBS->LocateProtocol (&gEfiPciRootBridgeIoProtocolGuid, NULL, (VOID **) &mPciRootBridgeIo);
  ASSERT_EFI_ERROR (Status);

  //
  // Change PCD's value that depend on xxxSetup variable
  //
  PlatformPcdInit();

  //
  // Get platform Setup setting.
  //
  // @todo missing SETUP_DATA.MicrocodeUpdateSignature and SETUP_DATA.InterruptFilteringEnable
  //
  VariableSize = sizeof (CPU_SETUP);
  ZeroMem (&mCpuSetup, VariableSize);
  gRT->GetVariable (L"CpuSetup", &gCpuSetupVariableGuid, NULL, &VariableSize, &mCpuSetup);

  //
  // Create and zero initialize SetupVolatileData variable.
  //
  VariableSize = sizeof(SETUP_VOLATILE_DATA);
  Status = gRT->GetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  &SetupVolAttributes,
                  &VariableSize,
                  &mSetupVolatileData
                  );
  ASSERT (Status == EFI_NOT_FOUND);
  mSetupVolatileData.PlatformFlavor = PcdGet8 (PcdPlatformFlavor); // Platform Reset will check this data for SusPwrDnAck setting
  mSetupVolatileData.IpfEnable = PcdGetBool (VpdPcdIpfEnable);
  SetupVolAttributes = EFI_VARIABLE_BOOTSERVICE_ACCESS;
  Status = gRT->SetVariable (
                  L"SetupVolatileData",
                  &gSetupVariableGuid,
                  SetupVolAttributes,
                  sizeof(SETUP_VOLATILE_DATA),
                  &mSetupVolatileData
                  );
  ASSERT_EFI_ERROR(Status);


  //
  // Initialize some variables for CPU Setup Page
  //
  CpuSetupInit();

  //
  // Initialize the MEM_INFO_PROTOCOL structure
  //
  MemoryInfoProtocol = AllocateZeroPool (sizeof (MEM_INFO_PROTOCOL));
  if (MemoryInfoProtocol == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  //
  // Set up the policy protocols
  //
  SetupPlatformPolicies (
    MemoryInfoProtocol
    );
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gMemInfoProtocolGuid,
                  MemoryInfoProtocol,
                  &gDxePolicyProtocolGuid,
                  NULL,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool (PcdEcEnable) == 1
  Status = gBS->InstallProtocolInterface (
                  &Handle,
                  &gEfiPs2PolicyProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  &mPs2PolicyData
                  );
  ASSERT_EFI_ERROR (Status);
#endif

  return EFI_SUCCESS;
}
