/** @file
  Common methods both for PEI and DXE phase for GpioV2Controller lib.

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

#include <Library/BaseMemoryLib.h>
#include <GpioV2ControllerInterface.h>
#include <Register/GpioRegs.h>

/**
  Helper PWM services function for retrieving PMWC register
  offset and GPIO community.

  @param[in]   GpioPwm       Gpio PWM Services
  @param[in]   PwmcOffset    PWMC offset
  @param[in]   PwmcCom       PWMC GPIO community

  @retval EFI_SUCCESS        Function completed successfully
  @retval EFI_NOT_FOUND      PWMC register not found in GPIO topology
**/
EFI_STATUS
EFIAPI
GetPwmcOffsetAndCom (
  IN  GPIOV2_INTERFACE  *GpioInterface,
  IN  UINT32            *PwmcOffset,
  IN  UINT32             *PwmcCom
  )
{
  UINT32              CommunityIndex;
  UINT32              RegisterOffset;

  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    GpioInterface->Public.GetRegisterOffset (
      &(GpioInterface->Public),
      GpioV2Pwmc,
      GPIOV2_PAD_ID (0, 0, 0, CommunityIndex, 0, 0),
      &RegisterOffset
    );
    if (RegisterOffset != 0) {
      break;
    }
  }
  if (RegisterOffset == 0) {
    return EFI_NOT_FOUND;
  }

  *PwmcOffset = RegisterOffset;
  *PwmcCom    = CommunityIndex;

  return EFI_SUCCESS;
}


/**
  This function starts output of PWM signal. GPIO pad should be already
  set to "Blink" pad mode function before this function call.

  @param[in]   GpioPwm           Gpio PWM Services

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
  @retval EFI_NOT_FOUND          PWMC register not found in GPIO topology
**/
EFI_STATUS
EFIAPI
PwmOn (
  IN  GPIOV2_PWM  *GpioPwm
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  UINT32              RegisterOffset;
  UINT32              CommunityIndex;
  EFI_STATUS          Status;

  if (GpioPwm == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PWM (GpioPwm);

  Status = GetPwmcOffsetAndCom (GpioInterface, &RegisterOffset, &CommunityIndex);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Or32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    (UINT32)B_GPIO_PCR_PWMC_EN
  );

  return EFI_SUCCESS;
}


/**
  This function stops output of PWM signal.

  @param[in]   GpioPwm           Gpio PWM Services

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
  @retval EFI_NOT_FOUND          PWMC register not found in GPIO topology
**/
EFI_STATUS
EFIAPI
PwmOff (
  IN  GPIOV2_PWM  *GpioPwm
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  UINT32              RegisterOffset;
  UINT32               CommunityIndex;
  EFI_STATUS          Status;

  if (GpioPwm == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PWM (GpioPwm);

  Status = GetPwmcOffsetAndCom (GpioInterface, &RegisterOffset, &CommunityIndex);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.And32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    (UINT32)~B_GPIO_PCR_PWMC_EN
  );

  return EFI_SUCCESS;
}


/**
  This function sets frequency of PWM signal.

  @param[in]   GpioPwm           Gpio PWM Services
  @param[in]   Frequency         Fpwm = Frequency * 7.8125 [mHz], Fpwm_max = 32.767 [kHz]

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
  @retval EFI_NOT_FOUND          PWMC register not found in GPIO topology
**/
EFI_STATUS
EFIAPI
PwmSetFreq (
  IN  GPIOV2_PWM  *GpioPwm,
  IN  UINT32      Frequency
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  UINT32              RegisterOffset;
  UINT32               CommunityIndex;
  EFI_STATUS          Status;

  if (GpioPwm == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PWM (GpioPwm);

  Status = GetPwmcOffsetAndCom (GpioInterface, &RegisterOffset, &CommunityIndex);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    (UINT32)~B_GPIO_PCR_PWMC_BASEUNIT,
    (UINT32)(((Frequency << N_GPIO_PCR_PWMC_BASEUNIT) & B_GPIO_PCR_PWMC_BASEUNIT)  |
    B_GPIO_PCR_PWMC_SWUP)
  );

  return EFI_SUCCESS;
}


/**
  This function sets duty cycle of PWM signal.

  @param[in]   GpioPwm           Gpio PWM Services
  @param[in]   DutyCycle         Percentage value of PWM duty Cycle, max 99%

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
  @retval EFI_NOT_FOUND          PWMC register not found in GPIO topology
**/
EFI_STATUS
EFIAPI
PwmSetDuty (
  IN  GPIOV2_PWM  *GpioPwm,
  IN  UINT32      DutyCycle
  )
{
  GPIOV2_INTERFACE    *GpioInterface;
  UINT32              RegisterOffset;
  UINT32              CommunityIndex;
  EFI_STATUS          Status;

  if (GpioPwm == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PWM (GpioPwm);

  Status = GetPwmcOffsetAndCom (GpioInterface, &RegisterOffset, &CommunityIndex);
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  GpioInterface->Private.CommunityAccess[CommunityIndex].Access.AndThenOr32 (
    &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
    RegisterOffset,
    (UINT32)~B_GPIO_PCR_PWMC_ONTIMEDIV,
    (UINT32)((((DutyCycle * 256) / 100) & B_GPIO_PCR_PWMC_ONTIMEDIV) |
    B_GPIO_PCR_PWMC_SWUP)
  );

  return EFI_SUCCESS;
}


/**
  This procedure assigns base GpioV2 PWM functions.

  @param[in]   GpioPwm           Gpio PWM Services

  @retval EFI_SUCCESS            Function completed successfully
  @retval EFI_INVALID_PARAMETER  NULL pointer to GPIOV2_PWM
**/
EFI_STATUS
EFIAPI
GpioV2PwmInit (
  IN OUT GPIOV2_PWM       *GpioPwm
  )
{
  if (GpioPwm == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  GpioPwm->PwmOn            = PwmOn;
  GpioPwm->PwmOff           = PwmOff;
  GpioPwm->PwmSetFreq       = PwmSetFreq;
  GpioPwm->PwmSetDuty       = PwmSetDuty;

  return EFI_SUCCESS;
}
