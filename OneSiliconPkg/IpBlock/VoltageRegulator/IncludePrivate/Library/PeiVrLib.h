/** @file
  Header file for the Voltage Regulator (VR) initializations.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _PEI_VR_LIB_H
#define _PEI_VR_LIB_H

#include <Ppi/SiPolicy.h>
#include <Library/BaseLib.h>
#include <Library/ConfigBlockLib.h>
#include <SiPolicyStruct.h>
#include <Register/B2pMailbox.h>

///
/// Voltage Regulator domains temporary Vr address and Svid Enbled.
///
typedef struct {
  UINT8                       VrAddress[MAX_NUM_VRS];   ///< Temporary VR address according to various VR domains.
  UINT8                       SvidEnabled[MAX_NUM_VRS]; ///< Svid Enabled\Disabled according to various VR domains. Bit 0 - SA, Bit1 - IA, Bit 2 - RING, Bit - 3- GT, Bit 4 - Fivr
} VR_DOMAIN_TOPOLOGY;

/**
  Programs the VR parameters for the external VR's which support SVID communication.

  @param[in]  SiPreMemPolicyPpi   The Silicon PreMem Policy PPI instance
**/
VOID
EFIAPI
ConfigureSvidVrs (
  IN OUT SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  Programs the VR mailbox without using Override Values for the selected VR Address.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
**/
VOID
SetVrNonOverrideValuesFru (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex
  );

/**
  Load CPU power management Vr Config block default for specific generation.

  @param[in, out] CpuPowerMgmtVrConfig   Pointer to CPU_POWER_MGMT_VR_CONFIG instance
**/
VOID
EFIAPI
PeiCpuLoadPowerMgmtVrConfigDefaultFru (
  IN OUT CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  );

/**
  Get Vr topologies which contain generation specific Vr address and bit which represents Svid Enabled/Disabled. Transferred as parameter from Fru to IP block.

  @param[IN,OUT] - VrDomainTopology - It takes VR_DOMAIN_TOPOLOGY as parameter and returns same which contains VR address and Svid Enable/Disabled.
**/
VOID
EFIAPI
GetCpuVrTopologyFru (
  IN OUT VR_DOMAIN_TOPOLOGY *VrDomainTopology
  );

/**
  Returns the maximum number of voltage regulator domains.

  @retval Maximum Number of VR Domains
**/
UINT16
EFIAPI
GetMaxNumVrsFru (
  VOID
  );

/**
  Enables/Disables DLVR's PHASE_SSC feature.
  @param[in] CpuPowerMgmtVrConfig   Pointer to CPU_POWER_MGMT_VR_CONFIG instance
**/
VOID
EFIAPI
SetDlvrPhaseSsc (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig
  );

/**
  Send Early PM Configuration done through mailbox
**/
VOID
EFIAPI
SendPowerDeliveryDone (
  VOID
  );

/**
  Enables CEP/SIRP feature via mailbox.

  @param[in]  CpuPowerMgmtVrConfig   Vr config block
  @param[in]  TempVrAddress          Selected VR Address.
  @param[in]  VrIndex                Selected VR Index.
**/
VOID
SetVrMiscConfig (
  IN CPU_POWER_MGMT_VR_CONFIG    *CpuPowerMgmtVrConfig,
  IN UINT8                       TempVrAddress,
  IN UINTN                       VrIndex
  );

/**
  Get the VR name list

  @param[out]  VrCount   Output the list count

  @retval      The VR name list
**/
UINT32 *
EFIAPI
GetVrListFru (
  OUT UINTN  *VrCount
  );

#endif //_PEI_VR_LIB_H
