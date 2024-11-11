/** @file
  This file Initialize resources for Telemetry

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

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/BaseLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/IoLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>
#include <Register/TelemetryRegs.h>
#include <Library/TelemetryLib.h>
#include <Library/PeiTelemetryFruLib.h>
#include <Library/TelemetryPrivateLib.h>
#include <Library/PeiTelemetryInitLib.h>
#include <Library/PeiImrInitLib.h>
#include <Library/MemorySubSystemInfoLib.h>

/**
  Align Streamtracer size to 1MB

  @param[in] Size Streamtracer size

  @retval 1MB bounded size
**/
UINT32
EFIAPI
StreamTracer1MbAlign (
  UINT32 StreamTracerSize
  )
{
  //
  // StreamTracer size adjusted to MB boundary, Memory requirement
  //
  if ((StreamTracerSize & (SIZE_1MB - 1)) != 0x0) {
    StreamTracerSize = (StreamTracerSize & ((UINT64)~(SIZE_1MB - 1))) + SIZE_1MB;
  }

  return StreamTracerSize;
}

/**
  Reserve Memory for Telemetry

  @param[in]  TopUseableMemAddr         Top usable memory address
  @param[in]  TopMemSize                Top of memory size
  @param[in]  MemoryMapData             Memory Map Info
  @param[out] StreamTracerMemSize       StreamTracerMemSize in MB
  @param[out] StreamTracerBaseAddr      StreamTracerBaseAddr in MB
**/
VOID
PeiTelemetryMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud,
  IN OUT EFI_RESOURCE_ATTRIBUTE_TYPE  ResourceAttributeTested,
  OUT UINT32                          *StreamTracerMemSize,
  OUT UINT64                          *StreamTracerBaseAddr
  )
{

  EFI_STATUS                            Status;
  UINT32                                StreamTracerSize;
  UINT32                                lStreamTracerMemSize;
  UINT64                                lStreamTracerBaseAddr;
  UINT64                                TempStreamTracerBaseAddr;
  EFI_RESOURCE_TYPE                     ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE           ResourceAttribute;
  TELEMETRY_PEI_PREMEM_CONFIG           *TelemetryPreMemConfig;
  SI_PREMEM_POLICY_PPI                  *SiPreMemPolicyPpi;

  TelemetryPreMemConfig = NULL;

  Status = PeiServicesLocatePpi (&gSiPreMemPolicyPpiGuid, 0, NULL, (VOID **) &SiPreMemPolicyPpi);
  ASSERT_EFI_ERROR (Status);

  if ((Status != EFI_SUCCESS) || (SiPreMemPolicyPpi == NULL)) {
    return;
  }

  if (IsTelemetrySupported () && (SiPreMemPolicyPpi != NULL)) {

    Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTelemetryPeiPreMemConfigGuid, (VOID *) &TelemetryPreMemConfig);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "Unable to Get gTelemetryPeiPreMemConfigGuid block\n"));
      TelemetryPreMemConfig = NULL;
      ASSERT_EFI_ERROR (Status);
    }

    if ((TelemetryPreMemConfig != NULL) && (TelemetryPreMemConfig->StreamTracerMode != StreamTracerDisabled)) {
      lStreamTracerBaseAddr = (UINT64)*TopUseableMemAddr;
      StreamTracerSize = GetStreamTracerImrSize ();
      //
      // The size allocated needs to be bounded to MB , Memory requirement
      //
      lStreamTracerMemSize = StreamTracer1MbAlign (StreamTracerSize);
      TempStreamTracerBaseAddr = lStreamTracerBaseAddr;
      lStreamTracerBaseAddr = NaturalAlignment (lStreamTracerBaseAddr, lStreamTracerMemSize);

      if (lStreamTracerBaseAddr > TempStreamTracerBaseAddr) {
        ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                            EFI_RESOURCE_ATTRIBUTE_TESTED |
                            EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE;
        DEBUG ((DEBUG_INFO, "After Streamtracer alignement changes, Change in size = 0x%lx\n",
                                                  (lStreamTracerBaseAddr - TempStreamTracerBaseAddr)));
        BuildResourceDescriptorHob (
          ResourceType,                                        // MemoryType,
          ResourceAttribute,                                   // MemoryAttribute
          TempStreamTracerBaseAddr,                            // MemoryBegin
          (lStreamTracerBaseAddr - TempStreamTracerBaseAddr)   // MemoryLength
        );
        // Decrement the TopMemSize if there is change in address
        *Touud -= (lStreamTracerBaseAddr - TempStreamTracerBaseAddr);
        *TopUseableMemAddr = lStreamTracerBaseAddr;
      }

      DEBUG ((DEBUG_INFO, "StreamTracer memory base = 0x%lx Size = 0x%lx\n",
                                                      lStreamTracerBaseAddr, lStreamTracerMemSize));

      //
      // Program StreamTracer IMR.
      //
      Status = SetImr (IMR_STREAM_TRACER, lStreamTracerBaseAddr, StreamTracerSize);
      if (Status == EFI_SUCCESS) {
        ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                            ResourceAttributeTested |
                            EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                            EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;

        BuildResourceDescriptorHob (
          ResourceType,                   // MemoryType,
          ResourceAttribute,              // MemoryAttribute
          lStreamTracerBaseAddr,          // MemoryBegin
          lStreamTracerMemSize            // MemoryLength
        );

        BuildMemoryAllocationHob (
          lStreamTracerBaseAddr,
          lStreamTracerMemSize,
          EfiReservedMemoryType
        );

        *TopUseableMemAddr = lStreamTracerBaseAddr + lStreamTracerMemSize;
        *Touud -= lStreamTracerMemSize;
        *StreamTracerMemSize  = (UINT32) RShiftU64 (lStreamTracerMemSize, 20);
        *StreamTracerBaseAddr = RShiftU64 (lStreamTracerBaseAddr, 20);
        TelemetryPreMemConfig->StreamTracerBase = lStreamTracerBaseAddr;
      } else {
        DEBUG ((DEBUG_INFO, "Failed to enable StreamTracer\n"));
        TelemetryPreMemConfig->StreamTracerMode = StreamTracerDisabled;
        ASSERT_EFI_ERROR (Status);
      }
    }
  }
}
