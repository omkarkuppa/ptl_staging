/** @file
 Header file for NV data structure definition.

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

#ifndef XMLCLI_SETUP_H
#define XMLCLI_SETUP_H

#define XMLCLI_SETUP_NAME      L"XmlCli"
// 8fd6d4e5-138c-4b5c-95ef-dd00d2689b8c
#define XMLCLI_FORM_SET_GUID   {0x8fd6d4e5, 0x138c, 0x4b5c, {0xef, 0x95, 0xdd, 0x00, 0xd2, 0x68, 0x9b, 0x8c}}
#define XMLCLI_SETUP_GUID      {0xeac04db0, 0xc87d, 0x40c7, {0x85, 0x82, 0x78, 0x58, 0x1a, 0xe1, 0x44, 0x4b}}

#ifndef AUTO_ID
#define AUTO_ID(x) x
#endif

#define KEY_XMLCLI_FORM_ID     1

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define XMLCLI_SETUP_REVISION  1


#pragma pack(push, 1)

typedef struct {
  UINT32  Revision;
  UINT8   XmlCliSupport;
  UINT8   PublishSetupPgPtr;
  UINT8   EnableXmlCliLite;
  UINT32  XmlCliDramCmosAddr;
} XMLCLI_SETUP;

#pragma pack(pop)


#endif  // XMLCLI_SETUP_H
