<!--
@file

 This file provide the details for FSP wrapper feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright 2021 Intel Corporation.

  The source code contained or described herein and all documents related to the
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
* Feature Name: FSP wrapper feature
* PI Phase(s) Supported: SEC, PEI, DXE
* SMM Required: No

## Purpose
This feature package implements FSP wrapper feature.

# High-Level Theory of Operation
To consume FSP provided by Intel

## Firmware Volumes
* PreMemory
* PostMemory

## Modules
This feature package support 3 Modules as follows.

  1) CheckFspErrorInfoHobDxe.inf
    This DXE driver registers some callbacks to check FSP_ERROR_INFO HOBs output by FSP notify phase APIs.

  2) DebugEventHandler.inf
    This PEI driver installs gDebugEventHandlerPpiGuid to provide debug event handler to report status code.

  3) FspWrapperPeim.inf
    This PEI driver performs PlatformLateInit, PchSmmControlInit and PublishFspVersion functions. If boot mode is S3, it will install MP service for FSP dispatch mode and register MTRR notify for S3 resume.

  4) FspVPhatUpdate.inf
    This DXE driver was created to support FSPV feature. It locates all the HOBs created by FSPV PEIMs and publishes PHAT table with HOB data.

This feature supports multiple Libraries.

  5) SecFspWrapperPlatformSecLib.inf
    Provide FSP wrapper platform SEC related functions, SEC entry and PEI entry to consume FSP APIs.

  6) PeiFspWrapperPlatformLib.inf
    This Library provides FSP wrapper platform related reset and some APIs below for PEI phase.
    a. CallFspWrapperResetSystem - Perform platform related reset in FSP wrapper.
    b. UpdateFspmUpdData - Override the default configurations in the FSP-M UPD data region.
    c. UpdateFspsUpdData - Override the default configurations in the FSP-S UPD data region.
    d. UpdateTempRamExitParam - Update TempRamExit parameter.
    e. GetS3MemoryInfo - Get S3 PEI memory information.

  7) DxeFspWrapperPlatformLib.inf
    This Library provides FSP wrapper platform related reset for DXE phase.

## Configuration
There is no special configuration. Platform can have its own SioSerialPortInitHookLib to replace the null one provided by FSP wrapper package.

## Data Flows

## Build Flows
No special tools are required to build this feature.

## Feature Enabling Checklist
Include FspWrapperFeature.dsc, FspWrapperPkgPeiPreMem.fdf, FspWrapperPkgPeiPostMem.fdf and FspWrapperPkgDxe.fdf in platform dsc and fdf files respectively.

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p FspWrapperFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p FspWrapperFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p FspWrapperFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose