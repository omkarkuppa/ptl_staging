/** @file
  This file provides Smbus public library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#ifndef _SMBUS_INFO_LIB_H_
#define _SMBUS_INFO_LIB_H_

/**
  Return Smbus Device Number

  @retval Smbus Device Number
**/
UINT8
SmbusDevNumber (
  VOID
  );

/**
  Return Smbus Function Number

  @retval  Smbus Function Number
**/
UINT8
SmbusFuncNumber (
  VOID
  );

/**
  This function checks if SMBUS Host Lock is set

  @retval SMBUS host Hock state
**/
BOOLEAN
SmbusIsHostLocked (
  VOID
  );

/**
  This function clears the Smbus Status Register
**/
VOID
SmbusClearStatusReg (
  VOID
  );

/**
  Gets Io port base address of Smbus Host Controller.

  @retval The Io port base address of Smbus host controller.

**/
UINT16
GetSmbusIoPortBaseAddress (
  VOID
  );

#endif // _SMBUS_INFO_LIB_H_
