/** @file
  Header file for PchPcieRpLib.

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
#ifndef _PCH_PCIERP_LIB_H_
#define _PCH_PCIERP_LIB_H_

#include <Uefi.h>
#include <PcrDefine.h>

/**
  PCIe controller bifurcation configuration.
**/
typedef enum {
  PcieBifurcationDefault = 0,
  PcieBifurcation4x1,
  PcieBifurcation1x2_2x1,
  PcieBifurcation2x2,
  PcieBifurcation1x4,
  PcieBifurcation4x2,
  PcieBifurcation1x4_2x2,
  PcieBifurcation2x2_1x4,
  PcieBifurcation2x4,
  PcieBifurcation1x8,
  PcieBifurcationUnknown,
  PcieBifurcationMax
} PCIE_BIFURCATION_CONFIG;

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval P2SB_PID    Returns PID for SBI Access
**/
P2SB_PID
GetRpSbiPid (
  IN UINTN  RpIndex
  );

/**
  Get Pch Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber            Root port physical number. (0-based)
  @param[out] RpDev               Return corresponding root port device number.
  @param[out] RpFun               Return corresponding root port function number.

  @retval EFI_SUCCESS
**/
EFI_STATUS
EFIAPI
GetPchPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINTN   *RpDev,
  OUT UINTN   *RpFun
  );

/**
  OnHotPlugEnabledPciePort: Check if the input Endpoint device bus number is enumerated under Hot pluggable Root port.

  @param[in]     EndpointBus    Input Endpoint Device bus number

  @retval TRUE  if  if Endpoint device bus number is enumerated under Hot pluggable Root port.
  @retval False if  if Endpoint device bus number is not enumerated under Hot pluggable Root port.
**/

BOOLEAN
EFIAPI
OnHotPlugEnabledPciePort (
  IN UINT8 EndpointBus
  );
#endif // _PCH_PCIERP_LIB_H_
