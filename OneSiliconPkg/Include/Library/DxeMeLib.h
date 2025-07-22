/** @file
  Header file for functions to get Intel ME information

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
#ifndef _DXE_ME_LIB_H_
#define _DXE_ME_LIB_H_

#include <Protocol/HeciControlProtocol.h>
#include <Protocol/PlatformMeHook.h>
#include <Protocol/MePolicy.h>
#include <MeDefines.h>
#include <MeState.h>
#include <MeiBusMsg.h>
#include <IshMsgs.h>
#include <PsrMsgs.h>
#include <BupMsgs.h>
#include <MkhiMsgs.h>
#include <MchiMsgs.h>
#include <IccMsgs.h>
#include <UpidMsgs.h>

#define MAX_ASSET_TABLE_ALLOCATED_SIZE  0x3000
#define HWA_TABLE_PUSH_CMD              0

#pragma pack(push, 1)

///
/// Definitions for Hardware Asset Header
///
typedef union {
  UINT32  Data;
  struct {
    UINT32  MessageLength      : 16;
    UINT32  Command            : 4;      ///< only supported command would be HWA_TABLE_PUSH_CMD =0;
    UINT32  PciFruTablePresent : 1;
    UINT32  MediaTablePresent  : 1;
    UINT32  SmbiosTablePresent : 1;
    UINT32  AsfTablePresent    : 1;
    UINT32  Reserved           : 8;
  } Fields;
} HWA_MESSAGE_HEADER;

///
/// Hardware Asset Table Index
///
typedef enum {
  HWA_TABLE_TYPE_INDEX_FRU_DEVICE  = 0,
  HWA_TABLE_TYPE_INDEX_MEDIA_DEVICE,
  HWA_TABLE_TYPE_INDEX_SMBIOS,
  HWA_TABLE_TYPE_INDEX_ASF,
  HWA_TABLE_TYPE_INDEX_MAX
} HWA_TABLE_TYPE_INDEX;

typedef struct {
  UINT16  Offset;
  UINT16  Length;
} SINGLE_TABLE_POSITION;

typedef struct {
  SINGLE_TABLE_POSITION Table[HWA_TABLE_TYPE_INDEX_MAX];
  UINT8                 TableData[0];
} HWA_TABLE;

///
/// Asset Update Table Message
///
typedef struct {
  HWA_MESSAGE_HEADER Header;
  HWA_TABLE          Data;
} HWA_PUSH_TABLES_MSG;

#pragma pack(pop)

//
// FW Feature Shipment Time State Override bitmasks definitions
//
#define CLEAR_FEATURES_BITMASK 0x00000000
#define AMT_BITMASK            BIT2
#define ISH_BITMASK            BIT10
#define PTT_BITMASK            BIT29

//
// Prototype for HECI message DXE phase
//

//
// ME Client - HBM
//

/**
  Checks if CSME supports HBM protocol version 2.2.

  @param[in] HeciDev                HECI device number

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
  @retval EFI_INCOMPATIBLE_VERSION  CSME does not support protocol version required by BIOS
**/
EFI_STATUS
HbmHostVersion (
  IN  HECI_DEVICE HeciDev
  );

/**
  Instructs CSME to stop all communication on given Heci device

  @param[in] HeciDev                HECI device number

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmHostStop (
  IN  HECI_DEVICE HeciDev
  );

/**
  Checks at which CSME addresses there are any clients

  @param[in]  HeciDevice            HECI device number
  @param[out] ValidAddresses        Mask of addresses that have a client behind them

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmHostEnumRequest (
  IN  HECI_DEVICE   HeciDevice,
  OUT UINT8         *ValidAddresses
  );

/**
  Retrieves GUID of a single CSME client

  @param[in]  HeciDevice            HECI device number
  @param[in]  ClientAddr            Address of the CSME client
  @param[out] ProtocolGuid          GUID of the client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmGetClientProperties (
  IN  HECI_DEVICE HeciDevice,
  IN  UINT8       ClientAddr,
  OUT EFI_GUID*   ProtocolGuid
  );

/**
  Creates a connection between given host and CSME addresses

  @param[in] HeciDev                HECI device number
  @param[in] MeAddress              Address of the CSME client
  @param[in] HostAddress            Address of the Host client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmHostClientConnect (
  IN HECI_DEVICE HeciDev,
  IN UINT8       MeAddress,
  IN UINT8       HostAddress
  );

/**
  Disconnects an established connection between a Host and CSME clients

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Host Address of the connection
  @param[in] MeAddr                 CSME Address of the connection

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmClientDisconnect (
  IN  HECI_DEVICE HeciDev,
  IN  UINT8       HostAddr,
  IN  UINT8       MeAddr
  );

/**
  Sends a flow control token from BIOS to CSME that allows CSME to send one message over connection
  identified by host:Me address pair

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Address of the Host client
  @param[in] MeAddr                 Address of the CSME client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmSendFlowControlToken (
  IN HECI_DEVICE   HeciDev,
  IN UINT8         HostAddr,
  IN UINT8         MeAddr
  );

/**
  Attempts to receive a flow control token from CSME. This token allows BIOS to send one message over commection
  identified by Host:Me address pair

  @param[in] HeciDev                HECI device number
  @param[in] HostAddr               Address of the Host client
  @param[in] MeAddr                 Address of the CSME client

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HbmGetFlowControlToken (
  IN HECI_DEVICE HeciDev,
  IN UINT8       HostAddr,
  IN UINT8       MeAddr
  );

/**
  Informs about Host and CSME capabilities over HBM

  @param[in]      HeciDev           HECI device number
  @param[in, out] HbmCaps             On input:  Host HBM capabilities
                                      On output: CSME HBM capabilities

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_INVALID_PARAMETER     NULL parameter
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmCapabilitiesRequest (
  IN     HECI_DEVICE      HeciDev,
  IN OUT HBM_CAPS_DATA    *HbmCaps
  );

/**
  Requests mapping CSME HECI client MMIO to the given physcial memory addresses.

  @param[in] HeciDev                HECI device number
  @param[in] ClientDmaBufferId      Identifies Client Id to which physical address should be mapped
  @param[in] AddressLow             Physical address for DMA buffer (Low DWORD)
  @param[in] AddressHigh            Physical address for DMA buffer (High DWORD)
  @param[in] Size                   Size of the physical memory to be mapped

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmMapRequest (
  IN HECI_DEVICE            HeciDev,
  IN UINT8                  ClientDmaBufferId,
  IN UINT32                 AddressLow,
  IN UINT32                 AddressHigh,
  IN UINT32                 Size
  );

/**
  Requests unmapping CSME HECI client MMIO from the given physcial memory addresses.

  @param[in] HeciDev                HECI device number
  @param[in] ClientDmaBufferId      Identifies Client Id from which physical address should be unampped

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_ABORTED               Wrong response was received
**/
EFI_STATUS
HbmUnmapRequest (
  IN HECI_DEVICE HeciDev,
  IN UINT8       ClientDmaBufferId
  );

//
// ME Client - ISH Service
//

/**
  This message is sent by the BIOS when it wants to send
  PDT Unlock Message to ISH.

  @param[in] FileName             File name of bios2ish file.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciPdtUnlockMsg (
  IN CONST CHAR8*                FileName
  );

/**
  This message is sent by the BIOS when it wants to send
  PDT Update Message

  @param[in] FileName             File name of bios2ish file.
  @param[in] PdtFileBuffer        Contains ISH PDT file
  @param[in] PdtBufferSize        Size of PDT file

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_ABBORTED            Unable to allocate memory
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciPdtUpdateMsg (
  IN CONST CHAR8*  FileName,
  IN UINT8         *PdtFileBuffer,
  IN UINT16        PdtBufferSize
  );

/**
  This message is sent by the BIOS when it wants to send
  Sensor Control Message to ISH.

  @param[in] FileName                   File name of bios2ish file.
  @param[in] SensorExposurePayload      Table of Sensor Exposure settings given as buffer
  @param[in] SensorExposurePayloadSize  SensorExposurePayload size

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate memory
  @retval EFI_BAD_BUFFER_SIZE     The size of FileName is larger than maximum allowed value
**/
EFI_STATUS
HeciSensorControlMsg (
  IN CONST CHAR8*               FileName,
  IN UINT8                      *SensorExposurePayload,
  IN UINT16                     SensorExposurePayloadSize
  );

//
// ME Client - PSR
//

/**
  Inform CSME of Psr Capability Set.

  @param[in] CapabilityMask       Bit mask of all capabilities supported by PSR feature.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendPsrCapabilitySet (
  IN UINT32 CapabilityMask
  );

/**
  Get Platform Service Record from CSME

  @param[in, out] PsrData         Pointer to PSR data struct

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPsrRecord (
  IN OUT PSR_DATA *PsrData
  );

/**
  PSR Report Event

  @param[in] EventId       Different event ID for each event
  @param[in] EventData     Data payload

  @retval EFI_UNSUPPORTED  Current ME mode doesn't support this function
  @retval EFI_SUCCESS      Command succeeded
**/
EFI_STATUS
HeciEventReport (
  IN UINT32 EventId,
  IN UINT32 EventData
  );

/**
  Inform CSME of Chassis Intrusion Detected Event

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendChassisIntrDetEvent (
  VOID
  );

/**
  Inform CSME of Critical Temperature Shutdown Event

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendCriticalTempShutdownEvent (
  VOID
  );

/**
  Report Device Erase Event

  @param[in] DeviceEraseReport    Device Erase Report Data

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
**/
EFI_STATUS
HeciDeviceEraseReport (
  IN PSR_DEVICE_ERASE_REPORT      *DeviceEraseReport
  );

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin          Reset source
  @param[in] ResetType            Global or Host reset

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  );

/**
  Retrieve invocation codes from Firmware.

  @param[out] InvocationCode        Bitmask of invocation codes set by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetInvocationCode (
  OUT UINT32 *InvocationCode
  );

/**
  Set invocation codes in Firmware.

  @param[in] InvocationCode         Bitmask of invocation codes to be set by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetInvocationCode (
  IN UINT32 InvocationCode
  );

/**
  Clear invocation codes in Firmware.

  @param[in] InvocationCode         Bitmask of invocation codes to be cleared by Firmware.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciClearInvocationCode (
  IN UINT32 InvocationCode
  );

//
// MKHI_PWD_GROUP_ID = 0x02
//

/**
  This function logs in with current password and updates to new password.

  @param[in] OldPwdSize             Size of the old password
  @param[in] OldPwd                 Buffer holding old password
  @param[in] NewPwdSize             Size of the new password
  @param[in] NewPwd                 Buffer holding new password

  @retval ME_PWD_STATUS             Password management Status Code:
                                      @see ME_PWD_STATUS
**/
ME_PWD_STATUS
HeciModifyMebxPassword (
  IN UINT8 OldPwdSize,
  IN UINT8 *OldPwd,
  IN UINT8 NewPwdSize,
  IN UINT8 *NewPwd
  );

/**
  This function uses the User entered password to login into FW.

  @param[in]  PasswordSize          Size of the user entered password
  @param[in]  Password              Buffer holding user enetered password
  @param[out] MaxRetries            Informs if maximum number of login retries have been reached

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_INVALID_PARAMETER     Null buffer
  @retval EFI_DEVICE_ERROR          Command failed
**/
EFI_STATUS
HeciValidateMebxPassword (
  IN     UINT8   PasswordSize,
  IN     UINT8   *Password,
     OUT BOOLEAN *MaxRetries
  );

/**
  Checks if password was modified.

  @param[out] Modified              Password modification status

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
HeciIsMebxPasswordModified (
  OUT UINT32 *Modified
  );

//
// MKHI_FWCAPS_GROUP_ID = 0x03
//

/**
  Send Get Rule Data Request to CSME

  @param[in]   RuleId               Identifies the rule which data is requested.
  @param[out]  *RuleData            Pointer to requested rule data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid.
**/
EFI_STATUS
HeciFwCapsGetRuleData (
  IN  UINT32                      RuleId,
  OUT UINT32                      *RuleData
  );

/**
  Send Get Firmware Capabilities Request to CSME

  @param[out] *FwCapsSku            Firmware Capabilities

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU               *RuleData
  );

/**
  Send Get Platform State Request to CSME
  @param[out] RuleData              Platform State
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatformStateMsg (
  OUT UINT32                      *RuleData
  );

/**
  This message is sent by the BIOS prior to the End of Post (EOP) on the boot
  where host wants to query the local firmware update interface status.

  @param[out] RuleData              2 - Partial and full firmware update is disabled
                                    1 - Firmware update interface enable
                                    0 - Firmware update interface disable

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwUpdateState (
  OUT UINT32                     *RuleData
  );

/**
  Send Get User Capabilities State Request to ME.

  @param[out] RuleData           User Capabilities State

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUserCapabilitiesState (
  OUT MEFWCAPS_SKU *RuleData
  );

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to get Ibex Peak platform type.
  One of usages is to utilize this command to determine if the platform runs in
  Consumer or Corporate SKU size firmware.

  @param[out] RuleData            PlatformBrand,
                                  IntelMeFwImageType,
                                  SuperSku,
                                  PlatformTargetUsageType

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  );

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS/IntelR MEBX to get firmware runtime status.
  The GET FW RUNTIME STATUS message doesn't need to check the HFS.FWInitComplete
  value before sending the command.
  It means this message can be sent regardless of HFS.FWInitComplete.

  @param[out] *RuleData             MEFWCAPS_SKU message

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  );

/**
  Get available ME features.

  @param[out] FwFeatures            Mask of available ME Features

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwAvailableFeatures (
  OUT UINT32 *FwFeatures
  );

/**
  The firmware will respond to GET OEM TAG message even after the End of Post (EOP).

  @param[out] RuleData            Default is zero. Tool can create the OEM specific OEM TAG data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetOemTagMsg (
  OUT UINT32                      *RuleData
  );

/**
  This message is sent by the BIOS when it wants to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is not disabled

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUnconfigOnRtcClearDisableMsg (
  OUT UINT32                *RuleData
  );

/**
  This message is sent by the BIOS on the boot where the host wants to get the firmware provisioning state.
  The firmware will respond to AMT BIOS SYNC INFO message even after the End of Post.

  @param[out] RuleData            @see AMT_BIOS_SYNC_INFO

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  );

/**
  Get User Consent requirement.

  @param[out] IsUserConsentRequired     User consent requirement:
                                          TRUE  - User Consent required
                                          FALSE - User Consent not required

  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetIsUserConsentRequired (
  OUT BOOLEAN *IsUserConsentRequired
  );

/**
  Return information if Client Initiated Remote Access (CIRA) is available.

  @retval TRUE    CIRA is available
  @retval FALSE   CIRA isn't available or cannot get infomation
**/
BOOLEAN
IsCiraAvailable (
  VOID
  );

/**
  Send Set Rule Data Request to CSME

  @param[in]   RuleId             Identifies the rule which data needs to be changed.
  @param[in]   RuleDataLength     Rule Data Length.
  @param[in]   RuleData           Pointer to new rule data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  );

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to disable the ME State. The firmware allows a single
  update once it receives the disable command Once firmware receives this message,
  the firmware will work in "Soft Temporary Disable" mode (HFS[19:16] = 3) after a
  global reset. Note, this message is not allowed when AT firmware is enrolled/configured.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetMeDisableMsg (
  VOID
  );

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP) on the boot
  where host wants to enable or disable the local firmware update interface.
  The firmware allows a single update once it receives the enable command

  @param[in] RuleData             1 - local firmware update interface enable
                                  0 - local firmware update interface disable

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFwUpdateState (
  IN UINT8                        RuleData
  );

/**
  Send Set User Capabilities State Request to ME.

  @param[in] MEFWCAPS_SKU        User Capabilities State

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL   Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUserCapabilitiesState (
  IN MEFWCAPS_SKU RuleData
  );

/**
  Set FW Enabled Features.

  @param[in] NewFeatures          New Fw Enabled Features
  @param[in] FeaturesMask         Bitmask with new Fw Enabled Features

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFwEnabledFeatures (
  IN UINT32 NewFeatures,
  IN UINT32 FeaturesMask
  );

/**
  This message is sent by the BIOS to inform ME FW whether or not to take the
  TPM 1.2 Deactivate flow

  @param[in] TpmDeactivate        0 - ME FW should not take the deactivate flow.
                                  1 - ME FW should take the deactivate flow.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetAcmTpmData (
  IN UINT8               TpmDeactivate
  );

/**
  This message is sent by the BIOS when it wants to set
  ME Unconfig on RTC Clear Disable state.

  @param[in] RuleData             1 - Disable Unconfig on RTC clear
                                  0 - Enable Unconfig on RTC clear

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  );


//
// MKHI_HMRFPO_GROUP_ID = 0x05
//

/**
  Sends a message to ME to unlock a specified SPI Flash region for writing and receiving a response message.
  It is recommended that HMRFPO_ENABLE MEI message needs to be sent after all OPROMs finish their initialization.

  @param[out] Result              HMRFPO_ENABLE response

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoEnable (
  OUT UINT8                       *Result
  );

/**
  Sends a message to ME to lock a specified SPI Flash region for writing and receiving a response message.

  @param[out] Result              Status report

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_NOT_READY           HECI Device hasn't ready yet
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoLock (
  OUT UINT8                       *Result
  );

/**
  System BIOS sends this message to get status for HMRFPO_LOCK message.

  @param[out] Result              HMRFPO_GET_STATUS response

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciHmrfpoGetStatus (
  OUT UINT8                       *Result
  );

//
// MKHI_GEN_GROUP_ID = 0xF0
//

/**
  This message is sent by the BIOS or IntelR MEBX prior to the End of Post (EOP)
  on the boot where host wants to enable the ME State. The firmware allows a single
  update once it receives the enable command. Once firmware receives this message,
  the firmware will be in normal mode after a global reset.

  @retval EFI_SUCCESS             ME enabled message sent
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
HeciSetMeEnableMsg (
  VOID
  );

/**
  Send HMRFPO disable MKHI to CSME.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
HeciHmrfpoDisable (
  VOID
  );

/**
  Send command to terminate HECI communication in ME Debug Mode.

  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciTerminateHeciMsg (
  VOID
  );

/**
  This function sends a command to CSME to get Log information of Measurements provided in
  CSME Extend Registers for BIOS to extend to TPM-PCR registers.

  @param[out]     EventLogBuffer    Pointer to received Event log buffer
  @param[in]      RemainingBufSize  Size of available buffer to collect event log
  @param[out]     ReceivedLogSize   Size of received event log
  @param[in, out] LogEntryIndex     On input - Index of first / starting log entry in the buffer. On output - Index of last log entry returned
  @param[out]     RemainingEntries  Pointer to Remaining Entries (0 - no more entries in a log; 1 - additional iteration(s) is(are) required)

  @retval EFI_SUCCESS             Log received successfully
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
HeciGetErLog (
  OUT UINT8        *EventLogBuffer,
  IN  UINT32       RemainingBufSize,
  IN OUT UINT32    *ReceivedLogSize,
  IN OUT UINT8     *LogEntryIndex,
  OUT UINT8        *RemainingEntries
  );

/**
  This command is sent by the BIOS to clear CSME data to image creation state.
  After the command succeeds, platform reset is expected.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Get ME mode fail.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout or transition failed
                                  within allowed time limit
**/
EFI_STATUS
HeciDataClear (
  VOID
  );

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMkhiVersion (
  OUT MKHI_VERSION *MkhiVersion
  );

/**
  Send Get Firmware Version Request to ME

  @param[out] *GetFwVersionAckData  Return the message of FW version

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFwVersionMsg (
  OUT GET_FW_VER_ACK_DATA  *GetFwVersionAckData
  );

/**
  This message is sent by the BIOS to notify ME firmware that the BIOS is starting
  to run 3rd party code.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function or EOP was sent
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
HeciCoreBiosDoneMsg (
  VOID
  );

/**
  Send CPU brand class fuse to the CSME (only for corporate SKU).

  This message is required for BIOS running on CPUs don't support SID message to obtain CPU information.

  @param[in] CpuBrandClass        CPU brand class

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSendCpuBrandClassFuse (
  IN UINT8 CpuBrandClass
  );

/**
  Get CPU Type Change User Message.

  @param[in] CpuTypeChangeInfo    CPU Type Change User Message

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetCpuTypeChange (
  OUT GEN_GET_CPU_TYPE_CHANGE_DATA *CpuTypeChangeInfo
  );

/**
  Send CPU Type Change User Response.

  @param[in] UserResponse         CPU Type Change User Response:
                                    0 - Change Rejected
                                    1 - Change Accepted

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetCpuTypeChangeUserResponse (
  IN UINT8 UserResponse
  );

/**
  Unconfigure ME without password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciUnconfigWithoutPwd (
  VOID
  );

/**
  Get ME Unconfiguration Status.

  @param[out] UnconfigStatus      Unconfiguration Status:
                                    0x01 - Unconfiguration in progress
                                    0x02 - No unconfigure w/o password to be processed
                                    0x03 - CSME finished processing unconfigure w/o password
                                    0x80 - Error encountered by CSME during unconfigure w/o password

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUnconfigureStatus (
  OUT UINT8 *UnconfigStatus
  );

/**
  Send Set Manufacturing Me Reset and Halt Request to ME

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetManufacturingMeResetAndHalt (
  VOID
  );

/**
  Sends the MKHI Enable/Disable FW Features.

  @param[in] EnableBitMask        Bitmask of features to enable
  @param[in] DisableBitMask       Bitmask of features to disable

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
  @retval EFI_ACCESS_DENIED       Command request is rejected
**/
EFI_STATUS
HeciFwFeatureStateOverride (
  IN UINT32                       EnableBitmask,
  IN UINT32                       DisableBitmask
  );

/**
  Send Get Image Firmware Version Request to ME

  @param[in]  PartitionId          Partition ID
  @param[out] NumOfModules         Return the number of modules in the partition
  @param[out] PartitionIdData      Return the version of the Partition ID

  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetImageFwVersionMsg (
  IN  UINT32                    PartitionId,
  OUT UINT32                    *NumOfModules,
  OUT FLASH_PARTITION_DATA      *PartitionIdData
  );

/**
  Send Set FIPS Mode to Enabled or Disabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFipsMode (
  IN UINT32 FipsMode
  );

/**
  Send Get Current FIPS Mode and Crypto Driver version

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetFipsMode (
  OUT GET_FIPS_MODE_DATA  *GetFipsModeData
  );

/**
  Send Set Measured Boot State to Enable or Disable CSME Measurement

  @param[in]  SetMeasuredBootState  Set Measured boot state to Enable/Disable CSME Extended Measured boot

  @retval EFI_UNSUPPORTED         Current CSME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetMeasuredBootState (
  IN UINT8 SetMeasuredBootState
  );

/**
  Send Get Current CSME Measured Boot State

  @param[out] GetMeasuredBootState    Pointer to get CSME Measured boot status.

  @retval EFI_UNSUPPORTED         Current (CS)ME mode/version doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetMeasuredBootState (
  OUT UINT8  *GetMeasuredBootState
  );

/**
  This message is sent by the BIOS to retrieve data from SPI

  @param[in] SpiReadType           Requested SPI read type
  @param[in] SpiDataLength         Length of the requested SPI data
  @param[out] SpiData              Retrieved SPI data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciReadFromSpi (
  IN  UINT32                    SpiReadType,
  IN  UINT32                    SpiDataLength,
  OUT UINT32                    *SpiData
  );

/**
  This message is sent by the BIOS to retrieve Primary Access data from SPI

  @param[out] SpiData              Retrieved Primary Access data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadPrimaryAccess (
  OUT SPI_DESC_PRIMARY_DATA *SpiData
  );

/**
  This message is sent by the BIOS to retrieve GBE Version data from SPI

  @param[out] SpiData              Retrieved GBE Version data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadGbeVersion (
  OUT SPI_GBE_VERSION *SpiData
  );

/**
  This message is sent by the BIOS to retrieve Flash Info data from SPI

  @param[out] SpiData              Retrieved Flash Info data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadFlashInfo (
  OUT SPI_FLASH_INFO *SpiData
  );

/**
  This message is sent by the BIOS to retrieve Descriptor Resilience Info data from SPI

  @param[out] SpiData              Retrieved Flash Info data

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES     Unable to allocate required resources
**/
EFI_STATUS
HeciSpiReadDescriptorResilienceInfo (
  OUT SPI_DESCRIPTOR_RESILIENCE_INFO *SpiData
  );

//
// ME Client - Hardware Asset
//

/**
  Send Hardware Asset Tables to Firmware

  @param[in] AssetTableData       Hardware Asset Table Data
  @param[in] TableDataSize        Size of Asset table

  @retval EFI_SUCCESS             Table sent
  @retval EFI_ABORTED             Could not allocate Memory
**/
EFI_STATUS
HeciAssetUpdateFwMsg (
  IN HWA_TABLE *AssetTableData,
  IN UINT16    TableDataSize
  );

//
// ME Client - Manufacturing and Configuration Host Interface (MCHI)
//

//
// MCHI_MCA_GROUP_ID = 0x0A
//

/**
  This message is sent by the BIOS to retrieve file stored in ME firmware NVM.

  @param[in]      FileId          Id number of file to read
  @param[in]      Offset          File offset
  @param[in, out] DataSize        On input - size of data to read, on output - size of read data
  @param[in]      Flags           Flags
  @param[out]     Buffer          Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciReadFileExMsg (
  IN UINT32      FileId,
  IN UINT32      Offset,
  IN OUT UINT32  *DataSize,
  IN UINT8       Flags,
  OUT UINT8      *DataBuffer
  );

/**
  Read Delayed Authentication Mode from ME NVM

  @param[out] DamState            Delayed Authentication Mode State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetDamState (
  OUT UINT8 *DamState
  );

/**
  Read UPID Platform Id feature supported state from ME NVM

  @param[out] PlatIdSupported     Platform Id feature supported State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetPlatIdSupportedState (
  OUT UINT8  *PlatIdSupported
  );

/**
  Read Qualified Brand Identity support from ME NVM

  @param[out] QBEState            Qualified Brand Identity State

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetQualifiedBrandEntitlements (
  OUT UINT8 *QBEState
  );

/**
  This message is sent by the BIOS to save data in a file stored in ME firmware NVM.

  @param[in]   FileId             Id Number of file to write
  @param[in]   Offset             File offset
  @param[in]   DataSize           Size of data to write
  @param[in]   Flags              Flags
  @param[in]   DataBuffer         Pointer to the data buffer

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetFileExMsg (
  IN UINT32 FileId,
  IN UINT32 Offset,
  IN UINT32 DataSize,
  IN UINT8  Flags,
  IN UINT8  *DataBuffer
  );

/**
  Set Delayed Authentication Mode in ME NVM

  @param[in] DamState             New Delayed Authentication Mode State:
                                    0 - DAM Disable
                                    1 - DAM Enable

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_ABORTED             Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetDamState (
  IN UINT8 DamState
  );

/**
  This message is sent by the BIOS after HeciSetFileExMsg to notify ME firmware that it needs to
  commit previously set files.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciCommitFilesMsg (
  VOID
  );

/**
  This message is sent by the BIOS to retrieve RPMC status data from CSME.

  @param[out] RpStatus            RPMC status data

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support send this message through this HECI
  @retval EFI_INVALID_PARAMETER   Parameter invalid
**/
EFI_STATUS
HeciGetRpmcStatus (
  OUT RP_STATUS_3         *RpStatus
  );

/**
  This command indicates to the FW that it shall commit ARBSVN to fuse.

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
**/
EFI_STATUS
HeciArbSvnCommitMsg (
  VOID
  );

/**
  The command retrieves anti-replay SVN information.
  Caller can set Entries as 0 to get the correct number of entries CSME contains.

  @param[in, out] Entries         On input, it is the number of entries caller expects.
                                  On output, it indicates the number of entries CSME contains.
  @param[in, out] ArbSvnInfo      Anti-Rollback SVN Information

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources
  @retval EFI_BUFFER_TOO_SMALL    The Entries is too small for the result
**/
EFI_STATUS
HeciArbSvnGetInfoMsg (
  IN OUT UINT32                *Entries,
  IN OUT ARB_SVN_INFO_ENTRY    *ArbSvnInfo
  );

/**
  Send Get Manufacturing Info Request to CSME

  @param[out] *ManufacturingInfo    Retrieved manufacturing info data.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Parameter invalid.
**/
EFI_STATUS
HeciGetManufacturingInfo (
  OUT UINT32 *ManufacturingInfo
  );

//
// ME Client - Unique Platform Id (UPID)
//

/**
  This message is sent by the BIOS to check if given UPID feature is enabled in CSME

  @param[in]  UpidFeature          UPID feature
  @param[out] UpidFeatureState     Determines if feature is enabled

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidFeatureState (
  IN  UINT32                      UpidFeature,
  OUT UINT8                       *UpidFeatureState
  );

/**
  This message is sent by the BIOS to set given UPID feature state in CSME

  @param[in] UpidFeature           UPID feature
  @param[in] UpidFeatureState      New UPID feature state

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUpidFeatureState (
  IN UINT32                UpidFeature,
  IN UINT8                 UpidFeatureState
  );

/**
  This message is sent by the BIOS to check if given UPID feature state can be
  modified by the OS

  @param[in]  UpidFeature               UPID feature
  @param[out] UpidFeatureOsControlState Determines if OS can modify the feature state

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER         Invalid input parameter
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidOsControlState (
  IN  UINT32                          UpidFeature,
  OUT UINT8                           *UpidFeatureOsControlState
  );

/**
  This message is sent by the BIOS to set if given UPID feature state can be
  modified by the OS

  @param[in] UpidFeature                UPID feature
  @param[in] UpidFeatureOsControlState  Determines if OS can modify the feature state

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciSetUpidOsControlState (
  IN UINT32                           UpidFeature,
  IN UINT8                            UpidFeatureOsControlState
  );

/**
  This message is sent by the BIOS to read the Platform Id

  @param[out] OemPlatformIdType    Determines format of the OemPlatformId field
  @param[out] OemPlatformId        Platform Id set by the OEM
  @param[out] CsmePlatformId       CSME Platfrom Id generated from CSME fuses

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidPlatformId (
  OUT UINT32                      *OemPlatformIdType,
  OUT UINT8                       *OemPlatformId,
  OUT UINT8                       *CsmePlatformId
  );

/**
  This message is sent by the BIOS to read the current Platform Id Refurbish Counter

  @param[out] PlatformIdRefurbishCounter  Current value of the Refurbish Counter


  @retval EFI_SUCCESS                     Command succeeded
  @retval EFI_UNSUPPORTED                 Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER           Invalid input parameter
  @retval EFI_DEVICE_ERROR                HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                     HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL            Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciGetUpidRefurbishCounter (
  OUT UINT8                       *PlatformIdRefurbishCounter
  );

/**
  This message is sent by the BIOS to update the OEM Platform Id

  @param[in] OemPlatformIdType     Determines format of the OemPlatformId field
  @param[in] OemPlatformId         OEM Platform Id to be set, up to 32 bytes
  @param[in] ManufacturingType     Manufacturing event

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_UNSUPPORTED          Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER    Invalid input parameter
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL     Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
HeciUpdateUpidOemPlatformId (
  IN UINT32                     OemPlatformIdType,
  IN UINT8                      *OemPlatformId,
  IN UINT32                     ManufacturingType
  );

//
// Prototype for ME Policy DXE phase
//

/**
  Check if End of Post Message is enabled in setup options.

  @retval FALSE                   EndOfPost is disabled.
  @retval TRUE                    EndOfPost is enabled.
**/
BOOLEAN
MeEndOfPostEnabled (
  VOID
  );

/**
  Check if MeFwDowngrade is enabled in setup options.

  @retval FALSE                   MeFwDowngrade is disabled.
  @retval TRUE                    MeFwDowngrade is enabled.
**/
BOOLEAN
MeFwDowngradeSupported (
  VOID
  );

/**
  Check if Core Bios Done Message is enabled in setup options.

  @retval FALSE                   Core Bios Done is disabled.
  @retval TRUE                    Core Bios Done is enabled.
**/
BOOLEAN
MeCoreBiosDoneEnabled (
    VOID
  );

/**
  Check if Auto Arb SVN Commit is enabled in setup options.

  @retval FALSE                   Auto Arb SVN Commit is disabled.
  @retval TRUE                    Auto Arb SVN Commit is enabled.
**/
BOOLEAN
MeAutoArbSvnCommitEnabled (
  VOID
  );

/**
  Read the ME policy to see if D0I3 for HECI devices are enabled.

  @retval TRUE                   D0I3 for HECI devices are enabled
  @retval FALSE                  D0I3 for HECI devices are disabled
**/
BOOLEAN
MeHeciD0I3Enabled (
  VOID
  );

/**
  Checks if EndOfPost event already happened

  @retval TRUE                    End of post already happened
  @retval FALSE                   End of post did not happen yet
**/
BOOLEAN
MeIsAfterEndOfPost (
  VOID
  );

/**
  Set EndOfPost done
**/
VOID
MeSetEndOfPostDone (
  VOID
  );

/**
  Check if HECI1 should be force enabled for debug purpose.

  @retval FALSE                   HECI1 should follow POR setting.
  @retval TRUE                    HECI1 should be force enabled.
**/
BOOLEAN
MeIsHeci1ForceEnabled (
  VOID
  );
#endif
