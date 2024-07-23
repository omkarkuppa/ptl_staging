/**@file
  Library provide the interface to get the PMON (Performance Monitoring) data.

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

#ifndef _PMON_DATA_LIB_H_
#define _PMON_DATA_LIB_H_

/**
  Create PMON Hob.

  @param[in]  SiPolicyPpi    Pointer to SI_POLICY_PPI.
  @param[out] PmonDataHobOut Pointer to the PMON Hob.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
CreatePmonHob (
  IN  SI_POLICY_PPI       *SiPolicyPpi,
  OUT PMON_HOB_DATA       **PmonDataHobOut
  );
/**
  Get the PMON global discovery.

  @param[out] Size      Size of the PMON global discovery.

  @retval               Return the pointer to the PMON global discovery.
**/
VOID*
GetPmonGlobalDiscoveryTable (
  OUT UINTN  *Size
  );
/**
  Get the PMON unit discovery.

  @param[out] Size      Size of the PMON global discovery.

  @retval               Return the pointer to the PMON unit discovery.
**/
VOID*
GetPmonUnitDiscoveryTable (
  OUT UINTN  *Size
  );

/**
  Check if PMON is support or not at that port.

  @param[in] PortId     Pmon port id.

  @retval               TRUE for support, FALSE for not support.
**/
BOOLEAN
GetPmonPortSupport (
  IN UINT16 PortId
  );
#endif  // _PMON_DATA_LIB_H_
