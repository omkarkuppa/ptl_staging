/** @file
  Definitions for DisableBmeProtocol

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _DISABLE_TBT_BME_PROTOCOL_H_
#define _DISABLE_TBT_BME_PROTOCOL_H_

//
// Forward reference for pure ANSI compatability
//
typedef struct _EFI_DISABLE_TBT_BME_PROTOCOL EFI_DISABLE_TBT_BME_PROTOCOL;

/**
  Disabling TBT BME bit under UEFI Shell environment

  @param[in] Event     - A pointer to the Event that triggered the callback.
  @param[in] Context   - A pointer to private data registered with the callback function.
**/
typedef
VOID
(EFIAPI *DISABLE_BUS_MASTER_ENABLE) (
  IN EFI_EVENT    Event,
  IN VOID         *Context
  );

//
// Disable TBT BME  Protocol structure.
//
struct _EFI_DISABLE_TBT_BME_PROTOCOL {
  DISABLE_BUS_MASTER_ENABLE DisableBme;
};

//
// Disable iTBT BME Protocol GUID variable.
//
extern EFI_GUID gDxeDisableITbtBmeProtocolGuid;

#endif
