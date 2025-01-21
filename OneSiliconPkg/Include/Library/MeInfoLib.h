/** @file
  Definition for ME Info Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _ME_INFO_LIB_H_
#define _ME_INFO_LIB_H_

#include <MeDefines.h>

/**
  Get HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
MeHeciPciCfgBase (
  IN HECI_DEVICE   HeciFunc
  );

/**
  Get PSE HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
MePseHeciPciCfgBase (
  IN PSE_HECI_DEVICE   HeciFunc
  );

#endif
