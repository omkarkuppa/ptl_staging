;; @file
;  This file contains the code to launch BIOS ACM functions in PEI phase
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
;@par Specification
;;


DEFAULT REL
SECTION .text


;
; BIOS ACM functions
;
%define TXT_FN_SCLEAN        0x0
%define TXT_FN_RESET_EST_BIT 0x1
%define TXT_FN_RESET_AUX     0x2
%define TXT_FN_SCHECK        0x4
%define TXT_FN_CLEAR_SECRETS 0x5
%define TXT_FN_LOCK_CONFIG   0x6
%define TXT_FN_ACHECK        0x7
;
; Misc
;
%define ENTERACCS            0x2
%define PARAMETERS           0x6
%define ACMOD_SIZE           24
#define PORT80_CODE_PREFIX   0xA0
;
; MSRs
;
%define IA32_MISC_ENABLE     0x1A0
%define IA32_EFER_MSR        0xC0000080

;
; Only low order bits are assumed
;
%define MTRR_MASK            0xFFFFF000
%define MTRR_ENABLE          (1 << 11)
%define MTRR_FIXED_ENABLE    (1 << 10)
%define MTRR_VALID           (1 << 11)
%define UC                   0x0
%define WB                   0x6
;
; Control register bits
;
%define CR0_NE_MASK          (1 << 5)
%define CR0_NW_MASK          (1 << 29)
%define CR0_CD_MASK          (1 << 30)
%define CR4_DE               (1 << 3)
%define CR4_OSFXSR           (1 << 9)
%define CR4_SMXE             (1 << 14)
;
; Machine check architecture MSR registers
;
%define MCG_CAP 0x179
%define MC0_STATUS 0x401

%define BIOS_GDT [ebp-0x10]
%define BIOS_IDT [ebp-0x20]

extern ASM_PFX(LoadAcm)

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

;
; TXT register space definitions
;
#define TXT_PUBLIC_BASE      0xFED30000
#define TXT_SINIT_SIZE2      0x27C
#define BIOACM_ADDR          TXT_SINIT_SIZE2


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Launch BiosAcm SCLEAN
;
;  Setup GETSEC environment (protected mode, mtrrs, etc) and
;    invoke GETSEC:ENTERACCS with requested BIOS ACM entry point.
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(LaunchBiosAcmSclean)
ASM_PFX(LaunchBiosAcmSclean):
    ; START_FRAME
    ; MAKE_LOCAL BIOS_GDT[2]:QWORD
    ; MAKE_LOCAL BIOS_IDT[2]:QWORD
    ; END_FRAME
    enter 0x20,0

    ;
    ; Tell where we are
    ;
    mov     eax, 0x11110000
    in      ax, 0x80
    mov     ah, PORT80_CODE_PREFIX + TXT_FN_SCLEAN
    out     0x80, eax
    mov     edx, TXT_FN_SCLEAN

    jmp     LaunchBiosAcm_CommonFlow


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Launch BiosAcm ClearSecrets
;
;  Setup GETSEC environment (protected mode, mtrrs, etc) and
;    invoke GETSEC:ENTERACCS with requested BIOS ACM entry point.
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(LaunchBiosAcmClearSecrets)
ASM_PFX(LaunchBiosAcmClearSecrets):
    ; START_FRAME
    ; MAKE_LOCAL BIOS_GDT[2]:QWORD
    ; MAKE_LOCAL BIOS_IDT[2]:QWORD
    ; END_FRAME
    enter 0x20,0

    ;
    ; Tell where we are
    ;
    mov     eax, 0x11110000
    in      ax, 0x80
    mov     ah, PORT80_CODE_PREFIX + TXT_FN_CLEAR_SECRETS
    out     0x80, eax
    mov     edx, TXT_FN_CLEAR_SECRETS

    jmp     LaunchBiosAcm_CommonFlow


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Launch BiosAcm Alias Check
;
;  Setup GETSEC environment (protected mode, mtrrs, etc) and
;   invoke GETSEC:ENTERACCS with requested BIOS ACM entry point.
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(LaunchBiosAcmAcheck)
ASM_PFX(LaunchBiosAcmAcheck):
    ; START_FRAME
    ; MAKE_LOCAL BIOS_GDT[2]:QWORD
    ; MAKE_LOCAL BIOS_IDT[2]:QWORD
    ; END_FRAME
    enter 0x20,0

    ;
    ; Tell where we are
    ;
    mov     eax, 0x11110000
    in      ax, 0x80
    mov     ah, PORT80_CODE_PREFIX + TXT_FN_ACHECK
    out     0x80, eax
    mov     edx, TXT_FN_ACHECK

    jmp     LaunchBiosAcm_CommonFlow


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;
;  DoPowerCycleReset
;
;  @param[in]   None
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(DoPowerCycleReset)
ASM_PFX(DoPowerCycleReset):

ResetSystem:
    mov     dx, 0xCF8               ; Make warm system reset through port 0CF9h
    mov     eax, 0x8000FAAC         ; to be global system reset - set bit 20
    out     dx, eax                 ; of device 1F
    mov     dx, 0xCFC
    in      eax, dx
    or      eax, (1 << 20)
    out     dx, eax

    mov     dx, 0xCF9
    mov     al, 0
    out     dx, al
    mov     al, 0xE
    out     dx, al
    cli
    hlt
    jmp     $


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Save Ap Config
;
;  @param[in]  ApCfg   pointer to save area
;
;  Function is called in memory present environment on S3 resume
;    path. Saves contents of all MTRRs into table plus some registers.
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(SaveApConfig)
ASM_PFX(SaveApConfig):
    ret


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;
;  Procedure:   LaunchBiosAcm
;
;  @param[in]   AcmBase   Base address of LT BIOS ACM
;  @param[in]   Function  function number to execute
;
;  Output:
;
;  Stack:       Available at beginning of routine, but routine goes 'stackless'
;
;  Registers:
;
;  Description: Setup GETSEC environment (protected mode, mtrrs, etc) and
;               invoke GETSEC:ENTERACCS with requested BIOS ACM entry point.
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(LaunchBiosAcm)
ASM_PFX(LaunchBiosAcm):
;    START_FRAME
;    MAKE_LOCAL BIOS_GDT[2]:QWORD
;    MAKE_LOCAL BIOS_IDT[2]:QWORD
;    END_FRAME

    enter  0x30,0

    mov     ecx, DWORD [ebp+0x8]
    mov     ecx, [ecx]              ; ecx = AcmBase
    mov     edx, DWORD [ebp+0xc]    ; edx = Function

    ;
    ; Save the general purpose register state
    ;
    pushf
    cli
    PUSHA_64

    ; check if the AcmBase is a good address (none zero)
    cmp     ecx, 0                  ; bad ACM address?
    je      LaunchBiosAcm_SkipLaunch              ; Yes, then exit without any further work

LaunchBiosAcm_CommonFlow:
    ;
    ; Save the parameters passed to us
    ;
    mov     rax, cr3                ; save page table in MMX1
    movd    mm1, eax
    mov     eax, edx                ; save value of ESI for GETSEC[ENTERACCS] in MMX2
    movd    mm2, eax

    sgdt    BIOS_GDT                ; save gdtr
    sidt    BIOS_IDT                ; save idtr

    lea     eax, BIOS_GDT           ; eax = address of saved gdtr
    movd    mm3, eax                ; mm3 = address of saved gdtr

    mov     eax, ebp
    movd    mm4, eax                ; mm4 = ebp
    xor     eax, eax
    mov     ax, ss
    movd    mm5, eax                ; mm5 = ss
    mov     ax, cs
    movd    mm6, eax                ; mm6 = cs

    ;
    ; Save control registers
    ;
    mov     rax, cr4
    push    rax
    mov     rax, cr0
    push    rax

    ;
    ; Save IA32_MISC_ENABLES MSR
    ;
    mov     ecx, IA32_MISC_ENABLE
    rdmsr
    push    rax
    push    rdx

    ; Save the rest of the segment registers
    xor     eax, eax
    mov     ax, gs
    push    rax
    mov     ax, fs
    push    rax
    mov     ax, es
    push    rax
    mov     ax, ds
    push    rax

    ;
    ; For reloading CS
    ;
    mov     bx, cs
    push    rbx
    mov     rbx, LaunchBiosAcm_ReloadCS
    push    rbx


    ;
    ; Now that all of our state is on the stack, save esp
    ;
    mov     eax, esp
    movd    mm7, eax                ; mm7 = esp

    ;
    ; Enable SMXE, SSE and debug extensions
    ;
    mov     rax, cr4
    or      eax, CR4_OSFXSR + CR4_DE + CR4_SMXE
    mov     cr4, rax

    ;
    ; Clean all MCi_STATUS MSR registers
    ; SCLEAN will generate GPF otherwise
    ;
    ;mov    ecx, MCG_CAP
    ;rdmsr
    mov     al, 5                   ; skl only supports clear mc0-4 early PEI phase
    movzx   ebx, al                 ; ebx = MCR bank count
    xor     eax, eax                ; Write 0 into all MCi_STATUS registers
    xor     edx, edx
    mov     ecx, MC0_STATUS

LaunchBiosAcm_McaErrorCleanLoopStart:
    wrmsr
    dec     ebx
    jz      LaunchBiosAcm_CallGetsec
    add     ecx, 4                  ; Number of MSRs per bank
    jmp     LaunchBiosAcm_McaErrorCleanLoopStart

LaunchBiosAcm_CallGetsec:
    ;
    ; Call GETSEC[ENTERACCS]
    ;
    PUSHA_64
    sub     rsp, 20h
    call    ASM_PFX(LoadAcm)        ; Load ACM in WB memory
    add     rsp, 20h
    movd    mm0, eax                ; Save ACM base
    POPA_64
    movd    eax, mm2                ; eax = ACM function
    mov     esi, eax                ; esi = ACM function
    movd    eax, mm0                ; eax = AcmBase
    mov     ebx, eax                ; ebx = AcmBase
    mov     ecx, DWORD [ebx+ACMOD_SIZE]  ; ecx = size of ACM in dwords
    shl     ecx, 2                  ; ecx = size of ACM in bytes
    xor     edx, edx
    xor     edi, edi
    mov     eax, ENTERACCS          ; eax = ENTERACCS
    getsec

LaunchBiosAcm_ReloadCS:

    ;
    ; Restore the rest of the segment registers
    ;
    pop     rax
    mov     ds, ax
    pop     rax
    mov     es, ax
    pop     rax
    mov     fs, ax
    pop     rax
    mov     gs, ax

LaunchBiosAcm_SkipLaunch:
    POPA_64
    popf

    emms

    leave
    ret 0

;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Restore Mtrr Programming
;    Function is executed on BSP in memory present environment on S3
;    resume path. Restores contents of all MTRRs from table
;
;  @param[in]   ApMtrrTab    pointer to save area
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(RestoreMtrrProgramming)
ASM_PFX(RestoreMtrrProgramming):
    ret
