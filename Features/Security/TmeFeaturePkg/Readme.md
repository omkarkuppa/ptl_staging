# Overview

* Feature Name: Total Memory Encription (TME)
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This feature updates UEFI memory map to add EFI_MEMORY_CPU_CRYPTO attribute for all system memory.
It also measures and logs data for TME state and configuration.

# High-Level Theory of Operation

Feature consists of a single driver (TmeInitDxe.inf) which performs the purpose.

## Modules

* TmeInitDxe.inf

## Configuration

This feature is dependent on gTmeFeaturePkgTokenSpaceGuid.PcdTmeFeatureEnable. This PCD needs to be set to "TRUE" for enabling this feature.

## Data flows

Register a callback to measure and log TME if TME and TPM are supported.

## Build flows

No special tools are required to build this feature.