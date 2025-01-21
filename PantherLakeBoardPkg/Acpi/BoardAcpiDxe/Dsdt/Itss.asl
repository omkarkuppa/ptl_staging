/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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


// ITSS
// Define the needed ITSS registers used by ASL on Interrupt
// mapping.

scope (\_SB) {
      OperationRegion (ITSS, SystemMemory, 0xfdc43100, 0x208)
      Field (ITSS, ByteAcc, NoLock, Preserve)
      {
        PARC, 8,
        PBRC, 8,
        PCRC, 8,
        PDRC, 8,
        PERC, 8,
        PFRC, 8,
        PGRC, 8,
        PHRC, 8,
       Offset (0x200),   // Offset 3300h ITSSPRC - ITSS Power Reduction Control
            , 1,
            , 1,
        SCGE, 1,         // ITSSPRC[2]: 8254 Static Clock Gating Enable (8254CGE)

      }
}


