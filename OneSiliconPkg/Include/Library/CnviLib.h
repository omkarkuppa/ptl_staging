/** @file
  Header file for CnviLib.

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
#ifndef _CNVI_LIB_H_
#define _CNVI_LIB_H_

/**
  This function checks if CNVi CRF module is present

  @retval TRUE:  CNVi CRF module is present
          FALSE: CNVi CRF module is NOT present
**/
BOOLEAN
CnviCrfModuleIsPresent (
  VOID
  );

/**
  Check if CNVi Wifi is Enabled.

  @retval TRUE:  CNVi Wifi is enabled
          FALSE: CNVi Wifi is disabled
**/
BOOLEAN
IsCnviWifiEnabled (
  VOID
  );

#endif // _CNVI_LIB_H_
