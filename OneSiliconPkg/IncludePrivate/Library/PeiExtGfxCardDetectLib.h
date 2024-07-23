/** @file
  Header file for the External Gfxcard Detect Lib
  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _PEI_EXT_GFX_CARD_DETECT_LIB_H_
#define _PEI_EXT_GFX_CARD_DETECT_LIB_H_

#include <Library/DomainIGpuInit.h>

//
// Functions
//
/**
  This function enumerate all downstream bridge.

  @param[in] BusNum  - Primary bus number of current bridge

  @retval Current bus number: if current bus is an enpoint device
  @retval subordinate bus number: if current bus is a bridge
**/
UINT8
EnumerateDownstream (
  IN UINT8 BusNum
  );

/**
  Find the MMIO size that a given PCI device requires

  @param[in] BusNum     - Bus number of the device
  @param[in] DevNum     - device Number of the device
  @param[in] FunNum     - Function number of the device
  @param[out] MmioLength - MMIO Length in bytes
**/
VOID
FindPciDeviceMmioLength (
  IN UINT32  BusNum,
  IN UINT32  DevNum,
  IN UINT32  FunNum,
  OUT UINT32 *MmioLength
  );

/**
  CheckOffboardPcieVga: Check if off board PCIe graphics Card is present

  @param[in, out] PchPcieMmioLength       - Total PCIe MMIO length on all PCH root ports
  @param[in, out] PrimaryDisplay          - Primary Display - default is IGD
  @param[in]      ResizableBarSupport     - Resizable BAR is support
**/
VOID
CheckOffboardPcieVga (
  IN OUT   UINT32                       *PchPcieMmioLength,
  IN OUT   DISPLAY_DEVICE               *PrimaryDisplay,
  IN       UINT8                        ResizableBarSupport
  );

#endif
