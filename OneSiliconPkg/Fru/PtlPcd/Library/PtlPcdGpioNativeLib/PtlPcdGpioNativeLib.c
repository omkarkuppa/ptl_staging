/** @file
  PTL SOC Gpio library.
  All function in this library is available for PEI, DXE, and SMM,
  But do not support UEFI RUNTIME environment call.

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

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/PcdLib.h>
#include <Library/PcdGpioNativeLib.h>
#include <Library/GpioHelpersLib.h>
#include <Library/SocInfoLib.h>
#include <Library/PcdInfoLib.h>
#include <GpioV2Pad.h>
#include <Pins/GpioV2PadMuxing.h>
#include <Register/GpioRegs.h>
#include <Register/GpioV2ChipsetId.h>
#include <Register/GpioV2PcdPins.h>
#include <GpioV2ControllerInterface.h>
#include <Library/GpioV2AccessLib.h>
#include <GpioV2Signals.h>
#include <GpioV2Services.h>
#include <TcssPeiPreMemConfig.h>

/**
  This function provides GPIO Native Pad for a given native function
  @param[in]  GpioServices           GPIO Services
  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioV2Signals.h

  @retval     GpioPad                GPIO pad with encoded native function
**/
GPIOV2_PAD
PtlPcdGpioGetNativePadByFunction (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT32           PadFunction
  )
{
  GPIOV2_PAD       GpioPad;
  GPIOV2_PAD_MODE  PadMode;
  EFI_STATUS       Status;

  if (GpioServices == NULL) {
    return GPIOV2_PAD_NONE;
  }

  Status = GpioServices->GetNativePadByFunction (GpioServices, PadFunction, &GpioPad, &PadMode);
  if (EFI_ERROR (Status)) {
    return GPIOV2_PAD_NONE;
  }

  return GPIOV2_PAD_SET_NATIVE_FUNCTION (GpioPad, ((PadMode >> 1) & GPIOV2_PAD_MODE_MASK));
}

/**
  This function provides recommended GPIO IO Standby configuration for a given native function

  @param[in]  GpioServices           GPIO Services
  @param[in]  PadFunction            PadFunction for a specific native signal. Please refer to GpioV2Signals.h
  @param[out] StandbyState           IO Standby State for specified native function
  @param[out] StandbyTerm            IO Standby Termination for specified native function

  @retval Status
**/
EFI_STATUS
PtlPcdGpioGetFunctionIoStandbyConfig (
  IN  GPIOV2_SERVICES         *GpioServices,
  IN  UINT32                  PadFunction,
  OUT GPIOV2_IOSTANDBY_STATE  *StandbyState,
  OUT GPIOV2_IOSTANDBY_TERM   *StandbyTerm
  )
{
  EFI_STATUS                 Status;
  GPIOV2_NATIVE_SIGNAL_DATA  SignalData = { 0 };

  if ((GpioServices == NULL) || (StandbyState == NULL) || (StandbyTerm == NULL)) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioServices->GetSignalInfoBySignal (
                           GpioServices,
                           PadFunction,
                           &SignalData
                           );

  if (EFI_ERROR (Status)) {
    return EFI_NOT_FOUND;
  }

  *StandbyState = SignalData.IosState;
  *StandbyTerm  = SignalData.IosTerm;

  return EFI_SUCCESS;
}

/**
  Configures IO standby related settings for the GPIO function.

  @param[in]  GpioServices        GPIO Services
  @param[in]  PadFunction         GPIO pad function
  @param[in]  PinMux              GPIO pad pinmux

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid GPIO pad function
**/
EFI_STATUS
PtlPcdGpioConfigurePadIoStandbyByFunction (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT32           PadFunction,
  IN  UINT32           PinMux
  )
{
  GPIOV2_PAD              GpioPad;
  EFI_STATUS              Status;
  GPIOV2_IOSTANDBY_STATE  StandbyState = GpioV2IosStateDefault;
  GPIOV2_IOSTANDBY_TERM   StandbyTerm  = GpioV2IosTermDefault;

  GpioPad = PtlPcdGpioGetNativePadByFunction (GpioServices, PadFunction);

  if (GpioPad != GPIOV2_PAD_NONE) {
    //
    // For situations where native signal can be enabled only on
    // a single pad then PinMux argument should be set to 0.
    //
    ASSERT (PinMux == GPIOV2_PAD_NONE);
  } else {
    //
    // If native signal can be enabled on multiple pads
    // then PinMux argument needs to be configured to platform specific value
    //
    GpioPad = PinMux;
  }

  Status = PtlPcdGpioGetFunctionIoStandbyConfig (GpioServices, PadFunction, &StandbyState, &StandbyTerm);
  //
  // Not all PCH support Io Standby, in such case BIOS will skip IoStandby configuration
  //
  if (Status == EFI_UNSUPPORTED) {
    return EFI_SUCCESS;
  } else if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  } else {
    return GpioServices->SetIostandbyConfig (GpioServices, GpioPad, StandbyState, StandbyTerm);
  }
}

/**
  This procedure will get GPIO native pad based on provided native function and platform muxing selection (if needed).

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval  NativePad              GPIO pad with encoded native function
**/
GPIOV2_NATIVE_PAD
PtlPcdGpioGetNativePadByFunctionAndPinMux (
  IN GPIOV2_SERVICES  *GpioServices,
  IN UINT32           PadFunction,
  IN UINT32           PinMux
  )
{
  GPIOV2_PAD  GpioPad;

  GpioPad = PtlPcdGpioGetNativePadByFunction (GpioServices, PadFunction);

  if (GpioPad != GPIOV2_PAD_NONE) {
    //
    // For situations where native signal can be enabled only on
    // a single pad then PinMux argument should be set to 0.
    //
    ASSERT (PinMux == GPIOV2_PAD_NONE);
  } else {
    //
    // If native signal can be enabled on multiple pads
    // then PinMux argument needs to be configured to platform specific value
    GpioPad = PinMux;
  }

  return GpioPad;
}

/**
  This procedure will set GPIO pad to native function based on provided native function
  and platform muxing selection (if needed).

  @param[in]  GpioServices        GPIO Services
  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux              GPIO Native pin mux platform config.
                                  This argument is optional and needs to be
                                  provided only if feature can be enabled
                                  on multiple pads

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid group or pad number
**/
EFI_STATUS
PtlPcdGpioSetNativePadByFunction (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT32           PadFunction,
  IN  UINT32           PinMux
  )
{
  GPIOV2_PAD  GpioPad;
  EFI_STATUS  Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  GpioPad = PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, PadFunction, PinMux);

  if (GpioPad == GPIOV2_PAD_NONE) {
    return EFI_INVALID_PARAMETER;
  }

  Status = GpioServices->SetPadMode (GpioServices, GpioPad, GPIOV2_PAD_GET_PAD_MODE (GpioPad));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PtlPcdGpioConfigurePadIoStandbyByFunction (GpioServices, PadFunction, PinMux);
  if (EFI_ERROR (Status)) {
    return EFI_UNSUPPORTED;
  }

  return EFI_SUCCESS;
}

//
// CNVi BRI(Bluetooth Radio Interface) and RGI(Radio Generic Interface) buses from PCH to CRF(Companion RF) module
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdCnviBriRgiGpioPadFunction[] =
{
  GPIOV2_SIGNAL_CNVI_BRI_DT,  // CNV_BRI_DT
  GPIOV2_SIGNAL_CNVI_BRI_RSP, // CNV_BRI_RSP
  GPIOV2_SIGNAL_CNVI_RGI_DT,  // CNV_RGI_DT
  GPIOV2_SIGNAL_CNVI_RGI_RSP  // CNV_RGI_RSP
};

/**
  This function configures GPIO connection between CNVi and CRF

  @param[in]  GpioServices        GPIO Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioConfigureCnviCrfConnection (
  IN  GPIOV2_SERVICES  *GpioServices
  )
{
  EFI_STATUS  Status;
  UINT32      Index;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdCnviBriRgiGpioPadFunction); Index++) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdCnviBriRgiGpioPadFunction[Index], 0);

    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mSocMCnviMfUart2GpioPadFunction[] =
{
  GPIOV2_SIGNAL_CNVI_MFUART_RXD (2), // CNV_MFUART2_RXD
  GPIOV2_SIGNAL_CNVI_MFUART_TXD (2)  // CNV_MFUART2_TXD
};

/**
  This function configures CNVi MFUART2 external pins

  @param[in]  GpioServices        GPIO Services

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetCnviMfUart2ExternalPins (
  IN  GPIOV2_SERVICES  *GpioServices
  )
{
  EFI_STATUS  Status;
  UINT32      Index;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  for (Index = 0; Index < ARRAY_SIZE (mSocMCnviMfUart2GpioPadFunction); Index++) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mSocMCnviMfUart2GpioPadFunction[Index], 0);

    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  return EFI_SUCCESS;
}

/**
  This function provides CNVi CRF presence strap pin

  @param[in]  GpioServices        Gpio Services

  @retval     BOOLEAN             True: Device is present
                                  False : Device is absent
**/
BOOLEAN
PtlPcdGpioGetCnviCrfStrapPinStatus (
  IN  GPIOV2_SERVICES    *GpioServices
  )
{
  UINT32                      GpioDw2Value;
  UINT32                      GpioDw2Offset;
  EFI_STATUS                  Status;
  GPIOV2_INTERFACE            *GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);
  UINT32                      CommunityIndex;
  GPIOV2_PAD                  CrfStrapPin;

    CrfStrapPin = GPIOV2_PTL_PCD_XXGPP_F_2;

  Status = GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw2Reg,
    CrfStrapPin,
    &GpioDw2Offset
    );

  if (!EFI_ERROR (Status)) {
    // Reading register value
    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (CrfStrapPin);
    GpioDw2Value = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      GpioDw2Offset
      );

    //
    // CNV_RGI_DT is used to strap presence of the CRF, which sampling immediately after RSMRST# de-assert.
    // It is pulled down by the CRF as long as RF_RESET_B = 0
    //
    if (GpioDw2Value & B_GPIO_PCR_PINSTRAPVAL) {
      return FALSE;
    } else {
      DEBUG((DEBUG_INFO, " CRF Device is Present.\n"));
      return TRUE;
    }
  }
  return FALSE;
}

//
// CNVi Bluetooth I2S pads
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD mPtlPcdVCnviBtI2sGpioPad[][PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  {
    GPIOV2_PTL_PCD_VGPIO_30, // vCNV_BT_I2S_BCLK
    GPIOV2_PTL_PCD_VGPIO_31, // vCNV_BT_I2S_WS_SYNC
    GPIOV2_PTL_PCD_VGPIO_32, // vCNV_BT_I2S_SDO
    GPIOV2_PTL_PCD_VGPIO_33  // vCNV_BT_I2S_SDI
  },
  {
    GPIOV2_PTL_PCD_VGPIO_40, // vCNV_BT_I2S_BCLK_2
    GPIOV2_PTL_PCD_VGPIO_41, // vCNV_BT_I2S_WS_SYNC_2
    GPIOV2_PTL_PCD_VGPIO_42, // vCNV_BT_I2S_SDO_2
    GPIOV2_PTL_PCD_VGPIO_43  // vCNV_BT_I2S_SDI_2
  }
};

//
// vSSP for Bluetooth
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD mPtlPcdVSspForCnviBtGpioPad[][PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS] =
{
  {
    GPIOV2_PTL_PCD_VGPIO_34, // vSSP2_SCLK
    GPIOV2_PTL_PCD_VGPIO_35, // vSSP2_SFRM
    GPIOV2_PTL_PCD_VGPIO_36, // vSSP2_TXD
    GPIOV2_PTL_PCD_VGPIO_37  // vSSP2_RXD
  },
  {
    GPIOV2_PTL_PCD_VGPIO_44, // vSSP2_SCLK_2
    GPIOV2_PTL_PCD_VGPIO_45, // vSSP2_SFRM_2
    GPIOV2_PTL_PCD_VGPIO_46, // vSSP2_TXD_2
    GPIOV2_PTL_PCD_VGPIO_47  // vSSP2_RXD_2
  }
};


/**
  This function provides CNVi BT I2S pins

  @param[in]  GpioServices        GPIO Services
  @param[in]  ConnectionType      CNVi BT I2S connection type
  @param[in]  ConnectionIndex     CNVi BT I2S connection index

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
  @retval EFI_UNSUPPORTED         Unsupported operation
**/
EFI_STATUS
PtlPcdGpioConfigureCnviBtI2sConnection (
  IN  GPIOV2_SERVICES                      *GpioServices,
  IN  VGPIOV2_CNVI_BT_I2S_CONNECTION_TYPE  ConnectionType,
  IN  UINT32                               ConnectionIndex
  )
{
  EFI_STATUS       Status;
  UINT32           Index;
  GPIOV2_PAD_MODE  VCnviBtI2sPadMode;
  GPIOV2_PAD_MODE  VSspForCnviBtPadMode;
  UINT32           VCnviBtI2sGpioPadSize;
  UINT32           VSspForCnviBtGpioPadSize;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }
    VCnviBtI2sGpioPadSize = ARRAY_SIZE (mPtlPcdVCnviBtI2sGpioPad);
    VSspForCnviBtGpioPadSize = ARRAY_SIZE (mPtlPcdVSspForCnviBtGpioPad);
  if ((ConnectionIndex < VCnviBtI2sGpioPadSize) && (ConnectionIndex < VSspForCnviBtGpioPadSize)) {
    switch (ConnectionType) {
      case GpioV2CnviBtI2sToSsp0:
        VCnviBtI2sPadMode    = GpioV2PadModeNative1;
        VSspForCnviBtPadMode = GpioV2PadModeNative1;
        break;
      case GpioV2CnviBtI2sToSsp1:
        VCnviBtI2sPadMode    = GpioV2PadModeNative2;
        VSspForCnviBtPadMode = GpioV2PadModeNative1;
        break;
      case GpioV2CnviBtI2sToSsp2:
        VCnviBtI2sPadMode    = GpioV2PadModeNative3;
        VSspForCnviBtPadMode = GpioV2PadModeNative1;
        break;
      case GpioV2CnviBtI2sNotConnected:
      case GpioV2CnviBtI2sToExternalPads:
        VCnviBtI2sPadMode    = GpioV2PadModeGpio;
        VSspForCnviBtPadMode = GpioV2PadModeGpio;
        break;
      default:
        ASSERT (FALSE);
        return EFI_UNSUPPORTED;
    }

    //
    // Configure CNVi Bluetooth I2S for certain connection
    //
    for (Index = 0; Index < PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS; Index++) {
        Status = GpioServices->SetPadMode (GpioServices, mPtlPcdVCnviBtI2sGpioPad[ConnectionIndex][Index], VCnviBtI2sPadMode);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return EFI_UNSUPPORTED;
      }
    }

    //
    // Enable virtual connection for SSP for Bluetooth
    //
    for (Index = 0; Index < PCH_GPIO_CNVI_SSP_NUMBER_OF_PINS; Index++) {
        Status = GpioServices->SetPadMode (GpioServices, mPtlPcdVSspForCnviBtGpioPad[ConnectionIndex][Index], VSspForCnviBtPadMode);
      if (EFI_ERROR (Status)) {
        ASSERT_EFI_ERROR (Status);
        return EFI_UNSUPPORTED;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  This function configures CNVi Bluetooth Enable pad

  @param[in]  GpioServices        GPIO Services
  @param[in]  Value               CNVi BT enable value
                                   0: Disable, 1: Enable

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetCnviBtEnState (
  IN  GPIOV2_SERVICES  *GpioServices,
  IN  UINT8            Value
  )
{
  EFI_STATUS     Status;
  GPIOV2_CONFIG  PadConfig;
  GPIOV2_PAD     GpioPin;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&PadConfig, sizeof (PadConfig));

  PadConfig.PadMode   = GpioV2PadModeGpio;
  PadConfig.HostOwn   = GpioV2HostOwnGpio;
  PadConfig.Direction = GpioV2DirOut;

  if (Value == 1) {
    PadConfig.OutputState = GpioV2StateHigh;
  } else {
    PadConfig.OutputState = GpioV2StateLow;
  }

    GpioPin = GPIOV2_PTL_PCD_VGPIO_0;

  // Unlock GPIO pad due to Host Software Pad Ownership is GPIO Driver mode and it is GPO
  Status = GpioServices->SetLock (GpioServices, GpioPin, GpioV2Unlock);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GpioServices->SetLockTx (GpioServices, GpioPin, GpioV2Unlock);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = GpioServices->ConfigurePad (GpioServices, GpioPin, &PadConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function enables CNVi BT interface

  @param[in]  GpioServices        GPIO Services
  @param[in]  BtInterface         Bluetooth Interface Value

  @retval EFI_SUCCESS             The function completed successfully
  @retval EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioEnableCnviBtInterface (
  IN  GPIOV2_SERVICES            *GpioServices,
  IN  VGPIOV2_CNVI_BT_INTERFACE  BtInterface
  )
{
  EFI_STATUS     Status;
  GPIOV2_CONFIG  PadConfig;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  ZeroMem (&PadConfig, sizeof (PadConfig));

  PadConfig.PadMode   = GpioV2PadModeGpio;
  PadConfig.Direction = GpioV2DirOut;

  if (BtInterface == GpioV2CnviBtIfUsb) {
    PadConfig.OutputState = GpioV2StateHigh;
  } else {
    PadConfig.OutputState = GpioV2StateLow;
  }

    Status = GpioServices->ConfigurePad (GpioServices, GPIOV2_PTL_PCD_VGPIO_5, &PadConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This procedure will initialize all GPIO pads based on PadInitConfigTable
  @param[in] GpioServices           GPIO Services
  @param[in] PadInitConfigTable     Pointer to PadInitConfigTable
  @param[in] PadInitConfigTableSize Size of PadInitConfigTable

  @retval Status
**/
EFI_STATUS
PtlPcdGpioInitPads (
  IN GPIOV2_SERVICES     *GpioServices,
  IN GPIOV2_INIT_CONFIG  *PadInitConfigTable,
  IN UINT32              PadInitConfigTableSize
  )
{
  EFI_STATUS  Status;

  DEBUG ((DEBUG_INFO, "[GPIOV2] %a start\n", __FUNCTION__));

  Status = GpioServices->ConfigurePads (
                           GpioServices,
                           PadInitConfigTable,
                           PadInitConfigTableSize
                           );
  if (Status != EFI_SUCCESS) {
    return Status;
  }

  DEBUG ((DEBUG_INFO, "[GPIOV2] %a end\n", __FUNCTION__));

  return EFI_SUCCESS;
}

//
// TBT LSX 0/1/2/3 A and B pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD_NATIVE_FUNCTION mPtlPcdTbtLsxInterfacePins[][2] =
{
  {// DDI 1 TBT LSX
    {GPIOV2_PTL_PCD_XXGPP_C_16, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame},// TBT_LSX0_A
    {GPIOV2_PTL_PCD_XXGPP_C_17, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame} // TBT_LSX0_B
  },
  {// DDI 2 TBT LSX
    {GPIOV2_PTL_PCD_XXGPP_C_18, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame},// TBT_LSX1_A
    {GPIOV2_PTL_PCD_XXGPP_C_19, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame} // TBT_LSX1_B
  },
  {// DDI 3 TBT LSX
    {GPIOV2_PTL_PCD_XXGPP_C_20, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame},// TBT_LSX2_A
    {GPIOV2_PTL_PCD_XXGPP_C_21, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame} // TBT_LSX2_B
  },
  {// DDI 4 TBT LSX
    {GPIOV2_PTL_PCD_XXGPP_C_22, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame},// TBT_LSX3_A
    {GPIOV2_PTL_PCD_XXGPP_C_23, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame} // TBT_LSX3_B
  }
};


/**
  This function sets TBT_LSx pin into native mode

  @param[in]  GpioServices      Gpio Services
  @param[in]  TbtLsxDdiPort     TBT_LSx for DDI Port Number

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableTbtLsxInterface (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  GPIOV2_TBT_LSX     TbtLsxDdiPort
  )
{
  EFI_STATUS                   Status;
  UINTN                        Index;
  GPIOV2_PAD_NATIVE_FUNCTION   *TbtLsxGpio;
  UINT32                       TbtLsxPinIndex;
  UINT32                       TbtLsxPinsSize;

  TbtLsxGpio = NULL;
  if (GpioOverrideLevel1Enabled()) {
    return EFI_SUCCESS;
  }

  TbtLsxPinIndex = TbtLsxDdiPort - GpioV2TbtLsxDdi1;
    TbtLsxPinsSize = ARRAY_SIZE (mPtlPcdTbtLsxInterfacePins);
  if (TbtLsxPinIndex < TbtLsxPinsSize) {
      TbtLsxGpio = mPtlPcdTbtLsxInterfacePins[TbtLsxPinIndex];
  }
  if (TbtLsxGpio == NULL) {
    return EFI_UNSUPPORTED;
  }
  for (Index = 0; Index < 2; Index++) {
    Status = GpioServices->SetPadMode (GpioServices, TbtLsxGpio[Index].Pad, TbtLsxGpio[Index].Mode);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    Status = GpioServices->SetIostandbyConfig (GpioServices, TbtLsxGpio[Index].Pad, TbtLsxGpio[Index].IosState, TbtLsxGpio[Index].IosTerm);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

//
// TBT LSx_OE 0/1/2/3 pins
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD_NATIVE_FUNCTION mPtlPcdTbtLsxOeInterfacePins[] =
{
  // MISCB TBT LSx0_OE
  {GPIOV2_PTL_PCD_XXGPP_B_17, GpioV2PadModeNative1, GpioV2IosStateDefault, GpioV2IosTermSame},// TBT_LSX0_OE
  // MISCB TBT LSx1_OE
  {GPIOV2_PTL_PCD_XXGPP_B_16, GpioV2PadModeNative1, GpioV2IosStateDefault, GpioV2IosTermSame},// TBT_LSX1_OE
  // MISCH TBT LSx2_OE
  {GPIOV2_PTL_PCD_XXGPP_H_16, GpioV2PadModeNative1, GpioV2IosStateDefault, GpioV2IosTermSame},// TBT_LSX2_OE
  // MISCD TBT LSx3_OE
  {GPIOV2_PTL_PCD_XXGPP_D_14, GpioV2PadModeNative2, GpioV2IosStateDefault, GpioV2IosTermSame} // TBT_LSX3_OE
};


/**
  This function sets TBT LSx_OE pins into native mode

  @param[in]  GpioServices     Gpio Services
  @param[in]  TbtLsxOePort     TBT LSx for OE Port Number

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableTbtLsxOeInterface (
  IN  GPIOV2_SERVICES    *GpioServices,
  IN  GPIOV2_TBT_LSX_OE  TbtLsxOePort
  )
{
  EFI_STATUS                   Status;
  UINT32                       TbtLsxOePinIndex;
  UINT32                       TbtLsxOePinsSize;
  GPIOV2_PAD_NATIVE_FUNCTION*  TbtLsxOeInterfacePins;

  if (GpioOverrideLevel1Enabled()) {
    return EFI_SUCCESS;
  }

  TbtLsxOePinIndex = TbtLsxOePort - GpioV2TbtLsxOe0;
    TbtLsxOeInterfacePins = mPtlPcdTbtLsxOeInterfacePins;
    TbtLsxOePinsSize = ARRAY_SIZE (mPtlPcdTbtLsxOeInterfacePins);
  if (TbtLsxOePinIndex < TbtLsxOePinsSize) {
    Status = GpioServices->SetPadMode (GpioServices, TbtLsxOeInterfacePins[TbtLsxOePinIndex].Pad, TbtLsxOeInterfacePins[TbtLsxOePinIndex].Mode);
    if (EFI_ERROR (Status)) {
      return Status;
    }

    Status = GpioServices->SetIostandbyConfig (GpioServices, TbtLsxOeInterfacePins[TbtLsxOePinIndex].Pad, TbtLsxOeInterfacePins[TbtLsxOePinIndex].IosState, TbtLsxOeInterfacePins[TbtLsxOePinIndex].IosTerm);
    if (EFI_ERROR (Status)) {
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  Power button debounce configuration
  Debounce time can be specified in microseconds. Only certain values according
  to below formula are supported:
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(glitch filter clock period).
  RTC clock with f = 32 KHz is used for glitch filter.
   DebounceTime = (2 ^ PADCFG_DW2.DEBOUNCE)*(31.25 us).
  Supported DebounceTime values are following:
   DebounceTime = 0 -> Debounce feature disabled
   DebounceTime > 0 && < 250us -> Not supported
   DebounceTime = 250us - 1024000us -> Supported range (DebounceTime = 250us * 2^n)
  For values not supported by HW, they will be rounded down to closest supported one

  @param[in]  GpioServices   GPIO Services
  @param[in]  DebounceTime   Debounce Time in microseconds
                             If Debounce Time = 0, Debouncer feature will be disabled
                             Function will set DebounceTime argument to rounded supported value

   @retval Status
**/
EFI_STATUS
PtlPcdGpioSetDebounceTimerforPwrBtn (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN UINT32                DebounceTime
  )
{
  EFI_STATUS        Status;
  BOOLEAN           IsValid;
  GPIOV2_PAD_OWN    Ownership;
  GPIOV2_PAD        PwrBtnPad;
    PwrBtnPad = GPIOV2_PTL_PCD_XXGPP_V_3;
  GpioServices->IsPadValid (GpioServices, PwrBtnPad , &IsValid);
  if (!IsValid) {
    return EFI_INVALID_PARAMETER;
  }

  GpioServices->GetOwnership (GpioServices, PwrBtnPad, &Ownership);
  if (Ownership != GpioV2PadOwnHost) {
    return EFI_ACCESS_DENIED;
  }

  Status = GpioServices->SetDebounce (GpioServices, PwrBtnPad, DebounceTime);
  return Status;
}

//
// Timed GPIO pin mapping
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD_NATIVE_FUNCTION mPtlPcdTimeSyncToGpioMap[] =
{
  {GPIOV2_PTL_PCD_MUXING__XXGPP_B_22__TIME_SYNC_0, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame},
  {GPIOV2_PTL_PCD_MUXING__XXGPP_B_23__TIME_SYNC_1, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame}
};

/**
  This function sets Time Sync Gpio into native mode

  @param[in]  GpioServices      GPIO Services
  @param[in]  Index             Pin Index

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableTimeSync (
  IN  GPIOV2_SERVICES          *GpioServices,
  IN  UINT32                   Index
  )
{
  EFI_STATUS                  Status;
  GPIOV2_PAD_NATIVE_FUNCTION  *TimeSyncToGpioMap;
  UINT32                      TimeSyncToGpioMapSize;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }
    TimeSyncToGpioMap = mPtlPcdTimeSyncToGpioMap;
    TimeSyncToGpioMapSize = ARRAY_SIZE (mPtlPcdTimeSyncToGpioMap);
  if (Index < TimeSyncToGpioMapSize) {
    Status = GpioServices->SetPadMode (GpioServices, TimeSyncToGpioMap[Index].Pad, TimeSyncToGpioMap[Index].Mode);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    Status = GpioServices->SetIostandbyConfig (GpioServices, TimeSyncToGpioMap[Index].Pad, TimeSyncToGpioMap[Index].IosState, TimeSyncToGpioMap[Index].IosTerm);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
    return EFI_SUCCESS;
  }

  return EFI_UNSUPPORTED;
}

//
// CPU_C10_GATE pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD_NATIVE_FUNCTION mPtlPcdCpuC10GatePin = {GPIOV2_PTL_PCD_XXGPP_H_13, GpioV2PadModeNative1, GpioV2IosStateMasked, GpioV2IosTermSame};
/**
  This function sets CPU C10 Gate pins into native mode

  @param[in]  GpioServices        GPIO Services

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableCpuC10GatePin (
  IN  GPIOV2_SERVICES     *GpioServices
  )
{
  EFI_STATUS                  Status;
  GPIOV2_PAD_NATIVE_FUNCTION  CpuC10GateGpio;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }
    CpuC10GateGpio = mPtlPcdCpuC10GatePin;
  Status = GpioServices->SetPadMode (GpioServices, CpuC10GateGpio.Pad, CpuC10GateGpio.Mode);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = GpioServices->SetIostandbyConfig (GpioServices, CpuC10GateGpio.Pad, CpuC10GateGpio.IosState, CpuC10GateGpio.IosTerm);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

//
// VRALERTB pin
//
GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD_NATIVE_FUNCTION mPtlPcdVrAlertbPin = {GPIOV2_PTL_PCD_XXGPP_E_2, GpioV2PadModeNative2, GpioV2IosStateMasked, GpioV2IosTermSame};

/**
  This function sets VRALERTB pin into native mode

  @param[in]  GpioServices        GPIO Services

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableVrAlert (
  IN  GPIOV2_SERVICES     *GpioServices
  )
{
  EFI_STATUS Status;
  GPIOV2_PAD_NATIVE_FUNCTION VrAlertbPin;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }
    VrAlertbPin = mPtlPcdVrAlertbPin;
  Status = GpioServices->SetPadMode (GpioServices, VrAlertbPin.Pad, VrAlertbPin.Mode);
  if (EFI_ERROR(Status)) {
    return Status;
  }
  Status = GpioServices->SetIostandbyConfig (GpioServices, VrAlertbPin.Pad, VrAlertbPin.IosState, VrAlertbPin.IosTerm);
  if (EFI_ERROR(Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

//
// GPIO signals for SMBUS
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdSmbusGpioPadFunction[] =
{
  GPIOV2_SIGNAL_SMB_CLK, // SMB_CLK
  GPIOV2_SIGNAL_SMB_DATA // SMB_DATA
};

//
// SMBUS Alert signal
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdSmbusAlertGpioPadFunction =  GPIOV2_SIGNAL_SMB_ALERT_B;

/**
  This procedure sets Smbus gpio pads into native mode

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioEnableSmbus (
  IN GPIOV2_SERVICES  *GpioServices
  )
{
  EFI_STATUS  Status;
  UINT8       Index;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  for (Index = 0; Index < ARRAY_SIZE (mPtlPcdSmbusGpioPadFunction); Index++) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdSmbusGpioPadFunction[Index], 0);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }
  return EFI_SUCCESS;
}

/**
  This procedure sets Smbus Alert gpio pads into native mode

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioEnableSmbusAlert (
  IN GPIOV2_SERVICES  *GpioServices
  )
{
  EFI_STATUS  Status;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdSmbusAlertGpioPadFunction, 0);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }
  return EFI_SUCCESS;
}

//
// GPIO SPI IO pins for Touch Host Controller [SPI1 or SPI2][pin: IO_0/IO_1/IO_2/IO_3]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcSpiGpioIo[][4] =
{
  {
    GPIOV2_SIGNAL_THC_IO0(0),
    GPIOV2_SIGNAL_THC_IO1(0),
    GPIOV2_SIGNAL_THC_IO2(0),
    GPIOV2_SIGNAL_THC_IO3(0),
  },
  {
    GPIOV2_SIGNAL_THC_IO0(1),
    GPIOV2_SIGNAL_THC_IO1(1),
    GPIOV2_SIGNAL_THC_IO2(1),
    GPIOV2_SIGNAL_THC_IO3(1),
  }
};

/**
  This function provides SPI IO pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  IoIndex                   IoIndex Valid from 0 (SPI_IO_0) to 3 (SPI_IO_3)

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiIo (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN  UINT32               SpiIndex,
  IN  UINT32               IoIndex
  )
{
  EFI_STATUS   Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (SpiIndex < ARRAY_SIZE (mPtlPcdThcSpiGpioIo) && IoIndex < ARRAY_SIZE (mPtlPcdThcSpiGpioIo[SpiIndex])) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdThcSpiGpioIo[SpiIndex][IoIndex], 0);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    return EFI_SUCCESS;
  }
  return EFI_UNSUPPORTED;
}

//
// GPIO SPI CS pins for Touch Host Controller [SPI1 or SPI2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcSpiCs[2] =
{
  GPIOV2_SIGNAL_THC_CSB(0),
  GPIOV2_SIGNAL_THC_CSB(1),
};

/**
  This function provides SPI ChipSelect pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiCs (
  IN GPIOV2_SERVICES      *GpioServices,
  IN UINT32               SpiIndex
  )
{
  EFI_STATUS  Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcSpiCs)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdThcSpiCs[SpiIndex], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

//
// GPIO SPI CLK pins for Touch Host Controller [SPI1 or SPI2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcSpiClk[2] =
{
  GPIOV2_SIGNAL_THC_CLK(0),
  GPIOV2_SIGNAL_THC_CLK(1),
};

/**
  This function provides SPI Clock pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiClk (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                SpiIndex
  )
{
  EFI_STATUS  Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcSpiClk)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdThcSpiClk[SpiIndex], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcDSyncGpioPad[] =
{
  GPIOV2_SIGNAL_THC_DSYNC (0), // THC_DSYNC_0
  GPIOV2_SIGNAL_THC_DSYNC (1)  // THC_DSYNC_1
};

/**
  This function programes SPI THC DSYNC pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiDSync (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                SpiIndex
  )
{
  EFI_STATUS  Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcDSyncGpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdThcDSyncGpioPad[SpiIndex], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

//
// GPIO SPI Reset pins for Touch Host Controller [SPI1 or SPI2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcSpiReset[2] =
{
  GPIOV2_SIGNAL_THC_RSTB(0),
  GPIOV2_SIGNAL_THC_RSTB(1),
};

/**
  This function provides SPI Reset pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[out] Pad                       Reset pad for a given SPI instance

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioGetThcSpiResetPin (
  IN  GPIOV2_SERVICES       *GpioServices,
  IN  UINT32                SpiIndex,
  OUT GPIOV2_PAD            *Pad
  )
{
  if (Pad == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcSpiReset)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  *Pad = PtlPcdGpioGetNativePadByFunction (GpioServices, mPtlPcdThcSpiReset[SpiIndex]);

  return EFI_SUCCESS;
}

/**
  This function configures SPI Reset pin for Touch Host Controller

  @param[in]  GpioServices              GPIO Services
  @param[in]  SpiIndex                  SPI1 or SPI2 - 0 or 1
  @param[in]  NativePin                 TRUE  - pin needs to be configured as native pad
                                        FALSE - pin needs to be configured as GPIO pad
  @param[in]  ResetPad                  Reset pad

  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcSpiReset (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 SpiIndex,
  IN BOOLEAN                NativePin,
  IN GPIOV2_PAD             ResetPad
  )
{
  EFI_STATUS              Status;
  GPIOV2_CONFIG           PadConfig;
  GPIOV2_PAD              Pad;
  GPIOV2_IOSTANDBY_STATE  IosState;
  GPIOV2_IOSTANDBY_TERM   IosTerm;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcSpiReset)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Pad = PtlPcdGpioGetNativePadByFunction (GpioServices, mPtlPcdThcSpiReset[SpiIndex]);
  Status = PtlPcdGpioGetFunctionIoStandbyConfig (GpioServices, mPtlPcdThcSpiReset[SpiIndex], &IosState, &IosTerm);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (NativePin) {
    Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdThcSpiReset[SpiIndex], 0);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  } else {
    ZeroMem (&PadConfig, sizeof (GPIOV2_CONFIG));
    PadConfig.Direction       = GpioV2DirOut;
    PadConfig.PadMode         = GpioV2PadModeGpio;
    PadConfig.HostOwn         = GpioV2HostOwnGpio;
    PadConfig.InterruptConfig = GpioV2IntDefault;
    PadConfig.ResetConfig     = GpioV2ResetHost;
    if (ResetPad != 0) {
      Pad = ResetPad;
    }

    Status = GpioServices->ConfigurePad (
                             GpioServices,
                             Pad,
                             &PadConfig
                             );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    Status = GpioServices->SetLockTx (
                             GpioServices,
                             Pad,
                             GpioV2Unlock
                             );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  Status = GpioServices->SetIostandbyConfig (
                           GpioServices,
                           Pad,
                           IosState,
                           IosTerm
                           );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_PAD mPtlPcdThcWotGpioPad[] = {
  GPIOV2_PTL_PCD_VGPIO_THC0,
  GPIOV2_PTL_PCD_VGPIO_THC1
};

/**
  This function provides GPIO Native Pad with encoded Wake on Touch native function for a given THC index

  @param[in]  ThcIndex            THC0 or THC1 - 0 or 1

  @retval     GpioPad             GPIO pad with encoded native function
**/
GPIOV2_PAD
PtlPcdGpioGetThcWotGpioPad (
  IN UINT8      ThcIndex
  )
{
    return mPtlPcdThcWotGpioPad[ThcIndex];
}

/**
  This function configures THC Wake On Touch int pin

  @param[in]  GpioServices        GPIO Services
  @param[in]  SpiIndex            SPI1 or SPI2 - 0 or 1

  @retval Status
**/
EFI_STATUS
PtlPcdGpioConfigureThcWakeOnTouchPin (
  IN  GPIOV2_SERVICES      *GpioServices,
  IN  UINT32               SpiIndex
  )
{
  EFI_STATUS    Status;
  GPIOV2_CONFIG PadConfig;

  if (SpiIndex >= ARRAY_SIZE (mPtlPcdThcWotGpioPad)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  ZeroMem (&PadConfig, sizeof (PadConfig));

  PadConfig.PadMode         = GpioV2PadModeGpio;
  PadConfig.Direction       = GpioV2DirIn;
  PadConfig.HostOwn         = GpioV2HostOwnGpio;
  PadConfig.InterruptConfig = GpioV2IntApic | GpioV2IntLevel;
  PadConfig.ResetConfig     = GpioV2ResetHost;

  //
  // Unlock GPIO pad due to Host Software Pad Ownership is GPIO Driver mode.
  // It won't work in FSP API mode, Unlock/Lock are only supported for FSP wrapper instance.
  //
  Status = GpioServices->SetLock (GpioServices, mPtlPcdThcWotGpioPad[SpiIndex], GpioV2Unlock);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GpioServices->ConfigurePad (GpioServices, mPtlPcdThcWotGpioPad[SpiIndex], &PadConfig);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = GpioServices->SetIostandbyConfig (
                           GpioServices,
                           mPtlPcdThcWotGpioPad[SpiIndex],
                           GpioV2IosStateTxDisRxEn,
                           GpioV2IosTermDefault
                           );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

//
// GPIO THC I2C SDA pins for Touch Host Controller [I2C1 or I2C2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdSocThcI2cSda[2] =
{
  GPIOV2_SIGNAL_THC_I2C_SDA(0),
  GPIOV2_SIGNAL_THC_I2C_SDA(1),
};

//
// GPIO THC I2C SCL pins for Touch Host Controller [I2C1 or I2C2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdSocThcI2cScl[2] =
{
  GPIOV2_SIGNAL_THC_I2C_SCL(0),
  GPIOV2_SIGNAL_THC_I2C_SCL(1),
};

/**
  This function provides I2C SCL/SDA pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableQuickI2c (
  IN GPIOV2_SERVICES       *GpioServices,
  IN UINT32                I2cIndex
  )
{
  EFI_STATUS  Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if ((I2cIndex >= ARRAY_SIZE (mPtlPcdSocThcI2cSda)) || (I2cIndex >= ARRAY_SIZE (mPtlPcdSocThcI2cScl))) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdSocThcI2cSda[I2cIndex], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdSocThcI2cScl[I2cIndex], 0);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  return EFI_SUCCESS;
}

//
// GPIO I2C Reset pins for Touch Host Controller [I2C1 or I2C2]
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdThcI2cReset[2] =
{
  GPIOV2_SIGNAL_THC_RSTB(0),
  GPIOV2_SIGNAL_THC_RSTB(1),
};

/**
  This function configures I2c Reset pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @param[in]  NativePin                 TRUE  - pin needs to be configured as native pad
                                        FALSE - pin needs to be configured as GPIO pad
  @param[in]  ResetPad                  Reset pad
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcI2cReset (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 I2cIndex,
  IN BOOLEAN                NativePin,
  IN GPIOV2_PAD             ResetPad
  )
{
  EFI_STATUS              Status;
  GPIOV2_CONFIG           PadConfig;
  GPIOV2_PAD              Pad;
  GPIOV2_IOSTANDBY_STATE  IosState;
  GPIOV2_IOSTANDBY_TERM   IosTerm;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  if (I2cIndex >= ARRAY_SIZE (mPtlPcdThcI2cReset)) {
    ASSERT (FALSE);
    return EFI_UNSUPPORTED;
  }

  Pad = PtlPcdGpioGetNativePadByFunction (GpioServices, mPtlPcdThcI2cReset[I2cIndex]);
  Status = PtlPcdGpioGetFunctionIoStandbyConfig (GpioServices, mPtlPcdThcI2cReset[I2cIndex], &IosState, &IosTerm);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  if (NativePin) {
    Status = PtlPcdGpioSetNativePadByFunction  (GpioServices, mPtlPcdThcI2cReset[I2cIndex], 0);
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  } else {
    ZeroMem (&PadConfig, sizeof (GPIOV2_CONFIG));
    PadConfig.Direction       = GpioV2DirOut;
    PadConfig.PadMode         = GpioV2PadModeGpio;
    PadConfig.HostOwn         = GpioV2HostOwnGpio;
    PadConfig.InterruptConfig = GpioV2IntDefault;
    PadConfig.ResetConfig     = GpioV2ResetHost;
    if (ResetPad != 0) {
      Pad = ResetPad;
    }

    Status = GpioServices->ConfigurePad (
                             GpioServices,
                             Pad,
                             &PadConfig
                             );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }

    Status = GpioServices->SetLockTx (
                             GpioServices,
                             Pad,
                             GpioV2Unlock
                             );
    if (EFI_ERROR (Status)) {
      ASSERT_EFI_ERROR (Status);
      return Status;
    }
  }

  Status = GpioServices->SetIostandbyConfig (
                           GpioServices,
                           Pad,
                           IosState,
                           IosTerm
                           );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function configures I2c Int pin for Touch Host Controller
  @param[in]  GpioServices              GPIO Services
  @param[in]  I2cIndex                  I2C1 or I2C2 - 0 or 1
  @param[in]  UINT32 InterruptPinMuxing Thc Port Interrupt Pin Muxing
  @retval     Status
**/
EFI_STATUS
PtlPcdGpioEnableThcI2cInt (
  IN GPIOV2_SERVICES        *GpioServices,
  IN UINT32                 I2cIndex,
  IN UINT32                 InterruptPinMuxing
  )
{
  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }

  PtlPcdGpioSetNativePadByFunction (
    GpioServices,
    GPIOV2_SIGNAL_THC_SPI_INT (I2cIndex),
    InterruptPinMuxing
    );

  GpioServices->SetInputInversion (
                  GpioServices,
                  PtlPcdGpioGetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_THC_SPI_INT (I2cIndex)),
                  GpioV2InputInversionDisable
                  );

  return EFI_SUCCESS;
}

/**
  The function performs GPIO Power Management programming.

  @param[in] GpioServices            GPIO Services

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   GPIO V2 Services were not found
**/
EFI_STATUS
PtlPcdGpioConfigurePm (
  IN GPIOV2_SERVICES    *GpioServices
  )
{
  GPIOV2_INTERFACE        *GpioInterface;
  UINT32                  ComIdx;
  UINT32                  MiscCfg;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  for (ComIdx = 0; ComIdx < GpioInterface->Private.CommunitiesNum; ComIdx++) {
    GpioInterface->Public.GetMiscCfg (&(GpioInterface->Public), ComIdx, &MiscCfg);

    //
    // Enable MISCCFG.GPVNNREQEn, MISCCFG.GPPGCBDPCGEn, MISCCFG.GPSIDEDPCGEn, MISCCFG.GPRCOMPCDLCGEn,
    // MISCCFG.GPRTCDLCGEn, MISCCFG.GPDLCGEn and MISCCFG.GPDPCGEn for GPIO communities
    //
    MiscCfg |= (B_GPIO_PCR_MISCCFG_GPVNNREQEN     |
                B_GPIO_PCR_MISCCFG_GPPGCBDPCGEN   |
                B_GPIO_PCR_MISCCFG_GPSIDEDPCGEN   |
                B_GPIO_PCR_MISCCFG_GPRCOMPCDLCGEN |
                B_GPIO_PCR_MISCCFG_GPRTCDLCGEN    |
                B_GPIO_PCR_MISCCFG_GPDLCGEN       |
                B_GPIO_PCR_MISCCFG_GPDPCGEN);

    GpioInterface->Public.SetMiscCfg (&(GpioInterface->Public), ComIdx, MiscCfg);
  }

  return EFI_SUCCESS;
}

/**
  This procedure will set GPIO Driver IRQ number

  @param[in] GpioServices            GPIO Services
  @param[in] Irq                     Irq number

  @retval    EFI_SUCCESS             The function completed successfully
  @retval    EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioSetIrq (
  IN GPIOV2_SERVICES    *GpioServices,
  IN UINT8              Irq
  )
{
  GPIOV2_INTERFACE        *GpioInterface;
  UINT32                  ComIdx;
  UINT32                  MiscCfg;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  for (ComIdx = 0; ComIdx < GpioInterface->Private.CommunitiesNum; ComIdx++) {
    GpioInterface->Public.GetMiscCfg (&(GpioInterface->Public), ComIdx, &MiscCfg);

    MiscCfg &= (UINT32)~(B_GPIO_PCR_MISCCFG_IRQ_ROUTE);
    MiscCfg |= (UINT32)Irq << N_GPIO_PCR_MISCCFG_IRQ_ROUTE;

    GpioInterface->Public.SetMiscCfg (&(GpioInterface->Public), ComIdx, MiscCfg);
  }

  return EFI_SUCCESS;
}

//
// USB2 OC pins
//
GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdUsbOcGpioPins[] =
{
  GPIOV2_SIGNAL_USB_OC (0),  // USB_OC_0
  GPIOV2_SIGNAL_USB_OC (1),  // USB_OC_1
  GPIOV2_SIGNAL_USB_OC (2),  // USB_OC_2
  GPIOV2_SIGNAL_USB_OC (3)   // USB_OC_3
};


GLOBAL_REMOVE_IF_UNREFERENCED GPIOV2_VWOC_FUNCTION mPtlPcdUsbVwOcGpioPins[] =
{
  {GPIOV2_PTL_PCD_VGPIO_USB_0, GPIOV2_PTL_PCD_VGPIO_USB_1},   // USB_VW_OC_PIN_0
  {GPIOV2_PTL_PCD_VGPIO_USB_2, GPIOV2_PTL_PCD_VGPIO_USB_3},   // USB_VW_OC_PIN_1
  {GPIOV2_PTL_PCD_VGPIO_USB_4, GPIOV2_PTL_PCD_VGPIO_USB_5},   // USB_VW_OC_PIN_2
  {GPIOV2_PTL_PCD_VGPIO_USB_6, GPIOV2_PTL_PCD_VGPIO_USB_7}    // USB_VW_OC_PIN_3
};

/**
  This procedure will Enable USB Virtual Wire Overcurrent pin by cycling NAF_VWE bit

  @param[in]  GpioServices         Gpio Services
  @param[in]  GPIO_VWOC_FUNCTION   GpioPad

**/
STATIC
VOID
GpioCycleNafVweBit (
  IN GPIOV2_SERVICES      *GpioServices,
  IN CONST GPIOV2_PAD     GpioPad
  )
{
  UINT32                      RegisterValue;
  UINT32                      RegisterOffset;
  EFI_STATUS                  Status;
  GPIOV2_INTERFACE            *GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);
  UINT32                      CommunityIndex;

  //
  // Upon PLTRST# de-assertion, BIOS always clear the OC pin's
  // Native Function Virtual wire enable (NAF_VWE) bit, followed by
  // setting the same register bit
  //
  Status = GpioInterface->Public.GetRegisterOffset (
    &(GpioInterface->Public),
    GpioV2Dw0Reg,
    GpioPad,
    &RegisterOffset
  );
  if (!EFI_ERROR (Status)) {
    DEBUG ((DEBUG_INFO, "[GPIOV2] %a start\n", __FUNCTION__));
    CommunityIndex = GPIOV2_PAD_GET_COMMUNITY_INDEX (GpioPad);

    // Reading register value
    RegisterValue = GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Read32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset
    );

    // Reset Bit
    RegisterValue &= ~B_PCH_GPIO_NAF_VWE;
    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      RegisterValue
    );

    // Set Bit
    RegisterValue |= B_PCH_GPIO_NAF_VWE;
    GpioInterface->Private.CommunityAccess[CommunityIndex].Access.Write32 (
      &(GpioInterface->Private.CommunityAccess[CommunityIndex].Access),
      RegisterOffset,
      RegisterValue
    );
  }

  return;
}

/**
  This function enables USB OverCurrent pins by setting
  USB1 OCB pins into native mode

  @param[in]  GpioServices        Gpio Services
  @param[in]  OcPinNumber         USB OC pin number
  @param[in]  NativeActiveFlag    Native Function Virtual Wire Enable

  @retval     EFI_SUCCESS         The function completed successfully
  @retval     EFI_UNSUPPORTED     Unsupported operation
**/
EFI_STATUS
PtlPcdGpioEnableUsbOverCurrent (
  IN GPIOV2_SERVICES     *GpioServices,
  IN UINTN               OcPinNumber,
  IN UINT8               NativeActiveFlag
  )
{
  GPIOV2_VWOC_FUNCTION      VwOcGpio;
  UINT8                     PhysicalPinTableSize;
  UINT8                     VirtualPinTableSize;
  EFI_STATUS                Status;
  GPIOV2_PAD                GpioPad;
  UINT32                    UsbOcSignal;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }
    PhysicalPinTableSize = ARRAY_SIZE (mPtlPcdUsbOcGpioPins);
    VirtualPinTableSize  = ARRAY_SIZE (mPtlPcdUsbVwOcGpioPins);

  if (OcPinNumber >= PhysicalPinTableSize) {
    if ((OcPinNumber - PhysicalPinTableSize) >= VirtualPinTableSize) {
      return EFI_UNSUPPORTED;
    }
      VwOcGpio = mPtlPcdUsbVwOcGpioPins [OcPinNumber - PhysicalPinTableSize];
    GpioCycleNafVweBit (GpioServices, VwOcGpio.OcRxPad);
    GpioCycleNafVweBit (GpioServices, VwOcGpio.OcTxPad);

    return EFI_SUCCESS;
  }

    UsbOcSignal = mPtlPcdUsbOcGpioPins[OcPinNumber];
  // Set NafVweBit for PhysicalOcPin if NativeActiveFlag is TRUE
  if (NativeActiveFlag) {
    GpioPad = PtlPcdGpioGetNativePadByFunction (GpioServices, UsbOcSignal);
    GpioCycleNafVweBit (GpioServices, GpioPad);
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, UsbOcSignal, 0);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function sets PCHHOT pin into native mode

  @param[in]  GpioServices            Gpio Services

  @retval     EFI_SUCCESS             The function completed successfully
  @retval     EFI_INVALID_PARAMETER   Invalid parameter
**/
EFI_STATUS
PtlPcdGpioEnablePchHot (
  IN GPIOV2_SERVICES     *GpioServices
  )
{
  EFI_STATUS Status;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_SMB_PCH_HOT_B, 0);
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}


/**
  This function performs initial IO Standby State related configurations
**/
VOID
PtlPcdGpioConfigureIoStandbyState (
  VOID
  )
{
  UINT8                   SignalIndex;
  UINT32                  NativeSignalIndex;
  GPIOV2_PAD_MODE         GpioPadMode;
  EFI_STATUS              Status;
  GPIOV2_SERVICES         *GpioServices;
  GPIOV2_INTERFACE        *GpioInterface;
  UINT32                  CommunityIndex;
  UINT32                  GroupIndex;
  UINT32                  PadIndex;

  if (GpioOverrideLevel1Enabled ()) {
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
    if (Status != EFI_SUCCESS) {
      DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: retrieving GpioServices(%a) failed (Status: %d)\n", __FUNCTION__, GPIO_HID_PTL_PCD_P, Status));
      return;
    }

  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  for (CommunityIndex = 0; CommunityIndex < GpioInterface->Private.CommunitiesNum; CommunityIndex++) {
    for (GroupIndex = 0; GroupIndex < GpioInterface->Private.Communities[CommunityIndex].GroupsNum; GroupIndex++) {
      for (PadIndex = 0; PadIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadsNum; PadIndex++) {
        GpioInterface->Public.GetPadMode (&(GpioInterface->Public), GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex], &GpioPadMode);
        if (GpioPadMode == GpioV2PadModeGpio) {
          continue;
        }
        for (SignalIndex = 0; SignalIndex < GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignalsNum; SignalIndex++) {
          if (GpioPadMode == GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[SignalIndex].PadMode) {
            for (NativeSignalIndex = 0; NativeSignalIndex < GpioInterface->Private.NativeSignalDataNum; NativeSignalIndex++) {
              if (GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].PadDataArray[PadIndex].PadSignals[SignalIndex].Signal.Value == GpioInterface->Private.NativeSignalData[NativeSignalIndex].Signal.Value) {
                if (GpioInterface->Private.NativeSignalData[NativeSignalIndex].IosState == GpioV2IosStateDefault){
                  break;
                }
                GpioInterface->Public.SetIostandbyConfig (
                  &(GpioInterface->Public),
                  GpioInterface->Private.Communities[CommunityIndex].Groups[GroupIndex].Pads[PadIndex],
                  GpioInterface->Private.NativeSignalData[NativeSignalIndex].IosState,
                  GpioInterface->Private.NativeSignalData[NativeSignalIndex].IosTerm
                );
                break;
              }
            }
            break;
          }
        }
      }
    }
  }
}

/**
  This function enables Link Down functionality
  on platform specific GPIO

  @param[in]  GpioServices        GPIO Services

  @retval     EFI_SUCCESS         If enabling Link Down functionality is a success
  @retval     EFI_NOT_FOUND       If GpioService is not found
**/
EFI_STATUS
PtlPcdLinkDownEnable (
  IN GPIOV2_SERVICES    *GpioServices
  )
{
  EFI_STATUS      Status;

  if (GpioServices == NULL) {
    ASSERT (FALSE);
    return EFI_INVALID_PARAMETER;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, GPIOV2_SIGNAL_PCIE_LNK_DOWN, 0);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: [GPIOV2]: enabling link down functionality failed\n", __FUNCTION__));
  }

  return Status;
}

GLOBAL_REMOVE_IF_UNREFERENCED UINT32 mPtlPcdImguClkOutGpioPad[] =
{
  GPIOV2_SIGNAL_IPU_IMGCLKOUT (0), // IMGCLKOUT_0
  GPIOV2_SIGNAL_IPU_IMGCLKOUT (1), // IMGCLKOUT_1
  GPIOV2_SIGNAL_IPU_IMGCLKOUT (2)  // IMGCLKOUT_2
};

/**
  This function enables IMG CLKOUT native pin

  @param[in] GpioServices          Gpio Services
  @param[in] ImguClkOutPinIndex    The index of IMGU CLKOUT native pin

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableImguClkOut (
  IN  GPIOV2_SERVICES*   GpioServices,
  IN  UINT8              ImguClkOutPinIndex
  )
{
  EFI_STATUS                Status;

  if (GpioOverrideLevel1Enabled ()) {
    return EFI_SUCCESS;
  }


  if (ImguClkOutPinIndex >= ARRAY_SIZE (mPtlPcdImguClkOutGpioPad)) {
    return EFI_UNSUPPORTED;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, mPtlPcdImguClkOutGpioPad[ImguClkOutPinIndex], 0);
  DEBUG ((DEBUG_ERROR, "%a  PtlPcdGpioSetNativePadByFunction  =%r \n", __FUNCTION__, Status));
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  return EFI_SUCCESS;
}

/**
  This function Implements Tcss LSx Pins

  @param[in] Index   TCSS USBC Port Index
**/
VOID
PtlPcdTcssLsxPinEnable (
  IN  UINT8            Index
  )
{
  GPIOV2_SERVICES      *GpioServices;
  EFI_STATUS           Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Configure Display GPIO's\n"));
    return;
  }

  //
  // According the TCSS Port policy set the LSx Pins
  //
  PtlPcdGpioEnableTbtLsxInterface (GpioServices, GpioV2TbtLsxDdi1 + Index);

  return;
}

/**
  This function implements TCSS LSx_OE pins

  @param[in] Index   TCSS USBC Port Index

**/
VOID
PtlPcdTcssLsxOePinEnable (
  IN  UINT8            Index
  )
{
  GPIOV2_SERVICES  *GpioServices;
  EFI_STATUS       Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to Configure LSx_OE GPIOs\n"));
    return;
  }

  //
  // According the TCSS port policy set the LSx_OE pins
  //
  Status = PtlPcdGpioEnableTbtLsxOeInterface (GpioServices, GpioV2TbtLsxOe0 + Index);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Failed to configure USB4 20G retimerless USBC port [%d] GPIO pin. Status: %r\n", Index, Status));
  }
}

/**
  This function performs basic initialization IOM for AUX Layout in PEI phase after Policy produced at Pre-Mem phase.
  For those GPIO pins used for DP Aux orientation control and enabled, BIOS is responsible to -
  1. Configure Pad Mode (PMode) to function# associated with IOM_GPP*_*
     The Pad Mode here could be various per PCH design.
  2. BIOS Provide the following information for the DP Aux orientation bias control for provide the GPIO VwIndex,
     Bit Position, and VW Index information to IOM FW.
     I. GPIO endpoint IOSF-SB port ID (Pch Community port ID)
     II. VW index and data bit position
  @param[in]  GpioPad    - GPIO Pad
  @param[out] PchPortId  - GPIO Pad mapping GPCOM PortID number
  @param[out] Bits       - GPIO Pad mapping Bit position
  @param[out] VwIndex    - GPIO Pad mapping Virtual Wire Index number
**/
VOID
PtlGpioIomAuxOriSetting (
  UINT32   GpioPadNum,
  UINT16   *PortId,
  UINT8    *Bits,
  UINT8    *VwIndex
  )
{
  EFI_STATUS                  Status;
  GPIOV2_SERVICES             *GpioServices;
  GPIOV2_INTERFACE            *GpioInterface;
  GPIOV2_PAD_DATA             *GpioPadData;
  UINT32                      Index;
  GPIOV2_NATIVE_SIGNAL_DATA   SignalData;
  UINT32                      ComIdx;

  DEBUG ((DEBUG_INFO, "%a Start\n", __FUNCTION__));

  DEBUG ((DEBUG_INFO, "%a GpioPadNum: %08x\n", __FUNCTION__, GpioPadNum));
  if ((GpioPadNum == 0) || (PortId == NULL) || (Bits == NULL) || (VwIndex == NULL)) {
    //
    // The paratemeter data is invalid.
    //
    return;
  }

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR getting GpioServices\n", __FUNCTION__));
    return;
  }
  GpioInterface = GPIOV2_INTERFACE_FROM_PUBLIC (GpioServices);

  Status = GpioServices->GetPadDataForPad (GpioServices, GpioPadNum, &GpioPadData);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR getting pad data for gpio 0x%x\n", __FUNCTION__, GpioPadNum));
    return;
  }

  for (Index = 0; Index < GpioPadData->PadSignalsNum; Index++) {
    if (GpioPadData->PadSignals[Index].Signal.Fields.Interface == GPIOV2_PERIPHERAL_INTERFACE_IOM) {
      Status = GpioServices->SetPadMode (GpioServices, GpioPadNum, GpioPadData->PadSignals[Index].PadMode);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR failed to set the pad mode\n", __FUNCTION__));
        return;
      }

      Status = GpioServices->GetSignalInfoBySignal (GpioServices, GpioPadData->PadSignals[Index].Signal.Value, &SignalData);
      if (EFI_ERROR (Status)) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR getting signal info\n", __FUNCTION__));
        return;
      }

      ComIdx = GPIOV2_PAD_GET_COMMUNITY_INDEX(GpioPadNum);
      if (ComIdx >= GpioInterface->Private.CommunitiesNum) {
        DEBUG ((DEBUG_ERROR, "[GPIOV2][%a]: ERROR wrong community index\n", __FUNCTION__));
        return;
      }

      *PortId = (UINT16)GpioInterface->Private.Communities[ComIdx].Pid;
      *Bits = (UINT8)SignalData.BitPosition;
      *VwIndex = (UINT8)SignalData.VwIndex;

      break;
    }
  }

  DEBUG ((DEBUG_INFO, "%a End\n", __FUNCTION__));
}

/**
  Configure Display GPIO's

  @param[in]  PadFunction         PadMode for a specific native signal. Please refer to GpioV2Signals
  @param[in]  PinMux              GPIO Native pin mux platform config.
**/
EFI_STATUS
EFIAPI
ConfigureDisplayGpio (
  IN  UINT32           PadFunction,
  IN  UINT32           PinMux
  )
{
  GPIOV2_SERVICES    *GpioServices;
  EFI_STATUS         Status;

    Status = GpioV2GetAccess (GPIO_HID_PTL_PCD_P, 0, &GpioServices);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = PtlPcdGpioSetNativePadByFunction (GpioServices, PadFunction, PinMux);

  if (PadFunction == GPIOV2_SIGNAL_DDSP_HPDALV) {
    Status = GpioServices->SetTerminationConfig (GpioServices, PtlPcdGpioGetNativePadByFunctionAndPinMux (GpioServices, GPIOV2_SIGNAL_DDSP_HPDALV, 0), GpioV2TermNone);
  }

  return Status;
}

/**
  This function sets TSN0 into native mode

  @param[in]  GpioServices   Gpio Services
  @param[in]  GpioServices   TSN controller index

  @retval Status
**/
EFI_STATUS
PtlPcdGpioEnableTsnPins (
  IN  GPIOV2_SERVICES   *GpioServices,
  IN  UINT8             TsnIndex
  )
{

  return EFI_SUCCESS;
}
