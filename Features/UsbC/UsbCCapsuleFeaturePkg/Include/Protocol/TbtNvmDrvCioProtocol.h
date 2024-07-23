/** @file
CIO protocol defitions.

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

#ifndef TBT_NVM_DRV_CIO_PROTO_
#define TBT_NVM_DRV_CIO_PROTO_

#include <Library/TbtNvmDrvUtils.h>

typedef enum {
  PDF_READ_CONFIGURATION_REGISTERS = 1,
  PDF_WRITE_CONFIGURATION_REGISTERS = 2,
  /*
  // The PDFs below are not supported
  PDF_ERROR_NOTIFICATION,
  PDF_ERROR_ACKNOWLEDGMENT,
  PDF_PLUG_EVENT_NOTIFICATION,
  PDF_INTER_DOMAIN_REQUEST,
  PDF_INTER_DOMAIN_RESPONSE,
  PDF_CM_OVERRIDE,
  PDF_RESET_CIO_SWITCH,
  PDF_FW_TO_SW_NOTIFICATION,
  */
  PDF_SW_TO_FW_COMMAND = 11,
  PDF_FW_TO_SW_RESPONSE = 12
} PDF_VALUE;

// The configuration space that is accessed by request
typedef enum {
  /*PATH_CONFIG_SPACE,*/
  ADAPTER_CONFIG_SPACE=1,
  DEVICE_CONFIG_SPACE=2
  //COUNTERS_CONFIG_SPACE
} CIO_CONFIGURATION_SPACE;

typedef struct _USB4_PORT_CAPABILITY_REGS
{
  // DWORD 0
  UINT32 NextCapabilityPtr    : BITFIELD_RANGE (0, 7);
  UINT32 CapabilityID         : BITFIELD_RANGE (8, 15);
  UINT32                      : BITFIELD_RANGE (16, 31);
  // DWORD 1
  UINT32 Addess               : BITFIELD_RANGE (0, 7);
  UINT32 Length               : BITFIELD_RANGE (8, 15);
  UINT32 Target               : BITFIELD_RANGE (16, 18);
  UINT32                      : BITFIELD_BIT (19);
  UINT32 CascadedRetimerIndex : BITFIELD_RANGE (20, 23);
  UINT32 WnR                  : BITFIELD_BIT (24);
  UINT32 NoResp               : BITFIELD_BIT (25);
  UINT32 ResultCode           : BITFIELD_BIT (26);
  UINT32                      : BITFIELD_RANGE (27, 30);
  UINT32 Pending              : BITFIELD_BIT (31);

  // DWORDs 2-17
  UINT32 Data[16];

  // DWORDs 18-19
  UINT32 NotUsed[2];
} USB4_PORT_CAPABILITY_REGS;

typedef struct _READ_CONFIGURATION_REGISTERS_REQUEST
{
  // DWORD 0 - 1
  UINT64 RouteString;

  // DWORD 2
  UINT32 DwIndex            : BITFIELD_RANGE (0, 12);
  UINT32 Length             : BITFIELD_RANGE (13, 18);
  UINT32 Adapter            : BITFIELD_RANGE (19, 24);
  UINT32 ConfigurationSpace : BITFIELD_RANGE (25, 26);
  UINT32 SequenceNumber     : BITFIELD_RANGE (27, 28);
  UINT32                    : BITFIELD_RANGE (29, 31);

  // DWORD 3
  UINT32 Crc;
} READ_CONFIGURATION_REGISTERS_REQUEST;


typedef struct _READ_CONFIGURATION_REGISTERS_RESPONSE
{
  // DWORD 0 - 1
  UINT64 RouteString;

  // DWORD 2
  UINT32 DwIndex            : BITFIELD_RANGE (0, 12);
  UINT32 Length             : BITFIELD_RANGE (13, 18);
  UINT32 Adapter            : BITFIELD_RANGE (19, 24);
  UINT32 ConfigurationSpace : BITFIELD_RANGE (25, 26);
  UINT32 SequenceNumber     : BITFIELD_RANGE (27, 28);
  UINT32                    : BITFIELD_RANGE (29, 31);

  // DWORD 3
  UINT32 ReadData;
  // DWORD 4
  UINT32 Crc;
} READ_CONFIGURATION_REGISTERS_RESPONSE;

typedef struct _WRITE_CONFIGURATION_REGISTERS_REQUEST
{
  // DWORD 0 - 1
  UINT64 RouteString;

  // DWORD 2
  UINT32 DwIndex            : BITFIELD_RANGE (0, 12);
  UINT32 Length             : BITFIELD_RANGE (13, 18);
  UINT32 Adapter            : BITFIELD_RANGE (19, 24);
  UINT32 ConfigurationSpace : BITFIELD_RANGE (25, 26);
  UINT32 SequenceNumber     : BITFIELD_RANGE (27, 28);
  UINT32                    : BITFIELD_RANGE (29, 31);

  // DWORD 3 - 19
  UINT32 WrData[17];        // Data + CRC
} WRITE_CONFIGURATION_REGISTERS_REQUEST;

typedef struct _WRITE_CONFIGURATION_REGISTERS_RESPONSE
{
  // DWORD 0 - 1
  UINT64 RouteString;

  // DWORD 2
  UINT32 DwIndex            : BITFIELD_RANGE (0, 12);
  UINT32 Length             : BITFIELD_RANGE (13, 18);
  UINT32 Adapter            : BITFIELD_RANGE (19, 24);
  UINT32 ConfigurationSpace : BITFIELD_RANGE (25, 26);
  UINT32 SequenceNumber     : BITFIELD_RANGE (27, 28);
  UINT32                    : BITFIELD_RANGE (29, 31);

  // DWORD 3
  UINT32 Crc;
} WRITE_CONFIGURATION_REGISTERS_RESPONSE;

#endif /* TBT_NVM_DRV_CIO_PROTO_ */
