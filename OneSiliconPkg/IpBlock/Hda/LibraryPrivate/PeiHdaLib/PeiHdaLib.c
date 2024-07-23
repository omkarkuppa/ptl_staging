/** @file
  PEI PCH HD Audio Library implementation.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2018 Intel Corporation.

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
#include <Library/PeiServicesLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/IoLib.h>
#include <Library/TimerLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/PeiHdaLib.h>
#include <IndustryStandard/Pci30.h>
#include <Ppi/SiPolicy.h>
#include <Register/PchRegs.h>
#include <Register/HdaRegs.h>

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling (
  IN      UINTN           StatusReg,
  IN      UINT32          PollingBitMap,
  IN      UINT32          PollingData
  )
{
  UINT32  LoopTime;

  for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
    if ((MmioRead32 (StatusReg) & PollingBitMap) == PollingData) {
      break;
    } else {
      MicroSecondDelay (HDA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= HDA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Polling the Status bit.
  Maximum polling time (us) equals HDA_MAX_LOOP_TIME * HDA_WAIT_PERIOD.

  @param[in] StatusReg            The register address to read the status
  @param[in] PollingBitMap        The bit mapping for polling
  @param[in] PollingData          The Data for polling

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_TIMEOUT             Polling the bit map time out
**/
EFI_STATUS
StatusPolling16 (
  IN      UINTN           StatusReg,
  IN      UINT16          PollingBitMap,
  IN      UINT16          PollingData
  )
{
  UINT32  LoopTime;

  for (LoopTime = 0; LoopTime < HDA_MAX_LOOP_TIME; LoopTime++) {
    if ((MmioRead16 (StatusReg) & PollingBitMap) == PollingData) {
      break;
    } else {
      MicroSecondDelay (HDA_WAIT_PERIOD);
    }
  }

  if (LoopTime >= HDA_MAX_LOOP_TIME) {
    return EFI_TIMEOUT;
  }

  return EFI_SUCCESS;
}

/**
  Print HDA handle, controller and callbacks information.

  @param[in] HdaHandle          Pointer to Hda controller configuration structure
**/
VOID
PrintHdaHandle (
  IN  HDA_HANDLE                *HdaHandle
  )
{
  HDA_CONTROLLER               *HdaController;
  HDA_PRIVATE_CONFIG           *HdaPrivateConfig;

  if (HdaHandle == NULL) {
    DEBUG ((DEBUG_WARN, "%a (): HdaHandle is NULL pointer\n", __FUNCTION__));
    return;
  }

  HdaController    = HdaHandle->HdaController;
  HdaPrivateConfig = HdaHandle->HdaPrivateConfig;

  if (HdaController != NULL) {
    DEBUG ((DEBUG_INFO, "\n---------------- HDA Controller ----------------\n"));
    DEBUG ((DEBUG_INFO, "  PCI config space address  = 0x%08x\n",              HdaController->PciCfgBaseAddr));
    DEBUG ((DEBUG_INFO, "  S:B:D:F                   = %02x:%02x:%02x:%02x\n", HdaController->Segment, HdaController->Bus, HdaController->Device, HdaController->Function));
    DEBUG ((DEBUG_INFO, "  Hda bar                   = 0x%08x\n",              HdaController->HdaMmioAddress));
    DEBUG ((DEBUG_INFO, "  Dsp bar                   = 0x%08x\n",              HdaController->DspMmioAddress));
  }

  if (HdaPrivateConfig != NULL) {
    DEBUG ((DEBUG_INFO, "\n---------------- HDA Private Config ----------------\n"));
    DEBUG ((DEBUG_INFO, "  Node ID                   = 0x%08x\n",              HdaPrivateConfig->NodeId));
    DEBUG ((DEBUG_INFO, "  Max SNDW link number      = 0x%02x\n",              HdaPrivateConfig->HdaMaxSndwLinkNum));
    DEBUG ((DEBUG_INFO, "  Max DMIC link number      = 0x%02x\n",              HdaPrivateConfig->HdaMaxDmicLinkNum));
    DEBUG ((DEBUG_INFO, "  Max SSP link number       = 0x%02x\n",              HdaPrivateConfig->HdaMaxSspLinkNum));
    DEBUG ((DEBUG_INFO, "  IGPU enabled              = 0x%02x\n",              HdaPrivateConfig->IGfxEnabled));
    DEBUG ((DEBUG_INFO, "  Skip IGPU programming     = 0x%02x\n",              HdaPrivateConfig->SkipIGfxProgramming));
    DEBUG ((DEBUG_INFO, "  TC Assignment for USB     = 0x%02x\n",              HdaPrivateConfig->TrafficClassAssignmentUsb));
    DEBUG ((DEBUG_INFO, "  MicPrivacySupported       = 0x%02x\n",              HdaPrivateConfig->MicPrivacySupported));
  }
}
