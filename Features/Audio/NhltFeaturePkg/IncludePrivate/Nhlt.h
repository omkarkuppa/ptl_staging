/** @file
  NHLT structure definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#ifndef _NHLT_H_
#define _NHLT_H_

#include <IndustryStandard/Acpi.h>

//
// ACPI support protocol instance signature definition.
//
#define NHLT_ACPI_TABLE_SIGNATURE  SIGNATURE_32 ('N', 'H', 'L', 'T')

typedef enum {
  NhltDmicX1Format24Bit48kHzFreq38_4MHz     = 0,
  NhltDmicX1Format24Bit96kHzFreq38_4MHz     = 1,
  NhltDmicX2Format24Bit48kHzFreq19_2MHzVP   = 2,
  NhltDmicX2Format24Bit48kHzFreq38_4MHz     = 3,
  NhltDmicX2Format24Bit96kHzFreq38_4MHz     = 4,
  NhltDmicX4Format24Bit48kHzFreq38_4MHz     = 5,
  NhltDmicX4Format24Bit96kHzFreq38_4MHz     = 6,
  NhltI2sAlc274Format24Bit48kHzFreq38_4MHz  = 7,
  NhltI2sAlc274Format24Bit96kHzFreq38_4MHz  = 8,
  NhltBtFreq38_4MHzBtDrivesI2sClockHfp      = 9,
  NhltBtFreq38_4MHzDspDrivesI2sClockHfp     = 10,
  NhltConfigurationMax
} NHLT_CONFIGURATIONS;

typedef enum {
  NhltClockSourcePlatformPor = 0,
  NhltClockSource24MHz       = 1,
  NhltClockSource24_576MHz   = 2,
  NhltClockSource38_4MHz     = 3,
  NhltClockSourceInvalid
} NHLT_CLOCK_SOURCE;

typedef enum {
  NhltDmicDisabled                      = 0x0,
  NhltDmicClock38_4MHz_Format24b_48kHz  = 0x1,
  NhltDmicClock38_4MHz_Format24b_96kHz  = 0x2,
  NhltDmicFormatInvalid
} NHLT_DMIC_FORMAT_OPTIONS;

typedef enum {
  NhltBtDisabled              = 0x0,
  NhltBtClockDrivenByBtInHfp  = 0x1,
  NhltBtClockDrivenByDspInHfp = 0x2,
  NhltBtFormatInvalid
} NHLT_BT_FORMAT_OPTIONS;

typedef enum {
  NhltI2sDisabled                      = 0x0,
  NhltI2sClock38_4MHz_Format24b_48kHz  = 0x1,
  NhltI2sClock38_4MHz_Format24b_96kHz  = 0x2,
  NhltI2sFormatInvalid
} NHLT_I2S_FORMAT_OPTIONS;

#pragma pack (push, 1)

typedef struct {
  UINT16  wFormatTag;
  UINT16  nChannels;
  UINT32  nSamplesPerSec;
  UINT32  nAvgBytesPerSec;
  UINT16  nBlockAlign;
  UINT16  wBitsPerSample;
  UINT16  cbSize;
} WAVEFORMATEX;

typedef struct {
  WAVEFORMATEX Format;
  union {
    UINT16 wValidBitsPerSample;
    UINT16 wSamplesPerBlock;
    UINT16 wReserved;
  } Samples;
  UINT32       dwChannelMask;
  GUID         SubFormat;
} WAVEFORMATEXTENSIBLE;

typedef struct {
  UINT32    CapabilitiesSize;
  UINT8     Capabilities[1];
} SPECIFIC_CONFIG;

typedef struct {
  WAVEFORMATEXTENSIBLE Format;
  SPECIFIC_CONFIG      FormatConfiguration;
} FORMAT_CONFIG;

typedef struct {
  UINT8           FormatsCount;
  FORMAT_CONFIG   FormatsConfiguration[1];
} FORMATS_CONFIG;

typedef struct {
  UINT8           DeviceId[16];
  UINT8           DeviceInstanceId;
  UINT8           DevicePortId;
} DEVICE_INFO;

typedef struct {
  UINT8           DeviceInfoCount;
  DEVICE_INFO     DeviceInformation[1];
} DEVICES_INFO;

typedef struct {
  UINT32          EndpointDescriptorLength;
  UINT8           LinkType;
  UINT8           InstanceId;
  UINT16          HwVendorId;
  UINT16          HwDeviceId;
  UINT16          HwRevisionId;
  UINT32          HwSubsystemId;
  UINT8           DeviceType;
  UINT8           Direction;
  UINT8           VirtualBusId;
  SPECIFIC_CONFIG EndpointConfig;
  FORMATS_CONFIG  FormatsConfig;
  DEVICES_INFO    DevicesInformation;
} ENDPOINT_DESCRIPTOR;

//
// High Level Table structure
//
typedef struct {
  EFI_ACPI_DESCRIPTION_HEADER Header;
  UINT8                       EndpointCount;   // Actual number of endpoints
  ENDPOINT_DESCRIPTOR         EndpointDescriptors[1];
  SPECIFIC_CONFIG             OedConfiguration;
} NHLT_ACPI_TABLE;

#pragma pack (pop)

#endif // _DXE_PCH_HDA_NHLT_H_
