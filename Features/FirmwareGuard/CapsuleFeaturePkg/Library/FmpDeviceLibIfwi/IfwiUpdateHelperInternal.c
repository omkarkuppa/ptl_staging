/** @file
  Support FmpDeviceLibIfwi library.
  This file defined some internal macro and internal function.

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

//
// EDK-II Foundation, used in this driver.
//
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
//
// Silicon Foundation, used in this driver.
//
#include <Register/HeciRegs.h>
#include <Library/DxeMeLib.h>
#include <Library/MeInfoLib.h>
#include <Library/SpiAccessLib.h>

#include "IfwiUpdateHelperInternal.h"

#define ME_FWSTS2_TIMEOUT_COUNTER  150

/**
  This code is used to bypass the runtime service set variable.
  To prevent write the variable in Non-Volatile device.

  @param[in] VariableName  Name of Variable to be found.
  @param[in] VendorGuid    Variable vendor GUID.
  @param[in] Attributes    Attribute value of the variable found
  @param[in] DataSize      Size of Data found. If size is less than the
                           data, this value contains the required size.
  @param[in] Data          Data pointer.

  @retval EFI_SUCCESS  Hook the service always return success.

**/
EFI_STATUS
EFIAPI
HookSetVariableFunction (
  IN CHAR16    *VariableName,
  IN EFI_GUID  *VendorGuid,
  IN UINT32    Attributes,
  IN UINTN     DataSize,
  IN VOID      *Data
  )
{
  DEBUG ((
    DEBUG_INFO,
    "Hook the set service! VariableName: %s; GUID: %g DataSize: %x\n",
    VariableName,
    *VendorGuid,
    DataSize
    ));

  return EFI_SUCCESS;
}

/**
  This code is used to bypass the runtime service get variable.
  To prevent the variable read error in Non-Volatile device.

  @param[in]      VariableName  Name of Variable to be found.
  @param[in]      VendorGuid    Variable vendor GUID.
  @param[out]     Attributes    Attribute value of the variable found.
  @param[in, out] DataSize      Size of Data found. If size is less than the
                                data, this value contains the required size.
  @param[out]     Data          The buffer to return the contents of the variable. May be NULL
                                with a zero DataSize in order to determine the size buffer needed.

  @retval EFI_SUCCESS  Hook the service always return success.

**/
EFI_STATUS
EFIAPI
HookGetVariableFunction (
  IN     CHAR16    *VariableName,
  IN     EFI_GUID  *VendorGuid,
     OUT UINT32    *Attributes,   OPTIONAL
  IN OUT UINTN     *DataSize,
     OUT VOID      *Data          OPTIONAL
  )
{
  DEBUG ((
    DEBUG_INFO,
    "Hook the get service! VariableName: %s; GUID: %g DataSize: %x\n",
    VariableName,
    *VendorGuid,
    *DataSize
    ));

  return EFI_SUCCESS;
}

/**
  Check FWSTS2 InfPhaseCode and ExtStatCode1 status after issuing Set
  Manufacturing Me Reset and Halt Request to ME. Or wait for 15 sec.,
  whichever occurs first. So that ME is now halted and can be flashed.

**/
VOID
WaitForMeBeforeFlashing (
  VOID
  )
{
  UINT64                 HeciBaseAddress;
  HECI_GS_SHDW_REGISTER  MeFwsts2;
  UINT32                 TimeOut;

  HeciBaseAddress = MeHeciPciCfgBase (HECI1);
  MeFwsts2.ul     = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);  // Read ME FWSTS2
  TimeOut         = ME_FWSTS2_TIMEOUT_COUNTER;

  ///
  /// Poll to check if check if FWSTS2 InfPhaseCode is set to 0x03 and ExtStatCode1 is 0x47.Once these values are set
  /// or 15 sec. has passed whichever occurs first, we proceed with flash operations.
  ///
  while (TimeOut != 0) {
    if ((MeFwsts2.r.InfPhaseCode == 0x03) && (MeFwsts2.r.ExtStatCode1 == 0x47)) {
      break;
    } else {
      MicroSecondDelay (100000); // delay 100 ms.
      MeFwsts2.ul = PciSegmentRead32 (HeciBaseAddress + R_ME_CFG_HFS_2);
      TimeOut--;
    }
  }
  if (TimeOut == 0) {
    DEBUG ((DEBUG_INFO, "Me FWSTS2 15 sec. timeout has occurred. Proceeding with ME Flash update via capsule \n"));
  }
}

/**
  Support to halt the CSME for programming to SPI directly.

  @retval EFI_UNSUPPORTED       Current ME mode doesn't support this function.
  @retval EFI_SUCCESS           Command succeeded.
  @retval EFI_DEVICE_ERROR      HECI Device error, command aborts abnormally.
  @retval EFI_TIMEOUT           HECI does not return the buffer before timeout.
  @retval EFI_BUFFER_TOO_SMALL  Message Buffer is too small for the Acknowledge.

**/
EFI_STATUS
HaltMeBeforeFlash (
  VOID
  )
{
  EFI_STATUS  Status;

  Status = EFI_SUCCESS;

  if (SpiIsFlashDescriptorOverrideEnabled ()) {
    DEBUG ((DEBUG_INFO, "Flash Descriptor Override is enabled, ME is already halted.\n"));
    return Status;
  }

  Status = HeciSetManufacturingMeResetAndHalt ();
  if (EFI_ERROR (Status)) {
    DEBUG((DEBUG_ERROR, "Unable to set ME Reset and Halt\n"));
    ASSERT_EFI_ERROR (Status);
  } else {
    WaitForMeBeforeFlashing (); // TimeOut 15 sec.
  }

  return Status;
}

/**
  Get the IFWI flash request by config data.

  @param[in]      ConfigData       Update data within configuration INI file.
  @param[in]      UpdateImageBase  Input update image base address pointer.
  @param[in]      HelperImageBase  BGUP update image base address pointer.
                                   It would only be referenced when BiosGuard
                                   is enabled on platform.
  @param[in, out] FlashReq         Generic Flash ACCESS request to different
                                   flash storage mediaFlash request.

**/
VOID
GetIfwiUpdateRequest (
  IN     CONST UPDATE_CONFIG_DATA    ConfigData,
  IN     CONST VOID                  *UpdateImageBase,
  IN     CONST VOID                  *HelperImageBase,
  IN OUT       FLASH_ACCESS_REQUEST  *FlashReq
  )
{
  EFI_PHYSICAL_ADDRESS  FlashAddress;
  VOID                  *UpdateBuffer;
  UINTN                 UpdateLength;
  VOID                  *BgupBuffer;
  UINTN                 BgupBufferLength;

  //
  // This base address is related to firmware device base address.
  // In here, refer to the full SPI.
  //
  FlashAddress     = ConfigData.BaseAddress;
  UpdateBuffer     = (VOID *)((UINTN) UpdateImageBase + ConfigData.ImageOffset);
  UpdateLength     = ConfigData.Length;
  BgupBuffer       = (VOID *)((UINTN) HelperImageBase + ConfigData.HelperImageOffset);
  BgupBufferLength = ConfigData.HelperImageLength;

  //
  // Setup the flash requirement
  //
  FlashReq->Data            = UpdateBuffer;
  FlashReq->DataSize        = UpdateLength;
  FlashReq->FlashAddress    = FlashAddress;
  FlashReq->FlashRegionType = FlashRegionAll;
  FlashReq->BgupImage       = BgupBuffer;
  FlashReq->BgupImageSize   = BgupBufferLength;
}
