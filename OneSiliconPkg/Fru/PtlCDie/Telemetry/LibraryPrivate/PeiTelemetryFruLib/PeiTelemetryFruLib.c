/** @file
  This code provides an instance of PeiTelemetryFruLib.

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
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <CpuRegs.h>
#include <Library/CpuPlatformLib.h>
#include <Register/TelemetryRegs.h>
#include <Library/PciSegmentLib.h>
#include <Ppi/SiPolicy.h>
#include <PiPei.h>
#include <Library/PeiServicesLib.h>
#include <Register/HostBridgeRegs.h>
#include <TelemetryPeiConfig.h>
#include <IndustryStandard/Pci22.h>
#include <Library/PeiHostBridgeIpStatusLib.h>

typedef union {
  struct {
    UINT32  Low;
    UINT32  High;
  } Data32;
  UINT64 Data;
} UINT64_STRUCT;

/**
  This function will check if Intel Telemetry is Enabled and supported

  @retval TRUE          Telemetry Enabled and supported
  @retval FALSE         Telemetry not Enabled or not Supported
**/
BOOLEAN
EFIAPI
IsTelemetrySupported (
  VOID
)
{
  UINT64                      TelemetryBaseAddress;
  BOOLEAN                     TelemetryEnable;

  TelemetryBaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  TelemetryEnable      = UncoreDevEnRead (Telemetry, 0);

  if (PciSegmentRead16 (TelemetryBaseAddress + PCI_VENDOR_ID_OFFSET) == 0xFFFF) {
    DEBUG ((DEBUG_WARN, "Telemetry is unsupported or disabled\n"));
    return FALSE;
  }
  if (!TelemetryEnable) {
    DEBUG ((DEBUG_WARN, "Telemetry is disabled from DevEn\n"));
    return FALSE;
  }
  DEBUG ((DEBUG_INFO, "Telemetry is enabled and supported\n"));
  return TRUE;
}

/**
  Get PmBar base address

  @return The PMBar Base Address
**/
UINT64
EFIAPI
GetPmBarBaseAddr (
  VOID
  )
{
  UINT64         DeviceBaseAddr;
  UINT64_STRUCT  PmBarAdd;

  DeviceBaseAddr = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  //
  // Set MSE
  //
  PciSegmentOr16 (DeviceBaseAddr + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);

  PmBarAdd.Data32.Low = PciSegmentRead32 (DeviceBaseAddr + R_TEL_CFG_BAR0);
  PmBarAdd.Data32.High = PciSegmentRead32 (DeviceBaseAddr + R_TEL_CFG_BAR0 + 4);
  PmBarAdd.Data = PmBarAdd.Data & 0xFFFFFFFFFFFFFFF0ULL;
  if (PmBarAdd.Data == 0x0) {
    PciSegmentWrite32 ((DeviceBaseAddr + R_TEL_CFG_BAR0),  PcdGet32 (PcdSiliconInitTempMemBaseAddr));
    PmBarAdd.Data32.Low = PciSegmentRead32 (DeviceBaseAddr + R_TEL_CFG_BAR0) & 0xFFFFFFF0;
  }

  return PmBarAdd.Data;
}

/**
  This function will clear MEMORY SPACE ENABLE bit
  MSE need to cleared for telemtry before exiting

**/
VOID
EFIAPI
ClearTelemetryMSE (
  VOID
  )
{
  UINT64         DeviceBaseAddr;

  DeviceBaseAddr = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, TEL_BUS_NUM, TEL_DEV_NUM, TEL_FUN_NUM, 0);
  PciSegmentAnd16 (DeviceBaseAddr + PCI_COMMAND_OFFSET, (UINT16) ~(EFI_PCI_COMMAND_MEMORY_SPACE));
  return;
}

/**
  This function will program Stream Tracer memory buffers
  @param[in]  SiPreMemPolicy      The Silicon Pre Mem Policy PPI instance

**/
VOID
EFIAPI
SetStreamTracerMemoryBuffer (
  IN    SI_PREMEM_POLICY_PPI        *SiPreMemPolicy
  )
{
  UINTN                        PmBarBaseAddr;
  TELEMETRY_PEI_PREMEM_CONFIG  *TelemetryPreMemConfig;
  EFI_STATUS                   Status;
  UINT32                       MemoryBufferSize;
  STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_STRUCT StreamTracerBaseStruct;
  STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_STRUCT StreamTracerSizeStruct;

  TelemetryPreMemConfig = NULL;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  if (Status != EFI_SUCCESS) {
    ASSERT_EFI_ERROR (Status);
  }
  if (TelemetryPreMemConfig != NULL) {
    if (TelemetryPreMemConfig->StreamTracerMode == StreamTracerDisabled) {
      DEBUG ((DEBUG_INFO, "StreamTracer disabled\n"));
      return;
    }
    PmBarBaseAddr = (UINTN) GetPmBarBaseAddr ();
    //
    // Align MemoryBufferSize to DWORD
    //
    MemoryBufferSize = (TelemetryPreMemConfig->StreamTracerSize << 10) / 4;
    MmioWrite64 (PmBarBaseAddr + STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_REG, TelemetryPreMemConfig->StreamTracerBase);
    MmioWrite32 (PmBarBaseAddr + STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_REG, MemoryBufferSize);
    StreamTracerBaseStruct.Data = MmioRead64 (PmBarBaseAddr + STREAM_TRACER_MEM_BUFFER_BASE_PMBAR_REG);
    StreamTracerSizeStruct.Data = MmioRead32 (PmBarBaseAddr + STREAM_TRACER_MEM_BUFFER_SIZE_PMBAR_REG);
    DEBUG ((DEBUG_INFO, "StreamTracer Base = %016lx\n", StreamTracerBaseStruct.Bits.base_addr));
    DEBUG ((DEBUG_INFO, "StreamTracer Size in DWORDs = %lx\n", StreamTracerSizeStruct.Bits.mem_buffer_size));
    ClearTelemetryMSE ();
  }
}

/**
  Calculate Streamtracer size required for IMR

  @retval Size Streamtracer IMR size
**/
UINT32
EFIAPI
GetStreamTracerImrSize (
  VOID
  )
{
  EFI_STATUS     Status;
  UINT32         StreamTracerSize;

  TELEMETRY_PEI_PREMEM_CONFIG           *TelemetryPreMemConfig;
  SI_PREMEM_POLICY_PPI                  *SiPreMemPolicyPpi;

  TelemetryPreMemConfig = NULL;
  StreamTracerSize = 0;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);
  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    return 0;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Unable to Get gTelemetryPeiPreMemConfigGuid block\n"));
    ASSERT_EFI_ERROR (Status);
    return 0;
  }

  if ((TelemetryPreMemConfig != NULL) && (TelemetryPreMemConfig->StreamTracerMode != StreamTracerDisabled) && IsTelemetrySupported ()) {
    StreamTracerSize = TelemetryPreMemConfig->StreamTracerSize << 10;
  }
  DEBUG ((DEBUG_INFO, "StreamTracer Size = 0x%lx\n",StreamTracerSize));

  return StreamTracerSize;
}
