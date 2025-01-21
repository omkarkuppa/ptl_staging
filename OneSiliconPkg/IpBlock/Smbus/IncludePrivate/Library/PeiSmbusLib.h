/** @file
  PCH Smbus Library

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
#ifndef _PEI_SMBUS_LIB_H_
#define _PEI_SMBUS_LIB_H_

#include <SmbusConfig.h>

typedef struct _SMBUS_HANDLE SMBUS_HANDLE;

/**
  Called as a last step in SMBUS controller disable flow.

  @param[in] SmbusHandle  Handle
**/
typedef
VOID
(*SMBUS_DISABLE) (
  IN  SMBUS_HANDLE           *SmbusHandle
  );

/**
Configures GPIO pins for SMBUS Alert

@param[in]  SMBUS_HANDLE     Pointer to SMBUS Handle structure

@retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SMB_ALERT_ENABLE) (
  IN  SMBUS_HANDLE           *SmbusHandle
  );

/**
Configures GPIO pins for SMBUS Interface

@param[in]  SMBUS_HANDLE     Pointer to SMBUS Handle structure

@retval EFI_STATUS      Status returned by worker function
**/
typedef
EFI_STATUS
(*SMB_GPIO_ENABLE) (
  IN  SMBUS_HANDLE           *SmbusHandle
  );

typedef struct {
  SMBUS_DISABLE             SmbusDisable;
  SMB_GPIO_ENABLE           SmbusGpioEnable;
  SMB_ALERT_ENABLE          SmbusAlertEnable;
} SMBUS_CALLBACKS;

typedef struct {
  UINT8    IntPin;
  UINT8    Irq;
  UINT8    TcoIrq;
  /**
  This option says if SMBUS controller supports
  any Power Management options
  <b>FALSE: NOT Supported</b>, TRUE: Supported
  **/
  BOOLEAN  PowerManagementSupport;
  /**
  This option says if TCO timer build-in SMBUS controller
  supports SMI generation when TCO timer reach 0
  <b>FALSE: NOT Supported</b>, TRUE: Supported
  **/
  BOOLEAN   TcoSmiTimeoutSupport;
  /**
  This option says if the controller supports
  reception of a Host Notify command as a wake event.
  When supported this event is ORed in with the other SMBus wake events
  and is reflected in the SMB_WAK_STS bit of the General Purpose Event 0 Status register.
  <b>FALSE: NOT Supported</b>, TRUE: Supported
  **/
  BOOLEAN   HostNotifyWakeSupport;
  /**
  This option stores Host Timing Register value
  that shall be programmed.
  Setting these timing parameters will affect the SMBus host phy layer.
  Note:
  Zero value means HTIM register is not present,
  don't program the register.
  <b>Host Timing Register Value</b>
  **/
  UINT32    HostTiming;
  BOOLEAN   InstallPpi;
  UINT16    TcoBase;
} SMBUS_SOC_CONFIG;

//
//  SMBUS Controller Structure
//
typedef struct {
  UINT64                      PciCfgBase;       // PCI configuration base
  UINT8                       Segment;          // PCI Segment
  UINT8                       Bus;              // PCI Bus
  UINT8                       Device;           // PCI Device
  UINT8                       Function;         // PCI Fuction
  UINT32                      SmbusCtrlIndex;   // Smbus Controller Index
  UINT32                      TotalCtrlPortNum; // Total Smbus Ports
  UINT16                      AcpiBase;         // AcpiBase
} SMBUS_CONTROLLER;

struct _SMBUS_HANDLE {
  SMBUS_CONTROLLER         *Controller;         // Describes SMBUS controller location
  SMBUS_SOC_CONFIG         *SocConfig;          // Describes SoC specific config
  PCH_SMBUS_PREMEM_CONFIG  *Config;             // Describes platform config
  SMBUS_CALLBACKS          *Callbacks;          // Set of SoC callbacks
  REGISTER_ACCESS          *SmbusSbAccessMmio;  // P2SB Message type, Smbus controller Private Config Register (PCR) space access
};

/**
  Initialize the Smbus PPI and program the Smbus BAR

  @param[in]  SMBUS_HANDLE        SMBUS_HANDLE instance

  @retval EFI_SUCCESS             The function completes successfully
  @retval EFI_OUT_OF_RESOURCES    Insufficient resources to create database
**/
EFI_STATUS
SmbusPreMemInit (
  IN  SMBUS_HANDLE       *SmbusHandle
  );

/**
  The function performs SMBUS specific programming.

  @param[in] SMBUS_HANDLE       SMBUS_HANDLE instance

**/
VOID
SmbusPostMemConfigure (
  IN  SMBUS_HANDLE           *SmbusHandle
  );
#endif //_PEI_SMBUS_LIB_H_
