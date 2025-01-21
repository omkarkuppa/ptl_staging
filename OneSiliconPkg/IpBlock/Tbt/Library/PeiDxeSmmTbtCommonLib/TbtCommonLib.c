/** @file
  The implementition of Tbt library shared between dTBT and iTBT.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#if FixedPcdGetBool(PcdTcssSupport) == 1
#include <Register/Usb4Regs.h>
#include <Register/HostDmaRegs.h>
#include <TcssInfo.h>
#endif
#include <Library/DebugLib.h>
#include <Uefi/UefiBaseType.h>
#include <Library/TbtCommonLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseLib.h>

/**
  Internal function to Wait for Tbt2PcieDone Bit.to Set or clear
  @param[in]  CommandOffsetAddress      Tbt2Pcie Register Address
  @param[in]  TimeOut                   The number of 1 ms units to delay
  @param[in]  Tbt2PcieDone              Wait condition (wait for Bit to Clear/Set)
  @param[out] *Tbt2PcieValue Function   Register value
  @retval     TRUE                      If command executes succesfully
**/
BOOLEAN
InternalWaitforCommandCompletion (
  IN  UINT64                            CommandOffsetAddress,
  IN  UINT32                            TimeOut,
  IN  BOOLEAN                           Tbt2PcieDone,
  OUT UINT32                            *Tbt2PcieValue
  )
{
  BOOLEAN                               ReturnFlag;
  UINT32                                Tbt2PcieCheck;
  UINT32                                Index;

  DEBUG ((DEBUG_INFO, "[TBT] Waiting for TBT controller to finish command\n"));
  ReturnFlag = FALSE;
  for (Index = 0; Index < TimeOut; Index++) {
    *Tbt2PcieValue = PciSegmentRead32 (CommandOffsetAddress);

    if (0xFFFFFFFF == *Tbt2PcieValue ) {
      //
      // Device is not here return now
      //
      ReturnFlag     = FALSE;
      break;
    }

    if(Tbt2PcieDone) {
      Tbt2PcieCheck  =  *Tbt2PcieValue & TBT2PCIE_DON_R;
    } else {
      Tbt2PcieCheck  = !(*Tbt2PcieValue & TBT2PCIE_DON_R);
    }

    if (Tbt2PcieCheck) {
      ReturnFlag     = TRUE;
      break;
    }

    MicroSecondDelay (TBT_MAILBOX_DELAY);
  }

  if (Index >= TimeOut) {
    DEBUG ((DEBUG_WARN, "[TBT] Command timeout!\n"));
  }
  return ReturnFlag;
}

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
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  UINT8        ReturnFlag;

  ReturnFlag           = 0xFF;

  DEBUG ((DEBUG_INFO, "GetSecLevel(), Cmd = 0x%x \n", Command));

  Tbt2Pcie = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Tbt2PcieOffset);
  Pcie2Tbt = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Pcie2TbtOffset);

  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  if(InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue)) {
    ReturnFlag     = (UINT8) (0xFF & (RegisterValue >> 8));
  }

  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Security Level configured to %x \n", ReturnFlag));

  return ReturnFlag;
}

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
  )
{
  UINT64       Pcie2Tbt;
  UINT64       Tbt2Pcie;
  UINT32       RegisterValue;
  BOOLEAN      ReturnFlag;

  ReturnFlag   = FALSE;

  DEBUG ((DEBUG_INFO, "SetSecLevel(), Cmd = 0x%x\n", Command));

  Tbt2Pcie = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Tbt2PcieOffset);
  Pcie2Tbt = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Pcie2TbtOffset);

  PciSegmentWrite32 (Pcie2Tbt, (Data << 8) | Command | PCIE2TBT_VLD_B);

  ReturnFlag = InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, TRUE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "RegisterValue %x \n", RegisterValue));
  PciSegmentWrite32 (Pcie2Tbt, 0);

  InternalWaitforCommandCompletion(Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  DEBUG ((DEBUG_INFO, "Return value %x \n", ReturnFlag));
  return ReturnFlag;
}

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
  @param[in]  Timeout                   Time out with 1 ms granularity
  @retval     TRUE                      If command executes successfully
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
)
{
  UINT64                                Pcie2Tbt;
  UINT64                                Tbt2Pcie;
  UINT32                                RegisterValue;
  BOOLEAN                               ReturnFlag;

  Tbt2Pcie = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Tbt2PcieOffset);
  Pcie2Tbt = PCI_SEGMENT_LIB_ADDRESS(0, Bus, Device, Function, Pcie2TbtOffset);

  DEBUG ((DEBUG_INFO, "[TBT] Send command %x to controller \n", Command));

  ///
  /// Load PCIE2TBT register with command and set VALID bit of PCIE2TBT register to indicate a new command sent, generates
  /// interrupt to LC and CM.
  ///
  PciSegmentWrite32 (Pcie2Tbt, Command | PCIE2TBT_VLD_B);

  ///
  /// Wait for DONE bit set by LC acknowledging the command sent from the host before timeout
  ///
  ReturnFlag = InternalWaitforCommandCompletion (Tbt2Pcie, Timeout, TRUE, &RegisterValue);

  ///
  /// Clear PCIE2TBT register valid bit which should be cleared by host SW after command done
  ///
  PciSegmentWrite32(Pcie2Tbt, 0);

  //
  /// Wait for DONE bit cleared by LC if it is iTBT controller
  ///
  if (Tbt2PcieOffset == FixedPcdGet8 (PcdITbtToPcieRegister)) {
    ReturnFlag = InternalWaitforCommandCompletion (Tbt2Pcie, Timeout, FALSE, &RegisterValue);
  }

  return ReturnFlag;
}

/**
  Get Segment, Bus, Device and Function Number for TBT DMA controller by Root Port physical number

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
)
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  if (Controller >= (MAX_ITBT_PCIE_PORT / 2)) {
    *Segment  = 0xFF;
    *Bus      = 0xFF;
    *Device   = 0xFF;
    *Function = 0xFF;
    return EFI_NOT_FOUND;
  } else {
    *Segment  = 0;
    *Bus      = HOST_DMA_BUS_NUM;
    *Device   = HOST_DMA_DEV_NUM;
    *Function = HOST_DMA0_FUN_NUM + (UINTN) Controller;
    return EFI_SUCCESS;
  }
#else
  return EFI_NOT_FOUND;
#endif
}

/**
  VGA Enable and VGA 16-bit decode registers of Bridge control register of Integrated and Discrete Thunderbolt root port
  should be cleaned

  @param[in]  Segment                   Segment number
  @param[in]  Bus                       Bus number
  @param[in]  Device                    Device number
  @param[in]  Function                  Function number
**/
VOID
TbtClearVgaRegisters(
  IN  UINTN                             Segment,
  IN  UINTN                             Bus,
  IN  UINTN                             Device,
  IN  UINTN                             Function
)
{
  UINT64                                BridngeBaseAddress;
  UINT16                                Data16;

  //
  // Get the TBT Root Port Base Address
  //
  BridngeBaseAddress = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, (UINT32)Device, (UINT32)Function, 0);
  Data16 = PciSegmentRead16 (BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET);
  Data16 &= (~(EFI_PCI_BRIDGE_CONTROL_VGA | EFI_PCI_BRIDGE_CONTROL_VGA_16));
  PciSegmentWrite16 (BridngeBaseAddress + PCI_BRIDGE_CONTROL_REGISTER_OFFSET, Data16);
}

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
  )
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  EFI_STATUS  Status;
  UINT32      Index;
  UINT32      RegVal;

  RegVal = PciSegmentRead32 (DevAddr);
  if (RegVal == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "ITbtUnsetHrForcePower: Invalid Device ID\n"));
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < ITbtForcePowerOnTimeoutInMs; Index++) {
    RegVal = PciSegmentRead32 (DevAddr + R_USB4_CFG_TBT_DMA_CFG_VS_CAP_9);
    if ((RegVal & B_USB4_CFG_TBT_DMA_CFG_VS_CAP_9_FW_READY) == B_USB4_CFG_TBT_DMA_CFG_VS_CAP_9_FW_READY) {
      Status = EFI_SUCCESS;
      DEBUG ((DEBUG_INFO, "ITBT HR NVM FW version = 0x%08X, retry = %d\n", RegVal, Index));
      break;
    }
  }

  if (Index >= ITbtForcePowerOnTimeoutInMs) {
    DEBUG ((DEBUG_ERROR, "ITBT HR is not ready, RegVal = 0x%x\n", RegVal));
    Status = EFI_TIMEOUT;
  }

  return Status;
#else
  return EFI_UNSUPPORTED;
#endif
}

/**
  Force power to USB4 host router.

  @param[in] Segment                      - USB4 host router PCI Segment number.
  @param[in] Bus                          - USB4 host router PCI bus number.
  @param[in] Dev                          - USB4 host router PCI device number.
  @param[in] Func                         - USB4 host router PCI function number.
  @param[in] ITbtForcePowerOnTimeoutInMs  - ITbt force power waiting time out in Ms.

  @retval EFI_SUCCESS            - Force power successfully.
  @retval EFI_UNSUPPORTED        - Device is not ready for force power.
  @retval EFI_TIMEOUT            - Device is not ready after timeout period.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
**/
EFI_STATUS
EFIAPI
ITbtSetHrForcePower (
  IN UINT16   Segment,
  IN UINT8    Bus,
  IN UINT8    Dev,
  IN UINT8    Func,
  IN UINT16   ITbtForcePowerOnTimeoutInMs
  )
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  EFI_STATUS  Status;
  UINT32      RegVal;
  UINT64      DevAddr;

  DEBUG ((DEBUG_INFO, "ITbtSetHrForcePower entry - BDF = (0x%02x,0x%02x,0x%02x)\n", Bus, Dev, Func));

  DevAddr = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Dev, Func, 0);
  RegVal = PciSegmentRead32 (DevAddr);
  if (RegVal == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "ITbtSetHrForcePower: Invalid Device ID\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Set ITBT host router force power
  //
  PciSegmentAndThenOr32 (
    DevAddr + R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22,
    (UINT32) ~(B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_FORCE_POWER),
    B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_FORCE_POWER | (V_HOST_DMA_DMA_ACTIVE_DELAY_VALUE << N_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_DMA_ACTIVE_DELAY)
  );

  //
  // Polling NVM FW version and check FW ready bit
  //
  Status = ITbtCheckFwReadyBit (DevAddr, ITbtForcePowerOnTimeoutInMs);

  return Status;
#else
  return EFI_UNSUPPORTED;
#endif
}

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
  )
{
#if FixedPcdGetBool(PcdTcssSupport) == 1
  UINT32      RegVal;
  UINT64      DevAddr;

  DEBUG ((DEBUG_INFO, "ITbtUnsetHrForcePower entry - BDF = (0x%02x,0x%02x,0x%02x)\n", Bus, Dev, Func));

  DevAddr = PCI_SEGMENT_LIB_ADDRESS (Segment, Bus, Dev, Func, 0);
  RegVal = PciSegmentRead32 (DevAddr);
  if (RegVal == 0xFFFFFFFF) {
    DEBUG ((DEBUG_ERROR, "ITbtUnsetHrForcePower: Invalid Device ID\n"));
    return EFI_INVALID_PARAMETER;
  }

  //
  // Un-power the ITBT host router
  //
  PciSegmentAnd8 (DevAddr + R_USB4_CFG_TBT_DMA_CFG_VS_CAP_22, (UINT8) (~B_USB4_CFG_TBT_DMA_CFG_VS_CAP_22_FORCE_POWER));

  return EFI_SUCCESS;
#else
  return EFI_UNSUPPORTED;
#endif
}