/** @file
  Header file for GbeSocLib.

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

@par Specification Reference:
**/

#ifndef _PCD_GBE_SOC_LIB_H_
#define _PCD_GBE_SOC_LIB_H_

#include <GbeHandle.h>

/**
  Get GBE controller instance for PCH

  @param[out]  *GbeController         Pointer to GBE controller identification structure

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - GbeController is NULL
**/
EFI_STATUS
GbeGetController (
  OUT GBE_CONTROLLER         *GbeController
  );

#endif
