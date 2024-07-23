/** @file
  General GPIO V2 Pad definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _GPIOV2_PAD_MUXING_H_
#define _GPIOV2_PAD_MUXING_H_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <GpioV2Pad.h>

//
// General Gpio Pin Muxing structure
//
typedef struct {
  GPIOV2_PAD        GpioPad;
  GPIOV2_PAD_MODE   PadMode;
} GPIOV2_PIN_MUX;

//
// UART
//
typedef struct {
  GPIOV2_PIN_MUX Tx;
  GPIOV2_PIN_MUX Rx;
  GPIOV2_PIN_MUX Cts;
  GPIOV2_PIN_MUX Rts;
} GPIOV2_PIN_MUX_UART;

typedef struct {
  UINT32               PinMuxNum;
  GPIOV2_PIN_MUX_UART  *PinMux;
} GPIOV2_UART;

//
// SPI
//
typedef struct {
  GPIOV2_PIN_MUX Miso;
  GPIOV2_PIN_MUX Mosi;
  GPIOV2_PIN_MUX Clk;
  GPIOV2_PIN_MUX Cs0;
  GPIOV2_PIN_MUX Cs1;
} GPIOV2_PIN_MUX_SPI;

typedef struct {
  UINT32              PinMuxNum;
  GPIOV2_PIN_MUX_SPI  *PinMux;
} GPIOV2_SPI;

//
// HDA
//
typedef struct {
  GPIOV2_PIN_MUX Bclk;
  GPIOV2_PIN_MUX Rstb;
  GPIOV2_PIN_MUX Sync;
  GPIOV2_PIN_MUX Sdo;
  GPIOV2_PIN_MUX Sdi0;
  GPIOV2_PIN_MUX Sdi1;
} GPIOV2_PIN_MUX_HDA;

typedef struct {
  UINT32                PinMuxNum;
  GPIOV2_PIN_MUX_HDA    *PinMux;
} GPIOV2_HDA;

//
// HDA SNDW
//
typedef struct {
  GPIOV2_PIN_MUX Clk;
  GPIOV2_PIN_MUX Data;
} GPIOV2_PIN_MUX_HDA_SNDW;

typedef struct {
  UINT32                    PinMuxNum;
  GPIOV2_PIN_MUX_HDA_SNDW   *PinMux;
} GPIOV2_HDA_SNDW;

typedef struct {
  GPIOV2_PIN_MUX UsbOcPin;
} GPIOV2_PIN_MUX_USB_OVERCURRENT;

typedef struct {
  UINT32                          PinMuxNum;
  GPIOV2_PIN_MUX_USB_OVERCURRENT  *PinMux;
} GPIOV2_USB_OVERCURRENT;

typedef struct {
  GPIOV2_PIN_MUX SataLed;
} GPIOV2_PIN_MUX_SATA_LED;

typedef struct {
  UINT32                    PinMuxNum;
  GPIOV2_PIN_MUX_SATA_LED   *PinMux;
} GPIOV2_USB_SATA_LED;

typedef struct {
  GPIOV2_PIN_MUX CpuC10Gate;
} GPIOV2_PIN_MUX_CPU_C10_GATE;

typedef struct {
  UINT32                        PinMuxNum;
  GPIOV2_PIN_MUX_CPU_C10_GATE   *PinMux;
} GPIOV2_CPU_C10_GATE;

typedef struct {
  GPIOV2_PIN_MUX ScsSdCardDetect;
} GPIOV2_PIN_MUX_SCS_SDCARD_DETECT;

typedef struct {
  UINT32                             PinMuxNum;
  GPIOV2_PIN_MUX_SCS_SDCARD_DETECT   *PinMux;
} GPIOV2_SCS_SDCARD_DETECT;

typedef struct {
  GPIOV2_PIN_MUX Clk;
  GPIOV2_PIN_MUX Data;
} GPIOV2_PIN_MUX_SMBUS;

typedef struct {
  GPIOV2_PIN_MUX UsbVwOverCurrent;
} GPIOV2_PIN_MUX_USB_VW_OVER_CURRENT;

typedef struct {
  UINT32                               PinMuxNum;
  GPIOV2_PIN_MUX_USB_VW_OVER_CURRENT    *PinMux;
} GPIOV2_USB_VW_OVER_CURRENT;

typedef struct {
  GPIOV2_PIN_MUX PchHot;
} GPIOV2_PIN_MUX_PCH_HOT;

typedef struct {
  UINT32                   PinMuxNum;
  GPIOV2_PIN_MUX_PCH_HOT    *PinMux;
} GPIOV2_PCH_HOT;

typedef struct {
  GPIOV2_PIN_MUX Clk;
  GPIOV2_PIN_MUX Data;
} GPIOV2_PIN_MUX_DP_INTERFACE;

typedef struct {
  UINT32                        PinMuxNum;
  GPIOV2_PIN_MUX_DP_INTERFACE   *PinMux;
} GPIOV2_DP_INTERFACE;

typedef struct {
  GPIOV2_PIN_MUX I2sSclk;
  GPIOV2_PIN_MUX I2sSfrm;
  GPIOV2_PIN_MUX I2sTxd;
  GPIOV2_PIN_MUX I2sRxd;
} GPIOV2_PIN_MUX_HDA_SSP;

typedef struct {
  UINT32                   PinMuxNum;
  GPIOV2_PIN_MUX_HDA_SSP   *PinMux;
} GPIOV2_HDA_SSP;

typedef struct {
  GPIOV2_PIN_MUX SmbusAlert;
} GPIOV2_PIN_MUX_SMBUS_ALERT;

typedef struct {
  UINT32                      PinMuxNum;
  GPIOV2_PIN_MUX_SMBUS_ALERT  *PinMux;
} GPIOV2_SMBUS_ALERT;

typedef struct {
  GPIOV2_PIN_MUX SataGpPin;
} GPIOV2_PIN_MUX_SATA_GP_PIN;

typedef struct {
  UINT32                      PinMuxNum;
  GPIOV2_PIN_MUX_SATA_GP_PIN  *PinMux;
} GPIOV2_SATA_GP_PIN;

typedef struct {
  GPIOV2_PIN_MUX VrAlert;
} GPIOV2_PIN_MUX_VR_ALERT;

typedef struct {
  UINT32                          PinMuxNum;
  GPIOV2_PIN_MUX_VR_ALERT         *PinMux;
} GPIOV2_VR_ALERT;

typedef struct {
  GPIOV2_PIN_MUX TbtLsxA;
  GPIOV2_PIN_MUX TbtLsxB;
} GPIOV2_PIN_MUX_TBT_LSX;

typedef struct {
  UINT32                   PinMuxNum;
  GPIOV2_PIN_MUX_TBT_LSX   *PinMux;
} GPIOV2_TBT;

typedef struct {
  GPIOV2_PIN_MUX HdaSspMasterClock;
} GPIOV2_PIN_MUX_HDA_SSP_MASTER_CLOCK;

typedef struct {
  UINT32                                PinMuxNum;
  GPIOV2_PIN_MUX_HDA_SSP_MASTER_CLOCK   *PinMux;
} GPIOV2_HDA_SSP_MASTER_CLOCK;

typedef struct {
  GPIOV2_PIN_MUX SClock;
  GPIOV2_PIN_MUX SLoad;
  GPIOV2_PIN_MUX SDataOut;
} GPIOV2_PIN_MUX_SATA_SGPIO;

typedef struct {
  UINT32                      PinMuxNum;
  GPIOV2_PIN_MUX_SATA_SGPIO   *PinMux;
} GPIOV2_SATA_SGPIO;

typedef struct {
  GPIOV2_PIN_MUX CpuGpPin;
} GPIOV2_PIN_MUX_CPU_GP_PIN;

typedef struct {
  UINT32                      PinMuxNum;
  GPIOV2_PIN_MUX_CPU_GP_PIN   *PinMux;
} GPIOV2_CPU_GP_PIN;

typedef struct {
  GPIOV2_PIN_MUX TimeSync;
} GPIOV2_PIN_MUX_TIME_SYNC;

typedef struct {
  UINT32                      PinMuxNum;
  GPIOV2_PIN_MUX_TIME_SYNC    *PinMux;
} GPIOV2_TIME_SYNC;


typedef struct {
  GPIOV2_PIN_MUX Tsn;
} GPIOV2_PIN_MUX_TSN;

typedef struct {
  UINT32               PinMuxNum;
  GPIOV2_PIN_MUX_TSN   *PinMux;
} GPIOV2_TSN;

//
// Structure for USB Virtual Wire OverCurrent Pad Mode group
//
typedef struct {
  GPIOV2_PAD       OcRxPad;
  GPIOV2_PAD       OcTxPad;
} GPIOV2_VWOC_FUNCTION;

/**
VCCIO level selection
**/
typedef enum {
  SmbusPins,
  SmbusAlert
} GPIOV2_PIN_SEL;
#endif // _GPIOV2_PAD_MUXING_H_
