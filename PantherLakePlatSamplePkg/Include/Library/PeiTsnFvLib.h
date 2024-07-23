/** @file
  Header file for PeiTsnFvFile

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

#ifndef _PEI_TSN_FV_LIB_H_
#define _PEI_TSN_FV_LIB_H_

typedef struct {
  UINT32  Type;
  UINT32  Version;
  UINT64  DataOffset;
  UINT32  DataSize;
  UINT64  SignatureOffset;
  UINT32  SignatureType;
} TSN_SUB_REGION_HEADER;

typedef struct {
  UINT32 Seg     : 8;
  UINT32 Bus     : 8;
  UINT32 Dev     : 5;
  UINT32 Func    : 3;
  UINT32 PcieCap : 8;
} SBDF;

typedef struct {
  SBDF    Bdf;
  UINT32  PortLength;
  UINT32  MacAddressHigh;
  UINT32  MacAddressLow;
} PCH_TSN_SUB_REGION_PORT;

typedef struct {
  PCH_TSN_SUB_REGION_PORT  Port;
  UINT32 Signature;
} TSN_SUB_REGION;

/**
  Install Base and Size Info Ppi for Siip Firmware Volume.

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
InstallTsnFv (
  VOID
  );

/**
  Install Base and Size Info Ppi for TSN Firmware Volume.

  @retval     EFI_SUCCESS  Always returns success.

**/
EFI_STATUS
EFIAPI
LoadFlashTsnFv (
  VOID
  );

/*
  Reads TSN Sub region.

  Reads TSN sub region data from FV. Returns pointer to data.

  @param[out]  SubRegion Pointer to sub region data.

  @retval     EFI_SUCCESS  Function has completed successfully.
  @retval     Others       TSN region was not found
*/
EFI_STATUS
GetTsnSubRegion (
  OUT TSN_SUB_REGION** SubRegion
  );

#endif
