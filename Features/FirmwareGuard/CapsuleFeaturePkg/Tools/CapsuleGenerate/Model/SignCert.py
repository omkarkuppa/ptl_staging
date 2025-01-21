## @file
#  Description a certificate sign object should expose what methods.
#
#  Note:
#    The main purpose of this object is used to unify the sign
#    certificate behavior and enhance the porting to another sign tool.
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

from typing import List

from CapsuleCommon import *

SIGN_X509_CERT_FILE   : int = 0x0000_0000
SIGN_CERT_SUBJECT_NAME: int = 0x0000_0001
SIGN_CERT_UNKNOWN     : int = 0xFFFF_FFFF

SIGN_TOOL_OPENSSL: int = 0x0000_0000
SIGN_TOOL_WINDOWS: int = 0x0000_0001
SIGN_TOOL_EDSS   : int = 0x0000_0002
SIGN_TOOL_UNKNOWN: int = 0xFFFF_FFFF

EDSS_SIGNED_KEY: str = 'EDSS_SIGNED'

class SignCert (object):
    def __init__ (self):
        """ Class to represent the FMP sign certificate object.

        Note:
            It is caller responsibility to provide the properly data
            before call the relevant function.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.CertName    : str = 'Un-assigned Cert Name'
        self.CertType    : int = SIGN_CERT_UNKNOWN
        self.SignToolType: int = SIGN_TOOL_UNKNOWN
        self.SignToolPath: str = None

        #
        # SIGN_X509_CERT_FILE mandatory
        #
        self.RootPubKeyPath  : str = None
        self.IntraPubKeyPath : str = None
        self.SignerPriKeyPath: str = None

        #
        # SIGN_CERT_SUBJECT_NAME mandatory.
        #
        self.SubjectName: str = None

    def __PreCheck (self) -> None:
        """ Class used to check the user input meet the requirement.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        CheckPathList: List[str] = list ()

        if self.SignToolType in [SIGN_TOOL_OPENSSL, SIGN_TOOL_EDSS]:
            CheckPathList.append (self.SignToolPath)

        CheckDirListExist (CheckPathList)

    def __CheckX509CertFileValid (self) -> None:
        """ Check input data meet the X.509 sign file requirement.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.RootPubKeyPath,
            self.IntraPubKeyPath,
            self.SignerPriKeyPath,
            ]

        CheckFileListExist (CheckPathList)

    def __GetX509SignCommand (self) -> List[str]:
        """ Return the X.509 sign command list.

        Args:
            None.

        Raises:
            ValueError:
                Sign tool type is unsupported.

        Returns:
            List[str]:
                List of command string used to sign the capsule file.
        """
        if self.SignToolType != SIGN_TOOL_OPENSSL:
            raise ValueError (f'Unsupported sign tool type.')

        DEBUG (DEBUG_INFO, f'Sign Tool Path  : {self.SignToolPath}')
        DEBUG (DEBUG_INFO, f'Root   Pub. File: {self.RootPubKeyPath}')
        DEBUG (DEBUG_INFO, f'Intra  Pub. File: {self.IntraPubKeyPath}')
        DEBUG (DEBUG_INFO, f'Signer Pri. File: {self.SignerPriKeyPath}')
        return [
            '--signing-tool-path',   f'{self.SignToolPath}',
            '--signer-private-cert', f'{self.SignerPriKeyPath}',
            '--other-public-cert',   f'{self.IntraPubKeyPath}',
            '--trusted-public-cert', f'{self.RootPubKeyPath}',
            ]

    def __CheckCertSubjectNameValid (self) -> None:
        """ Check input data meet the cert name sign requirement.

        Args:
            None.

        Raises:
            ValueError:
                Subject name is empty.

        Returns:
            None.
        """
        if (self.SubjectName is None) or (not self.SubjectName):
            raise ValueError (f'Certificate subject name can not empty.')

    def __GetCertSubjectNameSignCommand (self) -> List[str]:
        """ Return the certificate subject name sign command list.

        Args:
            None.

        Raises:
            ValueError:
                Sign tool type is unsupported.

        Returns:
            List[str]:
                List of command string used to sign the capsule file.
        """
        CmdList: List[str] = list ()
        if self.SignToolType not in [SIGN_TOOL_WINDOWS, SIGN_TOOL_EDSS]:
            raise ValueError ('Unsupported sign tool type.')

        DEBUG (DEBUG_INFO, f'Sign with subject name: {self.SubjectName}')

        CmdList = [
            '--subject-name', f'{self.SubjectName}',
            ]

        if self.SignToolType is not None:
            CmdList += [
                '--signing-tool-path', f'{self.SignToolPath}',
                ]

        return CmdList

    @property
    def Command (self) -> List[str]:
        """ Return the sign command for OpenSSL.

        Note:
            Command would return based on the CertType.

        Args:
            None.

        Raises:
            ValueError:
                CertType is not supported.

        Returns:
            List[str]:
                List of command string used to sign the capsule file.
        """
        CmdList: List[str] = list ()

        self.__PreCheck ()

        if self.CertType == SIGN_X509_CERT_FILE:
            #
            # X.509 certificate file input.
            #
            self.__CheckX509CertFileValid ()
            CmdList = self.__GetX509SignCommand ()
        elif self.CertType == SIGN_CERT_SUBJECT_NAME:
            #
            # Certificate subject name sign.
            #
            self.__CheckCertSubjectNameValid ()
            CmdList = self.__GetCertSubjectNameSignCommand ()
        else:
            raise ValueError (
                    f'Certificate type is not valid. [{self.CertType}]'
                    )

        return CmdList
