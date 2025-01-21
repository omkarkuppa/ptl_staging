## @file
#  Definition of OpenSSL X.509 certificate handling command and flags.
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
#    - https://docs.openssl.org/master/man1/openssl-x509/
#
##

from typing import *

from CapsuleCommon.OpenSSL.Constant.KeyForm import *

#
# X.509 certificate handling command.
#
OPENSSL_CMD_X509: str = 'x509'

#
# X.509 certificate handling command flags.
#
OPENSSL_X509_FLAG_IN      : str = '-in'
OPENSSL_X509_FLAG_IN_FORM : str = '-inform'
OPENSSL_X509_FLAG_OUT     : str = '-out'
OPENSSL_X509_FLAG_OUT_FORM: str = '-outform'

#
# X.509 certificate handling command supported key form.
#
OPENSSL_X509_KEY_IN_FORM_SUPPORTED_LIST: List[str] = [
    OPENSSL_KEY_FORM_DER_STR,
    OPENSSL_KEY_FORM_PEM_STR,
    ]

OPENSSL_X509_KEY_OUT_FORM_SUPPORTED_LIST: List[str] = [
    OPENSSL_KEY_FORM_DER_STR,
    OPENSSL_KEY_FORM_PEM_STR,
    ]
