/** @file
  Implementation file for Alert Standard Format library functionality
  This library depends on Alert Standard Format Protocol.
  Caller should check drivers' dependency.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/HobLib.h>
#include <Library/DebugLib.h>
#include <Protocol/AsfProtocol.h>
#include <AmtConfig.h>

//
// Function implementations
//

/**
  Get ASF boot options

  @retval NULL              Locate ASF Protocol failed.
  @retval AsfBootOptions    ASF boot options.
**/
STATIC
ASF_BOOT_OPTIONS *
GetAsfBootOption (
  VOID
  )
{
  EFI_STATUS                          Status;
  ALERT_STANDARD_FORMAT_PROTOCOL      *AsfProtocol;

  Status = gBS->LocateProtocol (
                  &gAlertStandardFormatProtocolGuid,
                  NULL,
                  (VOID **) &AsfProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a() Locate ASF Protocol failed, Status = %r\n", __FUNCTION__, Status));
    return NULL;
  }

  return &AsfProtocol->AsfBootOptions;
}

/**
  Check if ASF boot options is present.

  @retval True                    ASF boot option is present.
  @retval False                   ASF boot option is not present
**/
BOOLEAN
AsfIsBootOptionsPresent (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return FALSE;
  }

  return (AsfBootOptions->SubCommand == ASF_BOOT_OPTIONS_PRESENT);
}

/**
  Check if LANA ID of ASF boot options is Industry ID.

  @retval True                    IANA ID of ASF boot options is Industry ID.
  @retval False                   IANA ID of ASF boot options is not Industry ID.
**/
BOOLEAN
AsfIsIndustryIanaId (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return FALSE;
  }

  if (AsfIsBootOptionsPresent () && (AsfBootOptions->IanaId == ASF_INDUSTRY_CONVERTED_IANA)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check if LANA ID of ASF boot options is Intel ID.

  @retval True                    IANA ID of ASF boot options is Intel ID.
  @retval False                   IANA ID of ASF boot options is not Intel ID.
**/
BOOLEAN
IsIntelIanaId (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return FALSE;
  }

  if (AsfIsBootOptionsPresent () && (AsfBootOptions->IanaId == ASF_INTEL_CONVERTED_IANA)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check if it is Intel ASF boot options.

  @retval True                    It is Intel ASF boot options.
  @retval False                   It is not Intel ASF boot options.
**/
BOOLEAN
IsIntelAmtBootOptions (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return FALSE;
  }

  if (IsIntelIanaId () && (AsfBootOptions->SpecialCommand == ASF_INTEL_OEM_CMD)) {
    return TRUE;
  }

  return FALSE;
}

///
/// Special Command Parameters
///

/**
  Check the Special Command Parameter Low Byte of Intel ASF boot options

  @param[in] Options              Special Command Parameter bit we want to check
                                  Bit 0: Set if Storage Redirection is to be used on the next boot. Parameter 2 is set
                                  to the driver number to be used.
                                  Bit 1: Set if Secure Boot is enforced over Storage Redirection
                                  Bit 2: Set if the BIOS is to be re-flashed on the next boot
                                  Bit 3: Set if the BIOS is to boot into the BIOS set-up screen.
                                  Bit 4: Boot into BIOS Pause on the next boot is supported
                                  Bit 5: Set if the BIOS is to participate in KVM session
                                  Bit 6: Set if the BIOS is to start a Remote Secure Erase (RSE) session @Deprecated.
  @retval True                    Special Command Parameter bit in Options is enabled.
  @retval False                   Special Command Parameter bit in Options is disabled.
**/
STATIC
BOOLEAN
GetSpecialParamLowByteState (
  IN UINT8       Options
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return FALSE;
  }

  if (IsIntelAmtBootOptions () && ((AsfBootOptions->SpecialCommandParamLowByte & Options) == Options)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check Intel ASF Boot Options Mask

  @param[in] Options              Boot Options Mask bit want to check
  @param[in] Index                Indicate Boot Options Mask is Byte1 or Byte2

  @retval True                    Boot Options Mask bit is enabled.
  @retval False                   Boot Options Mask bit is disabled.
**/
STATIC
BOOLEAN
GetBootOptionsMaskState (
  IN UINT8          Options,
  IN OPTION_INDEX   Index
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if ((AsfBootOptions == NULL) || !AsfIsBootOptionsPresent ()) {
    return FALSE;
  }

  if ((Index == Byte1) && ((AsfBootOptions->BootOptionsMaskByte1 & Options) == Options)) {
    return TRUE;
  } else if ((Index == Byte2) && ((AsfBootOptions->BootOptionsMaskByte2 & Options) == Options)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Check the OEM Parameter of Intel ASF boot options

  @param[in] Options              OEM Parameter bit we want to check
                                  Bit 0: Set if SOL is to be used on the next boot.
  @param[in] Index                Indicate OEM Parameter is Byte1 or Byte2

  @retval True                    OEM Parameter bit in Options is enabled.
  @retval False                   OEM Parameter bit in Options is disabled.
**/
STATIC
BOOLEAN
GetOemParamatersState (
  IN UINT8          Options,
  IN OPTION_INDEX   Index
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if ((AsfBootOptions == NULL) || !IsIntelAmtBootOptions ()) {
    return FALSE;
  }

  if ((Index == Byte1) && ((AsfBootOptions->OemParameterByte1 & Options) == Options)) {
    return TRUE;
  } else if ((Index == Byte2) && ((AsfBootOptions->OemParameterByte2 & Options) == Options)) {
    return TRUE;
  }

  return FALSE;
}

/**
  Get Special Commands of Intel ASF boot options

  @retval UINT16            Special Commands.
**/
UINT8
AsfGetSpecialCommand (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return 0;
  }

  return AsfBootOptions->SpecialCommand;
}

/**
  Get Parameters of Special Commands High Byte in Intel ASF boot options

  @retval UINT8             Parameters of Special Commands High Byte.
**/
UINT8
AsfGetSpecialCmdParamHighByte (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return 0;
  }

  return AsfBootOptions->SpecialCommandParamHighByte;
}

/**
  Get Parameters of Special Commands Low Byte in Intel ASF boot options

  @retval UINT8             Parameters of Special Commands Low Byte.
**/
UINT8
AsfGetSpecialCmdParamLowByte (
  VOID
  )
{
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    return 0;
  }

  return AsfBootOptions->SpecialCommandParamLowByte;
}

/**
  This will return Storage Redirection Boot Option.
  True if the option is enabled.

  @retval True                    Storage Redirection boot option is enabled.
  @retval False                   Storage Redirection boot option is disabled.
**/
BOOLEAN
AsfIsStorageRedirectionEnabled (
  VOID
  )
{
  return GetSpecialParamLowByteState (USE_STORAGE_REDIR);
}

/**
  This will return Storage Redirection boot device number/type

  @retval            Return the boot device number to boot
                     Bit 1  Bit0
                      0    0    USBr is connected to CD/DVD device
                      0    1    USBr is connected to floppy device
                     Bits 2-7: Reserved set to 0
**/
UINT8
AsfGetStorageRedirectionBootDevice (
  VOID
  )
{
  UINT8               StorageRedirBootDevice;
  ASF_BOOT_OPTIONS    *AsfBootOptions;

  AsfBootOptions = GetAsfBootOption ();
  if (AsfBootOptions == NULL) {
    StorageRedirBootDevice = 0;
  } else {
    StorageRedirBootDevice = AsfBootOptions->SpecialCommandParamHighByte & STORAGE_REDIRECTION_BOOT_DEVICE_MASK;
  }
  return StorageRedirBootDevice;
}

/**
  This will return Enforce Secure Boot over Storage Redirection Boot Option.
  True if the option is enabled.

  @retval True                    Enforce Secure Boot is enabled.
  @retval False                   Enforce Secure Boot is disabled.
**/
BOOLEAN
AsfIsEnforceSecureBootEnabled (
  VOID
  )
{
  return GetSpecialParamLowByteState (ENFORCE_SECURE_BOOT);
}

/**
  This will return Remote Flash Boot Option.
  True if the option is enabled.

  @retval True                    Remote Flash is enabled.
  @retval False                   Remote Flash is disabled.
**/
BOOLEAN
AsfGetRemoteFlashState (
  VOID
  )
{
  return GetSpecialParamLowByteState (REFLASH_BIOS);
}


/**
  This will return BIOS Setup Boot Option.
  True if the option is enabled.

  @retval True                    BIOS Setup is enabled.
  @retval False                   BIOS Setup is disabled.
**/
BOOLEAN
AsfIsEnterSetupEnabled (
  VOID
  )
{
  return GetSpecialParamLowByteState (BIOS_SETUP);
}

/**
  This will return BIOS Pause Boot Option.
  True if the option is enabled.

  @retval True                    BIOS Pause is enabled.
  @retval False                   BIOS Pause is disabled.
**/
BOOLEAN
AsfIsPauseBootEnabled (
  VOID
  )
{
  return GetSpecialParamLowByteState (BIOS_PAUSE);
}

/**
  This will return KVM Boot Option.
  True if the option is enabled.

  @retval True                    KVM is enabled.
  @retval False                   KVM is disabled.
**/
BOOLEAN
AsfIsKvmEnabled (
  VOID
  )
{
  return GetSpecialParamLowByteState (USE_KVM);
}

/**
  This will return Serial-over-Lan Boot Option.
  True if the option is enabled.

  @retval True                    Serial-over-Lan is enabled.
  @retval False                   Serial-over-Lan is disabled.
**/
BOOLEAN
AsfIsSolEnabled (
  VOID
  )
{
  EFI_HOB_GUID_TYPE       *GuidHob;
  AMT_PEI_CONFIG          *AmtPeiConfig;

  GuidHob = GetFirstGuidHob (&gAmtPolicyHobGuid);
  if (GuidHob == NULL) {
    return FALSE;
  }

  AmtPeiConfig = (AMT_PEI_CONFIG*) GET_GUID_HOB_DATA (GuidHob);
  if ((AmtPeiConfig != NULL) && (AmtPeiConfig->AmtSolEnabled == 1)) {
    return GetOemParamatersState (USE_SOL, Byte1);
  }

  return FALSE;
}

/**
  This will return Console Lock Boot Option.
  True if the option is enabled.

  @retval True                    Console Lock is enabled.
  @retval False                   Console Lock is disabled.
**/
BOOLEAN
AsfIsConsoleLocked (
  VOID
  )
{
  return GetBootOptionsMaskState (LOCK_KEYBOARD, Byte1);
}

/**
  This will return Progress Event Option.
  True if the option is enabled.

  @retval True                    Progress Event is enabled.
  @retval False                   Progress Event is disabled.
**/
BOOLEAN
AsfIsFwProgressEnabled (
  VOID
  )
{
  return GetBootOptionsMaskState (FORCE_PROGRESS_EVENTS, Byte2);
}

/**
  This will return Remote Platform Erase Boot Option.

  @retval True                    Remote Platform Erase is enabled.
  @retval False                   Remote Platform Erase is disabled.
**/
BOOLEAN
AsfIsRemotePlatformEraseEnabled (
  VOID
  )
{
  if (IsIntelAmtBootOptions ()) {
    return GetSpecialParamLowByteState (ENABLE_REMOTE_PLATFORM_ERASE);
  } else {
    return FALSE;
  }
}

/**
  This will return Configuration data reset Boot Option.
  True if the option is enabled.

  @retval True                    Configuration data reset is enabled.
  @retval False                   Configuration data reset is disabled.
**/
BOOLEAN
AsfIsConfigurationDataResetEnabled (
  VOID
  )
{
  return GetBootOptionsMaskState (CONFIG_DATA_RESET, Byte2);
}

