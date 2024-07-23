/** @file
  Google Test mocks for UsbcRetimerProtocolLib

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

#ifndef MOCK_USB_C_RETIMER_PROTOCOL_LIB_H_
#define MOCK_USB_C_RETIMER_PROTOCOL_LIB_H_

#include <Library/GoogleTestLib.h>
#include <Library/FunctionMockLib.h>
extern "C" {
#include <Uefi.h>
#include <Protocol/UsbCRetimerProtocol.h>
}

struct MockUsbcRetimerProtocolLib {
  MOCK_INTERFACE_DECLARATION (MockUsbcRetimerProtocolLib);

  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    UsbcRetimerProtocol_Drive,
    (IN USBC_RETIMER_PROTOCOL      *This,
     IN EFI_GUID                   RetimerGuid)
     );
  MOCK_FUNCTION_DECLARATION (
    EFI_STATUS,
    UsbcRetimerProtocol_Restore,
    (IN USBC_RETIMER_PROTOCOL      *This,
     IN EFI_GUID                   RetimerGuid)
     );
};

#endif
