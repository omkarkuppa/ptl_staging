/** @file
  This header file contains SPA log Output Protocol Interface.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#ifndef __SMM_SPA_LOG_OUTPUT_PROTOCOL_H__
#define __SMM_SPA_LOG_OUTPUT_PROTOCOL_H__

extern EFI_GUID gEfiSpaLogOutputProtocolGuid;

typedef struct _EFI_SPA_LOG_OUTPUT_PROTOCOL EFI_SPA_LOG_OUTPUT_PROTOCOL;

typedef
EFI_STATUS
(EFIAPI *SPA_LOG_OUTPUT) (
  IN  EFI_SPA_LOG_OUTPUT_PROTOCOL *This,
  IN  CHAR16                      *LogString
);

struct _EFI_SPA_LOG_OUTPUT_PROTOCOL {
   SPA_LOG_OUTPUT    SpaLogOutput;
};

#endif
