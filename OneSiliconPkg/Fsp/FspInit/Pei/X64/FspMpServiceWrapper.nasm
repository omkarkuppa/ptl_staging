;; @file
;  This file implements FspMpServiceWrapper
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
  mov     ecx, DWORD [rsp + 4]
  mov     DWORD [rax + 4], ecx
  mov     ecx, DWORD [rsp]
  mov     DWORD [rax], ecx
  add     rsp, 16
  POPA_64
  popfq
  popfq
%endmacro

struc MpServiceParameter
  .CpuMpPpi:          resq 1
  .CpuMpPpiFunction:  resq 1
  .Uintn1:            resq 1
  .Uintn2:            resq 1
  .ProInfo:           resq 1
  .ApPro:             resq 1
  .Void:              resq 1
  .Uint32:            resq 1
  .MpBoolean:         resb 1
  .size:
endstruc

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackGetNumberOfProcessors (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackGetNumberOfProcessors)
ASM_PFX(SwitchStackGetNumberOfProcessors):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     r8, QWORD [rax + MpServiceParameter.Uintn2]
  mov     rdx, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackGetProcessorInfo (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackGetProcessorInfo)
ASM_PFX(SwitchStackGetProcessorInfo):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     r8, QWORD [rax + MpServiceParameter.ProInfo]
  mov     rdx, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackStartupAllAPs (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackStartupAllAPs)
ASM_PFX(SwitchStackStartupAllAPs):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    rax  ; push rax to make rsp 16-byte aligned before calling CpuMpPpiFunction
  push    QWORD [rax + MpServiceParameter.Void]
  mov     r9, QWORD [rax + MpServiceParameter.Uintn1]
  mov     r8, QWORD [rax + MpServiceParameter.MpBoolean]
  mov     rdx, QWORD [rax + MpServiceParameter.ApPro]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 30h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackStartupThisAP (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackStartupThisAP)
ASM_PFX(SwitchStackStartupThisAP):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    rax  ; push rax to make rsp 16-byte aligned before calling CpuMpPpiFunction
  push    QWORD [rax + MpServiceParameter.Void]
  mov     r9, QWORD [rax + MpServiceParameter.Uintn2]
  mov     r8, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rdx, QWORD [rax + MpServiceParameter.ApPro]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 30h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackSwitchBSP (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackSwitchBSP)
ASM_PFX(SwitchStackSwitchBSP):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     r8, QWORD [rax + MpServiceParameter.MpBoolean]
  mov     rdx, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackEnableDisableAP (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackEnableDisableAP)
ASM_PFX(SwitchStackEnableDisableAP):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     r9, QWORD [rax + MpServiceParameter.Uint32]
  mov     r8, QWORD [rax + MpServiceParameter.MpBoolean]
  mov     rdx, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackWhoAmI (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackWhoAmI)
ASM_PFX(SwitchStackWhoAmI):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     rdx, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret

;------------------------------------------------------------------------------
; EFI_STATUS
; EFIAPI
; SwitchStackStartupAllCPUs (
;   VOID
;   )
;------------------------------------------------------------------------------
global ASM_PFX(SwitchStackStartupAllCPUs)
ASM_PFX(SwitchStackStartupAllCPUs):
  sub     rsp, 8  ; make rsp 16-byte aligned before using P2L_SWITCHSTACK macro
  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     rax, [rax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  mov     r9, QWORD [rax + MpServiceParameter.Void]
  mov     r8, QWORD [rax + MpServiceParameter.Uintn1]
  mov     rdx, QWORD [rax + MpServiceParameter.ApPro]
  mov     rcx, QWORD [rax + MpServiceParameter.CpuMpPpi]
  sub     rsp, 20h
  call    QWORD [rax + MpServiceParameter.CpuMpPpiFunction]
  add     rsp, 20h
  L2P_SWITCHSTACK
  add     rsp, 8
  ret