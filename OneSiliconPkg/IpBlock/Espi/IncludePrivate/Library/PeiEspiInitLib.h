/**@file
  This is header for PCH eSPI initialization library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef _PEI_ESPI_INIT_LIB_H_
#define _PEI_ESPI_INIT_LIB_H_

#include <Ppi/SiPolicy.h>

/**
  Modifies CNTLR_PWRGATE_TYMOUT and CLKGATE_HOLDOFF_TYMOUT registers with
  new timeout value

  @param[in] EspiHandle      Pointer to initialized ESPI handle
  @param[in] TimeoutValue    New timeout value that will be programmed
 */
VOID
EspiSetTimeoutValues (
  ESPI_HANDLE       *EspiHandle,
  UINT8             TimeoutValue
  );

/**
  Configures PCH eSPI

  @param[in] EspiHandle      Pointer to initialized ESPI handle
**/
VOID
EspiInit (
  IN  ESPI_HANDLE       *EspiHandle
  );

#endif // _PEI_ESPI_INIT_LIB_H_
