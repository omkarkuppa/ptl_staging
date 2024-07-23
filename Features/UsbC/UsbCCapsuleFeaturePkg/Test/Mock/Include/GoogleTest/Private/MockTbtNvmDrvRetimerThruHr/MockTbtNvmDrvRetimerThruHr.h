/** @file
  Google Test mocks for TbtNvmDrvRetimerThruHr.h

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

@par Specification Reference:

**/

#ifndef MOCK_TBT_NVM_DRV_RETIMER_THRU_HR_H_
#define MOCK_TBT_NVM_DRV_RETIMER_THRU_HR_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
  #include <Uefi.h>
  #include <Library/UefiLib.h>
  #include <Protocol/TbtNvmDrvHrProtocol.h>
  #include <Protocol/TbtNvmDrvDevice.h>
  #include <TbtNvmRetimer.h>

}

struct MockTbtNvmDrvRetimerThruHr {
  MOCK_INTERFACE_DECLARATION (MockTbtNvmDrvRetimerThruHr);

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    WriteIecs,
    (IN TBT_RETIMER      *This,
     IN UINT8            RegNum,
     IN UINT32           *DataPtr,
     IN UINT8            Length)
    );

  MOCK_FUNCTION_DECLARATION (
    TBT_STATUS,
    ReadIecs,
    (IN  TBT_RETIMER    *This,
     IN  UINT8          RegNum,   // TODO: RegNum type should be enum
     IN  UINT32         DataLength,
     OUT UINT32         *DataPtr)
    );

  MOCK_FUNCTION_DECLARATION (
    TBT_RETIMER *,
    TbtNvmDrvRetimerThruHrCtor,
    (IN UINT8          FirmwareType,
     IN PCIE_BDF       *TbtDmaPcieBdf,
     IN TBT_PORT       TbtPort,
     IN UINT32         CascadedRetimerIndex,
     IN PCIE_RP_CONFIG *PcieRpConfig,
     IN FORCE_PWR_HR   ForcePwrFunc OPTIONAL)
    );

  MOCK_FUNCTION_DECLARATION (
    VOID,
    Dtor,
    (IN TBT_RETIMER *This)
    );
};

#endif
