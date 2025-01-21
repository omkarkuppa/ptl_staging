/** @file
  ITBT PCIE root port library.
  All function in this library is available for PEI, DXE, and SMM.
  This library has a dependency with TcssHob.

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

@par Specification Reference:
**/

#include <Library/ItbtPcieRpLib.h>
#include <Uefi.h>
#include <Library/DebugLib.h>
#include <Register/ItbtPcieRegs.h>
#include <Library/HobLib.h>

GLOBAL_REMOVE_IF_UNREFERENCED ITBT_PCIE_RP_INFO mIbtbPcieRpInfo[] = {
//
// Segment       Bus                  Device                 Function                 Enable
//
  { 0,     ITBT_PCIE_BUS_NUM, ITBT_PCIE_DEV_NUM, ITBT_PCIE_0_DEFAULT_FUN_NUM, TRUE },  // Itbt Pcie Rp 1
  { 0,     ITBT_PCIE_BUS_NUM, ITBT_PCIE_DEV_NUM, ITBT_PCIE_1_DEFAULT_FUN_NUM, TRUE },  // Itbt Pcie Rp 2
  { 0,     ITBT_PCIE_BUS_NUM, ITBT_PCIE_DEV_NUM, ITBT_PCIE_2_DEFAULT_FUN_NUM, FALSE }, // Itbt Pcie Rp 3
  { 0,     ITBT_PCIE_BUS_NUM, ITBT_PCIE_DEV_NUM, ITBT_PCIE_3_DEFAULT_FUN_NUM, FALSE }  // Itbt Pcie Rp 4
};

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mItbtPcieRpCount = sizeof (mIbtbPcieRpInfo) / sizeof (mIbtbPcieRpInfo[0]);

GLOBAL_REMOVE_IF_UNREFERENCED BOOLEAN mItbtPcieRpInfoUpdated = FALSE;

/**
  Get ITBT Pcie Root Port Segment Bus Device and Function Number by Port Index

  @param[in]  PortIndex             Root port index. (0-based)
  @param[out] Segment               Return corresponding root port segment number.
  @param[out] Bus                   Return corresponding root port bus number.
  @param[out] Device                Return corresponding root port device number.
  @param[out] Function              Return corresponding root port function number.

  @retval     EFI_SUCCESS           Root port Info is retrieved
  @retval     EFI_UNSUPPORTED       PortIndex is invalid/disabled, or TcssHob is not existed when mItbtPcieRpInfoUpdated is FALSE.
**/
EFI_STATUS
EFIAPI
GetItbtPcieRpInfo (
  IN   UINTN    PortIndex,
  OUT  UINTN    *Segment,
  OUT  UINTN    *Bus,
  OUT  UINTN    *Device,
  OUT  UINTN    *Function
  )
{
  TCSS_DATA_HOB            *TcssHob;
  UINT32                   PcieFuncMap;

  TcssHob   = NULL;
  PcieFuncMap = 0;

  //
  // Iniatialize the Segment, Bus, Device, Function
  //
  *Segment  = 0;
  *Bus      = 0;
  *Device   = 0;
  *Function = 0;

  if(PortIndex >= MAX_ITBT_PCIE_PORT) {
    DEBUG ((DEBUG_ERROR, "Invalid PortIndex %x", PortIndex));
    return EFI_UNSUPPORTED;
  }

  if (mItbtPcieRpInfoUpdated == TRUE) {
    if (mIbtbPcieRpInfo[PortIndex].Enable == TRUE) {
      *Segment  = mIbtbPcieRpInfo[PortIndex].Segment;
      *Bus      = mIbtbPcieRpInfo[PortIndex].Bus;
      *Device   = mIbtbPcieRpInfo[PortIndex].Device;
      *Function = mIbtbPcieRpInfo[PortIndex].Function;
      return EFI_SUCCESS;
    }
  } else {
    TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
    if (TcssHob != NULL) {
      PcieFuncMap= TcssHob->TcssData.ItbtPcieFuncMap;
      //
      // Update the Function pointers based on PcieFuncMap when Port enabled
      //
      if (TcssHob->TcssData.ItbtPcieRpEn[PortIndex]) {
        *Segment  = 0;
        *Bus      = mIbtbPcieRpInfo[PortIndex].Bus;
        *Device   = mIbtbPcieRpInfo[PortIndex].Device;
        *Function = (PcieFuncMap >> (3 * PortIndex) ) & 0x7;
        return EFI_SUCCESS;
      }
    }
  }

  return EFI_UNSUPPORTED;
}

/**
  Get ITBT PCIE Root Port Index by ITBT PCIE Root Port Segment Bus Device and Function Number

  @param[in]  Segment             Root port segment number.
  @param[in]  Bus                 Root port Bus.
  @param[in]  Device              Root port Device.
  @param[in]  Function            Root port Function.
  @param[out] PortIndex           Return corresponding Root Port index (0-based)

  @retval     EFI_SUCCESS           PortIndex is retrieved
  @retval     EFI_UNSUPPORTED       PortIndex is invalid/disabled or TcssHob is not existed when mItbtPcieRpInfoUpdated is FALSE.
**/
EFI_STATUS
EFIAPI
GetItbtPciePortIndex (
  IN   UINTN    Segment,
  IN   UINTN    Bus,
  IN   UINTN    Device,
  IN   UINTN    Function,
  OUT  UINTN    *PortIndex
  )
{
  UINTN                    Index;
  TCSS_DATA_HOB            *TcssHob;
  UINT32                   PcieFuncMap;
  UINT8                    Seg;
  UINT8                    Func;

  TcssHob   = NULL;
  PcieFuncMap = 0;
  Seg         = 0;
  Func        = 0;

  for (Index = 0; Index < mItbtPcieRpCount; Index++) {
    if (mItbtPcieRpInfoUpdated == TRUE) {
      if (mIbtbPcieRpInfo[Index].Segment == Segment && mIbtbPcieRpInfo[Index].Bus == Bus && mIbtbPcieRpInfo[Index].Device == Device && mIbtbPcieRpInfo[Index].Function == Function) {
        if (mIbtbPcieRpInfo[Index].Enable == TRUE) {
          *PortIndex = Index;
          return EFI_SUCCESS;
        }
      }
    } else {
      TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);
      if (TcssHob != NULL) {
        PcieFuncMap= TcssHob->TcssData.ItbtPcieFuncMap;
        Seg  = 0;
        Func = (PcieFuncMap >> (3 * Index) ) & 0x7;

        if (Seg == Segment && mIbtbPcieRpInfo[Index].Bus == Bus && mIbtbPcieRpInfo[Index].Device == Device && Func == Function) {
          *PortIndex = Index;
          return EFI_SUCCESS;
        }
      }
    }
  }
  return EFI_UNSUPPORTED;
}

/**
  This constructor performs initialization for TBT Device Info Internal Structure.
**/
EFI_STATUS
EFIAPI
ITbtPcieRpLibConstructor (
  VOID
  )
{
  TCSS_DATA_HOB              *TcssHob;
  UINT32                   PcieFuncMap;
  UINTN                    Index;
  UINT8                    Segment;
  UINT8                    Function;

  DEBUG ((DEBUG_INFO, "ITbtPcieRpLibConstructor - Start\n"));

  TcssHob = NULL;
  TcssHob = (TCSS_DATA_HOB *) GetFirstGuidHob (&gTcssHobGuid);

  if ((TcssHob != NULL) && (TcssHob->TcssData.IOMReady == TRUE)) {
    PcieFuncMap = TcssHob->TcssData.ItbtPcieFuncMap;

    for(Index = 0; Index < mItbtPcieRpCount; Index++) {
      Function = (PcieFuncMap >> (3 * Index) ) & 0x7;
      Segment  = 0;

      DEBUG ((DEBUG_INFO, "iTBT PCIE RP%d Segment %d Function %d \n",Index,Segment, Function));
      mIbtbPcieRpInfo[Index].Function = (UINT8)Function;
      mIbtbPcieRpInfo[Index].Segment  = (UINT8)Segment;
      //
      //  Update the Enable Status based on iTBT Pcie RP Status
      //
      if (TcssHob->TcssData.ItbtPcieRpEn[Index]) {
        mIbtbPcieRpInfo[Index].Enable = TRUE;
      } else {
        mIbtbPcieRpInfo[Index].Enable = FALSE;
      }
    }
    //
    // Update the global flag indicating iTBT PCIE RP Info Table Updated
    //
    mItbtPcieRpInfoUpdated = TRUE;
  } else {
    DEBUG ((DEBUG_INFO, "Do nothing.\n"));
  }
  DEBUG ((DEBUG_INFO, "ITbtPcieRpLibConstructor - End\n"));

  return EFI_SUCCESS;
}
