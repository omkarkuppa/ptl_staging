/** @file
  Header file for Gpio V2 Controller Interace library.

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

#ifndef _GPIOV2_CONTROLLER_PRIVATE_H_
#define _GPIOV2_CONTROLLER_PRIVATE_H_

#include <Library/GpioV2AccessLib.h>
#include <Library/P2SbSidebandAccessLib.h>
#include <GpioV2Services.h>
#include <GpioV2Pwm.h>
#include <Library/DebugLib.h>
typedef struct {
  UINT8       GpeControllerDwxVal;
  UINT8       GpioGpeDwxVal;
} GPIOV2_GROUP_TO_GPE_MAPPING;

typedef struct _GPIOV2_SERVICES_PRIVATE GPIOV2_SERVICES_PRIVATE;

//
// Stores information about the unlock request for each pad in the group.
// Groups are assumed to be limited to 32 pads. Each bit in each UINT32 fields
// corresponds to single pad in the group.
//
typedef struct {
  UINT32  UnlockConfigBitMask;
  UINT32  UnlockTxBitMask;
} GPIOV2_UNLOCK_GROUP_DATA;

typedef struct {
  UINT8   Hid[GPIOV2_CONTROLLER_HID_LENGTH];
  UINT32  Uid;
} GPIOV2_CONFLICT_DATA_HEADER;

typedef struct {
  GPIOV2_PAD  GpioPad;
  UINT32      GpioPadMode:5;
  UINT32      Reserved:27;
} GPIOV2_PAD_MODE_INFO;

typedef union _GPIOV2_SIGNAL {
  struct {
    UINT32  PinIndex: 4;
    UINT32  InterfaceIndex: 8;
    UINT32  SignalType: 8;
    UINT32  Interface: 8;
    UINT32  InstanceIndex: 4;
  }                               Fields;
  UINT32                          Value;
} GPIOV2_SIGNAL;

typedef struct _GPIOV2_NATIVE_SIGNAL_DATA {
  GPIOV2_SIGNAL                   Signal;
  GPIOV2_IOSTANDBY_STATE          IosState;
  GPIOV2_IOSTANDBY_TERM           IosTerm;
  BOOLEAN                         VirtualWireMessageMappingAvailable;
  UINT8                           VwIndex;
  UINT8                           BitPosition;
} GPIOV2_NATIVE_SIGNAL_DATA;

typedef struct _GPIOV2_PAD_SIGNAL {
  GPIOV2_SIGNAL                   Signal;
  GPIOV2_PAD_MODE                 PadMode;
} GPIOV2_PAD_SIGNAL;

typedef struct _GPIOV2_PAD_DATA {
  GPIOV2_PAD_SIGNAL               *PadSignals;
  UINT32                          PadSignalsNum;
} GPIOV2_PAD_DATA;

typedef struct {
  CONST CHAR8                     *Name;
  GPIOV2_PAD_GROUP                GpioPadGroup;
  GPIOV2_GROUP_TO_GPE_MAPPING     GroupToGpeMapping;
  GPIOV2_GROUP_REGISTERS_OFFSETS  RegisterOffsets;
  UINT32                          PadsNum;
  GPIOV2_PAD                      *Pads;
  GPIOV2_PAD_DATA                 *PadDataArray;
} GPIOV2_GROUP;

typedef struct GPIOV2_COMMUNITY_S {
  GPIOV2_COMMUNITY_REGISTERS_OFFSETS  RegisterOffsets;
  UINT32                              GroupsNum;
  GPIOV2_GROUP                        *Groups;
  UINT32                              Pid;
  BOOLEAN                             IsComDsw;
} GPIOV2_COMMUNITY;

struct _GPIOV2_SERVICES_PRIVATE {
  UINT32                          ChipsetId;
  UINT32                          CommunitiesNum;
  GPIOV2_COMMUNITY                *Communities;
  P2SB_SIDEBAND_REGISTER_ACCESS   *CommunityAccess;
  UINT64                          *CommunityRegisterBase;
  GPIOV2_UNLOCK_GROUP_DATA        *UnlockData;
  P2SB_CONTROLLER                 P2SbController;
  GPIOV2_NATIVE_SIGNAL_DATA       *NativeSignalData;
  UINT32                          NativeSignalDataNum;
  UINT32                          UnlockAll;
  UINT32                          GpioV2PadOwnRegRev;
  UINT32                          PadCfgLockCntrlGrpIndex;
};

typedef struct {
  UINT32                        Signature;
  GPIOV2_SERVICES               Public;
  GPIOV2_PWM                    Pwm;
  GPIOV2_SERVICES_PRIVATE       Private;
} GPIOV2_INTERFACE;

#define GPIOV2_INTERFACE_SIGNATURE         SIGNATURE_32 ('g', 'v', '2', 'i')

#define GPIOV2_INTERFACE_FROM_PUBLIC(a)    CR (                            \
                                               a,                          \
                                               GPIOV2_INTERFACE,           \
                                               Public,                     \
                                               GPIOV2_INTERFACE_SIGNATURE  \
                                               )

#define GPIOV2_INTERFACE_FROM_PWM(a)       CR (                            \
                                               a,                          \
                                               GPIOV2_INTERFACE,           \
                                               Pwm,                        \
                                               GPIOV2_INTERFACE_SIGNATURE  \
                                               )



#endif // _GPIOV2_CONTROLLER_PRIVATE_H_
