/** @file
  Itss handle definition

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _ITSS_HANDLE_H_
#define _ITSS_HANDLE_H_

#include <ConfigBlock.h>
#include <InterruptConfig.h>
#include <IoApicConfig.h>
#include <RegisterAccess.h>

typedef struct _ITSS_HANDLE ITSS_HANDLE;

#define VALID_DEV_NO    0  ///< Device does not exist
#define VALID_DEV_YES   1  ///< Device exists

#define PCIE_RP_NO      0  ///< Device is not a PCIe root port
#define PCIE_RP_YES     1  ///< Device is a PCIe root port

#define INTERRUPT_NO    0  ///< Device doesn't generate interrupts
#define INTERRUPT_YES   1  ///< Device generates interrupts

#define USES_PIRQ_NO    0  ///< Device does not route its interrupt to PIRQ
#define USES_PIRQ_YES   1  ///< Device is under INTx->PIRQy mapping

#define INTP_NO_REQ     0  ///< There is no requirement regarding INTP programming
#define INTP_MUST_SET   1  ///< INTP is RWO and needs to be programmed
#define INTP_IS_INTA    2  ///< INTP is required to be set to INTA
#define INTP_IS_INTB    3  ///< INTP is required to be set to INTB
#define INTP_IS_INTC    4  ///< INTP is required to be set to INTC
#define INTP_IS_INTD    5  ///< INTP is required to be set to INTD

#define IRQ_UNIQUE_NO       0  ///< IRQx can be shareable for this device
#define IRQ_UNIQUE_SERIALIO 1  ///< SerialIo devices need unique IRQ
#define IRQ_UNIQUE_SCS      2  ///< SCS devices need unique IRQ
#define IRQ_UNIQUE_OTG      3  ///< OTG device need unique IRQ
#define IRQ_UNIQUE_ISH      4  ///< ISH device need unique IRQ
#define IRQ_UNIQUE_CAN      5  ///< CAN device need unique IRQ

//
// Structure for storing mapping between device number and PIRx register
//
typedef struct {
  UINT8   Device;
  UINT8   PirIndex;
} PCH_PIRX_REGISTER;

typedef struct {
  UINT32        Device            : 5;  ///< Device number
  UINT32        Function          : 3;  ///< Device function
  UINT32        ValidDevice       : 1;  ///< Flag which indicates if DxFy is a PCH device
  UINT32        IsPcieRp          : 1;  ///< Flag which indicates if DxFy is a PCIe Root port
  UINT32        UsesInterrupts    : 1;  ///< Flag indicating if device is using interrupts
  UINT32        UsesPirq          : 1;  ///< Flag indicating that device uses INTx->PIRQ mapping
  UINT32        IntPinRequirement : 3;  ///< Setting on how INTP reg should be treated
                                        ///
                                        /// 0 - IRQ can be shareable, other - for IRQ>23 it needs to be unique among all devices
                                        /// or if IRQ <=23 it needs to be unique only between devices with the same PortID
                                        ///
  UINT32        IrqUnique         : 3;
  UINT32        Rsvd              : 14; ///< Reserved
} PCH_DEV_INT_INFO;

/**
  Check for GPIO IRQ conflicts

  @param[in]  *ItssHandle  Pointer to ITSS handle
  @param[in]  *IrqUsage    Pointer to array that represents IRQs in use

  @retval      FALSE       No conflicts found with the GPIO IRQ configuration
  @retval      TRUE        A conflict was found with the GPIO IRQ configuration
**/
typedef
BOOLEAN
(EFIAPI *ITSS_CHECK_GPIO_CONFLICTS) (
  IN  ITSS_HANDLE               *ItssHandle,
  IN  UINT32                    *IrqUsage
  );

/**
  Configures the P2Sb IoAPIC Control Register

  @param[in]   *ItssHandle    Pointer to ITSS handle

  @retval      None
**/
typedef
VOID
(EFIAPI *ITSS_P2SB_CONFIGURE_IOAPIC) (
  IN  ITSS_HANDLE             *ItssHandle
  );

typedef struct {
  BOOLEAN                  LegacyIoLowLatency;
  UINT8                    UniqueIrqOverride;
  CONST PCH_DEV_INT_INFO   *PchDevIntInfoData;
  UINT32                   PchDevIntInfoSize;
  CONST PCH_PIRX_REGISTER  *ItssPirxRegData;
  UINT32                   ItssPirxRegDataSize;
  UINT32                   IoApicAddress;
} ITSS_PRIVATE_CONFIG;

typedef struct {
  ITSS_CHECK_GPIO_CONFLICTS          ItssCheckGpioConflicts;
  ITSS_P2SB_CONFIGURE_IOAPIC         ItssP2SbConfigureIoApic;
} ITSS_CALLBACKS;

struct  _ITSS_HANDLE {
  ITSS_PRIVATE_CONFIG   *ItssPrivateConfig;
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  PCH_IOAPIC_CONFIG     *IoApicConfig;
  REGISTER_ACCESS       *PciPcrAccess;
  REGISTER_ACCESS       *P2SbPcieConfigAccess;
  ITSS_CALLBACKS        *ItssCallBack;
};

#endif
