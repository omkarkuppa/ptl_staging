<!--
@file
 This file provide the details for One Click Recovery and Wifi Profile Sync
 feature.

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
* **Feature Name:** One Click Recovery
* **PI Phase(s) Supported:** BDS
* **SMM Required?** NO

## Purpose
* One Click Recovery: Provide a method driver to support Out of Band OS recovery, triggered from a remote administrator.
* WiFi Profile Sync: Supporting wireless LAN KVM and One Click Recovery WinRe, PBA, and HTTPS flows through an AMT profided WiFi profile. Install's gEdkiiWiFiProfileSyncProtocolGuid.

## Currently supported function:
- One Click Recovery:
  - Set SMBIOS table 130 with system One Click Recovery capability
  - Get recovery request type (WinRe, HTTP Boot, PBA, etc.)
  - Notify AMT of recovery flow
  - Get connection (LAN or WLAN) device path for recovery image
  - Clean boot options from system when system recovers
- Wifi Profile Sync:
  - Install gEdkiiWiFiProfileSyncProtocolGuid
  - Requests WiFi profile from AMT
  - Requests Certificate and Key from AMT
  - Requests Root CA Certificate from AMT
  - Converts profile data from AMT for WifiConnectionManager WLAN connection
  - Manages WiFi profile data from request to cleaning at Exti Boot Services

# High-Level Theory of Operation
1. OS corruption occurs
2. Remote Admin triggers OCR operation through MeshCommander
3. OCR checks SMBIOS table 130 for recovery capability
4. OCR sets boot priority to desired recovery flow
  4.1 Windows Recovery (WinRe)
  4.2 Pre-Boot Application (PBA)
  4.3 HTTPS Boot
5. OCR sets the recovery boot setup configuration
6. System reboots
7. WiFi Profile Sync driver loaded
  7.1. WiFiProfileSync driver will install gEdkiiWiFiProfileSyncProtocolGuid during DXE.
  7.2. WiFiProfileSync driver will check if boot guard and secure boot are enabled and if either in WinRe, HTTPS, PBA recovery or KVM is enabled.
  7.3. Driver will poll request WiFi profile name from AMT, if no name provided in 15 seconds then uninstall protocol and exit.
  7.4. Driver will request WiFi profile data using the profile name to identify the profile.
  7.5. Driver will request Root CA, Certificate and Key if WiFi profile data certificate avalability bit is set.
  7.6. Driver will store data in memory till exit boot services, where its callback will delete all data in memory.
  7.7. WifiConnectionManager Driver will check if gEdkiiWiFiProfileSyncProtocolGuid installed, if found it will request WiFi profile data.
  7.8. WiFiProfileSync driver will configure the profile data to WiFiConnectionManager driver profile sturcture and pass back data.
8. Recovery flow operates to access new OS and install
9. OCR uninstall OCR protocol
10. At Exit Boot Service WifiProfileSync driver callback will delete all profile data from memory and uninstall gEdkiiWiFiProfileSyncProtocolGuid.
11. Boots to OS

## Modules
* OneClickRecovery.inf
* WifiProfileSync.inf

### OneClickRecovery.inf
Provides ONE_CLICK_RECOVERY_PROTOCOL service for use.
  - ONE_CLICK_RECOVERY_ENTRY: The Intel One Click Recovery setup main function. This does the necessary work to setup the One Click Recovery feature. Called in BDS phase.
  - ONE_CLICK_RECOVERY_CAPABILITIES: Returns device One Click Recovery capabilities from SMBIOS table 130, to check for support types.
  - ONE_CLICK_RECOVERY_SAVE_UEFI_BOOT_OPTION: Saves OCR Uefi Boot Option from AMT. Needs to be called before Clear Boot Options.

### WifiProfileSync.inf
 Provide EDKII_WIFI_PROFILE_SYNC_PROTOCOL service for usage.
  - Get / Set Wifi Connection Status
    Use status for reconnection attempts and reporting connection status.
  - Get WiFi Profile
    WiFiConnectionManager uses this protocol function to get profile data for connection.

## Configuration
* Set gSiPkgTokenSpaceGuid.PcdAmtEnable to True
* gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable default True with a dependency on PcdAmtEnable True
* gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdWifiProfileSyncEnable enables WifiProfileSync feature to load
* gOneClickRecoveryFeaturePkgTokenSpaceGuid.PcdOneClickRecoveryEnable to True to set PcdWifiProfileSyncEnable to True

### Dependencies
* HTTP boot depends on gEfiRamDiskProtocol, therefore RamDiskDxe must be included for OCR HTTP Boot - along with the rest of the Network stack.

## Data Flows
* Update SMBIOS table 130 with One Click Recovery.
* Install ONE_CLICK_RECOVERY_PROTOCOL service for usage.
* Get recovery boot option from AMT.
* Add recovery boot option to the boot options menu.
* Install recovery boot option callback gEfiHttpBootCallbackProtocolGuid
  and gEdkiiHttpCallbackProtocolGuid
* Install EDKII_WIFI_PROFILE_SYNC_PROTOCOL service for usage.
* Update SMBIOS table 130 with One Click Recovery WLAN support from WiFi Profile Sync capabilities.
* Store and pass Wifi Profile Data from AMT to WifiConnectionManager for WLAN connection use.

## Control Flows
* SKU must be corporate and support AMT.
* BootGuard and Secure boot must be endabled.
* Must request special boot command for WinRe, PBA, or HTTPS recovery boot or find KVM enabled.

## Build flows
No special tools are required to build this feature.

## Feature Enabling Checklist
1. Add OneClickRecoveryFeature.dsc entry and path to your build file and check if all required packages/libraries exist for this package.
2. Add OneClickRecoveryFeature.fdf entry and path to your flash map file.

## Common Optimizations
* N/A

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p OneClickRecoveryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p OneClickRecoveryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p OneClickRecoveryFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose
