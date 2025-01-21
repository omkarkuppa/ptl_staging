/** @file
  Integrated USB4 host router initialization.

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

@par Specification
**/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <ITbtInfoHob.h>
#include <TcssDataHob.h>
#include <Library/Usb4PlatformInfo.h>
#include "IUsb4Hr.h"

GLOBAL_REMOVE_IF_UNREFERENCED CONST PCI_DEV_BDF  mIusb4HrBdf[IUSB4_HR_COUNT] = {
  {IUSB4_HR_0_BUS, IUSB4_HR_0_DEV, IUSB4_HR_0_FUNC},
  {IUSB4_HR_1_BUS, IUSB4_HR_1_DEV, IUSB4_HR_1_FUNC}
};

/**
  Query the integrated USB4 host router information in the platform.

  @param[out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Query the integrated USB4 host router information successfully.
  @retval EFI_NOT_FOUND         - No integrated USB4 host router found in platform.
  @retval EFI_UNSUPPORTED       - Fail to query the integrated USB4 host router information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Iusb4PlatformInfoQuery (
  OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  )
{
  EFI_STATUS         Status;
  ITBT_INFO_HOB      *ITbtInfoHob;
  UINT32             Index;
  UINT32             HrIndex;
  UINT32             MbStatusReg;
  UINT32             MbCmdReg;
  PUSB4_HR_INFO      Usb4HrInfo;
  TCSS_DATA_HOB      *TcssHob;

  DEBUG ((DEBUG_INFO, "Iusb4PlatformInfoQuery entry\n"));

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Iusb4PlatformInfoQuery: Null Usb4PlatformInfo!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  //
  // Check if iTBT exists
  //
  ITbtInfoHob = NULL;
  ITbtInfoHob = GetFirstGuidHob (&gITbtInfoHobGuid);
  if (ITbtInfoHob == NULL) {
    DEBUG ((DEBUG_ERROR, "No integrated USB4 host rounter found!\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  TcssHob = NULL;
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
  if (TcssHob == NULL) {
    DEBUG ((DEBUG_ERROR, "Iusb4PlatformInfoQuery: Fail to get Tcss data hob\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }


  //
  // Save platform CM mode applied in platform policy.
  //
  Usb4PlatformInfo->CmMode = ITbtInfoHob->Usb4CmMode;
  Usb4PlatformInfo->CmSwitchSupport = TRUE;

  //
  // Query iTBT mailbox command offset from PCD.
  //
  MbStatusReg = FixedPcdGet8 (PcdITbtToPcieRegister);
  MbCmdReg    = FixedPcdGet8 (PcdPcieToITbtRegister);
  DEBUG ((DEBUG_INFO, "Integrated USB4 HR mailbox command register offset = 0x%X\n", MbCmdReg));
  DEBUG ((DEBUG_INFO, "Integrated USB4 HR mailbox status register offset = 0x%X\n", MbStatusReg));

  //
  // Fill in the enabled USB4 host router information
  //
  HrIndex = Usb4PlatformInfo->Usb4HrCount;
  for (Index = 0; (Index < IUSB4_HR_COUNT) && (HrIndex < USB4_HR_SUPPORT_MAX); Index++) {
    if (TcssHob->TcssData.ItbtDmaEn[Index] == 0) {
        DEBUG ((DEBUG_INFO, "Integrated USB4 host router %d is not enabled!\n", Index));
        continue;
    }

    Usb4HrInfo = &(Usb4PlatformInfo->Usb4Hr[HrIndex]);
    Usb4HrInfo->IntegratedHr = TRUE;
    Usb4HrInfo->TbtIndex     = (UINT8) Index;

    //
    // Native host interface device
    //
    Usb4HrInfo->NhiBdf = mIusb4HrBdf[Index];

    //
    // Device for receiving mailbox command
    //
    Usb4HrInfo->MailboxBdf  = mIusb4HrBdf[Index];
    Usb4HrInfo->MbCmdReg    = MbCmdReg;
    Usb4HrInfo->MbStatusReg = MbStatusReg;

    //
    // Supported USB4 control
    // bit 0 - USB Tunneling
    // bit 1 - DisplayPort Tunneling
    // bit 2 - PCIe Tunneling
    // bit 3 - Inter-Domain USB4 Internet Protocol
    // bit 31:4 - Reserved
    //
    Usb4HrInfo->Usb4Control.Value  = DEFAULT_USB4_CONTROLS;
    if (Usb4PlatformInfo->PcieOverUsb4En == 0) {
      Usb4HrInfo->Usb4Control.Fields.Pcie = 0;
    }

    DEBUG ((DEBUG_INFO, "Integrated USB4 HR %d BDF = (%d,%d,%d)\n",
            Index,
            Usb4HrInfo->NhiBdf.Bus,
            Usb4HrInfo->NhiBdf.Dev,
            Usb4HrInfo->NhiBdf.Func
          ));
    DEBUG ((DEBUG_INFO, "Integrated USB4 HR %d USB4 control mask = 0x%0x\n", Index, Usb4HrInfo->Usb4Control));

    //
    // iTBT only supports USB4 Version 1
    //
    Usb4HrInfo->CmVer = USB4_CM_VERSION_1;

    Usb4PlatformInfo->Usb4HrMask |= (1 << Index);
    Usb4PlatformInfo->Usb4HrCount++;
    HrIndex++;
  }

  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Iusb4PlatformInfoQuery exit\n"));
  return Status;
}

