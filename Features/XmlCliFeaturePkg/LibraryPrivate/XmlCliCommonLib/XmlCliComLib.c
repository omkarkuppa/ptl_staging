/** @file
  Common Lib Functions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#include <XmlCliComLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BiosIdLib.h>
#include <IndustryStandard/SmBios.h>
#include <Protocol/Smbios.h>
#include <Protocol/SmmBase2.h>


#define CMOS_DRAM_SHARED_MB_ADDR_REG       (0xF0)
#define SMBIOS_TYPE4_CPU_SOCKET_POPULATED  (BIT6)


STATIC BOOLEAN mInSmm = FALSE;


/**
  Update Front Page Strings such as FirmwareVersion, ProductName,
  CpuVersion, CpuSpeed, MemorySize to the XML.

  @retval  VOID
**/
VOID
UpdateFrontPageStrings (
  VOID
  );

/**
  Convert Unicode String to Ascii

  @param[in]  Source       Character pointer for Unicode string
  @param[out] Destination  Character pointer for ASCII string
  @param[in]  DestMax      Length of expected ASCII Output string

  @retval  VOID
**/
VOID
UnicodeToAsciiUnaligned (
  IN  CHAR16  *Source,
  OUT CHAR8   *Destination,
  IN  UINTN   DestMax
  );


/**
  XmlGenerateSystemInfo function generated the SYSTEM header information in following format
    <PLATFORM NAME="UNKNOWN" CPU="UNKNOWN" CHIPSET="UNKNOWN"/>
    <BIOS VERSION="ADLSFWI1.R00.1394.A00.2010130909" TSTAMP="10.13.2020 at 09:09 Hrs"/>
    <GBT Version="3.0302" TSTAMP="Feb 25 2015" Type="OsInternal" XmlCliVer="5.0.0" XmlCliType="Full"/>

  @param[in] XmlCliCommon  XmlCliCommon Structure

**/
VOID
XmlGenerateSystemInfo (
  IN XML_CLI_COMMON  *XmlCliCommon
  )
{
  XmlCreateChild("PLATFORM",0);
    XmlAddAttributes("NAME","%a", "UNKNOWN");
    XmlAddAttributes("CPU","%a", "UNKNOWN");
    XmlAddAttributes("CHIPSET", "%a", "UNKNOWN");
  XmlCloseChild();
  XmlCreateChild("BIOS", 0);
    XmlAddAttributes("VERSION", "%a", XmlCliCommon->BiosVersion);
    XmlAddAttributes("TSTAMP", "%a", XmlCliCommon->BuildTimeStamp);
  XmlCloseChild();

  XmlCreateChild("GBT", 0);
  // <GBT Version="3.0302" TSTAMP="Feb 25 2015" Type="OsInternal" XmlCliVer="5.0.0" XmlCliType="Full"/>
    XmlAddAttributes("Version", "%a", "3.0302");
    XmlAddAttributes("TSTAMP", "%a", "Feb 25 2015");
    if (XmlCliCommon->XmlType == XML_OS_INT) {
      XmlAddAttributes("Type", "%a", "OsInternal");
    } else if (XmlCliCommon->XmlType == XML_OS_EXT) {
      XmlAddAttributes("Type", "%a", "External");
    } else {
      XmlAddAttributes("Type", "%a", "Unknown");
    }
    XmlAddAttributes("XmlCliVer", "%a", XML_CLI_VERSION);
    if (XmlCliCommon->EnableXmlCliLite) {
      XmlAddAttributes("XmlCliType", "%a", "Lite");
    } else {
      XmlAddAttributes("XmlCliType", "%a", "Full");
    }
  XmlCloseChild();

  UpdateFrontPageStrings();
}

/**
  Helper Function to copy Bios Version

  @param[in] StrVersion  Pointer to Bios Version String
  @param[in] StrTime     Pointer to Timestamp String
  @param[in] StrUnicode  Pointer to Unicode String

  @retval  VOID
**/
VOID
CopyBiosVersion (
  IN OUT CHAR8 *StrVersion,
  IN OUT CHAR8 *StrTime,
  IN CHAR16 *StrUnicode
  )
{
  UINT8   DotCount=0;
  CHAR16  *BiosVersion=NULL;

  BiosVersion = StrUnicode;
  while (*BiosVersion != 0)
  {
    *StrVersion = (CHAR8) (*BiosVersion);
    ++StrVersion;
    ++BiosVersion;

    if (DotCount < 4)
    {
      ++StrUnicode;
    }
    if ((CHAR8) (*StrUnicode) == 0x2E)
    {
      DotCount++;
    }
  }

  ++StrUnicode;
  *(StrTime++) = (CHAR8) *(StrUnicode+2);
  *(StrTime++) = (CHAR8) *(StrUnicode+3);
  *(StrTime++) = (CHAR8)'.';    // ' '
  *(StrTime++) = (CHAR8) *(StrUnicode+4);
  *(StrTime++) = (CHAR8) *(StrUnicode+5);
  *(StrTime++) = (CHAR8)'.';    // ' '
  *(StrTime++) = (CHAR8)'2';    // ' '
  *(StrTime++) = (CHAR8)'0';    // ' '
  *(StrTime++) = (CHAR8) *(StrUnicode+0);
  *(StrTime++) = (CHAR8) *(StrUnicode+1);
  *(StrTime++) = 0x20;    // ' '
  *(StrTime++) = (CHAR8)'a';    // ' '
  *(StrTime++) = (CHAR8)'t';    // ' '
  *(StrTime++) = 0x20;    // ' '
  *(StrTime++) = (CHAR8) *(StrUnicode+6);
  *(StrTime++) = (CHAR8) *(StrUnicode+7);
  *(StrTime++) = (CHAR8)':';    // ' '
  *(StrTime++) = (CHAR8) *(StrUnicode+8);
  *(StrTime++) = (CHAR8) *(StrUnicode+9);
  *(StrTime++) = 0x20;    // ' '
  *(StrTime++) = 0x48;    // 'H'
  *(StrTime++) = 0x72;    // 'r'
  *(StrTime++) = 0x73;    // 's'
  *StrTime = 0;
}

/**
  Helper Function to update Bios Version
  Update Bios Version, timestamp details to XmlCliCommon structure from BIOS_ID_IMAGE

  @param[in,out] XmlCliCommon  XmlCliCommon Structure

  @retval  VOID
**/
VOID UpdateBiosVersionBiosRepository (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  )
{
  EFI_STATUS      Status;
  BIOS_ID_IMAGE   BiosIdImage;

  Status = GetBiosId (&BiosIdImage);
  if (EFI_ERROR (Status))
  {
    DEBUG ((DEBUG_INFO, "XML_CLI: BIOS ID not found \n"));
  } else
  {
    CopyBiosVersion(XmlCliCommon->BiosVersion, XmlCliCommon->BuildTimeStamp, (CHAR16 *)(UINTN)&(BiosIdImage.BiosIdString));
  }
  DEBUG ((DEBUG_INFO, "XML_CLI: BiosId = '%a'\n", XmlCliCommon->BiosVersion));
  DEBUG ((DEBUG_INFO, "XML_CLI: timestamp = '%a'\n", XmlCliCommon->BuildTimeStamp));
}

/**
  Convert Processor Frequency Data to a string.

  @param[in]  ProcessorFrequency  The frequency data to process
  @param[in]  Base10Exponent      The exponent based on 10
  @param[out] String              The string that is created

  @retval  VOID
**/
VOID
ConvertProcessorToString (
  IN  UINT16  ProcessorFrequency,
  IN  UINT16  Base10Exponent,
  OUT CHAR16  **String
  )
{
  UINT32  FreqMhz;
  UINTN   Index;
  CHAR16  *StringBuffer;

  if (Base10Exponent >= 6) {
    FreqMhz = ProcessorFrequency;
    for (Index = 0; Index < ((UINT32)Base10Exponent - 6); Index++) {
      FreqMhz *= 10;
    }
  } else {
    FreqMhz = 0;
  }

  StringBuffer = AllocateZeroPool (0x20);
  ASSERT (StringBuffer != NULL);
  if (StringBuffer != NULL) {
    UnicodeValueToStringS (StringBuffer, 0x20, LEFT_JUSTIFY, FreqMhz / 1000, 3);
    Index = StrnLenS (StringBuffer, 0x20 / sizeof (CHAR16));
    StrCatS (StringBuffer, 0x20 / sizeof (CHAR16), L".");
    UnicodeValueToStringS (StringBuffer + Index + 1, 0x20 - sizeof (CHAR16) * (Index + 1), PREFIX_ZERO, (FreqMhz % 1000) / 10, 2);
    StrCatS (StringBuffer, 0x20 / sizeof (CHAR16), L" GHz");
    *String = (CHAR16 *) StringBuffer;
  }
  return ;
}


/**
  Convert Memory Size to a string.

  @param[in]  MemorySize  The size of the memory to process
  @param[out] String      The string that is created

  @retval  VOID
**/
VOID
ConvertMemorySizeToString (
  IN  UINT32  MemorySize,
  OUT CHAR16  **String
  )
{
  CHAR16  *StringBuffer;

  StringBuffer = AllocateZeroPool (0x20);
  ASSERT (StringBuffer != NULL);
  if (StringBuffer != NULL) {
    UnicodeValueToStringS (StringBuffer, 0x20, LEFT_JUSTIFY, MemorySize, 6);
    StrCatS (StringBuffer, 0x20 / sizeof (CHAR16), L" MB RAM");

    *String = (CHAR16 *) StringBuffer;
  }
  return ;
}

/**

  Acquire the string associated with the Index from smbios structure and return it.
  The caller is responsible for free the string buffer.

  @param[in]  OptionalStrStart  The start position to search the string
  @param[in]  Index             The index of the string to extract
  @param[out] String            The string that is extracted

  @retval  EFI_SUCCESS   String Found and copied successfully
  @retval  !EFI_SUCCESS  Failure
**/
EFI_STATUS
GetOptionalStringByIndex (
  IN  CHAR8   *OptionalStrStart,
  IN  UINT8   Index,
  OUT CHAR16  **String
  )
{
  UINTN  StrSize;

  if (Index == 0) {
    *String = AllocateZeroPool (sizeof (CHAR16));
    return EFI_SUCCESS;
  }

  StrSize = 0;
  do {
    Index--;
    OptionalStrStart += StrSize;
    StrSize           = AsciiStrSize (OptionalStrStart);
  } while (OptionalStrStart[StrSize] != 0 && Index != 0);

  if ((Index != 0) || (StrSize == 1)) {
    //
    // Meet the end of strings set but Index is non-zero, or
    // Find an empty string
    //
    *String = L"Missing String";
  } else {
    *String = AllocatePool (StrSize * sizeof (CHAR16));
    AsciiStrToUnicodeStrS (OptionalStrStart, *String, StrSize);
  }

  return EFI_SUCCESS;
}

/**
  Update Front Page Strings such as FirmwareVersion, ProductName,
  CpuVersion, CpuSpeed, MemorySize to the XML.

  @retval  VOID
**/
VOID
UpdateFrontPageStrings (
  VOID
  )
{
  BOOLEAN                  Find[5];
  CHAR8                    AsciiStrBlock[0x100];
  UINT8                    StrIndex;
  CHAR16                   *NewString;
  UINT64                   InstalledMemory;
  EFI_STATUS               Status;
  EFI_SMBIOS_HANDLE        SmbiosHandle;
  EFI_SMBIOS_PROTOCOL      *Smbios;
  EFI_SMBIOS_TABLE_HEADER  *Record;
  SMBIOS_TABLE_TYPE1       *Type1Record;
  SMBIOS_TABLE_TYPE4       *Type4Record;
  SMBIOS_TABLE_TYPE19      *Type19Record;

  InstalledMemory = 0;
  ZeroMem (Find, sizeof (Find));

  //
  // Update Front Page strings
  //
  Status = gBS->LocateProtocol (&gEfiSmbiosProtocolGuid, NULL, (VOID **) &Smbios);
  if (EFI_ERROR (Status)) {
    return ;
  }

  SmbiosHandle = SMBIOS_HANDLE_PI_RESERVED;
  XmlCreateChild("FrontPage", 0);  // <PCH-reference Version="0.7.0-00"/>
  do {
    Status = Smbios->GetNext (Smbios, &SmbiosHandle, NULL, &Record, NULL);
    if (EFI_ERROR(Status)) {
      break;
    }

    if (Record->Type == EFI_SMBIOS_TYPE_BIOS_INFORMATION) {
      BIOS_ID_IMAGE   BiosIdImage;
      Status = GetBiosId (&BiosIdImage);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO, "XML_CLI: BIOS ID not found \n"));
        XmlAddAttributes("FirmwareVersion", "??");
      } else {
        UnicodeToAsciiUnaligned((CHAR16*)((UINTN)&BiosIdImage.BiosIdString), AsciiStrBlock, sizeof(AsciiStrBlock));
        XmlAddAttributes("FirmwareVersion", "%a", HandleSpecialXmlChars(AsciiStrBlock));
      }
      Find[0] = TRUE;
    }

    if (Record->Type == EFI_SMBIOS_TYPE_SYSTEM_INFORMATION) {
      Type1Record = (SMBIOS_TABLE_TYPE1 *) Record;
      StrIndex = Type1Record->ProductName;
      GetOptionalStringByIndex ((CHAR8*)((UINT8*)Type1Record + Type1Record->Hdr.Length), StrIndex, &NewString);
      if (NewString != NULL) {
        UnicodeToAsciiUnaligned(NewString, AsciiStrBlock, sizeof(AsciiStrBlock));
        XmlAddAttributes("ProductName", "%a", HandleSpecialXmlChars(AsciiStrBlock));
      } else {
        DEBUG ((DEBUG_INFO, "XML_CLI: ProductName not found \n"));
        XmlAddAttributes("ProductName", "??");
      }
      FreePool (NewString);
      Find[1] = TRUE;
    }

    if ((Record->Type == EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION) && !Find[2]) {
      Type4Record = (SMBIOS_TABLE_TYPE4 *) Record;
      //
      // The information in the record should be only valid when the CPU Socket is populated.
      //
      if ((Type4Record->Status & SMBIOS_TYPE4_CPU_SOCKET_POPULATED) == SMBIOS_TYPE4_CPU_SOCKET_POPULATED) {
        StrIndex = Type4Record->ProcessorVersion;
        GetOptionalStringByIndex ((CHAR8*)((UINT8*)Type4Record + Type4Record->Hdr.Length), StrIndex, &NewString);
        if (NewString != NULL){
          UnicodeToAsciiUnaligned(NewString, AsciiStrBlock, sizeof(AsciiStrBlock));
          XmlAddAttributes("CpuVersion", "%a", HandleSpecialXmlChars(AsciiStrBlock));
        } else {
          DEBUG ((DEBUG_INFO, "XML_CLI: CpuVersion not found \n"));
          XmlAddAttributes("CpuVersion", "??");
        }
        FreePool (NewString);
        Find[2] = TRUE;
      }
    }

    if ((Record->Type == EFI_SMBIOS_TYPE_PROCESSOR_INFORMATION) && !Find[3]) {
      Type4Record = (SMBIOS_TABLE_TYPE4 *) Record;
      //
      // The information in the record should be only valid when the CPU Socket is populated.
      //
      if ((Type4Record->Status & SMBIOS_TYPE4_CPU_SOCKET_POPULATED) == SMBIOS_TYPE4_CPU_SOCKET_POPULATED) {
        ConvertProcessorToString(Type4Record->CurrentSpeed, 6, &NewString);
        if (NewString != NULL){
          UnicodeToAsciiUnaligned(NewString, AsciiStrBlock, sizeof(AsciiStrBlock));
          XmlAddAttributes("CpuSpeed", "%a", HandleSpecialXmlChars(AsciiStrBlock));
        } else {
          DEBUG ((DEBUG_INFO, "XML_CLI: CpuSpeed not found \n"));
          XmlAddAttributes("CpuSpeed", "??");
        }
        FreePool (NewString);
        Find[3] = TRUE;
      }
    }
   if (Record->Type == EFI_SMBIOS_TYPE_MEMORY_ARRAY_MAPPED_ADDRESS) {
      Type19Record = (SMBIOS_TABLE_TYPE19 *) Record;
      if (Type19Record->StartingAddress != 0xFFFFFFFF) {
        InstalledMemory += RShiftU64(Type19Record->EndingAddress - Type19Record->StartingAddress + 1, 10);
      } else {
        InstalledMemory += RShiftU64(Type19Record->ExtendedEndingAddress - Type19Record->ExtendedStartingAddress + 1, 20);
      }
    }
  } while (!(Find[0] && Find[1] && Find[2] && Find[3] && Find[4]));

  ConvertMemorySizeToString ((UINT32)InstalledMemory, &NewString);
  if (NewString != NULL) {
    UnicodeToAsciiUnaligned(NewString, AsciiStrBlock, sizeof(AsciiStrBlock));
    XmlAddAttributes("MemorySize", "%a", HandleSpecialXmlChars(AsciiStrBlock));
  } else {
    DEBUG ((DEBUG_INFO, "XML_CLI: MemorySize not found \n"));
    XmlAddAttributes("MemorySize", "??");
  }
  FreePool (NewString);
  Find[4] = TRUE;

  XmlCloseChild();
}

/**
  Helper function to Convert Hex array to ascii values.
  Takes input as pointer hex values and start offset to
  convert to ascii string

  @param[in,out] AsciiStr   Pointer to Ascii String to be stored
  @param[in]     InHexVal   Pointer to the hex values
  @param[in]     VarLength  Number of hex values to be converted to ascii string

  @retval  VOID
**/
VOID
ConvHexArray2asciiVal (
  IN OUT CHAR8  *AsciiStr,
  IN UINT8      *InHexVal,
  IN UINTN      VarLength
  )
{
  UINTN Index=0;
  CHAR8  HexStr[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

  for (Index = 0; Index < VarLength; Index++) {
    AsciiStr[(Index*2)] = HexStr[((InHexVal[(VarLength-1-Index)] >> 4) & 0xF)];
    AsciiStr[(Index*2)+1] = HexStr[(InHexVal[(VarLength-1-Index)] & 0xF)];
  }
}

/**
  Helper function to write Dram Mailbox Address to CMOS
  As the mailbox address is reserved area and may change,
  hence saving in CMOS for the tool to read

  @param[in] DramMbAddress  Address of Dram Mailbox

  @retval  VOID
**/
VOID
WriteDramMbAddr2Cmos (
  IN UINT32  DramMbAddress
  )
{
  UINT8 IoPort;

  IoWrite8(0x72, (UINT8)CMOS_DRAM_SHARED_MB_ADDR_REG);    // Save the Mailbox address in CMOS so that the tool can read it
  IoWrite8(0x73, (UINT8)(DramMbAddress >> 16));          // save lower byte of higher UINT16
  IoWrite8(0x72, (UINT8)CMOS_DRAM_SHARED_MB_ADDR_REG+1);
  IoWrite8(0x73, (UINT8)(DramMbAddress >> 24));          // save upper byte of higher UINT16

  if ((0 != FixedPcdGet8(PcdDramPublishToCmos)) && (((UINT8)CMOS_DRAM_SHARED_MB_ADDR_REG) != FixedPcdGet8(PcdDramPublishToCmos))) {
    if (FixedPcdGet8(PcdDramPublishToCmos) < 0x80) {
      IoPort = 0x70;
    } else {
      IoPort = 0x72;
    }
    IoWrite8(IoPort, (UINT8)FixedPcdGet8(PcdDramPublishToCmos));    // Save the Mailbox address in CMOS so that the tool can read it
    IoWrite8(IoPort + 1, (UINT8)(DramMbAddress >> 16));          // save lower byte of higher UINT16
    IoWrite8(IoPort, (UINT8)FixedPcdGet8(PcdDramPublishToCmos)+1);
    IoWrite8(IoPort + 1, (UINT8)(DramMbAddress >> 24));          // save upper byte of higher UINT16
  }
}

UINT64 AndMask[] = {0, 0xFF, 0xFFFF, 0xFFFFFF, 0xFFFFFFFF, 0xFFFFFFFFFF, 0xFFFFFFFFFFFF, 0xFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF};

/**
  Extracts Byte Size from given BIT size

  @param[in] UINT8  Bit Offset within the given Byte (max Value of 7)
  @param[in] UINT8  Bit Size (max value of 63)

  @retval  UINT8  Extracted byte Size
**/
UINT8
BitGetByteSize (
  IN UINT8  BitOffset,
  IN UINT8  BitSize
  )
{
  UINT8 ByteSize;
  ByteSize = BitOffset + BitSize;
  if (ByteSize % 8) {
    ByteSize = (ByteSize/8) + 1;
  } else {
    ByteSize = (ByteSize/8);
  }
  return ByteSize;
}

/**
  Extracts BIT Value from given Bit Offset for given Bit Size

  @param[in] UINT64  Input Value
  @param[in] UINT8   Offset within the given Byte (max Value of 7)
  @param[in] UINT8   Size (max value of 63)

  @retval  UINT64  Extracted Value
**/
UINT64
BitExtractValue64 (
  IN UINT64  InputVal,
  IN UINT8   BitOffset,
  IN UINT8   BitSize
  )
{
  UINT8  ByteSize;
  ByteSize = BitGetByteSize(BitOffset, BitSize);
  return RShiftU64 (InputVal, BitOffset) & RShiftU64 (AndMask[ByteSize], ((ByteSize*8) - BitSize));
}

/**
  BIT wise Compare function

  @param[in] VOID   Destination Buffer Pointer
  @param[in] VOID   Source Buffer Pointer
  @param[in] UINT8  Bit Offset within the given Byte (max Value of 7)
  @param[in] UINT8  Bit Size (max value of 63)
  @param[in] UINT8  CompareType (will be 0 if Source is Absolute Value that start at BIT00, else 1)

  @retval  BOOLEAN  (TRUE if Value Matches)
**/
BOOLEAN
BitDataMatch64 (
  IN VOID*  Destination,
  IN VOID*  Source,
  IN UINT8  BitOffset,
  IN UINT8  BitSize,
  IN UINT8  CompareType
  )
{
  UINT8   ByteSize;
  UINT64  BitMask;
  UINT64  DstVal=0;
  UINT64  SrcVal=0;
  ByteSize = BitGetByteSize(BitOffset, BitSize);
  BitMask = RShiftU64 (AndMask[ByteSize], ((ByteSize*8) - BitSize));
  CopyMem(&DstVal,Destination, ByteSize);
  CopyMem(&SrcVal,Source, ByteSize);
  if (0 == CompareType) {
    SrcVal = (UINT64)(SrcVal & BitMask);
  } else {
    SrcVal = (UINT64)(RShiftU64 (SrcVal, BitOffset) & BitMask);
  }
  DstVal = (UINT64)(RShiftU64 (DstVal, BitOffset) & BitMask);
  if (SrcVal == DstVal) {
    return TRUE;
  } else {
    return FALSE;
  }
}

/**
  BIT wise Copy function

  @param[in] VOID   Destination Buffer Pointer
  @param[in] VOID   Source Buffer Pointer
  @param[in] UINT8  Bit Offset within the given Byte (max Value of 7)
  @param[in] UINT8  Bit Size (max value of 63)

  @retval  VOID
**/
VOID
BitDataCopy64 (
  IN VOID*  Destination,
  IN VOID*  Source,
  IN UINT8  BitOffset,
  IN UINT8  BitSize
  )
{
  UINT8   ByteSize;
  UINT64  BitMask;
  UINT64  DstVal=0;
  UINT64  SrcVal=0;
  ByteSize = BitGetByteSize(BitOffset, BitSize);
  BitMask = LShiftU64 (RShiftU64 (AndMask[ByteSize], ((ByteSize*8) - BitSize)), BitOffset);
  CopyMem(&DstVal,Destination, ByteSize);
  CopyMem(&SrcVal,Source, ByteSize);
  SrcVal = (UINT64)(LShiftU64 (SrcVal, BitOffset) & BitMask);
  DstVal = (UINT64)((DstVal & (~BitMask)) | SrcVal);
  CopyMem(Destination,(VOID*)&DstVal, ByteSize);
}

/**
  Fetch the correct index for the given Bitwise knob
  If Index not found then 0xFFFF will be returned

  @param[in] UINT32  KnobEntryAddress
  @param[in] UINT32  KnobValMapAddr
  @param[in] UINT16  ParentIndex
  @param[in] UINT8   BitField

  @retval  UINT16  Index Value
**/
UINT16
BitFetchIndex (
  UINT32  KnobEntryAddress,
  UINT32  KnobValMapAddr,
  UINT16  ParentIndex,
  UINT8   BitField
  )
{
  UINT16  Index;
  KNOB_ENTRY_TBL_BITWISE *KnobXmlEntryPtr = (KNOB_ENTRY_TBL_BITWISE*)(UINTN)KnobEntryAddress;
  BIT_KNOB_INDEX_LOOKUP  *BitKnobLookup;

  if(KnobXmlEntryPtr[ParentIndex].IsBitWise) {
    if(KnobXmlEntryPtr[ParentIndex].StartIndex == KnobXmlEntryPtr[ParentIndex].EndIndex) {
      return (UINT16)KnobXmlEntryPtr[ParentIndex].StartIndex;
    } else {
      for (Index = (UINT16)KnobXmlEntryPtr[ParentIndex].StartIndex; Index <= (UINT16)KnobXmlEntryPtr[ParentIndex].EndIndex; Index++) {
        if (KnobXmlEntryPtr[Index].IsBitWise == 0) {
          continue;
        }
        BitKnobLookup = (BIT_KNOB_INDEX_LOOKUP*)(UINTN)((UINTN)KnobValMapAddr + ((KNOB_ENTRY_TBL*)KnobXmlEntryPtr)[Index].KnobValMapOffset - sizeof(BIT_KNOB_INDEX_LOOKUP));
        if ((BitKnobLookup->ByteOffset == ParentIndex) && (BitKnobLookup->BitField == BitField)) {
          return Index;
          break;
        }
      }
    }
  }
  return 0xFFFF;  // to indicate Index not found
}

/**
  Get Information if we are running in SMM or not

  @retval  TRUE    We are in SMM
  @retval  FALSE   We are not in SMM
**/
BOOLEAN
EFIAPI
XmlCliExecuteInSmm (
  VOID
  )
{
  return mInSmm;
}

/**
  The constructor function for XmlCliCommon Library.
  To get Information if we are running in SMM or not

  The functions saves information about if the UEFI FW is running in SMM

  @retval  TRUE    We are in SMM
  @retval  FALSE   We are not in SMM
**/
EFI_STATUS
EFIAPI
XmlCliExecuteInSmmConstructor (
  VOID
  )
{
  EFI_STATUS              Status;
  EFI_SMM_BASE2_PROTOCOL  *SmmBase2;

  mInSmm = FALSE;
  Status = gBS->LocateProtocol (
                  &gEfiSmmBase2ProtocolGuid,
                  NULL,
                  (VOID **) &SmmBase2
                  );
  if (!EFI_ERROR (Status)) {
    SmmBase2->InSmm(SmmBase2, &mInSmm);
  }

  return mInSmm;
}  // XmlCliExecuteInSmmConstructor
