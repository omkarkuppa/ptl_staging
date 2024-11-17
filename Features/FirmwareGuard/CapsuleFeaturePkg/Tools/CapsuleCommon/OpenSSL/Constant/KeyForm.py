## @file
#  Definition of OpenSSL specific key form acronym or terminology.
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
#    - https://docs.openssl.org/master/man1/openssl-format-options
#
##

from typing import List

OPENSSL_KEY_FORM_DER_STR   : str       = 'DER'
OPENSSL_KEY_FORM_ENGINE_STR: str       = 'ENGINE'
OPENSSL_KEY_FORM_P12_STR   : str       = 'P12'
OPENSSL_KEY_FORM_PEM_STR   : str       = 'PEM'
OPENSSL_KEY_FORM_SMIME_STR : str       = 'SMIME'
OPENSSL_KEY_FORM_STR_LIST  : List[str] = [
    OPENSSL_KEY_FORM_DER_STR,
    OPENSSL_KEY_FORM_ENGINE_STR,
    OPENSSL_KEY_FORM_P12_STR,
    OPENSSL_KEY_FORM_PEM_STR,
    OPENSSL_KEY_FORM_SMIME_STR,
    ]
