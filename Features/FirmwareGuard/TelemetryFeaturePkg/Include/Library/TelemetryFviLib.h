/** @file
  Telemetry API Lib for Firmware Information Version.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _TELEMETRY_LIB_H_
#define _TELEMETRY_LIB_H_

/**
  Append an entry after Fvi AIP table.

  @param[in] ComponentId      ComponentId for the Fvi entry.
  @param[in] Version          Version for Fvi entry.
  @param[in] ProducerId       ProducerId for Fvi entry, e.g. 'INTC'.

  @retval EFI_SUCCESS           - if the data is successfully reported.
  @retval EFI_OUT_OF_RESOURCES  - if not able to get resources.
  @retval EFI_INVALID_PARAMETER - if Data == NULL.
**/
EFI_STATUS
EFIAPI
AppendTelemetryFviBlock (
  IN     EFI_GUID                            ComponentId,
  IN     UINT64                              Version,
  IN     UINT32                              ProducerId
  );

#endif
