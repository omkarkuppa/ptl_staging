/** @file
  This file provides services for TCSS PreMem policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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
#include <Library/DebugLib.h>
#include <Library/SiConfigBlockLib.h>
#include <Library/ConfigBlockLib.h>
#include <Ppi/SiPolicy.h>
#include <TcssPeiPreMemConfig.h>
#include <Library/PchInfoLib.h>
#include <Library/SocInfoLib.h>

/**
  Print TCSS_PEI_PREMEM_CONFIG and serial out.

  @param[in] SiPreMemPolicyPpi            Pointer to a SI_PREMEM_POLICY_PPI
**/
VOID
TcssPrintConfigPreMem (
  IN SI_PREMEM_POLICY_PPI     *SiPreMemPolicyPpi
  )
{
  EFI_STATUS               Status;
  TCSS_PEI_PREMEM_CONFIG   *TcssPeiPreMemConfig;
  UINTN                    Index;
  UINT8                    PortPolicyData8;

  Status = GetConfigBlock ((VOID *) SiPreMemPolicyPpi, &gTcssPeiPreMemConfigGuid, (VOID *) &TcssPeiPreMemConfig);
  ASSERT_EFI_ERROR (Status);

  DEBUG ((DEBUG_INFO, "----------------------- TCSS_PEI_PREMEM_CONFIG -----------------\n"));
  DEBUG ((DEBUG_INFO, " Revision : %d\n", TcssPeiPreMemConfig->Header.Revision));

  for (Index = 0; Index < MAX_IOM_AUX_BIAS_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomAuxPortPad[%x].ReceptacleSbu2BiasCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu2BiasCtrl));
    DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomAuxPortPad[%x].ReceptacleSbu1BiasCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu1BiasCtrl));
    DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomAuxPortPad[%x].AuxIsoCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl));
  }

  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri 0x%04X\n", TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri));
  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri 0x%04X\n", TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri));

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[%d] 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[Index]));
  }

  DEBUG ((DEBUG_INFO, " TCSS Feature Support Enabled Policy : %01x\n", TcssPeiPreMemConfig->TcssEnable));
  DEBUG ((DEBUG_INFO, " TCSS Port Enabled Policy : %01x\n", TcssPeiPreMemConfig->UsbTcConfig.PortEnData32));

  for (Index = 0; Index < GetPchMaxTypeCPortNum(); Index++) {
    PortPolicyData8 = TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[Index];
    DEBUG ((DEBUG_INFO, " TCSS Port[%d] Policy Configuration : ", Index));
    switch (PortPolicyData8) {
      case UsbCDisable:
        DEBUG ((DEBUG_INFO, "No Function\n"));
      break;
      case DpOnly:
        DEBUG ((DEBUG_INFO, "DP ALT and DP Tunneling\n"));
      break;
      case NoThunderbolt:
        DEBUG ((DEBUG_INFO, "USB3 Native and DP ALT\n"));
      break;
      case NoPcie:
        DEBUG ((DEBUG_INFO, "No PCIE\n"));
      break;
      case FullFunction:
        DEBUG ((DEBUG_INFO, "Full Function\n"));
      break;
      default:
        DEBUG ((DEBUG_INFO, "Unknown state\n"));
      break;
    }
  }
}

/**
  Load Config block default

  @param[in] ConfigBlockPointer         Pointer to config block
**/
VOID
TcssLoadConfigPreMemDefault (
  IN VOID          *ConfigBlockPointer
  )
{
  TCSS_PEI_PREMEM_CONFIG      *TcssPeiPreMemConfig;
  UINTN                       Index;

  TcssPeiPreMemConfig = (TCSS_PEI_PREMEM_CONFIG *)ConfigBlockPointer;

  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->Header.GuidHob.Name = %g\n", &TcssPeiPreMemConfig->Header.GuidHob.Name));
  DEBUG ((DEBUG_INFO, "TcssPeiPreMemConfig->Header.GuidHob.Header.HobLength = 0x%x\n", TcssPeiPreMemConfig->Header.GuidHob.Header.HobLength));

  TcssPeiPreMemConfig->TcssEnable = 1;
  // Default set all ports are Type C enabled
  for (Index = 0; Index < GetPchMaxTypeCPortNum(); Index++) {
    TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[Index] = FullFunction;
  }
}

STATIC COMPONENT_BLOCK_ENTRY  mTcssBlocksPreMem = {
  &gTcssPeiPreMemConfigGuid,
  sizeof (TCSS_PEI_PREMEM_CONFIG),
  TCSS_PEI_PREMEM_CONFIG_REVISION,
  TcssLoadConfigPreMemDefault
};

/**
  Get TCSS PreMem config block table size.

  @retval      Size of config block
**/
UINT16
TcssGetConfigBlockTotalSizePreMem (
  VOID
  )
{
  return mTcssBlocksPreMem.Size;
}

/**
  Add TCSS PreMem ConfigBlock.

  @param[in] ConfigBlockTableAddress    The pointer to config block table

  @retval EFI_SUCCESS                   The policy default is initialized.
  @retval EFI_OUT_OF_RESOURCES          Insufficient resources to create buffer
**/
EFI_STATUS
TcssAddConfigBlockPreMem (
  IN VOID           *ConfigBlockTableAddress
  )
{
  return AddComponentConfigBlocks (ConfigBlockTableAddress, &mTcssBlocksPreMem, 1);
}