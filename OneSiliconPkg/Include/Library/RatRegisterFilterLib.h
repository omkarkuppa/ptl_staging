/** @file
  Header of RAT Register Filter Library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef __RAT_REGISTER_FILTER_LIB_H__
#define __RAT_REGISTER_FILTER_LIB_H__

#include <RegisterAddressTranslation.h>

/**
  Filter the register access.

  @param[in] Socket         Socket number
  @param[in] AccessorType   Type of register space
  @param[in] Address        Register address
  @param[in] Size           Size of register
  @param[in] Operation      Read or write operation
  @param[in] Value          Register value for write operation

  @retval TRUE if the register access has been filtered
  @retval FALSE if the access was not filtered out
**/
BOOLEAN
RatFilterAccess (
  IN UINT8                    Socket,
  IN REGISTER_SPACE_TYPE      AccessorType,
  IN UINTN                    Address,
  IN REGISTER_WIDTH_BYTES     Size,
  IN REGISTER_ACCESS_RW       Operation,
  IN UINT64                   *Value
  );

#endif // #ifndef __RAT_REGISTER_FILTER_LIB_H__
