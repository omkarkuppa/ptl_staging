; @file
; Find and call SecStartup
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

extern ASM_PFX(SecStartup)
extern ASM_PFX(PlatformInit)

global ASM_PFX(CallPeiCoreEntryPoint)
ASM_PFX(CallPeiCoreEntryPoint):
  ;
  ; Obtain the hob list pointer
  ;
  mov     eax, [esp+4]
  ;
  ; Obtain the stack information
  ;   ECX: start of range
  ;   EDX: end of range
  ;
  mov     ecx, [esp+8]
  mov     edx, [esp+0Ch]

  ;
  ; Platform init
  ;
  pushad
  push edx
  push ecx
  push eax
  call ASM_PFX(PlatformInit)
  pop  eax
  pop  eax
  pop  eax
  popad

  ;
  ; Set stack top pointer
  ;
  mov     esp, edx

  ;
  ; Push the hob list pointer
  ;
  push    eax

  ;
  ; Save the value
  ;   ECX: start of range
  ;   EDX: end of range
  ;
  mov     ebp, esp
  push    ecx
  push    edx

  ;
  ; Push processor count to stack first, then BIST status (AP then BSP)
  ;
  mov     eax, 1
  cpuid
  shr     ebx, 16
  and     ebx, 0000000FFh
  cmp     bl, 1
  jae     PushProcessorCount

  ;
  ; Some processors report 0 logical processors.  Effectively 0 = 1.
  ; So we fix up the processor count
  ;
  inc     ebx

PushProcessorCount:
  push    ebx

  ;
  ; We need to implement a long-term solution for BIST capture.  For now, we just copy BSP BIST
  ; for all processor threads
  ;
  xor     ecx, ecx
  mov     cl, bl
PushBist:
  movd    eax, mm0
  push    eax
  loop    PushBist

  ; Save Time-Stamp Counter
  movd eax, mm5
  push eax

  movd eax, mm6
  push eax

  ;
  ; Pass entry point of the PEI core
  ;
  mov     edi, 0FFFFFFE0h
  push    DWORD [edi]

  ;
  ; Pass BFV into the PEI Core
  ;
  mov     edi, 0FFFFFFFCh
  push    DWORD [edi]

  ;
  ; Pass stack size into the PEI Core
  ;
  mov     ecx, [ebp - 4]
  mov     edx, [ebp - 8]
  push    ecx       ; RamBase

  sub     edx, ecx
  push    edx       ; RamSize

  ;
  ; Pass Control into the PEI Core
  ;
  call ASM_PFX(SecStartup)

