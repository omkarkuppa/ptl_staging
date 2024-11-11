/** @file
  USB4 Platform Info and DTBT Hob Dump function library.

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

#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Library/PrintLib.h>
#include <DTbtDefinition.h>
#include <DTbtInfoHob.h>
#include <Library/Usb4PlatformHob.h>
#include <Library/HobLib.h>

/**
  Print DTBT DXE HOB information

  @retval EFI_SUCCESS           Success.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
DumpDtbtInfoHob (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT8                     Index;
  DTBT_INFO_HOB             *DTbtInfoHob;


  Status      = EFI_SUCCESS;
  DTbtInfoHob = NULL;

  //
  // Get DTBT INFO HOB
  //
  DTbtInfoHob = (DTBT_INFO_HOB *) GetFirstGuidHob (&gDTbtInfoHobGuid);
  if (DTbtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "gDTbtInfoHobGuid not found\n"));
    return EFI_NOT_FOUND;
  }

  DEBUG ((DEBUG_INFO, "USB4 DTBT HOB information :\n"));
  DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtGenericConfig.Usb4CmMode                = 0x%x\n", DTbtInfoHob->DTbtGenericConfig.Usb4CmMode));
  DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtGenericConfig.Usb4ClassOption           = 0x%x\n", DTbtInfoHob->DTbtGenericConfig.Usb4ClassOption));

  for (Index = 0; Index < MAX_DTBT_CONTROLLER_NUMBER; Index++) {
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].DTbtControllerEn   = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].DTbtControllerEn));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].RpType             = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].RpType));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpNumber       = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpNumber));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].RpAcpiNumber       = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].RpAcpiNumber));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpBus          = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpBus));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpDev          = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpDev));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpFunc         = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpFunc));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].BdfValid           = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].BdfValid));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].WakeGpioPin        = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].WakeGpioPin));
    DEBUG ((DEBUG_INFO, "DTbtInfoHob->DTbtControllerConfig[%d].PcieRpMps          = 0x%x\n", Index, DTbtInfoHob->DTbtControllerConfig[Index].PcieRpMps));
  }

  return Status;
}

/**
  Print USB4 Platform HOB information

  @retval EFI_SUCCESS           Success.
  @retval EFI_NOT_FOUND         The required object information is not found.
**/
EFI_STATUS
EFIAPI
DumpUsb4PlatformHob (
  VOID
  )
{
  EFI_STATUS                Status;
  UINT8                     Index;
  UINT32                    HrCount;
  VOID                      *HobPtr;
  USB4_PLATFORM_HOB         *Usb4PlatformHob;
  USB4_PLATFORM_INFO        *Usb4PlatformInfo;

  Status      = EFI_SUCCESS;
  HobPtr      = NULL;

  //
  // Find USB4 Hob to get platform USB4 host router information
  //
  HobPtr = GetFirstGuidHob (&gUsb4PlatformHobGuid);
  if (HobPtr == NULL) {
    DEBUG ((DEBUG_ERROR, "gUsb4PlatformHobGuid not found\n"));
    return EFI_NOT_FOUND;
  }

  Usb4PlatformHob = GET_GUID_HOB_DATA (HobPtr);
  Usb4PlatformInfo = &(Usb4PlatformHob->Usb4PlatformInfo);

  DEBUG ((DEBUG_INFO, "USB4 Platform HOB information :\n"));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4HrCount              = 0x%x\n", Usb4PlatformInfo->Usb4HrCount));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4HrMask               = 0x%x\n", Usb4PlatformInfo->Usb4HrMask));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->CmMode                   = 0x%x\n", Usb4PlatformInfo->CmMode));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->CmModeOption             = 0x%x\n", Usb4PlatformInfo->CmModeOption));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->CmSwitchSupport          = 0x%x\n", Usb4PlatformInfo->CmSwitchSupport));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->PcieOverUsb4En           = 0x%x\n", Usb4PlatformInfo->PcieOverUsb4En));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4DebugLevel           = 0x%x\n", Usb4PlatformInfo->Usb4DebugLevel));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->CmVer2Support            = 0x%x\n", Usb4PlatformInfo->CmVer2Support));
  DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4MaxTopologyDepth     = 0x%x\n", Usb4PlatformInfo->Usb4MaxTopologyDepth));

  //
  // Check if the USB4 HR count exceeds the maximum HR count.
  //
  HrCount = Usb4PlatformInfo->Usb4HrCount;
  if (HrCount > USB4_HR_SUPPORT_MAX) {
    DEBUG ((DEBUG_ERROR, "USB4 HR count = %d, only %d HRs can be supported\n", HrCount, USB4_HR_SUPPORT_MAX));
    HrCount = USB4_HR_SUPPORT_MAX;
  }

  for (Index = 0; Index < HrCount; Index++) {
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].IntegratedHr  = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].IntegratedHr));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].TbtIndex      = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].TbtIndex));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].CmVer         = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].CmVer));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].Usb4Control   = 0x%x (USB3/DP/PCIe/InterDomain)\n", Index, Usb4PlatformInfo->Usb4Hr[Index].Usb4Control));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].MbStatusReg   = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].MbStatusReg));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].MbCmdReg      = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].MbCmdReg));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].MailboxBdf    = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].MailboxBdf));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].NhiBdf        = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].NhiBdf));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].Rp            = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].Rp));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].UsPort        = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].UsPort));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].DsPort        = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].DsPort));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].PeiDmaBufBase = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].PeiDmaBufBase));
    DEBUG ((DEBUG_INFO, "Usb4PlatformInfo->Usb4Hr[%d].PeiDmaBufSize = 0x%x\n", Index, Usb4PlatformInfo->Usb4Hr[Index].PeiDmaBufSize));
  }

  return Status;
}