/** @file
  Header file for AMT functionality

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
#ifndef _DXE_AMT_HECI_LIB_H_
#define _DXE_AMT_HECI_LIB_H_

#include <AsfMsgs.h>
#include <WifiProfileSyncAsfMsgs.h>

/**
  Stop ASF Watchdog Timer HECI message.
**/
VOID
AsfStopWatchDog (
  VOID
  );

/**
  Start ASF Watchdog Timer.
  The WDT will be started only if AMT Watchdog policy is enabled and corresponding timer value is not zero.

  @param[in] WatchDogType         ASF Watchdog Type: 0 - BIOS Watchdog
                                                     1 - OS Watchdog
**/
VOID
AsfStartWatchDog (
  IN  UINT8                       WatchDogType
  );

/**
  This message is sent to switch active LAN interface.

  @param[in] ActiveInterface      Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtSetActiveLanInterface (
  IN UINT8                        ActiveInterface
  );

/**
  This message is sent to get active LAN interface.

  @param[out] ActiveInterface     Active Lan Interface
                                    0: As defined in FIT
                                    1: Integrated LAN
                                    2: Discrete LAN

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
AmtGetActiveLanInterface (
  OUT UINT8                          *ActiveInterface
  );

/**
  This is used to send KVM request message to Intel ME. When
  Bootoptions indicate that a KVM session is requested then BIOS
  will send this message before any graphical display output to
  ensure that FW is ready for KVM session.

  @param[in] QueryType            0 - Query Request
                                  1 - Cancel Request
  @param[out] ResponseCode        1h - Continue, KVM session established.
                                  2h - Continue, KVM session cancelled.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmtQueryKvm (
  IN  UINT32                      QueryType,
  OUT UINT32                      *ResponseCode
  );

/**
  This message is sent by the BIOS in order to pass MAC Address
  which should be used for LAN in a dock.

  @param[in] Enabled              Determines if MAC Passthrough should be used
  @param[in] MacAddress           MAC Address to be used

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
AmtSetMacPassthrough (
  IN UINT8 Enabled,
  IN UINT8 *MacAddress
  );

/**
  Send Remote Platform Erase operation status using PET

  @param[in]    RpeOperationCode    PET Alert Code for the RPE Operation.
  @param[in]    RpeDevice           Device for which the RPE OperationCode is reported.
  @param[in]    RpeOperationStatus  Reports if RPE Operation is in progress or failure.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
SendRpePetAlert (
  IN UINT8                        RpeOperationCode,
  IN UINT8                        RpeDevice,
  IN UINT8                        RpeOperationStatus
  );

/**
  Set UEFI Boot Options States that are available for AMT to use by sending ASF
  command through HECI

  @param[in] AmtBootControl      AMT boot control

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfSetUefiBootOptionsState (
  IN AMT_BOOT_CONTROL      AmtBootControl
  );

/**
  Get UEFI Boot Options States that are available for AMT use by sending ASF
  proper HECI Command.

  @param[out] AmtBootControl     AMT boot control

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR        ASF Response is invalid
**/
EFI_STATUS
AsfGetUefiBootOptionsState (
  OUT AMT_BOOT_CONTROL      *AmtBootControl
  );

/**
  Update UEFI Boot Options available to ASF by sending proper HECI Command

  @param[in] UefiBootOptions      UEFI Boot Options
  @param[in] NumOfUefiBootOptions Number of UEFI Boot Options

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid Boot Options State
  @retval EFI_NOT_FOUND           HECI Communication failed
  @retval EFI_OUT_OF_RESOURCES    Ran out of resources
  @retval EFI_DEVICE_ERROR        ASF Response is invalid
**/
EFI_STATUS
AsfUpdateUefiBootOptions (
  IN UEFI_BOOT_OPTION      *UefiBootOptions,
  IN UINT16                NumOfUefiBootOptions
  );

/**
  Get Boot Option with Parameters that being requested from ASF by sending proper
  HECI Command

  @param[out] UefiBootOptionWithParm    UEFI Boot Option with parameters

  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_INVALID_PARAMETER         Invalid Boot Options State
  @retval EFI_NOT_FOUND                 HECI Communication failed
  @retval EFI_DEVICE_ERROR              ASF Response is invalid
**/
EFI_STATUS
AsfGetUefiBootParameters (
  OUT UEFI_BOOT_OPTION_PARAMETER  *UefiBootOptionWithParm
  );

/**
  Get Root Certificate Authority(CA) Certificate from ASF at specified Index by sending
  proper HECI Command

  @param[in]  Index              Index
  @param[out] Cert               Root CA Certificate

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_INVALID_PARAMETER  Invalid Boot Options State
  @retval EFI_NOT_FOUND          HECI Communication failed
  @retval EFI_DEVICE_ERROR       ASF Response is invalid
**/
EFI_STATUS
AsfGetRootCaCertificate (
  IN UINT32                   Index,
  OUT ROOT_CA_CERTIFICATE     *Cert
  );

/**
  Send One Click Recovery progress or error event using PET

  @param[in] EventOffset          ASF Event Data Codes for OCR Fw Progress/Error Events
  @param[in] EventData3           OCR Additional Event Data 3
  @param[in] EventData4           OCR Additional Event Data 4

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
**/
EFI_STATUS
SendOcrPetEvent (
  IN UINT8              EventOffset,
  IN UINT8              EventData3,
  IN UINT8              EventData4
  );

/**
  Get the wireless profile name

  @param[in, out] Profile         Profile where name will be held and used to aquire profile data, cert and key

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetProfileName (
  IN OUT WIFI_PROFILE_DATA  *Profile
  );

/**
  Get the wireless profile data

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetProfileData (
  IN OUT WIFI_PROFILE_DATA  *Profile
  );

/**
  Get the wireless profile specific 8021X client certificate

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientCertificate (
  IN OUT WIFI_PROFILE     *Profile
  );

/**
  Get the wireless profile specific 8021X client key

  @param[in, out] Profile         Profile data defined in wifi profile protocol

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @return Others                  Error happened
**/
EFI_STATUS
AsfWifiGetClientKey (
  IN OUT WIFI_PROFILE     *Profile
  );

/**
  Write storage erase report to a file with given file name.

  @param[in] FileName            File name of the written data.
  @param[in] DataSize            Size of the written data.
  @param[in] Data                The data to be written to a file.

  @retval EFI_SUCCESS                  Write data to the AMT WEB successfully
  @retval EFI_INVALID_PARAMETER        The parameter is invalid
  @retval EFI_DEVICE_ERROR             Failed to initialize HECI
  @retval EFI_TIMEOUT                  HECI is not ready for communication
  @retval EFI_UNSUPPORTED              Current ME mode doesn't support send this function
**/
EFI_STATUS
WriteStorageEraseReport (
  IN CHAR8   *FileName,
  IN UINT32  DataSize,
  IN UINT8   *Data
  );

/**
  Get HTTP Proxy information by sending a proper ASF command through HECI

  @param[out] AsfHttpProxy        HTTP Proxy information returned from HECI
  @param[out] NumberOfProxies     Number of Proxy servers returned from HECI

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetHttpProxy (
  OUT ASF_HTTP_PROXY            *AsfHttpProxy,
  OUT UINT32                    *NumberOfProxies
  );
#endif
