/** @file
  PD Bridge Protocol driver definitions.

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

#ifndef _USBC_PD_BRIDGE_PROTOCOL_H_
#define _USBC_PD_BRIDGE_PROTOCOL_H_

#include <Uefi.h>
#include <PiDxe.h>
#include "UsbCPdBridgeRetimer.h"

#define PD_BRIDGE_MAX_TO_WRITE        32
#define PD_BRIDGE_NVM_VERSION_OFFSET  2
#define PD_BRIDGE_NVM_OFFSET          0
#define PD_BRIDGE_TOTAL_NUM_OF_RECOVERIES_DURING_IMAGE_WRITE 1

#define MASK(EndBit, StartBit)   (((1 << ((EndBit) - (StartBit) + 1)) - 1) << \
                                  (StartBit))
#define PD_BRIDGE_IMAGE_START_ADDRESS_MASK MASK(23, 0)

///
/// PD BRIDGE NVM ACCESS CMD
///
#define VENDOR_SPECIFIC_CMD_NVM_READ                0x1
#define VENDOR_SPECIFIC_CMD_NVM_SET_OFFSET          0x2
#define VENDOR_SPECIFIC_CMD_NVM_WRITE               0x3
#define VENDOR_SPECIFIC_CMD_NVM_AUTHENTICATE_WRITE  0x4
#define VENDOR_SPECIFIC_CMD_STALL_NVM_ACCESS        0x5
#define VENDOR_SPECIFIC_CMD_RESET                   0x6

///
/// EC PD I2C target lock status
///
#define UNLOCK_I2C  0x00 ///< Unlock the I2C target
#define LOCK_I2C    0x01 ///< Lock the I2C target

typedef struct {
  UINT32  Reserved1 : 2;  ///< Bit 0:1
  UINT32  NvmOffset : 22; ///< Bit 2:23
  UINT32  Length    : 4;  ///< Bit 24:27
  UINT32  Reserved2 : 4;  ///< Bit 28:31
} NVM_READ_CMD_METADATA;

typedef struct {
  UINT32  Reserved1 : 2;  ///< Bit 0:1
  UINT32  NvmOffset : 22; ///< Bit 2:23
  UINT32  Reserved2 : 8;  ///< Bit 24:31
} NVM_SET_OFFSET_CMD_METADATA;

///
/// Forward declaration for the USBC_PD_BRIDGE_PROTOCOL.
///
typedef struct _USBC_PD_BRIDGE_PROTOCOL USBC_PD_BRIDGE_PROTOCOL;

///
/// Protocol revision number
/// Any backwards compatible changes to this protocol will result in an update in the revision number
/// Major changes will require publication of a new protocol
///
#define USBC_PD_BRIDGE_PROTOCOL_REVISION  0x00010000  ///< Initial version
#define USBC_PD_BRIDGE_FROM_THIS(This)  CR (This, USBC_PD_BRIDGE_INSTANCE, PdBridgeProtocol, PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE)

#define USBC_PD_BRIDGE_PROTOCOL_GUID \
{ \
    0x1AC461F5, 0x8E41, 0x4C6E, { 0x9F, 0x6A, 0x88, 0x7E, 0x96, 0x59, 0x5D, 0xDC} \
}

extern EFI_GUID  gUsbCPdBridgeProtocolGuid;

/**
  The function to get PD Bridge version via EC command

  @param[in]  This             Pointer to the USBC_PD_BRIDGE_PROTOCOL instance.
  @param[in]  PdCntrlIndex     PD controller index (1-based).
  @param[in]  PdBridgeVersion  A Pointer to PD Bridge version

  @retval EFI_SUCCESS          Get PD Bridge Version successfully
  @retval others               Get PD Bridge Version failed

**/
typedef
EFI_STATUS
(*GET_PD_BRIDGE_VERSION) (
  IN USBC_PD_BRIDGE_PROTOCOL  *This,
  IN UINT8                    PdCntrlIndex,
  IN UINT64                   *PdBridgeVersion
  );

/**
  Send Command to EC to lock/unlock EC-PD regular communication

  @param[in] This             Pointer to the USBC_PD_BRIDGE_PROTOCOL instance.
  @param[in] Lock             Lock(0x01) or unlock(0x00).

  @retval EFI_SUCCESS         Lock/Unlock EC-PD regular communication successfully
  @retval EFI_ACCESS_DENIED   Already Locked or Unlocked
  @retval others              Lock/Unlock EC-PD regular communication failed

**/
typedef
EFI_STATUS
(*LOCK_COMMUNICATION) (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    Lock
  );

/**
  Execute the PD Vendor Command via EC private port

  @param[in]  This               Pointer to the USBC_PD_BRIDGE_PROTOCOL instance.
  @param[in]  PdCntrlIndex       PD controller index (0-based).
  @param[in]  VendorCmd          PD Vendor command data
  @param[in]  Lock               Need to Lock the EC PD I2C target or not
  @param[in]  InputData          A pointer to input data
  @param[in]  InputDataSize      A pointer to input data size
  @param[out] OutputData         A pointer to out data
  @param[out] OutputDataSize     A pointer to out data size

  @retval EFI_SUCCESS            Command success.
  @retval EFI_TIMEOUT            EC is busy.
  @retval EFI_ACCESS_DENIED      EC PD I2C target is lock.
  @retval EFI_INVALID_PARAMETER  Parameter invalid.
  @retval EFI_UNSUPPORTED        Unsupported EC channel or the command is not found in mEcCommand.
  @retval EFI_BUFFER_TOO_SMALL   The DataBuffer is too small to Read/Write data with EC FW.

**/
typedef
EFI_STATUS
(*EXECUTE_VENDOR_CMD) (
  IN  USBC_PD_BRIDGE_PROTOCOL  *This,
  IN  UINT8                    PdCntrlIndex,
  IN  UINT8                    VendorCmd,
  IN  BOOLEAN                  Lock,
  IN  UINT8                    *InputData,
  IN  UINT8                    *InputDataSize,
  OUT UINT8                    *OutputData OPTIONAL,
  OUT UINT8                    *OutputDataSize OPTIONAL
  );

///
/// Interface structure for the PD Bridge Protocol
///
struct _USBC_PD_BRIDGE_PROTOCOL {
  UINT64                 Revision;          ///< Revision for the protocol structure
  GET_PD_BRIDGE_VERSION  GetVersion;        ///< Get PD Bridge version
  LOCK_COMMUNICATION     Lock;              ///< Lock/Unlock EC-PD regular communication
  EXECUTE_VENDOR_CMD     ExecuteVendorCmd;  ///< Execute the PD Vendor Command via EC private port
};

typedef struct {
  UINTN                   Signature;
  USBC_PD_BRIDGE_PROTOCOL PdBridgeProtocol;
} USBC_PD_BRIDGE_INSTANCE;

#endif
