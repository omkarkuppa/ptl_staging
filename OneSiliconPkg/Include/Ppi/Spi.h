/** @file
  This file defines the PCH SPI PPI which implements the
  Intel(R) PCH SPI Host Controller Compatibility Interface.

  @note The APIs in this file are designed to be backward compatible with
  previous versions. Any change in behavior of these APIs will result in
  newer version of the API.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2006 Intel Corporation.

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
#ifndef _PCH_SPI_PPI_H_
#define _PCH_SPI_PPI_H_

#include <Protocol/Spi.h>

//
// Extern the GUID for PPI users.
//
extern EFI_GUID           gPchSpiPpiGuid;

/**
  Reuse the PCH_SPI_PROTOCOL definitions
  This is possible becaues the PPI implementation does not rely on a PeiService pointer,
  as it uses EDKII Glue Lib to do IO accesses
**/
typedef PCH_SPI_PROTOCOL PCH_SPI_PPI;

#endif
