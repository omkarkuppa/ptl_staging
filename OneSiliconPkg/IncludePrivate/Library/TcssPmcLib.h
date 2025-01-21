/** @file
  TcssPmcSocLib  header file

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
#ifndef _TCSS_PMC_SOC_LIB_H_
#define _TCSS_PMC_SOC_LIB_H_

///
/// PMC IPC command opcodes for Type-C Information
///
#define PMC_IPC_COMMAND_ID_TCSS_INFO                   0xA5
#define PMC_IPC_SUB_COMMAND_ID_TCSS_BIOS_INIT_DONE     0x00
#define TCSS_PMC_REPLAY_EN                             BIT1

///
/// Convert to type A command
///
#define PMC_CONVERT_TO_TYPE_A                          0xA7
#define ORIENTATION_HSL_BIT                            BIT13

/**
  Handshake with PMC if PMC handshaking support is enabled BIOS sends TCSS_BIOS_INIT Done (aka.USBC connection replay)
  message along with USBC connctions replay setting to PMC. All TCSS initialization is expected to be done before PMC
  do any port negotiation. The indicator sent from BIOS to PMC should be treated as TCSS BIOS initialization done
  semaphore.

  @param[in]       IomReadyStatus       Status of IomReady bit read from TCSS Hob
  @param[in]       TcssPmcBitMask       TCSS PMC status, BIT1: Replay status based on VccSt and BootMode

  @retval          EFI_SUCCESS          Command success
  @retval          EFI_DEVICE_ERROR     Command error
**/
UINTN
PmcTcssInitDone (
  IN  UINT32               IomReadyStatus,
  IN  UINT32               TcssPmcBitMask
);

/**
  Tcss usb typeC convert to typeA

  @param[in]       TcssUsbcPort         Tcss usb typc port number
  @param[in]       PchXhciUsbPort       Pch xhci usb port number
  @param[in]       OriHslEnable         Enable / Disable Orientation Hsl bit

  @retval          EFI_SUCCESS          Command success
  @retval          EFI_DEVICE_ERROR     Command error
**/
UINTN
PmcTcssConvUsbA (
  IN  UINT8                   TcssUsbcPort,
  IN  UINT8                   PchXhciUsbPort,
  IN  UINT8                   OriHslEnable
);

#endif // _TCSS_PMC_SOC_LIB_H_
