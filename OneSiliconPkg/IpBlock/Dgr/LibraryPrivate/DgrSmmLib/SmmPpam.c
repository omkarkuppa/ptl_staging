/** @file
  SMM PPAM support functions

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

#include "DgrSmm.h"

EFI_HANDLE  mPpamSmmCpuHandle = NULL;
BOOLEAN     mSmmDgrLockLoadMonitor  = FALSE;

//
// MSEG Base and Length in SMRAM
//
UINTN  mSmmDgrMsegBase = 0;
UINTN  mSmmDgrMsegSize = 0;

SMM_ENTRY_POINT_INFORMATION_TABLE  *mSmmDgrEntryPointInfoTable;

VOID
PpamSmiEntryPointEnd (
  VOID
  );

VOID
PpamSmmConfigurationTableInit (
  VOID
  );

//
// System Configuration Table pointing to PPAM Configuration Table
//
GLOBAL_REMOVE_IF_UNREFERENCED
EFI_SM_MONITOR_INIT_PROTOCOL mPpamSmMonitorInitProtocol = {
  PpamLoadMonitor,
  PpamAddPiResource,
  PpamDeletePiResource,
  PpamGetPiResource,
  PpamGetMonitorState,
};

/**

  Retieves MSEG information.

**/
VOID
GetMsegInfo (
  VOID
  )
{
  CPUID_VERSION_INFO_ECX  RegEcx;
  EFI_HOB_GUID_TYPE       *GuidHob;
  EFI_SMRAM_DESCRIPTOR    *SmramDescriptor;

  //
  // If CPU supports VMX, then determine SMRAM range for MSEG.
  //
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &RegEcx.Uint32, NULL);
  if (RegEcx.Bits.VMX == 1) {
    GuidHob = GetFirstGuidHob (&gMsegSmramGuid);
    if (GuidHob != NULL) {
      DEBUG((DEBUG_INFO, "Found gMsegSmramGuid hob\n"));
      //
      // Retrieve MSEG location from MSEG SRAM HOB
      //
      SmramDescriptor = (EFI_SMRAM_DESCRIPTOR *) GET_GUID_HOB_DATA (GuidHob);
      if (SmramDescriptor->PhysicalSize > 0) {
        mSmmDgrMsegBase       = (UINTN)SmramDescriptor->CpuStart;
        mSmmDgrMsegSize       = (UINTN)SmramDescriptor->PhysicalSize;
      }
    } else if (PcdGet32 (PcdCpuMsegSize) > 0) {
      //
      // Allocate MSEG from SMRAM memory
      //
      mSmmDgrMsegBase = (UINTN)AllocatePages (EFI_SIZE_TO_PAGES (PcdGet32 (PcdCpuMsegSize)));
      if (mSmmDgrMsegBase > 0) {
        mSmmDgrMsegSize = ALIGN_VALUE (PcdGet32 (PcdCpuMsegSize), EFI_PAGE_SIZE);
      } else {
        DEBUG ((DEBUG_ERROR, "Not enough SMRAM resource to allocate MSEG size %08x\n", PcdGet32 (PcdCpuMsegSize)));
      }
    }
    if (mSmmDgrMsegBase > 0) {
      DEBUG ((DEBUG_INFO, "MsegBase: 0x%08x, MsegSize: 0x%08x\n", mSmmDgrMsegBase, mSmmDgrMsegSize));
    }
  }
}

/**
  Internal worker function that is called to complete CPU initialization at the
  end of SmmCpuFeaturesInitializeProcessor().

**/
VOID
FinishSmmCpuFeaturesInitializeProcessor (
  VOID
  )
{
  MSR_IA32_SMM_MONITOR_CTL_REGISTER  SmmMonitorCtl;

  //
  // Set MSEG Base Address in SMM Monitor Control MSR.
  //
  if (mSmmDgrMsegBase > 0) {
    SmmMonitorCtl.Uint64 = 0;
    SmmMonitorCtl.Bits.MsegBase = (UINT32)mSmmDgrMsegBase >> 12;
    SmmMonitorCtl.Bits.Valid = 1;
    AsmWriteMsr64 (MSR_IA32_SMM_MONITOR_CTL, SmmMonitorCtl.Uint64);
  }
}

/**
  SMM End Of Dxe event notification handler.

  PPAM support need patch AcpiRsdp in TXT_PROCESSOR_SMM_DESCRIPTOR.

  @param[in] Protocol   Points to the protocol's unique identifier.
  @param[in] Interface  Points to the interface instance.
  @param[in] Handle     The handle on which the interface was installed.

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
PpamSmmEndOfDxeEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  DEBUG ((DEBUG_INFO, "SmmEndOfDxeEventNotify\n"));

  mSmmDgrLockLoadMonitor = TRUE;

  return EFI_SUCCESS;
}

/**
  This function initializes the PPAM configuration table.
**/
VOID
PpamSmmConfigurationTableInit (
  VOID
  )
{
  EFI_STATUS    Status;
    VOID        *Registration;

  Status = gSmst->SmmInstallProtocolInterface (
                    &mPpamSmmCpuHandle,
                    &gEfiSmMonitorInitProtocolGuid,
                    EFI_NATIVE_INTERFACE,
                    &mPpamSmMonitorInitProtocol
                    );
  ASSERT_EFI_ERROR (Status);

  //
  //
  // Register SMM End of DXE Event
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmEndOfDxeProtocolGuid,
                    PpamSmmEndOfDxeEventNotify,
                    &Registration
                    );
  ASSERT_EFI_ERROR (Status);
}

/**
  Debug log output of SMM Entry Data information as per its size.

  @param[in] Data  Points to the Data offset.
  @param[in] Size  Size of Data.
**/
VOID
DumpDataWithSize (
  IN VOID *Data,
  IN UINTN Size
  )
{
  switch (Size) {
  case 1:
    DEBUG ((DEBUG_INFO, "0x%02x", *(UINT8 *)Data));
    break;
  case 2:
    DEBUG ((DEBUG_INFO, "0x%04x", *(UINT16 *)Data));
    break;
  case 4:
    DEBUG ((DEBUG_INFO, "0x%08x", *(UINT32 *)Data));
    break;
  case 6:
    DEBUG ((DEBUG_INFO, "0x%04x%08x", *(UINT16 *)((UINT8 *)Data + 4), *(UINT32 *)Data));
    break;
  case 8:
    DEBUG ((DEBUG_INFO, "0x%016lx", *(UINT64 *)Data));
    break;
  case 10:
    DEBUG ((DEBUG_INFO, "0x%04x%016lx", *(UINT16 *)((UINT8 *)Data + 8), *(UINT64 *)Data));
    break;
  }
}

/**
  Copies the data into SMM Entry Info Table

  @param[in] SmmEntryPointHeader  Points to the SMM Entry Point Header.
  @param[in] Type                 SMM Entry point Information type.
  @param[in] Data                 Points to Data to be copied.
  @param[in] DataSize             Size of Data.
**/
VOID
EFIAPI
PatchSmmEntryPoint (
  IN VOID      *SmmEntryPointHeader,
  IN UINT8     Type,
  IN VOID      *Data,
  IN UINT8     DataSize
  )
{
  SMM_ENTRY_POINT_INFORMATION_ENTRY  *SmmInfoEntry;

  DEBUG ((DEBUG_INFO, "SMM INFO TABLE\n"));
  // Add 1 to SMM Entry Info Table to point to Info Entry
  SmmInfoEntry = (VOID *)(mSmmDgrEntryPointInfoTable + 1);

  for (; ; SmmInfoEntry++) {
    if (SmmInfoEntry->Type == SMM_ENTRY_POINT_INFO_END) {
      break;
    }
    if (SmmInfoEntry->Type == Type) {
      ASSERT (SmmInfoEntry->DataSize == DataSize);
      DEBUG ((DEBUG_INFO, "Entry Address: 0x%llx\n", (UINT64) SmmInfoEntry));
      DEBUG ((DEBUG_INFO, "Patch: [Type:0x%x] [Size:0x%x] Offset: 0x%x (", SmmInfoEntry->Type, SmmInfoEntry->DataSize, SmmInfoEntry->DataOffset));
      DumpDataWithSize ((UINT8 *) SmmEntryPointHeader + SmmInfoEntry->DataOffset, SmmInfoEntry->DataSize);
      DEBUG ((DEBUG_INFO, ") <- ("));
      DumpDataWithSize (Data, SmmInfoEntry->DataSize);
      DEBUG ((DEBUG_INFO, ")\n"));

      CopyMem ((UINT8 *) SmmEntryPointHeader + SmmInfoEntry->DataOffset, Data, SmmInfoEntry->DataSize);
      break;
    }
  }

  if (SmmInfoEntry->Type != Type) {
    //
    // Asserting the system in case if any Entry field is missed to update in SMM Entry Info Table.
    // Not required in release build as this is only to catch if any field is missed.
    //
    DEBUG ((DEBUG_ERROR, "SMM Entry not found for the given Type 0x%x in SMM Info Table. Asserting...\n", Type));
    ASSERT (FALSE);
  }
}

/**

  Get PPAM state.

  @return PPAM state

**/
EFI_SM_MONITOR_STATE
EFIAPI
PpamGetMonitorState (
  VOID
  )
{
  return 0;
}

/**

  Add resources in list to database. Allocate new memory areas as needed.

  @param ResourceList  A pointer to resource list to be added
  @param NumEntries    Optional number of entries.
                       If 0, list must be terminated by END_OF_RESOURCES.

  @retval EFI_SUCCESS            If resources are added
  @retval EFI_INVALID_PARAMETER  If nested procedure detected resource failer
  @retval EFI_OUT_OF_RESOURCES   If nested procedure returned it and we cannot allocate more areas.

**/
EFI_STATUS
EFIAPI
PpamAddPiResource (
  IN  STM_RSC    *ResourceList,
  IN  UINT32      NumEntries OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Delete resources in list to database.

  @param ResourceList  A pointer to resource list to be deleted
                       NULL means delete all resources.
  @param NumEntries    Optional number of entries.
                       If 0, list must be terminated by END_OF_RESOURCES.

  @retval EFI_SUCCESS            If resources are deleted
  @retval EFI_INVALID_PARAMETER  If nested procedure detected resource failer

**/
EFI_STATUS
EFIAPI
PpamDeletePiResource (
  IN  STM_RSC    *ResourceList,
  IN  UINT32      NumEntries OPTIONAL
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Get BIOS resources.

  @param ResourceList  A pointer to resource list to be filled
  @param ResourceSize  On input it means size of resource list input.
                       On output it means size of resource list filled,
                       or the size of resource list to be filled if size of too small.

  @retval EFI_SUCCESS            If resources are returned.
  @retval EFI_BUFFER_TOO_SMALL   If resource list buffer is too small to hold the whole resources.

**/
EFI_STATUS
EFIAPI
PpamGetPiResource (
  OUT    STM_RSC *ResourceList,
  IN OUT UINT32  *ResourceSize
  )
{
  return EFI_UNSUPPORTED;
}

/**

  Get 4K page aligned VMCS size.

  @return 4K page aligned VMCS size

**/
UINT32
GetVmcsSize (
  VOID
  )
{
  MSR_IA32_VMX_BASIC_REGISTER  VmxBasic;

  //
  // Read VMCS size and and align to 4KB
  //
  VmxBasic.Uint64 = AsmReadMsr64 (MSR_IA32_VMX_BASIC);
  return ALIGN_VALUE (VmxBasic.Bits.VmcsSize, SIZE_4KB);
}

/**

  Check PPAM image size.

  @param PpamImage      PPAM image
  @param PpamImageSize  PPAM image size

  @retval TRUE  check pass
  @retval FALSE check fail
**/
BOOLEAN
PpamCheckPpamImage (
  IN EFI_PHYSICAL_ADDRESS PpamImage,
  IN UINTN                PpamImageSize
  )
{
  UINTN                     MinMsegSize;
  PPAM_HEADER               *PpamHeader;
  IA32_VMX_MISC_REGISTER    VmxMiscMsr;

  //
  // Check to see if PPAM image is compatible with CPU
  //
  PpamHeader = (PPAM_HEADER *)(UINTN)PpamImage;
  VmxMiscMsr.Uint64 = AsmReadMsr64 (MSR_IA32_VMX_MISC);
  if (PpamHeader->HwPpamHdr.HardwareHeaderRevision != VmxMiscMsr.Bits.MsegRevisionIdentifier) {
    DEBUG ((DEBUG_ERROR, "PPAM Image not compatible with CPU\n"));
    DEBUG ((DEBUG_ERROR, "  PpamHeader->HwPpamHdr.HardwareHeaderRevision = %08x\n", PpamHeader->HwPpamHdr.HardwareHeaderRevision));
    DEBUG ((DEBUG_ERROR, "  VmxMiscMsr.Bits.MsegRevisionIdentifier = %08x\n", VmxMiscMsr.Bits.MsegRevisionIdentifier));
    return FALSE;
  }

  //
  // Get Minimal required Mseg size
  //
  MinMsegSize = (EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (PpamHeader->SwPpamHdr.StaticImageSize)) +
                 PpamHeader->SwPpamHdr.AdditionalDynamicMemorySize +
                 (PpamHeader->SwPpamHdr.PerProcDynamicMemorySize + GetVmcsSize ()) * gSmst->NumberOfCpus);
  if (MinMsegSize < PpamImageSize) {
    MinMsegSize = PpamImageSize;
  }

  if (PpamHeader->HwPpamHdr.Cr3Offset >= PpamHeader->SwPpamHdr.StaticImageSize) {
    //
    // We will create page table, just in case that SINIT does not create it.
    //
    if (MinMsegSize < PpamHeader->HwPpamHdr.Cr3Offset + EFI_PAGES_TO_SIZE(6)) {
      MinMsegSize = PpamHeader->HwPpamHdr.Cr3Offset + EFI_PAGES_TO_SIZE(6);
    }
  }

  //
  // Check if it exceeds MSEG size
  //
  if (MinMsegSize > mSmmDgrMsegSize) {
    DEBUG ((DEBUG_ERROR, "MSEG too small.  Min MSEG Size = %08x  Current MSEG Size = %08x\n", MinMsegSize, mSmmDgrMsegSize));
    DEBUG ((DEBUG_ERROR, "  PpamHeader->SwPpamHdr.StaticImageSize             = %08x\n", PpamHeader->SwPpamHdr.StaticImageSize));
    DEBUG ((DEBUG_ERROR, "  PpamHeader->SwPpamHdr.AdditionalDynamicMemorySize = %08x\n", PpamHeader->SwPpamHdr.AdditionalDynamicMemorySize));
    DEBUG ((DEBUG_ERROR, "  PpamHeader->SwPpamHdr.PerProcDynamicMemorySize    = %08x\n", PpamHeader->SwPpamHdr.PerProcDynamicMemorySize));
    DEBUG ((DEBUG_ERROR, "  VMCS Size                                         = %08x\n", GetVmcsSize ()));
    DEBUG ((DEBUG_ERROR, "  Max CPUs                                          = %08x\n", gSmst->NumberOfCpus));
    DEBUG ((DEBUG_ERROR, "  PpamHeader->HwPpamHdr.Cr3Offset                   = %08x\n", PpamHeader->HwPpamHdr.Cr3Offset));
    return FALSE;
  }

  return TRUE;
}

/**

  Load PPAM image to MSEG.

  @param PpamImage      PPAM image
  @param PpamImageSize  PPAM image size

**/
VOID
PpamLoadPpamImage (
  IN EFI_PHYSICAL_ADDRESS PpamImage,
  IN UINTN                PpamImageSize
  )
{
  MSR_IA32_SMM_MONITOR_CTL_REGISTER  SmmMonitorCtl;
  UINT32                             MsegBase;
  PPAM_HEADER                        *PpamHeader;

  //
  // Get MSEG base address from MSR_IA32_SMM_MONITOR_CTL
  //
  SmmMonitorCtl.Uint64 = AsmReadMsr64 (MSR_IA32_SMM_MONITOR_CTL);
  MsegBase = SmmMonitorCtl.Bits.MsegBase << 12;

  //
  // Zero all of MSEG base address
  //
  ZeroMem ((VOID *)(UINTN)MsegBase, mSmmDgrMsegSize);

  //
  // Copy PPAM Image into MSEG
  //
  CopyMem ((VOID *)(UINTN)MsegBase, (VOID *)(UINTN)PpamImage, PpamImageSize);

  //
  // PPAM Header is at the beginning of the PPAM Image
  //
  PpamHeader = (PPAM_HEADER *)(UINTN)PpamImage;

  PpamGen4GPageTable ((UINTN)MsegBase + PpamHeader->HwPpamHdr.Cr3Offset);
}

/**

  Load PPAM image to MSEG.

  @param PpamImage      PPAM image
  @param PpamImageSize  PPAM image size

  @retval EFI_SUCCESS            Load PPAM to MSEG successfully
  @retval EFI_ALREADY_STARTED    PPAM image is already loaded to MSEG
  @retval EFI_BUFFER_TOO_SMALL   MSEG is smaller than minimal requirement of PPAM image
  @retval EFI_UNSUPPORTED        MSEG is not enabled

**/
EFI_STATUS
EFIAPI
PpamLoadMonitor (
  IN EFI_PHYSICAL_ADDRESS PpamImage,
  IN UINTN                PpamImageSize
  )
{
  MSR_IA32_SMM_MONITOR_CTL_REGISTER  SmmMonitorCtl;

  if (mSmmDgrLockLoadMonitor) {
    return EFI_ACCESS_DENIED;
  }

  SmmMonitorCtl.Uint64 = AsmReadMsr64 (MSR_IA32_SMM_MONITOR_CTL);
  if (SmmMonitorCtl.Bits.MsegBase == 0) {
    return EFI_UNSUPPORTED;
  }

  if (!PpamCheckPpamImage (PpamImage, PpamImageSize)) {
    return EFI_BUFFER_TOO_SMALL;
  }

  PpamLoadPpamImage (PpamImage, PpamImageSize);

  return EFI_SUCCESS;
}
