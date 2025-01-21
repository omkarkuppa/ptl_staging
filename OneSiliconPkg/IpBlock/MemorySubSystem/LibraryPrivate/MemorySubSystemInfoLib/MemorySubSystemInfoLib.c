/** @file
  Memory Subsystem Info Library.

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

#include <PiPei.h>
#include <Library/BaseLib.h>
#include <Library/IoLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/SiPolicy.h>
#include <MemorySubSystemHob.h>
#include <Library/HobLib.h>
#include <MemorySubSystemHandle.h>
#include <Defines/HostBridgeDefines.h>
#include <Register/NocRegs.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#include <Library/NguInfoLib.h>

//
// Support 64 K IO space
// Moving RES_IO_BASE due to new ACPI Base address 0x1800
//
#define RES_IO_BASE   0x2000
#define RES_IO_LIMIT  0xFFFF
//
// Support 4G address space
//
#define RES_MEM_LIMIT_1 ((UINTN) PcdGet64 (PcdSiPciExpressBaseAddress) - 1)

/**
  This function returns True if CCE0 Enable in MemSS PMA

  @retval TRUE  - if CCE0 Enable
  @retval FALSE - if CCE0 Disable 
**/
BOOLEAN
EFIAPI
IsCce0Enable (
  VOID
  )
{
  UINT64                                        MchBar;
  R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_STRUCT    MemSsPmaCrMemConfig;

  MchBar              = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemSsPmaCrMemConfig.Data = MmioRead32 ((UINTN) (MchBar + R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_REG));
  DEBUG ((DEBUG_INFO, "CCE0 Enable Status = 0x%x\n", MemSsPmaCrMemConfig.Bits.cce0_en));
  if (MemSsPmaCrMemConfig.Bits.cce0_en) {
    return TRUE;
  }
  return FALSE;
}

/**
  This function returns True if CCE1 Enable in MemSS PMA

  @retval TRUE  - if CCE1 Enable
  @retval FALSE - if CCE1 Disable 
**/
BOOLEAN
EFIAPI
IsCce1Enable (
  VOID
  )
{
  UINT64                                        MchBar;
  R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_STRUCT    MemSsPmaCrMemConfig;

  MchBar              = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemSsPmaCrMemConfig.Data = MmioRead32 ((UINTN) (MchBar + R_SA_MCHBAR_MEMSS_PMA_CR_MEM_CONFIG_REG));
  DEBUG ((DEBUG_INFO, "CCE1 Enable Status = 0x%x\n", MemSsPmaCrMemConfig.Bits.cce1_en));
  if (MemSsPmaCrMemConfig.Bits.cce1_en) {
    return TRUE;
  }
  return FALSE;
}

/**
  This function returns the Mc hash LSB

  @retval Mc Hash LSB
**/
UINT32
EFIAPI
GetMcHashLsb (
  VOID
  )
{
  UINT64                              MchBar;
  MEMORY_SLICE_HASH_HBO_MEM_STRUCT    MemorySliceHash;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemorySliceHash.Data= MmioRead32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG));
  return MemorySliceHash.Bits.hash_lsb;
}

/**
  This function returns the Mc hash Mask

  @retval Mc Hash Mask
**/
UINT32
EFIAPI
GetMcHashMask (
  VOID
  )
{
  UINT64                              MchBar;
  MEMORY_SLICE_HASH_HBO_MEM_STRUCT    MemorySliceHash;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemorySliceHash.Data= MmioRead32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG));
  return MemorySliceHash.Bits.hash_mask;
}

/**
  This function returns Mc hash is enabled or not.
  @retval Mc Hash Enable
**/
BOOLEAN
EFIAPI
GetMcHashEnable (
  VOID
  )
{
  UINT64                              MchBar;
  MEMORY_SLICE_HASH_HBO_MEM_STRUCT    MemorySliceHash;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);
  MemorySliceHash.Data= MmioRead32 (MchBar + GET_HBO_EFFECTIVE_ADDRESS(MEMORY_SLICE_HASH_HBO_MEM_REG));

  return (BOOLEAN) MemorySliceHash.Bits.hash_enabled;
}

/**
  Check whether the Base Address is Naturally aligned or not - Base needs to be multiple of size.
  If not then it will return the Naturally aligned Base Address

  @param[in]       EFI_PHYSICAL_ADDRESS MemBaseAddr   - Base Address
  @param[in]       UINT64               MemSize       - Size

  @retval          EFI_PHYSICAL_ADDRESS MemBaseAddr   -  Naturally Aligned Base Address
**/
EFI_PHYSICAL_ADDRESS
NaturalAlignment (
  IN  EFI_PHYSICAL_ADDRESS  MemBaseAddr,
  IN  UINT64                MemSize
  )
{
  UINT64 Remainder;

  DEBUG ((DEBUG_INFO, "Address = 0x%lx Size = 0x%x\n", MemBaseAddr, MemSize));

  if (MemSize == 0) {
    return MemBaseAddr;
  }
  
  DivU64x64Remainder (MemBaseAddr, MemSize, &Remainder);
  if (Remainder == 0) {
    DEBUG ((DEBUG_INFO, "The Base Address = 0x%lx is naturally aligned with Size = 0x%x\n", MemBaseAddr, MemSize));
    return MemBaseAddr;
  } else {
    MemBaseAddr += (MemSize - Remainder);
    DEBUG ((DEBUG_INFO, "The adjusted Base Address = 0x%lx is naturally aligned with Size = 0x%x\n", MemBaseAddr, MemSize));
    return MemBaseAddr;
  }

}

/**
  Reserve Memory for I/O Space

**/
VOID
PeiIOMemoryAllocation(
  VOID
  )
{
  //
  // Allocate I/O space resource
  //
  BuildResourceDescriptorHob (
    EFI_RESOURCE_IO,
    (
      EFI_RESOURCE_ATTRIBUTE_PRESENT     |
      EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
      EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE
    ),
    RES_IO_BASE,
    RES_IO_LIMIT - RES_IO_BASE + 1
  );
  DEBUG ((DEBUG_INFO, " Providing I/O space resource from 0x%X to 0x%X\n", RES_IO_BASE, RES_IO_LIMIT - RES_IO_BASE + 1));

  return;
}

/**
  Reserve Memory from Tolud to PCie base

**/
VOID
ReserveMmioBelowPciConfig (
  VOID
  )
{
  EFI_PHYSICAL_ADDRESS         ToludBaseAddress;
  UINT64                       Length;

  //
  // Allocate PCI memory space.
  //
  ToludBaseAddress     = (EFI_PHYSICAL_ADDRESS) (UINT32) GetHostBridgeRegisterData (HostBridgeTolud, HostBridgeToludBase);
  Length = RES_MEM_LIMIT_1 - ToludBaseAddress + 1;

  if (Length != 0) {
    DEBUG ((DEBUG_INFO, " Reserving Tolud to PCIe space space 0x%X to 0x%X\n", (UINT32) ToludBaseAddress, (UINT32)(ToludBaseAddress + Length - 1)));
    BuildResourceDescriptorHob (
      EFI_RESOURCE_MEMORY_MAPPED_IO,
      (
        EFI_RESOURCE_ATTRIBUTE_PRESENT     |
        EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
        EFI_RESOURCE_ATTRIBUTE_UNCACHEABLE
      ),
      ToludBaseAddress,
      Length
    );
  }
}