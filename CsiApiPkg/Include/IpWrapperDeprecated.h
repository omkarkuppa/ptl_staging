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

#ifndef IP_WRAPPER_DEPRECATED_H_
#define IP_WRAPPER_DEPRECATED_H_

/** DEPRECATED INTERFACES:
 * - This file contains deprecated interfaces for backwards compatability.
 * - These interfaces should not be used in newer code
**/

/**
  DEPRECATED: please use IpWrRegPollUs()
  Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - The Mask and Match criteria have been met
    - The number of retries is exceeded
  Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match)
  @param[in]      Match          Exit polling when ((regRead & Mask) == Match)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  minimum uSecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
 DEPRECATED: please use IpWrRegPoll2ValUs()
 Polling Register read. This function will continue to read the specified register until
  one of the following criteria is met:
    - ((regRead & Mask) == Match1)
    - ((regRead & Mask) == Match2)
    - The number of retries is exceeded
Time Accuracy is limited by the environment.

  @param[in]      pRegCntxt      Used for reg read (see IpWrRegRead)
  @param[in]      Offset         Used for reg read (see IpWrRegRead)
  @param[in]      RegFlags       Flags used for reg read (see IpWrRegRead)
  @param[in]      Mask           Exit polling when ((regRead & Mask) == Match1) || ((regRead & Mask) == Match2)
  @param[in]      Match1         Exit polling when ((regRead & Mask) == Match1)
  @param[in]      Match2         Exit polling when ((regRead & Mask) == Match2)
  @param[in]      MaxRetry       # of retry before timeout.  0 loops forever
  @param[in]      pTimeCntxt     Time context for delay
  @param[in]      DelayMicroSec  minimum uSecond delay per retry. Can be 0.
  @param[out]     pApiSts        Optional (ie may be NULL)
                                 on Match, return IpCsiStsSuccess
                                 on timeout, return IpCsiStsErrorTimeout
**/
void
IpWrRegPoll2Val (
  IP_WR_REG_CNTXT   pRegCntxt,
  UINT32            Offset,
  UINT32            RegFlags,
  UINT64            Mask,
  UINT64            Match1,
  UINT64            Match2,
  UINT32            MaxRetry,
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            DelayMicroSec,
  IP_CSI_STATUS     *pApiSts
  );

/**
  DEPRECATED: please use IpWrDelayUs()
  Return after a microsecond delay that is at least MinMicroSec in duration.
  May "sleep" if supported
  Time Accuracy is limited by the environment.

  @param[in]  pTimeCntxt     Time context (Opaque to CSI FW)
  @param[in]  MinMicroSec    The minimum number of micro seconds to delay for.
**/
void
IpWrDelay (
  IP_WR_TIME_CNTXT  pTimeCntxt,
  UINT32            MinMicroSec
  );

#endif //IP_WRAPPER_DEPRECATED_H_

#ifdef __cplusplus
}
#endif
