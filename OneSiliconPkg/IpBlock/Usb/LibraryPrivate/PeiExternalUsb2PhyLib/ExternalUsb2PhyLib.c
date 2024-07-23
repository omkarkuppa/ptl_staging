/** @file
  Library for External USB2 PHY programming.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#include <Library/Usb2PhyLib.h>
#include <Library/DebugLib.h>
#include <Register/Usb2PhyRegs.h>

/**
  Tune the USB 2.0 high-speed signals quality.

  @param[in]  UsbHandle                USB Handle
  @param[in]  SbAccess                 P2SB Register Access to USB2
**/
VOID
Usb2PhyProgramming (
  IN  USB_HANDLE                      *UsbHandle,
  IN  REGISTER_ACCESS                 *SbAccess
  )
{
  DEBUG ((DEBUG_INFO, "[%a] Start\n", __FUNCTION__));

  //
  //  SNPS PHY SIP POLICIES (SNPSPHYSIPPOL)
  //  Offset: 0x00
  //
  SbAccess->And32 (
    SbAccess,
    R_USB2_PCR_SNPSPHYSIPPOL,
    (UINT32)~(B_USB2_PCR_SNPSPHYSIPPOL_CLKCORETCGL2)
    );
}

/**
  Tune the USB 2.0 high-speed signals quality of USB2PHY version 4.4

  @param[in]  UsbHandle                USB Handle
  @param[in]  SbMsgAccess              P2SB Register Access to USB2
**/
VOID
Usb2Phy4p4Programming (
  IN  USB_HANDLE                      *UsbHandle,
  IN  REGISTER_ACCESS                 *SbMsgAccess
  )
{

}