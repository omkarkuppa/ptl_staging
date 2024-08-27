/** @file
  Header file for SPI SoC library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#ifndef _PTL_PCD_SPI_SOC_LIB_H_
#define _PTL_PCD_SPI_SOC_LIB_H_

#include <Ppi/SiPolicy.h>
#include <SpiHandle.h>

/**
  Get SPI Flash controller instance

  @param[in, out] SpiHandle       SPI controller handle
  @param[in]      SiPolicy        The Silicon Policy PPI instance - OPTIONAL

  @retval     EFI_SUCCESS           - Completed successfully
              EFI_INVALID_PARAMETER - SpiHandle NULL
**/
EFI_STATUS
PtlPcdGetSpiHandle (
  IN OUT  SPI_HANDLE       *SpiHandle,
  IN      SI_POLICY_PPI    *SiPolicy
  );

#endif // _PTL_PCD_SPI_SOC_LIB_H_
