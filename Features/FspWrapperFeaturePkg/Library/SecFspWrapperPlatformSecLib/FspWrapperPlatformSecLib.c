/** @file
  Provide FSP wrapper platform sec related function.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <PiPei.h>

#include <Ppi/SecPlatformInformation.h>
#include <Ppi/SecPerformance.h>
#include <Ppi/TopOfTemporaryRam.h>
#include <Ppi/SizeOfTemporaryRam.h>
#include <Ppi/PeiCoreFvLocation.h>
#include <Library/SecBoardInitLib.h>
#include <Library/LocalApicLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Register/Msr.h>
#include <Library/SerialPortLib.h>
#include <Library/IoLib.h>

UINT8 *
EFIAPI
AppendAdditionalPlatformPpi (
  IN     UINT8  *CopyDestinationPointer,
  IN OUT UINT8  *PpiIndex
  );

//
// Declare I/O Ports used to perform PCI Confguration Cycles
//
#define PCI_CONFIGURATION_ADDRESS_PORT  0xCF8
#define PCI_CONFIGURATION_DATA_PORT     0xCFC

//
// Offset 0 of B0:D0:F0
//
#define PCI_SA_DID_VID_ADDRESS          0x80000000

/**
  This interface conveys state information out of the Security (SEC) phase into PEI.

  @param[in]     PeiServices               Pointer to the PEI Services Table.
  @param[in,out] StructureSize             Pointer to the variable describing size of the input buffer.
  @param[out]    PlatformInformationRecord Pointer to the EFI_SEC_PLATFORM_INFORMATION_RECORD.

  @retval EFI_SUCCESS           The data was successfully returned.
  @retval EFI_BUFFER_TOO_SMALL  The buffer was too small.

**/
EFI_STATUS
EFIAPI
SecPlatformInformation (
  IN CONST EFI_PEI_SERVICES                     **PeiServices,
  IN OUT   UINT64                               *StructureSize,
     OUT   EFI_SEC_PLATFORM_INFORMATION_RECORD  *PlatformInformationRecord
  );

/**
  This interface conveys performance information out of the Security (SEC) phase into PEI.

  This service is published by the SEC phase. The SEC phase handoff has an optional
  EFI_PEI_PPI_DESCRIPTOR list as its final argument when control is passed from SEC into the
  PEI Foundation. As such, if the platform supports collecting performance data in SEC,
  this information is encapsulated into the data structure abstracted by this service.
  This information is collected for the boot-strap processor (BSP) on IA-32.

  @param[in]  PeiServices  The pointer to the PEI Services Table.
  @param[in]  This         The pointer to this instance of the PEI_SEC_PERFORMANCE_PPI.
  @param[out] Performance  The pointer to performance data collected in SEC phase.

  @retval EFI_SUCCESS  The data was successfully returned.

**/
EFI_STATUS
EFIAPI
SecGetPerformance (
  IN CONST EFI_PEI_SERVICES          **PeiServices,
  IN       PEI_SEC_PERFORMANCE_PPI   *This,
  OUT      FIRMWARE_SEC_PERFORMANCE  *Performance
  );

PEI_SEC_PERFORMANCE_PPI  mSecPerformancePpi = {
  SecGetPerformance
};

#if (FixedPcdGet8(PcdFspModeSelection) == 0) && (FixedPcdGet8(PcdFspDispatchModeUseFspPeiMain) == 1)
EFI_PEI_CORE_FV_LOCATION_PPI mPeiCoreFvLocationPpi = {
  (VOID *) (UINTN) (FixedPcdGet32 (PcdBiosAreaBaseAddress) + FixedPcdGet32 (PcdFlashFvFspMOffset))
};

EFI_PEI_PPI_DESCRIPTOR  mPeiCoreFvLocationPpiList[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gEfiPeiCoreFvLocationPpiGuid,
    &mPeiCoreFvLocationPpi
  }
};
#endif

EFI_PEI_PPI_DESCRIPTOR  mPeiSecPlatformPpi[] = {
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gTopOfTemporaryRamPpiGuid,
    NULL // To be patched later.
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI,
    &gSizeOfTemporaryRamPpiGuid,
    NULL // To be patched later.
  },
  {
    EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST,
    &gPeiSecPerformancePpiGuid,
    &mSecPerformancePpi
  },
};

/**
  A developer supplied function to perform platform specific operations.

  It's a developer supplied function to perform any operations appropriate to a
  given platform. It's invoked just before passing control to PEI core by SEC
  core. Platform developer may modify the SecCoreData passed to PEI Core.
  It returns a platform specific PPI list that platform wishes to pass to PEI core.
  The Generic SEC core module will merge this list to join the final list passed to
  PEI core.

  @param[in,out] SecCoreData           The same parameter as passing to PEI core. It
                                       could be overridden by this function.

  @return The platform specific PPI list to be passed to PEI core or
          NULL if there is no need of such platform specific PPI list.

**/
EFI_PEI_PPI_DESCRIPTOR *
EFIAPI
SecPlatformMain (
  IN OUT   EFI_SEC_PEI_HAND_OFF        *SecCoreData
  )
{
  EFI_PEI_PPI_DESCRIPTOR          *PpiList;
  UINT8                           PpiIndex;
  UINT8                           *CopyDestinationPointer;
  UINT32                          CpuId;
  MSR_IA32_BIOS_SIGN_ID_REGISTER  MsrSignId;
  UINT32                          SaId;

  AsmWriteMsr64 (MSR_IA32_BIOS_SIGN_ID, 0);
  AsmCpuid (1, &CpuId, 0, 0, 0);
  MsrSignId.Uint64 = AsmReadMsr64 (MSR_IA32_BIOS_SIGN_ID);

  IoWrite32 (PCI_CONFIGURATION_ADDRESS_PORT, PCI_SA_DID_VID_ADDRESS);
  SaId = IoRead32 (PCI_CONFIGURATION_DATA_PORT);

  DEBUG ((DEBUG_INFO, "FSP Wrapper BootFirmwareVolumeBase - 0x%x\n", SecCoreData->BootFirmwareVolumeBase));
  DEBUG ((DEBUG_INFO, "FSP Wrapper BootFirmwareVolumeSize - 0x%x\n", SecCoreData->BootFirmwareVolumeSize));
  DEBUG ((DEBUG_INFO, "FSP Wrapper TemporaryRamBase       - 0x%x\n", SecCoreData->TemporaryRamBase));
  DEBUG ((DEBUG_INFO, "FSP Wrapper TemporaryRamSize       - 0x%x\n", SecCoreData->TemporaryRamSize));
  DEBUG ((DEBUG_INFO, "FSP Wrapper PeiTemporaryRamBase    - 0x%x\n", SecCoreData->PeiTemporaryRamBase));
  DEBUG ((DEBUG_INFO, "FSP Wrapper PeiTemporaryRamSize    - 0x%x\n", SecCoreData->PeiTemporaryRamSize));
  DEBUG ((DEBUG_INFO, "FSP Wrapper StackBase              - 0x%x\n", SecCoreData->StackBase));
  DEBUG ((DEBUG_INFO, "FSP Wrapper StackSize              - 0x%x\n", SecCoreData->StackSize));
  DEBUG ((DEBUG_INFO, "FSP CPUID                          - 0x%08x\n", CpuId));
  DEBUG ((DEBUG_INFO, "FSP SAID                           - 0x%08x\n", SaId));
  DEBUG ((DEBUG_INFO, "FSP PatchID                        - 0x%08x\n", MsrSignId.Bits.MicrocodeUpdateSignature));

  InitializeApicTimer (0, (UINT32) -1, TRUE, 5);

  //
  // Use middle of Heap as temp buffer, it will be copied by caller.
  // Do not use Stack, because it will cause wrong calculation on stack by PeiCore
  //
  PpiList = (VOID *)((UINTN) SecCoreData->PeiTemporaryRamBase + (UINTN) SecCoreData->PeiTemporaryRamSize/2);
  CopyDestinationPointer = (UINT8 *) PpiList;
  PpiIndex = 0;
#if (FixedPcdGet8(PcdFspModeSelection) == 0) && (FixedPcdGetBool(PcdFspDispatchModeUseFspPeiMain) == 1)
  //
  // In Dispatch mode, wrapper should provide PeiCoreFvLocationPpi.
  //
  CopyMem (CopyDestinationPointer, mPeiCoreFvLocationPpiList, sizeof (mPeiCoreFvLocationPpiList));
  PpiIndex = 1;
  CopyDestinationPointer += sizeof (mPeiCoreFvLocationPpiList);
#endif
  CopyDestinationPointer = AppendAdditionalPlatformPpi (CopyDestinationPointer, &PpiIndex);
  CopyMem (CopyDestinationPointer, mPeiSecPlatformPpi, sizeof (mPeiSecPlatformPpi));
  //
  // Patch TopOfTemporaryRamPpi
  //
  PpiList[PpiIndex++].Ppi = (VOID *)((UINTN) SecCoreData->TemporaryRamBase + SecCoreData->TemporaryRamSize);
  //
  // Patch SizeOfTemporaryRamPpi
  //
  PpiList[PpiIndex].Ppi = (VOID *)((UINTN) SecCoreData->TemporaryRamSize);

  return PpiList;
}
