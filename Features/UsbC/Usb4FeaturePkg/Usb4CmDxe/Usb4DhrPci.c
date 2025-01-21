/** @file
  Discrete USB4 host router PCI routines.

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

#include <Uefi.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/Usb4HrSrvLib.h>
#include "Usb4DhrPci.h"

/**
  Dump PCI registers of USB4 host router

  @param[in] HrInfo - USB4 host router information
**/
VOID
Dusb4HrPciDump (
  PUSB4_HR_INFO    HrInfo
  )
{
  UINT64         DevAddr;
  PCI_DEV_BDF    *Bdf;


  if (HrInfo != NULL) {
    DEBUG ((DEBUG_INFO, "Discrete USB4 Host Router PCI Information:\n"));
    DEBUG ((DEBUG_INFO, "Root Port:\n"));
    Bdf = &(HrInfo->Rp.Bdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bdf->Bus, Bdf->Dev, Bdf->Func, 0);
    HrSrvDumpPciDevRegs (DevAddr);

    DEBUG ((DEBUG_INFO, "Upstream Port:\n"));
    Bdf = &(HrInfo->UsPort.Bdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bdf->Bus, Bdf->Dev, Bdf->Func, 0);
    HrSrvDumpPciDevRegs (DevAddr);

    DEBUG ((DEBUG_INFO, "Downstream Port:\n"));
    Bdf = &(HrInfo->DsPort.Bdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bdf->Bus, Bdf->Dev, Bdf->Func, 0);
    HrSrvDumpPciDevRegs (DevAddr);

    DEBUG ((DEBUG_INFO, "NHI Device:\n"));
    Bdf = &(HrInfo->NhiBdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bdf->Bus, Bdf->Dev, Bdf->Func, 0);
    HrSrvDumpPciDevRegs (DevAddr);
  }
}

/**
  Dump discrete USB4 host router information.

  @param[in] Usb4HrInst   - Pointer to USB4 host router instance.
**/
VOID
Dusb4HrDump (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  PCI_DEV_BDF      *Bdf;
  PUSB4_HR_INFO    HrInfo;

  if (Usb4HrInst != NULL) {
    HrInfo = &(Usb4HrInst->HrInfo);
    DEBUG ((DEBUG_INFO, "Discrete USB4 Host Router Info:\n"));
    DEBUG ((DEBUG_INFO, "    MbCmdReg = 0x%x, MbStatusReg = 0x%x\n", HrInfo->MbCmdReg, HrInfo->MbStatusReg));
    Bdf = &(HrInfo->MailboxBdf);
    DEBUG ((DEBUG_INFO, "    Mailbox B/D/F = (%x, %x, %x)\n", Bdf->Bus,  Bdf->Dev,  Bdf->Func));
    Bdf = &(HrInfo->NhiBdf);
    DEBUG ((DEBUG_INFO, "    NHI B/D/F = (%x, %x, %x)\n", Bdf->Bus, Bdf->Dev, Bdf->Func));
    DEBUG ((DEBUG_INFO, "    Iommu DmaBuf base = 0x%0x, size = %d\n", HrInfo->PeiDmaBufBase, HrInfo->PeiDmaBufSize));
    DEBUG ((DEBUG_INFO, "    Rp B/D/F = (%x, %x, %x)\n, Pri %x Sec %x Sub %x MemBase 0x%x\n",
            HrInfo->Rp.Bdf.Bus, HrInfo->Rp.Bdf.Dev, HrInfo->Rp.Bdf.Func,
            HrInfo->Rp.PriBus, HrInfo->Rp.SecBus, HrInfo->Rp.SubBus, HrInfo->Rp.MemBase
          ));
    DEBUG ((DEBUG_INFO, "    Us B/D/F = (%x, %x, %x)\n, Pri %x Sec %x Sub %x MemBase 0x%x\n",
            HrInfo->UsPort.Bdf.Bus, HrInfo->UsPort.Bdf.Dev, HrInfo->UsPort.Bdf.Func,
            HrInfo->UsPort.PriBus, HrInfo->UsPort.SecBus, HrInfo->UsPort.SubBus, HrInfo->UsPort.MemBase
          ));
    DEBUG ((DEBUG_INFO, "    Ds B/D/F = (%x, %x, %x)\n, Pri %x Sec %x Sub %x MemBase 0x%x\n",
            HrInfo->DsPort.Bdf.Bus, HrInfo->DsPort.Bdf.Dev, HrInfo->DsPort.Bdf.Func,
            HrInfo->DsPort.PriBus, HrInfo->DsPort.SecBus, HrInfo->DsPort.SubBus, HrInfo->DsPort.MemBase
          ));
  }
}

/**
  Initialize PCI configuration space of discrete USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS            - PCI initialization success.
  @retval EFI_INVALID_PARAMETER  - Invalid parameter.
  @retval EFI_NOT_FOUND          - Device is not present.
  @retval EFI_UNSUPPORTED        - Can't initialize PCI configuration space of USB4 host router.
**/
EFI_STATUS
Dusb4HrPciInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  EFI_STATUS          Status;
  UINT64              DevAddr;
  UINT16              BaseLimit;
  PPCI_BRIDGE_INFO    Bridge;
  PPCI_DEV_BDF        NhiBdf;

  DEBUG ((DEBUG_INFO, "Dusb4HrPciInit entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciInit: Invalid parameter\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  BaseLimit = (UINT16) RShiftU64 (Usb4HrInst->MmioBase, 16);

  //
  // Init Host Router Root Port
  //
  Bridge = &(Usb4HrInst->HrInfo.Rp);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
  Status = HrSrvPciBridgeInit (DevAddr, BaseLimit, Bridge->PriBus, Bridge->SecBus, Bridge->SubBus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciInit: Fail to initialize Root Port, %r\n", Status));
    goto Exit;
  }

  //
  // Init Host Router US bridge
  //
  Bridge = &(Usb4HrInst->HrInfo.UsPort);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
  Status = HrSrvPciBridgeInit (DevAddr, BaseLimit, Bridge->PriBus, Bridge->SecBus, Bridge->SubBus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciInit: Fail to initialize Upstream Port, %r\n", Status));
    goto Exit;
  }

  //
  // Init Host Router DS bridge
  //
  Bridge = &(Usb4HrInst->HrInfo.DsPort);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
  Status = HrSrvPciBridgeInit (DevAddr, BaseLimit, Bridge->PriBus, Bridge->SecBus, Bridge->SubBus);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciInit: Fail to initialize Downstream Port, %r\n", Status));
    goto Exit;
  }

  //
  // Init NHI device
  //
  NhiBdf = &(Usb4HrInst->HrInfo.NhiBdf);
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, NhiBdf->Bus, NhiBdf->Dev, NhiBdf->Func, 0);
  if (Usb4HrInst->HrInfo.CmVer != USB4_CM_VERSION_2) {
    //
    // Check if the NHI device is USB4 host router.
    //
    Status = HrSrvPciDetectUsb4Hr (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_INFO, "Dusb4HrPciInit: USB4 host router is not present!\n"));
      Status = EFI_UNSUPPORTED;
      goto Exit;
    }
  }

  Status = HrSrvPciDevInit (DevAddr, Usb4HrInst->MmioBase);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciInit: Fail to initialize NHI, %r\n", Status));
    goto Exit;
  }

  Usb4HrInst->PciInitDone = TRUE;

  Dusb4HrDump (Usb4HrInst);
  Dusb4HrPciDump (&(Usb4HrInst->HrInfo));

Exit:
  DEBUG ((DEBUG_INFO, "Dusb4HrPciInit exit\n"));
  return Status;
}

/**
  Un-Initialize PCI configuration space of discrete USB4 host router.

  @param[in] Usb4HrInst - Pointer to USB4 host router instance.
**/
VOID
Dusb4HrPciUnInit (
  IN USB4_HR_INSTANCE    *Usb4HrInst
  )
{
  EFI_STATUS         Status;
  UINT64             DevAddr;
  PCI_BRIDGE_INFO    *Bridge;
  PCI_DEV_BDF        *NhiBdf;

  DEBUG ((DEBUG_INFO, "Dusb4HrPciUnInit entry\n"));

  if (Usb4HrInst == NULL) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciUnInit: Invalid parameter\n"));
    goto Exit;
  }

  if (Usb4HrInst->PciInitDone) {
    //
    // Un-Init NHI device
    //
    NhiBdf = &(Usb4HrInst->HrInfo.NhiBdf);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0,NhiBdf->Bus, NhiBdf->Dev, NhiBdf->Func, 0);
    Status = HrSrvPciDevUninit (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Dusb4HrPciUnInit: Fail to un-initialize NHI\n"));
    }

    //
    // Un-Init Host Router DS bridge
    //
    Bridge = &(Usb4HrInst->HrInfo.DsPort);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
    Status = HrSrvPciBridgeUninit (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Dusb4HrPciUnInit: Fail to un-initialize Downstream Port\n"));
    }

    //
    // Un-Init Host Router US bridge
    //
    Bridge = &(Usb4HrInst->HrInfo.UsPort);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
    Status = HrSrvPciBridgeUninit (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Dusb4HrPciUnInit: Fail to un-initialize Upstream Port\n"));
    }

    //
    // Init Host Router Root Port
    //
    Bridge = &(Usb4HrInst->HrInfo.Rp);
    DevAddr = PCI_SEGMENT_LIB_ADDRESS (0, Bridge->Bdf.Bus, Bridge->Bdf.Dev, Bridge->Bdf.Func, 0);
    Status = HrSrvPciBridgeUninit (DevAddr);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "Dusb4HrPciUnInit: Fail to un-initialize Root Port\n"));
    }
    Usb4HrInst->PciInitDone = FALSE;
  }
Exit:
  DEBUG ((DEBUG_INFO, "Dusb4HrPciUnInit exit\n"));
}


/**
  Query PCI information of discrete USB4 host router.
  This function should be called after PCI enumeration.

  @param[in, out] Usb4Hr - Pointer to USB4 host router instance.

  @retval EFI_SUCCESS           - Query PCI information successfully.
  @retval EFI_NOT_FOUND         - Discrete USB4 host router is not found.
  @retval EFI_UNSUPPORTED       - Fail to query PCI information.
  @retval EFI_INVALID_PARAMETER - Invalid parameter.
**/
EFI_STATUS
Dusb4HrPciQuery (
  IN OUT USB4_HR_INSTANCE    *Usb4Hr
  )
{
  EFI_STATUS        Status;
  PCI_DEV_BDF       *NhiBdf;
  UINT64            DevAddr;

  DEBUG ((DEBUG_INFO, "Dusb4HrPciQuery entry\n"));

  if (Usb4Hr == NULL) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Null Usb4Hr\n"));
    Status = EFI_INVALID_PARAMETER;
    goto Exit;
  }

  Status = HrSrvPciBridgeQuery (&(Usb4Hr->HrInfo.Rp));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Fail to query discrete USB4 HR RP, %r\n", Status));
    goto Exit;
  }

  //
  // Update upstream port bus number and query the new bridge info since the PCI resources have been re-assigned.
  //
  Usb4Hr->HrInfo.UsPort.Bdf.Bus = Usb4Hr->HrInfo.Rp.SecBus;
  Status = HrSrvPciBridgeQuery (&(Usb4Hr->HrInfo.UsPort));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Fail to query discrete USB4 HR upstream port, %r\n", Status));
    goto Exit;
  }

  //
  // Update downstream port bus number and query the new bridge info since the PCI resources have been re-assigned.
  //
  Usb4Hr->HrInfo.DsPort.Bdf.Bus = Usb4Hr->HrInfo.UsPort.SecBus;
  Status = HrSrvPciBridgeQuery (&(Usb4Hr->HrInfo.DsPort));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Fail to query discrete USB4 HR downstream port, %r\n", Status));
    goto Exit;
  }

  //
  // Update NHI device bus number since the PCI resources have been re-assigned.
  //
  NhiBdf = &(Usb4Hr->HrInfo.NhiBdf);
  NhiBdf->Bus = Usb4Hr->HrInfo.DsPort.SecBus;

  //
  // Query PCI I/O protocol of host router.
  //
  Status = HrSrvPciQueryIoProtocol (NhiBdf->Bus, NhiBdf->Dev, NhiBdf->Func, &(Usb4Hr->PciIo));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Query PCI I/O protocol failure, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

  //
  // Query MMIO space for the programming interface of Tx/Rx Ring access.
  //
  DevAddr = PCI_SEGMENT_LIB_ADDRESS (
              0,
              (UINT32)NhiBdf->Bus,
              (UINT32)NhiBdf->Dev,
              (UINT32)NhiBdf->Func,
              0
              );
  Status = HrSrvPciQueryMmio (DevAddr, &(Usb4Hr->MmioBase));
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Dusb4HrPciQuery: Query MMIO failure, %r\n", Status));
    Status = EFI_UNSUPPORTED;
    goto Exit;
  }

Exit:
  DEBUG ((DEBUG_INFO, "Dusb4HrPciQuery exit\n"));
  return Status;
}