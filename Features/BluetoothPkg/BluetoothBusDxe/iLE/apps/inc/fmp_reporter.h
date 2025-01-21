/** @file
  FMP reporter header definitions.

  Copyright (c) 2017, Intel Mobile Communications GmbH. All rights reserved.<BR>
  This program and the accompanying materials
  are licensed and made available under the terms and conditions of the BSD License
  which accompanies this distribution.  The full text of the license may be found at
  http://opensource.org/licenses/bsd-license.php

  THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
  WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2017 Intel Corporation.

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

#ifndef FMP_REPORTER_H_
#define FMP_REPORTER_H_

/* FMP Request queue size */
#define FMP_SRV_REQ_QUEUE_LEN  2

/* Immidiate alert Service */
#define IMMEDIATE_ALERT_SERVICE_UUID  0x1802
#define ALERT_LEVEL_CHAR_UUID         0x2A06

/* FMP Message opcodes */
#define FMP_SRV_APP_REGISTERED      0
#define FMP_SRV_BT_CORE_STACK_INIT  1
#define FMP_SRV_SERVICE_STARTED     2
#define FMP_SRV_DISCONNECTED        3
#define FMP_SRV_CHAR_READ_REQ       4
#define FMP_SRV_CHAR_WRITE_REQ      5
#define FMP_SRV_START_ADV_REQ       6
#define FMP_SRV_STOP_ADV_REQ        7
#define FMP_SRV_DISCONNECT_REQ      8

typedef struct {
  UINT8    opcode;
  UINT8    *param;
} fmp_srv_msg_t;

typedef struct {
  UINT8     conn_id;
  UINT8     att_opcode;
  UINT8     offset;
  UINT16    att_id;
} fmp_srv_attr_read_req_param_t;

typedef struct {
  UINT8     conn_id;
  UINT8     att_opcode;
  UINT16    att_id;
} fmp_srv_attr_write_req_param_t;

typedef struct {
  BOOLEAN            state;
  BOOLEAN            isconnected;
  UINT8              app_handle;
  UINT16             service_handle;
  queue_t            req_queue;
  uuid_t             service_uuid;
  UINT8              alert_level;
  osa_semaphore_t    msg_sem;
  osa_semaphore_t    thread_sem;
} fmp_srv_data_t;

#endif /* FMP_REPORTER_H_ */
