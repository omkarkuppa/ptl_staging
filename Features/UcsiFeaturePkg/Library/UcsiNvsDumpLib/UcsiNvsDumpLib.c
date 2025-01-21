/** @file
  UCSI ACPI NVS Dump function library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <UcsiNvs.h>
#include <Ucsi.h>

/**
  Print UCSI NVS information

  @retval EFI_SUCCESS           Success.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
DumpUcsiNvsData (
  VOID
  )
{
  EFI_STATUS  Status;
  UCSI_NVS_AREA_PROTOCOL *mUcsiNvsAreaProtocol;
  UCSI_OPM_BUF           *OpmBuffer;
  UINT8 Idx;

  Status = gBS->LocateProtocol (
                  &gUcsiNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &mUcsiNvsAreaProtocol
                  );
  ASSERT_EFI_ERROR (Status);

  OpmBuffer = (UCSI_OPM_BUF *)(UINTN)mUcsiNvsAreaProtocol->Area->UcsiOpBaseAddr;

  DEBUG ((DEBUG_INFO, "UCSI NVS data:\n"));
  DEBUG ((DEBUG_INFO, "NvsArea->UcsiOpBaseAddr = 0x%x\n", mUcsiNvsAreaProtocol->Area->UcsiOpBaseAddr));
  DEBUG ((DEBUG_INFO, "  OpmBuffer->Version = 0x%x\n", OpmBuffer->Version));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort1UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort1UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort2UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort2UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort3UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort3UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort4UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort4UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort5UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort5UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort6UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort6UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort7UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort7UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort8UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort8UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPort9UcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPort9UcxxSupport));
  DEBUG ((DEBUG_INFO, "NvsArea->UsbCPortAUcxxSupport = 0x%x\n", mUcsiNvsAreaProtocol->Area->UsbCPortAUcxxSupport));

  for ( Idx = 0; Idx < 10; Idx++) {
    DEBUG ((DEBUG_INFO, "NvsArea->ConnectorGroupPosition[%d] = 0x%x\n", Idx, mUcsiNvsAreaProtocol->Area->ConnectorGroupPosition[Idx]));
    DEBUG ((DEBUG_INFO, "NvsArea->ConnectorVisibility[%d] = 0x%x\n", Idx, mUcsiNvsAreaProtocol->Area->ConnectorVisibility[Idx]));
    DEBUG ((DEBUG_INFO, "NvsArea->ConnectorConnectable[%d] = 0x%x\n", Idx, mUcsiNvsAreaProtocol->Area->ConnectorConnectable[Idx]));
    DEBUG ((DEBUG_INFO, "NvsArea->ConnectorType[%d] = 0x%x\n", Idx, mUcsiNvsAreaProtocol->Area->ConnectorType[Idx]));
    DEBUG ((DEBUG_INFO, "NvsArea->ConnectorCapability[%d] = 0x%x\n", Idx, mUcsiNvsAreaProtocol->Area->ConnectorCapability[Idx]));
    DEBUG ((DEBUG_INFO, "==============================================\n"));
  }

  return Status;
}