<!--
  @file

  This file provide the details for Usb Port Map.

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
* **Feature Name:** UsbPortMapPkg
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To provide SSDT with ACPI Table to support UPC PLD for supporting integrated TCSS and dTBT in AFP.

# High-Level Theory of Operation
This package Implements UsbPortMap Dxe driver and publish UsbPortMap Acpi table.

## Firmware Volumes
FvAdvanced

## Modules

## Key Functions
PublishAcpiTable
UsbPortMapEntryPoint

## Control Flows
PcdUsbPortMapEnable to enable this feature.

## Feature Enabling Checklist
PcdUsbPortMapEnable enabled.


## Performance Impact

## Common Optimizations
