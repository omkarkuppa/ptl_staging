/** @file
  Source file for FSP MP Service implementation.

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

@par Specification
**/
#include <Library/PeiServicesTablePointerLib.h>
#include <Ppi/MpServices2.h>
#include "FspInit.h"

typedef struct {
  EDKII_PEI_MP_SERVICES2_PPI  *CpuMp2Ppi;
  VOID                        *CpuMpPpiFunction;
  UINTN                       Uintn1;
  UINTN                       Uintn2;
  EFI_PROCESSOR_INFORMATION   *ProInfo;
  EFI_AP_PROCEDURE            ApPro;
  VOID                        *Void;
  UINT32                      *Uint32;
  BOOLEAN                     MpBoolean;
} MP_SERVICE_PARAMETER;

extern EDKII_PEI_MP_SERVICES2_PPI    *mCpuMp2Ppi;
extern FSP_GLOBAL_DATA               *mFspGlobalData;

// These global variables only works in Post memory phase.
MP_SERVICE_PARAMETER          mMpServiceParameter;
EFI_STATUS                    mStatus;
BOOLEAN                       mIsSwitchStackDone = FALSE;

//
// CPU MP WRAPPER PPI to be installed
//
EDKII_PEI_MP_SERVICES2_PPI              mMpServices2WrapperPpi = {
  MpWrapperGetNumberOfProcessors,
  MpWrapperGetProcessorInfo,
  MpWrapperStartupAllAPs,
  MpWrapperStartupThisAP,
  MpWrapperSwitchBSP,
  MpWrapperEnableDisableAP,
  MpWrapperWhoAmI,
  MpWrapperStartupAllCPUs
};

EFI_STATUS
EFIAPI
SwitchStackGetNumberOfProcessors (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackGetProcessorInfo (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackStartupAllAPs (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackStartupThisAP (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackSwitchBSP (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackEnableDisableAP (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackWhoAmI (
  VOID
  );

EFI_STATUS
EFIAPI
SwitchStackStartupAllCPUs (
  VOID
  );

EFI_STATUS
EFIAPI
MpWrapperGetNumberOfProcessors (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  OUT UINTN                      *NumberOfProcessors,
  OUT UINTN                      *NumberOfEnabledProcessors
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->GetNumberOfProcessors (
                           mCpuMp2Ppi,
                           NumberOfProcessors,
                           NumberOfEnabledProcessors
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->GetNumberOfProcessors;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.Uintn1 = (UINTN ) NumberOfProcessors;
    mMpServiceParameter.Uintn2 = (UINTN ) NumberOfEnabledProcessors;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackGetNumberOfProcessors();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperGetProcessorInfo (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                      ProcessorNumber,
  OUT EFI_PROCESSOR_INFORMATION  *ProcessorInfoBuffer
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->GetProcessorInfo (
                           mCpuMp2Ppi,
                           ProcessorNumber,
                           ProcessorInfoBuffer
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->GetProcessorInfo;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.Uintn1 = ProcessorNumber;
    mMpServiceParameter.ProInfo = ProcessorInfoBuffer;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackGetProcessorInfo();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperStartupAllAPs (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  BOOLEAN                    SingleThread,
  IN  UINTN                      TimeoutInMicroSeconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->StartupAllAPs (
                           mCpuMp2Ppi,
                           Procedure,
                           SingleThread,
                           TimeoutInMicroSeconds,
                           ProcedureArgument
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->StartupAllAPs;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.ApPro = Procedure;
    mMpServiceParameter.MpBoolean = SingleThread;
    mMpServiceParameter.Uintn1 = TimeoutInMicroSeconds;
    mMpServiceParameter.Void = ProcedureArgument;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackStartupAllAPs();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperStartupThisAP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  UINTN                      ProcessorNumber,
  IN  UINTN                      TimeoutInMicroseconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->StartupThisAP (
                           mCpuMp2Ppi,
                           Procedure,
                           ProcessorNumber,
                           TimeoutInMicroseconds,
                           ProcedureArgument
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->StartupThisAP;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.ApPro = Procedure;
    mMpServiceParameter.Uintn1 = ProcessorNumber;
    mMpServiceParameter.Uintn2 = TimeoutInMicroseconds;
    mMpServiceParameter.Void = (UINTN *) ProcedureArgument;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackStartupThisAP();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperSwitchBSP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                      ProcessorNumber,
  IN  BOOLEAN                    EnableOldBSP
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->SwitchBSP (
                           mCpuMp2Ppi,
                           ProcessorNumber,
                           EnableOldBSP
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->SwitchBSP;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.Uintn1 = ProcessorNumber;
    mMpServiceParameter.MpBoolean = EnableOldBSP;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackSwitchBSP();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperEnableDisableAP (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  UINTN                      ProcessorNumber,
  IN  BOOLEAN                    EnableAP,
  IN  UINT32                     *HealthFlag OPTIONAL
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->EnableDisableAP (
                           mCpuMp2Ppi,
                           ProcessorNumber,
                           EnableAP,
                           HealthFlag
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->EnableDisableAP;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.Uintn1 = ProcessorNumber;
    mMpServiceParameter.MpBoolean = EnableAP;
    mMpServiceParameter.Uint32 = (UINT32 *) HealthFlag;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackEnableDisableAP();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperWhoAmI (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  OUT UINTN                      *ProcessorNumber
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->WhoAmI (
                           mCpuMp2Ppi,
                           ProcessorNumber
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->WhoAmI;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.Uintn1 = (UINTN) ProcessorNumber;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackWhoAmI();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}

EFI_STATUS
EFIAPI
MpWrapperStartupAllCPUs (
  IN  EDKII_PEI_MP_SERVICES2_PPI *This,
  IN  EFI_AP_PROCEDURE           Procedure,
  IN  UINTN                      TimeoutInMicroSeconds,
  IN  VOID                       *ProcedureArgument      OPTIONAL
  )
{
  mStatus = EFI_SUCCESS;

  if (mIsSwitchStackDone) {
    mStatus = mCpuMp2Ppi->StartupAllCPUs (
                           mCpuMp2Ppi,
                           Procedure,
                           TimeoutInMicroSeconds,
                           ProcedureArgument
                           );
  } else {
    mMpServiceParameter.CpuMpPpiFunction = (VOID *)(UINTN) mCpuMp2Ppi->StartupAllCPUs;
    mMpServiceParameter.CpuMp2Ppi = mCpuMp2Ppi;
    mMpServiceParameter.ApPro = Procedure;
    mMpServiceParameter.Uintn1 = TimeoutInMicroSeconds;
    mMpServiceParameter.Void = ProcedureArgument;

    mFspGlobalData->FunctionParameterPtr = &mMpServiceParameter;

    mIsSwitchStackDone = TRUE;
    mStatus = SwitchStackStartupAllCPUs();
    mIsSwitchStackDone = FALSE;
  }

  return mStatus;
}