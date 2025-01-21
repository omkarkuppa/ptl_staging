/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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


#ifndef _S3_MEMORY_VARIABLE_H_
#define _S3_MEMORY_VARIABLE_H_
#define S3_MEMORY_VARIABLE_NAME  L"S3MemoryVariable"

typedef struct {
  EFI_PHYSICAL_ADDRESS  AcpiReservedMemoryBase;
  UINT32                AcpiReservedMemorySize;
  UINT64                SystemMemoryLength;
} S3_MEMORY_VARIABLE;

#endif
