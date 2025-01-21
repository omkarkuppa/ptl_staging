/** @file
  Header file of the Connectivity Setup Component.

  Macros defined here are used to specify VFR FormId or QuestionId in VFR and SD files.
  When VFR files are updated, new macros may be required to specify new FormId or QuestionId.

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

#ifndef _CNV_SETUP_ID_H_
#define _CNV_SETUP_ID_H_

#ifndef AUTO_ID
#define AUTO_ID(x)  x
#endif

#define SUBTITLE(Text)  subtitle text = Text;
#define SEPARATOR  SUBTITLE(STRING_TOKEN(STR_EMPTY))

#define CONCATENATE2_(X, Y)  X##Y
#define CONCATENATE2(X, Y)   CONCATENATE2_(X, Y)

#define CONCATENATE3_(X, Y, Z)  X##Y##Z
#define CONCATENATE3(X, Y, Z)   CONCATENATE3_(X, Y, Z)

// Connectivity Setup Form ID
#define CONNECTIVITY_OPTIONS_FORM_ID        5074
#define CNV_VFR_CONFIG_SETUP_DATA_ID        5075
#define KEY_CNV_BT_AUDIO_OFFLOAD            5230
#define KEY_CNV_BT_AUDIO_OFFLOAD_INTERFACE  5231
#define KEY_CNV_BT_INTERFACE                5436
#define KEY_CNV_BT_CNVI_MODE                5437
// WiFi Connectivity Menu
#define KEY_WIFI_POWER_LIMIT  5432

#endif
