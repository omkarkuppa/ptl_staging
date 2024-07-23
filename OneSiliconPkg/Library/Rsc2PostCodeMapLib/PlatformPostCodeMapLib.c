/** @file
  Platform Rsc2PostCodeMap Library implementation.

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

@par Specification
**/

#include <Base.h>
#include <Uefi.h>

#include <Pi/PiStatusCode.h>
#include <IntelRcStatusCode.h>

STATUS_CODE_TO_POST_CODE_MAP mPlatformPostCodeProgressMap[] = {
//
// BOARD
//
 { INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_ENTRY, 0x7500 },                     // [Board] Init Dxe Entry
 { INTEL_RC_STATUS_CODE_BOARD_INIT_DXE_EXIT, 0x757F },                      // [Board] Init Dxe Exit
 { INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_ENTRY, 0x8500 },                     // [Board] Init SMM Entry
 { INTEL_RC_STATUS_CODE_BOARD_INIT_SMM_EXIT, 0x857F },                      // [Board] Init SMM Exit
 { INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_ENTRY, 0x9500 },                 // [Board] Init Post-Mem Entry
 { INTEL_RC_STATUS_CODE_BOARD_INIT_POSTMEM_EXIT, 0x957F },                  // [Board] Init Post-Mem Exit
 { INTEL_RC_STATUS_CODE_BOARD_BIOS_S3_ENTRY, 0xB503 },                      // [Board] BIOS S3 Entry
 { INTEL_RC_STATUS_CODE_BOARD_BIOS_S4_ENTRY, 0xB504 },                      // [Board] BIOS S4 Entry
 { INTEL_RC_STATUS_CODE_BOARD_BIOS_S5_ENTRY, 0xB505 },                      // [Board] BIOS S5 Entry
 { INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_ENTRY, 0xD500 },                  // [Board] Init Pre-Mem Entry
 { INTEL_RC_STATUS_CODE_BOARD_INIT_BOARD_ID_UNKNOWN, 0xD50E },              // [Board] Unknown BOARD-ID
 { INTEL_RC_STATUS_CODE_BOARD_INIT_PREMEM_EXIT, 0xD57F },                   // [Board] Init Pre-Mem Exit
 //
 // Features : TCSS
 //
 { INTEL_RC_STATUS_CODE_FEATURES_TCSS_GET_SET_TBT_SEC_LEVEL_ENTRY, 0x7680},               // [Features] TCSS Get TBT Security Level Entry
 { INTEL_RC_STATUS_CODE_FEATURES_TCSS_GET_SET_TBT_SEC_LEVEL_EXIT, 0x7681},                // [Features] TCSS Get TBT Security Level Exit
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_START, 0x98E0},                     // [Features] Retimer Capsule Update Start
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_OFFLINE_MODE_ENTRY, 0x98E1},        // [Features] Retimer Capsule Send LSEN(0) cmd - Offline Mode Entry
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_OFFLINE_MODE_EXIT, 0x98E2},         // [Features] Retimer Capsule Send LSEN(1) cmd - Offline Mode Exit
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_ENUM_CMD, 0x98E3},                  // [Features] Retimer Capsule Sned ENUM cmd
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_LSUP_CMD, 0x98E4},                  // [Features] Retimer Capsule Sned LSUP cmd
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_USUP_CMD, 0x98E5},                  // [Features] Retimer Capsule Sned USUP cmd
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_PD_DRIVE, 0x98E6},                  // [Features] Retimer Capsule PD Drive Start
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_PD_RESTORE, 0x98E7},                // [Features] Retimer Capsule PD Restore Start
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_RETIMER_FP_GPIO_ASSERT, 0x98E8},    // [Features] Retimer Capsule Assert Retimer FP GPIO
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_RETIMER_FP_GPIO_DEASSERT, 0x98E9},  // [Features] Retimer Capsule De-Assert Retimer FP GPIO
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_RETIMER_CAPSULE_UPDATE_RETIMER_NVM_FW, 0x98EA},     // [Features] Retimer Capsule Update NVM FW
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_START, 0x98EB},                        // [Features] dTBT Capsule Update Start
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_CREATE_DEV_INST, 0x98EC},              // [Features] dTBT Capsule Create Device Instance
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_DTBT_CAPSULE_UPDATE_NVM_FW, 0x98ED},                // [Features] dTBT Capsule Update NVM FW
 { INTEL_RC_STATUS_CODE_FEATURES_USBC_READ_RETIMER_VERSION_START, 0x98EE},                // [Features] Read Retimer Version Start
 {0,0}
 };

STATUS_CODE_TO_POST_CODE_MAP *mPlatformPostCodeRscMap[] = {
  mPlatformPostCodeProgressMap
};

/**
  Find the post code data from status code value.

  @param  Map              The map used to find in.
  @param  Value            The status code value.

  @return PostCode         0 for not found.

**/
UINT32
FindPlatformRscPostCodeData (
  IN STATUS_CODE_TO_POST_CODE_MAP *Map,
  IN EFI_STATUS_CODE_VALUE   Value
  )
{
  while (Map->Value != 0) {
    if (Map->Value == Value) {
      return Map->Data;
    }
    Map++;
  }
  return 0;     // If value = 0 => No corresponding PostCode value exists for the RSC value.
}

/**
  Get Platform Reference Code PostCode from status code value.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
GetPlatformRscPostCodeData (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  return FindPlatformRscPostCodeData (mPlatformPostCodeRscMap[0], Value);
}
