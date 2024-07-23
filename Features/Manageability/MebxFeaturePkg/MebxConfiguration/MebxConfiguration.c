/** @file
  This is the driver that set/get Mebx Config data.

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

@par Specification Reference:
**/

#include <Base.h>
#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/HobLib.h>
#include <Protocol/MebxConfigProtocol.h>
#include <MebxData.h>
#include <MeBiosPayloadHob.h>

EFI_STATUS
EFIAPI
SetMebxConfig (
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  );

EFI_STATUS
EFIAPI
GetMebxConfig (
  IN OUT  UINTN                    *DataSize,
     OUT  VOID                     *Data
  );

MEBX_CONFIG_PROTOCOL               mMebxConfig = {
  MEBX_CONFIG_PROTOCOL_REVISION,
  SetMebxConfig,
  GetMebxConfig
};

/**
  This function is used to set MEBx settings.

  @param[in]  DataSize           The size in bytes of the MEBx Data buffer.
  @param[in]  Data               The contents for the MEBx Data.

  @retval EFI_SUCCESS            MEBx config saved.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold MEBx data.
  @retval EFI_DEVICE_ERROR       MEBx config could not be retrieved due to a hardware error.
**/
EFI_STATUS
EFIAPI
SetMebxConfig (
  IN  UINTN                        DataSize,
  IN  VOID                         *Data
  )
{
  EFI_STATUS              Status;
  MEBX_DATA               MebxData;
  UINTN                   VariableSize;
  EFI_HANDLE              Handle;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  if (DataSize != sizeof (MEBX_DATA)) {
    return EFI_INVALID_PARAMETER;
  }

  VariableSize = DataSize;
  Status = gRT->GetVariable (
                  MEBX_DATA_VARIABLE_NAME,
                  &gMebxDataGuid,
                  NULL,
                  &VariableSize,
                  &MebxData
                  );

  if ((!EFI_ERROR (Status) && (CompareMem (&Data, &MebxData, VariableSize) != 0)) ||
      (Status == EFI_NOT_FOUND)) {
    DEBUG ((DEBUG_INFO, "Update MebxData Variable.\n"));
    Status = gRT->SetVariable (
                    MEBX_DATA_VARIABLE_NAME,
                    &gMebxDataGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS,
                    DataSize,
                    Data
                    );

    Handle = NULL;
    Status = gBS->InstallProtocolInterface (
                    &Handle,
                    &gUpdateSmbiosHwAssetTableGuid,
                    EFI_NATIVE_INTERFACE,
                    NULL
                    );
    ASSERT_EFI_ERROR (Status);

  }

  DEBUG ((DEBUG_INFO, "%a - %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This function is used to retrieve MEBx settings.

  @param[in,out] DataSize        The size in bytes of the MEBx data buffer.
  @param[out]    Data            The pointer to MEBx configuration data buffer.

  @retval EFI_SUCCESS            MEBx config was successfully retrieved.
  @retval EFI_INVALID_PARAMETER  Parameter Data or Datasize is NULL.
  @retval EFI_DEVICE_ERROR       MEBx config could not be retrieved due to a hardware error.
**/
EFI_STATUS
EFIAPI
GetMebxConfig (
  IN OUT  UINTN                    *DataSize,
     OUT  VOID                     *Data
  )
{
  EFI_STATUS    Status;

  DEBUG ((DEBUG_INFO, "%a enter\n", __FUNCTION__));

  if (Data == NULL || DataSize == NULL || *DataSize != sizeof (MEBX_DATA)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = gRT->GetVariable (
                  MEBX_DATA_VARIABLE_NAME,
                  &gMebxDataGuid,
                  NULL,
                  DataSize,
                  Data
                  );

  DEBUG ((DEBUG_INFO, "%a - %r.\n", __FUNCTION__, Status));

  return Status;
}

/**
  This is the standard EFI driver entrypoint to install protocol to set/get MEBX data from silicon package.

  @param[in] ImageHandle         Handle for the image of this driver
  @param[in] SystemTable         Pointer to the EFI System Table

  @retval EFI_SUCCESS            The protocol interface was installed.
  @retval EFI_OUT_OF_RESOURCES   Space for a new handle could not be allocated.
  @retval EFI_INVALID_PARAMETER  Handle or Protocol is NULL.
**/
EFI_STATUS
EFIAPI
MebxConfigEntry (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS            Status;
  ME_BIOS_PAYLOAD_HOB   *MbpHob;

  MbpHob = NULL;
  MbpHob = GetFirstGuidHob (&gMeBiosPayloadHobGuid);
  if (MbpHob != NULL) {
    if (MbpHob->MeBiosPayload.FwPlatType.RuleData.Fields.IntelMeFwImageType != IntelMeCorporateFw ||
        MbpHob->MeBiosPayload.FwCapsSku.FwCapabilities.Fields.Amt != 1) {
      return EFI_UNSUPPORTED;
    }
  }

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gMebxConfigProtocolGuid,
                  &mMebxConfig,
                  NULL
                  );

  ASSERT_EFI_ERROR (Status);
  return Status;
}
