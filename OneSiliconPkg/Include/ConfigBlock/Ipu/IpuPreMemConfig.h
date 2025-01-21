/** @file
  IPU policy definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _IPU_CONFIG_PREMEM_H_
#define _IPU_CONFIG_PREMEM_H_

#pragma pack(push, 1)
#include <ConfigBlock.h>

#define IPU_PREMEM_CONFIG_REVISION 1

//
// GPIO native features pins data
//
#define GPIO_IMGUCLK_NUMBER_OF_PINS 6

//
// Maximum CSI (Camera Serial Interface) Port
//
#define MAX_CSI_PORT  8

// @todo file name will be changed to IpuConfig.h.

extern EFI_GUID gIpuPreMemConfigGuid;

/**
 IPU PreMem configuration\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;           /// Config Block Header
  /**
  <b>(Test)</b> It enables the SA IPU Device if supported and not fused off.
  If FALSE, all other policies in this config block will be ignored.
  <b>1=TRUE</b>;
  0=FALSE.
  **/
  UINT8    IpuEnable;
  /**
  It enable the IMGU CLKOUT.
  <b>TRUE</b>
  FALSE
  **/
  UINT8    ImguClkOutEn[GPIO_IMGUCLK_NUMBER_OF_PINS];
  UINT8    Rsvd;

} IPU_PREMEM_CONFIG;
#pragma pack(pop)

#endif // _IPU_PREMEM_CONFIG_H_
