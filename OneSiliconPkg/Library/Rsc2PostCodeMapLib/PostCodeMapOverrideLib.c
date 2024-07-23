/** @file
  Override Rsc2PostCodeMap Library implementation.
  Board/Platform/OEM can force override the RSC value to required PostCode value using this library.
  OEMs can override this library to convert any Intel RC REPORT STATUS CODE Values to corresponding
  OEM specific 16Bit/8Bit PostCode Value and send to PostCode Interface.

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

STATUS_CODE_TO_POST_CODE_MAP mOverridePostCodeProgressMap[] = {
//
// Force override here
//
// Ex: { REPORT_STATUS_CODE_VALUE, 0xABCD },                 // [CPU] New PostCode Description
 {0,0}
 };

STATUS_CODE_TO_POST_CODE_MAP *mOverridePostCodeRscMap[] = {
  mOverridePostCodeProgressMap
};

/**
  Find the post code data from status code value.

  @param  Map              The map used to find in.
  @param  Value            The status code value.

  @return PostCode         0 for not found.

**/
UINT32
FindOverrideRscPostCodeData (
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
  Get Override Reference Code PostCode from status code value.

  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.

  @return PostCode

**/
UINT32
GetRsc2PostCodeOverrideMap (
  IN EFI_STATUS_CODE_VALUE          Value
  )
{
  return FindOverrideRscPostCodeData (mOverridePostCodeRscMap[0], Value);
}