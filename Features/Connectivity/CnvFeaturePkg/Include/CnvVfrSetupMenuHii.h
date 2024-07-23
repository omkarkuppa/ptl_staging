/**@file
  The header file of CNV VFR Menu Setup Variable Hii.

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

@par Specification
**/

#ifndef _CNV_VFR_MENU_SETUP_VAR_HII_H_
#define _CNV_VFR_MENU_SETUP_VAR_HII_H_

// CNV Setup Form Guid
#define CNV_FEATURE_SETUP_GUID \
{ \
  0x5b8ed662, 0x3498, 0x4dec, {0xab, 0x00, 0x59, 0x96, 0x1f, 0x8d, 0xb1, 0x78 } \
}

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define CNV_SETUP_REVISION       1
#define CNV_SETUP_VARIABLE_NAME  L"CnvSetup"

#pragma pack(1)
// CNV VFR Menu Setup Variable
typedef struct {
  UINT8    Revision;
  //
  // BT Regulatory
  //
  UINT8    BluetoothSar;
  UINT8    BluetoothSarBr;
  UINT8    BluetoothSarEdr2;
  UINT8    BluetoothSarEdr3;
  UINT8    BluetoothSarLe;
  UINT8    BluetoothSarLe2Mhz;
  UINT8    BluetoothSarLeLr;

  //
  // Preboot BLE
  //
  UINT8    PrebootBleEnable;

  //
  // Discrete BT settings
  //
  UINT8    DiscreteBtModule;

  //
  // CNVi
  //
  UINT8    CnviMode;
  UINT8    CnviWifiCore;
  UINT8    CnviBtCore;
  UINT8    CnviBtInterface;
  UINT8    CnviBtAudioOffload;
  UINT8    CnviBtAudioOffloadInterface;
  UINT8    CnviBtAudioOffloadI2sInterfaceOnly;
  UINT8    CnviIsPresent;
  UINT8    CnviBtInterfaceUsbOnly;
  UINT8    CnviBtInterfaceUsbUpdate;

  UINT8    SkipVidDidCheck;

} CNV_VFR_CONFIG_SETUP;
#pragma pack ()

#endif
