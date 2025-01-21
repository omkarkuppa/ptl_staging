<!--
@file

 This file provide the details for Multi-IBB feature.

@copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
* **Feature Name:** MultiIbbFeaturePkg
* **PI Phase(s) Supported:** PEI
* **SMM Required?** No

## Purpose
This package implements Multi-IBB feature which splits original one single IBB into one common IBB and multiple smaller IBBs for loading sequentially to reduce the required NEM (No-Evict Mode) size for execution.

# High-Level Theory of Operation
This feature provides services of Multi-IBB support:

1. MultiIbbServicePei.inf will initialize the service for Multi-IBB support after MultiIbbConfig at platform installs gPeiFirmwareVolumeVerifyHashFvPpiGuid to retrieve platform flash map information.
2. EndOfIbbPei.inf will install gEfiEndOfIbbSignalPpiGuid to callback MultiIbbService for switching to the next IBB for execution at the end of each IBB.
3. AccessNemConfigLib provides APIs for you to access functions in BaseNemConfigLib placed in 1S Pkg to adjust NEM range and calculate the current remaining NEM size.

## Firmware Volumes
* FvBspPreMemory

## Modules
* MultiIbbServicePei
* EndOfIbbPei
* AccessNemConfigLib

## MultiIbbServicePei
The PEIM initializes the service for Multi-IBB support to verify the integrity of IBBs and switch the next IBB that is going to be executed into cache.
This service is enabled only after MultiIbbConfig at platform installs gPeiFirmwareVolumeVerifyHashFvPpiGuid to retrieve platform flash map information and update PcdMultiIbbInfoCount & PcdAcmSetNem with correct values. If the retrieved value of PcdAcmSetNem is False, that means NEM is not initiated by Boot Guard ACM and Multi-IBB will rearrange Mtrrs setting to cache only Common IBB range at the beginning.

## EndOfIbbPei
The PEIM is included at the end of each IBB to install gEfiEndOfIbbSignalPpiGuid and callback MultiIbbService for switching to the next IBB for execution.

## AccessNemConfigLib
A library provides APIs for accessing functions in BaseNemConfigLib placed in 1S Pkg to adjust NEM range and calculate the current remaining NEM size.

## Configuration
Set gMultiIbbFeaturePkgTokenSpaceGuid.PcdCacheMissDetectEnable to TRUE to enable "Cache Miss Detect" debug feature.

## Data Flows
Enabled after MultiIbbConfig at platform installs gPeiFirmwareVolumeVerifyHashFvPpiGuid to retrieve platform flash map information. MultiIbbConfig also registers gMtrrRangeChangePpiGuid and gNemSizeCalculatePpiGuid to wait for notify callback from MultiIbbService to access BaseNemConfigLib outside of Feature Pkg.
EndOfIbb callbacks to MultiIbbService at the end of each IBB to switch in the next IBB into cache and verify it.

## Control Flows
PcdMultiIbbFeatureEnable to enable this feature.

## Build Flows
* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows |  VS2019  |

> Step1. prepare build environment with VS2019 as needed <BR>
> Step2. set PACKAGES_PATH=%cd%/Edk2/;%cd%/Intel/;%cd%/Edk2Platforms/Silicon/Intel/;%cd%/Edk2Platforms/Platform/Intel/;%cd%/Intel/Features/Storage/ <BR>
> Step3. Edk2\edksetup.bat Rebuild<BR>
> Step4. build -p MultiIbbFeaturePkg/MultiIbbFeaturePkg.dsc -a IA32 -a X64 -D FSP_ARCH=IA32 -t VS2019 -b DEBUG <BR>
> Step5. build -p MultiIbbFeaturePkg/MultiIbbFeaturePkg.dsc -a IA32 -a X64 -D FSP_ARCH=IA32 -t VS2019 -b RELEASE <BR>
> Step6. build -p MultiIbbFeaturePkg/MultiIbbFeaturePkg.dsc -a IA32 -a X64 -D FSP_ARCH=X64 -t VS2019 -b DEBUG <BR>
> Step7. build -p MultiIbbFeaturePkg/MultiIbbFeaturePkg.dsc -a IA32 -a X64 -D FSP_ARCH=X64 -t VS2019 -b RELEASE <BR>

## Test Point Results
Verified by bios debug log.

## Functional Exit Criteria
Check by bios debug log to see if gPeiFirmwareVolumeVerifyHashFvPpiGuid is installed and updated with platform information successfully.
Also check wether the system can boot successfully with Multi-IBB feature enabled.

## Feature Enabling Checklist
PcdMultiIbbFeatureEnable enabled. <BR>
PcdCacheMissDetectEnable enabled if the cache miss debug service is needed. <BR>
Platform needs to run MultiIbbConfig before MultiIbbService initialization. <BR>
Platform needs to include MultiIbbServicePei.inf to initialize the Multi-IBB support. <BR>
Platform needs to include EndOfIbbPei.inf to callback at the end of each IBB (except the first and the last IBB don't need to) on their own.

## Common Optimizations
N/A
## Unit Test build flows

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment as needed.
         - Visual Studio VS2019
         - Python 3.10
         - NASM 2.15.05
         - OpenCppCoverage
         - virtualenv <BR>
> Step2. Perform a one-time setup of python virtual environments.
         Sample : py -m venv unit-testing <BR>
> Step3. Activate the python virtual environment.
         Sample : .\unit-testing\Scripts\activate <BR>
> Step4. Install the pip requirements.
         Sample : pip install -r Edk2\pip-requirements.txt <BR>
> Step5. Setup Edk2 environment.
         Sample : Edk2\edksetup.bat Rebuild <BR>
> Step6. Get the code dependencies.
         Sample : stuart_setup -c Intel\.pytool\CISettings.py -p MultiIbbFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose <BR>
> Step7. Update other dependencies.
         Sample : stuart_update -c Intel\.pytool\CISettings.py -p MultiIbbFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose <BR>
> Step8. Build unit test host.
         Sample : stuart_ci_build -c Intel\.pytool\CISettings.py -p MultiIbbFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose <BR>
