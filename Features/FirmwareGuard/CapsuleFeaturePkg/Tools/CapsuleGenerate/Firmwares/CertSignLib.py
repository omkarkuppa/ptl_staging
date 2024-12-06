## @file
#  Definition of FMP device sign certificate object.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2023 Intel Corporation.
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


from CapsuleGenerate._Global import *
from CapsuleGenerate.Model.SignCert import *

#
# Sign Tool Path.
#
OPENSSL_SIGN_TOOL_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gPathInfo[OPENSSL_DIR_KEY],
      )
EDSS_SIGN_TOOL_PATH   : str = \
    JoinPath (
      WORKSPACE_PATH,
      gPathInfo[EDSS_SIGN_TOOL_DIR_KEY],
      )

#
# EDK-II 2K X.509 Certificate.
#
EDKII_ROOT_PUB_KEY_FILE_PATH  : str = \
    JoinPath (
      EDKII_TOOL_PATH,
      EDKII_BASE_TOOL_TRUSTED_PUBLIC_CERT_PATH,
      )
EDKII_INTRA_PUB_KEY_FILE_PATH : str = \
    JoinPath (
      EDKII_TOOL_PATH,
      EDKII_BASE_TOOL_OTHER_PUBLIC_CERT_PATH,
      )
EDKII_SIGNER_PRI_KEY_FILE_PATH: str = \
    JoinPath (
      EDKII_TOOL_PATH,
      EDKII_BASE_TOOL_SIGNER_PRIVATE_CERT_PATH,
      )

EDKII_CERT_SIGN: SignCert        = SignCert ()
EDKII_CERT_SIGN.CertName         = 'EDK-II Default 2K Test Certificate'
EDKII_CERT_SIGN.CertType         = SIGN_X509_CERT_FILE
EDKII_CERT_SIGN.SignToolType     = SIGN_TOOL_OPENSSL
EDKII_CERT_SIGN.SignToolPath     = OPENSSL_SIGN_TOOL_PATH
EDKII_CERT_SIGN.RootPubKeyPath   = EDKII_ROOT_PUB_KEY_FILE_PATH
EDKII_CERT_SIGN.IntraPubKeyPath  = EDKII_INTRA_PUB_KEY_FILE_PATH
EDKII_CERT_SIGN.SignerPriKeyPath = EDKII_SIGNER_PRI_KEY_FILE_PATH

#
# Platform specific 3K X.509 Certificate.
#
PLATFORM_ROOT_PUB_KEY_FILE_PATH  : str = \
    JoinPath (
      WORKSPACE_PATH,
      gCapSignKeyInfo[CAP_TRUST_PUBLIC_CERT_KEY],
      )
PLATFORM_INTRA_PUB_KEY_FILE_PATH : str = \
    JoinPath (
      WORKSPACE_PATH,
      gCapSignKeyInfo[CAP_OTHER_PUBLIC_CERT_KEY],
      )
PLATFORM_SIGNER_PRI_KEY_FILE_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gCapSignKeyInfo[CAP_SIGNER_PRIVATE_CERT_KEY],
      )

PLATFORM_CERT_SIGN: SignCert        = SignCert ()
PLATFORM_CERT_SIGN.CertName         = 'Intel Platform Sample Test Certificate'
PLATFORM_CERT_SIGN.CertType         = SIGN_X509_CERT_FILE
PLATFORM_CERT_SIGN.SignToolType     = SIGN_TOOL_OPENSSL
PLATFORM_CERT_SIGN.SignToolPath     = OPENSSL_SIGN_TOOL_PATH
PLATFORM_CERT_SIGN.RootPubKeyPath   = PLATFORM_ROOT_PUB_KEY_FILE_PATH
PLATFORM_CERT_SIGN.IntraPubKeyPath  = PLATFORM_INTRA_PUB_KEY_FILE_PATH
PLATFORM_CERT_SIGN.SignerPriKeyPath = PLATFORM_SIGNER_PRI_KEY_FILE_PATH

#
# EDSS Keystore.
#
EDSS_PRE_PROD_CERT_SUBJECT_NAME: str = \
    gCapSignKeyInfo[ME_PRE_PROD_CERT_SUBJECT_KEY]
EDSS_PROD_CERT_SUBJECT_NAME    : str = \
    gCapSignKeyInfo[ME_PROD_CERT_SUBJECT_KEY]

EDSS_CERT_PRE_PROD_SIGN: SignCert    = SignCert ()
EDSS_CERT_PRE_PROD_SIGN.CertName     = 'EDSS Platform Pre-Prod Certificate'
EDSS_CERT_PRE_PROD_SIGN.CertType     = SIGN_CERT_SUBJECT_NAME
EDSS_CERT_PRE_PROD_SIGN.SignToolType = SIGN_TOOL_EDSS
EDSS_CERT_PRE_PROD_SIGN.SignToolPath = EDSS_SIGN_TOOL_PATH
EDSS_CERT_PRE_PROD_SIGN.SubjectName  = EDSS_PRE_PROD_CERT_SUBJECT_NAME

EDSS_CERT_PROD_SIGN: SignCert    = SignCert ()
EDSS_CERT_PROD_SIGN.CertName     = 'EDSS Platform Production Certificate'
EDSS_CERT_PROD_SIGN.CertType     = SIGN_CERT_SUBJECT_NAME
EDSS_CERT_PROD_SIGN.SignToolType = SIGN_TOOL_EDSS
EDSS_CERT_PROD_SIGN.SignToolPath = EDSS_SIGN_TOOL_PATH
EDSS_CERT_PROD_SIGN.SubjectName  = EDSS_PROD_CERT_SUBJECT_NAME

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'EDKII_CERT_SIGN',
    'PLATFORM_CERT_SIGN',
    'EDSS_CERT_PRE_PROD_SIGN',
    'EDSS_CERT_PROD_SIGN',
    ]
