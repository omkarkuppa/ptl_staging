/** @file
  Serial IO I3C Private Lib implementation.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi/UefiBaseType.h>
#include <Library/SerialIoI3cSocLib.h>
#include <Library/BaseLib.h>


/**
  Initialize the I3c controller

  @param[in, out] I3cHandle       The SerialIo I3C Handle instance
  @param[in]      SiPolicy         The SI Policy PPI instance
  @param[in]      SerialIoConfig   SerialIo Config Block
**/
VOID
SerialIoI3cSocInit (
  IN OUT SERIAL_IO_I3C_HANDLE   *I3cHandle,
  IN     SI_POLICY_PPI          *SiPolicy,
  IN     SERIAL_IO_CONFIG       *SerialIoConfig
  )
{
  return;
}

