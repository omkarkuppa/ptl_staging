<!--
@file

 This file provide the details for UsbCCapsuleFeature.

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
* **Feature Name:** UsbCCapsuleFeature
* **PI Phase(s) Supported:** DXE
* **SMM Required?** No

## Purpose
To provide Capsule update function on UsbC firmwares.

# High-Level Theory of Operation
This package depends on FmpDevicePkg and produce different FmpDeviceLib library instances for different UsbC firmware type.

## Firmware Volumes
FvAdvanced

## Modules

## Libraries

   FmpDeviceLibTbtRetimer - Library instance to support UsbC Retimer Capsule update
   TbtNvmRetimerUpdateLib - Library serves FmpDeviceLibTbtRetimer to update Retimer NVM.

## Key Functions

gUsbCRetimerProtocolGuid - Platform need to install this protocol interface to driver/restore Retimer device to/from
                           FwUpdate mode for supporting Retimer FW update.

//
// Interface structure for the TBT retimer Protocol
//
struct _UsbC_RETIMER_PROTOCOL {
  UsbC_RETIMER_INFO                   GetRetimerInfo;   ///< Get UsbC Retimer Device info
  FW_UPDATE_MODE_ENTRY                Drive;            ///< Drive UsbC retimer device into FW Update mode.
  FW_UPDATE_MODE_EXIT                 Restore;          ///< Restore UsbC retimer device into original mode.
};


## Configuration

PcdUsbCPdNumber - Platform should configure the number of PD based on its board. layout.


## Data Flows
Architecturally defined data structures and flows for the feature.

## Control Flows

## Build Flows
No special tools are required to build this feature.

## Capsule Files Build Flows
Please refer to the ReadMe.txt under CapsuleBuild directory.

## Test Point Results
The FMP devices with below GUID will be installed and observed in ESRT table:
  DEFINE FMP_CLIENT_PLATFORM_TBT_RETIMER          = 2FE2CBFC-B9AA-4A93-AB5B-40173B581C42
  DEFINE FMP_CLIENT_PLATFORM_DISCRETE_TBT         = 86A885EE-D71E-2ED6-0FC1-9D6CCC9677EB

## Functional Exit Criteria
Capsule update is triggered successfuly and the version and status is correctly reported in ESRT.

## Feature Enabling Checklist
1. Retimer Capsule update:
   gUsbCRetimerProtocolGuid protocol interface needs to be installed by platform.

## Performance Impact
Minor impact. Most could happen in FmpDeviceGetVersion() to get FW version since it's invoked every POST.
If in the future Retimer FW version is read from Retimer FW during every POST, needs to reevalute performance impact.

## Common Optimizations
Common size or performance tuning options for this feature.

This section is recommended but not required. If not used, the contents should be left empty.
