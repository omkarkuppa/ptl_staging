;; @file
;  This is the code that goes from real-mode to protected mode.
;  It consumes the reset vector, calls TempRamInit API from FSP binary.
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2022 Intel Corporation.
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

#include "FspHelper.h"

IA32_CR4_OSFXSR           equ        200h
IA32_CR4_OSXMMEXCPT       equ        400h
IA32_CR0_MP               equ        2h

IA32_CPUID_SSE2           equ        02000000h
IA32_CPUID_SSE2_B         equ        26

SECTION .text

extern   ASM_PFX(FspLoadComponents)
extern   ASM_PFX(PcdGet32 (PcdFlashFvFsptBase))


struc FitTableEntry
    .FItEntryAddress:               resq    1
    .FItEntrySize0:                 resb    1
    .FItEntrySize1:                 resb    1
    .FItEntrySize2:                 resb    1
    .FItEntryReserved:              resb    1
    .FItEntryVersion:               resw    1
    .FItEntryTypeCv:                resb    1
    .FItEntryChecksum:              resb    1
    .size:
endstruc

struc BpmElementHeader
    .BpmStructureId                 resq   1
    .BpmStructVersion               resb   1
    .BpmStructureReserved0          resb   1
    .BpmStructureElementSize        resw   1
    .size:
endstruc

struc PlatformConfigDataElement
    .PlatformConfigStructureId      resq   1
    .PlatformConfigStructVersion    resb   1
    .PlatformConfigReserved0        resb   1
    .PlatformConfigElementSize      resw   1
    .PlatformConfigReserved1        resw   1
    .PlatformConfigSizeOfData       resw   1
    .PlatformConfigData             resq   1
    .size:
endstruc

struc PcdeElement
  .PcdeElementStructureId    resq   1
  .PcdeElementStructVersion  resb   1
  .PcdeElementReserved0      resb   1
  .PcdeElementSizeOfData     resw   1
  .size:
endstruc

struc BspmElement
  .BspmElementStructureID        resq   1
  .BspmElementStructVersion      resb   1
  .BspmElementSizeOfElement      resw   1
  .BspmElementReserved0          resb   1
  .BspmElementBspEntryPoint      resd   1
  .BspmElementFsptUpdAddress     resd   1
  ; The following fields is reserved because BspmElement.FsptUpd is taken by address
  ; .BspmElementFspmLoadingPolicy  resb   1
  ; .BspmElementReserved1          resb   3
  ; .BspmElementFspmBaseAddress    resd   1
  ; .BspmElementFspmSize           resd   1
  ; .BspmElementBspBaseAddress     resd   1
  ; .BspmElementBspSize            resd   1
  ; .BspmElementSegmentCount       resd   1
  ;REGION_SEGMENT               SegmentArray[SegmentCount];
  ;SHAX_HASH_STRUCTURE       DigestData;
  .size:
endstruc
;-----------------------------------------------------------------------------
;  Macro:        PUSHA_64
;
;  Description:  Saves all registers on stack
;
;  Input:        None
;
;  Output:       None
;-----------------------------------------------------------------------------
%macro PUSHA_64   0
  push    r8
  push    r9
  push    r10
  push    r11
  push    r12
  push    r13
  push    r14
  push    r15
  push    rax
  push    rcx
  push    rdx
  push    rbx
  push    rsp
  push    rbp
  push    rsi
  push    rdi
%endmacro

;-----------------------------------------------------------------------------
;  Macro:        POPA_64
;
;  Description:  Restores all registers from stack
;
;  Input:        None
;
;  Output:       None
;-----------------------------------------------------------------------------
%macro POPA_64   0
  pop    rdi
  pop    rsi
  pop    rbp
  pop    rsp
  pop    rbx
  pop    rdx
  pop    rcx
  pop    rax
  pop    r15
  pop    r14
  pop    r13
  pop    r12
  pop    r11
  pop    r10
  pop    r9
  pop    r8
%endmacro

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
;   This driver launches Fsp Temp Ram Init API and give control to Bsp Sec Core by consuming Bsp manifest binary.
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
  mov     esi, FIT_POINTER_LOCATION
  mov     esi, dword [esi]
  mov     rax, qword [esi]
  mov     rdx, FIT_SIGNATURE
  cmp     rax, rdx
  jnz     FailedToGetFspeFromBpm
  xor     rax, rax
  xor     rdx, rdx
  ; Get number of Fit entries
  mov     dl,  byte [esi + FitTableEntry.FItEntrySize0]
  mov     ax,  word [esi + FitTableEntry.FItEntrySize1]
  shl     eax,  8
  or      eax, edx
  sub     eax, 1
  mov     edx, FitTableEntry.size
  mul     edx
  add     esi, eax
  ; esi points to the last entry in the Fit Table
  ; and search till the First entry to find the
  ; Bpm address in FLash
SearchBpmFitEntry:
  mov     al, byte [esi + FitTableEntry.FItEntryTypeCv]
  and     al, 7fh
  cmp     al, 00h
  jz      FailedToGetFspeFromBpm
  cmp     al, 0ch
  jz      BpmFitEntryFound
  sub     esi, FitTableEntry.size
  jmp     SearchBpmFitEntry
BpmFitEntryFound:
  mov     rdi, qword [esi]
  cmp     rdi, 0
  jz      FailedToGetFspeFromBpm
  mov     rax, qword [edi]
  mov     rdx, BPM_HEADER_STRUCTURE_ID
  cmp     rax, rdx
  jnz     FailedToGetFspeFromBpm
  xor     rax, rax
  xor     rdx, rdx
  mov     dl,  byte [esi + FitTableEntry.FItEntrySize0]
  mov     ax,  word [esi + FitTableEntry.FItEntrySize1]
  shl     eax,  8
  or      eax, edx
  ; After this, ESI hold start address and EDI hold End address
  ; of Bpm Data and traverse through to find PlatformConfigData
  ; data
  mov     esi, edi
  add     edi, eax
  xor     rax, rax
  mov     ax, word [esi + BpmElementHeader.BpmStructureElementSize]
  add     esi, eax
SearchPcdeElement:
  cmp    esi, edi
  ja     FailedToGetFspeFromBpm
  mov    rax, qword [esi]
  mov    rdx, BPM_SIGNATURE_ELEMENT_STRUCTURE_ID
  cmp    rax, rdx
  jz     FailedToGetFspeFromBpm
  mov    rdx, BPM_PLATFORM_CONFIG_DATA_ELEMENT_STRUCTURE_ID
  cmp    rax, rdx
  jz     PcdeElementFound
  xor    rax, rax
  mov    ax, word [esi + BpmElementHeader.BpmStructureElementSize]
  add    esi, eax
  jmp    SearchPcdeElement
PcdeElementFound:
  add    esi, PlatformConfigDataElement.PlatformConfigData
  xor    rax, rax
  mov    ax, word [esi + PlatformConfigDataElement.PlatformConfigElementSize]
  add    edi, eax
  ; Bsss is part of PlatformConfigData and search for BSIS
  ; signature
SearchBsisSegment:
  cmp    esi, edi
  ja     FailedToGetFspeFromBpm
  mov    rax, qword[esi]
  mov    rdx, BPM_BSIS_ELEMENT_STRUCTURE_ID
  cmp    rax, rdx
  jz     BsisSegmentFound
  xor    rax, rax
  mov    ax, word [esi + PcdeElement.PcdeElementSizeOfData]
  add    esi, eax
  add    esi, PcdeElement.size
  jmp    SearchBsisSegment
BsisSegmentFound:
  add    esi, PcdeElement.size
  mov    rax, qword[esi]
  mov    rdx, BPM_BSSS_BSPM_STRUCTURE_ID
  cmp    rax, rdx
  jz    FoundBspm
FailedToGetFspeFromBpm:
  ; Search in Bsp Region near 4 GB
  mov     esi, BSSS_SEARCH_START_ADDRESSS
  mov     rdx, BPM_BSSS_BSPM_STRUCTURE_ID
SearchBspm:
  mov     rax, qword[esi]
  cmp     rax, rdx
  jz      FoundBspm
  add     esi, 0x20 ; Search for BSSS on every 32 byte Boundary
  cmp     esi, BSSS_SEARCH_END_ADDRESSS
  jz      FspApiFailed
  jmp     SearchBspm
FoundBspm:
  ; Find the fsp info header
  mov     rdi, ASM_PFX(PcdGet32 (PcdFlashFvFsptBase))
  mov     edi,  dword [rdi]

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
  xor     ecx, ecx
  mov     ecx, esi
  add     ecx, BspmElement.BspmElementFsptUpdAddress
  ; Setup the hardcode stack
  mov     rsp, TempRamInitStack

  ; Call the fsp TempRamInit Api
  jmp     rax

TempRamInitDone:
  mov     rsp, rdx
  ;
  ; Per X64 calling convention, make sure RSP is 16-byte aligned.
  ;
  mov     rax, rsp
  and     rax, 0fh
  sub     rsp, rax

  ; Load FSPM and BspPremem in FSP Signing flow
  PUSHA_64
  sub     rsp, 20h
  mov     rcx, rdx
  call    ASM_PFX(FspLoadComponents)
  add     rsp, 20h
  POPA_64

  xor     eax, eax
  mov     eax, dword [esi + BspmElement.BspmElementBspEntryPoint]
  jmp     rax

FspApiFailed:
  jmp     $

align 10h
TempRamInitStack:
    DQ  TempRamInitDone
