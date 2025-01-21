/** @file
  Header file for HSIO handle

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

@par Specification
**/

#ifndef _HSIO_HANDLE_H_
#define _HSIO_HANDLE_H_

#include <ConfigBlock.h>
#include <HsioConfig.h>
#include <HsioPcieConfig.h>
#include <HsioSataConfig.h>
#include <HsioLane.h>
#include <Usb3HsioConfig.h>

/**
  Internal HSIO policy options
**/
typedef struct {
  /**
    This is internal switch which allow skip updating Chipset Init table from IP block flow
    EBG is not supporting updating SUS tables.
  **/
  BOOLEAN                    SkipWriteToChipsetInitTable;
  /**
    Specifies the Pcie Pll Spread Spectrum Percentage
    TODO: move this option to HSIO config block
  **/
  UINT8                      PciePllSsc;
} HSIO_PRIVATE_CONFIG;

/**
  HSIO device structure
  Stores all data necessary to initialize HSIO IP block
**/
typedef struct {
  HSIO_LANE                   *Lane;
  USB3_HSIO_CONFIG            *Usb3HsioConfig;
  PCH_HSIO_SATA_PREMEM_CONFIG *SataLanePreMemConfig;
  PCH_HSIO_PCIE_PREMEM_CONFIG *PcieLanePreMemConfig;
  HSIO_PRIVATE_CONFIG         *PrivateConfig;
  HSIO_CONFIG                 *HsioConfig;
} HSIO_HANDLE;

#endif // _HSIO_HANDLE_H_
