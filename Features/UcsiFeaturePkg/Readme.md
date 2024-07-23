<!--
  @file

  This file provide the details for UcsiFeature.

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
* **Feature Name:** UcsiFeaturePkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To provide SSDT with ACPI Table to support universal USCI 1.2 & 2.0 declaration for supporting integrated TCSS and dTBT in AFP.

# High-Level Theory of Operation
This package Implements Ucsi Dxe driver and publish Ucsi Acpi table.

## Firmware Volumes
FvAdvanced

## Modules

## Key Functions
PublishAcpiTable
UcsiEntryPoint

## Control Flows
PcdUcsiFeatureEnable to enable this feature.

## Feature Enabling Checklist
PcdUcsiFeatureEnable enabled.


## Performance Impact

## Common Optimizations
