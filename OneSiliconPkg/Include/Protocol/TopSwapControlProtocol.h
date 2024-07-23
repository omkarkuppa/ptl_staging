/** @file
  Set or unset Top Swap bit via RTC BUC register.

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

#ifndef _TOP_SWAP_CONTROL_PROTOCOL_H_
#define _TOP_SWAP_CONTROL_PROTOCOL_H_

#define TOP_SWAP_CONTROL_PROTOCOL_GUID \
  { \
    0x1D95D68F, 0xF556, 0x4B32, { 0xA1, 0xC6, 0x46, 0x0A, 0xD5, 0x8E, 0x21, 0x60 } \
  }

#define SMM_TOP_SWAP_CONTROL_PROTOCOL_GUID \
  { \
    0xB1D181EB, 0x76BB, 0x4E10, { 0x86, 0xCF, 0x6B, 0xF7, 0x5A, 0x27, 0x67, 0xFD } \
  }

typedef struct _TOP_SWAP_CONTROL_PROTOCOL TOP_SWAP_CONTROL_PROTOCOL;

/**
  Get Top Swap status.

  @param[in]  This  A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.

  @retval  TRUE   Top Swap is set.
  @retval  FALSE  Top Swap is clear.

  @note  It is caller's responsibility to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

**/
typedef
BOOLEAN
(EFIAPI *GET_TOP_SWAP_STATUS) (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This
  );

/**
  Set Top Swap status.

  @param[in]  This           A pointer to the TOP_SWAP_CONTROL_PROTOCOL instance.
  @param[in]  TopSwapEnable  Enable Top Swap or disable it.

  @retval  EFI_SUCCESS  Succeed to set Top Swap.
  @retval  Others       Failed to set Top Swap.

  @note  It is caller's responsibility to ensure the accessibility of Backed Up Control register and TS bit.
         This register might be locked or restricted access in certain phases. (e.g. POSTBOOT_SAI)

**/
typedef
EFI_STATUS
(EFIAPI *SET_TOP_SWAP_STATUS) (
  IN TOP_SWAP_CONTROL_PROTOCOL  *This,
  IN BOOLEAN                    TopSwapEnable
  );

///
/// The interface functions are for enabling/disabling Top Swap.
///
struct _TOP_SWAP_CONTROL_PROTOCOL {
  GET_TOP_SWAP_STATUS  GetTopSwapStatus;
  SET_TOP_SWAP_STATUS  SetTopSwapStatus;
};

extern EFI_GUID  gTopSwapControlProtocolGuid;
extern EFI_GUID  gSmmTopSwapControlProtocolGuid;

#endif
