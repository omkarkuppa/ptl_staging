/** @file
  Header file for TraceHub Init Lib.
  All functions in this library are available for PEI.

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
#ifndef _PEI_TRACEHUB_INIT_LIB_H_
#define _PEI_TRACEHUB_INIT_LIB_H_

#include <TraceHubHandle.h>
#include <Ppi/SiPolicy.h>
#include <Library/P2SbSidebandAccessLib.h>

/**
  This function performs basic initialization for TraceHub, and return if trace hub needs to be power gated.

  Although controller allows access to a 64bit address resource, PEI phase is a 32-bit environment,
  addresses greater than 4G is not allowed by CPU address space.
  So, the addresses must be limited to below 4G and UBARs should be set to 0.
  If scratchpad0 bit24 is set, which indicates trace hub is used by tool that implies probe is connected and capturing traces.
  With such assumption, BIOS may not power gate trace hub but initialize trace hub as host debugger mode despite what trace hub policy it is.

  @param[in]  TraceHubHandle         Pointer to TraceHub handle structure
  @param[in, out]  TraceHubDisable   Determine if TraceHub needs to be power-gated.
                                     TRUE : Power gate TraceHub
                                     FALSE : Do not power-gate TraceHub
**/
VOID
TraceHubInitialize (
  IN TRACEHUB_HANDLE                 *TraceHubHandle,
  IN OUT  BOOLEAN                    *TraceHubDisable
  );

/**
  Disable and power gate TraceHub

  @param[in]  TraceHubHandle       Pointer to TraceHub handle structure

  @retval     EFI_SUCCESS       The function completed successfully.
  @retval     EFI_UNSUPPORTED   The device is not present
**/
EFI_STATUS
TraceHubDisable (
  IN TRACEHUB_HANDLE                 *TraceHubHandle
  );

/**
  TraceHub initialization in PostMem.

  Configure TraceHub memory and device mode

  @param[in] TraceHubHandle          Pointer to TraceHub handle structure
  @param[in, out] TraceHubMemBase    Pointer TraceHub memory base
**/
VOID
TraceHubInitPostMem (
  IN TRACEHUB_HANDLE           *TraceHubHandle,
  IN OUT UINT64                *TraceHubMemBase
  );

/**
  Create and initialize TraceHub Data HOB

  @param[in] SiPreMemPolicyPpi The SI Pre-Mem Policy PPI instance
  @param[in] FwBase            TraceHub Firmware BAR
**/
VOID
TraceHubDataHobInit (
  IN SI_PREMEM_POLICY_PPI        *SiPreMemPolicyPpi,
  IN UINT32                      FwBase
  );

/**
  Store trace verbosity value into HOB

  @param[in] TraceVerbosity BIOS trace verbosity
  @param[in] FwBase         TraceHub Firmware Trace BAR
**/
VOID
UpdateTraceHubDataHob (
  IN UINT8           TraceVerbosity,
  IN UINT32          FwBase
  );

/**
  Initialize Soc TraceHub handle structure
  This should be done according TraceHub IP integration in SoC

  @param[in] SiPreMemPolicyPpi       The SI PreMem Policy PPI instance
  @param[in out]  TraceHubHandle     TraceHub handle structure
  @param[in] TraceHubController      TRACEHUB_CONTROLLER structure
  @param[in] PrivateConfig           TRACEHUB_PRIVATE_CONFIG structure
  @param[in] P2SbController          P2SB controller pointer
  @param[in] Callback                TRACEHUB_CALLBACK structure
  @param[in] TraceHubSbAccessMmio    P2SB Register Access to TraceHub
**/
VOID
TraceHubInitSocHandle (
  IN SI_PREMEM_POLICY_PPI           *SiPreMemPolicyPpi,
  IN OUT  TRACEHUB_HANDLE            *TraceHubHandle,
  IN TRACEHUB_CONTROLLER             *TraceHubController,
  IN TRACEHUB_PRIVATE_CONFIG         *PrivateConfig,
  IN TRACEHUB_CALLBACK               *Callback,
  IN P2SB_CONTROLLER                 *P2SbController,
  IN P2SB_SIDEBAND_REGISTER_ACCESS   *TraceHubSbAccessMmio
  );

/**
  Determine if Soc TraceHub is in use by tool

  @retval     TRUE           - debugger is in use
              FALSE          - debugger is not in use
**/
BOOLEAN
DebuggerInUseSoc (
  VOID
  );

/**
  Read TraceHub Scratchpad 1 register

  @param[out] Scrpd1                 Scrpd1 value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
ReadTraceHubScrpd1 (
  OUT UINT32       *Scrpd1
  );

/**
  Write TraceHub Scratchpad 1 register

  @param[in]  Scrpd1                 Scrpd1 value

  @retval     EFI_SUCCESS            Completed successfully
              EFI_UNSUPPORTED        TraceHub does not exist
**/
EFI_STATUS
WriteTraceHubScrpd1 (
  IN UINT32       *Scrpd1
  );

/**
  Reserve Trace Hub Memory

  @param[in] TopUseableMemAddr  Top usable memory address
  @param[in] TopMemSize         Top of memory size
**/
VOID
PeiTraceHubMemoryAllocation(
  IN OUT EFI_PHYSICAL_ADDRESS         *TopUseableMemAddr,
  IN OUT UINT64                       *Touud
);

#endif // _PEI_TRACEHUB_INIT_LIB_H_
