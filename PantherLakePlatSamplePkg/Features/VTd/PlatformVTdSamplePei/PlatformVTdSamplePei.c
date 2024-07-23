/** @file
  Platform VTd Sample PEI driver.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <PiPei.h>
#include <Ppi/VtdInfo.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/PciLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/HobLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TmeInfoLib.h>
#include <SetupVariable.h>
#include <Guid/VtdPmrInfoHob.h>
#include <IndustryStandard/Vtd.h>
#include <Ppi/VtdNullRootEntryTable.h>
#include <Ppi/VtdEnableDmaBufferPpi.h>


typedef struct {
  EFI_ACPI_DMAR_HEADER                         DmarHeader;
  //
  // VTd engine
  //
  EFI_ACPI_DMAR_DRHD_HEADER                    Drhd[1];
} PLATFORM_VTD_INFO_PPI;

// BIOS uses TE with a null root entry table to block VT-d engine access to block any DMA traffic in pre-memory phase.
EDKII_VTD_NULL_ROOT_ENTRY_TABLE_PPI mNullRootEntryTable = 0xFED20000;

EFI_PEI_PPI_DESCRIPTOR mPlatformNullRootEntryTableDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiVTdNullRootEntryTableGuid,
  &mNullRootEntryTable
};

typedef struct {
  BOOLEAN GlobalVtdEnabled;
  BOOLEAN GfxVtdEnabled;
  BOOLEAN NonGfxVtdEnabled;
} PLATFORM_VTD_PEI_STATE_INFO;

EFI_GUID mPlatformVtdPeiStateInfoGuid = {
  0x78e4b506, 0x7e2e, 0x4947, { 0xa6, 0xb6, 0x48, 0x1e, 0x5b, 0x9f, 0xcc, 0x50 }
};

/**
  The function installs data for EDKII_VTD_INFO_PPI.

  @param  PpiDescriptor         The pointer to the PEI PPI Descriptors.

  @retval EFI_SUCCESS           The interface was successfully installed.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space in the PPI database.
  @retval EFI_NOT_FOUND         The PPI for which the reinstallation was requested has not been
                                installed.
**/
EFI_STATUS
InstallPlatformVtdInfoPpi (
  EFI_PEI_PPI_DESCRIPTOR        *PpiDescriptor
  )
{
  EFI_STATUS                  Status;
  VOID                        *OldPpi;
  EFI_PEI_PPI_DESCRIPTOR      *OldPpiDescriptor;

  Status = PeiServicesLocatePpi (
             &gEdkiiVTdInfoPpiGuid,
             0,
             &OldPpiDescriptor,
             &OldPpi
             );

  if (EFI_ERROR (Status)) {
    Status = PeiServicesInstallPpi (PpiDescriptor);
  } else {
    //
    // Re-install the PPI
    //
    Status = PeiServicesReInstallPpi (OldPpiDescriptor, PpiDescriptor);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Failed to install a ppi - %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  return Status;
}

/**
  The function of installs EDKII_VTD_INFO_PPI.

  @param  GfxVtdEnabled           Enable GFX IOMMU
  @param  NonGfxVtdEnabled        Enable NON GFX IOMMU

  @retval  EFI_SUCCESS           The function completed successfully.
  @retval  EFI_OUT_OF_RESOURCES  The function isn't able to allocate memory.
**/
EFI_STATUS
InstallPlatformMultiVtdInfoPpi (
  BOOLEAN EnableGfxVtd,
  BOOLEAN EnableNonGfxVtd
  )
{
  UINT32                        DmarTableSize;
  UINT8                         *BufferBase;
  PLATFORM_VTD_INFO_PPI         *PlatformVtdInfo;
  EFI_PEI_PPI_DESCRIPTOR        *PpiDescriptor;
  UINT8                         DrhdId;

  DmarTableSize = sizeof (PLATFORM_VTD_INFO_PPI) + sizeof (EFI_ACPI_DMAR_DRHD_HEADER) * ((EnableGfxVtd ? 1 : 0) + (EnableNonGfxVtd ? 1 : 0));

  BufferBase = AllocateZeroPool (DmarTableSize + sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (BufferBase == NULL) {
    ASSERT (0);
    return EFI_OUT_OF_RESOURCES;
  }

  PlatformVtdInfo = (PLATFORM_VTD_INFO_PPI *) BufferBase;
  PlatformVtdInfo->DmarHeader.Header.Signature = EFI_ACPI_6_5_DMA_REMAPPING_TABLE_SIGNATURE;
  PlatformVtdInfo->DmarHeader.Header.Length = DmarTableSize;
  PlatformVtdInfo->DmarHeader.Header.Revision = EFI_ACPI_DMAR_REVISION;
  if (IsTmeActivated ()) {
    PlatformVtdInfo->DmarHeader.HostAddressWidth = GetMaxPhysicalAddressSize () + 4 - 1;
  } else {
    PlatformVtdInfo->DmarHeader.HostAddressWidth = GetMaxPhysicalAddressSize () - 1;
  }

  DrhdId = 0;

  if (EnableGfxVtd) {
    PlatformVtdInfo->Drhd[DrhdId].Header.Type = EFI_ACPI_DMAR_TYPE_DRHD;
    PlatformVtdInfo->Drhd[DrhdId].Header.Length = sizeof(EFI_ACPI_DMAR_DRHD_HEADER);
    PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress = GetVtdBaseAddress (GFX_VTD);
    DEBUG ((DEBUG_INFO, "%d Add DRHD for VTd 0x%lx\n", DrhdId, PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress));
    DrhdId++;
  }

  if (EnableNonGfxVtd) {
    PlatformVtdInfo->Drhd[DrhdId].Header.Type = EFI_ACPI_DMAR_TYPE_DRHD;
    PlatformVtdInfo->Drhd[DrhdId].Header.Length = sizeof(EFI_ACPI_DMAR_DRHD_HEADER);
    PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress = GetVtdBaseAddress (NON_GFX_VTD);
    DEBUG ((DEBUG_INFO, "%d Add DRHD for VTd 0x%lx\n", DrhdId, PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress));
    DrhdId++;
  }

  PlatformVtdInfo->Drhd[DrhdId].Header.Type = EFI_ACPI_DMAR_TYPE_DRHD;
  PlatformVtdInfo->Drhd[DrhdId].Header.Length = sizeof(EFI_ACPI_DMAR_DRHD_HEADER);
  PlatformVtdInfo->Drhd[DrhdId].Flags = EFI_ACPI_DMAR_DRHD_FLAGS_INCLUDE_PCI_ALL;
  PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress = GetVtdBaseAddress (GLOBAL_VTD);
  DEBUG ((DEBUG_INFO, "%d Add DRHD for VTd 0x%lx\n", DrhdId, PlatformVtdInfo->Drhd[DrhdId].RegisterBaseAddress));
  DrhdId++;

  PpiDescriptor = (EFI_PEI_PPI_DESCRIPTOR *) (BufferBase + DmarTableSize);
  PpiDescriptor->Flags = (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST);
  PpiDescriptor->Guid  = &gEdkiiVTdInfoPpiGuid;
  PpiDescriptor->Ppi   = PlatformVtdInfo;

  return InstallPlatformVtdInfoPpi (PpiDescriptor);
}

/**
  Enable VT-d Engine

  @param[in]  VtdEngineNumber   VT-d Engine Number

  @retval     EFI_SUCCESS       Enable success
  @retval     EFI_UNSUPPORTED   Enable failed
  @retval     EFI_DEVICE_ERROR  Enable failed
**/
EFI_STATUS
EnableVTdEngine (
  IN  VTD_ENGINE VtdEngineNumber
  )
{
  UINT64                               MchBar;
  BOOLEAN                              VtdSupported;
  UINT32                               VtdBarOffset;
  UINT32                               VtdBar;
  UINT64                               VtdBarRead;
  UINT64                               VtdBarValue;

  if (IsVtdEngineEnabled (VtdEngineNumber)) {
    DEBUG ((DEBUG_INFO, "VT-d Engine[%d] was enabled.\n", VtdEngineNumber));
    return EFI_SUCCESS;
  }

  VtdSupported = GetVtdEngineInfo (
                   VtdEngineNumber,
                   NULL,
                   NULL,
                   &VtdBarOffset,
                   &VtdBar
                   );
  if (!VtdSupported) {
    DEBUG ((DEBUG_INFO, "VT-d Engine[%d] not support.\n", VtdEngineNumber));
    return EFI_UNSUPPORTED;
  }

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  DEBUG ((DEBUG_INFO, "VT-d Engine[%d] MchBar - 0x%lx\n", VtdEngineNumber, MchBar));

  VtdBarRead = MmioRead64 (MchBar + VtdBarOffset);
  DEBUG ((DEBUG_INFO, "VTd BAR current - 0x%lx\n", VtdBarRead));
  VtdBarValue = VtdBar | (VtdBarRead & 0xFF) | (UINT64) (1<<VtdEngineNumber);

  MmioWrite64 (MchBar + VtdBarOffset, VtdBarValue);
  VtdBarRead = MmioRead64 (MchBar + VtdBarOffset);
  DEBUG ((DEBUG_INFO, "VTd BAR - 0x%lx\n", VtdBarRead));

  if (VtdBarRead != VtdBarValue) {
    DEBUG ((DEBUG_INFO, "VT-d Engine[%d] enabled failed.\n", VtdEngineNumber));
    return EFI_DEVICE_ERROR;
  }

  DEBUG ((DEBUG_INFO, "VT-d Engine[%d] success\n", VtdEngineNumber));
  return EFI_SUCCESS;
}

/**
  Initialize VTd register.
**/
VOID
InitGlobalVtd (
  VOID
  )
{
  DEBUG ((DEBUG_INFO, "InitGlobalVtd\n"));

  EnableVTdEngine (GLOBAL_VTD);
}

/**
  The callback function for SiInitDone.
  It reinstalls VTD_INFO_PPI.

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS       The function completed successfully.
**/
EFI_STATUS
EFIAPI
SiInitDonePpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  VOID                         *Hob;
  PLATFORM_VTD_PEI_STATE_INFO  *VtdPeiStateInfo;
  EFI_STATUS                   Status;

  DEBUG ((DEBUG_INFO, "SiInitDonePpiNotifyCallback for iommu\n"));

  Hob = GetFirstGuidHob (&mPlatformVtdPeiStateInfoGuid);
  if (Hob == NULL) {
    ASSERT (0);
    return EFI_NOT_FOUND;
  }
  VtdPeiStateInfo = GET_GUID_HOB_DATA (Hob);
  Status = EFI_SUCCESS;

  //
  // Enable GLOBAL IOMMU DMA protection
  //
  if (!VtdPeiStateInfo->GlobalVtdEnabled) {
    Status = InstallPlatformMultiVtdInfoPpi (FALSE, FALSE);
    if (!EFI_ERROR (Status)) {
      VtdPeiStateInfo->GlobalVtdEnabled = TRUE;
    }
  }

  return Status;
}

#if FixedPcdGetBool(PcdFspModeSelection) == 1
EFI_PEI_NOTIFY_DESCRIPTOR mSiInitDoneNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gFspSiliconInitDonePpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SiInitDonePpiNotifyCallback
};
#else
EFI_PEI_NOTIFY_DESCRIPTOR mSiInitDoneNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEndOfSiInitPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) SiInitDonePpiNotifyCallback
};
#endif

/**
  The callback function for EndOfPeiPpi.
  It enables IGD IOMMU PMR

  @param[in]  PeiServices       General purpose services available to every PEIM.
  @param[in]  NotifyDescriptor  Notify that this module published.
  @param[in]  Ppi               PPI that was installed.

  @retval     EFI_SUCCESS            The function completed successfully.
  @retval     EFI_UNSUPPORTED        Either IGD or IPU VTd is disabled or not supported
**/
EFI_STATUS
EFIAPI
EndOfPeiPpiNotifyCallback (
  IN CONST EFI_PEI_SERVICES     **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  VOID                         *Hob;
  PLATFORM_VTD_PEI_STATE_INFO  *VtdPeiStateInfo;
  EFI_STATUS                   Status;
  BOOLEAN                      NeedEnableGfxVtd;
  BOOLEAN                      NeedEnableNonGfxVtd;

  DEBUG ((DEBUG_INFO, "EndOfPeiPpiNotifyCallback for iommu\n"));

  Hob = GetFirstGuidHob (&mPlatformVtdPeiStateInfoGuid);
  if (Hob == NULL) {
    ASSERT (0);
    return EFI_NOT_FOUND;
  }
  VtdPeiStateInfo = GET_GUID_HOB_DATA (Hob);
  Status = EFI_SUCCESS;

  NeedEnableGfxVtd = FALSE;
  if (!VtdPeiStateInfo->GfxVtdEnabled) {
    if (IsVtdEngineEnabled (GFX_VTD)) {
      NeedEnableGfxVtd = TRUE;
      DEBUG ((DEBUG_INFO, "Need enable GFX VTd\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "GFX VTd is not enabled\n"));
    }
  }

  NeedEnableNonGfxVtd = FALSE;
  if (!VtdPeiStateInfo->NonGfxVtdEnabled) {
    if (IsVtdEngineEnabled (NON_GFX_VTD)) {
      NeedEnableNonGfxVtd = TRUE;
      DEBUG ((DEBUG_INFO, "Need enable NON GFX VTd\n"));
    } else {
      DEBUG ((DEBUG_ERROR, "NON GFX VTd is not enabled\n"));
    }
  }

  if (NeedEnableGfxVtd || NeedEnableNonGfxVtd) {
    Status = InstallPlatformMultiVtdInfoPpi (
               NeedEnableGfxVtd ? TRUE : VtdPeiStateInfo->GfxVtdEnabled,
               NeedEnableNonGfxVtd ? TRUE : VtdPeiStateInfo->NonGfxVtdEnabled
               );
    if (!EFI_ERROR (Status)) {
      VtdPeiStateInfo->GfxVtdEnabled = NeedEnableGfxVtd;
      VtdPeiStateInfo->NonGfxVtdEnabled = NeedEnableNonGfxVtd;
      DEBUG ((DEBUG_INFO, "Enable VTd engine done\n"));
    }
  }

  return Status;
}

EFI_PEI_NOTIFY_DESCRIPTOR mEndOfPeiNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT) EndOfPeiPpiNotifyCallback
};

/*
  Enable DMA buffer in VTd

  @param[in] VtdEngineIndex         VTd engine index.

  @retval EFI_SUCCESS               Enable successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_UNSUPPORTED           VTd base is zero.
  @retval EFI_OUT_OF_RESOURCES      There is no additional space in the PPI database.
*/
EFI_STATUS
EFIAPI
EnableDmaBuffer (
  IN VTD_ENGINE VtdEngineIndex
  )
{
  VOID                         *Hob;
  PLATFORM_VTD_PEI_STATE_INFO  *VtdPeiStateInfo;
  EFI_STATUS                   Status;
  VOID                         *MemoryDiscovered;

  DEBUG ((DEBUG_INFO, "Platform VTd Enable Dma Buffer PPI : EnableDmaBuffer(%d)\n", VtdEngineIndex));

  //
  // Check if memory is initialized.
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiMemoryDiscoveredPpiGuid,
             0,
             NULL,
             &MemoryDiscovered
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Memory is not initialized.\n"));
    return EFI_NOT_READY;
  }

  Hob = GetFirstGuidHob (&mPlatformVtdPeiStateInfoGuid);
  if (Hob == NULL) {
    ASSERT (0);
    return EFI_NOT_FOUND;
  }
  VtdPeiStateInfo = GET_GUID_HOB_DATA (Hob);
  Status = EFI_SUCCESS;

  if (VtdEngineIndex == GLOBAL_VTD) {
    //
    // Enable GLOBAL IOMMU DMA protection
    //
    if (!VtdPeiStateInfo->GlobalVtdEnabled) {
      Status = InstallPlatformMultiVtdInfoPpi (VtdPeiStateInfo->GfxVtdEnabled, VtdPeiStateInfo->NonGfxVtdEnabled);
      if (!EFI_ERROR (Status)) {
        VtdPeiStateInfo->GlobalVtdEnabled = TRUE;
        DEBUG ((DEBUG_INFO, "Enable Dma for GLOBAL VTd done\n"));
      }
    }
  } else if (VtdEngineIndex == GFX_VTD) {
    //
    // Enable GFX IOMMU DMA protection
    //
    if (!VtdPeiStateInfo->GfxVtdEnabled) {
      if (IsVtdEngineEnabled (VtdEngineIndex)) {
        Status = InstallPlatformMultiVtdInfoPpi (TRUE, VtdPeiStateInfo->NonGfxVtdEnabled);
        if (!EFI_ERROR (Status)) {
          VtdPeiStateInfo->GfxVtdEnabled = TRUE;
          DEBUG ((DEBUG_INFO, "Enable Dma for GFX VTd done\n"));
        }
      } else {
        DEBUG ((DEBUG_ERROR, "GFX VTd is not enabled\n"));
        Status = EFI_UNSUPPORTED;
      }
    }
  } else if (VtdEngineIndex == NON_GFX_VTD) {
    //
    // Enable NON GFX IOMMU DMA protection
    //
    if (!VtdPeiStateInfo->NonGfxVtdEnabled) {
      if (IsVtdEngineEnabled (VtdEngineIndex)) {
        Status = InstallPlatformMultiVtdInfoPpi (VtdPeiStateInfo->GfxVtdEnabled, TRUE);
        if (!EFI_ERROR (Status)) {
          VtdPeiStateInfo->NonGfxVtdEnabled = TRUE;
          DEBUG ((DEBUG_INFO, "Enable Dma for NON GFX VTd done\n"));
        }
      } else {
        DEBUG ((DEBUG_ERROR, "NON GFX VTd is not enabled\n"));
        Status = EFI_UNSUPPORTED;
      }
    }
  } else {
    Status = EFI_INVALID_PARAMETER;
  }
  return Status;
}

EFI_PEI_PPI_DESCRIPTOR mPlatformVtdEnableDmaBufferPpiDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gVTdEnableDmaBufferPpiGuid,
  (VOID*) (UINTN) EnableDmaBuffer
};

/**
  Check Iommu Ability base on Vtd Policy.
**/
VOID
PeiCheckIommuSupport (
  VOID
  )
{
  EFI_STATUS                      Status;
  SA_SETUP                        *SaSetup;
  SETUP_DATA                      *SetupData;
  UINTN                           VarSize;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINT64                          MchBar;
  BOOLEAN                         VtdSupport;
  UINT32                          VtdBarOffset;
  UINT32                          VtdBaseAddress;
  UINT32                          PreviousVtdBarValue;
  UINT32                          VtdBarEnBits;

  SetupData                       = NULL;
  SaSetup                         = NULL;
  VarSize                         = 0;
  //
  // Locate Setup variables
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid,
             0,
             NULL,
             (VOID **) &VariableServices
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "PeiServicesLocatePpi failed\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  VarSize = sizeof (SETUP_DATA);
  SetupData = AllocateZeroPool (VarSize);
  if (SetupData == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate SetupData size\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               SetupData
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (SetupData) failed, Status:%r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  //
  // Locate SaSetup variables
  //
  VarSize = sizeof (SA_SETUP);
  SaSetup = AllocateZeroPool (VarSize);
  if (SaSetup == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to allocate SaSetup size\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SaSetup",
                               &gSaSetupVariableGuid,
                               NULL,
                               &VarSize,
                               SaSetup
                               );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetVariable (SaSetup) failed, Status:%r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto Exit;
  }

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  VtdSupport = GetVtdEngineInfo (
                 GLOBAL_VTD,
                 NULL,
                 NULL,
                 &VtdBarOffset,
                 &VtdBaseAddress
                 );

  PreviousVtdBarValue = MmioRead32 (MchBar + VtdBarOffset);
  DEBUG ((DEBUG_INFO, "Previous Vtd Bar : 0x%X\n", PreviousVtdBarValue));

  // Check if Vtd Bar support
  if (VtdSupport) {
    MmioWrite32 (MchBar + VtdBarOffset, VtdBaseAddress | 0xFF);
    VtdBarEnBits = MmioRead32 (MchBar + VtdBarOffset) & 0xFF;
    DEBUG ((DEBUG_INFO, "VT-d engine enable bits: 0x%02x\n", VtdBarEnBits));
    VtdSupport = (BOOLEAN) (VtdBarEnBits & (1<<GLOBAL_VTD));

    //
    // Restore to previous value
    //
    MmioWrite32 (MchBar + VtdBarOffset, PreviousVtdBarValue);
  }

  if ((SaSetup->EnableVtd == 0x00) || (!VtdSupport)) {
    //
    // Set PcdVTdPolicyPropertyMask to 0 when VTd disable
    //
    PcdSet8S (PcdVTdPolicyPropertyMask, 0x00);
  } else {

    //
    // This Control Iommu setup option is mainly for debug purpose.
    // It might be removed when feature is stable
    //
    if (SetupData->ControlIommu != 0x00) {
      PcdSet8S (PcdVTdPolicyPropertyMask, PcdGet8 (PcdVTdPolicyPropertyMask) | BIT0);
    } else {
      PcdSet8S (PcdVTdPolicyPropertyMask, 0x00);
    }
  }

Exit:
  DEBUG ((DEBUG_INFO, "PcdVTdPolicyPropertyMask value: 0x%x\n", PcdGet8(PcdVTdPolicyPropertyMask)));

  if (SetupData != NULL) {
    FreePool (SetupData);
  }

  if (SaSetup != NULL) {
    FreePool (SaSetup);
  }

}

/**
  Platform VTd Info sample driver.

  @param[in] FileHandle         Handle of the file being invoked.
  @param[in] PeiServices        Describes the list of possible PEI Services.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_UNSUPPORTED       Iommu is unsupported.
**/
EFI_STATUS
EFIAPI
PlatformVTdInfoSampleInitialize (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                      Status;
  PLATFORM_VTD_PEI_STATE_INFO     *VtdPeiStateInfo;

  Status = EFI_UNSUPPORTED;

  DEBUG ((DEBUG_INFO, "PlatformVTdSampleInitialize START\n"));

  //
  // check if we should enable Iommu
  //
  PeiCheckIommuSupport ();

  if ((PcdGet8(PcdVTdPolicyPropertyMask) & BIT0) == 0) {
    DEBUG ((DEBUG_INFO, "Iommu is unsupported\n"));
    DEBUG ((DEBUG_INFO, "PlatformVTdInfoSampleInitialize END\n"));
    return Status;
  }

  //
  // Register a notification for SiInitDone to reinstall VTdInfo PPI
  //
  Status = PeiServicesNotifyPpi (&mSiInitDoneNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Failed to register a notification - %r\n", Status));
  }

  //
  // Register a notification for End of PEI to reinstall VTdInfo PPI
  //
  Status = PeiServicesNotifyPpi (&mEndOfPeiNotifyList);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Failed to register a notification - %r\n", Status));
  }

  InitGlobalVtd ();

  Status = PeiServicesInstallPpi (&mPlatformNullRootEntryTableDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Install RootEntryTable Ppi - %r\n", Status));
  }

  //
  // Install VTdInfo PPI in pre-memory phase
  //
  Status = InstallPlatformMultiVtdInfoPpi (FALSE, FALSE);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "InstallPlatformMultiVtdInfoPpi - %r\n", Status));
  }

  VtdPeiStateInfo = BuildGuidHob (&mPlatformVtdPeiStateInfoGuid, sizeof(PLATFORM_VTD_PEI_STATE_INFO));
  if (VtdPeiStateInfo == NULL) {
    ASSERT (0);
    return EFI_OUT_OF_RESOURCES;
  }
  VtdPeiStateInfo->GlobalVtdEnabled = FALSE;
  VtdPeiStateInfo->GfxVtdEnabled    = FALSE;
  VtdPeiStateInfo->NonGfxVtdEnabled = FALSE;

  Status = PeiServicesInstallPpi (&mPlatformVtdEnableDmaBufferPpiDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR , "Failed to install vtd enable dma buffer ppi - %r\n", Status));
  }

  DEBUG ((DEBUG_INFO, "PlatformVTdSampleInitialize END\n"));
  return Status;
}

