/** @file
  PEIM to initialize VT-d FRU.

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
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/HobLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/VtdInfoLib.h>
#include <Library/IaxInfoLib.h>
#include <Library/NpuInfoLib.h>
#include <Library/IpuInitLib.h>
#include <Ppi/SiPolicy.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/HacRegs.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PciSegmentLib.h>
#include <Library/IGpuInfoLib.h>
#include <Fru/PtlPcd/IncludePrivate/Library/PtlPcdPsfSocLib.h>

/**
  The function is responsible for setting vtden bit in all PSFs (RS0) in CPU

  Vtden bit will force all upstream transaction to go to IOMMU so will no bypass vtd logic. This has been necessary since PSF 2.0.
**/
VOID
PsfEnableVtd (
  VOID
  )
{
  ///
  /// Enable SOC PSF for VT-d
  ///
  PtlPcdPsfEnableVtd ();
}

/**
  Configure VT-d BAR in pre-mem.

  In pre-mem BIOS FW initializes VT-d BAR and enables Gfx VT-d engine if needed and
  global VT-d engine.

  @param[in] Vtd               VT-d config block

  @retval    EFI_DEVICE_ERROR  VT-d BAR cannot be enabled
  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
ConfigureVtdBarPreMem (
  IN  VTD_CONFIG  *Vtd
  )
{
  VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT  VtdBar;
  VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT  ReadVtdBar;
  UINTN                             MchBar;

  MchBar = (UINT32)GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  VtdBar.Data32[0]     = FixedPcdGet32 (PcdVtdMmioBase);
  VtdBar.Bits.vtdbaren = 0;

  //
  // Enable GFX VT-d engine only if Intel Graphics is supported and enabled
  //
  if (IGpuIsSupported ()) {
    VtdBar.Bits.vtdbaren |= LShiftU64 (1, GFX_VTD);
  }

  //
  // Enable global VT-d engine
  //
  VtdBar.Bits.vtdbaren |= LShiftU64 (1, GLOBAL_VTD);

  MmioWrite32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG, VtdBar.Data32[0]);
  ReadVtdBar.Data32[0] = MmioRead32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG);

  DEBUG ((DEBUG_INFO, "VT-d BAR in pre-mem: 0x%x\n", ReadVtdBar.Data32[0]));

  //
  // Verify if VT-d BAR is programmed as expected
  //
  if (ReadVtdBar.Bits.vtdbaren == 0) {
    DEBUG ((DEBUG_ERROR, "VT-d BAR cannot be programmed properly\n"));
    MmioWrite32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG, 0);
    ASSERT (ReadVtdBar.Bits.vtdbaren != 0);
    return EFI_DEVICE_ERROR;
  }

  //
  // This function builds a HOB that describes a chunk of system memory.
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_MEMORY_MAPPED_IO,
    (EFI_RESOURCE_ATTRIBUTE_PRESENT | EFI_RESOURCE_ATTRIBUTE_INITIALIZED | EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE),
    FixedPcdGet32 (PcdVtdMmioBase),
    FixedPcdGet32 (PcdVtdMmioSize)
    );

  //
  // This function builds a HOB for the memory allocation.
  //
  BuildMemoryAllocationHob (
    FixedPcdGet32 (PcdVtdMmioBase),
    FixedPcdGet32 (PcdVtdMmioSize),
    EfiMemoryMappedIO
    );

  return EFI_SUCCESS;
}

/**
  Configure VT-d BAR in post-mem.

  It enables non-Gfx VT-d engine if at least one of devices which are within
  scope of non-Gfx VT-d engine is enabled.

  @param[in] Vtd               VT-d config block

  @retval    EFI_DEVICE_ERROR  VT-d BAR cannot be enabled
  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
ConfigureVtdBarPostMem (
  IN  VTD_CONFIG  *Vtd
  )
{
  VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT         VtdBar;
  VTDBAR_NCU_CCF_IDP_MCHBAR_STRUCT         ReadVtdBar;
  UINTN                                    MchBar;
  UINT64                                   CamarilloBaseAddress;
  BOOLEAN                                  CamarilloEnabled;
  UINT64                                   TelemetryBaseAddress;
  BOOLEAN                                  TelemetryEnabled;

  MchBar = (UINT32)GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  VtdBar.Data32[0] = MmioRead32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG);

  //
  // Check if Camarillo is enabled
  //
  CamarilloBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, SA_PCI_DEV_4, SA_PCI_FUN_0, 0);

  if (PciSegmentRead16 (CamarilloBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    CamarilloEnabled = FALSE;
  } else {
    CamarilloEnabled = TRUE;
  }

  //
  // Check if Telemetry is enabled
  //
  TelemetryBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, 0, SA_PCI_DEV_A, SA_PCI_FUN_0, 0);

  if (PciSegmentRead16 (TelemetryBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    TelemetryEnabled = FALSE;
  } else {
    TelemetryEnabled = TRUE;
  }

  //
  // Enable non-GFX VT-d engine only if at least on of accelarators (IPU/NPU/IAX/Camarillo/Telemetry) is enabled
  //
  if (IsIpuEnabled () || IsNpuEnabled () || IsIaxEnabled () || CamarilloEnabled || TelemetryEnabled) {
    VtdBar.Bits.vtdbaren |= LShiftU64 (1, NON_GFX_VTD);
  }

  MmioWrite32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG, VtdBar.Data32[0]);
  ReadVtdBar.Data32[0] = MmioRead32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG);

  DEBUG ((DEBUG_INFO, "VT-d BAR in post-mem: 0x%x\n", ReadVtdBar.Data32[0]));

  return EFI_SUCCESS;
}

/**
  Disable VT-d BAR.

  @retval    EFI_SUCCESS       Function executed successfully
**/
EFI_STATUS
DisableVtdBar (
  VOID
  )
{
  UINTN  MchBar;

  MchBar = (UINT32)GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  if (MmioRead32 (MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG) != 0) {
    MmioWrite32 ((MchBar + VTDBAR_NCU_CCF_IDP_MCHBAR_REG), 0);
  }

  return EFI_SUCCESS;
}
/**
  Check if VT-d is disabled.


  @param[in] Vtd               VT-d config block

  @retval    FALSE  VT-d is enabled
  @retval    TRUE   VT-d is disabled
**/
BOOLEAN
IsVtdDisabled (
  IN  VTD_CONFIG  *Vtd
  )
{
  if (Vtd->VtdDisable) {
    return TRUE;
  }

  return FALSE;

}