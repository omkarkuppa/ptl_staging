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

/**
  Save IPU BAR for S3

**/
VOID
SaveIpuBarForS3 (
  VOID
  )
{
  return;
}

/**
  Update IPU NVS AREA tables
**/
VOID
UpdateIpuNvs (
  VOID
  )
{

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
  return 0;
}

/**
  Install Ipu NVS protocol
**/
VOID
InstallIpuNvsProtocol (
  IN EFI_HANDLE         ImageHandle
  )
{

}