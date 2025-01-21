/** @file
  Library API definitions for TbtNvmRetimerUpdateLib

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

#ifndef __TBT_NVM_RETIMER_UPDATE_LIB_H__
#define __TBT_NVM_RETIMER_UPDATE_LIB_H__

#include <TbtNvmRetimer.h>
#include <Protocol/FirmwareManagement.h>
#include <Protocol/UsbCRetimerProtocol.h>

typedef VOID   *RETIMER_DEV_INSTANCE;
typedef VOID   *HR_DEV_INSTANCE;
typedef VOID   *DISCRETE_TBT_DEV_INSTANCE;

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
  );

/**
  Create and initialize the required data structure to access a Retimer device.
  Besides, set up a communication with it if needed.

  @param[in]   FirmwareType       The type of update firmware.
  @param[in]   PcieRpType         PCIE root port type.
  @param[in]   PcieRootPort       The parent pcie root port of discrete TBT device.
  @param[in]   DevAddress         The address info of target Retimer device.
  @param[out]  RetimerDevice      Pointer to a Retimer device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             RetimerDevice is created successfully and is ready to accept commands.
  @retval Others                  An error occurred attempting to access the Retimer device.

**/
EFI_STATUS
EFIAPI
CreateRetimerDevInstance (
  IN   UINT8                                          FirmwareType,
  IN   UINT8                                          PcieRpType,
  IN   UINT8                                          PcieRootPort,
  IN   RETIMER_DEV_ADDRESS                            *DevAddress,
  OUT  RETIMER_DEV_INSTANCE                           *RetimerDevice
  );

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
  );

/**
  Create and initialize the required data structure to access a Thunderbolt device.
  Besides, set up a communication with it if needed.

  @param[in]   FirmwareType       The type of update firmware.
  @param[in]   PcieRpType         PCIE root port type.
  @param[in]   PcieRootPort       The parent pcie root port of discrete TBT device.
  @param[out]  DiscreteTbtDevice  Pointer to a Tbt device instance.

  @retval EFI_SUCCESS             DiscreteTbtDevice is created successfully and is ready to accept commands.
  @retval Others                  An error occurred attempting to access the TBT device.

**/
EFI_STATUS
EFIAPI
CreateTBTDevInstance (
  IN   UINT8                                          FirmwareType,
  IN   UINT8                                          PcieRpType,
  IN   UINT8                                          PcieRootPort,
  OUT  DISCRETE_TBT_DEV_INSTANCE                      *DiscreteTbtDevice
  );

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
  IN  RETIMER_DEV_INSTANCE                            RetimerDevice
  );

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
  IN  DISCRETE_TBT_DEV_INSTANCE                       DiscreteTbtDevice
  );

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
  IN   RETIMER_DEV_INSTANCE                           RetimerDevice,
  OUT  UINT32                                         *Version
  );

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
  IN   UINT8                                          FirmwareType,
  IN   UINT8                                          PcieRpType,
  IN   UINT8                                          PcieRootPort,
  IN   RETIMER_DEV_ADDRESS                            *DevAddress,
  IN   UINT32                                         RetimerVersion
  );

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
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,          OPTIONAL
  IN  UINTN                                          StartPercentage,
  IN  UINTN                                          EndPercentage
  );

/**
  Initialize Retimer HW on given RetimerDevice.
  Note: Caller is supposed to invoke CreateRetimerDevInstance() first to ensure the data structure
        of RetimerDevice is created properly and the communication to the target device is settled down.
        DestroyRetimerDevInstance() should be invoked after the caller is done with all Retimer accesses.

  @param[in]  RetimerDevice       Retimer device instance corresponding to DevAddress.
  @retval EFI_SUCCESS             Retimer Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the Retimer firmware
**/
EFI_STATUS
EFIAPI
InitRetimerHW (
  IN  RETIMER_DEV_INSTANCE                           RetimerDevice
  );


/**
  Terminate Retimer HW on given RetimerDevice.

  @param[in]  RetimerDevice       Retimer device instance corresponding to DevAddress.

  @retval EFI_SUCCESS             Terminate retimer successfully.
  @retval Others                  An error occurred attempting to access the Retimer firmware
**/
EFI_STATUS
EFIAPI
TerminateRetimerHW (
  IN  RETIMER_DEV_INSTANCE          RetimerDevice
  );

/**
  Update Discrete TBT NVM Firmware on given DiscreteTbtDevice.

  Note: Caller is supposed to invoke CreateTBTDevInstance() first to ensure the data structure
        of DiscreteTbtDevice is created properly and the communication to the target device is settled down.
        DestroyTbtDevInstance() should be invoked after the caller is done with all TBT accesses.

  @param[in]  DiscreteTbtDevice   Discrete TBT device instance corresponding to DISCRETE_TBT_ITEM.
  @param[in]  DiscreteTbtImage    Pointer to an image buffer contains discrete TBT FW to be updated with.
  @param[in]  ImageSize           The size of DiscreteTbtImage memory buffer.
  @param[in]  Progress            A function used to report the progress of updating the firmware
                                  device with the new firmware image.
  @param[in]  StartPercentage     The start completion percentage value that may be used to report progress
                                  during the flash write operation. The value is between 1 and 100.
  @param[in]  EndPercentage       The end completion percentage value that may be used to report progress
                                  during the flash write operation.

  @retval EFI_SUCCESS             TBT Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the TBT firmware

**/
EFI_STATUS
EFIAPI
UpdateDiscreteTbtNvmFirmware (
  IN  DISCRETE_TBT_DEV_INSTANCE                      DiscreteTbtDevice,
  IN  UINT8                                          *DiscreteTbtImage,
  IN  UINTN                                          ImageSize,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,          OPTIONAL
  IN  UINTN                                          StartPercentage,
  IN  UINTN                                          EndPercentage
  );

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
  );

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
  );

#endif
