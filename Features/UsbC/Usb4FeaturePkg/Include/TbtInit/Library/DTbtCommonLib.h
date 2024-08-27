/**@file

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

@par Specification
**/

#ifndef _DTBT_COMMON_LIB_H_
#define _DTBT_COMMON_LIB_H_

#include <Library/BaseLib.h>

//
// Macro that checks PCI config
//
#define IS_PCI_A_BRIDGE(Hdr)            (((Hdr)->HeaderType & HEADER_LAYOUT_CODE) == (HEADER_TYPE_PCI_TO_PCI_BRIDGE))
#define IS_PCI_SUPPORT_MULTI_FUNC(Hdr)  ((Hdr)->HeaderType & HEADER_TYPE_MULTI_FUNCTION)
#define IS_PCI_SUPPORT_CAPABILITY(Hdr)  ((Hdr)->Status & EFI_PCI_STATUS_CAPABILITY)

//
//Thunder Bolt Device IDs
//

//
// Alpine Ridge HR device IDs
//
#define AR_HR_2C  0x1576
#define AR_HR_4C  0x1578
#define AR_XHC    0x15B5
#define AR_XHC_4C 0x15B6
#define AR_HR_LP  0x15C0
//
// Alpine Ridge C0 HR device IDs
//
#define AR_HR_C0_2C  0x15DA
#define AR_HR_C0_4C  0x15D3
//
// Titan Ridge HR device IDs
//
#define TR_HR_2C  0x15E7
#define TR_HR_4C  0x15EA
//
// Maple Ridge HR device IDs
//
#define MR_HR_2C  0x1133
#define MR_HR_4C  0x1136

//
// Barlow Ridge HR device IDs
//
#define BR_HR_1  0x5780
#define BR_HR_2  0x5783

#define PCI_VID_INTEL       0x8086

/**
  BIOS initiates the "SetMaxPayloadSize" mailbox command with the required Meta Data

  @param[in]  Data                      Mps Data. 0 map to 128b and 1 map to 256b
  @param[in]  Bus                       Bus number for Host Router (DTBT)
  @param[in]  Device                    Device number for Host Router (DTBT)
  @param[in]  Function                  Function number for Host Router (DTBT)
  @param[in]  Timeout                   Time out with 100 ms granularity
**/
BOOLEAN
SetMaxPayloadSize (
  IN    UINT8                           Data,
  IN    UINT8                           Bus,
  IN    UINT8                           Device,
  IN    UINT8                           Function,
  IN    UINT32                          Timeout
);

/**
  Query USB4 version that can be supported by the given host router's VID & PID

  @param[in] VendorId - USB4 host router Vendor ID.
  @param[in] DeviceId - USB4 host router Device ID.

  @retval USB4 version supported by host router.
**/
UINT8 DtbtQueryUsb4Ver (
  IN UINT16    VendorId,
  IN UINT16    DeviceId
  );

//
//End of Thunderbolt(TM) Device IDs
//

//@todo Seems to only be used by Platform/TBT/Smm/TbtSmm.inf
//@todo should refactor this to only be present in that driver
//@todo also definitions like this should never be in a .h file anyway
//@todo this is a quick hack to get things compiling for now
#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wunused-variable"
#endif

/**
  Check connected TBT controller is supported or not by DeviceID

  @param[in]  DeviceID              DeviceID of of TBT controller


  @retval     TRUE                  Valid DeviceID
  @retval     FALSE                 Invalid DeviceID
**/

BOOLEAN
IsTbtHostRouter (
  IN    UINT16  DeviceID
  );

/**
  Get Pch/Peg Pcie Root Port Device and Function Number for TBT by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[in]  RpBus                 Root port bus number.
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetDTbtRpDevFun(
  IN  UINT8   RpNumber,
  IN  UINT8   RpBus,
  OUT UINT8   *RpDev,
  OUT UINT8   *RpFunc
  );

#endif
