/** @file
  Unit test case of the ExportPsrToUsbStorage function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include "TestCaseExportPsrToUsbStorage.h"
#include "PrivateMock.h"
#include <Library/MemoryAllocationLib.h>
#include "PsrMsgs.h"
#include <Protocol/BlockIo.h>

/**
  Unit Test case for ExportPsrToUsbStorage function.
  @param[in]  Context                       The pointer for the Unit Test context.
  @retval     UNIT_TEST_PASSED              Pass the test case.
  @retval     UNIT_TEST_ERROR_TEST_FAILED   Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
UnitTestExportPsrToUsbStorage (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  EFI_FILE                FileHandle;
  EFI_BLOCK_IO_PROTOCOL   *BlkIo = NULL;
  UINT8       TestDevicePath[] = {
                0x03, 0x05, 0x06, 0x00,                         // MESSAGING_DEVICE_PATH, MSG_USB_DP, sizeof (USB_DEVICE_PATH)
                0x01, 0x02,                                     //   ParentPortNumber, InterfaceNumber
                0x04, 0x01, 0x2A, 0x00,                         // MEDIA_DEVICE_PATH, MEDIA_HARDDRIVE_DP, sizeof (HARDDRIVE_DEVICE_PATH)
                0x01, 0x00, 0x00, 0x00,                         //   PartitionNumber
                0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //   PartitionStart
                0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, //   PartitionSize
                0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, //   Signature[16]
                0x88, 0x99, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, //
                0x00, 0x02,                                     //   MBRType, SignatureType
                0x7F, 0xFF, 0x04, 0x00                          // END_DEVICE_PATH_TYPE, END_ENTIRE_DEVICE_PATH_SUBTYPE, END_DEVICE_PATH_LENGTH
              };

  //
  // Mock PSR DATA
  //
  mPsrDataBlob = AllocateZeroPool (sizeof (PSR_DATA));
  if (mPsrDataBlob == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }

  //
  // Mock boot service
  //
  gBS = AllocateZeroPool (sizeof (EFI_BOOT_SERVICES));
  if (gBS == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  gBS->LocateHandleBuffer = MockBootServiceLocateHandleBuffer;
  gBS->HandleProtocol     = MockBootServiceHandleProtocol;
  gBS->LocateDevicePath   = MockBootServiceLocateDevicePath;

  //
  // Mock runtime service
  //
  gRT = AllocateZeroPool (sizeof (EFI_RUNTIME_SERVICES));
  if (gRT == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  gRT->GetTime = MockRuntimeServiceGetTime;

  //
  // Mock protocol gEfiSimpleFileSystemProtocolGuid
  //
  gFileSys = AllocateZeroPool (sizeof (EFI_SIMPLE_FILE_SYSTEM_PROTOCOL));
  if (gFileSys == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  gFileSys->OpenVolume = MockFileSysOpenVolume;

  //
  // Mock File System Handle
  //
  gFileSysHandle = AllocateZeroPool (sizeof (EFI_FILE));
  if (gFileSysHandle == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  gFileSysHandle->Open = MockFileSysHandleOpen;
  gFileSysHandle->Write = MockFileSysHandleWrite;
  gFileSysHandle->Close = MockFileSysHandleClose;

  // Mock function for gBS->LocateHandleBuffer
  gHandleBuffer = AllocateZeroPool (sizeof (EFI_HANDLE));
  if (gHandleBuffer == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  expect_value (MockBootServiceLocateHandleBuffer, SearchType, ByProtocol);
  expect_value (MockBootServiceLocateHandleBuffer, Protocol, &gEfiSimpleFileSystemProtocolGuid);
  expect_value (MockBootServiceLocateHandleBuffer, SearchKey, NULL);
  expect_any (MockBootServiceLocateHandleBuffer, NumberHandles);
  expect_any (MockBootServiceLocateHandleBuffer, Buffer);
  will_return (MockBootServiceLocateHandleBuffer, 1);
  will_return (MockBootServiceLocateHandleBuffer, gHandleBuffer);
  will_return (MockBootServiceLocateHandleBuffer, EFI_SUCCESS);

  // Mock function for gBS->HandleProtocol
  gMockDevicePath = AllocateCopyPool (sizeof (TestDevicePath), TestDevicePath);
  if (gMockDevicePath == NULL) {
    return UNIT_TEST_ERROR_PREREQUISITE_NOT_MET;
  }
  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiDevicePathProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gMockDevicePath);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  expect_value (MockBootServiceLocateDevicePath, Protocol, &gEfiUsbIoProtocolGuid);
  expect_any (MockBootServiceLocateDevicePath, DevicePath);
  expect_any (MockBootServiceLocateDevicePath, Device);
  will_return (MockBootServiceLocateDevicePath, &gExportHandle);
  will_return (MockBootServiceLocateDevicePath, EFI_SUCCESS);

  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiBlockIoProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, BlkIo);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  // Mock function for gBS->HandleProtocol
  expect_any (MockBootServiceHandleProtocol, UserHandle);
  expect_value (MockBootServiceHandleProtocol, Protocol, &gEfiSimpleFileSystemProtocolGuid);
  expect_any (MockBootServiceHandleProtocol, Interface);
  will_return (MockBootServiceHandleProtocol, gFileSys);
  will_return (MockBootServiceHandleProtocol, EFI_SUCCESS);

  // Mock function for FileSys->OpenVolume
  expect_value (MockFileSysOpenVolume, This, gFileSys);
  expect_any (MockFileSysOpenVolume, RootFile);
  will_return (MockFileSysOpenVolume, gFileSysHandle);
  will_return (MockFileSysOpenVolume, EFI_SUCCESS);

  // Mock function for gRT->GetTime
  expect_any (MockRuntimeServiceGetTime, Time);
  expect_value (MockRuntimeServiceGetTime, Capabilities, NULL);

  // Mock function for FileSysHandle->Open
  expect_value (MockFileSysHandleOpen, This, gFileSysHandle);
  expect_any (MockFileSysHandleOpen, NewHandle);
  expect_any (MockFileSysHandleOpen, FileName);
  expect_value (MockFileSysHandleOpen, OpenMode, EFI_FILE_MODE_CREATE | EFI_FILE_MODE_READ | EFI_FILE_MODE_WRITE);
  expect_value (MockFileSysHandleOpen, Attributes, EFI_FILE_ARCHIVE);
  will_return (MockFileSysHandleOpen, &FileHandle);
  will_return (MockFileSysHandleOpen, EFI_SUCCESS);

  // Mock function for FileSysHandle->Write
  expect_value (MockFileSysHandleWrite, This, &FileHandle);
  expect_any (MockFileSysHandleWrite, BufferSize);
  expect_value (MockFileSysHandleWrite, Buffer, mPsrDataBlob);
  will_return (MockFileSysHandleWrite, EFI_SUCCESS);

  // Mock function for FileSysHandle->Close
  expect_value (MockFileSysHandleClose, This, &FileHandle);
  will_return (MockFileSysHandleClose, EFI_SUCCESS);

  UT_ASSERT_STATUS_EQUAL (ExportPsrToUsbStorage (NULL, &gExportHandle), EFI_SUCCESS);

  FreePool (gMockDevicePath);
  FreePool (gBS);
  FreePool (gRT);
  FreePool (mPsrDataBlob);
  FreePool (gFileSysHandle);
  FreePool (gFileSys);

  return UNIT_TEST_PASSED;
}
