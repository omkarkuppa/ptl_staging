/** @file
  Tcss Pmc Soc library.

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

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PmcLib.h>
#include <Library/TcssPmcLib.h>
#include <Library/PmcPrivateLib.h>

/**
  This function sends command to SOC PMC.

  @param[in]  Command           Command to be issued to PMC IPC 1 interface
  @param[in]  SubCmdId          SUB_CMD_ID for provided Command
  @param[in]  CmdSize           Total size in byte to be sent via PMC IPC 1 interface
  @param[in]  WriteBufPtr       Pointer to Structure of 4 DWORDs to be issued to PMC IPC 1 interface
  @param[out] ReadBufPtr        Pointer to Structure of 4 DWORDs to be filled by PMC IPC 1 interface

  @retval EFI_SUCCESS             Command was executed successfully
  @retval EFI_INVALID_PARAMETER   Invalid command size
  @retval EFI_DEVICE_ERROR        IPC command failed with an error
  @retval EFI_TIMEOUT             IPC command did not complete after 1s
**/

EFI_STATUS
TcssSocPmcSendCommand (
  IN  UINT8                    Command,
  IN  UINT8                    SubCmdId,
  IN  UINT8                    CmdSize,
  IN  PMC_IPC_COMMAND_BUFFER   *WriteBufPtr,
  OUT PMC_IPC_COMMAND_BUFFER   *ReadBufPtr
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[TCSS] TcssSocPmcSendCommand Start\n"));

  Status = PmcSendCommand (
             PmcGetPwrmBase (),
             Command,
             SubCmdId,
             CmdSize,
             WriteBufPtr,
             ReadBufPtr
             );

  DEBUG ((DEBUG_INFO, "[TCSS] TcssSocPmcSendCommand End, Status = %r\n", Status));

  return Status;
}

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
)
{
  EFI_STATUS                            Status;
  UINT32                                PmcIomReadyBitMask;
  PMC_IPC_COMMAND_BUFFER                Wbuf;

  DEBUG ((DEBUG_INFO, "[TCSS] %a: Sending TCSS BIOS Init Done message to PMC. TcssPmcBitMask is 0x%x\n", __FUNCTION__, TcssPmcBitMask));

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));

  //
  // Set IomReady bit if IOM is ready
  //
  PmcIomReadyBitMask = 0;
  if (IomReadyStatus == EFI_SUCCESS) {
    PmcIomReadyBitMask = BIT0;
  }

  Wbuf.Buf0 = PmcIomReadyBitMask | TcssPmcBitMask;

  Status = TcssSocPmcSendCommand (
             PMC_IPC_COMMAND_ID_TCSS_INFO,
             PMC_IPC_SUB_COMMAND_ID_TCSS_BIOS_INIT_DONE,
             4,
             &Wbuf,
             NULL);

  return Status;
}

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
)
{
  EFI_STATUS              Status;
  PMC_IPC_COMMAND_BUFFER  Wbuf;

  DEBUG ((DEBUG_INFO, "[TCSS] PmcTcssConvUsbA Start\n"));

  ZeroMem (&Wbuf, sizeof (PMC_IPC_COMMAND_BUFFER));
  Wbuf.Buf0 = (((TcssUsbcPort & 0xF) << 4) | ((PchXhciUsbPort & 0xF) << 8));
  if (OriHslEnable == 1) {
    Wbuf.Buf0 |= ORIENTATION_HSL_BIT;
  }
  Status = TcssSocPmcSendCommand (PMC_CONVERT_TO_TYPE_A,
                                  0,
                                  4,
                                  &Wbuf,
                                  NULL);

  DEBUG ((DEBUG_INFO, "[TCSS] PmcTcssConvUsbA End, Status = %r\n", Status));

  return Status;
}
