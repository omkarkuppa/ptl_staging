/** @file
  Base IMR Library header file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _BASE_IMR_INFO_LIB_H_
#define _BASE_IMR_INFO_LIB_H_

/**
  This function will check if IMR range is enabled or not

  @param[in] ImrBaseReg       IMR base address
  @param[in] ImrMaskReg       IMR mask value

  @retval TRUE                IMR range is enabled
  @retval FALSE               IMR range is disabled
**/
BOOLEAN
IsImrBaseRangeEnabled (
  IN UINT32                   ImrBaseReg,
  IN UINT32                   ImrMaskReg
  );

/**
  This function will check if TraceHub range is enabled or not

  @retval TRUE                TraceHub range is enabled
  @retval FALSE               TraceHub range is disabled
**/
BOOLEAN
IsImrTraceHubBaseRangeEnabled (
  VOID
  );

/**
  Determine if Imr Prmrr Base Range is Enabled.

  @retval TRUE      Imr Prmrr Base Range is enabled.
  @retval FALSE     Imr Prmrr Base Range is disabled.

**/
BOOLEAN
EFIAPI
IsImrPrmrrBaseRangeEnabled (
  VOID
  );

/**
   Determine if TSE Imr Base Range is Enabled.
   @retval TRUE      Imr Tse Base Range is enabled.
   @retval FALSE     Imr Tse Base Range is disabled.
**/
BOOLEAN
EFIAPI
IsImrTseBaseRangeEnabled (
  VOID
  );

#endif // _BASE_IMR_INFO_LIB_H_
