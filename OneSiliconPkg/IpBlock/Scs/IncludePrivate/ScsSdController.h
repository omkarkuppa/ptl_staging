/** @file
  SCS SD controller definition.

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

#ifndef _SCS_SD_CONTROLLER_H_
#define _SCS_SD_CONTROLLER_H_

#include <Iosf2OcpPort.h>
#include <ScsConfig.h>
#include <ScsInfoHob.h>

typedef struct _SCS_SDCARD_HANDLE  SCS_SDCARD_HANDLE;
typedef struct _SCS_EMMC_HANDLE  SCS_EMMC_HANDLE;

/**
  Called as a last step in SD card controller disable flow.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_DISABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_BAR1_DISABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

/**
  Called after SD card MMIO init with the intention to enable SD card
  phy.

  @param[in] ScsSdCardHandle  Handle
**/
typedef
VOID
(*SCS_SDCARD_PHY_ENABLE) (
  IN SCS_SDCARD_HANDLE  *ScsSdCardHandle
  );

typedef struct {
  SCS_SDCARD_DISABLE       Disable;
  SCS_SDCARD_BAR1_DISABLE  Bar1Disable;
  SCS_SDCARD_PHY_ENABLE    PhyEnable;
} SCS_SDCARD_CALLBACKS;

/**
  Called as a last step in eMMC controller disable flow.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_DISABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

/**
  Called after disabling BAR1 in IOSF2OCP.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_BAR1_DISABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

/**
  Called after eMMC MMIO init with the intention to enable eMMC
  phy.

  @param[in] ScsEmmcHandle  Handle
**/
typedef
VOID
(*SCS_EMMC_PHY_ENABLE) (
  IN SCS_EMMC_HANDLE  *ScsEmmcHandle
  );

typedef struct {
  SCS_EMMC_DISABLE       Disable;
  SCS_EMMC_BAR1_DISABLE  Bar1Disable;
  SCS_EMMC_PHY_ENABLE    PhyEnable;
} SCS_EMMC_CALLBACKS;

typedef struct {
  UINT32  CapReg1;
  UINT32  CapReg2;
} SCS_SD_CAPS;

typedef struct {
  SCS_SD_CAPS  Caps;
  SCS_SD_DLL   DefaultDll;
  UINT8        IntPin;
  UINT8        Irq;
} SCS_SD_SOC_CONFIG;

typedef struct {
  IOSF2OCP_PORT  *Iosf2OcpPort; // Iosf2OcpPort to which SD controller belongs.
  UINT64         PciCfgBase; // PCI configuration base
  UINTN          MmioBase; // MMIO for usage in BAR access
} SCS_SD_CONTROLLER;

struct _SCS_SDCARD_HANDLE {
  SCS_SD_CONTROLLER     Controller; // Describes controller location
  SCS_SD_SOC_CONFIG     SocConfig; // Describes SoC specific configuration
  SCS_SDCARD_CONFIG     *Config; // Describes platform configuration
  SCS_SDCARD_CALLBACKS  Callbacks;  // SD card callbacks
  SCS_SDCARD_INFO       *SdCardInfo; // Contains initialization status
};

struct _SCS_EMMC_HANDLE {
  SCS_SD_CONTROLLER   Controller; // Describes controller location
  SCS_SD_SOC_CONFIG   SocConfig; // Describes SoC specific configuration
  SCS_EMMC_CONFIG     *Config; // Describes platform configuration
  SCS_EMMC_CALLBACKS  Callbacks; // eMMC callbacks
  SCS_EMMC_INFO       *EmmcInfo; // Contains initialization status
};

#endif
