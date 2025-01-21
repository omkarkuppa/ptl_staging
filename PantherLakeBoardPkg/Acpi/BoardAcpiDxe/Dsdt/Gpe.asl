/** @file
  ACPI DSDT table

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

  // General Purpose Events.  This Scope handles the Run-time and
  // Wake-time SCIs.  The specific method called will be determined by
  // the _Lxx value, where xx equals the bit location in the General
  // Purpose Event register (s).


  External (\_SB.PC00.MC, DeviceObj)
  External (\_SB.PC00.MC.D1F0, FieldUnitObj)
  External (\_SB.PC00.MC.D1F1, FieldUnitObj)
  External (\_SB.PC00.MC.D6F0, FieldUnitObj)
  External (\_GPE.AL6F, MethodObj)
  External (\_GPE.P0L6, MethodObj)
  External (\_GPE.P1L6, MethodObj)
  External (\_GPE.P2L6, MethodObj)
  External (\_GPE.P3L6, MethodObj)
  External (\_SB.PC00.TRP0.VDID)
  External (\_SB.PC00.TRP0.PDCX)
  External (\_SB.PC00.TRP0.PDSX)
  External (\_SB.PC00.TRP0.L0SE)
  External (\_SB.PC00.TRP1.VDID)
  External (\_SB.PC00.TRP1.PDCX)
  External (\_SB.PC00.TRP1.PDSX)
  External (\_SB.PC00.TRP1.L0SE)
  External (\_SB.PC00.TRP2.VDID)
  External (\_SB.PC00.TRP2.PDCX)
  External (\_SB.PC00.TRP2.PDSX)
  External (\_SB.PC00.TRP2.L0SE)
  External (\_SB.PC00.TRP3.VDID)
  External (\_SB.PC00.TRP3.PDCX)
  External (\_SB.PC00.TRP3.PDSX)
  External (\_SB.PC00.TRP3.L0SE)

  Scope (\_GPE)
  {
  // Note:
  // Originally, the two GPE methods below are automatically generated, but, for ASL code restructuring,
  // disabled the automatic generation and declare the ASL code here.
  //

  //
  // This PME event (PCH's GPE 69h) is received on one or more of the PCI Express* ports or
  // an assert PMEGPE message received via DMI
  //
  Method (_L69, 0, serialized) {
    \_SB.PC00.RP01.HPME ()
    \_SB.PC00.RP02.HPME ()
    \_SB.PC00.RP03.HPME ()
    \_SB.PC00.RP04.HPME ()
    \_SB.PC00.RP05.HPME ()
    \_SB.PC00.RP06.HPME ()

  }

  // PCI Express Hot-Plug caused the wake event.

  Method (_L61)
  {
    Add (L01C,1,L01C)  // Increment L01 Entry Count.

    P8XH (0,0x01)      // Output information to Port 80h.
    P8XH (1,L01C)


    // Check Root Port 1 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP01.VDID,0xFFFFFFFF),\_SB.PC00.RP01.HPSX))
    {
      If (\_SB.PC00.RP01.PDCX)
      {
        // Clear all status bits first.

        Store (1,\_SB.PC00.RP01.PDCX)
        Store (1,\_SB.PC00.RP01.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If (LNot (\_SB.PC00.RP01.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store (0,\_SB.PC00.RP01.L0SE)

        }
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store (1,\_SB.PC00.RP01.HPSX)
      }
    }

    // Check Root Port 2 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP02.VDID,0xFFFFFFFF),\_SB.PC00.RP02.HPSX))
    {
      If (\_SB.PC00.RP02.PDCX)
      {
        Store (1,\_SB.PC00.RP02.PDCX)
        Store (1,\_SB.PC00.RP02.HPSX)

        If (LNot (\_SB.PC00.RP02.PDSX)) {
          Store (0,\_SB.PC00.RP02.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP02.HPSX)
      }
    }

    // Check Root Port 3 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP03.VDID,0xFFFFFFFF),\_SB.PC00.RP03.HPSX))
    {
      If (\_SB.PC00.RP03.PDCX)
      {
        Store (1,\_SB.PC00.RP03.PDCX)
        Store (1,\_SB.PC00.RP03.HPSX)

        If (LNot (\_SB.PC00.RP03.PDSX)) {
          Store (0,\_SB.PC00.RP03.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP03.HPSX)
      }
    }

    // Check Root Port 4 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP04.VDID,0xFFFFFFFF),\_SB.PC00.RP04.HPSX))
    {
      If (\_SB.PC00.RP04.PDCX)
      {
        Store (1,\_SB.PC00.RP04.PDCX)
        Store (1,\_SB.PC00.RP04.HPSX)

        If (LNot (\_SB.PC00.RP04.PDSX)) {
          Store (0,\_SB.PC00.RP04.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP04.HPSX)
      }
    }

    // Check Root Port 5 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP05.VDID,0xFFFFFFFF),\_SB.PC00.RP05.HPSX))
    {
      If (\_SB.PC00.RP05.PDCX)
      {
        Store (1,\_SB.PC00.RP05.PDCX)
        Store (1,\_SB.PC00.RP05.HPSX)

        If (LNot (\_SB.PC00.RP05.PDSX)) {
          Store (0,\_SB.PC00.RP05.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP05.HPSX)
      }
    }

    // Check Root Port 6 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.RP06.VDID,0xFFFFFFFF),\_SB.PC00.RP06.HPSX))
    {
      If (\_SB.PC00.RP06.PDCX)
      {
        Store (1,\_SB.PC00.RP06.PDCX)
        Store (1,\_SB.PC00.RP06.HPSX)

        If (LNot (\_SB.PC00.RP06.PDSX)) {
          Store (0,\_SB.PC00.RP06.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP06.HPSX)
      }
    }

    If (LAnd (LNotEqual (\_SB.PC00.RP21.VDID,0xFFFFFFFF),\_SB.PC00.RP21.HPSX))
    {
      If (\_SB.PC00.RP21.PDCX)
      {
        Store (1,\_SB.PC00.RP21.PDCX)
        Store (1,\_SB.PC00.RP21.HPSX)

        If (LNot (\_SB.PC00.RP21.PDSX)) {
          Store (0,\_SB.PC00.RP21.L0SE)
        }
      }
      Else
      {
        Store (1,\_SB.PC00.RP21.HPSX)
      }
    }

    //
    // The iTBT PCIe Hot-Plug event
    //
    // Check iTBT PCIe Root Port 0 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.TRP0.VDID,0xFFFFFFFF),\_SB.PC00.TRP0.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

      Sleep (100)

      If (\_SB.PC00.TRP0.PDCX)
      {
        // Clear all status bits first.

        Store (1,\_SB.PC00.TRP0.PDCX)
        Store (1,\_SB.PC00.TRP0.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If (LNot (\_SB.PC00.TRP0.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store (0,\_SB.PC00.TRP0.L0SE)

        }
        // Call Handler for Bios assisted enumeration
        // Handler uses syncronization algoritham to enumerate one Root port
        // at a time
        If (LEqual (ITBS, 1))
        {
          \_GPE.ITBT (0x00)
        }
        // Perform proper notification
        // to the OS.
        Notify (\_SB.PC00.TRP0,0)
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store (1,\_SB.PC00.TRP0.HPSX)
      }
    }

    // Check iTBT PCIe Root Port 1 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.TRP1.VDID,0xFFFFFFFF),\_SB.PC00.TRP1.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

      Sleep (100)

      If (\_SB.PC00.TRP1.PDCX)
      {
        // Clear all status bits first.

        Store (1,\_SB.PC00.TRP1.PDCX)
        Store (1,\_SB.PC00.TRP1.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If (LNot (\_SB.PC00.TRP1.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store (0,\_SB.PC00.TRP1.L0SE)

        }
        // Call Handler for Bios assisted enumeration
        // Handler uses syncronization algoritham to enumerate one Root port
        // at a time
        If (LEqual (ITBS, 1))
        {
          \_GPE.ITBT (0x01)
        }
        // Perform proper notification
        // to the OS.
        Notify (\_SB.PC00.TRP1,0)
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store (1,\_SB.PC00.TRP1.HPSX)
      }
    }

    // Check iTBT PCIe Root Port 2 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.TRP2.VDID,0xFFFFFFFF),\_SB.PC00.TRP2.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

        Sleep (100)

      If (\_SB.PC00.TRP2.PDCX)
      {
        // Clear all status bits first.

        Store (1,\_SB.PC00.TRP2.PDCX)
        Store (1,\_SB.PC00.TRP2.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If (LNot (\_SB.PC00.TRP2.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store (0,\_SB.PC00.TRP2.L0SE)

        }
        // Call Handler for Bios assisted enumeration
        // Handler uses syncronization algoritham to enumerate one Root port
        // at a time
        If (LEqual (ITBS, 1))
        {
          \_GPE.ITBT (0x02)
        }
        // Perform proper notification
        // to the OS.
        Notify (\_SB.PC00.TRP2,0)
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store (1,\_SB.PC00.TRP2.HPSX)
      }
    }

    // Check iTBT PCIe Root Port 3 for a Hot Plug Event if the Port is
    // enabled.

    If (LAnd (LNotEqual (\_SB.PC00.TRP3.VDID,0xFFFFFFFF),\_SB.PC00.TRP3.HPSX))
    {
      // Delay for 100ms to meet the timing requirements
      // of the PCI Express Base Specification, Revision
      // 1.0A, Section 6.6 ("...software must wait at
      // least 100ms from the end of reset of one or more
      // device before it is permitted to issue
      // Configuration Requests to those devices").

      Sleep (100)

      If (\_SB.PC00.TRP3.PDCX)
      {
        // Clear all status bits first.

        Store (1,\_SB.PC00.TRP3.PDCX)
        Store (1,\_SB.PC00.TRP3.HPSX)

        //
        // PCH BIOS Spec Update Rev 1.03, Section 8.9 PCI Express* Hot-Plug BIOS Support
        // In addition, BIOS should intercept Presence Detect Changed interrupt, enable L0s on
        // hot plug and disable L0s on hot unplug. BIOS should also make sure the L0s is
        // disabled on empty slots prior booting to OS.
        //
        If (LNot (\_SB.PC00.TRP3.PDSX)) {
          // The PCI Express slot is empty, so disable L0s on hot unplug
          //
          Store (0,\_SB.PC00.TRP3.L0SE)

        }
        // Call Handler for Bios assisted enumeration
        // Handler uses syncronization algoritham to enumerate one Root port
        // at a time
        If (LEqual (ITBS, 1))
        {
          \_GPE.ITBT (0x03)
        }
        // Perform proper notification
        // to the OS.
        Notify (\_SB.PC00.TRP3,0)
      }
      Else
      {
        // False event.  Clear Hot-Plug Status
        // then exit.

        Store (1,\_SB.PC00.TRP3.HPSX)
      }
    }

  }
  //
  // Software GPE caused the event.
  //
  Method (_L62)
  {
    // Clear GPE status bit.
    Store (0,GPEC)

    ///
    /// Handle HWP SCI event
    ///
    If (LEqual (\_SB.HWPI, 1)) {
      If (CondRefOf (\_GPE.HLVT)) {
        \_GPE.HLVT ()
      }
      ///
      /// Clear HWP interrupt status
      ///
      Store (0,\_SB.HWPI)
    }
    ///
    /// Handle Intel Turbo Boost Max Technology 3.0 SCI event
    ///
    If (LEqual (\_SB.ITBI, 1)) {
      If (CondRefOf (\_GPE.ITBH)) {
        \_GPE.ITBH ()
      }
      ///
      /// Clear interrupt status
      ///
      Store (0,\_SB.ITBI)
    }
  }

}
