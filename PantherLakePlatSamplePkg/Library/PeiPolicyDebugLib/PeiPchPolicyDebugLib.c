/** @file
  This file is SampleCode of the library for Intel PCH PEI Debug Policy initialization.

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
#include <Uefi.h>
#include <PiPei.h>
#include <SetupVariable.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PchInfoLib.h>
#include <Library/SiPolicyLib.h>
#include <PolicyUpdateMacro.h>
#include <Ppi/PchPolicy.h>
#include <PmConfig.h>
#include <P2sbConfig.h>
#include <PchPcieRpConfig.h>
#include <PchDmiConfig.h>
#include <UsbConfig.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspsUpd.h>
#endif

/**
  Update USB debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateUsbDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 0
  USB_CONFIG                      *UsbConfig;
  EFI_STATUS                      Status;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  UsbConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gUsbConfigGuid, (VOID *) &UsbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchXhciOcLock, UsbConfig->XhciOcLock, PchSetup->PchXhciOcLock);
}

/**
  Update Power Management debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdatePmDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 0
  PCH_PM_CONFIG                *PmConfig;
  EFI_STATUS                   Status;
#endif

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                         *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  PmConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gPmConfigGuid, (VOID *) &PmConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchPmDisableEnergyReport,     PmConfig->DisableEnergyReport, (PchSetup->PchEnergyReport == 1)? 0 : 1);
  UPDATE_POLICY (((FSPS_UPD *)FspsUpd)->FspsConfig.PchPmErDebugMode,              PmConfig->PmErDebugMode, PchSetup->PchPmErDebugMode);
  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PmcLpmS0ixSubStateEnableMask, PmConfig->LpmS0ixSubStateEnable.Val,  ((PchSetup->PmcLpmS0i2p0En << 0) |
                                                                                                                             (PchSetup->PmcLpmS0i2p1En << 1) |
                                                                                                                             (PchSetup->PmcLpmS0i2p2En << 2) |
                                                                                                                             (PchSetup->PmcLpmS0i3p0En << 3) |
                                                                                                                             (PchSetup->PmcLpmS0i3p1En << 4) |
                                                                                                                             (PchSetup->PmcLpmS0i3p2En << 5) |
                                                                                                                             (PchSetup->PmcLpmS0i3p3En << 6) |
                                                                                                                             (PchSetup->PmcLpmS0i3p4En << 7)));
}

/**
  Update P2sb debug policies.

  @param[in] SiPolicy             Pointer to SI_POLICY_PPI
  @param[in] PchSetup             Pointer to PCH_SETUP buffer
**/
STATIC
VOID
UpdateP2sbDebugPolicy (
  IN SI_POLICY_PPI             *SiPolicy,
  IN PCH_SETUP                 *PchSetup
  )
{
#if FixedPcdGet8(PcdFspModeSelection) == 0
  PCH_P2SB_CONFIG                 *P2sbConfig;
  EFI_STATUS                      Status;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                            *FspsUpd;
  FspsUpd = (FSPS_UPD *)(UINTN) PcdGet64 (PcdFspsUpdDataAddress64);
  ASSERT (FspsUpd != NULL);
#else
  P2sbConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPolicy, &gP2sbConfigGuid, (VOID *) &P2sbConfig);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return;
  }
#endif

  UPDATE_POLICY (((FSPS_UPD *) FspsUpd)->FspsConfig.PchSbAccessUnlock, P2sbConfig->SbAccessUnlock, PchSetup->PchSidebandLock ? 0 : 1);
}

/**
  This function performs PCH PEI Debug Policy initialization.

  @param[in] SiPreMemPolicyPpi  The RC PREMEM Policy PPI instance
  @param[in] SiPolicyPpi        The RC POSTMEM Policy PPI instance

  @retval EFI_SUCCESS           The PPI is installed and initialized.
  @retval EFI ERRORS            The PPI is not successfully installed.
**/
EFI_STATUS
EFIAPI
UpdatePeiPchPolicyDebug (
  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  SI_POLICY_PPI            *SiPolicy
  )
{
  EFI_STATUS                      Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI *VariableServices;
  UINTN                           VariableSize;
  PCH_SETUP                       PchSetup;

  DEBUG ((DEBUG_INFO, "Update PeiPchPolicyDebug Pos-Mem Start\n"));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VariableSize = sizeof (PCH_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"PchSetup",
                               &gPchSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &PchSetup
                               );
  ASSERT_EFI_ERROR (Status);

  UpdateUsbDebugPolicy (SiPolicy, &PchSetup);
  UpdatePmDebugPolicy (SiPolicy, &PchSetup);
  UpdateP2sbDebugPolicy (SiPolicy, &PchSetup);

  return Status;
}
