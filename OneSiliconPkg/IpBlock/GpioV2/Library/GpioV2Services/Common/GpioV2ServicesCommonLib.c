/** @file
  Common methods both for PEI and DXE phase for GpioV2Controller lib.

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
#include <Base.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/GpioV2AccessLib.h>
#include <Library/PrintLib.h>
#include <GpioV2Config.h>
#include <GpioV2ControllerInterface.h>
#include <GpioV2Pad.h>
#include <Register/GpioV2ChipsetId.h>
#include <GpioV2Services.h>
#include <Register/GpioRegs.h>
#include <ConfigBlock.h>
#include <Library/HobLib.h>
#include <Register/Sai.h>
#include <GpioV2Pad.h>
#include <Library/BaseLib.h>
#include <GpioV2AccessControl.h>

/**
  This procedure retrieves register offset

  @param[in]  GPIOV2_SERVICES     Gpio Services
  @param[in]  Register            Register for which user want to retrieve offset. Please refer to GpioV2Pad.h
  @param[in]  GpioPad             Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[out] RegisterOffset      Pointer to a buffer for register offset

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetRegisterOffset (
  IN  GPIOV2_SERVICES     *GpioServices,
  IN  GPIOV2_REGISTER     Register,
  IN  GPIOV2_PAD          GpioPad,
  OUT UINT32              *RegisterOffset
  )
{
  GPIOV2_INTERFACE   *GpioInterface;
  UINT32             CommunityIndex;
  UINT32             GroupIndex;
  UINT32             PadIndex;

  if ((GpioServices == NULL) || RegisterOffset == NULL)  {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  *RegisterOffset = 0;

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  switch (Register) {
    case GpioV2PadOwnReg:
      if (GpioInterface->Private.GpioV2PadOwnRegRev == GpioV2PadOwnRegRev1) {
        *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.PadOwn + (PadIndex * 0x04);
      } else {
        *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.PadOwn + (PadIndex / 8) * 0x04;
      }
      break;
    case GpioV2PadCfgLockReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.PadCfgLock;
      break;
    case GpioV2PadCfgLockTxReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.PadCfgLockTx;
      break;
    case GpioV2PadHostSwOwnReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.HostOwn;
      break;
    case GpioV2GpiIsReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.GpiIs;
      break;
    case GpioV2GpiIeReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.GpiIe;
      break;
    case GpioV2GpiGpeStsReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.GpiGpeSts;
      break;
    case GpioV2GpiGpeEnReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.GpiGpeEn;
      break;
    case GpioV2SmiStsReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.SmiSts;
      break;
    case GpioV2SmiEnReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.SmiEn;
      break;
    case GpioV2NmiStsReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.NmiSts;
      break;
    case GpioV2NmiEnReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.NmiEn;
      break;
    case GpioV2Dw0Reg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.Dw0 + PadIndex * 0x10;
      break;
    case GpioV2Dw1Reg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.Dw0 + 0x04 + PadIndex * 0x10;
      break;
    case GpioV2Dw2Reg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].RegisterOffsets.Dw0 + 0x08 + PadIndex * 0x10;
      break;
    case GpioV2MiscCfg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].RegisterOffsets.MiscCfg;
      break;
    case GpioV2AcReg:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].RegisterOffsets.AcSaiGrup0RcpDw0;
      break;
    case GpioV2Pwmc:
      *RegisterOffset = GpioInterface->Private.Communities[CommunityIndex].RegisterOffsets.Pwmc;
      break;
    default:
      return EFI_INVALID_PARAMETER;
  }

  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO mode

  @param[in] GPIOV2_SERVICES      Gpio Services
  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] PadModeValue         GPIO pad mode value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetPadMode (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN GPIOV2_PAD_MODE         PadMode
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  GPIOV2_PAD_OWN    Ownership;
  UINT32  AndValue;
  UINT32  OrValue;
  UINT32  CommunityIndex;
  UINT32  RegisterOffset;
  BOOLEAN PadValid;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  if (PadMode != GpioV2PadModeHardwareDefault) {

    AndValue = (UINT32)~(GPIOV2_PAD_MODE_MASK << GPIOV2_PAD_MODE_DW0_POS);
    OrValue  = ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK) << GPIOV2_PAD_MODE_DW0_POS;

    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
    );

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );

   }

  return EFI_SUCCESS;
}

/**
  This procedure reads current GPIO Pad Mode

  @param[in] GPIOV2_SERVICES     Gpio Services
  @param[in] GpioPad             GPIO Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] PadMode             Pointer to a buffer for GPIO Pad Mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetPadMode (
  IN GPIOV2_SERVICES       *GpioServices,
  IN GPIOV2_PAD            GpioPad,
  IN GPIOV2_PAD_MODE       *PadMode
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  GPIOV2_PAD_OWN    Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (PadMode == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  *PadMode = GPIO_ASSIGN_VALUE (((RegisterValue >> GPIOV2_PAD_MODE_DW0_POS) & GPIOV2_PAD_MODE_MASK));

  return EFI_SUCCESS;
}


/**
  This procedure will set GPIO enable or disable input inversion on rquested pad

  @param[in] GPIOV2_SERVICES   Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] InputInversion      GpioV2InputInversionEnable or GpioV2InputInversionDisable, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetInputInversion (
  IN GPIOV2_SERVICES             *GpioServices,
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_INPUT_INVERSION  InputInversion
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  GPIOV2_PAD_OWN    Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  if (InputInversion != GpioV2InputInversionHardwareDefault) {

    AndValue = (UINT32)~(GPIOV2_PAD_INPUT_INVERSION_MASK << GPIOV2_PAD_INPUT_INVERSION_DW0_POS);
    OrValue  = ((InputInversion >> 1) & GPIOV2_PAD_INPUT_INVERSION_MASK) << GPIOV2_PAD_INPUT_INVERSION_DW0_POS;

    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
    );

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure will Get GPIO enable or disable input inversion on rquested pad

  @param[in]  GPIOV2_SERVICES     Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[Out] Inverted            Buffer for Boolean value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetInputInversion (
  IN GPIOV2_SERVICES             *GpioServices,
  IN GPIOV2_PAD                  GpioPad,
  OUT BOOLEAN                    *Inverted
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32  CommunityIndex;
  UINT32  RegisterOffset;
  BOOLEAN PadValid;
  UINT32  Data;

  if (GpioServices == NULL || Inverted == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
  );

  Data = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
           &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
           RegisterOffset
         );

  if ((Data >> GPIOV2_PAD_INPUT_INVERSION_DW0_POS) & GPIOV2_PAD_INPUT_INVERSION_MASK) {
    *Inverted = TRUE;
  }
  else {
    *Inverted = FALSE;
  }

  return EFI_SUCCESS;
}
/**
  This procedure will set GPIO Lock (register PADCFGLOCK)

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] Lock                GpioV2Unlock - Unlock pad, GpioV2Lock - Lock pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetLock (
  IN GPIOV2_SERVICES           *GpioServices,
  IN GPIOV2_PAD                GpioPad,
  IN GPIOV2_PAD_LOCK           Lock
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 PadIndex;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (Lock != GpioV2LockHardwareDefault) {
    AndValue = ~(GPIOV2_PAD_LOCK_MASK << PadIndex);
    OrValue  = ((Lock >> 1) & GPIOV2_PAD_LOCK_MASK) << PadIndex;

    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2PadCfgLockReg,
      GpioPad,
      &RegisterOffset
    );

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO Lock (register PADCFGLOCK)

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] Lock                Buffer for GPIOV2_PAD_LOCK

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetLock (
  IN  GPIOV2_SERVICES              *GpioServices,
  IN  GPIOV2_PAD                   GpioPad,
  OUT GPIOV2_PAD_LOCK              *Lock
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 PadIndex;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (Lock == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &GpioInterface->Public,
    GpioV2PadCfgLockReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &GpioInterface->Private.CommunityAccess[CommunityIndex].Access,
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & GPIOV2_PAD_LOCK_MASK) {
    *Lock = GpioV2Lock;
  } else {
    *Lock = GpioV2Unlock;
  }

  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO Lock Tx (register PADCFGLOCKTX)

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] LockTx              GpioV2Unlock - Unlock output state of Gpio Pad, GpioV2Lock - Lock output state of Gpio Pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetLockTx (
  IN GPIOV2_SERVICES             *GpioServices,
  IN GPIOV2_PAD                  GpioPad,
  IN GPIOV2_PAD_LOCK             LockTx
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 PadIndex;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (LockTx != GpioV2LockHardwareDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2PadCfgLockTxReg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = ~(GPIOV2_PAD_LOCK_MASK << PadIndex);
    OrValue  = ((LockTx >> 1) & GPIOV2_PAD_LOCK_MASK) << PadIndex;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );

  }
  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO Lock Tx (register PADCFGLOCKTX)

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] Lock                Buffer for GPIOV2_PAD_LOCK

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetLockTx (
  IN GPIOV2_SERVICES              *GpioServices,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_PAD_LOCK              *Lock
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32           PadIndex;
  UINT32           CommunityIndex;
  UINT32           RegisterOffset;
  UINT32           RegisterValue;

  if ((GpioServices == NULL) || (Lock == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2PadCfgLockTxReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & GPIOV2_PAD_LOCK_MASK) {
    *Lock = GpioV2Lock;
  } else {
    *Lock = GpioV2Unlock;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets Gpio Pad output state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         GpioV2StateLow - output state low, GpioV2StateHigh - output state high

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetTx (
  IN GPIOV2_SERVICES        *GpioServices,
  IN GPIOV2_PAD             GpioPad,
  IN GPIOV2_PAD_STATE       OutputState
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  if (OutputState != GpioV2StateDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = (UINT32)~(GPIOV2_PAD_OUTPUT_STATE_MASK << GPIOV2_PAD_OUTPUT_STATE_DW0_POS);
    OrValue  = ((OutputState >> 1) & GPIOV2_PAD_OUTPUT_STATE_MASK) << GPIOV2_PAD_OUTPUT_STATE_DW0_POS;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current Gpio Pad output state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] OutputState         Pointer to a buffer for output state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetTx (
  IN GPIOV2_SERVICES        *GpioServices,
  IN GPIOV2_PAD             GpioPad,
  IN GPIOV2_PAD_STATE       *OutputState
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (OutputState == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> GPIOV2_PAD_OUTPUT_STATE_DW0_POS) & (GPIOV2_PAD_OUTPUT_STATE_MASK)) {
    *OutputState = GpioV2StateHigh;
  } else {
    *OutputState = GpioV2StateLow;
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current Gpio Pad input state

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] InputState          Pointer to a buffer for input state

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetRx (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_PAD_STATE     *InputState
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (InputState == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> GPIOV2_PAD_INPUT_STATE_DW0_POS) & (GPIOV2_PAD_INPUT_STATE_MASK)) {
    *InputState = GpioV2StateHigh;
  } else {
    *InputState = GpioV2StateLow;
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads if TX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetTxDisable (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *TxDisabled
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if (((GpioServices == NULL)) || (TxDisabled == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> GPIOV2_PAD_TX_DISABLE_DW0_POS) & (GPIOV2_PAD_TX_DISABLE_MASK)) {
    *TxDisabled = TRUE;
  } else {
    *TxDisabled = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets TX buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetTxDisable (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  TxDisabled
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_TX_DISABLE_MASK << GPIOV2_PAD_TX_DISABLE_DW0_POS);
  OrValue  = (TxDisabled & GPIOV2_PAD_TX_DISABLE_MASK) << GPIOV2_PAD_TX_DISABLE_DW0_POS;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );


  return EFI_SUCCESS;
}

/**
  This procedure reads if RX buffer for requested Gpio Pad is enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] RxDisabled          Pointer to a buffer for enabled/disabled information

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetRxDisable (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN BOOLEAN                 *RxDisabled
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if (((GpioServices == NULL)) || (RxDisabled == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> GPIOV2_PAD_RX_DISABLE_DW0_POS) & (GPIOV2_PAD_RX_DISABLE_MASK)) {
    *RxDisabled = TRUE;
  } else {
    *RxDisabled = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets Rx buffer for requested Gpio Pad as enabled or disabled

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TxDisabled          True - TX buffer disabled, False - TX buffer enabled

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRxDisable (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  RxDisabled
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_RX_DISABLE_MASK << GPIOV2_PAD_RX_DISABLE_DW0_POS);
  OrValue  = (RxDisabled & GPIOV2_PAD_RX_DISABLE_MASK) << GPIOV2_PAD_RX_DISABLE_DW0_POS;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure sets Reset Configuration for requested Gpio Pad

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] ResetConfig         Reset Configuration - please refer to GpioV2Pad.h (GPIOV2_RESET_CONFIG)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetResetConfig (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_RESET_CONFIG      ResetConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT8  PadRstIndex;
  GPIOV2_RESET_CONFIG *PadRstCfgToGpioResetConfigMap;

  static GPIOV2_RESET_CONFIG  GppPadRstCfgToGpioResetConfigMap[] = {
    GpioV2ResetResume,
    GpioV2ResetHostDeep,
    GpioV2ResetHost,
    GpioV2ResetGlobal
  };
  static GPIOV2_RESET_CONFIG  GpdPadRstCfgToGpioResetConfigMap[] = {
    GpioV2DswReset,
    GpioV2ResetHostDeep,
    GpioV2ResetHost,
    GpioV2ResetResume
  };

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  if (ResetConfig != GpioV2ResetDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
    );

    if (GpioInterface->Private.Communities[CommunityIndex].IsComDsw) {
      PadRstCfgToGpioResetConfigMap = GpdPadRstCfgToGpioResetConfigMap;
    } else {
      PadRstCfgToGpioResetConfigMap = GppPadRstCfgToGpioResetConfigMap;
    }
    for (PadRstIndex = 0; PadRstIndex <= (GPIOV2_PAD_RESET_CONFIG_MASK + 1); PadRstIndex++) {
      if (PadRstIndex == (GPIOV2_PAD_RESET_CONFIG_MASK + 1)) {
        DEBUG ((DEBUG_ERROR, "[%a] ResetConfig = %d has not been found in mapping table!\n", __FUNCTION__, ResetConfig));
        return EFI_NOT_FOUND;
      } else if (PadRstCfgToGpioResetConfigMap[PadRstIndex] == ResetConfig) {
        break;
      }
    }


    AndValue = (UINT32)~(GPIOV2_PAD_RESET_CONFIG_MASK << GPIOV2_PAD_RESET_CONFIG_DW0_POS);
    OrValue  = (PadRstIndex & GPIOV2_PAD_RESET_CONFIG_MASK) << GPIOV2_PAD_RESET_CONFIG_DW0_POS;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current Reset Configuration for requested Gpio Pad

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] ResetConfig         Pointer to a buffer for Reset Configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetResetConfig (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_RESET_CONFIG      *ResetConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;
  UINT8  Padrstcfg;

  static GPIOV2_RESET_CONFIG  GppPadRstCfgToGpioResetConfigMap[] = {
    GpioV2ResetResume,
    GpioV2ResetHostDeep,
    GpioV2ResetHost,
    GpioV2ResetGlobal
  };
  static GPIOV2_RESET_CONFIG  GpdPadRstCfgToGpioResetConfigMap[] = {
    GpioV2DswReset,
    GpioV2ResetHostDeep,
    GpioV2ResetHost,
    GpioV2ResetResume
  };

  if ((GpioServices == NULL) || (ResetConfig == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  // Please refer to enum GPIOV2_RESET_CONFIG in GpioV2Pad.h file
  Padrstcfg = (RegisterValue >> GPIOV2_PAD_RESET_CONFIG_DW0_POS) & GPIOV2_PAD_RESET_CONFIG_MASK;
  if (GpioInterface->Private.Communities[CommunityIndex].IsComDsw) {
    *ResetConfig = GpdPadRstCfgToGpioResetConfigMap[Padrstcfg];
  } else {
    *ResetConfig = GppPadRstCfgToGpioResetConfigMap[Padrstcfg];
  }

  return EFI_SUCCESS;
}


/**
  This procedure sets termination configuration for requested Gpio Pad

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TerminationConfig   Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetTerminationConfig (
  IN GPIOV2_SERVICES            *GpioServices,
  IN GPIOV2_PAD                 GpioPad,
  IN GPIOV2_TERMINATION_CONFIG  TerminationConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  if (TerminationConfig != GpioV2TermDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw1Reg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = (UINT32)~(GPIOV2_PAD_TERMINATION_CONFIG_MASK << GPIOV2_PAD_TERMINATION_CONFIG_DW1_POS);
    OrValue  = ((TerminationConfig >> 1) & GPIOV2_PAD_TERMINATION_CONFIG_MASK) << GPIOV2_PAD_TERMINATION_CONFIG_DW1_POS;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current termination configuration for requested Gpio Pad

  @param[in] GpioServices        Gpio Services
  @param[in] GpioPad             GPIO pad
  @param[in] TerminationConfig   Pointer to a buffer for Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetTerminationConfig (
  IN GPIOV2_SERVICES              *GpioServices,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_TERMINATION_CONFIG    *TerminationConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (TerminationConfig == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw1Reg,
    GpioPad,
    &RegisterOffset
  );


  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  // Please refer to enum GPIOV2_TERMINATION_CONFIG in GpioV2Pad.h file
  *TerminationConfig = (((RegisterValue >> GPIOV2_PAD_TERMINATION_CONFIG_DW1_POS) & GPIOV2_PAD_TERMINATION_CONFIG_MASK) << 1) | 0x01;

  return EFI_SUCCESS;
}

/**
  This procedure sets IO Standby configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetIostandbyConfig (
  IN GPIOV2_SERVICES              *GpioServices,
  IN GPIOV2_PAD                   GpioPad,
  IN GPIOV2_IOSTANDBY_STATE       IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM        IosTerminationConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  //
  // State
  //
  if (IosStateConfig != GpioV2IosStateDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw1Reg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = (UINT32)~(GPIOV2_PAD_IOSTANDBY_STATE_MASK << GPIOV2_PAD_IOSTANDBY_STATE_DW1_POS);
    OrValue  = ((IosStateConfig >> 1) & GPIOV2_PAD_IOSTANDBY_STATE_MASK) << GPIOV2_PAD_IOSTANDBY_STATE_DW1_POS;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  //
  // Termination
  //
  if (IosTerminationConfig != GpioV2IosTermDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw1Reg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = (UINT32)~(GPIOV2_PAD_IOSTANDBY_TERM_MASK << GPIOV2_PAD_IOSTANDBY_TERM_DW1_POS);
    OrValue  = ((IosTerminationConfig >> 1) & GPIOV2_PAD_IOSTANDBY_TERM_MASK) << GPIOV2_PAD_IOSTANDBY_TERM_DW1_POS;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}


/**
  This procedure reads current IO Standby configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] IosStateConfig       Pointer to a buffer for IO Standby State configuration, please refer to GpioV2Pad.h
  @param[in] IosTerminationConfig Pointer to a buffer for IO Standby Termination configuration, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetIostandbyConfig (
  IN GPIOV2_SERVICES            *GpioServices,
  IN GPIOV2_PAD                 GpioPad,
  IN GPIOV2_IOSTANDBY_STATE     *IosStateConfig,
  IN GPIOV2_IOSTANDBY_TERM      *IosTerminationConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (IosStateConfig == NULL) || (IosTerminationConfig == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  //
  // State
  //
  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw1Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  // Please refer to enum GPIOV2_IOSTANDBY_STATE in GpioV2Pad.h file
  *IosStateConfig = (((RegisterValue >> GPIOV2_PAD_IOSTANDBY_STATE_DW1_POS) & GPIOV2_PAD_IOSTANDBY_STATE_MASK) << 1) | 0x01;

  //
  // Termination
  //
  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw1Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  // Please refer to enum GPIOV2_IOSTANDBY_TERM in GpioV2Pad.h file
  *IosTerminationConfig = (((RegisterValue >> GPIOV2_PAD_IOSTANDBY_TERM_DW1_POS) & GPIOV2_PAD_IOSTANDBY_TERM_MASK) << 1) | 0x01;

  return EFI_SUCCESS;
}

//
// For GPIO debounce feature glitch filter clock is used
// which in CNL is driven by RTC clock with f = 32kHz (T = 31.25us)
//
#define GPIOV2_DEB_CLK_PERIOD_IN_NS  31250

/**
  This procedure set debounce configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] DebounceTime         Debounce time in microseconds

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetDebounce (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN UINT32                   DebounceTime
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32  AndValue;
  UINT32  OrValue;
  UINT32  CommunityIndex;
  UINT32  RegisterOffset;
  UINT32  DebounceEnable;
  UINT32  DebounceValue;
  UINT32  InRangeDebounceTime;
  UINT32  SupportedDebounceTime;
  UINT32  Temp;
  BOOLEAN SupportedValue;
  CHAR8   PadName[12];

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  if (DebounceTime > 1024000) {
    InRangeDebounceTime = 1024000;
    SupportedValue = FALSE;
  } else if ((DebounceTime < 250) && (DebounceTime > 0)) {
    InRangeDebounceTime = 0;
    SupportedValue = FALSE;
  } else {
    InRangeDebounceTime = DebounceTime;
    SupportedValue = TRUE;
  }

  //
  // DebounceValue = log2 (InRangeDebounceTime * f_deb_clk)
  //
  DebounceValue = 0;
  Temp = InRangeDebounceTime * 1000 / GPIOV2_DEB_CLK_PERIOD_IN_NS;

  //
  // Check if any rounding occurred
  //
  if (InRangeDebounceTime != (Temp * GPIOV2_DEB_CLK_PERIOD_IN_NS / 1000)) {
    SupportedValue = FALSE;
  }

  //
  // Check if value is power of 2
  //
  if ((Temp != 0) && ((Temp & (Temp - 1)) != 0)) {
    SupportedValue = FALSE;
  }

  //
  // DebounceValue = log2 (Temp)
  //
  while (Temp > 1) {
    Temp >>= 1;
    DebounceValue++;
  }

  if (DebounceValue > 0) {
    DebounceEnable = (1 & GPIOV2_PAD_DEBOUNCE_ENABLE_MASK) << GPIOV2_PAD_DEBOUNCE_ENABLE_DW2_POS;
    SupportedDebounceTime = (1 << DebounceValue) * GPIOV2_DEB_CLK_PERIOD_IN_NS / 1000;
  } else {
    DebounceEnable = 0;
    SupportedDebounceTime = 0;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw2Reg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~((GPIOV2_PAD_DEBOUNCE_ENABLE_MASK << GPIOV2_PAD_DEBOUNCE_ENABLE_DW2_POS) |\
                       (GPIOV2_PAD_DEBOUNCE_TIMER_MASK << GPIOV2_PAD_DEBOUNCE_TIMER_DW2_POS));
  OrValue  = ((DebounceValue & GPIOV2_PAD_DEBOUNCE_TIMER_MASK) << GPIOV2_PAD_DEBOUNCE_TIMER_DW2_POS) |\
             ((DebounceEnable & GPIOV2_PAD_DEBOUNCE_ENABLE_MASK) << GPIOV2_PAD_DEBOUNCE_ENABLE_DW2_POS);

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  if (!SupportedValue) {
    GpioInterface->Public.GetPadName (&(GpioInterface->Public), GpioPad, sizeof(PadName), PadName);
    DEBUG ((DEBUG_WARN, "GPIO WARNING: %a %dus debounce time rounded down to %dus\n",
            PadName,
            DebounceTime,
            SupportedDebounceTime));
  }
  return EFI_SUCCESS;
}

/**
  This procedure reads current ownership configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetOwnership (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN  GPIOV2_PAD              GpioPad,
  OUT GPIOV2_PAD_OWN          *Ownership
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32  CommunityIndex;
  UINT32  RegisterOffset;
  UINT32  RegisterValue;
  UINT32  PadOffset;
  CHAR8   PadName[GPIOV2_NAME_LENGTH_MAX];

  if ((GpioServices == NULL) || (Ownership == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2PadOwnReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if (GpioInterface->Private.GpioV2PadOwnRegRev == GpioV2PadOwnRegRev1) {
    *Ownership = ((RegisterValue & GPIOV2_PAD_OWNERSHIP_MASK_REV1) << 1) | 0x01;
  } else {
    PadOffset = (GPIOV2_PAD_GET_PAD_INDEX(GpioPad) % 8) * 4;
    *Ownership = (((RegisterValue >> PadOffset) & GPIOV2_PAD_OWNERSHIP_MASK) << 1) | 0x01;
  }
  if (*Ownership != GpioV2PadOwnHost) {
     GpioInterface->Public.GetPadName (&GpioInterface->Public, GpioPad, sizeof (PadName), PadName);
     DEBUG ((DEBUG_ERROR, "%a is not owned by HOST\n", PadName));
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets host ownership configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Host ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetHostOwnership (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN GPIOV2_HOSTSW_OWN        HostOnwership
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (HostOnwership != GpioV2HostOwnDefault) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2PadHostSwOwnReg,
      GpioPad,
      &RegisterOffset
    );

    AndValue = (UINT32)~(GPIOV2_PAD_HOST_OWNERSHIP_MASK << PadIndex);
    OrValue  = ((HostOnwership >> 1) & GPIOV2_PAD_HOST_OWNERSHIP_MASK) << PadIndex;

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current host ownership configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] Ownership            Pointer to a buffer for ownership, please refer to GpioV2Pad.h

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetHostOwnership (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN  GPIOV2_PAD              GpioPad,
  OUT GPIOV2_HOSTSW_OWN       *HostOwnership
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;


  if ((GpioServices == NULL) || (HostOwnership == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2PadHostSwOwnReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  // Please refer to enum GPIOV2_HOSTSW_OWN in GpioV2Pad.h file
  *HostOwnership = (((RegisterValue >> PadIndex) & GPIOV2_PAD_HOST_OWNERSHIP_MASK) << 1) | 0x01;

  return EFI_SUCCESS;
}

/**
  This procedure reads current Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                Pointer to a buffer for GPI Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetGpiIe (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN BOOLEAN                 *GpiIe
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (GpiIe == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiIeReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_GPI_IE_MASK)) {
    *GpiIe = TRUE;
  } else {
    *GpiIe = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets Gpi Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                GPI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetGpiIe (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  GpiIe
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiIeReg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_GPI_IE_MASK << PadIndex);
  OrValue  = (GpiIe & GPIOV2_PAD_GPI_IE_MASK) << PadIndex;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure reads current Gpi Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                Pointer to a buffer for GPI Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetGpiIs (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *GpiIs
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (GpiIs == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiIsReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_GPI_IE_MASK)) {
    *GpiIs = TRUE;
  } else {
    *GpiIs = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetGpiGpeEn (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN BOOLEAN                 GpiGpeEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiGpeEnReg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_GPI_GPE_EN_MASK << PadIndex);
  OrValue  = (GpiGpeEn & GPIOV2_PAD_GPI_GPE_EN_MASK) << PadIndex;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure reads current GPI GPE Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiIe                GPI GPE Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetGpiGpeEn (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN BOOLEAN                 *GpiGpeEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (GpiGpeEn == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiGpeEnReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_GPI_GPE_EN_MASK)) {
    *GpiGpeEn = TRUE;
  } else {
    *GpiGpeEn = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current GPI GPE Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] GpiGpeSts            Pointer to a buffer for GPI GPE Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetGpiGpeSts (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *GpiGpeSts
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (GpiGpeSts == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2GpiGpeStsReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_GPI_GPE_STS_MASK)) {
    *GpiGpeSts = TRUE;
  } else {
    *GpiGpeSts = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets NMI Enable for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] NmiEn                NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetNmiEn (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  NmiEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2NmiEnReg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_NMI_EN_MASK << PadIndex);
  if (NmiEn) {
    OrValue = 0x01 << PadIndex;
  } else {
    OrValue = 0x0;
  }

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure reads current NMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] NmiEn                Pointer to a buffer for NMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetNmiEn (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *NmiEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (NmiEn == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2NmiEnReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_NMI_EN_MASK)) {
    *NmiEn = TRUE;
  } else {
    *NmiEn = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current NMI Status configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] NmiSts               Pointer to a buffer for NMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetNmiSts (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *NmiSts
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (NmiSts == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2NmiStsReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_NMI_STS_MASK)) {
    *NmiSts = TRUE;
  } else {
    *NmiSts = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure sets SMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] SmiEn                SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetSmiEn (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  SmiEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2SmiEnReg,
    GpioPad,
    &RegisterOffset
  );

  AndValue = (UINT32)~(GPIOV2_PAD_SMI_EN_MASK << PadIndex);
  OrValue  = (SmiEn & GPIOV2_PAD_SMI_EN_MASK) << PadIndex;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure reads current SMI Enable configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] SmiEn               Pointer to a buffer for SMI Enable, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetSmiEn (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_PAD               GpioPad,
  IN BOOLEAN                  *SmiEn
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (SmiEn == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2SmiEnReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_SMI_EN_MASK)) {
    *SmiEn = TRUE;
  } else {
    *SmiEn = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure reads current SMI Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] SmiSts              Pointer to a buffer for SMI Status, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetSmiSts (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN BOOLEAN                 *SmiSts
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if ((GpioServices == NULL) || (SmiSts == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2SmiStsReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  if ((RegisterValue >> PadIndex) & (GPIOV2_PAD_SMI_STS_MASK)) {
    *SmiSts = TRUE;
  } else {
    *SmiSts = FALSE;
  }

  return EFI_SUCCESS;
}

/**
  This procedure clears current SMI Status for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
ClearSmiSts (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD               GpioPad
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 PadIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;
  BOOLEAN PadValid;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2SmiStsReg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  RegisterValue &= GPIOV2_PAD_SMI_STS_MASK << PadIndex;

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    RegisterValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure sets RxEv configuration for requested Gpio Pad

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[in] RxEvCfg              RxEv configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRxEvCfg (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_RXEVCFG       RxEvCfg
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  if (RxEvCfg != GpioV2IntRxEvCfgDefault) {

    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

    AndValue = (UINT32)~(GPIOV2_PAD_RXEV_MASK << GPIOV2_PAD_RXEV_DW0_POS);
    OrValue  = ((RxEvCfg >> 1) & GPIOV2_PAD_RXEV_MASK) << GPIOV2_PAD_RXEV_DW0_POS;

    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Dw0Reg,
      GpioPad,
      &RegisterOffset
    );

    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      AndValue,
      OrValue
    );
   }

  return EFI_SUCCESS;
}

/**
  This procedure gets RxEv configuration for requested Gpio Pad

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] RxEvCfg             Buffer for RxEvLvl configuration

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetRxEvCfg (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN GPIOV2_RXEVCFG       *RxEvCfg
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;
  UINT32 RegisterValue;

  if (GpioServices == NULL || RxEvCfg == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&GpioInterface->Public, GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.GetRegisterOffset (
    &GpioInterface->Public,
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &GpioInterface->Private.CommunityAccess[CommunityIndex].Access,
    RegisterOffset
  );

  *RxEvCfg = GPIO_ASSIGN_VALUE ((RegisterValue >> GPIOV2_PAD_RXEV_DW0_POS) & GPIOV2_PAD_RXEV_MASK);

  return EFI_SUCCESS;
}


/**
  This procedure sets GPIROUTNMI bit (17th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         GPIO Controller
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 17 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRouteNmi (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  AndValue = (UINT32)~(GPIOV2_PAD_GPIROUTNMI_DW0_MASK << GPIOV2_PAD_GPIROUTNMI_DW0_POS);
  if (Enable) {
    OrValue = 0x1 << GPIOV2_PAD_GPIROUTNMI_DW0_POS;
  } else {
    OrValue = 0x0;
  }

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure sets GPIROUTSMI bit (18th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         GPIO Controller
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 18 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRouteSmi (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  AndValue = (UINT32)~(GPIOV2_PAD_GPIROUTSMI_DW0_MASK << GPIOV2_PAD_GPIROUTSMI_DW0_POS);
  if (Enable) {
    OrValue = 0x1 << GPIOV2_PAD_GPIROUTSMI_DW0_POS;
  } else {
    OrValue = 0x0;
  }

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure sets GPIROUTSCI bit (19th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         GPIO Controller
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 19 bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRouteSci (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  AndValue = (UINT32)~(GPIOV2_PAD_GPIROUTSCI_DW0_MASK << GPIOV2_PAD_GPIROUTSCI_DW0_POS);
  if (Enable) {
    OrValue = 0x1 << GPIOV2_PAD_GPIROUTSCI_DW0_POS;
  } else {
    OrValue = 0x0;
  }

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure sets GPIROUTIOXAPIC bit (20th bit in DW0) for requested Gpio Pad

  @param[in] GpioServices         GPIO Controller
  @param[in] GpioPad              GPIO pad
  @param[in] Enable               TRUE or FALSE, either enable or disable 20th bit in DW0

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
SetRouteIoxApic (
  IN GPIOV2_SERVICES      *GpioServices,
  IN GPIOV2_PAD           GpioPad,
  IN BOOLEAN              Enable
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  UINT32 AndValue;
  UINT32 OrValue;
  UINT32 CommunityIndex;
  UINT32 RegisterOffset;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  AndValue = (UINT32)~(GPIOV2_PAD_GPIROUTIOXAPIC_DW0_MASK << GPIOV2_PAD_GPIROUTIOXAPIC_DW0_POS);
  if (Enable) {
    OrValue = 0x1 << GPIOV2_PAD_GPIROUTIOXAPIC_DW0_POS;
  } else {
    OrValue = 0x0;
  }

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AndValue,
    OrValue
  );

  return EFI_SUCCESS;
}

/**
  This procedure verifies if requested GpioGroup definition is ment for platform that it is used on

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioGroup           GPIO pad
  @param[out] IsValid             Pointer to a buffer for validation information, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
IsGroupValid (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN  GPIOV2_PAD_GROUP        GpioGroup,
  OUT BOOLEAN                 *IsValid
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32  ChipsetId;
  UINT32  CommunityIndex;
  UINT32  GroupIndex;

  if ((GpioServices == NULL) || (IsValid == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  ChipsetId = GPIOV2_PAD_GET_CHIPSETID (GpioGroup);
  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup);

  *IsValid = FALSE;

  if (ChipsetId != GpioInterface->Private.ChipsetId) {
    DEBUG((DEBUG_ERROR, "[GPIOV2][IsGroupValid] Error: wrong ChipsetId; com:%d,grp:%d id: 0x%x (exp: 0x%x)\n", CommunityIndex, GroupIndex, ChipsetId, GpioInterface->Private.ChipsetId));
    ASSERT(FALSE);
    return EFI_SUCCESS;
  }

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    DEBUG((DEBUG_ERROR, "[GPIOV2][IsGroupValid] Error: wrong community index; com:%d (max: %d)\n", CommunityIndex, GpioInterface->Private.CommunitiesNum));
    ASSERT(FALSE);
    return EFI_SUCCESS;
  }

  if (GroupIndex >= GpioInterface->Private.Communities[CommunityIndex].GroupsNum) {
    DEBUG((DEBUG_ERROR, "[GPIOV2][IsGroupValid] Error: wrong group index; g:%d (max: %d)\n", GroupIndex, GpioInterface->Private.Communities[CommunityIndex].GroupsNum));
    ASSERT(FALSE);
    return EFI_SUCCESS;
  }

  *IsValid = TRUE;

  return EFI_SUCCESS;
}

/**
  This procedure verifies if requested GpioPad definition is ment for platform that it is used on

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              GPIO pad
  @param[out] IsValid             Pointer to a buffer for validation information, TRUE or FALSE

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
IsPadValid (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN  GPIOV2_PAD              GpioPad,
  OUT BOOLEAN                 *IsValid
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32  CommunityIndex;
  UINT32  GroupIndex;
  UINT32  PadIndex;

  if ((GpioServices == NULL) || (IsValid == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioServices->IsGroupValid (GpioServices, (GPIOV2_PAD_GROUP)GpioPad, IsValid);
  if (*IsValid != TRUE) {
    ASSERT(FALSE);
    return EFI_SUCCESS;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (PadIndex >= GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum) {
    DEBUG((DEBUG_WARN, "[GPIOV2][IsPadValid] Error: Pad 0x%x, wrong pad index\n", GpioPad));
    *IsValid = FALSE;
    ASSERT(FALSE);
    return EFI_SUCCESS;
  }

  return EFI_SUCCESS;
}

/**
  This function allows user to control read and write access
  to gpio registers for requested SAI policy.

  To properly configure Access Control to a certain group of gpio registers
  user has to modify three registers:
  1. CP (ControlPolicy) - this register controls access to RRAC and RWAC registers
  2. RRAC (Read) - this register controls read access to gpio registers
  3. RWAC (Write) - this register controls write access to gpio registers

  @param[in] GpioServices                Gpio Services
  @param[in] CommunityIndex              Index of community
  @param[in] SaiGroupIndex               Index of Group in Access Control SAI Groups
                                         There are several groups which gpio registers are assigned to
  @param[in] SaiGroupDwIndex             Index of DW in Access Control Group
                                         For each of the above mentioned groups there are several DWs
  @param[in] AccessControlData           Bitmask with SAI permissions for policy

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
SetAccessControl (
  IN GPIOV2_SERVICES                    *GpioServices,
  IN UINT32                             CommunityIndex,
  IN UINT32                             SaiGroupIndex,
  IN GPIOV2_ACCESS_CONTROL_SAI_DW_DATA  *AccessControlDwData
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32            RegisterOffset;
  UINT32            GpioPad;

  if ((GpioServices == NULL) || (AccessControlDwData == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioPad = GPIOV2_PAD_SET_COMMUNITY_INDEX (0, CommunityIndex);

  // Get address of Dw0 of Sai Group 0
  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2AcReg,
    GpioPad,
    &RegisterOffset
  );

  //
  // Calculate offset of DW0 of Sai Group 'SaiGroupIndex'
  // All groups has got 3 registers (CP, RAC, WAC).
  // Each register in a SAI Group is 64 bits long (2 x 0x04 bytes - DW0 and DW1)
  // Each SAI Group is 3 * (2 * 4) = 24 = 0x18 bytes long
  //
  RegisterOffset += SaiGroupIndex * 0x18;

  //
  // Policy - RRAC register
  //
  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset + 0x08,
    AccessControlDwData->Read
  );

  //
  // Policy - RWAC register
  //
  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset + 0x10,
    AccessControlDwData->Write
  );

  //
  // Policy - CP register
  //
  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    AccessControlDwData->Policy
  );

  return EFI_SUCCESS;
}

/**
  This function allows user to get read and write access settings
  to gpio registers for requested SAI policy.

  To properly configure Access Control to a certain group of gpio registers
  user has to modify three registers:
  1. CP (ControlPolicy) - this register controls access to RRAC and RWAC registers
  2. RRAC (Read) - this register controls read access to gpio registers
  3. RWAC (Write) - this register controls write access to gpio registers

  @param[in] GpioServices                Gpio Services
  @param[in] CommunityIndex              Index of community
  @param[in] SaiGroupIndex               Index of Group in Access Control SAI Groups
                                         There are several groups which gpio registers are assigned to
  @param[in] SaiGroupDwIndex             Index of DW in Access Control Group
                                         For each of the above mentioned groups there are several DWs
  @param[in] AccessControlData           Bitmask with SAI group permissions for policy

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
GetAccessControl (
  IN GPIOV2_SERVICES                    *GpioServices,
  IN UINT32                             CommunityIndex,
  IN UINT32                             SaiGroupIndex,
  IN GPIOV2_ACCESS_CONTROL_SAI_DW_DATA  *AccessControlDwData
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32           RegisterOffset;
  UINT32           RegisterValue;
  UINT32           GpioPad;

  if ((GpioServices == NULL) || (AccessControlDwData == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioPad = GPIOV2_PAD_SET_COMMUNITY_INDEX (0, CommunityIndex);

  // Get address of Dw0 of Sai Group 0
  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2AcReg,
    GpioPad,
    &RegisterOffset
  );

  //
  // Calculate offset of DW0 of Sai Group 'SaiGroupIndex'
  // All groups has got 3 registers (CP, RAC, WAC).
  // Each register in a SAI Group is 64 bits long (2 x 0x04 bytes - DW0 and DW1)
  // Each SAI Group is 3 * (2 * 4) = 24 = 0x18 bytes long
  //
  RegisterOffset += SaiGroupIndex * 0x18;

  //
  // Read - RRAC register
  //
  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset + 0x08
  );

  AccessControlDwData->Read = RegisterValue;

  // Write - RWAC register
  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset + 0x10
  );

  AccessControlDwData->Write = RegisterValue;

  //
  // Policy - CP register
  //
  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  AccessControlDwData->Policy = RegisterValue;

  return EFI_SUCCESS;
}

/**
  This procedure set Lock and LockTx registers as enabled for all GpioPads

  @param[in] GpioServices         Gpio Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
LockAll (
  IN  GPIOV2_SERVICES       *GpioServices
  )
{
  GPIOV2_INTERFACE                    *GpioInterface;
  UINT32                              CommunityIndex;
  UINT32                              GroupIndex;
  UINT32                              PadIndex;
  GPIOV2_PAD_OWN                      Ownership;
  UINT32                              GlobalGroupIndex;
  GPIOV2_PAD                          Pad;
  GPIOV2_PAD_GROUP                    Group;
  CHAR8                               PadName[GPIOV2_NAME_LENGTH_MAX];
  GPIOV2_ACCESS_CONTROL_SAI_DW_DATA   AcData;


  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  DEBUG ((DEBUG_INFO, "[GPIOV2][LockAll][%a]: START\n", GpioInterface->Public.Hid));

  if (GpioInterface->Private.UnlockData == NULL) {
    return EFI_UNSUPPORTED;
  }

  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    for (GroupIndex = 0; GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum; GroupIndex++) {

      Group = GPIOV2_PAD_ID(0, GpioInterface->Private.ChipsetId, 0, CommunityIndex, GroupIndex, 0xFF);
      GpioInterface->Public.GetGlobalGroupIndex (
                              &(GpioInterface->Public),
                              Group,
                              &GlobalGroupIndex
                              );

      for (PadIndex = 0; PadIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum; PadIndex++) {
        Pad = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex];
        GpioInterface->Public.GetOwnership (&(GpioInterface->Public), Pad, &Ownership);
        if (Ownership == GpioV2PadOwnHost) {
          if (GpioInterface->Private.UnlockAll) {
            GpioInterface->Public.SetLock (&(GpioInterface->Public), Pad, GpioV2Unlock);
            GpioInterface->Public.SetLockTx (&(GpioInterface->Public), Pad, GpioV2Unlock);
            continue;
          }
          if (!(GpioInterface->Private.UnlockData[GlobalGroupIndex].UnlockConfigBitMask & (0x1 << PadIndex))) {
            GpioInterface->Public.SetLock (&(GpioInterface->Public), Pad, GpioV2Lock);
          } else {
            GpioInterface->Public.GetPadName (&GpioInterface->Public, Pad, sizeof (PadName), PadName);
            DEBUG ((DEBUG_INFO, "%a leaving config unlocked\n", PadName));
          }
          if (!(GpioInterface->Private.UnlockData[GlobalGroupIndex].UnlockTxBitMask & (0x1 << PadIndex))) {
            GpioInterface->Public.SetLockTx (&(GpioInterface->Public), Pad, GpioV2Lock);
          } else {
            GpioInterface->Public.GetPadName (&GpioInterface->Public, Pad, sizeof (PadName), PadName);
            DEBUG ((DEBUG_INFO, "%a leaving Tx unlocked\n", PadName));
          }
        }
      }
    }
  }

  //
  // After properly setting gpio lock registers remove
  // access to lock registers after SetBiosDone flag is set
  //
  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    GetAccessControl (&(GpioInterface->Public), CommunityIndex, GpioInterface->Private.PadCfgLockCntrlGrpIndex, &AcData);
    AcData.Policy &= ~(B_SAI_HOSTIA_POSTBOOT);
    AcData.Write  &= ~(B_SAI_HOSTIA_POSTBOOT);
    SetAccessControl (&(GpioInterface->Public), CommunityIndex, GpioInterface->Private.PadCfgLockCntrlGrpIndex, &AcData);
  }

  DEBUG ((DEBUG_INFO, "[GPIOV2][LockAll]: END\n"));

  return EFI_SUCCESS;
}


/**
  Store unlock data.

  @param[in] GpioInterface  GPIO interface
  @param[in] GpioPad        GPIO pad
  @param[in] LockCfg        Pad config lock policy
  @param[in] LockTx         Pad Tx lock policy
**/
VOID
GpioV2StoreUnlockData (
  IN GPIOV2_INTERFACE  *GpioInterface,
  IN GPIOV2_PAD        GpioPad,
  IN GPIOV2_PAD_LOCK   LockCfg,
  IN GPIOV2_PAD_LOCK   LockTx
  )
{
  EFI_STATUS  Status;
  UINT32      PadIndex;
  UINT32      GlobalGroupIndex;
  CHAR8       PadName[GPIOV2_NAME_LENGTH_MAX];

  if (GpioInterface == NULL) {
    return;
  }

  if (GpioInterface->Private.UnlockData == NULL) {
    return;
  }

  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  Status = GpioInterface->Public.GetGlobalGroupIndex (&(GpioInterface->Public), (GPIOV2_PAD_GROUP)GpioPad, &GlobalGroupIndex);
  if (EFI_ERROR (Status)) {
    return;
  }

  GpioInterface->Public.GetPadName (&GpioInterface->Public, GpioPad, sizeof (PadName), PadName);
  DEBUG ((DEBUG_INFO, "[GPIOV2] [UNLOCK] [SELECT] %a ", PadName));
  if (LockCfg == GpioV2Unlock) {
    DEBUG ((DEBUG_INFO, "CFG"));
    GpioInterface->Private.UnlockData[GlobalGroupIndex].UnlockConfigBitMask |= (0x1 << PadIndex);
  }

  if (LockTx == GpioV2Unlock) {
    DEBUG ((DEBUG_INFO, " TX"));
    GpioInterface->Private.UnlockData[GlobalGroupIndex].UnlockTxBitMask |= (0x1 << PadIndex);
  }
  DEBUG ((DEBUG_INFO, "\n"));
}

/**
  This procedure will set GPIO mode for HDA SNDW functionality

  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_INVALID_PARAMETER    Invalid group or pad number
  @retval EFI_ACCESS_DENIED        Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
ConfigurePad (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN GPIOV2_CONFIG           *PadConfig
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  GPIOV2_PAD_OWN   Ownership;
  EFI_STATUS       Status = EFI_SUCCESS;
  BOOLEAN          IsPadValid = TRUE;
  CHAR8            PadName[GPIOV2_NAME_LENGTH_MAX];

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);
  Status = GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &IsPadValid);
  if (!IsPadValid) {
    DEBUG ((DEBUG_WARN, "[GPIOV2] %a PadIsNot Valid\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  GpioInterface->Public.GetPadName (GpioServices, GpioPad, sizeof (PadName), PadName);

  GpioInterface->Public.SetPadMode (&(GpioInterface->Public), GpioPad, PadConfig->PadMode);

  if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntSmi >> 1)) {
    GpioInterface->Public.SetHostOwnership (&(GpioInterface->Public), GpioPad, GpioV2HostOwnAcpi);
  } else {
    GpioInterface->Public.SetHostOwnership (&(GpioInterface->Public), GpioPad, PadConfig->HostOwn);
  }

  //
  // Make sure we set a Tx state before setting a direction to avoid glitches on the GPIO line.
  // A glitch can happen if by default Tx of the GPIO pad is disabled and Tx state is set to drive
  // the pad low but signal on the board is pulled up. When BIOS wants to take control over the GPIO pad
  // and enable the Tx and set the Tx state to high to match the platform default signal state we need to make
  // sure tha Tx is programed to high before we enable Tx otherwise signal on the board will experience a
  // very quick glitch which can lead to a number of problems.
  //
  GpioInterface->Public.SetTx (&(GpioInterface->Public), GpioPad, PadConfig->OutputState);

  if (PadConfig->Direction != GpioV2DirDefault) {
    switch (PadConfig->Direction) {
      case GpioV2DirInOut:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionDisable);
        break;
      case GpioV2DirInInvOut:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionEnable);
        break;
      case GpioV2DirIn:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, TRUE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionDisable);
        break;
      case GpioV2DirInInv:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, TRUE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionEnable);
        break;
      case GpioV2DirOut:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, FALSE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, TRUE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionDisable);
        break;
      case GpioV2DirNone:
        GpioInterface->Public.SetTxDisable (&(GpioInterface->Public), GpioPad, TRUE);
        GpioInterface->Public.SetRxDisable (&(GpioInterface->Public), GpioPad, TRUE);
        GpioInterface->Public.SetInputInversion (&(GpioInterface->Public), GpioPad, GpioV2InputInversionDisable);
        break;
      case GpioV2DirDefault:
      default:
        break;
    }
  }

  if (PadConfig->InterruptConfig != GpioV2IntDefault) {

    //
    // All Interrupt/wake events turned off by default
    //
    GpioInterface->Public.SetNmiEn (&(GpioInterface->Public), GpioPad, FALSE);
    GpioInterface->Public.SetSmiEn (&(GpioInterface->Public), GpioPad, FALSE);
    GpioInterface->Public.SetGpiGpeEn (&(GpioInterface->Public), GpioPad, FALSE);
    GpioInterface->Public.SetGpiIe (&(GpioInterface->Public), GpioPad, FALSE);

    if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntNmi >> 1)) {
      GpioInterface->Public.SetNmiEn (&(GpioInterface->Public), GpioPad, TRUE);
      GpioInterface->Public.SetRouteNmi (&(GpioInterface->Public), GpioPad, TRUE);
    }

    if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntSmi >> 1)) {
      GpioInterface->Public.SetSmiEn (&(GpioInterface->Public), GpioPad, TRUE);
      GpioInterface->Public.SetRouteSmi (&(GpioInterface->Public), GpioPad, TRUE);
    }

    if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntSci >> 1)) {
      GpioInterface->Public.SetGpiGpeEn (&(GpioInterface->Public), GpioPad, TRUE);
      GpioInterface->Public.SetRouteSci (&(GpioInterface->Public), GpioPad, TRUE);
    }

    if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntApic >> 1)) {
      GpioInterface->Public.SetRouteIoxApic (&(GpioInterface->Public), GpioPad, TRUE);
    }

    if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntLevel >> 1)) {
      GpioInterface->Public.SetRxEvCfg (&(GpioInterface->Public), GpioPad, GpioV2IntRxEvCfgLevel);

    } else if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntEdge >> 1)) {
      GpioInterface->Public.SetRxEvCfg (&(GpioInterface->Public), GpioPad, GpioV2IntRxEvCfgEdge);

    } else if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntLvlEdgDis >> 1)) {
      GpioInterface->Public.SetRxEvCfg (&(GpioInterface->Public), GpioPad, GpioV2IntRxEvCfgDisable);

    } else if ((PadConfig->InterruptConfig >> 1) & (GpioV2IntBothEdge >> 1)) {
      GpioInterface->Public.SetRxEvCfg (&(GpioInterface->Public), GpioPad, GpioV2IntRxEvCfgLevelEdge);
    }
  }

  GpioInterface->Public.SetResetConfig (&(GpioInterface->Public), GpioPad, PadConfig->ResetConfig);

  GpioInterface->Public.SetTerminationConfig (&(GpioInterface->Public), GpioPad, PadConfig->TerminationConfig);

  //
  // Store unlock data
  //
  if ( ((PadConfig->Direction == GpioV2DirOut) || (PadConfig->Direction == GpioV2DirInOut) ||  (PadConfig->Direction == GpioV2DirInInvOut)) &&
       (PadConfig->PadMode == GpioV2PadModeGpio) &&
       (PadConfig->LockTx == GpioV2LockHardwareDefault) ) {
    GpioV2StoreUnlockData ( GpioInterface,
                          GpioPad,
                           PadConfig->LockConfig,
                           GpioV2Unlock );
  } else if ( (PadConfig->LockConfig == GpioV2Unlock) ||
              (PadConfig->LockTx == GpioV2Unlock) ) {
    GpioV2StoreUnlockData ( GpioInterface,
                          GpioPad,
                          PadConfig->LockConfig,
                          PadConfig->LockTx );
  }

  if (PadConfig->OtherSettings != GpioV2RxRaw1Default) {
    // TO BE DONE
  }

  return Status;
}

/**
  This procedure will get actual GPIO PAD configuration
  @param[in] GpioServices         Gpio Services
  @param[in] GpioPad              Gpio Pad
  @param[in] PadConfig            Gpio Pad configuration - please refer to GPIOV2_CONFIG in GpioV2Config.h

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_INVALID_PARAMETER    Invalid group or pad number
  @retval EFI_ACCESS_DENIED        Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetPadConfiguration (
  IN GPIOV2_SERVICES         *GpioServices,
  IN GPIOV2_PAD              GpioPad,
  IN GPIOV2_CONFIG           *PadConfig
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  GPIOV2_PAD_OWN    Ownership;
  EFI_STATUS        Status;
  BOOLEAN           IsPadValid = TRUE;
  UINT32            Value;

  if ((GpioServices == NULL)  || (PadConfig == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  Status = GpioInterface->Public.IsPadValid (GpioServices, GpioPad, &IsPadValid);
  if (!IsPadValid) {
    DEBUG ((DEBUG_WARN, "[GPIOV2] %a PadIsNot Valid\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  ZeroMem (PadConfig, sizeof (GPIOV2_CONFIG));

  //
  // Pad Mode
  //
  Status = GpioServices->GetPadMode (GpioServices, GpioPad, (GPIOV2_PAD_MODE *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->PadMode = (GPIOV2_PAD_MODE)Value;

  //
  // Host Ownership
  //
  Status = GpioServices->GetHostOwnership (GpioServices, GpioPad, (GPIOV2_HOSTSW_OWN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->HostOwn = (GPIOV2_HOSTSW_OWN)Value;

  //
  // Direction
  //
  Status = GpioServices->GetInputInversion (GpioServices, GpioPad, (BOOLEAN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  if ((BOOLEAN)Value == FALSE) {
    Status = GpioServices->GetTxDisable (GpioServices, GpioPad, (BOOLEAN *)&Value);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    if ((BOOLEAN)Value == TRUE) {
      Status = GpioServices->GetRxDisable (GpioServices, GpioPad, (BOOLEAN *)&Value);
      if (Status != EFI_SUCCESS) {
        return Status;
      }

      if ((BOOLEAN)Value == TRUE) {
        PadConfig->Direction = GpioV2DirNone;
      } else {
        PadConfig->Direction = GpioV2DirIn;
      }
    } else {
      Status = GpioServices->GetRxDisable (GpioServices, GpioPad, (BOOLEAN *)&Value);
      if (Status != EFI_SUCCESS) {
        ASSERT(FALSE);
        return Status;
      }

      if ((BOOLEAN)Value == TRUE) {
        PadConfig->Direction = GpioV2DirOut;
      } else {
        PadConfig->Direction = GpioV2DirInOut;
      }
    }
  } else {
    Status = GpioServices->GetTxDisable (GpioServices, GpioPad, (BOOLEAN *)&Value);
    if (Status != EFI_SUCCESS) {
      return Status;
    }

    if ((BOOLEAN)Value == TRUE) {
      PadConfig->Direction = GpioV2DirInInv;
    } else {
      PadConfig->Direction = GpioV2DirInInvOut;
    }
  }

  //
  // Output State
  //
  Status = GpioServices->GetTx (GpioServices, GpioPad, (GPIOV2_PAD_STATE *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->OutputState = (GPIOV2_PAD_STATE)Value;

  //
  // Interrupt Config
  //
  PadConfig->InterruptConfig = GpioV2IntDis;
  // NMI
  Status = GpioServices->GetNmiEn (GpioServices, GpioPad, (BOOLEAN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if ((BOOLEAN)Value == TRUE) {
    PadConfig->InterruptConfig |= GpioV2IntNmi;
  }

  // SMI
  Status = GpioServices->GetSmiEn (GpioServices, GpioPad, (BOOLEAN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if ((BOOLEAN)Value == TRUE) {
    PadConfig->InterruptConfig |= GpioV2IntSmi;
  }

  // SCI/GPE
  Status = GpioServices->GetGpiGpeEn (GpioServices, GpioPad, (BOOLEAN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if ((BOOLEAN)Value == TRUE) {
    PadConfig->InterruptConfig |= GpioV2IntSci;
  }

  // APIC
  Status = GpioServices->GetGpiIe (GpioServices, GpioPad, (BOOLEAN *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  if ((BOOLEAN)Value == TRUE) {
    PadConfig->InterruptConfig |= GpioV2IntApic;
  }

  // RxEvCfg
  if (PadConfig->InterruptConfig != GpioV2IntDis) {
    Status = GpioServices->GetRxEvCfg (GpioServices, GpioPad, (GPIOV2_RXEVCFG *)&Value);
    if (Status != EFI_SUCCESS) {
      return Status;
    }
    switch (Value) {
      case GpioV2IntRxEvCfgLevel:
        PadConfig->InterruptConfig |= GpioV2IntLevel;
        break;
      case GpioV2IntRxEvCfgEdge:
        PadConfig->InterruptConfig |= GpioV2IntEdge;
        break;
      case GpioV2IntRxEvCfgDisable:
        PadConfig->InterruptConfig |= GpioV2IntLvlEdgDis;
        break;
      case GpioV2IntRxEvCfgLevelEdge:
        PadConfig->InterruptConfig |= GpioV2IntBothEdge;
        break;
      default:
        break;
    }
  }

  //
  // Reset Config
  //
  Status = GpioServices->GetResetConfig (GpioServices, GpioPad, (GPIOV2_RESET_CONFIG *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->ResetConfig = (GPIOV2_RESET_CONFIG)Value;

  //
  // Termination Config
  //
  Status = GpioServices->GetTerminationConfig (GpioServices, GpioPad, (GPIOV2_TERMINATION_CONFIG *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->TerminationConfig = (GPIOV2_TERMINATION_CONFIG)Value;

  //
  // Lock Config
  //
  Status = GpioServices->GetLock (GpioServices, GpioPad, (GPIOV2_PAD_LOCK *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->LockConfig = Value;

  //
  // Lock Tx
  //
  Status = GpioServices->GetLockTx (GpioServices, GpioPad, (GPIOV2_PAD_LOCK *)&Value);
  if (Status != EFI_SUCCESS) {
    return Status;
  }
  PadConfig->LockTx = Value;

  return EFI_SUCCESS;
}

/**
  This procedure retrieves name of requested Gpio Pad

  @param[in] GPIOV2_SERVICES      Gpio Services
  @param[in] GpioPad              Gpio Pad. Please refer to GpioPinsYYY.h - where YYY name of the platform (eg. MTL, EBG, ...)
  @param[in] NameBufferLength     Maximum number of characters to be stored in NameBuffer
  @param[out] NameBuffer          Pointer to a buffer for Gpio Pad name

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetPadName (
  IN  GPIOV2_SERVICES       *GpioServices,
  IN  GPIOV2_PAD            GpioPad,
  IN  UINT32                NameBufferLength,
  OUT CHAR8                 *NameBuffer
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32 PadIndex;
  UINT32 CommunityIndex;
  UINT32 GroupIndex;

  if ((GpioServices == NULL) ||  (NameBuffer == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (GpioPad == GPIOV2_PAD_NONE) {
    AsciiSPrint (NameBuffer, NameBufferLength, "No muxing");
    return EFI_SUCCESS;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioPad);
  PadIndex = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  if (AsciiSPrint (NameBuffer, NameBufferLength, "%a_%02d",
          GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Name,
          PadIndex) == 0) {
            ASSERT(FALSE);
            return EFI_INVALID_PARAMETER;
          }

  return EFI_SUCCESS;
}

/**
  This procedure will initialize all GPIO pads based on PadInitConfigTable

  @param[in] GpioServices           Gpio Services
  @param[in] PadInitConfigTable     Pointer to PadInitConfigTable
  @param[in] PadInitConfigTableSize Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
EFIAPI
ConfigurePads (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
  )
{
  UINT32       Index;
  EFI_STATUS   Status;
  Status = EFI_SUCCESS;

  for (Index = 0; Index < GpioPadsConfigTableSize; Index++) {
    Status = GpioServices->ConfigurePad (
               GpioServices,
               GpioPadsConfigTable[Index].GpioPad,
               &GpioPadsConfigTable[Index].GpioConfig
               );
    if (Status != EFI_SUCCESS && Status != EFI_ACCESS_DENIED) {
      ASSERT(FALSE);
      break;
    }
  }

  return Status;
}

/**
  This procedure gets Smi Gpi Number based on GPIOV2_PAD

  @param[in]  GpioServices           Gpio Services
  @param[in]  GpioPad                Gpio Pad
  @param[out] GpiNum                 Smi Gpi Number

  @retval Status
**/
EFI_STATUS
EFIAPI
GetGpiSmiNumFromPad (
  IN  GPIOV2_SERVICES     *GpioServices,
  IN  GPIOV2_PAD          GpioPad,
  OUT UINTN               *GpiNum
  )
{
  EFI_STATUS                    Status;
  UINT32                        CommunityIndex;
  UINT32                        GroupIndex;
  UINT32                        GlobalGroupIndex;
  GPIOV2_INTERFACE              *GpioInterface;
  BOOLEAN                       PadValid;
  GPIOV2_PAD_OWN                Ownership;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  Status = GpioServices->GetGlobalGroupIndex(GpioServices, (GPIOV2_PAD_GROUP)GpioPad, &GlobalGroupIndex);
  if (Status != EFI_SUCCESS) {
    return EFI_INVALID_PARAMETER;
  }

  *GpiNum = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);
  for (CommunityIndex = 0; (CommunityIndex < GpioInterface->Private.CommunitiesNum) && ( GlobalGroupIndex > 0 ); CommunityIndex++) {
    for (GroupIndex = 0; (GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum) && ( GlobalGroupIndex > 0 ); GroupIndex++) {
      *GpiNum += GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum;
      GlobalGroupIndex--;
    }
  }

  return EFI_SUCCESS;
}

/**
  This procedure gets GPIOV2_PAD based on Smi Gpi Number

  @param[in]   GpioServices           Gpio Services
  @param[in]   GpiNum                 Smi Gpi Number
  @param[out]  GpioPad                Gpio Pad

  @retval Status
**/
EFI_STATUS
EFIAPI
GetPadFromGpiSmiNum (
  IN   GPIOV2_SERVICES   *GpioServices,
  IN   UINTN             GpiNum,
  OUT  GPIOV2_PAD        *GpioPad
  )
{
  GPIOV2_INTERFACE       *GpioInterface;
  UINT32                 ComIdx;
  UINT8                  GrpIdx;
  UINTN                  PadIdx = GpiNum;

  if ((GpioServices == NULL) || (GpioPad == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  for (ComIdx = 0; ComIdx < GpioInterface->Private.CommunitiesNum; ComIdx++) {
    for (GrpIdx = 0; GrpIdx < GpioInterface->Private.Communities[ComIdx].GroupsNum; GrpIdx++) {
      if (PadIdx >= GpioInterface->Private.Communities[ComIdx].Groups[GrpIdx].PadsNum) {
        PadIdx -= GpioInterface->Private.Communities[ComIdx].Groups[GrpIdx].PadsNum;
      } else {
        *GpioPad = GPIOV2_PAD_ID (
                     0,
                     GpioInterface->Private.ChipsetId,
                     0,
                     ComIdx,
                     GrpIdx,
                     PadIdx
                     );
        return EFI_SUCCESS;
      }
    }
  }

  *GpioPad = GPIOV2_PAD_NONE;
  return EFI_NOT_FOUND;
}

/**
  This procedure is used by PchSmiDispatcher and will return information
  needed to register GPI SMI.

  @param[in]  GpioServices            Gpio Services
  @param[in]  Index                   GPI SMI number
  @param[out] GpioPad                 GPIO pad
  @param[out] GpiSmiBitOffset         GPI SMI bit position within GpiSmi Registers
  @param[out] GpiHostSwOwnRegAddress  Address of HOSTSW_OWN register
  @param[out] GpiSmiStsRegAddress     Address of GPI SMI status register

  @retval Status
**/
EFI_STATUS
EFIAPI
GetPadAndSmiRegs (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  UINT32             Index,
  OUT GPIOV2_PAD         *GpioPad,
  OUT UINT8              *GpiSmiBitOffset,
  OUT UINT32             *GpiHostSwOwnRegAddress,
  OUT UINT32             *GpiSmiStsRegAddress
  )
{
  GPIOV2_INTERFACE       *GpioInterface;
  UINT32                 CommunityIndex;
  UINT32                 GpiHostSwOwnRegOffset;
  UINT32                 GpiSmiStsRegOffset;
  EFI_STATUS             Status;

  if ((GpioServices == NULL) ||
      (GpioPad == NULL) ||
      (GpiSmiBitOffset == NULL) ||
      (GpiHostSwOwnRegAddress == NULL) ||
      (GpiSmiStsRegAddress == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  Status = GetPadFromGpiSmiNum (
             GpioServices,
             Index,
             GpioPad
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (*GpioPad);
  *GpiSmiBitOffset = GPIOV2_PAD_GET_PAD_INDEX (*GpioPad);

  // Current SMI code does not support registration with 64bit addresses
  if ((GpioInterface->Private.CommunityRegisterBase[CommunityIndex] >> 32) != 0) {
    return EFI_UNSUPPORTED;
  }

  Status = GetRegisterOffset (
             GpioServices,
             GpioV2PadHostSwOwnReg,
             *GpioPad,
             &GpiHostSwOwnRegOffset
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }
  *GpiHostSwOwnRegAddress = (UINT32) GpioInterface->Private.CommunityRegisterBase[CommunityIndex] + GpiHostSwOwnRegOffset;

  Status = GetRegisterOffset (
             GpioServices,
             GpioV2SmiStsReg,
             *GpioPad,
             &GpiSmiStsRegOffset
             );
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }
  *GpiSmiStsRegAddress = (UINT32) GpioInterface->Private.CommunityRegisterBase[CommunityIndex] + GpiSmiStsRegOffset;

  return EFI_SUCCESS;
}

/**
  This procedure will get GPIO IOxAPIC interrupt number

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] IrqNum              IRQ number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid
  @retval EFI_ACCESS_DENIED       Pad is not owned by Host.
**/
EFI_STATUS
EFIAPI
GetPadIoApicIrqNum (
  IN GPIOV2_SERVICES  *GpioServices,
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *IrqNum
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  GPIOV2_PAD_OWN    Ownership;
  BOOLEAN           PadValid;
  UINT32            CommunityIndex;
  UINT32            RegisterOffset;
  UINT32            RegisterValue;


  if (GpioServices == NULL || IrqNum == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  GpioInterface->Public.GetOwnership (&(GpioInterface->Public), GpioPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw1Reg,
    GpioPad,
    &RegisterOffset
  );

  RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  *IrqNum = (RegisterValue & B_GPIO_PCR_INTSEL) >> N_GPIO_PCR_INTSEL;

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
SetMiscCfg (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           CommunityIndex,
  IN UINT32           MiscCfgValue
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  UINT32            RegisterOffset;
  UINT32            GpioPad;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioPad = GPIOV2_PAD_SET_COMMUNITY_INDEX (0, CommunityIndex);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2MiscCfg,
    GpioPad,
    &RegisterOffset
  );

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    MiscCfgValue
  );

  return EFI_SUCCESS;
}

EFI_STATUS
EFIAPI
GetMiscCfg (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           CommunityIndex,
  IN UINT32           *MiscCfgValue
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  UINT32            RegisterOffset;
  UINT32            GpioPad;

  if ((GpioServices == NULL) || (MiscCfgValue == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioPad = GPIOV2_PAD_SET_COMMUNITY_INDEX (0, CommunityIndex);

  GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2MiscCfg,
    GpioPad,
    &RegisterOffset
  );

  *MiscCfgValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset
  );

  return EFI_SUCCESS;
}


/**
  This procedure sets Gpio Group to GPE mapping

  @param[in] This                 Gpio Services
  @param[in] GpioGroup0           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW0
  @param[in] GpioGroup1           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW1
  @param[in] GpioGroup2           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
SetGroupToGpeMapping (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD_GROUP              GpioGroup0,
  IN GPIOV2_PAD_GROUP              GpioGroup1,
  IN GPIOV2_PAD_GROUP              GpioGroup2
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  BOOLEAN           IsValid;
  UINT32            CommunityIndex;
  UINT32            GroupIndex;
  UINT32            GpioGpeDwx[3];
  UINT32            MiscCfgValue;

  if (This == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  GpioInterface->Public.IsGroupValid (&(GpioInterface->Public), GpioGroup0, &IsValid);
  if (IsValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.IsGroupValid (&(GpioInterface->Public), GpioGroup1, &IsValid);
  if (IsValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioInterface->Public.IsGroupValid (&(GpioInterface->Public), GpioGroup2, &IsValid);
  if (IsValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  //
  // Check if each group number is unique
  //
  if ((GpioGroup0 == GpioGroup1) ||
      (GpioGroup0 == GpioGroup2) ||
      (GpioGroup1 == GpioGroup2)) {
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (GpioGpeDwx, sizeof (GpioGpeDwx));

  //
  // GpioGroup0
  //
  if (GpioGroup0 != GPIOV2_PAD_GROUP_NONE) {
    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup0);
    GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup0);
    GpioGpeDwx[0] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpioGpeDwxVal;
  }

  //
  // GpioGroup1
  //
  if (GpioGroup1 != GPIOV2_PAD_GROUP_NONE) {
    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup1);
    GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup1);
    GpioGpeDwx[1] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpioGpeDwxVal;
  }

  //
  // GpioGroup2
  //
  if (GpioGroup2 != GPIOV2_PAD_GROUP_NONE) {
    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup2);
    GroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup2);
    GpioGpeDwx[2] = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].GroupToGpeMapping.GpioGpeDwxVal;
  }

  // Set Misccfg register
  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    GpioInterface->Public.GetMiscCfg (&(GpioInterface->Public), CommunityIndex, &MiscCfgValue);

    MiscCfgValue &= (UINT32) ~(B_GPIO_PCR_MISCCFG_GPE0_DW2 | B_GPIO_PCR_MISCCFG_GPE0_DW1 | B_GPIO_PCR_MISCCFG_GPE0_DW0);
    MiscCfgValue |= (UINT32) ((GpioGpeDwx[2] << N_GPIO_PCR_MISCCFG_GPE0_DW2) |
                              (GpioGpeDwx[1] << N_GPIO_PCR_MISCCFG_GPE0_DW1) |
                              (GpioGpeDwx[0] << N_GPIO_PCR_MISCCFG_GPE0_DW0));

    GpioInterface->Public.SetMiscCfg (&(GpioInterface->Public), CommunityIndex, MiscCfgValue);
  }

  return EFI_SUCCESS;
}

/**
  This is a null version of SetGroupToGpeMapping procedure. By default GpioV2Protocol
  GPE setter is disabled and it is enabled during PEI (only) in GpioV2PpiInitPei constructor.

  @param[in] This                 Gpio Services
  @param[in] GpioGroup0           GPIO Group
  @param[in] GpioGroup1           GPIO Group
  @param[in] GpioGroup2           GPIO Group

  @retval EFI_UNSUPPORTED         EFI_UNSUPPORTED
**/
EFI_STATUS
EFIAPI
SetGroupToGpeMappingNull (
  IN GPIOV2_SERVICES              *This,
  IN GPIOV2_PAD_GROUP              GpioGroup0,
  IN GPIOV2_PAD_GROUP              GpioGroup1,
  IN GPIOV2_PAD_GROUP              GpioGroup2
  )
{
  return EFI_UNSUPPORTED;
}

/**
  This procedure retrieves GPIOV2_PAD_GROUP based on GPIO/COM0/MISCCFG DwX values.

  @param[in]  This                 Gpio Services
  @param[out] GpioGroup0           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW0
  @param[out] GpioGroup1           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW1
  @param[out] GpioGroup2           GPIO Group (please refer to GpioV2Pad.h) to be mapped to GPE DW2

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetGroupToGpeMapping (
  IN  GPIOV2_SERVICES               *This,
  OUT GPIOV2_PAD_GROUP              *GpioGroup0,
  OUT GPIOV2_PAD_GROUP              *GpioGroup1,
  OUT GPIOV2_PAD_GROUP              *GpioGroup2
  )
{
  GPIOV2_INTERFACE   *GpioInterface;
  UINT32              MiscCfgDwX[3] = {0};
  GPIOV2_PAD_GROUP    GroupToGpeDwX[3] = {0};
  UINT32              MiscCfgValue;
  UINT8               GpeDwXIdx;
  UINT32              ComIdx;
  UINT8               GrpIdx;
  EFI_STATUS          Status;

  if ((This == NULL) ||
      (GpioGroup0 == NULL) ||
      (GpioGroup1 == NULL) ||
      (GpioGroup2 == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  // Taking data from community 0, beacuse all communities should have the same DwX values
  Status = This->GetMiscCfg (This, 0, &MiscCfgValue);
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  MiscCfgDwX[0] = (UINT8) ((MiscCfgValue & B_GPIO_PCR_MISCCFG_GPE0_DW0) >> N_GPIO_PCR_MISCCFG_GPE0_DW0);
  MiscCfgDwX[1] = (UINT8) ((MiscCfgValue & B_GPIO_PCR_MISCCFG_GPE0_DW1) >> N_GPIO_PCR_MISCCFG_GPE0_DW1);
  MiscCfgDwX[2] = (UINT8) ((MiscCfgValue & B_GPIO_PCR_MISCCFG_GPE0_DW2) >> N_GPIO_PCR_MISCCFG_GPE0_DW2);

  for (GpeDwXIdx = 0; GpeDwXIdx < 3; GpeDwXIdx++) {
    for (ComIdx = 0; ComIdx < GpioInterface->Private.CommunitiesNum; ComIdx++) {
      for (GrpIdx = 0; GrpIdx < GpioInterface->Private.Communities[ComIdx].GroupsNum; GrpIdx++) {
        if (MiscCfgDwX[GpeDwXIdx] == GpioInterface->Private.Communities[ComIdx].Groups[GrpIdx].GroupToGpeMapping.GpioGpeDwxVal) {
          GroupToGpeDwX[GpeDwXIdx] = GpioInterface->Private.Communities[ComIdx].Groups[GrpIdx].GpioPadGroup;
          break;
        }
      }
    }
  }

  if ((GroupToGpeDwX[0] == GroupToGpeDwX[1]) ||
      (GroupToGpeDwX[1] == GroupToGpeDwX[2]) ||
      (GroupToGpeDwX[0] == GroupToGpeDwX[2])) {
    return EFI_DEVICE_ERROR;
  }

  *GpioGroup0 = GroupToGpeDwX[0];
  *GpioGroup1 = GroupToGpeDwX[1];
  *GpioGroup2 = GroupToGpeDwX[2];

  return EFI_SUCCESS;
}

/**
  This procedure gets Gpio Pad GPE number

  @param[in]  This                Gpio Services
  @param[in]  GpioPad             GPIO Pad
  @param[out] GpeNumber           GPE Number

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetGpeNumber (
  IN GPIOV2_SERVICES  *This,
  IN GPIOV2_PAD       GpioPad,
  OUT UINT32          *GpeNumber
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  BOOLEAN           PadValid;
  UINT32            PadIdx;
  UINT32            Index;
  GPIOV2_PAD_GROUP  GroupToGpeMapping[3] = {0};
  GPIOV2_PAD_GROUP  GpioPadGroup;
  EFI_STATUS        Status;

  if ((This == NULL) || (GpeNumber == NULL)) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (PadValid != TRUE) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioInterface->Public.GetGroupToGpeMapping (
    &(GpioInterface->Public),
    &(GroupToGpeMapping[0]),
    &(GroupToGpeMapping[1]),
    &(GroupToGpeMapping[2])
  );
  if (EFI_ERROR (Status)) {
    return Status;
  }

  GpioPadGroup = GPIOV2_PAD_SET_PAD_INDEX (GpioPad, 0xFF);
  PadIdx  = GPIOV2_PAD_GET_PAD_INDEX (GpioPad);

  //
  // Check if pad is routed to 1-Tier GPE
  //
  for (Index = 0; Index < 3; Index++) {
    if (GroupToGpeMapping[Index] == GpioPadGroup) {
      *GpeNumber = PadIdx + (Index * 32);
      return EFI_SUCCESS;
    }
  }

  //
  // If GpioPadGroup number doesn't match any of above then
  // it means that the pad is routed to 2-tier GPE
  // which corresponds to  GPE_111 (0x6F)
  //
  *GpeNumber = PCH_GPIO_2_TIER_MASTER_GPE_NUMBER;

  return EFI_SUCCESS;
}

/**
  This procedure will create HOB for Conflict Check Data.

  @param[in] GpioServices             Gpio Services
  @param[in] GpioPadsConfigTable      Pointer to PadInitConfigTable
  @param[in] GpioPadsConfigTableSize  Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
EFIAPI
SetConflictCheckData (
  IN GPIOV2_SERVICES          *GpioServices,
  IN GPIOV2_INIT_CONFIG       *GpioPadsConfigTable,
  IN UINT32                   GpioPadsConfigTableSize
  )
{
  UINT32                       Index;
  UINT32                       GpioIndex;
  UINT16                       GpioCount;
  UINT32                       HobDataSize;
  VOID                         *ConflictCheckData;
  GPIOV2_CONFLICT_DATA_HEADER  *ConflictCheckHeader;
  GPIOV2_PAD_MODE_INFO         *PadModeInfo;

  if (GpioServices == NULL || GpioPadsConfigTable == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  DEBUG ((DEBUG_INFO, "[GPIOV2] [SetConflictCheckData]: Start\n"));

  GpioCount = 0;
  for (Index = 0; Index < GpioPadsConfigTableSize ; Index++) {
    if (GpioPadsConfigTable[Index].GpioConfig.PadMode == GpioV2PadModeHardwareDefault) {
      continue;
    } else {
      //
      // Calculate non-default GPIO number
      //
      GpioCount++;
    }
  }
  HobDataSize = GpioCount * sizeof (GPIOV2_PAD_MODE_INFO) + sizeof (GPIOV2_CONFLICT_DATA_HEADER);

  //
  // Build a HOB tagged with a GUID for identification and returns
  // the start address of GUID HOB data.
  //
  ConflictCheckData = BuildGuidHob (&gGpioCheckConflictHobGuid, HobDataSize);
  if (ConflictCheckData == NULL) {
    DEBUG ((DEBUG_INFO, "[GPIOV2] [SetConflictCheckData] [ERROR]: allocating memory for ConflictCheckData failed\n"));
    return EFI_OUT_OF_RESOURCES;
  }
  ZeroMem (ConflictCheckData, HobDataSize);

  ConflictCheckHeader = (GPIOV2_CONFLICT_DATA_HEADER *) ConflictCheckData;
  CopyMem (ConflictCheckHeader->Hid, GpioServices->Hid, GPIOV2_CONTROLLER_HID_LENGTH);
  ConflictCheckHeader->Uid = GpioServices->Uid;

  //
  // Record Non Default Gpio entries to the Hob
  //
  GpioIndex = 0;
  ConflictCheckHeader++;
  PadModeInfo = (GPIOV2_PAD_MODE_INFO *) ConflictCheckHeader;
  for (Index = 0; Index < GpioPadsConfigTableSize; Index++) {
    if (GpioPadsConfigTable[Index].GpioConfig.PadMode == GpioV2PadModeHardwareDefault) {
      continue;
    } else {
      if (GpioIndex < GpioCount) {
        PadModeInfo->GpioPad     = GpioPadsConfigTable[Index].GpioPad;
        PadModeInfo->GpioPadMode = GpioPadsConfigTable[Index].GpioConfig.PadMode;
      } else {
        DEBUG ((DEBUG_ERROR, "[GPIOV2] [SetConflictCheckData] [ERROR]: GpioIndex >= GpioCount\n"));
        ASSERT (FALSE);
        break;
      }
      GpioIndex++;
      PadModeInfo++;
    }
  }

  DEBUG ((DEBUG_INFO, "[GPIOV2] [SetConflictCheckData] End\n"));

  return EFI_SUCCESS;
}

/**
  This procedure will check if there are any discrepancies between
  data saved by GPIO_SET_CONFLICT_CHECK_DATA method and actual
  GPIOs configuration

  @param[in] GpioServices             Gpio Services

  @retval Status
**/
EFI_STATUS
EFIAPI
ConflictCheck (
  IN GPIOV2_SERVICES          *GpioServices
  )
{
  EFI_HOB_GUID_TYPE            *GpioCheckConflictHob;
  GPIOV2_PAD_MODE_INFO         *GpioCheckConflictPadData;
  GPIOV2_CONFLICT_DATA_HEADER  *ConflictCheckHeader;
  VOID                         *HobPtr;
  UINT32                       HobDataSize;
  UINT32                       GpioCount;
  UINT32                       GpioIndex;
  GPIOV2_PAD_MODE              PadMode;
  CHAR8                        PadName[GPIOV2_NAME_LENGTH_MAX];

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioCheckConflictHob = NULL;

  DEBUG ((DEBUG_INFO, "[GPIOV2] [ConflictCheck]: Start\n"));

  //
  // Use Guid to find HOB.
  //
  GpioCheckConflictHob = (EFI_HOB_GUID_TYPE *) GetFirstGuidHob (&gGpioCheckConflictHobGuid);
  if (GpioCheckConflictHob == NULL) {
    DEBUG ((DEBUG_INFO, "[GPIOV2] [ConflictCheck]: No GPIO HOB found.\n"));
  } else {
    while (GpioCheckConflictHob != NULL) {
      //
      // Find the Data area pointer and Data size from the Hob
      //
      HobPtr      = GET_GUID_HOB_DATA (GpioCheckConflictHob);
      HobDataSize = GET_GUID_HOB_DATA_SIZE (GpioCheckConflictHob);

      GpioCount = (HobDataSize - sizeof (GPIOV2_CONFLICT_DATA_HEADER)) / sizeof (GPIOV2_PAD_MODE_INFO);
      DEBUG ((DEBUG_INFO, "[GPIOV2] [ConflictCheck]: GpioCount =  %d\n", GpioCount));
      ConflictCheckHeader = (GPIOV2_CONFLICT_DATA_HEADER *) HobPtr;
      if (CompareMem (ConflictCheckHeader->Hid, GpioServices->Hid, GPIOV2_CONTROLLER_HID_LENGTH) == 0 &&
          ConflictCheckHeader->Uid == GpioServices->Uid) {
        //
        // Probe Gpio entries in Hob and compare which are conflicted
        //
        ConflictCheckHeader++;
        GpioCheckConflictPadData = (GPIOV2_PAD_MODE_INFO *) ConflictCheckHeader;
        for (GpioIndex = 0; GpioIndex < GpioCount ; GpioIndex++) {
          GpioServices->GetPadMode (GpioServices, GpioCheckConflictPadData->GpioPad, &PadMode);
          if ((GPIOV2_PAD_MODE) GpioCheckConflictPadData->GpioPadMode != PadMode) {
            GpioServices->GetPadName (GpioServices, GpioCheckConflictPadData->GpioPad, sizeof(PadName), PadName);
            DEBUG ((DEBUG_INFO, "[GPIOV2] [ConflictCheck]: Identified conflict on pad %a (com.grp.pad: %d.%d.%d) - pad mode %d, expected %d\n",
              PadName,
              GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioCheckConflictPadData->GpioPad),
              GPIOV2_PAD_GET_GROUP_INDEX (GpioCheckConflictPadData->GpioPad),
              GPIOV2_PAD_GET_PAD_INDEX (GpioCheckConflictPadData->GpioPad),
              PadMode,
              GpioCheckConflictPadData->GpioPadMode));
          }
          GpioCheckConflictPadData++;
        }
      }
      //
      // Find next Hob and return the Hob pointer by the specific Hob Guid
      //
      GpioCheckConflictHob = GET_NEXT_HOB (GpioCheckConflictHob);
      GpioCheckConflictHob = GetNextGuidHob (&gGpioCheckConflictHobGuid, GpioCheckConflictHob);
    }

    DEBUG ((DEBUG_INFO, "[GPIOV2] [ConflictCheck]: End\n"));
  }

  return EFI_SUCCESS;
}

/**
  This procedure gets GPIOV2_NATIVE_SIGNAL_DATA structure for a given Signal.

  Data in SignalData is valid only when the function returns with EFI_SUCCESS.

  @param[in]  GpioServices        Gpio Services
  @param[in]  Signal              Signal
  @param[out] SignalData          Signal data (it contains IosState, IosTerm, VwIndex etc.)

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_NOT_FOUND           A signal data not found
**/
EFI_STATUS
EFIAPI
GetSignalInfoBySignal (
  IN  GPIOV2_SERVICES             *This,
  IN  UINT32                      Signal,
  OUT GPIOV2_NATIVE_SIGNAL_DATA   *SignalData
  )
{
  GPIOV2_INTERFACE                *GpioInterface;
  GPIOV2_NATIVE_SIGNAL_DATA       *SignalDataIter;
  UINT32                          Index;

  if (This == NULL || SignalData == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  for (Index = 0; Index < GpioInterface->Private.NativeSignalDataNum; Index++) {
    SignalDataIter = &GpioInterface->Private.NativeSignalData[Index];
    if (SignalDataIter->Signal.Value == Signal) {
      *SignalData = *SignalDataIter;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  This function looks for GPIO pad and its mode for a given Signal.

  @attention
  If the function doesn't find any gpio pad for a given signal or find more than
  one, then it returns with EFI_NOT_FOUND.

  @param[in]  GpioServices        Gpio Services
  @param[in]  Signal              Signal
  @param[out] GpioPad             GPIO pad
  @param[out] PadMode             GPIO pad mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter
  @retval EFI_NOT_FOUND           GPIO pad not found
**/
EFI_STATUS
EFIAPI
GetNativePadByFunction (
  IN  GPIOV2_SERVICES             *This,
  IN  UINT32                      Signal,
  OUT GPIOV2_PAD                  *GpioPad,
  OUT GPIOV2_PAD_MODE             *PadMode
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  UINT32 CommunityIndex;
  UINT32 GroupIndex;
  UINT32 PadIndex;
  UINT32 PadSignalIndex;
  UINT32 FoundPadsCounter = 0;

  if (This == NULL || GpioPad == NULL || PadMode == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  //
  // Iterate through all signals which are defined in *GpioV2PadData.c file
  //
  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    for (GroupIndex = 0; GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum; GroupIndex++) {
      //
      // check if PadDataArray is supported
      //
      if (GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray == NULL) {
        continue;
      }
      for (PadIndex = 0; PadIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum; PadIndex++) {
        for (PadSignalIndex = 0; PadSignalIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignalsNum; PadSignalIndex++) {
          if (GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[PadSignalIndex].Signal.Value == Signal) {
            if (FoundPadsCounter > 0) {
              *GpioPad = GPIOV2_PAD_NONE;
              *PadMode = 0;
              return EFI_NOT_FOUND;
            }

            *PadMode = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[PadSignalIndex].PadMode;
            *GpioPad = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex];
            FoundPadsCounter++;
          }
        }
      }
    }
  }

  if (FoundPadsCounter == 0) {
    *GpioPad = GPIOV2_PAD_NONE;
    *PadMode = 0;
    return EFI_NOT_FOUND;
  }

  return EFI_SUCCESS;
}

/**
  This function returns GPIO pad data array for a given GPIO pad.

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[out] PadData             GPIO pad data

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter or invalid GPIO pad
**/
EFI_STATUS
EFIAPI
GetPadDataForPad (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  OUT GPIOV2_PAD_DATA             **PadData
  )
{
  GPIOV2_INTERFACE *GpioInterface;
  UINT32           CommunityIndex;
  UINT32           GroupIndex;
  UINT32           PadIndex;
  BOOLEAN          PadValid;

  if (This == NULL || PadData == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  GpioInterface->Public.IsPadValid (&(GpioInterface->Public), GpioPad, &PadValid);
  if (!PadValid) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2] %a PadIsNot Valid\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndex      = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);
  GroupIndex          = GPIOV2_PAD_GET_GROUP_INDEX     (GpioPad);
  PadIndex            = GPIOV2_PAD_GET_PAD_INDEX       (GpioPad);

  *PadData = &GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex];

  return EFI_SUCCESS;
}

/**
  This function returns GPIO pad mode for given GPIO pad and signal.

  @param[in]  GpioServices        Gpio Services
  @param[in]  GpioPad             GPIO pad
  @param[in]  Signal              Signal
  @param[out] PadMode             GPIO pad mode

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid input parameter or invalid GPIO pad
  @retval EFI_NOT_FOUND           GPIO pad mode not found
**/
EFI_STATUS
EFIAPI
GetPadModeForPad (
  IN  GPIOV2_SERVICES             *This,
  IN  GPIOV2_PAD                  GpioPad,
  IN  UINT32                      Signal,
  OUT GPIOV2_PAD_MODE             *PadMode
  )
{
  GPIOV2_PAD_DATA  *PadData;
  EFI_STATUS       Status;
  UINT32           Index;
  BOOLEAN          PadValid;

  if (This == NULL || PadMode == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = This->IsPadValid (This, GpioPad, &PadValid);
  if (!PadValid) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2] %a PadIsNot Valid\n", __FUNCTION__));
    return EFI_INVALID_PARAMETER;
  }

  Status = This->GetPadDataForPad (This, GpioPad, &PadData);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  for (Index = 0; Index < PadData->PadSignalsNum; Index++) {
    if (PadData->PadSignals[Index].Signal.Value == Signal) {
      *PadMode = PadData->PadSignals[Index].PadMode;
      return EFI_SUCCESS;
    }
  }

  return EFI_NOT_FOUND;
}

/**
  This function restricts 'policy group 1' write access to requested community leaving
  read access unchanged. Restriction is done for below SAI agents:
    1. HOSTIA_POSTBOOT_SAI
    2. HOSTIA_SMM_SAI
    3. HOSTIA_BOOT_SAI
    4. CSE_INTEL_SAI
    5. ISH_SAI

  @param[in] GpioServices                Gpio Services
  @param[in] CommunityIndex              Index of community

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
EFIAPI
RestrictWriteAccess (
  IN GPIOV2_SERVICES                    *GpioServices,
  IN UINT32                             CommunityIndex
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  GPIOV2_ACCESS_CONTROL_SAI_DW_DATA   AcData;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CommunityIndex >= GpioInterface->Private.CommunitiesNum) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GetAccessControl (&(GpioInterface->Public), CommunityIndex, 1, &AcData);
  AcData.Policy &= ~(B_SAI_HOSTIA_BOOT | B_SAI_HOSTIA_SMM | B_SAI_HOSTIA_POSTBOOT | B_SAI_CSE_INTEL | B_SAI_ISH_SAI);
  AcData.Read   |=  (B_SAI_HOSTIA_BOOT | B_SAI_HOSTIA_SMM | B_SAI_HOSTIA_POSTBOOT | B_SAI_CSE_INTEL | B_SAI_ISH_SAI);
  AcData.Write  &= ~(B_SAI_HOSTIA_BOOT | B_SAI_HOSTIA_SMM | B_SAI_HOSTIA_POSTBOOT | B_SAI_CSE_INTEL | B_SAI_ISH_SAI);
  SetAccessControl (&(GpioInterface->Public), CommunityIndex, 1, &AcData);

  return EFI_SUCCESS;
}

/**
  This function retreives Global Group Index from GPIOV2_PAD_GROUP.
  Global Group Index is used in ASL code in ACPI interface.
  Please refer to OneSiliconPkg\Fru\XXX\Include\AcpiTables\Dsdt\GpioGroupsXXX.asl file.

  @param[in]  This                Gpio Services
  @param[in]  GpioGroup           Gpio Group
  @param[out] GlobalGroupIndex    buffer for Global Group Index

  @retval EFI_SUCCESS                    The function completed successfully
  @retval EFI_INVALID_PARAMETER          Invalid group or pad number
**/
EFI_STATUS
EFIAPI
GetGlobalGroupIndex (
  IN   GPIOV2_SERVICES             *This,
  IN   GPIOV2_PAD_GROUP            GpioGroup,
  OUT  UINT32                      *GlobalGroupIndex
  )
{
  GPIOV2_INTERFACE   *GpioInterface;
  UINT32             CommunityIndex;
  UINT32             CommunityIndexMax;
  BOOLEAN            IsValid;

  if ( (This == NULL) || (GlobalGroupIndex == NULL) ){
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (This);

  GpioInterface->Public.IsGroupValid (&(GpioInterface->Public), GpioGroup, &IsValid);
  if (IsValid != TRUE) {
    ASSERT(FALSE);
    return EFI_INVALID_PARAMETER;
  }

  CommunityIndexMax = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioGroup);

  *GlobalGroupIndex = GPIOV2_PAD_GET_GROUP_INDEX (GpioGroup);
  for (CommunityIndex = 0; CommunityIndex < CommunityIndexMax; CommunityIndex++) {
    *GlobalGroupIndex += GpioInterface->Private.Communities[CommunityIndex].GroupsNum;
  }

  return EFI_SUCCESS;
}


/**
  This function returns next pad definition based on provided one.
  If CurrentPad is GPIOV2_PAD_NONE, a first pad from first group of first community is returned.
  When the end of GPIO pad list is reached, GPIOV2_PAD_INVALID is returned.

  @param[in]   GpioServices  Gpio Services
  @param[in]   CurrentPad    Current pad from which the next one is returned
  @param[out]  NextPad       GPIO pad following the provided one

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  Invalid input parameter
**/
EFI_STATUS
EFIAPI
GetNextPad (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  GPIOV2_PAD       CurrentPad,
  OUT GPIOV2_PAD       *NextPad
  )
{
  GPIOV2_INTERFACE  *GpioInterface;
  EFI_STATUS        Status;
  BOOLEAN           PadValid;
  UINT32            CommunityIndex;
  UINT32            GroupIndex;
  UINT32            PadIndex;

  if (GpioServices == NULL || NextPad == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  if (CurrentPad != GPIOV2_PAD_NONE) {
    Status = GpioInterface->Public.IsPadValid (&GpioInterface->Public, CurrentPad, &PadValid);
    if (EFI_ERROR (Status) || !PadValid) {
      return EFI_INVALID_PARAMETER;
    }
  } else {
    *NextPad = GpioInterface->Private.Communities[0].Groups[0].Pads[0];
    return EFI_SUCCESS;
  }

  CommunityIndex      = GPIOV2_PAD_GET_COMMUNITY_INDEX (CurrentPad);
  GroupIndex          = GPIOV2_PAD_GET_GROUP_INDEX     (CurrentPad);
  PadIndex            = GPIOV2_PAD_GET_PAD_INDEX       (CurrentPad);

  PadIndex++;
  for (; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    for (; GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum; GroupIndex++) {
      if (PadIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum) {
        *NextPad = GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex];
        return EFI_SUCCESS;
      } else {
        PadIndex = 0;
      }
    }
    GroupIndex = 0;
  }
  *NextPad = GPIOV2_PAD_INVALID;
  return EFI_NOT_FOUND;
}

/**
  This procedure assigns base functions to GpioServices

  @param[in, out] GpioServices     Gpio Services

  @retval EFI_SUCCESS              The function completed successfully
  @retval EFI_INVALID_PARAMETER    Invalid group or pad number
**/

EFI_STATUS
EFIAPI
GpioV2ServicesInit (
  IN OUT GPIOV2_SERVICES       *GpioServices
  )
{

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioServices->GetRegisterOffset        = GetRegisterOffset;
  GpioServices->SetPadMode               = SetPadMode;
  GpioServices->GetPadMode               = GetPadMode;
  GpioServices->SetInputInversion        = SetInputInversion;
  GpioServices->GetInputInversion        = GetInputInversion;
  GpioServices->SetLock                  = SetLock;
  GpioServices->GetLock                  = GetLock;
  GpioServices->SetLockTx                = SetLockTx;
  GpioServices->GetLockTx                = GetLockTx;
  GpioServices->SetTx                    = SetTx;
  GpioServices->GetTx                    = GetTx;
  GpioServices->GetRx                    = GetRx;
  GpioServices->SetTxDisable             = SetTxDisable;
  GpioServices->GetTxDisable             = GetTxDisable;
  GpioServices->SetRxDisable             = SetRxDisable;
  GpioServices->GetRxDisable             = GetRxDisable;
  GpioServices->SetResetConfig           = SetResetConfig;
  GpioServices->GetResetConfig           = GetResetConfig;
  GpioServices->SetTerminationConfig     = SetTerminationConfig;
  GpioServices->GetTerminationConfig     = GetTerminationConfig;
  GpioServices->SetIostandbyConfig       = SetIostandbyConfig;
  GpioServices->GetIostandbyConfig       = GetIostandbyConfig;
  GpioServices->SetDebounce              = SetDebounce;
  GpioServices->GetOwnership             = GetOwnership;
  GpioServices->SetHostOwnership         = SetHostOwnership;
  GpioServices->GetHostOwnership         = GetHostOwnership;
  GpioServices->SetGpiIe                 = SetGpiIe;
  GpioServices->GetGpiIe                 = GetGpiIe;
  GpioServices->GetGpiIs                 = GetGpiIs;
  GpioServices->SetGpiGpeEn              = SetGpiGpeEn;
  GpioServices->GetGpiGpeEn              = GetGpiGpeEn;
  GpioServices->GetGpiGpeSts             = GetGpiGpeSts;
  GpioServices->SetNmiEn                 = SetNmiEn;
  GpioServices->GetNmiEn                 = GetNmiEn;
  GpioServices->GetNmiSts                = GetNmiSts;
  GpioServices->SetSmiEn                 = SetSmiEn;
  GpioServices->GetSmiEn                 = GetSmiEn;
  GpioServices->GetSmiSts                = GetSmiSts;
  GpioServices->ClearSmiSts              = ClearSmiSts;
  GpioServices->GetGpiSmiNumFromPad      = GetGpiSmiNumFromPad;
  GpioServices->GetPadFromGpiSmiNum      = GetPadFromGpiSmiNum;
  GpioServices->GetPadAndSmiRegs         = GetPadAndSmiRegs;
  GpioServices->GetRxEvCfg               = GetRxEvCfg;
  GpioServices->SetRxEvCfg               = SetRxEvCfg;
  GpioServices->SetRouteNmi              = SetRouteNmi;
  GpioServices->SetRouteSmi              = SetRouteSmi;
  GpioServices->SetRouteSci              = SetRouteSci;
  GpioServices->SetRouteIoxApic          = SetRouteIoxApic;
  GpioServices->IsGroupValid             = IsGroupValid;
  GpioServices->IsPadValid               = IsPadValid;
  GpioServices->LockAll                  = LockAll;
  GpioServices->GetPadName               = GetPadName;
  GpioServices->ConfigurePad             = ConfigurePad;
  GpioServices->ConfigurePads            = ConfigurePads;
  GpioServices->GetPadConfiguration      = GetPadConfiguration;
  GpioServices->GetPadIoApicIrqNum       = GetPadIoApicIrqNum;
  GpioServices->SetMiscCfg               = SetMiscCfg;
  GpioServices->GetMiscCfg               = GetMiscCfg;
  GpioServices->SetGroupToGpeMapping     = SetGroupToGpeMappingNull;
  GpioServices->GetGroupToGpeMapping     = GetGroupToGpeMapping;
  GpioServices->GetGpeNumber             = GetGpeNumber;
  GpioServices->SetConflictCheckData     = SetConflictCheckData;
  GpioServices->ConflictCheck            = ConflictCheck;
  GpioServices->GetSignalInfoBySignal    = GetSignalInfoBySignal;
  GpioServices->GetPadDataForPad         = GetPadDataForPad;
  GpioServices->GetNativePadByFunction   = GetNativePadByFunction;
  GpioServices->GetPadModeForPad         = GetPadModeForPad;
  GpioServices->RestrictWriteAccess      = RestrictWriteAccess;
  GpioServices->GetGlobalGroupIndex      = GetGlobalGroupIndex;
  GpioServices->GetNextPad               = GetNextPad;

  return EFI_SUCCESS;
}
