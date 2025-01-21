/** @file
  FspV Config block

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef _FSPV_CONFIG_H_
#define _FSPV_CONFIG_H_

#include <ConfigBlock.h>

#define FSPV_POSTMEM_CONFIG_REVISION 1
extern EFI_GUID gFspVConfigGuid;

#define FSPV_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gFspVPreMemConfigGuid;

/**
  Add FSPM phase specific test case IDs
**/
typedef enum {
  RunNoFspmTest       = 0x0000,
  RunAllFspmTest      = 0xFFFF,
} FSPM_TEST_PARAMETER;

/**
  Add FSPS phase specific test case IDs
**/
typedef enum {
  RunNoFspsTest       = 0x0000,
  RunAllFspsTest      = 0xFFFF,
} FSPS_TEST_PARAMETER;

/**
  The FSPV_PREMEM_CONFIG block describes policies related to FSP Validation

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    FSP Validation Test Config Structure for FSPM
  **/
  UINT32                TestId;
  UINT32                ReservedBytes[3];
} FSPV_PREMEM_CONFIG;

/**
  The FSPV_POSTMEM_CONFIG block describes policies related to FSP Validation

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    FSP Validation Test Config Structure for FSPM
  **/
  UINT32                TestId;
  UINT32                ReservedBytes[3];
} FSPV_POSTMEM_CONFIG;

#endif // _FSPV_CONFIG_H_
