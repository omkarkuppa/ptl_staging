#ifndef __MrcRegisterPtl0xxx_h__
#define __MrcRegisterPtl0xxx_h__

/**
@file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

#pragma pack(push, 1)


#define DATA0CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000000)
#define DATA0CH0_CR_DDRDATADQRANK0LANE0_DEF                            (0x00000000)
#define DATA0CH0_CR_DDRDATADQRANK0LANE0_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_OFF                     ( 0)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_WID                     ( 6)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_MSK                     (0x0000003F)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_MIN                     (0)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_MAX                     (63) // 0x0000003F
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_DEF                     (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_rsvd_HSH                     (0x06000000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_OFF   ( 6)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_WID   ( 4)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_MSK   (0x000003C0)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_MIN   (-8)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_MAX   (7) // 0x00000007
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_DEF   (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskewOffset_HSH   (0x840C0000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_OFF         (10)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_WID         ( 6)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_MSK         (0x0000FC00)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_MIN         (0)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_MAX         (63) // 0x0000003F
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQPerBitDeskew_HSH         (0x06140000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_OFF          (16)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_WID          ( 5)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_MSK          (0x001F0000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_MIN          (-16)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_MAX          (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetR_HSH          (0x85200000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_OFF          (21)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_WID          ( 5)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_MSK          (0x03E00000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_MIN          (-16)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_MAX          (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_RxDQVrefOffsetF_HSH          (0x852A0000)

  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_OFF         (26)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_WID         ( 6)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_MSK         (0xFC000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_MIN         (0)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_MAX         (63) // 0x0000003F
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRDATADQRANK0LANE0_TxDQPerBitDeskew_HSH         (0x06340000)

#define DATA0CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000004)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000008)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x0000000C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000010)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000014)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000018)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x0000001C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000020)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000024)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000028)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x0000002C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000030)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000034)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000038)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x0000003C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQSRANK0_REG                                (0x00000040)
#define DATA0CH0_CR_DDRDATADQSRANK0_DEF                                (0x00000000)
#define DATA0CH0_CR_DDRDATADQSRANK0_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_OFF                         ( 0)
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_WID                         ( 7)
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_MSK                         (0x0000007F)
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_MIN                         (0)
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_MAX                         (127) // 0x0000007F
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_DEF                         (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_rsvd_HSH                         (0x07000040)

  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_OFF              ( 7)
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_WID              ( 5)
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_MSK              (0x00000F80)
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_MIN              (-16)
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_RxDQSVrefOffset_HSH              (0x850E0040)

  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_OFF                        (12)
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_WID                        ( 4)
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_MSK                        (0x0000F000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_MIN                        (0)
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_MAX                        (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_DEF                        (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_Rsvd1_HSH                        (0x04180040)

  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_OFF                   (16)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_WID                   ( 6)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_MSK                   (0x003F0000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_MIN                   (-32)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_MAX                   (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsTcoCode_HSH                   (0x86200040)

  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_OFF              (22)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_WID              ( 5)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_MSK              (0x07C00000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_MIN              (-16)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPrNfTcoDelay_HSH              (0x852C0040)

  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_OFF              (27)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_WID              ( 5)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_MSK              (0xF8000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_MIN              (-16)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRDATADQSRANK0_DqsPfNrTcoDelay_HSH              (0x85360040)

#define DATA0CH0_CR_DDRDATADQSRANK1_REG                                (0x00000044)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA0CH0_CR_TXCONTROL0RANK0_REG                                (0x00000048)
#define DATA0CH0_CR_TXCONTROL0RANK0_DEF                                (0x03FFFFFF)
#define DATA0CH0_CR_TXCONTROL0RANK0_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_OFF                    ( 0)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_WID                    (11)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_MSK                    (0x000007FF)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_MIN                    (0)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_MAX                    (2047) // 0x000007FF
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_DEF                    (0x000007FF)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqDelay_HSH                    (0x0B000048)

  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_OFF                   (11)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_WID                   (11)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_MSK                   (0x003FF800)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_MIN                   (0)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_MAX                   (2047) // 0x000007FF
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_DEF                   (0x000007FF)
  #define DATA0CH0_CR_TXCONTROL0RANK0_TxDqsDelay_HSH                   (0x0B160048)

  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_OFF             (22)
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_WID             ( 6)
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_MSK             (0x0FC00000)
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_MIN             (-32)
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_MAX             (31) // 0x0000001F
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_DEF             (0x0000000F)
  #define DATA0CH0_CR_TXCONTROL0RANK0_DqTxEqualization_HSH             (0x862C0048)

  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_OFF            (28)
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_WID            ( 4)
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_MSK            (0xF0000000)
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_MIN            (0)
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_MAX            (15) // 0x0000000F
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_DEF            (0x00000000)
  #define DATA0CH0_CR_TXCONTROL0RANK0_dqstxequalization_HSH            (0x04380048)

#define DATA0CH0_CR_TXCONTROL0RANK1_REG                                (0x0000004C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000050)
#define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_DEF                           (0x00000000)
#define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_OFF             ( 0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_WID             ( 8)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_MSK             (0x000000FF)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_MIN             (-128)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_MAX             (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RcvEnOffset_HSH             (0x88000050)

  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_OFF             ( 8)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_WID             ( 7)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_MSK             (0x00007F00)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_MIN             (-64)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_MAX             (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_RxDqsOffset_HSH             (0x87100050)

  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_OFF              (15)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_WID              ( 8)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_MSK              (0x007F8000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_MIN              (-128)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_MAX              (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqOffset_HSH              (0x881E0050)

  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_OFF             (23)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_WID             ( 8)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_MSK             (0x7F800000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_MIN             (-128)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_MAX             (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_TxDqsOffset_HSH             (0x882E0050)

  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_OFF                   (31)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_WID                   ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_MSK                   (0x80000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_Spare_HSH                   (0x013E0050)

#define DATA0CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000054)
#define DATA0CH0_CR_DDRCRDATACONTROL2_DEF                              (0x00412002)
#define DATA0CH0_CR_DDRCRDATACONTROL2_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_OFF                   ( 0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_WID                   ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_MSK                   (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BertMode_HSH                   (0x01000054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_OFF                   ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_WID                   ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_MSK                   (0x00000002)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_DEF                   (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_GearMode_HSH                   (0x01020054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_OFF              ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_WID              ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_MSK              (0x0000000C)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_MAX              (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALTailCtrl_HSH              (0x02040054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_OFF                 ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_WID                 ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_MSK                 (0x00000070)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_MAX                 (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_wrpreamble_HSH                 (0x03080054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_OFF          ( 7)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_WID          ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_MSK          (0x00000080)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_MIN          (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_MAX          (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_BlockSdlWithRcvEn_HSH          (0x010E0054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_OFF                 ( 8)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_WID                 ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_MSK                 (0x00000300)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_MAX                 (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RcvEnExtFF_HSH                 (0x02100054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_OFF          (10)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_WID          ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_MSK          (0x00001C00)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_MIN          (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVddqProgMFC_HSH          (0x03140054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_OFF           (13)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_WID           ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_MSK           (0x0000E000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_MAX           (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_DEF           (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxVrefVttProgMFC_HSH           (0x031A0054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_OFF        (16)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_WID        ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_MSK        (0x000F0000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_MIN        (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_MAX        (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_DEF        (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxBurstLenBy2Minus1_HSH        (0x04200054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_OFF                (20)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_WID                ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_MSK                (0x00100000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_MAX                (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_EnDqsNRcvEn_HSH                (0x01280054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_OFF        (21)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_WID        ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_MSK        (0x00200000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_MIN        (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_MAX        (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_DisableDqsOdtStatic_HSH        (0x012A0054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_OFF                (22)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_WID                ( 6)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_MSK                (0x0FC00000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_MAX                (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_DEF                (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkStgNum_HSH                (0x062C0054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_OFF               (28)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_WID               ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_MSK               (0x10000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_ForceRcvEnOn_HSH               (0x01380054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_OFF                (29)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_WID                ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_MSK                (0x60000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_MAX                (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxClkDlySel_HSH                (0x023A0054)

  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_OFF                 (31)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_WID                 ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_MSK                 (0x80000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_MAX                 (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL2_RxSALSpeed_HSH                 (0x013E0054)

#define DATA0CH0_CR_DCCFSMCONTROL_REG                                  (0x00000058)
#define DATA0CH0_CR_DCCFSMCONTROL_DEF                                  (0x00801FFF)
#define DATA0CH0_CR_DCCFSMCONTROL_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_OFF                         ( 0)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_WID                         ( 2)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_MSK                         (0x00000003)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_MIN                         (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_MAX                         (3) // 0x00000003
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_DEF                         (0x00000003)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankEn_HSH                         (0x02000058)

  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_OFF                         ( 2)
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_WID                         (10)
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_MSK                         (0x00000FFC)
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_MIN                         (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_MAX                         (1023) // 0x000003FF
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_DEF                         (0x000003FF)
  #define DATA0CH0_CR_DCCFSMCONTROL_LaneEn_HSH                         (0x0A040058)

  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_OFF                        (12)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_WID                        ( 2)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_MSK                        (0x00003000)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_DEF                        (0x00000001)
  #define DATA0CH0_CR_DCCFSMCONTROL_RankMap_HSH                        (0x02180058)

  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_OFF                     (14)
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_WID                     ( 5)
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_MSK                     (0x0007C000)
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_MIN                     (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_MAX                     (31) // 0x0000001F
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_DccSamples_HSH                     (0x051C0058)

  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_OFF                    (19)
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_WID                    ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_MSK                    (0x00080000)
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_MIN                    (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_MAX                    (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_DEF                    (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_SaveFullDcc_HSH                    (0x01260058)

  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_OFF                         (20)
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_WID                         ( 3)
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_MSK                         (0x00700000)
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_MIN                         (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_MAX                         (7) // 0x00000007
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_DEF                         (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_RORank_HSH                         (0x03280058)

  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_OFF                     (23)
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_WID                     ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_MSK                     (0x00800000)
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_MIN                     (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_DEF                     (0x00000001)
  #define DATA0CH0_CR_DCCFSMCONTROL_EnRankOvrd_HSH                     (0x012E0058)

  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_OFF                      (24)
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_WID                      ( 2)
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_MSK                      (0x03000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_MIN                      (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_MAX                      (3) // 0x00000003
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_DEF                      (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_MeasPoint_HSH                      (0x02300058)

  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_OFF                    (26)
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_WID                    ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_MSK                    (0x04000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_MIN                    (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_MAX                    (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_DEF                    (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_SkipCRWrite_HSH                    (0x01340058)

  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_OFF                       (27)
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_WID                       ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_MSK                       (0x08000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_MIN                       (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_MAX                       (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_DEF                       (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_ForceAck_HSH                       (0x01360058)

  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_OFF              (28)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_WID              ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_MSK              (0x10000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_MIN              (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_DEF              (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeEnable_HSH              (0x01380058)

  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_OFF               (29)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_WID               ( 1)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_MSK               (0x20000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_MIN               (0)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_DEF               (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_LargeChangeReset_HSH               (0x013A0058)

  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_OFF                   (30)
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_WID                   ( 2)
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_MSK                   (0xC0000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_MIN                   (-2)
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCFSMCONTROL_TargetBitDBI_HSH                   (0x823C0058)

#define DATA0CH0_CR_DCCCALCCONTROL_REG                                 (0x0000005C)
#define DATA0CH0_CR_DCCCALCCONTROL_DEF                                 (0x0000000F)
#define DATA0CH0_CR_DCCCALCCONTROL_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_OFF                   ( 0)
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_WID                   ( 5)
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_MSK                   (0x0000001F)
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_MIN                   (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_MAX                   (31) // 0x0000001F
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_DEF                   (0x0000000F)
  #define DATA0CH0_CR_DCCCALCCONTROL_DccStepSize_HSH                   (0x0500005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_OFF                    ( 5)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_WID                    ( 4)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_MSK                    (0x000001E0)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_MIN                    (-8)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_MAX                    (7) // 0x00000007
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_DEF                    (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR0_HSH                    (0x840A005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_OFF                    ( 9)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_WID                    ( 4)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_MSK                    (0x00001E00)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_MIN                    (-8)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_MAX                    (7) // 0x00000007
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_DEF                    (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqTargetR1_HSH                    (0x8412005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_OFF                   (13)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_WID                   ( 4)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_MSK                   (0x0001E000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_MIN                   (-8)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_MAX                   (7) // 0x00000007
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR0_HSH                   (0x841A005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_OFF                   (17)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_WID                   ( 4)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_MSK                   (0x001E0000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_MIN                   (-8)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_MAX                   (7) // 0x00000007
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_DqsTargetR1_HSH                   (0x8422005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_OFF              (21)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_WID              ( 4)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_MSK              (0x01E00000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_MIN              (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_DEF              (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeLane_HSH              (0x042A005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_OFF               (25)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_WID               ( 1)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_MSK               (0x02000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_MIN               (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_DEF               (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeMax_HSH               (0x0132005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_OFF              (26)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_WID              ( 1)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_MSK              (0x04000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_MIN              (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_DEF              (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_TrackExtremeRank_HSH              (0x0134005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_OFF                   (27)
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_WID                   ( 3)
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_MSK                   (0x38000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_MIN                   (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_MAX                   (7) // 0x00000007
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_LargeChange_HSH                   (0x0336005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_OFF                      (30)
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_WID                      ( 1)
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_MSK                      (0x40000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_MIN                      (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_DEF                      (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_WrLUTDq0_HSH                      (0x013C005C)

  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_OFF                          (31)
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_WID                          ( 1)
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_MSK                          (0x80000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_MIN                          (0)
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_MAX                          (1) // 0x00000001
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DCCCALCCONTROL_rsvd_HSH                          (0x013E005C)

#define DATA0CH0_CR_DCCLANETARGET_REG                                  (0x00000060)
#define DATA0CH0_CR_DCCLANETARGET_DEF                                  (0x00000000)
#define DATA0CH0_CR_DCCLANETARGET_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_OFF                     ( 0)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_MSK                     (0x0000000F)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit0_HSH                     (0x84000060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_OFF                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_MSK                     (0x000000F0)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit1_HSH                     (0x84080060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_OFF                     ( 8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_MSK                     (0x00000F00)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit2_HSH                     (0x84100060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_OFF                     (12)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_MSK                     (0x0000F000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit3_HSH                     (0x84180060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_OFF                     (16)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_MSK                     (0x000F0000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit4_HSH                     (0x84200060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_OFF                     (20)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_MSK                     (0x00F00000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit5_HSH                     (0x84280060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_OFF                     (24)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_MSK                     (0x0F000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit6_HSH                     (0x84300060)

  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_OFF                     (28)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_WID                     ( 4)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_MSK                     (0xF0000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_MIN                     (-8)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCLANETARGET_TargetBit7_HSH                     (0x84380060)

#define DATA0CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000064)
#define DATA0CH0_CR_DDRCRRANKMUXDELAY_DEF                              (0x00000000)
#define DATA0CH0_CR_DDRCRRANKMUXDELAY_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_OFF       ( 0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_WID       ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_MSK       (0x00000007)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_MIN       (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_MAX       (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqsOffset_HSH       (0x03000064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_OFF        ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_WID        ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_MSK        (0x00000038)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_MIN        (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_MAX        (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForDqOffset_HSH        (0x03060064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_OFF            ( 6)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_WID            ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_MSK            (0x000001C0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_MIN            (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_MAX            (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRxPi_HSH            (0x030C0064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_OFF           ( 9)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_WID           ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_MSK           (0x00000E00)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_MIN           (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_MAX           (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDelayForRcvEn_HSH           (0x03120064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_OFF           (12)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_WID           ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_MSK           (0x00007000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_MIN           (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_MAX           (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForEq_HSH           (0x03180064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_OFF            (15)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_WID            ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_MSK            (0x00038000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_MIN            (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_MAX            (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForEq_HSH            (0x031E0064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_OFF           (18)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_WID           ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_MSK           (0x001C0000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_MIN           (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_MAX           (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQSDelayForPi_HSH           (0x03240064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_OFF            (21)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_WID            ( 3)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_MSK            (0x00E00000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_MIN            (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_MAX            (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_R2RDQDelayForPi_HSH            (0x032A0064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_OFF             (24)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_WID             ( 4)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_MSK             (0x0F000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_MIN             (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_MAX             (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_RxRankMuxDelay_HSH             (0x04300064)

  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_OFF             (28)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_WID             ( 4)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_MSK             (0xF0000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_MIN             (0)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_MAX             (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRRANKMUXDELAY_TxRankMuxDelay_HSH             (0x04380064)

#define DATA0CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000068)
#define DATA0CH0_CR_DDRCRDLLCONTROL0_DEF                               (0x06100018)
#define DATA0CH0_CR_DDRCRDLLCONTROL0_VOLATILE_BITFIELDS_MSK            (0x01E3FF80)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_OFF                 ( 0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_WID                 ( 2)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_MSK                 (0x00000003)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_MAX                 (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLPHClkSel_HSH                 (0x02000068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_OFF            ( 2)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_MSK            (0x00000004)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_MIN            (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDacCodeFreeze_HSH            (0x01040068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_OFF                     ( 3)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_WID                     ( 3)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_MSK                     (0x00000038)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_MAX                     (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_DEF                     (0x00000003)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockCtr_HSH                     (0x03060068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_OFF                     ( 6)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_WID                     ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_MSK                     (0x00000040)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_DEF                     (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_LockSel_HSH                     (0x010C0068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_OFF                    ( 7)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_WID                    (11)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_MSK                    (0x0003FF80)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_MIN                    (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_MAX                    (2047) // 0x000007FF
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_DEF                    (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_VctlInit_HSH                    (0x0B8E0068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_OFF              (18)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_WID              ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_MSK              (0x00040000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_MIN              (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllPwrDownOvrd_HSH              (0x01240068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_OFF             (19)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_MSK             (0x00080000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_MIN             (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllDcdTargetSel_HSH             (0x01260068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_OFF            (20)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_MSK            (0x00100000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_MIN            (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_DEF            (0x00000001)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCodeEn_HSH            (0x01280068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_OFF              (21)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_WID              ( 4)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_MSK              (0x01E00000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_MIN              (-8)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_MAX              (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DllVcdlDccCode_HSH              (0x84AA0068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_OFF                 (25)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_WID                 ( 3)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_MSK                 (0x0E000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_MAX                 (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_DEF                 (0x00000003)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_FastLockCtr_HSH                 (0x03320068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_OFF            (28)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_MSK            (0x10000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_MIN            (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnTurboOnStartUp_HSH            (0x01380068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_OFF                        (29)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_WID                        ( 1)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_MSK                        (0x20000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_MIN                        (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_MAX                        (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_DEF                        (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_EnWL_HSH                        (0x013A0068)

  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_OFF           (30)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_WID           ( 2)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_MSK           (0xC0000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_MIN           (0)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLCONTROL0_DLLSegmentDisable_HSH           (0x023C0068)

#define DATA0CH0_CR_DCCPILUT0_REG                                      (0x0000006C)
#define DATA0CH0_CR_DCCPILUT0_DEF                                      (0x00000000)
#define DATA0CH0_CR_DCCPILUT0_VOLATILE_BITFIELDS_MSK                   (0x00000000)

  #define DATA0CH0_CR_DCCPILUT0_Fine0_OFF                              ( 0)
  #define DATA0CH0_CR_DCCPILUT0_Fine0_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine0_MSK                              (0x0000000F)
  #define DATA0CH0_CR_DCCPILUT0_Fine0_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine0_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine0_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine0_HSH                              (0x8400006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine1_OFF                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine1_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine1_MSK                              (0x000000F0)
  #define DATA0CH0_CR_DCCPILUT0_Fine1_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine1_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine1_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine1_HSH                              (0x8408006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine2_OFF                              ( 8)
  #define DATA0CH0_CR_DCCPILUT0_Fine2_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine2_MSK                              (0x00000F00)
  #define DATA0CH0_CR_DCCPILUT0_Fine2_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine2_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine2_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine2_HSH                              (0x8410006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine3_OFF                              (12)
  #define DATA0CH0_CR_DCCPILUT0_Fine3_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine3_MSK                              (0x0000F000)
  #define DATA0CH0_CR_DCCPILUT0_Fine3_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine3_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine3_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine3_HSH                              (0x8418006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine4_OFF                              (16)
  #define DATA0CH0_CR_DCCPILUT0_Fine4_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine4_MSK                              (0x000F0000)
  #define DATA0CH0_CR_DCCPILUT0_Fine4_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine4_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine4_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine4_HSH                              (0x8420006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine5_OFF                              (20)
  #define DATA0CH0_CR_DCCPILUT0_Fine5_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine5_MSK                              (0x00F00000)
  #define DATA0CH0_CR_DCCPILUT0_Fine5_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine5_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine5_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine5_HSH                              (0x8428006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine6_OFF                              (24)
  #define DATA0CH0_CR_DCCPILUT0_Fine6_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine6_MSK                              (0x0F000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine6_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine6_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine6_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine6_HSH                              (0x8430006C)

  #define DATA0CH0_CR_DCCPILUT0_Fine7_OFF                              (28)
  #define DATA0CH0_CR_DCCPILUT0_Fine7_WID                              ( 4)
  #define DATA0CH0_CR_DCCPILUT0_Fine7_MSK                              (0xF0000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine7_MIN                              (-8)
  #define DATA0CH0_CR_DCCPILUT0_Fine7_MAX                              (7) // 0x00000007
  #define DATA0CH0_CR_DCCPILUT0_Fine7_DEF                              (0x00000000)
  #define DATA0CH0_CR_DCCPILUT0_Fine7_HSH                              (0x8438006C)

#define DATA0CH0_CR_REFPICONTROL_REG                                   (0x00000070)
#define DATA0CH0_CR_REFPICONTROL_DEF                                   (0x00000000)
#define DATA0CH0_CR_REFPICONTROL_VOLATILE_BITFIELDS_MSK                (0x01FF00FF)

  #define DATA0CH0_CR_REFPICONTROL_RefPi_OFF                           ( 0)
  #define DATA0CH0_CR_REFPICONTROL_RefPi_WID                           ( 8)
  #define DATA0CH0_CR_REFPICONTROL_RefPi_MSK                           (0x000000FF)
  #define DATA0CH0_CR_REFPICONTROL_RefPi_MIN                           (0)
  #define DATA0CH0_CR_REFPICONTROL_RefPi_MAX                           (255) // 0x000000FF
  #define DATA0CH0_CR_REFPICONTROL_RefPi_DEF                           (0x00000000)
  #define DATA0CH0_CR_REFPICONTROL_RefPi_HSH                           (0x08800070)

  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_OFF                     ( 8)
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_WID                     ( 8)
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_MSK                     (0x0000FF00)
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_MIN                     (0)
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_MAX                     (255) // 0x000000FF
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_DEF                     (0x00000000)
  #define DATA0CH0_CR_REFPICONTROL_RefPiOffset_HSH                     (0x08100070)

  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_OFF                     (16)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_WID                     ( 9)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_MSK                     (0x01FF0000)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_MIN                     (0)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_MAX                     (511) // 0x000001FF
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_DEF                     (0x00000000)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4Xover_HSH                     (0x09A00070)

  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_OFF               (25)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_WID               ( 7)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_MSK               (0xFE000000)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_MIN               (0)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_MAX               (127) // 0x0000007F
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_DEF               (0x00000000)
  #define DATA0CH0_CR_REFPICONTROL_RefPi4XoverOffset_HSH               (0x07320070)

#define DATA0CH0_CR_SPARE_REG                                          (0x00000074)
#define DATA0CH0_CR_SPARE_DEF                                          (0x00000000)
#define DATA0CH0_CR_SPARE_VOLATILE_BITFIELDS_MSK                       (0x00000000)

  #define DATA0CH0_CR_SPARE_spare_OFF                                  ( 0)
  #define DATA0CH0_CR_SPARE_spare_WID                                  (32)
  #define DATA0CH0_CR_SPARE_spare_MSK                                  (0xFFFFFFFF)
  #define DATA0CH0_CR_SPARE_spare_MIN                                  (0)
  #define DATA0CH0_CR_SPARE_spare_MAX                                  (4294967295) // 0xFFFFFFFF
  #define DATA0CH0_CR_SPARE_spare_DEF                                  (0x00000000)
  #define DATA0CH0_CR_SPARE_spare_HSH                                  (0x20000074)

#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000078)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DEF                         (0x00000000)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_OFF                  ( 0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_WID                  (15)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_MSK                  (0x00007FFF)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_MAX                  (32767) // 0x00007FFF
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_Rsvd_HSH                  (0x0F000078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_OFF             (15)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_WID             ( 1)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_MSK             (0x00008000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_MIN             (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_sparebits_HSH             (0x011E0078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_OFF            (16)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_WID            ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_MSK            (0x00030000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_MIN            (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_MAX            (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQPbiasCtl_HSH            (0x02200078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_OFF            (18)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_WID            ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_MSK            (0x000C0000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_MIN            (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_MAX            (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQNbiasCtl_HSH            (0x02240078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_OFF           (20)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_WID           ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_MSK           (0x00300000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_MIN           (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSPbiasCtl_HSH           (0x02280078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_OFF           (22)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_WID           ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_MSK           (0x00C00000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_MIN           (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSNbiasCtl_HSH           (0x022C0078)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_OFF              (24)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_WID              ( 8)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_MSK              (0xFF000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_MIN              (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_MAX              (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_DQSBonus_HSH              (0x08300078)

#define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x0000007C)
#define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DEF                         (0x00578000)
#define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_OFF       ( 0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_WID       ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_MSK       (0x00000001)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_MIN       (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_MAX       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_sdlmode_HSH       (0x0100007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_OFF   ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_WID   ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_MSK   (0x00000002)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_MIN   (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_MAX   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_DEF   (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_selphclkdll_HSH   (0x0102007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_OFF ( 2)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_WID ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_MSK (0x00000004)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_MIN (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_terdcdselrefclk_HSH (0x0104007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_OFF ( 3)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_WID ( 5)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_MSK (0x000000F8)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_MIN (-16)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_MAX (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_dqsmdll_tlclk_dlldcccode_HSH (0x8506007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_OFF           ( 8)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_WID           ( 2)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_MSK           (0x00000300)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_MIN           (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_SelWCKDQSPi_HSH           (0x0210007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_OFF     (10)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_WID     ( 2)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_MSK     (0x00000C00)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_MIN     (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_MAX     (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_DEF     (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_rxfifodcccode_sag_HSH     (0x0214007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_OFF       (12)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_WID       ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_MSK       (0x00001000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_MIN       (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_MAX       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_phen_ovrd_onsag_HSH       (0x0118007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_OFF   (13)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_WID   ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_MSK   (0x00002000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_MIN   (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_MAX   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_DEF   (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenHigh_HSH   (0x011A007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_OFF    (14)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_WID    ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_MSK    (0x00004000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_MIN    (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_MAX    (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_DEF    (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_DqIdleModeDrvenLow_HSH    (0x011C007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_OFF       (15)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_WID       ( 8)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_MSK       (0x007F8000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_MIN       (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_MAX       (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_DEF       (0x000000AF)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDqsDelaySatPN_HSH       (0x081E007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_OFF             (23)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_MSK             (0x00800000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_MIN             (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_RxDdrMode_HSH             (0x012E007C)

  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_OFF                  (24)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_WID                  ( 8)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_MSK                  (0xFF000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_MAX                  (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_Rsvd_HSH                  (0x0830007C)

#define DATA0CH0_CR_TERDCCCONTROL0_REG                                 (0x00000080)
#define DATA0CH0_CR_TERDCCCONTROL0_DEF                                 (0x00000000)
#define DATA0CH0_CR_TERDCCCONTROL0_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_OFF             ( 0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_WID             ( 2)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_MSK             (0x00000003)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_MIN             (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_MAX             (3) // 0x00000003
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_DEF             (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovrval_HSH             (0x02000080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_OFF              ( 2)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_WID              ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_MSK              (0x00000004)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_MIN              (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_DEF              (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcdselovren_HSH              (0x01040080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_OFF          ( 3)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_WID          ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_MSK          (0x00000008)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_MIN          (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_MAX          (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_DEF          (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terclkandselovrval_HSH          (0x01060080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_OFF            ( 4)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_WID            ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_MSK            (0x00000010)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_MIN            (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_DEF            (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_srzdcdpatterninv_HSH            (0x01080080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_OFF                    ( 5)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_WID                    ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_MSK                    (0x00000020)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_MIN                    (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_MAX                    (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_DEF                    (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_terdcden_HSH                    (0x010A0080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_OFF ( 6)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_WID ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_MSK (0x00000040)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_MIN (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_MAX (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_DEF (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_clkwckbuf_selterclkandovrval_HSH (0x010C0080)

  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_OFF      ( 7)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_WID      ( 1)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_MSK      (0x00000080)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_MIN      (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_MAX      (1) // 0x00000001
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_DEF      (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_i_ddrdcd_quatdcden_onsag_HSH      (0x010E0080)

  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_OFF                          ( 8)
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_WID                          (24)
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_MSK                          (0xFFFFFF00)
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_MIN                          (0)
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_MAX                          (16777215) // 0x00FFFFFF
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_TERDCCCONTROL0_rsvd_HSH                          (0x18100080)

#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000084)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_DEF                         (0x006DB6DB)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_OFF          ( 0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_MSK          (0x00000007)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq0_xtalksel_HSH          (0x03000084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_OFF          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_MSK          (0x00000038)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq1_xtalksel_HSH          (0x03060084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_OFF          ( 6)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_MSK          (0x000001C0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq2_xtalksel_HSH          (0x030C0084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_OFF          ( 9)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_MSK          (0x00000E00)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq3_xtalksel_HSH          (0x03120084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_OFF          (12)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_MSK          (0x00007000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq4_xtalksel_HSH          (0x03180084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_OFF          (15)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_MSK          (0x00038000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq5_xtalksel_HSH          (0x031E0084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_OFF          (18)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_MSK          (0x001C0000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq6_xtalksel_HSH          (0x03240084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_OFF          (21)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_MSK          (0x00E00000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_DEF          (0x00000003)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_dq7_xtalksel_HSH          (0x032A0084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_OFF               (24)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_WID               ( 1)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_MSK               (0x01000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_MIN               (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_xtalken_HSH               (0x01300084)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_OFF                 (25)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_WID                 ( 7)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_MSK                 (0xFE000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_MIN                 (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_MAX                 (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_spare_HSH                 (0x07320084)

#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000088)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DEF                         (0x00000000)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_OFF          ( 0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_MSK          (0x00000007)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap0_HSH          (0x03000088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_OFF          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_MSK          (0x00000038)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkCap1_HSH          (0x03060088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_OFF         ( 6)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_MSK         (0x000000C0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain0_HSH         (0x020C0088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_OFF         ( 8)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_MSK         (0x00000300)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ0XtalkGain1_HSH         (0x02100088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_OFF          (10)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_MSK          (0x00001C00)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap0_HSH          (0x03140088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_OFF          (13)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_MSK          (0x0000E000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkCap1_HSH          (0x031A0088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_OFF         (16)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_MSK         (0x00030000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain0_HSH         (0x02200088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_OFF         (18)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_MSK         (0x000C0000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ1XtalkGain1_HSH         (0x02240088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_OFF          (20)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_MSK          (0x00700000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap0_HSH          (0x03280088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_OFF          (23)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_MSK          (0x03800000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkCap1_HSH          (0x032E0088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_OFF         (26)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_MSK         (0x0C000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain0_HSH         (0x02340088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_OFF         (28)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_MSK         (0x30000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_DQ2XtalkGain1_HSH         (0x02380088)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_OFF                  (30)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_rsvd_HSH                  (0x023C0088)

#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000008C)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DEF                         (0x00000000)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_OFF          ( 0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_MSK          (0x00000007)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap0_HSH          (0x0300008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_OFF          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_MSK          (0x00000038)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkCap1_HSH          (0x0306008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_OFF         ( 6)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_MSK         (0x000000C0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain0_HSH         (0x020C008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_OFF         ( 8)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_MSK         (0x00000300)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ3XtalkGain1_HSH         (0x0210008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_OFF          (10)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_MSK          (0x00001C00)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap0_HSH          (0x0314008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_OFF          (13)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_MSK          (0x0000E000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkCap1_HSH          (0x031A008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_OFF         (16)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_MSK         (0x00030000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain0_HSH         (0x0220008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_OFF         (18)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_MSK         (0x000C0000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ4XtalkGain1_HSH         (0x0224008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_OFF          (20)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_MSK          (0x00700000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap0_HSH          (0x0328008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_OFF          (23)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_MSK          (0x03800000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkCap1_HSH          (0x032E008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_OFF         (26)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_MSK         (0x0C000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain0_HSH         (0x0234008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_OFF         (28)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_MSK         (0x30000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_DQ5XtalkGain1_HSH         (0x0238008C)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_OFF                  (30)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_rsvd_HSH                  (0x023C008C)

#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000090)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DEF                         (0x00000000)
#define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_OFF          ( 0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_MSK          (0x00000007)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap0_HSH          (0x03000090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_OFF          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_MSK          (0x00000038)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkCap1_HSH          (0x03060090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_OFF         ( 6)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_MSK         (0x000000C0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain0_HSH         (0x020C0090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_OFF         ( 8)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_MSK         (0x00000300)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ6XtalkGain1_HSH         (0x02100090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_OFF          (10)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_MSK          (0x00001C00)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap0_HSH          (0x03140090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_OFF          (13)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_MSK          (0x0000E000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkCap1_HSH          (0x031A0090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_OFF         (16)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_MSK         (0x00030000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain0_HSH         (0x02200090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_OFF         (18)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_MSK         (0x000C0000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ7XtalkGain1_HSH         (0x02240090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_OFF          (20)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_MSK          (0x00700000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap0_HSH          (0x03280090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_OFF          (23)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_WID          ( 3)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_MSK          (0x03800000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_MIN          (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_MAX          (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkCap1_HSH          (0x032E0090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_OFF         (26)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_MSK         (0x0C000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain0_HSH         (0x02340090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_OFF         (28)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_MSK         (0x30000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_MIN         (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_DQ8XtalkGain1_HSH         (0x02380090)

  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_OFF                  (30)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_rsvd_HSH                  (0x023C0090)

#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000094)
#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_DEF                      (0x00000000)
#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_VOLATILE_BITFIELDS_MSK   (0x3FFFFFFF)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_OFF ( 0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_MSK (0x0000003F)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq0_HSH (0x86800094)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_OFF ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_MSK (0x00000FC0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq1_HSH (0x868C0094)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_OFF (12)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_MSK (0x0003F000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq2_HSH (0x86980094)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_OFF (18)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_MSK (0x00FC0000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq3_HSH (0x86A40094)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_OFF (24)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_MSK (0x3F000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_TxDqDCCOffsetR0dq4_HSH (0x86B00094)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_OFF               (30)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_WID               ( 2)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_MSK               (0xC0000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_MIN               (0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_MAX               (3) // 0x00000003
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_rsvd_HSH               (0x023C0094)

#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000098)
#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_DEF         (0x00000000)
#define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_VOLATILE_BITFIELDS_MSK (0x3FFFFFFF)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_OFF ( 0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_MSK (0x0000003F)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq5_HSH (0x86800098)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_OFF ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_MSK (0x00000FC0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq6_HSH (0x868C0098)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_OFF (12)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_MSK (0x0003F000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR0dq7_HSH (0x86980098)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_OFF (18)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_MSK (0x00FC0000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq0_HSH (0x86A40098)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_OFF (24)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_MSK (0x3F000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_TxDqDCCOffsetR1dq1_HSH (0x86B00098)

  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_OFF  (30)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_WID  ( 2)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_MSK  (0xC0000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_MIN  (0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_MAX  (3) // 0x00000003
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_DEF  (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_rsvd_HSH  (0x023C0098)

#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000009C)
#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_DEF                      (0x00000000)
#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_VOLATILE_BITFIELDS_MSK   (0x3FFFFFFF)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_OFF ( 0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_MSK (0x0000003F)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq2_HSH (0x8680009C)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_OFF ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_MSK (0x00000FC0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq3_HSH (0x868C009C)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_OFF (12)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_MSK (0x0003F000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq4_HSH (0x8698009C)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_OFF (18)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_MSK (0x00FC0000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq5_HSH (0x86A4009C)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_OFF (24)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_MSK (0x3F000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_TxDqDCCOffsetR1dq6_HSH (0x86B0009C)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_OFF               (30)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_WID               ( 2)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_MSK               (0xC0000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_MIN               (0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_MAX               (3) // 0x00000003
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_rsvd_HSH               (0x023C009C)

#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x000000A0)
#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_DEF               (0x00000000)
#define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_VOLATILE_BITFIELDS_MSK (0xFFFFFFFF)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_OFF ( 0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_MSK (0x0000003F)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dq7_HSH (0x868000A0)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_OFF ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_WID ( 7)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_MSK (0x00001FC0)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_MIN (-64)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_MAX (63) // 0x0000003F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dqs_HSH (0x878C00A0)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_OFF (13)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_WID ( 7)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_MSK (0x000FE000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_MIN (-64)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_MAX (63) // 0x0000003F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dqs_HSH (0x879A00A0)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_OFF (20)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_MSK (0x03F00000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR0dbi_HSH (0x86A800A0)

  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_OFF (26)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_WID ( 6)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_MSK (0xFC000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_MIN (-32)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_MAX (31) // 0x0000001F
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_DEF (0x00000000)
  #define DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_TxDqDCCOffsetR1dbi_HSH (0x86B400A0)

#define DATA0CH0_CR_SPARE1_REG                             (0x000000A4)
#define DATA0CH0_CR_SPARE1_DEF                             (0x00000000)
#define DATA0CH0_CR_SPARE1_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DATA0CH0_CR_SPARE1_spare_OFF                     (0)
  #define DATA0CH0_CR_SPARE1_spare_WID                     (32)
  #define DATA0CH0_CR_SPARE1_spare_MSK                     (0xFFFFFFFF)
  #define DATA0CH0_CR_SPARE1_spare_MIN                     (0)
  #define DATA0CH0_CR_SPARE1_spare_MAX                     (4294967295) // 0xFFFFFFFF
  #define DATA0CH0_CR_SPARE1_spare_DEF                     (0x00000000)
  #define DATA0CH0_CR_SPARE1_spare_HSH                     (0x200000A4)

#define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x000000B4)
#define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_DEF                             (0x00000000)
#define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_VOLATILE_BITFIELDS_MSK          (0x00200000)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_OFF          ( 0)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_WID          ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_MSK          (0x0000001F)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_MIN          (-16)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_MAX          (15) // 0x0000000F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_DEF          (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPiOffset_HSH          (0x850000B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_OFF        ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_WID        ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_MSK        (0x000003E0)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_MIN        (-16)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_MAX        (15) // 0x0000000F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_DEF        (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxPhEnOffset_HSH        (0x850A00B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_OFF        (10)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_WID        ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_MSK        (0x00007C00)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_MIN        (-16)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_MAX        (15) // 0x0000000F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_DEF        (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxLockOffset_HSH        (0x851400B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_OFF          (15)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_WID          ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_MSK          (0x000F8000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_MIN          (-16)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_MAX          (15) // 0x0000000F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_DEF          (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_FFCodeRxWLOffset_HSH          (0x851E00B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_OFF (20)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_WID ( 1)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_MSK (0x00100000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_MIN (0)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_MAX (1) // 0x00000001
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_DEF (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_en_HSH (0x012800B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_OFF (21)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_WID ( 1)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_MSK (0x00200000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_MIN (0)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_MAX (1) // 0x00000001
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_DEF (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rxdll_ffcode_compupdt_ovrd_val_HSH (0x01AA00B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_OFF            (22)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_WID            ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_MSK            (0x07C00000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_MIN            (-16)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_MAX            (15) // 0x0000000F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_DEF            (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_RloadDqsOffset_HSH            (0x852C00B4)

  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_OFF                      (27)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_WID                      ( 5)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_MSK                      (0xF8000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_MIN                      (0)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_MAX                      (31) // 0x0000001F
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_DEF                      (0x00000000)
  #define DATA0CH0_CR_VCCCLKCOMPRXOFFSET_rsvd_HSH                      (0x053600B4)

#define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x000000CC)
#define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_DEF                        (0x00000000)
#define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_VOLATILE_BITFIELDS_MSK     (0x03FFFFFF)

  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_OFF           ( 0)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_WID           (10)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_MSK           (0x000003FF)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_MIN           (0)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_MAX           (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_InitPiCode_HSH           (0x0A8000CC)

  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_OFF              (10)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_WID              (16)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_MSK              (0x03FFFC00)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_MIN              (0)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_MAX              (65535) // 0x0000FFFF
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_ROCount_HSH              (0x109400CC)

  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_OFF                (26)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_WID                ( 6)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_MSK                (0xFC000000)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_MIN                (0)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_MAX                (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAININITRANK1_Spare_HSH                (0x063400CC)

#define DATA0CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x000000D0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x000000DC)
#define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_DEF                        (0x00000000)
#define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_VOLATILE_BITFIELDS_MSK     (0x07FFFFFF)

  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_OFF           ( 0)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_WID           (11)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_MSK           (0x000007FF)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_MIN           (0)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_MAX           (2047) // 0x000007FF
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_InitPiCode_HSH           (0x0B8000DC)

  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_OFF              (11)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_WID              (16)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_MSK              (0x07FFF800)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_MIN              (0)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_MAX              (65535) // 0x0000FFFF
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_ROCount_HSH              (0x109600DC)

  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_OFF                (27)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_WID                ( 5)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_MSK                (0xF8000000)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_MIN                (0)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_MAX                (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAININITRANK1_Spare_HSH                (0x053600DC)

#define DATA0CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x000000E0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA0CH0_CR_DDRCRRXCTLE_REG                                    (0x000000E4)
#define DATA0CH0_CR_DDRCRRXCTLE_DEF                                    (0x5555558A)
#define DATA0CH0_CR_DDRCRRXCTLE_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_OFF                           ( 0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_WID                           ( 1)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_MSK                           (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_MIN                           (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_MAX                           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_DEF                           (0x00000000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEn_HSH                           (0x010000E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_OFF                          ( 1)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_WID                          ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_MSK                          (0x00000006)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_MIN                          (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_MAX                          (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_DEF                          (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleCap_HSH                          (0x020200E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_OFF                          ( 3)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_WID                          ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_MSK                          (0x00000018)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_MIN                          (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_MAX                          (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_DEF                          (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRes_HSH                          (0x020600E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_OFF                           ( 5)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_WID                           ( 3)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_MSK                           (0x000000E0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_MIN                           (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_MAX                           (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_DEF                           (0x00000004)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleEq_HSH                           (0x030A00E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_OFF                         ( 8)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_WID                         ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_MSK                         (0x00000300)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_MIN                         (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_MAX                         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_DEF                         (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleRcmn_HSH                         (0x021000E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_OFF                       (10)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_WID                       ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_MSK                       (0x00000C00)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_MIN                       (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_MAX                       (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_DEF                       (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcLoad_HSH                       (0x021400E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_OFF                        (12)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_WID                        ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_MSK                        (0x00003000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_MIN                        (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_DEF                        (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleDcCmn_HSH                        (0x021800E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_OFF                  (14)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_MSK                  (0x0000C000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq0_HSH                  (0x021C00E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_OFF                  (16)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_MSK                  (0x00030000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq1_HSH                  (0x022000E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_OFF                  (18)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_MSK                  (0x000C0000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq2_HSH                  (0x022400E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_OFF                  (20)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_MSK                  (0x00300000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq3_HSH                  (0x022800E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_OFF                  (22)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_MSK                  (0x00C00000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq4_HSH                  (0x022C00E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_OFF                  (24)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_MSK                  (0x03000000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq5_HSH                  (0x023000E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_OFF                  (26)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_MSK                  (0x0C000000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq6_HSH                  (0x023400E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_OFF                  (28)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_MSK                  (0x30000000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dq7_HSH                  (0x023800E4)

  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_OFF                  (30)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_MIN                  (0)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRCRRXCTLE_CtleTailCtl_dbi_HSH                  (0x023C00E4)

#define DATA0CH0_CR_RXCONTROL0RANK0_REG                                (0x000000E8)
#define DATA0CH0_CR_RXCONTROL0RANK0_DEF                                (0x000007FF)
#define DATA0CH0_CR_RXCONTROL0RANK0_VOLATILE_BITFIELDS_MSK             (0x00000000)

  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_OFF                    ( 0)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_WID                    (11)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_MSK                    (0x000007FF)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_MIN                    (0)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_MAX                    (2047) // 0x000007FF
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_DEF                    (0x000007FF)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxRcvEnPi_HSH                    (0x0B0000E8)

  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_OFF                (11)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_WID                ( 7)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_MSK                (0x0003F800)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_MIN                (0)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_MAX                (127) // 0x0000007F
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_DEF                (0x00000000)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsPDelayPi_HSH                (0x071600E8)

  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_OFF                (18)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_WID                ( 7)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_MSK                (0x01FC0000)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_MIN                (0)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_MAX                (127) // 0x0000007F
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_DEF                (0x00000000)
  #define DATA0CH0_CR_RXCONTROL0RANK0_RxDqsNDelayPi_HSH                (0x072400E8)

  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_OFF                        (25)
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_WID                        ( 7)
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_MSK                        (0xFE000000)
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_MIN                        (0)
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_MAX                        (127) // 0x0000007F
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_DEF                        (0x00000000)
  #define DATA0CH0_CR_RXCONTROL0RANK0_Spare_HSH                        (0x073200E8)

#define DATA0CH0_CR_RXCONTROL0RANK1_REG                                (0x000000EC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA0CH0_CR_DDRDQREPLICACTLE_REG                               (0x000000F0)
#define DATA0CH0_CR_DDRDQREPLICACTLE_DEF                               (0x0002AAC5)
#define DATA0CH0_CR_DDRDQREPLICACTLE_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_OFF                     ( 0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_WID                     ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_MSK                     (0x00000003)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_MIN                     (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_MAX                     (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_DEF                     (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLECap_HSH                     (0x020000F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_OFF                     ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_WID                     ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_MSK                     (0x0000000C)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_MIN                     (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_MAX                     (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_DEF                     (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERes_HSH                     (0x020400F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_OFF                      ( 4)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_WID                      ( 3)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_MSK                      (0x00000070)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_MIN                      (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_MAX                      (7) // 0x00000007
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_DEF                      (0x00000004)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEEq_HSH                      (0x030800F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_OFF                    ( 7)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_WID                    ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_MSK                    (0x00000180)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_MIN                    (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_MAX                    (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_DEF                    (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLERcmn_HSH                    (0x020E00F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_OFF                  ( 9)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_WID                  ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_MSK                  (0x00000600)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_MIN                  (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_DEF                  (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcLoad_HSH                  (0x021200F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_OFF                   (11)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_WID                   ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_MSK                   (0x00001800)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_MIN                   (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_MAX                   (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_DEF                   (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEDcCmn_HSH                   (0x021600F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_OFF                 (13)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_WID                 ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_MSK                 (0x00006000)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_MIN                 (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_MAX                 (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_DEF                 (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLETailCtl_HSH                 (0x021A00F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_OFF                (15)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_WID                ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_MSK                (0x00018000)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_MIN                (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_MAX                (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_DEF                (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLENBiasCtl_HSH                (0x021E00F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_OFF             (17)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_WID             ( 2)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_MSK             (0x00060000)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_MIN             (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_MAX             (3) // 0x00000003
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_DEF             (0x00000001)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_CTLEPBiasCasCtl_HSH             (0x022200F0)

  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_OFF                        (19)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_WID                        (13)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_MSK                        (0xFFF80000)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_MIN                        (0)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_MAX                        (8191) // 0x00001FFF
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_DEF                        (0x00000000)
  #define DATA0CH0_CR_DDRDQREPLICACTLE_Rsvd_HSH                        (0x0D2600F0)

#define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x000000F8)
#define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DEF                            (0x00400000)
#define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_VOLATILE_BITFIELDS_MSK         (0x00015000)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_OFF          ( 0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_WID          ( 6)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_MSK          (0x0000003F)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_MIN          (-32)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_MAX          (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqDrvCompOffset_HSH          (0x860000F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_OFF          ( 6)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_WID          ( 6)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_MSK          (0x00000FC0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_MIN          (-32)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_MAX          (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_DqOdtCompOffset_HSH          (0x860C00F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_OFF (12)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_MSK (0x00001000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_val_HSH (0x019800F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_OFF (13)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_MSK (0x00002000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dq_rcompcode_compupdt_ovrd_en_HSH (0x011A00F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_OFF (14)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_MSK (0x00004000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_val_HSH (0x019C00F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_OFF (15)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_MSK (0x00008000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Dqs_rcompcode_compupdt_ovrd_en_HSH (0x011E00F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_OFF (16)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_MSK (0x00010000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_val_HSH (0x01A000F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_OFF (17)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_MSK (0x00020000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_MIN (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Wck_rcompcode_compupdt_ovrd_en_HSH (0x012200F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_OFF          (18)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_WID          ( 4)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_MSK          (0x003C0000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_MIN          (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_MAX          (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqstx_eqdlycomp_HSH          (0x042400F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_OFF         (22)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_WID         ( 6)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_MSK         (0x0FC00000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_MIN         (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_MAX         (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_DEF         (0x00000001)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_dqtx_eqdelaycomp_HSH         (0x062C00F8)

  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_OFF                     (28)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_WID                     ( 4)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_MSK                     (0xF0000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_MAX                     (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_DEF                     (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAOFFSETCOMP_Rsvd_HSH                     (0x043800F8)

#define DATA0CH0_CR_DDRCRDATACONTROL0_REG                              (0x000000FC)
#define DATA0CH0_CR_DDRCRDATACONTROL0_DEF                              (0x58008002)
#define DATA0CH0_CR_DDRCRDATACONTROL0_VOLATILE_BITFIELDS_MSK           (0x01000000)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_OFF ( 0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_MSK (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_MIN (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_tx_clock_on_with_txanalogen_HSH (0x010000FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_OFF            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_MSK            (0x00000002)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_DEF            (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_local_gate_d0tx_HSH            (0x010200FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_OFF           ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_WID           ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_MSK           (0x00000004)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_InternalClocksOn_HSH           (0x010400FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_OFF                     ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_WID                     ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_MSK                     (0x00000008)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_DEF                     (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxPiOn_HSH                     (0x010600FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_OFF                       ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_WID                       ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_MSK                       (0x00000010)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_MIN                       (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_MAX                       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_DEF                       (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxOn_HSH                       (0x010800FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_OFF                  ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_MSK                  (0x00000020)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_TxDisable_HSH                  (0x010A00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_OFF            ( 6)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_MSK            (0x00000040)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqOdtParkMode_HSH            (0x010C00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_OFF              ( 7)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_WID              ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_MSK              (0x00000080)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDQODTParkHi_HSH              (0x010E00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_OFF           ( 8)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_WID           ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_MSK           (0x00000300)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_EnDqsOdtParkMode_HSH           (0x021000FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_OFF                 (10)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_WID                 ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_MSK                 (0x00000C00)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_MAX                 (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_BiasPMCtrl_HSH                 (0x021400FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_OFF                 (12)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_WID                 ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_MSK                 (0x00001000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_MAX                 (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_VrefPMCtrl_HSH                 (0x011800FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_OFF                  (13)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_MSK                  (0x00002000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxDisable_HSH                  (0x011A00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_OFF                  (14)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_MSK                  (0x00004000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_ForceRxOn_HSH                  (0x011C00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_OFF                     (15)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_WID                     ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_MSK                     (0x00008000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_DEF                     (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxMode_HSH                     (0x011E00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_OFF             (16)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_MSK             (0x00010000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DqsNParkLowVoh_HSH             (0x012000FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_OFF             (17)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_MSK             (0x00020000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtForceQDrvEn_HSH             (0x012200FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_OFF                 (18)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_WID                 ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_MSK                 (0x00040000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_MAX                 (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtSampOff_HSH                 (0x012400FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_OFF           (19)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_WID           ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_MSK           (0x00080000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisableOdtStatic_HSH           (0x012600FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_OFF            (20)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_MSK            (0x00100000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DdrCRForceODTOn_HSH            (0x012800FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_OFF                    (21)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_WID                    ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_MSK                    (0x00600000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_MIN                    (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_MAX                    (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_DEF                    (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_OdtMode_HSH                    (0x022A00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_OFF            (23)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_MSK            (0x00800000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RxPwrMuxSelVdd2_HSH            (0x012E00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_OFF                (24)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_WID                ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_MSK                (0x01000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_MAX                (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_RunRefPIFSM_HSH                (0x01B000FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_OFF                   (25)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_WID                   ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_MSK                   (0x02000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_PreDrvOn_HSH                   (0x013200FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_OFF     (26)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_WID     ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_MSK     (0x04000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_MIN     (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_MAX     (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_DEF     (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_DisRxSalOffWhenDrvEnHi_HSH     (0x013400FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_OFF            (27)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_MSK            (0x08000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_DEF            (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dficmddecode_en_HSH            (0x013600FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_OFF (28)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_WID ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_MSK (0x10000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_MIN (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_DEF (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_gvblock_blockretrainload_dis_HSH (0x013800FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_OFF            (29)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_WID            ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_MSK            (0x20000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_MIN            (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqidlemodedrven_HSH            (0x013A00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_OFF             (30)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_MSK             (0x40000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_DEF             (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqdrvstatlegen_HSH             (0x013C00FC)

  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_OFF           (31)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_WID           ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_MSK           (0x80000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL0_dqsidlemodedrven_HSH           (0x013E00FC)

#define DATA0CH0_CR_DDRCRDATACONTROL1_REG                              (0x00000100)
#define DATA0CH0_CR_DDRCRDATACONTROL1_DEF                              (0x00000002)
#define DATA0CH0_CR_DDRCRDATACONTROL1_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_OFF               ( 0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_WID               ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_MSK               (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DisableTxDqs_HSH               (0x01000100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_OFF              ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_WID              ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_MSK              (0x00000002)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_DEF              (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_TxDqsNmosOnly_HSH              (0x01020100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_OFF       ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_WID       ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_MSK       (0x00000004)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_MIN       (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_MAX       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_dqssecterfinalmuxsel_HSH       (0x01040100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_OFF                  ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_MSK                  (0x00000018)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIenable_HSH                  (0x02060100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_OFF                    ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_WID                    ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_MSK                    (0x00000020)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_MIN                    (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_MAX                    (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_DEF                    (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DBIMode_HSH                    (0x010A0100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_OFF                ( 6)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_WID                ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_MSK                (0x000007C0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_MAX                (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDelay_HSH                (0x050C0100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_OFF             (11)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_WID             ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_MSK             (0x00003800)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_MAX             (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqsOdtDuration_HSH             (0x03160100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_OFF                 (14)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_WID                 ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_MSK                 (0x0007C000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_MAX                 (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDelay_HSH                 (0x051C0100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_OFF              (19)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_WID              ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_MSK              (0x00780000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_DqOdtDuration_HSH              (0x04260100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_OFF              (23)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_WID              ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_MSK              (0x0F800000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_MAX              (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDelay_HSH              (0x052E0100)

  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_OFF           (28)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_WID           ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_MSK           (0xF0000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_MAX           (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL1_SenseAmpDuration_HSH           (0x04380100)

#define DATA0CH0_CR_DDRCRDATACONTROL5_REG                              (0x00000104)
#define DATA0CH0_CR_DDRCRDATACONTROL5_DEF                              (0x80000810)
#define DATA0CH0_CR_DDRCRDATACONTROL5_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_OFF                    ( 0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_WID                    ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_MSK                    (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_MIN                    (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_MAX                    (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_DEF                    (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxXtalk_HSH                    (0x01000104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_OFF           ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_WID           ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_MSK           (0x00000006)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DataInvertNibble_HSH           (0x02020104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_OFF                     ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_WID                     ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_MSK                     (0x00000008)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_DEF                     (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxPiOn_HSH                     (0x01060104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_OFF               ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_WID               ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_MSK               (0x00000010)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_DEF               (0x00000001)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_TxDqNmosOnly_HSH               (0x01080104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_OFF              ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_WID              ( 3)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_MSK              (0x000000E0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_MAX              (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasRloadVref_HSH              (0x030A0104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_OFF                ( 8)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_WID                ( 4)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_MSK                (0x00000F00)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_MAX                (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_DEF                (0x00000008)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_BiasIrefAdj_HSH                (0x04100104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_OFF                (12)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_WID                ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_MSK                (0x00003000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_MAX                (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXVcmResSel_HSH                (0x02180104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_OFF               (14)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_WID               ( 2)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_MSK               (0x0000C000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_MAX               (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RXDqsTailCtl_HSH               (0x021C0104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_OFF             (16)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_WID             ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_MSK             (0x00010000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_ODTSingleSegEn_HSH             (0x01200104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_OFF           (17)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_WID           ( 5)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_MSK           (0x003E0000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_MIN           (-16)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_MAX           (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_DqsOdtCompOffset_HSH           (0x85220104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_OFF          (22)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_WID          ( 1)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_MSK          (0x00400000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_MIN          (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_MAX          (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_VrefPwrMuxSelVdd2_HSH          (0x012C0104)

  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_OFF                     (23)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_WID                     ( 9)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_MSK                     (0xFF800000)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_MIN                     (0)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_MAX                     (511) // 0x000001FF
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_DEF                     (0x00000100)
  #define DATA0CH0_CR_DDRCRDATACONTROL5_RxVref_HSH                     (0x092E0104)

#define DATA0CH0_CR_DDRCRRXEQTAP01_REG                                 (0x00000108)
#define DATA0CH0_CR_DDRCRRXEQTAP01_DEF                                 (0x00000000)
#define DATA0CH0_CR_DDRCRRXEQTAP01_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_OFF                 ( 0)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_WID                 ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_MSK                 (0x0000003F)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_MIN                 (-32)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_MAX                 (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap0_HSH                 (0x86000108)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_OFF                 ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_WID                 ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_MSK                 (0x00000FC0)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_MIN                 (-32)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_MAX                 (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap1_HSH                 (0x860C0108)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_OFF                 (12)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_WID                 ( 5)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_MSK                 (0x0001F000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_MIN                 (-16)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_MAX                 (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap2_HSH                 (0x85180108)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_OFF                 (17)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_WID                 ( 5)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_MSK                 (0x003E0000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_MIN                 (-16)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_MAX                 (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_RxEqRank0Tap3_HSH                 (0x85220108)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_OFF              (22)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_WID              ( 2)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_MSK              (0x00C00000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_MIN              (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_MAX              (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_dqpadmux_selvddq_HSH              (0x022C0108)

  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_OFF                          (24)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_WID                          ( 8)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_MSK                          (0xFF000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_MIN                          (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_MAX                          (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP01_Rsvd_HSH                          (0x08300108)

#define DATA0CH0_CR_DDRCRRXEQTAP_REG                                   (0x0000010C)
#define DATA0CH0_CR_DDRCRRXEQTAP_DEF                                   (0x00000000)
#define DATA0CH0_CR_DDRCRRXEQTAP_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_OFF                   ( 0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_WID                   ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_MSK                   (0x0000003F)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_MIN                   (-32)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_MAX                   (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap0_HSH                   (0x8600010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_OFF                   ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_WID                   ( 6)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_MSK                   (0x00000FC0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_MIN                   (-32)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_MAX                   (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap1_HSH                   (0x860C010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_OFF                   (12)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_WID                   ( 5)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_MSK                   (0x0001F000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_MIN                   (-16)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_MAX                   (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap2_HSH                   (0x8518010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_OFF                   (17)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_WID                   ( 5)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_MSK                   (0x003E0000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_MIN                   (-16)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_MAX                   (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_RxEqRank1Tap3_HSH                   (0x8522010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_OFF                            (22)
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_WID                            ( 2)
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_MSK                            (0x00C00000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_MIN                            (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_MAX                            (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_DEF                            (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_Rsvd_HSH                            (0x022C010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_OFF                        (24)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_WID                        ( 2)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_MSK                        (0x03000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_MIN                        (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_DEF                        (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqCap_HSH                        (0x0230010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_OFF                        (26)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_WID                        ( 2)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_MSK                        (0x0C000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_MIN                        (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_DEF                        (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqRes_HSH                        (0x0234010C)

  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_OFF                       (28)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_WID                       ( 4)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_MSK                       (0xF0000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_MIN                       (0)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_MAX                       (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_DEF                       (0x00000000)
  #define DATA0CH0_CR_DDRCRRXEQTAP_DqsEqLegs_HSH                       (0x0438010C)

#define DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000110)
#define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DEF                               (0x00000000)
#define DATA0CH0_CR_DDRCRCMDBUSTRAIN_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_OFF             ( 0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_WID             (11)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_MSK             (0x000007FF)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_MIN             (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_MAX             (2047) // 0x000007FF
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdModeEn_HSH             (0x0B000110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_OFF               (11)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_WID               (11)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_MSK               (0x003FF800)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_MIN               (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_MAX               (2047) // 0x000007FF
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrDqOvrdData_HSH               (0x0B160110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_OFF            (22)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_WID            ( 2)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_MSK            (0x00C00000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_MIN            (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_MAX            (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRNumOfPulses_HSH            (0x022C0110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_OFF   (24)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_WID   ( 4)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_MSK   (0x0F000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_MIN   (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_MAX   (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_DEF   (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRMaskCntPulseNumStart_HSH   (0x04300110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_OFF            (28)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_WID            ( 1)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_MSK            (0x10000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_MIN            (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_DdrCRWrPostamble_HSH            (0x01380110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_OFF                       (29)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_WID                       ( 2)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_MSK                       (0x60000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_MIN                       (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_MAX                       (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_DEF                       (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_Spare_HSH                       (0x023A0110)

  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_OFF            (31)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_WID            ( 1)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_MSK            (0x80000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_MIN            (0)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_MAX            (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRCMDBUSTRAIN_ddrdqsovrdtoggle_HSH            (0x013E0110)

#define DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000114)
#define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DEF                           (0x00000000)
#define DATA0CH0_CR_DDRCRDATATCOCONTROL0_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_OFF           ( 0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_MSK           (0x0000003F)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit0_HSH           (0x86000114)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_OFF           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_MSK           (0x00000FC0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit1_HSH           (0x860C0114)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_OFF           (12)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_MSK           (0x0003F000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit2_HSH           (0x86180114)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_OFF           (18)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_MSK           (0x00FC0000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit3_HSH           (0x86240114)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_OFF           (24)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_MSK           (0x3F000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_DqTcoCompBit4_HSH           (0x86300114)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_OFF                   (30)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_WID                   ( 2)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_MSK                   (0xC0000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_MAX                   (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL0_Spare_HSH                   (0x023C0114)

#define DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000118)
#define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DEF                           (0x00000000)
#define DATA0CH0_CR_DDRCRDATATCOCONTROL1_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_OFF           ( 0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_MSK           (0x0000003F)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit5_HSH           (0x86000118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_OFF           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_MSK           (0x00000FC0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit6_HSH           (0x860C0118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_OFF           (12)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_WID           ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_MSK           (0x0003F000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_MIN           (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_MAX           (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DqTcoCompBit7_HSH           (0x86180118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_OFF              (18)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_WID              ( 6)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_MSK              (0x00FC0000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_MIN              (-32)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_MAX              (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_DMITcoComp_HSH              (0x86240118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_OFF              (24)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_WID              ( 4)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_MSK              (0x0F000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_MIN              (0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_MAX              (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_IOLBCycles_HSH              (0x04300118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_OFF           (28)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_WID           ( 2)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_MSK           (0x30000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_dqmmskipcount_HSH           (0x02380118)

  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_OFF                   (30)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_WID                   ( 2)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_MSK                   (0xC0000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_MIN                   (0)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_MAX                   (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_DEF                   (0x00000000)
  #define DATA0CH0_CR_DDRCRDATATCOCONTROL1_Spare_HSH                   (0x023C0118)

#define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000011C)
#define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_DEF                    (0x00000000)
#define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_VOLATILE_BITFIELDS_MSK (0xFFFE0800)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_OFF        ( 0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_WID        ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_MSK        (0x00000001)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_MIN        (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_MAX        (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_InitTrain_HSH        (0x0100011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_OFF         ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_WID         (10)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_MSK         (0x000007FE)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_MIN         (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_MAX         (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_Duration_HSH         (0x0A02011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_OFF      (11)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_WID      ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_MSK      (0x00000800)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_MIN      (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_MAX      (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_DEF      (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_ResetStatus_HSH      (0x0196011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_OFF (12)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_MSK (0x00007000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeDelta_HSH (0x0318011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_OFF (15)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_WID ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_MSK (0x00008000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_updonlargechange_HSH (0x011E011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_OFF (16)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_WID ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_MSK (0x00010000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_StopOnLargeChange_HSH (0x0120011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_OFF         (17)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_WID         ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_MSK         (0x00060000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_MIN         (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R0Status_HSH         (0x02A2011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_OFF         (19)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_WID         ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_MSK         (0x00180000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_MIN         (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R1Status_HSH         (0x02A6011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_OFF         (21)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_WID         ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_MSK         (0x00600000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_MIN         (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R2Status_HSH         (0x02AA011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_OFF         (23)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_WID         ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_MSK         (0x01800000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_MIN         (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_R3Status_HSH         (0x02AE011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_OFF        (25)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_WID        ( 4)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_MSK        (0x1E000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_MIN        (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_MAX        (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMStatus_HSH        (0x04B2011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_OFF          (29)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_WID          ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_MSK          (0x60000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_MIN          (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_MAX          (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_FSMRank_HSH          (0x02BA011C)

  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_OFF  (31)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_WID  ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_MSK  (0x80000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_MIN  (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_MAX  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_DEF  (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_LargeChangeTrig_HSH  (0x01BE011C)

#define DATA0CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000120)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000124)
#define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DEF                   (0x00FAC688)
#define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_OFF ( 0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_MSK (0x00000007)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ0_HSH (0x03000124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_OFF ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_MSK (0x00000038)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_DEF (0x00000001)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ1_HSH (0x03060124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_OFF ( 6)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_MSK (0x000001C0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_DEF (0x00000002)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ2_HSH (0x030C0124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_OFF ( 9)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_MSK (0x00000E00)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_DEF (0x00000003)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ3_HSH (0x03120124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_OFF (12)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_MSK (0x00007000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_DEF (0x00000004)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ4_HSH (0x03180124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_OFF (15)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_MSK (0x00038000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_DEF (0x00000005)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ5_HSH (0x031E0124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_OFF (18)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_MSK (0x001C0000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_DEF (0x00000006)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ6_HSH (0x03240124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_OFF (21)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_WID ( 3)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_MSK (0x00E00000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_MAX (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_DEF (0x00000007)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_DQ7_HSH (0x032A0124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_OFF (24)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_WID ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_MSK (0x01000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_MAX (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrain_ByteSel_HSH (0x01300124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_OFF   (25)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_WID   ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_MSK   (0x06000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_MIN   (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_MAX   (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_DEF   (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataRetrainEn_HSH   (0x02320124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_OFF  (27)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_WID  ( 1)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_MSK  (0x08000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_MIN  (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_MAX  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_DEF  (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_SerialMRROrder_HSH  (0x01360124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_OFF (28)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_WID ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_MSK (0x30000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_MIN (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_MAX (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_DEF (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_DataInvertNibble_HSH (0x02380124)

  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_OFF           (30)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_WID           ( 2)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_MSK           (0xC0000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_MIN           (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_MAX           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_Spare_HSH           (0x023C0124)

#define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000128)
#define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DEF                        (0x00000000)
#define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_VOLATILE_BITFIELDS_MSK     (0x0FFFFFFF)

  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_OFF        ( 0)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_WID        ( 7)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_MSK        (0x0000007F)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_MIN        (-64)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_MAX        (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR0_HSH        (0x87800128)

  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_OFF        ( 7)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_WID        ( 7)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_MSK        (0x00003F80)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_MIN        (-64)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_MAX        (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR1_HSH        (0x878E0128)

  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_OFF        (14)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_WID        ( 7)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_MSK        (0x001FC000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_MIN        (-64)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_MAX        (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR2_HSH        (0x879C0128)

  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_OFF        (21)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_WID        ( 7)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_MSK        (0x0FE00000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_MIN        (-64)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_MAX        (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_DeltaPiCodeR3_HSH        (0x87AA0128)

  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_OFF                (28)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_WID                ( 4)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_MSK                (0xF0000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_MIN                (0)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_MAX                (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_Spare_HSH                (0x04380128)

#define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x0000012C)
#define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DEF                        (0x00000000)
#define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_VOLATILE_BITFIELDS_MSK     (0xFFFFFFFF)

  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_OFF        ( 0)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_WID        ( 8)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_MSK        (0x000000FF)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_MIN        (-128)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_MAX        (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR0_HSH        (0x8880012C)

  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_OFF        ( 8)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_WID        ( 8)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_MSK        (0x0000FF00)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_MIN        (-128)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_MAX        (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR1_HSH        (0x8890012C)

  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_OFF        (16)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_WID        ( 8)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_MSK        (0x00FF0000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_MIN        (-128)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_MAX        (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR2_HSH        (0x88A0012C)

  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_OFF        (24)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_WID        ( 8)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_MSK        (0xFF000000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_MIN        (-128)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_MAX        (127) // 0x0000007F
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_DeltaPiCodeR3_HSH        (0x88B0012C)

#define DATA0CH0_CR_DATATRAINFEEDBACK_REG                              (0x00000130)
#define DATA0CH0_CR_DATATRAINFEEDBACK_DEF                              (0x00000000)
#define DATA0CH0_CR_DATATRAINFEEDBACK_VOLATILE_BITFIELDS_MSK           (0xFFC00000)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_OFF                   ( 0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_WID                   ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_MSK                   (0x00000001)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_MIN                   (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_MAX                   (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_DEF                   (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankOvrd_HSH                   (0x01000130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_OFF                  ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_WID                  ( 2)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_MSK                  (0x00000006)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_MIN                  (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_DEF                  (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RankValue_HSH                  (0x02020130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_OFF              ( 3)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_WID              ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_MSK              (0x00000008)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_MIN              (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_DEF              (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxAmpOffsetEn_HSH              (0x01060130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_OFF             ( 4)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_WID             ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_MSK             (0x00000010)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_MIN             (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_DEF             (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RxTrainingMode_HSH             (0x01080130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_OFF             ( 5)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_WID             ( 2)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_MSK             (0x00000060)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_MIN             (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_MAX             (3) // 0x00000003
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_DEF             (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_WLTrainingMode_HSH             (0x020A0130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_OFF             ( 7)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_WID             ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_MSK             (0x00000080)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_MIN             (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_DEF             (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_RLTrainingMode_HSH             (0x010E0130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_OFF       ( 8)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_WID       ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_MSK       (0x00000100)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_MIN       (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_MAX       (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_DEF       (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_SenseampTrainingMode_HSH       (0x01100130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_OFF             ( 9)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_WID             ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_MSK             (0x00000200)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_MIN             (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_DEF             (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CATrainingMode_HSH             (0x01120130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_OFF                (10)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_WID                ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_MSK                (0x00000400)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_MIN                (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_MAX                (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_DEF                (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_CAParityOdd_HSH                (0x01140130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_OFF              (11)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_WID              ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_MSK              (0x00000800)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_MIN              (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_DEF              (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_caparitytrain_HSH              (0x01160130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_OFF                     (12)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_WID                     ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_MSK                     (0x00001000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_MIN                     (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_DEF                     (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_LPDDR5_HSH                     (0x01180130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_OFF            (13)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_WID            ( 4)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_MSK            (0x0001E000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_MIN            (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_MAX            (15) // 0x0000000F
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_DEF            (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_R2RTrainingMode_HSH            (0x041A0130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_OFF          (17)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_WID          ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_MSK          (0x00020000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_MIN          (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_MAX          (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_DEF          (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DqsRFTrainingMode_HSH          (0x01220130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_OFF                 (18)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_WID                 ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_MSK                 (0x00040000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_MIN                 (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_MAX                 (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_DEF                 (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_En16UIMask_HSH                 (0x01240130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_OFF        (19)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_WID        ( 1)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_MSK        (0x00080000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_MIN        (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_MAX        (1) // 0x00000001
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_DEF        (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Ddr5_CSTrainingMode_HSH        (0x01260130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_OFF                      (20)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_WID                      ( 2)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_MSK                      (0x00300000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_MIN                      (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_MAX                      (3) // 0x00000003
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_DEF                      (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_Spare_HSH                      (0x02280130)

  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_OFF          (22)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_WID          (10)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_MSK          (0xFFC00000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_MIN          (0)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_MAX          (1023) // 0x000003FF
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_DEF          (0x00000000)
  #define DATA0CH0_CR_DATATRAINFEEDBACK_DataTrainFeedback_HSH          (0x0AAC0130)

#define DATA0CH0_CR_DCCFSMSTATUS_REG                                   (0x00000134)
#define DATA0CH0_CR_DCCFSMSTATUS_DEF                                   (0x00010000)
#define DATA0CH0_CR_DCCFSMSTATUS_VOLATILE_BITFIELDS_MSK                (0xFFFFFFFF)

  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_OFF                        ( 0)
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_WID                        (10)
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_MSK                        (0x000003FF)
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_MAX                        (1023) // 0x000003FF
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_DEF                        (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_LaneDone_HSH                        (0x0A800134)

  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_OFF                        (10)
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_WID                        ( 2)
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_MSK                        (0x00000C00)
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_DEF                        (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_RankDone_HSH                        (0x02940134)

  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_OFF                       (12)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_WID                       ( 3)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_MSK                       (0x00007000)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_MIN                       (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_MAX                       (7) // 0x00000007
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_DEF                       (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcState_HSH                       (0x03980134)

  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_OFF                        (15)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_WID                        ( 2)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_MSK                        (0x00018000)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_DEF                        (0x00000002)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcRank_HSH                        (0x029E0134)

  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_OFF                        (17)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_WID                        ( 4)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_MSK                        (0x001E0000)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_MAX                        (15) // 0x0000000F
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_DEF                        (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_CalcLane_HSH                        (0x04A20134)

  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_OFF                     (21)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_WID                     ( 1)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_MSK                     (0x00200000)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_MIN                     (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateReady_HSH                     (0x01AA0134)

  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_OFF                      (22)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_WID                      ( 1)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_MSK                      (0x00400000)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_MIN                      (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_DEF                      (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_UpdateDone_HSH                      (0x01AC0134)

  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_OFF                     (23)
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_WID                     ( 1)
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_MSK                     (0x00800000)
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_MIN                     (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_MAX                     (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_DEF                     (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_LargeChange_HSH                     (0x01AE0134)

  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_OFF                        (24)
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_WID                        ( 1)
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_MSK                        (0x01000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_MIN                        (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_MAX                        (1) // 0x00000001
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_DEF                        (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_DCCReady_HSH                        (0x01B00134)

  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_OFF                            (25)
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_WID                            ( 7)
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_MSK                            (0xFE000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_MIN                            (0)
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_MAX                            (127) // 0x0000007F
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_DEF                            (0x00000000)
  #define DATA0CH0_CR_DCCFSMSTATUS_rsvd_HSH                            (0x07B20134)

#define DATA0CH0_CR_DCCLANESTATUS0_REG                                 (0x00000138)
#define DATA0CH0_CR_DCCLANESTATUS0_DEF                                 (0x00000000)
#define DATA0CH0_CR_DCCLANESTATUS0_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_OFF                   ( 0)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_MSK                   (0x000001FF)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane0Result_HSH                   (0x09000138)

  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_OFF                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_MSK                   (0x0003FE00)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane1Result_HSH                   (0x09120138)

  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_OFF                   (18)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_MSK                   (0x07FC0000)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS0_Lane2Result_HSH                   (0x09240138)

  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_OFF                          (27)
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_WID                          ( 5)
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_MSK                          (0xF8000000)
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_MIN                          (0)
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_MAX                          (31) // 0x0000001F
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS0_rsvd_HSH                          (0x05360138)

#define DATA0CH0_CR_DCCLANESTATUS1_REG                                 (0x0000013C)
#define DATA0CH0_CR_DCCLANESTATUS1_DEF                                 (0x00000000)
#define DATA0CH0_CR_DCCLANESTATUS1_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_OFF                   ( 0)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_MSK                   (0x000001FF)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane3Result_HSH                   (0x0900013C)

  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_OFF                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_MSK                   (0x0003FE00)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane4Result_HSH                   (0x0912013C)

  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_OFF                   (18)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_MSK                   (0x07FC0000)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS1_Lane5Result_HSH                   (0x0924013C)

  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_OFF                          (27)
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_WID                          ( 5)
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_MSK                          (0xF8000000)
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_MIN                          (0)
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_MAX                          (31) // 0x0000001F
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS1_rsvd_HSH                          (0x0536013C)

#define DATA0CH0_CR_DCCLANESTATUS2_REG                                 (0x00000140)
#define DATA0CH0_CR_DCCLANESTATUS2_DEF                                 (0x00000000)
#define DATA0CH0_CR_DCCLANESTATUS2_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_OFF                   ( 0)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_MSK                   (0x000001FF)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane6Result_HSH                   (0x09000140)

  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_OFF                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_MSK                   (0x0003FE00)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane7Result_HSH                   (0x09120140)

  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_OFF                   (18)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_MSK                   (0x07FC0000)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS2_Lane8Result_HSH                   (0x09240140)

  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_OFF                          (27)
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_WID                          ( 5)
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_MSK                          (0xF8000000)
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_MIN                          (0)
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_MAX                          (31) // 0x0000001F
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS2_rsvd_HSH                          (0x05360140)

#define DATA0CH0_CR_DCCLANESTATUS3_REG                                 (0x00000144)
#define DATA0CH0_CR_DCCLANESTATUS3_DEF                                 (0x00000000)
#define DATA0CH0_CR_DCCLANESTATUS3_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_OFF                   ( 0)
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_WID                   ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_MSK                   (0x000001FF)
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_MIN                   (0)
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_MAX                   (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_DEF                   (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS3_Lane9Result_HSH                   (0x09000144)

  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_OFF                 ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_WID                 ( 7)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_MSK                 (0x0000FE00)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_MIN                 (0)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_MAX                 (127) // 0x0000007F
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_DEF                 (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeOffset_HSH                 (0x07120144)

  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_OFF                  (16)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_WID                  ( 9)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_MSK                  (0x01FF0000)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_MIN                  (0)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_MAX                  (511) // 0x000001FF
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_DEF                  (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS3_ExtremeCount_HSH                  (0x09200144)

  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_OFF                          (25)
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_WID                          ( 7)
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_MSK                          (0xFE000000)
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_MIN                          (0)
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_MAX                          (127) // 0x0000007F
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_DEF                          (0x00000000)
  #define DATA0CH0_CR_DCCLANESTATUS3_rsvd_HSH                          (0x07320144)

#define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000148)
#define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_DEF                        (0x00000000)
#define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_VOLATILE_BITFIELDS_MSK     (0x00000000)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_OFF              ( 0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_WID              ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_MSK              (0x00000001)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_MIN              (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_RunTest_HSH              (0x01000148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_OFF                ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_WID                ( 3)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_MSK                (0x0000000E)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_MIN                (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_MAX                (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_Param_HSH                (0x03020148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_OFF               ( 4)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_WID               ( 9)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_MSK               (0x00001FF0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_MIN               (-256)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_MAX               (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinVal_HSH               (0x89080148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_OFF               (13)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_WID               ( 9)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_MSK               (0x003FE000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_MIN               (-256)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_MAX               (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MaxVal_HSH               (0x891A0148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_OFF             (22)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_WID             ( 6)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_MSK             (0x0FC00000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_MIN             (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_MAX             (63) // 0x0000003F
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_MinWidth_HSH             (0x062C0148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_OFF           (28)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_WID           ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_MSK           (0x10000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_MIN           (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_CalcCenter_HSH           (0x01380148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_OFF       (29)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_WID       ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_MSK       (0x20000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_MIN       (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_MAX       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_BurstIdleBurst_HSH       (0x013A0148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_OFF           (30)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_WID           ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_MSK           (0x40000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_MIN           (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_LaneResult_HSH           (0x013C0148)

  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_OFF         (31)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_WID         ( 1)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_MSK         (0x80000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_MIN         (0)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_MAX         (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODECONTROL0_EnDQSDrvPark_HSH         (0x013E0148)

#define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x0000014C)
#define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_DEF                       (0x00000000)
#define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_OFF              ( 0)
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_WID              (32)
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_MSK              (0xFFFFFFFF)
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_MIN              (0)
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_MAX              (4294967295) // 0xFFFFFFFF
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_result_HSH              (0x2000014C)

#define DATA0CH0_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000150)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA0CH0_CR_DDRCRDLLSTATUS_REG                                 (0x00000154)
#define DATA0CH0_CR_DDRCRDLLSTATUS_DEF                                 (0x00000000)
#define DATA0CH0_CR_DDRCRDLLSTATUS_VOLATILE_BITFIELDS_MSK              (0x00FFFFFF)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_OFF                      ( 0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_WID                      ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_MSK                      (0x00000001)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_MIN                      (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_DEF                      (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReset_HSH                      (0x01800154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_OFF                      ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_WID                      ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_MSK                      (0x00000002)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_MIN                      (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_DEF                      (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DLLReady_HSH                      (0x01820154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_OFF              ( 2)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_WID              ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_MSK              (0x00000004)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_MIN              (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompCompleteOnce_HSH              (0x01840154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_OFF                       ( 3)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_WID                       ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_MSK                       (0x00000008)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_MIN                       (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_MAX                       (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_DEF                       (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Startup_HSH                       (0x01860154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_OFF                 ( 4)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_WID                 ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_MSK                 (0x00000010)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_MIN                 (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_MAX                 (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_DEF                 (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DetrmLockStat_HSH                 (0x01880154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_OFF                  ( 5)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_MSK                  (0x00000020)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_FuncLockStat_HSH                  (0x018A0154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_OFF                  ( 6)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_MSK                  (0x00000040)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_StickyUnlock_HSH                  (0x018C0154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_OFF                      ( 7)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_WID                      ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_MSK                      (0x00000080)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_MIN                      (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_DEF                      (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_LongLock_HSH                      (0x018E0154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_OFF                  ( 8)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_MSK                  (0x00000100)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLockDone_HSH                  (0x01900154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_OFF                  ( 9)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_WID                  ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_MSK                  (0x00000200)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_MAX                  (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_CompComplete_HSH                  (0x01920154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_OFF                      (10)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_WID                      ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_MSK                      (0x00000400)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_MIN                      (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_MAX                      (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_DEF                      (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_WeakLock_HSH                      (0x01940154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_OFF               (11)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_WID               ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_MSK               (0x00000800)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_MIN               (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooLow_HSH               (0x01960154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_OFF                (12)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_WID                ( 1)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_MSK                (0x00001000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_MIN                (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_MAX                (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_PDVctlWayTooHi_HSH                (0x01980154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_OFF                    (13)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_WID                    (11)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_MSK                    (0x00FFE000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_MIN                    (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_MAX                    (2047) // 0x000007FF
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_DEF                    (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_DllDacCode_HSH                    (0x0B9A0154)

  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_OFF                         (24)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_WID                         ( 8)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_MSK                         (0xFF000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_MIN                         (0)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_MAX                         (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_DEF                         (0x00000000)
  #define DATA0CH0_CR_DDRCRDLLSTATUS_Spare_HSH                         (0x08300154)

#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000158)
#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_DEF                     (0x00000000)
#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_VOLATILE_BITFIELDS_MSK  (0x00000001)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_OFF             ( 0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_WID             ( 1)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_MSK             (0x00000001)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_MIN             (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_EnCnt_HSH             (0x01800158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_OFF          ( 1)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_WID          ( 5)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_MSK          (0x0000003E)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_MIN          (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_MAX          (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Duration_HSH          (0x05020158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_OFF            ( 6)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_WID            ( 3)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_MSK            (0x000001C0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_MIN            (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_MAX            (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_DEF            (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_SelCnt_HSH            (0x030C0158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_OFF       ( 9)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_WID       ( 4)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_MSK       (0x00001E00)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_MIN       (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_MAX       (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi0_HSH       (0x04120158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_OFF       (13)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_WID       ( 3)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_MSK       (0x0000E000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_MIN       (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_MAX       (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi1_HSH       (0x031A0158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_OFF       (16)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_WID       ( 3)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_MSK       (0x00070000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_MIN       (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_MAX       (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPhPi2_HSH       (0x03200158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_OFF         (19)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_WID         ( 4)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_MSK         (0x00780000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_MIN         (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_MAX         (15) // 0x0000000F
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi0_HSH         (0x04260158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_OFF         (23)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_WID         ( 2)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_MSK         (0x01800000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_MIN         (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi1_HSH         (0x022E0158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_OFF         (25)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_WID         ( 2)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_MSK         (0x06000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_MIN         (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_MAX         (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_DEF         (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightPi2_HSH         (0x02320158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_OFF       (27)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_WID       ( 3)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_MSK       (0x38000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_MIN       (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_MAX       (7) // 0x00000007
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_DEF       (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_WeightRxDrv_HSH       (0x03360158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_OFF          (30)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_WID          ( 1)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_MSK          (0x40000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_MIN          (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_MAX          (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_DEF          (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_CntStart_HSH          (0x013C0158)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_OFF             (31)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_WID             ( 1)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_MSK             (0x80000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_MIN             (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_MAX             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_DEF             (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_Spare_HSH             (0x013E0158)

#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x0000015C)
#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_DEF                (0x00000000)
#define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_VOLATILE_BITFIELDS_MSK (0xFFFFFFFF)

  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_OFF        ( 0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_WID        (32)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_MSK        (0xFFFFFFFF)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_MIN        (0)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_MAX        (4294967295) // 0xFFFFFFFF
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_DEF        (0x00000000)
  #define DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_Count_HSH        (0x2080015C)

#define DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000160)
#define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DEF                              (0x00000000)
#define DATA0CH0_CR_DDRCRCOMPDQSDELAY_VOLATILE_BITFIELDS_MSK           (0x05FFFFFF)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_OFF                ( 0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_WID                ( 8)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_MSK                (0x000000FF)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_MIN                (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_MAX                (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayP_HSH                (0x08800160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_OFF              ( 8)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_WID              ( 1)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_MSK              (0x00000100)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_MIN              (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPTargetNUI_HSH              (0x01900160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_OFF              ( 9)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_WID              ( 2)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_MSK              (0x00000600)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_MIN              (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_MAX              (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsPOffsetNUI_HSH              (0x02920160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_OFF                (11)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_WID                ( 8)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_MSK                (0x0007F800)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_MIN                (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_MAX                (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_RxDqsDelayN_HSH                (0x08960160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_OFF              (19)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_WID              ( 1)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_MSK              (0x00080000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_MIN              (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNTargetNUI_HSH              (0x01A60160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_OFF              (20)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_WID              ( 2)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_MSK              (0x00300000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_MIN              (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_MAX              (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_DEF              (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsNOffsetNUI_HSH              (0x02A80160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_OFF               (22)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_WID               ( 1)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_MSK               (0x00400000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_MIN               (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsTargetNUI_HSH               (0x01AC0160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_OFF               (23)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_WID               ( 2)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_MSK               (0x01800000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_MIN               (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_MAX               (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_DqsOffsetNUI_HSH               (0x02AE0160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_OFF               (25)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_WID               ( 1)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_MSK               (0x02000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_MIN               (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_MAX               (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_DEF               (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepBinEn_HSH               (0x01320160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_OFF           (26)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_WID           ( 1)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_MSK           (0x04000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_MIN           (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_MAX           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_DEF           (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_SinStepResumeBin_HSH           (0x01B40160)

  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_OFF                      (27)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_WID                      ( 5)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_MSK                      (0xF8000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_MIN                      (0)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_MAX                      (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_DEF                      (0x00000000)
  #define DATA0CH0_CR_DDRCRCOMPDQSDELAY_Spare_HSH                      (0x05360160)

#define DATA0CH0_CR_DDRCRADC_REG                                       (0x00000164)
#define DATA0CH0_CR_DDRCRADC_DEF                                       (0x0000003C)
#define DATA0CH0_CR_DDRCRADC_VOLATILE_BITFIELDS_MSK                    (0xFF83FF80)

  #define DATA0CH0_CR_DDRCRADC_ADC_frz_OFF                             ( 0)
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_WID                             ( 1)
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_MSK                             (0x00000001)
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_MIN                             (0)
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_MAX                             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_DEF                             (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_ADC_frz_HSH                             (0x01000164)

  #define DATA0CH0_CR_DDRCRADC_ADC_en_OFF                              ( 1)
  #define DATA0CH0_CR_DDRCRADC_ADC_en_WID                              ( 1)
  #define DATA0CH0_CR_DDRCRADC_ADC_en_MSK                              (0x00000002)
  #define DATA0CH0_CR_DDRCRADC_ADC_en_MIN                              (0)
  #define DATA0CH0_CR_DDRCRADC_ADC_en_MAX                              (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRADC_ADC_en_DEF                              (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_ADC_en_HSH                              (0x01020164)

  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_OFF                       ( 2)
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_WID                       ( 2)
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_MSK                       (0x0000000C)
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_MIN                       (0)
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_MAX                       (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_DEF                       (0x00000003)
  #define DATA0CH0_CR_DDRCRADC_ADCstartcount_HSH                       (0x02040164)

  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_OFF                           ( 4)
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_WID                           ( 2)
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_MSK                           (0x00000030)
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_MIN                           (0)
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_MAX                           (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_DEF                           (0x00000003)
  #define DATA0CH0_CR_DDRCRADC_ADCClkDiv_HSH                           (0x02080164)

  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_OFF                           ( 6)
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_WID                           ( 1)
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_MSK                           (0x00000040)
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_MIN                           (0)
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_MAX                           (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_DEF                           (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_ADCChopEn_HSH                           (0x010C0164)

  #define DATA0CH0_CR_DDRCRADC_ADCDone_OFF                             ( 7)
  #define DATA0CH0_CR_DDRCRADC_ADCDone_WID                             ( 1)
  #define DATA0CH0_CR_DDRCRADC_ADCDone_MSK                             (0x00000080)
  #define DATA0CH0_CR_DDRCRADC_ADCDone_MIN                             (0)
  #define DATA0CH0_CR_DDRCRADC_ADCDone_MAX                             (1) // 0x00000001
  #define DATA0CH0_CR_DDRCRADC_ADCDone_DEF                             (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_ADCDone_HSH                             (0x018E0164)

  #define DATA0CH0_CR_DDRCRADC_CountStatus_OFF                         ( 8)
  #define DATA0CH0_CR_DDRCRADC_CountStatus_WID                         (10)
  #define DATA0CH0_CR_DDRCRADC_CountStatus_MSK                         (0x0003FF00)
  #define DATA0CH0_CR_DDRCRADC_CountStatus_MIN                         (0)
  #define DATA0CH0_CR_DDRCRADC_CountStatus_MAX                         (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRADC_CountStatus_DEF                         (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_CountStatus_HSH                         (0x0A900164)

  #define DATA0CH0_CR_DDRCRADC_Spare_OFF                               (18)
  #define DATA0CH0_CR_DDRCRADC_Spare_WID                               ( 5)
  #define DATA0CH0_CR_DDRCRADC_Spare_MSK                               (0x007C0000)
  #define DATA0CH0_CR_DDRCRADC_Spare_MIN                               (0)
  #define DATA0CH0_CR_DDRCRADC_Spare_MAX                               (31) // 0x0000001F
  #define DATA0CH0_CR_DDRCRADC_Spare_DEF                               (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_Spare_HSH                               (0x05240164)

  #define DATA0CH0_CR_DDRCRADC_VrefOffset_OFF                          (23)
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_WID                          ( 9)
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_MSK                          (0xFF800000)
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_MIN                          (-256)
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_MAX                          (255) // 0x000000FF
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_DEF                          (0x00000000)
  #define DATA0CH0_CR_DDRCRADC_VrefOffset_HSH                          (0x89AE0164)

#define DATA0CH0_CR_REFPIFSMCONTROL_REG                                (0x00000168)
#define DATA0CH0_CR_REFPIFSMCONTROL_DEF                                (0x0657FC00)
#define DATA0CH0_CR_REFPIFSMCONTROL_VOLATILE_BITFIELDS_MSK             (0x1007FFFF)

  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_OFF                        ( 0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_WID                        ( 2)
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_MSK                        (0x00000003)
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_MIN                        (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_MAX                        (3) // 0x00000003
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_DEF                        (0x00000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_Param_HSH                        (0x02800168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_OFF                       ( 2)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_WID                       ( 8)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_MSK                       (0x000003FC)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_MIN                       (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_MAX                       (255) // 0x000000FF
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_DEF                       (0x00000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinVal_HSH                       (0x08840168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_OFF                       (10)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_WID                       ( 9)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_MSK                       (0x0007FC00)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_MIN                       (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_MAX                       (511) // 0x000001FF
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_DEF                       (0x000001FF)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MaxVal_HSH                       (0x09940168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_OFF                     (19)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_WID                     ( 6)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_MSK                     (0x01F80000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_MIN                     (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_MAX                     (63) // 0x0000003F
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_DEF                     (0x0000000A)
  #define DATA0CH0_CR_REFPIFSMCONTROL_MinWidth_HSH                     (0x06260168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_OFF                    (25)
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_WID                    ( 3)
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_MSK                    (0x0E000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_MIN                    (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_MAX                    (7) // 0x00000007
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_DEF                    (0x00000003)
  #define DATA0CH0_CR_REFPIFSMCONTROL_TestCycle_HSH                    (0x03320168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_OFF              (28)
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_WID              ( 1)
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_MSK              (0x10000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_MIN              (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_MAX              (1) // 0x00000001
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_DEF              (0x00000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_ForceWrongPhase_HSH              (0x01B80168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_OFF                (29)
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_WID                ( 1)
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_MSK                (0x20000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_MIN                (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_MAX                (1) // 0x00000001
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_DEF                (0x00000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_WriteValParam_HSH                (0x013A0168)

  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_OFF                     (30)
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_WID                     ( 2)
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_MSK                     (0xC0000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_MIN                     (0)
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_MAX                     (3) // 0x00000003
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_DEF                     (0x00000000)
  #define DATA0CH0_CR_REFPIFSMCONTROL_SaveModN_HSH                     (0x023C0168)

#define DATA0CH0_CR_DDRDATADQRANK0LANE8_REG                            (0x0000016C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRDATADQRANK1LANE8_REG                            (0x00000170)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000174)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_DEF                         (0x10040100)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_OFF               ( 0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_MSK               (0x000003FF)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref0_HSH               (0x0A000174)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_OFF               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_MSK               (0x000FFC00)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref1_HSH               (0x0A140174)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_OFF               (20)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_MSK               (0x3FF00000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_RxVref2_HSH               (0x0A280174)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_OFF                  (30)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT012_Rsvd_HSH                  (0x023C0174)

#define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000178)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_DEF                         (0x10040100)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_OFF               ( 0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_MSK               (0x000003FF)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref3_HSH               (0x0A000178)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_OFF               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_MSK               (0x000FFC00)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref4_HSH               (0x0A140178)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_OFF               (20)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_WID               (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_MSK               (0x3FF00000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_MIN               (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_MAX               (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_DEF               (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_RxVref5_HSH               (0x0A280178)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_OFF                  (30)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_WID                  ( 2)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_MSK                  (0xC0000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_MIN                  (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_MAX                  (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_DEF                  (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT345_Rsvd_HSH                  (0x023C0178)

#define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x0000017C)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_DEF                       (0x10040100)
#define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_OFF             ( 0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_WID             (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_MSK             (0x000003FF)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_MAX             (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_DEF             (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref6_HSH             (0x0A00017C)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_OFF             (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_WID             (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_MSK             (0x000FFC00)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_MIN             (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_MAX             (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_DEF             (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVref7_HSH             (0x0A14017C)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_OFF           (20)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_WID           (10)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_MSK           (0x3FF00000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_MIN           (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_MAX           (1023) // 0x000003FF
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_DEF           (0x00000100)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_RxVrefDBI_HSH           (0x0A28017C)

  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_OFF                (30)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_WID                ( 2)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_MSK                (0xC0000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_MIN                (0)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_MAX                (3) // 0x00000003
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_DEF                (0x00000000)
  #define DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_Rsvd_HSH                (0x023C017C)

#define DATA0CH1_CR_DDRDATADQRANK0LANE0_REG                            (0x00000180)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE1_REG                            (0x00000184)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE2_REG                            (0x00000188)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE3_REG                            (0x0000018C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE4_REG                            (0x00000190)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE5_REG                            (0x00000194)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE6_REG                            (0x00000198)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE7_REG                            (0x0000019C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE0_REG                            (0x000001A0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE1_REG                            (0x000001A4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE2_REG                            (0x000001A8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE3_REG                            (0x000001AC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE4_REG                            (0x000001B0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE5_REG                            (0x000001B4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE6_REG                            (0x000001B8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE7_REG                            (0x000001BC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQSRANK0_REG                                (0x000001C0)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA0CH1_CR_DDRDATADQSRANK1_REG                                (0x000001C4)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA0CH1_CR_TXCONTROL0RANK0_REG                                (0x000001C8)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA0CH1_CR_TXCONTROL0RANK1_REG                                (0x000001CC)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA0CH1_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x000001D0)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA0CH1_CR_DDRCRDATACONTROL2_REG                              (0x000001D4)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA0CH1_CR_DCCFSMCONTROL_REG                                  (0x000001D8)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA0CH1_CR_DCCCALCCONTROL_REG                                 (0x000001DC)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA0CH1_CR_DCCLANETARGET_REG                                  (0x000001E0)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA0CH1_CR_DDRCRRANKMUXDELAY_REG                              (0x000001E4)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA0CH1_CR_DDRCRDLLCONTROL0_REG                               (0x000001E8)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA0CH1_CR_DCCPILUT0_REG                                      (0x000001EC)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA0CH1_CR_REFPICONTROL_REG                                   (0x000001F0)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA0CH1_CR_SPARE_REG                                          (0x000001F4)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA0CH1_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x000001F8)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA0CH1_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x000001FC)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA0CH1_CR_TERDCCCONTROL0_REG                                 (0x00000200)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA0CH1_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000204)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA0CH1_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000208)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA0CH1_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000020C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA0CH1_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000210)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA0CH1_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000214)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA0CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000218)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA0CH1_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000021C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA0CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000220)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA0CH1_CR_SPARE1_REG                                         (0x00000224)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA0CH1_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000234)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA0CH1_CR_DDRCRWRRETRAININITRANK1_REG                        (0x0000024C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA0CH1_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000250)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA0CH1_CR_DDRCRRDRETRAININITRANK1_REG                        (0x0000025C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA0CH1_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000260)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA0CH1_CR_DDRCRRXCTLE_REG                                    (0x00000264)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA0CH1_CR_RXCONTROL0RANK0_REG                                (0x00000268)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA0CH1_CR_RXCONTROL0RANK1_REG                                (0x0000026C)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA0CH1_CR_DDRDQREPLICACTLE_REG                               (0x00000270)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA0CH1_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000278)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA0CH1_CR_DDRCRDATACONTROL0_REG                              (0x0000027C)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA0CH1_CR_DDRCRDATACONTROL1_REG                              (0x00000280)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA0CH1_CR_DDRCRDATACONTROL5_REG                              (0x00000284)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA0CH1_CR_DDRCRRXEQTAP01_REG                                 (0x00000288)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA0CH1_CR_DDRCRRXEQTAP_REG                                   (0x0000028C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA0CH1_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000290)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA0CH1_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000294)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA0CH1_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000298)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA0CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000029C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA0CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x000002A0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA0CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x000002A4)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA0CH1_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x000002A8)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA0CH1_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x000002AC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA0CH1_CR_DATATRAINFEEDBACK_REG                              (0x000002B0)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA0CH1_CR_DCCFSMSTATUS_REG                                   (0x000002B4)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA0CH1_CR_DCCLANESTATUS0_REG                                 (0x000002B8)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA0CH1_CR_DCCLANESTATUS1_REG                                 (0x000002BC)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA0CH1_CR_DCCLANESTATUS2_REG                                 (0x000002C0)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA0CH1_CR_DCCLANESTATUS3_REG                                 (0x000002C4)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA0CH1_CR_DDRCRMARGINMODECONTROL0_REG                        (0x000002C8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA0CH1_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x000002CC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA0CH1_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x000002D0)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA0CH1_CR_DDRCRDLLSTATUS_REG                                 (0x000002D4)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA0CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x000002D8)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA0CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x000002DC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA0CH1_CR_DDRCRCOMPDQSDELAY_REG                              (0x000002E0)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA0CH1_CR_DDRCRADC_REG                                       (0x000002E4)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA0CH1_CR_REFPIFSMCONTROL_REG                                (0x000002E8)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA0CH1_CR_DDRDATADQRANK0LANE8_REG                            (0x000002EC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRDATADQRANK1LANE8_REG                            (0x000002F0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA0CH1_CR_DDRCRDATAVREFPERBIT012_REG                         (0x000002F4)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA0CH1_CR_DDRCRDATAVREFPERBIT345_REG                         (0x000002F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA0CH1_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x000002FC)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000300)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000304)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000308)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x0000030C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000310)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000314)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000318)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x0000031C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000320)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000324)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000328)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x0000032C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000330)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000334)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000338)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x0000033C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQSRANK0_REG                                (0x00000340)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA1CH0_CR_DDRDATADQSRANK1_REG                                (0x00000344)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA1CH0_CR_TXCONTROL0RANK0_REG                                (0x00000348)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA1CH0_CR_TXCONTROL0RANK1_REG                                (0x0000034C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA1CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000350)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA1CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000354)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA1CH0_CR_DCCFSMCONTROL_REG                                  (0x00000358)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA1CH0_CR_DCCCALCCONTROL_REG                                 (0x0000035C)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA1CH0_CR_DCCLANETARGET_REG                                  (0x00000360)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA1CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000364)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA1CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000368)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA1CH0_CR_DCCPILUT0_REG                                      (0x0000036C)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA1CH0_CR_REFPICONTROL_REG                                   (0x00000370)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA1CH0_CR_SPARE_REG                                          (0x00000374)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA1CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000378)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA1CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x0000037C)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA1CH0_CR_TERDCCCONTROL0_REG                                 (0x00000380)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA1CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000384)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA1CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000388)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA1CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000038C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA1CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000390)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA1CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000394)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA1CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000398)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA1CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000039C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA1CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x000003A0)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA1CH0_CR_SPARE1_REG                                         (0x000003A4)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA1CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x000003B4)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA1CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x000003CC)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA1CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x000003D0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA1CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x000003DC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA1CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x000003E0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA1CH0_CR_DDRCRRXCTLE_REG                                    (0x000003E4)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA1CH0_CR_RXCONTROL0RANK0_REG                                (0x000003E8)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA1CH0_CR_RXCONTROL0RANK1_REG                                (0x000003EC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA1CH0_CR_DDRDQREPLICACTLE_REG                               (0x000003F0)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA1CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x000003F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA1CH0_CR_DDRCRDATACONTROL0_REG                              (0x000003FC)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA1CH0_CR_DDRCRDATACONTROL1_REG                              (0x00000400)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA1CH0_CR_DDRCRDATACONTROL5_REG                              (0x00000404)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA1CH0_CR_DDRCRRXEQTAP01_REG                                 (0x00000408)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA1CH0_CR_DDRCRRXEQTAP_REG                                   (0x0000040C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA1CH0_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000410)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA1CH0_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000414)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA1CH0_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000418)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA1CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000041C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA1CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000420)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA1CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000424)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA1CH0_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000428)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA1CH0_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x0000042C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA1CH0_CR_DATATRAINFEEDBACK_REG                              (0x00000430)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA1CH0_CR_DCCFSMSTATUS_REG                                   (0x00000434)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA1CH0_CR_DCCLANESTATUS0_REG                                 (0x00000438)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA1CH0_CR_DCCLANESTATUS1_REG                                 (0x0000043C)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA1CH0_CR_DCCLANESTATUS2_REG                                 (0x00000440)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA1CH0_CR_DCCLANESTATUS3_REG                                 (0x00000444)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA1CH0_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000448)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA1CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x0000044C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA1CH0_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000450)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA1CH0_CR_DDRCRDLLSTATUS_REG                                 (0x00000454)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA1CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000458)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA1CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x0000045C)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA1CH0_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000460)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA1CH0_CR_DDRCRADC_REG                                       (0x00000464)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA1CH0_CR_REFPIFSMCONTROL_REG                                (0x00000468)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA1CH0_CR_DDRDATADQRANK0LANE8_REG                            (0x0000046C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRDATADQRANK1LANE8_REG                            (0x00000470)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH0_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000474)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA1CH0_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000478)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA1CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x0000047C)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE0_REG                            (0x00000480)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE1_REG                            (0x00000484)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE2_REG                            (0x00000488)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE3_REG                            (0x0000048C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE4_REG                            (0x00000490)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE5_REG                            (0x00000494)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE6_REG                            (0x00000498)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE7_REG                            (0x0000049C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE0_REG                            (0x000004A0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE1_REG                            (0x000004A4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE2_REG                            (0x000004A8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE3_REG                            (0x000004AC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE4_REG                            (0x000004B0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE5_REG                            (0x000004B4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE6_REG                            (0x000004B8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE7_REG                            (0x000004BC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQSRANK0_REG                                (0x000004C0)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA1CH1_CR_DDRDATADQSRANK1_REG                                (0x000004C4)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA1CH1_CR_TXCONTROL0RANK0_REG                                (0x000004C8)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA1CH1_CR_TXCONTROL0RANK1_REG                                (0x000004CC)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA1CH1_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x000004D0)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA1CH1_CR_DDRCRDATACONTROL2_REG                              (0x000004D4)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA1CH1_CR_DCCFSMCONTROL_REG                                  (0x000004D8)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA1CH1_CR_DCCCALCCONTROL_REG                                 (0x000004DC)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA1CH1_CR_DCCLANETARGET_REG                                  (0x000004E0)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA1CH1_CR_DDRCRRANKMUXDELAY_REG                              (0x000004E4)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA1CH1_CR_DDRCRDLLCONTROL0_REG                               (0x000004E8)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA1CH1_CR_DCCPILUT0_REG                                      (0x000004EC)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA1CH1_CR_REFPICONTROL_REG                                   (0x000004F0)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA1CH1_CR_SPARE_REG                                          (0x000004F4)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA1CH1_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x000004F8)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA1CH1_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x000004FC)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA1CH1_CR_TERDCCCONTROL0_REG                                 (0x00000500)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA1CH1_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000504)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA1CH1_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000508)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA1CH1_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000050C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA1CH1_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000510)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA1CH1_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000514)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA1CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000518)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA1CH1_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000051C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA1CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000520)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA1CH1_CR_SPARE1_REG                                         (0x00000524)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA1CH1_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000534)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA1CH1_CR_DDRCRWRRETRAININITRANK1_REG                        (0x0000054C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA1CH1_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000550)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA1CH1_CR_DDRCRRDRETRAININITRANK1_REG                        (0x0000055C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA1CH1_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000560)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA1CH1_CR_DDRCRRXCTLE_REG                                    (0x00000564)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA1CH1_CR_RXCONTROL0RANK0_REG                                (0x00000568)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA1CH1_CR_RXCONTROL0RANK1_REG                                (0x0000056C)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA1CH1_CR_DDRDQREPLICACTLE_REG                               (0x00000570)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA1CH1_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000578)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA1CH1_CR_DDRCRDATACONTROL0_REG                              (0x0000057C)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA1CH1_CR_DDRCRDATACONTROL1_REG                              (0x00000580)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA1CH1_CR_DDRCRDATACONTROL5_REG                              (0x00000584)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA1CH1_CR_DDRCRRXEQTAP01_REG                                 (0x00000588)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA1CH1_CR_DDRCRRXEQTAP_REG                                   (0x0000058C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA1CH1_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000590)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA1CH1_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000594)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA1CH1_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000598)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA1CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000059C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA1CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x000005A0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA1CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x000005A4)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA1CH1_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x000005A8)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA1CH1_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x000005AC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA1CH1_CR_DATATRAINFEEDBACK_REG                              (0x000005B0)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA1CH1_CR_DCCFSMSTATUS_REG                                   (0x000005B4)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA1CH1_CR_DCCLANESTATUS0_REG                                 (0x000005B8)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA1CH1_CR_DCCLANESTATUS1_REG                                 (0x000005BC)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA1CH1_CR_DCCLANESTATUS2_REG                                 (0x000005C0)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA1CH1_CR_DCCLANESTATUS3_REG                                 (0x000005C4)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA1CH1_CR_DDRCRMARGINMODECONTROL0_REG                        (0x000005C8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA1CH1_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x000005CC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA1CH1_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x000005D0)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA1CH1_CR_DDRCRDLLSTATUS_REG                                 (0x000005D4)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA1CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x000005D8)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA1CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x000005DC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA1CH1_CR_DDRCRCOMPDQSDELAY_REG                              (0x000005E0)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA1CH1_CR_DDRCRADC_REG                                       (0x000005E4)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA1CH1_CR_REFPIFSMCONTROL_REG                                (0x000005E8)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA1CH1_CR_DDRDATADQRANK0LANE8_REG                            (0x000005EC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRDATADQRANK1LANE8_REG                            (0x000005F0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA1CH1_CR_DDRCRDATAVREFPERBIT012_REG                         (0x000005F4)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA1CH1_CR_DDRCRDATAVREFPERBIT345_REG                         (0x000005F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA1CH1_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x000005FC)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000600)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000604)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000608)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x0000060C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000610)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000614)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000618)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x0000061C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000620)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000624)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000628)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x0000062C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000630)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000634)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000638)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x0000063C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQSRANK0_REG                                (0x00000640)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA2CH0_CR_DDRDATADQSRANK1_REG                                (0x00000644)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA2CH0_CR_TXCONTROL0RANK0_REG                                (0x00000648)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA2CH0_CR_TXCONTROL0RANK1_REG                                (0x0000064C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA2CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000650)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA2CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000654)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA2CH0_CR_DCCFSMCONTROL_REG                                  (0x00000658)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA2CH0_CR_DCCCALCCONTROL_REG                                 (0x0000065C)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA2CH0_CR_DCCLANETARGET_REG                                  (0x00000660)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA2CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000664)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA2CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000668)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA2CH0_CR_DCCPILUT0_REG                                      (0x0000066C)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA2CH0_CR_REFPICONTROL_REG                                   (0x00000670)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA2CH0_CR_SPARE_REG                                          (0x00000674)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA2CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000678)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA2CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x0000067C)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA2CH0_CR_TERDCCCONTROL0_REG                                 (0x00000680)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA2CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000684)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA2CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000688)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA2CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000068C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA2CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000690)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA2CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000694)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA2CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000698)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA2CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000069C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA2CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x000006A0)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA2CH0_CR_SPARE1_REG                                         (0x000006A4)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA2CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x000006B4)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA2CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x000006CC)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA2CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x000006D0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA2CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x000006DC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA2CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x000006E0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA2CH0_CR_DDRCRRXCTLE_REG                                    (0x000006E4)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA2CH0_CR_RXCONTROL0RANK0_REG                                (0x000006E8)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA2CH0_CR_RXCONTROL0RANK1_REG                                (0x000006EC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA2CH0_CR_DDRDQREPLICACTLE_REG                               (0x000006F0)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA2CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x000006F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA2CH0_CR_DDRCRDATACONTROL0_REG                              (0x000006FC)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA2CH0_CR_DDRCRDATACONTROL1_REG                              (0x00000700)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA2CH0_CR_DDRCRDATACONTROL5_REG                              (0x00000704)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA2CH0_CR_DDRCRRXEQTAP01_REG                                 (0x00000708)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA2CH0_CR_DDRCRRXEQTAP_REG                                   (0x0000070C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA2CH0_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000710)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA2CH0_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000714)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA2CH0_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000718)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA2CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000071C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA2CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000720)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA2CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000724)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA2CH0_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000728)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA2CH0_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x0000072C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA2CH0_CR_DATATRAINFEEDBACK_REG                              (0x00000730)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA2CH0_CR_DCCFSMSTATUS_REG                                   (0x00000734)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA2CH0_CR_DCCLANESTATUS0_REG                                 (0x00000738)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA2CH0_CR_DCCLANESTATUS1_REG                                 (0x0000073C)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA2CH0_CR_DCCLANESTATUS2_REG                                 (0x00000740)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA2CH0_CR_DCCLANESTATUS3_REG                                 (0x00000744)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA2CH0_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000748)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA2CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x0000074C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA2CH0_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000750)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA2CH0_CR_DDRCRDLLSTATUS_REG                                 (0x00000754)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA2CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000758)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA2CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x0000075C)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA2CH0_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000760)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA2CH0_CR_DDRCRADC_REG                                       (0x00000764)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA2CH0_CR_REFPIFSMCONTROL_REG                                (0x00000768)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA2CH0_CR_DDRDATADQRANK0LANE8_REG                            (0x0000076C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRDATADQRANK1LANE8_REG                            (0x00000770)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH0_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000774)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA2CH0_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000778)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA2CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x0000077C)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE0_REG                            (0x00000780)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE1_REG                            (0x00000784)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE2_REG                            (0x00000788)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE3_REG                            (0x0000078C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE4_REG                            (0x00000790)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE5_REG                            (0x00000794)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE6_REG                            (0x00000798)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE7_REG                            (0x0000079C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE0_REG                            (0x000007A0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE1_REG                            (0x000007A4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE2_REG                            (0x000007A8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE3_REG                            (0x000007AC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE4_REG                            (0x000007B0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE5_REG                            (0x000007B4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE6_REG                            (0x000007B8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE7_REG                            (0x000007BC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQSRANK0_REG                                (0x000007C0)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA2CH1_CR_DDRDATADQSRANK1_REG                                (0x000007C4)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA2CH1_CR_TXCONTROL0RANK0_REG                                (0x000007C8)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA2CH1_CR_TXCONTROL0RANK1_REG                                (0x000007CC)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA2CH1_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x000007D0)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA2CH1_CR_DDRCRDATACONTROL2_REG                              (0x000007D4)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA2CH1_CR_DCCFSMCONTROL_REG                                  (0x000007D8)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA2CH1_CR_DCCCALCCONTROL_REG                                 (0x000007DC)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA2CH1_CR_DCCLANETARGET_REG                                  (0x000007E0)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA2CH1_CR_DDRCRRANKMUXDELAY_REG                              (0x000007E4)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA2CH1_CR_DDRCRDLLCONTROL0_REG                               (0x000007E8)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA2CH1_CR_DCCPILUT0_REG                                      (0x000007EC)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA2CH1_CR_REFPICONTROL_REG                                   (0x000007F0)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA2CH1_CR_SPARE_REG                                          (0x000007F4)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA2CH1_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x000007F8)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA2CH1_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x000007FC)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA2CH1_CR_TERDCCCONTROL0_REG                                 (0x00000800)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA2CH1_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000804)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA2CH1_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000808)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA2CH1_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000080C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA2CH1_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000810)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA2CH1_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000814)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA2CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000818)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA2CH1_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000081C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA2CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000820)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA2CH1_CR_SPARE1_REG                                         (0x00000824)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA2CH1_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000834)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA2CH1_CR_DDRCRWRRETRAININITRANK1_REG                        (0x0000084C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA2CH1_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000850)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA2CH1_CR_DDRCRRDRETRAININITRANK1_REG                        (0x0000085C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA2CH1_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000860)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA2CH1_CR_DDRCRRXCTLE_REG                                    (0x00000864)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA2CH1_CR_RXCONTROL0RANK0_REG                                (0x00000868)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA2CH1_CR_RXCONTROL0RANK1_REG                                (0x0000086C)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA2CH1_CR_DDRDQREPLICACTLE_REG                               (0x00000870)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA2CH1_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000878)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA2CH1_CR_DDRCRDATACONTROL0_REG                              (0x0000087C)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA2CH1_CR_DDRCRDATACONTROL1_REG                              (0x00000880)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA2CH1_CR_DDRCRDATACONTROL5_REG                              (0x00000884)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA2CH1_CR_DDRCRRXEQTAP01_REG                                 (0x00000888)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA2CH1_CR_DDRCRRXEQTAP_REG                                   (0x0000088C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA2CH1_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000890)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA2CH1_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000894)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA2CH1_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000898)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA2CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x0000089C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA2CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x000008A0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA2CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x000008A4)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA2CH1_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x000008A8)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA2CH1_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x000008AC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA2CH1_CR_DATATRAINFEEDBACK_REG                              (0x000008B0)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA2CH1_CR_DCCFSMSTATUS_REG                                   (0x000008B4)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA2CH1_CR_DCCLANESTATUS0_REG                                 (0x000008B8)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA2CH1_CR_DCCLANESTATUS1_REG                                 (0x000008BC)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA2CH1_CR_DCCLANESTATUS2_REG                                 (0x000008C0)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA2CH1_CR_DCCLANESTATUS3_REG                                 (0x000008C4)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA2CH1_CR_DDRCRMARGINMODECONTROL0_REG                        (0x000008C8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA2CH1_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x000008CC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA2CH1_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x000008D0)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA2CH1_CR_DDRCRDLLSTATUS_REG                                 (0x000008D4)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA2CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x000008D8)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA2CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x000008DC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA2CH1_CR_DDRCRCOMPDQSDELAY_REG                              (0x000008E0)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA2CH1_CR_DDRCRADC_REG                                       (0x000008E4)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA2CH1_CR_REFPIFSMCONTROL_REG                                (0x000008E8)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA2CH1_CR_DDRDATADQRANK0LANE8_REG                            (0x000008EC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRDATADQRANK1LANE8_REG                            (0x000008F0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA2CH1_CR_DDRCRDATAVREFPERBIT012_REG                         (0x000008F4)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA2CH1_CR_DDRCRDATAVREFPERBIT345_REG                         (0x000008F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA2CH1_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x000008FC)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000900)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000904)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000908)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x0000090C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000910)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000914)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000918)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x0000091C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000920)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000924)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000928)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x0000092C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000930)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000934)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000938)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x0000093C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQSRANK0_REG                                (0x00000940)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA3CH0_CR_DDRDATADQSRANK1_REG                                (0x00000944)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA3CH0_CR_TXCONTROL0RANK0_REG                                (0x00000948)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA3CH0_CR_TXCONTROL0RANK1_REG                                (0x0000094C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA3CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000950)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA3CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000954)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA3CH0_CR_DCCFSMCONTROL_REG                                  (0x00000958)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA3CH0_CR_DCCCALCCONTROL_REG                                 (0x0000095C)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA3CH0_CR_DCCLANETARGET_REG                                  (0x00000960)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA3CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000964)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA3CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000968)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA3CH0_CR_DCCPILUT0_REG                                      (0x0000096C)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA3CH0_CR_REFPICONTROL_REG                                   (0x00000970)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA3CH0_CR_SPARE_REG                                          (0x00000974)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA3CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000978)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA3CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x0000097C)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA3CH0_CR_TERDCCCONTROL0_REG                                 (0x00000980)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA3CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000984)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA3CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000988)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA3CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x0000098C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA3CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000990)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA3CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000994)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA3CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000998)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA3CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x0000099C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA3CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x000009A0)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA3CH0_CR_SPARE1_REG                                         (0x000009A4)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA3CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x000009B4)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA3CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x000009CC)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA3CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x000009D0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA3CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x000009DC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA3CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x000009E0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA3CH0_CR_DDRCRRXCTLE_REG                                    (0x000009E4)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA3CH0_CR_RXCONTROL0RANK0_REG                                (0x000009E8)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA3CH0_CR_RXCONTROL0RANK1_REG                                (0x000009EC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA3CH0_CR_DDRDQREPLICACTLE_REG                               (0x000009F0)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA3CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x000009F8)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA3CH0_CR_DDRCRDATACONTROL0_REG                              (0x000009FC)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA3CH0_CR_DDRCRDATACONTROL1_REG                              (0x00000A00)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA3CH0_CR_DDRCRDATACONTROL5_REG                              (0x00000A04)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA3CH0_CR_DDRCRRXEQTAP01_REG                                 (0x00000A08)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA3CH0_CR_DDRCRRXEQTAP_REG                                   (0x00000A0C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA3CH0_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000A10)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA3CH0_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000A14)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA3CH0_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000A18)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA3CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x00000A1C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA3CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000A20)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA3CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000A24)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA3CH0_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000A28)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA3CH0_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x00000A2C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA3CH0_CR_DATATRAINFEEDBACK_REG                              (0x00000A30)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA3CH0_CR_DCCFSMSTATUS_REG                                   (0x00000A34)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA3CH0_CR_DCCLANESTATUS0_REG                                 (0x00000A38)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA3CH0_CR_DCCLANESTATUS1_REG                                 (0x00000A3C)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA3CH0_CR_DCCLANESTATUS2_REG                                 (0x00000A40)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA3CH0_CR_DCCLANESTATUS3_REG                                 (0x00000A44)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA3CH0_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000A48)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA3CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x00000A4C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA3CH0_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000A50)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA3CH0_CR_DDRCRDLLSTATUS_REG                                 (0x00000A54)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA3CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000A58)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA3CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x00000A5C)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA3CH0_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000A60)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA3CH0_CR_DDRCRADC_REG                                       (0x00000A64)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA3CH0_CR_REFPIFSMCONTROL_REG                                (0x00000A68)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA3CH0_CR_DDRDATADQRANK0LANE8_REG                            (0x00000A6C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRDATADQRANK1LANE8_REG                            (0x00000A70)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH0_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000A74)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA3CH0_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000A78)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA3CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x00000A7C)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE0_REG                            (0x00000A80)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE1_REG                            (0x00000A84)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE2_REG                            (0x00000A88)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE3_REG                            (0x00000A8C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE4_REG                            (0x00000A90)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE5_REG                            (0x00000A94)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE6_REG                            (0x00000A98)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE7_REG                            (0x00000A9C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE0_REG                            (0x00000AA0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE1_REG                            (0x00000AA4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE2_REG                            (0x00000AA8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE3_REG                            (0x00000AAC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE4_REG                            (0x00000AB0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE5_REG                            (0x00000AB4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE6_REG                            (0x00000AB8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE7_REG                            (0x00000ABC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQSRANK0_REG                                (0x00000AC0)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA3CH1_CR_DDRDATADQSRANK1_REG                                (0x00000AC4)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA3CH1_CR_TXCONTROL0RANK0_REG                                (0x00000AC8)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA3CH1_CR_TXCONTROL0RANK1_REG                                (0x00000ACC)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA3CH1_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000AD0)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA3CH1_CR_DDRCRDATACONTROL2_REG                              (0x00000AD4)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA3CH1_CR_DCCFSMCONTROL_REG                                  (0x00000AD8)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA3CH1_CR_DCCCALCCONTROL_REG                                 (0x00000ADC)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA3CH1_CR_DCCLANETARGET_REG                                  (0x00000AE0)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA3CH1_CR_DDRCRRANKMUXDELAY_REG                              (0x00000AE4)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA3CH1_CR_DDRCRDLLCONTROL0_REG                               (0x00000AE8)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA3CH1_CR_DCCPILUT0_REG                                      (0x00000AEC)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA3CH1_CR_REFPICONTROL_REG                                   (0x00000AF0)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA3CH1_CR_SPARE_REG                                          (0x00000AF4)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA3CH1_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000AF8)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA3CH1_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x00000AFC)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA3CH1_CR_TERDCCCONTROL0_REG                                 (0x00000B00)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA3CH1_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000B04)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA3CH1_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000B08)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA3CH1_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x00000B0C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA3CH1_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000B10)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA3CH1_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000B14)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA3CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000B18)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA3CH1_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x00000B1C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA3CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000B20)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA3CH1_CR_SPARE1_REG                                         (0x00000B24)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA3CH1_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000B34)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA3CH1_CR_DDRCRWRRETRAININITRANK1_REG                        (0x00000B4C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA3CH1_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000B50)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA3CH1_CR_DDRCRRDRETRAININITRANK1_REG                        (0x00000B5C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA3CH1_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000B60)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA3CH1_CR_DDRCRRXCTLE_REG                                    (0x00000B64)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA3CH1_CR_RXCONTROL0RANK0_REG                                (0x00000B68)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA3CH1_CR_RXCONTROL0RANK1_REG                                (0x00000B6C)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA3CH1_CR_DDRDQREPLICACTLE_REG                               (0x00000B70)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA3CH1_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000B78)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA3CH1_CR_DDRCRDATACONTROL0_REG                              (0x00000B7C)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA3CH1_CR_DDRCRDATACONTROL1_REG                              (0x00000B80)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA3CH1_CR_DDRCRDATACONTROL5_REG                              (0x00000B84)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA3CH1_CR_DDRCRRXEQTAP01_REG                                 (0x00000B88)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA3CH1_CR_DDRCRRXEQTAP_REG                                   (0x00000B8C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA3CH1_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000B90)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA3CH1_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000B94)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA3CH1_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000B98)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA3CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x00000B9C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA3CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000BA0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA3CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000BA4)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA3CH1_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000BA8)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA3CH1_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x00000BAC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA3CH1_CR_DATATRAINFEEDBACK_REG                              (0x00000BB0)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA3CH1_CR_DCCFSMSTATUS_REG                                   (0x00000BB4)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA3CH1_CR_DCCLANESTATUS0_REG                                 (0x00000BB8)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA3CH1_CR_DCCLANESTATUS1_REG                                 (0x00000BBC)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA3CH1_CR_DCCLANESTATUS2_REG                                 (0x00000BC0)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA3CH1_CR_DCCLANESTATUS3_REG                                 (0x00000BC4)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA3CH1_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000BC8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA3CH1_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x00000BCC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA3CH1_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000BD0)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA3CH1_CR_DDRCRDLLSTATUS_REG                                 (0x00000BD4)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA3CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000BD8)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA3CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x00000BDC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA3CH1_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000BE0)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA3CH1_CR_DDRCRADC_REG                                       (0x00000BE4)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA3CH1_CR_REFPIFSMCONTROL_REG                                (0x00000BE8)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA3CH1_CR_DDRDATADQRANK0LANE8_REG                            (0x00000BEC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRDATADQRANK1LANE8_REG                            (0x00000BF0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA3CH1_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000BF4)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA3CH1_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000BF8)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA3CH1_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x00000BFC)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000C00)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000C04)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000C08)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x00000C0C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000C10)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000C14)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000C18)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x00000C1C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000C20)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000C24)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000C28)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x00000C2C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000C30)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000C34)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000C38)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x00000C3C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQSRANK0_REG                                (0x00000C40)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA4CH0_CR_DDRDATADQSRANK1_REG                                (0x00000C44)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA4CH0_CR_TXCONTROL0RANK0_REG                                (0x00000C48)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA4CH0_CR_TXCONTROL0RANK1_REG                                (0x00000C4C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA4CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000C50)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA4CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000C54)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA4CH0_CR_DCCFSMCONTROL_REG                                  (0x00000C58)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA4CH0_CR_DCCCALCCONTROL_REG                                 (0x00000C5C)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA4CH0_CR_DCCLANETARGET_REG                                  (0x00000C60)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA4CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000C64)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA4CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000C68)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA4CH0_CR_DCCPILUT0_REG                                      (0x00000C6C)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA4CH0_CR_REFPICONTROL_REG                                   (0x00000C70)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA4CH0_CR_SPARE_REG                                          (0x00000C74)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA4CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000C78)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA4CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x00000C7C)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA4CH0_CR_TERDCCCONTROL0_REG                                 (0x00000C80)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA4CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000C84)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA4CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000C88)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA4CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x00000C8C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA4CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000C90)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA4CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000C94)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA4CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000C98)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA4CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x00000C9C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA4CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000CA0)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA4CH0_CR_SPARE1_REG                                         (0x00000CA4)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA4CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000CB4)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA4CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x00000CCC)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA4CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000CD0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA4CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x00000CDC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA4CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000CE0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA4CH0_CR_DDRCRRXCTLE_REG                                    (0x00000CE4)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA4CH0_CR_RXCONTROL0RANK0_REG                                (0x00000CE8)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA4CH0_CR_RXCONTROL0RANK1_REG                                (0x00000CEC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA4CH0_CR_DDRDQREPLICACTLE_REG                               (0x00000CF0)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA4CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000CF8)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA4CH0_CR_DDRCRDATACONTROL0_REG                              (0x00000CFC)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA4CH0_CR_DDRCRDATACONTROL1_REG                              (0x00000D00)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA4CH0_CR_DDRCRDATACONTROL5_REG                              (0x00000D04)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA4CH0_CR_DDRCRRXEQTAP01_REG                                 (0x00000D08)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA4CH0_CR_DDRCRRXEQTAP_REG                                   (0x00000D0C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA4CH0_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000D10)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA4CH0_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000D14)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA4CH0_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000D18)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA4CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x00000D1C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA4CH0_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000D20)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA4CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000D24)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA4CH0_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000D28)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA4CH0_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x00000D2C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA4CH0_CR_DATATRAINFEEDBACK_REG                              (0x00000D30)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA4CH0_CR_DCCFSMSTATUS_REG                                   (0x00000D34)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA4CH0_CR_DCCLANESTATUS0_REG                                 (0x00000D38)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA4CH0_CR_DCCLANESTATUS1_REG                                 (0x00000D3C)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA4CH0_CR_DCCLANESTATUS2_REG                                 (0x00000D40)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA4CH0_CR_DCCLANESTATUS3_REG                                 (0x00000D44)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA4CH0_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000D48)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA4CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x00000D4C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA4CH0_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000D50)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA4CH0_CR_DDRCRDLLSTATUS_REG                                 (0x00000D54)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA4CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000D58)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA4CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x00000D5C)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA4CH0_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000D60)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA4CH0_CR_DDRCRADC_REG                                       (0x00000D64)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA4CH0_CR_REFPIFSMCONTROL_REG                                (0x00000D68)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA4CH0_CR_DDRDATADQRANK0LANE8_REG                            (0x00000D6C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRDATADQRANK1LANE8_REG                            (0x00000D70)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH0_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000D74)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA4CH0_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000D78)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA4CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x00000D7C)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE0_REG                            (0x00000D80)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE1_REG                            (0x00000D84)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE2_REG                            (0x00000D88)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE3_REG                            (0x00000D8C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE4_REG                            (0x00000D90)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE5_REG                            (0x00000D94)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE6_REG                            (0x00000D98)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE7_REG                            (0x00000D9C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE0_REG                            (0x00000DA0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE1_REG                            (0x00000DA4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE2_REG                            (0x00000DA8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE3_REG                            (0x00000DAC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE4_REG                            (0x00000DB0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE5_REG                            (0x00000DB4)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE6_REG                            (0x00000DB8)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE7_REG                            (0x00000DBC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQSRANK0_REG                                (0x00000DC0)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA4CH1_CR_DDRDATADQSRANK1_REG                                (0x00000DC4)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA4CH1_CR_TXCONTROL0RANK0_REG                                (0x00000DC8)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA4CH1_CR_TXCONTROL0RANK1_REG                                (0x00000DCC)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA4CH1_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000DD0)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA4CH1_CR_DDRCRDATACONTROL2_REG                              (0x00000DD4)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA4CH1_CR_DCCFSMCONTROL_REG                                  (0x00000DD8)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA4CH1_CR_DCCCALCCONTROL_REG                                 (0x00000DDC)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA4CH1_CR_DCCLANETARGET_REG                                  (0x00000DE0)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA4CH1_CR_DDRCRRANKMUXDELAY_REG                              (0x00000DE4)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA4CH1_CR_DDRCRDLLCONTROL0_REG                               (0x00000DE8)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA4CH1_CR_DCCPILUT0_REG                                      (0x00000DEC)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA4CH1_CR_REFPICONTROL_REG                                   (0x00000DF0)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA4CH1_CR_SPARE_REG                                          (0x00000DF4)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA4CH1_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000DF8)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA4CH1_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x00000DFC)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA4CH1_CR_TERDCCCONTROL0_REG                                 (0x00000E00)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA4CH1_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000E04)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA4CH1_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000E08)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA4CH1_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x00000E0C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA4CH1_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000E10)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA4CH1_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000E14)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA4CH1_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000E18)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA4CH1_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x00000E1C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA4CH1_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000E20)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA4CH1_CR_SPARE1_REG                                         (0x00000E24)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA4CH1_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000E34)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA4CH1_CR_DDRCRWRRETRAININITRANK1_REG                        (0x00000E4C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA4CH1_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000E50)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA4CH1_CR_DDRCRRDRETRAININITRANK1_REG                        (0x00000E5C)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA4CH1_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000E60)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA4CH1_CR_DDRCRRXCTLE_REG                                    (0x00000E64)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA4CH1_CR_RXCONTROL0RANK0_REG                                (0x00000E68)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA4CH1_CR_RXCONTROL0RANK1_REG                                (0x00000E6C)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA4CH1_CR_DDRDQREPLICACTLE_REG                               (0x00000E70)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA4CH1_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000E78)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA4CH1_CR_DDRCRDATACONTROL0_REG                              (0x00000E7C)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG

#define DATA4CH1_CR_DDRCRDATACONTROL1_REG                              (0x00000E80)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL1_REG

#define DATA4CH1_CR_DDRCRDATACONTROL5_REG                              (0x00000E84)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL5_REG

#define DATA4CH1_CR_DDRCRRXEQTAP01_REG                                 (0x00000E88)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP01_REG

#define DATA4CH1_CR_DDRCRRXEQTAP_REG                                   (0x00000E8C)
//Duplicate of DATA0CH0_CR_DDRCRRXEQTAP_REG

#define DATA4CH1_CR_DDRCRCMDBUSTRAIN_REG                               (0x00000E90)
//Duplicate of DATA0CH0_CR_DDRCRCMDBUSTRAIN_REG

#define DATA4CH1_CR_DDRCRDATATCOCONTROL0_REG                           (0x00000E94)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL0_REG

#define DATA4CH1_CR_DDRCRDATATCOCONTROL1_REG                           (0x00000E98)
//Duplicate of DATA0CH0_CR_DDRCRDATATCOCONTROL1_REG

#define DATA4CH1_CR_DDRCRWRRETRAINCONTROLSTATUS_REG                    (0x00000E9C)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA4CH1_CR_DDRCRRDRETRAINCONTROLSTATUS_REG                    (0x00000EA0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINCONTROLSTATUS_REG

#define DATA4CH1_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG                   (0x00000EA4)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINSWIZZLECONTROL_REG

#define DATA4CH1_CR_DDRCRWRRETRAINDELTARANK_REG                        (0x00000EA8)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAINDELTARANK_REG

#define DATA4CH1_CR_DDRCRRDRETRAINDELTARANK_REG                        (0x00000EAC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAINDELTARANK_REG

#define DATA4CH1_CR_DATATRAINFEEDBACK_REG                              (0x00000EB0)
//Duplicate of DATA0CH0_CR_DATATRAINFEEDBACK_REG

#define DATA4CH1_CR_DCCFSMSTATUS_REG                                   (0x00000EB4)
//Duplicate of DATA0CH0_CR_DCCFSMSTATUS_REG

#define DATA4CH1_CR_DCCLANESTATUS0_REG                                 (0x00000EB8)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS0_REG

#define DATA4CH1_CR_DCCLANESTATUS1_REG                                 (0x00000EBC)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS1_REG

#define DATA4CH1_CR_DCCLANESTATUS2_REG                                 (0x00000EC0)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS2_REG

#define DATA4CH1_CR_DCCLANESTATUS3_REG                                 (0x00000EC4)
//Duplicate of DATA0CH0_CR_DCCLANESTATUS3_REG

#define DATA4CH1_CR_DDRCRMARGINMODECONTROL0_REG                        (0x00000EC8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODECONTROL0_REG

#define DATA4CH1_CR_DDRCRMARGINMODEDEBUGMSB0_REG                       (0x00000ECC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA4CH1_CR_DDRCRMARGINMODEDEBUGLSB0_REG                       (0x00000ED0)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATA4CH1_CR_DDRCRDLLSTATUS_REG                                 (0x00000ED4)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATA4CH1_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                     (0x00000ED8)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define DATA4CH1_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                (0x00000EDC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATA4CH1_CR_DDRCRCOMPDQSDELAY_REG                              (0x00000EE0)
//Duplicate of DATA0CH0_CR_DDRCRCOMPDQSDELAY_REG

#define DATA4CH1_CR_DDRCRADC_REG                                       (0x00000EE4)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define DATA4CH1_CR_REFPIFSMCONTROL_REG                                (0x00000EE8)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define DATA4CH1_CR_DDRDATADQRANK0LANE8_REG                            (0x00000EEC)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRDATADQRANK1LANE8_REG                            (0x00000EF0)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA4CH1_CR_DDRCRDATAVREFPERBIT012_REG                         (0x00000EF4)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT012_REG

#define DATA4CH1_CR_DDRCRDATAVREFPERBIT345_REG                         (0x00000EF8)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT345_REG

#define DATA4CH1_CR_DDRCRDATAVREFPERBIT67DBI_REG                       (0x00000EFC)
//Duplicate of DATA0CH0_CR_DDRCRDATAVREFPERBIT67DBI_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE0_REG                            (0x00000F00)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE1_REG                            (0x00000F04)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE2_REG                            (0x00000F08)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE3_REG                            (0x00000F0C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE4_REG                            (0x00000F10)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE5_REG                            (0x00000F14)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE6_REG                            (0x00000F18)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK0LANE7_REG                            (0x00000F1C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE0_REG                            (0x00000F20)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE1_REG                            (0x00000F24)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE2_REG                            (0x00000F28)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE3_REG                            (0x00000F2C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE4_REG                            (0x00000F30)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE5_REG                            (0x00000F34)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE6_REG                            (0x00000F38)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQRANK1LANE7_REG                            (0x00000F3C)
//Duplicate of DATA0CH0_CR_DDRDATADQRANK0LANE0_REG

#define DATA5CH0_CR_DDRDATADQSRANK0_REG                                (0x00000F40)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA5CH0_CR_DDRDATADQSRANK1_REG                                (0x00000F44)
//Duplicate of DATA0CH0_CR_DDRDATADQSRANK0_REG

#define DATA5CH0_CR_TXCONTROL0RANK0_REG                                (0x00000F48)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA5CH0_CR_TXCONTROL0RANK1_REG                                (0x00000F4C)
//Duplicate of DATA0CH0_CR_TXCONTROL0RANK0_REG

#define DATA5CH0_CR_DDRCRDATAOFFSETTRAIN_REG                           (0x00000F50)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETTRAIN_REG

#define DATA5CH0_CR_DDRCRDATACONTROL2_REG                              (0x00000F54)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL2_REG

#define DATA5CH0_CR_DCCFSMCONTROL_REG                                  (0x00000F58)
//Duplicate of DATA0CH0_CR_DCCFSMCONTROL_REG

#define DATA5CH0_CR_DCCCALCCONTROL_REG                                 (0x00000F5C)
//Duplicate of DATA0CH0_CR_DCCCALCCONTROL_REG

#define DATA5CH0_CR_DCCLANETARGET_REG                                  (0x00000F60)
//Duplicate of DATA0CH0_CR_DCCLANETARGET_REG

#define DATA5CH0_CR_DDRCRRANKMUXDELAY_REG                              (0x00000F64)
//Duplicate of DATA0CH0_CR_DDRCRRANKMUXDELAY_REG

#define DATA5CH0_CR_DDRCRDLLCONTROL0_REG                               (0x00000F68)
//Duplicate of DATA0CH0_CR_DDRCRDLLCONTROL0_REG

#define DATA5CH0_CR_DCCPILUT0_REG                                      (0x00000F6C)
//Duplicate of DATA0CH0_CR_DCCPILUT0_REG

#define DATA5CH0_CR_REFPICONTROL_REG                                   (0x00000F70)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define DATA5CH0_CR_SPARE_REG                                          (0x00000F74)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DATA5CH0_CR_DDRCRXTALKBIASCONTROL1_REG                         (0x00000F78)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL1_REG

#define DATA5CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG                         (0x00000F7C)
//Duplicate of DATA0CH0_CR_DDRCRDQSWCKDLLCONTROL0_REG

#define DATA5CH0_CR_TERDCCCONTROL0_REG                                 (0x00000F80)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define DATA5CH0_CR_DDRCRXTALKBIASCONTROL2_REG                         (0x00000F84)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL2_REG

#define DATA5CH0_CR_DDRCRXTALKBIASCONTROL3_REG                         (0x00000F88)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL3_REG

#define DATA5CH0_CR_DDRCRXTALKBIASCONTROL4_REG                         (0x00000F8C)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL4_REG

#define DATA5CH0_CR_DDRCRXTALKBIASCONTROL5_REG                         (0x00000F90)
//Duplicate of DATA0CH0_CR_DDRCRXTALKBIASCONTROL5_REG

#define DATA5CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG                      (0x00000F94)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE0TO4_REG

#define DATA5CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG         (0x00000F98)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK0LANE5TO7RANK1LANE0TO1_REG

#define DATA5CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG                      (0x00000F9C)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE2TO6_REG

#define DATA5CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG               (0x00000FA0)
//Duplicate of DATA0CH0_CR_DDRDATADQDCCRANK1LANE7DQSDBIR0R1_REG

#define DATA5CH0_CR_SPARE1_REG                                         (0x00000FA4)
//Duplicate of DATA0CH0_CR_SPARE1_REG

#define DATA5CH0_CR_VCCCLKCOMPRXOFFSET_REG                             (0x00000FB4)
//Duplicate of DATA0CH0_CR_VCCCLKCOMPRXOFFSET_REG

#define DATA5CH0_CR_DDRCRWRRETRAININITRANK1_REG                        (0x00000FCC)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA5CH0_CR_DDRCRWRRETRAININITRANK0_REG                        (0x00000FD0)
//Duplicate of DATA0CH0_CR_DDRCRWRRETRAININITRANK1_REG

#define DATA5CH0_CR_DDRCRRDRETRAININITRANK1_REG                        (0x00000FDC)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA5CH0_CR_DDRCRRDRETRAININITRANK0_REG                        (0x00000FE0)
//Duplicate of DATA0CH0_CR_DDRCRRDRETRAININITRANK1_REG

#define DATA5CH0_CR_DDRCRRXCTLE_REG                                    (0x00000FE4)
//Duplicate of DATA0CH0_CR_DDRCRRXCTLE_REG

#define DATA5CH0_CR_RXCONTROL0RANK0_REG                                (0x00000FE8)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA5CH0_CR_RXCONTROL0RANK1_REG                                (0x00000FEC)
//Duplicate of DATA0CH0_CR_RXCONTROL0RANK0_REG

#define DATA5CH0_CR_DDRDQREPLICACTLE_REG                               (0x00000FF0)
//Duplicate of DATA0CH0_CR_DDRDQREPLICACTLE_REG

#define DATA5CH0_CR_DDRCRDATAOFFSETCOMP_REG                            (0x00000FF8)
//Duplicate of DATA0CH0_CR_DDRCRDATAOFFSETCOMP_REG

#define DATA5CH0_CR_DDRCRDATACONTROL0_REG                              (0x00000FFC)
//Duplicate of DATA0CH0_CR_DDRCRDATACONTROL0_REG



#pragma pack(pop)
#endif /* __MrcRegisterPtl0xxx_h__ */







