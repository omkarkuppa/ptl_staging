/** @file
  UFS platform driver header file for Intel platforms.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#include <Uefi.h>
#include <Protocol/PciIo.h>
#include <Protocol/UfsHostControllerPlatform.h>

#ifndef _UFS_PLATFORM_HCI_H_
#define _UFS_PLATFORM_HCI_H_

#define UFS_HC_IS_OFFSET           0x0020  // Interrupt Status
#define UFS_HC_IS_UPMS             BIT4
#define UFS_TIMEOUT                EFI_TIMER_PERIOD_SECONDS(3)

#define UFS_UIC_ARG_MIB_SEL(x,y)   ((((x) & 0xFFFF) << 16) | ((y) & 0xFFFF))
#define UFS_PA_HS_MODE_A           0x1
#define UFS_PA_HS_MODE_B           0x2
#define UFS_CR_WR                  0x1
#define UFS_CR_RD                  0x0
#pragma pack(1)
//
// UIC command opcodes
//
typedef enum {
  UfsUicDmeGet            = 0x01,
  UfsUicDmeSet            = 0x02,
  UfsUicDmePeerGet        = 0x03,
  UfsUicDmePeerSet        = 0x04,
  UfsUicDmePwrOn          = 0x10,
  UfsUicDmePwrOff         = 0x11,
  UfsUicDmeEnable         = 0x12,
  UfsUicDmeReset          = 0x14,
  UfsUicDmeEndpointReset  = 0x15,
  UfsUicDmeLinkStartup    = 0x16,
  UfsUicDmeHibernateEnter = 0x17,
  UfsUicDmeHibernateExit  = 0x18,
  UfsUicDmeTestMode       = 0x1A
} UFS_UIC_OPCODE;

typedef enum {
  UfsRxLane                = 0,
  UfsTxLane                = 1,
  UfsReservedLane
} UFS_LANE_TYPE;

typedef enum {
  Fast_Mode     = 1,
  Slow_Mode     = 2,
  FastAuto_Mode = 4,
  SlowAuto_Mode = 5,
  UNCHANGED     = 7,
  UfsReservedPowerMode
} UFS_POWER_MODE;

typedef enum {
  PWM_G1 = 1,
  PWM_G2 = 2,
  PWM_G3 = 3,
  PWM_G4 = 4,
  PWM_G5 = 5,
  PWM_G6 = 6,
  PWM_G7 = 7,
  UfsReservedPwmGear
} UFS_PWM_GEARS;

typedef enum {
  NO_HS  = 0,
  HS_G1  = 1,
  HS_G2  = 2,
  HS_G3  = 3,
  HS_G4  = 4,
  UfsReservedHsGear
} UFS_HS_GEARS;

typedef enum {
  PA_ActiveRxDataLanes                = 0x1580,
  PA_ConnectedRxDataLanes             = 0x1581,
  PA_AvailRxDataLanes                 = 0x1540,
  PA_RXGear                           = 0x1583,
  PA_MaxRxPWMGear                     = 0x1586,
  PA_MaxRxHSGear                      = 0x1587,
  PA_RxPWRStatus                      = 0x1582,
  PA_RxTermination                    = 0x1584,
  PA_ActiveTxDataLanes                = 0x1560,
  PA_ConnectedTxDataLanes             = 0x1561,
  PA_AvailTxDataLanes                 = 0x1520,
  PA_TXGear                           = 0x1568,
  PA_TxPWRStatus                      = 0x1567,
  PA_TxTermination                    = 0x1569,
  PA_HSSeries                         = 0x156A,
  PA_PWRMode                          = 0x1571,
  PA_Scrambling                       = 0x1585,
  PA_TxHsAdaptType                    = 0x15D4,
  PA_AdaptTypeInPA_INIT               = 0x15D5,
  PA_Local_TX_LCC_Enable              = 0x155E,
  PA_Peer_TX_LCC_Enable               = 0x155F,
  PA_PWRModeUserData0                 = 0x15B0,
  PA_PWRModeUserData1                 = 0x15B1,
  PA_PWRModeUserData2                 = 0x15B2,
  PA_Reserved                         = 0xFFFF
} PA_ATTRIBUTES;


typedef enum {
  DME_FC0ProtectionTimeOutVal        = 0xD041,
  DME_TC0ReplayTimeOutVal            = 0xD042,
  DME_AFC0ReqTimeOutVal              = 0xD043,
  DME_Reserved                       = 0xFFFF,
} DME_ATTRIBUTES;

typedef enum {
  CBRATESEL                        = 0x8114,
  CBCREGADDRLSB                    = 0x8116,
  CBCREGADDRMSB                    = 0x8117,
  CBCREGWRLSB                      = 0x8118,
  CBCREGWRMSB                      = 0x8119,
  CBCREGRDLSB                      = 0x811A,
  CBCREGRDMSB                     =  0x811B,
  CBCREGRDWRSEL                    = 0x811C,
  VS_MPHY_CFG_UPDT                 = 0xD085
} RMMI_ATTRIBUTES;

typedef enum {
  T_ConnectionState                  = 0x4020,
  T_CPortFlags                       = 0x4025,
  TL_Reserved                        = 0xFFFF
} TL_ATTRIBUTES;

typedef enum {
  RAWLANEN0_DIG_PCS_XF_RX_OVRD_IN_1 = 0x3006,
  RAWLANEN0_DIG_PCS_XF_RX_PCS_OUT   = 0x300F,
  RAWLANEN1_DIG_PCS_XF_RX_OVRD_IN_1 = 0x3106,
  RAWLANEN1_DIG_PCS_XF_RX_PCS_OUT   = 0x310F
} MPHY_ATTRIBUTES;

typedef union {
  struct {
    UINT16 RxAck              : 1; // Value from Raw PCS for rx_ack
    UINT16 Reserved           : 15;
  } Fields;
  UINT16 Data16;
} UFS_MPHY_RAWLANEN_DIG_PCS_XF_RX_PCS_OUT;

/**
  Switches the link Power Mode and Gear.

  @param[in] Private The pointer to the UFS_PASS_THRU_PRIVATE_DATA data structure

  @retval EFI_SUCCESS  Successfully switched the Power Mode and Gear
  @retval others       Failed to switch the Power Mode and Gear
**/
VOID
UfsHcPlatformPostHceSwitchGear (
  IN EFI_HANDLE                     ControllerHandle,
  IN EFI_PCI_IO_PROTOCOL            *PciIo,
  IN EDKII_UFS_HC_DRIVER_INTERFACE  *UfsHcDriverInterface
  );

#pragma pack()

#endif


