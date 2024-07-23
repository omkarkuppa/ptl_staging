/** @file
  DMI device Init header file

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
#ifndef __DMI_INIT_H__
#define __DMI_INIT_H__

#include <Ppi/SiPolicy.h>
#include <Library/PeiDmiTaskDispatchLib.h>

extern IP_DMI3_FEAT_ID_DISPATCH_ENTRY DmiPrememCallTable[];
extern IP_DMI3_FEAT_ID_DISPATCH_ENTRY DmiPostMemCallTable[];

/**
  DpDmiPrememInit: Initialize the DPDMI device

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiPrememInit (
  IN  VOID    *PeiDmiConfig
  );

/**
  DpDmiPostMemInit: Initialize the DMI device

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiPostMemInit (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Basic Init.

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiBasicInit (
  IN  VOID    *PeiDmiConfig
  );

/*
  Program registers for Elastic Buffer

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiElasticBufferProgramming (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI clock gating.
  This function must be called before ASPM is enabled

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
*/
EFI_STATUS
EFIAPI
DpDmiConfigureClockGating (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI link power management.
  This function must be called before ASPM is enabled

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureLinkPowerManagement (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Squelch Power Management.

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureSquelchPowerManagement (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI PLL Shutdown.

  @param[in] PEI_DMI_CONFIG            *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePllShutdown (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Power Gating.
  This needs to done before enabling ASPM L1

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePowerGating (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Coalescing

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureCoalescing (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Atomics

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureAtomics (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI SMI/SCI

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureSmiSci (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure TLP Prefix

  @param[in] PEI_DMI_CONFIG            *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureTlpPrefix (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Ack Latency

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureAckLatency (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Design Specific Configuration

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureDesignSpecificConfiguration (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Enabling

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureEnabling (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI 10 Bit Tag and Scaled Flow Control

  @param[in] PEI_DMI_CONFIG             *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigure10BitTagAndScaledFlowControl (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI MultiVC support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureMultiVC (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Ptm Init

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePtm (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Retimer support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureRetimerSupport (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI Rx Primary Cycle Decode support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureRxPrimaryCycleDecode (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DMI DPC support

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureDPC (
  IN  VOID    *PeiDmiConfig
  );

/**
  This function configure PCIe power down mapping

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigurePowerDownMapping (
  IN  VOID    *PeiDmiConfig
  );

/**
  Configure DPDMI Speed Change

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiSpeedChange (
  IN  VOID    *PeiDmiConfig
  );

/**
  DPDMI Enable Relaxed Order
  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig

**/
EFI_STATUS
EFIAPI
DpDmiEnablePcieRelaxedOrder (
    IN  VOID    *PeiDmiConfig
  );

/**
  Configure Rx Margin Programming
  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig

**/
EFI_STATUS
EFIAPI
DpDmiConfigureRxMarginProgramming (
    IN  VOID    *PeiDmiConfig
  );

/**
  Configure DPDMI Max Payload Size.

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureMPS (
  IN  VOID    *PeiDmiConfig
  );

/**
      This function performs configuration of DMI D3 Vnn removal

  @param[in] PEI_DMI_CONFIG    *PeiDmiConfig
**/
EFI_STATUS
EFIAPI
DpDmiConfigureD3VnnRemCtl (
  IN  VOID    *PeiDmiConfig
  );

/**
  UpDmiConfigureAspm: Configures UpDmi ASPM
**/
VOID
EFIAPI
UpDmiConfigureAspm (
  VOID
  );

/**
  UpDmiConfigureTls: Configure UpDmi link speed

  @param[in] UINT8    TargetLinkSpeed
  @param[in] UINT8    DmiEqMode
**/
VOID
EFIAPI
UpDmiConfigureTls (
  IN  UINT8 TargetLinkSpeed,
  IN  UINT8                   DmiEqMode
  );

/**
  UpDmiGetPchPciePeerMemoryWriteStatus

  @param[out] BOOLEAN    PchPciePeerMemoryWriteStatus
**/
VOID
EFIAPI
UpDmiGetPchPciePeerMemoryWriteStatus (
  OUT BOOLEAN    *PchPciePeerMemoryWriteStatus
);

/**
  UpDmiConfigureL1ss: Configures UpDmi L1SS
**/
VOID
EFIAPI
UpDmiConfigureL1ss (
  VOID
  );

/**
  Enable Port8x decode for DP DMI.

  @param[in] PeiDpDmiConfig  DP DMI configuration
**/
VOID
DpDmiEnablePort8xDecode (
  IN PEI_DMI_CONFIG  *PeiDpDmiConfig
  );
/**
  Dump Figure of merit scoreboard(FOMS) for each of the lanes.

  @param[in] Speed        Speed for which FOM is requested
**/
VOID
EFIAPI
UpDmiDumpFoms (
  IN UINT8                 Speed
);
#endif
