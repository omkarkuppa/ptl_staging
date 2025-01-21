/** @file
  Mother Board Dxe driver, produces Mother Board Health Aip protocol.

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

#include "MotherBoardHealthDxe.h"

/**
  Get a list of supported information types for this instance of the protocol.

  This function returns a list of InformationType GUIDs that are supported on an
  adapter with this instance of EFI_ADAPTER_INFORMATION_PROTOCOL. The list is returned
  in InfoTypesBuffer, and the number of GUID pointers in InfoTypesBuffer is returned in
  InfoTypesBufferCount.

  @param[in]  This                  A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] InfoTypesBuffer       A pointer to the array of InformationType GUIDs that are supported
                                    by This.
  @param[out] InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.

  @retval EFI_SUCCESS               The list of information type GUIDs that are supported on this adapter was
                                    returned in InfoTypesBuffer. The number of information type GUIDs was
                                    returned in InfoTypesBufferCount.
  @retval EFI_INVALID_PARAMETER     This is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBuffer is NULL.
  @retval EFI_INVALID_PARAMETER     InfoTypesBufferCount is NULL.
  @retval EFI_OUT_OF_RESOURCES      There is not enough pool memory to store the results.

**/
EFI_STATUS
EFIAPI
MotherBoardHealthAipGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  if ((This == NULL) || (InfoTypesBuffer == NULL) || (InfoTypesBufferCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InfoTypesBuffer = AllocateCopyPool (sizeof (gAdapterInfoMotherBoardHealthGuid), &gAdapterInfoMotherBoardHealthGuid);
  if (*InfoTypesBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

/**
  Returns the current state information for the adapter.

  This function returns information of type InformationType from the adapter.
  If an adapter does not support the requested informational type, then
  EFI_UNSUPPORTED is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[out] InformationBlock       The service returns a pointer to the buffer with the InformationBlock
                                     structure which contains details about the data specific to InformationType.
  @param[out] InformationBlockSize   The driver returns the size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The InformationType information was retrieved.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_OUT_OF_RESOURCES       The request could not be completed due to a lack of resources.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlockSize is NULL.

**/
EFI_STATUS
EFIAPI
MotherBoardHealthAipGetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  EFI_AIP_MOTHERBOARD_HEALTH_STATE      *MotherboardAipPtr;
  UINT8                                 *LastUpdateDataHeader;

  if ((This == NULL) || (InformationBlock == NULL) || (InformationBlockSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gAdapterInfoMotherBoardHealthGuid)) {
    return EFI_UNSUPPORTED;
  }

  *InformationBlock = AllocateZeroPool (sizeof (EFI_AIP_MOTHERBOARD_HEALTH_STATE));
  if (*InformationBlock == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MotherboardAipPtr = (EFI_AIP_MOTHERBOARD_HEALTH_STATE *) (*InformationBlock);
  MotherboardAipPtr->Header.Length            = sizeof (EFI_AIP_MOTHERBOARD_HEALTH_STATE);
  MotherboardAipPtr->Header.Revision          = MOTHER_BOARD_AIP_HEADER_REVISION;
  MotherboardAipPtr->Header.Healthy           = (UINT8) TelemetryHealthy;
  CopyGuid (&MotherboardAipPtr->Header.Signature, &gAdapterInfoMotherBoardHealthGuid);
  MotherboardAipPtr->Revision                 = MOTHER_BOARD_AIP_DATA_REVISION;
  MotherboardAipPtr->MeanBootTime             = 0;
  MotherboardAipPtr->CurrentBootTime          = 0;
  MotherboardAipPtr->NumberOfBootsSinceUpdate = 0;

  LastUpdateDataHeader = (UINT8 *) (*InformationBlock) + OFFSET_OF (EFI_AIP_MOTHERBOARD_HEALTH_STATE, LastUpdateTime);
  //
  // LastUpdateTime and Esrtid are initialized to default value
  //
  ZeroMem (LastUpdateDataHeader, sizeof (EFI_LAST_UPDATE_DATA));
  *InformationBlockSize = sizeof (EFI_AIP_MOTHERBOARD_HEALTH_STATE);

  return EFI_SUCCESS;
}

/**
  Sets state information for an adapter.

  This function sends information of type InformationType for an adapter.
  If an adapter does not support the requested information type, then EFI_UNSUPPORTED
  is returned.

  @param[in]  This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in]  InformationType        A pointer to an EFI_GUID that defines the contents of InformationBlock.
  @param[in]  InformationBlock       A pointer to the InformationBlock structure which contains details
                                     about the data specific to InformationType.
  @param[in]  InformationBlockSize   The size of the InformationBlock in bytes.

  @retval EFI_SUCCESS                The information was received and interpreted successfully.
  @retval EFI_UNSUPPORTED            The InformationType is not known.
  @retval EFI_DEVICE_ERROR           The device reported an error.
  @retval EFI_INVALID_PARAMETER      This is NULL.
  @retval EFI_INVALID_PARAMETER      InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED        The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().

**/
EFI_STATUS
EFIAPI
MotherBoardHealthAipSetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gAdapterInfoMotherBoardHealthGuid)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_WRITE_PROTECTED;
}

/**
  Initialize the variable of Mother board.

  This function initializes variable of Mother board.
  If variable cannot be read and error status is EFI_NOT_FOUND,
  the variable will be created.

  @retval EFI_SUCCESS            The function completed successfully.
  @retval other                  Some error occurs when initializing MotherBoardHealth variable.

**/
EFI_STATUS
EFIAPI
InitializeMotherBoardVariable (
  VOID
  )
{
  EFI_STATUS                       Status;
  MOTHER_BOARD_HEALTH_VARIABLE     TelemetryVariable;
  UINTN                            TelemetryBufferSize;


  TelemetryBufferSize = sizeof (MOTHER_BOARD_HEALTH_VARIABLE);
  ZeroMem (&TelemetryVariable, sizeof (MOTHER_BOARD_HEALTH_VARIABLE));

  Status = gRT->GetVariable (
                  MOTHERBOARD_HEALTH_DATA_NAME,
                  &gMotherBoardHealthVariableGuid,
                  NULL,
                  &TelemetryBufferSize,
                  &TelemetryVariable
                  );
  if (Status == EFI_SUCCESS) {
    DEBUG ((EFI_D_INFO, "MotherBoardHealth variable already exists.\n"));
    return Status;
  } else if (Status == EFI_NOT_FOUND) {
    //
    // Initialize the variable of Mother board.
    //
    Status = gRT->SetVariable (
                    MOTHERBOARD_HEALTH_DATA_NAME,
                    &gMotherBoardHealthVariableGuid,
                    EFI_VARIABLE_NON_VOLATILE | EFI_VARIABLE_BOOTSERVICE_ACCESS | EFI_VARIABLE_RUNTIME_ACCESS,
                    sizeof (MOTHER_BOARD_HEALTH_VARIABLE),
                    &TelemetryVariable
                    );
    if (Status != EFI_SUCCESS) {
      DEBUG ((EFI_D_ERROR, "Failed to set the MotherBoardHealth variable. Status: %r\n", Status));
      return Status;
    }
  } else {
    DEBUG ((EFI_D_ERROR, "Failed to get the MotherBoardHealth variable. Status: %r\n", Status));
  }

  return Status;
}

/**
  The user Entry Point for mother board driver health module.

  This is the entry point for Mother Board Driver Health DXE Driver. It
  installs the Adapter Information Protocol.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
MotherBoardHealthDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                       Status;
  MOTHER_BOARD_HEALTH_DEV          *MBHealthPrivate;
  VENDOR_DEVICE_PATH               VendorDeviceNode;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;

  Status                  = EFI_SUCCESS;
  DevicePath              = NULL;

  //
  // Initialize MotherBoardHealth variable.
  //
  Status = InitializeMotherBoardVariable ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitializeMotherBoardVariable failed. Status: %r\n", Status));
    return Status;
  }

  //
  // Allocate the private device structure for mother board health
  //
  MBHealthPrivate = (MOTHER_BOARD_HEALTH_DEV *) AllocateZeroPool (sizeof (MOTHER_BOARD_HEALTH_DEV));
  if (MBHealthPrivate == NULL) {
    DEBUG ((EFI_D_ERROR, "EFI_OUT_OF_RESOURCES to Allocate EFI_ADAPTER_INFORMATION_PROTOCOL. \n"));
    return EFI_OUT_OF_RESOURCES;
  }
  MBHealthPrivate->Handle = NULL;

  MBHealthPrivate->AdapterInfo.GetInformation    = MotherBoardHealthAipGetInfo;
  MBHealthPrivate->AdapterInfo.SetInformation    = MotherBoardHealthAipSetInfo;
  MBHealthPrivate->AdapterInfo.GetSupportedTypes = MotherBoardHealthAipGetSupportedTypes;

  ZeroMem (&VendorDeviceNode, sizeof (VENDOR_DEVICE_PATH));
  VendorDeviceNode.Header.Type    = HARDWARE_DEVICE_PATH;
  VendorDeviceNode.Header.SubType = HW_VENDOR_DP;
  CopyGuid (&VendorDeviceNode.Guid, &gAdapterInfoMotherBoardHealthGuid);

  //
  // Fill VendorDeviceNode.Header.Length[2]
  //
  SetDevicePathNodeLength (&VendorDeviceNode.Header, sizeof (VENDOR_DEVICE_PATH));

  DevicePath = AppendDevicePathNode (NULL, (EFI_DEVICE_PATH_PROTOCOL*) &VendorDeviceNode);
  if (DevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ERROR;
  }
  MBHealthPrivate->MBHealthDevicePath = DevicePath;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &MBHealthPrivate->Handle,
                  &gEfiAdapterInformationProtocolGuid,
                  &MBHealthPrivate->AdapterInfo,
                  &gEfiDevicePathProtocolGuid,
                  MBHealthPrivate->MBHealthDevicePath,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "MotherBoardHealth : faild to install AIP protocol, %r.\n", Status));
    goto ERROR;
  }

  return Status;

ERROR:
  //
  // On error, clean the Aip service context data, and free the memory allocated.
  //
  FreePool (MBHealthPrivate);
  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }

  return Status;
}

