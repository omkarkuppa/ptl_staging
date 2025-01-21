;; @file
;  Assembly code of Cpu
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
; @par Specification
;;

IA32_CR4_OSFXSR           equ        200h
IA32_CR4_OSXMMEXCPT       equ        400h
IA32_CR0_MP               equ        2h

IA32_CPUID_SSE2           equ        02000000h
IA32_CPUID_SSE2_B         equ        26

SECTION .text

;------------------------------------------------------------------------------
;   Set up flags in CR4 for XMM instruction enabling
;------------------------------------------------------------------------------
global ASM_PFX(InitXmm)
ASM_PFX(InitXmm):
    push    rbx

    ; Check whether SSE2 is supported
    mov     eax, 1
    cpuid
    bt      edx, IA32_CPUID_SSE2_B
    jnc     .skipxmm

    ; Enable XMM
    mov     rax, cr0
    or      rax, IA32_CR0_MP
    mov     cr0, rax
    mov     rax, cr4
    or      rax, IA32_CR4_OSFXSR
    or      rax, IA32_CR4_OSXMMEXCPT
    mov     cr4, rax

.skipxmm:
    pop     rbx
    ret

;------------------------------------------------------------------------------
;   Invalidate cache
;------------------------------------------------------------------------------
global ASM_PFX(CacheInvd)
ASM_PFX(CacheInvd):
    invd
    ret
