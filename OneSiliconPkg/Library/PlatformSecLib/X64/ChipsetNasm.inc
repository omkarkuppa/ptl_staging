;; @file
;  Chipset constants and macros
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
;
;;


PCIEX_BAR_REG                 EQU 80000060h     ; B0:D0:F0:R60

;
; HPET REG address
;
PCH_HPET_REG                  EQU 000F9060h       ; PCH Register B0:D1F:F1:R60
PCH_HPET_AE                   EQU 80h             ; [7] Address Enable

;
; HPET compare register
;
HPET_COMP_1                   EQU 0FED00108h
HPET_COMP_2                   EQU 0FED0010Ch

;
; PCI registers
;
MCHBAR_REG                    EQU 048h
