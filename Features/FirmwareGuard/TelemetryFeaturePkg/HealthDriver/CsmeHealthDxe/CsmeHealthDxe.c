/** @file
  CSME FW Health Data Initialization for Telemetry.

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
#include <Library/HobLib.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/DevicePathLib.h>
#include <TelemetryCommon.h>
#include <Protocol/DevicePath.h>
#include <MeFwHob.h>
#include <Register/HeciRegs.h>
#include <Library/MeInfoLib.h>
#include <MeDefines.h>
#include <IndustryStandard/Pci30.h>

typedef struct {
  EFI_HANDLE                                  Handle;
  EFI_ADAPTER_INFORMATION_PROTOCOL            AdapterInfo;
  EFI_DEVICE_PATH_PROTOCOL                    *CsmeDevicePath;
} CSME_HEALTH_DEV;

/**
  This function gets the CSME health data.

  @param[in] This                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in] InformationType           A pointer to gAdapterInfoCsmeGuid that defines the contents of InformationBlock.
                                        The caller must use the InformationType to specify the infromation it needs to retrieve
                                        from this service and to determine how to parse the InformationBlock.
                                        The driver should not attempt to free InformationType.
  @param[out] InformationBlock         A pointer to the number of GUIDs present in InfoTypesBuffer.
  @param[out] InformationBlockSize     The driver return the size of the InformationBlock.

  @retval EFI_INVALID_PARAMETER
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_UNSUPPORTED
  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CsmeHealthDataAipGetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  OUT VOID                              **InformationBlock,
  OUT UINTN                             *InformationBlockSize
  )
{
  UINT8                           IsHealthy;
  EFI_AIP_CSME_HEALTH_STATE       *HealthDataBlockPtr;
  UINT64                          HeciBaseAddress;
  ME_FW_HOB                       *MeFwHob;
  HECI_FWS_REGISTER               FwSts1;
  HECI_GS_SHDW_REGISTER           FwSts2;
  HECI_FW_STS6_REGISTER           FwSts6;
  UINTN                           BlockSize;

  if ((This == NULL) || (InformationBlock == NULL) || (InformationBlockSize == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  if (!CompareGuid (InformationType, &gAdapterInfoCsmeGuid)) {
    return EFI_UNSUPPORTED;
  }

  DEBUG ((DEBUG_INFO, "CsmeHealthDataAipGetInfo : Getting CSME Health Data ...\n"));

  *InformationBlockSize = 0;
  *InformationBlock     = NULL;
  IsHealthy  = 0;

  BlockSize = sizeof(EFI_AIP_CSME_HEALTH_STATE);
  HealthDataBlockPtr = (EFI_AIP_CSME_HEALTH_STATE *) AllocateZeroPool (BlockSize);
  if (HealthDataBlockPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "CsmeHealthDataAipGetInfo : Out of resource to allocate for EFI_AIP_CSME_HEALTH_STATE.\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // If HECI device is available, read current FWSTS1 and FWSTS2 from config space first.
  // If HECI device is not available, get info from ME_FW_HOB.
  //
  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  if (PciSegmentRead16 (HeciBaseAddress + PCI_DEVICE_ID_OFFSET) != 0xFFFF) {
    DEBUG ((DEBUG_INFO, "CsmeHealthDataAipGetInfo : Reading FWSTS from registers ... \n"));
    FwSts1.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS);
    FwSts2.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);
    FwSts6.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_6);
    DEBUG ((DEBUG_INFO, "CsmeHealthDataAipGetInfo : FwSts1 = 0x%08x, FwSts2 = 0x%08x \n", FwSts1.ul, FwSts2.ul));
    DEBUG ((DEBUG_INFO, "                         : FwSts6 = 0x%08x \n", FwSts6.ul));
  } else {
    DEBUG ((DEBUG_INFO, "CsmeHealthDataAipGetInfo : HECI device is not availbale so reading FWSTS from HOB ... \n"));
    MeFwHob = GetFirstGuidHob (&gMeFwHobGuid);
    if (MeFwHob != NULL) {
      FwSts1.ul = MeFwHob->Group[HECI1].Reg[0];
      FwSts2.ul = MeFwHob->Group[HECI1].Reg[1];
      FwSts6.ul = MeFwHob->Group[HECI1].Reg[5];
      DEBUG ((DEBUG_INFO, "CsmeHealthDataAipGetInfo : FwSts1 = 0x%08x, FwSts2 = 0x%08x \n", FwSts1, FwSts2));
      DEBUG ((DEBUG_INFO, "                         : FwSts6 = 0x%08x \n", FwSts6));
    } else {
      DEBUG ((DEBUG_ERROR, "CsmeHealthDataAipGetInfo : ME FW Hob is not available \n"));
      FreePool (HealthDataBlockPtr);
      return EFI_DEVICE_ERROR;
    }
  }

  //
  // Store CSME Health Data
  //
  HealthDataBlockPtr->Revision           = CSME_AIP_DATA_REVISION;
  HealthDataBlockPtr->CurrentState       = (UINT8)FwSts1.r.CurrentState;
  HealthDataBlockPtr->FwInitComplete     = (UINT8)FwSts1.r.FwInitComplete;
  HealthDataBlockPtr->MfsFailure         = (UINT8)FwSts2.r.MfsFailure;
  HealthDataBlockPtr->MeOperationMode    = (UINT8)FwSts1.r.MeOperationMode;
  HealthDataBlockPtr->ErrorCode          = (UINT8)FwSts1.r.ErrorCode;
  //
  // Replace ManufacturingMode with SpiProtectionMode
  //
  HealthDataBlockPtr->SpiProtectionMode =  (UINT8)FwSts1.r.SpiProtectionMode;
  HealthDataBlockPtr->FpfSocConfigLock   = (UINT8)FwSts6.r.FpfSocConfigLock;
  HealthDataBlockPtr->FwUpdateInprogress = (UINT8)FwSts1.r.FwUpdateInprogress;

  //
  // Evaluate if it is healthy
  //
  IsHealthy = TelemetryUnhealthy;
  if ((HealthDataBlockPtr->CurrentState == ME_STATE_NORMAL) &&  // ME is in normal working state
      (HealthDataBlockPtr->MfsFailure == 0x0)       &&  // No MFS failure detected
      (HealthDataBlockPtr->MeOperationMode == 0x0)  &&  // Normal operation mode
      (HealthDataBlockPtr->ErrorCode == 0x0)) {         // No error
    IsHealthy = TelemetryHealthy;                      // 1: healthy
  }

  //
  // Create the data header
  //
  HealthDataBlockPtr->Header.Length     = sizeof(EFI_AIP_CSME_HEALTH_STATE);
  HealthDataBlockPtr->Header.Revision   = CSME_AIP_HEADER_REVISION;
  HealthDataBlockPtr->Header.Healthy    = IsHealthy;
  CopyGuid (&HealthDataBlockPtr->Header.Signature, &gAdapterInfoCsmeGuid);

  //
  // Print CSME health data block
  //
  DEBUG ((DEBUG_INFO, "=== EFI_AIP_CSME_HEALTH_STATE  ==\n"));
  DEBUG ((DEBUG_INFO, " EFI_AIP_HEALTH_HEADER \n"));
  DEBUG ((DEBUG_INFO, "     Length     = 0x%04x \n",     HealthDataBlockPtr->Header.Length));
  DEBUG ((DEBUG_INFO, "     Revision   = 0x%02x \n",     HealthDataBlockPtr->Header.Revision));
  DEBUG ((DEBUG_INFO, "     Healthy    = 0x%02x \n",     HealthDataBlockPtr->Header.Healthy));
  DEBUG ((DEBUG_INFO, "     Signature  = %g \n",         HealthDataBlockPtr->Header.Signature));
  DEBUG ((DEBUG_INFO, " Revision           = 0x%02x \n", HealthDataBlockPtr->Revision));
  DEBUG ((DEBUG_INFO, " FwInitComplete     = 0x%02x \n", HealthDataBlockPtr->FwInitComplete));
  DEBUG ((DEBUG_INFO, " CurrentState       = 0x%02x \n", HealthDataBlockPtr->CurrentState));
  DEBUG ((DEBUG_INFO, " MfsFailure         = 0x%02x \n", HealthDataBlockPtr->MfsFailure));
  DEBUG ((DEBUG_INFO, " MeOperationMode    = 0x%02x \n", HealthDataBlockPtr->MeOperationMode));
  DEBUG ((DEBUG_INFO, " ErrorCode          = 0x%02x \n", HealthDataBlockPtr->ErrorCode));
  DEBUG ((DEBUG_INFO, " SpiProtectionMode  = 0x%02x \n", HealthDataBlockPtr->SpiProtectionMode));
  DEBUG ((DEBUG_INFO, " FpfSocConfigLock   = 0x%02x \n", HealthDataBlockPtr->FpfSocConfigLock));
  DEBUG ((DEBUG_INFO, " FwUpdateInprogress = 0x%02x \n", HealthDataBlockPtr->FwUpdateInprogress));
  DEBUG ((DEBUG_INFO, "==================================\n"));

  *InformationBlock  = HealthDataBlockPtr;
  *InformationBlockSize = sizeof (EFI_AIP_CSME_HEALTH_STATE);

  return EFI_SUCCESS;
}

/**
  This function sets the CSME health data.

  @param[in] *This                      A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[in] EFI_GUID *InformationType  A pointer to gAdapterInfoCsmeGuid that defines the contents of InformationBlock.
                                        The caller must use the InformationType to specify the infromation it needs to retrieve
                                        from this service and to determine how to parse the InformationBlock.
                                        The driver should not attempt to free InformationType.
  @param[in] VOID **InformationBlock    A pointer to the number of GUIDs present in InfoTypesBuffer.
  @retval EFI_WRITE_PROTECTED
  @retval EFI_INVALID_PARAMETER         This is NULL.
  @retval EFI_INVALID_PARAMETER         InformationBlock is NULL.
  @retval EFI_WRITE_PROTECTED           The InformationType cannot be modified using EFI_ADAPTER_INFO_SET_INFO().
**/
EFI_STATUS
EFIAPI
CsmeHealthDataAipSetInfo (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  IN  EFI_GUID                          *InformationType,
  IN  VOID                              *InformationBlock,
  IN  UINTN                             InformationBlockSize
  )
{
  if ((This == NULL) || (InformationBlock == NULL)) {
    return EFI_INVALID_PARAMETER;
  }
  if (!CompareGuid (InformationType, &gAdapterInfoCsmeGuid)) {
    return EFI_UNSUPPORTED;
  }
  return EFI_WRITE_PROTECTED;
}

/**
  This function gets a list of Information types for this instance of the protocol.

  @param[in] *This                   A pointer to the EFI_ADAPTER_INFORMATION_PROTOCOL instance.
  @param[out] **InfoTypesBuffer      A pointer to the array of InformationType GUID is supported by This.
                                     This buffer is allocated by this service, and it is the responsibility
                                     of the caller to free it after using it.
  @param[out] *InfoTypesBufferCount  A pointer to the number of GUIDs present in InfoTypesBuffer.
  @retval EFI_INVALID_PARAMETER
  @retval EFI_OUT_OF_RESOURCES
  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
CsmeHealthDataGetSupportedTypes (
  IN  EFI_ADAPTER_INFORMATION_PROTOCOL  *This,
  OUT EFI_GUID                          **InfoTypesBuffer,
  OUT UINTN                             *InfoTypesBufferCount
  )
{
  if ((This == NULL) || (InfoTypesBuffer == NULL) || (InfoTypesBufferCount == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  *InfoTypesBuffer = AllocateCopyPool (sizeof(gAdapterInfoCsmeGuid), &gAdapterInfoCsmeGuid);

  if (*InfoTypesBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *InfoTypesBufferCount = 1;

  return EFI_SUCCESS;
}

/**
  This function installs EFI_ADAPTER_INFORMATION_PROTOCOL for CSME Health Data.
  The data is consumed by Telemetry data collector.

  @param[in] ImageHandle    The firmware allocated handle for the EFI image.
  @param[in] SystemTable    A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The CSME Health data is successfully published in AIP protocol.
  @retval EFI_OUT_OF_RESOURCES  Out of resources to allocate EFI_ADAPTER_INFORMATION_PROTOCOL.
**/
EFI_STATUS
EFIAPI
CsmeHealthDataDxeEntryPoint (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS                       Status;
  CSME_HEALTH_DEV                  *CsmeHealthPrivate;
  VENDOR_DEVICE_PATH               VendorDeviceNode;
  EFI_DEVICE_PATH_PROTOCOL         *DevicePath;

  Status                       = EFI_SUCCESS;
  DevicePath                   = NULL;

  DEBUG ((DEBUG_INFO, "InitializeCsmeHealthData Begins\n"));
  CsmeHealthPrivate = (CSME_HEALTH_DEV *) AllocateZeroPool (sizeof (CSME_HEALTH_DEV));
  if (CsmeHealthPrivate == NULL) {
    DEBUG ((EFI_D_ERROR, "EFI_OUT_OF_RESOURCES to Allocate EFI_ADAPTER_INFORMATION_PROTOCOL.\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  CsmeHealthPrivate->Handle    = NULL;

  CsmeHealthPrivate->AdapterInfo.GetInformation    = CsmeHealthDataAipGetInfo;
  CsmeHealthPrivate->AdapterInfo.SetInformation    = CsmeHealthDataAipSetInfo;
  CsmeHealthPrivate->AdapterInfo.GetSupportedTypes = CsmeHealthDataGetSupportedTypes;

  ZeroMem (&VendorDeviceNode, sizeof (VENDOR_DEVICE_PATH));
  VendorDeviceNode.Header.Type    = HARDWARE_DEVICE_PATH;
  VendorDeviceNode.Header.SubType = HW_VENDOR_DP;
  CopyGuid (&VendorDeviceNode.Guid, &gAdapterInfoCsmeGuid);
  SetDevicePathNodeLength (&VendorDeviceNode.Header, sizeof (VENDOR_DEVICE_PATH));
  DevicePath = AppendDevicePathNode (NULL, (EFI_DEVICE_PATH_PROTOCOL*)&VendorDeviceNode);
  if (DevicePath == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto ERROR;
  }

  CsmeHealthPrivate->CsmeDevicePath = DevicePath;

  Status = gBS->InstallMultipleProtocolInterfaces (
                  &CsmeHealthPrivate->Handle,
                  &gEfiAdapterInformationProtocolGuid,
                  &CsmeHealthPrivate->AdapterInfo,
                  &gEfiDevicePathProtocolGuid,
                  CsmeHealthPrivate->CsmeDevicePath,
                  NULL
                  );

  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "InitializeCsmeHealthData: faild to install AIP protocol, %r.\n", Status));
    goto ERROR;
  }

  DEBUG ((DEBUG_INFO, "InitializeCsmeHealthData Ends\n"));

  return Status;

ERROR:
  //
  // On error, clean the Aip service context data, and free the memory allocated.
  //
  FreePool (CsmeHealthPrivate);
  if (DevicePath != NULL) {
    FreePool (DevicePath);
  }

  return Status;

}
