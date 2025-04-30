/** @file
  PCI Device ID PTL PCD eSPI

  Conventions:

  - Prefixes:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register sizes
    Definitions beginning with "N_" are the bit position

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
#ifndef _PTL_PCD_ESPI_DEVICE_IDS_H_
#define _PTL_PCD_ESPI_DEVICE_IDS_H_

#define V_ESPI_CFG_DID_PTL_PCD_H                    0xE300
#define V_ESPI_CFG_DID_PTL_PCD_P                    0xE400

//
// PTL PCD P LPC Device IDs
//
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_0         0xE400          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_1         0xE401          ///< LPC/eSPI Controller P SuperSKU
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_2         0xE402          ///< LPC/eSPI Controller P Premiun
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_3         0xE403          ///< LPC/eSPI Controller P Base/Mainstream
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_4         0xE404          ///< LPC/eSPI Controller NEX INDU SuperSKU
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_5         0xE405          ///< LPC/eSPI Controller NEX INDU Premium
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_6         0xE406          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_7         0xE407          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_8         0xE408          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_9         0xE409          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_10        0xE40A          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_11        0xE40B          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_12        0xE40C          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_13        0xE40D          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_14        0xE40E          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_15        0xE40F          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_16        0xE410          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_17        0xE411          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_18        0xE412          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_19        0xE413          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_20        0xE414          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_21        0xE415          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_22        0xE416          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_23        0xE417          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_24        0xE418          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_25        0xE419          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_26        0xE41A          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_27        0xE41B          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_28        0xE41C          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_29        0xE41D          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_30        0xE41E          ///< LPC/eSPI Controller
#define V_PTL_PCD_P_ESPI_CFG_DEVICE_ID_MB_31        0xE41F          ///< LPC/eSPI Controller

//
// PTL PCD H LPC Device IDs
//
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_0         0xE300          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_1         0xE301          ///< LPC/eSPI Controller P SuperSKU
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_2         0xE302          ///< LPC/eSPI Controller P Premiun
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_3         0xE303          ///< LPC/eSPI Controller P Base/Mainstream
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_4         0xE304          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_5         0xE305          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_6         0xE306          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_7         0xE307          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_8         0xE308          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_9         0xE309          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_10        0xE30A          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_11        0xE30B          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_12        0xE30C          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_13        0xE30D          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_14        0xE30E          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_15        0xE30F          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_16        0xE310          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_17        0xE311          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_18        0xE312          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_19        0xE313          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_20        0xE314          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_21        0xE315          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_22        0xE316          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_23        0xE317          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_24        0xE318          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_25        0xE319          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_26        0xE31A          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_27        0xE31B          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_28        0xE31C          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_29        0xE31D          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_30        0xE31E          ///< LPC/eSPI Controller
#define V_PTL_PCD_H_ESPI_CFG_DEVICE_ID_MB_31        0xE31F          ///< LPC/eSPI Controller



#endif
