/** @file
  This code provides a initialization of intel VT-d (Virtualization Technology for Directed I/O).

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiVtdInitLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/VtdInfoLib.h>
#include <CpuRegs.h>
#include <VtdDataHob.h>
#include <Library/MemoryAllocationLib.h>
#include <Register/VtdRegs.h>
#include <Library/PeiVtdInitFruLib.h>
#include <Library/PerformanceLib.h>

EFI_STATUS
EFIAPI
VtdOnMemoryDiscovered (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
EFIAPI
VtdOnEnableDmaBufferNotify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_PEI_NOTIFY_DESCRIPTOR mVtdMemoryDiscoveredNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMemoryDiscoveredPpiGuid,
  VtdOnMemoryDiscovered
};

EFI_PEI_NOTIFY_DESCRIPTOR mVTdEnableDmaBufferNotifyDesc = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gVTdEnableDmaBufferPpiGuid,
  VtdOnEnableDmaBufferNotify
};

/**
  Enable DMA buffer in VT-d.

  @param[in] VtdEngineNumber        VT-d engine number.

  @retval EFI_SUCCESS               Enable successfully.
  @retval EFI_INVALID_PARAMETER     Input parameters are invalid.
  @retval EFI_UNSUPPORTED           VT-d isn't supported.
  @retval EFI_OUT_OF_RESOURCES      There is no additional space in the PPI database.
  @retval EFI_NOT_FOUND             Pointer to EnableDmaBuffer function is NULL.
**/
EFI_STATUS
VtdEnableDmaBuffer (
  IN  VTD_ENGINE    VtdEngineNumber
  )
{
  EFI_STATUS            Status;
  SI_PREMEM_POLICY_PPI  *SiPreMemPolicy;
  VTD_CONFIG            *Vtd;
  VTD_ENABLE_DMA_BUFFER EnableDmaBuffer;

  DEBUG ((DEBUG_INFO, "%a(%d) - start\n", __FUNCTION__, VtdEngineNumber));

  PERF_INMODULE_BEGIN("VtdEnableDmaBuffer");

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **)&SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    PERF_INMODULE_END ("VtdEnableDmaBuffer");
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gVtdConfigGuid, (VOID *) &Vtd);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    PERF_INMODULE_END ("VtdEnableDmaBuffer");
    return Status;
  }

  if (IsVtdDisabled(Vtd)) {
    DEBUG ((DEBUG_WARN, "VT-d disabled\n"));
    PERF_INMODULE_END ("VtdEnableDmaBuffer");
    return EFI_UNSUPPORTED;
  }

  if (!IsVtdEngineEnabled (VtdEngineNumber)) {
    DEBUG ((DEBUG_WARN, "VT-d engine %d disabled\n", VtdEngineNumber));
    PERF_INMODULE_END ("VtdEnableDmaBuffer");
    return EFI_UNSUPPORTED;
  }

  EnableDmaBuffer = NULL;
  Status = PeiServicesLocatePpi (
             &gVTdEnableDmaBufferPpiGuid,
             0,
             NULL,
             (VOID **) &EnableDmaBuffer
             );
  if (!EFI_ERROR (Status)) {
    ASSERT (EnableDmaBuffer != NULL);
    Status = EnableDmaBuffer (VtdEngineNumber);
  }
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "DMA buffer for VT-d engine %d enabled\n", VtdEngineNumber));
  } else {
    DEBUG ((DEBUG_WARN, "DMA buffer for VT-d engine %d not enabled, status: %r\n", VtdEngineNumber, Status));
  }

  PERF_INMODULE_END ("VtdEnableDmaBuffer");
  return Status;
}

/**
  A notify callback function to Enable DMA buffer for IOC VT-d engine, installed on memory discovered signal.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.  Not used.

  @retval    EFI_SUCCESS          Always return EFI_SUCCESS.
  */
EFI_STATUS
EFIAPI
VtdOnMemoryDiscovered (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  //
  // Enable DMA buffer for IOC VT-d
  //
  VtdEnableDmaBuffer (GLOBAL_VTD);

  return EFI_SUCCESS;
}

/**
  A notify callback function of EnableDmaBuffer installed.

  @param[in] PeiServices          General purpose services available to every PEIM.
  @param[in] NotifyDescriptor     The notification structure this PEIM registered on install.
  @param[in] Ppi                  The notify callback PPI.  Not used.

  @retval    EFI_SUCCESS          Always return EFI_SUCCESS.
  */
EFI_STATUS
EFIAPI
VtdOnEnableDmaBufferNotify (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  )
{
  DEBUG ((DEBUG_INFO, "VtdOnEnableDmaBufferNotify()\n"));

  //
  // Enable DMA buffer for IOC VT-d
  //
  VtdEnableDmaBuffer (GLOBAL_VTD);

  return EFI_SUCCESS;
}

/**
  Configure VT-d Base and capabilities.

  @param[in]   VTD_CONFIG       VT-d config block from SA Policy PPI

  @retval      EFI_SUCCESS      VT-d initialization completed
  @retval      EFI_UNSUPPORTED  VT-d is not supported by silicon or not enabled by policy
**/
EFI_STATUS
VtdInit (
  IN VTD_CONFIG  *Vtd
  )
{
  EFI_STATUS     Status;

  //
  // Install VTD Data Hob
  //
  DEBUG ((DEBUG_INFO, "Install VTD DATA HOB\n"));
  InstallVtdDataHob ();

  //
  // Install callback on Memory Discovered event
  //
  Status = PeiServicesNotifyPpi (&mVtdMemoryDiscoveredNotifyDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot register VtdMemoryDiscovered event, status = %r\n", Status));
  }

  //
  // Install callback on EnableDmaBuffer installed
  //
  Status = PeiServicesNotifyPpi (&mVTdEnableDmaBufferNotifyDesc);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot register EnableDmaBuffer event, status = %r\n", Status));
  }
  ///
  /// Check SA supports VTD and VTD is enabled in setup menu
  ///
  if ((IsVtdDisabled(Vtd)) || !IsVtdSupported ()) {
    //
    // If VT-d is disabled, then let clear VT-d BAR
    //
    DisableVtdBar ();

    DEBUG ((DEBUG_WARN, "VTd disabled or no capability!\n"));
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "VTd enabled\n"));

  ///
  /// Enable VT-d support in PSFs
  ///
  PsfEnableVtd ();

  ///
  /// Program Remap Engine Base Address
  ///
  ConfigureVtdBarPreMem (Vtd);

  return EFI_SUCCESS;
}

/**
  Installs VTD Data Hob

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
InstallVtdDataHob (
  VOID
  )
{
  EFI_STATUS                  Status;
  VTD_DATA_HOB                *VtdDataHob;

  ///
  /// Create HOB for VTD Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (VTD_DATA_HOB),
             (VOID **) &VtdDataHob
             );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Initialize default HOB data
  ///
  CopyGuid (&VtdDataHob->EfiHobGuidType.Name, &gVtdDataHobGuid);
  DEBUG ((DEBUG_INFO, "VtdDataHob->EfiHobGuidType.Name: %g\n", &VtdDataHob->EfiHobGuidType.Name));
  ZeroMem (&(VtdDataHob->VtdDisable), sizeof (VTD_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));

  DEBUG ((DEBUG_INFO, "VtdDataHob @ %X\n", VtdDataHob));
  DEBUG ((DEBUG_INFO, "VtdDataHobSize - HobHeader: %X\n", sizeof (VTD_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "VtdDataHobSize: %X\n", sizeof (VTD_DATA_HOB)));

  return Status;
}

/**
  Update Vtd Hob in PostMem

  @param[in]  Vtd    Instance of VTD_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateVtdHobPostMem (
  IN  VTD_CONFIG  *Vtd
  )
{
  VTD_DATA_HOB    *VtdDataHob;
  UINT8           Index;
  UINT32          VtdBase;
  BOOLEAN         VtdIntRemapSupport;
  ECAP_VTD_IOMMU_STRUCT Ecap;

  ///
  /// Locate and update VTD Hob Data
  ///
  VtdDataHob = (VTD_DATA_HOB *) GetFirstGuidHob (&gVtdDataHobGuid);
  if (VtdDataHob != NULL) {
    VtdDataHob->VtdDisable = IsVtdDisabled(Vtd);
    for (Index = 0; Index < GetMaxVtdEngineNumber (); Index++) {
      VtdDataHob->BaseAddress[Index] = Vtd->BaseAddress[Index];
    }
    VtdDataHob->DmaControlGuarantee = (BOOLEAN)Vtd->DmaControlGuarantee;

    VtdIntRemapSupport = TRUE;
    for (Index = 0; Index < VTD_ENGINE_NUMBER; Index++) {
      ///
      /// skip if the VT-d engine is disable
      ///
      if (!IsVtdEngineEnabled (Index)) {
        continue;
      }

      VtdBase = GetVtdBaseAddress (Index);
      ///
      /// Check IR status and update the InterruptRemappingSupport
      ///
      Ecap.Data = MmioRead64 (VtdBase + ECAP_VTD_IOMMU_REG);
      if (Ecap.Bits.ir == 0) {
        VtdIntRemapSupport = FALSE;
        break;
      }
    }
    VtdDataHob->InterruptRemappingSupport = VtdIntRemapSupport;

  }
  return EFI_SUCCESS;
}

/**
  Enable non-Gfx VT-d engine if required.

  @param[in]   VTD_CONFIG       VT-d config block from SA Policy PPI

  @retval      EFI_SUCCESS      VT-d initialization completed
  @retval      EFI_UNSUPPORTED  VT-d is not supported by silicon or not enabled by policy
**/
EFI_STATUS
VtdLateInit (
  IN VTD_CONFIG  *Vtd
  )
{
  ///
  /// Check SA supports VTD and VTD is enabled in setup menu
  ///
  if (IsVtdDisabled(Vtd)||!IsVtdSupported ()) {
    DEBUG ((DEBUG_WARN, "VTd disabled or no capability!\n"));
    return EFI_UNSUPPORTED;
  }

  ConfigureVtdBarPostMem (Vtd);

  return EFI_SUCCESS;
}
