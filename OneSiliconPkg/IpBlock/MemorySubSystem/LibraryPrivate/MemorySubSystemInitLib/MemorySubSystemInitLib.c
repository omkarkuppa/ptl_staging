/** @file
  PEIM to Memory Map Init Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <MemorySubSystemHob.h>
#include <Library/HobLib.h>
#include <MemorySubSystemHandle.h>
#include "MemoryCacheInitLib.h"
#include <Library/CpuMailboxLib.h>
#include <Library/TmeLib.h>
#include <Library/PeiTdxLib.h>
#include <Ppi/SiPolicy.h>
#include <HostBridgeConfig.h>
#include <TelemetryPeiConfig.h>
#include <Library/PeiTelemetryFruLib.h>
#include <TraceHubDataHob.h>
#include <Defines/PchReservedResources.h>
#include <Library/TxtLib.h>
#include <Txt.h>
#include <Library/IGpuInfoLib.h>
#include <MemorySubSystemConfig.h>
#include <Library/TdxInfoLib.h>
#include <Library/TdxFruLib.h>
#include <IGpuDataHob.h>
#include <Library/PeiVtdInitFruLib.h>
#include <Register/B2pMailbox.h>

/**
  Updates MemorySubSystem Data Hob

  @param[in] MeStolenBase  Me Stolen Base
  @param[in] MeStolenSize  Me Slolen Size
**/
VOID
EFIAPI
UpdateMemorySubSystemDataHob (
  UINT32         MeStolenBase,
  UINT32         MeStolenSize
  )
{
  MEMORY_SUB_SYSTEM_DATA_HOB  *MemorySubSystemHob;

    DEBUG ((DEBUG_INFO, "%a Start \n", __FUNCTION__));
  //
  // Update Hob based on data received from MRC
  //
  MemorySubSystemHob = (MEMORY_SUB_SYSTEM_DATA_HOB *) GetFirstGuidHob (&gMemorySubSystemHobGuid);
  if (MemorySubSystemHob != NULL) {
    MemorySubSystemHob->MeStolenBase = MeStolenBase;
    MemorySubSystemHob->MeStolenSize = MeStolenSize;
  }
  DEBUG ((DEBUG_INFO, "%a End \n", __FUNCTION__));
}
/**
  Initialize Security Data from config blocks

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
EFIAPI
InitializeSecurityPolicy (
  IN  SI_PREMEM_POLICY_PPI *SiPreMemPolicyPpi,
  IN  MrcInput             *Inputs,
  OUT TME_POLICY           *TmePolicy,
  OUT TDX_POLICY           *TdxPolicy,
  OUT UINTN                *TdxActmModuleAddr,
  OUT UINT32               *TdxActmModuleSize,
  OUT UINT8                *TdxSeamldrSvn
  )
{
  EFI_STATUS Status = EFI_SUCCESS;
  CPU_INIT_PREMEM_CONFIG     *CpuInitPreMemConfig = NULL;
  CPU_SECURITY_PREMEM_CONFIG *CpuSecurityPreMemConfig = NULL;
  VTD_CONFIG                 *VtdConfig = NULL;

  if (TmePolicy == NULL ||
      TdxPolicy == NULL ||
      TdxActmModuleAddr == NULL ||
      TdxActmModuleSize == NULL ||
      TdxSeamldrSvn == NULL) {
    Status = EFI_INVALID_PARAMETER;
    return Status;
  }

  // Initialize default values TmePolicy
  TmePolicy->TmeEnable            = FALSE;
  TmePolicy->TmeBypassCapability  = FALSE;
  TmePolicy->TmeExcludeBase       = Inputs->TmeExcludeBase;
  TmePolicy->TmeExcludeSize       = Inputs->TmeExcludeSize;
  TmePolicy->GenerateNewTmeKey    = Inputs->GenerateNewTmeKey;
  TmePolicy->TdxEnable            = FALSE;
  TmePolicy->VtdEnable            = FALSE;

  // Initialize default values TdxPolicy
  TdxPolicy->MktmeEnable          = FALSE;
  TdxPolicy->TdxEnable            = FALSE;
  TdxPolicy->VmxEnable            = FALSE;
  TdxPolicy->VtdEnable            = FALSE;
  *TdxActmModuleAddr              = 0;
  *TdxActmModuleSize              = 0;
  *TdxSeamldrSvn                  = 0xFF;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuInitPreMemConfigGuid, (VOID *) &CpuInitPreMemConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetConfigBlock gCpuInitPreMemConfigGuid Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetConfigBlock gCpuSecurityPreMemConfigGuid Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *) &VtdConfig);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "GetConfigBlock gVtdConfigGuid Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  if (CpuInitPreMemConfig != NULL) {
    TmePolicy->TmeEnable = CpuInitPreMemConfig->TmeEnable;
    TmePolicy->TmeBypassCapability = CpuInitPreMemConfig->TmeBypassCapability;
    if (CpuSecurityPreMemConfig != NULL) {
      TmePolicy->TdxEnable = CpuSecurityPreMemConfig->TdxEnable;
      TdxPolicy->TdxEnable = CpuSecurityPreMemConfig->TdxEnable;
      TdxPolicy->MktmeEnable = CpuInitPreMemConfig->TmeEnable;
      TdxPolicy->VmxEnable = CpuInitPreMemConfig->VmxEnable;
      *TdxActmModuleAddr   = CpuSecurityPreMemConfig->TdxActmModuleAddr;
      *TdxActmModuleSize   = CpuSecurityPreMemConfig->TdxActmModuleSize;
      *TdxSeamldrSvn       = CpuSecurityPreMemConfig->TdxSeamldrSvn;
    }
  }

  if (VtdConfig != NULL) {
    if (IsVtdDisabled(VtdConfig) == FALSE) {
      TmePolicy->VtdEnable = TRUE;
      TdxPolicy->VtdEnable = TRUE;
    }
  }

  return Status;
}

/**
  Installs MemorySubSystem Data Hob

  @retval EFI_SUCCESS           The HOB was successfully created.
  @retval EFI_OUT_OF_RESOURCES  There is no additional space for HOB creation.
**/
EFI_STATUS
EFIAPI
InstallMemorySubSystemDataHob (
  VOID
  )
{
  EFI_STATUS                  Status;
  MEMORY_SUB_SYSTEM_DATA_HOB  *MemorySubSystemHob;

  ///
  /// Create HOB for MemorySubSystemHob Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (MEMORY_SUB_SYSTEM_DATA_HOB),
             (VOID **) &MemorySubSystemHob
             );
  ASSERT_EFI_ERROR (Status);
  ///
  /// Initialize default HOB data
  ///
  CopyGuid (&MemorySubSystemHob->EfiHobGuidType.Name, &gMemorySubSystemHobGuid);
  MemorySubSystemHob->Revision = MEMORY_SUB_SYSTEM_DATA_HOB_REVISION;
  DEBUG ((DEBUG_INFO, "MemorySubSystemHob->EfiHobGuidType.Name: %g\n", &MemorySubSystemHob->EfiHobGuidType.Name));

  DEBUG ((DEBUG_INFO, "MemorySubSystemHob @ %X\n", MemorySubSystemHob));
  DEBUG ((DEBUG_INFO, "MemorySubSystemHobSize - HobHeader: %X\n", sizeof (MEMORY_SUB_SYSTEM_DATA_HOB) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "MemorySubSystemHobSize: %X\n", sizeof (MEMORY_SUB_SYSTEM_DATA_HOB)));
  DEBUG ((DEBUG_INFO, "MemorySubSystemHob Revision : 0x%x\n", MemorySubSystemHob->Revision));

  return Status;
}

/**
  Initialization of Memory map

  @param[in]  SiPreMemPolicyPpi  The SI PreMem Policy PPI instance
  @param[in]  MrcData            The Global MRC data struct

  @retval     EFI_SUCCES         Memory map initalization complete.
**/
EFI_STATUS
PeiMemoryMapInit (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  MrcParameters            *CONST MrcData
  )
{
  EFI_STATUS                 Status;
  MEMORY_SUB_SYSTEM_HANDLE   MemorySubSystemHandle;
  MEMORY_SUB_SYSTEM_CALLBACK MemorySubSystemCallback;
  MrcOutput                  *Outputs;
  MrcInput                   *Inputs;
  TME_POLICY                 TmePolicy;
  UINTN                      TdxActmModuleAddr;
  UINT32                     TdxActmModuleSize;
  TDX_POLICY                 TdxPolicy;
  UINT64                     Tom;
  UINT32                     TsegSize;
  UINT32                     MeStolenBase;
  UINT32                     MeStolenSize;
  UINT32                     PrmrrBase;
  UINT32                     PrmrrSize;
  UINT32                     TotalPhysicalMemorySize;
  UINT32                     ToludBase;
  UINT32                     TouudBase;
  UINT32                     TsegBase;
  UINT32                     BdsmBase;
  UINT32                     GttBase;
  UINT32                     Mc0Size;
  UINT32                     Mc1Size;
  UINT32                     RemapBase;
  UINT32                     RemapLimit;
  MrcMemoryMap               *MemoryMapData;
  UINT32                     SeamrrSize;
  BOOLEAN                    IsDdr5;
  UINT8                      TdxSeamldrSvn;

  DEBUG ((DEBUG_INFO, "%a - Start...\n", __FUNCTION__));

  Outputs = &MrcData->Outputs;
  Inputs = &MrcData->Inputs;
  MemoryMapData = &MrcData->Outputs.MemoryMapData;
  IsDdr5 = Outputs->IsDdr5;
  TsegSize = Inputs->TsegSize;
  PrmrrSize = Inputs->PrmrrSize;
  MeStolenSize = Inputs->MeStolenSize;
  TotalPhysicalMemorySize = MemoryMapData->TotalPhysicalMemorySize;

  Status = InitializeSecurityPolicy (SiPreMemPolicyPpi, Inputs, &TmePolicy, &TdxPolicy, &TdxActmModuleAddr, &TdxActmModuleSize, &TdxSeamldrSvn);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitializeSecurityPolicy () Status = %r\n", Status));
    ASSERT_EFI_ERROR (Status);
  }

  Status = InstallMemorySubSystemDataHob ();
  ASSERT_EFI_ERROR (Status);


  MemorySubSystemHandleInit (&MemorySubSystemHandle, &MemorySubSystemCallback);

  ///
  /// Configure MKTME
  ///
  /// The specified TME exclude base plus size must not be greater than the
  /// total physical memory (TOM). Note: The TOM is specified in MB therefore
  /// the value needs to be shifted left 20.
  Tom = LShiftU64 (TotalPhysicalMemorySize, 20);
  if (TmePolicy.TmeExcludeBase + TmePolicy.TmeExcludeSize > Tom) {
    TmePolicy.TmeExcludeBase = 0;
    TmePolicy.TmeExcludeSize = 0;
    DEBUG ((DEBUG_INFO, "TmeExcludeBase + TmeExcludeSize more than TOM: 0x%llx.\n", Tom));
    DEBUG ((DEBUG_INFO, "No memory range is excluded, defaulting base and size to 0.\n"));
  }
  DEBUG ((DEBUG_INFO, "TmeExcludeBase: 0x%llx, TmeExcludeSize: 0x%llx, TOM: 0x%llx.\n",
    TmePolicy.TmeExcludeBase, TmePolicy.TmeExcludeSize, Tom));

  //
  // Call TmeInit
  //
  MemorySubSystemCallback.TmeInit ((VOID *)&TmePolicy);

  //
  // Set the Memory Map
  //
  MemorySubSystemCallback.SetMemoryMap (MrcData);

  //
  // Update the Variables here because MemoryMapData available after SetMemoryMap
  //
  MeStolenBase = MemoryMapData->MeStolenBase;
  PrmrrBase = MemoryMapData->PrmrrBase;
  ToludBase = MemoryMapData->ToludBase;
  TouudBase = MemoryMapData->TouudBase;
  TsegBase = MemoryMapData->TsegBase;
  BdsmBase = MemoryMapData->BdsmBase;
  GttBase = MemoryMapData->GttBase;
  Mc0Size = MemoryMapData->McMemorySize[0];
  Mc1Size = MemoryMapData->McMemorySize[1];
  RemapBase = MemoryMapData->RemapBase;
  RemapLimit = MemoryMapData->RemapLimit;
  SeamrrSize = CalculateSeamrrSize (Outputs);

  UpdateMemorySubSystemDataHob (MeStolenBase, MeStolenSize);

  //
  // This function Program NOC IMR Exclusion
  //
  MemorySubSystemCallback.SetNocImrExclusion (SiPreMemPolicyPpi, MeStolenBase, MeStolenSize, PrmrrBase, PrmrrSize, TdxPolicy.TdxEnable, SeamrrSize, TotalPhysicalMemorySize);

  //
  // This function Program VGA IMR Exclusion
  //
  MemorySubSystemCallback.SetVgaImrExclusion ();

  //
  // This function Program IA IMR Exclusion below 4GB
  //
  MemorySubSystemCallback.SetIaImrExclusionBelow4Gb (MeStolenBase, MeStolenSize);


  //
  // This function calls into MemoryMap APIs to program memory Map.
  //
  MemorySubSystemCallback.HostBridgeMemoryMapInit (TotalPhysicalMemorySize, ToludBase, TouudBase, TsegBase, BdsmBase, GttBase, Mc0Size, Mc1Size);

  //
  // This function is to set IGPU Memory map.
  //
  MemorySubSystemCallback.SetIGpuMemMap ();

  //
  // Program IMR and IMR_1M
  //
  MemorySubSystemCallback.ImrInit (TsegSize);

  //
  // This function programs CMI NOC Registers
  //
  MemorySubSystemCallback.NocInit (Mc0Size, Mc1Size, RemapBase, RemapLimit, IsDdr5);

  //
  // This function programs CCE IP
  //
  MemorySubSystemCallback.CceInit (RemapBase, RemapLimit);

  //
  // This function programs Tdx IP
  //
  MemorySubSystemCallback.TdxInit ((VOID *)&TdxPolicy, MrcData, TdxActmModuleAddr, TdxActmModuleSize, TdxSeamldrSvn);

  ///
  /// Call TXT related functions here
  ///
  MemorySubSystemCallback.AliasCheck ();  // Txt library function
  if (Inputs->TxtClean) {
    if (Outputs->TxtScrubSuccess) {
      MemorySubSystemCallback.ClearSecretsBit ();
    } else {
      MemorySubSystemCallback.SetPowerDownRequest (1);  // PdrSrc = 1 (MRC requested Power Down due to scrubbing failure)
    }
  }

  //
  // lock the MC and memory map registers.
  //
  MemorySubSystemCallback.CpuSetPrmrrRegion (LShiftU64(PrmrrBase, 20), (PrmrrSize << 20));
  MemorySubSystemCallback.LockDpr (
    );

  //
  // This function Lock Memory Map registers
  //
  MemorySubSystemCallback.LockHostBridgeMemoryMapRegs ();

  //
  // MEM_CFG_DONE should be at the end of this function.
  // place all other function calls above
  //
  MemorySubSystemCallback.SetMemConfigDone ();
  DEBUG ((DEBUG_INFO, "EndOfMrcSocMemCfg Init - End\n"));

  return EFI_SUCCESS;
}

/**
  Initialization of Memory SubSystem.

  @param[in]  SiPreMemPolicyPpi  The SI PreMem Policy PPI instance
  @param[in]  MrcData            The Global MRC data struct

  @retval     EFI_SUCCES         Initialized successfully
**/
EFI_STATUS
EFIAPI
PeiMemorySubSystemInit (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi,
  IN  MrcParameters            *CONST MrcData
  )
{
  DEBUG ((DEBUG_INFO, "%a - Start...\n", __FUNCTION__));

  //
  // Memory map initalization
  //
  PeiMemoryMapInit (SiPreMemPolicyPpi, MrcData);

  //
  // Memory Cache initialization
  //
  PeiMemoryCacheInit (SiPreMemPolicyPpi);

  DEBUG ((DEBUG_INFO, "%a - End...\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  Initialization of IMR Exclusion Registers.

  @retval     EFI_SUCCES         Initialized successfully
**/
EFI_STATUS
EFIAPI
PeiImrExclusionInit (
  VOID
  )
{
  MEMORY_SUB_SYSTEM_HANDLE   MemorySubSystemHandle;
  MEMORY_SUB_SYSTEM_CALLBACK MemorySubSystemCallback;

  DEBUG ((DEBUG_INFO, "%a - Start...\n", __FUNCTION__));

  MemorySubSystemHandleInit (&MemorySubSystemHandle, &MemorySubSystemCallback);

  //
  // This function Program IA IMR Exclusion above 4Gb
  //
  MemorySubSystemCallback.SetIaImrExclusionAbove4Gb ();

  //
  // Lock the IA exclusion
  //
  MemorySubSystemCallback.SetIaImrExclusionLock ();
  DEBUG ((DEBUG_INFO,  "Memory map registers are locked\n"));

  DEBUG ((DEBUG_INFO, "%a - End...\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/*
  This function is to poll Path_to_memory_available with BIOS Mailbox Command which indicates Path to memory available.

  @retval EFI_STATUS     - Successful if bit is set.
*/
EFI_STATUS
PollPathToMemAvailable (
  VOID
  )
{
  UINT32                          MailboxStatus;
  MAILBOX_DATA_MISC_CONFIG_STATUS MailboxData;
  PCODE_MAILBOX_INTERFACE         MailboxCommand;

  DEBUG ((DEBUG_INFO, "PollPathToMemAvailable - Start\n"));

  MailboxCommand.InterfaceData  = 0;
  MailboxData.Data32            = 0;
  MailboxCommand.Fields.Command = MAILBOX_PCODE_CMD_READ_MISC_CONFIG_STATUS;

  do {
    MailboxRead (MailboxCommand.InterfaceData, &MailboxData.Data32, &MailboxStatus);
    if (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Error Reading Path to Memory Available. Mailbox Status = %X\n", MailboxStatus));
      break;
    }
  } while (MailboxData.Fields.PatchToMemAvailable == 0);

  DEBUG ((DEBUG_INFO, "PollPathToMemAvailable - End\n"));

  return EFI_SUCCESS;
}
