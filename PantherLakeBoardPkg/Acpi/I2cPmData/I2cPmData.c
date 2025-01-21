/**@file
  I2C Board Driver

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

#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <GpioV2Pad.h>
#include <Uefi/UefiBaseType.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <SetupVariable.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PchInfoLib.h>
#include <I2cPowerMeterAcpiTable.h>

/**
  I2c Power Meter data configuration.
  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.
  @retval EFI_SUCCESS        The protocol was located succesfully.
**/
EFI_STATUS
EFIAPI
I2cPmInfo (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                      Status;
  I2C_PM_CONFIG_PROTOCOL          *I2cPmConfigProtocol;
  POWER_METER_PKG                 *PwrMtrPkg;
  SETUP_DATA                      SetupData;
  UINTN                           VarDataSize;

  VarDataSize = sizeof (SETUP_DATA);

  Status = gRT->GetVariable (
                  L"Setup",
                  &gSetupVariableGuid,
                  NULL,
                  &VarDataSize,
                  &SetupData
                  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "Power Meter Info\n"));
  Status = gBS->LocateProtocol (&gI2cPmUpdateProtocolGuid, NULL, (VOID **)&I2cPmConfigProtocol);
  if (!EFI_ERROR(Status)) {
    PwrMtrPkg = (POWER_METER_PKG*) PcdGetPtr (VpdPcdPowerMeter);
    if (SetupData.TelemetryDeviceEnable == 1){
      PwrMtrPkg = (POWER_METER_PKG*) PcdGetPtr (VpdPcdTelemetry);
    }
    if (PwrMtrPkg->I2cBusNumber == 0) { //I2cBusNumber value should be '0', '1' ...
      DEBUG ((DEBUG_INFO, "VpdPcdPowerMeter data is not valid\n"));
    } else {
      I2cPmConfigProtocol->I2cPmData (PwrMtrPkg, SetupData.PowermeterDeviceEnable);
    }
  } else {
    DEBUG ((DEBUG_INFO, "Not able to find Protocol\n"));
  }
  return Status;
}

