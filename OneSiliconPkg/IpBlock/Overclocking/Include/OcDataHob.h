/** @file
  The GUID definition for OcDataHob

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
**/
#ifndef _OC_DATA_HOB_H_
#define _OC_DATA_HOB_H_

#include <Base.h>
#include <OcLimits.h>

#define OC_DATA_HOB_REVISION                 1

extern EFI_GUID gOcDataHobGuid;
#pragma pack (push,1)


typedef struct {
  /**
  Revision for the OC data hob.
  Any backwards compatible changes to this protocol will result in an update in the revision number.
  Major changes will require publication of a new protocol

  <b>Revision 1</b>:
   -  Initial version
  **/
  UINT8                  OverclockingEnabled;                  ///< Enable or disable Overclocking in BIOS.
  UINT8                  PerCoreRatioOverride;                 ///< Enable or disable Per Core PState OC supported.
  /**
  GranularityUnits for whole Package.
  **/
  UINT8                  GranularityUnits;
  UINT8                  MinP1Ratio;

  /**
  Use to get current Ia IccMax value.
  **/
  UINT16                 IaIccMax;
  /**
  Used to get current Gt IccMax value.
  **/
  UINT16                 GtIccMax;
  /**
  Used to get current Sa IccMax value.
  **/
  UINT16                 SaIccMax;
  UINT8                  MaxOcRatioLimit[MAX_OC_DOMAINS];
  /**
  Number of supported Voltage & Frequency Point Offset.
  **/
  UINT8                  VfPointCount[MAX_OC_DOMAINS];
  /**
  Array for the each selected VF Point to display the ratio.
  **/
  UINT8                  VfPointRatio[MAX_OC_DOMAINS][MAX_OC_VF_POINTS];
} OC_DATA_HOB;
#pragma pack (pop)
#endif
