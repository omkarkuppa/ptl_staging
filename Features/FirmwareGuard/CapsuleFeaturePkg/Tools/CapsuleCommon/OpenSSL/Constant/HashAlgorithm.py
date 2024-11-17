## @file
#  Definition of OpenSSL hash algorithm specific acronym or terminology.
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
#    - https://docs.openssl.org/master/man1/openssl/#message-digest-commands
#
##

from typing import List

OPENSSL_HASH_ALGORITHM_BLAKE2B512_STR: str       = 'blake2b512'
OPENSSL_HASH_ALGORITHM_BLAKE2S256_STR: str       = 'blake2s256'
OPENSSL_HASH_ALGORITHM_MD4_STR       : str       = 'md4'
OPENSSL_HASH_ALGORITHM_MD5_STR       : str       = 'md5'
OPENSSL_HASH_ALGORITHM_MDC2_STR      : str       = 'mdc2'
OPENSSL_HASH_ALGORITHM_RMD160_STR    : str       = 'rmd160'
OPENSSL_HASH_ALGORITHM_SHA1_STR      : str       = 'sha1'
OPENSSL_HASH_ALGORITHM_SHA224_STR    : str       = 'sha224'
OPENSSL_HASH_ALGORITHM_SHA256_STR    : str       = 'sha256'
OPENSSL_HASH_ALGORITHM_SHA3_224_STR  : str       = 'sha3-224'
OPENSSL_HASH_ALGORITHM_SHA3_256_STR  : str       = 'sha3-256'
OPENSSL_HASH_ALGORITHM_SHA3_384_STR  : str       = 'sha3-384'
OPENSSL_HASH_ALGORITHM_SHA3_512_STR  : str       = 'sha3-512'
OPENSSL_HASH_ALGORITHM_SHA384_STR    : str       = 'sha384'
OPENSSL_HASH_ALGORITHM_SHA512_STR    : str       = 'sha512'
OPENSSL_HASH_ALGORITHM_SHA512_224_STR: str       = 'sha512-224'
OPENSSL_HASH_ALGORITHM_SHA512_256_STR: str       = 'sha512-256'
OPENSSL_HASH_ALGORITHM_SHAKE128_STR  : str       = 'shake128'
OPENSSL_HASH_ALGORITHM_SHAKE256_STR  : str       = 'shake256'
OPENSSL_HASH_ALGORITHM_SM3_STR       : str       = 'sm3'
OPENSSL_HASH_ALGORITHM_STR_LIST      : List[str] = [
    OPENSSL_HASH_ALGORITHM_BLAKE2B512_STR,
    OPENSSL_HASH_ALGORITHM_BLAKE2S256_STR,
    OPENSSL_HASH_ALGORITHM_MD4_STR,
    OPENSSL_HASH_ALGORITHM_MD5_STR,
    OPENSSL_HASH_ALGORITHM_MDC2_STR,
    OPENSSL_HASH_ALGORITHM_RMD160_STR,
    OPENSSL_HASH_ALGORITHM_SHA1_STR,
    OPENSSL_HASH_ALGORITHM_SHA224_STR,
    OPENSSL_HASH_ALGORITHM_SHA256_STR,
    OPENSSL_HASH_ALGORITHM_SHA3_224_STR,
    OPENSSL_HASH_ALGORITHM_SHA3_256_STR,
    OPENSSL_HASH_ALGORITHM_SHA3_384_STR,
    OPENSSL_HASH_ALGORITHM_SHA3_512_STR,
    OPENSSL_HASH_ALGORITHM_SHA384_STR,
    OPENSSL_HASH_ALGORITHM_SHA512_STR,
    OPENSSL_HASH_ALGORITHM_SHA512_224_STR,
    OPENSSL_HASH_ALGORITHM_SHA512_256_STR,
    OPENSSL_HASH_ALGORITHM_SHAKE128_STR,
    OPENSSL_HASH_ALGORITHM_SHAKE256_STR,
    OPENSSL_HASH_ALGORITHM_SM3_STR,
    ]
