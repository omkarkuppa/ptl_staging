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
  #define BomIdPtlUHLp5Aep32Gb             0x02
  #define BomIdPtlUHLp5Aep32Gb9600         0x03
#define BoardIdPtlUHLp5MemSktmRvp          0x05
#define BoardIdPtlHLp5Gcs1                 0x11
#define BoardIdPtlHLp5Gcs2                 0x12
#define BoardIdPtlUHLp5Adk1                0x15
#define BoardIdPtlUHLp5Adk2                0x16
#define BoardIdPtlUHCammBep                0x08
#define BoardIdPtlUHLp5DerCore             0x0B

// WildcatLake Board Id
#define BoardIdWclNDdr5Rvp1                0x20
  #define BomIdWclDdr5PnpRvp1              0x02
  #define BomIdWclDdr5Rvp1                 0x04
  #define BomIdWclDdr5McioRvp1             0x07
#define BoardIdWclLp5xRvp2                 0x21
  #define BomIdWclLp5SktRvp2               0x01
  #define BomIdWclLp5PpvRvp2               0x05
  #define BomIdWclLp5PnpRvp2               0x04
  #define BomIdWclLp5MeccRvp2              0x03
  #define BomIdWclLp5HsioMcioRvp2          0x07
#define BoardIdWclLp5xBep                  0x1C

#define BoardIdPtlUHDdr5EmbCrb             0x32
  #define BomIdPtlUHDdr5EmbCrb             0x01

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
#define SkuIdPtlUHLp5Rvp1Fab1             0x00000101
#define SkuIdPtlUHLp5MemSdRvp1Fab1        0x00010101
#define SkuIdPtlUHLp5MemSktPnpRvp1Fab1    0x00020101
#define SkuIdPtlUHLp5MemSdEpsRvp1Fab1     0x00030101
#define SkuIdPtlUHLp5MemSktChromeRvp1Fab1 0x00040101
#define SkuIdPtlUHLp5MemSdChromeRvp1Fab1  0x00050101
#define SkuIdPtlUHLp5Rvp1BomFab1          0x00060101
#define SkuIdPtlUHLp5MemSdMeccRvp1Fab1    0x00070101

#define SkuIdPtlUHCammDTbTRvp2            0x00000002
#define SkuIdPtlUHCammDTbTExtGfxRvp2      0x00010002
#define SkuIdPtlUHCammDTbTRvp2Bom         0x00060002
#define SkuIdPtlUHCammDTbTRvp2Fab1        0x00000102
#define SkuIdPtlUHCammDTbTExtGfxRvp2Fab1  0x00010102
#define SkuIdPtlUHCammDTbTRvp2BomFab1     0x00060102
#define SkuIdPtlUHCammDTbTRvp2Fab2        0x00000202
#define SkuIdPtlUHCammDTbTExtGfxRvp2Fab2  0x00010202
#define SkuIdPtlUHCammDTbTRvp2BomFab2     0x00060202

#define SkuIdPtlUHLp5Rvp3                 0x00000003
#define SkuIdPtlUHLp5MemSdRvp3            0x00010003
#define SkuIdPtlUHLp5MemSktPnpRvp3        0x00020003
#define SkuIdPtlUHLp5Rvp3PpvBom           0x00050003
#define SkuIdPtlUHLp5Rvp3Bom              0x00060003
#define SkuIdPtlUHLp5Rvp3Fab1             0x00000103
#define SkuIdPtlUHLp5MemSdRvp3Fab1        0x00010103
#define SkuIdPtlUHLp5MemSktPnpRvp3Fab1    0x00020103
#define SkuIdPtlUHLp5Rvp3PpvBomFab1       0x00050103
#define SkuIdPtlUHLp5Rvp3BomFab1          0x00060103
#define SkuIdPtlUHLp5Rvp3Fab2             0x00000203
#define SkuIdPtlUHLp5MemSdRvp3Fab2        0x00010203
#define SkuIdPtlUHLp5MemSktPnpRvp3Fab2    0x00020203
#define SkuIdPtlUHLp5Rvp3PpvBomFab2       0x00050203
#define SkuIdPtlUHLp5Rvp3BomFab2          0x00060203

#define SkuIdPtlUHDdr5Rvp4                0x00000004
#define SkuIdPtlUHDdr5Rvp4Chrome          0x00040004
#define SkuIdPtlUHDdr5Rvp4PpvBom          0x00050004
#define SkuIdPtlUHDdr5Rvp4Bom             0x00060004
#define SkuIdPtlUHDdr5Rvp4Dp              0x01000004
#define SkuIdPtlUHDdr5Rvp4DpChrome        0x01040004
#define SkuIdPtlUHDdr5Rvp4DpPpvBom        0x01050004
#define SkuIdPtlUHDdr5Rvp4DpBom           0x01060004
#define SkuIdPtlUHDdr5Rvp4Fab1            0x00000104
#define SkuIdPtlUHDdr5Rvp4ChromeFab1      0x00040104
#define SkuIdPtlUHDdr5Rvp4PpvBomFab1      0x00050104
#define SkuIdPtlUHDdr5Rvp4BomFab1         0x00060104
#define SkuIdPtlUHDdr5Rvp4DpFab1          0x01000104
#define SkuIdPtlUHDdr5Rvp4DpChromeFab1    0x01040104
#define SkuIdPtlUHDdr5Rvp4DpPpvBomFab1    0x01050104
#define SkuIdPtlUHDdr5Rvp4DpBomFab1       0x01060104

#define SkuIdPtlUHLp5MemSktmRvp           0x00000005
#define SkuIdPtlHLp5Gcs1                  0x00000011
#define SkuIdPtlHLp5Gcs2                  0x00000012
#define SkuIdPtlUHLp5Adk1                 0x00000015
#define SkuIdPtlUHLp5Adk2                 0x00000016

#define SkuIdPtlUHLp5Aep                  0x00000007
#define SkuIdPtlUHLp5AepBom               0x00010007
#define SkuIdPtlUHLp5AepBom32Gb           0x00020007
#define SkuIdPtlUHLp5AepBom32Gb9600       0x00030007
#define SkuIdPtlUHCammBep                 0x00000008
#define SkuIdPtlUHLp5DerCore              0x0000000B

#define SkuIdPtlUHDdr5EmbCrb              0x01000032
#define SkuIdPtlUHDdr5EmbCrbBom           0x01010032
#define SkuIdPtlUHDdr5EmbCrbFabB          0x01000132
#define SkuIdPtlUHDdr5EmbCrbFabBBom       0x01010132

// WildcatLake Sku Id
#define SkuIdWclNDdr5Rvp1                 0x08000020
#define SkuIdWclNDdr5Rvp1HDMI             0x0A000020
#define SkuIdWclDdr5Rvp1Bom               0x08040020
#define SkuIdWclNDdr5Rvp1HDMIBom          0x0A040020
#define SkuIdWclLp5Rvp2                   0x08000021
#define SkuIdWclDdr5PnpRvp1               0x08020020
#define SkuIdWclDdr5PnpRvp1HDMI           0x0A020020
#define SkuIdWclDdr5McioRvp1              0x08070020
#define SkuIdWclDdr5McioRvp1HDMI          0x0A070020
#define SkuIdWclLp5SktRvp2                0x08010021
#define SkuIdWclLp5PpvRvp2                0x08050021
#define SkuIdWclLp5PnpRvp2                0x08040021
#define SkuIdWclLp5MeccRvp2               0x08030021
#define SkuIdWclLp5HsioMcioRvp2           0x08070021
#define SkuIdWclLp5Bep                    0x0800001C

#endif // _PLATFORM_BOARD_ID_H_
