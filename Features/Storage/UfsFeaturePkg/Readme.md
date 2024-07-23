<!--
@file
  This file provide the details for UFS feature.

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
-->
# Overview

* Feature Name: Universal Flash Storage(UFS) platform driver
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This feature enables platform logic required to fully support boot from UFS.

Currently supported use cases:
* Applying controller specific work arounds

# High-Level Theory of Operation

Feature consists of a single driver(UfsPlatformDxe.inf) which is responsible for installing the protocol instance of EDKII\_UFS\_HC\_PLATFORM\_PROTOCOL. The protocol
is called from UFS driver and allows the platform to introduce platform-specific behavior at predefined points in UFS flows.

## Modules

* UfsPlatformDxe.inf

## Configuration

There is no build-time configuration of the feature at the moment.

## Data flows

UfsPlatform driver installs the EDKII\_UFS\_HC\_PLATFORM\_PROTOCOL which is consumed by standard UFS driver.

## Control Flows

PcdUfsFeatureEnable to enable this feature.
PcdUfsEraseSupport to enable UFS erase driver.

## Build flows

The UfsPlatformDxe.inf module has to be dispatched before UFS driver starts binding with hardware. It is platform responsibility to make
sure the module is correctly located in the flash memory.

## Unit Test build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed. <BR>
> Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt <BR>
> Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild <BR>
> Step4. Get the code dependencies.
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p UfsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 <BR>
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p UfsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 <BR>
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p UfsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose <BR>
