/** @file
  Overclocking platform Null library.

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

#include <Library/OcPlatformLib.h>

typedef struct {
  UINT32           MailboxData;
  UINT32           MailboxStatus;
} PROCESSOR_FUSED_P0_MAILBOX_DATA_READ;

/**
  Get first threads' Fused P0 ratio and voltage value

  @param[in]     ProcessorNumber  The handle number of processor
  @param[out]    VoltageTarget    Pointer to VoltageTarget, output data with binary millivolts format

  @retval  EFI_SUCCESS            Get Fused P0 data successfully.
  @retval  EFI_NOT_READY          MP Initialize Library is not initialized.
  @retval  EFI_NOT_READY          The specified AP is busy.
  @retval  EFI_NOT_FOUND          gEfiMpServiceProtocolGuid not found.
  @retval  EFI_NOT_FOUND          The processor with the handle specified by
                                  ProcessorNumber does not exist.
  @retval  EFI_INVALID_PARAMETER  ProcessorNumber specifies the BSP or disabled AP.
  @retval  EFI_INVALID_PARAMETER  MailboxStatus not match MAILBOX_OC_COMPLETION_CODE_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetProcessorFusedP0MinimumVoltage (
  IN   UINT32           ProcessorNumber,
  OUT  UINT16           *VoltageTarget
  )
{
  return EFI_SUCCESS;
}

/**
  Get GT/Ring/MemSS/Compute SAF Fused P0 ratio and voltage value

  @param[in]     DomainId         Mailbox domainId, GT : 0x1, Ring : 0x2
  @param[out]    VoltageTarget    Pointer to VoltageTarget, output data with binary millivolts format

  @retval  EFI_SUCCESS            Get Fused P0 data successfully.
  @retval  EFI_INVALID_PARAMETER  DomainId no match GT or Ring.
  @retval  EFI_INVALID_PARAMETER  MailboxStatus not match MAILBOX_OC_COMPLETION_CODE_SUCCESS.
**/
EFI_STATUS
EFIAPI
GetVfDomainFusedP0MinimumVoltage (
  IN   UINT8            DomainId,
  OUT  UINT16           *VoltageTarget
  )
{
  return EFI_SUCCESS;
}
