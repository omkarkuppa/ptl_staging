/** @file
  This file contains code required for handling AMT Settings in MEBx.

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

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DxeMeLib.h>
#include <Library/DebugLib.h>
#include <Library/HiiLib.h>
#include <Library/DxeMebxWrapperLib.h>
#include <Library/DxeMebxDisplayWrapperLib.h>
#include <Library/BaseLib.h>
#include <Library/PrintLib.h>
#include <Library/DxeAsfLib.h>
#include <Library/MeUtilsLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <MebxSetup.h>
#include <MebxSetupVariable.h>
#include <Protocol/MebxDisplayProtocol.h>

#define WAIT_FOR_FW_FEATURE_DEFAULT_TIMEOUT 15     ///< timeout in seconds
#define PWD_POLICY_UNKNOWN                  3

extern EFI_HII_HANDLE mMebxHiiHandle;

#define FormatIpv4TextStr(OutputUnicodeBuffer, IpAddress) \
  UnicodeSPrint (OutputUnicodeBuffer, 0x40, L"%d.%d.%d.%d", (UINTN) ((IpAddress)->SegAddr[3]), \
  (UINTN) ((IpAddress)->SegAddr[2]), (UINTN) ((IpAddress)->SegAddr[1]), (UINTN) ((IpAddress)->SegAddr[0]))

/**
  This function retrieves current MEBx password change policy. If AMTHI API fails,
  it assigns a temporary value so that RVP Setup doesn't detect a change.

  @param[out] PwdPolicy              Current MEBx password change policy:
                                       @see CFG_MEBX_PWD_CHANGE_POLICY

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetMebxPasswordPolicy (
  OUT UINT32 *PwdPolicy
  )
{
  EFI_STATUS Status;

  if (PwdPolicy == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetMebxPwdPolicy (PwdPolicy);
  if (EFI_ERROR (Status)) {
    //
    // This AMTHI API only succeeds after User has successfully logged into MEBx.
    // Assign temporary value so that RVPs MEBx setup doesn't detect a "configuration change"
    //
    *PwdPolicy = PWD_POLICY_UNKNOWN;
  }

  return Status;
}

/**
  This function retrieves the current state of redirection (Sol/IDER/KVM) features (enabled/disabled).

  @param[out] IderState              IDER state
                                       0 - enabled
                                       1 - disabled
  @param[out] SolState               SOL state
                                       0 - enabled
                                       1 - disabled

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetRedirectionState (
  OUT UINT8 *IderState,
  OUT UINT8 *SolState
  )
{
  EFI_STATUS                 Status;
  REDIRECTION_FEATURES_STATE RedirectionFeaturesState;

  Status = MebxGetRedirectionState (&RedirectionFeaturesState);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *IderState = (UINT8) RedirectionFeaturesState.StorageRedirState;
  *SolState  = (UINT8) RedirectionFeaturesState.SolState;

  return Status;
}

/**
  Converts the low nibble of a byte  to hex unicode character.

  @param[in] Nibble      Lower nibble of a byte

  @retval CHAR16         Hex unicode character
**/
CHAR16
NibbleToHexChar (
  IN UINT8      Nibble
  )
{
  Nibble &= 0x0F;
  if (Nibble <= 0x9) {
    return (CHAR16) (Nibble + L'0');
  }

  return (CHAR16) (Nibble - 0xA + L'A');
}

/**
  Converts retrieved Hash to string adding a hyphen after every 2nd byte.

  @param[out] String     Unicode String
  @param[in]  Buffer     Hash in hex format
  @param[in]  Length     Length of hash to convert
**/
VOID
HashToString (
  OUT UINT16 *String,
  IN UINT8   *Buffer,
  IN UINTN   Length
  )
{
  UINTN   Index;
  UINT16  *OrigStr;
  UINT8   Byte;

  OrigStr = String;

  for (Index = 0; Index < Length; Index++) {
    if ((Index % 2 == 0) && (Index != 0)) {
      *String = '-';
      String++;
    }
    Byte = Buffer[Index];
    *String = NibbleToHexChar ((UINT8) (Byte >> 4));
    *(String + 1) = NibbleToHexChar (Byte);
    String += 2;
  }

  DEBUG ((DEBUG_INFO, "@%03d: %s\n", Index, OrigStr));
  *String = '\0';
}

/**
  The function returns a number of bytes for specified Hash Algorithm.

  @param[in] HashAlgorithm    Hash algorithm
                                @see CERT_HASH_ALGORITHM

  @retval    Number of bytes in hash
**/
UINT32
GetNumBytesInHash (
  IN CERT_HASH_ALGORITHM HashAlgorithm
  )
{
  switch (HashAlgorithm) {
    case CertHashAlgorithmSha1:
      return 20;
    case CertHashAlgorithmSha256:
      return 32;
    case CertHashAlgorithmSha384:
      return 48;
    case CertHashAlgorithmSha224:
      return 28;
    case CertHashAlgorithmSha512:
      return 64;
    default:
      return 0;
  }
}

/**
  This function updates dummy strings for Hashed Certificates in the Hii Data base
  with strings obtained from ME FW.

  @param[in] HashEntry                Index of the  Hashed Certificate
  @param[in] CertHashEntry            Structure containing Hashed Certificate info
                                        @see CERT_HASH_ENTRY
**/
VOID
UpdateHashedCertificatesStrings (
  IN UINT32           HashEntry,
  IN CERT_HASH_ENTRY  *CertHashEntry
  )
{
  CHAR16           TempUniString[CERT_NAME_MAX_ASCII_STRING];
  UINT32           NumBytesInHash;
  CHAR16           TempCertHash[HASHDATA_CHARACTERS];

  ZeroMem (TempUniString, sizeof (CHAR16) * CERT_NAME_MAX_ASCII_STRING);
  ZeroMem (TempCertHash, sizeof (CHAR16) * HASHDATA_CHARACTERS);

  AsciiStrToUnicodeStrS ((CHAR8 *) &CertHashEntry[HashEntry].CertName.Buffer, TempUniString, CERT_NAME_MAX_ASCII_STRING);
  HiiSetString (mMebxHiiHandle, STRING_TOKEN (STR_CERTIFICATE_NAME_0) + (EFI_STRING_ID)HashEntry, TempUniString, NULL);

  NumBytesInHash = GetNumBytesInHash ((CERT_HASH_ALGORITHM) CertHashEntry[HashEntry].HashAlgorithm);
  HashToString (TempCertHash, CertHashEntry[HashEntry].CertHash, NumBytesInHash);
  HiiSetString (mMebxHiiHandle, STRING_TOKEN (STR_CERTIFICATE_HASH_0) + (EFI_STRING_ID)HashEntry, TempCertHash, NULL);
}

/**
  This function retrieves the initial Hashed Certificates configuration and updates the strings
  in the Manage Certificates Submenu. Caller is responsible for allocating enough space in Setup Variable.

  @param[out] CertQuantity           Number of available Hashed Certificates entries
  @param[out] CertHandlesTable       Array with available Hashed Certificates list
  @param[out] CertDefaultStates      Array with available Hashed Certificates default state
  @param[out] CertActiveStates       Array with available Hashed Certificates active state
  @param[out] CertHashType           Array with available Hashed Certificates hashes type
                                       @see CERT_HASH_ALGORITHM

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetAvailableHashedCertificates (
  OUT UINT32 *CertQuantity,
  OUT UINT32 *CertHandlesTable,
  OUT UINT32 *CertDefaultStates,
  OUT UINT32 *CertActiveStates,
  OUT UINT8  *CertHashType
  )
{
  EFI_STATUS       Status;
  CERT_HASH_ENTRY  CertHashEntry[MAX_HASH_ENTRIES];
  UINT32           HashEntry;

  if (CertQuantity == NULL ||  CertHandlesTable == NULL || CertDefaultStates == NULL ||
      CertActiveStates == NULL || CertHashType == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetHashedCertificates (CertQuantity, CertHandlesTable , CertHashEntry);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[MEBx] CertQuantity %d\n", *CertQuantity));

  for (HashEntry = 0; HashEntry < *CertQuantity; HashEntry++) {
    CertDefaultStates[HashEntry] = CertHashEntry[HashEntry].CertDefault;
    CertActiveStates[HashEntry]  = CertHashEntry[HashEntry].CertActive;
    CertHashType[HashEntry]      = CertHashEntry[HashEntry].HashAlgorithm;
    UpdateHashedCertificatesStrings (HashEntry, CertHashEntry);
  }

  return Status;
}

/**
  This function retrieves the Configuration server settings and formats retrieved data to Unicode format.

  @param[out] PkiDnsSuffix           TCP port used for the provisioning service
  @param[out] ProvServerAddr         Formatted Provisioning server address

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetConfigServerData (
  OUT UINT16 *ProvServerPort,
  OUT CHAR16 *ProvServerAddr
  )
{
  EFI_STATUS       Status;
  FQDN_ANSI_STRING Fqdn;
  UINT8            ServerAddr[CFG_IPV6_ADDR_LEN_MAX];

  if (ProvServerPort == NULL || ProvServerAddr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetConfigServerData (
             ProvServerPort,
             ServerAddr,
             &Fqdn
             );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AsciiStrToUnicodeStrS ((CHAR8 *) &ServerAddr, ProvServerAddr, CFG_IPV6_ADDR_LEN_MAX);
  if (!StrCmp (ProvServerAddr, L"") ||
      !StrCmp (ProvServerAddr, L"0.0.0.0") ||
      !StrCmp (ProvServerAddr, L"::0")) {
    AsciiStrToUnicodeStrS ((CHAR8 *) &Fqdn.Buffer, ProvServerAddr, MAX_STRING_LENGTH_FQDN_SUFFIX);
  }

  return Status;
}

/**
  This function retrieves platform provisioning state and determines whether network
  access is enabled.

  @param[out] ProvisioningState      Current platform provisioning state:
                                       @see PT_PROVISIONING_STATE
  @param[out] NetAccess              Determines if platform has network access enabled

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetPlatformProvisioningState (
  OUT UINT32 *ProvisioningState,
  OUT UINT32 *NetAccess
  )
{
  EFI_STATUS Status;

  if (ProvisioningState == NULL || NetAccess == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  *NetAccess = 0;

  Status = MebxGetProvisioningState (ProvisioningState);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (*ProvisioningState != ProvisioningStatePost) {
    *NetAccess = 1;
  }

  return Status;
}

/**
  This function retrieves the PKI FQDN suffix and formats retrieved data to Unicode format.

  @param[out] PkiDnsSuffix           Formatted PKI FQDN suffix

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetPkiFqdnSuffix (
  OUT CHAR16 *PkiDnsSuffix
  )
{
  EFI_STATUS              Status;
  FQDN_SUFFIX_ANSI_STRING Fqdn;

  if (PkiDnsSuffix == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetPkiFqdnSuffix (&Fqdn);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  AsciiStrToUnicodeStrS ((CHAR8 *) &Fqdn.Buffer, PkiDnsSuffix, MAX_STRING_LENGTH_FQDN_SUFFIX);

  return Status;
}

/**
  This function retrieves the IPV4 configuration and updates the strings
  in the Wired LAN IPV4 Submenu.

  @param[out] IsLanlessPlatform      Indicates if Wired LAN is available
  @param[out] DhcpMode               Current DHCP Mode:
                                       @see DHCP_MODE
  @param[out] Ipv4Addr               Formatted IPV4 Address
  @param[out] Ipv4MaskAddr           Formatted IPV4 Mask Address
  @param[out] Ipv4GatewayAddr        Formatted IPV4 Gateway Address
  @param[out] Ipv4PrefDnsAddr        Formatted IPV4 Preferred DNS Address
  @param[out] Ipv4AltDnsAddr         Formatted IPV4 Alternate DNS Address

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetIpv4Settings (
  OUT UINT8  *IsLanlessPlatform,
  OUT UINT8  *DhcpMode,
  OUT CHAR16 *Ipv4Addr,
  OUT CHAR16 *Ipv4MaskAddr,
  OUT CHAR16 *Ipv4GatewayAddr,
  OUT CHAR16 *Ipv4PrefDnsAddr,
  OUT CHAR16 *Ipv4AltDnsAddr
  )
{
  EFI_STATUS       Status;
  IMB_TCPIP_PARAMS TcpIpParameters;

  if (IsLanlessPlatform == NULL ||  DhcpMode == NULL || Ipv4Addr == NULL ||
      Ipv4MaskAddr == NULL || Ipv4GatewayAddr == NULL || Ipv4PrefDnsAddr == NULL || Ipv4AltDnsAddr == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetIpv4Params (LanInterfaceWired, FALSE, IsLanlessPlatform, &TcpIpParameters);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *DhcpMode = (UINT8) TcpIpParameters.DhcpMode;

  if (TcpIpParameters.SubnetMask == 0) {
    TcpIpParameters.SubnetMask = 0xFFFFFFFE; //255.255.255.254
  }
  FormatIpv4TextStr (Ipv4Addr,        (IP_ADDR*) &TcpIpParameters.LocalAddr);
  FormatIpv4TextStr (Ipv4MaskAddr,    (IP_ADDR*) &TcpIpParameters.SubnetMask);
  FormatIpv4TextStr (Ipv4GatewayAddr, (IP_ADDR*) &TcpIpParameters.GatewayAddr);
  FormatIpv4TextStr (Ipv4PrefDnsAddr, (IP_ADDR*) &TcpIpParameters.PriDnsAddr);
  FormatIpv4TextStr (Ipv4AltDnsAddr,  (IP_ADDR*) &TcpIpParameters.SecDnsAddr);

  return Status;
}

/**
  This function retrieves the FQDN data and updates the FQDN string used by Intel AMT to maintain network
  presence.

  @param[out] FqdnType               Indicates if domain name is shared with the Host or dedicated to ME
                                       0 - dedicated
                                       1 - shared
  @param[out] HostDomainName         FQDN string used by Intel AMT to maintain network presence

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetFqdnData (
  OUT UINT8  *FqdnType,
  OUT CHAR16 *HostDomainName
  )
{
  EFI_STATUS Status;
  FQDN_DATA  FqdnData;

  Status = MebxGetFqdn (&FqdnData);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *FqdnType         = (UINT8) FqdnData.SharedFqdn;

  AsciiStrToUnicodeStrS ((CHAR8 *) &FqdnData.Fqdn.Buffer, HostDomainName, MAX_STRING_LENGTH_FQDN);

  return Status;
}

/**
  This function retrieves the provisioning mode and ME provisioning audit record (PKI provisioning only).
  It updates the strings in the Remote Setup And Configuration submenu.

  @param[out] ProvisioningTlsMode    Current provisioning TLS mode
  @param[out] SecureDns              Was secure DNS mode used
  @param[out] HostInitiated          Was the provisioning started by a local agent (Host initiated)
                                     or automatically (Bare metal)
  @param[out] SelectedHashType       Hash algorithm for the hash value
  @param[out] SelectedHashData       Hash of the root of the certificate that did the provisioning
  @param[out] CaCertificateSerials   Intermediate CAs serial numbers
  @param[out] AdditionalCaSerialNums If the certificate chain includes additional intermediate CAs not recorded
  @param[out] IsOemDefault           State of the IsDefault bit in the certificate hash table
  @param[out] IsTimeValid            Time validity
  @param[out] ProvRecordServerIp     The IP of the provisioning server
  @param[out] Date                   Date of provision
  @param[out] Time                   Time of provision
  @param[out] ProvRecordServerFqdn   FQDN of provisioning server mentioned in certificate

  @retval EFI_SUCCESS                Function succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetMeSetupAuditRecord  (
  OUT UINT8        *ProvisioningTlsMode,
  OUT UINT32       *SecureDns,
  OUT UINT32       *HostInitiated,
  OUT UINT32       *SelectedHashType,
  OUT UINT16       *SelectedHashData,
  OUT UINT16       *CaCertificateSerials,
  OUT UINT32       *AdditionalCaSerialNums,
  OUT UINT32       *IsOemDefault,
  OUT UINT32       *IsTimeValid,
  OUT CHAR16       *ProvRecordServerIp,
  OUT EFI_HII_DATE *Date,
  OUT EFI_HII_TIME *Time,
  OUT CHAR16       *ProvRecordServerFqdn
  )
{
  EFI_STATUS                  Status;
  ME_ADMIN_AUDIT_RECORD       AuditRecord;
  UINT32                      NumBytesInHash;

  if (ProvisioningTlsMode == NULL || SecureDns == NULL || HostInitiated == NULL || SelectedHashType == NULL ||
      SelectedHashData == NULL || CaCertificateSerials == NULL || AdditionalCaSerialNums == NULL ||
      IsOemDefault == NULL || IsTimeValid == NULL || ProvRecordServerIp == NULL ||
      Date == NULL || Time == NULL || ProvRecordServerFqdn == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetMeSetupAuditRecord (&AuditRecord);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  *ProvisioningTlsMode = AuditRecord.ProvisioningTlsMode;
  *SecureDns        = MIN (AuditRecord.SecureDns, 2);
  *HostInitiated    = MIN (AuditRecord.HostInitiated, 2);
  *SelectedHashType = AuditRecord.SelectedHashType;

  NumBytesInHash = GetNumBytesInHash ((CERT_HASH_ALGORITHM) AuditRecord.SelectedHashType);

  HashToString (SelectedHashData, AuditRecord.SelectedHashData, NumBytesInHash);
  HashToString (CaCertificateSerials, AuditRecord.CaCertificateSerials, sizeof (AuditRecord.CaCertificateSerials));
  *AdditionalCaSerialNums = MIN (AuditRecord.AdditionalCaSerialNums, 2);
  *IsOemDefault           = MIN (AuditRecord.IsOemDefault, 2);
  *IsTimeValid            = MIN (AuditRecord.IsTimeValid, 2);
  AsciiStrToUnicodeStrS ((CHAR8 *) &AuditRecord.ProvServerIp.Address, ProvRecordServerIp, sizeof (AuditRecord.ProvServerIp.Address));

  Date->Year   = AuditRecord.TlsStartTime.Year;
  Date->Month  = (UINT8) AuditRecord.TlsStartTime.Month;
  Date->Day    = (UINT8) AuditRecord.TlsStartTime.Day;
  Time->Hour   = (UINT8) AuditRecord.TlsStartTime.Hour;
  Time->Minute = (UINT8) AuditRecord.TlsStartTime.Minute;
  Time->Second = (UINT8) AuditRecord.TlsStartTime.Second;
  AsciiStrToUnicodeStrS ((CHAR8 *) &AuditRecord.ProvServerFqdn.Buffer, ProvRecordServerFqdn, sizeof (AuditRecord.ProvServerFqdn.Buffer));

  return Status;
}

/**
  This function updates dummy strings for ME Power policy in the Hii Data base
  with strings obtained from ME FW.

  @param[in] NumberOfPowerPkgs        Number of Power Packages on given platform
  @param[in] *PackageDescription      Pointer to array of Power Package descriptions in Ascii

  @retval EFI_SUCCESS                 Hii database updated successfully
  @retval EFI_INVALID_PARAMETER       NULL parameter
**/
EFI_STATUS
UpdatePowerPkgStrings (
  IN UINT8 PackageNumber,
  IN CHAR8 *PackageDescription
  )
{
  CHAR16               TempEfiPkgDesc[MAX_PWR_PKG_DESC_SIZE];
  UINTN                ConvLength;
  static EFI_STRING_ID PowerPkgStringArray[] = {
    STRING_TOKEN(STR_SELECTION_1_MEONHOSTSLPSTATES),
    STRING_TOKEN(STR_SELECTION_2_MEONHOSTSLPSTATES),
    STRING_TOKEN(STR_SELECTION_3_MEONHOSTSLPSTATES),
  };

  if (PackageDescription == NULL) {
   return EFI_INVALID_PARAMETER;
  }

  AsciiStrnToUnicodeStrS (PackageDescription, MAX_PWR_PKG_DESC_SIZE - 1, TempEfiPkgDesc, MAX_PWR_PKG_DESC_SIZE, &ConvLength);
  //
  // Update string in Hii Database
  //
  HiiSetString (mMebxHiiHandle, PowerPkgStringArray[PackageNumber], TempEfiPkgDesc, NULL);

  return EFI_SUCCESS;
}

/**
  This function retrieves the initial power control configuration and updates the strings
  in the Power Control Submenu.

  @param[out] NumberOfPowerPkgs       Number of available power packages GUIDs
  @param[out] CurrentMeOnHostSlpState Index of the currently used power package GUID

  @retval EFI_SUCCESS                 Function succeeded
  @retval EFI_INVALID_PARAMETER       NULL parameter
  @retval EFI_UNSUPPORTED             Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR            HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                 HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL        Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetPowerControlSettings (
  OUT UINT8           *NumberOfPowerPkgs,
  OUT UINT8           *CurrentMeOnHostSlpState
  )
{
  EFI_STATUS      Status;
  UINT8           PackageNumber;
  IMB_ANSI_STRING PackageDescription[MAX_PWR_PKG_COUNT];

  if (NumberOfPowerPkgs == NULL || CurrentMeOnHostSlpState == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = MebxGetPowerControlSettings (NumberOfPowerPkgs, CurrentMeOnHostSlpState, PackageDescription);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (PackageNumber = 0; PackageNumber < *NumberOfPowerPkgs; PackageNumber++) {
    Status = UpdatePowerPkgStrings (PackageNumber, (CHAR8 *)PackageDescription[PackageNumber].Buffer);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return Status;
}

/**
  Checks if remote administrator has access to system.

  @retval TRUE       Remote session is active
  @retval FALSE      Remote session is not active
**/
BOOLEAN
IsRedirectionActive (
  )
{
  if (AsfIsStorageRedirectionEnabled () || AsfIsSolEnabled () || AsfIsKvmEnabled ()) {
    return TRUE;
  }
  return FALSE;
}

/**
  Populate Mebx Setup Variable structure for UI display.

  @retval     EFI_SUCCESS         AMT Data updated successfully
  @retval     Others              An error has occurred
**/
EFI_STATUS
MebxGetAmtData (
  IN OUT MEBX_CONFIGURATION  *MebxConfiguration
  )
{
  EFI_STATUS                  Status;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  if (!MebxIsAmthiInterfaceVersionSupported ()) {
    MebxConfiguration->GlobalExposure = SHOW_UNSUPPORTED_INTERFACE;
    MebxDisplayError (AmthiGetAmthiInterfaceVersionApiError);
    return EFI_INCOMPATIBLE_VERSION;
  }

  GetMebxPasswordPolicy (&MebxConfiguration->PwdPolicy);

  Status = GetRedirectionState (&MebxConfiguration->StorageRedirEnable, &MebxConfiguration->SolEnable);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetPolicyAmtRedirectionStateApiError);
    return Status;
  }

  if (MebxConfiguration->KvmExposure) {
    Status = MebxGetKvmEnabled (&MebxConfiguration->KvmEnable);
    if (EFI_ERROR (Status)) {
      MebxDisplayError (AmthiGetKvmStateApiError);
      return Status;
    }
  }

  Status = GetAvailableHashedCertificates (
             &MebxConfiguration->CertQuantity,
             MebxConfiguration->CertHandle,
             MebxConfiguration->CertDefault,
             MebxConfiguration->CertActive,
             MebxConfiguration->CertHashType
             );
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetHashDataApiError);
    return Status;
  }

  Status = MebxGetPlatformOptInPolicy (
             &MebxConfiguration->OptInConfigExposure,
             &MebxConfiguration->RemoteItOptInConfigExposure,
             &MebxConfiguration->UserOptIn,
             &MebxConfiguration->RemoteItOptInConfig
             );

  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetOptinStateApiError);
    return Status;
  }

  Status = GetConfigServerData (&MebxConfiguration->ProvServerPort, MebxConfiguration->ProvServerAddr);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetConfigSvrDataApiError);
    return Status;
  }

  Status = GetPlatformProvisioningState (&MebxConfiguration->ProvisioningState, &MebxConfiguration->NetAccess);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetProvisionStateApiError);
    return Status;
  }

  Status = MebxGetZtcEnabledStatus (&MebxConfiguration->TlsPpkRemoteConfig);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetZtcConfigApiError);
    return Status;
  }

  Status = GetPkiFqdnSuffix (MebxConfiguration->PkiDnsSuffix);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetPkiFqdnSuffixApiError);
    return Status;
  }

  Status = GetIpv4Settings (
             &MebxConfiguration->LanLessPlatform,
             &MebxConfiguration->DhcpMode,
             MebxConfiguration->Ipv4Addr,
             MebxConfiguration->Ipv4MaskAddr,
             MebxConfiguration->Ipv4GatewayAddr,
             MebxConfiguration->Ipv4PrefDnsAddr,
             MebxConfiguration->Ipv4AltDnsAddr
             );
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetIpv4ParamsApiError);
    return Status;
  }

  Status = GetFqdnData (
             &MebxConfiguration->FqdnType,
             MebxConfiguration->HostDomainName
             );
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetFqdnApiError);
    return Status;
  }

  Status = MebxGetIdleTimeout (&MebxConfiguration->IdleTimeout);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetIdleTimeoutApiError);
    return Status;
  }

  Status = GetMeSetupAuditRecord (
             &MebxConfiguration->ProvTlsMode,
             &MebxConfiguration->SecureDns,
             &MebxConfiguration->HostInitiated,
             &MebxConfiguration->SelectedHashType,
             MebxConfiguration->SelectedHashData,
             MebxConfiguration->CaCertificateSerials,
             &MebxConfiguration->AdditionalCaSerialNums,
             &MebxConfiguration->IsOemDefault,
             &MebxConfiguration->IsTimeValid,
             MebxConfiguration->ProvRecordServerIp,
             &MebxConfiguration->Date,
             &MebxConfiguration->Time,
             MebxConfiguration->ProvRecordServerFqdn
             );
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetAuditRecordApiError);
    return Status;
  }

  Status = MebxGetAmtPrivacyLevel (&MebxConfiguration->PrivacyLevel);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetPrivacyLevelApiError);
    return Status;
  }

  Status = GetPowerControlSettings (&MebxConfiguration->NumberOfPowerPkgs, &MebxConfiguration->MeOnHostSlpStates);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (AmthiGetPowerPolicyApiError);
    return Status;
  }

  return Status;
}

/**
  Retrieve initial MEBx Menu Exposure.

  @param[out] GlobalExposure         Main menu exposure

  @retval EFI_SUCCESS                User is allowed to enter MEBx
  @retval EFI_ACCESS_DENIED          User is not allowed to enter MEBx
  @retval Others                     An error has occurred - show login screen
**/
EFI_STATUS
GetMebxMenuExposure (
  OUT UINT32 *GlobalExposure
  )
{
  EFI_STATUS                Status;
  CFG_EOP_STATE             EopState;
  UINT32                    PasswordModified;

  PasswordModified = 0;
  Status = WaitFirmwareInitComplete ();
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "FW Init Complete bit Error\n"));
    *GlobalExposure = SHOW_TOO_EARLY;
    return EFI_ACCESS_DENIED;
  }

  if (IsRedirectionActive ()) {
    *GlobalExposure = SHOW_REDIR_ACTIVE;
    return EFI_ACCESS_DENIED;
  }

  Status = MebxGetEndOfPostState (&EopState);
  if (EopState == EopStatusPost) {
    *GlobalExposure = SHOW_AFTER_EOP;
    return EFI_ACCESS_DENIED;
  } else if (EopState == EopStatusMebx) {
    Status = HeciIsMebxPasswordModified (&PasswordModified);
    if (PasswordModified) {
      *GlobalExposure = SHOW_EVERYTHING;
    } else {
      *GlobalExposure = SHOW_LOGIN;
    }
  } else {
    *GlobalExposure = SHOW_LOGIN;
  }

  return Status;
}

/**
  Wait for the availability bits of FW features defined in the input BitMask.

  The function checks if the specified bits of FW features are set until those bits are set
  or the determined timeout expires. This function checks FW features every 1/4 second.

  @param[in] BitMask            Bitmask of FW features meant to be available
  @param[in] Timeout            Timeout in seconds

  @retval EFI_SUCCESS           FW features defined in bitmask available
  @retval EFI_TIMEOUT           FW features defined in bitmask unavailable
**/
EFI_STATUS
MeWaitFwFeatureAvailable (
  IN UINT32 BitMask,
  IN UINT32 Timeout
  )
{
  EFI_STATUS   Status;
  MEFWCAPS_SKU FwFeatures;

  FwFeatures.Data = 0;

  Status = HeciGetFwFeatureStateMsg (&FwFeatures);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (FwFeatures.Data & BitMask) {
    for (Timeout *= 4; Timeout > 0; Timeout--) {
      Status = HeciGetFwAvailableFeatures (&FwFeatures.Data);
      if (EFI_ERROR (Status)) {
        return Status;
      }

      if (FwFeatures.Data & BitMask) {
        return EFI_SUCCESS;
      }

      // wait 250 miliseconds
      MicroSecondDelay (250000);
    }

    if (Timeout == 0 && !(FwFeatures.Data & BitMask)) {
      return EFI_TIMEOUT;
    }
  }

  return Status;
}

/**
  Retrieve current AMT Config.

  @param[out] AmtState               Current AMT State:
                                       @see AMT_STATE
  @param[out] AmtSubmenuExposure     Determines whether AMT Submenu should be exposed to user
  @param[out] KvmExposure            Determines whether platform supports KVM and KVM State should be
                                     exposed to user

  @retval EFI_SUCCESS                Command succeeded
  @retval EFI_INVALID_PARAMETER      NULL parameter
  @retval EFI_UNSUPPORTED            Current ME mode doesn't support this function
  @retval EFI_DEVICE_ERROR           HECI Device error, command aborts abnormally
  @retval EFI_TIMEOUT                HECI does not return the buffer before timeout
  @retval EFI_BUFFER_TOO_SMALL       Message Buffer is too small for the Acknowledge
**/
EFI_STATUS
GetAmtConfig (
  OUT UINT8    *AmtState,
  OUT UINT32   *AmtSubmenuExposure,
  OUT UINT32   *KvmExposure
  )
{
  EFI_STATUS   Status;
  MEFWCAPS_SKU UserCapabilities;
  MEFWCAPS_SKU FwCaps;
  MEFWCAPS_SKU FwFeatures;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Status = HeciGetUserCapabilitiesState (&UserCapabilities);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (MeGetUserCapsApiError);
    return Status;
  }

  Status = HeciGetFwCapsSkuMsg (&FwCaps);
  if (EFI_ERROR (Status)) {
    MebxDisplayError (MeGetFwCapsApiError);
    return Status;
  }

  *KvmExposure = !!FwCaps.Fields.KVM;

  Status = HeciGetFwFeatureStateMsg (&FwFeatures);
  DEBUG ((DEBUG_INFO, "CSME Enabled features returns %r SKU mask = 0x%x\n", Status, FwFeatures.Data));
  if (EFI_ERROR (Status)) {
    MebxDisplayError (MeGetFwEnabledFeatureApiError);
    return Status;
  }

  if (FwFeatures.Fields.Amt) {
    Status = MeWaitFwFeatureAvailable (AMT_BITMASK, WAIT_FOR_FW_FEATURE_DEFAULT_TIMEOUT);
    DEBUG ((DEBUG_INFO, "MeWaitFwFeatureAvailable returns %r\n", Status));
    if (EFI_ERROR (Status)) {
      MebxDisplayError (MeMeWaitFwFeatureAvailableApiError);
      return Status;
    }
  }

  //
  // Assign appropriate AMT state based on UserCapabilitesState (Rule Id = 9)
  // FwFeaturesState (RuleId = 32) rules.
  //
  // -------------------------------------------------------|
  // AmtDisabled          - Amt disabled in UserCapabilites |
  // -------------------------------------------------------|
  // AmtPartiallyDisabled - Amt enabled  in UserCapabilites |
  //                        and disabled in FwFeatures      |
  // -------------------------------------------------------|
  // AmtEnabled           - Amt enabled  in UserCapabilites |
  //                        and enabled  in FwFeatures      |
  //--------------------------------------------------------|
  if (UserCapabilities.Fields.Amt) {
    if (FwFeatures.Fields.Amt) {
      *AmtState = AmtEnabled;
      *AmtSubmenuExposure = TRUE;
    } else {
      *AmtState = AmtPartiallyDisabled;
    }
  } else {
    *AmtState = AmtDisabled;
  }

  return EFI_SUCCESS;
}

/**
  Retrieve initial MEBx configuration from Firmware.

  @param[out] MEBxConfiguration      Initial MEBx Configuration

  @retval EFI_SUCCESS                Initial MEBx Configuration retrieved
  @retval Others                     Failed to retrieve initial MEBx Configuration
**/
EFI_STATUS
MebxGetInfo (
  OUT MEBX_CONFIGURATION *MebxConfiguration
  )
{
  EFI_STATUS             Status;

  MebxConfiguration->GlobalExposure     = SHOW_LOGIN;
  MebxConfiguration->AmtSubmenuExposure = FALSE;
  MebxConfiguration->AmtStateExposure   = TRUE;
  MebxConfiguration->AmtState           = AmtDisabled;

  DEBUG ((DEBUG_INFO, "%a start\n", __FUNCTION__));

  Status = GetMebxMenuExposure (&MebxConfiguration->GlobalExposure);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GetAmtConfig (&MebxConfiguration->AmtState, &MebxConfiguration->AmtSubmenuExposure, &MebxConfiguration->KvmExposure);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  if (MebxConfiguration->AmtState == AmtEnabled) {
    Status = MebxGetAmtData (MebxConfiguration);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }

  return Status;
}
