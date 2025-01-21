/** @file
  This file contains the Intel Graphics SSDT Table ASL code.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2012 Intel Corporation.

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
#include <Include/AcpiDebug.h>

DefinitionBlock (
  "IgfxSsdt.aml",
  "SSDT",
  2,
  "INTEL ",
  "IgfxSsdt",
  0x3000
  )
{
  External (\_SB.PC00, DeviceObj)
  External (\_SB.PC00.MC, DeviceObj)
  External (\_SB.PC00.GFX0, DeviceObj)
  External (\_SB.PC00.PC2M, MethodObj)
  ACPI_DEBUG_EXTERNAL_REFERENCE
  External (\NDID)
  External (\DID1)
  External (\DID2)
  External (\DID3)
  External (\DID4)
  External (\DID5)
  External (\DID6)
  External (\DID7)
  External (\DID8)
  External (\DID9)
  External (\DIDA)
  External (\DIDB)
  External (\DIDC)
  External (\DIDD)
  External (\DIDE)
  External (\DIDF)
  External (\DIDX)
  External (\DIDY)

  External (\NXD1)
  External (\NXD2)
  External (\NXD3)
  External (\NXD4)
  External (\NXD5)
  External (\NXD6)
  External (\NXD7)
  External (\NXD8)
  External (\NXDY)

  External (\IPTP)
  External (\EDPV)
  External (\NXDX)
  External (\HGDT)
  External (\LIDS)
  External (\BRTL)
  External (\ASLB)

  Scope (\_SB.PC00.GFX0)
  {
    include ("IGpu.asl")

    Method (_S3D, 0, NotSerialized)
    {
      Return (3)
    }
    Method (_S4D, 0, NotSerialized)
    {
      Return (3)
    }
    Method (_PS0,0,Serialized)
    {
    }
    Method (_PS3,0,Serialized)
    {
    }
    Method (GFPS,0,Serialized) {
      OperationRegion (PXCS, SystemMemory, \_SB.PC00.PC2M (0x00020000, 0), 0x100) // PCI eXpress Configuration Space
      Field (PXCS, AnyAcc, NoLock, Preserve) {
        Offset (0xD4), // PMCSR
        D3HT, 2,       // PowerState
      }
      Return (D3HT)
    }
  }
}
