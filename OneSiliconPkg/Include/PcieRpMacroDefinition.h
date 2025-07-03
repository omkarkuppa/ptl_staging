/** @file
  This file contains definitions of PCIE RP MACRO.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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
#ifndef _PCIE_RP_MACRO_DEFINITION_H
#define _PCIE_RP_MACRO_DEFINITION_H

//
// PCIE RP Type MACRO Definitions
//


// Defining MACRO Value for PCH PCIE RP Type.
#ifndef PCIE_RP_TYPE_PCH
#define PCIE_RP_TYPE_PCH                  0x02
#endif

// Defining MACRO Value for CPU PCIE RP Type.
#ifndef PCIE_RP_TYPE_CPU
#define PCIE_RP_TYPE_CPU                  0x04
#endif

// Defining MACRO Value for ITBT PCIE RP Type.
#ifndef PCIE_RP_TYPE_ITBT
#define PCIE_RP_TYPE_ITBT                 0x08
#endif


//
// PCIE EP Type MACRO Definitions
//

// Note:- Defining only those EndPoint Device Type Macro, those doesn't fully
// compliance with PCIE Standard Power Sequence. For these device we might need
// additional flow in Power Sequence.


// Defining MACRO Value for Generic PCIE EP Type.
#ifndef PCIE_EP_TYPE_GENERIC_PCIE
#define PCIE_EP_TYPE_GENERIC_PCIE         0x80
#endif

// Defining MACRO Value for Discrete Thunderbolt EP Type.
#ifndef PCIE_EP_TYPE_DTBT
#define PCIE_EP_TYPE_DTBT                 0x02
#endif

// Defining MACRO Value for Invalid EP Type.
#ifndef PCIE_EP_TYPE_INVALID
#define PCIE_EP_TYPE_INVALID              0xFF
#endif

#endif // _PCIE_RP_MACRO_DEFINITION_H
