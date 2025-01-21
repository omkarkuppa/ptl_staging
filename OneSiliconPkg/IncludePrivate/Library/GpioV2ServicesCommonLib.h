/** @file
  Header file for common part for Gpio V2 Controller PEI and DXE modules

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

#ifndef _GPIOV2_SERVICES_COMMON_H_
#define _GPIOV2_SERVICES_COMMON_H_

#include <GpioV2Services.h>
#include <GpioV2ControllerInterface.h>


/**
  This procedure retrieves pointer to array of communities for PCH with index 'PchIndex'

  @param[in] GpioServices         Gpio Services

  @retval                         pointer to array of communities on requested PCH
**/
EFI_STATUS
EFIAPI
GpioV2ServicesInit (
  IN  GPIOV2_SERVICES       *GpioServices
  );


/**
  This procedure assigns base GpioV2 PWM functions.

  @param[in]   GpioPwm           Gpio PWM Services

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
**/
EFI_STATUS
EFIAPI
GpioV2PwmInit (
  IN OUT GPIOV2_PWM       *GpioPwm
  );

/**
  This function allows user to control read and write access
  to gpio registers for requested SAI policy.

  To properly configure Access Control to a certain group of gpio registers
  user has to modify three registers:
  1. CP (ControlPolicy) - this register controls access to RRAC and RWAC registers
  2. RRAC (Read) - this register controls read access to gpio registers
  3. RWAC (Write) - this register controls write access to gpio registers

  @param[in] GpioServices                Gpio Services
  @param[in] CommunityIndex              Index of community
  @param[in] SaiGroupIndex               Index of Group in Access Control SAI Groups
                                         There are several groups which gpio registers are assigned to
  @param[in] SaiGroupDwIndex             Index of DW in Access Control Group
                                         For each of the above mentioned groups there are several DWs
  @param[in] AccessControlData           Bitmask with SAI permissions for policy

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
SetAccessControl (
  IN GPIOV2_SERVICES                    *GpioServices,
  IN UINT32                             CommunityIndex,
  IN UINT32                             SaiGroupIndex,
  IN GPIOV2_ACCESS_CONTROL_SAI_DW_DATA  *AccessControlDwData
  );

/**
  This function allows user to get read and write access settings
  to gpio registers for requested SAI policy.

  To properly configure Access Control to a certain group of gpio registers
  user has to modify three registers:
  1. CP (ControlPolicy) - this register controls access to RRAC and RWAC registers
  2. RRAC (Read) - this register controls read access to gpio registers
  3. RWAC (Write) - this register controls write access to gpio registers

  @param[in] GpioServices                Gpio Services
  @param[in] CommunityIndex              Index of community
  @param[in] SaiGroupIndex               Index of Group in Access Control SAI Groups
                                         There are several groups which gpio registers are assigned to
  @param[in] SaiGroupDwIndex             Index of DW in Access Control Group
                                         For each of the above mentioned groups there are several DWs
  @param[in] AccessControlData           Bitmask with SAI group permissions for policy

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
GetAccessControl (
  IN GPIOV2_SERVICES                    *GpioServices,
  IN UINT32                             CommunityIndex,
  IN UINT32                             SaiGroupIndex,
  IN GPIOV2_ACCESS_CONTROL_SAI_DW_DATA  *AccessControlDwData
  );

#endif // _GPIOV2_SERVICES_COMMON_H_
