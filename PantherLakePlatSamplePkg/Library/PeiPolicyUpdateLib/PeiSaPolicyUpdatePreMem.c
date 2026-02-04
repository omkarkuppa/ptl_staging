/** @file
  Do Platform Stage System Agent initialization.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#include "PeiSaPolicyUpdate.h"
#include "MemoryConfig.h"
#include <Guid/AcpiVariable.h>
#include <Guid/MemoryOverwriteControl.h>
#include <Guid/MemoryTypeInformation.h>
#include <Guid/S3MemoryVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/HobLib.h>
#include <Library/MeFwStsLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiSiPolicyUpdateLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/VtdInfoLib.h>
#include <Register/GenerationMsr.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <CpuRegs.h>
#include <HostBridgeConfig.h>
#include <IpuPreMemConfig.h>
#include <Platform.h>
#include <PlatformBoardConfig.h>
#include <PlatformBoardId.h>
#include <PolicyUpdateMacro.h>
#include <Setup.h>
#include <TelemetryPeiConfig.h>
#include <MemorySubSystemConfig.h>
#include <Ppi/FspmArchConfigPpi.h>
#include <Library/UefiDecompressLib.h>
#include <TcssPeiPreMemConfig.h>
#include <BoardSaConfigPreMem.h>
#include <IGpuConfig.h>
#include <Library/PcdLib.h>
#include <Library/EcMiscLib.h>
#include <MemInfoHob.h>
#if FixedPcdGet8(PcdFspModeSelection) == 1
#include <FspmUpd.h>
#endif
#include <Library/PeiLib.h>

#if FixedPcdGetBool (PcdMemoryTelemetryEnabled) == 1
#include <Library/PeiMemoryTelemetryLib.h>
#endif

#if FixedPcdGetBool (PcdTcssSupport) == 1
#include <TcssPeiPreMemConfig.h>
#include <Library/PlatformUsbConfigLib.h>
#endif
#include <TishDataHob.h>
///
///
///
/// Memory Reserved should be between 125% to 150% of the Current required memory
/// otherwise BdsMisc.c would do a reset to make it 125% to avoid s4 resume issues.
///
GLOBAL_REMOVE_IF_UNREFERENCED EFI_MEMORY_TYPE_INFORMATION mDefaultMemoryTypeInformation[] = {
  { EfiACPIReclaimMemory,   FixedPcdGet32 (PcdPlatformEfiAcpiReclaimMemorySize) },  // ASL
  { EfiACPIMemoryNVS,       FixedPcdGet32 (PcdPlatformEfiAcpiNvsMemorySize) },      // ACPI NVS (including S3 related)
  { EfiReservedMemoryType,  FixedPcdGet32 (PcdPlatformEfiReservedMemorySize) },     // BIOS Reserved (including S3 related)
  { EfiRuntimeServicesData, FixedPcdGet32 (PcdPlatformEfiRtDataMemorySize) },       // Runtime Service Data
  { EfiRuntimeServicesCode, FixedPcdGet32 (PcdPlatformEfiRtCodeMemorySize) },       // Runtime Service Code
  { EfiMaxMemoryType, 0 }
};

#define GET_COMPRESS_DATA_SIZE(x) (x[4]) + (x[5] << 8) + (x[6] << 16) + (x[7] << 24)

//
// if OEM wants to display multiple lines then we can use \n
// Ex: "MEMORY TRAINING\nIN PROGRESS"
//
GLOBAL_REMOVE_IF_UNREFERENCED const CHAR8 VgaMessage[] = "MEMORY TRAINING IN PROGRESS";

/**
  Return TRUE when the Operand is exactly power of 2.
  @retval TRUE  Operand is exactly power of 2.
  @retval FALSE Operand is not power of 2.
**/
BOOLEAN
IsPowerOfTwo (
  UINT32 Operand
  )
{
  return (Operand != 0) && ((Operand & (Operand - 1)) == 0);
}

/**
  Decompresses a compressed memory nvs buffer.
  This routine will use UefiDecompress routine to decompress
  the compressed memory nvs data
  @param[in]       CompressedMemNvsData  The source buffer containing the compressed memory nvs data.
  @param[in][out]  NvsDataSize           Size of compressed memory nvs data and this routine would
                                         update the variable to size of decompressed data.
  @param[out]      DecompressedData      The destination buffer to store the decompressed data.
  @retval          RETURN_SUCCESS        Decompression completed successfully, and
                                         the uncompressed buffer is returned in Destination.
  @retval          RETURN_INVALID_PARAMETER
                                         The source buffer specified by Source is corrupted
                                         (not in a valid compressed format) or invalid
                                         parameters are passed into the function.
**/
EFI_STATUS
EFIAPI
DecompressMemoryNvsData (
  IN      VOID      *CompressedMemNvsData,
  IN OUT  UINT32    *NvsDataSize,
  OUT     VOID      **DecompressedData
  )
{
  UINT8              ScratchData[0x4000];
  EFI_STATUS         Status;

  *DecompressedData = NULL;

  if ((NvsDataSize == NULL) || (CompressedMemNvsData == NULL)) {
    DEBUG ((DEBUG_ERROR, "DecompressMemoryNvsData: return %r\n", EFI_INVALID_PARAMETER));
    return EFI_INVALID_PARAMETER;
  }

  *NvsDataSize = GET_COMPRESS_DATA_SIZE (((UINT8 *) CompressedMemNvsData));
  DEBUG ((DEBUG_INFO, "Size is :0x%x\n",*NvsDataSize));

  if (*NvsDataSize == 0) {
    return EFI_INVALID_PARAMETER;
  }

  *DecompressedData = AllocatePool(*NvsDataSize);
  if (*DecompressedData == NULL) {
    DEBUG ((DEBUG_ERROR, "DecompressMemoryNvsData: Could not allocate memory\n"));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (ScratchData, sizeof(UINT8) * 0x4000);
  Status = UefiDecompress (CompressedMemNvsData, *DecompressedData, &ScratchData);
  DEBUG ((DEBUG_INFO, "Decompress return %r\n", Status));

  return Status;
}

/**
  UpdatePeiSaPolicyPreMem performs SA PEI Policy initialization

  @retval EFI_SUCCESS              The policy is installed and initialized.
**/
EFI_STATUS
EFIAPI
UpdatePeiSaPolicyPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                                      Status;
  EFI_STATUS                                      Status2;
  EFI_STATUS                                      Status3;
  EFI_STATUS                                      Status4;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI                 *VariableServices;
  SI_SETUP                                        SiSetup;
  SETUP_DATA                                      SetupData;
  SA_SETUP                                        SaSetup;
  CPU_SETUP                                       CpuSetup;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OC_SETUP                                        OcSetup;
#endif
  UINTN                                           VariableSize;
  WDT_PPI                                         *gWdtPei;
  UINT8                                           WdtTimeout;
  UINT8                                           Index;
  S3_MEMORY_VARIABLE                              S3MemVariable;
  UINTN                                           DataSize;
  EFI_MEMORY_TYPE_INFORMATION                     MemoryData[EfiMaxMemoryType + 1];
  EFI_BOOT_MODE                                   BootMode;
  UINT8                                           MorControl;
  PCH_SETUP                                       PchSetup;
  UINT16                                          MmioSize;
  UINT64                                          PlatformMemorySize;
  VOID                                            *MemorySavedData;
  VOID                                            *NullSpdPtr;
  VOID                                            *DecompressedNvsData;
  TISH_CONFIG_HOB                                  *TishDataHob;
  BOOLEAN                                         ExternalSpdPresent;
  UINT16                                          *RcompTarget;
  MRC_PPR_TEST_TYPE                               PprTestType;
  RMT_VAR                                         mRmtdata;

#if FixedPcdGetBool (PcdTcssSupport) == 1
  UINT8                                           *TcssPortCap;
  UINT32                                          TcssPortCapMap;
  UINT8                                           TcssPortConf[MAX_TCSS_USB3_PORTS];
  UINT8                                           TcssPlatConfig;
  USB_CONNECTOR_HOB_DATA                          *UsbConnectorHobDataPtr;
  USBC_CONNECTOR_HOB_DATA                         *UsbCConnectorHobDataPtr;
  USB_CONNECTOR_BOARD_CONFIG                      *UsbConnectorBoardConfig;
  USBC_CONNECTOR_BOARD_CONFIG                     *UsbCConnectorBoardConfig;
#endif
  UINT32                                          StreamTracerSize;
#if FixedPcdGet8(PcdFspModeSelection) == 0
  EFI_PEI_PPI_DESCRIPTOR                          *FspmArchConfigPpiDesc;
  FSPM_ARCH_CONFIG_PPI                            *FspmArchConfigPpi;
#endif
  UINT16                                          AdjustedMmioSize;
  UINT8                                           SaDisplayConfigTable[16];
  UINT32                                          ProcessorTraceTotalMemSize;
  MSR_CORE_THREAD_COUNT_REGISTER                  MsrCoreThreadCount;
  CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS_EBX     Ebx;
  UINT32                                          CapsuleSupportMemSize;
  VOID                                            *Hob;
  VOID                                            *DisplayDdiConfig;
  UINT16                                          DisplayDdiConfigSize;
  MEMORY_CONFIG_NO_CRC                            *MemConfigNoCrc;
#if FixedPcdGetBool(PcdHgEnable) == 1
  UINT16                                          HgSubSystemId;
#endif
  GOP_CONFIG_DRIVER_HOB                           *GopConfigDriverHob;
  EFI_HOB_GUID_TYPE                               *GuidHob;
  BOOLEAN                                         LidStatusOpen;

#if FixedPcdGet8(PcdFspModeSelection) == 1
  VOID                                            *FspmUpd;
#else
  IGPU_PEI_PREMEM_CONFIG                          *IGpuPreMemConfig;
  MEMORY_CONFIGURATION                            *MemConfig;
  HOST_BRIDGE_PREMEM_CONFIG                       *HostBridgePreMemConfig;
  MEMORY_RCOMP                                    *RcompData;
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OVERCLOCKING_PREMEM_CONFIG                      *OcPreMemConfig;
#endif
  VTD_CONFIG                                      *Vtd;
  TELEMETRY_PEI_PREMEM_CONFIG                     *TelemetryPreMemConfig;
#if FixedPcdGetBool (PcdTcssSupport) == 1
  TCSS_PEI_PREMEM_CONFIG                          *TcssPeiPreMemConfig;
#endif
  MEMORY_SUB_SYSTEM_CONFIG                        *MemorySubSystemConfig;
#if FixedPcdGetBool(PcdHgEnable) == 1
  HYBRID_GRAPHICS_CONFIG                          *HgGpioData;
#endif
#if FixedPcdGetBool(PcdIpuEnable) == 1
  IPU_PREMEM_CONFIG                               *IpuPreMemPolicy;
#endif
#endif //FSpMode Check

  UINTN                                           Size;
  VOID                                            *Buffer;
  UINT8                                           VgaInitControl;
  UINT8                                           VtdCpabilityControl;

  DEBUG ((DEBUG_INFO, "Update %a Start\n", __FUNCTION__));
  ZeroMem ((VOID*) SaDisplayConfigTable, sizeof (SaDisplayConfigTable));
  WdtTimeout           = 0;
  BootMode             = 0;
  PlatformMemorySize   = 0;
  Buffer               = NULL;
  VgaInitControl       = 0;
  VtdCpabilityControl  = 0;
  MemConfigNoCrc       = NULL;
  StreamTracerSize     = 0;

#if FixedPcdGetBool (PcdTcssSupport) == 1
  TcssPortCapMap       = PcdGet32 (PcdTcssPortCapMap);
  TcssPortCap          = (UINT8 *) &TcssPortCapMap;
  TcssPlatConfig       = 0;
#endif
#if FixedPcdGet8(PcdFspModeSelection) == 0
  FspmArchConfigPpi    = NULL;
  RcompData            = NULL;
  IGpuPreMemConfig     = NULL;
  MemConfig            = NULL;
#if FixedPcdGetBool(PcdIpuEnable) == 1
  IpuPreMemPolicy      = NULL;
#endif
#endif

  ProcessorTraceTotalMemSize = 0;
  CapsuleSupportMemSize      = 0;
#if FixedPcdGetBool(PcdHgEnable) == 1
  HgSubSystemId        = 0;
#endif

  AdjustedMmioSize = PcdGet16 (PcdSaMiscMmioSizeAdjustment);

#if FixedPcdGet8(PcdFspModeSelection) == 1
  FspmUpd = (FSPM_UPD *)(UINTN) PcdGet64 (PcdFspmUpdDataAddress64);
  ASSERT (FspmUpd != NULL);
#else
#if FixedPcdGetBool(PcdOverclockEnable) == 1
  OcPreMemConfig       = NULL;
#endif
#if FixedPcdGetBool (PcdTcssSupport) == 1
  TcssPeiPreMemConfig  = NULL;
#endif
  TelemetryPreMemConfig = NULL;
  MemorySubSystemConfig = NULL;
  IGpuPreMemConfig     = NULL;
  MemConfig            = NULL;
  HostBridgePreMemConfig = NULL;
  RcompData            = NULL;

#if FixedPcdGetBool(PcdHgEnable) == 1
  HgGpioData           = NULL;
#endif

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gHostBridgePeiPreMemConfigGuid, (VOID *) &HostBridgePreMemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gMemorySubSystemConfigGuid, (VOID *) &MemorySubSystemConfig);
  ASSERT_EFI_ERROR (Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gGraphicsPeiPreMemConfigGuid, (VOID *) &IGpuPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gMemoryConfigGuid, (VOID *) &MemConfig);
  ASSERT_EFI_ERROR(Status);

#if FixedPcdGetBool(PcdHgEnable) == 1
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gHybridGraphicsConfigGuid, (VOID *) &HgGpioData);
  ASSERT_EFI_ERROR(Status);
#endif

#if FixedPcdGetBool(PcdIpuEnable) == 1
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gIpuPreMemConfigGuid, (VOID *) &IpuPreMemPolicy);
  ASSERT_EFI_ERROR (Status);
#endif
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gMemoryConfigNoCrcGuid, (VOID *) &MemConfigNoCrc);
  ASSERT_EFI_ERROR(Status);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gOverclockingPreMemConfigGuid, (VOID *) &OcPreMemConfig);
  ASSERT_EFI_ERROR(Status);
#endif

#if FixedPcdGetBool (PcdTcssSupport) == 1
  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR(Status);
#endif

  Status = GetConfigBlock ((VOID *)SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  ASSERT_EFI_ERROR(Status);

  Status = GetConfigBlock((VOID *) SiPreMemPolicyPpi, &gVtdConfigGuid, (VOID *)&Vtd);
  ASSERT_EFI_ERROR(Status);

  RcompData = MemConfigNoCrc->RcompData;
#endif // Fspcheck
  //
  // Locate system configuration variable
  //
  Status = PeiServicesLocatePpi(
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **) &VariableServices       // PPI
             );
  ASSERT_EFI_ERROR(Status);

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  //
  // Initialize S3 Data variable (S3DataPtr)
  //
  VariableSize = 0;
  MemorySavedData = NULL;
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"FspNvsBuffer",
                               &gFspNvsBufferVariableGuid,
                               NULL,
                               &VariableSize,
                               MemorySavedData
                               );
  if (Status == EFI_BUFFER_TOO_SMALL) {
    MemorySavedData = AllocateZeroPool (VariableSize);
    ASSERT (MemorySavedData != NULL);

    Status = VariableServices->GetVariable (
                                 VariableServices,
                                 L"FspNvsBuffer",
                                 &gFspNvsBufferVariableGuid,
                                 NULL,
                                 &VariableSize,
                                 MemorySavedData
                                 );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Fail to retrieve Variable: MemoryConfig, Status = %r\n", Status));
      ASSERT_EFI_ERROR (Status);
    }
  }

  if (PcdGetBool (PcdEnableCompressedFspNvsBuffer) == 1) {
    DecompressedNvsData  = NULL;
    if (VariableSize != 0) {
      DEBUG ((DEBUG_INFO, "Memory NVS Saved Data size:%d\n", VariableSize));
      Status = DecompressMemoryNvsData(MemorySavedData, (UINT32 *)&VariableSize, &DecompressedNvsData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "Fail to decompress memory nvs data, Status = %r\n", Status));
        ASSERT_EFI_ERROR (Status);
      }
      FreePool(MemorySavedData);
      MemorySavedData = DecompressedNvsData;
    }
  }

#if FixedPcdGet8(PcdFspModeSelection) == 0
  FspmArchConfigPpi = (FSPM_ARCH_CONFIG_PPI *) AllocateZeroPool (sizeof (FSPM_ARCH_CONFIG_PPI));
  if (FspmArchConfigPpi == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpi->Revision     = 1;
  FspmArchConfigPpi->NvsBufferPtr = MemorySavedData;

  FspmArchConfigPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (FspmArchConfigPpiDesc == NULL) {
    ASSERT (FALSE);
    FreePool (FspmArchConfigPpi);
    return EFI_OUT_OF_RESOURCES;
  }
  FspmArchConfigPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  FspmArchConfigPpiDesc->Guid  = &gFspmArchConfigPpiGuid;
  FspmArchConfigPpiDesc->Ppi   = FspmArchConfigPpi;

  //
  // Install FSP-M Arch Config PPI
  //
  Status = PeiServicesInstallPpi (FspmArchConfigPpiDesc);
  ASSERT_EFI_ERROR (Status);
#else
  ((FSPM_UPD *) FspmUpd)->FspmArchUpd.NvsBufferPtr = (EFI_PHYSICAL_ADDRESS)(UINTN) MemorySavedData;
#endif

  //
  // Get S3 Memory Variable
  //
  VariableSize = sizeof (S3_MEMORY_VARIABLE);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               S3_MEMORY_VARIABLE_NAME,
                               &gS3MemoryVariableGuid,
                               NULL,
                               &VariableSize,
                               &S3MemVariable
                               );
  if (Status == EFI_SUCCESS) {
#if FixedPcdGet8(PcdFspModeSelection) == 0
    MemorySubSystemConfig->AcpiReservedMemoryBase = S3MemVariable.AcpiReservedMemoryBase;
    MemorySubSystemConfig->AcpiReservedMemorySize = S3MemVariable.AcpiReservedMemorySize;
#endif
  }

  VariableSize = sizeof (MorControl);
  Status = VariableServices->GetVariable(
                               VariableServices,
                               MEMORY_OVERWRITE_REQUEST_VARIABLE_NAME,
                               &gEfiMemoryOverwriteControlDataGuid,
                               NULL,
                               &VariableSize,
                               &MorControl
                               );
  if (EFI_ERROR (Status)) {
    MorControl = 0;
  }

  //
  // Get System configuration variables
  //
  VariableSize = sizeof (SI_SETUP);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"SiSetup",
                               &gSiSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SiSetup
                               );

  VariableSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               PLATFORM_SETUP_VARIABLE_NAME,
                               &gSetupVariableGuid,
                               NULL,
                               &VariableSize,
                               &SetupData
                               );
  ASSERT_EFI_ERROR(Status);

  VariableSize = sizeof (SA_SETUP);
  Status2 = VariableServices->GetVariable (
                                VariableServices,
                                L"SaSetup",
                                &gSaSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &SaSetup
                                );
  ASSERT_EFI_ERROR(Status2);

  VariableSize = sizeof (CPU_SETUP);
  Status3 = VariableServices->GetVariable (
                                VariableServices,
                                L"CpuSetup",
                                &gCpuSetupVariableGuid,
                                NULL,
                                &VariableSize,
                                &CpuSetup
                                );
  ASSERT_EFI_ERROR(Status3);

#if FixedPcdGetBool(PcdOverclockEnable) == 1
  VariableSize = sizeof (OC_SETUP);
  VariableServices->GetVariable (
                      VariableServices,
                      L"OcSetup",
                      &gOcSetupVariableGuid,
                      NULL,
                      &VariableSize,
                      &OcSetup
                      );
#endif

  VariableSize = sizeof (PCH_SETUP);
  VariableServices->GetVariable (
                      VariableServices,
                      L"PchSetup",
                      &gPchSetupVariableGuid,
                      NULL,
                      &VariableSize,
                      &PchSetup
                      );

#if FixedPcdGetBool (PcdTcssSupport) == 1
  //
  // Adjust MMIO size for iTBT
  //
  if (SetupData.EnablePcieTunnelingOverUsb4 == 1) {
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      if (IS_TCSS_SETUP_PCIE_EN (SaSetup.TcssPortConf[Index]) && IS_TCSS_SETUP_PCIE_EN (TcssPortCap[Index])) {
        AdjustedMmioSize += SetupData.ITbtPcieMemRsvd[Index];
      }
    }
  }
#endif
  PcdSet16S (PcdSaMiscMmioSizeAdjustment, AdjustedMmioSize);

  if (((VPD_RCOM_TARGET *)PcdGetPtr (VpdPcdMrcRcompTarget))->RcompTarget) {
#if FixedPcdGet8(PcdFspModeSelection) == 0
    CopyMem ((VOID *)RcompData->RcompTarget, (VOID *)(UINTN)(((VPD_RCOM_TARGET *)PcdGetPtr (VpdPcdMrcRcompTarget))->RcompTarget), sizeof (UINT16) * MRC_MAX_RCOMP_TARGETS);
#else
    CopyMem (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompTarget, (VOID *)(UINTN)(((VPD_RCOM_TARGET *)PcdGetPtr (VpdPcdMrcRcompTarget))->RcompTarget), sizeof (UINT16) * MRC_MAX_RCOMP_TARGETS);
#endif
  }

  if (PcdGetBool (VpdPcdMrcDqPinsInterleavedControl)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DqPinsInterleaved, MemConfig->ExternalInputs.DqPinsInterleaved, PcdGetBool (VpdPcdMrcDqPinsInterleaved));
  }

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[0], MemConfigNoCrc->ChannelToCkdQckMapping[0], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[0]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[1], MemConfigNoCrc->ChannelToCkdQckMapping[1], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[1]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[2], MemConfigNoCrc->ChannelToCkdQckMapping[2], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[2]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[3], MemConfigNoCrc->ChannelToCkdQckMapping[3], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[3]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[4], MemConfigNoCrc->ChannelToCkdQckMapping[4], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[4]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[5], MemConfigNoCrc->ChannelToCkdQckMapping[5], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[5]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[6], MemConfigNoCrc->ChannelToCkdQckMapping[6], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[6]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChannelToCkdQckMapping[7], MemConfigNoCrc->ChannelToCkdQckMapping[7], ((VPD_MRC_CH_2_CKD_QCK *) PcdGetPtr (VpdPcdMrcChannelToCkdQckMapping))->ChannelToCkdQckMapping[7]);

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[0], MemConfigNoCrc->PhyClockToCkdDimm[0], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[0]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[1], MemConfigNoCrc->PhyClockToCkdDimm[1], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[1]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[2], MemConfigNoCrc->PhyClockToCkdDimm[2], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[2]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[3], MemConfigNoCrc->PhyClockToCkdDimm[3], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[3]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[4], MemConfigNoCrc->PhyClockToCkdDimm[4], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[4]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[5], MemConfigNoCrc->PhyClockToCkdDimm[5], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[5]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[6], MemConfigNoCrc->PhyClockToCkdDimm[6], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[6]);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhyClockToCkdDimm[7], MemConfigNoCrc->PhyClockToCkdDimm[7], ((VPD_MRC_DDRIO_2_CKD_CLOCK *) PcdGetPtr (VpdPcdMrcPhyClockToCkdDimm))->PhyClockToCkdDimm[7]);

  // Assume internal SPD is used
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[0], MemConfigNoCrc->SpdAddressTable[0], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[1], MemConfigNoCrc->SpdAddressTable[1], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[2], MemConfigNoCrc->SpdAddressTable[2], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[3], MemConfigNoCrc->SpdAddressTable[3], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[4], MemConfigNoCrc->SpdAddressTable[4], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[5], MemConfigNoCrc->SpdAddressTable[5], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[6], MemConfigNoCrc->SpdAddressTable[6], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[7], MemConfigNoCrc->SpdAddressTable[7], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[8], MemConfigNoCrc->SpdAddressTable[8], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[9], MemConfigNoCrc->SpdAddressTable[9], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[10], MemConfigNoCrc->SpdAddressTable[10], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[11], MemConfigNoCrc->SpdAddressTable[11], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[12], MemConfigNoCrc->SpdAddressTable[12], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[13], MemConfigNoCrc->SpdAddressTable[13], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[14], MemConfigNoCrc->SpdAddressTable[14], 0);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[15], MemConfigNoCrc->SpdAddressTable[15], 0);

  // Check for external SPD presence
  ExternalSpdPresent = PcdGetBool (PcdSpdPresent);
  if (ExternalSpdPresent) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[0], MemConfigNoCrc->SpdAddressTable[0], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[0]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[1], MemConfigNoCrc->SpdAddressTable[1], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[1]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[2], MemConfigNoCrc->SpdAddressTable[2], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[2]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[3], MemConfigNoCrc->SpdAddressTable[3], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[3]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[4], MemConfigNoCrc->SpdAddressTable[4], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[4]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[5], MemConfigNoCrc->SpdAddressTable[5], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[5]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[6], MemConfigNoCrc->SpdAddressTable[6], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[6]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[7], MemConfigNoCrc->SpdAddressTable[7], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[7]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[8], MemConfigNoCrc->SpdAddressTable[8], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[8]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[9], MemConfigNoCrc->SpdAddressTable[9], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[9]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[10], MemConfigNoCrc->SpdAddressTable[10], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[10]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[11], MemConfigNoCrc->SpdAddressTable[11], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[11]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[12], MemConfigNoCrc->SpdAddressTable[12], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[12]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[13], MemConfigNoCrc->SpdAddressTable[13], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[13]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[14], MemConfigNoCrc->SpdAddressTable[14], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[14]);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdAddressTable[15], MemConfigNoCrc->SpdAddressTable[15], ((VPD_MRC_SPD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcSpdAddressTable))->SpdAddressTable[15]);
  }

  for (Index = 0; Index < MEM_CFG_MAX_SOCKETS; Index++) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CkdAddressTable[Index], MemConfigNoCrc->CkdAddressTable[Index], ((VPD_MRC_CKD_ADDRESS_TABLE *) PcdGetPtr (VpdPcdMrcCkdAddressTable))->CkdAddressTable[Index]);
  }

  NullSpdPtr = AllocateZeroPool (SPD_DATA_SIZE);
  ASSERT (NullSpdPtr != NULL);

#if FixedPcdGet8(PcdFspModeSelection) == 1
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr000 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr010 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr020 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr030 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr100 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr110 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr120 = (UINTN) NullSpdPtr;
  ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr130 = (UINTN) NullSpdPtr;
#endif

  if (PcdGet32 (VpdPcdMrcRcompResistor)) {
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompResistor, RcompData->RcompResistor, (UINT8) PcdGet32 (VpdPcdMrcRcompResistor));
  }
  if (((MRC_DQS *)PcdGetPtr (VpdPcdMrcDqsMapCpu2Dram))->DqsMapCpu2Dram) {
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.DqsMapCpu2DramMc0Ch0, (VOID *)MemConfigNoCrc->DqDqsMap->DqsMapCpu2Dram, (VOID *)(UINTN)(((MRC_DQS *)PcdGetPtr (VpdPcdMrcDqsMapCpu2Dram))->DqsMapCpu2Dram), sizeof (UINT8) * MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_NUM_BYTES_MAPPED);
  }
  if (((MRC_DQ *)PcdGetPtr (VpdPcdMrcDqMapCpu2Dram))->DqMapCpu2Dram) {
    COPY_POLICY ((VOID *)((FSPM_UPD *) FspmUpd)->FspmConfig.DqMapCpu2DramMc0Ch0, (VOID *)MemConfigNoCrc->DqDqsMap->DqMapCpu2Dram, (VOID *)(UINTN)(((MRC_DQ *)PcdGetPtr (VpdPcdMrcDqMapCpu2Dram))->DqMapCpu2Dram), sizeof (UINT8) * MEM_CFG_MAX_CONTROLLERS * MEM_CFG_MAX_CHANNELS * MEM_CFG_NUM_BYTES_MAPPED * 8);
  }

  if (PcdGetBool (PcdSpdPresent)) {
    // Clear SPD data so it can be filled in by the MRC init code
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr000, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr010, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr020, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr030, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr100, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr110, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr120, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
    COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr130, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN) NullSpdPtr, SPD_DATA_SIZE);
  } else {
    if (((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->OverrideSpd) {
      if ((UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData) {
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr000,(VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr010,(VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr020, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr030, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[0][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr100, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][0][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr110, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][1][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr120, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][2][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
        COPY_POLICY ((VOID *)(UINTN) ((FSPM_UPD *) FspmUpd)->FspmConfig.MemorySpdPtr130, (VOID *)(UINTN) MemConfigNoCrc->SpdData->SpdData[1][3][0], (VOID *)(UINTN)((SPD_DATA *)PcdGetPtr (VpdPcdMrcSpdData))->SpdData, SPD_DATA_SIZE);
      }
    }
  }

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMrcRetraining, MemConfigNoCrc->DisableMrcRetraining, PcdGet8(VpdPcdDisableMrcRetraining));
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MrcBdatTestType, MemConfigNoCrc->MrcBdatTestType, SaSetup.BdatTestType);
  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MrcBdatEnable, MemConfigNoCrc->MrcBdatEnable, SaSetup.BdatEnable);

  TishDataHob = (TISH_CONFIG_HOB*) GetFirstGuidHob (&gTishDataHobGuid);
  if (TishDataHob != NULL) {
    if ((TishDataHob->SafeLoadingBiosEnableState == 1) || (TishDataHob->TsegMemoryTestStatus == EFI_SUCCESS)) {
      TishDataHob->PprRecoveryStatusEnable = SaSetup.PprRunWCHMATS8    ||
                                             SaSetup.PprRunRetention   ||
                                             SaSetup.PprRunXMarch      ||
                                             SaSetup.PprRunXMarchG     ||
                                             SaSetup.PprRunYMarchShort ||
                                             SaSetup.PprRunYMarchLong  ||
                                             SaSetup.PprRunMmrw
                                             ? 1 : 0;
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SafeLoadingBiosEnableState, MemConfigNoCrc->SafeLoadingBiosEnableState, TishDataHob->SafeLoadingBiosEnableState);
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRecoveryStatusEnable, MemConfigNoCrc->PprRecoveryStatusEnable, TishDataHob->PprRecoveryStatusEnable);
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TsegMemoryTestStatus, MemConfigNoCrc->TsegMemoryTestStatus, TishDataHob->TsegMemoryTestStatus);
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RetryCount, MemConfigNoCrc->RetryCount, TishDataHob->RetryCount);
    }
  }

  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RetrainToWorkingChannel, MemConfigNoCrc->RetrainToWorkingChannel, SaSetup.RetrainToWorkingChannel);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc0Ch0, MemConfig->ExternalInputs.DisableChannel[0][0], SaSetup.DisableMc0Ch0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc0Ch1, MemConfig->ExternalInputs.DisableChannel[0][1], SaSetup.DisableMc0Ch1);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc0Ch2, MemConfig->ExternalInputs.DisableChannel[0][2], SaSetup.DisableMc0Ch2);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc0Ch3, MemConfig->ExternalInputs.DisableChannel[0][3], SaSetup.DisableMc0Ch3);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc1Ch0, MemConfig->ExternalInputs.DisableChannel[1][0], SaSetup.DisableMc1Ch0);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc1Ch1, MemConfig->ExternalInputs.DisableChannel[1][1], SaSetup.DisableMc1Ch1);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc1Ch2,MemConfig->ExternalInputs.DisableChannel[1][2], SaSetup.DisableMc1Ch2);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableMc1Ch3, MemConfig->ExternalInputs.DisableChannel[1][3], SaSetup.DisableMc1Ch3);
  COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.GearRatio, MemConfig->ExternalInputs.GearRatio, SaSetup.GearRatio);

  COPY_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaGvGear, (VOID *) MemConfig->ExternalInputs.SaGvGear, (VOID *) SaSetup.SaGvGear, (MEM_MAX_SAGV_POINTS * sizeof(SaSetup.SaGvGear[0])));
  COPY_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaGvFreq, (VOID *) MemConfig->ExternalInputs.SaGvFreq, (VOID *) SaSetup.SaGvFreq, (MEM_MAX_SAGV_POINTS * sizeof(SaSetup.SaGvFreq[0])));
  if (!EFI_ERROR(Status2)) {
    //
    // Get the Platform Configuration from SetupData
    //
#if FixedPcdGet8(PcdFspModeSelection) == 0
    HostBridgePreMemConfig->MchBar   = (UINT32) PcdGet64 (PcdMchBaseAddress);
#endif
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MmioSizeAdjustment, HostBridgePreMemConfig->MmioSizeAdjustment, PcdGet16 (PcdSaMiscMmioSizeAdjustment));
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnableAbove4GBMmio, HostBridgePreMemConfig->EnableAbove4GBMmio, 1);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CridEnable, HostBridgePreMemConfig->CridEnable, SaSetup.CridEnable);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FabricGVDisable, HostBridgePreMemConfig->FabricGVDisable, SaSetup.FabricGVDisable);

    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ApicLocalAddress, HostBridgePreMemConfig->ApicLocalAddress, PcdGet32 (PcdApicLocalAddress));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.NvmeHcPeiMmioBase, HostBridgePreMemConfig->NvmeHcPeiMmioBase, PcdGet32 (PcdNvmeHcPeiMmioBase));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.NvmeHcPeiMmioLimit, HostBridgePreMemConfig->NvmeHcPeiMmioLimit,PcdGet32 (PcdNvmeHcPeiMmioLimit));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AhciPeiMmioBase, HostBridgePreMemConfig->AhciPeiMmioBase, PcdGet32 (PcdAhciPeiMmioBase));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AhciPeiMmioLimit, HostBridgePreMemConfig->AhciPeiMmioLimit, PcdGet32 (PcdAhciPeiMmioLimit));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EcExtraIoBase, HostBridgePreMemConfig->EcExtraIoBase, PcdGet16 (PcdEcExtraIoBase));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VmdCfgBarBar, HostBridgePreMemConfig->VmdCfgBarBar, (UINTN) PcdGet32 (PcdVmdCfgBarBase));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VmdMemBar1Bar, HostBridgePreMemConfig->VmdMemBar1Bar, (UINTN) PcdGet32 (PcdVmdMemBar1Base));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VmdMemBar2Bar, HostBridgePreMemConfig->VmdMemBar2Bar, (UINTN) PcdGet32 (PcdVmdMemBar2Base));
    //
    // Initialize the Graphics configuration
    //
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IgdDvmt50PreAlloc, IGpuPreMemConfig->IgdDvmt50PreAlloc, SaSetup.IgdDvmt50PreAlloc);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.InternalGraphics, IGpuPreMemConfig->InternalGraphics, SaSetup.InternalGraphics );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PrimaryDisplay, IGpuPreMemConfig->PrimaryDisplay, SaSetup.PrimaryDisplay);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IGpuGsm2Size, IGpuPreMemConfig->IGpuGsm2Size, SaSetup.IGpuGsm2Size);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LMemBar, IGpuPreMemConfig->LMemBar, (UINTN) PcdGet64 (PcdLMemBarAddress));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.GttMmAdr, IGpuPreMemConfig->GttMmAdr, PcdGet32 (PcdGttMmAddress));
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DeltaT12PowerCycleDelay, IGpuPreMemConfig->DeltaT12PowerCycleDelay, 0x0);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MemoryBandwidthCompression, IGpuPreMemConfig->MemoryBandwidthCompression, SaSetup.MemoryBandwidthCompression);

    //
    // Check if SOL is enabled from BIOS Setup Menu
    //
    if (SaSetup.SolFeatureEnabled != VGA_DISPLAY_DISABLED) {
      if (SaSetup.SolFeatureEnabled == 1) {
        VgaInitControl = (VGA_INIT_DURING_MRC_TRAINING_SUPPORT | VGA_TEXT_MODE3_SUPPORT | VGA_DISPLAY_ENABLED);
      } else if (SaSetup.SolFeatureEnabled == 2) {
        VgaInitControl = (VGA_INIT_DURING_MRC_TRAINING_SUPPORT | VGA_GRAPHICS_MODE12_SUPPORT | VGA_DISPLAY_ENABLED);
      } else if (SaSetup.SolFeatureEnabled == 3) {
        VgaInitControl = (VGA_INIT_DURING_MRC_TRAINING_SUPPORT | VGA_NO_EXIT_SUPPORT | VGA_TEXT_MODE3_SUPPORT | VGA_DISPLAY_ENABLED);
      } else if (SaSetup.SolFeatureEnabled == 4) {
        VgaInitControl = (VGA_INIT_DURING_MRC_TRAINING_SUPPORT | VGA_NO_EXIT_SUPPORT | VGA_GRAPHICS_MODE12_SUPPORT | VGA_DISPLAY_ENABLED);
      } else {
        VgaInitControl = VGA_DISPLAY_DISABLED;
      }
    }

    if (IsSimicsEnvironment ()) {
      VgaInitControl = VGA_DISPLAY_DISABLED;
    }

    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VgaInitControl,  IGpuPreMemConfig->VgaInitControl,  VgaInitControl);

    if (VgaInitControl != VGA_DISPLAY_DISABLED) {
      PeiGetSectionFromAnyFv (PcdGetPtr (PcdIntelGraphicsPreMemVbtFileGuid), EFI_SECTION_RAW, 0, &Buffer, &Size);
      Status = CheckLidStatus (&LidStatusOpen);
#if FixedPcdGet8(PcdFspModeSelection) == 1
      ((FSPM_UPD *) FspmUpd)->FspmConfig.VgaMessage                 = (UINT64)VgaMessage;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.VbtPtr                     = (UINT64)Buffer;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.VgaGraphicsMode12ImagePtr  = (UINT64)VgaPlanarImage200x58;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.LogoPixelHeight            = VGA_LOGO_HEIGHT;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.LogoPixelWidth             = VGA_LOGO_WIDTH;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.LogoXPosition              = (VGA_GRAPHICS_MODE12_WIDTH - VGA_LOGO_WIDTH) / 2;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.LogoYPosition              = (VGA_GRAPHICS_MODE12_HEIGHT - VGA_LOGO_HEIGHT) / 2;
      ((FSPM_UPD *) FspmUpd)->FspmConfig.VbtSize                    = (UINT32)Size;
      if (Status == EFI_SUCCESS) {
        ((FSPM_UPD *) FspmUpd)->FspmConfig.LidStatus = LidStatusOpen;
        DEBUG ((DEBUG_INFO, "LidStatus = 0x%x\n", LidStatusOpen));
      } else {
        ((FSPM_UPD *) FspmUpd)->FspmConfig.LidStatus = LidOpen;
      }
#else
      IGpuPreMemConfig->VgaMessage = (VOID *)VgaMessage;
      IGpuPreMemConfig->VbtPtr = Buffer;
      DEBUG ((DEBUG_INFO, "Vbt Pointer from PeiGetSectionFromAnyFv is 0x%x\n", IGpuPreMemConfig->VbtPtr));
      IGpuPreMemConfig->GraphicsMode12Info.ImagePtr           = (UINT8 *)VgaPlanarImage200x58;
      IGpuPreMemConfig->GraphicsMode12Info.LogoPixelHeight    = VGA_LOGO_HEIGHT;
      IGpuPreMemConfig->GraphicsMode12Info.LogoPixelWidth     = VGA_LOGO_WIDTH;
      IGpuPreMemConfig->GraphicsMode12Info.LogoXPosition      = (VGA_GRAPHICS_MODE12_WIDTH - IGpuPreMemConfig->GraphicsMode12Info.LogoPixelWidth) / 2;
      IGpuPreMemConfig->GraphicsMode12Info.LogoYPosition      = (VGA_GRAPHICS_MODE12_HEIGHT - IGpuPreMemConfig->GraphicsMode12Info.LogoPixelHeight) / 2;
      IGpuPreMemConfig->VbtSize = (UINT32)Size;
      if (Status == EFI_SUCCESS) {
        IGpuPreMemConfig->LidStatus = LidStatusOpen;
      } else {
        IGpuPreMemConfig->LidStatus = LidOpen;
      }
#endif
    }

    //
    // Display DDI Initialization
    //
    Hob = GetFirstGuidHob (&gDisplayDdiConfigHobGuid);
    if (Hob == NULL) {
      DisplayDdiConfig     = (VOID *) (UINTN) PcdGet64 (PcdSaDisplayConfigTable);
      DisplayDdiConfigSize = PcdGet16 (PcdSaDisplayConfigTableSize);
    } else {
      DisplayDdiConfig     = (VOID *) ((VPD_DISPLAY_DDI_CONFIG *) GET_GUID_HOB_DATA (Hob))->DdiConfigTable;
      DisplayDdiConfigSize = ((VPD_DISPLAY_DDI_CONFIG *) GET_GUID_HOB_DATA (Hob))->Size;
    }
    if (DisplayDdiConfig != NULL) {
      CopyMem (SaDisplayConfigTable, DisplayDdiConfig, (UINTN) DisplayDdiConfigSize);
      UPDATE_POLICY(((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortAConfig, IGpuPreMemConfig->DdiConfiguration.DdiPortAConfig, SaDisplayConfigTable[0]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBConfig, IGpuPreMemConfig->DdiConfiguration.DdiPortBConfig, SaDisplayConfigTable[1]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortAHpd, IGpuPreMemConfig->DdiConfiguration.DdiPortAHpd,    SaDisplayConfigTable[2]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBHpd, IGpuPreMemConfig->DdiConfiguration.DdiPortBHpd,    SaDisplayConfigTable[3]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortCHpd, IGpuPreMemConfig->DdiConfiguration.DdiPortCHpd,    SaDisplayConfigTable[4]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort1Hpd, IGpuPreMemConfig->DdiConfiguration.DdiPort1Hpd,    SaDisplayConfigTable[5]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort2Hpd, IGpuPreMemConfig->DdiConfiguration.DdiPort2Hpd,    SaDisplayConfigTable[6]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort3Hpd, IGpuPreMemConfig->DdiConfiguration.DdiPort3Hpd,    SaDisplayConfigTable[7]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort4Hpd, IGpuPreMemConfig->DdiConfiguration.DdiPort4Hpd,    SaDisplayConfigTable[8]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortADdc, IGpuPreMemConfig->DdiConfiguration.DdiPortADdc,    SaDisplayConfigTable[9]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortBDdc, IGpuPreMemConfig->DdiConfiguration.DdiPortBDdc,    SaDisplayConfigTable[10]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPortCDdc, IGpuPreMemConfig->DdiConfiguration.DdiPortCDdc,    SaDisplayConfigTable[11]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort1Ddc, IGpuPreMemConfig->DdiConfiguration.DdiPort1Ddc,    SaDisplayConfigTable[12]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort2Ddc, IGpuPreMemConfig->DdiConfiguration.DdiPort2Ddc,    SaDisplayConfigTable[13]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort3Ddc, IGpuPreMemConfig->DdiConfiguration.DdiPort3Ddc,    SaDisplayConfigTable[14]);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdiPort4Ddc, IGpuPreMemConfig->DdiConfiguration.DdiPort4Ddc,    SaDisplayConfigTable[15]);
    }
    GuidHob = GetFirstGuidHob (&gGopConfigDriverHobGuid);
    if (GuidHob != NULL) {
      GopConfigDriverHob = (GOP_CONFIG_DRIVER_HOB *) GET_GUID_HOB_DATA (GuidHob);
      GopConfigDriverHob->GopConfigEnable = SaSetup.GopConfigEnable;
    }

#if FixedPcdGetBool(PcdHgEnable) == 1
    //
    // Initialize the Hybrid Graphics Configuration
    //
    if (SaSetup.PrimaryDisplay == DISPLAY_HG) { // When Primary Display is HG
      HgSubSystemId        = 0x2112;
#if FixedPcdGet8(PcdFspModeSelection) == 1
      ((FSPM_UPD *) FspmUpd)->FspmConfig.PrimaryDisplay = DISPLAY_IGD;
#else
      IGpuPreMemConfig->PrimaryDisplay = DISPLAY_IGD;
#endif
    } else {
      HgSubSystemId        = 0x2212;
    }
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.HgSubSystemId, HgGpioData->HgSubSystemId, HgSubSystemId);
    PcdSet16S (PcdHgSubSystemId, HgSubSystemId);
#endif

    ///
    /// Initialize the VTD Configuration
    ///
    if (SaSetup.EnableVtd == 0) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VtdDisable, Vtd->VtdDisable,   1);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PreBootDmaMask, Vtd->PreBootDmaMask, 0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DmaBufferSize, Vtd->DmaBufferSize, 0);
      for (Index = 0; Index < GetMaxVtdEngineNumber(); Index++) {
        UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.VtdBaseAddress[Index], Vtd->BaseAddress[Index], 0);
      }
    } else {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VtdDisable, Vtd->VtdDisable, IsVtdSupported() ? 0 : 1);
      for (Index = 0; Index < GetMaxVtdEngineNumber(); Index++) {
        UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.VtdBaseAddress[Index], Vtd->BaseAddress[Index], GetVtdBaseAddress (Index));
      }
      if ((SetupData.ControlIommu) != 0) {
        UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PreBootDmaMask, Vtd->PreBootDmaMask, 1);
      } else {
        UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PreBootDmaMask, Vtd->PreBootDmaMask, 0);
      }

      if (BootMode == BOOT_ON_S3_RESUME) {
        UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DmaBufferSize, Vtd->DmaBufferSize, PcdGet32 (PcdVTdPeiDmaBufferSizeS3));
      } else {
        UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DmaBufferSize, Vtd->DmaBufferSize, PcdGet32 (PcdVTdPeiDmaBufferSize));
      }
    }
    // Update VTD enabled PCD to be consumed by VTIO
    PcdSetBoolS (PcdIsVtdEnabled, (BOOLEAN)SaSetup.EnableVtd);

    //
    // StreamTracer
    //
    if (SetupData.StreamTracerMode != StreamTracerUserInputSize) {
      StreamTracerSize = SetupData.StreamTracerMode;
    } else {
      StreamTracerSize = SetupData.StreamTracerSize;
    }
    if (SaSetup.NestedSupport == 1) {
      VtdCpabilityControl |= BIT0;
    }
    if (SaSetup.PostedInterruptSupport == 1) {
      VtdCpabilityControl |= BIT1;
    }
    //
    // Align StreamTracer Size to power of two
    //
    if (!IsPowerOfTwo (StreamTracerSize)) {
      StreamTracerSize = GetPowerOfTwo32 (StreamTracerSize) << 1;
    }
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.StreamTracerMode, TelemetryPreMemConfig->StreamTracerMode, SetupData.StreamTracerMode);
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.StreamTracerSize, TelemetryPreMemConfig->StreamTracerSize, StreamTracerSize);

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DmaControlGuarantee, Vtd->DmaControlGuarantee, SaSetup.DmaControlGuarantee);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VtdCapabilityControl, Vtd->CapabilityControl, VtdCpabilityControl);
#if FixedPcdGetBool(PcdIpuEnable) == 1
    //
    // Initialize IPU PreMem Configuration
    //
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaIpuEnable, IpuPreMemPolicy->IpuEnable, SaSetup.SaIpuEnable);
#endif
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.HobBufferSize, MemConfig->ExternalInputs.HobBufferSize, SaSetup.HobBufferSize      );

#if FixedPcdGetBool(PcdOverclockEnable) == 1
    if (OcSetup.OverclockingSupport == 0) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdProfileSelected, MemConfig->ExternalInputs.MemoryProfile,  0);
    } else {
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdProfileSelected, MemConfig->ExternalInputs.MemoryProfile, SaSetup.SpdProfileSelected);
    }
#else
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpdProfileSelected, MemConfig->ExternalInputs.MemoryProfile,  0);
#endif

    //
    // If user custom profile is selected, we will send the setup values to the MRC.
    // The setup values will be the current memory settings plus user override values.
    // If any other profile is selected or a WDT timeout has occured, we zero out
    // the settings just to be safe.
    //
    gWdtPei = NULL;
    Status = PeiServicesLocatePpi(
               &gWdtPpiGuid,
               0,
               NULL,
               (VOID **) &gWdtPei
               );
    if (gWdtPei != NULL) {
      WdtTimeout = gWdtPei->CheckStatus();
    } else {
      WdtTimeout = FALSE;
    }

    if ((SaSetup.SpdProfileSelected == UserDefined) && (WdtTimeout == FALSE)) {
      //
      // If USER custom profile is selected, we will start the WDT.
      //
      if (gWdtPei != NULL) {
        Status = gWdtPei->ReloadAndStart(WDT_TIMEOUT);
        if (EFI_ERROR (Status)) {
          DEBUG ((DEBUG_ERROR, "Status-%r\n",Status));
        }
      }
      //
      // Read DDR RefClk setting selected in Bios setup, 0 for 133MHz and 1 for 100MHz.
      //
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCL, MemConfig->ExternalInputs.tCL,          SaSetup.tCL          );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRCDtRP, MemConfig->ExternalInputs.tRCDtRP,      SaSetup.tRCDtRP      );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRAS, MemConfig->ExternalInputs.tRAS,         SaSetup.tRAS         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWR, MemConfig->ExternalInputs.tWR,          SaSetup.tWR          );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRFC, MemConfig->ExternalInputs.tRFC,         SaSetup.tRFC         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRD, MemConfig->ExternalInputs.tRRD,         SaSetup.tRRD         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWTR, MemConfig->ExternalInputs.tWTR,         SaSetup.tWTR         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRTP, MemConfig->ExternalInputs.tRTP,         SaSetup.tRTP         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tFAW, MemConfig->ExternalInputs.tFAW,         SaSetup.tFAW         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCWL, MemConfig->ExternalInputs.tCWL,         SaSetup.tCWL         );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tREFI, MemConfig->ExternalInputs.tREFI,        SaSetup.tREFI        );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VddVoltage, MemConfig->ExternalInputs.VddVoltage,   SaSetup.MemoryVoltage); // Vddq in [mV], 0 = platform default.
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ratio, MemConfig->ExternalInputs.Ratio,        SaSetup.DdrRatio     );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.NModeSupport, MemConfig->ExternalInputs.NModeSupport, SaSetup.NModeSupport );
    } else {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCL, MemConfig->ExternalInputs.tCL,                 0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRCDtRP, MemConfig->ExternalInputs.tRCDtRP,         0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRAS, MemConfig->ExternalInputs.tRAS,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWR, MemConfig->ExternalInputs.tWR,                 0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRFC, MemConfig->ExternalInputs.tRFC,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRD, MemConfig->ExternalInputs.tRRD,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWTR, MemConfig->ExternalInputs.tWTR,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRTP, MemConfig->ExternalInputs.tRTP,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tFAW, MemConfig->ExternalInputs.tFAW,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCWL, MemConfig->ExternalInputs.tCWL,               0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tREFI, MemConfig->ExternalInputs.tREFI,             0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VddVoltage, MemConfig->ExternalInputs.VddVoltage,   0); // Use platform default as the safe value.
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ratio, MemConfig->ExternalInputs.Ratio,             0);
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.NModeSupport, MemConfig->ExternalInputs.NModeSupport, 0);
    }

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EccSupport, MemConfig->ExternalInputs.EccSupport,              SaSetup.EccSupport                                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RemapEnable, MemConfig->ExternalInputs.RemapEnable,             SaSetup.RemapEnable                                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ScramblerSupport, MemConfig->ExternalInputs.ScramblerSupport,        SaSetup.ScramblerSupport                                  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RmtPerTask, MemConfig->ExternalInputs.RmtPerTask,              SaSetup.RmtPerTask                                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TrainTrace, MemConfig->ExternalInputs.TrainTrace,              SaSetup.TrainTrace                                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisPgCloseIdleTimeout, MemConfig->ExternalInputs.DisPgCloseIdleTimeout,   SaSetup.DisPgCloseIdleTimeout                             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ibecc, MemConfig->ExternalInputs.Ibecc,                   SaSetup.Ibecc                                             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccParity, MemConfig->ExternalInputs.IbeccParity,             SaSetup.IbeccParity                                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccOperationMode, MemConfig->ExternalInputs.IbeccOperationMode,      SaSetup.IbeccOperationMode                                );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChHashOverride, MemConfig->ExternalInputs.ChHashOverride,          SaSetup.ChHashOverride                                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DvfsqEnabled, MemConfig->ExternalInputs.DvfsqEnabled,            SaSetup.DvfsqEnabled                                      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DvfscEnabled, MemConfig->ExternalInputs.DvfscEnabled,            SaSetup.DvfscEnabled && PcdGet8 (VpdPcdVdd2lPowerRailPresent));
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VoltageReadout, MemConfig->ExternalInputs.VoltageReadout,          SaSetup.VoltageReadout                                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CccHalfFrequency, MemConfig->ExternalInputs.CccHalfFrequency,        SaSetup.CccHalfFrequency                                  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Ddr5AutoPrechargeEnable, MemConfig->ExternalInputs.Ddr5AutoPrechargeEnable, SaSetup.Ddr5AutoPrechargeEnable                           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Lp5SplitACTEnable, MemConfig->ExternalInputs.Lp5SplitACTEnable,       SaSetup.Lp5SplitACTEnable                                 );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SubChHashOverride, MemConfig->ExternalInputs.SubChHashOverride,       SaSetup.SubChHashOverride                                 );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ProbelessTrace, MemConfig->ExternalInputs.ProbelessTrace,          SaSetup.ProbelessTrace                                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChHashInterleaveBit, MemConfig->ExternalInputs.ChHashInterleaveBit,     SaSetup.ChHashInterleaveBit                               );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MsHashOverride,      MemConfig->ExternalInputs.MsHashOverride,      SaSetup.MsHashOverride     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MsHashMask,          MemConfig->ExternalInputs.MsHashMask,          SaSetup.MsHashMask         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MsHashInterleaveBit, MemConfig->ExternalInputs.MsHashInterleaveBit, SaSetup.MsHashInterleaveBit);

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ECT,   MemConfig->ExternalInputs.TrainingEnables.ECT,             SaSetup.ECT            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SOT,   MemConfig->ExternalInputs.TrainingEnables.SOT,             SaSetup.SOT            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ERDMPRTC2D,  MemConfig->ExternalInputs.TrainingEnables.ERDMPRTC2D,      SaSetup.ERDMPRTC2D     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDMPRT,  MemConfig->ExternalInputs.TrainingEnables.RDMPRT,          SaSetup.RDMPRT         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RCVET,  MemConfig->ExternalInputs.TrainingEnables.RCVET,           SaSetup.RCVET          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.JWRL,  MemConfig->ExternalInputs.TrainingEnables.JWRL,            SaSetup.JWRL           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EWRTC2D,  MemConfig->ExternalInputs.TrainingEnables.EWRTC2D,         SaSetup.EWRTC2D        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ERDTC2D,  MemConfig->ExternalInputs.TrainingEnables.ERDTC2D,         SaSetup.ERDTC2D        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.UNMATCHEDWRTC1D,  MemConfig->ExternalInputs.TrainingEnables.UNMATCHEDWRTC1D, SaSetup.UNMATCHEDWRTC1D);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRTC1D,  MemConfig->ExternalInputs.TrainingEnables.WRTC1D,          SaSetup.WRTC1D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRVC1D,  MemConfig->ExternalInputs.TrainingEnables.WRVC1D,          SaSetup.WRVC1D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDTC1D,  MemConfig->ExternalInputs.TrainingEnables.RDTC1D,          SaSetup.RDTC1D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDVC1D,  MemConfig->ExternalInputs.TrainingEnables.RDVC1D,          SaSetup.RDVC1D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRTC2D,  MemConfig->ExternalInputs.TrainingEnables.WRTC2D,          SaSetup.WRTC2D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDTC2D,  MemConfig->ExternalInputs.TrainingEnables.RDTC2D,          SaSetup.RDTC2D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRVC2D,  MemConfig->ExternalInputs.TrainingEnables.WRVC2D,          SaSetup.WRVC2D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDVC2D,  MemConfig->ExternalInputs.TrainingEnables.RDVC2D,          SaSetup.RDVC2D         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRDSEQT,  MemConfig->ExternalInputs.TrainingEnables.WRDSEQT,         SaSetup.WRDSEQT        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DQSRF,  MemConfig->ExternalInputs.TrainingEnables.DQSRF,           SaSetup.DQSRF          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDDQSODTT,  MemConfig->ExternalInputs.TrainingEnables.RDDQSODTT,       SaSetup.RDDQSODTT      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDEQT,  MemConfig->ExternalInputs.TrainingEnables.RDEQT,           SaSetup.RDEQT          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DUNITC,  MemConfig->ExternalInputs.TrainingEnables.DUNITC,          SaSetup.DUNITC         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CMDVC,  MemConfig->ExternalInputs.TrainingEnables.CMDVC,           SaSetup.CMDVC          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LCT,  MemConfig->ExternalInputs.TrainingEnables.LCT,             SaSetup.LCT            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RTL,  MemConfig->ExternalInputs.TrainingEnables.RTL,             SaSetup.RTL            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TAT,  MemConfig->ExternalInputs.TrainingEnables.TAT,             SaSetup.TAT            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RMT,  MemConfig->ExternalInputs.TrainingEnables.RMT,             SaSetup.RMT            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RMTEVENODD,  MemConfig->ExternalInputs.TrainingEnables.RMTEVENODD,      SaSetup.RMTEVENODD     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ALIASCHK,  MemConfig->ExternalInputs.TrainingEnables.ALIASCHK,        SaSetup.ALIASCHK       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RCVENC1D,  MemConfig->ExternalInputs.TrainingEnables.RCVENC1D,        SaSetup.RCVENC1D       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RMC,  MemConfig->ExternalInputs.TrainingEnables.RMC,             SaSetup.RMC            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PRETRAIN,  MemConfig->ExternalInputs.TrainingEnables.PRETRAIN,        SaSetup.PRETRAIN       );

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DCCPICODELUT,  MemConfig->ExternalInputs.TrainingEnables2.DCCPICODELUT,   SaSetup.DCCPICODELUT   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DIMMODTT,  MemConfig->ExternalInputs.TrainingEnables2.DIMMODTT,       SaSetup.DIMMODTT       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DIMMRONT,  MemConfig->ExternalInputs.TrainingEnables2.DIMMRONT,       SaSetup.DIMMRONT       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TXTCO,  MemConfig->ExternalInputs.TrainingEnables2.TXTCO,          SaSetup.TXTCO          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CLKTCO,  MemConfig->ExternalInputs.TrainingEnables2.CLKTCO,         SaSetup.CLKTCO         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CMDSR,  MemConfig->ExternalInputs.TrainingEnables2.CMDSR,          SaSetup.CMDSR          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CMDDSEQ,  MemConfig->ExternalInputs.TrainingEnables2.CMDDSEQ,        SaSetup.CMDDSEQ        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DIMMODTCA,  MemConfig->ExternalInputs.TrainingEnables2.DIMMODTCA,      SaSetup.DIMMODTCA      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DDR5ODTTIMING,  MemConfig->ExternalInputs.TrainingEnables2.DDR5ODTTIMING,  SaSetup.DDR5ODTTIMING  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WREQT,  MemConfig->ExternalInputs.TrainingEnables2.WREQT,  SaSetup.WREQT  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DBI,  MemConfig->ExternalInputs.TrainingEnables2.DBI,            SaSetup.DBI            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DLLDCC,  MemConfig->ExternalInputs.TrainingEnables2.DLLDCC,         SaSetup.DLLDCC         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DLLBWSEL,  MemConfig->ExternalInputs.TrainingEnables2.DLLBWSEL,       SaSetup.DLLBWSEL       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDVREFDC,  MemConfig->ExternalInputs.TrainingEnables2.RDVREFDC,       SaSetup.RDVREFDC       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDTCIDLE,  MemConfig->ExternalInputs.TrainingEnables2.RDTCIDLE,       SaSetup.RDTCIDLE       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RMTBIT,  MemConfig->ExternalInputs.TrainingEnables2.RMTBIT,         SaSetup.RMTBIT         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DQDQSSWZ,  MemConfig->ExternalInputs.TrainingEnables2.DQDQSSWZ,       SaSetup.DQDQSSWZ       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.REFPI,  MemConfig->ExternalInputs.TrainingEnables2.REFPI,          SaSetup.REFPI          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DCCLP5READDCA,  MemConfig->ExternalInputs.TrainingEnables2.DCCLP5READDCA,  SaSetup.DCCLP5READDCA  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DCCDDR5READDCA,  MemConfig->ExternalInputs.TrainingEnables2.DCCDDR5READDCA,  SaSetup.DCCDDR5READDCA  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VCCCLKFF,  MemConfig->ExternalInputs.TrainingEnables2.VCCCLKFF,       SaSetup.VCCCLKFF       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FUNCDCCDQS,  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCDQS,     SaSetup.FUNCDCCDQS     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FUNCDCCCLK,  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCCLK,     SaSetup.FUNCDCCCLK     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FUNCDCCWCK,  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCWCK,     SaSetup.FUNCDCCWCK     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FUNCDCCDQ,  MemConfig->ExternalInputs.TrainingEnables2.FUNCDCCDQ,      SaSetup.FUNCDCCDQ      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DATAPILIN,  MemConfig->ExternalInputs.TrainingEnables2.DATAPILIN,      SaSetup.DATAPILIN      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DDR5XTALK,  MemConfig->ExternalInputs.TrainingEnables2.DDR5XTALK,      SaSetup.DDR5XTALK      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DCCLP5WCKDCA,  MemConfig->ExternalInputs.TrainingEnables2.DCCLP5WCKDCA,   SaSetup.DCCLP5WCKDCA   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RXUNMATCHEDCAL,  MemConfig->ExternalInputs.TrainingEnables2.RXUNMATCHEDCAL, SaSetup.RXUNMATCHEDCAL );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRTDIMMDFE,  MemConfig->ExternalInputs.TrainingEnables2.WRTDIMMDFE,     SaSetup.WRTDIMMDFE     );

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RXDQSDCC,  MemConfig->ExternalInputs.TrainingEnables3.RXDQSDCC,       SaSetup.RXDQSDCC       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DIMMNTODT,  MemConfig->ExternalInputs.TrainingEnables3.DIMMNTODT,      SaSetup.DIMMNTODT      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TXDQSDCC,  MemConfig->ExternalInputs.TrainingEnables3.TXDQSDCC,   SaSetup.TXDQSDCC   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RXVREFPERBIT,  MemConfig->ExternalInputs.TrainingEnables3.RXVREFPERBIT,   SaSetup.RXVREFPERBIT   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LVRAUTOTRIM,  MemConfig->ExternalInputs.TrainingEnables3.LVRAUTOTRIM,    SaSetup.LVRAUTOTRIM    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.OPTIMIZECOMP,  MemConfig->ExternalInputs.TrainingEnables3.OPTIMIZECOMP,   SaSetup.OPTIMIZECOMP   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WRTRETRAIN,  MemConfig->ExternalInputs.TrainingEnables3.WRTRETRAIN,     SaSetup.WRTRETRAIN     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TLINECLKCAL,  MemConfig->ExternalInputs.TrainingEnables3.TLINECLKCAL,    SaSetup.TLINECLKCAL    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DCCPISERIALCAL,  MemConfig->ExternalInputs.TrainingEnables3.DCCPISERIALCAL, SaSetup.DCCPISERIALCAL );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PHASECLKCAL,  MemConfig->ExternalInputs.TrainingEnables3.PHASECLKCAL,    SaSetup.PHASECLKCAL    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WCKPADDCCCAL,  MemConfig->ExternalInputs.TrainingEnables3.WCKPADDCCCAL,   SaSetup.WCKPADDCCCAL   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDCTLET,  MemConfig->ExternalInputs.TrainingEnables3.RDCTLET,        SaSetup.RDCTLET        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RDDQODTT,  MemConfig->ExternalInputs.TrainingEnables3.RDDQODTT,       SaSetup.RDDQODTT       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EMPHASIS,  MemConfig->ExternalInputs.TrainingEnables3.EMPHASIS,       SaSetup.EMPHASIS       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DIMMRXOFFSET,  MemConfig->ExternalInputs.TrainingEnables3.DIMMRXOFFSET,       SaSetup.DIMMRXOFFSET       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RXDQSVOCC,  MemConfig->ExternalInputs.TrainingEnables3.RXDQSVOCC,       SaSetup.RXDQSVOCC       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MrcTimeMeasure, MemConfig->ExternalInputs.MrcTimeMeasure,          SaSetup.MrcTimeMeasure         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MrcFastBoot, MemConfig->ExternalInputs.MrcFastBoot,             SaSetup.MrcFastBoot            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RankInterleave, MemConfig->ExternalInputs.RankInterleave,          SaSetup.RankInterleave         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnhancedInterleave, MemConfig->ExternalInputs.EnhancedInterleave,      SaSetup.EnhancedInterleave     );

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChHashEnable, MemConfig->ExternalInputs.ChHashEnable,            SaSetup.ChHashEnable           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnablePwrDn, MemConfig->ExternalInputs.EnablePwrDn,             SaSetup.EnablePwrDn            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnablePwrDnLpddr, MemConfig->ExternalInputs.EnablePwrDnLpddr,        SaSetup.EnablePwrDnLpddr       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SrefCfgEna, MemConfig->ExternalInputs.SrefCfgEna,              SaSetup.SrefCfgEna             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThrtCkeMinDefeat, MemConfig->ExternalInputs.ThrtCkeMinDefeat,        SaSetup.ThrtCkeMinDefeat       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThrtCkeMinDefeatLpddr,MemConfig->ExternalInputs.ThrtCkeMinDefeatLpddr,   SaSetup.ThrtCkeMinDefeatLpddr  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ForceSingleRank, MemConfig->ExternalInputs.ForceSingleRank,         SaSetup.ForceSingleRank        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DynamicMemoryBoost, MemConfig->ExternalInputs.DynamicMemoryBoost,      SaSetup.DynamicMemoryBoost     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RealtimeMemoryFrequency, MemConfig->ExternalInputs.RealtimeMemoryFrequency, SaSetup.RealtimeMemoryFrequency);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ExitOnFailure, MemConfig->ExternalInputs.ExitOnFailure,           SaSetup.ExitOnFailure          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaGv, MemConfig->ExternalInputs.SaGv,                    SaSetup.SaGv                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsDdr5MR7WicaSupported, MemConfig->ExternalInputs.IsDdr5MR7WicaSupported,  SaSetup.IsDdr5MR7WicaSupported );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CccPinsInterleaved, MemConfig->ExternalInputs.CccPinsInterleaved,      SaSetup.CccPinsInterleaved     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SrefCfgIdleTmr, MemConfig->ExternalInputs.SrefCfgIdleTmr,          SaSetup.SrefCfgIdleTmr         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChHashMask, MemConfig->ExternalInputs.ChHashMask,              SaSetup.ChHashMask             );
    //
    // DDR Frequency Limit (0 = Auto)
    // The values must match the definitions in IcelakeSiliconPkg\SystemAgent\MemoryInit\Include\MrcInterface.h
    //
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdrFreqLimit, MemConfig->ExternalInputs.DdrFreqLimit,          SaSetup.DdrFreqLimit         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThrtCkeMinTmr, MemConfig->ExternalInputs.ThrtCkeMinTmr,         SaSetup.ThrtCkeMinTmr        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ThrtCkeMinTmrLpddr, MemConfig->ExternalInputs.ThrtCkeMinTmrLpddr,    SaSetup.ThrtCkeMinTmrLpddr   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PerBankRefresh, MemConfig->ExternalInputs.PerBankRefresh,        SaSetup.PerBankRefresh       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SaGvWpMask, MemConfig->ExternalInputs.SaGvWpMask,            SaSetup.SaGvWpMask           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ExtendedBankHashing, MemConfig->ExternalInputs.ExtendedBankHashing,   SaSetup.ExtendedBankHashing  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.McRefreshRate, MemConfig->ExternalInputs.McRefreshRate, SaSetup.McRefreshRate );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RetrainOnFastFail, MemConfig->ExternalInputs.RetrainOnFastFail,     SaSetup.RetrainOnFastFail    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PowerDownMode, MemConfig->ExternalInputs.PowerDownMode,         SaSetup.PowerDownMode        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PwdwnIdleCounter, MemConfig->ExternalInputs.PwdwnIdleCounter,      SaSetup.PwdwnIdleCounter     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Lp5CccConfig, MemConfig->ExternalInputs.Lp5CccConfig,          PcdGet8 (VpdPcdMrcLp5CccConfig)); // BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 selects Ascending mapping and 1 selects Descending mapping.
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RMTLoopCount, MemConfig->ExternalInputs.RMTLoopCount,          SaSetup.RMTLoopCount         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdrOneDpc, MemConfig->ExternalInputs.DdrOneDpc,             SaSetup.DdrOneDpc            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VddqVoltageOverride, MemConfig->ExternalInputs.VddqVoltageOverride,   SaSetup.VddqVoltageOverride  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VccIogVoltageOverride, MemConfig->ExternalInputs.VccIogVoltageOverride, SaSetup.VccIogVoltageOverride);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VccClkVoltageOverride, MemConfig->ExternalInputs.VccClkVoltageOverride, SaSetup.VccClkVoltageOverride);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LpddrRttWr, MemConfig->ExternalInputs.LpddrRttWr,            SaSetup.LpddrRttWr           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LpddrRttCa, MemConfig->ExternalInputs.LpddrRttCa,            SaSetup.LpddrRttCa           );

    for (Index = 0; Index < MAX_IBECC_REGIONS; Index ++) {
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccProtectedRegionEnable[Index], MemConfig->ExternalInputs.IbeccProtectedRegionEnable[Index], SaSetup.IbeccProtectedRegionEnable[Index]);
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccProtectedRegionBase[Index], MemConfig->ExternalInputs.IbeccProtectedRegionBase[Index],   SaSetup.IbeccProtectedRegionBase[Index]  );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccProtectedRegionMask[Index], MemConfig->ExternalInputs.IbeccProtectedRegionMask[Index],   SaSetup.IbeccProtectedRegionMask[Index]  );
    }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccEccInjControl,  MemConfig->ExternalInputs.IbeccEccInjControl,  SaSetup.IbeccEccInjControl);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccEccInjAddrBase, MemConfig->ExternalInputs.IbeccEccInjAddrBase, SaSetup.IbeccEccInjAddrBase);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IbeccEccInjCount,    MemConfig->ExternalInputs.IbeccEccInjCount,    SaSetup.IbeccEccInjCount);

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.IsWckIdleExitEnabled,   MemConfig->ExternalInputs.IsWckIdleExitEnabled,   SaSetup.IsWckIdleExitEnabled);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FlexibleAnalogSettings, MemConfig->ExternalInputs.FlexibleAnalogSettings, SaSetup.FlexibleAnalogSettings);

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AllowOppRefBelowWriteThrehold, MemConfig->ExternalInputs.AllowOppRefBelowWriteThrehold, SaSetup.AllowOppRefBelowWriteThrehold);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WriteThreshold, MemConfig->ExternalInputs.WriteThreshold,        SaSetup.WriteThreshold               );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MarginLimitL2, MemConfig->ExternalInputs.MarginLimitL2,          SaSetup.MarginLimitL2                );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRFCpb, MemConfig->ExternalInputs.tRFCpb,                        SaSetup.tRFCpb                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRFC2, MemConfig->ExternalInputs.tRFC2,                          SaSetup.tRFC2                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRD_L, MemConfig->ExternalInputs.tRRD_L,                        SaSetup.tRRD_L                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRD_S, MemConfig->ExternalInputs.tRRD_S,                        SaSetup.tRRD_S                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWTR_L, MemConfig->ExternalInputs.tWTR_L,                        SaSetup.tWTR_L                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Vdd2Mv, MemConfig->ExternalInputs.Vdd2Mv,                        SaSetup.Vdd2Mv                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MarginLimitCheck, MemConfig->ExternalInputs.MarginLimitCheck,    SaSetup.MarginLimitCheck             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Lfsr0Mask, MemConfig->ExternalInputs.Lfsr0Mask,                  SaSetup.Lfsr0Mask                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Lfsr1Mask, MemConfig->ExternalInputs.Lfsr1Mask,                  SaSetup.Lfsr1Mask                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RefreshWm, MemConfig->ExternalInputs.RefreshWm,                  SaSetup.RefreshWm                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DramRfmMode, MemConfig->ExternalInputs.DramRfmMode,              SaSetup.DramRfmMode                  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TargetedRowRefreshMode, MemConfig->ExternalInputs.TargetedRowRefreshMode,        SaSetup.TargetedRowRefreshMode       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DrfmBrc, MemConfig->ExternalInputs.DrfmBrc,                       SaSetup.DrfmBrc                      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CmdMirror, MemConfig->ExternalInputs.CmdMirror,                     PcdGet8 (VpdPcdMrcCmdMirror)         ); // BitMask where bits [3:0] are controller 0 Channel [3:0] and [7:4] are Controller 1 Channel [3:0].  0 = No Command Mirror and 1 = Command Mirror.
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SafeModeOverride, MemConfig->ExternalInputs.SafeModeOverride,              SaSetup.SafeModeOverride             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DdrSafeMode, MemConfig->ExternalInputs.DdrSafeMode,                   SaSetup.DdrSafeMode                  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.McSafeMode, MemConfig->ExternalInputs.McSafeMode,                    SaSetup.McSafeMode                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LpMode, MemConfig->ExternalInputs.LpMode,                        SaSetup.LpMode                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LpMode4, MemConfig->ExternalInputs.LpMode4,                       SaSetup.LpMode4                      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EnPeriodicComp, MemConfig->ExternalInputs.EnPeriodicComp,                SaSetup.EnPeriodicComp               );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRFC4, MemConfig->ExternalInputs.tRFC4,                         SaSetup.tRFC4                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCCD_L, MemConfig->ExternalInputs.tCCD_L,                        SaSetup.tCCD_L                       );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWTR_S, MemConfig->ExternalInputs.tWTR_S,                        SaSetup.tWTR_S                       );
#if FixedPcdGet8(PcdFspModeSelection) == 0
    COPY_POLICY (NULL, (VOID *)(UINTN) MemConfig->ExternalInputs.DimmOdt, (VOID *) SaSetup.DimmOdt, (UINTN) sizeof(SaSetup.DimmOdt));
#endif

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DFETap1StepSize, MemConfig->ExternalInputs.DFETap1StepSize,               SaSetup.DFETap1StepSize              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DFETap2StepSize, MemConfig->ExternalInputs.DFETap2StepSize,               SaSetup.DFETap2StepSize              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CsVrefLow, MemConfig->ExternalInputs.CsVrefLow,                     SaSetup.CsVrefLow                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CsVrefHigh, MemConfig->ExternalInputs.CsVrefHigh,                    SaSetup.CsVrefHigh                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CaVrefLow, MemConfig->ExternalInputs.CaVrefLow,                     SaSetup.CaVrefLow                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CaVrefHigh, MemConfig->ExternalInputs.CaVrefHigh,                    SaSetup.CaVrefHigh                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EccCorrectionMode, MemConfig->ExternalInputs.EccCorrectionMode,             SaSetup.EccCorrectionMode            );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.EccGranularity32BEn, MemConfig->ExternalInputs.EccGranularity32BEn,           SaSetup.EccGranularity32BEn          );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.LowerBasicMemTestSize, MemConfig->ExternalInputs.LowerBasicMemTestSize,         SaSetup.LowerBasicMemTestSize        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MCREGOFFSET, MemConfig->ExternalInputs.MCREGOFFSET,                   SaSetup.MCREGOFFSET                  );

#if FixedPcdGet8(PcdFspModeSelection) == 0
    MemConfig->ExternalInputs.ForceRetrainPath  = SaSetup.ForceRetrainPath;
    MemConfig->ExternalInputs.PcuDdrVoltage     = SaSetup.PcuDdrVoltage;
#endif

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AsyncOdtDis, MemConfig->ExternalInputs.AsyncOdtDis,                 SaSetup.AsyncOdtDis        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.OpportunisticRead, MemConfig->ExternalInputs.OpportunisticRead,           SaSetup.OpportunisticRead  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Disable2CycleBypass, MemConfig->ExternalInputs.Disable2CycleBypass,         SaSetup.Disable2CycleBypass);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CAVrefCtlOffset, MemConfigNoCrc->CAVrefCtlOffset, SaSetup.CAVrefCtlOffset    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VrefCtlOffset,   MemConfigNoCrc->VrefCtlOffset,   SaSetup.VrefCtlOffset      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ClkPiCodeOffset, MemConfigNoCrc->ClkPiCodeOffset, SaSetup.ClkPiCodeOffset    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcvEnOffset,     MemConfigNoCrc->RcvEnOffset,     SaSetup.RcvEnOffset        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RxDqsOffset,     MemConfigNoCrc->RxDqsOffset,     SaSetup.RxDqsOffset        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TxDqOffset,      MemConfigNoCrc->TxDqOffset,      SaSetup.TxDqOffset         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TxDqsOffset,     MemConfigNoCrc->TxDqsOffset,     SaSetup.TxDqsOffset        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VrefOffset,      MemConfigNoCrc->VrefOffset,      SaSetup.VrefOffset         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CntrlrMask,      MemConfigNoCrc->CntrlrMask,      SaSetup.CntrlrMask         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ChMask,          MemConfigNoCrc->ChMask,          SaSetup.ChMask             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRSG, MemConfig->ExternalInputs.Trrsg,                       SaSetup.tRRSG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRDG, MemConfig->ExternalInputs.Trrdg,                       SaSetup.tRRDG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRDR, MemConfig->ExternalInputs.Trrdr,                       SaSetup.tRRDR              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRRDD, MemConfig->ExternalInputs.Trrdd,                       SaSetup.tRRDD              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWRSG, MemConfig->ExternalInputs.Twrsg,                       SaSetup.tWRSG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWRDG, MemConfig->ExternalInputs.Twrdg,                       SaSetup.tWRDG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWRDR, MemConfig->ExternalInputs.Twrdr,                       SaSetup.tWRDR              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWRDD, MemConfig->ExternalInputs.Twrdd,                       SaSetup.tWRDD              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWWSG, MemConfig->ExternalInputs.Twwsg,                       SaSetup.tWWSG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWWDG, MemConfig->ExternalInputs.Twwdg,                       SaSetup.tWWDG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWWDR, MemConfig->ExternalInputs.Twwdr,                       SaSetup.tWWDR              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tWWDD, MemConfig->ExternalInputs.Twwdd,                       SaSetup.tWWDD              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRWSG, MemConfig->ExternalInputs.Trwsg,                       SaSetup.tRWSG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRWDG, MemConfig->ExternalInputs.Trwdg,                       SaSetup.tRWDG              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRWDR, MemConfig->ExternalInputs.Trwdr,                       SaSetup.tRWDR              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tRWDD, MemConfig->ExternalInputs.Trwdd,                       SaSetup.tRWDD              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Interpreter, MemConfig->ExternalInputs.Interpreter,           SaSetup.Interpreter        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.OCSafeMode, MemConfig->ExternalInputs.OCSafeMode,             SaSetup.OCSafeMode         );

    if (PcdGet32 (VpdPcdMrcRcompResistor) && (SaSetup.RcompResistor == 0)) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompResistor, MemConfig->ExternalInputs.RcompResistor, (UINT8) PcdGet32 (VpdPcdMrcRcompResistor));
    } else {
      //Use user input
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompResistor, MemConfig->ExternalInputs.RcompResistor, SaSetup.RcompResistor);
    }

    if (((VPD_RCOM_TARGET *)PcdGetPtr (VpdPcdMrcRcompTarget))->RcompTarget) {
      RcompTarget = (((VPD_RCOM_TARGET *)PcdGetPtr (VpdPcdMrcRcompTarget))->RcompTarget);
      for (Index = 0; Index < sizeof(SaSetup.RcompTarget)/sizeof(SaSetup.RcompTarget[0]); Index++) {
        if (SaSetup.RcompTarget[Index] != 0) {
          //Use user Input
          UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompTarget[Index], MemConfig->ExternalInputs.RcompTarget[Index], SaSetup.RcompTarget[Index]);
        } else {
          UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompTarget[Index], MemConfig->ExternalInputs.RcompTarget[Index], RcompTarget[Index]);
        }
      }
    } else {
      COPY_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RcompTarget, (VOID *)(UINTN) MemConfig->ExternalInputs.RcompTarget, (VOID *)SaSetup.RcompTarget, (UINTN) sizeof(SaSetup.RcompTarget));
    }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.tCCD_L_WR, MemConfig->ExternalInputs.tCCD_L_WR, SaSetup.tCCD_L_WR);

    for (Index = 0; Index < MRC_PPR_REQUEST_MAX; Index++) {
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_INFO *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryInfo))[Index].PprValid,      MemConfigNoCrc->PprEntryInfo[Index].PprValid,                      SaSetup.PprRequestEnable[Index]                                            );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Controller, MemConfigNoCrc->PprEntryAddress[Index].Controller,                 SaSetup.PprRequestController[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Channel,    MemConfigNoCrc->PprEntryAddress[Index].Channel,                    SaSetup.PprRequestChannel[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Rank,       MemConfigNoCrc->PprEntryAddress[Index].Rank,                       SaSetup.PprRequestRank[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].BankGroup,  MemConfigNoCrc->PprEntryAddress[Index].BankGroup,                  SaSetup.PprRequestBankGroup[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Bank,       MemConfigNoCrc->PprEntryAddress[Index].Bank,                       SaSetup.PprRequestBank[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Row,        MemConfigNoCrc->PprEntryAddress[Index].Row,                        SaSetup.PprRequestRow[Index]                                           );
      COMPARE_AND_UPDATE_POLICY (((MRC_PPR_ENTRY_ADDRESS *)(((FSPM_UPD *) FspmUpd)->FspmConfig.PprEntryAddress))[Index].Device,     MemConfigNoCrc->PprEntryAddress[Index].Device,                     SaSetup.PprRequestDevice[Index]                                           );
    }
#if FixedPcdGetBool (PcdMemoryTelemetryEnabled)
    if (UpdateAmtPprMrcPolicy (MemConfigNoCrc) != EFI_SUCCESS) {
#endif
      PprTestType.Value = 0;
      PprTestType.Bits.WcMats8       = SaSetup.PprRunWCHMATS8;
      PprTestType.Bits.DataRetention = SaSetup.PprRunRetention;
      PprTestType.Bits.XMarch        = SaSetup.PprRunXMarch;
      PprTestType.Bits.XMarchG       = SaSetup.PprRunXMarchG;
      PprTestType.Bits.YMarchShort   = SaSetup.PprRunYMarchShort;
      PprTestType.Bits.YMarchLong    = SaSetup.PprRunYMarchLong;
      PprTestType.Bits.Mmrw          = SaSetup.PprRunMmrw;
      PprTestType.Bits.TestDisabled  = (PprTestType.Value == 0) ? 1 : 0;

      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunWCHMATS8,    MemConfigNoCrc->PprTestType.Bits.WcMats8,       SaSetup.PprRunWCHMATS8                 );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunRetention,   MemConfigNoCrc->PprTestType.Bits.DataRetention, SaSetup.PprRunRetention                );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunXMarch,      MemConfigNoCrc->PprTestType.Bits.XMarch,        SaSetup.PprRunXMarch                   );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunXMarchG,     MemConfigNoCrc->PprTestType.Bits.XMarchG,       SaSetup.PprRunXMarchG                  );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunYMarchShort, MemConfigNoCrc->PprTestType.Bits.YMarchShort,   SaSetup.PprRunYMarchShort              );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunYMarchLong,  MemConfigNoCrc->PprTestType.Bits.YMarchLong,    SaSetup.PprRunYMarchLong               );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunMmrw,        MemConfigNoCrc->PprTestType.Bits.Mmrw,          SaSetup.PprRunMmrw                     );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprTestDisabled,   MemConfigNoCrc->PprTestType.Bits.TestDisabled,  (UINT8) PprTestType.Bits.TestDisabled  );
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRepairType,     MemConfigNoCrc->PprRepairType,                  SaSetup.PprRepairType                  );
#if FixedPcdGetBool (PcdMemoryTelemetryEnabled)
    }
#endif
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRetryLimit,     MemConfigNoCrc->PprRetryLimit,                  SaSetup.PprRetryLimit                  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprRunOnce,        MemConfigNoCrc->PprRunOnce,                     SaSetup.PprRunOnce                     );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprErrorInjection, MemConfigNoCrc->PprErrorInjection,              SaSetup.PprErrorInjection              );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PprForceRepair,    MemConfigNoCrc->PprForceRepair,                 SaSetup.PprForceRepair                 );

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Vdd2HVoltage,      MemConfig->ExternalInputs.Vdd2HVoltage,         SaSetup.Vdd2HVoltage                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Vdd1Voltage,       MemConfig->ExternalInputs.Vdd1Voltage,          SaSetup.Vdd1Voltage                    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Vdd2LVoltage,      MemConfig->ExternalInputs.Vdd2LVoltage,         SaSetup.Vdd2LVoltage                   );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.VddqVoltage,       MemConfig->ExternalInputs.VddqVoltage,          SaSetup.VddqVoltage                    );

    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BoardStackUp,   MemConfig->ExternalInputs.BoardDetails.BoardStackUp,   PcdGet8(VpdPcdMrcBoardStackUp)                         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.BoardTopology,  MemConfig->ExternalInputs.BoardDetails.BoardTopology,  PcdGet8(VpdPcdMrcBoardTopology)                        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SingleVdd2Rail, MemConfig->ExternalInputs.BoardDetails.SingleVdd2Rail, (PcdGet8(VpdPcdVdd2lPowerRailPresent) == 0) ? 1 : 0    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DprLock, MemConfig->ExternalInputs.DprLock,                     SetupData.TestLtDprLock         );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SubChHashMask, MemConfig->ExternalInputs.SubChHashMask,               SaSetup.SubChHashMask           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SubChHashInterleaveBit, MemConfig->ExternalInputs.SubChHashInterleaveBit,      SaSetup.SubChHashInterleaveBit  );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ForceCkdBypass, MemConfig->ExternalInputs.ForceCkdBypass,            SaSetup.ForceCkdBypass);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MimicWcDisaplayInIpq, MemConfig->ExternalInputs.MimicWcDisaplayInIpq,        SaSetup.MimicWcDisaplayInIpq    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.FakeSagv, MemConfig->ExternalInputs.FakeSagv,                    SaSetup.FakeSagv                );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableZq, MemConfig->ExternalInputs.DisableZq,                   SaSetup.DisableZq               );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ReplicateSagv, MemConfig->ExternalInputs.ReplicateSagv,               SaSetup.ReplicateSagv           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.AdjustWckMode, MemConfig->ExternalInputs.AdjustWckMode,               SaSetup.AdjustWckMode           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TelemetryControl, MemConfig->ExternalInputs.TelemetryControl,            SaSetup.TelemetryControl        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpineAndPhclkGateControl, MemConfig->ExternalInputs.SpineAndPhclkGateControl,    SaSetup.SpineAndPhclkGateControl);
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.SpineGatePerLpmode, MemConfig->ExternalInputs.SpineGatePerLpmode,          SaSetup.SpineGatePerLpmode      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PhclkGatePerLpmode, MemConfig->ExternalInputs.PhclkGatePerLpmode,          SaSetup.PhclkGatePerLpmode      );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.DisableSwitchDfiToMc, MemConfig->ExternalInputs.DisableSwitchDfiToMc,        SaSetup.DisableSwitchDfiToMc    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RowPressEn, MemConfig->ExternalInputs.RowPressEn,                 SaSetup.RowPressEn                           );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.WeaklockEn,     MemConfig->ExternalInputs.WeaklockEn,              SaSetup.WeaklockEn             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.RxDqsDelayCompEn,     MemConfig->ExternalInputs.RxDqsDelayCompEn,              SaSetup.RxDqsDelayCompEn             );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MrcFailureOnUnsupportedDimm, MemConfig->ExternalInputs.MrcFailureOnUnsupportedDimm, SaSetup.MrcFailureOnUnsupportedDimm );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Use1p5ReadPostamble, MemConfig->ExternalInputs.Use1p5ReadPostamble, SaSetup.Use1p5ReadPostamble );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.Lp5SafeSpeed,        MemConfig->ExternalInputs.Lp5SafeSpeed,        SaSetup.Lp5SafeSpeed        );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.ForceInternalClkOn,  MemConfig->ExternalInputs.ForceInternalClkOn,  SaSetup.ForceInternalClkOn  );


    DEBUG_CODE (
      DEBUG_CONFIG_DATA DebugConfigData;

      VariableSize = sizeof (DEBUG_CONFIG_DATA);
      Status = VariableServices->GetVariable (
                                   VariableServices,
                                   L"DebugConfigData",
                                   &gDebugConfigVariableGuid,
                                   NULL,
                                   &VariableSize,
                                   &DebugConfigData
                                   );
      ASSERT_EFI_ERROR (Status);
      COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.SerialDebugMrcLevel, MemConfigNoCrc->SerialDebugLevel, DebugConfigData.SerialDebugMrcLevel);
    );

    //
    // Update MemTestOnWarmBoot variable. Default is run BaseMemoryTest on warm boot.
    //
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *)FspmUpd)->FspmConfig.MemTestOnWarmBoot, MemConfigNoCrc->MemTestOnWarmBoot, SaSetup.MemTestOnWarmBoot);

    //
    // Update CleanMemory variable from Memory overwrite request value. Ignore if we are performing capsule update.
    //
    if ((BootMode != BOOT_ON_FLASH_UPDATE) && (BootMode != BOOT_ON_S3_RESUME)) {
      UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.CleanMemory, MemConfigNoCrc->CleanMemory, (BOOLEAN)(MorControl & MOR_CLEAR_MEMORY_BIT_MASK));
    }

    //
    // Based on BIOS setup to determine maximum top of memory size below 4G, and reserved for MMIO
    //
    switch (SaSetup.MaxTolud) {
      case MAX_TOLUD_DYNAMIC:
        MmioSize = 0x0;
        break;
      case MAX_TOLUD_1G:
        MmioSize = 0xC00;
        break;
      case MAX_TOLUD_1_25G:
        MmioSize = 0xB00;
        break;
      case MAX_TOLUD_1_5G:
        MmioSize = 0xA00;
        break;
      case MAX_TOLUD_1_75G:
        MmioSize = 0x900;
        break;
      case MAX_TOLUD_2G:
        MmioSize = 0x800;
        break;
      default:
        MmioSize = 0x0;
        break;
    }
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.MmioSize, HostBridgePreMemConfig->MmioSize, MmioSize);
  }

  if (BootMode == BOOT_ON_FLASH_UPDATE) {
    //
    // Reserve 64M for Capsule update/Recovery usage in PEI.
    // To avoid the page allocation failure.
    //
    CapsuleSupportMemSize = SIZE_64MB;
  }

  //Read RMT OS variable
  DataSize = sizeof (RMT_VAR);
  Status4 = VariableServices->GetVariable (
                               VariableServices,
                               EFI_RMT_OS_VARIABLE_NAME,
                               &gRmtVariableGuid,
                               NULL,
                               &DataSize,
                               &mRmtdata
                               );
  if (!EFI_ERROR (Status4)) {
    if(mRmtdata.EnDsRmt == RMT_ENABLE) {
      #if FixedPcdGet8(PcdFspModeSelection) == 1
        ((FSPM_UPD *) FspmUpd)->FspmConfig.RMT           = mRmtdata.EnDsRmt;
        ((FSPM_UPD *) FspmUpd)->FspmConfig.MrcFastBoot   = 0;
        ((FSPM_UPD *) FspmUpd)->FspmConfig.MrcBdatEnable = mRmtdata.EnDsRmt;
      #else
        MemConfig->ExternalInputs.TrainingEnables.RMT = mRmtdata.EnDsRmt;
        MemConfig->ExternalInputs.MrcFastBoot         = 0;
        MemConfigNoCrc->MrcBdatEnable                 = mRmtdata.EnDsRmt;
      #endif
     }
  } else {
    DEBUG ((DEBUG_ERROR, "RMT OS variable not found  = %r \n", Status4));
  }

  DataSize = sizeof (MemoryData);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               EFI_MEMORY_TYPE_INFORMATION_VARIABLE_NAME,
                               &gEfiMemoryTypeInformationGuid,
                               NULL,
                               &DataSize,
                               &MemoryData
                               );
  ///
  /// Accumulate maximum amount of memory needed
  ///
  PlatformMemorySize = MemConfigNoCrc->PlatformMemorySize;

  AsmCpuidEx (CPUID_STRUCTURED_EXTENDED_FEATURE_FLAGS, 0, NULL, &Ebx.Uint32, NULL, NULL);
  if ((Ebx.Bits.IntelProcessorTrace == 1) && (CpuSetup.ProcessorTraceEnable == 1) && (CpuSetup.ProcessorTraceMemSize <= RtitTopaMemorySize128M)) {
    ///
    /// Get the total number of enabled processor threads.
    /// Read MSR 0x35 to get the max enabled number of cores/threads.
    ///
    MsrCoreThreadCount.Uint64 = AsmReadMsr64 (MSR_CORE_THREAD_COUNT);

    //
    // ProcessorTraceTotalMemSize is mutiplied by 2 for adding aligned memory (overhead) for natural alignment.
    // please refer to InternalAllocateAlignedPages (), the overhead will be free when creating memory map, it won't waste
    //
    ProcessorTraceTotalMemSize = 2 * (1 << (CpuSetup.ProcessorTraceMemSize + 12)) * (UINT32) MsrCoreThreadCount.Bits.Threadcount;
    }
  if (EFI_ERROR (Status)) {
    if (BootMode == BOOT_IN_RECOVERY_MODE) {
      PlatformMemorySize = PEI_RECOVERY_MIN_MEMORY_SIZE;
    } else {
      ///
      /// Use default value to avoid memory fragment. Plus tracehub memory if required.
      /// OS boot/installation fails if there is not enough continuous memory available
      ///
      PlatformMemorySize = PEI_MIN_MEMORY_SIZE + ProcessorTraceTotalMemSize;
      DataSize = sizeof (mDefaultMemoryTypeInformation);
      CopyMem (MemoryData, mDefaultMemoryTypeInformation, DataSize);
    }
  } else {
    ///
    /// Start with at least PEI_MIN_MEMORY_SIZE of memory for the DXE Core and the DXE Stack
    ///
    PlatformMemorySize = PEI_MIN_MEMORY_SIZE + ProcessorTraceTotalMemSize;
  }

  //
  // PEI memory support for capsule update.
  //
  PlatformMemorySize += CapsuleSupportMemSize;

  UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PlatformMemorySize, MemConfigNoCrc->PlatformMemorySize, PlatformMemorySize);

  if (BootMode != BOOT_IN_RECOVERY_MODE) {
    for (Index = 0; Index < DataSize / sizeof (EFI_MEMORY_TYPE_INFORMATION); Index++) {
      PlatformMemorySize += MemoryData[Index].NumberOfPages * EFI_PAGE_SIZE;
    }
    UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.PlatformMemorySize, MemConfigNoCrc->PlatformMemorySize, PlatformMemorySize);

    ///
    /// Build the GUID'd HOB for DXE
    ///
    BuildGuidDataHob (
      &gEfiMemoryTypeInformationGuid,
      MemoryData,
      DataSize
      );

#if FixedPcdGetBool (PcdTcssSupport) == 1
    ///
    /// TCSS DEVEN bits from setup to policy
    ///
    DEBUG ((DEBUG_INFO, "[TCSS] UpdatePeiSaPolicyPreMem Filling TCSS iTBT Policy Start\n"));
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssXhciEn, TcssPeiPreMemConfig->TcssEnable, PcdGet8 (VpdPcdTcssEnable));
    for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
      if (TcssPortCap[Index] != NoThunderbolt && TcssPortCap[Index] != UsbCDisable) {
        TcssPortConf[Index] = SaSetup.TcssPortConf[Index];
      } else {
        //
        // The TCSS port doesn't support TBT. It can only be NoThunderbolt or UsbCDisable
        //
        if (TcssPortCap[Index] == UsbCDisable) {
          TcssPortConf[Index] = UsbCDisable;
        } else {
          TcssPortConf[Index] = (SaSetup.TcssPortConf[Index] != UsbCDisable) ? NoThunderbolt : UsbCDisable;
        }
      }
      //
      // Update TCSS Port Config
      //
    }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort0,
      TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[0],
      (SaSetup.TcssXhciEn == 0) ? UsbCDisable : TcssPortConf[0]
    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort1,
      TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[1],
      (SaSetup.TcssXhciEn == 0) ? UsbCDisable : TcssPortConf[1]
    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort2,
      TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[2],
      (SaSetup.TcssXhciEn == 0) ? UsbCDisable : TcssPortConf[2]
    );
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPort3,
      TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[3],
      (SaSetup.TcssXhciEn == 0) ? UsbCDisable : TcssPortConf[3]
    );

    //
    // Update Tcss Platform configuration (retimer)
    //
    UsbConnectorHobDataPtr = GetUsbConnectorHobData ();
    UsbCConnectorHobDataPtr = GetUsbCConnectorHobData ();
    if (UsbConnectorHobDataPtr == NULL || UsbCConnectorHobDataPtr == NULL) {
      DEBUG((DEBUG_ERROR, "%a: UsbConnectorHobDataPtr or UsbCConnectorHobDataPtr is not available!!\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }

    UsbConnectorBoardConfig = UsbConnectorHobDataPtr->UsbConnectorBoardConfig;
    if (UsbConnectorBoardConfig == NULL) {
      DEBUG ((DEBUG_ERROR, "%a: UsbConnectorBoardConfig is not available!!\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }

    UsbCConnectorBoardConfig = UsbCConnectorHobDataPtr->UsbCConnectorBoardConfig;
    if (UsbCConnectorBoardConfig == NULL) {
      DEBUG ((DEBUG_ERROR, "%a: UsbCConnectorBoardConfig is not available!!\n", __FUNCTION__));
      return EFI_NOT_FOUND;
    }

    for (Index = 0; Index < UsbConnectorHobDataPtr->NumberOfUsbConnectors; Index++) {
      if (UsbConnectorBoardConfig[Index].ConnectorConnectable == CONNECTABLE &&
          UsbConnectorBoardConfig[Index].Usb3Controller == TCSS_USB3 &&
          UsbConnectorBoardConfig[Index].Usb3PortNum < MAX_TCSS_USB3_PORTS &&
          UsbCConnectorBoardConfig[Index].RetimerCount > 0) {
        TcssPlatConfig |= 0x01 << (UsbConnectorBoardConfig[Index].Usb3PortNum * 2);
      }
    }
    COMPARE_AND_UPDATE_POLICY (((FSPM_UPD *) FspmUpd)->FspmConfig.TcssPlatConf,
      TcssPeiPreMemConfig->TcssPlatConf,
      TcssPlatConfig
    );
    DEBUG ((DEBUG_INFO, "[TCSS] UpdatePeiSaPolicyPreMem Filling TCSS iTBT Policy End\n"));
#endif
  }

  return EFI_SUCCESS;
}
