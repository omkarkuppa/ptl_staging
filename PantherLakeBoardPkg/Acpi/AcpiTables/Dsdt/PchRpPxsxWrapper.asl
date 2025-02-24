/** @file
  Intel ACPI Reference Code for Pch Rootport Endpoint Wrapper

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

Include ("Gmio.asl")                  // Provided by CnvFeaturePkg

Scope (\_SB.PC00.RP01.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP01
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }
  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Method (_DSM, 4, Serialized) {
      Include ("WwanDsm.asl")
      // If the code falls through to this point, just return a buffer of 0.
      Return (Buffer () {0x00})
    }  // End _DSM Method
  }
}

Scope (\_SB.PC00.RP02.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP02
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Method (_DSM, 4, Serialized)
    {
      // Compare passed in UUID to supported UUID.
      Include ("WwanDsm.asl")
      // If the code falls through to this point, just return a buffer of 0.
      Return (Buffer () {0x00})
    }
  }
}

Scope (\_SB.PC00.RP03.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP03
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Method (_DSM, 4, Serialized)
    {
      Include ("WwanDsm.asl")
      // If the code falls through to this point, just return a buffer of 0.
      Return (Buffer () {0x00})
    }
  }
}

Scope (\_SB.PC00.RP04.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP04
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT))) {
    Method (_DSM, 4, Serialized)
    {
      Include ("WwanDsm.asl")
      // If the code falls through to this point, just return a buffer of 0.
      Return (Buffer () {0x00})
    }
  }
}

Scope (\_SB.PC00.RP05.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP05
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LOr (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)), LAnd (CondRefOf (\DLRM), (LNotEqual (\DLRM, 0)))))
  {
    Method (_DSM, 4, Serialized)
    {
      //
      // DLRM support
      //
      If (LEqual (Arg0, ToUUID ("C41F8AFB-4701-F0EB-1D26-0296648C30E4")))
      {
        If (LEqual (1, ToInteger (Arg1)))        // Revision 1.
        {
          Switch (ToInteger (Arg2))            // Switch to Function Index.
          {
            //
            // Function 0, Query of supported functions.
            //
            Case (0)
            {
              Return (Buffer () {0x03})
            }

            //
            // Function 1, DLRM Support for Storage to reduce active power usage in D3.
            //
            Case (1)
            {
              // Only return support if platform enabled DLRM via setup.
              If (LAnd (PNVM (), LAnd (LNotEqual (\DLRM, 0), LEqual (S1G4, 1)))) {
                ADBG ("Enable DLRM for Storage")
                Return (1)
              } Else {
                ADBG ("Disable DLRM for Storage")
                Return (0)
              }
            }
          }
        } Else {
          ADBG ("DLRM Revision 0: No function supported")
        }
        Return (Buffer() {0x00})
      }

      Include ("WwanDsm.asl")
      Return (Buffer () {0x00})
    }  // End _DSM Method
  }
}

Scope (\_SB.PC00.RP06.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP06
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Method (_DSM, 4, Serialized)
    {
      Include ("WwanDsm.asl")
      // If the code falls through to this point, just return a buffer of 0.
      Return (Buffer () {0x00})
    }
  }  // End _DSM Method
}

Scope (\_SB.PC00.RP09.PXSX)
{
  #undef PCIE_ROOT_PORT
  #define PCIE_ROOT_PORT \_SB.PC00.RP09
  Include ("EpOpRegion.asl")          // Provided by CnvFeaturePkg
#if FixedPcdGetBool (PcdCnvAcpiTables) == 1
  Include ("Wist.asl")                // Provided by CnvFeaturePkg
#endif
  If (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)))
  {
    Include ("Wwan.asl")
  }

  //
  // _DSM : Device Specific Method
  //
  // Arg0: UUID Unique function identifier
  // Arg1: Integer Revision Level
  // Arg2: Integer Function Index
  // Arg3: Package Parameters
  //
  //
  If (LOr (LAnd (LNotEqual (WWEN, 0), LEqual (WWRP, SLOT)), LAnd (CondRefOf (\DLRM), (LNotEqual (\DLRM, 0)))))
  {
    Method (_DSM, 4, Serialized)
    {
      //
      // DLRM support
      //
      If (LEqual (Arg0, ToUUID ("C41F8AFB-4701-F0EB-1D26-0296648C30E4")))
      {
        If (LEqual (1, ToInteger (Arg1)))        // Revision 1.
        {
          Switch (ToInteger (Arg2))            // Switch to Function Index.
          {
            //
            // Function 0, Query of supported functions.
            //
            Case (0)
            {
              Return (Buffer () {0x03})
            }

            //
            // Function 1, DLRM Support for Storage to reduce active power usage in D3.
            //
            Case (1)
            {
              // Only return support if platform enabled DLRM via setup.
              If (LAnd (PNVM (), LAnd (LNotEqual (\DLRM, 0), LEqual (S2G4, 1)))) {
                ADBG ("Enable DLRM for Storage")
                Return (1)
              } Else {
                ADBG ("Disable DLRM for Storage")
                Return (0)
              }
            }
          }
        } Else {
          ADBG ("DLRM Revision 0: No function supported")
        }
        Return (Buffer () {0x00})
      }

      Include ("WwanDsm.asl")

      Return (Buffer () {0x00})
    }  // End _DSM Method
  }
}