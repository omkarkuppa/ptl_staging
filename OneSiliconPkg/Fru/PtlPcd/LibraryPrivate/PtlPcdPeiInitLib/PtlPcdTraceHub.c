/** @file
  Initializes TraceHub Device in PEI

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
#include "PtlPcdInitPei.h"
#include <Library/PeiTraceHubInitLib.h>

/**
  Initialize Soc TraceHub

  @param[in] SiPreMemPolicyPpi        The SI PreMem Policy PPI instance

**/
VOID
PtlPcdTraceHubInitPreMem (
  IN  SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi
  )
{
  BOOLEAN                            DisableTraceHub;
  TRACEHUB_HANDLE                    TraceHubHandle;
  TRACEHUB_CONTROLLER                TraceHubController;
  TRACEHUB_PRIVATE_CONFIG            PrivateConfig;
  TRACEHUB_CALLBACK                  Callback;
  P2SB_CONTROLLER                    P2SbController;
  P2SB_SIDEBAND_REGISTER_ACCESS      TraceHubSbAccessMmio;

  DisableTraceHub = FALSE;

  TraceHubInitSocHandle (
    SiPreMemPolicyPpi,
    &TraceHubHandle,
    &TraceHubController,
    &PrivateConfig,
    &Callback,
    &P2SbController,
    &TraceHubSbAccessMmio
    );
  TraceHubInitialize (&TraceHubHandle, &DisableTraceHub);

  if (DisableTraceHub) {
    TraceHubDisable (&TraceHubHandle);
  }
}
