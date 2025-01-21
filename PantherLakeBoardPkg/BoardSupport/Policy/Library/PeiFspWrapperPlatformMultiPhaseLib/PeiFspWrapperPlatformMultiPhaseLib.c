/** @file
  FSP Wrapper to handle platform specific actions for
  FSP MultiPhase (SeparatePhase) Initialization.

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <FspGlobalData.h>
#include <SetupVariable.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <FirwmareConfigurations.h>
#include <Ppi/SiPolicy.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/PeiPchPolicyDebugLib.h>
#include <Library/PeiSiPolicyDebugLib.h>
#include <Library/PeiSaPolicyDebugLib.h>
#include <Library/PeiCpuPolicyDebugLib.h>
#include <SiliconPolicyHob.h>
#include <Library/HobLib.h>
#include <Library/PeiPolicyBoardConfigLib.h>
#include <Library/PeiPolicyAdvancedConfigLib.h>

STATIC
VOID
EFIAPI
SiliconPolicyUpdateMemInit (
  IN OUT VOID  **FspHobListPtr
  )
{
  SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi;
  SILICON_POLICY_HOB     *SiliconPolicyHob;
  EFI_HOB_GUID_TYPE      *GuidHob;
  EFI_PEI_HOB_POINTERS   Hob;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Hob.Raw = (UINT8 *)(UINTN)*FspHobListPtr;
  GuidHob = GetNextGuidHob (&gSiliconPolicyHobGuid, Hob.Raw);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }

  SiliconPolicyHob = (SILICON_POLICY_HOB *) GET_GUID_HOB_DATA (GuidHob);
  SiPreMemPolicyPpi = (SI_PREMEM_POLICY_PPI *) SiliconPolicyHob->SiPreMemPolicyPpi;

  //
  // Update essential policies by board configuration
  //
  UpdatePeiPolicyBoardConfigPreMem (SiPreMemPolicyPpi);

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePreMemAdvancedConfig (SiPreMemPolicyPpi);

  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
}

STATIC
VOID
EFIAPI
SiliconPolicyUpdateSiInit (
  IN OUT VOID  **FspHobListPtr
  )
{
  SI_PREMEM_POLICY_PPI   *SiPreMemPolicyPpi;
  SI_POLICY_PPI          *SiPolicyPpi;
  SILICON_POLICY_HOB     *SiliconPolicyHob;
  EFI_HOB_GUID_TYPE      *GuidHob;
  EFI_PEI_HOB_POINTERS   Hob;

  DEBUG ((DEBUG_INFO, "%a Entry\n", __FUNCTION__));

  Hob.Raw = (UINT8 *)(UINTN)*FspHobListPtr;
  GuidHob = GetNextGuidHob (&gSiliconPolicyHobGuid, Hob.Raw);
  ASSERT (GuidHob != NULL);
  if (GuidHob == NULL) {
    return;
  }

  SiliconPolicyHob = (SILICON_POLICY_HOB *) GET_GUID_HOB_DATA (GuidHob);
  SiPreMemPolicyPpi = (SI_PREMEM_POLICY_PPI *) SiliconPolicyHob->SiPreMemPolicyPpi;
  SiPolicyPpi       = (SI_POLICY_PPI *) SiliconPolicyHob->SiPolicyPpi;

  //
  // Update essential policies by board configuration
  //
  UpdatePeiPolicyBoardConfig (SiPreMemPolicyPpi, SiPolicyPpi);

  //
  // Update advanced policies
  //
  SiliconPolicyUpdatePostMemAdvancedConfig (SiPreMemPolicyPpi, SiPolicyPpi);
  DEBUG ((DEBUG_INFO, "%a Exit\n", __FUNCTION__));
}

/**
  FSP Wrapper Platform MultiPhase Handler

  @param[in] FspHobListPtr        - Pointer to FSP HobList (valid after FSP-M completed)
  @param[in] ComponentIndex       - FSP Component which executing MultiPhase initialization.
  @param[in] PhaseIndex           - Indicates current execution phase of FSP MultiPhase initialization.

  @retval EFI_STATUS        Always return EFI_SUCCESS

**/
VOID
EFIAPI
FspWrapperPlatformMultiPhaseHandler (
  IN OUT VOID  **FspHobListPtr,
  IN UINT8     ComponentIndex,
  IN UINT32    PhaseIndex
  )
{
  switch (ComponentIndex) {
    case FspMultiPhaseMemInitApiIndex:
      switch (PhaseIndex) {
        case 1:
          DEBUG ((DEBUG_INFO, "%a PhaseIndex = %d\n", __FUNCTION__, PhaseIndex));
          SiliconPolicyUpdateMemInit (FspHobListPtr);
        break;
      }
      break;
    case FspMultiPhaseSiInitApiIndex:
      switch (PhaseIndex) {
        case 1:
          DEBUG ((DEBUG_INFO, "%a PhaseIndex = %d\n", __FUNCTION__, PhaseIndex));
          SiliconPolicyUpdateSiInit (FspHobListPtr);
        break;
      }
      break;
  }
}
