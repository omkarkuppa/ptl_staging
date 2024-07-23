/** @file
  The DXE TCSS Init Fru Library Implements After Memory PEIM

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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

#include <Base.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/DebugLib.h>
#include <TcssDataHob.h>
#include <Ppi/SiPolicy.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <IndustryStandard/Pci22.h>
#include <Protocol/TcssNvsArea.h>
#include <Library/PcdLib.h>
#include <Library/PciExpressHelpersLib.h>
#include <Library/BaseMemoryLib.h>
#include <PcieRegs.h>
#include <Register/HostDmaRegs.h>
#include <Register/CpuUsbRegs.h>
#include <Register/ItbtPcieRegs.h>
#include <Register/UsbDeviceRegs.h>
#include <Library/DxeSaNvsInitLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/HostBridgeDataLib.h>
#include <Register/HostBridgeRegs.h>
#include <Library/AslUpdateLib.h>

/**
This function performs Itbt PCI initialization before EndOfDxe.

**/
VOID
TcssIbtPcieConfigPciEnumComplete (
  VOID
  )
{
  UINT8                              Index;
  UINTN                              MchBar;
  UINT8                              RootPortFn;
  TCSS_DATA_HOB                      *TcssHob;
  LTR_LIMIT                          PolicyLtr;
  IOC_PMC_DEVEN_C_IOC_MCHBAR_STRUCT  TcssDevenCfg;
  UINTN                              BdfAllocRegOffset;
  BDF_ALLOC_C_0_0_IOC_MCHBAR_STRUCT  BdfAllocCfg;

  ///
  /// Update TCSS Device Enable status to TCSS HOB
  ///
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  ZeroMem (&PolicyLtr, sizeof (LTR_LIMIT));

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBaseLow);
  TcssDevenCfg.Data = MmioRead32 (MchBar + IOC_PMC_DEVEN_C_IOC_MCHBAR_REG);

  DEBUG((DEBUG_INFO, "TcssIbtPcieConfigPciEnumComplete ()\n"));

  ///
  /// Initialize TBT PCI Root Ports
  ///
  for (Index = 0; Index < MAX_ITBT_PCIE_PORT; Index++) {
    if ((TcssDevenCfg.Data >> Index) & BIT0) {
      ///
      /// BdfAllocRegOffset = BDF_ALLOC_C_0_0_IOC_MCHBAR_REG + (PMC_DEVEN_C[i] * 4),
      /// PMC_DEVEN_C[i] = Index
      ///
      BdfAllocRegOffset = BDF_ALLOC_C_0_0_IOC_MCHBAR_REG + (Index * 4);
      BdfAllocCfg.Data =  MmioRead32 (MchBar + BdfAllocRegOffset);
      RootPortFn = (UINT8)(BdfAllocCfg.Bits.func << BDF_ALLOC_C_0_0_IOC_MCHBAR_FUNC_LSB);

      DEBUG((DEBUG_INFO, "TcssRootportDownstreamPmConfiguration %x\n", RootPortFn));

      if (TcssHob != NULL) {
        PolicyLtr.MaxSnoopLatencyScale   = (TcssHob->PcieItbtLtrData[Index].LtrMaxSnoopLatency & 0x1C00) >> 10;
        PolicyLtr.MaxSnoopLatencyValue   = TcssHob->PcieItbtLtrData[Index].LtrMaxSnoopLatency & 0x3FF;
        PolicyLtr.MaxNoSnoopLatencyScale = (TcssHob->PcieItbtLtrData[Index].LtrMaxNoSnoopLatency & 0x1C00) >> 10;
        PolicyLtr.MaxNoSnoopLatencyValue = TcssHob->PcieItbtLtrData[Index].LtrMaxNoSnoopLatency & 0x3FF;
      }

      TcssRootportDownstreamPmConfiguration (
        0,
        SA_MC_BUS,
        (UINT8)ITBT_PCIE_DEV_NUM,
        (UINT8)RootPortFn,
        PolicyLtr
      );
    }
  }
}

