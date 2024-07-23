;; @file
;  This is the code that provide early platform initialization and pass control into PEI core.
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
;
;;

%include "PlatformNasm.inc"
%include "ChipsetNasm.inc"
%include "SecCoreNasm.inc"

extern   ASM_PFX(SecHostBridgeInit)
extern   ASM_PFX(PcdGet64 (PcdMchBaseAddress))

extern   ASM_PFX(SecCarInit)
extern   ASM_PFX(PcdGet32 (PcdFlashFvBfvBase))
extern   ASM_PFX(PcdGet32 (PcdPeiCoreEntryBase))



;
; Define SSE macros using SSE 4.1 instructions
; Save REG to XMM
; args 1:XMM, 2:IDX, 3:REG
;
%macro SXMMN        3
            pinsrq  %1, %3, (%2 & 3)
            %endmacro

;
; Restore REG from XMM
; args 1:XMM, 2:REG, 3:IDX
;
%macro LXMMN        3
            pextrq  %2, %1, (%3 & 3)
            %endmacro

;
; Define AVX macros using AVX instructions
; Save XMM to YMM
; args 1:YMM, 2:IDX (0 - lower 128bits, 1 - upper 128bits), 3:XMM
%macro SYMMN        3
            vinsertf128  %1, %1, %3, %2
            %endmacro

;
; Restore XMM from YMM
; args 1:YMM, 2:XMM, 3:IDX (0 - lower 128bits, 1 - upper 128bits)
;
%macro LYMMN        3
            vextractf128  %2, %1, %3
            %endmacro

;
; Save Time Stamp to YMM6[192:255]
; arg 1:general purpose register which holds time stamp
; Modified: XMM5 and YMM6
;
%macro SAVE_TS      1
            LYMMN   ymm6, xmm5, 1
            SXMMN   xmm5, 1, %1
            SYMMN   ymm6, 1, xmm5
            %endmacro

;
; Restore Time Stamp from YMM6[192:255]
; arg 1:general purpose register where to save time stamp
; Modified: XMM5 and %1
;
%macro LOAD_TS      1
            LYMMN   ymm6, xmm5, 1
            LXMMN   xmm5, %1, 1
            %endmacro

;
; YMM7[128:191] for calling stack
; arg 1:Entry
; Modified: RSI, XMM5, YMM7
;
%macro CALL_YMM     1
            mov     rsi, %%ReturnAddress
            LYMMN   ymm7, xmm5, 1
            SXMMN   xmm5, 0, rsi
            SYMMN   ymm7, 1, xmm5
            mov     rsi,  %1
            jmp     rsi
%%ReturnAddress:
            %endmacro

;
; Restore RIP from YMM7[128:191]
; Modified: RSI, XMM5
;
%macro RET_YMM      0
            LYMMN   ymm7, xmm5, 1
            movq    rsi, xmm5
            jmp     rsi
            %endmacro

;
; RBP for calling stack
; args 1:RoutineLabel
; Modified: RBP
;
%macro CALL_RBP  1
  mov     rbp,  %%ReturnAddress
  jmp     %1
%%ReturnAddress:
%endmacro

;
; args none
;
%macro RET_RBP  0
  jmp     rbp                  ; restore RIP from RBP
%endmacro

SECTION .text

;------------------------------------------------------------------------------
;
;  SEC "Security" Code module.
;
;  IA32 specific cache as RAM modules
;  This code also enables the Cache-as-RAM
;
;  RETURNS:    none
;
;  MMX Usage:
;              MM0 = BIST State
;              MM3 = Patch Revision
;              MM4 = Patch Pointer
;              YMM6[192:255] = Save time-stamp counter value
;              YMM7[128:191] = Used in CALL_YMM & RET_YMM macro
;
;------------------------------------------------------------------------------

;
; Perform early platform initialization
;

global ASM_PFX(SecPlatformInit)
ASM_PFX(SecPlatformInit):

  CALL_RBP  ASM_PFX(SecHostBridgeInit)

  ;
  ; Enable HPET decode in PCH.
  ;
  ;
  ; Read PCI Express base address
  ;
  mov     eax,  PCIEX_BAR_REG
  mov     dx,   0CF8h
  out     dx,   eax
  mov     dx,   0CFCh
  in      eax,  dx
  and     eax,  0xFC000000
  mov     esi,  eax
  add     esi,  PCH_HPET_REG
  mov     al,   PCH_HPET_AE
  mov     Byte  [esi], al
  mov     al,   Byte [esi]                     ; read back
  xor     rax,  rax
  mov     esi,  HPET_COMP_1
  mov     Dword [esi], eax
  mov     esi,  HPET_COMP_2
  mov     Dword [esi], eax
  RET_YMM

;
; Establish Stack
;
global ASM_PFX(EstablishStack)
ASM_PFX(EstablishStack):
  ;
  ; Enable STACK
  ;
  RET_YMM
