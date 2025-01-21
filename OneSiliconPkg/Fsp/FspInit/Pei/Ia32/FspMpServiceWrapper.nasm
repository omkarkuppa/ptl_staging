;; @file
;  This file implements FspMpServiceWrapper
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2020 Intel Corporation.
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
FSP_DATA_FUNC_PARAMETER_OFFSET    EQU   64h   ; This offset need sync with FSP_GLOBAL_DATA

%macro P2L_SWITCHSTACK 0
  pushfd
  cli
  pushad
  sub     esp, 8
  sidt    [esp]
  push    esp
  call    ASM_PFX(SwapStack)
  mov     esp, eax
  lidt    [esp]
%endmacro

%macro L2P_SWITCHSTACK 0
  cli
  push    esp
  call    ASM_PFX(SwapStack)
  mov     esp, eax
  lidt    [esp]
  add     esp, 8
  popad
  popfd
%endmacro

%macro SWITCHIDT 0
  pushfd
  cli
  pushad
  sub     esp, 8
  sidt    [esp]
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_CORE_STACK_OFFSET]
  lidt    [eax]
  mov     ecx, DWORD [esp + 4]
  mov     DWORD [eax + 4], ecx
  mov     ecx, DWORD [esp]
  mov     DWORD [eax], ecx
  add     esp, 8
  popad
  popfd
%endmacro

struc MpServiceParameter
  .CpuMpPpi:          resd 1
  .CpuMpPpiFunction:  resd 1
  .Uintn1:            resd 1
  .Uintn2:            resd 1
  .ProInfo:           resd 1
  .ApPro:             resd 1
  .Void:              resd 1
  .Uint32:            resd 1
  .MpBoolean:         resd 1
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Uintn2]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 0ch
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.ProInfo]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 0ch
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Void]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.MpBoolean]
  push    DWORD [eax + MpServiceParameter.ApPro]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 14h
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Void]
  push    DWORD [eax + MpServiceParameter.Uintn2]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.ApPro]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 14h
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.MpBoolean]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 0ch
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Uint32]
  push    DWORD [eax + MpServiceParameter.MpBoolean]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 10h
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 08h
  L2P_SWITCHSTACK
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

  P2L_SWITCHSTACK
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + MpServiceParameter.Void]
  push    DWORD [eax + MpServiceParameter.Uintn1]
  push    DWORD [eax + MpServiceParameter.ApPro]
  push    DWORD [eax + MpServiceParameter.CpuMpPpi]
  call    DWORD [eax + MpServiceParameter.CpuMpPpiFunction]
  add     esp, 10h
  L2P_SWITCHSTACK
  ret