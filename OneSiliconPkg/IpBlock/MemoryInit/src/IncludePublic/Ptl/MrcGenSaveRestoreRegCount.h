/**
@file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

@par Specification Reference:
**/

#ifndef MRC_GEN_SAVE_RESTORE_REG_COUNTS
#define MRC_GEN_SAVE_RESTORE_REG_COUNTS

// Total Registers including hardcoded MrsFsmCtrl regs: 3006
// PHY Common 32 Bit Registers: 68
// PHY Common 64 Bit Registers: 0
// MC Common 32 Bit Registers: 742
// MC Common 64 Bit Registers: 24

// PHY SaGv 32 Bit Registers: 2033
// PHY SaGv 64 Bit Registers: 0
// MC SaGv 32 Bit Registers: 100
// MC SaGv 64 Bit Registers: 40

#define MRC_REGISTER_COUNT_COMMON  (3412 / sizeof (UINT32))
#define MRC_REGISTER_COUNT_SAGV    (8824 / sizeof (UINT32))

#endif // MRC_GEN_SAVE_RESTORE_REG_COUNTS
