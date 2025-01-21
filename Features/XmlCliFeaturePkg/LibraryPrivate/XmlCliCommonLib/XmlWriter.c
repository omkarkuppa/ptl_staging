/** @file
 Implements the core XML Writer functions required for
 generating the GBT XML file in the memory. these function will
 print the tags and the required info to the XML memory, the
 memory pointers will be adjusted automatically.  we have tried
 to cover various scenarios or XML generation, if a new format of
 XML printing is required and is not met by using given
 functions, you are welcomed to add new functions according to
 your requirements.

 Example: see below the XML format and corresponding code to generate this format.

 Xml Format:
   <SYSTEM>
    <SYSTEM NAME="PASOROBLES"/>
    <BIST>0x00000000</BIST>
    <!-- ==== Knob Definition ==== -->
    <knob type="scalar" name="Signature">
    </knob>
  </SYSTEM>

 Corresponding Code:
  XmlCreateElement("SYSTEM", 0);                               //   "<SYSTEM>/r/n"
    XmlCreateChild("SYSTEM",0);                                //    "<SYSTEM"
      XmlAddAttributes("NAME","%a", GBT_PLATFORM_NAME);        //    " NAME = "PASOROBLES""
    XmlCloseChild();                                           //    "/>/r/n"
    XmlCreateChild("BIST","0x%08X", CpuDataPtr->BistResult);  //    "<BIST>0x00000000</BIST>/r/n"
    AddComent("==== Knob Definition ====");                 //    "<!-- ==== Knob Definition ==== -->/r/n"
    XmlCreateElement("knob", 1);                               //    "<knob"
      XmlAddAttributes("type", "%a", Knobs[Count].Type);       //    " type="scalar""
      XmlAddAttributes("name", "%a", Knobs[Count].KnobName);   //    " name="Signature""
    XmlCloseElement("knob", 3);                                //    ">/r/n  </knob>/r/n"
  XmlCloseElement("SYSTEM", 0);                                //   "</SYSTEM>/r/n"

 New XML generation Concept (12/25/2008)
 Initial porting by ibar (12/25/2008)
 Final porting, modification and validation (1/4/2009) contact:
 let us know your feedback on this.


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

@par Specification
**/

#include <XmlCliComLib.h>

#define TEMP_BUFFER_LENGTH         (1024*64)
#define ASCII_SPACE                (0x20)        // ` `
#define ASCII_TAB                  (0x09)        // `\t`
#define ASCII_NEW_LINE             (0x0A0D)      // `\r\n`
#define ASCII_CLOSE_TAG_BEGIN      (0x2F3C)      // `</`
#define ASCII_CLOSE_TAG_END        (0x0A0D3E)    // `>\r\n`
#define ASCII_CLOSE_TAG_CHILD_END  (0x0A0D3E2F)  // `/>\r\n`
#define ASCII_DOUBLE_QUOTE         (0x22)        // `"`
#define ASCII_EQUALS_QUOTE         (0x223D)      // `="`

#define XMLPRINTF                                                                               \
  VA_LIST  Marker;                                                                              \
  if (StringFormat != NULL) {                                                                   \
    VA_START (Marker, StringFormat);                                                            \
    AsciiVSPrint (XmlWriterContext.StringValue, TEMP_BUFFER_LENGTH, StringFormat, Marker);      \
    VA_END (Marker);                                                                            \
  } else {                                                                                      \
    *XmlWriterContext.StringValue = 0;                                                          \
  }

#pragma pack(push, 1)
typedef struct {
  CHAR8   *XmlBaseAddress;
  CHAR8   *XmlPtrAddress;
  UINT32  XmlTabLevel;
  CHAR8   *StringValue;
} XMLWriterContext_T;
#pragma pack(pop)

#ifdef __GNUC__
BOOLEAN             InitXmlWriterFlag;
#endif
XMLWriterContext_T  XmlWriterContext;


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
  )
{
  UINTN level;
  UINTN space;
  for (level = 0; level < XmlWriterContext.XmlTabLevel; level++) {
    *(UINT8*)(XmlWriterContext.XmlPtrAddress++) = (UINT8) ASCII_TAB;  // "\t"
    if (AdditionalSpace > 0) {
      for (space=0; space<=AdditionalSpace; space++) {
        *(UINT8*)(XmlWriterContext.XmlPtrAddress++) = (UINT8) ASCII_SPACE;  // " "
      }
    }
  }
}

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
  )
{
  UINTN    StrSize=0;
  VA_LIST  Marker;
  VOID     *PrintAddr=(VOID*)(UINTN)*(UINT32*)(Address);

  if (StringFormat != NULL) {
    VA_START (Marker, StringFormat);
    StrSize = AsciiVSPrint (PrintAddr, TEMP_BUFFER_LENGTH, StringFormat, Marker);
    VA_END (Marker);
  }
  *(UINT32*)(Address) = (UINT32)(*(UINT32*)(Address) + StrSize);
}

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
  )
{
  UINTN  count=0;
  while (*String) {
    count++;
    if (TEMP_BUFFER_LENGTH == count) {
      ASSERT(FALSE);
    }
    *(CHAR8 *)(XmlWriterContext.XmlPtrAddress++) = *(String++);
  }
}

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
  )
{
  UINTN  count;
  for (count=0; count<Size; count++) {
    *(CHAR8 *)(XmlWriterContext.XmlPtrAddress++) = *(String++);
  }
}

/**
  Copy and replace String at given address location

  @param[out] Address  Pointer to address which is to be replaced
  @param[in]  String   Pointer to string which is to be copied

  @retval  VOID
**/
VOID
ReplaceStrCpy (
  OUT CHAR8  *Address,
  IN CHAR8  *String
  )
{
  while (*String)
  {
    *(Address++) = *(String++);
  }
}

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
  )
{
  XMLPRINTF
  XmlStrCpy(XmlWriterContext.StringValue);
}

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
  )
{
  XMLPRINTF
  ReplaceStrCpy((CHAR8*)(UINTN)Address, XmlWriterContext.StringValue);
}

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
  )
{
  *(UINT16 *)(XmlWriterContext.XmlPtrAddress) = (UINT16) ASCII_NEW_LINE;  // "\r\n"
  XmlWriterContext.XmlPtrAddress += 2;  // adjust pointer
}

/**
  Print the ascii equivalent of close tag `</string>\r\n` to the XML memory
  The XML memory pointer will be adjusted accordingly
  This function is implemented specially to reduce CPU Cycles.

  @author ashinde (1/4/2009)

  @param[in] String  Pointer to ascii string which is
                     to be enclosed within XML close tag

  @retval  VOID
**/
VOID
XmlCloseTag (
  IN CHAR8 *String
  )
{
  *(UINT16 *)(XmlWriterContext.XmlPtrAddress) = (UINT16) ASCII_CLOSE_TAG_BEGIN;   // "</"
  XmlWriterContext.XmlPtrAddress += 2;    // adjust pointer
  while (*String) {
    *(CHAR8 *)(XmlWriterContext.XmlPtrAddress++) = *(String++);
  }
  *(UINT32 *)(XmlWriterContext.XmlPtrAddress) = (UINT32) ASCII_CLOSE_TAG_END; // ">\r\n"
  XmlWriterContext.XmlPtrAddress += 3;  // Adjust pointer
}

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
  )
{
  *(UINT8 *)(XmlWriterContext.XmlPtrAddress++) = (UINT8) ASCII_DOUBLE_QUOTE;    // print "
}

/**
  Initialize XML Writer at given address

  @author ibar (12/23/2008)

  @param[in]  Address  Pointer to Address where XML Writer to be initialized

  @retval  VOID
**/
VOID
InitXmlWriter (
  IN CHAR8  *Address
  )
{
  EFI_STATUS Status;

  ZeroMem(&XmlWriterContext, sizeof(XMLWriterContext_T));
  XmlWriterContext.XmlBaseAddress = Address;
  XmlWriterContext.XmlPtrAddress  = Address;
  XmlWriterContext.XmlTabLevel = 0;

  Status = gBS->AllocatePool (EfiRuntimeServicesData, TEMP_BUFFER_LENGTH, (VOID**)&XmlWriterContext.StringValue);
  ASSERT_EFI_ERROR (Status);

  InitXmlWriterFlag = TRUE;
}


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
  )
{
  XMLPRINTF
  XmlPrintTab(0);     //Indent properly
  // XmlPrintf("<!-- %a -->\r\n", XmlWriterContext.StringValue);
  XmlStrCpy("<!-- ");
  XmlStrCpy(XmlWriterContext.StringValue);
  XmlStrCpy(" -->\r\n");
}

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
  )
{
  XMLPRINTF

  // create the first node ' TagAttribute="sVal"'
  //  XmlPrintf(" %a=\"%a\"", TagAttribute, XmlWriterContext.StringValue);    // use \" for printing " (quotes)
  *(UINT8 *)(XmlWriterContext.XmlPtrAddress++) = ASCII_SPACE;    // print space
  XmlStrCpy(TagAttribute);
  *(UINT16 *)(XmlWriterContext.XmlPtrAddress) = ASCII_EQUALS_QUOTE;   // print ="
  XmlWriterContext.XmlPtrAddress += 2;    // adjust pointer
  XmlStrCpy(XmlWriterContext.StringValue);
  *(UINT8 *)(XmlWriterContext.XmlPtrAddress++) = ASCII_DOUBLE_QUOTE;    // print "
}

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
  )
{
  XmlPrintTab(0);  //Indent properly
  XmlStrCpy("<");
  XmlStrCpy(XmlTag);

  if (WithAttributes == 0) {
    *(UINT32 *)(XmlWriterContext.XmlPtrAddress) = (UINT32) ASCII_CLOSE_TAG_END; // ">\r\n"
    XmlWriterContext.XmlPtrAddress += 3;    // adjust pointer  }
  }

  XmlWriterContext.XmlTabLevel++;
}

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
  )
{
  switch (Type) {
  case 0:
    XmlWriterContext.XmlTabLevel--;
    XmlPrintTab(0);     //Indent properly
    XmlCloseTag(XmlTag);
    break;

  case 1:
    *(UINT32 *)(XmlWriterContext.XmlPtrAddress) = (UINT32) ASCII_CLOSE_TAG_END; // ">\r\n"
    XmlWriterContext.XmlPtrAddress += 3;    // adjust pointer  }
    break;

  case 2:
    XmlCloseTag(XmlTag);
    XmlWriterContext.XmlTabLevel--;
    break;

  case 3:
    *(UINT32 *)(XmlWriterContext.XmlPtrAddress) = (UINT32) ASCII_CLOSE_TAG_END; // ">\r\n"
    XmlWriterContext.XmlPtrAddress += 3;    // adjust pointer  }
    XmlWriterContext.XmlTabLevel--;
    XmlPrintTab(0);     //Indent properly
    XmlCloseTag(XmlTag);
    break;
  }
}

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
  )
{
  XMLPRINTF

  XmlPrintTab(0);   //Indent properly
  XmlStrCpy("<");
  XmlStrCpy(XmlTag);

  //add value and close tag
  if (StringFormat != NULL)
  { // ">sVal</XmlTag>"
    XmlStrCpy(">");
    XmlStrCpy(XmlWriterContext.StringValue);
    XmlCloseTag(XmlTag);
  }
}

/**
  Close child with Attributes
  usage: use XmlCloseChild("XYZ") to generate "/>\r\n"

  @author ashinde (1/4/2009)

  @retval  VOID
**/
VOID
XmlCloseChild (
  VOID
  )
{
  *(UINT32 *)(XmlWriterContext.XmlPtrAddress) = (UINT32) ASCII_CLOSE_TAG_CHILD_END;   // "/>\r\n"
  XmlWriterContext.XmlPtrAddress += 4;    // adjust pointer
}

/**
  Get XML file size in bytes

  @author ibar (12/25/2008)

  @retval  UINT32  Size of the Xml file at Xml Writer Context
                   Address for file would be (Xml Pointer Address - Xml Base Address)
**/
UINT32
XmlGetFileSize (
  VOID
  )
{
  return ((UINT32)(UINTN)(XmlWriterContext.XmlPtrAddress - XmlWriterContext.XmlBaseAddress));
}

/**
  Get XML file size in bytes

  @retval  UINT32  Size of XML at XmlWriterContext's Pointer address
**/
UINT32
ReturnXmlPtrAddr (
  VOID
  )
{
  return (UINT32)(UINTN)(XmlWriterContext.XmlPtrAddress);
}

/**
  Verify whether GBT XML is valid or not at given address

  @param[in] GbtXmlAddress  Address of Gbt XML

  @retval  TRUE   Valid GBT XML preset at given address
  @retval  FALSE  Invalid XmlSize or Invalid GBT Xml
**/
BOOLEAN
XmlValid (
  IN UINT32  GbtXmlAddress
  )
{
  UINT32  XmlSize;
  CopyMem((VOID*)&XmlSize, (VOID*)(UINTN)GbtXmlAddress, 4);

  if (XmlSize != 0) {
    if ((CompareMem((VOID*)(UINTN)((UINTN)GbtXmlAddress + 0x04), "<SYSTEM>", 8) == 0) &&
       (CompareMem((VOID*)(UINTN)((UINTN)GbtXmlAddress + XmlSize - 7), "</SYSTEM>", 9) == 0)) {
        return TRUE;
    }
  }
  return FALSE;
}

/**
  Extract knob name from given KnobEntry pointer.

  @param[in]  KnobEntryAdd  Knob entry Address
  @param[out] KnobName      Name of the Knob

  @retval  VOID
**/
VOID
XmlFindKnobName (
  IN UINT32  KnobEntryAdd,
  OUT CHAR8  *KnobName
  )
{
  UINT8   count;
  UINT8   Index;
  UINT64  Value;

  ZeroMem((VOID*)KnobName, 60);
  for (Index = 0; Index < 0xFF; Index++) {
    Value = ((*(UINT64*)(UINTN)((UINTN)KnobEntryAdd+Index)) & 0xFFFFFFFFFFFF);
    if (Value == 0x223D656D616E) { // compare with name="
      for (count = 0; count < 0x7F; count++) {
        if ((*(UINT8*)(UINTN)((UINTN)KnobEntryAdd+Index+6+count)) == ASCII_DOUBLE_QUOTE) {  // save till next "
          CopyMem((UINT8*)KnobName, (UINT8*)(UINTN)((UINTN)KnobEntryAdd+Index+6), count);
          return;
        }
      }
    }
  }
}

/**
  Open Xml File at given GBT XML Address

  @param[in] GbtXmlAddress  Address of GBT XML

  @retval  VOID
**/
VOID
OpenXmlFile (
  IN UINT32  GbtXmlAddress
  )
{
  InitXmlWriterFlag = FALSE;
  InitXmlWriter((CHAR8*)(UINTN)((UINTN)GbtXmlAddress + sizeof(UINT32)));  // Initialize the writer. keep sizeof(UINT32) for xml file length

  if (InitXmlWriterFlag == FALSE) {  // the XML Writer pointers are not initialized
    ASSERT(FALSE);
  }
  XmlCreateElement("SYSTEM", 0);  // start generating various sections of the GBT XML file
}

/**
  Close Xml File at given GBT XML Address and
  returns the size of the XML

  @param[in] GbtXmlAddress  Address of GBT XML

  @retval  UINT32  Xml Size
**/
UINT32
CloseXmlFile (
  IN UINT32  GbtXmlAddress
  )
{
  UINT32  XmlSize;
  XmlCloseElement("SYSTEM", 0);

  XmlSize = XmlGetFileSize();
  *((UINT32*)(UINTN)GbtXmlAddress) = XmlSize;
  return XmlSize;
}

/**
  Return Address of XML Writer context

  @retval  VOID  Void Pointer to Xml Writer Context
**/
VOID
*RetXmlWriterContext (
  VOID
  )
{
  return (VOID*)(UINTN)(&XmlWriterContext);
}

/**
  Override Xml Writer Context

  @param[in,out] NewXmlWriterContext  Pointer to Xml Writer Context
**/
VOID
OvrdXmlWriterContext (
  IN OUT VOID *NewXmlWriterContext
  )
{
  CopyMem((VOID *)(UINTN)(&XmlWriterContext), NewXmlWriterContext,sizeof(XMLWriterContext_T));
}

