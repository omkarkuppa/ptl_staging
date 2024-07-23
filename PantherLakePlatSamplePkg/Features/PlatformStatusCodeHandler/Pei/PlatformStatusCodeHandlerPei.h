/** @file
  Platform status code implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2010 Intel Corporation.

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

#ifndef __STATUS_CODE_HANDLER_PEI_H__
#define __STATUS_CODE_HANDLER_PEI_H__

#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/DebugLib.h>
#include <Library/ReportStatusCodeLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PcdLib.h>
#include <Library/Usb3DebugPortLib.h>
#include <Library/SerialIoUartDebugHelperLib.h>
#include <Library/SerialPortParameterLib.h>
#include <Library/SerialIoUartLib.h>
#include <Library/SerialIoUartDebugPropertyLib.h>
#include <SerialIoDevices.h>


/**
  Convert status code value and extended data to readable ASCII string, send string to Usb3 device.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to Usb3 successfully.

**/
EFI_STATUS
EFIAPI
Usb3StatusCodeReportWorker (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  );

/**
  Convert status code value and extended data to readable ASCII string, send string to SerialIoUart device.

  @param  PeiServices      An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param  CodeType         Indicates the type of status code being reported.
  @param  Value            Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param  Instance         The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param  CallerId         This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param  Data             This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to SerialIoUart successfully.

**/
EFI_STATUS
EFIAPI
SerialIoUartStatusCodeReportWorker (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  );

#endif
