/**@file
  Private Header file for Touch Host Controller QuickI2c Driver Sub IP

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

#ifndef _QUICK_I2C_SUB_IP_H_
#define _QUICK_I2C_SUB_IP_H_


//offset 0x0
typedef union {
  struct {
    UINT32 MasterMode               : 1;
    UINT32 Speed                    : 2;
    UINT32 Ic10BitAddrSlave         : 1;
    UINT32 Ic10BitAddrMaster        : 1;
    UINT32 IcRestartEn              : 1;
    UINT32 IcSlaveDisable           : 1;
    UINT32 StopDetIfAddressed       : 1;
    UINT32 TxEmptyCtrl              : 1;
    UINT32 RxFifoFullHldCtrl        : 1;
    UINT32 StopDetIfMasterActive    : 1;
    UINT32 BusClearFeatureCtrl      : 1;
    UINT32 Rsvd0                    : 4;
    UINT32 OptionalSarCtrl          : 1;
    UINT32 SmbusSlaveQuickEn        : 1;
    UINT32 SmbusArpEn               : 1;
    UINT32 SmbusPersistantSlvAddrEn : 1;
    UINT32 Rsvd1                    : 12;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CON;

//offset 0x4
typedef union {
  struct {
    UINT32 IcTar              : 10;
    UINT32 GcOrStart          : 1;
    UINT32 Special            : 1;
    UINT32 Ic10BitAddrMaster  : 1;
    UINT32 DdeviceId          : 1;
    UINT32 Rsvd0              : 2;
    UINT32 SmbusQuickCmd      : 1;
    UINT32 Rsvd1              : 15;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_TAR;

//offset 0x8
typedef union {
  struct {
    UINT32 IcSar            : 10;
    UINT32 Rsvd0            : 22;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_HS_MADDR;

//offset 0xc
typedef union {
  struct {
    UINT32 IcHsMar          : 3;
    UINT32 Rsvd0            : 29;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SAR;

//offset 0x10
typedef union {
  struct {
    UINT32 Data             : 8;
    UINT32 Cmd              : 1;
    UINT32 Stop             : 1;
    UINT32 Restart          : 1;
    UINT32 FirstDataByte    : 1;
    UINT32 Rsvd0            : 20;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_DATA_CMD;

//offset 0x14
typedef union {
  struct {
    UINT32 IcSsSclHcnt      : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SS_SCL_HCNT;

//offset 0x14
typedef union {
  struct {
    UINT32 IcUfmSclHcnt     : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_UFM_SCL_HCNT;

//offset 0x18
typedef union {
  struct {
    UINT32 IcSsSclLcnt      : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SS_SCL_LCNT;

//offset 0x1c
typedef union {
  struct {
    UINT32 IcFsSclHcnt      : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_FS_SCL_HCNT;

//offset 0x1c
typedef union {
  struct {
    UINT32 IcUfmTbufCnt     : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_UFM_TBUF_CNT;

//offset 0x20
typedef union {
  struct {
    UINT32 IcFsSclLcnt      : 16;
    UINT32 Rsvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_FS_SCL_LCNT;

//offset 0x24
typedef union {
  struct {
    UINT32 IcHsSclHcnt      : 16;
    UINT32 Rcvd0            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_HS_SCL_HCNT;

//offset 0x28
typedef union {
  struct {
    UINT32 IcHsSclLcnt      : 16;
    UINT32 RSVD1            : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_HS_SCL_LCNT;

//offset 0x2c
typedef union {
  struct {
    UINT32 RRxUnder         : 1;
    UINT32 RRxOver          : 1;
    UINT32 RRxFull          : 1;
    UINT32 RTxOver          : 1;
    UINT32 RTxEmpty         : 1;
    UINT32 RRdReq           : 1;
    UINT32 RTxAbrt          : 1;
    UINT32 RRxDone          : 1;
    UINT32 RActivity        : 1;
    UINT32 RStopDet         : 1;
    UINT32 RStartDet        : 1;
    UINT32 RGenCall         : 1;
    UINT32 RRestartDet      : 1;
    UINT32 RMasterOnHold    : 1;
    UINT32 RSclStuckAtLow   : 1;
    UINT32 Rsvd0            : 17;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_INTR_STAT;

//offset 0x30
typedef union {
  struct {
    UINT32 MRxUnder         : 1;
    UINT32 MRxOver          : 1;
    UINT32 MRxFull          : 1;
    UINT32 MTxOver          : 1;
    UINT32 MTxEmpty         : 1;
    UINT32 MRdReq           : 1;
    UINT32 MTxAbrt          : 1;
    UINT32 MRxDone          : 1;
    UINT32 MActivity        : 1;
    UINT32 MStopDet         : 1;
    UINT32 MStartDet        : 1;
    UINT32 MGenCall         : 1;
    UINT32 MRestartDet      : 1;
    UINT32 MMasterOnHold    : 1;
    UINT32 MSclStuckAtLow   : 1;
    UINT32 Rsvd0            : 17;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_INTR_MASK;

//offset 0x34
typedef union {
  struct {
    UINT32 RxUnder          : 1;
    UINT32 RxOver           : 1;
    UINT32 RxFull           : 1;
    UINT32 TxOver           : 1;
    UINT32 TxEmpty          : 1;
    UINT32 RdReq            : 1;
    UINT32 TxAbrt           : 1;
    UINT32 RxDone           : 1;
    UINT32 Activity         : 1;
    UINT32 StopDet          : 1;
    UINT32 StartDet         : 1;
    UINT32 GenCall          : 1;
    UINT32 RestartDet       : 1;
    UINT32 MasterOnHold     : 1;
    UINT32 SclStuckAtLow    : 1;
    UINT32 Rsvd0            : 17;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_RAW_INTR_STAT;

//offset 0x38
typedef union {
  struct {
    UINT32 RxTl             : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_RX_TL;

//offset 0x3c
typedef union {
  struct {
    UINT32 TxTl             : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_TX_TL;

//offset 0x40
typedef union {
  struct {
    UINT32 ClrIntr          : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_INTR;

//offset 0x44
typedef union {
  struct {
    UINT32 ClrRxUnder       : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_RX_UNDER;

//offset 0x48
typedef union {
  struct {
    UINT32 ClrRxOver        : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_RX_OVER;

//offset 0x4c
typedef union {
  struct {
    UINT32 ClrTxOver        : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_TX_OVER;

//offset 0x50
typedef union {
  struct {
    UINT32 ClrRdReq         : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_RD_REQ;

//offset 0x54
typedef union {
  struct {
    UINT32 ClrTxAbrt        : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_TX_ABRT;

//offset 0x58
typedef union {
  struct {
    UINT32 ClrRxDone        : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_RX_DONE;

//offset 0x5c
typedef union {
  struct {
    UINT32 ClrActivity      : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_ACTIVITY;

//offset 0x60
typedef union {
  struct {
    UINT32 ClrStopDet       : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_STOP_DET;

//offset 0x64
typedef union {
  struct {
    UINT32 ClrStartDet      : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_START_DET;

//offset 0x68
typedef union {
  struct {
    UINT32 ClrGenCall       : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_GEN_CALL;

//offset 0x6c
typedef union {
  struct {
    UINT32 Enable                 : 1;
    UINT32 Abort                  : 1;
    UINT32 TxCmdBlock             : 1;
    UINT32 SdaStuckRecoveryEnable : 1;
    UINT32 Rsvd0                  : 12;
    UINT32 SmbusClkReset          : 1;
    UINT32 SmbusSuspendEn         : 1;
    UINT32 SmbusAlertEn           : 1;
    UINT32 Rsvd1                  : 13;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_ENABLE;

//offset 0x70
typedef union {
  struct {
    UINT32 Activity               : 1;
    UINT32 Tfnf                   : 1;
    UINT32 Tfe                    : 1;
    UINT32 Rfne                   : 1;
    UINT32 Rff                    : 1;
    UINT32 MstActivity            : 1;
    UINT32 SlvActivity            : 1;
    UINT32 MstHoldTxFifoEmpty     : 1;
    UINT32 MstHoldRxFifoFull      : 1;
    UINT32 SlvHoldTxFifoEmpty     : 1;
    UINT32 SlsHoldRxFifoFull      : 1;
    UINT32 SdaStuckNotRecovered   : 1;
    UINT32 Rsvd0                  : 4;
    UINT32 SmbusQuickCmdBit       : 1;
    UINT32 SmbusSlaveAddrValid    : 1;
    UINT32 SmbusSlaveAddrResolved : 1;
    UINT32 SmbusSuspendStatus     : 1;
    UINT32 SmbusAlertStatus       : 1;
    UINT32 Rsvd1                  : 11;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_STATUS;

//offset 0x74
typedef union {
  struct {
    UINT32 Txflr            : 32; // bit 0 to (TX_ABW_P1 - 1)
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_TXFLR;

//offset 0x78
typedef union {
  struct {
    UINT32 Rxflr            : 32; // bit 0 to (RX_ABW_P1 - 1)
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_RXFLR;

//offset 0x7c
typedef union {
  struct {
    UINT32 IcSdaTxHold      : 16;
    UINT32 IcSdaRxHold      : 8;
    UINT32 Rsvd0            : 8;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SDA_HOLD;

//offset 0x80
typedef union {
  struct {
    UINT32 Abrt7bAddrNoAck          : 1;
    UINT32 Abrt10Addr1NoAck         : 1;
    UINT32 Abrt10Addr2NoAck         : 1;
    UINT32 AbrtTxDataNoAck          : 1;
    UINT32 AbrtGcallNoAck           : 1;
    UINT32 AbrtGcallRead            : 1;
    UINT32 AbrtHsAckDet             : 1;
    UINT32 AbrtSByteAckDet          : 1;
    UINT32 AbrtHsNorStrt            : 1;
    UINT32 AbrtSbyteNoRstrt         : 1;
    UINT32 Abrt10BRdNoRstrt         : 1;
    UINT32 AbrtMasterDis            : 1;
    UINT32 ArbLost                  : 1;
    UINT32 AbrtSlvFlushTxFifo       : 1;
    UINT32 AbrtSlvArbLost           : 1;
    UINT32 AbrtSlvRdIntx            : 1;
    UINT32 AbrtUserAbrt             : 1;
    UINT32 AbrSdaStuckAtLow         : 1;
    UINT32 AbrtDeviceNoAck          : 1;
    UINT32 AbrtDeviceSlvAddrNoAck   : 1;
    UINT32 AbrtDeviceWrite          : 1;
    UINT32 Rsvd0                    : 2;
    UINT32 TxFluchCnt               : 9;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_TX_ABRT_SOURCE;

//offset 0x84
typedef union {
  struct {
    UINT32 Nack             : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SLV_DATA_NACK_ONLY;

//offset 0x88
typedef union {
  struct {
    UINT32 Rdmae            : 1;
    UINT32 Tdmae            : 1;
    UINT32 Rsvd0            : 30;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_DMA_CR;

//offset 0x8c
typedef union {
  struct {
    UINT32 Dmatdl           : 32; // bit 0 to (TX_ABW - 1)
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_DMA_TDLR;

//offset 0x90
typedef union {
  struct {
    UINT32 Dmadrl           : 32; // bit 0 to (RX_ABW - 1)
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_DMA_RDLR;

//offset 0x94
typedef union {
  struct {
    UINT32 SdaSetup         : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SDA_SETUP;

//offset 0x98
typedef union {
  struct {
    UINT32 AckGenCall       : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_ACK_GENERAL_CALL;

//offset 0x9c
typedef union {
  struct {
    UINT32 IcEn                 : 1;
    UINT32 SlvDisabledWhileBusy : 1;
    UINT32 SlvRxDataLost        : 1;
    UINT32 Rsvd0                : 29;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_ENABLE_STATUS;

//offset 0xa0
typedef union {
  struct {
    UINT32 IcFsSpklen       : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_FS_SPKLEN;

//offset 0xa0
typedef union {
  struct {
    UINT32 IcUfmSpklen      : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_UFM_SPKLEN;

//offset 0xa4
typedef union {
  struct {
    UINT32 IcHsSpklen       : 8;
    UINT32 Rsvd0            : 24;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_HS_SPKLEN;

//offset 0xa8
typedef union {
  struct {
    UINT32 ClrRestartDet    : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_RESTART_DET;

//offset 0xac
typedef union {
  struct {
    UINT32 IcSclStuckLowTimeout : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SCL_STUCK_AT_LOW_TIMEOUT;

//offset 0xb0
typedef union {
  struct {
    UINT32 IcSdaStuckLowTimeout : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SDA_STUCK_AT_LOW_TIMEOUT;

//offset 0xb4
typedef union {
  struct {
    UINT32 ClrSclStuckDet   : 1;
    UINT32 Rsvd0            : 31;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_SCL_STUCK_DET;

//offset 0xb8
typedef union {
  struct {
    UINT32 DeviceId         : 24;
    UINT32 Rsvd0            : 8;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_DEVICE_ID;

//offset 0xbc
typedef union {
  struct {
    UINT32 SmbusClkLowSextTimeout : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_CLK_LOW_SEXT;

//offset 0xc0
typedef union {
  struct {
    UINT32 SmbusClkLowMextTimeout : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_CLK_LOW_MEXT;

//offset 0xc4
typedef union {
  struct {
    UINT32 SmbusThighMaxBusIdleCnt  : 16;
    UINT32 Rsvd0                    : 16;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_THIGH_MAX_IDLE_COUNT;

//offset 0xc8
typedef union {
  struct {
    UINT32 RSlvClockExtndTimeout  : 1;
    UINT32 RMstClockExtndTimeout  : 1;
    UINT32 RQuickCmdDet           : 1;
    UINT32 RHostNtfyMstDet        : 1;
    UINT32 RArpPrepareCmdDet      : 1;
    UINT32 RArpRstCmdDet          : 1;
    UINT32 RArpGetUdidCmdDet      : 1;
    UINT32 RArpAssgnAddrCmdDet    : 1;
    UINT32 RSlvRxPecNack          : 1;
    UINT32 RSmbusSuspendDet       : 1;
    UINT32 RSmbusAlertDet         : 1;
    UINT32 Rsvd0                  : 21;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_INTR_STAT;

//offset 0xcc
typedef union {
  struct {
    UINT32 MSlvClockExtndTimeout  : 1;
    UINT32 MMstClockExtndTimeout  : 1;
    UINT32 MQuickCmdDet           : 1;
    UINT32 MHostNtfyMstDet        : 1;
    UINT32 MArpPrepareCmdDet      : 1;
    UINT32 MArpRstCmdDet          : 1;
    UINT32 MArpGetUdidCmdDet      : 1;
    UINT32 MArpAssgnAddrCmdDet    : 1;
    UINT32 MSlvRxPecNack          : 1;
    UINT32 MSmbusSuspendDet       : 1;
    UINT32 MSmbusAlertDet         : 1;
    UINT32 Rsvd0                  : 21;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_INTR_MASK;

//offset 0xd0
typedef union {
  struct {
    UINT32 SlvClockExtndTimeout   : 1;
    UINT32 MstClockExtndTimeout   : 1;
    UINT32 QuickCmdDet            : 1;
    UINT32 HostNtfyMstDet         : 1;
    UINT32 ArpPrepareCmdDet       : 1;
    UINT32 ArpRstCmdDet           : 1;
    UINT32 ArpGetUdidCmdDet       : 1;
    UINT32 ArpAssgnAddrCmdDet     : 1;
    UINT32 SlvRxPecNack           : 1;
    UINT32 SmbusSuspendDet        : 1;
    UINT32 SmbusAlertDet          : 1;
    UINT32 Rsvd0                  : 21;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_RAW_INTR_STAT;

//offset 0xd4
typedef union {
  struct {
    UINT32 ClrSlvClockExtndTimeout  : 1;
    UINT32 ClrMstClockExtndTimeout  : 1;
    UINT32 ClrQuickCmdDet           : 1;
    UINT32 ClrHostNotifyMstDet      : 1;
    UINT32 ClrArpPrepareCmdDet      : 1;
    UINT32 ClrArpRstCmdDet          : 1;
    UINT32 ClrArpGetUdidCmdDet      : 1;
    UINT32 ClrArpAssgnAddrCmdDet    : 1;
    UINT32 ClrSlvRxPecNack          : 1;
    UINT32 ClrSmbusSuspendDet       : 1;
    UINT32 ClrSmbusAlertDet         : 1;
    UINT32 Rsvd0                    : 21;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_CLR_SMBUS_INTR;

//offset 0xd8
typedef union {
  struct {
    UINT32 OptionalSar      : 7;
    UINT32 Rsvd0            : 25;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_OPTIONAL_SAR;

//offset 0xdc
typedef union {
  struct {
    UINT32 SmbusUdidWord0   : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_UDID_LSB;

//offset 0xe0
typedef union {
  struct {
    UINT32 SmbusUdidWord1   : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_UDID_WORD1;

//offset 0xe4
typedef union {
  struct {
    UINT32 SmbusUdidWord2   : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_UDID_WORD2;

//offset 0xe8
typedef union {
  struct {
    UINT32 SmbusUdidWord3   : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_SMBUS_UDID_WORD3;

//offset 0xf0
typedef union {
  struct {
    UINT32 RegTimeoutRst    : 32; // bit 0 to (REG_TIMEOUT_WIDTH - 1)
  } Fields;
  UINT32 Data32;
} THC_I2C_REG_TIMEOUT_RST;

//offset 0xf4
typedef union {
  struct {
    UINT32 ApbDataWidth     : 2;
    UINT32 MaxSpeedMode     : 2;
    UINT32 HcCountValues    : 1;
    UINT32 IntrIo           : 1;
    UINT32 HasDma           : 1;
    UINT32 AddEncodedParams : 1;
    UINT32 RxBufferDepth    : 8;
    UINT32 TxBufferDepth    : 8;
    UINT32 Rsvd0            : 8;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_COMP_PARAM_1;

//offset 0xf8
typedef union {
  struct {
    UINT32 IcCompVersion    : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_COMP_VERSION;

//offset 0xfc
typedef union {
  struct {
    UINT32 IcCompType       : 32;
  } Fields;
  UINT32 Data32;
} THC_I2C_IC_COMP_TYPE;

//
// THC_I2C_SUB_IP_REG_OFFSET
//
#define THC_I2C_REG_IC_CON                        0x0
#define THC_I2C_REG_IC_TAR                        0x4
#define THC_I2C_REG_IC_SAR                        0x8
#define THC_I2C_REG_IC_HS_MADDR                   0xC
#define THC_I2C_REG_IC_DATA_CMD                   0x10
#define THC_I2C_REG_IC_SS_SCL_HCNT                0x14
#define THC_I2C_REG_IC_UFM_SCL_HCNT               0x14
#define THC_I2C_REG_IC_SS_SCL_LCNT                0x18
#define THC_I2C_REG_IC_UFM_SCL_LCNT               0x18
#define THC_I2C_REG_IC_FS_SCL_HCNT                0x1C
#define THC_I2C_REG_IC_UFM_TBUF_CNT               0x1C
#define THC_I2C_REG_IC_FS_SCL_LCNT                0x20
#define THC_I2C_REG_IC_HS_SCL_HCNT                0x24
#define THC_I2C_REG_IC_HS_SCL_LCNT                0x28
#define THC_I2C_REG_IC_INTR_STAT                  0x2C
#define THC_I2C_REG_IC_INTR_MASK                  0x30
#define THC_I2C_REG_IC_RAW_INTR_STAT              0x34
#define THC_I2C_REG_IC_RX_TL                      0x38
#define THC_I2C_REG_IC_TX_TL                      0x3C
#define THC_I2C_REG_IC_CLR_INTR                   0x40
#define THC_I2C_REG_IC_CLR_RX_UNDER               0x44
#define THC_I2C_REG_IC_CLR_RX_OVER                0x48
#define THC_I2C_REG_IC_CLR_TX_OVER                0x4C
#define THC_I2C_REG_IC_CLR_RD_REQ                 0x50
#define THC_I2C_REG_IC_CLR_TX_ABRT                0x54
#define THC_I2C_REG_IC_CLR_RX_DONE                0x58
#define THC_I2C_REG_IC_CLR_ACTIVITY               0x5C
#define THC_I2C_REG_IC_CLR_STOP_DET               0x60
#define THC_I2C_REG_IC_CLR_START_DET              0x64
#define THC_I2C_REG_IC_CLR_GEN_CALL               0x68
#define THC_I2C_REG_IC_ENABLE                     0x6C
#define THC_I2C_REG_IC_STATUS                     0x70
#define THC_I2C_REG_IC_TXFLR                      0x74
#define THC_I2C_REG_IC_RXFLR                      0x78
#define THC_I2C_REG_IC_SDA_HOLD                   0x7C
#define THC_I2C_REG_IC_TX_ABRT_SOURCE             0x80
#define THC_I2C_REG_IC_SLV_DATA_NACK_ONLY         0x84
#define THC_I2C_REG_IC_DMA_CR                     0x88
#define THC_I2C_REG_IC_DMA_TDLR                   0x8C
#define THC_I2C_REG_IC_DMA_RDLR                   0x90
#define THC_I2C_REG_IC_SDA_SETUP                  0x94
#define THC_I2C_REG_IC_ACK_GENERAL_CALL           0x98
#define THC_I2C_REG_IC_ENABLE_STATUS              0x9C
#define THC_I2C_REG_IC_FS_SPKLEN                  0xA0
#define THC_I2C_REG_IC_UFM_SPKLEN                 0xA0
#define THC_I2C_REG_IC_HS_SPKLEN                  0xA4
#define THC_I2C_REG_IC_CLR_RESTART_DET            0xA8
#define THC_I2C_REG_IC_SCL_STUCK_AT_LOW_TIMEOUT   0xAC
#define THC_I2C_REG_IC_SDA_STUCK_AT_LOW_TIMEOUT   0xB0
#define THC_I2C_REG_IC_CLR_SCL_STUCK_DET          0xB4
#define THC_I2C_REG_IC_DEVICE_ID                  0xB8
#define THC_I2C_REG_IC_SMBUS_CLK_LOW_SEXT         0xBC
#define THC_I2C_REG_IC_SMBUS_CLK_LOW_MEXT         0xC0
#define THC_I2C_REG_IC_SMBUS_THIGH_MAX_IDLE_COUNT 0xC4
#define THC_I2C_REG_IC_SMBUS_INTR_STAT            0xC8
#define THC_I2C_REG_IC_SMBUS_INTR_MASK            0xCC
#define THC_I2C_REG_IC_SMBUS_RAW_INTR_STAT        0xD0
#define THC_I2C_REG_IC_CLR_SMBUS_INTR             0xD4
#define THC_I2C_REG_IC_OPTIONAL_SAR               0xD8
#define THC_I2C_REG_IC_SMBUS_UDID_LSB             0xDC
#define THC_I2C_REG_IC_SMBUS_UDID_WORD0           0xDC
#define THC_I2C_REG_IC_SMBUS_UDID_WORD1           0xE0
#define THC_I2C_REG_IC_SMBUS_UDID_WORD2           0xE4
#define THC_I2C_REG_IC_SMBUS_UDID_WORD3           0xE8
#define THC_I2C_REG_REG_TIMEOUT_RST               0xF0
#define THC_I2C_REG_IC_COMP_PARAM_1               0xF4
#define THC_I2C_REG_IC_COMP_VERSION               0xF8
#define THC_I2C_REG_IC_COMP_TYPE                  0xFC

#endif
