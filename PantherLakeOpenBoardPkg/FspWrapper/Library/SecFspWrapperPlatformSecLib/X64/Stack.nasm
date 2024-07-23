;; @file
; Switch the stack from temporary memory to permenent memory.
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
    ; Save four register: rax, rbx, rcx, rdx
    ;
    push  rax
    push  rbx
    push  rcx
    push  rdx

    ;
    ; !!CAUTION!! this function address's is pushed into stack after
    ; migration of whole temporary memory, so need save it to permanent
    ; memory at first!
    ;

    mov   rbx, rcx                 ; Save the first parameter
    mov   rcx, rdx                 ; Save the second parameter

    ;
    ; Save this function's return address into permanent memory at first.
    ; Then, Fixup the esp point to permanent memory
    ;
    mov   rax, rsp
    sub   rax, rbx
    add   rax, rcx
    mov   rdx, qword [rsp]         ; copy pushed register's value to permanent memory
    mov   qword [rax], rdx
    mov   rdx, qword [rsp + 8]
    mov   qword [rax + 8], rdx
    mov   rdx, qword [rsp + 16]
    mov   qword [rax + 16], rdx
    mov   rdx, qword [rsp + 24]
    mov   qword [rax + 24], rdx
    mov   rdx, qword [rsp + 32]    ; Update this function's return address into permanent memory
    mov   qword [rax + 32], rdx
    mov   rsp, rax                 ; From now, rsp is pointed to permanent memory

    ;
    ; Fixup the rbp point to permanent memory
    ;
    mov   rax, rbp
    sub   rax, rbx
    add   rax, rcx
    mov   rbp, rax                 ; From now, rbp is pointed to permanent memory

    pop   rdx
    pop   rcx
    pop   rbx
    pop   rax
    ret