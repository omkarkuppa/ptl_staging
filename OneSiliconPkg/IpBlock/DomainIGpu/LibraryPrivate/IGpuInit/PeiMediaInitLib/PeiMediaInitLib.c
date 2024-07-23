/** @file
  This contains code necessary to build the Domain IGpu Library fpr Media.

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

#include <IpWrapperCntxtInfoClient.h>
#include <Library/DebugLib.h>
#include <IpIGpu.h>

/**
  Initialize Media of IGPU.

  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval EFI_SUCCESS   - GT Power Management initialization complete
  @retval !EFI_SUCCESS  - The input parameter is invalid
**/
EFI_STATUS
IGpuMediaInit (
  IP_IGPU_INST  *pInst
  )
{
  EFI_STATUS   Status;

  Status = IpIGpuMediaInit (pInst);

  return Status;
}

