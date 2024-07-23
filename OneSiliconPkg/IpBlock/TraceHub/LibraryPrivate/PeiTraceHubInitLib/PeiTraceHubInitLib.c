/** @file
  TraceHub Init PostMem Lib

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
#include <Uefi/UefiBaseType.h>
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PeiTraceHubInitLib.h>
#include <Library/DebugLib.h>
#include <IndustryStandard/Pci22.h>
#include <Register/TraceHubRegs.h>
#include <Library/MtrrLib.h>

/**
  Configure TraceHub Msc operational region to corresponding memory base/size and return next MSC base address

  @param[in] TraceHubHandle        Pointer to TraceHub handle structure
  @param[in] TraceHubMemBase       Pointer to TraceHub memory base
**/
STATIC
VOID
TraceHubConfigMsc (
  IN TRACEHUB_HANDLE               *TraceHubHandle,
  IN UINT64                        *TraceHubMemBase
  )
{
  UINT32                           TraceHubMtbBar;

  TraceHubMtbBar = PciSegmentRead32 (TraceHubHandle->Controller->PciCfgBaseAddr + R_TRACE_HUB_CFG_MTB_LBAR) & B_TRACE_HUB_CFG_MTB_LBAR_ADDR;

  if (TraceHubHandle->TraceHubConfig->EnableMode != TraceHubModeDisabled) {
    ///
    ///  Program MSC0BAR, MSC0SIZE, MSC1BAR, MSC1SIZE
    ///
    ///  Write the base address and buffer size for Memory Region 0 to the the MSC0BAR and MSC0SIZE registers.
    ///  Write the base address and buffer size for Memory Region 1 to the the MSC1BAR and MSC1SIZE registers.
    ///  Note that BAR and Size are shifted left 12 bits by the hardware to arrive at physical starting address and size.
    ///  BIOS, therefore, should shift-right by 12 bits before writing a value to these registers.
    ///  This also forces the starting address and size to be on 4kB boundaries.
    ///
    if (TraceHubHandle->TraceHubConfig->MemReg0Size != 0) {
      DEBUG ((DEBUG_INFO, "TraceHubMemBase = 0x%lx\n", *TraceHubMemBase));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0UBAR, (UINT32) RShiftU64 (*TraceHubMemBase, 44));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0BAR, (UINT32) (RShiftU64 (*TraceHubMemBase, 12) & 0xFFFFFFFF));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0DESTSZ, (UINT32) RShiftU64 (TraceHubHandle->TraceHubConfig->MemReg0Size, 12));
      DEBUG ((DEBUG_INFO, "MSC0 Upper Base Address Register [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0UBAR, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0UBAR)));
      DEBUG ((DEBUG_INFO, "MSC0 Base Address Register       [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0BAR, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0BAR)));
      DEBUG ((DEBUG_INFO, "MSC0 Memory Buffer Size          [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0DESTSZ, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC0DESTSZ)));
      *TraceHubMemBase += TraceHubHandle->TraceHubConfig->MemReg0Size;
    }
    if (TraceHubHandle->TraceHubConfig->MemReg1Size != 0) {
      DEBUG ((DEBUG_INFO, "TraceHubMemBase = 0x%lx\n", *TraceHubMemBase));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1UBAR, (UINT32) RShiftU64 (*TraceHubMemBase, 44));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1BAR, (UINT32) (RShiftU64 (*TraceHubMemBase, 12) & 0xFFFFFFFF));
      MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1DESTSZ, (UINT32) RShiftU64 (TraceHubHandle->TraceHubConfig->MemReg1Size, 12));
      DEBUG ((DEBUG_INFO, "MSC1 Upper Base Address Register [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1UBAR, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1UBAR)));
      DEBUG ((DEBUG_INFO, "MSC1 Base Address Register       [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1BAR, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1BAR)));
      DEBUG ((DEBUG_INFO, "MSC1 Memory Buffer Size          [%x] = 0x%x\n", TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1DESTSZ, MmioRead32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSC1DESTSZ)));
      *TraceHubMemBase += TraceHubHandle->TraceHubConfig->MemReg1Size;
    }
    //
    // Set MSU to Root Space 3
    //
    MmioWrite32 (TraceHubMtbBar + R_TRACE_HUB_MEM_MSUGCTL, TraceHubHandle->PrivateConfig->RootSpace);
  }
}

/**
  TraceHub initialization in PostMem.

  Configure TraceHub memory and device mode

  @param[in] TraceHubHandle          Pointer to TraceHub handle structure
  @param[in, out] TraceHubMemBase    Pointer TraceHub memory base
**/
VOID
TraceHubInitPostMem (
  IN TRACEHUB_HANDLE           *TraceHubHandle,
  IN OUT UINT64                *TraceHubMemBase
  )
{
  EFI_STATUS                   Status;
  UINT32                       TraceHubMtbBar;

  Status = EFI_SUCCESS;
  TraceHubMtbBar = 0;

  DEBUG ((DEBUG_INFO, "TraceHubInitPostMem Device B%d:D%d:F%d\n", TraceHubHandle->Controller->Bus, TraceHubHandle->Controller->Device, TraceHubHandle->Controller->Function));

  if (PciSegmentRead16 (TraceHubHandle->Controller->PciCfgBaseAddr) == 0xFFFF) {
    DEBUG ((DEBUG_INFO, "TraceHubDevice does not exist\n"));
    return;
  }

  if (TraceHubHandle->PrivateConfig->RtitBar > 0) {
    //
    // Set RTIT BAR as WC
    //
    Status = MtrrSetMemoryAttribute (TraceHubHandle->PrivateConfig->RtitBar, TraceHubHandle->PrivateConfig->RtitSize, CacheWriteCombining);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Set RTIT BAR MTRR attribute WC fail, Status = %r\n", Status));
    }
  }

  PciSegmentOr8 (TraceHubHandle->Controller->PciCfgBaseAddr + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE | EFI_PCI_COMMAND_BUS_MASTER);
  ///
  /// Set SCRPD2[15:8] to indicate RS3 PCI Bus number, MUST be written with a single-byte write
  ///
  TraceHubMtbBar = PciSegmentRead32 (TraceHubHandle->Controller->PciCfgBaseAddr + R_TRACE_HUB_CFG_MTB_LBAR) & B_TRACE_HUB_CFG_MTB_LBAR_ADDR;
  MmioWrite8 (TraceHubMtbBar + (R_TRACE_HUB_MEM_SCRPD2 + 1), TraceHubHandle->Controller->Bus);
  DEBUG ((DEBUG_INFO, "Scrpd2[15:8] [%x] = 0x%02x\n", TraceHubMtbBar + (R_TRACE_HUB_MEM_SCRPD2 + 1), MmioRead8 (TraceHubMtbBar + (R_TRACE_HUB_MEM_SCRPD2 + 1))));

  //
  // Fill BAR and size to each TraceHub MSC0/1 registers
  //
  if (*TraceHubMemBase > 0) {
    TraceHubConfigMsc (TraceHubHandle, TraceHubMemBase);
  }
}
