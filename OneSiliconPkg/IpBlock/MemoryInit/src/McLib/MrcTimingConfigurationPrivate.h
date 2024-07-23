/** @file
  This header exposes private members of MrcAddressDecodeConfiguration such that
  they can be used in Unit Testing

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

#ifndef MrcTimingConfigurationPrivate_h_
#define MrcTimingConfigurationPrivate_h_


/**
  This function returns nWR latency.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR parameter.
**/
UINT32
GetnWR (
  IN MrcParameters *const MrcData
  );


/**
  This function returns nWR_diff = nWR - tWR.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns nWR_diff parameter.
**/
UINT32
GetnWRDiff (
  IN MrcParameters *const MrcData
  );


/**
  This function returns derating_ext value which is added to tRAS, tRRD, tRP and tRCD when LPDDR5 is hot.

  @param[in]  MrcData - Pointer to MRC global data.

  @returns derating_ext parameter.
**/
UINT64
GetDeratingExt (
  IN MrcParameters *const MrcData
  );



#endif // MrcTimingConfigurationPrivate_h_
