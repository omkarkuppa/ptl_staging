<!--
@file

 This file provides the details for Free Space Skip FV Copy feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2024 Intel Corporation.

  The source code contained or described herein, and all documents related to the
  source code ("Material") are owned by Intel Corporation or its suppliers or
  licensors. Title to the Material remains with Intel Corporation or its suppliers
  and licensors. The Material may contain trade secrets and proprietary and
  confidential information of Intel Corporation and its suppliers and licensors,
  and is protected by worldwide copyright and trade secret laws and treaty
  provisions. No part of the Material may be used, copied, reproduced, modified,
  published, uploaded, posted, transmitted, distributed, or disclosed in any way
  without Intel's prior express written permission.

  No license under any patent, copyright, trade secret or other intellectual
  property right is granted to or conferred upon you by disclosure or delivery
  of the Materials, either expressly, by implication, inducement, estoppel or
  otherwise. Any license under such intellectual property rights must be
  express and approved by Intel in writing.

  Unless otherwise agreed by Intel in writing, you may not remove or alter
  this notice or any other notice embedded in Materials by Intel or
  Intel's suppliers or licensors in any way.

  This file contains a 'Sample Driver' and is licensed as such under the terms
  of your license agreement with Intel or your vendor. This file may be modified
  by the user, subject to the additional terms of the license agreement.

@par Specification Reference:
-->
# Overview
* **Feature Name:** Free Space Skip FV Copy
* **PI Phase(s) Supported:** PEIM
* **SMM Required?** NO

## Purpose
This package implements for Free Space Skip FV Copy features only.

# High-Level Theory of Operation
* The Firmware Volumes(FVs) has huge amounts of Free Space which are copied over by
* FvReportPei from SPI Flash to Memory(destination). This boot performance optimization copies only
* the used space in the FV from SPI Flash to Memory, and replicates the free space of the FV in memory
* using cpu based instructions, to save time.
* If we traverse the Ffs files, we can find the PAD files(empty files) or freespace at the end of the FV,
* We can use setmem to fill the freespace based on the polarity mentioned
* in the FV Header. This is a faster operation than copying over Free Space from
* Flash to Memory, to avoid using a very slow SPI protocol.

* PEIM Drivers
 * FreeSpaceSkipFvCopyPei

## FreeSpaceSkipFvCopyLib.inf
  - PEIM to register the gEdkiiPeiFirmwareVolumeShadowPpiGuid.
  - If SPI DMA has already registered the gEdkiiPeiFirmwareVolumeShadowPpiGuid
  - We will deregister it, and reinstall gEdkiiPeiFirmwareVolumeShadowPpiGuid with FreeSpaceSkipFvCopy's implementation.
  - FreeSpaceSkipFvCopy will use the SPI DMA Ppi for copying relevant data if it exists.

## FreeSpaceSkipFvCopyHostTest.inf
  - Will test the FreeSpaceSkipFvCopy function for both code flows
  - whether SpiDmaPpi exists or not.

## Key Functions
C code:
* FreeSpaceSkipEntryPoint
* FreeSpaceSkipFvCopy

## Configuration
* Set gFvCopyPkgTokenSpaceGuid.PcdFreeSpaceSkipFvCopyEnable to TRUE will enable FreeSpaceSkipFvCopyPei PEIM in conjuction with SpiDmaPpi
* Set gFvCopyPkgTokenSpaceGuid.PcdFreeSpaceSkipFvCopyEnable to FALSE will disable FreeSpaceSkipFvCopyPei PEIM

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
* Step1. prepare build environment with VS2019 as needed
* Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Edk2Platforms/Platform/Intel;%cd%/Intel;%cd%/Intel/Features
* Step3. Edk2\edksetup.bat
* Step4. build -p FvCopyFeaturePkg/FvCopyFeaturePkg.dsc -a IA32 -a X64 -b DEBUG
* Step5. build -p FvCopyFeaturePkg/FvCopyFeaturePkg.dsc -a IA32 -a X64 -b RELEASE

## Functional Exit Criteria
* Any error while performing FreeSpaceSkipFvCopy or SpiDmaPpi will cause the FV Copy to fail.

## Unit Test build flows

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed.
> Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt
> Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild
> Step4. Get the code dependencies.
         Sample : stuart_setup -c .pytool/CISettings.py -a X64 TOOL_CHAIN_TAG=VS2019
> Step5. Update other dependencies.
         Sample : stuart_update -c .pytool/CISettings.py -a X64 TOOL_CHAIN_TAG=VS2019
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Features\FvCopyFeaturePkg\CISettings.py -a X64 -t NOOPT -v TOOL_CHAIN_TAG=VS2019 -p FvCopyFeaturePkg BUILDMODULE=Features\FvCopyFeaturePkg\FreeSpaceSkipFvCopyPei\UnitTest\FreeSpaceSkipFvCopyHostTest.inf