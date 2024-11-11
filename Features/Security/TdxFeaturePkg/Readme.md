# Overview

* Feature Name: Trusted Domain Extension (TDX)
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This feature consumes TdxDataHob produced from PeiTdx Lib and stores actm MAC into non-volatile memory

# High-Level Theory of Operation

Feature consists of a single driver (TdxDxeDriver.inf) which performs the purpose.

## Modules

* TdxDxeDriver.inf

## Configuration

This feature is dependent on gTdxFeaturePkgTokenSpaceGuid.PcdTdxFeatureEnable. This PCD needs to be set to "TRUE" for enabling this feature.

## Data flows

TdxDxeDriver creates EFI variable and stores actm MAC into it.

## Build flows

No special tools are required to build this feature.