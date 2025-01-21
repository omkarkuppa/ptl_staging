/** @file
  Header file for ITBT PCIE IP specific defines.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _ITBT_PCIE_REGS_PTL_PCD_H_
#define _ITBT_PCIE_REGS_PTL_PCD_H_

#define ITBT_PCIE_BUS_NUM                 0x0 // Bus      #0
#define ITBT_PCIE_DEV_NUM                 0x7 // Device   #7
#define ITBT_PCIE_0_DEFAULT_FUN_NUM       0x0 // Function #0
#define ITBT_PCIE_1_DEFAULT_FUN_NUM       0x1 // Function #1
#define ITBT_PCIE_2_DEFAULT_FUN_NUM       0x2 // Function #2
#define ITBT_PCIE_3_DEFAULT_FUN_NUM       0x3 // Function #3

//
// Flattening Portal Bridge Capability Header (CAPID:0015h)
//
#define V_PCIE_FPBCAP_CID                        0x0015
#define V_FPBCAPR_FPBRIDVECSIZES                 0x0
#define V_FPBCAPR_FPBMEMLVECSIZES                0x1
#define V_FPBCAPR_FPBMEMHVECSIZES                0x0

#define V_TCSS_EXCAP_NCO_LISTEND                 0
#define V_TCSS_SVCAP_CID                         0x0D
#define V_TCSS_MID_CID                           0x05
#define V_TCSS_XCAP_CV                           0x02
#define V_TCSS_CLIST_CID                         0x10
#define V_TCSS_PMCAP_CID                         0x01

#define V_TCSS_DLFECH_CV                         0x1
#define V_TCSS_DLFECH_CID                        0x25
#define V_TCSS_CFG_SPEECH_CV                     0x1
#define V_TCSS_CFG_SPEECH_CID                    0x19
#define V_TCSS_CFG_DPCECH_CV                     0x1
#define V_TCSS_CFG_DPCECH_CID                    0x19
#define V_TCSS_CFG_PTMECH_CV                     0x1
#define V_TCSS_CFG_PTMECH_CID                    0x1F
#define V_TCSS_CFG_ACSECH_CV                     0x1
#define V_TCSS_CFG_ACSECH_CID                    0x0D
#define V_TCSS_CFG_AECH_CV                       0x1
#define V_TCSS_CFG_AECH_CID                      0x01

//
//Mask to Get the first enabled iTBT PCIe port number.
//
#define TCSS_DEVEN_IMPH_IOC_MCHBAR_PCIE_EN_MASK 0x00ff

#endif // _ITBT_PCIE_REGS_H_
