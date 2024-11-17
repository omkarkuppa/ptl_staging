/** @file
  Post Code Library instance that writes post code values to I/O port 0x80 and Trace Tool.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include <Base.h>
#include <FspmUpd.h>
#include <Library/PostCodeLib.h>
#include <Library/PcdLib.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/FspCommonLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <Library/TraceHubDebugLib.h>
#include <Library/PostCodeToScratchPadLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoI2cLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/SmbusLib.h>

//
// Define the maximum debug and assert message length that this library supports
//
#define MAX_DEBUG_MESSAGE_LENGTH  0x100

/**
  Send 4 digit PostCode commands to Max6950 device
  @param[in]  PostCodeValue - 4 digit PostCode value to be displayed
**/
EFI_STATUS
EFIAPI
DisplayI2cPostCode (
  IN UINT32 PostCodeValue
  )
{
  UINT8           Index=0;
  UINT8           WriBuf [5] ={0, 0, 0 ,0,0};
  EFI_STATUS      Status;
  UINT8           I2cNumber = PcdGet8(PcdI2cPostCodePortNumber);

  Index =3;
  WriBuf [0] = PcdGet8(PcdI2cPostCodeCommand);
  WriBuf [1] = (UINT8)(PostCodeValue >> (4 * Index-- )) & 0xF;
  WriBuf [2] = (UINT8)(PostCodeValue >> (4 * Index--)) & 0xF;
  WriBuf [3] = (UINT8)(PostCodeValue >> (4 * Index--)) & 0xF;
  WriBuf [4] = (UINT8)(PostCodeValue >> (4 * Index )) & 0xF;

  Status = SerialIoI2cWrite (
              0, //as I2C hidden give PCICFG fixed address
              &I2cNumber, //I2cNumber
              PcdGet8(PcdI2cPostCodeAddress),
              PcdGet8(PcdI2cPostCodeByteCount),
              WriBuf,
              1000000, //1sec
              400, //400kHz
              NULL,
              TRUE,
              TRUE
              );
  return Status;
}

/**
  Send 4 digit PostCode commands to Max6958 device
  @param[in]  PostCodeValue - 4 digit PostCode value to be displayed
  @param[in]  Address       - Smbus slave address
  @param[in]  Command       - Smbus command
**/
EFI_STATUS
EFIAPI
DisplaySmbusPostCode (
  IN UINT32 PostCodeValue,
  IN UINT8  Address,
  IN UINT8  Command
  )
{
  UINT8           Index;
  UINT8           WriBuf[4] = {0, 0, 0 ,0};
  UINT8           Length = 4;
  EFI_STATUS      Status;
  UINTN           SmbusAddress;

  SmbusAddress = SMBUS_LIB_ADDRESS (Address, Command, Length, FALSE);
  Index = 3;
  WriBuf[0] = (UINT8)(PostCodeValue >> (4 * Index-- )) & 0xF;
  WriBuf[1] = (UINT8)(PostCodeValue >> (4 * Index--)) & 0xF;
  WriBuf[2] = (UINT8)(PostCodeValue >> (4 * Index--)) & 0xF;
  WriBuf[3] = (UINT8)(PostCodeValue >> (4 * Index )) & 0xF;

  SmBusWriteBlock (SmbusAddress, WriBuf, &Status);

  return Status;
}

/**
  Prints a Post Code message to the Trace Hub device if the specified error level is enabled.

  @param  Format      Format string for the Post Code message to print.
  @param  ...         Variable argument list whose contents are accessed
                      based on the format string specified by Format.

**/
VOID
EFIAPI
PrintPostCodeFormat (
  IN  CONST CHAR8  *Format,
  ...
  )
{
  CHAR8    Buffer[MAX_DEBUG_MESSAGE_LENGTH];
  VA_LIST  Marker;

  //
  // Convert the message to an ASCII String
  //
  VA_START (Marker, Format);
  AsciiVSPrint (Buffer, sizeof (Buffer), Format, Marker);
  VA_END (Marker);

  //
  // Send to DebugLib
  //
  DEBUG ((DEBUG_INFO, "%a", Buffer));

  //
  // Send the print string to Trace Hub
  //
  TraceHubDebugWrite (SeverityNormal, (UINT8 *) Buffer, AsciiStrLen (Buffer));
}

/**
  Sends an 32-bit value to a POST card.

  Sends the 32-bit value specified by Value to a POST card, and returns Value.
  Some implementations of this library function may perform I/O operations
  directly to a POST card device. Other implementations may send Value to
  ReportStatusCode(), and the status code reporting mechanism will eventually
  display the 32-bit value on the status reporting device.

  @param[in] Value   The 32-bit value to write to the POST card.

  @retval    Value   The 32-bit value to write to the POST card.
**/
UINT32
EFIAPI
PostCode (
  IN UINT32  Value
  )
{
  SetPostCodeToScratchPad (Value);
  return PostCodeWithDescription (Value, NULL);
}


/**
  Sends an 32-bit value to a POST and associated ASCII string.

  Sends the 32-bit value specified by Value to a POST card, and returns Value.
  If Description is not NULL, then the ASCII string specified by Description is
  also passed to the handler that displays the POST card value. Some
  implementations of this library function may perform I/O operations directly
  to a POST card device. Other implementations may send Value to ReportStatusCode(),
  and the status code reporting mechanism will eventually display the 32-bit
  value on the status reporting device.

  PostCodeWithDescription()must actively prevent recursion. If
  PostCodeWithDescription() is called while processing another any other Post
  Code Library function, then PostCodeWithDescription() must return Value
  immediately.

  @param[in] Value        The 32-bit value to write to the POST card.
  @param[in] Description  The pointer to an ASCII string that is a description of the
                          POST code value. This is an optional parameter that may
                          be NULL.

  @retval    Value        The 32-bit value to write to the POST card.
**/
UINT32
EFIAPI
PostCodeWithDescription (
  IN UINT32       Value,
  IN CONST CHAR8  *Description  OPTIONAL
  )
{
  FSPM_UPD                     *FspmUpd;
  UINT16                       PostCodeOutputPort;
  FSP_GLOBAL_DATA              *FspData;
  UINT8                        I2cPostCodeEnable;
  UINT8                        SmbusPostCodeEnable;

  // Postcode display using port 80
  FspData = GetFspGlobalDataPointer ();
  if (((UINTN)FspData == 0x00 || (UINTN)FspData == 0xFFFFFFFF) ||
    (FspData->Signature != FSP_GLOBAL_DATA_SIGNATURE)) {
    if (PcdGet8 (PcdI2cPostCode) == 0x1) {
      // Postcode display using I2C Interface
      DisplayI2cPostCode (Value);
    } else if (PcdGet8 (PcdSmbusPostCode) == 0x1) {
      // Postcode display using Smbus Interface
      DisplaySmbusPostCode (Value, PcdGet8(PcdSmbusPostCodeAddress), PcdGet8(PcdSmbusPostCodeCommand));
    } else {
      IoWrite16 (0x80, (UINT16) Value);
    }
  } else if ((FspData->FspMode == FSP_IN_API_MODE) && (FspData->MemoryInitUpdPtr != NULL)) { // FSP_MODE_CHECK
    FspmUpd = FspData->MemoryInitUpdPtr;
    PostCodeOutputPort = FspmUpd->FspmConfig.PostCodeOutputPort;
    I2cPostCodeEnable = FspmUpd->FspmConfig.I2cPostCodeEnable;
    SmbusPostCodeEnable = FspmUpd->FspmConfig.SmbusPostCodeEnable;
    Value &= 0xFFF;
    //Get the PostCode set using SetPhaseStatusCode
    Value |= GetPhaseStatusCode();
    if (I2cPostCodeEnable == 0x1) {
      DisplayI2cPostCode (Value);
    } else if (SmbusPostCodeEnable == 0x1) {
      // Postcode display using Smbus Interface
      DisplaySmbusPostCode (Value, FspmUpd->FspmConfig.SmbusPostCodeAddress, FspmUpd->FspmConfig.SmbusPostCodeCommand);
    } else {
      IoWrite16 (PostCodeOutputPort, (UINT16) Value);
    }
  } else {
    Value &= 0xFFF;
    //Get the PostCode set using SetPhaseStatusCode
    Value |= GetPhaseStatusCode();
    if (PcdGet8 (PcdI2cPostCode) == 0x1) {
      // Postcode display using I2C Interface
      DisplayI2cPostCode (Value);
    } else if (PcdGet8 (PcdSmbusPostCode) == 0x1) {
      // Postcode display using Smbus Interface
      DisplaySmbusPostCode (Value, PcdGet8(PcdSmbusPostCodeAddress), PcdGet8(PcdSmbusPostCodeCommand));
    } else {
      IoWrite16 (0x80, (UINT16) Value);
    }
  }

#if FixedPcdGetBool (PcdTraceHubCatalogEnable) == 1
  TraceHubWriteCataLog64_0 (SeverityNormal, Value);
  return Value;
#else
  if (Description == NULL) {
    PrintPostCodeFormat ("POSTCODE = <%08x>\n", Value);
  } else {
    PrintPostCodeFormat ("POSTCODE = <%08x> - %a\n", Value, Description);
  }
  return Value;
#endif
}


/**
  Returns TRUE if POST Codes are enabled.

  This function returns TRUE if the POST_CODE_PROPERTY_POST_CODE_ENABLED
  bit of PcdPostCodePropertyMask is set.  Otherwise FALSE is returned.

  @retval  TRUE   The POST_CODE_PROPERTY_POST_CODE_ENABLED bit of
                  PcdPostCodeProperyMask is set.
  @retval  FALSE  The POST_CODE_PROPERTY_POST_CODE_ENABLED bit of
                  PcdPostCodeProperyMask is clear.
**/
BOOLEAN
EFIAPI
PostCodeEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8(PcdPostCodePropertyMask) & POST_CODE_PROPERTY_POST_CODE_ENABLED) != 0);
}


/**
  Returns TRUE if POST code descriptions are enabled.

  This function returns TRUE if the POST_CODE_PROPERTY_POST_CODE_DESCRIPTION_ENABLED
  bit of PcdPostCodePropertyMask is set. Otherwise FALSE is returned.

  @retval  TRUE   The POST_CODE_PROPERTY_POST_CODE_DESCRIPTION_ENABLED bit of
                  PcdPostCodeProperyMask is set.
  @retval  FALSE  The POST_CODE_PROPERTY_POST_CODE_DESCRIPTION_ENABLED bit of
                  PcdPostCodeProperyMask is clear.
**/
BOOLEAN
EFIAPI
PostCodeDescriptionEnabled (
  VOID
  )
{
  return (BOOLEAN) ((PcdGet8(PcdPostCodePropertyMask) & POST_CODE_PROPERTY_POST_CODE_DESCRIPTION_ENABLED) != 0);
}
