/** @file
  Header file for TCSS Init Lib.

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
#ifndef _LPSS_INIT_LIB_H_
#define _LPSS_INIT_LIB_H_

#include <Uefi/UefiBaseType.h>
#include <RegisterAccess.h>

typedef struct _LPSS_DEVICE LPSS_DEVICE;

typedef enum {
  LpssFlagForceClockOn = 0x1,
} LPSS_FLAGS;

typedef enum {
  IrqModeAcpi,
  IrqModePci
} IRQ_MODE;

typedef enum {
  /*
  **Sequence for Disabled Mode**
    1. Device is placed in D3
    2. Gpio configuration is skipped
    3. Device will be disabled in PSF
      !important! If given device is Function 0 and other higher functions on given device
                  are enabled, PSF disabling is skipped. PSF default will remain and device PCI CFG Space will still be visible.
                  This is needed to allow PCI enumerator access functions above 0 in a multifunction device.
  */
  ModeDisabled,

  /*
  **Sequence for PCI Mode**
    1. Device will be enabled in PSF
    2. Disable BAR1 register in Bridge
    3. Disable BAR1 in PSF
    4. PCI Enumeration -> BAR0 is programmed and enabled (only BAR0 will be enabled)
    5. Write Remap Register with BAR value
    6. Gpio pin configuration in native mode
  */
  ModePci,

  /*
  **Sequence for ACPI Mode**
    1. PCI Enumeration -> BAR0 is programmed
    2. PCI Enumeration-> BAR1 is programmed
      *  BAR 0 MUST always be enumerated in order for proper operation of LPSS
    3. Requirement that MSE=1 and Device is in D0 state
    4. Program Bridge in ACPI mode -> Turn OFF PCI config space
    5. Program PSF in ACPI mode -> Turn OFF PCI config space
    6. Write Remap register with BAR value
    7. Gpio pin configuration in native mode
    @note
         If this controller is located at function 0 and it's mode is set to hidden it will not be visible in the PCI space.
  */
  ModeAcpi,

  /*
  **Sequence for COM Mode**
    It is sub-ModeAcpi mode, designated for UART 16550/PNP0501 compatible COM device
    1. Gpio pin configuration in native mode for each assigned pin
    2. Device disabled in the PSF
    3. Both BAR0 and BAR1 are enabled, BAR1 becomes devices Pci Config Space
    4. BAR0 assigned from the global PCH reserved memory range, reported as motherboard resource by SIRC
  */
  ModeCom,

  /*
  **Sequence for Skip Mode**
    All the initialization will be skipped
  */
  ModeSkipInit
} LPSS_DEV_MODE;

/**
  Disable LPSS Device in PSF

  @param[in] InterfaceNumber  Interface number
**/
typedef
VOID
(*LPSS_PSF_DISABLE_DEVICE) (
  IN UINT32  InterfaceNumber
  );

/**
  Hide LPSS Device in PSF

  @param[in] InterfaceNumber  Interface number
**/
typedef
VOID
(*LPSS_PSF_HIDE_DEVICE) (
  IN UINT32  InterfaceNumber
  );

/**
  Enable BAR 1 in PSF

  @param[in] InterfaceNumber  Interface number
**/
typedef
VOID
(*LPSS_PSF_BAR1_ENABLE) (
  IN UINT32  InterfaceNumber
  );

struct _LPSS_DEVICE {
  /*
  Name of the device
  */
  CHAR8             *Name;

  /*
  Interface index in the Controller Type
  UART 0, 1, 2 etc.
  I2C 0, 1, 2 etc.
  */
  UINT8             InterfaceIndex;

  /*
  PCI Configuration and Control register offset
  in LPSS Subsystem PCR space assigned for this device
  */
  UINT32            PciCfgCtrlOffset;

  /*
  The device Mode assigned by the user
  */
  LPSS_DEV_MODE     Mode;

  /*
  Interrrupt Pin assigned by the user
  */
  UINT8             InterruptPin;

  /*
  IRQ assigned by the user
  */
  UINT8             Irq;

  /*
  PCIe Device Number
  Requred for Function 0 disable handling
  */
  UINT8             PciDev;

  /*
  PCIe Function Number
  Requred for Function 0 disable handling
  */
  UINT8             PciFun;

  /*
  Pointer to PSF Disable for this LPSS device
  */
  LPSS_PSF_DISABLE_DEVICE PsfDisable;

  /*
  Pointer to PSF PCI CFG Hide for this LPSS device
  */
  LPSS_PSF_HIDE_DEVICE    PsfPciCfgHide;

  /*
  Pointer to PSF PCI CFG Hide for this LPSS device
  */
  LPSS_PSF_BAR1_ENABLE    PsfBar1Enable;

  /*
  Pointer to next device on the list
  */
  LPSS_DEVICE       *Next;
};

typedef struct {
  /*
  LPSS feature flags
  */
  UINT32            Flags;

  /*
  LPSS Subsystem registers access protocol
  for Private Configuration Registers (PCR) space
   */

  REGISTER_ACCESS   *PcrAccess;
  /*
  List of devices integrated in LPSS Subsytem
  */
  LPSS_DEVICE       *Devices;
} LPSS_SUBSYSTEM;

/**
  Initializes LPSS subsystem

  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
**/
VOID
LpssInit (
  IN LPSS_SUBSYSTEM   *LpssSubsystem
  );

/**
  Function configures LPSS devices disable mode
  in Primary Scalable Fabric (PSF)
  The function uses upper layer (FRU/SOC) function pointer
  to access PSF registers.

  @param[in] LpssSubsystem   Pointer to LPSS Subsystem structure
  @param[in] PciSegment      PCI Segment assigned for LPSS
  @param[in] PciBus          PCI Bus for assigned LPSS
**/
VOID
LpssFabricFunctionDisable (
  LPSS_SUBSYSTEM  *LpssSubsystem,
  UINT8           PciSegment,
  UINT8           PciBus
  );

#endif //_LPSS_INIT_LIB_H_
