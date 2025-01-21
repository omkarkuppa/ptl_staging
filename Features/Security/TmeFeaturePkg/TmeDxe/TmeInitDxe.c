/** @file
  TME DXE driver

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
#include <Library/TmeInfoLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Library/PcdLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TpmMeasurementLib.h>
#include <Uefi/UefiSpec.h>
#include <IndustryStandard/UefiTcgPlatform.h>

// define mktme_tpm strings
#define FEATURE_ENABLE_TME_STRING     "FeatureTME=1"
#define FEATURE_DISABLE_TME_STRING    "FeatureTME=0"

/**
  Update UEFI memory map to add EFI_MEMORY_CPU_CRYPTO attributes for all system memory.

  @retval EFI_SUCCESS       Successfully update system memory map.
  @retval others            Failed to get DS memory map

**/
EFI_STATUS
UpdateMemoryMap (
  VOID
  )
{
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryMap;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR *MemoryDescriptor;
  UINTN                           NumberOfDescriptors;
  UINTN                           Index;
  UINT64                          Attributes;
  UINT64                          Capabilities;
  EFI_STATUS                      Status;

  MemoryMap = NULL;
  NumberOfDescriptors = 0;
  Status = gDS->GetMemorySpaceMap (&NumberOfDescriptors, &MemoryMap);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get GCD memory map space\n"));
    return Status;
  }

  for (Index = 0; Index < NumberOfDescriptors; Index++) {
    MemoryDescriptor = &MemoryMap[Index];
    if (MemoryDescriptor->GcdMemoryType == EfiGcdMemoryTypeSystemMemory) {
      Capabilities = MemoryDescriptor->Capabilities | EFI_MEMORY_CPU_CRYPTO;
      Status = gDS->SetMemorySpaceCapabilities (
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->Length,
        Capabilities
      );
      DEBUG ((DEBUG_ERROR,
        "Set Range %016lx-%016lx Cap %016lx Status = %r\n",
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->BaseAddress + MemoryDescriptor->Length - 1,
        Capabilities,
        Status
        ));

      Attributes = MemoryDescriptor->Attributes | EFI_MEMORY_CPU_CRYPTO;
      Status = gDS->SetMemorySpaceAttributes (
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->Length,
        Attributes
      );
      DEBUG ((DEBUG_ERROR,
        "Set Range %016lx-%016lx Attr %016lx Status = %r\n",
        MemoryDescriptor->BaseAddress,
        MemoryDescriptor->BaseAddress + MemoryDescriptor->Length - 1,
        Attributes,
        Status
        ));
    }
  }

  return Status;
}

/**
  Measure and log data for TME state and configuration

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.
**/
VOID
EFIAPI
MeasurementTmeState (
  IN  EFI_EVENT     Event,
  IN  VOID          *Context
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  VOID       *String;
  UINT32     StringLen;
  UINT32     PcrIndex = 1;

  if (IsTmeActivated () == TRUE) {
    String = FEATURE_ENABLE_TME_STRING;
    StringLen = sizeof (FEATURE_ENABLE_TME_STRING) - 1;
    DEBUG ((DEBUG_INFO, "Measure Tme Enabled. \n"));
  } else {
    String = FEATURE_DISABLE_TME_STRING;
    StringLen = sizeof (FEATURE_DISABLE_TME_STRING) - 1;
    DEBUG ((DEBUG_INFO, "Measure Tme Disabled. \n"));
  }

  // Measure and Log TME ACTIVATE MSR Data into PCR[1]
  Status = TpmMeasureAndLogData (
    PcrIndex,
    EV_PLATFORM_CONFIG_FLAGS,
    String,
    StringLen,
    String,
    StringLen
    );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MeasurementTmeState() PCR[1] return error: %r\n", Status));
  }

  return;
}

/**
  Tme Init DXE Module Entry Point.

  @param[in] ImageHandle          Image handle of this driver.
  @param[in] SystemTable          Global system service table.

  @retval EFI_SUCCESS             Initialization complete.
**/
EFI_STATUS
EFIAPI
TmeInitDxeEntryPoint (
  IN EFI_HANDLE            ImageHandle,
  IN EFI_SYSTEM_TABLE      *SystemTable
  )
{
  EFI_STATUS    Status;
  EFI_EVENT     Event;

  Status = EFI_SUCCESS;

  ///
  /// Make sure TME is supported
  ///
  if (IsTmeSupported() == TRUE) {
    /// Register a callback only if Tpm is supported
    if (FeaturePcdGet (PcdTpm2Enable) == TRUE) {
      ///
      /// Register callback to measure TME state
      ///
      Status = EfiCreateEventReadyToBootEx (
                TPL_CALLBACK,
                MeasurementTmeState,
                NULL,
                &Event
                );

      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "EfiCreateEventReadyToBootEx() return error: %r\n", Status));
        ASSERT_EFI_ERROR (Status);
      }
    }

    ///
    /// Check for TME status
    ///
    if (IsTmeActivated() == TRUE) {
      ///
      /// Update Memory type to EFI_MEMORY_CPU_CRYPTO
      /// when TME is enabled.
      ///
      Status = UpdateMemoryMap();
      if (EFI_ERROR (Status)) {
        DEBUG ((EFI_D_ERROR, "UpdateMemoryMap() return error: %r\n", Status));
        ASSERT_EFI_ERROR (Status);
      }
    }
  }

  return Status;
}