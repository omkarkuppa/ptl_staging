/** @file
  Library API definitions for UsbCPdBridgeUpdateLib

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef __USBC_PD_BRIDGE_UPDATE_LIB_H__
#define __USBC_PD_BRIDGE_UPDATE_LIB_H__

#include <Uefi.h>
#include <UsbCPdBridgeRetimer.h>
#include <Protocol/UsbCPdBridgeProtocol.h>
#include <Protocol/FirmwareManagement.h>

#define SHARE_FLASH_MODE_DISABLE 0
#define SHARE_FLASH_MODE_ENABLE  1

#define START_UPDATE_PROCESS     6
#define END_UPDATE_PROCESS       90

#define USBC_PD_BRIDGE_VIRTUAL_VERSION  0x1
#define USBC_PD_BRIDGE_VERSION_NAME     L"UsbCPdBridgeVersion"

/**
  In the vendor command data, the first eight bytes represent the header and
  it is always the same. The header structure is as follows:

  | Byte Position | Description     |
  |---------------|-----------------|
  | 0             | Vendor Command  |
  | 1-3           | Reserved        |
  | 4-5           | Vendor ID       |
  | 6-7           | Product ID      |

  Initialize the vendor command data structure before sending the vendor command.

  @param[in] ProductId   The Product ID to be set in the vendor command data.

  @retval EFI_SUCCESS           The vendor command data was initialized successfully.
  @retval EFI_INVALID_PARAMETER The Product ID is invalid (zero).

**/
EFI_STATUS
InitialVendorCommandData (
  UINT16 ProductId
  );

/**
  The command to read data up to 32bytes from NVM.

  @param[in]  This            The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex        TCP index which PD Bridge is connected to.
  @param[in]  NvmOffset       This field contains the first address to be read relative to the base address of the
                              region being read. NVM Offset is specified in DWs.
  @param[in]  Length          Number of DoubleWords that shall be read starting from the NVM Offset field
                              value
  @param[out] OutputData      The Data read from NVM
  @param[out] OutputDataSize  The size of Data read from NVM

  @retval EFI_SUCCESS         Read data successfully
  @retval others              Read data failed

**/
EFI_STATUS
NvmRead (
  IN   USBC_PD_BRIDGE_PROTOCOL  *This,
  IN   UINT8                    TcpIndex,
  IN   UINT32                   NvmOffset,
  IN   UINT8                    Length,
  OUT  UINT8                    *OutputData,
  OUT  UINT8                    *OutputDataSize
  );

/**
  The command to set the first location in NVM to be written by the following NVM_WRITE command.
  This NVM offset is incremented by the CommandDataLen after each NVM_WRITE.
  The address is specified in DWs.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.
  @param[in]  NvmOffset      This field contains the first address to be written relative to the base address of the
                             region been written.

  @retval EFI_SUCCESS        Set Offset successfully
  @retval others             Read data failed

**/
EFI_STATUS
NvmSetOffset (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex,
  IN  UINT32                   NvmOffset
  );

/**
  The command writes (CommandDataLen - 1) bytes in NVM, starting at the address equal to the NVM_OFFSET.
  PD Bridge increases its NVM Offset value by (CommandDataLen - 1) after executing
  this command (even if the write operation fails).

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.
  @param[in]  Buffer         Data need to be written to NVM.
  @param[in]  BufferSize     The size of Data need to be written to NVM.

  @retval EFI_SUCCESS        write data to NVM successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmWrite (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex,
  IN  UINT8                    *Buffer,
  IN  UINT8                    BufferSize
  );

/**
  The command indicates to the PD Bridge that all data was sent to it.
  The PD Bridge performs an authentication check over the data written.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmAuthenticateWrite (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex
  );

/**
  The commands indicates to the PD Bridge that the "second" PD Bridge performs NVM updates flow.
  After receiving this command, the PD Bridge performs a PD port reset and stalls any access to the NVM
  until it gets the RESET command.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmStallNvmAccess (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex
  );

/**
  The command indicates to the PD Bridge that the "second" PD Bridge completed the authentication check over the data written.
  After receiving this command, the PD Bridge performs a SW reset and "authenticate on boot" flow again, the reloads
  all the memories/data from the NVM.

  @param[in]  This           The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex       TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS        send the command successfully
  @retval others             write data to NVM failed

**/
EFI_STATUS
NvmReset (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    TcpIndex
  );

/**
  The command indicates to the PD Bridge that the "second" PD Bridge starts NVM update flow.

  @param[in]  This      The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  TcpIndex  TCP index which PD Bridge is connected to.

  @retval EFI_SUCCESS   Send the command successfully
  @retval others        Failed to send the command

**/
EFI_STATUS
EnterNvmUpdateMode (
  IN USBC_PD_BRIDGE_PROTOCOL  *This,
  IN UINT8                    TcpIndex
  );

/**
  Update NVM Firmware on PD Bridge with given PD Bridge index.

  Flow to update PD Bridge NVM Firmware:
  1.  Send the NVM_SET_OFFSET command which set the first location in NVM to be written by the following
      NVM_WRITE command
  2.  Send a sequence of NVM_WRITE commands, each writing from 1 to 32 bytes of data to NVM. NVM offset will
      be incremented by the CommandDataLen of each NVM_WRITE
  3.a Shared Flash mode disable :
      i.   send NVM_AUTHENTICATE_WRITE command to indicate to PD Bridge that all data was sent.
  3.b Shared Flash mode enable :
      i.   send STALL_NVM_ACCESS to second PD Bridge to perform a PD port reset. PD Bridge will stall any access
           to NVM until get RESET command
      ii.  send NVM_AUTHENTICATE_WRITE command to indicate to PD Bridge that all data was sent.
      iii. send RESET command to second PD Bridge

  @param[in]  This                The UsbC PD Bridge PROTOCOL Instance.
  @param[in]  PdBridgeImage       Pointer to an image buffer contains PD Bridge FW to be updated with.
  @param[in]  ImageSize           The size of PD Bridge image buffer.
  @param[in]  PrivateData         The pointer to the Firmware Private Data
  @param[in]  Progress            A function used to report the progress of updating the firmware
                                  device with the new firmware image.
  @param[in]  StartPercentage     The start completion percentage value that may be used to report progress
                                  during the flash write operation. The value is between 1 and 100.
  @param[in]  EndPercentage       The end completion percentage value that may be used to report progress
                                  during the flash write operation.
  @param[out] LastAttemptStatus   A pointer to a UINT32 that holds the last attempt status to report back
                                  to the ESRT table in case of error. This value will only be checked when
                                  this function returns an error.

  @retval EFI_SUCCESS             PD Bridge Firmware is successfully updated.
  @retval Others                  An error occurred attempting to access the PD Bridge firmware

**/
EFI_STATUS
EFIAPI
UpdatePdBridgeNvmFirmware (
  IN   USBC_PD_BRIDGE_PROTOCOL                        *This,
  IN   UINT8                                          *PdBridgeImage,
  IN   UINTN                                          ImageSize,
  IN   FIRMWARE_PRIVATE_DATA                          *PrivateData,
  IN   EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress, OPTIONAL
  IN   UINTN                                          StartPercentage,
  IN   UINTN                                          EndPercentage,
  OUT  UINT32                                         *LastAttemptStatus
  );

#endif
