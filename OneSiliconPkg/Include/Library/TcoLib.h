/** @file
  Header file for TcoLib.

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
#ifndef _TCO_LIB_H_
#define _TCO_LIB_H_

/**
  Check TCO second timeout status.

  @param[in] BaseAddress TCO Base Address.

  @retval  TRUE   TCO reboot happened.
  @retval  FALSE  TCO reboot didn't happen.
**/
BOOLEAN
TcoSecondToHappened (
  IN UINT16 BaseAddress
  );

/**
  This function clears the Second TO status bit

  @param[in] BaseAddress TCO Base Address.
  
**/
VOID
TcoClearSecondToStatus (
  IN UINT16 BaseAddress
  );

/**
  Check TCO SMI ENABLE is locked

  @param[in] BaseAddress TCO Base Address.

  @retval TRUE  TCO SMI ENABLE is locked
          FALSE TCO SMI ENABLE is not locked
**/
BOOLEAN
TcoIsSmiLock (
  IN UINT16 BaseAddress
  );

/**
  Halts Tco timer

  @param[in] BaseAddress TCO Base Address.

**/
VOID
TcoHaltTimer (
  IN UINT16 BaseAddress
  );

#endif // _TCO_LIB_H_
