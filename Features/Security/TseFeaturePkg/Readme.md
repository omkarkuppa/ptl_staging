# Overview

* Feature Name: Total Storage Encription (TSE)
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This feature implementes the NvmExpress UEFI driver with TSE support.

# High-Level Theory of Operation

Feature consists of a single driver (TseNvmExpressDxe.inf) which add BlockIoCrypto protocol and TSE support to the EDK NvmExpressDxe implementation.

## Modules

* TseNvmExpressDxe.inf

## Configuration

The gSiPkgTokenSpaceGuid.PcdTseEnable is defined in order to include or exclude the TSE definitions.

## Data flows

The TseNvmExpressDxe driver consumes the TseProtocol defined by the TSE IP block.

## Build flows

No special tools are required to build this feature.