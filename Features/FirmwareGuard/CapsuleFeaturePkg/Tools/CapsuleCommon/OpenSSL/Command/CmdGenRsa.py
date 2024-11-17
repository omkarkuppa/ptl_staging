## @file
#  Definition of OpenSSL generate RSA key-pair command and flags.
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
#    - https://docs.openssl.org/master/man1/openssl-genrsa/
#
##

from typing import *

from CapsuleCommon.OpenSSL.Constant.Cipher import *

#
# Generate RSA command.
#
OPENSSL_CMD_GEN_RSA: str = 'genrsa'

#
# Generate RSA command flags.
#
OPENSSL_GEN_RSA_FLAG_PASS_OUT: str = '-passout'
OPENSSL_GEN_RSA_FLAG_OUT     : str = '-out'
OPENSSL_GEN_RSA_FLAG_QUIET   : str = '-quiet'
OPENSSL_GEN_RSA_FLAG_VERBOSE : str = '-verbose'

#
# Generate RSA command supported cipher.
#
OPENSSL_GEN_RSA_CIPHER_SUPPORTED_LIST: List[str] = [
    OPENSSL_CIPHER_AES_128_STR,
    OPENSSL_CIPHER_AES_192_STR,
    OPENSSL_CIPHER_AES_256_STR,
    OPENSSL_CIPHER_ARIA_128_STR,
    OPENSSL_CIPHER_ARIA_192_STR,
    OPENSSL_CIPHER_ARIA_256_STR,
    OPENSSL_CIPHER_CAMELLIA_128_STR,
    OPENSSL_CIPHER_CAMELLIA_192_STR,
    OPENSSL_CIPHER_CAMELLIA_256_STR,
    OPENSSL_CIPHER_DES_STR,
    OPENSSL_CIPHER_DES3_STR,
    OPENSSL_CIPHER_IDEA_STR,
    ]
