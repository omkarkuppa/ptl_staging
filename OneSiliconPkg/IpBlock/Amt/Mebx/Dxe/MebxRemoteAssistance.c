/** @file
  This file contains source for Remote Assistance.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PrintLib.h>
#include <Library/TimerLib.h>
#include <Library/AmthiMsgsLib.h>
#include <Library/DxeMebxDisplayWrapperLib.h>

#define FormatIpv4TextStr(OutputUnicodeBuffer, IpAddress) \
  UnicodeSPrint (OutputUnicodeBuffer, 0x40, L"%d.%d.%d.%d",(UINTN)((IpAddress)->SegAddr[3]), \
  (UINTN)((IpAddress)->SegAddr[2]), (UINTN)((IpAddress)->SegAddr[1]), (UINTN)((IpAddress)->SegAddr[0]))

#define IPV6_ADRESSES_MAX_NUMBER         4
#define REMOTE_ASSISTANCE_ABORTED_DELAY  5000000 ///< 5 secs
#define PT_CONSOLE_DELAY                 2000000 ///< 2 second delay
#define KEYSTROKE_MSEC_DELAY             1000
#define MAX_IP_TEXT_LEN                  16

/**
  Display IPv6 parameters during initialization of CIRA/CILA flow.

  @param[in] Interface           LAN Interface type:
                                   0 - Wired
                                   1 - Wireless
**/
VOID
PrintIpv6Summary (
  IN UINT32 Interface
  )
{
  EFI_STATUS                      Status;
  UINT32                          Index;
  BOOLEAN                         StaticIpv6Present;
  BOOLEAN                         DynamicIpv6Present;
  CFG_IP_ADDR                     TempIpv6Addresses[IPV6_ADRESSES_MAX_NUMBER];
  IPV6_LAN_INTERACE_STRUC         Ipv6Status;
  CFG_IPV6_LAN_INTERFACE_SETTINGS Ipv6Settings;
  UINTN                           CurrentIpv6Address;
  UINTN                           NumValidIpv6Add;
  CHAR16                          pIpv6AddressString[CFG_IPV6_ADDR_LEN_MAX];

  Index              = 0;
  StaticIpv6Present  = FALSE;
  DynamicIpv6Present = FALSE;
  CurrentIpv6Address = 0;
  NumValidIpv6Add    = 0;

  ZeroMem ((UINT8*) &Ipv6Status, sizeof (IPV6_LAN_INTERACE_STRUC));
  ZeroMem ((UINT8*) &Ipv6Settings, sizeof (CFG_IPV6_LAN_INTERFACE_SETTINGS));


  // We need to print the Ipv6 addresses in the following format defined in RCR CCG0100008496
  // IPv6 Addresses for wired interface:    [x:x:x::x:x]
  //                                        [x:x:x::x:x]
  //                                        [x:x:x::x:x]
  //                                        [x:x:x::x:x]
  //
  // Default Gateway (for static IPv6 only): [x:x:x::x:x]
  //
  // Up to 4 IPv6 addresses:
  // -  No need to display link local address
  // -  No need to display static address when there are dynamic addresses configured

  Status = AmthiCfgGetIPv6LanIntStatus (Interface, &Ipv6Status);
  if (EFI_ERROR (Status)) {
    if (Status == EFI_NOT_STARTED) {
      return;
    }

    MebxDisplayError (AmthiGetIpv6LanIntfSettingsApiError);
    return;
  }

  // First scan the address types to identify Static/Manual vs. Dynamic/DHCP
  for (Index = 0; Index < Ipv6Status.NumOfAddr; Index++) {
    if (Ipv6Status.Ipv6Addresses[Index].Type == Ipv6AddrTypeManual) {
      StaticIpv6Present = TRUE;
    }

    if (Ipv6Status.Ipv6Addresses[Index].Type == Ipv6AddrTypeDhcp) {
      DynamicIpv6Present = TRUE;
    }
  }

  // Second, copy the required address list to be displayed into an ordered TempIpv6Address array.
  for (Index = 0; Index < Ipv6Status.NumOfAddr; Index++) {
    //
    // Only Two rules exists when filtering out addresses
    // No need to display link local address
    // No need to display static address when there are dynamic addresses configured
    //
    if (((Ipv6Status.Ipv6Addresses[Index].Type == Ipv6AddrTypeManual) && DynamicIpv6Present) ||
        (Ipv6Status.Ipv6Addresses[Index].Type == Ipv6AddrTypeLinkLocal)) {
      continue;
    }
    // Only 4 addresses are allowed
    if (CurrentIpv6Address >= IPV6_ADRESSES_MAX_NUMBER) {
      break;
    }
    CopyMem (
      TempIpv6Addresses[CurrentIpv6Address].Address,
      (UINT8*) Ipv6Status.Ipv6Addresses[Index].Address.Address,
      sizeof (CFG_IP_ADDR)
      );
    CurrentIpv6Address++;
  }

  // CurrentIpv6Address indicates the number of IPV6 addresses we have identified to display.
  NumValidIpv6Add = CurrentIpv6Address;
  CurrentIpv6Address = 0;

  for (CurrentIpv6Address = 0; CurrentIpv6Address < NumValidIpv6Add; CurrentIpv6Address++) {
    //
    // Convert the address ASCII array to Unicode and print the first line in the address list.
    // MebxAsciiPrint() takes only UNICODE formatted buffer
    //
    ZeroMem (pIpv6AddressString, sizeof (pIpv6AddressString));
    AsciiStrToUnicodeStrS ((CHAR8 *) &TempIpv6Addresses[CurrentIpv6Address].Address, pIpv6AddressString, CFG_IPV6_ADDR_LEN_MAX);

    if (CurrentIpv6Address == 0) {
      //
      // First line has different string format
      //
      if (Interface == LanInterfaceWired) {
        AsciiPrint ("Wired IPV6 addresses:         [%s]\n\r", pIpv6AddressString);
      } else if (Interface == LanInterfaceWireless) {
        AsciiPrint ("Wireless IPV6 addresses:      [%s]\n\r", pIpv6AddressString);
      }
    } else {
      //
      // Subsequent lines have the same format
      //
      AsciiPrint ("                              [%s]\n\r",   pIpv6AddressString);
    }
  }

  // No need to display the default gateway when dynamic addresses are configured.
  if ((NumValidIpv6Add) && (StaticIpv6Present) && (!DynamicIpv6Present)) {
    ZeroMem (pIpv6AddressString, sizeof (pIpv6AddressString));
    AsciiStrToUnicodeStrS ((CHAR8 *) &Ipv6Status.Ipv6DefaultRouter.Address, pIpv6AddressString, CFG_IPV6_ADDR_LEN_MAX);
    AsciiPrint ("Default Gateway:              [%s]\n\r", pIpv6AddressString);
  }

  AsciiPrint ("\n\n\r");
}

/**
  Display IPv4 parameters during initialization of CIRA/CILA flow.

  @param[in] Interface           LAN Interface type:
                                   0 - Wired
                                   1 - Wireless
**/
VOID
PrintIpv4Summary (
  IN UINT32              Interface
  )
{
  EFI_STATUS             Status;
  CHAR16                 TempIpv4Text[MAX_IP_TEXT_LEN];
  IMB_TCPIP_PARAMS       TcpipCfg;
  LAN_INTERFACE_SETTINGS LanInterfaceSettings;

  ZeroMem ((UINT8*) &TempIpv4Text, MAX_IP_TEXT_LEN);
  ZeroMem ((UINT8*) &TcpipCfg, sizeof (IMB_TCPIP_PARAMS));
  ZeroMem (&LanInterfaceSettings, sizeof (LAN_INTERFACE_SETTINGS));

  Status = AmthiGetLanInterfaceSettings (&LanInterfaceSettings, Interface);
  if (Status == EFI_NO_MEDIA) {
    return;
  }

  if (LanInterfaceSettings.DhcpEnabled == 1) {
    if (Interface == LanInterfaceWired) {
      AsciiPrint ("Wired IPV4 addresses:         Dhcp Enabled: Yes\n\r");
     } else {
      AsciiPrint ("Wireless IPV4 addresses:      Dhcp Enabled: Yes\n\r");
    }
  } else {
    if (Interface == LanInterfaceWired) {
      AsciiPrint ("Wired IPV4 addresses:         Dhcp Enabled: No\n\r");
    } else {
      AsciiPrint ("Wireless IPV4 addresses:      Dhcp Enabled: No\n\r");
    }
  }

  FormatIpv4TextStr (TempIpv4Text, (IP_ADDR*) &LanInterfaceSettings.Ipv4Address);
  AsciiPrint ("                              IP Address: %s\n\r", TempIpv4Text);

  //
  // Only display Gateway if static IP is configured
  //
  if (LanInterfaceSettings.DhcpEnabled == 0) {
    Status = AmthiGetIpv4Params (&TcpipCfg);
    if (EFI_ERROR (Status)) {
      //
      // Lanless platform -> set gateway to 0.0.0.0
      //
      TcpipCfg.GatewayAddr = 0;
    }

    FormatIpv4TextStr (TempIpv4Text, (IP_ADDR*) &TcpipCfg.GatewayAddr);
    AsciiPrint ("                              Default Gateway: %s\n\r", TempIpv4Text);
  }
  AsciiPrint ("\n\r");
}

/**
  Perform CILA Flow.

  @retval EFI_SUCCESS            CILA Flow performed successfully
  @retval Others                 Error has occurred
**/
EFI_STATUS
PerformCilaFlow (
  VOID
  )
{
  EFI_STATUS            Status;
  FQDN_DATA             FqdnData;
  CHAR16                TempUnicodeString[MAX_STRING_LENGTH_FQDN];
  MEBX_USER_INPUT       UserInput;

  ZeroMem (&FqdnData, sizeof (FQDN_DATA));

  MebxDisplayText (AmtManageabilityCilaMsg);

  Status = AmthiGetFqdn (&FqdnData);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetFqdnApiError);
    return Status;
  }

  //
  // Add null-termination character since acquired string
  // doesn't have to be null-terminated
  //
  FqdnData.Fqdn.Buffer[FqdnData.Fqdn.Length] = '\0';

  AsciiPrint ("Intel (R) Manageability Application Network Status:\n\n\r");
  AsciiStrToUnicodeStrS ((CHAR8 *) &FqdnData.Fqdn.Buffer, TempUnicodeString, sizeof (FqdnData.Fqdn.Buffer));
  AsciiPrint ("FQDN:                         [%s]\n\n\r", TempUnicodeString);

  PrintIpv4Summary (LanInterfaceWired);
  PrintIpv6Summary (LanInterfaceWired);
  PrintIpv4Summary (LanInterfaceWireless);
  PrintIpv6Summary (LanInterfaceWireless);

  AsciiPrint ("Press <Enter> to continue boot\n\r");

  // Loop till user presses Enter
  while (TRUE) {
    Status = MebxReadUserInput (&UserInput);
    if (EFI_ERROR (Status)) {
      break;
    }
    if (UserInput == MebxUserInputEnter) {
      break;
    }
  }

  return Status;
}

/**
  Perform CIRA Flow.

  @param[in] ConnectionData      Connection Data

  @retval EFI_SUCCESS            CIRA Flow performed successfully
  @retval Others                 Error has occurred
**/
EFI_STATUS
PerformCiraFlow (
  IN REMOTE_ACCESS_CONNECTION_STRUCT *ConnectionData
  )
{
  UINT32          Timeout;
  CHAR16          MpsHostName[MAX_STRING_LENGTH_FQDN];
  CHAR16          OldMpsHostName[MAX_STRING_LENGTH_FQDN];
  EFI_STATUS      Status;
  MEBX_USER_INPUT UserInput;

  Timeout   = KEYSTROKE_MSEC_DELAY;
  UserInput = MebxUserInputNoInput;

  ZeroMem ((UINT8*) &MpsHostName, MAX_STRING_LENGTH_FQDN * sizeof (CHAR16));
  ZeroMem ((UINT8*) &OldMpsHostName, MAX_STRING_LENGTH_FQDN * sizeof (CHAR16));

  //
  // Populate initial value of MPS server, it may be already connected
  // Since pMpsHostName has just been allocated with 0 content, no need to clear it
  //
  AsciiStrToUnicodeStrS ((CHAR8 *) &ConnectionData->MpsHostname.Buffer, MpsHostName, MAX_STRING_LENGTH_FQDN);

  while ((ConnectionData->RemoteAccessConnectionStatus == RemoteAccessConnectionStatusConnecting) &&
         (UserInput != MebxUserInputEsc)) {

    Status = AmthiGetConnectionStatus (ConnectionData);
    if (EFI_ERROR (Status)) {
      MebxDisplayError (AmthiGetConnectionStatusApiError);
      return Status;
    }

    //
    // Get MPS Host Name from mConnectionData record and compare to previous string
    // If MPS Host Name has been changed, then update displayed info
    //
    ZeroMem (MpsHostName, sizeof (MpsHostName));
    AsciiStrToUnicodeStrS ((CHAR8 *) &ConnectionData->MpsHostname.Buffer, MpsHostName, MAX_STRING_LENGTH_FQDN);
    if ((CompareMem (MpsHostName, OldMpsHostName, sizeof (MpsHostName))) &&
        (ConnectionData->RemoteAccessConnectionStatus != RemoteAccessConnectionStatusConnected)) {
      MebxClearScreen ();
      AsciiPrint ("Connecting to %s...\n\r    Press <ESC> to abort \n\n\r", MpsHostName);
      ZeroMem (OldMpsHostName, sizeof (OldMpsHostName));
      CopyMem (OldMpsHostName, MpsHostName, sizeof (OldMpsHostName));
    }

    //
    // Poll the keystroke for 1 second
    //
    Timeout = KEYSTROKE_MSEC_DELAY;
    do {
      Status = MebxReadUserInput (&UserInput);
      if (EFI_ERROR (Status)) {
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while ((Timeout) && (UserInput == MebxUserInputNoInput));
  }

  if (UserInput == MebxUserInputEsc) {
    AsciiPrint ("Connection aborted\n\r");

    // Delay required to allow time to read the text before the screen is cleared.
    MicroSecondDelay (PT_CONSOLE_DELAY);

    Status = AmthiCloseUserInitiatedConnection ();
    if (EFI_ERROR (Status)) {
      MebxDisplayError (AmthiCloseUserInitiatedConnApiError);
    }

    return Status;
  }

  if (ConnectionData->RemoteAccessConnectionStatus == RemoteAccessConnectionStatusConnected) {
    AsciiPrint ("Connected to MPS Host %s\n\r", MpsHostName);
    AsciiPrint ("Press <ESCAPE> to close the connection and continue to boot\n\r");
    AsciiPrint ("Press <ENTER> to keep the connection and continue to boot\n\r");
  } else {
    AsciiPrint ("Could not connect to MPS Host %s\n\r", MpsHostName);
    AsciiPrint ("Press <ENTER> to continue to boot\n\r");
  }

  while (TRUE) {
    // Poll the keystroke for 1 second
    Timeout = KEYSTROKE_MSEC_DELAY;
    do {
      Status = MebxReadUserInput (&UserInput);
      if (EFI_ERROR (Status)) {
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while ((Timeout) && (UserInput == MebxUserInputNoInput));


    if (UserInput == MebxUserInputEsc &&
       (ConnectionData->RemoteAccessConnectionStatus == RemoteAccessConnectionStatusConnected)) {
      Status = AmthiCloseUserInitiatedConnection ();
      if (EFI_ERROR (Status)) {
        MebxDisplayError (AmthiCloseUserInitiatedConnApiError);
      }

      break;
    } else if (UserInput == MebxUserInputEnter) {
      break;
    }
  }

  return Status;
}

/**
  Checks whether proccess of establishing remote assistance connection should be continued.

  @retval TRUE             Continue to poll for connection
  @retval FALSE            Abort connection establishment
**/
BOOLEAN
ContinuePollingForConnection (
  VOID
  )
{
  EFI_STATUS      Status;
  static BOOLEAN  IsFirstNotReadyError = TRUE;
  UINT32          KeystrokeTimeout;
  MEBX_USER_INPUT UserInput;

  if (IsFirstNotReadyError) {
    AsciiPrint ("Intel(R) Remote Assistance mechanism is trying to get a network connection.\n\rThis may take a couple of minutes...\n\r");
    AsciiPrint ("Press <ESC> to abort");
    IsFirstNotReadyError = FALSE;
  }

  // Poll the keystroke for 1 second
  KeystrokeTimeout = KEYSTROKE_MSEC_DELAY;
  do {
    Status = MebxReadUserInput (&UserInput);
    if (EFI_ERROR (Status)) {
      break;
    }
    MicroSecondDelay (1000);
    KeystrokeTimeout--;
  } while (KeystrokeTimeout && (UserInput == MebxUserInputNoInput));

  if (UserInput == MebxUserInputEsc) {
    AsciiPrint ("\nConnection aborted - check your LAN cable and your WLAN connection\n\r");
    AsciiPrint ("Press <ENTER> to try again or <ESC> to abort and continue boot\n");
    while (TRUE) {
      Status = MebxReadUserInput (&UserInput);
      if (EFI_ERROR (Status)) {
        MebxDisplayError (AmthiGetConnectionStatusApiError);
        return FALSE;
      }
      if (UserInput == MebxUserInputEnter) {
        MebxClearScreen ();
        IsFirstNotReadyError = TRUE;
        return TRUE;
      } else if (UserInput == MebxUserInputEsc) {
        return FALSE;
      }
    }
  }
  AsciiPrint (".");
  return TRUE;
}


/**
  Perform all steps necessary for connection and execution of CIRA/CILA flow.

  @retval EFI_SUCCESS             Remote Assistance process completed
  @retval EFI_UNSUPPORTED         Remote Assistance unsupported for current AMT network connection
  @retval Others                  Other error has occured
**/
EFI_STATUS
ActivateRemoteAssistance (
  VOID
  )
{
  EFI_STATUS                      Status;
  UINT32                          OpenConnStatus;
  REMOTE_ACCESS_CONNECTION_STRUCT ConnectionData;

  OpenConnStatus = AmtStatusInternalError;

  while (TRUE) {
    AmthiOpenUserInitiatedConnection (&OpenConnStatus);
    switch (OpenConnStatus) {

      case AmtStatusSuccess:
      case AmtStatusDataMissing:
        break;

      case AmtStatusNotPermitted:
        AsciiPrint ("Network interface has been disabled - Intel(R) Remote Assistance is no longer available.\n\r");
        AsciiPrint ("Boot will now continue...");
        MicroSecondDelay (REMOTE_ASSISTANCE_ABORTED_DELAY);
        return EFI_SUCCESS;

      case AmtStatusNotReady:
        if (ContinuePollingForConnection ()) {
          continue;
        } else {
          MebxClearScreen ();
          return EFI_SUCCESS;
        }

      default:
        MebxDisplayError (AmthiGetConnectionStatusApiError);
        return EFI_DEVICE_ERROR;
    }
    //
    // Exit the Loop when AmtStatusSuccess or AmtStatusDataMissing
    //
    break;
  };

  MebxClearScreen ();

  Status = AmthiGetConnectionStatus (&ConnectionData);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetConnectionStatusApiError);
    return Status;
  }

  switch (ConnectionData.AmtNetworkConnectionStatus) {

    case AmtNetworkConnectionDirect:
      return PerformCilaFlow ();

    case AmtNetworkConnectionOutsideEnterprise:
      if (OpenConnStatus == AmtStatusDataMissing) {
        AsciiPrint ("Intel (R) Remote Assistance mechanism failed to connect due to missing remote\naccess configuration.\n\r");
        return EFI_UNSUPPORTED;
      }
      return PerformCiraFlow (&ConnectionData);

    case AmtNetworkConnectionOutsideUnknown:
    case AmtNetworkConnectionVpn:

    default:
      return EFI_UNSUPPORTED;
  }
}