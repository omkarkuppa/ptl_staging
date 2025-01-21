/** @file
  Platform GUID forward implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#ifndef __GUID_FORWARD_HANDLER_RUNTIME_DXE_H__
#define __GUID_FORWARD_HANDLER_RUNTIME_DXE_H__

#include <Protocol/ReportStatusCodeHandler.h>
#include <Library/DebugLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/PostCodeMapLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Defines/HostBridgeDefines.h>
#define NCDECS_CR_SCRATCHPAD_NCU_0_REG 0x5428
#define NCDECS_CR_SCRATCHPAD_NCU_2_REG 0x542C
#include <Guid/ZeroGuid.h>
#include <Guid/EventGroup.h>
#include <Protocol/LoadedImage.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PostCodeLib.h>
#include <Library/PcdLib.h>

/**
  Convert status code value and GUID,  write data to Scratch register.

  @param[in]  CodeType     Indicates the type of status code being reported.
  @param[in]  Value        Describes the current status of a hardware or software entity.
                           This included information about the class and subclass that is used to
                           classify the entity as well as an operation.
  @param[in]  Instance     The enumeration of a hardware or software entity within
                           the system. Valid instance numbers start with 1.
  @param[in]  CallerId     This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data         This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to port code successfully.

**/
EFI_STATUS
EFIAPI
GuidForwardWorker (
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN EFI_GUID                       *CallerId,
  IN EFI_STATUS_CODE_DATA           *Data OPTIONAL
  );

#endif
