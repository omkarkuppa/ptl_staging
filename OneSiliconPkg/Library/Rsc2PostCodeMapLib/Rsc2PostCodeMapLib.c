/** @file
  Rsc2PostCodeMap Library implementation.

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
#include <Library/SiPostCodeMapLib.h>
#include <Library/PlatformPostCodeMapLib.h>
#include <Library/PostCodeMapLibOverride.h>


/**
  Get PostCode from Intel RC status code structures.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
EFIAPI
GetFromIntelRsc2PostCodeMaps (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  // Check if incoming RSC is for MRC.
  if ((Value & INTEL_RC_CLASS_SUBCLASS_CODE_MASK) == INTEL_RC_STATUS_CODE_MRC_POST_CODE) {
    return GetMrcRscPostCodeData (Value);
  }

  //
  // If value is in the Silicon Reference Code Range, get the Rsc2PostCode value from the SiPostCodeMapLib else get from the PlatformRscMap
  //
  if ((Value & INTEL_RC_SUBCLASS_CODE_MASK) < INTEL_RC_PLATFORM_STATUS_CODE_BEGIN ){
    return GetSiRscPostCodeData (Value);
  } else {
    //
    // If RSC value is in the Platform Code range.
    //
    return GetPlatformRscPostCodeData (Value);
  }

}

/**
  Get PostCode from status code value.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
EFIAPI
GetPostCodeFromRscMap (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  UINT32               PostCodeValue=0;

  //
  // Check for the Rsc2PostCode map value in the Override Map Library. Board/Platform/OEM can force override the RSC value to
  // required PostCode value using this library.
  //
  PostCodeValue = GetRsc2PostCodeOverrideMap (Value);
  if ( PostCodeValue != 0 ) {
    return PostCodeValue;
  }

  return GetFromIntelRsc2PostCodeMaps (Value);
}
