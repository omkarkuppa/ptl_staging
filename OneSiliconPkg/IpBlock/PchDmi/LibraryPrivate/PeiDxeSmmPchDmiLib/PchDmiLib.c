/** @file
  PCH DMI library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PchPcrLib.h>
#include <Library/PcieHelperLib.h>
#include <Register/PchPcrRegsDeprecated.h>
#include <Register/PchDmi15Regs.h>
#include <Register/PchDmi17Regs.h>
#include <PchPcieRpConfig.h>

/**
  Read DMI Negotiated Link width

  @return DMI Negotiated Link Width
**/
UINT8
PchDmiNegotiatedLinkWidth (
  VOID
  )
{
  return (UINT8) ((PchPcrRead16 (PID_DMI_DEPRECATED, R_PCH_DMI15_PCR_LSTS) & B_PCIE_LSTS_NLW) >> N_PCIE_LSTS_NLW);
}

/**
  Enable PCIe Relaxed Order
**/
VOID
PchDmiEnablePcieRelaxedOrder (
  VOID
  )
{
  //
  // Enable Forced Relaxed Ordering to always allow downstream completions to pass posted writes.
  // Set Completion Relaxed Ordering Attribute Override Value
  // and Completion Relaxed Ordering Attribute Override Enable
  //
  if (IsPchWithPdmi ()) {
    PchPcrAndThenOr32 (PID_DMI_DEPRECATED, R_PCH_DMI15_PCR_PCIECFG2, ~0u, (B_PCH_DMI15_PCR_PCIECFG2_CROAOV | B_PCH_DMI15_PCR_PCIECFG2_CROAOE));
  }
}

/**
  This function will do necessary configuration for
  Unsupported Transaction Policy bit.
**/
VOID
PchDmiConfigUnsupportedTransactionPolicy (
  VOID
  )
{
  if (IsPchWithPdmi ()) {
    //
    // Set Unsupported Transaction Policy Bit
    // Enable Transaction Credit Update Arbitration Performance Fix
    //
    PchPcrAndThenOr32 (
    PID_DMI_DEPRECATED, R_PCH_DMI15_PCR_CTRL1,
    ~0u,
    B_PCH_DMI15_PCR_CTRL1_UTPB | B_PCH_DMI15_PCR_CTRL1_TCUAPF
  );
  }
}

