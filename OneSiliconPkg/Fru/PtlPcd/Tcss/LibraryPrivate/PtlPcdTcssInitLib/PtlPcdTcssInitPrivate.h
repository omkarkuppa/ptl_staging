/** @file
  Private header file for PTL TCSS initialization library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _PTL_SOC_TCSS_INIT_PRIVATE_
#define _PTL_SOC_TCSS_INIT_PRIVATE_

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/BaseMemoryLib.h>

#include <RegisterAccess.h>
#include <IpWrapperCntxtInfoClient.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <Register/ItbtPcieRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/PmcPrivateLib.h>
#include <Library/TcssPmcLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/SocInfoLib.h>
#include <ConfigBlock.h>
#include <ConfigBlock/SiConfig.h>
#include <TcssInfo.h>
#include <Register/CpuUsbRegs.h>
#include <Register/HostDmaRegs.h>
#include <HostBridgeConfig.h>
#include <TcssPeiPreMemConfig.h>
#include <TcssPeiConfig.h>
#include <PeiITbtConfig.h>
#include <Library/PcdTcssInitLib.h>
#include <Defines/PchReservedResources.h>
#include <ConfigBlock/SiConfig.h>
#include <TcssLib.h>
#include <IpIom.h>
#include <IpUsb4.h>
#include <IpTcPcieRp.h>
#include <TcssDataHob.h>

#ifndef ENVIRONMENT_DEFINED
#define ENVIRONMENT_DEFINED
typedef enum {
  SiliconEnv = 0,
  SimicsEnv  = 1,
  HfpgaEnv   = 2
} ENVIRONMENT;
#endif //ENVIRONMENT_DEFINED

/**
  Initializes TCSS instance structure and Sub-IPs instance structures
  Note:
  Caller is responsible for the memory allocation
 **/
UINT32
PtlTcssInstInit (
  TCSS_INST               *TcssInst,
  TCSS_CALLBACKS          *TcssCallbacks,
  TCSS_CONFIG             *TcssOptions,
  TCSS_INFO               *TcssInfo,
  IP_USB3_INST            *Usb3Inst,
  IP_WR_REG_INFO          *Usb3RegInfoPciPrim,
  IP_WR_REG_INFO          *Usb3RegInfoMmio,
  IP_WR_REG_INFO          *Usb3RegInfoPcr,
  IP_WR_REG_INFO          *Usb3RegInfoExi,
  P2SB_CONTROLLER         *P2SbController,
  IP_USB4_INST            *Usb4Inst,
  IP_USB4_DMA             *Usb4Dma,
  IP_WR_REG_INFO          *Usb4DmaRegInfoPciPrim,
  IP_IOM_INST             *IomInst,
  IP_WR_REG_INFO          *IomRegInfoPcr,
  IP_TC_PCIE_RP_INST      *PcieRpInst,
  IP_PCIE_ROOT_PORT       *PcieRpPorts,
  IP_WR_REG_INFO          *PcieRpRegInfoPciPrim,
  IP_WR_REG_INFO          *PcieRpRegInfoPcr,
  IP_PCIE_CALLBACKS       *PcieCallbacks,
  TCSS_PEI_PREMEM_CONFIG  *TcssPeiPreMemConfig,
  TCSS_PEI_CONFIG         *TcssPeiConfig,
  PEI_ITBT_CONFIG         *PeiITbtConfig,
  HOST_BRIDGE_PREMEM_CONFIG   *HostBridgePreMemConfig
  );

VOID
PtlSetSvidSsid (
  VOID
  );

/**
  Get Device Enables form SOC IO Cache service

  @param[out]  DevEnCfg  Pointer to Device Enables

  @retval     Status
**/
UINT32
PtlIpIocTcssGetDevEn (
  VOID
  );

/**
  PCIe Rp downstream port configuration

  @param[in]  FuncMap PCIe RP function mapping
**/
VOID
PtlTcPcieRpDownStreamConfig (
  UINT8 Index
  );

/**
  Enables PCIe PME Events for TCSS Root Ports

**/
VOID
PtlEnablePciePmeEvents (
  VOID
  );

#endif // _PTL_SOC_TCSS_INIT_PRIVATE_
