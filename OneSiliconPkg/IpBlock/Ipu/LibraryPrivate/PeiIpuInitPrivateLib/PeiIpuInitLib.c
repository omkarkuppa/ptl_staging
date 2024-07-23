/** @file
  PEIM Private Library to initialize for IPU device

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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
#include <Ppi/SiPolicy.h>
#include <IpuPreMemConfig.h>
#include <IpuDataHob.h>
#include <IpuConfigHob.h>

#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IpuInitLib.h>
#include <Defines/HostBridgeDefines.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/IpuLib.h>

#include <IndustryStandard/Pci30.h>
#include <Library/IGpuInfoLib.h>

/**
  IpuInit: Initialize the IPU device
**/
VOID
IpuInit (
  VOID
  )
{
  BOOLEAN                     IpuDisabled;
  UINT64                      IpuBaseAddress;
  IPU_DATA_HOB                *IpuDataHob;
  IPU_CONFIG_HOB              *IpuConfigHob;
  IPU_PREMEM_CONFIG           *IpuPreMemPolicy;
  BOOLEAN                     IpuSupport;
  SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi;
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "IpuInit Begins\n"));
  IpuBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetIpuBusNumber (), GetIpuDevNumber (), GetIpuFuncNumber (), 0);
  IpuDisabled = FALSE;
  IpuDataHob = NULL;
  IpuConfigHob = NULL;
  IpuPreMemPolicy = NULL;

  // @todo :  Call GetConfigBlock here. IsIpuSupport() ouput IpuPreMemPolicy in the future.
  SiPreMemPolicyPpi  = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "BIOS doens't have IPU ConfigBlock\n"));
  }

  if (!IsIpuSupported () || IpuPreMemPolicy == NULL) {
    DEBUG ((DEBUG_INFO, "IPU is not supported\n"));
    IpuDisabled = TRUE;
    IpuSupport = FALSE;
  } else {
    IpuSupport = TRUE;
    DEBUG ((DEBUG_INFO, "Silicon supports IPU and BIOS has IPU config block\n"));
    //
    // Install IPU HOB
    //
    DEBUG ((DEBUG_INFO, "Install IPU HOB\n"));
    InstallIpuHob(&IpuDataHob, &IpuConfigHob);
    //
    // IPU is not fused off, and IPU ConfigBlock present
    //
    if ((PciSegmentRead16 (IpuBaseAddress + PCI_VENDOR_ID_OFFSET)) == 0xFFFF) {
      DEBUG ((DEBUG_WARN, "IPU config space not accessible!\n"));
      IpuDisabled = TRUE;
    }
    if (IpuPreMemPolicy->IpuEnable == 0) {
      DEBUG ((DEBUG_INFO, "Policy decides to disable IPU\n"));
      IpuDisabled = TRUE;
    }
  }

  //
  //  Program IPU bit at DEVEN register
  //
  if (!IpuDisabled) {
    //
    //  Set D5EN bit at DEVEN register
    //
    DEBUG ((DEBUG_INFO, "Set IPU bit at DEVEN regiser\n"));
    UncoreDevEnWrite (Ipu, 0, Enable);
  } else {
    //
    //  Clear D5EN bit at DEVEN register
    //
    DEBUG ((DEBUG_INFO, "Clear IPU bit at DEVEN regiser\n"));
    UncoreDevEnWrite (Ipu, 0, Disable);
  }

  if (!IpuDisabled) {
    //
    // Configure IMGUCLK
    //
    ConfigureImguClkOutGpio(IpuPreMemPolicy);
  }

  //
  // Initialize Hob for DXE phase use.
  //
  if (IpuDataHob != NULL && IpuConfigHob != NULL) {
    IpuDataHob->IpuSupport = IpuSupport;
    DEBUG ((DEBUG_INFO, "IpuDataHob->IpuSupport = %X \n", IpuDataHob->IpuSupport));
    //
    // Update IPU ACPI mode depending on IGPU present or not
    //
    if ((!IpuDisabled) && (IGpuGetVendorId () != 0xFFFF)) {
      IpuConfigHob->IpuAcpiMode = 1;         // Set IPU ACPI mode as IGPU Child device
    } else {
      IpuConfigHob->IpuAcpiMode = 0;         // Set IPU ACPI mode as Disabled
    }
    DEBUG ((DEBUG_INFO, "IpuConfigHob->IpuAcpiMode = %X \n", IpuConfigHob->IpuAcpiMode));

  }
}

/**
  IsIpuEnabled: Check if IPU is Enabled or not

  @retval FALSE = IPU is not supported, and/or IPU policy is disabled. TRUE = IPU is supported and IPU policy is enabled.
**/
BOOLEAN
IsIpuEnabled (
  VOID
  )
{
  IPU_PREMEM_CONFIG    *IpuPreMemPolicy;
  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi;
  EFI_STATUS           Status;
  UINT64               IpuBaseAddress;

  // @todo :  Call GetConfigBlock here. IsIpuSupport() ouput IpuPreMemPolicy in the future.
  ///
  /// Check if BIOS supports IPU by ConfigBlock presence
  ///
  SiPreMemPolicyPpi  = NULL;
  IpuPreMemPolicy = NULL;
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_INFO, "BIOS doens't have IPU ConfigBlock\n"));
    return FALSE;
  }

  IpuBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetIpuBusNumber (), GetIpuDevNumber (), GetIpuFuncNumber (), 0);
  if (!IsIpuSupported () || ((PciSegmentRead16 (IpuBaseAddress + PCI_VENDOR_ID_OFFSET)) == 0xFFFF)) {
    return FALSE;
  }

  if (IpuPreMemPolicy->IpuEnable) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  Init and Install IPU Hob
  @param[out] IPU_DATA_HOB        - IPU_DATA_HOB instance installed by this function
  @param[out] IPU_CONFIG_HOB      - IPU_CONFIG_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallIpuHob (
  OUT      IPU_DATA_HOB                 **IpuDataHobOut,
  OUT      IPU_CONFIG_HOB               **IpuConfigHobOut
  )
{
  EFI_STATUS                    Status;
  IPU_DATA_HOB                  *IpuDataHob;
  IPU_CONFIG_HOB                *IpuConfigHob;

  ///
  /// Create HOB for IPU Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (IPU_DATA_HOB),
             (VOID **) &IpuDataHob
             );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Create HOB for IPU Config
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (IPU_CONFIG_HOB),
             (VOID **) &IpuConfigHob
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize default HOB data
  ///
  ///
  /// IPU_DATA_HOB
  ///
  IpuDataHob->EfiHobGuidType.Name = gIpuDataHobGuid;
  DEBUG ((DEBUG_INFO, "IpuDataHob->EfiHobGuidType.Name: %g\n", &IpuDataHob->EfiHobGuidType.Name));
  ZeroMem (&(IpuDataHob->IpuSupport), sizeof (IPU_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  DEBUG ((DEBUG_INFO, "IpuDataHob @ %X\n", IpuDataHob));
  DEBUG ((DEBUG_INFO, "IpuDataHobSize - HobHeader: %X\n", sizeof (IPU_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "IpuDataHobSize: %X\n", sizeof (IPU_DATA_HOB)));
  ///
  /// IPU_DATA_HOB
  ///
  IpuConfigHob->EfiHobGuidType.Name = gIpuConfigHobGuid;
  DEBUG ((DEBUG_INFO, "IpuConfigHob->EfiHobGuidType.Name: %g\n", &IpuConfigHob->EfiHobGuidType.Name));
  ZeroMem (&(IpuConfigHob->IpuAcpiMode), sizeof (IPU_CONFIG_HOB) - sizeof (EFI_HOB_GUID_TYPE));
  DEBUG ((DEBUG_INFO, "IpuConfigHob @ %X\n", IpuDataHob));
  DEBUG ((DEBUG_INFO, "IpuConfigHobSize - HobHeader: %X\n", sizeof (IPU_CONFIG_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "IpuConfigHobSize: %X\n", sizeof (IPU_CONFIG_HOB)));


  (*IpuDataHobOut)   = IpuDataHob;
  (*IpuConfigHobOut) = IpuConfigHob;

  DEBUG ((DEBUG_INFO, "IPU HOB installed\n"));

  return EFI_SUCCESS;
}



/**
  Check IPU policies are valid for debugging unexpected problem if these
  values are not initialized or assigned incorrect resource.

  @param[in] IpuPreMemPolicy    The IPU PreMem Policy instance

**/
VOID
IpuValidatePolicy (
  IN    IPU_PREMEM_CONFIG    *IpuPreMemPolicy
  )
{
  ASSERT (IpuPreMemPolicy->Header.Revision == IPU_PREMEM_CONFIG_REVISION);
}
