/** @file
  Installs the FspRsc2PostCodeMapServicePpi.

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

#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/FspRsc2PostCodeMapService.h>
#include <Library/PostCodeLib.h>
#include <Library/Rsc2PostCodeMapLib.h>



VOID
EFIAPI
PostCodeFromRscMap (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  UINT32                  PostCodeValue;

  PostCodeValue = GetPostCodeFromRscMap (Value);
  if ( PostCodeValue != 0 ) {
    PostCode (PostCodeValue);
  }
}

RSC2POSTCODE_MAP_SERVICE_PPI  mFspRsc2PostCodeMapServicePpi = {
  PostCodeFromRscMap
};

EFI_PEI_PPI_DESCRIPTOR mPpiList[] = {
  {
    (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
    &gFspRsc2PostCodeMapServicePpiGuid,
    &mFspRsc2PostCodeMapServicePpi
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
FspRsc2PostCodeMapServiceEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS               Status;

  Status = PeiServicesInstallPpi (&mPpiList[0]);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
