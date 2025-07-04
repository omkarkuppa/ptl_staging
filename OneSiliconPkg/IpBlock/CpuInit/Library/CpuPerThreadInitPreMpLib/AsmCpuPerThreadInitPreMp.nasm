; @file
; Assembly code to CPU Per Thread Pre-MP Library
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2025 Intel Corporation.
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

global ASM_PFX(gApInitPreMpTemplate)
global ASM_PFX(gApInitPreMpTemplateSize)

global ASM_PFX(gLockstepEnableOffset)
global ASM_PFX(gApicIdMaskOffset)

global ASM_PFX(gX2ApicEnableOffset)
global ASM_PFX(gApCountOffset)


SECTION .text

BITS 16
ASM_PFX(gApInitPreMpTemplate):
; At this point CS = 0x(vv00) and ip= 0x0.
    mov        ax,  cs
    mov        ds,  ax
    mov        es,  ax
    mov        ss,  ax
    xor        ax,  ax
    mov        fs,  ax
    mov        gs,  ax

    xor   ebx, ebx

    mov   si, LockstepEnable - ASM_PFX(gApInitPreMpTemplate)
    mov   eax, dword[si]    ; EAX contains LockstepEnable
    cmp   eax, 0
    je    ConfigX2Apic      ; If LockstepEnable is 0, skip Lockstep mode setting

    xor   ecx, ecx
    mov   eax, 0x1a
    cpuid                   ; EDX contains CORE TYPE
    mov   edx, 0x20000000
    and   eax, 0x20000000
    cmp   edx, eax
    jne   loopjump          ; Skip not ATOM

    xor   ecx, ecx
    mov   eax, 0x1f
    cpuid                   ; EDX contains APIC_ID
    mov   si, ApicIdMask - ASM_PFX(gApInitPreMpTemplate)
    mov   eax, dword[si]    ; EAX contains ApicIdMask

    mov ebx, edx
    mov cx, 0x4             ; APIC ID 0x4x
    shr bx, 4
    cmp cx, bx
    jne lowerid
    shr ax, 8

lowerid:
    and   edx, 0x0f
    shr   dx, 1
    mov   cx, dx
    mov   bx, 0x01
    shl   bx, cl
    and   ax, bx
    mov   dx, ax

    xor   eax, eax          ; clear lower half of 64 bit data
    xor   ecx, ecx
    cmp   dx, bx
    jne   SkipEnableLSM

    ; Set Lockstep mode based on the ApicIdMask. Skip if the bit is 0

    mov   si, ApCount - ASM_PFX(gApInitPreMpTemplate)
    lock dec   dword[si]    ; decrease ApCount
lockloop:
    mov   si, ApCount - ASM_PFX(gApInitPreMpTemplate)
    mov   ebx, dword[si]    ; EAX contains LockstepEnable
    cmp   ebx, 0
    jne    lockloop          ; If LockstepEnable is 0, skip Lockstep mode setting

    mov   eax, 1            ; lower half of 64 bit data, enable LSM

SkipEnableLSM:
    mov   edx, 0            ; upper half of 64 bit data
    mov   ebx, 0            ; upper half of 64 bit data
    mov   ecx, 0x2d8        ; MSR 0x2d8 is MSR_ENABLE_LSM
    wrmsr
    nop
    ;mov   si, ApCount - ASM_PFX(gApInitPreMpTemplate)
    ;lock dec   dword[si]    ; decrease ApCount
loopend:
    nop
    nop
    hlt
    jmp        loopend

ConfigX2Apic:
    mov   si, X2ApicEnable - ASM_PFX(gApInitPreMpTemplate)
    mov   eax, dword[si]    ; EAX contains X2ApicEnable
    cmp   eax, 0
    je    ApCountIncrease   ; If X2ApicEnable is 0, skip X2Apic mode enabling
    mov   ecx, 0x01b        ; MSR 0x1B is APIC_BASE_MSR
    rdmsr                   ; EDX:EAX := MSR[ECX];
    bts   eax, 10           ; bit 10 is APIC_BASE_MSR EXTD bit
    wrmsr                   ; MSR[ECX] := EDX:EAX;

ApCountIncrease:
    mov   si, ApCount - ASM_PFX(gApInitPreMpTemplate)
    lock inc   dword[si]    ; increase ApCount

loop:
    nop
    nop
    hlt
    jmp        loop

loopjump:
    nop
    nop
    nop
    xor   eax, eax
    jmp        SkipEnableLSM

LockstepEnable:
    dd         0      ; receive Lockstep enable
ApicIdMask:
    dd         0      ; to be fixed by C code

X2ApicEnable:
    dd         0      ; receive X2Apic enable
ApCount:
    dd         0      ; will be the count of all APs

;
; Compute offsets to pass data from C code

ASM_PFX(gLockstepEnableOffset)               : DW  LockstepEnable  - ASM_PFX(gApInitPreMpTemplate)
ASM_PFX(gApicIdMaskOffset)                   : DW  ApicIdMask      - ASM_PFX(gApInitPreMpTemplate)

ASM_PFX(gX2ApicEnableOffset)                 : DW  X2ApicEnable    - ASM_PFX(gApInitPreMpTemplate)
ASM_PFX(gApCountOffset)                      : DW  ApCount         - ASM_PFX(gApInitPreMpTemplate)
ASM_PFX(gApInitPreMpTemplateSize)            : DW  $               - ASM_PFX(gApInitPreMpTemplate)
