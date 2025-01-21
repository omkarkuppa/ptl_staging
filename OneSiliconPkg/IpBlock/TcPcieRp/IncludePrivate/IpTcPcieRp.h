/** @file
  IP API for PCIE_RP

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

  @par Specification

**/

#ifndef __IP_TC_PCIE_RP_H__
#define __IP_TC_PCIE_RP_H__

#include <IpCsi.h>

#define VERSION_HW_TC_PCIE_RP              0x00010001
#define VERSION_HW_CUSTOM_TC_PCIE_RP       0x00000001
#define VERSION_FW_TC_PCIE_RP              0x00000003
#define VERSION_IPFWINTF_TC_PCIE_RP        0x05230102
#define VERSION_IPFWINTFEXT_TC_PCIE_RP     0x00000002
#define VERSION_UNKNOWN_TC_PCIE_RP         0

#ifndef MAX_ITBT_PCIE_PORT
#define MAX_ITBT_PCIE_PORT 4
#endif  //MAX_ITBT_PCIE_PORT

typedef struct _IP_TC_PCIE_RP_INST IP_TC_PCIE_RP_INST;

/**
  PCIe Rp downstream port configuration

  @param[in]  FuncMap PCIe RP function mapping
**/
typedef
void
(*PCIE_RP_DOWNSTREAM_PORT_CONFIG) (
  UINT8 Index
  );

typedef struct {
    // PCIe Rp downstream port configuration
  // Traverse all devices connected to the port and programs
  // consistent setings in entire PCIe hierarchy.
  // The programming must be executed in proper order and moment.
  PCIE_RP_DOWNSTREAM_PORT_CONFIG  PcieRpDownStreamConfig;
} IP_PCIE_CALLBACKS;

#ifndef _TCSS_VER
#define _TCSS_VER
typedef enum {
  TcssOld   = 0,
  TcssNew   = 1,
  TcssNewH  = 2
} TCSS_VER;
#endif // _TCSS_VER

typedef union {
  UINT32      DevEn;            ///< Maps to bits in TCSS DEVEN register
  struct {
    UINT32    TcssPcieRpEn : 8;
    UINT32    TcssXhciEn   : 1;
    UINT32    Rsrvd        : 1;
    UINT32    Usb4HrEn     : 4;
    UINT32 : 18;
  } DevEnBit;
} IP_PCIE_DEVEN_CONFIG;

typedef struct {
   UINT32          PtmEnabled;
   UINT32          AdvancedErrorReporting;
   UINT32          SlotNumber;
   UINT32          AcsEnabled;
   UINT8           LtrEnable;
   UINT32          LtrMaxSnoopLatency;
   UINT32          LtrMaxNoSnoopLatency;
   UINT32          SnoopLatencyOverrideMode;
   UINT32          SnoopLatencyOverrideMultiplier;
   UINT32          SnoopLatencyOverrideValue;
   UINT32          NonSnoopLatencyOverrideMode;
   UINT32          NonSnoopLatencyOverrideMultiplier;
   UINT32          NonSnoopLatencyOverrideValue;
   UINT32          ForceLtrOverride;
   UINT32          LtrConfigLock;
} IP_TC_PCIE_RP_CONFIG;

typedef struct _IP_PCIE_ROOT_PORT IP_PCIE_ROOT_PORT;
struct _IP_PCIE_ROOT_PORT {
  IP_WR_REG_CNTXT       PcrAccess;
  IP_WR_REG_CNTXT       PrimCfgAccess;
  IP_TC_PCIE_RP_CONFIG  PcieRpConfig;
  IP_PCIE_ROOT_PORT     *Next;
};

struct _IP_TC_PCIE_RP_INST {
  IP_CSI_INST_PREFIX    Prefix;

  // IP Wrapper Register Contexts
  IP_PCIE_ROOT_PORT     *PciRootPortList;

  // IP Wrapper Print Contexts
  IP_WR_PRINT_CNTXT     PrintError;
  IP_WR_PRINT_CNTXT     PrintWarning;
  IP_WR_PRINT_CNTXT     PrintLevel1;

  IP_PCIE_DEVEN_CONFIG    DevEn;
  UINT32                  PcieFuncMap;
  UINT32                  BusMin;
  UINT32                  BusMax;
  IP_PCIE_CALLBACKS       *Callbacks;
  TCSS_VER                TcssVersion;
};

/**
  Instance structure initialization for this version of IP
  For current version of TC PCIe RP there is no requirements for this function.
  All data in Instance Structure is initialized by the caller.
  The function just returns success status.

  @param[in] pInst  TC PCIe RP Instance Structure pointer

  @retval     IpCsiStsSuccess on success
  @retval     !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpTcPcieRpInstInit (
  IP_TC_PCIE_RP_INST    *pInst
  );

/**
  TypeC PCIe Root Ports Initialization procedure.
  The function shall be the first API function called after IpIomInstInit.
  This is a good place to fix bad register defaults or work around bugs.

  @param[in]  pInst TC PCIe RP Instance Structure pointer

  @retval         IpCsiStsSuccess on success
  @retval         !IpCsiStsSuccess on failure
**/
IP_CSI_STATUS
IpTcPcieRpIpInit (
  IP_TC_PCIE_RP_INST    *pInst
  );

/**
  Locks PCIe Root Port configuration.

  @param[in]    PcieRpInst        PCIE Root Port instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         PCIe RP programming error
**/
IP_CSI_STATUS
IpTcPcieRpLockConfiguration (
  IP_TC_PCIE_RP_INST    *pInst
  );

/**
  This interface will return a version value that corresponds to the VerId provided and
  is formatted per the CSI specification.
  - CSI Defined Interface

  @param[in]      pInst      A pointer to the IP instance to be used.
  @param[in]      VerId      An enum value defined in the CSI specification of type IP_CSI_VER_ID.
  @retval         0          The VerId value is not recognized and a version could not be determined.
  @retval         Non-Zero   A version value corresponding to the VerId provided, and formatted per the CSI.
**/
UINT32
IpTcPcieRpGetVersion (
  IP_TC_PCIE_RP_INST    *pInst,
  IP_CSI_VER_ID         VerId
  );

/**
  Programs PCIe Root Port function mapping.
  Returns programmed mapping to be used by SOC related programming.

  @param[in]    pInst        PCIE Root Port instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       programming completed without error
    - IpCsiStsError:         PCIe RP programming error
**/
IP_CSI_STATUS
IpTcPcieRpSetFunctionMap (
  IP_TC_PCIE_RP_INST    *pInst
  );

/**
  Returns PCIe Toot Port Function mapping

  @param[in]    pInst       PCIE RP instance structure
  @param[out]   Mapping     Pointer to mapping variable

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfuly
    - IpCsiStsError:         Error during IOM reading
**/
IP_CSI_STATUS
IpTcPcieRpGetFunctionMap (
  IP_TC_PCIE_RP_INST  *pInst,
  UINT32              *Mapping
  );

/**
  Clears VGA and VGA-16-bit decodes of Bridge control register
  for all TC PCIe Root Ports

  @param[in]    pInst       PCIE RP instance structure

  @retval   IP_CSI_STATUS
    - IpCsiStsSuccess:       Executed successfuly
    - IpCsiStsError:         PCIe Root Port disabled
**/
IP_CSI_STATUS
IpTcPcieRpClearVgaRegisters (
  IP_TC_PCIE_RP_INST  *pInst
  );

#endif //__IP_TC_PCIE_RP_H__
