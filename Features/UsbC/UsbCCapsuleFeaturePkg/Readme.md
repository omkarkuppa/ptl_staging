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

   FmpDeviceLibTbtRetimer  - Library instance to support UsbC Retimer Capsule update
   FmpDeviceLibDiscreteTbt - Library instance to support Discrete TBT update
   FmpDeviceLibPdBridge    - Library instance to support PD Bridge update
   TbtNvmRetimerUpdateLib  - Library serves FmpDeviceLibTbtRetimer to update Retimer NVM.
   UsbCPdBridgeUpdateLib   - Library serves FmpDeviceLibPdBridge to update PD Bridge NVM.
   UsbcCapsuleDebugLib     - Library support UsbC Capsule diagnostic tool in release BIOS.


## Key Functions

gUsbCRetimerProtocolGuid - Platform need to install this protocol interface to driver/restore Retimer device to/from
                           FwUpdate mode for supporting Retimer FW update.

//
// Interface structure for the UsbC retimer Protocol
//
struct _UsbC_RETIMER_PROTOCOL {
  GET_PD_CONTROLLER_MODE   GetPdControllerMode;   ///  Get PD controller Fw Update Mode.
  SET_PD_CONTROLLER_MODE   SetPdControllerMode;   ///  Set PD controller Fw Update Mode.
  CONTROL_RETIMER_FP_GPIO  RetimerFP;             ///  Control Retimer FP GPIO
};

gUsbCPdBridgeProtocolGuid                 - Platform need to install this protocol interface to support PD Bridge capsule update

///
/// Interface structure for the PD Bridge Protocol
///
struct _USBC_PD_BRIDGE_PROTOCOL {
  UINT64                 Revision;          ///< Revision for the protocol structure
  GET_PD_BRIDGE_VERSION  GetVersion;        ///< Get PD Bridge version
  LOCK_COMMUNICATION     Lock;              ///< Lock/Unlock EC-PD regular communication
  EXECUTE_VENDOR_CMD     ExecuteVendorCmd;  ///< Execute the PD Vendor Command via EC private port
};

gUsbCCapsuleDebugProtocolGuid             - Support UsbC Capsule diagnostic tool in release BIOS

///
/// Data structure of Capsule Debug Protocol
///
struct _USBC_CAPSULE_DEBUG_PROTOCOL {
  CAPSULE_LOG_BUFFER               CapsuleLogBuf;         ///< Log buffer for Log data
  const CAPSULE_LOG_MAPPING_ENTRY  *LogMappingTable;      ///< Mapping table for converting log data to log string
  UINT32                           LogMappingEntries;     ///< The number of mapping entries in Log mapping table
  UINT32                           DebugLevel;            ///< Debug level used for controlling what level of Log data can be saved in Log buffer
  LOG_WRITE                        LogWrite;              ///< Write the log data to Log buffer
  LOG_READ                         LogRead;               ///< Read the log data from Log buffer
  LOG_PARSE                        LogParse;              ///< Parse the log data to produce Log string
  LOG_READ_RESET                   LogReadReset;          ///< Reset the log data reading index to the first log entry
} ;

gUsbCCapsuleDebugProgressCodeProtocolGuid - Show UsbCCapsuleFeaturePkg related progress code.
                                            The detail of status code format define in UsbCProgressCodeProtocol.h

struct _USBC_PROGRESS_CODE_PROTOCOL {
  SHOW_PROGRESS_CODE  ShowProgressCode;  ///  Show Capsule Debug Progress Code
};


## Configuration

PcdMaxUsbCPdNumber                - The Max number of PD supported
PcdUsbCRetimerFlashNumber         - The number of retimer flash part from the configuration table
PcdUsbCCapabilityParsingTimeDelay - Time delay to handle TBT controller responding after first read register attempt
PcdTcssWaitRetimerDeviceReady     - Time delay to make sure retimer is ready for communication after Drive to TBT mode.
PcdUsbCRetimerConfigTable         - The table of retimer addresses configuration
PcdUsbCCapsuleDebugLevel          - To determine what level of debug message needs to be stored via

## Data Flows
Architecturally defined data structures and flows for the feature.

## Control Flows

## Build Flows
No special tools are required to build this feature.

## Capsule Files Build Flows
Please refer to the ReadMe.txt under UsbCCapsuleFileBuildSample directory.

## Test Point Results
The FMP devices with below GUID will be installed and observed in ESRT table:
  DEFINE FMP_CLIENT_PLATFORM_TBT_RETIMER          = 2FE2CBFC-B9AA-4A93-AB5B-40173B581C42
  DEFINE FMP_CLIENT_PLATFORM_DISCRETE_TBT         = 86A885EE-D71E-2ED6-0FC1-9D6CCC9677EB
  DEFINE FMP_CLIENT_PLATFORM_PD_BRIDGE            = 11984C7E-1207-4DE0-92C5-29661F9A6557

## Functional Exit Criteria
Capsule update is triggered successfully and the version and status is correctly reported in ESRT.

## Feature Enabling Checklist
1. Retimer Capsule update:
   gUsbCRetimerProtocolGuid protocol interface needs to be installed by platform.
2. PD Bridge Capsule update:
   gUsbCPdBridgeProtocolGuid protocol interface needs to be installed by platform.

## Performance Impact
Minor impact. Most could happen in FmpDeviceGetVersion() to get FW version since it's invoked every POST.
If in the future Retimer FW version is read from Retimer FW during every POST, needs to reevaluate performance impact.

## Common Optimizations
Common size or performance tuning options for this feature.

This section is recommended but not required. If not used, the contents should be left empty.
