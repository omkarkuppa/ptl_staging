/** @file
  Header file for PtlPcdMeLib

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
#ifndef _PTL_PCD_ME_INIT_LIB_H_
#define _PTL_PCD_ME_INIT_LIB_H_

#include <MeHandle.h>

/**
  Get CSME HECI devices sideband registers access

  @param[in]      This                    The ME_HANDLE instance.
  @param[in]      DeviceSel               Device to be configured
  @param[in, out] P2SbController          P2SB controller pointer
  @param[in, out] HeciPcrAccess           P2SB register access to ME devices
**/
VOID
EFIAPI
PtlPcdGetCsmeHeciSbAccess (
  IN     ME_HANDLE                     *This,
  IN     HECI_DEVICE                   DeviceSel,
  IN OUT P2SB_CONTROLLER               *P2SbController,
  IN OUT P2SB_SIDEBAND_REGISTER_ACCESS *HeciPcrAccess
  );

/**
  Get CSME HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval CSME HECI controller address in PCI Segment Library representation
**/
UINT64
EFIAPI
PtlPcdGetCsmeHeciPciCfgBase (
  IN HECI_DEVICE   HeciFunc
  );

/**
  Get PSE HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc     PSE HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
EFIAPI
PtlPcdGetPseHeciPciCfgBase (
  IN PSE_HECI_DEVICE   HeciFunc
  );

/**
  Get Xhci controller address that can be passed to the PCI Segment Library functions.

  @retval XHCI controller address in PCI Segment Library representation
**/
UINT64
PtlPcdGetXhciPciCfgBase (
  VOID
  );

#endif
