/** @file
  This file contains the mpservices helper functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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

#include "HstiIhvDxe.h"

typedef struct {
  UINT32  Index;
  UINT64  Value;
} AP_PRPCEDURE_ARGUMENT_READMSR;

typedef struct {
  UINT32  Index;
  UINT32  Eax;
  UINT32  Ebx;
  UINT32  Ecx;
  UINT32  Edx;
} AP_PRPCEDURE_ARGUMENT_CPUID;

EFI_MP_SERVICES_PROTOCOL  *mMpService;
UINTN                     mBspNumber;
UINTN                     mNumberOfProcessors;
UINTN                     mNumberOfEnabledProcessors;

/**
  Initialize MP Helper
**/
VOID
InitMp (
  VOID
  )
{
  EFI_STATUS  Status;

  DEBUG ((EFI_D_INFO, "InitMp\n"));

  Status = gBS->LocateProtocol (&gEfiMpServiceProtocolGuid, NULL, (VOID **) &mMpService);
  if (EFI_ERROR (Status)) {
    mMpService = NULL;
  }

  if (mMpService != NULL) {
    Status = mMpService->WhoAmI (mMpService, &mBspNumber);
    ASSERT_EFI_ERROR (Status);

    Status = mMpService->GetNumberOfProcessors (mMpService, &mNumberOfProcessors, &mNumberOfEnabledProcessors);
    ASSERT_EFI_ERROR (Status);
  } else {
    mBspNumber = 0;
    mNumberOfProcessors = 1;
    mNumberOfEnabledProcessors = 1;
  }

  DEBUG ((DEBUG_INFO, "BspNumber                 - 0x%x\n", mBspNumber));
  DEBUG ((DEBUG_INFO, "NumberOfProcessors        - 0x%x\n", mNumberOfProcessors));
  DEBUG ((DEBUG_INFO, "NumberOfEnabledProcessors - 0x%x\n", mNumberOfEnabledProcessors));
}

/**
  Concatenate error string.

  @retval UINTN - CpuNumber.
**/
UINTN
GetCpuNumber (
  VOID
  )
{
  return mNumberOfEnabledProcessors;
}

/**
  Read each processor MSR value

  @param[in, out] Buffer     - Pointer to Argument

**/
VOID
EFIAPI
ApReadMsr64 (
  IN OUT VOID  *Buffer
  )
{
  AP_PRPCEDURE_ARGUMENT_READMSR  *Argument;

  Argument = Buffer;
  Argument->Value = AsmReadMsr64 (Argument->Index);
}

/**
  Read each processor MSR value

  @param[in, out] Buffer     - Pointer to Argument

**/
VOID
EFIAPI
ApWriteMsr64 (
  IN OUT VOID  *Buffer
  )
{
  AP_PRPCEDURE_ARGUMENT_READMSR  *Argument;

  Argument = Buffer;
  Argument->Value = AsmWriteMsr64 (Argument->Index, Argument->Value);
}

/**
  Concatenate error string.

  @param[in, out] Buffer     - Pointer to Argument

**/
VOID
EFIAPI
ApCpuId (
  IN OUT VOID  *Buffer
  )
{
  AP_PRPCEDURE_ARGUMENT_CPUID  *Argument;

  Argument = Buffer;
  AsmCpuid (Argument->Index, &Argument->Eax, &Argument->Ebx, &Argument->Ecx, &Argument->Edx);
}

/**
  Concatenate error string.

  @param[in] ProcessorNumber     - Processor ID
  @param[in] Index               - Index

  @retval UINT64 - Msr Value.
**/
UINT64
ProcessorReadMsr64 (
  IN UINTN   ProcessorNumber,
  IN UINT32  Index
  )
{
  EFI_STATUS                      Status;
  AP_PRPCEDURE_ARGUMENT_READMSR   Argument;

  ASSERT (ProcessorNumber < mNumberOfEnabledProcessors);
  if (ProcessorNumber == mBspNumber) {
    return AsmReadMsr64 (Index);
  }

  ZeroMem (&Argument, sizeof (Argument));
  Argument.Index = Index;
  Status = mMpService->StartupThisAP (
                         mMpService,
                         ApReadMsr64,
                         ProcessorNumber,
                         NULL,   // WaitEvent
                         0,      // TimeoutInMicroseconds
                         &Argument,
                         NULL    // Finished
                         );
  ASSERT_EFI_ERROR (Status);

  return Argument.Value;
}

/**
  Concatenate error string.

  @param[in] ProcessorNumber     - Processor ID
  @param[in] Index               - Index
  @param[in] Value               - Value

**/
VOID
ProcessorWriteMsr64 (
  IN UINTN   ProcessorNumber,
  IN UINT32  Index,
  IN UINTN   Value
  )
{
  EFI_STATUS                      Status;
  AP_PRPCEDURE_ARGUMENT_READMSR   Argument;

  ASSERT (ProcessorNumber < mNumberOfEnabledProcessors);
  if (ProcessorNumber == mBspNumber) {
    AsmWriteMsr64 (Index, Value);
    return;
  }

  ZeroMem (&Argument, sizeof (Argument));
  Argument.Index = Index;
  Argument.Value = Value;
  Status = mMpService->StartupThisAP (
                         mMpService,
                         ApWriteMsr64,
                         ProcessorNumber,
                         NULL,   // WaitEvent
                         0,      // TimeoutInMicroseconds
                         &Argument,
                         NULL    // Finished
                         );
  ASSERT_EFI_ERROR (Status);

  return;
}

/**
  Concatenate error string.

  @param[in]  ProcessorNumber     - Processor ID
  @param[in]  Index               - Index
  @param[out] Eax                 - Eax
  @param[out] Ebx                 - Ebx
  @param[out] Ecx                 - Ecx
  @param[out] Edx                 - Edx
**/
VOID
ProcessorCpuid (
  IN  UINTN   ProcessorNumber,
  IN  UINT32  Index,
  OUT UINT32  *Eax,  OPTIONAL
  OUT UINT32  *Ebx,  OPTIONAL
  OUT UINT32  *Ecx,  OPTIONAL
  OUT UINT32  *Edx   OPTIONAL
  )
{
  EFI_STATUS                      Status;
  AP_PRPCEDURE_ARGUMENT_CPUID     Argument;

  ASSERT (ProcessorNumber < mNumberOfEnabledProcessors);
  if (ProcessorNumber == mBspNumber) {
    AsmCpuid (Index, Eax, Ebx, Ecx, Edx);
    return;
  }

  ZeroMem (&Argument, sizeof (Argument));
  Argument.Index = Index;
  Status = mMpService->StartupThisAP (
                         mMpService,
                         ApCpuId,
                         ProcessorNumber,
                         NULL,   // WaitEvent
                         0,      // TimeoutInMicroseconds
                         &Argument,
                         NULL    // Finished
                         );
  ASSERT_EFI_ERROR (Status);

  if (Eax != NULL) {
    *Eax = Argument.Eax;
  }
  if (Ebx != NULL) {
    *Ebx = Argument.Ebx;
  }
  if (Ecx != NULL) {
    *Ecx = Argument.Ecx;
  }
  if (Edx != NULL) {
    *Edx = Argument.Edx;
  }

  return;
}