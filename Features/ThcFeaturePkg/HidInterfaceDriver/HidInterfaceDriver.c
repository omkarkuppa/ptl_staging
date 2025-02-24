/** @file
  HID Interface Driver

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

@par Specification Reference:
**/

#include <Include/HidInterfaceParser.h>

#define DEBUG_HID_PARSE_INFO DEBUG_VERBOSE

/*
  HID's InputReportDescriptor consists of variable length tokens.
  On entry, Descriptor points to InputReportDescriptor's start or a boundary
    between two tokens somewhere inside descriptor. Size means number of bytes
    before end of Descriptor.
  Function returns a single Token.
  On exit, Descriptor points next boundary between tokens, just after the token
    that was returned. Size is decreased by as many bytes as Descriptor pointer
    was moved forward.
  Tokens in Long Item format are only partially supported; they will return
    invalid value but Descriptor pointer and Size will be updated correctly to
    allow further parsing
*/
HID_TOKEN
STATIC
HidGetToken (
  IN OUT UINT8  **Descriptor,
  IN OUT UINT32 *Size
  )
{
  HID_TOKEN  Token;
  UINT32     Length;
  UINT8      DataSize;

  Token.ID = ((**Descriptor)>>2);
  Token.Value = 0;
  DataSize = (**Descriptor)&0x3;
  DEBUG ((DEBUG_HID_PARSE_INFO, "\n (0x%x, ", (**Descriptor)));

  if (**Descriptor==0xFE) {
    // Long Item format
    Token.ID = *(*Descriptor+2);
    //don't care about value - Long Items are not supported
    Length = *(*Descriptor + 1);
    (*Descriptor) += Length;
    if ((*Size) >= Length) {
      (*Size) -= Length;
    } else {
      (*Size) = 0;
    }
    return Token;
  }
  switch (DataSize) {
    case 0:
      Token.Value = 0;
      DEBUG ((DEBUG_HID_PARSE_INFO, " 0) "));
      break;
    case 1:
      Token.Value = *(*Descriptor + 1);
      DEBUG ((DEBUG_HID_PARSE_INFO, "0x%x) ", Token.Value));
      break;
    case 2:
      Token.Value = *(*Descriptor + 1) + (*(*Descriptor + 2) << 8);
      DEBUG ((DEBUG_HID_PARSE_INFO, "0x%x) ", Token.Value));
      break;
    case 3:
      Token.Value = *(*Descriptor + 1) + ((*(*Descriptor + 2)) << 8) + ((*(*Descriptor + 3)) << 16) + ((*(*Descriptor + 4)) << 24);
      DataSize = 4;
      DEBUG ((DEBUG_HID_PARSE_INFO, "0x%x) ", Token.Value));
      break;
    default: ;
  }
  // move descriptor pointer and decrease size by datasize + 1 byte for ID
  (*Descriptor) += (1 + DataSize);
  if ((*Size) >= (UINT32) (1 + DataSize)) {
    (*Size) -= (1 + DataSize);
  } else {
    (*Size) = 0;
  }

  return Token;
}

/*
  Stack contains a set of data retrieved from parsing InputReportDescriptor.
  This function checks if that set of data constitutes a valid InputData
  dictionary, and if so then puts it into ReportTable, table of dictionaries.
  Unless this is the 1st dictionary for a particular device, this means
  allocating new bigger table and deallocating old table

  @param[in]     Stack             A pointer to the PARSER_STACK
  @param[in/out] ReportTable       Pointer to the final report table that contains all the Reports

*/
VOID
STATIC
HidExportReport (
  IN     HID_PARSER_STACK       *Stack,
  IN OUT HID_INPUT_REPORT_TABLE *ReportTable
  )
{
  HID_INPUT_REPORT_FORMAT *ArrayOfReports;

  if (Stack->UsageTouchPanel) {
    ReportTable->TouchPanel = 1;
  }

  if (Stack->UsageTouchPad) {
    ReportTable->TouchPad = 1;
  }

  // Mouse case will be set only if supports Input descriptor as Rel otherwise it will be set as 0
  if (Stack->UsageMouse) {
    ReportTable->Mouse = 1;
  }

  if ((Stack->UsageX != 0) && (Stack->UsageY != 0)) {
    ArrayOfReports = (HID_INPUT_REPORT_FORMAT*) AllocatePool (sizeof(HID_INPUT_REPORT_FORMAT) * (ReportTable->Quantity + 1));
    ASSERT (ArrayOfReports != NULL);
    if (ArrayOfReports != NULL) {
      if (ReportTable->Quantity != 0) {
        CopyMem (ArrayOfReports, ReportTable->Report, sizeof(HID_INPUT_REPORT_FORMAT) * ReportTable->Quantity);
        FreePool (ReportTable->Report);
      }
      ReportTable->Report = ArrayOfReports;
      CopyMem (&(ReportTable->Report[ReportTable->Quantity]), &(Stack->TempReport), sizeof(HID_INPUT_REPORT_FORMAT));
      ReportTable->Quantity++;
    }
  }
}

/*
  Stack is pointer to a not-yet-complete InputReport dictionary.
  This function consumes Token retrieved from InputReportDescriptor stream
  and uses it to update the dictionary.
  Once a dictionary is completed, it calls ExportReport() which puts the
  dictionary in a table of dictionaries. Then it clears internal data
  and prepares to build a new one.
  This implementation of Descriptor parser ignores all types of data except for
  information on how to decode Button presses and X/Y touch coordinates, as this
  is the only info relevant for touchpanels.

  @param[in/out] Stack                 A pointer to the PARSER_STACK
  @param[in]     Token                 Current Token
  @param[in/out] ReportTable           Pointer to the final report table that contains all the Reports
*/
VOID
STATIC
HidUpdateStack (
  IN OUT HID_PARSER_STACK       *Stack,
  IN     HID_TOKEN              Token,
  IN OUT HID_INPUT_REPORT_TABLE *ReportTable,
  IN OUT HID_GET_REPORT_FORMAT  *GetReportPacket
  )
{

  switch (Token.ID) {
    case USE_PAGE:
      Stack->GlobalUsage = Token.Value;
      if (Stack->GlobalUsage == BUTTON) {
        Stack->Usages++;
        Stack->UsageB = Stack->Usages;
      }
      break;
    case USAGE:
      if (Token.Value > 0xFFFF) { //special case for 32 bit usages - they indicate Usage + Use Page together
        Stack->GlobalUsage = Token.Value >> 16;
        Token.Value = Token.Value & 0xFFFF;
      }
      Stack->Usages++;
      if (Stack->GlobalUsage == DIGITIZER && Token.Value == TOUCHPANEL) { Stack->UsageTouchPanel = 1; Stack->TempReport.UsageTouchPanel= 1;}
      if (Stack->GlobalUsage == DIGITIZER && Token.Value == TOUCHPPAD) { Stack->UsageTouchPad = 1; Stack->TempReport.UsageTouchPad = 1;}
      if (Stack->GlobalUsage == DIGITIZER && Token.Value == MOUSE) { Stack->UsageMouse = 1; Stack->TempReport.UsageMouse = 1; }
      if (Stack->GlobalUsage == DIGITIZER && Token.Value == TIP_SWITCH) { Stack->UsageB = Stack->Usages;}
      if (Stack->GlobalUsage == DIGITIZER && Token.Value == TOUCH_VALID) { Stack->UsageB = Stack->Usages;}
      if (Stack->GlobalUsage == DESKTOP && Token.Value == MOUSE) { Stack->UsageMouse = 1; Stack->TempReport.UsageMouse = 1; }
      if (Stack->GlobalUsage == DESKTOP && Token.Value == X_AXIS) { Stack->UsageX = Stack->Usages; }
      if (Stack->GlobalUsage == DESKTOP && Token.Value == Y_AXIS) { Stack->UsageY = Stack->Usages; }
      if (Stack->GlobalUsage == CUSTOM_REPORT_ID && Token.Value == THQA_FEATURE_REPORT) {
        GetReportPacket->FeatureReportAvailable = TRUE;
        GetReportPacket->ReportId = (UINT8) Stack->TempReport.Id;
      }
      break;
    case LOG_MIN:  Stack->LogMin = Token.Value; break;
    case LOG_MAX:  Stack->LogMax = Token.Value; break;
    case REP_SIZE:  Stack->ReportSize = Token.Value; break;
    case REP_COUNT:  Stack->ReportCount = Token.Value; break;
    case REPORT_ID:
      //
      // Is new report?
      //
      if (Stack->TempReport.Id != 0xFFFF) {
        HidExportReport (Stack, ReportTable);
      }
      Stack->Usages = 0;
      Stack->UsageX = 0;
      Stack->UsageY = 0;
      Stack->UsageB = 0;
      Stack->LogMin = 0;
      Stack->LogMax = 0;
      // TouchPanel/Pad/Mouse Usage is not cleared as it will indicate main usage of the device
      // it is required for AbsPointer Protocol installation with proper GUID
      Stack->ReportSize = 0;
      Stack->ReportCount = 0;
      Stack->TempReport.Id = Token.Value;

      //
      // New Report does not indicate new collection set!
      //
      ZeroMem (Stack->TempReport.Collection, sizeof(HID_INPUT_REPORT_COLLECTION) * MAX_HID_COLLECTION);
      Stack->TempReport.CollectionCount = 1;
      Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].ValidCollection = FALSE;

      break;
    case INPUT:
      if (Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopB == 0 && Stack->UsageB != 0 && Stack->Usages == Stack->UsageB) {
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStartB = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopB = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal + (Stack->ReportSize * Stack->ReportCount);
        Stack->LogMax = 0;
        Stack->LogMin = 0;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += (Stack->ReportSize * Stack->ReportCount);
        break;
      }

      // Combined Input descriptor for UsageX and UsageY
      if ((Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopX == 0)
          && (Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopY == 0)
          && (Stack->UsageX != 0)
          && (Stack->UsageY != 0)) {
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStartX = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopX = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal + (Stack->ReportSize);
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMaxX = Stack->LogMax;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMinX = Stack->LogMin;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += Stack->ReportSize;

        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStartY = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopY = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal + (Stack->ReportSize);
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMaxY = Stack->LogMax;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMinY = Stack->LogMin;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsInputX = Token.Value;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsInputY = Token.Value;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += Stack->ReportSize;
        break;
      }

      // Independent Input descriptor for UsageX
      if (Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopX == 0 && Stack->UsageX != 0 && Stack->Usages == Stack->UsageX) {
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStartX = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopX = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal + (Stack->ReportSize * Stack->ReportCount);
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMaxX = Stack->LogMax;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMinX = Stack->LogMin;
        Stack->LogMax = 0;
        Stack->LogMin = 0;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsInputX = Token.Value;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += (Stack->ReportSize * Stack->ReportCount);
        break;
      }
      // Independent Input descriptor for UsageY
      if (Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopY == 0 && Stack->UsageY != 0 && Stack->Usages == Stack->UsageY) {
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStartY = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitStopY = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal + (Stack->ReportSize * Stack->ReportCount);
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMaxY = Stack->LogMax;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].LogMinY = Stack->LogMin;
        Stack->LogMax = 0;
        Stack->LogMin = 0;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsInputY = Token.Value;
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += (Stack->ReportSize * Stack->ReportCount);
        break;
      }

      // Other cases
      Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal += (Stack->ReportSize * Stack->ReportCount);
      break;
    case COLLECTION:
      if (Token.Value == APPLICATION) { }
      else if (Token.Value == LOGICAL) {
        if ((Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].ValidCollection == TRUE) && (Stack->TempReport.CollectionCount < MAX_HID_COLLECTION)) {
              Stack->TempReport.CollectionCount += 1;
          Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].BitsTotal = Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 2].BitsTotal;
        }
        Stack->TempReport.Collection[Stack->TempReport.CollectionCount - 1].ValidCollection = TRUE;
      }
      break;
    case END_COLLECTION:
      break;
    default:;
  }
}

/*
  Retrieves single bit from multi-byte stream

  @param[in]  InputStream   Data
  @param[in]  Bit           Bit offset
*/
UINT32
STATIC
HidAccessBit (
  IN UINT8  *InputStream,
  IN UINT32 Bit
  )
{
  return ((InputStream[Bit/8] & (1 << (Bit % 8)) ) ? 1:0 );
}

/**
  Dump report table
  - shows the amount of parsed Reports along with their collections

  @param[in] ReportTable           Pointer to the final report table that contains all the Reports
**/
VOID
STATIC
HidShowReportTable (
  IN HID_INPUT_REPORT_TABLE *ReportTable
  )
{
  UINT32 Index;
  UINT32 Collections;

  for (Index = 0; Index < ReportTable->Quantity; Index++) {
    DEBUG ((DEBUG_INFO, "Report Id_%x \n", ReportTable->Report[Index].Id));
    DEBUG ((DEBUG_INFO, " %d_TouchPanel_%d_Touchpad_%d_Mouse \n", ReportTable->Report[Index].UsageTouchPanel, ReportTable->Report[Index].UsageTouchPad, ReportTable->Report[Index].UsageMouse));
    for (Collections = 0; Collections < ReportTable->Report[Index].CollectionCount; Collections++) {
      DEBUG ((DEBUG_INFO, " Collection Id_%x ", Collections));
      DEBUG ((DEBUG_INFO, "MaxX_%x ", ReportTable->Report[Index].Collection[Collections].LogMaxX));
      DEBUG ((DEBUG_INFO, "MinX_%x ", ReportTable->Report[Index].Collection[Collections].LogMinX));
      DEBUG ((DEBUG_INFO, "MaxY_%x ", ReportTable->Report[Index].Collection[Collections].LogMaxY));
      DEBUG ((DEBUG_INFO, "MinY_%x ", ReportTable->Report[Index].Collection[Collections].LogMinY));
      DEBUG ((DEBUG_INFO, "BitB_%d-%d ", ReportTable->Report[Index].Collection[Collections].BitStartB, ReportTable->Report[Index].Collection[Collections].BitStopB));
      DEBUG ((DEBUG_INFO, "BitX_%d-%d ", ReportTable->Report[Index].Collection[Collections].BitStartX, ReportTable->Report[Index].Collection[Collections].BitStopX));
      DEBUG ((DEBUG_INFO, "Input_%d_Rel_%d ", ReportTable->Report[Index].Collection[Collections].BitsInputX, ReportTable->Report[Index].Collection[Collections].BitsInputX & HID_FLAG_RELATIVE));
      DEBUG ((DEBUG_INFO, "BitY_%d-%d ", ReportTable->Report[Index].Collection[Collections].BitStartY, ReportTable->Report[Index].Collection[Collections].BitStopY));
      DEBUG ((DEBUG_INFO, "Input_%d_Rel_%d ", ReportTable->Report[Index].Collection[Collections].BitsInputY, ReportTable->Report[Index].Collection[Collections].BitsInputY & HID_FLAG_RELATIVE));
      DEBUG ((DEBUG_INFO, "\n"));
    }
  }
}

/**
  Convert Output X/Y Data to INT

  @param[in]  BitCount      Number of BITs which indicates the size of Output
  @param[in]  Output        X/Y Data to convert

  @retval INT32             Converted X/Y data
**/
INT32
STATIC
ConvertOutputToInt (
  UINT32 BitCount,
  UINT32 Output
  )
{
  if (BitCount > 16) {
    return (INT32) Output;
  } else if (BitCount > 8) {
    return (INT16) Output;
  }
  return  (INT8) Output;
}

/*
  This function uses dictionaries to parse incoming InputReport and convert it into X/Y coordinates plus Button info.

  @param[in]  This                    Pointer to the HID_INTERFACE_PROTOCOL instance
  @param[in]  ReportTable             Report Table with all supported HID reports.
  @param[in]  InputStream             Pointer to the HID report.
  @param[out] Output                  Result X/Y/B data.
  @param[out] MouseTouchOutput        Result relative X/Y data.
  @param[in]  MinMax                  X/Y Min and Max data.
  @param[in]  HidSolutionFlag         Flag for HID protocol.
  @param[out] UsageDevice             Device usage

  @retval EFI_SUCCESS       Parsing completed.
  @retval EFI_NOT_FOUND     Corresponding Report ID was not found in the Report Table.
*/
EFI_STATUS
EFIAPI
HidParseInput (
  IN HID_INTERFACE_PROTOCOL     *This,
  IN HID_INPUT_REPORT_TABLE     ReportTable,
  IN UINT8                      *InputStream,
  IN OUT HID_TOUCH_OUTPUT       *Output,
  IN OUT HID_REL_TOUCH_OUTPUT   *MouseTouchOutput,
  IN HID_XY_BOUNDARY            *MinMax,
  IN UINT8                      HidSolutionFlag,
  IN OUT UINT8                  *UsageDevice
  )
{
  UINT32                Index;
  UINT32                BitIndex;
  UINT8*                RawData;
  UINT32                Collections;
  UINT32                BitCountX;
  UINT32                BitCountY;
  UINT32                BitStopX;
  UINT32                BitStopY;
  HID_TOUCH_OUTPUT      *LastOutput;
  HID_REL_TOUCH_OUTPUT  *LastMouseOutput;

  // For mouse relative pointer report, we need to save the last (X,Y) position.
  LastMouseOutput = AllocateZeroPool (sizeof (HID_REL_TOUCH_OUTPUT));
  if (LastMouseOutput == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (LastMouseOutput, MouseTouchOutput, sizeof (HID_REL_TOUCH_OUTPUT));

  // For touchpad relative pointer report, we need to save the last (X,Y) position.
  LastOutput = AllocateZeroPool (sizeof (HID_TOUCH_OUTPUT));
  if (LastOutput == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (LastOutput, Output, sizeof (HID_TOUCH_OUTPUT));

  // Reset Output state.
  Output->X = 0;
  Output->Y = 0;
  Output->B = 0;

  MinMax->MaxX = 0;
  MinMax->MinX = 0;
  MinMax->MaxY = 0;
  MinMax->MinY = 0;

  // Skip content length
  InputStream++;
  InputStream++;

  // Save the pointer for Report ID
  RawData = InputStream;
  //Skip Report ID and point to raw pointer data.
  RawData++;

  //
  // Incoming data is parsed using proper dictionary selected by ReportID. In cases where
  // ReportID is not provided, it is guaranteed that only a single dictionary exists
  //
  // Dictionary describes which bits from input stream hold X / Y / Button data.
  // There's no guarantee coordinates will be byte-aligned, therefore we use bit access
  //
  for (Index = 0; Index < ReportTable.Quantity; Index++) {
    DEBUG ((DEBUG_VERBOSE, "HidParseInput Report ID 0x%X \n", InputStream[0]));
    if (InputStream[0] == ReportTable.Report[Index].Id) {
      for (Collections = 0; Collections < ReportTable.Report[Index].CollectionCount; Collections++) {
        //
        // ignore other collections if already got valid data
        //
        if (Output->X != 0 && Output->Y != 0) {
          return EFI_SUCCESS;
        }
        Output->B = 0;
        Output->X = 0;
        Output->Y = 0;
        MinMax->MaxX = 0;
        MinMax->MinX = 0;
        MinMax->MaxY = 0;
        MinMax->MinY = 0;

        BitCountX = 0;
        //
        // For Elan Panel,if ReportID BitStartX --> BitStopX is more than 2 bytes or equal 4 byte, use 2 byte only as BitStopX
        //
        BitStopX = ReportTable.Report[Index].Collection[Collections].BitStopX;
        if ((HidSolutionFlag == ElanHidProtocol) &&
            ((ReportTable.Report[Index].Collection[Collections].BitStopX - ReportTable.Report[Index].Collection[Collections].BitStartX) == ELAN_HID_MAX_LENGTH)) {
          BitStopX = ReportTable.Report[Index].Collection[Collections].BitStopX - ELAN_HID_SKIP_LENGTH;
        }
        for (BitIndex = ReportTable.Report[Index].Collection[Collections].BitStartX; BitIndex < BitStopX; BitIndex++) {
          Output->X += HidAccessBit (RawData, BitIndex) << (BitIndex - ReportTable.Report[Index].Collection[Collections].BitStartX);
          BitCountX++;
        }

        BitCountY = 0;
        //
        // For Elan Panel,if Report_ID BitStartY --> BitStopY is more than 2 bytes or equal 4 byte, use 2 byte only as BitStopY
        //
        BitStopY = ReportTable.Report[Index].Collection[Collections].BitStopY;
        if ((HidSolutionFlag == ElanHidProtocol) &&
            ((ReportTable.Report[Index].Collection[Collections].BitStopY - ReportTable.Report[Index].Collection[Collections].BitStartY) == ELAN_HID_MAX_LENGTH)) {
          BitStopY = ReportTable.Report[Index].Collection[Collections].BitStopY - ELAN_HID_SKIP_LENGTH;
        }
        for (BitIndex = ReportTable.Report[Index].Collection[Collections].BitStartY; BitIndex < BitStopY; BitIndex++) {
          Output->Y += HidAccessBit (RawData, BitIndex) << (BitIndex - ReportTable.Report[Index].Collection[Collections].BitStartY);
          BitCountY++;
        }

        for (BitIndex = ReportTable.Report[Index].Collection[Collections].BitStartB; BitIndex < ReportTable.Report[Index].Collection[Collections].BitStopB; BitIndex++) {
          Output->B += HidAccessBit(RawData, BitIndex) << (BitIndex - ReportTable.Report[Index].Collection[Collections].BitStartB);
        }

        MinMax->MinX = ReportTable.Report[Index].Collection[Collections].LogMinX;
        MinMax->MaxX = ReportTable.Report[Index].Collection[Collections].LogMaxX;
        MinMax->MinY = ReportTable.Report[Index].Collection[Collections].LogMinY;
        MinMax->MaxY = ReportTable.Report[Index].Collection[Collections].LogMaxY;

        if (ReportTable.Report[Index].UsageMouse == 1 && ReportTable.Report[Index].Collection[Collections].BitsInputX & HID_FLAG_RELATIVE) {
          *UsageDevice = Relative;
        } else {
          *UsageDevice = Absolute;
        }

        //
        // Touch Pad - converted data to absolute position
        //
        if ((ReportTable.Report[Index].UsageTouchPad == 1 && (*UsageDevice == Absolute)) ||
            (ReportTable.Report[Index].UsageMouse == 1 && (*UsageDevice == Absolute))) {
          //
          // Logical Max and Min in descriptor are only provided for the relative values
          //
          MinMax->MinX = 0;
          MinMax->MaxX = RELATIVE_COORDINATES_MAX_X;
          MinMax->MinY = 0;
          MinMax->MaxY = RELATIVE_COORDINATES_MAX_Y;

          // Calculate absolute X position using CurrentX onscreen and incoming relative_x data
          Output->X =
            MIN (
              MAX (((INT32) LastOutput->X + ConvertOutputToInt (BitCountX, Output->X)),
              (INT32) MinMax->MinX),
                (INT32) MinMax->MaxX
            );

          // Calculate absolute Y position using CurrentY onscreen and incoming Relative_Y data
          Output->Y =
            MIN (
              MAX (((INT32) LastOutput->Y + ConvertOutputToInt (BitCountY, Output->Y)),
              (INT32) MinMax->MinY),
                (INT32) MinMax->MaxY
            );
        }

        if (ReportTable.Report[Index].UsageMouse == 1 && (*UsageDevice == Relative)) {
          MouseTouchOutput->B = (UINT8) Output->B;
          //
          // Calculate relative X position using CurrentX onscreen
          //
          MouseTouchOutput->X = (INT32)LastMouseOutput->X + ConvertOutputToInt (BitCountX, Output->X);

          // Calculate relative Y position using CurrentY onscreen
          MouseTouchOutput->Y = (INT32)LastMouseOutput->Y + ConvertOutputToInt (BitCountY, Output->Y);
        }
      }
      FreePool (LastOutput);
      FreePool (LastMouseOutput);
      return EFI_SUCCESS;
    }
  }
  FreePool (LastOutput);
  FreePool (LastMouseOutput);
  return EFI_NOT_FOUND;
}


/**
  Parses HID Descriptor and creates Report Tables

  @param[in]  This              Pointer to the HID_INTERFACE_PROTOCOL instance
  @param[in]  InputReportTable  Pointer to the final report table that contains all the Reports
  @param[in]  ReportPacket      Pointer to the report packet format
  @param[in]  Descriptor        Pointer to the descriptor
  @param[in]  DescriptorLength  Size of the descriptor to parse

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval EFI_OUT_OF_RESOURCES  Failed to allocate memory.
**/
EFI_STATUS
EFIAPI
HidParseDescriptor (
  IN HID_INTERFACE_PROTOCOL *This,
  IN HID_INPUT_REPORT_TABLE *InputReportTable,
  IN HID_GET_REPORT_FORMAT  *ReportPacket,
  IN UINT8                  *Descriptor,
  IN UINT32                 DescriptorLength
  )
{
  UINT8            *MovingPointer;
  HID_PARSER_STACK *ParseStack;
  HID_TOKEN        Token;

  DEBUG((DEBUG_INFO, "%a \n", __FUNCTION__));

  ParseStack = (HID_PARSER_STACK*)AllocateZeroPool((UINTN)sizeof(HID_PARSER_STACK));

  if (ParseStack == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MovingPointer = Descriptor;

  ParseStack->TempReport.Id = 0xFFFF; //Invalid/Default report

  while (DescriptorLength > 0) {
    Token = HidGetToken (&MovingPointer, &DescriptorLength);
    HidUpdateStack (ParseStack, Token, InputReportTable, ReportPacket);
  }
  HidExportReport (ParseStack, InputReportTable);
  HidShowReportTable (InputReportTable);
  FreePool(ParseStack);
  return EFI_SUCCESS;
}

/**
  This is the declaration of an EFI image entry point. This entry point is
  the same for UEFI Applications, UEFI OS Loaders, and UEFI Drivers including
  both device drivers and bus drivers.

  @param  ImageHandle           The firmware allocated handle for the UEFI image.
  @param  SystemTable           A pointer to the EFI System Table.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
HidInterfaceDriverEntryPoint (
  IN EFI_HANDLE                 ImageHandle,
  IN EFI_SYSTEM_TABLE           *SystemTable
  )
{
  EFI_STATUS                    Status;
  HID_INTERFACE_PROTOCOL        *HidInterfaceProtocol;

  DEBUG ((DEBUG_INFO, "%a: Entry Point\n", __FUNCTION__));

  // Allocate memory for the protocol instance
  Status = gBS->AllocatePool (EfiBootServicesData, sizeof (HID_INTERFACE_PROTOCOL), (VOID**)&HidInterfaceProtocol);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  // Initialize the protocol instance
  HidInterfaceProtocol->Version             = HID_INTERFACE_PROTOCOL_VERSION;
  HidInterfaceProtocol->ParseDescriptor     = HidParseDescriptor;
  HidInterfaceProtocol->ParseInput          = HidParseInput;


  // Install the protocol interface
  Status = gBS->InstallProtocolInterface (
                  &ImageHandle,
                  &gHidProtocolGuid,
                  EFI_NATIVE_INTERFACE,
                  HidInterfaceProtocol
                  );
  if (EFI_ERROR(Status)) {
    gBS->FreePool(HidInterfaceProtocol);
  }
  DEBUG ((DEBUG_INFO, "%a: Install HidInterfaceProtocol Status %r\n", __FUNCTION__, Status));

  return Status;
}