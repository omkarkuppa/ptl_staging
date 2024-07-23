/** @file
  Definition for ME FWSTS Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _ME_FWSTS_LIB_H_
#define _ME_FWSTS_LIB_H_

#include <FwStsSmbiosTable.h>

extern CONST MEI_DEV_STRING gFwStsDeviceList[];
extern CONST UINT32         gFwStsOffsetTable[];

/**
  Get ME Firmware Status from FWSTS Hob by passed register

  @param[in]  FwStsReg     FWSTS register.

  @retval     UINT32       ME firmware status value.
                           In error case 0 returned
**/
UINT32
GetMeStatusFromFwstsHob (
  IN UINT32    FwStsReg
  );

/**
  Get ME operation mode from FWSTS Hob.

  @retval UINT32             ME operation mode.
**/
UINT32
GetMeModeFromFwstsHob (
  VOID
  );

#endif
