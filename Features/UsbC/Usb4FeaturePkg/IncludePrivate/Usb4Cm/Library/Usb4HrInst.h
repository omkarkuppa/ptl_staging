/** @file
  Header file for USB4 host router instance.

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

#ifndef _USB4_HR_INST_H_
#define _USB4_HR_INST_H_

#include <Protocol/PciIo.h>
#include <Library/Usb4PlatformInfo.h>
#include <Library/Usb4HiCoreDefs.h>
#include <Library/Usb4HcMem.h>
#include <Library/Usb4DomainDefs.h>

#define USB4_HR_INSTANCE_SIGNATURE      SIGNATURE_32 ('u', '4', 'h', 'r')
typedef struct _USB4_HR_INSTANCE {
  UINT32                 Signature;
  USBHC_MEM_POOL         *MemPool;
  // USB4 host router information
  USB4_HR_INFO           HrInfo;
  // PCI I/O protocol should be used for MMIO and DMA buffer allocation if it's present.
  EFI_PCI_IO_PROTOCOL    *PciIo;
  // MMIO base of USB4 host router for Tx/Rx Ring
  PHYSICAL_ADDRESS       MmioBase;
  // MMIO length of USB4 host router for Tx/Rx Ring, not used in post pci
  UINT32                 MmioLen;
  // Indicate whether PEI reserved DMA buffer is in use, not used in post pci
  BOOLEAN                PeiDmaBufInUse;
  // USB4 Host Interface core
  USB4_HI_CORE           HiCore;
  // USB4 Domain
  USB4_DOMAIN            *Usb4Domain;
  BOOLEAN                PciInitDone;
  BOOLEAN                CmStart;
  EFI_EVENT              PollTimer;
} USB4_HR_INSTANCE, *PUSB4_HR_INSTANCE;

#define USB4_HR_FROM_HC_PROTOCOL(a) \
  CR (a, USB4_HR_INSTANCE, Usb4Hr, USB4_HR_INSTANCE_SIGNATURE)

#endif
