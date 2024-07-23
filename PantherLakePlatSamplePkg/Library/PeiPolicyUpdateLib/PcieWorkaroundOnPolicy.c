/** @file
  This file is SampleCode for Intel SA Workaround on Policy initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#include <Library/PcieWorkaroundOnPolicy.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif

/**
  PCIe GPIO Write

  @param[in] Gpio        - GPIO Number
  @param[in] Active      - GPIO Active Information; High/Low
  @param[in] Level       - Write GPIO value (0/1)

**/
VOID
PcieGpioWrite (
  IN  UINT32                Gpio,
  IN  BOOLEAN               Active,
  IN  BOOLEAN               Level
  )
{
}
