/** @file
  Device IDs for LPSS Controllers

  Conventions:

  - Register definition format:
    Prefix_[GenerationName]_[ComponentName]_SubsystemName_RegisterSpace_RegisterName
  - Prefix:
    Definitions beginning with "R_" are registers
    Definitions beginning with "B_" are bits within registers
    Definitions beginning with "V_" are meaningful values within the bits
    Definitions beginning with "S_" are register size
    Definitions beginning with "N_" are the bit position
  - [GenerationName]:
    Three letter acronym of the generation is used (e.g. SKL,KBL,CNL etc.).
    Register name without GenerationName applies to all generations.
  - [ComponentName]:
    This field indicates the component name that the register belongs to (e.g. PCH, SA etc.)
    Register name without ComponentName applies to all components.
    Register that is specific to -H denoted by "_PCH_S_" in component name.
    Register that is specific to -N denoted by "_PCH_N_" in component name.
    Register that is specific to -LP denoted by "_PCH_LP_" in component name.
  - SubsystemName:
    This field indicates the subsystem name of the component that the register belongs to
    (e.g. PCIE, USB, SATA, GPIO, PMC etc.).
  - RegisterSpace:
    MEM - MMIO space register of subsystem.
    IO  - IO space register of subsystem.
    PCR - Private configuration register of subsystem.
    CFG - PCI configuration space register of subsystem.
  - RegisterName:
    Full register name.

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
#ifndef _LPSS_I2C_DEVICE_ID_PTL_H_
#define _LPSS_I2C_DEVICE_ID_PTL_H_

//
//  LPSS I2C0 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C0_DEVICE_ID        0xE478
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C0_DEVICE_ID        0xE378
//
//  LPSS I2C1 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C1_DEVICE_ID        0xE479
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C1_DEVICE_ID        0xE379
//
//  LPSS I2C2 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C2_DEVICE_ID        0xE47A
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C2_DEVICE_ID        0xE37A
//
//  LPSS I2C3 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C3_DEVICE_ID        0xE47B
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C3_DEVICE_ID        0xE37B
//
//  LPSS I2C4 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C4_DEVICE_ID        0xE450
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C4_DEVICE_ID        0xE350
//
//  LPSS I2C5 Controller DevId
//
#define V_PTL_PCD_P_LPSS_I2C_CFG_I2C5_DEVICE_ID        0xE451
#define V_PTL_PCD_H_LPSS_I2C_CFG_I2C5_DEVICE_ID        0xE351


#endif //_LPSS_I2C_DEVICE_ID_PTL_H_
