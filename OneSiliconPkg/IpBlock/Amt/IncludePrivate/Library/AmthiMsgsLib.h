/** @file
  File contains all of the messages definitions for communication between MEBx and AMT FW.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _AMTHI_MSGS_LIB_H_
#define _AMTHI_MSGS_LIB_H_

#include <AmthiMsgs.h>

/**
  This message is sent by the BIOS to retrieve the AMTHI Interface version

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
  OUT AMTHI_VERSION *AmthiInterfaceVersion
  );

/**
  This message is sent by the BIOS to retrieve current IPV4 TCPIP Parameters.

  @param[out] Ipv4Cfg                IPV4 TCPIP Parameters

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
  );

/**
  This message is sent by the BIOS to update current IPV4 TCPIP Parameters.

  @param[in] NewIpv4Cfg              IPV4 TCPIP Parameters

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
  );

/**
  This message is sent by the BIOS to request full AMT Unprovision.

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmthiUnprovision (
  VOID
  );

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
  );

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
  );

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
  );

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
  );

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
  OUT UINT32 *Entries,
  OUT UINT32 *Handles
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
AmthiSetConfigServerData (
  IN UINT16             ServerPort,
  IN UINT8              *ServerAddr,
  IN FQDN_ANSI_STRING   *Fqdn
  );

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
AmthiCfgGetIPv6LanIntStatus (
  IN UINT32                    ConnectionType,
  OUT IPV6_LAN_INTERACE_STRUC  *Ipv6LanIntfStrc
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
  );

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
AmthiSetOptInState (
  IN UINT32 OptInState,
  IN UINT32 RemoteOptInState
  );

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
  );

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
  );

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
AmthiGetAmtPrivacyLevel (
  OUT UINT8 *PrivacyLevel
  );

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
  );

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
  );

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
  );

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
  IN UINT32  ImageId,
  IN UINT32  ByteOffset,
  IN UINT32  ByteSizeReq,
  OUT UINT32 *ByteSizeResp,
  OUT UINT8  *Data
  );

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
  );

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
  );

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
  );

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
  );

#endif
