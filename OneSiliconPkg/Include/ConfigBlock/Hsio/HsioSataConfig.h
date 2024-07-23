/** @file
  Hsio Sata policy

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2015 Intel Corporation.

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
#ifndef _HSIO_SATA_CONFIG_H_
#define _HSIO_SATA_CONFIG_H_

#define PCH_HSIO_SATA_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gHsioSataPreMemConfigGuid;

#pragma pack (push,1)

/**
  The PCH_HSIO_SATA_PORT_LANE describes HSIO settings for SATA Port lane
**/
typedef struct {
  //
  // HSIO Rx Eq
  //
  UINT32  HsioRxGen1EqBoostMagEnable   : 1;       ///< <b>0: Disable</b>; 1: Enable Receiver Equalization Boost Magnitude Adjustment Value override
  UINT32  HsioRxGen1EqBoostMag         : 6;       ///< SATA 1.5 Gb/sReceiver Equalization Boost Magnitude Adjustment value
  UINT32  HsioRxGen2EqBoostMagEnable   : 1;       ///< <b>0: Disable</b>; 1: Enable Receiver Equalization Boost Magnitude Adjustment Value override
  UINT32  HsioRxGen2EqBoostMag         : 6;       ///< SATA 3.0 Gb/sReceiver Equalization Boost Magnitude Adjustment value
  UINT32  HsioRxGen3EqBoostMagEnable   : 1;       ///< <b>0: Disable</b>; 1: Enable Receiver Equalization Boost Magnitude Adjustment Value override
  UINT32  HsioRxGen3EqBoostMag         : 6;       ///< SATA 6.0 Gb/sReceiver Equalization Boost Magnitude Adjustment value
  //
  // HSIO Tx Eq
  //
  UINT32  HsioTxGen1DownscaleAmpEnable : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 1.5 Gb/s TX Output Downscale Amplitude Adjustment value override
  UINT32  HsioTxGen1DownscaleAmp       : 6;       ///< SATA 1.5 Gb/s TX Output Downscale Amplitude Adjustment value
  UINT32  RsvdBits0                    : 4;       ///< Reserved bits

  UINT32  HsioTxGen2DownscaleAmpEnable : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 3.0 Gb/s TX Output Downscale Amplitude Adjustment value override
  UINT32  HsioTxGen2DownscaleAmp       : 6;       ///< SATA 3.0 Gb/s TX Output Downscale Amplitude Adjustment
  UINT32  HsioTxGen3DownscaleAmpEnable : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 6.0 Gb/s TX Output Downscale Amplitude Adjustment value override
  UINT32  HsioTxGen3DownscaleAmp       : 6;       ///< SATA 6.0 Gb/s TX Output Downscale Amplitude Adjustment
  UINT32  HsioTxGen1DeEmphEnable       : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 1.5 Gb/s TX Output De-Emphasis Adjustment Setting value override
  UINT32  HsioTxGen1DeEmph             : 6;       ///< SATA 1.5 Gb/s TX Output De-Emphasis Adjustment Setting

  UINT32  HsioTxGen2DeEmphEnable       : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 3.0 Gb/s TX Output De-Emphasis Adjustment Setting value override
  UINT32  HsioTxGen2DeEmph             : 6;       ///< SATA 3.0 Gb/s TX Output De-Emphasis Adjustment Setting
  UINT32  RsvdBits1                    : 4;       ///< Reserved bits

  UINT32  HsioTxGen3DeEmphEnable       : 1;       ///< <b>0: Disable</b>; 1: Enable SATA 6.0 Gb/s TX Output De-Emphasis Adjustment Setting value override
  UINT32  HsioTxGen3DeEmph             : 6;       ///< SATA 6.0 Gb/s TX Output De-Emphasis Adjustment Setting value override
  UINT32  RsvdBits2                    : 25;      ///< Reserved bits
} PCH_HSIO_SATA_PORT_LANE;

///
/// The PCH_HSIO_SATA_CONFIG block describes the HSIO configuration of the SATA controller.
///
typedef struct {
  CONFIG_BLOCK_HEADER   Header;                   ///< Config Block Header
  ///
  /// These members describe the configuration of HSIO for SATA lanes.
  ///
  PCH_HSIO_SATA_PORT_LANE        PortLane[PCH_MAX_SATA_PORTS];
} PCH_HSIO_SATA_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _HSIO_SATA_CONFIG_H_
