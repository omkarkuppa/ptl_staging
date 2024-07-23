# Overview

The goal of the Hardware Security Testability Interface (HSTI) is to provide some level of assurance that the platform is properly and securely configured.  This assurance is done via the alignment of requirements between OS Vendor, Silicon Vendor, BIOS Vendor, ODM, and/or OEM.  It is imperative that each of these entities works in concert to provide a securely configured platform to the end user.
Additionally these test results will be used as an input to the Windows* Compatibility program and the results will be available both during certification as well as within the Operating System Event Log.

* Feature Name: Hardware Security Testability Interface (HSTI)
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This package publishes a driver to allow for caching of HSTI test results

# High-Level Theory of Operation

Feature currently consists of a single driver (HstiResultDxe.inf) which is responsible for caching the HSTI test results between boots. This allows the HSTI tests only when there are sgnificant changes to bios setup and on cold boot

## Modules

* HstiResultDxe.inf

## Configuration

There is no build-time configuration of the feature at the moment.

## Data flows

HstiResultDxe driver consumes MemoryPlatformData Hob and caches HSTI results in SiPolicyData.

## Build flows

No special tools are required to build this feature.