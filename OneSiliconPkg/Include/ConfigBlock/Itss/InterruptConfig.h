/** @file
  Interrupt policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _INTERRUPT_CONFIG_H_
#define _INTERRUPT_CONFIG_H_

#include <ConfigBlock.h>

#define PCH_INTERRUPT_CONFIG_REVISION 1
extern EFI_GUID gInterruptConfigGuid;

#pragma pack (push,1)

//
// --------------------- Interrupts Config ------------------------------
//
typedef enum {
  PchNoInt,        ///< No Interrupt Pin
  PchIntA,
  PchIntB,
  PchIntC,
  PchIntD
} PCH_INT_PIN;

///
/// The PCH_DEVICE_INTERRUPT_CONFIG block describes interrupt pin, IRQ and interrupt mode for PCH device.
///
typedef struct {
  UINT8        Device;                  ///< Device number
  UINT8        Function;                ///< Device function
  UINT8        IntX;                    ///< Interrupt pin: INTA-INTD (see PCH_INT_PIN)
  UINT8        Irq;                     ///< IRQ to be set for device.
} PCH_DEVICE_INTERRUPT_CONFIG;

#define PCH_MAX_DEVICE_INTERRUPT_CONFIG 128       ///< Number of all PCH devices
#define PCH_MAX_PXRC_CONFIG               8       ///< Number of PXRC registers in ITSS
#define PCH_MAX_ITSS_IPC_REGS             4       ///< Number of IPC registers in ITSS
#define PCH_MAX_ITSS_IRQ_NUM            120       ///< Maximum number of IRQs


///
/// The PCH_INTERRUPT_CONFIG block describes interrupt settings for PCH.
///
typedef struct {
  CONFIG_BLOCK_HEADER          Header;                                          ///< Config Block Header
  UINT8                        NumOfDevIntConfig;                               ///< Number of entries in DevIntConfig table
  UINT8                        Rsvd0[3];                                        ///< Reserved bytes, align to multiple 4.
  PCH_DEVICE_INTERRUPT_CONFIG  DevIntConfig[PCH_MAX_DEVICE_INTERRUPT_CONFIG];   ///< Array which stores PCH devices interrupts settings
  UINT8                        GpioIrqRoute;                                    ///< Interrupt routing for GPIO. Default is <b>14</b>.
  UINT8                        SciIrqSelect;                                    ///< Interrupt select for SCI. Default is <b>9</b>.
  UINT8                        TcoIrqSelect;                                    ///< Interrupt select for TCO. Default is <b>9</b>.
  UINT8                        TcoIrqEnable;                                    ///< Enable IRQ generation for TCO. <b>0: Disable</b>; 1: Enable.
} PCH_INTERRUPT_CONFIG;

#pragma pack (pop)

#endif // _INTERRUPT_CONFIG_H_
