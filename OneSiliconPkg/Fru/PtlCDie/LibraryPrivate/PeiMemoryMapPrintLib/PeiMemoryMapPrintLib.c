/** @file
  PEIM Private Library to initialize PeiMemoryMapPrintLib registers

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


#include <Ppi/SiPolicy.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciSegmentLib.h>
#include <CMrcInterface.h>
#include <Library/PcdLib.h>
#include <MePeiConfig.h>
#include <HostBridgeConfig.h>
#include <Register/PttPtpRegs.h>
#include <Txt.h>
#include <Base.h>
#include <Defines/PchReservedResources.h>
#include <IGpuConfig.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Defines/IgdDefines.h>
#include <Library/NguInfoLib.h>
#include <Library/IGpuInfoLib.h>
#include <TelemetryPeiConfig.h>
#include <Library/PeiTelemetryFruLib.h>
#include <Library/PeiTdxLib.h>
#include <Library/TdxInfoLib.h>
#include <Library/HobLib.h>
#include <IGpuDataHob.h>
#include <Defines/HostBridgeDefines.h>

/**
  This Function calls to Print Memory Map

  @param[in] MrcData             - The MRC "global data" area.
**/

VOID
PrintMemoryMap (
  IN MrcParameters *CONST  MrcData
  )
{
  MrcMemoryMap                        *MemoryMap;
  MrcInput                            *Inputs;
  SI_PREMEM_POLICY_PPI                *SiPreMemPolicyPpi;
  EFI_STATUS                           Status;
  IGPU_PEI_PREMEM_CONFIG              *IGpuPreMemConfig;
  HOST_BRIDGE_PREMEM_CONFIG           *HostBridgePreMemConfig;
  TELEMETRY_PEI_PREMEM_CONFIG         *TelemetryPreMemConfig;
  UINT64                               TouudBase;
  UINT64                               TotalPhysicalMemorySize;
  UINT32                               MediaRc6CtxBase;
  UINT32                               GcdRc6CtxBase;
  UINT32                               MchBar;
  UINT64                               GMSBase;
  UINT32                               SeamrrSize;
  UINT64                               FlatCcsBaseAddr;
  UINT64                               FlatCcsSize;
  IGPU_DATA_HOB                        *IGpuDataHob;


  TelemetryPreMemConfig     = NULL;

  MemoryMap                 = &MrcData->Outputs.MemoryMapData;
  Inputs                    = &MrcData->Inputs;
  TouudBase                 =  MemoryMap->TouudBase;
  TotalPhysicalMemorySize   =  MemoryMap->TotalPhysicalMemorySize;
  IGpuDataHob               = NULL;
  FlatCcsBaseAddr           = 0;
  FlatCcsSize               = 0;

  IGpuDataHob = (IGPU_DATA_HOB *) GetFirstGuidHob (&gIGpuDataHobGuid);
  if (IGpuDataHob != NULL) {
    FlatCcsBaseAddr = IGpuDataHob->FlatCcsBaseAddr;
  }

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

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  MchBar = (UINT32) GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);

  //GTRC6CTXBASE and MediaRC6CTXBASE
  GMSBase=  (UINT32) GetHostBridgeRegisterData (HostBridgeBdsm, HostBridgeBdsmBase);
  GcdRc6CtxBase = (UINT32)(GMSBase + IGpuGetDsmSizeInBytes () - SIZE_64KB);
  MediaRc6CtxBase = (UINT32)(GMSBase + IGpuGetDsmSizeInBytes () - (GCD_WOPCM_SIZE_2_MB * SIZE_1MB) - SIZE_64KB);
  Status = GetSeamrrSize (&SeamrrSize);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Cannot execute GetSeamrrSize() properly %r\n", Status));
  }

  ///
  /// Print Memory Map
  ///

  DEBUG ((DEBUG_INFO, "+----------------------------------------------------------------------------------------------------------------+\n"));
  DEBUG ((DEBUG_INFO, "|                                                   MEMORY MAP                                                   |\n"));
  DEBUG ((DEBUG_INFO, "+----------------------------------------------------------------------------------------------------------------+\n"));
  DEBUG ((DEBUG_INFO, "| USAGE                                  |  | START                         |  | LIMIT                           |\n"));
  DEBUG ((DEBUG_INFO, "| SBREG                                  |  | 0x%lx                  |  | 0x%lx                    |\n", PCH_PCR_BASE_ADDRESS, (PCH_PCR_BASE_ADDRESS + (PCH_PCR_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| SBREG2                                 |  | 0x%lx                  |  | 0x%lx                    |\n", PCH_PCR_SECOND_BASE_ADDRESS, (PCH_PCR_SECOND_BASE_ADDRESS + (PCH_PCR_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| SafBarBaseAddress                      |  | 0x%lx                  |  | 0x%lx                    |\n", PcdGet64 (PcdSafBarBaseAddress), (PcdGet64 (PcdSafBarBaseAddress) + (SIZE_32MB - 1))));
  DEBUG ((DEBUG_INFO, "| TOUUD (Top of Upper Usable DRAM)       |  | 0x%lx                   |  |                                 |\n", TouudBase << 20));
  DEBUG ((DEBUG_INFO, "| REMAP Base                             |  | 0x%lx                   |  | 0x%lx                     |\n", GetRemapBase (), GetRemapLimit ()));
  DEBUG ((DEBUG_INFO, "| TotalPhysicalMemorySize                |  | 0x%lx                   |  |                                 |\n", TotalPhysicalMemorySize << 20));
  if ((TelemetryPreMemConfig != NULL) && (TelemetryPreMemConfig->StreamTracerMode != StreamTracerDisabled)) {
    DEBUG ((DEBUG_INFO, "| StreamTracer                           |  | 0x%lx                   |  | 0x%lx                     |\n", LShiftU64 ((UINT64)MemoryMap->StreamTracerBase, 20), (LShiftU64 ((UINT64)MemoryMap->StreamTracerBase, 20) + (LShiftU64 ((UINT64)MemoryMap->StreamTracerSize, 20) - 1))));
  }
  if (IGpuIsSupported () && (IGpuPreMemConfig->MemoryBandwidthCompression == 1)) {
    FlatCcsSize = LShiftU64 ((UINT64)IGpuGetFlatCcsSizeInMb (), 20);
    DEBUG ((DEBUG_INFO, "| Flat CCS base                          |  | 0x%lx                   |  | 0x%lx                     |\n", FlatCcsBaseAddr, (FlatCcsBaseAddr + (FlatCcsSize - 1))));
  }
  if (SeamrrSize != 0) {
    DEBUG ((DEBUG_INFO, "| SEAMRR base                            |  | 0x%lx                   |  | 0x%lx                     |\n", MemoryMap->SeamrrBase, (MemoryMap->SeamrrBase  + ((SeamrrSize << 20) - 1))));
  }
  if (Inputs->PrmrrSize >= 512) {
    DEBUG ((DEBUG_INFO, "| PRMRR base                           |  | 0x%lx            |  | 0x%lx              |\n", MemoryMap->PrmrrBase, (MemoryMap->PrmrrBase  + (Inputs->PrmrrSize - 1))));
  }

  DEBUG ((DEBUG_INFO, "| NemCodeCacheBase                       |  | 0x%x                    |  |                                 |\n", FixedPcdGet64 (PcdNemCodeCacheBase)));
  DEBUG ((DEBUG_INFO, "| BiosAreaBaseAddress                    |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdBiosAreaBaseAddress), (FixedPcdGet64 (PcdBiosAreaBaseAddress) + (FixedPcdGet64 (PcdBiosSize) - 1))));
  DEBUG ((DEBUG_INFO, "| TemporaryRamBase                       |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdTemporaryRamBase), (FixedPcdGet64 (PcdTemporaryRamBase) + (FixedPcdGet64 (PcdTemporaryRamSize) - 1))));
  DEBUG ((DEBUG_INFO, "| ApicLocalAddress                       |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->ApicLocalAddress, (HostBridgePreMemConfig->ApicLocalAddress + (SIZE_4KB - 1))));
  DEBUG ((DEBUG_INFO, "| MchBaseAddress                         |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdMchBaseAddress), (FixedPcdGet64 (PcdMchBaseAddress) + (FixedPcdGet64 (PcdMchMmioSize) - 1))));
  DEBUG ((DEBUG_INFO, "| VtdMmioBase                            |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdVtdMmioBase), (FixedPcdGet64 (PcdVtdMmioBase) + (FixedPcdGet64 (PcdVtdMmioSize) - 1))));
  DEBUG ((DEBUG_INFO, "| TXT Config Space                       |  | 0x%x                    |  | 0x%x                      |\n", TXT_PRIVATE_BASE, (TXT_PRIVATE_BASE + (TXT_CONFIG_SPACE_LENGTH - 1))));
  DEBUG ((DEBUG_INFO, "| HpetBaseAdress                         |  | 0x%x                    |  |                                 |\n", FixedPcdGet64 (PcdSiHpetBaseAddress)));
  DEBUG ((DEBUG_INFO, "| IoApicAddress                          |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdSiIoApicBaseAddress), (FixedPcdGet64 (PcdSiIoApicBaseAddress) + (SIZE_4KB - 1))));
  DEBUG ((DEBUG_INFO, "| SiliconInitTempMemBaseAddr             |  | 0x%x                    |  | 0x%x                      |\n", FixedPcdGet64 (PcdSiliconInitTempMemBaseAddr), (FixedPcdGet64 (PcdSiliconInitTempMemBaseAddr) + (FixedPcdGet64 (PcdSiliconInitTempMemSize) - 1))));
  DEBUG ((DEBUG_INFO, "| TraceHub FW BAR                        |  | 0x%x                    |  | 0x%x                      |\n", PCH_TRACE_HUB_FW_BASE_ADDRESS, (PCH_TRACE_HUB_FW_BASE_ADDRESS + (PCH_TRACE_HUB_FW_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| TraceHub MTB BAR                       |  | 0x%x                    |  | 0x%x                      |\n", PCH_TRACE_HUB_MTB_BASE_ADDRESS, (PCH_TRACE_HUB_MTB_BASE_ADDRESS + (PCH_TRACE_HUB_MTB_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| eSPI2 SEGMR BAR                        |  | 0x%x                    |  | 0x%x                      |\n", PCH_ESPI_SEGMR_BASE_ADDRESS, (PCH_ESPI_SEGMR_BASE_ADDRESS + (PCH_ESPI_SEGMR_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| eSPI LGMR BAR                          |  | 0x%x                    |  | 0x%x                      |\n", PCH_ESPI_LGMR_BASE_ADDRESS, (PCH_ESPI_LGMR_BASE_ADDRESS + (PCH_ESPI_LGMR_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| LpssUartDebugMmioBase                  |  | 0x%x                    |  |                                 |\n", FixedPcdGet64 (PcdLpssUartDebugMmioBase)));
  DEBUG ((DEBUG_INFO, "| LpssUart2ndMmioBase                    |  | 0x%x                    |  |                                 |\n", FixedPcdGet64 (PcdLpssUart2ndMmioBase)));
  DEBUG ((DEBUG_INFO, "| SerialIo BAR in ACPI mode              |  | 0x%x                    |  | 0x%x                      |\n", PCH_SERIAL_IO_BASE_ADDRESS, (PCH_SERIAL_IO_BASE_ADDRESS + (PCH_SERIAL_IO_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| SPI BAR0                               |  | 0x%x                    |  | 0x%x                      |\n", PCH_SPI_BASE_ADDRESS, (PCH_SPI_BASE_ADDRESS + (PCH_SPI_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| PWRM BAR                               |  | 0x%x                    |  | 0x%x                      |\n", PCH_PWRM_BASE_ADDRESS, (PCH_PWRM_BASE_ADDRESS + (PCH_PWRM_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| TraceHub SW BAR                        |  | 0x%x                    |  | 0x%x                      |\n", PCH_TRACE_HUB_SW_BASE_ADDRESS, (PCH_TRACE_HUB_SW_BASE_ADDRESS + (PCH_TRACE_HUB_SW_MMIO_SIZE - 1))));
  DEBUG ((DEBUG_INFO, "| RegBarBaseAddress                      |  | 0x%lx                    |  | 0x%x                      |\n", PcdGet64 (PcdRegBarBaseAddress), (PcdGet64 (PcdRegBarBaseAddress) + (V_SA_MCHBAR_REGBAR_SIZE  - 1))));
  DEBUG ((DEBUG_INFO, "| SiPciExpressBaseAddress                |  | 0x%x                    |  |                                 |\n", PcdGet64 (PcdSiPciExpressBaseAddress)));
  DEBUG ((DEBUG_INFO, "| SiPciExpressLimitAddress               |  |                               |  | 0x%x                      |\n", (PcdGet64 (PcdSiPciExpressBaseAddress) + (PcdGet32 (PcdPciExpressRegionLength) - 1))));
  DEBUG ((DEBUG_INFO, "| LMemBarBase                            |  | 0x%x                    |  |                                 |\n", IGpuPreMemConfig->LMemBar));
  DEBUG ((DEBUG_INFO, "| GTBaseAddress                          |  | 0x%x                    |  | 0x%x                      |\n", IGpuPreMemConfig->GttMmAdr, (IGpuPreMemConfig->GttMmAdr + (SIZE_8MB - 1))));
  DEBUG ((DEBUG_INFO, "| VmdMemBar2Base                         |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->VmdMemBar2Bar, (HostBridgePreMemConfig->VmdMemBar2Bar + (SIZE_1MB - 1))));
  DEBUG ((DEBUG_INFO, "| VmdMemBar1Base                         |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->VmdMemBar1Bar, (HostBridgePreMemConfig->VmdMemBar1Bar + (SIZE_32MB - 1))));
  DEBUG ((DEBUG_INFO, "| VmdCfgBarBase                          |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->VmdCfgBarBar, (HostBridgePreMemConfig->VmdCfgBarBar + (SIZE_32MB - 1))));
  DEBUG ((DEBUG_INFO, "| NvmeHcPeiMmioBase                      |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->NvmeHcPeiMmioBase, HostBridgePreMemConfig->NvmeHcPeiMmioLimit));
  DEBUG ((DEBUG_INFO, "| AhciPeiMmioBase                        |  | 0x%x                    |  | 0x%x                      |\n", HostBridgePreMemConfig->AhciPeiMmioBase, HostBridgePreMemConfig->AhciPeiMmioLimit));
  DEBUG ((DEBUG_INFO, "| TOLUD (Top of Lower Usable DRAM)       |  | 0x%x                    |  |                           |\n", MemoryMap->ToludBase << 20));

  if (IGpuIsSupported () && IGpuIsMediaPresent ()) {
    DEBUG ((DEBUG_INFO, "| MediaRc6CtxBaseLow                     |  | 0x%x                    |  | 0x%x                      |\n", MediaRc6CtxBase, (MediaRc6CtxBase + (SIZE_64KB - 1))));
  }

  if (IGpuIsSupported () && IGpuIsGtPresent ()){
    DEBUG ((DEBUG_INFO, "| GcdRc6CtxBaseLow                       |  | 0x%x                    |  | 0x%x                      |\n", GcdRc6CtxBase, (GcdRc6CtxBase + (SIZE_64KB - 1))));
  }

  DEBUG ((DEBUG_INFO, "| BDSM base                              |  | 0x%08x                    |  |                                 |\n", MemoryMap->BdsmBase << 20));
  DEBUG ((DEBUG_INFO, "| GTT base                               |  | 0x%08x                    |  |                                 |\n", MemoryMap->GttBase << 20));
  DEBUG ((DEBUG_INFO, "| TSEG base                              |  | 0x%08x                    |  | 0x%08x                      |\n", MemoryMap->TsegBase << 20, ((MemoryMap->TsegBase << 20) + (Inputs->TsegSize << 20)) - 1));
  DEBUG ((DEBUG_INFO, "| MeStolen Base                          |  | 0x%08x                    |  | 0x%08x                      |\n", MemoryMap->MeStolenBase << 20, ((MemoryMap->MeStolenBase << 20) + (MemoryMap->MeStolenSize << 20)) - 1));
  DEBUG ((DEBUG_INFO, "| TSE IMR Base                           |  | 0x%08x                    |  | 0x%08x                      |\n", MemoryMap->TseImrBase << 20, ((MemoryMap->TseImrBase << 20) + (MemoryMap->TseImrSize << 20)) - 1));
  DEBUG ((DEBUG_INFO, "| SmbusBaseAddress                       |  | 0x%08x                    |  |                                 |\n", FixedPcdGet64 (PcdSmbusBaseAddress)));
  DEBUG ((DEBUG_INFO, "| PciReservedIobase                      |  | 0x%08x                    |  | 0x%08x                      |\n", FixedPcdGet64 (PcdPciReservedIobase), FixedPcdGet64 (PcdPciReservedIoLimit)));
  DEBUG ((DEBUG_INFO, "| AcpiBaseAddress                        |  | 0x%08x                    |  |                                 |\n", FixedPcdGet16 (PcdAcpiBaseAddress)));
  DEBUG ((DEBUG_INFO, "| EcExtraIoBase                          |  | 0x%08x                    |  |                                 |\n", HostBridgePreMemConfig->EcExtraIoBase));
  DEBUG ((DEBUG_INFO, "| SioBaseAddress                         |  | 0x%08x                    |  |                                 |\n", HostBridgePreMemConfig->SioBaseAddress));
  DEBUG ((DEBUG_INFO, "| TcoBaseAddress                         |  | 0x%08x                    |  |                                 |\n", FixedPcdGet16 (PcdTcoBaseAddress)));
  DEBUG ((DEBUG_INFO, "| ImrIaExcLow                            |  | 0x%x%x                   |  | 0x%x%x                     |\n", MmioRead32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG + 4),MmioRead32 (MchBar + R_SA_MCHBAR_IMRIAEXCBASE_CBO_INGRESS_REG),MmioRead32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG + 4), MmioRead32 (MchBar + R_SA_MCHBAR_IMRIAEXCLIMIT_CBO_INGRESS_REG)));
  DEBUG ((DEBUG_INFO, "+----------------------------------------------------------------------------------------------------------------+\n"));

}
