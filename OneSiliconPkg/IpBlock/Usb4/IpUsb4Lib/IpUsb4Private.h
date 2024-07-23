/** @file
  USB4 IP internal interfaces and definitions

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


#ifndef __IP_USB4_PRIVATE_H__
#define __IP_USB4_PRIVATE_H__

// Suggested PRINT Macros
#if defined (IP_USB4_PRINT_ALL) || defined (IP_USB4_PRINT_ERROR)
#define USB4_PRINT_ERROR(...)  IpWrPrint(pInst->PrintError, __VA_ARGS__)
#else
#define USB4_PRINT_ERROR(...) \
   do {                   \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_USB4_PRINT_ALL) || defined (IP_USB4_PRINT_WARNING)
#define USB4_PRINT_WARNING(...)  IpWrPrint(pInst->PrintWarning, __VA_ARGS__)
#else
#define USB4_PRINT_WARNING(...) \
   do {                     \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_USB4_PRINT_ALL) || defined (IP_USB4_PRINT_INFO)
#define USB4_PRINT_INFO(...)  IpWrPrint(pInst->PrintLevel1, __VA_ARGS__)
#else
#define USB4_PRINT_INFO(...) \
   do {                  \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_USB4_PRINT_ALL) || defined (IP_USB4_PRINT_ERROR)
#define USB4_PRINT_ERROR_PINST_NULL(...)                            \
  do {                                                              \
    IpWrPrint ((IP_WR_ERROR_CNTXT)0, __VA_ARGS__);                  \
    } while (0)
#else
#define USB4_PRINT_ERROR_PINST_NULL(...)                            \
 do {                                                               \
   } while (0) // NOOP at compile time
#endif

// Suggested ERROR Macros
#define ERROR_ASSERT(Minor)  IpWrError(pInst->ErrorAssert, IpWrErrorMajorAssert, Minor)

#define USB4_API_ENTRY() \
if (pInst == NULL) {\
  USB4_PRINT_ERROR_PINST_NULL("%s: ERROR: Invalid Instance structure!\n", __FUNCTION__);\
  return IpCsiStsErrorNullPtr;\
}\
USB4_PRINT_INFO("%s: Enter\n", __FUNCTION__);

// Check return value ant return if != SUCSESS
#define USB4_RETURN_IF_ERROR(Status) \
if (Status != IpCsiStsSuccess) {\
    return Status;\
}

#endif // __IP_USB4_PRIVATE_H__
