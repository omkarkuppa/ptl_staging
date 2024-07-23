/** @file
  DGR SMM Library

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

extern SMM_ENTRY_POINT_INFORMATION_TABLE  *mSmmDgrEntryPointInfoTable;

BOOLEAN                  mSmmDgrEnable = FALSE;
BOOLEAN                  mSmmDgrStateSaveEnable = FALSE;
BOOLEAN                  mSmmDgrProtectedModeEnable;
UINT8                    gSmmDgrEnableState = 0;

extern BOOLEAN           gSmmDgrSpaEnable;
extern UINT64            gSmmDgrSupovrStateLockData;
extern BOOLEAN           gSmmDgrRing3Supported;
extern SPIN_LOCK         *mSmmDgrInternalDebugLock;
extern UINT8             *gSmmDgrExceptionStack;
extern UINTN             gSmmDgrExceptionStackSize;

///
/// Lookup table for the IOMisc width information
///
CONST CPU_SMM_DGR_SAVE_STATE_IO_WIDTH mSmmDgrFeatureCpuIoWidth[] = {
  { 0, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 0
  { 1, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  },  // SMM_IO_LENGTH_BYTE  = 1
  { 2, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT16 },  // SMM_IO_LENGTH_WORD  = 2
  { 0, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 3
  { 4, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT32 },  // SMM_IO_LENGTH_DWORD = 4
  { 0, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 5
  { 0, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  },  // Undefined           = 6
  { 0, EFI_SMM_SAVE_STATE_IO_WIDTH_UINT8  }   // Undefined           = 7
};

///
/// Lookup table for the IOMisc type information
///
CONST EFI_SMM_SAVE_STATE_IO_TYPE mSmmDgrFeatureCpuIoType[] = {
  EFI_SMM_SAVE_STATE_IO_TYPE_OUTPUT,     // SMM_IO_TYPE_OUT_DX        = 0
  EFI_SMM_SAVE_STATE_IO_TYPE_INPUT,      // SMM_IO_TYPE_IN_DX         = 1
  EFI_SMM_SAVE_STATE_IO_TYPE_STRING,     // SMM_IO_TYPE_OUTS          = 2
  EFI_SMM_SAVE_STATE_IO_TYPE_STRING,     // SMM_IO_TYPE_INS           = 3
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 4
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 5
  EFI_SMM_SAVE_STATE_IO_TYPE_REP_PREFIX, // SMM_IO_TYPE_REP_OUTS      = 6
  EFI_SMM_SAVE_STATE_IO_TYPE_REP_PREFIX, // SMM_IO_TYPE_REP_INS       = 7
  EFI_SMM_SAVE_STATE_IO_TYPE_OUTPUT,     // SMM_IO_TYPE_OUT_IMMEDIATE = 8
  EFI_SMM_SAVE_STATE_IO_TYPE_INPUT,      // SMM_IO_TYPE_OUT_IMMEDIATE = 9
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 10
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 11
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 12
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 13
  (EFI_SMM_SAVE_STATE_IO_TYPE)0,         // Undefined                 = 14
  (EFI_SMM_SAVE_STATE_IO_TYPE)0          // Undefined                 = 15
};

/**
  Return the size, in bytes, of a custom SMI Handler in bytes.  If 0 is
  returned, then a custom SMI handler is not provided by this library,
  and the default SMI handler must be used.

  @retval 0    Use the default SMI handler.
  @retval > 0  Use the SMI handler installed by SmmCpuFeaturesInstallSmiHandler()
               The caller is required to allocate enough SMRAM for each CPU to
               support the size of the custom SMI handler.
**/
UINT32
DgrGetSmiHandlerSizeSps (
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
DgrInstallSmiHandlerSps (
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
  SmmCpuFeaturesInstallSmiHandlerSps (CpuIndex, SmBase, SmiStack, StackSize, GdtBase, GdtSize, IdtBase, IdtSize, Cr3);
}

/**
  Patches SMM_SUPOVR_STATELOCK and Ring 3 support values to 0.

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] CpuIndex   The index of the CPU.

  @retval EFI_STATUS    Always return EFI_SUCCESS
**/
EFI_STATUS
DgrPatch0SupovrAndRing3Support (
  IN UINT32   *SmBase,
  IN UINTN    CpuIndex
  )
{
  gSmmDgrSupovrStateLockData = 0;
  PatchSmmEntryPoint ((VOID *) ((UINTN)SmBase[CpuIndex] + SMM_HANDLER_OFFSET), SMM_ENTRY_POINT_INFO_SUPOVR_STATE_LOCK, &gSmmDgrSupovrStateLockData, sizeof(gSmmDgrSupovrStateLockData));
  gSmmDgrRing3Supported = 0;
  PatchSmmEntryPoint ((VOID *) ((UINTN)SmBase[CpuIndex] + SMM_HANDLER_OFFSET), SMM_ENTRY_POINT_INFO_USER_MODE_ENABLE, &gSmmDgrRing3Supported, sizeof(gSmmDgrRing3Supported));

  return EFI_SUCCESS;
}

/**
  Patches SMM_SUPOVR_STATELOCK and Ring 3 support values.

  @param[in] SmBase                The SMBASE address for the CPU specified by CpuIndex.
  @param[in] CpuIndex              The index of the CPU.
  @param[in] SupovrStateLockValue  SMM_SUPOVR_STATE_LOCK MSR value
**/
VOID
DgrPatch1SupovrAndRing3Support (
  IN UINT32   *SmBase,
  IN UINTN    Index,
  IN UINT64   SupovrStateLockValue
  )
{
  gSmmDgrSupovrStateLockData = SupovrStateLockValue;
  PatchSmmEntryPoint ((VOID *) ((UINTN)SmBase[Index] + SMM_HANDLER_OFFSET), SMM_ENTRY_POINT_INFO_SUPOVR_STATE_LOCK, &gSmmDgrSupovrStateLockData, sizeof(gSmmDgrSupovrStateLockData));
  gSmmDgrRing3Supported = 1;
  PatchSmmEntryPoint ((VOID *) ((UINTN)SmBase[Index] + SMM_HANDLER_OFFSET), SMM_ENTRY_POINT_INFO_USER_MODE_ENABLE, &gSmmDgrRing3Supported, sizeof(gSmmDgrRing3Supported));
}

/**
  This function sets
   - Page access rights within SMRAM based on ring separation policy requirements.
   - MMIO access rights.
   - System Memory access rights which are being accessed from SMM.

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
**/
VOID
DgrSetPolicy (
  IN UINT32   *SmBase
  )
{
  SetSmmDgrPolicy (SmBase);
  REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_VPRO_DGR_SET_POLICY);
}

/**
  Function that is called to complete CPU initialization.
**/
VOID
DgrFinishInitializeProcessor (
  VOID
  )
{
  FinishSmmCpuFeaturesInitializeProcessor ();
}

/**
  Get the IOMisc register value

  @param[in] CpuIndex     The index of the CPU.

  @retval IOMISC Register Value
**/
UINT32
DgrStateSaveReadIoMisc (
  IN UINTN  CpuIndex
  )
{
  DGR_COMM_BUFFER             *DgrCommBuffer;

  DgrCommBuffer = (DGR_COMM_BUFFER *) (gSmmDgrExceptionStack + (gSmmDgrExceptionStackSize * CpuIndex));
  return DgrCommBuffer->IoMisc;
}

/**
  Get the IO Info Data register value

  @param[in] CpuIndex    The index of the CPU.
  @param[in] IoWidth     IO width

  @retval IO Info Data
**/
UINT64
DgrStateSaveReadIoInfoData (
  IN UINTN                       CpuIndex,
  IN EFI_MM_SAVE_STATE_IO_WIDTH  IoWidth
  )
{
  UINT64             IoData;
  DGR_COMM_BUFFER    *DgrCommBuffer;

  DgrCommBuffer = (DGR_COMM_BUFFER *) (gSmmDgrExceptionStack + (gSmmDgrExceptionStackSize * CpuIndex));

  //Copy lower 4 bytes of RAX from User exception stack buffer to lower 4 bytes of IoData.
  IoData = DgrCommBuffer->RaxLow;

  // If Io Width is more than 4 bytes then copy higher 4 bytes of RAX from User exception
  // stack buffer to higher 4 bytes of IoData.
  if (IoWidth > EFI_MM_SAVE_STATE_IO_WIDTH_UINT32) {
    CopyMem ((UINT8 *) &(IoData) + 4, (UINT8 *) DgrCommBuffer + 8, 4);
  }

  return IoData;
}

/**
  Get the RAX Data register value

  @param[in] CpuIndex    The index of the CPU.
  @param[in] Width       IO Width
  @param[in] Buffer      Buffer address

  @retval EFI_STATUS    Status returned by worker function
**/
EFI_STATUS
DgrStateSaveReadRax (
  IN UINTN                    CpuIndex,
  IN  UINTN                   Width,
  OUT VOID                    *Buffer
  )
{
  UINT64                      RaxBuffer;
  DGR_COMM_BUFFER             *DgrCommBuffer;
  SMRAM_SAVE_STATE_IOMISC     IoMisc;

  // IoMisc.Uint32 = IOMisc value from user exception stack buffer;
  DgrCommBuffer = (DGR_COMM_BUFFER *) (gSmmDgrExceptionStack + (gSmmDgrExceptionStackSize * CpuIndex));
  IoMisc.Uint32 = DgrCommBuffer->IoMisc;

  //
  // Check for the SMI_FLAG in IOMisc
  //
  if (IoMisc.Bits.SmiFlag == 0) {
    return EFI_NOT_FOUND;
  }

  //
  // Only support IN/OUT, but not INS/OUTS/REP INS/REP OUTS.
  //
  if ((mSmmDgrFeatureCpuIoType[IoMisc.Bits.Type] != EFI_SMM_SAVE_STATE_IO_TYPE_INPUT) &&
      (mSmmDgrFeatureCpuIoType[IoMisc.Bits.Type] != EFI_SMM_SAVE_STATE_IO_TYPE_OUTPUT)) {
    return EFI_NOT_FOUND;
  }

  //
  // Compute index for the I/O Length and I/O Type lookup tables
  //
  if (mSmmDgrFeatureCpuIoWidth[IoMisc.Bits.Length].Width == 0 || mSmmDgrFeatureCpuIoType[IoMisc.Bits.Type] == 0) {
    return EFI_NOT_FOUND;
  }

  if (Width <= sizeof (UINT32)) {
    *(UINT32 *) Buffer = DgrCommBuffer->RaxLow;
  } else {
    RaxBuffer = (UINT32) DgrCommBuffer->RaxHigh;
    *(UINTN *) Buffer = (UINT64) (LShiftU64 (RaxBuffer, 32) | (DgrCommBuffer->RaxLow));
  }

  return EFI_SUCCESS;
}

/**
  Copy lower 32bits of RAX to lower 32Bits of user exception stack buffer.

  @param[in] CpuIndex    The index of the CPU.
  @param[in] Width       IO Width
  @param[in] Buffer      Buffer address
**/
VOID
DgrStateSaveWriteRaxLower (
  IN UINTN       CpuIndex,
  IN UINTN       Width,
  IN CONST VOID  *Buffer
  )
{
  DGR_COMM_BUFFER             *DgrCommBuffer;

  DgrCommBuffer = (DGR_COMM_BUFFER *) (gSmmDgrExceptionStack + (gSmmDgrExceptionStackSize * CpuIndex));

  CopyMem ((UINT8 *) &(DgrCommBuffer->RaxLow), Buffer, MIN(4, Width));
  DgrCommBuffer->Flags = 0x01;
}

/**
  Copy higher 32bits of RAX to higher 32Bits of user exception stack buffer.

  @param[in] CpuIndex    The index of the CPU.
  @param[in] Width       IO Width
  @param[in] Buffer      Buffer address
**/
VOID
DgrStateSaveWriteRaxHigher (
  IN UINTN       CpuIndex,
  IN UINTN       Width,
  IN CONST VOID  *Buffer
  )
{
  DGR_COMM_BUFFER             *DgrCommBuffer;

  DgrCommBuffer = (DGR_COMM_BUFFER *) (gSmmDgrExceptionStack + (gSmmDgrExceptionStackSize * CpuIndex));

  CopyMem ((UINT8 *) &(DgrCommBuffer->RaxHigh), (UINT8 *) Buffer + 4, Width - 4);
}

/**
  Check if DGR is enabled.

  @retval TRUE     If DGR is enabled
  @retval FALSE    If DGR is disabled
**/
BOOLEAN
IsDgrEnable (
  VOID
  )
{
  return mSmmDgrEnable;
}

/**
  Check if DGR State Save is enabled.

  @retval TRUE     If DGR State Save is enabled
  @retval FALSE    If DGR State Save is disabled
**/
BOOLEAN
IsStateSaveEnable (
  VOID
  )
{
  return mSmmDgrStateSaveEnable;
}

/**
  Check if DGR SPA is enabled.

  @retval TRUE     If DGR SPA is enabled
  @retval FALSE    If DGR SPA is disabled
**/
BOOLEAN
IsSpaEnable (
  VOID
  )
{
  return gSmmDgrSpaEnable;
}

/**
  Allocate pages for code.

  @param  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
SmmFeatureAllocateCodePagesDgr (
  IN UINTN           Pages
  )
{
  EFI_STATUS            Status;
  EFI_PHYSICAL_ADDRESS  Memory;

  if (Pages == 0) {
    return NULL;
  }

  Status = gSmst->SmmAllocatePages (AllocateAnyPages, EfiRuntimeServicesCode, Pages, &Memory);
  if (EFI_ERROR (Status)) {
    return NULL;
  }
  return (VOID *) (UINTN) Memory;
}

/**
  The driver entry point for DGR SMM Library.

  @param[in] ImageHandle          Handle for this drivers loaded image protocol.
  @param[in] SystemTable          EFI system table.

  @retval EFI_SUCCESS             The driver installed without error.
  @retval EFI_UNSUPPORTED         The chipset is unsupported by this driver.
  @retval EFI_OUT_OF_RESOURCES    Unable to allocate necessary data structures.
**/
EFI_STATUS
EFIAPI
DgrSmmLibConstructor (
  IN EFI_HANDLE                   ImageHandle,
  IN EFI_SYSTEM_TABLE             *SystemTable
  )
{
  EFI_STATUS                   Status;
#if FixedPcdGetBool (PcdPpamEnable) == 1
  DXE_CPU_POLICY_PROTOCOL      *CpuPolicyData;
  UINTN                        SpinLockSize;
  VOID                         *SpinLockBuffer;
#endif

  DEBUG ((DEBUG_ERROR, "%a Entry\n", __FUNCTION__));
  Status = EFI_SUCCESS;

#if FixedPcdGetBool (PcdPpamEnable) == 1
  Status = gBS->LocateProtocol (&gDxeCpuPolicyProtocolGuid, NULL, (VOID **) &CpuPolicyData);
  gSmmDgrEnableState = NO_NR_PPAM_SUPPORT;
  if (CpuPolicyData->DgrEnable) {
      gSmmDgrEnableState = NiftyRockSupportLevel ();
    if (gSmmDgrEnableState >= NR_PPAM_11_SUPPORT) {
      mSmmDgrEnable = TRUE;
#if FixedPcdGetBool (PcdSpsStateSaveEnable) == 1
      mSmmDgrStateSaveEnable = TRUE;
#endif
      //
      // Related PCD values are retrieved into global variables.
      //
      DEBUG ((DEBUG_INFO, "DGR with Nifty Rock 1.1 feature support is enabled\n"));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_VPRO_DGR_IP_ENABLED);

      //
      // Allocate page for SPIN LOCK buffer
      //
      SpinLockSize   = GetSpinLockProperties ();
      SpinLockBuffer = AllocatePages (EFI_SIZE_TO_PAGES (SpinLockSize));
      if (SpinLockBuffer == NULL) {
        ASSERT (SpinLockBuffer != NULL);
        return EFI_OUT_OF_RESOURCES;
      }
      mSmmDgrInternalDebugLock = (SPIN_LOCK *) SpinLockBuffer;
      InitializeSpinLock (mSmmDgrInternalDebugLock);

      mSmmDgrProtectedModeEnable = PcdGetBool (PcdCpuSmmProtectedModeEnable);

      GetMsegInfo ();
      SpsConstructor ();
    } else {
      DEBUG ((DEBUG_INFO, "DGR feature either NOT support by CPU or disabled in Policy.\n"));
      REPORT_STATUS_CODE (EFI_PROGRESS_CODE, INTEL_RC_STATUS_CODE_VPRO_DGR_IP_DISABLED);
    }
  }
#endif

  DEBUG ((DEBUG_ERROR, "%a Exit\n",__FUNCTION__));

  return Status;
}
