/** @file
  Hybrid Graphics policy definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _HYBRID_GRAPHICS_CONFIG_H_
#define _HYBRID_GRAPHICS_CONFIG_H_

#define HYBRID_GRAPHICS_CONFIG_REVISION 2

#pragma pack(push, 1)

/**
  This Configuration block configures CPU PCI Express 0/1/2 RTD3 GPIOs & Root Port.
  Hybrid Gfx uses the same GPIOs & Root port as PCI Express 0/1/2 RTD3.
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Add HgSlot Policy: PEG or PCH Slot Slection for Hybrid Graphics
  <b>Revision 3</b>:
  - Removed HgSlot Policy: As there is no selection between PEG or PCH Slot for Hybrid Graphics
**/
typedef struct {
  CONFIG_BLOCK_HEADER Header;                ///< Offset 0-27 Config Block Header
  UINT16              HgSubSystemId;         ///< Offset 54 Hybrid Graphics Subsystem ID: <b>2212</b>
  UINT16              Reserved;
} HYBRID_GRAPHICS_CONFIG;
#pragma pack(pop)
#endif // _HYBRID_GRAPHICS_CONFIG_H_
