/** @file
  Header file for CnviHandle.

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
#ifndef _CNVI_HANDLE_H_
#define _CNVI_HANDLE_H_

#include <Uefi/UefiBaseType.h>
#include <CnviConfig.h>
#include <Library/P2SbSidebandAccessLib.h>

typedef struct _CNVI_HANDLE CNVI_HANDLE;

/**
  CNVi controller structure
  Stores information required to access to controller registers
**/
typedef struct {
  UINT64    WifiPciCfgBaseAddr; ///< CNVi WiFi controller PCI config space address
  UINT64    BtPciCfgBaseAddr;   ///< CNVi Bluetooth controller PCI config space address
} CNVI_CONTROLLER;

/**
  CNVi Function State
**/
typedef enum {
  FunctionEnable,
  FunctionDisable
} CNVI_STATE;

/**
  Set CNVi Function Enable/Disable

  @param[in] State               Target CNVi Function State
**/
typedef
VOID
(*CNVI_SET_FUNCTION) (
  IN CNVI_STATE                  State
  );

/**
  CNVi Pin Function
**/
typedef enum {
  ConfigCrfPin,
  ConfigBtI2sPin,
  ConfigBtDisPin
} CNVI_PIN_FUNCTION;

/**
  Configure CNVi GPIO Pin Function

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
  @param[in] PinFunction         Target Pin Function
**/
typedef
VOID
(*CNVI_CONFIG_PIN_FUNCTION) (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_PIN_FUNCTION           PinFunction
  );

/**
  This function sets CNVi WiFi/Bluetooth Core Enable/Disable State

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
  @param[in] State               Target CNVi Function State
**/
typedef
VOID
(*CNVI_CORE_STATE) (
  IN CNVI_HANDLE                 *CnviHandle,
  IN CNVI_STATE                  State
  );

/**
  This function sets CNVi Bluetooth Interface

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
typedef
VOID
(*CNVI_BT_SET_INTERFACE) (
  IN CNVI_HANDLE                 *CnviHandle
  );

/**
  This function performs CNVi Bluetooth prerequisite setting before initialization

  @param[in] CnviHandle          Pointer to CNVi Handle Structure
**/
typedef
VOID
(*CNVI_BT_PRE_INIT) (
  IN CNVI_HANDLE                 *CnviHandle
  );

#pragma pack (push,1)

/**
  Private CNVi Config
**/
typedef struct {
  BOOLEAN               IsFunctionDisabled;
  BOOLEAN               IsCrfModulePresent;
  UINT8                 WifiInterruptPin;
  UINT8                 WifiIrq;
  UINT8                 BtInterruptPin;
  UINT8                 BtIrq;
} CNVI_PRIVATE_CONFIG;

/**
  CNVi callbacks
  List of function pointers can be passed to the IP Block driver
**/
typedef struct {
  CNVI_SET_FUNCTION                        SetFunction;
  CNVI_CONFIG_PIN_FUNCTION                 ConfigPinFunction;
  //
  // Wi-Fi
  //
  CNVI_CORE_STATE                          WifiSetCore;
  //
  // Bluetooth
  //
  CNVI_CORE_STATE                          BtSetCore;
  CNVI_BT_SET_INTERFACE                    BtSetInterface;
  CNVI_BT_PRE_INIT                         BtPreInit;
} CNVI_CALLBACKS;

/**
  CNVi handle structure
  Stores all data necessary to initialize CNVi IP block
**/
struct _CNVI_HANDLE {
  CNVI_CONFIG                *Config;
  CNVI_PRIVATE_CONFIG        *PrivateConfig;
  CNVI_CONTROLLER            *Controller;
  CNVI_CALLBACKS             *Callbacks;
  REGISTER_ACCESS            *SbAccess;
};
#pragma pack (pop)

#endif // _CNVI_HANDLE_H_
