/** @file

  This file contains RST related PPI definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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


#ifndef __PEI_RST_CONFIG_PPI_H__
#define __PEI_RST_CONFIG_PPI_H__

//
// This PPI is installed by AhciHc driver when AHCI controller is RAID mode.
// RST driver will consume this PPI to produce passthru PPI/ SSCP interface of RST version.
//
#define RST_RAID_CONTROLLER_PPI_GUID \
{  \
  0x62f52aed, 0xf299, 0x4167, { 0x8e, 0x94, 0xea, 0x61, 0x9f, 0x5, 0x12, 0xad } \
}

extern EFI_GUID gRstRaidControllerPpiGuid;

//
// This PPI is installed by NvmeHc driver when NVMe device is hybrid device.
// RST driver will consume this PPI to produce passthru PPI/ SSCP interface of RST version.
//
#define HYBRID_NVME_EXPRESS_HOST_CONTROLLER_PPI_GUID \
{ \
  0x5da70373, 0xd684, 0x4b95, { 0x9a, 0xae, 0x72, 0x6d, 0xa3, 0xfd, 0x47, 0xcb } \
}

extern EFI_GUID gPeiHybridNvmExpressHostControllerPpiGuid;

#endif
