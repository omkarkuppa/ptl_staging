/** @file
  This file contains functions that initialize PCI Express clock sources in PCH.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#include <Ppi/SiPolicy.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiPchPcieClocksLib.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>
#include <Library/TimerLib.h>
#include <Library/PchFiaLib.h>
#include <GpioV2Functionalities.h>

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFiaAssignPchPciePortsClkReq (
  IN UINT8  *ClkReqMap,
  IN UINT8  ClkReqMapSize
  );

/**
  Assigns CLKREQ# to GbE

  @param[in]  ClkReqNum  CLKREQ# number
**/
VOID
PchFiaAssignGbeClkReq (
  IN UINT8  ClkReqNum
  );

/**
  Retrieves and returns PCIe Clocks Policy.

  @retval  pointer to PCIe Clocks Policy
**/
PCH_PCIE_CLOCK*
GetClocksPolicy (
  VOID
  )
{
  EFI_STATUS                         Status;
  SI_PREMEM_POLICY_PPI               *SiPreMemPolicyPpi;
  PCH_PCIE_RP_PREMEM_CONFIG          *PchPciePeiPreMemConfig;

  //
  // Get Policy settings through the SiPolicy PPI
  //
  Status = PeiServicesLocatePpi (
             &gSiPreMemPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &SiPreMemPolicyPpi
             );
  ASSERT_EFI_ERROR (Status);
  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gPcieRpPreMemConfigGuid, (VOID *) &PchPciePeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);
  return &(PchPciePeiPreMemConfig->PcieClock[0]);
}

/**
  Assign CLKREQ# to PCH PCIe ports
**/
VOID
PchConfigurePchPciePortsClkReqMapping (
  VOID
  )
{
  UINT8  MaxPciePortNum;
  UINT8  PortIndex;
  UINT8  ClkReqMap[PCH_MAX_PCIE_ROOT_PORTS];
  UINT8  ClkReqNum;

  MaxPciePortNum = GetPchMaxPciePortNum ();
  ASSERT (MaxPciePortNum <= PCH_MAX_PCIE_ROOT_PORTS);

  for (PortIndex = 0; PortIndex < MaxPciePortNum; PortIndex++) {
    ClkReqNum = FindClkReqForUsage (PchClockUsagePchPcie0 + PortIndex);
    if (ClkReqNum == PCH_PCIE_NO_SUCH_CLOCK) {
      ClkReqMap[PortIndex] = PCH_FIA_NO_CLKREQ;
    } else {
      ClkReqMap[PortIndex] = ClkReqNum;
    }
  }

  PchFiaAssignPchPciePortsClkReq (ClkReqMap, MaxPciePortNum);
}

/**
  Assign CLKREQ# to GbE
**/
VOID
PchConfigureGbeClkReqMapping (
  VOID
  )
{
  UINT8  ClkReqNum;

  ClkReqNum = FindClkReqForUsage (PchClockUsageLan);
  if (ClkReqNum == PCH_PCIE_NO_SUCH_CLOCK) {
    PchFiaAssignGbeClkReq (PCH_FIA_NO_CLKREQ);
  } else {
    PchFiaAssignGbeClkReq (ClkReqNum);
  }
}

/**
  Assign CLKREQ# to controllers
  Assignment is based on the platform policy.
**/
VOID
PchConfigureClkreqMapping (
  VOID
  )
{
  PchConfigurePchPciePortsClkReqMapping ();
  PchConfigureGbeClkReqMapping ();
}

/**
  Checks if given PCIe port is assigned with Clock Request signal

  @param[in]  ClockUsage  type and number of PCIe port to check ClkReq for
  @retval     TRUE        there's a ClkReq pad corresponding to given Port
  @retval     FALSE       there's no ClkReq pad corresponding to given Port
**/
BOOLEAN
IsClkReqAssigned (
  PCH_PCIE_CLOCK_USAGE ClockUsage
  )
{
  UINT32 ClkNumber;
  UINT32 ClkReqNumber;

  ClkNumber = ClockUsageToClockNumber (GetClocksPolicy (), ClockUsage);
  if (ClkNumber == PCH_PCIE_NO_SUCH_CLOCK) {
    return FALSE;
  }
  ClkReqNumber = ClockNumberToClkReqNumber (GetClocksPolicy (), ClkNumber);
  if (ClkReqNumber == PCH_PCIE_NO_SUCH_CLOCK) {
    return FALSE;
  }
  return TRUE;
}

/**
  Disables one PCIe clock.

  @param[in] ClockUsage    type and number of PCIe port for which Clock should be disabled
**/
VOID
DisableClock (
  PCH_PCIE_CLOCK_USAGE ClockUsage
  )
{
  UINT32 ClkNumber;

  ClkNumber = ClockUsageToClockNumber (GetClocksPolicy (), ClockUsage);
  if (ClkNumber == PCH_PCIE_NO_SUCH_CLOCK) {
    return;
  }
  DisableClockBufferProgramming (ClockUsage, ClkNumber);
}

/**
  Disables all PCIe clocks which are not used according to Policy
**/
VOID
DisableUnusedPcieClocks (
  VOID
  )
{
  UINT32          Index;
  PCH_PCIE_CLOCK* ClocksPolicy;

  ClocksPolicy = GetClocksPolicy ();
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    if (ClocksPolicy[Index].Usage == PchClockUsageNotUsed) {
      DisableClockBufferProgramming (ClocksPolicy[Index].Usage, Index);
    }
  }
}

/**
  This function scans PCH PCIE configuration and finds CLK_REQ signal that corresponds to given usage.

  @param[in]  ClockUsage   purpose of this CLK_SRC signal, either root port index or LAN
  @retval     number of CLK_REQ signal if any is assigned, PCH_PCIE_NO_SUCH_CLOCK otherwise
**/
UINT8
FindClkReqForUsage (
  PCH_PCIE_CLOCK_USAGE  ClockUsage
  )
{
  UINT32 Index;
  PCH_PCIE_CLOCK* ClocksPolicy;

  ClocksPolicy = GetClocksPolicy ();
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    if (ClocksPolicy[Index].Usage == ClockUsage) {
      if (ClocksPolicy[Index].ClkReq < GetPchMaxPcieClockReqNum ()) {
        return ClocksPolicy[Index].ClkReq;
      } else {
        return PCH_PCIE_NO_SUCH_CLOCK;
      }
    }
  }
  return PCH_PCIE_NO_SUCH_CLOCK;
}

/**
  This function scans PCH PCIE configuration and finds Clk Req GPIO that corresponds to given usage.

  @param[in]  ClockUsage   purpose of this CLK_SRC signal, either root port index or LAN
  @retval     number of CLK_REQ signal if any is assigned, PCH_PCIE_NO_SUCH_CLOCK otherwise
**/
UINT32
FindGpioPinMuxForClkReq (
  PCH_PCIE_CLOCK_USAGE  ClockUsage
  )
{
  UINT32 Index;
  PCH_PCIE_CLOCK* ClocksPolicy;

  ClocksPolicy = GetClocksPolicy ();
  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    if (ClocksPolicy[Index].Usage == ClockUsage) {
      return ClocksPolicy[Index].ClkReqGpioPinMux;
    }
  }
  return PCH_PCIE_NO_GPIO_MUXING;
}

/**
  This function scans PCH PCIE configuration and finds clock number that corresponds to given usage.
  If there's no clock assigned, it will return PCH_PCIE_NO_SUCH_CLOCK

  @param[in]  ClocksPolicy  PCIe clocks configuration policy structure
  @param[in]  ClockUsage   user of clock, either PCH PCIe port, CPU PCIe port or LAN
  @retval     number of clock if any is assigned, PCH_PCIE_NO_SUCH_CLOCK otherwise
**/
UINT8
ClockUsageToClockNumber (
  CONST PCH_PCIE_CLOCK* ClocksPolicy,
  PCH_PCIE_CLOCK_USAGE  ClockUsage
  )
{
  UINT8 Index;

  for (Index = 0; Index < GetPchMaxPcieClockNum (); Index++) {
    if (ClocksPolicy[Index].Usage == ClockUsage) {
      return Index;
    }
  }
  return 0xFF;
}

/**
  This function scans PCH PCIE configuration and finds Clock Request signal that corresponds to given Clock Source.
  If there's no CLK_REQ, it will return 0xFF

  @param[in]  ClocksPolicy  PCIe clocks configuration policy structure
  @param[in]  ClkSrcNumber  purpose of this CLK_SRC signal, either PCH PCIe port, CPU PCIe port or LAN
  @retval     number of CLK_REQ signal if any is assigned, PCH_PCIE_NO_SUCH_CLOCK otherwise
**/
UINT8
ClockNumberToClkReqNumber (
  CONST PCH_PCIE_CLOCK* ClocksPolicy,
  UINT32                ClkSrcNumber
  )
{
  if (ClocksPolicy[ClkSrcNumber].ClkReq < GetPchMaxPcieClockReqNum ()) {
    return ClocksPolicy[ClkSrcNumber].ClkReq;
  } else {
    return PCH_PCIE_NO_SUCH_CLOCK;
  }
}

/**
  This function scans PCH PCIE configuration and finds Gpio Pin Mux Value that corresponds to given Clock Source.
  If there's no CLK_REQ, it will return 0xFF

  @param[in]  ClocksPolicy  PCIe clocks configuration policy structure
  @param[in]  ClkSrcNumber  purpose of this CLK_SRC signal, either PCH PCIe port, CPU PCIe port or LAN
  @retval     number of CLK_REQ signal if any is assigned, PCH_PCIE_NO_SUCH_CLOCK otherwise
**/
UINT32
ClockNumberToClkReqGpioPinMux (
  CONST PCH_PCIE_CLOCK* ClocksPolicy,
  UINT32                ClkSrcNumber
  )
{
  if (ClkSrcNumber < GetPchMaxPcieClockReqNum ()) {
    return ClocksPolicy[ClkSrcNumber].ClkReqGpioPinMux;
  } else {
    return PCH_PCIE_NO_GPIO_MUXING;
  }
}
