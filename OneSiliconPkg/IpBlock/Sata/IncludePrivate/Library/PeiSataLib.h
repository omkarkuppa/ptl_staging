/** @file
  SATA routines

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

@par Specification
**/
#ifndef _PEI_SATA_LIB_H_
#define _PEI_SATA_LIB_H_

#include <SataCtrl.h>
#include <SataConfig.h>
#include <GpioV2Pad.h>
#include <Library/PchInfoLib.h>

typedef enum {
  Scale1ns,
  Scale32ns,
  Scale1024ns,
  Scale32768ns,
  Scale1048576ns,
  Scale33554432ns
} SATA_SNOOP_LAT_SCALE;

typedef SATA_SNOOP_LAT_SCALE SATA_LTR_LAT_SCALE;

typedef enum {
  SataSosc125Mhz = 0,
  SataSosc120Mhz,
  SataSosc100Mhz,
  SataSosc25Mhz,
  SataSosc19p2Mhz,
  SataSoscUnsupported
} SATA_SOSC_CLK_FREQ;

typedef enum {
  RstUnsupported  = 0,
  RstPremium,
  RstOptane,
  RstMaxMode
} RST_MODE;

/**
  Internal SATA policy options
**/
typedef struct {
  /**
    Description: Indicate if remapping is supported
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               RemappingSupported;
  /**
    Description: RST mode supported
    Possible values: RST_MODE options
  **/
  RST_MODE              SataSupportedRstMode;
  /**
    Description: Indicate if storage remapping is enabled on SATA controller
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               StorageRemappingEnabled;
  /**
    Description: Disables SATA SRAM parity check
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataSramParityCheckDisable;
  /**
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SirC4Programming;
  /**
    Description: Indicate if SATA controller supports power gating features
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataPowerGatingSupported;
  /**
    Description: Skips SATA port clock disable during SATA controller disable flow.
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataSkipPortClockDisable;
  /**
    Description: Indicates whether PsOn feature is enabled on the system.
    PsOn is a feature that will cut the primary ATX rail
    and transition all SATA ports into D3Cold.
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               SataPsOnSupported;
  /**
    Description: SATA controller oscylator frequency
    Possible values: SATA_SOSC_CLK_FREQ options
  **/
  SATA_SOSC_CLK_FREQ    SataOscClkFreq;
  /**
    Description: Indicates SATA controller interrupt pin is supported
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               InterruptPinSupported;
  /**
    Description: SATA controller interrupt pin
    Possible values: 0 - 255
  **/
  UINT8                 InterruptPin;
  /**
    Description: Sets Command Parity Error and Data Phase Parity Error handling.
    Possible values: TRUE or FALSE
  **/
  BOOLEAN               EnableParityCheck;
  /**
    Description: Default value of Snoop Latency Value
    Possible values: 0 - 511
  **/
  UINT16                SataSnoopLatVal;
  /**
    Description: Default value of Snoop Latency Scale factor
    Possible values: SATA_SNOOP_LAT_SCALE options
  **/
  UINT16                SataSnoopLatScale;
  /**
    Description: Default value of LTR Slumber Latency Scale factor.
    Possible values: 0 - 1023
  **/
  UINT16                LtrSlumberScale;
  /**
    Description: Default value of LTR Slumber Latency Value.
    Possible values: SATA_LTR_LAT_SCALE options
  **/
  UINT16                LtrSlumberValue;
  /**
    Description: Default value of LTR Partial Latency Scale factor.
    Possible values: 0 - 1023
  **/
  UINT16                LtrPartialScale;
  /**
    Description: Default value of LTR Partial Latency Value.
    Possible values: SATA_LTR_LAT_SCALE options
  **/
  UINT16                LtrPartialValue;
} SATA_PRIVATE_CONFIG;

/**
  Checks if SATA controller has lanes connected

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) to be examined

  @retval TRUE if SATA controller has lanes assigned, otherwise FALSE
**/
typedef
BOOLEAN
(*SATA_PHY_CONNECTED) (
  IN  SATA_CONTROLLER   *SataController,
  IN  UINT8             Port
  );

/**
  Configures GPIO pins for SATA DevSlp

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Dev Slp enable
  @param[in]  ResetType         GPIO reset type (see GPIO_RESET_CONFIG)
  @param[in]  DevSlpGpioPinMux  Pin Mux GPIO pad value.

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_DEV_SLP_ENABLE) (
  IN  SATA_CONTROLLER    *SataController,
  IN  UINT8              Port,
  IN  UINT32             ResetType,
  IN  GPIOV2_PAD         DevSlpGpioPinMux
  );

/**
  Configures GPIO pins for SATA Hot Plug

  @param[in]  SataController    Pointer to SATA Controller structure
  @param[in]  Port              Port number (0 based) for Hot Plug enable

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_HOTPLUG_ENABLE) (
  IN  SATA_CONTROLLER    *SataController,
  IN  UINT8              Port
  );

/**
  Configures GPIO pins for SATA Serial GPIO

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_SGPIO_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Configures GPIO pins for SATA LED

  @param[in]  SataController     Pointer to SATA Controller structure

  @retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SATA_LED_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Disables SATA controller in power controller and fabric

  @param[in]  SataController     Pointer to SATA Controller structure

**/
typedef
VOID
(*SATA_CTRL_DISABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  Enables SATA test mode in PHY

  @param[in]  SataController     Pointer to SATA Controller structure

**/
typedef
VOID
(*SATA_TEST_MODE_ENABLE) (
  IN  SATA_CONTROLLER    *SataController
  );

/**
  SATA callbacks
  List of function pointers can passed to IB Block driver
**/
typedef struct {
  SATA_PHY_CONNECTED          SataIsPhyConnected;
  SATA_TEST_MODE_ENABLE       SataTestModeEnable;
  SATA_DEV_SLP_ENABLE         SataDevSleepEnable;
  SATA_HOTPLUG_ENABLE         SataHotPlugEnable;
  SATA_SGPIO_ENABLE           SataSgpioEnable;
  SATA_LED_ENABLE             SataLedEnable;
  SATA_CTRL_DISABLE           SataControllerDisable;
} SATA_CALLBACK;

  /**
    SATA controller structure
    Stores all data necessary to initialize SATA IP block
  **/
typedef struct {
  SATA_CONTROLLER             *Controller;
  SATA_CONFIG                 *SataConfig;
  SATA_PRIVATE_CONFIG         *PrivateConfig;
  SATA_CALLBACK               *Callback;
  /**
    Temporary Bar
  **/
  UINT64                      Mmio;
} SATA_HANDLE;

/**
  Initializes SATA device

  @param[in]  Sata          Pointer to SATA controller structure

**/
VOID
ConfigureSata (
  IN  SATA_HANDLE   *Sata
  );

/**
  Disable SATA Controller if needed after RST Remapping code

  @param[in]  Sata          Pointer to SATA controller structure

**/
VOID
ConfigureSataAfterRst (
  IN  SATA_HANDLE   *Sata
  );
#endif // _PEI_SATA_LIB_H_
