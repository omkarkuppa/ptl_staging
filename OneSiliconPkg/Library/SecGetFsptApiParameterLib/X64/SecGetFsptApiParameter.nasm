;; @file
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
; args 1:YMM, 2:XMM, 3:IDX (0 - lower 128bits, 1 - upper 128bits)
;
%macro LYMMN        3
            vextractf128  %2, %1, %3
            %endmacro

SECTION .text

;----------------------------------------------------------------------------
; This function gets Fspt API parameter (FSP-T UPD pointer)
; Get Fspt API parameter from ymm10 and pass it over to the caller
;
; YMM10 is used to store param0 of TempRamInit API or built-in UPD region
; XMM5 is a temp register used to save/restore data to/from YMMN in TempRamInit API
;----------------------------------------------------------------------------
global ASM_PFX(SecGetFsptApiParameter)
ASM_PFX(SecGetFsptApiParameter):
    LYMMN   ymm10, xmm5, 1
    movq    rax,  xmm5
    ret
