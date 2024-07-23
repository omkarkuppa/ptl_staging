/** @file
  HSIO routines

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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
#ifndef _PEI_HSIO_LIB_H_
#define _PEI_HSIO_LIB_H_

#include <Ppi/SiPolicy.h>
#include <HsioHandle.h>

/**
PCH PCIE PLL SSC Data
**/
#define MAX_PCIE_PLL_SSC_PERCENT  20
#define PCIE_PLL_SSC_ZERO_PERCENT 0
#define PCIE_PLL_SSC_PT_5_PERCENT 5
#define PCIE_PLL_SSC_AUTO         0xFF
#define PCIE_PLL_SSC_DISABLE      0xFE

/**
  Read HSIO lane DWORD

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @return DWORD value
**/
UINT32
HsioLaneRead32 (
  HSIO_LANE *HsioLane,
  UINT32 Offset
  );

/**
  Write HSIO lane DWORD

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] Data32     DWORD Value
**/
VOID
HsioLaneWrite32 (
  HSIO_LANE *HsioLane,
  UINT32          Offset,
  UINT32          Data32
  );

/**
  Modify HSIO lane DWORD.

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] AndMask    Mask to be ANDed with original value.
  @param[in] OrMask     Mask to be ORed with original value.
**/
VOID
HsioLaneAndThenOr32 (
  HSIO_LANE *HsioLane,
  UINT32          Offset,
  UINT32          AndMask,
  UINT32          OrMask
  );

/**
  Modify HSIO lane DWORD and update ChipsetInit SUS Table.

  @param[in] HsioLane   HSIO Lane
  @param[in] Offset     PCR offset
  @param[in] AndMask    Mask to be ANDed with original value.
  @param[in] OrMask     Mask to be ORed with original value.
**/
VOID
HsioLaneAndThenOr32WithSusWrite (
  HSIO_HANDLE     *HsioHandle,
  UINT32          Offset,
  UINT32          AndMask,
  UINT32          OrMask
  );

/**
  Write PCR register and update ChipsetInit SUS Table.
  It programs PCR register and size in 4bytes then update ChipsetInit SUS Table..
  The Offset should not exceed 0xFFFF and must be aligned with size.

  @param[in]  Pid      Port ID
  @param[in]  Offset   Register offset of Port ID.
  @param[in]  AndData  AND Data. Must be the same size as Size parameter.
  @param[in]  OrData   OR Data. Must be the same size as Size parameter.

**/
VOID
PchPcrAndThenOr32WithSusWrite (
  IN  HSIO_HANDLE                       *HsioHandle,
  IN  UINT32                            Offset,
  IN  UINT32                            AndData,
  IN  UINT32                            OrData
  );

/**
  Initialize HSIO lane for non-common clock PCIe port.

  @param[in] HsiLane   HSIO Lane
**/
VOID
HsioPcieNccLaneInit (
  IN HSIO_LANE     *HsioLane
);

/**
  Tune the USB 3.0 signals quality.

  @param[in]  Usb3HsioConfig      The USB3_HSIO_CONFIG policy instance

**/
VOID
XhciUsb3HsioTuning (
  IN  HSIO_HANDLE             *HsioHandle,
  IN  UINT8                   Index
  );

/**
  Enables SATA test mode in Modular PHY

  @param[in]  SataCtrlIndex     SATA controlller index
  @param[in]  CtrlPortNum       SATA controlller number of ports

**/
VOID
HsioSataTestModeEnable (
  IN  HSIO_LANE      *HsioLane
  );

/**
  The function programs HSIO Rx\Tx Eq policy registers for SATA lanes.

  @param[in]  SiPreMemPolicyPpi  The SI PreMem Policy PPI instance
**/
VOID
HsioRxTxEqPolicySataProg (
  IN  HSIO_HANDLE             *HsioHandle,
  IN  UINT32                  Index
  );

/**
  The function programs HSIO Rx\Tx Eq policy registers for PCIe lanes.

  @param[in]  SiPreMemPolicyPpi  The SI PreMem Policy PPI instance
**/
VOID
HsioRxTxEqPolicyPcieProg (
  IN     HSIO_HANDLE             *HsioHandle,
  IN     UINT8                   Index,
  IN     UINT64                  RpBase,
  IN OUT UINT32                  *RpToRetrain
  );

/**
  This function enables PLL SSC modulator.

  @param[in]  HsioHandle  Pointer to HSIO handle structure
**/
VOID
PchPciePllSscEnable (
  IN  HSIO_HANDLE  *HsioHandle
  );

/**
  This function disables PLL SSC modulator.

  @param[in]  HsioHandle  Pointer to HSIO handle structure
**/
VOID
PchPciePllSscDisable (
  IN  HSIO_HANDLE  *HsioHandle
  );

/**
  The function programs the Pcie Pll SSC registers.

  @param[in]  SiPolicyPpi        The SI Policy PPI instance
**/
VOID
PchPciePllSscProg (
  IN  HSIO_HANDLE      *HsioHandle
  );

/**
  The function programs the TSN Pll PwrGating registers.
**/
VOID
PchTsnHsioPllPwrGatingProg (
  IN  HSIO_HANDLE      *HsioHandle
  );

VOID
PchHsioEnablePcieComplianceMode (
  IN  HSIO_HANDLE      *HsioHandle
  );

/**
  The function programs the Pcie Pll PwrGating registers for unused lanes.
**/
VOID
PchPcieHsioPllPwrGatingProg (
  IN  HSIO_HANDLE      *HsioHandle
  );

/**
  This function configures I/O Margining tool support

  @param[in] HsioHandle  Pointer to HSIO handle structure
  @param[in] PwrmBase    PWRM base address
**/
VOID
PchHsioConfigureIoMargining (
  IN HSIO_HANDLE *HsioHandle,
  IN UINTN       PwrmBase
  );

#endif // _PEI_HSIO_LIB_H_
