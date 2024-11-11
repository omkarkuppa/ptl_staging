/** @file
  Intel DGR SSDT ACPI Code.

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

#define UNKNOWN_PARTICIPANT_HID "XXXX9999"

DefinitionBlock (
  "DGRSSDT.aml",
  "SSDT",
  0x02,
  "Intel",
  "DgrSsdt",
  0x3000
  )
{
  External (\_SB.IETM, DeviceObj)
  External (\_SB.IETM.LHID, MethodObj)
  External (LIMA)
  External (LIIA)

  Scope (\_SB)
  {
    Include ("DgrNvs.asl")

    If (CondRefOf (\_SB.IETM)) {
      Scope (\_SB.IETM)
      {
        Device (GPD1) // Generic Participant Device 1
        {
          Name (PTYP, 0x18)

          Method (_HID) { // return HID based on Platform definition
            If (CondRefOf (\_SB.IETM.LHID))
            {
              Return (\_SB.IETM.LHID ())
            }
            Return (UNKNOWN_PARTICIPANT_HID)
          }

          //
          // Operational region for Smi port access
          //
          OperationRegion (SMIP, SystemIO, 0xB2, 1)
          Field (SMIP, ByteAcc, NoLock, Preserve)
          {
              IOPN, 8,
          }

          //
          // Name: GPDB
          // Description: Trigger SMI to provide Ledger Island Buffer to IPF driver
          // Return : Buffer (Version, DataLen, DataBuffer)
          //
          //
          Method (GPDB, 0, Serialized, 0, BuffObj)
          {
            //
            // Trigger the SMI interrupt
            //
            Store (LIIA, IOPN)
            Name (BUF1, Buffer (16) {})
            CreateDWordField (BUF1, 0x00, VERS)  // DWORD: Version
            CreateDWordField (BUF1, 0x04, DLEN)  // DWORD: DataLen
            CreateQWordField (BUF1, 0x08, DBUF)  // QWORD: DataBuffer
            Store (1, VERS)
            Store (FixedPcdGet32 (PcdLedgerIslandBufferSize), DLEN)
            Store (LIMA, DBUF)
            Return (BUF1)
          }
        } // End GPD1
      } // End Scope(IETM)
    } // End CondRefOf
  } // End Scope(\_SB)
} // End of Definition Block
