/**@file
    Unit test main of the Nvme Erase Dxe driver.

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

@par Specification Reference:
**/

#include "CleanUp/CleanUp.h"

//
// GetParentProtocol Header.
//
#include "TestCase/GetParentProtocol/GetParentProtocolMock.h"

//
// IsNvmeDevice Header.
//
#include "TestCase/IsNvmeDevice/IsNvmeDeviceMock.h"

//
// NvmeEraseBindingSupported Header.
//
#include "TestCase/NvmeEraseBindingSupported/NvmeEraseBindingSupportedMock.h"

//
// NvmeEraseEntryPoint Header.
//
#include "TestCase/NvmeEraseEntryPoint/NvmeEraseEntryPointMock.h"

//
// NvmeIdentifyController Header.
//
#include "TestCase/NvmeIdentifyController/NvmeIdentifyControllerMock.h"

//
// NvmeFormatNvmCommand Header.
//
#include "TestCase/NvmeFormatNvmCommand/NvmeFormatNvmCommandMock.h"

//
// NvmeDataOverwrite Header.
//
#include "TestCase/NvmeDataOverwrite/NvmeDataOverwriteMock.h"

//
// NvmeDataBlockErase Header.
//
#include "TestCase/NvmeDataBlockErase/NvmeDataBlockEraseMock.h"

//
// NvmeDataCryptoErase Header.
//
#include "TestCase/NvmeDataCryptoErase/NvmeDataCryptoEraseMock.h"

//
// UpdateResult Header.
//
#include "TestCase/UpdateResult/UpdateResultMock.h"

//
// GetNvmeEraseResult Header.
//
#include "TestCase/GetNvmeEraseResult/GetNvmeEraseResultMock.h"

//
// NvmeEraseBlocks Header.
//
#include "TestCase/NvmeEraseBlocks/NvmeEraseBlocksMock.h"

//
// GetNvmeEstimateTime Header.
//
#include "TestCase/GetNvmeEstimateTime/GetNvmeEstimateTimeMock.h"

//
// SetNvmeEstimateTime Header.
//
#include "TestCase/SetNvmeEstimateTime/SetNvmeEstimateTimeMock.h"