/** @file
  This file provides services for TCSS PreMem policy function

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
#include <TcssPeiPreMemConfig.h>
#include <TcssInfo.h>

/**
  Print TCSS_PEI_PREMEM_CONFIG and serial out.

  @param[in] TcssPeiPreMemConfig          Pointer to a TCSS_PEI_PREMEM_CONFIG
  @param[in] MaxTcssPortNumber            Max number of TCSS Root Ports
**/
VOID
TcssPreMemConfigDump (
  IN TCSS_PEI_PREMEM_CONFIG     *TcssPeiPreMemConfig,
  IN UINT8                      MaxTcssPortNumber
  )
{
  UINTN                    Index;
  UINT8                    PortPolicyData8;

  if ((TcssPeiPreMemConfig == NULL) || (MaxTcssPortNumber == 0)) {
    DEBUG ((DEBUG_INFO, "The TcssPeiPreMemConfig is NULL!!"));
    return;
  }

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));
  DEBUG ((DEBUG_INFO, "Revision : %d\n", TcssPeiPreMemConfig->Header.Revision));
  DEBUG ((DEBUG_INFO, "IomAuxPortPad\n"));
  for (Index = 0; Index < MAX_IOM_AUX_BIAS_COUNT; Index++) {
    DEBUG ((DEBUG_INFO, "[%x].ReceptacleSbu2BiasCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu2BiasCtrl));
    DEBUG ((DEBUG_INFO, "[%x].ReceptacleSbu1BiasCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].ReceptacleSbu1BiasCtrl));
    DEBUG ((DEBUG_INFO, "[%x].AuxIsoCtrl 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomAuxPortPad[Index].AuxIsoCtrl));
  }

  DEBUG ((DEBUG_INFO, "AuxOri 0x%04X\n", TcssPeiPreMemConfig->IomConfig.IomOverrides.AuxOri));
  DEBUG ((DEBUG_INFO, "HslOri 0x%04X\n", TcssPeiPreMemConfig->IomConfig.IomOverrides.HslOri));

  for (Index = 0; Index < MAX_TCSS_USB3_PORTS; Index++) {
    DEBUG ((DEBUG_INFO, "IomUsbCDpConfig[%d] 0x%08x\n", Index, TcssPeiPreMemConfig->IomConfig.IomUsbCDpConfig[Index]));
  }

  DEBUG ((DEBUG_INFO, "TcssEnable : %01x\n", TcssPeiPreMemConfig->TcssEnable));
  DEBUG ((DEBUG_INFO, "TPortEnData32 : %01x\n", TcssPeiPreMemConfig->UsbTcConfig.PortEnData32));

  for (Index = 0; Index < MaxTcssPortNumber; Index++) {
    PortPolicyData8 = TcssPeiPreMemConfig->UsbTcConfig.PortIndex.CapPolicy[Index];
    DEBUG ((DEBUG_INFO, "TCSS Port[%d] Policy Configuration : ", Index));
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