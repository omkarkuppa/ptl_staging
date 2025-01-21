/** @file
  Header file for SPI handle structure definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _SPI_HANDLE_H_
#define _SPI_HANDLE_H_

#include <Base.h>
#include <SpiConfig.h>
#include <SpiConfigHob.h>

typedef struct _SPI_HANDLE SPI_HANDLE;

/**
  SPI Flash Controller handle
**/
struct _SPI_HANDLE {
  /**
    SPI controller PCI config space address in PCI Segment Library representation.
  **/
  UINT64                       PciCfgBase;
  /**
    MMIO base address that will be programmed into SPI PCI Config and used to
    access memory mapped registers
  **/
  UINTN                        MmioBase;
  /**
    SPI Config Block instance pointer. This holds SPI related configuration.
  **/
  SPI_CONFIG                   *SpiConfig;
  /**
    SPI Config HOB instance pointer.
  **/
  SPI_CONFIG_HOB               *SpiConfigHob;
};

#endif

