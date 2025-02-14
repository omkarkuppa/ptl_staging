/**@file
  This PEIM driver initialize I2C sensor

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

@par Specification
**/
#include "I2cSensorLSM6DSOX.h"
#include "I2cSensorBMI323.h"
#include "I2cSensorPei.h"
#include "Ppi/ReadOnlyVariable2.h"
#include "Library/PcdLib.h"
#include <Ppi/SensorI2cPolicyPpi.h>
#include <Library/SerialIoI2cLib.h>

PEI_SENSOR_I2C_POLICY_PPI mReadSensorDataNotify = {
  ReadBmi323Orientation,
  ReadLsm6dsoxOrientation
};

EFI_PEI_PPI_DESCRIPTOR     mReadSensorDataNotifyList = {
  (EFI_PEI_PPI_DESCRIPTOR_NOTIFY_DISPATCH | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST | EFI_PEI_PPI_DESCRIPTOR_PPI),
  &gSensorI2cPpiGuid,
  &mReadSensorDataNotify
};


/**
  Read orientation sensor data

  @param[in]   Address
  @param[out]  Buffer
  @param[in]   BufferSize

  @retval  EFI_STATUS
**/
EFI_STATUS
ReadSensorData (
  IN UINT8      Address,
  OUT UINT8     *Buffer,
  IN UINT32     BufferSize
  )
{
  EFI_STATUS      Status;
  UINT8           I2cNumber;

  DEBUG((DEBUG_INFO, "ReadSensorData ()\n"));

  if (BufferSize == 0) {
    return EFI_SUCCESS;
  }

  if (Buffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  I2cNumber = PcdGet8 (PcdOrientationI2cSensorBusNumber);

  Status = SerialIoI2cWrite (
             0,
             &I2cNumber,
             PcdGet8 (PcdOrientationI2cSensorAddress),
             1,
             &Address,
             1000000, //1sec
             400, //400kHz
             NULL,
             TRUE,
             TRUE
             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  return SerialIoI2cRead (
           0,
           &I2cNumber,
           PcdGet8 (PcdOrientationI2cSensorAddress),
           BufferSize,
           Buffer,
           1000000, //1sec
           400, //400kHz
           NULL,
           FALSE
           );
}

/**
  Write 16 bit data to orientation sensor.

  @param[in]  Address    Sensor internal register address
  @param[in]  Value      Value to write

  @retval  EFI_STATUS
**/
EFI_STATUS
WriteSensor16 (
  IN UINT8      Address,
  IN UINT16     Value
  )
{
  EFI_STATUS      Status;
  UINT8           I2cNumber;
  UINT8           WriBuf[3] = {0,0,0};

  DEBUG((DEBUG_INFO, "WriteSensor16 ()\n"));

  WriBuf[0] = Address;
  WriBuf[1] = (UINT8)(Value & 0xFF);
  WriBuf[2] = (UINT8)(Value >> 8);
  I2cNumber = PcdGet8 (PcdOrientationI2cSensorBusNumber);

  Status = SerialIoI2cWrite (
             0,
             &I2cNumber,
             PcdGet8 (PcdOrientationI2cSensorAddress),
             3,
             WriBuf,
             1000000, //1sec
             400, //400kHz
             NULL,
             TRUE,
             TRUE
             );

  return Status;
}


/**
  Write 8 bit data to orientation sensor.

  @param[in]  Address    Sensor internal register address
  @param[in]  Value      Value to write

  @retval  EFI_STATUS
**/
EFI_STATUS
WriteSensorByte (
  IN UINT8      Address,
  IN UINT8      Value
  )
{
  EFI_STATUS      Status;
  UINT8           I2cNumber;
  UINT8           WriBuf[2] = {0, 0};

  DEBUG((DEBUG_INFO, "WriteSensorByte ()\n"));

  WriBuf[0] = Address;
  WriBuf[1] = Value;

  I2cNumber = PcdGet8 (PcdOrientationI2cSensorBusNumber);

  Status = SerialIoI2cWrite (   // Writes the register address
             0,
             &I2cNumber,
             PcdGet8 (PcdOrientationI2cSensorAddress),
             2,
             WriBuf,
             1000000, //1sec
             400, //400kHz
             NULL,
             TRUE,
             TRUE
             );

  return Status;
}

/**
  The user code starts with this function.

  @param[in]  FileHandle       Handle of the file being invoked.
  @param[in]  PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS          The driver is successfully initialized.
  @retval Others               Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
I2cSensorPeiEntry (
  IN EFI_PEI_FILE_HANDLE     FileHandle,
  IN CONST EFI_PEI_SERVICES  **PeiServices
  )
{
  EFI_STATUS                       Status;

  DEBUG ((DEBUG_INFO, "%a.\n", __FUNCTION__));

  Status = PeiServicesInstallPpi (&mReadSensorDataNotifyList);

  return Status;
}
