/** @file
  PCH preserved MMIO resource definitions.

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

#ifndef _RESERVED_RESOURCE_PRODUCT_H_
#define _RESERVED_RESOURCE_PRODUCT_H_

/**
  Detailed recommended static allocation
  +----------------------------------------------------------------------------------------------------------+
  | PCH preserved MMIO range, 16 MB, from 0xFD800000 to 0xFE7FFFFF                                           |
  +----------------------------------------------------------------------------------------------------------+
  | Size        | Start        | End          | Usage                                                        |
  | 8 MB        | 0xFD800000   | 0xFDFFFFFF   | TraceHub SW BAR (SOC)                                        |
  | 64 KB       | 0xFE000000   | 0xFE00FFFF   | PMC MBAR                                                     |
  | 4 KB        | 0xFE010000   | 0xFE010FFF   | SPI BAR0                                                     |
  | 176 KB      | 0xFE020000   | 0xFE04BFFF   | SerialIo BAR in ACPI mode                                    |
  | 400 KB      | 0xFE04C000   | 0xFE0AFFFF   | Unused                                                       |
  | 64 KB       | 0xFE0B0000   | 0xFE0BFFFF   | eSPI LGMR BAR                                                |
  | 64 KB       | 0xFE0C0000   | 0xFE0CFFFF   | eSPI2 SEGMR BAR                                              |
  | 192 KB      | 0xFE0D0000   | 0xFE0FFFFF   | Unused                                                       |
  | 1 MB        | 0xFE100000   | 0xFE1FFFFF   | TraceHub MTB BAR (SOC)                                       |
  | 2 MB        | 0xFE200000   | 0xFE3FFFFF   | TraceHub FW BAR (SOC)                                        |
  | 64 KB       | 0xFE400000   | 0xFE40FFFF   | IOE PMC BAR                                                  |
  | 64 KB       | 0xFE410000   | 0xFE41FFFF   | Tracehub RTIT BAR (SOC)                                      |
  | 896 KB      | 0xFE420000   | 0xFE4FFFFF   | Unused                                                       |
  | 1 MB        | 0xFE500000   | 0xFE5FFFFF   | Unused                                                       |
  | 2 MB        | 0xFE600000   | 0xFE7FFFFF   | Temp address                                                 |
  | 4 KB        | 0xFEDA2000   | 0xFEDA2FFF   | HECI1 BAR0                                                   |
  | 4 KB        | 0xFEDA3000   | 0xFEDA3FFF   | HECI2 BAR0                                                   |
  | 4 KB        | 0xFEDA4000   | 0xFEDA4FFF   | HECI3 BAR0                                                   |
  +----------------------------------------------------------------------------------------------------------+
  | SBREG MMIO range                                                                                         |
  +----------------------------------------------------------------------------------------------------------+
  | 256 MB       | 0x4000000000   | 0x400FFFFFFF   | SBREG                                                   |
  | 256 MB       | 0x4010000000   | 0x401FFFFFFF   | 2nd SBREG                                               |
  +----------------------------------------------------------------------------------------------------------+
**/
#define PCH_PRESERVED_BASE_ADDRESS          0xFD800000     ///< Pch preserved MMIO base address
#define PCH_PRESERVED_MMIO_SIZE             0x01000000     ///< 16MB
#define PCH_TRACE_HUB_SW_BASE_ADDRESS       0xFD800000     ///< TraceHub SW MMIO base address
#define PCH_TRACE_HUB_SW_MMIO_SIZE          0x00800000     ///< 8MB
#define PCH_PWRM_BASE_ADDRESS               0xFE000000     ///< PMC MBAR MMIO base address
#define PCH_PWRM_MMIO_SIZE                  0x00010000     ///< 64KB
#define PCH_SPI_BASE_ADDRESS                0xFE010000     ///< SPI BAR0 MMIO base address
#define PCH_SPI_MMIO_SIZE                   0x00001000     ///< 4KB
#define PCH_SERIAL_IO_BASE_ADDRESS          0xFE020000     ///< SerialIo MMIO base address
#define PCH_SERIAL_IO_MMIO_SIZE             0x0002C000     ///< 176KB
#define PCH_ESPI_LGMR_BASE_ADDRESS          0xFE0B0000     ///< eSPI LGMR MMIO base address
#define PCH_ESPI_LGMR_MMIO_SIZE             0x00010000     ///< 64KB
#define PCH_ESPI_SEGMR_BASE_ADDRESS         0xFE0C0000     ///< Second eSPI GMR MMIO base address
#define PCH_ESPI_SEGMR_MMIO_SIZE            0x00010000     ///< 64KB
#define PCH_TRACE_HUB_MTB_BASE_ADDRESS      0xFE100000     ///< TraceHub MTB MMIO base address
#define PCH_TRACE_HUB_MTB_MMIO_SIZE         0x00100000     ///< 1MB
#define PCH_TRACE_HUB_FW_BASE_ADDRESS       0xFE200000     ///< TraceHub FW MMIO base address
#define PCH_TRACE_HUB_FW_MMIO_SIZE          0x00200000     ///< 2MB
#define PCH_TRACE_HUB_RTIT_BASE_ADDRESS     0xFE410000     ///< TraceHub RTIT MMIO base address
#define PCH_TRACE_HUB_RTIT_MMIO_SIZE        0x00010000     ///< 64KB
#define PCH_TEMP_BASE_ADDRESS               0xFE600000     ///< preserved temp address for misc usage
#define PCH_TEMP_MMIO_SIZE                  0x00200000     ///< 2MB
#define PCH_HECI1_BASE_ADDRESS              0xFEDA2000     ///< HECI1 BAR0 MMIO base address
#define PCH_HECI1_MMIO_SIZE                 0x00001000     ///< 4KB
#define PCH_HECI2_BASE_ADDRESS              0xFEDA3000     ///< HECI2 BAR0 MMIO base address
#define PCH_HECI2_MMIO_SIZE                 0x00001000     ///< 4KB
#define PCH_HECI3_BASE_ADDRESS              0xFEDA4000     ///< HECI3 BAR0 MMIO base address
#define PCH_HECI3_MMIO_SIZE                 0x00001000     ///< 4KB

#define PCH_PCR_BASE_ADDRESS                0x4000000000     ///< SBREG MMIO base address
#define PCH_PCR_SECOND_BASE_ADDRESS         0x4010000000     ///< Second SBREG MMIO base address
#define PCH_PCR_MMIO_SIZE                   0x10000000       ///< 256MB


#endif
