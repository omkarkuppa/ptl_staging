/** @file
  COC test application header definitions.

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

#ifndef L2CAP_COC_TEST_APP_H_
#define L2CAP_COC_TEST_APP_H_

#define L2CAP_COC_LE_DYNAMIC_PSM  0x0080
#define L2CAP_COC_LE_MTU          46               /* To be tuned as per use cases */

typedef struct {
  /* True if coc_handle is open otherwise false */
  BOOLEAN    coc_handle_idx_list[MAX_NUMBER_OF_L2CAP_COC_CHNL];
} conn_id_coc_handle_t;

typedef struct {
  UINT8                   app_handle;
  conn_id_coc_handle_t    conn_id_coc_handle[MAX_NUM_CONNECTED_DEVICE];
} l2cap_coc_test_app_data_t;

#endif /* L2CAP_COC_TEST_APP_H_ */
