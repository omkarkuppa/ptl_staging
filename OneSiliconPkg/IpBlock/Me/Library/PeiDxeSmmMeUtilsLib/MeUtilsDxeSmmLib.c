/** @file
  HECI utilities for use in DXE and SMM.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MeUtilsLib.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/HeciControlProtocol.h>
#include <MeDefines.h>

STATIC BOOLEAN mInSmm = FALSE;
STATIC EFI_SMM_SYSTEM_TABLE2 *mSmmSystemTable2 = NULL;

/**
  Wrapper function to reset HECI interface.
  It assumes CSME HECI1 interface with default timeout value.

  @retval EFI_SUCCESS             Interface reset
  @retval EFI_NOT_FOUND           HECI protocol could not be found
  @retval EFI_TIMEOUT             ME is not ready
**/
EFI_STATUS
HeciWrapperReset (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_SMM_SYSTEM_TABLE2   *Smst2;
  HECI_CONTROL            *Heci;
  BOOLEAN                 InSmm;

  InSmm = MeIsInSmm (&Smst2);
  if (InSmm) {
    if (Smst2 != NULL) {
      Status = Smst2->SmmLocateProtocol (&gHeciControlSmmProtocolGuid, NULL, (VOID**) &Heci);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "%a () - HECI Control SMM protocol could not be found!\n", __FUNCTION__));
        return EFI_NOT_FOUND;
      }
    } else {
      DEBUG ((DEBUG_ERROR, "%a () - SMM System Table could not be found!\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }
  } else {
    Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &Heci);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a () - HECI Control protocol could not be found!\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }
  }

  Status = Heci->HeciReset (
                   Heci,
                   Csme,
                   HECI1,
                   NULL
                   );

  return Status;
}

/**
  Wrapper function to send HECI message with acknowledge.
  It assumes CSME HECI1 interface with default timeout and retries values.

  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message
  @param[in]     ReqLen        Request message length in bytes
  @param[out]    RspMsg        Buffer for the response message
  @param[in,out] RspLen        On input buffer size, on exit message, in bytes

  @retval EFI_SUCCESS           Message request has been completed successfully
  @retval EFI_NOT_FOUND         HECI protocol could not be found
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
**/
EFI_STATUS
HeciWrapperSendWithAck (
  IN     UINT8    HostAddress,
  IN     UINT8    EngineAddress,
  IN     UINT32   *ReqMsg,
  IN     UINT32   ReqLen,
     OUT UINT32   *RspMsg,
  IN OUT UINT32   *RspLen
  )
{
  EFI_STATUS      Status;
  HECI_CONTROL    *HeciControl;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &HeciControl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - HECI Control protocol could not be found!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  Status = HeciControl->HeciSendAndReceive (
                          HeciControl,
                          Csme,
                          HECI1,
                          NULL,
                          HECI_DEFAULT_RETRY_NUMBER,
                          HostAddress,
                          EngineAddress,
                          ReqMsg,
                          ReqLen,
                          RspMsg,
                          RspLen,
                          NULL,
                          NULL
                          );

  return Status;
}

/**
  Wrapper function to send HECI message.
  It assumes CSME HECI1 interface with default timeout value.

  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     ReqMsg        Request message buffer
  @param[in]     ReqLen        Request message length in bytes

  @retval EFI_SUCCESS           Message request has been completed successfully
  @retval EFI_NOT_FOUND         HECI protocol could not be found
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
**/
EFI_STATUS
HeciWrapperSend (
  IN     UINT8    HostAddress,
  IN     UINT8    EngineAddress,
  IN     UINT32   *ReqMsg,
  IN     UINT32   ReqLen
  )
{
  EFI_STATUS      Status;
  HECI_CONTROL    *HeciControl;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &HeciControl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - HECI Control protocol could not be found!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  Status = HeciControl->HeciSend (
                          HeciControl,
                          Csme,
                          HECI1,
                          NULL,
                          HostAddress,
                          EngineAddress,
                          ReqMsg,
                          ReqLen
                          );

  return Status;
}

/**
  Wrapper function to receive HECI message.
  It assumes CSME HECI1 interface with default timeout value.

  @param[in]     Blocking      Used to determine if the read is BLOCKING or NON_BLOCKING.
  @param[in]     HostAddress   Host address
  @param[in]     EngineAddress Engine (ME/IE) address
  @param[in]     RspMsg        Response message buffer
  @param[in]     RspLen        Pointer to response message length in bytes

  @retval EFI_SUCCESS           Message request has been completed successfully
  @retval EFI_NOT_FOUND         HECI protocol could not be found
  @retval EFI_INVALID_PARAMETER Input parameters are invalid
  @retval EFI_WRITE_PROTECTED   Can't find suitable HECI Transport
                                Possible reason: HECI filter active
  @retval EFI_DEVICE_ERROR      HECI MBar can't be initialized
  @retval EFI_TIMEOUT           Timeout occurs during operation
  @retval EFI_BAD_BUFFER_SIZE   Message is too big
**/
EFI_STATUS
HeciWrapperReceive (
  IN     UINT32   Blocking,
  IN     UINT8    *HostAddress,
  IN     UINT8    *EngineAddress,
  IN     UINT32   *RspMsg,
  IN     UINT32   *RspLen
  )
{
  EFI_STATUS      Status;
  HECI_CONTROL    *HeciControl;

  Status = gBS->LocateProtocol (&gHeciControlProtocolGuid, NULL, (VOID**) &HeciControl);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a () - HECI Control protocol could not be found!\n", __FUNCTION__));
    return EFI_NOT_FOUND;
  }

  Status = HeciControl->HeciReceive (
                          HeciControl,
                          Csme,
                          HECI1,
                          NULL,
                          HostAddress,
                          EngineAddress,
                          RspMsg,
                          RspLen
                          );

  return Status;
}

/**
  Get Information is we are running in SMM or not

  @param[in, out]         Pointer to EFI_SMM_SYSTEM_TABLE2 pointer

  @retval  TRUE           We are in SMM
  @retval  FALSE          We are not in SMM
**/
BOOLEAN
MeIsInSmm (
  IN OUT EFI_SMM_SYSTEM_TABLE2  **SmmSystemTable2
  )
{
  if (SmmSystemTable2 != NULL) {
    *SmmSystemTable2 = mSmmSystemTable2;
  }

  return mInSmm;
}

/**
  The constructor function for ME Utils Library.

  The functions saves information about if the bios is running in SMM and pointer to
  SmmSystemTable2 in modules variables. These variables are used in MeIsInSmm function.

  @param[in] ImageHandle          The firmware allocated handle for the EFI image.
  @param[in] SystemTable          A pointer to the EFI System Table.

  @retval EFI_SUCCESS             The function always return EFI_SUCCESS.
  @retval EFI_INVALID_PARAMETER   Smst was invalid.
  @retval EFI_UNSUPPORTED         Smst used outside of SMM.
**/
EFI_STATUS
EFIAPI
MeUtilsLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_BASE2_PROTOCOL         *SmmBase2;
  EFI_SMM_SYSTEM_TABLE2          *Smst2;

  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &SmmBase2
                  );
  if (EFI_ERROR (Status)) {
    mInSmm = FALSE;
    return EFI_SUCCESS;
  }

  Smst2 = NULL;
  mInSmm = FALSE;
  SmmBase2->InSmm (
              SmmBase2,
              &mInSmm
              );
  if (mInSmm) {
    Status = SmmBase2->GetSmstLocation (
                         SmmBase2,
                         &Smst2
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a () - Could not get SMM System Table location!\n", __FUNCTION__));
      return Status;
    }

    mSmmSystemTable2 = Smst2;
  }

  return EFI_SUCCESS;
}
