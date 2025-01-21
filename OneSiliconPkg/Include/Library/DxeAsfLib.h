/** @file
  Header file for ASF functionality

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
#ifndef _DXE_ASF_LIB_H_
#define _DXE_ASF_LIB_H_

//
// Prototype for Alert Standard Format
//

/**
  Check if LANA ID of ASF boot options is Industry ID.

  @retval True                    IANA ID of ASF boot options is Industry ID.
  @retval False                   IANA ID of ASF boot options is not Industry ID.
**/
BOOLEAN
AsfIsIndustryIanaId (
  VOID
  );

/**
  Get Special Commands of Intel ASF boot options

  @retval UINT8             Special Commands.
**/
UINT8
AsfGetSpecialCommand (
  VOID
  );

/**
  Get Parameters of Special Commands High Byte in Intel ASF boot options

  @retval UINT8             Parameters of Special Commands High Byte.
**/
UINT8
AsfGetSpecialCmdParamHighByte (
  VOID
  );

/**
  Get Parameters of Special Commands Low Byte in Intel ASF boot options

  @retval UINT8             Parameters of Special Commands Low Byte.
**/
UINT8
AsfGetSpecialCmdParamLowByte (
  VOID
  );

/**
  This will return Storage Redirection Boot Option.
  True if the option is enabled.

  @retval True                    Storage Redirection boot option is enabled.
  @retval False                   Storage Redirection boot option is disabled.
**/
BOOLEAN
AsfIsStorageRedirectionEnabled (
  VOID
  );

/**
  This will return Storage Redirection boot device number/type

  @retval UINT8      Return the boot device number to boot
                                  Bit 1  Bit0
                                   0    0    USBr is connected to CD/DVD device
                                   0    1    USBr is connected to floppy device
                                  Bits 2-7: Reserved set to 0
**/
UINT8
AsfGetStorageRedirectionBootDevice (
  VOID
  );

/**
  This will return Enforce Secure Boot over Storage Redirection Boot Option.
  True if the option is enabled.

  @retval True                    Enforce Secure Boot is enabled.
  @retval False                   Enforce Secure Boot is disabled.
**/
BOOLEAN
AsfIsEnforceSecureBootEnabled (
  VOID
  );

/**
  This will return Remote Flash Boot Option.
  True if the option is enabled.

  @retval True                    Remote Flash is enabled.
  @retval False                   Remote Flash is disabled.
**/
BOOLEAN
AsfGetRemoteFlashState (
  VOID
  );

/**
  This will return BIOS Setup Boot Option.
  True if the option is enabled.

  @retval True                    BIOS Setup is enabled.
  @retval False                   BIOS Setup is disabled.
**/
BOOLEAN
AsfIsEnterSetupEnabled (
  VOID
  );

/**
  This will return BIOS Pause Boot Option.
  True if the option is enabled.

  @retval True                    BIOS Pause is enabled.
  @retval False                   BIOS Pause is disabled.
**/
BOOLEAN
AsfIsPauseBootEnabled (
  VOID
  );

/**
  This will return KVM Boot Option.
  True if the option is enabled.

  @retval True                    KVM is enabled.
  @retval False                   KVM is disabled.
**/
BOOLEAN
AsfIsKvmEnabled (
  VOID
  );

/**
  This will return Serial-over-Lan Boot Option.
  True if the option is enabled.

  @retval True                    Serial-over-Lan is enabled.
  @retval False                   Serial-over-Lan is disabled.
**/
BOOLEAN
AsfIsSolEnabled (
  VOID
  );

/**
  This will return Console Lock Boot Option.
  True if the option is enabled.

  @retval True                    Console Lock is enabled.
  @retval False                   Console Lock is disabled.
**/
BOOLEAN
AsfIsConsoleLocked (
  VOID
  );

/**
  This will return Progress Event Option.
  True if the option is enabled.

  @retval True                    Progress event is enabled.
  @retval False                   Progress event is disabled.
**/
BOOLEAN
AsfIsFwProgressEnabled (
  VOID
  );

/**
  Check if ASF boot options is present.

  @retval True                    ASF boot option is present.
  @retval False                   ASF boot option is not present
**/
BOOLEAN
AsfIsBootOptionsPresent (
  VOID
  );

/**
  This will return Remote Platform Erase Boot Option.

  @retval True                    Remote Platform Erase is enabled.
  @retval False                   Remote Platform Erase is disabled.
**/
BOOLEAN
AsfIsRemotePlatformEraseEnabled (
  VOID
  );

/**
  This will return Configuration data reset.
  True if the option is enabled.

  @retval True                    Configuration data reset is enabled.
  @retval False                   Configuration data reset is disabled.
**/
BOOLEAN
AsfIsConfigurationDataResetEnabled (
  VOID
  );

#endif
