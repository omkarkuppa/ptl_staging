/** @file
  Header file for PEI SI SSID LIB.

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
#ifndef _PEI_SI_SSID_LIB_H_
#define _PEI_SI_SSID_LIB_H_

#include <Ppi/SiPolicy.h>
#include <ConfigBlock.h>
#include <ConfigBlock/SiConfig.h>
#include <ConfigBlock/Thc/ThcConfig.h>

/**
  Silicon to override default SVID and SSID

  @param[in]      PciDevNum       Pci device number
  @param[in]      PciFuncNum      Pci function number
  @param[in,out]  Svid            Svid value
  @param[in,out]  Ssid            Ssid value

  @retval         TRUE            Silicon overrides the SSID
  @retval         FALSE           Silicon doesn't override the SSID
**/
typedef
BOOLEAN
(*SI_SSID_OVERRIDE_FUNC) (
  UINT32                 PciDevNum,
  UINT32                 PciFuncNum,
  UINT16                 *Svid,
  UINT16                 *Ssid
  );

/**
  Program devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] SiSsidOverrideFunc   Silicon SSID override function
  @param[in] Segment              Segment number required for PciCfgBase evaluation
  @param[in] Bus                  Bus number required for PciCfgBase evaluation
**/
VOID
SiProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SI_SSID_OVERRIDE_FUNC     SiSsidOverrideFunc,
  IN  UINT8                     Segment,
  IN  UINT8                     Bus
  );

/**
  Program devices Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID) for specific SBDF device.

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] SiSsidOverrideFunc   Silicon SSID override function
  @param[in] Segment              Segment number required for PciCfgBase
  @param[in] Bus                  Bus number required for PciCfgBase
  @param[in] DevNum               Device number required for PciCfgBase
  @param[in] FuncNum              Function number required for PciCfgBase
**/
VOID
SiProgramSsidForSbdf (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SI_SSID_OVERRIDE_FUNC     SiSsidOverrideFunc,
  IN  UINT8                     Segment,
  IN  UINT8                     Bus,
  IN  UINT32                    DevNum,
  IN  UINT32                    FuncNum
  );

/**
  Program TCSS Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] TcssSsidTablePtr     Tcss SSID policy table
  @param[in] NumberOfTcssTable    The number of SSID Items
**/
VOID
SiTcssProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SVID_SID_SKIP_TABLE       *TcssSsidTablePtr,
  IN  UINT16                    NumberOfTcssTable
  );

/**
  Program Pcie Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).

  @param[in] SiConfig             The SI config information
**/
VOID
SiPcieProgramSsid (
  IN  SI_CONFIG                 *SiConfig
  );

/**
  Program THC Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID).
  @param[in] SiPolicy             The SI Policy PPI instance
  @param[in] ThcSsidTablePtr      Thc SSID policy table
  @param[in] Index                Thc Port index
  @param[in] CustomizedSvid       Customized Sub System Vendor ID
  @param[in] CustomizedSsid       Customized Sub System ID
**/
VOID
SiThcProgramSsid (
  IN  SI_POLICY_PPI             *SiPolicy,
  IN  SVID_SID_SKIP_TABLE       *ThcSsidTablePtr,
  IN  UINT8                     Index,
  IN  UINT16                    CustomizedSvid,
  IN  UINT16                    CustomizedSsid
  );

/**
  Get Subsystem Vendor Identifier (SVID) and Subsystem Identifier (SID)
  for requested Device and function number

  @param[in]   SiPolicy     The SI Policy PPI instance
  @param[in]   PciDevNum    PCI device number
  @param[in]   PciFuncNum   PCI function number
  @param[out]  Svid         Svid value
  @param[out]  Ssid         Ssid value   
**/
VOID
SiGetSsidSvid (
  IN  SI_POLICY_PPI      *SiPolicy,
  IN  UINT32             PciDevNum,
  IN  UINT32             PciFuncNum,
  OUT UINT16             *Svid,
  OUT UINT16             *Ssid
  );

#endif
