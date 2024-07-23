/** @file
  Processor Power Management initialization code. This code determines current
  user configuration and modifies and loads ASL as well as initializing chipset
  and processor features to enable the proper power management.

  Acronyms:
  - PPM:  Processor Power Management
  - TM:   Thermal Monitor
  - IST:  Intel(R) Speedstep technology
  - HT:   Hyper-Threading Technology
  - ITBM: Intel(R) Turbo Boost Max Technology

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

#include "CpuNvsAreaInit.h"
#include "PowerMgmtInit.h"
#include <Protocol/CpuInfo.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Library/CpuInfoFruLib.h>
#include <Library/CpuLib.h>

//
// Global variables
//

GLOBAL_REMOVE_IF_UNREFERENCED CPU_NVS_AREA_PROTOCOL *gCpuNvsAreaProtocol = NULL;  ///< CPU GlobalNvs Protocol
#if FixedPcdGetBool(PcdBiosGuardEnable) == 1
GLOBAL_REMOVE_IF_UNREFERENCED BIOSGUARD_NVS_AREA_PROTOCOL *gBiosGuardNvsAreaProtocol = NULL;  ///< BIOS Guard GlobalNvs Protocol
#endif

//
// FVID Table Information
// Default FVID table
// One header field plus states
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT16     mNumberOfStates            = 0;
GLOBAL_REMOVE_IF_UNREFERENCED FVID_TABLE *mFvidPointer              = NULL;

//
// Globals to support updating ACPI Tables
//
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_TABLE_PROTOCOL                *mAcpiTable                   = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpu0IstTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mApIstTable                  = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpu0CstTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mApCstTable                  = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpuSsdtTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpu0TstTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mApTstTable                  = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpu0HwpTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mApHwpTable                  = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mCpu0PsdTable                = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED EFI_ACPI_DESCRIPTION_HEADER            *mApPsdTable                  = NULL;
GLOBAL_REMOVE_IF_UNREFERENCED CPU_POWER_MANAGEMENT_DATA_HOB          *mCpuPowerMgmtDataHob         = NULL;

/**
  Initialize the power management support.
  This function will do boot time configuration:
    Detect HW capabilities and SW configuration
    Initialize HW and software state (primarily MSR and ACPI tables)

  @param[in] ImageHandle  Pointer to the loaded image protocol for this driver
  @param[in] SystemTable  Pointer to the EFI System Table

  @retval EFI_SUCCESS     The driver installes/initialized correctly.
  @retval Driver will ASSERT in debug builds on error.  PPM functionality is considered critical for mobile systems.
**/
EFI_STATUS
EFIAPI
InitPowerManagement (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  EFI_STATUS                            Status;
  VOID                                  *Hob;
  VOID                                  *CpuPowerDataHob;
  CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *CpuPowerMgmtPrivateDataHob;

  ///
  /// Get CPU Power Management Data Hob
  ///
  CpuPowerDataHob = GetFirstGuidHob (&gCpuPowerManagementDataHobGuid);
  if (CpuPowerDataHob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU Power Management Data HOB not available\n"));
    ASSERT (CpuPowerDataHob != NULL);
    return EFI_NOT_FOUND;
  }
  mCpuPowerMgmtDataHob = (CPU_POWER_MANAGEMENT_DATA_HOB *) GET_GUID_HOB_DATA (CpuPowerDataHob);

  ///
  /// Get CPU power management private data hob
  ///
  Hob = GetFirstGuidHob (&gCpuPowerManagementPrivateDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "CPU power management private data hob not available\n"));
    ASSERT (Hob != NULL);
    return EFI_NOT_FOUND;
  }
  CpuPowerMgmtPrivateDataHob = (CPU_POWER_MANAGEMENT_PRIVATE_DATA_HOB *) GET_GUID_HOB_DATA (Hob);
  ///
  /// Initialize the Global pointer for VID Table
  ///
  mFvidPointer         = (FVID_TABLE *) &(CpuPowerMgmtPrivateDataHob->FvidTable);

  InitCpuNvsArea ();

  ///
  /// Initialize Power management Global variables
  ///
  InitPowerManagementGlobalVariables ();

  /// Initialize CPU Power management code (Patch and install CPU ACPI tables, save S3 boot script info)
  ///
  Status = InitPpmDxe ();
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}

/**
  Initializes the platform power management global variable.
  This must be called prior to any of the functions being used.
**/
VOID
InitPowerManagementGlobalVariables (
  VOID
  )
{
  EFI_STATUS                            Status;

#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
  BIOSGUARD_HOB                   *BiosGuardHobPtr;
#endif

  ///
  /// Locate CPU Nvs Protocol.
  ///
  Status = gBS->LocateProtocol (
                  &gCpuNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &gCpuNvsAreaProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CPU GloableNvs protocol not found"));
  }
  ASSERT_EFI_ERROR (Status);
  if (gCpuNvsAreaProtocol != NULL) {
    gCpuNvsAreaProtocol->Area->Cpuid = GetCpuFamilyModel () | GetCpuSteppingId ();
  }

#if (FixedPcdGetBool(PcdBiosGuardEnable) == 1) && (FixedPcdGetBool(PcdBiosGuardLegacyAcpi) == 1)
  ///
  /// Get BIOS Guard Config Hob
  ///
  BiosGuardHobPtr = GetFirstGuidHob (&gBiosGuardHobGuid);
  if (BiosGuardHobPtr != NULL) {
    ///
    /// Locate BIOS Guard Nvs Protocol.
    ///
    Status = gBS->LocateProtocol (
                    &gBiosGuardNvsAreaProtocolGuid,
                    NULL,
                    (VOID **) &gBiosGuardNvsAreaProtocol
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "BIOS Guard Global Nvs protocol not found"));
    }
  }else {
    DEBUG ((DEBUG_ERROR, "BIOS Guard Hob not found, BIOS Guard Global Nvs protocol not initialized\n"));
  }
#endif

  ///
  /// Locate ACPI table protocol
  ///
  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **) &mAcpiTable);
  ASSERT_EFI_ERROR (Status);
  return;
}

/**
  Initialize the processor power management based on hardware capabilities
  and user configuration settings.

  @retval EFI_SUCCESS - on success
  @retval Appropiate failure code on error
**/
EFI_STATUS
InitPpmDxe (
  VOID
  )
{
  EFI_STATUS   Status;

  Status      = EFI_SUCCESS;

  Status = InitCpuAcpiTable ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return Status;
}
