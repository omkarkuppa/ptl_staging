/** @file
  Discrete USB4 host controller initialization.

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
#include <Ppi/PeiDTbtPolicy.h>
#include <IndustryStandard/Pci22.h>
#include <DTbtDefinition.h>
#include <Library/DTbtCommonLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PeiServicesLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PcdLib.h>
#include <Library/TimerLib.h>
#include <Library/DebugLib.h>
#include <Library/Usb4PlatformInfo.h>
#include <Library/Usb4CmMode.h>

/**
  Query the discrete USB4 host router information in the platform.

  @param[out] Usb4PlatformInfo - Pointer to USB4 host routers information in the platform.

  @retval EFI_SUCCESS           - Query the discrete USB4 host router information successfully.
  @retval EFI_NOT_FOUND         - No discrete USB4 host router information found in platform.
  @retval EFI_UNSUPPORTED       - Fail to query the discrete USB4 host router information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Dusb4PlatformInfoQuery (
  OUT USB4_PLATFORM_INFO    *Usb4PlatformInfo
  )
{
  EFI_STATUS         Status;
  PEI_DTBT_POLICY    *PeiDTbtConfig;
  PUSB4_HR_INFO      Usb4HrInfo;
  UINT8              HrIndex;
  UINT8              Index;
  UINT8              RpBus;
  UINT8              RpDev;
  UINT8              RpFunc;
  UINT8              TbtTempBus;
  UINT16             MailboxCmdReg;
  UINT16             MailboxStatusReg;
  UINT32             RpOrgBusConfig;
  UINT32             UsVidPid;
  UINT64             RpAddr;
  UINT64             Epbase;

  DEBUG ((DEBUG_INFO, "Dusb4PlatformInfoQuery entry\n"));

  if (Usb4PlatformInfo == NULL) {
    DEBUG ((DEBUG_ERROR, "Dusb4PlatformInfoQuery: Null Usb4PlatformInfo!\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  PeiDTbtConfig = NULL;
  Status = PeiServicesLocatePpi (
             &gPeiDTbtPolicyPpiGuid,
             0,
             NULL,
             (VOID **) &PeiDTbtConfig
             );
  if (PeiDTbtConfig == NULL) {
    DEBUG ((DEBUG_INFO, "No discrete USB4 host router found in platform!\n"));
    Status = EFI_NOT_FOUND;
    goto Exit;
  }

  //
  // Usb4CmMode - Bit 2:0 = CM mode, Bit 3 = CM switch unsupported
  //
  Usb4PlatformInfo->CmMode = (PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode & 0x07);

  if (PeiDTbtConfig->DTbtGenericConfig.Usb4CmMode & USB4_CM_MODE_SWITCH_UNSUPPORTED &&
      (PeiDTbtConfig->DTbtControllerConfig[0].DTbtControllerEn == 1 ||
      PeiDTbtConfig->DTbtControllerConfig[1].DTbtControllerEn == 1)) {
    DEBUG ((DEBUG_INFO, "Dusb4PlatformInfoQuery: USB4 CM mode switch is not supported in platform!\n"));
    Usb4PlatformInfo->CmSwitchSupport = FALSE;
  } else {
    Usb4PlatformInfo->CmSwitchSupport = TRUE;
  }

  //
  // Retrieve dTBT control information from PCD
  //
  TbtTempBus       = FixedPcdGet8 (PcdDTbtTempBusNumber);
  MailboxCmdReg    = FixedPcdGet16 (PcdPcieToDTbtRegister);
  MailboxStatusReg = FixedPcdGet16 (PcdDTbtToPcieRegister);

  //
  // Search discrete USB4 host router information in platform
  //
  HrIndex = Usb4PlatformInfo->Usb4HrCount;
  for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++)
  {
    if (PeiDTbtConfig->DTbtControllerConfig[Index].DTbtControllerEn != 0) {
      DEBUG ((DEBUG_INFO, "Discrete USB4 host router %d : RpType = %d, PCIe RP number = %d\n", Index, PeiDTbtConfig->DTbtControllerConfig[Index].RpType, PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber));
      RpBus = PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpBus;
      Status = GetDTbtRpDevFun (
            PeiDTbtConfig->DTbtControllerConfig[Index].PcieRpNumber,
            RpBus,
            &RpDev,
            &RpFunc);
      if (Status != EFI_SUCCESS) {
        DEBUG ((DEBUG_ERROR, "Dusb4PlatformInfoQuery: Fail to get RP BDF, %r\n", Status));
        continue;
      }

      DEBUG ((DEBUG_INFO, "Discrete USB4 host router %d : Dev = %d, Func = %d\n", Index, RpDev, RpFunc));

      if (HrIndex >= USB4_HR_SUPPORT_MAX) {
        DEBUG ((DEBUG_ERROR, "Dusb4PlatformInfoQuery: No more USB4 host router can be supported!\n"));
        Status = EFI_UNSUPPORTED;
        goto Exit;
      }

      //
      // Fill in discrete USB4 host router information.
      // Discrete USB4 host router information consists of RP, upstream port, downstream port and NHI device.
      // Upstream port is the target for mailbox command communication.
      // NHI device is the device providing USB4 host interface for USB4 config space access.
      //
      Usb4HrInfo = &(Usb4PlatformInfo->Usb4Hr[HrIndex]);

      Usb4HrInfo->IntegratedHr = FALSE;
      Usb4HrInfo->TbtIndex     = Index;
      if (Usb4PlatformInfo->CmVer2Support) {
        DEBUG ((DEBUG_INFO, "USB4 CM Ver2 Support is enabled - Check CM Ver supported by Host Router\n"));
        //
        // Query Device ID of discrete USB4 host router to determine SW CM version of host router.
        //
        RpAddr = PCI_SEGMENT_LIB_ADDRESS (0, RpBus, RpDev, RpFunc, 0);
        RpOrgBusConfig = PciSegmentRead32 (RpAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET);
        //
        // Set Sec/Sub buses to a temporary value for upstream port PCI access
        //
        PciSegmentWrite8 (RpAddr + PCI_BRIDGE_SECONDARY_BUS_REGISTER_OFFSET, TbtTempBus);
        PciSegmentWrite8 (RpAddr + PCI_BRIDGE_SUBORDINATE_BUS_REGISTER_OFFSET, TbtTempBus);
        //
        // Read upstream port device ID
        //
        // need to provide one dummy write before reading the Upstream port configuration space
        // A config write is required in order for the device to re-capture the Bus number,
        // according to PCI Express Base Specification, 2.2.6.2
        // Write to a read-only register VendorID to not cause any side effects.
        Epbase = PCI_SEGMENT_LIB_ADDRESS (0, TbtTempBus, 0, 0, 0);
        PciSegmentWrite16 (Epbase + PCI_VENDOR_ID_OFFSET, 0x8086);
        UsVidPid = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (0, TbtTempBus, 0, 0, 0));
        //
        // Reset Sec/Sub buses to original value
        //
        PciSegmentWrite32 (RpAddr + PCI_BRIDGE_PRIMARY_BUS_REGISTER_OFFSET, RpOrgBusConfig);
        //
        // Query CM version supported by USB4 host router
        //
        Usb4HrInfo->CmVer = DtbtQueryUsb4Ver (UsVidPid & 0x0000FFFF, UsVidPid>> 16);
      } else {
        //
        // USB4 CM Ver2 support is not enabled
        // Default USB4 CM Ver1 is applied
        //
        DEBUG ((DEBUG_INFO, "USB4 CM Ver2 Support is not enabled - CM Ver1 is applied.\n"));
        Usb4HrInfo->CmVer = USB4_CM_VERSION_1;
      }

      //
      // Supported USB4 control
      // bit 0 - USB Tunneling
      // bit 1 - DisplayPort Tunneling
      // bit 2 - PCIe Tunneling
      // bit 3 - Inter-Domain USB4 Internet Protocol
      // bit 31:4 - Reserved
      //
      Usb4HrInfo->Usb4Control.Value = DEFAULT_USB4_CONTROLS;
      if (Usb4PlatformInfo->PcieOverUsb4En == 0) {
        Usb4HrInfo->Usb4Control.Fields.Pcie = 0;
      }
      //
      // Target for mailbox command communication
      //
      Usb4HrInfo->MbStatusReg     = MailboxStatusReg;
      Usb4HrInfo->MbCmdReg        = MailboxCmdReg;
      Usb4HrInfo->MailboxBdf.Bus  = TbtTempBus;
      Usb4HrInfo->MailboxBdf.Dev  = 0;
      Usb4HrInfo->MailboxBdf.Func = 0;
      //
      // Root port information
      //
      Usb4HrInfo->Rp.Bdf.Bus  = RpBus;
      Usb4HrInfo->Rp.Bdf.Dev  = RpDev;
      Usb4HrInfo->Rp.Bdf.Func = RpFunc;
      Usb4HrInfo->Rp.PriBus   = 0;
      Usb4HrInfo->Rp.SecBus   = TbtTempBus;
      Usb4HrInfo->Rp.SubBus   = TbtTempBus + 2;
      //
      // Upstream port information
      //
      Usb4HrInfo->UsPort.Bdf.Bus  = TbtTempBus;
      Usb4HrInfo->UsPort.Bdf.Dev  = 0;
      Usb4HrInfo->UsPort.Bdf.Func = 0;
      Usb4HrInfo->UsPort.PriBus   = TbtTempBus;
      Usb4HrInfo->UsPort.SecBus   = TbtTempBus + 1;
      Usb4HrInfo->UsPort.SubBus   = TbtTempBus + 2;
      //
      // Downstream port information
      //
      Usb4HrInfo->DsPort.Bdf.Bus  = TbtTempBus + 1;
      Usb4HrInfo->DsPort.Bdf.Dev  = 0;
      Usb4HrInfo->DsPort.Bdf.Func = 0;
      Usb4HrInfo->DsPort.PriBus   = TbtTempBus + 1;
      Usb4HrInfo->DsPort.SecBus   = TbtTempBus + 2;
      Usb4HrInfo->DsPort.SubBus   = TbtTempBus + 2;
      //
      // Native Host Interface device (DMA controller) information
      //
      Usb4HrInfo->NhiBdf.Bus  = TbtTempBus + 2;
      Usb4HrInfo->NhiBdf.Dev  = 0;
      Usb4HrInfo->NhiBdf.Func = 0;

      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] Root port BDF = (%d,%d,%d)\n",
              Index,
              Usb4HrInfo->Rp.Bdf.Bus,
              Usb4HrInfo->Rp.Bdf.Dev,
              Usb4HrInfo->Rp.Bdf.Func
            ));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] Upstream port BDF = (%d,%d,%d)\n",
              Index,
              Usb4HrInfo->UsPort.Bdf.Bus,
              Usb4HrInfo->UsPort.Bdf.Dev,
              Usb4HrInfo->UsPort.Bdf.Func
            ));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] Downstream port BDF = (%d,%d,%d)\n",
              Index,
              Usb4HrInfo->DsPort.Bdf.Bus,
              Usb4HrInfo->DsPort.Bdf.Dev,
              Usb4HrInfo->DsPort.Bdf.Func
            ));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] NHI BDF = (%d,%d,%d)\n",
              Index,
              Usb4HrInfo->NhiBdf.Bus,
              Usb4HrInfo->NhiBdf.Dev,
              Usb4HrInfo->NhiBdf.Func
            ));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] mailbox command register offset = 0x%X\n", Index, Usb4HrInfo->MbCmdReg));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] mailbox status register offset = 0x%X\n", Index, Usb4HrInfo->MbStatusReg));
      DEBUG ((DEBUG_INFO, "Discrete USB4 HR[%d] USB4 control = 0x%0X\n", Index, Usb4HrInfo->Usb4Control));

      //
      // Enabled USB4 host router mask
      // bit 0 - 3 : Integrated USB4 host router 0 - 3
      // bit 4 - 7 : Discrete USB4 host router 0 - 3
      //
      Usb4PlatformInfo->Usb4HrMask |= (1 << (Index + 4));
      Usb4PlatformInfo->Usb4HrCount++;
      HrIndex++;
      TbtTempBus += 3;
    }
  } // for (Index = 0; Index < PcdGet8 (PcdBoardDTbtControllerNumber); Index++)

  Status = EFI_SUCCESS;

Exit:
  DEBUG ((DEBUG_INFO, "Dusb4PlatformInfoQuery exit\n"));
  return Status;
}