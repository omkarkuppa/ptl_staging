/** @file
  eSPI handle definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _ESPI_HANDLE_H_
#define _ESPI_HANDLE_H_

#include <EspiConfig.h>
#include <Library/P2SbSidebandAccessLib.h>

typedef struct {
  BOOLEAN   PmSupport;
  BOOLEAN   EspiErrorReporting;
  /**
    Enable/disable option for Bus Master Enable (BME) programming<br>
    FALSE: Disable BME programming<br>
    TRUE: Enable BME programming
  **/
  BOOLEAN   BmeControl;
  /**
    Dynamic clock gating enable for the core clock used in ESPI Device IO logic.
  **/
  BOOLEAN   DynamicClockGatingEnableForEspiIoLogic;
} ESPI_PRIVATE_CONFIG;

typedef struct  {
  UINT64                PciCfgBase;
  ESPI_PRIVATE_CONFIG   EspiPrivateConfig;
  PCH_ESPI_CONFIG       *EspiConfig;
  REGISTER_ACCESS       *EspiPcrAccess;
} ESPI_HANDLE;

#endif
