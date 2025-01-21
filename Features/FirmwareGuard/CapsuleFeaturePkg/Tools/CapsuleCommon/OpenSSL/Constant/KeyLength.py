## @file
#  Definition of OpenSSL specific key length acronym or terminology.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
#  @par Specification
#
##

from typing import List

OPENSSL_KEY_LENGTH_1024_BIT_STR: str       = '1024'
OPENSSL_KEY_LENGTH_2048_BIT_STR: str       = '2048'
OPENSSL_KEY_LENGTH_3072_BIT_STR: str       = '3072'
OPENSSL_KEY_LENGTH_4096_BIT_STR: str       = '4096'
OPENSSL_KEY_LENGTH_STR_LIST    : List[str] = [
    OPENSSL_KEY_LENGTH_1024_BIT_STR,
    OPENSSL_KEY_LENGTH_2048_BIT_STR,
    OPENSSL_KEY_LENGTH_3072_BIT_STR,
    OPENSSL_KEY_LENGTH_4096_BIT_STR,
    ]
