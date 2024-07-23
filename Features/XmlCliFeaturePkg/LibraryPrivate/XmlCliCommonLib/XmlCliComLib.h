/** @file
  Definitions for XmlCliComLib.h

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

#ifndef XML_CLI_COM_LIB_H
#define XML_CLI_COM_LIB_H

#include <XmlCliCommon.h>

#define MAX_DUP_KNOBS_SUPPORTED     (0x200)
#define MAX_DUP_INSTANCE            (32)
#define COMMENT                     (0xC)   // Comment at the given point in XML Knobs list
#define END_OF_LIST                 (0xF)   // End of List

#define MAX_HII_PACKAGE_STRINGS     (0x4000)
#define MAX_FORMID                  (0x10400)
#define UNITS_256                   (0x100)
#define UNITS_512                   (0x200)

#define DIGEST_SIZE_256             (0x20)  // 256-bits

#define GUID_SIZE                   (0x10)  // 0x10 bytes
#define KEY_OFFSET                  (GUID_SIZE)  // 0x10
#define KEY_SIZE                    (0x20)  // 0x20 bytes random numbers to be generated
#define DIGEST_OFFSET               (GUID_SIZE + KEY_SIZE)  // 0x30
#define DIGEST_SIZE                 (DIGEST_SIZE_256)
#define ADDRESS_ALIGNMENT           (0x3F)  // In order to align XmlCli Temp buffer address to 0x40 byte
#define CLI_TEMP_BUFFER_SIZE        (DIGEST_OFFSET + DIGEST_SIZE + ADDRESS_ALIGNMENT + 1)
#define CLI_TEMP_BUFFER_RESERVE     (0x4 + DIGEST_SIZE)

#define XMLCLI_REQUEST_VALIDATED_SIG1  (0xB05ADC0D3EB0079A)
#define XMLCLI_REQUEST_VALIDATED_SIG2  (0xD095BE110E9AB1ED)

#pragma pack(push, 1)

typedef struct _KNOB_PARAM_DATA {
  KNOB_ENTRY_TBL  *KnobXmlEntry;
  UINT32          GbtXmlAddress;
  UINT32          BiosKnobsDataAddr;
  UINT32          BiosKnobsDataSize;
  UINT32          KnobValMapAddr;
  UINT32          KnobValMapCurrOff;
  UINT8           MergeDuplicateKnobs;
  UINT8           NumberOfVarstores;
  UINT8           NvroCount;
  BOOLEAN         BitWiseQuo;
  VARSTORE_TABLE  *VarstoreTable;
  UINT8           ProcessSetupPgPtr;
  CHAR8           **CurentPrompt;
  UINT16          *ParentFormId;
  UINT16          CurrentFormId;
  UINT32          CurrGetSetVarBuffer;
  VOID            *XmlCliComm;
} KNOB_PARAM_DATA;


// Global variable
#ifndef __GNUC__
BOOLEAN  InitXmlWriterFlag;
#endif


/**
  Print the given string to the XML memory.
  The XML memory pointer will also be incremented

  @param[in] StringFormat  Standard string format to print to the memory Address

  @retval  VOID
**/
VOID
EFIAPI
XmlPrintf (
  IN CHAR8  *StringFormat,
  ...
  );

/**
  Print the given string to the memory Address specified.
  The Address pointer will also be incremented

  @author ashinde (1/4/2009)

  @param[in] Address       Address to print
  @param[in] StringFormat  Standard string format to print to the memory Address

  @retval  VOID
**/
VOID
EFIAPI
MyPrintf (
  IN UINTN        Address,
  IN CONST CHAR8  *StringFormat,
  ...
  );

/**
  Replace the string in XML Memory

  @param[in] Address       Address to print
  @param[in] StringFormat  Standard string format to print to the memory Address

**/
VOID
EFIAPI
XmlReplaceString (
  IN UINT32  Address,
  IN CHAR8   *StringFormat,
  ...
  );

/**
  Print Tab in the XML file.
  if AdditionalSpace == 0 we just print the tabs according to level.
  if AdditionalSpace > 0 we add additional space to the level tabs

  @author ibar (12/25/2008)
  @author ashinde (1/4/2009)

  @param[in] AdditionalSpace  Number of spaces to be added while writing string

  @retval  VOID
**/
VOID
XmlPrintTab (
  IN UINTN  AdditionalSpace
  );

/**
  Copy given plain string to the XML memory.
  The XML memory pointer will also be incremented

  @author ashinde (1/4/2009)

  @param[in] String  string to be print in xml memory

  @retval  VOID
**/
VOID
XmlStrCpy (
  IN CHAR8  *String
  );

/**
  Copy string to of specified size to XML memory.

  @param[in] String  string to copy
  @param[in] Size    number of bytes of string to be copied

  @retval  VOID
**/
VOID
XmlStrCpySz (
  IN CHAR8  *String,
  IN UINTN  Size
  );

/**
  Print the ascii equivalent of next line `\r\n` to the XML memory
  The XML memory pointer will be adjusted accordingly
  This function is implemented specially to reduce CPU Cycles.

  @author ashinde (1/4/2009)

  @retval  VOID
**/
VOID
XmlNextLine (
  VOID
  );

/**
  Print the ascii equivalent of double quotes `"` to the XML memory.
  The XML memory pointer will be adjusted accordingly
  This function is implemented specially to reduce CPU Cycles.
  Also equivalent of XmlPrintf("\"")

  @author ashinde (1/4/2009)

  @retval  VOID
**/
VOID
XmlPrintQuotes (
  VOID
  );

/**
  Initialize XML Writer at given address

  @author ibar (12/23/2008)

  @param[in]  Address  Pointer to Address where XML Writer to be initialized

  @retval  VOID
**/
VOID
InitXmlWriter (
  IN CHAR8  *Address
  );

/**
  Add comment into XML Memory

  @author ibar (12/23/2008)
  @author ashinde (1/4/2009)

  @param[in] StringFormat  Standard string format

  @retval  VOID
**/
VOID
EFIAPI
XmlAddComment (
  IN CHAR8  *StringFormat,
  ...
  );

/**
  Print the xml attribute to the XML memory
  usage:
    use XmlAddAttributes("BIOS", "%a", XmlCliCommon->BiosVersion) to generate " BIOS="CGIBX125_23""

  @author ibar (12/24/2008)
  @author ashinde (1/4/2009)

  @param[in]  TagAttribute
  @param[in]  StringFormat     standard string format

  @retval  VOID
**/
VOID
EFIAPI
XmlAddAttributes (
  IN CHAR8  *TagAttribute,
  IN CHAR8  *StringFormat,
  ...
  );

/**
  Create an element including it's elements
  usage:
    use XmlCreateElement("XYZ", 0) to generate "<XYZ>\r\n"
    use XmlCreateElement("XYZ", 1) to generate "<XYZ" you may add attributes following this.

  @author ibar (12/24/2008)
  @author ashinde (1/4/2009)

  @param[in] XmlTag          XML Tag
  @param[in] WithAttributes  Specify whether tag will have the attribute or not
                              0: tag added as <tag>
                              1: tag added as <tag

  @retval  VOID
**/
VOID
XmlCreateElement (
  IN CHAR8    *XmlTag,
  IN BOOLEAN  WithAttributes
  );

/**
  Close an element
  usage:
    use XmlCloseElement("XYZ", 0) to generate (decrease tab level by one + "</XYZ>\r\n")
    use XmlCloseElement("XYZ", 1) to generate (">\r\n" + increase tab level by one)
    use XmlCloseElement("XYZ", 2) to generate ("</XYZ>\r\n")
    use XmlCloseElement("XYZ", 3) to generate (">\r\n" + decrease tab level by one + "</XYZ>\r\n")

  @author ashinde (1/4/2009)

  @param[in] XmlTag  XML Tag
  @param[in] Type    Type of Tag to adjust indent level
                      0: Decrease tab level by 1
                      1: Increase tab level by 1
                      2: Do not modify indent (tab level)
                      3: Add new line and decrease tab level by 1

  @retval  VOID
**/
VOID
XmlCloseElement (
  IN CHAR8  *XmlTag,
  IN UINT8  Type
  );

/**
  Create XML child with all attributes to be followed after this.
  usage:
    use XmlCreateChild("XYZ", 0) to generate "</XYZ"    Attributes will follow
    use XmlCreateChild("BIOS", "%a", XmlCliCommon->BiosVersion) to generate "<BIOS>CGIBX125_23</BIOS>"

  @author ibar (12/24/2008)
  @author ashinde (1/4/2009)

  @param[in]  XmlTag        XML Tag
  @param[in]  StringFormat  Standard string format

  @retval  VOID
**/
VOID
EFIAPI
XmlCreateChild(
  IN CHAR8 *XmlTag,
  IN CHAR8 *StringFormat,
  ...
  );

/**
  Close child with Attributes
  usage: use XmlCloseChild("XYZ") to generate "/>\r\n"

  @author ashinde (1/4/2009)

  @retval  VOID
**/
VOID
XmlCloseChild (
  VOID
  );

/**
  Get XML file size in bytes

  @author ibar (12/25/2008)

  @retval  UINT32  Size of the Xml file at Xml Writer Context
                   Address for file would be (Xml Pointer Address - Xml Base Address)
**/
UINT32
XmlGetFileSize (
  VOID
  );

/**
  Get XML file size in bytes

  @retval  UINT32  Size of XML at XmlWriterContext's Pointer address
**/
UINT32
ReturnXmlPtrAddr (
  VOID
  );

/**
  Open Xml File at given GBT XML Address

  @param[in] GbtXmlAddress  Address of GBT XML

  @retval  VOID
**/
VOID
OpenXmlFile (
  IN UINT32  GbtXmlAddress
  );

/**
  Close Xml File at given GBT XML Address and
  returns the size of the XML

  @param[in] GbtXmlAddress  Address of GBT XML

  @retval  UINT32  Xml Size
**/
UINT32
CloseXmlFile (
  IN UINT32  GbtXmlAddress
  );

/**
  Return Address of XML Writer context

  @retval  VOID  Void Pointer to Xml Writer Context
**/
VOID
*RetXmlWriterContext (
  VOID
  );

/**
  Override Xml Writer Context

  @param[in,out] NewXmlWriterContext  Pointer to Xml Writer Context
**/
VOID
OvrdXmlWriterContext (
  IN OUT VOID *NewXmlWriterContext
  );

/**
  Extract knob name from given KnobEntry pointer.
  This method is used by XmlCli Efi Application
  It is used for debugging to know what knob present at that entry

  @param[in]  KnobEntryAdd  Knob entry Address
  @param[out] KnobName      Name of the Knob

  @retval  VOID
**/
VOID
XmlFindKnobName (
  IN UINT32  KnobEntryAdd,
  OUT CHAR8  *KnobName
  );

/**
  Verify whether GBT XML is valid or not at given address

  @param[in] GbtXmlAddress  Address of Gbt XML

  @retval  TRUE   Valid GBT XML preset at given address
  @retval  FALSE  Invalid XmlSize or Invalid GBT Xml
**/
BOOLEAN
XmlValid (
  IN UINT32  GbtXmlAddress
  );

/**
  Handle Special characters in the string

  @param[in] String  Character pointer for input string

  @retval  CHAR8*  escape special characters
**/
CHAR8*
HandleSpecialXmlChars (
  IN CHAR8  *String
  );

// GBTXML_2 Function Prototypes
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
  );

/**
  Generate BIOS knobs parameter from XmlCli Common Structure

  @param[in,out] XmlCliCommon  XmlCliCommon Structure

  @retval  VOID
**/
VOID
XmlGenerateBiosKnobs (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  Save Default VarStore values to XmlCli Common Structure

  @param[in,out] XmlCliCommon  XmlCli Common Structure

  @retval  VOID
**/
VOID
XmlSaveDefVarstore (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  We have this function so that we can have following XML block generating functions
  available only in one driver and the other driver will just reuse them.
  Not required, but will save both FV_MAIN & RunTime/BootTime memory resources.

  @param[in]  XmlCliCom            XmlCli Common Structure
  @param[out] NewXmlWriterContext  Get New XmlCi Writer Context at the specified pointer

**/
UINTN
XmlGenBiosKnobsSection (
  IN VOID   *XmlCliCom,
  OUT VOID  *NewXmlWriterContext
  );

/**
  Compress the XML specified at location
  XmlPatchBaseAddr from parameter XmlCli Common structure.


  @param[in] XmlCliCom  XmlCli Common Structure
  @param[in] XmlSize    Uncompressed Xml Size

  @retval  UINT32  End Address of the Compressed XML
**/
UINT32
DxeXmlCompress (
  IN VOID    *XmlCliCom,
  IN UINT32  XmlSize
  );

/**
  Compress and save the data

  @param[in] CompStartAddr  Address at which compress data to be stored
  @param[in] DataBaseAddr   Base Address for Data to be compressed
  @param[in] DataSize       Size of the data to be compressed
  @param[in] DataType       Type of data to be compressed

  @retval  UINT32  Address where compressed data is stored
**/
UINT32
XmlCompressSave (
  IN UINT32  CompStartAddr,
  IN UINT32  DataBaseAddr,
  IN UINT32  DataSize,
  IN UINT8   DataType
  );

/**
  Helper Function to update Bios Version
  Update Bios Version, timestamp details to XmlCliCommon structure from BIOS_ID_IMAGE

  @param[in,out] XmlCliCommon  XmlCliCommon Structure

  @retval  VOID
**/
VOID UpdateBiosVersionBiosRepository (
  IN OUT XML_CLI_COMMON  *XmlCliCommon
  );

/**
  Get Knob Variable data from NVRAM

  @param[in] XmlCliCommon         XmlCli Common Structure
  @param[in] UniCodeName          Name of the variable in unicode character array
  @param[in] VarGuid              Guid of the NVRAM variable
  @param[in] SetupDataAttributes  Attribute of the NVRAM Variable
  @param[in] VarSize              Size of the NVRAM Variable
  @param[in] NvarData             NVRAM Data value (setup knob values)

  @retval  EFI_STATUS
**/
EFI_STATUS
XmlCliGetNvramData (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN CHAR16          *UniCodeName,
  IN EFI_GUID        *VarGuid,
  IN UINT32          *SetupDataAttributes,
  IN UINTN           *VarSize,
  IN VOID            *NvarData
  );

/**
  Set Knob Variable data from NVRAM

  @param[in] XmlCliCommon         XmlCli Common Structure
  @param[in] UniCodeName          Name of the variable in unicode character array
  @param[in] VarGuid              Guid of the NVRAM variable
  @param[in] SetupDataAttributes  Attribute of the NVRAM Variable
  @param[in] VarSize              Size of the NVRAM Variable
  @param[in] NvarData             NVRAM Data value (setup knob values)

  @retval  EFI_STATUS  Response Status of SetVariable/SmmSetVariable
**/
EFI_STATUS
XmlCliSetNvramData (
  IN XML_CLI_COMMON  *XmlCliCommon,
  IN CHAR16          *UniCodeName,
  IN EFI_GUID        *VarGuid,
  IN UINT32          SetupDataAttributes,
  IN UINTN           VarSize,
  IN VOID            *NvarData
  );

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
  );

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
  );

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
  );

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
  );

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
  );

/**
  Get Information if we are running in SMM or not

  @retval  TRUE    We are in SMM
  @retval  FALSE   We are not in SMM
**/
BOOLEAN
EFIAPI
XmlCliExecuteInSmm (
  VOID
  );

#pragma pack(pop)
#endif //XML_CLI_COM_LIB_H
