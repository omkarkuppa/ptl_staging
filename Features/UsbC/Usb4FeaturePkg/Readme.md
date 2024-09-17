<!--
  @file

  This file provide the details for Usb4FeaturePkg.

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
* **Feature Name:** Usb4FeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To provide discrete TBT functions on platform

# High-Level Theory of Operation


## Firmware Volumes


## Modules

## Libraries


## Key Functions


## Configuration
* Set gBoardModuleTokenSpaceGuid.VpdPcdDTbtBoardConfig in BoardVpdPcdInit.dsc of each board for passing platform data.
* Set gUsb4FeaturePkgTokenSpaceGuid.PcdIoeDTbtRpSupport to reflect all possible dTBT Root Ports at IOE.
* Set gUsb4FeaturePkgTokenSpaceGuid.PcdPchDTbtRpSupport to reflect all possible dTBT Root Ports at PCH.
* Define macro in BoardPkg.dsc to overwrite the following dTBT Root Port ACPI Path components:
*   IOE_RP_PC: IOE Root Port PC segment. Default = PC00.
*   IOE_RP_PREFIX: IOE Root Port ACPI name prefix. Default = RP.
*   IOE_RP_PREFIX_NO_ZERO: For RP name with port number < 10, Zero will be inserted such as RP09.
*                          Define this macro if additional 0 is not desired for IOE RP. For example, PEG2.
*   IOE_RP_UPORT: Bridge Upstream Port ACPI name under IOE Root Port. Default = PXSX.
*   PCH_RP_PC: PCH Root Port PC segment. Default = PC02.
*   PCH_RP_PREFIX: PCH Root Port ACPI name prefix. Default = RP.
*   PCH_RP_PREFIX_NO_ZERO: For RP name with port number < 10, Zero will be inserted such as RP09.
*                          Define this macro if additional 0 is not desired for PCH RP.
*   PCH_RP_UPORT: Bridge Upstream Port ACPI name under PCH Root Port. Default = PXSX.
* Platform codes should set DTBT_CONTROLLER_CONFIG.RpAcpiNumber to reflect the numerical portion of dTBT RP ACPI name.
*   Example: dTBT RP ACPI name = RP09 => The associated DTBT_CONTROLLER_CONFIG.RpAcpiNumber should be set to 9.
*            dTBT RP ACPI name = PEG2 => The associated DTBT_CONTROLLER_CONFIG.RpAcpiNumber should be set to 2.

## Data Flows


## Control Flows

## Build Flows


## Test Point Results

## Functional Exit Criteria


## Feature Enabling Checklist

## Performance Impact

## Common Optimizations
