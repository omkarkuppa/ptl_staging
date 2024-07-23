; @file
; Switch the stack from temporary memory to permenent memory.
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2017 Intel Corporation.
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

    SECTION .text

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; SecSwitchStack (
;   UINT32   TemporaryMemoryBase,
;   UINT32   PermanentMemoryBase
;   );
;------------------------------------------------------------------------------
global ASM_PFX(SecSwitchStack)
ASM_PFX(SecSwitchStack):
    ;
    ; Save four register: eax, ebx, ecx, edx
    ;
    push  eax
    push  ebx
    push  ecx
    push  edx

    ;
    ; !!CAUTION!! this function address's is pushed into stack after
    ; migration of whole temporary memory, so need save it to permanent
    ; memory at first!
    ;

    mov   ebx, [esp + 20]          ; Save the first parameter
    mov   ecx, [esp + 24]          ; Save the second parameter

    ;
    ; Save this function's return address into permanent memory at first.
    ; Then, Fixup the esp point to permanent memory
    ;
    mov   eax, esp
    sub   eax, ebx
    add   eax, ecx
    mov   edx, dword [esp]         ; copy pushed register's value to permanent memory
    mov   dword [eax], edx
    mov   edx, dword [esp + 4]
    mov   dword [eax + 4], edx
    mov   edx, dword [esp + 8]
    mov   dword [eax + 8], edx
    mov   edx, dword [esp + 12]
    mov   dword [eax + 12], edx
    mov   edx, dword [esp + 16]    ; Update this function's return address into permanent memory
    mov   dword [eax + 16], edx
    mov   esp, eax                 ; From now, esp is pointed to permanent memory

    ;
    ; Fixup the ebp point to permanent memory
    ;
    mov   eax, ebp
    sub   eax, ebx
    add   eax, ecx
    mov   ebp, eax                 ; From now, ebp is pointed to permanent memory

    pop   edx
    pop   ecx
    pop   ebx
    pop   eax
    ret

