/** @file
  FspRsc2PostCodeMap PPI prototypes

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _FSP_RSC2POSTCODE_MAP_SERVICE_PPI_H_
#define _FSP_RSC2POSTCODE_MAP_SERVICE_PPI_H_

#include <Pi/PiStatusCode.h>


/**
  Prints a catalog debug message to the TH Debug device if the specified error level is enabled.

  If any bit in ErrorLevel is also set in DebugPrintErrorLevelLib function
  GetDebugPrintErrorLevel (), then print the message specified by Format and the
  associated variable argument list to the debug output device.

  If Format is not NULL, then return.

  If the length of the message string specificed by Format is larger than the maximum allowable
  record length, then directly return and not print it.

  @param  ErrorLevel  The error level of the debug message.
  @param  Format      Format string for the debug message to print.
  @param  Marker      Variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
typedef
VOID
(EFIAPI *RSC_2_POSTCODE_MAP)(
  IN EFI_STATUS_CODE_VALUE          Value
  );

typedef struct {
  RSC_2_POSTCODE_MAP            PostCodeFromRscMap;
} RSC2POSTCODE_MAP_SERVICE_PPI;

extern EFI_GUID gFspRsc2PostCodeMapServicePpiGuid;

#endif
