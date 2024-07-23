/** @file
  Library instance of SPI Flash Common Library Class

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2011 Intel Corporation.

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

#include <Library/SpiFlashCommon.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeLib.h>
#include <Library/DxeServicesTableLib.h>
#include <Guid/EventGroup.h>

extern PCH_SPI_PROTOCOL *mSpiProtocol;

//
// BiosAreaBaseAddress and Size for boottime and runtime usage.
//
extern UINTN mBiosAreaBaseAddress;
extern UINTN mBiosSize;

EFI_EVENT  mSpiFlashCommonLibAddressChangeEvent = NULL;

/**
  Notification function of EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE.

  This is a notification function registered on EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE event.
  It convers pointer to new virtual address.

  @param[in]  Event        Event whose notification function is being invoked.
  @param[in]  Context      Pointer to the notification function's context.

**/
VOID
EFIAPI
SpiFlashAddressChangeEvent (
  IN EFI_EVENT             Event,
  IN VOID                  *Context
  )
{
  EfiConvertPointer (0x0, (VOID **) &mSpiProtocol);
  EfiConvertPointer (0x0, (VOID **) &mBiosAreaBaseAddress);
}

/**
  The library constructuor.

  The function does the necessary initialization work for this library
  instance.

  @param[in]  ImageHandle       The firmware allocated handle for the UEFI image.
  @param[in]  SystemTable       A pointer to the EFI system table.

  @retval     EFI_SUCCESS       The function always return EFI_SUCCESS for now.
                                It will ASSERT on error for debug version.
  @retval     EFI_ERROR         Please reference LocateProtocol for error code details.
**/
EFI_STATUS
EFIAPI
DxeSpiFlashCommonLibConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                            Status;
  EFI_GCD_MEMORY_SPACE_DESCRIPTOR       GcdDescriptor;
  EFI_PHYSICAL_ADDRESS                  BaseAddress;
  UINT64                                Length;

  mBiosAreaBaseAddress = (UINTN) PcdGet32 (PcdBiosAreaBaseAddress);
  mBiosSize = (UINTN) PcdGet32 (PcdBiosSize);

#if FixedPcdGetBool(PcdExtendedBiosRegionSupport) == 1
  mBiosAreaBaseAddress = (UINTN) PcdGet32 (PcdBiosAreaBaseAddress) + (UINTN) PcdGet32 (PcdFlashExtendRegionOffset);
  mBiosSize            = 0x1000000 + (UINTN) PcdGet32 (PcdFlashExtendRegionSizeInUse);
#endif
  BaseAddress = mBiosAreaBaseAddress & (~EFI_PAGE_MASK);
  Length      = (mBiosSize + EFI_PAGE_SIZE - 1) & (~EFI_PAGE_MASK);
  Status = gDS->GetMemorySpaceDescriptor (BaseAddress, &GcdDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "SpiFlashCommonLib failed to add EFI_MEMORY_RUNTIME attribute to Flash.\n"));
  } else {
    Status = gDS->SetMemorySpaceAttributes (
                    BaseAddress,
                    Length,
                    GcdDescriptor.Attributes | EFI_MEMORY_RUNTIME
                    );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "SpiFlashCommonLib failed to add EFI_MEMORY_RUNTIME attribute to Flash.\n"));
    }
  }

  //
  // Locate the SPI protocol.
  //
  Status = gBS->LocateProtocol (
                  &gPchSpiProtocolGuid,
                  NULL,
                  (VOID **)&mSpiProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  Status = gBS->CreateEventEx (
                  EVT_NOTIFY_SIGNAL,
                  TPL_NOTIFY,
                  SpiFlashAddressChangeEvent,
                  NULL,
                  &gEfiEventVirtualAddressChangeGuid,
                  &mSpiFlashCommonLibAddressChangeEvent
                  );
  ASSERT_EFI_ERROR (Status);

  return Status;
}
