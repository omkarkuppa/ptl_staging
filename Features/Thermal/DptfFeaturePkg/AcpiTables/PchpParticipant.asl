/** @file
  Intel ACPI Reference Code for Intel(R) Dynamic Tuning

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

#include <Register/PmcRegs.h>
#include <Register/TcoRegs.h>

External(IPCS, MethodObj)  // IPC Command Send (Read/Write)

Scope(\_SB.IETM)
{
  Device(TPCH)  // PCH FIVR Participant
  {
    Name(_UID, "TPCH")
    Method(_HID)
    {
      Return(\_SB.PLDT.GHID(_UID))  // Intel(R) Dynamic Tuning Technology platform power device
    }

    Name(_STR, Unicode("Intel PCH FIVR Participant"))
    Name(PTYP, 0x05)

    // _STA (Status)
    //
    // This object returns the current status of a device.
    //
    // Arguments: (0)
    //   None
    // Return Value:
    //   An Integer containing a device status bitmap:
    //    Bit 0 - Set if the device is present.
    //    Bit 1 - Set if the device is enabled and decoding its resources.
    //    Bit 2 - Set if the device should be shown in the UI.
    //    Bit 3 - Set if the device is functioning properly (cleared if device failed its diagnostics).
    //    Bit 4 - Set if the battery is present.
    //    Bits 5-31 - Reserved (must be cleared).
    //
    Method(_STA)
    {
      If(LEqual(\_SB.PCHE,1)) {
        Return(0x0F)
      } Else {
        Return(0x00)
      }
    }

    // RFC0 (Set PCH FIVR RFI value)
    //
    // Set PCH FIVR RFI Value
    //
    // Arguments:
    //   Arg0: PCH FIVR RFI Control0 Setting Value.
    // Return Value:
    //   An empty package as w/a to resolve EVAL failure in DPTF driver ESIF shell
    //
    Method(RFC0, 1, Serialized)
    {
      //
      // Need to perform Logic Write (subcommand 0x01)
      //
      IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_WRITE, 0x8, V_PMC_PWRM_IPC_SUBCMD_RFI_CTRL0_LOGIC, Arg0, 0, 0)
      Return(Package(){0})
    }

    // RFC1 (Set PCH FIVR RFI value)
    //
    // Set PCH FIVR RFI Value
    //
    // Arguments:
    //   Arg0: PCH FIVR RFI Control4 Setting Value.
    // Return Value:
    //   An empty package as w/a to resolve EVAL failure in DPTF driver ESIF shell
    //
    Method(RFC1, 1, Serialized)
    {
      //
      // Need to perform Logic Write (subcommand 0x01)
      //
      IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_WRITE, 0x8, V_PMC_PWRM_IPC_SUBCMD_RFI_CTRL4_LOGIC, Arg0, 0, 0)
      Return(Package(){0})
    }

    // SEMI (Set PCH FIVR EMI value)
    //
    // Set PCH FIVR EMI Value
    //
    // Arguments:
    //   Arg0: PCH FIVR EMI Control Setting Value.
    // Return Value:
    //   An empty package as w/a to resolve EVAL failure in DPTF driver ESIF shell
    //
    Method(SEMI, 1, Serialized)
    {
      //
      // Need to perform Logic Write (subcommand 0x01)
      //
      IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_WRITE, 0x8, V_PMC_PWRM_IPC_SUBCMD_EMI_CTRL0_LOGIC, Arg0, 0, 0)
      Return(Package(){0})
    }

    // PKG (Truncate Package recieved from IPC)
    //
    // Arguments:
    //   Arg0: Package returned from the IPCS read call from the Pmc.
    // Return Value:
    //   Return Package with just the Status and ReadBuf0.
    //   Status returns 0 for success and 2 for device error
    //
    Method(PKGC, 1, Serialized)
    {
      //
      // Convert to Pchp Return Package
      //
      Name(PPKG, Package (2) {
        Zero, // Status (DWORD)
        Zero  // ReadBuf0 (DWORD)
      })
      Store(DerefOf(Index(Arg0, 0)), Index(PPKG, 0))
      Store(DerefOf(Index(Arg0, 1)), Index(PPKG, 1))
      Return(PPKG)
    }

    // GFC0 (Get PCH FIVR RFI_CONTROL0 register)
    //
    // Get PCH FIVR RFI_CONTROL0 register
    //
    // Arguments:
    //   None.
    // Return Value:
    //   Package containing two ACPI integers- IPCS Status & PCH FIVR RFI_CONTROL0 register value
    //
    Method(GFC0, 0, Serialized)
    {
      //
      // Need to perform Logic Read (subcommand 0x00)
      //
      Store(IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_READ, 0x8, V_PMC_PWRM_IPC_SUBCMD_RFI_CTRL0_LOGIC, 0, 0, 0), Local0)
      Store(PKGC(Local0), Local1)
      Return(Local1)
    }

    // GFC1 (Get PCH FIVR RFI_CONTROL4 register)
    //
    // Get PCH FIVR RFI_CONTROL4 register
    //
    // Arguments:
    //   None.
    // Return Value:
    //   Package containing two ACPI integers- IPCS Status & PCH FIVR RFI_CONTROL4 register value
    //
    Method(GFC1, 0, Serialized)
    {
      //
      // Need to perform Logic Read (subcommand 0x00)
      //
      Store(IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_READ, 0x8, V_PMC_PWRM_IPC_SUBCMD_RFI_CTRL4_LOGIC, 0, 0, 0), Local0)
      Store(PKGC(Local0), Local1)
      Return(Local1)
    }

    // GEMI (Get PCH FIVR EMI value)
    //
    // Get PCH FIVR EMI Value
    //
    // Arguments:
    //   None.
    // Return Value:
    //   Package containing two ACPI integers- IPCS Status & PCH FIVR EMI Control Setting Value.
    //
    Method(GEMI, 0, Serialized)
    {
      //
      // Need to perform Logic Read (subcommand 0x00)
      //
      Store(IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_READ, 0x8, V_PMC_PWRM_IPC_SUBCMD_EMI_CTRL0_LOGIC, 0, 0, 0), Local0)
      Store(PKGC(Local0), Local1)
      Return(Local1)
    }

    // GFFS (Get PCH FIVR FFFC_FAULT_STATUS register)
    //
    // Get PCH FIVR FFFC_FAULT_STATUS register
    //
    // Arguments:
    //   None.
    // Return Value:
    //   Package containing two ACPI integers- IPCS Status & PCH FIVR FFFC_FAULT_STATUS register value
    //
    Method(GFFS, 0, Serialized)
    {
      //
      // Need to perform Logic Read (subcommand 0x00)
      //
      Store(IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_READ, 0x8, V_PMC_PWRM_IPC_SUBCMD_FFFC_FAULT_STATUS, 0, 0, 0), Local0)
      Store(PKGC(Local0), Local1)
      Return(Local1)
    }

    // GFCS (Get PCH FIVR FFFC_RFI_STATUS register)
    //
    // Get PCH FIVR FFFC_RFI_STATUS register
    //
    // Arguments:
    //   None.
    // Return Value:
    //   Package containing two ACPI integers- IPCS Status & PCH FIVR FFFC_RFI_STATUS register value
    //
    Method(GFCS, 0, Serialized)
    {
      //
      // Need to perform Logic Read (subcommand 0x00)
      //
      Store(IPCS(V_PMC_PWRM_IPC_CMD_COMMAND_FIVR, V_PMC_PWRM_IPC_CMD_CMD_ID_FIVR_READ, 0x8, V_PMC_PWRM_IPC_SUBCMD_FFFC_RFI_STATUS, 0, 0, 0), Local0)
      Store(PKGC(Local0), Local1)
      Return(Local1)
    }

  } // End TPCH device
} // End Scope (\_SB)
