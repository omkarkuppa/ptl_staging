<!--
@file
 This file provide the details for Memory Health Insights feature.

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

* **Feature Name:** Memory Health Insights
* **PI Phase(s) Supported:** PEI & DXE
* **SMM Required?** NO

## Purpose

MHI PEIM supports reading the AmtPprEnabled variable in pre-mem and accordingly enable MRC PPR/AMT tests. This variable is then cleared in post-mem/DXE MHI.

MHI driver Provides system memory information from MRC hob to PHAT ACPI table for OS analysis use.

Currently supported function in DXE:

* Requests Memory Info Data Hob
* Converts data from AIP to ACPI
* Publish data to PHAT ACPI table

# High-Level Theory of Operation

1. MHI PEIM reads the AMT PPR Enabled variable
2. Will then access and set the Memory Config table according to the variable enablement.
3. Memory Health Insights Driver will request MEMORY_INFO_DATA_HOB from MRC
4. The driver will then convert the data to ACPI format
5. Then it will search XSDT for an existing PHAT ACPI Table
6. If the table exists we will save the data locally, uninstall the found PHAT, append the
   MHI ACPI data to the local PHAT, and install the new XSDT PHAT ACPI table
7. If the table does not exist we will generate the full PHAT ACPI Table with MHI data and install
8. MHI driver will then check for AMT PPR Enable variable
   1. If variable does not exist, it will be created
   2. If it does exist the driver will clear the variable if found

## Modules

* PeiMemoryHealthInsights.inf
* DxeMemoryHealthInsights.inf

## Configuration

* BIOS will implement feature as it is enabled by default.
* PCD for managing enablement: PcdMhiEnabled

## Data Flows

MHI PEIM

* Read AMT_PPR_ENABLE_VARIABLE_NAME
* Get gMemoryConfigGuid
* Set gMemoryConfigGuid PPR according to AMT PPR enable variable

MHI Driver

* Get MEMORY_INFO_DATA_HOB
* Convert MEMORY_INFO_DATA_HOB data to ACPI
* Add data to PHAT ACPI Table

## Control Flows

* Driver is always enabled through PcdMhiEnabled

## Build flows

* No special tools are required to build this feature.

## Feature Enabling Checklist

1. Add MemoryHealthInsightsFeaturePkg.dsc entry and path to the build file and check if all required packages/libraries exist for this package.
2. Add MemoryHealthInsightsPreMemFeaturePkg.fdf entry and path to the Pre-Memory flash map file.
3. Add MemoryHealthInsightsPostMemFeaturePkg.fdf entry and path to the Post-Memory flash map file.

## Dependencies

* Edk2Platforms/PhatAcpiLib - Handles all operations for installing MHI data to the ACPI PHAT

## Common Optimizations

* N/A

## Unit Test build flows

|   OS    | Compiler |
| :-----: | :------: |
| Windows |  VS2019  |

Step1. Prepare build environment with VS2019 as needed. Step2. Install the pip requirements. Sample : pip install -r Edk2/pip-requirements.txt. Step3. Setup Edk2 environment. Sample : Edk2/edksetup.bat Rebuild. Step4. Get the code dependencies. Sample : stuart_setup -c Intel/.pytool/CISettings.py -p MemoryHealthInsightsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose Step5. Update other dependencies. Sample : stuart_update -c Intel/.pytool/CISettings.py -p MemoryHealthInsightsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose Step6. Build unit test host. Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p MemoryHealthInsightsFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
