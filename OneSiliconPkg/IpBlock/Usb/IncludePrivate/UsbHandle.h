/** @file
  Header file for USB handle structure definition- USB IP Block API

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _USB_HANDLE_H_
#define _USB_HANDLE_H_

#include <ConfigBlock.h>
#include <Usb2PhyConfig.h>
#include <UsbConfig.h>
#include <UsbController.h>

typedef struct _USB_HANDLE USB_HANDLE;

// USB IP Version enumeration
typedef enum {
  UNKNOWN,
  V18_0,
  V18_1,
  V19_0,
  V19_1,
  V19_2,
  V19_3,
  V19_4
} USB_IP_VERSION;

// USB controller location
typedef enum {
  Tcss,       // Type-C SubSystem
  Standalone, // Standalone
  Pch         // PCH
} USB_INTEGRATION;

// Location stepping
typedef enum {
  A0,
  A1,
  B0,
  B1,
  C0,
  C1,
  D0,
  D1
} INTEGRATION_STEPPING;

// CPX_DEEMPH register structure definition
typedef union {
  struct {
    UINT32  CminusOnePrecursor    :  6;  // Bits 0-5
    UINT32  Czero                 :  6;  // Bits 6-11
    UINT32  CplusOnePrecursor     :  6;  // Bits 12-17
    UINT32  ReservedBits          : 14;  // Reserved bits
  } Field;
  UINT32  Value;
} XHCI_CPX_DEEMPH;

//
// USB2 PHY reference frequencies values (MHz)
//
typedef enum {
  USB_FREQ_19_2 = 0u,
  USB_FREQ_24_0,
  USB_FREQ_96_0,
  USB_FREQ_MAX
} USB2_PHY_REF_FREQ;

/**
  Internal USB controller description
**/
typedef struct {
  BOOLEAN IsPortResetMessagingSupported;  // Holds true if controller supports port reset messaging
  BOOLEAN IsDebugEnabled;                 // CPU Debug information
  BOOLEAN ExternalPowerControl;           // Allow outside agent to configure power and clock gating
  BOOLEAN IsDbcDebugEnabled;              // Holds information whether DbC debug is enabled
  /**
    LTV Limit value that will be written to xHCI MMIO 0x8178[12:0] unless it's 0 then it'll be ignored.
    This setting for some PCH's will depend on CPU pairing.
  **/
  UINT16 LtvLimit;
  /**
    xHC Latency Tolerance Parameters used during initialization process
  **/
  UINT32 LtrHigh;
  UINT32 LtrMid;
  UINT32 LtrLow;
  USB_IP_VERSION       IpVersion;      // USB IP Version
  USB_INTEGRATION      Location;       // Location of USB controller
  INTEGRATION_STEPPING Stepping;       // Stepping of the owner
  BOOLEAN              CpxProgramming; // Flag whether to programm CPX_DEEMPH registers
  //
  //  Fields with values that will be programmed to CPX registers if CpxProgramming is enabled
  //
  XHCI_CPX_DEEMPH Cp13Deemph;      // Value to be set in CP13_DEEMPH register
  XHCI_CPX_DEEMPH Cp14Deemph;      // Value to be set in CP14_DEEMPH register
  XHCI_CPX_DEEMPH Cp15Deemph;      // Value to be set in CP15_DEEMPH register
  XHCI_CPX_DEEMPH Cp16Deemph;      // Value to be set in CP16_DEEMPH register

  BOOLEAN RxStandbySupport; // RxStandby capability flag
  BOOLEAN LaneDeassertInPs3Support; // Lane Deassert in PS3 capability flag

  /**
    This is internal switch which allow skip updating Chipset Init table from IP block flow
    EBG is not supporting updating SUS tables.
  **/
  BOOLEAN                    SkipWriteToChipsetInitTable;
  /**
    This is an internal switch to allow for enabling/disabling programming around
    xHCI USB2 Debug Mode Back to Back WR support
  **/
  BOOLEAN                    DisableBackToBackWRSupport;
  USB2_PHY_REF_FREQ          Usb2PhyRefFreq;  // USB2 PHY Reference Clock frequency
  UINT8                      DebugPortNumer;  // Numer of USB2 port number used for debug

  BOOLEAN SkipD3HE; // If set code will not enable D3 Hot Enable in PCE
} USB_PRIVATE_CONFIG;

/**
  Configures HHPIO pins for USB OverCurrent detection

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
  @param[in]  OvercurrentPin    Index of OverCurrent Pin to be enabled
**/
typedef
VOID
(*USB_ENABLE_OVERCURRENT_PIN) (
  IN    USB_HANDLE        *UsbHandle,
  IN    UINT8             OverCurrentPin
  );

/**
  Disables USB Controller

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
**/
typedef
VOID
(*USB_CONTROLLER_DISABLE) (
  IN    USB_HANDLE        *UsbHandle
  );

/**
  Checks in FIA is given lane is USB owned

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
  @param[in]  LaneIndex         Index of lane to check
**/
typedef
BOOLEAN
(*USB_LANE_OWNED) (
  IN    USB_HANDLE        *UsbHandle,
  IN    UINT8             LaneNumber
  );

/**
  Schedules host system reset

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
  @param[in]  ResetType         UEFI defined reset type.
**/
typedef
VOID
(*USB_SCHEDULE_HOST_RESET) (
  IN    USB_HANDLE        *UsbHandle,
  IN    EFI_RESET_TYPE    ResetType
  );

/**
  Checks if DCI debug is enabled on platform

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance

  @retval     TRUE              Dci debug is enabled on platform
              FALSE             Dci debug is not enabled on platform
**/
typedef
BOOLEAN
(*USB_CHECK_DCI_DEBUG_ENABLED) (
  IN    USB_HANDLE        *UsbHandle
  );

/**
  Delays execution for desired amount of us (microseconds)

  @param[in]  UsbHandle         Pointer to USB_HANDLE instance
  @param[in]  MicroSeconds      The minimum number of microseconds to delay
**/
typedef
VOID
(*USB_DELAY_FOR_US) (
  IN    USB_HANDLE        *UsbHandle,
  IN    UINT32            MicroSeconds
  );

/**
  This function Calculates OverCurrentPin number match XHCI USB3 OCM
  register bit map
  @param[in]  OverCurrentPin     USB OC pin number

  @retval     USB OCM register index
**/
typedef
UINT32
(*USB_CALCULATE_OC_PIN) (
  IN    UINT32            OverCurrentPin
  );

/**
  USB callbacks
  List of upper layer function pointers (callbacks)
  that can be passed to IP initialization library.
  Used for cross IP communication
  and SoC related operations.
**/
typedef struct {
  USB_ENABLE_OVERCURRENT_PIN  UsbEnableOvercurrentPin;
  USB_CONTROLLER_DISABLE      UsbHostControllerDisable;
  USB_CONTROLLER_DISABLE      UsbDeviceControllerDisable;
  USB_LANE_OWNED              UsbIsLaneOwned;
  USB_SCHEDULE_HOST_RESET     ScheduleHostReset;
  USB_CHECK_DCI_DEBUG_ENABLED CheckDciDebugEnabled;
  USB_DELAY_FOR_US            DelayForUs;
  USB_CALCULATE_OC_PIN        CalculateOcPin;
} USB_CALLBACK;

/**
  USB controller handle. Stores all data needed for USB IP initialization
**/
struct _USB_HANDLE {
  /**
    USB Controllers access definition
  **/
  USB_CONTROLLER        *DeviceController;

  /**
    USB IP Public/User configuration
  **/
  USB_CONFIG            *UsbConfig;

  /**
    USB2 phy configuration
  **/
  USB2_PHY_CONFIG       *Usb2PhyConfig;

  /**
    USB IP Private configuration
  **/
  USB_PRIVATE_CONFIG    *PrivateConfig;

  /**
    Upper layer (SoC/FRU) callback function pointers
  **/
  USB_CALLBACK          *Callback;
};

#endif// _USB_HANDLE_H_
