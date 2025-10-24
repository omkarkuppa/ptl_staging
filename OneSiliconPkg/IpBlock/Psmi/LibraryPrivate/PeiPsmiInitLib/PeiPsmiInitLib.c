/** @file
  This code provides an instance of Pei PSMI Init Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Ppi/SiPolicy.h>
#include <Library/PsmiInitLib.h>
#include <Library/MtrrLib.h>
#include <MemInfoHob.h>
#include <Library/HobLib.h>
#include <Library/BaseMemoryLib.h>
#include <PsmiConfig.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci.h>
#include <IndustryStandard/Pci22.h>
#include <Base.h>
#include <PsmiDataHob.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiPsmiInitFruLib.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Library/MemorySubSystemInfoLib.h>

//
// Size Definitions for PSMI handler and Trace Region
//
UINT32  PsmiHandlerSizeTab[4] = {0, SIZE_256KB, SIZE_512KB, SIZE_1MB};
UINT64  TraceSizeTab[16] = {0, SIZE_1MB, SIZE_2MB, SIZE_4MB, SIZE_8MB, SIZE_16MB, SIZE_32MB, SIZE_64MB, SIZE_128MB, SIZE_256MB, SIZE_512MB, SIZE_1GB, SIZE_2GB, SIZE_4GB, SIZE_8GB, SIZE_16GB};

/**
  Initializes PSMI Handler and Trace memory size

  @retval EFI_SUCCESS           PsmiInit Function executed successfully
  @retval EFI_UNSUPPORTED       PsmiInit failed to execute
**/
EFI_STATUS
PsmiInit (
  IN    SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi
  )
{
  EFI_STATUS                 Status;
  PSMI_INPUT_SCRPD1_REGISTER Scrpd1;
  PSMI_DATA_HOB_VER1         *PsmiDataHob;
  UINT8                      Index;
  UINT8                      TraceRegion;
  UINT64                     TraceRegionSize;
  UINT8                      TraceRegionCacheType;
  UINT64                     TotalTraceRegionDontCare = 0;
  UINT64                     TotalTraceRegionWb = 0;

  PsmiDataHob = NULL;
  DEBUG ((DEBUG_INFO, "PsmiInit () - Start\n"));
  //
  // Read SCRPD1 register to get PSMI handler, trace memory sizes , Trace Region Cache Type
  //
  Status = ReadTraceHubScrpd1 (&Scrpd1.RegValue);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "TraceHub device does not exist, return.\n"));
    return Status;
  }
  DEBUG ((DEBUG_INFO, "PSMI SCRPD1 = 0x%x\n", Scrpd1.RegValue));
  //
  // Install PSMI Data Hob
  //
  DEBUG ((DEBUG_INFO, "Install PSMI Data HOB\n"));
  InstallPsmiHobVer1 (SiPreMemPolicyPpi, &PsmiDataHob);
  //
  // Locate the PSMI Data hob, if not found , return
  //
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *) GetFirstGuidHob (&gPsmiDataHobGuid);
  if (PsmiDataHob == NULL) {
    return EFI_UNSUPPORTED;
  }
  //
  // Use Scratchpad only when Psmihandler size is greater than zero
  //
  if ((Scrpd1.Bits.PsmiHandler != 0) && (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize == 0)) {
    PsmiDataHob->PsmiConfig.PsmiInputRegValue = Scrpd1.RegValue;
    /*
    //
    // SCPRD1 bit field description for decoding handler, trace region size and cacheability
    //
    Bit range     Field Name                  Description

    01:00     PSMI_HANDLER_SIZE                 Size of the PSMI Handler;
    05:02     TRACE_REGION_0_SIZE               Size of tracing region 0;
       06     TRACE_REGION_0_MEMTYPE            Cacheability of tracing region 0;
       07     TRACE_REGION_0_RANGE_COMPATIBLE   RangeCompatible for tracing region 0;
    11:08     TRACE_REGION_1_SIZE               Size of tracing region 1;
       12     TRACE_REGION_1_MEMTYPE            Cacheability of tracing region 1;
       13     TRACE_REGION_1_RANGE_COMPATIBLE   RangeCompatible for tracing region 1;
    17:14     TRACE_REGION_2_SIZE               Size of tracing region 2;
       18     TRACE_REGION_2_MEMTYPE            Cacheability of tracing region 2;
       19     TRACE_REGION_2_RANGE_COMPATIBLE   RangeCompatible for tracing region 2;
    23:20     TRACE_REGION_3_SIZE               Size of tracing region 3;
       24     TRACE_REGION_3_MEMTYPE            Cacheability of tracing region 3;
       25     TRACE_REGION_3_RANGE_COMPATIBLE   RangeCompatible for tracing region 3;
    29:26     TRACE_REGION_4_SIZE               Size of tracing region 4;
       30     TRACE_REGION_4_MEMTYPE            Cacheability of tracing region 4;
       31     TRACE_REGION_4_RANGE_COMPATIBLE   RangeCompatible for tracing region 4;
    */
    PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize             = (UINT8) Scrpd1.Bits.PsmiHandler;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[0].CacheType        = (UINT8) Scrpd1.Bits.TraceRegion0CacheType;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[0].RangeCompatible  = (UINT8) Scrpd1.Bits.TraceRegion0RangeCompatible;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[0].Size             = Scrpd1.Bits.TraceRegion0Size;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[1].CacheType        = (UINT8) Scrpd1.Bits.TraceRegion1CacheType;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[1].RangeCompatible  = (UINT8) Scrpd1.Bits.TraceRegion1RangeCompatible;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[1].Size             = Scrpd1.Bits.TraceRegion1Size;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[2].CacheType        = (UINT8) Scrpd1.Bits.TraceRegion2CacheType;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[2].RangeCompatible  = (UINT8) Scrpd1.Bits.TraceRegion2RangeCompatible;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[2].Size             = Scrpd1.Bits.TraceRegion2Size;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[3].CacheType        = (UINT8) Scrpd1.Bits.TraceRegion3CacheType;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[3].RangeCompatible  = (UINT8) Scrpd1.Bits.TraceRegion3RangeCompatible;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[3].Size             = Scrpd1.Bits.TraceRegion3Size;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[4].CacheType        = (UINT8) Scrpd1.Bits.TraceRegion4CacheType;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[4].RangeCompatible  = (UINT8) Scrpd1.Bits.TraceRegion4RangeCompatible;
    PsmiDataHob->PsmiConfig.TraceRegionInfo[4].Size             = Scrpd1.Bits.TraceRegion4Size;
  }
  //
  // No PSMI reservation needed , return
  //
  if (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize == 0) {
    DEBUG ((DEBUG_INFO, "No PSMI Reservation Request \nPsmiInit () - End \n"));
    return EFI_SUCCESS;
  }

  if (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize >= ARRAY_SIZE (PsmiHandlerSizeTab)){
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "PSMI Handler size 0x%x \n", PsmiHandlerSizeTab[PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize]));

  for (Index = 0; Index < MAX_TRACE_REGION; Index++ ) {
    TraceRegion = (UINT8) PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].Size;
    //
    // Trace Region should be allocated only for TraceRegion(N) > 0
    //
    if (TraceRegion == 0 || TraceRegion >= ARRAY_SIZE (TraceSizeTab)) {
      continue;
    }
    TraceRegionSize = TraceSizeTab[TraceRegion];
    TraceRegionCacheType = (UINT8) PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].CacheType;
    DEBUG ((DEBUG_INFO, "PSMI Trace %d Size = 0x%lx Cache = 0x%x \n", Index, TraceRegionSize, TraceRegionCacheType));

    PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].CacheType = TraceRegionCacheType;
    PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize = TraceRegionSize;

    DEBUG ((DEBUG_INFO, "PSMI  Region = %d TotalSize = 0x%lx CacheType = 0x%x\n",Index,PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize,PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].CacheType));

    switch( TraceRegionCacheType ) {
      case RegionDonotCare:
        TotalTraceRegionDontCare += TraceRegionSize;
        break;
      case RegionWriteBack:
        TotalTraceRegionWb += TraceRegionSize;
        break;
    }
  }

  //
  // Fill the PSMI MRC Info block to which will be later consumed MRC
  //
  PsmiDataHob->PsmiConfig.PsmiMrcParam[0].CacheType = RegionDonotCare;
  PsmiDataHob->PsmiConfig.PsmiMrcParam[0].TotalSize = TotalTraceRegionDontCare;
  PsmiDataHob->PsmiConfig.PsmiMrcParam[1].CacheType = RegionWriteBack;
  PsmiDataHob->PsmiConfig.PsmiMrcParam[1].TotalSize = TotalTraceRegionWb;

  DEBUG ((DEBUG_INFO, "PSMI Total Dont care Size requested 0x%lx \n", PsmiDataHob->PsmiConfig.PsmiMrcParam[0].TotalSize));
  DEBUG ((DEBUG_INFO, "PSMI Total WriteBack Size requested 0x%lx \n", PsmiDataHob->PsmiConfig.PsmiMrcParam[1].TotalSize));

  DEBUG ((DEBUG_INFO, "PsmiInit () - End\n"));

  return EFI_SUCCESS;
}

/**
  Configures PSMI Handler/Trace Memory

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmi (
  VOID
)
{
  PSMI_DATA_HOB_VER1             *PsmiDataHob;
  PSMI_OUTPUT_SCRPD1_REGISTER    Scrpd1;
  PSMI_HANDLER_SPACE_OFFSET      PsmiHandlerSpace;
  MEMORY_PLATFORM_DATA_HOB       *MemInfoHob;
  UINT8                          Index;
  UINT64                         Size;
  UINT64                         BaseAddress;
  UINT64                         Remainder;

  PsmiDataHob    = NULL;

  DEBUG ((DEBUG_INFO, "ConfigurePsmi () - Start\n"));
  //
  // Locate the PSMI Data hob, if not found , return
  //
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *)GetFirstGuidHob(&gPsmiDataHobGuid);
  if (PsmiDataHob == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize == 0) {
    DEBUG ((DEBUG_INFO, "ConfigurePsmi () - PSMI disabled \n"));
    return EFI_SUCCESS;
  }

  MemInfoHob = (MEMORY_PLATFORM_DATA_HOB *) GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  if (MemInfoHob == NULL) {
    DEBUG ((DEBUG_INFO, "MemoryPlatformData hob not found, returning from PSMI Configuration \n"));
    return EFI_UNSUPPORTED;
  }

  for (Index = 0; Index < MAX_TRACE_REGION; Index++) {
    //
    // Pass the MRC Hob Info structure for Size not equal to zero
    //
    if (PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize == 0) {
      DEBUG ((DEBUG_INFO, "PSMI Trace Region = %d not requested\n", Index));
      continue;
    }
    Size        = PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize;
    BaseAddress = MemInfoHob->Data.PsmiRegionInfo[Index].BaseAddress;
    DEBUG ((DEBUG_INFO, "PSMI Region = %d BaseAddress = 0x%lx Size = 0x%lx \n",Index, BaseAddress, Size));

    //
    // check for RangeCompatible if it's enabled else update the error
    //
    if (PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].RangeCompatible == ENABLED) {
      DivU64x64Remainder (BaseAddress, Size, &Remainder);
      if (Remainder != 0) {
        DEBUG ((DEBUG_INFO,"Doesnot statisfy range compatibility\n"));
        PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode = ERROR_OUT_OF_RESOURCES;
        ASSERT(FALSE);
        continue;
      }
    }

    //
    // Update the trace region if BaseAddress is valid else copy the error code
    //
    if (MemInfoHob->Data.PsmiRegionInfo[Index].BaseAddress != 0) {
      PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].TraceRegionBase = BaseAddress;
      //
      // Update the ErrCode to SUCCESS
      //
      PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode = PSMI_ALLOCATION_SUCCESS;
      DEBUG ((DEBUG_INFO, "PSMI Region = %d BaseAddress = 0x%lx Size = 0x%lx ErrCode = 0x%x\n",Index, BaseAddress, TraceSizeTab[PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].Size],PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode));
    } else {
      PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode = MemInfoHob->Data.PsmiRegionInfo[Index].ErrorCode;
      //
      // if the ErrorCode is SUCESS then change that to Out of Resources so that when address is 0 ErrorCode from MemInfoHob Success so to avoid Override the ErrorCode to Out of Resources
      //
      if ((PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode == PSMI_ALLOCATION_SUCCESS || (PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode == 0))) {
        PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode = ERROR_OUT_OF_RESOURCES;
      }
      DEBUG ((DEBUG_INFO, "Error Trace Region %d Error 0x%x \n",Index, PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].ErrCode ));
    }
  }

  //
  // Update PSMI SCPRD1 register with PSMI outputs info, zero the register first to override before MRC value
  //
  Scrpd1.RegValue = 0;
  // get the handler details from Hob which is set during ConfigurePsmiHandler
  Scrpd1.Bits.PsmiHandlerBase = (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase >> 18);
  Scrpd1.Bits.PsmiHandlerResultCode  = PsmiDataHob->PsmiConfig.HandlerInfo.HandlerErrCode;
  Scrpd1.Bits.TraceRegion0ResultCode = PsmiDataHob->PsmiConfig.TraceRegionInfo[0].ErrCode;
  Scrpd1.Bits.TraceRegion1ResultCode = PsmiDataHob->PsmiConfig.TraceRegionInfo[1].ErrCode;
  Scrpd1.Bits.TraceRegion2ResultCode = PsmiDataHob->PsmiConfig.TraceRegionInfo[2].ErrCode;
  Scrpd1.Bits.TraceRegion3ResultCode = PsmiDataHob->PsmiConfig.TraceRegionInfo[3].ErrCode;
  Scrpd1.Bits.TraceRegion4ResultCode = PsmiDataHob->PsmiConfig.TraceRegionInfo[4].ErrCode;
  //
  // Write SCRPD1 register to output PSMI handler Base and Trace Error Info
  //
  WriteTraceHubScrpd1 (&Scrpd1.RegValue);
  DEBUG((DEBUG_INFO, "SCRPD1 Output = 0x%x \n",Scrpd1.RegValue));
  //
  // Output the Trace Region Base Address at Handler Memory offset 0x1000
  //
  PsmiHandlerSpace.PsmiHandlerBaseAddress = (UINT64) PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase;

  DEBUG((DEBUG_INFO, "PsmiHandlerSpace.PsmiHandlerBaseAddress = 0x%x \n", PsmiHandlerSpace.PsmiHandlerBaseAddress));
  DEBUG((DEBUG_INFO, "Scrpd1.Bits.PsmiHandlerResultCode = 0x%x \n", Scrpd1.Bits.PsmiHandlerResultCode));

  PsmiHandlerSpace.PsmiHandlerBaseAddress =  (UINT64) PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase;
  PsmiHandlerSpace.TraceRegion0BaseAddress = PsmiDataHob->PsmiConfig.TraceRegionInfo[0].TraceRegionBase;
  PsmiHandlerSpace.TraceRegion1BaseAddress = PsmiDataHob->PsmiConfig.TraceRegionInfo[1].TraceRegionBase;
  PsmiHandlerSpace.TraceRegion2BaseAddress = PsmiDataHob->PsmiConfig.TraceRegionInfo[2].TraceRegionBase;
  PsmiHandlerSpace.TraceRegion3BaseAddress = PsmiDataHob->PsmiConfig.TraceRegionInfo[3].TraceRegionBase;
  PsmiHandlerSpace.TraceRegion4BaseAddress = PsmiDataHob->PsmiConfig.TraceRegionInfo[4].TraceRegionBase;
  PsmiHandlerSpace.PsmiInputRegValue       = PsmiDataHob->PsmiConfig.PsmiInputRegValue;

  CopyMem ((VOID *) (UINTN)(PsmiHandlerSpace.PsmiHandlerBaseAddress + PSMI_HANDLER_MEMORY_OFFSET_1000), (VOID *)&(PsmiHandlerSpace.PsmiHandlerBaseAddress), sizeof (PSMI_HANDLER_SPACE_OFFSET));

  DEBUG ((DEBUG_INFO, "ConfigurePsmi () - End \n"));
  return EFI_SUCCESS;
}

/**
  Configures PSMI Handler Base

  @retval EFI_SUCCESS           ConfigurePsmi Function executed successfully
  @retval EFI_UNSUPPORTED       ConfigurePsmi failed to execute
**/
EFI_STATUS
ConfigurePsmiHandler (
  VOID
  )
{
  PSMI_OUTPUT_SCRPD1_REGISTER    Scrpd1;
  UINT32                         PsmiHandlerSize;
  UINT32                         PsmiHandlerBase;
  PSMI_DATA_HOB_VER1             *PsmiDataHob;
  UINT8                          HandlerErrCode;
  PSMI_HANDLER_SPACE_OFFSET      PsmiHandlerSpace;

  PsmiDataHob    = NULL;
  HandlerErrCode = PSMI_ALLOCATION_SUCCESS;

  DEBUG ((DEBUG_INFO, "%a () - Start\n", __FUNCTION__));
  //
  // Locate the PSMI Data hob, if not found , return
  //
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *)GetFirstGuidHob(&gPsmiDataHobGuid);
  if (PsmiDataHob == NULL) {
    return EFI_UNSUPPORTED;
  }

  if (PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize >= ARRAY_SIZE (PsmiHandlerSizeTab)){
    return EFI_INVALID_PARAMETER;
  }

  PsmiHandlerSize = PsmiHandlerSizeTab[PsmiDataHob->PsmiConfig.HandlerInfo.HandlerSize];

  if (PsmiHandlerSize == 0) {
    DEBUG ((DEBUG_INFO, "%a () - PSMI disabled \n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  PsmiHandlerBase = PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase;

  if (PsmiHandlerBase == 0) {
    DEBUG ((DEBUG_ERROR, "PSMI Handler allocation failed\n"));
    HandlerErrCode = ERROR_OUT_OF_RESOURCES;
  } else {
    DEBUG ((DEBUG_INFO, "PSMI Handler Base = 0x%x Size = 0x%x \n",PsmiHandlerBase,PsmiHandlerSize));
    //
    // Configure the PSMI Handler base memory type as Writeback(WB)
    //
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_RESERVED,                    // MemoryType,
      EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE,     // MemoryAttribute
      PsmiHandlerBase,                                 // MemoryBegin
      PsmiHandlerSize                                  // MemoryLength
      );
  }
  Scrpd1.RegValue = 0;
  Scrpd1.Bits.PsmiHandlerBase        = (PsmiHandlerBase >> 18);
  Scrpd1.Bits.PsmiHandlerResultCode  = HandlerErrCode;

  //
  // Update the Handler Base and ErrCode to Hob later while write to register get the Handler details from Hob
  //
  PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase = PsmiHandlerBase;
  PsmiDataHob->PsmiConfig.HandlerInfo.HandlerErrCode = HandlerErrCode;

  DEBUG((DEBUG_INFO, "SCRPD1 Output = 0x%x \n", Scrpd1.RegValue));

  //
  // Write SCRPD1 register to output PSMI handler Base
  //
  PsmiHandlerSpace.PsmiHandlerBaseAddress =  (UINT64) PsmiHandlerBase;
  CopyMem ((VOID *) (UINTN)(PsmiHandlerSpace.PsmiHandlerBaseAddress + PSMI_HANDLER_MEMORY_OFFSET_1000), (VOID *)&(PsmiHandlerSpace.PsmiHandlerBaseAddress), sizeof (PSMI_HANDLER_SPACE_OFFSET));

  DEBUG ((DEBUG_INFO, "%a () - End\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  Init and Install PSMI Hob
  @param[in] SiPreMemPolicyPpi       - The Silicon PreMem Policy PPI instance
  @param[out] PSMI_DATA_HOB_VER1           - PSMI_DATA_HOB instance installed by this function

  @retval EFI_SUCCESS
**/
EFI_STATUS
InstallPsmiHobVer1 (
  IN    SI_PREMEM_POLICY_PPI      *SiPreMemPolicyPpi,
  OUT   PSMI_DATA_HOB_VER1        **PsmiDataHobOut
  )
{
  EFI_STATUS                    Status;
  PSMI_DATA_HOB_VER1            *PsmiDataHob;

  ///
  /// Create HOB for PSMI Data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (PSMI_DATA_HOB_VER1),
             (VOID **) &PsmiDataHob
             );
  ASSERT_EFI_ERROR (Status);

  ///
  /// Initialize default HOB data
  ///
  ///
  /// PSMI_DATA_HOB_VER1
  ///
  CopyGuid (&PsmiDataHob->EfiHobGuidType.Name, &gPsmiDataHobGuid);
  DEBUG ((DEBUG_INFO, "PsmiDataHob->EfiHobGuidType.Name: %g\n", &PsmiDataHob->EfiHobGuidType.Name));
  ZeroMem (&(PsmiDataHob->PsmiConfig), sizeof (PSMI_DATA_HOB_VER1) - sizeof (EFI_HOB_GUID_TYPE));
  DEBUG ((DEBUG_INFO, "PsmiDataHob @ %X\n", PsmiDataHob));
  DEBUG ((DEBUG_INFO, "PsmiDataHobSize - HobHeader: %X\n", sizeof (PSMI_DATA_HOB_VER1) - sizeof (EFI_HOB_GUID_TYPE)));
  DEBUG ((DEBUG_INFO, "PsmiDataHobSize: %X\n", sizeof (PSMI_DATA_HOB_VER1)));

  (*PsmiDataHobOut)   = PsmiDataHob;

  DEBUG ((DEBUG_INFO, "PSMI HOB installed\n"));

  return EFI_SUCCESS;
}

/**
  Update PSMI Handler Base

  @param[in] BaseAddress  - Base Address of PSMI Hanlder

**/
VOID
EFIAPI
UpdatePsmiHandlerBaseAddress (
  IN UINT32     BaseAddress
  )
{
  PSMI_DATA_HOB_VER1             *PsmiDataHob;

  PsmiDataHob    = NULL;
  //
  // Locate the PSMI Data hob, if not found , return
  //
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *) GetFirstGuidHob(&gPsmiDataHobGuid);
  if (PsmiDataHob == NULL) {
    DEBUG ((DEBUG_INFO, "PSMI HOB Not Present\n"));
    return;
  }

  PsmiDataHob->PsmiConfig.HandlerInfo.HandlerBase = BaseAddress << 20;
}

/**
  Reserve Graphics Memory.
  @param[in] TopUseableMemAddr          - Moving address pointer
  @param[in] Touud                      - Size leftover pointer
  @param[in] ResourceAttributeTested    - Resource attribute flag
  @retval EFI_SUCCESS            - GT Memory Allocation Success
**/
VOID
EFIAPI
PeiPsmiMemoryAllocation (
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud,
  IN EFI_RESOURCE_ATTRIBUTE_TYPE       ResourceAttributeTested
)
{
  BOOLEAN                            PsmiMemoryRequired;
  UINT8                              Index;
  UINT32                             Alignment;
  UINT64                             PsmiMemSize;
  EFI_PHYSICAL_ADDRESS               PsmiMemBaseAddr;
  EFI_PHYSICAL_ADDRESS               TempPsmiMemBaseAddr;
  PSMI_DATA_HOB_VER1                 *PsmiDataHob;
  MEMORY_PLATFORM_DATA_HOB           *MemoryPlatformData;
  EFI_RESOURCE_TYPE                  ResourceType;
  EFI_RESOURCE_ATTRIBUTE_TYPE        ResourceAttribute;

  //
  // Align at 512 MB boundary in end since FSP later reserves MTRR for High Memory at 512 MB alignment upwards which overlaps with PSMIU memory
  //
  Alignment = SIZE_512MB;
  PsmiMemoryRequired = FALSE;
  PsmiDataHob = NULL;
  MemoryPlatformData = NULL;

  //
  // Locate the Hob structures
  //
#if FixedPcdGetBool (PcdPsmiEnable) == 1
  PsmiDataHob = (PSMI_DATA_HOB_VER1 *) GetFirstGuidHob (&gPsmiDataHobGuid);
  MemoryPlatformData = (MEMORY_PLATFORM_DATA_HOB *) GetFirstGuidHob (&gSiMemoryPlatformDataGuid);
  if ((MemoryPlatformData == NULL) || (PsmiDataHob == NULL)) { // Skip PSMI allocation if output hob not present
    PsmiMemoryRequired = FALSE;
    DEBUG ((DEBUG_INFO, "MemoryPlatformData/PSMI hob not found, skipping PSMI allocation \n"));
  }
#endif

  if ((MemoryPlatformData != NULL) && (PsmiDataHob != NULL)) {
    // MemoryPlatformData hob is present so find out if PSMI memory is required.
    for (Index = 0; Index < MAX_TRACE_REGION; Index++) {
      if (PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize) {
        PsmiMemoryRequired = TRUE;
        break;
      }
    }
    //
    // No Memory Available above 4GB, add the error code
    //
    if (PsmiMemoryRequired && (*Touud <= BASE_4GB)) {
      for (Index = 0; Index < MAX_TRACE_CACHE_TYPE; Index++) {
        MemoryPlatformData->Data.PsmiInfo[Index].ErrorCode = ERROR_NO_ABOVE_4G_MEMORY;
      }
    }
  }

  if (PsmiMemoryRequired && (MemoryPlatformData != NULL) && (PsmiDataHob != NULL)) {

    PsmiMemBaseAddr      = *TopUseableMemAddr;
    for (Index = 0; Index < MAX_TRACE_REGION; Index++) {
      //
      // Check if any of PSMI memory Type needed
      //
      PsmiMemSize = PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].TotalSize;
      DEBUG ((DEBUG_INFO, "PSMI Region = %d Size = 0x%lx\n", Index, PsmiMemSize));
      if (PsmiMemSize == 0) {
        continue;
      }
      DEBUG ((DEBUG_INFO, "PSMI Region = 0x%x Touud = 0x%lx\n",Index,*Touud));
      if (*Touud <= PsmiMemSize) {
        //
        // Not sufficient memory above PsmiMemBaseAddr
        //
        MemoryPlatformData->Data.PsmiRegionInfo[Index].ErrorCode = ERROR_OUT_OF_RESOURCES;
        DEBUG ((DEBUG_INFO,"No sufficient memory above 4GB\n"));
        break;
      }
      ResourceType         = EFI_RESOURCE_MEMORY_RESERVED;
      ResourceAttribute    = 0;
      if (PsmiDataHob->PsmiConfig.TraceRegionInfo[Index].RangeCompatible == ENABLED) {
        // Copy Current Base Address to Temp so that after Alignment changes we can use this temp to calculate the diffence between the change in size
        TempPsmiMemBaseAddr = PsmiMemBaseAddr;
        PsmiMemBaseAddr = NaturalAlignment (PsmiMemBaseAddr, PsmiMemSize);
        //
        // if not naturally aligned, and contiguous trace region is enabled reverve that region else dont do anything, that region will be available to OS
        //
        if (PsmiMemBaseAddr > TempPsmiMemBaseAddr) {
            DEBUG ((DEBUG_INFO, "After PSMI Alignement has Changes, Chance in Size = 0x%lx\n",(PsmiMemBaseAddr - TempPsmiMemBaseAddr)));
            BuildResourceDescriptorHob (
              ResourceType,                             // MemoryType,
              ResourceAttribute,                        // MemoryAttribute
              TempPsmiMemBaseAddr,                      // MemoryBegin
              (PsmiMemBaseAddr - TempPsmiMemBaseAddr)   // MemoryLength
              );
          // Decrement the TopMemSize.Data if there is change in address
          *Touud -= (PsmiMemBaseAddr - TempPsmiMemBaseAddr);
        }
      }
      DEBUG ((DEBUG_INFO, "PSMI after align Address = 0x%lx  Size = 0x%lx\n", PsmiMemBaseAddr, PsmiMemSize));
      if (PsmiDataHob->PsmiConfig.PsmiMrcRegionParam[Index].CacheType == RegionWriteBack) { // Write-Back
        ResourceAttribute = EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE;
        MemoryPlatformData->Data.PsmiRegionInfo[Index].CacheType = RegionWriteBack;
      }
      BuildResourceDescriptorHob (
        ResourceType,       // MemoryType,
        ResourceAttribute,  // MemoryAttribute
        PsmiMemBaseAddr,    // MemoryBegin
        PsmiMemSize         // MemoryLength
        );
      // After reserving the memory increase the TopUseableMemAddr and PsmiMemBaseAddr
      MemoryPlatformData->Data.PsmiRegionInfo[Index].BaseAddress = PsmiMemBaseAddr;
      DEBUG ((DEBUG_INFO, "PSMI Region 0x%x Reserved Size = 0x%lx at BaseAddress = 0x%lx\n" , Index, PsmiMemSize, PsmiMemBaseAddr));
      PsmiMemBaseAddr += PsmiMemSize;
      *Touud -= PsmiMemSize;
      DEBUG ((DEBUG_INFO, "Remaining Touud = 0x%lX\n",*Touud));
    }

    //
    // Reserve 512 MB above PSMI memory for alignment, capture error Out of Resources if No/less above 4G memory for alignment
    //
    if ((*Touud - Alignment) < PsmiMemBaseAddr) {
      for (Index = 0; Index < MAX_TRACE_REGION; Index++) {
        MemoryPlatformData->Data.PsmiRegionInfo[Index].ErrorCode = ERROR_OUT_OF_RESOURCES;
        DEBUG ((DEBUG_INFO, "PSMI MemoryPlatformData->Data.PsmiRegionInfo[Index].ErrorCode = 0x%x\n",MemoryPlatformData->Data.PsmiRegionInfo[Index].ErrorCode));
      }
    } else {
      ResourceType      = EFI_RESOURCE_MEMORY_RESERVED;
      ResourceAttribute = 0;
      DEBUG ((DEBUG_INFO, "PsmiMemBaseAddr = 0x%016lX\n", PsmiMemBaseAddr));
     BuildResourceDescriptorHob (
        ResourceType,                        // MemoryType,
        ResourceAttribute,                   // MemoryAttribute
        PsmiMemBaseAddr,                     // MemoryBegin
        Alignment                            // MemoryLength
        );
      DEBUG ((DEBUG_INFO, "Found 0x%x bytes for PSMI alignment reserved at PsmiMemBaseAddr = 0x%016lX\n", Alignment, PsmiMemBaseAddr));
      PsmiMemBaseAddr += Alignment;
      *Touud -= Alignment;
    }
    // Reassign the TopUseableMemAddr with the updated Address
    *TopUseableMemAddr = PsmiMemBaseAddr;
  }
}
