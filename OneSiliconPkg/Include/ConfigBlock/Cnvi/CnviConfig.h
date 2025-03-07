/** @file
  CNVi policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2016 Intel Corporation.

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
#ifndef _CNVI_CONFIG_H_
#define _CNVI_CONFIG_H_

#include <ConfigBlock.h>

#define CNVI_CONFIG_REVISION 5
extern EFI_GUID gCnviConfigGuid;

#define CNVI_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gCnviPreMemConfigGuid;

#pragma pack (push,1)

/**
  CNVi Mode options
**/
typedef enum {
  CnviModeDisabled = 0,
  CnviModeAuto
} CNVI_MODE;

/**
  CNVi Bluetooth interface options
**/
typedef enum {
  CnviBtIfUsb  = 1,
  CnviBtIfPci  = 2
} CNVI_BT_INTERFACE;

/**
  CNVi Bluetooth Audio Offload Interface
**/
typedef enum {
  CnviBtAudioOffloadI2s = 0,
  CnviBtAudioOffloadSoundwire = 1
} CNVI_BT_OFFLOAD_INTERFACE;


/**
  CNVi signals pin muxing settings. If signal can be enable only on a single pin
  then this parameter is ignored by RC. Refer to GPIO_*_MUXING_CNVI_* in GpioPins*.h
  for supported settings on a given platform
**/
typedef struct {
  UINT32 RfReset; ///< RF_RESET# Pin mux configuration. Refer to GPIO_*_MUXING_CNVI_RF_RESET_*
  UINT32 Clkreq;  ///< CLKREQ Pin mux configuration. Refer to GPIO_*_MUXING_CNVI_*_CLKREQ_*
} CNVI_PIN_MUX;

/**
  The CNVI_CONFIG block describes the expected configuration of the CNVi IP.

  <b>Revision 1</b>: - Initial version.
  <b>Revision 2</b>: - Add DdrRfim.
  <b>Revision 3</b>: - Add WifiCore.
  <b>Revision 4</b>: - Move DdrRfim to Pre-Mem config.
  <b>Revision 5</b>: - Add WwanCoex.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;      ///< Config Block Header
  /**
    This option allows for automatic detection of Connectivity Solution.
    Auto Detection assumes that CNVi will be enabled when available;
    Disable allows for disabling CNVi.
    CnviModeDisabled = Disabled,
    <b>CnviModeAuto = Auto Detection</b>
  **/
  UINT32 Mode                     :  1;
  UINT32 BtCore                   :  1; ///< The option to turn ON or OFF the BT Core. 0: Disabled, <b>1: Enabled</b>
  /**
    The option to enable or disable BT Audio Offload.
    0: Disabled, <b>1: Enabled</b>
    @note This feature supports from Intel(R) Wireless-AX 22560
  **/
  UINT32 BtAudioOffload           :  1;
  UINT32 BtAudioOffloadInterface  :  1;
  UINT32 WifiCore                 :  1; ///< The option to turn ON or OFF the Wi-Fi Core. 0: Disabled, <b>1: Enabled</b>
  /**
    This option allows configuration of BT interface to USB or PCIe
    Use CNVI_BT_INTERFACE enum for selection:
    <b>CnviBtIfUsb = BT over USB interface</b>
    CnviBtIfPci = BT over PCIe interface
    @note PCIe interface supports from Blazar generation
  **/
  UINT32 BtInterface              :  2;
  UINT32 WwanCoex                 :  1;
  UINT32 RsvdBits                 : 24;
  /**
    CNVi PinMux Configuration
    RESET#/CLKREQ to CRF, can have two alternative mappings, depending on board routing requirements.
  **/
  CNVI_PIN_MUX          PinMux;
} CNVI_CONFIG;


/**
  The CNVI_PREMEM_CONFIG block describes the expected configuration of the CNVi IP.

  <b>Revision 1</b>: - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER  Header;      ///< Config Block Header
  UINT8                DdrRfim;     ///< The option to enable or disable DDR RFI Mitigation. 0: Disabled, <b>1: Enabled</b>
  UINT8                RsvdByte[3];
} CNVI_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _CNVI_CONFIG_H_
