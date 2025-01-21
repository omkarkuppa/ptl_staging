/** @file
  PCIe RP SoC library.

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

#ifndef _PTL_PCD_PCIE_RP_LIB_H_
#define _PTL_PCD_PCIE_RP_LIB_H_

#include <Library/P2SbSidebandAccessLib.h>
#include <IpCpcie.h>
#include <PchPcieRpConfig.h>

/**
  Translate RpIndex to controller index.

  @param[in] RpIndex  Root port index

  @return ControllerIndex
  @retval 0xFF Failed to lookup controller index.
**/
UINT32
PtlPcdRpIndexToControllerIndex (
  IN UINTN  RpIndex
  );

/**
  Get the FID number of the PCIe root port.

  @param[in] RpIndex  Index of the root port

  @return FID number to use when sending SBI msg
**/
UINT16
PtlPcdPcieRpGetSbiFid (
  IN UINTN  RpIndex
  );

/**
  Get PTL SoC Pcie Root Port Device and Function Number by Root Port physical Number

  @param[in]  RpNumber              Root port physical number. (0-based)
  @param[out] RpDev                 Return corresponding root port device number.
  @param[out] RpFun                 Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port device and function is retrieved
  @retval     EFI_INVALID_PARAMETER RpNumber is invalid
**/
EFI_STATUS
PtlPcdGetPcieRpDevFun (
  IN  UINTN   RpNumber,
  OUT UINT16  *RpDev,
  OUT UINT16  *RpFun
  );

/**
  This function returns PID according to PCIe controller index

  @param[in] ControllerIndex     PCIe controller index

  @retval P2SB_PID    Returns PID for SBI Access
**/
P2SB_PID
PtlPcdGetPcieControllerSbiPid (
  IN UINT32  ControllerIndex
  );

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval P2SB_PID   Returns PID for SBI Access
**/
P2SB_PID
PtlPcdGetRpSbiPid (
  IN UINTN  RpIndex
  );

/**
  Return number of Root Ports for given PCIe Controller number

  @param[in] ControllerIndex     PCIe controller index

  @retval NumOfRp    Number of Root Ports for PCIe Controller
**/
UINT8
PtlPcdGetPcieControllerNumOfRootPorts (
  IN UINT32  ControllerIndex
  );

/**
  Return number of lanes for given PCIe Controller number

  @param[in] ControllerIndex     PCIe controller index

  @retval NumOfLanes    Number of lanes for PCIe Controller
**/
UINT8
PtlPcdGetPcieControllerNumOfLanes (
  IN UINT32  ControllerIndex
  );

/**
  Checks if root port is decoded by IOC.

  @param[in] RpIndex  Root port index.

  @return TRUE if root port is IOC decoded.
**/
BOOLEAN
PtlPcdIsRpIocDecoded (
  IN UINT32  RpIndex
  );

/**
  Update Config block Context for PCIE

  @param[in]  *pInst          Pointer to IP inst
  @param[in]  *PcieConfig     Rootport configuration
**/
VOID
PcieUpdateConfig (
  IN  IP_PCIE_INST                  *pInst,
  IN  PCH_PCIE_CONFIG               *PcieConfig
  );

/**
  Update Private Config for Ip Instance

  @param[in]  *pInst          Pointer to IP inst
**/

VOID
PcieUpdatePrivateConfig (
  IN IP_PCIE_INST       *pInst,
  IN  PCH_PCIE_CONFIG   *PcieConfig
  );

/**
  Update Private Config for PCIE IP Instance

  @param[in]  pInst    *pInst
**/
VOID
PcieUpdateControllerConfig (
  IN IP_PCIE_INST       *pInst
  );

/**
  Get the FID number of the PCIe root port.
  @param[in] RpIndex  Index of the root port
  @return FID number to use when sending SBI msg
**/
UINT16
GetRpSbiFid (
  IN UINTN  RpIndex
  );

/**
  Checks if root port is decoded by IOC.
  @param[in] RpIndex  Root port index.
  @return TRUE if root port is IOC decoded.
**/
BOOLEAN
IsRpIocDecoded (
  IN UINT32  RpIndex
  );

/**
  Checks whether RpIndex is located on IOE die.
  @param[in] RpIndex  Root port index.
  @return TRUE if root port is located on IOE die.
**/
BOOLEAN
IsRpOnIoeDie (
  IN UINT32  RpIndex
  );

/**
  Checks whether RpIndex is Controller.
  @param[in] RpIndex  Root port index.
  @return TRUE if RpIndex is Controller.
**/
BOOLEAN
IsController (
  IN UINT32                      RpIndex
  );

/**
  Get P2SB instance for PCIe root port

  @param[in]      RpIndex              Root port physical number. (0-based)
  @param[in, out] P2SbController       P2SB controller pointer

**/
VOID
PtlPcdGetPcieP2SbController (
  IN UINT32                      RpIndex,
  IN OUT P2SB_CONTROLLER         *P2SbController
  );

/**
  This function returns PID according to Root Port Number

  @param[in] RpIndex     Root Port Index (0-based)

  @retval P2SB_PID       Returns PID for SBI Access
**/
P2SB_PID
GetRootPortSbiPid (
  IN UINTN  RpIndex
  );

  /**
  Returns the PHY version of the PCIe root port.

  @param[in] RpIndex  Index of the root port in PCH

  @return Version of the PHY
**/
IP_PCIE_SIP_PHY_VERSION
PcieRpGetSipPhyVer (
  IN UINT32  RpIndex
  );
#endif
