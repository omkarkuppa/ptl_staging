/** @file
  Source file for FSP PCD Init PEI module

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

#define PCIEX_BAR_REG 0x80000060     // B0:D0:F0:R60
#define PCIEX_BAR_EN_MASK  0x1
#define PCIEX_BAR_LENGTH_MASK  0xE
#define PCIEX_BAR_ADDR_MASK  0x7FFC000000

/**
  FSP PCD Init PEI module entry point

  @param[in]  FileHandle           Not used.
  @param[in]  PeiServices          General purpose services available to every PEIM.

  @retval     EFI_SUCCESS          The function completes successfully
  @retval     EFI_OUT_OF_RESOURCES Insufficient resources to create database
**/
EFI_STATUS
EFIAPI
FspPcdInitEntryPoint (
  IN       EFI_PEI_FILE_HANDLE    FileHandle,
  IN CONST EFI_PEI_SERVICES       **PeiServices
  )
{
  UINT64       PciExBarRegValue;
  UINT8        BarLength;
  BOOLEAN      InterruptState;

  InterruptState = SaveAndDisableInterrupts ();
  IoWrite32(0xCF8, PCIEX_BAR_REG);
  PciExBarRegValue = IoRead32(0xCFC);
  SetInterruptState (InterruptState);
  if ((PciExBarRegValue & PCIEX_BAR_EN_MASK) == 1) {
    PcdSetEx64S (&gSiPkgTokenSpaceGuid, PcdSiPciExpressBaseAddress, (PciExBarRegValue & PCIEX_BAR_ADDR_MASK));
    BarLength = (UINT8) (PciExBarRegValue & PCIEX_BAR_LENGTH_MASK);
    BarLength = BarLength >> 1;
    if (BarLength == 0) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_256MB);
    } else if (BarLength == 1) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_128MB);
    } else if (BarLength == 2) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_64MB);
    } else if (BarLength == 3) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_512MB);
    } else if (BarLength == 4) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_1GB);
    } else if (BarLength == 5) {
      PcdSetEx32S (&gSiPkgTokenSpaceGuid, PcdPciExpressRegionLength, SIZE_2GB);
    }
    DEBUG ((DEBUG_INFO, "FspPcdInitEntryPoint() - Updating PcdPciExpressRegionLength to 0x%x & PcdPciExpressBaseAddress to 0x%lx\n",
            PcdGet32(PcdPciExpressRegionLength), PcdGet64(PcdSiPciExpressBaseAddress)));
  } else {
    DEBUG ((DEBUG_INFO, "FspPcdInitEntryPoint() - ERROR PciExpressBaseAddress not programmed\n" ));
    ASSERT ((PciExBarRegValue & PCIEX_BAR_EN_MASK) == 1);
  }
  return EFI_SUCCESS;
}
