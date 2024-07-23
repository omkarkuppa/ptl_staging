/** @file
  PMC SoC configuration

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

#ifndef _PMC_CONTROLLER_H_
#define _PMC_CONTROLLER_H_

#include <PmcSocConfig.h>
#include <PmConfig.h>
#include <FivrConfig.h>

typedef struct _PMC_HANDLE PMC_HANDLE;

/**
  Perform SoC specific SLP_S0 configuration.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_SLPS0) (
  IN PMC_HANDLE *PmcHandle
  );

/**
  Configure CPPM

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_CPPM) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_POWER_BUTTON_DEBOUNCE) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after CPU C10 gate is configured on PMC side to
  finish up feature configuration on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_CPU_C10_GATE) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Called after PMC is done configuring VR alert to finish up
  init on SoC side.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_VR_ALERT) (
  IN PMC_HANDLE  *PmcHandle
  );

/**
  Perform SoC specific PSON configuration.

  @param[in] PmcHandle  Pointer to PMC handle
**/
typedef
VOID
(*PMC_CONFIGURE_PSON) (
  IN PMC_HANDLE *PmcHandle
  );

typedef struct {
  PMC_CONFIGURE_SLPS0                  PmcConfigureSlpS0;
  PMC_CONFIGURE_CPPM                   PmcConfigureCppm;
  PMC_CONFIGURE_POWER_BUTTON_DEBOUNCE  PmcConfigurePowerButtonDebounce;
  PMC_CONFIGURE_CPU_C10_GATE           PmcConfigureCpuC10Gate;
  PMC_CONFIGURE_VR_ALERT               PmcConfigureVrAlert;
} PMC_CALLBACK;

struct _PMC_HANDLE {
  UINTN           PwrmBase;
  UINT64          PciBase;
  PMC_CALLBACK    *PmcCallback;
  PMC_SOC_CONFIG  *PmcSocConfig;
  PCH_PM_CONFIG   *PmConfig;
};

typedef struct {
  UINTN                PwrmBase;
  PMC_FIVR_SOC_CONFIG  *PmcFivrSocConfig;
  PCH_FIVR_CONFIG      *FivrConfig;
  PCH_PM_CONFIG        *PmConfig;
} PMC_FIVR_HANDLE;

#endif

