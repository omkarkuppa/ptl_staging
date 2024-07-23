/** @file
  Installs the FspCatalogTHDebugServicePpi.

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

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/FspCatalogTHDebugService.h>



VOID
EFIAPI
FspDebugPrint (
  IN  UINTN                          ErrorLevel,
  IN  CONST CHAR8                    *Format,
  IN  TRACE_HUB_SEVERITY_TYPE        TraceHubSeverityLevel,
  IN  UINTN                          Id,
  IN  UINTN                          *Param,
  IN  UINTN                          NumberOfParams
  )
{
  //
  // Curenlty FSP sends debug messages directly to NPK without using ReportStatuscode/FSP(Debug/Event)Handle
  //
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

CATALOG_TH_DEBUG_SERVICE_PPI mFspCatalogDebugServicePpi = {
  FspDebugPrint
};

EFI_PEI_PPI_DESCRIPTOR mPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gFspCatalogTHDebugServicePpiGuid,
    (VOID *) &mFspCatalogDebugServicePpi
  }
};

/**
  Main entry

  @param[in]  FileHandle              Handle of the file being invoked.
  @param[in]  PeiServices             Pointer to PEI Services table.

  @retval EFI_SUCCESS Install function successfully.

**/
EFI_STATUS
EFIAPI
DebugServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS               Status;

  Status = PeiServicesInstallPpi (&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
