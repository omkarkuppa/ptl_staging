/** @file
  Overclocking early post initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Uefi.h>
#include <Library/CpuCommonLib.h>
#include <Library/PeiOcLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/ConfigBlockLib.h>
#include <IntelRcStatusCode.h>
#include <Library/CpuPlatformLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/LocalApicLib.h>
#include <Register/GenerationMsr.h>
#include <Library/BaseOcFruLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/CpuInfoFruLib.h>
#include <Ppi/Wdt.h>
#include <Register/Cpuid.h>
#include <OcDataHob.h>
#include <Library/OcMailboxLib.h>
#include <OcLimits.h>
#include <Register/B2pMailbox.h>
#include <Guid/MpInformation2.h>

GLOBAL_REMOVE_IF_UNREFERENCED UINT8                          mAtomClusterFlag[MAX_OC_ATOM_CLUSTERS_ID] = {0};
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                          mGranularityUnits = 0;
GLOBAL_REMOVE_IF_UNREFERENCED UINT8                          mCoreTypeForModuleIndex[MAX_OC_CCP_MODULES] = {0};
GLOBAL_REMOVE_IF_UNREFERENCED OC_MODULE_INFO                 mOcModuleAndDieInfo[MAX_CDIE][MAX_OC_MODULE_ID] = {0};
STATIC BOOLEAN                                               mCDiePresenceFlag[MAX_CDIE] = {FALSE, FALSE};
GLOBAL_REMOVE_IF_UNREFERENCED OC_INSTANCE_NODE               *mListHead = NULL;

#define FIRST_THREAD                                         0
#define SECOND_THREAD                                        1
#define CPUID_NATIVE_MODEL_ID_INFO                           0x1A

// OCMB INSTANCEID definitions
#define LP_ID_START_BIT                                      0
#define LP_ID_BIT_COUNT                                      2
#define LP_ID_BIT_MASK                                       0x7 //111b
#define MODULE_ID_START_BIT                                  2
#define MODULE_ID_BIT_COUNT                                  6
#define DIE_ID_START_BIT                                     8
#define DIE_ID_BIT_COUNT                                     3


typedef struct {
  UINT32           MailboxData;
  UINT32           MailboxStatus;
} PROCESSOR_MAILBOX_DATA;

///
/// Encapsulate all information AP procedure needs into a structure.
/// A pointer to the structure will be passed to AP procedure.
///
typedef struct {
  OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig;
  EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi;
  UINTN                        BspNumber;
} PER_CORE_SETTING_CONTEXT;


/**
  Add note to the Module linked-list for each enabled module to save it's InstanceID and CoreType.
  This list will be used by all the OCMB commands that needs IstanceID as a mailbox parameter.

  @param[in] DieId          - DieID of a Module
  @param[in] ModuleId       - ModuleID of a Module
  @param[in] InstanceId     - InstanceID of a Module
  @param[in] CoreType       - CoreType of a Module

**/
VOID
AddToList (
UINT8                         DieId,
UINT8                         ModuleId,
UINT16                        InstanceId,
UINT8                         CoreType
)
{
  OC_INSTANCE_NODE               *NewNode;
  OC_INSTANCE_NODE               *temp;

  NewNode = AllocateZeroPool (sizeof (OC_INSTANCE_NODE));
  if (NewNode == NULL) {
    return;
  }
  // Initialize Node elements
  NewNode->DieId        = DieId;
  NewNode->ModuleId     = ModuleId;
  NewNode->InstanceId   = InstanceId;
  NewNode->CoreType     = CoreType;
  NewNode->NextNodePtr  = NULL;

  if (mListHead == NULL) { //add first node
    mListHead = NewNode;
  } else { //add new node to the end
    temp = (OC_INSTANCE_NODE*) mListHead;
    while (temp->NextNodePtr != NULL) {
      temp = (OC_INSTANCE_NODE*) temp->NextNodePtr;
    }
    temp->NextNodePtr = NewNode;
  }
}

/**
  Print all the nodes added in the Module linked-list.
**/
VOID
PrintList (
  VOID
)
{
  OC_INSTANCE_NODE               *temp;
  if (mListHead == NULL) {
    DEBUG (( DEBUG_INFO, "(OC) List is empty\n"));
    return;
  }
  temp = mListHead;
  while (temp != NULL) {
    DEBUG (( DEBUG_INFO, "(OC) Node DieId is %d ModuleId is %d InstanceId is 0x%02x and CoreType is 0x%x\n",
    temp->DieId, temp->ModuleId, temp->InstanceId, temp->CoreType));
    temp = temp->NextNodePtr;
  }
}

/**
  Release all the allocated memory resources
**/
VOID
RemoveList (
  VOID
)
{
  OC_INSTANCE_NODE               *temp;
  if (mListHead == NULL) {
    DEBUG (( DEBUG_INFO, "(OC) List is empty\n"));
    return;
  }
  //free-up all the nodes
  while (mListHead != NULL) {
    temp = mListHead;
    mListHead = mListHead->NextNodePtr;
    DEBUG (( DEBUG_INFO, "(OC) Removing Node 0x%x\n", temp));
    FreePool (temp);
  }
  DEBUG (( DEBUG_INFO, "(OC) List resorces are freed up\n"));
}


/**
  Get processor minimum "max non-turbo" Ratio (P1).

  @param[in] MpServices2Ppi     - Pointer to this instance of the MpServices.

  @retval Processor minimum "max non-turbo" Ratio (P1)
**/
UINT8
GetMinP1Ratio (
  IN EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi
  )
{
  EFI_STATUS                      Status;
  UINTN                           NumberOfProcessors;
  UINTN                           NumberOfEnabledProcessors;
  EFI_PROCESSOR_INFORMATION       *ProcessorInfo;
  UINT8                           *P1Ratio;
  UINTN                           Index;
  UINT8                           MinP1Ratio;

  ProcessorInfo = NULL;
  P1Ratio       = NULL;
  MinP1Ratio    = 0xFF;

  //
  // Get NumberOfProcessors
  //
  Status = MpServices2Ppi->GetNumberOfProcessors (
                             MpServices2Ppi,
                             &NumberOfProcessors,
                             &NumberOfEnabledProcessors
                             );
  ASSERT_EFI_ERROR (Status);

  //
  // Allocate buffer for ProcessorInfo & P1Ratio
  //
  ProcessorInfo = (EFI_PROCESSOR_INFORMATION *) AllocatePages (EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (EFI_PROCESSOR_INFORMATION)));
  if (ProcessorInfo == NULL) {
    ASSERT (FALSE);
  }

  P1Ratio = (UINT8 *) AllocatePages (EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (UINT8)));
  if (P1Ratio == NULL) {
    ASSERT (FALSE);
  }
  ASSERT (ProcessorInfo != NULL && P1Ratio != NULL);
  if (ProcessorInfo != NULL && P1Ratio != NULL) {
    DEBUG ((DEBUG_INFO, "Collect the ProcessorInfo and P1Ratio. \n"));
    GetProcessorInfoAndCoreType (ProcessorInfo , NULL, NumberOfProcessors);

    //
    // Get Processor Min P1 Ratio
    //
    GetP1RatioFru (ProcessorInfo, P1Ratio, NumberOfProcessors);

    //
    // Find the smallest one
    //
    for (Index = 0; Index < NumberOfProcessors; Index++) {
      if (MinP1Ratio > P1Ratio[Index]) {
        MinP1Ratio = P1Ratio[Index];
      }
    }
  }
  FreePages (ProcessorInfo, EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (EFI_PROCESSOR_INFORMATION)));
  FreePages (P1Ratio, EFI_SIZE_TO_PAGES (NumberOfProcessors * sizeof (UINT8)));

  return MinP1Ratio;
}

/**
  Create and Initialize OC Data Hob

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

**/
VOID
CreateAndInitOcDataHob (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  VOID                       *Hob;
  OC_DATA_HOB                OcDataHob;
  OC_DATA_HOB                *OcDataHobPtr;
  OC_MAILBOX_INTERFACE       OcMailboxReadCommand;
  OCMB_VR_TOPOLOGY_DATA      VrMailboxData;
  OCMB_ICCMAX_DATA           IccMaxMailboxData;
  UINT32                     MailboxStatus;
  EFI_STATUS                 Status;

  DEBUG ((DEBUG_INFO, "(OC) Create and Initialize OcDataHob Start\n"));
  ///
  /// Initial OC Hob data
  ///
  ZeroMem (&OcDataHob, sizeof (OC_DATA_HOB));

  OcDataHob.OverclockingEnabled = (UINT8) OverClockingConfig->OcSupport;

  //
  // Read the current values of ICC Max
  // Read the VR topology to get the VR address and VR type
  // Param1 and Param2 are zero
  //
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_VR_TOPOLOGY;

  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &VrMailboxData.Data, &MailboxStatus);
  if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. EFI Status is = %r, MailboxStatus is 0x%x \n", Status, MailboxStatus));
  }
  DEBUG ((DEBUG_INFO, "(OC) IA VR Address is 0x%x and IA VR Type is %d\n", VrMailboxData.Fields.IaVrAddress, VrMailboxData.Fields.IaVrType));
  DEBUG ((DEBUG_INFO, "(OC) GT VR Address is 0x%x and GT VR Type is %d\n", VrMailboxData.Fields.GtVrAddress, VrMailboxData.Fields.GtVrType));
  DEBUG ((DEBUG_INFO, "(OC) SA VR Address is 0x%x and SA VR Type is %d\n", VrMailboxData.Fields.SaVrAddress, VrMailboxData.Fields.SaVrType));

  //
  // Read current value of respective VR based ICC Max
  //
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_ICCMAX;

  if (VrMailboxData.Fields.IaVrType == SVID_VR) { // Read current value of IA VR ICC Max
    IccMaxMailboxData.Data = 0;
    OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.IaVrAddress;
    Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &IccMaxMailboxData.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
      DEBUG ((DEBUG_INFO, "(OC) Current value of IA ICC Max is %d and mode is %d\n", IccMaxMailboxData.Fields.IccMaxValue, IccMaxMailboxData.Fields.UnlimitedIccMaxMode));
      OcDataHob.IaIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
    } else {
      OcDataHob.IaIccMax = OverClockingConfig->IaIccMax;
    }
  }

  if (VrMailboxData.Fields.GtVrType == SVID_VR) { // Read current value of GT VR ICC Max
    IccMaxMailboxData.Data = 0;
    OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.GtVrAddress;
    Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &IccMaxMailboxData.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
      DEBUG ((DEBUG_INFO, "(OC) Current value of GT ICC Max is %d and mode is %d\n", IccMaxMailboxData.Fields.IccMaxValue, IccMaxMailboxData.Fields.UnlimitedIccMaxMode));
      OcDataHob.GtIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
    } else {
      OcDataHob.GtIccMax = OverClockingConfig->GtIccMax;
    }
  }

  if (VrMailboxData.Fields.SaVrType == SVID_VR) { // Read current value of SA VR ICC Max
    IccMaxMailboxData.Data = 0;
    OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.SaVrAddress;
    Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &IccMaxMailboxData.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
      DEBUG ((DEBUG_INFO, "(OC) Current value of SA ICC Max is %d and mode is %d\n", IccMaxMailboxData.Fields.IccMaxValue, IccMaxMailboxData.Fields.UnlimitedIccMaxMode));
      OcDataHob.SaIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
    } else {
      OcDataHob.SaIccMax = OverClockingConfig->SaIccMax;
    }
  }

  //
  // Create HOB for Overclocking Info
  //
  Hob = BuildGuidDataHob (
          &gOcDataHobGuid,
          (VOID *) &OcDataHob,
          sizeof (OC_DATA_HOB)
          );
  ASSERT (Hob != NULL);
  OcDataHobPtr = (OC_DATA_HOB*) Hob;

  //
  // Print default HOB data
  //
  DEBUG ((DEBUG_INFO, "(OC) OcDataHob @ 0x%x\n", &OcDataHob));
  DEBUG ((DEBUG_INFO, "(OC) OcDataHobSize: 0x%x\n", sizeof (OC_DATA_HOB)));
  DEBUG ((DEBUG_INFO, "(OC) OcDataHobPtr->OverclockingEnabled %d\n", OcDataHobPtr->OverclockingEnabled));
  DEBUG ((DEBUG_INFO, "(OC) IaIccMax, GtIccMax and SaIccMax is %d  %d  %d\n", OcDataHobPtr->IaIccMax, OcDataHobPtr->GtIccMax, OcDataHobPtr->SaIccMax));

  DEBUG ((DEBUG_INFO, "(OC) Create and Initialize OcDataHob End\n"));
}

/**
  Update OC Data Hob in PreMem

  @param[in] OverClockingConfig           - A pointer to OverClockingConfig used to pass VOLTAGE_FREQUENCY_DATA

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateOcDataHobPreMem (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
)
{
  EFI_STATUS                    Status;
  OC_DATA_HOB                   *OcDataHob;
  UINT8                         VfPointIndex;
  VOID                          *Hob;
  OC_CAPABILITIES_ITEM          OcCapabilities;
  UINT32                        MailboxStatus;
  UINT8                         DomainId;
  UINT8                         MaxDomainId;

  Status = EFI_SUCCESS;
  MaxDomainId = MAILBOX_OC_DOMAIN_ID_MAX;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));
  ///
  /// Locate and update OC Data Hob
  ///
  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_INFO, "(OC) OcDataHob Not Found\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);

  ///
  /// Get OC Capabilities of the domain
  ///
  for (DomainId = 0; DomainId < MaxDomainId; DomainId++) {
    if (DomainId == MAILBOX_OC_DOMAIN_ID_RESERVED) {
      continue;
    }
    ZeroMem (&OcCapabilities, sizeof (OcCapabilities));
    Status = GetOcCapabilities (&OcCapabilities, DomainId, &MailboxStatus);
    if (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
      ///
      /// If any OC is supported on this domain, then proceed
      ///
      if (OcCapabilities.Fields.RatioOcSupported || OcCapabilities.Fields.VoltageOverridesSupported || OcCapabilities.Fields.VoltageOffsetSupported) {
        OcDataHob->MaxOcRatioLimit[DomainId] = (UINT8) OcCapabilities.Fields.MaxOcRatioLimit;
        OcDataHob->VfPointCount[DomainId] = OverClockingConfig->VfPointCount[DomainId];
        for (VfPointIndex = 0; VfPointIndex < OcDataHob->VfPointCount[DomainId]; VfPointIndex++) {
          OcDataHob->VfPointRatio[DomainId][VfPointIndex]  = OverClockingConfig->VfPointRatio[DomainId][VfPointIndex];
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return Status;
}

/**
  Create OC Instance ID
  Read the processor information for DieId, ModuleId, ApicId, CoreType etc. from the
  MpInfoHob which is populated in CpuMpPei lib.
  Populate the global structure with required information such as InstanceID, CoreType etc.
  Also create linked-list of enabled modules.
  These global structures will be used by multiple OCMB commands that needs InstanceID as a parameter.
**/
VOID
CreateInstanceId (
  VOID
)
{
  UINT8                         LpId;
  UINT8                         ModuleId;
  UINT8                         DieId;
  UINT8                         Index;
  UINT8                         ModuleIndex;
  VOID                          *Hob;
  MP_INFORMATION2_HOB_DATA      *MpInfoHobData;
  MP_INFORMATION2_ENTRY         *MpInformation2Entry;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));
  ///
  /// Get Mp Information Hob
  ///
  Hob = GetFirstGuidHob (&gMpInformation2HobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "MpInformation2HobData not available\n"));
    ASSERT (Hob != NULL);
    return;
  }
  MpInfoHobData = (MP_INFORMATION2_HOB_DATA *) GET_GUID_HOB_DATA (Hob);

  DEBUG ((DEBUG_INFO, "(OC) NumberOfProcessors = %d\n", MpInfoHobData->NumberOfProcessors));
  for (Index = 0; Index < MpInfoHobData->NumberOfProcessors; Index++) {
    MpInformation2Entry = GET_MP_INFORMATION_ENTRY(MpInfoHobData, Index);
    ModuleId = (UINT8) MpInformation2Entry->ProcessorInfo.ExtendedInformation.Location2.Module;
    DieId    = (UINT8) MpInformation2Entry->ProcessorInfo.ExtendedInformation.Location2.Die;

    // C-Die IDs will be 0 and 1 and Hub-die ID will be 2. Hub die cores will not support overclocking
    if (DieId >= MAX_CDIE) {
      DEBUG (( DEBUG_INFO,"(OC) Overclocking is not supported on this Die ID %d\n", DieId));
      continue;
    }
    if (mOcModuleAndDieInfo[DieId][ModuleId].CoreType != 0) {
      continue; // CoreType information is already captured for this ModuleId and DieID
    }
    mCDiePresenceFlag[DieId] = TRUE;  // no harm in updatig this bit multiple times (for every module) instead of condition check.

    // get the core-type information
    mOcModuleAndDieInfo[DieId][ModuleId].CoreType = MpInformation2Entry->CoreType;
    if (MpInformation2Entry->CoreType == 0x40) { // P-core
      LpId  = (UINT8) MpInformation2Entry->ProcessorInfo.ProcessorId & LP_ID_BIT_MASK; // For P-core bit[2:1] is always 0 and bit0 is LpId
    } else if (MpInformation2Entry->CoreType == 0x20) { // E-core
      LpId  = (UINT8) (MpInformation2Entry->ProcessorInfo.ProcessorId & LP_ID_BIT_MASK) >> 1; // For E-core bit[2:1] is core ID within module and bit0 is always 0
    } else {
      LpId = 0; // LpId 0 will also be valid for both P-core and E-core tyep for a respective module
    }
    mOcModuleAndDieInfo[DieId][ModuleId].LpId     = LpId;

    // Now formulate InstanceId
    mOcModuleAndDieInfo[DieId][ModuleId].InstanceId = ((LpId & (~(0xFFFFFFFF << LP_ID_BIT_COUNT))) << LP_ID_START_BIT) |
               ((ModuleId & (~(0xFFFFFFFF << MODULE_ID_BIT_COUNT))) << MODULE_ID_START_BIT) |
               ((DieId & (~(0xFFFFFFFF << DIE_ID_BIT_COUNT))) << DIE_ID_START_BIT) ;

  DEBUG (( DEBUG_INFO, "Processor[%02d]: ApicId = 0x%x, ModuleId = 0x%x, DieId = 0x%x, LpId = 0x%x \n", Index,
      MpInformation2Entry->ProcessorInfo.ProcessorId, ModuleId, DieId, LpId));
  } // end of for loop for NumberOfProcessors

  // Create linked-list of all the enabled modules and store InstanceID and CoreType for the enabled modules.
  for (Index = C_DIE_0; Index< MAX_CDIE; ++Index) {
    for (ModuleIndex = 0; ModuleIndex < MAX_OC_CCP_MODULES; ++ModuleIndex) {
      if (mOcModuleAndDieInfo[Index][ModuleIndex].CoreType == 0) {
        continue;
      }
      // Craete linked-list of enabled modules
      AddToList(Index, ModuleIndex, mOcModuleAndDieInfo[Index][ModuleIndex].InstanceId, mOcModuleAndDieInfo[Index][ModuleIndex].CoreType);
      // Print collected information
      DEBUG (( DEBUG_INFO, "(OC) DieId = 0x%x, ModuleId = 0x%x, LpId = 0x%x InstanceId = 0x%x, CoreType = 0x%x\n",
      Index, ModuleIndex, mOcModuleAndDieInfo[Index][ModuleIndex].LpId, mOcModuleAndDieInfo[Index][ModuleIndex].InstanceId, mOcModuleAndDieInfo[Index][ModuleIndex].CoreType));
    }
  }
  PrintList ();
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return;
}


/**
  Update OC Data Hob in PostMem

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateOcDataHobPostMem (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig,
  IN EDKII_PEI_MP_SERVICES2_PPI *MpServices2Ppi
)
{
  EFI_STATUS                    Status;
  OC_DATA_HOB                   *OcDataHob;
  VOID                          *Hob;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));
  ///
  /// Locate and update OC Data Hob
  ///
  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_INFO, "(OC) OcDataHob Not Found\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);

  ///
  /// Initialize GranularRatio Units Per Module
  ///
  OcDataHob->GranularityUnits = mGranularityUnits;

  ///
  /// Initialize Processor MinP1Ratio
  ///
  OcDataHob->MinP1Ratio = GetMinP1Ratio (MpServices2Ppi);

  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return Status;
}

/**
  Initializes TURBO Control configuration.

  @param[in] OverClockingConfig    Pointer to CPU Overclocking Policy instance

**/
VOID
EFIAPI
MiscTurboConfig (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  EFI_STATUS                      Status;
  UINT32                          MailboxStatus;
  MISC_TURBO_CONTROL              MiscTurboControl;
  OC_MAILBOX_INTERFACE            OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE            OcMailboxWriteCommand;
  BOOLEAN                         ConfigChanged;

  Status        = EFI_SUCCESS;
  MailboxStatus = EFI_SUCCESS;
  ConfigChanged = FALSE;

  ///
  /// Program Ring Downbin feature if needed
  ///
  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_MISC_TURBO_CONTROL;
  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_MISC_TURBO_CONTROL;

  ZeroMem (&MiscTurboControl, sizeof (MISC_TURBO_CONTROL));
  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &MiscTurboControl.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    ///
    /// Ring Downbin mailbox command uses reverse encoding. 0 - Enable, 1 - Disable
    ///
    if (MiscTurboControl.Fields.RingDownbinDisable == OverClockingConfig->RingDownBin) {
      DEBUG ((DEBUG_INFO, "(OC) Current RingDownBin   = 0x%X\n", ~(MiscTurboControl.Fields.RingDownbinDisable)));
      DEBUG ((DEBUG_INFO, "(OC) Requested RingDownBin = 0x%X\n", OverClockingConfig->RingDownBin));
      MiscTurboControl.Fields.RingDownbinDisable = ~(OverClockingConfig->RingDownBin);
      ConfigChanged = TRUE;
    }
    ///
    /// Program Thermal Velocity Boost (TVB) features
    /// Thermal Turbo and Negative ITD programming.
    /// Mailbox encoding is reversed, 0 = Enable, 1 = Disable.
    ///
    if (MiscTurboControl.Fields.ThermalTurboDisable == OverClockingConfig->ThermalTurbo) {
      DEBUG ((DEBUG_INFO, "(OC) Current ThermalTurbo   = 0x%X\n", ~(MiscTurboControl.Fields.ThermalTurboDisable)));
      DEBUG ((DEBUG_INFO, "(OC) Requested ThermalTurbo = 0x%X\n", OverClockingConfig->ThermalTurbo));
      MiscTurboControl.Fields.ThermalTurboDisable = ~(OverClockingConfig->ThermalTurbo);
      ConfigChanged = TRUE;
    }
    if (MiscTurboControl.Fields.NegativeItdDisable == OverClockingConfig->NegativeItd) {
      DEBUG ((DEBUG_INFO, "(OC) Current NegativeItd   = 0x%X\n", ~(MiscTurboControl.Fields.NegativeItdDisable)));
      DEBUG ((DEBUG_INFO, "(OC) Requested NegativeItd = 0x%X\n", OverClockingConfig->NegativeItd));
      MiscTurboControl.Fields.NegativeItdDisable = ~(OverClockingConfig->NegativeItd);
      ConfigChanged = TRUE;
    }

    if (ConfigChanged) {
      OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, MiscTurboControl.Data, &MailboxStatus);
      if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "(OC) Set Misc Turbo Config failed, mailbox status = 0x%x\n", MailboxStatus));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Get Misc Turbo Config failed, status = %r, mailbox status = %r\n", Status, MailboxStatus));
  }

  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Set DisableCoreMask. Disable Core mask is a bitwise indication of which core should be disabled.
  Each bit represents logical core id:
  1 - Disable
  0 - Ignored

  @param[in] DisablePerCoreMask          - Bit 0 - core 0, bit 7 - core 7.
  @param[out] ColdResetFlag              - Whether need to do a cold reset.

**/
VOID
PeiCpuOcSetDisablePerCoreMask (
  IN UINT64            DisablePerCoreMask,
  OUT BOOLEAN          *ColdResetFlag
  )
{
  UINT32                          DisableCoreMaskLowNew;
  UINT32                          DisableCoreMaskHighNew;
  EFI_STATUS                      Status;
  UINT8                           Index;
  UINT32                          CpuStrapData[5];

  DisableCoreMaskLowNew  = 0;
  DisableCoreMaskHighNew = 0;
  //
  // Step 1: BIOS is to query existing disable core mask using B2P command MAILBOX_BIOS_CMD_READ_SOFT_STRAPS with PARAM1 for sub-cmd 0x1 to 0x5.
  // Read the current values of CPU soft straps
  //
  Status = GetCpuStrapData (&CpuStrapData[0], &CpuStrapData[1], &CpuStrapData[2], &CpuStrapData[3], &CpuStrapData[4]);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "(OC) Error Reading CPU Softstraps. EFI_STATUS = %r, \n", Status));
    return ;
  }

  for (Index = 0; Index < ARRAY_SIZE (CpuStrapData); ++Index) {
    DEBUG ((DEBUG_INFO, "(OC) Existing value of CPU soft strap is %d = 0x%x\n", Index, CpuStrapData[Index]));
  }

  DisableCoreMaskLowNew  = (UINT32) DisablePerCoreMask; //LSB 32 bits
  DisableCoreMaskHighNew = (UINT32) (DisablePerCoreMask >> 32); //MSB 32 bits
  DEBUG ((DEBUG_INFO, "(OC) New Core Disable value requested by user is LowBitMask = 0x%x and HighBitMask = 0x%x\n", DisableCoreMaskLowNew, DisableCoreMaskHighNew));
  //
  // Step 2: Resolve the required core disable mask with the current value of core bit mask.
  //
  DisableCoreMaskLowNew  |= CpuStrapData[3];
  DisableCoreMaskHighNew |= CpuStrapData[4];
  DEBUG ((DEBUG_INFO, "(OC) DisableCoreMaskLowNew 0x%x DisableCoreMaskHighNew 0x%x \n", DisableCoreMaskLowNew, DisableCoreMaskHighNew));
  //
  // Step 3: Check whether the existing core disable mask match the BIOS Setup option or not.
  //
  if ((DisableCoreMaskLowNew  != CpuStrapData[3]) || (DisableCoreMaskHighNew  != CpuStrapData[4])) {
    //
    // Step 4: BIOS is to write disable core mask using IPC command to PMCMAILBOX_OC_CMD_OC_INTERFACE.MAILBOX_OC_SUBCMD_WRITE_DISABLED_IA_CORES_MASK.
    //
    if (DisableCoreMaskLowNew  != CpuStrapData[3])  {
      DEBUG ((DEBUG_INFO, "(OC) The existing core disable mask low value doesn't match the BIOS Setup option = 0x%x / 0x%x\n",
        CpuStrapData[3], DisableCoreMaskLowNew));
      DEBUG ((DEBUG_INFO, "(OC) Sending IPC command to PMC\n"));
      Status = SetCpuStrapData (&CpuStrapData[0], &CpuStrapData[1], &CpuStrapData[2], &DisableCoreMaskLowNew, NULL);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "BIOS failed to write disable core mask.\n"));
        ASSERT_EFI_ERROR (Status);
        return ;
      }
    }
    if (DisableCoreMaskHighNew  != CpuStrapData[4]) {
      DEBUG ((DEBUG_INFO, "(OC) The existing core disable mask high value doesn't match the BIOS Setup option = 0x%x / 0x%x\n",
        CpuStrapData[4], DisableCoreMaskHighNew));
      DEBUG ((DEBUG_INFO, "(OC) Sending IPC command to PMC\n"));
      Status = SetCpuStrapData (NULL, NULL, NULL, NULL, &DisableCoreMaskHighNew);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "BIOS failed to write disable core mask.\n"));
        ASSERT_EFI_ERROR (Status);
        return ;
      }
    }
    *ColdResetFlag = TRUE;
  }
  return ;
}

/**
  Initializes CPU Overclocking miscellaneous configuration vector.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

**/
VOID
EFIAPI
OcMiscConfig (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS                Status;
  OC_MISC_CONFIG_DATA       OcMiscConfigData;
  PCODE_MAILBOX_INTERFACE   MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE   MailboxWriteCommand;
  UINT32                    MailboxStatus;
  BOOLEAN                   ConfigChanged;

  ConfigChanged = FALSE;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  OcMiscConfigData.Data = 0;

  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_OC_MISC_CONFIG;
  Status = MailboxRead (MailboxReadCommand.InterfaceData, &OcMiscConfigData.Data, &MailboxStatus);
  if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    return;
  }
  DEBUG ((DEBUG_INFO, "(OC) Current value of Misc Config Data is 0x%x\n", OcMiscConfigData.Data));

  ///
  /// Update Realtime Memory Timings if needed
  ///
  if (OcMiscConfigData.Fields.DynamicMemTimeChangeEn != OverClockingConfig->RealtimeMemoryTiming) {
    OcMiscConfigData.Fields.DynamicMemTimeChangeEn = OverClockingConfig->RealtimeMemoryTiming;
    DEBUG ((DEBUG_INFO, "(OC) Requested Realtime Memory Timing Enable status is = 0x%x\n", OcMiscConfigData.Fields.DynamicMemTimeChangeEn));
    ConfigChanged = TRUE;
  }

  ///
  /// Update Core Operating point reporting status if needed
  ///
  if (OcMiscConfigData.Fields.CoreOpPointReportingEn != OverClockingConfig->CoreOpPointReportingEn) {
    OcMiscConfigData.Fields.CoreOpPointReportingEn = OverClockingConfig->CoreOpPointReportingEn;
    DEBUG ((DEBUG_INFO, "(OC) Requested Core OpPoint Reporting Enable status is = 0x%x\n", OcMiscConfigData.Fields.CoreOpPointReportingEn));
    ConfigChanged = TRUE;
  }

  /// Check if user is disabling P-core or E-core Power density throttling
  /// Once changed it is applied with immediate effect and it remains in the modified state till next warm or cold reset.
  /// Mailbox parameter is to disable the option and thus have reserve encoding, 0: Enabled(Default), 1: Disabled
  if (OverClockingConfig->PcorePowerDensityThrottle == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Requested value of P-core Power Density Throttling is  %X\n", OverClockingConfig->PcorePowerDensityThrottle));
    OcMiscConfigData.Fields.PcorePowerDensityThrottleDis = 1;
    ConfigChanged = TRUE;
  }
  if (OverClockingConfig->EcorePowerDensityThrottle == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Requested value of E-core Power Density Throttling is  %X\n", OverClockingConfig->EcorePowerDensityThrottle));
    OcMiscConfigData.Fields.EcorePowerDensityThrottleDis = 1;
    ConfigChanged = TRUE;
  }

  if (ConfigChanged == TRUE) {
    MailboxWriteCommand.InterfaceData = 0;
    MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_OC_MISC_CONFIG;
    Status = MailboxWrite (MailboxWriteCommand.InterfaceData, OcMiscConfigData.Data, &MailboxStatus);
    if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_INFO, "(OC) Set OC Misc Config command failed, mailbox status = 0x%X\n", MailboxStatus));
      return;
    } else {
      // Read after write to confirm the BIOS programming
      Status = MailboxRead (MailboxReadCommand.InterfaceData, &OcMiscConfigData.Data, &MailboxStatus);
      if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        return;
      }
      DEBUG ((DEBUG_INFO, "(OC) Read after Write value of Misc Config Data is 0x%x\n", OcMiscConfigData.Data));
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Program C-Die TJMAX offset.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
TjMaxOffset (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS                     Status;
  UINT32                         MailboxStatus;
  PCODE_MAILBOX_INTERFACE        MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE        MailboxWriteCommand;
  UINT32                         CurrentTjMaxOffset;
  UINT32                         RequestedTjMaxOffset;
  BOOLEAN                        WarmResetFlag;

  ///
  /// Read the current TjMax Offset
  ///
  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  WarmResetFlag = FALSE;
  CurrentTjMaxOffset = 0;
  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_TJ_MAX_OFFSET;
  Status = MailboxRead (MailboxReadCommand.InterfaceData, &CurrentTjMaxOffset, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    RequestedTjMaxOffset = (UINT8) OverClockingConfig->TjMaxOffset;
    DEBUG ((DEBUG_INFO, "(OC) Current TjMax Offset  = 0x%x\n", CurrentTjMaxOffset));
    //
    // If needed, write the new TjMax Offset. 0 indicates no override / keep hardware defaults.
    //
    if ((RequestedTjMaxOffset) && (RequestedTjMaxOffset != CurrentTjMaxOffset)) {
      DEBUG ((DEBUG_INFO, "(OC) Current TjMax Offset  = 0x%x\n", CurrentTjMaxOffset));
      DEBUG ((DEBUG_INFO, "(OC) Requested TjMax Offset  = 0x%x\n", RequestedTjMaxOffset));

      //
      // Check if TjMax Offset is within acceptable range of 10 to 63.
      //
      if ((RequestedTjMaxOffset >= 10) && (RequestedTjMaxOffset <= 63)) {
        MailboxWriteCommand.InterfaceData = 0;
        MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
        MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_TJ_MAX_OFFSET;
        Status = MailboxWrite (MailboxWriteCommand.InterfaceData, RequestedTjMaxOffset, &MailboxStatus);
        if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
          DEBUG ((DEBUG_ERROR, "(OC) Failed to change TjMax Offset to 0x%x, mailbox status = 0x%X\n", RequestedTjMaxOffset, MailboxStatus));
        } else {
          WarmResetFlag = TRUE;
          DEBUG ((DEBUG_INFO, "(OC) Reading the TjMax Value set by write command\n"));
          Status = MailboxRead (MailboxReadCommand.InterfaceData, &CurrentTjMaxOffset, &MailboxStatus);
          if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
            DEBUG ((DEBUG_INFO, "(OC) Current TjMax Offset  = 0x%x\n", CurrentTjMaxOffset));
          }
        }
      } else {
        DEBUG ((DEBUG_INFO, "(OC) Requested TjMax Offset is out of range and will not be applied. Valid Range 10 to 63.\n"));
      }
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return WarmResetFlag;
}

/**
  Program CPU Die PLL trim offset value.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
EFIAPI
PllVccTrimOffset (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS                     Status;
  UINT32                         CurrentPllOffset;
  UINT32                         RequestedPllOffset;
  UINT32                         ReadbackPllOffset;
  PCODE_MAILBOX_INTERFACE        MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE        MailboxWriteCommand;
  UINT32                         MailboxStatus;
  UINT8                          DomainId;
  BOOLEAN                        WarmResetFlag;

  //
  // Initialize
  //
  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  RequestedPllOffset  = 0x00;
  WarmResetFlag       = FALSE;

  for (DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE; DomainId <= MAILBOX_OC_DOMAIN_ID_CPU_SA; DomainId++ ) {
    if ((DomainId == MAILBOX_OC_DOMAIN_ID_GT) || (DomainId == MAILBOX_OC_DOMAIN_ID_RESERVED) ||
        (DomainId == MAILBOX_OC_DOMAIN_ID_COMPUTE_SAF)) {
      continue;
    }

    MailboxReadCommand.InterfaceData  = 0x00;
    MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_PLL_VCC_TRIM_OFFSET;

    MailboxWriteCommand.InterfaceData = 0x00;
    MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_PLL_VCC_TRIM_OFFSET;

    switch (DomainId) {
      case MAILBOX_OC_DOMAIN_ID_IA_CORE:
        RequestedPllOffset = OverClockingConfig->CorePllVoltageOffset;
        MailboxReadCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_CORE;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_CORE;
      break;

      case MAILBOX_OC_DOMAIN_ID_RING:
        RequestedPllOffset = OverClockingConfig->RingPllVoltageOffset;
        MailboxReadCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_RING;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_RING;
      break;

      case MAILBOX_OC_DOMAIN_ID_IA_ATOM:
        RequestedPllOffset = OverClockingConfig->IaAtomPllVoltageOffset;
        MailboxReadCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_ATOM;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_ATOM;
      break;

      default:
      break;
    }
    ///
    /// Read the current PLL voltage offset
    ///
    Status = MailboxRead (MailboxReadCommand.InterfaceData, &CurrentPllOffset, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
      ///
      /// If needed, write the new PLL voltage offset
      ///
      if (RequestedPllOffset != CurrentPllOffset) {
        DEBUG ((DEBUG_INFO, "(OC) Current Pll offset for domain %d  = 0x%X\n", DomainId, CurrentPllOffset));
        DEBUG ((DEBUG_INFO, "(OC) Requested Pll offset for domain %d = 0x%X\n",DomainId, RequestedPllOffset));
        Status = MailboxWrite (MailboxWriteCommand.InterfaceData, RequestedPllOffset, &MailboxStatus);
        if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
          DEBUG ((DEBUG_INFO, "(OC) Write Pll message failed for Domain %d, mailbox status = 0x%X\n", DomainId, MailboxStatus));
        } else {
          MailboxRead (MailboxReadCommand.InterfaceData, &ReadbackPllOffset, &MailboxStatus);
          DEBUG ((DEBUG_INFO, "(OC) Readback Pll offset for domain %d = 0x%X\n", DomainId, ReadbackPllOffset));
          WarmResetFlag = TRUE;
        }
      }
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return WarmResetFlag;
}

/**
  Overrides PVD Threshold Ratio Configuration.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
EFIAPI
PvdRatioThresholdOverride (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  PCODE_MAILBOX_INTERFACE  MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE  MailboxWriteCommand;
  PVD_OVERRIDE             MailboxData;
  UINT32                   MailboxStatus;
  BOOLEAN                  OverrideFlag;
  BOOLEAN                  WarmResetFlag;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_PVD_RATIO_THRESHOLD_OVERRIDE;

  MailboxWriteCommand.InterfaceData = 0;
  MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_PVD_RATIO_THRESHOLD_OVERRIDE;

  OverrideFlag     = FALSE;
  WarmResetFlag    = FALSE;
  MailboxData.Data = 0;

  ///
  /// When zero, PLL selects static ratios as defined by PVD Mode, if the PVD Mode is supported, else default PVD ratio threshold is used.
  /// When non-zero, defines the ratio between VCO and output clock.
  ///
  Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "(OC) Current PVD Ratio Threshold value is %d \n", MailboxData.Fields.PvdRatioThreshold));
    DEBUG ((DEBUG_INFO, "(OC) Current PVD Mode is %d \n", MailboxData.Fields.PvdMode));
    if ((OverClockingConfig->PvdRatioThreshold == 0) && (MailboxData.Fields.PvdMode != OverClockingConfig->PvdMode)) {
      DEBUG ((DEBUG_INFO, "(OC) Modifying Static PVD ratio Specified by the PVD Mode as per the policy to %d\n",OverClockingConfig->PvdMode));
      MailboxData.Fields.PvdRatioThreshold = 0;
      MailboxData.Fields.PvdMode = OverClockingConfig->PvdMode;
      OverrideFlag = TRUE;
    } else if (MailboxData.Fields.PvdRatioThreshold != OverClockingConfig->PvdRatioThreshold) {
      DEBUG ((DEBUG_INFO, "(OC) Modifying PVD Ratio Threshold value as per the policy to %d\n",OverClockingConfig->PvdRatioThreshold));
      MailboxData.Fields.PvdRatioThreshold = OverClockingConfig->PvdRatioThreshold;
      OverrideFlag = TRUE;
    }
    if (OverrideFlag == TRUE) {
      Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        // Request will take effect after warm reset.
        DEBUG ((DEBUG_INFO, "(OC) Setting Warm Reset flag for the PVD Ratio change\n"));
        WarmResetFlag = TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox Read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return WarmResetFlag;
}

/**
  Program CPU D2D Configuration/Ratio.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
EFIAPI
D2dConfig (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS                     Status;
  PCODE_MAILBOX_INTERFACE        MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE        MailboxWriteCommand;
  UINT32                         MailboxData;
  UINT32                         MailboxStatus;
  BOOLEAN                        WarmResetFlag;

  //
  // Program CPU D2D Ratio
  //
  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  WarmResetFlag = FALSE;

  if (OverClockingConfig->CpuD2dRatio != 0) {
    MailboxReadCommand.InterfaceData = 0;
    MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_D2D_CONFIG;
    Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS) && (MailboxData != OverClockingConfig->CpuD2dRatio)) {
      DEBUG ((DEBUG_INFO, "(OC) Current CPU D2D Ratio = 0x%x\n", MailboxData));
      MailboxData = OverClockingConfig->CpuD2dRatio;
      DEBUG ((DEBUG_INFO, "(OC) Set CPU D2D Ratio = 0x%x\n", MailboxData));

      MailboxWriteCommand.InterfaceData = 0;
      MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
      MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_D2D_CONFIG;
      Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        // Request will take effect after warm reset.
        DEBUG ((DEBUG_INFO, "(OC) Issuing Warm Reset for the CPU D2D Ratio change\n"));
        WarmResetFlag = TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Set CPU D2D Ratio Failed. EFI Status = 0x%x, Mailbox Status = 0x%x\n", Status, MailboxStatus));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "(OC) Mailbox Read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return WarmResetFlag;
}

/**
  Initializes FLL Ratio Threshold for all domains.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
EFIAPI
FllOverride (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  PCODE_MAILBOX_INTERFACE  MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE  MailboxWriteCommand;
  FLL_OVERRIDE             MailboxData;
  UINT32                   MailboxStatus;
  BOOLEAN                  WarmResetFlag;

  WarmResetFlag = FALSE;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  MailboxData.Data = 0;
  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_FLL_OVERRIDE;

  MailboxWriteCommand.InterfaceData = 0;
  MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_FLL_OVERRIDE;

  Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "(OC) Current FLL Mode is %d \n", MailboxData.Fields.FllOverclockMode));
    if (MailboxData.Fields.FllOverclockMode != OverClockingConfig->FllOverclockMode) {
      DEBUG ((DEBUG_INFO, "(OC) Modifying FLL Overclock Mode as per the policy to %d\n", OverClockingConfig->FllOverclockMode));
      MailboxData.Fields.FllOverclockMode = OverClockingConfig->FllOverclockMode;
      Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        // Request will take effect after warm reset.
        DEBUG ((DEBUG_INFO, "(OC) Setting Warm Reset flag for the FLL Overclock Mode change\n"));
        WarmResetFlag = TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
      }
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
  return WarmResetFlag;
}

/**
  Initializes OC Persistent Overrides
  For the overrides, request will take effect after warm reset. Each bit set will persist until the next cold boot.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
OcPersistentOverrides (
  IN OVERCLOCKING_PREMEM_CONFIG  *OverClockingConfig
  )
{
  EFI_STATUS                     Status;
  PCODE_MAILBOX_INTERFACE        MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE        MailboxWriteCommand;
  OC_PERSISTENT_OVERRIDES_DATA   MailboxData;
  UINT32                         MailboxStatus;
  BOOLEAN                        SettingsChanged;
  BOOLEAN                        SettingNeedWarmReset;
  BOOLEAN                        WarmResetFlag;

  SettingsChanged       = FALSE;
  SettingNeedWarmReset  = FALSE;
  WarmResetFlag         = FALSE;

  DEBUG ((DEBUG_INFO, "(OC) Setting Oc Persistent Overrides\n"));
  //
  // BIT 5: Disable Core HW Fixup during TSC copy from PMA to APIC
  // BIT 6: Disable Process VMAX Limit
  // BIT 7: Enable Negative Temperature Reporting
  //
  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_OC_PERSISTENT_OVERRIDES;
  MailboxWriteCommand.InterfaceData = 0;
  MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_OC_PERSISTENT_OVERRIDES;

  Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
  if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
    return WarmResetFlag;
  }
  DEBUG ((DEBUG_INFO, "(OC) Oc Persistent Overrides read data is 0x%x\n", MailboxData.Data));

  //
  // Check the status of TSC_DISABLE_HW_FIXUP bit
  //
  if (MailboxData.Fields.TscDisableHwFixup != OverClockingConfig->TscDisableHwFixup) {
    DEBUG ((DEBUG_INFO, "(OC) Modifying TSC HW Fixup disable bit as per the policy \n"));
    MailboxData.Fields.TscDisableHwFixup = OverClockingConfig->TscDisableHwFixup;
    SettingsChanged      = TRUE;
    SettingNeedWarmReset = TRUE;
  }
  //
  // Check if user is disabling the Process Vmax Limit
  // Mailbox parameter is to disable the option and thus have reserve encoding, 0: Enabled(Default), 1: Disabled
  // Hence requested value of 1 for the mailbox data, means disable the Process Vmax Limit.
  // Once it is disabled, it remains disabled till next cold reset.
  //
  if ((OverClockingConfig->ProcessVmaxLimit == 0) && (MailboxData.Fields.DisableProcessVmaxLimit != 1)) {
    DEBUG ((DEBUG_INFO, "(OC) Modifying Process Vmax Limit Disable bit as per the policy \n"));
    MailboxData.Fields.DisableProcessVmaxLimit = 1;
    SettingsChanged = TRUE;
  }
  //
  // Check the status of ENABLE_NEGATIVE_TEMPERATURE_REPORTING bit. Enable this bit to report temperature below 0 degree Celsius.
  // BIOS can only set this bit, it cannot be cleared by mailbox command. This setting persists over warm reset, and cleared on cold reset.
  //
  if ((OverClockingConfig->NegativeTemperatureReporting != MailboxData.Fields.NegativeTemperatureReporting) && (MailboxData.Fields.NegativeTemperatureReporting == 0)) {
    DEBUG ((DEBUG_INFO, "(OC) Modifying Negative Temperature Reporting Enable bit as per the policy \n"));
    MailboxData.Fields.NegativeTemperatureReporting = OverClockingConfig->NegativeTemperatureReporting;
    SettingsChanged = TRUE;
  }

  if (SettingsChanged == TRUE) {
    Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
    if ((Status != EFI_SUCCESS) || (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
      if (SettingNeedWarmReset == TRUE) {
        // Request will take effect after warm reset.
        DEBUG ((DEBUG_INFO, "(OC) Issuing Warm Reset for the Mode change\n"));
        WarmResetFlag = TRUE;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. Mailbox command return status 0x%x \n", MailboxStatus));
    }
  }
  return WarmResetFlag;
}

/**
  Limit Core minimum ratio for extreme overclocking.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
IaMinRatio (
  IN OVERCLOCKING_PREMEM_CONFIG  *OverClockingConfig
  )
{
  EFI_STATUS                    Status;
  PCODE_MAILBOX_INTERFACE       MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE       MailboxWriteCommand;
  UINT32                        MailboxData;
  UINT32                        MailboxStatus;
  BOOLEAN                       WarmResetFlag;

  WarmResetFlag = FALSE;

  //
  // Limit Core minimum ratio for extreme overclocking
  //
  if (OverClockingConfig->CoreMinRatio != 0) {
    MailboxReadCommand.InterfaceData = 0;
    MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_IA_MIN_RATIO;
    Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS) && (MailboxData != OverClockingConfig->CoreMinRatio)) {
      DEBUG ((DEBUG_INFO, "(OC) Current Core Min Ratio = 0x%x\n", MailboxData));
      MailboxData = OverClockingConfig->CoreMinRatio;
      DEBUG ((DEBUG_INFO, "(OC) Request Core Min Ratio = 0x%x\n", MailboxData));

      MailboxWriteCommand.InterfaceData = 0;
      MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
      MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_IA_MIN_RATIO;
      Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        //
        // Request will take effect after warm reset.
        //
        DEBUG ((DEBUG_INFO, "(OC) Issuing Warm Reset for the Core Min Ratio change\n"));
        WarmResetFlag = TRUE;
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Request Core Min Ratio Failed. EFI Status = 0x%x, Mailbox Status = 0x%x\n", Status, MailboxStatus));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "(OC) Mailbox Read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
    }
  }
  return WarmResetFlag;
}

/**
  Configure the Pll IReftune offset.

  @param[in] OverClockingConfig - A pointer to OverClockingConfig used to pass OVERCLOCKING_PREMEM_CONFIG

  @retval TRUE   Settings changed and need warm reset.
  @retval FALSE  Settings not changed.
**/
BOOLEAN
PllIRefTuning (
  IN OVERCLOCKING_PREMEM_CONFIG  *OverClockingConfig
  )
{
  EFI_STATUS                      Status;
  UINT32                          MailboxStatus;
  PCODE_MAILBOX_INTERFACE         MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE         MailboxWriteCommand;
  UINT32                          CurrentPllIRefTuningOffset;
  UINT32                          RequestedPllIRefTuningOffset;
  UINT32                          ReadbackPllIRefTuningOffset;
  UINT8                           DomainId;
  BOOLEAN                         WarmResetFlag;

  RequestedPllIRefTuningOffset       = 0x00;
  WarmResetFlag                      = FALSE;

  MailboxReadCommand.InterfaceData   = 0x00;
  MailboxReadCommand.Fields.Command  = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1   = MAILBOX_OC_SUBCMD_READ_PLL_IREFTUNE;

  MailboxWriteCommand.InterfaceData  = 0x00;
  MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxWriteCommand.Fields.Param1  = MAILBOX_OC_SUBCMD_WRITE_PLL_IREFTUNE;

  for (DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE; DomainId <= MAILBOX_OC_DOMAIN_ID_CPU_SA; DomainId++ ) {
    if ((DomainId == MAILBOX_OC_DOMAIN_ID_GT) || (DomainId == MAILBOX_OC_DOMAIN_ID_RESERVED) ||
        (DomainId == MAILBOX_OC_DOMAIN_ID_COMPUTE_SAF)) {
      continue;
    }

    MailboxReadCommand.InterfaceData   = 0x00;
    MailboxReadCommand.Fields.Command  = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxReadCommand.Fields.Param1   = MAILBOX_OC_SUBCMD_READ_PLL_IREFTUNE;

    MailboxWriteCommand.InterfaceData  = 0x00;
    MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxWriteCommand.Fields.Param1  = MAILBOX_OC_SUBCMD_WRITE_PLL_IREFTUNE;

    switch (DomainId) {
      case MAILBOX_OC_DOMAIN_ID_IA_CORE:
        RequestedPllIRefTuningOffset   = OverClockingConfig->CorePllCurrentRefTuningOffset;
        MailboxReadCommand.Fields.Param2  = MAILBOX_OC_DOMAIN_ID_IA_CORE;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_CORE;
      break;

      case MAILBOX_OC_DOMAIN_ID_RING:
        RequestedPllIRefTuningOffset   = OverClockingConfig->RingPllCurrentRefTuningOffset;
        MailboxReadCommand.Fields.Param2  = MAILBOX_OC_DOMAIN_ID_RING;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_RING;
      break;

      case MAILBOX_OC_DOMAIN_ID_IA_ATOM:
        RequestedPllIRefTuningOffset   = OverClockingConfig->IaAtomPllCurrentRefTuningOffset;
        MailboxReadCommand.Fields.Param2  = MAILBOX_OC_DOMAIN_ID_IA_ATOM;
        MailboxWriteCommand.Fields.Param2 = MAILBOX_OC_DOMAIN_ID_IA_ATOM;
      break;

      default:
      break;
    }

    if ((DomainId == MAILBOX_OC_DOMAIN_ID_IA_CORE) || (DomainId == MAILBOX_OC_DOMAIN_ID_RING) || (DomainId == MAILBOX_OC_DOMAIN_ID_IA_ATOM)) {
      ///
      /// Read Current Reference Tuning Offset
      ///
      Status = MailboxRead (MailboxReadCommand.InterfaceData, &CurrentPllIRefTuningOffset, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        ///
        /// If needed, write the new Current Reference Tuning Offset
        ///
        if (RequestedPllIRefTuningOffset != CurrentPllIRefTuningOffset) {
          DEBUG ((DEBUG_INFO, "(OC) Current Pll Current Reference Tuning offset for domain %d  = 0x%x\n", DomainId, CurrentPllIRefTuningOffset));
          DEBUG ((DEBUG_INFO, "(OC) Requested Pll Current Reference Tuning offset for domain %d = 0x%x\n", DomainId, RequestedPllIRefTuningOffset));
          Status = MailboxWrite (MailboxWriteCommand.InterfaceData, RequestedPllIRefTuningOffset, &MailboxStatus);
          if ((Status != EFI_SUCCESS) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
            DEBUG ((DEBUG_INFO, "(OC) Write Pll Current Reference Tuning offset message failed for Domain %d, mailbox status = 0x%X\n", DomainId, MailboxStatus));
          } else {
            MailboxRead (MailboxReadCommand.InterfaceData, &ReadbackPllIRefTuningOffset, &MailboxStatus);
            DEBUG ((DEBUG_INFO, "(OC) Readback Pll Current Reference Tuning for domain %d = 0x%X\n", DomainId, ReadbackPllIRefTuningOffset));
            WarmResetFlag = TRUE;
          }
        }
      }
    }
  }
  return WarmResetFlag;
}

/**
  Initializes various Overclocking controls with OC interface command (0x37).

  @param[in]  OverClockingConfig    Pointer to CPU Overclocking Policy instance
  @param[out] WarmResetFlag         Pointer to flag to indicate whether warm reset should be performed

**/
VOID
EFIAPI
B2pConfig (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig,
  OUT BOOLEAN                   *WarmResetFlag
  )
{

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  ///
  /// Configure OC Misc. configuration settings.
  ///
  OcMiscConfig (OverClockingConfig);

  ///
  ///  Configure TjMax offset.
  ///
  *WarmResetFlag |= TjMaxOffset (OverClockingConfig);

  ///
  /// Configure the Pll Voltage offsets for each CPU domain.
  ///
  *WarmResetFlag |= PllVccTrimOffset (OverClockingConfig);

  ///
  /// Configure the Pll IReftune offsets.
  ///
  *WarmResetFlag |= PllIRefTuning (OverClockingConfig);

  ///
  /// Configure PVD Ratio Threshold for all domains.
  ///
  *WarmResetFlag |= PvdRatioThresholdOverride (OverClockingConfig);

  ///
  /// Configure OC Persistent overrides
  ///
  *WarmResetFlag |= OcPersistentOverrides (OverClockingConfig);

  //
  // Program CPU D2D Ratio.
  //
  *WarmResetFlag |= D2dConfig (OverClockingConfig);

  //
  // Limit Core minimum ratio for extreme overclocking
  //
  *WarmResetFlag |= IaMinRatio (OverClockingConfig);

  ///
  /// Configure FLL Override for cores.
  ///
  *WarmResetFlag |= FllOverride (OverClockingConfig);

  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Configure AVX Ratio negative offset control for all AVX levels.

  @param[in]  OverClockingConfig    Pointer to CPU Overclocking Policy instance

**/
VOID
EFIAPI
AvxRatioControl (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  EFI_STATUS                      Status;
  UINT32                          MailboxStatus;
  AVX_CONTROL                     AvxControl;
  OC_MAILBOX_INTERFACE            OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE            OcMailboxWriteCommand;

  Status        = EFI_SUCCESS;
  MailboxStatus = EFI_SUCCESS;

  ///
  /// Get the current AVX ratio from OC mailbox
  ///
  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  ///
  ///  Avx Voltage Guardband Scaling
  ///
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_AVX_RATIO_OFFSET;
  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_AVX_RATIO_OFFSET;

  ZeroMem (&AvxControl, sizeof (AVX_CONTROL));
  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &AvxControl.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    if (AvxControl.Fields.AVX2RatioOffset != (UINT8) OverClockingConfig->Avx2RatioOffset) {
      DEBUG ((DEBUG_INFO, "(OC) Current AVX2 Ratio Offset   = 0x%x\n", AvxControl.Fields.AVX2RatioOffset));
      DEBUG ((DEBUG_INFO, "(OC) Requested AVX2 Ratio Offset = 0x%x\n", OverClockingConfig->Avx2RatioOffset));
      AvxControl.Fields.AVX2RatioOffset = (UINT8) OverClockingConfig->Avx2RatioOffset;
      ///
      /// Set the requested AVX ratio to OC mailbox
      ///
      OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, AvxControl.Data, &MailboxStatus);
      if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "(OC) Set AVX Ratio offset failed. , Mailbox Status = %r\n", MailboxStatus));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Get AVX Ratio offset failed. EFI Status = %r, Mailbox Status = %r\n", Status, MailboxStatus));
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Configure the AVX Voltage Guardband Scale Factor.

  @param[in]  OverClockingConfig    Pointer to CPU Overclocking Policy instance

**/
VOID
EFIAPI
AvxVoltageGuardbandScaleFactor (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  EFI_STATUS                      Status;
  OC_MAILBOX_INTERFACE            OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE            OcMailboxWriteCommand;
  AVX_VOLTAGE_SCALE               AvxVoltageScale;
  UINT32                          MailboxStatus;
  UINT8                           CurrentAvx2VoltageScale;
  UINT8                           RequestedAvx2VoltageScale;

  Status        = EFI_SUCCESS;
  MailboxStatus = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  ///
  ///  Avx Voltage Guardband Scaling
  ///
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_AVX_VOLTAGE_GUARDBAND;
  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_AVX_VOLTAGE_GUARDBAND;

  ZeroMem (&AvxVoltageScale, sizeof (AVX_VOLTAGE_SCALE));
  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &AvxVoltageScale.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    ///
    ///  Mailbox Format for AVX Voltage Scaling Command:
    ///  [15:8] Avx512 Voltage Guardband Scale Factor in U1.7 format
    ///  [7:0]  Avx2 Voltage Guardband Scale Factor in U1.7 format
    ///  AVX guard band scale factor in U1.7 format is "fixed-point representation" (U : unsigned, 1: integer, 7: fraction)
    ///  Scale factor(X) = U1.7 units (N, PCODE) = BIOS setup value (input BIOS inputs= 100 * X (scale factor))
    ///  U1.7 units N = X * 128 (128 is 2^7 = 80h) (which is for PCODE BIOS MB)
    ///  So BIOS will calculate N = ((BIOS inputs * 128) / 100)
    ///  Actually in BIOS has to consider to add or subtract rounding errors (50) depends on BIOS inputs:
    ///  in range 1 - 100
    ///    N = ((BIOS inputs * 128 + 50) / 100)
    ///  in range 101 - 200
    ///    N = ((BIOS inputs * 128 - 50) / 100)
    ///
    CurrentAvx2VoltageScale   = (UINT8) AvxVoltageScale.Fields.Avx2VoltageScaleFactor;
    DEBUG ((DEBUG_INFO, "(OC) Current values of Avx Voltage Guardband Scaling in U1.7 format is CurrentAvx2VoltageScale = %d\n", CurrentAvx2VoltageScale));

    if (OverClockingConfig->Avx2VoltageScaleFactor > 100) {
      RequestedAvx2VoltageScale   = (UINT32) (((OverClockingConfig->Avx2VoltageScaleFactor   * 128) - 50 ) / 100);
    } else {
      RequestedAvx2VoltageScale   = (UINT32) (((OverClockingConfig->Avx2VoltageScaleFactor   * 128) + 50 ) / 100);
    }

    if (CurrentAvx2VoltageScale != RequestedAvx2VoltageScale) {
      DEBUG ((DEBUG_INFO, "(OC) Requested values of Avx Voltage Guardband Scaling in U1.7 format is RequestedAvx2VoltageScale = %d\n", RequestedAvx2VoltageScale));
      AvxVoltageScale.Fields.Avx2VoltageScaleFactor = RequestedAvx2VoltageScale;
      OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, AvxVoltageScale.Data, &MailboxStatus);
      if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "(OC) Get Avx Voltage Guardband Scaling Factor failed, Mailbox Status = %r\n", MailboxStatus));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Get Avx Voltage Guardband Scaling Factor failed, EFI Status = %r, Mailbox Status = %r\n", Status, MailboxStatus));
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Configure core V/F premem settings and bclk adaptive voltage in OC global configuration.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

**/
VOID
EFIAPI
MiscGlobalConfig (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  EFI_STATUS                      Status;
  UINT32                          MailboxStatus;
  MISC_GLOBAL_CONFIG              MiscGlobalConfig;
  OC_MAILBOX_INTERFACE            OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE            OcMailboxWriteCommand;
  BOOLEAN                         ConfigChanged;

  Status        = EFI_SUCCESS;
  MailboxStatus = EFI_SUCCESS;
  ConfigChanged = FALSE;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_MISC_GLOBAL_CONFIG;
  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_MISC_GLOBAL_CONFIG;

  ZeroMem (&MiscGlobalConfig, sizeof (MISC_GLOBAL_CONFIG));
  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &MiscGlobalConfig.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    if (MiscGlobalConfig.Fields.BclkAdaptiveVoltage != OverClockingConfig->BclkAdaptiveVoltage) {
      DEBUG ((DEBUG_INFO, "(OC) Current BclkAdaptiveVoltage   = 0x%x\n", MiscGlobalConfig.Fields.BclkAdaptiveVoltage));
      DEBUG ((DEBUG_INFO, "(OC) Requested BclkAdaptiveVoltage = 0x%x\n", OverClockingConfig->BclkAdaptiveVoltage));
      MiscGlobalConfig.Fields.BclkAdaptiveVoltage = OverClockingConfig->BclkAdaptiveVoltage;
      ConfigChanged = TRUE;
    }
    if (MiscGlobalConfig.Fields.PerCoreVfOverride != OverClockingConfig->CoreVfConfigScope) {
      DEBUG ((DEBUG_INFO, "(OC) Current PerCoreVfOverride   = 0x%x\n", MiscGlobalConfig.Fields.PerCoreVfOverride));
      DEBUG ((DEBUG_INFO, "(OC) Requested CoreVfConfigScope = 0x%x\n", OverClockingConfig->CoreVfConfigScope));
      MiscGlobalConfig.Fields.PerCoreVfOverride = OverClockingConfig->CoreVfConfigScope;
      ConfigChanged = TRUE;
    }

    if (ConfigChanged) {
      OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, MiscGlobalConfig.Data, &MailboxStatus);
      if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "(OC) Set Misc Global Config Failed. Mailbox Status = %r\n", MailboxStatus));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Get Misc Global Config Failed. EFI Status = %r, Mailbox Status = %r\n", Status, MailboxStatus));
  }

  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Initializes CPU domain voltage settings for overclocking.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
CpuOcInitVoltageSettings (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS              Status;
  OC_CAPABILITIES_ITEM    OcCapabilities;
  VOLTAGE_FREQUENCY_ITEM  CurrentVfItem;
  VOLTAGE_FREQUENCY_ITEM  RequestedVfItem;
  UINT32                  MailboxStatus;
  UINT8                   DomainId;
  UINT8                   MaxDomainId;
  UINT8                   VfPointIndex;
  WDT_PPI                 *WdtPei;

  MaxDomainId             = MAILBOX_OC_DOMAIN_ID_MAX;
  Status                  = EFI_SUCCESS;
  VfPointIndex            = 0;

  WdtPei                  = NULL;

  DEBUG ((DEBUG_INFO, "(OC) %a Start \n", __FUNCTION__));

  //
  // Locate WDT_PPI (ICC WDT PPI)
  //
  Status = PeiServicesLocatePpi (
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &WdtPei
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ///
  /// We will loop on the CPU domains to manage the voltage/frequency settings
  ///
  for (DomainId = 0; DomainId < MaxDomainId; DomainId++) {
    if (DomainId == MAILBOX_OC_DOMAIN_ID_RESERVED) {
      continue;
    }
    ///
    /// Get OC Capabilities of the domain
    ///
    ZeroMem (&OcCapabilities, sizeof (OcCapabilities));
    Status = GetOcCapabilities (&OcCapabilities, DomainId, &MailboxStatus);
    if (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
      ///
      /// If any OC is supported on this domain, then proceed
      ///
      if (OcCapabilities.Fields.RatioOcSupported || OcCapabilities.Fields.VoltageOverridesSupported || OcCapabilities.Fields.VoltageOffsetSupported) {
        ///
        /// Need to populate the user requested settings from the Policy
        /// to determine if OC changes are desired.
        ///
        if (OcCapabilities.Fields.RatioOcSupported) {
          ZeroMem (&CurrentVfItem, sizeof (CurrentVfItem));
          CurrentVfItem.DomainId = DomainId;
          ///
          /// Initialize the supported VfPointCount & each VF point ratio.
          ///
          for (VfPointIndex = 0; VfPointIndex < MAX_OC_VF_POINTS; VfPointIndex++) {
            CurrentVfItem.VfSettings.VfPointIndex = VfPointIndex + 1;
            Status = GetVoltageFrequencyItem (&CurrentVfItem, &MailboxStatus);
            if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
              break;
            }
            OverClockingConfig->VfPointCount[DomainId]++;
            ///
            /// Initialize each supported VF point ratio.
            ///
            OverClockingConfig->VfPointRatio[DomainId][VfPointIndex] = CurrentVfItem.VfSettings.MaxOcRatio;
          }
          DEBUG ((DEBUG_INFO, "(OC) Domain = %d, VF Point Count = %d\n", DomainId, OverClockingConfig->VfPointCount[DomainId]));
        }
      }

      ///
      /// Populate the user requested VfSettings struct
      ///
      ZeroMem (&CurrentVfItem, sizeof (CurrentVfItem));
      CurrentVfItem.DomainId = DomainId;
      ZeroMem (&RequestedVfItem, sizeof (RequestedVfItem));
      RequestedVfItem.DomainId = DomainId;

      RequestedVfItem.VfSettings.VoltageTargetMode = (UINT8) OverClockingConfig->VoltageMode[DomainId];
      if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->AdaptiveVoltage[DomainId];
      } else {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->VoltageOverride[DomainId];
      }
      if (OverClockingConfig->VfPointOffsetMode[DomainId] == 0 && OverClockingConfig->CoreVfConfigScope == 0) {
        ///
        /// Get a copy of the current domain VfSettings from the Mailbox Library
        ///
        CurrentVfItem.VfSettings.VfPointIndex = 0;
        Status = GetVoltageFrequencyItem (&CurrentVfItem, &MailboxStatus);
        if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
          continue;
        }
        if ((UINT8) OverClockingConfig->MaxOcRatio[DomainId] > OcCapabilities.Fields.MaxOcRatioLimit) {
          RequestedVfItem.VfSettings.MaxOcRatio  = (UINT8) OcCapabilities.Fields.MaxOcRatioLimit;
        } else {
          RequestedVfItem.VfSettings.MaxOcRatio  = (UINT8) OverClockingConfig->MaxOcRatio[DomainId];
        }
          RequestedVfItem.VfSettings.VfPointIndex  = 0;
          RequestedVfItem.VfSettings.VoltageOffset = OverClockingConfig->VoltageOffset[DomainId];
          Status = UpdateVoltageFrequencyItem (&CurrentVfItem, &RequestedVfItem, WdtPei);
      } else if (OverClockingConfig->VfPointOffsetMode[DomainId] == 1) {
        ASSERT (OverClockingConfig->VfPointCount[DomainId] <= MAX_OC_VF_POINTS);
        //voltage target value should be zero when voltage offset programming is done for each VF point
        RequestedVfItem.VfSettings.VoltageTarget = 0;
        for (VfPointIndex = 0; VfPointIndex < OverClockingConfig->VfPointCount[DomainId]; VfPointIndex++) {
          ///
          /// Get a copy of the current domain VfSettings for each Vf point from the Mailbox Library
          ///
          CurrentVfItem.VfSettings.VfPointIndex = VfPointIndex + 1;
          Status = GetVoltageFrequencyItem (&CurrentVfItem, &MailboxStatus);
          if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
            continue;
          }
          RequestedVfItem.VfSettings.MaxOcRatio    = 0;
          RequestedVfItem.VfSettings.VfPointIndex  = VfPointIndex + 1; ///< Update the VF Point Index.
          RequestedVfItem.VfSettings.VoltageOffset = OverClockingConfig->VfPointOffset[DomainId][VfPointIndex]; ///< Update the VF point offset.
          Status = UpdateVoltageFrequencyItem (&CurrentVfItem, &RequestedVfItem, WdtPei);
        }
      }
    }
  }

  DEBUG ((DEBUG_INFO, "(OC) %a end \n", __FUNCTION__));

  return Status;
}

/**
  VR ICCMAX override for supported domains.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_UNSUPPORTED      If the command is not supported.
**/
EFI_STATUS
STATIC
VrIccMaxOcOverride  (
    IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
    )
{
  EFI_STATUS                     Status;
  OC_MAILBOX_INTERFACE           OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE           OcMailboxWriteCommand;
  OCMB_VR_TOPOLOGY_DATA          VrMailboxData;
  OCMB_ICCMAX_DATA               IccMaxMailboxData;
  UINT32                         MailboxStatus;
  UINT8                          DomainId;
  UINT16                         RequestedIccMaxValue;
  UINT8                          RequestedIccMaxUnlimitedMode;
  OC_DATA_HOB                    *OcDataHob;
  VOID                           *Hob;

  DEBUG ((DEBUG_INFO, "(OC) VrIccMaxOcOverride \n"));

  if ((OverClockingConfig->IaIccMax == 0)&&(OverClockingConfig->GtIccMax == 0)&&(OverClockingConfig->SaIccMax == 0)) {
    DEBUG ((DEBUG_INFO, "(OC) Vr IccMax Oc Override is not required \n"));
    return EFI_SUCCESS;
  }

  ///
  /// Get OC Data Hob
  ///
  Hob = GetFirstGuidHob (&gOcDataHobGuid);
  if (Hob == NULL) {
    DEBUG ((DEBUG_ERROR, "OC Data HOB not available\n"));
    ASSERT (Hob != NULL);
  }
  OcDataHob = GET_GUID_HOB_DATA (Hob);
  //
  // Read the VR topology to get the VR address and VR type
  // Param is zero
  //
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_VR_TOPOLOGY;

  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &VrMailboxData.Data, &MailboxStatus);
  if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "(OC) IA VR Address is 0x%x and IA VR Type is %d\n", VrMailboxData.Fields.IaVrAddress, VrMailboxData.Fields.IaVrType));
  DEBUG ((DEBUG_INFO, "(OC) GT VR Address is 0x%x and GT VR Type is %d\n", VrMailboxData.Fields.GtVrAddress, VrMailboxData.Fields.GtVrType));
  DEBUG ((DEBUG_INFO, "(OC) SA VR Address is 0x%x and SA VR Type is %d\n", VrMailboxData.Fields.SaVrAddress, VrMailboxData.Fields.SaVrType));

  //
  // Override VR ICC Max for supported domains
  //
  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_ICCMAX;

  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_ICCMAX;

  for (DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE; DomainId <= MAILBOX_OC_DOMAIN_ID_SYSTEM_AGENT; DomainId++ ) {
    IccMaxMailboxData.Data = 0; //reset the value
    if (DomainId == MAILBOX_OC_DOMAIN_ID_IA_CORE) {
      // Check if IA VR override is possible and/or required
      if ((VrMailboxData.Fields.IaVrType != SVID_VR) || (OverClockingConfig->IaIccMax == 0)) {
            continue;
      } else {
        OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.IaVrAddress;
        OcMailboxWriteCommand.Fields.Param_4_0 = (UINT8) VrMailboxData.Fields.IaVrAddress;
        RequestedIccMaxValue                = OverClockingConfig->IaIccMax;
        RequestedIccMaxUnlimitedMode        = (UINT8) OverClockingConfig->IaIccUnlimitedMode;
      }
    } else if (DomainId == MAILBOX_OC_DOMAIN_ID_GT) {
      // Check if GT VR override is possible and/or required
      if ((VrMailboxData.Fields.GtVrType != SVID_VR) || (OverClockingConfig->GtIccMax == 0)) {
        continue;
      } else {
        OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.GtVrAddress;
        OcMailboxWriteCommand.Fields.Param_4_0 = (UINT8) VrMailboxData.Fields.GtVrAddress;
        RequestedIccMaxValue                = OverClockingConfig->GtIccMax;
        RequestedIccMaxUnlimitedMode        = (UINT8) OverClockingConfig->GtIccUnlimitedMode;
      }
    } else if (DomainId == MAILBOX_OC_DOMAIN_ID_SYSTEM_AGENT) {
      // Check if SA VR override is possible and/or required
      if ((VrMailboxData.Fields.SaVrType != SVID_VR) || (OverClockingConfig->SaIccMax == 0)) {
        continue;
      } else {
        OcMailboxReadCommand.Fields.Param_4_0  = (UINT8) VrMailboxData.Fields.SaVrAddress;
        OcMailboxWriteCommand.Fields.Param_4_0 = (UINT8) VrMailboxData.Fields.SaVrAddress;
        RequestedIccMaxValue                = OverClockingConfig->SaIccMax;
        RequestedIccMaxUnlimitedMode        = (UINT8) OverClockingConfig->SaIccUnlimitedMode;
      }
    } else {
      continue;
    }
    //
    // Read current value and unlimited mode
    //
    Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &IccMaxMailboxData.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
      DEBUG ((DEBUG_INFO, "(OC) Current value for DomainId %d ICC Max is %d and mode is %d\n", DomainId, IccMaxMailboxData.Fields.IccMaxValue, IccMaxMailboxData.Fields.UnlimitedIccMaxMode));
      if ((IccMaxMailboxData.Fields.IccMaxValue != RequestedIccMaxValue) ||
          (IccMaxMailboxData.Fields.UnlimitedIccMaxMode != RequestedIccMaxUnlimitedMode)) {
        IccMaxMailboxData.Fields.IccMaxValue         = RequestedIccMaxValue;
        IccMaxMailboxData.Fields.UnlimitedIccMaxMode = RequestedIccMaxUnlimitedMode;
        //
        // Write new ICC value and unlimited mode as per the user selection
        //
        Status = OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, IccMaxMailboxData.Data, &MailboxStatus);
        if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
          //
          // Read the modified value and unlimited mode to confirm the mailbox write success
          //
          IccMaxMailboxData.Data = 0;
          DEBUG ((DEBUG_INFO, "(OC) Read after write for the ICC Max command for DomainId %d\n", DomainId));
          Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &IccMaxMailboxData.Data, &MailboxStatus);
          if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
            if (DomainId == MAILBOX_OC_DOMAIN_ID_IA_CORE) {
              OcDataHob->IaIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
            } else if (DomainId == MAILBOX_OC_DOMAIN_ID_GT) {
              OcDataHob->GtIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
            } else if (DomainId == MAILBOX_OC_DOMAIN_ID_SYSTEM_AGENT) {
              OcDataHob->SaIccMax = (UINT16) IccMaxMailboxData.Fields.IccMaxValue;
            }
            DEBUG ((DEBUG_INFO, "(OC) New value for DomainId %d ICC Max is %d and mode is %d\n", DomainId, IccMaxMailboxData.Fields.IccMaxValue, IccMaxMailboxData.Fields.UnlimitedIccMaxMode));
          }
        } else {
          DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
        }
      } else {
        DEBUG ((DEBUG_INFO, "(OC) ICC values are in sync with the user inputs, no modification required \n"));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "(OC) Failed to read the ICC Max Mailbox command. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", MailboxStatus, Status));
    }
    }
  return EFI_SUCCESS;
}

/**
  This function will perform the BCLK warm reset overclocking flow.

  @param[in]  UINT32 BclkFrequency    Required BCLK frequency in 10 KHz unit.
  @param[out] WarmResetFlag           Pointer to flag to indicate whether warm reset should be performed

  @retval EFI_SUCCESS          Function successfully executed.

  Appropriate failure code on error.
**/
EFI_STATUS
BclkOcInit (
  IN OVERCLOCKING_PREMEM_CONFIG   *OcConfig,
  OUT BOOLEAN         *WarmResetFlag
  )
{
  EFI_STATUS                Status;
  WDT_PPI                   *WdtPei;
  UINT8                     WarmResetRequired;
  UINT8                     WdtTimeout;
  UINT32                    MailboxStatus;
  UINT32                    RequiredBclkFrequency;
  OC_MAILBOX_INTERFACE      MailboxReadCmd;
  OC_MAILBOX_INTERFACE      MailboxWriteCmd;
  UINT32                    CurrentBclkFrequency;

  Status = EFI_SUCCESS;
  WarmResetRequired = FALSE;
  WdtTimeout = FALSE;
  MailboxStatus = 0;
  RequiredBclkFrequency = 0;
  MailboxReadCmd.InterfaceData = 0;
  MailboxWriteCmd.InterfaceData = 0;

  //
  // Locate WDT_PPI (ICC WDT PPI)
  //
  Status = PeiServicesLocatePpi (
             &gWdtPpiGuid,
             0,
             NULL,
             (VOID **) &WdtPei
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Check for WDT timeout on previous boot
  //
  WdtTimeout = WdtPei->CheckStatus();

  //
  // Check BCLK source
  //
  MailboxReadCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_BCLK_FREQUENCY_CMD;
  DEBUG ((DEBUG_INFO, "(BCLK) Read BCLK CMD = 0x%x\n", MailboxReadCmd));

  Status = OcMailboxRead (MailboxReadCmd.InterfaceData, &CurrentBclkFrequency, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "(BCLK) Current BCLK (0x5) (KHz) = %d\n", CurrentBclkFrequency));
  } else {
    DEBUG ((DEBUG_ERROR, "(BCLK) BCLK CMD=0x5 Read fail.....\n"));
  }

  //
  // If no WDT timeout, ramp BCLK, otherwise, continue boot to recover.
  //
  if (WdtTimeout == FALSE) {
    DEBUG ((DEBUG_INFO, "(OC) BclkOcInit start.\n"));
    if (OcConfig->CpuBclkOcFrequency != 0) {
      RequiredBclkFrequency = OcConfig->CpuBclkOcFrequency * 10;
      MailboxReadCmd.InterfaceData = 0;
      MailboxReadCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_CPU_BCLK_FREQUENCY;
      MailboxReadCmd.Fields.Param_4_0 = CPU_BCLK_SELECT;

      DEBUG ((DEBUG_INFO, "(BCLK) Required CPU BCLK (KHz)= %d\n", RequiredBclkFrequency));
      Status = OcMailboxRead (MailboxReadCmd.InterfaceData, &CurrentBclkFrequency, &MailboxStatus);
      DEBUG ((DEBUG_INFO, "(BCLK) Current CPU BCLK (KHz) = %d\n", CurrentBclkFrequency));
      if (RequiredBclkFrequency != CurrentBclkFrequency) {
        MailboxWriteCmd.InterfaceData = 0;
        MailboxWriteCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_CPU_BCLK_FREQUENCY;
        MailboxWriteCmd.Fields.Param_4_0 = CPU_BCLK_SELECT;
        Status = OcMailboxWrite (MailboxWriteCmd.InterfaceData, RequiredBclkFrequency, &MailboxStatus);
        if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
          WarmResetRequired = 1;
          Status = OcMailboxRead (MailboxReadCmd.InterfaceData, &CurrentBclkFrequency, &MailboxStatus);
          DEBUG ((DEBUG_INFO, "(BCLK)Reading back CPU BCLK value set by BIOS (KHz) = %d\n", CurrentBclkFrequency));
        }
      }
    }
    if (OcConfig->SocBclkOcFrequency != 0) {
      RequiredBclkFrequency = OcConfig->SocBclkOcFrequency * 10;
      MailboxReadCmd.InterfaceData = 0;
      MailboxReadCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_CPU_BCLK_FREQUENCY;
      MailboxReadCmd.Fields.Param_4_0  = SOC_BCLK_SELECT;
      DEBUG ((DEBUG_INFO, "(BCLK) Required SOC BCLK (KHz)= %d\n", RequiredBclkFrequency));
      Status = OcMailboxRead (MailboxReadCmd.InterfaceData, &CurrentBclkFrequency, &MailboxStatus);
      DEBUG ((DEBUG_INFO, "(BCLK) Current SOC BCLK (KHz) = %d\n", CurrentBclkFrequency));
      if (RequiredBclkFrequency != CurrentBclkFrequency) {
        MailboxWriteCmd.InterfaceData = 0;
        MailboxWriteCmd.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_CPU_BCLK_FREQUENCY;
        MailboxWriteCmd.Fields.Param_4_0 = SOC_BCLK_SELECT;
        Status = OcMailboxWrite (MailboxWriteCmd.InterfaceData, RequiredBclkFrequency, &MailboxStatus);
        if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
          WarmResetRequired = 1;
          Status = OcMailboxRead (MailboxReadCmd.InterfaceData, &CurrentBclkFrequency, &MailboxStatus);
          DEBUG ((DEBUG_INFO, "(BCLK)Reading back SOC BCLK value set by BIOS (KHz) = %d\n", CurrentBclkFrequency));
        }
      }
    }

    if (WarmResetRequired == 1) {
      DEBUG ((DEBUG_INFO, "(OC) Acknowledged BCLK Ramp reset is required.\n"));

      //
      // Warm reset is required for BCLK frequency changes. The BCLK frequency
      // is ramped while the CPU is in reset.
      //
      DEBUG ((DEBUG_INFO, "(OC) Performing Warm Reset for BCLK frequency change.\n"));
      *WarmResetFlag = TRUE;
    }
  }

  DEBUG ((DEBUG_INFO, "(OC) BclkOcInit End.\n"));
  return Status;
}

/**
  Compare the VCCIA boot voltage setup option against the CPU strap setting
  and in case of mismatch request a reset.

  @param[in]  OverClockingConfig     Pointer to CPU Overclocking Policy instance
  @param[out] ColdResetFlag          Pointer to flag to indicate whether cold reset should be performed

**/
VOID
ConfigVcciaBootVoltage (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig,
  OUT BOOLEAN                   *ColdResetFlag
  )
{

  UINT32      CpuStrapSet1High, CpuStrapSet2Low, CpuStrapSet2High, CpuStrapSet3Low;
  BOOLEAN     VcciaBootVoltageSel;
  EFI_STATUS  Status;

  CpuStrapSet1High = 0;
  CpuStrapSet2Low = 0;
  CpuStrapSet2High = 0;
  CpuStrapSet3Low = 0;
  Status = GetCpuStrapData (&CpuStrapSet1High, &CpuStrapSet2Low, &CpuStrapSet2High, &CpuStrapSet3Low, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "ConfigVcciaBootVoltage: GetCpuStrapData return %r\n", Status));
    return ;
  }

  VcciaBootVoltageSel = (BOOLEAN) ((CpuStrapSet1High & B_PMC_PWRM_SSMD_CORE_HIGH_VOLTAGE_MODE) >> N_PMC_PWRM_SSMD_CORE_HIGH_VOLTAGE_MODE);
  //
  // Check if the vccia boot voltage bit in strap setting matches the BIOS Setup option.
  //
  if (OverClockingConfig->VcciaBootVoltageSel != VcciaBootVoltageSel) {
    DEBUG ((
      DEBUG_INFO,
      "VcciaBootVoltageSel strap setting doesn't match the value from Setup = 0x%x / 0x%x\n",
      OverClockingConfig->VcciaBootVoltageSel,
      VcciaBootVoltageSel
      ));
    //
    // Toggle bit 14 in SSMD register
    //
    CpuStrapSet1High ^= B_PMC_PWRM_SSMD_CORE_HIGH_VOLTAGE_MODE;
    Status = SetCpuStrapData (&CpuStrapSet1High, &CpuStrapSet2Low, &CpuStrapSet2High, &CpuStrapSet3Low, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "ConfigVcciaBootVoltage: SetCpuStrapData return %r\n", Status));
      return ;
    }
    *ColdResetFlag = TRUE;
  }
}

/**
  Initializes VCCIA boot voltage in SSMD register, VCCSA boot voltage and CPU bandgap reference mode in CPU EPOC2 register as per the policy.
  Changing this configuration will result in cold reset for the system.

  @param[in]  OverClockingConfig     Pointer to CPU Overclocking Policy instance
  @param[out] ColdResetFlag          Pointer to flag to indicate whether cold reset should be performed

  @retval EFI_SUCCESS
**/
EFI_STATUS
ColdBootBoost (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig,
  OUT BOOLEAN         *ColdResetFlag
  )
{
  UINT32                     CpuEpoc2Value;
  BOOLEAN                    CpuBandgapRefMode;
  BOOLEAN                    SettingsChanged = FALSE;

  //
  // Check if GCR CPU_EPOC_LOCK bit was locked
  //
  if (PmcIsCpuEpocLocked () == FALSE) {
    //
    // EPOC2 register can be programmed irrespective of the EPOC2_EN bit status in EPOC register
    //
    CpuEpoc2Value = PmcGetCpuEpoc2 ();
    //
    // Configure for VCCSA Boot Voltage
    //
    if (OverClockingConfig->VccsaBootVoltageSel != ((CpuEpoc2Value & B_PMC_PWRM_CPU_EPOC2_VCCSA_BOOT_VOLTAGE_SEL) >> N_PMC_PWRM_CPU_EPOC2_VCCSA_BOOT_VOLTAGE_SEL)) {
      DEBUG ((DEBUG_INFO, "(OC) Modifying VCCSA boot voltage bit to select between nominal and high voltage(up to 1.2/1.3V) as per the policy\n"));
      //
      // Toggle bit 0 in CPU_EPOC2 register
      //
      CpuEpoc2Value ^= B_PMC_PWRM_CPU_EPOC2_VCCSA_BOOT_VOLTAGE_SEL;

      SettingsChanged = TRUE;
    }
    //
    // Configure for CPU Bandgap Reference Mode
    //
    CpuBandgapRefMode = (BOOLEAN) ((CpuEpoc2Value & B_PMC_PWRM_CPU_EPOC2_CPU_BGREF_MODE_OVR_VAL) >> N_PMC_PWRM_CPU_EPOC2_CPU_BGREF_MODE_OVR_VAL);

    if (OverClockingConfig->CpuBandgapRefMode != CpuBandgapRefMode) {
      DEBUG ((DEBUG_INFO, "(OC) Modifying CPU BandgapRefMode bit to select between normal and Bandgap Bypassed as per the policy\n"));
      //
      // Toggle bit 1 in CPU_EPOC2 register to set CPU bandgap reference mode override value
      //
      CpuEpoc2Value ^= B_PMC_PWRM_CPU_EPOC2_CPU_BGREF_MODE_OVR_VAL;
      //
      // Set bit 2 in CPU_EPOC2 register to enable CPU bandgap reference mode override
      //
      CpuEpoc2Value |= B_PMC_PWRM_CPU_EPOC2_CPU_BGREF_MODE_OVR_EN;

      SettingsChanged = TRUE;
    }

    if (SettingsChanged == TRUE) {
      DEBUG ((DEBUG_INFO, "(OC) CPU EPOC2 is set to : 0x%x ... Issuing Cold Reset\n", CpuEpoc2Value));
      PmcSetCpuEpoc2 (CpuEpoc2Value);
      *ColdResetFlag = TRUE;
    }
  } else {
    DEBUG ((DEBUG_INFO, "(OC) CPU EPOC was locked\n"));
  }
  //
  // Configure VCCIA boot voltage
  //
  ConfigVcciaBootVoltage (OverClockingConfig, ColdResetFlag);

  return EFI_SUCCESS;
}

/**
  Disables UnderVolt Protection in Pre-Mem to allow BIOS to program undervolt.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER      If user input if out of range.
**/
VOID
EFIAPI
UnderVoltProtectionPreMem (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  PCODE_MAILBOX_INTERFACE  MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE  MailboxWriteCommand;
  UNDERVOLT_PROTECTION_CMD_DATA    MailboxData;
  UINT32                   MailboxStatus;

  DEBUG ((DEBUG_INFO, "(OC) UnderVoltProtectionPreMem \n"));

  MailboxReadCommand.InterfaceData = 0;
  MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_UNDERVOLT_PROTECTION;

  MailboxWriteCommand.InterfaceData = 0;
  MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
  MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_UNDERVOLT_PROTECTION;


  MailboxData.Data = 0;
  DEBUG ((DEBUG_INFO, "(OC) Reading UnderVolt Protection status\n"));
  Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "(OC) Current value of UnderVolt Protection is %d \n", MailboxData.Fields.UnderVoltProtection));
    if (MailboxData.Fields.UnderVoltProtection != UNDERVOLT_PROTECTION_DISABLED) {
      MailboxData.Fields.UnderVoltProtection = UNDERVOLT_PROTECTION_DISABLED;
      if (OverClockingConfig->UnderVoltProtection == UNDERVOLT_PROTECTION_ENABLED) {
        DEBUG ((DEBUG_INFO, "(OC) Disabling UnderVolt Protection during BIOS phase, it will be enabled back before BIOS Done\n"));
      } else {
        DEBUG ((DEBUG_INFO, "(OC) Disabling UnderVolt Protection as per the policy\n"));
      }
      DEBUG ((DEBUG_INFO, "(OC) Writing new value of UnderVolt Protection %d to Mailbox\n", MailboxData.Fields.UnderVoltProtection));
      Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        // Read the value back to confirm the configuration.
        Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
        if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
          DEBUG ((DEBUG_INFO, "(OC) Read after Write value to confirm UnderVolt Protection is %d \n", MailboxData.Fields.UnderVoltProtection));
        } else {
          DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox function return status 0x%x \n", MailboxStatus, Status));
        }
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. MailboxStatus = 0x%x, Mailbox function return status 0x%x \n", MailboxStatus, Status));
      }
    }
  } else {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox function return status 0x%x \n", MailboxStatus, Status));
  }
}

/**
  Enables UnderVolt Protection in Post-Mem if policy is set to enable it.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER      If user input if out of range.
**/
VOID
EFIAPI
UnderVoltProtectionPostMem (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  PCODE_MAILBOX_INTERFACE  MailboxReadCommand;
  PCODE_MAILBOX_INTERFACE  MailboxWriteCommand;
  UNDERVOLT_PROTECTION_CMD_DATA    MailboxData;
  UINT32                   MailboxStatus;

  DEBUG ((DEBUG_INFO, "(OC) UnderVoltProtectionPostMem \n"));

  if (OverClockingConfig->UnderVoltProtection == UNDERVOLT_PROTECTION_ENABLED) {
    // Enable UnderVolt protection
    MailboxReadCommand.InterfaceData = 0;
    MailboxReadCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxReadCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_READ_UNDERVOLT_PROTECTION;

    MailboxWriteCommand.InterfaceData = 0;
    MailboxWriteCommand.Fields.Command = MAILBOX_OC_CMD_OC_INTERFACE;
    MailboxWriteCommand.Fields.Param1 = MAILBOX_OC_SUBCMD_WRITE_UNDERVOLT_PROTECTION;

    MailboxData.Data = 0;

    MailboxData.Fields.UnderVoltProtection = UNDERVOLT_PROTECTION_ENABLED;
    DEBUG ((DEBUG_INFO, "(OC) Enabling UnderVolt Protection\n"));
    Status = MailboxWrite (MailboxWriteCommand.InterfaceData, MailboxData.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
      // Read the value back to confirm the configuration.
      Status = MailboxRead (MailboxReadCommand.InterfaceData, &MailboxData.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_INFO, "(OC) Read after Write value to confirm UnderVolt Protection is %d \n", MailboxData.Fields.UnderVoltProtection));
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox function return status 0x%x \n", MailboxStatus, Status));
      }
    } else {
      DEBUG ((DEBUG_ERROR, "(OC) Mailbox write command failed. MailboxStatus = 0x%x, Mailbox function return status 0x%x \n", MailboxStatus, Status));
    }
  }
}

/**
  CPU SSC configuration for specific generation.

  @param[in] RequestComputeDieSsc    - Enable/Disable Compute Die SSC.
  @param[in] RequestSocDieSsc        - Enable/Disable Soc Die SSC.

**/
EFI_STATUS
EFIAPI
CpuSscConfigPreMem (
  IN UINT8    RequestComputeDieSsc,
  IN UINT8    RequestSocDieSsc
  )
{
  EFI_STATUS                      Status;
  PCODE_MAILBOX_INTERFACE         MailboxCmd;
  MAILBOX_DATA_SSC_CONTROL        SscMailboxData;
  UINT32                          MailboxStatus;

  DEBUG ((DEBUG_INFO, "CpuSscConfigPreMem Start.\n"));

  Status                   = EFI_SUCCESS;
  MailboxStatus            = PCODE_MAILBOX_CC_SUCCESS;
  MailboxCmd.InterfaceData = 0;
  SscMailboxData.Data32    = 0;

  DEBUG ((DEBUG_INFO, "Request Compute Die SSC = %d\n", RequestComputeDieSsc));
  DEBUG ((DEBUG_INFO, "Request SOC Die SSC = %d\n", RequestSocDieSsc));

  //
  // Config Compute Die SSC
  //
  MailboxCmd.Fields.Command = MAILBOX_PCODE_CMD_BCLK_CONFIG;
  MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND;
  MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_CPU_BCLK;
  Status = MailboxRead (MailboxCmd.InterfaceData, &SscMailboxData.Data32, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "Current Compute Die SSC = %d\n", SscMailboxData.Fields.SscEnable));
    if (RequestComputeDieSsc != (UINT8) (SscMailboxData.Fields.SscEnable)) {
      MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_SET_SSC_CONTROL_SUBCOMMAND;
      MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_CPU_BCLK;
      SscMailboxData.Fields.SscEnable = RequestComputeDieSsc;
      Status = MailboxWrite (MailboxCmd.InterfaceData, SscMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "Mailbox Command Write Compute Die SSC config failed. EFI_STATUS = %r, Mailbox Status = 0x%x\n", Status, MailboxStatus));
      }
    }
  }

  //
  // Config SOC Die SSC
  //
  MailboxCmd.Fields.Command = MAILBOX_PCODE_CMD_BCLK_CONFIG;
  MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND;
  MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_SOC_BCLK;
  Status = MailboxRead (MailboxCmd.InterfaceData, &SscMailboxData.Data32, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_INFO, "Current Compute Die SSC = %d\n", SscMailboxData.Fields.SscEnable));
    if (RequestSocDieSsc != (UINT8) (SscMailboxData.Fields.SscEnable)) {
      MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_SET_SSC_CONTROL_SUBCOMMAND;
      MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_SOC_BCLK;
      SscMailboxData.Fields.SscEnable = RequestSocDieSsc;
      Status = MailboxWrite (MailboxCmd.InterfaceData, SscMailboxData.Data32, &MailboxStatus);
      if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "Mailbox Command Write Soc Die SSC config failed. EFI_STATUS = %r, Mailbox Status = 0x%x\n", Status, MailboxStatus));
      }
    }
  }

  //
  // Check if SSC configuration is applied.
  //
  DEBUG_CODE_BEGIN ();
  {
    //
    // Compute Die Get SSC config
    //
    MailboxCmd.Fields.Command = MAILBOX_PCODE_CMD_BCLK_CONFIG;
    MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND;
    MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_CPU_BCLK;
    Status = MailboxRead (MailboxCmd.InterfaceData, &SscMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Read Compute Die SSC config failed. EFI_STATUS = %r, Mailbox Status = 0x%x\n", Status, MailboxStatus));
    } else {
      DEBUG ((DEBUG_INFO, "MailboxRead             : Compute Die SSC Enable = %d\n", SscMailboxData.Data32));
    }

    //
    // Soc Die Get SSC config
    //
    MailboxCmd.Fields.Command = MAILBOX_PCODE_CMD_BCLK_CONFIG;
    MailboxCmd.Fields.Param1  = MAILBOX_PCODE_BCLK_CONFIG_GET_SSC_CONTROL_SUBCOMMAND;
    MailboxCmd.Fields.Param2  = SSC_PLL_SELECT_SOC_BCLK;
    Status = MailboxRead (MailboxCmd.InterfaceData, &SscMailboxData.Data32, &MailboxStatus);
    if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
      DEBUG ((DEBUG_ERROR, "Mailbox Command Read Compute Die SSC config failed. EFI_STATUS = %r, Mailbox Status = 0x%x\n", Status, MailboxStatus));
    } else {
      DEBUG ((DEBUG_INFO, "MailboxRead             : Soc Die SSC Enable = %d\n", SscMailboxData.Data32));
    }
  }
  DEBUG_CODE_END ();

  DEBUG ((DEBUG_INFO, "CpuSscConfigPreMem End.\n"));
  return Status;
}

/**
  Program OC max voltage limits
  This function overrides the process Vmax limits for the applicable domains.
  This programming should be done before any voltage override for the respective domains.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

**/
VOID
STATIC
OcMaxVoltageLimit (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  UINT32                   MailboxStatus;
  OC_MAILBOX_INTERFACE     OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE     OcMailboxWriteCommand;
  UINT8                    DieIndex;
  UINT8                    DomainId;
  UINT32                   OcMaxVolLimitData;
  UINT16                   TempVoltage;

  DEBUG ((DEBUG_INFO, "(OC) Configuring Max Voltage Limit\n"));

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_READ_MAX_VOLTAGE_LIMIT;

  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_WRITE_MAX_VOLTAGE_LIMIT;

  for (DieIndex = 0; DieIndex < MAX_CDIE; DieIndex++) {
    if (mCDiePresenceFlag[DieIndex] != TRUE) {
      continue;
    }
    //
    // 0: Core  1:GT  2:Ring  3:Reserved  4:SA  5:IA_Atom  6:MemSS  7:Compute SAF  8:NPU
    // NOTE: Only Core/Ring Domain support multi-die, for the other domains DIE = 0 as they are package scope.
    //
    for (DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE; DomainId <= MAILBOX_OC_DOMAIN_ID_NPU; DomainId++) {
      if ((DomainId == MAILBOX_OC_DOMAIN_ID_RING) || (DomainId == MAILBOX_OC_DOMAIN_ID_RESERVED) || (DomainId == MAILBOX_OC_DOMAIN_ID_IA_ATOM)) {
        continue;
      }
      TempVoltage = 0;
      OcMaxVolLimitData = 0;
      OcMailboxReadCommand.Fields.Param_4_0 = DomainId;
      OcMailboxWriteCommand.Fields.Param_4_0 = DomainId;
      Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &OcMaxVolLimitData, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
        //
        // Converts the input voltage target to the fixed point U12.2.10 Volt format
        //
        ConvertVoltageTarget (OverClockingConfig->MaxVoltageLimit[DieIndex][DomainId], &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
        if (TempVoltage != 0) {
          DEBUG ((DEBUG_INFO, "(OC) Updating Max Voltage Limit for Die = %d, Domain = %d, Value = %d\n, ConvertVoltage = 0x%x", DieIndex, DomainId, OverClockingConfig->MaxVoltageLimit[DieIndex][DomainId], TempVoltage));
          Status = OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, TempVoltage, &MailboxStatus);
          if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
            DEBUG ((DEBUG_INFO, "(OC) Max Voltage Limit failed, mailbox status = 0x%x\n", MailboxStatus));
          }
        }
      } else {
        DEBUG ((DEBUG_ERROR, "(OC) Max Voltage Limit Failed. EFI Status = %r, Mailbox Status = 0x%x\n", Status, MailboxStatus));
      }
    }
  }
}

/**
  Initializes pre-memory Overclocking settings for all CPU package domains.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPreMem (
  IN SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi
  )
{
  EFI_STATUS    Status;
  OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig;
  BOOLEAN               ColdResetFlag;
  BOOLEAN               WarmResetFlag;

  DEBUG ((DEBUG_INFO, "(OC) %a Start \n", __FUNCTION__));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_OCINIT_START_PREMEM); //PostCode (0xC20)

  ColdResetFlag = FALSE;
  WarmResetFlag = FALSE;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OverClockingConfig);
  ASSERT_EFI_ERROR (Status);

  CreateAndInitOcDataHob (OverClockingConfig);

  ///
  /// Check for Overclocking support
  ///
  if (OverClockingConfig->OcSupport == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Overclocking is disabled. Bypassing CPU core overclocking flow.\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Check for UnderVolt Protection status in Pre-Mem and if required disable protection to allow under-voltage programming in BIOS phase.
  ///
  UnderVoltProtectionPreMem (OverClockingConfig);

  ///
  /// Check for VccsaBootVoltageSel, CpuBandgapRefMode and VcciaBootVoltageSel configuration
  ///
  Status = ColdBootBoost (OverClockingConfig, &ColdResetFlag);

  ///
  /// Initializes various Overclocking controls with OC interface command.
  ///
  B2pConfig (OverClockingConfig, &WarmResetFlag);

  ///
  /// Program OC Vmax limits for applicable domains before the VF override
  ///
  OcMaxVoltageLimit (OverClockingConfig);

  ///
  /// Configure core V/F premem settings and bclk adaptive voltage. OC Mailbox command 0x15.
  ///
  MiscGlobalConfig (OverClockingConfig);

  ///
  /// Initializes TURBO Control configuration. OC Mailbox command 0x18/0x19.
  ///
  MiscTurboConfig (OverClockingConfig);

  ///
  /// Configure AVX Ratio negative offset control for all AVX levels. OC Mailbox command 0x1A/0x1B.
  ///
  AvxRatioControl (OverClockingConfig);

  ///
  /// Configure the AVX Voltage Guardband Scale Factor.  OC Mailbox command 0x20/0x21.
  ///
  AvxVoltageGuardbandScaleFactor (OverClockingConfig);

  ///
  /// Program per-core disable feature
  ///
  PeiCpuOcSetDisablePerCoreMask (OverClockingConfig->DisablePerCoreMask, &ColdResetFlag);

  ///
  /// Configure OC voltage for all CPU package domains settings
  ///
  Status = CpuOcInitVoltageSettings (OverClockingConfig);

  ///
  /// Configure VR ICCMAX override
  ///
  Status = VrIccMaxOcOverride (OverClockingConfig);

  //
  // Initialize Bclk for CPU OC features
  //
  Status = BclkOcInit (OverClockingConfig, &WarmResetFlag);

  ///
  ///  SSC Configuration
  ///
  DEBUG ((DEBUG_INFO, "Invoking CpuSscConfigPreMem \n"));
  Status = CpuSscConfigPreMem (OverClockingConfig->ComputeDieSscEnable, OverClockingConfig->SocDieSscEnable);


  //
  // Update OC Data to DataHob for Pre-Memory
  //
  UpdateOcDataHobPreMem (OverClockingConfig);

  ///
  /// Perform reset if required by oc programming flows
  ///
  if (ColdResetFlag || WarmResetFlag) {
    REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_OCINIT_SET_RESET_FLAG); //PostCode (0xC21)
    DEBUG ((DEBUG_INFO, "CpuOcInitPreMem Reset Flag Set\n"));
    if (ColdResetFlag) {
      DEBUG ((DEBUG_INFO, "(OC) Perform Cold Reset\n"));
      (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetCold, EFI_SUCCESS, 0, NULL);
    } else if (WarmResetFlag) {
      DEBUG ((DEBUG_INFO, "(OC) Perform Warm Reset\n"));
      (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetWarm, EFI_SUCCESS, 0, NULL);
    }
    DEBUG ((DEBUG_INFO, "CpuOcInit Reset \n"));
  }
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_CPU_OCINIT_END_PREMEM); //PostCode (0xC2F)
  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return Status;
}

/**
  GranularRatio Setting is applicable only to Core Domain. This configuration is Moduled scoped.
  When targeted Module is E-core, the command modifies the setting for all E-cores in the cluster.
  When targeted Module is P-core, the command modifies the setting for all P-cores in the cluster.
  (we don't need separate entries for P/E cores because CoreType for Module won't change runtime)

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
**/
STATIC
VOID
GranularRatioSupport (
OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  UINT32                       MailboxStatus;
  EFI_STATUS                   Status;
  OC_MAILBOX_INTERFACE         MailboxCommand;
  OC_GRANULAR_RATIO_DATA       GranularRatioConfigData;
  OC_INSTANCE_NODE             *InstanceNode;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  if (mListHead == NULL) {
    DEBUG (( DEBUG_INFO, "(OC) Enabled Modules List is empty\n"));
    return;
  }
  ///
  /// Start with the first enabled module in the list and
  /// program Granular Ratio support (commands 0x26 and 0x27) for each module to allow per Module Granularity Bins setting.
  ///
  InstanceNode = mListHead;
  while (InstanceNode != NULL) {
    GranularRatioConfigData.Data = 0;
    MailboxCommand.InterfaceData = 0;
    MailboxCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_READ_GRANULAR_RATIO_SUPPORT_CMD;
    MailboxCommand.Fields.InstanceId        = InstanceNode->InstanceId;
    Status = OcMailboxRead (MailboxCommand.InterfaceData, &GranularRatioConfigData.Data, &MailboxStatus);
    if (Status == EFI_SUCCESS && MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
      DEBUG ((DEBUG_INFO, "(OC) For DieId %d and ModuleId %d :Current GranularityBins = %d, Current GranularityUnits = %d.\n",
      InstanceNode->DieId, InstanceNode->ModuleId, GranularRatioConfigData.Fields.GranularityBins, GranularRatioConfigData.Fields.GranularityUnits));
      mGranularityUnits = (UINT8) GranularRatioConfigData.Fields.GranularityUnits; // This value will be same for all modules.
      DEBUG ((DEBUG_INFO, "(OC) As per policy required Granular bins is %d \n", OverClockingConfig->GranularityBins[InstanceNode->DieId][InstanceNode->ModuleId]));
      ///
      /// check if update is required or not
      ///
      if (GranularRatioConfigData.Fields.GranularityBins != OverClockingConfig->GranularityBins[InstanceNode->DieId][InstanceNode->ModuleId]) {
        GranularRatioConfigData.Fields.GranularityBins = OverClockingConfig->GranularityBins[InstanceNode->DieId][InstanceNode->ModuleId];
        MailboxCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_WRITE_GRANULAR_RATIO_SUPPOR_CMD; // no need to populate InstanceId again
        Status = OcMailboxWrite (MailboxCommand.InterfaceData, GranularRatioConfigData.Data, &MailboxStatus);
        if (Status == EFI_SUCCESS && MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
          DEBUG ((DEBUG_ERROR, "(OC) Updated Granular Ratio setting for InstanceId %d, \n", InstanceNode->InstanceId));
        }
      }
    }
    ///
    /// Go to next enabled module in the list
    ///
    InstanceNode = InstanceNode->NextNodePtr;
  }
  return;
}

/**
  Per-Core Voltage & Frequency Settings to run in all threads.

  @param[in] Context - A pointer to PER_CORE_SETTING_CONTEXT.
**/
STATIC
VOID
EFIAPI
PerCoreVoltageFrequencySetting (
  IN PER_CORE_SETTING_CONTEXT *Context
  )
{
  OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig;
  VOLTAGE_FREQUENCY_ITEM       CurrentVfItem;
  VOLTAGE_FREQUENCY_ITEM       RequestedVfItem;
  EFI_STATUS                   Status;
  UINT32                       MailboxStatus;
  STATIC UINT8                 BigCoreIndex = 0;
  STATIC UINT8                 AtomClusterIndex = 0;
  INT16                        RequestedVoltage;;
  UINT8                        CoreType;
  UINTN                        CpuNumber;
  UINT32                       ThreadIndex;
  UINT32                       ModuleIndex;

  Status = Context->MpServices2Ppi->WhoAmI (Context->MpServices2Ppi, &CpuNumber);
  ASSERT_EFI_ERROR (Status);

  OverClockingConfig = Context->OverClockingConfig;
  CoreType = 0;

  //
  // GetProcessorLocation2ByApicId(InitialApicId,Package,Die,Tile,Module,Core,Thread)
  //
  GetProcessorLocation2ByApicId (GetInitialApicId (), NULL, NULL, NULL, &ModuleIndex, NULL, &ThreadIndex);
  ASSERT (ModuleIndex <= MAX_OC_ATOM_CLUSTERS_ID);

  //
  // This featue is module level, one module should only progrom once .
  // For Big cores, use thread check to do that. for Small cores, all threads are 0 . so use Module Id to check that in below.
  //
  if (ThreadIndex != 0) {
    return;
  }

  //
  // Get the current Voltage & Frequency item.
  //
  ZeroMem (&CurrentVfItem, sizeof (CurrentVfItem));
  CurrentVfItem.DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE;
  CurrentVfItem.VfSettings.VfPointIndex = 0;
  Status = GetVoltageFrequencyItem (&CurrentVfItem, &MailboxStatus);
  if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    return;
  }

  //
  // Populate the user requested Voltage & Frequency item.
  //
  ZeroMem (&RequestedVfItem, sizeof (RequestedVfItem));
  RequestedVfItem.DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE;

  GetCoreType (&CoreType);
  if (CoreType == CPUID_CORE_TYPE_INTEL_CORE) {
    RequestedVoltage = OverClockingConfig->PerCoreVoltageOffset[BigCoreIndex];
    if (IsPerCoreVotageFru ()) {
      RequestedVfItem.VfSettings.MaxOcRatio = OverClockingConfig->PerCoreMaxRatio[BigCoreIndex];
      RequestedVfItem.VfSettings.VoltageTargetMode = (UINT8) OverClockingConfig->PerCoreVoltageMode[BigCoreIndex];
      if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->PerCoreAdaptiveVoltage[BigCoreIndex];
      } else {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->PerCoreVoltageOverride[BigCoreIndex];
      }
    }
    ++BigCoreIndex;
  } else {
    if (mAtomClusterFlag[ModuleIndex] == 1) {
      return;
    }
    RequestedVoltage = OverClockingConfig->PerAtomClusterVoltageOffset[AtomClusterIndex];
    if (IsPerCoreVotageFru ()) {
      RequestedVfItem.VfSettings.MaxOcRatio = OverClockingConfig->PerAtomClusterMaxRatio[AtomClusterIndex];
      RequestedVfItem.VfSettings.VoltageTargetMode = (UINT8) OverClockingConfig->PerAtomClusterVoltageMode[AtomClusterIndex];
      if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->PerAtomClusterAdaptiveVoltage[AtomClusterIndex];
      } else {
        RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->PerAtomClusterVoltageOverride[AtomClusterIndex];
      }
    }
    if (mAtomClusterFlag[ModuleIndex] == 0) {
      mAtomClusterFlag[ModuleIndex] = 1;
      ++AtomClusterIndex;
    }
  }
  if (!IsPerCoreVotageFru ()) {
    RequestedVfItem.VfSettings.VoltageTargetMode = (UINT8) OverClockingConfig->VoltageMode[MAILBOX_OC_DOMAIN_ID_IA_CORE];
    if (RequestedVfItem.VfSettings.VoltageTargetMode == OC_LIB_OFFSET_ADAPTIVE) {
      RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->AdaptiveVoltage[MAILBOX_OC_DOMAIN_ID_IA_CORE];
    } else {
      RequestedVfItem.VfSettings.VoltageTarget = OverClockingConfig->VoltageOverride[MAILBOX_OC_DOMAIN_ID_IA_CORE];
    }
  }
  RequestedVfItem.VfSettings.VfPointIndex  = 0;
  RequestedVfItem.VfSettings.VoltageOffset = RequestedVoltage;

  //
  // Update existing OC Voltage & Frequency item to the requested OC configuration.
  //
  if (CpuNumber == Context->BspNumber) {
    DEBUG ((DEBUG_INFO, "(OC) Update Per-Core Voltage & Frequency offset...\n"));
  }
  if (CompareMem ((VOID *) &RequestedVfItem, (VOID *) &CurrentVfItem, sizeof (VOLTAGE_FREQUENCY_ITEM))) {
    UpdateVoltageFrequencyItem (&CurrentVfItem, &RequestedVfItem, NULL);
  }

  return;
}

/**
  Read each processor's ratio value

  @param[out]    Buffer      Pointer to struct PROCESSOR_MAILBOX_DATA
**/
VOID
MailboxReadRatioValue (
  OUT  VOID   *Buffer
  )
{
  PROCESSOR_MAILBOX_DATA       *EachProcessorRatioOverride;
  OC_MAILBOX_INTERFACE         MailboxCommand;
  EFI_STATUS                   Status;

  //
  // Initialize
  //
  EachProcessorRatioOverride = (PROCESSOR_MAILBOX_DATA *) Buffer;

  if (Buffer == NULL) {
    return;
  }

  //
  // Read From CpuMailboxLib
  //
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_READ_PER_CORE_RATIO_LIMITS_CMD;
  Status = OcMailboxRead (
    MailboxCommand.InterfaceData,
    (UINT32 *) &EachProcessorRatioOverride->MailboxData,
    (UINT32 *) &EachProcessorRatioOverride->MailboxStatus
    );
  if ((Status != EFI_SUCCESS) || (EachProcessorRatioOverride->MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    DEBUG ((DEBUG_ERROR, "(OC) Mailbox read command failed. MailboxStatus = 0x%x, Mailbox command return status 0x%x \n", EachProcessorRatioOverride->MailboxStatus, Status));
  }
}

/**
  Write each processor's ratio value

  @param[out]    Buffer      Pointer to struct PROCESSOR_MAILBOX_DATA
**/
VOID
MailboxWriteRatioValue (
  OUT  VOID   *Buffer
  )
{
  PROCESSOR_MAILBOX_DATA       *EachProcessorRatioOverride;
  MSR_FLEX_RATIO_REGISTER      FlexRatioMsr;
  OC_MAILBOX_INTERFACE         MailboxCommand;

  //
  // Initialize
  //
  EachProcessorRatioOverride = (PROCESSOR_MAILBOX_DATA *) Buffer;

  if (Buffer == NULL) {
    return;
  }

  //
  // Check whether OC lock bit already set
  //
  FlexRatioMsr.Uint64 = AsmReadMsr64 (MSR_FLEX_RATIO);
  if (((UINT8) FlexRatioMsr.Bits.OcLock) != 0) {
    return;
  }

  //
  // Read From CpuMailboxLib
  //
  MailboxCommand.InterfaceData = 0;
  MailboxCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_WRITE_PER_CORE_RATIO_LIMITS_CMD;
  OcMailboxWrite (
    MailboxCommand.InterfaceData,
    EachProcessorRatioOverride->MailboxData,
    (UINT32 *) &EachProcessorRatioOverride->MailboxStatus
    );
}

/**
  Each Core Ratio Value Override.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
  @param[in] MpServices2Ppi           Pointer to this instance of the MpServices.

  @retval    EFI_SUCCESS              Big / Small Core Ratio Override successfully
  @retval    Others                   Big / Small Core Ratio Override failed
**/
EFI_STATUS
RatioValueOverride (
  IN  OVERCLOCKING_PREMEM_CONFIG  *OverClockingConfig,
  IN  EDKII_PEI_MP_SERVICES2_PPI  *MpServices2Ppi
  )
{
  EFI_STATUS                   Status;
  UINT32                       ProcessorNumber;
  UINT32                       CoreIndex;
  UINT32                       AtomIndex;
  UINT8                        MaxNonTurboLimRatio;
  UINT8                        MaxEfficiencyRatio;
  UINT8                        ApCoreType;
  PROCESSOR_MAILBOX_DATA       EachProcessorRatioOverride;
  EFI_PROCESSOR_INFORMATION    MpContext;
  UINTN                        NumberOfProcessors;
  UINTN                        NumberOfEnabledProcessors;

  //
  // Initialize
  //
  Status                = EFI_SUCCESS;
  MaxNonTurboLimRatio   = 0;
  MaxEfficiencyRatio    = 0;
  ZeroMem (&EachProcessorRatioOverride, sizeof (PROCESSOR_MAILBOX_DATA));
  ZeroMem (&MpContext, sizeof (EFI_PROCESSOR_INFORMATION));

  DEBUG ((DEBUG_INFO, "(OC) RatioValueOverride Start \n"));

  if (!OverClockingConfig->PerCoreRatioOverride) {
    return EFI_SUCCESS;
  }
  Status = MpServices2Ppi->GetNumberOfProcessors (
                             MpServices2Ppi,
                             (UINTN *)&NumberOfProcessors,
                             (UINTN *)&NumberOfEnabledProcessors
                             );
  ASSERT_EFI_ERROR (Status);

  CoreIndex = 0;
  AtomIndex = 0;
  GetBusRatio (&MaxNonTurboLimRatio, &MaxEfficiencyRatio);
  for (ProcessorNumber = 0; ProcessorNumber < NumberOfEnabledProcessors; ProcessorNumber++) {
    Status = MpServices2Ppi->GetProcessorInfo (
                               MpServices2Ppi,
                               (UINTN) ProcessorNumber,
                               &MpContext
                               );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Processor %d GetProcessorInfo() failed\n", ProcessorNumber));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    if (MpContext.Location.Thread != FIRST_THREAD) {
      // Continue when the processor is not 1st thread of Core, i.e. the 2nd, 3rd or 4th thread of Core.
      continue;
    }

    if ((MpContext.StatusFlag & BIT0) == BIT0) {
      //
      // if MpContext.StatusFlag BIT0 be raised, this processor is BSP
      //
      MailboxReadRatioValue ((VOID *) &EachProcessorRatioOverride);
      if (EachProcessorRatioOverride.MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
        continue;
      }
      ApCoreType = 0;
      GetCoreType (&ApCoreType);
      if (ApCoreType == CPUID_CORE_TYPE_INTEL_CORE) {
      //
      // BSP is Big Core
      //
        if (OverClockingConfig->PerCoreRatio[CoreIndex] > MaxNonTurboLimRatio) {
          EachProcessorRatioOverride.MailboxData = (UINT32) OverClockingConfig->PerCoreRatio[CoreIndex];
          DEBUG ((DEBUG_INFO, "Setup PerCore Max Ratio[%d] = 0x%02X\n", CoreIndex, OverClockingConfig->PerCoreRatio[CoreIndex]));
          MailboxWriteRatioValue ((VOID *) &EachProcessorRatioOverride);
          DEBUG ((DEBUG_INFO, "MailboxWriteRatioValue = 0x%08X, Status = 0x%08X\n", EachProcessorRatioOverride.MailboxData, EachProcessorRatioOverride.MailboxStatus));
        }
        CoreIndex++;
      } else if (ApCoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
        //
        // BSP is Small Core
        //
        if ((OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER] > MaxNonTurboLimRatio) && (AtomIndex % ATOM_QUANTITY_IN_CLUSTER == 0)) {
          EachProcessorRatioOverride.MailboxData = (UINT32) OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER];
          DEBUG ((DEBUG_INFO, "Setup AtomClustere Max Ratio[%d] = 0x%02X\n", AtomIndex / ATOM_QUANTITY_IN_CLUSTER, OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER]));
          MailboxWriteRatioValue ((VOID *) &EachProcessorRatioOverride);
          DEBUG ((DEBUG_INFO, "MailboxWriteRatioValue = 0x%08X, Status = 0x%08X\n", EachProcessorRatioOverride.MailboxData, EachProcessorRatioOverride.MailboxStatus));
        }
        AtomIndex++;
      }
    } else {
      Status = MpServices2Ppi->StartupThisAP (
                                 MpServices2Ppi,
                                 (EFI_AP_PROCEDURE) MailboxReadRatioValue,
                                 (UINTN) ProcessorNumber,
                                 0,
                                 (VOID *) &EachProcessorRatioOverride
                                 );
      if (EachProcessorRatioOverride.MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
        continue;
      }
      ApCoreType = 0;
      Status = MpServices2Ppi->StartupThisAP (
                                 MpServices2Ppi,
                                 (EFI_AP_PROCEDURE) GetCoreType,
                                 (UINTN) ProcessorNumber,
                                 0,
                                 (UINT8 *) &ApCoreType
                                 );
      if (ApCoreType == CPUID_CORE_TYPE_INTEL_CORE) {
        //
        // AP is Big Core
        //
        if (OverClockingConfig->PerCoreRatio[CoreIndex] > MaxNonTurboLimRatio) {
          EachProcessorRatioOverride.MailboxData = (UINT32) OverClockingConfig->PerCoreRatio[CoreIndex];
          DEBUG ((DEBUG_INFO, "Setup PerCore Max Ratio[%d] = 0x%02X\n", CoreIndex, OverClockingConfig->PerCoreRatio[CoreIndex]));
          Status = MpServices2Ppi->StartupThisAP (
                                     MpServices2Ppi,
                                     (EFI_AP_PROCEDURE) MailboxWriteRatioValue,
                                     (UINTN) ProcessorNumber,
                                     0,
                                     (VOID *) &EachProcessorRatioOverride
                                     );
          DEBUG ((DEBUG_INFO, "MailboxWriteRatioValue = 0x%08X, Status = 0x%08X\n", EachProcessorRatioOverride.MailboxData, EachProcessorRatioOverride.MailboxStatus));
          if (EFI_ERROR (Status)) {
            return Status;
          }
        }
        CoreIndex++;
      } else if (ApCoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
        //
        // AP is Small Core
        //
        if ((OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER] > MaxNonTurboLimRatio) && (AtomIndex % ATOM_QUANTITY_IN_CLUSTER == 0)) {
          EachProcessorRatioOverride.MailboxData = (UINT32) OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER];
          DEBUG ((DEBUG_INFO, "Setup AtomCluster Max Ratio[%d] = 0x%02X\n", AtomIndex / ATOM_QUANTITY_IN_CLUSTER, OverClockingConfig->AtomClusterRatio[AtomIndex / ATOM_QUANTITY_IN_CLUSTER]));
          Status = MpServices2Ppi->StartupThisAP (
                                     MpServices2Ppi,
                                     (EFI_AP_PROCEDURE) MailboxWriteRatioValue,
                                     (UINTN) ProcessorNumber,
                                     0,
                                     (VOID *) &EachProcessorRatioOverride
                                     );
          DEBUG ((DEBUG_INFO, "MailboxWriteRatioValue = 0x%08X, Status = 0x%08X\n", EachProcessorRatioOverride.MailboxData, EachProcessorRatioOverride.MailboxStatus));
          if (EFI_ERROR (Status)) {
            return Status;
          }
        }
        AtomIndex++;
      }
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) RatioValueOverride End \n"));

  return EFI_SUCCESS;
}


/**
  Get Core details per CCP Module.
**/
STATIC
VOID
GetCoreCcpInfo (
  VOID
  )
{
  UINT8                        CoreType;
  UINT32                       ThreadIndex;
  UINT32                       ModuleIndex;
  CoreType = 0;

  //
  // GetProcessorLocation2ByApicId(InitialApicId,Package,Die,Tile,Module,Core,Thread)
  // Use this API to get the ModuleIndex(CCP) value for cores.
  // ModuleIndex will be same for both the threads of Pcore and same for four Ecores in a Ecore cluster.
  //
  GetProcessorLocation2ByApicId (GetInitialApicId (), NULL, NULL, NULL, &ModuleIndex, NULL, &ThreadIndex);
  ASSERT (ModuleIndex <= MAX_OC_CCP_MODULES);

  //
  // This feature is module level, one module should only program once .
  //
  if (ThreadIndex != 0) {
   return;
  }

  GetCoreType (&CoreType);
  mCoreTypeForModuleIndex[ModuleIndex] = CoreType;
  return;
}


/**
  Program OC TVB Parameters Per CCP Module.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER      If user input if out of range.
**/
EFI_STATUS
STATIC
ProgramTvbThresholdsPerCcp (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  UINT32                   MailboxStatus;
  UINT8                    ModuleIndex;
  OCMB_TVB_DATA            TvbCommand;
  OCTVB_MAILBOX_INTERFACE  OcMailboxReadCommand;
  OCTVB_MAILBOX_INTERFACE  OcMailboxWriteCommand;
  UINT8                    Threshold;
  UINT8                    PcoreIndex;
  UINT8                    EclusterIndex;
  UINT8                    UpdateTvbFlag;

  PcoreIndex = 0;
  EclusterIndex = 0;
  UpdateTvbFlag = 0;
  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "(OC) Configuring TVB parameters Per CCP Module\n"));

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_OC_TVB_CONFIG;

  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_OC_TVB_CONFIG;

  TvbCommand.Data           = 0;

  for (ModuleIndex = 0; ModuleIndex < MAX_OC_CCP_MODULES; ModuleIndex++) {
    if (mCoreTypeForModuleIndex[ModuleIndex] == 0) { // module not present
      continue;
    }
    OcMailboxReadCommand.Fields.Param_4_0  = 0;         // Core Group is 0
    OcMailboxWriteCommand.Fields.Param_4_0 = 0;         // Core Group is 0
    // Limit select
    OcMailboxReadCommand.Fields.Param_19_18  = 0;         // Per-CCP
    OcMailboxWriteCommand.Fields.Param_19_18 = 0;         // Per-CCP

    OcMailboxReadCommand.Fields.InstanceId  = ModuleIndex;
    OcMailboxWriteCommand.Fields.InstanceId = ModuleIndex;
    for (Threshold = 0; Threshold <= 1; Threshold++) {
      //OcMailboxReadCommand.Fields.Param1  |= (Threshold << TVB_THRESHOLD_OFFSET);
      OcMailboxReadCommand.Fields.Param_21_20 = Threshold;
      //OcMailboxWriteCommand.Fields.Param1 |= (Threshold << TVB_THRESHOLD_OFFSET);
      OcMailboxWriteCommand.Fields.Param_21_20 = Threshold;
      TvbCommand.Data = 0; // Reset data
      UpdateTvbFlag = 0; // Reset the update flag
      Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &TvbCommand.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_INFO, "(OC) Current TVB Config for Module Index %d and Threshold %d is = 0x%x\n",ModuleIndex, Threshold, TvbCommand.Data));
        if ((Threshold == 0) && (mCoreTypeForModuleIndex[ModuleIndex] == CPUID_CORE_TYPE_INTEL_CORE)) {
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerPcoreRatioDownBinAboveT0[PcoreIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->PcoreTvbTempThreshold0)) {
            DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Pcore Index %d Threshold 0\n", PcoreIndex));
            TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerPcoreRatioDownBinAboveT0[PcoreIndex];
            TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->PcoreTvbTempThreshold0;
            UpdateTvbFlag = 1;
          }
        } else if ((Threshold == 1) && (mCoreTypeForModuleIndex[ModuleIndex] == CPUID_CORE_TYPE_INTEL_CORE)) {
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerPcoreRatioDownBinAboveT1[PcoreIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->PcoreTvbTempThreshold1)) {
              DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Pcore Index %d Threshold 1\n", PcoreIndex));
              TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerPcoreRatioDownBinAboveT1[PcoreIndex];
              TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->PcoreTvbTempThreshold1;
              UpdateTvbFlag = 1;
          }
          ++PcoreIndex; // Increment Pcore Index after Threshold 1
        } else if ((Threshold == 0) && (mCoreTypeForModuleIndex[ModuleIndex] == CPUID_CORE_TYPE_INTEL_ATOM)) {
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerEcoreCcpRatioDownBinAboveT0[EclusterIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->EcoreTvbTempThreshold0)) {
              DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Ecluster Index %d Threshold 0\n",EclusterIndex));
              TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerEcoreCcpRatioDownBinAboveT0[EclusterIndex];
              TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->EcoreTvbTempThreshold0;
              UpdateTvbFlag = 1;
          }
        } else if ((Threshold == 1) && (mCoreTypeForModuleIndex[ModuleIndex] == CPUID_CORE_TYPE_INTEL_ATOM)) {
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerEcoreCcpRatioDownBinAboveT1[EclusterIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->EcoreTvbTempThreshold1)) {
              DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Ecluster Index %d Threshold 1\n",EclusterIndex));
              TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerEcoreCcpRatioDownBinAboveT1[EclusterIndex];
              TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->EcoreTvbTempThreshold1;
              UpdateTvbFlag = 1;
          }
          ++EclusterIndex; // Increment Ecore cluster Index after Threshold 1
        } else {
          DEBUG ((DEBUG_INFO, "(OC) Module Id %d not present\n",ModuleIndex));
          break;
        }
        if (UpdateTvbFlag == 1) {
          // Write the new parameters
          DEBUG ((DEBUG_INFO, "(OC) New TVB Config for Module Index %d and Threshold %d is = 0x%x\n",ModuleIndex, Threshold, TvbCommand.Data));
          OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, TvbCommand.Data, &MailboxStatus);
          if (MailboxStatus != EFI_SUCCESS) {
            DEBUG ((DEBUG_INFO, "(OC) Thermal Velocity Boost downbin message failed, mailbox status = 0x%x\n", MailboxStatus));
          } else {
            Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &TvbCommand.Data, &MailboxStatus);
            if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
              DEBUG ((DEBUG_INFO, "(OC) Reading back the recently written TVB Config for Module Index %d is = 0x%x\n",ModuleIndex, TvbCommand.Data));
            }
          }
        }
      }
    }
  }
  return Status;
}

/**
  Program OC TVB Parameters Per Pcore TRL Groups.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER      If user input if out of range.
**/
EFI_STATUS
STATIC
ProgramTvbThresholdsPerPcoreGroup (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;
  UINT32                   MailboxStatus;
  UINT8                    GroupIndex;
  OCMB_TVB_DATA            TvbCommand;
  OCTVB_MAILBOX_INTERFACE  OcMailboxReadCommand;
  OCTVB_MAILBOX_INTERFACE  OcMailboxWriteCommand;
  UINT8                    Threshold;
  UINT8                    UpdateTvbFlag;

  UpdateTvbFlag = 0;

  DEBUG ((DEBUG_INFO, "(OC) Program OC TVB Parameters\n"));

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_OC_TVB_CONFIG;

  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_OC_TVB_CONFIG;

  TvbCommand.Data           = 0;

  DEBUG ((DEBUG_INFO, "(OC) Configuring TVB parameters Per Pcore Group\n"));

  for (GroupIndex = 0; GroupIndex < MAX_OC_BIG_CORE_TRL_GROUPS; GroupIndex++) {
    // Limit select
    OcMailboxReadCommand.Fields.Param_19_18  = 0;         // Per Pcore group
    OcMailboxWriteCommand.Fields.Param_19_18 = 0;         // Per Pcore group

    OcMailboxReadCommand.Fields.Param_4_0  = GroupIndex;
    OcMailboxWriteCommand.Fields.Param_4_0 = GroupIndex;

    for (Threshold = 0; Threshold <= 1; Threshold++) {
      //OcMailboxReadCommand.Fields.Param1  |= (Threshold << TVB_THRESHOLD_OFFSET);
      OcMailboxReadCommand.Fields.Param_21_20 = Threshold;
      //OcMailboxWriteCommand.Fields.Param1 |= (Threshold << TVB_THRESHOLD_OFFSET);
      OcMailboxWriteCommand.Fields.Param_21_20 = Threshold;
      DEBUG ((DEBUG_INFO, "(OC) Current TVB Config Cmd Interface data for GroupIndex %d, Threshold %d is 0x%x\n",GroupIndex, Threshold, OcMailboxReadCommand.InterfaceData));
      TvbCommand.Data = 0; //Reset data
      UpdateTvbFlag = 0; // Reset the update flag
      Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &TvbCommand.Data, &MailboxStatus);
      if ((Status == EFI_SUCCESS) && (MailboxStatus == PCODE_MAILBOX_CC_SUCCESS)) {
        DEBUG ((DEBUG_INFO, "(OC) Current TVB Config for Group Index %d and Threshold %d is = 0x%x\n",GroupIndex, Threshold, TvbCommand.Data));
        if (Threshold == 0) {
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerPcoreGrRatioDownBinAboveT0[GroupIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->PcoreTvbTempThreshold0)) {
            DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Pcore GroupIndex %d Threshold 0\n", GroupIndex));
            TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerPcoreGrRatioDownBinAboveT0[GroupIndex];
            TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->PcoreTvbTempThreshold0;
            UpdateTvbFlag = 1;
          }
        } else { //Threshold 1
          if ((TvbCommand.Fields.TvbDownBins  != OverClockingConfig->PerPcoreGrRatioDownBinAboveT1[GroupIndex]) ||
          (TvbCommand.Fields.TvbTempThreshold != OverClockingConfig->PcoreTvbTempThreshold1)) {
            DEBUG ((DEBUG_INFO, "(OC) Updating TVB parameters for Pcore GroupIndex %d Threshold 1\n", GroupIndex));
            TvbCommand.Fields.TvbDownBins      = OverClockingConfig->PerPcoreGrRatioDownBinAboveT1[GroupIndex];
            TvbCommand.Fields.TvbTempThreshold = OverClockingConfig->PcoreTvbTempThreshold1;
            UpdateTvbFlag = 1;
          }
        }
        if (UpdateTvbFlag == 1) {
          // Write the new parameters
          DEBUG ((DEBUG_INFO, "(OC) New TVB Config for Group Index %d and Threshold %d is = 0x%x\n",GroupIndex, Threshold, TvbCommand.Data));
          OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, TvbCommand.Data, &MailboxStatus);
          if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
            DEBUG ((DEBUG_INFO, "(OC) Thermal Velocity Boost downbin message failed, mailbox status = 0x%x\n", MailboxStatus));
          } else {
            Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &TvbCommand.Data, &MailboxStatus);
            if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
              DEBUG ((DEBUG_INFO, "(OC) Reading back the recently written TVB Config for Group Index %d is = 0x%x\n",GroupIndex, TvbCommand.Data));
            }
          }
        }
     }
    }
  }
  return Status;
}


/**
  Program OC TVB Parameters, either Per CCP Module or Per Pcore Group, based on the
  Limit Selection parameter.

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance

  @retval EFI_SUCCESS
  @retval EFI_INVALID_PARAMETER      If user input if out of range.
**/
EFI_STATUS
STATIC
ProgramTvbThresholds (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig
  )
{
  EFI_STATUS               Status;

  DEBUG ((DEBUG_INFO, "(OC) Program OC TVB Parameters\n"));

  switch (OverClockingConfig->TvbConfigLimitSelect) {
    case TVB_CONFIG_PER_CCP:
    {
      Status = ProgramTvbThresholdsPerCcp (OverClockingConfig);
    }
    break;
    case TVB_CONFIG_PER_PCORE_GROUP:
    {
      Status = ProgramTvbThresholdsPerPcoreGroup (OverClockingConfig);
    }
    break;
    default :
     Status = EFI_INVALID_PARAMETER;
    break;
  }
  return Status;
}

/**
  Program OC max voltage limit for core/ring

  @param[in] OverClockingConfig      Pointer to CPU Overclocking Policy instance
  @param[in] InstanceId              Die Index
  @param[in] DomainId                DomainId for Core/Ring
  @param[in] DomainIndex             OC max voltage limit setup Domain Index
  @param[in] InstanceId              Instrance ID

**/
VOID
STATIC
CpuCoreRingOcMaxVoltageLimit (
  IN OVERCLOCKING_PREMEM_CONFIG *OverClockingConfig,
  IN UINT8                      DieIndex,
  IN UINT8                      DomainId,
  IN UINT8                      DomainIndex,
  IN UINT16                     InstanceId
  )
{
  EFI_STATUS               Status;
  UINT32                   MailboxStatus;
  OC_MAILBOX_INTERFACE     OcMailboxReadCommand;
  OC_MAILBOX_INTERFACE     OcMailboxWriteCommand;
  UINT32                   OcMaxVolLimitData = 0;
  UINT16                   TempVoltage = 0;

  OcMailboxReadCommand.InterfaceData = 0;
  OcMailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_READ_MAX_VOLTAGE_LIMIT;

  OcMailboxWriteCommand.InterfaceData = 0;
  OcMailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_WRITE_MAX_VOLTAGE_LIMIT;

  OcMailboxReadCommand.Fields.Param_4_0 = DomainId;
  OcMailboxWriteCommand.Fields.Param_4_0 = DomainId;

  OcMailboxReadCommand.Fields.InstanceId = InstanceId;

  Status = OcMailboxRead (OcMailboxReadCommand.InterfaceData, &OcMaxVolLimitData, &MailboxStatus);
  if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
    //
    // Converts the input voltage target to the fixed point U12.2.10 Volt format
    //
    ConvertVoltageTarget (OverClockingConfig->MaxVoltageLimit[DieIndex][DomainIndex], &TempVoltage, CONVERT_TO_FIXED_POINT_VOLTS);
    if (TempVoltage != 0) {
      Status = OcMailboxWrite (OcMailboxWriteCommand.InterfaceData, TempVoltage, &MailboxStatus);
      if ((Status != EFI_SUCCESS) || (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS)) {
        DEBUG ((DEBUG_ERROR, "(OC) Update Max Voltage failed, mailbox status = 0x%x\n", MailboxStatus));
      }
    }
  }
}

/**
  Configure the maximum voltage limit for a core/ring domain.
  This function looks at the CoreType and set the Max Voltage limit
  for Core Domain: All P/E core on a die will have same limit, only need to program once.
  for Ring Domain: Each C-die will have unique limits.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance

**/
VOID
EFIAPI
CoreRingMaxVoltageLimit (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig
  )
{
  UINT8                    DieIndex;
  UINT8                    DomainId;
  UINT8                    ModuleIndex;
  BOOLEAN                  PCoreOcMaxVoltageLimitFlag;
  BOOLEAN                  ECoreOcMaxVoltageLimitFlag;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  for (DieIndex = 0; DieIndex < MAX_CDIE; DieIndex++) {
    if (mCDiePresenceFlag[DieIndex] != TRUE) {
      continue;
    }
    //
    // Core Domain. 0 : Core
    // All P-core on a die (i.e. same compute complex) will have same limit
    // All E-cores on the die (i.e. same compute complex) will have same limit.
    //
    DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE;
    PCoreOcMaxVoltageLimitFlag = FALSE;
    ECoreOcMaxVoltageLimitFlag = FALSE;
    for (ModuleIndex = 0; ModuleIndex < MAX_OC_CCP_MODULES; ModuleIndex++) {
      if ((mOcModuleAndDieInfo[DieIndex][ModuleIndex].CoreType == CPUID_CORE_TYPE_INTEL_CORE) && (!PCoreOcMaxVoltageLimitFlag)) {
        CpuCoreRingOcMaxVoltageLimit (OverClockingConfig, DieIndex, DomainId, MAILBOX_OC_DOMAIN_ID_IA_CORE, mOcModuleAndDieInfo[DieIndex][ModuleIndex].InstanceId);
        PCoreOcMaxVoltageLimitFlag = TRUE;
      } else if ((mOcModuleAndDieInfo[DieIndex][ModuleIndex].CoreType == CPUID_CORE_TYPE_INTEL_ATOM) && (!ECoreOcMaxVoltageLimitFlag)) {
        CpuCoreRingOcMaxVoltageLimit (OverClockingConfig, DieIndex, DomainId, MAILBOX_OC_DOMAIN_ID_IA_ATOM, mOcModuleAndDieInfo[DieIndex][ModuleIndex].InstanceId);
        ECoreOcMaxVoltageLimitFlag = TRUE;
      }
    }

    //
    // Ring Domain. 2 : Ring
    // Ring/CCF on each C-die will have unique limits.
    //
    DomainId = MAILBOX_OC_DOMAIN_ID_RING;
    for (ModuleIndex = 0; ModuleIndex < MAX_OC_CCP_MODULES; ModuleIndex++) {
      if (mOcModuleAndDieInfo[DieIndex][ModuleIndex].CoreType == 0) {
        continue;
      }
      CpuCoreRingOcMaxVoltageLimit (OverClockingConfig, DieIndex, DomainId, MAILBOX_OC_DOMAIN_ID_RING, mOcModuleAndDieInfo[DieIndex][ModuleIndex].InstanceId);
      break;
    }
  }

  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Configure Per CPU Core OC V/F postmem settings.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
  @param[in] MpServices2Ppi           Pointer to this instance of the MpServices.

**/
VOID
EFIAPI
PerCoreVfOverrides (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig,
  IN EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi
  )
{
  EFI_STATUS                      Status;
  OC_CAPABILITIES_ITEM            OcCapabilities;
  UINT32                          MailboxStatus;
  MISC_GLOBAL_CONFIG              MiscGlobalConfig;
  OC_MAILBOX_INTERFACE            MailboxReadCommand;
  OC_MAILBOX_INTERFACE            MailboxWriteCommand;
  PER_CORE_SETTING_CONTEXT        Context;
  UINT8                           DomainId;

  DEBUG ((DEBUG_INFO, "(OC) %a Start\n", __FUNCTION__));

  ZeroMem (&OcCapabilities, sizeof (OcCapabilities));
  DomainId = MAILBOX_OC_DOMAIN_ID_IA_CORE;
  GetOcCapabilities (&OcCapabilities, DomainId, &MailboxStatus);
  if (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS && (OcCapabilities.Fields.RatioOcSupported || OcCapabilities.Fields.VoltageOverridesSupported || OcCapabilities.Fields.VoltageOffsetSupported)) {
    ///
    /// Program MISC GLOBAL CONFIG (commands 0x14 and 0x15) to allow per-core V/F setting.
    ///
    MailboxReadCommand.InterfaceData = 0;
    MailboxReadCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_GET_MISC_GLOBAL_CONFIG;
    Status = OcMailboxRead (MailboxReadCommand.InterfaceData, &MiscGlobalConfig.Data, &MailboxStatus);
    if ((Status == EFI_SUCCESS) && (MailboxStatus == MAILBOX_OC_COMPLETION_CODE_SUCCESS) && (MiscGlobalConfig.Fields.PerCoreVfOverride == 0)) {
      MiscGlobalConfig.Fields.PerCoreVfOverride |= OverClockingConfig->CoreVfConfigScope;
      MailboxWriteCommand.InterfaceData = 0;
      MailboxWriteCommand.Fields.CommandCompletion = MAILBOX_OC_CMD_SET_MISC_GLOBAL_CONFIG;
      OcMailboxWrite (MailboxWriteCommand.InterfaceData, MiscGlobalConfig.Data, &MailboxStatus);
      if (MailboxStatus != MAILBOX_OC_COMPLETION_CODE_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "(OC) Update MISC GLOBAL CONFIG setting failed, mailbox status = %r\n", MailboxStatus));
        return;
      }
    }
    //
    // Per-core V/F setting via MP service.
    //
    Status = MpServices2Ppi->WhoAmI (MpServices2Ppi, &Context.BspNumber);
    ASSERT_EFI_ERROR (Status);
    Context.MpServices2Ppi     = MpServices2Ppi;
    Context.OverClockingConfig = OverClockingConfig;
    ZeroMem (&mAtomClusterFlag, sizeof (mAtomClusterFlag));

    PerCoreVoltageFrequencySetting (&Context);
    Status = MpServices2Ppi->StartupAllAPs (
                              MpServices2Ppi,
                              (EFI_AP_PROCEDURE) PerCoreVoltageFrequencySetting,
                              TRUE,
                              0,
                              &Context
                              );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "%a: StartupAllAps(): %r\n", __func__, Status));
    }
  }
  DEBUG ((DEBUG_INFO, "(OC) %a End\n", __FUNCTION__));
}

/**
  Initializes post-memory Overclocking settings in the processor.

  @param[in] OverclockingtConfig      Pointer to Policy protocol instance
  @param[in] MpServices2Ppi            Pointer to this instance of the MpServices.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CpuOcInitPostMem (
  IN OVERCLOCKING_PREMEM_CONFIG   *OverClockingConfig,
  IN EDKII_PEI_MP_SERVICES2_PPI   *MpServices2Ppi
  )
{
  EFI_STATUS                                 Status;

  Status = EFI_SUCCESS;
  DEBUG ((DEBUG_INFO, "(OC) CpuOcInitPostMem Start \n"));
  if (OverClockingConfig == NULL || MpServices2Ppi == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  ///
  /// Check for Overclocking support
  ///
  if (OverClockingConfig->OcSupport == 0) {
    DEBUG ((DEBUG_INFO, "(OC) Overclocking is disabled. Skipping Cpu Oc Init PostMem.\n"));
    return EFI_SUCCESS;
  }

  ///
  /// Check all the enabled Modules and collect the information to formulate InstanceID,
  /// which will be used by multiple OCMB commands.
  ///
  CreateInstanceId ();

  ///
  /// Program OC Vmax limits for Core/Ring before VF override using OC Mailbox command 0x28/0x29.
  /// These voltage limit overrides should be programmed before VF override, so that VF override can leverage modifed Vmax limits
  ///
  CoreRingMaxVoltageLimit (OverClockingConfig);

  ///
  /// Per CPU Core OC V/F adjustment if OC is supported and in Legacy Mode.
  ///
  if (OverClockingConfig->VfPointOffsetMode[MAILBOX_OC_DOMAIN_ID_IA_CORE] == 0 && OverClockingConfig->CoreVfConfigScope == 1) {
    PerCoreVfOverrides (OverClockingConfig, MpServices2Ppi);
  }

  ///
  /// Per-Core / AtomCluster Max Core Ratio Override -- P0 Ratio override using Favored Core Control Command (0x1C/0x1D)
  ///
  RatioValueOverride (OverClockingConfig, MpServices2Ppi);

  ///
  /// Per-Module Granular Ratio Override -- Sub-Bin Ratio bins override using Granular Ratio Support Command (0x26/0x27)
  ///
  if (OverClockingConfig->GranularRatioOverride) {
    GranularRatioSupport (OverClockingConfig);
  }

  ///
  /// Program OC TVB parameters if Thermal Turbo and OcTvb control is enabled
  ///
  if ((OverClockingConfig->ThermalTurbo) && (OverClockingConfig->OcTvb == 1)) {
    ///
    /// Find the CCP module index and core type for all the cores
    ///
    GetCoreCcpInfo ();
    Status = MpServices2Ppi->StartupAllAPs (
                              MpServices2Ppi,
                              (EFI_AP_PROCEDURE) GetCoreCcpInfo,
                              TRUE,
                              0,
                              OverClockingConfig
                              );
    if (EFI_ERROR (Status) && (Status != EFI_NOT_STARTED)) {
      DEBUG ((DEBUG_ERROR, "GetCoreCcpInfo: StartupAllAps(): %r\n", Status));
    }
    ProgramTvbThresholds (OverClockingConfig);
  }

  ///
  /// Enable UnderVolt Protection at the end of Post-Mem.
  ///
  UnderVoltProtectionPostMem (OverClockingConfig);

  UpdateOcDataHobPostMem (OverClockingConfig, MpServices2Ppi);

  ///
  /// All OCMB commands are done using the InstanceID linked list, so free up the resources allocated to  the list
  ///
  RemoveList ();
  return Status;
}
