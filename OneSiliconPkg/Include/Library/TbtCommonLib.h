/**@file
  Header file of TBT common library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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
#ifndef _TBT_COMMON_LIB_H_
#define _TBT_COMMON_LIB_H_

#include <Library/BaseLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <TbtMailBoxCmdDefinition.h>

#define DEFAULT_PCI_SEGMENT_NUMBER_ITBT_DMA0   0
#define DEFAULT_PCI_BUS_NUMBER_ITBT_DMA0       0
#define DEFAULT_PCI_DEVICE_NUMBER_ITBT_DMA0    0x0D
#define DEFAULT_PCI_FUNCTION_NUMBER_ITBT_DMA0  0x02

typedef struct _DEV_ID {
  UINT8 Segment;
  UINT8 Bus;
  UINT8 Dev;
  UINT8 Fun;
} DEV_ID;

#ifdef __GNUC__
#pragma GCC diagnostic warning "-Wunused-variable"
#endif

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   The number of 1 ms units to delay
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
  @retval     TRUE                      If command executes succesfully
**/
BOOLEAN
InternalWaitforCommandCompletion(
  IN  UINT64                            CommandOffsetAddress,
  IN  UINT32                            TimeOut,
  IN  BOOLEAN                           Tbt2PcieDone,
  OUT UINT32                            *Tbt2PcieValue
);

/**
  Get Security Level.
  @param[in]  Tbt2PcieOffset  TBT2PCIE Register Offset
  @param[in]  Pcie2TbtOffset  PCIE2TBT Register Offset
  @param[in]  Bus             Bus number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Device          Device number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Function        Function number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Timeout         Time out with 100 ms garnularity
**/
UINT8
TbtGetSecLevel (
  IN    UINT16                  Tbt2PcieOffset,
  IN    UINT16                  Pcie2TbtOffset,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
  Set Security Level.
  @param[in]  Data            Security State
  @param[in]  Tbt2PcieOffset  TBT2PCIE Register Offset
  @param[in]  Pcie2TbtOffset  PCIE2TBT Register Offset
  @param[in]  Bus             Bus number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Device          Device number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Function        Function number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Timeout         Time out with 100 ms garnularity
**/
BOOLEAN
TbtSetSecLevel (
  IN    UINT8                   Data,
  IN    UINT16                  Tbt2PcieOffset,
  IN    UINT16                  Pcie2TbtOffset,
  IN    UINT8                   Bus,
  IN    UINT8                   Device,
  IN    UINT8                   Function,
  IN    UINT8                   Command,
  IN    UINT32                  Timeout
  );

/**
  Execute TBT Mailbox Command without data

  1.Load PCIE2TBT register with command and set VALID bit of PCIE2TBT register to indicate a new command sent
  2.Wait for DONE bit set by LC acknowledging the command sent from the host before timeout
  3.Clear PCIE2TBT register valid bit which should be cleared by host SW after command done
  4.Wait for DONE bit cleared by LC if it is iTBT controller

  @param[in]  Command                   TBT Command
  @param[in]  Tbt2PcieOffset            TBT2PCIE Register Offset
  @param[in]  Pcie2TbtOffset            PCIE2TBT Register Offset
  @param[in]  Bus                       Bus number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Device                    Device number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Function                  Function number for HIA (ITBT) or Host Router (DTBT)
  @param[in]  Timeout                   Time out with 1 ms garnularity
  @retval     TRUE                      If command executes succesfully
**/
BOOLEAN
TbtSetPcie2TbtCommand(
  IN  UINT8                             Command,
  IN  UINT16                            Tbt2PcieOffset,
  IN  UINT16                            Pcie2TbtOffset,
  IN  UINT8                             Bus,
  IN  UINT8                             Device,
  IN  UINT8                             Function,
  IN  UINT32                            Timeout
);

/**
  Get Segment, Bus, Device and Function Number for ITBT DMA controller by Root Port physical number

  @param[in]  Controller                Controller number. (0-based)
  @param[out] Segment                   Return corresponding Tbt HIA Segment number.
  @param[out] Bus                       Return corresponding Tbt HIA Bus number.
  @param[out] Device                    Return corresponding Tbt HIA device number.
  @param[out] Function                  Return corresponding Tbt HIA function number.

  @retval     EFI_SUCCESS               Root port device and function is retrieved
**/
EFI_STATUS
EFIAPI
GetITbtDmaDevFun(
  IN  UINT8                             Controller,
  OUT UINTN                             *Segment,
  OUT UINTN                             *Bus,
  OUT UINTN                             *Device,
  OUT UINTN                             *Function
);

/**
  VGA Enable and VGA 16-bit decode registers of Bridge control register of Intergrated and Discrete Thunderbolt root port
  should be cleaned

  @param[in]  Segment   Segment number
  @param[in]  Bus       Bus number
  @param[in]  Device    Device number
  @param[in]  Function  Function number
**/
VOID
TbtClearVgaRegisters(
  IN  UINTN                             Segment,
  IN  UINTN                             Bus,
  IN  UINTN                             Device,
  IN  UINTN                             Function
);

/**
  Check ITBT HR FW ready bit is set.

  @param[in] DevAddr                      - USB4 host router device address.
  @param[in] ITbtForcePowerOnTimeoutInMs  - ITbt force power waiting time out in Ms.

  @retval EFI_SUCCESS            - Force power successfully.
  @retval EFI_UNSUPPORTED        - Device is not ready for force power.
  @retval EFI_TIMEOUT            - Device is not ready after timeout period.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
**/
EFI_STATUS
EFIAPI
ITbtCheckFwReadyBit (
  IN UINT64   DevAddr,
  IN UINT16   ITbtForcePowerOnTimeoutInMs
);

/**
  Force power to USB4 host router.

  @param[in] Segment                     - USB4 host router PCI Segment number.
  @param[in] Bus                         - USB4 host router PCI bus number.
  @param[in] Dev                         - USB4 host router PCI device number.
  @param[in] Func                        - USB4 host router PCI function number.
  @param[in] ITbtForcePowerOnTimeoutInMs - ITbt force power waiting time out in Ms.

  @retval EFI_SUCCESS           - Force power successfully.
  @retval EFI_UNSUPPORTED       - Device is not ready for force power.
  @retval EFI_TIMEOUT           - Device is not ready after timeout period.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
EFIAPI
ITbtSetHrForcePower (
  IN UINT16   Segment,
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Func,
  IN UINT16   ITbtForcePowerOnTimeoutInMs
);

/**
  Remove Force Power for USB4 host router

  @param[in] Segment  - USB4 host router PCI Segment number.
  @param[in] Bus      - USB4 host router PCI bus number.
  @param[in] Dev      - USB4 host router PCI device number.
  @param[in] Func     - USB4 host router PCI function number.

  @retval EFI_SUCCESS            - The function completes successfully
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
**/
EFI_STATUS
EFIAPI
ITbtUnsetHrForcePower (
  IN UINT16   Segment,
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Func
);

#endif
