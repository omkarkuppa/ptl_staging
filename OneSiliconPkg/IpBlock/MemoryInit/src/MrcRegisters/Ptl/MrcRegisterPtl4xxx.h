#ifndef __MrcRegisterPtl4xxx_h__
#define __MrcRegisterPtl4xxx_h__

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


#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_REG                              (0x00004000)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DEF                              (0x003E0000)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_OFF           ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_MSK           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_FuseDistDoneOvrd_HSH           (0x01004000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_OFF           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_MSK           (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_saxgoffcheckovrd_HSH           (0x01024000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_OFF           ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_MSK           (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrd_HSH           (0x01044000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_OFF        ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_WID        ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_MSK        (0x00000078)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_MAX        (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteOvrdVal_HSH        (0x04064000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_OFF           ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_MSK           (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CompDistDoneOvrd_HSH           (0x010E4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_OFF                 ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_MSK                 (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VttAckOvrd_HSH                 (0x01104000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_OFF              ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_MSK              (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkAckOvrd_HSH              (0x01124000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_OFF              (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_MSK              (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccDDQAckOvrd_HSH              (0x01144000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_OFF                (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_MSK                (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DD2GAckOvrd_HSH                (0x01164000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_OFF                 (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_MSK                 (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOGAckOvrd_HSH                 (0x01184000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_OFF                 (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_MSK                 (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_SAGAckOvrd_HSH                 (0x011A4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_OFF                (14)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_MSK                (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_BlockAbpReq_HSH                (0x011C4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_OFF             (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_MSK             (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_IOSFClkGateDis_HSH             (0x011E4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_OFF              (16)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_MSK              (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_CRIClkGateDis_HSH              (0x01204000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_OFF             (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_MSK             (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_TrainRstOnInit_HSH             (0x01224000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_OFF                (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_MSK                (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_DEF                (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_OtherCRWait_HSH                (0x01244000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_OFF             (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_MSK             (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccClkFFCRWait_HSH             (0x01264000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_OFF               (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_MSK               (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_DEF               (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_VccLVRCRWait_HSH               (0x01284000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_OFF           (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_MSK           (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_DEF           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_InitCompleteWait_HSH           (0x012A4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_OFF                (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_WID                ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_MSK                (0x00C00000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_MAX                (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ForceLPMode_HSH                (0x022C4000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_OFF             (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_MSK             (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_blockcrrestore_HSH             (0x01304000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_OFF             (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_MSK             (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_mrc_workaround_HSH             (0x01324000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_OFF   (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_MSK   (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_ungateinitcompletebypm15_HSH   (0x01344000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_OFF             (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_WID             ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_MSK             (0x78000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_MAX             (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_EnPeriodicComp_HSH             (0x04364000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_OFF          (31)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_MSK          (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD_DisCstateExitComp_HSH          (0x013E4000)

#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_REG                           (0x00004004)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VOLATILE_BITFIELDS_MSK        (0xBFFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_OFF              ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_MSK              (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2PwrGood_HSH              (0x01804004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_OFF               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_MSK               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAPwrGood_HSH               (0x01824004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_OFF      ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_MSK      (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_Ispid_PwrGood_rstb_HSH      (0x01844004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_OFF                ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_MSK                (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_FusePull_HSH                (0x01864004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_OFF              ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_MSK              (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PmaIPReady_HSH              (0x01884004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_OFF              ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_MSK              (0x00000020)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAGpwrGood_HSH              (0x018A4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_OFF             ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_MSK             (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DD2GPwrGood_HSH             (0x018C4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_OFF              ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_MSK              (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_IOGPwrGood_HSH              (0x018E4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_OFF              ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_MSK              (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKPwrGood_HSH              (0x01904004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_OFF              ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_MSK              (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DDQPwrGood_HSH              (0x01924004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_OFF              (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_MSK              (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VTTPwrGood_HSH              (0x01944004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_OFF              (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_MSK              (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_QClkLocked_HSH              (0x01964004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_OFF            (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_MSK            (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AuxClkLocked_HSH            (0x01984004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_OFF              (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_MSK              (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_SAFusePush_HSH              (0x019A4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_OFF                 (14)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_MSK                 (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DLLLock_HSH                 (0x019C4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_OFF                (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_MSK                (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDone_HSH                (0x019E4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_OFF                (16)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_MSK                (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompDist_HSH                (0x01A04004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_OFF              (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_MSK              (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CompUpdate_HSH              (0x01A24004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_OFF               (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_MSK               (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_VttOffset_HSH               (0x01A44004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_OFF                     (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_WID                     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_MSK                     (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_MIN                     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_MAX                     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_DEF                     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_DCC_HSH                     (0x01A64004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_OFF             (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_MSK             (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WeakLockCal_HSH             (0x01A84004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_OFF                 (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_MSK                 (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadCDR_HSH                 (0x01AA4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_OFF             (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_MSK             (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_AllCompDone_HSH             (0x01AC4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_OFF          (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_MSK          (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_ReadReTraining_HSH          (0x01AE4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_OFF         (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_MSK         (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_WriteReTraining_HSH         (0x01B04004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_OFF            (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_WID            ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_MSK            (0x1E000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_MAX            (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_InitComplete_HSH            (0x04B24004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_OFF                   (29)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_WID                   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_MSK                   (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_MAX                   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_PMAck_HSH                   (0x01BA4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_OFF               (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_MSK               (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_TXDLLLock_HSH               (0x013C4004)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_OFF            (31)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_MSK            (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS0_CLKFbPwrGood_HSH            (0x01BE4004)

#define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_REG                          (0x00004008)
#define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_DEF                          (0x00000006)
#define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_VOLATILE_BITFIELDS_MSK       (0x80000030)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_OFF              ( 0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_WID              ( 4)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_MSK              (0x0000000F)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_MIN              (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_MAX              (15) // 0x0000000F
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_DEF              (0x00000006)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_PMMessage_HSH              (0x04004008)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_OFF           ( 4)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_WID           ( 2)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_MSK           (0x00000030)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_MIN           (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_DEF           (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_WorkPointSel_HSH           (0x02884008)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_OFF                 ( 6)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_WID                 (25)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_MSK                 (0x7FFFFFC0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_MIN                 (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_MAX                 (33554431) // 0x01FFFFFF
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_DEF                 (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_Spares_HSH                 (0x190C4008)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_OFF                (31)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_WID                ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_MSK                (0x80000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_MIN                (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_DEF                (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMMESSAGE_RunBusy_HSH                (0x01BE4008)

#define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_REG                     (0x0000400C)
#define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_DEF                     (0x00000000)
#define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_OFF            ( 0)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_WID            ( 8)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_MSK            (0x000000FF)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_MAX            (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_ReadpJ_HSH            (0x0800400C)

  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_OFF           ( 8)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_WID           ( 8)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_MSK           (0x0000FF00)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_MAX           (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_WritepJ_HSH           (0x0810400C)

  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_OFF            (16)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_WID            ( 8)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_MSK            (0x00FF0000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_MAX            (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_IdlemW_HSH            (0x0820400C)

  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_OFF             (24)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_WID             ( 8)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_MSK             (0xFF000000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_MAX             (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_CKEmW_HSH             (0x0830400C)

#define DDRPHY_MISC_SAUG_CR_DDR_PHY_VDD2_POWER_REG                     (0x00004010)
//Duplicate of DDRPHY_MISC_SAUG_CR_DDR_PHY_VDDQ_POWER_REG

#define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_REG                            (0x00004014)
#define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_DEF                            (0x051A550D)
#define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_OFF                    ( 0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_WID                    ( 9)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_MSK                    (0x000001FF)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_MAX                    (511) // 0x000001FF
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_DEF                    (0x0000010D)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF0_HSH                    (0x09004014)

  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_OFF                    ( 9)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_WID                    ( 9)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_MSK                    (0x0003FE00)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_MAX                    (511) // 0x000001FF
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_DEF                    (0x0000012A)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF1_HSH                    (0x09124014)

  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_OFF                    (18)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_WID                    ( 9)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_MSK                    (0x07FC0000)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_MAX                    (511) // 0x000001FF
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_DEF                    (0x00000146)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_VccF2_HSH                    (0x09244014)

  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_OFF           (27)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_MSK           (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_EnOverClocking_HSH           (0x01364014)

  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_OFF                    (28)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_WID                    ( 4)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_MSK                    (0xF0000000)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_MAX                    (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_DEF                    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VCCCLKRANGE_Spare_HSH                    (0x04384014)

#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_REG                           (0x00004018)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VOLATILE_BITFIELDS_MSK        (0xFFFE1FFF)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_OFF            ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_WID            ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_MSK            (0x0000000F)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_MAX            (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentState_HSH            (0x04804018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_OFF               ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_WID               ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_MSK               (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_MAX               (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CurrentWP_HSH               (0x02884018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_OFF        ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_MSK        (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaVoltageSense_HSH        (0x018C4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_OFF            ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_MSK            (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaPLLCheck_HSH            (0x018E4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_OFF            ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_MSK            (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaAuxCheck_HSH            (0x01904018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_OFF            ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_MSK            (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDLLCheck_HSH            (0x01924018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_OFF            (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_MSK            (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaDCCCheck_HSH            (0x01944018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_OFF        (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_MSK        (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaRetrainCheck_HSH        (0x01964018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_OFF    (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_MSK    (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SaveRestoreDoneCheck_HSH    (0x01984018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_OFF           (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_WID           ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_MSK           (0x0001E000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_MAX           (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdVal_HSH           (0x041A4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_OFF            (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_MSK            (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PwrFsmOvrdEn_HSH            (0x01A24018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_OFF               (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_MSK               (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PmAckOvrd_HSH               (0x01A44018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_OFF   (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_MSK   (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_CRI_Fuse_Force_Repull_HSH   (0x01A64018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_OFF           (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_MSK           (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_OvrdCompBlock_HSH           (0x01A84018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_OFF           (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_MSK           (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_ForceFuseDist_HSH           (0x01AA4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_OFF          (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_MSK          (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_FusaTXDLLCheck_HSH          (0x01AC4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_OFF             (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_MSK             (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAXGPwrGood_HSH             (0x01AE4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_OFF            (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_MSK            (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_VssHiPwrGood_HSH            (0x01B04018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_OFF            (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_MSK            (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DistgPwrGood_HSH            (0x01B24018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_OFF          (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_MSK          (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DD2PGEnableAck_HSH          (0x01B44018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_OFF           (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_MSK           (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_SAPGEnableAck_HSH           (0x01B64018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_OFF          (28)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_MSK          (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_IOGPGEnableAck_HSH          (0x01B84018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_OFF         (29)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_MSK         (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_DDQGPGEnableAck_HSH         (0x01BA4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_OFF          (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_MSK          (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_LVRPwrGoodOnSA_HSH          (0x01BC4018)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_OFF              (31)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_MSK              (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS1_PllLockExt_HSH              (0x01BE4018)

#define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_REG                            (0x0000401C)
#define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DEF                            (0xDE000000)
#define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_OFF      ( 0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_WID      ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_MSK      (0x00000001)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_MIN      (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_DEF      (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrd_HSH      (0x0100401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_OFF   ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_WID   ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_MSK   (0x00000002)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_MIN   (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_DEF   (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_WorkPointUpdateOvrdVal_HSH   (0x0102401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_OFF   ( 2)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_WID   ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_MSK   (0x00000004)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_MIN   (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_DEF   (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrd_HSH   (0x0104401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_OFF ( 3)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_WID ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_MSK (0x00000008)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_MIN (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_MAX (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_AuxWorkPointUpdateOvrdVal_HSH (0x0106401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_OFF          ( 4)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_WID          ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_MSK          (0x00000010)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_MIN          (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_DEF          (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_NoWaitCompBlock_HSH          (0x0108401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_OFF          ( 5)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_WID          ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_MSK          (0x00000020)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_MIN          (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_DEF          (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisDvfsExitComp_HSH          (0x010A401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_OFF         ( 6)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_WID         ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_MSK         (0x00000040)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_MIN         (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_DEF         (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_Iosf_ClkGate_Dis_HSH         (0x010C401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_OFF          ( 7)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_WID          ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_MSK          (0x00000080)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_MIN          (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_DEF          (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_ClkGate_Dis_HSH          (0x010E401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_OFF           ( 8)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_WID           ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_MSK           (0x00000100)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_MIN           (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_DEF           (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipDDRIPReady_HSH           (0x0110401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_OFF            ( 9)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_WID            ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_MSK            (0x00000200)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_MIN            (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_DEF            (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SkipSRIPReady_HSH            (0x0112401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_OFF      (10)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_WID      ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_MSK      (0x00000400)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_MIN      (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_DEF      (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_CRI_FusePull_Bypass_HSH      (0x0114401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_OFF      (11)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_WID      ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_MSK      (0x00000800)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_MIN      (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_DEF      (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrd_HSH      (0x0116401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_OFF         (12)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_WID         ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_MSK         (0x00001000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_MIN         (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_FusePullDoneOvrd_HSH         (0x0118401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_OFF        (13)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_WID        ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_MSK        (0x00002000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_MIN        (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_DEF        (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_Val_HSH        (0x011A401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_OFF         (14)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_WID         ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_MSK         (0x00004000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_MIN         (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_DEF         (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_IP_Ready_Ovrd_En_HSH         (0x011C401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_OFF        (15)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_WID        ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_MSK        (0x00008000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_MIN        (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_DEF        (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DelayCompOnPMExit_HSH        (0x011E401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_OFF             (16)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_WID             ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_MSK             (0x00010000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_MIN             (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_DEF             (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DisCompAbort_HSH             (0x0120401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_OFF           (17)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_WID           ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_MSK           (0x00020000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_MIN           (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_DEF           (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_HSH           (0x0122401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_OFF            (18)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_WID            ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_MSK            (0x00040000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_MIN            (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_DEF            (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_HSH            (0x0124401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_OFF            (19)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_WID            ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_MSK            (0x00080000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_MIN            (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_DEF            (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_ignoreack_HSH            (0x0126401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_OFF (20)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_WID ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_MSK (0x00100000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_MIN (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_MAX (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_DEF (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_blockreqen_pkgc_only_HSH (0x0128401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_OFF  (21)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_WID  ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_MSK  (0x00200000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_MIN  (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_DEF  (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_dvp_forcetsen_dvfs_only_HSH  (0x012A401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_OFF  (22)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_WID  ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_MSK  (0x00400000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_MIN  (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_DEF  (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrd_HSH  (0x012C401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_OFF (23)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_WID ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_MSK (0x00800000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_MIN (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_MAX (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DdrSwitch2DeepSleepOvrdVal_HSH (0x012E401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_OFF   (24)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_WID   ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_MSK   (0x01000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_MIN   (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_DEF   (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_SelPllBypassClkOvrdVal_HSH   (0x0130401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_OFF          (25)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_WID          ( 2)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_MSK          (0x06000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_MIN          (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_MAX          (3) // 0x00000003
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_DEF          (0x00000003)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_DvfsRptCntDelay_HSH          (0x0232401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_OFF           (27)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_WID           ( 2)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_MSK           (0x18000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_MIN           (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_DEF           (0x00000003)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_PllRptCntDelay_HSH           (0x0236401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_OFF           (29)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_WID           ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_MSK           (0x20000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_MIN           (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_DEF           (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_EnDynRestoreCR_HSH           (0x013A401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_OFF  (30)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_WID  ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_MSK  (0x40000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_MIN  (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_DEF  (0x00000001)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_upma_sbclk_gate_disable_HSH  (0x013C401C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_OFF  (31)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_WID  ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_MSK  (0x80000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_MIN  (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_DEF  (0x00000001)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMAOVRD_pmsb_sbclk_gate_disable_HSH  (0x013E401C)

#define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_REG                             (0x00004020)
#define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_DEF                             (0x00048540)
#define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_OFF           ( 0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_WID           ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_MSK           (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wpulse_HSH           (0x02004020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_OFF       ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_MSK       (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_disable_HSH       (0x01044020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_OFF               ( 3)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_WID               ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_MSK               (0x00000018)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_MAX               (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_ra_HSH               (0x02064020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_OFF               ( 5)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_WID               ( 3)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_MSK               (0x000000E0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_MAX               (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_DEF               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wa_HSH               (0x030A4020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_OFF              ( 8)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_MSK              (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_DEF              (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_mce_HSH              (0x01104020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_OFF            ( 9)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_MSK            (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_stbyp_HSH            (0x01124020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_OFF             (10)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_WID             ( 4)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_MSK             (0x00003C00)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_MAX             (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_rmce_HSH             (0x04144020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_OFF             (14)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_WID             ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_MSK             (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_MAX             (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_DEF             (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_wmce_HSH             (0x021C4020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_OFF              (16)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_WID              ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_MSK              (0x00030000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_MAX              (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_sramtrim_sbc_HSH              (0x02204020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_OFF                (18)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_MSK                (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_DEF                (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_mce_HSH                (0x01244020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_OFF              (19)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_MSK              (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_stbyp_HSH              (0x01264020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_OFF               (20)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_WID               ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_MSK               (0x00300000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_MAX               (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_rmce_HSH               (0x02284020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_OFF               (22)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_WID               ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_MSK               (0x00C00000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_MAX               (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_wmce_HSH               (0x022C4020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_OFF                (24)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_WID                ( 2)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_MSK                (0x03000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_MAX                (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_rftrim_sbc_HSH                (0x02304020)

  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_OFF                     (26)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_WID                     ( 6)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_MSK                     (0xFC000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_MIN                     (0)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_MAX                     (63) // 0x0000003F
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_DEF                     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_UCSRAMTRIM_Spare_HSH                     (0x06344020)

#define DDRPHY_MISC_SAUG_CR_DDRPERFMON_REG                             (0x00004024)
#define DDRPHY_MISC_SAUG_CR_DDRPERFMON_DEF                             (0x80000000)
#define DDRPHY_MISC_SAUG_CR_DDRPERFMON_VOLATILE_BITFIELDS_MSK          (0x3FFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_OFF                     ( 0)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_WID                     (30)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_MSK                     (0x3FFFFFFF)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_MIN                     (0)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_MAX                     (1073741823) // 0x3FFFFFFF
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_DEF                     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_Count_HSH                     (0x1E804024)

  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_OFF                   (30)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_WID                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_MSK                   (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_MAX                   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_DEF                   (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_DDRPERFMON_CountEn_HSH                   (0x023C4024)

#define DDRPHY_MISC_SAUG_CR_IPVERSION_REG                              (0x00004028)
#define DDRPHY_MISC_SAUG_CR_IPVERSION_DEF                              (0x00003108)
#define DDRPHY_MISC_SAUG_CR_IPVERSION_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_OFF                    ( 0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_WID                    ( 8)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_MSK                    (0x000000FF)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_MAX                    (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_DEF                    (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Version_HSH                    (0x08004028)

  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_OFF                 ( 8)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_WID                 ( 4)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_MSK                 (0x00000F00)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_MAX                 (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_DEF                 (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Derivative_HSH                 (0x04104028)

  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_OFF                    (12)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_WID                    ( 4)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_MSK                    (0x0000F000)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_MAX                    (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_DEF                    (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Segment_HSH                    (0x04184028)

  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_OFF                 (16)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_WID                 ( 8)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_MSK                 (0x00FF0000)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_MAX                 (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_Subversion_HSH                 (0x08204028)

  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_OFF                       (24)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_WID                       ( 8)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_MSK                       (0xFF000000)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_MIN                       (0)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_MAX                       (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_DEF                       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_IPVERSION_rsvd_HSH                       (0x08304028)

#define DDRPHY_MISC_SAUG_CR_RTL_VERSION_REG                            (0x0000402C)
#define DDRPHY_MISC_SAUG_CR_RTL_VERSION_DEF                            (0x00000000)
#define DDRPHY_MISC_SAUG_CR_RTL_VERSION_VOLATILE_BITFIELDS_MSK         (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_OFF                 ( 0)
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_WID                 (32)
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_MSK                 (0xFFFFFFFF)
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_MAX                 (4294967295) // 0xFFFFFFFF
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_RTL_VERSION_IP_Model_HSH                 (0x2000402C)

#define DDRPHY_MISC_SAUG_CR_SR_OVRD_REG                                (0x00004030)
#define DDRPHY_MISC_SAUG_CR_SR_OVRD_DEF                                (0x00000000)
#define DDRPHY_MISC_SAUG_CR_SR_OVRD_VOLATILE_BITFIELDS_MSK             (0x00000006)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_OFF                    ( 0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_WID                    ( 1)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_MSK                    (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_MAX                    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_DEF                    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_SRDebugEn_HSH                    (0x01004030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_OFF               ( 1)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_MSK               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartSave_HSH               (0x01824030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_OFF            ( 2)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_MSK            (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ForceStartRestore_HSH            (0x01844030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_OFF                      ( 3)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_WID                      ( 4)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_MSK                      (0x00000078)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_MAX                      (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP0_HSH                      (0x04064030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_OFF                      ( 7)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_WID                      ( 4)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_MSK                      (0x00000780)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_MAX                      (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP1_HSH                      (0x040E4030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_OFF                      (11)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_WID                      ( 4)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_MSK                      (0x00007800)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_MAX                      (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP2_HSH                      (0x04164030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_OFF                      (15)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_WID                      ( 4)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_MSK                      (0x00078000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_MAX                      (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_ROMGRP3_HSH                      (0x041E4030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_OFF         (19)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_MSK         (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_DisableSRMiscClkGate_HSH         (0x01264030)

  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_OFF                        (20)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_WID                        (12)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_MSK                        (0xFFF00000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_MIN                        (0)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_MAX                        (4095) // 0x00000FFF
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_DEF                        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_SR_OVRD_Spare_HSH                        (0x0C284030)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL0_REG                               (0x00004034)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL0_DEF                               (0x054A952A)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_OFF ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_MSK (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_forceon_deassert_dly_HSH (0x02004034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_OFF ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_MSK (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pwrgood_deassert_dly_HSH (0x02044034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_OFF ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_MSK (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_enableack_assert_dly_HSH (0x02084034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_OFF ( 6)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_MSK (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccSAG_pgctrlfsm_clkgate_disable_HSH (0x010C4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_OFF ( 7)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_MSK (0x00000180)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_forceon_deassert_dly_HSH (0x020E4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_OFF ( 9)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_MSK (0x00000600)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pwrgood_deassert_dly_HSH (0x02124034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_OFF (11)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_MSK (0x00001800)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_enableack_assert_dly_HSH (0x02164034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_OFF (13)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_MSK (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDD2G_pgctrlfsm_clkgate_disable_HSH (0x011A4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_OFF (14)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_MSK (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_forceon_deassert_dly_HSH (0x021C4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_OFF (16)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_MSK (0x00030000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pwrgood_deassert_dly_HSH (0x02204034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_OFF (18)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_MSK (0x000C0000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_enableack_assert_dly_HSH (0x02244034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_OFF (20)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_MSK (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccDDQG_pgctrlfsm_clkgate_disable_HSH (0x01284034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_OFF (21)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_MSK (0x00600000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_forceon_deassert_dly_HSH (0x022A4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_OFF (23)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_MSK (0x01800000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pwrgood_deassert_dly_HSH (0x022E4034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_OFF (25)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_MSK (0x06000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_enableack_assert_dly_HSH (0x02324034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_OFF (27)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_MSK (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_VccIOG_pgctrlfsm_clkgate_disable_HSH (0x01364034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_OFF     (28)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_MSK     (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_SAG_PwrGoodEarlyOvrdVal_HSH     (0x01384034)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_OFF                       (29)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_WID                       ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_MSK                       (0xE0000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_MIN                       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_MAX                       (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_DEF                       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL0_Spare_HSH                       (0x033A4034)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL1_REG                               (0x00004038)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DEF                               (0xAA01F111)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_OFF            ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_WID            ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_MSK            (0x0000000F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_MAX            (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_DEF            (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_prampictl_HSH            (0x04004038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_OFF           ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_WID           ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_MSK           (0x000000F0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_MAX           (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_DEF           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_prampictl_HSH           (0x04084038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_OFF           ( 8)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_WID           ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_MSK           (0x00000F00)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_MAX           (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_DEF           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_prampictl_HSH           (0x04104038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_OFF             (12)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_WID             ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_MSK             (0x0001F000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_MAX             (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_DEF             (0x0000001F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_DD2G_PG_DacCode_HSH             (0x05184038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_OFF                    (17)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_WID                    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_MSK                    (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_MAX                    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_DEF                    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_reserved_HSH                    (0x01224038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_OFF             (18)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_WID             ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_MSK             (0x000C0000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_MAX             (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAG_PG_Bonus_HSH             (0x02244038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_OFF            (20)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_MSK            (0x00300000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDD2G_PG_Bonus_HSH            (0x02284038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_OFF            (22)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_MSK            (0x00C00000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccDDQG_PG_Bonus_HSH            (0x022C4038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_OFF (24)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_MSK (0x03000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_assert_dly_HSH (0x02304038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_OFF  (26)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_MSK  (0x0C000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_DEF  (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_deassert_dly_HSH  (0x02344038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_OFF    (28)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_WID    ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_MSK    (0x30000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_MAX    (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_DEF    (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_reset_assert_dly_HSH    (0x02384038)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_OFF (30)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_MSK (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL1_VccSAXG_clkgateen_deassert_dly_HSH (0x023C4038)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL2_REG                               (0x0000403C)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DEF                               (0xFFE00410)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL2_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_OFF          ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_MSK          (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_sbclk_gate_disable_HSH          (0x0100403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_OFF           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_MSK           (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_qclk_gate_disable_HSH           (0x0102403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_OFF           ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_WID           ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_MSK           (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_RampDnDelay_HSH           (0x0204403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_OFF  ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_MSK  (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_DEF  (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_PwrGoodDeassertDelay_HSH  (0x0208403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_OFF          ( 6)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_WID          ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_MSK          (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_MAX          (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SagPwrGoodEarlyDly_HSH          (0x020C403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_OFF        ( 8)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_WID        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_MSK        (0x00000300)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_MAX        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVREnableDelay_HSH        (0x0210403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_OFF        (10)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_WID        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_MSK        (0x00000C00)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_MAX        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistG_LVRRelockDelay_HSH        (0x0214403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_OFF       (12)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_MSK       (0x00003000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_LVRPwrGoodFSMCntDelay_HSH       (0x0218403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_OFF     (14)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_MSK     (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetDeassertDelay_HSH     (0x021C403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_OFF       (16)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_MSK       (0x00030000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGResetAssertDelay_HSH       (0x0220403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_OFF        (18)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_MSK        (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DistGLVRResetOvrdVal_HSH        (0x0124403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_OFF           (19)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_MSK           (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_start_HSH           (0x0126403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_OFF            (20)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_MSK            (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_phy_content_done_HSH            (0x0128403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_OFF (21)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_MSK (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_rptch_sbclk_spine_clkgate_dis_HSH (0x012A403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_OFF             (22)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_WID             ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_MSK             (0x07C00000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_MAX             (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_DEF             (0x0000001F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_DDQG_PG_DacCode_HSH             (0x052C403C)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_OFF              (27)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_WID              ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_MSK              (0xF8000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_MAX              (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_DEF              (0x0000001F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL2_SAG_PG_DacCode_HSH              (0x0536403C)

#define DDRPHY_MISC_SAUG_CR_VISA_CTRL_REG                              (0x00004040)
#define DDRPHY_MISC_SAUG_CR_VISA_CTRL_DEF                              (0x00000000)
#define DDRPHY_MISC_SAUG_CR_VISA_CTRL_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_OFF           ( 0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_MSK           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_request_HSH           (0x01004040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_OFF     ( 1)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_WID     ( 5)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_MSK     (0x0000003E)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_MAX     (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_offset_HSH     (0x05024040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_OFF            ( 6)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_MSK            (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_local_mode_HSH            (0x010C4040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_OFF         ( 7)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_WID         ( 9)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_MSK         (0x0000FF80)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_MAX         (511) // 0x000001FF
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_write_addr_id_HSH         (0x090E4040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_OFF                   (16)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_WID                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_MSK                   (0x00030000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_MAX                   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_cmd_HSH                   (0x02204040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_OFF             (18)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_WID             ( 2)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_MSK             (0x000C0000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_MAX             (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_visa_vrc_range_HSH             (0x02244040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_OFF         (20)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_MSK         (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sbr_local_vrc_mode_HSH         (0x01284040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_OFF          (21)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_MSK          (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_sr_local_vrc_mode_HSH          (0x012A4040)

  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_OFF                      (22)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_WID                      (10)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_MSK                      (0xFFC00000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_MAX                      (1023) // 0x000003FF
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_CTRL_Spare_HSH                      (0x0A2C4040)

#define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_REG                        (0x00004044)
#define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_DEF                        (0x00000000)
#define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_VOLATILE_BITFIELDS_MSK     (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_OFF ( 0)
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_WID (32)
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_MSK (0xFFFFFFFF)
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_MAX (4294967295) // 0xFFFFFFFF
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_WRITE_DATA_ddrphy_visa_write_data_HSH (0x20004044)

#define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_REG                         (0x00004048)
#define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_DEF                         (0x00000000)
#define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_VOLATILE_BITFIELDS_MSK      (0xFFFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_OFF ( 0)
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_WID (32)
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_MSK (0xFFFFFFFF)
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_MAX (4294967295) // 0xFFFFFFFF
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_VISA_READ_DATA_ddrphy_VISA_READ_DATA_HSH (0x20804048)

#define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_REG                           (0x0000404C)
#define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DEF                           (0x2000001D)
#define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_OFF    ( 0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_MSK    (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_DEF    (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsPrepOverPChannel_HSH    (0x0100404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_OFF    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_MSK    (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChannelAlwaysAccept_HSH    (0x0102404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_OFF ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_MSK (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitComplete4BitResolution_HSH (0x0104404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_OFF ( 3)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_MSK (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DfiInitBlockInitComplete_HSH (0x0106404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_OFF ( 4)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_MSK (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_DvfsOverDfiIntPropDelay_HSH (0x0208404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_OFF     ( 6)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_MSK     (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LpMode4IntPropDelay_HSH     (0x020C404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_OFF          ( 8)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_MSK          (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPhCheck_HSH          (0x0110404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_OFF       ( 9)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_MSK       (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisTrainReset_HSH       (0x0112404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_OFF     (10)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_MSK     (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisPowerDownFsm_HSH     (0x0114404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_OFF          (11)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_MSK          (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrd_HSH          (0x0116404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_OFF       (12)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_MSK       (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPAcceptOvrdVal_HSH       (0x0118404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_OFF            (13)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_MSK            (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrd_HSH            (0x011A404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_OFF         (14)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_MSK         (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PChPDenyOvrdVal_HSH         (0x011C404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_OFF        (15)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_MSK        (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrd_HSH        (0x011E404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_OFF     (16)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_WID     ( 4)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_MSK     (0x000F0000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_MAX     (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmStateOvrdVal_HSH     (0x0420404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_OFF         (20)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_MSK         (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisLvrPwrDn_HSH         (0x0128404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_OFF         (21)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_MSK         (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisSAGPwrDn_HSH         (0x012A404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_OFF        (22)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_MSK        (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisDDQGPwrDn_HSH        (0x012C404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_OFF         (23)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_MSK         (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisIOGPwrDn_HSH         (0x012E404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_OFF     (24)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_MSK     (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4SAGThenLVRPwrUp_HSH     (0x0130404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_OFF       (25)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_MSK       (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisCompUpdate_HSH       (0x0132404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_OFF       (26)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_MSK       (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_PMSAGThenLVRPwrUp_HSH       (0x0134404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_OFF       (27)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_MSK       (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4FsmClkGateDis_HSH       (0x0136404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_OFF         (28)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_MSK         (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4ForcePMExit_HSH         (0x0138404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_OFF   (29)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_MSK   (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_DEF   (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisableIOSFClkReq_HSH   (0x013A404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_OFF    (30)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_MSK    (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_LPM4DisVssHILvrPwrDn_HSH    (0x013C404C)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_OFF                   (31)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_WID                   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_MSK                   (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_MAX                   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_CTRL_Spare_HSH                   (0x013E404C)

#define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_REG                           (0x00004050)
#define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_DEF                           (0x800F0003)
#define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_VOLATILE_BITFIELDS_MSK        (0x0FC00000)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_OFF             ( 0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_MSK             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_crrst_HSH             (0x01004050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_OFF               ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_MSK               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_DEF               (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_rst_HSH               (0x01024050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_OFF        ( 2)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_MSK        (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_forceclken_HSH        (0x01044050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_OFF         ( 3)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_MSK         (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_pmstc_clkgateen_HSH         (0x01064050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_OFF              ( 4)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_MSK              (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_ucss_reset_HSH              (0x01084050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_OFF                    ( 5)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_WID                    (11)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_MSK                    (0x0000FFE0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_MAX                    (2047) // 0x000007FF
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_DEF                    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_rsvd_HSH                    (0x0B0A4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_OFF        (16)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_MSK        (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_ne_sram_pg_en_HSH        (0x01204050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_OFF        (17)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_MSK        (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_sram_pg_en_HSH        (0x01224050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_OFF        (18)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_MSK        (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_xt_fe_side_clken_HSH        (0x01244050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_OFF             (19)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_MSK             (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_crirst_HSH             (0x01264050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_OFF      (20)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_MSK      (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_div2ordiv4b_HSH      (0x01284050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_OFF              (21)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_MSK              (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dunclk_rst_HSH              (0x012A4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_OFF      (22)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_MSK      (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_write_retrain_done_HSH      (0x01AC4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_OFF       (23)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_MSK       (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_read_retrain_done_HSH       (0x01AE4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_OFF        (24)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_MSK        (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_cmd_HSH        (0x01B04050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_OFF        (25)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_MSK        (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_mrr_HSH        (0x01B24050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_OFF        (26)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_MSK        (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_rdr_HSH        (0x01B44050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_OFF        (27)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_MSK        (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_initcomplete_wrr_HSH        (0x01B64050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_OFF              (28)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_MSK              (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_clken_HSH              (0x01384050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_OFF             (29)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_MSK             (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_dfi_mux_sel_HSH             (0x013A4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_OFF                 (30)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_MSK                 (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_en_HSH                 (0x013C4050)

  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_OFF                (31)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_MSK                (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_DEF                (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTUSS_CTRL0_mptu_rst_HSH                (0x013E4050)

#define DDRPHY_MISC_SAUG_CR_PMCTRL_REG                                 (0x00004054)
#define DDRPHY_MISC_SAUG_CR_PMCTRL_DEF                                 (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PMCTRL_VOLATILE_BITFIELDS_MSK              (0xFFFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_OFF                   ( 0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_WID                   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_MSK                   (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_MAX                   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_reset_abort_HSH                   (0x01804054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_OFF               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_WID               (11)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_MSK               (0x00000FFE)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_MAX               (2047) // 0x000007FF
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_DistGLvrCtrlFsm_HSH               (0x0B824054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_OFF                  (12)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_WID                  ( 4)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_MSK                  (0x0000F000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_MIN                  (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_MAX                  (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_DEF                  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_LPM4FsmState_HSH                  (0x04984054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_OFF                (16)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_WID                ( 4)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_MSK                (0x000F0000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_MAX                (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAXGFsmState_HSH                (0x04A04054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_OFF                 (20)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_WID                 ( 3)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_MSK                 (0x00700000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_MAX                 (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgSAGFsmState_HSH                 (0x03A84054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_OFF                (23)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_WID                ( 3)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_MSK                (0x03800000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_MAX                (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDD2GFsmState_HSH                (0x03AE4054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_OFF                 (26)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_WID                 ( 3)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_MSK                 (0x1C000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_MAX                 (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgIOGFsmState_HSH                 (0x03B44054)

  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_OFF                (29)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_WID                ( 3)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_MSK                (0xE0000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_MAX                (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PMCTRL_PgDDQGFsmState_HSH                (0x03BA4054)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL3_REG                               (0x00004058)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DEF                               (0x0806A045)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VOLATILE_BITFIELDS_MSK            (0x04000400)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_OFF       ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_MSK       (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_DEF       (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRDisableDelay_HSH       (0x02004058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_OFF  ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_MSK  (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_DEF  (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_PwrGoodDeassertDelay_HSH  (0x02044058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_OFF        ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_WID        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_MSK        (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_MAX        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVREnableDelay_HSH        (0x02084058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_OFF        ( 6)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_WID        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_MSK        (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_MAX        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_DistG_LVRRelockDelay_HSH        (0x020C4058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_OFF   ( 8)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_MSK   (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAG_PwrgoodEarly_override_HSH   (0x01104058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_OFF    ( 9)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_MSK    (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_PG_PwrgoodEarly_override_HSH    (0x01124058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_OFF                 (10)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_WID                 ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_MSK                 (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_DEF                 (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Enable_HSH                 (0x01944058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_OFF (11)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_MSK (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pgctrlfsm_clkgate_disable_HSH (0x01164058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_OFF (12)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_MSK (0x00003000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_enableack_assert_dly_HSH (0x02184058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_OFF (14)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_MSK (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_pwrgood_deassert_dly_HSH (0x021C4058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_OFF (16)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_MSK (0x00030000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_DEF (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_forceon_deassert_dly_HSH (0x02204058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_OFF           (18)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_WID           ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_MSK           (0x003C0000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_MAX           (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_DEF           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_prampictl_HSH           (0x04244058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_OFF            (22)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_MSK            (0x00C00000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_PG_Bonus_HSH            (0x022C4058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_OFF          (24)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_MSK          (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_BisrBypass_HSH          (0x01304058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_OFF (25)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_MSK (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SkipSAXGOffChkDuringRstAbrt_HSH (0x01324058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_OFF                  (26)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_WID                  ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_MSK                  (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_MIN                  (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_MAX                  (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_DEF                  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_SAXG_Ready_HSH                  (0x01B44058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_OFF   (27)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_MSK   (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_DEF   (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_VccSAXG_DisDuringRstAbort_HSH   (0x01364058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_OFF     (28)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_MSK     (0x30000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_BISRHandshakeStartDelay_HSH     (0x02384058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_OFF          (30)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_MSK          (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_bypass_srsram_bisr_HSH          (0x013C4058)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_OFF       (31)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_MSK       (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL3_saxg_reset_abort_cbit_HSH       (0x013E4058)

#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_REG                             (0x0000405C)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DEF                             (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_OFF          ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_MSK          (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGOvrdModeEn_HSH          (0x0100405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_OFF          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_MSK          (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnableOvrd_HSH          (0x0102405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_OFF        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_MSK        (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PowerUpOvrdVal_HSH        (0x0104405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_OFF        ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_MSK        (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_ForceOnOvrdVal_HSH        (0x0106405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_OFF        ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_MSK        (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PwrGoodOvrdVal_HSH        (0x0108405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_OFF         ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_MSK         (0x00000020)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGOvrdModeEn_HSH         (0x010A405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_OFF         ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_MSK         (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnableOvrd_HSH         (0x010C405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_OFF       ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_MSK       (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PowerUpOvrdVal_HSH       (0x010E405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_OFF       ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_MSK       (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ForceOnOvrdVal_HSH       (0x0110405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_OFF       ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_MSK       (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PwrGoodOvrdVal_HSH       (0x0112405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_OFF         (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_MSK         (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ResetOvrdVal_HSH         (0x0114405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_OFF         (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_MSK         (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_ClkEnOvrdVal_HSH         (0x0116405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_OFF         (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_MSK         (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGOvrdModeEn_HSH         (0x0118405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_OFF         (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_MSK         (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnableOvrd_HSH         (0x011A405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_OFF       (14)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_MSK       (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PowerUpOvrdVal_HSH       (0x011C405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_OFF       (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_MSK       (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_ForceOnOvrdVal_HSH       (0x011E405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_OFF       (16)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_MSK       (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PwrGoodOvrdVal_HSH       (0x0120405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_OFF          (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_MSK          (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGOvrdModeEn_HSH          (0x0122405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_OFF          (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_MSK          (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnableOvrd_HSH          (0x0124405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_OFF        (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_MSK        (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PowerUpOvrdVal_HSH        (0x0126405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_OFF        (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_MSK        (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_ForceOnOvrdVal_HSH        (0x0128405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_OFF        (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_MSK        (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PwrGoodOvrdVal_HSH        (0x012A405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_OFF         (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_MSK         (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGOvrdModeEn_HSH         (0x012C405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_OFF         (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_MSK         (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnableOvrd_HSH         (0x012E405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_OFF       (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_MSK       (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PowerUpOvrdVal_HSH       (0x0130405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_OFF       (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_MSK       (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_ForceOnOvrdVal_HSH       (0x0132405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_OFF       (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_MSK       (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PwrGoodOvrdVal_HSH       (0x0134405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_OFF        (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_MSK        (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAG_PGEnAckOvrdVal_HSH        (0x0136405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_OFF       (28)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_MSK       (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_SAXG_PGEnAckOvrdVal_HSH       (0x0138405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_OFF       (29)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_MSK       (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DD2G_PGEnAckOvrdVal_HSH       (0x013A405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_OFF        (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_MSK        (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_IOG_PGEnAckOvrdVal_HSH        (0x013C405C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_OFF       (31)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_MSK       (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD1_DDQG_PGEnAckOvrdVal_HSH       (0x013E405C)

#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_REG                             (0x00004060)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DEF                             (0x08000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_OFF             ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_MSK             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROvrdModeEn_HSH             (0x01004060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_OFF          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_MSK          (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVREnableOvrdVal_HSH          (0x01024060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_OFF         ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_MSK         (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVRPwrGoodOvrdVal_HSH         (0x01044060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_OFF             ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_MSK             (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_LVROffOvrdVal_HSH             (0x01064060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_OFF       ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_MSK       (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROvrdModeEn_HSH       (0x01084060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_OFF       ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_MSK       (0x00000020)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVREnableOvrd_HSH       (0x010A4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_OFF         ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_MSK         (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_PwrGoodOvrd_HSH         (0x010C4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_OFF          ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_MSK          (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_DistG_LVROffOvrd_HSH          (0x010E4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_OFF       ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_MSK       (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_VccCLKFBPwrGoodOvrd_HSH       (0x01104060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_OFF         ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_MSK         (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrd_HSH         (0x01124060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_OFF      (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_MSK      (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCtrlOvrdVal_HSH      (0x01144060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_OFF    (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_MSK    (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrd_HSH    (0x01164060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_OFF (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_MSK (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCrRestoreOvrdVal_HSH (0x01184060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_OFF    (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_MSK    (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrd_HSH    (0x011A4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_OFF (14)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_MSK (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_clken_qclk_ovrdval_HSH (0x011C4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_OFF         (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_MSK         (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrd_HSH         (0x011E4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_OFF      (16)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_MSK      (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_reset_ovrdval_HSH      (0x01204060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_OFF      (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_MSK      (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrd_HSH      (0x01224060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_OFF   (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_MSK   (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_cr_reset_ovrdval_HSH   (0x01244060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_OFF         (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_MSK         (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrd_HSH         (0x01264060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_OFF      (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_MSK      (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_pllen_ovrdval_HSH      (0x01284060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_OFF       (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_MSK       (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrd_HSH       (0x012A4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_OFF    (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_MSK    (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_plllock_ovrdval_HSH    (0x012C4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_OFF              (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_MSK              (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOffOvrd_HSH              (0x012E4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_OFF              (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_WID              ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_MSK              (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_DEF              (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOnOvrdVal_HSH              (0x01304060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_OFF             (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_MSK             (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllOffOvrdVal_HSH             (0x01324060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_OFF   (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_MSK   (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_PllUnLockRstAllCompDone_HSH   (0x01344060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_OFF (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_MSK (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_ddrphy_sr_safe_mode_rst_b_HSH (0x01364060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_OFF   (28)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_MSK   (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrd_HSH   (0x01384060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_OFF (29)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_MSK (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionSRSbClkDisOvrdVal_HSH (0x013A4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_OFF     (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_MSK     (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrd_HSH     (0x013C4060)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_OFF  (31)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_WID  ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_MSK  (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD2_RetentionCriClkEnOvrdVal_HSH  (0x013E4060)

#define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_REG                          (0x00004064)
#define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_DEF                          (0xC81045E2)
#define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_OFF         ( 0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_MSK         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_crforcelpmode4_HSH         (0x01004064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_OFF             ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_MSK             (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_plloffmode_HSH             (0x01024064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_OFF      ( 2)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_MSK      (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_dislpmode4clkgate_HSH      (0x01044064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_OFF       ( 3)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_WID       (10)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_MSK       (0x00001FF8)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_MAX       (1023) // 0x000003FF
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_DEF       (0x000000BC)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_lpmode4exittimer_HSH       (0x0A064064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_OFF           (13)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_WID           ( 6)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_MSK           (0x0007E000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_MAX           (63) // 0x0000003F
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_DEF           (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrx_HSH           (0x061A4064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_OFF       (19)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_WID       ( 6)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_MSK       (0x01F80000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_MAX       (63) // 0x0000003F
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_DEF       (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_twaitsrxdone_HSH       (0x06264064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_OFF     (25)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_WID     ( 4)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_MSK     (0x1E000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_MAX     (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_DEF     (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_scr_tlpmode4endone_HSH     (0x04324064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_OFF         (29)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_MSK         (0x20000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_enable_lpmode4_HSH         (0x013A4064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_OFF        (30)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_MSK        (0x40000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifypmlp4fsm_HSH        (0x013C4064)

  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_OFF       (31)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_MSK       (0x80000000)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_DEF       (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LPMODE4_CTRL0_qualifylpmode4en_HSH       (0x013E4064)

#define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_REG                           (0x00004068)
#define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DEF                           (0x10000000)
#define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_VOLATILE_BITFIELDS_MSK        (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_OFF          ( 0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_MSK          (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrd_HSH          (0x01004068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_OFF       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_WID       ( 3)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_MSK       (0x0000000E)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_MAX       (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPmStateOvrdVal_HSH       (0x03024068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_OFF          ( 4)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_MSK          (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrd_HSH          (0x01084068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_OFF       ( 5)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_WID       ( 3)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_MSK       (0x000000E0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_MAX       (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChIfStateOvrdVal_HSH       (0x030A4068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_OFF    ( 8)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_MSK    (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrd_HSH    (0x01104068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_OFF ( 9)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_WID ( 3)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_MSK (0x00000E00)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_MAX (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_DvfsOverDfiStateOvrdVal_HSH (0x03124068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_OFF          (12)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_MSK          (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrd_HSH          (0x01184068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_OFF       (13)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_WID       ( 8)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_MSK       (0x001FE000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_MAX       (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_PChPActiveOvrdVal_HSH       (0x081A4068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_OFF (21)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_MSK (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrd_HSH (0x012A4068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_OFF (22)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_MSK (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_ddrphy_cr_reset_saug_qclk_ovrdval_HSH (0x012C4068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_OFF      (23)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_MSK      (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_RetentionClkEnOvrd_HSH      (0x012E4068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_OFF     (24)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_MSK     (0x03000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteExtOvrd_HSH     (0x02304068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_OFF (26)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_MSK (0x0C000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntMcmiscsOvrd_HSH (0x02344068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_OFF (28)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_MSK (0x30000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_InitCompleteIntRptchOvrd_HSH (0x02384068)

  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_OFF                   (30)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_WID                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_MSK                   (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_MAX                   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PCH_DFI_OVRD_Spare_HSH                   (0x023C4068)

#define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_REG                              (0x0000406C)
#define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_DEF                              (0x00000001)
#define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_OFF                 ( 0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_WID                 ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_MSK                 (0x00000001)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_MIN                 (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_MAX                 (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_DEF                 (0x00000001)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_PChannelEn_HSH                 (0x0100406C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_OFF              ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_WID              ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_MSK              (0x00000002)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_MIN              (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_MAX              (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_DEF              (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRestoreCR_HSH              (0x0102406C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_OFF            ( 2)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_WID            ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_MSK            (0x00000004)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_MIN            (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_DEF            (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipRetentionCR_HSH            (0x0104406C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_OFF                  ( 3)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_WID                  ( 1)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_MSK                  (0x00000008)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_MIN                  (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_MAX                  (1) // 0x00000001
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_DEF                  (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_SkipPMAck_HSH                  (0x0106406C)

  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_OFF                     ( 4)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_WID                     (28)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_MSK                     (0xFFFFFFF0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_MIN                     (0)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_MAX                     (268435455) // 0x0FFFFFFF
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_DEF                     (0x00000000)
  #define DDRPHY_MISC_PMA_SAUG_CR_PMCFG_Spares_HSH                     (0x1C08406C)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL4_REG                               (0x00004070)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DEF                               (0x02A41205)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_OFF           ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_WID           ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_MSK           (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_DEF           (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_RampUpDelay_HSH           (0x02004070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_OFF       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_MSK       (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_DEF       (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_DistG_LVRDisableDelay_HSH       (0x02044070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_OFF            ( 4)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_MSK            (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPGRampRate_HSH            (0x02084070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_OFF                ( 6)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_WID                ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_MSK                (0x000001C0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_MAX                (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampDn_HSH                (0x030C4070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_OFF          ( 9)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_WID          ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_MSK          (0x00000600)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_MAX          (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_DEF          (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGPwrGoodDelay_HSH          (0x02124070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_OFF                (11)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_WID                ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_MSK                (0x00001800)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_MAX                (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_DEF                (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccIOGRampUp_HSH                (0x02164070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_OFF           (13)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_WID           ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_MSK           (0x00006000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_MAX           (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPGRampRate_HSH           (0x021A4070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_OFF               (15)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_WID               ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_MSK               (0x00038000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_MAX               (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampDn_HSH               (0x031E4070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_OFF         (18)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_MSK         (0x000C0000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_DEF         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGPwrGoodDelay_HSH         (0x02244070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_OFF               (20)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_WID               ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_MSK               (0x00300000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_MAX               (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_DEF               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccDDQGRampUp_HSH               (0x02284070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_OFF               (22)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_WID               ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_MSK               (0x01C00000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_MAX               (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_DEF               (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampUp_HSH               (0x032C4070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_OFF         (25)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_MSK         (0x06000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_DEF         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGPwrGoodDelay_HSH         (0x02324070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_OFF               (27)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_WID               ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_MSK               (0x38000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_MAX               (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_VccSAXGRampDn_HSH               (0x03364070)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_OFF             (30)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_WID             ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_MSK             (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_MAX             (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL4_PGRampRate_SAXG_HSH             (0x023C4070)

#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_REG                           (0x00004074)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_VOLATILE_BITFIELDS_MSK        (0x3FFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_OFF           ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_WID           ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_MSK           (0x00000007)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_MAX           (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChIfState_HSH           (0x03804074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_OFF           ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_WID           ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_MSK           (0x00000038)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_MAX           (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DdrPChPmState_HSH           (0x03864074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_OFF     ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_WID     ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_MSK     (0x000001C0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_MAX     (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsOverDfiFsmState_HSH     (0x038C4074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_OFF        ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_WID        ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_MSK        (0x00000E00)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_MAX        (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_CompCtrlFsmState_HSH        (0x03924074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_OFF          (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_WID          ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_MSK          (0x00007000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_MAX          (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_AuxPllFsmState_HSH          (0x03984074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_OFF    (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_WID    ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_MSK    (0x00038000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_MAX    (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_InitCompleteFsmState_HSH    (0x039E4074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_OFF         (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_WID         ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_MSK         (0x007C0000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_MAX         (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_PllCtrlFsmState_HSH         (0x05A44074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_OFF            (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_WID            ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_MSK            (0x07800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_MAX            (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_DvfsFsmState_HSH            (0x04AE4074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_OFF      (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_WID      ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_MSK      (0x38000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_MAX      (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_LvrPwrGoodFsmState_HSH      (0x03B64074)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_OFF                   (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_WID                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_MSK                   (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_MAX                   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS2_Spare_HSH                   (0x023C4074)

#define DDRPHY_MISC_SAUG_CR_PG_REG                                     (0x00004078)
#define DDRPHY_MISC_SAUG_CR_PG_DEF                                     (0x00082869)
#define DDRPHY_MISC_SAUG_CR_PG_VOLATILE_BITFIELDS_MSK                  (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_OFF                         ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_WID                         ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_MSK                         (0x00000007)
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_MIN                         (0)
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_MAX                         (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_DEF                         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_LVRRampDn_HSH                         (0x03004078)

  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_OFF                       ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_WID                       ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_MSK                       (0x00000018)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_MIN                       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_MAX                       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_DEF                       (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampUp_HSH                       (0x02064078)

  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_OFF                 ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_WID                 ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_MSK                 (0x00000060)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_MIN                 (0)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_MAX                 (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_DEF                 (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GPwrGoodDelay_HSH                 (0x020A4078)

  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_OFF                       ( 7)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_WID                       ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_MSK                       (0x00000380)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_MIN                       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_MAX                       (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_DEF                       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_Vdd2GRampDn_HSH                       (0x030E4078)

  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_OFF                      (10)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_WID                      ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_MSK                      (0x00001C00)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_MAX                      (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_DEF                      (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampUp_HSH                      (0x03144078)

  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_OFF                (13)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_WID                ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_MSK                (0x00006000)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_MAX                (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_DEF                (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGPwrGoodDelay_HSH                (0x021A4078)

  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_OFF                      (15)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_WID                      ( 3)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_MSK                      (0x00038000)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_MAX                      (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_VccSAGRampDn_HSH                      (0x031E4078)

  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_OFF                        (18)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_WID                        ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_MSK                        (0x000C0000)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_MIN                        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_MAX                        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_DEF                        (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_HSH                        (0x02244078)

  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_OFF                  (20)
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_WID                  ( 1)
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_MSK                  (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_MIN                  (0)
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_MAX                  (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_DEF                  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_ReportFuSaErrors_HSH                  (0x01284078)

  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_OFF                    (21)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_WID                    ( 2)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_MSK                    (0x00600000)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_MIN                    (0)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_MAX                    (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_DEF                    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_PGRampRate_SAG_HSH                    (0x022A4078)

  #define DDRPHY_MISC_SAUG_CR_PG_Spare_OFF                             (23)
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_WID                             ( 9)
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_MSK                             (0xFF800000)
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_MIN                             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_MAX                             (511) // 0x000001FF
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_DEF                             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_Spare_HSH                             (0x092E4078)

#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_REG                             (0x0000407C)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DEF                             (0x80812000)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_OFF       ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_MSK       (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteSetOvrd_HSH       (0x0100407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_OFF        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_MSK        (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PhCheckFsmDoneOvrd_HSH        (0x0102407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_OFF                ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_MSK                (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAckOvrd_HSH                (0x0104407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_OFF               ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_WID               ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_MSK               (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_MAX               (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_DEF               (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllLockOvrd_HSH               (0x0106407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_OFF       ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_MSK       (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DllCompCompleteOvrd_HSH       (0x0108407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_OFF    ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_MSK    (0x00000020)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DqsDlyCompCompleteOvrd_HSH    (0x010A407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_OFF        ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_MSK        (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TrainResetDoneOvrd_HSH        (0x010C407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_OFF             ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_MSK             (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_TxDllLockOvrd_HSH             (0x010E407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_OFF  ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_WID  ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_MSK  (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrd_HSH  (0x0110407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_OFF ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_WID ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_MSK (0x00000E00)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_MAX (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_InitCompleteFsmStateOvrdVal_HSH (0x0312407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_OFF              (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_WID              ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_MSK              (0x00007000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_MAX              (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_DEF              (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_SkewCntDelay_HSH              (0x0318407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_OFF              (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_WID              ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_MSK              (0x00038000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_MIN              (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_MAX              (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_DEF              (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PropCntDelay_HSH              (0x031E407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_OFF      (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_WID      ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_MSK      (0x001C0000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_MAX      (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_CriClkEnPropCntDelay_HSH      (0x0324407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_OFF  (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_MSK  (0x00600000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_RetentionCtrlRptCntDelay_HSH  (0x022A407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_OFF             (23)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_MSK             (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_DEF             (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOpt_HSH             (0x012E407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_OFF      (24)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_MSK      (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_UseLPM4PllOptForPkgc_HSH      (0x0130407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_OFF                (25)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_MSK                (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_pgddr5mode_HSH                (0x0132407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_OFF (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_MSK (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdEn_HSH (0x0134407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_OFF (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_MSK (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_IOGLVRWorkPointUpdateOvrdVal_HSH (0x0136407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_OFF           (28)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_MSK           (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_DccAck_fsm_Ovrd_HSH           (0x0138407C)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_OFF               (29)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_WID               ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_MSK               (0xE0000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_MIN               (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_MAX               (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_DEF               (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD3_PerfCountEn_HSH               (0x033A407C)

#define DDRPHY_MISC_SAUG_CR_PG_CTRL5_REG                               (0x00004080)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL5_DEF                               (0x00001F5F)
#define DDRPHY_MISC_SAUG_CR_PG_CTRL5_VOLATILE_BITFIELDS_MSK            (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_OFF             ( 0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_WID             ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_MSK             (0x0000001F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_MAX             (31) // 0x0000001F
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_DEF             (0x0000001F)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_SAXG_PG_DacCode_HSH             (0x05004080)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_OFF        ( 5)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_WID        ( 8)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_MSK        (0x00001FE0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_MAX        (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_DEF        (0x000000FA)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_PeriodicCompTimerCnt_HSH        (0x080A4080)

  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_OFF                       (13)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_WID                       (19)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_MSK                       (0xFFFFE000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_MIN                       (0)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_MAX                       (524287) // 0x0007FFFF
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_DEF                       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PG_CTRL5_Spare_HSH                       (0x131A4080)

#define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_REG                              (0x00004084)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_DEF                              (0x00000045)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_OFF        ( 0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_WID        ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_MSK        (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_MAX        (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_DEF        (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRDisableDelay_HSH        (0x02004084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_OFF   ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_WID   ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_MSK   (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_MAX   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_DEF   (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodDeassertDelay_HSH   (0x02044084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_OFF         ( 4)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_MSK         (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableDelay_HSH         (0x02084084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_OFF         ( 6)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_MSK         (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_DEF         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDelay_HSH         (0x020C4084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_OFF            ( 8)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_MSK            (0x00000300)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampUpDelay_HSH            (0x02104084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_OFF            (10)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_WID            ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_MSK            (0x00000C00)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_MAX            (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_RampDnDelay_HSH            (0x02144084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_OFF     (12)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_MSK     (0x00003000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetDeassertDelay_HSH     (0x02184084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_OFF       (14)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_MSK       (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_ResetAssertDelay_HSH       (0x021C4084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_OFF        (16)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_MSK        (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRResetOvrdVal_HSH        (0x01204084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_OFF          (17)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_MSK          (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROvrdModeEn_HSH          (0x01224084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_OFF          (18)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_MSK          (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVREnableOvrd_HSH          (0x01244084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_OFF            (19)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_WID            ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_MSK            (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_MAX            (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_PwrGoodOvrd_HSH            (0x01264084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_OFF             (20)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_WID             ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_MSK             (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_MIN             (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_MAX             (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_DEF             (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVROffOvrd_HSH             (0x01284084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_OFF          (21)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_MSK          (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrd_HSH          (0x012A4084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_OFF       (22)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_MSK       (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_DisableGV_Switch_HSH       (0x012C4084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_OFF (23)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_MSK (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRPwrGoodOnWhileRelockOvrd_HSH (0x012E4084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_OFF        (24)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_MSK        (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRVrefEnOvrdEn_HSH        (0x01304084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_OFF        (25)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_MSK        (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrd_HSH        (0x01324084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_OFF      (26)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_MSK      (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRWPUpdateOvrdEn_HSH      (0x01344084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_OFF      (27)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_MSK      (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_LVRRelockDoneOvrd_HSH      (0x01364084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_OFF       (28)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_MSK       (0x10000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_IOG_GV_LVROvrdModeEn_HSH       (0x01384084)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_OFF                      (29)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_WID                      ( 3)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_MSK                      (0xE0000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_MAX                      (7) // 0x00000007
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL0_Spare_HSH                      (0x033A4084)

#define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_REG                              (0x00004088)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_DEF                              (0x00000045)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_OFF     ( 0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_WID     ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_MSK     (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_MAX     (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_DEF     (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRDisableDelay_HSH     (0x02004088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_OFF ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_MSK (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_DEF (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodDeassertDelay_HSH (0x02044088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_OFF      ( 4)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_WID      ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_MSK      (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_MAX      (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableDelay_HSH      (0x02084088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_OFF      ( 6)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_WID      ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_MSK      (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_MAX      (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_DEF      (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDelay_HSH      (0x020C4088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_OFF         ( 8)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_MSK         (0x00000300)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampUpDelay_HSH         (0x02104088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_OFF         (10)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_WID         ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_MSK         (0x00000C00)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_MAX         (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_RampDnDelay_HSH         (0x02144088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_OFF  (12)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_MSK  (0x00003000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetDeassertDelay_HSH  (0x02184088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_OFF    (14)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_WID    ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_MSK    (0x0000C000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_MAX    (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_ResetAssertDelay_HSH    (0x021C4088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_OFF     (16)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_MSK     (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRResetOvrdVal_HSH     (0x01204088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_OFF       (17)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_MSK       (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROvrdModeEn_HSH       (0x01224088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_OFF       (18)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_MSK       (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVREnableOvrd_HSH       (0x01244088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_OFF         (19)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_MSK         (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_PwrGoodOvrd_HSH         (0x01264088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_OFF          (20)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_MSK          (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVROffOvrd_HSH          (0x01284088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_OFF       (21)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_MSK       (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrd_HSH       (0x012A4088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_OFF    (22)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_MSK    (0x00400000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_DisableGV_Switch_HSH    (0x012C4088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_OFF (23)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_MSK (0x00800000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRPwrGoodOnWhileRelockOvrd_HSH (0x012E4088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_OFF     (24)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_MSK     (0x01000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRVrefEnOvrdEn_HSH     (0x01304088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_OFF     (25)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_MSK     (0x02000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrd_HSH     (0x01324088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_OFF   (26)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_MSK   (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRWPUpdateOvrdEn_HSH   (0x01344088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_OFF   (27)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_MSK   (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_VccClk_LVRRelockDoneOvrd_HSH   (0x01364088)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_OFF                      (28)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_WID                      ( 4)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_MSK                      (0xF0000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_MAX                      (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL1_Spare_HSH                      (0x04384088)

#define MCMISCSSAUG_SB_DEBUG_REG                                       (0x0000408C)
#define MCMISCSSAUG_SB_DEBUG_DEF                                       (0x00000000)
#define MCMISCSSAUG_SB_DEBUG_VOLATILE_BITFIELDS_MSK                    (0x00000000)

  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_OFF                       ( 0)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_WID                       ( 1)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_MSK                       (0x00000001)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_MIN                       (0)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_MAX                       (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_DEF                       (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeen_HSH                       (0x0100408C)

  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_OFF                       ( 1)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_WID                       ( 1)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_MSK                       (0x00000002)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_MIN                       (0)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_MAX                       (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_DEF                       (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeen_HSH                       (0x0102408C)

  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_OFF                       ( 2)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_WID                       ( 3)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_MSK                       (0x0000001C)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_MIN                       (0)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_MAX                       (7) // 0x00000007
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_DEF                       (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxsel_HSH                       (0x0304408C)

  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_OFF                  ( 5)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_WID                  ( 1)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_MSK                  (0x00000020)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_MIN                  (0)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_MAX                  (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_DEF                  (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_anaeleprobeenshare_HSH                  (0x010A408C)

  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_OFF                  ( 6)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_WID                  ( 1)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_MSK                  (0x00000040)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_MIN                  (0)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_MAX                  (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_DEF                  (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_digeleprobeenshare_HSH                  (0x010C408C)

  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_OFF                  ( 7)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_WID                  ( 3)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_MSK                  (0x00000380)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_MIN                  (0)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_MAX                  (7) // 0x00000007
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_DEF                  (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_i_probemuxselShare_HSH                  (0x030E408C)

  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_OFF                  (10)
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_WID                  ( 1)
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_MSK                  (0x00000400)
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_MIN                  (0)
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_MAX                  (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_DEF                  (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_Ddr_reg_clken_ovrd_HSH                  (0x0114408C)

  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_OFF              (11)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_WID              ( 7)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_MSK              (0x0003F800)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_MIN              (0)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_MAX              (127) // 0x0000007F
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_DEF              (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port0_HSH              (0x0716408C)

  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_OFF              (18)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_WID              ( 7)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_MSK              (0x01FC0000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_MIN              (0)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_MAX              (127) // 0x0000007F
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_DEF              (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmuxsel_port1_HSH              (0x0724408C)

  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_OFF         (25)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_WID         ( 3)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_MSK         (0x0E000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_MIN         (0)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_MAX         (7) // 0x00000007
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_DEF         (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port0_HSH         (0x0332408C)

  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_OFF         (28)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_WID         ( 3)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_MSK         (0x70000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_MIN         (0)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_MAX         (7) // 0x00000007
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_DEF         (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_directviewmergemuxsel_port1_HSH         (0x0338408C)

  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_OFF                    (31)
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_WID                    ( 1)
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_MSK                    (0x80000000)
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_MIN                    (0)
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_MAX                    (1) // 0x00000001
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_DEF                    (0x00000000)
  #define MCMISCSSAUG_SB_DEBUG_digviewdirectsel_HSH                    (0x013E408C)

#define MCMISCSSAUG_SBCLK_GATING_REG                                   (0x00004090)
#define MCMISCSSAUG_SBCLK_GATING_DEF                                   (0x3FFFFFFF)
#define MCMISCSSAUG_SBCLK_GATING_VOLATILE_BITFIELDS_MSK                (0x00000000)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_OFF           ( 0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_WID           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_MSK           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_MAX           (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_DEF           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC0_HSH           (0x01004090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_OFF           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_WID           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_MSK           (0x00000002)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_MAX           (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_DEF           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC1_HSH           (0x01024090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_OFF           ( 2)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_WID           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_MSK           (0x00000004)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_MAX           (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_DEF           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC2_HSH           (0x01044090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_OFF           ( 3)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_WID           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_MSK           (0x00000008)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_MAX           (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_DEF           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCCC3_HSH           (0x01064090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_OFF           ( 4)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_WID           ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_MSK           (0x00000010)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_MAX           (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_DEF           (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrCOMP_HSH           (0x01084090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_OFF          ( 5)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_MSK          (0x00000020)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA0_HSH          (0x010A4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_OFF          ( 6)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_MSK          (0x00000040)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA1_HSH          (0x010C4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_OFF          ( 7)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_MSK          (0x00000080)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA2_HSH          (0x010E4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_OFF          ( 8)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_MSK          (0x00000100)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA3_HSH          (0x01104090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_OFF          ( 9)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_MSK          (0x00000200)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA4_HSH          (0x01124090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_OFF          (10)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_MSK          (0x00000400)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA5_HSH          (0x01144090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_OFF          (11)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_MSK          (0x00000800)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA6_HSH          (0x01164090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_OFF          (12)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_WID          ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_MSK          (0x00001000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_MIN          (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_MAX          (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_DEF          (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrDATA7_HSH          (0x01184090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_OFF        (13)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_WID        ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_MSK        (0x00002000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_MIN        (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_MAX        (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_DEF        (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrMCMISCS_HSH        (0x011A4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_OFF       (14)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_MSK       (0x00004000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO0_HSH       (0x011C4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_OFF       (15)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_MSK       (0x00008000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1_HSH       (0x011E4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_OFF      (16)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_WID      ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_MSK      (0x00010000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_MIN      (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_MAX      (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_DEF      (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO1B_HSH      (0x01204090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_OFF       (17)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_MSK       (0x00020000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO2_HSH       (0x01224090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_OFF       (18)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_MSK       (0x00040000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO3_HSH       (0x01244090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_OFF       (19)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_MSK       (0x00080000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO4_HSH       (0x01264090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_OFF       (20)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_MSK       (0x00100000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO5_HSH       (0x01284090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_OFF       (21)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_MSK       (0x00200000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO6_HSH       (0x012A4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_OFF       (22)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_MSK       (0x00400000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7_HSH       (0x012C4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_OFF      (23)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_WID      ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_MSK      (0x00800000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_MIN      (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_MAX      (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_DEF      (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO7B_HSH      (0x012E4090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_OFF       (24)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_MSK       (0x01000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO8_HSH       (0x01304090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_OFF       (25)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_WID       ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_MSK       (0x02000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_MIN       (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_MAX       (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_DEF       (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGCOMBO9_HSH       (0x01324090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_OFF        (26)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_WID        ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_MSK        (0x04000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_MIN        (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_MAX        (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_DEF        (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM0_HSH        (0x01344090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_OFF        (27)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_WID        ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_MSK        (0x08000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_MIN        (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_MAX        (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_DEF        (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrPGTERM1_HSH        (0x01364090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_OFF        (28)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_WID        ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_MSK        (0x10000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_MIN        (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_MAX        (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_DEF        (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_P_HSH        (0x01384090)

  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_OFF        (29)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_WID        ( 1)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_MSK        (0x20000000)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_MIN        (0)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_MAX        (1) // 0x00000001
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_DEF        (0x00000001)
  #define MCMISCSSAUG_SBCLK_GATING_DisSbClkGatingDdrRPTCH_S_HSH        (0x013A4090)

  #define MCMISCSSAUG_SBCLK_GATING_Spare_OFF                           (30)
  #define MCMISCSSAUG_SBCLK_GATING_Spare_WID                           ( 2)
  #define MCMISCSSAUG_SBCLK_GATING_Spare_MSK                           (0xC0000000)
  #define MCMISCSSAUG_SBCLK_GATING_Spare_MIN                           (0)
  #define MCMISCSSAUG_SBCLK_GATING_Spare_MAX                           (3) // 0x00000003
  #define MCMISCSSAUG_SBCLK_GATING_Spare_DEF                           (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING_Spare_HSH                           (0x023C4090)

#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_REG                             (0x00004094)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DEF                             (0x03400800)
#define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VOLATILE_BITFIELDS_MSK          (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_OFF         ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_MSK         (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdEn_HSH         (0x01004094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_OFF      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_WID      ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_MSK      (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_MAX      (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_DEF      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdEn_HSH      (0x01024094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_OFF   ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_MSK   (0x00000004)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdEn_HSH   (0x01044094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_OFF     ( 3)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_MSK     (0x00000008)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdEn_HSH     (0x01064094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_OFF         ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_MSK         (0x00000010)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdEn_HSH         (0x01084094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_OFF           ( 5)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_MSK           (0x00000020)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdEn_HSH           (0x010A4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_OFF        ( 6)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_MSK        (0x00000040)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCtrlOvrdVal_HSH        (0x010C4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_OFF     ( 7)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_MSK     (0x00000080)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetRestoreOvrdVal_HSH     (0x010E4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_OFF  ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_WID  ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_MSK  (0x00000100)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_MAX  (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetSRSBClkDisOvrdVal_HSH  (0x01104094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_OFF    ( 9)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_WID    ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_MSK    (0x00000200)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_MIN    (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_MAX    (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_DEF    (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4RetCriClkEnOvrdVal_HSH    (0x01124094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_OFF        (10)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_MSK        (0x00000400)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4CrResetOvrdVal_HSH        (0x01144094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_OFF          (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_MSK          (0x00000800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_DEF          (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_LPM4ClkEnOvrdVal_HSH          (0x01164094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_OFF (12)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_MSK (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CBitBlockInitHandShakeDurCompUpdate_HSH (0x01184094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_OFF     (13)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_MSK     (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrdEn_HSH     (0x011A4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_OFF     (14)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_WID     ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_MSK     (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_MIN     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_MAX     (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_DEF     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrd_HSH     (0x011C4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_OFF   (15)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_MSK   (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRWPUpdateOvrdEn_HSH   (0x011E4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_OFF   (16)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_MSK   (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrd_HSH   (0x01204094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_OFF (17)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_MSK (0x00020000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRRelockDoneOvrdEn_HSH (0x01224094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_OFF   (18)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_WID   ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_MSK   (0x00040000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_MAX   (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_IOG_LVRRelockDoneOvrdEn_HSH   (0x01244094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_OFF (19)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_MSK (0x00080000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_VccClk_LVRRelockDoneOvrdEn_HSH (0x01264094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_OFF (20)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_MSK (0x00100000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRPwrGoodOnWhileRelockOvrd_HSH (0x01284094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_OFF       (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_MSK       (0x00200000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_DistG_LVRVrefEnOvrd_HSH       (0x012A4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_OFF                  (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_WID                  ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_MSK                  (0x03C00000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_MIN                  (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_MAX                  (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_DEF                  (0x0000000D)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PLLEnLow_HSH                  (0x042C4094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_OFF (26)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_MSK (0x04000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_PllBypassenwaitForceonOvrd_HSH (0x01344094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_OFF (27)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_MSK (0x08000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_CstateExitPllenwaitSagpwrgoodOvrd_HSH (0x01364094)

  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_OFF                     (28)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_WID                     ( 4)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_MSK                     (0xF0000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_MIN                     (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_MAX                     (15) // 0x0000000F
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_DEF                     (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMOVRD4_Spare_HSH                     (0x04384094)

#define MCMISCSSAUG_SBCLK_GATING1_REG                                  (0x00004098)
#define MCMISCSSAUG_SBCLK_GATING1_DEF                                  (0x00000000)
#define MCMISCSSAUG_SBCLK_GATING1_VOLATILE_BITFIELDS_MSK               (0x00000000)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_OFF               ( 0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_WID               ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_MSK               (0x0000001F)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_MIN               (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_MAX               (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_DEF               (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCCC_HSH               (0x05004098)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_OFF              ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_WID              ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_MSK              (0x000003E0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_MIN              (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_MAX              (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_DEF              (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrDATA_HSH              (0x050A4098)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_OFF              (10)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_WID              ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_MSK              (0x00007C00)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_MIN              (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_MAX              (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_DEF              (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrCOMP_HSH              (0x05144098)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_OFF           (15)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_WID           ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_MSK           (0x000F8000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_MAX           (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_DEF           (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrMCMISCS_HSH           (0x051E4098)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_OFF             (20)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_WID             ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_MSK             (0x01F00000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_MIN             (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_MAX             (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_DEF             (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrRPTCH_HSH             (0x05284098)

  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_OFF           (25)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_WID           ( 5)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_MSK           (0x3E000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_MIN           (0)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_MAX           (31) // 0x0000001F
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_DEF           (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_GraceCountDdrPGCOMBO_HSH           (0x05324098)

  #define MCMISCSSAUG_SBCLK_GATING1_Spare_OFF                          (30)
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_WID                          ( 2)
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_MSK                          (0xC0000000)
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_MIN                          (0)
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_MAX                          (3) // 0x00000003
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_DEF                          (0x00000000)
  #define MCMISCSSAUG_SBCLK_GATING1_Spare_HSH                          (0x023C4098)

#define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_REG                              (0x0000409C)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_DEF                              (0x00000100)
#define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VOLATILE_BITFIELDS_MSK           (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_OFF       ( 0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_WID       ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_MSK       (0x00000003)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_MAX       (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableDelay_HSH       (0x0200409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_OFF          ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_WID          ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_MSK          (0x0000000C)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_MAX          (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampUpDelay_HSH          (0x0204409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_OFF   ( 4)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_WID   ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_MSK   (0x00000030)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_MAX   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodAssertDelay_HSH   (0x0208409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_OFF   ( 6)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_WID   ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_MSK   (0x000000C0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_MIN   (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_MAX   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_DEF   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_ReadyDeassertDelay_HSH   (0x020C409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_OFF      ( 8)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_WID      ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_MSK      (0x00000300)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_MIN      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_MAX      (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_DEF      (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRDisableDelay_HSH      (0x0210409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_OFF          (10)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_WID          ( 2)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_MSK          (0x00000C00)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_MAX          (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_RampDnDelay_HSH          (0x0214409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_OFF        (12)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_MSK        (0x00001000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROvrdModeEn_HSH        (0x0118409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_OFF        (13)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_WID        ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_MSK        (0x00002000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_MAX        (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVREnableOvrd_HSH        (0x011A409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_OFF          (14)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_WID          ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_MSK          (0x00004000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_MIN          (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_MAX          (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_DEF          (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_PwrGoodOvrd_HSH          (0x011C409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_OFF           (15)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_WID           ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_MSK           (0x00008000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_MAX           (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVROffOvrd_HSH           (0x011E409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_OFF         (16)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_MSK         (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_VssHi_LVRReadyOvrd_HSH         (0x0120409C)

  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_OFF                      (17)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_WID                      (15)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_MSK                      (0xFFFE0000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_MIN                      (0)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_MAX                      (32767) // 0x00007FFF
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_DEF                      (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_LVR_CTRL2_Spare_HSH                      (0x0F22409C)

#define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_REG                       (0x000040A0)
#define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_DEF                       (0x00000000)
#define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_OFF ( 0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_WID ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_MSK (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_MAX (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_EnMptuLpmodeExitCounter_HSH (0x010040A0)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_OFF ( 1)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_WID ( 6)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_MSK (0x0000007E)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_MAX (63) // 0x0000003F
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode0p5or2ExitCount_HSH (0x060240A0)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_OFF ( 7)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_WID ( 2)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_MSK (0x00000180)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_MAX (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode0p5or2_HSH (0x020E40A0)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_OFF ( 9)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_WID ( 6)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_MSK (0x00007E00)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_MIN (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_MAX (63) // 0x0000003F
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_DEF (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_LpMode3or4ExitCount_HSH (0x061240A0)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_OFF  (15)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_WID  ( 2)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_MSK  (0x00018000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_MIN  (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_MAX  (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_DEF  (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_StepSizeLpMode3or4_HSH  (0x021E40A0)

  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_OFF            (17)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_WID            (15)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_MSK            (0xFFFE0000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_MIN            (0)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_MAX            (32767) // 0x00007FFF
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_DEF            (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_MPTU_LPMODE_CTRL_reserved_HSH            (0x0F2240A0)

#define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_REG                           (0x000040A4)
#define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_VOLATILE_BITFIELDS_MSK        (0x00010001)

  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_OFF                ( 0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_WID                ( 1)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_MSK                (0x00000001)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_MIN                (0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_MAX                (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_DEF                (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_WaitiInt_HSH                (0x018040A4)

  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_OFF       ( 1)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_WID       ( 1)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_MSK       (0x00000002)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_MIN       (0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_MAX       (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_DEF       (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_EnGVRestoreBypass_HSH       (0x010240A4)

  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_OFF                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_WID                   (14)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_MSK                   (0x0000FFFC)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_MAX                   (16383) // 0x00003FFF
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd0_HSH                   (0x0E0440A4)

  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_OFF         (16)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_WID         ( 1)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_MSK         (0x00010000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_MAX         (1) // 0x00000001
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_GVRestoreBypass_HSH         (0x01A040A4)

  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_OFF                   (17)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_WID                   (15)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_MSK                   (0xFFFE0000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_MAX                   (32767) // 0x00007FFF
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_XTPATCH_CTRL_rsvd1_HSH                   (0x0F2240A4)

#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_REG                           (0x000040A8)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VOLATILE_BITFIELDS_MSK        (0x3FFFFFFF)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_OFF        ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_WID        (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_MSK        (0x000007FF)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_MIN        (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_MAX        (2047) // 0x000007FF
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_DEF        (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VccClkLVRCtrlFsm_HSH        (0x0B8040A8)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_OFF           (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_WID           (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_MSK           (0x003FF800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_MIN           (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_MAX           (2047) // 0x000007FF
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_DEF           (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_IOGLVRCtrlFsm_HSH           (0x0B9640A8)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_OFF         (22)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_WID         ( 8)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_MSK         (0x3FC00000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_MAX         (255) // 0x000000FF
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_VssHiLVRCtrlFsm_HSH         (0x08AC40A8)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_OFF                   (30)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_WID                   ( 2)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_MSK                   (0xC0000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_MAX                   (3) // 0x00000003
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS3_Spare_HSH                   (0x023C40A8)

#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_REG                           (0x000040AC)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_DEF                           (0x00000000)
#define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_VOLATILE_BITFIELDS_MSK        (0x000007FF)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_OFF         ( 0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_WID         (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_MSK         (0x000007FF)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_MIN         (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_MAX         (2047) // 0x000007FF
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_DEF         (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_IOGGVLVRCtrlFsm_HSH         (0x0B8040AC)

  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_OFF                   (11)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_WID                   (21)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_MSK                   (0xFFFFF800)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_MIN                   (0)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_MAX                   (2097151) // 0x001FFFFF
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_DEF                   (0x00000000)
  #define DDRPHY_MISC_SAUG_CR_PHYPMSTATUS4_Spare_HSH                   (0x151640AC)

#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG                          (0x00004120)
#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_DEF                          (0x04848320)
#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_VOLATILE_BITFIELDS_MSK       (0x07FFF9FE)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_OFF  ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_WID  ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MSK  (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MIN  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MAX  (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_DEF  (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_HSH  (0x01004120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_OFF         ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_WID         ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MSK         (0x000001FE)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MIN         (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MAX         (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_DEF         (0x00000090)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_HSH         (0x08824120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_OFF        ( 9)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_WID        ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MSK        (0x00000600)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MIN        (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MAX        (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_DEF        (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_HSH        (0x02124120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_OFF  (11)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_WID  ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_MSK  (0x0007F800)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_MIN  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_MAX  (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_DEF  (0x00000090)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb0vrefsel_HSH  (0x08964120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_OFF  (19)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_WID  ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_MSK  (0x07F80000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_MIN  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_MAX  (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_DEF  (0x00000090)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_rxdllb1vrefsel_HSH  (0x08A64120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_OFF     (27)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_WID     ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_MSK     (0x18000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_MIN     (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_MAX     (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_DEF     (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_GV_HSH     (0x02364120)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_OFF               (29)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_WID               ( 3)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MSK               (0xE0000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MIN               (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MAX               (7) // 0x00000007
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_DEF               (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_HSH               (0x033A4120)

#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP1_REG                          (0x00004124)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP2_REG                          (0x00004128)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP3_REG                          (0x0000412C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG                         (0x00004130)
#define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_DEF                         (0x00000000)
#define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_VOLATILE_BITFIELDS_MSK      (0x01FFFFE1)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_OFF               ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_WID               ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_MSK               (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_MIN               (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_MAX               (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_DEF               (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_RunBusy_HSH               (0x01804130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_OFF                ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_WID                ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_MSK                (0x00000006)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_MIN                (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_MAX                (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_DEF                (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_SelLVR_HSH                (0x02024130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_OFF                 ( 3)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_WID                 ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_MSK                 (0x00000018)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_MIN                 (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_MAX                 (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_DEF                 (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_Delay_HSH                 (0x02064130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_OFF           ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_WID           ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_MSK           (0x000003E0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_MIN           (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_MAX           (31) // 0x0000001F
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_DEF           (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop0_HSH           (0x058A4130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_OFF           (10)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_WID           ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_MSK           (0x00007C00)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_MIN           (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_MAX           (31) // 0x0000001F
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_DEF           (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_StatusChop1_HSH           (0x05944130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_OFF             (15)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_WID             ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MSK             (0x000F8000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MIN             (-16)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MAX             (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_DEF             (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetIOG_HSH             (0x859E4130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_OFF             (20)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_WID             ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_MSK             (0x01F00000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_MIN             (-16)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_MAX             (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_DEF             (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_OffsetClk_HSH             (0x85A84130)

  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_OFF              (25)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_WID              ( 7)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_MSK              (0xFE000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_MIN              (0)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_MAX              (127) // 0x0000007F
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_DEF              (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_reserved_HSH              (0x07324130)

#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG                          (0x00004134)
#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_DEF                          (0x00788995)
#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_OFF                    ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_MSK                    (0x0000000F)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_DEF                    (0x00000005)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw0_HSH                    (0x04004134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_OFF                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_MSK                    (0x000000F0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_DEF                    (0x00000009)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw1_HSH                    (0x04084134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_OFF                    ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_MSK                    (0x00000F00)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_DEF                    (0x00000009)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw2_HSH                    (0x04104134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_OFF                    (12)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_MSK                    (0x0000F000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_DEF                    (0x00000008)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw3_HSH                    (0x04184134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_OFF                    (16)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_MSK                    (0x000F0000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_DEF                    (0x00000008)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw4_HSH                    (0x04204134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_OFF                    (20)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_MSK                    (0x00F00000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_DEF                    (0x00000007)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_bw5_HSH                    (0x04284134)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_OFF                  (24)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_WID                  ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_MSK                  (0xFF000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_MIN                  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_MAX                  (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_DEF                  (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_spare_HSH                  (0x08304134)

#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG                          (0x00004138)
#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_DEF                          (0x016BF447)
#define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_VOLATILE_BITFIELDS_MSK       (0x00000000)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_OFF                    ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_MSK                    (0x0000000F)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_DEF                    (0x00000007)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw6_HSH                    (0x04004138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_OFF                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_MSK                    (0x000000F0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_DEF                    (0x00000004)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw7_HSH                    (0x04084138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_OFF                    ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_WID                    ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_MSK                    (0x00000F00)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_MAX                    (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_DEF                    (0x00000004)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_bw8_HSH                    (0x04104138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_OFF                    (12)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_MSK                    (0x00003000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_DEF                    (0x00000003)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb0_HSH                    (0x02184138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_OFF                    (14)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_MSK                    (0x0000C000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_DEF                    (0x00000003)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb1_HSH                    (0x021C4138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_OFF                    (16)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_MSK                    (0x00030000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_DEF                    (0x00000003)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb2_HSH                    (0x02204138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_OFF                    (18)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_MSK                    (0x000C0000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_DEF                    (0x00000002)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb3_HSH                    (0x02244138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_OFF                    (20)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_MSK                    (0x00300000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_DEF                    (0x00000002)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb4_HSH                    (0x02284138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_OFF                    (22)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_MSK                    (0x00C00000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_DEF                    (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb5_HSH                    (0x022C4138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_OFF                    (24)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_MSK                    (0x03000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_DEF                    (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb6_HSH                    (0x02304138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_OFF                    (26)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_MSK                    (0x0C000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_DEF                    (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb7_HSH                    (0x02344138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_OFF                    (28)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_WID                    ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_MSK                    (0x30000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_MAX                    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_DEF                    (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_cb8_HSH                    (0x02384138)

  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_OFF                  (30)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_WID                  ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_MSK                  (0xC0000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_MIN                  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_MAX                  (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_DEF                  (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_spare_HSH                  (0x023C4138)

#define DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE0_REG                        (0x0000413C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM0_CR_DDRCRTXDLLCBTUNE1_REG                        (0x00004140)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG                             (0x00004144)
#define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DEF                             (0x0000000C)
#define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VOLATILE_BITFIELDS_MSK          (0x00006CF0)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_OFF    ( 0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_WID    ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MSK    (0x00000003)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MIN    (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MAX    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_DEF    (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_HSH    (0x02004144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_OFF   ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_WID   ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MSK   (0x0000000C)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MIN   (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MAX   (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_DEF   (0x00000003)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_HSH   (0x02044144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_OFF           ( 4)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_WID           ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MSK           (0x00000030)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MIN           (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MAX           (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_DEF           (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_HSH           (0x02884144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_OFF           ( 6)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_WID           ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MSK           (0x000000C0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MIN           (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MAX           (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_DEF           (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_HSH           (0x028C4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_OFF     ( 8)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_WID     ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MSK     (0x00000300)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MIN     (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MAX     (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_DEF     (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_HSH     (0x02104144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_OFF    (10)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_WID    ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MSK    (0x00000C00)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MIN    (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MAX    (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_DEF    (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_HSH    (0x02944144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_OFF      (12)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_WID      ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MSK      (0x00001000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MIN      (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MAX      (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_DEF      (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_HSH      (0x01184144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_OFF         (13)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_WID         ( 2)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MSK         (0x00006000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MIN         (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MAX         (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_DEF         (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_HSH         (0x029A4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_OFF               (15)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_WID               ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MSK               (0x00008000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MIN               (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MAX               (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_DEF               (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_HSH               (0x011E4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_OFF  (16)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_WID  ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MSK  (0x00010000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MIN  (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MAX  (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_DEF  (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_HSH  (0x01204144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_OFF      (17)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_WID      ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MSK      (0x00020000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MIN      (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MAX      (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_DEF      (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_HSH      (0x01224144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_OFF  (18)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_WID  ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MSK  (0x00040000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MIN  (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MAX  (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_DEF  (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_HSH  (0x01244144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_OFF      (19)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_WID      ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MSK      (0x00080000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MIN      (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MAX      (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_DEF      (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_HSH      (0x01264144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_OFF  (20)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_WID  ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_MSK  (0x00100000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_MIN  (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_MAX  (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_DEF  (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_txdlldisable_ovrd_HSH  (0x01284144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_OFF (21)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_WID ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_MSK (0x00200000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_MIN (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_MAX (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_DEF (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb0disable_ovrd_HSH (0x012A4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_OFF (22)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_WID ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_MSK (0x00400000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_MIN (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_MAX (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_DEF (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_clklvr_rxdllb1disable_ovrd_HSH (0x012C4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_OFF       (23)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_WID       ( 1)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MSK       (0x00800000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MIN       (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MAX       (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_DEF       (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_HSH       (0x012E4144)

  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_OFF                  (24)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_WID                  ( 8)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_MSK                  (0xFF000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_MIN                  (0)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_MAX                  (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_DEF                  (0x00000000)
  #define DDRDATA_SBMEM0_CR_PM_MISC_CTRL_reserved_HSH                  (0x08304144)

#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG                          (0x00004148)
#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_DEF                          (0x00010320)
#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_VOLATILE_BITFIELDS_MSK       (0x000001FE)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_OFF  ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_WID  ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_MSK  (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_MIN  (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_MAX  (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_DEF  (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_selvdd2_ladder_HSH  (0x01004148)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_OFF         ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_WID         ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_MSK         (0x000001FE)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_MIN         (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_MAX         (255) // 0x000000FF
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_DEF         (0x00000090)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_vrefsel_HSH         (0x08824148)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_OFF        ( 9)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_WID        ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_MSK        (0x00000600)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_MIN        (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_MAX        (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_DEF        (0x00000001)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_gainctrl_HSH        (0x02124148)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_OFF      (11)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_WID      ( 6)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_MSK      (0x0001F800)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_MIN      (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_MAX      (63) // 0x0000003F
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_DEF      (0x00000020)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_ffreadcode_HSH      (0x06164148)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_OFF        (17)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_WID        ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_MSK        (0x00060000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_MIN        (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_MAX        (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_DEF        (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_ioglvr_bonus_in_HSH        (0x02224148)

  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_OFF               (19)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_WID               (13)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_MSK               (0xFFF80000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_MIN               (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_MAX               (8191) // 0x00001FFF
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_DEF               (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_reserved_HSH               (0x0D264148)

#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP1_REG                          (0x0000414C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP2_REG                          (0x00004150)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP3_REG                          (0x00004154)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM0_SB_DEBUG_REG                                    (0x00004158)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG                               (0x0000415C)
#define DDRDATA_SBMEM0_CR_DDRCRVSSHI_DEF                               (0x00000005)
#define DDRDATA_SBMEM0_CR_DDRCRVSSHI_VOLATILE_BITFIELDS_MSK            (0x003FC300)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_OFF            ( 0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_WID            ( 3)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MSK            (0x00000007)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MIN            (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MAX            (7) // 0x00000007
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_DEF            (0x00000005)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_HSH            (0x0300415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_OFF            ( 3)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_WID            ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MSK            (0x00000008)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MIN            (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MAX            (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_DEF            (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_HSH            (0x0106415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_OFF      ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_WID      ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MSK      (0x00000010)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MIN      (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MAX      (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_DEF      (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_HSH      (0x0108415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_OFF   ( 5)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_WID   ( 1)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MSK   (0x00000020)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MIN   (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MAX   (1) // 0x00000001
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_DEF   (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_HSH   (0x010A415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_OFF           ( 6)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_WID           ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MSK           (0x000000C0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MIN           (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MAX           (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_DEF           (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_HSH           (0x020C415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_OFF          ( 8)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_WID          ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MSK          (0x00000300)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MIN          (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MAX          (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_DEF          (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_HSH          (0x0290415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_OFF       (10)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_WID       ( 4)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_MSK       (0x00003C00)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_MIN       (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_MAX       (15) // 0x0000000F
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_DEF       (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_in_nonGV_HSH       (0x0414415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_OFF            (14)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_WID            ( 6)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_MSK            (0x000FC000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_MIN            (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_MAX            (63) // 0x0000003F
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_DEF            (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_clklvr_bonus_out_HSH            (0x069C415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_OFF            (20)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_WID            ( 2)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_MSK            (0x00300000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_MIN            (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_MAX            (3) // 0x00000003
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_DEF            (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_ioglvr_bonus_out_HSH            (0x02A8415C)

  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_OFF                    (22)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_WID                    (10)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_MSK                    (0xFFC00000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_MIN                    (0)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_MAX                    (1023) // 0x000003FF
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_DEF                    (0x00000000)
  #define DDRDATA_SBMEM0_CR_DDRCRVSSHI_reserved_HSH                    (0x0A2C415C)

#define DDRDATA_SBMEM0_CR_SPARE_REG                                    (0x00004160)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x00004164)
#define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_DEF                       (0x00000000)
#define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_OFF    ( 0)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_WID    ( 8)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_MSK    (0x000000FF)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_MIN    (0)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_MAX    (255) // 0x000000FF
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_DEF    (0x00000000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port0_HSH    (0x08004164)

  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_OFF    ( 8)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_WID    ( 8)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_MSK    (0x0000FF00)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_MIN    (0)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_MAX    (255) // 0x000000FF
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_DEF    (0x00000000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_viewmuxsel_port1_HSH    (0x08104164)

  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_OFF          (16)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_WID          ( 8)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_MSK          (0x00FF0000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_MIN          (0)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_MAX          (255) // 0x000000FF
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_DEF          (0x00000000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_visamuxsel_HSH          (0x08204164)

  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_OFF               (24)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_WID               ( 8)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_MSK               (0xFF000000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_MIN               (0)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_MAX               (255) // 0x000000FF
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_DEF               (0x00000000)
  #define DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_Spare_HSH               (0x08304164)

#define DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP0_REG                          (0x000041A0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP1_REG                          (0x000041A4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP2_REG                          (0x000041A8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCCLK_WP3_REG                          (0x000041AC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRLVRAUTOTRIM_REG                         (0x000041B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE0_REG                          (0x000041B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM1_CR_DDRCRDLLCBTUNE1_REG                          (0x000041B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE0_REG                        (0x000041BC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM1_CR_DDRCRTXDLLCBTUNE1_REG                        (0x000041C0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM1_CR_PM_MISC_CTRL_REG                             (0x000041C4)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP0_REG                          (0x000041C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP1_REG                          (0x000041CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP2_REG                          (0x000041D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM1_CR_DDRCRVCCIOG_WP3_REG                          (0x000041D4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM1_SB_DEBUG_REG                                    (0x000041D8)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM1_CR_DDRCRVSSHI_REG                               (0x000041DC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM1_CR_SPARE_REG                                    (0x000041E0)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM1_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x000041E4)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP0_REG                          (0x00004220)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP1_REG                          (0x00004224)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP2_REG                          (0x00004228)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCCLK_WP3_REG                          (0x0000422C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRLVRAUTOTRIM_REG                         (0x00004230)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM2_CR_DDRCRDLLCBTUNE0_REG                          (0x00004234)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM2_CR_DDRCRDLLCBTUNE1_REG                          (0x00004238)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM2_CR_DDRCRTXDLLCBTUNE0_REG                        (0x0000423C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM2_CR_DDRCRTXDLLCBTUNE1_REG                        (0x00004240)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM2_CR_PM_MISC_CTRL_REG                             (0x00004244)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP0_REG                          (0x00004248)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP1_REG                          (0x0000424C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP2_REG                          (0x00004250)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM2_CR_DDRCRVCCIOG_WP3_REG                          (0x00004254)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM2_SB_DEBUG_REG                                    (0x00004258)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM2_CR_DDRCRVSSHI_REG                               (0x0000425C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM2_CR_SPARE_REG                                    (0x00004260)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM2_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x00004264)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP0_REG                          (0x000042A0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP1_REG                          (0x000042A4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP2_REG                          (0x000042A8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCCLK_WP3_REG                          (0x000042AC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRLVRAUTOTRIM_REG                         (0x000042B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM3_CR_DDRCRDLLCBTUNE0_REG                          (0x000042B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM3_CR_DDRCRDLLCBTUNE1_REG                          (0x000042B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM3_CR_DDRCRTXDLLCBTUNE0_REG                        (0x000042BC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM3_CR_DDRCRTXDLLCBTUNE1_REG                        (0x000042C0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM3_CR_PM_MISC_CTRL_REG                             (0x000042C4)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP0_REG                          (0x000042C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP1_REG                          (0x000042CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP2_REG                          (0x000042D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM3_CR_DDRCRVCCIOG_WP3_REG                          (0x000042D4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM3_SB_DEBUG_REG                                    (0x000042D8)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM3_CR_DDRCRVSSHI_REG                               (0x000042DC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM3_CR_SPARE_REG                                    (0x000042E0)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM3_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x000042E4)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP0_REG                          (0x00004320)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP1_REG                          (0x00004324)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP2_REG                          (0x00004328)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCCLK_WP3_REG                          (0x0000432C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRLVRAUTOTRIM_REG                         (0x00004330)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM4_CR_DDRCRDLLCBTUNE0_REG                          (0x00004334)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM4_CR_DDRCRDLLCBTUNE1_REG                          (0x00004338)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM4_CR_DDRCRTXDLLCBTUNE0_REG                        (0x0000433C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM4_CR_DDRCRTXDLLCBTUNE1_REG                        (0x00004340)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM4_CR_PM_MISC_CTRL_REG                             (0x00004344)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP0_REG                          (0x00004348)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP1_REG                          (0x0000434C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP2_REG                          (0x00004350)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM4_CR_DDRCRVCCIOG_WP3_REG                          (0x00004354)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM4_SB_DEBUG_REG                                    (0x00004358)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM4_CR_DDRCRVSSHI_REG                               (0x0000435C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM4_CR_SPARE_REG                                    (0x00004360)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM4_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x00004364)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP0_REG                          (0x000043A0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP1_REG                          (0x000043A4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP2_REG                          (0x000043A8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCCLK_WP3_REG                          (0x000043AC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRLVRAUTOTRIM_REG                         (0x000043B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM5_CR_DDRCRDLLCBTUNE0_REG                          (0x000043B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM5_CR_DDRCRDLLCBTUNE1_REG                          (0x000043B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM5_CR_DDRCRTXDLLCBTUNE0_REG                        (0x000043BC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM5_CR_DDRCRTXDLLCBTUNE1_REG                        (0x000043C0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM5_CR_PM_MISC_CTRL_REG                             (0x000043C4)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP0_REG                          (0x000043C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP1_REG                          (0x000043CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP2_REG                          (0x000043D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM5_CR_DDRCRVCCIOG_WP3_REG                          (0x000043D4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM5_SB_DEBUG_REG                                    (0x000043D8)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM5_CR_DDRCRVSSHI_REG                               (0x000043DC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM5_CR_SPARE_REG                                    (0x000043E0)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM5_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x000043E4)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP0_REG                          (0x00004420)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP1_REG                          (0x00004424)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP2_REG                          (0x00004428)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCCLK_WP3_REG                          (0x0000442C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRLVRAUTOTRIM_REG                         (0x00004430)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM6_CR_DDRCRDLLCBTUNE0_REG                          (0x00004434)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM6_CR_DDRCRDLLCBTUNE1_REG                          (0x00004438)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM6_CR_DDRCRTXDLLCBTUNE0_REG                        (0x0000443C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM6_CR_DDRCRTXDLLCBTUNE1_REG                        (0x00004440)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM6_CR_PM_MISC_CTRL_REG                             (0x00004444)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP0_REG                          (0x00004448)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP1_REG                          (0x0000444C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP2_REG                          (0x00004450)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM6_CR_DDRCRVCCIOG_WP3_REG                          (0x00004454)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM6_SB_DEBUG_REG                                    (0x00004458)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM6_CR_DDRCRVSSHI_REG                               (0x0000445C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM6_CR_SPARE_REG                                    (0x00004460)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM6_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x00004464)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP0_REG                          (0x000044A0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP1_REG                          (0x000044A4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP2_REG                          (0x000044A8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCCLK_WP3_REG                          (0x000044AC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRLVRAUTOTRIM_REG                         (0x000044B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRDATA_SBMEM7_CR_DDRCRDLLCBTUNE0_REG                          (0x000044B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM7_CR_DDRCRDLLCBTUNE1_REG                          (0x000044B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM7_CR_DDRCRTXDLLCBTUNE0_REG                        (0x000044BC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRDATA_SBMEM7_CR_DDRCRTXDLLCBTUNE1_REG                        (0x000044C0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRDATA_SBMEM7_CR_PM_MISC_CTRL_REG                             (0x000044C4)
//Duplicate of DDRDATA_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP0_REG                          (0x000044C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP1_REG                          (0x000044CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP2_REG                          (0x000044D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM7_CR_DDRCRVCCIOG_WP3_REG                          (0x000044D4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRDATA_SBMEM7_SB_DEBUG_REG                                    (0x000044D8)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRDATA_SBMEM7_CR_DDRCRVSSHI_REG                               (0x000044DC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRDATA_SBMEM7_CR_SPARE_REG                                    (0x000044E0)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRDATA_SBMEM7_SB_DEBUG_1_0_0_MCHBAR_REG                       (0x000044E4)
//Duplicate of DDRDATA_SBMEM0_SB_DEBUG_1_0_0_MCHBAR_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG                           (0x000045A0)
#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_DEF                           (0x00000320)
#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_VOLATILE_BITFIELDS_MSK        (0x000001FE)

  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_OFF   ( 0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_WID   ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MSK   (0x00000001)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MIN   (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_MAX   (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_DEF   (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_selvdd2_ladder_HSH   (0x010045A0)

  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_OFF          ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_WID          ( 8)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MSK          (0x000001FE)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MIN          (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_MAX          (255) // 0x000000FF
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_DEF          (0x00000090)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_vrefsel_HSH          (0x088245A0)

  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_OFF         ( 9)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_WID         ( 2)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MSK         (0x00000600)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MIN         (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_MAX         (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_DEF         (0x00000001)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_gainctrl_HSH         (0x021245A0)

  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_OFF         (11)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_WID         ( 2)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_MSK         (0x00001800)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_MIN         (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_MAX         (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_DEF         (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_clklvr_bonus_in_HSH         (0x021645A0)

  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_OFF                (13)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_WID                (19)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MSK                (0xFFFFE000)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MIN                (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_MAX                (524287) // 0x0007FFFF
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_DEF                (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_reserved_HSH                (0x131A45A0)

#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP1_REG                           (0x000045A4)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP2_REG                           (0x000045A8)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP3_REG                           (0x000045AC)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG                          (0x000045B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRCCC_SBMEM0_CR_DDRCRTXDLLCBTUNE0_REG                         (0x000045B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRCCC_SBMEM0_CR_DDRCRTXDLLCBTUNE1_REG                         (0x000045B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_REG                         (0x000045BC)
#define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DEF                         (0x027920CB)
#define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_OFF                ( 0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_WID                ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_MSK                (0x00000007)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_MIN                (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_MAX                (7) // 0x00000007
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_DEF                (0x00000003)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCKCSH_HSH                (0x030045BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_OFF                  ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_WID                  ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_MSK                  (0x00000038)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_MIN                  (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_MAX                  (7) // 0x00000007
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_DEF                  (0x00000001)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSH_HSH                  (0x030645BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_OFF             ( 6)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_WID             ( 4)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_MSK             (0x000003C0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_MIN             (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_MAX             (15) // 0x0000000F
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_DEF             (0x00000003)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tCSLortXP_HSH             (0x040C45BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_OFF           (10)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_WID           ( 6)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_MSK           (0x0000FC00)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_MIN           (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_MAX           (63) // 0x0000003F
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_DEF           (0x00000008)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_tSRX2CKSTOP_HSH           (0x061445BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_OFF            (16)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_WID            ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_MSK            (0x00070000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_MIN            (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_MAX            (7) // 0x00000007
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_DEF            (0x00000001)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_TimerCount_HSH            (0x032045BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_OFF                (19)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_WID                ( 4)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_MSK                (0x00780000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_MIN                (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_MAX                (15) // 0x0000000F
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_DEF                (0x0000000F)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_RANKEN_HSH                (0x042645BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_OFF            (23)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_WID            ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_MSK            (0x00800000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_MIN            (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_MAX            (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_DEF            (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH0_HSH            (0x012E45BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_OFF            (24)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_WID            ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_MSK            (0x01000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_MIN            (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_MAX            (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_DEF            (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_LPDDR5DCH1_HSH            (0x013045BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_OFF              (25)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_WID              ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_MSK              (0x02000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_MIN              (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_MAX              (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_DEF              (0x00000001)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENSRXFSM_HSH              (0x013245BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_OFF      (26)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_WID      ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_MSK      (0x04000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_MIN      (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_MAX      (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_DEF      (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSRXFSMCLKGATE_HSH      (0x013445BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_OFF        (27)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_WID        ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_MSK        (0x08000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_MIN        (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_MAX        (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_DEF        (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISSHADOWBLOCK_HSH        (0x013645BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_OFF     (28)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_WID     ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_MSK     (0x10000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_MIN     (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_MAX     (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_DEF     (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_DISLPMODE4CLKGATE_HSH     (0x013845BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_OFF          (29)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_WID          ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_MSK          (0x20000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_MIN          (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_MAX          (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_DEF          (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECADRVEN_HSH          (0x013A45BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_OFF         (30)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_WID         ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_MSK         (0x40000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_MIN         (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_MAX         (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_DEF         (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_FORCECLKDRVEN_HSH         (0x013C45BC)

  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_OFF          (31)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_WID          ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_MSK          (0x80000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_MIN          (0)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_MAX          (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_DEF          (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_ENCADRVENSRX_HSH          (0x013E45BC)

#define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG                              (0x000045C0)
#define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DEF                              (0x000000C0)
#define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VOLATILE_BITFIELDS_MSK           (0x0006CF0F)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_OFF           ( 0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_WID           ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_MSK           (0x00000003)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_MIN           (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_MAX           (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_DEF           (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_bonus_out_HSH           (0x028045C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_OFF           ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_WID           ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_MSK           (0x0000000C)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_MIN           (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_MAX           (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_DEF           (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_bonus_out_HSH           (0x028445C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_OFF     ( 4)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_WID     ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MSK     (0x00000030)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MIN     (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_MAX     (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_DEF     (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_en_HSH     (0x020845C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_OFF    ( 6)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_WID    ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MSK    (0x000000C0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MIN    (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_MAX    (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_DEF    (0x00000003)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_channel_ovrd_val_HSH    (0x020C45C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_OFF            ( 8)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_WID            ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MSK            (0x00000300)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MIN            (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_MAX            (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_DEF            (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG0BonusOut_HSH            (0x029045C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_OFF            (10)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_WID            ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MSK            (0x00000C00)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MIN            (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_MAX            (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_DEF            (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGPG1BonusOut_HSH            (0x029445C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_OFF      (12)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_WID      ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MSK      (0x00003000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MIN      (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_MAX      (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_DEF      (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusIn_HSH      (0x021845C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_OFF     (14)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_WID     ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MSK     (0x0000C000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MIN     (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_MAX     (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_DEF     (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_PwrGoodDistParBonusOut_HSH     (0x029C45C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_OFF       (16)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_WID       ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MSK       (0x00010000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MIN       (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_MAX       (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_DEF       (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_EnIOGDDQGLpMode4Mask_HSH       (0x012045C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_OFF          (17)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_WID          ( 2)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MSK          (0x00060000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MIN          (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_MAX          (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_DEF          (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_VccDD2GPGBonusOut_HSH          (0x02A245C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_OFF                (19)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_WID                ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MSK                (0x00080000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MIN                (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_MAX                (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_DEF                (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_DDQGDisable_HSH                (0x012645C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_OFF   (20)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_WID   ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MSK   (0x00100000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MIN   (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_MAX   (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_DEF   (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVRVrefEn_MaskforLp4_HSH   (0x012845C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_OFF       (21)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_WID       ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MSK       (0x00200000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MIN       (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_MAX       (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_DEF       (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_CLK_LVREn_MaskforLp4_HSH       (0x012A45C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_OFF   (22)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_WID   ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MSK   (0x00400000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MIN   (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_MAX   (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_DEF   (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVRVrefEn_MaskforLp4_HSH   (0x012C45C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_OFF       (23)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_WID       ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MSK       (0x00800000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MIN       (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_MAX       (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_DEF       (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_IOG_LVREn_MaskforLp4_HSH       (0x012E45C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_OFF        (24)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_WID        ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MSK        (0x01000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MIN        (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_MAX        (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_DEF        (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_ioglvr_disable_ovrd_HSH        (0x013045C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_OFF        (25)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_WID        ( 1)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_MSK        (0x02000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_MIN        (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_MAX        (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_DEF        (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_clklvr_disable_ovrd_HSH        (0x013245C0)

  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_OFF                   (26)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_WID                   ( 6)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_MSK                   (0xFC000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_MIN                   (0)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_MAX                   (63) // 0x0000003F
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_DEF                   (0x00000000)
  #define DDRCCC_SBMEM0_CR_PM_MISC_CTRL_reserved_HSH                   (0x063445C0)

#define DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP0_REG                           (0x000045C4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP1_REG                           (0x000045C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP2_REG                           (0x000045CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM0_CR_DDRCRVCCIOG_WP3_REG                           (0x000045D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM0_SB_DEBUG_REG                                     (0x000045D4)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG                                (0x000045D8)
#define DDRCCC_SBMEM0_CR_DDRCRVSSHI_DEF                                (0x00000005)
#define DDRCCC_SBMEM0_CR_DDRCRVSSHI_VOLATILE_BITFIELDS_MSK             (0x00000300)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_OFF             ( 0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_WID             ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MSK             (0x00000007)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MIN             (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_MAX             (7) // 0x00000007
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_DEF             (0x00000005)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_vrefsel_HSH             (0x030045D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_OFF             ( 3)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_WID             ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MSK             (0x00000008)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MIN             (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_MAX             (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_DEF             (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvrbypassen_HSH             (0x010645D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_OFF       ( 4)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_WID       ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MSK       (0x00000010)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MIN       (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_MAX       (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_DEF       (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVREn_MaskforLp4_HSH       (0x010845D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_OFF    ( 5)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_WID    ( 1)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MSK    (0x00000020)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MIN    (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_MAX    (1) // 0x00000001
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_DEF    (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshi_LVRReady_MaskforLp4_HSH    (0x010A45D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_OFF            ( 6)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_WID            ( 2)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MSK            (0x000000C0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MIN            (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_MAX            (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_DEF            (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_in_HSH            (0x020C45D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_OFF           ( 8)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_WID           ( 2)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MSK           (0x00000300)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MIN           (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_MAX           (3) // 0x00000003
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_DEF           (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_vsshilvr_bonus_out_HSH           (0x029045D8)

  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_OFF                     (10)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_WID                     (22)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_MSK                     (0xFFFFFC00)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_MIN                     (0)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_MAX                     (4194303) // 0x003FFFFF
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_DEF                     (0x00000000)
  #define DDRCCC_SBMEM0_CR_DDRCRVSSHI_reserved_HSH                     (0x161445D8)

#define DDRCCC_SBMEM0_CR_SPARE_REG                                     (0x000045DC)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP0_REG                           (0x00004620)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP1_REG                           (0x00004624)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP2_REG                           (0x00004628)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCCLK_WP3_REG                           (0x0000462C)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRLVRAUTOTRIM_REG                          (0x00004630)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRCCC_SBMEM1_CR_DDRCRTXDLLCBTUNE0_REG                         (0x00004634)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRCCC_SBMEM1_CR_DDRCRTXDLLCBTUNE1_REG                         (0x00004638)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRCCC_SBMEM1_CR_DDRCRLPMODE4CTRL0_REG                         (0x0000463C)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_REG

#define DDRCCC_SBMEM1_CR_PM_MISC_CTRL_REG                              (0x00004640)
//Duplicate of DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP0_REG                           (0x00004644)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP1_REG                           (0x00004648)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP2_REG                           (0x0000464C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM1_CR_DDRCRVCCIOG_WP3_REG                           (0x00004650)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM1_SB_DEBUG_REG                                     (0x00004654)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRCCC_SBMEM1_CR_DDRCRVSSHI_REG                                (0x00004658)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRCCC_SBMEM1_CR_SPARE_REG                                     (0x0000465C)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP0_REG                           (0x000046A0)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP1_REG                           (0x000046A4)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP2_REG                           (0x000046A8)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCCLK_WP3_REG                           (0x000046AC)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRLVRAUTOTRIM_REG                          (0x000046B0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRCCC_SBMEM2_CR_DDRCRTXDLLCBTUNE0_REG                         (0x000046B4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRCCC_SBMEM2_CR_DDRCRTXDLLCBTUNE1_REG                         (0x000046B8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRCCC_SBMEM2_CR_DDRCRLPMODE4CTRL0_REG                         (0x000046BC)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_REG

#define DDRCCC_SBMEM2_CR_PM_MISC_CTRL_REG                              (0x000046C0)
//Duplicate of DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP0_REG                           (0x000046C4)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP1_REG                           (0x000046C8)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP2_REG                           (0x000046CC)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM2_CR_DDRCRVCCIOG_WP3_REG                           (0x000046D0)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM2_SB_DEBUG_REG                                     (0x000046D4)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRCCC_SBMEM2_CR_DDRCRVSSHI_REG                                (0x000046D8)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRCCC_SBMEM2_CR_SPARE_REG                                     (0x000046DC)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP0_REG                           (0x00004720)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP1_REG                           (0x00004724)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP2_REG                           (0x00004728)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCCLK_WP3_REG                           (0x0000472C)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVCCCLK_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRLVRAUTOTRIM_REG                          (0x00004730)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRLVRAUTOTRIM_REG

#define DDRCCC_SBMEM3_CR_DDRCRTXDLLCBTUNE0_REG                         (0x00004734)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE0_REG

#define DDRCCC_SBMEM3_CR_DDRCRTXDLLCBTUNE1_REG                         (0x00004738)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRDLLCBTUNE1_REG

#define DDRCCC_SBMEM3_CR_DDRCRLPMODE4CTRL0_REG                         (0x0000473C)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRLPMODE4CTRL0_REG

#define DDRCCC_SBMEM3_CR_PM_MISC_CTRL_REG                              (0x00004740)
//Duplicate of DDRCCC_SBMEM0_CR_PM_MISC_CTRL_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP0_REG                           (0x00004744)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP1_REG                           (0x00004748)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP2_REG                           (0x0000474C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM3_CR_DDRCRVCCIOG_WP3_REG                           (0x00004750)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRCCC_SBMEM3_SB_DEBUG_REG                                     (0x00004754)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRCCC_SBMEM3_CR_DDRCRVSSHI_REG                                (0x00004758)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRCCC_SBMEM3_CR_SPARE_REG                                     (0x0000475C)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG                     (0x00004780)
#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_DEF                     (0x00014530)
#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_OFF        ( 0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_WID        ( 5)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_MSK        (0x0000001F)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_MIN        (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_MAX        (31) // 0x0000001F
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_DEF        (0x00000010)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermcomp_HSH        (0x05004780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_OFF    ( 5)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_WID    ( 1)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_MSK    (0x00000020)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_MIN    (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_MAX    (1) // 0x00000001
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_DEF    (0x00000001)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermstaticen_HSH    (0x010A4780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_OFF      ( 6)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_WID      ( 1)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_MSK      (0x00000040)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_MIN      (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_MAX      (1) // 0x00000001
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_DEF      (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_tltermovrden_HSH      (0x010C4780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_OFF  ( 7)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_WID  ( 6)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_MSK  (0x00001F80)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_MIN  (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_MAX  (63) // 0x0000003F
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_DEF  (0x0000000A)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLClken2TLTermEn_HSH  (0x060E4780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_OFF  (13)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_WID  ( 6)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_MSK  (0x0007E000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_MIN  (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_MAX  (63) // 0x0000003F
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_DEF  (0x0000000A)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_TLTermEn2TLClken_HSH  (0x061A4780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_OFF (19)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_WID ( 1)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_MSK (0x00080000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_MIN (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_MAX (1) // 0x00000001
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_DEF (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEnonLP4_HSH (0x01264780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_OFF (20)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_WID ( 1)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_MSK (0x00100000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_MIN (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_MAX (1) // 0x00000001
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_DEF (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_LVRPwrGoodQualTLTermEn_HSH (0x01284780)

  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_OFF          (21)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_WID          (11)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_MSK          (0xFFE00000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_MIN          (0)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_MAX          (2047) // 0x000007FF
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_DEF          (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_reserved_HSH          (0x0B2A4780)

#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP1_REG                     (0x00004784)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP2_REG                     (0x00004788)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP3_REG                     (0x0000478C)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_REG                            (0x00004790)
#define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DEF                            (0x00000000)
#define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_VOLATILE_BITFIELDS_MSK         (0x000001FC)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_OFF        ( 0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_WID        ( 2)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_MSK        (0x00000003)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_MIN        (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_MAX        (3) // 0x00000003
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_DEF        (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusIn_HSH        (0x02004790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_OFF       ( 2)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_WID       ( 2)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_MSK       (0x0000000C)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_MIN       (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_MAX       (3) // 0x00000003
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_DEF       (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PwrGoodPODBonusOut_HSH       (0x02844790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_OFF           ( 4)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_WID           ( 3)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_MSK           (0x00000070)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_MIN           (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_MAX           (7) // 0x00000007
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_DEF           (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_MEMGPGBonusOut_HSH           (0x03884790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_OFF           ( 7)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_WID           ( 2)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_MSK           (0x00000180)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_MIN           (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_MAX           (3) // 0x00000003
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_DEF           (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_DD2GPGBonusOut_HSH           (0x028E4790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_OFF            ( 9)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_WID            ( 2)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_MSK            (0x00000600)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_MIN            (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_MAX            (3) // 0x00000003
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_DEF            (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_PGTermBonusIn_HSH            (0x02124790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_OFF (11)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_WID ( 6)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_MSK (0x0001F800)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_MIN (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_MAX (63) // 0x0000003F
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_DEF (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Ddr_reg_clken_ovrd_ddrpg_HSH (0x06164790)

  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_OFF                    (17)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_WID                    (15)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_MSK                    (0xFFFE0000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_MIN                    (0)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_MAX                    (32767) // 0x00007FFF
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_DEF                    (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_Spare_HSH                    (0x0F224790)

#define DDRVCCCLK_SBMEM0_SB_DEBUG_REG                                  (0x00004794)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRVCCCLK_SBMEM0_CR_SPARE_REG                                  (0x00004798)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP0_REG                     (0x00004800)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP1_REG                     (0x00004804)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP2_REG                     (0x00004808)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM1_CR_DDRRPTCHCRMISC_WP3_REG                     (0x0000480C)
//Duplicate of DDRVCCCLK_SBMEM0_CR_DDRRPTCHCRMISC_WP0_REG

#define DDRVCCCLK_SBMEM1_CR_PMMISC_CTRL_REG                            (0x00004810)
//Duplicate of DDRVCCCLK_SBMEM0_CR_PMMISC_CTRL_REG

#define DDRVCCCLK_SBMEM1_SB_DEBUG_REG                                  (0x00004814)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRVCCCLK_SBMEM1_CR_SPARE_REG                                  (0x00004818)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_REG                         (0x0000479C)
#define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DEF                         (0x00000000)
#define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_VOLATILE_BITFIELDS_MSK      (0x00000003)

  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_OFF    ( 0)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_WID    ( 2)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_MSK    (0x00000003)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_MIN    (0)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_MAX    (3) // 0x00000003
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_DEF    (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_DD2GPGBonusOut_PG5_HSH    (0x0280479C)

  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_OFF              ( 2)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_WID              (30)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_MSK              (0xFFFFFFFC)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_MIN              (0)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_MAX              (1073741823) // 0x3FFFFFFF
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_DEF              (0x00000000)
  #define DDRVCCCLK_SBMEM0_CR_BONUSMISC_CTRL_reserved_HSH              (0x1E04479C)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG             (0x00004900)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_DEF             (0x0222E346)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_VOLATILE_BITFIELDS_MSK (0x800001FF)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_OFF    ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_WID    ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_MSK    (0x000001FF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_MAX    (511) // 0x000001FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_DEF    (0x00000146)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Target_HSH    (0x09804900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_OFF ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_MSK (0x00000200)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_DEF (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_RemoteSense_HSH (0x01124900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_OFF (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_MSK (0x00000400)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_OCDivider_HSH (0x01144900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_OFF    (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_MSK    (0x00000800)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Bypass_HSH    (0x01164900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_OFF   (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_MSK   (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_Disable_HSH   (0x01184900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_OFF   (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_WID   ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_MSK   (0x0001E000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_MAX   (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_DEF   (0x00000007)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_BiasAdj_HSH   (0x041A4900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_OFF (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_WID ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_MSK (0x001E0000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_MAX (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_DEF (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj1_HSH (0x04224900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_OFF (21)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_WID ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_MSK (0x03E00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_MAX (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_DEF (0x00000011)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_AdaptiveZeroAdj2_HSH (0x052A4900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_OFF (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_MSK (0x0C000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LockTimer_HSH (0x02344900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_OFF (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_MSK (0x10000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PMBiasAdj_HSH (0x01384900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_OFF     (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_WID     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_MSK     (0x60000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_MAX     (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_LvrPM_HSH     (0x023A4900)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_OFF (31)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_MSK (0x80000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_PwrGoodFB_HSH (0x01BE4900)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP1_REG             (0x00004904)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP2_REG             (0x00004908)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP3_REG             (0x0000490C)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOGCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG             (0x00004910)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_DEF             (0x0222E346)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_VOLATILE_BITFIELDS_MSK (0x800001FF)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_OFF    ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_WID    ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_MSK    (0x000001FF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_MAX    (511) // 0x000001FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_DEF    (0x00000146)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Target_HSH    (0x09804910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_OFF ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_MSK (0x00000200)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_DEF (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_RemoteSense_HSH (0x01124910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_OFF (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_MSK (0x00000400)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_OCDivider_HSH (0x01144910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_OFF    (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_MSK    (0x00000800)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Bypass_HSH    (0x01164910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_OFF   (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_MSK   (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_Disable_HSH   (0x01184910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_OFF   (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_WID   ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_MSK   (0x0001E000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_MAX   (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_DEF   (0x00000007)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_BiasAdj_HSH   (0x041A4910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_OFF (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_WID ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_MSK (0x001E0000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_MAX (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_DEF (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj1_HSH (0x04224910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_OFF (21)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_WID ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_MSK (0x03E00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_MAX (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_DEF (0x00000011)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_AdaptiveZeroAdj2_HSH (0x052A4910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_OFF (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_MSK (0x0C000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LockTimer_HSH (0x02344910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_OFF (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_MSK (0x10000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PMBiasAdj_HSH (0x01384910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_OFF     (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_WID     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_MSK     (0x60000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_MAX     (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_LvrPM_HSH     (0x023A4910)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_OFF (31)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_MSK (0x80000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_PwrGoodFB_HSH (0x01BE4910)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP1_REG             (0x00004914)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP2_REG             (0x00004918)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP3_REG             (0x0000491C)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCDDQCONTROL_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP0_REG                    (0x00004920)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP1_REG                    (0x00004924)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP2_REG                    (0x00004928)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG_WP3_REG                    (0x0000492C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_REG                   (0x00004930)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_DEF                   (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_VOLATILE_BITFIELDS_MSK (0x3FFFFFE1)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_OFF         ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_MSK         (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_RunBusy_HSH         (0x01804930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_OFF          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_WID          ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_MSK          (0x00000006)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_MAX          (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_SelLVR_HSH          (0x02024930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_OFF           ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_WID           ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_MSK           (0x00000018)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_MIN           (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_MAX           (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_DEF           (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Delay_HSH           (0x02064930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_OFF     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_MSK     (0x000003E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_MAX     (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop0_HSH     (0x058A4930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_OFF     (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_MSK     (0x00007C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_MAX     (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_StatusChop1_HSH     (0x05944930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_OFF       (15)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_WID       ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MSK       (0x000F8000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MIN       (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_MAX       (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_DEF       (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOG_HSH       (0x859E4930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_OFF     (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_MSK     (0x01F00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_MIN     (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_MAX     (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetIOGGV_HSH     (0x85A84930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_OFF      (25)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_WID      ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_MSK      (0x3E000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_MIN      (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_MAX      (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_DEF      (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_OffsetDist_HSH      (0x85B24930)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_OFF           (30)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_WID           ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_MSK           (0xC0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_MIN           (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_MAX           (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_DEF           (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRLVRAUTOTRIM_Spare_HSH           (0x023C4930)

#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG                         (0x00004934)
#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DEF                         (0x57802010)
#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VOLATILE_BITFIELDS_MSK      (0x0FF00000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_OFF             ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_WID             ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_MSK             (0x000001FF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_MAX             (511) // 0x000001FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_DEF             (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_QClkRatio_HSH             (0x09004934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_OFF           ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_WID           ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_MSK           (0x0003FE00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_MIN           (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_MAX           (511) // 0x000001FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_DEF           (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_AuxClkRatio_HSH           (0x09124934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_OFF                 (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_WID                 ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_MSK                 (0x00040000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_MIN                 (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_MAX                 (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_DEF                 (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Gear4_HSH                 (0x01244934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_OFF (19)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_MSK (0x00080000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRSelVdd2Ladder_HSH (0x01264934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_OFF       (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_WID       ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_MSK       (0x0FF00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_MAX       (255) // 0x000000FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_DEF       (0x00000078)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_DISTGLVRVrefSel_HSH       (0x08A84934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_OFF       (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_WID       ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_MSK       (0x70000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_MAX       (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_DEF       (0x00000005)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_VddqCompVrefSel_HSH       (0x03384934)

  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_OFF                 (31)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_WID                 ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_MSK                 (0x80000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_MIN                 (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_MAX                 (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_DEF                 (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_Spare_HSH                 (0x013E4934)

#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT1_REG                         (0x00004938)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT2_REG                         (0x0000493C)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT3_REG                         (0x00004940)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_WORKPOINT0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_REG                     (0x00004944)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_DEF                     (0xDA6BB9F4)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_OFF      ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_WID      (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_MSK      (0x000003FF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_MAX      (1023) // 0x000003FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_DEF      (0x000001F4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_ColdLockTime_HSH      (0x0A004944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_OFF        (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_MSK        (0x00000C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_DEF        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockSource_HSH        (0x02144944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_OFF        (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_MSK        (0x00003000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_DEF        (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLLockSel_HSH        (0x02184944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_OFF     (14)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_WID     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_MSK     (0x0000C000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_MAX     (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_DEF     (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_IntPLLLockSel_HSH     (0x021C4944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_OFF  (16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_WID  ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_MSK  (0x00030000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_MAX  (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_DEF  (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_StatusPLLLockSel_HSH  (0x02204944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_OFF        (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_MSK        (0x000C0000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_DEF        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountF_HSH        (0x02244944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_OFF      (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_WID      ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_MSK      (0x00300000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_MAX      (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_DEF      (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockCountP_M_HSH      (0x02284944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_OFF        (22)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_WID        ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_MSK        (0x03C00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_MAX        (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_DEF        (0x00000009)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockWindow_HSH        (0x042C4944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_OFF         (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_MSK         (0x04000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_LockPhyST_HSH         (0x01344944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_OFF    (27)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_MSK    (0x08000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_DEF    (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_FusaFlagUnlock_HSH    (0x01364944)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_OFF          (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_WID          ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_MSK          (0xF0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_MAX          (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_DEF          (0x0000000D)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEVIRTUAL_PLLEnLow_HSH          (0x04384944)

#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG                       (0x00004948)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DEF                       (0x00000349)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_OFF            ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_WID            ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_MSK            (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_MIN            (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_MAX            (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_DEF            (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_DeskewEn_HSH            (0x01004948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_OFF        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_MSK        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalByp_HSH        (0x01024948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_OFF        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_WID        ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_MSK        (0x0000001C)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_MAX        (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_DEF        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCnt_HSH        (0x03044948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_OFF   ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_WID   ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_MSK   (0x000000E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_MAX   (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_DEF   (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalCntEarly_HSH   (0x030A4948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_OFF         ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_MSK         (0x00000100)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_DEF         (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalEn_HSH         (0x01104948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_OFF         ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_MSK         (0x00000200)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_DEF         (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_OffsetCalWr_HSH         (0x01124948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_OFF        (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_WID        (14)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_MSK        (0x00FFFC00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_MAX        (16383) // 0x00003FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved0_HSH        (0x0E144948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_OFF        (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_MSK        (0x03000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_PLLReserved1_HSH        (0x02304948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_OFF           (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_WID           ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_MSK           (0x04000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_MIN           (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_MAX           (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_DEF           (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_AltRefClk_HSH           (0x01344948)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_OFF               (27)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_WID               ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_MSK               (0xF8000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_MIN               (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_MAX               (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_DEF               (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_Spare_HSH               (0x05364948)

#define DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEAUXCLK_REG                      (0x0000494C)
//Duplicate of DDRPHY_DDRCOMP_SBMEM_CR_PLLFUSEPHCLK_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_REG                          (0x00004950)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_DEF                          (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_VOLATILE_BITFIELDS_MSK       (0x03FF03FF)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_OFF             ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_WID             ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_MSK             (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_MAX             (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_DEF             (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkPLLEn_HSH             (0x01804950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_OFF              ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_WID              ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_MSK              (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_MIN              (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_MAX              (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_DEF              (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLock_HSH              (0x01824950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_OFF        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_MSK        (0x00000004)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFilterLock_HSH        (0x01844950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_OFF          ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_MSK          (0x00000008)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateLock_HSH          (0x01864950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_OFF        ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_MSK        (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyLock_HSH        (0x01884950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_OFF     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_MSK     (0x000003E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_MAX     (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLockFallCount_HSH     (0x058A4950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_OFF (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_MSK (0x00000400)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkOffsetCalLock_spare_HSH (0x01144950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_OFF (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_MSK (0x00000800)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkLateCalLock_spare_HSH (0x01164950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_MSK (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkStickyCalLock_spare_HSH (0x01184950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_OFF (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_WID ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_MSK (0x0000E000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_MAX (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkCalLockFallCount_spare_HSH (0x031A4950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_OFF         (16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_MSK         (0x00010000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_PhClkFinalLock_HSH         (0x01A04950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_OFF            (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_WID            ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_MSK            (0x00020000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_MIN            (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_MAX            (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_DEF            (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkPLLEn_HSH            (0x01A24950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_OFF             (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_WID             ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_MSK             (0x00040000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_MAX             (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_DEF             (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLock_HSH             (0x01A44950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_OFF         (19)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_MSK         (0x00080000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLateLock_HSH         (0x01A64950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_OFF    (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_WID    ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_MSK    (0x01F00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_MAX    (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkLockFallCount_HSH    (0x05A84950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_OFF        (25)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_MSK        (0x02000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_AuxClkFinalLock_HSH        (0x01B24950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_OFF           (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_WID           ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_MSK           (0x04000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_MIN           (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_MAX           (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_DEF           (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLock_spare_HSH           (0x01344950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_OFF       (27)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_WID       ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_MSK       (0x08000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_MAX       (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_DEF       (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLateLock_spare_HSH       (0x01364950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_OFF     (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_WID     ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_MSK     (0x10000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_MAX     (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLStickyLock_spare_HSH     (0x01384950)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_OFF  (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_WID  ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_MSK  (0xE0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_MAX  (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLSTATUS_TLLockFallCount_spare_HSH  (0x033A4950)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_REG                       (0x00004954)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_DEF                       (0x004C8800)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_VOLATILE_BITFIELDS_MSK    (0x0000F000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_OFF         ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_WID         ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_MSK         (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_MAX         (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClock_HSH         (0x01004954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_OFF      ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_WID      ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_MSK      (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_MAX      (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_DEF      (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdPhClkPLLEn_HSH      (0x01024954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_OFF        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_MSK        (0x00000004)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkDTestEn_HSH        (0x01044954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_OFF    ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_MSK    (0x00000008)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkForceBypass_HSH    (0x01064954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_OFF  ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_WID  ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_MSK  (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_MAX  (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PHClkOffsetFastCal_HSH  (0x01084954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_OFF        ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_MSK        (0x00000020)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxClock_HSH        (0x010A4954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_OFF        ( 6)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_MSK        (0x00000040)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_OvrdAuxPLLEn_HSH        (0x010C4954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_OFF          ( 7)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_MSK          (0x00000080)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxDTestEn_HSH          (0x010E4954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_OFF      ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_WID      ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_MSK      (0x00000100)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_MAX      (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_DEF      (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxForceBypass_HSH      (0x01104954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_OFF    ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_MSK    (0x00000200)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_AuxOffsetFastCal_HSH    (0x01124954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_OFF    (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_WID    ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_MSK    (0x00000C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_MAX    (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_DEF    (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_PhClkPLLFeedback_HSH    (0x02144954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_OFF             (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_WID             ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_MSK             (0x0000F000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_MAX             (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_DEF             (0x00000008)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLDelay_HSH             (0x04984954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_OFF          (16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_WID          ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_MSK          (0x00030000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_MAX          (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmStart_HSH          (0x02204954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_OFF             (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_WID             ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_MSK             (0x000C0000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_MAX             (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_DEF             (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmEn_HSH             (0x02244954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_OFF          (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_WID          ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_MSK          (0x00300000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_MAX          (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLFsmCount_HSH          (0x02284954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_OFF          (22)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_MSK          (0x00400000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_DEF          (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_TLRstDelay_HSH          (0x012C4954)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_OFF               (23)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_WID               ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_MSK               (0xFF800000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_MIN               (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_MAX               (511) // 0x000001FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_DEF               (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPPLL_Spare_HSH               (0x092E4954)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_REG                 (0x00004958)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_DEF                 (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal2_spare_HSH (0x0C004958)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_OffsetCal3_spare_HSH (0x0C184958)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_OFF          (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_WID          ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_MSK          (0xFF000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_MAX          (255) // 0x000000FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL1_rsvd_HSH          (0x08304958)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_REG                 (0x0000495C)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_DEF                 (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal0_spare_HSH (0x0C00495C)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_OffsetCal1_spare_HSH (0x0C18495C)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_OFF          (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_WID          ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_MSK          (0xFF000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_MAX          (255) // 0x000000FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPQCLK0CAL0_rsvd_HSH          (0x0830495C)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_REG               (0x00004960)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_DEF               (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_VOLATILE_BITFIELDS_MSK (0x1F000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal2_spare_HSH (0x0C004960)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_OffsetCal3_spare_HSH (0x0C184960)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_OFF    (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_WID    ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_MSK    (0x1F000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_MIN    (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_MAX    (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_QClkDCC3_HSH    (0x85B04960)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_OFF        (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_WID        ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_MSK        (0xE0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_MAX        (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL3_rsvd_HSH        (0x033A4960)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_REG               (0x00004964)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_DEF               (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_VOLATILE_BITFIELDS_MSK (0x1F000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal0_spare_HSH (0x0C004964)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_OffsetCal1_spare_HSH (0x0C184964)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_OFF    (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_WID    ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_MSK    (0x1F000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_MIN    (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_MAX    (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_QClkDCC2_HSH    (0x85B04964)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_OFF        (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_WID        ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_MSK        (0xE0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_MAX        (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL2_rsvd_HSH        (0x033A4964)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_REG               (0x00004968)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_DEF               (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_VOLATILE_BITFIELDS_MSK (0x1F000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal2_spare_HSH (0x0C004968)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_OffsetCal3_spare_HSH (0x0C184968)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_OFF    (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_WID    ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_MSK    (0x1F000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_MIN    (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_MAX    (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_QClkDCC1_HSH    (0x85B04968)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_OFF        (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_WID        ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_MSK        (0xE0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_MAX        (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL1_rsvd_HSH        (0x033A4968)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_REG               (0x0000496C)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_DEF               (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_VOLATILE_BITFIELDS_MSK (0x1F000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_MSK (0x00000FFF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal0_spare_HSH (0x0C00496C)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_OFF (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_WID (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_MSK (0x00FFF000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_MAX (4095) // 0x00000FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_OffsetCal1_spare_HSH (0x0C18496C)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_OFF    (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_WID    ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_MSK    (0x1F000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_MIN    (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_MAX    (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_QClkDCC0_HSH    (0x85B0496C)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_OFF        (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_WID        ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_MSK        (0xE0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_MAX        (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRCOMPAUXCLK0CAL0_rsvd_HSH        (0x033A496C)

#define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_REG                         (0x00004970)
#define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_DEF                         (0x02660008)
#define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VOLATILE_BITFIELDS_MSK      (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_OFF        ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_MSK        (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGGndRefBypass_HSH        (0x01004970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_OFF            ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_WID            ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_MSK            (0x0000000E)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_MIN            (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_MAX            (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_DEF            (0x00000004)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_BGBiasTrim_HSH            (0x03024970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_OFF            ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_WID            ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_MSK            (0x000001F0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_MIN            (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_MAX            (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_DEF            (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB0Offset_HSH            (0x85084970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_OFF            ( 9)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_WID            ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_MSK            (0x00003E00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_MIN            (-16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_MAX            (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_DEF            (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGB1Offset_HSH            (0x85124970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_OFF        (14)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_WID        ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_MSK        (0x0003C000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_MAX        (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_DEF        (0x00000008)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_UGBRampUpDelay_HSH        (0x041C4970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_OFF          (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_WID          ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_MSK          (0x003C0000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_MAX          (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_DEF          (0x00000009)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccIOSlopeOC_HSH          (0x04244970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_OFF         (22)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_WID         ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_MSK         (0x03C00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_MAX         (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_DEF         (0x00000009)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_VccCLKSlopeOC_HSH         (0x042C4970)

  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_OFF                 (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_WID                 ( 6)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_MSK                 (0xFC000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_MIN                 (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_MAX                 (63) // 0x0000003F
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_DEF                 (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_UGBCONTROL_Spare_HSH                 (0x06344970)

#define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_REG                  (0x00004974)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_DEF                  (0x00000000)
#define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_VOLATILE_BITFIELDS_MSK (0x0C3FFFFF)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_OFF  ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_WID  (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_MSK  (0x000007FF)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_MAX  (2047) // 0x000007FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkLockTime_HSH  (0x0B804974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_OFF (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_WID (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_MSK (0x003FF800)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_MAX (2047) // 0x000007FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLockTime_HSH (0x0B964974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_OFF (22)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_MSK (0x00400000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseEarly_HSH (0x012C4974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_OFF (23)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_MSK (0x00800000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_PHClkPhaseLate_HSH (0x012E4974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_OFF (24)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_MSK (0x01000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseEarly_HSH (0x01304974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_OFF (25)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_MSK (0x02000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkPhaseLate_HSH (0x01324974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_OFF (26)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_MSK (0x04000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaPhPllResetStatus_HSH (0x01B44974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_OFF (27)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_MSK (0x08000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_FusaAuxPllResetStatus_HSH (0x01B64974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_OFF (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_MSK (0x10000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkOffsetCalLock_HSH (0x01384974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_OFF (29)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_MSK (0x20000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkLateCalLock_HSH (0x013A4974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_OFF (30)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_MSK (0x40000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_AuxClkStickyCalLock_HSH (0x013C4974)

  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_OFF          (31)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_MSK          (0x80000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_PLLLOCKTIMESTATUS_Spare_HSH          (0x013E4974)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_REG                  (0x00004978)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_DEF                  (0x1A384202)
#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_VOLATILE_BITFIELDS_MSK (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_OFF      ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_WID      ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_MSK      (0x0000001F)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_MAX      (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_DEF      (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClk_HSH      (0x05004978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_OFF     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_MSK     (0x000003E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_MAX     (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_DEF     (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvNClkb_HSH     (0x050A4978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_OFF      (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_WID      ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_MSK      (0x00007C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_MAX      (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_DEF      (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClk_HSH      (0x05144978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_OFF     (15)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_WID     ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_MSK     (0x000F8000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_MAX     (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_DEF     (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_EnDrvSClkb_HSH     (0x051E4978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_OFF (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_WID ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_MSK (0x01F00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_MAX (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_DEF (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync12In_HSH (0x05284978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_OFF (25)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_WID ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_MSK (0x3E000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_MAX (31) // 0x0000001F
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_DEF (0x0000000D)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_LjpllUsync1Out_HSH (0x05324978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_OFF (30)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_MSK (0x40000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Ljpll_clk_bypass_sel_HSH (0x013C4978)

  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_OFF (31)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_MSK (0x80000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRTLCLKDRVCTRL_Auxpll_clk_bypass_sel_HSH (0x013E4978)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVSSHI_REG                         (0x0000497C)
//Duplicate of DDRCCC_SBMEM0_CR_DDRCRVSSHI_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_REG                          (0x00004980)
#define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DEF                          (0x078000F0)
#define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_VOLATILE_BITFIELDS_MSK       (0x00198C0F)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_OFF       ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_WID       ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_MSK       (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_MAX       (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_DEF       (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_bonus_out_HSH       (0x02804980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_OFF     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_WID     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_MSK     (0x0000000C)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_MAX     (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_bonus_out_HSH     (0x02844980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_OFF ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_MSK (0x00000030)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_DEF (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_en_HSH (0x02084980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_OFF ( 6)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_MSK (0x000000C0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_DEF (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_channel_ovrd_val_HSH (0x020C4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_OFF  ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_WID  ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_MSK  (0x00000300)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_MAX  (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusIn_HSH  (0x02104980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_OFF (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_MSK (0x00000C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_PwrGoodDistParBonusOut_HSH (0x02944980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_OFF   (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_MSK   (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_EnIOGDDQGLpMode4Mask_HSH   (0x01184980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_OFF         (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_WID         ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_MSK         (0x00006000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_MAX         (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusIn_HSH         (0x021A4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_OFF        (15)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_MSK        (0x00018000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_DistLvrBonusOut_HSH        (0x029E4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_OFF          (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_WID          ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_MSK          (0x00060000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_MAX          (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_DEF          (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVrefBonusIn_HSH          (0x02224980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_OFF        (19)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_MSK        (0x00180000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_BGVref0BonusOut_HSH        (0x02A64980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_OFF (21)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_MSK (0x00200000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVRVrefEn_MaskforLp4_HSH (0x012A4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_OFF   (22)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_MSK   (0x00400000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_IOG_LVREn_MaskforLp4_HSH   (0x012C4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_OFF (23)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_MSK (0x01800000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_DEF (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_en_HSH (0x022E4980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_OFF (25)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_WID ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_MSK (0x06000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_MAX (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_DEF (0x00000003)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr40_channel_ovrd_val_HSH (0x02324980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_OFF    (27)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_MSK    (0x08000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_ioglvr_disable_ovrd_HSH    (0x01364980)

  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_OFF                  (28)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_WID                  ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_MSK                  (0xF0000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_MIN                  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_MAX                  (15) // 0x0000000F
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_DEF                  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_MISCBONUS_Spare_HSH                  (0x04384980)

#define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_REG                       (0x00004984)
#define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_DEF                       (0x00008202)
#define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_VOLATILE_BITFIELDS_MSK    (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_OFF   ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_MSK   (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_disable_ringosc_h_HSH   (0x01004984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_OFF          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_MSK          (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_DEF          (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_enable_HSH          (0x01024984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_OFF       ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_WID       ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_MSK       (0x0000000C)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_MAX       (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_DEF       (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pwrup_clk_sel_HSH       (0x02044984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_OFF      ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_WID      ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_MSK      (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_MAX      (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_DEF      (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_bypass_clk_sel_HSH      (0x01084984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_OFF ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_WID ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_MSK (0x000000E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_MAX (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_ratio_scaling_factor_HSH (0x030A4984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_OFF        ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_WID        ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_MSK        (0x00000300)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_MAX        (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_DEF        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_fdbk_src_sel_HSH        (0x02104984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_OFF         (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_WID         ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_MSK         (0x00000C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_MIN         (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_MAX         (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_DEF         (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_dcc_src_sel_HSH         (0x02144984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_OFF    (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_MSK    (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_pll_refclk_valid_HSH    (0x01184984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_OFF    (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_MSK    (0x00002000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_int_HSH    (0x011A4984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_OFF    (14)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_MSK    (0x00004000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_finallocksel_ext_HSH    (0x011C4984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_OFF          (15)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_WID          ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_MSK          (0x00008000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_MIN          (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_MAX          (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_DEF          (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_clkout3_en_HSH          (0x011E4984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_OFF (16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_MSK (0x00010000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_backuppll_qclk_lock_HSH (0x01204984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_OFF (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_MSK (0x00020000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_early_lock_selection_HSH (0x01224984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_OFF   (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_WID   ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_MSK   (0x00040000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_MIN   (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_MAX   (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_DEF   (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_keep_regulator_on_HSH   (0x01244984)

  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_OFF               (19)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_WID               (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_MSK               (0xFFF80000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_MIN               (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_MAX               (8191) // 0x00001FFF
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_DEF               (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_QCLKPLLSTRAP_spare_HSH               (0x0D264984)

#define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_REG                     (0x00004988)
#define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_DEF                     (0x00008002)
#define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_VOLATILE_BITFIELDS_MSK  (0x00000000)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_OFF ( 0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_MSK (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_disable_ringosc_h_HSH (0x01004988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_OFF        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_MSK        (0x00000002)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_DEF        (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_enable_HSH        (0x01024988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_OFF     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_WID     ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_MSK     (0x0000000C)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_MIN     (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_MAX     (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_DEF     (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pwrup_clk_sel_HSH     (0x02044988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_OFF    ( 4)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_WID    ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_MSK    (0x00000010)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_MIN    (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_MAX    (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_DEF    (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_bypass_clk_sel_HSH    (0x01084988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_OFF ( 5)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_WID ( 3)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_MSK (0x000000E0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_MAX (7) // 0x00000007
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_ratio_scaling_factor_HSH (0x030A4988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_OFF      ( 8)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_WID      ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_MSK      (0x00000300)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_MIN      (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_MAX      (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_DEF      (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_fdbk_src_sel_HSH      (0x02104988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_OFF       (10)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_WID       ( 2)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_MSK       (0x00000C00)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_MIN       (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_MAX       (3) // 0x00000003
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_DEF       (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_dcc_src_sel_HSH       (0x02144988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_OFF  (12)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_WID  ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_MSK  (0x00001000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_MAX  (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_pll_refclk_valid_HSH  (0x01184988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_OFF  (13)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_WID  ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_MSK  (0x00002000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_MAX  (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_int_HSH  (0x011A4988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_OFF  (14)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_WID  ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_MSK  (0x00004000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_MAX  (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_finallocksel_ext_HSH  (0x011C4988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_OFF        (15)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_MSK        (0x00008000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_DEF        (0x00000001)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_clkout3_en_HSH        (0x011E4988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_OFF (16)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_MSK (0x00010000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_backuppll_auxclk_lock_HSH (0x01204988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_OFF (17)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_MSK (0x00020000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_keep_regulator_on_HSH (0x01224988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_OFF        (18)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_WID        ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_MSK        (0x00040000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_MIN        (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_MAX        (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_DEF        (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxclkgear_HSH        (0x01244988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_OFF  (19)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_WID  ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_MSK  (0x00080000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_MIN  (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_MAX  (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_DEF  (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_HSH  (0x01264988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_OFF (20)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_WID ( 1)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_MSK (0x00100000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_MIN (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_MAX (1) // 0x00000001
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_DEF (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_auxpll_lock_ovrd_en_HSH (0x01284988)

  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_OFF             (21)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_WID             (11)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_MSK             (0xFFE00000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_MIN             (0)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_MAX             (2047) // 0x000007FF
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_DEF             (0x00000000)
  #define DDRPHY_DDRCOMP_SBMEM_CR_AUXCLKPLLSTRAP_spare_HSH             (0x0B2A4988)

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP0_REG                  (0x0000498C)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP1_REG                  (0x00004990)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP2_REG                  (0x00004994)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_DDRCRVCCIOG40_WP3_REG                  (0x00004998)
//Duplicate of DDRDATA_SBMEM0_CR_DDRCRVCCIOG_WP0_REG

#define DDRPHY_DDRCOMP_SBMEM_SB_DEBUG_REG                              (0x0000499C)
//Duplicate of MCMISCSSAUG_SB_DEBUG_REG

#define DDRPHY_DDRCOMP_SBMEM_CR_SPARE_REG                              (0x000049A0)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRPMSTC_PCMD_CTRL0_REG                                        (0x00004D00)
#define DDRPMSTC_PCMD_CTRL0_DEF                                        (0x00000000)
#define DDRPMSTC_PCMD_CTRL0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define DDRPMSTC_PCMD_CTRL0_CMD0_OFF                                 ( 0)
  #define DDRPMSTC_PCMD_CTRL0_CMD0_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD0_MSK                                 (0x0000000F)
  #define DDRPMSTC_PCMD_CTRL0_CMD0_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD0_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD0_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD0_HSH                                 (0x04004D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD1_OFF                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD1_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD1_MSK                                 (0x000000F0)
  #define DDRPMSTC_PCMD_CTRL0_CMD1_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD1_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD1_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD1_HSH                                 (0x04084D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD2_OFF                                 ( 8)
  #define DDRPMSTC_PCMD_CTRL0_CMD2_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD2_MSK                                 (0x00000F00)
  #define DDRPMSTC_PCMD_CTRL0_CMD2_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD2_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD2_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD2_HSH                                 (0x04104D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD3_OFF                                 (12)
  #define DDRPMSTC_PCMD_CTRL0_CMD3_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD3_MSK                                 (0x0000F000)
  #define DDRPMSTC_PCMD_CTRL0_CMD3_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD3_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD3_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD3_HSH                                 (0x04184D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD4_OFF                                 (16)
  #define DDRPMSTC_PCMD_CTRL0_CMD4_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD4_MSK                                 (0x000F0000)
  #define DDRPMSTC_PCMD_CTRL0_CMD4_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD4_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD4_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD4_HSH                                 (0x04204D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD5_OFF                                 (20)
  #define DDRPMSTC_PCMD_CTRL0_CMD5_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD5_MSK                                 (0x00F00000)
  #define DDRPMSTC_PCMD_CTRL0_CMD5_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD5_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD5_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD5_HSH                                 (0x04284D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD6_OFF                                 (24)
  #define DDRPMSTC_PCMD_CTRL0_CMD6_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD6_MSK                                 (0x0F000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD6_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD6_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD6_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD6_HSH                                 (0x04304D00)

  #define DDRPMSTC_PCMD_CTRL0_CMD7_OFF                                 (28)
  #define DDRPMSTC_PCMD_CTRL0_CMD7_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL0_CMD7_MSK                                 (0xF0000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD7_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL0_CMD7_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL0_CMD7_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL0_CMD7_HSH                                 (0x04384D00)

#define DDRPMSTC_PCMD_CTRL1_REG                                        (0x00004D04)
#define DDRPMSTC_PCMD_CTRL1_DEF                                        (0x00000000)
#define DDRPMSTC_PCMD_CTRL1_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define DDRPMSTC_PCMD_CTRL1_CMD0_OFF                                 ( 0)
  #define DDRPMSTC_PCMD_CTRL1_CMD0_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD0_MSK                                 (0x0000000F)
  #define DDRPMSTC_PCMD_CTRL1_CMD0_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD0_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD0_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD0_HSH                                 (0x04004D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD1_OFF                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD1_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD1_MSK                                 (0x000000F0)
  #define DDRPMSTC_PCMD_CTRL1_CMD1_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD1_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD1_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD1_HSH                                 (0x04084D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD2_OFF                                 ( 8)
  #define DDRPMSTC_PCMD_CTRL1_CMD2_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD2_MSK                                 (0x00000F00)
  #define DDRPMSTC_PCMD_CTRL1_CMD2_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD2_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD2_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD2_HSH                                 (0x04104D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD3_OFF                                 (12)
  #define DDRPMSTC_PCMD_CTRL1_CMD3_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD3_MSK                                 (0x0000F000)
  #define DDRPMSTC_PCMD_CTRL1_CMD3_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD3_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD3_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD3_HSH                                 (0x04184D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD4_OFF                                 (16)
  #define DDRPMSTC_PCMD_CTRL1_CMD4_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD4_MSK                                 (0x000F0000)
  #define DDRPMSTC_PCMD_CTRL1_CMD4_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD4_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD4_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD4_HSH                                 (0x04204D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD5_OFF                                 (20)
  #define DDRPMSTC_PCMD_CTRL1_CMD5_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD5_MSK                                 (0x00F00000)
  #define DDRPMSTC_PCMD_CTRL1_CMD5_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD5_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD5_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD5_HSH                                 (0x04284D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD6_OFF                                 (24)
  #define DDRPMSTC_PCMD_CTRL1_CMD6_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD6_MSK                                 (0x0F000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD6_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD6_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD6_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD6_HSH                                 (0x04304D04)

  #define DDRPMSTC_PCMD_CTRL1_CMD7_OFF                                 (28)
  #define DDRPMSTC_PCMD_CTRL1_CMD7_WID                                 ( 4)
  #define DDRPMSTC_PCMD_CTRL1_CMD7_MSK                                 (0xF0000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD7_MIN                                 (0)
  #define DDRPMSTC_PCMD_CTRL1_CMD7_MAX                                 (15) // 0x0000000F
  #define DDRPMSTC_PCMD_CTRL1_CMD7_DEF                                 (0x00000000)
  #define DDRPMSTC_PCMD_CTRL1_CMD7_HSH                                 (0x04384D04)

#define DDRPMSTC_PCMDPATTERN_CTRL0_REG                                 (0x00004D08)
#define DDRPMSTC_PCMDPATTERN_CTRL0_DEF                                 (0x00000000)
#define DDRPMSTC_PCMDPATTERN_CTRL0_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_OFF                    ( 0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_WID                    (16)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_MSK                    (0x0000FFFF)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_MIN                    (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_MAX                    (65535) // 0x0000FFFF
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_DEF                    (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_TotalCount_HSH                    (0x10004D08)

  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_OFF               (16)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_WID               ( 1)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_MSK               (0x00010000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_MIN               (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_MAX               (1) // 0x00000001
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_DEF               (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_CONTINUOUS_MODE_HSH               (0x01204D08)

  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_OFF                      (17)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_WID                      ( 3)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_MSK                      (0x000E0000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_MIN                      (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_MAX                      (7) // 0x00000007
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_DEF                      (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_RUN_MODE_HSH                      (0x03224D08)

  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_OFF                        (20)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_WID                        ( 1)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_MSK                        (0x00100000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_MIN                        (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_MAX                        (1) // 0x00000001
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_DEF                        (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_LFSREn_HSH                        (0x01284D08)

  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_OFF                         (21)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_WID                         (11)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_MSK                         (0xFFE00000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_MIN                         (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_MAX                         (2047) // 0x000007FF
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_DEF                         (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL0_spare_HSH                         (0x0B2A4D08)

#define DDRPMSTC_PCMDPATTERN_CTRL1_REG                                 (0x00004D0C)
#define DDRPMSTC_PCMDPATTERN_CTRL1_DEF                                 (0x04000000)
#define DDRPMSTC_PCMDPATTERN_CTRL1_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_OFF               ( 0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_WID               (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_MSK               (0x00000FFF)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_MIN               (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_MAX               (4095) // 0x00000FFF
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_DEF               (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue0_HSH               (0x0C004D0C)

  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_OFF                      (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_WID                      ( 2)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_MSK                      (0x00003000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_MIN                      (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_MAX                      (3) // 0x00000003
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_DEF                      (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel0_HSH                      (0x02184D0C)

  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_OFF               (14)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_WID               (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_MSK               (0x03FFC000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_MIN               (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_MAX               (4095) // 0x00000FFF
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_DEF               (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRStartValue1_HSH               (0x0C1C4D0C)

  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_OFF                      (26)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_WID                      ( 2)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_MSK                      (0x0C000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_MIN                      (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_MAX                      (3) // 0x00000003
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_DEF                      (0x00000001)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_LFSRSel1_HSH                      (0x02344D0C)

  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_OFF                         (28)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_WID                         ( 4)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_MSK                         (0xF0000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_MIN                         (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_MAX                         (15) // 0x0000000F
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_DEF                         (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL1_spare_HSH                         (0x04384D0C)

#define DDRPMSTC_PCMDPATTERN_CTRL2_REG                                 (0x00004D10)
#define DDRPMSTC_PCMDPATTERN_CTRL2_DEF                                 (0x0C002000)
#define DDRPMSTC_PCMDPATTERN_CTRL2_VOLATILE_BITFIELDS_MSK              (0x00000000)

  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_OFF               ( 0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_WID               (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_MSK               (0x00000FFF)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_MIN               (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_MAX               (4095) // 0x00000FFF
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_DEF               (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue2_HSH               (0x0C004D10)

  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_OFF                      (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_WID                      ( 2)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_MSK                      (0x00003000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_MIN                      (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_MAX                      (3) // 0x00000003
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_DEF                      (0x00000002)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel2_HSH                      (0x02184D10)

  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_OFF               (14)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_WID               (12)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_MSK               (0x03FFC000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_MIN               (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_MAX               (4095) // 0x00000FFF
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_DEF               (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRStartValue3_HSH               (0x0C1C4D10)

  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_OFF                      (26)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_WID                      ( 2)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_MSK                      (0x0C000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_MIN                      (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_MAX                      (3) // 0x00000003
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_DEF                      (0x00000003)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_LFSRSel3_HSH                      (0x02344D10)

  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_OFF                         (28)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_WID                         ( 4)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_MSK                         (0xF0000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_MIN                         (0)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_MAX                         (15) // 0x0000000F
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_DEF                         (0x00000000)
  #define DDRPMSTC_PCMDPATTERN_CTRL2_spare_HSH                         (0x04384D10)

#define DDRPMSTC_RUN_CTRL0_REG                                         (0x00004D14)
#define DDRPMSTC_RUN_CTRL0_DEF                                         (0x00000000)
#define DDRPMSTC_RUN_CTRL0_VOLATILE_BITFIELDS_MSK                      (0x00000001)

  #define DDRPMSTC_RUN_CTRL0_RUN_OFF                                   ( 0)
  #define DDRPMSTC_RUN_CTRL0_RUN_WID                                   ( 1)
  #define DDRPMSTC_RUN_CTRL0_RUN_MSK                                   (0x00000001)
  #define DDRPMSTC_RUN_CTRL0_RUN_MIN                                   (0)
  #define DDRPMSTC_RUN_CTRL0_RUN_MAX                                   (1) // 0x00000001
  #define DDRPMSTC_RUN_CTRL0_RUN_DEF                                   (0x00000000)
  #define DDRPMSTC_RUN_CTRL0_RUN_HSH                                   (0x01804D14)

  #define DDRPMSTC_RUN_CTRL0_spare_OFF                                 ( 1)
  #define DDRPMSTC_RUN_CTRL0_spare_WID                                 (31)
  #define DDRPMSTC_RUN_CTRL0_spare_MSK                                 (0xFFFFFFFE)
  #define DDRPMSTC_RUN_CTRL0_spare_MIN                                 (0)
  #define DDRPMSTC_RUN_CTRL0_spare_MAX                                 (2147483647) // 0x7FFFFFFF
  #define DDRPMSTC_RUN_CTRL0_spare_DEF                                 (0x00000000)
  #define DDRPMSTC_RUN_CTRL0_spare_HSH                                 (0x1F024D14)

#define DDRPMSTC_DEBUG_CTRL0_REG                                       (0x00004D18)
#define DDRPMSTC_DEBUG_CTRL0_DEF                                       (0x00000000)
#define DDRPMSTC_DEBUG_CTRL0_VOLATILE_BITFIELDS_MSK                    (0x0003FAAA)

  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_OFF           ( 0)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_WID           ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_MSK           (0x00000001)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_MIN           (0)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_MAX           (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_DEF           (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_EN_HSH           (0x01004D18)

  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_OFF          ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_WID          ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_MSK          (0x00000002)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_MIN          (0)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_MAX          (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_DEF          (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_MASTERCTRL_SINGLE_STEP_ADV_HSH          (0x01824D18)

  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_OFF               ( 2)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_WID               ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_MSK               (0x00000004)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_MIN               (0)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_MAX               (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_DEF               (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_EN_HSH               (0x01044D18)

  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_OFF              ( 3)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_WID              ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_MSK              (0x00000008)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_MIN              (0)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_MAX              (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_DEF              (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_CSTATE_SINGLE_STEP_ADV_HSH              (0x01864D18)

  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_OFF                 ( 4)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_WID                 ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_MSK                 (0x00000010)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_MIN                 (0)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_MAX                 (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_DEF                 (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_EN_HSH                 (0x01084D18)

  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_OFF                ( 5)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_WID                ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_MSK                (0x00000020)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_MIN                (0)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_MAX                (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_DEF                (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_SAGV_SINGLE_STEP_ADV_HSH                (0x018A4D18)

  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_OFF                ( 6)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_WID                ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_MSK                (0x00000040)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_MIN                (0)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_MAX                (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_DEF                (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_EN_HSH                (0x010C4D18)

  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_OFF               ( 7)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_WID               ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_MSK               (0x00000080)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_MIN               (0)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_MAX               (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_DEF               (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_XCTRL_SINGLE_STEP_ADV_HSH               (0x018E4D18)

  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_OFF               ( 8)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_WID               ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_MSK               (0x00000100)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_MIN               (0)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_MAX               (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_DEF               (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_EN_HSH               (0x01104D18)

  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_OFF              ( 9)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_WID              ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_MSK              (0x00000200)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_MIN              (0)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_MAX              (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_DEF              (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_UPCTRL_SINGLE_STEP_ADV_HSH              (0x01924D18)

  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_OFF               (10)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_WID               ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_MSK               (0x00000400)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_MIN               (0)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_MAX               (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_DEF               (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_EN_HSH               (0x01144D18)

  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_OFF              (11)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_WID              ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_MSK              (0x00000800)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_MIN              (0)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_MAX              (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_DEF              (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_LPCTRL_SINGLE_STEP_ADV_HSH              (0x01964D18)

  #define DDRPMSTC_DEBUG_CTRL0_FSMState_OFF                            (12)
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_WID                            ( 6)
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_MSK                            (0x0003F000)
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_MIN                            (0)
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_MAX                            (63) // 0x0000003F
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_DEF                            (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_FSMState_HSH                            (0x06984D18)

  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_OFF                              (18)
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_WID                              ( 3)
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_MSK                              (0x001C0000)
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_MIN                              (0)
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_MAX                              (7) // 0x00000007
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_DEF                              (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_FSMSel_HSH                              (0x03244D18)

  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_OFF                   (21)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_WID                   ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_MSK                   (0x00200000)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_MIN                   (0)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_MAX                   (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_DEF                   (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp3_HSH                   (0x012A4D18)

  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_OFF                   (22)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_WID                   ( 1)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_MSK                   (0x00400000)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_MIN                   (0)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_MAX                   (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_DEF                   (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_dislpctrlasynclp4_HSH                   (0x012C4D18)

  #define DDRPMSTC_DEBUG_CTRL0_RSVD_OFF                                (23)
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_WID                                ( 9)
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_MSK                                (0xFF800000)
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_MIN                                (0)
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_MAX                                (511) // 0x000001FF
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_DEF                                (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL0_RSVD_HSH                                (0x092E4D18)

#define DDRPMSTC_DEBUG_CTRL1_REG                                       (0x00004D1C)
#define DDRPMSTC_DEBUG_CTRL1_DEF                                       (0x00000000)
#define DDRPMSTC_DEBUG_CTRL1_VOLATILE_BITFIELDS_MSK                    (0x06000000)

  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_OFF                         ( 0)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_WID                         (24)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_MSK                         (0x00FFFFFF)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_MIN                         (0)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_MAX                         (16777215) // 0x00FFFFFF
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_DEF                         (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_HSH                         (0x18004D1C)

  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_OFF                      (24)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_WID                      ( 1)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_MSK                      (0x01000000)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_MIN                      (0)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_MAX                      (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_DEF                      (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_En_HSH                      (0x01304D1C)

  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_OFF                     (25)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_WID                     ( 1)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_MSK                     (0x02000000)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_MIN                     (0)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_MAX                     (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_DEF                     (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL1_BreakPoint0_Hit_HSH                     (0x01B24D1C)

  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_OFF                      (26)
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_WID                      ( 1)
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_MSK                      (0x04000000)
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_MIN                      (0)
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_MAX                      (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_DEF                      (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL1_BREAKPOINT_ADV_HSH                      (0x01B44D1C)

  #define DDRPMSTC_DEBUG_CTRL1_RSVD_OFF                                (27)
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_WID                                ( 5)
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_MSK                                (0xF8000000)
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_MIN                                (0)
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_MAX                                (31) // 0x0000001F
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_DEF                                (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL1_RSVD_HSH                                (0x05364D1C)

#define DDRPMSTC_DEBUG_CTRL2_REG                                       (0x00004D20)
#define DDRPMSTC_DEBUG_CTRL2_DEF                                       (0x00000000)
#define DDRPMSTC_DEBUG_CTRL2_VOLATILE_BITFIELDS_MSK                    (0x02000000)

  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_OFF                         ( 0)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_WID                         (24)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_MSK                         (0x00FFFFFF)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_MIN                         (0)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_MAX                         (16777215) // 0x00FFFFFF
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_DEF                         (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_HSH                         (0x18004D20)

  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_OFF                      (24)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_WID                      ( 1)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_MSK                      (0x01000000)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_MIN                      (0)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_MAX                      (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_DEF                      (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_En_HSH                      (0x01304D20)

  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_OFF                     (25)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_WID                     ( 1)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_MSK                     (0x02000000)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_MIN                     (0)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_MAX                     (1) // 0x00000001
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_DEF                     (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL2_BreakPoint1_Hit_HSH                     (0x01B24D20)

  #define DDRPMSTC_DEBUG_CTRL2_RSVD_OFF                                (26)
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_WID                                ( 6)
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_MSK                                (0xFC000000)
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_MIN                                (0)
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_MAX                                (63) // 0x0000003F
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_DEF                                (0x00000000)
  #define DDRPMSTC_DEBUG_CTRL2_RSVD_HSH                                (0x06344D20)

#define DDRPMSTC_MASTERCTRL_CTRL0_REG                                  (0x00004D24)
#define DDRPMSTC_MASTERCTRL_CTRL0_DEF                                  (0x00000000)
#define DDRPMSTC_MASTERCTRL_CTRL0_VOLATILE_BITFIELDS_MSK               (0xFFC0FFFF)

  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_OFF                       ( 0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_WID                       (16)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_MSK                       (0x0000FFFF)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_MIN                       (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_MAX                       (65535) // 0x0000FFFF
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_DEF                       (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCount_HSH                       (0x10804D24)

  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_OFF                    (16)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_WID                    ( 4)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_MSK                    (0x000F0000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_MIN                    (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_MAX                    (15) // 0x0000000F
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_DEF                    (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_RunCountSel_HSH                    (0x04204D24)

  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_OFF                   (20)
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_WID                   ( 2)
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_MSK                   (0x00300000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_MIN                   (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_MAX                   (3) // 0x00000003
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_DEF                   (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_StartGVPoint_HSH                   (0x02284D24)

  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_OFF                 (22)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_WID                 ( 2)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_MSK                 (0x00C00000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_MIN                 (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_MAX                 (3) // 0x00000003
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_DEF                 (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentGVPoint_HSH                 (0x02AC4D24)

  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_OFF                    (24)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_WID                    ( 4)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_MSK                    (0x0F000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_MIN                    (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_MAX                    (15) // 0x0000000F
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_DEF                    (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_CurrentTest_HSH                    (0x04B04D24)

  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_OFF                       (28)
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_WID                       ( 4)
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_MSK                       (0xF0000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_MIN                       (0)
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_MAX                       (15) // 0x0000000F
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_DEF                       (0x00000000)
  #define DDRPMSTC_MASTERCTRL_CTRL0_PrevTest_HSH                       (0x04B84D24)

#define DDRPMSTC_FAILMODE_CTRL0_REG                                    (0x00004D28)
#define DDRPMSTC_FAILMODE_CTRL0_DEF                                    (0x00000000)
#define DDRPMSTC_FAILMODE_CTRL0_VOLATILE_BITFIELDS_MSK                 (0xF0000FFF)

  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_OFF                        ( 0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_WID                        (12)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_MSK                        (0x00000FFF)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_MIN                        (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_MAX                        (4095) // 0x00000FFF
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_DEF                        (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCount_HSH                        (0x0C804D28)

  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_OFF                     (12)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_WID                     ( 4)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_MSK                     (0x0000F000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_MIN                     (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_MAX                     (15) // 0x0000000F
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_DEF                     (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailCountSel_HSH                     (0x04184D28)

  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_OFF                     (16)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_WID                     ( 2)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_MSK                     (0x00030000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_MIN                     (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_MAX                     (3) // 0x00000003
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_DEF                     (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopMode_HSH                     (0x02204D28)

  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_OFF            (18)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_WID            ( 6)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_MSK            (0x00FC0000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_MIN            (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_MAX            (63) // 0x0000003F
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_DEF            (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeThreshold_HSH            (0x06244D28)

  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_OFF                  (24)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_WID                  ( 4)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_MSK                  (0x0F000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_MIN                  (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_MAX                  (15) // 0x0000000F
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_DEF                  (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FailStopModeSel_HSH                  (0x04304D28)

  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_OFF                    (28)
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_WID                    ( 4)
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_MSK                    (0xF0000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_MIN                    (0)
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_MAX                    (15) // 0x0000000F
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_DEF                    (0x00000000)
  #define DDRPMSTC_FAILMODE_CTRL0_FirstFailTest_HSH                    (0x04B84D28)

#define DDRPMSTC_UPCONTROLLER_CTRL0_REG                                (0x00004D2C)
#define DDRPMSTC_UPCONTROLLER_CTRL0_DEF                                (0x00000000)
#define DDRPMSTC_UPCONTROLLER_CTRL0_VOLATILE_BITFIELDS_MSK             (0xFFFF0000)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_OFF                   ( 0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_WID                   ( 5)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_MSK                   (0x0000001F)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_MIN                   (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_MAX                   (31) // 0x0000001F
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_DEF                   (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_TimerCount_HSH                   (0x05004D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_OFF                ( 5)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_WID                ( 3)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_MSK                (0x000000E0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_MIN                (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_MAX                (7) // 0x00000007
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_DEF                (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdVal_HSH                (0x030A4D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_OFF                 ( 8)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_WID                 ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_MSK                 (0x00000100)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_MIN                 (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_MAX                 (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_DEF                 (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PstateOvrdEn_HSH                 (0x01104D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_OFF                  ( 9)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_WID                  ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_MSK                  (0x00000200)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_MIN                  (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_MAX                  (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_DEF                  (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdVal_HSH                  (0x01124D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_OFF                   (10)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_WID                   ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_MSK                   (0x00000400)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_MIN                   (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_MAX                   (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_DEF                   (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PreqOvrdEn_HSH                   (0x01144D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_OFF                    (11)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_WID                    ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_MSK                    (0x00000800)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_MIN                    (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_MAX                    (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_DEF                    (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdEn_HSH                    (0x01164D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_OFF                   (12)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_WID                   ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_MSK                   (0x00001000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_MIN                   (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_MAX                   (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_DEF                   (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_AckOvrdVal_HSH                   (0x01184D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_OFF                    (13)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_WID                    ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_MSK                    (0x00002000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_MIN                    (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_MAX                    (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_DEF                    (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdEn_HSH                    (0x011A4D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_OFF                   (14)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_WID                   ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_MSK                   (0x00004000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_MIN                   (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_MAX                   (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_DEF                   (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ErrOvrdVal_HSH                   (0x011C4D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_OFF                  (15)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_WID                  ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_MSK                  (0x00008000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_MIN                  (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_MAX                  (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_DEF                  (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_ManualCycle_HSH                  (0x011E4D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_OFF                      (16)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_WID                      ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_MSK                      (0x00010000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_MIN                      (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_MAX                      (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_DEF                      (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PAccept_HSH                      (0x01A04D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_OFF                        (17)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_WID                        ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_MSK                        (0x00020000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_MIN                        (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_MAX                        (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_DEF                        (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PDeny_HSH                        (0x01A24D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_OFF                      (18)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_WID                      ( 8)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_MSK                      (0x03FC0000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_MIN                      (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_MAX                      (255) // 0x000000FF
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_DEF                      (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PActive_HSH                      (0x08A44D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_OFF                         (26)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_WID                         ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_MSK                         (0x04000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_MIN                         (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_MAX                         (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_DEF                         (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PReq_HSH                         (0x01B44D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_OFF                       (27)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_WID                       ( 3)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_MSK                       (0x38000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_MIN                       (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_MAX                       (7) // 0x00000007
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_DEF                       (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_PState_HSH                       (0x03B64D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_OFF                          (30)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_WID                          ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_MSK                          (0x40000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_MIN                          (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_MAX                          (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_DEF                          (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Ack_HSH                          (0x01BC4D2C)

  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_OFF                          (31)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_WID                          ( 1)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_MSK                          (0x80000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_MIN                          (0)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_MAX                          (1) // 0x00000001
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_DEF                          (0x00000000)
  #define DDRPMSTC_UPCONTROLLER_CTRL0_Err_HSH                          (0x01BE4D2C)

#define DDRPMSTC_XTCONTROLLER_CTRL0_REG                                (0x00004D30)
#define DDRPMSTC_XTCONTROLLER_CTRL0_DEF                                (0x00000000)
#define DDRPMSTC_XTCONTROLLER_CTRL0_VOLATILE_BITFIELDS_MSK             (0x07FF0000)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_OFF                   ( 0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_WID                   ( 5)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_MSK                   (0x0000001F)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_MIN                   (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_MAX                   (31) // 0x0000001F
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_DEF                   (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_TimerCount_HSH                   (0x05004D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_OFF                ( 5)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_WID                ( 6)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_MSK                (0x000007E0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_MIN                (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_MAX                (63) // 0x0000003F
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_DEF                (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrdval_HSH                (0x060A4D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_OFF                 (11)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_WID                 ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_MSK                 (0x00000800)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_MIN                 (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_MAX                 (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_DEF                 (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfuncovrden_HSH                 (0x01164D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_OFF                 (12)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_WID                 ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_MSK                 (0x00001000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_MIN                 (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_MAX                 (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_DEF                 (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrdval_HSH                 (0x01184D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_OFF                  (13)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_WID                  ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_MSK                  (0x00002000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_MIN                  (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_MAX                  (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_DEF                  (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreqovrden_HSH                  (0x011A4D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_OFF                 (14)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_WID                 ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_MSK                 (0x00004000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_MIN                 (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_MAX                 (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_DEF                 (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrdval_HSH                 (0x011C4D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_OFF                  (15)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_WID                  ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_MSK                  (0x00008000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_MIN                  (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_MAX                  (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_DEF                  (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtackovrden_HSH                  (0x011E4D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_OFF                        (16)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_WID                        ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_MSK                        (0x00010000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_MIN                        (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_MAX                        (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_DEF                        (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtreq_HSH                        (0x01A04D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_OFF                       (17)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_WID                       ( 6)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_MSK                       (0x007E0000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_MIN                       (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_MAX                       (63) // 0x0000003F
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_DEF                       (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtfunc_HSH                       (0x06A24D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_OFF                        (23)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_WID                        ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_MSK                        (0x00800000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_MIN                        (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_MAX                        (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_DEF                        (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtack_HSH                        (0x01AE4D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_OFF                       (24)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_WID                       ( 1)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_MSK                       (0x01000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_MIN                       (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_MAX                       (1) // 0x00000001
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_DEF                       (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_xtcack_HSH                       (0x01B04D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_OFF                    (25)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_WID                    ( 2)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_MSK                    (0x06000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_MIN                    (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_MAX                    (3) // 0x00000003
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_DEF                    (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_dramstate_HSH                    (0x02B24D30)

  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_OFF                     (27)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_WID                     ( 5)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_MSK                     (0xF8000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_MIN                     (0)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_MAX                     (31) // 0x0000001F
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_DEF                     (0x00000000)
  #define DDRPMSTC_XTCONTROLLER_CTRL0_reserved_HSH                     (0x05364D30)

#define DDRPMSTC_PKGC_CTRL0_REG                                        (0x00004D34)
#define DDRPMSTC_PKGC_CTRL0_DEF                                        (0x00000000)
#define DDRPMSTC_PKGC_CTRL0_VOLATILE_BITFIELDS_MSK                     (0xFFF30800)

  #define DDRPMSTC_PKGC_CTRL0_TimerCount_OFF                           ( 0)
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_WID                           ( 5)
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_MSK                           (0x0000001F)
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_MIN                           (0)
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_MAX                           (31) // 0x0000001F
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_DEF                           (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_TimerCount_HSH                           (0x05004D34)

  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_OFF                   ( 5)
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_WID                   ( 5)
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_MSK                   (0x000003E0)
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_MIN                   (0)
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_MAX                   (31) // 0x0000001F
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_DEF                   (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_PkgCExitTimerCount_HSH                   (0x050A4D34)

  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_OFF                    (10)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_WID                    ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_MSK                    (0x00000400)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_MIN                    (0)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_MAX                    (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_DEF                    (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTriggerEn_HSH                    (0x01144D34)

  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_OFF                      (11)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_WID                      ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_MSK                      (0x00000800)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_MIN                      (0)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_MAX                      (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_DEF                      (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ExternalTrigger_HSH                      (0x01964D34)

  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_OFF                         (12)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_WID                         ( 1)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_MSK                         (0x00001000)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_MIN                         (0)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_MAX                         (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_DEF                         (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest0_HSH                         (0x01184D34)

  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_OFF                         (13)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_WID                         ( 1)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_MSK                         (0x00002000)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_MIN                         (0)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_MAX                         (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_DEF                         (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_SkipMemtest1_HSH                         (0x011A4D34)

  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_OFF                        (14)
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_WID                        ( 1)
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_MSK                        (0x00004000)
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_MIN                        (0)
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_MAX                        (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_DEF                        (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_MemBlockCheck_HSH                        (0x011C4D34)

  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_OFF                      (15)
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_WID                      ( 1)
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_MSK                      (0x00008000)
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_MIN                      (0)
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_MAX                      (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_DEF                      (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_EnRandomTraffic_HSH                      (0x011E4D34)

  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_OFF                         (16)
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_WID                         ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_MSK                         (0x00010000)
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_MIN                         (0)
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_MAX                         (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_DEF                         (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ManualC3Test_HSH                         (0x01A04D34)

  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_OFF                         (17)
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_WID                         ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_MSK                         (0x00020000)
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_MIN                         (0)
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_MAX                         (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_DEF                         (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ManualC7Test_HSH                         (0x01A24D34)

  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_OFF                      (18)
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_WID                      ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_MSK                      (0x00040000)
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_MIN                      (0)
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_MAX                      (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_DEF                      (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ManualPrepAbort_HSH                      (0x01244D34)

  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_OFF                        (19)
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_WID                        ( 1)
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_MSK                        (0x00080000)
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_MIN                        (0)
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_MAX                        (1) // 0x00000001
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_DEF                        (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_ManualSRAbort_HSH                        (0x01264D34)

  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_OFF                          (20)
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_WID                          (12)
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_MSK                          (0xFFF00000)
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_MIN                          (0)
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_MAX                          (4095) // 0x00000FFF
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_DEF                          (0x00000000)
  #define DDRPMSTC_PKGC_CTRL0_PKGCCounter_HSH                          (0x0CA84D34)

#define DDRPMSTC_SAGV_CTRL0_REG                                        (0x00004D38)
#define DDRPMSTC_SAGV_CTRL0_DEF                                        (0x00000000)
#define DDRPMSTC_SAGV_CTRL0_VOLATILE_BITFIELDS_MSK                     (0xFFFF8000)

  #define DDRPMSTC_SAGV_CTRL0_TimerCount_OFF                           ( 0)
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_WID                           ( 5)
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_MSK                           (0x0000001F)
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_MIN                           (0)
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_MAX                           (31) // 0x0000001F
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_DEF                           (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_TimerCount_HSH                           (0x05004D38)

  #define DDRPMSTC_SAGV_CTRL0_Reserved_OFF                             ( 5)
  #define DDRPMSTC_SAGV_CTRL0_Reserved_WID                             ( 4)
  #define DDRPMSTC_SAGV_CTRL0_Reserved_MSK                             (0x000001E0)
  #define DDRPMSTC_SAGV_CTRL0_Reserved_MIN                             (0)
  #define DDRPMSTC_SAGV_CTRL0_Reserved_MAX                             (15) // 0x0000000F
  #define DDRPMSTC_SAGV_CTRL0_Reserved_DEF                             (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_Reserved_HSH                             (0x040A4D38)

  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_OFF                         ( 9)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_WID                         ( 1)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_MSK                         (0x00000200)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_MIN                         (0)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_MAX                         (1) // 0x00000001
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_DEF                         (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest0_HSH                         (0x01124D38)

  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_OFF                         (10)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_WID                         ( 1)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_MSK                         (0x00000400)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_MIN                         (0)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_MAX                         (1) // 0x00000001
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_DEF                         (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_SkipMemtest1_HSH                         (0x01144D38)

  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_OFF                        (11)
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_WID                        ( 1)
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_MSK                        (0x00000800)
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_MIN                        (0)
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_MAX                        (1) // 0x00000001
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_DEF                        (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_MemBlockCheck_HSH                        (0x01164D38)

  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_OFF                      (12)
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_WID                      ( 1)
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_MSK                      (0x00001000)
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_MIN                      (0)
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_MAX                      (1) // 0x00000001
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_DEF                      (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_EnRandomTraffic_HSH                      (0x01184D38)

  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_OFF                  (13)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_WID                  ( 2)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_MSK                  (0x00006000)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_MIN                  (0)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_MAX                  (3) // 0x00000003
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_DEF                  (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitchPoint_HSH                  (0x021A4D38)

  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_OFF                       (15)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_WID                       ( 1)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_MSK                       (0x00008000)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_MIN                       (0)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_MAX                       (1) // 0x00000001
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_DEF                       (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_ManualGVSwitch_HSH                       (0x019E4D38)

  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_OFF                          (16)
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_WID                          (16)
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_MSK                          (0xFFFF0000)
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_MIN                          (0)
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_MAX                          (65535) // 0x0000FFFF
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_DEF                          (0x00000000)
  #define DDRPMSTC_SAGV_CTRL0_SAGVCounter_HSH                          (0x10A04D38)

#define DDRPMSTC_LPMODE_CTRL0_REG                                      (0x00004D3C)
#define DDRPMSTC_LPMODE_CTRL0_DEF                                      (0x00000000)
#define DDRPMSTC_LPMODE_CTRL0_VOLATILE_BITFIELDS_MSK                   (0xFFFC4000)

  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_OFF                         ( 0)
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_WID                         ( 5)
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_MSK                         (0x0000001F)
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_MIN                         (0)
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_MAX                         (31) // 0x0000001F
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_DEF                         (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_TimerCount_HSH                         (0x05004D3C)

  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_OFF                   ( 5)
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_WID                   ( 8)
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_MSK                   (0x00001FE0)
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_MIN                   (0)
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_MAX                   (255) // 0x000000FF
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_DEF                   (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_LPExitTimerCount_HSH                   (0x080A4D3C)

  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_OFF                  (13)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_WID                  ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_MSK                  (0x00002000)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_MIN                  (0)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_MAX                  (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_DEF                  (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTriggerEn_HSH                  (0x011A4D3C)

  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_OFF                    (14)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_WID                    ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_MSK                    (0x00004000)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_MIN                    (0)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_MAX                    (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_DEF                    (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_ExternalTrigger_HSH                    (0x019C4D3C)

  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_OFF                       (15)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_WID                       ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_MSK                       (0x00008000)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_MIN                       (0)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_MAX                       (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_DEF                       (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest0_HSH                       (0x011E4D3C)

  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_OFF                       (16)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_WID                       ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_MSK                       (0x00010000)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_MIN                       (0)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_MAX                       (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_DEF                       (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_SkipMemtest1_HSH                       (0x01204D3C)

  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_OFF                      (17)
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_WID                      ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_MSK                      (0x00020000)
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_MIN                      (0)
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_MAX                      (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_DEF                      (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_MemBlockCheck_HSH                      (0x01224D3C)

  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_OFF                      (18)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_WID                      ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_MSK                      (0x00040000)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_MIN                      (0)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_MAX                      (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_DEF                      (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP3Test_HSH                      (0x01A44D3C)

  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_OFF                      (19)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_WID                      ( 1)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_MSK                      (0x00080000)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_MIN                      (0)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_MAX                      (1) // 0x00000001
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_DEF                      (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_ManualLP4Test_HSH                      (0x01A64D3C)

  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_OFF                      (20)
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_WID                      (12)
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_MSK                      (0xFFF00000)
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_MIN                      (0)
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_MAX                      (4095) // 0x00000FFF
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_DEF                      (0x00000000)
  #define DDRPMSTC_LPMODE_CTRL0_LPModeCounter_HSH                      (0x0CA84D3C)

#define DDRPMSTC_TESTSTATUS_REG                                        (0x00004D40)
#define DDRPMSTC_TESTSTATUS_DEF                                        (0x00000000)
#define DDRPMSTC_TESTSTATUS_VOLATILE_BITFIELDS_MSK                     (0xFFFFFFFF)

  #define DDRPMSTC_TESTSTATUS_MemTestPass_OFF                          ( 0)
  #define DDRPMSTC_TESTSTATUS_MemTestPass_WID                          ( 1)
  #define DDRPMSTC_TESTSTATUS_MemTestPass_MSK                          (0x00000001)
  #define DDRPMSTC_TESTSTATUS_MemTestPass_MIN                          (0)
  #define DDRPMSTC_TESTSTATUS_MemTestPass_MAX                          (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_MemTestPass_DEF                          (0x00000000)
  #define DDRPMSTC_TESTSTATUS_MemTestPass_HSH                          (0x01804D40)

  #define DDRPMSTC_TESTSTATUS_MemTestFail_OFF                          ( 1)
  #define DDRPMSTC_TESTSTATUS_MemTestFail_WID                          ( 1)
  #define DDRPMSTC_TESTSTATUS_MemTestFail_MSK                          (0x00000002)
  #define DDRPMSTC_TESTSTATUS_MemTestFail_MIN                          (0)
  #define DDRPMSTC_TESTSTATUS_MemTestFail_MAX                          (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_MemTestFail_DEF                          (0x00000000)
  #define DDRPMSTC_TESTSTATUS_MemTestFail_HSH                          (0x01824D40)

  #define DDRPMSTC_TESTSTATUS_MemTestCode_OFF                          ( 2)
  #define DDRPMSTC_TESTSTATUS_MemTestCode_WID                          ( 6)
  #define DDRPMSTC_TESTSTATUS_MemTestCode_MSK                          (0x000000FC)
  #define DDRPMSTC_TESTSTATUS_MemTestCode_MIN                          (0)
  #define DDRPMSTC_TESTSTATUS_MemTestCode_MAX                          (63) // 0x0000003F
  #define DDRPMSTC_TESTSTATUS_MemTestCode_DEF                          (0x00000000)
  #define DDRPMSTC_TESTSTATUS_MemTestCode_HSH                          (0x06844D40)

  #define DDRPMSTC_TESTSTATUS_PkgCPass_OFF                             ( 8)
  #define DDRPMSTC_TESTSTATUS_PkgCPass_WID                             ( 1)
  #define DDRPMSTC_TESTSTATUS_PkgCPass_MSK                             (0x00000100)
  #define DDRPMSTC_TESTSTATUS_PkgCPass_MIN                             (0)
  #define DDRPMSTC_TESTSTATUS_PkgCPass_MAX                             (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_PkgCPass_DEF                             (0x00000000)
  #define DDRPMSTC_TESTSTATUS_PkgCPass_HSH                             (0x01904D40)

  #define DDRPMSTC_TESTSTATUS_PkgCFail_OFF                             ( 9)
  #define DDRPMSTC_TESTSTATUS_PkgCFail_WID                             ( 1)
  #define DDRPMSTC_TESTSTATUS_PkgCFail_MSK                             (0x00000200)
  #define DDRPMSTC_TESTSTATUS_PkgCFail_MIN                             (0)
  #define DDRPMSTC_TESTSTATUS_PkgCFail_MAX                             (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_PkgCFail_DEF                             (0x00000000)
  #define DDRPMSTC_TESTSTATUS_PkgCFail_HSH                             (0x01924D40)

  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_OFF                         (10)
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_WID                         ( 6)
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_MSK                         (0x0000FC00)
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_MIN                         (0)
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_MAX                         (63) // 0x0000003F
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_DEF                         (0x00000000)
  #define DDRPMSTC_TESTSTATUS_PkgCFailCode_HSH                         (0x06944D40)

  #define DDRPMSTC_TESTSTATUS_SAGVPass_OFF                             (16)
  #define DDRPMSTC_TESTSTATUS_SAGVPass_WID                             ( 1)
  #define DDRPMSTC_TESTSTATUS_SAGVPass_MSK                             (0x00010000)
  #define DDRPMSTC_TESTSTATUS_SAGVPass_MIN                             (0)
  #define DDRPMSTC_TESTSTATUS_SAGVPass_MAX                             (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_SAGVPass_DEF                             (0x00000000)
  #define DDRPMSTC_TESTSTATUS_SAGVPass_HSH                             (0x01A04D40)

  #define DDRPMSTC_TESTSTATUS_SAGVFail_OFF                             (17)
  #define DDRPMSTC_TESTSTATUS_SAGVFail_WID                             ( 1)
  #define DDRPMSTC_TESTSTATUS_SAGVFail_MSK                             (0x00020000)
  #define DDRPMSTC_TESTSTATUS_SAGVFail_MIN                             (0)
  #define DDRPMSTC_TESTSTATUS_SAGVFail_MAX                             (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_SAGVFail_DEF                             (0x00000000)
  #define DDRPMSTC_TESTSTATUS_SAGVFail_HSH                             (0x01A24D40)

  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_OFF                         (18)
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_WID                         ( 6)
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_MSK                         (0x00FC0000)
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_MIN                         (0)
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_MAX                         (63) // 0x0000003F
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_DEF                         (0x00000000)
  #define DDRPMSTC_TESTSTATUS_SAGVFailCode_HSH                         (0x06A44D40)

  #define DDRPMSTC_TESTSTATUS_LPModePass_OFF                           (24)
  #define DDRPMSTC_TESTSTATUS_LPModePass_WID                           ( 1)
  #define DDRPMSTC_TESTSTATUS_LPModePass_MSK                           (0x01000000)
  #define DDRPMSTC_TESTSTATUS_LPModePass_MIN                           (0)
  #define DDRPMSTC_TESTSTATUS_LPModePass_MAX                           (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_LPModePass_DEF                           (0x00000000)
  #define DDRPMSTC_TESTSTATUS_LPModePass_HSH                           (0x01B04D40)

  #define DDRPMSTC_TESTSTATUS_LPModeFail_OFF                           (25)
  #define DDRPMSTC_TESTSTATUS_LPModeFail_WID                           ( 1)
  #define DDRPMSTC_TESTSTATUS_LPModeFail_MSK                           (0x02000000)
  #define DDRPMSTC_TESTSTATUS_LPModeFail_MIN                           (0)
  #define DDRPMSTC_TESTSTATUS_LPModeFail_MAX                           (1) // 0x00000001
  #define DDRPMSTC_TESTSTATUS_LPModeFail_DEF                           (0x00000000)
  #define DDRPMSTC_TESTSTATUS_LPModeFail_HSH                           (0x01B24D40)

  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_OFF                       (26)
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_WID                       ( 6)
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_MSK                       (0xFC000000)
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_MIN                       (0)
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_MAX                       (63) // 0x0000003F
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_DEF                       (0x00000000)
  #define DDRPMSTC_TESTSTATUS_LPModeFailCode_HSH                       (0x06B44D40)

#define DDRPMSTC_MISC_CTRL0_REG                                        (0x00004D44)
#define DDRPMSTC_MISC_CTRL0_DEF                                        (0x00000000)
#define DDRPMSTC_MISC_CTRL0_VOLATILE_BITFIELDS_MSK                     (0x00000000)

  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_OFF                     ( 0)
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_WID                     ( 1)
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_MSK                     (0x00000001)
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_MIN                     (0)
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_MAX                     (1) // 0x00000001
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_DEF                     (0x00000000)
  #define DDRPMSTC_MISC_CTRL0_pchannel_mux_sel_HSH                     (0x01004D44)

  #define DDRPMSTC_MISC_CTRL0_reserved_OFF                             ( 1)
  #define DDRPMSTC_MISC_CTRL0_reserved_WID                             (31)
  #define DDRPMSTC_MISC_CTRL0_reserved_MSK                             (0xFFFFFFFE)
  #define DDRPMSTC_MISC_CTRL0_reserved_MIN                             (0)
  #define DDRPMSTC_MISC_CTRL0_reserved_MAX                             (2147483647) // 0x7FFFFFFF
  #define DDRPMSTC_MISC_CTRL0_reserved_DEF                             (0x00000000)
  #define DDRPMSTC_MISC_CTRL0_reserved_HSH                             (0x1F024D44)



#pragma pack(pop)
#endif /* __MrcRegisterPtl4xxx_h__ */







