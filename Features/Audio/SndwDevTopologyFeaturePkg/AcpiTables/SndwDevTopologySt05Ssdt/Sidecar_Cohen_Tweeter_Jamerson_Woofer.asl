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

Package(2) { "01fa-sidecar-instances", 2 },
Package(2) { "01fa-sidecar-0-sidecar-type", 0x3556 },
Package(2) { "01fa-sidecar-0-spi-ss-gpio-mask", 0x400 }, // GPIO3 mask
Package(2) { "01fa-sidecar-0-parallelize-sidecar-fw-download", 1 },
Package(2) { "01fa-sidecar-0-cal-data-index", 0 },
Package(2) { "01fa-sidecar-1-sidecar-type", 0x3556 },
Package(2) { "01fa-sidecar-1-spi-ss-gpio-mask", 0x100 }, // GPIO1 mask
Package(2) { "01fa-sidecar-1-parallelize-sidecar-fw-download", 1 },
Package(2) { "01fa-sidecar-1-cal-data-index", 1 },
