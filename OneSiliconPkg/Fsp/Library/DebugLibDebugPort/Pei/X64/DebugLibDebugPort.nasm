;; @file
;  This file implements DebugLibDebugPort
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
; @par Specification Reference:
;;

extern   ASM_PFX(SwapStack)
extern   ASM_PFX(GetFspGlobalDataPointer)

FSP_DATA_CORE_STACK_OFFSET        EQU   8     ; This offset need sync with FSP_GLOBAL_DATA
FSP_DATA_FUNC_PARAMETER_OFFSET    EQU   90h   ; This offset need sync with FSP_GLOBAL_DATA
SOFTWARE_PEI_MODULE               EQU   3030000h
DEBUG_CODE                        EQU   3h

struc EventHanlderParameter
  .FspEventHandler:       resq  1
  .StatusCodeType:        resd  1
  .StatusCodeValue:       resd  1
  .Instance:              resd  1
  .CallerId:              resq  1
  .Data:                  resq  1
  .size:
endstruc

struc DebugHanlderParameter
  .FspDebugHandler:       resq  1
  .Buffer:                resq  1
  .BufferSize:            resd  1
  .size:
endstruc

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

%macro P2L_SWITCHSTACK 0
  pushfq
  pushfq  ; one more pushfq to make rsp 16-byte aligned before calling SwapStack
  cli
  PUSHA_64
  sub     rsp, 16
  sidt    [rsp]
  mov     rcx, rsp
  call    ASM_PFX(SwapStack)
  mov     rsp, rax
  lidt    [rsp]
%endmacro

%macro L2P_SWITCHSTACK 0
  cli
  mov     rcx, rsp
  call    ASM_PFX(SwapStack)
  mov     rsp, rax
  lidt    [rsp]
  add     rsp, 16
  POPA_64
  popfq
  popfq
%endmacro

%macro SWITCHIDT 0
  pushfq
  pushfq  ; one more pushfq to make rsp 16-byte aligned before calling GetFspGlobalDataPointer
  cli
  PUSHA_64
  sub     rsp, 16
  sidt    [rsp]
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_CORE_STACK_OFFSET]
  lidt    [rax]
  mov     rcx, QWORD [rsp + 8]
  mov     QWORD [rax + 8], rcx
  mov     rcx, QWORD [rsp]
  mov     QWORD [rax], rcx
  add     rsp, 16
  POPA_64
  popfq
  popfq
%endmacro

%macro CALL_DEBUG_EVENT_HANDLER 0
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    rax    ; push rax to make rsp 16-byte aligned before calling FspEventHandler
  push    QWORD [rax + EventHanlderParameter.Data]
  mov     r9, QWORD [rax + EventHanlderParameter.CallerId]
  mov     edx, DWORD [rax + EventHanlderParameter.Instance]
  mov     r8, rdx
  mov     rdx, QWORD SOFTWARE_PEI_MODULE
  mov     rcx, QWORD DEBUG_CODE
  sub     rsp, 20h
  call    QWORD [rax + EventHanlderParameter.FspEventHandler]
  add     rsp, 30h
%endmacro

%macro CALL_DEBUG_HANDLER 0
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     rdx, QWORD [rax + DebugHanlderParameter.Buffer]
  mov     ecx, DWORD [rax + DebugHanlderParameter.BufferSize]
  sub     rsp, 20h
  call    QWORD [rax + DebugHanlderParameter.FspDebugHandler]
  add     rsp, 20h
%endmacro

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; SwitchStackCallDebugEventHandler (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackCallDebugEventHandler)
ASM_PFX(SwitchStackCallDebugEventHandler):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  CALL_DEBUG_EVENT_HANDLER
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; SwitchIdtCallDebugEventHandler (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchIdtCallDebugEventHandler)
ASM_PFX(SwitchIdtCallDebugEventHandler):
  sub     rsp, 8  ; make rsp 16-byte aligned before using SWITCHIDT macro
  SWITCHIDT
  CALL_DEBUG_EVENT_HANDLER
  SWITCHIDT
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; VOID
; EFIAPI
; SwitchStackCallDebugHandler (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackCallDebugHandler)
ASM_PFX(SwitchStackCallDebugHandler):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  CALL_DEBUG_HANDLER
  L2P_SWITCHSTACK
  add     rsp, 8
  ret


;------------------------------------------------------------------------------
; VOID
; EFIAPI
; SwitchIdtCallDebugHandler (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchIdtCallDebugHandler)
ASM_PFX(SwitchIdtCallDebugHandler):
  sub     rsp, 8  ; make rsp 16-byte aligned before using SWITCHIDT macro
  SWITCHIDT
  CALL_DEBUG_HANDLER
  SWITCHIDT
  add     rsp, 8
  ret
