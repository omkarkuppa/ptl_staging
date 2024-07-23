/**@file
  TSN FV Installation

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

@par Specification
**/

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PeiTsnFvLib.h>
#include <Library/PeiSubRegionLib.h>

/**
  Get Tsn FV base address

  Base address will vary between SPI and UFS boot.

  @return FvBase associated with FvBinary FV.
**/
UINT32
GetFlashTsnFvBase (
  VOID
  )
{
  return 0;
}

/**
  Install Base and Size Info Ppi for Siip Firmware Volume.

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
InstallTsnFv (
  VOID
)
{
  return EFI_SUCCESS;
}

/**
  Build FV Hob for TSN Sub Region so it can be found in DXE

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
LoadFlashTsnFv (
  VOID
)
{
  return EFI_SUCCESS;
}

/*
  Reads TSN Sub region.

  Reads TSN sub region data from FV. Returns pointer to data.

  @param[out]  SubRegion Pointer to sub region data.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       TSN region was not found
*/
EFI_STATUS
GetTsnSubRegion (
  OUT TSN_SUB_REGION** SubRegion
  )
{
  return EFI_SUCCESS;
}