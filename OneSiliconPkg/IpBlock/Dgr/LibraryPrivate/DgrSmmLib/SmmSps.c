/** @file
  SMM Policy Shim (SPS) support functions

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
#include "SmmResourceConfig.h"
#include "SmmSpa.h"
#include <Protocol/SmmMemoryAttribute.h>
#include <Uefi/UefiBaseType.h>
#include <Library/VtdInfoLib.h>
#include <IntelRcStatusCode.h>
#include <Register/Intel/SmramSaveStateMap.h>
#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
#include <Protocol/SmmSwDispatch2.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/DgrNvsArea.h>
#endif

/**
  Setup SMM Protected Mode context for processor.

  @param  ProcessorNumber    The processor number.
  @param  SmBase             The SMBASE value of the processor.
  @param  StackAddress       Stack address of the processor.
  @param  GdtBase            Gdt table base address of the processor.
  @param  GdtSize            Gdt table size of the processor.
  @param  CodeSegment        Code segment value.
  @param  ProtModeIdt        Pointer to protected-mode IDT descriptor.
**/
VOID
SetupSmmProtectedModeContext(
  IN UINTN                    ProcessorNumber,
  IN UINT32                   SmBase,
  IN UINT32                   StackAddress,
  IN UINTN                    GdtBase,
  IN UINTN                    GdtSize,
  IN UINT16                   CodeSegment,
  IN IA32_DESCRIPTOR          *ProtModeIdt
  );

extern VOID
SetPageTableAttributes (
  VOID
  );

extern VOID *
AllocatePageTableMemory (
  IN UINTN  Pages
  );

extern VOID
PpamSmmConfigurationTableInit (
  VOID
  );

IA32_DESCRIPTOR                     gSmmDgrFeatureSmiHandlerIdtr;
volatile UINT32                     gSmmDgrFeatureSmiStack;
UINT32                              gSmmDgrFeatureSmiCr3;

extern SPA_CTXT                     *gSmmDgrSpaCtxt;
extern EFI_HANDLE                   gSmmDgrSpaReadyToLockRegistration;
extern UINT32                       gSmmDgrProtModeIdtr;
extern BOOLEAN                      mSmmDgrProtectedModeEnable;
extern BOOLEAN                      mSmmDgrStateSaveEnable;
extern UINT8                        gSmmDgrEnableState;
UINT64                              gSmmDgrSupovrStateLockData;
BOOLEAN                             gSmmDgrRing3Supported;
BOOLEAN                             gSmmDgrSupervisorStateSave = 0;

volatile UINT32                     gSmmDgrFeatureSmiUserStack;
UINT32                              gSmmDgrUserStackSize;
UINTN                               gSmmDgrCpuStackArrayBase;
UINTN                               gSmmDgrCpuStackSize;
UINT64                              gSmmDgrMsrBitMapBase;
BOOLEAN                             gSmmDgrSpaEnable = FALSE;

UINT8                               *gSmmDgrExceptionStack;
UINTN                               gSmmDgrExceptionStackSize;
SPIN_LOCK                           *mSmmDgrInternalDebugLock;

BOOLEAN                             gSmmDgrGdtFlag = FALSE;
EFI_PHYSICAL_ADDRESS                gSmmDgrGdtBuffer;
UINTN                               gSmmDgrGdtBufferSize;
UINTN                               gSmmDgrGdtTssTableSize;
EFI_PHYSICAL_ADDRESS                gSmmDgrTxtDprMemoryBase = 0;
UINT64                              gSmmDgrTxtDprMemorySize = 0;
#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
EFI_PHYSICAL_ADDRESS                gSmmPpamParamPage = 0;
EFI_PHYSICAL_ADDRESS                gSmmLedgerIslandBuffer = 0;;
EFI_PHYSICAL_ADDRESS                gLedgerIslandBuffer = 0;
EFI_SMM_BASE2_PROTOCOL              *gSMM = NULL;
EFI_SMM_SYSTEM_TABLE2               *mSmst = NULL;
#endif

//
// Global copy of the PcdPteMemoryEncryptionAddressOrMask
//
extern UINT64                             mAddressEncMask;
extern UINTN                              mNumberOfCpus;

EDKII_SMM_MEMORY_ATTRIBUTE_PROTOCOL       *gSmmDgrMemoryAttribute = NULL;
SPS_RING3_EXCEPTION_HANDLER               *gSpsRing3ExceptionHandlerProtocol = NULL;
SMM_ENTRY_POINT_HEADER                    *mSmmDgrEntryPointHeader;
extern SMM_ENTRY_POINT_INFORMATION_TABLE  *mSmmDgrEntryPointInfoTable;
BOOLEAN                                   mPpamConfigurationTableInitialized = FALSE;

VOID
EFIAPI
AsmDgrSmiRendezvous (
  VOID
  );

VOID
EFIAPI
AsmOemExceptionHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  );

VOID   *gSpsBin;
UINTN  gSpsBinSize;

SPS_KERNEL_CONTEXT      *mSpsKernelContext;

/**
  Allocate Code page for PE code

  @param[in] ImageAddress  Points to addres of Image data
  @param[in] ImageSize     Size of Image data
  @param[in] ImageAddressTemp  Points to addres of Image data extracted from FV.
**/
VOID
ConvertCodePage (
  IN VOID                    *ImageAddress,
  IN UINTN                   ImageSize,
  IN VOID                    *ImageAddressTemp
  )
{
  EFI_IMAGE_DOS_HEADER                 *DosHdr;
  UINT32                               PeCoffHeaderOffset;
  UINT32                               SectionAlignment;
  EFI_IMAGE_SECTION_HEADER             *Section;
  EFI_IMAGE_OPTIONAL_HEADER_PTR_UNION  Hdr;
  UINT8                                *Name;
  UINTN                                Index;
  UINT16                               Magic;
  EFI_STATUS                           Status;
  EFI_PHYSICAL_ADDRESS                 CodeMemory;
  UINTN                                CodePages;
  EFI_PHYSICAL_ADDRESS                 CodeMemoryTemp;

  DosHdr = (EFI_IMAGE_DOS_HEADER *) (UINTN) ImageAddress;
  PeCoffHeaderOffset = 0;
  if (DosHdr->e_magic == EFI_IMAGE_DOS_SIGNATURE) {
    PeCoffHeaderOffset = DosHdr->e_lfanew;
  }

  Hdr.Pe32 = (EFI_IMAGE_NT_HEADERS32 *)((UINT8 *) (UINTN) ImageAddress + PeCoffHeaderOffset);
  if (Hdr.Pe32->Signature != EFI_IMAGE_NT_SIGNATURE) {
    DEBUG ((DEBUG_ERROR, "SPS Hdr.Pe32->Signature invalid - 0x%x\n", Hdr.Pe32->Signature));
    ASSERT(FALSE);
    return;
  }

  //
  // Get SectionAlignment
  //
  if (Hdr.Pe32->FileHeader.Machine == IMAGE_FILE_MACHINE_IA64 && Hdr.Pe32->OptionalHeader.Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    //
    // NOTE: Some versions of Linux ELILO for Itanium have an incorrect magic value
    //       in the PE/COFF Header. If the MachineType is Itanium(IA64) and the
    //       Magic value in the OptionalHeader is EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC
    //       then override the magic value to EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC
    //
    Magic = EFI_IMAGE_NT_OPTIONAL_HDR64_MAGIC;
  } else {
    //
    // Get the magic value from the PE/COFF Optional Header
    //
    Magic = Hdr.Pe32->OptionalHeader.Magic;
  }
  if (Magic == EFI_IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
    SectionAlignment  = Hdr.Pe32->OptionalHeader.SectionAlignment;
  } else {
    SectionAlignment  = Hdr.Pe32Plus->OptionalHeader.SectionAlignment;
  }

  if ((SectionAlignment & (SIZE_4KB - 1)) != 0) {
    DEBUG ((DEBUG_ERROR, "SPS Section Alignment(0x%x) is not 4K\n", SectionAlignment));
    ASSERT(FALSE);
    return;
  }

  Section = (EFI_IMAGE_SECTION_HEADER *) (
               (UINT8 *) (UINTN) ImageAddress +
               PeCoffHeaderOffset +
               sizeof(UINT32) +
               sizeof(EFI_IMAGE_FILE_HEADER) +
               Hdr.Pe32->FileHeader.SizeOfOptionalHeader
               );
  for (Index = 0; Index < Hdr.Pe32->FileHeader.NumberOfSections; Index++) {
    Name = Section[Index].Name;
    DEBUG ((
      DEBUG_VERBOSE,
      "SMM   Section - '%c%c%c%c%c%c%c%c'\n",
      Name[0],
      Name[1],
      Name[2],
      Name[3],
      Name[4],
      Name[5],
      Name[6],
      Name[7]
      ));

    if ((Section[Index].Characteristics & EFI_IMAGE_SCN_CNT_CODE) != 0) {
      DEBUG ((DEBUG_VERBOSE, "SMM   VirtualSize          - 0x%08x\n", Section[Index].Misc.VirtualSize));
      DEBUG ((DEBUG_VERBOSE, "SMM   VirtualAddress       - 0x%08x\n", Section[Index].VirtualAddress));
      DEBUG ((DEBUG_VERBOSE, "SMM   SizeOfRawData        - 0x%08x\n", Section[Index].SizeOfRawData));
      DEBUG ((DEBUG_VERBOSE, "SMM   PointerToRawData     - 0x%08x\n", Section[Index].PointerToRawData));
      DEBUG ((DEBUG_VERBOSE, "SMM   PointerToRelocations - 0x%08x\n", Section[Index].PointerToRelocations));
      DEBUG ((DEBUG_VERBOSE, "SMM   PointerToLinenumbers - 0x%08x\n", Section[Index].PointerToLinenumbers));
      DEBUG ((DEBUG_VERBOSE, "SMM   NumberOfRelocations  - 0x%08x\n", Section[Index].NumberOfRelocations));
      DEBUG ((DEBUG_VERBOSE, "SMM   NumberOfLinenumbers  - 0x%08x\n", Section[Index].NumberOfLinenumbers));
      DEBUG ((DEBUG_VERBOSE, "SMM   Characteristics      - 0x%08x\n", Section[Index].Characteristics));

      CodeMemory = (UINTN) ImageAddress + Section[Index].VirtualAddress;
      CodePages = EFI_SIZE_TO_PAGES(Section[Index].SizeOfRawData);
      DEBUG ((DEBUG_INFO, "SPS Code: 0x%016lx - 0x%016lx\n", CodeMemory, EFI_PAGES_TO_SIZE(CodePages)));

      CodeMemoryTemp = (UINTN) ImageAddressTemp + Section[Index].VirtualAddress;
      //
      // Allocate Code page for PE code.
      //
      Status = gSmst->SmmFreePages (CodeMemory, CodePages);
      ASSERT_EFI_ERROR (Status);
      Status = gSmst->SmmAllocatePages (AllocateAddress, EfiRuntimeServicesCode, CodePages, &CodeMemory);
      ASSERT_EFI_ERROR (Status);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "SmmAllocatePages Allocation failed\n"));
        continue;
      }
      //
      // SmmFreePages corrupts first 24 bytes in CodeMemory.
      // To recover it, copying back from the parent buffer as a WA.
      //
      CopyMem ((VOID *) CodeMemory, (VOID *) CodeMemoryTemp, 24);
    }
  }
}

/**
  Allocates buffer for Exception stack and updates supervisor exception stack address in TSS

  @param[in] CpuIndex          The processor index.
  @param[in] TssBase           Base address of Tast State Segment.
**/
VOID
InitRing3ModeGdt (
  IN UINTN CpuIndex,
  IN VOID  *TssBase
  )
{

  if (gSmmDgrExceptionStack == NULL) {
    gSmmDgrExceptionStackSize = PcdGet32 (PcdCpuSmmStackSize);
    gSmmDgrExceptionStack = AllocatePages (EFI_SIZE_TO_PAGES(gSmmDgrExceptionStackSize * mNumberOfCpus));
    ASSERT (gSmmDgrExceptionStack != NULL);
  }

  DEBUG ((DEBUG_INFO,
    "ExceptionStack(%d) - Ring0: 0x%x, Ring3: 0x%x\n",
    CpuIndex,
    (UINTN)gSmmDgrExceptionStack + gSmmDgrExceptionStackSize * (CpuIndex + 1),
    (UINTN)gSmmDgrExceptionStack + gSmmDgrExceptionStackSize * (CpuIndex + 1) - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM)
    ));
  *(UINT64 *) ((UINTN) TssBase + TSS_X64_RSP0_OFFSET) = (UINTN) gSmmDgrExceptionStack + gSmmDgrExceptionStackSize * (CpuIndex + 1);

}

/**
  Initialize Gdt for all processors.
   +-----------------------------+-----------------------------+--------+-----------------------------------------+
   | GDT0 | TSS0 | IoMap0 | 0xFF | GDT1 | TSS1 | IoMap1 | 0xFF | ...... | GDT(n-1) | TSS(n-1) | IoMap(n-1) | 0xFF |
   +-----------------------------+-----------------------------+--------+-----------------------------------------+
**/
VOID
InitGdtDgr (
  VOID
  )
{
  UINTN                     Index;
  IA32_SEGMENT_DESCRIPTOR   *GdtDescriptor;
  UINTN                     TssBase;
  UINT8                     *GdtTssTables;
  UINTN                     GdtTableStepSize;

  //
  // For X64 SMM, we allocate separate GDT/TSS for each CPUs to avoid TSS load contention
  // on each SMI entry.
  //
  gSmmDgrGdtTssTableSize = (mSmmDgrSmiGdtr.Limit + 1 + FULL_TSS_SIZE + 7) & ~7; // 8 bytes aligned
  gSmmDgrGdtBufferSize = gSmmDgrGdtTssTableSize * mNumberOfCpus;

  GdtTssTables = (UINT8*) SmmFeatureAllocateCodePagesDgr (EFI_SIZE_TO_PAGES (gSmmDgrGdtBufferSize));
  ASSERT (GdtTssTables != NULL);
  if (GdtTssTables == NULL) {
    return ;
  }
  gSmmDgrGdtBuffer = (UINTN)GdtTssTables;
  GdtTableStepSize = gSmmDgrGdtTssTableSize;

  for (Index = 0; Index < mNumberOfCpus; Index++) {
    CopyMem (GdtTssTables + GdtTableStepSize * Index, (VOID*)(UINTN)mSmmDgrSmiGdtr.Base, mSmmDgrSmiGdtr.Limit + 1 + FULL_TSS_SIZE);

    //
    // Fixup TSS descriptors
    //
    TssBase = (UINTN)(GdtTssTables + GdtTableStepSize * Index + mSmmDgrSmiGdtr.Limit + 1);
    GdtDescriptor = (IA32_SEGMENT_DESCRIPTOR *) (TssBase) - 2;
    ASSERT (GdtDescriptor != NULL);
    if (GdtDescriptor == NULL) {
      return ;
    }
    GdtDescriptor->Bits.BaseLow = (UINT16)(UINTN)TssBase;
    GdtDescriptor->Bits.BaseMid = (UINT8)((UINTN)TssBase >> 16);
    GdtDescriptor->Bits.BaseHigh = (UINT8)((UINTN)TssBase >> 24);

    InitRing3ModeGdt (Index, (VOID *)TssBase);
  }

  return;
}

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
/**
  Software SMI callback for Ledger Island which is called from ACPI method.

  @param[in]      DispatchHandle    The unique handle assigned to this handler by SmiHandlerRegister().
  @param[in]      Context           Points to an optional handler context which was specified when the
                                    handler was registered.
  @param[in, out] CommBuffer        A pointer to a collection of data in memory that will
                                    be conveyed from a non-SMM environment into an SMM environment.
  @param[in, out] CommBufferSize    The size of the CommBuffer.

  @retval EFI_SUCCESS               The interrupt was handled successfully.

**/
EFI_STATUS
EFIAPI
LedgerIslandSmmCallback (
  IN EFI_HANDLE     DispatchHandle,
  IN CONST VOID     *Context,
  IN OUT VOID       *CommBuffer,
  IN OUT UINTN      *CommBufferSize
  )
{
  CopyMem ((VOID *)(UINTN)gLedgerIslandBuffer, (VOID *)(UINTN)gSmmLedgerIslandBuffer, PcdGet32 (PcdLedgerIslandBufferSize));
  return EFI_SUCCESS;
}

/**
  SMM Ledger Island callback function at gEfiSmmSwDispatch2ProtocolGuid.
  Updates the DGR NVS Area with Ledger Island Data

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   Notification handler runs successfully.
  @return Others        Other error as indicated.
**/
EFI_STATUS
EFIAPI
SmmLedgerIslandNvsInit (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  ) {
  EFI_SMM_SW_DISPATCH2_PROTOCOL     *SwDispatch;
  EFI_SMM_SW_REGISTER_CONTEXT       SwContext;
  EFI_HANDLE                        SwHandle;
  DGR_NVS_AREA_PROTOCOL             *DgrNvsAreaProtocol;
  EFI_STATUS                        Status;

  SwDispatch = NULL;

  Status = gBS->LocateProtocol(&gEfiSmmBase2ProtocolGuid, NULL, (VOID **)&gSMM);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gSMM->GetSmstLocation (gSMM, &mSmst);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = mSmst->SmmLocateProtocol (&gEfiSmmSwDispatch2ProtocolGuid, NULL, (VOID **) &SwDispatch);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // If SwSmiInputValue is set to (UINTN) -1 then a unique value
  // will be assigned and returned in the structure.
  //
  SwContext.SwSmiInputValue = (UINTN) -1;
  Status = SwDispatch->Register (SwDispatch, LedgerIslandSmmCallback, &SwContext, &SwHandle);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = gBS->LocateProtocol (&gDgrNvsAreaProtocolGuid, NULL, (VOID **) &DgrNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DgrNvsAreaProtocol->Area->LedgerIslandMemAddress     = gLedgerIslandBuffer;
  DEBUG ((DEBUG_INFO, "DgrNvsAreaProtocol LedgerIslandMemAddress = 0x%x\n", DgrNvsAreaProtocol->Area->LedgerIslandMemAddress));
  DgrNvsAreaProtocol->Area->LedgerIslandSwSmi          = (UINT8) SwContext.SwSmiInputValue;
  DEBUG ((DEBUG_INFO, "DgrNvsAreaProtocol LedgerIslandSwSmi = 0x%x\n", DgrNvsAreaProtocol->Area->LedgerIslandSwSmi));

  return EFI_SUCCESS;
}
#endif

/**
  Constructor for SPS
**/
VOID
SpsConstructor (
  VOID
  )
{
  EFI_STATUS              Status;
  VOID                    *SmmEntryPointBuffer;
  UINTN                   SmmEntryPointBufferSize;
  VOID                    *SpsBin = NULL;
  SPS_ENTRY_POINT         SpsEntryPoint;
  SPS_STATUS              SpsStatus;
  SPS_BIOS_CONTEXT        BiosContext;
  UINTN                   Index;
  TXT_INFO_HOB            *HobList = NULL;
  TXT_INFO_DATA           *TxtInfoData = NULL;
  EFI_GUID                *SmmEntryPointFileGuid;
  EFI_GUID                *SpsFileGuid;
#if FixedPcdGetBool (PcdSpaEnable) == 1
  DXE_CPU_POLICY_PROTOCOL *CpuPolicyData;
#endif
#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
  VOID                    *Registration;
#endif

  DEBUG ((DEBUG_INFO, "***** SPS Constructor. Extracts binaries from FV and initializes them *****\n"));

#if FixedPcdGetBool (PcdSpaEnable) == 1
  //
  // Get Policy settings for SPA
  //
  Status = gBS->LocateProtocol (&gDxeCpuPolicyProtocolGuid, NULL, (VOID **) &CpuPolicyData);
  ASSERT_EFI_ERROR (Status);
  if ((!EFI_ERROR (Status)) && (CpuPolicyData->DgrSpaEnable)) {
  //
  // Register EFI_SMM_READY_TO_LOCK_PROTOCOL_GUID notify function.
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmReadyToLockProtocolGuid,
                    SpaSmmReadyToLockCallback,
                    &gSmmDgrSpaReadyToLockRegistration
                    );
  ASSERT_EFI_ERROR (Status);

  gSmmDgrSpaEnable = TRUE;
  DEBUG ((DEBUG_INFO, "*** SPA Enabled. Will load SPA binary instead of SPS ***\n"));
  }
#endif

  //
  // Extract SmmEntryPoint binary from FV
  //
  SmmEntryPointBuffer = NULL;
  SmmEntryPointBufferSize = 0;

  SmmEntryPointFileGuid = PcdGetPtr (PcdSpsSmmEntryPointBinFile);
#if FixedPcdGetBool (PcdSpaEnable) == 1
  if (gSmmDgrSpaEnable) {
    SmmEntryPointFileGuid = PcdGetPtr (PcdSpaSmmEntryPointBinFile);
  }
#endif
  Status = GetSectionFromAnyFv (
             SmmEntryPointFileGuid,
             EFI_SECTION_RAW,
             0,
             &SmmEntryPointBuffer,
             &SmmEntryPointBufferSize
             );
  if (EFI_ERROR (Status) || (SmmEntryPointBufferSize == 0)) {
    DEBUG ((DEBUG_ERROR, "      Failed to get SMM Entry Point Buffer from FV\n"));
    ASSERT_EFI_ERROR (Status);
    return;
  }

  mSmmDgrEntryPointHeader = (VOID *) (UINTN) SmmFeatureAllocateCodePagesDgr (EFI_SIZE_TO_PAGES(SmmEntryPointBufferSize));
  if (mSmmDgrEntryPointHeader == NULL) {
    ASSERT (mSmmDgrEntryPointHeader != NULL);
    goto CleanUp2;
  }

  CopyMem ((VOID *) (UINTN) mSmmDgrEntryPointHeader, SmmEntryPointBuffer, SmmEntryPointBufferSize);
  DEBUG ((DEBUG_INFO, "      mSmmDgrEntryPointHeader - 0x%x\n", mSmmDgrEntryPointHeader));
  mSmmDgrEntryPointInfoTable = (VOID *) ((UINT8 *) mSmmDgrEntryPointHeader + mSmmDgrEntryPointHeader->InfoTableAddress);
  DEBUG ((DEBUG_INFO, "      mSmmDgrEntryPointInfoTable - 0x%x\n", mSmmDgrEntryPointInfoTable));
  mSmmDgrEntryPointHeader->InfoTableAddress = (UINT32)(UINTN)mSmmDgrEntryPointInfoTable;
  gBS->FreePool ((VOID *) ((UINTN) SmmEntryPointBuffer));

  //
  // Extract SPS or SPA binary from FV
  //
  gSpsBinSize = 0;

  SpsFileGuid = PcdGetPtr (PcdSpsBinFile);
#if FixedPcdGetBool (PcdSpaEnable) == 1
  if (gSmmDgrSpaEnable) {
    SpsFileGuid = PcdGetPtr (PcdSpaBinFile);
  }
#endif

  Status = GetSectionFromAnyFv (
             SpsFileGuid,
             EFI_SECTION_RAW,
             0,
             &SpsBin,
             &gSpsBinSize
             );
  if (EFI_ERROR (Status) || (gSpsBinSize == 0)) {
    DEBUG ((DEBUG_ERROR, "      Failed to get Binary image from FV\n"));
    ASSERT_EFI_ERROR (Status);
    goto CleanUp2;
  }

  gSpsBin = (VOID *) (UINTN) AllocatePages (EFI_SIZE_TO_PAGES (gSpsBinSize));
  if (gSpsBin == NULL) {
    ASSERT (gSpsBin != NULL);
    goto CleanUp2;
  }
  ZeroMem ((VOID *) (UINTN) gSpsBin, EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (gSpsBinSize)));
  CopyMem ((VOID *) (UINTN) gSpsBin, SpsBin, gSpsBinSize);
  DEBUG ((DEBUG_INFO, "      Binary image copied at - 0x%x\n", gSpsBin));

  ConvertCodePage (gSpsBin, gSpsBinSize, SpsBin);

  gBS->FreePool ((VOID *) ((UINTN) SpsBin));

  //
  // Initialize SPS / SPA
  //
  BiosContext.ImageBase = gSpsBin;
  BiosContext.CodeSegment = LONG_MODE_CS;
  BiosContext.TaskSegment = TSS_SEGMENT;

  Status = PeCoffLoaderGetEntryPoint (gSpsBin, (VOID **) &SpsEntryPoint);
  ASSERT_EFI_ERROR(Status);
  DEBUG ((DEBUG_INFO, "      Image EntryPoint - 0x%x\n", SpsEntryPoint));

  SpsStatus = SpsEntryPoint (&BiosContext, &mSpsKernelContext);
  DEBUG ((DEBUG_INFO, "      Image Initialize Status - 0x%x\n", SpsStatus));
  DEBUG ((DEBUG_INFO, "      Image KernelContext - 0x%x\n", mSpsKernelContext));
  for (Index = 0; Index < ARRAY_SIZE (mSpsKernelContext->SpsExceptionEntryPoint); Index++) {
    DEBUG ((DEBUG_INFO, "      Image ExceptionEntryPoint[%2d] - 0x%x\n", Index, mSpsKernelContext->SpsExceptionEntryPoint [Index]));
  }
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_VPRO_DGR_SPS_BIN_LOADED);

  ///
  /// Get TXT DPR Base address and size from HOB.
  /// TXT DPR & Heap memory will be write protected at SMM ready to Lock.
  ///
  HobList = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (HobList != NULL) {
    TxtInfoData = &HobList->Data;
    if ((TxtInfoData                   != 0) &&
        (TxtInfoData->TxtMode          != 0) &&
        (TxtInfoData->TxtDprMemoryBase != 0) &&
        (TxtInfoData->TxtDprMemorySize != 0)
       ) {
      gSmmDgrTxtDprMemoryBase = TxtInfoData->TxtDprMemoryBase;
      gSmmDgrTxtDprMemorySize = TxtInfoData->TxtDprMemorySize;
    }
  }

#if FixedPcdGetBool (PcdSpaEnable) == 1
  if (gSmmDgrSpaEnable) {
    gSmmDgrSpaCtxt = AllocateZeroPool (sizeof (SPA_CTXT));
    if (gSmmDgrSpaCtxt == NULL) {
      ASSERT_EFI_ERROR (FALSE);
      DEBUG ((DEBUG_ERROR, "Failed to allocate buffer for SPA context\n"));
      goto CleanUp2;
    }
  }
#endif

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
  if (gSmmDgrEnableState != NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND) {
    DEBUG ((DEBUG_INFO, "Ledger Island not supported.\n"));
    return;
  }

  // Allocate 4K (1 page) for PPAM Param Page
  gSmmPpamParamPage = (EFI_PHYSICAL_ADDRESS) (UINTN) SmmFeatureAllocateCodePagesDgr (1);
  if (gSmmPpamParamPage == (EFI_PHYSICAL_ADDRESS) NULL) {
    DEBUG ((DEBUG_ERROR, "Allocation of PPAM Param page failed.\n"));
    ASSERT (FALSE);
    return;
  }

  // Allocate Ledger Island SMM Buffer
  gSmmLedgerIslandBuffer = (EFI_PHYSICAL_ADDRESS) (UINTN) SmmFeatureAllocateCodePagesDgr (EFI_SIZE_TO_PAGES (PcdGet32 (PcdLedgerIslandBufferSize)));
  if (gSmmLedgerIslandBuffer == (EFI_PHYSICAL_ADDRESS) NULL) {
    DEBUG ((DEBUG_ERROR, "Allocation of SMM buffer for Ledger Island failed.\n"));
    ASSERT (FALSE);
    goto CleanUp1;
  }

  // Update the PPAM Param Page entries
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->Version             = 1;
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->DataBuffVersion     = 1;
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->BaseAddrBufferLower = (UINT32) (gSmmLedgerIslandBuffer & 0xFFFFFFFF);
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->BaseAddrBufferUpper = (UINT32) RShiftU64 (gSmmLedgerIslandBuffer, 32);
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->BufferEntrySize     = PcdGet32 (PcdLedgerIslandBufferSize);
  ((PPAM_PARAM_PAGE *)gSmmPpamParamPage)->TotalNumberEntry    = 1;

  // Allocate (non-SMM) Ledger Island Buffer
  Status = gBS->AllocatePages (AllocateAnyPages, EfiReservedMemoryType, EFI_SIZE_TO_PAGES (PcdGet32 (PcdLedgerIslandBufferSize)), &gLedgerIslandBuffer);
  if (EFI_ERROR (Status) || gLedgerIslandBuffer == (EFI_PHYSICAL_ADDRESS) NULL) {
    DEBUG ((DEBUG_ERROR, "Allocation of buffer for Ledger Island failed.\n"));
    ASSERT_EFI_ERROR (Status);
    goto CleanUp1;
  }

  //
  //
  // Register SMM Event based on gEfiSmmSwDispatch2ProtocolGuid
  //
  Status = gSmst->SmmRegisterProtocolNotify (
                    &gEfiSmmSwDispatch2ProtocolGuid,
                    SmmLedgerIslandNvsInit,
                    &Registration
                    );
  ASSERT_EFI_ERROR (Status);
  if (EFI_ERROR (Status)) {
    goto CleanUp1;
  }
  return;

CleanUp1: // Free Ledger Island resources due to error and exit the function. ISSR/ISRD is still expected to work.
  if (gSmmPpamParamPage != (EFI_PHYSICAL_ADDRESS) NULL) {
    Status = gSmst->SmmFreePages (gSmmPpamParamPage, 1);
  }
  if (gSmmLedgerIslandBuffer != (EFI_PHYSICAL_ADDRESS) NULL) {
    Status = gSmst->SmmFreePages (gSmmLedgerIslandBuffer, EFI_SIZE_TO_PAGES (PcdGet32 (PcdLedgerIslandBufferSize)));
  }
#endif
  return;

CleanUp2:
  if (SmmEntryPointBuffer != NULL) {
    gBS->FreePool ((VOID *) ((UINTN) SmmEntryPointBuffer));
  }
  if (mSmmDgrEntryPointHeader != NULL) {
    Status = gSmst->SmmFreePages ((EFI_PHYSICAL_ADDRESS)mSmmDgrEntryPointHeader, EFI_SIZE_TO_PAGES(SmmEntryPointBufferSize));
  }
#if FixedPcdGetBool (PcdSpaEnable) == 1
  if (gSmmDgrSpaCtxt != NULL) {
    gBS->FreePool (gSmmDgrSpaCtxt);
  }
#endif
  if (SpsBin != NULL) {
    gBS->FreePool (SpsBin);
  }
  if (gSpsBin != NULL) {
    gBS->FreePages ((EFI_PHYSICAL_ADDRESS)gSpsBin, EFI_SIZE_TO_PAGES (gSpsBinSize));
  }
}

/**
  Fill in IDT with SPS Kernel Context

  @param[in] IdtBase  Points to IDT Base
  @param[in] IdtSize  IDT Size
**/
VOID
SpsPatchIdt (
  IN VOID *IdtBase,
  IN UINTN IdtSize
  )
{
  UINTN                     Index;
  IA32_IDT_GATE_DESCRIPTOR  *IdtEntry;

  IdtEntry = IdtBase;

  for (Index = 0; Index < ARRAY_SIZE(mSpsKernelContext->SpsExceptionEntryPoint); Index++) {
    IdtEntry[Index].Bits.Selector       = LONG_MODE_CS;
    IdtEntry[Index].Bits.OffsetLow      = (UINT16)(mSpsKernelContext->SpsExceptionEntryPoint[Index]);
    IdtEntry[Index].Bits.OffsetHigh     = (UINT16)RShiftU64 (mSpsKernelContext->SpsExceptionEntryPoint[Index], 16);
    IdtEntry[Index].Bits.OffsetUpper    = (UINT32)RShiftU64 (mSpsKernelContext->SpsExceptionEntryPoint[Index], 32);
    IdtEntry[Index].Bits.GateType       = IdtEntry[Index].Bits.GateType | IA32_DPL(3);
    // clear IST
    IdtEntry[Index].Bits.Reserved_0 = 0;
  }
}

/**
  Return the size, in bytes, of a custom SMI Handler in bytes.  If 0 is
  returned, then a custom SMI handler is not provided by this library,
  and the default SMI handler must be used.

  @retval 0    Use the default SMI handler.
  @retval > 0  Use the SMI handler installed by SmmCpuFeaturesInstallSmiHandler()
               The caller is required to allocate enough SMRAM for each CPU to
               support the size of the custom SMI handler.
**/
UINTN
EFIAPI
SmmCpuFeaturesGetSmiHandlerSizeSps (
  VOID
  )
{
  return mSmmDgrEntryPointInfoTable->SmmEntryPointSize;
}

/**
  Install a custom SMI handler for the CPU specified by CpuIndex.  This function
  is only called if SmmCpuFeaturesGetSmiHandlerSize() returns a size is greater
  than zero and is called by the CPU that was elected as monarch during System
  Management Mode initialization.

  @param[in] CpuIndex   The index of the CPU to install the custom SMI handler.
                        The value must be between 0 and the NumberOfCpus field
                        in the System Management System Table (SMST).
  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] SmiStack   The stack to use when an SMI is processed by the
                        the CPU specified by CpuIndex.
  @param[in] StackSize  The size, in bytes, if the stack used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtBase    The base address of the GDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] GdtSize    The size, in bytes, of the GDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtBase    The base address of the IDT to use when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] IdtSize    The size, in bytes, of the IDT used when an SMI is
                        processed by the CPU specified by CpuIndex.
  @param[in] Cr3        The base address of the page tables to use when an SMI
                        is processed by the CPU specified by CpuIndex.
**/
VOID
EFIAPI
SmmCpuFeaturesInstallSmiHandlerSps (
  IN UINTN   CpuIndex,
  IN UINT32  SmBase,
  IN VOID    *SmiStack,
  IN UINTN   StackSize,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize,
  IN UINTN   IdtBase,
  IN UINTN   IdtSize,
  IN UINT32  Cr3
  )
{
  EFI_STATUS           Status;
  IA32_PROT_DESCRIPTOR GdtDesc;
  UINT32               SmmDgrStackSize;
  UINT32               ExceptionStack;
  UINT32               ExceptionStackSize;
  UINT32               UserExceptionStack;
  UINT32               UserExceptionStackSize;
  UINT64               UserExceptionEntry;
  UINT64               Xcr0Data;
  UINT64               XssData;
  UINT64               DgrSmiRendezvous;

  DEBUG ((DEBUG_INFO, "SmmCpuFeaturesInstallSmiHandlerSps (%d)\n", CpuIndex));

  if (!gSmmDgrGdtFlag) {
    gSmmDgrCpuStackArrayBase = (UINTN)SmiStack;
    gSmmDgrCpuStackSize = StackSize;

    //
    // DGR feature is overriding the GDT built in PiSmmCpuDxeSmm. So, below code is freeing
    // the GDT buffer build in PiSmmCpuDxeSmm and create a new GDT with full TSS (which includes IO Bitmap)
    // When EDKII open source is updated with GDT to support user mode descriptors and full TSS, then below
    // GDT override can be removed.
    //
    Status = gSmst->SmmFreePages (GdtBase, EFI_SIZE_TO_PAGES (((GdtSize + TSS_SIZE + 7) & ~7) * mNumberOfCpus));
    ASSERT_EFI_ERROR (Status);
    if (EFI_ERROR (Status)) {
      return;
    }
    InitGdtDgr ();
    gSmmDgrGdtFlag = TRUE;
  }
  GdtBase = (UINTN)(gSmmDgrGdtBuffer + gSmmDgrGdtTssTableSize * CpuIndex);
  GdtSize = mSmmDgrSmiGdtr.Limit + 1;

  if (mSmmDgrProtectedModeEnable) {
    //
    // Initialize protected mode IDT
    //
    InitProtectedModeIdtDgr (Cr3);
  }

  //
  // Initialize values in template before copy
  //
  gSmmDgrFeatureSmiStack  = (UINT32)((UINTN)SmiStack + StackSize - sizeof (UINTN));
  SmmDgrStackSize            = (UINT32)(EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM) - sizeof (UINTN));
  //
  // Initialize Nested flag location with value 0. Nested flag is used by supervisor mode exception handler
  //
  *(UINT32 *)((UINTN)gSmmDgrFeatureSmiStack - SmmDgrStackSize + 4) = 0;
  gSmmDgrFeatureSmiUserStack = gSmmDgrFeatureSmiStack - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM);
  gSmmDgrUserStackSize       = (UINT32)(StackSize - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM) - sizeof (UINTN));
  DEBUG ((DEBUG_INFO, "Stack(%d) - Ring0: 0x%x, Ring3: 0x%x\n", CpuIndex, (UINTN)gSmmDgrFeatureSmiStack, (UINTN)gSmmDgrFeatureSmiUserStack));
  gSmmDgrFeatureSmiCr3       = Cr3;
  gSmmDgrFeatureSmiHandlerIdtr.Base = IdtBase;
  gSmmDgrFeatureSmiHandlerIdtr.Limit = (UINT16)(IdtSize - 1);
  GdtDesc.Base = (UINT32)GdtBase;
  GdtDesc.Limit = (UINT16)(GdtSize - 1);

  //
  // Set the value at the top of the CPU stack to the CPU Index
  //
  *(UINTN*)(UINTN)gSmmDgrFeatureSmiStack = CpuIndex;
  *(UINTN*)(UINTN)gSmmDgrFeatureSmiUserStack = CpuIndex;

  *(UINTN*)(UINTN)gSmmDgrFeatureSmiStack = CpuIndex;

  DgrSmiRendezvous              = (UINTN)AsmDgrSmiRendezvous;
  UserExceptionEntry            = (UINTN)AsmOemExceptionHandler;
  ExceptionStack                = (UINT32)(UINTN)(gSmmDgrExceptionStack + gSmmDgrExceptionStackSize * (CpuIndex + 1));
  ExceptionStackSize            = EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM);
  UserExceptionStack            = (UINT32)(UINTN)(gSmmDgrExceptionStack + gSmmDgrExceptionStackSize * (CpuIndex + 1) - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM));
  UserExceptionStackSize        = (UINT32)(gSmmDgrExceptionStackSize - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM));

  //
  // Fix IDT
  //
  SpsPatchIdt ((VOID *)IdtBase, IdtSize);

  InitializeIoMsrBitmap (CpuIndex, GdtBase, GdtSize);
  Xcr0Data = 3;
  XssData = 0;

  if (mSmmDgrStateSaveEnable == TRUE) {
    gSmmDgrSupervisorStateSave = 1;
  }

  //
  // Patch EntryPoint based upon InfoTable Entry
  //
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_GDT, &GdtDesc, sizeof(GdtDesc));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_IDT, &gSmmDgrFeatureSmiHandlerIdtr, sizeof(gSmmDgrFeatureSmiHandlerIdtr));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_CR3, &gSmmDgrFeatureSmiCr3, sizeof(gSmmDgrFeatureSmiCr3));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_SUPOVR_STATE_LOCK, &gSmmDgrSupovrStateLockData, sizeof(gSmmDgrSupovrStateLockData));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_STACK, (VOID *)&gSmmDgrFeatureSmiStack, sizeof(gSmmDgrFeatureSmiStack));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_STACK_SIZE, &SmmDgrStackSize, sizeof(SmmDgrStackSize));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_EXCEPTION_STACK, (VOID *)&ExceptionStack, sizeof(ExceptionStack));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_EXCEPTION_STACK_SIZE, &ExceptionStackSize, sizeof(ExceptionStackSize));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_SPS, &gSpsBin, sizeof(gSpsBinSize));

  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_ENABLE, &gSmmDgrRing3Supported, sizeof(gSmmDgrRing3Supported));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_ENTRY_POINT, (VOID *)&DgrSmiRendezvous, sizeof(DgrSmiRendezvous));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_STACK, (VOID *)&gSmmDgrFeatureSmiUserStack, sizeof(gSmmDgrFeatureSmiUserStack));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_STACK_SIZE, &gSmmDgrUserStackSize, sizeof(gSmmDgrUserStackSize));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_EXCEPTION_ENTRY_POINT, (VOID *)&UserExceptionEntry, sizeof(UserExceptionEntry));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_EXCEPTION_STACK, &UserExceptionStack, sizeof(UserExceptionStack));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_EXCEPTION_STACK_SIZE, &UserExceptionStackSize, sizeof(UserExceptionStackSize));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_MSR_BITMAP, &gSmmDgrMsrBitMapBase, sizeof(gSmmDgrMsrBitMapBase));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_XCR0, &Xcr0Data, sizeof(Xcr0Data));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_USER_MODE_MSR_IA32_XSS, &XssData, sizeof(XssData));
  PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_SUPERVISOR_STATE_SAVE, &gSmmDgrSupervisorStateSave, sizeof (gSmmDgrSupervisorStateSave));

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
  if (gSmmDgrEnableState == NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND) {
    PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_PPAM_PARAM_PAGE, &gSmmPpamParamPage, sizeof (gSmmPpamParamPage));
  }
#endif

#if FixedPcdGetBool (PcdSpaEnable) == 1
  if (gSmmDgrSpaEnable) {
    PatchSmmEntryPoint (mSmmDgrEntryPointHeader, SMM_ENTRY_POINT_INFO_SPA_CTXT, &gSmmDgrSpaCtxt, sizeof (gSmmDgrSpaCtxt));
  }
#endif

  //
  // Copy template to CPU specific SMI handler location
  //
  CopyMem (
    (VOID*)((UINTN)SmBase + SMM_HANDLER_OFFSET),
    (VOID*)mSmmDgrEntryPointHeader,
    mSmmDgrEntryPointInfoTable->SmmEntryPointSize
    );

  //
  // Copy template to CPU specific SMI handler location
  //
  if (mSmmDgrProtectedModeEnable) {
    //
    // Prepare for the SMMSEG structure
    //
    SetupSmmProtectedModeContext (
      CpuIndex,
      (UINT32)SmBase,
      gSmmDgrFeatureSmiStack,
      GdtBase,
      GdtSize,
      SMMSEG_PROTECT_MODE_CODE_SEGMENT,
      (IA32_DESCRIPTOR *)(UINTN)gSmmDgrProtModeIdtr
      );
  }

  if (!mPpamConfigurationTableInitialized) {
    PpamSmmConfigurationTableInit();
    mPpamConfigurationTableInitialized = TRUE;
  }

}

/**
  Final function which sets up User or Supervisor page access.

  @param[in]  PageEntry   Address of the page entry.
  @param[in]  PageAccessRight      User or Supervisor page access right.
  @param[out] IsModified           Set to 1 if the access has been changes, 0 if no change to the attribute
**/
VOID
ConvertPageEntryAccessRight (
  IN  UINT64                            *PageEntry,
  IN  PAGE_ACCESS_RIGHT                 PageAccessRight,
  OUT BOOLEAN                           *IsModified
  )
{
  UINT64  CurrentPageEntry;
  UINT64  NewPageEntry;

  CurrentPageEntry = *PageEntry;
  NewPageEntry = CurrentPageEntry;
  if (PageAccessRight == PageAccessRightSupervisor) {
    NewPageEntry &= ~(UINT64)IA32_PG_U;
  } else {
    NewPageEntry |= IA32_PG_U;
  }
  *PageEntry = NewPageEntry;
  if (CurrentPageEntry != NewPageEntry) {
    *IsModified = TRUE;
    DEBUG ((DEBUG_INFO, "ConvertPageEntryAccessRight 0x%lx", CurrentPageEntry));
    DEBUG ((DEBUG_INFO, "->0x%lx\n", NewPageEntry));
  } else {
    *IsModified = FALSE;
  }
}

PAGE_ATTRIBUTE_TABLE gPageAttributeTable[] = {
  {Page4K,  SIZE_4KB, PAGING_4K_ADDRESS_MASK_64},
  {Page2M,  SIZE_2MB, PAGING_2M_ADDRESS_MASK_64},
  {Page1G,  SIZE_1GB, PAGING_1G_ADDRESS_MASK_64},
};

/**
  Return page table base.

  @return page table base.
**/
UINTN
DgrGetPageTableBase (
  VOID
  )
{
  return (AsmReadCr3 () & PAGING_4K_ADDRESS_MASK_64);
}

/**
  Return length according to page attributes.

  @param[in]  PageAttributes   The page attribute of the page entry.

  @return The length of page entry.
**/
UINTN
DgrPageAttributeToLength (
  IN PAGE_ATTRIBUTE  PageAttribute
  )
{
  UINTN  Index;
  for (Index = 0; Index < sizeof(gPageAttributeTable)/sizeof(gPageAttributeTable[0]); Index++) {
    if (PageAttribute == gPageAttributeTable[Index].Attribute) {
      return (UINTN)gPageAttributeTable[Index].Length;
    }
  }
  return 0;
}

/**
  Return page table entry to match the address.

  @param[in]   Address          The address to be checked.
  @param[out]  PageAttributes   The page attribute of the page entry.

  @return The page entry.
**/
VOID *
DgrGetPageTableEntry (
  IN  PHYSICAL_ADDRESS                  Address,
  OUT PAGE_ATTRIBUTE                    *PageAttribute
  )
{
  UINTN                 Index1;
  UINTN                 Index2;
  UINTN                 Index3;
  UINTN                 Index4;
  UINT64                *L1PageTable;
  UINT64                *L2PageTable;
  UINT64                *L3PageTable;
  UINT64                *L4PageTable;

  Index4 = ((UINTN)RShiftU64 (Address, 39)) & PAGING_PAE_INDEX_MASK;
  Index3 = ((UINTN)RShiftU64 (Address, 30)) & PAGING_PAE_INDEX_MASK;
  Index2 = ((UINTN)RShiftU64 (Address, 21)) & PAGING_PAE_INDEX_MASK;
  Index1 = ((UINTN)RShiftU64 (Address, 12)) & PAGING_PAE_INDEX_MASK;

  if (sizeof (UINTN) == sizeof (UINT64)) {
    L4PageTable = (UINT64 *) DgrGetPageTableBase ();
    if (L4PageTable[Index4] == 0) {
      *PageAttribute = PageNone;
      return NULL;
    }

    L3PageTable = (UINT64 *)(UINTN)(L4PageTable[Index4] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  } else {
    L3PageTable = (UINT64 *)DgrGetPageTableBase ();
  }

  if (L3PageTable[Index3] == 0) {
    *PageAttribute = PageNone;
    return NULL;
  }

  if ((L3PageTable[Index3] & IA32_PG_PS) != 0) {
    // 1G
    *PageAttribute = Page1G;
    return &L3PageTable[Index3];
  }

  L2PageTable = (UINT64 *)(UINTN)(L3PageTable[Index3] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  if (L2PageTable[Index2] == 0) {
    *PageAttribute = PageNone;
    return NULL;
  }
  if ((L2PageTable[Index2] & IA32_PG_PS) != 0) {
    // 2M
    *PageAttribute = Page2M;
    return &L2PageTable[Index2];
  }

  // 4k
  L1PageTable = (UINT64 *)(UINTN)(L2PageTable[Index2] & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
  if ((L1PageTable[Index1] == 0) && (Address != 0)) {
    *PageAttribute = PageNone;
    return NULL;
  }
  *PageAttribute = Page4K;
  return &L1PageTable[Index1];
}

/**
  Set Page attribute to Supervisor if available.

  @param[in]   Address     The page base address.
**/
VOID
DgrSetPageEntrySupervisor (
  IN  PHYSICAL_ADDRESS      Address
  )
{
  UINTN        Index4;
  UINTN        Index3;
  UINTN        Index2;
  UINTN        Index1;
  UINT64       *L4PageAddr;
  UINTN        L4PageEntry;
  UINT64       *L3PageAddr;
  UINTN        L3PageEntry;
  UINT64       *L2PageAddr;
  UINTN        L2PageEntry;
  UINT64       *L1PageAddr;
  BOOLEAN      MemAttrModified = FALSE;

  Index4 = ((UINTN) RShiftU64 (Address, 39)) & PAGING_PAE_INDEX_MASK;
  Index3 = ((UINTN) RShiftU64 (Address, 30)) & PAGING_PAE_INDEX_MASK;
  Index2 = ((UINTN) RShiftU64 (Address, 21)) & PAGING_PAE_INDEX_MASK;
  Index1 = ((UINTN) RShiftU64 (Address, 12)) & PAGING_PAE_INDEX_MASK;

  L4PageAddr = (UINT64 *) DgrGetPageTableBase ();
  L4PageEntry = L4PageAddr [Index4];

  if ((L4PageEntry & IA32_PG_P) != 0) {
    L3PageAddr = (UINT64 *)(UINTN) (L4PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
    L3PageEntry = L3PageAddr[Index3];

    if (((L3PageEntry & IA32_PG_P) != 0) && ((L3PageEntry & IA32_PG_PS) == 0)) {
      L2PageAddr = (UINT64 *)(UINTN)(L3PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);
      L2PageEntry = L2PageAddr[Index2];

      if (((L2PageEntry & IA32_PG_P) != 0) && ((L2PageEntry & IA32_PG_PS) == 0)) {
        L1PageAddr = (UINT64 *) (UINTN) (L2PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);

        if ((L2PageEntry & IA32_PG_RW) == 0) {
          L2PageAddr[Index2] |= (UINT64) IA32_PG_RW;
          MemAttrModified = TRUE;
        }

        // Set L1 page to supervisor
        L1PageAddr [Index1] &= ~(UINT64) IA32_PG_U;

        if (MemAttrModified) {
          L2PageAddr[Index2] &= ~(UINT64) IA32_PG_RW;
          MemAttrModified = FALSE;
        }
      }
    }
  }
}

/**
  Parse Page table as per CR3 and set Page table entry attributes to Supervisor.
**/
VOID
SmmSetPageTableEntriesAccessRight (
  VOID
  )
{
  UINTN            Index4;
  UINTN            Index3;
  UINTN            Index2;
  UINT64           *L4PageAddr;
  UINTN            L4PageEntry;
  UINT64           *L3PageAddr;
  UINTN            L3PageEntry;
  UINT64           *L2PageAddr;
  UINTN            L2PageEntry;

  L4PageAddr = (UINT64 *) DgrGetPageTableBase ();

  for (Index4 = 0; Index4 < MAX_PAGE_ENTRIES; Index4++) {
    L4PageEntry = L4PageAddr [Index4];
    if ((L4PageEntry & IA32_PG_P) != 0) {
      DgrSetPageEntrySupervisor ((PHYSICAL_ADDRESS) (L4PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64));
      L3PageAddr = (UINT64 *) (UINTN) (L4PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);

      for (Index3 = 0; Index3 < MAX_PAGE_ENTRIES; Index3++) {
        L3PageEntry = L3PageAddr [Index3];
        if (((L3PageEntry & IA32_PG_P) != 0) && ((L3PageEntry & IA32_PG_PS) == 0)) {
          DgrSetPageEntrySupervisor ((PHYSICAL_ADDRESS) (L3PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64));
          L2PageAddr = (UINT64 *) (UINTN) (L3PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64);

          for (Index2 = 0; Index2 < MAX_PAGE_ENTRIES; Index2++) {
            L2PageEntry = L2PageAddr [Index2];
            if (((L2PageEntry & IA32_PG_P) != 0) && ((L2PageEntry & IA32_PG_PS) == 0)) {
              DgrSetPageEntrySupervisor ((PHYSICAL_ADDRESS) (L2PageEntry & ~mAddressEncMask & PAGING_4K_ADDRESS_MASK_64));
            }
          }
        }
      }
    }
  }
}

/**
  This function returns if there is need to split page entry.

  @param[in]  BaseAddress      The base address to be checked.
  @param[in]  Length           The length to be checked.
  @param[in]  PageEntry        The page entry to be checked.
  @param[in]  PageAttribute    The page attribute of the page entry.

  @retval SplitAttributes on if there is need to split page entry.
**/
PAGE_ATTRIBUTE
DgrNeedSplitPage (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  UINT64                            *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute
  )
{
  UINT64                PageEntryLength;

  PageEntryLength = DgrPageAttributeToLength (PageAttribute);

  if (((BaseAddress & (PageEntryLength - 1)) == 0) && (Length >= PageEntryLength)) {
    return PageNone;
  }

  if (((BaseAddress & PAGING_2M_MASK) != 0) || (Length < SIZE_2MB)) {
    return Page4K;
  }

  return Page2M;
}

/**
  This function splits one page entry to small page entries.

  @param[in]  PageEntry        The page entry to be splitted.
  @param[in]  PageAttribute    The page attribute of the page entry.
  @param[in]  SplitAttribute   How to split the page entry.

  @retval RETURN_SUCCESS            The page entry is splitted.
  @retval RETURN_UNSUPPORTED        The page entry does not support to be splitted.
  @retval RETURN_OUT_OF_RESOURCES   No resource to split page entry.
**/
RETURN_STATUS
DgrSplitPage (
  IN  UINT64                            *PageEntry,
  IN  PAGE_ATTRIBUTE                    PageAttribute,
  IN  PAGE_ATTRIBUTE                    SplitAttribute
  )
{
  UINT64   BaseAddress;
  UINT64   *NewPageEntry;
  UINTN    Index;

  ASSERT (PageAttribute == Page2M || PageAttribute == Page1G);

  if (PageAttribute == Page2M) {
    //
    // Split 2M to 4K
    //
    ASSERT (SplitAttribute == Page4K);
    if (SplitAttribute == Page4K) {
      NewPageEntry = AllocatePageTableMemory (1);
      DEBUG ((DEBUG_INFO, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = *PageEntry & PAGING_2M_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_4KB * Index) | mAddressEncMask | ((*PageEntry) & PAGE_PROGATE_BITS);
      }
      (*PageEntry) = (UINT64)(UINTN)NewPageEntry | mAddressEncMask | PAGE_ATTRIBUTE_BITS;
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else if (PageAttribute == Page1G) {
    //
    // Split 1G to 2M
    // No need support 1G->4K directly, we should use 1G->2M, then 2M->4K to get more compact page table.
    //
    ASSERT (SplitAttribute == Page2M || SplitAttribute == Page4K);
    if ((SplitAttribute == Page2M || SplitAttribute == Page4K)) {
      NewPageEntry = AllocatePageTableMemory (1);
      DEBUG ((DEBUG_INFO, "Split - 0x%x\n", NewPageEntry));
      if (NewPageEntry == NULL) {
        return RETURN_OUT_OF_RESOURCES;
      }
      BaseAddress = *PageEntry & PAGING_1G_ADDRESS_MASK_64;
      for (Index = 0; Index < SIZE_4KB / sizeof(UINT64); Index++) {
        NewPageEntry[Index] = (BaseAddress + SIZE_2MB * Index) | mAddressEncMask | IA32_PG_PS | ((*PageEntry) & PAGE_PROGATE_BITS);
      }
      (*PageEntry) = (UINT64)(UINTN)NewPageEntry | mAddressEncMask | PAGE_ATTRIBUTE_BITS;
      return RETURN_SUCCESS;
    } else {
      return RETURN_UNSUPPORTED;
    }
  } else {
    return RETURN_UNSUPPORTED;
  }
}

/**
  Sets up page access right

  @param[in]  BaseAddress          Base address for the page that the access need to be changed.
  @param[in]  Length               Length of the memory range where page access need to be changed.
  @param[in]  PageAccessRight      User or Supervisor page access right.
  @param[out] IsModified           Set to 1 if the access has been changes, 0 if no change to the attribute
**/
RETURN_STATUS
EFIAPI
ConvertMemoryPageAccessRight (
  IN  PHYSICAL_ADDRESS                  BaseAddress,
  IN  UINT64                            Length,
  IN  PAGE_ACCESS_RIGHT                 PageAccessRight,
  OUT BOOLEAN                           *IsModified   OPTIONAL
  )
{
  UINT64                            *PageEntry;
  UINT64                            *ParentPageEntry;
  PAGE_ATTRIBUTE                    PageAttribute;
  PAGE_ATTRIBUTE                    ParentPageAttribute;
  UINTN                             PageEntryLength;
  PAGE_ATTRIBUTE                    SplitAttribute;
  RETURN_STATUS                     Status;
  BOOLEAN                           IsEntryModified;
  BOOLEAN                           MemAttrModified = FALSE;

  DEBUG ((DEBUG_INFO, "ConvertMemoryPageAccessRight - %016lx, %016lx, %x\n", BaseAddress, Length, PageAccessRight));
  ASSERT ((PageAccessRight == PageAccessRightSupervisor) || (PageAccessRight == PageAccessRightUser));
  ASSERT ((BaseAddress & (SIZE_4KB - 1)) == 0);
  ASSERT ((Length & (SIZE_4KB - 1)) == 0);

  if (Length == 0) {
    return RETURN_INVALID_PARAMETER;
  }

  if (IsModified != NULL) {
    *IsModified = FALSE;
  }

  //
  // Below logic is to check 2M/4K page to make sure we do not waste memory.
  //
  while (Length != 0) {
    PageEntry = DgrGetPageTableEntry (BaseAddress, &PageAttribute);

    if (PageEntry == NULL) {
      return RETURN_UNSUPPORTED;
    }

    PageEntryLength = DgrPageAttributeToLength (PageAttribute);
    SplitAttribute = DgrNeedSplitPage (BaseAddress, Length, PageEntry, PageAttribute);
    if (SplitAttribute == PageNone) {

      ParentPageEntry = DgrGetPageTableEntry ((PHYSICAL_ADDRESS)PageEntry, &ParentPageAttribute);
      ASSERT (ParentPageEntry != NULL);
      if (ParentPageEntry == NULL) {
        return RETURN_UNSUPPORTED;
      }
      if ((*ParentPageEntry & IA32_PG_RW) == 0) {
        *ParentPageEntry |= IA32_PG_RW;
        MemAttrModified = TRUE;
      }

      ConvertPageEntryAccessRight (PageEntry, PageAccessRight, &IsEntryModified);

      if (MemAttrModified) {
        *ParentPageEntry &= ~(UINT64)IA32_PG_RW;
        MemAttrModified = FALSE;
      }
      if (IsEntryModified) {
        if (IsModified != NULL) {
          *IsModified = TRUE;
        }
      }
      //
      // Convert success, move to next
      //
      BaseAddress += PageEntryLength;
      Length -= PageEntryLength;
    } else {
      Status = DgrSplitPage (PageEntry, PageAttribute, SplitAttribute);
      if (RETURN_ERROR (Status)) {
        return RETURN_UNSUPPORTED;
      }
      if (IsModified != NULL) {
        *IsModified = TRUE;
      }
      //
      // Just split current page
      // Convert success in next around
      //
    }
  }

  return RETURN_SUCCESS;
}

/**
  Sets up page access right

  @param[in] BaseAddress          Base address for the page that the access need to be changed.
  @param[in] Length               Length of the memory range where page access need to be changed.
  @param[in] PageAccessRight      User or Supervisor page access right.
**/
EFI_STATUS
EFIAPI
SmmSetPageAccessRight (
  IN  EFI_PHYSICAL_ADDRESS                       BaseAddress,
  IN  UINT64                                     Length,
  IN  PAGE_ACCESS_RIGHT                          PageAccessRight
  )
{
  BOOLEAN     IsModified;

  ConvertMemoryPageAccessRight (BaseAddress, Length, PageAccessRight, &IsModified);

  return EFI_SUCCESS;
}

/**
  Setup the page access rights within SMRAM based on ring separation policy requirements

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
**/
VOID
SetPageTableAccessRight (
  IN UINT32   *SmBase
  )
{
  EFI_STATUS Status;
  UINTN  Index;
  UINTN  StackBase;

  //
  // Set GDT table with Non-Executable
  //
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
             gSmmDgrMemoryAttribute,
             gSmmDgrGdtBuffer,
             EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (gSmmDgrGdtBufferSize)),
             EFI_MEMORY_XP
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Set MSR Bitmap policy page as Non-executable
  //
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
             gSmmDgrMemoryAttribute,
             gSmmDgrMsrBitMapBase,
             EFI_PAGES_TO_SIZE (1),
             EFI_MEMORY_XP
             );
  ASSERT_EFI_ERROR (Status);

  //
  // Set SMM Info Table page as Non-executable
  //
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
             gSmmDgrMemoryAttribute,
             (EFI_PHYSICAL_ADDRESS) mSmmDgrEntryPointHeader,
             EFI_PAGES_TO_SIZE (1),
             EFI_MEMORY_XP
             );
  ASSERT_EFI_ERROR (Status);

#if FixedPcdGetBool (PcdLedgerIslandEnable) == 1
  if (gSmmDgrEnableState == NR_PPAM_11_SUPPORT_WITH_LEDGER_ISLAND) {
    //
    // Set PPAM Param Page as Not Present
    //
    Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
               gSmmDgrMemoryAttribute,
               gSmmPpamParamPage,
               EFI_PAGES_TO_SIZE (1),
               EFI_MEMORY_RP
               );
    ASSERT_EFI_ERROR (Status);

    //
    // Set Ledger Island Buffers as Non-executable
    //
    Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
               gSmmDgrMemoryAttribute,
               gLedgerIslandBuffer,
               PcdGet32 (PcdLedgerIslandBufferSize),
               EFI_MEMORY_XP
               );
    ASSERT_EFI_ERROR (Status);
  
    Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
               gSmmDgrMemoryAttribute,
               gSmmLedgerIslandBuffer,
               PcdGet32 (PcdLedgerIslandBufferSize),
               EFI_MEMORY_XP
               );
    ASSERT_EFI_ERROR (Status);
  }
#endif

//
// Set Page tables with Ring0 (Supervisor) / Ring3 (User) Policies
//
  for (Index = 0; Index < mNumberOfCpus; Index++) {
    StackBase = gSmmDgrCpuStackArrayBase + Index * gSmmDgrCpuStackSize;
    // Supervisor Stack
    SmmSetPageAccessRight (
      StackBase + gSmmDgrCpuStackSize - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM),
      EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM),
      PageAccessRightSupervisor
      );
    // Known Good Stack - used for exception in Ring0
    SmmSetPageAccessRight (
      StackBase,
      EFI_PAGES_TO_SIZE(1),
      PageAccessRightUser
      );

    StackBase = (UINTN)gSmmDgrExceptionStack + Index * gSmmDgrExceptionStackSize;
    // Supervisor Exception Stack - used for exception in Ring3
    SmmSetPageAccessRight (
      StackBase + gSmmDgrExceptionStackSize - EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM),
      EFI_PAGES_TO_SIZE(SUPERVISOR_PAGE_NUM),
      PageAccessRightSupervisor
      );

    // Set supervisor attribute for SMM entry point code
    SmmSetPageAccessRight (
      SmBase[Index] + SMM_HANDLER_OFFSET,
      EFI_PAGES_TO_SIZE (1),
      PageAccessRightSupervisor
      );

    // Set Supervisor attribute for Save State Map page
    if (mSmmDgrStateSaveEnable == TRUE) {
      SmmSetPageAccessRight (
        SmBase[Index] + (SMRAM_SAVE_STATE_MAP_OFFSET & ~PAGING_4K_MASK),
        EFI_PAGES_TO_SIZE (1),
        PageAccessRightSupervisor
        );
    }
  }

  // Set supervisor attribute for SPS binary.
  SmmSetPageAccessRight (
    (EFI_PHYSICAL_ADDRESS) gSpsBin,
    gSpsBinSize,
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for GDT Descriptor Table (GDT, TSS)
  SmmSetPageAccessRight (
    gSmmDgrGdtBuffer,
    EFI_PAGES_TO_SIZE (EFI_SIZE_TO_PAGES (gSmmDgrGdtBufferSize)),
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for Interrupt Descriptor Table
  SmmSetPageAccessRight (
    gSmmDgrFeatureSmiHandlerIdtr.Base,
    EFI_PAGES_TO_SIZE(1),
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for MSR Bitmap
  SmmSetPageAccessRight (
    (EFI_PHYSICAL_ADDRESS) gSmmDgrMsrBitMapBase,
    EFI_PAGES_TO_SIZE (1),
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for CR3 Page
  SmmSetPageAccessRight (
    (EFI_PHYSICAL_ADDRESS) gSmmDgrFeatureSmiCr3,
    EFI_PAGES_TO_SIZE (1),
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for SMM Information Table
  SmmSetPageAccessRight (
    (EFI_PHYSICAL_ADDRESS) mSmmDgrEntryPointHeader,
    EFI_PAGES_TO_SIZE (1),
    PageAccessRightSupervisor
    );

  // Set supervisor attribute for SMM Page Table Entries
  SmmSetPageTableEntriesAccessRight ();
  //
  // Set page table itself to be read-only
  //
  SetPageTableAttributes ();
}

/**
  User mode exception handler. This handler can be called by SPS supervisor exception handler to log the exception contect.

  @param[in] InterruptType          Defines which interrupt or exception to hook.
  @param[in] SystemContext          Pointer to EFI_SYSTEM_CONTEXT.
**/
VOID
EFIAPI
OemExceptionHandler (
  IN CONST  EFI_EXCEPTION_TYPE  InterruptType,
  IN CONST  EFI_SYSTEM_CONTEXT  SystemContext
  )
{
  EFI_STATUS            Status;

  //
  // Post code 0x0B23 to represent SMM Resource Access Violation
  //
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_PCH_SMM_RESOURCE_ACCESS_VIOLATION); //PostCode (0xB23)

  //
  // Check for OEM Hook to transfer controler to that function to handle Exception.
  //
  if (gSpsRing3ExceptionHandlerProtocol != NULL) {
    Status = gSpsRing3ExceptionHandlerProtocol->SpsRing3ExceptionHandler (InterruptType, SystemContext);
    if (Status == EFI_SUCCESS) {
      return ;
    }
  }

  while (!AcquireSpinLockOrFail (mSmmDgrInternalDebugLock)) {
    ;
  }

  DEBUG ((DEBUG_INFO, "OemExceptionHandler ...\n"));
  DumpCpuContext (InterruptType, SystemContext);
  DEBUG ((DEBUG_INFO, "OemExceptionHandler Done\n"));
  ReleaseSpinLock (mSmmDgrInternalDebugLock);
  CpuDeadLoop ();
  return ;
}

/**
  Restrict write access for available VTD MMIO regions within SMM.
**/
VOID
RestrictSmmVtdMmioAccess (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_PHYSICAL_ADDRESS      VtdBase;
  EFI_PHYSICAL_ADDRESS      VtdBasePageTableEntry;
  UINT8                     Index;
  UINT8                     MaxVtdEngine;
  PAGE_ATTRIBUTE            PageAttribute;

  MaxVtdEngine = GetMaxVtdEngineNumber ();

  for (Index = 0; Index < MaxVtdEngine; Index++) {
    VtdBase = GetVtdBaseAddress (Index);
    DEBUG ((DEBUG_ERROR, "Vtd Engine%d Base Address =  0x%016lx\n", Index + 1, VtdBase));
    if (VtdBase != 0) {
      //
      // Restrict write access for Vtd region MMIO Page.
      //
      Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
                                      gSmmDgrMemoryAttribute,
                                      VtdBase,
                                      EFI_PAGES_TO_SIZE (1),
                                      EFI_MEMORY_RO
                                      );
      ASSERT_EFI_ERROR (Status);

      //
      // Get Vtd region MMIO Page Entry pointer and
      // restrict its page for write access and also set the page access right to Supervisor.
      //
      VtdBasePageTableEntry = (EFI_PHYSICAL_ADDRESS) DgrGetPageTableEntry (VtdBase, &PageAttribute);
      DEBUG ((DEBUG_ERROR, "Vtd Engine%d Base Page Table Entry =  0x%016lx\n", Index + 1, VtdBasePageTableEntry));
      if (VtdBasePageTableEntry == 0) {
        ASSERT (FALSE);
        return ;
      }

      VtdBasePageTableEntry &= ~(UINT64) EFI_PAGE_MASK;

      Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
                                      gSmmDgrMemoryAttribute,
                                      VtdBasePageTableEntry,
                                      EFI_PAGES_TO_SIZE (1),
                                      EFI_MEMORY_RO
                                      );
      ASSERT_EFI_ERROR (Status);

      SmmSetPageAccessRight (
        VtdBasePageTableEntry,
        EFI_PAGES_TO_SIZE (1),
        PageAccessRightSupervisor
        );

      DEBUG ((DEBUG_ERROR, "Vtd Engine%d MMIO page and its entry point Page Attributes are updated for Read Only\n", Index+1));
    }
  }

  return ;
}

/**
  Restrict write access for all TXT MMIO (TXT Private & Public Space).
  Also restrict Write access to its Page Table Entry and keep in supervisor access right.

  TXT Private Space 0xFED20000 - 0xFED2FFFF (64K = 16 * 4K = 16 Pages).
  TXT Public Space 0xFED30000 - 0xFED3FFFF (16 Pages).
**/
VOID
RestrictSmmTxtMmioAccess (
  VOID
  )
{
  EFI_STATUS                          Status;

  DEBUG ((DEBUG_INFO, "RestrictSmmTxtMmioAccess...\n"));

  //
  // Write Protect TXT Private space
  //
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
                                 gSmmDgrMemoryAttribute,
                                 TXT_PRIVATE_BASE,
                                 EFI_PAGE_SIZE * 16,
                                 EFI_MEMORY_RO
                                 );
  ASSERT_EFI_ERROR (Status);

  //
  // Write Protect TXT Public space
  //
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
                                 gSmmDgrMemoryAttribute,
                                 TXT_PUBLIC_BASE,
                                 EFI_PAGE_SIZE * 16,
                                 EFI_MEMORY_RO
                                 );
  ASSERT_EFI_ERROR (Status);

  return ;
}

/**
  Restrict System Memory write access being inside SMM.
**/
EFI_STATUS
RestrictSmmSystemMemoryAccess (
  VOID
  )
{
  EFI_STATUS                           Status;

  if ((gSmmDgrTxtDprMemoryBase == 0) || (gSmmDgrTxtDprMemorySize == 0)) {
    DEBUG ((DEBUG_INFO, "TXT DPR memory Base, Size values are not valid.\n"));
    return EFI_NOT_FOUND;
  }

  ///
  /// Write Protect TXT DPR Memory region (TXT Heap memory is within DPR Memory region).
  ///
  Status = gSmmDgrMemoryAttribute->SetMemoryAttributes (
                                 gSmmDgrMemoryAttribute,
                                 gSmmDgrTxtDprMemoryBase,
                                 gSmmDgrTxtDprMemorySize,
                                 EFI_MEMORY_RO
                                 );
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "Write protected TXT DPR memory from 0x%016lx, size of 0x%x\n", gSmmDgrTxtDprMemoryBase, gSmmDgrTxtDprMemorySize));
  return EFI_SUCCESS;
}

/**
  This function sets
   - Page access rights within SMRAM based on ring separation policy requirements.
   - MMIO access rights.
   - System Memory access rights which are being accessed from SMM.

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
**/
EFI_STATUS
SetSmmDgrPolicy (
  IN UINT32   *SmBase
  )
{
  EFI_STATUS                  Status;

  Status = gSmst->SmmLocateProtocol (
             &gEdkiiSmmMemoryAttributeProtocolGuid,
             NULL,
             (VOID **) &gSmmDgrMemoryAttribute
             );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to locate EdkiiSmmMemoryAttributeProtocol...\n"));
    return EFI_NOT_FOUND;
  }

  Status = gSmst->SmmLocateProtocol (
                    &gSpsRing3ExceptionHandlerProtocolGuid,
                    NULL,
                    (VOID **) &gSpsRing3ExceptionHandlerProtocol
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Failed to locate gSpsRing3ExceptionHandlerProtocol, Status - %r\n", Status));
  }

  ///
  /// Restrict MMIO write access for VTD and TXT.
  ///
  RestrictSmmVtdMmioAccess ();
  RestrictSmmTxtMmioAccess ();

  ///
  /// Restrict System Memory write access.
  ///
  RestrictSmmSystemMemoryAccess ();

  ///
  /// Set page table and page access rights.
  ///
  SetPageTableAccessRight (SmBase);

  return EFI_SUCCESS;
}
