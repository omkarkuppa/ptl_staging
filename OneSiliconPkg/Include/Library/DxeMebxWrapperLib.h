/** @file
  Header file for MEBx Wrapper Library.

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
#ifndef _DXE_MEBX_WRAPPER_LIB_H_
#define _DXE_MEBX_WRAPPER_LIB_H_

#include <AmthiMsgs.h>
#include <Protocol/GraphicsOutput.h>

/**
  Check if BIOS supports the currently used AMTHI Interface version.

  @retval TRUE    AMTHI Interface version supported
  @retval FALSE   AMTHI Interface version not supported
**/
BOOLEAN
MebxIsAmthiInterfaceVersionSupported (
  VOID
  );

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
  );

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
  );

typedef enum {
  UnprovisionTypePartial,
  UnprovisionTypeFull
} UNPROVISION_TYPE;

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Update current PKI FQDN DNS Suffix.

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
MebxSetPkiFqdnSuffix (
  IN FQDN_SUFFIX_ANSI_STRING *NewPkiDns
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Update current FQDN Data.

  @param[in]  FqdnData               New FQDN data

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
  );

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
  );

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
  );

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
  );

/**
  Start the remote configuration process.

  @param[out] AmtStatus              Amt Error Status

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
  );

/**
  Set MEBx state and verify if KVM session is not active during MEBx UI.

  @param[in] MebxState               New MEBx state

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetMebxState (
  IN AMT_MEBX_STATE MebxState
  );

/**
  This function retrieves platform settings regarding Opt-in policies.

  @param[out] OptInConfigExposure        Determines if user should be able to change
                                         User Opt-In configuration
  @param[out] RemoteItOptInConfigExpoure Determines if user should be able to change
                                         Remote IT Opt-In configuration
  @param[out] UserOptIn                  Determines list of features that require User Consent
  @param[out] RemoteItOptInConfig        Determines whether user consent policy can be modified from remote


  @retval EFI_SUCCESS                    Function succeeded
  @retval EFI_INVALID_PARAMETER          NULL parameter
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxGetPlatformOptInPolicy (
  OUT UINT32 *OptInConfigExposure,
  OUT UINT32 *RemoteItOptInConfigExposure,
  OUT UINT32 *UserOptIn,
  OUT UINT32 *RemoteItOptInConfig
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Update ME Power Policy.

  @param[in] MeOnHostSlpStates       Index of the GUID previously retrieved by XXX function
                                     to be set

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
  );

typedef enum {
  AmtDisabled,
  AmtPartiallyDisabled,
  AmtEnabled
} AMT_STATE;

/**
  Update Amt State.

  @param[in] AmtState                New AMT State

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
MebxSetAmtState (
  IN UINT8     AmtState
  );

#endif
