/** @filepchpcibdflibpri
  PCI SBDF definition

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

#ifndef _PCI_SBDF_H
#define _PCI_SBDF_H

/*
  PCI Segment:Bus:Device:Function structure
  The structure is aligned to PCI Segment library addres representation

  | Reserved (MBZ) | Segment | Reserved (MBZ) |     Bus     | Device | Function | Register |
  63           48  47      32 31           28 27         20 19    15 14      12 11         0
*/
typedef union {
  struct {
    UINT64    Reg  : 12;    // PCI Register: Bits 0..11, Range 0..255 for PCI.  Range 0..4095 for PCI Express.
    UINT64    Func : 3;     // PCI Function  Bits 12..14, Range 0..7
    UINT64    Dev  : 5;     // PCI Device  Bits 15..19, Range 0..31
    UINT64    Bus  : 8;     // PCI Bus Bits 20..27, Range 0..255
    UINT64    Res1 : 4;     // Reserved  Bits 28..31.  Must be 0.
    UINT64    Seg  : 16;    // PCI Segment Bits 32..47, Range 0..65535
    UINT64    Res2 : 16;    // Reserved  Bits 48..63.  Must be 0
  } Fields;
  UINT64    PciSegmentLibAddress; // PCI Segment Library Address representation
} PCI_SBDF;

#endif
