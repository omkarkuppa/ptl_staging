/** @file
  Touch Host Controller QuickI2c Driver API

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

#include "QuickI2cLibPrivate.h"
#include "QuickI2cAbsPtr.h"
#include "QuickI2cHid.h"
#include "QuickI2cSubIP.h"
#include "QuickI2cDriver.h"

#include <Protocol/ThcResetProtocol.h>


/**
  Prints end Device Configuration

  @param[in]  QuickI2cDev      Context of QuickI2c device
**/
VOID
QuickI2cPrintDeviceDescriptor (
  IN  QUICK_I2C_DEV    *QuickI2cDev
  )
{
  THC_LOCAL_DEBUG (L"QuickI2cPrintDeviceDescriptor () Start\n")
  THC_LOCAL_DEBUG (L"| 0x00 | wDeviceDescLength        | 0x%X\n", QuickI2cDev->DeviceDescriptor.wDeviceDescLength)
  THC_LOCAL_DEBUG (L"| 0x02 | bcdVersion               | 0x%X\n", QuickI2cDev->DeviceDescriptor.bcdVersion)
  THC_LOCAL_DEBUG (L"| 0x04 | wReportDescLength        | 0x%X\n", QuickI2cDev->DeviceDescriptor.wReportDescLength)
  THC_LOCAL_DEBUG (L"| 0x06 | wReportDescRegister      | 0x%X\n", QuickI2cDev->DeviceDescriptor.wReportDescRegister)
  THC_LOCAL_DEBUG (L"| 0x06 | wInputRegister           | 0x%X\n", QuickI2cDev->DeviceDescriptor.wInputRegister)
  THC_LOCAL_DEBUG (L"| 0x08 | wMaxInputLength          | 0x%X\n", QuickI2cDev->DeviceDescriptor.wMaxInputLength)
  THC_LOCAL_DEBUG (L"| 0x0A | wOutputRegister          | 0x%X\n", QuickI2cDev->DeviceDescriptor.wOutputRegister)
  THC_LOCAL_DEBUG (L"| 0x0C | wMaxOutputLength         | 0x%X\n", QuickI2cDev->DeviceDescriptor.wMaxOutputLength)
  THC_LOCAL_DEBUG (L"| 0x0E | wCommandRegister         | 0x%X\n", QuickI2cDev->DeviceDescriptor.wCommandRegister)
  THC_LOCAL_DEBUG (L"| 0x10 | wDataRegister            | 0x%X\n", QuickI2cDev->DeviceDescriptor.wDataRegister)
  THC_LOCAL_DEBUG (L"| 0x12 | wVendorID                | 0x%X\n", QuickI2cDev->DeviceDescriptor.wVendorID)
  THC_LOCAL_DEBUG (L"| 0x14 | wProductID               | 0x%X\n", QuickI2cDev->DeviceDescriptor.wProductID)
  THC_LOCAL_DEBUG (L"| 0x16 | wVersionID               | 0x%X\n", QuickI2cDev->DeviceDescriptor.wVersionID)
  THC_LOCAL_DEBUG (L"| 0x18 | wFlags.NoOutputReportAck | 0x%X\n", QuickI2cDev->DeviceDescriptor.wFlags.Fields.NoOutputReportAck)
  THC_LOCAL_DEBUG (L"| 0x20 | Reserved                 | 0x%X\n", QuickI2cDev->DeviceDescriptor.Reserved)
  THC_LOCAL_DEBUG (L"THC HID Protocol                  | %a  \n", (QuickI2cDev->HidSolutionFlag == ElanHidProtocol) ? "ELAN" : "WACOM")
  THC_LOCAL_DEBUG (L"QuickI2cPrintDeviceDescriptor () End\n")
  DEBUG ((DEBUG_INFO, "QuickI2cPrintDeviceDescriptor () Start\n"));
  DEBUG ((DEBUG_INFO, "| 0x00 | wDeviceDescLength        | 0x%X\n", QuickI2cDev->DeviceDescriptor.wDeviceDescLength));
  DEBUG ((DEBUG_INFO, "| 0x02 | bcdVersion               | 0x%X\n", QuickI2cDev->DeviceDescriptor.bcdVersion));
  DEBUG ((DEBUG_INFO, "| 0x04 | wReportDescLength        | 0x%X\n", QuickI2cDev->DeviceDescriptor.wReportDescLength));
  DEBUG ((DEBUG_INFO, "| 0x06 | wReportDescRegister      | 0x%X\n", QuickI2cDev->DeviceDescriptor.wReportDescRegister));
  DEBUG ((DEBUG_INFO, "| 0x06 | wInputRegister           | 0x%X\n", QuickI2cDev->DeviceDescriptor.wInputRegister));
  DEBUG ((DEBUG_INFO, "| 0x08 | wMaxInputLength          | 0x%X\n", QuickI2cDev->DeviceDescriptor.wMaxInputLength));
  DEBUG ((DEBUG_INFO, "| 0x0A | wOutputRegister          | 0x%X\n", QuickI2cDev->DeviceDescriptor.wOutputRegister));
  DEBUG ((DEBUG_INFO, "| 0x0C | wMaxOutputLength         | 0x%X\n", QuickI2cDev->DeviceDescriptor.wMaxOutputLength));
  DEBUG ((DEBUG_INFO, "| 0x0E | wCommandRegister         | 0x%X\n", QuickI2cDev->DeviceDescriptor.wCommandRegister));
  DEBUG ((DEBUG_INFO, "| 0x10 | wDataRegister            | 0x%X\n", QuickI2cDev->DeviceDescriptor.wDataRegister));
  DEBUG ((DEBUG_INFO, "| 0x12 | wVendorID                | 0x%X\n", QuickI2cDev->DeviceDescriptor.wVendorID));
  DEBUG ((DEBUG_INFO, "| 0x14 | wProductID               | 0x%X\n", QuickI2cDev->DeviceDescriptor.wProductID));
  DEBUG ((DEBUG_INFO, "| 0x16 | wVersionID               | 0x%X\n", QuickI2cDev->DeviceDescriptor.wVersionID));
  DEBUG ((DEBUG_INFO, "| 0x18 | wFlags.NoOutputReportAck | 0x%X\n", QuickI2cDev->DeviceDescriptor.wFlags.Fields.NoOutputReportAck));
  DEBUG ((DEBUG_INFO, "| 0x20 | Reserved                 | 0x%X\n", QuickI2cDev->DeviceDescriptor.Reserved));
  DEBUG ((DEBUG_INFO, " THC HID Protocol                 | %a  \n", (QuickI2cDev->HidSolutionFlag == ElanHidProtocol) ? "ELAN" : "WACOM"));
}

/**
  Initialize QuickI2c according to procedure described in EDS

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidOverI2c       Hid Over I2C policy data
  @param[in]  Reset            Reset data

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cInitialize (
  IN QUICK_I2C_DEV        *QuickI2cDev,
  IN THC_HID_OVER_I2C     *HidOverI2c,
  IN THC_RESET            *Reset
  )
{
  EFI_STATUS              Status;
  UINT32                  QuickI2cSpeedBasedConfigVar;
  THC_I2C_IC_CON          IcCon;
  THC_I2C_IC_ENABLE       IcEnable;
  THC_I2C_IC_TAR          IcTar;
  THC_I2C_IC_SDA_HOLD     IcSdaHold;
  THC_I2C_IC_SS_SCL_HCNT  IcSsSclHcnt;
  THC_I2C_IC_SS_SCL_LCNT  IcSsSclLcnt;
  THC_I2C_IC_FS_SCL_HCNT  IcFsSclHcnt;
  THC_I2C_IC_FS_SCL_LCNT  IcFsSclLcnt;
  THC_I2C_IC_HS_SCL_HCNT  IcHsSclHcnt;
  THC_I2C_IC_HS_SCL_LCNT  IcHsSclLcnt;
  THC_I2C_IC_INTR_MASK    IcIntrMask;
  THC_I2C_IC_RX_TL        IcRxTl;
  THC_I2C_IC_TX_TL        IcTxTl;
  THC_I2C_IC_DMA_CR       IcDmaCr;
  THC_I2C_IC_DMA_TDLR     IcDmaTdlr;
  THC_I2C_IC_DMA_RDLR     IcDmaRdlr;

  THC_LOCAL_DEBUG (L"QuickI2cInitialize Start ()\n")

  //
  // Any speed configuration's LCNT and HCNT should have valid values otherwise set default values
  //
  if (((HidOverI2c->StandardModeSerialClockLineHighPeriod      == 0) || (HidOverI2c->StandardModeSerialClockLineLowPeriod      == 0)) &&
      ((HidOverI2c->FastModeSerialClockLineHighPeriod          == 0) || (HidOverI2c->FastModeSerialClockLineLowPeriod          == 0)) &&
      ((HidOverI2c->FastModePlusSerialClockLineHighPeriod      == 0) || (HidOverI2c->FastModePlusSerialClockLineLowPeriod      == 0)) &&
      ((HidOverI2c->HighSpeedModePlusSerialClockLineHighPeriod == 0) || (HidOverI2c->HighSpeedModePlusSerialClockLineLowPeriod == 0))) {
    DEBUG ((DEBUG_WARN, "QuickI2cInitialize configuring default policy\n"));
    QuickI2cLibConfigureDefaultPolicy (HidOverI2c);
  }

  //
  // Step 1- Set port type to I2C
  //
  QuickI2cLibSetI2cPort (QuickI2cDev->PciBar0);

  //
  // Step 2- All write operations are preceded by reads
  // Write to IC_ENABLE register to disable
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_ENABLE, &IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return Status;
  }
  IcEnable.Fields.Enable = 0;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_ENABLE, IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 3- Program IC_CON register fields
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_CON, &IcCon.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_CON error, Status %r\n", Status));
    return Status;
  }

  //
  // Decide I2C bus speed
  //
  if ((HidOverI2c->ConnectionSpeed > (UINT32) 0) && (HidOverI2c->ConnectionSpeed <= (UINT32) QUICK_I2C_STANDARD_MODE_MAX_SPEED)) {
    THC_LOCAL_DEBUG(L"QuickI2cInitialize ConnectionSpeed to set is : 0x%X, Its in standard range, Setting I2cSpeedStandard \n", HidOverI2c->ConnectionSpeed);
    // Bit 1-2 --> SPEED
    IcCon.Fields.Speed = I2C_SPEED_STANDARD;
    QuickI2cSpeedBasedConfigVar = I2cSpeedStandard;
  } else if ((HidOverI2c->ConnectionSpeed > (UINT32) QUICK_I2C_STANDARD_MODE_MAX_SPEED) && (HidOverI2c->ConnectionSpeed <= (UINT32) QUICK_I2C_FAST_MODE_MAX_SPEED)) {
    THC_LOCAL_DEBUG(L"QuickI2cInitialize ConnectionSpeed to set is : 0x%X, Its in fast range, Setting I2cSpeedFast \n", HidOverI2c->ConnectionSpeed);
    // Bit 1-2 --> SPEED
    IcCon.Fields.Speed = I2C_SPEED_FAST_FAST_PLUS;
    QuickI2cSpeedBasedConfigVar = I2cSpeedFast;
  } else if ((HidOverI2c->ConnectionSpeed > (UINT32) QUICK_I2C_FAST_MODE_MAX_SPEED) && (HidOverI2c->ConnectionSpeed <= (UINT32) QUICK_I2C_FASTPLUS_MODE_MAX_SPEED)) {
    THC_LOCAL_DEBUG(L"QuickI2cInitialize ConnectionSpeed to set is : 0x%X, Its in fast plus range, Setting I2cSpeedFastPlus \n", HidOverI2c->ConnectionSpeed);
    // Bit 1-2 --> SPEED
    IcCon.Fields.Speed = I2C_SPEED_FAST_FAST_PLUS;
    QuickI2cSpeedBasedConfigVar = I2cSpeedFastPlus;
  } else if ((HidOverI2c->ConnectionSpeed > (UINT32) QUICK_I2C_FASTPLUS_MODE_MAX_SPEED) && (HidOverI2c->ConnectionSpeed <= (UINT32) QUICK_I2C_HIGH_SPEED_MODE_MAX_SPEED)) {
    THC_LOCAL_DEBUG(L"QuickI2cInitialize ConnectionSpeed to set is : 0x%X, Its in high speed range, Setting I2cSpeedHigh \n", HidOverI2c->ConnectionSpeed);
    // Bit 1-2 --> SPEED
    IcCon.Fields.Speed = I2C_SPEED_HIGH;
    QuickI2cSpeedBasedConfigVar = I2cSpeedHigh;
  } else {
    THC_LOCAL_DEBUG (L"QuickI2cInitialize ConnectionSpeed value is invalid so setting I2cSpeedStandard \n");
    // Bit 1-2 --> SPEED
    IcCon.Fields.Speed = I2C_SPEED_STANDARD;
    QuickI2cSpeedBasedConfigVar = I2cSpeedStandard;
  }

  // Bit 0 --> MASTER_MODE set to 1
  IcCon.Fields.MasterMode = 1;
  // Bit 1-2 --> SPEED
  // Bit 3 --> IC_10BITADDR_SLAVE set to 0 (7 bit addressing)
  IcCon.Fields.Ic10BitAddrSlave = 0;
  // Bit 4 --> IC_10BITADDR_MASTER set to 0 (7 bit addressing)
  IcCon.Fields.Ic10BitAddrMaster = 0;
  // Bit 5 --> IC_RESTART_EN set to 1
  IcCon.Fields.IcRestartEn = 1;
  // Bit 6 --> IC_SLAVE_DISABLE set to 1 (not in slave mode)
  IcCon.Fields.IcSlaveDisable = 1;
  // Bit 7 --> STOP_DET_IFADDRESSED set to 0 (issues STOP_DET irrespective of whether it's addressed or not)
  IcCon.Fields.StopDetIfAddressed = 0;
  // Bit 8 --> TX_EMPTY_CTRL set to 0
  IcCon.Fields.TxEmptyCtrl = 0;
  // Bit 9 --> RX_FIFO_FULL_HLD_CTRL set to 1
  IcCon.Fields.RxFifoFullHldCtrl = 1;
  // Bit 10 --> STOP_DET_IF_MASTER_ACTIVE set to 1
  IcCon.Fields.StopDetIfMasterActive = 1;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_CON, IcCon.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_CON error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 4 - Set address of target slave by writing it to TAR
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_TAR, &IcTar.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_TAR error, Status %r\n", Status));
    return Status;
  }
  IcTar.Fields.IcTar = HidOverI2c->DeviceAddress;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_TAR, IcTar.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_TAR error, Status %r\n", Status));
    return Status;
  }

  //
  // Decide which HCNT/LCNT registers to set based on bus speed value
  //
  switch (QuickI2cSpeedBasedConfigVar) {
    case I2cSpeedStandard:
      //
      // SS HCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SS_SCL_HCNT, &IcSsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }
      IcSsSclHcnt.Fields.IcSsSclHcnt = HidOverI2c->StandardModeSerialClockLineHighPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SS_SCL_HCNT, IcSsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // SS LCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SS_SCL_LCNT, &IcSsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }
      IcSsSclLcnt.Fields.IcSsSclLcnt = HidOverI2c->StandardModeSerialClockLineLowPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SS_SCL_LCNT, IcSsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // SS SDA HOLD
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
      // To make sure it is programmed with default settings only if not programmed by user
      IcSdaHold.Fields.IcSdaTxHold = (HidOverI2c->StandardModeSerialDataLineTransmitHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_TX_100_1000_KHZ
                                     : HidOverI2c->StandardModeSerialDataLineTransmitHoldPeriod;
      IcSdaHold.Fields.IcSdaRxHold = (HidOverI2c->StandardModeSerialDataLineReceiveHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_RX_100_1000_KHZ
                                     : HidOverI2c->StandardModeSerialDataLineReceiveHoldPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }

    break;

    case I2cSpeedFast:
      //
      // FS HCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_HCNT, &IcFsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }
      IcFsSclHcnt.Fields.IcFsSclHcnt = HidOverI2c->FastModeSerialClockLineHighPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_HCNT, IcFsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // FS LCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_LCNT, &IcFsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }
      IcFsSclLcnt.Fields.IcFsSclLcnt = HidOverI2c->FastModeSerialClockLineLowPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_LCNT, IcFsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // FS SDA HOLD / FS Plus
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
      // To make sure it is programmed with default settings only if not programmed by user
      IcSdaHold.Fields.IcSdaTxHold = (HidOverI2c->FastModeSerialDataLineTransmitHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_TX_1000_3400_KHZ
                                     : HidOverI2c->FastModeSerialDataLineTransmitHoldPeriod;
      IcSdaHold.Fields.IcSdaRxHold = (HidOverI2c->FastModeSerialDataLineReceiveHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_RX_1000_3400_KHZ
                                     : HidOverI2c->FastModeSerialDataLineReceiveHoldPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
    break;

    case I2cSpeedFastPlus:
      //
      // FS Plus HCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_HCNT, &IcFsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }
      IcFsSclHcnt.Fields.IcFsSclHcnt = HidOverI2c->FastModePlusSerialClockLineHighPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_HCNT, IcFsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_FS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // FS Plus LCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_LCNT, &IcFsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }
      IcFsSclLcnt.Fields.IcFsSclLcnt = HidOverI2c->FastModePlusSerialClockLineLowPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_FS_SCL_LCNT, IcFsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_FS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // FS Plus SDA HOLD
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
      // To make sure it is programmed with default settings only if not programmed by user
      IcSdaHold.Fields.IcSdaTxHold = (HidOverI2c->FastModePlusSerialDataLineTransmitHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_TX_1000_3400_KHZ
                                     : HidOverI2c->FastModePlusSerialDataLineTransmitHoldPeriod;
      IcSdaHold.Fields.IcSdaRxHold = (HidOverI2c->FastModePlusSerialDataLineReceiveHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_RX_1000_3400_KHZ
                                     : HidOverI2c->FastModePlusSerialDataLineReceiveHoldPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
    break;

    case I2cSpeedHigh:
      //
      // HS HCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_HS_SCL_HCNT, &IcHsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_HS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }
      IcHsSclHcnt.Fields.IcHsSclHcnt = HidOverI2c->HighSpeedModePlusSerialClockLineHighPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_HS_SCL_HCNT, IcHsSclHcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_HS_SCL_HCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // HS LCNT
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_HS_SCL_LCNT, &IcHsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_HS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }
      IcHsSclLcnt.Fields.IcHsSclLcnt = HidOverI2c->HighSpeedModePlusSerialClockLineLowPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_HS_SCL_LCNT, IcHsSclLcnt.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_HS_SCL_LCNT error, Status %r\n", Status));
        return Status;
      }

      //
      // HS SDA HOLD
      //
      Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, &IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibReadSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }
      // To make sure it is programmed with default settings only if not programmed by user
      IcSdaHold.Fields.IcSdaTxHold = (HidOverI2c->HighSpeedModePlusSerialDataLineTransmitHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_TX_3400_INF_KHZ
                                     : HidOverI2c->HighSpeedModePlusSerialDataLineTransmitHoldPeriod;
      IcSdaHold.Fields.IcSdaRxHold = (HidOverI2c->HighSpeedModePlusSerialDataLineReceiveHoldPeriod == 0)
                                     ? QUICK_I2C_MEM_SDA_RX_3400_INF_KHZ
                                     : HidOverI2c->HighSpeedModePlusSerialDataLineReceiveHoldPeriod;
      Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_SDA_HOLD, IcSdaHold.Data32);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_SDA_HOLD error, Status %r\n", Status));
        return Status;
      }

    break;
  }

  //
  // Step 7. Write to IC_INTR_MASK to enable all
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_INTR_MASK, &IcIntrMask.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_INTR_MASK error, Status %r\n", Status));
    return Status;
  }
  IcIntrMask.Data32 = QUICK_I2C_INTR_MASK_DEFAULT;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_INTR_MASK, IcIntrMask.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_INTR_MASK error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 8. Write to IC_RX_TL to set Rx FIFO threshold level
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_RX_TL, &IcRxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_RX_TL error, Status %r\n", Status));
    return Status;
  }
  IcRxTl.Fields.RxTl = QUICK_I2C_DEFAULT_RX_TL;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_RX_TL, IcRxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_RX_TL error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 9. Write to IC_TX_TL to set Tx FIFO threshold level
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_TX_TL, &IcTxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_TX_TL error, Status %r\n", Status));
    return Status;
  }
  IcTxTl.Fields.TxTl = QUICK_I2C_DEFAULT_TX_TL;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_TX_TL, IcTxTl.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_TX_TL error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 10. Write to IC_DMA_CR to set DMA control
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_CR, &IcDmaCr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_CR error, Status %r\n", Status));
    return Status;
  }
  IcDmaCr.Data32 = QUICK_I2C_DEFAULT_DMA_CR;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_CR, IcDmaCr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_CR error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 11. Write to IC_DMA_TDLR to set DMA transmit data level
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_TDLR, &IcDmaTdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_TDLR error, Status %r\n", Status));
    return Status;
  }
  IcDmaTdlr.Fields.Dmatdl = QUICK_I2C_DEFAULT_DMA_TDLR;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_TDLR, IcDmaTdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_TDLR error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 12. Write to IC_DMA_RDLR to set DMA receive data level
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_RDLR, &IcDmaRdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_RDLR error, Status %r\n", Status));
    return Status;
  }
  IcDmaRdlr.Fields.Dmadrl = QUICK_I2C_DEFAULT_DMA_RDLR;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_DMA_RDLR, IcDmaRdlr.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_DMA_RDLR error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 13. Write to IC_ENABLE to enable I2C protocol IO
  //
  Status = QuickI2cLibReadSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_ENABLE, &IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return Status;
  }
  IcEnable.Fields.Enable = 1;
  IcEnable.Fields.SdaStuckRecoveryEnable = 1;
  Status = QuickI2cLibWriteSubIpRegister (QuickI2cDev->PciBar0, THC_I2C_REG_IC_ENABLE, IcEnable.Data32);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cInitialize QuickI2cLibWriteSubIpRegister THC_I2C_REG_IC_ENABLE error, Status %r\n", Status));
    return Status;
  }

  //
  // Step 14- Power up device through RST GPIO only for ResetPadTrigger LOW case
  //
if (!Reset->ResetPadTrigger) {
      //
      // Power up device through RST GPIO, if not already by default RST line should be high
      //
    Status = QuickI2cLibGpioGetOutOfReset (Reset->ResetSequencingDelay, QuickI2cDev->InstanceId, Reset->ResetPadTrigger);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickI2cInitialize QuickI2cLibGpioGetOutOfReset error status: %r\n", Status));
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  Reads Touch Panels Device registers (TouchCapabilities, TouchId etc.)

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  HidOverI2c       Hid Over I2c Context

  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cReadDeviceDescriptor (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c
  )
{
  EFI_STATUS                      Status;
  UINT16                          WriteSize;
  UINT16                          ReadSize;
  UINT32                          WriteData;
  UINT32                          DeviceDescriptorAddressValue;

  WriteSize                     = QUICK_I2C_DEFAULT_DEVICE_DESCRIPTOR_ADDRESS_LENGTH;
  DeviceDescriptorAddressValue  = HidOverI2c->DeviceDescriptorAddress;
  ReadSize                      = QUICK_I2C_DEFAULT_DEVICE_DESCRIPTOR_LENGTH;
  WriteData                     = QuickI2cOutputReportDeviceDescriptor;

  Status = QuickI2cLibNoCyclesPending (QuickI2cDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG (L"QuickI2cReadDeviceDescriptor: QuickI2cLibNoCyclesPending Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cLibNoCyclesPending error, Status %r\n", Status));
    return Status;
  }

  THC_LOCAL_DEBUG (L"QuickI2cReadDeviceDescriptor DeviceDescriptorAddressValue - 0x%x\n", DeviceDescriptorAddressValue)

  QuickI2cLibPrepareRwPioOperation (
    QuickI2cWriteReadCommand,
    QuickI2cDev->PciBar0,
    ReadSize,
    WriteSize,
    DeviceDescriptorAddressValue
    );

  QuickI2cLibWriteSeqData1 (QuickI2cDev->PciBar0, WriteData);
  QuickI2cLibGo (QuickI2cDev->PciBar0);

  Status = QuickI2cLibWaitCycleCompleted (QuickI2cDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG (L"QuickI2cReadDeviceDescriptor:  QuickI2cLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cLibWaitCycleCompleted: error, Status %r\n", Status));
    return Status;
  }

  THC_LOCAL_DEBUG (L"QuickI2cReadDeviceDescriptor: QuickI2cLibReadComplete Start()\n")
  Status =  QuickI2cLibReadComplete (QuickI2cDev->PciBar0, (UINT32*) &QuickI2cDev->DeviceDescriptor);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cLibReadComplete error, Status %r\n", Status));
    return Status;
  }

  //
  // Validate Device Descriptor
  //
  if ((QuickI2cDev->DeviceDescriptor.wDeviceDescLength != 0x1E) ||
     (QuickI2cDev->DeviceDescriptor.wCommandRegister   == 0x00) ||
     (QuickI2cDev->DeviceDescriptor.wVendorID          == 0x00) ||
     (QuickI2cDev->DeviceDescriptor.wProductID         == 0x00) ||
     (QuickI2cDev->DeviceDescriptor.wCommandRegister   == 0xFF) ||
     (QuickI2cDev->DeviceDescriptor.wVendorID          == 0xFF) ||
     (QuickI2cDev->DeviceDescriptor.wProductID         == 0xFF) ||
     (QuickI2cDev->DeviceDescriptor.wProductID         == QUICK_I2C_INVALID_PRODUCT_ID)) {
    DEBUG ((DEBUG_WARN, "QuickI2cReadDeviceDescriptor: ERROR - Validate Failed <0x%x>\n", QuickI2cDev->DeviceDescriptor.wProductID));
    THC_LOCAL_DEBUG (L"QuickI2cReadDeviceDescriptor: ERROR - Validate Failed <0x%x>\n", QuickI2cDev->DeviceDescriptor.wProductID)
    return EFI_DEVICE_ERROR;
  }

  QuickI2cDev->HidSolutionFlag = (QuickI2cDev->DeviceDescriptor.wVendorID == THC_HID_ELAN_VID)
                                 ? ElanHidProtocol : WacomHidProtocol;

  return Status;
}


/**
  Perform disable interrupt
  @param[in]  QuickI2cDev      Context of QuickI2c device
  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cDisableInterrupt (
  IN QUICK_I2C_DEV      *QuickI2cDev
  )
{
  EFI_STATUS            Status;
  //
  // Clear Global error and stall
  //
  if (QuickI2cLibIsQuiesceDisabled (QuickI2cDev->PciBar0)) {
    THC_LOCAL_DEBUG (L"QuickI2cDisableInterrupt QuiesceDisabled \n")
    Status = QuickI2cLibStartQuiesce (QuickI2cDev->PciBar0, StartQuiesceTimeout);
    THC_LOCAL_DEBUG (L"QuickI2cDisableInterrupt QuickI2cLibStartQuiesce Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickI2cDisableInterrupt QuickI2cLibStartQuiesce error, Status %r\n", Status));
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  Perform enable interrupt
  @param[in]  QuickI2cDev      Context of QuickI2c device
  @retval EFI_SUCCESS     QuickI2c initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cEnableInterrupt (
  IN QUICK_I2C_DEV      *QuickI2cDev
  )
{
  EFI_STATUS            Status;

  if (QuickI2cDev->InitProcessDoneEnableInterrupt) {
    if (QuickI2cLibIsQuiesceEnabled (QuickI2cDev->PciBar0)) {
      THC_LOCAL_DEBUG (L"QuickI2cEnableInterrupt QuiesceEnabled \n")
      Status = QuickI2cLibEndQuiesce (QuickI2cDev->PciBar0, EndQuiesceTimeout);
      THC_LOCAL_DEBUG (L"QuickI2cEnableInterrupt QuickI2cLibEndQuiesce Status: %r\n", Status)
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_WARN, "QuickI2cEnableInterrupt QuickI2cLibEndQuiesce error, Status %r\n", Status));
        return Status;
      }
    }
  }
  return EFI_SUCCESS;
}

/**
  Power up device through I2C Power request and reset command

  @param[in] QuickI2cDev       QuickI2c MMIO BAR0

**/
EFI_STATUS
QuickI2cLibGetOutOfReset (
  IN QUICK_I2C_DEV             *QuickI2cDev
  )
{
  EFI_STATUS   Status;

  THC_LOCAL_DEBUG (L"QuickI2cLibGetOutOfReset: Entry() InstanceId- %d\n", QuickI2cDev->InstanceId)

  Status = QuickI2cSendSetPowerRequest (QuickI2cDev, (UINT8) QuickI2cSubIpPowerStateOn);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSendSetPowerRequest Status - %r\n", Status);
    DEBUG ((DEBUG_WARN, "QuickI2cSendSetPowerRequest: ERROR Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cSendResetRequest (QuickI2cDev);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest returned Status- %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cSendResetRequest: ERROR Status %r\n", Status));
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Initialize Memory for Prd used by the HW
  Only ReadRx2 and Write are supported by this driver

  @param[in]  QuickI2cDev        Context of QuickI2c device

  @retval EFI_SUCCESS            QuickI2c Dma Initialized successfuly
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickI2cDmaCreatePrdCommonBuffers (
  IN  QUICK_I2C_DEV                *QuickI2cDev
  )
{
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINT64                        PageCount;
  UINT64                        ByteCount;
  EFI_STATUS                    Status;

  PciIo = QuickI2cDev->PciIo;

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &QuickI2cDev->DmaRead2.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) QuickI2cDev->DmaRead2.CommonPrdBuffer,
                       &ByteCount,
                       &QuickI2cDev->DmaRead2.CommonPrdBufferDevAddr,
                       &QuickI2cDev->DmaRead2.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Read2 buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) QuickI2cDev->DmaRead2.CommonPrdBuffer, ByteCount);

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &QuickI2cDev->DmaWrite.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Read2 buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) QuickI2cDev->DmaWrite.CommonPrdBuffer,
                       &ByteCount,
                       &QuickI2cDev->DmaWrite.CommonPrdBufferDevAddr,
                       &QuickI2cDev->DmaWrite.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Write buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) QuickI2cDev->DmaWrite.CommonPrdBuffer, ByteCount);

  PageCount = EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_SW_DMA_TABLES_SUPPORTED);
  Status = PciIo->AllocateBuffer (
                    PciIo,
                    AllocateAnyPages,
                    EfiBootServicesData,
                    PageCount,
                    (VOID*) &QuickI2cDev->SwDmaRead.CommonPrdBuffer,
                    0
                    );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers SwDmaRead buffer allocateBuffer failed Status %r\n", Status));
    return EFI_OUT_OF_RESOURCES;
  }

  ByteCount = EFI_PAGES_TO_SIZE (PageCount);
  Status    = PciIo->Map (
                       PciIo,
                       EfiPciIoOperationBusMasterCommonBuffer,
                       (VOID*) QuickI2cDev->SwDmaRead.CommonPrdBuffer,
                       &ByteCount,
                       &QuickI2cDev->SwDmaRead.CommonPrdBufferDevAddr,
                       &QuickI2cDev->SwDmaRead.CommonPrdBufferMapping
                       );
  if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers SwDmaRead buffer map failed Status %r\n", Status));
    DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateCommonBuffers Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
    return EFI_OUT_OF_RESOURCES;
  }

  ZeroMem ((UINT8*) QuickI2cDev->SwDmaRead.CommonPrdBuffer, ByteCount);

  return Status;
}

/**
  Fills single Prd Table with the defualt values

  @param[in]  QuickI2cDma      Dma that stores Driver PRD table
  @param[in]  PrdTableIndex    Index of current Prd table
**/
VOID
QuickI2cDmaFillSinglePrdTable (
  IN THC_DMA    *QuickI2cDma,
  IN UINT32     PrdTableIndex
  )
{
  PRD_TABLE     *PrdTable;
  UINT32        CurrentEntry;
  UINT32        EntryOffset;

  CurrentEntry = 0;
  EntryOffset  = QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries * PrdTableIndex;
  PrdTable =  (PRD_TABLE*) QuickI2cDma->CommonPrdBuffer;

  for (CurrentEntry = 0; CurrentEntry < QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; CurrentEntry++) {
    CopyMem (
      &PrdTable->Entries[CurrentEntry + EntryOffset],
      &QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry],
      sizeof (PRD_ENTRY)
      );
  }
}

/**
  Fills every Prd Table with the defualt values

  @param[in]  QuickI2cDma           Dma that stores Driver PRD Table
**/
VOID
QuickI2cDmaFillPrdTables (
  THC_DMA                        *QuickI2cDma
  )
{
  UINT32                        PrdTableIndex;

  THC_LOCAL_DEBUG (L"QuickI2cDmaFillPrdTables Start ()\n")

  //
  // Loop through total number of Entries in Common Prd
  // Fill every entry with the corresponding data for the HW to use
  //
  for (PrdTableIndex = 0; PrdTableIndex < QuickI2cDma->NumOfPrdTables; PrdTableIndex++) {
    QuickI2cDmaFillSinglePrdTable (QuickI2cDma, PrdTableIndex);
  }
}

/**
  Initialize Driver Prd Tables
  Allocates and maps every single page for each used entry
  besed on the supported BufferSize for each MAXIMUM_READ_SIZE/MAXIMUM_WRITE_SIZE

  @param[in]  QuickI2cDev      Context of QuickI2c device
  @param[in]  QuickI2cDma      Dma that stores Driver PRD Table
  @param[in]  PciIoOperation   BusMasterRead or BusMasterWrite

  @retval EFI_SUCCESS            QuickI2c Dma Initialized successfully
  @retval EFI_OUT_OF_RESOURCES   Not enough memory available
**/
EFI_STATUS
QuickI2cDmaCreatePrds (
  IN QUICK_I2C_DEV                  *QuickI2cDev,
  IN THC_DMA                        *QuickI2cDma,
  IN EFI_PCI_IO_PROTOCOL_OPERATION  PciIoOperation
  )
{
  EFI_STATUS                    Status;
  EFI_PCI_IO_PROTOCOL           *PciIo;
  UINTN                         PageCount;
  UINTN                         ByteCount;
  UINT16                        CurrentEntry;
  UINT64                        CurrentAddress;
  UINT32                        PrdTableIndex;
  UINTN                         BytesRemaining;

  THC_LOCAL_DEBUG (L"QuickI2cDmaCreatePrds Start ()\n")

  PciIo = QuickI2cDev->PciIo;

  THC_LOCAL_DEBUG (L"QuickI2cDmaCreatePrds NumOfPrdTables- %d, BufferSize- 0x%x, Pagecount- %d\n ",
                    QuickI2cDma->NumOfPrdTables, QuickI2cDma->BufferSize, EFI_SIZE_TO_PAGES (QuickI2cDma->BufferSize))

  for (PrdTableIndex = 0; PrdTableIndex < QuickI2cDma->NumOfPrdTables; PrdTableIndex++) {
    PageCount = EFI_SIZE_TO_PAGES (QuickI2cDma->BufferSize);
    Status    = PciIo->AllocateBuffer (
                         PciIo,
                         AllocateAnyPages,
                         EfiBootServicesData,
                         PageCount,
                         (VOID*) &QuickI2cDma->DriverPrdTable[PrdTableIndex].Buffer,
                         0
                         );
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateReadPrds AllocateBuffer for PrdTableIndex: %d failed with Status %r\n", PrdTableIndex, Status));
      return EFI_OUT_OF_RESOURCES;
    }

    ByteCount = EFI_PAGES_TO_SIZE (PageCount);
    Status    = PciIo->Map (
                         PciIo,
                         PciIoOperation,
                         (VOID*) QuickI2cDma->DriverPrdTable[PrdTableIndex].Buffer,
                         &ByteCount,
                         &QuickI2cDma->DriverPrdTable[PrdTableIndex].DevAddr,
                         &QuickI2cDma->DriverPrdTable[PrdTableIndex].Mapping
                         );

    if (EFI_ERROR (Status) || (ByteCount != EFI_PAGES_TO_SIZE (PageCount))) {
      DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateReadPrds Read buffer map failed PrdTableIndex: %d Status %r\n", PrdTableIndex, Status));
      DEBUG ((DEBUG_WARN, "QuickI2cDmaCreateReadPrds Requsted ByteCount: %d Mapped Count: %d\n", EFI_PAGES_TO_SIZE (PageCount), ByteCount));
      return EFI_OUT_OF_RESOURCES;
    }

    ZeroMem ((UINT8*) QuickI2cDma->DriverPrdTable[PrdTableIndex].Buffer, ByteCount);

    BytesRemaining   = ByteCount;
    CurrentEntry     = 0;
    CurrentAddress   = QuickI2cDma->DriverPrdTable[PrdTableIndex].Buffer;
    //
    // Loop through total number of Entries
    // Each entry Max length is 4K (size of the single page), max entries is 256.
    // Max allocated total for single PRD table is 1MB
    //
    while (BytesRemaining > 0 && CurrentEntry < 256) {
      QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].DestinationAddress = RShiftU64 (CurrentAddress, ADDRESS_SHIFT);
      QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd           = 0x0;
      QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length             = EFI_PAGE_SIZE;
      if (BytesRemaining <= EFI_PAGE_SIZE) {
        QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].Length           = BytesRemaining;
        QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry].EndOfPrd         = 0x1;
        break;
      }
      CurrentEntry++;
      CurrentAddress += EFI_PAGE_SIZE;
      BytesRemaining -= EFI_PAGE_SIZE;
    }
    if (CurrentEntry == 256) {
      QuickI2cDma->DriverPrdTable[PrdTableIndex].Entries[CurrentEntry-1].EndOfPrd = 0x1;
      QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries = CurrentEntry;
      DEBUG ((DEBUG_WARN, "QuickI2cDmaCreatePrds QuickI2c ERROR: 256 Entries are not sufficient for the TBL[%d]:NumOfConfiguredEntries=0x%x BytesRemaining=0x%x\n", PrdTableIndex, QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries, BytesRemaining));
    } else {
      QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries = CurrentEntry + 1;
    }
  }

  return EFI_SUCCESS;
}

/**
  Initialize QuickI2c DMA

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS     QuickI2c Dma Initialized successfully
  @retval other           Error during initialization
**/
EFI_STATUS
QuickI2cDmaInitialize (
  IN QUICK_I2C_DEV                *QuickI2cDev
  )
{
  EFI_STATUS                  Status;
  THC_M_PRT_DEVINT_CFG_1      DevIntCfg1;
  UINT32                      MaxSize;

  THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize Start ()\n")

  DevIntCfg1.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_DEVINT_CFG_1);
  DevIntCfg1.Fields.IntTypDataVal = 0xFFFF; // Enabling all interrupts
  MmioWrite32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_DEVINT_CFG_1, DevIntCfg1.Data32);

  Status = QuickI2cDmaCreatePrdCommonBuffers (QuickI2cDev);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize error, failed to CreatePrdCommonBuffers Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cDmaInitialize error, failed to CreatePrdCommonBuffers Status %r\n", Status));
    return Status;
  }

  Status = QuickI2cLibSetPrdTablesAddress (QuickI2cDev->PciBar0,
                                           QuickI2cDev->DmaWrite.CommonPrdBuffer,
                                           QuickI2cDev->DmaRead2.CommonPrdBuffer,
                                           QuickI2cDev->SwDmaRead.CommonPrdBuffer
                                           );
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize error, failed to SetPrdTablesAddress Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cDmaInitialize error, failed to SetPrdTablesAddress Status %r\n", Status));
    return Status;
  }

  MaxSize = MAX (QuickI2cDev->DeviceDescriptor.wMaxInputLength, QuickI2cDev->DeviceDescriptor.wReportDescLength);
  QuickI2cDev->DmaRead2.BufferSize     = (((MaxSize) + (THC_MAX_HID_BUFFER_SIZE) - 1) & ~((THC_MAX_HID_BUFFER_SIZE) - 1));
  QuickI2cDev->DmaRead2.NumOfPrdTables = PRD_READ_TABLES_SUPPORTED;
  //
  // Currently wMaxOutputLength is 0 i.e. not expected, so allocating 4KB
  //
  QuickI2cDev->DmaWrite.BufferSize     = THC_MAX_HID_BUFFER_SIZE;
  QuickI2cDev->DmaWrite.NumOfPrdTables = PRD_WRITE_TABLES_SUPPORTED;

  // For SwDMA
  QuickI2cDev->SwDmaRead.BufferSize     = THC_MAX_HID_BUFFER_SIZE;
  QuickI2cDev->SwDmaRead.NumOfPrdTables = PRD_SW_DMA_TABLES_SUPPORTED;


  Status = QuickI2cDmaCreatePrds (QuickI2cDev, &QuickI2cDev->DmaRead2, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize error, failed to Create Read Prds Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cDmaInitialize error, failed to Create Read Prds Status %r\n", Status));
    return Status;
  }

  QuickI2cDmaFillPrdTables (&QuickI2cDev->DmaRead2);

  Status = QuickI2cDmaCreatePrds (QuickI2cDev, &QuickI2cDev->DmaWrite, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize error, failed to Create Write Prds Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cDmaInitialize error, failed to Create Write Prds Status %r\n", Status));
    return Status;
  }

  QuickI2cDmaFillPrdTables (&QuickI2cDev->DmaWrite);

  Status = QuickI2cDmaCreatePrds (QuickI2cDev, &QuickI2cDev->SwDmaRead, EfiPciIoOperationBusMasterCommonBuffer);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaInitialize error, failed to Create Read Prds Status %r\n", Status)
    DEBUG ((DEBUG_WARN, "QuickI2cDmaInitialize error, failed to Create Read Prds Status %r\n", Status));
    return Status;
  }

  QuickI2cDmaFillPrdTables (&QuickI2cDev->SwDmaRead);

  QuickI2cLibSetLengthInPrd  (QuickI2cDev->PciBar0,
                              (UINT8) QuickI2cDev->DmaRead2.DriverPrdTable[0].NumOfConfiguredEntries,
                              PRD_READ_TABLES_SUPPORTED,
                              (UINT8) QuickI2cDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries,
                              (UINT8) QuickI2cDev->SwDmaRead.DriverPrdTable[0].NumOfConfiguredEntries,
                              PRD_SW_DMA_TABLES_SUPPORTED
                              );

  QuickI2cDev->HidBuffer        = AllocateAlignedPages (EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE), sizeof (UINT8));
  if (QuickI2cDev->HidBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  QuickI2cLibSetReadRx2StartBit (QuickI2cDev->PciBar0);

  return EFI_SUCCESS;
}

/**
  Calculates the length of the current message

  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  PrdTable           Prd Table updated by the THC HW
  @param[in]  PrdTableIndex      Index of the Prd Table
  @param[in]  EntryOffset        Entry offset

  @retval MessageLength          Size of the message
**/
UINT64
QuickI2cDmaGetMessageLength (
  IN THC_DMA                        *QuickI2cDma,
  IN PRD_TABLE                      *PrdTable,
  IN UINT16                         PrdTableIndex,
  IN UINT32                         EntryOffset
  )
{
  UINT32  Index;
  UINT64  MessageLength = 0;

  for (Index = 0; Index < QuickI2cDma->DriverPrdTable[PrdTableIndex].NumOfConfiguredEntries; Index++) {
    MessageLength += PrdTable->Entries[EntryOffset + Index].Length;
    if (PrdTable->Entries[EntryOffset + Index].EndOfPrd == 1) {
      return MessageLength;
    }
  }
  return MessageLength;
}

/**
  For RxDMA2 Reads first PRD Table Entry and checks if header data type
  equals TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickI2c Context.

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller than its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller the one present in Prd
**/
EFI_STATUS
QuickI2cDmaReadSingleReport (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN THC_DMA                *QuickI2cDma,
  IN UINT16                 CurrentPrdTable
  )
{
  QUICK_I2C_READ_DATA_BUFF  *ReadDataBuff;
  PRD_TABLE                 *PrdTable;
  UINT64                    MessageLength;
  UINT64                    DataAddress;
  UINT32                    EntryOffset;
  HID_TOUCH_OUTPUT          TouchOutput;
  HID_XY_BOUNDARY           XyBoundary;
  EFI_STATUS                Status;

  MessageLength = 0;
  DataAddress   = 0;
  EntryOffset   = 0;
  EntryOffset   = (UINT32) (QuickI2cDma->DriverPrdTable[CurrentPrdTable].NumOfConfiguredEntries) * CurrentPrdTable;
  PrdTable      = (PRD_TABLE *) QuickI2cDma->CommonPrdBuffer;

  ReadDataBuff = AllocateZeroPool (sizeof (QUICK_I2C_READ_DATA_BUFF));
  if (ReadDataBuff == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  MessageLength = QuickI2cDmaGetMessageLength (QuickI2cDma, PrdTable, CurrentPrdTable, EntryOffset);

  //
  // To handle reset response case only, reset response is always 2 bytes
  //
  if (MessageLength == QUICK_I2C_DEFAULT_RESET_RESPONSE_LENGTH) {
    THC_LOCAL_DEBUG (L"QuickI2cDmaReadSingleReport QuickI2cInputReportResetResponse received\n")

    DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);
    CopyMem ((UINT8*) ReadDataBuff, (UINT8*) DataAddress, MessageLength);

    //ShowBuffer ((UINT8*)(DataAddress), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled
    QuickI2cDev->HidDataAvailable = TRUE;
    FreePool (ReadDataBuff);
    return EFI_SUCCESS;
  }

  if (MessageLength < sizeof (QUICK_I2C_INPUT_REPORT_BODY_HEADER)) {
    THC_LOCAL_DEBUG (L"ERROR- MessageLength < sizeof (QUICK_I2C_INPUT_REPORT_BODY_HEADER) >> %d \n", sizeof (QUICK_I2C_INPUT_REPORT_BODY_HEADER))
    FreePool (ReadDataBuff);
    return EFI_BAD_BUFFER_SIZE;
  }
  if (QuickI2cDma->DriverPrdTable[CurrentPrdTable].Entries[0].Length < PrdTable->Entries[EntryOffset].Length) {
    THC_LOCAL_DEBUG (L"ERROR- Length - %d, PrdTable->Entries[EntryOffset].Length - %d \n", QuickI2cDma->DriverPrdTable[CurrentPrdTable].Entries[0].Length, PrdTable->Entries[EntryOffset].Length)
    FreePool (ReadDataBuff);
    return EFI_BUFFER_TOO_SMALL;
  }

  DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);
  CopyMem ((UINT8*) QuickI2cDev->HidBuffer, (UINT8*) DataAddress, MessageLength);
  QuickI2cDev->HidDataAvailable = TRUE;

  //ShowBuffer ((UINT8*)(QuickI2cDev->HidBuffer), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled

  if (QuickI2cDev->HidBuffer != NULL) {
    // Initialize the TouchOutput X/Y values with previous pointer location.
    TouchOutput.B = 0;
    TouchOutput.X = (QuickI2cDev->Report.HigherXByte << 8) + (QuickI2cDev->Report.LowerXByte);
    TouchOutput.Y = (QuickI2cDev->Report.HigherYByte << 8) + (QuickI2cDev->Report.LowerYByte);
    Status = HidParseInput (QuickI2cDev->InputReportTable, (UINT8*) QuickI2cDev->HidBuffer, &TouchOutput, &XyBoundary, QuickI2cDev->HidSolutionFlag);
    if (Status == EFI_SUCCESS) {
      QuickI2cDev->Report.HigherXByte = (UINT8) (((TouchOutput.X) & 0xFF00) >> 8);
      QuickI2cDev->Report.LowerXByte  = (UINT8) (TouchOutput.X) & 0xFF;
      QuickI2cDev->Report.HigherYByte = (UINT8) (((TouchOutput.Y) & 0xFF00) >> 8);
      QuickI2cDev->Report.LowerYByte  = (UINT8) (TouchOutput.Y) & 0xFF;
      QuickI2cDev->Report.TouchSts    = (UINT8) TouchOutput.B;
      QuickI2cDev->AbsPtrProtocol.Mode->AbsoluteMaxX = XyBoundary.MaxX;
      QuickI2cDev->AbsPtrProtocol.Mode->AbsoluteMinX = XyBoundary.MinX;
      QuickI2cDev->AbsPtrProtocol.Mode->AbsoluteMaxY = XyBoundary.MaxY;
      QuickI2cDev->AbsPtrProtocol.Mode->AbsoluteMinY = XyBoundary.MinY;
      QuickI2cDev->AbsPtrDataAvailable = TRUE;
      THC_LOCAL_DEBUG (L"Touch data: State | X:%d | Y:%d | ActiveButton: %d \n",
                      (QuickI2cDev->Report.HigherXByte << 8) + QuickI2cDev->Report.LowerXByte,
                      (QuickI2cDev->Report.HigherYByte << 8) + QuickI2cDev->Report.LowerYByte,
                      QuickI2cDev->Report.TouchSts)
      Status = gBS->SignalEvent (QuickI2cDev->AbsPtrProtocol.WaitForInput);
      THC_LOCAL_DEBUG (L"WaitForInput SignalEvent Status - %r\n", Status)
    }
  }

  FreePool (ReadDataBuff);

  return EFI_SUCCESS;
}


/**
  For SwDMA Reads first PRD Table Entry and checks if header data type
  equals TouchReadDataTypeHidReport
  and Report Id is equal to SINGLE_FINGER_REPORT_ID.
  If passed report data is copied to the QuickI2c Context.

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  QuickI2cDma        Dma that stores Driver PRD Table
  @param[in]  CurrentPrdTable    Index of current Prd table

  @retval EFI_SUCCESS            Data read done
  @retval EFI_BAD_BUFFER_SIZE    MessageLength is smaller than its header
  @retval EFI_BUFFER_TOO_SMALL   Allocated entry length is smaller the one present in Prd
**/
EFI_STATUS
QuickI2cSwDmaReadSingleReport (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN THC_DMA                *QuickI2cDma,
  IN UINT16                 CurrentPrdTable
  )
{
  QUICK_I2C_READ_DATA_BUFF  *ReadDataBuff;
  PRD_TABLE                 *PrdTable;
  UINT32                    MessageLength;
  UINT64                    DataAddress;
  UINT32                    EntryOffset;
  THC_M_PRT_RPRD_CNTRL_SW   RprdCntrlSw;
  THC_M_PRT_SW_DMA_PRD_TABLE_LEN  DmaPrdTableLen;

  MessageLength = 0;
  DataAddress   = 0;
  EntryOffset   = 0;
  EntryOffset   = (UINT32) (QuickI2cDma->DriverPrdTable[CurrentPrdTable].NumOfConfiguredEntries) * CurrentPrdTable;
  PrdTable      = (PRD_TABLE *) QuickI2cDma->CommonPrdBuffer;

  ReadDataBuff = AllocateZeroPool (sizeof (QUICK_I2C_READ_DATA_BUFF));
  if (ReadDataBuff == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  RprdCntrlSw.Data32 = MmioRead32 ((QuickI2cDev->PciBar0) + R_THC_MEM_PRT_RPRD_CNTRL_SW);

  if (RprdCntrlSw.Fields.SwDmaI2cRxDlenEn == QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_0) {
    //
    // For report descriptor where the length is known beforehand
    //
    DmaPrdTableLen.Data32 = MmioRead32 ((QuickI2cDev->PciBar0) + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN);
    MessageLength = DmaPrdTableLen.Fields.SwDmaPrdTableLen;

    //
    // For Hynitron THAT trackpad to consider extra 160 bytes read to match with INT line LOW period,
    // if below conditions met, below will be applied
    //
    if ((QuickI2cDev->DeviceDescriptor.wVendorID == THC_HID_THAT_VID) && (QuickI2cDev->DeviceDescriptor.wProductID == THC_HID_THAT_PID)) {
      MessageLength -= THC_THAT_SW_DMA_ADD_LENGTH;
      THC_LOCAL_DEBUG (L"QuickI2cSwDma: For Hynitron THAT trackpad applied, MessageLength - %d bytes \n", MessageLength)
    }

    // Compare against ContentLength UINT16
    if (MessageLength < sizeof (UINT16)) {
      THC_LOCAL_DEBUG (L"ERROR: QuickI2cSwDmaReadSingleReport SwDma response <%d> Bytes < ContentLength\n", MessageLength)
      FreePool (ReadDataBuff);
      return EFI_BAD_BUFFER_SIZE;
    }

    DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);
    CopyMem ((UINT8*) ReadDataBuff, (UINT8*) DataAddress, sizeof (QUICK_I2C_INPUT_REPORT_BODY_HEADER));
    //ShowBuffer ((UINT8*)(DataAddress), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled

    if ((QuickI2cDev->HidActive == TRUE) && (QuickI2cDev->HidBuffer != NULL)) {
      THC_LOCAL_DEBUG (L"QuickI2cSwDmaReadSingleReport: SwDma response of <%d> Bytes\n", MessageLength)
      CopyMem ((UINT8*) QuickI2cDev->HidBuffer, (UINT8*) DataAddress, MessageLength);
      QuickI2cDev->HidBuffer += MessageLength;
      QuickI2cDev->SwDmaMessageLength = MessageLength;
      QuickI2cDev->HidDataAvailable = TRUE;
    }
    FreePool (ReadDataBuff);
    return EFI_SUCCESS;

  } else if (RprdCntrlSw.Fields.SwDmaI2cRxDlenEn == QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_1) {
    //
    // Get Feature report response
    //
    DmaPrdTableLen.Data32 = MmioRead32 ((QuickI2cDev->PciBar0) + R_THC_MEM_PRT_SW_DMA_PRD_TABLE_LEN);
    MessageLength = DmaPrdTableLen.Fields.SwDmaPrdTableLen;

    // Compare against ContentLength UINT16
    if (MessageLength < sizeof (UINT16)) {
      THC_LOCAL_DEBUG (L"ERROR: QuickI2cSwDmaReadSingleReport SwDma response <%d> Bytes < ContentLength\n", MessageLength)
      DEBUG ((DEBUG_INFO, "ERROR: QuickI2cSwDmaReadSingleReport SwDma response <%d> Bytes < ContentLength\n", MessageLength));
      FreePool (ReadDataBuff);
      return EFI_BAD_BUFFER_SIZE;
    }

    DataAddress = LShiftU64 (PrdTable->Entries[EntryOffset].DestinationAddress, ADDRESS_SHIFT);

    CopyMem ((UINT8*) ReadDataBuff, (UINT8*) DataAddress, sizeof (QUICK_I2C_INPUT_REPORT_BODY_HEADER));

    THC_LOCAL_DEBUG (L"SwDma response MessageLength <%d> Bytes, ContentLength <%d>\n", MessageLength, ReadDataBuff->Header.Fields.ContentLength)
    //ShowBuffer ((UINT8*)(DataAddress), (UINT32) MessageLength); // Uncomment when THC_LOCAL_DEBUG is enabled

    if ((QuickI2cDev->HidActive == TRUE) && (QuickI2cDev->HidBuffer != NULL)) {
      THC_LOCAL_DEBUG (L"QuickI2cSwDmaReadSingleReport: SwDma response of <%d> Bytes\n", ReadDataBuff->Header.Fields.ContentLength)
      CopyMem ((UINT8*) QuickI2cDev->HidBuffer, (UINT8*) DataAddress, ReadDataBuff->Header.Fields.ContentLength);
      QuickI2cDev->HidBuffer += MessageLength;
      QuickI2cDev->SwDmaMessageLength = MessageLength;
      QuickI2cDev->HidDataAvailable = TRUE;
    }
    FreePool (ReadDataBuff);

    return EFI_SUCCESS;
  }

  return EFI_DEVICE_ERROR;
}

/**
  For RxDma2 Reads data from each Read Rx2 DMA PRD Table Entry until write pointer is equal to read pointer
  Write pointer is updated after each PRD Table read is finished

  @param[in]  QuickI2cDev               Context of QuickI2c device
  @param[in]  CurrentCBReadPointer      Circular Buffer Read Pointer
  @param[in]  CurrentCBWritePointer     Circular Buffer Write Pointer

**/
VOID
QuickI2cDmaReadRx2 (
  IN QUICK_I2C_DEV          *QuickI2cDev,
  IN UINT8                  CurrentCBWritePointer,
  IN UINT8                  CurrentCBReadPointer
  )
{
  THC_M_PRT_READ_DMA_CNTRL_2  ReadDmaCntrl2;
  EFI_STATUS                  Status;
  UINT8                       CurrentPrdTable;

  while (((CurrentCBWritePointer & POINTER_MASK) != (CurrentCBReadPointer & POINTER_MASK)) ||  // CB Not Empty
      (CurrentCBWritePointer == CurrentCBReadPointer)) {                                       // CB Full
    CurrentPrdTable = (UINT8) (CurrentCBWritePointer & POINTER_MASK);
    THC_LOCAL_DEBUG (L"PrdTable %d CBWritePointer 0x%x CBReadPointer 0x%x \n",
                     CurrentPrdTable,
                     CurrentCBWritePointer,
                     CurrentCBReadPointer
                    )
    Status = QuickI2cDmaReadSingleReport (QuickI2cDev, &QuickI2cDev->DmaRead2, CurrentPrdTable);
    if (EFI_ERROR (Status)) {
      THC_LOCAL_DEBUG (L"QuickI2cDmaReadRx2 QuickI2cDmaReadbuffer error, failed to Read Data Buffer Status %r\n", Status)
      DEBUG ((DEBUG_ERROR, "QuickI2cDmaReadRx2 Error Read Data Buffer Status %r\n", Status));
      // No return to avoid dead loop, always change Write Pointer, even on bad data
    }

    // Return PRD table to its initial state
    QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->DmaRead2, CurrentPrdTable);

    if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_16) {
      THC_LOCAL_DEBUG (L"WRAPAROUND_VALUE_16 \n")
      QuickI2cLibSetReadRx2WritePointer (QuickI2cDev->PciBar0, POINTER_WRAPAROUND);
    } else if ((CurrentCBWritePointer + 1) == WRAPAROUND_VALUE_0X90) {
      THC_LOCAL_DEBUG (L"WRAPAROUND_VALUE_0X90 \n")
      QuickI2cLibSetReadRx2WritePointer (QuickI2cDev->PciBar0, 0x0);
    } else {
      THC_LOCAL_DEBUG (L"WP + 1\n")
      QuickI2cLibSetReadRx2WritePointer (QuickI2cDev->PciBar0, CurrentCBWritePointer + 1);
    }

    ReadDmaCntrl2.Data    = QuickI2cLibGetReadRx2Data (QuickI2cDev->PciBar0);
    CurrentCBWritePointer = (UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer;
    //
    // Cashing last WP
    //
    QuickI2cDev->LastWritePointer = CurrentCBWritePointer;
  }
}


/**
  For SwDMA Reads data from each Read Sw DMA PRD Table Entry if write pointer is equal to read pointer
  in case of SwRxDMA event

  @param[in]  QuickI2cDev  Context of QuickI2c device

**/
VOID
QuickI2cReadSwRxDma (
  IN QUICK_I2C_DEV       *QuickI2cDev
  )
{
  EFI_STATUS             Status;
  UINT8                  CurrentPrdTable;
  UINT8                  CurrentCBWritePointer;
  UINT8                  CurrentCBReadPointer;
  THC_M_PRT_READ_DMA_CNTRL_SW   ReadDmaCntrlSw;

  ReadDmaCntrlSw.Data32 = MmioRead32 ((QuickI2cDev->PciBar0) + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  CurrentCBWritePointer = (UINT8) ReadDmaCntrlSw.Fields.Tpcwp;
  CurrentCBReadPointer = (UINT8) ReadDmaCntrlSw.Fields.Tpcrp;

  if ((CurrentCBWritePointer & POINTER_MASK) == (CurrentCBReadPointer & POINTER_MASK)) {
    CurrentPrdTable = (UINT8) (CurrentCBWritePointer & POINTER_MASK);
    THC_LOCAL_DEBUG (L"PrdTable %d CBWritePointer %d CBReadPointer %d CB 0x%X \n",
                     CurrentPrdTable,
                     CurrentCBWritePointer & POINTER_MASK,
                     CurrentCBReadPointer & POINTER_MASK,
                     CurrentCBWritePointer
                    )
    Status = QuickI2cSwDmaReadSingleReport (QuickI2cDev, &QuickI2cDev->SwDmaRead, CurrentPrdTable);
    if (EFI_ERROR (Status)) {
      THC_LOCAL_DEBUG (L"QuickI2cReadSwRxDma QuickI2cDmaReadbuffer error, failed to Read Data Buffer Status %r\n", Status)
      DEBUG ((DEBUG_ERROR, "QuickI2cReadSwRxDma Error Read Data Buffer Status %r\n", Status));
      // No return to avoid dead loop, always change Write Pointer, even on bad data
    }
  }
}


/**
  Writes data to a single Tx DMA PRD Table

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  DataHeader         Write Data Header
  @param[in]  Buffer             Data Container that will be sent to external device

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickI2cDmaWriteTx (
  IN QUICK_I2C_DEV                      *QuickI2cDev,
  IN QUICK_I2C_OUTPUT_REPORT_HEADER     OutputHeader,
  IN UINT8                              *DataBuffer
  )
{
  PRD_TABLE   *PrdTable;
  UINT64      DataAddress;
  UINT32      PrdEntryLength;
  UINT32      WriteLength;
  UINT32      EntryIndex;
  UINT32      ContentLength;

  DataAddress         = 0;
  PrdEntryLength      = 0;
  EntryIndex          = 0;
  PrdTable            = (PRD_TABLE *) QuickI2cDev->DmaWrite.CommonPrdBuffer;
  ContentLength       = OutputHeader.Fields.ContentLength;

  //THC_LOCAL_DEBUG (L"%a: OutputHeader- ", __FUNCTION__)
  //ShowBuffer ((UINT8*)&(OutputHeader), sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER)); // Uncomment when THC_LOCAL_DEBUG is enabled
  //THC_LOCAL_DEBUG (L"%a: DataBuffer- ", __FUNCTION__)
  //ShowBuffer (DataBuffer, OutputHeader.Fields.ContentLength); // Uncomment when THC_LOCAL_DEBUG is enabled

  //
  // Step 1 - Check DMA Intr Status bit
  //
  QuickI2cLibCheckWriteDmaStatus (QuickI2cDev->PciBar0);

  //
  // Step 2 - Copy Buffer data to device PRD
  //
  if ((PrdTable->Entries[0].Length * PRD_MAX_ENTRIES) < (ContentLength + sizeof (OutputHeader.Fields.CommandRegister))) {
    return EFI_BUFFER_TOO_SMALL;
  }

  //
  // Copy Header information, increase address and set remaining length of the first entry
  //
  DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
  //
  // DMA QUICK_I2C_OUTPUT_REPORT_HEADER
  //
  CopyMem ((UINT8*) DataAddress, &(OutputHeader.Fields.CommandRegister), sizeof (OutputHeader.Fields.CommandRegister));

  DataAddress += sizeof (OutputHeader.Fields.CommandRegister);
  PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length - sizeof (OutputHeader.Fields.CommandRegister);

  do {
    PrdTable->Entries[EntryIndex].EndOfPrd = 0;
    //
    // Set write length for given Prd Entry
    //
    if (ContentLength >= PrdEntryLength) {
      WriteLength = PrdEntryLength;
    } else {
      WriteLength = ContentLength;
    }
    //
    // Put the data in the destination address
    //
    if (WriteLength > 0) {
      CopyMem ((UINT8*) DataAddress, DataBuffer, WriteLength);
    }
    //
    // Substract copied data from the ContentLength
    //
    ContentLength = ContentLength - WriteLength;

    //
    // Store new write length along with the header in Prd entry.
    //
    if (EntryIndex == 0) {
      WriteLength += sizeof (OutputHeader.Fields.CommandRegister);
    }
    PrdTable->Entries[EntryIndex].Length = WriteLength;
    THC_LOCAL_DEBUG (L"QuickI2cDmaWriteTx WriteLength %d, ContentLength- %d\n", WriteLength, ContentLength)
    if (ContentLength > 0) {
      DataBuffer += WriteLength;
      EntryIndex++;
      DataAddress = LShiftU64 (PrdTable->Entries[EntryIndex].DestinationAddress, ADDRESS_SHIFT);
      PrdEntryLength = (UINT32) PrdTable->Entries[EntryIndex].Length;
    } else {
      break;
    }
  } while (EntryIndex < QuickI2cDev->DmaWrite.DriverPrdTable[0].NumOfConfiguredEntries);

  //
  // Set End on the last PRD entry used
  //
  PrdTable->Entries[EntryIndex].EndOfPrd = 1;

  //
  // Step 3 - Start transfer
  //
  QuickI2cLibSetWriteStartBit (QuickI2cDev->PciBar0);

  //
  // Step 4 - Poll for Dma Complete
  //
  if (EFI_ERROR (QuickI2cLibPollDmaComplete (QuickI2cDev->PciBar0, WritePollingTimeout))) {
    return EFI_TIMEOUT;
  }

  //
  // Done - Reset PRD table settings
  //
  QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->DmaWrite, 0);

  return EFI_SUCCESS;
}

/**
  Writes data to a QuickI2c Sw DMA PRD Table

  @param[in]  QuickI2cDev        Context of QuickI2c device
  @param[in]  DataBuffer         Write Data Header
  @param[in]  RxDlenEn           Context of QuickI2c device
  @param[in]  Wbc                Data Container that will be sent to external device
  @param[in]  WriteDataHeader    Write Data Header

  @retval EFI_BUFFER_TOO_SMALL   THC DMA buffer is unable to fit that much data
  @retval EFI_TIMEOUT            DMA transaction did not finish in time
  @retval EFI_SUCCESS            Data sent
**/
EFI_STATUS
QuickI2cSwDma (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN UINT32                       *DataBuffer,
  IN UINT32                       RxDlenEn,
  IN UINT32                       Wbc,
  IN UINT32                       WriteDataHeader
  )
{
  THC_M_PRT_READ_DMA_CNTRL_SW     ReadDmaCntrlSw;
  EFI_STATUS                      Status;
  UINT16                          ReportDescLength;

  THC_LOCAL_DEBUG (L"QuickI2cSwDma: Entry ()\n")

  //
  // Pause RxDMA_1/RxDMA_2
  //
  Status = QuickI2cWaitForRxDMA2Pause(QuickI2cDev->PciBar0);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSwDma: QuickI2cWaitForRxDMA2Pause Status - %r\n", Status);
    return EFI_TIMEOUT;
  }

  //
  // Reset CB pointer and STOP DMA
  //
  QuickI2cResetDmaSettings (QuickI2cDev->PciBar0);

  //
  // Clear Global error and stall
  //
  if (QuickI2cLibIsQuiesceDisabled (QuickI2cDev->PciBar0)) {
    THC_LOCAL_DEBUG (L"QuickI2cSwDma QuiesceDisabled \n")
    Status = QuickI2cLibStartQuiesce (QuickI2cDev->PciBar0, StartQuiesceTimeout);
    THC_LOCAL_DEBUG (L"QuickI2cSwDma QuickI2cLibStartQuiesce Status: %r\n", Status)
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "QuickI2cSwDma QuickI2cLibStartQuiesce error, Status %r\n", Status));
      return Status;
    }
  }

  //
  // Reset PRD tables and clear out everything
  //
  ReadDmaCntrlSw.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_SW);
  ReadDmaCntrlSw.Fields.Start = 0;
  MmioWrite32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_SW, ReadDmaCntrlSw.Data32);

  //
  // Configure SwDma assuming PRD base table registers is set already
  //
  Status = QuickI2cSwDmaConfigure (QuickI2cDev);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSwDma: QuickI2cSwDmaConfigure return status %r\n", Status)
    return EFI_TIMEOUT;
  }

  //
  // Set appropriate values in R_THC_MEM_PRT_RPRD_CNTRL_SW for report descriptor retrieval via SWDMA
  //
  ReportDescLength = QuickI2cDev->DeviceDescriptor.wReportDescLength;

  //
  // For Hynitron THAT trackpad to consider extra 160 bytes read to match with INT line LOW period,
  // if below conditions met, below will be applied
  //
  if ((QuickI2cDev->DeviceDescriptor.wVendorID == THC_HID_THAT_VID) && (QuickI2cDev->DeviceDescriptor.wProductID == THC_HID_THAT_PID)) {
    ReportDescLength += THC_THAT_SW_DMA_ADD_LENGTH;
    THC_LOCAL_DEBUG (L"QuickI2cSwDma: For Hynitron THAT trackpad applied, ReportDescLength - %d bytes \n", ReportDescLength)
  }

  QuickI2cSwDmaProgramReadLen (QuickI2cDev->PciBar0, RxDlenEn, Wbc, ReportDescLength);

  //
  // Start Write SwDMA
  //
  QuickI2cStartWriteSwdma (QuickI2cDev->PciBar0, WriteDataHeader, *DataBuffer);

  //
  // Start transfer
  //
  QuickI2cLibSwDmaSetReadStartBit (QuickI2cDev->PciBar0);

  return EFI_SUCCESS;
}

/**
  Unmaps and frees Driver Prd Tables

  @param[in]  QuickI2cDev             Context of QuickI2c device
  @param[in]  QuickI2cDma             Dma that stores Driver PRD Table

**/
VOID
QuickI2cDmaFreePrds (
  IN QUICK_I2C_DEV                  *QuickI2cDev,
  IN THC_DMA                        *QuickI2cDma
  )
{
  UINT32         PrdTableIndex;

  for (PrdTableIndex = 0; PrdTableIndex < QuickI2cDma->NumOfPrdTables; PrdTableIndex++) {
    QuickI2cDev->PciIo->Unmap (
                        QuickI2cDev->PciIo,
                        QuickI2cDma->DriverPrdTable[PrdTableIndex].Mapping
                        );
    QuickI2cDev->PciIo->FreeBuffer (
                          QuickI2cDev->PciIo,
                          EFI_SIZE_TO_PAGES (QuickI2cDma->BufferSize),
                          (UINT8 *) QuickI2cDma->DriverPrdTable[PrdTableIndex].Buffer
                          );
  }
}

/**
  Stops Touch Panel and brings it back to reset state
  Frees all allocated memory and QuickI2c device context

  @param[in]  QuickI2cDev         Context of QuickI2c device
  @param[in]  HidOverI2c          Hid Over I2C policy data
  @param[in]  Reset               Reset data

**/
VOID
EFIAPI
QuickI2cStop (
  IN QUICK_I2C_DEV                *QuickI2cDev,
  IN THC_HID_OVER_I2C             *HidOverI2c,
  IN THC_RESET                    *Reset
  )
{
  if (QuickI2cDev == NULL) {
    DEBUG ((DEBUG_WARN, "QuickI2cStop QuickI2cDev is already Stopped\n"));
    return;
  }

  THC_LOCAL_DEBUG (L"QuickI2cStop\n")

  QuickI2cResetDmaSettings (QuickI2cDev->PciBar0);
  QuickI2cResetSwdmaSettings (QuickI2cDev->PciBar0);
  QuickI2cStopAbsPtr (QuickI2cDev);
  QuickI2cLibCleanUp (QuickI2cDev->PciBar0);

  //
  // Free/Unmap Common Buffers and Prd Tables
  //
  if (QuickI2cDev->DmaRead2.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG (L"QuickI2cStop Free DmaRead2.CommonPrdBuffer\n")
    QuickI2cDev->PciIo->Unmap (
                        QuickI2cDev->PciIo,
                        QuickI2cDev->DmaRead2.CommonPrdBufferMapping
                        );
    QuickI2cDev->PciIo->FreeBuffer (
                        QuickI2cDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_READ_TABLES_SUPPORTED),
                        (UINT8 *) QuickI2cDev->DmaRead2.CommonPrdBuffer
                        );
    QuickI2cDmaFreePrds (QuickI2cDev, &QuickI2cDev->DmaRead2);
  }
  if (QuickI2cDev->DmaWrite.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG (L"QuickI2cStop Free DmaWrite.CommonPrdBuffer\n")
    QuickI2cDev->PciIo->Unmap (
                        QuickI2cDev->PciIo,
                        QuickI2cDev->DmaWrite.CommonPrdBufferMapping
                        );
    QuickI2cDev->PciIo->FreeBuffer (
                        QuickI2cDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED),
                        (UINT8 *) QuickI2cDev->DmaWrite.CommonPrdBuffer
                        );
    QuickI2cDmaFreePrds (QuickI2cDev, &QuickI2cDev->DmaWrite);
  }
  if (QuickI2cDev->SwDmaRead.CommonPrdBuffer != 0) {
    THC_LOCAL_DEBUG (L"QuickI2cStop Free SwDmaRead.CommonPrdBuffer\n")
    QuickI2cDev->PciIo->Unmap (
                        QuickI2cDev->PciIo,
                        QuickI2cDev->SwDmaRead.CommonPrdBufferMapping
                        );
    QuickI2cDev->PciIo->FreeBuffer (
                        QuickI2cDev->PciIo,
                        EFI_SIZE_TO_PAGES (sizeof (PRD_TABLE) * PRD_WRITE_TABLES_SUPPORTED),
                        (UINT8 *) QuickI2cDev->SwDmaRead.CommonPrdBuffer
                        );
    QuickI2cDmaFreePrds (QuickI2cDev, &QuickI2cDev->SwDmaRead);
  }

  //
  // Restore original PCI attributes
  //
  QuickI2cDev->PciIo->Attributes (
                   QuickI2cDev->PciIo,
                   EfiPciIoAttributeOperationSet,
                   QuickI2cDev->PciAttributes,
                   NULL
                   );

  //
  // Close and Uninstall PCI I/O Protocol that was opened with PROTOCOL_BY_DRIVER
  //
  gBS->CloseProtocol (
         QuickI2cDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &QuickI2cDev->PciIo,
         QuickI2cDev->ControllerHandle
         );
  gBS->UninstallMultipleProtocolInterfaces (
         QuickI2cDev->ControllerHandle,
         &gEfiPciIoProtocolGuid,
         &QuickI2cDev->PciIo,
         NULL
         );

  //
  // Done with the controller name table
  //
  FreeUnicodeStringTable (QuickI2cDev->ControllerNameTable);

  //
  //  Done with the driver context
  //
  ZeroMem (QuickI2cDev, sizeof (QUICK_I2C_DEV));
  FreePool (QuickI2cDev);
}

/**
  Polls on QuickI2c Rx2 Circullar Buffers state
  If CB Write and Read Pointer are not equal Read Rx2 Dma is trigerred

  @param[in]  Event       Polling event
  @param[in]  Context     Context of QuickI2c device

**/
VOID
EFIAPI
QuickI2cPolling (
  IN EFI_EVENT Event,
  IN VOID      *Context
  )
{
  QUICK_I2C_DEV                 *QuickI2cDev;
  THC_M_PRT_READ_DMA_CNTRL_2    ReadDmaCntrl2;
  THC_M_PRT_READ_DMA_INT_STS_2  ReadDmaIntSts2;
  THC_M_PRT_READ_DMA_INT_STS_SW ReadDmaIntStsSw;
  UINT32                        Timeout;
  UINT8                         CurrentPrdTable;
  UINT8                         PrdLoopCounter;

  QuickI2cDev = (QUICK_I2C_DEV *) Context;

  if (QuickI2cDev == NULL) {
    DEBUG ((DEBUG_ERROR, "QuickI2cPolling QuickI2cDev is Null\n"));
    return;
  }
  if (QuickI2cDev->ReadDone) {
    Timeout = PollingLoopTimeout;
    ReadDmaCntrl2.Data     = QuickI2cLibGetReadRx2Data (QuickI2cDev->PciBar0);
    ReadDmaIntSts2.Data32  = QuickI2cLibGetReadRx2IntSts (QuickI2cDev->PciBar0);
    ReadDmaIntStsSw.Data32 = QuickI2cLibGetReadSwDmaRx2IntSts (QuickI2cDev->PciBar0);
    if (ReadDmaIntSts2.Fields.StallSts == 1) {
      DEBUG ((DEBUG_WARN, "QuickI2c Stall detected.\n"));
      THC_LOCAL_DEBUG (L"QuickI2c Stall detected.\n")
      //
      // 1. Read all PRD tables
      //
      CurrentPrdTable = (UINT8) (QuickI2cDev->LastWritePointer & POINTER_MASK);
      for (PrdLoopCounter = PRD_READ_TABLES_SUPPORTED; PrdLoopCounter > 0; PrdLoopCounter--) {
        if (CurrentPrdTable + 1 >= 16) {
          CurrentPrdTable = 0;
        } else {
          CurrentPrdTable++;
        }
        QuickI2cDmaReadSingleReport (QuickI2cDev, &QuickI2cDev->DmaRead2, CurrentPrdTable);
        //Return PRD table to its initial state
        QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->DmaRead2, CurrentPrdTable);
      }
      //
      // 2. Reset CB pointer and STOP DMA
      //
      ReadDmaCntrl2.Fields.Start                     = 0;
      ReadDmaCntrl2.Fields.QuickI2cPrdCbPointerReset = 1;
      MmioWrite32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_CNTRL_2, ReadDmaCntrl2.Data);
      //
      // 3. Clear Global error and stall
      //
      ReadDmaIntSts2.Fields.StallSts = 1;
      ReadDmaIntSts2.Fields.ErrorSts = 1;
      MmioWrite32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_READ_DMA_INT_STS_2, ReadDmaIntSts2.Data32);
      ReadDmaCntrl2.Data    = QuickI2cLibGetReadRx2Data (QuickI2cDev->PciBar0);
      //
      // 4. Start DMA
      //
      QuickI2cLibSetReadRx2StartBit (QuickI2cDev->PciBar0);
      ReadDmaCntrl2.Data = QuickI2cLibGetReadRx2Data (QuickI2cDev->PciBar0);
      //
      // Reset cashed WP
      //
      QuickI2cDev->LastWritePointer = (UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer;
      return;
    }

    //
    // Read SwDma
    //
    if ((ReadDmaIntStsSw.Fields.DmacplSts == 1) || (ReadDmaIntStsSw.Fields.IocSts == 1)) {
      THC_LOCAL_DEBUG (L"\n\nGot SW RXDMA Read Interrupt...\n")

      // SWDMA read
       QuickI2cReadSwRxDma (QuickI2cDev);
       QuickI2cLibClearSwDmaReadIntSts (QuickI2cDev->PciBar0);
    }

    //
    // Read RxDma2
    //
    if (!QuickI2cDev->SwDmaActive) {
      while ((((UINT8) (ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer  & POINTER_MASK)) != ((UINT8) (ReadDmaCntrl2.Fields.QuickI2cPrdCbReadPointer & POINTER_MASK))) ||
        ((UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer == (UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbReadPointer)) {                                           // CB Full
        QuickI2cDev->ReadDone = FALSE;
        THC_LOCAL_DEBUG (L"RxDma2 Read Interrupt >> 0x%x >> 0x%x \n", ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer, ReadDmaCntrl2.Fields.QuickI2cPrdCbReadPointer);

        QuickI2cDmaReadRx2 (QuickI2cDev, (UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer, (UINT8) ReadDmaCntrl2.Fields.QuickI2cPrdCbReadPointer);
        ReadDmaCntrl2.Data = QuickI2cLibGetReadRx2Data (QuickI2cDev->PciBar0);
        ReadDmaIntSts2.Data32 = QuickI2cLibGetReadRx2IntSts (QuickI2cDev->PciBar0);
        if (ReadDmaIntSts2.Fields.StallSts == 1) {
          DEBUG ((DEBUG_ERROR, "QuickI2cPolling Stall while detected ! ReadDmaIntSts2: 0x%X\n", ReadDmaIntSts2.Data32));
          THC_LOCAL_DEBUG (L"QuickI2cPollingStall while detected ! ReadDmaIntSts2: 0x%X\n", ReadDmaIntSts2.Data32);
          QuickI2cLibClearReadRx2IntSts (QuickI2cDev->PciBar0);
          QuickI2cDev->ReadDone = TRUE;
          return;
        }
        if (((UINT8) (ReadDmaCntrl2.Fields.QuickI2cPrdCbWritePointer & POINTER_MASK)) == ((UINT8) (ReadDmaCntrl2.Fields.QuickI2cPrdCbReadPointer & POINTER_MASK))) {
          QuickI2cDev->ReadDone = TRUE;
        }
        if (Timeout == 0) {
          DEBUG ((DEBUG_ERROR, "QuickI2cPolling Timeout while reading DMA, possible interrupt storm\n"));
          ASSERT (FALSE);
          // Clear interrupt status
          QuickI2cLibClearReadRx2IntSts (QuickI2cDev->PciBar0);
          QuickI2cDev->ReadDone = TRUE;
          return;
        }
        MicroSecondDelay (10);
        Timeout -= 10;
      }
    }
  }
}

/**
  Enables end point device.
 - TBD

  @param[in]  QuickI2cDev           Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_DEVICE_ERROR     TSSDONE not set or ERR set
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cEnableAfterReset (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  )
{
/**
  EFI_STATUS Status;

  QuickI2cReadDeviceDescriptor (QuickI2cDev);
  QuickI2cPrintDeviceDescriptor (QuickI2cDev);

  QuickI2cLibPrepareWriteConfigCmd (QuickI2cDev->PciBar0);
//  QuickI2cDev->DeviceRegisters.TouchCfg.Fields.TouchEnable = 0x1;
//  QuickI2cLibWriteSeqData1 (QuickI2cDev->PciBar0, QuickI2cDev->DeviceRegisters.TouchCfg.Data);
  QuickI2cLibGo (QuickI2cDev->PciBar0);

  Status = QuickI2cLibWaitCycleCompleted (QuickI2cDev->PciBar0, CycleTimeout);
  THC_LOCAL_DEBUG (L"QuickI2cEnableEndDevice  QuickI2cLibWaitCycleCompleted Status: %r\n", Status)
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cEnableEndDevice error, Status %r\n", Status));
    return Status;
  }
**/
  return EFI_NOT_AVAILABLE_YET;
}

/**
  Reads HID Descriptor

  @param[in]  QuickI2cDev           Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to request for report descriptro
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cReadHidDescriptor (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  )
{
  EFI_STATUS          Status;
  UINT32              Timeout;
  UINT32              HidMessageLength;
  UINT8               *Buffer;
  UINT32              RxDlenEn;
  UINT32              Wbc;
  UINT32              Address;

  THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor Start()\n")

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = TRUE;
  Buffer = QuickI2cDev->HidBuffer;

  ZeroMem (QuickI2cDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);

  RxDlenEn            = QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_0;
  Wbc                 = sizeof (QuickI2cDev->DeviceDescriptor.wReportDescRegister);
  Address             = QuickI2cDev->DeviceDescriptor.wReportDescRegister;

  THC_LOCAL_DEBUG(L"QuickI2cReadHidDescriptor: QuickI2c SwDma Active !\n")
  QuickI2cDev->SwDmaActive = TRUE;

  Status = QuickI2cSwDma (QuickI2cDev, NULL, RxDlenEn, Wbc, Address);
  THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor QuickI2cSwDma: %r\n", Status)
  if (EFI_ERROR (Status)) {
    QuickI2cDev->SwDmaActive = FALSE;
    THC_LOCAL_DEBUG(L"QuickI2cReadHidDescriptor: QuickI2c SwDma End\n")
    return Status;
  }

  Timeout = HidResponseTimeout;

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor Waiting for Response \n")
    do {
      if (QuickI2cDev->HidDataAvailable == TRUE && QuickI2cDev->ReadDone == TRUE) {
        Status = EFI_SUCCESS;
        THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor Response received %r\n", Status)
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor Response Timeout\n")
      DEBUG ((DEBUG_ERROR, "QuickI2cReadHidDescriptor Response Timeout\n"));
      //
      // Even for error case need to clear out everything
      //
      QuickI2cCompleteSwdma (QuickI2cDev);

      // Just to make sure interrupt is disabled since its init flow
      THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor: QuickI2cDisableInterrupt\n")
      QuickI2cDisableInterrupt (QuickI2cDev);
      QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->SwDmaRead, 0);
      QuickI2cDev->SwDmaActive = FALSE;
      THC_LOCAL_DEBUG(L"QuickI2cReadHidDescriptor: QuickI2c SwDma End\n")
      return EFI_TIMEOUT;
    }
  }

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  //
  // Restore initial pointer
  //
  QuickI2cDev->HidBuffer = Buffer;

  //
  // Calculate Recieved data length
  //
  HidMessageLength = QuickI2cDev->SwDmaMessageLength;

  //
  // Parse HID data
  //
  HidParseDescriptor (QuickI2cDev, QuickI2cDev->HidBuffer, HidMessageLength);

  //
  // Complete the process
  //
  QuickI2cCompleteSwdma (QuickI2cDev);

  // Just to make sure interrupt is disabled since its init flow, complete the flow
  QuickI2cDisableInterrupt (QuickI2cDev);
  QuickI2cDev->SwDmaMessageLength = 0;

  // Reset PRD table settings
  //
  QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->SwDmaRead, 0);
  QuickI2cDev->SwDmaActive = FALSE;
  THC_LOCAL_DEBUG (L"QuickI2cReadHidDescriptor SwDmaActive %r End\n", Status)
  return EFI_SUCCESS;
}

/**
  Touch Enable

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to Set Feature command
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cTouchEnable (
  IN  QUICK_I2C_DEV   *QuickI2cDev
  )
{
  EFI_STATUS                      Status;
  UINT8                           Content[3] = {0xE, 0x2, 0x0};
  UINT8                           *WriteBuffer;
  UINT16                          LenghtField;
  UINT8                           TotalLength;
  QUICK_I2C_COMMAND_REQUEST       QuickI2cCommandRequest;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;

  THC_LOCAL_DEBUG (L"QuickI2cTouchEnable\n")

  if ((QuickI2cDev->HidSolutionFlag == ElanHidProtocol) || (QuickI2cDev->InputReportTable.TouchPad)) {
    THC_LOCAL_DEBUG (L"QuickI2cTouchEnable: Skipping it !!\n")
    DEBUG ((DEBUG_INFO, "QuickI2cTouchEnable: Skipping it !!\n"));
    return EFI_SUCCESS;
  }

  SetMem (&QuickI2cCommandRequest, sizeof (QUICK_I2C_COMMAND_REQUEST), 0);
  SetMem (&OutputReportHeader,sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER), 0);

  QuickI2cDev->HidActive        = TRUE;
  TotalLength                   = 0;
  QuickI2cDev->HidBuffer        = AllocateAlignedPages (EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE), sizeof (UINT8));
  if (QuickI2cDev->HidBuffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  OutputReportHeader.Fields.ReportType    = QuickI2cOutputReportSetFeature;

  // First add the command register LSB + MSB to the TXDMA buffer
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }

 // For optional third byte, add the data register LSB + MSB to the TXDMA buffer
  OutputReportHeader.Fields.ContentLength = sizeof (QUICK_I2C_COMMAND_REQUEST) +
                                            sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister) +
                                            sizeof (LenghtField) +
                                            sizeof (Content);

  WriteBuffer = AllocateZeroPool (OutputReportHeader.Fields.ContentLength);
  if (WriteBuffer == NULL) {
    FreeAlignedPages (QuickI2cDev->HidBuffer, EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE));
    return EFI_OUT_OF_RESOURCES;
  }

  //
  // Send command request
  //
  QuickI2cCommandRequest.Fields.High.Opcode     = (UINT8) QuickI2cSubIpOpcodeSet;
  QuickI2cCommandRequest.Fields.High.Reserved   = 0b0000;
  QuickI2cCommandRequest.Fields.Low.ReportID    = QUICK_I2C_TOUCH_ENABLE_REQUEST_REPORT_ID;
  QuickI2cCommandRequest.Fields.Low.ReportType  = (UINT8) QuickI2cReportTypeFeature;
  QuickI2cCommandRequest.Fields.Low.Reserved    = 0b00;

  CopyMem (WriteBuffer, (VOID*)&(QuickI2cCommandRequest), sizeof (QUICK_I2C_COMMAND_REQUEST));
  TotalLength += sizeof (QUICK_I2C_COMMAND_REQUEST);

  //
  // Add the data register LSB + MSB to the TXDMA buffer if there is a data to send
  //
  if (QuickI2cDev->DeviceDescriptor.wDataRegister) {
      CopyMem (WriteBuffer + TotalLength, (VOID*)&(QuickI2cDev->DeviceDescriptor.wDataRegister), sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister));
      TotalLength += sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
  }

  //
  // Copy Length of data field and Content itself to TxDma write buffer
  //
  LenghtField = sizeof (UINT16) + sizeof (Content);
  CopyMem (WriteBuffer + TotalLength, (VOID*)&LenghtField, sizeof (UINT16));
  TotalLength += sizeof (UINT16);
  CopyMem (WriteBuffer + TotalLength, (VOID*)&Content, sizeof (Content));

  // No response is needed for QuickI2cOutputReportSetFeature, WriteDmaPollComplete is enough
  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, WriteBuffer);
  THC_LOCAL_DEBUG (L"QuickI2cTouchEnable QuickI2cDmaWriteTx: %r\n", Status)
  if (EFI_ERROR (Status)) {
    FreeAlignedPages (QuickI2cDev->HidBuffer, EFI_SIZE_TO_PAGES (THC_MAX_HID_BUFFER_SIZE));
    DEBUG ((DEBUG_INFO, "QuickI2cTouchEnable QuickI2cDmaWriteTx: %r, might be expected\n", Status));
  }
  FreePool (WriteBuffer);
  return EFI_SUCCESS;
}

/**
  Send Power on request to SubIP

  @param[in]  MmioBase         QuickI2c MMIO BAR0
  @param[in]  PowerState       PowerState

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendSetPowerRequest (
  IN QUICK_I2C_DEV    *QuickI2cDev,
  IN UINT8            PowerState
  )
{
  EFI_STATUS                      Status;
  QUICK_I2C_SET_POWER_REQUEST     QuickI2cSetPowerReq;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;

  THC_LOCAL_DEBUG (L"QuickI2cSendSetPowerRequest Entry() \n");

  SetMem (&QuickI2cSetPowerReq, sizeof(QUICK_I2C_SET_POWER_REQUEST), 0);
  SetMem (&OutputReportHeader,sizeof(QUICK_I2C_OUTPUT_REPORT_HEADER), 0);

  //
  // As per QuickI2c Spec only ContentLength and Command Register will be passed to THC HW PRD
  //
  OutputReportHeader.Fields.ReportType    = QuickI2cOutputReportCommandContent;
  OutputReportHeader.Fields.ContentLength = sizeof (QUICK_I2C_SET_POWER_REQUEST);

  //
  // First add the command register LSB + MSB to the TXDMA buffer
  //
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }

  //
  // First add the command register LSB+MSB to the TXDMA buffer
  //
  QuickI2cSetPowerReq.Fields.High.Opcode    = (UINT8) QuickI2cSubIpOpcodeSetPower;
  QuickI2cSetPowerReq.Fields.High.Reserved  = 0b0000;
  QuickI2cSetPowerReq.Fields.Low.PowerState = (UINT8) PowerState;
  QuickI2cSetPowerReq.Fields.Low.Reserved   = 0b000000;

  //
  // For Power On request thier is no response expected
  //
  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, (UINT8 *) &QuickI2cSetPowerReq);
  if (EFI_ERROR (Status)) {
    THC_LOCAL_DEBUG (L"QuickI2cSendSetPowerRequest: QuickI2cDmaWriteTx Error Status %r\n", Status)
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  Send Reset request to SubIP

  @param[in]  MmioBase         QuickI2c MMIO BAR0

  @retval EFI_SUCCESS          No Cycles running
  @retval EFI_TIMEOUT          Timeout
**/
EFI_STATUS
QuickI2cSendResetRequest (
  IN QUICK_I2C_DEV      *QuickI2cDev
  )
{
  EFI_STATUS                      Status;
  UINT32                          Timeout;
  QUICK_I2C_COMMAND_REQUEST       QuickI2cCommandRequest;
  QUICK_I2C_OUTPUT_REPORT_HEADER  OutputReportHeader;
  THC_M_PRT_INT_STATUS            ThcIntStatus;
  RESET_RESPONSE_DATA             ResetResponseData;

  THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Entry() \n");

  SetMem (&QuickI2cCommandRequest, sizeof (QUICK_I2C_COMMAND_REQUEST), 0);
  SetMem (&OutputReportHeader, sizeof (QUICK_I2C_OUTPUT_REPORT_HEADER), 0);
  SetMem (&ResetResponseData, sizeof (RESET_RESPONSE_DATA), 0);

  OutputReportHeader.Fields.ReportType    = QuickI2cOutputReportCommandContent;
  OutputReportHeader.Fields.ContentLength = sizeof (QUICK_I2C_COMMAND_REQUEST);
  //
  // First add the command register LSB + MSB to the TXDMA buffer
  //
  if (QuickI2cDev->DeviceDescriptor.wCommandRegister) {
    OutputReportHeader.Fields.CommandRegister = QuickI2cDev->DeviceDescriptor.wCommandRegister;
  }

  //
  // Send Reset Request
  //
  QuickI2cCommandRequest.Fields.High.Opcode     = (UINT8) QuickI2cSubIpOpcodeReset;
  QuickI2cCommandRequest.Fields.High.Reserved   = 0b0000;
  QuickI2cCommandRequest.Fields.Low.ReportID    = QUICK_I2C_RESET_REQUEST_REPORT_ID;
  QuickI2cCommandRequest.Fields.Low.ReportType  = (UINT8) QuickI2cReportTypeReset;
  QuickI2cCommandRequest.Fields.Low.Reserved    = 0b00;

  Timeout = WaitForResetTimeout;

   // Set other registers to clear INT status
  ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
  ThcIntStatus.Fields.DevRawIntSts = 1;
  MmioWrite32 ((UINTN)(QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS), ThcIntStatus.Data32);
  THC_LOCAL_DEBUG (L"Instance %d:, Before sending reset request DEV_RAW_INT_STS = 0x%X\n", QuickI2cDev->InstanceId, MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS))
  Status = QuickI2cDmaWriteTx (QuickI2cDev, OutputReportHeader, (UINT8 *) &QuickI2cCommandRequest);
  THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest: QuickI2cDmaWriteTx Status %r\n", Status)
  if (EFI_ERROR (Status)) {
    return Status;
  }

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Waiting for Response DEV_RAW_INT_STS = 0x%X \n", MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS))
    DEBUG ((DEBUG_INFO, "QuickI2cSendResetRequest Waiting for Response DEV_RAW_INT_STS = 0x%X\n", MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS)));
    do {
      ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
      if (ThcIntStatus.Fields.DevRawIntSts) {
        Status = QuickI2cLibPerformPioRead (
                  QuickI2cDev->PciBar0,
                  QuickI2cDev->DeviceDescriptor.wInputRegister,
                  sizeof (RESET_RESPONSE_DATA),
                  &ResetResponseData.Data32
                );
        if (!EFI_ERROR (Status)) {
          DEBUG ((DEBUG_INFO, "QuickI2cLibPerformPioRead: Complete, Status %r\n", Status));
          //
          // Clear int status bit
          //
          ThcIntStatus.Data32 = MmioRead32 (QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS);
          ThcIntStatus.Fields.DevRawIntSts = 1;
          MmioWrite32 ((UINTN)(QuickI2cDev->PciBar0 + R_THC_MEM_PRT_INT_STATUS), ThcIntStatus.Data32);
          Status = EFI_SUCCESS;
          break;
        }
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      // Output reponse is optional
      THC_LOCAL_DEBUG (L"QuickI2cSendResetRequest Response Timeout might be expected\n")
      DEBUG ((DEBUG_WARN, "QuickI2cSendResetRequest Response Timeout might be expected\n"));
    }
  }


  return EFI_SUCCESS;
}

/**
  Get Feature via SwDma

  @param[in]  QuickI2cDev      Context of QuickI2c device

  @retval EFI_SUCCESS          Enabling completed
  @retval EFI_NO_RESPONSE      Device did not respond to request for report descriptro
  @retval EFI_TIMEOUT          Timeout while waiting for response from the device
  @retval EFI_OUT_OF_RESOURCES Unable to allocate buffer for QuickI2c HID support
**/
EFI_STATUS
QuickI2cGetFeatureViaSwDma (
  IN  QUICK_I2C_DEV              *QuickI2cDev
  )
{
  EFI_STATUS          Status;
  UINT32              Timeout;
  UINT32              RxDlenEn;
  UINT32              Wbc;
  UINT32              WriteDataHeader;
  UINT32              Data;
  UINT8               rId = 0;
  QUICK_I2C_COMMAND_REQUEST Quick2cGetFeatureRequest;

  THC_LOCAL_DEBUG (L"QuickI2cGetFeatureViaSwDma Start()\n")

  if (QuickI2cDev->ReportPacket.FeatureReportAvailable == FALSE) {
    DEBUG ((DEBUG_INFO, "QuickI2cGetFeatureViaSwDma: FeatureReportAvailable = %d not available \n", QuickI2cDev->ReportPacket.FeatureReportAvailable));
    THC_LOCAL_DEBUG(L"QuickI2cGetFeatureViaSwDma: FeatureReportAvailable = %d not available \n", QuickI2cDev->ReportPacket.FeatureReportAvailable)
    return EFI_SUCCESS;
  }
  DEBUG ((DEBUG_INFO, "QuickI2cGetFeatureViaSwDma: For ReportId = 0x%x\n", QuickI2cDev->ReportPacket.ReportId));

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = TRUE;
  ZeroMem (QuickI2cDev->HidBuffer, THC_MAX_HID_BUFFER_SIZE);
  ZeroMem (&Quick2cGetFeatureRequest, sizeof (QUICK_I2C_COMMAND_REQUEST));

  //
  // Send Reset Request
  //
  Quick2cGetFeatureRequest.Fields.High.Opcode     = QuickI2cSubIpOpcodeGet;
  Quick2cGetFeatureRequest.Fields.High.Reserved   = 0b0000;
  Quick2cGetFeatureRequest.Fields.Low.ReportType  = QuickI2cReportTypeFeature;
  Quick2cGetFeatureRequest.Fields.Low.Reserved    = 0b00;

  // need to check reportID value to determine if the third optional byte is needed and also determine the write-byte-size
  if (QuickI2cDev->ReportPacket.ReportId < QUICKI_I2C_THC_SWDMA_WRITE_BYTE_SIZE_LIMIT) {
    Quick2cGetFeatureRequest.Fields.Low.ReportID  = (UINT8)QuickI2cDev->ReportPacket.ReportId;
    RxDlenEn = QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_1;
    Wbc = sizeof (QuickI2cDev->DeviceDescriptor.wCommandRegister) + sizeof (Quick2cGetFeatureRequest) + sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
    WriteDataHeader = QuickI2cDev->DeviceDescriptor.wCommandRegister | (Quick2cGetFeatureRequest.Data << 16);
    Data = QuickI2cDev->DeviceDescriptor.wDataRegister;
  } else {
    Quick2cGetFeatureRequest.Fields.Low.ReportID = 0b1111;
    rId = (UINT8)QuickI2cDev->ReportPacket.ReportId;
    RxDlenEn = QUICK_I2C_THC_SWDMA_I2C_RX_DLEN_EN_1;
    Wbc = sizeof (QuickI2cDev->DeviceDescriptor.wCommandRegister) + sizeof (Quick2cGetFeatureRequest) + sizeof (rId) + sizeof (QuickI2cDev->DeviceDescriptor.wDataRegister);
    WriteDataHeader = QuickI2cDev->DeviceDescriptor.wCommandRegister | (Quick2cGetFeatureRequest.Data << 16);
    Data = (QuickI2cDev->DeviceDescriptor.wDataRegister << 8) | rId;
  }

  THC_LOCAL_DEBUG(L"QuickI2cGetFeatureViaSwDma: RxDlenEn = %d, Wbc = %d, WriteDataHeader= 0x%x, Data= 0x%x\n", RxDlenEn, Wbc, WriteDataHeader, Data)
  THC_LOCAL_DEBUG(L"QuickI2cGetFeatureViaSwDma: QuickI2c SwDma Active !\n")
  QuickI2cDev->SwDmaActive = TRUE;
  Status = QuickI2cSwDma (QuickI2cDev, &Data, RxDlenEn, Wbc, WriteDataHeader);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "QuickI2cGetFeatureViaSwDma QuickI2cSwDma return Status: %r\n", Status));
    // return status is optional
  }

  Timeout = HidResponseTimeout;

  //
  // Wait for Response
  //
  if (Timeout > 0) {
    DEBUG ((DEBUG_INFO, "QuickI2cGetFeatureViaSwDma Waiting for Response \n"));
    do {
      if (QuickI2cDev->HidDataAvailable == TRUE && QuickI2cDev->ReadDone == TRUE) {
        THC_LOCAL_DEBUG (L"QuickI2cGetFeatureViaSwDma Response received\n")
        DEBUG ((DEBUG_INFO, "QuickI2cGetFeatureViaSwDma Response received\n"));
        Status = EFI_SUCCESS;
        break;
      }
      MicroSecondDelay (1000);
      Timeout--;
    } while (Timeout > 0);

    if (Timeout == 0) {
      THC_LOCAL_DEBUG (L"QuickI2cGetFeatureViaSwDma Response Timeout might be optional\n")
      DEBUG ((DEBUG_ERROR, "QuickI2cGetFeatureViaSwDma Response Timeout might be optional\n"));
      // return status is optional
    }
  }

  QuickI2cDev->HidDataAvailable = FALSE;
  QuickI2cDev->HidActive        = FALSE;

  //
  // complete the process
  //
  QuickI2cCompleteSwdma (QuickI2cDev);
  QuickI2cDev->SwDmaMessageLength = 0;

  QuickI2cDmaFillSinglePrdTable (&QuickI2cDev->SwDmaRead, 0);
  QuickI2cDev->SwDmaActive = FALSE;
  DEBUG ((DEBUG_INFO, "QuickI2cGetFeatureViaSwDma SwDmaActive End\n"));
  return EFI_SUCCESS;
}
