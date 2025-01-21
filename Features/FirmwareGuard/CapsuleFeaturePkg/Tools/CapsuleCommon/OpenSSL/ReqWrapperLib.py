## @file
#  Wrapper to execute the OpenSSL "req" command.
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
#
##

import os
from typing import Any, Dict, List, Union

from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.Base.SearchLib import *
from CapsuleCommon.OpenSSL.Command.CmdReq import *
from CapsuleCommon.OpenSSL.Constant.HashAlgorithm import *
from CapsuleCommon.OpenSSL.Constant.Subject import *

class OpenSSLSubject (object):
    def __init__ (
        self,
        CommonName          : str,
        CountryName         : str = None,
        StateOrProvinceName : str = None,
        LocalName           : str = None,
        OrganizationName    : str = None,
        OrganizationUnitName: str = None,
        Email               : str = None,
        ) -> None:
        """ Class to initialize OpenSSL subject with provided attributes.

        Args:
            CommonName (str):
                The common name to be used in the certificate.
                (Shall be unique.)
            CountryName (str, optional):
                The country name to be used in the certificate.
                Defaults to None.
            StateOrProvinceName (str, optional):
                The state or province name to be used in the certificate.
                Defaults to None.
            LocalName (str, optional):
                The local name to be used in the certificate.
                Defaults to None.
            OrganizationName (str, optional):
                The organization name to be used in the certificate
                Defaults to None.
            OrganizationUnitName (str, optional):
                The organization unit name to be used in the certificate.
                Defaults to None.
            Email (str, optional):
                The Email address to be used in the certificate.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__CommonName          : str = CommonName
        self.__CountryName         : str = CountryName
        self.__StateOrProvinceName : str = StateOrProvinceName
        self.__LocalName           : str = LocalName
        self.__OrganizationName    : str = OrganizationName
        self.__OrganizationUnitName: str = OrganizationUnitName
        self.__Email               : str = Email

        self.__PreCheck ()

    @property
    def String (self) -> str:
        """ Return the subject name string in OpenSSL format.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String of the subject name.
        """
        SubjectStrList: List[str] = list ()

        MAPPING_TABLE: Dict[str, str] = {
            OPENSSL_SUBJECT_COUNTRY_NAME_STR          : self.__CountryName,
            OPENSSL_SUBJECT_STATE_OR_PROVINCE_NAME_STR: self.__StateOrProvinceName,
            OPENSSL_SUBJECT_LOCAL_NAME_STR            : self.__LocalName,
            OPENSSL_SUBJECT_ORGANIZATION_NAME_STR     : self.__OrganizationName,
            OPENSSL_SUBJECT_ORGANIZATION_UNIT_NAME_STR: self.__OrganizationUnitName,
            OPENSSL_SUBJECT_COMMON_NAME_STR           : self.__CommonName,
            OPENSSL_SUBJECT_EMAIL_STR                 : self.__Email,
            }

        for Key, Value in MAPPING_TABLE.items ():
            if Value:
                SubjectStrList.append (f'/{Key}={Value}')

        return ''.join (SubjectStrList)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) CommonName is not str type.
                (2) CountryName is not None nor str type.
                (3) StateOrProvinceName is not None nor str type.
                (4) LocalName is not None nor str type.
                (5) OrganizationName is not None nor str type.
                (6) OrganizationUnitName is not None nor str type.
                (7) Email is not None nor str type.

        Returns:
            None.
        """
        if not isinstance (self.__CommonName, str):
            raise TypeError (f'CommonName shall be str type.')
        elif not isinstance (self.__CountryName, (type (None), str)):
            raise TypeError (f'CountryName is shall be None or str type.')
        elif not isinstance (self.__StateOrProvinceName, (type (None), str)):
            raise TypeError (f'StateOrProvinceName is shall be None or str type.')
        elif not isinstance (self.__LocalName, (type (None), str)):
            raise TypeError (f'LocalName is shall be None or str type.')
        elif not isinstance (self.__OrganizationName, (type (None), str)):
            raise TypeError (f'OrganizationName is shall be None or str type.')
        elif not isinstance (self.__OrganizationUnitName, (type (None), str)):
            raise TypeError (f'OrganizationUnitName is shall be None or str type.')
        elif not isinstance (self.__Email, (type (None), str)):
            raise TypeError (f'Email is shall be None or str type.')

class OpenSSLReqSelfSigned (object):
    def __init__ (
        self,
        KeyPairPath  : Union[str, os.PathLike],
        Extensions   : str,
        Subject      : OpenSSLSubject,
        OutputPath   : Union[str, os.PathLike],
        HashAlgorithm: str = OPENSSL_HASH_ALGORITHM_SHA384_STR,
        Validity     : str = None,
        Passphrase   : str = None,
        ) -> None:
        """ Class to generate the self-signed certificate.

        Args:
            KeyPairPath (Union[str, os.PathLike]):
                Path to the key-pair file.
            Extensions (str):
                Section in OpenSSL configuration file to specify
                the extensions to use for this certificate.
            Subject (OpenSSLSubject):
                Subject attribute of this certificate.
            OutputPath (Union[str, os.PathLike]):
                Path to the output self-signed certificate file.
            HashAlgorithm (str, optional):
                Hash algorithm of the digest.
                Defaults to OPENSSL_HASH_ALGORITHM_SHA384_STR.
            Validity (str, optional):
                Number of day on the validity of this certificate.
                (If not set, it would be decided by OpenSSL program.)
                Defaults to None.
            Passphrase (str, optional):
                String of password to be protected the private key.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__KeyPairPath  : str            = AbsPath (KeyPairPath)
        self.__Extensions   : str            = Extensions
        self.__Subject      : OpenSSLSubject = Subject
        self.__HashAlgorithm: str            = HashAlgorithm
        self.__Validity     : str            = Validity
        self.__Passphrase   : str            = Passphrase
        self.__OutputPath   : str            = AbsPath (OutputPath)

        self.__PreCheck ()

        self.__CmdList: List[str] = self.__GetCmdList ()

    @property
    def CmdList (self) -> List[str]:
        """ Return the list of command to execute the OpenSSL program.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command to execute the OpenSSL program.
        """
        return self.__CmdList

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Extensions is not str type.
                (2) Subject is not OpenSSLSubject type.
                (3) HashAlgorithm is not str type.
                (4) Validity is not None nor str type.
                (5) Passphrase is not None nor str type.
            ValueError:
                HashAlgorithm is not in OPENSSL_HASH_ALGORITHM_STR_LIST.

        Returns:
            None.
        """
        IsValid      : bool      = None
        Value        : Any       = None
        CheckPathList: List[str] = None

        if not isinstance (self.__Extensions, str):
            raise TypeError (f'Extensions shall be str type.')
        elif not isinstance (self.__Subject, OpenSSLSubject):
            raise TypeError (f'Subject shall be OpenSSLSubject type.')
        elif not isinstance (self.__HashAlgorithm, str):
            raise TypeError (f'HashAlgorithm shall be str type.')
        elif not isinstance (self.__Validity, (type (None), str)):
            raise TypeError (f'Validity shall be None or str type.')
        elif not isinstance (self.__Passphrase, (type (None), str)):
            raise TypeError (f'Passphrase shall be None or str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__KeyPairPath,
            ]
        CheckFileListExist (CheckPathList)

        #
        # Check the hash algorithm is valid.
        #
        IsValid, Value = SearchValueInList (
                            InputList   = OPENSSL_HASH_ALGORITHM_STR_LIST,
                            TargetValue = self.__HashAlgorithm,
                            )
        if not IsValid:
            raise ValueError (
                f'HashAlgorithm - [{self.__HashAlgorithm}] is not valid. '
                f'Shall be one of {OPENSSL_HASH_ALGORITHM_STR_LIST}.'
                )

        self.__HashAlgorithm = Value

    def __GetCmdList (self) -> List[str]:
        """ Return the list of command to execute the OpenSSL program.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command to execute the OpenSSL program.
        """
        CmdList: List[str] = list ()

        CmdList.extend ([
            OPENSSL_CMD_REQ,
            OPENSSL_REQ_FLAG_VERBOSE,
            OPENSSL_REQ_FLAG_EXTENSIONS, f'{self.__Extensions}',
            OPENSSL_REQ_FLAG_NEW,
            OPENSSL_REQ_FLAG_X509,
            f'-{self.__HashAlgorithm}',
            OPENSSL_REQ_FLAG_DAYS,       f'{self.__Validity}',
            OPENSSL_REQ_FLAG_SUBJECT,    f'{self.__Subject.String}',
            OPENSSL_REQ_FLAG_KEY,        f'{self.__KeyPairPath}',
            OPENSSL_REQ_FLAG_OUT,        f'{self.__OutputPath}',
            ])

        if self.__Passphrase:
            CmdList.extend ([
                OPENSSL_REQ_FLAG_PASS_IN, f'{self.__Passphrase}'
                ])

        return CmdList

class OpenSSLReqCsr (object):
    def __init__ (
        self,
        KeyPairPath  : Union[str, os.PathLike],
        Subject      : OpenSSLSubject,
        OutputPath   : Union[str, os.PathLike],
        HashAlgorithm: str = OPENSSL_HASH_ALGORITHM_SHA384_STR,
        Passphrase   : str = None,
        ) -> None:
        """ Class to generate the certificate request (CSR).

        Args:
            KeyPairPath (Union[str, os.PathLike]):
                Path to the key-pair file.
            Subject (OpenSSLSubject):
                Subject attribute of this certificate.
            OutputPath (Union[str, os.PathLike]):
                Path to the output certificate request (CSR) file.
            HashAlgorithm (str, optional):
                Hash algorithm of the digest.
                Defaults to OPENSSL_HASH_ALGORITHM_SHA384_STR.
            Passphrase (str, optional):
                String of password to be protected the private key.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__KeyPairPath  : str            = AbsPath (KeyPairPath)
        self.__Subject      : OpenSSLSubject = Subject
        self.__HashAlgorithm: str            = HashAlgorithm
        self.__Passphrase   : str            = Passphrase
        self.__OutputPath   : str            = AbsPath (OutputPath)

        self.__PreCheck ()

        self.__CmdList: List[str] = self.__GetCmdList ()

    @property
    def Command (self) -> List[str]:
        """ Return the list of command to execute the OpenSSL program.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command to execute the OpenSSL program.
        """
        return self.__CmdList

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Subject is not OpenSSLSubject type.
                (2) HashAlgorithm is not str type.
                (3) Passphrase is not None nor str type.
            ValueError:
                HashAlgorithm is not in OPENSSL_HASH_ALGORITHM_STR_LIST.

        Returns:
            None.
        """
        IsValid      : bool      = None
        Value        : Any       = None
        CheckPathList: List[str] = None

        if not isinstance (self.__Subject, OpenSSLSubject):
            raise TypeError (f'Subject shall be OpenSSLSubject type.')
        elif not isinstance (self.__HashAlgorithm, str):
            raise TypeError (f'HashAlgorithm shall be str type.')
        elif not isinstance (self.__Passphrase, (type (None), str)):
            raise TypeError (f'Passphrase shall be None or str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__KeyPairPath,
            ]
        CheckFileListExist (CheckPathList)

        #
        # Check the hash algorithm is valid.
        #
        IsValid, Value = SearchValueInList (
                            InputList   = OPENSSL_HASH_ALGORITHM_STR_LIST,
                            TargetValue = self.__HashAlgorithm,
                            )
        if not IsValid:
            raise ValueError (
                f'HashAlgorithm - [{self.__HashAlgorithm}] is not valid. '
                f'Shall be one of {OPENSSL_HASH_ALGORITHM_STR_LIST}.'
                )

        self.__HashAlgorithm = Value

    def __GetCmdList (self) -> List[str]:
        """ Return the list of command to execute the OpenSSL program.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of command to execute the OpenSSL program.
        """
        CmdList: List[str] = list ()

        CmdList.extend ([
            OPENSSL_CMD_REQ,
            OPENSSL_REQ_FLAG_VERBOSE,
            OPENSSL_REQ_FLAG_NEW,
            f'-{self.__HashAlgorithm}',
            OPENSSL_REQ_FLAG_KEY,       f'{self.__KeyPairPath}',
            OPENSSL_REQ_FLAG_SUBJECT,   f'{self.__Subject.String}',
            OPENSSL_REQ_FLAG_OUT,       f'{self.__OutputPath}',
            ])

        if self.__Passphrase:
            CmdList.extend ([
                OPENSSL_REQ_FLAG_PASS_IN, f'{self.__Passphrase}',
                ])

        return CmdList
