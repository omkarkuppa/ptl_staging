/** @file
  Header file for PchEspiLib.
  All function in this library is available for PEI, DXE, and SMM,

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

#ifndef _PTL_SOC_ESPI_LIB_H_
#define _PTL_SOC_ESPI_LIB_H_

/**
  Is eSPI enabled in strap.

  @retval TRUE          Espi is enabled in strap
  @retval FALSE         Espi is disabled in strap
**/
BOOLEAN
PtlPcdIsEspiEnabled (
  VOID
  );

#endif // _PTL_SOC_ESPI_LIB_H_
