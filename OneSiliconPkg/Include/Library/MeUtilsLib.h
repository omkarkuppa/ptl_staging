/** @file
  HECI utilities for use in PEI, DXE, and SMM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _ME_UTILS_LIB_H_
#define _ME_UTILS_LIB_H_

#include <Library/SmmServicesTableLib.h>

#define NON_BLOCKING    0
#define BLOCKING        1

/**
  Get Information is we are running in SMM or not

  @param[in, out]         Pointer to EFI_SMM_SYSTEM_TABLE2 pointer

  @retval  TRUE           In SMM
  @retval  FALSE          Not in SMM
**/
BOOLEAN
MeIsInSmm (
  IN OUT EFI_SMM_SYSTEM_TABLE2  **SmmSystemTable2
  );

/**
  Return ME Mode

  @param [out] MeMode             Pointer for ME Mode report

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer of MeMode is invalid
**/
EFI_STATUS
MeBiosGetMeMode (
  OUT UINT32                       *MeMode
  );

/**
  Get an abstract Intel HECI1 State from Firmware Status Register.
  This is used to control BIOS flow for different Intel ME functions.

  @param[out] MeStatus            Pointer for status report

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeStatus is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
MeBiosGetMeStatus (
  OUT UINT32        *MeStatus
  );

/**
  Checks if Me FW Sku indicated in Firmware Status 3 is Client

  @retval FALSE           Not Client ME SKU
  @retval TRUE            Client ME SKU detected
**/
BOOLEAN
MeSkuIsClient (
  VOID
  );

/**
  Checks if Me FW is Client and it is ready

  @retval FALSE           Not ME Client and ready
  @retval TRUE            Ready Client ME detected
**/
BOOLEAN
MeClientIsReady (
  VOID
  );

/**
  Checks if Me FW is Client Corporate

  @retval FALSE           Not ME Client corporate
  @retval TRUE            Corporate Client ME detected
**/
BOOLEAN
MeClientIsCorporate (
  VOID
  );

/**
  Checks if Me FW is Client in recovery

  @retval FALSE           Client is not in recovery
  @retval TRUE            CLient is in recovery
-**/
BOOLEAN
MeClientInRecovery (
  VOID
  );

/**
  Wrapper function to reset HECI interface.
  It assumes CSME HECI1 interface with default timeout value.

  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_SUCCESS             Interface reset
  @retval EFI_UNSUPPORTED         Not supported
**/
EFI_STATUS
HeciWrapperReset (
  VOID
  );

/**
  Wrapper function to send HECI message with acknowledge.
  It assumes CSME HECI1 interface with default timeout and retries values.

  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
HeciWrapperSendWithAck (
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *ReqMsg,
  IN     UINT32              ReqLen,
     OUT UINT32              *RspMsg,
  IN OUT UINT32              *RspLen
  );

/**
  Wrapper function to send HECI message.
  It assumes CSME HECI1 interface with default timeout value.

  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message buffer
  @param[in]     ReqLen        Request message length in bytes

  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
  @retval EFI_SUCCESS           Message request has been completed successfully
**/
EFI_STATUS
HeciWrapperSend (
  IN     UINT8               HostAddress,
  IN     UINT8               EngineAddress,
  IN     UINT32              *ReqMsg,
  IN     UINT32              ReqLen
  );

/**
  Wrapper function to receive HECI message.
  It assumes CSME HECI1 interface with default timeout value.

  @param[in]     Blocking      Used to determine if the read is BLOCKING or NON_BLOCKING.
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     RspMsg        Response message buffer
  @param[in]     RspLen        Pointer to response message length in bytes

  @retval EFI_SUCCESS           Message request has been completed successfully
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
**/
EFI_STATUS
HeciWrapperReceive (
  IN     UINT32              Blocking,
  IN     UINT8               *HostAddress,
  IN     UINT8               *EngineAddress,
  IN     UINT32              *RspMsg,
  IN     UINT32              *RspLen
  );

/**
  Wait until Firmware Init Complete is set in Firmware Status Register.

  @retval EFI_SUCCESS             Firmware Init Complete bit is set
  @retval EFI_TIMEOUT             Firmware Init Complete bit is not set within specified time
**/
EFI_STATUS
WaitFirmwareInitComplete (
  VOID
  );

#endif // _ME_UTILS_LIB_H_
