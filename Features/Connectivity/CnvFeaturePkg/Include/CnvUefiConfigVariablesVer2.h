/** @file
  Header file to define internal macro, structures for CNV variable implementation

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef __CNV_UEFI_VARIABLES_VER2_H__
#define __CNV_UEFI_VARIABLES_VER2_H__

#include <CnvVariablesHeaders.h>
#include <CnvVariablesCommon.h>
#include <CnvVariablesWlan.h>
#include <CnvVariablesBt.h>

#pragma pack(push, 1)
//
// CNV Configuration
//
typedef struct {
  CNV_UEFI_VAR_WCSC             Wcsc;             // Wireless CNV Serial Time Encoded Protocol(STEP) Characterization
  CNV_UEFI_VAR_SADS             Sads;             // Switched Antenna Diversity Selection
  CNV_UEFI_VAR_ECKV             Eckv;             // External Clock Valid
  ICNV_WWAN_SKU_REV_1           WwanCoEx;         // WWAN CoEx support
  CNV_UEFI_VAR_SPLC             Splc;             // Static Power Limit Configuration
  CNV_UEFI_VAR_WRDD             Wrdd;             // Wireless Regulatory Domain Description
  CNV_UEFI_VAR_WRDS_WIFI_SAR    Wrds;             // Wireless Regulatory Domain SAR
  CNV_UEFI_VAR_EWRD             Ewrd;             // Extended Wireless Regulatory Domain SAR
  CNV_UEFI_VAR_WGDS             Wgds;             // Wireless Geo Delta Settings
  CNV_UEFI_VAR_PPAG             Ppag;             // Per Platform Antenna Gain
  CNV_UEFI_VAR_WTAS             Wtas;             // WiFi Time Average SAR
  WLAN_GENERAL_CONFIG_SET       WlanGeneralCfg;   // WiFi General configuration simlar to legacy DSM
  WLAN_OEM_PNVM                 WlanOemPnvm;      // WiFi OEM PNVM
  WLAN_TX_REDUCED_POWER         WlanReducedPower; // WiFi Tx Power Reduction
  CNV_UEFI_WLAN_SGOM_DATA       Sgom;             // SAR GEO Offset Mapping
  CNV_UEFI_VAR_BTLC             Btlc;             // Bluetooth Tile Configuration
 #if (FixedPcdGet16 (PcdCnvUefiVarVersion) == 4)
  CNV_UEFI_VAR_AOLD             Aold;            // Bluetooth Audio Offload
 #endif
  WLAN_MPCC_CONFIG_SET          Mpcc;
} UEFI_CNV_CONFIG;
#pragma pack(pop)

#endif
