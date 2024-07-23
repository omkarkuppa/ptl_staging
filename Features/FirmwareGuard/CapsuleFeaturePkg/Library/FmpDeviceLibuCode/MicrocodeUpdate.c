/** @file
  SetImage instance to update Microcode.

  Caution: This module requires additional review when modified.
  This module will have external input - uCode capsule image.
  This external input must be validated carefully to avoid security issue like
  buffer overflow, integer overflow.

  MicrocodeWrite() and VerifyMicrocode() will receive untrusted input and do basic validation.

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

@par Specification Reference:

**/

#include <Library/PlatformFlashAccessLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/TopSwapSupportLib.h>
#include "MicrocodeUpdate.h"
#include <Library/ResiliencySupportLib.h>
#include <Library/PayloadResiliencySupportLib.h>

/**
  Check if input CPU index is in new uCode targeted CPU list.

  @param[in] CpuIndex      Given CPU Index to check
  @param[in] TargetProcessorBuf   Target CPU processor buffer
  @param[in] TargetProcessorNum  number Target CPU processor in buffer

  @retval TRUE  Given CpuIndex is in target uCode CPU list
  @retval FALSE Given CpuIndex is not in target uCode CPU list.
**/
BOOLEAN
IsCpuInTargetCpuList(
  UINTN    CpuIndex,
  UINTN    *TargetProcessorBuf,
  UINTN    TargetProcessorNum
  )
{
  UINTN Index;

  for (Index = 0; Index < TargetProcessorNum; Index++ ) {
    if (CpuIndex == TargetProcessorBuf[Index]) {
      return TRUE;
    }
  }

  return FALSE;
}


/**
  Get Microcode Region.

  @param[out] MicrocodePatchAddress      The address of Microcode
  @param[out] MicrocodePatchRegionSize   The region size of Microcode

  @retval TRUE   The Microcode region is returned.
  @retval FALSE  No Microcode region.
**/
BOOLEAN
GetMicrocodeRegion (
  OUT VOID     **MicrocodePatchAddress,
  OUT UINTN    *MicrocodePatchRegionSize
  )
{
  UINT32 MicrocodeBaseAddress;

  MicrocodeBaseAddress = GetMicrocodeBaseAddressInRecovery ();
  if (MicrocodeBaseAddress == 0) {
    MicrocodeBaseAddress = PcdGet32 (PcdFlashFvMicrocodeBase);
  }
  //
  // Flash Block size alignment is not required for TopSwap Recovery storage selection
  // It is a requirement only when we use Mechanism 2 Option 1.
  //
  ASSERT((MicrocodeBaseAddress % SIZE_4KB) == 0 && (PcdGet32(PcdFlashFvMicrocodeSize) % SIZE_4KB) == 0);
  //
  // Exclude FV header and FFS header
  // We must ensure CpuMicrocodePatchAddress & PcdCpuMicrocodePatchRegionSize must be Flash block size aligned
  //
  // ToDo:
  //   4KB is hardcode rightnow. We can use PcdMicrocodeOffsetInFv in the future
  //
  *MicrocodePatchAddress = (VOID *)(((UINTN)MicrocodeBaseAddress + PcdGet32(PcdMicrocodeOffsetInFv)));
  *MicrocodePatchRegionSize = (UINTN)((UINTN)PcdGet32(PcdFlashFvMicrocodeSize) - PcdGet32(PcdMicrocodeOffsetInFv)) ;

  DEBUG ((DEBUG_INFO, "MicrocodePatchAddress 0x%x\n", *MicrocodePatchAddress));
  DEBUG ((DEBUG_INFO, "MicrocodePatchRegionSize 0x%x\n", *MicrocodePatchRegionSize));

  if ((*MicrocodePatchAddress == NULL) || (*MicrocodePatchRegionSize == 0)) {
    return FALSE;
  }

  return TRUE;
}


/**
  Calculates the checksum of the header of a file.

  @param FileHeader      Pointer to FFS File Header.

  @return Checksum of the header.
          Zero means the header is good.
          Non-zero means the header is bad.
**/
UINT8
GetHeaderChecksum (
  IN EFI_FFS_FILE_HEADER  *FileHeader
  )
{
  EFI_FFS_FILE_HEADER2 TestFileHeader;

  if (IS_FFS_FILE2 (FileHeader)) {
    CopyMem (&TestFileHeader, FileHeader, sizeof (EFI_FFS_FILE_HEADER2));
    //
    // Ingore State and File field in FFS header.
    // Set Header field to 0
    //
    TestFileHeader.State = 0;
    TestFileHeader.IntegrityCheck.Checksum16 = 0;

    return (0 - CalculateSum8 ((CONST UINT8 *) &TestFileHeader, sizeof (EFI_FFS_FILE_HEADER2)));
  } else {
    CopyMem (&TestFileHeader, FileHeader, sizeof (EFI_FFS_FILE_HEADER));
    //
    // Ingore State and File field in FFS header.
    // Set Header filed to 0
    //
    TestFileHeader.State = 0;
    TestFileHeader.IntegrityCheck.Checksum16  = 0;

    return (0 - CalculateSum8 ((CONST UINT8 *) &TestFileHeader, sizeof (EFI_FFS_FILE_HEADER)));
  }
}

/**
  Exract  uCode Payload, It follows RFC 4506 External Data Representation Standard (XDR)
each element is a variable-length opaque data. Elements are organized in Fixed-Length Array.
Now Array contains 4 entries below.  Some elements are optional regarding platform configuration.
uCode capsule generater need to mark length n = 0 in variable-length opaque data element .

  --------------------------------------
  |         UCodeFullRangeImage        |
  --------------------------------------
  |    UCodeFullRangeBgup (Optional)   |
  ----------------------------
  |       uCode Version(Optional)      |
  --------------------------------------
  |          uCdeArray(Optional)       |
  --------------------------------------

  @param[In] Image            uCode Capsule payload
  @param[In] ImageSize      uCode Capsule payload length
  @param[Out] EntryList      List of parsed XDR entry
  @param[Out] EntryCount      Entry count in the list

  @retval Status  status of parsing uCode Capsule XDR formated data.
**/
EFI_STATUS
ExtractuCodeCapsuleXdrData (
  IN  VOID                         *Image,
  IN  UINTN                        ImageSize,
  OUT UCODE_PAYLOAD_ENTRY          **EntryList,
  OUT UINTN                        *ListCount
  )
{
  EFI_STATUS           Status;
  UINTN                Index;
  UINT8                *PayloadXdr;
  UINT8                *PayloadXdrEnd;
  UCODE_PAYLOAD_ENTRY  *XdrEntryArray;
  UINTN                XdrEntryLength;

  ASSERT(((UINTN)Image % sizeof(UINT32)) == 0);

  PayloadXdr     = (UINT8 *)Image;
  PayloadXdrEnd  = (UINT8 *)Image + ImageSize;
  Index          = 0;

  *EntryList     = NULL;
  *ListCount     = 0;

  XdrEntryArray = AllocateZeroPool(uCodeArrayMax * sizeof(UCODE_PAYLOAD_ENTRY));
  if (XdrEntryArray == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Iterate each data entry within uCode capsule payload
  //
  Status     = EFI_SUCCESS;

  while (PayloadXdr < PayloadXdrEnd) {
    if ((PayloadXdr + sizeof (UINT32)) > PayloadXdrEnd) {
      //
      // Xdr described data and Xdr itself should not exceed payload boundary
      //
      DEBUG ((DEBUG_ERROR, "ExtractuCodeCapsuleXdrData: Xdr described data exceeds boundary.\n"));
      Status = EFI_INVALID_PARAMETER;
      goto FUNC_EXIT;
    }

    //
    // Read key length stored in big-endian format
    //
    XdrEntryLength = SwapBytes32 (*(UINT32 *)(PayloadXdr));

    //
    // Point to the start of the uCode payload data entry
    //
    PayloadXdr += sizeof (UINT32);
    if (PayloadXdr + XdrEntryLength > PayloadXdrEnd) {
      //
      // Xdr described data and Xdr itself should not exceed payload boundary
      //
      DEBUG ((DEBUG_ERROR, "ExtractuCodeCapsuleXdrData: Xdr described data exceeds boundary.\n"));
      Status = EFI_INVALID_PARAMETER;
      goto FUNC_EXIT;
    }

    //
    // Check if there is free room to save payload entry
    //
    if (Index >= uCodeArrayMax) {
      DEBUG ((DEBUG_ERROR, "ExtractuCodeCapsuleXdrData: Xdr described data exceeds maximum entry number %x.\n", uCodeArrayMax));
      Status = EFI_INVALID_PARAMETER;
      break;
    }

    XdrEntryArray[Index].EntryPtr  = PayloadXdr;
    XdrEntryArray[Index].EntrySize = XdrEntryLength;

    Index++;

    PayloadXdr += XdrEntryLength;
    //
    // According to RFC 4506, Chapter 3, each XDR descriptor must be aligned to UINT32/32bit boundry.
    //
    PayloadXdr = (UINT8 *)ALIGN_POINTER (PayloadXdr, sizeof (UINT32));
  }

  if (Status != EFI_INVALID_PARAMETER) {
    *EntryList = XdrEntryArray;
    *ListCount = Index;

    DEBUG_CODE (
      DEBUG ((DEBUG_INFO, "ExtractuCodeCapsuleXdrData %x entries are found in payload\n", *ListCount));
      for (Index = 0; Index < *ListCount; Index++) {
        DEBUG ((DEBUG_INFO, "Index %x Offset %x, Size %x\n", Index, XdrEntryArray[Index].EntryPtr - (UINT8 *)Image, XdrEntryArray[Index].EntrySize));
      }
    );
  }

FUNC_EXIT:
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "uCode Paylod Xdr Parsing error\n"));
    FreePool(XdrEntryArray);
  }

  return Status;
}

/**
  Update Version info FFS inside uCode FV.

  uCode FV is orgnized as follwing rules

  ----------------------------
  |         FV Header        |
  ----------------------------
  |   Optional Ext Header    |
  ----------------------------
  |        Version FFS       |
  ----------------------------
  |     Optional Pad FFS     |
  ----------------------------
  |         uCode FFS        |
  ----------------------------

  During update, we can update FFS + Optional Pad FFS to store new Version FFS. but must keep uCode FFS unchanged

  @param[In] VerInfoFFS      The content of New VersionInf

  @retval Version File Payload.   The Microcode Version file content is returned.
**/
EFI_STATUS
UpdateVersionInfo (
  INTEL_MICROCODE_VERSION_FFS_DATA *NewVersion,
  UINTN                            NewVersionSize
  )
{
  EFI_STATUS           Status;
  UINT8                *uCodeFv;
  UINTN                Index;
  UINTN                VersionSize;
  UINTN                MaxVersionSize;
  UINT8                *VersionAddr;
  EFI_FFS_FILE_HEADER  *uCodeFfsHdr;
  EFI_FFS_FILE_HEADER  *VersionFfsHdr;
  FLASH_ACCESS_REQUEST AccessReq;

  uCodeFfsHdr   = NULL;
  VersionFfsHdr = NULL;
  uCodeFv       = NULL;

  //
  // Use SPI read instead of MMIO read bicoz we are using TopSwap. and MMIO is mapping to LastKnownGood
  //
  uCodeFv = AllocateZeroPool(PcdGet32(PcdMicrocodeOffsetInFv));
  if (uCodeFv == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem (&AccessReq, sizeof (AccessReq));
  AccessReq.FlashRegionType = FlashRegionBios;
  AccessReq.FlashAddress    = (UINTN)PcdGet32(PcdFlashFvMicrocodeOffset);
  AccessReq.Data            = uCodeFv;
  AccessReq.DataSize        = (UINTN)PcdGet32(PcdMicrocodeOffsetInFv);

  Status = FlashRead (&AccessReq);
  if (EFI_ERROR(Status)) {
    goto FUNC_EXIT;
  }

  //
  // Search uCode array FFS Header
  //
  for (Index = sizeof(EFI_FIRMWARE_VOLUME_HEADER); Index < PcdGet32(PcdMicrocodeOffsetInFv); Index++) {
    if (CompareGuid((EFI_GUID *)&uCodeFv[Index], &gIntelMicrocodeArrayFfsFileGuid)) {
      break;
    }
  }

  if (Index >= PcdGet32(PcdMicrocodeOffsetInFv)) {
    DEBUG ((DEBUG_ERROR, "Can't find Microcode array FFS\n"));
    ASSERT(FALSE);
    Status = EFI_INVALID_PARAMETER;
    if (uCodeFv != NULL) {
      FreePool(uCodeFv);
    }
    return Status;
  }
  uCodeFfsHdr = (EFI_FFS_FILE_HEADER *)&uCodeFv[Index];

  //
  // Search Version FFS Header. It is between FV Header and uCode array FFS
  //
  for (Index = sizeof(EFI_FIRMWARE_VOLUME_HEADER); &uCodeFv[Index] < (UINT8 *)uCodeFfsHdr; Index++) {
    if (CompareGuid((EFI_GUID *)&uCodeFv[Index], &gIntelMicrocodeVersionFfsFileGuid)) {
      break;
    }
  }

  if (&uCodeFv[Index] >= (UINT8 *)uCodeFfsHdr) {
    DEBUG ((DEBUG_ERROR, "Can't find Microcode Version FFS\n"));
    ASSERT(FALSE);
    Status = EFI_INVALID_PARAMETER;
    goto FUNC_EXIT;
  }
  VersionFfsHdr = (EFI_FFS_FILE_HEADER *)&uCodeFv[Index];

  if (uCodeFfsHdr->Type != EFI_FV_FILETYPE_RAW || VersionFfsHdr->Type != EFI_FV_FILETYPE_RAW) {
    DEBUG ((DEBUG_ERROR, "Can't find Microcode Version FFS\n"));
    ASSERT(FALSE);
    Status = EFI_INVALID_PARAMETER;
    goto FUNC_EXIT;
  }


  if (IS_FFS_FILE2 (VersionFfsHdr)) {
    VersionSize    = FFS_FILE2_SIZE (VersionFfsHdr) - sizeof(EFI_FFS_FILE_HEADER2);
    VersionAddr    = (UINT8 *)VersionFfsHdr + sizeof (EFI_FFS_FILE_HEADER2);
    MaxVersionSize = (UINTN)(PHYSICAL_ADDRESS)((UINTN)(UINT8 *)uCodeFfsHdr - (UINTN)(UINT8 *)VersionFfsHdr) - sizeof(EFI_FFS_FILE_HEADER2);
  } else {
    VersionSize    = FFS_FILE_SIZE (VersionFfsHdr) - sizeof(EFI_FFS_FILE_HEADER);
    VersionAddr    = (UINT8 *)VersionFfsHdr + sizeof (EFI_FFS_FILE_HEADER);
    MaxVersionSize = (UINTN)(PHYSICAL_ADDRESS)((UINTN)(UINT8 *)uCodeFfsHdr - (UINTN)(UINT8 *)VersionFfsHdr) - sizeof(EFI_FFS_FILE_HEADER);
  }

  //
  // Update Version FFS based on current situation
  //
  if (NewVersionSize <= VersionSize) {
    //
    // New version content is smaller, Update Version data and mask remaing tail to all Zero
    // Do not update current Version FFS Header
    //
    CopyMem(VersionAddr, NewVersion, NewVersionSize);
    ZeroMem(VersionAddr + NewVersionSize, VersionSize - NewVersionSize);

    ZeroMem (&AccessReq, sizeof (AccessReq));
    AccessReq.FlashRegionType = FlashRegionBios;
    AccessReq.FlashAddress    = PcdGet32(PcdFlashFvMicrocodeOffset) + VersionAddr - uCodeFv;
    AccessReq.Data            = VersionAddr;
    AccessReq.DataSize        = VersionSize;

    Status = FlashUpdate (&AccessReq, NULL, 0, 0);
  } else  if (NewVersionSize <= MaxVersionSize) {
    //
    // New version content is bigger. Expand version file to a bigger one
    //
    if (IS_FFS_FILE2 (VersionFfsHdr)) {
      ((EFI_FFS_FILE_HEADER2 *)VersionFfsHdr)->ExtendedSize = (UINT64)(MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER2));
    } else {
      VersionFfsHdr->Size[0] = (UINT8)((MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER)) & 0xFF);
      VersionFfsHdr->Size[1] = (UINT8)(((MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER)) & 0xFF00) >> 8);
      VersionFfsHdr->Size[2] = (UINT8)(((MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER)) & 0xFF0000) >> 16);
    }

    //
    // Todo:  Still need to check FFS_ATTRIB_CHECKSUM Attribute.
    //            Now assume this attribute bit is 0. CHECKSU Checksum.File = 0xAA
    // Re-calcutate check sum bicoz size is changed
    //
    if ((VersionFfsHdr->Attributes & FFS_ATTRIB_CHECKSUM) == 0) {
      VersionFfsHdr->IntegrityCheck.Checksum.Header = GetHeaderChecksum(VersionFfsHdr);
    } else {
      DEBUG ((DEBUG_ERROR, "Attribute FFS_ATTRIB_CHECKSUM not supported\n"));
      Status = EFI_INVALID_PARAMETER;
      goto FUNC_EXIT;
    }
    CopyMem(VersionAddr, NewVersion, NewVersionSize);
    ZeroMem(VersionAddr + NewVersionSize, MaxVersionSize - NewVersionSize);

    //
    // Update entire Version FFS. Not its size changed to MaxVersionSize + FFS Header
    //
    ZeroMem (&AccessReq, sizeof (AccessReq));
    AccessReq.FlashRegionType = FlashRegionBios;
    AccessReq.FlashAddress    = PcdGet32(PcdFlashFvMicrocodeOffset) + (UINT8 *)VersionFfsHdr - uCodeFv;
    AccessReq.Data            = (UINT8 *)VersionFfsHdr;
    if (IS_FFS_FILE2 (VersionFfsHdr)) {
      AccessReq.DataSize = MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER2);
    } else {
      AccessReq.DataSize = MaxVersionSize + sizeof (EFI_FFS_FILE_HEADER);
    }

    Status = FlashUpdate (&AccessReq, NULL, 0, 0);
  } else {
    DEBUG ((DEBUG_ERROR, "Microcode Version FFS size 0x%x is bigger than MaxSize %x\n", NewVersionSize, MaxVersionSize));
    ASSERT(FALSE);
    Status = EFI_INVALID_PARAMETER;
  }

FUNC_EXIT:

  if (uCodeFv != NULL) {
    FreePool(uCodeFv);
  }

  return Status;
}


/**
  Get Microcode Version File Data. Caller is responsible for freeing this resource

  @param[out] MicrocodePatchAddress      The address of Microcode
  @param[out] MicrocodePatchRegionSize   The region size of Microcode

  @retval Version File Payload.   The Microcode Version file content is returned.
**/
EFI_STATUS
GetMicrocodeRegionVersionInfo (
  OUT UINT8 **VersionInfo,
  OUT UINTN *VersionInfoSize
  )
{
  EFI_STATUS                           Status;
  EFI_HANDLE                           *HandleBuffer;
  UINTN                                NumberOfHandles;
  EFI_FV_FILETYPE                      FileType;
  UINT32                               FvStatus;
  EFI_FV_FILE_ATTRIBUTES               Attributes;
  UINTN                                Size;
  UINTN                                Index;
  EFI_FIRMWARE_VOLUME2_PROTOCOL        *FvInstance;
  UINT8                                *DataBuf;

  DataBuf          = NULL;
  FvInstance       = NULL;
  *VersionInfo     = NULL;
  *VersionInfoSize = 0;

  //
  // Locate FV protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  &gEfiFirmwareVolume2ProtocolGuid,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }

  //
  // Looking for uCode FVB
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {
    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    &gEfiFirmwareVolume2ProtocolGuid,
                    (VOID**) &FvInstance
                    );
    ASSERT_EFI_ERROR (Status);

    DEBUG_CODE(
      EFI_FIRMWARE_VOLUME_BLOCK_PROTOCOL  *Fvb;
      EFI_PHYSICAL_ADDRESS                BaseAddr;

      BaseAddr = 0;
      Status = gBS->HandleProtocol (
                      HandleBuffer[Index],
                      &gEfiFirmwareVolumeBlockProtocolGuid,
                      (VOID**) &Fvb
                      );
      Fvb->GetPhysicalAddress(Fvb, &BaseAddr);
      DEBUG ((DEBUG_ERROR, "FVB BaseAddress 0x%x\n", BaseAddr));
    );

    //
    // See if it has the Fmp Version File storage file
    //
    Status = FvInstance->ReadFile (
                           FvInstance,
                           &gIntelMicrocodeVersionFfsFileGuid,
                           NULL,
                           &Size,
                           &FileType,
                           &Attributes,
                           &FvStatus
                           );

    //
    // Found and file attribute is correct
    //
    if (Status == EFI_SUCCESS && FileType == EFI_FV_FILETYPE_RAW && Size >= sizeof(INTEL_MICROCODE_VERSION_FFS_DATA)) {
      DataBuf = AllocatePool(Size);
      break;
    }
  }

  if ((DataBuf != NULL) && (FvInstance != NULL)) {
    Status = FvInstance->ReadFile (
                           FvInstance,
                           &gIntelMicrocodeVersionFfsFileGuid,
                           (VOID**) &DataBuf,
                           &Size,
                           &FileType,
                           &Attributes,
                           &FvStatus
                           );
     ASSERT_EFI_ERROR(Status);

     *VersionInfo     = DataBuf;
     *VersionInfoSize = Size;
     DEBUG ((DEBUG_INFO, "Found Version file for uCode Region\n"));
  }

  //
  // Free any allocated buffers
  //
  gBS->FreePool (HandleBuffer);

  return Status;
}


/**
  Get Microcode update signature of currently loaded Microcode update.

  @return  Microcode signature.

**/
UINT32
GetCurrentMicrocodeSignature (
  VOID
  )
{
  MSR_IA32_BIOS_SIGN_ID_REGISTER      MsrSignId;

  AsmWriteMsr64 (MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (CPUID_VERSION_INFO, NULL, NULL, NULL, NULL);
  MsrSignId.Uint64 = AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID);
  return MsrSignId.Bits.MicrocodeUpdateSignature;
}

/**
  Get current processor signature.

  @return current processor signature.
**/
UINT32
GetCurrentProcessorSignature (
  VOID
  )
{
  UINT32                                  RegEax;
  AsmCpuid(CPUID_VERSION_INFO, &RegEax, NULL, NULL, NULL);
  return RegEax;
}

/**
  Get current platform ID.

  @return current platform ID.
**/
UINT8
GetCurrentPlatformId (
  VOID
  )
{
  UINT8                                   PlatformId;

  PlatformId = (UINT8)AsmMsrBitFieldRead64(MSR_IA32_PLATFORM_ID, 50, 52);
  return PlatformId;
}

/**
  Load new Microcode.

  @param[in] Address  The address of new Microcode.

  @return  Loaded Microcode signature.

**/
UINT32
LoadMicrocode (
  IN UINT64  Address
  )
{
  AsmWriteMsr64(MSR_IA32_BIOS_UPDT_TRIG, Address);
  return GetCurrentMicrocodeSignature();
}

/**
  Load Microcode on an Application Processor.
  The function prototype for invoking a function on an Application Processor.

  @param[in,out] Buffer  The pointer to private data buffer.
**/
VOID
EFIAPI
MicrocodeLoadAp (
  IN OUT VOID  *Buffer
  )
{
  MICROCODE_LOAD_BUFFER                *MicrocodeLoadBuffer;

  MicrocodeLoadBuffer = Buffer;
  MicrocodeLoadBuffer->Revision = LoadMicrocode (MicrocodeLoadBuffer->Address);
}

/**
  Load new Microcode on this processor

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  CpuIndex                   The index of the processor.
  @param[in]  Address                    The address of new Microcode.

  @return  Loaded Microcode signature.

**/
UINT32
LoadMicrocodeOnThis (
  IN  MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate,
  IN  UINTN                       CpuIndex,
  IN  UINT64                      Address
  )
{
  EFI_STATUS                           Status;
  EFI_MP_SERVICES_PROTOCOL             *MpService;
  MICROCODE_LOAD_BUFFER                MicrocodeLoadBuffer;

  if (CpuIndex == MicrocodeFmpPrivate->BspIndex) {
    return LoadMicrocode (Address);
  } else {
    MpService = MicrocodeFmpPrivate->MpService;
    MicrocodeLoadBuffer.Address = Address;
    MicrocodeLoadBuffer.Revision = 0;
    Status = MpService->StartupThisAP (
                          MpService,
                          MicrocodeLoadAp,
                          CpuIndex,
                          NULL,
                          0,
                          &MicrocodeLoadBuffer,
                          NULL
                          );
    ASSERT_EFI_ERROR(Status);
    return MicrocodeLoadBuffer.Revision;
  }
}

/**
  Collect processor information.
  The function prototype for invoking a function on an Application Processor.

  @param[in,out] Buffer  The pointer to private data buffer.
**/
VOID
EFIAPI
CollectProcessorInfo (
  IN OUT VOID  *Buffer
  )
{
  PROCESSOR_INFO  *ProcessorInfo;

  ProcessorInfo = Buffer;
  ProcessorInfo->ProcessorSignature = GetCurrentProcessorSignature();
  ProcessorInfo->PlatformId         = GetCurrentPlatformId();
  ProcessorInfo->ProcessorFlags     = 1 << ProcessorInfo->PlatformId;
  ProcessorInfo->MicrocodeRevision  = GetCurrentMicrocodeSignature();

}

/**
  Get current Microcode information.

  The ProcessorInformation (BspIndex/ProcessorCount/ProcessorInfo)
  in MicrocodeFmpPrivate must be initialized.

  The MicrocodeInformation (DescriptorCount/ImageDescriptor/MicrocodeInfo)
  in MicrocodeFmpPrivate may not be avaiable in this function.

  @param[in]   MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]   DescriptorCount            The count of Microcode ImageDescriptor allocated.
  @param[out]  ImageDescriptor            Microcode ImageDescriptor
  @param[out]  MicrocodeInfo              Microcode information

  @return Microcode count
**/
UINTN
GetMicrocodeInfo (
  IN  MICROCODE_FMP_PRIVATE_DATA     *MicrocodeFmpPrivate,
  IN  UINTN                          DescriptorCount,  OPTIONAL
  OUT EFI_FIRMWARE_IMAGE_DESCRIPTOR  *ImageDescriptor, OPTIONAL
  OUT MICROCODE_INFO                 *MicrocodeInfo    OPTIONAL
  )
{
  VOID                                    *MicrocodePatchAddress;
  UINTN                                   MicrocodePatchRegionSize;
  CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint;
  UINTN                                   MicrocodeEnd;
  UINTN                                   TotalSize;
  UINTN                                   Count;
  UINT64                                  ImageAttributes;
  BOOLEAN                                 IsInUse;
  EFI_STATUS                              Status;
  UINT32                                  AttemptStatus;
  UINTN                                   TargetCpuIndex;

  MicrocodePatchAddress = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  DEBUG ((DEBUG_INFO, "Microcode Region - 0x%x - 0x%x\n", MicrocodePatchAddress, MicrocodePatchRegionSize));

  Count = 0;

  MicrocodeEnd = (UINTN)MicrocodePatchAddress + MicrocodePatchRegionSize;
  MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (UINTN) MicrocodePatchAddress;
  do {
    if (MicrocodeEntryPoint->HeaderVersion == 0x1 && MicrocodeEntryPoint->LoaderRevision == 0x1) {
      //
      // It is the microcode header. It is not the padding data between microcode patches
      // becasue the padding data should not include 0x00000001 and it should be the repeated
      // byte format (like 0xXYXYXYXY....).
      //
      if (MicrocodeEntryPoint->DataSize == 0) {
        TotalSize = 2048;
      } else {
        TotalSize = MicrocodeEntryPoint->TotalSize;
      }

      TargetCpuIndex = (UINTN)-1;
      Status = VerifyMicrocode(MicrocodeFmpPrivate, MicrocodeEntryPoint, TotalSize, FALSE, &AttemptStatus, NULL, &TargetCpuIndex);

      if (!EFI_ERROR(Status)) {
        IsInUse = TRUE;
        ASSERT (TargetCpuIndex < MicrocodeFmpPrivate->ProcessorCount);
        MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].MicrocodeIndex = Count;
        //
        // Always update logic CPU MicrocodeRevision with highest Supported Microcode Version.
        // This logic complies with Physical CPU uCode patching logic
        //
        if (!MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].IsRealCpu) {
          if ((MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].MicrocodeRevision == (UINT32)-1) ||
              (MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].MicrocodeRevision < MicrocodeEntryPoint->UpdateRevision)) {
            MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].MicrocodeRevision = MicrocodeEntryPoint->UpdateRevision;
          }
        }
      } else {
        IsInUse = FALSE;
      }

      if (ImageDescriptor != NULL && DescriptorCount > Count) {
        ImageDescriptor[Count].ImageIndex = (UINT8)(Count + 1);
        CopyGuid (&ImageDescriptor[Count].ImageTypeId, &gFmpDeviceMicrocodeDefaultGuid);
        ImageDescriptor[Count].ImageId = LShiftU64(MicrocodeEntryPoint->ProcessorFlags, 32) + MicrocodeEntryPoint->ProcessorSignature.Uint32;
        ImageDescriptor[Count].ImageIdName = NULL;
        ImageDescriptor[Count].Version = MicrocodeEntryPoint->UpdateRevision;
        ImageDescriptor[Count].VersionName = NULL;
        ImageDescriptor[Count].Size = TotalSize;
        ImageAttributes = IMAGE_ATTRIBUTE_IMAGE_UPDATABLE | IMAGE_ATTRIBUTE_RESET_REQUIRED;
        if (IsInUse) {
          ImageAttributes |= IMAGE_ATTRIBUTE_IN_USE;
        }
        ImageDescriptor[Count].AttributesSupported = ImageAttributes | IMAGE_ATTRIBUTE_IN_USE;
        ImageDescriptor[Count].AttributesSetting = ImageAttributes;
        ImageDescriptor[Count].Compatibilities = 0;
        ImageDescriptor[Count].LowestSupportedImageVersion = MicrocodeEntryPoint->UpdateRevision; // do not support rollback
        ImageDescriptor[Count].LastAttemptVersion = 0;
        ImageDescriptor[Count].LastAttemptStatus = 0;
        ImageDescriptor[Count].HardwareInstance = 0;
      }
      if (MicrocodeInfo != NULL && DescriptorCount > Count) {
        MicrocodeInfo[Count].MicrocodeEntryPoint = MicrocodeEntryPoint;
        MicrocodeInfo[Count].TotalSize = TotalSize;
        MicrocodeInfo[Count].InUse = IsInUse;
      }
    } else {
      //
      // It is the padding data between the microcode patches for microcode patches alignment.
      // Because the microcode patch is the multiple of 1-KByte, the padding data should not
      // exist if the microcode patch alignment value is not larger than 1-KByte. So, the microcode
      // alignment value should be larger than 1-KByte. We could skip SIZE_4KB padding data to
      // find the next possible microcode patch header.
      //
      MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (((UINTN) MicrocodeEntryPoint) + SIZE_1KB);
      continue;
    }

    Count++;
    ASSERT(Count < 0xFF);

    //
    // Get the next patch.
    //
    MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) (((UINTN) MicrocodeEntryPoint) + TotalSize);
  } while (((UINTN) MicrocodeEntryPoint < MicrocodeEnd));

  return Count;
}

/**
  Return matched processor information.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  ProcessorSignature         The processor signature to be matched
  @param[in]  ProcessorFlags             The processor flags to be matched
  @param[in, out] TargetCpuIndex         On input, the index of target CPU which tries to match the Microcode. (UINTN)-1 means to try all.
                                         On output, the index of target CPU which matches the Microcode.

  @return matched processor information.
**/
PROCESSOR_INFO *
GetMatchedProcessor (
  IN MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate,
  IN UINT32                      ProcessorSignature,
  IN UINT32                      ProcessorFlags,
  IN OUT UINTN                   *TargetCpuIndex
  )
{
  UINTN  Index;

  if (*TargetCpuIndex != (UINTN)-1) {
    Index = *TargetCpuIndex;
    if (ProcessorSignature == MicrocodeFmpPrivate->ProcessorInfo[Index].ProcessorSignature &&
        (ProcessorFlags & MicrocodeFmpPrivate->ProcessorInfo[Index].ProcessorFlags) != 0) {
      return &MicrocodeFmpPrivate->ProcessorInfo[Index];
    } else {
      return NULL;
    }
  }

  for (Index = 0; Index < MicrocodeFmpPrivate->ProcessorCount; Index++) {
    if ((ProcessorSignature == MicrocodeFmpPrivate->ProcessorInfo[Index].ProcessorSignature) &&
        ((ProcessorFlags & MicrocodeFmpPrivate->ProcessorInfo[Index].ProcessorFlags) != 0)) {
      *TargetCpuIndex = Index;
      return &MicrocodeFmpPrivate->ProcessorInfo[Index];
    }
  }

  return NULL;
}

/**
  Verify Microcode.
  if the uCode fails check sum. or version is lower than any of the processor that matches signature and flags
  Or tryload == TRUE but uCode fails CPU authentication. Microcode verification fails

  Caution: This function may receive untrusted input.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  Image                      The Microcode image buffer.
  @param[in]  ImageSize                  The size of Microcode image buffer in bytes.
  @param[in]  TryLoad                    Try to load Microcode or not.
  @param[out] LastAttemptStatus          The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out] AbortReason                A pointer to a pointer to a null-terminated string providing more
                                         details for the aborted operation. The buffer is allocated by this function
                                         with AllocatePool(), and it is the caller's responsibility to free it with a
                                         call to FreePool().
  @param[in, out] TargetCpuIndex         On input, the index of target CPU which tries to match the Microcode. (UINTN)-1 means to try all.
                                         On output, the index of target CPU which matches the Microcode.

  @retval EFI_SUCCESS               The Microcode image passes verification.
  @retval EFI_VOLUME_CORRUPTED      The Microcode image is corrupted.
  @retval EFI_INCOMPATIBLE_VERSION  The Microcode image version is incorrect.
  @retval EFI_UNSUPPORTED           The Microcode ProcessorSignature or ProcessorFlags is incorrect.
  @retval EFI_SECURITY_VIOLATION    The Microcode image fails to load.
**/
EFI_STATUS
VerifyMicrocode (
  IN  MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate,
  IN  VOID                        *Image,
  IN  UINTN                       ImageSize,
  IN  BOOLEAN                     TryLoad,
  OUT UINT32                      *LastAttemptStatus,
  OUT CHAR16                      **AbortReason,   OPTIONAL
  IN OUT UINTN                    *TargetCpuIndex
  )
{
  UINTN                                   Index;
  CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint;
  UINTN                                   TotalSize;
  UINTN                                   DataSize;
  UINT32                                  CurrentRevision;
  PROCESSOR_INFO                          *ProcessorInfo;
  UINT32                                  CheckSum32;
  UINT32                                  InCompleteSum32;
  UINTN                                   ExtendedTableLength;
  UINT32                                  ExtendedTableCount;
  CPU_MICROCODE_EXTENDED_TABLE            *ExtendedTable;
  CPU_MICROCODE_EXTENDED_TABLE_HEADER     *ExtendedTableHeader;
  BOOLEAN                                 CorrectMicrocode;

  //
  // Check HeaderVersion
  //
  MicrocodeEntryPoint = Image;
  if (MicrocodeEntryPoint->HeaderVersion != 0x1) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on HeaderVersion\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidHeaderVersion"), L"InvalidHeaderVersion");
    }
    return EFI_INCOMPATIBLE_VERSION;
  }
  //
  // Check LoaderRevision
  //
  if (MicrocodeEntryPoint->LoaderRevision != 0x1) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on LoaderRevision\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidLoaderVersion"), L"InvalidLoaderVersion");
    }
    return EFI_INCOMPATIBLE_VERSION;
  }
  //
  // Check TotalSize
  //
  if (MicrocodeEntryPoint->DataSize == 0) {
    TotalSize = 2048;
  } else {
    TotalSize = MicrocodeEntryPoint->TotalSize;
  }
  if (TotalSize <= sizeof(CPU_MICROCODE_HEADER)) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - TotalSize too small\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidTotalSize"), L"InvalidTotalSize");
    }
    return EFI_VOLUME_CORRUPTED;
  }
  if ((TotalSize & (SIZE_1KB - 1)) != 0) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - TotalSize is not multiples of 1024 bytes (1 KBytes)\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidTotalSize"), L"InvalidTotalSize");
    }
    return EFI_VOLUME_CORRUPTED;
  }
  if (TotalSize != ImageSize) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - TotalSize not equal to ImageSize\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidTotalSize"), L"InvalidTotalSize");
    }
    return EFI_VOLUME_CORRUPTED;
  }
  //
  // Check DataSize
  //
  if (MicrocodeEntryPoint->DataSize == 0) {
    DataSize = 2048 - sizeof(CPU_MICROCODE_HEADER);
  } else {
    DataSize = MicrocodeEntryPoint->DataSize;
  }
  if (DataSize > TotalSize - sizeof(CPU_MICROCODE_HEADER)) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - DataSize too big\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidDataSize"), L"InvalidDataSize");
    }
    return EFI_VOLUME_CORRUPTED;
  }
  if ((DataSize & 0x3) != 0) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - DataSize is not multiples of DWORDs\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidDataSize"), L"InvalidDataSize");
    }
    return EFI_VOLUME_CORRUPTED;
  }
  //
  // Check CheckSum32
  //
  CheckSum32 = CalculateSum32((UINT32 *)MicrocodeEntryPoint, DataSize + sizeof(CPU_MICROCODE_HEADER));
  if (CheckSum32 != 0) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on CheckSum32\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INVALID_FORMAT;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"InvalidChecksum"), L"InvalidChecksum");
    }
    return EFI_VOLUME_CORRUPTED;
  }

  InCompleteSum32 = CheckSum32;
  InCompleteSum32 -= MicrocodeEntryPoint->ProcessorSignature.Uint32;
  InCompleteSum32 -= MicrocodeEntryPoint->ProcessorFlags;
  InCompleteSum32 -= MicrocodeEntryPoint->Checksum;

  //
  // Check ProcessorSignature/ProcessorFlags
  //
  ProcessorInfo = GetMatchedProcessor (MicrocodeFmpPrivate, MicrocodeEntryPoint->ProcessorSignature.Uint32, MicrocodeEntryPoint->ProcessorFlags, TargetCpuIndex);
  if (ProcessorInfo == NULL) {
    CorrectMicrocode = FALSE;
    ExtendedTableLength = TotalSize - (DataSize + sizeof(CPU_MICROCODE_HEADER));
    if (ExtendedTableLength != 0) {
      //
      // Extended Table exist, check if the CPU in support list
      //
      ExtendedTableHeader = (CPU_MICROCODE_EXTENDED_TABLE_HEADER *)((UINT8 *)(MicrocodeEntryPoint) + DataSize + sizeof(CPU_MICROCODE_HEADER));
      //
      // Calculate Extended Checksum
      //
      if ((ExtendedTableLength > sizeof(CPU_MICROCODE_EXTENDED_TABLE_HEADER)) && ((ExtendedTableLength & 0x3) == 0)) {
        CheckSum32 = CalculateSum32((UINT32 *)ExtendedTableHeader, ExtendedTableLength);
        if (CheckSum32 != 0) {
          //
          // Checksum incorrect
          //
          DEBUG ((DEBUG_ERROR, "VerifyMicrocode - The checksum for extended table is incorrect\n"));
        } else {
          //
          // Checksum correct
          //
          ExtendedTableCount = ExtendedTableHeader->ExtendedSignatureCount;
          if (ExtendedTableCount > (ExtendedTableLength - sizeof(CPU_MICROCODE_EXTENDED_TABLE_HEADER)) / sizeof(CPU_MICROCODE_EXTENDED_TABLE)) {
            DEBUG ((DEBUG_ERROR, "VerifyMicrocode - ExtendedTableCount too big\n"));
          } else {
            ExtendedTable = (CPU_MICROCODE_EXTENDED_TABLE *)(ExtendedTableHeader + 1);
            for (Index = 0; Index < ExtendedTableCount; Index++) {
              CheckSum32 = InCompleteSum32;
              CheckSum32 += ExtendedTable->ProcessorSignature.Uint32;
              CheckSum32 += ExtendedTable->ProcessorFlag;
              CheckSum32 += ExtendedTable->Checksum;
              if (CheckSum32 != 0) {
                DEBUG ((DEBUG_ERROR, "VerifyMicrocode - The checksum for ExtendedTable entry with index 0x%x is incorrect\n", Index));
              } else {
                //
                // Verify Header
                //
                ProcessorInfo = GetMatchedProcessor (MicrocodeFmpPrivate, ExtendedTable->ProcessorSignature.Uint32, ExtendedTable->ProcessorFlag, TargetCpuIndex);
                if (ProcessorInfo != NULL) {
                  //
                  // Find one
                  //
                  CorrectMicrocode = TRUE;
                  break;
                }
              }
              ExtendedTable++;
            }
          }
        }
      }
    }
    if (!CorrectMicrocode) {
      if (TryLoad) {
        DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on Current ProcessorSignature/ProcessorFlags\n"));
      }
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INCORRECT_VERSION;
      if (AbortReason != NULL) {
        *AbortReason = AllocateCopyPool(sizeof(L"UnsupportedProcessorSignature/ProcessorFlags"), L"UnsupportedProcessorSignature/ProcessorFlags");
      }
      return EFI_UNSUPPORTED;
    }
  }

  //
  // Regard same revision as processor CurrentRevion can pass verification.
  // Caller can double filter this case outside VerifyMicrocde
  //
  CurrentRevision = ProcessorInfo->MicrocodeRevision;
  if ((CurrentRevision != (UINT32)-1) && (MicrocodeEntryPoint->UpdateRevision < CurrentRevision)) {
    DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on UpdateRevision\n"));

    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INCORRECT_VERSION;
    if (AbortReason != NULL) {
      *AbortReason = AllocateCopyPool(sizeof(L"IncorrectRevision"), L"IncorrectRevision");
    }
    return EFI_INCOMPATIBLE_VERSION;
  }

  //
  // Check UpdateRevision and Load MCU when CPU is real hardware.
  // If CPU definition comes  from uCode Policy PCD, do not TryLoad
  //
  if (ProcessorInfo->IsRealCpu && TryLoad) {
    CurrentRevision = LoadMicrocodeOnThis(MicrocodeFmpPrivate, ProcessorInfo->CpuIndex, (UINTN)MicrocodeEntryPoint + sizeof(CPU_MICROCODE_HEADER));
    if (MicrocodeEntryPoint->UpdateRevision != CurrentRevision) {
      DEBUG ((DEBUG_ERROR, "VerifyMicrocode - fail on LoadMicrocode\n"));
      *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_AUTH_ERROR;
      if (AbortReason != NULL) {
        *AbortReason = AllocateCopyPool(sizeof(L"InvalidData"), L"InvalidData");
      }
      return EFI_SECURITY_VIOLATION;
    }
  }

  return EFI_SUCCESS;
}

/**
  Get next Microcode entrypoint.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  MicrocodeEntryPoint        Current Microcode entrypoint

  @return next Microcode entrypoint.
**/
CPU_MICROCODE_HEADER *
GetNextMicrocode (
  IN MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint
  )
{
  UINTN                                   Index;

  for (Index = 0; Index < MicrocodeFmpPrivate->DescriptorCount; Index++) {
    if (MicrocodeEntryPoint == MicrocodeFmpPrivate->MicrocodeInfo[Index].MicrocodeEntryPoint) {
      if (Index == (UINTN)MicrocodeFmpPrivate->DescriptorCount - 1) {
        // it is last one
        return NULL;
      } else {
        // return next one
        return MicrocodeFmpPrivate->MicrocodeInfo[Index + 1].MicrocodeEntryPoint;
      }
    }
  }

  ASSERT(FALSE);
  return NULL;
}

/**
  Get next FIT Microcode entrypoint.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  MicrocodeEntryPoint        Current Microcode entrypoint

  @return next FIT Microcode entrypoint.
**/
CPU_MICROCODE_HEADER *
GetNextFitMicrocode (
  IN MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint
  )
{
  UINTN                                   Index;

  for (Index = 0; Index < MicrocodeFmpPrivate->FitMicrocodeEntryCount; Index++) {
    if (MicrocodeEntryPoint == MicrocodeFmpPrivate->FitMicrocodeInfo[Index].MicrocodeEntryPoint) {
      if (Index == (UINTN) MicrocodeFmpPrivate->FitMicrocodeEntryCount - 1) {
        // it is last one
        return NULL;
      } else {
        // return next one
        return MicrocodeFmpPrivate->FitMicrocodeInfo[Index + 1].MicrocodeEntryPoint;
      }
    }
  }

  ASSERT(FALSE);
  return NULL;
}

/**
  Find empty FIT Microcode entrypoint.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  ImageSize                  The size of Microcode image buffer in bytes.
  @param[out] AvailableSize              Available size of the empty FIT Microcode entrypoint.

  @return Empty FIT Microcode entrypoint.
**/
CPU_MICROCODE_HEADER *
FindEmptyFitMicrocode (
  IN MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN UINTN                                   ImageSize,
  OUT UINTN                                  *AvailableSize
  )
{
  UINTN                                   Index;
  CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint;
  CPU_MICROCODE_HEADER                    *NextMicrocodeEntryPoint;
  VOID                                    *MicrocodePatchAddress;
  UINTN                                   MicrocodePatchRegionSize;

  MicrocodePatchAddress = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  for (Index = 0; Index < MicrocodeFmpPrivate->FitMicrocodeEntryCount; Index++) {
    if (MicrocodeFmpPrivate->FitMicrocodeInfo[Index].Empty) {
      MicrocodeEntryPoint = MicrocodeFmpPrivate->FitMicrocodeInfo[Index].MicrocodeEntryPoint;
      NextMicrocodeEntryPoint = GetNextFitMicrocode (MicrocodeFmpPrivate, MicrocodeEntryPoint);
      if (NextMicrocodeEntryPoint != NULL) {
        *AvailableSize = (UINTN) NextMicrocodeEntryPoint - (UINTN) MicrocodeEntryPoint;
      } else {
        *AvailableSize = (UINTN) MicrocodePatchAddress + MicrocodePatchRegionSize - (UINTN) MicrocodeEntryPoint;
      }
      if (*AvailableSize >= ImageSize) {
        return MicrocodeEntryPoint;
      }
    }
  }

  return NULL;
}

/**
  Find unused FIT Microcode entrypoint.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  ImageSize                  The size of Microcode image buffer in bytes.
  @param[out] AvailableSize              Available size of the unused FIT Microcode entrypoint.

  @return Unused FIT Microcode entrypoint.
**/
CPU_MICROCODE_HEADER *
FindUnusedFitMicrocode (
  IN MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN UINTN                                   ImageSize,
  OUT UINTN                                  *AvailableSize
  )
{
  UINTN                                   Index;
  UINTN                                   CpuIndex;
  CPU_MICROCODE_HEADER                    *MicrocodeEntryPoint;
  CPU_MICROCODE_HEADER                    *NextMicrocodeEntryPoint;
  VOID                                    *MicrocodePatchAddress;
  UINTN                                   MicrocodePatchRegionSize;

  MicrocodePatchAddress = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  //
  // Get uCode Slot that doesn't match uCode selection Policy
  //
  for (Index = 0; Index < MicrocodeFmpPrivate->FitMicrocodeEntryCount; Index++) {

    //
    //  If uCode in FIT slot matches uCode selection policy PCD, skip over it
    //
    for (CpuIndex = 0; CpuIndex <  MicrocodeFmpPrivate->ProcessorCount; CpuIndex++) {
      if (MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].MicrocodeIndex == Index) {
        break;
      }
    }

    if (CpuIndex < MicrocodeFmpPrivate->ProcessorCount) {
      continue;
    }

    MicrocodeEntryPoint = MicrocodeFmpPrivate->FitMicrocodeInfo[Index].MicrocodeEntryPoint;

    NextMicrocodeEntryPoint = GetNextFitMicrocode (MicrocodeFmpPrivate, MicrocodeEntryPoint);
    if (NextMicrocodeEntryPoint != NULL) {
      *AvailableSize = (UINTN) NextMicrocodeEntryPoint - (UINTN) MicrocodeEntryPoint;
    } else {
      *AvailableSize = (UINTN) MicrocodePatchAddress + MicrocodePatchRegionSize - (UINTN) MicrocodeEntryPoint;
    }

    if (*AvailableSize >= ImageSize) {
      return MicrocodeEntryPoint;
    }

  }

  DEBUG ((DEBUG_INFO, "Can't find right uCode slot that doesn't match uCode Selection Policy\n"));

  return NULL;
}

/**
  Get current Microcode used region size.

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data

  @return current Microcode used region size.
**/
UINTN
GetCurrentMicrocodeUsedRegionSize (
  IN MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate
  )
{
  if (MicrocodeFmpPrivate->DescriptorCount == 0) {
    return 0;
  }

  return (UINTN)MicrocodeFmpPrivate->MicrocodeInfo[MicrocodeFmpPrivate->DescriptorCount - 1].MicrocodeEntryPoint
         + (UINTN)MicrocodeFmpPrivate->MicrocodeInfo[MicrocodeFmpPrivate->DescriptorCount - 1].TotalSize
         - (UINTN)MicrocodeFmpPrivate->MicrocodePatchAddress;
}

/**
  Sync content in source flash region to dest flash region.

  Todo:  Ensure all Microcde are saved at 4KB boundary

  @param[in]  FlashRegionType        FlashRegionType that Src and Target flash region belong to
  @param[in]  Src                           Source flash region
  @param[in]  Dest                          Target flash region
  @param[in]  NumBytes                  Bytes to flash

  @retval EFI_SUCCESS             The dest image is written.
  @retval EFI_WRITE_PROTECTED     The flash device is read only.
**/
EFI_STATUS
SyncSpiFlashRegion (
  IN FLASH_REGION_TYPE                 FlashRegionType,
  IN UINT32                            Src,
  IN UINT32                            Dest,
  IN UINTN                             NumBytes
  )
{
  EFI_STATUS            Status;
  UINT8                 *FlashSrcBuf;
  FLASH_ACCESS_REQUEST  AccessReq;

  FlashSrcBuf = NULL;

  FlashSrcBuf = AllocatePool(NumBytes);
  if (FlashSrcBuf == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FUNC_EXIT;
  }

  ZeroMem (&AccessReq, sizeof (AccessReq));
  AccessReq.FlashRegionType = FlashRegionBios;
  AccessReq.FlashAddress    = (UINTN)Src;
  AccessReq.Data            = FlashSrcBuf;
  AccessReq.DataSize        = NumBytes;
  //
  // Caution!
  //    SpiFlashRead must not use MMIO for TopSwap flash region update
  //
  Status = FlashRead (&AccessReq);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Read Flash Source Region fail"));
    goto FUNC_EXIT;
  }

  //
  // Todo: Use UpdateMicrocode first. Will replace wtih byte level SpiFlashUpdate later
  //
  AccessReq.FlashAddress    = (UINTN)Dest;
  Status = FlashUpdate (&AccessReq, NULL, 0, 0);
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Write Flash Dest Region fail"));
    goto FUNC_EXIT;
  }


FUNC_EXIT:
  if (FlashSrcBuf != NULL) {
    FreePool(FlashSrcBuf);
  }

  return Status;
}

/**
  Update Microcode flash region with FIT.

  ToDo: skip reclaim. Reclaim could corrupt entire ucode range

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  TargetMicrocodeEntryPoint  Target Microcode entrypoint to be updated
  @param[in]  Image                      The Microcode image buffer.
  @param[in]  ImageSize                  The size of Microcode image buffer in bytes.
  @param[out] LastAttemptStatus          The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.

  @retval EFI_SUCCESS                    The Microcode image is written.
  @retval EFI_WRITE_PROTECTED            The flash device is read only.
**/
EFI_STATUS
UpdateMicrocodeFlashRegionWithFit (
  IN  MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN  CPU_MICROCODE_HEADER                    *TargetMicrocodeEntryPoint,
  IN  VOID                                    *Image,
  IN  UINTN                                   ImageSize,
  IN  VOID                                    *NewVersion,  OPTIONAL
  IN  UINTN                                   NewVersionSize, OPTIONAL
  OUT UINT32                                  *LastAttemptStatus
  )
{
  VOID                                    *MicrocodePatchAddress;
  UINTN                                   MicrocodePatchRegionSize;
  UINTN                                   TargetTotalSize;
  EFI_STATUS                              Status;
  VOID                                    *MicrocodePatchScratchBuffer;
  UINT8                                   *ScratchBufferPtr;
  UINTN                                   ScratchBufferSize;
  UINTN                                   RestSize;
  UINTN                                   AvailableSize;
  VOID                                    *NextMicrocodeEntryPoint;
  VOID                                    *EmptyFitMicrocodeEntry;
  VOID                                    *UnusedFitMicrocodeEntry;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS         PreviousProgress;
  FLASH_ACCESS_REQUEST                    UpdateReq;

  DEBUG ((DEBUG_INFO, "UpdateMicrocodeFlashRegionWithFit: Image - 0x%x, size - 0x%x\n", Image, ImageSize));
  MicrocodePatchAddress = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  MicrocodePatchScratchBuffer = AllocateZeroPool (MicrocodePatchRegionSize);
  if (MicrocodePatchScratchBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate Microcode Scratch buffer\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }
  ScratchBufferPtr = MicrocodePatchScratchBuffer;
  ScratchBufferSize = 0;

  IsPreviousUpdateUnfinished(&PreviousProgress);

  //
  // Step 2. Update uCode Major region.
  //
  // Target data collection
  //
  TargetTotalSize = 0;
  AvailableSize = 0;
  if (TargetMicrocodeEntryPoint != NULL) {
    if (TargetMicrocodeEntryPoint->DataSize == 0) {
      TargetTotalSize = 2048;
    } else {
      TargetTotalSize = TargetMicrocodeEntryPoint->TotalSize;
    }
    DEBUG ((DEBUG_INFO, "  TargetTotalSize - 0x%x\n", TargetTotalSize));
    NextMicrocodeEntryPoint = GetNextFitMicrocode (MicrocodeFmpPrivate, TargetMicrocodeEntryPoint);
    DEBUG ((DEBUG_INFO, "  NextMicrocodeEntryPoint - 0x%x\n", NextMicrocodeEntryPoint));
    if (NextMicrocodeEntryPoint != NULL) {
      ASSERT ((UINTN) NextMicrocodeEntryPoint >= ((UINTN) TargetMicrocodeEntryPoint + TargetTotalSize));
      AvailableSize = (UINTN) NextMicrocodeEntryPoint - (UINTN) TargetMicrocodeEntryPoint;
    } else {
      AvailableSize = (UINTN) MicrocodePatchAddress + MicrocodePatchRegionSize - (UINTN) TargetMicrocodeEntryPoint;
    }
    DEBUG ((DEBUG_INFO, "  AvailableSize - 0x%x\n", AvailableSize));
    ASSERT (AvailableSize >= TargetTotalSize);
  }
  //
  // Total Size means the Microcode size.
  // Available Size means the Microcode size plus the pad till (1) next Microcode or (2) the end.
  //
  // (1)
  // +------+-----------+-----+------+===================+
  // | MCU1 | Microcode | PAD | MCU2 |      Empty        |
  // +------+-----------+-----+------+===================+
  //        | TotalSize |
  //        |<-AvailableSize->|
  //
  // (2)
  // +------+-----------+===================+
  // | MCU  | Microcode |      Empty        |
  // +------+-----------+===================+
  //        | TotalSize |
  //        |<-      AvailableSize        ->|
  //

  //
  // Update based on policy
  //

  //
  // 1. If there is enough space to override old one in situ, replace old microcode in situ.
  //
  if (AvailableSize >= ImageSize) {
    DEBUG ((DEBUG_INFO, "Replace old microcode in situ\n"));
    //
    // +------+------------+------+===================+
    // |Other | Old Image  | ...  |      Empty        |
    // +------+------------+------+===================+
    //
    // +------+---------+--+------+===================+
    // |Other |New Image|FF| ...  |      Empty        |
    // +------+---------+--+------+===================+
    //
    // 1.1. Copy new image
    CopyMem (ScratchBufferPtr, Image, ImageSize);
    ScratchBufferSize += ImageSize;
    ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    // 1.2. Pad 0xFF
    RestSize = AvailableSize - ImageSize;
    if (RestSize > 0) {
      SetMem (ScratchBufferPtr, RestSize, 0xFF);
      ScratchBufferSize += RestSize;
      ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    }

    UpdateReq.FlashRegionType = FlashRegionBios;
    UpdateReq.FlashAddress    = (UINTN)TargetMicrocodeEntryPoint - PcdGet32(PcdBiosAreaBaseAddress);
    UpdateReq.Data        = MicrocodePatchScratchBuffer;
    UpdateReq.DataSize        = ScratchBufferSize;

    Status = FlashUpdate(&UpdateReq, NULL, 0, 0);
    goto CONTEXT_CLEAR;
  }

  //
  // 2. If there is empty FIT microcode entry with enough space, use it.
  //
  EmptyFitMicrocodeEntry = FindEmptyFitMicrocode (MicrocodeFmpPrivate, ImageSize, &AvailableSize);
  if (EmptyFitMicrocodeEntry != NULL) {
    DEBUG ((DEBUG_INFO, "Use empty FIT microcode entry\n"));
    // 2.1. Copy new image
    CopyMem (ScratchBufferPtr, Image, ImageSize);
    ScratchBufferSize += ImageSize;
    ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    // 2.2. Pad 0xFF
    RestSize = AvailableSize - ImageSize;
    if (RestSize > 0) {
      SetMem (ScratchBufferPtr, RestSize, 0xFF);
      ScratchBufferSize += RestSize;
    }

    UpdateReq.FlashRegionType = FlashRegionBios;
    UpdateReq.FlashAddress    = (UINTN) EmptyFitMicrocodeEntry - PcdGet32(PcdBiosAreaBaseAddress);
    UpdateReq.Data        = MicrocodePatchScratchBuffer;
    UpdateReq.DataSize        = ScratchBufferSize;

    Status = FlashUpdate (&UpdateReq, NULL, 0, 0);
    if (!EFI_ERROR (Status) && (TargetMicrocodeEntryPoint != NULL)) {
      //
      // Empty old microcode.
      //
      ScratchBufferPtr = MicrocodePatchScratchBuffer;
      SetMem (ScratchBufferPtr, TargetTotalSize, 0xFF);
      ScratchBufferSize = TargetTotalSize;
      ScratchBufferPtr = (UINT8 *) MicrocodePatchScratchBuffer + ScratchBufferSize;

      UpdateReq.FlashRegionType = FlashRegionBios;
      UpdateReq.FlashAddress    = (UINTN) TargetMicrocodeEntryPoint - PcdGet32(PcdBiosAreaBaseAddress);
      UpdateReq.Data      = MicrocodePatchScratchBuffer;
      UpdateReq.DataSize        = ScratchBufferSize;

      Status = FlashUpdate (&UpdateReq, NULL, 0, 0);
    }
    goto CONTEXT_CLEAR;
  }

  //
  // 3. If there is unused microcode entry with enough space, use it.
  //
  UnusedFitMicrocodeEntry = FindUnusedFitMicrocode (MicrocodeFmpPrivate, ImageSize, &AvailableSize);
  if (UnusedFitMicrocodeEntry != NULL) {
    DEBUG ((DEBUG_INFO, "Use unused FIT microcode entry\n"));
    // 3.1. Copy new image
    CopyMem (ScratchBufferPtr, Image, ImageSize);
    ScratchBufferSize += ImageSize;
    ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    // 3.2. Pad 0xFF
    RestSize = AvailableSize - ImageSize;
    if (RestSize > 0) {
      SetMem (ScratchBufferPtr, RestSize, 0xFF);
      ScratchBufferSize += RestSize;
    }

    UpdateReq.FlashRegionType = FlashRegionBios;
    UpdateReq.FlashAddress    = (UINTN) UnusedFitMicrocodeEntry - PcdGet32(PcdBiosAreaBaseAddress),
    UpdateReq.Data            = MicrocodePatchScratchBuffer;
    UpdateReq.DataSize        = ScratchBufferSize;

    Status = FlashUpdate (&UpdateReq, NULL, 0, 0);

    if (!EFI_ERROR (Status) && (TargetMicrocodeEntryPoint != NULL)) {
      //
      // Empty old microcode.
      //
      ScratchBufferPtr = MicrocodePatchScratchBuffer;
      SetMem (ScratchBufferPtr, TargetTotalSize, 0xFF);
      ScratchBufferSize = TargetTotalSize;
      ScratchBufferPtr = (UINT8 *) MicrocodePatchScratchBuffer + ScratchBufferSize;

      UpdateReq.FlashRegionType = FlashRegionBios;
      UpdateReq.FlashAddress    = (UINTN) TargetMicrocodeEntryPoint - PcdGet32(PcdBiosAreaBaseAddress),
      UpdateReq.Data            = MicrocodePatchScratchBuffer;
      UpdateReq.DataSize        = ScratchBufferSize;

      Status = FlashUpdate (&UpdateReq, NULL, 0, 0);

    }
    goto CONTEXT_CLEAR;
  }

  //
  // 4. No usable FIT microcode entry
  //    It is an expected failure. Recovery to interrupted update is not supprted
  //
  DEBUG ((DEBUG_ERROR, "No usable FIT microcode entry\n"));
  *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
  Status = EFI_OUT_OF_RESOURCES;

CONTEXT_CLEAR:
  //
  // 5. Update Version FFS if update succeed
  //
  if (!EFI_ERROR(Status)) {
    if (NewVersion != NULL && NewVersionSize >= sizeof(INTEL_MICROCODE_VERSION_FFS_DATA)) {
      Status = UpdateVersionInfo (
                 (INTEL_MICROCODE_VERSION_FFS_DATA *)NewVersion,
                 NewVersionSize
                 );
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "Update VersionFfs fail!\n"));
        goto FUNC_EXIT;
      }
      DEBUG ((DEBUG_INFO, "Update VersionFfs succeed!\n"));
    }
  }

  ClearUpdateProgress ();


FUNC_EXIT:

  if (!EFI_ERROR(Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
  } else {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  }

  //
  // Todo: May need to add memory free for FIT case
  //
  //
  if (MicrocodePatchScratchBuffer != NULL) {
    FreePool(MicrocodePatchScratchBuffer);
  }

  return Status;
}


/**
  Update Microcode flash region.

  Todo:  Ensure all Microcde are saved at 4KB boundary

  @param[in]  MicrocodeFmpPrivate        The Microcode driver private data
  @param[in]  TargetMicrocodeEntryPoint  Target Microcode entrypoint to be updated
  @param[in]  Image                      The Microcode image buffer.
  @param[in]  ImageSize                  The size of Microcode image buffer in bytes.
  @param[out] LastAttemptStatus          The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.

  @retval EFI_SUCCESS                    The Microcode image is written.
  @retval EFI_WRITE_PROTECTED            The flash device is read only.
**/
EFI_STATUS
UpdateMicrocodeFlashRegion (
  IN  MICROCODE_FMP_PRIVATE_DATA              *MicrocodeFmpPrivate,
  IN  CPU_MICROCODE_HEADER                    *TargetMicrocodeEntryPoint,
  IN  VOID                                    *Image,
  IN  UINTN                                   ImageSize,
  OUT UINT32                                  *LastAttemptStatus
  )
{
  VOID                                    *MicrocodePatchAddress;
  UINTN                                   MicrocodePatchRegionSize;
  UINTN                                   TargetTotalSize;
  UINTN                                   UsedRegionSize;
  EFI_STATUS                              Status;
  VOID                                    *MicrocodePatchScratchBuffer;
  UINT8                                   *ScratchBufferPtr;
  UINTN                                   ScratchBufferSize;
  UINTN                                   RestSize;
  UINTN                                   AvailableSize;
  VOID                                    *NextMicrocodeEntryPoint;
  MICROCODE_INFO                          *MicrocodeInfo;
  UINTN                                   MicrocodeCount;
  UINTN                                   Index;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS         PreviousProgress;
  FLASH_ACCESS_REQUEST                    UpdateReq;


  DEBUG ((DEBUG_INFO, "UpdateMicrocodeFlashRegion: Image - 0x%x, size - 0x%x\n", Image, ImageSize));

  MicrocodePatchAddress    = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  MicrocodePatchScratchBuffer = AllocateZeroPool (MicrocodePatchRegionSize);
  if (MicrocodePatchScratchBuffer == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate Microcode Scratch buffer\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }
  ScratchBufferPtr = MicrocodePatchScratchBuffer;
  ScratchBufferSize = 0;

  IsPreviousUpdateUnfinished(&PreviousProgress);

  //
  // Step 2. Update uCode Major region.
  // Before this function, TopSwap region is now a mirror to Major uCode region after syncing with Major uCode,.
  // After switching TopSwap bit,  MMIO will be routed to TopSwap region and get same content as from Major region
  //
  DEBUG ((DEBUG_ERROR, "MicrocodeUpdate Phase 1 Start\n"));

  //
  // Target data collection
  //
  TargetTotalSize = 0;
  AvailableSize = 0;
  NextMicrocodeEntryPoint = NULL;
  if (TargetMicrocodeEntryPoint != NULL) {
    if (TargetMicrocodeEntryPoint->DataSize == 0) {
      TargetTotalSize = 2048;
    } else {
      TargetTotalSize = TargetMicrocodeEntryPoint->TotalSize;
    }
    DEBUG ((DEBUG_INFO, "  TargetTotalSize - 0x%x\n", TargetTotalSize));
    NextMicrocodeEntryPoint = GetNextMicrocode(MicrocodeFmpPrivate, TargetMicrocodeEntryPoint);
    DEBUG ((DEBUG_INFO, "  NextMicrocodeEntryPoint - 0x%x\n", NextMicrocodeEntryPoint));
    if (NextMicrocodeEntryPoint != NULL) {
      ASSERT ((UINTN)NextMicrocodeEntryPoint >= ((UINTN)TargetMicrocodeEntryPoint + TargetTotalSize));
      AvailableSize = (UINTN)NextMicrocodeEntryPoint - (UINTN)TargetMicrocodeEntryPoint;
    } else {
      AvailableSize = (UINTN)MicrocodePatchAddress + MicrocodePatchRegionSize - (UINTN)TargetMicrocodeEntryPoint;
    }
    DEBUG ((DEBUG_INFO, "  AvailableSize - 0x%x\n", AvailableSize));
    ASSERT (AvailableSize >= TargetTotalSize);
  }
  UsedRegionSize = GetCurrentMicrocodeUsedRegionSize(MicrocodeFmpPrivate);
  DEBUG ((DEBUG_INFO, "  UsedRegionSize - 0x%x\n", UsedRegionSize));
  ASSERT (UsedRegionSize >= TargetTotalSize);
  if (TargetMicrocodeEntryPoint != NULL) {
    ASSERT ((UINTN)MicrocodePatchAddress + UsedRegionSize >= ((UINTN)TargetMicrocodeEntryPoint + TargetTotalSize));
  }
  //
  // Total Size means the Microcode size.
  // Available Size means the Microcode size plus the pad till (1) next Microcode or (2) the end.
  //
  // (1)
  // +------+-----------+-----+------+===================+
  // | MCU1 | Microcode | PAD | MCU2 |      Empty        |
  // +------+-----------+-----+------+===================+
  //        | TotalSize |
  //        |<-AvailableSize->|
  // |<-        UsedRegionSize     ->|
  //
  // (2)
  // +------+-----------+===================+
  // | MCU  | Microcode |      Empty        |
  // +------+-----------+===================+
  //        | TotalSize |
  //        |<-      AvailableSize        ->|
  // |<-UsedRegionSize->|
  //

  //
  // Update based on policy
  //

  //
  // 1. If there is enough space to update old one in situ, replace old microcode in situ.
  //
  if (AvailableSize >= ImageSize) {
    DEBUG ((DEBUG_INFO, "Replace old microcode in situ\n"));
    //
    // +------+------------+------+===================+
    // |Other | Old Image  | ...  |      Empty        |
    // +------+------------+------+===================+
    //
    // +------+---------+--+------+===================+
    // |Other |New Image|FF| ...  |      Empty        |
    // +------+---------+--+------+===================+
    //
    // 1.1. Copy new image
    CopyMem (ScratchBufferPtr, Image, ImageSize);
    ScratchBufferSize += ImageSize;
    ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    // 1.2. Pad 0xFF
    RestSize = AvailableSize - ImageSize;
    if (RestSize > 0) {
      SetMem (ScratchBufferPtr, RestSize, 0xFF);
      ScratchBufferSize += RestSize;
      ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    }

    UpdateReq.FlashRegionType = FlashRegionBios;
    UpdateReq.FlashAddress    = (UINTN)TargetMicrocodeEntryPoint - PcdGet32(PcdBiosAreaBaseAddress);
    UpdateReq.Data            = MicrocodePatchScratchBuffer;
    UpdateReq.DataSize        = ScratchBufferSize;

    Status = FlashUpdate(&UpdateReq, NULL, 0, 0);

    goto CONTEXT_CLEAR;
  }

  //
  // 2. If there is enough space to remove old one and add new one, reorg and replace old microcode.
  //
  if (MicrocodePatchRegionSize - (UsedRegionSize - TargetTotalSize) >= ImageSize) {
    if (TargetMicrocodeEntryPoint == NULL) {
      DEBUG ((DEBUG_INFO, "Append new microcode\n"));
      //
      // +------+------------+------+===================+
      // |Other1|   Other    |Other2|      Empty        |
      // +------+------------+------+===================+
      //
      // +------+------------+------+-----------+=======+
      // |Other1|   Other    |Other2| New Image | Empty |
      // +------+------------+------+-----------+=======+
      //
      UpdateReq.FlashRegionType = FlashRegionBios;
      UpdateReq.FlashAddress    = (UINTN)MicrocodePatchAddress + UsedRegionSize - FixedPcdGet32(PcdBiosAreaBaseAddress),
      UpdateReq.Data            = Image;
      UpdateReq.DataSize        = ImageSize;

      Status = FlashUpdate(&UpdateReq, NULL, 0, 0);
    } else {
      DEBUG ((DEBUG_INFO, "Reorg and replace old microcode\n"));
      //
      // +------+------------+------+===================+
      // |Other | Old Image  | ...  |      Empty        |
      // +------+------------+------+===================+
      //
      // +------+---------------+------+================+
      // |Other |   New Image   | ...  |      Empty     |
      // +------+---------------+------+================+
      //
      // 2.1. Copy new image
      CopyMem (ScratchBufferPtr, Image, ImageSize);
      ScratchBufferSize += ImageSize;
      ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
      // 2.2. Copy rest images after the old image.
      if (NextMicrocodeEntryPoint != 0) {
        RestSize = (UINTN)MicrocodePatchAddress + UsedRegionSize - ((UINTN)NextMicrocodeEntryPoint);
        CopyMem (ScratchBufferPtr, NextMicrocodeEntryPoint, RestSize);
        ScratchBufferSize += RestSize;
        ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
      }

      UpdateReq.FlashRegionType = FlashRegionBios;
      UpdateReq.FlashAddress  = (UINTN)TargetMicrocodeEntryPoint - PcdGet32(PcdBiosAreaBaseAddress);
      UpdateReq.Data      = MicrocodePatchScratchBuffer;
      UpdateReq.DataSize    = ScratchBufferSize;

      Status = FlashUpdate(&UpdateReq, NULL, 0, 0);
    }
    goto CONTEXT_CLEAR;
  }

  //
  // 3. The new image can be put in MCU region, but not all others can be put.
  //    So all the unused MCU is removed.
  //
  if (MicrocodePatchRegionSize >= ImageSize) {
    //
    // +------+------------+------+===================+
    // |Other1| Old Image  |Other2|      Empty        |
    // +------+------------+------+===================+
    //
    // +-------------------------------------+--------+
    // |        New Image                    | Other  |
    // +-------------------------------------+--------+
    //
    DEBUG ((DEBUG_INFO, "Add new microcode from beginning\n"));

    MicrocodeCount = MicrocodeFmpPrivate->DescriptorCount;
    MicrocodeInfo = MicrocodeFmpPrivate->MicrocodeInfo;

    // 3.1. Copy new image
    CopyMem (ScratchBufferPtr, Image, ImageSize);
    ScratchBufferSize += ImageSize;
    ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    // 3.2. Copy some others to rest buffer
    for (Index = 0; Index < MicrocodeCount; Index++) {
      if (!MicrocodeInfo[Index].InUse) {
        continue;
      }
      if (MicrocodeInfo[Index].MicrocodeEntryPoint == TargetMicrocodeEntryPoint) {
        continue;
      }
      if (MicrocodeInfo[Index].TotalSize <= MicrocodePatchRegionSize - ScratchBufferSize) {
        CopyMem (ScratchBufferPtr, MicrocodeInfo[Index].MicrocodeEntryPoint, MicrocodeInfo[Index].TotalSize);
        ScratchBufferSize += MicrocodeInfo[Index].TotalSize;
        ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
      }
    }
    // 3.3. Pad 0xFF
    RestSize = MicrocodePatchRegionSize - ScratchBufferSize;
    if (RestSize > 0) {
      SetMem (ScratchBufferPtr, RestSize, 0xFF);
      ScratchBufferSize += RestSize;
      ScratchBufferPtr = (UINT8 *)MicrocodePatchScratchBuffer + ScratchBufferSize;
    }

    UpdateReq.FlashRegionType = FlashRegionBios;
    UpdateReq.FlashAddress    = (UINTN)MicrocodePatchAddress - PcdGet32(PcdBiosAreaBaseAddress);
    UpdateReq.Data            = MicrocodePatchScratchBuffer;
    UpdateReq.DataSize        = ScratchBufferSize;

    Status = FlashUpdate(&UpdateReq, NULL, 0, 0);

    goto CONTEXT_CLEAR;
  }

  //
  // 4. The new image size is bigger than the whole MCU region.
  //    It is an expected failure. Recovery to interrupted update is not supprted
  //
  DEBUG ((DEBUG_ERROR, "Microcode too big\n"));
  *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
  Status = EFI_OUT_OF_RESOURCES;

CONTEXT_CLEAR:

  ClearUpdateProgress ();

//FUNC_EXIT:

  if (!EFI_ERROR(Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
  } else {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  }

  DEBUG((DEBUG_ERROR, "MicrocodeUpdate Phase 1 End. Status 0x%x\n", Status));

  if (MicrocodePatchScratchBuffer != NULL) {
    FreePool(MicrocodePatchScratchBuffer);
  }

  return Status;
}


/**
  Sort FIT microcode entries based upon MicrocodeEntryPoint, from low to high.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.

**/
VOID
SortFitMicrocodeInfo (
  IN MICROCODE_FMP_PRIVATE_DATA     *MicrocodeFmpPrivate
  )
{
  FIT_MICROCODE_INFO        *FitMicrocodeEntry;
  FIT_MICROCODE_INFO        *NextFitMicrocodeEntry;
  FIT_MICROCODE_INFO        TempFitMicrocodeEntry;
  FIT_MICROCODE_INFO        *FitMicrocodeEntryEnd;

  FitMicrocodeEntry = MicrocodeFmpPrivate->FitMicrocodeInfo;
  NextFitMicrocodeEntry = FitMicrocodeEntry + 1;
  FitMicrocodeEntryEnd = MicrocodeFmpPrivate->FitMicrocodeInfo + MicrocodeFmpPrivate->FitMicrocodeEntryCount;
  while (FitMicrocodeEntry < FitMicrocodeEntryEnd) {
    while (NextFitMicrocodeEntry < FitMicrocodeEntryEnd) {
      if (FitMicrocodeEntry->MicrocodeEntryPoint > NextFitMicrocodeEntry->MicrocodeEntryPoint) {
        CopyMem (&TempFitMicrocodeEntry, FitMicrocodeEntry, sizeof (FIT_MICROCODE_INFO));
        CopyMem (FitMicrocodeEntry, NextFitMicrocodeEntry, sizeof (FIT_MICROCODE_INFO));
        CopyMem (NextFitMicrocodeEntry, &TempFitMicrocodeEntry, sizeof (FIT_MICROCODE_INFO));
      }

      NextFitMicrocodeEntry = NextFitMicrocodeEntry + 1;
    }

    FitMicrocodeEntry     = FitMicrocodeEntry + 1;
    NextFitMicrocodeEntry = FitMicrocodeEntry + 1;
  }
}

/**
  Write Microcode.

  Caution: This function may receive untrusted input.

  @param[in]   MicrocodeFmpPrivate       The Microcode driver private data
  @param[in]   Image                     The Microcode image buffer.
  @param[in]   ImageSize                 The size of Microcode image buffer in bytes.
  @param[out]  LastAttemptVersion        The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  LastAttemptStatus         The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  AbortReason               A pointer to a pointer to a null-terminated string providing more
                                         details for the aborted operation. The buffer is allocated by this function
                                         with AllocatePool(), and it is the caller's responsibility to free it with a
                                         call to FreePool().

  @retval      EFI_SUCCESS               The Microcode image is written.
  @retval      EFI_VOLUME_CORRUPTED      The Microcode image is corrupted.
  @retval      EFI_INCOMPATIBLE_VERSION  The Microcode image version is incorrect.
  @retval      EFI_SECURITY_VIOLATION    The Microcode image fails to load.
  @retval      EFI_WRITE_PROTECTED       The flash device is read only.
**/
EFI_STATUS
EFIAPI
MicrocodeSlotSpiWrite (
  IN  MICROCODE_FMP_PRIVATE_DATA        *MicrocodeFmpPrivate,
  IN  VOID                              *Image,
  IN  UINTN                             ImageSize,
  IN  VOID                              *NewVersion,  OPTIONAL
  IN  UINTN                             NewVersionSize, OPTIONAL
  OUT UINT32                            *LastAttemptVersion,
  OUT UINT32                            *LastAttemptStatus,
  OUT CHAR16                            **AbortReason
  )
{
  EFI_STATUS                              Status;
  VOID                                    *AlignedImage;
  CPU_MICROCODE_HEADER                    *TargetMicrocodeEntryPoint;
  UINTN                                   TargetMicrcodeIndex;
  UINTN                                   Index;
  UINTN                                   TargetCpuIndex;
  UINTN                                   CpuIndex;
  PROCESSOR_INFO                          *ProcessorInfo;
  UINTN                                   *TargetProcessorBuf;
  UINTN                                   TargetProcessorNum;
  MICROCODE_INFO                          *MicrocodeInfo;


  AlignedImage                     = Image;
  TargetMicrocodeEntryPoint        = NULL;
  ProcessorInfo                    = MicrocodeFmpPrivate->ProcessorInfo;
  TargetMicrcodeIndex              = (UINTN) - 1;

  //
  // MCU must be 16 bytes aligned
  //
  AlignedImage = AllocateCopyPool(ImageSize, Image);
  if (AlignedImage == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate aligned image\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }

  TargetProcessorBuf = AllocateZeroPool(sizeof(UINTN) * MicrocodeFmpPrivate->ProcessorCount);
  if (TargetProcessorBuf == NULL) {
    Status = EFI_OUT_OF_RESOURCES;
    goto FUNC_EXIT;
  }

  DEBUG ((DEBUG_INFO, "Start to verify new uCode!\n"));

  //
  // Check all physical/logic CPUs that new uCode can apply to
  //
  for (CpuIndex = 0, TargetProcessorNum = 0; CpuIndex < MicrocodeFmpPrivate->ProcessorCount; CpuIndex++) {
    Status = VerifyMicrocode(
               MicrocodeFmpPrivate,
               AlignedImage,
               ImageSize,
               FALSE,
               LastAttemptStatus,
               AbortReason,
               &CpuIndex
               );

    //
    // Update must make sure new uCode can apply to CPU and version is higher than existing one.
    //
    if (Status == EFI_SUCCESS) {

      MicrocodeInfo = &MicrocodeFmpPrivate->MicrocodeInfo[ProcessorInfo[CpuIndex].MicrocodeIndex];

      DEBUG ((DEBUG_INFO, " New uCode Target CpuIndex found- 0x%x\n", CpuIndex));
      DEBUG ((DEBUG_INFO, " New uCod Version 0x%x\n", ((CPU_MICROCODE_HEADER *)AlignedImage)->UpdateRevision));
      DEBUG ((DEBUG_INFO, " Existing uCode Revision 0x%x\n", MicrocodeInfo->MicrocodeEntryPoint->UpdateRevision));

      if ((ProcessorInfo[CpuIndex].MicrocodeIndex == (UINT32) -1 ||
          ((CPU_MICROCODE_HEADER *)AlignedImage)->UpdateRevision > MicrocodeInfo->MicrocodeEntryPoint->UpdateRevision)) {
        TargetProcessorBuf[TargetProcessorNum] = CpuIndex;
        TargetProcessorNum++;
      }
    }
  }

  if (TargetProcessorNum == 0) {
    //
    // New uCode is not in the uCode selection policy list. Ignore this uCode
    //
    DEBUG ((DEBUG_ERROR, "New uCode fails in verification! Quit Update.\n"));
    Status = EFI_INVALID_PARAMETER;
    goto FUNC_EXIT;
  }

  //
  // Find a replacable uCode existing on flash. It has a smaller or equivalent CPU support set comparing with new uCode
  // regarding platform supported CPU set.
  //
  for (Index = 0; Index < TargetProcessorNum; Index++) {
    TargetCpuIndex = TargetProcessorBuf[Index];

    //
    // New uCode targeting CPU don't have an exisiting uCode to replace. Skip it
    //
    if (ProcessorInfo[TargetCpuIndex].MicrocodeIndex == (UINTN) -1) {
      continue;
    }

    for (CpuIndex = 0; CpuIndex < MicrocodeFmpPrivate->ProcessorCount; CpuIndex++) {
      //
      // Only check Processor beyond new uCode support list
      //
      if (IsCpuInTargetCpuList(CpuIndex, TargetProcessorBuf, TargetProcessorNum)) {
        continue;
      }

      //
      // If existing uCode is supporting a larger Processor list than new uCode ,  it can't be replcaed
      //
      if (ProcessorInfo[CpuIndex].MicrocodeIndex == ProcessorInfo[TargetCpuIndex].MicrocodeIndex) {
        break;
      }
    }

    //
    // Found an exisintg uCode that support no more processors beyond new uCode.  It is replacable candidate
    //
    if (CpuIndex == MicrocodeFmpPrivate->ProcessorCount) {
      TargetMicrcodeIndex = MicrocodeFmpPrivate->ProcessorInfo[TargetCpuIndex].MicrocodeIndex;
      break;
    }
  }

  DEBUG ((DEBUG_INFO, "  TargetMicrocodeIndex - 0x%x\n", TargetMicrcodeIndex));

  if (TargetMicrcodeIndex != (UINTN)-1) {
    ASSERT (TargetMicrcodeIndex < MicrocodeFmpPrivate->DescriptorCount);
    TargetMicrocodeEntryPoint = MicrocodeFmpPrivate->MicrocodeInfo[TargetMicrcodeIndex].MicrocodeEntryPoint;
  } else {
    TargetMicrocodeEntryPoint = NULL;
  }

  DEBUG ((DEBUG_INFO, "  TargetMicrocodeEntryPoint - 0x%x\n", TargetMicrocodeEntryPoint));
  DEBUG ((DEBUG_INFO, "  Try to update, Fit mode or Non-fit mode!\n"));

  //
  // Record uCode update progress. Used for Seamless recovery from interrupted update
  //
  if (MicrocodeFmpPrivate->FitMicrocodeInfo != NULL) {
    Status = UpdateMicrocodeFlashRegionWithFit (
               MicrocodeFmpPrivate,
               TargetMicrocodeEntryPoint,
               AlignedImage,
               ImageSize,
               NewVersion,
               NewVersionSize,
               LastAttemptStatus
               );
  } else {
    Status = UpdateMicrocodeFlashRegion (
               MicrocodeFmpPrivate,
               TargetMicrocodeEntryPoint,
               AlignedImage,
               ImageSize,
               LastAttemptStatus
               );
  }

  *LastAttemptVersion = ((INTEL_MICROCODE_VERSION_FFS_DATA *)NewVersion)->Version;

FUNC_EXIT:

  FreePool(AlignedImage);

  if (TargetProcessorBuf!= NULL) {
    FreePool(TargetProcessorBuf);
  }

  return Status;
}

/**
  Write Microcode.

  Caution: This function may receive untrusted input.

  @param[in]   MicrocodeFmpPrivate The Microcode driver private data
  @param[in]   Image               The Microcode image buffer.
  @param[in]   ImageSize           The size of Microcode image buffer in bytes.
  @param[out]  LastAttemptVersion  The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  LastAttemptStatus   The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  AbortReason         A pointer to a pointer to a null-terminated string providing more
                                   details for the aborted operation. The buffer is allocated by this function
                                   with AllocatePool(), and it is the caller's responsibility to free it with a
                                   call to FreePool().

  @retval EFI_SUCCESS               The Microcode image is written.
  @retval EFI_VOLUME_CORRUPTED      The Microcode image is corrupted.
  @retval EFI_INCOMPATIBLE_VERSION  The Microcode image version is incorrect.
  @retval EFI_SECURITY_VIOLATION    The Microcode image fails to load.
  @retval EFI_WRITE_PROTECTED       The flash device is read only.
**/
EFI_STATUS
EFIAPI
MicrocodeSpiWrite (
  IN  MICROCODE_FMP_PRIVATE_DATA   *MicrocodeFmpPrivate,
  IN  VOID                         *Image,
  IN  UINTN                        ImageSize,
  OUT UINT32                       *LastAttemptVersion,
  OUT UINT32                       *LastAttemptStatus,
  OUT CHAR16                       **AbortReason
  )
{
  EFI_STATUS                              Status;
  VOID                                    *AlignedImage;
  FLASH_ACCESS_REQUEST                    AccessReq;

  AlignedImage                     = Image;

  if (ImageSize > FixedPcdGet32(PcdFlashFvMicrocodeSize)) {
    return EFI_INVALID_PARAMETER;
  }

  //
  // MCU must be 16 bytes aligned
  //
  AlignedImage = AllocateCopyPool(ImageSize, Image);
  if (AlignedImage == NULL) {
    DEBUG ((DEBUG_ERROR, "Fail to allocate aligned image\n"));
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_INSUFFICIENT_RESOURCES;
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // FaultTolerantTestPoint for uCode
  //

  AccessReq.FlashRegionType = FlashRegionBios;
  AccessReq.FlashAddress    = (UINTN)FixedPcdGet32(PcdFlashFvMicrocodeOffset);
  AccessReq.Data            = AlignedImage;
  AccessReq.DataSize        = ImageSize;

  DEBUG ((DEBUG_INFO, "Update uCode region - Start\n"));
  Status = FlashUpdate(&AccessReq, NULL, 0, 100);
  DEBUG ((DEBUG_INFO, "Update uCode region - Done\n"));

//FUNC_EXIT:

  if (!EFI_ERROR(Status)) {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_SUCCESS;
  } else {
    *LastAttemptStatus = LAST_ATTEMPT_STATUS_ERROR_UNSUCCESSFUL;
  }

  FreePool(AlignedImage);

  return Status;
}

/**
  Write Microcode.

  Caution: This function may receive untrusted input.

  @param[in]   MicrocodeFmpPrivate The Microcode driver private data
  @param[in]   Image               The Microcode image buffer.
  @param[in]   ImageSize           The size of Microcode image buffer in bytes.
  @param[out]  LastAttemptVersion  The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  LastAttemptStatus   The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  AbortReason         A pointer to a pointer to a null-terminated string providing more
                                   details for the aborted operation. The buffer is allocated by this function
                                   with AllocatePool(), and it is the caller's responsibility to free it with a
                                   call to FreePool().

  @retval EFI_SUCCESS               The Microcode image is written.
  @retval EFI_VOLUME_CORRUPTED      The Microcode image is corrupted.
  @retval EFI_INCOMPATIBLE_VERSION  The Microcode image version is incorrect.
  @retval EFI_SECURITY_VIOLATION    The Microcode image fails to load.
  @retval EFI_WRITE_PROTECTED       The flash device is read only.
**/
EFI_STATUS
EFIAPI
MicrocodeBiosGuardWrite (
  IN  MICROCODE_FMP_PRIVATE_DATA   *MicrocodeFmpPrivate,
  IN  VOID                         *Image,
  IN  UINTN                        ImageSize,
  IN  VOID                         *BgupImage,
  IN  UINTN                         BgupImageSize,
  OUT UINT32                       *LastAttemptVersion,
  OUT UINT32                       *LastAttemptStatus,
  OUT CHAR16                       **AbortReason
  )
{
  EFI_STATUS                       Status;
  FLASH_ACCESS_REQUEST             UpdateReq;

  //
  // Only support full uCode range update
  //
  if (ImageSize > PcdGet32(PcdFlashFvMicrocodeSize)) {
    DEBUG ((DEBUG_INFO, "[%a] ImageSize = 0x%x, PcdFlashFvMicrocodeSize = 0x%x\n", __FUNCTION__, ImageSize, PcdGet32(PcdFlashFvMicrocodeSize)));
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "BiosGuard Update BgupImage Size %x\n", BgupImageSize));

  //
  // FaultTolerantTestPoint for uCode
  //
  UpdateReq.FlashRegionType = FlashRegionBios;
  UpdateReq.Data          = Image;
  UpdateReq.DataSize      = ImageSize;
  UpdateReq.BgupImage     = BgupImage;
  UpdateReq.BgupImageSize = BgupImageSize;
  Status = FlashUpdate(&UpdateReq, NULL, 0, 0);

  DEBUG ((DEBUG_ERROR, "MicrocodeUpdate Phase 0 End. Status 0x%x\n", Status));
  if (EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "Sync Microcode TopSwap Region failed"));
    goto FUNC_EXIT;
  }

FUNC_EXIT:

  DEBUG ((DEBUG_ERROR, "MicrocodeUpdate Phase 1 End. Status 0x%x\n", Status));

  return Status;
}

/**
  Write Microcode.

  Caution: This function may receive untrusted input.

  @param[in]   MicrocodeFmpPrivate The Microcode driver private data
  @param[in]   Image               The Microcode image buffer.
  @param[in]   ImageSize           The size of Microcode image buffer in bytes.
  @param[in]   Progress            A function used by the driver to report the
                                   progress of the firmware update.
  @param[out]  LastAttemptVersion  The last attempt version, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  LastAttemptStatus   The last attempt status, which will be recorded in ESRT and FMP EFI_FIRMWARE_IMAGE_DESCRIPTOR.
  @param[out]  AbortReason         A pointer to a pointer to a null-terminated string providing more
                                   details for the aborted operation. The buffer is allocated by this function
                                   with AllocatePool(), and it is the caller's responsibility to free it with a
                                   call to FreePool().

  @retval EFI_SUCCESS              The Microcode image is written.
  @retval EFI_VOLUME_CORRUPTED     The Microcode image is corrupted.
  @retval EFI_INCOMPATIBLE_VERSION The Microcode image version is incorrect.
  @retval EFI_SECURITY_VIOLATION   The Microcode image fails to load.
  @retval EFI_WRITE_PROTECTED      The flash device is read only.
**/
EFI_STATUS
EFIAPI
MicrocodeWrite (
  IN  MICROCODE_FMP_PRIVATE_DATA                     *MicrocodeFmpPrivate,
  IN  VOID                                           *Image,
  IN  UINTN                                          ImageSize,
  IN  EFI_FIRMWARE_MANAGEMENT_UPDATE_IMAGE_PROGRESS  Progress,
  OUT UINT32                                         *LastAttemptVersion,
  OUT UINT32                                         *LastAttemptStatus,
  OUT CHAR16                                         **AbortReason
  )
{
  EFI_STATUS                            Status;
  UCODE_PAYLOAD_ENTRY                   *Entry;
  UINTN                                 EntryCount;
  UINT8                                 *AlignedImage;
  SYSTEM_FIRMWARE_UPDATE_PROGRESS       PreviousProgress;

  Entry                            = NULL;
  AlignedImage                     = NULL;

  DEBUG ((DEBUG_INFO, "[%a] Enter!\n", __FUNCTION__));

  if (Progress == NULL) {
    DEBUG ((DEBUG_ERROR, "MicrocodeWrite - Invalid progress callback\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // XDR RFC requres the Fixed-Length Array starting address to be UINT32 aligned
  // Aligned buffer will be used later on
  //
  if (((UINTN)Image % sizeof(UINT32)) != 0) {
    AlignedImage = AllocateCopyPool (ImageSize, Image);
    if (AlignedImage == NULL) {
      Status = EFI_OUT_OF_RESOURCES;
      goto EXIT;
    }
    Image = AlignedImage;
  }

  //
  // Extra All infomation from uCode Capsule payload. It is encoded
  //
  Status = ExtractuCodeCapsuleXdrData(Image, ImageSize, &Entry, &EntryCount);
  if (EFI_ERROR(Status) || Entry == NULL) {
    goto EXIT;
  }

  DEBUG ((DEBUG_INFO, "[%a] Extract XDR header from payload!\n", __FUNCTION__));

  IsPreviousUpdateUnfinished(&PreviousProgress);

  //
  // Print important flag before backup FVs in OBB Region and make IBBR
  //
  DEBUG ((DEBUG_INFO, "PcdBiosResiliencyEnable = %s\n", ((PcdGetBool (PcdBiosResiliencyEnable) == TRUE) ? (L"TRUE") : (L"FALSE"))));
  DEBUG ((DEBUG_INFO, "IsTopSwapEnabled        = %s\n", ((IsTopSwapEnabled () == TRUE) ? (L"TRUE") : (L"FALSE"))));
  DEBUG ((DEBUG_INFO, "Previous uCode Capsule Update Progress:\n  Component = %d\n  Progress = %d\n", (PreviousProgress.Component), (PreviousProgress.Progress)));

  //
  // There are 3 update modes.  Update policies are different
  //    1) BiosGuard enable,  only support full uCode range update. Do not consider
  //    2) BiosGuard disable,  full uCode range SPI update
  //    3) BiosGuard disable,  flexible uCode slot update controlled by uCode Selection PCD
  //
  Progress(51);

  if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    DEBUG ((DEBUG_INFO, "Set TopSwap before update!\n"));
    TopSwapControl (TRUE);
  }

  if (IsBiosGuardEnabled ()) {
    DEBUG ((DEBUG_INFO, "Microcode full range update (BiosGuard Enable)\n"));
    if (EntryCount < (uCodeFullRangeBgupIndex + 1) || Entry[uCodeFullRangeBgupIndex].EntrySize == 0) {
      DEBUG ((DEBUG_INFO, "Need a bgup payload capsule file.\n"));
      Status = EFI_INVALID_PARAMETER;
      goto EXIT;
    }
    Status = MicrocodeBiosGuardWrite (
               MicrocodeFmpPrivate,
               Entry[uCodeFullRangeIndex].EntryPtr,
               Entry[uCodeFullRangeIndex].EntrySize,
               Entry[uCodeFullRangeBgupIndex].EntryPtr,
               Entry[uCodeFullRangeBgupIndex].EntrySize,
               LastAttemptVersion,
               LastAttemptStatus,
               AbortReason
               );
  } else if (Entry[uCodeFullRangeIndex].EntrySize > 0) {
    DEBUG ((DEBUG_INFO, "Microcode full range update (BiosGuard Disable)\n"));
    Status = MicrocodeSpiWrite(
               MicrocodeFmpPrivate,
               Entry[uCodeFullRangeIndex].EntryPtr,
               Entry[uCodeFullRangeIndex].EntrySize,
               LastAttemptVersion,
               LastAttemptStatus,
               AbortReason
               );
  } else if (EntryCount == uCodeArrayMax && Entry[uCodeFullRangeIndex].EntrySize == 0 && Entry[uCodeArrayIndex].EntrySize > 0) {
    DEBUG ((DEBUG_INFO, "Microcode slot update mode (BiosGuard Disabled)\n"));
    Progress(61);
    Status = MicrocodeSlotSpiWrite(
               MicrocodeFmpPrivate,
               Entry[uCodeArrayIndex].EntryPtr,
               Entry[uCodeArrayIndex].EntrySize,
               Entry[uCodeVersionIndex].EntryPtr,
               Entry[uCodeVersionIndex].EntrySize,
               LastAttemptVersion,
               LastAttemptStatus,
               AbortReason
               );
  } else {
    ASSERT(FALSE);
    Status = EFI_INVALID_PARAMETER;
    DEBUG ((DEBUG_INFO, "[%a] Invalid capsule type!", __FUNCTION__));
  }

  if (GetBiosResiliencyType () == SUPPORT_BIOS_RESILIENCY_RECOVERY) {
    TopSwapControl (FALSE);
  }

  Progress(90);
  DEBUG((DEBUG_INFO, "Microcode update status 0x%x\n", Status));

EXIT:

  if (Entry != NULL) {
    FreePool(Entry);
  }

  if (AlignedImage != NULL) {
    FreePool(AlignedImage);
  }

  return Status;
}



/**
  Initialize FIT microcode information.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.

  @return EFI_SUCCESS           FIT microcode information is initialized.
  @return EFI_OUT_OF_RESOURCES  No enough resource for the initialization.
  @return EFI_DEVICE_ERROR      There is something wrong in FIT microcode entry.
**/
EFI_STATUS
InitializeFitMicrocodeInfo (
  IN MICROCODE_FMP_PRIVATE_DATA     *MicrocodeFmpPrivate
  )
{
  UINT64                            FitPointer;
  FIRMWARE_INTERFACE_TABLE_ENTRY    *FitEntry;
  UINT32                            EntryNum;
  UINT32                            MicrocodeEntryNum;
  UINT32                            Index;
  UINTN                             Address;
  VOID                              *MicrocodePatchAddress;
  UINTN                             MicrocodePatchRegionSize;
  FIT_MICROCODE_INFO                *FitMicrocodeInfo;
  FIT_MICROCODE_INFO                *FitMicrocodeInfoNext;
  CPU_MICROCODE_HEADER              *MicrocodeEntryPoint;
  CPU_MICROCODE_HEADER              *MicrocodeEntryPointNext;
  UINTN                             FitMicrocodeIndex;
  MICROCODE_INFO                    *MicrocodeInfo;
  UINTN                             MicrocodeIndex;

  if (MicrocodeFmpPrivate->FitMicrocodeInfo != NULL) {
    FreePool (MicrocodeFmpPrivate->FitMicrocodeInfo);
    MicrocodeFmpPrivate->FitMicrocodeInfo = NULL;
    MicrocodeFmpPrivate->FitMicrocodeEntryCount = 0;
  }

  FitPointer = *(UINT64 *) (UINTN) FIT_POINTER_ADDRESS;
  if ((FitPointer == 0) ||
      (FitPointer == 0xFFFFFFFFFFFFFFFF) ||
      (FitPointer == 0xEEEEEEEEEEEEEEEE)) {
    //
    // No FIT table.
    //
    return EFI_SUCCESS;
  }
  FitEntry = (FIRMWARE_INTERFACE_TABLE_ENTRY *) (UINTN) FitPointer;
  if ((FitEntry[0].Type != FIT_TYPE_00_HEADER) ||
      (FitEntry[0].Address != FIT_TYPE_00_SIGNATURE)) {
    //
    // Invalid FIT table, treat it as no FIT table.
    //
    return EFI_SUCCESS;
  }

  EntryNum = *(UINT32 *)(&FitEntry[0].Size[0]) & 0xFFFFFF;

  //
  // Calculate microcode entry number.
  //
  MicrocodeEntryNum = 0;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_01_MICROCODE) {
      MicrocodeEntryNum++;
    }
  }
  if (MicrocodeEntryNum == 0) {
    //
    // No FIT microcode entry.
    //
    return EFI_SUCCESS;
  }

  //
  // Allocate buffer.
  //
  MicrocodeFmpPrivate->FitMicrocodeInfo = AllocateZeroPool (MicrocodeEntryNum * sizeof (FIT_MICROCODE_INFO));
  if (MicrocodeFmpPrivate->FitMicrocodeInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MicrocodeFmpPrivate->FitMicrocodeEntryCount = MicrocodeEntryNum;

  MicrocodePatchAddress = MicrocodeFmpPrivate->MicrocodePatchAddress;
  MicrocodePatchRegionSize = MicrocodeFmpPrivate->MicrocodePatchRegionSize;

  //
  // Collect microcode entry info.
  //
  MicrocodeEntryNum = 0;
  for (Index = 0; Index < EntryNum; Index++) {
    if (FitEntry[Index].Type == FIT_TYPE_01_MICROCODE) {
      Address = (UINTN) FitEntry[Index].Address;
      if ((Address < (UINTN) MicrocodePatchAddress) ||
          (Address >= ((UINTN) MicrocodePatchAddress + MicrocodePatchRegionSize))) {
        DEBUG ((
          DEBUG_ERROR,
          "InitializeFitMicrocodeInfo - Address (0x%x) is not in Microcode Region\n",
          Address
          ));
        goto ErrorExit;
      }
      FitMicrocodeInfo = &MicrocodeFmpPrivate->FitMicrocodeInfo[MicrocodeEntryNum];
      FitMicrocodeInfo->MicrocodeEntryPoint = (CPU_MICROCODE_HEADER *) Address;
      if ((*(UINT32 *) Address) == 0xFFFFFFFF) {
        //
        // It is the empty slot as long as the first dword is 0xFFFF_FFFF.
        //
        FitMicrocodeInfo->Empty = TRUE;
      } else {
        FitMicrocodeInfo->Empty = FALSE;
      }
      MicrocodeEntryNum++;
    }
  }

  //
  // Every microcode should have a FIT microcode entry.
  //
  for (MicrocodeIndex = 0; MicrocodeIndex < MicrocodeFmpPrivate->DescriptorCount; MicrocodeIndex++) {
    MicrocodeInfo = &MicrocodeFmpPrivate->MicrocodeInfo[MicrocodeIndex];
    for (FitMicrocodeIndex = 0; FitMicrocodeIndex < MicrocodeFmpPrivate->FitMicrocodeEntryCount; FitMicrocodeIndex++) {
      FitMicrocodeInfo = &MicrocodeFmpPrivate->FitMicrocodeInfo[FitMicrocodeIndex];
      if (MicrocodeInfo->MicrocodeEntryPoint == FitMicrocodeInfo->MicrocodeEntryPoint) {
        FitMicrocodeInfo->InUse = MicrocodeInfo->InUse;
        break;
      }
    }
    if (FitMicrocodeIndex >= MicrocodeFmpPrivate->FitMicrocodeEntryCount) {
      DEBUG ((
        DEBUG_ERROR,
        "InitializeFitMicrocodeInfo - There is no FIT microcode entry for Microcode (0x%x). It could be caused by incorrect uCode\n",
        MicrocodeInfo->MicrocodeEntryPoint
        ));
      ASSERT(FALSE);
      FreePool (MicrocodeFmpPrivate->FitMicrocodeInfo);
      MicrocodeFmpPrivate->FitMicrocodeEntryCount = 0;
      MicrocodeFmpPrivate->FitMicrocodeInfo       = NULL;

      return EFI_DEVICE_ERROR;
    }
  }

  SortFitMicrocodeInfo (MicrocodeFmpPrivate);

  //
  // Check overlap.
  //
  for (FitMicrocodeIndex = 0; FitMicrocodeIndex < MicrocodeFmpPrivate->FitMicrocodeEntryCount - 1; FitMicrocodeIndex++) {
    FitMicrocodeInfo = &MicrocodeFmpPrivate->FitMicrocodeInfo[FitMicrocodeIndex];
    MicrocodeEntryPoint = FitMicrocodeInfo->MicrocodeEntryPoint;
    FitMicrocodeInfoNext = &MicrocodeFmpPrivate->FitMicrocodeInfo[FitMicrocodeIndex + 1];
    MicrocodeEntryPointNext = FitMicrocodeInfoNext->MicrocodeEntryPoint;
    if ((MicrocodeEntryPoint >= MicrocodeEntryPointNext) ||
        ((FitMicrocodeInfo->TotalSize != 0) &&
         ((UINTN) MicrocodeEntryPoint + FitMicrocodeInfo->TotalSize) >
          (UINTN) MicrocodeEntryPointNext)) {
      DEBUG ((
        DEBUG_ERROR,
        "InitializeFitMicrocodeInfo - There is overlap between FIT microcode entries (0x%x 0x%x)\n",
        MicrocodeEntryPoint,
        MicrocodeEntryPointNext
        ));
      goto ErrorExit;
    }

    FitMicrocodeInfo->TotalSize = (UINTN)MicrocodeEntryPointNext - (UINTN)MicrocodeEntryPoint;
  }
  //
  // Handle last Fit entry totalsize
  //
  FitMicrocodeInfo = &MicrocodeFmpPrivate->FitMicrocodeInfo[FitMicrocodeIndex];
  FitMicrocodeInfo->TotalSize = (UINTN)MicrocodeFmpPrivate->MicrocodePatchAddress + MicrocodeFmpPrivate->MicrocodePatchRegionSize - (UINTN)FitMicrocodeInfo->MicrocodeEntryPoint;

  DEBUG_CODE(
    for (FitMicrocodeIndex = 0; FitMicrocodeIndex < MicrocodeFmpPrivate->FitMicrocodeEntryCount; FitMicrocodeIndex++) {
      FitMicrocodeInfo = &MicrocodeFmpPrivate->FitMicrocodeInfo[FitMicrocodeIndex];
      MicrocodeEntryPoint = FitMicrocodeInfo->MicrocodeEntryPoint;

      DEBUG ((
        DEBUG_ERROR,
        "InitializeFitMicrocodeInfo - FIT microcode entries [0x%x 0x%x)\n",
        MicrocodeEntryPoint,
        (UINT8 *)MicrocodeEntryPoint + FitMicrocodeInfo->TotalSize
        ));
    }
  );

  return EFI_SUCCESS;

ErrorExit:

  FreePool (MicrocodeFmpPrivate->FitMicrocodeInfo);
  MicrocodeFmpPrivate->FitMicrocodeEntryCount = 0;
  MicrocodeFmpPrivate->FitMicrocodeInfo       = NULL;

  return EFI_DEVICE_ERROR;
}

/**
  Initialize Processor Microcode Index.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.
**/
VOID
InitializedProcessorMicrocodeIndex (
  IN MICROCODE_FMP_PRIVATE_DATA *MicrocodeFmpPrivate
  )
{
  UINTN           CpuIndex;
  UINTN           MicrocodeIndex;
  UINT32          AttemptStatus;
  EFI_STATUS      Status;
  MICROCODE_INFO  *MicrocodeInfo;
  PROCESSOR_INFO  *ProcessorInfo;

  for (CpuIndex = 0; CpuIndex < MicrocodeFmpPrivate->ProcessorCount; CpuIndex++) {
    ProcessorInfo = &MicrocodeFmpPrivate->ProcessorInfo[CpuIndex];
    if (ProcessorInfo->MicrocodeIndex != (UINTN)-1) {
      continue;
    }

    for (MicrocodeIndex = 0; MicrocodeIndex < MicrocodeFmpPrivate->DescriptorCount; MicrocodeIndex++) {
      MicrocodeInfo = &MicrocodeFmpPrivate->MicrocodeInfo[MicrocodeIndex];
      if (!MicrocodeInfo->InUse) {
        continue;
      }

      Status = VerifyMicrocode(
                 MicrocodeFmpPrivate,
                 MicrocodeInfo->MicrocodeEntryPoint,
                 MicrocodeInfo->TotalSize,
                 FALSE,
                 &AttemptStatus,
                 NULL,
                 &CpuIndex
                 );
      DEBUG ((DEBUG_INFO, "InitializedProcessorMicrocodeIndex Microcode %x verify status %x, CPU Index %x\n", MicrocodeIndex, Status, CpuIndex));
      if (!EFI_ERROR(Status)) {
        ProcessorInfo->MicrocodeIndex = MicrocodeIndex;
        //
        // Update logic CPU Microcode Revision with highest uCode revision on flash. As MircocodeVersion can't be read from logic CPU defined by uCode Selection Policy PCD
        // This logic complies with Physical CPU uCode patching logic
        //
        if (!ProcessorInfo->IsRealCpu) {
          if ((ProcessorInfo->MicrocodeRevision == (UINT32) -1) ||
              (ProcessorInfo->MicrocodeRevision < MicrocodeInfo->MicrocodeEntryPoint->UpdateRevision)) {
            ProcessorInfo->MicrocodeRevision = MicrocodeInfo->MicrocodeEntryPoint->UpdateRevision;
          }
        }
      }
    }
  }
}

/**
  Initialize Microcode Descriptor.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.

  @return EFI_SUCCESS           Microcode Descriptor is initialized.
  @return EFI_OUT_OF_RESOURCES  No enough resource for the initialization.
**/
EFI_STATUS
InitializeMicrocodeDescriptor (
  IN MICROCODE_FMP_PRIVATE_DATA *MicrocodeFmpPrivate
  )
{
  EFI_STATUS Status;
  UINT8      CurrentMicrocodeCount;

  CurrentMicrocodeCount = (UINT8)GetMicrocodeInfo (MicrocodeFmpPrivate, 0, NULL, NULL);

  if (CurrentMicrocodeCount > MicrocodeFmpPrivate->DescriptorCount) {
    if (MicrocodeFmpPrivate->ImageDescriptor != NULL) {
      FreePool(MicrocodeFmpPrivate->ImageDescriptor);
      MicrocodeFmpPrivate->ImageDescriptor = NULL;
    }
    if (MicrocodeFmpPrivate->MicrocodeInfo != NULL) {
      FreePool(MicrocodeFmpPrivate->MicrocodeInfo);
      MicrocodeFmpPrivate->MicrocodeInfo = NULL;
    }
  } else {
    ZeroMem(MicrocodeFmpPrivate->ImageDescriptor, MicrocodeFmpPrivate->DescriptorCount * sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR));
    ZeroMem(MicrocodeFmpPrivate->MicrocodeInfo, MicrocodeFmpPrivate->DescriptorCount * sizeof(MICROCODE_INFO));
  }

  MicrocodeFmpPrivate->DescriptorCount = CurrentMicrocodeCount;

  if (MicrocodeFmpPrivate->ImageDescriptor == NULL) {
    MicrocodeFmpPrivate->ImageDescriptor = AllocateZeroPool(MicrocodeFmpPrivate->DescriptorCount * sizeof(EFI_FIRMWARE_IMAGE_DESCRIPTOR));
    if (MicrocodeFmpPrivate->ImageDescriptor == NULL) {
      return EFI_OUT_OF_RESOURCES;
    }
  }
  if (MicrocodeFmpPrivate->MicrocodeInfo == NULL) {
    MicrocodeFmpPrivate->MicrocodeInfo = AllocateZeroPool(MicrocodeFmpPrivate->DescriptorCount * sizeof(MICROCODE_INFO));
    if (MicrocodeFmpPrivate->MicrocodeInfo == NULL) {
      FreePool (MicrocodeFmpPrivate->ImageDescriptor);
      return EFI_OUT_OF_RESOURCES;
    }
  }

  CurrentMicrocodeCount = (UINT8)GetMicrocodeInfo (MicrocodeFmpPrivate, MicrocodeFmpPrivate->DescriptorCount, MicrocodeFmpPrivate->ImageDescriptor, MicrocodeFmpPrivate->MicrocodeInfo);
  ASSERT(CurrentMicrocodeCount == MicrocodeFmpPrivate->DescriptorCount);

  InitializedProcessorMicrocodeIndex (MicrocodeFmpPrivate);

  Status = InitializeFitMicrocodeInfo (MicrocodeFmpPrivate);
  if (EFI_ERROR(Status)) {
    //
    // Error handling Fit Microcode init failure.
    // Switch to non-fit update mode and don't return failure
    //
//    FreePool (MicrocodeFmpPrivate->ImageDescriptor);
//    FreePool (MicrocodeFmpPrivate->MicrocodeInfo);
    DEBUG ((DEBUG_ERROR, "InitializeFitMicrocodeInfo - %r\n", Status));
    DEBUG ((DEBUG_ERROR, "Force switch to Non-FIT mode\n"));

    MicrocodeFmpPrivate->FitMicrocodeEntryCount = 0;
  }

  return EFI_SUCCESS;
}

/**
  Initialize MicrocodeFmpDriver multiprocessor information.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.

  @return EFI_SUCCESS           Processor information is initialized.
  @return EFI_OUT_OF_RESOURCES  No enough resource for the initialization.
**/
EFI_STATUS
InitializeProcessorInfo (
  IN MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate
  )
{
  EFI_STATUS                           Status;
  EFI_MP_SERVICES_PROTOCOL             *MpService;
  UINTN                                NumberOfPhysicalProcessors;
  UINTN                                NumberOfEnabledProcessors;
  UINTN                                Index;
  UINTN                                CpuIndex;
  UINTN                                BspIndex;
  MICROCODE_SELECTION_POLICY           *MicrocodePolicy;
  UINTN                                PolicySize;
  UINTN                                PolicyNum;

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **)&MpService);
  ASSERT_EFI_ERROR(Status);

  MicrocodeFmpPrivate->MpService      = MpService;
  MicrocodeFmpPrivate->ProcessorCount = 0;
  MicrocodeFmpPrivate->ProcessorInfo  = NULL;

  Status = MpService->GetNumberOfProcessors (MpService, &NumberOfPhysicalProcessors, &NumberOfEnabledProcessors);
  ASSERT_EFI_ERROR(Status);
  MicrocodeFmpPrivate->ProcessorCount = NumberOfPhysicalProcessors;

  Status = MpService->WhoAmI (MpService, &BspIndex);
  ASSERT_EFI_ERROR(Status);
  MicrocodeFmpPrivate->BspIndex = BspIndex;

  //
  // Collect PolicyProcessInfo from uCode selection PolicyPcd
  //
  MicrocodePolicy = (MICROCODE_SELECTION_POLICY *) PcdGetPtr(PcduCodeSelectionPolicy);
  PolicySize      = PcdGetSize(PcduCodeSelectionPolicy);

  if ((PolicySize & 0x7) == 0) {
    PolicyNum = (PolicySize >> 3);
  } else {
    DEBUG ((DEBUG_INFO, "PcduCodeSelectionPolicy size %x is not mutiple of 8. Treat it as empty\n", PolicySize));
    PolicyNum = 0;
  }
  DEBUG ((DEBUG_INFO, "PcduCodeSelectionPolicy PolicyNum %x\n", PolicyNum));

  MicrocodeFmpPrivate->ProcessorInfo = AllocateZeroPool (sizeof(PROCESSOR_INFO) * (NumberOfPhysicalProcessors + PolicyNum));
  if (MicrocodeFmpPrivate->ProcessorInfo == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (CpuIndex = 0; CpuIndex < NumberOfPhysicalProcessors; CpuIndex++) {
    MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].CpuIndex       = CpuIndex;
    MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].MicrocodeIndex = (UINTN)-1;
    MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].IsRealCpu      = TRUE;
    if (CpuIndex == BspIndex) {
      CollectProcessorInfo (&MicrocodeFmpPrivate->ProcessorInfo[CpuIndex]);
    } else {
      Status = MpService->StartupThisAP (
                            MpService,
                            CollectProcessorInfo,
                            CpuIndex,
                            NULL,
                            0,
                            &MicrocodeFmpPrivate->ProcessorInfo[CpuIndex],
                            NULL
                            );
      ASSERT_EFI_ERROR(Status);
    }
  }

  //
  // If uCode Selection Policy is not empty.
  //    1) Validate current CPU setting with Selection Policy
  //    2) Add CPU in Policy that isn't current CPU on board as Logic CPU (IsRealCpu = FALSE)
  //
  if (PolicyNum > 0) {
    //
    // Check if there is mis-configuration of PcduCodeSelectionPolicy that doesn't include current physical CPU on the platform
    //
    DEBUG ((DEBUG_INFO, "PolicyNum = 0x%08X\n", PolicyNum));
    for (CpuIndex = 0; CpuIndex < NumberOfPhysicalProcessors; CpuIndex++) {
      for (Index = 0; Index < PolicyNum; Index++) {
        DEBUG ((DEBUG_INFO, "MicrocodeFmpPrivate->ProcessorInfo[%d].ProcessorSignature 0x%08X\n", CpuIndex, MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorSignature));
        DEBUG ((DEBUG_INFO, "MicrocodePolicy[%d].ProcessorSignature 0x%08X\n", Index, MicrocodePolicy[Index].ProcessorSignature));
        DEBUG ((DEBUG_INFO, "MicrocodeFmpPrivate->ProcessorInfo[%d].ProcessorFlags 0x%08X\n", CpuIndex, MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorFlags));
        DEBUG ((DEBUG_INFO, "MicrocodePolicy[%d].ProcessorFlags 0x%08X\n", Index, MicrocodePolicy[Index].ProcessorFlags));
        if ((MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorSignature == MicrocodePolicy[Index].ProcessorSignature) &&
            (MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorFlags == MicrocodePolicy[Index].ProcessorFlags)) {
          break;
        }
      }

      // if (Index == PolicyNum) {
      //   DEBUG ((DEBUG_ERROR, "Current CPU on Platform is not found in PcduCodeSelectionPolicy\n"));
      //   DEBUG ((DEBUG_ERROR, "ProcessorSignature %x\n", MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorSignature));
      //   DEBUG ((DEBUG_ERROR, "ProcessorFlags %x\n", MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorFlags));

      //   ASSERT(FALSE);
      //   return EFI_INVALID_PARAMETER;
      // }
    }

    //
    // Add Logic CPU that is defined in PcduCodeSelectionPolicy which is not current physical CPU on the platform
    //
    for (Index = 0; Index < PolicyNum; Index++) {
      for (CpuIndex = 0; CpuIndex < NumberOfPhysicalProcessors; CpuIndex++) {
        if ((MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorSignature == MicrocodePolicy[Index].ProcessorSignature) &&
            (MicrocodeFmpPrivate->ProcessorInfo[CpuIndex].ProcessorFlags == MicrocodePolicy[Index].ProcessorFlags)) {
          break;
        }
      }

      //
      // If CPU identified in PcduCodeSelectionPolicy is not present on this platform. Mark it to be a logic CPU
      //
      if (CpuIndex == NumberOfPhysicalProcessors) {
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].CpuIndex           = MicrocodeFmpPrivate->ProcessorCount;
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].ProcessorSignature = MicrocodePolicy[Index].ProcessorSignature;
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].ProcessorFlags     = MicrocodePolicy[Index].ProcessorFlags;
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].MicrocodeRevision  = (UINT32)-1;
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].MicrocodeIndex     = (UINTN)-1;
        MicrocodeFmpPrivate->ProcessorInfo[MicrocodeFmpPrivate->ProcessorCount].IsRealCpu          = FALSE;
        MicrocodeFmpPrivate->ProcessorCount++;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  Dump private information.

  @param[in] MicrocodeFmpPrivate private data structure.
**/
VOID
DumpPrivateInfo (
  IN MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate
  )
{
  UINTN                                Index;
  PROCESSOR_INFO                       *ProcessorInfo;
  MICROCODE_INFO                       *MicrocodeInfo;
  EFI_FIRMWARE_IMAGE_DESCRIPTOR        *ImageDescriptor;
  FIT_MICROCODE_INFO                   *FitMicrocodeInfo;

  DEBUG ((DEBUG_INFO, "ProcessorInfo:\n"));
  DEBUG ((DEBUG_INFO, "  ProcessorCount - 0x%x\n", MicrocodeFmpPrivate->ProcessorCount));
  DEBUG ((DEBUG_INFO, "  BspIndex - 0x%x\n", MicrocodeFmpPrivate->BspIndex));

  ProcessorInfo = MicrocodeFmpPrivate->ProcessorInfo;
  for (Index = 0; Index < MicrocodeFmpPrivate->ProcessorCount; Index++) {
    DEBUG ((
      DEBUG_INFO,
      " Existing ProcessorInfo[0x%x] %a - 0x%08x, 0x%02x, 0x%02x, 0x%08x, (0x%x)\n",
      ProcessorInfo[Index].CpuIndex,
      ProcessorInfo[Index].IsRealCpu ? "PhysicalCpu" : "LogicCpu",
      ProcessorInfo[Index].ProcessorSignature,
      ProcessorInfo[Index].PlatformId,
      ProcessorInfo[Index].ProcessorFlags,
      ProcessorInfo[Index].MicrocodeRevision,
      ProcessorInfo[Index].MicrocodeIndex
      ));
  }

  DEBUG ((DEBUG_INFO, "MicrocodeInfo:\n"));
  MicrocodeInfo = MicrocodeFmpPrivate->MicrocodeInfo;
  DEBUG ((DEBUG_INFO, "  MicrocodeRegion - 0x%x - 0x%x\n", MicrocodeFmpPrivate->MicrocodePatchAddress, MicrocodeFmpPrivate->MicrocodePatchRegionSize));
  DEBUG ((DEBUG_INFO, "  MicrocodeCount - 0x%x\n", MicrocodeFmpPrivate->DescriptorCount));
  for (Index = 0; Index < MicrocodeFmpPrivate->DescriptorCount; Index++) {
    DEBUG ((
      DEBUG_INFO,
      "  MicrocodeInfo[0x%x] - 0x%08x, 0x%08x, (0x%x)\n",
      Index,
      MicrocodeInfo[Index].MicrocodeEntryPoint,
      MicrocodeInfo[Index].TotalSize,
      MicrocodeInfo[Index].InUse
      ));
  }

  ImageDescriptor = MicrocodeFmpPrivate->ImageDescriptor;
  DEBUG ((DEBUG_VERBOSE, "ImageDescriptor:\n"));
  for (Index = 0; Index < MicrocodeFmpPrivate->DescriptorCount; Index++) {
    DEBUG ((DEBUG_VERBOSE, "  ImageDescriptor (%d)\n", Index));
    DEBUG ((DEBUG_VERBOSE, "    ImageIndex                  - 0x%x\n", ImageDescriptor[Index].ImageIndex));
    DEBUG ((DEBUG_VERBOSE, "    ImageTypeId                 - %g\n", &ImageDescriptor[Index].ImageTypeId));
    DEBUG ((DEBUG_VERBOSE, "    ImageId                     - 0x%lx\n", ImageDescriptor[Index].ImageId));
    DEBUG ((DEBUG_VERBOSE, "    ImageIdName                 - %s\n", ImageDescriptor[Index].ImageIdName));
    DEBUG ((DEBUG_VERBOSE, "    Version                     - 0x%x\n", ImageDescriptor[Index].Version));
    DEBUG ((DEBUG_VERBOSE, "    VersionName                 - %s\n", ImageDescriptor[Index].VersionName));
    DEBUG ((DEBUG_VERBOSE, "    Size                        - 0x%x\n", ImageDescriptor[Index].Size));
    DEBUG ((DEBUG_VERBOSE, "    AttributesSupported         - 0x%lx\n", ImageDescriptor[Index].AttributesSupported));
    DEBUG ((DEBUG_VERBOSE, "    AttributesSetting           - 0x%lx\n", ImageDescriptor[Index].AttributesSetting));
    DEBUG ((DEBUG_VERBOSE, "    Compatibilities             - 0x%lx\n", ImageDescriptor[Index].Compatibilities));
    DEBUG ((DEBUG_VERBOSE, "    LowestSupportedImageVersion - 0x%x\n", ImageDescriptor[Index].LowestSupportedImageVersion));
    DEBUG ((DEBUG_VERBOSE, "    LastAttemptVersion          - 0x%x\n", ImageDescriptor[Index].LastAttemptVersion));
    DEBUG ((DEBUG_VERBOSE, "    LastAttemptStatus           - 0x%x\n", ImageDescriptor[Index].LastAttemptStatus));
    DEBUG ((DEBUG_VERBOSE, "    HardwareInstance            - 0x%lx\n", ImageDescriptor[Index].HardwareInstance));
  }

  if (MicrocodeFmpPrivate->FitMicrocodeInfo != NULL) {
    DEBUG ((DEBUG_INFO, "FitMicrocodeInfo:\n"));
    FitMicrocodeInfo = MicrocodeFmpPrivate->FitMicrocodeInfo;
    DEBUG ((DEBUG_INFO, "  FitMicrocodeEntryCount - 0x%x\n", MicrocodeFmpPrivate->FitMicrocodeEntryCount));
    for (Index = 0; Index < MicrocodeFmpPrivate->FitMicrocodeEntryCount; Index++) {
      DEBUG ((
        DEBUG_INFO,
        "  FitMicrocodeInfo[0x%x] - 0x%08x, 0x%08x, (0x%x, 0x%x)\n",
        Index,
        FitMicrocodeInfo[Index].MicrocodeEntryPoint,
        FitMicrocodeInfo[Index].TotalSize,
        FitMicrocodeInfo[Index].InUse,
        FitMicrocodeInfo[Index].Empty
        ));
    }
  }

}

/**
  Initialize MicrocodeFmpDriver private data structure.

  @param[in] MicrocodeFmpPrivate private data structure to be initialized.

  @return EFI_SUCCESS private data is initialized.
**/
EFI_STATUS
InitializeMicrocodePrivateData (
  IN MICROCODE_FMP_PRIVATE_DATA  *MicrocodeFmpPrivate
  )
{
  EFI_STATUS                  Status;
  EFI_STATUS                  VarStatus;
  UINTN                       VarSize;
  BOOLEAN                     Result;
  EFI_BOOT_MODE               BootMode;

  MicrocodeFmpPrivate->Signature       = MICROCODE_FMP_PRIVATE_DATA_SIGNATURE;

  MicrocodeFmpPrivate->PackageVersion     = 0x1;
  MicrocodeFmpPrivate->PackageVersionName = L"Microcode";

  MicrocodeFmpPrivate->LastAttempt.LastAttemptVersion = 0x0;
  MicrocodeFmpPrivate->LastAttempt.LastAttemptStatus  = 0x0;
  VarSize = sizeof(MicrocodeFmpPrivate->LastAttempt);
  VarStatus = gRT->GetVariable(
                     MICROCODE_FMP_LAST_ATTEMPT_VARIABLE_NAME,
                     &gEfiCallerIdGuid,
                     NULL,
                     &VarSize,
                     &MicrocodeFmpPrivate->LastAttempt
                     );
  DEBUG ((DEBUG_INFO, "GetLastAttempt - %r\n", VarStatus));
  DEBUG ((DEBUG_INFO, "GetLastAttempt Version - 0x%x, State - 0x%x\n", MicrocodeFmpPrivate->LastAttempt.LastAttemptVersion, MicrocodeFmpPrivate->LastAttempt.LastAttemptStatus));

  Result = GetMicrocodeRegion(&MicrocodeFmpPrivate->MicrocodePatchAddress, &MicrocodeFmpPrivate->MicrocodePatchRegionSize);
  if (!Result) {
    DEBUG ((DEBUG_ERROR, "Fail to get Microcode Region\n"));
    return EFI_NOT_FOUND;
  }

  Status = GetMicrocodeRegionVersionInfo((UINT8 **)&MicrocodeFmpPrivate->VersionInfo, &MicrocodeFmpPrivate->VersionInfoSize);
  if (EFI_ERROR(Status) || MicrocodeFmpPrivate->VersionInfo == NULL) {
    ASSERT(MicrocodeFmpPrivate->VersionInfo != NULL);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "Current Microcode Region Version : %x\n", MicrocodeFmpPrivate->VersionInfo->Version));
  DEBUG ((DEBUG_INFO, "          Lowest Support Version : %x\n", MicrocodeFmpPrivate->VersionInfo->LowestSupportedVersion));
  DEBUG ((DEBUG_INFO, "             Version Info String : %s\n", MicrocodeFmpPrivate->VersionInfo->VersionString));

  //
  // Init Processor info. Processor Info comes from 2 sources
  //   1. uCode Selection Policy PCD
  //   2. Existing CPU onboard
  //

  BootMode = GetBootModeHob ();
  if(BootMode == BOOT_ON_FLASH_UPDATE) {
    Status = InitializeProcessorInfo (MicrocodeFmpPrivate);
    if (EFI_ERROR(Status)) {
      DEBUG ((DEBUG_ERROR, "InitializeProcessorInfo - %r\n", Status));
      return Status;
    }

    Status = InitializeMicrocodeDescriptor(MicrocodeFmpPrivate);
    if (EFI_ERROR(Status)) {
      FreePool (MicrocodeFmpPrivate->ProcessorInfo);
      DEBUG ((DEBUG_ERROR, "InitializeMicrocodeDescriptor - %r\n", Status));
      return Status;
    }
  }

  DumpPrivateInfo (MicrocodeFmpPrivate);

  return Status;
}
