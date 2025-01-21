/** @file

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#ifndef LIFE_CYCLE_STATE_H_
#define LIFE_CYCLE_STATE_H_

#define LIFE_CYCLE_STATE_PROTOCOL_GUID \
  { 0xf3c1138e, 0xcd89, 0x4e20, { 0x9e, 0x68, 0x25, 0xa6, 0x76, 0x95, 0xa5, 0x6a } }

#define LIFE_CYCLE_STATE_PROTOCOL_REVISION1 0x00000001

//*******************************************************
// LIFE_CYCLE_STATE
//*******************************************************
typedef enum {
  StateManufacturing,
  StateEndUser,
  StateRnD,
  StateCare
} LIFE_CYCLE_STATE;

typedef struct _LIFE_CYCLE_STATE_PROTOCOL LIFE_CYCLE_STATE_PROTOCOL;

/**
  Retrieves life cycle state

  @param[in]   This              Protocol instance pointer.
  @param[out]  LifeCycleState    The life cycle state read from ME FW status.

  @retval      EFI_SUCCESS       The state was successfully retrived.

**/
typedef
EFI_STATUS
(EFIAPI * GET_LIFE_CYCLE_STATE) (
    IN LIFE_CYCLE_STATE_PROTOCOL *This,
    OUT LIFE_CYCLE_STATE *LifeCycleState
    );

#pragma pack(1)
struct _LIFE_CYCLE_STATE_PROTOCOL {
  UINT32 Revision;
  GET_LIFE_CYCLE_STATE GetLifeCycleState;
};
#pragma pack()

/**
  Retrieves life cycle state

  @param[in]   This              Protocol instance pointer.
  @param[out]  LifeCycleState    The life cycle state read from ME FW status.

  @retval      EFI_SUCCESS       The state was successfully retrived.

**/
EFI_STATUS
GetLifeCycleState (
  IN LIFE_CYCLE_STATE_PROTOCOL *This,
  OUT LIFE_CYCLE_STATE *LifeCycleState
  );

extern EFI_GUID gLifeCycleStateProtocolGuid;

#endif
