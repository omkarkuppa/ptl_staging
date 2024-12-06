/** @file
  SoundWire topology definitions

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

#define SIDECAR_GPIO_MASK_GPIO1         0x00000100
#define SIDECAR_GPIO_MASK_GPIO2         0x00000200
#define SIDECAR_GPIO_MASK_GPIO3         0x00000400
#define SIDECAR_GPIO_MASK_SPI_SS        0x80000000

Package(2) { "01fa-sidecar-instances", 2 },

Package(2) { "01fa-sidecar-0-sidecar-type", 0x3556 },
#ifndef SIDECAR_0_SPI_SS_GPIO_MASK
// The default SPI chip select for sidecar 0 is SPI_SS.
Package(2) { "01fa-sidecar-0-spi-ss-gpio-mask", SIDECAR_GPIO_MASK_GPIO1 },
#else
// Use the custom provided mask for sidecar 0.
Package(2) { "01fa-sidecar-0-spi-ss-gpio-mask", SIDECAR_0_SPI_SS_GPIO_MASK },
#endif
Package(2) { "01fa-sidecar-0-parallelize-sidecar-fw-download", 1 },
Package(2) { "01fa-sidecar-0-cal-data-index", 0 },

Package(2) { "01fa-sidecar-1-sidecar-type", 0x3556 },
#ifndef SIDECAR_1_SPI_SS_GPIO_MASK
// The default SPI chip select for sidecar 1 is GPIO1.
Package(2) { "01fa-sidecar-1-spi-ss-gpio-mask", SIDECAR_GPIO_MASK_SPI_SS },
#else
// Use the custom provided mask for sidecar 1.
Package(2) { "01fa-sidecar-1-spi-ss-gpio-mask", SIDECAR_1_SPI_SS_GPIO_MASK },
#endif
Package(2) { "01fa-sidecar-1-parallelize-sidecar-fw-download", 1 },
Package(2) { "01fa-sidecar-1-cal-data-index", 1 },
