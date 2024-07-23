# Overview

* Feature Name: SndwInstallDevTopology - The SoundWire (Sndw) Manager IP is a soft controller IP with configurable parameters that can be tailored to support audio streaming applications over the SoundWire interface on Intel platforms.
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This feature allows to install SSDT ACPI table which contains information about codecs and endpoints topology. For more information about SoundWire interface and, see [MIPI DisCo for SoundWire](https://www.mipi.org/specifications/mipi-disco-soundwire).

Currently supported use cases:

* Driver strength selection
* Based on user input install SSDT ACPI table which contains selected devices and endpoints topology.

## High-Level Theory of Operation

Feature package contains:

* Drivers
  * **SndwInstallDevTopology**  (*SndwInstallDevTopology.inf*) - This driver is responsible for installing the SSDT table based on selected configuration in SndwDevTopologyConfigurationVariable EFI Variable. Driver based on HDA device location update device scope in installed SSDT.
* Acpi Tables
  * **SndwDevTopologySsdt**  (*SndwDevTopologySsdt.inf*) - Module contain SSDT tables for different topology configurations:
    * SndwDevTopologySt00Ssdt - SSDT contains DisCo for legacy codecs topology.
    * SndwDevTopologySt01Ssdt - SSDT contains DisCo for ALC711-VD1, ALC714-VC1, 2x ALC316 and SSID: 10EC3056.
    * SndwDevTopologySt02Ssdt - SSDT contains DisCo for ALC712-VB, ALC1320, 5-Star Configuration.
    * SndwDevTopologySt03Ssdt - SSDT contains DisCo for ALC713-VB, 2xALC1320, 4-Star(3) Configuration.
    * SndwDevTopologySt04Ssdt - SSDT contains DisCo for ALC722-CG, 4-Star(1) Configuration.
    * SndwDevTopologySt05Ssdt - SSDT contains DisCo for Cirrus 6-speaker configuration

## Modules
* SndwInstallDevTopology.inf
* SndwDevTopologySsdt.inf

## Configuration

Set ```gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyFeatureEnable``` to ```TRUE``` to enable SoundWire Device and Endpoints Topology Feature.</br>
SndwInstallDevTopology is configured by EFI Variable: SndwDevTopologyConfigurationVariable. SndwDevTopologyConfigurationVariable.SndwDevTopologyConfigurationNumber is used to select the number of SoundWire device topology.

Currently supported devices and endpoints topology:

1. [0] - Legacy codecs topology
1. [1] - Configuration 10EC3056 ALC711-VD1, ALC714-VC1, 2x ALC316
1. [2] - ALC712-VB, ALC1320, 5-Star Configuration
1. [3] - ALC713-VB, 2xALC1320, 4-Star(3) Configuration
1. [4] - ALC722-CG, 4-Star(1) Configuration
1. [5] - Cirrus 6-speaker configuration

</br>

## Build flows

No special tools are required to build this feature.

## Feature Enabling Checklist

1. Add *SndwDevTopologyFeature.dsc* entry, check if all required packages and libraries are available.
1. Add *SndwDevTopologyFeaturePostMem.fdf* to flash map file.
1. Install SndwDevTopologyConfigurationVariable and select proper configuration.
1. Set gSndwDevTopologyFeaturePkgTokenSpaceGuid.PcdSndwDevTopologyFeatureEnable to true.
