/** @file
  PCH Smbus Protocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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
#ifndef _SMBUS_SMM_H
#define _SMBUS_SMM_H

#include <Protocol/SmbusHc.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseLib.h>
#include <Library/MmServicesTableLib.h>
#include <Protocol/SmmSmbus.h>
#include <Library/SmbusCommonLib.h>

///
/// Max number of SMBus devices (7 bit address yields 128 combinations but 21 of those are reserved)
///
#define MAX_SMBUS_DEVICES 107
#define MICROSECOND 10
#define MILLISECOND (1000 * MICROSECOND)
#define ONESECOND   (1000 * MILLISECOND)

///
/// Declare a local instance structure for this driver
///
typedef struct _SMBUS_INSTANCE {
  UINTN                 Signature;
  EFI_HANDLE            Handle;

  ///
  /// Published interface
  ///
  EFI_SMBUS_HC_PROTOCOL SmbusController;

} SMBUS_INSTANCE;

extern SMBUS_INSTANCE  *mSmbusContext;

//
// Prototypes
//

/**
  Execute an SMBUS operation

  @param[in] This                 The protocol instance
  @param[in] DeviceAddress        The address of the SMBUS device
  @param[in] Command              The SMBUS command
  @param[in] Operation            Which SMBus protocol will be issued
  @param[in] PecCheck             If Packet Error Code Checking is to be used
  @param[in, out] Length          Length of data
  @param[in, out] Buffer          Data buffer

  @retval EFI_SUCCESS             The SMBUS operation is successful
  @retval Other Values            Something error occurred
**/
EFI_STATUS
EFIAPI
SmbusExecute (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      EFI_SMBUS_DEVICE_COMMAND      Command,
  IN      EFI_SMBUS_OPERATION           Operation,
  IN      BOOLEAN                       PecCheck,
  IN OUT  UINTN                         *Length,
  IN OUT  VOID                          *Buffer
  );

/**
  Set Device address for an Smbus device with a known UDID or perform a general
  ARP of all devices.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] ArpAll               If TRUE, do a full ARP. Otherwise, just ARP the specified UDID.
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP. On output,If
                                  ArpAlll == TRUE, this will contain the newly assigned SMBus Device address.

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusArpDevice (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      BOOLEAN                       ArpAll,
  IN      EFI_SMBUS_UDID                * SmbusUdid, OPTIONAL
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS      * DeviceAddress OPTIONAL
  );

/**
  Get a pointer to the assigned mappings of UDID's to SMBus Device Addresses.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in, out] Length          Buffer to contain the lenght of the Device Map, it will be updated to
                                  contain the number of pairs of UDID's mapped to SMBus Device Addresses.
  @param[in, out] SmbusDeviceMap  Buffer to contian a pointer to the Device Map, it will be updated to
                                  point to the first pair in the Device Map

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusGetArpMap (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN OUT  UINTN                         *Length,
  IN OUT  EFI_SMBUS_DEVICE_MAP          **SmbusDeviceMap
  );

/**
  Register a callback in the event of a Host Notify command being sent by a
  specified SMBus Device.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] DeviceAddress        Address of the device whose Host Notify command we want to
                                  trap.
  @param[in] Data                 Data of the Host Notify command we want to trap.
  @param[in] NotifyFunction       Function to be called in the event the desired Host Notify
                                  command occurs.

  @exception EFI_UNSUPPORTED      This functionality is not supported
**/
EFI_STATUS
EFIAPI
SmbusNotify (
  IN      CONST EFI_SMBUS_HC_PROTOCOL   *This,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress,
  IN      UINTN                         Data,
  IN      EFI_SMBUS_NOTIFY_FUNCTION     NotifyFunction
  );

/**
  Initialize Smbus SMM service.

  @retval EFI_SUCCESS             Services initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
InitializeSmbusSmm (
  VOID
  );

/**
  Uninstall Smbus SMM service.

  @retval EFI_SUCCESS             Service is uninstalled successfully.
  @retval Other Values            Something error occurred.

**/
EFI_STATUS
EFIAPI
UninstallSmbusSmm (
  VOID
  );

#endif
