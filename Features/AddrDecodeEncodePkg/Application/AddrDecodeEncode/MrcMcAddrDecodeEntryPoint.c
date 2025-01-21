/** @file
  File to support address decoding and encoding
  Entry Point for the AddrDecodeEncode EFI application
INTEL CONFIDENTIAL
@copyright
 Copyright (c) 2023 Intel Corporation. All rights reserved
 This software and associated documentation (if any) is furnished
 under a license and may only be used or copied in accordance
 with the terms of the license. Except as permitted by the
 license, no part of this software or documentation may be
 reproduced, stored in a retrieval system, or transmitted in any
 form or by any means without the express written consent of
 Intel Corporation.
 This file contains an 'Intel Peripheral Driver' and is uniquely
 identified as "Intel Reference Module" and is licensed for Intel
 CPUs and chipsets under the terms of your license agreement with
 Intel or your vendor. This file may be modified by the user, subject
 to additional terms of the license agreement.
@par Specification Reference:
**/
#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/ShellLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Protocol/MemAddrDecodeEncode.h>

#define MAJOR_VERSION   1
#define MINOR_VERSION   0

STATIC CONST SHELL_PARAM_ITEM DecodeParamList[] = {
  {L"-h", TypeFlag},
  {L"-H", TypeFlag},
  {L"-v", TypeFlag},
  {L"-V", TypeFlag},
  {L"-e", TypeFlag},
  {L"-E", TypeFlag},
  {L"-d", TypeFlag},
  {L"-D", TypeFlag},
  {L"-l", TypeFlag},
  {L"-L", TypeFlag},
  {NULL, TypeMax}
  };

/**
   Display current version.
**/
VOID
ShowVersion (
  )
{
  ShellPrintEx (-1, -1, L"AddrDecodeEncode: Version %d.%02d\n", MAJOR_VERSION, MINOR_VERSION);
}

/**
   Display Usage and Help information.
**/
VOID
ShowHelp (
  )
{
  ShellPrintEx (-1, -1, L"AddrDecodeEncode [-e] [Controller] [Channel] [Dimm] [Rank] [BankGroup] [Bank] [RAS] [CAS]\n");
  ShellPrintEx (-1, -1, L"  -e         Specifies input of a Dram Address to be Encoded.\n");
  ShellPrintEx (-1, -1, L"  Controller  Specifies the Controller {0..1}\n");
  ShellPrintEx (-1, -1, L"  Channel     Specifies the Channel    {0..1}\n");
  ShellPrintEx (-1, -1, L"  Dimm        Specifies the Dimm       {0..1}\n");
  ShellPrintEx (-1, -1, L"  Rank        Specifies the Rank       {0..1}\n");
  ShellPrintEx (-1, -1, L"  BankGroup   Specifies the BankGroup  {0..3}\n");
  ShellPrintEx (-1, -1, L"  Bank        Specifies the Bank       {0..7}\n");
  ShellPrintEx (-1, -1, L"  RAS         Specifies the RAS        {0..2^17-1}\n");
  ShellPrintEx (-1, -1, L"  CAS         Specifies the CAS        {0..2^10-1}\n");
  ShellPrintEx (-1, -1, L"  The output of this command will be the System Address associated with the Dram Address\n\n");
  ShellPrintEx (-1, -1, L"AddrDecodeEncode [-d] [System Address]\n");
  ShellPrintEx (-1, -1, L"  -d            Specifies input of a System Address to be Decoded.\n");
  ShellPrintEx (-1, -1, L"  SystemAddress  Specifies the System Address.\n");
  ShellPrintEx (-1, -1, L"  The output of this command will be the Dram Address associated with the System Address\n");
}

/**
 * main entry of module
 *
 * @param ImageHandle
 * @param SystemTable
 *
 * @return EFI_STATUS EFIAPI
 */
EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE       ImageHandle,
  IN EFI_SYSTEM_TABLE *SystemTable
  )
{
  DRAM_ADDRESS   DramAddress;
  UINT64         SystemAddress;
  UINT64         Start, End, Looper, Temp;
  EFI_STATUS     Status = EFI_SUCCESS;
  LIST_ENTRY     *ParamPackage;
  CHAR16         *StringValue;
  UINT64         Value;
  MEMORY_ADDRESS_ENCODE_DECODE_PROTOCOL *MemEncodeDecodeProtocol;

  ParamPackage  = NULL;
  StringValue = NULL;

  //
  // initialize the shell lib (we must be in non-auto-init...)
  //
  Status = ShellInitialize ();
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gMemoryAddressEncodeDecodeProtocolGuid, NULL, (VOID **) &MemEncodeDecodeProtocol);
  ASSERT_EFI_ERROR (Status);

  //
  // parse the command line
  //
  Status = ShellCommandLineParse (DecodeParamList, &ParamPackage, NULL, TRUE);
  if (EFI_ERROR (Status)) {
    Status = EFI_INVALID_PARAMETER;
    goto Done;
  }

  //
  // check for "-?" help information, and for "-v" for version inforamtion.
  //
  if (ShellCommandLineGetFlag (ParamPackage, L"-h")  ||
      ShellCommandLineGetFlag (ParamPackage, L"-H")) {
    ShowHelp ();
    goto Done;
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"-v") ||
             ShellCommandLineGetFlag (ParamPackage, L"-V")) {
    ShowVersion ();
    goto Done;
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"-e") ||
             ShellCommandLineGetFlag (ParamPackage, L"-E")) {

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 1);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.Controller = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 2);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.ChannelNumber = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 3);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.DimmNumber = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 4);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.Rank = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 5);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.BankGroup = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 6);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.Bank = (UINT8) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 7);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.Ras = (UINT16) Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 8);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    DramAddress.Cas = (UINT32) Value;

    ShellPrintEx (
      -1,
      -1,
      L"Controller %d Ch %d Dimm %d Rank %d BankGroup %d Bank %d RAS 0x%X CAS 0x%X\n",
      DramAddress.Controller,
      DramAddress.ChannelNumber,
      DramAddress.DimmNumber,
      DramAddress.Rank,
      DramAddress.BankGroup,
      DramAddress.Bank,
      DramAddress.Ras,
      DramAddress.Cas
    );

    DEBUG ((
      DEBUG_INFO,
      "Controller %d Ch %d Dimm %d Rank %d BankGroup %d Bank %d RAS 0x%X CAS 0x%X\n",
      DramAddress.Controller,
      DramAddress.ChannelNumber,
      DramAddress.DimmNumber,
      DramAddress.Rank,
      DramAddress.BankGroup,
      DramAddress.Bank,
      DramAddress.Ras,
      DramAddress.Cas
    ));

    Status = MemEncodeDecodeProtocol->MemoryAddressEncode (&DramAddress, &SystemAddress);

    if (Status == EFI_SUCCESS) {
      ShellPrintEx (
        -1,
        -1,
        L"\nSystemAddress = 0x%llX\n",
        SystemAddress
      );

      DEBUG ((
        DEBUG_INFO,
        " - System Address = 0x%llX\n",
        SystemAddress
      ));
    } else {
      ShellPrintEx (
        -1,
        -1,
        L"Failed to encode address\n");
      DEBUG ((
        DEBUG_INFO,
      "Failed to encode address\n"));
    }

    goto Done;
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"-d") ||
             ShellCommandLineGetFlag (ParamPackage, L"-D")) {
    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 1);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    SystemAddress = Value;

    ShellPrintEx (
      -1,
      -1,
      L"\nSystemAddress = 0x%llX\n",
      SystemAddress
      );

    DEBUG ((
      DEBUG_INFO,
      " - System Address = 0x%llX\n",
      SystemAddress
      ));

    Status = MemEncodeDecodeProtocol->MemoryAddressDecode (SystemAddress, &DramAddress);

    if (Status == EFI_SUCCESS) {
      ShellPrintEx (
        -1,
        -1,
        L"Controller %d Ch %d Dimm %d Rank %d BankGroup %d Bank %d RAS 0x%X CAS 0x%X\n",
        DramAddress.Controller,
        DramAddress.ChannelNumber,
        DramAddress.DimmNumber,
        DramAddress.Rank,
        DramAddress.BankGroup,
        DramAddress.Bank,
        DramAddress.Ras,
        DramAddress.Cas
      );

      DEBUG ((
        DEBUG_INFO,
        "Controller %d Ch %d Dimm %d Rank %d BankGroup %d Bank %d RAS 0x%X CAS 0x%X\n",
        DramAddress.Controller,
        DramAddress.ChannelNumber,
        DramAddress.DimmNumber,
        DramAddress.Rank,
        DramAddress.BankGroup,
        DramAddress.Bank,
        DramAddress.Ras,
        DramAddress.Cas
      ));
    } else {
    ShellPrintEx (
      -1,
      -1,
      L"Failed to decode address\n");
    DEBUG ((
      DEBUG_INFO,
      "Failed to decode address\n"));

    }
  }

  if (ShellCommandLineGetFlag (ParamPackage, L"-l") ||
             ShellCommandLineGetFlag (ParamPackage, L"-L")) {
    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 1);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    Start = Value;

    StringValue = (CHAR16 *) ShellCommandLineGetRawValue (ParamPackage, 2);
    ShellConvertStringToUint64 (StringValue, &Value, FALSE, FALSE);
    End =  Value;

    for (Looper = Start; Looper <= End; Looper += 0x8) {
        Status = MemEncodeDecodeProtocol->MemoryAddressDecode (Looper, &DramAddress);
        if (Status != EFI_SUCCESS) {
            ShellPrintEx (
                -1,
                -1,
                L"Failed to decode address = 0x%llX\n", Looper);
            DEBUG ((
                DEBUG_INFO,
                "Failed to decode address = 0x%llX\n", Looper));
            break;
        }
        Status = MemEncodeDecodeProtocol->MemoryAddressEncode (&DramAddress, &Temp);
        if (Status != EFI_SUCCESS) {
            ShellPrintEx (
                -1,
                -1,
                L"Failed to encode address = 0x%llX\n", Looper);
            DEBUG ((
                DEBUG_INFO,
                "Failed to encode address = 0x%llX\n", Looper));
            break;
        }

        if (Looper == Temp) {
            ShellPrintEx (
                -1,
                -1,
                L"address = 0x%llX Pass\n", Looper);
            DEBUG ((
                DEBUG_INFO,
                "address = 0x%llX Pass\n", Looper));
        } else {
            ShellPrintEx (
                -1,
                -1,
                L"address = 0x%llX Fail, encoded to 0x%llX\n", Looper, Temp);
            DEBUG ((
                DEBUG_INFO,
                "address = 0x%llX Fail, encoded to 0x%llX\n", Looper, Temp));
        }
    }
  }


Done:

  //
  // free the command line package
  // map to ShellCommandLineParse
  //
  ShellCommandLineFreeVarList (ParamPackage);
  ParamPackage = NULL;

  return Status;
}
