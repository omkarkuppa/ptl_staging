/** @file
  Header file which contains Voltage regulator topology data and overrides for domains like SA, IA, Ring, GT, Fivr.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#ifndef _VOLTAGE_REGULATOR_DOMAINS_PTL_CDIE_H_
#define _VOLTAGE_REGULATOR_DOMAINS_PTL_CDIE_H_

#include <Register/CpuGenRegs.h>  //CPU_IDENTIFIER structure comes from this header. so we need this include

///
///  VR Domain Definitions
///
#define CPU_VR_DOMAIN_IA           0x0
#define CPU_VR_DOMAIN_GT           0x1
#define CPU_VR_DOMAIN_SA           0x2
#define CPU_VR_DOMAIN_ATOM         0x3

///
/// VR Topology Data structure containing the address and en/dis information
/// for all SVID VR domains
///
typedef union VR_TOPOLOGY_DATA {
  UINT32 VrTopology;              ///< All bit fields as a 32-bit value.
  ///
  /// Individual bit fields.
  ///
  struct {
    UINT32 Reserved             : 5;  ///< Reserved
    UINT32 VrIaAddress          : 4;  ///< IA VR address
    UINT32 VrIaSvidType         : 1;  ///< IA VR Type. 1 - No SVID VR, 0 - SVID VR
    UINT32 VrGtAddress          : 4;  ///< GT VR address
    UINT32 VrGtSvidType         : 1;  ///< GT VR Type. 1 - No SVID VR, 0 - SVID VR
    UINT32 VrAtomAddress        : 4;  ///< ATOM VR address
    UINT32 VrAtomSvidType       : 1;  ///< ATOM VR Type. 1 - No SVID VR, 0 - SVID VR
    UINT32 Reserved1            : 5;  ///< Reserved
    UINT32 VrSaAddress          : 4;  ///< SA VR address
    UINT32 VrSaSvidType         : 1;  ///< SA VR Type. 1 - No SVID VR, 0 - SVID VR
    UINT32 SetIaVrVid           : 1;  ///< Set IA VR OFFSET register to allow higher than 1.52V
    UINT32 PsysDisable          : 1;  ///< Imon Enable. 1 - Disable, 0 - Enable
  } Fields;
} VR_TOPOLOGY_DATA;

//
//RFI / EMI Register
//
typedef union {
  UINT32 Data;
  struct {
    UINT32 SscFreqSprd           : 5;   /// Set PLL spectrum spread percentage. 0x0: Disable. u3.2 value from 0% - 7.75%. Default is 0x6 (1.5%)
    UINT32 SscMode               : 1;   /// 0: Down spread. 1: Center spread.
    UINT32 SpreadSpecControlLock : 1;   /// 0: Future writes to be honored. 1: future writes are ignored.
    UINT32 RfiEnable             : 1;   /// 0: Disable RFI freq hopping. 1: Enable RFI freq hopping.
    UINT32 RfiFreqSelect         : 2;   /// Set PLL clock freq in Mhz. Default is 2227.2MHZ(0x08B3) Mhz.
    UINT32 Reserved              : 22;
  } Field;
} RFI_EMI_CONTROL_0_0_0_MCHBAR;

#endif // _VOLTAGE_REGULATOR_DOMAINS_PTL_CDIE_H_
