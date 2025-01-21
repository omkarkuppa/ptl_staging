/** @file
  This file defines the macro bases for dTBT ASL.

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

#ifndef _DTBT_ASL_BASE_H_
#define _DTBT_ASL_BASE_H_

//
// Convert the macro literal to string for print
//
#define GET_STR(VAL)                          #VAL
#define TOSTR(VAL)                            GET_STR(VAL)

#define CONCATENATE(NAME,NUM)                 NAME##NUM
#define CONCATENATE_NAME_NUM(NAME, NUM)       CONCATENATE(NAME,NUM)

#define CONCATENATE_ZERO(NAME,NUM)            NAME##0##NUM
#define CONCATENATE_NAME_ZERO_NUM(NAME, NUM)  CONCATENATE_ZERO(NAME,NUM)


// -----------------------------------------------------------------------

//
// Macro bases for IOE
//

// -----------------------------------------------------------------------

//
// Macro for getting IOE RP name with/without IOE_RP_PREFIX_NO_ZERO
//
#ifdef IOE_RP_PREFIX_NO_ZERO
#define CONCATENATE_NAME_NUM_IOE(NAME, NUM)      CONCATENATE_NAME_NUM(NAME, NUM)
#else
#define CONCATENATE_NAME_NUM_IOE(NAME, NUM)      CONCATENATE_NAME_ZERO_NUM(NAME,NUM)
#endif

//
// IOE RP PC segment : PC00, PC01, PC02 ...
//
#ifndef IOE_RP_PC
#define IOE_RP_PC               PC00
#endif

//
// IOE RP name prefix : RPxx, PEGx...
//
#ifndef IOE_RP_PREFIX
#define IOE_RP_PREFIX           RP
#endif

#define IOE_RP_NAME(NUM)        CONCATENATE_NAME_NUM(IOE_RP_PREFIX, NUM)
#define IOE_RP_NAME_1(NUM)      CONCATENATE_NAME_NUM_IOE(IOE_RP_PREFIX, NUM)

//
// IOE RP upstream port ACPI name : PXSX, PEGP ...
//
#ifndef IOE_RP_UPORT
#define IOE_RP_UPORT            PXSX
#endif

//
// IOE RP path with RP number >= 10
//
#define IOE_RP_PATH(NUM)        \_SB.IOE_RP_PC.IOE_RP_NAME(NUM)

//
// IOE RP path with RP number < 10
//
#define IOE_RP_PATH_1(NUM)      \_SB.IOE_RP_PC.IOE_RP_NAME_1(NUM)

// -----------------------------------------------------------------------

//
// Macro bases for PCH
//

// -----------------------------------------------------------------------

//
// Macro for getting PCH RP name with/without PCH_RP_PREFIX_NO_ZERO
//
#ifdef PCH_RP_PREFIX_NO_ZERO
#define CONCATENATE_NAME_NUM_PCH(NAME, NUM)      CONCATENATE_NAME_NUM(NAME, NUM)
#else
#define CONCATENATE_NAME_NUM_PCH(NAME, NUM)      CONCATENATE_NAME_ZERO_NUM(NAME,NUM)
#endif

//
// PCH RP PC segment : PC00, PC01, PC02 ...
//
#ifndef PCH_RP_PC
#define PCH_RP_PC               PC02
#endif

//
// PCH RP name prefix : RPxx, ...
//
#ifndef PCH_RP_PREFIX
#define PCH_RP_PREFIX           RP
#endif

#define PCH_RP_NAME(NUM)        CONCATENATE_NAME_NUM(PCH_RP_PREFIX, NUM)
#define PCH_RP_NAME_1(NUM)      CONCATENATE_NAME_NUM_PCH(PCH_RP_PREFIX, NUM)

//
// PCH RP upstream port ACPI name
//
#ifndef PCH_RP_UPORT
#define PCH_RP_UPORT            PXSX
#endif

//
// PCH RP path with RP number >= 10
//
#define PCH_RP_PATH(NUM)        \_SB.PCH_RP_PC.PCH_RP_NAME(NUM)

//
// PCH RP path with RP number < 10
//
#define PCH_RP_PATH_1(NUM)      \_SB.PCH_RP_PC.PCH_RP_NAME_1(NUM)

// -----------------------------------------------------------------------

#endif      // _DTBT_ASL_BASE_H_
