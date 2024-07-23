/** @file
  This driver installs gCatalogTHDebugPpiGuid PPI to provide
  catalog debug services for PEIMs.

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

#include <Uefi/UefiBaseType.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>
#include <CatalogTHDebug.h>
#include <Library/TraceHubDebugLib.h>


/**
  Print the catalog message to TH debug output device if the specified error level
  is enabled.

  @param[in] ErrorLevel               The error level of the debug message.
  @param[in] Format                   Format string for the debug message to print.
  @param[in] Marker                   BASE_LIST marker for the variable argument list.

**/
VOID
EFIAPI
CatalogTHDebugPrint(
  IN UINTN                          ErrorLevel,
  IN CONST CHAR8                    *Format,
  IN TRACE_HUB_SEVERITY_TYPE        TraceHubSeverityLevel,
  IN UINTN                          Id,
  IN UINTN                          *Param,
  IN UINTN                          NumberOfParams
  )
{
   switch (NumberOfParams) {
  case 0:
    TraceHubWriteCataLog64_0 (TraceHubSeverityLevel, Id);
    break;
  case 1:
    TraceHubWriteCataLog64_1 (TraceHubSeverityLevel, Id, (UINT32)Param[0]);
    break;
  case 2:
    TraceHubWriteCataLog64_2 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1]);
    break;
  case 3:
    TraceHubWriteCataLog64_3 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2]);
    break;
  case 4:
    TraceHubWriteCataLog64_4 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3]);
    break;
  case 5:
    TraceHubWriteCataLog64_5 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3], (UINT32)Param[4]);
    break;
  case 6:
    TraceHubWriteCataLog64_6 (TraceHubSeverityLevel, Id, (UINT32)Param[0], (UINT32)Param[1], (UINT32)Param[2], (UINT32)Param[3], (UINT32)Param[4], (UINT32)Param[5]);
    break;
  default:
    //
    // There is an unsupported debug message with more than 6 parameter
    //
    break;
  }
}

CATALOG_TH_DEBUG_PPI mCatalogTHDebugPpi = {
  CatalogTHDebugPrint
};

EFI_PEI_PPI_DESCRIPTOR mCatalogTHDebugServicePpi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gCatalogTHDebugPpiGuid,
  (VOID *)&mCatalogTHDebugPpi
};

/**
  Entry point of Debug Service PEIM

  This funciton installs EDKII DEBUG PPI

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCESS  The entry point of Debug Service PEIM executes successfully.
  @retval Others      Some error occurs during the execution of this function.

**/
EFI_STATUS
EFIAPI
CatalogTHDebugSerivceInitialize (
  IN EFI_PEI_FILE_HANDLE        FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  return PeiServicesInstallPpi (&mCatalogTHDebugServicePpi);
}

