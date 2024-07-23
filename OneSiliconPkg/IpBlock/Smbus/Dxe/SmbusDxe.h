/** @file
  DXE Smbus Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2004 Intel Corporation.

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
#ifndef _SMBUS_DXE_H
#define _SMBUS_DXE_H

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseLib.h>
#include <Protocol/SmbusHc.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <PiDxe.h>
#include <Library/HobLib.h>
#include <Guid/HobList.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/SmbusCommonLib.h>
#include <IndustryStandard/Pci30.h>
#include <Register/PchRegs.h>
#include <Register/SmbusRegs.h>
#include <Library/PchPciBdfLib.h>
#include <SmbusConfigHob.h>

///
/// Max number of SMBus devices (7 bit address yields 128 combinations but 21 of those are reserved)
///
#define MAX_SMBUS_DEVICES 107
#define MICROSECOND 10
#define MILLISECOND (1000 * MICROSECOND)
#define ONESECOND   (1000 * MILLISECOND)

///
/// Private Data Structures
///
typedef struct _SMBUS_NOTIFY_FUNCTION_LIST_NODE {
  UINT32                    Signature;
  LIST_ENTRY                Link;
  EFI_SMBUS_DEVICE_ADDRESS  DeviceAddress;
  UINTN                     Data;
  EFI_SMBUS_NOTIFY_FUNCTION NotifyFunction;
} SMBUS_NOTIFY_FUNCTION_LIST_NODE;

#define SMBUS_NOTIFY_FUNCTION_LIST_NODE_FROM_LINK(_node) \
  CR ( \
  _node, \
  SMBUS_NOTIFY_FUNCTION_LIST_NODE, \
  Link, \
  PCH_SMBUS_PRIVATE_DATA_SIGNATURE \
  )

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

  UINT8                 DeviceMapEntries;
  EFI_SMBUS_DEVICE_MAP  DeviceMap[MAX_SMBUS_DEVICES];

  UINT8                 PlatformNumRsvd;
  CONST UINT8           *PlatformRsvdAddr;

  LIST_ENTRY            NotifyFunctionList;
  EFI_EVENT             NotificationEvent;

} SMBUS_INSTANCE;

//
// Driver global data
//
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
  @param[out] Length              Length of data
  @param[out] Buffer              Data buffer

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
  <b>SMBus DXE Module Entry Point</b>\n
  - <b>Introduction</b>\n
    The SMBus module is a DXE Library that provides a standard way to execute an SMBus command.

  - @pre
    - @link _PCH_POLICY PCH_POLICY_HOB @endlink
      - This module uses the parameters in SmbusConfig for platform reserved (non ARP capable) addresses.

  - @result
    The SMBus driver produces EFI_SMBUS_HC_PROTOCOL which is documented in the SMBus Host Controller
    Protocol Specification.

  @param[in] ImageHandle          ImageHandle of this module
  @param[in] SystemTable          EFI System Table

  @retval EFI_SUCCESS             Driver initializes successfully
  @retval Other values            Some error occurred
**/
EFI_STATUS
EFIAPI
InitializeSmbus (
  IN EFI_HANDLE                         ImageHandle,
  IN EFI_SYSTEM_TABLE                   *SystemTable
  );

/**
  Set address for an Smbus device with a known UDID or perform a general
  ARP of all devices.

  @param[in] This                 Pointer to the instance of the EFI_SMBUS_HC_PROTOCOL.
  @param[in] ArpAll               If TRUE, do a full ARP. Otherwise, just ARP the specified UDID.
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @@param[in, out] DeviceAddress  Buffer to store new Device Address during directed ARP. On output,If
                                  ArpAlll == TRUE, this will contain the newly assigned address.

  @retval EFI_INVALID_PARAMETER   ArpAll == FALSE but SmbusUdid or DeviceAddress are NULL.
                                  Return value from SmbusFullArp() or SmbusDirectedArp().
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

  @retval EFI_SUCCESS             Function completed successfully.
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

  @exception EFI_UNSUPPORTED      Unable to create the event needed for notifications.
  @retval EFI_INVALID_PARAMETER   NotifyFunction was NULL.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate space to register the notification.
  @retval EFI_SUCCESS             Function completed successfully
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
  Set up a periodic event so that we can check if any SMBus Device has sent a
  Notify ARP Smbus Host Controller command.

  @retval EFI_SUCCESS             Periodic event successfully set up.
  @retval Other Errors            Failed to set up Periodic event.
                                  Error value from CreateEvent().
                                  Error value from SetTimer().
**/
EFI_STATUS
InitializePeriodicEvent (
  VOID
  );

/**
  Function to be called every time periodic event happens. This will check if
  the SMBus Host Controller has received a Host Notify command. If so, it will
  see if a notification has been reqested on that event and make any callbacks
  that may be necessary.

  @param[in] Event                The periodic event that occured and got us into this callback.
  @param[in] Context              Event context. Will be NULL in this case, since we already have our
                                  private data in a module global variable.

**/
VOID
EFIAPI
PollSmbusNotify (
  IN  EFI_EVENT                         Event,
  IN  VOID                              *Context
  );

/**
  Issue a prepare ARP command to informs all devices that the ARP Smbus Host Controller is starting the ARP process

  @param[in] Private              Pointer to the SMBUS_INSTANCE

  @retval EFI_SUCCESS             The SMBUS operation is successful
  @retval Other Values            Something error occurred
**/
EFI_STATUS
SmbusPrepareToArp (
  IN      SMBUS_INSTANCE                *Private
  );

/**
  Do a fully (general) Arp procress to assign the SMBus Device address of all ARP-capable device.
  This function will issue issue the "Prepare to ARP", "Get UDID" and "Assign Address" commands.

  @param[in] Private              Pointer to the SMBUS_INSTANCE

  @retval EFI_OUT_OF_RESOURCES    No available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusFullArp (
  IN      SMBUS_INSTANCE                *Private
  );

/**
  Do a directed Arp procress to assign the SMBus Device address of a single ARP-capable device.

  @param[in] Private              Pointer to the SMBUS_INSTANCE
  @param[in] SmbusUdid            When doing a directed ARP, ARP the device with this UDID.
  @param[in, out] DeviceAddress   Buffer to store new SMBus Device Address during directed ARP.

  @retval EFI_OUT_OF_RESOURCES    DeviceMapEntries is more than Max number of SMBus devices
                                  Or there is no available address to assign

  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
SmbusDirectedArp (
  IN      SMBUS_INSTANCE                *Private,
  IN      EFI_SMBUS_UDID                *SmbusUdid,
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS      *DeviceAddress
  );

/**
  Find an available address to assign

  @param[in] Private              Pointer to the SMBUS_INSTANCE
  @param[in] DeviceAddress        Buffer to store new SMBus Device Address during directed ARP.

  @retval EFI_OUT_OF_RESOURCES    There is no available address to assign
  @retval EFI_SUCCESS             The function completed successfully
**/
EFI_STATUS
GetNextAvailableAddress (
  IN      SMBUS_INSTANCE                *Private,
  IN OUT  EFI_SMBUS_DEVICE_ADDRESS      *DeviceAddress
  );

/**
  Check whether the address is assignable.

  @param[in] Private              Pointer to the SMBUS_INSTANCE
  @param[in] DeviceAddress        The SMBus Device Address for checking

  @retval TRUE                    The address is assignable
  @retval FALSE                   The address is not assignable
**/
BOOLEAN
IsAddressAvailable (
  IN      SMBUS_INSTANCE                *Private,
  IN      EFI_SMBUS_DEVICE_ADDRESS      DeviceAddress
  );

#endif
