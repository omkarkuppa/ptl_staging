/** @file
  Memory Sub System handle definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _MEMORY_SUB_SYSTEM_HANDLE_H_
#define _MEMORY_SUB_SYSTEM_HANDLE_H_

#include <CMrcInterface.h>
#include <Ppi/SiPolicy.h>

/**
  After BIOS determines the total physical memory size.
  Determines TOM which is defined by the total physical memory size.
  Determines TOM minus the ME memory size. The ME memory size is calculated from MESEG_BASE and MESEG_MASK.
  Determines MMIO allocation, which is system configuration dependent.

  Determines TOLUD which is the minimum value by comparing between "4GB minus MMIO size" and "TOM minus ME size".
  Determines Graphics Stolen Base, BDSM by subtracting the graphics data stolen memory size from TOLUD.
  Graphics Data Stolen Memory size is given by GMS field in GGC register.  It must be define before this stage.
  Determines Graphics GTT Stolen Base, BGSM by subtracting the GTT graphics stolen memory size from BDSM.
  GTT Stolen Memory size is given by GGMS field in GGC register.  It must be define before this stage.
  Determines TSEG Base, TSEGMB by subtracting TSEG size from BGSM.
  TSEG should be defined.
  Remove the memory hole caused by aligning TSEG to a 8MB boundary.
  Determine whether Memory Reclaim is available.  If "TOM minus ME Stolem Memory Size" is greater than the value of TOLUD, then memory reclaim is available to enable.
  Determine REMAPBASE if reclaim is enabled.  This is the maximum value by comparing between 4GB and "TOM minus ME size".
  Determine REMAPLIMIT () if reclaim is enabled.  This is the value of REMAPBASE plus "the difference between the value in TOLUD register and the lower of either 4GB or 'TOM minus ME Stolen memory size", and then minus 1 boundary.
  Determine TOUUD. TOUUD indicates the address one byte above the maximum DRAM.  If relcaim is disabled, this value is calculated by "TOM minus ME stolen size".  Otherwise, this value is set to REMAPLIMIT plus 1MB.

  @param[in, out] MrcData - Include all MRC global data. include also the memory map data.

  @retval EFI_SUCCESS           - Memory Map Setup is successful.
**/
typedef
EFI_STATUS
(EFIAPI *SET_MEMORY_MAP) (
  IN OUT MrcParameters *const MrcData
);

/**
  Perform Total Memory Encryption initialization.

  @param[in] TmePolicy             - TME policy

  @retval VOID - No value to return
**/
typedef
VOID
(*TME_INIT) (
  IN VOID  *TmePolicy
  );

/**
  This function configures the TME status register and notify if TME is enabled or not.
**/
typedef
VOID
(EFIAPI *PROGRAM_TME_STATUS) (
  VOID
  );

/**
  Program IA exclusion region below 4GB

  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_BELOW_4GB) (
  IN UINT32     MeStolenBase,
  IN UINT32     MeStolenSize
  );

/**
  Program IA exclusion region above 4GB
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_ABOVE_4GB) (
  VOID
  );

/**
  This function calls into MemoryMap APIs to program memory Map.

  @param[in] TotalPhysicalMemorySize  - DRAM Size
  @param[in] ToludBase                - Tolud Base Address
  @param[in] TouudBase                - Touud Base Address
  @param[in] TsegBase                 - Tseg Base Address
  @param[in] BdsmBase                 - BDSM Base Address
  @param[in] GttBase                  - GTT Base Address
  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1

**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_MEMORY_MAP_INIT) (
  IN UINT32     TotalPhysicalMemorySize,
  IN UINT32     ToludBase,
  IN UINT32     TouudBase,
  IN UINT32     TsegBase,
  IN UINT32     BdsmBase,
  IN UINT32     GttBase,
  IN UINT32     Mc0Size,
  IN UINT32     Mc1Size
  );

/**
  This function is to set IGPU Memory map (RC6 base, Doorbell base, Dfdbase etc)

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_NOT_FOUND           SiPreMemPolicyPpi not found
**/
typedef
EFI_STATUS
(*MEMORY_SUB_SYSTEM_SET_IGPU_MEM_MAP) (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi
  );

/**
  Program IMR and IMR_1M for all IPs.

  @param[in] TsegSize      - Size of TSEG

**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_IMR_INIT) (
  IN UINT32   TsegSize
  );

/**
  This function initialize NOC.

  @param[in] Mc0Size                  - Size of MC0
  @param[in] Mc1Size                  - Size of MC1
  @param[in] RemapBase                - Remap Base Address
  @param[in] RemapLimit               - Remap Limit
  @param[in] IsDdr5                   - Memory Type
**/
typedef
VOID
(EFIAPI *MEMORY_SUB_SYSTEM_NOC_INIT) (
  IN UINT32   Mc0Size,
  IN UINT32   Mc1Size,
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit,
  IN BOOLEAN  IsDdr5
  );


/**
  Configure CCE in MCHBAR register prior to MRC_DONE event.

  @param[in] RemapBase       - Remap Base Address
  @param[in] RemapLimit      - Remap Limit

  @retval VOID - No value to return
**/

typedef
VOID
(*MEMORY_SUB_SYSTEM_CCE_INIT) (
  IN UINT32   RemapBase,
  IN UINT32   RemapLimit
  );

/**
  Perform Trust Domain Execution(Tdx) initialization.

  @param[in] TdxPolicy           - TDx policy enable
  @param[in] MrcData             - MRC Parameter Structure
  @param[in] TdxActmModuleAddr   - Tdx Actm Module address
  @param[in] TdxActmModuleSize   - Tdx Actm Module Size

  @retval VOID              - No value to return
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_TDX_INIT) (
  IN VOID   *TdxPolicy,
  IN MrcParameters *CONST MrcData,
  IN UINTN  TdxActmModuleAddr,
  IN UINT32 TdxActmModuleSize,
  IN UINT8  TdxSeamldrSeSvn
  );

/**
  This is AliasCheck entry point for MRC call.  The routine checks for an TXT
  capable platform and determines whether ACHECK should be run.  If so,
  it launches the BIOS ACM to run ACHECK (which will reset the platform).
  The AliasCheck routine is in peitxtlib.

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_SUB_SYSTEM_ALIAS_CHECK) (
  VOID
  );

/**
  This is ClearSecretsBit entry point for MRC call.

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_SUB_SYSTEM_CLEAR_SECRETS_BIT) (
  VOID
  );

/**
  Set Power Down Request.

  @param[in] PdrSrc             Power down type
                            0 - No Power Down Request
                            1 - MRC requested Power Down due to scrubbing failure
                            2 - BIOS setup requested power down
                            3 - Power down through MOR interface

  @exception EFI_UNSUPPORTED  - If the platform is not TXT capable.
  @retval EFI_SUCCESS         - In all other cases not listed above.
**/
typedef
EFI_STATUS
(EFIAPI *MEMORY_SUB_SYSTEM_SET_POWER_DOWN_REQUEST) (
  UINT8 PdrSrc
  );

/**
  This function locks the memory controller and memory map registers.

  @param[in] MrcData - Include all MRC global data.
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_MC_REGISERS_LOCK) (
  IN     MrcParameters *const MrcData
  );

/**
  Lock IA exclusion region
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_LOCK) (
  VOID
  );

/**
  This function Lock HostBridge Memory Map registers
**/
typedef
VOID
(*MEMORY_SUB_SYSTEM_LOCK_HOSTBRIDGE_MEMORYMAP_REGS) (
  VOID
  );

/**
  This function Program NOC IMR Exclusion

  @param[in] SiPreMemPolicyPpi  Pointer to pre-mem SI policy.
  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
  @param[in] PrmrrBase    - PRMRR Base Address.
  @param[in] PrmrrSize    - PRMRR Size.
  @param[in] TdxEnabled   - TDX Policy.
  @param[in] SeamrrSize   - Seamrr region size.

**/
typedef
VOID
(*SET_NOC_IMR_EXCLUSION) (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN                       UINT32  MeStolenBase,
  IN                       UINT32  MeStolenSize,
  IN                       UINT32  PrmrrBase,
  IN                       UINT32  PrmrrSize,
  IN                       UINT32  TdxEnabled,
  IN                       UINT32  SeamrrSize,
  IN                       UINT32  TotalPhysicalMemorySize
  );

/**
  This function Program VGA IMR Exclusion

**/
typedef
VOID
(*SET_VGA_IMR_EXCLUSION) (
  VOID
  );

/**
  Set and Acknowledge MEM_CONFIG_DONE.

  @retval EFI_SUCCESS       MEM_CONFIG_DONE bit is set and acknowledged.
  @retval EFI_TIMEOUT       Timeout reached while checking MEM_CONFIG_DONE_ACK
**/
typedef
EFI_STATUS
(*MEMORY_SUB_SYSTEM_SET_MEM_CONFIG_DONE) (
  VOID
  );

/**
  Set the PRMRR MSRs.

  @param PrmrrBase PRMRR base address
  @param PrmrrSize PRMRR size

  @retval RETURN_SUCCESS       PRMRR MSRs are set successfully.
  @retval RETURN_UNSUPPORTED   PRMRR is not supported.
  @retval RETURN_ACCESS_DENIED PRMRR MSRs are already locked and cannot be changed.
*/
typedef
RETURN_STATUS
(EFIAPI *SET_PRMRR_REGION) (
  UINT64 PrmrrBase,
  UINT32 PrmrrSize
  );

/**
  This function Program DPR Lock Register

**/
typedef
VOID
(*LOCK_HOSTBRIDGE_DPR) (
  );

#pragma pack (push, 1)

typedef struct {
  SET_MEMORY_MAP                                   SetMemoryMap;
  MEMORY_SUB_SYSTEM_MEMORY_MAP_INIT                HostBridgeMemoryMapInit;
  TME_INIT                                         TmeInit;
  MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_BELOW_4GB      SetIaImrExclusionBelow4Gb;
  MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_ABOVE_4GB      SetIaImrExclusionAbove4Gb;
  MEMORY_SUB_SYSTEM_SET_IGPU_MEM_MAP               SetIGpuMemMap;
  MEMORY_SUB_SYSTEM_IMR_INIT                       ImrInit;
  MEMORY_SUB_SYSTEM_NOC_INIT                       NocInit;
  MEMORY_SUB_SYSTEM_CCE_INIT                       CceInit;
  MEMORY_SUB_SYSTEM_ALIAS_CHECK                    AliasCheck;
  MEMORY_SUB_SYSTEM_CLEAR_SECRETS_BIT              ClearSecretsBit;
  MEMORY_SUB_SYSTEM_SET_POWER_DOWN_REQUEST         SetPowerDownRequest;
  MEMORY_SUB_SYSTEM_MC_REGISERS_LOCK               McRegistersLock;
  MEMORY_SUB_SYSTEM_SET_IA_IMR_EXEC_LOCK           SetIaImrExclusionLock;
  MEMORY_SUB_SYSTEM_LOCK_HOSTBRIDGE_MEMORYMAP_REGS LockHostBridgeMemoryMapRegs;
  MEMORY_SUB_SYSTEM_SET_MEM_CONFIG_DONE            SetMemConfigDone;
  SET_NOC_IMR_EXCLUSION                            SetNocImrExclusion;
  SET_VGA_IMR_EXCLUSION                            SetVgaImrExclusion;
  SET_PRMRR_REGION                                 CpuSetPrmrrRegion;
  LOCK_HOSTBRIDGE_DPR                              LockDpr;
  MEMORY_SUB_SYSTEM_TDX_INIT                       TdxInit;
} MEMORY_SUB_SYSTEM_CALLBACK;

typedef struct  {
  MEMORY_SUB_SYSTEM_CALLBACK        *Callback;
} MEMORY_SUB_SYSTEM_HANDLE;

#pragma pack (pop)


/**
  Memory Sub System Handler Initialization

  @param[in] MemorySubSystemHandle    Hanlder for Memory Sub System
  @param[in] MemorySubSystemCallback  CallBack for Memory Sub System
**/
VOID
EFIAPI
MemorySubSystemHandleInit (
  IN OUT  MEMORY_SUB_SYSTEM_HANDLE    *MemorySubSystemHandle,
  IN OUT  MEMORY_SUB_SYSTEM_CALLBACK  *MemorySubSystemCallback
  );

#endif // _MEMORY_SUB_SYSTEM_HANDLE_H_
