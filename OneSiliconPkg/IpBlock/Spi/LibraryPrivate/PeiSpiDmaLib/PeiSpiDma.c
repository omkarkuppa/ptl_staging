/** @file
  SPI DMA enabling library

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

@par Specification Reference:
**/


#include <Library/PeiSpiDmaLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PrintLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <Pi/PiFirmwareVolume.h>
#include <Protocol/Spi.h>
#include <Ppi/SpiDma.h>
#include <Ppi/IoMmu.h>
#include <Ppi/FirmwareVolumeShadowPpi.h>
#include <Library/PchInfoLib.h>
#include <Library/SpiAccessLib.h>
#include <Library/PreSiliconEnvDetectLib.h>



#define SPI_DMA_ALIGNMENT            SIZE_1KB
#define MAX_DMA_TRANSFER_SIZE        SIZE_2MB
#define MAX_DMA_TRANSFER_SIZE_IN_KB  ((MAX_DMA_TRANSFER_SIZE/SIZE_1KB))

EFI_STATUS
EFIAPI
PeiFirmwareVolumeShadow (
  IN EFI_PHYSICAL_ADDRESS     FirmwareVolumeBase,
  IN VOID                     *Destination,
  IN UINTN                    DestinationLength
  );

EFI_STATUS
EFIAPI
PeiSpiDmaRead (
  IN     VOID   *FlashAddress,
  IN     VOID   *Destination,
  IN     UINTN  ReadSize
  );

BOOLEAN
IsSpiDmaSupported (
  VOID
  );

EFI_STATUS
StartSpiDma (
  IN  VOID                *BufferBaseAddress,
  IN  UINTN               BufferSize,
  IN  UINT32              BiosRegionOffset,
  IN  FLASH_REGION_TYPE   FlashRegion
  );

EFI_STATUS
EFIAPI
LockSpiDmaCallback (
  IN EFI_PEI_SERVICES           **PeiServices,
  IN EFI_PEI_NOTIFY_DESCRIPTOR  *NotifyDescriptor,
  IN VOID                       *Ppi
  );

EFI_STATUS
LockSpiDma (
  UINT64 SpiPciCfg
  );

STATIC EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI  mFirmwareVolumeShadowPpi = {
  PeiFirmwareVolumeShadow
};

STATIC EFI_PEI_PPI_DESCRIPTOR  mFirmwareVolumeShadowPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEdkiiPeiFirmwareVolumeShadowPpiGuid,
  &mFirmwareVolumeShadowPpi
};

STATIC SPI_DMA_READ_PPI  mSpiDmaReadPpi = {
  PeiSpiDmaRead
};

STATIC EFI_PEI_PPI_DESCRIPTOR mSpiDmaReadPpiList = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gSpiDmaReadPpiGuid,
  &mSpiDmaReadPpi
};

STATIC EFI_PEI_NOTIFY_DESCRIPTOR mLockSpiDmaNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_CALLBACK | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gEfiEndOfPeiSignalPpiGuid,
  LockSpiDmaCallback
};

#define LINE_BUFFER        51  //To construct a line needs 51 chars.
#define END_OF_LINE_OFFSET 47
/**
  For serial debugger used, it will show the buffer message line by line to serial console.

  @param[in] Message              the address point of buffer message
  @param[in] Length               message length

**/
STATIC
VOID
ShowBuffer (
  IN  UINT8                       *Message,
  IN  UINT32                      Length
  )
{
  DEBUG_CODE_BEGIN ();
  UINT32                          Index;
  UINT32                          Offset;
  CHAR16                          Buffer [LINE_BUFFER];

  Index  = 0;
  Offset = 0;
  ZeroMem (Buffer, sizeof (Buffer));

  while (Length-- > 0) {
    //
    // Get the corresponding offset value from the index of buffer message.
    //
    Offset = ((Index & 0x0F) > 7) ? (((Index & 0x0F) * 3) + 2) : ((Index & 0x0F) * 3);

    //
    // Print "- " at the half of a line increases the readability of debug message.
    //
    if ((Index & 0x0F) == 0x08) {
      UnicodeSPrint (&Buffer [24], 3 * sizeof (CHAR16), L"- ");
    }

    //
    // Collect the data of buffer message.
    //
    UnicodeSPrint (&Buffer[Offset], 4 * sizeof (CHAR16), L"%02x ", Message [Index]);

    //
    // A line contains 16 bytes of buffer message. If a line is complete, it will be shown through DEBUG macro.
    //
    if (Offset == END_OF_LINE_OFFSET) {
      DEBUG ((DEBUG_INFO, "%04x: %s\n", (Index & 0xFFF0), Buffer));
    }

    Index++;
  }

  //
  // If a line isn't complete, show the remaining data.
  //
  if (Offset != END_OF_LINE_OFFSET) {
    DEBUG ((DEBUG_INFO, "%04x: %s\n", (Index & 0xFFF0), Buffer));
  }
  DEBUG_CODE_END ();
  return;
}

/**
  Locate IOMMU PPI.

  @return Pointer to IOMMU PPI.

**/
STATIC
EDKII_IOMMU_PPI *
LocateIoMmuPpi (
  VOID
  )
{
  EFI_STATUS         Status;
  EDKII_IOMMU_PPI       *IoMmuPpi;

  Status = PeiServicesLocatePpi (
             &gEdkiiIoMmuPpiGuid,
             0,
             NULL,
             (VOID **) &IoMmuPpi
             );

  if (!EFI_ERROR (Status) && (IoMmuPpi != NULL)) {
    DEBUG ((DEBUG_INFO, "gEdkiiIoMmuPpiGuid installed\n"));
    return IoMmuPpi;
  }
  DEBUG ((DEBUG_INFO, "gEdkiiIoMmuPpiGuid not located %d %x \n", Status, IoMmuPpi));
  return NULL;
}

/**
  Allocates pages that are suitable for an OperationBusMasterCommonBuffer or
  OperationBusMasterCommonBuffer64 mapping.

  @param  IoMmuPpi              Pointer to IOMMU PPI.
  @param  Pages                 The number of pages to allocate.
  @param  HostAddress           A pointer to store the base system memory address of the
                                allocated range.
  @param  DeviceAddress         The resulting map address for the bus master PCI controller to use to
                                access the hosts HostAddress.
  @param  Mapping               A resulting value to pass to Unmap().

  @retval EFI_SUCCESS           The requested memory pages were allocated.
  @retval EFI_NOT_FOUND         IoMmuPpi Not Found
  @retval EFI_UNSUPPORTED       Attributes is unsupported. The only legal attribute bits are
                                MEMORY_WRITE_COMBINE and MEMORY_CACHED.
  @retval EFI_INVALID_PARAMETER One or more parameters are invalid.
  @retval EFI_OUT_OF_RESOURCES  The memory pages could not be allocated.

**/
STATIC
EFI_STATUS
IoMmuAllocateBuffer (
  OUT EDKII_IOMMU_PPI       **IoMmuPpi,
  IN UINTN                  Pages,
  OUT VOID                  **HostAddress,
  OUT EFI_PHYSICAL_ADDRESS  *DeviceAddress,
  OUT VOID                  **Mapping
  )
{
  EFI_STATUS            Status;
  UINTN                 NumberOfBytes;

  DEBUG ((DEBUG_INFO, "%a () enter\n", __FUNCTION__));

  *HostAddress   = NULL;
  *Mapping       = NULL;
  DeviceAddress = 0;
  *IoMmuPpi   = LocateIoMmuPpi ();

  if (*IoMmuPpi != NULL) {
    Status = (*IoMmuPpi)->AllocateBuffer (
                      *IoMmuPpi,
                      EfiBootServicesData,
                      Pages,
                      HostAddress,
                      0
                      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HsPhyInit: IoMmuPpi AllocateBuffer failure, %r\n", Status));
      goto Exit;
    }

    NumberOfBytes = EFI_PAGES_TO_SIZE (Pages);
    Status = (*IoMmuPpi)->Map (
                      *IoMmuPpi,
                      EdkiiIoMmuOperationBusMasterCommonBuffer,
                      *HostAddress,
                      &NumberOfBytes,
                      (EFI_PHYSICAL_ADDRESS   *)&DeviceAddress,
                      Mapping
                      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HsPhyInit: IoMmuPpi Map failure, %r\n", Status));
      goto Exit;
    }

    Status = (*IoMmuPpi)->SetAttribute (
                      *IoMmuPpi,
                      *Mapping,
                      EDKII_IOMMU_ACCESS_READ | EDKII_IOMMU_ACCESS_WRITE
                      );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "HsPhyInit: IoMmuPpi SetAttribute failure, %r\n", Status));
      goto Exit;
    }
  } else {
    return EFI_NOT_FOUND;
  }

Exit:
  if (EFI_ERROR (Status)) {
    if (*Mapping != NULL) {
      (*IoMmuPpi)->SetAttribute (*IoMmuPpi, *Mapping, 0);
      (*IoMmuPpi)->Unmap (*IoMmuPpi, *Mapping);
    }
    if (*HostAddress != NULL) {
      (*IoMmuPpi)->FreeBuffer (*IoMmuPpi, Pages, *HostAddress);
    }
  }
  DEBUG ((DEBUG_INFO, "%a () exit\n", __FUNCTION__));
  return Status;
}

/**
  Free Vtd Allocated Buffer

  @param  IoMmuPpi              Pointer to IOMMU PPI.
  @param  Buffer                Pointer to store the base system memory address of the
                                allocated range.
  @param  Pages                 The number of pages to allocate.
  @param  Mapping               A resulting value to pass to Unmap().

  @retval   EFI_SUCCESS             SPI DMA copied the data successfully

**/
STATIC
EFI_STATUS
FreeVtdCopyBuffer (
  IN EDKII_IOMMU_PPI  *IoMmuPpi,
  OUT    VOID         *Buffer,
  IN     UINTN        Pages,
  IN     VOID         *Mapping
) {
  if (IoMmuPpi != NULL) {
    if (Mapping != NULL) {
      IoMmuPpi->SetAttribute (IoMmuPpi, Mapping, 0);
      IoMmuPpi->Unmap (IoMmuPpi, Mapping);
    }
    if (Buffer != 0) {
      IoMmuPpi->FreeBuffer (IoMmuPpi, Pages, Buffer);
      DEBUG((DEBUG_INFO,"Vtd DMA Buffer is Freed\n"));
      return EFI_SUCCESS;
    }
  }
  return EFI_NOT_FOUND;
}

/**
  This function copies data using SPI DMA from flash to memory

  @param FlashAddress       -  Pointer to Flash Address to copy from.
  @param Destination        -  Pointer in Memory we are copying to.
  @param ReadSize           -  Number of bytes to be read.

  @retval   EFI_INVALID_PARAMETER   Invalid BIOS region chosen
  @retval   EFI_ACCESS_DENIED       SPI controller not available or DMA operation
                                    already ongoing
  @retval   EFI_TIMEOUT             Timed out on waiting on DMA pointer reset or
                                    DMA operation timed out
  @retval   EFI_UNSUPPORTED         SPI DMA is unsupported
  @retval   EFI_DEVICE_ERROR        One of status bits reported an error
  @retval   EFI_SUCCESS             SPI DMA copied the data successfully
**/
EFI_STATUS
EFIAPI
PeiSpiDmaRead (
  IN     VOID   *FlashAddress,
  IN     VOID   *Destination,
  IN     UINTN  ReadSize
  )
{
  UINTN            BiosRegionOffsetFromTop;
  UINTN            KbChunks;
  UINTN            Remainder;
  EFI_STATUS       Status;
  EFI_STATUS       IsVtdAllocated;
  UINTN            Alignment;
  UINTN            ToRead;
  VOID             *AlignedTempBuffer;
  UINTN            BlocksCopied;
  UINTN            KbChunksRemaining;
  UINTN            DmaTransferSize;
  UINT32           Pages;
  EDKII_IOMMU_PPI  *IoMmuPpi;
  UINT64           DeviceAddress;
  VOID             *BufferMapping;

  Alignment = 0;
  ToRead = ReadSize;

  if ((UINTN)FlashAddress == (UINTN)Destination) {
    return EFI_SUCCESS;
  }

  if (SpiIsTopSwapEnabled()) {
    DEBUG ((DEBUG_INFO, " Top Swap is Enabled, SPI DMA is not supported. Default to Direct Reads\n"));
    return EFI_UNSUPPORTED;
  }

  if (ToRead >= SIZE_1KB && ((UINTN)FlashAddress >= PcdGet32(PcdBiosAreaBaseAddress))) {
    DEBUG ((DEBUG_INFO, "%a ReadSize   %08X\n", __FUNCTION__, ReadSize));
    DEBUG ((DEBUG_INFO, "%a FlashAddress %08X\n", __FUNCTION__, FlashAddress));
    DEBUG ((DEBUG_INFO, "%a Destination %08X\n", __FUNCTION__, Destination));
    DEBUG ((DEBUG_INFO, "%a ToRead     %08X\n", __FUNCTION__, ToRead));

    BiosRegionOffsetFromTop = (UINTN) 0xFFFFFFFF - (UINTN)FlashAddress + 1;
    DEBUG ((DEBUG_INFO, "%a BiosRegionOffsetFromTop %08X\n", __FUNCTION__, BiosRegionOffsetFromTop));

    Alignment = BiosRegionOffsetFromTop & (SPI_DMA_ALIGNMENT - 1); // Check if Address is aligned
    if (Alignment != 0) {
      DEBUG ((DEBUG_INFO, "%a not aligned for 1kB access. Alignment %0X\n", __FUNCTION__, Alignment));
      CopyMem (Destination, FlashAddress, Alignment);

      ToRead -= Alignment;
      BiosRegionOffsetFromTop -= Alignment;
      DEBUG ((DEBUG_INFO, "%a BiosRegionOffsetFromTop %08X\n", __FUNCTION__, BiosRegionOffsetFromTop));
      DEBUG ((DEBUG_INFO, "%a ToRead %08X\n", __FUNCTION__, ToRead));
    }

    KbChunks = ToRead / SIZE_1KB; // Number of 1kB chunks
    Remainder = ToRead % SIZE_1KB; // Remainder of data to read after done with chunks

    if (PchStepping () < PCH_B0) {
      // DMA has warm reset limitation, using dma to transfer in 5KB multiples
      // to avoid pointers from being misaligned on warm reset.
      Remainder = Remainder + ((KbChunks % 5) * SIZE_1KB);
      KbChunks = KbChunks / 5;
      KbChunks = KbChunks * 5;
    }

    DEBUG ((DEBUG_INFO, "%a KbChunks %08X\n", __FUNCTION__, KbChunks));
    DEBUG ((DEBUG_INFO, "%a Remainder %08X\n", __FUNCTION__, Remainder));

    if (KbChunks > 0) {

      Pages = (UINT32)EFI_SIZE_TO_PAGES (SIZE_1KB * (KbChunks));

      IsVtdAllocated = IoMmuAllocateBuffer(
                        &IoMmuPpi,
                        Pages,
                        &AlignedTempBuffer,
                        &DeviceAddress,
                        &BufferMapping
                        );
      if (EFI_ERROR (IsVtdAllocated)) {
        // If Vtd DMA Protection is disabled, we need to allocate aligned buffer
        if (IsVtdAllocated == EFI_NOT_FOUND) {
          DEBUG ((DEBUG_INFO, " Vtd is disabled\n"));
          // No need to allocated 1KB aligned buffer if already aligned
          if (Alignment != 0) {
            AlignedTempBuffer = AllocateAlignedPages (Pages, SIZE_1KB);
          } else {
            AlignedTempBuffer = Destination;
          }
        } else {
          DEBUG ((DEBUG_INFO, " Failure to allocate Vtd IoMmu buffer and Vtd is enabled. Need to fallback %r\n", IsVtdAllocated));
          return IsVtdAllocated;
        }
      }


      DEBUG ((DEBUG_INFO, "%a AlignedTempBuffer     %08X\n", __FUNCTION__, AlignedTempBuffer));
      BiosRegionOffsetFromTop = BiosRegionOffsetFromTop / SIZE_1KB;
      DEBUG ((DEBUG_INFO, "%a BiosRegionOffsetFromTop %08X\n", __FUNCTION__, BiosRegionOffsetFromTop));

      KbChunksRemaining = KbChunks;
      BlocksCopied = 0;
      while (KbChunksRemaining > 0) {

        DmaTransferSize = ((KbChunksRemaining >= MAX_DMA_TRANSFER_SIZE_IN_KB)? MAX_DMA_TRANSFER_SIZE_IN_KB : KbChunksRemaining);

        // Use DMA to transfer the largest DMA transfer size in Kb Chunks
        Status = StartSpiDma (
                  (VOID *)((UINTN)AlignedTempBuffer + (BlocksCopied * MAX_DMA_TRANSFER_SIZE_IN_KB * SIZE_1KB)),
                  DmaTransferSize,
                  (UINT32)(BiosRegionOffsetFromTop - (BlocksCopied * MAX_DMA_TRANSFER_SIZE_IN_KB)),
                  0x1
                  );
        DEBUG ((DEBUG_INFO, "%a StartSpiDma %r\n", __FUNCTION__, Status));
        if (EFI_ERROR (Status)) {
          IsVtdAllocated = FreeVtdCopyBuffer(IoMmuPpi, AlignedTempBuffer, Pages, BufferMapping);
          if (EFI_ERROR (IsVtdAllocated) && AlignedTempBuffer != NULL) {
            FreeAlignedPages(AlignedTempBuffer, Pages);
            DEBUG((DEBUG_INFO, "AlignedTempBuffer is Freed\n"));
          }
          return Status;
        }
        BlocksCopied++;
        KbChunksRemaining-= DmaTransferSize;
      }

      DEBUG ((DEBUG_INFO, "%a AlignedTempBuffer %08X\n", __FUNCTION__, (UINTN)AlignedTempBuffer));

      if (Destination != AlignedTempBuffer) {
        // Copy data to destination from 1KB aligned temp memory buffer
        CopyMem (
          (VOID *)((UINTN)Destination + Alignment),
          (VOID *)((UINTN)AlignedTempBuffer),
          KbChunks * SIZE_1KB
          );

        IsVtdAllocated = FreeVtdCopyBuffer(IoMmuPpi, AlignedTempBuffer, Pages, BufferMapping);
        if (EFI_ERROR (IsVtdAllocated) && AlignedTempBuffer != NULL) {
          FreeAlignedPages(AlignedTempBuffer, Pages);
          DEBUG((DEBUG_INFO, "AlignedTempBuffer is Freed\n"));
        }
      }
    }

    if (Remainder > 0) {
      // Use normal copy for remainder of unaligned bytes at the end.
      CopyMem (
        (VOID *)((UINTN)Destination + Alignment + (KbChunks * SIZE_1KB)),
        (VOID *)((UINTN)FlashAddress + Alignment + (KbChunks * SIZE_1KB)),
        Remainder
        );
    }
    return EFI_SUCCESS;
  } else {
    CopyMem (Destination, FlashAddress, ReadSize);
  }

  return EFI_SUCCESS;
}

/**
  This function copies Firmware Volumes from SPI to Memory

  @param FirmwareVolumeBase       -  Pointer to the Firmware Volume being copied.
  @param Destination              -  Pointer in Memory the Firmware Volume is copied to.
  @param DestinationLength        -  Length of the Destination Buffer

  @retval   EFI_INVALID_PARAMETER   Invalid BIOS region chosen
  @retval   EFI_INVALID_PARAMETER   Destination is NULL
  @retval   EFI_INVALID_PARAMETER   DestinationLength = 0.
  @retval   EFI_INVALID_PARAMETER   FV does not have valid FV Header.
  @retval   EFI_INVALID_PARAMETER   FV overlaps Destination.
  @retval   EFI_INVALID_PARAMETER   Destination + DestinationLength rolls over 4GB
                                    for 32-bit or 64-bit rollover.
  @retval   EFI_DEVICE_ERROR        One of status bits reported an error
  @retval   EFI_BUFFER_TOO_SMALL    DestinationLength less than FV length from FV
                                    Header.
  @retval   EFI_UNSUPPORTED         FirmwareVolumeBase to FVBase + FVLength does
                                    not support shadow.  Caller should fallback to
                                    CopyMem().
  @retval   EFI_SUCCESS             SPI DMA copied the data successfully
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
  UINTN                        FvLength;
  EFI_STATUS                   Status;

  FwVolHeader = (EFI_FIRMWARE_VOLUME_HEADER *)(UINTN)FirmwareVolumeBase;
  FvLength = (UINTN)FwVolHeader->FvLength;
  if (FvLength == 0 ||
      Destination == NULL ||
      DestinationLength >= (~(UINT32)0) ||
      ((~(UINT32)0)- DestinationLength) <= (UINTN)Destination ||
      (((UINTN)Destination > (UINTN)FirmwareVolumeBase) && ((UINTN)Destination < (UINTN)FirmwareVolumeBase + FvLength))
  ) {
    DEBUG ((DEBUG_ERROR,
            "DMA FV Shadow Invalid Parameter Destination %08x Invalid Pointer %08x FvLength %x\n",
            (UINTN)Destination,
            ((~(UINT32)0)- DestinationLength),
             FvLength));
    ShowBuffer((UINT8 *)FwVolHeader, (UINT32)FvLength);
    return EFI_INVALID_PARAMETER;
  }

  if (FvLength > DestinationLength) {
    return EFI_BUFFER_TOO_SMALL;
  }

  Status = PeiSpiDmaRead((VOID *)((UINTN)FirmwareVolumeBase), Destination, FvLength);
  if (Status == EFI_UNSUPPORTED){
    DEBUG ((DEBUG_INFO, "PeiSpiDmaRead: Unsupported\n"));
  }
  else if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "PeiSpiDmaRead: Failed due to Error %r\n", Status));
    Status = EFI_DEVICE_ERROR;
  }
  return Status;
}

EFI_STATUS
PeiSpiDmaImageRead (
  IN     VOID   *FileHandle,
  IN     UINTN  FileOffset,
  IN     UINTN  *ReadSize,
  OUT    VOID   *Buffer
  )
{
  return PeiSpiDmaRead((VOID *)((UINTN)FileHandle + FileOffset), Buffer, *ReadSize);
}

/**
  This function initializes Dma services

  @retval EFI_STATUS  Results of the installation of the SPI services
**/
EFI_STATUS
EFIAPI
DmaServiceInit (
  VOID
  )
{
  EFI_STATUS        Status;
  SPI_DMA_READ_PPI  *SpiDmaPpi;
  EDKII_PEI_FIRMWARE_VOLUME_SHADOW_PPI *FvShadowPpi;

  Status = EFI_SUCCESS;
  if (IsSimicsEnvironment () || IsHSLEEnvironment ()){
    return Status;
  }
  // Detect SPI DMA presence on the platform.
  if (IsSpiDmaSupported ()){
    Status = PeiServicesLocatePpi (
               &gEdkiiPeiFirmwareVolumeShadowPpiGuid,
               0,
               NULL,
               (VOID **) &FvShadowPpi
               );
    if (EFI_ERROR (Status)) {
      Status = PeiServicesInstallPpi (&mFirmwareVolumeShadowPpiList);
      ASSERT_EFI_ERROR (Status);
    }

    Status = PeiServicesLocatePpi (
               &gSpiDmaReadPpiGuid,
               0,
               NULL,
               (VOID **) &SpiDmaPpi
               );

    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_INFO, "Installing SpiDmaReadPpi\n"));
      Status = PeiServicesInstallPpi (&mSpiDmaReadPpiList);
      ASSERT_EFI_ERROR (Status);
      DEBUG ((DEBUG_INFO, "Installed SpiDmaReadPpi\n"));
    } else {
      DEBUG ((DEBUG_INFO, "SpiDmaReadPpi already installed\n"));
    }

    //
    // Install notify PPIs to lock SPI DMA after copying FVs
    Status = PeiServicesNotifyPpi (&mLockSpiDmaNotifyList);
    ASSERT_EFI_ERROR (Status);
  } else {
    return EFI_UNSUPPORTED;
  }
  return Status;
}
