/** @file
Defines Platform BoardIds

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#ifndef _PLATFORM_BOARD_ID_H_
#define _PLATFORM_BOARD_ID_H_

#define FlavorUnknown                       0x0
#define FlavorMobile                        0x1
#define FlavorDesktop                       0x2
#define FlavorWorkstation                   0x3
#define FlavorUpServer                      0x4
#define FlavorEmbedded                      0x5
#define FlavorMobileWorkstation             0x6
#define FlavorPlatformMax                   0x7

#define TypeUnknown                         0x0
#define TypeTrad                            0x1
#define TypeUltUlx                          0x2

// PantherLake USB Type-C PD types
#define UsbCPdTypeUnknown                   0x0
#define UsbCPdTypeCypress                   0x1
#define UsbCPdTypeTi                        0x2


//
// BoardId Reserved for PCD Default SkuId usage
// SkuId = PcdDisplayId << 24 | PcdBoardBomId << 16 | PcdBoardRev << 8 | PcdBoardId
//
// PantherLake Simics Sku Id
#define SkuIdPtlSimics                        0x00000031
#define SkuIdPtlHfpgaBase                     0x00010031
#define SkuIdPtlHfpgaTcss                     0x00020031
#define SkuIdPtlHsle                          0x00040031
#define SkuIdPtlHsleNull                      0x00050031

// LunarLake M Sku Id
#define SkuIdLnlMLp5Rvp1                      0x00000001
#define SkuIdLnlMLp5PpvRvp1                   0x00010001
#define SkuIdLnlMLp5ChromeRvp1                0x00100001
#define SkuIdLnlMLp5PnpRvp1                   0x00110001

//
// LunarLake Sku Type
//
#define LnlMSkuType 2


//
// PantherLake Sku Type
//
#define PtlSimicsSkuType 1
#define PtlUHSkuType 2


//
// PantherLake Board Id 0x00 - 0x3F
//
#define BoardIdReserved                   0x00


// PantherLake Simics Board Id
#define BoardIdPtlSimics                  0x31


// PantherLake UH Board Id
#define BoardIdPtlUHLp5Rvp1                0x01
  #define BomIdPtlUHLp5Rvp                 0x06
  #define BomIdPtlUHLp5MemSdRvp            0x01
  #define BomIdPtlUHLp5MeccRvp             0x07
  #define BomIdPtlUHLp5EpsRvp              0x03
  #define BomIdPtlUHLp5PnpRvp              0x02
  #define BomIdPtlUHLp5MemSdChromeRvp      0x05
#define BoardIdPtlUHHmpRvp                 0x09
#define BoardIdPtlUHLp5Rvp3                0x03
#define BoardIdPtlUHDdr5Rvp4               0x04
  #define BomIdPtlUHLp5PpvRvp              0x05
#define BoardIdPtlUHCammDTbTRvp2           0x02
#define BoardIdPtlUHLp5Aep                 0x07
  #define BomIdPtlUHLp5Aep                 0x01

#define BoardIdPtlHLp5Gcs1                 0x11
#define BoardIdPtlHLp5Gcs2                 0x12

#define BoardIdUnknown1                   0xffff

// PantherLake UH Sku Id
#define SkuIdPtlUHHmpRvp                  0x00000009

#define SkuIdPtlUHLp5Rvp1                 0x00000001
#define SkuIdPtlUHLp5MemSdRvp1            0x00010001
#define SkuIdPtlUHLp5MemSktPnpRvp1        0x00020001
#define SkuIdPtlUHLp5MemSdEpsRvp1         0x00030001
#define SkuIdPtlUHLp5MemSktChromeRvp1     0x00040001
#define SkuIdPtlUHLp5MemSdChromeRvp1      0x00050001
#define SkuIdPtlUHLp5Rvp1Bom              0x00060001
#define SkuIdPtlUHLp5MemSdMeccRvp1        0x00070001

#define SkuIdPtlUHCammDTbTRvp2            0x00000002
#define SkuIdPtlUHCammDTbTExtGfxRvp2      0x00010002
#define SkuIdPtlUHCammDTbTRvp2Bom         0x00060002

#define SkuIdPtlUHLp5Rvp3                 0x00000003
#define SkuIdPtlUHLp5MemSdRvp3            0x00010003
#define SkuIdPtlUHLp5MemSktPnpRvp3        0x00020003
#define SkuIdPtlUHLp5Rvp3PpvBom           0x00050003
#define SkuIdPtlUHLp5Rvp3Bom              0x00060003

#define SkuIdPtlUHDdr5Rvp4                0x00000004
#define SkuIdPtlUHDdr5Rvp4Chrome          0x00040004
#define SkuIdPtlUHDdr5Rvp4PpvBom          0x00050004
#define SkuIdPtlUHDdr5Rvp4Bom             0x00060004
#define SkuIdPtlUHDdr5Rvp4Dp              0x01000004
#define SkuIdPtlUHDdr5Rvp4DpChrome        0x01040004
#define SkuIdPtlUHDdr5Rvp4DpPpvBom        0x01050004
#define SkuIdPtlUHDdr5Rvp4DpBom           0x01060004

#define SkuIdPtlHLp5Gcs1                  0x00000011
#define SkuIdPtlHLp5Gcs2                  0x00000012

#define SkuIdPtlUHLp5Aep                  0x00000007
#define SkuIdPtlUHLp5AepBom               0x00010007

#endif // _PLATFORM_BOARD_ID_H_
