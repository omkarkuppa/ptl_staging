/** @file
  SMM Replay Protected Monotonic Counter (RPMC) Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include <Base.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PcdLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/MeChipsetLib.h>
#include <Protocol/HeciControlProtocol.h>
#include <Protocol/TrustedChannelProtocol.h>
#include <MeDefines.h>
#include <BupMsgs.h>
#include <HeciAddresses.h>
#include <TrustedChannelMsgs.h>

#include <Library/PreSiliconEnvDetectLib.h>

#define RPMC_COUNTER_0  0
#define RPMC_COUNTER_1  1

HECI_CONTROL                   *mHeciProtocol               = NULL;
HECI_TRUSTED_CHANNEL_PROTOCOL  *mHeciTrustedChannelProtocol = NULL;
STATIC BOOLEAN                 mRpmcSupported               = FALSE;

/**
  Requests the current monotonic counter from the designated RPMC counter.

  @param[in]    CounterIndex            The RPMC index
  @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
RequestMonotonicCounter (
  IN  UINT8   CounterIndex,
  OUT UINT32  *CounterValue
  )
{
  EFI_STATUS                                   Status;
  BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_BUFFER  RpmcRead;
  UINT32                                       RespLength;
  BOOLEAN                                      IsVerificationSuccessful;
  BOOLEAN                                      D0I3BitCleared;

  ///
  /// Check RPMC & HECI interfaces are available before issuing commands
  ///
  if (mRpmcSupported == FALSE) {
    DEBUG ((DEBUG_INFO, "[RPMC] RPMC is not supported\n"));
    return EFI_UNSUPPORTED;
  }

  if (mHeciProtocol == NULL) {
    DEBUG ((DEBUG_INFO, "[RPMC] Unable to locate Heci Protocol\n"));
    return EFI_NOT_READY;
  }

  ///
  /// Set up Read Monotonic Counter Heci message
  ///
  ZeroMem ((VOID *) &RpmcRead, sizeof (BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_BUFFER));
  RpmcRead.Request.Header.TrustedHeader.Command.IsResponse = FALSE;
  RpmcRead.Request.Header.TrustedHeader.Command.Id         = BIOS_READ_AR_MC_COUNTER_CMD;
  RpmcRead.Request.CounterId                               = CounterIndex;

  ///
  /// Update the Trusted Header with message Hash
  ///
  Status = mHeciTrustedChannelProtocol->UpdateTrustedHeader (
                                          (VOID *) &RpmcRead.Request,
                                          sizeof (BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_REQUEST),
                                          0
                                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[RPMC] An error occurred updating the trusted header.\n"));
    return Status;
  }

  ///
  /// Send Read Monotonic Counter Heci message to CSME
  ///
  RespLength = sizeof (BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_RESPONSE);

  ///
  /// Detect whether D0I3 bit is set?  If it is set,
  /// clear it in order to communicate with CSME
  ///
  D0I3BitCleared = FALSE;
  if (IsD0I3BitEnabled (HECI2)) {
    ClearD0I3Bit (HECI2);
    D0I3BitCleared = TRUE;
  }

  Status = mHeciProtocol->HeciSendAndReceive (
                            mHeciProtocol,
                            Csme,
                            HECI2,
                            NULL,
                            HECI_DEFAULT_RETRY_NUMBER,
                            BIOS_FIXED_HOST_ADDR,
                            HECI_BIOS_STORAGE_MESSAGE_ADDR,
                            (UINT32 *) &RpmcRead.Request,
                            sizeof (BIOS_STORAGE_READ_MC_TRUSTED_CHANNEL_REQUEST),
                            (UINT32 *) &RpmcRead.Response,
                            &RespLength,
                            NULL,
                            NULL
                            );
  ///
  /// If D0I3 bit was cleared above then set it.
  ///
  if (D0I3BitCleared) {
    SetD0I3Bit (HECI2);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "[RPMC] Request Monotonic Counter[%d] Failed Status = %r\n", CounterIndex, Status));
    return Status;
  }

  ///
  /// Check Read Monotonic Counter message response for errors
  ///
  if ((RpmcRead.Response.Header.TrustedHeader.Command.Id != BIOS_READ_AR_MC_COUNTER_CMD) &&
      (RpmcRead.Response.Header.TrustedHeader.Command.IsResponse != 1) &&
      (RpmcRead.Response.Header.Status != 0)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Send Read Monotonic Counter Request Msg processed with error. Response: \n"));
    DEBUG ((DEBUG_INFO, "  CommandId  = 0x%02x\n", (UINT8)RpmcRead.Response.Header.TrustedHeader.Command.Id));
    DEBUG ((DEBUG_INFO, "  IsResponse = 0x%02x\n", (UINT8)RpmcRead.Response.Header.TrustedHeader.Command.IsResponse));
    DEBUG ((DEBUG_INFO, "  Status     = 0x%02x\n", (UINT8)RpmcRead.Response.Header.Status));
    return EFI_DEVICE_ERROR;
  }

  ///
  /// Check the Trusted Header hash
  ///
  IsVerificationSuccessful = FALSE;
  Status = mHeciTrustedChannelProtocol->VerifyTrustedHeader (
                                          (VOID *) &RpmcRead.Response,
                                          RespLength,
                                          &IsVerificationSuccessful
                                          );
  if (EFI_ERROR (Status) || (IsVerificationSuccessful == FALSE)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Trusted Header hash failed\n"));
    DEBUG ((DEBUG_INFO, "  Status                   = %r\n", Status));
    DEBUG ((DEBUG_INFO, "  IsVerificationSuccessful = 0x%X\n", IsVerificationSuccessful));
    return EFI_DEVICE_ERROR;
  }

  ///
  /// return counter value since pre-checks looks good
  ///
  *CounterValue = RpmcRead.Response.Counter;
  //DEBUG ((DEBUG_INFO, "[RPMC] Request Monotonic Counter[%d] Value = 0x%08x\n", CounterIndex, *CounterValue));

  return Status;
}

/**
  Increments the designated monotonic counter in the SPI flash device by 1.

  @param[in]    CounterIndex            The RPMC index

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
IncrementMonotonicCounter (
  IN  UINT8   CounterIndex
  )
{
  EFI_STATUS                                        Status;
  BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_BUFFER  RpmcIncrement;
  UINT32                                            RespLength;
  BOOLEAN                                           IsVerificationSuccessful;
  BOOLEAN                                           D0I3BitCleared;

  ///
  /// Check RPMC & HECI interfaces are available before issuing commands
  ///
  if (mRpmcSupported == FALSE) {
    DEBUG ((DEBUG_INFO, "[RPMC] RPMC is not supported\n"));
    return EFI_UNSUPPORTED;
  }

  if (mHeciProtocol == NULL) {
    DEBUG ((DEBUG_INFO, "[RPMC] Unable to locate Heci Protocol\n"));
    return EFI_NOT_READY;
  }

  ///
  /// Set up Increment Monotonic Counter Heci message
  ///
  ZeroMem ((VOID *) &RpmcIncrement, sizeof (BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_BUFFER));
  RpmcIncrement.Request.Header.TrustedHeader.Command.IsResponse = FALSE;
  RpmcIncrement.Request.Header.TrustedHeader.Command.Id         = BIOS_INCREMENT_AR_MC_CMD;
  RpmcIncrement.Request.CounterId                               = CounterIndex;

  ///
  /// Update the Trusted Header with message Hash
  ///
  Status = mHeciTrustedChannelProtocol->UpdateTrustedHeader (
                                          (VOID *) &RpmcIncrement.Request,
                                          sizeof (BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_REQUEST),
                                          0
                                          );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[RPMC] An error occurred updating the trusted header.\n"));
    return Status;
  }

  ///
  /// Send Increment Monotonic Counter Heci message to CSME
  ///
  RespLength = sizeof (BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_RESPONSE);

  ///
  /// Detect whether D0I3 bit is set?  If it is set,
  /// clear it in order to communicate with CSME
  ///
  D0I3BitCleared = FALSE;
  if (IsD0I3BitEnabled (HECI2)) {
    ClearD0I3Bit (HECI2);
    D0I3BitCleared = TRUE;
  }

  Status = mHeciProtocol->HeciSendAndReceive (
                            mHeciProtocol,
                            Csme,
                            HECI2,
                            NULL,
                            HECI_DEFAULT_RETRY_NUMBER,
                            BIOS_FIXED_HOST_ADDR,
                            HECI_BIOS_STORAGE_MESSAGE_ADDR,
                            (UINT32 *) &RpmcIncrement.Request,
                            sizeof (BIOS_STORAGE_INCREMENT_MC_TRUSTED_CHANNEL_REQUEST),
                            (UINT32 *) &RpmcIncrement.Response,
                            &RespLength,
                            NULL,
                            NULL
                            );
  ///
  /// If D0I3 bit was cleared above then set it.
  ///
  if (D0I3BitCleared) {
    SetD0I3Bit (HECI2);
  }

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[RPMC] Increment Monotonic Counter[%d] Failed Status = %r\n", CounterIndex, Status));
    return Status;
  }

  ///
  /// Check Increment Monotonic Counter message response for errors
  ///
  if ((RpmcIncrement.Response.Header.TrustedHeader.Command.Id == BIOS_INCREMENT_AR_MC_CMD) &&
      (RpmcIncrement.Response.Header.TrustedHeader.Command.IsResponse == 1) &&
      (RpmcIncrement.Response.Header.Status == 0)) {
    Status = EFI_SUCCESS;
  } else {
    Status = EFI_DEVICE_ERROR;
    DEBUG ((DEBUG_INFO, "[RPMC] Send Increment Monotonic Counter Msg processed with error. Response: \n"));
    DEBUG ((DEBUG_INFO, "  CommandId  = 0x%02x\n", (UINT8)RpmcIncrement.Response.Header.TrustedHeader.Command.Id));
    DEBUG ((DEBUG_INFO, "  IsResponse = 0x%02x\n", (UINT8)RpmcIncrement.Response.Header.TrustedHeader.Command.IsResponse));
    DEBUG ((DEBUG_INFO, "  Status     = 0x%02x\n", (UINT8)RpmcIncrement.Response.Header.Status));
    return Status;
  }

  ///
  /// Check the Trusted Header hash
  ///
  IsVerificationSuccessful = FALSE;
  Status = mHeciTrustedChannelProtocol->VerifyTrustedHeader (
                                          (VOID *) &RpmcIncrement.Response,
                                          RespLength,
                                          &IsVerificationSuccessful
                                          );
  if (EFI_ERROR (Status) || (IsVerificationSuccessful == FALSE)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Trusted Header hash failed\n"));
    DEBUG ((DEBUG_INFO, "  Status                   = %r\n", Status));
    DEBUG ((DEBUG_INFO, "  IsVerificationSuccessful = 0x%X\n", IsVerificationSuccessful));
    return EFI_DEVICE_ERROR;
  }

  return Status;
}

/**
  Requests count from monotonic counter 0.

 @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableRequestMonotonicCounter0 (
  OUT UINT32  *CounterValue
  )
{
  return (RequestMonotonicCounter (RPMC_COUNTER_0, CounterValue));
}

/**
  Requests count from monotonic counter 1.

 @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableRequestMonotonicCounter1 (
  OUT UINT32  *CounterValue
  )
{
  return (RequestMonotonicCounter (RPMC_COUNTER_1, CounterValue));;
}

/**
  Increments monotonic counter 0 in the SPI flash device by 1.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableIncrementMonotonicCounter0 (
  VOID
  )
{
  return (IncrementMonotonicCounter (RPMC_COUNTER_0));
}

/**
  Increments monotonic counter 1 in the SPI flash device by 1.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
EFIAPI
ProtectedVariableIncrementMonotonicCounter1 (
  VOID
  )
{
  return (IncrementMonotonicCounter (RPMC_COUNTER_1));
}

/**
  This constructor performs initialization for RPMC lib.

  @retval EFI_SUCCESS      The constructor completed successfully.
**/
EFI_STATUS
EFIAPI
SmmRpmcLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS       Status;

  DEBUG ((DEBUG_INFO, "[RPMC] %a()\n", __FUNCTION__));

  ///
  /// Check if Rpmc is supported in hardware or not.
  ///
  if (!PcdGetBool (PcdRpmcSupported)) {
    mRpmcSupported = FALSE;
    DEBUG ((DEBUG_INFO, "[RPMC] GetRpmcStatus failed.  RPMC is not supported\n"));
    return EFI_UNSUPPORTED;
  }

  ///
  /// Locate TrustedChannelProtocol
  ///
  Status = gSmst->SmmLocateProtocol (
                    &gHeciTrustedChannelProtocolGuid,
                    NULL,
                    (VOID **) &mHeciTrustedChannelProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Unable to locate Trusted Channel Protocol\n"));
    return Status;
  }

  ///
  /// Locate Heci protocol
  ///
  Status = gSmst->SmmLocateProtocol (
                    &gHeciControlSmmProtocolGuid,
                    NULL,
                    (VOID **) &mHeciProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[RPMC] Unable to locate Heci Protocol\n"));
    return Status;
  }

  mRpmcSupported = TRUE;
  DEBUG ((DEBUG_INFO, "[RPMC] RPMC SMM is supported\n"));

  return Status;
}
