/** @file
  Platform library to lock/unlock the PCH SMI Register and return the lock status
  of PCH SMI Register.

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

#ifndef _PLATFORM_PCH_SMI_REGISTER_LOCK_LIB_H_
#define _PLATFORM_PCH_SMI_REGISTER_LOCK_LIB_H_

/**
  Return the lock status of PCH SMI Register.

  @retval TRUE               PCH SMI Register is Locked.
  @retval FALSE              PCH SMI Register is Not Locked.

**/
BOOLEAN
EFIAPI
PlatformPchSmiRegisterIsLocked (
  VOID
  );

/**
  Lock/Unlock the PCH SMI Register.

  @param[in] Lock            Boolean to indicate lock or not.

  @retval EFI_SUCCESS        Succeed to set Lock or Unlock.

**/
EFI_STATUS
EFIAPI
PlatformLockPchSmiRegister (
  BOOLEAN                    Lock
  );

#endif // _PLATFORM_PCH_SMI_REGISTER_LOCK_LIB_H_
