/** @file
  TbtNvmRetimerUpdateLib instance to support TBT Retimer firmware update

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
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TbtNvmRetimerUpdateLib.h>
#include <Library/TbtNvmDrvRetimerThruHr.h>
#include <Library/TbtNvmDrvYflRouter.h>
#include <Library/TbtNvmRetimerDrvHelpers.h>
#include <Protocol/TbtNvmDrvDef.h>
#include <UsbCRetimerSetup.h>
#include <Library/UsbcCapsuleDebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <UsbCCapsuleDebug/UsbCCapsuleDebugProtocol.h>
#include <UsbCCapsuleDebug/UsbCCapsuleLogEvents.h>

UPDATE_TARGET_TYPE gUpdateTargetType;    // TARGET_TBT_HOST or TARGET_RETIMER

/**
  Construct the Host Router instances for sending OFFLINE_MODE.
  Same TBT port of the same DMA only needs to send OFFLINE_MODE one time.

  @param[in]  RetimerDeviceInstances       A pointer to the Retimer device instances base on RETIMER_DEV_INSTANCE.
  @param[in]  RetimerDeviceInstancesCount  A pointer to count the number of Retimer device instances.
  @param[out] HrDeviceInstances            A pointer to the Host Router device instances base on HR_DEV_INSTANCE.
  @param[out] HrDeviceInstancesCount       A pointer to count the number of Host Router device instances.

  @retval EFI_SUCCESS                      Host Router Instance is created successfully and is ready to accept commands.
  @retval Others                           An error occurred attempting to access the Retimer device.

**/
EFI_STATUS
EFIAPI
CreateHrDevInstance (
  IN   RETIMER_DEV_INSTANCE  *RetimerDeviceInstances,
  IN   UINT32                *RetimerDeviceInstancesCount,
  OUT  HR_DEV_INSTANCE       *HrDeviceInstances,
  OUT  UINT32                *HrDeviceInstancesCount
  )
{
  UINT32           RetimerIndex;
  UINT32           HrIndex;
  RETIMER_THRU_HR  *RetimerHr;
  RETIMER_THRU_HR  *TargetHr;
  BOOLEAN          NeedUpdated;

  if (*RetimerDeviceInstancesCount == 0) {
    DEBUG ((DEBUG_ERROR, "RetimerDeviceInstancesCount is zero.\n"));
    return EFI_UNSUPPORTED;
  }

  if (*HrDeviceInstancesCount != 0) {
    DEBUG ((DEBUG_ERROR, "HrDeviceInstances already exists.\n"));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "%a - Start\n", __FUNCTION__));

  for (RetimerIndex = 0; RetimerIndex < *RetimerDeviceInstancesCount; RetimerIndex++) {
    RetimerHr = (RETIMER_THRU_HR *) (((TBT_RETIMER *) RetimerDeviceInstances[RetimerIndex])->Impl);
    NeedUpdated = TRUE;

    for (HrIndex = 0; HrIndex < *HrDeviceInstancesCount; HrIndex++) {
      TargetHr = (RETIMER_THRU_HR *) (HrDeviceInstances[HrIndex]);

      //
      // Check whether there is the same PORT and the same DMA as RetimerHr in HrDeviceInstances.
      // If not, NeedUpdated will be TRUE and update the RetimerHr into HrDeviceInstances.
      //
      if ((RetimerHr->TbtPort == TargetHr->TbtPort) &&
          (RetimerHr->Hr->TbtDmaPcieBdf.Bus == TargetHr->Hr->TbtDmaPcieBdf.Bus) &&
          (RetimerHr->Hr->TbtDmaPcieBdf.Device == TargetHr->Hr->TbtDmaPcieBdf.Device) &&
          (RetimerHr->Hr->TbtDmaPcieBdf.Function == TargetHr->Hr->TbtDmaPcieBdf.Function)) {
        NeedUpdated = FALSE;
        break;
      }
    }

    if (NeedUpdated) {
      HrDeviceInstances[*HrDeviceInstancesCount] = ((TBT_RETIMER *) RetimerDeviceInstances[RetimerIndex])->Impl;
      ++(*HrDeviceInstancesCount);
    }
  }
  return EFI_SUCCESS;
}

/**
  Create and initialize the required data structure to access a Retimer device.
  Besides, set up a communication with it if needed.

  @param[in]   FirmwareType       The type of update firmware.
  @param[in]   PcieRpType         PCIE root port type.
  @param[in]   PchPcieRootPort    The parent pcie root port of discrete TBT device.
  @param[in]   DevAddress         The address info of target Retimer device.
  @param[out]  RetimerDevice      Pointer to a Retimer device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             RetimerDevice is created successfully and is ready to accept commands.
  @retval Others                  An error occurred attempting to access the Retimer device.

**/
EFI_STATUS
EFIAPI
CreateRetimerDevInstance (
  IN   UINT8                 FirmwareType,
  IN   UINT8                 PcieRpType,
  IN   UINT8                 PcieRootPort,
  IN   RETIMER_DEV_ADDRESS   *DevAddress,
  OUT  RETIMER_DEV_INSTANCE  *RetimerDevice
  )
{
  EFI_STATUS      Status;
  TBT_RETIMER     *Device;
  PCIE_BDF        *TbtDmaPciLocation;
  TBT_PORT        TBT_TARGET_PORT;
  PCIE_RP_CONFIG  *PcieRpConfig;
  FORCE_PWR_HR    ForcePwrFunc;

  if ((DevAddress == NULL) || (RetimerDevice == NULL)) {
    ASSERT (DevAddress != NULL);
    ASSERT (RetimerDevice != NULL);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize device start
  //
  gUpdateTargetType = TARGET_RETIMER;
  TbtDmaPciLocation = TbtNvmDrvAllocateMem (sizeof (PCIE_BDF));
  if (TbtDmaPciLocation == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateRetimerDevInstance: Failed to allocate memory for PCIE_BDF\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  PcieRpConfig = TbtNvmDrvAllocateMem (sizeof (PCIE_RP_CONFIG));
  if (PcieRpConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateRetimerDevInstance: Failed to allocate memory for PCIE_RP_CONFIG\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  Device = NULL;
  Status = EFI_SUCCESS;

  if (DevAddress->Port == 0) {
    TBT_TARGET_PORT = PORT_A;
  } else if (DevAddress->Port == 1) {
    TBT_TARGET_PORT = PORT_B;
  } else {
    DEBUG ((DEBUG_ERROR, "Error DevAddress->Port value.\n"));
    return EFI_UNSUPPORTED;
  }

  TbtDmaPciLocation->Bus      = DevAddress->Bus;
  TbtDmaPciLocation->Device   = DevAddress->Device;
  TbtDmaPciLocation->Function = DevAddress->Function;
  PcieRpConfig->PcieRpType    = PcieRpType;
  PcieRpConfig->PcieRootPort  = PcieRootPort;

  if (FirmwareType == INTEGRATED_TBT_RETIMER) {
    ForcePwrFunc = TbtNvmDrvYflForcePwrFunc;
  } else {
    ForcePwrFunc = NULL;
  }

  Device = TbtNvmDrvRetimerThruHrCtor (
             FirmwareType,
             TbtDmaPciLocation,
             TBT_TARGET_PORT,
             (UINT32) DevAddress->CascadedRetimerIndex,
             PcieRpConfig,
             ForcePwrFunc
             );

  if (Device == NULL) {
    DEBUG ((DEBUG_ERROR, "Failed to create a device context for the retimer.\n"));
    Status = EFI_DEVICE_ERROR;
  }

  TbtNvmDrvDeAllocateMem (TbtDmaPciLocation);
  TbtNvmDrvDeAllocateMem (PcieRpConfig);

  *RetimerDevice = Device;
  return Status;
}

/**
  Send OFFLINE_MODE to the specified HR and specified port

  @param[in]  HrDevice  Pointer to to the current implementation of Host Router device - RETIMER_THRU_HR.
  @param[in]  Data      0 - OFFLINE_MODE_ENTRY
                        1 - OFFLINE_MODE_EXIT

  @retval EFI_SUCCESS   indicate operation success
  @retval Others        Fail to send RETIMER_OFFLINE_MODE to the specified port on the HR.

**/
EFI_STATUS
EFIAPI
TbtSendOfflineMode (
  IN  HR_DEV_INSTANCE  *HrDevice,
  IN  UINT32           Data
  )
{
  EFI_STATUS       Status;
  RETIMER_THRU_HR  *HrPtr;

  HrPtr  = (RETIMER_THRU_HR *) HrDevice;
  Status = SendOfflineMode (HrPtr, Data);

  return Status;
}

/**
  Create and initialize the required data structure to access a TBT device.
  Besides, set up a communication with it if needed.

  @param[in]   FirmwareType       The type of update firmware.
  @param[in]   PcieRpType         PCIE root port type.
  @param[in]   PcieRootPort       The parent pcie root port of discrete TBT device.
  @param[out]  DiscreteTbtDevice  Pointer to a Tbt device instance.

  @retval EFI_SUCCESS             DiscreteTbtDevice is created successfully and is ready to accept commands.
  @retval Others                  An error occurred attempting to access the Retimer device.

**/
EFI_STATUS
EFIAPI
CreateTBTDevInstance (
  IN   UINT8                      FirmwareType,
  IN   UINT8                      PcieRpType,
  IN   UINT8                      PcieRootPort,
  OUT  DISCRETE_TBT_DEV_INSTANCE  *DiscreteTbtDevice
  )
{
  TBT_HOST_ROUTER  *HrPtr;
  PCIE_RP_CONFIG   *PcieRpConfig;

  if (DiscreteTbtDevice == NULL) {
    ASSERT (DiscreteTbtDevice != NULL);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Initialize device start
  //
  gUpdateTargetType = TARGET_TBT_HOST;

  PcieRpConfig = TbtNvmDrvAllocateMem (sizeof (PCIE_RP_CONFIG));
  if (PcieRpConfig == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateTBTDevInstance: Failed to allocate memory for PCIE_RP_CONFIG\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  PcieRpConfig->PcieRpType   = PcieRpType;
  PcieRpConfig->PcieRootPort = PcieRootPort;

  HrPtr = TbtNvmDrvHrCtor (FirmwareType, PcieRpConfig, NULL, NULL);

  TbtNvmDrvDeAllocateMem (PcieRpConfig);
  if (HrPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "CreateTBTDevInstance: Failed to create DMA\n"));
    return EFI_DEVICE_ERROR;
  }

  *DiscreteTbtDevice = (VOID *) HrPtr;
  return EFI_SUCCESS;
}

/**
  Release the data structure corresponding to given RetimerDevice.
  Besides, close the communication with it if needed.

  @param[in]  RetimerDevice       Retimer device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             RetimerDevice is released successfully.
  @retval Others                  An error occurred attempting to access the Retimer device.

**/
EFI_STATUS
EFIAPI
DestroyRetimerDevInstance (
  IN  RETIMER_DEV_INSTANCE  RetimerDevice
  )
{
  TBT_RETIMER  *Device;

  Device = (TBT_RETIMER *) RetimerDevice;

  if (Device != NULL) {
    Device->Destroy (Device);
  }

  return EFI_SUCCESS;
}

/**
  Release the data structure corresponding to given DiscreteTbtDevice.
  Besides, close the communication with it if needed.

  @param[in]  DiscreteTbtDevice   Discrete TBT device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             DiscreteTbtDevice is released successfully.
  @retval Others                  An error occurred attempting to access the Tbt device.

**/
EFI_STATUS
EFIAPI
DestroyTbtDevInstance (
  IN  DISCRETE_TBT_DEV_INSTANCE  DiscreteTbtDevice
  )
{
  TBT_HOST_ROUTER  *Device;

  Device = (TBT_HOST_ROUTER *) DiscreteTbtDevice;

  if (Device != NULL) {
    Device->Dtor (Device);
  }

  return EFI_SUCCESS;
}

/**
  Read Retimer NVM FW version on given RetimerDevice.

  Note: Caller is supposed to invoke CreateRetimerDevInstance() first to ensure the data structure
        of RetimerDevice is created properly and the communication to the target device is settled down.
        DestroyRetimerDevInstance() should be invoked after the caller is done with all Retimer accesses.

  @param[in]   RetimerDevice      Retimer device instance corresponding to DevAddress.
  @param[out]  Version            Retimer NVM FW version.

  @retval EFI_SUCCESS             Retimer FW version is successfully get.
  @retval Others                  An error occurred attempting to access the Retimer firmware

**/
EFI_STATUS
EFIAPI
ReadRetimerNvmVersion (
  IN   RETIMER_DEV_INSTANCE  RetimerDevice,
  OUT  UINT32                *Version
  )
{
  EFI_STATUS   Status;
  TBT_RETIMER  *Device;
  UINT32       RetimerVersion;
  UINT8        PlatformVersion;
  UINT8        MajorVersion;
  UINT8        MinorVersion;
  UINT8        OfficialNvm;

  if (Version == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  RetimerVersion = 0;
  Device = (TBT_RETIMER *) RetimerDevice;

  Status = TbtDrvReadNvmVersion (Device, &RetimerVersion);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "TbtDrvReadNvmVersion error %r.\n", Status));
    return EFI_DEVICE_ERROR;
  }

  RetimerVersion = RetimerVersion & TBT_NVM_VERSION_MASK;
  PlatformVersion = (RetimerVersion & 0xFF);                            // Per platform per retimer.
  MajorVersion    = ((RetimerVersion >> 16) & 0xFF);                    // The major version of retimer NVM.
  MinorVersion    = ((RetimerVersion >> 8)  & 0x3F);                    // The minor version of retimer NVM.
  OfficialNvm     = ((RetimerVersion >> 15) & 0x1);                     // The official NVM indication, should be set in official NVMs.

  DEBUG ((DEBUG_INFO, "Current NVM version is rev%02x v%02x.%02x Official bit: %d\n", PlatformVersion, MajorVersion, MinorVersion, OfficialNvm));
  RetimerVersion = ((PlatformVersion << 16) | (MajorVersion << 8) | MinorVersion);

  *Version = RetimerVersion;
  return EFI_SUCCESS;
}

/**
  Update Retimer NVM Information.

  @param[in]   FirmwareType       The type of update firmware.
  @param[in]   PcieRpType         PCIE root port type.
  @param[in]   PchPcieRootPort    The parent pcie root port of discrete TBT device.
  @param[in]   DevAddress         The address info of target Retimer device.
  @param[in]   RetimerVersion     Retimer NVM FW version.

**/
VOID
UpdateRetimerNvmInformation (
  IN   UINT8                FirmwareType,
  IN   UINT8                PcieRpType,
  IN   UINT8                PcieRootPort,
  IN   RETIMER_DEV_ADDRESS  *DevAddress,
  IN   UINT32               RetimerVersion
  )
{
  EFI_STATUS              Status;
  UINT8                   UsbCRetimerNumber;
  UINT8                   Index;
  RETIMER_CONFIG          *RetimerConfigTable;
  USBC_RETIMER_SETUP      UsbCRetimerSetup;
  UINT32                  VarAttributes;
  UINTN                   VarSize;

  DEBUG ((DEBUG_INFO, "UpdateRetimerNvmInformation - RetimerVersion is %x .\n", RetimerVersion));

  UsbCRetimerNumber = PcdGet8 (PcdUsbCRetimerFlashNumber);
  RetimerConfigTable = (RETIMER_CONFIG *) PcdGetPtr (PcdUsbCRetimerConfigTable);

  VarSize = sizeof (USBC_RETIMER_SETUP);
  Status = gRT->GetVariable (
                  USBC_RETIMER_NAME,
                  &gUsbCRetimerSetupGuid,
                  &VarAttributes,
                  &VarSize,
                  &UsbCRetimerSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to get setup variable\n"));
  }
  for (Index = 0; Index < UsbCRetimerNumber; Index++) {
    //
    // Find the matching retimer configuration in RetimerConfigTable.
    //
    if ( (RetimerConfigTable[Index].FirmwareType != FirmwareType)
      || (RetimerConfigTable[Index].PcieRpType != PcieRpType)
      || (RetimerConfigTable[Index].PcieRootPort != PcieRootPort)
      || (RetimerConfigTable[Index].RetimerDevAddress.Bus != DevAddress->Bus)
      || (RetimerConfigTable[Index].RetimerDevAddress.Device != DevAddress->Device)
      || (RetimerConfigTable[Index].RetimerDevAddress.Function != DevAddress->Function)
      || (RetimerConfigTable[Index].RetimerDevAddress.Port != DevAddress->Port)
      || (RetimerConfigTable[Index].RetimerDevAddress.CascadedRetimerIndex  != DevAddress->CascadedRetimerIndex))
    {
      Status = EFI_NOT_FOUND;
      DEBUG ((DEBUG_INFO, "RetimerConfigTable config %x does not match.\n", Index));
      continue;
    }
    //
    // Update UsbC Retimer information
    //
    switch (Index) {
      case 0:
        UsbCRetimerSetup.UsbCRetimer0Version = RetimerVersion;
        break;
      case 1:
        UsbCRetimerSetup.UsbCRetimer1Version = RetimerVersion;
        break;
      case 2:
        UsbCRetimerSetup.UsbCRetimer2Version = RetimerVersion;
        break;
      case 3:
        UsbCRetimerSetup.UsbCRetimer3Version = RetimerVersion;
        break;
      default:
        DEBUG ((DEBUG_ERROR, "UsbCRetimerNumber is more than Index setting.\n"));
        break;
    }
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "RetimerConfigTable config %x is matched.\n", Index));
    break;
  }
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, \
      "UpdateRetimerNvmInformation - Fail on RetimerPayloadConfig header in capsule file does not match the RetimerConfigTable.\n"));
  }
  Status = gRT->SetVariable (
                  USBC_RETIMER_NAME,
                  &gUsbCRetimerSetupGuid,
                  VarAttributes,
                  sizeof (USBC_RETIMER_SETUP),
                  &UsbCRetimerSetup
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "UpdateRetimerNvmInformation - Fail to set variable in UsbCRetimerSetup.\n"));
  } else {
    DEBUG ((DEBUG_INFO, "UpdateRetimerNvmInformation - Success to set variable in UsbCRetimerSetup. \n"));
  }
  return;
}

/**
  Initialize Retimer HW on given RetimerDevice.

  Note: Caller is supposed to invoke CreateRetimerDevInstance() first to ensure the data structure
        of RetimerDevice is created properly and the communication to the target device is settled down.
        DestroyRetimerDevInstance() should be invoked after the caller is done with all Retimer accesses.

  @param[in]  RetimerDevice       Retimer device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             Initialize retimer successfully.
  @retval Others                  An error occurred attempting to access the Retimer firmware

**/
EFI_STATUS
EFIAPI
InitRetimerHW (
  IN  RETIMER_DEV_INSTANCE  RetimerDevice
  )
{
  EFI_STATUS   Status;
  TBT_RETIMER  *Device;
  TBT_STATUS   TbtStatus;

  ASSERT (RetimerDevice != NULL);

  if ((RetimerDevice == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Device = (TBT_RETIMER *) RetimerDevice;

  DEBUG ((DEBUG_INFO, "InitRetimerHW START\n"));
  TbtStatus = Device->InitHW (Device);

  if (TBT_STATUS_ERR (TbtStatus)) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "InitRetimerHW: Got an ERROR(%d)\n", TbtStatus));
  } else {
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "InitRetimerHW success\n"));
  }

  return Status;
}

/**
  Terminate Retimer HW on given RetimerDevice.

  @param[in]  RetimerDevice  Retimer device instance corresponding to DevAddress.

  @retval  EFI_SUCCESS       Terminate retimer successfully.
  @retval  Others            An error occurred attempting to access the Retimer firmware

**/
EFI_STATUS
EFIAPI
TerminateRetimerHW (
  IN  RETIMER_DEV_INSTANCE  RetimerDevice
  )
{
  EFI_STATUS   Status;
  TBT_RETIMER  *Device;
  TBT_STATUS   TbtStatus;

  ASSERT (RetimerDevice != NULL);

  if ((RetimerDevice == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Device = (TBT_RETIMER *) RetimerDevice;

  DEBUG ((DEBUG_INFO, "TerminateRetimerHW START\n"));
  TbtStatus = Device->TerminateHW(Device);

  if (TBT_STATUS_ERR (TbtStatus)) {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_ERROR, "TerminateRetimerHW: Got an ERROR(%d)\n", TbtStatus));
  } else {
    Status = EFI_SUCCESS;
    DEBUG ((DEBUG_INFO, "TerminateRetimerHW success\n"));
  }

  return Status;
}

/**
  Update Retimer NVM Firmware on given RetimerDevice.

  Note: Caller is supposed to invoke CreateRetimerDevInstance() first to ensure the data structure
        of RetimerDevice is created properly and the communication to the target device is settled down.
        DestroyRetimerDevInstance() should be invoked after the caller is done with all Retimer accesses.

  @param[in]  RetimerDevice       Retimer device instance corresponding to DevAddress.
  @param[in]  RetimerImage        Pointer to an image buffer contains Retimer FW to be updated with.
  @param[in]  ImageSize           The size of RetimerImage memory buffer.
  @param[in]  Progress            A function used to report the progress of updating the firmware
                                  device with the new firmware image.
  @param[in]  StartPercentage     The start completion percentage value that may be used to report progress
                                  during the flash write operation. The value is between 1 and 100.
  @param[in]  EndPercentage       The end completion percentage value that may be used to report progress
                                  during the flash write operation.

  @retval EFI_SUCCESS             Retimer Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the Retimer firmware

**/
EFI_STATUS
EFIAPI
UpdateRetimerNvmFirmware (
  IN  RETIMER_DEV_INSTANCE                           RetimerDevice,
  IN  UINT8                                          *RetimerImage,
  IN  UINTN                                          ImageSize,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN  UINTN                                          StartPercentage,
  IN  UINTN                                          EndPercentage
  )
{
  EFI_STATUS   Status;
  UINT32       *BufferPointer;
  TBT_RETIMER  *Device;
  UINT32       Offset;
  TBT_STATUS   TbtStatus;
  UINT32       RecoveryCount;
  UINT8        WriteLength;
  UINTN        DisplayLength;

  if ((RetimerDevice == NULL) || (RetimerImage == NULL) || (EndPercentage < StartPercentage)) {
    ASSERT (RetimerDevice != NULL);
    ASSERT (RetimerImage != NULL);
    ASSERT (EndPercentage > StartPercentage);
    return EFI_INVALID_PARAMETER;
  }

  DisplayLength = EndPercentage - StartPercentage;

  Device = (TBT_RETIMER *) RetimerDevice;

  BufferPointer = (UINT32 *) RetimerImage;
  ImageSize = (UINT32) ImageSize;
  ImageSize -= (BufferPointer[0] & TBT_NVM_VERSION_MASK);
  BufferPointer += ((BufferPointer[0] & TBT_NVM_VERSION_MASK) / 4); // Point to digital

  DEBUG ((DEBUG_INFO, "UpdateRetimerNvmFirmware START (0x%06x bytes to write):\n", ImageSize));
  ImageSize /= 4;
  for (RecoveryCount = 0; RecoveryCount < TBT_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE; RecoveryCount++) {
    // Write BOPS once at the start
    TbtStatus = TbtNvmDrvDeviceWrOffset (Device, 0x0);
    if (TBT_STATUS_ERR (TbtStatus)) {
      if (TBT_STATUS_FATAL_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "Got a FATAL error, exiting...\n"));
        Status = EFI_DEVICE_ERROR;
        goto finish_set_image;
      }
      continue;  // retry image write from the start
    }
    DEBUG ((DEBUG_INFO, "The total data to write is 0x%6x\n", ImageSize));

    for (Offset = 0; Offset < ImageSize; Offset += TBT_NVM_MAX_DWS_TO_WRITE) {
      if ((Progress != NULL) & (DisplayLength > 0) && ((Offset & 0xFF) == 0)) {
        //
        // Display current progress
        //
        Progress (StartPercentage + ((Offset * (DisplayLength)) / ImageSize));
      }
      if ((Offset % 3200) == 0) {
        DEBUG ((DEBUG_INFO, "Written so far : 0x%05x bytes\n", Offset * 4));
      }
      WriteLength = (ImageSize - Offset) >= TBT_NVM_MAX_DWS_TO_WRITE ? TBT_NVM_MAX_DWS_TO_WRITE :
        (UINT8)(ImageSize - Offset);

      if ((Offset % 64) == 0) {
        DEBUG ((DEBUG_INFO, "Writing at Offset - 0x%x length of 0x%0x, first reg 0x%04x\n",
          Offset, WriteLength, (BufferPointer + Offset) [0]));
      }

      TbtStatus = TbtNvmDrvDeviceWrBlk (Device, BufferPointer + Offset, WriteLength);
      if (TBT_STATUS_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "Got an error at offset 0x%08x\n", Offset));
        if (TBT_STATUS_FATAL_ERR (TbtStatus)) {
          DEBUG ((DEBUG_ERROR, "Got a FATAL error, exiting...\n"));
          Status = EFI_DEVICE_ERROR;
          goto finish_set_image;
        }
        break;  // retry image write from the start
                // TODO more fine tuned recovery is possible
      }
    }

    if (!TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_INFO, "Image write finished.\n"));
      Status = EFI_SUCCESS;
      break;
    } else {
      DEBUG ((DEBUG_ERROR, "Got an error while writing the image. As a recovery, starting again\n"));
    }
  }
  if (RecoveryCount >= TBT_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE) {
    DEBUG ((DEBUG_ERROR, "Image write wasn't successful due to a device error\n"));
    Status = EFI_DEVICE_ERROR;
    goto finish_set_image;
  }

  TbtStatus = TbtNvmDrvDeviceExecAuth (Device);
  if (TBT_STATUS_ERR (TbtStatus)) {
    Status = EFI_DEVICE_ERROR;
    goto finish_set_image;
  }

  DEBUG ((DEBUG_INFO, "UpdateRetimerNvmFirmware: Retimer firmware update and authentication success\n"));

  Status = EFI_SUCCESS;

finish_set_image:

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  return Status;
}

/**
  Update Discrete TBT NVM Firmware on given DiscreteTbtDevice.

  Note: Caller is supposed to invoke CreateTBTDevInstance() first to ensure the data structure
        of DiscreteTbtDevice is created properly and the communication to the target device is settled down.
        DestroyRetimerDevInstance() should be invoked after the caller is done with all Retimer accesses.

  @param[in]  DiscreteTbtDevice   Discrete TBT device instance corresponding to DISCRETE_TBT_ITEM.
  @param[in]  DiscreteTbtImage    Pointer to an image buffer contains discrete TBT FW to be updated with.
  @param[in]  ImageSize           The size of DiscreteTbtImage memory buffer.
  @param[in]  Progress            A function used to report the progress of updating the firmware
                                  device with the new firmware image.
  @param[in]  StartPercentage     The start completion percentage value that may be used to report progress
                                  during the flash write operation. The value is between 1 and 100.
  @param[in]  EndPercentage       The end completion percentage value that may be used to report progress
                                  during the flash write operation.

  @retval EFI_SUCCESS             Retimer Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the Retimer firmware

**/
EFI_STATUS
EFIAPI
UpdateDiscreteTbtNvmFirmware (
  IN  DISCRETE_TBT_DEV_INSTANCE                      DiscreteTbtDevice,
  IN  UINT8                                          *DiscreteTbtImage,
  IN  UINTN                                          ImageSize,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN  UINTN                                          StartPercentage,
  IN  UINTN                                          EndPercentage
  )
{
  EFI_STATUS       Status;
  UINT32           *BufferPointer;
  TBT_HOST_ROUTER  *Device;
  UINT32           Offset;
  TBT_STATUS       TbtStatus;
  UINT32           RecoveryCount;
  UINT8            WriteLength;
  UINTN            DisplayLength;

  if ((DiscreteTbtDevice == NULL) || (DiscreteTbtImage == NULL) || (EndPercentage < StartPercentage)) {
    ASSERT (DiscreteTbtDevice != NULL);
    ASSERT (DiscreteTbtImage != NULL);
    ASSERT (EndPercentage > StartPercentage);
    return EFI_INVALID_PARAMETER;
  }

  DisplayLength = EndPercentage - StartPercentage;

  Device = (TBT_HOST_ROUTER *) DiscreteTbtDevice;

  BufferPointer = (UINT32 *) DiscreteTbtImage;
  ImageSize = (UINT32) ImageSize;
  ImageSize -= (BufferPointer[0] & TBT_NVM_VERSION_MASK);
  BufferPointer += ((BufferPointer[0] & TBT_NVM_VERSION_MASK) / 4); // Point to digital

  DEBUG ((DEBUG_INFO, "UpdateDiscreteTbtNvmFirmware START (0x%06x bytes to write):\n", ImageSize));
  ImageSize /= 4;
  for (RecoveryCount = 0; RecoveryCount < TBT_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE; RecoveryCount++) {
    // Write BOPS once at the start
    TbtStatus = TbtNvmDrvDeviceWrOffset (Device, 0x0);
    if (TBT_STATUS_ERR (TbtStatus)) {
      if (TBT_STATUS_FATAL_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "Got a FATAL error, exiting...\n"));
        Status = EFI_DEVICE_ERROR;
        goto finish_set_image;
      }
      continue;  // retry image write from the start
    }
    DEBUG ((DEBUG_INFO, "The total data to write is 0x%6x\n", ImageSize));

    for (Offset = 0; Offset < ImageSize; Offset += TBT_NVM_MAX_DWS_TO_WRITE) {
      if ((Progress != NULL) & (DisplayLength > 0) && ((Offset & 0xFF) == 0)) {
        //
        // Display current progress
        //
        Progress (StartPercentage + ((Offset * (DisplayLength)) / ImageSize));
      }
      if ((Offset % 3200) == 0) {
        DEBUG ((DEBUG_INFO, "Written so far : 0x%05x bytes\n", Offset * 4));
      }
      WriteLength = (ImageSize - Offset) >= TBT_NVM_MAX_DWS_TO_WRITE ? TBT_NVM_MAX_DWS_TO_WRITE :
        (UINT8)(ImageSize - Offset);

      if ((Offset % 64) == 0) {
        DEBUG ((DEBUG_INFO, "Writing at Offset - 0x%x length of 0x%0x, first reg 0x%04x\n",
          Offset, WriteLength, (BufferPointer + Offset) [0]));
      }

      TbtStatus = TbtNvmDrvDeviceWrBlk (Device, BufferPointer + Offset, WriteLength);
      if (TBT_STATUS_ERR (TbtStatus)) {
        DEBUG ((DEBUG_ERROR, "Got an error at offset 0x%08x\n", Offset));
        if (TBT_STATUS_FATAL_ERR (TbtStatus)) {
          DEBUG ((DEBUG_ERROR, "Got a FATAL error, exiting...\n"));
          Status = EFI_DEVICE_ERROR;
          goto finish_set_image;
        }
        break;  // retry image write from the start
                // TODO more fine tuned recovery is possible
      }
    }

    if (!TBT_STATUS_ERR (TbtStatus)) {
      DEBUG ((DEBUG_INFO, "Image write finished.\n"));
      Status = EFI_SUCCESS;
      break;
    }
    else {
      DEBUG ((DEBUG_ERROR, "Got an error while writing the image. As a recovery, starting again\n"));
    }
  }
  if (RecoveryCount >= TBT_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE) {
    DEBUG ((DEBUG_ERROR, "Image write wasn't successful due to a device error\n"));
    Status = EFI_DEVICE_ERROR;
    goto finish_set_image;
  }

  TbtStatus = TbtNvmDrvDeviceExecAuth (Device);
  if (TBT_STATUS_ERR (TbtStatus)) {
    Status = EFI_DEVICE_ERROR;
    goto finish_set_image;
  }

  DEBUG ((DEBUG_INFO, "UpdateDiscreteTbtNvmFirmware: Discrete TBT NVM firmware update and authentication success\n"));

  Status = EFI_SUCCESS;

finish_set_image:

  if (Progress != NULL) {
    Progress (EndPercentage);
  }

  return Status;
}

/**
  Change Tbt Retimer PD controller FW Update Mode.

  @param[in]  This                The UsbC RETIMER PROTOCOL Instance.
  @param[in]  PdControllerMode    Mode of PD Controller to SET.
  @param[in]  PdNumber            Number of PD Controllers

  @retval  EFI_SUCCESS            Successfully Mode is Changed to FW Update Mode.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_ALREADY_STARTED    This driver has been started. This is applicable for PD Fw Update Mode Entry.
  @retval  EFI_NOT_STARTED        This driver has not been started. This is applicable for PD Fw Update Mode Exit.
  @retval  EFI_TIMEOUT            Mode Change Command timeout Happen.

**/
EFI_STATUS
TbtPdRetimerFwUpdateModeChange (
  IN  USBC_RETIMER_PROTOCOL  *This,
  IN  UINT8                  PdControllerMode,
  IN  UINT8                  PdNumber
  )
{
  EFI_STATUS  Status;
  UINT8       DeviceMode;
  UINT8       EcPdTempBuffer;
  CHAR16      *String = NULL;

  String = ((PdControllerMode == RetimerFwUpdateEnableMode) ? L"Enable" : L"Disable");

  //
  // Initialize DeviceMode with Invalid Data before getting PD Controller Mode
  //
  DeviceMode     = INVALID_DEVICE_MODE;
  EcPdTempBuffer = 0;

  //
  // Check the PD Controller Mode before setting PdControllerMode
  //
  Status = This->GetPdControllerMode (&DeviceMode, &EcPdTempBuffer, PdNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller GET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_FAIL, (UINT32) Status, 0);
    return Status;
  }

  if (PdControllerMode == RetimerFwUpdateEnableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_FIRST_GET_MODE_EC_STATUS, (UINT32) EcPdTempBuffer, 0);
  }

  if (PdControllerMode == RetimerFwUpdateDisableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_FIRST_GET_MODE_EC_STATUS, (UINT32) EcPdTempBuffer, 0);
  }

  //
  // Check if PD controller is in or not in FW Update mode already.
  //
  if (DeviceMode == PdControllerMode) {
    if (PdControllerMode == RetimerFwUpdateEnableMode) {
      DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller is already in FW Update Mode\n", String));
      CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ALREADY_IN_UPDATE_MODE, 0, 0);
      Status = EFI_ALREADY_STARTED;
    }

    if (PdControllerMode == RetimerFwUpdateDisableMode) {
      DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller is already exited FW Update Mode.\n", String));
      CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ALREADY_IN_UPDATE_MODE, 0, 0);
      Status = EFI_NOT_STARTED;
    }

    return Status;
  }

  //
  // Send command to EC instruct to PD Controller to either enter or exit FW Update Mode
  //
  Status = This->SetPdControllerMode (PdControllerMode);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller SET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_FAIL, (UINT32) Status, 0);
    return Status;
  }

  //
  // Initialize DeviceMode with Invalid Data before getting PD Controller Mode
  //
  DeviceMode     = INVALID_DEVICE_MODE;
  EcPdTempBuffer = 0;

  Status = This->GetPdControllerMode (&DeviceMode, &EcPdTempBuffer, PdNumber);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller GET MODE Not Responding. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_FAIL, (UINT32) Status, 0);
    return Status;
  }

  if (PdControllerMode == RetimerFwUpdateEnableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SECOND_GET_MODE_EC_STATUS, (UINT32) EcPdTempBuffer, 0);
  }

  if (PdControllerMode == RetimerFwUpdateDisableMode) {
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_SECOND_GET_MODE_EC_STATUS, (UINT32) EcPdTempBuffer, 0);
  }

  //
  // Check the PD Controller Mode after setting PdControllerMode
  //
  if (DeviceMode != PdControllerMode) {
    Status = EFI_TIMEOUT;
    DEBUG ((DEBUG_ERROR, "\nTbtPdRetimerFwUpdateMode.%S: PD Controller SET MODE is not Completed. Returning with Status = %r\n", String, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_SET_MODE_NOT_COMPLETE, (UINT32) Status, 0);
    return Status;
  }

  if (PdControllerMode == RetimerFwUpdateEnableMode) {
    //
    // Now PD Controller Successfully Enter Into FW Update Mode.
    //
    DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Drive: "));
    DEBUG ((DEBUG_INFO, "PD Controller Enter Into FW Update Mode with Status = %r\n", Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_ENTER_FW_UPDATE_MODE, 0, 0);
  }

  if (PdControllerMode == RetimerFwUpdateDisableMode) {
    //
    // Now PD Controller Successfully EXIT From FW Update Mode.
    //
    DEBUG ((DEBUG_INFO, "\nTbtPdRetimerFwUpdateMode.Restore: "));
    DEBUG ((DEBUG_INFO, "PD Controller Exit from FW Update Mode with Status = %r\n", Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_ENTER_FW_UPDATE_MODE, 0, 0);
  }

  return EFI_SUCCESS;
}

/**
  Drive UsbC Retimer Controller into FW Update Mode.

  @param[in]  This                The UsbC RETIMER PROTOCOL Instance.
  @param[in]  RetimerGuid         GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Mode is Changed to FW Update Mode.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_ALREADY_STARTED    This driver has been started.
  @retval  EFI_TIMEOUT            DriveToFwUpdateMode Command timeout Happen.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.

**/
EFI_STATUS
EFIAPI
DriveToFwUpdateMode (
  IN  USBC_RETIMER_PROTOCOL  *This,
  IN  EFI_GUID               RetimerGuid ///< GUID from ESRT ACPI Table
  )
{
  EFI_STATUS                        Status;
  USBC_RETIMER_PROTOCOL_CAPABILITY  RetimerProtocolCapability;
  UINT32                            VarAttributes;
  UINTN                             VarSize;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "\n%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "  RetimerGuid = %g\n", RetimerGuid));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_START, 0, 0);

  //
  // Check if Valid Guid is passed or Not.
  //
  if (!CompareGuid (&RetimerGuid, &gAllTbtRetimerDeviceGuid)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "\n%a : Invalid Retimer Guid Passed for UsbCRetimer. Returning with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Check if Usb Type-C Retimer is not supported
  //
  VarSize = sizeof (USBC_RETIMER_PROTOCOL_CAPABILITY);
  Status  = gRT->GetVariable (
                   USBC_RETIMER_PROTOCOL_CAPABILITY_NAME,
                   &gUsbCRetimerProtocolCapabilityGuid,
                   &VarAttributes,
                   &VarSize,
                   &RetimerProtocolCapability
                   );
  if (!EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "\n%a: Usb Type-C Retimer is not supported. Returning with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Change the PD Controller Mode
  //
  Status = TbtPdRetimerFwUpdateModeChange (This, RetimerFwUpdateEnableMode, FixedPcdGet8 (PcdMaxUsbCPdNumber));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\n%a : Failed to Enable Retimer FW Update Mode with Status = %r\n", __FUNCTION__, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_DRIVE_PD_CHANGE_MODE_FAIL, (UINT32) Status, 0);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "\n%a : End\n", __FUNCTION__));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_DRIVE_END, 0, 0);
  return Status;
}

/**
  Restore TBT PD Controller into original mode.

  @param[in]  This                The UsbC RETIMER PROTOCOL Instance.
  @param[in]  RetimerGuid         GUID from ESRT ACPI Table.

  @retval  EFI_SUCCESS            Successfully Mode is Restore.
  @retval  EFI_INVALID_PARAMETER  Invalid GUID from ESRT Table is Passed.
  @retval  EFI_UNSUPPORTED        This driver does not support.
  @retval  EFI_DEVICE_ERROR       This driver cannot be started due to device Error.
  @retval  EFI_NOT_STARTED        This driver has not been started.
  @retval  EFI_TIMEOUT            RestoreToOriginalMode Command timeout Happen.
  @retval  EFI_NOT_READY          Board Retimer PCD is not ready or Not Available.

**/
EFI_STATUS
EFIAPI
RestoreToOriginalMode (
  IN USBC_RETIMER_PROTOCOL  *This,
  IN EFI_GUID               RetimerGuid ///< GUID from ESRT ACPI Table
  )
{
  EFI_STATUS                        Status;
  USBC_RETIMER_PROTOCOL_CAPABILITY  RetimerProtocolCapability;
  UINT32                            VarAttributes;
  UINTN                             VarSize;

  Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "\n%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "  RetimerGuid = %g\n", RetimerGuid));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_START, 0, 0);

  //
  // Check if Valid Guid is passed or Not.
  //
  if (!CompareGuid (&RetimerGuid, &gAllTbtRetimerDeviceGuid)) {
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_ERROR, "\n%a : Invalid Retimer Guid Passed for UsbCRetimer. Returning with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Check if Usb Type-C Retimer is not supported
  //
  VarSize = sizeof (USBC_RETIMER_PROTOCOL_CAPABILITY);
  Status  = gRT->GetVariable (
                   USBC_RETIMER_PROTOCOL_CAPABILITY_NAME,
                   &gUsbCRetimerProtocolCapabilityGuid,
                   &VarAttributes,
                   &VarSize,
                   &RetimerProtocolCapability
                   );
  if (!EFI_ERROR (Status)) {
    Status = EFI_UNSUPPORTED;
    DEBUG ((DEBUG_ERROR, "\n%a: Usb Type-C Retimer is not supported. Returning with Status = %r\n", __FUNCTION__, Status));
    return Status;
  }

  //
  // Change the PD Controller Mode
  //
  Status = TbtPdRetimerFwUpdateModeChange (This, RetimerFwUpdateDisableMode, FixedPcdGet8 (PcdMaxUsbCPdNumber));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "\n%a : Failed to Exit Retimer FW Update Mode For PD Controller with Status = %r\n", __FUNCTION__, Status));
    CapsuleLogWrite (USBC_CAPSULE_DBG_ERROR, USBC_RETIMER_CAPSULE_EVT_CODE_RESTORE_PD_CHANGE_MODE_FAIL, (UINT32) Status, 0);
    return Status;
  }

  DEBUG ((DEBUG_INFO, "\n%a : End\n", __FUNCTION__));
  CapsuleLogWrite (USBC_CAPSULE_DBG_VERBOSE, USBC_RETIMER_CAPSULE_EVT_CODE_PD_RESTORE_END, 0, 0);
  return Status;
}