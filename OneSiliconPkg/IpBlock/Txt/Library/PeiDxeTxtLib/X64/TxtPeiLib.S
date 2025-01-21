/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

 @par Specification
**/

.text
ASM_FUNCTION_REMOVE_IF_UNREFERENCED

.macro GETSEC
.byte 0x0F, 0x37
.endm

.set IA32_CPUID_SMX_B,  6
.set CAPABILITIES,      0

.set CR4_SMXE,          (1 << 14)


ASM_GLOBAL ASM_PFX(CheckSmxCapabilities)

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//  Procedure:  CheckSmxCapabilities
//
//  Input:  None
//
//  Output: None
//
//  Registers:  None are preserved
//
//  Description:  Execute GETSEC[CAPABILITIES] to report the SMX capabilities
//
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

ASM_PFX(CheckSmxCapabilities):

    //
    // Check whether SMX is supported, bail if not
    //
    push    %rbx
    mov     $1, %rax
    cpuid
    bt      $IA32_CPUID_SMX_B, %rcx
    jb      L_CarryFlagSet
    mov     $0, %rax
    jmp     1f

L_CarryFlagSet:

    //
    // Save CR4
    //
    mov     %cr4, %rdx

    //
    // Enable SMXE
    //
    mov     %cr4, %rax
    or      $CR4_SMXE, %rax
    mov     %rax, %cr4

    //
    // Call GETSEC[CAPABILITIES]
    //
    mov     $CAPABILITIES, %rax
    mov     $0, %rbx
    GETSEC

    //
    // Restore CR4
    //
    mov     %rdx, %cr4

1:
    pop     %rbx
    ret