/** @file
  Configuration parameters for DisplayPort/USB3/PCIe tunneling.

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

@par Specification
**/

#ifndef _PROTOCOL_CONFIG_H_
#define _PROTOCOL_CONFIG_H_

//
// Allocated Bandwidth for Isochronous USB3 traffic.
//
#define USB3_ALLOCATED_BW         500

//
// DP path type
//
typedef enum {
  DPIN_TO_LANE_MAIN     = 1,
  DPIN_TO_LANE_AUX      = 2,
  LANE_TO_DPIN_AUX      = 3,
  LANE_TO_DPOUT_MAIN    = 4,
  LANE_TO_DPOUT_AUX     = 5,
  DPOUT_TO_LANE_AUX     = 6,
  DP_LANE_TO_LANE_MAIN  = 7,
  DP_LANE_TO_LANE_AUX   = 8
} DP_PATH_TYPE, *PDP_PATH_TYPE;

//
// NFC buffers
//
#define DP_MID_NFC_BUFFERS              70
#define DP_OUT_NFC_BUFFERS              14

//
// Hop ID in path
//
#define PCIE_ADAPTER_HOPID               8
#define USB3_ADAPTER_HOPID               8
#define DP_ADAPTER_AUX_HOPID             8
#define DP_ADAPTER_MAIN_HOPID            9
#define MIN_PROTOCOL_HOPID               8

#define LANE_ADAPTER_PCIE_HOPID          8
#define LANE_ADAPTER_USB3_HOPID          9

//
// Map to HopIdMask bit 0 - bit 31
//
#define DYNAMIC_HOPID_MIN               10
#define DYNAMIC_HOPID_MAX               (DYNAMIC_HOPID_MIN + 31)

#define MAX_DPRX_WAIT_COUNT       3000
#define DPRX_POLL_US              10

//
// PCIE path parameters
//
#define PCIE_TO_LANE_PATH_PRIORITY                     3
#define PCIE_TO_LANE_PATH_WEIGHT                       1
#define PCIE_TO_LANE_PATH_IFC                          1
#define PCIE_TO_LANE_PATH_ISE                          0
#define PCIE_TO_LANE_PATH_EFC                          1
#define PCIE_TO_LANE_PATH_ESE                          0

#define LANE_TO_PCIE_PATH_PRIORITY                     3
#define LANE_TO_PCIE_PATH_WEIGHT                       1
#define LANE_TO_PCIE_PATH_IFC                          1
#define LANE_TO_PCIE_PATH_ISE                          0
#define LANE_TO_PCIE_PATH_EFC                          0
#define LANE_TO_PCIE_PATH_ESE                          0
#define LANE_TO_PCIE_PATH_CREDITS_DTBT_X1              16
#define LANE_TO_PCIE_PATH_CREDITS_DTBT_X2              32
#define LANE_TO_PCIE_PATH_CREDITS_ITBT_X1              32
#define LANE_TO_PCIE_PATH_CREDITS_ITBT_X2              64
#define LANE_TO_PCIE_PATH_CREDITS_LEGACY               16
#define LANE_TO_PCIE_PATH_CREDITS_10G                  16
#define LANE_TO_PCIE_PATH_CREDITS_20G                  24
#define LANE_TO_PCIE_PATH_CREDITS_40G                  32

//
// USB3 path parameters
//
#define USB3_TO_LANE_PATH_PRIORITY                     3
#define USB3_TO_LANE_PATH_WEIGHT                       3
#define USB3_TO_LANE_PATH_IFC                          1
#define USB3_TO_LANE_PATH_ISE                          0
#define USB3_TO_LANE_PATH_EFC                          1
#define USB3_TO_LANE_PATH_ESE                          0

#define LANE_TO_USB3_PATH_PRIORITY                     3
#define LANE_TO_USB3_PATH_WEIGHT                       1
#define LANE_TO_USB3_PATH_IFC                          1
#define LANE_TO_USB3_PATH_ISE                          0
#define LANE_TO_USB3_PATH_EFC                          0
#define LANE_TO_USB3_PATH_ESE                          0
#define LANE_TO_USB3_PATH_CREDITS_DTBT_X1              8
#define LANE_TO_USB3_PATH_CREDITS_DTBT_X2              14
#define LANE_TO_USB3_PATH_CREDITS_ITBT_X1              16
#define LANE_TO_USB3_PATH_CREDITS_ITBT_X2              32
#define LANE_TO_USB3_PATH_CREDITS_LEGACY               16
#define LANE_TO_USB3_PATH_CREDITS_10G                  16
#define LANE_TO_USB3_PATH_CREDITS_20G                  24
#define LANE_TO_USB3_PATH_CREDITS_40G                  32

//
// MAIN link path parameters : DP-IN to Lane
// Flow Control - N/A
//
#define DPIN_TO_LANE_PATH_MAIN_PRIORITY                1
#define DPIN_TO_LANE_PATH_MAIN_WEIGHT                  1
#define DPIN_TO_LANE_PATH_MAIN_CREDITS_ALLOCATED       5      // protocol adapter ignored
#define DPIN_TO_LANE_PATH_MAIN_IFC                     0      // NA in CM guide
#define DPIN_TO_LANE_PATH_MAIN_ISE                     0      // NA in CM guide
#define DPIN_TO_LANE_PATH_MAIN_EFC                     0
#define DPIN_TO_LANE_PATH_MAIN_ESE                     0

//
// Aux OUT path parameters : DP-IN to Lane
// Flow Control - N/A
//
#define DPIN_TO_LANE_PATH_AUX_PRIORITY                 2
#define DPIN_TO_LANE_PATH_AUX_WEIGHT                   1
#define DPIN_TO_LANE_PATH_AUX_CREDITS_ALLOCATED        4      // protocol adapter ignored
#define DPIN_TO_LANE_PATH_AUX_IFC                      1      // NA in CM guide
#define DPIN_TO_LANE_PATH_AUX_ISE                      0      // NA in CM guide
#define DPIN_TO_LANE_PATH_AUX_EFC                      1
#define DPIN_TO_LANE_PATH_AUX_ESE                      0

//
// Aux IN path parameters : Lane to DP-IN
// Flow Control/Allocated Buffers - Dedicated/1
//
#define LANE_TO_DPIN_PATH_AUX_PRIORITY                 2
#define LANE_TO_DPIN_PATH_AUX_WEIGHT                   1
#define LANE_TO_DPIN_PATH_AUX_CREDITS_ALLOCATED        1
#define LANE_TO_DPIN_PATH_AUX_IFC                      1
#define LANE_TO_DPIN_PATH_AUX_ISE                      0
#define LANE_TO_DPIN_PATH_AUX_EFC                      1      // NA in CM guide
#define LANE_TO_DPIN_PATH_AUX_ESE                      0      // NA in CM guide

//
// MAIN link path parameters : Lane to DP-OUT
// Flow Control/Allocated Buffers - Disabled/26
//
#define LANE_TO_DPOUT_PATH_MAIN_PRIORITY               1
#define LANE_TO_DPOUT_PATH_MAIN_WEIGHT                 1      // NA in CM guide
#define LANE_TO_DPOUT_PATH_MAIN_CREDITS_ALLOCATED      26     // 14 in CM guide
#define LANE_TO_DPOUT_PATH_MAIN_IFC                    0
#define LANE_TO_DPOUT_PATH_MAIN_ISE                    0
#define LANE_TO_DPOUT_PATH_MAIN_EFC                    0      // NA in CM guide
#define LANE_TO_DPOUT_PATH_MAIN_ESE                    0      // NA in CM guide

//
// Aux OUT path parameters : Lane to DP-OUT
// Flow Control/Allocated Buffers - Dedicated/1
//
#define LANE_TO_DPOUT_PATH_AUX_PRIORITY                2
#define LANE_TO_DPOUT_PATH_AUX_WEIGHT                  1
#define LANE_TO_DPOUT_PATH_AUX_CREDITS_ALLOCATED       1
#define LANE_TO_DPOUT_PATH_AUX_IFC                     1
#define LANE_TO_DPOUT_PATH_AUX_ISE                     0
#define LANE_TO_DPOUT_PATH_AUX_EFC                     1      // NA in CM guide
#define LANE_TO_DPOUT_PATH_AUX_ESE                     0      // NA in CM guide

//
// Aux IN path parameters : DP-OUT to Lane
// Flow Control - N/A
//
#define DPOUT_TO_LANE_PATH_AUX_PRIORITY                2
#define DPOUT_TO_LANE_PATH_AUX_WEIGHT                  1
#define DPOUT_TO_LANE_PATH_AUX_CREDITS_ALLOCATED       4      // protocol adapter ignored
#define DPOUT_TO_LANE_PATH_AUX_IFC                     1      // NA in CM guide
#define DPOUT_TO_LANE_PATH_AUX_ISE                     0      // NA in CM guide
#define DPOUT_TO_LANE_PATH_AUX_EFC                     1
#define DPOUT_TO_LANE_PATH_AUX_ESE                     0

//
// MAIN link path parameters : Lane to Lane
// Flow Control/Allocated Buffers - Disabled/26
//
#define LANE_TO_LANE_PATH_MAIN_PRIORITY                1
#define LANE_TO_LANE_PATH_MAIN_WEIGHT                  1
#define LANE_TO_LANE_PATH_MAIN_CREDITS_ALLOCATED       26     // 14 in CM guide
#define LANE_TO_LANE_PATH_MAIN_IFC                     0
#define LANE_TO_LANE_PATH_MAIN_ISE                     0
#define LANE_TO_LANE_PATH_MAIN_EFC                     0
#define LANE_TO_LANE_PATH_MAIN_ESE                     0

//
// Aux path parameters : Lane to Lane
// Flow Control/Allocated Buffers - Dedicated/1
//
#define LANE_TO_LANE_PATH_AUX_PRIORITY                 2
#define LANE_TO_LANE_PATH_AUX_WEIGHT                   1
#define LANE_TO_LANE_PATH_AUX_CREDITS_ALLOCATED        1
#define LANE_TO_LANE_PATH_AUX_IFC                      1
#define LANE_TO_LANE_PATH_AUX_ISE                      0
#define LANE_TO_LANE_PATH_AUX_EFC                      1
#define LANE_TO_LANE_PATH_AUX_ESE                      0

#endif
