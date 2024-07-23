/**@file
  Private mock for the I2C Device Common Driver

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

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <Uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <PiDxe.h>
#include <I2cCommon.h>
#include <Protocol/AcpiTable.h>
#include <Protocol/FirmwareVolume2.h>

extern EFI_ACPI_TABLE_PROTOCOL      *mAcpiTable;
extern EFI_ACPI_DESCRIPTION_HEADER  *mI2CAcpiTable;

/**
  Get ACPI Table raw data by file path.

  @param[out] FileData          ACPI Table RAW data
  @param[out] FileSize          ACPI Table size

  @retval EFI_SUCCESS           Get ACPI table successfully.
  @retval EFI_OUT_OF_RESOURCES  Don't have any resource to allocate memory.
  @retval EFI_NOT_FOUND         File path not found.

**/
EFI_STATUS
EFIAPI
MockI2cAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  );

/**
  Mock EfiFirmwareVolume2Protocol ReadFile: Retrieves a file and/or file information from the firmware volume (ReadFile).

  @param[in]     FwVl                  Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.
  @param[in]     NameGuid              Pointer to an EFI_GUID, which is the file name.
                                       All firmware file names are EFI_GUIDs. A single
                                       firmware volume must not have two valid files
                                       with the same file name EFI_GUID.
  @param[in,out] Buffer                Pointer to a pointer to a buffer in which the file
                                       contents are returned, not including the file header.
  @param[in,out] BufferSize            Pointer to a caller-allocated UINTN. It indicates
                                       the size of the memory represented by Buffer.
  @param[out]    FoundType             Pointer to a caller-allocated EFI_FV_FILETYPE.
  @param[out]    FileAttributes        Pointer to a  caller-allocated EFI_FV_FILE_ATTRIBUTES.
  @param[out]    AuthenticationStatus  Pointer to a caller-allocated UINT32 in which the
                                       authentication status is returned.

  @retval EFI_SUCCESS                  The call completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The buffer is too small to contain the requested output.
                                       The buffer is filled and the output is truncated.
  @retval EFI_OUT_OF_RESOURCES         An allocation failure occurred.
  @retval EFI_NOT_FOUND                Name was not found in the firmware volume.
  @retval EFI_DEVICE_ERROR             A hardware error occurred when attempting to access the firmware volume.
  @retval EFI_ACCESS_DENIED            The firmware volume is configured to disallow reads.

**/
EFI_STATUS
EFIAPI
MockFwVol2ReadFile (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN OUT   VOID                           **Buffer,
  IN OUT   UINTN                          *BufferSize,
  OUT      EFI_FV_FILETYPE                *FoundType,
  OUT      EFI_FV_FILE_ATTRIBUTES         *FileAttributes,
  OUT      UINT32                         *AuthenticationStatus
  );

/**
  Mock EfiFirmwareVolume2Protocol ReadSection: Locates the requested section within a file and returns it in a buffer (ReadSection).

  @param[in]     This                  Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.
  @param[in]     NameGuid              Pointer to an EFI_GUID, which indicates the file name from which
                                       the requested section will be read.
  @param[in]     SectionType           Indicates the section type to return. SectionType in conjunction
                                       with SectionInstance indicates which section to return.
  @param[in]     SectionInstance       Indicates which instance of sections with a type of SectionType to return.
                                       SectionType in conjunction with SectionInstance indicates which section
                                       to return. SectionInstance is zero based.
  @param[in,out] Buffer                Pointer to a pointer to a buffer in which the section
                                       contents are returned, not including the section header.
  @param[in,out] BufferSize            Pointer to a caller-allocated UINTN.
                                       It indicates the size of the memory represented by Buffer.
  @param[out]    AuthenticationStatus  Pointer to a caller-allocated UINT32 in which the
                                       authentication status is returned.

  @retval EFI_SUCCESS                  The call completed successfully.
  @retval EFI_WARN_BUFFER_TOO_SMALL    The caller-allocated buffer is too small to contain the requested output.
                                       The buffer is filled and the output is truncated.
  @retval EFI_OUT_OF_RESOURCES         An allocation failure occurred.
  @retval EFI_NOT_FOUND                The requested file was not found in the firmware volume.
                                       EFI_NOT_FOUND The requested section was not found in the specified file.
  @retval EFI_DEVICE_ERROR             A hardware error occurred when attempting to access the firmware volume.
  @retval EFI_ACCESS_DENIED            The firmware volume is configured to disallow reads.
                                       EFI_PROTOCOL_ERROR The requested section was not found,
                                       but the file could not be fully parsed because a required
                                       GUIDED_SECTION_EXTRACTION_PROTOCOL was not found.
                                       It is possible the requested section exists within the file and
                                       could be successfully extracted once the required
                                       GUIDED_SECTION_EXTRACTION_PROTOCOL is published.

**/
EFI_STATUS
EFIAPI
MockFwVol2ReadSection (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN       EFI_SECTION_TYPE               SectionType,
  IN       UINTN                          SectionInstance,
  IN OUT   VOID                           **Buffer,
  IN OUT   UINTN                          *BufferSize,
  OUT      UINT32                         *AuthenticationStatus
  );

/**

  Mock for InstallAcpiTable.


  @param This                 A pointer to a EFI_ACPI_TABLE_PROTOCOL.

  @param AcpiTableBuffer      A pointer to a buffer containing the
                              ACPI table to be installed.

  @param AcpiTableBufferSize  Specifies the size, in bytes, of
                              the AcpiTableBuffer buffer.


  @param TableKey             Returns a key to refer to the ACPI table.

  @retval EFI_SUCCESS           The table was successfully inserted

  @retval EFI_INVALID_PARAMETER Either AcpiTableBuffer is NULL,
                                TableKey is NULL, or
                                AcpiTableBufferSize and the size
                                field embedded in the ACPI table
                                pointed to by AcpiTableBuffer
                                are not in sync.

  @retval EFI_OUT_OF_RESOURCES  Insufficient resources exist to
                                complete the request.
  @retval EFI_ACCESS_DENIED     The table signature matches a table already
                                present in the system and platform policy
                                does not allow duplicate tables of this type.

**/
EFI_STATUS
EFIAPI
MockInstallAcpiTable (
  IN   EFI_ACPI_TABLE_PROTOCOL  *This,
  IN   VOID                     *AcpiTableBuffer,
  IN   UINTN                    AcpiTableBufferSize,
  OUT  UINTN                    *TableKey
  );

#endif  // _PRIVATE_MOCK_H_
