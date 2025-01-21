/** @file
  Application interface definations.

  Copyright (c) 2017 - 2020, Intel Mobile Communications GmbH. All rights reserved.<BR>
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

#ifndef APP_INTERFACE_H_
#define APP_INTERFACE_H_

#include "interface.h"

#ifndef L2CAP_COC_TEST_APP
#define L2CAP_COC_TEST_APP  FALSE
#endif

#ifndef HCI2ILE_ADAPT_APP
#define HCI2ILE_ADAPT_APP  FALSE
#endif

#if (TARGET_UEFI != TRUE)
/* These libraries are not available on UEFI platform */
  #include <string.h>
#endif

#if (TEST_APP == TRUE)
#define MAX_NUM_DISCOVERED_DEVICE  20
#else
#define MAX_NUM_DISCOVERED_DEVICE  10
#endif

#if (HRP_APP == TRUE)
void
hrp_init (
  );

void
hrp_cleanup (
  );

#endif

#if (TEST_APP == TRUE)
void
test_app_init (
  );

void
test_app_cleanup (
  );

#endif

#if (GAP_GATT_SRV_APP == TRUE)
void
gap_gatt_service_init (
  );

void
gap_gatt_service_cleanup (
  );

#endif

#if (A4WP_APP == TRUE)
void
a4wp_init (
  );

void
a4wp_cleanup (
  );

#endif

#if (FMP_APP == TRUE)
void
fmp_init (
  );

void
fmp_cleanup (
  );

#endif

#if (FMP_SRV_APP == TRUE)
void
fmp_srv_init (
  );

void
fmp_srv_cleanup (
  );

#endif

#if (SMP_APP == TRUE)
void
smp_app_init (
  );

void
smp_app_cleanup (
  );

#endif

#if (L2CAP_COC_TEST_APP == TRUE)
void
l2cap_coc_test_app_init (
  );

void
l2cap_coc_test_app_cleanup (
  );

void
test_app1_init (
  );

void
test_app1_cleanup (
  );

#endif

#if (HOGP_APP == TRUE)
void
hogp_init (
  );

void
hogp_cleanup (
  );

#endif

#if (ATP_AUTO_TEST_MODE == TRUE)
void
atp_app_init (
  );

void
atp_app_cleanup (
  );

#endif

#if (HCI2ILE_ADAPT_APP == TRUE)
void
adapt_app_init (
  );

void
adapt_app_cleanup (
  );

#endif

#if (TARGET_ANDROID == TRUE)
extern osa_semaphore_t  android_hal_init_sem;
#endif

#if (UEFI_APP == TRUE)
void
uefi_app_init (
  );

#endif

#endif /* APP_INTERFACE_H_ */
