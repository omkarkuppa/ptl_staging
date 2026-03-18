/** @file
  AMT/ASF Messages

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

#ifndef _ASF_MSGS_H_
#define _ASF_MSGS_H_

#include <HeciAddresses.h>
#include <AmthiMsgs.h>
#include <Pi/PiStatusCode.h>
#include <IndustryStandardAsf.h>
#include <IntelAsfExtensions.h>

#pragma pack(1)

//
// @todo Some work remains to be done for this module. This will be covered by a separate check-in:
// 1. Change GET_BOOT_OPTIONS_RESPONSE structure. The idea is to always define the ASF_MESSAGE_HEADER.
//    Current implementation does not allow this. Need to move 2 bytes from ASF_BOOT_OPTIONS to
//    GET_BOOT_OPTIONS_RESPONSE but this requires some bigger refactoring in the code.
//
// 2. Actions based on the GetBootOptions()'s response should be move to subfunctions in AsfDxe.c file.
//

//
// Calculates Byte count of ASF MESSAGE structure
//
#define ASF_MESSAGE_BYTE_COUNT(a) (a-2)

//
// Always define the whole ASF message structure so you can use the macro for ByteCount calculation
//
#define ASF_MESSAGE_BYTE_COUNT_MAP(a) (ASF_MESSAGE_BYTE_COUNT(sizeof(a)))

///
/// Abstract ASF message header. This header is part of HECI message sent from BIOS to AMT/ASF ME Client
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  Command       : 8;          ///< ASF Message Command.
    UINT32  ByteCount     : 8;          ///< ASF Message Bytes Count Beyond This Byte.
    UINT32  SubCommand    : 8;          ///< ASF Message Sub-command.
    UINT32  VersionNumber : 8;          ///< ASF Specification Version.
  } Fields;
} ASF_MESSAGE_HEADER;

//
// Management Control Command Definitions
//

///
/// Start Watchdog Timer Type
///
#define ASF_START_BIOS_WDT  0
#define ASF_START_OS_WDT    1

///
/// Start Watchdog Timer Message Structure.
/// This structure is only used for Starting Watchdog Timer Message.
/// Elements' description is based on Start Watchdog Timer.
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;       ///< ASF Message Header.
  UINT8                TimeoutLow;      ///< Low byte of watchdog timeout value in seconds.
  UINT8                TimeoutHigh;     ///< High byte of watchdog timeout value in seconds.
  UINT8                EventSensorType; ///< Default value is 0x23 to indicate it is for Watchdog 2.
  UINT8                EventType;       ///< Default value is 0x6F to indicate it is Sensor specific.
  UINT8                EventOffset;     ///< Default value is 0x00 to indicate it is for Timer expired.
  /**
    Class of device or type of software that originated the event.
      0x00: Platform firmware
      0x48: OS vendor
  **/
  UINT8                EventSourceType;
  UINT8                EventSeverity;   ///< Default value is 0x10 to indicate it is Critical.
  UINT8                SensorDevice;    ///< Default value is 0xFF to indicate it is Unspecified.
  UINT8                SensorNumber;    ///< Default value is 0xFF to indicate it is Unspecified.
  UINT8                Entity;          ///< Default value is 0x00 to indicate it is Unspecified.
  UINT8                EntityInstance;  ///< Default value is 0x00 to indicate it is Unspecified.
  /**
    Additional parametric data byte.
      BYTE 0: 0x40
      BYTE 1: 0x02, BIOS POST WDT Timeout
              0x04, OS WDT Timeout
      BYTE 2-4: Not defined.
  **/
  UINT8                EventData[5];
} ASF_START_WDT;

///
/// Stop Watchdog Timer Message Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;       ///< ASF Message Header.
} ASF_STOP_WDT;


//
// ASF Configuration Command Definitions
//

///
/// Clear Boot Options Message Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;       ///< ASF Message Header.
} CLEAR_BOOT_OPTIONS;

///
/// Return Boot Options
/// Elements' description is based on Get Boot Options and No Boot Options Message.
///
typedef struct {
  /**
    ASF Message Sub-command.
      0x16: Return boot option
      0x17: No boot option
  **/
  UINT8   SubCommand;
  UINT8   Version;                      ///< Default value is 0x10 to indicate ASF Specification Version.
  /**
    IANA-assigned Enterprise Number.
      0x000011BE: IANA Enterprise Number for ASF
      0x00000157: IANA Enterprise Number for Intel
  **/
  UINT32  IanaId;
  /**
    Defines commands to be processed by the managed client on the boot initiated by the ASF-RMCP message.
      0x00: NOP
      0x01: Force PXE Boot
      0x02: Force Hard-drive Boot
      0x03: Force Hard-drive Safe Mode Boot
      0x04: Force Diagnostic Boot
      0x05: Force CD/DVD Boot
      0x06-0xBF: Reserved
      0xC0-0xFF: OEM special commands
      0xC1: Intel special command
  **/
  UINT8   SpecialCommand;
  UINT8   SpecialCommandParamHighByte;  ///< Defines a command parameter to augment the Special Command High Byte.
  UINT8   SpecialCommandParamLowByte;   ///< Defines a command parameter to augment the Special Command Low Byte.
  UINT8   BootOptionsMaskByte1;         ///< Defines a standard set of firmware operations.
  UINT8   BootOptionsMaskByte2;         ///< Defines a standard set of firmware operations.
  UINT8   OemParameterByte1;            ///< Defines parameters that further augment the Special Command definition.
  UINT8   OemParameterByte2;            ///< Defines parameters that further augment the Special Command definition.
} ASF_BOOT_OPTIONS;

///
/// Get Boot Options Response Structure
///
typedef struct {
  UINT8                 Command;        ///< ASF Message Command.
  UINT8                 ByteCount;      ///< ASF Message Bytes Count Beyond This Byte.
  ASF_BOOT_OPTIONS      AsfBootOptions; ///< ASF Return Boot Options.
} GET_BOOT_OPTIONS_RESPONSE;

///
/// Get Boot Options Request Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;       ///< ASF Message Header.
} GET_BOOT_OPTIONS;

///
/// Get Boot Options Union Buffer
///
typedef union {
  GET_BOOT_OPTIONS          Request;
  GET_BOOT_OPTIONS_RESPONSE Response;
} GET_BOOT_OPTIONS_BUFFER;

///
/// Get HTTP Proxy
///
#define MAX_FQDN_LENGTH        256
#define MAX_DNS_SUFFIX_LENGTH  192
#define MAX_NUMBER_OF_PROXIES  15

typedef struct Proxy {
  CHAR8  AccessInfo [MAX_FQDN_LENGTH];
  UINT16 InfoFormat;
  UINT16 Port;
  CHAR8  NetworkDnsSuffix[MAX_DNS_SUFFIX_LENGTH];
  UINT32 Priority;
} ASF_HTTP_PROXY;

///
/// Get HTTP Proxy Response Structure
///
typedef struct {
  ASF_MESSAGE_HEADER  AsfHeader;       ///< ASF Message Header.
  UINT32              Status;
  UINT32              NumberOfProxies;
  ASF_HTTP_PROXY      Proxies[MAX_NUMBER_OF_PROXIES];
} GET_HTTP_PROXY_RESPONSE;

///
/// Get HTTP Proxy Request Structure
///
typedef struct {
  ASF_MESSAGE_HEADER  AsfHeader;       ///< ASF Message Header.
} GET_HTTP_PROXY_REQUEST;

///
/// Get HTTP Proxy Buffer
///
typedef union {
  GET_HTTP_PROXY_REQUEST  Request;
  GET_HTTP_PROXY_RESPONSE Response;
} GET_HTTP_PROXY_BUFFER;

///
/// Report BIOS Status
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;       ///< ASF Message Header.
  union {
    struct {
      /**
        Current BIOS Status.
          0: Success
          1: In Progress
          2: Not reported
          0xFFFF: Failed
      **/
      UINT16               BiosStatus;
      /**
        The Reason Causes Current BIOS Status.
          0: BiosStatus is Success/InProgress
          1: General Drive Failure
          2: Drive Password/Authentication Failure
          3: Feature is not supported
      **/
      UINT16               StatusCause;
    } Field;
    UINT32 Data;
  } AsfBiosStatus;
} REPORT_BIOS_STATUS;

//
// Definition of ASF WRITE FILE
//
#define MAX_APPLICATION_NAME_LENGTH  12
#define MAX_FILE_NAME_LENGTH         12
#define MAX_FILE_SIZE                6000

//
// ASF Write File Response Structure
//
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;                                     ///< ASF Message Header.
  UINT32               Status;  // 0: SUCCESS
} ASF_WRITE_FILE_RESPONSE;

//
// ASF Write File Request Structure
//
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;                                     ///< ASF Message Header.
  CHAR8                ApplicationName[MAX_APPLICATION_NAME_LENGTH];
  CHAR8                FileName[MAX_FILE_NAME_LENGTH];
  UINT32               FileSize;                                      ///< number of bytes
  UINT8                File[MAX_FILE_SIZE];
} ASF_WRITE_FILE_REQUEST;

//
// ASF Write File union Buffer
//
typedef union {
  ASF_WRITE_FILE_REQUEST     Request;
  ASF_WRITE_FILE_RESPONSE    Response;
} ASF_WRITE_FILE_BUFFER;

//
// AMT Boot Control Structure
//
typedef union {
  // Individual bit fields
  struct {
    UINT32 Reserved                  : 1;  ///< [0] Reserved
    UINT32 AmtTrigBootToHttps        : 1;  ///< [1] AMT can trigger Https Boot
    UINT32 AmtTrigBootToPba          : 1;  ///< [2] AMT can trigger PBA Boot
    UINT32 AmtTrigBootToWinRe        : 1;  ///< [3] AMT can trigger WinRe Boot
    UINT32 AmtAllowedToDisSecureBoot : 1;  ///< [4] AMT is allowed to disable secure boot
    UINT32 Reserved3                 : 1;  ///< [5] Reserved
    UINT32 BiosPasswordAuthRequired  : 1;  ///< [6] BIOS password authorization required
    UINT32 Reserved1                 : 1;  ///< [7] Reserved
    UINT32 RemotePlatformErase       : 1;  ///< [8] Remote Platform Erase
    UINT32 Reserved2                 : 23; ///< [31:9] Reserved
  } Bits;

  UINT32 Uint32;

} AMT_BOOT_CONTROL;

//
// Set UEFI Boot Options State Message Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;       ///< ASF Message Header.
  AMT_BOOT_CONTROL      AmtBootControl;  ///< AMT Boot Control
} SET_UEFI_BOOT_OPTIONS_STATE;

//
// Set UEFI Boot Options State Response Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;       ///< ASF Message Header
  AMT_STATUS            Status;          ///< AMT Status
} SET_UEFI_BOOT_OPTIONS_STATE_RESPONSE;

//
// Set UEFI Boot Options State Union Buffer
//
typedef union {
  SET_UEFI_BOOT_OPTIONS_STATE          Request;
  SET_UEFI_BOOT_OPTIONS_STATE_RESPONSE Response;
} SET_UEFI_BOOT_OPTIONS_STATE_BUFFER;

//
// Get UEFI Boot Options State Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;       ///< ASF Message Header.
} GET_UEFI_BOOT_OPTIONS_STATE;

//
// Get UEFI Boot Options State Response Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;       ///< ASF Message Header.
  AMT_BOOT_CONTROL      AmtBootControl;  ///< AMT Boot Control
} GET_UEFI_BOOT_OPTIONS_STATE_RESPONSE;

//
// Get UEFI Boot Options State Union Buffer
//
typedef union {
  GET_UEFI_BOOT_OPTIONS_STATE            Request;
  GET_UEFI_BOOT_OPTIONS_STATE_RESPONSE   Response;
} GET_UEFI_BOOT_OPTIONS_STATE_BUFFER;

//
// Boot Options Types
//
typedef enum {
  Https,
  Pba,
  WinRe
} BOOT_OPTION_TYPE;


// Max Description Length of UEFI Boot Option
#define MAX_UEFI_BOOT_OPTION_DESC_LENGTH     100

// Max Device Path Length of UEFI Boot Option
#define MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH 300

// Max Number of UEFI Boot Options
#define MAX_UEFI_BOOT_OPTIONS                10

//
// UEFI Boot Option Stucture
//
typedef struct {
  UINT32  UefiBootOptionType;                                   ///< Type of UEFI Boot Option
  UINT16  DescriptionLength;                                    ///< Length of Description
  UINT8   Description[MAX_UEFI_BOOT_OPTION_DESC_LENGTH];        ///< Description
  UINT16  DevicePathLength;                                     ///< Length of Device Path
  UINT8   EfiDevicePath[MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH];  ///< EFi Device Path
} UEFI_BOOT_OPTION;

//
// Update UEFI Boot Options Message Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;                       ///< ASF Message Header
  UINT16                NumOfUefiBootOptions;            ///< Number of UEFI Boot Options
  UEFI_BOOT_OPTION      UefiBootOptions[0];              ///< UEFI Boot Options
} UPDATE_UEFI_BOOT_OPTIONS;

//
// Update UEFI Boot Options Response Structure
//
typedef struct {
  ASF_MESSAGE_HEADER    AsfHeader;       ///< ASF Message Header
  AMT_STATUS            Status;          ///< AMT Status
} UPDATE_UEFI_BOOT_OPTIONS_RESPONSE;

//
// Update UEFI Boot Options Union Buffer
//
typedef union {
  UPDATE_UEFI_BOOT_OPTIONS          Request;
  UPDATE_UEFI_BOOT_OPTIONS_RESPONSE Response;
} UPDATE_UEFI_BOOT_OPTIONS_BUFFER;

//
// Get UEFI Boot Parameters Message structure
//
typedef struct {
  ASF_MESSAGE_HEADER  AsfHeader;    ///< ASF Message Header
  UINT32              Nonce;        ///< Nonce
} GET_UEFI_BOOT_PARAMETERS;


// Max Size of UEFI Boot Parameter Array
#define MAX_UEFI_BOOT_PARAMETERS_ARRAY_SIZE 1024

//
// UEFI Boot Parameter Option Structure
//
typedef struct {
  UINT32                UefiBootOptionType;                                   ///< Type of UEFI Boot Option
  UINT16                EfiDevicePathLen;                                     ///< Length of EFI Device Path
  UINT8                 EfiDevicePath[MAX_UEFI_BOOT_OPTION_DEV_PATH_LENGTH];  ///< EFI Device Path
  UINT32                NumberOfParams;                                       ///< Number of Parameters
  UINT32                LengthOfParamsArray;                                  ///< Length of Parameter Array
  UINT8                 ParametersArray[MAX_UEFI_BOOT_PARAMETERS_ARRAY_SIZE]; ///< Parameter Array
} UEFI_BOOT_OPTION_PARAMETER;

//
// Get UEFI Boot Parameters Response structure
//
typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;                ///< ASF Message Header
  UINT32                          Nonce;                    ///< Nonce
  UEFI_BOOT_OPTION_PARAMETER      UefiBootOptionWithParams; ///< Uefi Boot Option Parameters
} GET_UEFI_BOOT_OPTION_PARAMETERS_RESPONSE;

//
// Get UEFI Boot Parameters Union Buffer
//
typedef union {
  GET_UEFI_BOOT_PARAMETERS                        Request;
  GET_UEFI_BOOT_OPTION_PARAMETERS_RESPONSE        Response;
} GET_UEFI_BOOT_OPTION_PARAMETERS_BUFFER;

//
//  Get Root Certificate Authority(CA) Certificate Message structure
//
typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;        ///< ASF Message Header
  UINT32                          Nonce;            ///< Nonce
  UINT32                          RootCACertIndex;  ///< Index of Root CA Certificate
} GET_ROOT_CA_CERTIFICATE;

// Max Number of Root Certificate Authority(CA) Certificate
#define MAX_NUM_ROOT_CA_CERTIFICATE       4

// Max Size of a Root Certificate Authority(CA) Certificate
#define MAX_ROOT_CA_CERTIFICATE_SIZE      4100

//
// Root Certificate Authority(CA) Certificate Stucture
//
typedef struct {
  UINT32                        CertSize;                             ///< Size of Certificate
  UINT8                         Cert[MAX_ROOT_CA_CERTIFICATE_SIZE];   ///< Certificate
  UINT32                        LastCert;                             ///< Is this the Last Certificate
} ROOT_CA_CERTIFICATE;

//
//  Get Root Certificate Authority(CA) Certificate Response structure
//
typedef struct {
  ASF_MESSAGE_HEADER              AsfHeader;      ///< ASF Message Header
  UINT32                          Nonce;          ///< Nonce
  ROOT_CA_CERTIFICATE             Cert;           ///< Root CA Certificate
} GET_ROOT_CA_CERTIFICATE_RESPONSE;

//
// Get Root CA Certificate Union Buffer
//
typedef union {
  GET_ROOT_CA_CERTIFICATE                 Request;
  GET_ROOT_CA_CERTIFICATE_RESPONSE        Response;
} GET_ROOT_CA_CERTIFICATE_BUFFER;

//
// Messaging Command Definitions
//

///
/// ASF Message
///
typedef struct {
  UINT8 SubCommand;                     ///< ASF Message Sub-command.
  UINT8 Version;                        ///< ASF Specification Version.
  UINT8 EventSensorType;                ///< The Event Sensor Type field indicates what types of events the sensor is monitoring.
  UINT8 EventType;                      ///< Code indicating what type of transition/state change triggered the trap.
  UINT8 EventOffset;                    ///< Indicates which particular event occurred for a given Event Type.
  UINT8 EventSourceType;                ///< Class of device or type of software that originated the event.
  UINT8 EventSeverity;                  ///< Severity of the event.
  UINT8 SensorDevice;                   ///< Identifies the instance of the device that holds the sensor that generated the event.
  UINT8 SensorNumber;                   ///< Identify a given instance of a sensor relative to the Sensor Device.
  UINT8 Entity;                         ///< Indicates the platform entity the event is associated with.
  UINT8 EntityInstance;                 ///< Indicates which instance of the Entity the event is for.
  UINT8 EventData1;                     ///< Additional parametric data 1.
  UINT8 EventData2;                     ///< Additional parametric data 2.
} ASF_MESSAGE;

///
/// ASF Push Progress Code
///
typedef struct {
  UINT8           Command;              ///< ASF Message Command.
  UINT8           ByteCount;            ///< ASF Message Bytes Count Beyond This Byte.
  ASF_MESSAGE     AsfMessage;           ///< ASF Message Instance.
  UINT8           EventData[3];         ///< Additional parametric data.
} HECI_ASF_PUSH_PROGRESS_CODE;

///
/// ASF Framework Message
///
typedef struct {
  ASF_FRAMEWORK_MESSAGE_TYPE      MessageType;      ///< ASF Message Type.
  ASF_MESSAGE                     Message;          ///< ASF Message Instance.
} ASF_FRAMEWORK_MESSAGE;

///
/// Data Hub of ASF Progress Status
///
typedef struct {
  ASF_FRAMEWORK_MESSAGE_TYPE      MessageType;      ///< ASF Message Type.
  EFI_STATUS_CODE_VALUE           StatusCodeValue;  ///< Platform Progress Status Code.
} ASF_DATA_HUB_MAP;

///
/// Platform Event Trap (PET) Alert To Indicate Which Event Causes The Alert.
/// This structure is used for sending Remote Secure Erase / Boot Audit Entry PET Message.
/// Elements' description is based on Remote Secure Erase / Boot Audit Entry.
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;         ///< ASF Message Header.
  UINT8                EventSensorType;   ///< Default value is 0x0F to indicate it is Firmware progress/error type.
  UINT8                EventType;         ///< Default value is 0x6F to indicate it is Sensor specific.
  /**
    Indicates which particular event occurred for a given Event Type.
    For Remote Secure Erase:
      0x03: OEM specific firmware error
      0x05: OEM specific firmware progress
    For Boot Audit Entry:
      0x00: System Firmware Error
      0x01: System Firmware Hang
      0x02: System Firmware In Progress
  **/
  UINT8                EventOffset;
  UINT8                EventSourceType;   ///< Default value is 0x68 to indicate it is ASF 1.0.
  /**
    Severity of the event.
      0x01: Monitor
      0x02: Information
      0x04: Ok
      0x08: Non-critical
      0x10: Critical
      0x20: Non-Recoverable
  **/
  UINT8                EventSeverity;
  UINT8                SensorDevice;      ///< Default value is 0xFF to indicate it is Unspecified.
  UINT8                Sensornumber;      ///< Default value is 0xFF to indicate it is Unspecified.
  /**
    Indicates the platform device or subsystem associated with the event.
      0x00: Unspecified
      0x01: Other
      0x02: Unknown
      0x03: Processor
      0x04: Disk
      0x05: Peripheral
      0x07: System Board
      0x0A: Power Supply
      0x0B: Add-in Card
      0x20: Memory
      0x22: BIOS
      0x23: OS
  **/
  UINT8                Entity;
  /**
    Identifies which unique device is associated with the event when a system includes multiple device instances.
    For Remote Secure Erase:
        0xFF: Unspecified
    For Boot Audit Entry:
        0x00: Unspecified
  **/
  UINT8                EntityInstance;
  /**
    The Event Type determines the Event Data.
    For Remote Secure Erase:
        0xAA: Indicate EventData 2-5 set by OEM specification.
    For Boot Audit Entry:
        0x40: EventData 2 conforms ASF spec.
        0xAA: Indicate EventData 2-5 set by OEM specification.
  **/
  UINT8                EventData1;
  /**
    Additional Event Data 2.
    For Remote Secure Erase:
        0x20: Indicate it is for Remote Secure Erase.
    For Boot Audit Entry:
        0x13: Performing system boot.
  **/
  UINT8                EventData2;
  /**
    Additional Event Data 3.
    For Remote Secure Erase:
      Success Case: 0x00 - Secure erase success
      Failure Case: 0x01 - General failure
                    0x02 - Drive authentication failure
                    0x03 - Feature is not supported
                    0x04 - 0XFF - Reserved
    For Boot Audit Entry (Boot Device Not Changed):
      00: Not used.
    For Boot Audit Entry (Boot Device Changed):
      N/A: Most recent BAE queue entry.
  **/
  UINT8                EventData3;
  /**
    Additional Event Data 4.
    For Remote Secure Erase:
      N/A: Not used.
    For Boot Audit Entry (Boot Device Not Changed):
      N/A: Not used.
    For Boot Audit Entry (Boot Device Changed):
      N/A: Next most recent BAE queue entry.
  **/
  UINT8                EventData4;
  /**
    Additional Event Data 5.
    For Remote Secure Erase:
      N/A: Not used.
    For Boot Audit Entry (Boot Device Not Changed):
      N/A: Not used.
    For Boot Audit Entry (Boot Device Changed):
      N/A: Oldest BAE queue entry.
  **/
  UINT8                EventData5;
} PET_ALERT;

//
// LAN Command Definitions
//

///
/// Set Active LAN Interface Request Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;        ///< ASF Message Header.
  UINT8                ActiveInterface;  ///< 0:As defined in FIT; 1:Integrated LAN; 2:Discrete LAN.
} AMT_SET_ACTIVE_LAN_INTERFACE;

///
/// Set Active LAN Interface Response Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;         ///< ASF Message Header.
  UINT32               Status;            ///< Response status.
} AMT_SET_ACTIVE_LAN_INTERFACE_RESPONSE;

///
/// Set Active LAN Interface Union Buffer
///
typedef union {
  AMT_SET_ACTIVE_LAN_INTERFACE          Request;
  AMT_SET_ACTIVE_LAN_INTERFACE_RESPONSE Response;
} AMT_SET_ACTIVE_LAN_INTERFACE_BUFFER;


///
/// Get Active LAN Interface Request Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;        ///< ASF Message Header.
} AMT_GET_ACTIVE_LAN_INTERFACE;

///
/// Get Active LAN Interface Response Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;         ///< ASF Message Header.
  UINT32               Status;            ///< Response status.
  UINT8                ActiveInterface;   ///< 0:As defined in FIT; 1:Integrated LAN; 2:Discrete LAN.
  UINT32               Reserved[4];       ///< Reserved for feature use.
} AMT_GET_ACTIVE_LAN_INTERFACE_RESPONSE;

///
/// Get Active LAN Interface Union Buffer
///
typedef union {
  AMT_GET_ACTIVE_LAN_INTERFACE          Request;
  AMT_GET_ACTIVE_LAN_INTERFACE_RESPONSE Response;
} AMT_GET_ACTIVE_LAN_INTERFACE_BUFFER;


//
// KVM Command Definitions
//

///
/// AMT Query KVM timer related definition
///
#define KVM_STALL_1_SECOND  1000000   ///< Stall 1 second
#define KVM_MAX_WAIT_TIME   (60 * 8)  ///< 8 Mins

///
/// Query KVM Response Type
///
typedef enum {
  KvmSessionEstablished  = 1,
  KvmSessionCancelled
} RESPONSE_CODE;

///
/// Query KVM Response Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;         ///< ASF Message Header.
  /**
    Response Code of KVM Session Status.
      0x01: the KVM session established
      0x02: the KVM session cancelled
  **/
  UINT32               ResponseCode;
} AMT_QUERY_KVM_RESPONSE;

///
/// Query KVM Type
///
typedef enum {
  QueryRequest            = 0,
  CancelRequest
} QUERY_TYPE;

///
/// Query KVM Request Structure
///
typedef struct {
  ASF_MESSAGE_HEADER   AsfHeader;         ///< ASF Message Header.
  /**
    KVM Session Query Type.
      0x00: Query the KVM session request
      0x01: Cancel the KVM session request
  **/
  UINT32               QueryType;
} AMT_QUERY_KVM;

///
/// Query KVM Union Buffer
///
typedef union {
  AMT_QUERY_KVM          Request;
  AMT_QUERY_KVM_RESPONSE Response;
} AMT_QUERY_KVM_BUFFER;

//
// MAC Command Definitions
//

///
/// Set MAC Passthrough
///
typedef struct {
  ASF_MESSAGE_HEADER AsfHeader;         ///< ASF Message Header.
  UINT32             Status;
} AMT_SET_MAC_PASSTHROUGH_RESPONSE;

#define MAC_ADDRESS_SIZE 6

typedef struct {
  ASF_MESSAGE_HEADER AsfHeader;         ///< ASF Message Header.
  UINT8              Enabled;
  UINT8              MacAddress[MAC_ADDRESS_SIZE];
} AMT_SET_MAC_PASSTHROUGH;

///
/// Set MAC Passthrough Buffer
///
typedef union {
  AMT_SET_MAC_PASSTHROUGH          Request;
  AMT_SET_MAC_PASSTHROUGH_RESPONSE Response;
} AMT_SET_MAC_PASSTHROUGH_BUFFER;

#pragma pack()

#endif // _ASF_MSGS_H
