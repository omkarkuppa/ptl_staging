;; @file
;  Code template of the SMI handler for a particular processor
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

%include "StuffRsbNasm.inc"

%define MSR_IA32_SYSENTER_ESP  0x175
%define MSR_IA32_SYSENTER_EIP  0x176

extern ASM_PFX(SmiRendezvous)
extern ASM_PFX(OemExceptionHandler)
#if FixedPcdGetBool(PcdSpaEnable) == 1
extern ASM_PFX(SpaLogOutputCb)
#endif

SECTION .data

    DEFAULT REL
SECTION .text

global ASM_PFX(AsmDgrSmiRendezvous)
ASM_PFX(AsmDgrSmiRendezvous):
    ; rsp is the user stack
    mov     rcx, [rsp] ; rcx = CpuIndex

    ; rendezvous enter
    sub     rsp, 0x28
    ;DB      0x48, 0xb8                  ; mov rax, imm64
    ;DQ      ASM_PFX(SmiRendezvous)
    call    ASM_PFX(SmiRendezvous)
    add     rsp, 0x28
    ; rendezvous exit

    mov     eax, cs
    and     al, 0x3
    cmp     al, 0
    jnz     BackToRing0

    xor      rax, rax
    mov      ecx, MSR_IA32_SYSENTER_ESP
    rdmsr
    mov      rsp, rax
    mov      ecx, MSR_IA32_SYSENTER_EIP
    rdmsr
    jmp      rax

BackToRing0:
    ; prepare enter ring 0
    SYSENTER
    jmp     $

global ASM_PFX(AsmOemExceptionHandler)
ASM_PFX(AsmOemExceptionHandler):

    mov     rcx, [rsp + 8]
    mov     rdx, [rsp + 0x10]

    ; rendezvous enter
    sub     rsp, 0x28
    ;DB      0x48, 0xb8                  ; mov rax, imm64
    ;DQ      ASM_PFX(OemExceptionHandler)
    call    ASM_PFX(OemExceptionHandler)
    add     rsp, 0x28
    ; rendezvous exit

    ; prepare enter ring 0
    SYSENTER
    jmp     $

#if FixedPcdGetBool(PcdSpaEnable) == 1
global ASM_PFX(AsmSpaLogOutputCb)
ASM_PFX(AsmSpaLogOutputCb):

    mov     rcx, [rsp + 0x18]        ; Log String

    ; Enter into SPA Log Output Call back function
    sub     rsp, 0x28
    ;DB      0x48, 0xb8                  ; mov rax, imm64
    ;DQ      ASM_PFX(SpaLogOutputCb)
    call    ASM_PFX(SpaLogOutputCb)
    add     rsp, 0x28
    ; Exit SPA Log Output Call back function

    ; prepare enter ring 0
    SYSENTER
    jmp     $
#endif
