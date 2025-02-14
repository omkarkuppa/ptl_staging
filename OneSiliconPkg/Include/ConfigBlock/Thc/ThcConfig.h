/** @file
  Touch Host Controller policy.

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

#ifndef _THC_CONFIG_H_
#define _THC_CONFIG_H_

#include <ConfigBlock.h>
#include <ConfigBlock/SiConfig.h>

#define THC_CONFIG_REVISION 7

extern EFI_GUID gThcConfigGuid;
extern EFI_GUID gThcSsidHobGuid;
#pragma pack (push,1)

/**
  Available Port Assignments

**/
typedef enum {
  ThcAssignmentNone, ///< None of the avaialbe controllers assigned
  ThcAssignmentThc0, ///< Port assigned to THC0
  ThcAssignmentThc1  ///< Port assigned to THC1
} THC_PORT_ASSIGNMENT;

/**
  Available modes
**/
typedef enum {
  Thc,        ///< Intel THC protocol
  HidOverSpi, ///< Hid Over SPI protocol
  HidOverI2c,  ///< Hid Over I2C protocol
  ThcModeNone
} THC_MODE;

/**
  Available Port Clock configurations

**/
typedef enum {
  ThcClockFunctional, // SPI_DFX_CLK_EN BIT set to 0
  ThcClockUseDfx      // SPI_DFX_CLK_EN BIT set to 1
} THC_PORT_CLOCK;

/**
  THC Hid Over SPI mode related settings. Only applicable when HidOverSpi is enbled.
**/
typedef struct {
  UINT32 Frequency;                 ///<  Connection Speed - SPI Frequency in Hz
  UINT32 InputReportHeaderAddress;  ///<  Input Report Header Address
  UINT32 InputReportBodyAddress;    ///<  Input Report Body Address
  UINT32 OutputReportAddress;       ///<  Output Report Address
  UINT32 ReadOpcode;                ///<  Read Opcode
  UINT32 WriteOpcode;               ///<  Write Opcode
  UINT32 Flags;                     ///<  Flags
  UINT32 LimitPacketSize;           ///<  Limit Packet Size
} THC_HID_OVER_SPI;

/**
  THC Reset Pad related settings.
**/
typedef struct {
  UINT32 ResetPad;                  ///<  Reset Pad - <b>0x0 = THC HW default</b> BIOS will pass Pad infromation to ACPI, this Pad is used in _RST function
  UINT32 ResetPadTrigger;           ///<  Reset Pad Trigger
  UINT32 ResetSequencingDelay;      ///<  Policy control for reset sequencing delay (_INI, _RST)
} THC_RESET;

/**
  THC Hid Over I2C mode related settings. Only applicable when HidOverI2c is enbled.
**/
typedef struct {
  UINT32 DeviceAddress;                                      ///<  Device Address
  UINT32 ConnectionSpeed;                                    ///<  Connection Speed - Frequency [Hz]
  /**
    Addressing Mode
    0x1: The connection uses 10-bit addressing.
    0x0: The connection uses 7-bit addressing.
  **/
  UINT32 AddressingMode;
  UINT32 DeviceDescriptorAddress;                            ///<  Device Descriptor Address
  UINT32 StandardModeSerialClockLineHighPeriod;              ///<  Serial Clock Line High Period
  UINT32 StandardModeSerialClockLineLowPeriod;               ///<  Standard Mode Serial Clock Line Low Period
  UINT32 StandardModeSerialDataLineTransmitHoldPeriod;       ///<  Standard Mode Serial Data Line Transmit Hold Period
  UINT32 StandardModeSerialDataLineReceiveHoldPeriod;        ///<  Standard Mode Serial Data Line Receive Hold Period
  UINT32 FastModeSerialClockLineHighPeriod;                  ///<  Fast Mode Serial Clock Line High Period
  UINT32 FastModeSerialClockLineLowPeriod;                   ///<  Fast Mode Serial Clock Line Low Period
  UINT32 FastModeSerialDataLineTransmitHoldPeriod;           ///<  Fast Mode Serial Data Line Transmit Hold Period
  UINT32 FastModeSerialDataLineReceiveHoldPeriod;            ///<  Fast Mode Serial Data Line Receive Hold Period
  UINT32 MaxSuppressedSpikesSMFMFMP;                         ///<  Maximum Length Of Suppressed Spikes In Std Mode Fast Mode And Fast Mode Plus
  UINT32 FastModePlusSerialClockLineHighPeriod;              ///<  Fast Mode Plus Serial Clock Line High Period
  UINT32 FastModePlusSerialClockLineLowPeriod;               ///<  Fast Mode Plus Serial Clock Line Low Period
  UINT32 FastModePlusSerialDataLineTransmitHoldPeriod;       ///<  Fast Mode Plus Serial Data Line Transmit Hold Period
  UINT32 FastModePlusSerialDataLineReceiveHoldPeriod;        ///<  Fast Mode Plus Serial Data Line Receive Hold Period
  UINT32 HighSpeedModePlusSerialClockLineHighPeriod;         ///<  High Speed Mode Plus Serial Clock Line High Period
  UINT32 HighSpeedModePlusSerialClockLineLowPeriod;          ///<  High Speed Mode Plus Serial Clock Line Low Period
  UINT32 HighSpeedModePlusSerialDataLineTransmitHoldPeriod;  ///<  High Speed Mode Plus Serial Data Line Transmit Hold Period
  UINT32 HighSpeedModePlusSerialDataLineReceiveHoldPeriod;   ///<  High Speed Mode Plus Serial Data Line Receive Hold Period
  UINT32 MaximumLengthOfSuppressedSpikesInHighSpeedMode;     ///<  Maximum Length Of Suppressed Spikes In High Speed Mode
} THC_HID_OVER_I2C;

/**
  Structure to hold Sub System ID for port
**/
typedef struct {
  UINT16 CustomizedSsid;          ///< Sets Customized SubSytem ID for Port
  UINT16 CustomizedSvid;          ///< Sets Customized SubSytem Vendor ID for Port
} THC_PORT_SUBSYTEM_ID;

/**
  Structure to hold Wake On Touch pin configuration for GPIO resource in ACPI
**/
typedef struct {
  UINT8   EdgeLevel;    // Can be either Edge (0x1) or Level (0x0)
  UINT8   ActiveLevel;  // Can be Active High (0x0), Active Low (0x1).
                        // Active Both (0x2) is valid only for EdgeLevel set to Edge.
  UINT8   PinConfig;    // Pin config should be one of: PullDefault (0x0), PullUp (0x1), PullDown (0x2) or PullNone (0x3)
  UINT8   Rsvd1;
} THC_WOT_PIN_CONFIG;

/**
  Port Configuration structure required for each Port that THC might use.
**/
typedef struct {
  UINT32                Assignment;                     ///< Sets THCx assignment see THC_PORT_ASSIGNMENT
  UINT32                InterruptPinMuxing;             ///< Each GPIO PORTx/SPIx INTB Pin has different muxing options refer to GPIO_*_MUXING_THC_SPIx_*
  UINT8                 WakeOnTouch;                    ///< 1 = Enable <b>0 = Disable</b>. Based on this setting vGPIO for given THC will be in native mode, and additional _CRS for wake will be exposed in ACPI
  UINT8                 ThcDsyncPadEnable;              ///< 1 = Enable <b>0 = Disable</b>. Based on this settings enables display sync GPIO for given THC
  UINT8                 Mode;                           ///< 2 = Hid over I2C <b>1 = Hid over SPI <b>0 = THC</b>. Switch between Intel THC protcol and Industry standard HID Over SPI protocol.
  /**
    Timestamp timer behavior in D0i2
      <b>1 = Timer resets to 0 when entering D0i2</b>
      0 = Timer is paused instead of reset to 0 when entering D0i2
   **/
  UINT8                 TimestampTimerMode;
  THC_WOT_PIN_CONFIG    WotPinConfig;                   ///< Wake On Touch pin configuration
  THC_HID_OVER_SPI      HidOverSpi;                     ///< Hid Over Spi mode settings
  UINT32                ActiveLtr;                      ///< <b>0xFFFFFFFF = Driver Default </b> Active Ltr
  UINT32                IdleLtr;                        ///< <b>0xFFFFFFFF = Driver Default </b> Idle Ltr
  UINT32                PerformanceLimitation;          ///< Performance Limitation
  /**
    Period of the emulated display frame sync [ms]
    The minimum period is 2ms, maximum period is 100ms
    Default: <b>10</b>ms
  **/
  UINT32                DisplayFrameSyncPeriod;
  THC_RESET             Reset;                          ///< Reset Line settings
  THC_HID_OVER_I2C      HidOverI2c;                     ///< Hid Over I2c mode settings
  UINT32                Clock;                          ///< For available options see THC_PORT_CLOCK
  THC_PORT_SUBSYTEM_ID  ThcSubSystemId;              ///< Customized Subsystem ID
  UINT32                Reserved;
} THC_PORT;

/**
  THC_CONFIG block provides the configurations forTouch Host Controllers

  Assignment field in each THC port controlls the THC behavior.

  Available scenarios:
  1: Single Port 0 used by THC0
      - THC0 Enabled
      - Port0 assigned to THC0
      - Port1 unassigned
      - THC1 will be automatically Disabled.
  2: Both ports used by THC0
      - THC0 Enabled
      - Port0 assigned to THC0
      - Port1 assigned to THC0
      - THC1 will be automatically Disabled.
  3: Port 0 used by THC0 and Port 1 used by THC1
      - THC0 Enabled
      - Port0 assigned to THC0
      - THC1 Enabled
      - Port1 assigned to THC1.
<b>4: Both Ports unassigned.</b>
      Both THC Controllers will be disabled in that case.

  @note
  Invalid scenario that will cause ASSERT.
  1. Same port Number assigned to THC0 or THC1.
  2. Two Ports assigned to THC1.

  <b>Revision 1:</b>
  - Inital version.
  <b>Revision 2:</b>
  - Add HidOverSpi THC_MODE
  <b>Revision 3:</b>
  - Expand HidOverSpi support
  <b>Revision 4:</b>
  - Add ResetSequencingDelay in THC_HID_OVER_SPI
  <b>Revision 5:</b>
  - Moved out reset data from THC_HID_OVER_SPI
  - Add THC_HID_OVER_I2C
  <b>Revision 6:</b>
  - Added THC_WOT_PIN_CONFIG to enable configuration of ACPI resource of Wake Pin
  <b>Revision 7:</b>
  - Added THC_PORT_SUBSYTEM_ID to program customized SubSytem ID
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           ///< Config Block Header
  THC_PORT             ThcPort[2];       ///< Port Configuration
} THC_CONFIG;

///
/// THC SSID Data Hob
///
typedef struct {
  EFI_HOB_GUID_TYPE        EfiHobGuidType;                    ///< GUID
  SVID_SID_SKIP_TABLE      *ThcSsidTable;                     ///< THC SSID table
  UINT16                   NumberOfThcSsidTable;              ///< number of THC SSID table
  UINT8                    Reserved[2];                       ///< Reserved for Hob size allignment
} THC_DATA_SSID_CONFIG_HOB;

#pragma pack (pop)

#endif // _THC_CONFIG_H_
