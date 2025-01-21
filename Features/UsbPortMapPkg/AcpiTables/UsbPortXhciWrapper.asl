/**@file

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

DefinitionBlock (
    "UsbPortXhciWrapper.aml",
    "SSDT",
    0x02, // SHAD revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
    "INTEL", // OEM ID (6 byte string)
    "UpmpSsdt", // USB PORT MAP OEM table ID (8 byte string)
    0x0 // OEM version of DSDT table (4 byte Integer)
    )
{
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 0
  External (\_SB.PC00.XHCI.RHUB.HS01, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 1
  External (\_SB.PC00.XHCI.RHUB.HS02, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 2
  External (\_SB.PC00.XHCI.RHUB.HS03, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 3
  External (\_SB.PC00.XHCI.RHUB.HS04, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 4
  External (\_SB.PC00.XHCI.RHUB.HS05, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 5
  External (\_SB.PC00.XHCI.RHUB.HS06, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 6
  External (\_SB.PC00.XHCI.RHUB.HS07, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 7
  External (\_SB.PC00.XHCI.RHUB.HS08, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 8
  External (\_SB.PC00.XHCI.RHUB.HS09, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 9
  External (\_SB.PC00.XHCI.RHUB.HS10, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 10
  External (\_SB.PC00.XHCI.RHUB.HS11, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 11
  External (\_SB.PC00.XHCI.RHUB.HS12, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 12
  External (\_SB.PC00.XHCI.RHUB.HS13, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 13
  External (\_SB.PC00.XHCI.RHUB.HS14, DeviceObj)
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 0
  External (\_SB.PC00.XHCI.RHUB.SS01, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 1
  External (\_SB.PC00.XHCI.RHUB.SS02, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 2
  External (\_SB.PC00.XHCI.RHUB.SS03, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 3
  External (\_SB.PC00.XHCI.RHUB.SS04, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 4
  External (\_SB.PC00.XHCI.RHUB.SS05, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 5
  External (\_SB.PC00.XHCI.RHUB.SS06, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 6
  External (\_SB.PC00.XHCI.RHUB.SS07, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 7
  External (\_SB.PC00.XHCI.RHUB.SS08, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 8
  External (\_SB.PC00.XHCI.RHUB.SS09, DeviceObj)
#endif
#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 9
  External (\_SB.PC00.XHCI.RHUB.SS10, DeviceObj)
#endif

  External (\_SB.PC00.XHCI, DeviceObj)

  External (\_SB.TPLD, MethodObj)
  External (\_SB.TUPC, MethodObj)

  External (\_SB.A1UP, MethodObj)
  External (\_SB.A1PL, MethodObj)
  External (\_SB.A2UP, MethodObj)
  External (\_SB.A2PL, MethodObj)
  External (\_SB.A3UP, MethodObj)
  External (\_SB.A3PL, MethodObj)
  External (\_SB.A4UP, MethodObj)
  External (\_SB.A4PL, MethodObj)
  External (\_SB.A5UP, MethodObj)
  External (\_SB.A5PL, MethodObj)
  External (\_SB.A6UP, MethodObj)
  External (\_SB.A6PL, MethodObj)
  External (\_SB.A7UP, MethodObj)
  External (\_SB.A7PL, MethodObj)
  External (\_SB.A8UP, MethodObj)
  External (\_SB.A8PL, MethodObj)
  External (\_SB.A9UP, MethodObj)
  External (\_SB.A9PL, MethodObj)
  External (\_SB.AAUP, MethodObj)
  External (\_SB.AAPL, MethodObj)
  External (\_SB.ABUP, MethodObj)
  External (\_SB.ABPL, MethodObj)
  External (\_SB.ACUP, MethodObj)
  External (\_SB.ACPL, MethodObj)
  External (\_SB.ADUP, MethodObj)
  External (\_SB.ADPL, MethodObj)
  External (\_SB.AEUP, MethodObj)
  External (\_SB.AEPL, MethodObj)

  External (\ADBG, MethodObj)
  #define ADBG(x) \
    If (CondRefOf (ADBG)) { \
      ADBG(x) \
    }

  Include ("UsbPortMapNvs.asl")

  ADBG ("[USB PORT MAP SSDT][AcpiTableEntry]")

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 0
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS01)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS01) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A1UP)) {
          Store (A1UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A1CN, A1TP, A1CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A1PL)) {
          Store (A1PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A1VS, A1GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 1
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS02)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS02) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A2UP)) {
          Store (A2UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A2CN, A2TP, A2CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A2PL)) {
          Store (A2PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A2VS, A2GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 2
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS03)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS03) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A3UP)) {
          Store (A3UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A3CN, A3TP, A3CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A3PL)) {
          Store (A3PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A3VS, A3GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 3
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS04)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS04) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A4UP)) {
          Store (A4UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A4CN, A4TP, A4CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A4PL)) {
          Store (A4PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A4VS, A4GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 4
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS05)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS05) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A5UP)) {
          Store (A5UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A5CN, A5TP, A5CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A5PL)) {
          Store (A5PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A5VS, A5GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 5
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS06)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS06) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A6UP)) {
          Store (A6UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A6CN, A6TP, A6CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A6PL)) {
          Store (A6PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A6VS, A6GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 6
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS07)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS07) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A7UP)) {
          Store (A7UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A7CN, A7TP, A7CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A7PL)) {
          Store (A7PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A7VS, A7GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 7
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS08)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS08) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A8UP)) {
          Store (A8UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A8CN, A8TP, A8CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A8PL)) {
          Store (A8PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A8VS, A8GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 8
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS09)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS09) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.A9UP)) {
          Store (A9UP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (A9CN, A9TP, A9CP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.A9PL)) {
          Store (A9PL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (A9VS, A9GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 9
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS10)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS10) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.AAUP)) {
          Store (AAUP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (AACN, AATP, AACP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.AAPL)) {
          Store (AAPL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (AAVS, AAGP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 10
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS11)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS11) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.ABUP)) {
          Store (ABUP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (ABCN, ABTP, ABCP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.ABPL)) {
          Store (ABPL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (ABVS, ABGP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 11
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS12)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS12) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.ACUP)) {
          Store (ACUP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (ACCN, ACTP, ACCP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.ACPL)) {
          Store (ACPL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (ACVS, ACGP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 12
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS13)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS13) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.ADUP)) {
          Store (ADUP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (ADCN, ADTP, ADCP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.ADPL)) {
          Store (ADPL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (ADVS, ADGP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb2PortNumber) > 13
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.HS14)) {
    Scope (\_SB.PC00.XHCI.RHUB.HS14) {
      Method (_UPC, 0, Serialized) {
        If (CondRefOf (\_SB.AEUP)) {
          Store (AEUP (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TUPC (AECN, AETP, AECP))
      }
      Method (_PLD, 0, Serialized) {
        If (CondRefOf (\_SB.AEPL)) {
          Store (AEPL (), Local0)
          If (LEqual (DeRefOf (Index (Local0, 0)), 1)) {
            Return (DeRefOf (Index (Local0, 1)))
          }
        }
        Return (\_SB.TPLD (AEVS, AEGP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 0
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS01)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS01) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B1CN, B1TP, B1CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B1VS, B1GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 1
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS02)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS02) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B2CN, B2TP, B2CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B2VS, B2GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 2
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS03)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS03) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B3CN, B3TP, B3CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B3VS, B3GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 3
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS04)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS04) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B4CN, B4TP, B4CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B4VS, B4GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 4
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS05)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS05) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B5CN, B5TP, B5CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B5VS, V5GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 5
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS06)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS06) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B6CN, B6TP, B6CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B6VS, B6GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 6
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS07)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS07) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B7CN, B7TP, B7CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B7VS, B7GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 7
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS08)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS08) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B8CN, B8TP, B8CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B8VS, B8GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 8
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS09)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS09) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (B9CN, B9TP, B9CP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (B9VS, B9GP))
      }
    }
  }
#endif

#if FixedPcdGet8 (PcdMaxPchUsb3PortNumber) > 9
  If (CondRefOf (\_SB.PC00.XHCI.RHUB.SS10)) {
    Scope (\_SB.PC00.XHCI.RHUB.SS10) {
      Method (_UPC, 0, Serialized) {
        Return (\_SB.TUPC (BACN, BATP, BACP))
      }
      Method (_PLD, 0, Serialized) {
        Return (\_SB.TPLD (BAVS, BAGP))
      }
    }
  }
#endif

  ADBG ("[USB PORT MAP SSDT][AcpiTableExit]")

}