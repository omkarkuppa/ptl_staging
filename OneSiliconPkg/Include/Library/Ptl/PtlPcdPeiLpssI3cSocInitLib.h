/** @file
  Lpss I3C SoC PEI Initialization library for PTL PCD.
  All function in this library is available for PEI.

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

#ifndef _PTL_PCD_PEI_LPSS_I3C_SOC_INIT_LIB_H_
#define _PTL_PCD_PEI_LPSS_I3C_SOC_INIT_LIB_H_

#include <LpssI3cHandle.h>

/**
  Build Lpss I3C Handle with all Callbacks and Soc Private Config

  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in, out] I3cHandle        The Lpss I3C Handle instance
  @param[in]      I3cDeviceConfig  Lpss I3C Config Block
  @param[in]      I3cIndex         I3C Controller index

  @retval         EFI_SUCCESS           Completed successfully
                  EFI_UNSUPPORTED       SOC is not supported
**/
EFI_STATUS
PtlPcdLpssI3cBuildHandle (
  IN     SI_POLICY_PPI          *SiPolicy,
  IN OUT LPSS_I3C_HANDLE        *I3cHandle,
  IN     LPSS_I3C_DEVICE_CONFIG *I3cDeviceConfig,
  IN     UINT8                  I3cIndex
  );

/**
  Initialize the I3C controller

  @param[in]      SiPolicy         The SI Policy PPI instance

  @retval     EFI_SUCCESS           Initialization completed successfully
              EFI_UNSUPPORTED       The SOC is not supported
              EFI_NOT_FOUND         I3C Config Block not found
**/
EFI_STATUS
PtlPcdLpssI3cSocInit (
  IN     SI_POLICY_PPI          *SiPolicy
  );

#endif //_PTL_PCD_PEI_LPSS_I3C_SOC_INIT_LIB_H_
