/** @file
  This file provides services for Itss policy function

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
#include <Library/PeiItssPolicyLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <InterruptConfig.h>
#include <IoApicConfig.h>

/**
  mPxRcConfig[] table contains data for 8259 routing (how PIRQx is mapped to IRQy).
  This information is used by systems which choose to use legacy PIC
  interrupt controller. Only IRQ3-7,9-12,14,15 are valid. Values from this table
  will be programmed into ITSS.PxRC registers.
**/
GLOBAL_REMOVE_IF_UNREFERENCED UINT8 mPxRcConfig[] = {
  11,  // PARC: PIRQA -> IRQ11
  10,  // PBRC: PIRQB -> IRQ10
  11,  // PCRC: PIRQC -> IRQ11
  11,  // PDRC: PIRQD -> IRQ11
  11,  // PERC: PIRQE -> IRQ11
  11,  // PFRC: PIRQF -> IRQ11
  11,  // PGRC: PIRQG -> IRQ11
  11   // PHRC: PIRQH -> IRQ11
};

/**
  Print PCH_INTERRUPT_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
InterruptPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS            Status;
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  UINTN                 Index;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gInterruptConfigGuid, (VOID *) &InterruptConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ Interrupt Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " Interrupt assignment:\n"));
  DEBUG ((DEBUG_INFO, "  Dxx:Fx INTx IRQ\n"));
  for (Index = 0; Index < InterruptConfig->NumOfDevIntConfig; Index++) {
    DEBUG ((DEBUG_INFO, "  D%02d:F%d    %d %03d\n",
            InterruptConfig->DevIntConfig[Index].Device,
            InterruptConfig->DevIntConfig[Index].Function,
            InterruptConfig->DevIntConfig[Index].IntX,
            InterruptConfig->DevIntConfig[Index].Irq));
  }

  DEBUG ((DEBUG_INFO, " Other interrupt configuration:\n"));
  DEBUG ((DEBUG_INFO, "  GpioIrqRoute= %d\n", InterruptConfig->GpioIrqRoute));
  DEBUG ((DEBUG_INFO, "  SciIrqSelect= %d\n", InterruptConfig->SciIrqSelect));
  DEBUG ((DEBUG_INFO, "  TcoIrqEnable= %d\n", InterruptConfig->TcoIrqEnable));
  DEBUG ((DEBUG_INFO, "  TcoIrqSelect= %d\n", InterruptConfig->TcoIrqSelect));
}

/**
  Print PCH_IOAPIC_CONFIG and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
IoApicPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  EFI_STATUS            Status;
  PCH_IOAPIC_CONFIG     *IoApicConfig;

  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gIoApicConfigGuid, (VOID *) &IoApicConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "------------------ IOAPIC Config ------------------\n"));
  DEBUG ((DEBUG_INFO, " IoApicEntry24_119= %x\n", IoApicConfig->IoApicEntry24_119));
  DEBUG ((DEBUG_INFO, " Enable8254ClockGating= %x\n", IoApicConfig->Enable8254ClockGating));
  DEBUG ((DEBUG_INFO, " Enable8254ClockGatingOnS3= %x\n", IoApicConfig->Enable8254ClockGatingOnS3));
  DEBUG ((DEBUG_INFO, " IoApicId= %x\n", IoApicConfig->IoApicId));
}

/**
  Print Itss configs and serial out.

  @param[in] SiPolicyPpi            Pointer to a SI_POLICY_PPI
**/
VOID
ItssPrintConfig (
  IN SI_POLICY_PPI     *SiPolicyPpi
  )
{
  InterruptPrintConfig (SiPolicyPpi);
  IoApicPrintConfig (SiPolicyPpi);
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
IoApicLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_IOAPIC_CONFIG  *IoApicConfig;
  IoApicConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "IoApicConfig->Header.GuidHob.Name = %g\n", &IoApicConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "IoApicConfig->Header.GuidHob.Header.HobLength = 0x%x\n", IoApicConfig->Header.GuidHob.Header.HobLength));

  /********************************
    Io Apic configuration
  ********************************/
  IoApicConfig->IoApicId = 0x02;
  IoApicConfig->IoApicEntry24_119 = TRUE;
  IoApicConfig->Enable8254ClockGating = TRUE;
  IoApicConfig->Enable8254ClockGatingOnS3 = TRUE;
}

STATIC COMPONENT_BLOCK_ENTRY  mItssIoApicBlocks = {
  &gIoApicConfigGuid,
  sizeof (PCH_IOAPIC_CONFIG),
  PCH_IOAPIC_CONFIG_REVISION,
  IoApicLoadConfigDefault
};

/**
  Get Pch IoApic config block table size.

  @retval      Size of config block
**/
UINT16
GetIoApicConfigBlockTotalSize (
  VOID
  )
{
  return mItssIoApicBlocks.Size;
}

/**
  Add Pch IoApic ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
IoApicAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mItssIoApicBlocks, 1);
}

/**
  Get Interrupt config block table size.

  @retval      Size of config block

**/
UINT16
GetInterruptConfigBlockTotalSize (
  VOID
  )
{
  return sizeof (PCH_INTERRUPT_CONFIG);
}


/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
InterruptLoadConfigDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  PCH_INTERRUPT_CONFIG  *InterruptConfig;
  InterruptConfig = ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Name = %g\n", &InterruptConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "InterruptConfig->Header.GuidHob.Header.HobLength = 0x%x\n", InterruptConfig->Header.GuidHob.Header.HobLength));

  LoadDeviceInterruptConfig (&InterruptConfig->NumOfDevIntConfig, InterruptConfig->DevIntConfig);

  InterruptConfig->GpioIrqRoute = 14;
  InterruptConfig->SciIrqSelect = 9;
  InterruptConfig->TcoIrqSelect = 9;
}

STATIC COMPONENT_BLOCK_ENTRY  mItssBlocks = {
  &gInterruptConfigGuid,
  sizeof (PCH_INTERRUPT_CONFIG),
  PCH_INTERRUPT_CONFIG_REVISION,
  InterruptLoadConfigDefault
};

/**
  Add Interrupt ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
InterruptAddConfigBlock (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mItssBlocks, 1);
}