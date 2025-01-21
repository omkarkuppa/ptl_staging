/** @file
  IP Wrapper Intefaces defined by Common Services Interface (CSI) Spec

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation

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

#ifdef __cplusplus
extern "C" {
#endif

#ifndef IP_WRAPPER_OPTIONAL_H_
#define IP_WRAPPER_OPTIONAL_H_

/**
  Read list of registers

  @param[in]      pCntxt IP Wrapper (IPWR) register group context. Opaque to CSI FW and defined by IP Wrapper
  @param[in]      NumEntries the number of valid entries in OffsetList and DataList
  @param[in]      OffsetList: for i=0..NumEntries-1, DataList[i] = register read of OffsetList[i]
  @param[out]     DataList:   for i=0..NumEntries-1, DataList[i] = register read of OffsetList[i]
  @param[in]      RegFlags OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size
                  IpWrRegFlagSize64Bits is not allowed

  @retval         IpCsiStsSuccess on success
**/
IP_CSI_STATUS
IpWrRegRead32bitList (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           NumEntries,
  UINT32           *OffsetList,
  UINT32           *DataList,
  UINT32           RegFlags
  );

/**
  Write list of registers

  @param[in]  pCntxt      IP Wrapper defined register group context. Opaque to CSI FW.
  @param[in]  NumEntries  the number of valid entries in OffsetList and DataList
  @param[in]  OffsetList: for i=0..NumEntries-1, register write to OffsetList[i] = DataList[i]
  @param[in]  DataList:   for i=0..NumEntries-1, register write to OffsetList[i] = DataList[i]
  @param[in]  RegFlags    OR'ed values of IP_WR_REG_FLAGS. at least "IpWrRegFlagSize*" is required for size
                          IpWrRegFlagSize64Bits is not allowed

  @retval     IpCsiStsSuccess on success
**/
IP_CSI_STATUS
IpWrRegWrite32bitList (
  IP_WR_REG_CNTXT  pCntxt,
  UINT32           NumEntries,
  UINT32           *OffsetList,
  UINT32           *DataList,
  UINT32           RegFlags
  );

#endif //IP_WRAPPER_OPTIONAL_H_

#ifdef __cplusplus
}
#endif
