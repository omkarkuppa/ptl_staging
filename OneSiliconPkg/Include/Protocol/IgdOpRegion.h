/** @file
  This file is part of the IGD OpRegion Implementation.  The IGD OpRegion is
  an interface between system BIOS, ASL code, and Graphics drivers.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
  - IGD OpRegion/Software SCI SPEC
**/
#ifndef _IGD_OPREGION_PROTOCOL_H_
#define _IGD_OPREGION_PROTOCOL_H_

#include <IndustryStandard/IgdOpRegion30.h>

extern EFI_GUID gIgdOpRegionProtocolGuid;

///
/// IGD OpRegion Protocol
///
typedef struct {
  IGD_OPREGION_STRUCTURE_VER_3_0  *OpRegion; ///< IGD Operation Region Structure
} IGD_OPREGION_PROTOCOL;

#endif
