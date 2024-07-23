;; @file
;  Exception handlers used in SMM mode
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
;@par Specification Reference:
;;

SECTION .data

global   ASM_PFX(gSmiExceptionCr3)
extern   ASM_PFX(mMceHandlerLongMode)
extern   ASM_PFX(gSmmFeatureSmiHandlerIdtr)

;
; Below Global Descriptor Table has been overridden from original table from
; Edk2\UefiCpuPkg\PiSmmCpuDxeSmm\X64\SmiException.nasm. GDT override in this file
; can be removed once Edk2 open source has support for user mode descriptors and
; full TSS support.
;
NullSeg: DQ 0                   ; reserved by architecture
CodeSeg32:
            DW      -1                  ; 8 LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
ProtModeCodeSeg32:
            DW      -1                  ; 0x10 LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
ProtModeSsSeg32:
            DW      -1                  ; 0x18 LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
DataSeg32:
            DW      -1                  ; 0x20 LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
CodeSeg16:
            DW      -1                  ;0x28
            DW      0
            DB      0
            DB      0x9b
            DB      0x8f
            DB      0
DataSeg16:
            DW      -1                  ;0x30
            DW      0
            DB      0
            DB      0x93
            DB      0x8f
            DB      0
CodeSeg64:
            DW      -1                  ; 0x38 LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x9b
            DB      0xaf                ; LimitHigh
            DB      0                   ; BaseHigh
DataSeg64:                              ; 0x40 ; It must be CodeSeg64 + 8
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x93
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
R3CodeSeg32:                            ; 0x48 ; It must be R3CodeSeg64 - 16
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xfb
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
R3DataSeg32:                            ; 0x50 ; It must be R3CodeSeg64 - 8
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xf3
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh
R3CodeSeg64:                            ; 0x58 ; It must be CodeSeg64 + 32
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xfb
            DB      0xaf                ; LimitHigh
            DB      0                   ; BaseHigh
R3DataSeg64:                            ; 0x60 ; It must be R3CodeSeg64 + 8
            DW      -1                  ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0xf3
            DB      0xcf                ; LimitHigh
            DB      0                   ; BaseHigh

            DQ      0
; TSS Segment for X64 specially
TssSeg:                                 ; 0x70
            DW      TSS_DESC_SIZE - 1   ; LimitLow
            DW      0                   ; BaseLow
            DB      0                   ; BaseMid
            DB      0x89
            DB      0x00                ; LimitHigh
            DB      0                   ; BaseHigh
            DD      0                   ; BaseUpper
            DD      0                   ; Reserved

GDT_SIZE equ $ -   NullSeg

; Create TSS Descriptor just after GDT
TssDescriptor:
            DD      0                   ; Reserved
            DQ      0                   ; RSP0
            DQ      0                   ; RSP1
            DQ      0                   ; RSP2
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DQ      0                   ; IST1
            DQ      0                   ; IST2
            DQ      0                   ; IST3
            DQ      0                   ; IST4
            DQ      0                   ; IST5
            DQ      0                   ; IST6
            DQ      0                   ; IST7
            DD      0                   ; Reserved
            DD      0                   ; Reserved
            DW      0                   ; Reserved
            DW      104                 ; I/O Map Base Address
times 0x2000 DB     0x00                ; IO map: 0x10000/8 = full IO port
            DB      0xFF                ; Last byte of bitmap must 0xFF
TSS_DESC_SIZE equ $ -   TssDescriptor

ASM_PFX(gSmiExceptionCr3) EQU gSmiExceptionCr3Patch - 4
;
; Need to figure out how to get these values from GDT
; established in SMM module
;
%define CODE_SEL 0x38
%define DATA_SEL 0x20
%define CODE32_SEL 0x8

    DEFAULT REL
    SECTION .text

global ASM_PFX(PMIdtHandler)
ASM_PFX(PMIdtHandler):
    jmp    $
    retf

global ASM_PFX(PMIdtHandlerMachineCheck)
ASM_PFX(PMIdtHandlerMachineCheck):
    sub     esp, 8
    sidt    [rsp]            ; save 32bit IDTR
    push    rax
    push    rdx
    push    rcx

    mov eax, strict dword 0
gSmiExceptionCr3Patch:
    mov     cr3, rax

    mov     eax, 0x668                  ; as cr4.PGE is not set here, refresh cr3
    mov     cr4, rax                   ; in PreModifyMtrrs() to flush TLB.

; save old ss:esp
    mov     eax, ss
    push    rax
    push    rsp
    cmp     eax, DATA_SEL              ; check if ss:esp have been updated
    jz      Skip                       ; if not, update ss:esp

; change ss:esp
    sgdt    [rsp - 8]
    mov     ecx, dword [rsp - 6]
    add     ecx, eax

    xor     eax, eax
    mov     ax, word [rcx + 2]
    xor     edx, edx
    mov     dl, byte [rcx + 4]
    mov     dh, byte [rcx + 7]
    shl     edx, 0x10
    or      eax, edx
    add     esp, eax
    mov     eax, DATA_SEL
o16 mov     ss, ax

Skip:
    mov     rax, cr4
    push    rax                         ; save cr4
    mov     ecx, 0xC0000080
    rdmsr
    push    rax                         ; save MSR(0xc0000080)
    mov     rax, cr0
    push    rax                         ; save cr0

    mov     eax, CODE_SEL
    push    rax                         ; push cs hardcore
    call    Base                       ; push reture address for retf later
Base:
    add     dword [rsp], LongMode - Base ; offset for far retf, seg is the 1st arg
    mov     rax, cr4
    or      al, (1 << 5)
    mov     cr4, rax                    ; enable PAE

;
; Need to test for XD support
;
    mov     eax, 0x80000000
    cpuid
    cmp     eax, 0x80000000
    jbe     .0
    mov     eax, 0x80000001
    cpuid
    test    edx, BIT20
    jz      .0
    mov     ecx, 0xc0000080
    rdmsr
    or      ah, 8                      ; enable NXE
    wrmsr
.0:
    mov     ecx, 0xc0000080
    rdmsr
    or      ah, 1                      ; enable LME
    wrmsr

    mov     rbx, cr0
    or      ebx, 0x80010023            ; enable paging + WP + NE + MP + PE
    mov     cr0, rbx                    ; enable paging
    retf
LongMode:                               ; long mode starts here
    lea     rax, [ASM_PFX(gSmmFeatureSmiHandlerIdtr)] ; load long mode IDT table to handle the exception
    lidt    [rax]             ; that maybe issued in 64bit MC exception handler
    mov     rcx, rsp
    and     rsp, qword 0xfffffff0
    xor     rax, rax
    mov     ax,  ss
    push    rax
    push    rcx
    add     rcx, 32
    mov     eax, dword [rcx + 8]
    push    rax                         ; old eflags
    xor     rax, rax
    mov     ax,  cs                     ; cs
    push    rax
    lea     rax, [BackPmIdtHandler]     ; return EIP
    push    rax
    lea     rax, [ASM_PFX(mMceHandlerLongMode)]
    push    qword [rax]                       ; long mode MCA handle
    mov     eax, dword [rcx - 8]
    mov     edx, dword [rcx - 12]
    mov     ecx, dword [rcx - 16]

    add     rsp, 8
    jmp     qword [rsp - 8]         ; jmp to long mode MCA handle

BackPmIdtHandler:
    ;
    ; let rax save DS
    ;
    mov     rax, dword DATA_SEL

    ;
    ; Change to Compatible Segment
    ;
    mov     rcx, dword CODE32_SEL             ; load compatible mode selector
    shl     rcx, 32
    lea     rdx, [Compatible]      ; assume address < 4G
    or      rcx, rdx
    push    rcx
    retf

Compatible:
    pop     rax
    mov     cr0, rax                    ; restore cr0 to disable paging.

    mov     ecx, 0xC0000080
    rdmsr
    pop     rax
    wrmsr                               ; restore MSR (0xc0000080) to clear EFER.LME

    pop     rax
    mov     cr4, rax                    ; restore cr4 to clear PAE

    lss     esp, [rsp]        ; restore ss:esp

    add     esp, 4                      ; skip old ss
    pop     rcx
    pop     rdx
    pop     rax
    lidt    [rsp]             ; load saved 32bit IDTR
    add     esp, 12 + 8
    ;
    ; retf maybe used for MCE
    ;
    push    qword [rsp - 8]
    push    qword [rsp - 8]
    retf

