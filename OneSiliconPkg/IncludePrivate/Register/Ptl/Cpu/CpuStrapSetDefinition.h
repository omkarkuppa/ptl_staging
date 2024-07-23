/** @file
  Header file for Cpu Strap Set.

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

#ifndef _CPU_STRAP_SET_DEFINITION_H_
#define _CPU_STRAP_SET_DEFINITION_H_

#include <Base.h>

///
/// Structure for CPU Strap settings
///
typedef struct {
  UINT32 Reserved1                  :  1; ///< [0]    Reserved.
  UINT32 NumberOfActiveBigCores     :  4; ///< [4:1]  Number of Active Big Cores.
  UINT32 Bist                       :  1; ///< [5]    Built In Self Test (BIST) initiation.
  UINT32 FlexRatio                  :  6; ///< [11:6] Flex Ratio.
  UINT32 FastWakeup                 :  1; ///< [12]   Processor boot ratio; When set allows the processor to power up in maximum non-turbo ratio from the following boot.
  UINT32 Reserved2                  :  3; ///< [15:13]Reserved
  UINT32 EnDebugDisable             :  1; ///< [16]   Encrypted debug disable.
  UINT32 NumberOfLpAtomCores        :  6; ///< [22:17]Number of enabled off ring (in NOC cluster) Atom cores.
  UINT32 NumberOfNonLpAtomCores     :  5; ///< [27:23]Number of enabled on ring (in CCF cluster) Atom cores.
  UINT32 Reserved3                  :  2; ///< [29:28]Reserved.
  UINT32 KvmrCaptureDis             :  1; ///< [30]   Disable CSME VDM messages by OEM.
  UINT32 KvmrSpriteDis              :  1; ///< [31]   Disable CSME VDM messages by OEM.
} CPU_STRAP_SET;

#endif
