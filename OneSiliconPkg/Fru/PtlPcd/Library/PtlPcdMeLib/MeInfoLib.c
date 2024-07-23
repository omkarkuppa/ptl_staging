/** @file
  This code provides an instance of MeInfoLib.

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

@par Specification Reference:
**/
#include <Uefi.h>
#include <Pi/PiMultiPhase.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Defines/PcdPchBdfAssignment.h>
#include <MeDefines.h>

/**
  Get HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
MeHeciPciCfgBase (
  IN HECI_DEVICE   HeciFunc
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
           DEFAULT_PCI_SEGMENT_NUMBER_PCH,
           DEFAULT_PCI_BUS_NUMBER_PCH,
           PCI_DEVICE_NUMBER_PCH_HECI1,
           HeciFunc,
           0
           );
}

/**
  Get PSE HECI controller address that can be passed to the PCI Segment Library functions.

  @param[in] HeciFunc              HECI device function to be accessed.

  @retval HECI controller address in PCI Segment Library representation
**/
UINT64
MePseHeciPciCfgBase (
  IN PSE_HECI_DEVICE   HeciFunc
  )
{
  return PCI_SEGMENT_LIB_ADDRESS (
            DEFAULT_PCI_SEGMENT_NUMBER_PCH,
            DEFAULT_PCI_BUS_NUMBER_PCH,
            PCI_DEVICE_NUMBER_PCH_PSE_HECI1,
            HeciFunc,
            0
            );
}