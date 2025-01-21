<!--
@file

 This file provide the details for protected variable feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials, and your
  use of them is governed by the express license under which they were provided to
  you ("License"). Unless the License provides otherwise, you may not use, modify,
  copy, publish, distribute, disclose or transmit this software or the related
  documents without Intel's prior written permission.

  This software and the related documents are provided as is, with no express or
  implied warranties, other than those that are expressly stated in the License.

@par Specification Reference:
-->
# Overview
* **Feature Name:** ProtectedVariableFeaturePkg
* **PI Phase(s) Supported:** PEI
* **SMM Required?** Yes

## Purpose
This feature package implements protected variable related platform features for protected variable.

# High-Level Theory of Operation
This feature provides Protected Variable related platform features:

1. KeyServicePei.inf will get root key and trusted root key from CSME, store in
   Key Service Trusted Channel Hob and install KeyServicePpi.
2. TrustedChannelSmm.inf retrieves this trusted root key from KeyServicePei via a new HOB.
3. PeiProtectedVariableRecovery.inf provides integrity check handler for recovery.

## Firmware Volumes
* PreMemory and PostMemory

## Modules
* KeyServicePei
* TrustedChannelSmm
* PeiRpmcLib
* SmmRpmcLib
* PeiProtectedVariableRecovery

## KeyServicePei
The driver sends command to CSME to retrieve root key and trusted root key.

## TrustedChannelSmm
TrustedChannelSmm.inf retrieves this trusted root key from KeyServicePei via a new HOB.

## PeiProtectedVariableRecovery
The driver will install a integrity check handler for recovery. This is a template handler therefore recovery of variables is to be
designed in by the final product.

## PeiRpmcLib
Requests the current monotonic counter from CSME and increments the designated monotonic counter in the SPI flash device by 1 in PEI phase.

## SmmRpmcLib
Requests the current monotonic counter from CSME and increments the designated monotonic counter in the SPI flash device by 1 in SMM phase.

## Configuration
There is not special configuration.

## Data Flows
Architecturally defined data structures and flows for the feature.

## Control Flows
PcdProtectedVariableEnable to enable this feature at build time.
PcdRpmcSupported to indicate Rpmc hardware is detected.

## Build Flows
No any special build flow is needed.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

## Test Point Results
Verified by bios debug log.

## Functional Exit Criteria
Check by bios debug log to see if KEY_SERVICE_PPI and HECI_TRUSTED_CHANNEL_PROTOCOL is installed successfully.

## Feature Enabling Checklist
gHeciControlPpiGuid is installed.
PcdRpmcSupported and PcdProtectedVariableEnable are enabled.

## Common Optimizations

## Unit Test build flows
