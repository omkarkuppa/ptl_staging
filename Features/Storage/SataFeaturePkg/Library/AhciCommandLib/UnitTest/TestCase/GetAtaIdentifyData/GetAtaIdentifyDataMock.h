/**@file
    Unit test case mock header for GetAtaIdentifyData.

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
#ifndef _GET_ATA_IDENTIFY_MOCK_H_
#define _GET_ATA_IDENTIFY_MOCK_H_

#include <Uefi.h>
#include <Base.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>

#include "AtaPassThruProtocolMock.h"

/**
  Get ATA identify data.

  @param[in]    AtaDevice           The pointer to the EFI_ATA_PASS_THRU_PROTOCOL.
  @param[in]    Port                Port number on the ATA controller
  @param[in]    PortMultiplierPort  Port multiplier port number on the ATA controller
  @param[out]   ControllerData      The buffer used to store the identify controller data.

  @retval       EFI_SUCCESS         Successfully get the identify controller data.
  @retval       EFI_DEVICE_ERROR    Fail to get the identify controller data.

**/
EFI_STATUS
GetAtaIdentifyData (
  IN EFI_ATA_PASS_THRU_PROTOCOL   *AtaDevice,
  IN UINT16                       Port,
  IN UINT16                       PortMultiplierPort,
  OUT ATA_IDENTIFY_DATA           *ControllerData
  );

/**
  Unit test for SataGetIdentify. GetIdentify is successful

  @param[in]  Context    An optional parameter from precondition.

  @retval  UNIT_TEST_PASSED             The Unit test has completed and the test
                                        case was successful.
  @retval  UNIT_TEST_ERROR_TEST_FAILED  A test case assertion has failed.

**/
UNIT_TEST_STATUS
EFIAPI
GetAtaIdentifyDataUnitTest (
  IN UNIT_TEST_CONTEXT  Context
  );

#endif /*_SATA_GET_IDENTIFY_MOCK_H_*/
