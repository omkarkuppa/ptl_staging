## @file
#  Definition of OpenSSL PKCS#12 handling command and flags.
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
#    - https://docs.openssl.org/master/man1/openssl-pkcs12/
#
##

from typing import *

from CapsuleCommon.OpenSSL.Constant.Cipher import *

#
# PKCS#12 handling command.
#
OPENSSL_CMD_PKCS12: str = 'pkcs12'

#
# PKCS#12 handling command flags.
#
OPENSSL_PKCS12_FLAG_EXPORT    : str = '-export'
OPENSSL_PKCS12_FLAG_IN        : str = '-in'
OPENSSL_PKCS12_FLAG_IN_KEY    : str = '-inkey'
OPENSSL_PKCS12_FLAG_OUT       : str = '-out'
OPENSSL_PKCS12_FLAG_PASS_IN   : str = '-passin'
OPENSSL_PKCS12_FLAG_PASS_OUT  : str = '-passout'
OPENSSL_PKCS12_FLAG_NO_ENCRYPT: str = '-noenc'
