/** @file
  Provides services to access SMRAM Save State Map

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

#include "MmSaveState.h"
#include <Protocol/MpService.h>

/**
  The constructor function.

  @param  ImageHandle     The firmware allocated handle for the EFI image.
  @param  MmSystemTable   A pointer to the MM System Table.

  @retval EFI_SUCCESS   The constructor always returns EFI_SUCCESS.

**/
EFI_STATUS
EFIAPI
DxeMmSaveStateLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                 Status;
  EFI_MP_SERVICES_PROTOCOL   *MpServices;
  UINTN                      NumberOfEnabledProcessors;
  EFI_PROCESSOR_INFORMATION  ProcessorInfo;
  UINTN                      Index;

  Status = MmSaveStateLibConstructorCommon ();
  ASSERT_EFI_ERROR (Status);

  if (mMmSaveStateInMsr) {
    Status = SystemTable->BootServices->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&MpServices);
    ASSERT_EFI_ERROR (Status);

    //
    // Use MP Services Protocol to retrieve the number of processors and number of enabled processors
    //
    Status = MpServices->GetNumberOfProcessors (MpServices, &mMmNumberOfCpus, &NumberOfEnabledProcessors);
    ASSERT_EFI_ERROR (Status);
    mMmApicId = AllocatePool (sizeof (UINT32) * mMmNumberOfCpus);
    ASSERT (mMmApicId != NULL);

    for (Index = 0; Index < mMmNumberOfCpus; Index++) {
      Status = MpServices->GetProcessorInfo (MpServices, Index, &ProcessorInfo);
      ASSERT_EFI_ERROR (Status);
      mMmApicId[Index] =(UINT32) ProcessorInfo.ProcessorId;
    }
  }

  return EFI_SUCCESS;
}
