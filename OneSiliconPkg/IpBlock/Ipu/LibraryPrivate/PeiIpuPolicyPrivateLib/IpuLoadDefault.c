/** @file
  This file provides services for IPU PEI policy default load functon.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <IpuPreMemConfig.h>


/**
  Function call to Load defaults for IPU

  @param[in] ConfigBlockPointer  - Pointer to ConfigBlock
**/
VOID
IpuLoadPreMemDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  IPU_PREMEM_CONFIG      *IpuPreMemPolicy;

  IpuPreMemPolicy = ConfigBlockPointer;
  DEBUG ((DEBUG_INFO, "%a Name = %g HobLength = 0x%x\n", __FUNCTION__, &IpuPreMemPolicy->Header.GuidHob.Name, IpuPreMemPolicy->Header.GuidHob.Header.HobLength));
  //
  // IPU Policy default load.
  //
#if FixedPcdGetBool(PcdIpuEnable) == 1
  UINT8                  Index;
  IpuPreMemPolicy->IpuEnable = TRUE;

  // The first 4 ImgClkOut are enabled by default.
  for (Index = 0; Index < GPIO_IMGUCLK_NUMBER_OF_PINS-2; Index ++) {
    IpuPreMemPolicy->ImguClkOutEn[Index] = TRUE;
  }
#endif

}
