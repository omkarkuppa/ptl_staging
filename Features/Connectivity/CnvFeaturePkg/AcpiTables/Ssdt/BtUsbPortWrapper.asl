/** @file
  Intel ACPI Sample Code for BT USB Port Wrapper

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

External (USB2_PORT_PATH (01), DeviceObj)
External (USB2_PORT_PATH (02), DeviceObj)
External (USB2_PORT_PATH (03), DeviceObj)
External (USB2_PORT_PATH (04), DeviceObj)
External (USB2_PORT_PATH (05), DeviceObj)
External (USB2_PORT_PATH (06), DeviceObj)
External (USB2_PORT_PATH (07), DeviceObj)
External (USB2_PORT_PATH (08), DeviceObj)
External (USB2_PORT_PATH (09), DeviceObj)
External (USB2_PORT_PATH (10), DeviceObj)
External (USB2_PORT_PATH (11), DeviceObj)
External (USB2_PORT_PATH (12), DeviceObj)
External (USB2_PORT_PATH (13), DeviceObj)
External (USB2_PORT_PATH (14), DeviceObj)
External (USB2_PORT_PATH (15), DeviceObj)
External (USB2_PORT_PATH (16), DeviceObj)

#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 0
If (CondRefOf (USB2_PORT_PATH (01))) {
  Scope (USB2_PORT_PATH (01)) {
    If (LOr (LEqual (DUBP, 1), LEqual (IUBP, 1))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 1
If (CondRefOf (USB2_PORT_PATH (02))) {
  Scope (USB2_PORT_PATH (02)) {
    If (LOr (LEqual (DUBP, 2), LEqual (IUBP, 2))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 2
If (CondRefOf (USB2_PORT_PATH (03))) {
  Scope (USB2_PORT_PATH (03)) {
    If (LOr (LEqual (DUBP, 3), LEqual (IUBP, 3))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 3
If (CondRefOf (USB2_PORT_PATH (04))) {
  Scope (USB2_PORT_PATH (04)) {
    If (LOr (LEqual (DUBP, 4), LEqual (IUBP, 4))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 4
If (CondRefOf (USB2_PORT_PATH (05))) {
  Scope (USB2_PORT_PATH (05)) {
    If (LOr (LEqual (DUBP, 5), LEqual (IUBP, 5))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 5
If (CondRefOf (USB2_PORT_PATH (06))) {
  Scope (USB2_PORT_PATH (06)) {
    If (LOr (LEqual (DUBP, 6), LEqual (IUBP, 6))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 6
If (CondRefOf (USB2_PORT_PATH (07))) {
  Scope (USB2_PORT_PATH (07)) {
    If (LOr (LEqual (DUBP, 7), LEqual (IUBP, 7))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 7
If (CondRefOf (USB2_PORT_PATH (08))) {
  Scope (USB2_PORT_PATH (08)) {
    If (LOr (LEqual (DUBP, 8), LEqual (IUBP, 8))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 8
If (CondRefOf (USB2_PORT_PATH (09))) {
  Scope (USB2_PORT_PATH (09)) {
    If (LOr (LEqual (DUBP, 9), LEqual (IUBP, 9))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 9
If (CondRefOf (USB2_PORT_PATH (10))) {
  Scope (USB2_PORT_PATH (10)) {
    If (LOr (LEqual (DUBP, 10), LEqual (IUBP, 10))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 10
If (CondRefOf (USB2_PORT_PATH (11))) {
  Scope (USB2_PORT_PATH (11)) {
    If (LOr (LEqual (DUBP, 11), LEqual (IUBP, 11))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 11
If (CondRefOf (USB2_PORT_PATH (12))) {
  Scope (USB2_PORT_PATH (12)) {
    If (LOr (LEqual (DUBP, 12), LEqual (IUBP, 12))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 12
If (CondRefOf (USB2_PORT_PATH (13))) {
  Scope (USB2_PORT_PATH (13)) {
    If (LOr (LEqual (DUBP, 13), LEqual (IUBP, 13))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 13
If (CondRefOf (USB2_PORT_PATH (14))) {
  Scope (USB2_PORT_PATH (14)) {
    If (LOr (LEqual (DUBP, 14), LEqual (IUBP, 14))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 14
If (CondRefOf (USB2_PORT_PATH (15))) {
  Scope (USB2_PORT_PATH (15)) {
    If (LOr (LEqual (DUBP, 15), LEqual (IUBP, 15))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
#if FixedPcdGet8 (PcdMaxUsb2PortNumber) > 15
If (CondRefOf (USB2_PORT_PATH (16))) {
  Scope (USB2_PORT_PATH (16)) {
    If (LOr (LEqual (DUBP, 16), LEqual (IUBP, 16))) {
#if FixedPcdGetBool (PcdBtDsmSupport) == 1
      Include ("BtDsm.asl")
#endif
      Include ("BtReset.asl")
#if FixedPcdGetBool (PcdBtAudioOffloadSupport) == 1
      Include ("BtAudioOffload.asl")
#endif
      Include ("GuidLockIndicator.asl")
    }
  }
}
#endif
