/** @file
  CPU BIOS Guard Config Block.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _BIOS_GUARD_CONFIG_H_
#define _BIOS_GUARD_CONFIG_H_

#include <BiosGuard.h>

#define BIOS_GUARD_CONFIG_REVISION 1              ///< BIOS Guard 2.2 Configuration Revision
#define BIOSGUARD_TOTAL_HASH_MSRS  6              ///< Total HW HASH MSRS supported for BIOS Guard 2.2
#define BIOSGUARD_MAX_HASH_SIZE    48             ///< Maximum size for SHA384 HASH for BIOS Guard 2.2

extern EFI_GUID gBiosGuardConfigGuid;             ///< BIOS Guard Configuration GUID

#pragma pack (push,1)

/**
  BIOS Guard Configuration Structure.
  Platform policies for BIOS Guard Configuration for all processor security features configuration.
  Platform code can pass relevant configuration data through this structure.
  @note <b>Optional.</b> These policies will be ignored if CPU_SECURITY_PREMEM_CONFIG -> BiosGuard is disabled, or PeiBiosGuardLibNull is used.

  <b>Revision 1</b>:
  - Initial version.

**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                               ///< Config Block Header
  BIOSGUARD_ATTRIBUTES  BiosGuardAttr;                        ///< BIT1 - EC Present, BIT2 - EC BIOS Guard protection, BIT3 - Descriptor Override policy, BIT4 - Flash wearout protection, BIT5 - FTU enable
  UINT64                BgpdtHash[BIOSGUARD_TOTAL_HASH_MSRS]; ///< Hash of the BGPDT supported by BIOS Guard module PLAT_FRMW_PROT_HASH_0/1/2 MSR. Total HW HASH MSRS supported for BIOS Guard 2.2 = 6.
  EFI_PHYSICAL_ADDRESS  BiosGuardModulePtr;                   ///< Pointer to the BIOS Guard Module.
  EFI_PHYSICAL_ADDRESS  EcProvisionEav;                       ///< Pointer to the EC function Provision Ephemeral Auth Value.
  EFI_PHYSICAL_ADDRESS  EcBiosGuardCmdLock;                   ///< Pointer to the EC function Provision Lock
} BIOS_GUARD_CONFIG;
#pragma pack (pop)

/**
  Security function for BIOS guard: EC_C_PFAT_PROVISION_EAV.

  @param[out] ReturnValue       The return value from EC FW for Ephemeral Auth Value.

  @retval     EFI_SUCCESS       Command success
  @retval     EFI_DEVICE_ERROR  Command error

**/
typedef
EFI_STATUS
(EFIAPI *EC_PROVISION_EAV) (
  IN  UINT32              Eav,
  OUT UINT8               *ReturnValue
  );

/**
  Security function for BIOS guard: EC_C_PFAT_CMD_LOCK.

  @param[out] ReturnValue       The return value from EC FW.

  @retval     EFI_SUCCESS       Command success
  @retval     EFI_DEVICE_ERROR  Command error

**/
typedef
EFI_STATUS
(EFIAPI *EC_CMD_LOCK) (
  OUT UINT8               *ReturnValue
  );

#endif // _BIOS_GUARD_CONFIG_H_
