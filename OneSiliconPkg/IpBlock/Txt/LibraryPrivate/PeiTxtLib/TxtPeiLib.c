/** @file
  This file contains an implementation of the function call interfaces
  required by the main TXT PEIM file. Hopefully, future platform porting
  tasks will be mostly limited to modifying the functions in this file.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <Library/TxtPeiLib.h>
#include <Library/CpuCommonLib.h>
#include <Library/TxtLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Register/Cpuid.h>
#include <Register/Msr.h>
#include <PchResetPlatformSpecific.h>
#include <Library/PmcLib.h>
#include <Library/LocalApicLib.h>
#include <Ppi/MpServices2.h>
#include <Register/GenerationMsr.h>
#include <TxtConfig.h>
#include <Library/PcieInitLib.h>
#include <Library/BootGuardLib.h>
#include <IntelRcStatusCode.h>
#include <Library/PeiServicesLib.h>


/**
  PPI to clean FastBoot flags.
**/
static EFI_PEI_PPI_DESCRIPTOR mPeiTxtCleanResetNotificationPpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiTxtCleanResetNotificationPpiGuid,
  NULL
};

/**
  This routine checks if List ID match is found

  @param[in] FirstChrPtr          - A pointer to first character array
  @param[in] SecondChrPtr         - A pointer to second character array
  @param[in] Length                - Length of characters to be compared

  @retval TRUE       - If match found
  @retval FALSE      - If no match found
**/
BOOLEAN
CompareId (
  IN CHAR8 *FirstChrPtr,
  IN CHAR8 *SecondChrPtr,
  IN UINT8 Length
  )
{
  UINT8 Ptr = 0;

  for (Ptr = 0; Ptr < Length; Ptr++, FirstChrPtr++, SecondChrPtr++) {
    if (*FirstChrPtr != *SecondChrPtr) {
      break;
    }
  }

  if (Length == Ptr) {
    return TRUE;
  }

  return FALSE;
}

/**
  This routine initializes and collects all PPIs and data required
  by the routines in this file.

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @exception EFI_UNSUPPORTED   - If any of the required PPIs or data are unavailable
  @retval EFI_SUCCESS       - In all cases not listed above
**/
EFI_STATUS
InitializeTxtPeiLib (
  IN TXT_INFO_HOB **TxtInfoHob
  )
{
  EFI_STATUS Status;
  UINTN      Ia32ApicBase;

  ///
  /// Find TxtInfoHob by platform code
  ///
  Status = CreateTxtInfoHob (TxtInfoHob);

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "TXTPEI: TXT Info Hob not found or TXT is not supported.\n"));
    return EFI_UNSUPPORTED;
  }
  ///
  /// Print out the TxtInfo HOB if TXT_DEBUG_INFO is set
  ///
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtInfoHob passed from platform as:\n"));
  DEBUG ((DEBUG_INFO, "TXTPEI: ChipsetIsTxtCapable   = %x\n", (*TxtInfoHob)->Data.ChipsetIsTxtCapable));
  DEBUG ((DEBUG_INFO, "TXTPEI: ProcessorIsTxtCapable = %x\n", (*TxtInfoHob)->Data.ProcessorIsTxtCapable));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtMode               = %x\n", (*TxtInfoHob)->Data.TxtMode));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtAcheckRequest      = %x\n", (*TxtInfoHob)->Data.TxtAcheckRequest));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtPowerdownRequest   = %x\n", (*TxtInfoHob)->Data.TxtPowerdownRequest));
  DEBUG ((DEBUG_INFO, "TXTPEI: ResetAux              = %x\n", (*TxtInfoHob)->Data.ResetAux));
  DEBUG ((DEBUG_INFO, "TXTPEI: AcpiBase              = %x\n", (*TxtInfoHob)->Data.AcpiBase));
  DEBUG ((DEBUG_INFO, "TXTPEI: SinitMemorySize       = %x\n", (*TxtInfoHob)->Data.SinitMemorySize));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtHeapMemorySize     = %x\n", (*TxtInfoHob)->Data.TxtHeapMemorySize));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtDprMemoryBase      = %x\n", (*TxtInfoHob)->Data.TxtDprMemoryBase));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtDprMemorySize      = %x\n", (*TxtInfoHob)->Data.TxtDprMemorySize));
  DEBUG ((DEBUG_INFO, "TXTPEI: BiosAcmBase           = %x\n", (*TxtInfoHob)->Data.BiosAcmBase));
  DEBUG ((DEBUG_INFO, "TXTPEI: BiosAcmSize           = %x\n", (*TxtInfoHob)->Data.BiosAcmSize));
  DEBUG ((DEBUG_INFO, "TXTPEI: SinitAcmBase          = %x\n", (*TxtInfoHob)->Data.SinitAcmBase));
  DEBUG ((DEBUG_INFO, "TXTPEI: SinitAcmSize          = %x\n", (*TxtInfoHob)->Data.SinitAcmSize));
  DEBUG ((DEBUG_INFO, "TXTPEI: TgaSize               = %x\n", (*TxtInfoHob)->Data.TgaSize));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtLcpPdBase          = %x\n", (*TxtInfoHob)->Data.TxtLcpPdBase));
  DEBUG ((DEBUG_INFO, "TXTPEI: TxtLcpPdSize          = %x\n", (*TxtInfoHob)->Data.TxtLcpPdSize));
  DEBUG ((DEBUG_INFO, "TXTPEI: Flags                 = %x\n", (*TxtInfoHob)->Data.Flags));
  DEBUG ((DEBUG_INFO, "TXTPEI: ApStartupBase         = %x\n", (*TxtInfoHob)->Data.ApStartupBase));

  ///
  /// Check if platform specify BIOS ACM address by itself, BIOS ACM address must be 4K alignment in FLASH address space
  ///
  if ((*TxtInfoHob)->Data.BiosAcmBase != 0) {
    ///
    /// Check BIOS ACM is 4K alignment or not
    ///
    if (((*TxtInfoHob)->Data.BiosAcmBase & 0xFFF) != 0) {
      DEBUG ((DEBUG_WARN, "TXTPEI: BIOS ACM is not 4K aligned, force TxtMode=0 and unloaded!!\n"));
      (*TxtInfoHob)->Data.TxtMode = 0;
      return EFI_UNSUPPORTED;
    }
  } else {
    DEBUG ((DEBUG_ERROR, "TXTPEI: BIOS ACM not found, force TxtMode=0 and unloaded!\n"));
    (*TxtInfoHob)->Data.TxtMode = 0;
    return EFI_UNSUPPORTED;
  }
  ///
  /// Check ACM is matched to chipset or not, if not, disable TXT and return EFI_UNLOAD_IMAGE
  ///
  if (!CheckTxtAcmMatch ((ACM_HEADER*)(UINTN)((*TxtInfoHob)->Data.BiosAcmBase))) {
    DEBUG ((DEBUG_ERROR, "TXTPEI: BIOS ACM is not matched to chipset!! Force TxtMode=0 and unloaded!!\n"));
    (*TxtInfoHob)->Data.TxtMode = 0;
    return EFI_UNSUPPORTED;
  }

  /// Read ACM size from ACM header
  (*TxtInfoHob)->Data.BiosAcmSize = ((ACM_HEADER*)(UINTN)((*TxtInfoHob)->Data.BiosAcmBase))->Size << 2;

  ///
  /// Check if platform specify AP starup address
  ///
  if ((*TxtInfoHob)->Data.ApStartupBase == 0) {
    ///
    /// Get AP startup base from TxtInfoHob provided by platform code
    ///
    DEBUG ((DEBUG_INFO, "TXTPEI: AP Startup location not found\n"));
  }

  ///
  /// Initialize local APIC
  ///
  if ((((AsmReadMsr64 (MSR_IA32_APIC_BASE)) & (BIT11 + BIT10)) == BIT11 + BIT10)) {
    AsmWriteMsr64 (MSR_IA32_X2APIC_SIVR, 0x1FF);
  } else {
    ///
    /// Determine address of Local APIC
    ///
    Ia32ApicBase = ((UINTN) AsmReadMsr64 (MSR_IA32_APIC_BASE)) & BASE_ADDR_MASK;
    MmioWrite32 (Ia32ApicBase + APIC_SPURIOUS_VECTOR_REGISTER, 0x1FF);
  }

  ///
  /// Make sure none of our pointers are still NULL
  ///
  if (!((*TxtInfoHob)->Data.BiosAcmBase)) {
    (*TxtInfoHob)->Data.TxtMode = 0;
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

/**
  Determines whether or not the current processor is TXT Capable.

  @retval TRUE          - If the current processor supports TXT
  @retval FALSE         - If the current processor does not support TXT
**/
BOOLEAN
IsTxtProcessor (
  VOID
  )
{
  CPUID_VERSION_INFO_ECX Ecx;

  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, &Ecx.Uint32, NULL);

  return (BOOLEAN) (Ecx.Bits.SMX == 1);
}

/**
  Determines whether or not the current chipset is TXT Capable.

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @retval TRUE          - If the current chipset supports TXT
  @retval FALSE         - If the current chipset doesn't supports TXT
**/
BOOLEAN
IsTxtChipset (
  VOID
  )
{
  BOOLEAN TxtCapable;
  UINT32  Data32;

  TxtCapable  = FALSE;
  Data32      = CheckSmxCapabilities ();

  if ((Data32 & BIT0) != 0) {
    TxtCapable = TRUE;
  } else {
    TxtCapable = FALSE;
  }

  return TxtCapable;
}

/**
  Determines whether TXT is enabled by platform setting

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @retval TRUE          - If TXT is enabled by platform setting
  @retval FALSE         - If TXT is disabled by platform setting
**/
BOOLEAN
IsTxtEnabled (
  IN TXT_INFO_HOB *TxtInfoHob
  )
{
  if (TxtInfoHob == NULL) {
    return FALSE;
  }
  return (BOOLEAN)TxtInfoHob->Data.TxtMode;
}

/**
  Determines whether Alias Check Request is enabled by platform setting

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @retval TRUE          - If ACHECK REQUEST is enabled by platform setting
  @retval FALSE         - If ACHECK REQUEST is disabled by platform setting
**/
BOOLEAN
IsAcheckRequested (
  IN TXT_INFO_HOB *TxtInfoHob
  )
{
  if (TxtInfoHob == NULL) {
    return FALSE;
  }

  return (BOOLEAN) (TxtInfoHob->Data.TxtAcheckRequest);
}

/**
  Determines ACM is matched to chipset or not

  @param[in] TxtContextDataPtr         - Point to TXT_PEI_LIB_CONTEXT structure
  @param[in] BiosAcmBase  - A pointer to BIOS ACM location

  @retval TRUE          - BIOS ACM is matched to chipset
  @retval FALSE         - BIOS ACM is NOT matched to chipset
**/
BOOLEAN
CheckTxtAcmMatch (
  IN ACM_HEADER          *BiosAcmBase
  )
{
  BOOLEAN ChipsetIsProduction;
  BOOLEAN BiosAcmIsProduction;

  if (BiosAcmBase == NULL) {
    return FALSE;

  }
  ///
  /// Initializing ChipsetIsProduction default value
  ///
  ChipsetIsProduction = (MmioRead32 (TXT_PUBLIC_BASE + 0x200) & BIT31) ? TRUE : FALSE;

  ///
  /// Check ACM is production or not
  ///
  BiosAcmIsProduction = (BiosAcmBase->ModuleId & BIT31) ? FALSE : TRUE;

  return ChipsetIsProduction == BiosAcmIsProduction;
}

/**
  Create TXT Info HOB

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @retval EFI_SUCCESS   - If TXT Info Hob is found
  @exception EFI_NOT_FOUND - If TXT Info Hob is not found
  @exception EFI_UNSUPPORTED - If the platform is not TXT capable.
**/
EFI_STATUS
CreateTxtInfoHob (
  IN TXT_INFO_HOB **TxtInfoHob
  )
{
  EFI_STATUS                   Status;
  SI_PREMEM_POLICY_PPI         *SiPreMemPolicy;
  CPU_SECURITY_PREMEM_CONFIG   *CpuSecurityPreMemConfig;
  TXT_PREMEM_CONFIG            *TxtPreMemConfig;
  BOOLEAN                      TableFound = FALSE;
  UINT8                        *AcmPtr = 0;
  UINT8                        *StartPtr = 0;
  ACM_HEADER                   *AcmHeader = 0;
  ACM_INFO_TABLE               *AcmInfoTable = 0;

  *TxtInfoHob = NULL;
  //
  // Get TxtInfoHob if it is already present, then this is the reinitalize, just reload the hob and exit
  //
  *TxtInfoHob = (TXT_INFO_HOB *) GetFirstGuidHob (&gTxtInfoHobGuid);
  if (*TxtInfoHob != NULL) {
    return EFI_SUCCESS;
  }
  ///
  /// Locate Cpu policy ppi to initialized.
  ///
  Status = PeiServicesLocatePpi (
                  &gSiPreMemPolicyPpiGuid,
                  0,
                  NULL,
                  (VOID **) &SiPreMemPolicy
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gTxtPreMemConfigGuid, (VOID *) &TxtPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  ///
  /// Create hob for storing TXT data
  ///
  Status = PeiServicesCreateHob (
             EFI_HOB_TYPE_GUID_EXTENSION,
             sizeof (TXT_INFO_HOB),
             (VOID **) TxtInfoHob
             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  CopyGuid (&(*TxtInfoHob)->EfiHobGuidType.Name, &gTxtInfoHobGuid);
  ///
  /// Initiate Txt Info Hob
  ///
  ZeroMem (&((*TxtInfoHob)->Data), sizeof (TXT_INFO_DATA));

  (*TxtInfoHob)->Data.ChipsetIsTxtCapable = IsTxtChipset ();
  (*TxtInfoHob)->Data.ProcessorIsTxtCapable = IsTxtProcessor ();
  if (CpuSecurityPreMemConfig->Txt == 1) {
    (*TxtInfoHob)->Data.TxtMode = 1;
  }


  (*TxtInfoHob)->Data.AcpiBase            = PmcGetAcpiBase ();
  (*TxtInfoHob)->Data.TxtAcheckRequest    = (BOOLEAN) CpuSecurityPreMemConfig->TxtAcheckRequest;
  (*TxtInfoHob)->Data.TxtPowerdownRequest = (BOOLEAN) CpuSecurityPreMemConfig->TxtPowerdownRequest;
  (*TxtInfoHob)->Data.ResetAux            = (BOOLEAN) CpuSecurityPreMemConfig->ResetAux;
  (*TxtInfoHob)->Data.SinitMemorySize     = (UINT64) TxtPreMemConfig->SinitMemorySize;
  (*TxtInfoHob)->Data.TxtHeapMemorySize   = (UINT64) TxtPreMemConfig->TxtHeapMemorySize;
  (*TxtInfoHob)->Data.TxtDprMemoryBase    = TxtPreMemConfig->TxtDprMemoryBase;
  (*TxtInfoHob)->Data.TxtDprMemorySize    = (UINT64) TxtPreMemConfig->TxtDprMemorySize;
  (*TxtInfoHob)->Data.BiosAcmBase         = TxtPreMemConfig->BiosAcmBase;
  (*TxtInfoHob)->Data.BiosAcmSize         = (UINT64) TxtPreMemConfig->BiosAcmSize;
  (*TxtInfoHob)->Data.TgaSize             = (UINT64) TxtPreMemConfig->TgaSize;
  (*TxtInfoHob)->Data.TxtLcpPdBase        = TxtPreMemConfig->TxtLcpPdBase;
  (*TxtInfoHob)->Data.TxtLcpPdSize        = TxtPreMemConfig->TxtLcpPdSize;
  (*TxtInfoHob)->Data.ApStartupBase       = TxtPreMemConfig->ApStartupBase;
  (*TxtInfoHob)->Data.Flags              |= (UINT64)TxtPreMemConfig->IsTPMPresence;

  if ((*TxtInfoHob)->Data.BiosAcmBase != 0) {
    //
    // Parse the ACM header. Locate the ACM Info Table.
    // Get the ACM major/minor version
    //
    AcmHeader = (ACM_HEADER*)(UINTN)((*TxtInfoHob)->Data.BiosAcmBase);
    AcmPtr = (UINT8 *)AcmHeader;
    AcmPtr += AcmHeader->HeaderLen * 4;
    AcmPtr += AcmHeader->ScratchSize * 4;

    AcmInfoTable = (ACM_INFO_TABLE *)AcmPtr;

    if (AcmInfoTable->AitVersion < ACM_INFO_TABLE_VERSION_9) {
      //
      // Extract ACM Info Table version numbers
      //
      if (CompareGuid(&(AcmInfoTable->Guid), &gTxtAcmInfoTableGuid)) {
        (*TxtInfoHob)->Data.AcmMajorVersion = AcmInfoTable->AitRevision[0];
        (*TxtInfoHob)->Data.AcmMinorVersion = AcmInfoTable->AitRevision[1];
        (*TxtInfoHob)->Data.AcmRevision = AcmInfoTable->AitRevision[2];
      }
    } else if (AcmInfoTable->AitVersion == ACM_INFO_TABLE_VERSION_9) {
      //
      // With ACM INFO TABLE VERSION 9, ACM version is part of
      // ACM VERSION INFO table
      //
      AcmPtr += sizeof(ACM_INFO_TABLE);
      StartPtr = AcmPtr;

      //
      // Here we are dealing with variable structures. It is not feasible
      // to rely on structure fields to get upper limit of the condition.
      // Hence a hardcoded value is defined.
      //
      while (!(CompareId(((VAR_LIST *)AcmPtr)->Id, NULL_TERMINATOR_ID, 4)) && \
             ((AcmPtr - StartPtr) < 300)) {
        //
        // Check if ACM VERSION INFO table found
        //
        if (CompareId(((VAR_LIST *)AcmPtr)->Id, ACM_VERSION_INFORMATION_LIST_ID, 4)) {
          TableFound = TRUE;
          break;
        }
        AcmPtr += ((VAR_LIST *)AcmPtr)->Length;
      }

      if (TableFound) {
        (*TxtInfoHob)->Data.AcmMajorVersion = ((ACM_VER_INFO_TABLE *)AcmPtr)->AcmRev[0];
        (*TxtInfoHob)->Data.AcmMinorVersion = ((ACM_VER_INFO_TABLE *)AcmPtr)->AcmRev[1];
        (*TxtInfoHob)->Data.AcmRevision = ((ACM_VER_INFO_TABLE *)AcmPtr)->AcmRev[2];
      }
    }

    DEBUG((DEBUG_INFO, "(*TxtInfoHob)->Data.AcmMajorVersion:  %x\n", (*TxtInfoHob)->Data.AcmMajorVersion));
    DEBUG((DEBUG_INFO, "(*TxtInfoHob)->Data.AcmMinorVersion:  %x\n", (*TxtInfoHob)->Data.AcmMinorVersion));
    DEBUG((DEBUG_INFO, "(*TxtInfoHob)->Data.AcmRevision:  %x\n", (*TxtInfoHob)->Data.AcmRevision));
  }

  return EFI_SUCCESS;
}

/**
  Add extened elements to BiosOsData

  @param[in] TxtDxeCtx      - A pointer to an initialized TXT DXE Context data structure
  @param[in] Type        - The element's type
  @param[in] Buffer      - A pointer to buffer which need to append the element
  @param[in] Size        - return the size of the appened element.
**/
VOID
AppendElement (
  IN TXT_INFO_HOB         *TxtInfoHob,
  IN UINT32               Type,
  IN VOID                 *Buffer,
  OUT UINT32              *Size
  )
{
  VOID        *Element;
  UINT32      NumberOfAcm;
  UINT64      *AcmBase;
  EFI_STATUS  Status;
  NumberOfAcm = 1;
  AcmBase     = NULL;
  Element     = NULL;
  *Size       = 0;
  switch (Type) {
    case HEAP_EXTDATA_TYPE_BIOS_SPEC_VER:
      ///
      /// Fill BIOS spec ver element
      ///
      Status = PeiServicesAllocatePool (sizeof (HEAP_BIOS_SPEC_VER_ELEMENT),&Element);
      ASSERT_EFI_ERROR(Status);

      if (Element != NULL) {
        *Size = sizeof (HEAP_BIOS_SPEC_VER_ELEMENT);
        ((HEAP_BIOS_SPEC_VER_ELEMENT *) Element)->Header.Type     = HEAP_EXTDATA_TYPE_BIOS_SPEC_VER;
        ((HEAP_BIOS_SPEC_VER_ELEMENT *) Element)->Header.Size     = sizeof (HEAP_BIOS_SPEC_VER_ELEMENT);
        ((HEAP_BIOS_SPEC_VER_ELEMENT *) Element)->SpecVerMajor    = TXT_BIOS_SPEC_VER_MAJOR;
        ((HEAP_BIOS_SPEC_VER_ELEMENT *) Element)->SpecVerMinor    = TXT_BIOS_SPEC_VER_MINOR;
        ((HEAP_BIOS_SPEC_VER_ELEMENT *) Element)->SpecVerRevision = TXT_BIOS_SPEC_VER_REVISION;
      }
      break;

    case HEAP_EXTDATA_TYPE_BIOSACM:
      ///
      /// Fill BIOS ACM element
      ///
      Status = PeiServicesAllocatePool (sizeof (HEAP_BIOSACM_ELEMENT) + NumberOfAcm * sizeof (UINT64),&Element);
      ASSERT_EFI_ERROR(Status);

      if (Element != NULL) {
        *Size = sizeof (HEAP_BIOSACM_ELEMENT) + sizeof (UINT64) * NumberOfAcm;
        ((HEAP_BIOSACM_ELEMENT *) Element)->Header.Type = HEAP_EXTDATA_TYPE_BIOSACM;
        ((HEAP_BIOSACM_ELEMENT *) Element)->Header.Size = sizeof (HEAP_BIOSACM_ELEMENT) + NumberOfAcm * sizeof (UINT64);
        ((HEAP_BIOSACM_ELEMENT *) Element)->NumAcms     = NumberOfAcm;
        AcmBase = (UINT64 *) ((UINTN) Element + sizeof (HEAP_BIOSACM_ELEMENT));
        *AcmBase = TxtInfoHob->Data.BiosAcmBase;
      }
      break;

    case HEAP_EXTDATA_TYPE_END:
      ///
      /// Fill end type element
      ///
      Status = PeiServicesAllocatePool (sizeof (HEAP_EXT_DATA_ELEMENT),&Element);
      ASSERT_EFI_ERROR(Status);

      if (Element != NULL) {
        *Size = sizeof (HEAP_EXT_DATA_ELEMENT);
        ((HEAP_EXT_DATA_ELEMENT *) Element)->Type = HEAP_EXTDATA_TYPE_END;
        ((HEAP_EXT_DATA_ELEMENT *) Element)->Size = sizeof (HEAP_EXT_DATA_ELEMENT);
      }
      break;

    default:
      break;
  }

  if (Element != NULL) {
    CopyMem (Buffer, Element, *Size);
  }
}

/**
  Allocates 1 MB of 1MB-aligned memory for use as TXT Device Memory.  Records
  the location of TXT Device Memory in TXT Chipset registers and then adds
  programming instructions for these registers into BootScript.

  @param[in] TxtDxeCtx      - A pointer to an initialized TXT DXE Context data structure

  @retval EFI_SUCCESS     - TXT Device memory has been successfully initialized.
  @exception EFI_UNSUPPORTED - TXT Device memory not available.
**/
EFI_STATUS
SetupTxtDeviceMemory (
  IN TXT_INFO_HOB *TxtInfoHob,
  IN UINTN        CpuCount
  )
{
  UINTN                       TopAddr;
  UINTN                       *Ptr;
  UINT64                      Value64;
  UINT32                      Value32;
  UINTN                       TxtHeapMemoryBase;
  UINTN                       TxtSinitMemoryBase;
  BOOLEAN                     Locked;
  BIOS_OS_DATA_REGION         *BiosOsDataRegion;
  TXT_INFO_DATA               *TxtInfoData;
  UINT8                       *Ptr8;

  DEBUG ((DEBUG_INFO, "TXTPEI::SetupTxtDeviceMemory\n"));

  TxtHeapMemoryBase         = 0;
  TxtSinitMemoryBase        = 0;
  Locked                    = FALSE;
  Ptr8                      = NULL;
  Value32                   = 0;

  TxtInfoData         = &(TxtInfoHob->Data);

  if ((TxtInfoData == 0) ||
      (TxtInfoData->TxtDprMemoryBase == 0) ||
      (TxtInfoData->TxtDprMemorySize == 0) ||
      (TxtInfoData->TxtHeapMemorySize == 0) ||
      (TxtInfoData->SinitMemorySize == 0)
      ) {

  DEBUG ((DEBUG_INFO, "SetupTxtDeviceMemory : SinitMemorySize = 0x%x\n", TxtInfoData->SinitMemorySize));
  DEBUG ((DEBUG_INFO, "SetupTxtDeviceMemory : TxtHeapMemorySize = 0x%x\n", TxtInfoData->TxtHeapMemorySize));
  DEBUG ((DEBUG_INFO, "SetupTxtDeviceMemory : TxtDprMemoryBase = 0x%x\n", TxtInfoData->TxtDprMemoryBase));
  DEBUG ((DEBUG_INFO, "SetupTxtDeviceMemory : TxtDprMemorySize = 0x%x\n", TxtInfoData->TxtDprMemorySize));

    return EFI_UNSUPPORTED;
  } else {
    ///
    /// Use address passed from PEI
    ///
    TopAddr             = (UINTN) (TxtInfoData->TxtDprMemoryBase + TxtInfoData->TxtDprMemorySize);

    TxtHeapMemoryBase   = (UINTN) (TopAddr - TxtInfoData->TxtHeapMemorySize);

    TxtSinitMemoryBase  = (UINTN) (TxtHeapMemoryBase - TxtInfoData->SinitMemorySize);
  }

  ASSERT ((TopAddr & 0x0FFFFF) == 0);

  ///
  /// DPR registers
  ///
  Ptr   = (UINTN *) (UINTN) (TXT_PUBLIC_BASE + TXT_DPR_SIZE_REG_OFF);
  Value64 = RShiftU64 (TxtInfoData->TxtDprMemorySize, 16) | 1;
  *Ptr = (UINTN) (Value64 | TopAddr);
  ///
  /// Assert error if programmed value is different from requested. This
  /// means error is requested size.
  ///
  Value64 = *Ptr;
  ASSERT ((LShiftU64 ((Value64 & 0xFFE), 16)) == TxtInfoData->TxtDprMemorySize);

  ///
  /// HEAP Registers
  /// Program size register first
  ///
  Ptr = (UINTN *) (UINTN) (TXT_PUBLIC_BASE + TXT_HEAP_SIZE_REG_OFF);

  ///
  /// Test register locked status. If locked, skip programming since
  /// this will be done by BIOS ACM
  ///
  *Ptr  = TEST_PATTERN;
  Value32 = (UINT32) *Ptr;
  if (Value32 != TEST_PATTERN) {
    Locked = TRUE;
  } else {
    ///
    /// To be safe invert pattern and try again
    ///
    *Ptr  = (UINTN) ~TEST_PATTERN;
    Value32 = (UINT32) *Ptr;
    if (Value32 != (UINT32) ~TEST_PATTERN) {
      Locked = TRUE;
    }
  }

  if (!Locked) {

    *Ptr = (UINTN) (TxtInfoData->TxtHeapMemorySize);
    ///
    /// Assert error if programmed value is different from requested. This
    /// means error is requested size.
    ///
    Value64 = *Ptr;
    ASSERT (Value64 == TxtInfoData->TxtHeapMemorySize);

    ///
    /// Program base register.
    ///
    Ptr   = (UINTN *) (UINTN) (TXT_PUBLIC_BASE + TXT_HEAP_BASE_REG_OFF);
    *Ptr  = TxtHeapMemoryBase;

    ///
    /// Assert error if programmed value is different from requested. This
    /// means error is requested size.
    ///
    Value64 = *Ptr;
    ASSERT (Value64 == TxtHeapMemoryBase);
  }
  ///
  /// SINIT Registers
  /// Program size register first
  ///
  Ptr   = (UINTN *) (UINTN) (TXT_PUBLIC_BASE + TXT_SINIT_SIZE_REG_OFF);
  *Ptr  = (UINT32) (TxtInfoData->SinitMemorySize);
  ///
  /// Assert error if programmed value is different from requested. This
  /// means error is requested size.
  ///
  Value64 = *Ptr;
  ASSERT (Value64 == TxtInfoData->SinitMemorySize);
  ///
  /// Program base register
  ///
  Ptr   = (UINTN *) (UINTN) (TXT_PUBLIC_BASE + TXT_SINIT_BASE_REG_OFF);
  *Ptr  = TxtSinitMemoryBase;
  ///
  /// Assert error if programmed value is different from requested. This
  /// means error is requested size.
  ///
  Value64 = *Ptr;
  ASSERT (Value64 == TxtSinitMemoryBase);

  ///
  /// Make sure TXT Device Memory has been zeroed
  ///
  ZeroMem (
    (VOID *) ((UINTN) TxtSinitMemoryBase),
    (UINTN) (TopAddr - TxtSinitMemoryBase)
    );

  if (TxtInfoData->TgaSize) {
    ;
    ///
    /// Placeholder for Trusted graphics support
    ///
  }

  Ptr   = (UINTN *) TxtHeapMemoryBase;
  *Ptr  = sizeof (BIOS_OS_DATA_REGION);

  ///
  /// BiosOsDataSize plus sizew of data size feld itself
  ///
  BiosOsDataRegion                          = (BIOS_OS_DATA_REGION *) (Ptr + 2);
  BiosOsDataRegion->Version                 = BIOS_OS_DATAREGION_VERSION;
  BiosOsDataRegion->BiosSinitSize           = 0;
  BiosOsDataRegion->LcpPdBase               = TxtInfoData->TxtLcpPdBase;
  BiosOsDataRegion->LcpPdSize               = TxtInfoData->TxtLcpPdSize;
  BiosOsDataRegion->NumOfLogicalProcessors  = (UINT32) CpuCount;
  BiosOsDataRegion->Flags                   = TxtInfoData->Flags;
  Ptr8 = (UINT8 *) (UINTN) &(BiosOsDataRegion->ExtData);
  AppendElement (TxtInfoHob, HEAP_EXTDATA_TYPE_BIOS_SPEC_VER, Ptr8, &Value32);
  Ptr8 += Value32;
  AppendElement (TxtInfoHob, HEAP_EXTDATA_TYPE_BIOSACM, Ptr8, &Value32);
  Ptr8 += Value32;
  AppendElement (TxtInfoHob, HEAP_EXTDATA_TYPE_END, Ptr8, &Value32);
  Value64 = (UINTN) Ptr8 - TxtHeapMemoryBase + Value32;
  *Ptr  = (Value64 & 0xFFFFFFFF);
  *(Ptr+1)  = (UINT32) (RShiftU64 (Value64,8));


  return EFI_SUCCESS;
}

/**
  Determines whether or not the platform has executed a TXT launch by
  examining the TPM Establishment bit.

  @param[in] TxtContextDataPtr      - A pointer to an initialized TXT PEI Context data structure

  @retval TRUE          - If the TPM establishment bit is asserted.
  @retval FALSE         - If the TPM establishment bit is unasserted.
**/
BOOLEAN
TxtIsEstablishmentBitAsserted (
  IN TXT_INFO_HOB *TxtInfoHob
  )
{
  UINT8  Access;
  UINT16 TimeOutCount;


  ///
  /// Set TPM.ACCESS polling timeout about 750ms
  ///
  TimeOutCount = TPM_TIME_OUT;
  do {
    ///
    /// Read TPM status register
    ///
    Access = MmioRead8 (TPM_STATUS_REG_ADDRESS);

    ///
    /// if TPM.Access == 0xFF, TPM is not present
    ///
    if (Access == 0xFF) {
      return FALSE;
    }
    ///
    /// Check tpmRegValidSts bit before checking establishment bit
    ///
    if ((Access & 0x80) != 0x80) {
      ///
      /// Delay 1ms
      ///
      MicroSecondDelay (1000);
    } else {
      ///
      /// tpmRegValidSts set, we can check establishment bit now.
      ///
      break;
    }

    TimeOutCount--;
  } while (TimeOutCount != 0);
  ///
  /// if tpmRegValidSts is not set, TPM is not usable
  ///
  if ((Access & 0x80) != 0x80) {
    DEBUG ((DEBUG_ERROR, "TXTPEI: TPM Valid Status is not set!! TPM.ACCESS=%x\n", Access));
    ASSERT (FALSE);
    CpuDeadLoop ();
  }
  ///
  /// The bit we're interested in uses negative logic:
  /// If bit 0 == 1 then return False
  /// Else return True
  ///
  return (Access & 0x1) ? FALSE : TRUE;
}

/**
  Determines whether or not the platform has encountered an error during
  a sleep or power-off state.

  @retval TRUE          - If the TXT_WAKE_ERROR bit is asserted.
  @retval FALSE         - If the TXT_WAKE_ERROR bit is unasserted.
**/
BOOLEAN
IsTxtWakeError (
  VOID
  )
{
  UINT8 Ests;

  ///
  /// Read TXT.ESTS register
  ///
  Ests = MmioRead8 (TXT_PUBLIC_BASE + TXT_ERROR_STATUS_REG_OFF);

  DEBUG ((DEBUG_INFO, "TXTPEI: TXT.ESTS=%x\n", Ests));

  return (Ests & (0x1 << 6)) ? TRUE : FALSE;
}

/**
  Determines whether or not the platform memory has been locked

  @retval TRUE          - If memroy is locked
  @retval FALSE         - If memory is unlocked
**/
BOOLEAN
IsMemoryLocked (
  VOID
  )
{
  UINT32 CpuMemLockStatus;

  ///
  /// Check status register for now.
  ///
  CpuMemLockStatus = MmioRead32 (TXT_PUBLIC_BASE + TXT_E2STS_REG_OFF);

  DEBUG ((DEBUG_INFO, "TXTPEI: CPU_UNCORE_MEMLOCK_STATUS=%x\n", CpuMemLockStatus));
  ///
  /// if BLOCK_MEM_STS (BIT2) is set to 1, memory is in unlock.
  ///
  return (CpuMemLockStatus & TXT_BLOCK_MEM_STS) ? TRUE : FALSE;
}

/**
  Determines whether or not POISON bit is set in status register

  @retval TRUE          - If the TPM reset bit is asserted.
  @retval FALSE         - If the TPM reset bit is unasserted.
**/
BOOLEAN
IsTxtResetSet (
  VOID
  )
{
  UINT8 Ests;
  ///
  /// Read TXT.ESTS register
  ///
  Ests = MmioRead8 (TXT_PUBLIC_BASE + TXT_ERROR_STATUS_REG_OFF);

  return (Ests & (0x1 << 0)) ? TRUE : FALSE;
}

/**
  Determines whether or not SECRETS.STS bit is set in E2STS status register

  @retval TRUE          - If the LT.SECRETS.STS bit is asserted.
  @retval FALSE         - If the LT.SECRETS.STS bit is unasserted.
**/
BOOLEAN
IsTxtSecretsSet (
  VOID
  )
{
  UINT8 E2sts;

  ///
  /// Read TXT.E2STS register
  ///
  E2sts = MmioRead8 (TXT_PUBLIC_BASE + TXT_E2STS_REG_OFF);

  DEBUG ((DEBUG_INFO, "TXTPEI: TXT.E2STS=%x\n", E2sts));

  return (E2sts & TXT_SECRETS_STS) ? TRUE : FALSE;
}

/**
  Determines presence of TPM in system

  @param[in] TxtContextDataPtr           - Point to TXT_PEI_LIB_CONTEXT structure
  @param[in] TxtPeiCtx      - A pointer to an initialized TXT PEI Context data structure

  @retval EFI_SUCCESS          - If the TPM is present.
  @exception EFI_NOT_FOUND         - If the TPM is not present.
**/
EFI_STATUS
IsTpmPresent (
  IN TXT_INFO_HOB *TxtInfoHob
  )
{


  if((TxtInfoHob->Data.Flags & TPM_INIT_FAILED) == 0){
    return EFI_SUCCESS;
  } else {
    return EFI_DEVICE_ERROR;
  }
}

/**
  Clear Sleep Type register.

  @param[in] TxtContextDataPtr          - A pointer to an initialized TXT PEI Context data structure

  @retval EFI_SUCCESS   - Always
**/
EFI_STATUS
ClearSlpTyp (
  IN TXT_INFO_HOB          *TxtInfoHob
  )
{
  UINT32 Pm1Addr;
  UINT32 Pm1Value;

  ///
  /// Make address for PM1_CNT
  ///
  Pm1Addr = ((UINT32) TxtInfoHob->Data.AcpiBase) + 4;

  ///
  /// Read 32-bits from PM1_CNT
  ///
  Pm1Value = IoRead32 ((UINTN) Pm1Addr);

  ///
  /// Clear SLP_TYP bits 10-12
  ///
  Pm1Value = Pm1Value & 0xffffe3ff;
  IoWrite32 (
    (UINTN) Pm1Addr,
    (UINT32) (Pm1Value)
    );

  return EFI_SUCCESS;
}

/**
  Issue a Global Reset
**/
VOID
IssueGlobalReset(
  VOID
  )
{
  PCH_RESET_DATA             ResetData;

  CopyMem (&ResetData.Guid, &gPchGlobalResetGuid, sizeof (EFI_GUID));
  StrCpyS (ResetData.Description, PCH_RESET_DATA_STRING_MAX_LENGTH, PCH_PLATFORM_SPECIFIC_RESET_STRING);
  (*GetPeiServicesTablePointer ())->ResetSystem2 (EfiResetPlatformSpecific, EFI_SUCCESS, sizeof (PCH_RESET_DATA), &ResetData);
}

/**
  Initializes values passed to AP

  @param[in] TxtContextDataPtr      - A pointer to an initialized TXT PEI Context data structure
**/
VOID
PrepareApParams (
  IN TXT_INFO_HOB          *TxtInfoHob
  )
{
  UINTN TxtPublicSpace;

  TxtPublicSpace  = TXT_PUBLIC_BASE;

  MmioWrite32 (TxtPublicSpace + BIOACM_ADDR, (UINT32) (TxtInfoHob->Data.BiosAcmBase));
  MmioWrite32 (TxtPublicSpace + APINIT_ADDR, (UINT32) (TxtInfoHob->Data.ApStartupBase));
  MmioWrite32 (TxtPublicSpace + SEMAPHORE, 0);
}

/**
  Copy ACM in WB memory.

  @retval TempRamAddr   - Address of the ACM region.
**/
EFI_PHYSICAL_ADDRESS
LoadAcm (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS          AcmModuleAddr;
  UINT64                        AcmModuleSize;
  TXT_INFO_HOB                  *TxtInfoHob = NULL;
  EFI_PHYSICAL_ADDRESS          TempRamAddr;
  EFI_STATUS                    Status;

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TXTPEI::Failed to load ACM\n"));
    return EFI_NOT_FOUND;
  }
  AcmModuleAddr = TxtInfoHob->Data.BiosAcmBase;
  AcmModuleSize = TxtInfoHob->Data.BiosAcmSize;

  Status = PeiServicesAllocatePages (
                   EfiLoaderCode,
                   EFI_SIZE_TO_PAGES (AcmModuleSize),
                   &TempRamAddr
                   );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "Page Allocation Failed, Load ACM in Temporary RAM\n"));
    //
    // There are no enough pages available to be allocated for ACM.
    // Let's load ACM at the Temporary RAM base as ACM launch will anyway
    // trigger a system reset.
    //
    TempRamAddr = (EFI_PHYSICAL_ADDRESS) (PcdGet32 (PcdTemporaryRamBase));
  }

  DEBUG ((DEBUG_INFO, "Loading ACM at 0x%X\n", TempRamAddr));
  CopyMem (
    (UINT64 *) TempRamAddr,
    (UINT64 *) AcmModuleAddr,
    AcmModuleSize
    );

  return TempRamAddr;
}

/**
  Invokes the SCLEAN/ACHECK function from the TXT BIOS ACM.
  1. Clearing of sleep type is necessary because SCLEAN/ACHECK destroys memory
  context, so S3 after it is run and system is reset is impossible. We
  do it here since there is no any indication that can sustain reset
  for any other module to do it on our behalf.
  2. APs are initialized before calling of SCLEAN/ACHECK
  3. SCLEAN/ACHECK function is invoked.

  @param[in] TxtContextDataPtr      - A pointer to an initialized TXT PEI Context data structure
  @param[in] func      - Acm function to be launch

  @retval EFI_SUCCESS   - Always.
**/
EFI_STATUS
DoAcmLaunch (
  IN TXT_INFO_HOB          *TxtInfoHob,
  IN UINT8               func
  )
{
  EFI_STATUS                    Status;
  SI_PREMEM_POLICY_PPI          *SiPreMemPolicy;
  CPU_SECURITY_PREMEM_CONFIG    *CpuSecurityPreMemConfig;

  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicy
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetConfigBlock ((VOID *) SiPreMemPolicy, &gCpuSecurityPreMemConfigGuid, (VOID *) &CpuSecurityPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  ClearSlpTyp (TxtInfoHob);

  PrepareApParams (TxtInfoHob);

  if (CpuSecurityPreMemConfig->SkipStopPbet != TRUE) {
    ///
    /// Disable PBET before send IPI to APs
    ///
    StopPbeTimer ();
  }

  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TXT_ACM_ENTRY); //PostCode (0x9901)
  if (func == TXT_LAUNCH_CLEAR_SECRETS) {
    DEBUG ((DEBUG_INFO, "Trigger FastBootFlagStatusCallback to clean FastBoot Status\n"));
    Status = PeiServicesInstallPpi (&mPeiTxtCleanResetNotificationPpi);
    ASSERT_EFI_ERROR (Status);
    LaunchBiosAcmClearSecrets ();
  } else if (func == TXT_LAUNCH_ACHECK) {
    LaunchBiosAcmAcheck ();
  }
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TXT_ACM_EXIT); //PostCode (0x9902)

  return EFI_SUCCESS;
}

/**
  Launching of SCHECK function from the TXT BIOS ACM.

  @param[in] TxtContextDataPtr         - A pointer to an initialized TXT PEI Context data structure

  @retval EFI_SUCCESS     - Always.
**/
VOID
EFIAPI
DummyFunc (
  IN OUT VOID  *Buffer
  )
{
  return;
}

/**
  Invoke BIOS ACM function

  @param[in]
     TxtInfoHob: pointer to TXT_INFO_HOB struct
     Function:   index of ACM function to invoke
     BootMode:   Cold boot or S3
**/
EFI_STATUS
DoPeiLaunchAcm (
  IN EFI_PEI_SERVICES  **PeiServices,
  IN TXT_INFO_HOB          *TxtInfoHob,
  IN UINT64                Function,
  IN UINT64                BootMode
  )
{
  ///
  /// ApCfg array contains 10 elements for 8 MTRRs + 10 element for
  /// IA32_MTRR_DEF_TYPE MTRR + 1 element for IDT + 1 element for
  /// IA32_MISC_ENABLE MSR
  ///
  UINT64  ApCfg[MSR_IA32_MTRR_PHYSMASK9 - MSR_IA32_MTRR_PHYSBASE0 + 4];
  UINT64  BiosAcAddr;
  UINT64  SavedSmiSourcesVariables[2];
  UINTN   NumberOfProcessors;
  UINTN   NumberOfEnabledProcessors;
  UINTN   MyCpuNumber;
  UINTN   Index;
  EDKII_PEI_MP_SERVICES2_PPI *MpServices2Ppi = NULL;
  EFI_STATUS                 Status = EFI_SUCCESS;

  Status = PeiServicesLocatePpi (
             &gEdkiiPeiMpServices2PpiGuid,
             0,
             NULL,
             (VOID **) &MpServices2Ppi);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }
  Status = MpServices2Ppi->GetNumberOfProcessors (MpServices2Ppi, (UINTN *)&NumberOfProcessors, (UINTN *)&NumberOfEnabledProcessors);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }
  Status = MpServices2Ppi->WhoAmI (
                        MpServices2Ppi,
                        &MyCpuNumber
                        );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  BiosAcAddr = (UINT64) (UINTN) TxtInfoHob->Data.BiosAcmBase;
  SaveApConfig (ApCfg);
  DisableSmiSources (TxtInfoHob, SavedSmiSourcesVariables, TRUE);

  ///
  /// Disable APs and put in WFS state
  ///
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    if (Index != MyCpuNumber) {
      DEBUG ((DEBUG_INFO, "TXTPEI: disable AP Index=%x\n", Index));
      Status = MpServices2Ppi->EnableDisableAP (
                            MpServices2Ppi,
                            Index,
                            FALSE,
                            NULL
                            );
      ASSERT_EFI_ERROR (Status);
    }
  }
  SendInitIpiAllExcludingSelf();

  // Give the APs time to enter wait-for-SIPI state
  MicroSecondDelay (10 * STALL_ONE_MILLI_SECOND);

  ///
  /// Launch the BIOS ACM to run the requested function
  ///
  DEBUG ((DEBUG_INFO, "TXTPEI::Running of LaunchBiosAcm\n"));
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TXT_ACM_ENTRY); //PostCode (0x9901)
  LaunchBiosAcm (&BiosAcAddr,Function,BootMode);
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_TXT_ACM_EXIT); //PostCode (0x9902)

  ///
  /// Restart APs
  ///
  for (Index = 0; Index < NumberOfProcessors; Index++) {
    if (Index != MyCpuNumber) {
      DEBUG ((DEBUG_INFO, "TXTPEI: restart AP Index=%x\n", Index));
      Status = MpServices2Ppi->EnableDisableAP (
                            MpServices2Ppi,
                            Index,
                            TRUE,
                            NULL
                            );
      ASSERT_EFI_ERROR (Status);
    }
  }

  DisableSmiSources (TxtInfoHob, SavedSmiSourcesVariables, FALSE);
  RestoreMtrrProgramming (ApCfg);
  return EFI_SUCCESS;
}

/**
  Disable or restore possible SMI sources before or after SCHECK on S3 resume.

  @param[in] TxtContextDataPtr               - A pointer to an initialized TXT PEI Context data structure
  @param[in] SavedSmiSourcesVariablesPointer - Saved SMI source Variable
  @param[in] Operation
                                             - Boolean value telling what operation is requested:
                                             - TRUE: save and then disable possible SMI sources.
                                             - FALSE: restore original SMI settings.

  @retval EFI_SUCCESS   - Always.
**/
EFI_STATUS
DisableSmiSources (
  IN TXT_INFO_HOB          *TxtInfoHob,
  IN UINT64*               SavedSmiSourcesVariablesPointer,
  IN BOOLEAN               Operation
  )
{
  UINT64        GlobalSmiControlIoAddr;
  UINT32        SavedSmiControl;
  UINT64        SavedIa32ThermInterruptMsr;

  GlobalSmiControlIoAddr = TxtInfoHob->Data.AcpiBase + 0x30;

  if (Operation == TRUE) {
    SavedIa32ThermInterruptMsr = AsmReadMsr64 ((UINT32) MSR_IA32_THERM_INTERRUPT);
    //
    // Cannot use static variables because of PEI phase, static variables would be in code region which is read-only.
    //
    *SavedSmiSourcesVariablesPointer = SavedIa32ThermInterruptMsr;
    AsmWriteMsr64 (
      (UINT32) MSR_IA32_THERM_INTERRUPT,
      (UINT64) (SavedIa32ThermInterruptMsr & ~(BIT0 + BIT1 + BIT2 + BIT4 + BIT15 + BIT23))
      );
    SavedSmiControl = IoRead32 ((UINTN) GlobalSmiControlIoAddr);
    *((UINT64 *) (SavedSmiSourcesVariablesPointer + 1)) = SavedSmiControl;
    //
    // We can not disable Global SMI since it should be locked after SCHECK. we can only disable SMI sources individually.
    //

    IoWrite32 ((UINTN) GlobalSmiControlIoAddr, (UINT32) (SavedSmiControl & 0x01));
  } else {
    //
    // We can not disable Global SMI since it should be locked after SCHECK. we can only disable SMI sources individually.
    // Restore original SMI setting after SCHECK
    //

    SavedSmiControl = (UINT32) *((UINT64 *) (SavedSmiSourcesVariablesPointer + 1));
    IoWrite32 (
      (UINTN) GlobalSmiControlIoAddr,
      (UINT32) (SavedSmiControl)
      );

    SavedIa32ThermInterruptMsr = *SavedSmiSourcesVariablesPointer;
    //
    // Restore IA32_THERMAL_INTERRUPT MSR
    //
    AsmWriteMsr64 (
      (UINT32) MSR_IA32_THERM_INTERRUPT,
      (UINT64) SavedIa32ThermInterruptMsr
      );
  }

  return EFI_SUCCESS;
}

/**
  Unlock memory when security is set and TxT is not enabled

  @param[in] TxtContextDataPtr      - A pointer to an initialized TXT PEI Context data structure

  @retval EFI_SUCCESS  - If address has been found
  @exception EFI_UNSUPPORTED - CPU doesn't support TxT.
**/
EFI_STATUS
TxtUnlockMemory (
  IN TXT_INFO_HOB          *TxtInfoHob
  )
{
  BOOLEAN EstablishmentBitAsserted;

  EstablishmentBitAsserted = TxtIsEstablishmentBitAsserted (TxtInfoHob);

  ///
  /// Need to read FED40000 before unlocking memory
  ///
  if (!EstablishmentBitAsserted) {
    DEBUG ((DEBUG_INFO, "TXTPEI::Unlock memory\n"));
    AsmWriteMsr64 (MSR_LT_UNLOCK_MEMORY, 0);
  } else {
    ///
    /// Lunch SCLEAN if wake error bit is set.
    ///
    if (IsTxtWakeError ()) {
      ///
      /// If TXTRESET is set , we must clean TXTRESET bit otherwise SCLEAN
      /// will fail
      ///
      if (IsTxtResetSet ()) {
        IssueGlobalReset ();
      }

      if ((TxtInfoHob->Data.BiosAcmBase == 0) || (TxtInfoHob->Data.ApStartupBase == 0)) {
        return EFI_UNSUPPORTED;
      }
      ///
      /// Setup and Launch SCLEAN
      ///
      DEBUG ((DEBUG_INFO, "TXTPEI::Entering SCLEAN to unlock memory\n"));

    } else {
      return EFI_UNSUPPORTED;
    }
  }

  return EFI_SUCCESS;
}

/**
  This routine provides TXT status information from S_ACM.

  @retval TRUE - If Bit 30 of LT_SPAD is set by S-ACM , otherwise FALSE
**/
BOOLEAN
IsBptTxtPassed (
  VOID
  )
{
   UINT64  TxtSpad;

   TxtSpad = MmioRead64(TXT_PUBLIC_BASE + TXT_SPAD_REG_OFF);
   return (TxtSpad & B_BPT_TXT_PASSED) ? TRUE : FALSE;
}

/**
  This routine provides alias check request status.
  @retval TRUE - If acheck is requested, otherwise FALSE
**/
BOOLEAN
IsTxtACheckRequested (
  VOID
  )
{
  TXT_INFO_HOB        *TxtInfoHob = NULL;

  TxtInfoHob = GetFirstGuidHob (&gTxtInfoHobGuid);
  if (TxtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "IsTxtACheckRequested::TxtInfoHob not found\n"));
    return FALSE;
  }

  if (((MmioRead32 (TXT_PUBLIC_BASE + TXT_CRASHCODE_REG_OFF) & 0xFE007FF0) == 0xC00040B0) || \
      (IsAcheckRequested(TxtInfoHob))) {
        return TRUE;
      }
  return FALSE;
}

/**
  This routine will check the TxtStatus in CMOS
  @retval TRUE - If Txt is enabled, otherwise FALSE
**/
BOOLEAN
IsTxtEnabledCmos (
  VOID
  )
{
  UINT8 TxtStatus;

  TxtStatus = 0;

  IoWrite8 (CMOS_IO_ADDRESS, FIT_REC_TXT_POLICY_TYPE_A);
  TxtStatus = IoRead8(CMOS_IO_DATA);

  DEBUG ((DEBUG_INFO, "TXTPEI: CmosTxtStatus = %d\n", TxtStatus));

  return ((TxtStatus & BIT4) == BIT4) ? TRUE : FALSE;
}

/**
  UpdateTxtStatusCmos to write TXT Status to CMOS.
  @param[in] TxtStatus To Enable/Disable TXT
**/
VOID
UpdateTxtStatusCmos (
  BOOLEAN TxtStatus
  )
{
  UINT8               CmosStatus  = 0;

  IoWrite8 (CMOS_IO_ADDRESS, FIT_REC_TXT_POLICY_TYPE_A);
  CmosStatus = IoRead8(CMOS_IO_DATA);

  DEBUG ((DEBUG_INFO, "TXTPEI: CmosStatus = %d\n", CmosStatus));

  if (TxtStatus == TRUE) {
    IoWrite8 (CMOS_IO_ADDRESS, FIT_REC_TXT_POLICY_TYPE_A);
    IoWrite8 (CMOS_IO_DATA, CmosStatus | BIT4);
  } else {
    IoWrite8 (CMOS_IO_ADDRESS, FIT_REC_TXT_POLICY_TYPE_A);
    IoWrite8 (CMOS_IO_DATA, CmosStatus & ~BIT4);
  }

  IoWrite8 (CMOS_IO_ADDRESS, FIT_REC_TXT_POLICY_TYPE_A);
  DEBUG ((DEBUG_INFO, "TXTPEI: CmosStatus Post Write = %d\n", IoRead8(CMOS_IO_DATA)));
}