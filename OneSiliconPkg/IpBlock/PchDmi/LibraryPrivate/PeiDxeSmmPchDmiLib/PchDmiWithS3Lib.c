/** @file
  PCH DMI library with S3 boot script support.

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
#include <Uefi/UefiBaseType.h>
#include <Library/PchPcrLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/S3BootScriptLib.h>
#include <Register/PchPcrRegsDeprecated.h>
#include <Register/PchDmiRegs.h>
#include <Register/PchDmi15Regs.h>


/**
  Configure DMI Lock
**/
VOID
PchDmiSetLockWithS3BootScript (
  VOID
  )
{
  UINT32  Data32Or;
  UINT32  Data32And;

  Data32And = 0xFFFFFFFF;
  Data32Or = B_PCH_DMI15_PCR_MPC_SRL;

  if (IsPchWithPdmi ()) {
    PchPcrAndThenOr32 (
      PID_DMI_DEPRECATED, R_PCH_DMI15_PCR_MPC,
      Data32And,
      Data32Or
      );
    PCH_PCR_BOOT_SCRIPT_READ_WRITE (
      S3BootScriptWidthUint32,
      PID_DMI_DEPRECATED, R_PCH_DMI15_PCR_MPC,
      &Data32Or,
      &Data32And
      );
  }
}
