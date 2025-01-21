/** @file
  Prototype of the NhltLib library.

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

#ifndef _NHLT_LIB_H_
#define _NHLT_LIB_H_

#include <Nhlt.h>

/**
  Returns pointer to Endpoint ENDPOINT_DESCRIPTOR structure.

  @param[in] *NhltTable    Endpoint for which Format address is retrieved
  @param[in] FormatIndex   Index of Format to be retrieved
  @param[in] NhltTableSize Nhlt table size

  @retval                  Pointer to ENDPOINT_DESCRIPTOR structure with given index
**/
ENDPOINT_DESCRIPTOR *
GetNhltEndpoint (
  IN CONST NHLT_ACPI_TABLE      *NhltTable,
  IN CONST UINT8                EndpointIndex,
  IN CONST UINTN                NhltTableSize
  );

/**
  Returns pointer to Endpoint Specific Configuration SPECIFIC_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which config address is retrieved

  @retval                  Pointer to SPECIFIC_CONFIG structure with endpoint's capabilities
**/
SPECIFIC_CONFIG *
GetNhltEndpointDeviceCapabilities (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to all Formats Configuration FORMATS_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which Formats address is retrieved

  @retval                  Pointer to FORMATS_CONFIG structure
**/
FORMATS_CONFIG *
GetNhltEndpointFormatsConfig (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to Format Configuration FORMAT_CONFIG structure.

  @param[in] *Endpoint     Endpoint for which Format address is retrieved
  @param[in] FormatIndex   Index of Format to be retrieved

  @retval                  Pointer to FORMAT_CONFIG structure with given index
**/
FORMAT_CONFIG *
GetNhltEndpointFormat (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint,
  IN CONST UINT8                FormatIndex
  );

/**
  Returns pointer to all Device Information DEVICES_INFO structure.

  @param[in] *Endpoint     Endpoint for which DevicesInfo address is retrieved

  @retval                  Pointer to DEVICES_INFO structure
**/
DEVICES_INFO *
GetNhltEndpointDevicesInfo (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Returns pointer to Device Information DEVICES_INFO structure.

  @param[in] *Endpoint       Endpoint for which Device Info address is retrieved
  @param[in] DeviceInfoIndex Index of Device Info to be retrieved

  @retval                    Pointer to DEVICE_INFO structure with given index
**/
DEVICE_INFO *
GetNhltEndpointDeviceInfo (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint,
  IN CONST UINT8                DeviceInfoIndex
  );

/**
  Returns pointer to OED Configuration SPECIFIC_CONFIG structure.

  @param[in] *NhltTable      NHLT table for which OED address is retrieved
  @param[in] *NhltTableSize  NHLT table size

  @retval                  Pointer to SPECIFIC_CONFIG structure with NHLT capabilities
**/
SPECIFIC_CONFIG *
GetNhltOedConfig (
  IN CONST NHLT_ACPI_TABLE      *NhltTable,
  IN CONST UINTN                NhltTableSize
  );

/**
  Prints Format configuration.

  @param[in] *Format       Format to be printed

  @retval None
**/
VOID
NhltFormatDump (
  IN CONST FORMAT_CONFIG        *Format
  );


/**
  Prints Endpoint configuration.

  @param[in] *Endpoint     Endpoint to be printed

  @retval None
**/
VOID
NhltEndpointDump (
  IN CONST ENDPOINT_DESCRIPTOR  *Endpoint
  );

/**
  Prints OED (Offload Engine Driver) configuration.

  @param[in] *OedConfig   OED to be printed

  @retval None
**/
VOID
NhltOedConfigDump (
  IN CONST SPECIFIC_CONFIG      *OedConfig
  );

/**
  Prints NHLT (Non HDA-Link Table) to be exposed via ACPI (aka. OED (Offload Engine Driver) Configuration Table).

  @param[in] *NhltTable    The NHLT table to print

  @retval None
**/
VOID
NhltAcpiTableDump (
  IN NHLT_ACPI_TABLE            *NhltTable
  );

#endif // _NHLT_LIB_H_
