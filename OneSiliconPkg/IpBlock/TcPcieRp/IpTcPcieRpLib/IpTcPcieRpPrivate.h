/** @file
  PCIE Root Port IP internal interfaces and definitions

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

#ifndef __IP_TC_PCIE_RP_PRIVATE_H__
#define __IP_TC_PCIE_RP_PRIVATE_H__

// Suggested PRINT Macros
#if defined (IP_TC_PCIE_RP_PRINT_ALL) || defined (IP_TC_PCIE_RP_PRINT_ERROR)
#define TC_PCIE_RP_PRINT_ERROR(...)  IpWrPrint(pInst->PrintError, __VA_ARGS__)
#else
#define TC_PCIE_RP_PRINT_ERROR(...) \
   do {                   \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_TC_PCIE_RP_PRINT_ALL) || defined (IP_TC_PCIE_RP_PRINT_WARNING)
#define TC_PCIE_RP_PRINT_WARNING(...)  IpWrPrint(pInst->PrintWarning, __VA_ARGS__)
#else
#define TC_PCIE_RP_PRINT_WARNING(...) \
   do {                     \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_TC_PCIE_RP_PRINT_ALL) || defined (IP_TC_PCIE_RP_PRINT_INFO)
#define TC_PCIE_RP_PRINT_INFO(...)  IpWrPrint(pInst->PrintLevel1, __VA_ARGS__)
#else
#define TC_PCIE_RP_PRINT_INFO(...) \
   do {                  \
   } while (0) // NOOP at compile time
#endif

#if defined (IP_TC_PCIE_RP_PRINT_ALL) || defined (IP_TC_PCIE_RP_PRINT_ERROR)
#define TC_PCIE_RP_PRINT_ERROR_PINST_NULL(...)                         \
  do {                                                              \
    IpWrPrint ((IP_WR_ERROR_CNTXT)0, __VA_ARGS__);                  \
    } while (0)
#else
#define TC_PCIE_RP_PRINT_ERROR_PINST_NULL(...)                         \
 do {                                                               \
   } while (0) // NOOP at compile time
#endif

// Suggested ERROR Macros
#define ERROR_ASSERT(Minor)  IpWrError(pInst->ErrorAssert, IpWrErrorMajorAssert, Minor)

#define TC_PCIE_RP_API_ENTRY() \
if (pInst == NULL) {\
  TC_PCIE_RP_PRINT_ERROR_PINST_NULL("%s: ERROR: Invalid Instance structure!\n", __FUNCTION__);\
  return IpCsiStsErrorNullPtr;\
}\
TC_PCIE_RP_PRINT_INFO("%s: Enter\n", __FUNCTION__);

// Check return value ant return if != SUCSESS
#define PCIE_RP_RETURN_IF_ERROR(Status) \
if (Status != IpCsiStsSuccess) {\
    return Status;\
}

#endif // __IP_TC_PCIE_RP_PRIVATE_H__
