/** @file
  CatalogTHDebugPpi PPI prototypes.

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

#ifndef __CATALOG_TH_DEBUG_PPI_H__
#define __CATALOG_TH_DEBUG_PPI_H__

#include <Pi/PiPeiCis.h>
#include <Library/TraceHubDebugLib.h>

//
// Global ID for the CATALOG_TH_DEBUG_PPI
//
#define CATALOG_TH_DEBUG_PPI_GUID \
  { \
    0x10ac47a1, 0x5c6e, 0x456b, {0x85, 0x67, 0x43, 0xdf,0xd5,0x36,0x38, 0xd1} \
  }

///
/// Forward declaration for the PEI_DEBUG_LIB_DEBUG_PPI CATALOG_TH_DEBUG_PPI
///
typedef struct _CATALOG_TH_DEBUG_PPI CATALOG_TH_DEBUG_PPI;

/**
  Print a debug message to debug output device if the specified error level
  is enabled.

  @param[in] ErrorLevel               The error level of the debug message.
  @param[in] Format                   Format string for the debug message to print.
  @param[in] Marker                   BASE_LIST marker for the variable argument list.

**/
typedef
VOID
(EFIAPI *CATALOG_TH_DEBUG_PRINT)(
  IN UINTN                          ErrorLevel,
  IN CONST CHAR8                    *Format,
  IN TRACE_HUB_SEVERITY_TYPE        TraceHubSeverityLevel,
  IN UINTN                          Id,
  IN UINTN                          *Param,
  IN UINTN                          NumberOfParams
  );

///
/// This PPI contains a set of services to print message to debug output device
///
struct _CATALOG_TH_DEBUG_PPI {
  CATALOG_TH_DEBUG_PRINT            DebugPrint;
};

extern EFI_GUID gCatalogTHDebugPpiGuid;

BOOLEAN
EFIAPI
CheckTraceHubEnable();
#endif

