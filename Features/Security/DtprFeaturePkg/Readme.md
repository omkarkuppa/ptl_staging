# Overview

* Feature Name: DMA TXT Protected Range (DTPR) ACPI driver
* PI Phase(s) Supported: DXE
* SMM Required? No

## Purpose

This feature installs ACPI driver for DTPR

# High-Level Theory of Operation

Feature consists of a single driver(DtprDxe.inf) which is responsible for installing ACPI driver which reports DTPR table by consuming information provided by gTprInfoHob.

## Modules

* DtprDxe.inf

## Configuration

There is no build-time configuration of the feature at the moment.

## Data flows

DtprDxe driver consumes gTprInfoHob and publishes DTPR ACPI table which is consumed by OS.

## Build flows

No special tools are required to build this feature.