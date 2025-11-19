/**@file
  Private mock for the Mipi Camera Install Table Dxe driver.

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

@par Specification
**/

#ifndef _PRIVATE_MOCK_H_
#define _PRIVATE_MOCK_H_

#include <Uefi.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <cmocka.h>
#include <PiDxe.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Protocol/MipiCamNvsArea.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <MipiCamConfig.h>
#include <Library/HobLib.h>
#include <Guid/MipiCamConfigHob.h>

extern MIPICAM_NVS_AREA_PROTOCOL  mMipiCamNvsAreaProtocol;
extern MIPICAM_CONFIG             *gMipiCamSetupData;
extern MIPICAM_CONFIG_HOB         *gMipiCamConfigHob;
extern MIPICAM_CONFIG_HOB_EXT     *gMipiCamConfigHobExt;
extern VOID                       *gHobList;
extern VOID                       *gHobExtList;

/**
  Closes an event and frees the event structure.

  @param UserEvent               Event to close

  @retval EFI_INVALID_PARAMETER  Parameters are not valid.
  @retval EFI_SUCCESS            The event has been closed

**/
EFI_STATUS
EFIAPI
MockBootServiceCloseEvent (
  IN EFI_EVENT  UserEvent
  );

/**
  Mock of Allocates memory pages.

  @param[in]      Type           The type of allocation to perform.
  @param[in]      MemoryType     The type of memory to allocate.
                                 MemoryType values in the range 0x70000000..0x7FFFFFFF
                                 are reserved for OEM use. MemoryType values in the range
                                 0x80000000..0xFFFFFFFF are reserved for use by UEFI OS loaders
                                 that are provided by operating system vendors.
  @param[in]      Pages          The number of contiguous 4 KB pages to allocate.
  @param[in, out] Memory         The pointer to a physical address. On input, the way
                                 in which the address is used depends on the value of Type.

  @retval EFI_SUCCESS            The requested pages were allocated.
  @retval EFI_INVALID_PARAMETER  1) Type is not AllocateAnyPages or AllocateMaxAddress or AllocateAddress.
                                 2) MemoryType is in the range EfiMaxMemoryType..0x6FFFFFFF.
                                 3) Memory is NULL.
                                 4) MemoryType is EfiPersistentMemory.
  @retval EFI_OUT_OF_RESOURCES   The pages could not be allocated.
  @retval EFI_NOT_FOUND          The requested pages could not be found.

**/
EFI_STATUS
EFIAPI
MockBootServiceAllocatePages (
  IN     EFI_ALLOCATE_TYPE     Type,
  IN     EFI_MEMORY_TYPE       MemoryType,
  IN     UINTN                 Pages,
  IN OUT EFI_PHYSICAL_ADDRESS  *Memory
  );

/**
  Mock GetVariable: returns the value of a variable.

  @param[in]      VariableName    A Null-terminated string that is the name of the vendor's variable.
  @param[in]      VendorGuid      A unique identifier for the vendor.
  @param[out]     Attributes      If not NULL, a pointer to the memory location to return the
                                  attributes bitmask for the variable.
  @param[in, out] DataSize        On input, the size in bytes of the return Data buffer.
                                  On output the size of data returned in Data.
  @param[out]     Data            The buffer to return the contents of the variable. May be NULL
                                  with a zero DataSize in order to determine the size buffer needed.

  @retval EFI_SUCCESS             The function completed successfully.
  @retval EFI_NOT_FOUND           The variable was not found.
  @retval EFI_BUFFER_TOO_SMALL    The DataSize is too small for the result.
  @retval EFI_INVALID_PARAMETER   VariableName is NULL.
  @retval EFI_INVALID_PARAMETER   VendorGuid is NULL.
  @retval EFI_INVALID_PARAMETER   DataSize is NULL.
  @retval EFI_INVALID_PARAMETER   The DataSize is not too small and Data is NULL.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a hardware error.
  @retval EFI_SECURITY_VIOLATION  The variable could not be retrieved due to an authentication failure.

**/
EFI_STATUS
EFIAPI
MockRuntimeGetVariable (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
  OUT    UINT32    *Attributes OPTIONAL,
  IN OUT UINTN     *DataSize,
  OUT    VOID      *Data OPTIONAL
  );

/**
  Mock SetVariable: sets the value of a variable.

  @param[in] VariableName         A Null-terminated string that is the name of the vendor's variable.
                                  Each VariableName is unique for each VendorGuid. VariableName must
                                  contain 1 or more characters. If VariableName is an empty string,
                                  then EFI_INVALID_PARAMETER is returned.
  @param[in] VendorGuid           A unique identifier for the vendor.
  @param[in] Attributes           Attributes bitmask to set for the variable.
  @param[in] DataSize             The size in bytes of the Data buffer. Unless the EFI_VARIABLE_APPEND_WRITE or
                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACCESS attribute is set, a size of zero
                                  causes the variable to be deleted. When the EFI_VARIABLE_APPEND_WRITE attribute is
                                  set, then a SetVariable() call with a DataSize of zero will not cause any change to
                                  the variable value (the timestamp associated with the variable may be updated however
                                  even if no new data value is provided,see the description of the
                                  EFI_VARIABLE_AUTHENTICATION_2 descriptor below. In this case the DataSize will not
                                  be zero since the EFI_VARIABLE_AUTHENTICATION_2 descriptor will be populated).
  @param[in] Data                 The contents for the variable.

  @retval EFI_SUCCESS             The firmware has successfully stored the variable and its data as
                                  defined by the Attributes.
  @retval EFI_INVALID_PARAMETER   An invalid combination of attribute bits, name, and GUID was supplied, or the
                                  DataSize exceeds the maximum allowed.
  @retval EFI_INVALID_PARAMETER   VariableName is an empty string.
  @retval EFI_OUT_OF_RESOURCES    Not enough storage is available to hold the variable and its data.
  @retval EFI_DEVICE_ERROR        The variable could not be retrieved due to a hardware error.
  @retval EFI_WRITE_PROTECTED     The variable in question is read-only.
  @retval EFI_WRITE_PROTECTED     The variable in question cannot be deleted.
  @retval EFI_SECURITY_VIOLATION  The variable could not be written due to
                                  EFI_VARIABLE_TIME_BASED_AUTHENTICATED_WRITE_ACESS being set,
                                  but the AuthInfo does NOT pass the validation check carried out by the firmware.
  @retval EFI_NOT_FOUND           The variable trying to be updated or deleted was not found.

**/
EFI_STATUS
EFIAPI
MockRuntimeSetVariable (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID      *Data
  );

/**
  Mock of Boot Services for LocateHandleBuffer.

  @param[in]     SearchType      Specifies which handle(s) are to be returned.
  @param[in]     Protocol        Specifies the protocol to search by.
  @param[in]     SearchKey       Specifies the search key.
  @param[in,out] NumberHandles   On input, the size in bytes of Buffer. On output, the size in bytes of
                                 the array returned in Buffer (if the buffer was large enough) or the
                                 size, in bytes, of the buffer needed to obtain the array (if the buffer was
                                 not large enough).
  @param[out]    Buffer          The buffer in which the array is returned.

  @retval EFI_SUCCESS            The result array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the matching results.
  @retval EFI_INVALID_PARAMETER  One or more parameters are not valid.

**/
EFI_STATUS
EFIAPI
MockBootServiceLocateHandleBuffer (
  IN     EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN     EFI_GUID                *Protocol OPTIONAL,
  IN     VOID                    *SearchKey OPTIONAL,
  IN OUT UINTN                   *NumberHandles,
  OUT    EFI_HANDLE              **Buffer
  );

/**
  Mock of Boot Services for HandleProtocol.

  @param[in]  UserHandle         The handle being queried.
  @param[in]  Protocol           The published unique identifier of the protocol.
  @param[out] Interface          Supplies the address where a pointer to the
                                 corresponding Protocol Interface is returned.

  @retval EFI_SUCCESS            The interface information for the specified protocol was returned.
  @retval EFI_UNSUPPORTED        The device does not support the specified protocol.
  @retval EFI_INVALID_PARAMETER  Handle, Protocol, Interface is NULL.

**/
EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN  EFI_HANDLE  UserHandle,
  IN  EFI_GUID    *Protocol,
  OUT VOID        **Interface
  );

/**
  Mock of Boot Services for LocateProtocol.

  @param[in]  Protocol           The protocol to search for
  @param[in]  Registration       Optional Registration Key returned from RegisterProtocolNotify()
  @param[out] Interface          Return the Protocol interface (instance)

  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found

**/
EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN  EFI_GUID  *Protocol,
  IN  VOID      *Registration OPTIONAL,
  OUT VOID      **Interface
  );

/**
  Retrieves a file and/or file information from the firmware volume (ReadFile).

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
  Locates the requested section within a file and returns it in a buffer (ReadSection).

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
  Get ACPI Table raw data by file path

  @param[out] FileData          ACPI Table RAW data
  @param[out] FileSize          ACPI Table size

  @retval EFI_SUCCESS           Get ACPI table successfully.
  @retval EFI_OUT_OF_RESOURCES  Don't have any resource to allocate memory.
  @retval EFI_NOT_FOUND         File path not found.

**/
EFI_STATUS
EFIAPI
MockMipiCamAcpiTableFile (
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  );

/**
  The InstallAcpiTable() function allows a caller to install an ACPI table.

  @param[in]  This                 A pointer to a EFI_ACPI_TABLE_PROTOCOL.
  @param[in]  AcpiTableBuffer      A pointer to a buffer containing the ACPI table to be installed.
  @param[in]  AcpiTableBufferSize  Specifies the size, in bytes, of the AcpiTableBuffer buffer.
  @param[out] TableKey             Returns a key to refer to the ACPI table.

  @retval EFI_SUCCESS              The table was successfully inserted
  @retval EFI_INVALID_PARAMETER    Either AcpiTableBuffer is NULL, TableKey is NULL, or
                                   AcpiTableBufferSize and the size field embedded in the ACPI table
                                   pointed to by AcpiTableBuffer are not in sync.
  @retval EFI_OUT_OF_RESOURCES     Insufficient resources exist to complete the request.
  @retval EFI_ACCESS_DENIED        The table signature matches a table already present in the system
                                   and platform policy does not allow duplicate tables of this type.

**/
EFI_STATUS
EFIAPI
MockInstallAcpiTable (
  IN  EFI_ACPI_TABLE_PROTOCOL  *This,
  IN  VOID                     *AcpiTableBuffer,
  IN  UINTN                    AcpiTableBufferSize,
  OUT UINTN                    *TableKey
  );

/**
  Create a mock Hob data for Mipi Camera dummy configure setting
**/
VOID
EFIAPI
MockMipiCamGuidHob (
  VOID
  );

VOID
EFIAPI
MockMipiCamGuidHobExt (
  VOID
  );

#endif  // _PRIVATE_MOCK_H_
