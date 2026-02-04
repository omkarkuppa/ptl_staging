/** @file
  Memory Telemetry Library definition Header file.

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

#ifndef _MEMORY_TELEMETRY_LIB_H_
#define _MEMORY_TELEMETRY_LIB_H_

///
/// MRC Memory Types API matching definitions
///
#define NOREPAIR_PPR      0
#define HARD_PPR          2
#define PPR_TEST          6

/**
  Update MRC PprRepairType and PprTestType based on AMT PPR variable

  Reads AMT PPR variable and updates memory configuration policies directly.
  If AMT is enabled:
    - Sets MemConfigNoCrc->PprTestType.Value to PPR_TEST (6)
    - Sets MemConfigNoCrc->PprRepairType to HARD_PPR if PPR capability enabled, else NOREPAIR_PPR

  @param[in]  MemConfigNoCrc    Pointer to MEMORY_CONFIG_NO_CRC config block

  @retval     EFI_SUCCESS       AMT enabled and policies updated successfully
  @retval     EFI_NOT_FOUND     AMT PPR variable not found or AMT not enabled
  @retval     Others            Failed to locate PPI or get config block
**/
EFI_STATUS
UpdateAmtPprMrcPolicy (
  IN MEMORY_CONFIG_NO_CRC    *MemConfigNoCrc
  );

#endif //_MEMORY_TELEMETRY_LIB_H_
