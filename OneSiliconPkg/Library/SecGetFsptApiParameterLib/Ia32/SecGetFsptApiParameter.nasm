;; @file
;
;  @copyright
;  INTEL CONFIDENTIAL
;  Copyright (C) 2019 Intel Corporation.
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

SECTION .text

;
; Define SSE instruction set
;
%ifdef USE_SSE41_FLAG
;
; Define SSE macros using SSE 4.1 instructions
; args 1:XMM, 2:IDX, 3:REG
%macro SXMMN           3
             pinsrd  %1, %3, (%2 & 3)
             %endmacro

;
;args 1:XMM, 2:REG, 3:IDX
;
%macro LXMMN           3
             pextrd  %2, %1, (%3 & 3)
             %endmacro
%else
;
; Define SSE macros using SSE 2 instructions
; args 1:XMM, 2:IDX, 3:REG
%macro SXMMN       3
             pinsrw  %1, %3, (%2 & 3) * 2
             ror     %3, 16
             pinsrw  %1, %3, (%2 & 3) * 2 + 1
             rol     %3, 16
             %endmacro

;
;args 1:XMM, 2:REG, 3:IDX
;
%macro LXMMN    3
             pshufd  %1, %1, ((0E4E4E4h >> (%3 * 2))  & 0FFh)
             movd    %2, %1
             pshufd  %1, %1, ((0E4E4E4h >> (%3 * 2 + (%3 & 1) * 4)) & 0FFh)
             %endmacro
%endif

;----------------------------------------------------------------------------
; This function gets Fspt API parameter (FSP-T UPD pointer) from ECX-slot3
; in xmm6.
;
; ECX-slot3 in xmm6 is used to store param0 of TempRamInit API or built-in
; UPD region
;----------------------------------------------------------------------------
global ASM_PFX(SecGetFsptApiParameter)
ASM_PFX(SecGetFsptApiParameter):
    LXMMN xmm6, eax, 3
    ret