/** @file
  File contains all of the messages for communication between MEBx and AMT FW.

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

#include <PiDxe.h>
#include <Protocol/HeciFlowProtocol.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/AmthiMsgsLib.h>

/**
  This function prepares an AMTHI message to be sent through HECI Interface to AMT
  Client. Afterwards it uses Heci Flow Portocol to send and receive a response.
  The response is validated by the function.

  @param[in, out] MessageBuffer      On entry - body of the  message to be sent
                                     On exit  - response to the sent message
  @param[in]      Cmd                AMTHI Command
  @param[in]      MessageBuffer      AMTHI Request Message length
  @param[in]      MessageBuffer      AMTHI Response Message length

  @retval EFI_SUCCESS                Message was sent and response was received
  @retval EFI_BUFFER_TOO_SMALL       Message was sent and a response arrived but it was too big for provided buffer
  @retval EFI_TIMEOUT                No response received
  @retval EFI_PROTOCOL_ERROR         Internal HECI error or missing flow control token
  @retval EFI_ABORTED                Response message error
**/
STATIC
EFI_STATUS
AmthiSubmitRequest (
  IN OUT VOID   *MessageBuffer,
  IN     UINT32 Cmd,
  IN     UINT32 MsgLength,
  IN     UINT32 RespLength
  )
{
  AMTHI_HEADER       *Msg;
  AMTHI_RESP_HEADER  *Resp;
  EFI_STATUS         Status;
  HECI_FLOW_PROTOCOL *HeciFlow;
  UINT32             TrueRespLength;

  Status = gBS->LocateProtocol (&gHeciFlowProtocolGuid, NULL, (VOID **) &(HeciFlow));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "HECI FLOW protocol missing\n"));
    return Status;
  }

  Msg                       = (AMTHI_HEADER*) MessageBuffer;
  Resp                      = (AMTHI_RESP_HEADER*) MessageBuffer;
  Msg->Version.MajorVersion = AMTHI_VERSION_MAJOR;
  Msg->Version.MinorVersion = AMTHI_VERSION_MINOR;
  Msg->Reserved             = 0;
  Msg->Command              = Cmd;
  Msg->Length               = AMTHI_MESSAGE_BYTE_COUNT_MAP (MsgLength);
  TrueRespLength            = RespLength;

  Status = HeciFlow->SendAndReceive (HECI1, MessageBuffer, MsgLength, &TrueRespLength, &gAmtHeciAmthiGuid);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  if ((Resp->Command != (Cmd | AMTHI_COMMAND_IS_RESPONSE)) || Resp->Status || (Resp->Length > AMTHI_MESSAGE_BYTE_COUNT_MAP(RespLength))) {
    Status = EFI_ABORTED;
  }
  return Status;
}

/**
  This message is sent by the BIOS to retrieve the AMTHI Interface version.

  @param[out] AmthiInterfaceVersion  Amthi Interface Version used by the AMT client

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetInterfaceVersion (
  OUT AMTHI_VERSION                  *AmthiInterfaceVersion
  )
{
  EFI_STATUS                         Status;
  CFG_GET_INTERFACE_VERSION_RESPONSE ResponseBuffer;

  if (AmthiInterfaceVersion == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //Verify we support the correct interface version. This function also serves
  //the purpose of verifying when the AMT FW is ready to process commands.
  Status = AmthiSubmitRequest (
             &ResponseBuffer,
             AmthiCfgGetInterfaceVersion,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_INTERFACE_VERSION_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CopyMem (AmthiInterfaceVersion, &ResponseBuffer.Header.Version, sizeof (AMTHI_VERSION));
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current IPV4 TCPIP Parameters.

  @param[out] Ipv4Cfg                Current IPV4 TCPIP Parameters

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetIpv4Params (
  OUT IMB_TCPIP_PARAMS *Ipv4Cfg
  )
{
  EFI_STATUS                      Status;
  CFG_GET_TCPIP_PARAMETERS_BUFFER GetIpv4Params;

  if (Ipv4Cfg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetIpv4Params,
             AmthiCfgGetTcpipParameters,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_TCPIP_PARAMETERS_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CopyMem ((UINT8 *) Ipv4Cfg, (UINT8 *) &GetIpv4Params.Response.TcpipParams, sizeof (IMB_TCPIP_PARAMS));
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current IPV4 TCPIP Parameters.

  @param[in] NewIpv4Cfg              New IPV4 TCPIP Parameters

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetIpv4Params (
  IN IMB_TCPIP_PARAMS *NewIpv4Cfg
  )
{
  CFG_SET_TCPIP_PARAMETERS SetTcpIpParameters;

  if (NewIpv4Cfg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem ((UINT8*) &SetTcpIpParameters.TcpipParams, (UINT8*) NewIpv4Cfg, sizeof (IMB_TCPIP_PARAMS));

  return AmthiSubmitRequest (
           &SetTcpIpParameters,
           AmthiCfgSetTcpipParameters,
           sizeof (CFG_SET_TCPIP_PARAMETERS) - sizeof (UINT16) + NewIpv4Cfg->DomainName.Length,
           sizeof (CFG_SET_TCPIP_PARAMETERS_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to request full AMT Unprovision.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiUnprovision (
  VOID
  )
{
  CFG_UNPROVISION_REQUEST   Unprovision;

  Unprovision.Mode = ProvisioningModeEnterprise;  //Default Enterprise Mode

  return AmthiSubmitRequest (
           &Unprovision,
           AmthiCfgUnprovision,
           sizeof (CFG_UNPROVISION_REQUEST),
           sizeof (CFG_UNPROVISION_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to get current provisioning state.

  @param[out] ProvisioningState      Current provisioning status:
                                       @see PT_PROVISIONING_STATE

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetProvisioningState (
  OUT UINT32 *ProvisioningState
  )
{
  EFI_STATUS                          Status;
  CFG_GET_PROVISIONING_STATE_RESPONSE GetProvisionStatus;

  if (ProvisioningState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetProvisionStatus,
             AmthiCfgGetProvisioningState,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_PROVISIONING_STATE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *ProvisioningState = GetProvisionStatus.ProvisioningState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current SOL and Storage Redirection state.

  @param[in] NewSolStatus            New SOL state
  @param[in] NewStorageRedirStatus   New Storage Redirection state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetSolStorageRedir (
  IN UINT16 NewSolStatus,
  IN UINT16 NewStorageRedirStatus
  )
{
  CFG_SET_SOL_STORAGE_REDIR_STATE_REQUEST   SetSolStorageRedir;

  SetSolStorageRedir.SolEnable          = NewSolStatus;
  SetSolStorageRedir.StorageRedirEnable = NewStorageRedirStatus;

  return AmthiSubmitRequest (
           &SetSolStorageRedir,
           AmthiCfgSetSolStorageRedirStatus,
           sizeof (CFG_SET_SOL_STORAGE_REDIR_STATE_REQUEST),
           sizeof (CFG_SET_SOL_STORAGE_REDIR_STATE_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current EoP state.

  @param[out] EopState               Current End of Post State:
                                       @see CFG_EOP_STATE

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetEndOfPostState (
  OUT CFG_EOP_STATE *EopState
  )
{
  EFI_STATUS               Status;
  CFG_GET_EOP_STATE_BUFFER GetEndOfPostState;

  if (EopState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetEndOfPostState,
             AmthiCfgGetEndOfPostState,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_EOP_STATE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *EopState = GetEndOfPostState.Response.EopState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to request partial AMT Unprovision.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiPartialUnprovision (
  VOID
)
{
  CFG_PARTIAL_UNPROVISION_REQUEST PartialUnprovision;

  return AmthiSubmitRequest (
           &PartialUnprovision,
           AmthiCfgPartialUnprovision,
           sizeof (CFG_PARTIAL_UNPROVISION_REQUEST),
           sizeof (CFG_PARTIAL_UNPROVISION_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to query CSE for list of available Hashed Certificates.

  @param[out] Entries                Number of available Hashed Certificates entries
  @param[out] Handles                Array with available Hashed Certificates list

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiEnumerateHashHandles (
  OUT UINT32  *Entries,
  OUT UINT32  *Handles
  )
{
  EFI_STATUS                          Status;
  CFG_ENUMERATE_HASH_HANDLES_RESPONSE EnumerateHashHandles;

  if (Entries == NULL || Handles == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &EnumerateHashHandles,
             AmthiCfgEnumerateHashHandles,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_ENUMERATE_HASH_HANDLES_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *Entries = EnumerateHashHandles.EntriesCount;
    CopyMem (Handles, &EnumerateHashHandles.Handles, *Entries * sizeof (UINT32));
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve information for a particular Hash Entry.

  @param[in]  Handle                 Handle to Hash to get information for
  @param[out] CertHashEntry          Pointer to the structure containing info regarding
                                     Hashed Certificate defined by given handle
                                       @see CERT_HASH_ENTRY

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetCertificateHashEntry (
  IN UINT32           Handle,
  OUT CERT_HASH_ENTRY *CertHashEntry
  )
{
  EFI_STATUS                    Status;
  CFG_GET_CERTHASH_ENTRY_BUFFER GetCertificateHashEntry;

  if (CertHashEntry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GetCertificateHashEntry.Request.Handle = Handle;

  Status = AmthiSubmitRequest (
             &GetCertificateHashEntry,
             AmthiCfgGetCertificateHashEntry,
             sizeof (CFG_GET_CERTHASH_ENTRY_REQUEST),
             sizeof (CFG_GET_CERTHASH_ENTRY_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CertHashEntry->CertDefault = GetCertificateHashEntry.Response.CertHashEntry.CertDefault;
    CertHashEntry->CertActive = GetCertificateHashEntry.Response.CertHashEntry.CertActive;
    CopyMem (
      &CertHashEntry->CertHash,
      GetCertificateHashEntry.Response.CertHashEntry.CertHash,
      sizeof (GetCertificateHashEntry.Response.CertHashEntry.CertHash)
      );
    CopyMem (
      (CHAR8*) &CertHashEntry->CertName.Buffer,
      (CHAR8*) &GetCertificateHashEntry.Response.CertHashEntry.CertName.Buffer,
      GetCertificateHashEntry.Response.CertHashEntry.CertName.Length
      );
    CertHashEntry->CertName.Length = GetCertificateHashEntry.Response.CertHashEntry.CertName.Length;
    CertHashEntry->HashAlgorithm = GetCertificateHashEntry.Response.CertHashEntry.HashAlgorithm;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current PKI FQDN DNS Suffix.

  @attention Fuction is not allowed in POST_PROVISION State

  @param[in] NewPkiDns               New Fqdn Pki Dns suffix

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetPkiFqdnSuffix (
  IN FQDN_SUFFIX_ANSI_STRING *NewPkiDns
  )
{
  CFG_SET_PKI_FQDN_SUFFIX_REQUEST SetPkiFqdnSuffix;

  if (NewPkiDns == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  //get user selected new host name from setup
  SetPkiFqdnSuffix.Suffix.Length = NewPkiDns->Length;
  CopyMem ((CHAR8*) SetPkiFqdnSuffix.Suffix.Buffer, (CHAR8*) NewPkiDns->Buffer, SetPkiFqdnSuffix.Suffix.Length);

  return AmthiSubmitRequest (
           &SetPkiFqdnSuffix,
           AmthiCfgSetPkiFqdnSuffix,
           SetPkiFqdnSuffix.Suffix.Length + sizeof (SetPkiFqdnSuffix.Suffix.Length) + sizeof (AMTHI_HEADER),
           sizeof (CFG_SET_PKI_FQDN_SUFFIX_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to check whether ZTC is Enabled.

  @param[out] ZtcEnabledStatus       Current ZTC Enabled Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetZtcEnabledStatus (
  OUT UINT32 *ZtcEnabledStatus
  )
{
  EFI_STATUS                        Status;
  CFG_GET_ZTC_ENABLED_STATUS_BUFFER GetZtcEnabledStatus;

  if (ZtcEnabledStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetZtcEnabledStatus,
             AmthiCfgGetZtcEnabledStatus,
             sizeof (CFG_GET_ZTC_ENABLED_STATUS_REQUEST),
             sizeof (CFG_GET_ZTC_ENABLED_STATUS_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *ZtcEnabledStatus = GetZtcEnabledStatus.Response.Enable;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current AMT Idle Timeout.

  @param[out] AmtIdleTimeout         Current AMT Idle Timeout

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetIdleTimeout (
  OUT UINT16 *AmtIdleTimeout
  )
{
  EFI_STATUS                     Status;
  CFG_GET_IDLE_TIMEOUT_RESPONSE  GetAmtIdleTimeout;

  if (AmtIdleTimeout == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetAmtIdleTimeout,
             AmthiCfgGetIdleTimeout,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_IDLE_TIMEOUT_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *AmtIdleTimeout = (UINT16)GetAmtIdleTimeout.IdleTimeout;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current AMT Idle Timeout.

  @param[in] AmtIdleTimeout          New AMT Idle Timeout

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetIdleTimeout (
  IN UINT16 AmtIdleTimeout
  )
{
  CFG_SET_IDLE_TIMEOUT_REQUEST  SetIdleTimeout;

  SetIdleTimeout.IdleTimeout = AmtIdleTimeout;

  return AmthiSubmitRequest (
           &SetIdleTimeout,
           AmthiCfgSetIdleTimeout,
           sizeof (CFG_SET_IDLE_TIMEOUT_REQUEST),
           sizeof (CFG_SET_IDLE_TIMEOUT_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current PKI DNS Data.

  @param[out] CurrentPkiDns          Current PKI DNS Data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetPkiFqdnSuffix (
  OUT FQDN_SUFFIX_ANSI_STRING *CurrentPkiDns
  )
{
  EFI_STATUS                       Status;
  CFG_GET_PKI_FQDN_SUFFIX_RESPONSE GetPkiFqdnSuffix;

  if (CurrentPkiDns == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetPkiFqdnSuffix,
             AmthiCfgGetPkiFqdnSuffix,
             sizeof (CFG_GET_PKI_FQDN_SUFFIX_REQUEST),
             sizeof (CFG_GET_PKI_FQDN_SUFFIX_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    CurrentPkiDns->Length = 0;
    return Status;
  }

  CopyMem (CurrentPkiDns, &GetPkiFqdnSuffix.Suffix, sizeof (FQDN_SUFFIX_ANSI_STRING));

  return Status;
}

/**
  This message is sent by the BIOS to update Certificate Hash State.

  @param[in] Handle                  Handle to Hash to Update State for
  @param[in] IsActive                New User Defined State for Certificate Hash State
                                       (Active/Inactive)

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetCertificateHashState (
  IN UINT32 *Handle,
  IN UINT32 IsActive
  )
{
  CFG_SET_CERTHASH_STATE_REQUEST SetCertificateHashState;

  if (Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetCertificateHashState.Handle   = *Handle;
  SetCertificateHashState.IsActive = IsActive;

  return AmthiSubmitRequest (
           &SetCertificateHashState,
           AmthiCfgSetCertificateHashState,
           sizeof (CFG_SET_CERTHASH_STATE_REQUEST),
           sizeof (CFG_SET_CERTHASH_STATE_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to add new Customized Certificate Hash Entry to the AMT database.

  @attention Available in in-boot mode only when in AMT 2.0 provisioning mode.

  @param[in] IsActive                Default User Defined State for Certificate Hash State
                                        (Active/Not Active)
  @param[in] CertificateHash         New Certificate Hash
  @param[in] HashAlgorithm           Hash algorithm for new Custom Hash
  @param[in] Name                    New Custom Hash name

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiAddCustomizedCertificateHashEntry (
  IN UINT32           *IsActive,
  IN UINT8            *CertificateHash,
  IN UINT8            *HashAlgorithm,
  IN IMB_ANSI_STRING  *Name
  )
{
  CFG_ADD_CERTHASH_ENTRY_BUFFER AddCustomizedCertificateHashEntry;
  UINT32                        HashDataSize;

  if (IsActive == NULL || HashAlgorithm == NULL || CertificateHash == NULL || Name == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  AddCustomizedCertificateHashEntry.Request.IsActive = *IsActive;

  HashDataSize = 0;
  //Copy based on algorithm to avoid trailing garbage in the hash data
  switch (*HashAlgorithm) {
    case CertHashAlgorithmMd5:
      HashDataSize = 16;
      break;

    case CertHashAlgorithmSha1:
      HashDataSize = 20;
      break;

    case CertHashAlgorithmSha256:
      HashDataSize = 32;
      break;

    case CertHashAlgorithmSha384:
      HashDataSize = 48;
      break;
  }

  CopyMem (AddCustomizedCertificateHashEntry.Request.CertificateHash, CertificateHash, HashDataSize);

  AddCustomizedCertificateHashEntry.Request.HashAlgorithm = *HashAlgorithm;
  AddCustomizedCertificateHashEntry.Request.Name.Length   = Name->Length;

  CopyMem (&AddCustomizedCertificateHashEntry.Request.Name.Buffer, Name->Buffer, AddCustomizedCertificateHashEntry.Request.Name.Length);

  return AmthiSubmitRequest (
           &AddCustomizedCertificateHashEntry,
           AmthiCfgAddCustomizedCertificateHashEntry,
           sizeof (CFG_ADD_CERTHASH_ENTRY_REQUEST),
           sizeof (CFG_ADD_CERTHASH_ENTRY_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to request deletion of Customized Certificate Hash Entry
  from the AMT database.

  @param[in] Handle                  Handle to Hash to delete

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiDeleteCustomizedCertificateHashEntry (
  IN UINT32 *Handle
  )
{
  CFG_DELETE_CERTHASH_ENTRY_REQUEST DeleteCustomizedCertificateHashEntry;

  if (Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  DeleteCustomizedCertificateHashEntry.Handle = *Handle;

  return AmthiSubmitRequest (
           &DeleteCustomizedCertificateHashEntry,
           AmthiCfgDeleteCustomizedCertificateHashEntry,
           sizeof (CFG_DELETE_CERTHASH_ENTRY_REQUEST),
           sizeof (CFG_DELETE_CERTHASH_ENTRY_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to update Zero Touch Enabled State.

  @param[in] NewZtcStatus            New ZTC Enabled Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetZeroTouchEnabled (
  IN UINT32 NewZtcStatus
  )
{
  CFG_SET_ZTC_MODE_REQUEST SetZeroTouchEnabled;

  SetZeroTouchEnabled.Mode = NewZtcStatus;

  return AmthiSubmitRequest (
           &SetZeroTouchEnabled,
           AmthiCfgSetZeroTouchEnabled,
           sizeof (CFG_SET_ZTC_MODE_REQUEST),
           sizeof (CFG_SET_ZTC_MODE_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to request a User Connection for CIRA.

  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiOpenUserInitiatedConnection (
  OUT UINT32 *AmtStatus
  )
{
  EFI_STATUS                                   Status;
  CFG_OPEN_USER_INITIATED_CONNECTION_RESPONSE  AmthiOpenUserInitiatedConnection;

  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &AmthiOpenUserInitiatedConnection,
             AmthiCfgOpenUserInitiatedConnection,
             sizeof (CFG_OPEN_USER_INITIATED_CONNECTION_REQUEST),
             sizeof (CFG_OPEN_USER_INITIATED_CONNECTION_RESPONSE)
             );

  *AmtStatus = AmthiOpenUserInitiatedConnection.Status;

  return Status;
}

/**
  This message is sent by the BIOS to close a User Connection for CIRA.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiCloseUserInitiatedConnection (
  VOID
  )
{
  CFG_CLOSE_USER_INITIATED_CONNECTION_REQUEST CloseUserInitiatedConnection;

  return AmthiSubmitRequest (
           &CloseUserInitiatedConnection,
           AmthiCfgCloseUserInitiatedConnection,
           sizeof (CFG_CLOSE_USER_INITIATED_CONNECTION_REQUEST),
           sizeof (CFG_CLOSE_USER_INITIATED_CONNECTION_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to query the status of CIRA Connection.

  @param[out] ConnectionData         Connection Data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge or Fqdn too long
**/
EFI_STATUS
AmthiGetConnectionStatus (
  OUT REMOTE_ACCESS_CONNECTION_STRUCT *ConnectionData
  )
{
  EFI_STATUS                                        Status;
  CFG_GET_REMOTE_ACCESS_CONNECTION_STATUS_RESPONSE  GetConnectionStatus;

  if (ConnectionData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetConnectionStatus,
             AmthiCfgGetRemoteAccessConnectionStatus,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_REMOTE_ACCESS_CONNECTION_STATUS_RESPONSE)
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GetConnectionStatus.MpsHostname.Length >= MAX_STRING_LENGTH_FQDN) {
    return EFI_BUFFER_TOO_SMALL;
  }

  ConnectionData->AmtNetworkConnectionStatus    = GetConnectionStatus.AmtNetworkConnectionStatus;
  ConnectionData->RemoteAccessConnectionStatus  = GetConnectionStatus.RemoteAccessConnectionStatus;
  ConnectionData->RemoteAccessConnectionTrigger = GetConnectionStatus.RemoteAccessConnectionTrigger;
  AsciiStrnCpyS (
    (CHAR8 *) &ConnectionData->MpsHostname.Buffer,
    sizeof (ConnectionData->MpsHostname.Buffer),
    (CHAR8 *) &GetConnectionStatus.MpsHostname.Buffer,
    GetConnectionStatus.MpsHostname.Length
    );
  ConnectionData->MpsHostname.Buffer[GetConnectionStatus.MpsHostname.Length] = 0;
  ConnectionData->MpsHostname.Length            = (UINT16) AsciiStrLen ((CHAR8*) &GetConnectionStatus.MpsHostname.Buffer);

  return Status;
}

/**
  This message is sent by the BIOS to obtain the LAN interface information
  for wired and wireless IPV4.

  @param[in,out] InterfaceSettings   On input - if NULL, the function will query if the
                                                interface exists
                                     On output - Current LAN Interface Data
  @param[in]     Interface           Interface type:
                                       0 - WIRED
                                       1 - WIRELESS

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetLanInterfaceSettings (
  IN OUT LAN_INTERFACE_SETTINGS *InterfaceSettings,
  IN     UINT32                 Interface
  )
{
  EFI_STATUS                            Status;
  CFG_GET_LAN_INTERFACE_SETTINGS_BUFFER GetLanInterfaceSettings;

  GetLanInterfaceSettings.Request.InterfaceIndex = Interface;

  Status = AmthiSubmitRequest (
             &GetLanInterfaceSettings,
             AmthiCfgGetLanInterfaceSettings,
             sizeof (CFG_GET_LAN_INTERFACE_SETTINGS_REQUEST),
             sizeof (CFG_GET_LAN_INTERFACE_SETTINGS_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    if (GetLanInterfaceSettings.Response.Status == AmtStatusInterfaceDoesNotExist) {
      return EFI_NO_MEDIA;
    }
    return Status;
  }

  if (InterfaceSettings == NULL) {
    return EFI_SUCCESS;
  }

  CopyMem (InterfaceSettings, &GetLanInterfaceSettings.Response.InterfaceSettings, sizeof (LAN_INTERFACE_SETTINGS));

  return Status;
}

/**
  This message is sent by the BIOS to update current MEBx password change policy.

  @param[in] PwdPolicy               New MEBx password change policy:
                                       @see CFG_MEBX_PWD_CHANGE_POLICY

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetMebxPwdPolicy (
  IN UINT32 PwdPolicy
  )
{
  CFG_SET_MEBX_PWD_CHANGE_POLICY_REQUEST   SetMebxPwdPolicy;

  SetMebxPwdPolicy.Policy = PwdPolicy;

  return AmthiSubmitRequest (
           &SetMebxPwdPolicy,
           AmthiCfgSetMebxPwdChangePolicy,
           sizeof (CFG_SET_MEBX_PWD_CHANGE_POLICY_REQUEST),
           sizeof (CFG_SET_MEBX_PWD_CHANGE_POLICY_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current MEBx password change policy.

  @param[out] PwdPolicy              Current MEBx password change policy:
                                       @see CFG_MEBX_PWD_CHANGE_POLICY

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetMebxPwdPolicy (
  OUT UINT32 *PwdPolicy
  )
{
  EFI_STATUS                              Status;
  CFG_GET_MEBX_PWD_CHANGE_POLICY_RESPONSE GetMebxPwdPolicy;

  if (PwdPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetMebxPwdPolicy,
             AmthiCfgGetMebxPwdChangePolicy,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_MEBX_PWD_CHANGE_POLICY_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *PwdPolicy = GetMebxPwdPolicy.Policy;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current AMT Config Server data.

  @param[out] ServerPort             Current Configuration Server Port
  @param[out] ServerAddr             Current Configuration Server Address
  @param[out] Fqdn                   Current Fqdn Value

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetConfigServerData (
  OUT UINT16           *ServerPort,
  OUT UINT8            *ServerAddr,
  OUT FQDN_ANSI_STRING *Fqdn
  )
{
  EFI_STATUS                          Status;
  CFG_GET_CONFIG_SERVER_DATA_RESPONSE GetConfigServerData;

  if (ServerPort == NULL || ServerAddr == NULL || Fqdn == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetConfigServerData,
             AmthiCfgGetConfigServerData,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_CONFIG_SERVER_DATA_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *ServerPort = GetConfigServerData.ConfigServerData.ServerPort;
    CopyMem (ServerAddr, (UINT8*) GetConfigServerData.ConfigServerData.ServerAddr.Address, CFG_IPV6_ADDR_LEN_MAX);
    Fqdn->Length = GetConfigServerData.ConfigServerData.Fqdn.Length;
    CopyMem ((UINT8*) Fqdn->Buffer, (UINT8*) GetConfigServerData.ConfigServerData.Fqdn.Buffer, GetConfigServerData.ConfigServerData.Fqdn.Length);
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current AMT Config Server data.

  @param[in] ServerPort              New Configuration Server Port Value
  @param[in] ServerAddr              New Configuration Server Address
  @param[in] Fqdn                    New Fqdn Value

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetConfigServerData (
  IN UINT16             ServerPort,
  IN UINT8              *ServerAddr,
  IN FQDN_ANSI_STRING   *Fqdn
  )
{
  CFG_SET_CONFIG_SERVER_DATA_BUFFER SetConfigServerData;

  if (Fqdn == NULL || ServerAddr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((UINT8*)&SetConfigServerData, sizeof (CFG_SET_CONFIG_SERVER_DATA_BUFFER));

  SetConfigServerData.Request.ConfigServerData.ServerPort = ServerPort;
  SetConfigServerData.Request.ConfigServerData.Fqdn.Length = Fqdn->Length;
  CopyMem (&SetConfigServerData.Request.ConfigServerData.ServerAddr.Address, ServerAddr, sizeof (CFG_IP_ADDR));
  CopyMem (&SetConfigServerData.Request.ConfigServerData.Fqdn.Buffer, (UINT8*) Fqdn->Buffer, Fqdn->Length);

  return AmthiSubmitRequest (
           &SetConfigServerData,
           AmthiCfgSetConfigServerData,
           sizeof (CFG_SET_CONFIG_SERVER_DATA_REQUEST),
           sizeof (CFG_SET_CONFIG_SERVER_DATA_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current AMT FW Audit Record
  which Holds Info about Last Provisioning Attempt.

  @param[out] Audit                  AMT's Current Audit Record

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetMeSetupAuditRecord (
  OUT ME_ADMIN_AUDIT_RECORD *Audit
  )
{
  EFI_STATUS                     Status;
  CFG_GET_AUDIT_RECORD_RESPONSE  GetMeSetupAuditRecord;

  if (Audit == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetMeSetupAuditRecord,
             AmthiCfgGetMeSetupAuditRecord,
             sizeof (CFG_GET_AUDIT_RECORD_REQUEST),
             sizeof (CFG_GET_AUDIT_RECORD_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    if ((GetMeSetupAuditRecord.Status == AmtStatusNotReady) || (GetMeSetupAuditRecord.Status == AmtStatusDataMissing)) {
      //return SUCCESS if audit record is not ready, but there is no error.
      ZeroMem (Audit, sizeof (ME_ADMIN_AUDIT_RECORD));
      return EFI_SUCCESS;
    }
    return Status;
  }

  //Copy all data except the FQDN string buffer.
  CopyMem (Audit, &GetMeSetupAuditRecord.AuditRecord, sizeof (ME_ADMIN_AUDIT_RECORD) - sizeof (Audit->ProvServerFqdn));

  //Now copy the FQDN data
  Audit->ProvServerFqdn.Length = MIN (GetMeSetupAuditRecord.AuditRecord.ProvServerFqdn.Length, MAX_ASCII_STRING);
  CopyMem (
    Audit->ProvServerFqdn.Buffer,
    GetMeSetupAuditRecord.AuditRecord.ProvServerFqdn.Buffer,
    MIN (GetMeSetupAuditRecord.AuditRecord.ProvServerFqdn.Length,
    MAX_ASCII_STRING)
    );

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current AMT IPV6 LAN Settings.

  @param[in]  ConnectionType         Interface type:
                                       0 - WIRED
                                       1 - WIRELESS
  @param[out] Ipv6LanIntfStrc        Current LAN Settings Structure

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
                                     or too many IPV6 addresses
**/
EFI_STATUS
AmthiCfgGetIPv6LanIntStatus (
  IN UINT32                    ConnectionType,
  OUT IPV6_LAN_INTERACE_STRUC  *Ipv6LanIntfStrc
  )
{
  EFI_STATUS                                   Status;
  CFG_GET_IPV6_LAN_INTERFACE_STATUS_BUFFER     GetIPv6LanIntStatus;

  if (Ipv6LanIntfStrc == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GetIPv6LanIntStatus.Request.InterfaceIndex = ConnectionType;

  Status = AmthiSubmitRequest (
             &GetIPv6LanIntStatus,
             AmthiCfgGetIpv6LanInterfaceStatus,
             sizeof (CFG_GET_IPV6_LAN_INTERFACE_STATUS_REQUEST),
             sizeof (CFG_GET_IPV6_LAN_INTERFACE_STATUS_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    if ((GetIPv6LanIntStatus.Response.Status == AmtStatusInterfaceDoesNotExist) ||
        (GetIPv6LanIntStatus.Response.Status == AmtStatusIpv6InterfaceDisabled) ||
        (GetIPv6LanIntStatus.Response.Status == AmtStatusUnsupported)) {
      return EFI_NOT_STARTED;
    }
    return Status;
  }

  if (GetIPv6LanIntStatus.Response.Ipv6LanIntfStrc.NumOfAddr > MAX_IPV6_ADDRESS_NUM) {
    return EFI_BUFFER_TOO_SMALL;
  }

  // Copy Ipv6DefaultRouter + PrimaryDns + SecondaryDns = 3 * sizeof(CFG_IP_ADDR)
  CopyMem (
    &Ipv6LanIntfStrc->Ipv6DefaultRouter,
    &GetIPv6LanIntStatus.Response.Ipv6LanIntfStrc.Ipv6DefaultRouter,
    3 * sizeof (CFG_IP_ADDR)
    );

  Ipv6LanIntfStrc->NumOfAddr = GetIPv6LanIntStatus.Response.Ipv6LanIntfStrc.NumOfAddr;

  CopyMem (
    &Ipv6LanIntfStrc->Ipv6Addresses,
    &GetIPv6LanIntStatus.Response.Ipv6LanIntfStrc.Ipv6Addresses,
    sizeof (CFG_IPV6_ADDR_INFO) * GetIPv6LanIntStatus.Response.Ipv6LanIntfStrc.NumOfAddr
    );


  return Status;
}

/**
  This message is sent by the BIOS to update current AMT IPV6 LAN Settings.

  @param[in] ConnectionType          Interface type:
                                       0 - WIRED
                                       1 - WIRELESS
  @param[in] Ipv6Settings            New LAN Settings Structure

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetIpv6LanInterfaceSettings (
  IN UINT8                            ConnectionType,
  IN CFG_IPV6_LAN_INTERFACE_SETTINGS  *Ipv6Settings
  )
{
  EFI_STATUS                                 Status;
  CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_BUFFER SetIpv6LanInterfaceSettings;

  if (Ipv6Settings == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetIpv6LanInterfaceSettings.Request.InterfaceIndex = (UINT32) ConnectionType;
  CopyMem (
    (UINT8 *) &SetIpv6LanInterfaceSettings.Request.Ipv6Settings,
    (UINT8 *) Ipv6Settings,
    sizeof(CFG_IPV6_LAN_INTERFACE_SETTINGS)
    );

  Status = AmthiSubmitRequest (
             &SetIpv6LanInterfaceSettings,
             AmthiCfgSetIpv6LanInterfaceSettings,
             sizeof (CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_REQUEST),
             sizeof (CFG_SET_IPV6_LAN_INTERFACE_SETTINGS_RESPONSE)
             );

  if (SetIpv6LanInterfaceSettings.Response.Status == AmtStatusInterfaceDoesNotExist) {
    return EFI_NO_MEDIA;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current FQDN Data.

  @param[out] FqdnData               Current FQDN data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetFqdn (
  OUT FQDN_DATA *FqdnData
  )
{
  EFI_STATUS          Status;
  CFG_GET_FQDN_BUFFER GetFqdn;

  if (FqdnData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&FqdnData->Fqdn, sizeof (FQDN_ANSI_STRING));

  Status = AmthiSubmitRequest (
             &GetFqdn,
             AmthiCfgGetFqdn,
             sizeof (CFG_GET_FQDN_REQUEST),
             sizeof (CFG_GET_FQDN_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CopyMem (FqdnData, &GetFqdn.Response.FqdnData, sizeof (FQDN_DATA));
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current FQDN Data.

  @param[in] FqdnData                New FQDN data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetFqdn (
  IN FQDN_DATA *FqdnData
  )
{
  CFG_SET_FQDN_REQUEST SetFqdn;

  if (FqdnData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((UINT8*)&SetFqdn, sizeof(CFG_SET_FQDN_REQUEST));

  CopyMem (&SetFqdn.FqdnData, FqdnData, sizeof (FQDN_DATA));

  SetFqdn.FqdnData.Fqdn.Length = (UINT16) AsciiStrLen ((CHAR8 *) &FqdnData->Fqdn.Buffer);

  return AmthiSubmitRequest (
           &SetFqdn,
           AmthiCfgSetFqdn,
           sizeof (CFG_SET_FQDN_REQUEST),
           sizeof (CFG_SET_FQDN_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current Redirection features state.

  @param[out] FeaturesState          Current Redirection features state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetRedirectionState (
  OUT REDIRECTION_FEATURES_STATE *FeaturesState
  )
{
  EFI_STATUS                         Status;
  CFG_GET_REDIRECTION_STATE_RESPONSE GetRedirectionState;

  if (FeaturesState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetRedirectionState,
             AmthiCfgGetRedirectionState,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_REDIRECTION_STATE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CopyMem (FeaturesState, &GetRedirectionState.RedirectionFeatureStates, sizeof (REDIRECTION_FEATURES_STATE));
  }

  return Status;
}

/**
  This message is sent by the BIOS to request FW to complete configuration
  and transition to the post provisioning state.

  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiCompleteConfigurationRequest (
  OUT UINT32 *AmtStatus
  )
{
  EFI_STATUS                          Status;
  CFG_COMPLETE_CONFIGURATION_RESPONSE CompleteConfigurationResponse;

  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &CompleteConfigurationResponse,
             AmthiCfgCompleteConfiguration,
             sizeof (CFG_COMPLETE_CONFIGURATION_REQUEST),
             sizeof (CFG_COMPLETE_CONFIGURATION_RESPONSE)
             );

  *AmtStatus = CompleteConfigurationResponse.Status;

  return Status;
}

/**
  This message is sent by the BIOS to request FW to halt configuration.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiStopConfiguration (
  VOID
  )
{
  CFG_STOP_CONFIGURATION_REQUEST StopConfiguration;

  return AmthiSubmitRequest (
           &StopConfiguration,
           AmthiCfgStopConfiguration,
           sizeof (CFG_STOP_CONFIGURATION_REQUEST),
           sizeof (CFG_STOP_CONFIGURATION_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to request FW Start the Configuration Process

  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiStartConfigurationEx (
  OUT UINT32 *AmtStatus
  )
{
  EFI_STATUS                        Status;
  CFG_START_CONFIGURATION_EX_BUFFER StartConfigurationEx;

  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem ((UINT8*) &StartConfigurationEx, sizeof(CFG_START_CONFIGURATION_EX_REQUEST));  //clear out max possible resp in raw buffer
  StartConfigurationEx.Request.IPv6Enable = FALSE;

  Status = AmthiSubmitRequest (
             &StartConfigurationEx,
             AmthiCfgStartConfigurationEx,
             sizeof (CFG_START_CONFIGURATION_EX_REQUEST),
             sizeof (CFG_START_CONFIGURATION_EX_RESPONSE)
             );

  *AmtStatus = StartConfigurationEx.Response.Status;

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current Unprovisioning State.

  @param[out] UnprovisioningState    Current Unprovisioning State

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetUnprovisioningState (
  OUT UINT32 *UnprovisioningState
  )
{
  EFI_STATUS                            Status;
  CFG_GET_UNPROVISIONING_STATE_RESPONSE GetUnprovisioningState;

  if (UnprovisioningState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetUnprovisioningState,
             AmthiCfgGetUnprovisioningState,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_UNPROVISIONING_STATE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *UnprovisioningState = GetUnprovisioningState.UnprovisioningState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to set MEBx state and verify if KVM session
  is not active during MEBx UI.

  @param[in]  MebxState              New MEBx state
  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetMebxState (
  IN  AMT_MEBX_STATE MebxState,
  OUT UINT32         *AmtStatus
  )
{
  EFI_STATUS                Status;
  CFG_SET_MEBX_STATE_BUFFER SetMebxStateRequest;

  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  SetMebxStateRequest.Request.MebxState = MebxState;

  Status = AmthiSubmitRequest (
             &SetMebxStateRequest,
             AmthiCfgSetMebxState,
             sizeof (CFG_SET_MEBX_STATE_REQUEST),
             sizeof (CFG_SET_MEBX_STATE_RESPONSE)
             );

  *AmtStatus = SetMebxStateRequest.Response.Status;

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current AMT Control Mode.

  @param[out] ControlMode            Current Control Mode:
                                       0 - None
                                       1 - Client
                                       2 - Admin

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetControlMode (
  OUT UINT32 *ControlMode
  )
{
  EFI_STATUS                    Status;
  CFG_GET_CONTROL_MODE_RESPONSE GetControlMode;

  if (ControlMode == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetControlMode,
             AmthiCfgGetControlMode,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_CONTROL_MODE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *ControlMode = GetControlMode.ControlMode;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current state of User Consent for AMT Features:
  KVM / USB-R and Boot Parameters.

  @param[in] OptInState              Current OptInState
                                       0          - None
                                       1          - KVM
                                       0xFFFFFFFF - All
  @param[in] RemoteOptInState        Enable / Disable Remote OptIn:
                                       0 - Enabled
                                       1 - Disabled

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetOptInState (
  IN UINT32 OptInState,
  IN UINT32 RemoteOptInState
  )
{
  CFG_SET_OPTIN_STATE_REQUEST  SetOptInState;

  SetOptInState.OptInState       = OptInState;
  SetOptInState.RemoteOptInState = RemoteOptInState;

  return AmthiSubmitRequest (
           &SetOptInState,
           AmthiCfgSetOptInState,
           sizeof (CFG_SET_OPTIN_STATE_REQUEST),
           sizeof (CFG_SET_OPTIN_STATE_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current state of User Consent for AMT Features:
  KVM / USB-R and Boot Parameters.

  @param[out] OptInState             Current OptInState
                                       0          - None
                                       1          - KVM
                                       0xFFFFFFFF - All
  @param[out] RemoteOptInState       Remote OptIn State:
                                       0 - Enabled
                                       1 - Disabled

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetOptInState (
  OUT UINT32 *OptInState,
  OUT UINT32 *RemoteOptInState
  )
{
  EFI_STATUS                    Status;
  CFG_GET_OPTIN_STATE_RESPONSE  GetOptInState;

  if ((OptInState == NULL) || (RemoteOptInState == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetOptInState,
             AmthiCfgGetOptInState,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_OPTIN_STATE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *OptInState       = GetOptInState.OptInState;
    *RemoteOptInState = GetOptInState.RemoteOptInState;
  }

  return Status;
}

/**
  This message is sent by the BIOS to update current state of KVM Feature.

  @param[in] KvmState                New KVM State

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetKvmEnabled (
  IN UINT32 KvmState
  )
{
  CFG_SET_KVM_ENABLED_REQUEST SetKvmEnabled;

  SetKvmEnabled.Enabled = KvmState;

  return AmthiSubmitRequest (
           &SetKvmEnabled,
           AmthiCfgSetKvmEnabled,
           sizeof (CFG_SET_KVM_ENABLED_REQUEST),
           sizeof (CFG_SET_KVM_ENABLED_RESPONSE)
           );
}

/**
  This message is sent by the BIOS to retrieve current state of KVM Feature.

  @param[out] KvmState               Current KVM State

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetKvmEnabled (
  OUT UINT8 *KvmState
  )
{
  EFI_STATUS                   Status;
  CFG_GET_KVM_ENABLED_RESPONSE GetKvmEnabled;

  if (KvmState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetKvmEnabled,
             AmthiCfgGetKvmEnabled,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_KVM_ENABLED_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *KvmState = (UINT8) GetKvmEnabled.Enabled;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve current AMT Privacy Level.

  @param[out] PrivacyLevel           Current Privacy Level
                                       1 - Default
                                       2 - Enhanced
                                       3 - Extreme

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetAmtPrivacyLevel (
  OUT UINT8 *PrivacyLevel
  )
{
  EFI_STATUS                     Status;
  CFG_GET_PRIVACY_LEVEL_RESPONSE GetAmtPrivacyLevel;

  if (PrivacyLevel == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetAmtPrivacyLevel,
             AmthiCfgGetPrivacyLevel,
             sizeof (CFG_GET_PRIVACY_LEVEL_REQUEST),
             sizeof (CFG_GET_PRIVACY_LEVEL_RESPONSE)
             );

  if (!EFI_ERROR(Status)) {
    *PrivacyLevel = (UINT8) GetAmtPrivacyLevel.PrivacyLevel;
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve availability for the OptIn options.

  @param[out] OptInAvailable         Current OptIn Availability bitmap:
                                       0x1 - None
                                       0x2 - KVM Only
                                       0x4 - All
  @param[out] RemoteOptInAvailable   Current RemoteOptIn Availability:
                                       0 - Remote OptIn State can't be changed
                                       1 - Remote OptIn State can be changed

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetOptInChangeAvailable (
  OUT UINT32 *OptInAvailable,
  OUT UINT32 *OptInRemoteAvailable
  )
{
  EFI_STATUS                               Status;
  CFG_GET_OPTIN_CHANGE_AVAILABLE_RESPONSE  GetOptInChangeAvailable;

  if ((OptInAvailable == NULL) || (OptInRemoteAvailable == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetOptInChangeAvailable,
             AmthiCfgGetOptInChangeAvailable,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_OPTIN_CHANGE_AVAILABLE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *OptInAvailable       = GetOptInChangeAvailable.OptInPolicy;
    *OptInRemoteAvailable = GetOptInChangeAvailable.OptInRemoteEnabled;
  }

  return Status;
}

/**
  This message is sent by the BIOS to request UI Action for the screen.

  @param[in]  CurrentImageId         Current display state MEBx is in
  @param[in]  UserInput              Current user input
  @param[out] NextScreenId           Next ImageId to be displayed

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_ABORTED                Error occured during UI flow - abort
**/
EFI_STATUS
AmthiGetUiAction (
  IN  AMT_IMAGE_ID CurrentImageId,
  IN  UINT32       UserInput,
  OUT AMT_IMAGE_ID *NextScreenId
  )
{
  EFI_STATUS           Status;
  CFG_UI_ACTION_BUFFER GetUiAction;

  if (NextScreenId == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GetUiAction.Request.CurrentImageId = CurrentImageId;
  GetUiAction.Request.UserInput      = UserInput;

  Status = AmthiSubmitRequest (
             &GetUiAction,
             AmthiCfgUiAction,
             sizeof (CFG_UI_ACTION_REQUEST),
             sizeof (CFG_UI_ACTION_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
     return EFI_ABORTED;
  }

  *NextScreenId = GetUiAction.Response.NextScreen;

  return Status;
}

/**
  This message is sent by the BIOS to request GFX UI Info for the image.

  @param[in]  ScreenResolution       Gives the Heigth and Width of the image
  @param[in]  ImageId                Image Id
  @param[out] ImageDataLength        Image data length
  @param[out] UpperLeftXPos          Upper left X position of the image
  @param[out] UpperLeftYPos          Upper left Y position of the image

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_ABORTED                Error occured during UI flow - abort
**/
EFI_STATUS
AmthiGetGfxUiInfo (
  IN  AMT_SCREEN_RESOLUTION ScreenResolution,
  IN  AMT_IMAGE_ID          ImageId,
  OUT UINT32                *ImageDataLength,
  OUT UINT16                *UpperLeftXPos,
  OUT UINT16                *UpperLeftYPos
  )
{
  EFI_STATUS                 Status;
  CFG_GET_GFX_UI_INFO_BUFFER GetGfxUiInfo;

  if ((ImageDataLength == NULL) || (UpperLeftXPos == NULL) || (UpperLeftYPos == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  GetGfxUiInfo.Request.ScreenResolution.Height = ScreenResolution.Height;
  GetGfxUiInfo.Request.ScreenResolution.Width  = ScreenResolution.Width;
  GetGfxUiInfo.Request.ImageId                 = ImageId;

  Status = AmthiSubmitRequest (
             &GetGfxUiInfo,
             AmthiCfgGetGfxUiInfo,
             sizeof (CFG_GET_GFX_UI_INFO_REQUEST),
             sizeof (CFG_GET_GFX_UI_INFO_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  *ImageDataLength = GetGfxUiInfo.Response.ImageDataLength;
  *UpperLeftXPos   = GetGfxUiInfo.Response.UpperLeftXPos;
  *UpperLeftYPos   = GetGfxUiInfo.Response.UpperLeftYPos;

  return Status;
}

/**
  This message is sent by the BIOS to request GFX UI fragment of the image.

  @param[in]  ImageId                Image Id
  @param[in]  ByteOffset             Offset in the image data
  @param[in]  ByteSizeReq            Requested image data length
  @param[out] ByteSizeResp           Fragment data size in bytes
  @param[out] Data                   Image data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_ABORTED                Error occured during UI flow - abort
**/
EFI_STATUS
AmthiGetGfxFragment (
  IN AMT_IMAGE_ID ImageId,
  IN UINT32       ByteOffset,
  IN UINT32       ByteSizeReq,
  OUT UINT32      *ByteSizeResp,
  OUT UINT8       *Data
  )
{
  EFI_STATUS                    Status;
  CFG_GET_GFX_FRAGMENT_BUFFER   GetGfxFragment;

  if (ByteSizeResp == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  GetGfxFragment.Request.ImageId    = ImageId;
  GetGfxFragment.Request.ByteOffset = ByteOffset;
  GetGfxFragment.Request.ByteSize   = ByteSizeReq;

  Status = AmthiSubmitRequest (
             &GetGfxFragment,
             AmthiCfgGetGfxFragment,
             sizeof (CFG_GET_GFX_FRAGMENT_REQUEST),
             sizeof (CFG_GET_GFX_FRAGMENT_RESPONSE)
             );

  if (EFI_ERROR (Status)) {
    return EFI_ABORTED;
  }

  *ByteSizeResp = GetGfxFragment.Response.ByteSize;
  ZeroMem (Data, GetGfxFragment.Response.ByteSize);
  CopyMem (Data, GetGfxFragment.Response.Data, GetGfxFragment.Response.ByteSize);

  return Status;
}

/**
  This message is sent by the BIOS to retrieve GUIDs of supported power packages.

  @param[out] NumberOfGuids          Number of supported power packages
  @param[out] Guids                  Guids of supported power packages


  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiEnumeratePowerPackages (
  OUT UINT8    *NumberOfGuids,
  OUT EFI_GUID *Guids
  )
{
  EFI_STATUS                             Status;
  CFG_ENUMERATE_POWER_PACKAGES_RESPONSE  EnumeratePowerPackages;

  if ((NumberOfGuids == NULL) || (Guids == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &EnumeratePowerPackages,
             AmthiCfgEnumeratePowerPackages,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_ENUMERATE_POWER_PACKAGES_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    *NumberOfGuids = (UINT8) EnumeratePowerPackages.NumOfGuids;
    CopyMem (Guids, &EnumeratePowerPackages.Guids, *NumberOfGuids * sizeof (EFI_GUID));
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve GUID of currently active power package.

  @param[out] Guid                   Guid of currently active power package

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetCurrentPowerPackage (
  OUT EFI_GUID *Guid
  )
{
  EFI_STATUS                             Status;
  CFG_GET_CURRENT_POWER_PACKAGE_RESPONSE GetCurrentPowerPackage;

  if (Guid == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiSubmitRequest (
             &GetCurrentPowerPackage,
             AmthiCfgGetCurrentPowerPackage,
             sizeof (AMTHI_HEADER),
             sizeof (CFG_GET_CURRENT_POWER_PACKAGE_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    CopyMem (Guid, &GetCurrentPowerPackage.Guid, sizeof (EFI_GUID));
  }

  return Status;
}

/**
  This message is sent by the BIOS to retrieve description of power package
  under a certain GUID.

  @param[in]  Guid                   Guids of power package to get the description of
  @param[out] Description            Description of power package defined by given GUID

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiGetPowerPackageInfo (
  IN EFI_GUID         Guid,
  OUT IMB_ANSI_STRING *Description
  )
{
  EFI_STATUS                        Status;
  CFG_GET_POWER_PACKAGE_INFO_BUFFER GetPowerPackageInfo;

  if (Description == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  CopyMem (&GetPowerPackageInfo.Request.Guid, &Guid, sizeof (EFI_GUID));

  Status = AmthiSubmitRequest (
             &GetPowerPackageInfo,
             AmthiCfgGetPowerPackageInfo,
             sizeof (CFG_GET_POWER_PACKAGE_INFO_REQUEST),
             sizeof (CFG_GET_POWER_PACKAGE_INFO_RESPONSE)
             );

  if (!EFI_ERROR (Status)) {
    Description->Length = GetPowerPackageInfo.Response.Length;
    CopyMem (
      Description->Buffer,
      &GetPowerPackageInfo.Response.Description,
      (GetPowerPackageInfo.Response.Length > MAX_ASCII_STRING) ? MAX_ASCII_STRING : GetPowerPackageInfo.Response.Length
      );
  }

  return Status;
}

/**
  This message is sent by the BIOS to updated currently active power package.

  @param[in] Guid                    Guid of power package to activate

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiSetPowerPackage (
  IN EFI_GUID Guid
  )
{
  CFG_SET_POWER_PACKAGE_REQUEST SetPowerPackage;

  CopyMem (&SetPowerPackage.Guid, &Guid, sizeof (EFI_GUID));

  return AmthiSubmitRequest(
           &SetPowerPackage,
           AmthiCfgSetPowerPackage,
           sizeof (CFG_SET_POWER_PACKAGE_REQUEST),
           sizeof (CFG_SET_POWER_PACKAGE_RESPONSE)
           );
}

