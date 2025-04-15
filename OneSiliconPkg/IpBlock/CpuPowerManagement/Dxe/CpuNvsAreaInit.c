/** @file
  Initialize Cpu Nvs Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "CpuNvsAreaInit.h"
#include <Pi/PiMultiPhase.h>
#include <CpuPowerManagementPrivateDataHob.h>
#include <CpuPowerManagementDataHob.h>
#include <CpuPowerMgmt.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/DebugLib.h>
#include <Library/MsrFruLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/CpuLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/CpuMailboxLib.h>
#include <Protocol/CpuNvsArea.h>
#include <Protocol/MpService.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Library/MemoryAllocationLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED CPU_NVS_AREA_PROTOCOL       mCpuNvsAreaProtocol;

typedef struct {
  EFI_MP_SERVICES_PROTOCOL  *MpService;
  UINT8                     *CoreType;
} GET_CORE_TYPE_PER_THREAD_CONTEXT;

/**
  Get Core Type per thread

  @param Buffer  Pointer to GET_CORE_TYPE_PER_THREAD_CONTEXT
**/
VOID
EFIAPI
GetCoreTypePerThread (
  IN  VOID *Buffer
  )
{
  EFI_STATUS                        Status;
  UINTN                             CpuIndex;
  GET_CORE_TYPE_PER_THREAD_CONTEXT  *Context;

  Context = (GET_CORE_TYPE_PER_THREAD_CONTEXT *) Buffer;

  Status = Context->MpService->WhoAmI (Context->MpService, &CpuIndex);
  ASSERT_EFI_ERROR (Status);

  GetCoreType (&Context->CoreType[CpuIndex]);
}

STATIC_ASSERT (
  ARRAY_SIZE (mCpuNvsAreaProtocol.Area->P1Ratio) == ARRAY_SIZE (mCpuNvsAreaProtocol.Area->IpcScaling),
  "P1Ratio and IpcScaling must have the same size"
  );

/**
  Initialize Cpu Nvs Area Protocol

  @retval EFI_SUCCESS           - thread can be successfully created
**/
EFI_STATUS
InitCpuNvsArea (
  VOID
  )
{
  EFI_STATUS                             Status;
  EFI_MP_SERVICES_PROTOCOL               *MpService;
  UINTN                                  NumberOfProcessors;
  UINTN                                  NumberOfEnabledProcessors;
  EFI_PROCESSOR_INFORMATION              *ProcessorInfo;
  UINT8                                  *CoreType;
  EFI_PHYSICAL_ADDRESS                   Area;
  MSR_FLEX_RATIO_REGISTER                FlexRatio;
  MSR_TEMPERATURE_TARGET_REGISTER        TempTarget;
  UINT8                                  MaxRefTemp;
  UINT8                                  Index;
  CPU_POWER_MANAGEMENT_DATA_HOB          *CpuPowerMgmtDataHob;
  VOID                                   *CpuPowerDataHob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB  *CpuPowerMgmtPrivateDataHob;
  VOID                                   *Hob;
  UINTN                                  Pages;
  UINT16                                 C6Latency = 0;
  UINT16                                 C7Latency = 0;
  UINT16                                 C8Latency = 0;
  UINT16                                 C9Latency = 0;
  UINT16                                 C10Latency = 0;
  CPU_SKU                                CpuSku;
  CPUID_PROCESSOR_FREQUENCY_ECX          ProcessorFreq;
  EFI_HANDLE                             Handle;

  ProcessorInfo = NULL;
  CoreType       = NULL;
  ///
  /// Get CPU Init Data Hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU Data HOB not available\n"));
    ASSERT (Hob != NULL);
    return EFI_NOT_FOUND;
  }
  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *)((UINTN) Hob + sizeof (EFI_HOB_GUID_TYPE));

  ///
  /// Get CPU Power Management Data Hob
  ///
  CpuPowerDataHob = GetFirstGuidHob (&gCpuPowerManagementDataHobGuid);
  if (CpuPowerDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU Power Management Data HOB not available\n"));
    ASSERT (CpuPowerDataHob != NULL);
    return EFI_NOT_FOUND;
  }
  CpuPowerMgmtDataHob = (CPU_POWER_MANAGEMENT_DATA_HOB *) GET_GUID_HOB_DATA (CpuPowerDataHob);

  CpuSku = GetCpuSku ();

  ///
  /// Allocated ACPI NVS type memory for Cpu Nvs Configuration.
  ///
  Pages = EFI_SIZE_TO_PAGES (sizeof (CPU_NVS_AREA));
  Area = 0xffffffff; // allocate address below 4G.

  Status = gBS->AllocatePages (
                  AllocateMaxAddress,
                  EfiACPIMemoryNVS,
                  Pages,
                  &Area
                  );
  ASSERT_EFI_ERROR (Status);

  mCpuNvsAreaProtocol.Area = (CPU_NVS_AREA *)(UINTN) Area;
  ZeroMem ((VOID *)mCpuNvsAreaProtocol.Area, sizeof (CPU_NVS_AREA));
  mCpuNvsAreaProtocol.Area->Cpuid = GetCpuFamilyModel() | GetCpuSteppingId();
  mCpuNvsAreaProtocol.Area->Revision = CPU_NVS_AREA_REVISION;

  ///
  /// Calculate the number of Oc bins supported. Read in MSR 194h FLEX_RATIO bits (19:17)
  ///
  FlexRatio.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  mCpuNvsAreaProtocol.Area->OcBins = (UINT8) FlexRatio.Bits.OcBins;

  ///
  /// Update NVS ASL items.
  ///

  ///
  /// Automatic Thermal Reporting for Thermal Management
  ///
  if (CpuPowerMgmtPrivateDataHob->EnableAllThermalFunctions) {
    ///
    /// Thermal Reporting for Critical trip
    /// MSR 1A2 bits 23:16 define the temperature that this specific processor can
    /// function upto. It is recommended that this value + 5  be used as default Critical trip point
    /// _CRT.
    ///
    TempTarget.Uint64 = AsmReadMsr64 (MSR_TEMPERATURE_TARGET);
    MaxRefTemp = (UINT8) TempTarget.Bits.RefTemp;
    mCpuNvsAreaProtocol.Area->AutoCriticalTripPoint = MaxRefTemp + 5;

    ///
    /// Thermal Reporting for Active Thermal Management
    /// It is recommended that the processor specific value in MSR 1A2 bits 15:8
    /// be used as the highest Active trip point i.e. _AC0.
    ///
    mCpuNvsAreaProtocol.Area->AutoActiveTripPoint = MaxRefTemp - (UINT8) TempTarget.Bits.FanTempTargetOfst;

    ///
    /// Thermal Reporting for Passive Thermal Management
    /// On all turbo enabled systems, it is recommended that the ACPI _PSV point be
    /// set to a temperature above the Active cooling temperature and Tcc activation
    /// temperature.
    /// If platform embedded controller will issue PECI commands to reduce power as a
    /// passive thermal action, then it is recommended to use the package's max temperature
    /// for passive thermal control.
    ///
    mCpuNvsAreaProtocol.Area->AutoPassiveTripPoint = mCpuNvsAreaProtocol.Area->AutoCriticalTripPoint + 3;
  }

  ///
  ///  Update the PPM NVS area PPM flags
  ///
  mCpuNvsAreaProtocol.Area->PpmFlags = CpuPowerMgmtPrivateDataHob->PpmFlags;

  ///
  /// For U/Y, hard coded latency values are used instead of calculations. Set values here.
  ///
  switch (CpuSku) {
    case EnumCpuUlt:
    case EnumCpuUlx:
    case EnumCpuHalo:
    case EnumCpuTrad:
      C6Latency  = C6_LATENCY;
      C7Latency  = C7_LATENCY;
      C8Latency  = C8_LATENCY;
      C9Latency  = C9_LATENCY;
      C10Latency = C10_LATENCY;
    break;
    default:
    break;
  }

  ///
  /// Update PPM NVRAM Values for C6
  ///
  if (mCpuNvsAreaProtocol.Area->PpmFlags & C6_LONG_LATENCY_ENABLE) {
    mCpuNvsAreaProtocol.Area->C6MwaitValue = MWAIT_C6_1;
  } else {
    mCpuNvsAreaProtocol.Area->C6MwaitValue = MWAIT_C6;
  }


  ///
  /// Update PPM NVRAM Values for C7 - select the C-state supported among- C7 / C7S
  ///
  if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_C7) { // Is C7 supported ?
    if (mCpuNvsAreaProtocol.Area->PpmFlags & C7_LONG_LATENCY_ENABLE) {
      mCpuNvsAreaProtocol.Area->C7MwaitValue = MWAIT_C7_1;
    } else {
      mCpuNvsAreaProtocol.Area->C7MwaitValue = MWAIT_C7;
    }
  }
  if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_C7S) { // Is C7S supported ?
    if (mCpuNvsAreaProtocol.Area->PpmFlags & C7s_LONG_LATENCY_ENABLE) {
      mCpuNvsAreaProtocol.Area->C7MwaitValue = MWAIT_C7_3;
    } else {
      mCpuNvsAreaProtocol.Area->C7MwaitValue = MWAIT_C7_2;
    }
  }

  ///
  /// Hard coded values are used instead of calculations. Reinitialize here to simplify the code for C6 and C7.
  ///

  if (C6Latency != 0) {
    mCpuNvsAreaProtocol.Area->C6Latency = C6Latency;
  }

  if (C7Latency != 0) {
    mCpuNvsAreaProtocol.Area->C7Latency = C7Latency;
  }
  ///
  /// Update PPM NVRAM Values for CD - select the deepest C-state supported among- C8 / C9 / C10
  ///
  if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_C10) { // C10 supported
    mCpuNvsAreaProtocol.Area->CDPowerValue = C10_POWER;
    mCpuNvsAreaProtocol.Area->CDMwaitValue = MWAIT_CD_2;
    if (C10Latency != 0) {
      mCpuNvsAreaProtocol.Area->CDLatency = C10Latency;
    }
  } else if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_C9) { // C9 supported
    mCpuNvsAreaProtocol.Area->CDPowerValue = C9_POWER;
    mCpuNvsAreaProtocol.Area->CDMwaitValue = MWAIT_CD_1;
    if (C9Latency != 0) {
      mCpuNvsAreaProtocol.Area->CDLatency = C9Latency;
    }
  } else if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_C8) { // C8 supported
    mCpuNvsAreaProtocol.Area->CDPowerValue = C8_POWER;
    mCpuNvsAreaProtocol.Area->CDMwaitValue = MWAIT_CD;
    if (C8Latency != 0) {
      mCpuNvsAreaProtocol.Area->CDLatency = C8Latency;
    }
  }

  if (mCpuNvsAreaProtocol.Area->PpmFlags & PPM_HWP) {
    mCpuNvsAreaProtocol.Area->HwpEnable = 1;
  }

  mCpuNvsAreaProtocol.Area->CtdpLevelsSupported =CpuPowerMgmtPrivateDataHob->CtdpLevelsSupported;                        /// ConfigTdp Number Of Levels.
  mCpuNvsAreaProtocol.Area->ConfigTdpBootModeIndex = CpuPowerMgmtPrivateDataHob->ConfigTdpBootModeIndex;                 /// CTDP Boot Mode Index

  mCpuNvsAreaProtocol.Area->CtdpPowerLimit1  = CpuPowerMgmtPrivateDataHob->CtdpPowerLimit1;             /// CTDP active Level Power Limit1
  mCpuNvsAreaProtocol.Area->CtdpPowerLimit2  = CpuPowerMgmtPrivateDataHob->CtdpPowerLimit2;             /// CTDP active Level Power Limit2
  mCpuNvsAreaProtocol.Area->CtdpPowerLimitWindow  =CpuPowerMgmtPrivateDataHob->CtdpPowerLimitWindow;    /// CTDP active Level Power Limit1 Time Window

  mCpuNvsAreaProtocol.Area->ConfigurablePpc = CpuPowerMgmtPrivateDataHob->ConfigurablePpc;                               /// Max ratio of the slowest core.
  mCpuNvsAreaProtocol.Area->EnableRp = CpuPowerMgmtDataHob->EnableRp;                                                    /// Enable/Disable Resource Priority Feature
  DEBUG (( DEBUG_INFO, "CPU NVS: Resource Priority Feature = %d\n", mCpuNvsAreaProtocol.Area->EnableRp));

  mCpuNvsAreaProtocol.Area->ThETAIbattEnable = CpuPowerMgmtPrivateDataHob->ThETAIbattEnable;
  mCpuNvsAreaProtocol.Area->IsysLimitL1Enable = CpuPowerMgmtPrivateDataHob->IsysLimitL1Enable;
  mCpuNvsAreaProtocol.Area->IsysLimitL2Enable = CpuPowerMgmtPrivateDataHob->IsysLimitL2Enable;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &MpService);
  ASSERT_EFI_ERROR (Status);

  Status = MpService->GetNumberOfProcessors (
                        MpService,
                        &NumberOfProcessors,
                        &NumberOfEnabledProcessors
                        );
  ASSERT_EFI_ERROR (Status);


  //
  // Allocate buffer for ProcessorInfo & CoreType
  //
  ProcessorInfo = (EFI_PROCESSOR_INFORMATION *) AllocatePages (EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (EFI_PROCESSOR_INFORMATION)));
  if (ProcessorInfo == NULL) {
    ASSERT (FALSE);
  }
  CoreType = (UINT8 *)AllocatePages (EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (UINT8)));
  if (CoreType == NULL) {
    ASSERT (FALSE);
  }

  //
  // If NumberOfProcessors exceeds array size, then ASSERT().
  //
  ASSERT (NumberOfProcessors <= ARRAY_SIZE(mCpuNvsAreaProtocol.Area->IpcScaling));
  ASSERT (NumberOfProcessors <= ARRAY_SIZE(mCpuNvsAreaProtocol.Area->P1Ratio));

  if ((NumberOfProcessors > ARRAY_SIZE(mCpuNvsAreaProtocol.Area->IpcScaling)) ||
      (NumberOfProcessors > ARRAY_SIZE(mCpuNvsAreaProtocol.Area->P1Ratio))) {
    return EFI_ABORTED;
  }

  ASSERT (ProcessorInfo != NULL);
  if (ProcessorInfo != NULL) {
    DEBUG ((DEBUG_INFO, "Collect the ProcessorInfo, IpcScaling and P1Ratio. \n"));
    GetProcessorInfoAndCoreType (ProcessorInfo, CoreType, NumberOfProcessors);
    GetIpcScalingFru (ProcessorInfo, mCpuNvsAreaProtocol.Area->IpcScaling, NumberOfProcessors);
    GetP1RatioFru (ProcessorInfo, mCpuNvsAreaProtocol.Area->P1Ratio, NumberOfProcessors);
  }

  for (Index = 0; Index < NumberOfProcessors; Index++) {
    //
    // Dump ProcessorInfo[Index], CoreType[Index], IpcScaling[Index] and P1Ratio[Index]
    //
    DEBUG ((
      DEBUG_INFO, "CPU NVS: Processor[%d] APIC ID = 0x%08x, Die/Module/Core/Thread = %d/%d/%d/%d, CoreType = 0x%x, IpcScaling = %d, P1 Ratio = %d\n",
      Index, ProcessorInfo[Index].ProcessorId,
      ProcessorInfo[Index].ExtendedInformation.Location2.Die, ProcessorInfo[Index].ExtendedInformation.Location2.Module,
      ProcessorInfo[Index].ExtendedInformation.Location2.Core, ProcessorInfo[Index].ExtendedInformation.Location2.Thread,
      CoreType[Index], mCpuNvsAreaProtocol.Area->IpcScaling[Index], mCpuNvsAreaProtocol.Area->P1Ratio[Index]
      ));
  }

  AsmCpuid (CPUID_PROCESSOR_FREQUENCY, NULL, NULL, &ProcessorFreq.Uint32, NULL);
  mCpuNvsAreaProtocol.Area->BusFrequency = (UINT16) ProcessorFreq.Bits.BusFrequency;

  mCpuNvsAreaProtocol.Area->MaxP1Ratio = GetMaxNonTurboRatio ();
  DEBUG ((
    DEBUG_INFO, "CPU NVS: BusFrequency / MaxP1Ratio = %d / %d\n",
    mCpuNvsAreaProtocol.Area->BusFrequency, mCpuNvsAreaProtocol.Area->MaxP1Ratio
  ));
  ///
  /// Install Cpu Power management GlobalNVS Area protocol
  ///
  Handle = NULL;
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &Handle,
                  &gCpuNvsAreaProtocolGuid,
                  &mCpuNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
  FreePages (ProcessorInfo, EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (EFI_PROCESSOR_INFORMATION)));
  FreePages (CoreType, EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (UINT8)));
  return EFI_SUCCESS;
}
