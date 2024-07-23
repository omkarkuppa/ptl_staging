/** @file
  UBS3 IP Private API

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

#ifndef __IP_USB3_PRIVATE_H__
#define __IP_USB3_PRIVATE_H__

#include <IpUsb3.h>
#include <IpUsb3VscId.h>
#include <IpWrapper.h>

#define IP_WR_READ_8(Context, Offset)   ((UINT8)IpWrRegRead ((Context), (UINT32)(Offset), (UINT32)IpWrRegFlagSize8Bits))
#define IP_WR_READ_16(Context, Offset)  ((UINT16)IpWrRegRead ((Context), (UINT32)(Offset), (UINT32)IpWrRegFlagSize16Bits))
#define IP_WR_READ_32(Context, Offset)  ((UINT32)IpWrRegRead ((Context), (UINT32)(Offset), (UINT32)IpWrRegFlagSize32Bits))

#define IP_WR_WRITE_8(Context, Offset, Data)   (IpWrRegWrite ((Context), (UINT32)(Offset), (UINT64)(Data), (UINT32)IpWrRegFlagSize8Bits))
#define IP_WR_WRITE_16(Context, Offset, Data)  (IpWrRegWrite ((Context), (UINT32)(Offset), (UINT64)(Data), (UINT32)IpWrRegFlagSize16Bits))
#define IP_WR_WRITE_32(Context, Offset, Data)  (IpWrRegWrite ((Context), (UINT32)(Offset), (UINT64)(Data), (UINT32)IpWrRegFlagSize32Bits))

#define IP_WR_AND_8(Context, Offset, Data)                     IP_WR_WRITE_8((Context), (Offset), (IP_WR_READ_8((Context), (Offset)) & (UINT8)(Data)))
#define IP_WR_OR_8(Context, Offset, Data)                      IP_WR_WRITE_8((Context), (Offset), (IP_WR_READ_8((Context), (Offset)) | (UINT8)(Data)))
#define IP_WR_AND_THEN_OR_8(Context, Offset, DataAnd, DataOr)  IP_WR_WRITE_8((Context), (Offset), ((IP_WR_READ_8((Context), (Offset)) & (UINT8)(DataAnd)) | (UINT8)(DataOr)))

#define IP_WR_AND_16(Context, Offset, Data)                     IP_WR_WRITE_16((Context), (Offset), (IP_WR_READ_16((Context), (Offset)) & (UINT16)(Data)))
#define IP_WR_OR_16(Context, Offset, Data)                      IP_WR_WRITE_16((Context), (Offset), (IP_WR_READ_16((Context), (Offset)) | (UINT16)(Data)))
#define IP_WR_AND_THEN_OR_16(Context, Offset, DataAnd, DataOr)  IP_WR_WRITE_16((Context), (Offset), ((IP_WR_READ_16((Context), (Offset)) & (UINT16)(DataAnd)) | (UINT16)(DataOr)))

#define IP_WR_AND_32(Context, Offset, Data)                     IP_WR_WRITE_32((Context), (Offset), (IP_WR_READ_32((Context), (Offset)) & (UINT32)(Data)))
#define IP_WR_OR_32(Context, Offset, Data)                      IP_WR_WRITE_32((Context), (Offset), (IP_WR_READ_32((Context), (Offset)) | (UINT32)(Data)))
#define IP_WR_AND_THEN_OR_32(Context, Offset, DataAnd, DataOr)  IP_WR_WRITE_32((Context), (Offset), ((IP_WR_READ_32((Context), (Offset)) & (UINT32)(DataAnd)) | (UINT32)(DataOr)))

/**
  Ip Version Specific Configuration(VSC) Enabled

  @param[in]  pInst         USB3 IP Instance
  @param[in]  Id            Configuration ID (see IP_USB3_VSC_ID)

  @retval     TRUE          IP Version requires that programming
  @retval     FALSE         IP Version doesn't require additional programming
**/
BOOLEAN
IpUsb3VersionSpecificConfigurationEnabled (
  IP_USB3_INST    *pInst,
  IP_USB3_VSC_ID  Id
  );

/**
  This interface will apply the feature values provided by using SetControl interface.

  @param[in]  pInst         USB3 IP Instance
  @param[in]  pFeatureList  List of Parameter Ids and Parameter Values to be applied.
  @param[in]  EntryCount    The number of feature list entries in the pFeatureList.

  @retval     IpCsiStsSuccess         programming completed without error
  @retval     IpCsiStsErrorNullPtr    null pointer to IP Instance provided
  @retval     IpCsiStsError           call to IpUsb3SetControl returned an error
  @retval     IpCsiStsErrorUnknownId  bad IP_USB3_FEATURE_LIST_ENTRY
**/
IP_CSI_STATUS
IpUsb3ApplyFeatureList (
  IP_USB3_INST                *pInst,
  IP_USB3_FEATURE_LIST_ENTRY  *pFeatureList,
  UINT32                      EntryCount
  );

// Easy PRINT_*() Macros
#if defined (IP_USB3_PRINT_ALL) || defined (IP_USB3_PRINT_ERROR)
#define PRINT_ERROR_NULL_INST(...)                                      \
  do {                                                                  \
    IpWrPrint ((IP_WR_PRINT_CNTXT)0, __VA_ARGS__);                      \
    IpWrError ((IP_WR_ERROR_CNTXT)0, IpWrErrorMajorNullPtr, __LINE__);  \
  } while (0)
#else
#define PRINT_ERROR_NULL_INST(...)                                      \
  do {                                                                  \
    IpWrError ((IP_WR_ERROR_CNTXT)0, IpWrErrorMajorNullPtr, __LINE__);  \
  } while (0) // no printf
#endif

#if defined (IP_USB3_PRINT_ALL) || defined (IP_USB3_PRINT_ERROR)
#define PRINT_ERROR(...)                                          \
  do {                                                            \
    IpWrPrint (pInst->PrintError, __VA_ARGS__);                   \
    IpWrError (pInst->ErrorCntxt, IpWrErrorMajorError, __LINE__); \
  } while (0)
#else
#define PRINT_ERROR(...)                                          \
  do {                                                            \
    IpWrError (pInst->ErrorCntxt, IpWrErrorMajorError, __LINE__); \
  } while (0) // no printf
#endif

#if defined (IP_USB3_PRINT_ALL) || defined (IP_USB3_PRINT_WARNING)
#define PRINT_WARNING(...)                                                \
  do {                                                                    \
    IpWrPrint (pInst->PrintWarning, __VA_ARGS__);                         \
    IpWrError (pInst->ErrorCntxtWarning, IpWrErrorMajorError, __LINE__);  \
  } while (0)
#else
#define PRINT_WARNING(...)                                                \
  do {                                                                    \
    IpWrError (pInst->ErrorCntxtWarning, IpWrErrorMajorError, __LINE__);  \
  } while (0) // NO printf
#endif

#if defined (IP_USB3_PRINT_ALL) || defined (IP_USB3_PRINT_LEVEL1)
#define PRINT_LEVEL1(...)  IpWrPrint (pInst->PrintLevel1, __VA_ARGS__)
#else
#define PRINT_LEVEL1(...) do {} while (0) // NOOP at compile time
#endif

#endif
