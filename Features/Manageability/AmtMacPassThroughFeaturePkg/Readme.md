# Overview

* Feature Name: AmtMacPassThroughFeaturePkg
* PI Phase(s) Supported: DXE
* SMM Required: No

## Purpose

This package implements support for sending information about the onboard LAN MAC Address to AMT prior to OS boot.

# High-Level Theory of Operation

If Intel AMT is enabled in the system, BIOS shall send [MAC PassThrough command](#Amt-MAC-PassThrough-ASF-command) with current GBE MAC address to AMT before passing control to the OS.
This step is required each time the feature state changes or when the feature is enabled and GBE MAC changes.

For more information on ASF please refer to [Alert Standard Format specification](https://www.dmtf.org/standards/asf).

## Modules

* AmtMacPassThrough.inf

## Configuration

Set ```gAmtMacPassThroughFeaturePkgTokenSpaceGuid.PcdAmtMacPassThroughFeatureEnable``` to ```TRUE``` to enable AMT MAC PassThrough feature.

## Data flows

Before determining if AMT MAC PassThrough message shall be issued, BIOS shall compare the current feature settings with the ones from the previous boot. The current MAC Address should be determined by reading GBE region through SPI. These settings are held in [AMT MAC PassThrough Configuration EFI variable](#Amt-MAC-PassThrough-Configuration).
### Amt MAC PassThrough Configuration

Field Name                               | Description
---------------------------------------- | -----------
```Revision```                           | AMT MAC PassThrough Configuration Revision
```AmtMacPassThroughState```             | AMT MAC PassThrough feature state
```AmtMacPassThroughPreviousBootState``` | AMT MAC PassThrough Previous boot feature state
```MacBuffer[6]```                       | Buffer holding Integrated GBE MAC Address

### Amt MAC PassThrough ASF command

Field                  | Value      | Description
---------------------- | ---------- | --------
```ASF Command```      | ```0x0A``` | Set MAC PassThrough
```ASF Message Size``` | ```0x09``` | Number of bytes in ASF message beyond this byte
```ASF Subcommand```   | ```0x01``` | Set MAC PassThrough request
```ASF Version```      | ```0x10``` | Version number
```Feature State```    | ```N/A```  | Requested Feature State: </br> ```0``` -  Disabled;</br> ```1``` - Enabled
```MAC Address[6]```   | ```N/A```  | Integrated GBE MAC Address. Default MAC: </br> ```88:88:88:88:87:88```

</br>

## Build flows

No special tools are required to build this feature.

## Functional Exit Criteria

Once the feature is enabled on the platform, AMT MAC PassThrough HECI command shall be sent prior to OS boot.
This message shall be only issued when the feature state is changed, i.e **AmtMacPassThroughState** moved from ```Disabled``` -> ```Enabled``` or ```Disabled``` -> ```Enabled``` state.

## Feature Enabling Checklist

1. Add AmtMacPassThroughFeaturePkg.dsc entry and path to your build file and check if all required packages/libraries exists for this package.
2. Add AmtMacPassThroughFeaturePkg.fdf entry and path to your flash map file.
