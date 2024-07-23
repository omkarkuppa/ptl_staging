/** @file
  Definitions for functions used for handling MEBx Settings.

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

#ifndef _MEBX_SETTINGS_H
#define _MEBX_SETTINGS_H

/**
  Retrieve initial MEBx configuration from Firmware.

  @param[out] MEBxConfiguration      Initial MEBx Configuration

  @retval EFI_SUCCESS                Initial MEBx Configuration retrieved
  @retval Others                     Failed to retrieve initial MEBx Configuration
**/
EFI_STATUS
MebxGetInfo (
  OUT MEBX_CONFIGURATION *MebxConfiguration
  );

#endif // _MEBX_SETTINGS_H
