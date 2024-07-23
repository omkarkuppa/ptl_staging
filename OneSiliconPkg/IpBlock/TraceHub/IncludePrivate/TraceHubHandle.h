/** @file
  TraceHub Handle definitions

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

@par Specification
**/
#ifndef _TRACEHUB_HANDLE_H_
#define _TRACEHUB_HANDLE_H_

#include <ConfigBlock.h>
#include <TraceHubController.h>
#include <TraceHubConfig.h>
#include <RegisterAccess.h>

/**
  Internal TraceHub policy options
**/
typedef struct {
  /**
    Reserved MMIO region for TraceHub BARs
   **/
  UINT64               MtbBar;   ///< TraceHub configurable base address for CSRs (Configuration and Status Registers) and MTB (Memory Trace Buffer)
  UINT64               SwBar;    ///< TraceHub Software Trace BAR
  UINT64               RtitBar;  ///< TraceHub RTIT Trace BAR. When it is 0, RTIT bar is not implemented.
  UINT64               RtitSize; ///< TraceHub RTIT Trace BAR size
  UINT64               FwBar;    ///< TraceHub Firmware Trace BAR
  BOOLEAN              AcroClkSupported; ///< Determine if TraceHub uses HD Audio clock for Sx
  UINT8                RootSpace; ///< TraceHub memory root space
} TRACEHUB_PRIVATE_CONFIG;

/**
  Return if need to keep tracing which is activated by default

  @param[in]  TraceHubController      Pointer to TraceHub controller structure

  @retval TRUE    Keep Trace
  @retval FALSE   Stop Trace
**/
typedef
BOOLEAN
(*TRACEHUB_KEEP_TRACE) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  Enable TraceHub FW_BAR.
  Since FW BAR is not conventional PCI BAR offset (0x70) that needs to be shadowed into
  independent PCI device in PSF -- TraceHub phantom device.
  BIOS programs TraceHub phantom device's bar 0 with FW Base and enable MSE
  to make FW BAR MMIO available

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
  @param[in]  FwBar                   FW Bar Address
**/
typedef
VOID
(*TRACEHUB_ENABLE_FW_BAR) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController,
  IN  UINT64                   FwBar
  );

/**
  Set AET trace.
  AET is one of FW trace agents, need configure FW BAR for AET.

  @param[in]  FwBar                   FW Bar Address
**/
typedef
VOID
(*TRACEHUB_SET_AET_TRACE) (
  IN  UINT64                   FwBar
  );

/**
  Enable TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
typedef
VOID
(*TRACEHUB_ENABLE) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  Disable TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
typedef
VOID
(*TRACEHUB_DISABLE) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  Enable ACRO clock for TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
typedef
VOID
(*TRACEHUB_EN_CLK) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  Disable ACRO clock for TraceHub

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
typedef
VOID
(*TRACEHUB_DIS_CLK) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  Lock TraceHub power gate state

  @param[in]  TraceHubController      Pointer to TraceHub controller structure
**/
typedef
VOID
(*TRACEHUB_LOCK_PG_STATE) (
  IN  TRACEHUB_CONTROLLER      *TraceHubController
  );

/**
  TraceHub callbacks
  List of function pointers can be passed to IP Block driver
**/
typedef struct {
  TRACEHUB_KEEP_TRACE            TraceHubKeepTrace;
  TRACEHUB_ENABLE_FW_BAR         TraceHubEnableFwBar;
  TRACEHUB_SET_AET_TRACE         TraceHubSetAetTrace;
  TRACEHUB_ENABLE                TraceHubEnable;
  TRACEHUB_DISABLE               TraceHubDisable;
  TRACEHUB_EN_CLK                TraceHubEnClk;
  TRACEHUB_DIS_CLK               TraceHubDisClk;
} TRACEHUB_CALLBACK;

/**
  TraceHub handle structure
  Stores all data necessary to initialize TraceHub IP block
**/
typedef struct {
  TRACEHUB_CONTROLLER             *Controller;
  TRACE_HUB_CONFIG                *TraceHubConfig;
  TRACEHUB_PRIVATE_CONFIG         *PrivateConfig;
  TRACEHUB_CALLBACK               *Callback;
  REGISTER_ACCESS                 *TraceHubSbAccessMmio; /*Tracehub Bar0 MMIO access */
} TRACEHUB_HANDLE;

#endif // _TRACEHUB_HANDLE_H_
