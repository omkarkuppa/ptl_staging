/**@file

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
#ifndef _ME_SETUP_H_
#define _ME_SETUP_H_

//
// ME Mode
//
#define ME_MODE_MASK                  0x0F
#define NORMAL_MODE                   0x00
#define TEMP_DISABLED_MODE            0x03

//
// ME Image Type
//
#define ME_IMAGE_CONSUMER_SKU_FW      0x03
#define ME_IMAGE_CORPORATE_SKU_FW     0x04

#define ME_FW_IMAGE_TYPE_CONSUMER_STRING       "Consumer SKU"
#define ME_FW_IMAGE_TYPE_CORPORATE_STRING      "Corporate SKU"
#define ME_FW_IMAGE_TYPE_UNIDENTIFIED_STRING   "Unidentified"

#pragma pack(1)

/**
  Making any setup structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table\n
  <b>Revision 1</b>: - Initial version.
**/
#define ME_SETUP_STORAGE_REVISION 1

typedef struct {
  UINT8 MngState;
  UINT8 FwUpdEnabled;
  UINT8 MeStateControl;
  UINT8 DelayedAuthenticationMode;
  UINT8 FipsModeSelect;
  UINT8 InvokeArbSvnCommit;
  UINT8 AmtState;
  UINT8 UpidSupport;
  UINT8 UpidState;
  UINT8 UpidOsControlState;
  UINT8 ActiveLanInterface;
} ME_SETUP_STORAGE;

/**
  Making any setup structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table\n
  <b>Revision 1</b>: - Initial version.
**/
#define ME_STORAGE_EXPOSURE_REVISION 1

typedef struct {
  UINT8  MeState;
  UINT8  AfterEoP;
  UINT8  RemoteSessionActive;
} ME_STORAGE_EXPOSURE;

/**
  Making any setup structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table\n
  <b>Revision 1</b>: - Initial version.
**/
#define ME_INFO_SETUP_REVISION 1

typedef struct {
  UINT32 MeMajor;
  UINT32 MeMinor;
  UINT32 MeHotFix;
  UINT32 MeBuildNo;
} ME_INFO_SETUP_DATA;

#pragma pack()

#endif
