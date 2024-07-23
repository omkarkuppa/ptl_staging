/** @file
  Time Sensitive Networking Handle structure

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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
#ifndef _TSN_HANDLE_H_
#define _TSN_HANDLE_H_

#include <Uefi/UefiBaseType.h>
#include <TsnConfig.h>
#include <Ppi/SiPolicy.h>
#include <RegisterAccess.h>

typedef struct _TSN_HANDLE TSN_HANDLE;

/**
  Configure TSN GPIOs

**/
typedef
VOID
(*TSN_ENABLE_GPIO) (
  VOID
  );

/**
  Tsn callbacks
  List of function pointers can passed to IB Block driver
**/
typedef struct {
  TSN_ENABLE_GPIO   TsnEnableGpio;
} TSN_CALLBACKS;

/**
  Tsn Private Config data structure
**/
typedef struct {
  UINT8          InterruptPin;
  UINT8          Irq;
  BOOLEAN        IsFiaLanePchTsnOwned;
  UINT8          PchTsnLane;
  UINT8          ModPhyPortId;
} TSN_PRIVATE_CONFIG;

/**
  Tsn controller structure
  Stores information required to access to controller registers
  like PCI config space address, BARs and S:B:D:F
**/
typedef struct {
  UINT64    PciCfgBaseAddr;
  UINT8     Segment;
  UINT8     Bus;
  UINT8     Device;
  UINT8     Function;
} TSN_CONTROLLER;

/**
  Tsn handle structure
  Stores all data necessary to initialize Tsn
**/
struct _TSN_HANDLE {
  TSN_CONTROLLER        Controller;       // Tsn controller structure
  TSN_CONFIG            *Config;          // Describes SoC specific config
  TSN_PRIVATE_CONFIG    PrivateConfig;    // Tsn private config variables
  TSN_CALLBACKS         Callbacks;        // Set of SoC callbacks
  REGISTER_ACCESS*      TsnPcrAccess;     // Pcr register access
};

#endif // _TSN_HANDLE_H_s
