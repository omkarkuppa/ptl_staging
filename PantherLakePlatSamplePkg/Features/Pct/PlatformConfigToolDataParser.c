/** @file
  Source code for Platform Configuration Tool in Pre/Post Mem.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#include "PlatformConfigToolInit.h"
#include "PlatformConfigToolStructures.h"

CHAR8 PreMemStr[] = "Pre-Mem";
CHAR8 PostMemStr[] = "Post-Mem";

/**

  PctPeimDiscoveryPpiCallback

**/
EFI_STATUS
EFIAPI
PctPeimDiscoveryPpiCallback (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  );

static EFI_PEI_NOTIFY_DESCRIPTOR mPctPeimNotifyCallback = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiPeiMasterBootModePpiGuid,
  (EFI_PEIM_NOTIFY_ENTRY_POINT)PctPeimDiscoveryPpiCallback
};

/**
  GetPctDataType

  Get enumerated PCT data type

  @param    BoardPointer         Pointer to EFI_GUID in the data header
  @param    BoardInfoEnd         Pointer to UINTN to store PCT data typer

  @retval   Status               EFI_SUCCESS - Found PCT data
                                 EFI_NOT_FOUND - Didn't find PCT data

**/
EFI_STATUS
GetPctDataType (
  IN      EFI_GUID  *BoardPointer,
  IN  OUT UINTN     *PctDataType
  )
{
  UINTN       Index;
  EFI_STATUS  Status;

  Status = EFI_NOT_FOUND;

  for (Index = 0;
        (Index < PctConfigDataTypeGuidsCount)  &&
        (CompareGuid (BoardPointer, PctConfigDataTypeGuids[Index].Guid) == FALSE);
        Index++
        );

  if (Index < PctConfigDataTypeGuidsCount) {
    *PctDataType = PctConfigDataTypeGuids[Index].PctDataType;
    Status = EFI_SUCCESS;
  }

  return Status;
}

/**
  HandlePrePostMemGpios

  Get all data associated with this board.

  @param    DataPointer          Start of header of data, roving pointer

  @param    PctDataType          Data type

  @retval   Status               EFI_SUCCESS - Found PCT data
                                 EFI_NOT_FOUND - Didn't find PCT data
                                 EFI_INVALID_PARAMETER - SPCD not big enough for data

**/
EFI_STATUS
HandlePrePostMemGpios (
  IN  OUT UINT8          **DataPointer,
  IN      UINTN          PctDataType
  )
{
  UINTN                  Count;
  EFI_STATUS             Status;
  UINTN                  DataSize;
  GPIOV2_INIT_CONFIG     *SpcdPointer;
  CHAR8                  *ErrString;

  Status = EFI_SUCCESS;
  if (PctDataType == PctPreMemGpio) {
    ErrString = PreMemStr;
  } else {
    ErrString = PostMemStr;
  }

  Count = ((PCT_GPIO_BLOCK_HDR *)*DataPointer)->Count;
  *DataPointer += sizeof (PCT_GPIO_BLOCK_HDR);
  if (Count > 0) {
    DataSize = (Count * sizeof (GPIOV2_INIT_CONFIG)) + sizeof (GPIOV2_PAD);
    //
    // Transfer data to structured PCD
    //
    if (PctDataType == PctPreMemGpio) {
      Status = PcdSetExPtrS (
                 &gBoardModuleTokenSpaceGuid,
                 PcdBoardGpioTablePreMem,
                 &DataSize,
                 *DataPointer
                 );
      SpcdPointer = PcdGetPtr (PcdBoardGpioTablePreMem);
    } else {
      Status = PcdSetExPtrS (
                 &gBoardModuleTokenSpaceGuid,
                 PcdBoardGpioTable,
                 &DataSize,
                 *DataPointer
                 );
      SpcdPointer = PcdGetPtr (PcdBoardGpioTable);
    }
    if (Status == EFI_SUCCESS) {
      //
      // End GPIOs with a zero GpioPad
      //
      SpcdPointer = (GPIOV2_INIT_CONFIG *)((UINT8 *)SpcdPointer + (Count * sizeof (GPIOV2_INIT_CONFIG)));
      SpcdPointer->GpioPad = 0;
      OutGpios ((UINT8 *)*DataPointer, Count);
      OutGpiosVerbose ((UINT8 *)*DataPointer, Count);
    } else {
      DEBUG ((DEBUG_ERROR, "%a: Not enough space in %a GPIO SPCD\n", __FUNCTION__, ErrString));
    }
  } else {
    Status = EFI_NOT_FOUND;
    DEBUG ((DEBUG_ERROR, "%a: PCT %a GPIO count == 0\n", __FUNCTION__, ErrString));
  }

  //
  // Update data pointer
  //
  *DataPointer += Count * sizeof (GPIOV2_INIT_CONFIG);

  return Status;
}

/**
  RetrieveBoardData

  Get all data associated with this board.

  @param    DataPointer      Pointer to PCT DataType structure that contains
                               PCT data type guid
  @param    BoardInfoEnd     End address board data

  @retval   Status           EFI_SUCCESS - Found PCT data
                             EFI_NOT_FOUND - Didn't find PCT data

**/
EFI_STATUS
RetrieveBoardData (
  IN      UINT8   *DataPointer,
  IN      UINT8   *BoardSectionInfoEnd
  )
{
  UINT8           *DataStart;
  UINTN           DataSize;
  EFI_STATUS      Status;
  UINTN           Index;
  UINTN           PctDataType;
  UINT16          Count;

  ASSERT (DataPointer != NULL);
  ASSERT (BoardSectionInfoEnd != NULL);
  ASSERT ((BoardSectionInfoEnd - DataPointer) > 0);

  //
  // Initialize parameters
  //
  if ((DataPointer == NULL) ||
      (BoardSectionInfoEnd == NULL) ||
      ((BoardSectionInfoEnd - DataPointer) == 0)
      ) {
    Status = EFI_INVALID_PARAMETER;
  } else {
    Status = EFI_SUCCESS;
  }

  //
  // Collect all PCT data for this board
  //
  while ((Status == EFI_SUCCESS) && (DataPointer < BoardSectionInfoEnd)) {
    DataStart = DataPointer;
    DEBUG ((DEBUG_INFO, "PCT_DBG: DataPointer = %p, BoardSectionInfoEnd = %p\n", DataPointer, BoardSectionInfoEnd));
    //
    // Retrieve data type for this data set
    //
    Status = GetPctDataType ((EFI_GUID *)DataPointer, &PctDataType);
    if (Status == EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "PCT_DBG: Data Type = %d\n", PctDataType));
      switch (PctDataType) {
        //
        // Find correct board information for Pre Mem GPIOs
        //
        case PctPreMemGpio:
          DEBUG ((DEBUG_INFO, "PCT_DBG: Handling Premem GPIOs, PctRevision %d.%d.%d\n",
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[0],
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[1],
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[2]
            ));
          Status = HandlePrePostMemGpios (
                     &DataPointer,
                     PctDataType
                     );
        break;
        //
        // Find correct board information for Pre Mem GPIOs
        //
        case PctPostMemGpio:
          DEBUG ((DEBUG_INFO, "PCT_DBG: Handling Postmem GPIOs, PctRevision %d.%d.%d\n",
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[0],
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[1],
            (((PCT_ACPI_BLOCK_HDR *)DataPointer))->PctRevision[2]
            ));
          Status = HandlePrePostMemGpios (
                     &DataPointer,
                     PctDataType
                     );
        break;
        //
        // Find correct board for ACPI information
        //
          case PctAcpi:
            DEBUG ((DEBUG_INFO, "PCT_DBG: Handling ACPI, PctRevision %d.%d.%d\n",
              ((PCT_ACPI_BLOCK_HDR *)DataPointer)->PctRevision[0],
              ((PCT_ACPI_BLOCK_HDR *)DataPointer)->PctRevision[1],
              ((PCT_ACPI_BLOCK_HDR *)DataPointer)->PctRevision[2]
              ));
            Count = ((PCT_ACPI_BLOCK_HDR *)DataPointer)->Count;
            if (Count > 0) {
              DataPointer += sizeof (PCT_ACPI_BLOCK_HDR);
              //
              // Compute size of this data block
              //
              DataSize = 0;
              for (Index = 0; Index < Count; Index++)  {
                DataSize += ((EFI_ACPI_SDT_HEADER *)DataPointer)->Length;
                DataPointer += ((EFI_ACPI_SDT_HEADER *)DataPointer)->Length;
              }
              DEBUG ((DEBUG_INFO, "PCT_DBG: ACPI guid = %g, DataStart = %p, DataPointer = %p, Count = %d, DataSize = 0x%x\n",
                (&((PCT_ACPI_BLOCK_HDR *)DataStart)->Guid),
                DataStart,
                DataPointer,
                Count,
                DataSize + sizeof (PCT_ACPI_BLOCK_HDR)
                ));
              //
              // ACPI out to structured PCD
              //
              Status = PcdSetExPtrS (
                         &gBoardModuleTokenSpaceGuid,
                         PcdBoardAcpiData,
                         &DataSize,
                         DataStart + sizeof (PCT_ACPI_BLOCK_HDR)
                         );
              if (Status != EFI_SUCCESS) {
                DEBUG ((DEBUG_ERROR, "%a: Not enough space in ACPI SPCD\n", __FUNCTION__));
              }
            } else {  // Count = 0
              Status = EFI_NOT_FOUND;
              DEBUG ((DEBUG_ERROR, "%a: PCT ACPI SSDT count == 0\n", __FUNCTION__));
            }
          break;
        default:
          break;
      }  // switch (PctDataType)
    } else {  // Data not found
      Status = EFI_NOT_FOUND;
      DEBUG ((DEBUG_ERROR, "%a: Unrecognized PCT data type\n", __FUNCTION__));
    }
  } // While collecting data

  return Status;
}

/**
  PlatformConfigToolEntry

  Notify the presence of the Platform Configuration Tool
  presence.

  @retval   Status          EFI_SUCCESS - Successful notify
**/
EFI_STATUS
EFIAPI
PlatformConfigToolEntry (
  IN  EFI_PEI_FILE_HANDLE     DriverFileHandle,
  IN  CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "PCT_DBG: Entering %a\n", __FUNCTION__));

  Status = PeiServicesNotifyPpi (&mPctPeimNotifyCallback);

  DEBUG ((DEBUG_INFO, "PCT_DBG %a: PeiServicesNotifyPpi() Status = %r\n", __FUNCTION__, Status));
  DEBUG ((DEBUG_INFO, "PCT_DBG: Exiting %a\n", __FUNCTION__));

  return EFI_SUCCESS;
}

/**
  PctPeimDiscovereyPpiCallback

  Checks for the presence of Platform Configuration Tool
  configuration data in the FVs and moves that data to SPCDs.

@retval   Status          EFI_SUCCESS - found board data
**/
EFI_STATUS
EFIAPI
PctPeimDiscoveryPpiCallback (
  IN CONST EFI_PEI_SERVICES      **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR   *NotifyDescriptor,
  IN VOID                        *Ppi
  )
{
  EFI_STATUS                    PctStatus;
  EFI_STATUS                    FvStatus;
  UINTN                         FvInstance;
  UINTN                         FsInstance;
  EFI_PEI_FV_HANDLE             VolumeHandle;
  EFI_PEI_FILE_HANDLE           FileHandle;
  EFI_COMMON_SECTION_HEADER     *SectionHeaderPointer;
  BOOLEAN                       BoardIdMatch;;
  UINTN                         Index;
  UINTN                         PctFilesFound;
  UINT8                         *BoardSectionPointer;
  UINT8                         *BoardSectionInfoEnd;
  UINT16                        BoardId;
  UINT32                        AuthenticationStatus;
  UINT32                        SectionSize;

  //
  // Initialize variables
  //
  FvInstance = 0;
  PctStatus = EFI_SUCCESS;
  FvStatus = EFI_SUCCESS;
  PctFilesFound = 0;

  //
  // Get board ID
  //
  BoardId = PcdGet16 (PcdBoardId);
  if ((BoardId == BoardIdReserved) || (BoardId == BoardIdUnknown1)) {
    DEBUG ((DEBUG_INFO, "PCT_DBG: Exiting %a\n", __FUNCTION__));
    return EFI_SUCCESS;
  }

  DEBUG ((DEBUG_INFO, "PCT_DBG: Entering %a\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "PCT_DBG %a: PCD Board ID = 0x%x\n", __FUNCTION__, BoardId));

  do {
    //
    // Search for firmware volume
    //
    FvStatus = PeiServicesFfsFindNextVolume (FvInstance++, &VolumeHandle);
    DEBUG ((DEBUG_INFO, "PCT_DBG %a: Found a volume? %r\n", __FUNCTION__, FvStatus));
    if (FvStatus == EFI_SUCCESS) {
      //
      // Search for PCT files
      //
      for (Index = 0; Index < PctConfigFileCount; Index++) {
        PctStatus = PeiServicesFfsFindFileByName (
                      PctConfigFileGuids[Index],
                      VolumeHandle,
                      &FileHandle
                      );
        DEBUG ((DEBUG_INFO, "PCT_DBG %a: Found a pct file? %r, guid = %g\n", __FUNCTION__, PctStatus, PctConfigFileGuids[Index]));
        if (PctStatus == EFI_SUCCESS) {
          //
          // Read sections, Find board match
          //
          BoardIdMatch = FALSE;
          FsInstance = 0;
          do {
            PctStatus = PeiServicesFfsFindSectionData3 (
                          EFI_SECTION_RAW,
                          FsInstance++,
                          FileHandle,
                          (VOID **)&BoardSectionPointer,
                          &AuthenticationStatus
                          );
            if (PctStatus == EFI_SUCCESS) {
              DEBUG ((DEBUG_INFO, "PCT_DBG %a: PCT Board id = 0x%x\n", __FUNCTION__, ((PCT_SECTION_HEADER *)BoardSectionPointer)->BoardId));
              if (((PCT_SECTION_HEADER *)BoardSectionPointer)->BoardId == BoardId) {
                //
                // Board match
                //
                SectionHeaderPointer = (EFI_COMMON_SECTION_HEADER *)(BoardSectionPointer - sizeof (EFI_COMMON_SECTION_HEADER));
                SectionSize = SECTION_SIZE (SectionHeaderPointer);
                BoardSectionInfoEnd = (UINT8 *)(((UINT8 *)SectionHeaderPointer) + SectionSize);
                if (SectionSize > (sizeof (EFI_COMMON_SECTION_HEADER) + sizeof (PCT_SECTION_HEADER))) {
                  //
                  // PCT data found
                  //
                  BoardIdMatch = TRUE;
                  BoardSectionPointer += sizeof (PCT_SECTION_HEADER);
                  PctStatus = RetrieveBoardData (BoardSectionPointer, BoardSectionInfoEnd);
                  if (PctStatus == EFI_SUCCESS) {
                    ++PctFilesFound;
                  }
                }  // If board ID match
                DEBUG ((DEBUG_INFO, "PCT_DBG %a: SectionHeaderPointer = %p, BoardSectionPointer = %p, ",
                  __FUNCTION__, SectionHeaderPointer, BoardSectionPointer));
                DEBUG ((DEBUG_INFO, "BoardSectionInfoEnd = %p, section size = 0x%x\n",
                  BoardSectionInfoEnd, SectionSize));
                OutData ((UINT8 *)SectionHeaderPointer, (UINT16)SectionSize);
              }  // Board ID check
            }  // Couldn't find next file section
          } while ((BoardIdMatch == FALSE) && (PctStatus == EFI_SUCCESS));  //Board ID match search
        }  // Couldn't find PCT files
      }  // Search for PCT files
    }  // Couldn't find next volume
    DEBUG ((DEBUG_INFO, "PCT_DBG %a: FvStatus = %r, PctFilesFound = %d, PctConfigFileCount = %d\n",
      __FUNCTION__, FvStatus, PctFilesFound, PctConfigFileCount));
  } while ((FvStatus == EFI_SUCCESS) && (PctFilesFound < PctConfigFileCount));  // FV search

  //
  // Assess status
  //
  if (PctFilesFound >= 1) {
    PcdSetBoolS (PcdDisableVpdGpioTable, TRUE);
    DEBUG ((DEBUG_INFO, "PCT_DBG %a: Disabling Vpd support !!!\n", __FUNCTION__));
  }

  DEBUG ((DEBUG_INFO, "PCT_DBG: Exiting %a\n", __FUNCTION__));

  return EFI_SUCCESS;
}


