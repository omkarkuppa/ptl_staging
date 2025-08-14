/** @file
  This contains defination to build the Domain IGpu Library.

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

#ifndef __IGPU_INIT_H__
#define __IGPU_INIT_H__

#include <Uefi/UefiBaseType.h>
#include <Pi/PiPeiCis.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <IGpuConfig.h>
#include <Library/FspPerformanceLib.h>

/**
  Initialize GT of IGPU.

  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval EFI_SUCCESS   - GT Power Management initialization complete
  @retval !EFI_SUCCESS  - The input parameter is invalid
**/
EFI_STATUS
IGpuGtInit (
  IP_IGPU_INST  *pInst
  );

/**
  Initialize Media of IGPU.

  @param[in]  pInst       A pointer to the IP instance to be used.

  @retval EFI_SUCCESS   - GT Power Management initialization complete
  @retval !EFI_SUCCESS  - The input parameter is invalid
**/
EFI_STATUS
IGpuMediaInit (
  IP_IGPU_INST  *pInst
  );
#endif // __IGPU_INIT_H__
