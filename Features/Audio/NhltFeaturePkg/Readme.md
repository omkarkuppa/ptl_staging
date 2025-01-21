# Overview

* Feature Name: Nhlt platform driver - Non HD Audio Link Table. This is an ACPI table containing non-HD Audio endpoints and links configuration.
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This feature enable flows for creating and installing Nhlt (Non HD Audio Link Table) configuration for integrated DSP on Intel platforms.

Currently supported use cases:
* Driver strength selection
* Add selected group of endpoints to Nhlt configuration
* Install Nhlt configurations under XSDT Acpi table

# High-Level Theory of Operation

Feature consists of a single driver(NhltInstallTable.inf) which is responsible for installing the Nhlt table under XSDT Acpi table.
Based on selected options in EFI variable NHLT\_ENDPOINTS\_TABLE\_CONFIGURATION\_VARIABLE corresponding group of endpoints is added
to Nhlt configuration. After publication of Nhlt configuration driver(NhltInstallTable.inf), looks for installed configuration in XSDT
and validates NHLT configruation.

The NhltInstallTable.inf entry point has to be executed before drivers start binding to the hardware. It is board responsibility to place
them in flash in a way that meets this requirement.

## Modules

* NhltInstallTable.inf

## Configuration

Set gNhltFeaturePkgTokenSpaceGuid.PcdNhltFeatureEnable to false to disable all of the Nhlt modules.

## Data flows

Board can pass endpoints configuration to the feature using below EFI variable:

| Type                                                | Variable name                             | Variable GUID                       |
|-----------------------------------------------------|-------------------------------------------|-------------------------------------|
| NHLT\_ENDPOINTS\_TABLE\_CONFIGURATION\_VARIABLE     | NhltEndpointsTableConfigurationVariable   | A1D89A3A-4A90-429D-43651F64C3A29614 |

Board can publish endpoints configuration under one of selected guids below in FV. For enabling/disabling specific configuration board should use
NhltEndpointsTableConfigurationVariable and enable/disable appropriate number of configuration in NhltConfigurationEnabled array.

| Number of configuration | Configuration binary GUID in FV      |   Default    |
|-------------------------|--------------------------------------|--------------|
|            0            | 06ADC9B7-837F-47DF-AC0C-A0EE18BFAD67 |   Enabled    |
|            1            | 54067565-B069-4260-87AC-416AC8159BD5 |   Disabled   |
|            2            | 729D003A-0125-4489-B2EC-00415C88EFEB |   Disabled   |
|            3            | 40801B42-4B8E-4CFC-90F6-8AAAEA82607C |   Disabled   |
|            4            | 5F845818-7D38-4EBF-B410-922B8CE825BB |   Disabled   |
|            5            | 27BDBE6F-4C7A-48AA-9289-4C5A2ECA28D7 |   Disabled   |
|            6            | F1722F56-F045-4CA6-A84F-6A7CA1A14C8C |   Disabled   |
|            7            | 0046A325-B87B-4DD3-8A6F-6ABACC9EFD5A |   Disabled   |
|            8            | 4D77B5D2-A3B5-4ABE-BF70-F982B2BB2EED |   Disabled   |
|            9            | 9E83AE47-0DB1-4BE0-9478-6E86366D5EA4 |   Disabled   |
|           10            | CE49BFC7-EEBF-49B9-B5D0-94A771659B00 |   Disabled   |
|           11            | 7DA9FE6F-0AA9-443C-9CA8-231EBBC92B4B |   Disabled   |
|           12            | DCE94204-618E-4289-B97A-00CF43FDB29A |   Disabled   |
|           13            | 01A63902-0EFA-4AFF-83A0-FC1B909ECDDF |   Disabled   |
|           14            | C7227C60-F6FC-48EE-A148-19A4D065F47D |   Disabled   |
|           15            | 4EC0C41E-40D5-4760-AB08-11EFC717BAAE |   Disabled   |

If NhltEndpointsTableConfigurationVariable EFI variable was not installed NhltInstallTable driver load defaults in LoadNhltConfigurationDefaults () from PCD (NhltConfigurationsEnabledDefaults). User can override Nhlt configuration settings using NhltConfigurationsEnabledDefaults.

## Build flows

Nothing special tools are required to build this feature.

## Feature Enabling Checklist

1. Add NhltFeature.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add PostMemory.fdf entry and path to your flash map file.
3. Publish up to 16 endpoints configuration in your flash map and assign correct guids
4. Install NhltEndpointsTableConfigurationVariable and select proper configuration.
5. Set gNhltFeaturePkgTokenSpaceGuid.PcdNhltFeatureEnable to true.