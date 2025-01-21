/** @file
  Source code file for Silicon Init Pre-Memory Library

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

#include <PiPei.h>

#include <Library/BaseLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PchCycleDecodingLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiServicesLib.h>
#include <Register/EspiRegs.h>
#include <Register/PchRegs.h>
#include <Library/PchPciBdfLib.h>

/**
  Early Platform PCH initialization
**/
VOID
EarlyLpcIoDecode (
  VOID
  )
{
  ///
  /// LPC I/O Configuration
  ///
  PchLpcIoDecodeRangesSet (
    (V_ESPI_CFG_ESPI_IOD_IOE_LPT_378  << N_ESPI_CFG_ESPI_IOD_IOE_LPT)  |
    (V_ESPI_CFG_ESPI_IOD_IOE_CB_3E8 << N_ESPI_CFG_ESPI_IOD_IOE_CB) |
    (V_ESPI_CFG_ESPI_IOD_IOE_CA_3F8 << N_ESPI_CFG_ESPI_IOD_IOE_CA)
    );

  PchLpcIoEnableDecodingSet (
    V_ESPI_CFG_ESPI_IOE_ME2  |
    V_ESPI_CFG_ESPI_IOE_SE   |
    V_ESPI_CFG_ESPI_IOE_ME1  |
    V_ESPI_CFG_ESPI_IOE_KE   |
    V_ESPI_CFG_ESPI_IOE_HGE  |
    V_ESPI_CFG_ESPI_IOE_LGE  |
    V_ESPI_CFG_ESPI_IOE_FDE  |
    V_ESPI_CFG_ESPI_IOE_PPE  |
    V_ESPI_CFG_ESPI_IOE_CBE  |
    V_ESPI_CFG_ESPI_IOE_CAE
    );

}

// @todo: It should be moved Policy Init.
/**
  Initialize the GPIO IO selection, GPIO USE selection, and GPIO signal inversion registers

**/
VOID
SiliconInit (
  VOID
  )
{
}

