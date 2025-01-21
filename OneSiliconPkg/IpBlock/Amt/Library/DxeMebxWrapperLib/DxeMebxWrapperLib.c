/** @file
  File contains wrapper functions for MEBx interactions.

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
#include <Library/AmthiMsgsLib.h>
#include <Library/DxeMebxWrapperLib.h>
#include <Library/DxeMeLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MebxConfigProtocol.h>
#include <MebxData.h>

#define MEBX_PROVISIONING_TIMEOUT             5       ///< 5 seconds
#define MEBX_EXTENDED_PROVISIONING_TIMEOUT    30      ///< 30 seconds
#define MEBX_UNPROVISIONING_TIMEOUT           30      ///< 30 seconds
#define MEBX_STALL_1_SECOND                   1000000 ///< 1 second

/**
  Check if BIOS supports the currently used AMTHI Interface version.

  @retval TRUE    AMTHI Interface version supported
  @retval FALSE   AMTHI Interface version not supported
**/
BOOLEAN
MebxIsAmthiInterfaceVersionSupported (
  VOID
  )
{
  EFI_STATUS    Status;
  AMTHI_VERSION AmthiInterfaceVersion;

  Status = AmthiGetInterfaceVersion (&AmthiInterfaceVersion);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }

  if (AmthiInterfaceVersion.MajorVersion < AMTHI_VERSION_MAJOR ||
      AmthiInterfaceVersion.MinorVersion < AMTHI_VERSION_MINOR) {
    return FALSE;
  }

  return TRUE;
}

/**
  Retrieve current IPV4 TCPIP Parameters.

  @param[in]  Interface              Interface type:
                                       0 - WIRED
                                       1 - WIRELESS
  @param[in]  GetDynamicIpAddress    If TRUE, then the function will return
                                     IPV4 Address obtained from DHCP if DHCP
                                     is enabled
  @param[out] IsLanlessPlatform      Indicates if Interface LAN is available
  @param[out] Ipv4Cfg                IPV4 TCPIP Parameters. Returned only
                                     if Interface LAN Interface is available

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetIpv4Params (
  IN  UINT32           Interface,
  IN  BOOLEAN          GetDynamicIpAddress,
  OUT UINT8            *IsLanlessPlatform,
  OUT IMB_TCPIP_PARAMS *Ipv4Cfg
  )
{
  EFI_STATUS             Status;
  LAN_INTERFACE_SETTINGS LanInterfaceSettings;

  if (Ipv4Cfg == NULL || IsLanlessPlatform == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  // check if this is LAN-less platform
  *IsLanlessPlatform = 0;
  Status = AmthiGetLanInterfaceSettings (&LanInterfaceSettings, Interface);
  if (EFI_ERROR (Status) && Status == EFI_NO_MEDIA) {
    *IsLanlessPlatform = 1;
    return EFI_SUCCESS;
  }

  Status = AmthiGetIpv4Params (Ipv4Cfg);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (GetDynamicIpAddress == TRUE && Ipv4Cfg->DhcpMode == DhcpModeEnabled) {
    Ipv4Cfg->LocalAddr = LanInterfaceSettings.Ipv4Address;
  }

  return Status;
}

/**
  Update current IPV4 TCPIP Parameters.

  @param[in] NewIpv4Cfg              IPV4 TCPIP Parameters

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetIpv4Params (
  IN IMB_TCPIP_PARAMS *NewIpv4Cfg
  )
{
  if (NewIpv4Cfg == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiSetIpv4Params (NewIpv4Cfg);
}

/**
  Performs Intel(R) AMT full and partial unprovisioning flows.
  Performs unprovisiong, re-initializes AMTHI, and if full unprovision, resets the
  MEBx password to AMT.

  @param[in] UnprovisionType      Specifies Unprovisioning Type:
                                    0 - partial unprovision
                                    1 - full unprovision

  @retval EFI_SUCCESS             Unprovisioning successful
  @retval EFI_TIMEOUT             Timeout has occured
  @retval Others                  Other error has occured
**/
EFI_STATUS
MebxPerformUnprovisioning (
  IN UINT8 UnprovisionType
  )
{
  EFI_STATUS Status;
  UINT32     UnprovisioningState = 0;
  UINT32     Timeout = MEBX_UNPROVISIONING_TIMEOUT;

  //Step 1: Send Correct Unprovision command
  if (UnprovisionType == UnprovisionTypeFull) {
    Status = AmthiUnprovision ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
  } else {
    Status = AmthiPartialUnprovision ();
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  //Step 2: After unprovisioning is sent, poll unprovisioning state until it has left the IN state.
  Status = AmthiGetUnprovisioningState (&UnprovisioningState);
  if (EFI_ERROR (Status)) {
      return Status;
  }

  while ((UnprovisioningState == UnprovisioningStateIn) && (Timeout != 0)) {
    Status = AmthiGetUnprovisioningState (&UnprovisioningState);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MicroSecondDelay (MEBX_STALL_1_SECOND);
    Timeout--;
  }

  if (Timeout == 0) {
    Status = EFI_TIMEOUT;
  }

  return Status;
}

/**
  Retrieve current provisioning state.

  @param[out] ProvisioningState      Current provisioning state:
                                       @see PT_PROVISIONING_STATE

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetProvisioningState (
  OUT UINT32 *ProvisioningState
  )
{
  if (ProvisioningState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetProvisioningState (ProvisioningState);
}

/**
  Update current SOL and Storage Redirection state.

  @param[in] NewSolStatus            New SOL state
  @param[in] NewStorageRedirStatus   New Storage Redirection state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetSolStorageRedir (
  IN UINT16 NewSolStatus,
  IN UINT16 NewStorageRedirStatus
  )
{
  EFI_STATUS               Status;
  MEBX_CONFIG_PROTOCOL     *MebxConfigProtocol;
  MEBX_DATA                MebxData;
  UINTN                    MebxDataSize;

  Status = AmthiSetSolStorageRedir (NewSolStatus, NewStorageRedirStatus);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gMebxConfigProtocolGuid, NULL, (VOID**) &MebxConfigProtocol);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Platform must implement this protocol to set/get Mebx settings\n"));
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  MebxDataSize = sizeof (MEBX_DATA);
  Status = MebxConfigProtocol->GetMebxConfig (&MebxDataSize, (VOID*) &MebxData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "MebxConfigProtocol->GetMebxConfig %r, Create new MEBx Config\n", Status));
    ZeroMem (&MebxData, MebxDataSize);
  }
  MebxData.AmtSol  = (UINT8) NewSolStatus;
  MebxData.AmtSrou = (UINT8) NewStorageRedirStatus;

  return MebxConfigProtocol->SetMebxConfig (MebxDataSize, (VOID*) &MebxData);
}

/**
  Retrieve current EoP state.

  @param[out] State                  Current End of Post State:
                                       @see CFG_EOP_STATE

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetEndOfPostState (
  OUT CFG_EOP_STATE *State
  )
{
  if (State == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetEndOfPostState (State);
}

/**
  Retrieve available Hashed Certificates.

  @param[out] CertQuantity           Number of available Hashed Certificates entries
  @param[out] CertHandle             Array with available Hashed Certificates list
  @param[out] CertHashEntry          Pointer to the structure containing info regarding
                                     all the available Hashed Certificates
                                       @see CERT_HASH_ENTRY

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetHashedCertificates (
  OUT UINT32          *CertQuantity,
  OUT UINT32          *CertHandle,
  OUT CERT_HASH_ENTRY *CertHashEntry
  )
{
  EFI_STATUS Status;
  UINT32     HashEntry;
  CERT_NAME  *CertName;

  if (CertQuantity == NULL || CertHandle == NULL || CertHashEntry == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiEnumerateHashHandles (CertQuantity, CertHandle);
  ASSERT (*CertQuantity <= MAX_HASH_ENTRIES);

  for (HashEntry = 0; HashEntry < *CertQuantity; HashEntry++) {
    Status = AmthiGetCertificateHashEntry (CertHandle[HashEntry], &CertHashEntry[HashEntry]);
    CertName = &CertHashEntry[HashEntry].CertName;
    //
    // Add null-termination character since acquired string
    // doesn't have to be null-terminated
    //
    CertName->Buffer[CertName->Length] = '\0';
  }

  return Status;
}

/**
  Update current PKI FQDN DNS Suffix.

  @attention Function is not allowed in POST_PROVISION State

  @param[in] NewPkiDns               New Fqdn Pki Dns suffix

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetPkiFqdnSuffix (
  IN FQDN_SUFFIX_ANSI_STRING *NewPkiDns
  )
{
  if (NewPkiDns == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiSetPkiFqdnSuffix (NewPkiDns);
}

/**
  Retrieve ZTC State.

  @param[out] ZtcEnabledStatus       Current ZTC Enabled Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetZtcEnabledStatus (
  OUT UINT32 *ZtcEnabledStatus
  )
{
  if (ZtcEnabledStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetZtcEnabledStatus (ZtcEnabledStatus);
}

/**
  Retrieve current AMT Idle Timeout.

  @param[out] AmtIdleTimeout         Current AMT Idle Timeout

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetIdleTimeout (
  OUT UINT16 *AmtIdleTimeout
  )
{
  if (AmtIdleTimeout == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetIdleTimeout (AmtIdleTimeout);
}

/**
  Update current AMT Idle Timeout.

  @param[in] AmtIdleTimeout          New AMT Idle Timeout

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetIdleTimeout (
  IN UINT16 AmtIdleTimeout
  )
{
  return AmthiSetIdleTimeout (AmtIdleTimeout);
}

/**
  Retrieve current PKI DNS Data.

  @param[out] CurrentPkiDns          Current PKI DNS Data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetPkiFqdnSuffix (
  OUT FQDN_SUFFIX_ANSI_STRING *CurrentPkiDns
  )
{
  EFI_STATUS Status;

  if (CurrentPkiDns == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiGetPkiFqdnSuffix (CurrentPkiDns);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Add null-termination character since acquired string
  // doesn't have to be null-terminated
  //
  CurrentPkiDns->Buffer[CurrentPkiDns->Length] = '\0';

  return Status;
}

/**
  Update Certificate Hash State.

  @param[in] Handle                  Handle to Hash to Update State for
  @param[in] IsActive                New User Defined State for Certificate Hash State
                                        (Active/Not Active)

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetCertificateHashState (
  IN UINT32 *Handle,
  IN UINT32 IsActive
  )
{
  if (Handle == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiSetCertificateHashState (Handle, IsActive);
}

/**
  Update Zero Touch Enabled State.

  @param[in] NewZtcStatus            New ZTC Enabled Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetZeroTouchEnabled (
  IN UINT32 NewZtcStatus
  )
{
  return AmthiSetZeroTouchEnabled (NewZtcStatus);
}

/**
  Request a User Connection for CIRA.

  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxOpenUserInitiatedConnection (
  OUT UINT32 *AmtStatus
  )
{
  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiOpenUserInitiatedConnection (AmtStatus);
}

/**
  Close a User Connection for CIRA.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxCloseUserInitiatedConnection (
  VOID
  )
{
  return AmthiCloseUserInitiatedConnection ();
}

/**
  Query the status of CIRA Connection.

  @param[out] ConnectionData         Connection Data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge or Fqdn too long
**/
EFI_STATUS
MebxGetConnectionStatus (
  OUT REMOTE_ACCESS_CONNECTION_STRUCT *ConnectionData
  )
{
  if (ConnectionData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetConnectionStatus (ConnectionData);
}

/**
  Update current MEBx password change policy.

  @param[in] PwdPolicy               New MEBx password change policy:
                                       @see CFG_MEBX_PWD_CHANGE_POLICY

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetMebxPwdPolicy (
  IN UINT32 PwdPolicy
  )
{
  return AmthiSetMebxPwdPolicy (PwdPolicy);
}

/**
  Retrieve current MEBx password change policy.

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
MebxGetMebxPwdPolicy (
  OUT UINT32 *PwdPolicy
  )
{
  if (PwdPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetMebxPwdPolicy (PwdPolicy);
}

/**
  Retrieve current AMT Config Server data.

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
MebxGetConfigServerData (
  OUT UINT16           *ServerPort,
  OUT UINT8            *ServerAddr,
  OUT FQDN_ANSI_STRING *Fqdn
  )
{
  EFI_STATUS           Status;

  if (ServerPort == NULL || ServerAddr == NULL || Fqdn == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiGetConfigServerData (ServerPort, ServerAddr, Fqdn);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Add null-termination character since acquired string
  // doesn't have to be null-terminated
  //
  Fqdn->Buffer[Fqdn->Length] = '\0';

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
MebxSetConfigServerData (
  IN UINT16             ServerPort,
  IN UINT8              *ServerAddr,
  IN FQDN_ANSI_STRING   *Fqdn
  )
{
  if (Fqdn == NULL || ServerAddr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiSetConfigServerData (ServerPort, ServerAddr, Fqdn);
}

/**
  Retrieve current AMT FW Audit Record which Holds Info about Last Provisioning Attempt.

  @param[out] Audit                  AMT's Current Audit Record

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetMeSetupAuditRecord (
  OUT ME_ADMIN_AUDIT_RECORD *Audit
  )
{
  EFI_STATUS                Status;

  if (Audit == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiGetMeSetupAuditRecord (Audit);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Add null-termination character since acquired string
  // doesn't have to be null-terminated
  //
  Audit->ProvServerFqdn.Buffer[Audit->ProvServerFqdn.Length] = '\0';

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
MebxCfgGetIPv6LanIntStatus (
  IN UINT32                    ConnectionType,
  OUT IPV6_LAN_INTERACE_STRUC  *Ipv6LanIntfStrc
  )
{
  if (Ipv6LanIntfStrc == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiCfgGetIPv6LanIntStatus (ConnectionType, Ipv6LanIntfStrc);
}

/**
  Retrieve current FQDN Data.

  @param[out] FqdnData               Current FQDN data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetFqdn (
  OUT FQDN_DATA *FqdnData
  )
{
  EFI_STATUS Status;

  if (FqdnData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiGetFqdn (FqdnData);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  //
  // Add null-termination character since acquired string
  // doesn't have to be null-terminated
  //
  FqdnData->Fqdn.Buffer[FqdnData->Fqdn.Length] = '\0';

  return Status;
}

/**
  Update current FQDN Data.

  @param[in] FqdnData                New FQDN data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetFqdn (
  IN FQDN_DATA *FqdnData
  )
{
  if (FqdnData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiSetFqdn (FqdnData);
}

/**
  Retrieve current Redirection features state.

  @param[out] FeaturesState          Current Redirection features state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/

EFI_STATUS
MebxGetRedirectionState (
  OUT REDIRECTION_FEATURES_STATE *FeaturesState
  )
{
  if (FeaturesState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetRedirectionState (FeaturesState);
}

/**
  Complete configuration and transition to the post provisioning state.

  @param[out] AmtStatus              Amt Error Status

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxCompleteConfigurationRequest (
  OUT UINT32 *AmtStatus
  )
{
  if (AmtStatus == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiCompleteConfigurationRequest (AmtStatus);
}

/**
  Stop the remote configuration process.

  @param[out] ProvisioningState      Platform provisioning state

  @retval EFI_SUCCESS                Remote Configuration stopped
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxStopRemoteConfiguration (
  OUT UINT32 *ProvisioningState
  )
{
  EFI_STATUS Status;
  UINT32     UnprovisioningState;
  UINT32     Timeout = MEBX_UNPROVISIONING_TIMEOUT;

  if (ProvisioningState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiStopConfiguration ();
  if (EFI_ERROR (Status)) {
    return Status;
  }

  do {
    //After stop configuration is sent, poll unprovisioning state until it has left the IN state.
    Status = AmthiGetUnprovisioningState (&UnprovisioningState);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MicroSecondDelay (MEBX_STALL_1_SECOND);
    Timeout--;
  } while (Timeout && (UnprovisioningState == ProvisioningStateIn));


  return AmthiGetProvisioningState (ProvisioningState);
}

/**
  Start the remote configuration process.

  @param[out] ProvisioningState      Platform provisioning state

  @retval EFI_SUCCESS                Remote Configuration Activated
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
  @retval EFI_NOT_READY              Certificate not ready yet
**/
EFI_STATUS
MebxActivateRemoteConfiguration (
  OUT UINT32 *ProvisioningState
  )
{
  EFI_STATUS   Status;
  UINT32       AmtStatus;
  UINT32       Timeout;

  Timeout = MEBX_PROVISIONING_TIMEOUT;

  if (ProvisioningState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiStartConfigurationEx (&AmtStatus);
  if (EFI_ERROR (Status)) {
    if (AmtStatus == AmtStatusCertificateNotReady) {
      // This is not a real failure - AMT needs some more time, set extended timeout
      Timeout = MEBX_EXTENDED_PROVISIONING_TIMEOUT;
    } else {
      return EFI_DEVICE_ERROR;
    }
  }

  do {
    // Transition to PROVISIONING_MODE_IN determines whether StartConfiguration was processed successfully.
    // Ping for provisioning mode in intervals
    Status = AmthiGetProvisioningState (ProvisioningState);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    MicroSecondDelay (MEBX_STALL_1_SECOND);
    Timeout--;
  } while (Timeout && (*ProvisioningState != ProvisioningStateIn));

  if (*ProvisioningState != ProvisioningStateIn) {
    return EFI_NOT_READY;
  }

  return Status;
}

/**
  This message is sent by the BIOS to set MEBx state and verify if KVM session
  is not active during MEBx UI.

  @param[in] MebxState               New MEBx state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
  @retval EFI_ALREADY_STARTED        KVM Session is active
**/
EFI_STATUS
MebxSetMebxState (
  IN AMT_MEBX_STATE MebxState
  )
{
  EFI_STATUS Status;
  UINT32     AmtStatus;

  Status = AmthiSetMebxState (MebxState, &AmtStatus);
  if (EFI_ERROR (Status) && AmtStatus == AmtStatusRedirectionInProgress) {
    return EFI_ALREADY_STARTED;
  }

  return Status;
}

/**
  This function retrieves platform settings regarding Opt-in policies.

  @param[out] OptInConfigExposure         Determines if user should be able to change
                                          User Opt-In configuration
  @param[out] RemoteItOptInConfigExposure Determines if user should be able to change
                                          Remote IT Opt-In configuration
  @param[out] UserOptIn                   Determines list of features that require User Consent
  @param[out] RemoteItOptInConfig         Determines whether user consent policy can be modified from remote


  @retval EFI_SUCCESS                     Function succeeded
  @retval EFI_INVALID_PARAMETER           NULL parameter
  @retval EFI_UNSUPPORTED                 Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR                HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                     HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL            Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetPlatformOptInPolicy (
  OUT UINT32 *OptInConfigExposure,
  OUT UINT32 *RemoteItOptInConfigExposure,
  OUT UINT32 *UserOptIn,
  OUT UINT32 *RemoteItOptInConfig
  )
{
  EFI_STATUS Status;
  UINT32     OptInAvailable;
  UINT32     ControlMode;

  if (OptInConfigExposure == NULL || RemoteItOptInConfigExposure == NULL ||
      UserOptIn == NULL || RemoteItOptInConfig == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *OptInConfigExposure         = 0;
  *RemoteItOptInConfigExposure = 0;

  Status = AmthiGetControlMode (&ControlMode);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Changing the settings in User Mode is not allowed
  //
  if (ControlMode == AmtControlModeUser) {
    return EFI_SUCCESS;
  }

  Status = AmthiGetOptInChangeAvailable (&OptInAvailable, RemoteItOptInConfigExposure);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // check if OptInAvailable bitmap has only one bit set - we don't want the menu displayed in that case
  //
  if (OptInAvailable == OptinPolicyBitmapNone || OptInAvailable == OptinPolicyBitmapKvmOnly || OptInAvailable == OptinPolicyBitmapAll) {
    return EFI_SUCCESS;
  } else {
    *OptInConfigExposure = OptInAvailable;
  }

  return AmthiGetOptInState (UserOptIn, RemoteItOptInConfig);
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
MebxSetOptInState (
  IN UINT32 OptInState,
  IN UINT32 RemoteOptInState
  )
{
  return AmthiSetOptInState (OptInState, RemoteOptInState);
}

/**
  Update current state of KVM Feature.

  @param[in] KvmState                New KVM State

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetKvmEnabled (
  IN UINT32 KvmState
  )
{
  return AmthiSetKvmEnabled (KvmState);
}

/**
  Retrieve current state of KVM Feature.

  @param[out] KvmState               Current KVM State

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetKvmEnabled (
  OUT UINT8 *KvmState
  )
{
  if (KvmState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetKvmEnabled (KvmState);
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
MebxGetAmtPrivacyLevel (
  OUT UINT8 *PrivacyLevel
  )
{
  if (PrivacyLevel == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  return AmthiGetAmtPrivacyLevel (PrivacyLevel);
}

/**
  Retrieve ME Power Policy configuration.

  This function retrieves the maximum number of available power packages for the given platform.
  Power packages are described by a list of GUIDs and each GUID has it's own text description.

  @param[out] NumberOfPowerPkgs       Number of available power packages GUIDs
  @param[out] CurrentMeOnHostSlpState Index of the currently used power package GUID
  @param[out] PackageDescription      Array with the text decriptions for available GUIDs

  @retval EFI_SUCCESS                 Function succeeded
  @retval EFI_INVALID_PARAMETER       NULL parameter
  @retval EFI_UNSUPPORTED             Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR            HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                 HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL        Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetPowerControlSettings (
  OUT UINT8           *NumberOfPowerPkgs,
  OUT UINT8           *CurrentMeOnHostSlpState,
  OUT IMB_ANSI_STRING *PackageDescription
  )
{
  EFI_STATUS      Status;
  EFI_GUID        PackagesGuids[MAX_PWR_PKG_COUNT];
  EFI_GUID        ConfiguredGuid;
  UINT8           PackageNumber;

  if (NumberOfPowerPkgs == NULL || CurrentMeOnHostSlpState == NULL || PackageDescription == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiEnumeratePowerPackages (NumberOfPowerPkgs, PackagesGuids);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  ASSERT (*NumberOfPowerPkgs <= MAX_PWR_PKG_COUNT);

  Status = AmthiGetCurrentPowerPackage (&ConfiguredGuid);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (PackageNumber = 0; PackageNumber < *NumberOfPowerPkgs; PackageNumber++) {
    Status = AmthiGetPowerPackageInfo (PackagesGuids[PackageNumber], PackageDescription);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    //
    // Adjust length and add null-termination character since
    // acquired string doesn't have to be null-terminated
    //
    if (PackageDescription->Length >= MAX_PWR_PKG_DESC_SIZE) {
      PackageDescription->Length = MAX_PWR_PKG_DESC_SIZE - 1;
    }
    PackageDescription->Buffer[PackageDescription->Length] = '\0';

    //
    // Find the index of the currently used power package
    //
    if (CompareMem ((VOID *) &PackagesGuids[PackageNumber], (VOID *) &ConfiguredGuid, sizeof (EFI_GUID)) == 0) {
      *CurrentMeOnHostSlpState = PackageNumber;
    }
    PackageDescription++;
  }
  return Status;
}

/**
  Update ME Power Policy.

  @param[in] MeOnHostSlpStates       New index of a power package GUID

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxUpdatePowerPolicy (
  IN UINT8 MeOnHostSlpStates
  )
{
  EFI_STATUS Status;
  UINT8      NumberOfPowerPkgs;
  EFI_GUID   PackagesGuids[MAX_PWR_PKG_COUNT];

  Status = AmthiEnumeratePowerPackages (&NumberOfPowerPkgs, PackagesGuids);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return AmthiSetPowerPackage (PackagesGuids[MeOnHostSlpStates]);
}

/**
  Update Amt State.

  @param[in] AmtState                New AMT State:
                                       @see AMT_STATE

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetAmtState (
  IN UINT8     AmtState
  )
{
  EFI_STATUS   Status;
  MEFWCAPS_SKU UserCapabilitiesState;
  MEFWCAPS_SKU FwFeatures;

  Status = HeciGetUserCapabilitiesState (&UserCapabilitiesState);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = HeciGetFwFeatureStateMsg (&FwFeatures);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  switch (AmtState) {
    case AmtDisabled:
      UserCapabilitiesState.Fields.Amt = 0;
      FwFeatures.Fields.Amt            = 0;
      break;

    case AmtPartiallyDisabled:
      UserCapabilitiesState.Fields.Amt = 1;
      FwFeatures.Fields.Amt            = 0;
      break;

    case AmtEnabled:
      UserCapabilitiesState.Fields.Amt = 1;
      FwFeatures.Fields.Amt            = 1;
      break;
  }

  Status = HeciSetFwEnabledFeatures (FwFeatures.Data, AMT_BITMASK);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return HeciSetUserCapabilitiesState (UserCapabilitiesState);
}

