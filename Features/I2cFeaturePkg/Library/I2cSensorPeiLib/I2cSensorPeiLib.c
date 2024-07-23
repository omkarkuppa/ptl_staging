/**@file
  Mock function of the I2C Sensor PEIM.

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


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <PiPei.h>
#include <I2cSensorPeiLib.h>

/**
  Mock SerialIoI2cWrite.

  @param[in]   PciCfgBase     Pci Config Base Address of specified I2C controller
                              @Note: If I2C Number is provided this parameter is ignored and can be left as 0
  @param[in]   I2cNumber      Optional: I2C controller applicable on bus 0 only
  @param[in]   TargetAddress  Address of the end point device
  @param[in]   WriteLength    Write length of the current transaction
  @param[in]   WriteBuffer    Pointer to data in write buffer
  @param[in]   TimeBudget     Time allotted to verify if I2C is enabled
  @param[in]   Frequency      Bus clock frequency (in kHz)
  @param[in]   TimingConfig   Optional: Pointer to struct passed down to override SDA Hold and SCL LCNT/HCNT settings
  @param[in]   Stop           Issues STOP on the last byte
  @param[in]   Restart        Indication if restart is needed, if TRUE will Initialize I2C controller.
                              @Note: Restart is required in first transaction.

  @retval   EFI_INVALID_PARAMETER   Invalid length / buffer parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in I2C disable/enable or wrong ENST value
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Write was successful
**/
EFI_STATUS
EFIAPI
SerialIoI2cWrite (
  IN UINTN                 PciCfgBase,
  IN UINT8                 *I2cNumber  OPTIONAL,
  IN UINT32                TargetAddress,
  IN UINT32                WriteLength,
  IN UINT8                 *WriteBuffer,
  IN UINT64                TimeBudget,
  IN UINTN                 Frequency,
  IN UINTN                 *TimingConfig  OPTIONAL,
  IN BOOLEAN               Stop,
  IN BOOLEAN               Restart
  )
{
  UINT8 I2cBusNumber;

  I2cBusNumber = *I2cNumber;

  check_expected (I2cBusNumber);
  check_expected (TargetAddress);
  check_expected (WriteLength);

  WriteBuffer = mock_type (UINT8*);
  return mock_type (EFI_STATUS);
}

/**
  Mock SerialIoI2cRead.

  @param[in]   PciCfgBase     Pci Config Base Address of specified I2C controller
                              @Note: If I2C Number is provided this parameter is ignored and can be left as 0
  @param[in]   I2cNumber      Optional: I2C controller applicable on bus 0 only
  @param[in]   TargetAddress  Address of the end point device
  @param[in]   ReadLength     Read length of the current transaction
  @param[in]   ReadBuffer     Pointer to data in read buffer
  @param[in]   TimeBudget     Time allotted to verify if I2C is enabled
  @param[in]   Frequency      Bus clock frequency (in kHz)
  @param[in]   TimingConfig   Optional: Pointer to struct passed down to override SDA Hold and SCL LCNT/HCNT settings
  @param[in]   Restart        Indication if restart is needed, if TRUE will Initialize I2C controller.
                              @Note: Restart is required in first transaction.

  @retval   EFI_INVALID_PARAMETER   Invalid length / buffer parameters were passed into this function
  @retval   EFI_TIMEOUT             Timeout occurred in I2C disable/enable or wrong ENST value
  @retval   EFI_DEVICE_ERROR        The verification failed
  @retval   EFI_SUCCESS             Read was successful
**/
EFI_STATUS
EFIAPI
SerialIoI2cRead (
  IN UINTN                 PciCfgBase,
  IN UINT8                 *I2cNumber  OPTIONAL,
  IN UINT32                TargetAddress,
  IN UINT32                ReadLength,
  IN UINT8                 *ReadBuffer,
  IN UINT64                TimeBudget,
  IN UINTN                 Frequency,
  IN UINTN                 *TimingConfig  OPTIONAL,
  IN BOOLEAN               Restart
  )
{
  UINT8 I2cBusNumber;

  I2cBusNumber = *I2cNumber;

  check_expected (I2cBusNumber);
  check_expected (TargetAddress);
  check_expected (ReadLength);

  ReadBuffer = mock_type (UINT8*);
  return mock_type (EFI_STATUS);
}

