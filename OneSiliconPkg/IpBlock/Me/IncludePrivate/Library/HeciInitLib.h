/** @file
  Definitions for HECI driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#ifndef _HECI_INIT_LIB_H
#define _HECI_INIT_LIB_H

#include <Uefi.h>
#include <MeDefines.h>
#include <MkhiMsgs.h>

/**
  Abstract ME BIOS Boot Path definitions.
  BIOS Boot Path vs. MEI1 Host Firmware Status 1, 4, and 5 registers:
  MeNormalBiosPath -
    If (HECI_FWS_REGISTER.r.CurrentState == ME_STATE_NORMAL AND
      HECI_FWS_REGISTER.r.ErrorCode == ME_ERROR_CODE_NO_ERROR)
    - BIOS takes the normal firmware BIOS path.

  MeErrorWithoutDidMsgBiosPath -
    If HECI_FWS_REGISTER.r.ErrorCode == ME_ERROR_CODE_IMAGE_FAILURE
    - The BIOS does not send any Intel MEI messages including DID and EOP message.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - MEI 1 device should be enabled by the BIOS so it allows the user to update the new firmware and
      take the firmware out of a recovery or error condition. Hide MEI 2, MEI3, SOL and IDER. In addition,
      Image Failure results in a platform involuntary 30-minute shut down triggered by Intel ME.
      BIOS shall post the warning message as part of the error handling flow.

  MeErrorBiosPath -
    If (HECI_FWS_REGISTER.r.FptBad == 0x01) OR
      (HECI_FWS_REGISTER.r.ErrorCode != ME_ERROR_CODE_NO_ERROR) OR
      (HECI_FWS_REGISTER.r.ErrorCode != ME_ERROR_CODE_IMAGE_FAILURE)
    - The BIOS does not send any Intel MEI messages except for the DRAM Init Done message. Moreover,
      the BIOS doesn't even send EOP message.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - MEI 1 device should be enabled by the BIOS so it allows the user to update the new firmware and
      take the firmware out of a recovery or error condition. Hide MEI 2, SOL and IDER.

  MeRecoveryBiosPath -
    If HECI_FWS_REGISTER.r.CurrentState == ME_STATE_RECOVERY
    - The BIOS does not send any Intel MEI messages except for MEI Bus and MKHI messages, MKHI MCA Group is not allowed.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - MEI 1 device should be enabled by the BIOS so it allows the user to update the new firmware and
      take the firmware out of a recovery or error condition. Hide MEI 2, SOL and IDER.

  MeDebugModeBiosPath -
    If HECI_FWS_REGISTER.r.MeOperationMode == ME_OPERATION_MODE_DEBUG
    - The BIOS does not send any Intel MEI messages except for DID and EOP message.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - Hide MEI1, MEI2, MEI3, SOL and IDER before OS boot. It means there is no MEI drivers loaded in OS environment.

  MeSwTempDisableBiosPath
    If (HECI_FWS_REGISTER.r.MeOperationMode == ME_OPERATION_MODE_SOFT_TEMP_DISABLE)
    - The BIOS does not send any Intel MEI messages except for the DRAM Init Done message, Set Me Enable message
      and Global Reset Message.
      Moreover, the BIOS doesn't even send EOP message.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - Hide MEI2, MEI3, SOL and IDER before OS boot. It means there is no MEI drivers loaded in OS environment.
      Hides MEI1 device after sending the Set Me Enable message or prior to boot.
      It means there is no MEI drivers loaded in OS environment.

  MeSecoverJmprBiosPath
    If (HECI_FWS_REGISTER.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_JMPR)
    - The BIOS does not send any Intel MEI messages except for the DRAM Init Done message. Moreover,
      the BIOS doesn't even send EOP message.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - Hide MEI1, MEI2, MEI3, SOL and IDER before OS boot. It means there is no MEI drivers loaded in OS environment.

  MeSecoverMeiMsgBiosPath
    If HECI_FWS_REGISTER.r.MeOperationMode == ME_OPERATION_MODE_SECOVR_HECI_MSG
    - The BIOS does not send any Intel MEI messages except for the DRAM Init Done (DID) message and HMRFPO DISABLE message.
      The HMRFPO DISABLE message is to bring the firmware out of SECOVR_MEI_MSG operation mode back to normal.
    - The BIOS does not invoke the Intel MEBX and should hide Intel Remote Assistance hotkey entry.
    - Hide MEI2, MEI3, SOL and IDER before OS boot.
      Hides MEI1 device after sending the HMRFPO DISABLE message.
      It means there is no MEI drivers loaded in OS environment.
**/

/**
  The FW registers may report multiple status to reflect Me Bios boot path, BIOS will follow the
  prioritized Me Bios boot path to continue boot. If the priority will be changed, then
  BOOT_PATH enumeration shall be adjusted as well to reflect real priority.
**/
typedef enum {
  NormalPath = 0,
  ErrorPath,
  RecoveryPath,
  ErrorWithoutDidMsgPath,
  SecoverMeiMsgPath,
  SwTempDisablePath,
  SecoverJmprPath,
  DebugModePath,
  MaxPathValue
} ME_BIOS_BOOT_PATH;

/**
  Abstract devices map for CSME devices to be hidden
  Bit0 - MEI1
  Bit1 - MEI2
  Bit2 - MEI3
  Bit3 - MEI4
  Bit7 - SOL
**/
typedef UINT8 ME_DEV_EXPOSURE;

#define HIDE_MEI1                BIT0
#define HIDE_MEI2                BIT1
#define HIDE_MEI3                BIT2
#define HIDE_MEI4                BIT3
#define HIDE_SOL                 BIT7

#define HIDE_AMT_DEVICE          HIDE_SOL
#define HIDE_ALL_ME_DEVICE       (HIDE_MEI1|HIDE_MEI2|HIDE_MEI3|HIDE_MEI4|HIDE_AMT_DEVICE)

/**
  Abstract MEI message allowance in non MeNormalBiosPath definition
  Bit0  - IMR and DID messages are allowed to be sent with this BIOS path
  Bit1  - EOP message is allowed to be sent with this BIOS path
  Bit2  - HMRFPO DISABLE message is allowed to be sent with this BIOS path
  Bit3  - SET ME ENABLE message is allowed to be sent with this BIOS path
  Bit4  - GLOBAL RESET message is allowed to be sent with this BIOS path
  BIT5  - HMRFPO messages are allowed to be sent with this ME Bios boot path
  BIT6  - GEN messages are allowed to be sent with this ME Bios boot path
  BIT7  - MEI BUS messages are allowed to be sent with this ME Bios boot path
  BIT8  - Messages that for dynamic ME addressN are allowed to be sent with this ME Bios boot path
  BIT9  - All MKHI messages except for MCA Group are allowed to be sent with this ME Bios boot path
  BIT10 - READ FROM MPHY and SET EDEBUG STATE messages are allowed to be sent with this BIOS path
  BIT11 - CORE BIOS DONE message is allowed to be sent with this BIOS path
  BIT12 - Measurement messages are allowed to be sent with this BIOS path
  BIT13 - IP LOAD message is allowed to be sent with this BIOS path
  BIT14 - GET BIOS SEED message is allowed to be sent with this BIOS path
  BIT15 - TERMINATE HECI message is allowed to be sent with this BIOS path
  BIT31 - All messages are allowed to be sent with this ME Bios boot path
**/
typedef UINT32 MESSAGE_ALLOWANCE;

#define IMR_AND_DID_MSG_ALLOWANCE     BIT0
#define EOP_MSG_ALLOWANCE             BIT1
#define HMRFPO_DISABLE_MSG_ALLOWANCE  BIT2
#define SET_ME_ENABLE_MSG_ALLOWANCE   BIT3
#define GLOBAL_RST_MSG_ALLOWANCE      BIT4
#define HMRFPO_GRP_MSG_ALLOWANCE      BIT5
#define GEN_GRP_MSG_ALLOWANCE         BIT6
#define MEI_BUS_MSG_ALLOWANCE         BIT7
#define DYN_CLIENT_MSG_ALLOWANCE      BIT8
#define MKHI_MSG_ALLOWANCE            BIT9
#define CORE_BIOS_DONE_ALLOWANCE      BIT11
#define MEASUREMENT_MSG_ALLOWANCE     BIT12
#define GET_IP_FIRMWARE_MSG_ALLOWANCE BIT13
#define BIOS_SEED_ALLOWANCE           BIT14
#define TERMINATE_HECI_MSG_ALLOWANCE  BIT15
#define ALL_MSG_ALLOWANCE             BIT31

//
// Prototypes
//
/**
  Checks ME Boot path. The function provides ME BIOS boot path based on current
  HECI1 FW Status Register.  HECI1 must be enabled before invoking the function.
  The FW registers may report multiple statuses to reflect Me Bios boot path,
  BIOS will follow the prioritized Me Bios boot path to continue boot.
  If the priority changes, then BOOT_PATH enumerated type shall be
  adjusted as well to reflect the actual priority.

  @retval ME_BIOS_BOOT_PATH       Me-Bios Path taken based on FWSTS registers
                                  @see ME_BIOS_BOOT_PATH - for available paths
**/
ME_BIOS_BOOT_PATH
CheckMeBootPath (
  VOID
  );

/**
  Check if this message is allowed with the current ME Bios boot path.
  HECI1 must be enabled before invoking the function.

  @param[in] SecurityEngine     The engine where HECI is located
  @param[in] HeciDev            The HECI device to be accessed.
  @param[in] HostAddr           The address of the host processor.
  @param[in] MeAddr             Address of the ME subsystem the message is being sent to.
  @param[in] MkhiHeader         Pointer to the message header.

  @retval TRUE                  This message is allowed
  @retval FALSE                 This message is blocked
**/
BOOLEAN
CheckMsgAllowance (
  IN SECURITY_ENGINE            SecurityEngine,
  IN HECI_DEVICE                HeciDev,
  IN UINT8                      HostAddr,
  IN UINT8                      MeAddr,
  IN MKHI_MESSAGE_HEADER        *MkhiHeader
  );

/**
  Determines if the HECI device is present.
  If present, initializes it to be used by the BIOS.

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_DEVICE_ERROR        No HECI device
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         HECI MSG is unsupported
**/
EFI_STATUS
EFIAPI
HeciInitialize (
  IN HECI_DEVICE                  HeciDev
  );

/**
  Heci Re-initialization for Host

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             Heci Re-initialization successful
  @retval EFI_TIMEOUT             ME is not ready
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
**/
EFI_STATUS
EFIAPI
HeciReInitialize (
  IN HECI_DEVICE                  HeciDev
  );

/**
  Reads a message from CSME through HECI.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Blocking             Used to determine if the read is BLOCKING or NON_BLOCKING.
  @param[in, out] MessageBody     Pointer to a buffer used to receive a message.
  @param[in, out] Length          Pointer to the length of the buffer on input and the length
                                  of the message on return. (in bytes)

  @retval EFI_SUCCESS             One message packet read.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             Failed to receive a full message on time
  @retval EFI_NO_RESPONSE         No response from CSME
  @retval EFI_BUFFER_TOO_SMALL    The caller's buffer was not large enough
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciReceive (
  IN      HECI_DEVICE             HeciDev,
  IN      UINT32                  Blocking,
  IN OUT  UINT32                  *MessageBody,
  IN OUT  UINT32                  *Length
  );

/**
  Function sends one message (of any length) through the HECI circular buffer.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Message              Pointer to the message data to be sent.
  @param[in] Length               Length of the message in bytes.
  @param[in] HostAddress          The address of the host processor.
  @param[in] MeAddress            Address of the ME subsystem the message is being sent to.

  @retval EFI_SUCCESS             One message packet sent.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             CSME failed to empty the circular buffer
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSend (
  IN HECI_DEVICE                  HeciDev,
  IN UINT32                       *Message,
  IN UINT32                       Length,
  IN UINT8                        HostAddress,
  IN UINT8                        MeAddress
  );

/**
  Function sends one message through the HECI circular buffer and waits
  for the corresponding ACK message.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in][out] Message         Pointer to the message buffer.
  @param[in] Length               Length of the message in bytes.
  @param[in][out] RecLength       Length of the message response in bytes.
  @param[in] HostAddress          Address of the sending entity.
  @param[in] MeAddress            Address of the ME entity that should receive the message.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             CSME failed to empty or fill the circular buffer
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciSendwAck (
  IN      HECI_DEVICE             HeciDev,
  IN OUT  UINT32                  *Message,
  IN      UINT32                  Length,
  IN OUT  UINT32                  *RecLength,
  IN      UINT8                   HostAddress,
  IN      UINT8                   MeAddress
  );

/**
  Me reset and waiting for ready

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Delay                The biggest waiting time

  @retval EFI_SUCCESS             Host Ready bit cleared
  @retval EFI_TIMEOUT             Host Ready bit not cleared
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
**/
EFI_STATUS
EFIAPI
MeResetWait (
  IN  HECI_DEVICE                 HeciDev,
  IN  UINT32                      Delay
  );

/**
  Function forces a reinit of the heci interface by following the reset heci interface via host algorithm
  in HPS section 4.1.1.1

  @param[in] HeciDev              The HECI device to be accessed.

  @retval EFI_SUCCESS             Interface reset successful
  @retval EFI_TIMEOUT             ME is not ready
**/
EFI_STATUS
EFIAPI
ResetHeciInterface (
  IN  HECI_DEVICE                 HeciDev
  );

/**
  Get an abstract Intel HECI1 State from Firmware Status Register.
  This is used to control BIOS flow for different Intel ME functions.

  @param[out] MeStatus            Pointer for status report
                                    @see MeState.h - Abstract ME status definitions.

  @retval EFI_SUCCESS             MeStatus copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeStatus is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
HeciGetMeStatus (
  OUT UINT32                      *MeStatus
  );

/**
  Get HECI1 Mode

  @param[out] MeMode              Pointer for HECI1 Mode report
                                    @see MeState.h - Abstract ME Mode definitions.

  @retval EFI_SUCCESS             MeMode copied
  @retval EFI_INVALID_PARAMETER   Pointer to MeMode is invalid
  @retval EFI_NOT_FOUND           HECI1 Device hidden
**/
EFI_STATUS
EFIAPI
HeciGetMeMode (
  OUT UINT32                       *MeMode
  );

/**
  Function sends one message (of any length) through the HECI circular buffer asynchronously.

  @param[in] HeciDev              The HECI device to be accessed.
  @param[in] Message              Pointer to the message data to be sent.
  @param[in] Length               Length of the message in bytes.
  @param[in] HostAddress          The address of the host processor.
  @param[in] MeAddress            Address of the ME subsystem the message is being sent to.
  @param[in] Event                Pointer to asynchronous event.
  @param[in] RecvMessageBuffer    Pointer to asynchronous message receive buffer.
  @param[in] RecvLength           Length of the asynchronous message response.

  @retval EFI_SUCCESS             One message packet sent.
  @retval EFI_DEVICE_ERROR        Failed to initialize HECI
  @retval EFI_NOT_READY           HECI is not ready for communication
  @retval EFI_TIMEOUT             CSME failed to empty the circular buffer
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
**/
EFI_STATUS
EFIAPI
HeciAsyncSend (
  IN HECI_DEVICE               HeciDev,
  IN UINT32                    *Message,
  IN UINT32                    Length,
  IN UINT8                     HostAddress,
  IN UINT8                     MeAddress,
  IN EFI_EVENT                 *Event,
  IN UINT32                    *RecvMessageBuffer,
  IN UINT32                    RecvLength
  );

/**
  Send End of Post message to ME FW.

  @param[out] RequestedActions    Action request returned by EOP ACK
                                    0x00 (HECI_EOP_STATUS_SUCCESS) - Continue to boot
                                    0x01 (HECI_EOP_PERFORM_GLOBAL_RESET) - Global reset

  @retval EFI_SUCCESS             Platform reached End of Post successfully
  @retval EFI_DEVICE_ERROR        An error has occured by EoP message
**/
EFI_STATUS
MeEndOfPostEvent (
  OUT UINT32    *RequestedActions
  );

/**
  Perform Cf9Gr Lock Config
    - PCH BIOS Spec Rev 0.9 Section 18.4  Additional Power Management Programming
      Set "Power Management Initialization Register (PMIR) Field 1", D31:F0:ACh[31] = 1b
      for production machine according to "RS - PCH Intel Management Engine
      (Intel(r) ME) BIOS Writer's Guide".
**/
VOID
LockConfig (
  VOID
  );

//
// Prototype for ME Policy from PEI and DXE phase
//

/**
  Read the ME policy to see if HECI timeouts are enabled.
  @retval TRUE             Timeout is enabled
  @retval FALSE            Timeout is disabled
**/
extern
BOOLEAN
MeHeciTimeoutsEnabled (
  VOID
  );

/**
  Read the ME policy to see if HECI message check is enabled for Bios Boot Path.

  @retval TRUE             HECI message check is enabled
  @retval FALSE            HECI message check is disabled
**/
BOOLEAN
MeHeciMessageCheckEnabled (
  VOID
  );

#endif // _HECI_INIT_LIB_H

