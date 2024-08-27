/** @file
  ICC Messages

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

#ifndef _ICC_MSGS_H
#define _ICC_MSGS_H

#include <HeciAddresses.h>

#pragma pack(1)

//
// macro for determining length of ICC message buffer
//
#define ICC_MESSAGE_BUFFER_LENGTH(a) (a-sizeof(ICC_HEADER))

//
// definitions for ICC MEI Messages
//
#define ICC_API_VERSION    0x00050000

typedef enum {
  ICC_STATUS_SUCCESS                              = 0x0,
  ICC_STATUS_FAILURE                              = 0x1,
  ICC_STATUS_INCORRECT_API_VERSION                = 0x2,
  ICC_STATUS_INVALID_FUNCTION                     = 0x3,
  ICC_STATUS_INVALID_BUFFER_LENGTH                = 0x4,
  ICC_STATUS_INVALID_PARAMS                       = 0x5,
  ICC_STATUS_FLASH_WEAR_OUT_VIOLATION             = 0x6,
  ICC_STATUS_FLASH_CORRUPTION                     = 0x7,
  ICC_STATUS_PROFILE_NOT_SELECTABLE_BY_BIOS       = 0x8,
  ICC_STATUS_TOO_LARGE_PROFILE_INDEX              = 0x9,
  ICC_STATUS_NO_SUCH_PROFILE_IN_FLASH             = 0xA,
  ICC_STATUS_CMD_NOT_SUPPORTED_AFTER_END_OF_POST  = 0xB,
  ICC_STATUS_NO_SUCH_RECORD                       = 0xC,
  ICC_STATUS_FILE_NOT_FOUND                       = 0xD,
  ICC_STATUS_INVALID_RECORD_FORMAT                = 0xE,
  ICC_STATUS_TOO_LARGE_UOB_RECORD                 = 0xF,
  ICC_STATUS_CLOCK_NOT_CONFIGURABLE               = 0x10,
  ICC_STATUS_REGISTER_IS_LOCKED                   = 0x11,
  ICC_STATUS_NO_VALID_PRE_UOB                     = 0x12,
  ICC_STATUS_NO_VALID_PERM_UOB                    = 0x13,
  ICC_STATUS_NO_DATA_FOR_THIS_CLOCK               = 0x14,
  ICC_STATUS_PROFILE_INDEX_IS_CURRENT             = 0x15,
  ICC_STATUS_NO_BCLK_ADJUSTMENT_FOUND             = 0x16,
  ICC_STATUS_WARM_RESET_RAMP_NOT_SUPPORTED        = 0x17,
  ICC_STATUS_UOB_RECORD_IS_ALREADY_INVALID        = 0x18,
  ICC_STATUS_NO_PROFILES_EXIST                    = 0x19,
  ICC_STATUS_AUTH_FAILURE                         = 0x1A,
  ICC_STATUS_ERROR_READING_FILE                   = 0x1B,
  ICC_STATUS_RANGE_VIOLATION_FREQ_TOO_HIGH        = 0x1C,
  ICC_STATUS_HW_VIOLATION_FREQ_TOO_HIGH           = 0x1D,
  ICC_STATUS_PENDING_REVERT_TO_DEFAULT            = 0x1E,
  ICC_STATUS_PENDING_SET_PROFILE                  = 0x1F,
  ICC_STATUS_UNVALID_PROFILE                      = 0x20,
  ICC_STATUS_UNVALID_OEM_DATA                     = 0x21,
  ICC_STATUS_ERROR_READING_DYNAMIC_RECORD         = 0x22,
  ICC_STATUS_RESERVED35                           = 0x23,
  ICC_STATUS_RANGE_VIOLATION_FREQ_TOO_LOW         = 0x24,
  ICC_STATUS_HW_VIOLATION_FREQ_TOO_LOW            = 0x25,
  ICC_STATUS_GET_REGISTER_NO_SUCH_REG             = 0x26,
  ICC_STATUS_RESERVED39                           = 0x27,
  ICC_STATUS_RESERVED40                           = 0x28,
  ICC_STATUS_RESERVED41                           = 0x29,
  ICC_STATUS_RESERVED42                           = 0x2A,
  ICC_STATUS_RESERVED43                           = 0x2B,
  ICC_STATUS_SSC_MODE_CHANGE_NOT_SUPPORTED        = 0x2C,
  ICC_STATUS_RANGE_VIOLATION_SSC_TOO_HIGH         = 0x2D,
  ICC_STATUS_RESERVED46                           = 0x2E,
  ICC_STATUS_RESERVED47                           = 0x2F,
  ICC_STATUS_RESERVED48                           = 0x30,
  ICC_STATUS_RESERVED49                           = 0x31,
  ICC_STATUS_SURVIVABILITY_SYNC_DISABLED          = 0x32,
  ICC_STATUS_WARM_RESET_FREQ_TOO_LOW              = 0x33,
  ICC_STATUS_NO_SUCH_TARGET_ID                    = 0x34,
  ICC_STATUS_NO_SUCH_REGISTER                     = 0x35,
  ICC_STATUS_INVALIDATE_SUCCESSFUL                = 0x36,
  ICC_STATUS_BUFFER_TOO_SMALL                     = 0x37,
  ICC_STATUS_VALID_UOB_ALREADY_PRESENT            = 0x38,
  ICC_STATUS_WAITING_FOR_POWER_CYCLE              = 0x39,
  ICC_STATUS_SURVIVABILITY_TABLE_ACCESS_VIOLATION = 0x3A,
  ICC_STATUS_SURVIVABILITY_TABLE_TOO_LARGE        = 0x3B,
  ICC_STATUS_NO_SUCH_EID                          = 0x3C,
  ICC_STATUS_SUCCESS_TRANSLATE_ONLY               = 0x3D,
  ICC_STATUS_CMD_NOT_SUPPORTED_BEFORE_DID         = 0x40,
  ICC_STATUS_FIA_MUX_CONFIG_SKU_MISMATCH          = 0x41,
  ICC_STATUS_FIA_MUX_NO_CONFIG_FOUND              = 0x42,
  ICC_STATUS_MAX_BUNDLES_PER_RECORD_REACHED       = 0x500,
  ICC_STATUS_PLL_UNSUPPORTED                      = 0x501,
  ICC_STATUS_DATA_ITEM_UNSUPPORTED                = 0x502,
  ICC_STATUS_OEM_PROFILE_CRDR_VIOLATION           = 0x503,
  ICC_STATUS_TOO_MANY_RECORDS                     = 0x600,
  ICC_STATUS_OEM_WHITE_LIST_VIOLATION             = 0x601,
  ICC_STATUS_INTEL_WHITE_LIST_VIOLATION           = 0x602,
  ICC_STATUS_UNSPECIFIED_ERROR                    = 0xFFFF
} ICC_CMD_STATUS;

typedef enum {
  ///
  /// Commands 0-15 except 10 not used.
  ///
  ICC_READ_FROM_MPHY_CMD                    = 0xA,
  ICC_GET_MPHY_SETTINGS_CMD                 = 0x18,
  ICC_SET_MPHY_SETTINGS_CMD                 = 0x19,
  ICC_SET_FIA_MUX_CONFIG_CMD                = 0x1E,
  ICC_GET_FIA_MUX_CONFIG_CMD                = 0x1F,
  ICC_OVERRIDE_SOFT_STRAP_CMD               = 0x27,
  ICC_GET_SOFT_STRAP_CONFIG_CMD             = 0x28
} ICC_COMMAND_ID;

typedef struct {
  UINT32              ApiVersion;
  ICC_COMMAND_ID      IccCommand;
  ICC_CMD_STATUS      IccResponse;
  UINT32              BufferLength;
  UINT32              Reserved;
} ICC_HEADER;

//
// Defines the HECI request buffer format for the ICC_READ_FROM_MPHY_CMD.
//
typedef struct {
  ICC_HEADER  Header;         ///< Standard ICC HECI Header
  UINT32      Reserved1;      ///< Reserved for future use
  UINT32      Offset;         ///< Offset in the mPhy file to start reading from
  UINT32      Reserved2;      ///< Reserved for future use
  UINT32      NumOfBytes;     ///< How many bytes to read. For the entire file specify the mPhy max size
} ICC_READ_FROM_MPHY_REQUEST;

//
// Defines the HECI request buffer format for response to ICC_READ_FROM_MPHY_CMD.
//
typedef struct {
  ICC_HEADER  Header;               ///< Standard ICC HECI Header
  UINT32      Reserved1;            ///< Reserved for future use
  UINT32      FileSize;             ///< mPhy file size. This will be filled regardless of how many bytes the user asked to read (e.g. zero)
  UINT32      Reserved2;            ///< Reserved for future use
  UINT32      ReadSize;             ///< How many bytes were actually read, having less than requested is NOT an error
  UINT32      Reserved3;            ///< Reserved for future use
  UINT8       Payload[];            ///< The binary content requested (or at least as many bytes as we could read)
} ICC_READ_FROM_MPHY_RESPONSE;

typedef union {
  ICC_READ_FROM_MPHY_REQUEST    Request;
  ICC_READ_FROM_MPHY_RESPONSE   Response;
} ICC_READ_FROM_MPHY_BUFFER;

//
// Defines the HECI request buffer format for the ICC_HSIO_WRITE_SETTINGS_CMD.
//
typedef struct {
  ICC_HEADER  Header;      ///< Standard ICC HECI Header
  UINT32      Reserved;    ///< Reserved for future use
  UINT8       MphyData[];  ///< Payload
} ICC_SET_MPHY_SETTINGS_REQUEST;

//
// Defines the HECI request buffer format for response to the ICC_HSIO_WRITE_SETTINGS_CMD.
//
typedef struct {
  ICC_HEADER  Header;      ///< Standard ICC HECI Header
} ICC_SET_MPHY_SETTINGS_RESPONSE;

typedef union {
  ICC_SET_MPHY_SETTINGS_REQUEST    Request;
  ICC_SET_MPHY_SETTINGS_RESPONSE   Response;
} ICC_SET_MPHY_SETTINGS_BUFFER;

//
// Defines the HECI request buffer format for the ICC_HSIO_READ_MPHY_CMD.
//
typedef struct {
  ICC_HEADER  Header;         ///< Standard ICC HECI Header
  UINT32      Reserved;       ///< Reserved for future use
  UINT32      Offset;         ///< Offset in the mPhy file to start reading from
  UINT32      Reserved2;      ///< Reserved for future use
  UINT32      NumBytesToRead; ///< How many bytes to read. For the entire file specify the mPhy max size
} HSIO_READ_MPHY_REQUEST;

//
// Defines the HECI request buffer format for response to ICC_HSIO_READ_MPHY_CMD.
//
typedef struct {
  ICC_HEADER  Header;               ///< Standard ICC HECI Header
  UINT32      Reserved;             ///< Reserved for future use
  UINT32      MphyFileSize;         ///< mPhy file size. This will be filled regardless of how many bytes the user asked to read (e.g. zero)
  UINT32      Reserved2;            ///< Reserved for future use
  UINT32      NumBytesActuallyRead; ///< How many bytes were actually read, having less than requested is NOT an error
  UINT32      Reserved3;            ///< Reserved for future use
  UINT8       Payload[];            ///< The binary content requested (or at least as many bytes as we could read)
} HSIO_READ_MPHY_RESPONSE;

typedef union {
  HSIO_READ_MPHY_REQUEST    Request;
  HSIO_READ_MPHY_RESPONSE   Response;
} HSIO_READ_MPHY_BUFFER;

///
/// ICC Soft Strap Config
///
typedef enum {
  PcieOverrideNone    = 0,
  PcieOverride1x4     = 1,
  PcieOverride2x2     = 2,
  PcieOverride1x2_2x1 = 3,
  PcieOverride4x1     = 4
} ICC_PCIE_CONTROLLER_SOFTSTRAP_OVERRIDE;

///
/// This is only a template. Data should be filled in runtime
///
typedef struct {
  ICC_HEADER  Header;
  UINT16      NumOfControllers;     ///< Number of PCI-E ports to be configured
  UINT8       ControllerConfig [0]; ///< Please refer to ICC_PCIE_CONTROLLER_SOFTSTRAP_OVERRIDE
  ///
  /// The ControllerConfig field size varies and it is followed by Reserved field.
  /// UINT32 Reserved[4];
  ///
} ICC_OVERRIDE_SOFT_STRAP_CMD_REQUEST;

typedef struct {
  ICC_HEADER  Header;
} ICC_OVERRIDE_SOFT_STRAP_CMD_RESPONSE;

typedef union {
  ICC_OVERRIDE_SOFT_STRAP_CMD_REQUEST   Request;
  ICC_OVERRIDE_SOFT_STRAP_CMD_RESPONSE  Response;
} ICC_OVERRIDE_PCIE_SOFT_STRAP_CMD_BUFFER;

///
/// Get Soft Strap Config
///
typedef struct {
  ICC_HEADER  Header;
} ICC_GET_SOFT_STRAP_CONFIG_CMD_REQUEST;

#define ICC_GET_SOFT_STRAP_CONFIG_CMD_RESPONSE ICC_OVERRIDE_SOFT_STRAP_CMD_REQUEST

typedef union {
  ICC_GET_SOFT_STRAP_CONFIG_CMD_REQUEST   Request;
  ICC_GET_SOFT_STRAP_CONFIG_CMD_RESPONSE  Response;
} ICC_GET_SOFT_STRAP_CONFIG_CMD_BUFFER;

#pragma pack()

#endif // _ICC_MSGS_H
