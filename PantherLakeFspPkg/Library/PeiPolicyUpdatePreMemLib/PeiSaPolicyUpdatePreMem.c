/** @file
  Do Platform Stage System Agent initialization.

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

#include <FspEas.h>
#include <Library/BaseMemoryLib.h>
#include <IpuPreMemConfig.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <FspmUpd.h>
#include <Library/PreSiliconEnvDetectLib.h>
#include <Library/VtdInfoLib.h>
#include <TcssPeiPreMemConfig.h>
#include <TelemetryPeiConfig.h>
#include <HostBridgeConfig.h>
#include <MemorySubSystemConfig.h>
#include <Ppi/SiPolicy.h>
#include <IGpuConfig.h>

/**
  UpdatePeiSaPolicy performs SA PEI Policy initialization in pre-memory

  @param[in, out] SiPreMemPolicyPpi       SI_PREMEM_POLICY_PPI
  @param[in]      FspmUpd                 The pointer of FspmUpd

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
FspUpdatePeiSaPolicyPreMem (
  IN OUT SI_PREMEM_POLICY_PPI       *SiPreMemPolicyPpi,
  IN     FSPM_UPD                   *FspmUpd
  )
{
  EFI_STATUS                    Status;
  MEMORY_SUB_SYSTEM_CONFIG      *MemorySubSystemConfig;
  MEMORY_CONFIG_NO_CRC          *MemConfigNoCrc;
  VTD_CONFIG                    *Vtd;
  MEMORY_CONFIGURATION          *MemConfig;
#if (FixedPcdGetBool(PcdHgEnable) == 1)
  HYBRID_GRAPHICS_CONFIG        *HgGpioData;
#endif
#if FixedPcdGetBool(PcdIpuEnable) == 1
  IPU_PREMEM_CONFIG             *IpuPreMemPolicy;
#endif
  UINT8                         Index;
  TCSS_PEI_PREMEM_CONFIG        *TcssPeiPreMemConfig;
  TELEMETRY_PEI_PREMEM_CONFIG   *TelemetryPreMemConfig;
  HOST_BRIDGE_PREMEM_CONFIG     *HostBridgePreMemConfig;
  IGPU_PEI_PREMEM_CONFIG        *IGpuPreMemConfig;

  DEBUG ((DEBUG_INFO | DEBUG_INIT, "FSP UpdatePeiSaPolicyPreMem\n"));

  HostBridgePreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  MemorySubSystemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  ASSERT_EFI_ERROR (Status);

  MemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR (Status);

  Vtd = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *)&Vtd);
  ASSERT_EFI_ERROR (Status);

  MemConfigNoCrc = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR (Status);

#if (FixedPcdGetBool(PcdHgEnable) == 1)
  HgGpioData = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHybridGraphicsConfigGuid, (VOID *) &HgGpioData);
  ASSERT_EFI_ERROR (Status);
#endif

#if FixedPcdGetBool(PcdIpuEnable) == 1
  IpuPreMemPolicy = NULL;
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *)&IpuPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
#endif


  TcssPeiPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  TelemetryPreMemConfig = NULL;
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  IGpuPreMemConfig     = NULL;
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  // Update ConfigBlock: MemConfigNoCrc
  //

  // Update SpdData only if SpdAddressTable is zero for each DIMM slot
  // MRC uses the following layout:
  // datOemSpd_0_0_0,      ///< use for get the device address for controller 0 channel 0 dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
  // datOemSpd_0_0_1,      ///< use for get the device address for controller 0 channel 0 dimm 1 - DDR4
  // datOemSpd_0_1_0,      ///< use for get the device address for controller 0 channel 1 dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
  // datOemSpd_0_1_1,      ///< use for get the device address for controller 0 channel 1 dimm 1 -------- DDR5 2DPC
  // datOemSpd_0_2_0,      ///< use for get the device address for controller 0 channel 2 dimm 0 --------------- LPDDR4 - LPDDR5
  // datOemSpd_0_3_0 = 6,  ///< use for get the device address for controller 0 channel 3 dimm 0 --------------- LPDDR4 - LPDDR5
  // datOemSpd_1_0_0 = 8,  ///< use for get the device address for controller 1 channel 0 dimm 0 - DDR4 - DDR5 - LPDDR4 - LPDDR5
  // datOemSpd_1_0_1,      ///< use for get the device address for controller 1 channel 0 dimm 1 - DDR4
  // datOemSpd_1_1_0,      ///< use for get the device address for controller 1 channel 1 dimm 0 -------- DDR5 - LPDDR4 - LPDDR5
  // datOemSpd_1_1_1,      ///< use for get the device address for controller 1 channel 1 dimm 1 -------- DDR5 2DPC
  // datOemSpd_1_2_0,      ///< use for get the device address for controller 1 channel 2 dimm 0 --------------- LPDDR4 - LPDDR5
  // datOemSpd_1_3_0 = 14, ///< use for get the device address for controller 1 channel 3 dimm 0 --------------- LPDDR4 - LPDDR5
  ZeroMem ((VOID *) MemConfigNoCrc->SpdData->SpdData, sizeof (SPD_DATA_BUFFER));

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr000 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[0] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr000, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr001 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[1] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][0][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr001, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr010 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[2] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr010, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr011 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[3] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][1][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr011, FspmUpd->FspmConfig.MemorySpdDataLen);
  }
  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr020 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[4] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr020, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr021 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[5] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][2][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr021, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr030 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[6] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr030, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr031 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[7] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][3][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr031, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr100 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[8] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr100, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr101 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[9] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][0][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr101, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr110 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[10] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr110, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr111 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[11] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][1][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr111, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr120 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[12] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr120, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr121 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[13] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][2][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr121, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr130 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[14] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr130, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  if (((VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr131 != NULL) && (FspmUpd->FspmConfig.SpdAddressTable[15] == 0)) {
    CopyMem ((VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][3][1], (VOID *)(UINTN) FspmUpd->FspmConfig.MemorySpdPtr131, FspmUpd->FspmConfig.MemorySpdDataLen);
  }

  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[0][0], &FspmUpd->FspmConfig.DqsMapCpu2DramMc0Ch0[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[0][1], &FspmUpd->FspmConfig.DqsMapCpu2DramMc0Ch1[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[0][2], &FspmUpd->FspmConfig.DqsMapCpu2DramMc0Ch2[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[0][3], &FspmUpd->FspmConfig.DqsMapCpu2DramMc0Ch3[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[1][0], &FspmUpd->FspmConfig.DqsMapCpu2DramMc1Ch0[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[1][1], &FspmUpd->FspmConfig.DqsMapCpu2DramMc1Ch1[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[1][2], &FspmUpd->FspmConfig.DqsMapCpu2DramMc1Ch2[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram[1][3], &FspmUpd->FspmConfig.DqsMapCpu2DramMc1Ch3[0], MEM_CFG_NUM_BYTES_MAPPED);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[0][0][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc0Ch0[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[0][1][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc0Ch1[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[0][2][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc0Ch2[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[0][3][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc0Ch3[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[1][0][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc1Ch0[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[1][1][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc1Ch1[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[1][2][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc1Ch2[0], MEM_CFG_NUM_BYTES_MAPPED * 8);
  CopyMem ((VOID *)(UINTN) MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram[1][3][0], &FspmUpd->FspmConfig.DqMapCpu2DramMc1Ch3[0], MEM_CFG_NUM_BYTES_MAPPED * 8);

  CopyMem ((VOID *) (UINTN) MemConfigNoCrc->CkdAddressTable,        &FspmUpd->FspmConfig.CkdAddressTable,        sizeof(MemConfigNoCrc->CkdAddressTable));
  CopyMem ((VOID *) (UINTN) MemConfigNoCrc->ChannelToCkdQckMapping, &FspmUpd->FspmConfig.ChannelToCkdQckMapping, sizeof(MemConfigNoCrc->ChannelToCkdQckMapping));
  CopyMem ((VOID *) (UINTN) MemConfigNoCrc->PhyClockToCkdDimm,      &FspmUpd->FspmConfig.PhyClockToCkdDimm,      sizeof(MemConfigNoCrc->PhyClockToCkdDimm));

  MemConfigNoCrc->RcompData->RcompResistor = FspmUpd->FspmConfig.RcompResistor;

  CopyMem ((VOID *) &(MemConfigNoCrc->RcompData->RcompTarget[0]), (VOID *) &FspmUpd->FspmConfig.RcompTarget[0], 10);

  MemConfigNoCrc->PlatformMemorySize = FspmUpd->FspmConfig.PlatformMemorySize;
  MemConfigNoCrc->SerialDebugLevel   = FspmUpd->FspmConfig.SerialDebugMrcLevel;
  MemConfigNoCrc->DisableMrcRetraining  = FspmUpd->FspmConfig.DisableMrcRetraining;
  MemConfigNoCrc->MrcBdatTestType  = FspmUpd->FspmConfig.MrcBdatTestType;
  MemConfigNoCrc->MrcBdatEnable  = FspmUpd->FspmConfig.MrcBdatEnable;
  ZeroMem((VOID *)MemConfigNoCrc->SpdAddressTable, sizeof(MemConfigNoCrc->SpdAddressTable));

    for (Index = 0; Index < MEM_CFG_MAX_SOCKETS; Index++) {
      if (FspmUpd->FspmConfig.SpdAddressTable[Index] != 0) {
        MemConfigNoCrc->SpdAddressTable[Index] = FspmUpd->FspmConfig.SpdAddressTable[Index];
      }
    }

  MemConfigNoCrc->PlatformMemorySize = FspmUpd->FspmConfig.PlatformMemorySize;
  MemConfigNoCrc->SerialDebugLevel   = FspmUpd->FspmConfig.SerialDebugMrcLevel;
  MemConfigNoCrc->CleanMemory        = FspmUpd->FspmConfig.CleanMemory;
  MemConfigNoCrc->MemTestOnWarmBoot  = FspmUpd->FspmConfig.MemTestOnWarmBoot;
  MemConfigNoCrc->SafeLoadingBiosEnableState  = FspmUpd->FspmConfig.SafeLoadingBiosEnableState;
  MemConfigNoCrc->PprRecoveryStatusEnable     = FspmUpd->FspmConfig.PprRecoveryStatusEnable;
  MemConfigNoCrc->TsegMemoryTestStatus        = FspmUpd->FspmConfig.TsegMemoryTestStatus;
  MemConfigNoCrc->MrcPprStatus                = FspmUpd->FspmConfig.MrcPprStatus;
  MemConfigNoCrc->RetryCount                  = FspmUpd->FspmConfig.RetryCount;
  MemConfigNoCrc->CAVrefCtlOffset             = FspmUpd->FspmConfig.CAVrefCtlOffset;
  MemConfigNoCrc->VrefCtlOffset               = FspmUpd->FspmConfig.VrefCtlOffset;
  MemConfigNoCrc->ClkPiCodeOffset             = FspmUpd->FspmConfig.ClkPiCodeOffset;
  MemConfigNoCrc->RcvEnOffset                 = FspmUpd->FspmConfig.RcvEnOffset;
  MemConfigNoCrc->RxDqsOffset                 = FspmUpd->FspmConfig.RxDqsOffset;
  MemConfigNoCrc->TxDqOffset                  = FspmUpd->FspmConfig.TxDqOffset;
  MemConfigNoCrc->TxDqsOffset                 = FspmUpd->FspmConfig.TxDqsOffset;
  MemConfigNoCrc->VrefOffset                  = FspmUpd->FspmConfig.VrefOffset;
  MemConfigNoCrc->CntrlrMask                  = FspmUpd->FspmConfig.CntrlrMask;
  MemConfigNoCrc->ChMask                      = FspmUpd->FspmConfig.ChMask;
  MemConfigNoCrc->RetrainToWorkingChannel     = FspmUpd->FspmConfig.RetrainToWorkingChannel;

  //
  // Update ConfigBlock: MemConfig
  //
  MemConfig->ExternalInputs.DqPinsInterleaved      = FspmUpd->FspmConfig.DqPinsInterleaved;
  MemorySubSystemConfig->MemMapOverlapCheckSupport = FspmUpd->FspmConfig.MemMapOverlapCheckSupport;
  MemConfig->ExternalInputs.MrcFastBoot            = FspmUpd->FspmConfig.MrcFastBoot;
  MemConfig->ExternalInputs.SaGv                   = FspmUpd->FspmConfig.SaGv;
  MemConfig->ExternalInputs.SaGvWpMask             = FspmUpd->FspmConfig.SaGvWpMask;
  for (Index = 0; Index < MAX_SAGV_POINTS; Index++) {
    MemConfig->ExternalInputs.SaGvFreq[Index]      = FspmUpd->FspmConfig.SaGvFreq[Index];
    MemConfig->ExternalInputs.SaGvGear[Index]      = FspmUpd->FspmConfig.SaGvGear[Index];
  }
  MemConfig->ExternalInputs.GearRatio              = FspmUpd->FspmConfig.GearRatio;
  MemConfig->ExternalInputs.DdrFreqLimit           = FspmUpd->FspmConfig.DdrFreqLimit;
  MemConfig->ExternalInputs.DisableChannel[0][0]   = FspmUpd->FspmConfig.DisableMc0Ch0;
  MemConfig->ExternalInputs.DisableChannel[0][1]   = FspmUpd->FspmConfig.DisableMc0Ch1;
  MemConfig->ExternalInputs.DisableChannel[0][2]   = FspmUpd->FspmConfig.DisableMc0Ch2;
  MemConfig->ExternalInputs.DisableChannel[0][3]   = FspmUpd->FspmConfig.DisableMc0Ch3;
  MemConfig->ExternalInputs.DisableChannel[1][0]   = FspmUpd->FspmConfig.DisableMc1Ch0;
  MemConfig->ExternalInputs.DisableChannel[1][1]   = FspmUpd->FspmConfig.DisableMc1Ch1;
  MemConfig->ExternalInputs.DisableChannel[1][2]   = FspmUpd->FspmConfig.DisableMc1Ch2;
  MemConfig->ExternalInputs.DisableChannel[1][3]   = FspmUpd->FspmConfig.DisableMc1Ch3;
  MemConfig->ExternalInputs.TrainTrace             = FspmUpd->FspmConfig.TrainTrace;
  MemConfig->ExternalInputs.RmtPerTask             = FspmUpd->FspmConfig.RmtPerTask;
  MemConfig->ExternalInputs.UserThresholdEnable    = FspmUpd->FspmConfig.UserThresholdEnable;
  MemConfig->ExternalInputs.ScramblerSupport       = FspmUpd->FspmConfig.ScramblerSupport;
  MemConfig->ExternalInputs.SafeModeOverride       = FspmUpd->FspmConfig.SafeModeOverride;
  MemConfig->ExternalInputs.DdrSafeMode            = FspmUpd->FspmConfig.DdrSafeMode;
  MemConfig->ExternalInputs.McSafeMode             = FspmUpd->FspmConfig.McSafeMode;
  MemConfig->ExternalInputs.LpMode                 = FspmUpd->FspmConfig.LpMode;
  MemConfig->ExternalInputs.LpMode4                = FspmUpd->FspmConfig.LpMode4;
  MemConfig->ExternalInputs.EnPeriodicComp         = FspmUpd->FspmConfig.EnPeriodicComp;
  MemConfig->ExternalInputs.SpineGatePerLpmode     = FspmUpd->FspmConfig.SpineGatePerLpmode;
  MemConfig->ExternalInputs.PhclkGatePerLpmode     = FspmUpd->FspmConfig.PhclkGatePerLpmode;
  MemConfig->ExternalInputs.DdrOneDpc              = FspmUpd->FspmConfig.DdrOneDpc;
  MemConfig->ExternalInputs.VddqVoltageOverride    = FspmUpd->FspmConfig.VddqVoltageOverride;
  MemConfig->ExternalInputs.VccIogVoltageOverride  = FspmUpd->FspmConfig.VccIogVoltageOverride;
  MemConfig->ExternalInputs.VccClkVoltageOverride  = FspmUpd->FspmConfig.VccClkVoltageOverride;
  MemConfig->ExternalInputs.DvfsqEnabled           = FspmUpd->FspmConfig.DvfsqEnabled;
  MemConfig->ExternalInputs.VoltageReadout         = FspmUpd->FspmConfig.VoltageReadout;
  MemConfig->ExternalInputs.MrcTimeMeasure         = FspmUpd->FspmConfig.MrcTimeMeasure;
  MemConfig->ExternalInputs.DvfscEnabled           = FspmUpd->FspmConfig.DvfscEnabled;
  MemConfig->ExternalInputs.DFETap1StepSize            = FspmUpd->FspmConfig.DFETap1StepSize;
  MemConfig->ExternalInputs.DFETap2StepSize            = FspmUpd->FspmConfig.DFETap2StepSize;

  for (Index = 0; Index < PPR_REQUEST_MAX; Index++) {
    MemConfigNoCrc->PprEntryInfo[Index].PprValid      = ((MRC_PPR_ENTRY_INFO *)(FspmUpd->FspmConfig.PprEntryInfo))[Index].PprValid;
    MemConfigNoCrc->PprEntryAddress[Index].Controller = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Controller;
    MemConfigNoCrc->PprEntryAddress[Index].Channel    = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Channel;
    MemConfigNoCrc->PprEntryAddress[Index].Rank       = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Rank;
    MemConfigNoCrc->PprEntryAddress[Index].BankGroup  = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].BankGroup;
    MemConfigNoCrc->PprEntryAddress[Index].Bank       = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Bank;
    MemConfigNoCrc->PprEntryAddress[Index].Row        = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Row;
    MemConfigNoCrc->PprEntryAddress[Index].Device     = ((MRC_PPR_ENTRY_ADDRESS *)(FspmUpd->FspmConfig.PprEntryAddress))[Index].Device;
  }

  MemConfigNoCrc->PprTestType.Bits.WcMats8       = FspmUpd->FspmConfig.PprRunWCHMATS8;
  MemConfigNoCrc->PprTestType.Bits.DataRetention = FspmUpd->FspmConfig.PprRunRetention;
  MemConfigNoCrc->PprTestType.Bits.XMarch        = FspmUpd->FspmConfig.PprRunXMarch;
  MemConfigNoCrc->PprTestType.Bits.XMarchG       = FspmUpd->FspmConfig.PprRunXMarchG;
  MemConfigNoCrc->PprTestType.Bits.YMarchShort   = FspmUpd->FspmConfig.PprRunYMarchShort;
  MemConfigNoCrc->PprTestType.Bits.YMarchLong    = FspmUpd->FspmConfig.PprRunYMarchLong;
  MemConfigNoCrc->PprTestType.Bits.Mmrw          = FspmUpd->FspmConfig.PprRunMmrw;
  MemConfigNoCrc->PprTestType.Bits.TestDisabled  = FspmUpd->FspmConfig.PprTestDisabled;

  MemConfigNoCrc->PprRepairType                  = FspmUpd->FspmConfig.PprRepairType;
  MemConfigNoCrc->PprRetryLimit                  = FspmUpd->FspmConfig.PprRetryLimit;
  MemConfigNoCrc->PprRunOnce                     = FspmUpd->FspmConfig.PprRunOnce;
  MemConfigNoCrc->PprErrorInjection              = FspmUpd->FspmConfig.PprErrorInjection;
  MemConfigNoCrc->PprForceRepair                 = FspmUpd->FspmConfig.PprForceRepair;

  MemConfig->ExternalInputs.BoardDetails.BoardStackUp   = FspmUpd->FspmConfig.BoardStackUp;
  MemConfig->ExternalInputs.BoardDetails.BoardTopology  = FspmUpd->FspmConfig.BoardTopology;
  MemConfig->ExternalInputs.BoardDetails.SingleVdd2Rail = FspmUpd->FspmConfig.SingleVdd2Rail;

  MemConfig->ExternalInputs.Vdd2HVoltage = FspmUpd->FspmConfig.Vdd2HVoltage;
  MemConfig->ExternalInputs.Vdd1Voltage  = FspmUpd->FspmConfig.Vdd1Voltage;
  MemConfig->ExternalInputs.Vdd2LVoltage = FspmUpd->FspmConfig.Vdd2LVoltage;
  MemConfig->ExternalInputs.VddqVoltage  = FspmUpd->FspmConfig.VddqVoltage;

  MemConfig->ExternalInputs.DprLock                = FspmUpd->FspmConfig.DprLock;
  MemConfig->ExternalInputs.SubChHashMask          = FspmUpd->FspmConfig.SubChHashMask;
  MemConfig->ExternalInputs.DqPinsInterleaved      = FspmUpd->FspmConfig.DqPinsInterleaved;
  MemConfig->ExternalInputs.MimicWcDisaplayInIpq   = FspmUpd->FspmConfig.MimicWcDisaplayInIpq;
  MemConfig->ExternalInputs.FakeSagv               = FspmUpd->FspmConfig.FakeSagv;
  MemConfig->ExternalInputs.DisableZq              = FspmUpd->FspmConfig.DisableZq;
  MemConfig->ExternalInputs.ReplicateSagv          = FspmUpd->FspmConfig.ReplicateSagv;
  MemConfig->ExternalInputs.AdjustWckMode          = FspmUpd->FspmConfig.AdjustWckMode;
  MemConfig->ExternalInputs.TelemetryControl       = FspmUpd->FspmConfig.TelemetryControl;
  MemConfig->ExternalInputs.SpineAndPhclkGateControl = FspmUpd->FspmConfig.SpineAndPhclkGateControl;
  MemConfig->ExternalInputs.SpineGatePerLpmode     = FspmUpd->FspmConfig.SpineGatePerLpmode;
  MemConfig->ExternalInputs.PhclkGatePerLpmode     = FspmUpd->FspmConfig.PhclkGatePerLpmode;
  MemConfig->ExternalInputs.DisableSwitchDfiToMc   = FspmUpd->FspmConfig.DisableSwitchDfiToMc;
  MemConfig->ExternalInputs.AsyncOdtDis            = FspmUpd->FspmConfig.AsyncOdtDis;
  MemConfig->ExternalInputs.WeaklockEn             = FspmUpd->FspmConfig.WeaklockEn;
  MemConfig->ExternalInputs.RxDqsDelayCompEn       = FspmUpd->FspmConfig.RxDqsDelayCompEn;
  MemConfig->ExternalInputs.ForceSingleRank        = FspmUpd->FspmConfig.ForceSingleRank;
  MemConfig->ExternalInputs.DynamicMemoryBoost     = FspmUpd->FspmConfig.DynamicMemoryBoost;
  MemConfig->ExternalInputs.PerBankRefresh         = FspmUpd->FspmConfig.PerBankRefresh;
  MemConfig->ExternalInputs.SrefCfgIdleTmr         = FspmUpd->FspmConfig.SrefCfgIdleTmr;
  MemConfig->ExternalInputs.IsDdr5MR7WicaSupported = FspmUpd->FspmConfig.IsDdr5MR7WicaSupported;
  MemConfig->ExternalInputs.CccPinsInterleaved     = FspmUpd->FspmConfig.CccPinsInterleaved;
  MemConfig->ExternalInputs.OpportunisticRead      = FspmUpd->FspmConfig.OpportunisticRead;
  MemConfig->ExternalInputs.Disable2CycleBypass    = FspmUpd->FspmConfig.Disable2CycleBypass;
  MemConfig->ExternalInputs.MCREGOFFSET            = FspmUpd->FspmConfig.MCREGOFFSET;
  MemConfig->ExternalInputs.Trrsg                  = FspmUpd->FspmConfig.tRRSG;
  MemConfig->ExternalInputs.Trrdg                  = FspmUpd->FspmConfig.tRRDG;
  MemConfig->ExternalInputs.Trrdr                  = FspmUpd->FspmConfig.tRRDR;
  MemConfig->ExternalInputs.Trrdd                  = FspmUpd->FspmConfig.tRRDD;
  MemConfig->ExternalInputs.Twrsg                  = FspmUpd->FspmConfig.tWRSG;
  MemConfig->ExternalInputs.Twrdg                  = FspmUpd->FspmConfig.tWRDG;
  MemConfig->ExternalInputs.Twrdr                  = FspmUpd->FspmConfig.tWRDR;
  MemConfig->ExternalInputs.Twrdd                  = FspmUpd->FspmConfig.tWRDD;
  MemConfig->ExternalInputs.Twwsg                  = FspmUpd->FspmConfig.tWWSG;
  MemConfig->ExternalInputs.Twwdg                  = FspmUpd->FspmConfig.tWWDG;
  MemConfig->ExternalInputs.Twwdr                  = FspmUpd->FspmConfig.tWWDR;
  MemConfig->ExternalInputs.Twwdd                  = FspmUpd->FspmConfig.tWWDD;
  MemConfig->ExternalInputs.Trwsg                  = FspmUpd->FspmConfig.tRWSG;
  MemConfig->ExternalInputs.Trwdg                  = FspmUpd->FspmConfig.tRWDG;
  MemConfig->ExternalInputs.Trwdr                  = FspmUpd->FspmConfig.tRWDR;
  MemConfig->ExternalInputs.Trwdd                  = FspmUpd->FspmConfig.tRWDD;
  MemConfig->ExternalInputs.Interpreter            = FspmUpd->FspmConfig.Interpreter;
  MemConfig->ExternalInputs.OCSafeMode             = FspmUpd->FspmConfig.OCSafeMode;
  MemConfig->ExternalInputs.IoOdtMode              = FspmUpd->FspmConfig.IoOdtMode;
  MemConfig->ExternalInputs.CccHalfFrequency       = FspmUpd->FspmConfig.CccHalfFrequency;
  MemConfig->ExternalInputs.Ddr5AutoPrechargeEnable = FspmUpd->FspmConfig.Ddr5AutoPrechargeEnable;
  MemConfig->ExternalInputs.Lp5SplitACTEnable = FspmUpd->FspmConfig.Lp5SplitACTEnable;
  MemConfig->ExternalInputs.SubChHashOverride = FspmUpd->FspmConfig.SubChHashOverride;
  MemConfig->ExternalInputs.RowPressEn             = FspmUpd->FspmConfig.RowPressEn;
  MemConfig->ExternalInputs.MrcFailureOnUnsupportedDimm = FspmUpd->FspmConfig.MrcFailureOnUnsupportedDimm;
  MemConfig->ExternalInputs.Use1p5ReadPostamble    = FspmUpd->FspmConfig.Use1p5ReadPostamble;
  MemConfig->ExternalInputs.Lp5SafeSpeed           = FspmUpd->FspmConfig.Lp5SafeSpeed;
  MemConfig->ExternalInputs.ForceInternalClkOn     = FspmUpd->FspmConfig.ForceInternalClkOn;
  //
  // Enabling Probeless Trace will need to enable IED together
  //
  MemConfig->ExternalInputs.ProbelessTrace = FspmUpd->FspmConfig.ProbelessTrace;
  MemConfig->ExternalInputs.MemoryProfile      = FspmUpd->FspmConfig.SpdProfileSelected;
  MemConfig->ExternalInputs.VddVoltage              = FspmUpd->FspmConfig.VddVoltage;
  MemConfig->ExternalInputs.Ratio                   = FspmUpd->FspmConfig.Ratio;
  MemConfig->ExternalInputs.tCL                     = FspmUpd->FspmConfig.tCL;
  MemConfig->ExternalInputs.tCWL                    = FspmUpd->FspmConfig.tCWL;
  MemConfig->ExternalInputs.tFAW                    = FspmUpd->FspmConfig.tFAW;
  MemConfig->ExternalInputs.tRAS                    = FspmUpd->FspmConfig.tRAS;
  MemConfig->ExternalInputs.tRCDtRP                 = FspmUpd->FspmConfig.tRCDtRP;
  MemConfig->ExternalInputs.tREFI                   = FspmUpd->FspmConfig.tREFI;
  MemConfig->ExternalInputs.tRFC                    = FspmUpd->FspmConfig.tRFC;
  MemConfig->ExternalInputs.tRRD                    = FspmUpd->FspmConfig.tRRD;
  MemConfig->ExternalInputs.tRTP                    = FspmUpd->FspmConfig.tRTP;
  MemConfig->ExternalInputs.tWR                     = FspmUpd->FspmConfig.tWR;
  MemConfig->ExternalInputs.tWTR                    = FspmUpd->FspmConfig.tWTR;
  MemConfig->ExternalInputs.NModeSupport            = FspmUpd->FspmConfig.NModeSupport;
  MemConfig->ExternalInputs.Vdd2Mv                  = FspmUpd->FspmConfig.Vdd2Mv;
  MemConfig->ExternalInputs.tRFC2                   = FspmUpd->FspmConfig.tRFC2;
  MemConfig->ExternalInputs.tRFC4                   = FspmUpd->FspmConfig.tRFC4;
  MemConfig->ExternalInputs.tRRD_L                  = FspmUpd->FspmConfig.tRRD_L;
  MemConfig->ExternalInputs.tRRD_S                  = FspmUpd->FspmConfig.tRRD_S;
  MemConfig->ExternalInputs.tWTR_L                  = FspmUpd->FspmConfig.tWTR_L;
  MemConfig->ExternalInputs.tCCD_L                  = FspmUpd->FspmConfig.tCCD_L;
  MemConfig->ExternalInputs.tCCD_L_WR               = FspmUpd->FspmConfig.tCCD_L_WR;
  MemConfig->ExternalInputs.tWTR_S                  = FspmUpd->FspmConfig.tWTR_S;
  MemConfig->ExternalInputs.tRFCpb                  = FspmUpd->FspmConfig.tRFCpb;
  MemConfig->ExternalInputs.RealtimeMemoryFrequency = FspmUpd->FspmConfig.RealtimeMemoryFrequency;
  MemConfig->ExternalInputs.CmdMirror               = FspmUpd->FspmConfig.CmdMirror;
  MemConfig->ExternalInputs.ForceCkdBypass          = FspmUpd->FspmConfig.ForceCkdBypass;
   if (IsSimicsEnvironment() || IsHfpgaEnvironment ()) {
     MemConfig->ExternalInputs.SimicsFlag = 1;
   } else {
     MemConfig->ExternalInputs.SimicsFlag = 0;
   }

  MemConfig->ExternalInputs.HobBufferSize                           = FspmUpd->FspmConfig.HobBufferSize;
  MemConfig->ExternalInputs.TrainingEnables.ECT                     = FspmUpd->FspmConfig.ECT;
  MemConfig->ExternalInputs.TrainingEnables.SOT                     = FspmUpd->FspmConfig.SOT;
  MemConfig->ExternalInputs.RMTLoopCount                            = FspmUpd->FspmConfig.RMTLoopCount;
  MemConfig->ExternalInputs.TrainingEnables.ERDMPRTC2D              = FspmUpd->FspmConfig.ERDMPRTC2D;
  MemConfig->ExternalInputs.tCCD_L_WR                               = FspmUpd->FspmConfig.tCCD_L_WR;
  MemConfig->ExternalInputs.TrainingEnables.RDMPRT                  = FspmUpd->FspmConfig.RDMPRT;
  MemConfig->ExternalInputs.TrainingEnables3.RXDQSDCC               = FspmUpd->FspmConfig.RXDQSDCC;
  MemConfig->ExternalInputs.TrainingEnables3.DIMMNTODT              = FspmUpd->FspmConfig.DIMMNTODT;
  MemConfig->ExternalInputs.SubChHashInterleaveBit                  = FspmUpd->FspmConfig.SubChHashInterleaveBit;
  MemConfig->ExternalInputs.DFETap1StepSize                         = FspmUpd->FspmConfig.DFETap1StepSize;
  MemConfig->ExternalInputs.RealtimeMemoryFrequency                 = FspmUpd->FspmConfig.RealtimeMemoryFrequency;
  MemConfig->ExternalInputs.CmdRanksTerminated                      = FspmUpd->FspmConfig.CmdRanksTerminated;
  MemConfig->ExternalInputs.CmdMirror                               = FspmUpd->FspmConfig.CmdMirror;
  MemConfig->ExternalInputs.TrainingEnables3.PHASECLKCAL            = FspmUpd->FspmConfig.PHASECLKCAL;
  MemConfig->ExternalInputs.TrainingEnables3.TXDQSDCC               = FspmUpd->FspmConfig.TXDQSDCC;
  MemConfig->ExternalInputs.TrainingEnables3.RXVREFPERBIT           = FspmUpd->FspmConfig.RXVREFPERBIT;
  MemConfig->ExternalInputs.TrainingEnables3.LVRAUTOTRIM            = FspmUpd->FspmConfig.LVRAUTOTRIM;
  MemConfig->ExternalInputs.TrainingEnables3.OPTIMIZECOMP           = FspmUpd->FspmConfig.OPTIMIZECOMP;
  MemConfig->ExternalInputs.TrainingEnables3.WRTRETRAIN             = FspmUpd->FspmConfig.WRTRETRAIN;
  MemConfig->ExternalInputs.TrainingEnables3.TLINECLKCAL            = FspmUpd->FspmConfig.TLINECLKCAL;
  MemConfig->ExternalInputs.TrainingEnables.RCVET                   = FspmUpd->FspmConfig.RCVET;
  MemConfig->ExternalInputs.TrainingEnables.JWRL                    = FspmUpd->FspmConfig.JWRL;
  MemConfig->ExternalInputs.TrainingEnables.EWRTC2D                 = FspmUpd->FspmConfig.EWRTC2D;
  MemConfig->ExternalInputs.TrainingEnables.ERDTC2D                 = FspmUpd->FspmConfig.ERDTC2D;
  MemConfig->ExternalInputs.TrainingEnables.WRTC1D                  = FspmUpd->FspmConfig.WRTC1D;
  MemConfig->ExternalInputs.TrainingEnables.WRVC1D                  = FspmUpd->FspmConfig.WRVC1D;
  MemConfig->ExternalInputs.TrainingEnables.RDTC1D                  = FspmUpd->FspmConfig.RDTC1D;
  MemConfig->ExternalInputs.TrainingEnables2.DIMMODTT               = FspmUpd->FspmConfig.DIMMODTT;
  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCCLK             = FspmUpd->FspmConfig.FUNCDCCCLK;
  MemConfig->ExternalInputs.TrainingEnables2.DIMMRONT               = FspmUpd->FspmConfig.DIMMRONT;
  MemConfig->ExternalInputs.TrainingEnables.RDEQT                   = FspmUpd->FspmConfig.RDEQT;
  MemConfig->ExternalInputs.TrainingEnables.WRTC2D                  = FspmUpd->FspmConfig.WRTC2D;
  MemConfig->ExternalInputs.TrainingEnables.UNMATCHEDWRTC1D         = FspmUpd->FspmConfig.UNMATCHEDWRTC1D;
  MemConfig->ExternalInputs.TrainingEnables.RDTC2D                  = FspmUpd->FspmConfig.RDTC2D;
  MemConfig->ExternalInputs.TrainingEnables.WRVC2D                  = FspmUpd->FspmConfig.WRVC2D;
  MemConfig->ExternalInputs.TrainingEnables.RDVC2D                  = FspmUpd->FspmConfig.RDVC2D;
  MemConfig->ExternalInputs.TrainingEnables.WRDSEQT                 = FspmUpd->FspmConfig.WRDSEQT;
  MemConfig->ExternalInputs.TrainingEnables.RDDQSODTT               = FspmUpd->FspmConfig.RDDQSODTT;
  MemConfig->ExternalInputs.TrainingEnables.DUNITC                  = FspmUpd->FspmConfig.DUNITC;
  MemConfig->ExternalInputs.TrainingEnables.DQSRF                   = FspmUpd->FspmConfig.DQSRF;
  MemConfig->ExternalInputs.TrainingEnables.CMDVC                   = FspmUpd->FspmConfig.CMDVC;
  MemConfig->ExternalInputs.TrainingEnables.LCT                     = FspmUpd->FspmConfig.LCT;
  MemConfig->ExternalInputs.TrainingEnables.RTL                     = FspmUpd->FspmConfig.RTL;
  MemConfig->ExternalInputs.TrainingEnables.TAT                     = FspmUpd->FspmConfig.TAT;
  MemConfig->ExternalInputs.TrainingEnables.RCVENC1D                = FspmUpd->FspmConfig.RCVENC1D;
  MemConfig->ExternalInputs.TrainingEnables.RMT                     = FspmUpd->FspmConfig.RMT;
  MemConfig->ExternalInputs.TrainingEnables.RMTEVENODD              = FspmUpd->FspmConfig.RMTEVENODD;
  MemConfig->ExternalInputs.TrainingEnables.ALIASCHK                = FspmUpd->FspmConfig.ALIASCHK;
  MemConfig->ExternalInputs.TrainingEnables.RMC                     = FspmUpd->FspmConfig.RMC;
  MemConfig->ExternalInputs.TrainingEnables.PRETRAIN                = FspmUpd->FspmConfig.PRETRAIN;
  MemConfig->ExternalInputs.EccSupport              = FspmUpd->FspmConfig.EccSupport;
  MemConfig->ExternalInputs.RemapEnable             = FspmUpd->FspmConfig.RemapEnable;
  MemConfig->ExternalInputs.MrcFastBoot             = FspmUpd->FspmConfig.MrcFastBoot;
  MemConfig->ExternalInputs.RankInterleave          = FspmUpd->FspmConfig.RankInterleave;
  MemConfig->ExternalInputs.EnhancedInterleave      = FspmUpd->FspmConfig.EnhancedInterleave;
  MemConfig->ExternalInputs.ChHashEnable            = FspmUpd->FspmConfig.ChHashEnable;
  MemConfig->ExternalInputs.EnablePwrDn             = FspmUpd->FspmConfig.EnablePwrDn;
  MemConfig->ExternalInputs.EnablePwrDnLpddr        = FspmUpd->FspmConfig.EnablePwrDnLpddr;
  MemConfig->ExternalInputs.SrefCfgEna              = FspmUpd->FspmConfig.SrefCfgEna;
  MemConfig->ExternalInputs.ThrtCkeMinDefeatLpddr   = FspmUpd->FspmConfig.ThrtCkeMinDefeatLpddr;
  MemConfig->ExternalInputs.ThrtCkeMinDefeat        = FspmUpd->FspmConfig.ThrtCkeMinDefeat;
  MemConfig->ExternalInputs.ExitOnFailure           = FspmUpd->FspmConfig.ExitOnFailure;
  MemConfig->ExternalInputs.ChHashInterleaveBit     = FspmUpd->FspmConfig.ChHashInterleaveBit;
  MemConfig->ExternalInputs.SmramMask               = FspmUpd->FspmConfig.SmramMask;
  MemConfig->ExternalInputs.ChHashMask              = FspmUpd->FspmConfig.ChHashMask;
  MemConfig->ExternalInputs.ExtendedBankHashing     = FspmUpd->FspmConfig.ExtendedBankHashing;
  MemConfig->ExternalInputs.MsHashOverride          = FspmUpd->FspmConfig.MsHashOverride;
  MemConfig->ExternalInputs.MsHashMask              = FspmUpd->FspmConfig.MsHashMask;
  MemConfig->ExternalInputs.MsHashInterleaveBit     = FspmUpd->FspmConfig.MsHashInterleaveBit;
  MemConfig->ExternalInputs.TrainingEnables2.DCCPICODELUT            = FspmUpd->FspmConfig.DCCPICODELUT;
  MemConfig->ExternalInputs.TrainingEnables.RDVC1D                   = FspmUpd->FspmConfig.RDVC1D;
  MemConfig->ExternalInputs.TrainingEnables2.TXTCO                   = FspmUpd->FspmConfig.TXTCO;
  MemConfig->ExternalInputs.TrainingEnables2.CLKTCO                  = FspmUpd->FspmConfig.CLKTCO;
  MemConfig->ExternalInputs.TrainingEnables2.CMDSR                   = FspmUpd->FspmConfig.CMDSR;
  MemConfig->ExternalInputs.TrainingEnables2.CMDDSEQ                 = FspmUpd->FspmConfig.CMDDSEQ;
  MemConfig->ExternalInputs.TrainingEnables2.DIMMODTCA               = FspmUpd->FspmConfig.DIMMODTCA;
  MemConfig->ExternalInputs.TrainingEnables2.WRTDIMMDFE              = FspmUpd->FspmConfig.WRTDIMMDFE;
  MemConfig->ExternalInputs.TrainingEnables2.DDR5ODTTIMING           = FspmUpd->FspmConfig.DDR5ODTTIMING;
  MemConfig->ExternalInputs.TrainingEnables2.WREQT                   = FspmUpd->FspmConfig.WREQT;
  MemConfig->ExternalInputs.TrainingEnables2.RDVREFDC                = FspmUpd->FspmConfig.RDVREFDC;
  MemConfig->ExternalInputs.TrainingEnables2.RDTCIDLE                = FspmUpd->FspmConfig.RDTCIDLE;
  MemConfig->ExternalInputs.TrainingEnables2.RMTBIT                  = FspmUpd->FspmConfig.RMTBIT;
  MemConfig->ExternalInputs.TrainingEnables2.REFPI                   = FspmUpd->FspmConfig.REFPI;
  MemConfig->ExternalInputs.TrainingEnables2.DATAPILIN               = FspmUpd->FspmConfig.DATAPILIN;
  MemConfig->ExternalInputs.TrainingEnables2.DDR5XTALK               = FspmUpd->FspmConfig.DDR5XTALK;
  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCWCK              = FspmUpd->FspmConfig.FUNCDCCWCK;
  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCCLK              = FspmUpd->FspmConfig.FUNCDCCCLK;
  MemConfig->ExternalInputs.TrainingEnables3.WCKPADDCCCAL            = FspmUpd->FspmConfig.WCKPADDCCCAL;
  MemConfig->ExternalInputs.TrainingEnables3.RDCTLET                 = FspmUpd->FspmConfig.RDCTLET;
  MemConfig->ExternalInputs.TrainingEnables3.RDDQODTT                = FspmUpd->FspmConfig.RDDQODTT;
  MemConfig->ExternalInputs.TrainingEnables3.EMPHASIS                = FspmUpd->FspmConfig.EMPHASIS;
  MemConfig->ExternalInputs.TrainingEnables3.DIMMRXOFFSET            = FspmUpd->FspmConfig.DIMMRXOFFSET;
  MemConfig->ExternalInputs.TrainingEnables3.RXDQSVOCC               = FspmUpd->FspmConfig.RXDQSVOCC;
  MemConfig->ExternalInputs.TrainingEnables3.DCCPISERIALCAL          = FspmUpd->FspmConfig.DCCPISERIALCAL;
  MemConfig->ExternalInputs.ThrtCkeMinTmr           = FspmUpd->FspmConfig.ThrtCkeMinTmr;
  MemConfig->ExternalInputs.McRefreshRate           = FspmUpd->FspmConfig.McRefreshRate;
  MemConfig->ExternalInputs.PowerDownMode           = FspmUpd->FspmConfig.PowerDownMode;
  MemConfig->ExternalInputs.PwdwnIdleCounter        = FspmUpd->FspmConfig.PwdwnIdleCounter;
  MemConfig->ExternalInputs.WriteThreshold          = FspmUpd->FspmConfig.WriteThreshold;
  MemConfig->ExternalInputs.MarginLimitL2           = FspmUpd->FspmConfig.MarginLimitL2;
  MemConfig->ExternalInputs.MarginLimitCheck        = FspmUpd->FspmConfig.MarginLimitCheck;
  MemConfig->ExternalInputs.AllowOppRefBelowWriteThrehold   = FspmUpd->FspmConfig.AllowOppRefBelowWriteThrehold;
  MemConfig->ExternalInputs.DisPgCloseIdleTimeout   = FspmUpd->FspmConfig.DisPgCloseIdleTimeout;
  MemConfig->ExternalInputs.SubChHashInterleaveBit  = FspmUpd->FspmConfig.SubChHashInterleaveBit;
  MemConfig->ExternalInputs.TrainingEnables2.DBI    = FspmUpd->FspmConfig.DBI;
  MemConfig->ExternalInputs.TrainingEnables2.DLLDCC = FspmUpd->FspmConfig.DLLDCC;
  MemConfig->ExternalInputs.TrainingEnables2.DQDQSSWZ = FspmUpd->FspmConfig.DQDQSSWZ;
  MemConfig->ExternalInputs.TrainingEnables2.DCCLP5READDCA = FspmUpd->FspmConfig.DCCLP5READDCA;
  MemConfig->ExternalInputs.TrainingEnables2.DCCDDR5READDCA = FspmUpd->FspmConfig.DCCDDR5READDCA;
  MemConfig->ExternalInputs.TrainingEnables2.DLLBWSEL = FspmUpd->FspmConfig.DLLBWSEL;
  MemConfig->ExternalInputs.TrainingEnables2.VCCCLKFF = FspmUpd->FspmConfig.VCCCLKFF;
  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCDQ = FspmUpd->FspmConfig.FUNCDCCDQ;
  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCDQS = FspmUpd->FspmConfig.FUNCDCCDQS;
  MemConfig->ExternalInputs.TrainingEnables2.RXUNMATCHEDCAL = FspmUpd->FspmConfig.RXUNMATCHEDCAL;
  MemConfig->ExternalInputs.TrainingEnables2.DCCLP5WCKDCA = FspmUpd->FspmConfig.DCCLP5WCKDCA;
  MemConfig->ExternalInputs.LowerBasicMemTestSize   = FspmUpd->FspmConfig.LowerBasicMemTestSize;

  MemConfig->ExternalInputs.LpddrRttWr       = FspmUpd->FspmConfig.LpddrRttWr;
  MemConfig->ExternalInputs.LpddrRttCa       = FspmUpd->FspmConfig.LpddrRttCa;

  MemConfig->ExternalInputs.DramRfmMode             = FspmUpd->FspmConfig.DramRfmMode;
  MemConfig->ExternalInputs.TargetedRowRefreshMode  = FspmUpd->FspmConfig.TargetedRowRefreshMode;
  MemConfig->ExternalInputs.DrfmBrc                 = FspmUpd->FspmConfig.DrfmBrc;
  MemConfig->ExternalInputs.Lfsr0Mask               = FspmUpd->FspmConfig.Lfsr0Mask;
  MemConfig->ExternalInputs.Lfsr1Mask               = FspmUpd->FspmConfig.Lfsr1Mask;
  MemConfig->ExternalInputs.RefreshWm               = FspmUpd->FspmConfig.RefreshWm;
  MemConfig->ExternalInputs.Lp5CccConfig            = FspmUpd->FspmConfig.Lp5CccConfig;
  MemConfig->ExternalInputs.Ibecc                   = FspmUpd->FspmConfig.Ibecc;
  MemConfig->ExternalInputs.IbeccParity             = FspmUpd->FspmConfig.IbeccParity;
  MemConfig->ExternalInputs.MsHashEnable            = FspmUpd->FspmConfig.MsHashEnable;
  MemConfig->ExternalInputs.UserBudgetEnable        = FspmUpd->FspmConfig.UserBudgetEnable;
  MemConfig->ExternalInputs.IbeccOperationMode      = FspmUpd->FspmConfig.IbeccOperationMode;
  MemConfig->ExternalInputs.ChHashOverride          = FspmUpd->FspmConfig.ChHashOverride;
  MemConfig->ExternalInputs.RetrainOnFastFail       = FspmUpd->FspmConfig.RetrainOnFastFail;

  MemConfig->ExternalInputs.EccGranularity32BEn     = FspmUpd->FspmConfig.EccGranularity32BEn;
  MemConfig->ExternalInputs.CaVrefLow               = FspmUpd->FspmConfig.CaVrefLow;
  for (Index = 0; Index < MAX_IBECC_REGIONS; Index++) {
    MemConfig->ExternalInputs.IbeccProtectedRegionEnable[Index] = FspmUpd->FspmConfig.IbeccProtectedRegionEnable[Index];
    MemConfig->ExternalInputs.IbeccProtectedRegionBase[Index]   = FspmUpd->FspmConfig.IbeccProtectedRegionBase[Index];
    MemConfig->ExternalInputs.IbeccProtectedRegionMask[Index]   = FspmUpd->FspmConfig.IbeccProtectedRegionMask[Index];
  }
  MemConfig->ExternalInputs.IbeccEccInjControl                  = FspmUpd->FspmConfig.IbeccEccInjControl;
  MemConfig->ExternalInputs.IbeccEccInjAddrBase                 = FspmUpd->FspmConfig.IbeccEccInjAddrBase;
  MemConfig->ExternalInputs.IbeccEccInjCount                    = FspmUpd->FspmConfig.IbeccEccInjCount;

  MemConfig->ExternalInputs.IsWckIdleExitEnabled    = FspmUpd->FspmConfig.IsWckIdleExitEnabled;
  MemConfig->ExternalInputs.FlexibleAnalogSettings  = FspmUpd->FspmConfig.FlexibleAnalogSettings;

  MemConfig->ExternalInputs.EccCorrectionMode       = FspmUpd->FspmConfig.EccCorrectionMode;
  MemConfig->ExternalInputs.CaVrefHigh              = FspmUpd->FspmConfig.CaVrefHigh;
  MemConfig->ExternalInputs.CsVrefLow               = FspmUpd->FspmConfig.CsVrefLow;
  MemConfig->ExternalInputs.CsVrefHigh              = FspmUpd->FspmConfig.CsVrefHigh;
  MemConfig->ExternalInputs.DFETap2StepSize         = FspmUpd->FspmConfig.DFETap2StepSize;

  if (Vtd != NULL) {
    Vtd->DmaControlGuarantee = FspmUpd->FspmConfig.DmaControlGuarantee;
    for (Index = 0; Index < GetMaxVtdEngineNumber(); Index++) {
      Vtd->BaseAddress[Index] = FspmUpd->FspmConfig.VtdBaseAddress[Index];
    }
    Vtd->VtdDisable          = FspmUpd->FspmConfig.VtdDisable;
    Vtd->PreBootDmaMask      = FspmUpd->FspmConfig.PreBootDmaMask;
    Vtd->DmaBufferSize       = FspmUpd->FspmConfig.DmaBufferSize;
    Vtd->CapabilityControl   = FspmUpd->FspmConfig.VtdCapabilityControl;
  }

  if (TelemetryPreMemConfig != NULL) {
    TelemetryPreMemConfig->CpuCrashLogDevice  = FspmUpd->FspmConfig.CpuCrashLogDevice;
    TelemetryPreMemConfig->StreamTracerBase   = FspmUpd->FspmConfig.StreamTracerBase;
    TelemetryPreMemConfig->StreamTracerSize   = FspmUpd->FspmConfig.StreamTracerSize;
    TelemetryPreMemConfig->StreamTracerMode   = FspmUpd->FspmConfig.StreamTracerMode;
  }

  //
  // Initialize the Graphics configuration
  //
  if (IGpuPreMemConfig != NULL) {
    IGpuPreMemConfig->IgdDvmt50PreAlloc                  = FspmUpd->FspmConfig.IgdDvmt50PreAlloc;
    IGpuPreMemConfig->InternalGraphics                   = FspmUpd->FspmConfig.InternalGraphics;
    IGpuPreMemConfig->PrimaryDisplay                     = FspmUpd->FspmConfig.PrimaryDisplay;
    IGpuPreMemConfig->GttMmAdr                           = FspmUpd->FspmConfig.GttMmAdr;
    IGpuPreMemConfig->LMemBar                            = FspmUpd->FspmConfig.LMemBar;
    IGpuPreMemConfig->DeltaT12PowerCycleDelay            = FspmUpd->FspmConfig.DeltaT12PowerCycleDelay;
    IGpuPreMemConfig->PanelPowerEnable                   = FspmUpd->FspmConfig.PanelPowerEnable;
    IGpuPreMemConfig->OemT12DelayOverride                = FspmUpd->FspmConfig.OemT12DelayOverride;
    IGpuPreMemConfig->MemoryBandwidthCompression         = FspmUpd->FspmConfig.MemoryBandwidthCompression;
    IGpuPreMemConfig->VbtPtr                             = (VOID *)(UINTN)FspmUpd->FspmConfig.VbtPtr;
    IGpuPreMemConfig->VgaInitControl                     = FspmUpd->FspmConfig.VgaInitControl;
    IGpuPreMemConfig->VgaMessage                         = (VOID *)(UINTN)FspmUpd->FspmConfig.VgaMessage;
    IGpuPreMemConfig->GraphicsMode12Info.ImagePtr        = (UINT8 *)(UINTN)FspmUpd->FspmConfig.VgaGraphicsMode12ImagePtr;
    IGpuPreMemConfig->GraphicsMode12Info.LogoPixelHeight = FspmUpd->FspmConfig.LogoPixelHeight;
    IGpuPreMemConfig->GraphicsMode12Info.LogoPixelWidth  = FspmUpd->FspmConfig.LogoPixelWidth;
    IGpuPreMemConfig->GraphicsMode12Info.LogoXPosition   = FspmUpd->FspmConfig.LogoXPosition;
    IGpuPreMemConfig->GraphicsMode12Info.LogoYPosition   = FspmUpd->FspmConfig.LogoYPosition;
    IGpuPreMemConfig->VbtSize                            = FspmUpd->FspmConfig.VbtSize;
    IGpuPreMemConfig->LidStatus                          = FspmUpd->FspmConfig.LidStatus;

    //
    // Display DDI Initialization
    //
    IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig = FspmUpd->FspmConfig.DdiPortAConfig;
    IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig = FspmUpd->FspmConfig.DdiPortBConfig;
    IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd    = FspmUpd->FspmConfig.DdiPortAHpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd    = FspmUpd->FspmConfig.DdiPortBHpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPortCHpd    = FspmUpd->FspmConfig.DdiPortCHpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd    = FspmUpd->FspmConfig.DdiPort1Hpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd    = FspmUpd->FspmConfig.DdiPort2Hpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd    = FspmUpd->FspmConfig.DdiPort3Hpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd    = FspmUpd->FspmConfig.DdiPort4Hpd;
    IGpuPreMemConfig->DdiConfiguration.DdiPortADdc    = FspmUpd->FspmConfig.DdiPortADdc;
    IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc    = FspmUpd->FspmConfig.DdiPortBDdc;
    IGpuPreMemConfig->DdiConfiguration.DdiPortCDdc    = FspmUpd->FspmConfig.DdiPortCDdc;
    IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc    = FspmUpd->FspmConfig.DdiPort1Ddc;
    IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc    = FspmUpd->FspmConfig.DdiPort2Ddc;
    IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc    = FspmUpd->FspmConfig.DdiPort3Ddc;
    IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc    = FspmUpd->FspmConfig.DdiPort4Ddc;
  }
#if (FixedPcdGetBool(PcdHgEnable) == 1)
  if (HgGpioData != NULL) {
    HgGpioData->HgSubSystemId  = FspmUpd->FspmConfig.HgSubSystemId;
    if (IGpuPreMemConfig != NULL) {
      if (IGpuPreMemConfig->PrimaryDisplay == 4) {
        IGpuPreMemConfig->PrimaryDisplay = 0;
      }
    }
  }
#endif

  if (HostBridgePreMemConfig != NULL) {
    HostBridgePreMemConfig->MmioSizeAdjustment   = FspmUpd->FspmConfig.MmioSizeAdjustment;
    HostBridgePreMemConfig->MchBar               = FspmUpd->FspmConfig.MchBar;
    HostBridgePreMemConfig->RegBar               = FspmUpd->FspmConfig.RegBar;
    HostBridgePreMemConfig->MmioSize             = FspmUpd->FspmConfig.MmioSize;
    HostBridgePreMemConfig->EnableAbove4GBMmio   = FspmUpd->FspmConfig.EnableAbove4GBMmio;

    HostBridgePreMemConfig->ApicLocalAddress     = FspmUpd->FspmConfig.ApicLocalAddress;
    HostBridgePreMemConfig->NvmeHcPeiMmioBase    = FspmUpd->FspmConfig.NvmeHcPeiMmioBase;
    HostBridgePreMemConfig->NvmeHcPeiMmioLimit   = FspmUpd->FspmConfig.NvmeHcPeiMmioLimit;
    HostBridgePreMemConfig->AhciPeiMmioBase      = FspmUpd->FspmConfig.AhciPeiMmioBase;
    HostBridgePreMemConfig->AhciPeiMmioLimit     = FspmUpd->FspmConfig.AhciPeiMmioLimit;
    HostBridgePreMemConfig->EcExtraIoBase        = FspmUpd->FspmConfig.EcExtraIoBase;
    HostBridgePreMemConfig->SioBaseAddress       = FspmUpd->FspmConfig.SioBaseAddress;
    HostBridgePreMemConfig->VmdCfgBarBar         = FspmUpd->FspmConfig.VmdCfgBarBar;
    HostBridgePreMemConfig->SafBar               = FspmUpd->FspmConfig.SafBar;
    HostBridgePreMemConfig->VmdMemBar1Bar        = FspmUpd->FspmConfig.VmdMemBar1Bar;
    HostBridgePreMemConfig->VmdMemBar2Bar        = FspmUpd->FspmConfig.VmdMemBar2Bar;
    HostBridgePreMemConfig->CridEnable           = FspmUpd->FspmConfig.CridEnable;
    HostBridgePreMemConfig->FabricGVDisable      = FspmUpd->FspmConfig.FabricGVDisable;
  }

#if FixedPcdGetBool(PcdIpuEnable) == 1
  if (IpuPreMemPolicy != NULL) {
    IpuPreMemPolicy->IpuEnable = FspmUpd->FspmConfig.SaIpuEnable;
    for (Index = 0; Index < GPIO_IMGUCLK_NUMBER_OF_PINS; Index ++) {
      IpuPreMemPolicy->ImguClkOutEn[Index] = FspmUpd->FspmConfig.ImguClkOutEn[Index];
    }
  }
#endif


  //
  // TCSS pre-mem configuration
  //
  if(TcssPeiPreMemConfig != NULL) {
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[0].ReceptacleSbu1BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[0];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[0].ReceptacleSbu2BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[1];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[0].AuxIsoCtrl             = FspmUpd->FspmConfig.IomTypeCPortPadCfg[2];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[1].ReceptacleSbu1BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[3];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[1].ReceptacleSbu2BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[4];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[1].AuxIsoCtrl             = FspmUpd->FspmConfig.IomTypeCPortPadCfg[5];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[2].ReceptacleSbu1BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[6];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[2].ReceptacleSbu2BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[7];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[2].AuxIsoCtrl             = FspmUpd->FspmConfig.IomTypeCPortPadCfg[8];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[3].ReceptacleSbu1BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[9];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[3].ReceptacleSbu2BiasCtrl = FspmUpd->FspmConfig.IomTypeCPortPadCfg[10];
    TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[3].AuxIsoCtrl             = FspmUpd->FspmConfig.IomTypeCPortPadCfg[11];

    TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri                     = FspmUpd->FspmConfig.TcssAuxOri;
    TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri                     = FspmUpd->FspmConfig.TcssHslOri;
    TcssPeiPreMemConfig->TcssEnable                                        = FspmUpd->FspmConfig.TcssXhciEn;
    TcssPeiPreMemConfig->TcssPlatConf                                      = FspmUpd->FspmConfig.TcssPlatConf;
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[0]       = FspmUpd->FspmConfig.TcssPort0;
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[1]       = FspmUpd->FspmConfig.TcssPort1;
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[2]       = FspmUpd->FspmConfig.TcssPort2;
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[3]       = FspmUpd->FspmConfig.TcssPort3;

    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[Index] = FspmUpd->FspmConfig.IomUsbCDpConfig[Index];
    }
  }

  return EFI_SUCCESS;
}
