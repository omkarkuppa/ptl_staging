/** @file
  Silicon Policy PPI is used for specifying platform
  related Intel silicon information and policy setting.
  This PPI is consumed by the silicon PEI modules and carried
  over to silicon DXE modules.

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

#ifndef _SI_POLICY_PRODUCT_H_
#define _SI_POLICY_PRODUCT_H_

#include <SiPolicyStruct.h>
#include <PchPolicyCommon.h>
#include <PchPreMemPolicyCommon.h>
#include <MePeiConfig.h>
#include <AmtConfig.h>
#include <FusaConfig.h>
#include <Uefi.h>
#include <CpuPowerMgmt.h>
#include <CpuInitConfig.h>
#include <CpuPowerMgmtBasicConfig.h>
#include <CpuPowerMgmtCustomConfig.h>
#include <CpuPowerDeliveryConfig.h>
#include <CpuPowerMgmtTestConfig.h>
#include <CpuTestConfig.h>
#include <CpuInitPreMemConfig.h>
#include <CpuSecurityPreMemConfig.h>
#include <TxtConfig.h>
#include <BiosGuardConfig.h>
#include <ConfigBlock/VoltageRegulator/CpuPowerMgmtVrConfig.h>
#include <OverclockingConfig.h>
#include <FiaConfig.h>
#include <Defines/HostBridgeDefines.h>

#ifndef DISABLED
#define DISABLED  0
#endif
#ifndef ENABLED
#define ENABLED   1
#endif

extern EFI_GUID gSiPreMemPolicyPpiGuid;
extern EFI_GUID gSiPolicyPpiGuid;


#include <IGpuConfig.h>
extern EFI_GUID gGraphicsPeiPreMemConfigGuid;
extern EFI_GUID gGraphicsPeiConfigGuid;

#include <VtdConfig.h>
extern EFI_GUID gVtdConfigGuid;


#include <HybridGraphicsConfig.h>
extern EFI_GUID gHybridGraphicsConfigGuid;

#include <MemoryConfig.h>
extern EFI_GUID gMemoryConfigGuid;
extern EFI_GUID gMemoryConfigNoCrcGuid;

#include <TraceHubConfig.h>

#include <HostBridgeConfig.h>
extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#include <NpuConfig.h>
extern EFI_GUID gNpuPeiConfigGuid;

#include <IaxConfig.h>
extern EFI_GUID gIaxPeiConfigGuid;

#include <PeiDmiConfig.h>
extern EFI_GUID gPeiDmiConfigGuid;

typedef struct _SI_PREMEM_POLICY_STRUCT SI_PREMEM_POLICY_PPI;
typedef struct _SI_POLICY_STRUCT SI_POLICY_PPI;

#endif // _SI_POLICY_PRODUCT_H_
