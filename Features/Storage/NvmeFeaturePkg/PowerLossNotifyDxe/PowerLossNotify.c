/**@file
  Power Loss Notification Dxe driver provides functionlity to enable or disable PLN feature.

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

#include "PowerLossNotify.h"

POWER_LOSS_NOTIFY_SETTING_PROTOCOL  *mPlnProtocol = NULL;
BOOLEAN                             mSetPlnController = FALSE;
GLOBAL_REMOVE_IF_UNREFERENCED  VOID *mNvmePassthruRegistration  = NULL;


/**
  Set feature command.

  @param[in]   NvmeDevice       The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   NameSpaceId      Namespace ID
  @param[in]   EnablePLN        Indicate if PLN is enabled or not.

  @return EFI_SUCCESS           Successfully send command to device.
  @return EFI_DEVICE_ERROR      Fail to send command to device.

**/
EFI_STATUS
NvmeSetFeature (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT32                                NameSpaceId,
  IN  UINT8                                 EnablePln
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                  Command;
  EFI_NVM_EXPRESS_COMPLETION               Completion;
  EFI_STATUS                               Status;
  SAVE_FEATURE_DW3                         *ResultDw3;
  SAVE_FEATURE_DW0                         *ResultDw0;
  NVME_ADMIN_SET_FEATURES                  SetFeatureCdw10;

  if (NvmeDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));
  ZeroMem (&SetFeatureCdw10, sizeof (UINT32));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  SetFeatureCdw10.Fid = PLN_FEATURE_ID;
  SetFeatureCdw10.Sv = BIT0;

  Command.Cdw0.Opcode          = NVME_ADMIN_SET_FEATURES_CMD;
  CommandPacket.NvmeCmd->Cdw10 = *((UINT32*) &SetFeatureCdw10);
  CommandPacket.NvmeCmd->Cdw11 = (UINT32)(EnablePln & BIT0);
  CommandPacket.NvmeCmd->Flags = CDW10_VALID | CDW11_VALID;
  CommandPacket.NvmeCmd->Nsid  = NVME_CONTROLLER_ID;
  CommandPacket.CommandTimeout = EFI_TIMER_PERIOD_SECONDS (5);
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Status = NvmeDevice->PassThru (
                         NvmeDevice,
                         NameSpaceId,
                         &CommandPacket,
                         NULL
                         );

  if (!EFI_ERROR (Status)) {
    ResultDw3 = (SAVE_FEATURE_DW3*) &Completion.DW3;
    ResultDw0 = (SAVE_FEATURE_DW0*) &Completion.DW0;
    if ((ResultDw3->StatusCode == 0) && (ResultDw3->StatusCodeType == 0) && ((UINT8)ResultDw0->PlnEnable == EnablePln)) {
      return EFI_SUCCESS;
    }
    return EFI_UNSUPPORTED;
  }

  return Status;
}

/**
  Get feature command.

  @param[in]   NvmeDevice       The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]   Sel              Select field for get feature command.
  @param[in]   NameSpaceId      Namespace ID
  @param[out]  Result           capability status

  @return EFI_SUCCESS           Successfully send command to device.
  @return EFI_DEVICE_ERROR      Fail to send command to device.

**/
EFI_STATUS
NvmeGetFeature (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmeDevice,
  IN  UINT8                                 Sel,
  IN  UINT32                                NameSpaceId,
  OUT UINT32                                *Result
  )
{
  EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET CommandPacket;
  EFI_NVM_EXPRESS_COMMAND                  Command;
  EFI_NVM_EXPRESS_COMPLETION               Completion;
  EFI_STATUS                               Status;
  NVME_ADMIN_GET_FEATURES                  GetFeatureCdw10;

  if (Result == NULL || NvmeDevice == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&CommandPacket, sizeof (EFI_NVM_EXPRESS_PASS_THRU_COMMAND_PACKET));
  ZeroMem (&Command, sizeof (EFI_NVM_EXPRESS_COMMAND));
  ZeroMem (&Completion, sizeof (EFI_NVM_EXPRESS_COMPLETION));
  ZeroMem (&GetFeatureCdw10, sizeof (UINT32));

  CommandPacket.NvmeCmd        = &Command;
  CommandPacket.NvmeCompletion = &Completion;

  GetFeatureCdw10.Fid = PLN_FEATURE_ID;
  GetFeatureCdw10.Sel = Sel;

  Command.Cdw0.Opcode          = NVME_ADMIN_GET_FEATURES_CMD;
  CommandPacket.NvmeCmd->Cdw10 = *((UINT32*) &GetFeatureCdw10);
  CommandPacket.NvmeCmd->Flags = CDW10_VALID;
  CommandPacket.NvmeCmd->Nsid  = NVME_CONTROLLER_ID;
  CommandPacket.CommandTimeout = EFI_TIMER_PERIOD_SECONDS (5);
  CommandPacket.QueueType      = NVME_ADMIN_QUEUE;

  Status = NvmeDevice->PassThru (
                         NvmeDevice,
                         NameSpaceId,
                         &CommandPacket,
                         NULL
                         );

  if (!EFI_ERROR (Status)) {
    CopyMem (Result, &Completion.DW0, sizeof (UINT32));
  }

  return Status;
}

/**
  Enable or disable PLN on device if device support PLN

  @param[in]  NvmePassthru        The pointer to the NVME_PASS_THRU_DEVICE data structure.
  @param[in]  NameSpaceId         Name space ID.
  @param[in]  EnablePln           Enable / Disable PLN feature.

**/
EFI_STATUS
ConfigureDevicePln (
  IN  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL    *NvmePassthru,
  IN  UINT32                                NameSpaceId,
  IN  UINT8                                 EnablePln
  )
{
  EFI_STATUS                         Status;
  UINT8                              PlnVal;
  GET_FEATURE_ATTR_CUR               CurrentAttr;

  //
  // Test if PLN is supported
  //
  Status = NvmeGetFeature (NvmePassthru, FEATURE_SEL_CURRENT, NameSpaceId, (UINT32*) &CurrentAttr);

  if (!EFI_ERROR(Status)) {
    PlnVal = (UINT8) CurrentAttr.PlnEnable;
    DEBUG ((DEBUG_INFO, "PLN - Device support PLN, namespaceid= %d, status: %d -> %d.\n", NameSpaceId, PlnVal, EnablePln));

    if (EnablePln != PlnVal) {
      Status = NvmeSetFeature (NvmePassthru, NameSpaceId, EnablePln);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "  Configured PLN to value: %d was failed. Status: %r \n", PlnVal, Status));
      }
    }
  }

  return Status;
}

/**
  Nvme Pass Thru Protocol notification event handler.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context.

**/
VOID
EFIAPI
NvmePassthruNotificationEvent (
  IN  EFI_EVENT       Event,
  IN  VOID            *Context
  )
{
  EFI_STATUS                         Status;
  UINTN                              BufferSize;
  EFI_HANDLE                         Handle;
  EFI_NVM_EXPRESS_PASS_THRU_PROTOCOL *NvmePassthru;
  UINTN                              PlnSetting;
  UINT32                             NamespaceId;

  PlnSetting = 0;
  BufferSize = sizeof (EFI_HANDLE);
  Status = gBS->LocateHandle (
                  ByRegisterNotify,
                  NULL,
                  mNvmePassthruRegistration,
                  &BufferSize,
                  &Handle
                  );
  if (EFI_ERROR (Status)) {
    return ;
  }

  //
  // Get NvmePassThruProtocol interface
  //
  Status = gBS->HandleProtocol (
                  Handle,
                  &gEfiNvmExpressPassThruProtocolGuid,
                  (VOID **) &NvmePassthru
                  );
  if (EFI_ERROR (Status)) {
    return;
  }

  //
  // Get PLN Setting
  //
  if(mPlnProtocol != NULL) {
    Status = mPlnProtocol->GetPlnSetting (mPlnProtocol, &PlnSetting);
    if (EFI_ERROR (Status)) {
      return;
    }
  }

  //
  // Configured device with desired PLN setting if device support PLN, follow the below flow
  // 1. send command to controller, if it is failed then
  // 2. send command to the rest name space
  //
  Status = ConfigureDevicePln (NvmePassthru, NVME_CONTROLLER_ID, (UINT8)PlnSetting);
  if (EFI_ERROR (Status)) {
    NamespaceId = NVME_ALL_NAMESPACES;
    while (TRUE) {
      Status = NvmePassthru->GetNextNamespace (
                               NvmePassthru,
                               (UINT32 *)&NamespaceId
                               );
      if (EFI_ERROR (Status)) {
        break;
      }

      ConfigureDevicePln (NvmePassthru, NamespaceId, (UINT8)PlnSetting);
    }
  }

  //
  // set the GPIO and EC
  //
  if((mPlnProtocol != NULL) && (!mSetPlnController)) {
    mPlnProtocol->SetPlnController (mPlnProtocol, PlnSetting);
    mSetPlnController = TRUE;
  }

  return;
}

/**
  Driver entry point to register Nvme PassThru Protocol callback.

  @param[in] ImageHandle      The firmware allocated handle for the EFI image.
  @param[in] SystemTable      A pointer to the EFI System Table.

  @retval EFI_SUCCESS         Notify event has been created
**/
EFI_STATUS
EFIAPI
PowerLossNotifyEntryPoint (
  IN EFI_HANDLE                       ImageHandle,
  IN EFI_SYSTEM_TABLE                 *SystemTable
  )
{
  EFI_STATUS                         Status;
  UINTN                              PlnSetting;

  PlnSetting = 0;
  //
  // Check the PLN Setting protocol installed
  //
  Status = gBS->LocateProtocol (
                  &gPowerLossNotifySettingProtocolGuid,
                  NULL,
                  (VOID **) &mPlnProtocol
                  );
  DEBUG ((DEBUG_INFO, "PLN - PowerLostNotifyEntryPoint\n"));
  if (!EFI_ERROR (Status)) {
    //
    // Get the Pln Setting
    //
    mPlnProtocol->GetPlnSetting (mPlnProtocol, &PlnSetting);
    if((PlnSetting == PLN_DISABLE) || (PlnSetting == PLN_ENABLE)) {
      //
      // Register NvmePassthruNotificationEvent() notify function.
      //
      EfiCreateProtocolNotifyEvent (
        &gEfiNvmExpressPassThruProtocolGuid,
        TPL_CALLBACK,
        NvmePassthruNotificationEvent,
        NULL,
        &mNvmePassthruRegistration
        );
    }
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}
