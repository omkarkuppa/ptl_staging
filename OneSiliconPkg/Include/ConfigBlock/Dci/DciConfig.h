/** @file
  Dci policy

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
#ifndef _DCI_CONFIG_H_
#define _DCI_CONFIG_H_

#include <ConfigBlock.h>

#define PCH_DCI_PREMEM_CONFIG_REVISION 1
extern EFI_GUID gDciPreMemConfigGuid;

#pragma pack (push,1)

typedef enum {
  DciDbcDisabled       = 0x0,
  DciDbcUsb2           = 0x1,
  DciDbcUsb3           = 0x2,
  DciDbcBoth           = 0x3,
  DciDbcNoChange       = 0x4,
  DciDbcMax
} DCI_DBC_MODE;

typedef enum {
  KetDisabled          = 0x0,
  KetEnabled           = 0x1,
  KetNoChange          = 0x2,
  KetMax
} KET_MODE;

/**
  ExI mailbox handler configuration registers
  Register default value: 0x00000000
**/
typedef enum {
  Emb0,
  Emb1,
  Emb2,
  Emb3,
  EmbMax
} DCI_MAILBOX_REG;

/**
  The PCH_DCI_PREMEM_CONFIG block describes policies related to Direct Connection Interface (DCI)

  <b>Revision 1</b>:
  - Initial version.
**/
typedef struct {
  CONFIG_BLOCK_HEADER   Header;         ///< Config Block Header
  /**
    DCI enable.
    Determine if to enable DCI debug from host.
    <b>0:Disabled</b>; 1:Enabled
  **/
  UINT8    DciEn;
  /**
    USB DbC enable mode.
    Disabled: Clear both USB2/3DBCEN; USB2: Set USB2DBCEN; USB3: Set USB3DBCEN; Both: Set both USB2/3DBCEN; No Change: Comply with HW value
    Refer to definition of DCI_USB_DBC_MODE for supported settings.
    0:Disabled; 1:USB2; 2:USB3; 3:Both; <b>4:No Change</b>
  **/
  UINT8    DciDbcMode;
  /**
    Determine if to enable or disable DCI clock request in lowest power state.
    0:Disabled; <b>1:Enabled</b>
  **/
  UINT8    DciClkEnable;
  /**
    Early trace is activated by default.
    Enable to keep early trace data and keep tracing; disable to stop tracing;
    When debug probe is connected and granted, force to no change, let host tool to set it.
    <b>0: Disable</b>; 1: Enable early trace; 2:No Change;
  **/
  UINT8    KeepEarlyTrace;
  UINT8    ReservedBytes[4];
} PCH_DCI_PREMEM_CONFIG;

#pragma pack (pop)

#endif // _DCI_CONFIG_H_
