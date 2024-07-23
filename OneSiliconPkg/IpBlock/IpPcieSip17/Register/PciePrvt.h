/** @file
  PCIE_PRVT.h

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2007 Intel Corporation.

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

  This file contains Silicon register definitions.

  This is a generated file; please do not modify it directly.

**/
#ifndef _PCIE_PRVT_h
#define _PCIE_PRVT_h



// IP GUID: 9c70b5f0-c685-4db9-9bc3-9efbf5016183
#define PCIE_PRVT_ID 0x9c70b5f0c6854db9

//************************************************ RegisterStructs


/** PCD desc:
  */

#define PCD_PCIE_PRVT_REG                (0x00003e00U)

typedef union {
  struct {
    UINT32 rp0fn : 3;
    UINT32 rp0ch : 1;
    UINT32 rp1fn : 3;
    UINT32 rp1ch : 1;
    UINT32 rp2fn : 3;
    UINT32 rp2ch : 1;
    UINT32 rp3fn : 3;
    UINT32 rp3ch : 1;
    UINT32 p0d : 1;
    UINT32 p1d : 1;
    UINT32 p2d : 1;
    UINT32 p3d : 1;
    UINT32 p4d : 1;
    UINT32 p5d : 1;
    UINT32 p6d : 1;
    UINT32 p7d : 1;
    UINT32 p8d : 1;
    UINT32 p9d : 1;
    UINT32 p10d : 1;
    UINT32 p11d : 1;
    UINT32 p12d : 1;
    UINT32 p13d : 1;
    UINT32 p14d : 1;
    UINT32 p15d : 1;
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} PCD_PCIE_PRVT_STRUCT;

#define PCD_PCIE_PRVT_WIDTH 32
#define PCD_PCIE_PRVT_DEFAULT 0x00000000U
#define PCD_PCIE_PRVT_ENDPOINT PCIE_PRVT

#define PCD_PCIE_PRVT_RP0FN_LSB 0x0000
#define PCD_PCIE_PRVT_RP0FN_SIZE 0x0003
#define PCD_PCIE_PRVT_RP0CH_LSB 0x0003
#define PCD_PCIE_PRVT_RP0CH_SIZE 0x0001
#define PCD_PCIE_PRVT_RP1FN_LSB 0x0004
#define PCD_PCIE_PRVT_RP1FN_SIZE 0x0003
#define PCD_PCIE_PRVT_RP1CH_LSB 0x0007
#define PCD_PCIE_PRVT_RP1CH_SIZE 0x0001
#define PCD_PCIE_PRVT_RP2FN_LSB 0x0008
#define PCD_PCIE_PRVT_RP2FN_SIZE 0x0003
#define PCD_PCIE_PRVT_RP2CH_LSB 0x000b
#define PCD_PCIE_PRVT_RP2CH_SIZE 0x0001
#define PCD_PCIE_PRVT_RP3FN_LSB 0x000c
#define PCD_PCIE_PRVT_RP3FN_SIZE 0x0003
#define PCD_PCIE_PRVT_RP3CH_LSB 0x000f
#define PCD_PCIE_PRVT_RP3CH_SIZE 0x0001
#define PCD_PCIE_PRVT_P0D_LSB 0x0010
#define PCD_PCIE_PRVT_P0D_SIZE 0x0001
#define PCD_PCIE_PRVT_P1D_LSB 0x0011
#define PCD_PCIE_PRVT_P1D_SIZE 0x0001
#define PCD_PCIE_PRVT_P2D_LSB 0x0012
#define PCD_PCIE_PRVT_P2D_SIZE 0x0001
#define PCD_PCIE_PRVT_P3D_LSB 0x0013
#define PCD_PCIE_PRVT_P3D_SIZE 0x0001
#define PCD_PCIE_PRVT_P4D_LSB 0x0014
#define PCD_PCIE_PRVT_P4D_SIZE 0x0001
#define PCD_PCIE_PRVT_P5D_LSB 0x0015
#define PCD_PCIE_PRVT_P5D_SIZE 0x0001
#define PCD_PCIE_PRVT_P6D_LSB 0x0016
#define PCD_PCIE_PRVT_P6D_SIZE 0x0001
#define PCD_PCIE_PRVT_P7D_LSB 0x0017
#define PCD_PCIE_PRVT_P7D_SIZE 0x0001
#define PCD_PCIE_PRVT_P8D_LSB 0x0018
#define PCD_PCIE_PRVT_P8D_SIZE 0x0001
#define PCD_PCIE_PRVT_P9D_LSB 0x0019
#define PCD_PCIE_PRVT_P9D_SIZE 0x0001
#define PCD_PCIE_PRVT_P10D_LSB 0x001a
#define PCD_PCIE_PRVT_P10D_SIZE 0x0001
#define PCD_PCIE_PRVT_P11D_LSB 0x001b
#define PCD_PCIE_PRVT_P11D_SIZE 0x0001
#define PCD_PCIE_PRVT_P12D_LSB 0x001c
#define PCD_PCIE_PRVT_P12D_SIZE 0x0001
#define PCD_PCIE_PRVT_P13D_LSB 0x001d
#define PCD_PCIE_PRVT_P13D_SIZE 0x0001
#define PCD_PCIE_PRVT_P14D_LSB 0x001e
#define PCD_PCIE_PRVT_P14D_SIZE 0x0001
#define PCD_PCIE_PRVT_P15D_LSB 0x001f
#define PCD_PCIE_PRVT_P15D_SIZE 0x0001



/** IMRLE desc:
  */

#define IMRLE_PCIE_PRVT_REG              (0x00003e1cU)

typedef union {
  struct {
    UINT32 rsvd_0 : 31;
    UINT32 srl : 1;
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} IMRLE_PCIE_PRVT_STRUCT;

#define IMRLE_PCIE_PRVT_WIDTH 32
#define IMRLE_PCIE_PRVT_DEFAULT 0x00000000U
#define IMRLE_PCIE_PRVT_ENDPOINT PCIE_PRVT

#define IMRLE_PCIE_PRVT_SRL_LSB 0x001f
#define IMRLE_PCIE_PRVT_SRL_SIZE 0x0001



/** SRL desc:
  */

#define SRL_PCIE_PRVT_REG                (0x00003e24U)

typedef union {
  struct {
    UINT32 srl : 1;
    UINT32 rsvd_m : 31;
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} SRL_PCIE_PRVT_STRUCT;

#define SRL_PCIE_PRVT_WIDTH 32
#define SRL_PCIE_PRVT_DEFAULT 0x00000000U
#define SRL_PCIE_PRVT_ENDPOINT PCIE_PRVT

#define SRL_PCIE_PRVT_SRL_LSB 0x0000
#define SRL_PCIE_PRVT_SRL_SIZE 0x0001
#define SRL_PCIE_PRVT_RSVD_M_LSB 0x0001
#define SRL_PCIE_PRVT_RSVD_M_SIZE 0x001f



/** DRPC desc:
  */

#define DRPC_PCIE_PRVT_REG               (0x00003e28U)

typedef union {
  struct {
    UINT32 drpcfoe : 1;
    UINT32 dlrovr : 1;
    UINT32 drpcovr : 3;
    UINT32 rsvd_0 : 26;
    UINT32 drpcud : 1;
  }     Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} DRPC_PCIE_PRVT_STRUCT;

#define DRPC_PCIE_PRVT_WIDTH 32
#define DRPC_PCIE_PRVT_DEFAULT 0x00000000U
#define DRPC_PCIE_PRVT_ENDPOINT PCIE_PRVT

#define DRPC_PCIE_PRVT_DRPCFOE_LSB 0x0000
#define DRPC_PCIE_PRVT_DRPCFOE_SIZE 0x0001
#define DRPC_PCIE_PRVT_DLROVR_LSB 0x0001
#define DRPC_PCIE_PRVT_DLROVR_SIZE 0x0001
#define DRPC_PCIE_PRVT_DRPCOVR_LSB 0x0002
#define DRPC_PCIE_PRVT_DRPCOVR_SIZE 0x0003
#define DRPC_PCIE_PRVT_DRPCUD_LSB 0x001f
#define DRPC_PCIE_PRVT_DRPCUD_SIZE 0x0001



//************************************************ /RegisterStructs
#endif      // _PCIE_PRVT_h
