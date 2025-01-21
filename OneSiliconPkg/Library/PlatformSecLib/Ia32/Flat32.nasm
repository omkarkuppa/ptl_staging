;; @file
;  This is the code that goes from real-mode to protected mode.
;  it consumes the reset vector.
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 1999 Intel Corporation.
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
;
;;

%include "PlatformNasm.inc"
%include "ChipsetNasm.inc"
%include "SecCoreNasm.inc"

extern   ASM_PFX(SecHostBridgeInit)

extern   ASM_PFX(PcdGet64 (PcdMchBaseAddress))

extern   ASM_PFX(SecCarInit)
extern   ASM_PFX(PcdGet32 (PcdFlashFvBfvBase))
extern   ASM_PFX(PcdGet32 (PcdPeiCoreEntryBase))

SECTION .text

;_TEXT_REALMODE      SEGMENT PARA PUBLIC USE16 'CODE'
;                   ASSUME  CS:_TEXT_REALMODE, DS:_TEXT_REALMODE
global ASM_PFX(_TEXT_REALMODE)
ASM_PFX(_TEXT_REALMODE):
;------------------------------------------------------------------------------
;
;  SEC "Security" Code module.
;
;  Transition to non-paged flat-model protected mode from a
;  hard-coded GDT that provides exactly two descriptors.
;  This is a bare bones transition to protected mode only
;  used for while in PEI and possibly DXE.
;
;  IA32 specific cache as RAM modules
;
;  After enabling protected mode, a far jump is executed to
;  TransferToPEI using the newly loaded GDT.
;  This code also enables the Cache-as-RAM
;
;  RETURNS:    none
;
;  MMX Usage:
;              MM0 = BIST State
;              MM3 = Patch Revision
;              MM4 = Patch Pointer
;              MM5 = Save time-stamp counter value high32bit
;              MM6 = Save time-stamp counter value low32bit.
;              MM7 = Used in CALL_MMX & RET_ESI macro
;
;------------------------------------------------------------------------------

;_TEXT_PROTECTED_MODE      SEGMENT PARA PUBLIC USE32 'CODE'
;                          ASSUME  CS:_TEXT_PROTECTED_MODE, DS:_TEXT_PROTECTED_MODE
global ASM_PFX(_TEXT_PROTECTED_MODE)
ASM_PFX(_TEXT_PROTECTED_MODE):

;
;args 1:ReturnAddress 2: MmxRegister
;
%macro LOAD_MMX_EXT  2
  mov     esi, %1
  movd    %2, esi              ; save ReturnAddress into MM7
%endmacro

;
;args 1:RoutineLabel 2:MmxRegister
;
%macro CALL_MMX_EXT    2
  mov     esi,  %%ReturnAddress
  movd    %2, esi              ; save ReturnAddress into MM7
  jmp     %1
%%ReturnAddress:
%endmacro

;
;args 1:MmxRegister
;
%macro RET_ESI_EXT 1
  movd    esi, %1              ; restore EIP from MM7
  jmp     esi
%endmacro

;
;args 1:RoutineLabel
;
%macro CALL_MMX 1
  mov     esi,  %%ReturnAddress
  movd    mm7, esi             ; save ReturnAddress into MM7
  jmp     %1
%%ReturnAddress:
%endmacro

;
;args none
;
%macro RET_ESI 0
  movd    esi, mm7             ; restore EIP from MM7
  jmp     esi
%endmacro

;
;args 1:RoutineLabel
;
%macro CALL_EBP  1
  mov     ebp,  %%ReturnAddress
  jmp     %1
%%ReturnAddress:
%endmacro

;
;args none
;
%macro RET_EBP  0
  jmp     ebp                  ; restore EIP from EBP
%endmacro

align 4
global ASM_PFX(ProtectedModeSecStart)
ASM_PFX(ProtectedModeSecStart):
BITS    32

;
; Perform early platform initialization
;

global ASM_PFX(SecPlatformInit)
ASM_PFX(SecPlatformInit):

  CALL_EBP  ASM_PFX(SecHostBridgeInit)

  ;
  ; Enable HPET decode in PCH.
  ;

  ;
  ; Read PCI Express base address
  ;
  mov     eax,  PCIEX_BAR_REG
  mov     dx,   0CF8h
  out     dx,   eax
  mov     dx,   0CFCh
  in      eax,  dx
  and     eax,  0xFC000000
  mov     esi,  eax
  add     esi,  PCH_HPET_REG
  mov     al,   PCH_HPET_AE
  mov     Byte  [esi], al
  mov     al,   Byte [esi]                     ; read back
  xor     eax,  eax
  mov     esi,  HPET_COMP_1
  mov     Dword [esi], eax
  mov     esi,  HPET_COMP_2
  mov     Dword [esi], eax
  RET_ESI

;
; Establish Stack
;
global ASM_PFX(EstablishStack)
ASM_PFX(EstablishStack):
  ;
  ; Enable STACK
  ;
  RET_ESI

align 16

;
; GDT[0]: 0x00: Null entry, never used.
;
NULL_SEL        EQU $ - GDT_BASE        ; Selector [0]
GDT_BASE:
BootGdtTable        DD  0
                    DD  0
;
; Linear data segment descriptor
;
LINEAR_SEL      EQU $ - GDT_BASE        ; Selector [0x8]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  092h                            ; present, ring 0, data, expand-up, writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; Linear code segment descriptor
;
LINEAR_CODE_SEL EQU $ - GDT_BASE        ; Selector [0x10]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  09Bh                            ; present, ring 0, data, expand-up, not-writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; System data segment descriptor
;
SYS_DATA_SEL    EQU $ - GDT_BASE        ; Selector [0x18]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  093h                            ; present, ring 0, data, expand-up, not-writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0

;
; System code segment descriptor
;
SYS_CODE_SEL    EQU $ - GDT_BASE        ; Selector [0x20]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0
    DB  09Ah                            ; present, ring 0, data, expand-up, writable
    DB  0CFh                            ; page-granular, 32-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_CODE_SEL  EQU $ - GDT_BASE        ; Selector [0x28]
    DW  0FFFFh                          ; limit 0xFFFFF
    DW  0                               ; base 0
    DB  0Eh                             ; Changed from F000 to E000.
    DB  09Bh                            ; present, ring 0, code, expand-up, writable
    DB  00h                             ; byte-granular, 16-bit
    DB  0
;
; Spare segment descriptor
;
SYS16_DATA_SEL  EQU $ - GDT_BASE        ; Selector [0x30]
    DW  0FFFFh                          ; limit 0xFFFF
    DW  0                               ; base 0
    DB  0
    DB  093h                            ; present, ring 0, data, expand-up, not-writable
    DB  00h                             ; byte-granular, 16-bit
    DB  0

;
; Spare segment descriptor
;
SPARE5_SEL      EQU $ - GDT_BASE        ; Selector [0x38]
    DW  0                               ; limit 0
    DW  0                               ; base 0
    DB  0
    DB  0                               ; present, ring 0, data, expand-up, writable
    DB  0                               ; page-granular, 32-bit
    DB  0
GDT_SIZE        EQU $ - BootGdtTable    ; Size, in bytes

GdtDesc:                                ; GDT descriptor
    DW  GDT_SIZE - 1                    ; GDT limit
    DD  BootGdtTable                    ; GDT base address

ASM_PFX(NemInitLinearAddress):
NemInitLinearOffset:
    DD  ASM_PFX(ProtectedModeSecStart)  ; Offset of our 32 bit code
    DW  LINEAR_CODE_SEL
