/** @file
  Thermal Handle structure

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
#ifndef _THERMAL_HANDLE_H_
#define _THERMAL_HANDLE_H_

#include <ThermalConfig.h>
#include <Ppi/SiPolicy.h>

typedef struct _THERMAL_HANDLE THERMAL_HANDLE;

/**
  Thermal Enable PCH HOT.
**/
typedef
VOID
(*THERMAL_ENABLE_PCH_HOT) (
  VOID
  );

/**
  Thermal callbacks
  List of function pointers can passed to IB Block driver
**/
typedef struct {
  THERMAL_ENABLE_PCH_HOT  EnablePchHot;
} THERMAL_CALLBACKS;

/**
  Thermal Private Config data structure
**/
typedef struct {
  BOOLEAN  PchHotSupported;
  BOOLEAN  EcThermalReportSupported;
  BOOLEAN  SocIntFabThermTtSupported;
} THERMAL_PRIVATE_CONFIG;

/**
  Thermal handle structure
  Stores all data necessary to initialize DCI
**/
struct _THERMAL_HANDLE {
  UINTN                     PwrmBase;
  THERMAL_CONFIG            *Config;           // Describes SoC specific config
  THERMAL_PRIVATE_CONFIG    *PrivateConfig;    // Thermal private config variables
  THERMAL_CALLBACKS         *Callbacks;        // Set of SoC callbacks
};

#endif // _THERMAL_HANDLE_H_
