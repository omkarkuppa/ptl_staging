/** @file
  Header file for TME Init Library

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2020 Intel Corporation.

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

#ifndef _TME_LIB_H_
#define _TME_LIB_H_

#include <Ppi/SiPolicy.h>

#define V_TME_ACTIVATE_TME_POLICY_AES_XTS                             0x00
#define V_TME_ACTIVATE_TME_POLICY_AES_XTS_256                         0x02

#pragma pack (push, 1)

typedef struct {
  UINT32 TmeEnable;
  UINT32 TmeBypassCapability;
  UINT64 TmeExcludeBase;
  UINT64 TmeExcludeSize;
  UINT32 TdxEnable;
  UINT32 VtdEnable;
  UINT8  GenerateNewTmeKey;
} TME_POLICY;

#pragma pack (pop)

/**
  Perform Total Memory Encryption initialization.

  @param[in] TmePolicy      - TME policy enable

  @retval VOID - No value to return
**/
VOID
TmeInit (
  IN VOID *TmePolicy
  );

/**
  This function is to configure TME Activate MSR.

  @param[in]  TmeEnable
  @param[in]  KeySelect
  @param[in]  SaveKeyForStandby
  @param[in]  TmePolicy
  @param[in]  MkTmeKeyIdBits
  @param[in]  TdxKeyIdBits
  @param[in]  MkTmeCryptoAlgs
  @param[in]  TmeBypassCapability
**/
VOID
EFIAPI
MsrConfigureTmeActivate (
  IN UINT8   TmeEnable,
  IN UINT8   KeySelect,
  IN UINT8   SaveKeyForStandby,
  IN UINT8   TmePolicy,
  IN UINT8   MkTmeKeyIdBits,
  IN UINT8   TdxKeyIdBits,
  IN UINT16  MkTmeCryptoAlgs,
  IN UINT8   TmeBypassCapability
  );

/**
  This function write-lock the TME Activate MSR.

  @param[in]  TmeEnable
**/

VOID
EFIAPI
MsrLockTmeActivate (
  IN UINT32  TmeEnable
  );

/**
  Configure TME Core Activation MSR.

  @retval EFI_UNSUPPORTED When Tme is not enabled nor supported in the system
  @retval EFI_SUCCESS     Successfully write to Core Activation Msr
**/
EFI_STATUS
EFIAPI
MsrConfigureTmeCoreActivate (
  VOID
  );
#endif // _TME_LIB_H_
