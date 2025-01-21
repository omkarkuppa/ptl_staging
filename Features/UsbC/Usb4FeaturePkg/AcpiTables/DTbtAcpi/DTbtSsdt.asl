/** @file
  This file contains the Segment SSDT Table ASL code.
  It defines a SSDT table for extra Segment

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

@par Specification
**/

#include <Include/AcpiDebug.h>
#include <PcieRpMacroDefinition.h>
#include <DTbtAslBase.h>

DefinitionBlock (
  "DTbtSsdt.aml",
  "SSDT",
  2,
  "INTEL ",
  "DTbtSsdt",
  0x1000
  )
{
  External (\ADBG, MethodObj)

  ADBG ("[dTBT] Loading DTBT AFP SSDT start")

  Include ("DTbtNvs.asl")
  Include ("DTbtLib.asl")

  If (LEqual (DTFS, 0x01)) {
    Include ("DTbtIoe.asl")
    Include ("DTbtPch.asl")
  } Else {
    ADBG ("[dTBT] DTBT feature is not enabled, skip loading DTBT AFP SSDT!")
  }
  ADBG ("[dTBT] Loading DTBT AFP SSDT end")
}
