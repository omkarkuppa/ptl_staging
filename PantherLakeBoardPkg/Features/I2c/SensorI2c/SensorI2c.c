/**@file
  This PEIM driver initialize I2C sensor

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

#include <Ppi/ReadOnlyVariable2.h>
#include <SetupVariable.h>
#include <Library/PcdLib.h>
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Ppi/SensorI2cPolicyPpi.h>

#define BMI323_SENSOR       1
#define LSM6DSOX_SENSOR     2

/**
  The user code starts with this function.

  @param[in]  FileHandle       Handle of the file being invoked.
  @param[in]  PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS      The driver is successfully initialized.
  @retval Others         Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
SensorI2cEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );

/**
  The user code starts with this function.

  @param[in]  FileHandle       Handle of the file being invoked.
  @param[in]  PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS          The driver is successfully initialized.
  @retval Others               Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
SensorI2cEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                       Status;
  EFI_PEI_READ_ONLY_VARIABLE2_PPI  *VariableServices;
  UINTN                            VarSize;
  SETUP_DATA                       SetupData;
  PEI_SENSOR_I2C_POLICY_PPI        *ReadSensorData;

  DEBUG ((DEBUG_INFO, "%a.\n", __FUNCTION__));

  //
  // Retrieve Setup variable
  //
  Status = PeiServicesLocatePpi (
             &gEfiPeiReadOnlyVariable2PpiGuid, // GUID
             0,                                // INSTANCE
             NULL,                             // EFI_PEI_PPI_DESCRIPTOR
             (VOID **)&VariableServices        // PPI
             );
  ASSERT_EFI_ERROR (Status);

  VarSize = sizeof (SETUP_DATA);
  Status = VariableServices->GetVariable (
                               VariableServices,
                               L"Setup",
                               &gSetupVariableGuid,
                               NULL,
                               &VarSize,
                               &SetupData
                               );

  Status = PeiServicesLocatePpi(
              &gSensorI2cPpiGuid,
              0,
              NULL,
              (VOID **) &ReadSensorData
              );

  if (SetupData.OrientationSensor == BMI323_SENSOR) {
    Status = ReadSensorData->ReadBmi323Orientation();
  }
  else if (SetupData.OrientationSensor == LSM6DSOX_SENSOR) {
    Status = ReadSensorData->ReadLsm6dsoxOrientation();
  } else {
    DEBUG ((DEBUG_ERROR, "Orientation sensor is disabled.\n"));
    Status = EFI_SUCCESS;
  }

  return Status;
}
