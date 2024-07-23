;; @file
;  This is the code that goes from real-mode to protected mode.
;  It consumes the reset vector, calls TempRamInit API from FSP binary.
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

SECTION .text

;----------------------------------------------------------------------------
;
; Procedure:    _ModuleEntryPoint
;
; Input:        None
;
; Output:       None
;
; Destroys:     Assume all registers
;
; Description:
;
;   FspSecCore can only work under X64 PEI.
;
; Return:       None
;
;  MMX Usage:
;              MM0 = BIST State
;
;----------------------------------------------------------------------------

BITS 64
align 16
global ASM_PFX(_ModuleEntryPoint)
ASM_PFX(_ModuleEntryPoint):

   jmp     $

