/** @file
  Header file for PEI GBE Init Library

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

#ifndef _PEI_GBE_INIT_LIB_H_
#define _PEI_GBE_INIT_LIB_H_

#include <Ppi/SiPolicy.h>
#include <GbeHandle.h>

/**
  Initialize LAN device.

  Global reset may be perfomed due to static power gating requirements.

  @param[in] GbeHandle             The SI Policy PPI instance
**/
VOID
GbeInit (
  IN  GBE_HANDLE          *GbeHandle
  );

/**
  Sets GbE Lock Write Flash

  @param[in]  GbePciBase     GbE PCI Base Address
  @param[in]  GbeMemBar      GbE MMIO Address
**/
VOID
GbeLockWriteFlash (
  IN   UINT64         GbePciBase,
  IN   UINTN          GbeMemBar
  );
#endif // _PEI_GBE_INIT_LIB_H_
