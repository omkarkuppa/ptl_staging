## @file
#  Definition of OpenSSL cipher specific acronym or terminology.
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
#    - https://docs.openssl.org/master/man1/openssl/#encryption-decryption-and-encoding-commands
#
##

#
# AES-128 Cipher.
#
OPENSSL_CIPHER_AES_128_STR    : str = 'aes128'
OPENSSL_CIPHER_AES_128_CBC_STR: str = 'aes-128-cbc'
OPENSSL_CIPHER_AES_128_CFB_STR: str = 'aes-128-cfb'
OPENSSL_CIPHER_AES_128_CTR_STR: str = 'aes-128-ctr'
OPENSSL_CIPHER_AES_128_ECB_STR: str = 'aes-128-ecb'
OPENSSL_CIPHER_AES_128_OFB_STR: str = 'aes-128-ofb'

#
# AES-192 Cipher.
#
OPENSSL_CIPHER_AES_192_STR    : str = 'aes192'
OPENSSL_CIPHER_AES_192_CBC_STR: str = 'aes-192-cbc'
OPENSSL_CIPHER_AES_192_CFB_STR: str = 'aes-192-cfb'
OPENSSL_CIPHER_AES_192_CTR_STR: str = 'aes-192-ctr'
OPENSSL_CIPHER_AES_192_ECB_STR: str = 'aes-192-ecb'
OPENSSL_CIPHER_AES_192_OFB_STR: str = 'aes-192-ofb'

#
# AES-256 Cipher.
#
OPENSSL_CIPHER_AES_256_STR    : str = 'aes256'
OPENSSL_CIPHER_AES_256_CBC_STR: str = 'aes-256-cbc'
OPENSSL_CIPHER_AES_256_CFB_STR: str = 'aes-256-cfb'
OPENSSL_CIPHER_AES_256_CTR_STR: str = 'aes-256-ctr'
OPENSSL_CIPHER_AES_256_ECB_STR: str = 'aes-256-ecb'
OPENSSL_CIPHER_AES_256_OFB_STR: str = 'aes-256-ofb'

#
# ARIA-128 Cipher.
#
OPENSSL_CIPHER_ARIA_128_STR    : str = 'aria128'
OPENSSL_CIPHER_ARIA_128_CBC_STR: str = 'aria-128-cbc'
OPENSSL_CIPHER_ARIA_128_CFB_STR: str = 'aria-128-cfb'
OPENSSL_CIPHER_ARIA_128_CTR_STR: str = 'aria-128-ctr'
OPENSSL_CIPHER_ARIA_128_ECB_STR: str = 'aria-128-ecb'
OPENSSL_CIPHER_ARIA_128_OFB_STR: str = 'aria-128-ofb'

#
# ARIA-192 Cipher.
#
OPENSSL_CIPHER_ARIA_192_STR    : str = 'aria192'
OPENSSL_CIPHER_ARIA_192_CBC_STR: str = 'aria-192-cbc'
OPENSSL_CIPHER_ARIA_192_CFB_STR: str = 'aria-192-cfb'
OPENSSL_CIPHER_ARIA_192_CTR_STR: str = 'aria-192-ctr'
OPENSSL_CIPHER_ARIA_192_ECB_STR: str = 'aria-192-ecb'
OPENSSL_CIPHER_ARIA_192_OFB_STR: str = 'aria-192-ofb'

#
# AES-256 Cipher.
#
OPENSSL_CIPHER_ARIA_256_STR    : str = 'aria256'
OPENSSL_CIPHER_ARIA_256_CBC_STR: str = 'aria-256-cbc'
OPENSSL_CIPHER_ARIA_256_CFB_STR: str = 'aria-256-cfb'
OPENSSL_CIPHER_ARIA_256_CTR_STR: str = 'aria-256-ctr'
OPENSSL_CIPHER_ARIA_256_ECB_STR: str = 'aria-256-ecb'
OPENSSL_CIPHER_ARIA_256_OFB_STR: str = 'aria-256-ofb'

#
# Blowfish Cipher.
#
OPENSSL_CIPHER_BF_STR    : str = 'bf'
OPENSSL_CIPHER_BF_CBC_STR: str = 'bf-cbc'
OPENSSL_CIPHER_BF_CFB_STR: str = 'bf-cfb'
OPENSSL_CIPHER_BF_ECB_STR: str = 'bf-ecb'
OPENSSL_CIPHER_BF_OFB_STR: str = 'bf-ofb'

#
# Camellia-128 Cipher.
#
OPENSSL_CIPHER_CAMELLIA_128_STR    : str = 'camellia128'
OPENSSL_CIPHER_CAMELLIA_128_CBC_STR: str = 'camellia-128-cbc'
OPENSSL_CIPHER_CAMELLIA_128_CFB_STR: str = 'camellia-128-cfb'
OPENSSL_CIPHER_CAMELLIA_128_CTR_STR: str = 'camellia-128-ctr'
OPENSSL_CIPHER_CAMELLIA_128_ECB_STR: str = 'camellia-128-ecb'
OPENSSL_CIPHER_CAMELLIA_128_OFB_STR: str = 'camellia-128-ofb'

#
# Camellia-192 Cipher.
#
OPENSSL_CIPHER_CAMELLIA_192_STR    : str = 'camellia192'
OPENSSL_CIPHER_CAMELLIA_192_CBC_STR: str = 'camellia-192-cbc'
OPENSSL_CIPHER_CAMELLIA_192_CFB_STR: str = 'camellia-192-cfb'
OPENSSL_CIPHER_CAMELLIA_192_CTR_STR: str = 'camellia-192-ctr'
OPENSSL_CIPHER_CAMELLIA_192_ECB_STR: str = 'camellia-192-ecb'
OPENSSL_CIPHER_CAMELLIA_192_OFB_STR: str = 'camellia-192-ofb'

#
# Camellia-256 Cipher.
#
OPENSSL_CIPHER_CAMELLIA_256_STR    : str = 'camellia256'
OPENSSL_CIPHER_CAMELLIA_256_CBC_STR: str = 'camellia-256-cbc'
OPENSSL_CIPHER_CAMELLIA_256_CFB_STR: str = 'camellia-256-cfb'
OPENSSL_CIPHER_CAMELLIA_256_CTR_STR: str = 'camellia-256-ctr'
OPENSSL_CIPHER_CAMELLIA_256_ECB_STR: str = 'camellia-256-ecb'
OPENSSL_CIPHER_CAMELLIA_256_OFB_STR: str = 'camellia-256-ofb'

#
# CAST Cipher.
#
OPENSSL_CIPHER_CAST_STR    : str = 'cast'
OPENSSL_CIPHER_CAST_CBC_STR: str = 'cast-cbc'

#
# CAST5 Cipher.
#
OPENSSL_CIPHER_CAST5_CBC_STR: str = 'cast5-cbc'
OPENSSL_CIPHER_CAST5_CFB_STR: str = 'cast5-cfb'
OPENSSL_CIPHER_CAST5_ECB_STR: str = 'cast5-ecb'
OPENSSL_CIPHER_CAST5_OFB_STR: str = 'cast5-ofb'

#
# Chacha20 Cipher.
#
OPENSSL_CIPHER_CHACHA20_STR: str = 'chacha20'

#
# DES Cipher.
#
OPENSSL_CIPHER_DES_STR        : str = 'des'
OPENSSL_CIPHER_DES_CBC_STR    : str = 'des-cbc'
OPENSSL_CIPHER_DES_CFB_STR    : str = 'des-cfb'
OPENSSL_CIPHER_DES_ECB_STR    : str = 'des-ecb'
OPENSSL_CIPHER_DES_EDE_STR    : str = 'des-ede'
OPENSSL_CIPHER_DES_EDE_CBC_STR: str = 'des-ede-cbc'
OPENSSL_CIPHER_DES_EDE_CFB_STR: str = 'des-ede-cfb'
OPENSSL_CIPHER_DES_EDE_OFB_STR: str = 'des-ede-ofb'
OPENSSL_CIPHER_DES_OFB_STR    : str = 'des-ofb'

#
# Triple-DES Cipher.
#
OPENSSL_CIPHER_DES3_STR        : str = 'des3'
OPENSSL_CIPHER_DESX_STR        : str = 'desx'
OPENSSL_CIPHER_DES_EDE3_STR    : str = 'des-ede3'
OPENSSL_CIPHER_DES_EDE3_CBC_STR: str = 'des-ede3-cbc'
OPENSSL_CIPHER_DES_EDE3_CFB_STR: str = 'des-ede3-cfb'
OPENSSL_CIPHER_DES_EDE3_OFB_STR: str = 'des-ede3-ofb'

#
# IDEA Cipher.
#
OPENSSL_CIPHER_IDEA_STR    : str = 'idea'
OPENSSL_CIPHER_IDEA_CBC_STR: str = 'idea-cbc'
OPENSSL_CIPHER_IDEA_CFB_STR: str = 'idea-cfb'
OPENSSL_CIPHER_IDEA_ECB_STR: str = 'idea-ecb'
OPENSSL_CIPHER_IDEA_OFB_STR: str = 'idea-ofb'

#
# RC2 Cipher.
#
OPENSSL_CIPHER_RC2_STR    : str = 'rc2'
OPENSSL_CIPHER_RC2_CBC_STR: str = 'rc2-cbc'
OPENSSL_CIPHER_RC2_CFB_STR: str = 'rc2-cfb'
OPENSSL_CIPHER_RC2_ECB_STR: str = 'rc2-ecb'
OPENSSL_CIPHER_RC2_OFB_STR: str = 'rc2-ofb'

#
# RC4 Cipher.
#
OPENSSL_CIPHER_RC4_STR: str = 'rc4'

#
# RC5 Cipher.
#
OPENSSL_CIPHER_RC5_STR    : str = 'rc5'
OPENSSL_CIPHER_RC5_CBC_STR: str = 'rc5-cbc'
OPENSSL_CIPHER_RC5_CFB_STR: str = 'rc5-cfb'
OPENSSL_CIPHER_RC5_ECB_STR: str = 'rc5-ecb'
OPENSSL_CIPHER_RC5_OFB_STR: str = 'rc5-ofb'

#
# SEED Cipher.
#
OPENSSL_CIPHER_SEED_STR    : str = 'seed'
OPENSSL_CIPHER_SEED_CBC_STR: str = 'seed-cbc'
OPENSSL_CIPHER_SEED_CFB_STR: str = 'seed-cfb'
OPENSSL_CIPHER_SEED_ECB_STR: str = 'seed-ecb'
OPENSSL_CIPHER_SEED_OFB_STR: str = 'seed-ofb'

#
# SM4 Cipher.
#
OPENSSL_CIPHER_SM4_STR    : str = 'sm4'
OPENSSL_CIPHER_SM4_CBC_STR: str = 'sm4-cbc'
OPENSSL_CIPHER_SM4_CFB_STR: str = 'sm4-cfb'
OPENSSL_CIPHER_SM4_CTR_STR: str = 'sm4-ctr'
OPENSSL_CIPHER_SM4_ECB_STR: str = 'sm4-ecb'
OPENSSL_CIPHER_SM4_OFB_STR: str = 'sm4-ofb'
