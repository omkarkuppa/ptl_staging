<!--
@file
 This file provide the details for Platform Sanitization feature.

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
* **Feature Name:** Platform Sanitization Feature
* **PI Phase(s) Supported:** DXE
* **SMM Required?** NO

## Purpose
This feature package implements platform sanitization support which sanitize Hard drivers, TPM, NVM Variable region,
Resetting BIOS & CSME Configuration and also provides Hook for OEM to consider OEM specific sanitization.

## High-Level Theory of Operation
This feature package supports to sanitize the platform by triggering from Local BIOS setup menu (LPE) as well as
triggering from remote access (RPE).

Default this feature gets added in the BIOS image and supports to all client platforms.
Remote Trigger Capability (RPE) is depending with AMT and controlled by Brand Identity. Hence this feature will be supported
only when AMT is enabled and Brand Identity is set to allow this feature support.

In the BIOS Setup, this feature available under Intel Advanced Menu --> Manageability --> With the naming "Platform Sanitization".
Under "Platform Sanitization": Below options given to configure
  - Remote Platform Erase (RPE) Support:
    Default enabled. Which notifies CSME and remote admin tool about RPE support. Erase will takes place only when Triggered.
    Remote Admin triggers the sanitization remotely by accessing the target system.
    This option is not shown when AMT is disabled.

  - Local Platform Erase (LPE) Configuration:
    This configuration have Enable/Disable of LPE, and options to select all supported erase actions.
    Default LPE is disabled. On enabling it, will perform erase actions on the next boot.

  - Storage Erase Configuration:
    This provides selection of storage devices, and erase methods to sanitize as per NIST 800-88R1 Standard.

  - Simulation Mode:
    Enabling this mode will make the BIOS flow till attempting the erase action but returns with Success without
    peforming any erase. This is to verify the path from trigger location till reaching the erase action.

Local Platform Erase (LPE):
  This support is available across all client platforms. When LPE is enabled and system is restarted, then system enters into
  sanitize flow and get the selected erase options from setup and peform one by one as per order.

Remote Platform Erase (RPE):
  When remote admin access the target machine and triggered RPE sanitize and restart the system. Then one the next boot, BIOS get
  to know the RPE trigger from CSME-AMT and also gets teh list of selected erase options from CSME-AMT and perform one by one
  as per order.
  PET Alerts will be send to AMT for sanitize updates.

SMBIOS 130 update:
  SMBIOS Table 130 is updated with Platform Sanitize capaibilities (PcdPsSupportCapabilities)

Notes:
 1. All the Sanitize setup configuration must be controlled with Admin Privileges. (Intel BIOS do not differentiate between
    Admin and User but OEM must take a note to change the settings with Admin Privileges). Similarly remote access and remote
    sanitize must be triggered with remote Admin privileges.
 2. Storage Erase configuration and Simulation applies to both LPE & RPE.
 3. Only the trigger method is different between RPE & LPE. After the list of erase options are recived then rest of the erase
    flow is common between both LPE & RPL.
 4. The overall sanitize summary is capture and send to CSME to record in PSR for both LPE & RPE.

## Modules
* PlatformSanitizeDxe
* PlatformSanitizeSetupDxe
* StorageSanitize
  - StorageSanitizeDxe
  - PlatformOpalDxe
* PsBoardOemDxe
* MeSmbiosUpdateConfigDxe

## PlatformSanitizeDxe
  This is the main driver which checks for RPE support and enables it. On every boot, it checks whether LPE/RPE is triggered.
  If yes, then gets boot parameter and perform erase actions as per order.
  This module depends on all other sanitize modules & libraries to check for LPE/RPE boot and get boot parameters and to perform
  sanitization.

## PlatformSanitizeSetupDxe
  This driver provides all the setup interfaces for both LPE & RPE.

## StorageSanitize
  StorageSanitizeDxe - Provide Storage erase support for all storage device types. This driver takes action when platform
              sanitize triggered with Storage erase selection.

  PlatformOpalDxe - Provide PLATFORM_OPAL_PROTOCOL service for storage which support SecurityCommandProtocol.
            - PerformOpalRevert to perform Revert action
            - GetBlockSidState to get BlockSid state
            - GetState to get Opal state: support / enabled / locked.

## PsBoardOemDxe
  This is Board and OEM specific driver. This module install protocols to perform platform specific erase actions such as
  BIOS setup configurations, provision to add platform specific Keys/certificates in case if erase all NVM Variable is selected.
  This module also provides OEM Hooks, where OEM can have their own sanitize implementation in these functions.
  Platform sanitize driver locates these protocols and perform Platform, OEM specific erase actions when selected for sanitize.

## MeSmbiosUpdateConfigDxe
  This is Board package SMBIOS Driver. This driver is being used to update SMBIOS Table Type 130. Platform Sanitize feature
  makes use of this driver to update RPE Capabilities at SMBIOS Table Type 130.

## AsfTable
  This is AMT AsfTable Dxe driver, Platform Sanitize feature makes use of this driver to update RemoteControlCapabilities with
  CONFIG_DATA_RESET. Setting of this bit is referred to trigger CSME Unconfiguration.

## Libraries
* PsCsmeAccessLib - CSME access library, this is used by both RPE & LPE.
* PsCsmeAccessRpeLib - CSME access library specific to RPE boot. This is used only when RPE is supported.
* PsLpeLib - LPE specific library.
* PsStorageLib - Storage support library which interactsion with Storage driver module.
* SanitizeUiLib - Storage support User Interface library.
* DxeAsfLib    - This library used to get CONFIG_DATA_RESET Trigger notification to perform CSME Unconfiguration.

## Configuration
PcdPlatformSanitizeFeatureEnable    - This flag controls the entire platform Sanitize feature.
PcdPlatformSanitizeRpeSupport - This flag controls only RPE support. This flag is valid only if PcdPlatformSanitizeFeatureEnable is TRUE.

PcdPsSupportCapabilities      - This flag is used to set the sanitize support capability by the platform.
  Below each bit position representing the capability support
   Bit 0     - Remote Platform Erase (RPE) support
   Bit 1     - Reserved
   Bit 2     - Erase Storage Media as per NIST 800-88R1 spec.
   Bit 3     - Verify Storage Media after erasing them. (This bit is valid only if bit#2 is set)
   Bit 4     - Generate Sanitize Report (This bit is valid only if bit#2 is set)
   Bit 5     - Reserved
   Bit 6     - TPE Clear
   Bit 15:7  - Reserved
   Bit 16    - OEM Custom Erase Action
   Bit 20:17 - Reserved for OEM Custom Erase Actions
   Bit 24:21 - Reserved
   Bit 25    - Clear All BIOS NVM Variable Region
   Bit 26    - BIOS reload to Golden Config
   Bit 30:27 - Reserved
   Bit 31    - CSME Unconfiguration

PcdPsSetupConfiguration       - This flag is to determine formset display location,
  The definition of each field is listed below:
   Bit 15:0  - The id of form to display formset under Advanced menu
   Bit 23:16 - Formset display order
   Bit 30:24 - Reserved
   Bit 31    - Whether to show formset, 1 - show, 0 - hide

PcdPsSimulationSupport         - This flag enables the simulation support for sanitization. This is used to verify the path
  between triggered source (BIOS setup in case of LPE, Remote UI incase of RPE) and actual erase action to take place.
  Intel BIOS set this flag for Internal validation purpose.
  For external, in case if OEM set this flag for their internal validation then must need to make sure to set to FALSE to
  avoid getting included after End Of Manufacturing.

PcdRpeOpalDriverEnable       - This is dynamic setting flag, set to TRUE while entering into sanitize flow to support
    PlatformOpalDxe Driver binding protocol.

## Data Flows
* N/A

## Control Flows
  RPE Flow:
  Remote User Interface --> CSME-AMT --> BIOS Platform Sanitize DXE --> Erase specific Driver --> Perform Erase.

  LPE Flow:
  BIOS Setup User Interface --> BIOS Platform Sanitize DXE --> Erase specific Driver --> Perform Erase.

## Build flows
|   OS    | Compiler |
|:-------:|:--------:|
| Windows | VS2019   |

> Step1. Prepare build environment with VS2019 as needed.
> Step2. Install the pip requirements.
         Sample : pip install -r Edk2/pip-requirements.txt
> Step3. Setup Edk2 environment.
         Sample : Edk2/edksetup.bat Rebuild
> Step4. Get the code dependencies.
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t DEBUG,RELEASE -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose

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
         Sample : stuart_setup -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step5. Update other dependencies.
         Sample : stuart_update -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019
> Step6. Build unit test host.
         Sample : stuart_ci_build -c Intel/.pytool/CISettings.py -p PlatformSanitizeFeaturePkg -t NOOPT -a IA32,X64 TOOL_CHAIN_TAG=VS2019 --verbose