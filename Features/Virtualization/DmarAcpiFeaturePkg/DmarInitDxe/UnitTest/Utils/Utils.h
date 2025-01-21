/** @file
  Utils for DMAR ACPI unit tests.

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

@par Specification Reference:
**/

#ifndef _UTILS_H_
#define _UTILS_H_

/**
  Dump the content of a given buffer.

  @param[in]  Buffer   pointer to a buffer which will be dumped
  @param[in]  Length   length of a given buffer
**/
VOID
DumpBuffer (
  UINT8 *Buffer,
  UINTN Length
  );

#endif // _UTILS_H_
