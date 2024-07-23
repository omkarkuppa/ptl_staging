/** @file
  Intel ACPI Reference Code for Intel Dynamic Power Performance Management

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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


    Name (ART1, Package ()    // ART package when in Quiet Mode / Passive Cooling Mode
    {
      0, // Revision
      // Source                     Target       Weight, AC0MaxLevel, AC1MaxLevel, AC21MaxLevel, AC31MaxLevel, AC41MaxLevel, AC51MaxLevel, AC61MaxLevel, AC71MaxLevel, AC81MaxLevel, AC91MaxLevel
      Package (){\_SB.IETM.TFN1, \_SB.PC00.TCPU, 100,             80,          60,           40,           30,           20,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN2, 100,             80,          60,           30,    0xFFFFFFFF,  0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN3, 100,     0xFFFFFFFF,  0xFFFFFFFF,   0xFFFFFFFF,           80,           60,           30,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN4, 100,             80,          60,           30,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN5, 100,             80,          60,           30,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
    })

    Name (ART0, Package ()    // ART package when in Performance Mode / Active Cooling Mode
    {
      0,    // Revision
      // Source                  Target          Weight, AC0MaxLevel, AC1MaxLevel, AC21MaxLevel, AC31MaxLevel, AC41MaxLevel, AC51MaxLevel, AC61MaxLevel, AC71MaxLevel, AC81MaxLevel, AC91MaxLevel
      Package (){\_SB.IETM.TFN1, \_SB.PC00.TCPU, 100,            100,          80,           50,           40,           30,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN2, 100,             80,          50,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN3, 100,     0xFFFFFFFF,  0xFFFFFFFF,   0xFFFFFFFF,          100,           80,           50,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN4, 100,            100,          80,           50,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF},
      Package (){\_SB.IETM.TFN1, \_SB.IETM.SEN5, 100,            100,          80,           50,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF,   0xFFFFFFFF}
    })
