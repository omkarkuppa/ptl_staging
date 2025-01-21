/** @file
  Intel ACPI Reference Code for WLAN Rootport Endpoint Wrapper

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#include <CnvBaseDef.h>

External (SLOT,IntObj)
External (WIFI_EP_PATH (01), DeviceObj)
External (WIFI_EP_PATH (02), DeviceObj)
External (WIFI_EP_PATH (03), DeviceObj)
External (WIFI_EP_PATH (04), DeviceObj)
External (WIFI_EP_PATH (05), DeviceObj)
External (WIFI_EP_PATH (06), DeviceObj)
External (WIFI_EP_PATH (07), DeviceObj)
External (WIFI_EP_PATH (08), DeviceObj)
External (WIFI_EP_PATH (09), DeviceObj)
External (WIFI_EP_PATH (10), DeviceObj)
External (WIFI_EP_PATH (11), DeviceObj)
External (WIFI_EP_PATH (12), DeviceObj)
External (WIFI_EP_PATH (13), DeviceObj)
External (WIFI_EP_PATH (14), DeviceObj)
External (WIFI_EP_PATH (15), DeviceObj)
External (WIFI_EP_PATH (16), DeviceObj)
External (WIFI_EP_PATH (17), DeviceObj)
External (WIFI_EP_PATH (18), DeviceObj)
External (WIFI_EP_PATH (19), DeviceObj)
External (WIFI_EP_PATH (20), DeviceObj)
External (WIFI_EP_PATH (21), DeviceObj)
External (WIFI_EP_PATH (22), DeviceObj)
External (WIFI_EP_PATH (23), DeviceObj)
External (WIFI_EP_PATH (24), DeviceObj)
External (WIFI_EP_PATH (25), DeviceObj)
External (WIFI_EP_PATH (26), DeviceObj)
External (WIFI_EP_PATH (27), DeviceObj)
External (WIFI_EP_PATH (28), DeviceObj)

External (WIFI_RP_PATH (01)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (02)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (03)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (04)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (05)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (06)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (07)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (08)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (09)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (10)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (11)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (12)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (13)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (14)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (15)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (16)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (17)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (18)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (19)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (20)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (21)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (22)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (23)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (24)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (25)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (26)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (27)._ADR, RP_ADR_OBJ_TYPE)
External (WIFI_RP_PATH (28)._ADR, RP_ADR_OBJ_TYPE)

External (WIFI_EP_PATH (01)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (02)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (03)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (04)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (05)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (06)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (07)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (08)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (09)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (10)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (11)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (12)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (13)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (14)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (15)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (16)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (17)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (18)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (19)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (20)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (21)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (22)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (23)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (24)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (25)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (26)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (27)._ADR, EP_ADR_OBJ_TYPE)
External (WIFI_EP_PATH (28)._ADR, EP_ADR_OBJ_TYPE)

External (WIFI_RP_PATH (01).SLOT, IntObj)
External (WIFI_RP_PATH (02).SLOT, IntObj)
External (WIFI_RP_PATH (03).SLOT, IntObj)
External (WIFI_RP_PATH (04).SLOT, IntObj)
External (WIFI_RP_PATH (05).SLOT, IntObj)
External (WIFI_RP_PATH (06).SLOT, IntObj)
External (WIFI_RP_PATH (07).SLOT, IntObj)
External (WIFI_RP_PATH (08).SLOT, IntObj)
External (WIFI_RP_PATH (09).SLOT, IntObj)
External (WIFI_RP_PATH (10).SLOT, IntObj)
External (WIFI_RP_PATH (11).SLOT, IntObj)
External (WIFI_RP_PATH (12).SLOT, IntObj)
External (WIFI_RP_PATH (13).SLOT, IntObj)
External (WIFI_RP_PATH (14).SLOT, IntObj)
External (WIFI_RP_PATH (15).SLOT, IntObj)
External (WIFI_RP_PATH (16).SLOT, IntObj)
External (WIFI_RP_PATH (17).SLOT, IntObj)
External (WIFI_RP_PATH (18).SLOT, IntObj)
External (WIFI_RP_PATH (19).SLOT, IntObj)
External (WIFI_RP_PATH (20).SLOT, IntObj)
External (WIFI_RP_PATH (21).SLOT, IntObj)
External (WIFI_RP_PATH (22).SLOT, IntObj)
External (WIFI_RP_PATH (23).SLOT, IntObj)
External (WIFI_RP_PATH (24).SLOT, IntObj)
External (WIFI_RP_PATH (25).SLOT, IntObj)
External (WIFI_RP_PATH (26).SLOT, IntObj)
External (WIFI_RP_PATH (27).SLOT, IntObj)
External (WIFI_RP_PATH (28).SLOT, IntObj)

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 0
If (CondRefOf (WIFI_EP_PATH (01))) {
  Scope (WIFI_EP_PATH (01))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(01)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 1
If (CondRefOf (WIFI_EP_PATH (02))) {
  Scope(WIFI_EP_PATH (02))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(02)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 2
If (CondRefOf (WIFI_EP_PATH (03))) {
  Scope(WIFI_EP_PATH (03))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(03)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 3
If (CondRefOf (WIFI_EP_PATH (04))) {
  Scope(WIFI_EP_PATH (04))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(04)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 4
If (CondRefOf (WIFI_EP_PATH (05))) {
  Scope(WIFI_EP_PATH (05))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(05)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 5
If (CondRefOf (WIFI_EP_PATH (06))) {
  Scope(WIFI_EP_PATH (06))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(06)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 6
If (CondRefOf (WIFI_EP_PATH (07))) {
  Scope(WIFI_EP_PATH (07))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(07)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 7
If (CondRefOf (WIFI_EP_PATH (08))) {
  Scope(WIFI_EP_PATH (08))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(08)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 8
If (CondRefOf (WIFI_EP_PATH (09))) {
  Scope(WIFI_EP_PATH (09))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(09)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 9
If (CondRefOf (WIFI_EP_PATH (10))) {
  Scope(WIFI_EP_PATH (10))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(10)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 10
If (CondRefOf (WIFI_EP_PATH (11))) {
  Scope(WIFI_EP_PATH (11))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(11)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 11
If (CondRefOf (WIFI_EP_PATH (12))) {
  Scope(WIFI_EP_PATH (12))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(12)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 12
If (CondRefOf (WIFI_EP_PATH (13))) {
  Scope(WIFI_EP_PATH (13))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(13)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 13
If (CondRefOf (WIFI_EP_PATH (14))) {
  Scope(WIFI_EP_PATH (14))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(14)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 14
If (CondRefOf (WIFI_EP_PATH (15))) {
  Scope(WIFI_EP_PATH (15))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(15)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 15
If (CondRefOf (WIFI_EP_PATH (16))) {
  Scope(WIFI_EP_PATH (16))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(16)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 16
If (CondRefOf (WIFI_EP_PATH (17))) {
  Scope(WIFI_EP_PATH (17))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(17)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 17
If (CondRefOf (WIFI_EP_PATH (18))) {
  Scope(WIFI_EP_PATH (18))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(18)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 18
If (CondRefOf (WIFI_EP_PATH (19))) {
  Scope(WIFI_EP_PATH (19))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(19)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 19
If (CondRefOf (WIFI_EP_PATH (20))) {
  Scope(WIFI_EP_PATH (20))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(20)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 20
If (CondRefOf (WIFI_EP_PATH (21))) {
  Scope(WIFI_EP_PATH (21))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(21)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 21
If (CondRefOf (WIFI_EP_PATH (22))) {
  Scope(WIFI_EP_PATH (22))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(22)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 22
If (CondRefOf (WIFI_EP_PATH (23))) {
  Scope(WIFI_EP_PATH (23))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(23)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 23
If (CondRefOf (WIFI_EP_PATH (24))) {
  Scope(WIFI_EP_PATH (24))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(24)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 24
If (CondRefOf (WIFI_EP_PATH (25))) {
  Scope(WIFI_EP_PATH (25))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(25)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 25
If (CondRefOf (WIFI_EP_PATH (26))) {
  Scope(WIFI_EP_PATH (26))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(26)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 26
If (CondRefOf (WIFI_EP_PATH (27))) {
  Scope(WIFI_EP_PATH (27))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(27)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif

#if FixedPcdGet8 (PcdMaxRootPortNumber) > 27
If (CondRefOf (WIFI_EP_PATH (28))) {
  Scope(WIFI_EP_PATH (28))
  {
    #undef PCIE_ROOT_PORT
    #define PCIE_ROOT_PORT  WIFI_RP_PATH(28)
    //
    // Load WLAN tables only when WLAN has found on this RP
    //
    If (CondRefOf (PCIE_ROOT_PORT.SLOT)) {
      If (LEqual (WLRP, PCIE_ROOT_PORT.SLOT)) {
        Include ("WifiFeature.asl")
        Include ("GuidLockIndicator.asl")
      }
    }
  }
}
#endif
