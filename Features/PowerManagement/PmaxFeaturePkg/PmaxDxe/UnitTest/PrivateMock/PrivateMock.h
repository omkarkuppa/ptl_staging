/**@file

    Private mock of the PmaxFeaturePkg.

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

#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>

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
MockPmaxAcpiTableFile(
  OUT UINT8  **FileData,
  OUT UINTN  *FileSize
  );

/**
  Mock BootService LocateProtocol: Return the first Protocol Interface that matches the Protocol GUID. If
  Registration is passed in, return a Protocol Instance that was just add
  to the system. If Registration is NULL return the first Protocol Interface
  you find.

  @param  Protocol               The protocol to search for
  @param  Registration           Optional Registration Key returned from
                                 RegisterProtocolNotify()
  @param  Interface              Return the Protocol interface (instance).

  @retval EFI_SUCCESS            If a valid Interface is returned
  @retval EFI_INVALID_PARAMETER  Invalid parameter
  @retval EFI_NOT_FOUND          Protocol interface not found
**/

EFI_STATUS
EFIAPI
MockBootServiceLocateProtocol (
  IN EFI_GUID  *Protocol,
  IN VOID      *Registration  OPTIONAL,
  OUT VOID     **Interface
  );

/**

  Mock BootService LocateHandleBuffer: Function returns an array of handles that support the requested protocol
  in a buffer allocated from pool. This is a version of UnitTestLocateHandle()
  that allocates a buffer for the caller.

  @param  SearchType             Specifies which handle(s) are to be returned.
  @param  Protocol               Provides the protocol to search by.    This
                                 parameter is only valid for SearchType
                                 ByProtocol.
  @param  SearchKey              Supplies the search key depending on the
                                 SearchType.
  @param  NumberHandles          The number of handles returned in Buffer.
  @param  Buffer                 A pointer to the buffer to return the requested
                                 array of  handles that support Protocol.

  @retval EFI_SUCCESS            The result array of handles was returned.
  @retval EFI_NOT_FOUND          No handles match the search.
  @retval EFI_OUT_OF_RESOURCES   There is not enough pool memory to store the
                                 matching results.
  @retval EFI_INVALID_PARAMETER  One or more parameters are not valid.

**/

EFI_STATUS
EFIAPI
MockBootServiceLocateHandleBuffer (
  IN EFI_LOCATE_SEARCH_TYPE  SearchType,
  IN EFI_GUID                *Protocol   OPTIONAL,
  IN VOID                    *SearchKey  OPTIONAL,
  IN OUT UINTN               *NumberHandles,
  OUT EFI_HANDLE             **Buffer
  );

/**
  Mock FirmwareVolume2Protocol ReadFile: Read data or size of either a File Entry or an Extended File Entry.

  @param[in]      BlockIo         BlockIo interface.
  @param[in]      DiskIo          DiskIo interface.
  @param[in]      Volume          Volume information pointer.
  @param[in]      ParentIcb       Long Allocation Descriptor pointer.
  @param[in]      FileEntryData   FE/EFE structure pointer.
  @param[in, out] ReadFileInfo    Read file information pointer.

  @retval EFI_SUCCESS             Data or size of a FE/EFE was read.
  @retval EFI_OUT_OF_RESOURCES    Data or size of a FE/EFE was not read due to
                                  lack of resources.
  @retval EFI_INVALID_PARAMETER   The read file flag given in ReadFileInfo is
                                  invalid.
  @retval EFI_UNSUPPORTED         The FE recording flag given in FileEntryData
                                  is not supported.
  @retval other                   Data or size of a FE/EFE was not read.
**/

EFI_STATUS
EFIAPI
MockFwVol2ReadFile (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN OUT VOID                             **Buffer,
  IN OUT UINTN                            *BufferSize,
  OUT EFI_FV_FILETYPE                     *FoundType,
  OUT EFI_FV_FILE_ATTRIBUTES              *FileAttributes,
  OUT UINT32                              *AuthenticationStatus
  );

/**
  Mock BootService HandleProtocol: Queries a handle to determine if it supports a specified protocol.

  @param  UserHandle             The handle being queried.
  @param  Protocol               The published unique identifier of the protocol.
  @param  Interface              Supplies the address where a pointer to the
                                 corresponding Protocol Interface is returned.

  @return The requested protocol interface for the handle
**/

EFI_STATUS
EFIAPI
MockBootServiceHandleProtocol (
  IN EFI_HANDLE  UserHandle,
  IN EFI_GUID    *Protocol,
  OUT VOID       **Interface
  );

extern EFI_FIRMWARE_VOLUME2_PROTOCOL* gFwVol;

/**
  Mock FirmwareVolume2Protocol ReadSection: Locates the requested section within a file and returns it in a buffer.

  ReadSection() is used to retrieve a specific section from a file
  within a firmware volume. The section returned is determined
  using a depth-first, left-to-right search algorithm through all
  sections found in the specified file. The output buffer is specified by a double indirection
  of the Buffer parameter. The input value of Buffer is used to
  determine if the output buffer is caller allocated or is
  dynamically allocated by ReadSection(). If the input value of
  Buffer!=NULL, it indicates that the output buffer is caller
  allocated. In this case, the input value of *BufferSize
  indicates the size of the caller-allocated output buffer. If
  the output buffer is not large enough to contain the entire
  requested output, it is filled up to the point that the output
  buffer is exhausted and EFI_WARN_BUFFER_TOO_SMALL is returned,
  and then BufferSize is returned with the size that is required
  to successfully complete the read. All other
  output parameters are returned with valid values. If the input
  value of *Buffer==NULL, it indicates the output buffer is to
  be allocated by ReadSection(). In this case, ReadSection()
  will allocate an appropriately sized buffer from boot services
  pool memory, which will be returned in *Buffer. The size of
  the new buffer is returned in *BufferSize and all other output
  parameters are returned with valid values. ReadSection() is
  callable only from TPL_NOTIFY and below. Behavior of
  ReadSection() at any EFI_TPL above TPL_NOTIFY is
  undefined.

  @param  This                Indicates the EFI_FIRMWARE_VOLUME2_PROTOCOL instance.

  @param NameGuid             Pointer to an EFI_GUID, which indicates the
                              file name from which the requested section
                              will be read.

  @param SectionType          Indicates the section type to return.
                              SectionType in conjunction with
                              SectionInstance indicates which section to
                              return.

  @param SectionInstance      Indicates which instance of sections
                              with a type of SectionType to return.
                              SectionType in conjunction with
                              SectionInstance indicates which
                              section to return. SectionInstance is
                              zero based.

  @param Buffer               Pointer to a pointer to a buffer in which the
                              section contents are returned, not including
                              the section header.

  @param BufferSize           Pointer to a caller-allocated UINTN. It
                              indicates the size of the memory
                              represented by Buffer.

  @param AuthenticationStatus Pointer to a caller-allocated
                              UINT32 in which the authentication
                              status is returned.


  @retval EFI_SUCCESS   The call completed successfully.

  @retval EFI_WARN_BUFFER_TOO_SMALL   The caller-allocated
                                      buffer is too small to
                                      contain the requested
                                      output. The buffer is
                                      filled and the output is
                                      truncated.

  @retval EFI_OUT_OF_RESOURCES  An allocation failure occurred.

  @retval EFI_NOT_FOUND   The requested file was not found in
                          the firmware volume. EFI_NOT_FOUND The
                          requested section was not found in the
                          specified file.

  @retval EFI_DEVICE_ERROR  A error occurred when
                            attempting to access the firmware
                            volume.

  @retval EFI_ACCESS_DENIED The firmware volume is configured to
                            disallow reads. EFI_PROTOCOL_ERROR
                            The requested section was not found,
                            but the file could not be fully
                            parsed because a required
                            GUIDED_SECTION_EXTRACTION_PROTOCOL
                            was not found. It is possible the
                            requested section exists within the
                            file and could be successfully
                            extracted once the required
                            GUIDED_SECTION_EXTRACTION_PROTOCOL
                            is published.
**/

EFI_STATUS
EFIAPI
MockFwVol2ReadSection (
  IN CONST EFI_FIRMWARE_VOLUME2_PROTOCOL  *This,
  IN CONST EFI_GUID                       *NameGuid,
  IN EFI_SECTION_TYPE                     SectionType,
  IN UINTN                                SectionInstance,
  IN OUT VOID                             **Buffer,
  IN OUT UINTN                            *BufferSize,
  OUT UINT32                              *AuthenticationStatus
  );

/**
  Mock AcpiTableProtocol InstallAcpiTable: The InstallAcpiTable() function allows a caller to install an
  ACPI table. When successful, the table will be linked by the
  RSDT/XSDT. AcpiTableBuffer specifies the table to be installed.
  InstallAcpiTable() will make a copy of the table and insert the
  copy into the RSDT/XSDT. InstallAcpiTable() must insert the new
  table at the end of the RSDT/XSDT. To prevent namespace
  collision, ACPI tables may be created using UEFI ACPI table
  format. If this protocol is used to install a table with a
  signature already present in the system, the new table will not
  replace the existing table. It is a platform implementation
  decision to add a new table with a signature matching an
  existing table or disallow duplicate table signatures and
  return EFI_ACCESS_DENIED. On successful output, TableKey is
  initialized with a unique key. Its value may be used in a
  subsequent call to UninstallAcpiTable to remove an ACPI table.
  If an EFI application is running at the time of this call, the
  relevant EFI_CONFIGURATION_TABLE pointer to the RSDT is no
  longer considered valid.

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
  IN EFI_ACPI_TABLE_PROTOCOL  *This,
  IN VOID                     *AcpiTableBuffer,
  IN UINTN                    AcpiTableBufferSize,
  OUT UINTN                   *TableKey
  );
