/** @file
  Header file for ItbtPcieRpLib.

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
#ifndef _ITBT_PCIERP_LIB_H_
#define _ITBT_PCIERP_LIB_H_

#include <Ppi/SiPolicy.h>
#include <TcssDataHob.h>
#include <Library/HobLib.h>

#pragma pack(1)

typedef struct {
  UINT8   Segment;
  UINT8   Bus;
  UINT8   Device;
  UINT8   Function;
  BOOLEAN Enable;
} ITBT_PCIE_RP_INFO;

#pragma pack()
/**
  Get ITBT Pcie Root Port Segment Bus Device and Function Number by Port Index

  @param[in]  PortIndex             Root port index. (0-based)
  @param[out] Segment               Return corresponding root port segment number.
  @param[out] Bus                   Return corresponding root port bus number.
  @param[out] Device                Return corresponding root port device number.
  @param[out] Function              Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port Info is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid/disabled, or TcssHob is not existed when mItbtPcieRpInfoUpdated is FALSE.
**/
EFI_STATUS
EFIAPI
GetItbtPcieRpInfo (
  IN   UINTN    PortIndex,
  OUT  UINTN    *Segment,
  OUT  UINTN    *Bus,
  OUT  UINTN    *Device,
  OUT  UINTN    *Function
  );

/**
  Get ITBT PCIE Root Port Index by ITBT PCIE Root Port Segment Bus Device and Function Number

  @param[in]  Segment             Root port segment number.
  @param[in]  Bus                 Root port Bus.
  @param[in]  Device              Root port Device.
  @param[in]  Function            Root port Function.
  @param[out] PortIndex           Return corresponding Root Port index (0-based)

  @retval     EFI_SUCCESS         PortIndex is retrieved
  @retval     EFI_UNSUPPORTED     PortIndex is invalid/disabled, or TcssHob is not existed when mItbtPcieRpInfoUpdated is FALSE.
**/
EFI_STATUS
EFIAPI
GetItbtPciePortIndex (
  IN   UINTN    Segment,
  IN   UINTN    Bus,
  IN   UINTN    Device,
  IN   UINTN    Function,
  OUT  UINTN    *PortIndex
  );

#endif // _ITBT_PCIERP_LIB_H_
