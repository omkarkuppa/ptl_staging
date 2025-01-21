;; @file
;  This file implements SEC host bridge library
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
;@par Specification Reference:
;;


%include "SecHostBridgeLib.inc"

SECTION .text

extern   ASM_PFX(PcdGet64 (PcdMchBaseAddress))

%macro RET_RBP 0

  jmp     rbp                           ; restore RIP from RBP

%endmacro

FSPT_CFG_PCI_EXPRESS_BASE_OFF          EQU 048h ; FsptConfig.PcdPciExpressBaseAddress offset
FSPT_CFG_PCI_EXPRESS_LENGTH_OFF        EQU 050h ; FsptConfig.PcdPciExpressRegionLength offset

FSP22_FSPT_CFG_PCI_EXPRESS_BASE_OFF    EQU 068h ; FsptConfig.PcdPciExpressBaseAddress offset
FSP22_FSPT_CFG_PCI_EXPRESS_LENGTH_OFF  EQU 070h ; FsptConfig.PcdPciExpressRegionLength offset

;
;args 1:RoutineLabel
;
%macro CALL_RDI  1

  mov     rdi,  %%ReturnAddress
  jmp     %1
%%ReturnAddress:

%endmacro

global ASM_PFX(SecHostBridgeInit)
ASM_PFX(SecHostBridgeInit):
;
; Program PCI Express base address
;
  mov   eax,  PCIEX_BAR_REG
  mov   dx,   0CF8h
  out   dx,   eax
  ;
  ; RCX is FSP-T config region
  ; firstly use FSP22_FSPT_CFG_PCI_EXPRESS_LENGTH_OFF
  ;
  mov   eax,  DWORD [ecx + FSP22_FSPT_CFG_PCI_EXPRESS_LENGTH_OFF]      ; Read FsptConfig.PcdPciExpressRegionLength

  ; Check UPD structure revision
  cmp   BYTE [ecx + 8], 2
  jae   Fsp22UpdHeader

  ; for FSP21 UPD structure, use FSPT_CFG_PCI_EXPRESS_LENGTH_OFF
  mov   eax,  DWORD [ecx + FSPT_CFG_PCI_EXPRESS_LENGTH_OFF]      ; Read FsptConfig.PcdPciExpressRegionLength
Fsp22UpdHeader:
  mov   dx,   04h       ; Default is PCIEXBAR length is 64MB ([2:1] = 10b)
  cmp   eax,  10000000h ; Is it 256MB length?
  jne   Check128MB
  mov   dx,   00h       ; Set PCIEXBAR length as 256MB ([2:1] = 00b)
  jmp   ProgramLength

Check128MB:
  cmp   eax,  8000000h  ; Is it 128MB length?
  jne   Check512MB
  mov   dx,   02h       ; Set PCIEXBAR length as 128MB ([2:1] = 01b)
  jmp   ProgramLength
Check512MB:
  cmp   eax,  20000000h  ; Is it 512MB length?
  jne   ProgramLength
  mov   dx,   06h        ; Set PCIEXBAR length as 512MB ([2:1] = 11b)

ProgramLength:
  ;
  ; ECX is FSP-T Config region
  ; firstly use FSP22_FSPT_CFG_PCI_EXPRESS_BASE_OFF
  ;
  mov   esi,  DWORD [ecx + FSP22_FSPT_CFG_PCI_EXPRESS_BASE_OFF]      ; Read FsptConfig.PcdPciExpressBaseAddress

  ; Check UPD structure revision
  cmp   BYTE [ecx + 8], 2
  jae   Fsp22UpdHeader1

  ; for FSP21 UPD structure, use FSPT_CFG_PCI_EXPRESS_BASE_OFF
  mov   esi,  DWORD [ecx + FSPT_CFG_PCI_EXPRESS_BASE_OFF]      ; Read FsptConfig.PcdPciExpressBaseAddress
Fsp22UpdHeader1:
  mov   eax, esi
  or    ax,  dx
  or    eax,  (0 | 1)  ;  PCIEXBAREN = 1
  mov   dx,   0CFCh
  out   dx,   eax

;
; Enable MCH Bar
;
  ; esi holds PciExpressBaseAddress.
  add     esi, MCHBAR_REG
  mov     rax, ASM_PFX(PcdGet64 (PcdMchBaseAddress))
  mov     eax, DWORD  [rax]
  or      eax, 1
  mov     Dword [esi], eax
  RET_RBP
