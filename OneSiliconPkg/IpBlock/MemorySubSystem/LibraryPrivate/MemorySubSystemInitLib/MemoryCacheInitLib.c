/** @file
  PEIM to Memory Cache Init Library.

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
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/CpuMailboxLib.h>
#include <Ppi/SiPolicy.h>
#include <HostBridgeConfig.h>
#include "MemoryCacheInitLib.h"


/**
  Memory cache initalization.

  @param[in]  SiPreMemPolicyPpi      The SI PreMem Policy PPI instance

  @retval     EFI_UNSUPPORTED        Unsupported
  @retval     EFI_INVALID_PARAMETER  Invalid input parameter
  @retval     EFI_SUCCES             Initialized successfully
**/
EFI_STATUS
PeiMemoryCacheInit (
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{

  if (SiPreMemPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }


  return EFI_SUCCESS;
}

/**
  Get Fabric GV configuration.

  The function sends a pcode mailbox command to read Fabric GV configuration.

  @param[in]  FabrivGvConfiguration    Pointer to Fabric GV configuration variable where read data will be placed.
                                     The data variable is only valid when functions returns with EFI_SUCESS status.

  @retval     EFI_UNSUPPORTED        Unsupported
  @retval     EFI_INVALID_PARAMETER  Invalid parameter
  @retval     EFI_SUCCES             Fabric GV configuration was read successfully
**/
EFI_STATUS
GetFabricGVConfiguration (
  OUT FABRIC_GV_CONFIGURATION_DATA *FabrivGvConfiguration
  )
{
  UINT32                  MailboxStatus;
  EFI_STATUS              Status;
  PCODE_MAILBOX_INTERFACE MailboxCommand1;
  PCODE_MAILBOX_INTERFACE MailboxCommand2;
  PCODE_MAILBOX_INTERFACE MailboxCommand3;

  if (FabrivGvConfiguration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MailboxCommand1.InterfaceData  = 0;
  MailboxCommand1.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand1.Fields.Param1  = NCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID;
  FabrivGvConfiguration->Data   = 0;
  Status = MailboxRead (MailboxCommand1.InterfaceData, &FabrivGvConfiguration->Data, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Getting Fabric Gv NCLK Configuration. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "NCLK MAILBOX Data: %x\n", FabrivGvConfiguration->Data));

  MailboxCommand2.InterfaceData  = 0;
  MailboxCommand2.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand2.Fields.Param1  = D2DCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID;
  FabrivGvConfiguration->Data   = 0;
  Status = MailboxRead (MailboxCommand2.InterfaceData, &FabrivGvConfiguration->Data, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Getting Fabric Gv D2DCLK Configuration. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "D2DCLK MAILBOX Data: %x\n", FabrivGvConfiguration->Data));

  MailboxCommand3.InterfaceData  = 0;
  MailboxCommand3.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand3.Fields.Param1  = MSCLK_MAILBOX_SUBCOMMAND_GET_CONFIGURATION_ID;
  FabrivGvConfiguration->Data   = 0;
  Status = MailboxRead (MailboxCommand3.InterfaceData, &FabrivGvConfiguration->Data, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Getting Fabric Gv MSCLK Configuration. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }
  DEBUG ((DEBUG_INFO, "MSCLK MAILBOX Data: %x\n", FabrivGvConfiguration->Data));
  return EFI_SUCCESS;
}

/**
  Update Fabric GV configuration.

  The function sends a pcode mailbox command to update Fabric GV configuration.

  @param[in]  FabrivGvConfiguration    Fabric GV configuration which need to be applied

  @retval     EFI_UNSUPPORTED        Unsupported
  @retval     EFI_INVALID_PARAMETER  Invalid input parameter
  @retval     EFI_SUCCES             Fabric GV configuration was updated successfully
**/
EFI_STATUS
SetFabricGVConfiguration(
  IN FABRIC_GV_CONFIGURATION_DATA *FabrivGvConfiguration
)
{
  EFI_STATUS              Status;
  UINT32                  MailboxStatus;
  PCODE_MAILBOX_INTERFACE MailboxCommand1;
  PCODE_MAILBOX_INTERFACE MailboxCommand2;
  PCODE_MAILBOX_INTERFACE MailboxCommand3;

  if (FabrivGvConfiguration == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  MailboxCommand1.InterfaceData  = 0;
  MailboxCommand1.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand1.Fields.Param1  = NCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID;
  Status = MailboxWrite (MailboxCommand1.InterfaceData, FabrivGvConfiguration->Data, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Setting Fabric Gv NCLK Policy. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }

  MailboxCommand2.InterfaceData  = 0;
  MailboxCommand2.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand2.Fields.Param1  = D2DCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID;
  Status = MailboxWrite (MailboxCommand2.InterfaceData, FabrivGvConfiguration->Data, &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Setting Fabric Gv D2DCLK Policy. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }

  MailboxCommand3.InterfaceData  = 0;
  MailboxCommand3.Fields.Command = SAGV_CONFIG_HANDLER_ID;
  MailboxCommand3.Fields.Param1  = MSCLK_MAILBOX_SUBCOMMAND_SET_CONFIGURATION_ID;
  Status = MailboxWrite (MailboxCommand3.InterfaceData, (FabrivGvConfiguration->Data & 0xF), &MailboxStatus);
  if (EFI_ERROR (Status) || (MailboxStatus != PCODE_MAILBOX_CC_SUCCESS)) {
    DEBUG ((DEBUG_WARN, "Error on Setting Fabric Gv MSCLK Policy. EFI_STATUS = %r, Mailbox Status = %X\n", Status, MailboxStatus));
    return EFI_UNSUPPORTED;
  }
  return EFI_SUCCESS;
}

EFI_STATUS
PeiFabricGvInit(
  IN  SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
)
{
  EFI_STATUS                            Status;
  FABRIC_GV_CONFIGURATION_DATA          FabricGvConfiguration;
  HOST_BRIDGE_PREMEM_CONFIG             *HostBridgePreMemConfig;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  if (SiPreMemPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (HostBridgePreMemConfig->FabricGvEnable == 0) {
    DEBUG ((DEBUG_INFO, "Fabric GV is disabled\n"));
    FabricGvConfiguration.Data = 0;
    FabricGvConfiguration.Fields.IsFabricGvEn = 0xFFFFFFFF;

    Status = SetFabricGVConfiguration (&FabricGvConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    DEBUG ((DEBUG_INFO, "Fabric GV configuration is updated\n"));
    DEBUG ((DEBUG_INFO, "IsFabricGvEn: %x\n", HostBridgePreMemConfig->FabricGvEnable));
  } else {
    DEBUG ((DEBUG_INFO, "Fabric GV is enabled\n"));
    FabricGvConfiguration.Data = 0;
    FabricGvConfiguration.Fields.IsFabricGvEn = 0x0;

    Status = SetFabricGVConfiguration (&FabricGvConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //Print updated FabricGV values
  Status = GetFabricGVConfiguration (&FabricGvConfiguration);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
  return EFI_SUCCESS;
}