;; @file
;  SEC CPU Library constants and macros
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


;
; Set to 1 to enable debug
;
NO_EVICTION_MODE_DEBUG  EQU     1

;
; CNB = Communication NEM Buffer
; CNB is equivalent to ACM-BIOS data handoff buffer.
; ACM is the producer and BIOS is the consumer.
;
CNB_BASE_SIZE       EQU  1000h  ; 4KB
;
; The DataStack region must be >= 4096 bytes away from any executing code.
; This includes both the CodeRegion and any uncached code that may execute.
; This is to prevent the self-modifying code (SMC) detection mechanisms from forcing an eviction.
; In summary: To prevent data prefetching, there must be 4KB Guard Page between code and data NEM buffers.
;
GUARD_PAGE_SIZE     EQU  1000h  ; 4KB

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

;
;arg 1:Status
;
%macro STATUS_CODE   1
%if NO_EVICTION_MODE_DEBUG
                    mov  al, %1
                    out  080h, al
%endif
%endmacro

;
; MSR definitions
;
MSR_IA32_PLATFORM_ID          EQU 0017h
MSR_IA32_BIOS_UPDT_TRIG       EQU 0079h
MSR_IA32_BIOS_SIGN_ID         EQU 008Bh
MSR_IA32_DEBUG_INTERFACE      EQU 0C80h
MSR_APIC_BASE                 EQU 001Bh
MSR_BOOT_GUARD_SACM_INFO      EQU 013Ah
;
; MTRR definitions
;
IA32_MTRR_CAP                 EQU 0FEh
MTRR_PHYS_BASE_0              EQU 0200h
MTRR_PHYS_MASK_0              EQU 0201h
MTRR_PHYS_BASE_1              EQU 0202h
MTRR_PHYS_MASK_1              EQU 0203h
MTRR_PHYS_BASE_2              EQU 0204h
MTRR_PHYS_MASK_2              EQU 0205h
MTRR_PHYS_BASE_3              EQU 0206h
MTRR_PHYS_MASK_3              EQU 0207h
MTRR_PHYS_BASE_4              EQU 0208h
MTRR_PHYS_MASK_4              EQU 0209h
MTRR_PHYS_BASE_5              EQU 020Ah
MTRR_PHYS_MASK_5              EQU 020Bh
MTRR_PHYS_BASE_6              EQU 020Ch
MTRR_PHYS_MASK_6              EQU 020Dh
MTRR_PHYS_BASE_7              EQU 020Eh
MTRR_PHYS_MASK_7              EQU 020Fh
MTRR_PHYS_BASE_8              EQU 0210h
MTRR_PHYS_MASK_8              EQU 0211h
MTRR_PHYS_BASE_9              EQU 0212h
MTRR_PHYS_MASK_9              EQU 0213h
MTRR_FIX_64K_00000            EQU 0250h
MTRR_FIX_16K_80000            EQU 0258h
MTRR_FIX_16K_A0000            EQU 0259h
MTRR_FIX_4K_C0000             EQU 0268h
MTRR_FIX_4K_C8000             EQU 0269h
MTRR_FIX_4K_D0000             EQU 026Ah
MTRR_FIX_4K_D8000             EQU 026Bh
MTRR_FIX_4K_E0000             EQU 026Ch
MTRR_FIX_4K_E8000             EQU 026Dh
MTRR_FIX_4K_F0000             EQU 026Eh
MTRR_FIX_4K_F8000             EQU 026Fh

MTRR_DEF_TYPE                 EQU 02FFh
MTRR_DEF_TYPE_E               EQU 0800h
MTRR_DEF_TYPE_FE              EQU 0400h

;
; Define the high 32 bits of MTRR masking
; This should be read from CPUID EAX = 080000008h, EAX bits [7:0]
; But for most platforms this will be a fixed supported size so it is
; fixed to save space.
;
MTRR_PHYS_MASK_VALID          EQU 0800h
MTRR_MEMORY_TYPE_UC           EQU 00h
MTRR_MEMORY_TYPE_WC           EQU 01h
MTRR_MEMORY_TYPE_WT           EQU 04h
MTRR_MEMORY_TYPE_WP           EQU 05h
MTRR_MEMORY_TYPE_WB           EQU 06h

NO_EVICT_MODE                 EQU 2E0h

CR0_CACHE_DISABLE             EQU 040000000h
CR0_NO_WRITE                  EQU 020000000h

;
; Cache test values
; These are inverted to flip each bit at least once
;
CACHE_TEST_VALUE              EQU 05aa55aa55aa55aa5h

FVHEADER_LEN_OFF              EQU 30h
FFSHEADER_LEN                 EQU 18h

BLOCK_LENGTH_BYTES            EQU  2048

;
; Boot Device Indicator Config Register
;
TXT_PUBLIC_BASE               EQU 0FED30000h
BOOT_GUARD_BOOTSTATUS         EQU 0A0h
BLOCK_BOOT_ENABLED            EQU 1
UFS_BOOT_ENABLED              EQU 1
