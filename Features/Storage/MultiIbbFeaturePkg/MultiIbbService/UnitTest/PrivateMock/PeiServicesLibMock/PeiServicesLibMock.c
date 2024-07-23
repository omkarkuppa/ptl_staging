/** @file
  Mock instance of PeiServicesLib library class.

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

@par Specification
**/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>

/**
  This service enables a given PEIM to register an interface into the PEI Foundation.

  @param[in]  PpiList  A pointer to the list of interfaces that the caller shall install.

  @retval EFI_SUCCESS            - The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  - The PpiList pointer is NULL.
  @retval EFI_INVALID_PARAMETER  - Any of the PEI PPI descriptors in the list do not have the
                                   EFI_PEI_PPI_DESCRIPTOR_PPI bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES   - There is no additional space in the PPI database.
**/
EFI_STATUS
EFIAPI
PeiServicesInstallPpi (
  IN CONST EFI_PEI_PPI_DESCRIPTOR  *PpiList
  )
{
  check_expected_ptr (PpiList);

  return mock_type (EFI_STATUS);
}

/**
  This service enables PEIMs to discover a given instance of an interface.

  @param[in]       Guid           A pointer to the GUID whose corresponding interface needs to be found.
  @param[in]       Instance       The N-th instance of the interface that is required.
  @param[in, out]  PpiDescriptor  A pointer to instance of the EFI_PEI_PPI_DESCRIPTOR.
  @param[in, out]  Ppi            A pointer to the instance of the interface.

  @retval EFI_SUCCESS    - The interface was successfully returned.
  @retval EFI_NOT_FOUND  - The PPI descriptor is not found in the database.
**/
EFI_STATUS
EFIAPI
PeiServicesLocatePpi (
  IN CONST EFI_GUID              *Guid,
  IN UINTN                       Instance,
  IN OUT EFI_PEI_PPI_DESCRIPTOR  **PpiDescriptor  OPTIONAL,
  IN OUT VOID                    **Ppi
  )
{
  check_expected_ptr (Guid);
  check_expected (Instance);
  check_expected_ptr (PpiDescriptor);
  check_expected_ptr (Ppi);

  *Ppi = mock_ptr_type (VOID*);

  return mock_type (EFI_STATUS);
}

/**
  This service enables PEIMs to register a given service to be invoked when another service is
  installed or reinstalled.

  @param[in]  NotifyList  A pointer to the list of notification interfaces that the caller shall install.

  @retval EFI_SUCCESS            - The interface was successfully installed.
  @retval EFI_INVALID_PARAMETER  - The NotifyList pointer is NULL.
  @retval EFI_INVALID_PARAMETER  - Any of the PEI notify descriptors in the list do not have the
                                   EFI_PEI_PPI_DESCRIPTOR_NOTIFY_TYPES bit set in the Flags field.
  @retval EFI_OUT_OF_RESOURCES   - There is no additional space in the PPI database.
**/
EFI_STATUS
EFIAPI
PeiServicesNotifyPpi (
  IN CONST EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyList
  )
{
  check_expected_ptr (NotifyList);

  return mock_type (EFI_STATUS);
}

/**
  This service enables PEIMs to discover additional firmware volumes.

  @param[in]       Instance      This instance of the firmware volume to find. The value 0 is
                                 the Boot Firmware Volume (BFV).
  @param[in, out]  VolumeHandle  Handle of the firmware volume header of the volume to return.

  @retval EFI_SUCCESS            - The volume was found.
  @retval EFI_NOT_FOUND          - The volume was not found.
  @retval EFI_INVALID_PARAMETER  - FwVolHeader is NULL.
**/
EFI_STATUS
EFIAPI
PeiServicesFfsFindNextVolume (
  IN UINTN                  Instance,
  IN OUT EFI_PEI_FV_HANDLE  *VolumeHandle
  )
{
  check_expected (Instance);
  check_expected_ptr (VolumeHandle);

  *VolumeHandle = mock_ptr_type (EFI_PEI_FV_HANDLE);

  return mock_type (EFI_STATUS);
}

/**
  This service enables PEIMs to discover sections of a given type within a valid FFS file.

  @param[in]   SectionType  The value of the section type to find.
  @param[in]   FileHandle   A pointer to the file header that contains the set
                            of sections to be searched.
  @param[out]  SectionData  A pointer to the discovered section, if successful.

  @retval EFI_SUCCESS    - The section was found.
  @retval EFI_NOT_FOUND  - The section was not found.
**/
EFI_STATUS
EFIAPI
PeiServicesFfsFindSectionData (
  IN EFI_SECTION_TYPE     SectionType,
  IN EFI_PEI_FILE_HANDLE  FileHandle,
  OUT VOID                **SectionData
  )
{
  check_expected (SectionType);
  check_expected_ptr (FileHandle);

  *SectionData = mock_ptr_type (VOID*);

  return mock_type (EFI_STATUS);
}

/**
  This service is a wrapper for the PEI Service FfsFindByName(), except the pointer to the PEI Services
  Table has been removed.  See the Platform Initialization Pre-EFI Initialization Core Interface
  Specification for details.

  @param[in]   FileName      A pointer to the name of the file to find within the firmware volume.
  @param[in]   VolumeHandle  The firmware volume to search FileHandle Upon exit, points to the found
                             file's handle or NULL if it could not be found.
  @param[out]  FileHandle    The pointer to found file handle.

  @retval EFI_SUCCESS            - File was found.
  @retval EFI_NOT_FOUND          - File was not found.
  @retval EFI_INVALID_PARAMETER  - VolumeHandle or FileHandle or FileName was NULL.
**/
EFI_STATUS
EFIAPI
PeiServicesFfsFindFileByName (
  IN CONST  EFI_GUID             *FileName,
  IN CONST  EFI_PEI_FV_HANDLE    VolumeHandle,
  OUT       EFI_PEI_FILE_HANDLE  *FileHandle
  )
{
  check_expected_ptr (FileName);
  check_expected_ptr (VolumeHandle);

  *FileHandle = mock_ptr_type (EFI_PEI_FILE_HANDLE);

  return mock_type (EFI_STATUS);
}

/**
  This service is a wrapper for the PEI Service FfsGetVolumeInfo(), except the pointer to the PEI Services
  Table has been removed.  See the Platform Initialization Pre-EFI Initialization Core Interface
  Specification for details.

  @param[in]   VolumeHandle  Handle of the volume.
  @param[out]  VolumeInfo    Upon exit, points to the volume's information.

  @retval EFI_SUCCESS            - File information returned.
  @retval EFI_INVALID_PARAMETER  - If FileHandle does not represent a valid file.
  @retval EFI_INVALID_PARAMETER  - If FileInfo is NULL.
**/
EFI_STATUS
EFIAPI
PeiServicesFfsGetVolumeInfo (
  IN  EFI_PEI_FV_HANDLE  VolumeHandle,
  OUT EFI_FV_INFO        *VolumeInfo
  )
{
  check_expected_ptr (VolumeHandle);

  CopyMem (VolumeInfo, mock_ptr_type (EFI_FV_INFO*), sizeof (EFI_FV_INFO));

  return mock_type (EFI_STATUS);
}

/**
  Install a EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI instance so the PEI Core will be notified about a new firmware volume.

  This function allocates, initializes, and installs a new EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI using
  the parameters passed in to initialize the fields of the EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI instance.
  If the resources can not be allocated for EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI, then ASSERT().
  If the EFI_PEI_FIRMWARE_VOLUME_INFO2_PPI can not be installed, then ASSERT().
  If NULL is specified for FvFormat, but FvInfo does not have the firmware file system 2 format, then ASSERT.

  @param[in]  FvFormat              Unique identifier of the format of the memory-mapped
                                    firmware volume.  This parameter is optional and
                                    may be NULL.  If NULL is specified, the
                                    EFI_FIRMWARE_FILE_SYSTEM2_GUID format is assumed.
  @param[in]  FvInfo                Points to a buffer which allows the
                                    EFI_PEI_FIRMWARE_VOLUME_PPI to process the volume.
                                    The format of this buffer is specific to the FvFormat.
                                    For memory-mapped firmware volumes, this typically
                                    points to the first byte of the firmware volume.
  @param[in]  FvInfoSize            The size, in bytes, of FvInfo. For memory-mapped
                                    firmware volumes, this is typically the size of
                                    the firmware volume.
  @param[in]  ParentFvName          If the new firmware volume originated from a file
                                    in a different firmware volume, then this parameter
                                    specifies the GUID name of the originating firmware
                                    volume. Otherwise, this parameter must be NULL.
  @param[in]  ParentFileName        If the new firmware volume originated from a file
                                    in a different firmware volume, then this parameter
                                    specifies the GUID file name of the originating
                                    firmware file. Otherwise, this parameter must be NULL.
  @param[in]  AuthenticationStatus  Authentication Status
**/
VOID
EFIAPI
PeiServicesInstallFvInfo2Ppi (
  IN CONST EFI_GUID  *FvFormat  OPTIONAL,
  IN CONST VOID      *FvInfo,
  IN       UINT32    FvInfoSize,
  IN CONST EFI_GUID  *ParentFvName  OPTIONAL,
  IN CONST EFI_GUID  *ParentFileName  OPTIONAL,
  IN       UINT32    AuthenticationStatus
  )
{
  check_expected_ptr (FvFormat);
  check_expected_ptr (FvInfo);
  check_expected (FvInfoSize);
  check_expected_ptr (ParentFvName);
  check_expected_ptr (ParentFileName);
  check_expected (AuthenticationStatus);
}
