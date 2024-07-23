/** @file
  Header file for IEH (Integrated Error Handler) handle structure definition.

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

#ifndef _IEH_HANDLE_H_
#define _IEH_HANDLE_H_

#include <IehConfig.h>
#include <SwizzleConfig.h>

typedef struct _IEH_HANDLE  IEH_HANDLE;
typedef struct _EAH_HANDLE  EAH_HANDLE;

/**
  Internal IEH description
**/
typedef struct {
  UINT8    BusNum;      // Bus number of IEH
} IEH_PRIVATE_CONFIG;

/**
  Internal EAH description
**/
typedef struct {
  UINT8    BusNum;      // Bus number of EAH
} EAH_PRIVATE_CONFIG;

/**
  IEH controller handle
**/
struct _IEH_HANDLE {
  UINT64                PciCfgBase;
  IEH_CONFIG            *Config;
  IEH_PRIVATE_CONFIG    *PrivateConfig;
};



/**
  Structure describes severity programming for EAH.
  This will be used to keep severity programming aligned
  between EAH and IEH being on different Si parts.
**/
typedef struct {
  BOOLEAN    UrSeverityAsFatal;            // Report UR issue as fatal error
  BOOLEAN    UcSeverityAsFatal;            // Report UC issue as fatal error
  BOOLEAN    CommandParitySeverityAsFatal; // Report Command Parity issue as fatal error
} FABRIC_SEVERITY;

/**
  EAH controller handle
**/
struct _EAH_HANDLE {
  UINT64                PciCfgBase;
  IEH_CONFIG            *Config;
  EAH_PRIVATE_CONFIG    *PrivateConfig;
  SWIZZLE_CONFIG        *SwizzleConfig;
  FABRIC_SEVERITY       FabricSeverity;
};

#endif // _IEH_HANDLE_H_
