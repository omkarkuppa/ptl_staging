/** @file
  Header file for Reference code Firmware Version Telemetry Info implementation.

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

@par Specification Reference:
**/
#ifndef _TELEMETRY_SI_FVI_H_
#define _TELEMETRY_SI_FVI_H_

#define DEFAULT_FVI_TELEMETRY_VERSION   0X000000FFFFFFFFFF
typedef enum {
  UcodeVer = 0,
  TxtVer
} TELEMETRY_CPU_FVI_INDEX;



typedef enum {
  PchCridOriginal = 0,
  PchCridNew,
  RaidVer,
} TELEMETRY_PCH_FVI_INDEX;

typedef enum {
  CridOriginal = 0,
  Cridnew,
  VbiosVer,
  IomFwVer,
  PhyVer,
  TbtFwVer,
  SamFwVer
} TELEMETRY_SA_FVI_INDEX;

#endif
