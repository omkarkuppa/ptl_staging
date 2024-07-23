;; @file
;  This file is for launching ACTM
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

section .text
bits 64

%include "Common.inc"

;extern ASM_PFX(AsmSideDebugDeadloop)

;
; MSRs
;
%define IA32_MISC_ENABLE     0x1A0
%define IA32_EFER_MSR        0xC0000080
;
; MTRRs
;
%define IA32_MTRR_PHYSBASE0  0x200
%define IA32_MTRR_PHYSMASK0  0x201
%define IA32_MTRR_PHYSBASE1  0x202
%define IA32_MTRR_PHYSMASK1  0x203
%define IA32_MTRR_PHYSBASE2  0x204
%define IA32_MTRR_PHYSMASK2  0x205
%define IA32_MTRR_PHYSBASE3  0x206
%define IA32_MTRR_PHYSMASK3  0x207
%define IA32_MTRR_PHYSBASE4  0x208
%define IA32_MTRR_PHYSMASK4  0x209
%define IA32_MTRR_PHYSBASE5  0x20A
%define IA32_MTRR_PHYSMASK5  0x20B
%define IA32_MTRR_PHYSBASE6  0x20C
%define IA32_MTRR_PHYSMASK6  0x20D
%define IA32_MTRR_PHYSBASE7  0x20E
%define IA32_MTRR_PHYSMASK7  0x20F
%define IA32_MTRR_DEF_TYPE   0x2FF
%define IA32_MTRR_CAP        0xFE
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
;;
;; Machine check architecture MSR registers
;;
;%define MCG_CAP 0x179
;%define MC0_STATUS 0x401
;
;%define ACM_SIZE_TO_CACHE    xmm0
;%define ACM_BASE_TO_CACHE    xmm1
;%define NEXT_MTRR_INDEX      xmm2
;%define NEXT_MTRR_SIZE       xmm3
%define MTRR_PHYS_MASK_HIGH  xmm4

%define ACMOD_SIZE 24
%define LONG_MODE_CODE_SEGMENT  0x10
%define MOVQIN3                        movq mm3, rax
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

;--------------------------------------------------------------------------------------
;  Procedure:   AsmLaunchActm
;
;  Input:       rcx - ActmAddress
;               rdx - ActmSize
;
;  Output:      rax - ActmStatus
;
;  Description: Invokes GETSEC[ENTERACCS] for ACTM.
;               This function assumes:
;                 1. SMX is available and ENTERACCS, EXITAC are supported,
;                 2. Pointer to ACM is covered by WB MTRR (write-back = 0x06),
;                 3. Pointer to ACM resides in low 4GB physical address space,
;                 4. Segs are always zero-based (valid per PEI CIS).
;
;--------------------------------------------------------------------------------------
global ASM_PFX(AsmLaunchActm)
ASM_PFX(AsmLaunchActm):
  enter 0x30, 0
%define BIOS_GDT [rbp-0x10]
%define BIOS_IDT [rbp-0x20]

  ;-----------------------------------------
  ; Store GP registers
  ;-----------------------------------------
  pushfq
  PUSHA_64

  ;-----------------------------------------
  ; Store ACTM base address in MMX
  ;-----------------------------------------
  mov     rax, rcx 
  MOVQIN0            ; mm0 := ACTM base address
  xor     rax, rax

  mov     rax, rdx      
  MOVQIN1            ; mm1 := ACTM size in bytes
  xor     rax, rax

  ;-----------------------------------------
  ; Store GDTR, IDTR in MMX
  ;-----------------------------------------

  ; Store GDT
  sgdt    BIOS_GDT
  lea     rax, BIOS_GDT
  MOVQIN2               ; mm2 := BIOS GDT
  xor     rax, rax

  ; Store IDT
  sidt    BIOS_IDT
  lea     rax, BIOS_IDT
  MOVQIN3               ; mm3 := BIOS IDT
  xor     rax, rax

  ;--------------------------------
  ; Store MSRs on stack
  ;--------------------------------

  ; Store MSR_IA32_MISC_ENABLE
  mov     rcx, MSR_IA32_MISC_ENABLE
  rdmsr
  push    rax
  push    rdx

  ; Store MSR_IA32_CR_PAT
  mov     rcx, MSR_IA32_CR_PAT
  rdmsr
  push    rax
  push    rdx

  ; Store MSR_IA32_EFER
  mov     rcx, MSR_IA32_EFER
  rdmsr
  push    rax
  push    rdx

  ;-----------------------------------------
  ; Store CRs on stack
  ;-----------------------------------------

  ; Store CR0
  xor     rax, rax
  mov     rax, cr0
  push    rax

  ; Store CR4
  xor     rax, rax
  mov     rax, cr4
  push    rax

  ;-----------------------------------------
  ; Store ESP in MMX
  ;-----------------------------------------

  ; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
  ; ! no pushing to stack after this point !
  ; !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ; Store RBP
  mov     rax, rbp
  MOVQIN4           ; mm4:= RBP
  xor     rax, rax

  mov     rax, rsp
  MOVQIN5           ; mm5:= RSP
  xor     rax, rax

  ; Store CS
  mov     ax, cs
  movzx   rax, ax
  MOVQIN6           ; mm6 := cs
  xor     rax, rax

  xor rax, rax
  mov ax, ds
  MOVQIN7           ; mm7:= ds,es,fs,gs,ss
  xor rax, rax
  ;-----------------------------------------
  ; Enable features for ACTM
  ;-----------------------------------------

  ; Enable PE, MP, ET and NE in CR0
  mov rax, cr0
  or  rax, ACTM_CR0_MASK
  mov cr0, rax

  ; Enable DE, OSFXSR, OSXMMEXCPT, SMXE and OSXSAVE in CR4
  mov rax, cr4
  or  rax, ACTM_CR4_MASK
  mov cr4, rax
  xor rax, rax

  ;-----------------------------------------
  ; Store Return Address for ACTM in R10
  ;-----------------------------------------
  xor r10, r10
  mov r10, GetSecReturn
  mov r11, cr3

  MOVQOUT2
  mov rax, [rax+2]
  mov eax, eax ; zero out upper 32bits as previous read populates upper 32bits
  mov r9, rax
  xor rax, rax

  MOVQOUT3
  mov r12, [rax+2]
  xor rax, rax

  ;-----------------------------------------
  ; Prepare ACTM launch
  ;-----------------------------------------
  MOVQOUT0
  mov     rbx, rax ; rbx = ACTM base address
  xor     rax, rax

  MOVQOUT1
  mov     rcx, rax ; rcx = ACTM size in bytes
  xor     rax, rax

  mov     eax, ENTERACCS ; eax = ENTERACCS (0x02)

  ;-----------------------------------------
  ; Launch ACTM
  ;-----------------------------------------
  getsec                 ; GETSEC[ENTERACCS]

GetSecReturn:
  ;-----------------------------------------
  ; Restore EBP, SS, ESP
  ;-----------------------------------------

  ; Restore RBP
  xor rax, rax
  MOVQOUT4
  mov     rbp, rax

  ; Restore RSP
  xor rax, rax
  MOVQOUT5
  mov     rsp, rax

  ;
  ; GDTR and IDTR are restored from
  ; respectively r9 and r12 regs by
  ; SEAMLDR on the ACM exit.
  ;

  ;-----------------------------------------
  ; Restore CS
  ;-----------------------------------------
  xor rax, rax
  MOVQOUT6
  push rax
  mov rax, AsmLaunchActmAfterRestoreCs
  push rax
  retfq

AsmLaunchActmAfterRestoreCs:
  ;-----------------------------------------
  ; Restore segment registers
  ;-----------------------------------------
  xor rax, rax
  MOVQOUT7
  mov ds, ax
  mov es, ax
  mov fs, ax
  mov gs, ax
  mov ss, ax


  ;-----------------------------------------
  ; Restore CRs
  ;-----------------------------------------

  pop     rax
  mov     cr4, rax ; cr4 := eax
  xor rax, rax

  pop     rax
  mov     cr0, rax ; cr0 := eax
  xor rax, rax

  ;--------------------------------
  ; Restore MSRs
  ;--------------------------------

  ; Restore MSR_IA32_EFER
  mov rcx, MSR_IA32_EFER
  pop rdx
  pop rax
  wrmsr

  ; Restore MSR_IA32_CR_PAT
  mov rcx, MSR_IA32_CR_PAT
  pop rdx
  pop rax
  wrmsr

  ; Restore MSR_IA32_MISC_ENABLE
  mov rcx, MSR_IA32_MISC_ENABLE
  pop rdx
  pop rax
  wrmsr

  ;--------------------------------
  ; Write back and invalidate cache
  ;--------------------------------
  wbinvd

  ;--------------------------------
  ; Store ActmStatus in mm0
  ;--------------------------------
  mov rax, r9 ; rax := r9
  MOVQIN0     ; mm0 := rax

  ;-----------------------------------------
  ; Restore GP registers
  ;-----------------------------------------
  POPA_64
  popfq

 ;--------------------------------
  ; Return ActmStatus
  ; rax := ActmStatus
  ;--------------------------------
  MOVQOUT0 ; rax := mm0

  ;--------------------------------
  ; Zero-out MMX registers
  ;--------------------------------
  emms

  leave
  ret