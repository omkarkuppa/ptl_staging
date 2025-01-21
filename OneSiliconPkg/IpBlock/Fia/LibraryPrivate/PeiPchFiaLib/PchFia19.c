/** @file
  This file contains routines used to communicate with FIA 19.0 hardware

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

#include <Library/DebugLib.h>
#include <Library/PchFia19.h>
#include <Register/PchFiaRegs.h>

/**
  Converts FIA 19 lane owner encoding to version independent encoding.

  @param[in] LaneOwner  Lane owner value corresponding to the FIA 19 lane owner encoding
  @param[in] LaneNum    Fia lane number

  @retval PCH_FIA_LANE_OWNER  FIA lane owner in version independent format.
**/
PCH_FIA_LANE_OWNER
PchFiaDecodeOwner (
  IN UINT32  LaneOwner,
  IN UINT8   LaneNum
  );

/**
  Get FIA lane owner

  @param[in] FiaInst  FIA Instance
  @param[in] LaneNum  lane number

  @retval PCH_FIA_LANE_OWNER  FIA lane owner
**/
PCH_FIA_LANE_OWNER
PchFia19GetLaneOwner (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        LaneNum
  )
{
  UINT32  LosValue;
  UINT32  LosRegisterIndex;

  LosRegisterIndex = LaneNum / 8;
  //
  // Only 5 LOS registers in FIA 19.0
  //
  if (LosRegisterIndex > 4) {
    ASSERT (FALSE);
    return PchFiaOwnerInvalid;
  }

  LosValue = FiaInst.RegisterAccess->Read32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_LOS1_REG_BASE + (4 * LosRegisterIndex));
  return PchFiaDecodeOwner (LosValue >> (4 * (LaneNum % 8)) & B_PCH_FIA_PCR_LOS1_L0O, LaneNum);
}

/**
  Print FIA LOS registers.

  @param[in] FiaInst  FIA Instance
**/
VOID
PchFia19PrintLosRegisters (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        MaxLaneNum
  )
{
  UINT32  Index;
  UINT32  MaxLosRegister;

  MaxLosRegister = ((MaxLaneNum - 1) / 8) + 1;
  for (Index = 0; Index < MaxLosRegister; Index++) {
    DEBUG ((DEBUG_INFO, "FIA LOS%d = 0x%08X\n", Index + 1, FiaInst.RegisterAccess->Read32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_LOS1_REG_BASE + (4 * Index))));
  }
}

/**
  Assigns CLKREQ# to PCH PCIe ports

  @param[in] FiaInst        FIA Instance
  @param[in] ClkReqMap      Mapping between PCH PCIe ports and CLKREQ#
  @param[in] ClkReqMapSize  Size of the map
**/
VOID
PchFia19AssignPchPciePortsClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        *ClkReqMap,
  IN UINT8        ClkReqMapSize
  )
{
  UINT32  RegisterValue;
  UINT8   RegisterIndex;
  UINT8   ClkReqIndex;
  UINT8   FirstClkReqIndex;
  UINT8   LastClkReqIndex;
  UINT8   ClkReqPosition;

  ASSERT (ClkReqMapSize <= PCH_FIA_19_MAX_PCH_PCIE_CLKREQ);

  for (RegisterIndex = 0; RegisterIndex < PCH_FIA_19_MAX_PDRCRM; RegisterIndex++) {

    RegisterValue = 0;
    FirstClkReqIndex = RegisterIndex * S_PCH_FIA_19_PCR_DRCRM_FIELDS_PER_REG;
    LastClkReqIndex = FirstClkReqIndex + S_PCH_FIA_19_PCR_DRCRM_FIELDS_PER_REG;

    for (ClkReqIndex = FirstClkReqIndex; (ClkReqIndex < LastClkReqIndex) && (ClkReqIndex < PCH_FIA_19_MAX_PCH_PCIE_CLKREQ); ClkReqIndex++) {
      if ((ClkReqIndex < ClkReqMapSize) && (ClkReqMap[ClkReqIndex] != PCH_FIA_NO_CLKREQ)) {
        ClkReqPosition = S_PCH_FIA_19_PCR_DRCRM_BITS_PER_FIELD * (ClkReqIndex % S_PCH_FIA_19_PCR_DRCRM_FIELDS_PER_REG);
        RegisterValue |= ClkReqMap[ClkReqIndex] << ClkReqPosition;
      }
    }

    FiaInst.RegisterAccess->Write32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_PDRCRM1 + (RegisterIndex * 4), RegisterValue);
  }

  RegisterIndex = 0;
  RegisterValue = 0;
  for (ClkReqIndex = 0; ClkReqIndex < PCH_FIA_19_MAX_PCH_PCIE_CLKREQ; ClkReqIndex++) {

    if (ClkReqIndex < ClkReqMapSize && ClkReqMap[ClkReqIndex] != PCH_FIA_NO_CLKREQ) {
      RegisterValue |= 1 << (ClkReqIndex % 32);
    }

    if ((ClkReqIndex % 32 == 31) || (ClkReqIndex == PCH_FIA_19_MAX_PCH_PCIE_CLKREQ - 1)) {
      FiaInst.RegisterAccess->Write32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_PDRCRE1 + (RegisterIndex * 4), RegisterValue);
      RegisterValue = 0;
      RegisterIndex++;
    }
  }
}

/**
  Print CLKREQ# assigined to PCH PCIe ports

  @param[in] FiaInst        FIA Instance
**/
VOID
PchFia19PrintPciePortsClkReq (
  IN UINT8        FiaPid,
  IN FIA_INSTANCE FiaInst
  )
{
  UINT8   RegisterIndex;

  DEBUG ((DEBUG_INFO, "FIA register print Start \n"));
  DEBUG ((DEBUG_INFO, "FIA Port ID = 0x%x \n", FiaPid));

  for (RegisterIndex = 0; RegisterIndex < PCH_FIA_19_MAX_PDRCRM; RegisterIndex++) {
    DEBUG((DEBUG_INFO, "R_PCH_FIA_19_PCR_PDRCRM%d : Offset: 0x%x, value: 0x%x \n",
      RegisterIndex + 1,
      R_PCH_FIA_19_PCR_PDRCRM1 + (RegisterIndex * 4),
      FiaInst.RegisterAccess->Read32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_PDRCRM1 + (RegisterIndex * 4))
    ));
  }

  for (RegisterIndex = 0; RegisterIndex < PCH_FIA_19_MAX_PDRCRE; RegisterIndex++) {
    DEBUG((DEBUG_INFO, "R_PCH_FIA_19_PCR_PDRCRE%d : Offset: 0x%x, value: 0x%x \n",
      RegisterIndex + 1,
      R_PCH_FIA_19_PCR_PDRCRE1 + (RegisterIndex * 4),
      FiaInst.RegisterAccess->Read32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_PDRCRE1 + (RegisterIndex * 4))
    ));
  }
  DEBUG ((DEBUG_INFO, "FIA register print End \n"));
}

/**
  Disables ClkReq mapping for PCIe root port

  @param[in] FiaInst  FIA Instance
  @param[in] RpIndex  Root port index
**/
VOID
PchFia19DisablePchPciePortClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT32       RpIndex
  )
{
  UINT32  RegisterIndex;
  UINT32  FiaPortIndex;

  RegisterIndex = RpIndex / 32;
  FiaPortIndex = RpIndex % 32;
  FiaInst.RegisterAccess->AndThenOr32 (FiaInst.RegisterAccess, R_PCH_FIA_19_PCR_PDRCRE1 + RegisterIndex, ~(1 << FiaPortIndex), 0);
}

/**
  Assigns CLKREQ# to GbE

  @param[in] FiaInst    FIA Instance
  @param[in] ClkReqNum  CLKREQ# number
**/
VOID
PchFia19AssignGbeClkReq (
  IN FIA_INSTANCE FiaInst,
  IN UINT8        ClkReqNum
  )
{
  if (ClkReqNum == PCH_FIA_NO_CLKREQ) {
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_19_PCR_GDRCREM,
      ~(UINT32) B_PCH_FIA_19_PCR_GDRCREM_ENABLE,
      0);
  } else {
    ASSERT (ClkReqNum <= PCH_FIA_19_MAX_PCH_PCIE_CLKREQ);
    FiaInst.RegisterAccess->AndThenOr32 (
      FiaInst.RegisterAccess,
      R_PCH_FIA_19_PCR_GDRCREM,
      (UINT32)~(B_PCH_FIA_19_PCR_GDRCREM_NUMBER),
      ((ClkReqNum << N_PCH_FIA_19_PCR_GDRCREM_NUMBER) | (B_PCH_FIA_19_PCR_GDRCREM_ENABLE)));
  }
}

/**
  Configures lower bound of delay between ClkReq assertion and driving RefClk.

  @param[in] FiaInst      FIA Instance
  @param[in] OutputDelay  Clock output delay
**/
VOID
PchFia19SetClockOutputDelay (
  IN FIA_INSTANCE            FiaInst,
  IN FIA_CLOCK_OUTPUT_DELAY  OutputDelay
  )
{
  UINT32 Data32Or;
  UINT32 Data32And;
  UINT32 Index;

  Data32Or = 0;
  Data32And = 0xFFFFFFFF;
  for (Index = 0; Index < S_PCH_FIA_PCR_TREFCLKON_FIELDS_PER_REG; Index++) {
    Data32Or |= (OutputDelay << (Index * S_PCH_FIA_PCR_TREFCLKON_FIELD));
    Data32And &= ~(B_PCH_FIA_PCR_TREFCLKON_FIELD << (Index * S_PCH_FIA_PCR_TREFCLKON_FIELD));
  }
  FiaInst.RegisterAccess->AndThenOr32 (FiaInst.RegisterAccess, R_PCH_FIA_PCR_TREFCLKONTMR1, Data32And, Data32Or);
  FiaInst.RegisterAccess->AndThenOr32 (FiaInst.RegisterAccess, R_PCH_FIA_PCR_TREFCLKONTMR2, Data32And, Data32Or);
  FiaInst.RegisterAccess->AndThenOr32 (FiaInst.RegisterAccess, R_PCH_FIA_PCR_TREFCLKONTMR3, Data32And, Data32Or);
  FiaInst.RegisterAccess->AndThenOr32 (FiaInst.RegisterAccess, R_PCH_FIA_PCR_TREFCLKONTMR4, Data32And, Data32Or);
}

/**
  Converts FIA 19 lane owner encoding to version independent encoding.

  @param[in] LaneOwner  Lane owner value corresponding to the FIA 19 lane owner encoding
  @param[in] LaneNum    Fia lane number

  @retval PCH_FIA_LANE_OWNER  FIA lane owner in version independent format.
**/
PCH_FIA_LANE_OWNER
PchFiaDecodeOwner (
  IN UINT32  LaneOwner,
  IN UINT8   LaneNum
  )
{
  switch (LaneOwner) {
    case V_PCH_FIA_19_PCR_LANE_OWN_USB3:
      return PchFiaOwnerUsb3;
    case V_PCH_FIA_19_PCR_LANE_OWN_PCIE:
      return PchFiaOwnerPcie;
    case V_PCH_FIA_19_PCR_LANE_OWN_DMI:
      return PchFiaOwnerDmi;
    case V_PCH_FIA_19_PCR_LANE_OWN_SATA:
      return PchFiaOwnerSata;
    case V_PCH_FIA_19_PCR_LANE_OWN_GBE:
      return PchFiaOwnerGbe;
    case V_PCH_FIA_19_PCR_LANE_OWN_UFS:
      return PchFiaOwnerUfs;
    case V_PCH_FIA_19_PCR_LANE_OWN_TSN:
      return PchFiaOwnerTsn;
    default:
      return PchFiaOwnerInvalid;
  }
}
