/**@file
  Library provide the interface to allocate the memory for PMON (Performance Monitoring),
  save the allocated memory address to PMON MSR and populate the PMON discovery table to the memory.

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

#include <IndustryStandard/Pci.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PciLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/SiPolicyLib.h>
#include <Library/ConfigBlockLib.h>
#include <Library/HobLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PmonLib.h>
#include <Library/PmonDataLib.h>
#include <Register/CpuGenRegs.h>
#include <Register/SncuRegs.h>

/**
  Program the PMON allocated memory so that PMON Software can consume it.

  @param[in] SiPolicyPpi Pointer to SI_POLICY_PPI.

  @retval EFI_SUCCESS    The function completes successfully.
  @retval others         The function fails.

**/
EFI_STATUS
PmonMemoryAddressProgram (
  IN SI_POLICY_PPI       *SiPolicyPpi
  )
{
  EFI_STATUS             Status;
  UINT8                  *Pointer;
  CPU_INIT_CONFIG        *CpuInitConfig;
  PMON_HOB_DATA          *PmonDataHob;

  Status      = EFI_SUCCESS;
  Pointer     = NULL;
  PmonDataHob = NULL;

  if (SiPolicyPpi == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = CreatePmonHob (SiPolicyPpi, &PmonDataHob);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  //
  // Locate the PMON Data hob, if not found or not enable, return
  //
  if ((PmonDataHob == NULL) || !PmonDataHob->PmonEnable) {
    DEBUG ((DEBUG_INFO, "%a: PMON not support.\n", __FUNCTION__));
    return EFI_UNSUPPORTED;
  }

  //
  // Allocate the reserved memory for PMON
  //
  Pointer = AllocateReservedZeroPool (PMON_MEMORY_SIZE);
  if (Pointer == NULL) {
    DEBUG ((DEBUG_INFO, "%a: PMON memory allocation failed. Out of memory.\n", __FUNCTION__));
    return EFI_OUT_OF_RESOURCES;
  } else {
    PmonDataHob->PmonAddress = (UINTN) Pointer;
  }

  //
  // Program the PMON allocated memory so that PMON Software can consume it
  //
  Status = GetConfigBlock ((VOID *) SiPolicyPpi, &gCpuInitConfigGuid, (VOID *) &CpuInitConfig);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  DEBUG ((DEBUG_INFO, "%a:PMON enable:%d, memory address 0x%X.\n", __FUNCTION__, PmonDataHob->PmonEnable, PmonDataHob->PmonAddress));

  //
  // Set the Pmon config to CpuInitConfig for later writen Pmon address to Pmon bank1 MSR.
  //
  CpuInitConfig->PmonEnable  = PmonDataHob->PmonEnable;
  CpuInitConfig->PmonAddress = PmonDataHob->PmonAddress;

  return Status;
}

/**
  Get the register address for PMON block.

  @param[in]  PmonDiscoveryTableType   The Pmon discovery table type, Global or Unit.
  @param[in]  PortId                   Pmon port id.
  @param[out] OutputData               Output address in bar for the Pmon.

  @retval EFI_SUCCESS                  success to get the address
**/
EFI_STATUS
PmonGetAddress (
  IN PMON_DISCOVERY_TABLE_TYPE   PmonDiscoveryTableType,
  IN UINT16                      PortId,
  IN OUT UINT64                  *OutputData
  )
{
  UINT64        BaseAddress;
  UINT32        BarL;
  UINT64        RegisterAddress;
  UINT64        AddressOffset;
  UINT64        BarAddress;

  if (OutputData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  AddressOffset = *OutputData;
  switch (PortId) {
    case PMON_PORTID_MC0:
    case PMON_PORTID_MC1:
    {
      BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, MCHBAR_HOSTBRIDGE_CFG_REG );
      BarL = (PciSegmentRead32 (BaseAddress));
      if ((BarL & DEV0_BAR_EN) == DEV0_BAR_EN) {
        BarL &= (~DEV0_BAR_EN);
        BarAddress = BarL | (LShiftU64 (PciSegmentRead32 (BaseAddress + 4), 32));
        if (PortId == PMON_PORTID_MC0) {
          RegisterAddress = (BarAddress + MCHBAR_MC0 + AddressOffset);
        } else {
          RegisterAddress = (BarAddress + MCHBAR_MC1 + AddressOffset);
        }

        DEBUG ((DEBUG_INFO, "%a: MCHBAR :0x%lx, RegisterAddress:0x%016lx.\n", __FUNCTION__, BarAddress, RegisterAddress));
        *OutputData = RegisterAddress;
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "%a: MCHBAR is not programmed!\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
    }
    case PMON_PORTID_HBO0:
    case PMON_PORTID_HBO1:
    case PMON_PORTID_NCU:
    {
      BaseAddress = PCI_SEGMENT_LIB_ADDRESS (SA_SEG_NUM, SA_MC_BUS, SA_MC_DEV, SA_MC_FUN, SAFBAR_HOSTBRIDGE_CFG_REG);
      BarL = (PciSegmentRead32 (BaseAddress));
      if ((BarL & DEV0_BAR_EN) == DEV0_BAR_EN) {
        BarL &= (~DEV0_BAR_EN);
        BarAddress = BarL | (LShiftU64 (PciSegmentRead32 (BaseAddress + 4), 32));
        RegisterAddress = BarAddress;
        RegisterAddress += SAFBAR_MEMORY_ADDRESS (SEGMENT_ID_C_DIE, PortId, AddressOffset);
        DEBUG ((DEBUG_INFO, "%a: SAFBAR :0x%016lx, RegisterAddress:0x%016lx.\n", __FUNCTION__, BarAddress, RegisterAddress));
        *OutputData = RegisterAddress;
        return EFI_SUCCESS;
      } else {
        DEBUG ((DEBUG_INFO, "%a: SAFBAR is not programmed!\n", __FUNCTION__));
        return EFI_NOT_READY;
      }
    }
    default:
      DEBUG ((DEBUG_INFO, "%a: Port id %X is not support!\n", __FUNCTION__, PortId));
      break;
  }
  return EFI_UNSUPPORTED;
}

/**
  Dump PMON for debug.

  @retval
**/
VOID
DumpPmon(
  IN UINT8      *Buffer,
  IN UINT64     Size
  )
{
  UINT64     row = 0;
  UINT8      col = 0;
  UINT64     Offset = 0;

  if((Buffer == NULL) || (Size == 0)) {
    return;
  }

  DEBUG ((DEBUG_INFO, "Dump Pmon\n"));
  DEBUG ((DEBUG_INFO, "========================\n"));
  for (row = 0; row <= 0xf; row++) {
    DEBUG ((DEBUG_INFO, "%.8xh: ", Offset));
    for(col = 0; col <= 0xf; col++) {
      DEBUG ((DEBUG_INFO, "%.2x ", *(Buffer+Offset)));
      Offset++;
      if(Offset >= Size)
        break;
    }
    DEBUG ((DEBUG_INFO, "\n"));
    if(Offset >= Size)
      break;
  }
  DEBUG ((DEBUG_INFO, " ========================\n"));
}

/**
  Populate PMON discovery table to the memory.

  @retval EFI_SUCCESS     The function completes successfully
**/
EFI_STATUS
PmonPopulateDiscoveryTable (
  )
{
  EFI_STATUS                Status;
  UINT64                    PmonAddress;
  PMON_GLOBAL_DISCOVERY     *GlobalPtr;
  PMON_UNIT_DISCOVERY       *UnitPtr;
  PMON_UNIT_DISCOVERY       *UnitSrcPtr;
  UINTN                     Index;
  EFI_HOB_GUID_TYPE         *PmonHob;
  PMON_HOB_DATA             *PmonDataHob;
  VOID                      *DataPtr;
  UINTN                     Size;

  //
  // Locate the PMON Data hob, if not found or not enable, return
  //
  PmonHob = GetFirstGuidHob (&gPmonDataHobGuid);
  if (PmonHob == NULL) {
    return EFI_UNSUPPORTED;
  } else {
    PmonDataHob = (PMON_HOB_DATA *) GET_GUID_HOB_DATA (PmonHob);
    if (PmonDataHob == NULL || !PmonDataHob->PmonEnable) {
      return EFI_UNSUPPORTED;
    }
  }

  PmonAddress = PmonDataHob->PmonAddress;
  DEBUG ((DEBUG_INFO, "%a: PmonAddress:0x%016lx.\n", __FUNCTION__, PmonAddress));
  if (PmonAddress != 0x0) {
    DataPtr = GetPmonGlobalDiscoveryTable (&Size);
    if ((DataPtr != NULL) && (Size > 0)) {
      //
      // Populate global discovery
      //
      GlobalPtr = (PMON_GLOBAL_DISCOVERY*) (UINTN) PmonAddress;
      CopyMem (GlobalPtr, DataPtr, Size);
      Status = PmonGetAddress (
                GLOBAL_DISCOVERY_TABLE,
                PMON_PORTID_NCU,
                &GlobalPtr->GlobalControlAddress
                );
      if (EFI_ERROR(Status)) {
        GlobalPtr->GlobalControlAddress = INVALID_ADDRESS_64 ;
      }

      //
      // Populate unit discovery
      //
      DEBUG ((DEBUG_INFO, "%a: start NumBlockStatus:%d.\n", __FUNCTION__, GlobalPtr->NumBlockStatusBitsCtrl));
      DataPtr = GetPmonUnitDiscoveryTable (&Size);
      if ((GlobalPtr->NumBlockStatusBitsCtrl > 0) && (DataPtr != NULL) && (Size > 0)) {
        UnitPtr = (PMON_UNIT_DISCOVERY*) (GlobalPtr + 1);
        UnitSrcPtr = (PMON_UNIT_DISCOVERY*) DataPtr;
        DEBUG ((DEBUG_INFO, "%a: Unit discovery address:%p size:0x%X.\n", __FUNCTION__, UnitPtr, Size));
        Index = 0;
        while ((Index < GlobalPtr->NumBlockStatusBitsCtrl) && (Size > 0)) {
          if (GetPmonPortSupport ((UINT16)UnitSrcPtr->PmonBlockId)) {
            CopyMem (UnitPtr, UnitSrcPtr, sizeof (PMON_UNIT_DISCOVERY));
            Status = PmonGetAddress (
              UNIT_DISCOVERY_TABLE,
              (UINT16) UnitPtr->PmonBlockId,
              &(UnitPtr->UnitControlAddress)
              );
            if (EFI_ERROR(Status)) {
              UnitPtr->UnitControlAddress = INVALID_ADDRESS_64 ;
            }
            UnitPtr++;
          }
          Index++;
          UnitSrcPtr++;
          Size -= sizeof (PMON_UNIT_DISCOVERY);
        }
      }
      DEBUG_CODE_BEGIN ();
      DumpPmon((UINT8*)(UINTN)PmonAddress, GlobalPtr->NumBlockStatusBitsCtrl*sizeof(PMON_UNIT_DISCOVERY)+ sizeof (PMON_GLOBAL_DISCOVERY));
      DEBUG_CODE_END ();
    }
  }
  return EFI_SUCCESS;
}
