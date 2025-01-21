## @file
#  Definition of OpenSSL certificate authority command and flags.
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
#    - https://docs.openssl.org/master/man1/openssl-ca/
#
##

from typing import *

from CapsuleCommon.OpenSSL.Constant.Cipher import *

#
# Certificate authority command.
#
OPENSSL_CMD_CA: str = 'ca'

#
# Certificate authority command flags.
#
OPENSSL_CA_FLAG_EXTENSIONS: str = '-extensions'
OPENSSL_CA_FLAG_DAYS      : str = '-days'
OPENSSL_CA_FLAG_MD        : str = '-md'
OPENSSL_CA_FLAG_CERT      : str = '-cert'
OPENSSL_CA_FLAG_KEY_FILE  : str = '-keyfile'
OPENSSL_CA_FLAG_PASS_IN   : str = '-passin'
OPENSSL_CA_FLAG_IN        : str = '-in'
OPENSSL_CA_FLAG_OUT       : str = '-out'
OPENSSL_CA_FLAG_BATCH     : str = '-batch'
OPENSSL_CA_FLAG_QUIET     : str = '-quiet'
OPENSSL_CA_FLAG_VERBOSE   : str = '-verbose'
