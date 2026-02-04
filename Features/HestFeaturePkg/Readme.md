<!--
@file

 This file provide the details for HestFeaturePkg.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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
* **Feature Name:** HestFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To install ACPI HEST table for Error masking

# High-Level Theory of Operation
This package install ACPI BERT table mask device’s HLOS.

## Firmware Volumes
* AdvancedUncompact

## Configuration
* Set gHestFeaturePkgTokenSpaceGuid.PcdHestFeatureEnable to TRUE to enable HEST feature.

## Key Functions
* HestDxeEntry

## HestDxeEntry
It will install HEST Table with PCI Express Device AER Structure

## Build Flows

* No special tools are required to build this feature.

|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |
| Windows | CLANG20  |

> Step1. prepare build environment with VS2019 or CLANG20 as needed
> Step2. set PACKAGES_PATH=%cd%/Edk2;%cd%/Intel;%cd%/Intel/Features/
> Step3. Edk2\edksetup.bat
> Step4. build -p Intel/Features/HestFeaturePkg/HestFeaturePkg.dsc -a X64 -t VS2019
> Step5. build -p Intel/Features/HestFeaturePkg/HestFeaturePkg.dsc -a X64 -t CLANGPDB

## Test Point Results
Verified by bios debug log.

## Functional Exit Criteria
Check if ACPI HEST table is installed on OS.

## Feature Enabling Checklist
Include HestFeature.dsc

## Unit Test build flows
* TBD