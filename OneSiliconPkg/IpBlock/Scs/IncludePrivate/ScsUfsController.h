/** @file
  SCS UFS controller definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _SCS_UFS_CONTROLLER_H_
#define _SCS_UFS_CONTROLLER_H_

#include <Iosf2OcpPort.h>
#include <ScsConfig.h>
#include <ScsInfoHob.h>

typedef struct _SCS_UFS_HANDLE SCS_UFS_HANDLE;

typedef struct {
  REGISTER_ACCESS *RegisterAccess;
  UINT8           LaneMask;
} SCS_UFS_MMP;

/**
  Called as a last step in UFS controller disable flow.

  @param[in] ScsUfsHandle  Handle
**/
typedef
VOID
(*SCS_UFS_DISABLE) (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsUfsHandle  Handle
**/
typedef
VOID
(*SCS_UFS_BAR1_DISABLE) (
  IN SCS_UFS_HANDLE  *ScsUfsHandle
  );

typedef struct {
  SCS_UFS_DISABLE             Disable;
  SCS_UFS_BAR1_DISABLE        Bar1Disable;
} SCS_UFS_CALLBACKS;

typedef struct {
  BOOLEAN  IsBootMedium;
  UINT8    NumOfLanes;
  UINT8    IntPin;
  UINT8    Irq;
} SCS_UFS_SOC_CONFIG;

typedef struct {
  IOSF2OCP_PORT  *Iosf2OcpPort; // Iosf2OcpPort to which UFS controller belongs.
  SCS_UFS_MMP    *Mmp; // MMP instance used by this controller
  UINT64         PciCfgBase; // PCI configuration base
  UINT64         MmioBase; // MMIO for usage in BAR access
} SCS_UFS_CONTROLLER;

struct _SCS_UFS_HANDLE {
  SCS_UFS_CONTROLLER         Controller; // Describes UFS controller location
  SCS_UFS_SOC_CONFIG         SocConfig; // Describes SoC specific config
  SCS_UFS_CONTROLLER_CONFIG  *Config; // Describes platform config
  SCS_UFS_CALLBACKS          Callbacks; // Set of SoC callbacks
  SCS_UFS_INFO               *UfsInfo; // Contains initialization status
};

#endif
