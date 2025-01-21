/** @file
  @ Header file to get Iax information.

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
#ifndef _IAX_INFO_LIB_H_
#define _IAX_INFO_LIB_H_

/**
  IsIaxSupported: Check if IAX is supported or not by Silicon.

  @retval FALSE = IAX is fused off.
          TRUE = IAX is supported by Sillicon.
**/
BOOLEAN
EFIAPI
IsIaxSupported (
  VOID
  );

/**
  IsIaxEnabled: Check if IAX is enabled or not.

  @retval TRUE = IAX is enabled.
  @retval FALSE = IAX is disabled.
**/
BOOLEAN
EFIAPI
IsIaxEnabled (
  VOID
  );

#endif /* _IAX_INFO_LIB_H_ */
