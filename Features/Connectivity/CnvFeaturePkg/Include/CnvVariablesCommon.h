/** @file
  Declaration of Intel Connectivity Common Variables Headers , Structures and guids for WLAN, BT.

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

  @par Specification Reference: 559910_Intel_Connectivity_Platforms_BIOS_Guidelines_Rev6_6.pdf

**/

#ifndef __CNV_VARIABLES_COMMON_H__
#define __CNV_VARIABLES_COMMON_H__
//
// Include UEFI variables Revision, Headers and Guids definition.
//
#include <CnvVariablesHeaders.h>

#pragma pack(push, 1)

//
// Definition for Wireless CNV Serial Time Encoded Protocol Characterization (WCSC)
//
#define CNV_UEFI_WCSC_VAR_NAME        L"UefiCnvCommonSTEP"
#define CNV_UEFI_WCSC_VAR_REVISION_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
  <b>Revision 1</b>:  - Migration from ACPI to UEFI Variable.
**/

#define CNV_STEP_MODE_DISABLED  0       // Legacy DPHY/CNVIO (Non-Step)
#define CNV_STEP_MODE_GEN_1     1       // Activate STEP GEN 1 Activated
typedef struct {
  UINT8    StepMode;       ///< Define The STEP type:
                           ///< 0, CNV_STEP_MODE_DISABLED the Default on MTL, means disabled worked over legacy DPHY / CNVIO
                           ///< 1 - CNV_STEP_MODE_GEN_1,  Enable STEP GEN 1
  UINT8    CNVi_EQChannel; ///< The Equalizer channel Value that shall set on the PCH
  UINT8    CNVr_EQChannel; ///< The Equalizer channel Value that shall set on the CRF
  UINT8    Radio_1;        ///< Definitions to Radio1
  UINT8    Radio_2;        ///< Definitions to Radio2
} CNV_STEP_CHARACTERIZATION;

typedef struct {
  CNV_UEFI_VAR_HEADER          Header;              ///< Set CNV_UEFI_COMMON_STEP_REV
  CNV_STEP_CHARACTERIZATION    StepCharacterization;
} CNV_UEFI_VAR_WCSC;

//
// Definition for Switched Antenna Diversity Selection (SADS)
//
#define CNV_UEFI_SADS_VAR_NAME    L"UefiCnvCommonSADS"
#define CNV_UEFI_SADS_REVISION_0  0

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
**/
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;          ///< Revision
  UINT32                 AntDiversitySel; ///< 0:Antenna1, 1:Antenna2, 2:Diversity, 3:Diversity Antenna1, 4:Diversity Antenna2
} CNV_UEFI_VAR_SADS;

//
// Definition for External Clock Valid (ECKV)
//
#define CNV_UEFI_ECKV_VAR_NAME    L"UefiCnvCommonECKV"
#define CNV_UEFI_ECKV_REVISION_0  0

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version.
**/
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;                 ///< Revision
  UINT32                 CnvExternal32kHzClock;  ///< External 32kHz clock Valid/Not Valid
} CNV_UEFI_VAR_ECKV;

//
// Definition for WWAN CoEx support
//
#define CNV_UEFI_WWAN_CO_EX_VAR_NAME    L"IntelUefiCnvWwanSkuSupport"
#define CNV_UEFI_WWAN_CO_EX_REVISION_1  1

/**
  Update below history table to indicate the changes\n
  <b>Revision 0</b>:  - Initial version to support WiFi.
  <b>Revision 1</b>:  - Support both BT and WiFi.
**/
//
// The WiFi TX Limitation in case of WWAN CoEx, the Values dB shall set as Integers,
// in case of WWAN mode disabled or invalid values(or zero) the WiFi ignore those parameters.
//
typedef struct {
  UINT8    LowBand;   // Isolate the Wifi Low Band (2.4 GHz), TX Power from 15 dB to 40 dB (The value as Integer). By Default, shall be set to 15. In case that remains Zero, Intel WiFi Driver shall set the default value 15dB.
  UINT8    HighBand;  // Isolate the Wifi Higher Bands From 5.2 GHz to 7 GHz, TX Power from 15 dB to 40 dB (The value as Integer). By Default, shall be set to 20. In case that remains Zero, Intel WiFi Driver shall set the default value 20dB.
} ANT_ISOLATION_REV;
//
// IntelUefiCnvWwanSkuSupport - Main structure
//
typedef struct {
  CNV_UEFI_VAR_HEADER    Header;              ///< Set the revision to CNV_UEFI_WWAN_CO_EX_REV
  UINT32                 WwanMode;            ///< see Intel BIOS guideline to specific bits.
  ANT_ISOLATION_REV      AntIsolation;        ///< Antenna Isolation between WLAN and WWAN per Band ANT_ISOLATION_REV_1
} ICNV_WWAN_SKU_REV_1;

//
// Definition for Drive Strength BRI Rsp (DSBR)
//
#define CNV_UEFI_DSBR_VAR_NAME        L"UefiCnvCommonDSBR"
#define CNV_UEFI_DSBR_VAR_REVISION    0

typedef struct {
  CNV_UEFI_VAR_HEADER  Header;
  UINT32               CnvDriveStrengthBriRsp;  // Defines the drive strength BRI response.
} CNV_UEFI_VAR_DSBR;

#pragma pack(pop)

#endif //__CNV_VARIABLES_COMMON_H__
