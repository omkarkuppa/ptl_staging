/** @file
  Base type definition for IP API for common services interface (CSI)

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __IP_WRAPPER_BASE_TYPES_H__
#define __IP_WRAPPER_BASE_TYPES_H__

//
// To avoid defintion conflict during EDK2 bulid, it must include
// ProcessorBind.h before IpCsi.h
//
#ifdef __PROCESSOR_BIND_H__

#define IP_CSI_ATTR_VA_ARG EFIAPI
#define IP_WR_OPAQUE_DATA  UINT64

#else

#include <stdint.h>

typedef uint8_t  BOOLEAN;
typedef uint8_t  UINT8;
typedef uint16_t UINT16;
typedef uint32_t UINT32;
typedef uint64_t UINT64;
typedef int8_t   INT8;
typedef int16_t  INT16;
typedef int32_t  INT32;
typedef int64_t  INT64;

#if UINTPTR_MAX == UINT64_MAX
typedef UINT64 UINTN;
typedef INT64  INTN;
#else
typedef UINT32 UINTN;
typedef INT32  INTN;
#endif

//
// Constants. They may exist in other build structures, so #ifndef them.
//
#ifndef TRUE
#define TRUE  ((BOOLEAN)(1==1))
#endif

#ifndef FALSE
#define FALSE ((BOOLEAN)(0==1))
#endif

#ifndef NULL
#define NULL  ((void *) 0)
#endif

#ifndef MAX_UINT32
#define MAX_UINT32  ((UINT32)0xFFFFFFFF)
#endif // MAX_UINT32

#endif // __PROCESSOR_BIND_H__

#endif //__IP_WRAPPER_BASE_TYPES_H__
