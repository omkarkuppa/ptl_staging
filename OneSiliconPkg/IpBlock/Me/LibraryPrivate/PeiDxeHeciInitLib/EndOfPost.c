/** @file
  ME End Of Post message and process implementation prior to boot OS

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/HeciInitLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/MeInfoLib.h>
#include <Register/HeciRegs.h>
#include <IntelRcStatusCode.h>
#include <MeState.h>
#include <MkhiMsgs.h>
#include <MeiBusMsg.h>

/**
  Send End of Post Request Message through HECI.

  @param[out] RequestedActions    Action request returned by EOP ACK
                                    0x00 (HECI_EOP_STATUS_SUCCESS) - Continue to boot
                                    0x01 (HECI_EOP_PERFORM_GLOBAL_RESET) - Global reset

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendEndOfPostMessage (
  OUT UINT32                      *RequestedActions
  )
{
  EFI_STATUS         Status;
  UINT32             Length;
  UINT32             RecvLength;
  END_OF_POST_BUFFER EndOfPost;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ZeroMem (&EndOfPost, sizeof (END_OF_POST_BUFFER));
  EndOfPost.Request.MkhiHeader.Fields.Command = GEN_END_OF_POST_CMD;
  EndOfPost.Request.MkhiHeader.Fields.GroupId = MKHI_GEN_GROUP_ID;
  Length                                      = sizeof (END_OF_POST_REQUEST);
  RecvLength                                  = sizeof (END_OF_POST_RESPONSE);

  PERF_INMODULE_BEGIN ("HeciEOP");
  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_MKHI_MESSAGE_ADDR,
             (UINT32 *) &EndOfPost.Request,
             Length,
             (UINT32 *) &EndOfPost.Response,
             &RecvLength
             );
  PERF_INMODULE_END ("HeciEOP");

  if (!EFI_ERROR (Status)) {
    *RequestedActions = EndOfPost.Response.Data.RequestedActions;
    if (EndOfPost.Response.Data.RequestedActions == HeciEopPerformGlobalReset) {
      DEBUG ((DEBUG_INFO, "%a () : Global Reset requested by FW EOP ACK\n", __FUNCTION__));
    }
  }

  return Status;
}

/**
  This message is sent by the BIOS if EOP-ACK not received to force ME to disable
  HECI interfaces.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             HECI interfaces disabled by ME
**/
EFI_STATUS
HeciDisableHeciBusMsg (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT32                          Length;
  UINT32                          RespLength;
  HECI_BUS_DISABLE_CMD_RESPONSE   MsgHeciBusDisable;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ZeroMem (&MsgHeciBusDisable, sizeof (HECI_BUS_DISABLE_CMD_RESPONSE));
  MsgHeciBusDisable.Command.Data = HECI_BUS_DISABLE_OPCODE;
  Length     = sizeof (HECI_BUS_DISABLE_CMD_REQUEST);
  RespLength = sizeof (HECI_BUS_DISABLE_CMD_RESPONSE);

  Status = HeciWrapperSendWithAck (
             BIOS_FIXED_HOST_ADDR,
             HECI_HBM_MESSAGE_ADDR,
             (UINT32 *) &MsgHeciBusDisable,
             Length,
             (UINT32 *) &MsgHeciBusDisable,
             &RespLength
             );
  if (!EFI_ERROR (Status) &&
      ((MsgHeciBusDisable.Command.Fields.Command != HECI_BUS_DISABLE_OPCODE) ||
       (MsgHeciBusDisable.Command.Fields.IsResponse == 0) ||
       (MsgHeciBusDisable.Status != 0))) {
    Status = EFI_ABORTED;
  }

  return Status;
}

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
  )
{
  EFI_STATUS    Status;
  UINT8         EopSendRetries;

  Status = EFI_DEVICE_ERROR;
  //PostCodeWithDescription (0x0E05, "ME-BIOS: EOP Entry.");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_EOP_ENTRY_START); //PostCode(0xE05)
  DEBUG ((DEBUG_INFO, "ME-BIOS: EOP Entry.\n"));

  for (EopSendRetries = 0; EopSendRetries < MAX_EOP_SEND_RETRIES; EopSendRetries++) {
    Status = HeciSendEndOfPostMessage (RequestedActions);
    if (!EFI_ERROR (Status)) {
      //PostCodeWithDescription (0x0E25, "ME-BIOS: EOP Exit - Success.");
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_EOP_EXIT_SUCCESS); //PostCode(0xE25)
      DEBUG ((DEBUG_INFO, "ME-BIOS: EOP Exit - Success.\n"));
      return Status;
    }
  }

  REPORT_STATUS_CODE (EFI_ERROR_CODE, SW_HECI_EOP_ERROR | EFI_STATUS_TO_STATUS_CODE_VALUE (Status));
  HeciDisableHeciBusMsg ();
  //PostCodeWithDescription (0x0E85, "ME-BIOS: EOP Exit - Error by sending EOP message. Forcing HECI interface closure.");
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_ME_EOP_EXIT_ERR_EOPMSGSNDFAIL); //PostCode(0xE85)
  DEBUG ((DEBUG_INFO, "ME-BIOS: EOP Exit - Error by sending EOP message. Forcing HECI interface closure.\n"));

  return Status;
}

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
  )
{
  EFI_STATUS           Status;
  UINT32               MeMode;
  HECI_FWS_REGISTER    MeFirmwareStatus;

  DEBUG ((DEBUG_INFO, "%a () Start\n", __FUNCTION__));

  ZeroMem (&MeFirmwareStatus, sizeof (HECI_FWS_REGISTER));
  Status = HeciGetMeMode (&MeMode);
  MeFirmwareStatus.ul = PciSegmentRead32 (MeHeciPciCfgBase (HECI1) + R_ME_CFG_HFS);
  if (EFI_ERROR (Status) && (MeFirmwareStatus.ul == 0xFFFFFFFF)) {
    MeMode              = GetMeModeFromFwstsHob ();
    MeFirmwareStatus.ul = GetMeStatusFromFwstsHob (R_ME_CFG_HFS);
  }

  ///
  /// As per Intel Converged Security and Management Engine (Intel CSME) BIOS Specification
  ///
  /// PCH ME BWG section 3.6.2
  /// The IntelR FPT tool /GRST option uses CF9GR bit to trigger global reset.
  /// Based on above reason, the BIOS should not lock down CF9GR bit during Manufacturing and
  /// Re-manufacturing environment if HFSTS1 [4] is set.
  ///
  if (((MeMode == ME_MODE_NORMAL) || (MeMode == ME_MODE_TEMP_DISABLED)) && !(MeFirmwareStatus.r.SpiProtectionMode)) {
    ///
    /// PCH ME BWG section 3.5.1
    /// BIOS must also ensure that CF9GR is cleared and locked (via bit31 of the same register) before
    /// handing control to the OS in order to prevent the host from issuing global resets and reseting
    /// Intel Management Engine.
    /// For additional requirement about Global Reset, refer to Global Reset Section in
    /// Intel PCH BIOS Specification for more details
    ///
    PmcDisableCf9GlobalResetWithLock (PmcGetPwrmBase ());
  } else {
    PmcDisableCf9GlobalReset (PmcGetPwrmBase ());
  }
}
