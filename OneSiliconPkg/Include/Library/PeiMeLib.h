/** @file
  Header file for PEI Me functionality

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
#ifndef _PEI_ME_LIB_H_
#define _PEI_ME_LIB_H_

#include <BupMsgs.h>
#include <MkhiMsgs.h>
#include <MchiMsgs.h>
#include <IccMsgs.h>
#include <MeBiosPayloadData.h>

//
// Prototype for HECI message PEI phase
//

//
// ME Client - MKHI
//

//
// MKHI_CBM_GROUP_ID = 0x00
//

/**
  Send Core BIOS Reset Request Message through HECI to reset the system.

  @param[in] ResetOrigin         Reset source
  @param[in] ResetType           Global or Host reset

  @retval EFI_UNSUPPORTED        Current ME mode doesn't support this function
  @retval EFI_SUCCESS            Command succeeded
  @retval EFI_NOT_FOUND          No ME present
  @retval EFI_DEVICE_ERROR       HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT            HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciSendCbmResetRequest (
  IN  UINT8                      ResetOrigin,
  IN  UINT8                      ResetType
  );

//
// MKHI_FWCAPS_GROUP_ID = 0x03
//

/**
  Send Get Firmware SKU Request to ME

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwCapsSkuMsg (
  OUT MEFWCAPS_SKU               *RuleData
  );

/**
  This message is sent by the BIOS to retrieve information about CSME firmware
  that platform runs with.

  @param[out] RuleData              Including PlatformBrand, IntelMeFwImageType,
                                    SuperSku, and PlatformTargetUsageType

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetPlatformTypeMsg (
  OUT PLATFORM_TYPE_RULE_DATA     *RuleData
  );

/**
  The Get FW Feature Status message is based on MKHI interface.
  This command is used by BIOS to get firmware runtime status.

  @param[out] RuleData              ME Firmware Capability SKU

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetFwFeatureStateMsg (
  OUT MEFWCAPS_SKU                *RuleData
  );

/**
  This message is sent by the BIOS in PEI phase to query
  ME Unconfig on RTC Clear Disable state.

  @param[out] RuleData            1 - Unconfig on RTC clear is disabled
                                  0 - Unconfig on RTC clear is enabled

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetUnconfigOnRtcClearDisableMsg (
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
PeiHeciGetAmtBiosSyncInfo (
  OUT AMT_BIOS_SYNC_INFO *RuleData
  );

/**
  Send Set Rule Data Request to CSME

  @param[in]   RuleId             Identifies the rule which data needs to be changed.
  @param[in]   RuleDataLength     Rule Data Length.
  @param[in]   RuleData           Pointer to new rule data.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciFwCapsSetRuleData (
  IN  UINT32                      RuleId,
  IN  UINT8                       RuleDataLength,
  IN  UINT32                      RuleData
  );

/**
  This message is sent by the BIOS in PEI phase to set
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
PeiHeciSetUnconfigOnRtcClearDisableMsg (
  IN UINT32                RuleData
  );

//
// MKHI_GEN_GROUP_ID = 0xFF
//

/**
  Get MKHI (Management Engine Kernel Host Interface) Version.

  @param[out] MkhiVersion           MKHI Version

  @retval EFI_UNSUPPORTED           Can't localize HECI protocol
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetMkhiVersion (
  OUT MKHI_VERSION        *MkhiVersion
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
PeiHeciGetUnconfigureStatus (
  OUT UINT8* UnconfigStatus
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
PeiHeciGetImageFwVersionMsg (
  IN  UINT32                    PartitionId,
  OUT UINT32                    *NumOfModules,
  OUT FLASH_PARTITION_DATA      *PartitionIdData
  );

/**
  This message is sent by BIOS to set the soft straps override configuration

  @param[in]  StrapGroupsNumber   Number of strap groups
  @param[in]  StrapOverrideData   Strap override data
  @param[out] Flags               Pointer to flags

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetStrapOverrideConfig (
  IN  UINT32              StrapGroupsNumber,
  IN  STRAP_OVERRIDE_DATA *StrapOverrideData,
  OUT UINT32              *Flags
  );

/**
  This message is sent by BIOS to retrieve the soft straps override configuration

  @param[in]     StrapGroupId        Strap group ID
  @param[in out] StrapGroupsNumber   Number of strap groups
  @param[out]    StrapOverrideData   Strap override data

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_INVALID_PARAMETER      Invalid input parameter
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciGetStrapOverrideConfig (
  IN     STRAP_GROUPS        StrapGroupId,
  IN OUT UINT32              *StrapGroupsNumber,
  OUT    STRAP_OVERRIDE_DATA *StrapOverrideData
  );

//
// BUP_COMMON_GROUP_ID = 0xF0
//

/**
  Send DRAM init done message through HECI to inform ME of memory initialization done.

  @param[in]  ImrBaseLow                Base address for IMR region (Low DWORD)
  @param[in]  ImrBaseHigh               Base address for IMR region (High DWORD)
  @param[in]  MemStatus                 Memory init status
  @param[in]  PchBusNumber              Pch Bus Number
  @param[out] MkhiResult                MKHI Error Code
  @param[out] BiosAction                ME response to DID

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED               Message is not supported for current ME
**/
EFI_STATUS
PeiHeciSendDid (
  IN  UINT32  ImrBaseLow,
  IN  UINT32  ImrBaseHigh,
  IN  UINT8   MemStatus,
  IN  UINT8   PchBusNumber,
  OUT UINT8   *MkhiResult,
  OUT UINT8   *BiosAction
  );

/**
  Send Get MBP from FW

  @param[out]     MbpEntriesNumber  Number of MBP entries
  @param[in, out] MbpItems          MBP items of the response
  @paran[in]      SkipMbp           Skip MBP

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
                                    or response is incorrect
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER     Invalid input parameter
**/
EFI_STATUS
PeiHeciGetMbpMsg (
  OUT UINT32                   *MbpEntriesNumber,
  IN OUT UINT32                *MbpItems,
  IN BOOLEAN                   SkibMbp
  );

/**
  This message is sent by the BIOS to retrieve from CSME total size of IMRs.
  BIOS needs to provide mask of disabled IMRs and requested size for PCIe IMR.

  @param[in]  BiosImrDisableMask0  Low DWORD of BIOS IMR Disable mask
  @param[in]  BiosImrDisableMask1  High DWORD of BIOS IMR Disable mask
  @param[in]  PciImrSize           Requested IMR size for PCI
  @param[out] MkhiResult           MKHI Error Code
  @param[out] ImrsSize             Total IMR size
  @param[out] Alignment            Required address alignment

  @retval EFI_SUCCESS              Command succeeded
  @retval EFI_DEVICE_ERROR         HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT              HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciGetImrSizeMsg (
  IN  UINT32     BiosImrDisableMask0,
  IN  UINT32     BiosImrDisableMask1,
  IN  UINT32     PciImrSize,
  OUT UINT8      *MkhiResult,
  OUT UINT32     *ImrsSize,
  OUT UINT32     *Alignment
  );

/**
  Send Manufacturing Reset and Halt

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciBupManufacturingResetAndHalt (
  VOID
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
PeiHeciDataClear (
  VOID
  );

/**
  This message is sent by the BIOS to tell CSME that Buffer has been allocated for payload to be copied.

  @param[in]      LoadRequest            IP information for which firmware has to be loaded
  @param[in]      BufferAddressLow       Lower DRAM address of the buffer
  @param[in]      BufferAddressHigh      Upper DRAM address of the buffer
  @param[in, out] BufferSize             On input, BufferSize is size of Buffer allocated by Bios.On output, actual PayloadSize copied by ME
  @param[out]     IpLoaderStatus         IP Loading status from CSME
  @param[out]     HashType               Hash algorithm used for message digest
  @param[out]     Hash                   Hash Value

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support sending this message through this HECI or Input parameters are not valid.
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_INVALID_PARAMETER          Parameter invalid
**/
EFI_STATUS
PeiHeciGetFwPayload (
  IN     UINT32        LoadRequest,
  IN     UINT32        BufferAddressLow,
  IN     UINT32        BufferAddressHigh,
  IN OUT UINT32        *BufferSize,
  OUT    UINT32        *IpLoaderStatus,
  OUT    UINT8         *HashType,
  OUT    UINT8         *Hash
  );

/**
  This message is sent by the BIOS to retrieve debug token data from CSME.

  @param[out] KnobData              Debug Token Knob Data

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetDebugTokenData (
  OUT UINT32                  *KnobData
  );

/**
  This message is sent by the BIOS to retrieve device specific seed from CSME.

  @param[in] BiosSeedProperties     Seed properties
  @param[in] LockSeedGeneration     0 - No lock
                                    1 - Explicit lock requested by BIOS
  @param[out] EphemeralKeyCounter   Random counter to be used with ephemeral key for trusted channel
  @param[out] BiosSeedInfo          Seed information
  @param[out] BiosSeed              BIOS seed. If BIOS requested 32 bytes only, then the CSME will
                                    return the seed in first 32 bytes and fill rest of buffer by 0

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_NOT_FOUND             Couldn't locate HeciPpi or debug token data
  @retval EFI_INVALID_PARAMETER     Parameter invalid
**/
EFI_STATUS
PeiHeciGetBiosSeed (
  IN  SEED_PROPERTIES *BiosSeedProperties,
  IN  UINT8           LockSeedGeneration,
  OUT UINT32          *EphemeralKeyCounter,
  OUT SEED_INFO       *BiosSeedInfo,
  OUT UINT8           *BiosSeed
  );

/**
  The function sends a request to load a binary using HECI and waits for an acknowledgment.

  @param[in] BinaryIdsList        Pointer to the list of binary IDs.

  @retval EFI_SUCCESS             Command succeeded.
  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function.
  @retval EFI_INVALID_PARAMETER   BinaryIdsList is NULL.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate required resources.
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally.
**/
EFI_STATUS
PeiHeciLoadBinaryMsg (
  IN UINT32             BinaryId
  );

//
// BUP_ICC_GROUP_ID = 0xF1
//

/**
  Send ICC request through HECI to query if CSME FW requires the warm reset flow from a previous boot.

  @param[out] WarmResetRequired   1 - CSME requires a warm reset to complete BCLK ramp en flow

  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciIccBclkMsg (
  OUT UINT8    *WarmResetRequired
  );

//
// BUP_MPHY_GROUP_ID = 0xF2
//

/**
  Get the Mphy data from CSME

  @param[out] Data                Pointer to the data buffer.
  @param[in]  DataSize            Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
  @retval EFI_NOT_FOUND           Couldn't locate HeciPpi.
**/
EFI_STATUS
PeiHeciReadFromMphy (
  OUT UINT8  *Data,
  IN  UINT32 DataSize
  );

//
// BUP_PM_GROUP_ID = 0xF3
//

/**
  Send Host Reset Notification Message to determine if warm reset is required.

  @retval EFI_UNSUPPORTED           Current ME mode doesn't support this function
  @retval EFI_SUCCESS               Command succeeded
  @retval EFI_DEVICE_ERROR          HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT               HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL      Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSendHostResetNotificationMsg (
  VOID
  );

//
// BUP_BIOS_AR_GROUP_ID = 0xF4
//

/**
  Requests the current monotonic counter from the designated RPMC counter.

  @param[in]    CounterIndex            Counter Index to access.
  @param[out]   CounterValue            A pointer to a buffer to store the RPMC value.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
PeiHeciRequestMonotonicCounter (
  IN  UINT8   CounterIndex,
  OUT UINT32  *CounterValue
  );

/**
  Increments the designated monotonic counter in the SPI flash device by 1.

  @param[in]    CounterIndex            Counter Index to access.

  @retval       EFI_SUCCESS             The operation completed successfully.
  @retval       EFI_DEVICE_ERROR        A device error occurred while attempting to update the counter.
  @retval       EFI_UNSUPPORTED         The operation is un-supported.
**/
EFI_STATUS
PeiHeciIncrementMonotonicCounter (
  IN  UINT8   CounterIndex
  );

//
// ME Client - MCHI
//

//
// MCHI_MCA_GROUP_ID = 0x0A
//

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
  @retval EFI_OUT_OF_RESOURCES    Cannot allocate memory
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciSetFileExMsg (
  IN UINT32          FileId,
  IN UINT32          Offset,
  IN UINT32          DataSize,
  IN UINT8           Flags,
  IN UINT8           *DataBuffer
  );

/**
  This message is sent by the BIOS after HeciSetFileExMsg to notify ME firmware that it needs to
  commit previously set files. Caller shold invoke global reset afterwards.

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL    Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
PeiHeciCommitFilesMsg (
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
PeiHeciGetRpmcStatus (
  OUT RP_STATUS_3         *RpStatus
  );

//
// ME Client - Integrated Clock Chip (ICC)
//

/**
  Reads the ChipsetInit table from the CSME FW.

  @param[in, out] ChipsetInitTable     Pointer to buffer for the required system ChipsetInit Table.
  @param[in, out] ChipsetInitTableLen  Pointer to the length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
**/
EFI_STATUS
PeiHeciReadChipsetInitMsg (
  IN OUT UINT8                   *ChipsetInitTable,
  IN OUT UINT32                  *ChipsetInitTableLen
  );

/**
  Send the required system ChipsetInit Table to CSME FW.

  @param[in] ChipsetInitTable     The ChipsetInitSyncMsg HDR and the required system ChipsetInit Table.
  @param[in] ChipsetInitTableLen  Length of the table in bytes

  @retval EFI_UNSUPPORTED         Current ME mode doesn't support this function
  @retval EFI_SUCCESS             Command succeeded
  @retval EFI_DEVICE_ERROR        HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT             HECI does not return the buffer before timeout
  @retval EFI_OUT_OF_RESOURCES    HECI Could not allocate Memory
**/
EFI_STATUS
PeiHeciWriteChipsetInitMsg (
  IN  UINT8                 *ChipsetInitTable,
  IN  UINT32                ChipsetInitTableLen
  );

//
// Prototype for ME Policy PEI phase
//

/**
  Read the ME policy to see if MBP HOB is skipped.

  @retval TRUE             MBP HOB is skipped
  @retval FALSE            MBP HOB is not skipped
**/
BOOLEAN
MeMbpHobSkipped (
  VOID
  );

/**
  Read the ME policy to see if D0I3 for HECI devices are enabled.

  @retval TRUE             D0I3 for HECI devices are enabled
  @retval FALSE            D0I3 for HECI devices are disabled
**/
BOOLEAN
MeHeciD0I3Enabled (
  VOID
  );

/**
  Check if End of Post Message is enabled in setup options.

  @retval FALSE            EndOfPost is disabled.
  @retval TRUE             EndOfPost is enabled.
**/
BOOLEAN
MeEndOfPostEnabled (
  VOID
  );

/**
  Checks if EndOfPost event already happened

  @retval TRUE             End of post already happened
  @retval FALSE            End of post did not happen yet
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
  Checks if ME is available for HSIO

  @retval FALSE         Can't send HSIO messages
  @retval TRUE          HSIO messages can be send

**/
BOOLEAN
IsMeAvailableForHsio (
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

/**
  Retrieves information on whether Pcie root port configuration SoftStrap override is being done or not

  @param[in]      NumberOfControllers    On input, it is the number of controllers caller expects.
  @param[in, out] ControllerConfig       Pointer to the controller config message.

  @retval EFI_SUCCESS                   Command succeeded
  @retval EFI_DEVICE_ERROR              HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                   HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL          Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED               Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES          Unable to allocate required resources
**/
EFI_STATUS
PeiHeciGetSoftStrpConfigMsg (
  IN      UINT16     NumberOfControllers,
  IN OUT  UINT8      *ControllerConfig
  );

/**
  Set Pcie Root port configuration SoftStrap override for the specified Pcie Root Port controller

  @param[in] NumberOfControllers         On input, it is the number of controllers caller expects.
  @param[in] OverrideMask                Mask with RpController where Hybrid Storage is present and softstraps need to be overridden

  @retval EFI_SUCCESS                    Command succeeded
  @retval EFI_DEVICE_ERROR               HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                    HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL           Message Buffer is too small for the Acknowledge
  @retval EFI_UNSUPPORTED                Current ME mode doesn't support send this message through this HECI
  @retval EFI_OUT_OF_RESOURCES           Unable to allocate required resources
**/
EFI_STATUS
PeiHeciOverrideSoftStrapMsg (
  IN UINT16     NumberOfControllers,
  IN UINT8      OverrideMask
  );

/**
  Install ME call back function for reset2 ppi.

**/
VOID
RegisterMeReset (
  VOID
  );

#endif
