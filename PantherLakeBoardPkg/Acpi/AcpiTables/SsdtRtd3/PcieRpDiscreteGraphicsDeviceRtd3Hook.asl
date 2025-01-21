/** @file
  ACPI RTD3 Hook SSDT Library for Generic Pcie Rp with End Point as Discrete Graphics Device.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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

External(DGDS)
External(\_SB.PC00.RP12.PXSX.PEGB.PEGE.DEDP, DeviceObj)
External(BCLP)
External(ASLC)

  // DG Op Region
  OperationRegion (DGMR, SystemMemory, DGOP, 0x2000)
  Field (DGMR, AnyAcc, NoLock, Preserve)
  {
    //
    // OpRegion Header starts at 0x000
    //
    Offset (0x60),
    PCON, 32,  // Platform Configuration start at 0x60h
    Offset(0x100),
    DRDY, 32,      // Driver readiness (ACPI notification)
    CSTS, 32,      // Notification status
    CEVT, 32,      // Current event
    Offset(0x120),
    DIDL, 32,      // Supported display device ID list
    DDL2, 32,      // Allows for 8 devices
    DDL3, 32,
    DDL4, 32,
    DDL5, 32,
    DDL6, 32,
    DDL7, 32,
    DDL8, 32,
    CPDL, 32,      // Currently present display list
    CPL2, 32,      // Allows for 8 devices
    CPL3, 32,
    CPL4, 32,
    CPL5, 32,
    CPL6, 32,
    CPL7, 32,
    CPL8, 32,
    CADL, 32,      // Currently active display list
    CAL2, 32,      // Allows for 8 devices
    CAL3, 32,
    CAL4, 32,
    CAL5, 32,
    CAL6, 32,
    CAL7, 32,
    CAL8, 32,
    NADL, 32,      // Next active display list
    NDL2, 32,      // Allows for 8 devices
    NDL3, 32,
    NDL4, 32,
    NDL5, 32,
    NDL6, 32,
    NDL7, 32,
    NDL8, 32,
    ASLP, 32,      // ASL sleep timeout
    TIDX, 32,      // Toggle table index
    CHPD, 32,      // Current hot plug enable indicator
    CLID, 32,      // Current lid state indicator
    CDCK, 32,      // Current docking state indicator
    SXSW, 32,      // Display switch notify on resume
    EVTS, 32,      // Events supported by ASL (diag only)
    CNOT, 32,      // Current OS notifications (diag only)
    NRDY, 32,
    //
    //Extended DIDL list
    //
    DDL9, 32,
    DD10, 32,
    DD11, 32,
    DD12, 32,
    DD13, 32,
    DD14, 32,
    DD15, 32,
    //
    //Extended Currently attached Display Device List  CPD2
    //
    CPL9, 32,
    CP10, 32,
    CP11, 32,
    CP12, 32,
    CP13, 32,
    CP14, 32,
    CP15, 32,

    Offset(0x300),
        , 32,      // Driver readiness (power conservation)
    ASLC, 32,      // ASLE interrupt command/status
        , 32,      // Technology enabled indicator
        , 32,      // Current ALS illuminance reading
    BCLP, 32,      // Backlight brightness
        , 32,      // Panel fitting state or request
    CBLV, 32,      // Current brightness level
    BCLM, 320,     // Backlight brightness level duty cycle mapping table

    Offset (0x3C6), // VRSR start at 0x3C6
    VRSR, 1,

    Offset(0x1C00),
    PHED, 32,      // Panel Header
    BDDC, 2048,    // Panel EDID (Max 256 bytes)
  }


  // Get DG VRAMSR Enable Status
  // Check if BIOS supports VRAM self refersh and OS/driver enabled self refersh.
  // If VRAMSR is enable, OS/Device need to Self Refresh. So Device Core Power Can't be Removed.
  // So return 0 in this case to show Power Removal Request is denied.
  // If VRAMSR is disable then it is safe to Remove/Disable Power.
  // Input: VOID
  //
  // @return 1 if it is Safe to Remove/Disable Power. 0 Not allow for Power Removal.
  //
  Method (DVES, 0, Serialized) {
    ADBG_PRINT_PCIE_RP_INFO_AFTER ("DG: DVES Entry For ")
    ADBG (Concatenate ("DG: POFF PCON=", ToHexString (PCON)))
    ADBG (Concatenate ("DG: POFF VRSR = ", ToHexString (VRSR)))

    // Check if DG is Supported or not.
    If(LEqual (DGBA, 0)) {
      // DG is not Supported. No need to check for Permission. Safe To remove Power.
      ADBG_PRINT_PCIE_RP_INFO_AFTER ("DG: not Supported For ")
      Return (1) // Core power Removal Request is Accepted
    }

    // BIOS supports VRAM self refersh and OS/driver enabled self refersh
    If (LAnd (LEqual (And(0x1800, PCON), 0x1800), VRSR)) {
      ADBG_PRINT_PCIE_RP_INFO_AFTER ("DG: VRAMSR enabled for ")
      Return (0) // Core Power removal Request is denied.
    }

    // Either BIOS does not support self-refresh or OS/driver does not enabled self-refresh
    ADBG_PRINT_PCIE_RP_INFO_AFTER ("DG: VRAMSR is not enabled for ")
    Return (1) // Core power Removal Request is Accepted
  }


  //
  // This method is to be called when a graphics device
  // notification is required (display switch hotkey, etc).
  // Arg0 = Current event type
  // Arg1 = Notification type
  //
  Method (DNOT, 2)
  {
    Store(Arg0, CEVT) // Set up the current event value
    Store(3, CSTS) // CSTS=BIOS dispatched an event

    If(LAnd(LEqual(CHPD, 0), LEqual(Arg1, 0))) // Do not re-enum if driver supports hotplug
    {
      //
      // Re-enumerate the DG Device for non-XP operating systems.
      //
      Notify(\_SB.PC00.RP12.PXSX, Arg1)
    }

    Notify(\_SB.PC00.RP12.PXSX,0x80)

    Return(0x0) // Return success
  }

  //
  // This method is to handle a lid event for DGPU LFP device.
  //
  Method (DLID, 1)
  {
    If (LEqual(Arg0,1))
    {
      Store(3,CLID)
    }
    Else
    {
      Store(Arg0, CLID)
    }

    if (DNOT(2, 0)) {
      Or (CLID, 0x80000000, CLID)
      Return (1) // Return Fail
    }

    Return (0) // Return Pass
  }

  //
  // This menthod is to called to set a brightness level for DGPU LFP device.
  //
  Method (DINT, 2)
  {
    If(LEqual(Arg0, 1)) // Arg0=1, so set the backlight brightness.
    {
      Store(Divide(Multiply(Arg1, 255), 100), BCLP) // Convert from percent to 0-255.
      Or(BCLP, 0x80000000, BCLP) // Set the valid bit.
      Store(2, ASLC) // Store "Backlight control event" to ASLC[31:1]
    }
    Return(0x0) // Return success
  }

  //
  // This method is to send backlight notifications to the DGPU LFP device
  //
  Method (HBRT, 1 , Serialized)
  {
    //
    // Send backlight notifications to the DGPU LFP device.
    //

    If(And(4,DGDS))
    {
      If(LEqual(Arg0,4))
      {
        Notify(\_SB.PC00.RP12.PXSX.PEGB.PEGE.DEDP,0x87)
      }
      If(LEqual(Arg0,3))
      {
        Notify(\_SB.PC00.RP12.PXSX.PEGB.PEGE.DEDP,0x86)
      }
    }
  }