;; @file
;  This file implements FspDebugServicePei
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
SOFTWARE_PEI_MODULE               EQU   3030000h
DEBUG_CODE                        EQU   3h

struc EventHanlderParameter
  .FspEventHandler:       resd  1
  .StatusCodeType:        resd  1
  .StatusCodeValue:       resd  1
  .Instance:              resd  1
  .CallerId:              resd  1
  .Data:                  resd  1
  .size:
endstruc

struc DebugHanlderParameter
  .FspDebugHandler:       resd  1
  .Buffer:                resd  1
  .BufferSize:            resd  1
  .size:
endstruc

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

%macro CALL_DEBUG_EVENT_HANDLER 0
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + EventHanlderParameter.Data]
  push    DWORD [eax + EventHanlderParameter.CallerId]
  push    DWORD [eax + EventHanlderParameter.Instance]
  push    DWORD SOFTWARE_PEI_MODULE
  push    DWORD DEBUG_CODE
  call    DWORD [eax + EventHanlderParameter.FspEventHandler]
  add     esp, 14h
%endmacro

%macro CALL_DEBUG_HANDLER 0
  call    ASM_PFX(GetFspGlobalDataPointer)
  mov     eax, [eax + FSP_DATA_FUNC_PARAMETER_OFFSET]
  push    DWORD [eax + DebugHanlderParameter.Buffer]
  push    DWORD [eax + DebugHanlderParameter.BufferSize]
  call    DWORD [eax + DebugHanlderParameter.FspDebugHandler]
  add     esp, 8
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

  P2L_SWITCHSTACK
  CALL_DEBUG_EVENT_HANDLER
  L2P_SWITCHSTACK
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

  SWITCHIDT
  CALL_DEBUG_EVENT_HANDLER
  SWITCHIDT
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

  P2L_SWITCHSTACK
  CALL_DEBUG_HANDLER
  L2P_SWITCHSTACK
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

  SWITCHIDT
  CALL_DEBUG_HANDLER
  SWITCHIDT
  ret
