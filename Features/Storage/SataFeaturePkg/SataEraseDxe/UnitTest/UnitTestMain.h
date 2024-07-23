/**@file
    Unit test main of the Sata Erase Dxe driver.

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
// IsSataDevice Header.
//
#include "TestCase/IsSataDevice/IsSataDeviceMock.h"

//
// GetPortNum Header.
//
#include "TestCase/GetPortNum/GetPortNumMock.h"

//
// SataGetEraseTimeout Header.
//
#include "TestCase/SataGetEraseTimeout/SataGetEraseTimeoutMock.h"

//
// SataDataOverwrite Header.
//
#include "TestCase/SataDataOverwrite/SataDataOverwriteMock.h"

//
// SataDataCryptoErase Header.
//
#include "TestCase/SataDataCryptoErase/SataDataCryptoEraseMock.h"

//
// SataDataBlockErase Header.
//
#include "TestCase/SataDataBlockErase/SataDataBlockEraseMock.h"

//
// UpdateResult Header.
//
#include "TestCase/UpdateResult/UpdateResultMock.h"

//
// SataSwapAtaString Header.
//
#include "TestCase/SataSwapAtaString/SataSwapAtaStringMock.h"

//
// GetSataEraseResult Header.
//
#include "TestCase/GetSataEraseResult/GetSataEraseResultMock.h"

//
// SataEraseEntryPoint Header.
//
#include "TestCase/SataEraseEntryPoint/SataEraseEntryPointMock.h"

//
// GetSataEstimateTime Header.
//
#include "TestCase/GetSataEstimateTime/GetSataEstimateTimeMock.h"

//
// SetSataEstimateTime Header.
//
#include "TestCase/SetSataEstimateTime/SetSataEstimateTimeMock.h"
