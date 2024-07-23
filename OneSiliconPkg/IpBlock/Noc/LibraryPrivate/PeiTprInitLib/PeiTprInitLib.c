/** @file
  This code provides initialization of DTPR (DMA TXT PROTECTED RANGE).

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
#include <Library/PeiTprInitLib.h>
#include <Library/BaseLib.h>
#include <Library/BootGuardLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/IoLib.h>
#include <Library/PeiNguInitLib.h>
#include <Library/TimerLib.h>
#include <Library/BaseMemoryLib.h>
#include <TprInfoHob.h>
#include <Txt.h>

#define SERIALIZE_REGISTER_SYNC_TIMEOUT     500
#define MMIO_ACM_POLICY_STATUS  (TXT_PUBLIC_BASE + R_CPU_ACM_POLICY_STATUS)  // 0xFED30378

/**
  Function to wait till bit is reset

  @param[in]   Address        Address to read value from
  @param[in]   Offset         Offset of the value which needs to be checked
  @param[in]   TimeOut        Timeout value in microsecond

  @retval FALSE               Timeout occured
  @retval TRUE                Success
**/
BOOLEAN WaitBitClear (
  IN  UINTN          Address,
  IN  UINT64         Offset,
  IN  UINT32         TimeOut
)
{
  UINT64             Value = 0;

  for (UINT32 i = 0; i < TimeOut; i = i + 100) {
    Value = MmioRead64 (Address);
    if ((Value & Offset) == 0)
      return TRUE;
    MicroSecondDelay (100);
  }
  return FALSE;
}

/**
  Serialize DMA transactions through drain control registers

  @param[in]   TprInfoHob        Pointer to TPR_INFO_HOB

  @retval EFI_SUCCESS            All transaction serialized successfully
  @retval EFI_TIMEOUT            Timeout happened while waiting for status bit
**/
EFI_STATUS
SerializeDMATransactions (
  IN  TPR_INFO_HOB                   *TprInfoHob
)
{
  UINT64                              DrainCtrlArr[sizeof(DRAIN_CTL_REG)/sizeof(UINT64)];
  UINT64                              Raw = 0;
  UINT8                               Index = 0;
  UINT64                             *DrainCtrlArrayPtr = NULL;
  UINT8                               Count = 0;

  //
  // Initializing DrainCtrlArr
  //
  ZeroMem (&DrainCtrlArr[0], sizeof(DrainCtrlArr));

  Index = 0;

  DrainCtrlArrayPtr = (UINT64 *)&(TprInfoHob->DrainControlArray);

  // Check valid drain control register entries in tprInfoHob
  for (Index = 0; Index < ARRAY_SIZE (DrainCtrlArr); Index++) {
    if (*DrainCtrlArrayPtr != 0) {
      DrainCtrlArr[Count] = *DrainCtrlArrayPtr;
      Count++;
    }
    DrainCtrlArrayPtr++;
  }

  // Check count of valid entries is same as published by Hob
  if (Count != TprInfoHob->DrainControlCount) {
    DEBUG ((DEBUG_ERROR, "TprInitLib: Valid entries in Hob does not match with count. Found = %d, expected = %d\n", \
                          Count, TprInfoHob->DrainControlCount));
    return EFI_ABORTED;
  }

  // For serializing DMA trasactions, following operations are performed:
  // Set CTRL bit in serialize register
  // Wait for STS bit to change to 0

  for (Index = 0; Index < Count; Index++) {
      Raw = MmioRead64 ((UINTN)DrainCtrlArr[Index]);
      Raw |= DRAIN_CONTROL_REG_CTRL_OFFSET;
      MmioWrite64 ((UINTN)DrainCtrlArr[Index], Raw);
  }

  for (Index = 0; Index < Count; Index++) {
    if (!WaitBitClear((UINTN)DrainCtrlArr[Index], DRAIN_CONTROL_REG_STS_OFFSET, SERIALIZE_REGISTER_SYNC_TIMEOUT)) {
      DEBUG ((DEBUG_ERROR, "TprInitLib: Timeout occurred while checking for DrainCtrl register"));
      return EFI_TIMEOUT;
    }
  }

  return EFI_SUCCESS;
}

/**
  Disable TPR based DMA protection.
  @retval EFI_SUCCESS - TPR disabled successfully
  @retval EFI_ABORTED - TPR disabling process aborted
**/
EFI_STATUS
DisableTprDmaProtection (
  VOID
)
{
  EFI_STATUS                      Status;
  TPR_INFO_HOB                   *TprInfoHob;
  UINT64                          Raw = 0;
  UINT64                          Mask = 0;

  DEBUG ((DEBUG_INFO, "DisableTprDmaProtection\n"));

  // Get TPR_INFO_HOB
  TprInfoHob = (TPR_INFO_HOB *)GetFirstGuidHob (&gTprInfoHobGuid);

  if (TprInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "TprInfoHobGuid not found!"));
    return EFI_NOT_FOUND;
  }

  if (TprInfoHob->InstanceCount > 2) {
    return EFI_INVALID_PARAMETER;
  }

  // Bit 20-41 represent base/limit address in TPR range
  // Zeroing base/limit address
  Mask = TPR_BASE_LIMIT_ADDR_MASK;
  Mask = ~Mask;

  // For all instances, disable all TPR
  for (int i = 0; i < TprInfoHob->InstanceCount; i++) {

    // Updating TPR0.base
    Raw = MmioRead64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr0BaseOffset);
    Raw &= Mask;
    // Disable TPR range
    // 1 is disable and 0 is enable
    Raw |= TPR_CONTROL_BIT_OFFSET;
    MmioWrite64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr0BaseOffset, Raw);

    // Updating TPR0.limit
    Raw = MmioRead64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr0LimitOffset);
    Raw &= Mask;
    // Disable TPR range
    // 1 is disable and 0 is enable
    Raw |= TPR_CONTROL_BIT_OFFSET;
    MmioWrite64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr0LimitOffset, Raw);

    // Updating TPR1.base
    Raw = MmioRead64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr1BaseOffset);
    Raw &= Mask;
    // Disable TPR range
    // 1 is disable and 0 is enable
    Raw |= TPR_CONTROL_BIT_OFFSET;
    MmioWrite64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr1BaseOffset, Raw);

    // Updating TPR1.limit
    Raw = MmioRead64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr1LimitOffset);
    Raw &= Mask;
    // Disable TPR range
    // 1 is disable and 0 is enable
    Raw |= TPR_CONTROL_BIT_OFFSET;
    MmioWrite64 ((UINTN)TprInfoHob->InstanceArray[i].Tpr1LimitOffset, Raw);
    }

  // Serialize all DMA transaction
  Status = SerializeDMATransactions (TprInfoHob);

  if (Status != EFI_SUCCESS) {
    Status = EFI_ABORTED;
  }

  return Status;
}

/**
  TPR configuration as part of SiInit.

**/
VOID
TprInit (
  VOID
)
{
  UINT32         MsrValue;
  UINT32         AcmPolicySts;
  EFI_STATUS     Status = EFI_SUCCESS;

  DEBUG ((DEBUG_INFO, "TprInit\n"));
  MsrValue = (UINT32) AsmReadMsr64 (MSR_BOOT_GUARD_SACM_INFO);

  if ((MsrValue & B_BOOT_GUARD_SACM_INFO_NEM_ENABLED) != 0) {
    AcmPolicySts = MmioRead32 (MMIO_ACM_POLICY_STATUS );

    if  (AcmPolicySts & MMIO_ACM_POLICY_STATUS_DMA_EN_OFFSET) {
      //DMA protection is enabled
      DEBUG ((DEBUG_INFO, "[Boot Guard]Clear TPR based DMA protection\n"));
      Status = DisableTprDmaProtection();
    }
  }

  if (Status != EFI_SUCCESS) {
    DEBUG ((DEBUG_ERROR, "Failed to clear DMA protection set by Bootguard\n"));
  }

  return;
}
