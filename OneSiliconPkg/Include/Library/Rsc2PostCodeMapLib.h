/** @file
  This library class provides Report Status code to PostCode Map.
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

#ifndef __RSC_2_POST_CODE_MAP_LIB__
#define __RSC_2_POST_CODE_MAP_LIB__

/**
  Get PostCode from status code type and value.

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
  );

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
  );

#endif
