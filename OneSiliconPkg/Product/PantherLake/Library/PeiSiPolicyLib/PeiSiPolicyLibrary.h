/** @file
  Header file for the PeiSiPolicyLib library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PEI_SI_POLICY_LIBRARY_H_
#define _PEI_SI_POLICY_LIBRARY_H_

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/SiPolicyLib.h>
#include <Library/PcdPolicyLib.h>
#include <Library/PeiAmtPolicyLib.h>
#include <Library/PeiMePolicyLib.h>
#include <Library/CpuInitPolicyLib.h>
#include <Library/CpuPowerManagementPolicyLib.h>
#include <Library/PeiITbtPolicyLib.h>

// IPs
//
#include <Library/PeiIpuPolicyPrivateLib.h>
#include <Library/PeiDciPolicyLib.h>
#include <Library/PeiEspiPolicyLib.h>
#include <Library/PeiGbePolicyLib.h>
#include <Library/PeiTsnPolicyLib.h>
#include <Library/PeiHdaPolicyLib.h>
#include <Library/PeiIehPolicyLib.h>
#include <Library/PeiItssPolicyCommonLib.h>
#include <Library/PeiItssPolicyLib.h>
#include <Library/PeiP2sbPolicyLib.h>
#include <Library/PeiPchDmiPolicyLib.h>
#include <Library/PeiPsfPolicyLib.h>
#include <Library/PeiPmcPolicyLib.h>
#include <Library/PeiSataPolicyLib.h>
#include <Library/PeiScsPolicyLib.h>
#include <Library/PeiSmbusPolicyLib.h>
#include <Library/PeiUsbPolicyLib.h>
#include <Library/PeiWdtPolicyLib.h>
#include <Library/IGpuPeiPolicyLib.h>
#include <Library/PeiTraceHubPolicyLib.h>
#include <Library/PeiOcPolicyLib.h>
#include <Library/PeiThcPolicyLib.h>
#include <Library/PeiVtdPolicyLib.h>
#include <Library/PeiTcssPolicyLib.h>
#include <Library/PeiVmdPolicyLib.h>
#include <Library/PeiTelemetryPolicyLib.h>
#include <Library/PeiHybridGraphicsPolicyLib.h>
#include <Library/PeiFusaPolicyLib.h>
#include <Library/PeiVrPolicyLib.h>
#include <Library/PeiHostBridgePolicyLib.h>
#include <Library/PeiSpiPolicyLib.h>
#include <Library/PeiNpuPolicyLib.h>
#include <Library/PeiMemPolicyLib.h>
#include <Library/PeiCnviPolicyLib.h>
#include <Library/PeiBiosGuardPolicyLib.h>
#include <Library/PeiIaxPolicyLib.h>
#include <Library/PcdPeiIshPolicyLib.h>
#include <Library/PeiLpssI3cPolicyLib.h>
#include <Library/PeiLpssSpiPolicyLib.h>
#include <Library/PeiLpssUartPolicyLib.h>
#include <Library/PeiCanPolicyLib.h>

#define TEMP_MEM_BASE_ADDRESS 0xFE600000
#define TEMP_IO_BASE_ADDRESS  0xD000

//
// IO/MMIO resource limits
//
#define TEMP_MEM_SIZE         V_PCH_XDCI_MEM_LENGTH
#define TEMP_IO_SIZE          0x10

#endif // _PEI_SI_POLICY_LIBRARY_H_
