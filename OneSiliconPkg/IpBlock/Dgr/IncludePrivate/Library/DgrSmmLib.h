/** @file
  Include file for DGR SMM APIs

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
#ifndef _DGR_SMM_API_H_
#define _DGR_SMM_API_H_

#include <PiSmm.h>

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
  );


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
  );

/**
  Get ExceptionStack information

  @param[out] ExceptionStack      Exception Stack pointer
  @param[out] ExceptionStackSize  Size of Exception Stack
**/
VOID
DgrGetExceptionStack (
  OUT UINT8  *ExceptionStack,
  OUT UINTN  ExceptionStackSize
  );

/**
  Patches SMM_SUPOVR_STATELOCK and Ring 3 support values to 0.

  @param[in] SmBase     The SMBASE address for the CPU specified by CpuIndex.
  @param[in] CpuIndex   The index of the CPU.

  @retval EFI_STATUS    Always return EFI_SUCCESS
**/
EFI_STATUS
DgrPatch0SupovrAndRing3Support (
  IN UINT32  *SmBase,
  IN UINTN   CpuIndex
  );

/**
  Patches SMM_SUPOVR_STATELOCK and Ring 3 support values to 1.

  @param[in] SmBase                The SMBASE address for the CPU specified by CpuIndex.
  @param[in] CpuIndex              The index of the CPU.
  @param[in] SupovrStateLockValue  SMM_SUPOVR_STATE_LOCK MSR value
**/
VOID
DgrPatch1SupovrAndRing3Support (
  IN UINT32   *SmBase,
  IN UINTN    Index,
  IN UINT64   SupovrStateLockValue
  );

/**
  This function sets
   - Page access rights within SMRAM based on ring separation policy requirements.
   - MMIO access rights.
   - System Memory access rights which are being accessed from SMM.

  @param[in] SmBase                The SMBASE address for the CPU specified by CpuIndex.
**/
VOID
DgrSetPolicy (
  IN UINT32   *SmBase
  );

/**
  Function that is called to complete CPU initialization.
**/
VOID
DgrFinishInitializeProcessor (
  VOID
  );

/**
  Get the IOMisc register value

  @param[in] CpuIndex    The index of the CPU.

  @retval IOMISC Register Value
**/
UINT32
DgrStateSaveReadIoMisc (
  IN UINTN    CpuIndex
  );

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
  );

/**
  Get the RAX Data register value

  @param[in] CpuIndex    The index of the CPU.
  @param[in] Width       IO Width
  @param[in] Buffer      Buffer address

  @retval EFI_STATUS    Always return EFI_SUCCESS
**/
EFI_STATUS
DgrStateSaveReadRax (
  IN UINTN                    CpuIndex,
  IN  UINTN                   Width,
  OUT VOID                    *Buffer
  );

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
  );

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
  );

/**
  Check if DGR is enabled.

  @retval TRUE     If DGR is enabled
  @retval FALSE    If DGR is disabled
**/
BOOLEAN
IsDgrEnable (
  VOID
  );

/**
  Check if DGR State Save is enabled.

  @retval TRUE     If DGR State Save is enabled
  @retval FALSE    If DGR State Save is disabled
**/
BOOLEAN
IsStateSaveEnable (
  VOID
  );

/**
  Check if DGR SPA is enabled.

  @retval TRUE     If DGR SPA is enabled
  @retval FALSE    If DGR SPA is disabled
**/
BOOLEAN
IsSpaEnable (
  VOID
  );
#endif
