/** @file
  PEI Memory Sub System Fru library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Pi/PiPeiCis.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <HostBridgeConfig.h>
#include <Defines/PchReservedResources.h>
#include <Library/NguInfoLib.h>
#include <Library/MemorySubSystemInfoLib.h>
#include <Ppi/SiPolicy.h>
#include <MemorySubSystemConfig.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <HostBridgeConfig.h>
#include <Defines/PchReservedResources.h>
#include <Library/TxtLib.h>
#include <Txt.h>
#include <Library/IGpuInfoLib.h>
#include <Library/TdxInfoLib.h>
#include <IGpuDataHob.h>
#include <Defines/HostBridgeDefines.h>

/**
  Validates the base address and updates the limit accordingly

  @param[in] BaseAddress  - BaseAddress
  @param[in] Size         - Size

  @retval    UINT64       - Return Limit.
**/
UINT64
VerifyAndCalculateLimit (
  UINT64 BaseAddress,
  UINT64 Size
 )
{
  if (BaseAddress == 0){
    return 0;
  } else if (BaseAddress == MAX_UINT32 || BaseAddress == MAX_UINT64) {
    return BaseAddress;
  } else {
    return BaseAddress + Size - 1;
  }
}

/**
  Add Address names to MemoryMapTable

  @param[in] MemoryMapTable  Pointer to the MemoryMapTable.
**/
VOID
EFIAPI
LoadMemoryMapDict (
  IN MEMORY_RANGE_TABLE *MemoryMapTable
  )
{
  StrCpyS (MemoryMapTable[RemapBaseAddress].BaseName, 30, L"REMAP Base");
  StrCpyS (MemoryMapTable[TraceHubBaseAddress].BaseName, 30, L"TraceHub");
  StrCpyS (MemoryMapTable[StreamTracerBaseAddress].BaseName, 30, L"StreamTracer");
  StrCpyS (MemoryMapTable[FlatCcsBaseAddress].BaseName, 30, L"Flat CCS");
  StrCpyS (MemoryMapTable[PrmrrBaseAddress].BaseName, 30, L"Prmrr");
  StrCpyS (MemoryMapTable[BiosAreaBaseAddress].BaseName, 30, L"BiosAreaBaseAddress");
  StrCpyS (MemoryMapTable[TemporaryRamBaseAddress].BaseName, 30, L"TemporaryRam");
  StrCpyS (MemoryMapTable[ApicLocalBaseAddress].BaseName, 30, L"ApicLocalAddress");
  StrCpyS (MemoryMapTable[MchBaseAddress].BaseName, 30, L"MchBaseAddress");
  StrCpyS (MemoryMapTable[Heci3BaseAddress].BaseName, 30, L"HECI3 BAR0 MMIO address");
  StrCpyS (MemoryMapTable[Heci2BaseAddress].BaseName, 30, L"HECI2 BAR0 MMIO address");
  StrCpyS (MemoryMapTable[Heci1BaseAddress].BaseName, 30, L"HECI1 BAR0 MMIO address");
  StrCpyS (MemoryMapTable[TxtConfigBaseAddress].BaseName, 30, L"TXT Config Space");
  StrCpyS (MemoryMapTable[HpetBaseAddress].BaseName, 30, L"HpetBase");
  StrCpyS (MemoryMapTable[IoApicBaseAddress].BaseName, 30, L"IoApicBase");
  StrCpyS (MemoryMapTable[TempMemBaseAddress].BaseName, 30, L"SiliconInitTempMemAddr");
  StrCpyS (MemoryMapTable[TraceHubRTITBaseAddress].BaseName, 30, L"TraceHub RTIT");
  StrCpyS (MemoryMapTable[TraceHubFWBaseAddress].BaseName, 30, L"TraceHub FW");
  StrCpyS (MemoryMapTable[TraceHubMTBBaseAddress].BaseName, 30, L"TraceHub MTB");
  StrCpyS (MemoryMapTable[eSpi2SEGmrBaseAddress].BaseName, 30, L"eSPI2 SEGMR BAR");
  StrCpyS (MemoryMapTable[eSpi2SLGmrBaseAddress].BaseName, 30, L"eSPI LGMR BAR");
  StrCpyS (MemoryMapTable[SerialIoBarBaseAddress].BaseName, 30, L"SerialIoUartDebugMmioBase");
  StrCpyS (MemoryMapTable[SpiBarBaseAddress].BaseName, 30, L"SPI BAR0");
  StrCpyS (MemoryMapTable[PwrmBarBaseAddress].BaseName, 30, L"PWRM BAR");
  StrCpyS (MemoryMapTable[TraceHubSWBarBaseAddress].BaseName, 30, L"TraceHub SW BAR");
  StrCpyS (MemoryMapTable[VtdMmioBaseAddress].BaseName, 30, L"VtdMmioBase");
  StrCpyS (MemoryMapTable[SbRegBaseAddress].BaseName, 30, L"SBREG");
  StrCpyS (MemoryMapTable[SbReg2BaseAddress].BaseName, 30, L"SBREG2");
  StrCpyS (MemoryMapTable[SafBarBaseAddress].BaseName, 30, L"SafBarBaseAddres");
  StrCpyS (MemoryMapTable[RegBarBaseAddress].BaseName, 30, L"RegBarBaseAddress");
  StrCpyS (MemoryMapTable[PciExpressBaseAddress].BaseName, 30, L"SiPciExpressBaseAddress");
  StrCpyS (MemoryMapTable[LMemBarBaseAddress].BaseName, 30, L"LMemBarBase");
  StrCpyS (MemoryMapTable[GttMmAdrBaseAddress].BaseName, 30, L"GTBaseAddress");
  StrCpyS (MemoryMapTable[VmdMemBar2BaseAddress].BaseName, 30, L"VmdMemBar2Base");
  StrCpyS (MemoryMapTable[VmdMemBar1BaseAddress].BaseName, 30, L"VmdMemBar1Base");
  StrCpyS (MemoryMapTable[VmdCfgBarBaseAddress].BaseName, 30, L"VmdMemBarBase");
  StrCpyS (MemoryMapTable[NvmeHcPeiMmioBaseAddress].BaseName, 30, L"NvmeHcPeiMmioBase");
  StrCpyS (MemoryMapTable[AhciPeiMmioBaseAddress].BaseName, 30, L"AhciPeiMmioBase");
  StrCpyS (MemoryMapTable[BdsmBaseAddress].BaseName, 30, L"BDSM base");
  StrCpyS (MemoryMapTable[GttBaseAddress].BaseName, 30, L"GTT base");
  StrCpyS (MemoryMapTable[TsegBaseAddress].BaseName, 30, L"TSEG base");
  StrCpyS (MemoryMapTable[MeStolenBaseAddress].BaseName, 30, L"MeStolen Base");
  StrCpyS (MemoryMapTable[TseImrBaseAddress].BaseName, 30, L"TSE IMR Base");
  StrCpyS (MemoryMapTable[SeamrrBaseAddress].BaseName, 30, L"SEAMRR Base");
}

/**
  Check for any memory overlap in the address map

  @param[in] MemoryMapTable  POinter to the MemoryMapTable.
**/
VOID
EFIAPI
MemoryMapTableOverlapCheck (
  IN MEMORY_RANGE_TABLE *MemoryMapTable
  )
{
  UINT32  MemCheck;
  UINT32  MemRanges;

  for (MemCheck = 0 ; MemCheck < MemoryMapTableLength; MemCheck++ ) {
    if (MemoryMapTable[MemCheck].Base == 0 || MemoryMapTable[MemCheck].Base == MAX_UINT32 || MemoryMapTable[MemCheck].Base == MAX_UINT64) {
      continue;
    }
    for (MemRanges = MemCheck + 1 ; MemRanges < MemoryMapTableLength; MemRanges++ ) {
      if (MemoryMapTable[MemRanges].Base == 0 || MemoryMapTable[MemRanges].Base == MAX_UINT32 || MemoryMapTable[MemRanges].Base == MAX_UINT64) {
        continue;
      }
      if ((MemoryMapTable[MemRanges].Base <= MemoryMapTable[MemCheck].Base) && (MemoryMapTable[MemCheck].Base <= MemoryMapTable[MemRanges].Limit)) {
        DEBUG ((DEBUG_INFO, "Memory overlap,  %s Base = 0x%lx is already used in %s range 0x%lx - 0x%lx \n", MemoryMapTable[MemCheck].BaseName, MemoryMapTable[MemCheck].Base, MemoryMapTable[MemRanges].BaseName, MemoryMapTable[MemRanges].Base, MemoryMapTable[MemRanges].Limit));
        ASSERT(FALSE);
      } else if ((MemoryMapTable[MemRanges].Base <= MemoryMapTable[MemCheck].Limit) && (MemoryMapTable[MemCheck].Limit <= MemoryMapTable[MemRanges].Limit)) {
        DEBUG ((DEBUG_INFO, "Memory overlap, %s Limit = 0x%lx is already used in  %s the range 0x%lx - 0x%lx \n", MemoryMapTable[MemCheck].BaseName, MemoryMapTable[MemCheck].Limit, MemoryMapTable[MemRanges].BaseName, MemoryMapTable[MemRanges].Base, MemoryMapTable[MemRanges].Limit));
        ASSERT(FALSE);
      }
      if ((MemoryMapTable[MemCheck].Base <= MemoryMapTable[MemRanges].Base) && (MemoryMapTable[MemRanges].Limit <= MemoryMapTable[MemCheck].Limit)) {
        DEBUG ((DEBUG_INFO, "Memory overlap, In the %s 0x%lx - 0x%lx, %s 0x%lx - 0x%lx already mapped \n", MemoryMapTable[MemCheck].BaseName, MemoryMapTable[MemCheck].Base, MemoryMapTable[MemCheck].Limit, MemoryMapTable[MemRanges].BaseName, MemoryMapTable[MemRanges].Base, MemoryMapTable[MemRanges].Limit));
        ASSERT(FALSE);
      }
    }
  }
}

/**
  Load MemoryMapTable with Memory Address details

  @param[in] MemoryMapTable      - POinter to MemoryMapTable.
**/
VOID
EFIAPI
LoadMemoryMapTable (
  IN OUT MEMORY_RANGE_TABLE *MemoryMapTable
  )
{
  HOST_BRIDGE_PREMEM_CONFIG           *HostBridgePreMemConfig;
  SI_PREMEM_POLICY_PPI                *SiPreMemPolicyPpi;
  EFI_STATUS                           Status;

  SiPreMemPolicyPpi         = NULL;
  HostBridgePreMemConfig     = NULL;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot locate gSiPreMemPolicyPpiGuid, Status = %r\n", Status));
  }
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ///
  /// Memory Map Table filling
  /// Base above 4GB
  //
  MemoryMapTable[SbRegBaseAddress].Base       = PCH_PCR_BASE_ADDRESS;
  MemoryMapTable[SbRegBaseAddress].Limit      = PCH_PCR_BASE_ADDRESS + PCH_PCR_MMIO_SIZE - 1;
  MemoryMapTable[SbReg2BaseAddress].Base      = PCH_PCR_SECOND_BASE_ADDRESS;
  MemoryMapTable[SbReg2BaseAddress].Limit     = PCH_PCR_SECOND_BASE_ADDRESS + PCH_PCR_MMIO_SIZE - 1;
  MemoryMapTable[SafBarBaseAddress].Base      = PcdGet64 (PcdSafBarBaseAddress);
  MemoryMapTable[SafBarBaseAddress].Limit     = PcdGet64 (PcdSafBarBaseAddress) + SIZE_32MB - 1;
  MemoryMapTable[RemapBaseAddress].Base       = GetRemapBase ();
  MemoryMapTable[RemapBaseAddress].Limit      = GetRemapLimit ();

  MemoryMapTable[TraceHubBaseAddress].Base        = GetImrRegisterData (NPK, IMR_BASE_BYTE);
  MemoryMapTable[TraceHubBaseAddress].Limit       = VerifyAndCalculateLimit (MemoryMapTable[TraceHubBaseAddress].Base, GetImrRegisterData (NPK, IMR_SIZE_BYTE));
  if (IsTdxEnabled ()) {
    MemoryMapTable[SeamrrBaseAddress].Base        = GetImrRegisterData (SEAMRR, IMR_BASE_BYTE);
    MemoryMapTable[SeamrrBaseAddress].Limit       = VerifyAndCalculateLimit (MemoryMapTable[SeamrrBaseAddress].Base, GetImrRegisterData (SEAMRR, IMR_SIZE_BYTE));
  }
  MemoryMapTable[StreamTracerBaseAddress].Base    = GetImrRegisterData (STREAM_TRACER, IMR_BASE_BYTE);
  MemoryMapTable[StreamTracerBaseAddress].Limit   = VerifyAndCalculateLimit (MemoryMapTable[StreamTracerBaseAddress].Base, GetImrRegisterData (STREAM_TRACER, IMR_SIZE_BYTE));
  if (IGpuIsSupported ()) {
    MemoryMapTable[FlatCcsBaseAddress].Base       = GetImrRegisterData (CCS, IMR_BASE_BYTE);
    MemoryMapTable[FlatCcsBaseAddress].Limit      = GetImrRegisterData (CCS, IMR_LIMIT_BYTE) - 1;
  }
  MemoryMapTable[PrmrrBaseAddress].Base           = GetImrRegisterData (PRMRR, IMR_BASE_BYTE);
  MemoryMapTable[PrmrrBaseAddress].Limit          = VerifyAndCalculateLimit (MemoryMapTable[PrmrrBaseAddress].Base, GetImrRegisterData (PRMRR, IMR_SIZE_BYTE));

  //
  // Base below 4GB
  //
  MemoryMapTable[BiosAreaBaseAddress].Base        = FixedPcdGet64 (PcdBiosAreaBaseAddress);
  MemoryMapTable[BiosAreaBaseAddress].Limit       = FixedPcdGet64 (PcdBiosAreaBaseAddress) + FixedPcdGet64 (PcdBiosSize) - 1;
  MemoryMapTable[TemporaryRamBaseAddress].Base    = FixedPcdGet64 (PcdTemporaryRamBase);
  MemoryMapTable[TemporaryRamBaseAddress].Limit   = FixedPcdGet64 (PcdTemporaryRamBase) + FixedPcdGet64 (PcdTemporaryRamSize) - 1;
  MemoryMapTable[MchBaseAddress].Base             = FixedPcdGet64 (PcdMchBaseAddress);
  MemoryMapTable[MchBaseAddress].Limit            = FixedPcdGet64 (PcdMchBaseAddress) + FixedPcdGet64 (PcdMchMmioSize) - 1;
  MemoryMapTable[Heci3BaseAddress].Base           = PCH_HECI3_BASE_ADDRESS;
  MemoryMapTable[Heci3BaseAddress].Limit          = PCH_HECI3_BASE_ADDRESS + PCH_HECI3_MMIO_SIZE - 1;
  MemoryMapTable[Heci2BaseAddress].Base           = PCH_HECI2_BASE_ADDRESS;
  MemoryMapTable[Heci2BaseAddress].Limit          = PCH_HECI2_BASE_ADDRESS + PCH_HECI2_MMIO_SIZE - 1;
  MemoryMapTable[Heci1BaseAddress].Base           = PCH_HECI1_BASE_ADDRESS;
  MemoryMapTable[Heci1BaseAddress].Limit          = PCH_HECI1_BASE_ADDRESS + PCH_HECI1_MMIO_SIZE - 1;
  MemoryMapTable[TxtConfigBaseAddress].Base       = TXT_PRIVATE_BASE;
  MemoryMapTable[TxtConfigBaseAddress].Limit      = TXT_PRIVATE_BASE + TXT_CONFIG_SPACE_LENGTH - 1;
  MemoryMapTable[HpetBaseAddress].Base            = FixedPcdGet64 (PcdSiHpetBaseAddress);
  MemoryMapTable[HpetBaseAddress].Limit           = TXT_PRIVATE_BASE - 1;
  MemoryMapTable[IoApicBaseAddress].Base          = FixedPcdGet64 (PcdSiIoApicBaseAddress);
  MemoryMapTable[IoApicBaseAddress].Limit         = FixedPcdGet64 (PcdSiIoApicBaseAddress) + SIZE_4KB - 1;
  MemoryMapTable[TempMemBaseAddress].Base         = FixedPcdGet64 (PcdSiliconInitTempMemBaseAddr);
  MemoryMapTable[TempMemBaseAddress].Limit        = FixedPcdGet64 (PcdSiliconInitTempMemBaseAddr) + FixedPcdGet64 (PcdSiliconInitTempMemSize) - 1;
  MemoryMapTable[TraceHubRTITBaseAddress].Base    = PCH_TRACE_HUB_RTIT_BASE_ADDRESS;
  MemoryMapTable[TraceHubRTITBaseAddress].Limit   = PCH_TRACE_HUB_RTIT_BASE_ADDRESS + PCH_TRACE_HUB_RTIT_MMIO_SIZE - 1;
  MemoryMapTable[TraceHubFWBaseAddress].Base      = PCH_TRACE_HUB_FW_BASE_ADDRESS;
  MemoryMapTable[TraceHubFWBaseAddress].Limit     = PCH_TRACE_HUB_FW_BASE_ADDRESS + PCH_TRACE_HUB_FW_MMIO_SIZE - 1;
  MemoryMapTable[TraceHubMTBBaseAddress].Base     = PCH_TRACE_HUB_MTB_BASE_ADDRESS;
  MemoryMapTable[TraceHubMTBBaseAddress].Limit    = PCH_TRACE_HUB_MTB_BASE_ADDRESS + PCH_TRACE_HUB_MTB_MMIO_SIZE - 1;
  MemoryMapTable[eSpi2SEGmrBaseAddress].Base      = PCH_ESPI_SEGMR_BASE_ADDRESS;
  MemoryMapTable[eSpi2SEGmrBaseAddress].Limit     = PCH_ESPI_SEGMR_BASE_ADDRESS + PCH_ESPI_SEGMR_MMIO_SIZE - 1;
  MemoryMapTable[eSpi2SLGmrBaseAddress].Base      = PCH_ESPI_LGMR_BASE_ADDRESS;
  MemoryMapTable[eSpi2SLGmrBaseAddress].Limit     = PCH_ESPI_LGMR_BASE_ADDRESS + PCH_ESPI_LGMR_MMIO_SIZE - 1;
  MemoryMapTable[SerialIoBarBaseAddress].Base     = PCH_SERIAL_IO_BASE_ADDRESS;
  MemoryMapTable[SerialIoBarBaseAddress].Limit    = PCH_SERIAL_IO_BASE_ADDRESS + PCH_SERIAL_IO_MMIO_SIZE - 1;
  MemoryMapTable[SpiBarBaseAddress].Base          = PCH_SPI_BASE_ADDRESS;
  MemoryMapTable[SpiBarBaseAddress].Limit         = PCH_SPI_BASE_ADDRESS + PCH_SPI_MMIO_SIZE - 1;
  MemoryMapTable[PwrmBarBaseAddress].Base         = PCH_PWRM_BASE_ADDRESS;
  MemoryMapTable[PwrmBarBaseAddress].Limit        = PCH_PWRM_BASE_ADDRESS + PCH_PWRM_MMIO_SIZE - 1;
  MemoryMapTable[TraceHubSWBarBaseAddress].Base   = PCH_TRACE_HUB_SW_BASE_ADDRESS;
  MemoryMapTable[TraceHubSWBarBaseAddress].Limit  = PCH_TRACE_HUB_SW_BASE_ADDRESS + PCH_TRACE_HUB_SW_MMIO_SIZE - 1;
  MemoryMapTable[VtdMmioBaseAddress].Base         = FixedPcdGet64 (PcdVtdMmioBase);
  MemoryMapTable[VtdMmioBaseAddress].Limit        = FixedPcdGet64 (PcdVtdMmioBase) + FixedPcdGet64 (PcdVtdMmioSize) - 1;
  MemoryMapTable[RegBarBaseAddress].Base          = PcdGet64 (PcdRegBarBaseAddress);
  MemoryMapTable[RegBarBaseAddress].Limit         = PcdGet64 (PcdRegBarBaseAddress) + V_SA_MCHBAR_REGBAR_SIZE  - 1;
  MemoryMapTable[PciExpressBaseAddress].Base      = PcdGet64 (PcdSiPciExpressBaseAddress);
  MemoryMapTable[PciExpressBaseAddress].Limit     = PcdGet64 (PcdSiPciExpressBaseAddress) + PcdGet32 (PcdPciExpressRegionLength) - 1;
  if (IGpuIsSupported ()) {
    MemoryMapTable[LMemBarBaseAddress].Base         = IGpuGetLMemBar ();
    MemoryMapTable[LMemBarBaseAddress].Limit        = IGpuGetLMemBar () + IGpuGetLMemBarSize () - 1;
    MemoryMapTable[GttMmAdrBaseAddress].Base        = IGpuGetGttMmAdrBase ();
    MemoryMapTable[GttMmAdrBaseAddress].Limit       = IGpuGetGttMmAdrBase () + SIZE_16MB - 1;
  }
  if (HostBridgePreMemConfig != NULL) {
    MemoryMapTable[ApicLocalBaseAddress].Base       = HostBridgePreMemConfig->ApicLocalAddress;
    MemoryMapTable[ApicLocalBaseAddress].Limit      = HostBridgePreMemConfig->ApicLocalAddress + SIZE_4KB - 1;
    MemoryMapTable[VmdMemBar2BaseAddress].Base      = HostBridgePreMemConfig->VmdMemBar2Bar;
    MemoryMapTable[VmdMemBar2BaseAddress].Limit     = HostBridgePreMemConfig->VmdMemBar2Bar + SIZE_1MB - 1;
    MemoryMapTable[VmdMemBar1BaseAddress].Base      = HostBridgePreMemConfig->VmdMemBar1Bar;
    MemoryMapTable[VmdMemBar1BaseAddress].Limit     = HostBridgePreMemConfig->VmdMemBar1Bar + SIZE_32MB - 1;
    MemoryMapTable[VmdCfgBarBaseAddress].Base       = HostBridgePreMemConfig->VmdCfgBarBar;
    MemoryMapTable[VmdCfgBarBaseAddress].Limit      = HostBridgePreMemConfig->VmdCfgBarBar + SIZE_32MB - 1;
    MemoryMapTable[NvmeHcPeiMmioBaseAddress].Base   = HostBridgePreMemConfig->NvmeHcPeiMmioBase;
    MemoryMapTable[NvmeHcPeiMmioBaseAddress].Limit  = HostBridgePreMemConfig->NvmeHcPeiMmioLimit - 1;
    MemoryMapTable[AhciPeiMmioBaseAddress].Base     = HostBridgePreMemConfig->AhciPeiMmioBase;
    MemoryMapTable[AhciPeiMmioBaseAddress].Limit    = HostBridgePreMemConfig->AhciPeiMmioLimit - 1;
  }
  //
  // Below TOLUD
  //
  if (IGpuIsSupported ()) {
    MemoryMapTable[BdsmBaseAddress].Base          = GetImrRegisterData (DSM, IMR_BASE_BYTE);
    MemoryMapTable[BdsmBaseAddress].Limit         = GetImrRegisterData (DSM, IMR_LIMIT_BYTE) - 1;
    MemoryMapTable[GttBaseAddress].Base           = GetImrRegisterData (GSM, IMR_BASE_BYTE);
    MemoryMapTable[GttBaseAddress].Limit          = GetImrRegisterData (GSM, IMR_LIMIT_BYTE) - 1;
  }
  MemoryMapTable[TsegBaseAddress].Base            = GetImrRegisterData (TSEG, IMR_BASE_BYTE);
  MemoryMapTable[TsegBaseAddress].Limit           = GetImrRegisterData (TSEG, IMR_LIMIT_BYTE) - 1;
  MemoryMapTable[MeStolenBaseAddress].Base        = GetImrRegisterData (CSE, IMR_BASE_BYTE);
  MemoryMapTable[MeStolenBaseAddress].Limit       = VerifyAndCalculateLimit (MemoryMapTable[MeStolenBaseAddress].Base, GetImrRegisterData (CSE, IMR_SIZE_BYTE));
  MemoryMapTable[TseImrBaseAddress].Base          = GetImrRegisterData (TSE, IMR_BASE_BYTE);
  MemoryMapTable[TseImrBaseAddress].Limit         = VerifyAndCalculateLimit (GetImrRegisterData (TSE, IMR_BASE_BYTE), GetImrRegisterData (TSE, IMR_SIZE_BYTE));
}

/**
  Collect memory mapping details and check for overlaps in them

**/
VOID
EFIAPI
CheckMemoryMapOverlap (
  )
{
  SI_PREMEM_POLICY_PPI                *SiPreMemPolicyPpi;
  EFI_STATUS                           Status;
  MEMORY_RANGE_TABLE                   MemoryMapTable[MemoryMapTableLength];
  MEMORY_SUB_SYSTEM_CONFIG            *MemorySubSystemConfig;

  SiPreMemPolicyPpi         = NULL;
  MemorySubSystemConfig     = NULL;

  DEBUG ((DEBUG_INFO, "%a ()\n", __FUNCTION__));

  ///
  /// Get Policy settings through the SiPreMemPolicy PPI
  ///
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot locate gSiPreMemPolicyPpiGuid, Status = %r\n", Status));
  }
  Status  = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  ASSERT_EFI_ERROR (Status);

  if ((MemorySubSystemConfig != NULL) && (MemorySubSystemConfig->MemMapOverlapCheckSupport == 1)) {
    DEBUG ((DEBUG_INFO, "MemoryMapOverlapCheck Enabled \n"));
    ZeroMem (MemoryMapTable, sizeof (MEMORY_RANGE_TABLE) * MemoryMapTableLength);
    LoadMemoryMapTable (MemoryMapTable);
    LoadMemoryMapDict (MemoryMapTable);
    MemoryMapTableOverlapCheck (MemoryMapTable);
  }
  DEBUG ((DEBUG_INFO, "%a () end\n", __FUNCTION__));
}
