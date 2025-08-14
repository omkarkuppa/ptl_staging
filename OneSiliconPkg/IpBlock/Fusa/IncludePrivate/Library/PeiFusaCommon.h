/** @file
  PEI FUSA COMMON Header File

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2025 Intel Corporation.

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

#ifndef _PEI_FUSA_COMMON_H_
#define _PEI_FUSA_COMMON_H_

#define PORTID_HBO0             0x54
#define PORTID_HBO1             0x53
#define PORTID_SNCU             0x4B
#define PORTID_VTU              0x50
#define PORTID_PUNIT            0x46
#define PORTID_IPU_PB_PID       0x88
#define PORTID_VPU_PB_PID       0x90
#define PORTID_MEDIA_PB         0x69
#define PORTID_GT_PB            0x7B
#define PORTID_IAX_PB           0x8E
#define PORTID_DNI2CFI          0x9A
#define PORTID_IDIB             0x45
#define PORTID_CCE0             0x60
#define PORTID_CCE1             0x61
#define PORTID_CCF_SANTA1       0x33
#define PORTID_CCF_SANTA0       0x34
#define PORTID_IOC              0x58
#define PORTID_IOCCE            0x5D
#define PORTID_IVTU             0x59

#define SA_SEG_NUM              0x00
#define SA_MC_BUS               0x00
#define SA_MC_DEV               0x00
#define SA_MC_FUN               0x00
#define SA_GT_DEV               0x02
#define SAFBAR_HOSTBRIDGE_CFG_REG        (0x00000068U)
#define DEV0_BAR_EN             0x0001
#define SEGMENT_ID_C_DIE        0xEE
#define SEGMENT_ID_G_DIE        0xF0
#define SEGMENT_ID_IOE          0xF3
#define MCHBAR_HOSTBRIDGE_CFG_REG        (0x00000048U)

#define SAFBAR_MEMORY_ADDRESS(Segment,PortId,Offset) \
  ( ((Offset) & 0xFFF)                 | \
    (((PortId) & 0xFF) << 12)          | \
    (((Segment) & 0x1F) << 20)           \
  )

typedef union {
  struct {
    UINT32 CFI_ParityChk_En :  1;  // Bits 0:0
    UINT32 CFI_ParityGen_En :  1;  // Bits 1:1
    UINT32 LockBit          :  1;  // Bits 2:2
    UINT32 RSVD             :  5;  // Bits 31:3
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_GLOBAL_ENABLEMENT_STRUCT;


typedef union {
  struct {
    UINT32 RX0_Req_Chk_Sts     :  1;  // Bits 0:0
    UINT32 RX0_DataHdr_Chk_Sts :  1;  // Bits 1:1
    UINT32 RX0_Data_Chk_Sts    :  1;  // Bits 2:2
    UINT32 Rsvd                :  1;  // Bits 3:3
    UINT32 RX1_Req_Chk_Sts     :  1;  // Bits 4:4
    UINT32 RX1_DataHdr_Chk_Sts :  1;  // Bits 5:5
    UINT32 RX1_Data_Chk_Sts    :  1;  // Bits 6:6
    UINT32 Rsvd2               :  1;  // Bits 7:7
    UINT32 RX2_Req_Chk_Sts     :  1;  // Bits 8:8
    UINT32 RX2_DataHdr_Chk_Sts :  1;  // Bits 9:9
    UINT32 RX2_Data_Chk_Sts    :  1;  // Bits 10:10
    UINT32 Rsvd3               :  1;  // Bits 11:11
    UINT32 RX3_Req_Chk_Sts     :  1;  // Bits 12:12
    UINT32 RX3_DataHdr_Chk_Sts :  1;  // Bits 13:13
    UINT32 RX3_Data_Chk_Sts    :  1;  // Bits 14:14
    UINT32 Rsvd4               :  1;  // Bits 15:15
    UINT32 RX0_Req_Chk_En      :  1;  // Bits 16:16
    UINT32 RX0_DataHdr_Chk_En  :  1;  // Bits 17:17
    UINT32 RX0_Data_Chk_En     :  1;  // Bits 18:18
    UINT32 Rsvd5               :  1;  // Bits 19:19
    UINT32 RX1_Req_Chk_En      :  1;  // Bits 20:20
    UINT32 RX1_DataHdr_Chk_En  :  1;  // Bits 21:21
    UINT32 RX1_Data_Chk_En     :  1;  // Bits 22:22
    UINT32 Rsvd6               :  1;  // Bits 23:23
    UINT32 RX2_Req_Chk_En      :  1;  // Bits 24:24
    UINT32 RX2_DataHdr_Chk_En  :  1;  // Bits 25:25
    UINT32 RX2_Data_Chk_En     :  1;  // Bits 26:26
    UINT32 Rsvd7               :  1;  // Bits 27:27
    UINT32 RX3_Req_Chk_En      :  1;  // Bits 28:28
    UINT32 RX3_DataHdr_Chk_En  :  1;  // Bits 29:29
    UINT32 RX3_Data_Chk_En     :  1;  // Bits 30:30
    UINT32 Rsvd8               :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_PARITY_STS_CTL_0_STRUCT;

typedef union {
  struct {
    UINT32 RX0_Req_Fault_Res     :  1;  // Bits 0:0
    UINT32 RX0_DataHdr_Fault_Res :  1;  // Bits 1:1
    UINT32 RX0_Data_Fault_Res    :  1;  // Bits 2:2
    UINT32 Rsvd                  :  1;  // Bits 3:3
    UINT32 RX1_Req_Fault_Res     :  1;  // Bits 4:4
    UINT32 RX1_DataHdr_Fault_Res :  1;  // Bits 5:5
    UINT32 RX1_Data_Fault_Res    :  1;  // Bits 6:6
    UINT32 Rsvd2                 :  1;  // Bits 7:7
    UINT32 RX2_Req_Fault_Res     :  1;  // Bits 8:8
    UINT32 RX2_DataHdr_Fault_Res :  1;  // Bits 9:9
    UINT32 RX2_Data_Fault_Res    :  1;  // Bits 10:10
    UINT32 Rsvd3                 :  1;  // Bits 11:11
    UINT32 RX3_Req_Fault_Res     :  1;  // Bits 12:12
    UINT32 RX3_DataHdr_Fault_Res :  1;  // Bits 13:13
    UINT32 RX3_Data_Fault_Res    :  1;  // Bits 14:14
    UINT32 Rsvd4                 :  1;  // Bits 15:15
    UINT32 RX0_Req_Fault_inj     :  1;  // Bits 16:16
    UINT32 RX0_DataHdr_Fault_inj :  1;  // Bits 17:17
    UINT32 RX0_Data_Fault_inj    :  1;  // Bits 18:18
    UINT32 Rsvd5                 :  1;  // Bits 19:19
    UINT32 RX1_Req_Fault_inj     :  1;  // Bits 20:20
    UINT32 RX1_DataHdr_Fault_inj :  1;  // Bits 21:21
    UINT32 RX1_Data_Fault_inj    :  1;  // Bits 22:22
    UINT32 Rsvd6                 :  1;  // Bits 23:23
    UINT32 RX2_Req_Fault_inj     :  1;  // Bits 24:24
    UINT32 RX2_DataHdr_Fault_inj :  1;  // Bits 25:25
    UINT32 RX2_Data_Fault_inj    :  1;  // Bits 26:26
    UINT32 Rsvd7                 :  1;  // Bits 27:27
    UINT32 RX3_Req_Fault_inj     :  1;  // Bits 28:28
    UINT32 RX3_DataHdr_Fault_inj :  1;  // Bits 29:29
    UINT32 RX3_Data_Fault_inj    :  1;  // Bits 30:30
    UINT32 Rsvd8                 :  1;  // Bits 31:31
  } Bits;
  UINT32 Data;
  UINT16 Data16[2];
  UINT8  Data8[4];
} FUSA_PARITY_CTC_CTL_0_STRUCT;

#endif // _PEI_FUSA_COMMON_H_
