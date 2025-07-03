/** @file
  Initialize FUSA device in PEI

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Ppi/SiPolicy.h>
#include <FusaConfig.h>
#include <Register/Cpuid.h>
#include <Library/PciSegmentLib.h>
#include <Library/IoLib.h>
#include <Library/PeiFusaLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/MpServices2.h>
#include <Core/Pei/PeiMain.h>
#include <Library/PeiServicesLib.h>
#include <Library/HobLib.h>
#include <Library/TimerLib.h>
#include <Register/Ptl/Msr/MsrRegs.h>
#include <Library/FusaInfoLib.h>
#include <Library/PeiFusaRuntimeLib.h>

extern EFI_GUID                   gEdkiiPeiMpServices2PpiGuid;
STATIC EDKII_PEI_MP_SERVICES2_PPI *gMpServices2Ppi = NULL;
UINTN                             mNumberOfProcessors;
UINTN                             mNumberOfEnabledProcessors;
UINT32                            gTestData;
UINT64                            gfusa_capability;
LOCKSTEP_CONFIG                   gLockStepConfig;
volatile BOOLEAN                  gGlobalApEnable;
#define                           MAX_DKT_COUNT   12
UINT8                             DKT_MOD8_LPIDx_APIC_VAR[MAX_DKT_COUNT]  = {0x20, 0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x40, 0x42, 0x44, 0x46}; //these only covers HPC Module0 and HPC Module1, need to include LPC ATOM
UINT8                             Total_Atom_Count  = 0;

/**
  Execute FusaAddressLock

  @param[in] FusaConfig
**/
VOID
EFIAPI
FusaAddressLock (
  IN  FUSA_CONFIG    *FusaConfig
  )
{
  UINTN                                      MyCpuNumber;
  EFI_STATUS                                 Status = EFI_SUCCESS;
  UINT8                                      CurrProcessor;
  EFI_PROCESSOR_INFORMATION                  MpContext;
  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX    Eax;
  UINT64                                     fusa_capability;

  //
  // Locate MP protocol, enumerate AP and update MSR accordingly
  //
  Status = gMpServices2Ppi->WhoAmI (
                        gMpServices2Ppi,
                        &MyCpuNumber
                        );

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", MyCpuNumber));
  }

  for (CurrProcessor = 0; CurrProcessor < mNumberOfProcessors; CurrProcessor++) {
    Status = gMpServices2Ppi->GetProcessorInfo (
                          gMpServices2Ppi,
                          CurrProcessor,
                          &MpContext
                          );

    if ((MpContext.StatusFlag & PROCESSOR_ENABLED_BIT) != PROCESSOR_ENABLED_BIT)
    {
      DEBUG ((DEBUG_INFO, "Skip this CPU =  %d\n", CurrProcessor));
      continue;
    }

    Status = gMpServices2Ppi->SwitchBSP (
                          gMpServices2Ppi,
                          CurrProcessor,
                          TRUE
                          );

    DEBUG((DEBUG_INFO, "The CurrProcessor 0x%x ApicId is 0x%x\n", CurrProcessor, MpContext.ProcessorId));

    AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);
    fusa_capability = AsmReadMsr64 (MSR_INTEGRITY_CAPABILITIES);

    DEBUG((DEBUG_INFO, "fusa_capability 0x%llx\n", (UINT64) fusa_capability));
    AsmWriteMsr64(MSR_FUSARR_MASK, (UINT64)BIT10);
    DEBUG((DEBUG_INFO, "Fusa Addr Lock bit set 0x%x\n", CurrProcessor ));

  }

  DEBUG ((DEBUG_INFO, "Switch back to Main core's CpuNumber =  %x\n", MyCpuNumber));
  Status = gMpServices2Ppi->SwitchBSP (
                        gMpServices2Ppi,
                        MyCpuNumber,
                        TRUE
                        );
}

/**
  Get the register address of CFI
**/
EFI_STATUS
CfiRegisterAddress (
  IN UINT16                      PortId,
  IN OUT UINT64                  *InputOutputData
  )
{
  UINT64        BaseAddress;
  UINT32        BarL;
  UINT64        RegisterAddress;
  UINT64        AddressOffset;
  UINT64        BarAddress;

  if (InputOutputData == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  AddressOffset = *InputOutputData;
  switch (PortId) {
    case PORTID_HBO0:
    case PORTID_HBO1:
    case PORTID_SNCU:
    case PORTID_VTU:
    case PORTID_PUNIT:
    case PORTID_IPU_PB_PID:
    case PORTID_VPU_PB_PID:
    case PORTID_MEDIA_PB:
    case PORTID_IAX_PB:
    case PORTID_DNI2CFI:
    case PORTID_IDIB:
    case PORTID_CCE0:
    case PORTID_CCE1:
    case PORTID_CCF_SANTA1:
    case PORTID_CCF_SANTA0:
    {
      BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG);
      BarL = (PciSegmentRead32 (BaseAddress));
      if ((BarL & DEV0_BAR_EN) == DEV0_BAR_EN) {
        BarL &= (~DEV0_BAR_EN);
        BarAddress = BarL | (LShiftU64 (PciSegmentRead32 (BaseAddress + 4), 32));
        RegisterAddress = BarAddress;
        RegisterAddress += SAFBAR_MEMORY_ADDRESS (SEGMENT_ID_C_DIE, PortId, AddressOffset);
        DEBUG ((DEBUG_INFO, "%a: SAFBAR :0x%016lx, RegisterAddress:0x%016lx\n", __FUNCTION__, BarAddress, RegisterAddress));
        *InputOutputData = RegisterAddress;
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "%a: SAFBAR is not programmed!\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
      break;
    }

    case PORTID_GT_PB:
    {
      BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG);
      BarL = (PciSegmentRead32 (BaseAddress));
      if ((BarL & DEV0_BAR_EN) == DEV0_BAR_EN) {
        BarL &= (~DEV0_BAR_EN);
        BarAddress = BarL | (LShiftU64 (PciSegmentRead32 (BaseAddress + 4), 32));
        RegisterAddress = BarAddress;
        RegisterAddress += SAFBAR_MEMORY_ADDRESS (SEGMENT_ID_G_DIE, PortId, AddressOffset);
        DEBUG ((DEBUG_INFO, "%a: SAFBAR :0x%016lx, RegisterAddress:0x%016lx\n", __FUNCTION__, BarAddress, RegisterAddress));
        *InputOutputData = RegisterAddress;
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "%a: SAFBAR is not programmed!\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
      break;
    }

    case PORTID_IOC:
    case PORTID_IOCCE:
    case PORTID_IVTU:
    {
      BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG);
      BarL = (PciSegmentRead32 (BaseAddress));
      if ((BarL & DEV0_BAR_EN) == DEV0_BAR_EN) {
        BarL &= (~DEV0_BAR_EN);
        BarAddress = BarL | (LShiftU64 (PciSegmentRead32 (BaseAddress + 4), 32));
        RegisterAddress = BarAddress;
        RegisterAddress += SAFBAR_MEMORY_ADDRESS (SEGMENT_ID_IOE, PortId, AddressOffset);
        DEBUG ((DEBUG_INFO, "%a: SAFBAR :0x%016lx, RegisterAddress:0x%016lx\n", __FUNCTION__, BarAddress, RegisterAddress));
        *InputOutputData = RegisterAddress;
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "%a: SAFBAR is not programmed!\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
      break;
    }

    default:
      DEBUG ((DEBUG_INFO, "%a: Port id %X is not support!\n", __FUNCTION__, PortId));
      break;
  }
  return EFI_UNSUPPORTED;
}

/**
  Initialize FUSA Before the MCHECK

  @param[in] FUSA_CONFIG      FusaConfig

  @retval EFI_SUCCESS          - FUSA initialization complete
**/
EFI_STATUS
FusaInitOnEndOfPei (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  EFI_STATUS                                 Status;
  FUSA_CONFIG                                *FusaConfig;
  CPUID_NATIVE_MODEL_ID_AND_CORE_TYPE_EAX    Eax;
  UINT8                                      CurrProcessor;
  UINT8                                      CurrAtomProcessor;
  UINTN                                      MyCpuNumber;
  EFI_PROCESSOR_INFORMATION                  MpContext;

  CurrAtomProcessor = 0;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);

  // Locate CpuMpCpu MpService Ppi
  Status = PeiServicesLocatePpi (
            &gEdkiiPeiMpServices2PpiGuid,
            0,
            NULL,
            (VOID **) &gMpServices2Ppi
            );
  //
  // Locate MP protocol
  //
  Status = gMpServices2Ppi->GetNumberOfProcessors (gMpServices2Ppi, (UINTN *) &mNumberOfProcessors, (UINTN *) &mNumberOfEnabledProcessors);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR getting number of processor! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "NumberOfProcessors =  %x\n", mNumberOfProcessors));
    DEBUG ((DEBUG_INFO, "NumberOfEnabledProcessors =  %x\n", mNumberOfEnabledProcessors));
  }

  //
  // Locate MP protocol, enumerate AP and update MSR accordingly
  //
  Status = gMpServices2Ppi->WhoAmI (
                        gMpServices2Ppi,
                        &MyCpuNumber
                        );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "ERROR who am I! Status: %x\n", Status));
  } else {
    DEBUG ((DEBUG_INFO, "Main core's CpuNumber =  %x\n", MyCpuNumber));
  }

  for (CurrProcessor = 0; CurrProcessor < mNumberOfProcessors; CurrProcessor++) {
    Status = gMpServices2Ppi->GetProcessorInfo (
                          gMpServices2Ppi,
                          CurrProcessor,
                          &MpContext
                          );

    DEBUG((DEBUG_INFO, "The CurrProcessor 0x%x ApicId is 0x%x\n", CurrProcessor, MpContext.ProcessorId));
    Status = gMpServices2Ppi->SwitchBSP (
                          gMpServices2Ppi,
                          CurrProcessor,
                          TRUE
                          );

    AsmCpuid (CPUID_HYBRID_INFORMATION, &Eax.Uint32, NULL, NULL, NULL);

    if((UINT8)Eax.Bits.CoreType == CPUID_CORE_TYPE_INTEL_ATOM) {
      gfusa_capability = AsmReadMsr64 (MSR_INTEGRITY_CAPABILITIES);
      DEBUG ((DEBUG_INFO, "FUSA CAPABILITY  = %x\n", (UINT32)gfusa_capability));
      DKT_MOD8_LPIDx_APIC_VAR[CurrAtomProcessor] = (UINT8)MpContext.ProcessorId;
      Total_Atom_Count++;
      CurrAtomProcessor++;
    }
  }
  DEBUG ((DEBUG_INFO, "Total Atom Core =  %d\n", Total_Atom_Count));
  DEBUG ((DEBUG_INFO, "Switch back to Main core's CpuNumber =  %x\n", MyCpuNumber));
  Status = gMpServices2Ppi->SwitchBSP (
                      gMpServices2Ppi,
                      MyCpuNumber,
                      TRUE
                      );


  FusaAddressLock(FusaConfig);

  return EFI_SUCCESS;
}

/**
  Initialize FUSA

  @param[in] FUSA_CONFIG      FusaConfig

  @retval EFI_SUCCESS          - FUSA initialization complete
**/
EFI_STATUS
FusaInit (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  EFI_STATUS                  Status;
  FUSA_CONFIG                 *FusaConfig;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);
  ASSERT_EFI_ERROR (Status);
  if (FusaConfig->FusaConfigEnable == 1) {
  } else {
    DEBUG ((DEBUG_INFO, "Fusa is disabled\n"));
  }


  return EFI_SUCCESS;
}

/**
  Fusa Get Startup Pattern Addr

  @param[in] SI_POLICY_PPI      SiPolicy

  @retval UINT32          - FUSA Startup Pattern Addr
**/
UINT32
FusaStartupPatternAddr (
  IN  SI_POLICY_PPI    *SiPolicy
  )
{
  FUSA_CONFIG                 *FusaConfig;
  EFI_STATUS                  Status;

  Status = GetConfigBlock ((VOID *) SiPolicy, &gFusaConfigGuid, (VOID *) &FusaConfig);

  if (EFI_ERROR (Status)) {
    return 0;
  }
  
  return FusaConfig->FusaStartupPatternAddr;
}
