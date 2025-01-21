/** @file
  The DXE IPU Init Library Implements

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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
#include <IpuConfigHob.h>

#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/HobLib.h>
#include <Library/S3BootScriptLib.h>
#include <Library/PcdLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/IpuLib.h>
#include <Library/DxeIpuInit.h>

#include <Ppi/SiPolicy.h>
#include <Defines/IpuDefines.h>
#include <Protocol/IpuNvsArea.h>
#include <Library/AslUpdateLib.h>
#include <Library/BaseMemoryLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED IPU_NVS_AREA_PROTOCOL         mIpuNvsAreaProtocol;

/**
  Update IPU NVS AREA tables
**/
VOID
UpdateIpuNvs (
  VOID
  )
{
  IPU_NVS_AREA_PROTOCOL            *IpuNvsAreaProtocol;
  IPU_CONFIG_HOB                   *IpuConfigHob;
  EFI_STATUS                       Status;

  ///
  /// Update IPU Device Enable status in Nvs from HOB values
  ///
  Status = gBS->LocateProtocol (&gIpuNvsAreaProtocolGuid, NULL, (VOID **) &IpuNvsAreaProtocol);
  if (Status != EFI_SUCCESS) {
    return;
  }

  IpuConfigHob = NULL;
  IpuConfigHob = (IPU_CONFIG_HOB *) GetFirstGuidHob (&gIpuConfigHobGuid);
  if (IpuConfigHob != NULL) {
    DEBUG ((DEBUG_INFO, "[IPU] Update IPU NVS Area.\n"));
    IpuNvsAreaProtocol->Area->IpuAcpiMode = (UINT8) IpuConfigHob->IpuAcpiMode;
  }
}

/**
  Initialize Ipu Nvs Area operation region.
  @retval EFI_SUCCESS    initialized successfully
  @retval EFI_NOT_FOUND  Nvs Area operation region is not found
**/
EFI_STATUS
PatchIpuNvsAreaAddress (
  VOID
  )
{
  EFI_STATUS                            Status;
  UINT32                                Address;
  UINT16                                Length;
  IPU_NVS_AREA_PROTOCOL                 *IpuNvsAreaProtocol;

  Status = gBS->LocateProtocol (&gIpuNvsAreaProtocolGuid, NULL, (VOID **) &IpuNvsAreaProtocol);
  ASSERT_EFI_ERROR (Status);

  Address = (UINT32) (UINTN) IpuNvsAreaProtocol->Area;
  Length  = (UINT16) sizeof (IPU_NVS_AREA);
  DEBUG ((DEBUG_INFO, "Patch IPU NvsAreaAddress: IPU NVS Address %x Length %x\n", Address, Length));

  Status  = UpdateNameAslCode (SIGNATURE_32 ('I','P','N','B'), &Address, sizeof (Address));
  ASSERT_EFI_ERROR (Status);
  Status  = UpdateNameAslCode (SIGNATURE_32 ('I','P','N','L'), &Length, sizeof (Length));
  ASSERT_EFI_ERROR (Status);

  return Status;
}

/**
  Install Ipu NVS protocol
**/
VOID
InstallIpuNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{
  EFI_STATUS                Status;

  DEBUG ((DEBUG_INFO, "Install Ipu NVS protocol\n"));
  Status = (gBS->AllocatePool) (EfiACPIMemoryNVS, sizeof (IPU_NVS_AREA), (VOID **) &mIpuNvsAreaProtocol.Area);
  ASSERT_EFI_ERROR (Status);

  ZeroMem ((VOID *) mIpuNvsAreaProtocol.Area, sizeof (IPU_NVS_AREA));
  Status = gBS->InstallMultipleProtocolInterfaces (
                  &ImageHandle,
                  &gIpuNvsAreaProtocolGuid,
                  &mIpuNvsAreaProtocol,
                  NULL
                  );
  ASSERT_EFI_ERROR (Status);
}


/**
  Save IPU BAR for S3

**/
VOID
SaveIpuBarForS3 (
  VOID
  )
{
  UINT32      Data32;

  Data32 = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetIpuBusNumber (), GetIpuDevNumber (), GetIpuFuncNumber (), R_IPU_LBAR));
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                            SA_SEG_NUM,
                                            GetIpuBusNumber (),
                                            GetIpuDevNumber (),
                                            GetIpuFuncNumber (),
                                            R_IPU_LBAR),
    1,
    &Data32
  );
  Data32 = PciSegmentRead32 (PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, GetIpuBusNumber (), GetIpuDevNumber (), GetIpuFuncNumber (), R_IPU_UBAR));
  S3BootScriptSaveMemWrite (
    S3BootScriptWidthUint32,
    PcdGet64 (PcdSiPciExpressBaseAddress) + PCI_SEGMENT_LIB_ADDRESS (
                                            SA_SEG_NUM,
                                            GetIpuBusNumber (),
                                            GetIpuDevNumber (),
                                            GetIpuFuncNumber (),
                                            R_IPU_UBAR),
    1,
    &Data32
  );
}


