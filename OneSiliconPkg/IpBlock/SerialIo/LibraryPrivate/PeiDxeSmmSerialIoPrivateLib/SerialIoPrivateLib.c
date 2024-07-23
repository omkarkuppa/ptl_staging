/** @file
  Serial IO Private Lib implementation.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/SerialIoAccessLib.h>
#include <Library/SerialIoPrivateLib.h>
#include <Library/PciSegmentLib.h>
#include <Register/PchRegs.h>
#include <Register/SerialIoRegs.h>
#include <Include/PcieRegs.h>
#include <IndustryStandard/Pci30.h>

UINTN
GetReservedSerialIoMmio (
  VOID
  );

/**
  Checks if higher functions are enabled.
  Used for Function 0 Serial Io Device disabling

  @param[in] DeviceNum       Device Number

  @retval TRUE               At least one higher function device is enabled
          FALSE              Higher functions are disabled
**/
BOOLEAN
SerialIoHigherFunctionsEnabled (
  IN UINT8                    DeviceNum
  )
{
  UINT8 FuncNum;
  //
  // Check all other func devs(1 to 7) status except func 0.
  //
  for (FuncNum = 1; FuncNum <= PCI_MAX_FUNC; FuncNum++) {
    if (PciSegmentRead16 (PCI_SEGMENT_LIB_ADDRESS (DEFAULT_PCI_SEGMENT_NUMBER_PCH,
                                                   DEFAULT_PCI_BUS_NUMBER_PCH,
                                                   DeviceNum,
                                                   FuncNum,
                                                   PCI_DEVICE_ID_OFFSET)
                          ) != 0xFFFF) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
  Places SerialIo device in D3

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD3 (
  IN UINT64                    PciCfgBase
  )
{
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  PwrMgmControlStsReg;

  if (PciCfgBase < GetReservedSerialIoMmio ()) {
    PwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    PwrMgmControlStsReg.Fields.PwrState = 0x3; // Place Device in D3 State
    PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
  } else {
    PwrMgmControlStsReg.Data32 = MmioRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    PwrMgmControlStsReg.Fields.PwrState = 0x3; // D3 State
    MmioWrite32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
    //
    // Reading back value after write to ensure bridge observes the BAR1 write access
    //
    MmioRead8 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
  }
}

/**
  Places SerialIo device in D0

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoSetD0 (
  IN UINT64                    PciCfgBase
  )
{
  SERIAL_IO_PWRMG_CONTROL_AND_STATUS  PwrMgmControlStsReg;

  if (PciCfgBase < GetReservedSerialIoMmio ()) {
    PwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    PwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
    PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
  } else {
    PwrMgmControlStsReg.Data32 = MmioRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
    PwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
    MmioWrite32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS, PwrMgmControlStsReg.Data32);
    //
    // Reading back value after write to ensure bridge observes the BAR1 write access
    //
    MmioRead32 ((UINTN) PciCfgBase + R_SERIAL_IO_CFG_PMECTRLSTATUS);
  }
}

/**
  Allows memory access

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] Hidden           Mode that determines access type

**/
VOID
SerialIoEnableMse (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   Hidden
  )
{
  if (Hidden) {
    MmioOr16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
    //
    // Reading back value after write to ensure bridge observes the BAR1 write access
    //
    MmioRead16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET);
  } else {
    PciSegmentOr16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE);
  }
}

/**
  Disable SerialIo memory access

  @param[in] PciCfgBase       Pci Config Offset

**/
VOID
SerialIoDisableMse (
  IN UINT64                    PciCfgBase
  )
{
  PciSegmentAnd16 ((UINTN) PciCfgBase + PCI_COMMAND_OFFSET, (UINT16) ~EFI_PCI_COMMAND_MEMORY_SPACE);
}

/**
  Disable SerialIo memory encoding
  Designated for Pci modes

  @param[in] PciCfgBase       Pci Config Offset
  @param[in] RemoveTempBar    Remove temporary mem base address or not

**/
VOID
SerialIoMmioDisable (
  IN UINT64                    PciCfgBase,
  IN BOOLEAN                   RemoveBar
  )
{
  SerialIoDisableMse (PciCfgBase);

  if (RemoveBar == TRUE) {
    PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_BAR,  0x0);
    PciSegmentWrite32 (PciCfgBase + R_SERIAL_IO_CFG_BAR_HIGH,  0x0);
  }
}
