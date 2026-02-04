/**@file
  NULL implementation of PeiMemoryTelemetryLib

  Used when PcdMemoryTelemetryEnabled is FALSE to avoid unnecessary dependencies.
  All functions return EFI_UNSUPPORTED.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#include <PiPei.h>
#include <MemoryConfig.h>

/**
  Update MRC PprRepairType and PprTestType based on AMT PPR variable

  NULL implementation - returns EFI_UNSUPPORTED

  @param[in]  MemConfigNoCrc    Pointer to MEMORY_CONFIG_NO_CRC config block

  @retval     EFI_UNSUPPORTED   Feature not enabled
**/
EFI_STATUS
UpdateAmtPprMrcPolicy (
  IN MEMORY_CONFIG_NO_CRC    *MemConfigNoCrc
  )
{
  return EFI_UNSUPPORTED;
}
