/** @file
  TCSS internal interfaces and definitions

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
**/

#ifndef __TCSS_INIT_PRIVATE_H__
#define __TCSS_INIT_PRIVATE_H__

// Suggested PRINT Macros
#if defined (TCSS_PRINT_ALL) || defined (TCSS_PRINT_ERROR)
#define PRINT_ERROR(...)  IpWrPrint(pInst->PrintError, __VA_ARGS__)
#else
#define PRINT_ERROR(...) \
    do {                   \
    } while (0) // NOOP at compile time
#endif

#if defined (TCSS_PRINT_ALL) || defined (TCSS_PRINT_WARNING)
#define PRINT_WARNING(...)  IpWrPrint(pInst->PrintWarning, __VA_ARGS__)
#else
#define PRINT_WARNING(...) \
    do {                     \
    } while (0) // NOOP at compile time
#endif

#if defined (TCSS_PRINT_ALL) || defined (TCSS_PRINT_INFO)
#define PRINT_INFO(...)  IpWrPrint(pInst->PrintLevel1, __VA_ARGS__)
#else
#define PRINT_INFO(...) \
    do {                  \
    } while (0) // NOOP at compile time
#endif

#if defined (TCSS_PRINT_ALL) || defined (TCSS_PRINT_ERROR)
#define PRINT_ERROR_PINST_NULL(...)                                 \
  do {                                                              \
    IpWrPrint ((IP_WR_ERROR_CNTXT)0, __VA_ARGS__);                  \
    } while (0)
#else
#define PRINT_ERROR_PINST_NULL(...)                                 \
  do {                                                              \
    } while (0) // NOOP at compile time
#endif

// Suggested ERROR Macros
#define ERROR_ASSERT(Minor)  IpWrError(pInst->ErrorAssert, IpWrErrorMajorAssert, Minor)

#define TCSS_API_ENTRY() \
if (pInst == NULL) {\
  PRINT_ERROR_PINST_NULL("%s: ERROR: Invalid Instance structure!\n", __FUNCTION__);\
  return IpCsiStsErrorNullPtr;\
}\
PRINT_INFO("%s: Enter\n", __FUNCTION__);

// Check return value ant return if != SUCSESS
#define TCSS_RETURN_IF_ERROR(Status) \
if (Status != IpCsiStsSuccess) {\
    return Status;\
}

#endif // __TCSS_INIT_PRIVATE_H__
