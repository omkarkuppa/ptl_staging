## @file
#  Definition of OpenSSL certificate requests (CSRs) command and flags.
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
#    - https://docs.openssl.org/master/man1/openssl-req/
#
##

from typing import *

from CapsuleCommon.OpenSSL.Constant.Cipher import *

#
# Certificate requests (CSRs) command.
#
OPENSSL_CMD_REQ: str = 'req'

#
# Certificate requests (CSRs) command flags.
#
OPENSSL_REQ_FLAG_EXTENSIONS: str = '-extensions'
OPENSSL_REQ_FLAG_NEW       : str = '-new'
OPENSSL_REQ_FLAG_X509      : str = '-x509'
OPENSSL_REQ_FLAG_DAYS      : str = '-days'
OPENSSL_REQ_FLAG_SUBJECT   : str = '-subj'
OPENSSL_REQ_FLAG_KEY       : str = '-key'
OPENSSL_REQ_FLAG_PASS_IN   : str = '-passin'
OPENSSL_REQ_FLAG_OUT       : str = '-out'
OPENSSL_REQ_FLAG_BATCH     : str = '-batch'
OPENSSL_REQ_FLAG_VERBOSE   : str = '-verbose'
OPENSSL_REQ_FLAG_QUIET     : str = '-quiet'
