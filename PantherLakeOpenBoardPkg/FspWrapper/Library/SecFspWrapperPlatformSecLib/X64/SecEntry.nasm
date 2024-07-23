;; @file
;  This is the code that calls TempRamInit API from FSP binary and passes
;  control into PEI core.
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2023 Intel Corporation.
;
;  This software and the related documents are Intel copyrighted materials,
;  and your use of them is governed by the express license under which they
;  were provided to you ("License"). Unless the License provides otherwise,
;  you may not use, modify, copy, publish, distribute, disclose or transmit
;  this software or the related documents without Intel's prior written
;  permission.
;
;  This software and the related documents are provided as is, with no
;  express or implied warranties, other than those that are expressly stated
;  in the License.
;
;@par Specification Reference:
;;

#include "Fsp.h"

IA32_CR4_OSFXSR           equ        200h
IA32_CR4_OSXMMEXCPT       equ        400h
IA32_CR0_MP               equ        2h

IA32_CPUID_SSE2           equ        02000000h
IA32_CPUID_SSE2_B         equ        26

SECTION .text

extern   ASM_PFX(CallPeiCoreEntryPoint)
extern   ASM_PFX(FsptUpdDataPtr)

; Pcds
extern   ASM_PFX(PcdGet32 (PcdFsptBaseAddress))
extern   ASM_PFX(PcdGet32 (PcdFspTemporaryRamSize))

;----------------------------------------------------------------------------
;
; Procedure:    _ModuleEntryPoint
;
; Input:        None
;
; Output:       None
;
; Destroys:     Assume all registers
;
; Description:
;
;  Call TempRamInit API from FSP binary. After TempRamInit done, pass
;  control into PEI core.
;
; Return:       None
;
;  MMX Usage:
;              MM0 = BIST State
;
;----------------------------------------------------------------------------

BITS 64
align 16
global ASM_PFX(_ModuleEntryPoint)
ASM_PFX(_ModuleEntryPoint):
#if FixedPcdGet8(PcdFspWrapperResetVectorInFsp) == 1
  ;
  ; When Fsp holds reset vector, TempRamInitApi would be called by Fsp itself
  ; and this will be the first call to FspWrapper. So, Skip calling TempRamInitApi
  ; and proceed further to launch PeiCore Entry
  ;
  jmp    CallSecFspInit
#endif
  fninit                                ; clear any pending Floating point exceptions
  ;
  ; Store the BIST value in mm0
  ;
  movd    mm0, eax
  cli

  ;
  ; Check INIT#
  ; Trigger warm reset if PCIEBAR register is not in reset/default value state
  ;
  mov     eax, 80000060h ; PCIEX_BAR_REG B0:D0:F0:R60
  mov     dx,  0CF8h
  out     dx,  eax
  mov     dx,  0CFCh
  in      eax, dx
  cmp     eax, 0
  jz      NotWarmStart

  ;
  ; @note Issue warm reset, since if CPU only reset is issued not all MSRs are restored to their defaults
  ;
  mov     dx, 0CF9h
  mov     al, 06h
  out     dx, al
  jmp     $

NotWarmStart:

  ; Find the fsp info header
  mov     rax, ASM_PFX(PcdGet32 (PcdFsptBaseAddress))
  mov     edi, [eax]

  mov     eax, dword [edi + FVH_SIGINATURE_OFFSET]
  cmp     eax, FVH_SIGINATURE_VALID_VALUE
  jnz     FspHeaderNotFound

  xor     eax, eax
  mov     ax, word [edi + FVH_EXTHEADER_OFFSET_OFFSET]
  cmp     ax, 0
  jnz     FspFvExtHeaderExist

  xor     eax, eax
  mov     ax, word [edi + FVH_HEADER_LENGTH_OFFSET]     ; Bypass Fv Header
  add     edi, eax
  jmp     FspCheckFfsHeader

FspFvExtHeaderExist:
  add     edi, eax
  mov     eax, dword [edi + FVH_EXTHEADER_SIZE_OFFSET]  ; Bypass Ext Fv Header
  add     edi, eax

  ; Round up to 8 byte alignment
  mov     eax, edi
  and     al,  07h
  jz      FspCheckFfsHeader

  and     edi, 0FFFFFFF8h
  add     edi, 08h

FspCheckFfsHeader:
  ; Check the ffs guid
  mov     eax, dword [edi]
  cmp     eax, FSP_HEADER_GUID_DWORD1
  jnz     FspHeaderNotFound

  mov     eax, dword [edi + 4]
  cmp     eax, FSP_HEADER_GUID_DWORD2
  jnz     FspHeaderNotFound

  mov     eax, dword [edi + 8]
  cmp     eax, FSP_HEADER_GUID_DWORD3
  jnz     FspHeaderNotFound

  mov     eax, dword [edi + 0Ch]
  cmp     eax, FSP_HEADER_GUID_DWORD4
  jnz     FspHeaderNotFound

  add     edi, FFS_HEADER_SIZE_VALUE         ; Bypass the ffs header

  ; Check the section type as raw section
  mov     al, byte [edi + SECTION_HEADER_TYPE_OFFSET]
  cmp     al, 019h
  jnz FspHeaderNotFound

  add     edi, RAW_SECTION_HEADER_SIZE_VALUE ; Bypass the section header
  jmp     FspHeaderFound

FspHeaderNotFound:
  jmp     $

FspHeaderFound:
  ; Get the fsp TempRamInit Api address
  mov     eax, dword [edi + FSP_HEADER_IMAGEBASE_OFFSET]
  add     eax, dword [edi + FSP_HEADER_TEMPRAMINIT_OFFSET]

  ; Setup the hardcode stack
  mov     rsp, TempRamInitStack   ; move return address to rsp
  mov     rcx, ASM_PFX(FsptUpdDataPtr)  ; TempRamInitParams

  ; Call the fsp TempRamInit Api
  jmp     rax

TempRamInitDone:
  mov     rbx, 0800000000000000Eh
  cmp     rax, rbx                ; Check if EFI_NOT_FOUND returned. Error code for Microcode Update not found.
  je      CallSecFspInit          ; If microcode not found, don't hang, but continue.

  test    rax, rax                ; Check if EFI_SUCCESS returned.
  jnz     FspApiFailed

  ; RDX: start of range
  ; R8: end of range
CallSecFspInit:
#if FixedPcdGet8(PcdFspModeSelection) == 1
  push    rax
  mov     rax, ASM_PFX(PcdGet32 (PcdFspTemporaryRamSize))
  sub     edx, dword [rax] ; TemporaryRam for FSP
  pop     rax
#endif

  mov     r8,  rdx
  mov     rdx, rcx
  xor     ecx, ecx ; zero - no Hob List Yet
  mov     rsp, r8

  ;
  ; Per X64 calling convention, make sure RSP is 16-byte aligned.
  ;
  mov     rax, rsp
  and     rax, 0fh
  sub     rsp, rax

  call    ASM_PFX(CallPeiCoreEntryPoint)

FspApiFailed:
  jmp     $

align 10h
TempRamInitStack:
    DQ  TempRamInitDone
