/** @file
  Copy Firmware Volumes from SPI Flash to Memory, while skipping Free Space
  to improve boot times

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

**/
#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PerformanceLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Pi/PiFirmwareFile.h>
#include <Guid/FirmwareFileSystem2.h>
#include <Ppi/FirmwareVolumeShadowPpi.h>
#include <Library/PcdLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
#include <Ppi/SpiDma.h>
#endif
#include <UnitTest/PrivateMock/PrivateMock.h>

EFI_STATUS
EFIAPI
PeiFirmwareVolumeShadow (
  IN EFI_PHYSICAL_ADDRESS     FirmwareVolumeBase,
  IN VOID                     *Destination,
  IN UINTN                    DestinationLength
  );

static EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI  mFirmwareVolumeShadowPpi = {
  PeiFirmwareVolumeShadow
};

static EFI_PEI_PPI_DESCRIPTOR  mFirmwareVolumeShadowPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiFirmwareVolumeShadowPpiGuid,
  &mFirmwareVolumeShadowPpi
};

#define GET_OCCUPIED_SIZE(ActualSize, Alignment) \
  ((ActualSize) + (((Alignment) - ((ActualSize) & ((Alignment) - 1))) & ((Alignment) - 1)))

/**
  Returns the file state set by the highest zero bit in the State field

  @param ErasePolarity   Erase Polarity  as defined by EFI_FVB2_ERASE_POLARITY
                         in the Attributes field.
  @param FfsHeader       Pointer to FFS File Header.

  @retval EFI_FFS_FILE_STATE File state is set by the highest none zero bit
                             in the header State field.
**/
EFI_FFS_FILE_STATE
GetFileState (
  IN UINT8                ErasePolarity,
  IN EFI_FFS_FILE_HEADER  *FfsHeader
  )
{
  EFI_FFS_FILE_STATE  FileState;
  EFI_FFS_FILE_STATE  HighestBit;

  FileState = FfsHeader->State;
  if (ErasePolarity != 0) {
    FileState = (EFI_FFS_FILE_STATE) ~FileState;
  }

  //
  // Get file state set by its highest none zero bit.
  //
  HighestBit = 0x80;
  while (HighestBit != 0 && (HighestBit & FileState) == 0) {
    HighestBit >>= 1;
  }
  return HighestBit;
}

/**
  Returns the Ffs file header size based on it's type

  @param FfsHeader       Pointer to FFS File Header.

  @retval UINT32 returns the Ffs File Header size
**/
UINT32
GetFfsHeaderSize (
  IN EFI_FFS_FILE_HEADER  *FfsFileHeader
) {
  if (IS_FFS_FILE2 (FfsFileHeader)) {
    return sizeof (EFI_FFS_FILE_HEADER2);
  }
  return sizeof (EFI_FFS_FILE_HEADER);
}

/**
  Returns the Ffs file size based on it's FileState

  @param FfsHeader       Pointer to FFS File Header.
  @param FileState       File State as specified in UEFI PI Spec

  @retval UINT32 returns the Ffs file size
**/
UINT32
GetFfsFileSize (
  IN EFI_FFS_FILE_HEADER  *FfsFileHeader,
  IN UINT8 FileState
) {
  UINT32               FileLength;
  UINT32               FileOccupiedSize;

    if ((FileState == EFI_FILE_HEADER_CONSTRUCTION) ||
        (FileState == EFI_FILE_HEADER_INVALID)) {
        // If header is not complete or valid. File size is equal to header
        FileOccupiedSize = GetFfsHeaderSize(FfsFileHeader);
    } else if ((FileState == EFI_FILE_DELETED) ||
              (FileState == EFI_FILE_DATA_VALID) ||
              (FileState == EFI_FILE_MARKED_FOR_UPDATE) ) {
        if (IS_FFS_FILE2 (FfsFileHeader)) {
          FileLength = FFS_FILE2_SIZE (FfsFileHeader);
          ASSERT (FileLength > 0x00FFFFFF);
        } else {
          FileLength = FFS_FILE_SIZE (FfsFileHeader);
        }
        FileOccupiedSize = GET_OCCUPIED_SIZE (FileLength, 8);
    } else {
      return 0;
    }
    return FileOccupiedSize;
}

/**
  Copy Firmware Volumes from Flash to Permanent Memory while skipping
  Free Space

  @param[in]  FvBuffer            Fv Destination Pointer
  @param[in]  FvBase              Fv Source Pointer.
  @param[in]  FvBufferLen         Fv Destination Buffer Length

  @retval   EFI_DEVICE_ERROR        One of status bits reported an error
  @retval   EFI_SUCCESS             SPI DMA copied the data successfully
**/
EFI_STATUS
FreeSpaceSkipFvCopy (
  IN VOID    *FvBuffer,
  IN VOID    *FvBase,
  IN UINTN   FvBufferLen
) {
  EFI_FIRMWARE_VOLUME_EXT_HEADER  *FwVolExtHeader;
  EFI_FIRMWARE_VOLUME_HEADER      *FwVolHeader;
  EFI_FFS_FILE_HEADER             *FfsFileHeader;
  UINT32                          UsedSize;
  BOOLEAN                         FreeSpaceReached;
  UINT32                          FileOccupiedSize;
  UINT32                          FileOffset;
  UINT32                          HeaderSize;
  UINT8                           ErasePolarity;
  UINT8                           FileState;
  UINTN                           FvLength;
  EFI_STATUS                      Status;
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
  EFI_STATUS                      SpiDmaStatus;
  SPI_DMA_READ_PPI                *SpiDmaPpi;
#endif

  // Calculate Free Space
  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)FvBase;
  FvLength = (UINTN)FwVolHeader->FvLength;
  ASSERT (FvBufferLen>= FvLength);
  PERF_INMODULE_BEGIN ("FV SPI2Mem Copy");

  if ((FwVolHeader->Attributes & EFI_FVB2_ERASE_POLARITY) != 0) {
    ErasePolarity = 0xFF;
  } else {
    ErasePolarity = 0x00;
  }

#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
  SpiDmaStatus = PeiServicesLocatePpi (
                   &gSpiDmaReadPpiGuid,
                   0,
                   NULL,
                   (VOID **) &SpiDmaPpi
                   );
#endif
  DEBUG ((DEBUG_INFO, "FwVolHeader FwVolHeader->HeaderLength %04x\r\n", FwVolHeader->HeaderLength));
  if (FwVolHeader->ExtHeaderOffset != 0) {
    //
    // Searching for files starts on an 8 byte aligned boundary after the end of the Extended Header if it exists.
    //
    FwVolExtHeader = (EFI_FIRMWARE_VOLUME_EXT_HEADER *)((UINT8 *)FwVolHeader + FwVolHeader->ExtHeaderOffset);
    DEBUG ((DEBUG_INFO, "FwVolHeader FwVolHeader->ExtHeaderLength %04x\r\n", FwVolHeader->ExtHeaderOffset));
    FfsFileHeader  = (EFI_FFS_FILE_HEADER *)((UINT8 *)FwVolExtHeader + FwVolExtHeader->ExtHeaderSize);
  } else {
    FfsFileHeader = (EFI_FFS_FILE_HEADER *)((UINT8 *)FwVolHeader + FwVolHeader->HeaderLength);
  }
  FfsFileHeader = (EFI_FFS_FILE_HEADER *)ALIGN_POINTER (FfsFileHeader, 8);

  FileOffset = (UINT32)((UINT8 *)FfsFileHeader - (UINT8 *)FwVolHeader);
  CopyMem ((VOID *)((UINTN)FvBuffer), (UINT8 *)FwVolHeader, (UINTN)(FileOffset));
  DEBUG ((DEBUG_INFO, "Free space calculate FileOffset %08X Fvlength %08X \r\n", FileOffset, FvLength));
  FreeSpaceReached = FALSE;

  while ((FileOffset < (FvLength - sizeof (EFI_FFS_FILE_HEADER))) && (!FreeSpaceReached)) {

    ASSERT (FileOffset <= 0xFFFFFFFF);
    FileState = GetFileState (ErasePolarity, FfsFileHeader);
    FileOccupiedSize = GetFfsFileSize(FfsFileHeader, FileState);
    if(FileOccupiedSize == 0) {
      DEBUG ((DEBUG_INFO, "No Ffs file or zero size. End of FV volume %X\r\n", FileState));
      FreeSpaceReached = TRUE;
      break;
    }
    // Check if it is a pad file
    if (((FileState == EFI_FILE_DATA_VALID) || FileState == EFI_FILE_HEADER_CONSTRUCTION)
       && (FfsFileHeader->Type == EFI_FV_FILETYPE_FFS_PAD)) {
      DEBUG ((DEBUG_INFO, "PAD File reached\r\n"));
      HeaderSize = GetFfsHeaderSize(FfsFileHeader);
      // Copy Header as is
      CopyMem ((VOID *)((UINTN)FvBuffer + (UINTN)FileOffset), (UINT8 *)FfsFileHeader, (UINTN)HeaderSize);
      // SetMem remaining of PAD file based on Erase Polarity, to avoid copying Free Space from slower SPI Flash
      SetMem ( (VOID *)((UINTN)FvBuffer + (UINTN)FileOffset + (UINTN)HeaderSize), FileOccupiedSize - (UINTN)HeaderSize, ErasePolarity);
    } else {
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
      if (EFI_ERROR (SpiDmaStatus)) {
#endif
        CopyMem ((VOID *)((UINTN)FvBuffer+(UINTN)FileOffset), (UINT8 *)FfsFileHeader, (UINTN)FileOccupiedSize);
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
      } else {
        Status = SpiDmaPpi->SpiDmaRead((UINT8 *)FfsFileHeader, (VOID *)((UINTN)FvBuffer+(UINTN)FileOffset), (UINTN)FileOccupiedSize);
        if (Status != EFI_SUCCESS) {
          DEBUG ((DEBUG_ERROR, "PeiSpiDmaRead: Failed due to Error %r\n", Status));
          CopyMem ((VOID *)((UINTN)FvBuffer+(UINTN)FileOffset), (UINT8 *)FfsFileHeader, (UINTN)FileOccupiedSize);
        }
      }
#endif
    }
    //
    // Update pointers to point to the next FFS file header
    //
    FileOffset   += FileOccupiedSize;
    DEBUG ((DEBUG_INFO, "FFs file added Address-%08X Occupied Size- %08X, Filetype %02X\r\n", FileOffset - FileOccupiedSize, FileOccupiedSize, FfsFileHeader->Type));
    FfsFileHeader =  (EFI_FFS_FILE_HEADER *)((UINT8 *)FfsFileHeader + FileOccupiedSize);
  }
  UsedSize = (UINT32)((UINT8 *) FfsFileHeader - (UINT8 *)FvBase);
  DEBUG ((DEBUG_INFO, "FFsFileHeader %08X Base %08X Used Space (%08X) Length %08X\r\n", FfsFileHeader, FvBase, UsedSize, FvLength));

  if(((UINTN)FvLength - (UINTN)UsedSize)>0) {
    // If there is some Free Space at the end of the FV, Use SetMem to populate data based on ErasePolarity, to avoid copying Free Space from slower SPI Flash
    SetMem ( (VOID *)((UINT8 *)FvBuffer+UsedSize), ((UINTN)FvLength - (UINTN)UsedSize), ErasePolarity);
  }
  Status = EFI_SUCCESS;
  PERF_INMODULE_END ("FV SPI2Mem Copy");
  return Status;
}

/**
  Copy FV to Destination.  Length of copy is FV length from FV Header.

  @param[in]  FirmwareVolumeBase  Base address of FV to shadow. Length of FV
                                  is in FV Header.
  @param[in]  Destination         Pointer to the Buffer in system memory to
                                  shadow FV.
  @param[in]  DestinationLength   Size of Destination buffer in bytes.

  @retval EFI_SUCCESS            Shadow complete
  @retval EFI_INVALID_PARAMETER  Destination is NULL
  @retval EFI_INVALID_PARAMETER  DestinationLength = 0.
  @retval EFI_INVALID_PARAMETER  FV does not have valid FV Header.
  @retval EFI_INVALID_PARAMETER  FV overlaps Destination.
  @retval EFI_INVALID_PARAMETER  Destination + DestinationLength rolls over 4GB
                                 for 32-bit or 64-bit rollover.
  @retval EFI_BUFFER_TOO_SMALL   DestinationLength less than FV length from FV
                                 Header.
  @retval EFI_UNSUPPORTED        FirmwareVolumeBase to FVBase + FVLength does
                                 not support shadow.  Caller should fallback to
                                 CopyMem().

**/
EFI_STATUS
EFIAPI
PeiFirmwareVolumeShadow (
  IN EFI_PHYSICAL_ADDRESS     FirmwareVolumeBase,
  IN VOID                     *Destination,
  IN UINTN                    DestinationLength
  )
{
  EFI_FIRMWARE_VOLUME_HEADER   *FwVolHeader;
  UINTN                FvLength;

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FirmwareVolumeBase;
  FvLength = (UINTN)FwVolHeader->FvLength;
  if (FvLength == 0 ||
      Destination == NULL ||
      DestinationLength >= (~(UINT32)0) ||
      ((~(UINT32)0)- DestinationLength) <= (UINTN)Destination ||
      (((UINTN)Destination > (UINTN)FirmwareVolumeBase) && ((UINTN)Destination < (UINTN)FirmwareVolumeBase + FvLength))
  ) {
    DEBUG ((DEBUG_ERROR,
            "FreeSpace FV Shadow Invalid Parameter Destination %08x Invalid Pointer %08x FvLength %x\n",
            (UINTN)Destination,
            ((~(UINT32)0)- DestinationLength),
             FvLength));
    return EFI_INVALID_PARAMETER;
  }
  if (FvLength > DestinationLength) {
    DEBUG ((DEBUG_ERROR, "FreeSpace FV Shadow : Buffer is too small\n"));
    return EFI_BUFFER_TOO_SMALL;
  }
  FreeSpaceSkipFvCopy(Destination, (VOID *)((UINTN)FirmwareVolumeBase), FvLength);
  return EFI_SUCCESS;
}

/**
  This function Initializes FreeSpaceSkip services

  @retval EFI_STATUS  Results of the installation of the SPI services
**/
EFI_STATUS
EFIAPI
FreeSpaceSkipEntryPoint (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS        OldFvShadowStatus;
  EFI_STATUS        Status;
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
  SPI_DMA_READ_PPI  *SpiDmaPpi;
#endif
  EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI *FvShadowPpi;
  EFI_PEI_PPI_DESCRIPTOR               *OldFvShadowPpiDesc;

  OldFvShadowStatus = PeiServicesLocatePpi (
                        &gEdkiiPeiFirmwareVolumeShadowPpiGuid,
                        0,
                        &OldFvShadowPpiDesc,
                        (VOID **) &FvShadowPpi
                        );
  if (EFI_ERROR (OldFvShadowStatus)) {
    DEBUG ((DEBUG_INFO, "FreeSpaceSkip: Installing FvShadow\n"));
    Status = PeiServicesInstallPpi (&mFirmwareVolumeShadowPpiList);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "FreeSpaceSkip: Couldn't install FvShadow"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  } else {
    DEBUG ((DEBUG_INFO, "FreeSpaceSkip: FvShadow already installed by SPI DMA, Replace with FreeSpaceSkip\n"));
    Status = PeiServicesReInstallPpi(OldFvShadowPpiDesc, &mFirmwareVolumeShadowPpiList);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_INFO, "FreeSpaceSkip: Couldn't replace SPIDMA's FvShadow with it's own"));
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }
#if FixedPcdGetBool(PcdSpiDmaEnable) == 1
  Status = PeiServicesLocatePpi (
             &gSpiDmaReadPpiGuid,
             0,
             NULL,
             (VOID **) &SpiDmaPpi
             );

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Couldn't find SPI DMA\n"));
    if (!EFI_ERROR (OldFvShadowStatus)) {
      DEBUG ((DEBUG_ERROR, "Unexpected!! Couldn't find SPI DMA Ppi. But SPI DMA's FVShadow was uninstalled by FreeSpaceSkip\n"));
      ASSERT_EFI_ERROR (Status);
    }
  } else {
    DEBUG ((DEBUG_INFO, "SpiDmaReadPpi already installed\n"));
  }
#endif
  return Status;
}
