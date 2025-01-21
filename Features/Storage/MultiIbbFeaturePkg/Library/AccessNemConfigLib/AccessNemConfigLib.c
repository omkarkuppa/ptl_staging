/** @file
  Provide APIs for accessing BaseNemConfigLib not in Feature Pkg.

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

#include <Uefi.h>
#include <PiPei.h>
#include <Library/AccessNemConfigLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/DebugLib.h>


/**
  Ppi setup for accessing libraries not in Feature Pkg.

  @param[in]  FunctionName       Input the name of the calling function
  @param[in]  Ptr1               Pointer of the input parameter 1 consumed by indicating function
  @param[in]  Ptr2               Pointer of the input parameter 2 consumed by indicating function
  @param[in]  Ptr3               Pointer of the input parameter 3 consumed by indicating function
  @param[in]  Ptr4               Pointer of the input parameter 4 consumed by indicating function

  @retval EFI_SUCCESS            Ppi setup successfully
  @retval Others                 Ppi setup failed
**/
EFI_STATUS
BridgeTo1sLibPpiSetup (
  IN UINT32  FunctionName,
  IN UINTN  *Ptr1,
  IN UINTN  *Ptr2,
  IN UINTN  *Ptr3,
  IN UINTN  *Ptr4
  )
{
  EFI_PEI_PPI_DESCRIPTOR          *ConfigPpiDesc;
  NEM_DATA_INPUT_STRUCT           *DataStruct;
  EFI_STATUS                      Status;

  DataStruct = NULL;

  ConfigPpiDesc = (EFI_PEI_PPI_DESCRIPTOR *) AllocateZeroPool (sizeof (EFI_PEI_PPI_DESCRIPTOR));
  if (ConfigPpiDesc == NULL) {
    ASSERT (FALSE);
    return EFI_OUT_OF_RESOURCES;
  }

  DataStruct = AllocatePool (sizeof (*DataStruct));
  ASSERT (DataStruct != NULL);
  DataStruct->FunctionName   = FunctionName;
  DataStruct->Data1          = Ptr1;
  DataStruct->Data2          = Ptr2;
  DataStruct->Data3          = Ptr3;
  DataStruct->Data4          = Ptr4;

  switch (FunctionName) {
    case MtrrInitMask:
      ConfigPpiDesc->Guid  = &gMtrrInitMaskPpiGuid;
      break;
    case MtrrRangeChange:
      ConfigPpiDesc->Guid  = &gMtrrRangeChangePpiGuid;
      break;
    case NemSizeCalculation:
      ConfigPpiDesc->Guid  = &gNemSizeCalculatePpiGuid;
      break;
  }

  ConfigPpiDesc->Flags = EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST;
  ConfigPpiDesc->Ppi   = DataStruct;
  Status = PeiServicesInstallPpi (ConfigPpiDesc);

  return Status;
}