/** @file
  Build time limits of PCH resources.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#ifndef PCH_LIMITS_PRODUCT_H_
#define PCH_LIMITS_PRODUCT_H_
/*
 * Defines povided in this file are indended to be used only where static value
 * is needed. They are set to values which allow to accomodate multiple projects
 * needs. Where runtime usage is possible please used dedicated functions from
 * PchInfoLib to retrieve accurate values
 */

//
// HSIO limits
//
#define PCH_MAX_HSIO_LANES                  38

//
// PCIe limits
//
#define MAX_PCIE_ROOT_PORTS                 20

#define PCH_MAX_PCIE_ROOT_PORTS             12
#define PCH_MAX_PCIE_CONTROLLERS            5

//
// PCIe clocks limits
//
#define PCH_MAX_PCIE_CLOCKS                 10

//
// DMI lanes
//
#define PCH_MAX_DMI_LANES                   8

//
// PCH PSF segments
//
#define PCH_MAX_PSF_SEGMENTS                8

//
// RST PCIe Storage Cycle Router limits
//
#define PCH_MAX_RST_PCIE_STORAGE_CR         3

//
// SATA limits
//
#define PCH_MAX_SATA_CONTROLLERS            1
#define PCH_MAX_SATA_PORTS                  1

//
// SerialIo limits
//
#define PCH_MAX_SERIALIO_I2C_CONTROLLERS      8
#define PCH_MAX_SERIALIO_I3C_CONTROLLERS      3
#define PCH_MAX_SERIALIO_SPI_CONTROLLERS      4
#define PCH_MAX_SERIALIO_SPI_CHIP_SELECTS     2
#define PCH_MAX_SERIALIO_UART_CONTROLLERS     4

//
// THC limits
//
#define PCH_MAX_THC_CONTROLLERS               2
//
// ISH limits
//
#define PCH_MAX_ISH_GP_PINS                   12
#define PCH_MAX_ISH_UART_CONTROLLERS          2
#define PCH_MAX_ISH_I2C_CONTROLLERS           3
#define PCH_MAX_ISH_I3C_CONTROLLERS           2
#define PCH_MAX_ISH_SPI_CONTROLLERS           1
#define PCH_MAX_ISH_SPI_CS_PINS               1
//
// HDA limits
//
#define PCH_MAX_HDA_SDI                       2
#define PCH_MAX_HDA_SSP_LINK_NUM              7
#define PCH_MAX_HDA_SSP_EXTENDED_LINK_NUM     4
#define PCH_MAX_HDA_DMIC_LINK_NUM             2
#define PCH_MAX_HDA_SNDW_LINK_NUM             5
#define PCH_MAX_HDA_SNDW_MULTILANE_NUM        2

//
// Number of eSPI devices
//
#define PCH_MAX_ESPI_DEVICES                 4

//
// Number of UFS controllers
//
#define PCH_MAX_UFS_DEV_NUM                 2

#define SETUP_MAX_USB2_PORTS                16
#define SETUP_MAX_USB3_PORTS                10
#define PCH_MAX_ADR_SOURCE_NUMBER           25

//
// Number of TSN controllers
//
#define PCH_MAX_TSN_PORT                    4

//
// Number of CAN controllers
//
#define PCH_MAX_CAN_PORT                    2

#endif
