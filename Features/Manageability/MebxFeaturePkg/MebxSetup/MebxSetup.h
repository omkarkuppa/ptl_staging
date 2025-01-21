/** @file
  This file contains definitions for MEBx initialization, HECI Initialization,
  BPF Obtaining, DriverCallback.

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

@par Specification Reference:
**/

#ifndef _MEBX_SETUP_H
#define _MEBX_SETUP_H

#define INPUT_OK                   0
#define HDN_INVALID_CHARACTER      1
#define HDN_BAD_DOT                2
#define HDN_LABEL_TOO_LONG         3
#define HDN_NAME_TOO_LONG          4
#define HDN_BAD_HYPHEN             5
#define BAD_IP_ADDR                6
#define BAD_IP_MASK                7
#define BAD_IP_ADDR_MASK_COMBO     8
#define BAD_NEW_PWD                9
#define HDN_DOMAIN_TOO_LONG        10

#define SHOW_LOGIN                 0
#define SHOW_EVERYTHING            1
#define SHOW_PWD_EXCEEDED          2
#define SHOW_TOO_EARLY             3
#define SHOW_AMT_DISABLED          4
#define SHOW_AFTER_EOP             5
#define SHOW_REDIR_ACTIVE          6
#define SHOW_UNSUPPORTED_INTERFACE 7

#define FORM_ID_MEBX               9001
#define FORM_ID_POWER_CTRL         9003
#define FORM_ID_AMT                9004
#define FORM_ID_SOLSTORAGEREDIRKVM 9005
#define FORM_ID_USERCONSENT        9006
#define FORM_ID_NETWORK_SETUP      9007
#define FORM_ID_ME_NETWORK_NAME    9008
#define FORM_ID_TCPIP              9009
#define FORM_ID_WIRED_IPV4         9010
#define FORM_ID_REMOTE_SETUP       9011
#define FORM_ID_REMOTE_CONFIG      9012
#define FORM_ID_TLS_PKI            9013
#define FORM_ID_MANAGE_HASHES      9014
#define FORM_ID_CERT_0             9100
#define FORM_ID_CERT_1             9101
#define FORM_ID_CERT_2             9102
#define FORM_ID_CERT_3             9103
#define FORM_ID_CERT_4             9104
#define FORM_ID_CERT_5             9105
#define FORM_ID_CERT_6             9106
#define FORM_ID_CERT_7             9107
#define FORM_ID_CERT_8             9108
#define FORM_ID_CERT_9             9109
#define FORM_ID_CERT_10            9110
#define FORM_ID_CERT_11            9111
#define FORM_ID_CERT_12            9112
#define FORM_ID_CERT_13            9113
#define FORM_ID_CERT_14            9114
#define FORM_ID_CERT_15            9115
#define FORM_ID_CERT_16            9116
#define FORM_ID_CERT_17            9117
#define FORM_ID_CERT_18            9118
#define FORM_ID_CERT_19            9119
#define FORM_ID_CERT_20            9120
#define FORM_ID_CERT_21            9121
#define FORM_ID_CERT_22            9122
#define FORM_ID_CERT_23            9123
#define FORM_ID_CERT_24            9124
#define FORM_ID_CERT_25            9125
#define FORM_ID_CERT_26            9126
#define FORM_ID_CERT_27            9127
#define FORM_ID_CERT_28            9128
#define FORM_ID_CERT_29            9129
#define FORM_ID_CERT_30            9130
#define FORM_ID_CERT_31            9131
#define FORM_ID_CERT_32            9132
#define FORM_ID_CERT_33            9133

#define KEY_MEPWD                  0x900
#define KEY_CHANGEMEPWD            0x1101
#define KEY_HOSTDOMAINNAME         0x7654
#define KEY_FQDNTYPE               0x4302
#define KEY_IPV4ADDR               0x5101
#define KEY_IPV4MASKADDR           0x5102
#define KEY_IPV4GATEWAYADDR        0x5103
#define KEY_IPV4PREFDNSADDR        0x5104
#define KEY_IPV4ALTDNSADDR         0x5105
#define KEY_PROVSERVERIPV6ADDR     0x6902
#define KEY_PROVSERVERFQDNADDR     0x6903
#define KEY_STARTCONFIG            0x7500
#define KEY_HALTCONFIG             0x7501
#define KEY_TLS_PKI                0x8000
#define KEY_TLSPPKREMOTECONFIG     0x8200
#define KEY_PKIDNSSUFFIX           0x8101

#endif // _MEBX_SETUP_H
