/** @file
  PeiHostBridgeInitLib header file

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
#ifndef _HOST_BRIDGE_INIT_PTL_CDIE_H_
#define _HOST_BRIDGE_INIT_PTL_CDIE_H_

#include <HostBridgeConfig.h>
#include <HostBridgeDataHob.h>
#include <CMrcInterface.h>

/**
  Programs HostBridge Bars

**/
VOID
ProgramHostBridgeBars (
  VOID
  );

/**
  Programs DMI BAR.
**/
VOID
ProgramHostBridgeDmiBar (
  VOID
  );

/**
  Determine MMIO Size for Dynamic Tolud

  @param[in] PchPcieMmioLength - Total PCIe MMIO length on all PCH root ports
  @param[in] PegMmioLength     - Total PEG MMIO length on all PEG ports
  @param[in] IGpuMmioLength    - Total IGPU MMIO length
**/
VOID
MmioSizeCalculation (
  IN    UINT32  PchPcieMmioLength,
  IN    UINT32  PegMmioLength,
  IN    UINT32  IGpuMmioLength
  );

/**
  Program IA/GT exclusion region

  @param[in] IMR Base
  @param[in] IMR Limit
**/
VOID
SetIaGtImrExclusion (
  UINT32  ImrBase,
  UINT32  ImrLimit
  );

/**
  Lock IA/GT exclusion region
**/
VOID
SetIaGtImrExclusionLock (
  VOID
  );

/**
  This function program TOLUD register

  @param[in] TouldBase
**/
VOID
ProgramTolud (
  UINT32  ToludBase
  );

/**
  This function program TOUUD register

  @param[in] TouldBase
**/
VOID
ProgramTouud (
  UINT32  TouudBase
  );

/**
  This function program Tom register

  @param[in] TouudBase
**/
VOID
ProgramTom (
  UINT32  TotalMemorySize
  );

/**
  This function program Tseg register

  @param[in] TsegBase
**/
VOID
ProgramTseg (
  UINT32  TsegBase
  );

/**
  This function program BDSM register

  @param[in] BdsmBase
**/
VOID
ProgramBdsm (
  UINT32  BdsmBase
  );

/**
  This function program BGSM register

  @param[in] BgsmBase
**/
VOID
ProgramBgsm (
  UINT32  BgsmBase
  );

/**
  This function program Graphic Control register
**/
VOID
ProgramGfxCr (
  VOID
  );

/**
  This function Lock HostBridge Memory Map registers
**/
VOID
LockHostBridgeMemoryMapRegs (
  VOID
  );

/**
  ProgramEdramMode - Disable EDRAM by default and enable it through HW Test Mode policy if needed

  @param[in] HostBridgePeiConfig - Instance of HOST_BRIDGE_PEI_CONFIG
**/
VOID
ProgramEdramMode (
  IN    HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig
  );

/**
  Init and Install Host Bridge Data Hob

  @param[in] HOST_BRIDGE_PREMEM_CONFIG   - Instance of HOST_BRIDGE_PREMEM_CONFIG
  @param[out] HOST_BRIDGE_DATA_HOB       - HOST_BRIDGE_DATA_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallHostBridgeDataHob (
  IN    HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig,
  OUT   HOST_BRIDGE_DATA_HOB        **HostBridgeDataHobOut
  );

/**
  Update HostBridge Hob in PostMem

  @param[in] HostBridgePeiConfig    Instance of HOST_BRIDGE_PEI_CONFIG

  @retval EFI_SUCCESS
**/
EFI_STATUS
UpdateHostBridgeHobPostMem (
  IN    HOST_BRIDGE_PEI_CONFIG      *HostBridgePeiConfig
  );

/**
  Program IA exclusion region below 4GB

  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
**/
VOID
SetIaImrExclusionBelow4Gb (
  IN UINT32     MeStolenBase,
  IN UINT32     MeStolenSize
  );

/**
  Program IA exclusion region above 4GB
**/
VOID
SetIaImrExclusionAbove4Gb (
  VOID
  );

/**
  Lock IA exclusion region
**/
VOID
SetIaImrExclusionLock (
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
VOID
HostBridgeMemoryMapInit (
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
  This function program Mem Subsystem Config register

  @param[in] Mc0Size - Size of MC0
  @param[in] Mc1Size - Size of MC1
**/
VOID
ProgramMemSsConfig (
  IN UINT32     Mc0Size,
  IN UINT32     Mc1Size
  );

/**
  This function calls to print memory Map.

  @param[in] MrcData             - The MRC "global data" area.

**/
VOID
PrintMemoryMap (
  IN MrcParameters *CONST  MrcData
  );

/**
  Set and Acknowledge MEM_CONFIG_DONE.

  @retval EFI_SUCCESS       MEM_CONFIG_DONE bit is set and acknowledged.
  @retval EFI_TIMEOUT       Timeout reached while checking MEM_CONFIG_DONE_ACK
**/
EFI_STATUS
SetMemConfigDone (
  VOID
  );

/**
  This function is to lock PAM region at End Of firmware.

**/
VOID
PamLockEndOfFirmware (
  VOID
  );

/**
  This function Program NOC IMR Exclusion

  @param[in] MrcData - The MRC "global data" area.
  @param[in] MeStolenBase - ME Stolen Base Address.
  @param[in] MeStolenSize - ME Stolen Size.
  @param[in] PrmrrBase    - PRMRR Base Address.
  @param[in] PrmrrSize    - PRMRR Size.
  @param[in] TdxEnabled   - TDX Policy.
  @param[in] SeamrrSize   - Seamrr region size.

**/
VOID
SetNocImrExclusion (
  IN SI_PREMEM_POLICY_PPI  *SiPreMemPolicyPpi,
  IN UINT32                MeStolenBase,
  IN UINT32                MeStolenSize,
  IN UINT32                PrmrrBase,
  IN UINT32                PrmrrSize,
  IN UINT32                TdxEnabled,
  IN UINT32                SeamrrSize,
  IN UINT32                TotalPhysicalMemorySize
  );

/**
  This function program VGA IMR Exclusion

**/
VOID
SetVgaImrExclusion (
  VOID
  );

/**
  This function DPR Lock Register

**/
VOID
LockDpr (
  );

/**
  This function will dynamically calculate the MMIO Size Requirement
  @param[in] MmioSize             - Size of MMIO space reserved for devices
  @param[in] MmioSizeAdjustment   - MMIO size adjustment
  @retval RequiredMmio
**/
UINT32
EFIAPI
DynamicMmioCalculation (
  IN UINT32  MmioSize,
  IN UINT32  MmioSizeAdjustment
  );

/**
  This function performs SA internal devices enabling/disabling

  @param[in] HostBridgePeiConfig - Instance of HOST_BRIDGE_PEI_CONFIG

**/
VOID
DeviceConfigure(
  IN    HOST_BRIDGE_PEI_CONFIG  *HostBridgePeiConfig
  );

/**
  This function is to Set BIOS_RESET_CPL bits

  @retval None
**/
VOID
SetBiosResetCpl (
  VOID
  );

/**
  Enable VLW's

  @param[in] HostBridgePreMemConfig   Instance of HOST_BRIDGE_PREMEM_CONFIG

**/
VOID
VlwEnable (
  IN  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig
  );
/**
  This function is to Set WAC/CP for IMRGLOBAL_BM
  @retval None
**/

VOID
SetImrGlobalSai (
  VOID
  );

/**
  This function overrides the default configurations in the UPD data region.

  @param   UpdDataRgn   A pointer to the UPD_DATA_REGION data strcture.

  @return  None
**/
VOID
PlatformLateInit (
  VOID
  );

/**
  This function will return MmioSize.

  @retval UINT32 returns MmioSize
**/
UINT32
GetPreMemMmioSize (
  VOID
  );
#endif
