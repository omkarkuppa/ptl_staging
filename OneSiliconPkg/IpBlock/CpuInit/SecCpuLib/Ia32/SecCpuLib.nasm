;; @file
;  This file implements SEC CPU library
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2015 Intel Corporation.
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


%include "SecCpuLibNasm.inc"

;
; EarlyCycleDecoding is a helper function configure cycle decoding
;
extern   ASM_PFX(EarlyCycleDecoding)
;
; LpssUartDebugConfiguration is a helper function to init SerialIoUart
;
extern   ASM_PFX(LpssUartDebugConfiguration)
;
; SerialIoSpiEarlyinitalization is a helper function to init SerialIoSpi
;
extern   ASM_PFX(SerialIoSpiEarlyinitalization)
;
; SerialIoI2cEarlyinitalization is a helper function to init SerialIoI2c
;
extern   ASM_PFX(SerialIoI2cEarlyinitalization)
;
; SerialPortConfiguration is a helper function to init SerialPort
;
extern   ASM_PFX(SerialPortConfiguration)

extern   ASM_PFX(PcdGet32 (PcdNemCodeCacheBase))
extern   ASM_PFX(PcdGet32 (PcdTemporaryRamBase))
extern   ASM_PFX(PcdGet32 (PcdTopMemoryCacheSize))
extern   ASM_PFX(PcdGet32 (PcdFspReservedBufferSize))

SECTION .text

struc UpdateHeaderStruc
  .dHeaderVersion:        resd  1  ; Header version#
  .dUpdateRevision:       resd  1  ; Update revision#
  .dDate:                 resd  1  ; Date in binary (08/13/07 as 0x08132007)
  .dProcessorSignature:   resd  1  ; CPU type, family, model, stepping
  .dChecksum:             resd  1  ; Checksum
  .dLoaderRevision:       resd  1  ; Update loader version#
  .dProcessorFlags:       resd  1  ; Processor Flags
  .dDataSize:             resd  1  ; Size of encrypted data
  .dTotalSize:            resd  1  ; Total size of update in bytes
  .dReserved:             resd  3  ; 12 bytes reserved
  .size:
endstruc

struc SecCarInitParams
  ; FSP_UPD_HEADER {
  .FspUpdHeader:          resd  8
  ; }
  ; FSPT_CORE_UPD {
  .MicrocodeCodeAddr:     resd  1
  .MicrocodeCodeSize:     resd  1
  .CodeRegionBase:        resd  1
  .CodeRegionSize:        resd  1
  ; }
  .size:
endstruc

struc SecCarInitParamsFsp22
  ; FSP_UPD_HEADER {
  .FspUpdHeaderSignature: resd  2
  .FspUpdHeaderRevision:  resb  1
  .FspUpdHeaderReserved:  resb 23
  ; }
  ; FSPT_ARCH_UPD {
  .FsptArchRevision:      resb  1
  .FsptArchReserved:      resb  3
  .FsptArchUpd:           resd  7
  ; }
  ; FSPT_CORE_UPD {
  .MicrocodeCodeAddr:     resd  1
  .MicrocodeCodeSize:     resd  1
  .CodeRegionBase:        resd  1
  .CodeRegionSize:        resd  1
  ; }
  .size:
endstruc

struc SecCarInitParamsFsp24
  ; FSP_UPD_HEADER {
  .FspUpdHeaderSignature: resd  2
  .FspUpdHeaderRevision:  resb  1
  .FspUpdHeaderReserved:  resb 23
  ; }
  ; FSPT_ARCH2_UPD {
  .FsptArchRevision:      resb  1
  .FsptArchReserved:      resb  3
  .FsptArchLength:        resd  1
  .FspDebugHandler        resq  1
  .FsptArchUpd:           resd  4
  ; }
  ; FSPT_CORE_UPD {
  .MicrocodeCodeAddr:     resq  1
  .MicrocodeCodeSize:     resq  1
  .CodeRegionBase:        resq  1
  .CodeRegionSize:        resq  1
  ; }
  .size:
endstruc

FSPT_CFG_TOP_MEMORY_CACHE_SIZE_OFFSET       EQU 090h ; FsptConfig.TopMemoryCacheSize offset
FSP22_FSPT_CFG_TOP_MEMORY_CACHE_SIZE_OFFSET EQU 0B0h ; FsptConfig.TopMemoryCacheSize offset
BOOT_GUARD_CNB_SIZE EQU 1000h

%macro RET_ESI 0
  movd    esi, mm7                      ; restore EIP from MM7
  jmp     esi
%endmacro

%macro CALL_EBP 1
  mov     ebp, %%ReturnAddress1
  jmp     %1
  %%ReturnAddress1:
%endmacro

%macro RET_EBP 0
  jmp     ebp                           ; restore EIP from EBP
%endmacro

;
; XMM5 slot 2 forTemporaryRamSize
;
%macro LOAD_TEMPORARY_RAM_SIZE    1
  LXMMN      xmm5, %1, 2
             %endmacro

%macro SAVE_TEMPORARY_RAM_SIZE    1
  SXMMN      xmm5, 2, %1
             %endmacro

;-----------------------------------------------------------------------------
;
;  Section:     SecCarInit
;
;  Description: This function initializes the Cache for Data, Stack, and Code
;               as specified in the  BIOS Writer's Guide.
;
;-----------------------------------------------------------------------------
global ASM_PFX(SecCarInit)
ASM_PFX(SecCarInit):

  mov     esp, ecx                      ; Save UPD pointer to esp

  ;
  ; Detect Boot Guard Boot
  ;
DetectBootGuard:
  mov     ecx, MSR_BOOT_GUARD_SACM_INFO
  rdmsr
  and     eax, 01h
  jnz     BootGuardNemSetup

  ;   Ensure that all variable-range MTRR valid flags are clear and
  ;   IA32_MTRR_DEF_TYPE MSR E flag is clear.  Note: This is the default state
  ;   after hardware reset.
  ;
  ;   Initialize all fixed-range and variable-range MTRR register fields to 0.
  ;
   mov   ecx, IA32_MTRR_CAP             ; get variable MTRR support
   rdmsr
   movzx ebx, al                        ; EBX = number of variable MTRR pairs
   shl   ebx, 2                         ; *4 for Base/Mask pair and WORD size
   add   ebx, 2                         ; MTRR_DEF_TYPE

   xor   eax, eax                       ; Clear the low dword to write
   xor   edx, edx                       ; Clear the high dword to write

InitMtrrLoop:
  add   ebx, -2
  movzx ecx, WORD [cs:(MtrrInitTable + ebx)]  ; ecx <- address of mtrr to zero
  wrmsr
  jnz   InitMtrrLoop                    ; loop through the whole table

  ;
  ;   Configure the default memory type to un-cacheable (UC) in the
  ;   IA32_MTRR_DEF_TYPE MSR.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  and     eax, ~(00000CFFh)             ;Clear the enable bits and def type UC.
  wrmsr

  ;
  ; Enable the logical processor's (BSP) cache: execute INVD and set
  ; CR0.CD = 0, CR0.NW = 0.
  ;
  mov     eax, cr0
  and     eax, ~(CR0_CACHE_DISABLE + CR0_NO_WRITE)
  invd
  mov     cr0, eax

  ; Enable No-Eviction Mode Setup State by setting
  ; NO_EVICT_MODE  MSR 2E0h bit [0] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 1
  wrmsr

  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 80000008h                  ; Address sizes leaf
  cpuid
  sub   al, 32
  movzx eax, al
  xor   esi, esi
  bts   esi, eax
  dec   esi                             ; esi <- MTRR_PHYS_MASK_HIGH

  ;
  ;   Configure the DataStack region as write-back (WB) cacheable memory type
  ;   using the variable range MTRRs.
  ;
  ;
  ; Set the base address of the DataStack cache range
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  or      eax, MTRR_MEMORY_TYPE_WB
                                        ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_0         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  LOAD_TEMPORARY_RAM_SIZE eax
  add  eax, GUARD_PAGE_SIZE             ; Adjusting size with power of 2 to match MTRR size programming
  dec  eax
  not  eax
  or   eax, MTRR_PHYS_MASK_VALID
                                        ; turn on the Valid flag
  mov  edx, esi                         ; edx <- MTRR_PHYS_MASK_HIGH
  mov  ecx, MTRR_PHYS_MASK_0            ; For proper addressing above 4GB
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ;   Configure the BIOS code region as write-protected (WP) cacheable
  ;   memory type using a single variable range MTRR.
  ;
  ;   Ensure region to cache meets MTRR requirements for
  ;   size and alignment.
  ;

  ;
  ; NEM size formula
  ; 1. Get the cache size via CPUID leaf 4 of BSP. CacheSize (Bytes) = (WayCnt) * (PartitionCnt) * (LineSize) * (SetCnt)
  ; 2. Get the cache size of NEM supported cache level via CPUID leaf 4 of BSP. (Same as step 1.)
  ; 3. Compute cache size of 1 way (WayCacheSize = CacheSize / WayCnt)
  ; => WayCacheSize = (PartitionCnt) * (LineSize) * (SetCnt)
  ; 4. Compute Effective SetCnt (EffSetCnt = previous_power_of_two(SetCnt))
  ; 5. Reduce CacheSize using EffSetCnt value.
  ;    =>EffCacheSize = CacheSize * (EffSetCnt / SetCnt)
  ;                   = (WayCnt) * (PartitionCnt) * (LineSize) * (EffSetCnt)
  ; 6. Compute required guard band (GB). Assume each Guard Band == 1way size. GbSize = WayCacheSize
  ; 7. Compute NEM size based on number of contiguous ranges. NemSize = EffCacheSize - no. GB.
  ;
  ; note: below routine uses mm2 mm3 mm4 as temp registers
  ;       mm2 = SetCnt
  ;       mm3 = WayCacheSize
  ;       mm4 = WaysPartitionsLines => tells about no of ways,Partitions and lines together
  ;

  ;
  ; Calculate NEM size
  ; Determine Cache Parameter by CPUID Function 04h
  ; Dynamically find the farthest non-zero cache(Last Level Cache) starting from the smallest cache level.
  ;
  mov     edi, 0
Find_Farthest_Cache_Parameter:
  inc     edi
  mov     ecx, edi
  mov     eax, 4
  cpuid
  cmp     eax, 0
  jnz     Find_Farthest_Cache_Parameter

  dec     edi
  mov     ecx, edi
  mov     eax, 4
  cpuid

  ;
  ; Get Cache parameters
  ;
  ; This Cache Size in Bytes = (Ways + 1)       * (Partitions + 1) * (Line_Size + 1) * (Sets + 1)
  ;                          = (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
  ;
  mov     edi, ebx
  and     ebx, 0FFFh
  inc     ebx
  mov     eax, ebx                          ; eax = (EBX[11:0] + 1)
  mov     ebx, edi
  and     ebx, ~(0FFC00FFFh)
  shr     ebx, 12
  inc     ebx
  mul     ebx                               ; eax = (EBX[11:0] + 1) * (EBX[21:12] + 1)
  movd    mm3  eax                          ; mm3 = (EBX[11:0] + 1) * (EBX[21:12] + 1)
  mov     ebx, edi
  shr     ebx, 22
  inc     ebx
  mul     ebx                               ; eax = (EBX[11:0] + 1) * (EBX[21:12] + 1) * (EBX[31:22] + 1)
  movd    mm4, eax                          ; mm4 = WaysPartitionsLines
  mov     eax, ecx                          ;
  inc     eax                               ; eax = (ECX + 1)
  movd    mm2, eax                          ; mm2 = SetCnt
  movd    ebx, mm3
  mul     ebx                               ; eax = (ECX + 1) * (EBX[11:0] + 1) * (EBX[21:12] + 1)
  movd    mm3, eax                          ; mm3 = WayCacheSize = GbSize

  ;
  ; EffSetCnt = previous_power_of_2(SetCnt)
  ;
  movd    ebx, mm2                      ; mm2 = SetCnt
  xor     ecx, ecx
  bsr     ecx, ebx
  mov     edx, 1
  shl     edx, cl
  mov     ebx, edx                      ; ebx = EffSetCnt

  ;
  ;EffCacheSize = (WayCnt) * (PartitionCnt) * (LineSize) * (EffSetCnt)
  ;
  movd    eax, mm4                      ; mm4 = WaysPartitionsLines = (WayCnt) * (PartitionCnt) * (LineSize)
  mul     ebx                           ; eax = EffCacheSize = WaysPartitionsLines*EffSetCnt

  ;
  ; Since there is one code cache region and one data cache region, it requires one guard band.
  ;
  movd    ebx, mm3                      ; mm3 = WayCacheSize = GbSize
  sub     eax, ebx                      ; eax = EffCacheSize - GbSize

Get_NEM_size:
  ;
  ; Code cache size = Total NEM size - DataStack size
  ;
  LOAD_TEMPORARY_RAM_SIZE ecx
  cmp     ecx, eax
  ja      InvalidParameter

  sub     eax, ecx

  ;
  ; Round up to page size
  ;
  mov     ecx, eax                      ; Save
  and     ecx, 0FFFF0000h               ; Number of pages in 64K
  and     eax, 0FFFFh                   ; Number of "less-than-page" bytes
  jz      Rounded
  mov     eax, 10000h                   ; Add the whole page size

Rounded:
  add     eax, ecx                      ; eax - rounded up code cache size
  xor     ecx, ecx
  xor     ebx, ebx

  mov     ecx, esp                                            ; Restore UPD pointer to ecx

  ; firstly use FSP22 parameters
  mov     edx, ecx
  mov     edi, dword [edx + SecCarInitParamsFsp22.CodeRegionBase]   ; Code region base
  mov     ecx, dword [edx + SecCarInitParamsFsp22.CodeRegionSize]   ; Code region size

  ; check FSP UPD header revision
  cmp     byte [edx + SecCarInitParamsFsp22.FspUpdHeaderRevision], 2
  jb      Fsp21UpdHeader
  cmp     byte [edx + SecCarInitParamsFsp22.FsptArchRevision], 2
  je      Fsp24UpdHeader
  jmp     ChkCodeRegionSize

Fsp21UpdHeader:
  ; use FSP21 parameters
  mov     edi, dword [edx + SecCarInitParams.CodeRegionBase]   ; Code region base
  mov     ecx, dword [edx + SecCarInitParams.CodeRegionSize]   ; Code region size
  jmp     ChkCodeRegionSize

Fsp24UpdHeader:
  ; use FSP24 parameters
  mov     edi, dword [edx + SecCarInitParamsFsp24.CodeRegionBase]   ; Code region base
  mov     ecx, dword [edx + SecCarInitParamsFsp24.CodeRegionSize]   ; Code region size

ChkCodeRegionSize:
  cmp     ecx,  0
  jz      InvalidParameter
  cmp     ecx, 1000000h                 ; compare bios size. ecx = PcdBiosSize > 16M ? 16M : PcdBiosSize
  jb      FspBiosSizeBelow16M
  mov     ecx, 1000000h
FspBiosSizeBelow16M:
  ;
  ; Calculate the code cache base add code cache size.
  ; 1st: If CodeRegionBase is zero,
  ;      cache '4GB-CodeRegionSize to 4GB'
  ;
  cmp     edi, 0                        ; if (CodeRegionBase == 0),
  jz      SkipTopMemCache               ; Cache '4GB-CodeRegionSize to 4GB'
  cmp     edi, 0FF000000h               ; compare bios base address. edi = CodeRegionBase > 0xFF000000 ? CodeRegionBase : 0xFF000000
  ja      FspBiosBaseAddressAboveFF000000
  mov     edi, 0FF000000h
FspBiosBaseAddressAboveFF000000:
  ;
  ; 2nd: If CodeRegionBase + LLC available > 4GB,
  ;      cache Cache 'CodeRegionBase to 4GB'
  ;
  mov     ebx, edi
  add     ebx, eax                      ; if (CodeRegionBase + LLC available > 4GB),
  jc      SkipTopMemCache               ; Cache 'CodeRegionBase to 4GB'

  ;
  ; 3rd: If TopMemoryCacheSize is zero,
  ;        cache 'CodeRegionBase to CodeRegionBase + LLC available'
  ;      else,
  ;        cache '4GB-TopMemoryCacheSize to 4GB' AND
  ;        cache 'CodeRegionBase' to CodeRegionBase + LLC available - TopMemoryCacheSize'
  ;
  ; firstly use FSP22 parameters
  mov     ebx, dword [edx + FSP22_FSPT_CFG_TOP_MEMORY_CACHE_SIZE_OFFSET]   ; TopSwap Cache size

  ; check FSP UPD header revision
  cmp     byte [edx + SecCarInitParamsFsp22.FspUpdHeaderRevision], 2
  jae     Fsp22UpdHeader1

  ; use FSP21 parameters
  mov     ebx, dword [edx + FSPT_CFG_TOP_MEMORY_CACHE_SIZE_OFFSET]   ; TopSwap Cache size

Fsp22UpdHeader1:
  cmp     ebx, 0                                                   ; if (TopMemoryCacheSize == 0),
  jz      SkipTopMemCache                                          ; Cache 'CodeRegionBase to CodeRegionBase + LLC available'

  sub     eax, ebx                     ; Cache '4GB-TopMemoryCacheSize to 4GB'
                                       ; Cache 'CodeRegionBase' to CodeRegionBase + LLC available - TopMemoryCacheSize'
  movd    edx, mm3                     ; mm3 = WayCacheSize = GbSize.
  sub     eax, edx                     ; Leave GbSize as guard band for new cacheable region
  jmp     SecCarInitL3

InvalidParameter:
  mov     eax, 080000002h              ; RETURN_INVALID_PARAMETER
  jmp     SecCarInitExit

SkipTopMemCache:
  xor     ebx,  ebx                    ; ebx: TopMemoryCacheSize

SecCarInitL3:

  ;
  ; 4th: Use LLC size if CodeRegionSize is bigger than LLC size
  ;
  cmp     ecx, eax                     ; Compare FSP requested size with available size.
  ja      CodeSizeDone                 ; If Requested is larger than available, use available and jump.
  mov     eax, ecx                     ; Use requested size.
CodeSizeDone:

  ;
  ; Save MM5 into ESP before program MTRR, because program MTRR will use MM5 as the local variable.
  ; And, ESP is not initialized before CAR is enabled. So, it is safe to use ESP here.
  ;

  movd esp, mm5

  ;
  ; Define "local" vars for this routine
  ; Note that mm0 is used to store BIST result for BSP,
  ; mm1 is used to store the number of processor and BSP APIC ID,
  ; mm6 is used to save time-stamp counter value.
  ;
  %define CODE_SIZE_TO_CACHE      mm3
  %define CODE_BASE_TO_CACHE      mm4
  %define NEXT_MTRR_INDEX         mm5
  %define NEXT_MTRR_SIZE          mm2
  ;
  ; Initialize "locals"
  ;
  sub     ecx, ecx
  movd    NEXT_MTRR_INDEX, ecx          ; Count from 0 but start from MTRR_PHYS_BASE_1

  ;
  ; Save remaining size to cache
  ;
  movd    CODE_SIZE_TO_CACHE, eax       ; Size of code cache region that must be cached

  cmp     ebx, 0                        ; ebx: TopMemoryCacheSize
  jz      TopMemoryMtrrDone
  mov     eax, ebx                      ; eax: TopMemoryCacheSize

TopMemoryMtrr:
  ;
  ; To symply the algorithm, the TopMemoryCacheSize must be power of 2
  ; and only takes one MTRR record.
  ;

  ;
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; Set valid bit

  ;
  ; Program mask register
  ;
  mov     ecx, MTRR_PHYS_MASK_1         ; setup variable mtrr
  movd    ebx, NEXT_MTRR_INDEX
  add     ecx, ebx

  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr
  ;
  ; Program base register
  ;
  sub     edx, edx
  mov     ecx, MTRR_PHYS_BASE_1         ; setup variable mtrr
  add     ecx, ebx                      ; ebx is still NEXT_MTRR_INDEX

  and     eax, 0xFFFFF000               ; eax is the low 32bit MTRR mask. Retrieve the base address from MTRR mask
                                        ; MTRR mask = eax AND 0xFFFFF000 = 4G - mask_size
  or      eax, MTRR_MEMORY_TYPE_WP      ; set type to write protect
  wrmsr

  ;
  ; Increment MTRR index
  ;
  movd    ebx, NEXT_MTRR_INDEX
  add     ebx, 2
  movd    NEXT_MTRR_INDEX, ebx

  movd eax, CODE_SIZE_TO_CACHE           ; eax: code cache size

TopMemoryMtrrDone:

  ;
  ; For non-FSP, sets the base from 4GB - code cache length, which is the non-FSP post validation code.
  ; However, for FSP, the base is from the TempRamInit API. However, if base is 0, then use 4G - code cche lengthlength.
  ;

  cmp     edi, 0
  jnz     SecCarInitL4

  mov     edi, 0xFFFFFFFF
  sub     edi, eax
  inc     edi
  test    edi, 0xFFFF
  jz      SecCarInitL4
  add     edi, 0x10000
  and     edi, 0xFFFF0000

SecCarInitL4:

  movd    CODE_BASE_TO_CACHE, edi       ; Base code cache address

NextMtrr:
  ;
  ; Get remaining size to cache
  ;
  movd    eax, CODE_SIZE_TO_CACHE
  and     eax, eax
  jz      CodeRegionMtrrdone            ; If no left size - we are done
  ;
  ; Determine next size to cache.
  ; We start from bottom up. Use the following algorythm:
  ; 1. Get our own alignment. Max size we can cache equals to our alignment
  ; 2. Determine what is bigger - alignment or remaining size to cache.
  ;    If aligment is bigger - cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ;    If remaining size to cache is bigger
  ;      Determine the biggest 2^N part of it and cache it.
  ;      Adjust remaing size to cache and base address
  ;      Loop to 1.
  ; 3. End when there is no left size to cache or no left MTRRs
  ;
  movd    edi, CODE_BASE_TO_CACHE
  bsf     ecx, edi                      ; Get index of lowest bit set in base address
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1h
  shl     edx, cl                       ; Alignment is in edx
  cmp     edx, eax                      ; What is bigger, alignment or remaining size?
  jbe     GotSize                       ; JIf aligment is less
  ;
  ; Remaining size is bigger. Get the biggest part of it, 2^N in size
  ;
  bsr     ecx, eax                      ; Get index of highest set bit
  ;
  ; Convert index into size to be cached by next MTRR
  ;
  mov     edx, 1
  shl     edx, cl                       ; Size to cache

GotSize:
  mov     eax, edx
  movd    NEXT_MTRR_SIZE, eax           ; Save

  ;
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  dec     eax                           ; eax - size to cache less one byte
  not     eax                           ; eax contains low 32 bits of mask
  or      eax, MTRR_PHYS_MASK_VALID     ; Set valid bit

  ;
  ; Program mask register
  ;
  mov     ecx, MTRR_PHYS_MASK_1         ; setup variable mtrr
  movd    ebx, NEXT_MTRR_INDEX
  add     ecx, ebx

  mov     edx, esi                      ; edx <- MTRR_PHYS_MASK_HIGH
  wrmsr
  ;
  ; Program base register
  ;
  sub     edx, edx
  mov     ecx, MTRR_PHYS_BASE_1         ; setup variable mtrr
  add     ecx, ebx                      ; ebx is still NEXT_MTRR_INDEX

  movd    eax, CODE_BASE_TO_CACHE
  or      eax, MTRR_MEMORY_TYPE_WP      ; set type to write protect
  wrmsr
  ;
  ; Advance and loop
  ; Reduce remaining size to cache
  ;
  movd    ebx, CODE_SIZE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  sub     ebx, eax
  movd    CODE_SIZE_TO_CACHE, ebx

  ;
  ; Increment MTRR index
  ;
  movd    ebx, NEXT_MTRR_INDEX
  add     ebx, 2
  movd    NEXT_MTRR_INDEX, ebx
  ;
  ; Increment base address to cache
  ;
  movd    ebx, CODE_BASE_TO_CACHE
  movd    eax, NEXT_MTRR_SIZE
  add     ebx, eax
  ;
  ; if carry happens, means NEM base + size over 4G
  ;
  jc      CodeRegionMtrrdone
  movd    CODE_BASE_TO_CACHE, ebx

  jmp     NextMtrr

CodeRegionMtrrdone:
  ;
  ; Enable the MTRRs by setting the IA32_MTRR_DEF_TYPE MSR E flag.
  ;
  mov     ecx, MTRR_DEF_TYPE            ; Load the MTRR default type index
  rdmsr
  or      eax, MTRR_DEF_TYPE_E          ; Enable variable range MTRRs
  wrmsr

  ;
  ; Restore MM5 from ESP after program MTRR
  ;
  movd mm5, esp

  ;
  ; Enable No-Eviction Mode Run State by setting
  ; NO_EVICT_MODE MSR 2E0h bit [1] = '1'.
  ;
  mov     ecx, NO_EVICT_MODE
  rdmsr
  or      eax, 2
  wrmsr

  jmp     FinishedCacheConfig

  ;
  ; Jump to here when Boot Guard boot and NEM is initialized by Boot Guard ACM
  ;
BootGuardNemSetup:
  ;
  ; Finished with cache configuration
  ;

  ;
  ; For UFS boot, MTRRs and NEM is already set up by ACM so Disable MTRR9 and jump to FinishedCacheConfig
  ;
  xor  eax, eax
  mov  eax, DWORD [ds:(TXT_PUBLIC_BASE + BOOT_GUARD_BOOTSTATUS + 04h)]
  and  eax, BLOCK_BOOT_ENABLED
  cmp  eax, UFS_BOOT_ENABLED
  jne  ContinueCacheConfig

  ;
  ; Disable MTRR9
  ;
  mov     eax, 0
  or      eax, MTRR_MEMORY_TYPE_WB
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_9         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_9

  jmp FinishedCacheConfig

ContinueCacheConfig:

  ;
  ; Configure MTRR_PHYS_MASK_HIGH for proper addressing above 4GB
  ; based on the physical address size supported for this processor
  ; This is based on read from CPUID EAX = 080000008h, EAX bits [7:0]
  ;
  ; Examples:
  ;  MTRR_PHYS_MASK_HIGH = 00000000Fh  For 36 bit addressing
  ;  MTRR_PHYS_MASK_HIGH = 0000000FFh  For 40 bit addressing
  ;
  mov   eax, 80000008h                  ; Address sizes leaf
  cpuid
  sub   al, 32
  movzx eax, al
  xor   esi, esi
  bts   esi, eax
  dec   esi                             ; esi <- MTRR_PHYS_MASK_HIGH

  ;
  ; Configure the DataStack region as write-back (WB) cacheable memory type
  ; using the variable range MTRRs.
  ;
  ;
  ; Find available MTRR
  ;
  CALL_EBP     ASM_PFX(FindFreeMtrr)

  ;  +#############################+  <-- Original Stack end
  ;  |         Guard Page          |
  ;  +=============================+  <-- BIOS usage Data Stack end
  ;  |   . . . . . . . . . . . .   |
  ;  +                             +
  ;  |                             |
  ;  +       BIOS usage            +
  ;  |                             |
  ;  +     Data Stack Area         +
  ;  |                             |
  ;  +                             +
  ;  |   . . . . . . . . . . . .   |
  ;  +=============================+  <-- BIOS usage Data Stack Base
  ;  |         CNB buffer          |
  ;  +#############################+  <-- Original Stack Base = CNB Base

  ;
  ; Set the base address of the DataStack cache range
  ;
  mov     eax, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  or      eax, MTRR_MEMORY_TYPE_WB
                                        ; Load the write-back cache value
  xor     edx, edx                      ; clear upper dword
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Set the mask for the DataStack cache range
  ; Compute MTRR mask value:  Mask = NOT (Size - 1)
  ;
  LOAD_TEMPORARY_RAM_SIZE eax
  add  eax, GUARD_PAGE_SIZE             ; Adjusting size with power of 2 to match MTRR size programming
  dec  eax
  not  eax
  or   eax, MTRR_PHYS_MASK_VALID
                                        ; turn on the Valid flag
  mov  edx, esi                         ; edx <- MTRR_PHYS_MASK_HIGH
  inc  ecx
  wrmsr                                 ; the value in MTRR_PHYS_BASE_0

  ;
  ; Disable MTRR9 base and Mask
  ;
  mov     eax, 0
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_BASE_9         ; Load the MTRR index
  wrmsr                                 ; the value in MTRR_PHYS_BASE_9

  mov     eax, 0
  xor     edx, edx                      ; clear upper dword
  mov     ecx, MTRR_PHYS_MASK_9
  wrmsr                                 ; the value in MTRR_PHYS_MASK_9

  ;
  ; Finished with cache configuration
  ;
FinishedCacheConfig:

  ;
  ; Optionally Test the Region
  ;

  ;
  ; Test area by writing and reading
  ;
#if FixedPcdGetBool(PcdCacheCheckEnable) == 1
  cld
  mov     edi, [ASM_PFX(PcdGet32 (PcdTemporaryRamBase))]
  LOAD_TEMPORARY_RAM_SIZE ecx
  shr     ecx, 2
  ;
  ; If Boot Guard has written CNB data, don't overwrite it
  ;
  mov     esi, ecx
  xor     eax, eax
  mov     ecx, MSR_BOOT_GUARD_SACM_INFO
  rdmsr
  and     eax, 01h
  jz SkipBtgCnb
  mov     eax, BOOT_GUARD_CNB_SIZE
  add     edi, eax
  mov     ecx, esi
  sub     ecx, eax
  mov     eax, CACHE_TEST_VALUE
  jmp     TestDataStackArea
SkipBtgCnb:
  mov     ecx, esi
  mov     eax, CACHE_TEST_VALUE
TestDataStackArea:
  stosd
  cmp     eax, DWORD [edi-4]
  jnz     DataStackTestFail
  loop    TestDataStackArea,ecx
#endif
  jmp     DataStackTestPass

  ;
  ; Cache test failed
  ;
DataStackTestFail:

  STATUS_CODE (0D0h)

  jmp     $

  ;
  ; Configuration test failed
  ;
ConfigurationTestFailed:
  STATUS_CODE (0D1h)
  jmp     $

DataStackTestPass:

  ;
  ; At this point you may continue normal execution.  Typically this would include
  ; reserving stack, initializing the stack pointer, etc.
  ;

  ;
  ; Temporary set stack top pointer for C code usage.
  ;
  mov     eax, ASM_PFX(PcdGet32 (PcdTemporaryRamBase))
  mov     esp, [eax]
  LOAD_TEMPORARY_RAM_SIZE eax
  mov     ebx, ASM_PFX(PcdGet32 (PcdFspReservedBufferSize))
  mov     ebx, [ebx]
  sub     eax, ebx
  add     esp, eax
  ;
  ; program resource decoding
  ;
  pushad
  call    ASM_PFX(EarlyCycleDecoding)
  popad
  pushad
  call    ASM_PFX(LpssUartDebugConfiguration)
  popad
  pushad
  call    ASM_PFX(SerialIoSpiEarlyinitalization)
  popad
  pushad
  call    ASM_PFX(SerialIoI2cEarlyinitalization)
  popad
  pushad
  call    ASM_PFX(SerialPortConfiguration)
  popad

  ;
  ; After memory initialization is complete, please follow the algorithm in the BIOS
  ; Writer's Guide to properly transition to a normal system configuration.
  ; The algorithm covers the required sequence to properly exit this mode.
  ;
  xor    eax, eax

SecCarInitExit:

  RET_ESI


;
; Find available MTRRs
;
global ASM_PFX(FindFreeMtrr)
ASM_PFX(FindFreeMtrr):
  mov    ecx, MTRR_PHYS_MASK_0

L8:
  rdmsr
  test   eax, 800h
  jz     FoundFreeMtrr
  add    ecx, 2
  cmp    ecx, MTRR_PHYS_MASK_9
  jbe    L8
  ;
  ; No available MTRR, halt system
  ;
  jmp    $

FoundFreeMtrr:
  dec    ecx

  RET_EBP

;
; Add alignments for eSPI bug that causes registers to load false data at power on
;
align 2
MtrrInitTable:
    DW  MTRR_DEF_TYPE

    DW  MTRR_PHYS_BASE_0
    DW  MTRR_PHYS_MASK_0
    DW  MTRR_PHYS_BASE_1
    DW  MTRR_PHYS_MASK_1
    DW  MTRR_PHYS_BASE_2
    DW  MTRR_PHYS_MASK_2
    DW  MTRR_PHYS_BASE_3
    DW  MTRR_PHYS_MASK_3
    DW  MTRR_PHYS_BASE_4
    DW  MTRR_PHYS_MASK_4
    DW  MTRR_PHYS_BASE_5
    DW  MTRR_PHYS_MASK_5
    DW  MTRR_PHYS_BASE_6
    DW  MTRR_PHYS_MASK_6
    DW  MTRR_PHYS_BASE_7
    DW  MTRR_PHYS_MASK_7
    DW  MTRR_PHYS_BASE_8
    DW  MTRR_PHYS_MASK_8
    DW  MTRR_PHYS_BASE_9
    DW  MTRR_PHYS_MASK_9
MtrrCount      EQU (($ - MtrrInitTable) / 2)
