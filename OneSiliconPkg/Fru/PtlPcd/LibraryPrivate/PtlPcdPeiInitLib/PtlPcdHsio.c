/** @file
  The HSIO SoC specific implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#include <Ppi/SiPolicy.h>

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/TimerLib.h>
#include <Library/PchInfoLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiHsioLib.h>
#include <Library/PchFiaLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdHsioSocLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Register/PcieSipRegs.h>
#include <Register/PchRegsHsio.h>
#include <HsioHandle.h>
#include "PtlPcdInitPei.h"

/**
  The function program PHY registers.

  @param[in] SiPolicyPpi               The SI Policy PPI instance

**/
VOID
PtlPcdPhyConfigure (
  IN  SI_POLICY_PPI    *SiPolicyPpi
  )
{
  P2SB_SIDEBAND_REGISTER_ACCESS      SbAccess;

  DEBUG ((DEBUG_INFO, "PtlPcdPhyConfigure() Start\n"));

  PtlPcdPhyPwrGatingProg (&SbAccess);
  PtlPcdPhyBeforeSaiPostBoot (&SbAccess);

  DEBUG ((DEBUG_INFO, "PtlPcdPhyConfigure() End\n"));
}