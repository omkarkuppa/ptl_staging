/** @file
Defines the device IECS communication interface

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
#ifndef TBT_NVM_DRV_TARGET_DEVICE_
#define TBT_NVM_DRV_TARGET_DEVICE_

#include <Library/TbtNvmDrvUtils.h>

typedef struct _TBT_RETIMER TBT_RETIMER;

typedef enum {
  AFTER_AUTH
} DRIVER_STATE;


/**
  Function to write IECS register in the target device

  @param[in] This        The pointer to the interface, where this function is defined.
  @param[in] RegNum      IECS register number
  @param[in] Data        The pointer to 32b data to write
  @param[in] Length      How many DWs to write

**/
typedef
TBT_STATUS
(*WRITE_IECS_REG) (
  IN TBT_RETIMER      *This,
  IN UINT8            RegNum,
  IN UINT32           *Data,
  IN UINT8            Length
  );

/**
  Function to read IECS register from the target device

  @param[in] This        The pointer to the interface, where this function is defined.
  @param[in] RegNum      IECS register number
  @param[in] DataLength The length of the Data buffer parameter in DWs
  @param[out] Data       The pointer to the buffer

**/
typedef
TBT_STATUS
(*READ_IECS_REG) (
  IN TBT_RETIMER      *This,
  IN UINT8            RegNum,
  IN UINT32           DataLength,
  OUT UINT32          *Data
  );

/**
  Indicate to the device about the flow state

  @param[in] This  The pointer to the interface, where this function is defined.
  @param[in] State Indicate to the device about flow state, as it might be required to perform some steps.

**/
typedef
VOID
(*INDICATE_STATE) (
  IN TBT_RETIMER     *This,
  IN DRIVER_STATE    State
  );


/**
  A function to disconnect the interface and release the implementation

  @param[in] This The pointer to the interface, where this function is defined.

**/
typedef
VOID
(*DESTROY) (
  IN TBT_RETIMER   *This
  );

/**
  A function to initialize the required HW
  @param[in] This The pointer to the interface, where this function is defined.
**/
typedef
TBT_STATUS
(*INIT_HW) (
  IN TBT_RETIMER   *This
  );

/**
  Terminate the required HW through SendLsupCmdDis

  @param[in] This      Pointer to the generic device interface - TBT_RETIMER

  @retval Status:
    TBT_STATUS_NON_RECOVERABLE_ERROR - need to terminate the execution
    TBT_STATUS_SUCCESS               - indicate operation success
**/
typedef
TBT_STATUS
(*TERMINATE_HW) (
  IN TBT_RETIMER      *This
  );

// The interface to access the target device IECS registers.
struct _TBT_RETIMER {
  VOID                *Impl;
  WRITE_IECS_REG      WriteIecsReg;
  READ_IECS_REG       ReadIecsReg;
  DESTROY             Destroy;
  INDICATE_STATE      IndicateState;
  INIT_HW             InitHW;
  TERMINATE_HW        TerminateHW;
};

#endif /* TBT_NVM_DRV_DEVICE_IECS_COMMUNICATION_IF_ */
