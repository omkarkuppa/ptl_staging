/** @file
  Configurations for HostBridge

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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
#ifndef _HOST_BRIDGE_CONFIG_SILICON_H_
#define _HOST_BRIDGE_CONFIG_SILICON_H_

#include <ConfigBlock.h>

#define HOST_BRIDGE_PREMEM_CONFIG_REVISION 5
#define HOST_BRIDGE_PEI_CONFIG_REVISION    1

extern EFI_GUID gHostBridgePeiPreMemConfigGuid;
extern EFI_GUID gHostBridgePeiConfigGuid;

#pragma pack (push,1)

/**
  This configuration block describes HostBridge settings in PreMem.\n
  <b>Revision 1</b>:
  - Initial version.
  <b>Revision 2</b>:
  - Added GdxcBaseAddress, EdramBaseAddress, EpBaseAddress, ApicLocalAddress, NvmeHcPeiMmioBase.
  - Added NvmeHcPeiMmioLimit, AhciPeiMmioBase, AhciPeiMmioLimit, EcExtraIoBase, NvmeHcPeiMmioBase, SioBaseAddress.
  - Added VmdCfgBarBar, VmdMemBar2Bar, VmdMemBar1Bar.
  <b>Revision 3</b>:
  - Removed GdxcBaseAddress, EdramBaseAddress, EpBaseAddress.
  <b>Revision 4</b>:
  - Added CridEnable.
  <b>Revision 5</b>:
  - Resized MchBar, RegBar
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT64  MchBar;                            ///< Offset 28 Address of System Agent MCHBAR: <b>0xFEDC0000</b>
  UINT64  RegBar;                            ///< Offset 36 Address of System Agent REGBAR: <b>0xF0000000</b>
  /**
    Offset 44 :
    Size of reserved MMIO space for PCI devices\n
    <b>0=AUTO</b>, 512=512MB, 768=768MB, 1024=1024MB, 1280=1280MB, 1536=1536MB, 1792=1792MB,
    2048=2048MB, 2304=2304MB, 2560=2560MB, 2816=2816MB, 3072=3072MB\n
    When AUTO mode selected, the MMIO size will be calculated by required MMIO size from PCIe devices detected.
  **/
  UINT32  MmioSize;
  UINT32  MmioSizeAdjustment;                ///< Offset 48 Increase (given positive value) or Decrease (given negative value) the Reserved MMIO size when Dynamic Tolud/AUTO mode enabled (in MBs): <b>0=no adjustment</b>
  UINT32  ApicLocalAddress;                  ///< Offset 52 Address of System Agent ApicLocalAddress:   <b>0xFEE00000</b>
  UINT32  NvmeHcPeiMmioBase;                 ///< Offset 56 Address of System Agent NvmeHcPeiMmioBase:  <b>0xA0000000</b>
  UINT32  NvmeHcPeiMmioLimit;                ///< Offset 60 Address of System Agent NvmeHcPeiMmioLimit: <b>0xA1000000</b>
  UINT32  AhciPeiMmioBase;                   ///< Offset 64 Address of System Agent AhciPeiMmioBase:    <b>0xD1000000</b>
  UINT32  AhciPeiMmioLimit;                  ///< Offset 68 Address of System Agent AhciPeiMmioLimit:   <b>0xD10FFFFF</b>
  UINT16  EcExtraIoBase;                     ///< Offset 72 Address of System Agent EcExtraIoBase:      <b>0x6A0</b>
  UINT16  SioBaseAddress;                    ///< Offset 74 Address of System Agent SioBaseAddress:     <b>0x0680</b>
  UINT32  VmdCfgBarBar;                      ///< Offset 76 Address of System Agent VmdCfgBarBase:      <b>0xA0000000</b>
  UINT32  VmdMemBar2Bar;                     ///< Offset 80 Address of System Agent VmdMemBar2Base:     <b>0xA4000000</b>
  UINT32  VmdMemBar1Bar;                     ///< Offset 84 Address of System Agent VmdMemBar1Base:     <b>0xA2000000</b>
  UINT64  SafBar;                            ///< Offset 88 Address of System Agent SafBar:             <b>0x4150000000</b>
  UINT8   EnableAbove4GBMmio;                ///< Offset 96 Enable/disable above 4GB MMIO resource support: 0=Disable, <b>1=Enable</b>
  UINT8   CridEnable;                        ///< Offset 97 For Platforms supporting Intel(R) SIPP, this policy is use control enable/disable Compatibility Revision ID (CRID) feature: <b>0=FALSE</b>, 1=TRUE
  UINT8   Reserved[2];                       ///< Offset 98-97 Reserved to ensure config block size is a multiple of DWORDs
} HOST_BRIDGE_PREMEM_CONFIG;


/**
  This configuration block describes HostBridge settings in Post-Mem.\n
  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;               ///< Offset 0-27 Config Block Header
  UINT8  Device4Enable;                      ///< Offser 28 :This policy is used to control enable or disable System Agent Thermal device (0,4,0). <b>0=FALSE</b>, 1=TRUE.
  UINT8  SkipPamLock;                        ///< Offset 29 :To skip PAM register locking. @note It is still recommended to set PCI Config space B0: D0: F0: Offset 80h[0]=1 in platform code even Silicon code skipped this.\n <b>0=All PAM registers will be locked in Silicon code</b>, 1=Skip lock PAM registers in Silicon code.
  UINT8  Reserved[2];                        ///< Offset 31 Reserved for future use.
} HOST_BRIDGE_PEI_CONFIG;

#pragma pack (pop)

#endif // _HOST_BRIDGE_CONFIG_SILICON_H_
