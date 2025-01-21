/** @file
  Xml Cli Pei driver implementation.

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

#include <PiPei.h>
#include <Ppi/ReadOnlyVariable2.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/PeimEntryPoint.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/PerformanceLib.h>
#include <Library/PeiServicesTablePointerLib.h>
#include <Library/PrintLib.h>
#include <Library/IoLib.h>
#include <XmlCliSetup.h>

#define CMOS_DRAM_SHARED_MB_ADDR_REG  (0xF0)

/**
  Write a byte value to a CMOS address.

  @param [in] Address Location to write to CMOS.
  @param [in] Data    The byte value write to the CMOS address.
**/
VOID
EFIAPI
CmosWrite8 (
  IN      UINTN  Index,
  IN      UINT8  Value
  )
{
  IoWrite8 (0x70, (UINT8)Index);
  IoWrite8 (0x71, Value);
}

/**
  Main entry for XmlCli PEIM.
  This routine is to restore Dram Shared mailbox address during S3 RESUME.

  @param[in] FileHandle   Handle of the file being invoked.
  @param[in] PeiServices  General purpose services available to every PEIM.

**/
EFI_STATUS
EFIAPI
PeimXmlCliEntryPoint (
  IN EFI_PEI_FILE_HANDLE       FileHandle,
  IN CONST EFI_PEI_SERVICES    **PeiServices
  )
{
  UINT32                           DramMbAddress=0;
  UINTN                            DataSize;
  EFI_BOOT_MODE                    BootMode;
  EFI_STATUS                       Status;
  XMLCLI_SETUP                     XmlCliSetup;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *Variable;

  Status = PeiServicesGetBootMode (&BootMode);
  ASSERT_EFI_ERROR (Status);

  if (BootMode != BOOT_ON_S3_RESUME) {
    return EFI_SUCCESS;
  }

  Status = PeiServicesLocatePpi (&gEfiPeiReadOnlyVariable2PpiGuid, 0, NULL, (VOID **) &Variable);
  ASSERT_EFI_ERROR (Status);

  DataSize = sizeof(XMLCLI_SETUP);
  Status = Variable->GetVariable (Variable, XMLCLI_SETUP_NAME, &gXmlCliSetupGuid, NULL, &DataSize, &XmlCliSetup);
  if (!EFI_ERROR (Status)) {
    //
    // Restoring Dram Shared mailBox address
    //
    DramMbAddress = XmlCliSetup.XmlCliDramCmosAddr;
    // Restoring the Mailbox address in Cmos so that the tool can read it, this address is reserved area and may change, hence saving in cmos for the tool to read
    CmosWrite8((UINT8)CMOS_DRAM_SHARED_MB_ADDR_REG, (UINT8)(DramMbAddress >> 16));  // save lower byte of higher UINT16
    CmosWrite8((UINT8)CMOS_DRAM_SHARED_MB_ADDR_REG + 1, (UINT8)(DramMbAddress >> 24));  // save upper byte of higher UINT16
  }
  return Status;
}
