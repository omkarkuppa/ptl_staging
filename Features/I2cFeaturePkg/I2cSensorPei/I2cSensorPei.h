/** @file
  This PEIM driver initialize I2C Orientation sensor

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

#ifndef _SENSOR_I2C_H_
#define _SENSOR_I2C_H_

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>

/**
  Read orientation sensor data

  @param[in]  Address
  @param[out]  Buffer
  @param[in]  BufferSize

  @retval  EFI_STATUS
**/
EFI_STATUS
ReadSensorData (
  IN UINT8     Address,
  OUT UINT8     *Buffer,
  IN UINT32    BufferSize
  );


/**
  Write 16 bits data to orientation sensor.

  @param[in]  Address    Sensor internal register address
  @param[in]  Value      Value to write

  @retval  EFI_STATUS
**/
EFI_STATUS
WriteSensor16 (
  IN UINT8      Address,
  IN UINT16     Value
  );



/**
  Write 8 bits data to orientation sensor.

  @param[in]  Address    Sensor internal register address
  @param[in]  Value      Value to write

  @retval  EFI_STATUS
**/
EFI_STATUS
WriteSensorByte (
  IN UINT8      Address,
  IN UINT8      Value
  );


/**
  The user code starts with this function.

  @param[in]  FileHandle       Handle of the file being invoked.
  @param[in]  PeiServices      Describes the list of possible PEI Services.

  @retval EFI_SUCCESS      The driver is successfully initialized.
  @retval Others         Can't initialize the driver.

**/
EFI_STATUS
EFIAPI
I2cSensorPeiEntry (
  IN EFI_PEI_FILE_HANDLE      FileHandle,
  IN CONST EFI_PEI_SERVICES   **PeiServices
  );



#endif  // _SENSOR_I2C_H_
