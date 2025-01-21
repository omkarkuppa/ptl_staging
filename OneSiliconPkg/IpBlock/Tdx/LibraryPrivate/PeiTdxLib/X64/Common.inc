;; @file
;  This file is common macros for use in AsmLaunchActm.nasm
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

;
; CRs
;
CR0_PE_MASK                        EQU (1 << 0)
CR0_MP_MASK                        EQU (1 << 1)
CR0_ET_MASK                        EQU (1 << 4)
CR0_NE_MASK                        EQU (1 << 5)
ACTM_CR0_MASK                      EQU 033h

CR4_DE_MASK                        EQU (1 << 3)
CR4_OSFXSR_MASK                    EQU (1 << 9)
CR4_OSXMMEXCPT_MASK                EQU (1 << 10)
CR4_SMXE_MASK                      EQU (1 << 14)
CR4_PCIDE_MASK                     EQU (1 << 17)
CR4_OSXSAVE_MASK                   EQU (1 << 18)
ACTM_CR4_MASK                      EQU 044608h

;
; MSRs
;
MSR_IA32_MISC_ENABLE    EQU 1A0h
MSR_TEE_ERR_CODE        EQU 0A0h
MSR_IA32_MTRR_CAP       EQU 0FEh
MSR_IA32_MTRR_PHYSBASE0 EQU 0200h
MSR_IA32_MTRR_PHYSMASK0 EQU 0201h
MSR_IA32_CR_PAT         EQU 277h
MSR_IA32_EFER           EQU 0C0000080h

;
; MTRR specific
;
MTRR_ENABLE_BIT EQU 0800h ; BIT11

MTRR_UC EQU 00h ; Uncacheable
MTRR_WC EQU 01h ; Write-Combining
MTRR_WT EQU 04h ; Writethrough
MTRR_WP EQU 05h ; Write-Protect
MTRR_WB EQU 06h ; Writeback

NEM_NO_FREE_MTRR_ERROR_CODE EQU 0D0h

;
; ACM specific
;
ACMOD_SIZE EQU 24
ENTERACCS  EQU 02h

;
; MM0..7 registers
;
%define MOVQIN0  movq mm0, rax
%define MOVDIN0  movd mm0, eax
%define MOVQOUT0 movq rax, mm0
%define MOVDOUT0 movd eax, mm0

%define MOVQIN1  movq mm1, rax
%define MOVDIN1  movd mm1, eax
%define MOVQOUT1 movq rax, mm1
%define MOVDOUT1 movd eax, mm1

%define MOVQIN2  movq mm2, rax
%define MOVDIN2  movd mm2, eax
%define MOVQOUT2 movq rax, mm2
%define MOVDOUT2 movd eax, mm2

%define MOVQIN3  movq mm3, rax
%define MOVDIN3  movd mm3, eax
%define MOVQOUT3 movq rax, mm3
%define MOVDOUT3 movd eax, mm3

%define MOVQIN4  movq mm4, rax
%define MOVDIN4  movd mm4, eax
%define MOVQOUT4 movq rax, mm4
%define MOVDOUT4 movd eax, mm4

%define MOVQIN5  movq mm5, rax
%define MOVDIN5  movd mm5, eax
%define MOVQOUT5 movq rax, mm5
%define MOVDOUT5 movd eax, mm5

%define MOVQIN6  movq mm6, rax
%define MOVDIN6  movd mm6, eax
%define MOVQOUT6 movq rax, mm6
%define MOVDOUT6 movd eax, mm6

%define MOVQIN7  movq mm7, rax
%define MOVDIN7  movd mm7, eax
%define MOVQOUT7 movq rax, mm7
%define MOVDOUT7 movd eax, mm7
