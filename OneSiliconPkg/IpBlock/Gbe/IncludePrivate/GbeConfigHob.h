/** @file
  GBE config HOB definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _GBE_CONFIG_HOB_H_
#define _GBE_CONFIG_HOB_H_

#include <Uefi/UefiBaseType.h>

/**
  PCH GBE config HOB settings.
**/
typedef struct {
  UINT32  PchWOLFastSupport     :  1; ///< Enables bit B_PCH_ACPI_GPE0_EN_127_96_PME_B0 during PchLanSxCallback in PchLanSxSmm
  UINT32  Rsvd1                 : 31; ///< Reserved bytes
} GBE_CONFIG_HOB;

#endif
