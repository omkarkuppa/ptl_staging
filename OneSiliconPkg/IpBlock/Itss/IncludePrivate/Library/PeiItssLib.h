/** @file
  Header file for private PEI ITSS Lib.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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
#ifndef _PEI_ITSS_LIB_H_
#define _PEI_ITSS_LIB_H_

#include <ItssHandle.h>

typedef struct _SI_POLICY_STRUCT SI_POLICY_PPI;

/**
  Retrieve interrupt configuration for a given PCH device (DxFy)

  @param[in]  SiPolicy  Policy
  @param[in]  Device    Device number
  @param[in]  Function  Function number
  @param[out] IntPin    Interrupt pin: INTA-INTD (see PCH_INT_PIN for reference)
  @param[out] ApicIrq   APIC IRQ number
**/
VOID
ItssGetDevIntConfig (
  IN  CONST SI_POLICY_PPI  *SiPolicy,
  IN  UINT8                Device,
  IN  UINT8                Function,
  OUT UINT8                *IntPin,
  OUT UINT8                *ApicIrq
  );

/**
  Retrieve interrupt pin for a given PCH device (DxFy)

  @param[in]  SiPolicy  Policy
  @param[in]  Device    Device number
  @param[in]  Function  Function number

  @retval Interrupt pin: INTA-INTD (see PCH_INT_PIN for reference)
**/
UINT8
ItssGetDevIntPin (
  IN  CONST SI_POLICY_PPI  *SiPolicy,
  IN  UINT8                Device,
  IN  UINT8                Function
  );

/**
  Retrieve IRQ for Gpio Services

  @param[in]  SiPolicy  Policy

  @retval GpioIrq
**/
UINT8
ItssGetGpioDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  );

/**
  Retrieve IRQ for TCO

  @param[in]  SiPolicy  Policy

  @retval TcoIrq     TCO IRQ. If 0 TCO interrupt is not to be enabled
**/
UINT8
ItssGetTcoDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  );

/**
  Retrieve IRQ for PMC SCI

  @param[in]  SiPolicy  Policy

  @retval PmcSciIrq
**/
UINT8
ItssGetPmcSciDevIntConfig (
  IN CONST SI_POLICY_PPI  *SiPolicy
  );

/**
  Configures interrupts polarity for PCI devices.

  @param[in] ItssHandle            ITSS handle
  @param[in] PchInterruptConfig    PCH interrupt Configuration
**/
VOID
SetInterruptsPolarityForPchInternalDevices (
  IN ITSS_HANDLE             *ItssHandle,
  IN PCH_INTERRUPT_CONFIG    *InterruptConfig
  );

/**
  Configures PCH interrupts

  @param[in] SiPolicy          Policy
**/
VOID
ItssConfigureInterrupts (
  IN ITSS_HANDLE  *ItssHandle
  );

/**
  Initialize IOAPIC according to IoApicConfig policy of the PCH
  Policy PPI

  @param[in] SiPolicyPpi          The SI Policy PPI instance

**/
VOID
ItssInitIoApic (
  IN ITSS_HANDLE  *ItssHandle
  );


/**
  Write to mask registers of Controller and Target 8259 PICs.
**/
VOID
ItssMask8259Interrupts (
  VOID
  );

/**
  Configure Message Decoder Control register
    INTDDEN set to 1: Enable interrupts from unknown sources
    INTDDPIR set to 1101: Shift to use PIR13

  @param[in] ItssHandle          Pointer to Itss handle

**/
VOID
ItssMsgDecoderConfig (
  IN ITSS_HANDLE  *ItssHandle
  );

#endif // _PEI_ITSS_LIB_H_
