/** @file
  Header file to get TDX information.
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

@par Specification Reference:
**/
#ifndef _TDX_INFO_LIB_H_
#define _TDX_INFO_LIB_H_

/**
  Reports if TDX is Supported or not
  @retval TRUE             If TDX is Supported
  @retval FALSE            If TDX is not Supported
**/

BOOLEAN
IsTdxSupported (
  VOID
  );


/**
  Reports if TDX is Enabled or not
  @retval TRUE             If TDX is Enabled
  @retval FALSE            If TDX is not Enabled
**/
BOOLEAN
IsTdxEnabled (
  VOID
  );

/**
  Reports seamrr size in MB

  @param[out] SeamrrSize

  @retval     EFI_INVALID_PARAMETER - Bad inputs provided
  @retval     EFI_UNSUPPORTED       - System is not TDX capable
  @retval     EFI_NOT_READY         - Cannot retrieve seamrr size
  @retval     EFI_SUCCESS
**/
EFI_STATUS
GetSeamrrSize (
  OUT UINT32 *SeamrrSize
  );
#endif /* _TDX_INFO_LIB_H_ */
