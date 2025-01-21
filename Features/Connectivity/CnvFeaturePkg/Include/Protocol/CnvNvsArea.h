/** @file
  This file defines the CNV NVS Area Protocol.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _CNV_NVS_AREA_H_
#define _CNV_NVS_AREA_H_

//
// Platform NVS Area definition
//
#include <CnvPkgNvsAreaDef.h>

//
// Forward reference for pure ANSI compatibility
//
typedef struct _CNV_NVS_AREA_PROTOCOL CNV_NVS_AREA_PROTOCOL;

///
/// Extern the GUID for protocol users.
///
extern EFI_GUID  gCnvNvsAreaProtocolGuid;

/**
  Making any CNV_NVS_AREA structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table\n
  <b>Revision 1</b>:   - Initial version.\n
**/
#define CNV_NVS_AREA_REVISION  1

//
// Platform NVS Area Protocol
//
typedef struct _CNV_NVS_AREA_PROTOCOL {
  CNV_PKG_NVS_AREA    *Area;
} CNV_NVS_AREA_PROTOCOL;

#endif // _CNV_NVS_AREA_H_
