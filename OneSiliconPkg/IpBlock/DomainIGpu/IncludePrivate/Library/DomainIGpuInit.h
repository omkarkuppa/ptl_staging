/** @file
  The GUID definition for IGpuInfoHob

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

#ifndef __DOMAIN_IGPU_INIT_H__
#define __DOMAIN_IGPU_INIT_H__

#include <Uefi/UefiBaseType.h>
#include <Pi/PiPeiCis.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <IGpuConfig.h>
#include <Library/IoLib.h>
#include <Ppi/SiPolicy.h>

//
// The Enum should be aligned with IP_IGPU_XE_VERSION
//
typedef enum {
  // IpIGpuXe*
  IGpuXe   = 1,
  IGpuXe2  = 2,
  IGpuXe3  = 3,
  IGpuXe3p = 4,
} IGPU_XE_VERSION;

/**
  IGpuPreMemInit for Initilizing IGpu IP Instances

  @retval       EFI_SUCCESS on success
**/
EFI_STATUS
IGpuPreMemInit (
  IN  IGPU_XE_VERSION  IGpuXeVersion
  );

/**
  IGpuPostMemInit for Initilizing Post Mem IGpu IP Instances

  @retval       EFI_SUCCESS on success
**/
EFI_STATUS
IGpuPostMemInit (
  VOID
  );

/**
  Create and Init IGPU Data Hob

  @param[in]  IGpuPreMemConfig     - Instance of IGPU_PEI_PREMEM_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
CreateIGpuDataHob (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  );

/**
  IGpuInit: Initialize the IGD if no other external graphics is present

  @param[in] IGpuPreMemConfig     - IGpuPreMemConfig to access the IGpuPreMemConfig related information
  @param[in] PrimaryDisplay       - Primary Display - default is IGD
  @param[in] IGpuMmioLength       - Total IGPU MMIO length

**/
VOID
IGpuInit (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN  DISPLAY_DEVICE          *PrimaryDisplay,
  IN  OUT UINT32              *IGpuMmioLength
  );

/**
  Notification service to be called when gEfiEndOfPeiSignalPpiGuid is installed.

  @param  PeiServices                 Indirect reference to the PEI Services Table.
  @param  NotifyDescriptor            Address of the notification descriptor data structure. Type EFI_PEI_NOTIFY_DESCRIPTOR is defined above.
  @param  Ppi                         Address of the PPI that was installed.

  @retval   EFI_STATUS                This function will install a PPI to PPI database. The status
                                      code will be the code for (*PeiServices)->InstallPpi.

**/
EFI_STATUS
EFIAPI
IGpuEndOfPeiCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

/**
  Initialize GT Power management.

  @param[in] IGPU_PEI_PREMEM_CONFIG      IGpuPreMemConfig
  @param[in] IGPU_PEI_CONFIG             IGpuConfig

  @retval EFI_SUCCESS          - GT Power management initialization complete
**/
EFI_STATUS
IGpuPmInit (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN  IGPU_PEI_CONFIG         *IGpuConfig
  );

/**
  IGpuDisplayInitPreMem: Initialize the Display in PreMem phase.

  @param[in] IGpuPreMemConfig        - IGPU_PEI_PREMEM_CONFIG to access the IGpuConfig related information
**/
VOID
IGpuDisplayInitPreMem (
  IN  IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  );

/**
  Initialize the Display IP in Post Mem phase.
**/
VOID
IGpuDisplayInitPostMem (
  VOID
  );

/**
  This function is to set IGPU Memory map (RC6 base, Doorbell base, Dfdbase etc).

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuSetMemMap (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  IGpuPeiDisplayInit: Initialize the IGD using GraphicsInitPeim and display logo in PEI

  @param[in] IGpuPreMemConfig  - IGPU_PEI_PREMEM_CONFIG to access the IGpuPreMemConfig related information
  @param[in] IGpuConfig        - IGPU_PEI_CONFIG to access the IGpuConfig related information

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuPeiDisplayInit (
  IN   IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig,
  IN   IGPU_PEI_CONFIG         *IGpuConfig
  );

/**
  Initialize PAVP feature of IGPU.

  @param[in] IGpuPreMemConfig      - IGPU_PEI_CONFIG to access the IGpuPreMemConfig related information
  @param[in] IGpuConfig            - IGPU_PEI_CONFIG to access the IGpuConfig related information

  @retval     EFI_STATUS
**/
EFI_STATUS
IGpuPavpInit (
  IN       IGPU_PEI_CONFIG         *IGpuConfig,
  IN       IGPU_PEI_PREMEM_CONFIG  *IGpuPreMemConfig
  );

/**
  IGPU function called end of PEI

  @retval EFI_SUCCESS  The function completes successfully

**/
EFI_STATUS
EFIAPI
IGpuEndOfPei (
  VOID
  );

/**
  The function will inform the Display PHY that IGPU has been disabled
*/
VOID
InformIGpuDisableStatus (
  VOID
  );

/**
  Reserve IGPU Stolen Memory.

  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] Touud                      - Size leftover pointer
  @param[in] ResourceAttributeTested    - Resource attribute flag
**/
VOID
EFIAPI
IGpuMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  );

/**
Reserve IGPU Stolen Memory.

@param[in] SiPreMemPolicyPpi        - Pointer to SiPreMemPolicyPpi
@param[in] TopUseableMemAddr        - Moving address pointer
@param[in] Touud                    - Size leftover pointer
@param[in] ResourceAttributeTested  - Resource attribute flag
**/
VOID
EFIAPI
IGpuGsm2Allocation (
  IN SI_PREMEM_POLICY_PPI         *SiPreMemPolicyPpi,
  IN OUT EFI_PHYSICAL_ADDRESS     *TopUseableMemAddr,
  IN OUT UINT64                   *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested
  );

#endif // __DOMAIN_IGPU_INIT_H__
