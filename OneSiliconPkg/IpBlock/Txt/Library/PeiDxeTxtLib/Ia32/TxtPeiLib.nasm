;; @file
;  This file contains the code to determine Processor/Chipset TXT capaiblity
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2021 Intel Corporation.
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

 SECTION .text

IA32_CPUID_SMX_B              equ   6
CAPABILITIES                  equ   0
CR4_SMXE                      equ   (1 << 14)


;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
;  Procedure:   CheckSmxCapabilities
;
;  Input:       None
;
;  Output:      None
;
;  Registers:   None are preserved
;
;  Description: Execute GETSEC[CAPABILITIES] to report the SMX capabilities
;
;-----------------------------------------------------------------------------
;-----------------------------------------------------------------------------
global ASM_PFX(CheckSmxCapabilities)
ASM_PFX(CheckSmxCapabilities):

    ;
    ; Check whether SMX is supported
    ;
    push    ebx
    mov     eax, 1
    cpuid
    bt      ecx, IA32_CPUID_SMX_B
    jb      .L_CarryFlagSet
    mov     eax, 0
    jmp     .exitfunction
.L_CarryFlagSet:
    ;
    ; Save cr4
    ;
    mov     edx, CR4

    ;
    ; Enable SMXE
    ;
    mov     eax, CR4
    or      eax, CR4_SMXE
    mov     CR4, eax

    ;
    ; Call GETSEC[CAPABILITIES]
    ;
    mov     eax, CAPABILITIES        ; eax = CAPABILITIES
    mov     ebx, 0
    getsec

    ;
    ; Restore cr4
    ;
    mov     CR4, edx

.exitfunction:
    pop     ebx
    ret
