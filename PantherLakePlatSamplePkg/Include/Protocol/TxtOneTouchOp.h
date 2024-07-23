/** @file
  Txt specific PPI operation definition.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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

#ifndef _TXT_PPI_OPERATION_H_
#define _TXT_PPI_OPERATION_H_

typedef struct _TXT_ONE_TOUCH_OP_PROTOCOL TXT_ONE_TOUCH_OP_PROTOCOL;

//
// Member functions
//
/*
  Extend PPI operation for TxT.

  @param[in] This    - Point of TXT_ONE_TOUCH_OP_PROTOCOL
  @param[in] Command - Operation value for TxT

*/
typedef
EFI_STATUS
(EFIAPI *TXT_PPI_EXEC_OPERATION)(
  IN TXT_ONE_TOUCH_OP_PROTOCOL *This,
  IN UINT8                     Command
  );

/*
  Confirmation dialog for TxT PPI

  @param[in] This    - Point of TXT_ONE_TOUCH_OP_PROTOCOL
  @param[in] Command - Operation value for TxT
  @param[in] Confirm - User confirm
*/
typedef
EFI_STATUS
(EFIAPI *TXT_CONFIRMATION_DIALOG)(
  IN TXT_ONE_TOUCH_OP_PROTOCOL *This,
  IN UINT8                     Command,
  IN OUT BOOLEAN               *Confirm
  );

/**
  Reset system.

  @param[in] This    - Point of TXT_ONE_TOUCH_OP_PROTOCOL
  @param[in] Command - Operation value for TxT

  @retval EFI_SUCCESS - Always return EFI_SUCCESS
**/
typedef
EFI_STATUS
(EFIAPI *TXT_RESET_SYSTEM)(
  IN TXT_ONE_TOUCH_OP_PROTOCOL *This,
  IN UINT8                     Command
  );

struct _TXT_ONE_TOUCH_OP_PROTOCOL {
  TXT_PPI_EXEC_OPERATION  ExecuteOperation;
  TXT_CONFIRMATION_DIALOG ConfirmationDialog;
  TXT_RESET_SYSTEM        ResetSystem;
};

#endif
