/**@file

  Serial IO UART Common ACPI definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

  //
  //  Checks if UART specific DMA check _DSM is called
  //
  //  @param[in]  Arg0      UART mode
  //
  //  @retval               1 if true, 0 otherwise
  //
  Method (UDSM, 1 ,Serialized) {
    If (LEqual (Arg0, ToUUID ("F7AF8347-A966-49FE-9022-7A9DEEEBDB27"))) {
      return (1)
    }
    return (0)
  }

  //
  //  Uart DMA check _DSM
  //
  //  @param[in]  Arg0      Function Index
  //  @param[in]  Arg1      DMA/PIO support
  //
  //  @retval    case 0     Buffer with coresponding data
  //             case 1     Integer [0:PIO; 1:DMA]
  //             other      Empty Buffer
  //
  Method (UDMA, 2, Serialized) {
    Switch (ToInteger (Arg0)) {
      Case (0) {
        Return (ToBuffer (0x3)) // Supports Function 1.
      }
      // Function 1, Return UART operating mode [0:PIO; 1:DMA]
      Case (1) {
        Return (ToInteger (Arg1))
      }
    } // End Switch statement
    Return (ToBuffer (0x0))
  }
