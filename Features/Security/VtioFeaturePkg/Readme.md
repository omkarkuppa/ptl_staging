# Overview

Secure Biometric feature is an end to end system level solution for user authentication using biometric data in Windows Operating System.
This feature gives the capability to authorized user to login to windows machine using biometrics data instead of using username and password.
This feature relies on Intel Virtualization hardware (Intel VT and VT-d) to offer Virtualization based Trusted IO (VTIO).

This package is part of BIOS side of Secure Biometrics feature called Virtualized IO (VTIO). The role of this package is to inform OS level
drivers about secure Biometrics sensors (camera or fingerprint) through Secure Device (SDev) ACPI table. The OS drivers use this information
to interact with secure devices and capture Biometric data. This data is then compared with reference symbol to authenticate the user.

* Feature Name: Secure Biometric's Virtualized IO (VTIO) driver
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This driver is used for providing Biometric sensor related information to OS driver through Secure Device (SDev) ACPI table.
SDev table has following format:
        Field                   Byte Length        Byte Offset                  Description
|------------------------|---------------------|----------------------|-------------------------------------|
       Header
1.     Signature               4                       0                 'SDEV'. Signature of VTIO table
2.     Length                  4                       4                 Length, in bytes, of the entire VTIO table
3.     Revision                1                       8
4.     Checksum                1                       9                 Entire table must sum to zero
5.     OEM ID                  6                      10                 OEM ID
6.     OEM Table ID            8                      16                 Table ID is manufacture model ID
7.     OEM Revision            4                      24                 OEM revision of VTIO table for supplied OEM table ID
8.     Creator ID              4                      28                 Vendor ID of utility that created the table
9.     Creator Revision        4                      32                 Revision of utility that created the table
10.    Secure Device           -                      36                 A list of structures containing one of more VTIO
       Structures []                                                     structures

The Hardware device information is part of Secure Device structures field in SDEV ACPI table. There are two types of devices supported by SDEV table.
One PCIe based end point device and other is ACPI namespace device.
More information about secure device structure can be found in VTIO BIOS specification.
# High-Level Theory of Operation

VTIO feature consists of two drivers. First driver is called as VtioDxe and other is driver called VtioSetup.
VtioSetup populates VTIO_SETUP_DATA_STRUCT structure by reading BIOS setup menu options. This structure is later passed on to
VtioDxe driver using PCD gVtioFeaturePkgTokenSpaceGuid.PcdVtioSetupDataPtr. VtioDxe driver reads this PCD and creates SDev ACPI table
accordingly. SDev ACPI table is latter consumed by OS level VTIO drivers.

## Modules

* VtioDxe.inf
* VtioSetup.inf

## Configuration

This feature is dependent on gVtioFeaturePkgTokenSpaceGuid.PcdVtioFeatureEnable. This PCD needs to be set to "TRUE" for enabling this feature.
User also has an option to populate devices into Sdev ACPI table through BIOS menu. Configuration details of the sensors connected to the platform can be entered here.

## Data flows

VtioSetup driver's callback function creates EFI variable (if not already created) or retrieve the variable from EFI.
Any changes in VTIO configuration in BIOS menu is retrieved through HiiProtocol and stored back into EFI variable. This variable is passed on to VtioDxe driver through PCD.
VtioDxe driver consumes this variable and creates Sdev ACPI table and publishes it.

## Build flows

No special tools are required to build this feature.

## Feature Enabling Checklist

1. Add VtioFeature.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add VtioFeature.fdf entry and path to your flash map file.
3. Go to BIOS menu and configure secure devices entries according to device connected on platform.
4. Set gVtioFeaturePkgTokenSpaceGuid.PcdVtioFeatureEnable to true.