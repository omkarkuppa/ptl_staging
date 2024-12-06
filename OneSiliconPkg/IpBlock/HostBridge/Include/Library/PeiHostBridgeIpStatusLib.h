/** @file
  PeiHostBridgeIpStatusLib header file

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
#ifndef _HOST_BRIDGE_IP_STATUS_PTL_CDIE_H_
#define _HOST_BRIDGE_IP_STATUS_PTL_CDIE_H_

#include <HostBridgeConfig.h>

typedef enum {
  Graphics  = 2,
  Camarillo = 4,
  Ipu       = 5,
  Telemetry = 10,
  Npu       = 11,
  Iax       = 12,
  Vmd       = 14,
  Vtd,
  Adm
} UNCORE_IP;

typedef enum {
  Instance = 0
} UNCORE_IP_INSTANCE;

typedef enum {
  Disable = 0,
  Enable
} UNCORE_IP_STATUS;

//
// HostBridge Register
//
typedef enum {
  HostBridgeCfgReg  = 1,
  HostBridgeDeviceId,
  HostBridgeTolud,
  HostBridgeTsegMb,
  HostBridgeBdsm,
  HostBridgeBgsm,
  HostBridgeTouud,
  HostBridgeGgcCfgReg,
  HostBridgeTom,
  HostBridgeSafBar,
  HostBridgePam0CfgReg,
  HostBridgePam0CfgRegLateInit,
  HostBridgePam4CfgReg,
  HostBridgePam5CfgReg,
  HostBridgePam6CfgReg,
  HostBridgePamWpgWac,
  HostBridgePamWpgCp,
  HostBridgeDprCfg,
  HostBridgePavpc,
  HostBridgeDeven,
  HostBridgeNoHom
} HOSTBRIDGE_REGISTER;


typedef enum {
  MchBarCfgBaseLow  = 1,
  MchBarCfgBaseHigh,
  MchBarCfgBase
} HOSTBRIDGE_CFG_REGISTER_MODE;

typedef enum {
  HostBridgeToludLock = 1,
  HostBridgeToludBase,
  HostBridgeToludData
} HOSTBRIDGE_TOLUD_REGISTER_MODE;

typedef enum {
  HostBridgeTsegMbLock = 1,
  HostBridgeTsegMbBase,
  HostBridgeTsegMbData,
  HostBridgeTsegMbFullData
} HOSTBRIDGE_TSEGMB;

typedef enum {
  HostBridgeBdsmLock = 1,
  HostBridgeBdsmBase,
  HostBridgeBdsmFullData,
  HostBridgeBdsmData
} HOSTBRIDGE_BDSM_REGISTER_MODE;

typedef enum {
  HostBridgeBgsmLock = 1,
  HostBridgeBgsmBase,
  HostBridgeBgsmFullData,
  HostBridgeBgsmData
} HOSTBRIDGE_BGSM_REGISTER_MODE;

typedef enum {
  HostBridgeTouudBase = 1,
  HostBridgeTouudLock,
  HostBridgeTouudData
} HOSTBRIDGE_TOUUD_REGISTER_MODE;

typedef enum {
  HostBridgGgcLock = 1,
  HostBridgeGgcDsmSizeSelector,
  HostBridgeGgcGgsmSize,
  HostBridgeGgcMask,
  HostBridgeGgcFullData,
  HostBridgeGgcCfgSetIvdVamen,
  HostBridgeGgcCfgClearIvdVamen,
  HostBridgeGgcGmsWriteData,
  HostBridgeGgcClearGmsSetIvdVamen
} HOSTBRIDGE_GGC_REGISTER_MODE;

typedef enum {
  HostBridgeDeviceIdData
} HOSTBRIDGE_DEVICEID_REGISTER_MODE;

 typedef enum {
  HostBridgeTomInMb = 1,
  HostBridgeTomBase
} HOSTBRIDGE_TOM_REGISTER_MODE;

 typedef enum {
  SafBarCfgBaseLow  = 1,
  SafBarCfgBaseHigh,
  SafBarCfgBase
} HOSTBRIDGE_SAFBAR_REGISTER_MODE;

typedef enum {
  HostBridgePam0WriteData
} HOSTBRIDGE_PAM0_REGISTER_MODE;

typedef enum {
  HostBridgePam0LateInitWriteData
} HOSTBRIDGE_PAM4_LATE_INIT_REGISTER_MODE;

typedef enum {
  HostBridgePam4WriteData
} HOSTBRIDGE_PAM4_REGISTER_MODE;

typedef enum {
  HostBridgePam5WriteData
} HOSTBRIDGE_PAM5_REGISTER_MODE;

typedef enum {
  HostBridgePam6WriteData
} HOSTBRIDGE_PAM6_REGISTER_MODE;

typedef enum {
  HostBridgePamWpgWacWriteData
} HOSTBRIDGE_PAMWPGWAC_REGISTER_MODE;

typedef enum {
  HostBridgePamWpgCpWriteData
} HOSTBRIDGE_PAMWPGCP_REGISTER_MODE;

typedef enum {
  HostBridgeDprSetlock = 1,
  HostBridgeDprClearlock
} HOSTBRIDGE_DPR_REGISTER_MODE;

typedef enum {
  HostBridgeNoHomBase  = 1,
  HostBridgeNoHomLimit
} HOSTBRIDGE_NOHOM_REGISTER_MODE;

/**
  This function is to update DEVEN Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE
  @param[in] Status   - Instance of UNCORE_IP_STATUS

**/
VOID
EFIAPI
UncoreDevEnWrite (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance,
  IN UNCORE_IP_STATUS    Status
  );

/**
  This function is to Read Cap ID Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE

  @retval FALSE       - Uncore Ip is fused off
  @retval TRUE        - Uncore Ip is supported by Sillicon

**/
BOOLEAN
EFIAPI
UncoreCapIdRead (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance
  );

/**
  This function is to Read DEVEN Register for Uncore IPs.

  @param[in] Ip       - Instance of UNCORE_IP
  @param[in] Instance - Instance of UNCORE_IP_INSTANCE

  @retval FALSE       - Uncore Ip is Disable by DEVEN
  @retval TRUE        - Uncore Ip is Enable by DEVEN

**/
BOOLEAN
EFIAPI
UncoreDevEnRead (
  IN UNCORE_IP           Ip,
  IN UNCORE_IP_INSTANCE  Instance
  );

/**
  Program PcieFunc register for Function Swapping when PEG10 and PEG60 are not enabled, Here offset is 0x7C

  @param[in] RpIndex     - Root Port Index (0-based)
  @param[in] NewFunction - New function number
**/
VOID
EFIAPI
IocSetPcieFunction (
  IN UINT32  RpIndex,
  IN UINT32  NewFunction
  );

/**
  This function is to read the hostbrige register.

  @param[in] RegiterName   - Instance of HostBridgeRester
  @param[in] DataRequested - Instance of Mode of the Register

  @retval UINT64           - Register Details based on Mode

**/
UINT64
EFIAPI
GetHostBridgeRegisterData (
  IN HOSTBRIDGE_REGISTER  RegiterName,
  IN UINT16               DataRequested
  );

/**
  This function is to write to hostbrige register.

  @param[in] RegiterName   - Instance of HostBridgeRester
  @param[in] RegisterField - Instance of Mode of the Register
  @param[in] AdditionalData - when API need's to write some custom data, this parameter will be used

**/
VOID
EFIAPI
SetHostBridgeRegisterData (
  IN HOSTBRIDGE_REGISTER  RegiterName,
  IN UINT16               RegisterField,
  IN UINT64               AdditionalData
  );

/**
  This function will update Graphics register.

  @param[in] GraphicsGttSize    - GraphicsGttSize
  @param[in] GraphicsStolenSize - GraphicsStolenSize

  @retval UINT16                - Register Data

**/
UINT16
EFIAPI
UpdateGraphicsController (
  IN  UINT32                       GraphicsGttSize,
  IN  UINT32                       GraphicsStolenSize
  );

/**
  Disables the root port using DEV_EN bit.

  @param[in] RpIndex            Root Port Number

**/
VOID
EFIAPI
IocDisablePcieRootPort (
  IN UINT32   RpIndex
  );

/**
  Check if root port is enabled.

  @param[in] RpIndex            Root Port Number
**/
BOOLEAN
EFIAPI
IocIsPcieRootPortEnabled (
  IN UINT32   RpIndex
  );

/**
  This function wtites TCSS DEVEN Register.

  @param[in]  TcssDevEn  - Maps to bits in TCSS_DEVEN_0_0_0_MCHBAR_IMPH

**/
VOID
HostBridgeSetTcssDeven (
  IN UINT32 TcssDevEn
  );

/**
  This function is to read the TCSS DEVEN register

  @retval UINT32   - TCSS DEVEN Register

**/
UINT32
HostBridgeGetTcssDeven (
  VOID
  );

/**
  Sets function mapping for TCSS PCIe root ports
  Writes function ,mapping into PCIEFUNCMAP_HOSTBRIDGE_CFG_REG

  @param[in]  PcieFuncMap - TCSS PCIe root ports mapping

**/
VOID
HostBridgeSetTcssPcieRpFuncMap (
  IN UINT32  PcieFuncMap
  );

#endif
