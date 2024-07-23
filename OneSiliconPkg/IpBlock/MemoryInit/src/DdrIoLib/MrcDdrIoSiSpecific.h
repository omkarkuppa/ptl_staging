/** @file
  This file contains code related to Ddrio Silicon W/As

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

#ifndef MRC_DDRIO_SI_WA_H_
#define MRC_DDRIO_SI_WA_H_

/**
  Clear the top 32KB of Xtensa DRAM0 region.
  @param[in] MrcData - Include all MRC global data.
**/
VOID
BlueMrcClearXtensaDramTop (
  IN MrcParameters *const MrcData
  );

/**
  Set TerDcd Override Values

  @param[in] MrcData    - Include all MRC global data.
  @param[in]  Controller  - Memory controller in the processor socket (0-based).
  @param[in]  Channel     - DDR Channel Number within the memory controller (0-based).
  @param[in]  Byte        - DDR Byte (0-based).
  @param[in]  IsEnable    - TRUE if override values should be set

**/
VOID
MrcOverrideTerDcd (
  IN OUT MrcParameters* const MrcData,
  UINT8     Controller,
  UINT8     Channel,
  UINT8     Byte,
  BOOLEAN   IsEnable
  );

#endif // MRC_DDRIO_SI_WA_H_

