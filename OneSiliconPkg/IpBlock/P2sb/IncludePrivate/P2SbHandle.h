/** @file
  Header file for P2SB handle

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

@par Specification
**/
#ifndef _P2SB_HANDLE_H_
#define _P2SB_HANDLE_H_

#include <ConfigBlock.h>
#include <P2sbConfig.h>
#include <P2SbController.h>
#include <RegisterAccess.h>

/**
  Internal P2SB policy options
**/
typedef struct {
  /**
    Enable HSLE Workarounds
   **/
  BOOLEAN               HsleWorkaround;
  /**
    Set to enable low latency of legacy IO.
    Some systems require lower IO latency irrespective of power.
    This is a tradeoff between power and IO latency.
    @note: Once this is enabled, DmiAspm, Pcie DmiAspm in SystemAgent
    and ITSS Clock Gating are forced to disabled.
    <b>FALSE: Disable</b>, TRUE: Enable
  **/
  BOOLEAN               LegacyIoLowLatency;
  /**
    On server PCHs "Hardware Autonomous Enable" has to be always disabled.
    This flag allow to skip programming Hardware Autonomous Enable bit
    in Power Control Enable (PCE) at offset E4h P2SB config space register.
    <b>FALSE: Disable</b>, TRUE: Enable
  **/
  BOOLEAN               HaPowerGatingSupported;
  /**
    If enabled, sets DPEE, CPEE and DPPEE bits to enable parity error handling
  **/
  BOOLEAN               EnableParityCheck;
  /**
  This option disables Integrated Embedded Controller (IEC)
  and then LPC Fixed IO routing to IEC
  (write 0 to LFIORIEC register)
  <b>FALSE: IEC enabled</b>, TRUE: IEC disabled
  **/
  BOOLEAN               IecSupportDisable;
  /**
    Allow avoid HPET and APIC BDFs to be programmed.
   <b>FALSE: BDF is programmed</b>, TRUE: BDF is not programmed
  **/
  BOOLEAN               DisableHpetAndApicBdfProgramming;
} P2SB_PRIVATE_CONFIG;

/**
  P2SB callbacks
  List of function pointers can be passed to IP Block driver
**/
typedef struct {
  VOID   *PlaceHolder;
} P2SB_CALLBACK;

/**
  GBE device structure
  Stores all data necessary to initialize GBE IP block
**/
typedef struct {
  P2SB_CONTROLLER             *Controller;
  REGISTER_ACCESS             *PcieConfigAccess;
  PCH_P2SB_CONFIG             *P2SbConfig;
  P2SB_PRIVATE_CONFIG         *PrivateConfig;
  P2SB_CALLBACK               *Callback;
} P2SB_HANDLE;

#endif // _P2SB_HANDLE_H_
