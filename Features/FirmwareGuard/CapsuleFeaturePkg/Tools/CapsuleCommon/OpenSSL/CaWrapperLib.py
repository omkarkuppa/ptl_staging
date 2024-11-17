## @file
#  Wrapper to execute the OpenSSL "ca" command.
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
from typing import List, Union

from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.OpenSSL.Command.CmdCa import *
from CapsuleCommon.OpenSSL.Constant.HashAlgorithm import *

class OpenSSLCaCsrHandle (object):
    def __init__ (
        self,
        Extensions     : str,
        CsrPath        : Union[str, os.PathLike],
        CaKeyPairPath  : Union[str, os.PathLike],
        CaKeyCrtPath   : Union[str, os.PathLike],
        OutputPath     : Union[str, os.PathLike],
        HashAlgorithm  : str = OPENSSL_HASH_ALGORITHM_SHA384_STR,
        Validity       : str = None,
        CaKeyPassphrase: str = None,
        ) -> None:
        """ Class to issue the certificate by CA from CSR file.

        Args:
            Extensions (str):
                Section in OpenSSL configuration file to specify
                the extensions to use for this certificate.
            CsrPath (Union[str, os.PathLike]):
                Path to the certificate request file.
            CaKeyPairPath (Union[str, os.PathLike]):
                Path to the CA key-pair file.
            CaKeyCrtPath (Union[str, os.PathLike]):
                Path to the CA certificate file.
            OutputPath (Union[str, os.PathLike]):
                Path to the output certificate file issued by CA.
            HashAlgorithm (str, optional):
                Hash algorithm of the digest.
                Defaults to OPENSSL_HASH_ALGORITHM_SHA384_STR.
            Validity (str, optional):
                Number of day on the validity of this certificate.
                (If not set, it would be decided by OpenSSL program.)
                Defaults to None.
            CaKeyPassphrase (str, optional):
                String of password to be protected the private key.
                (For the input CA key-pair.)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Extensions     : str = Extensions
        self.__CsrPath        : str = AbsPath (CsrPath)
        self.__CaKeyPairPath  : str = AbsPath (CaKeyPairPath)
        self.__CaKeyCrtPath   : str = AbsPath (CaKeyCrtPath)
        self.__HashAlgorithm  : str = HashAlgorithm
        self.__Validity       : str = Validity
        self.__CaKeyPassPhrase: str = CaKeyPassphrase
        self.__OutputPath     : str = AbsPath (OutputPath)

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
                (1) Extensions is not str type.
                (2) HashAlgorithm is not str type.
                (3) Validity is not None nor str type.
                (4) CaKeyPassphrase is not None nor str type.
            ValueError:
                HashAlgorithm is not in OPENSSL_HASH_ALGORITHM_STR_LIST.

        Returns:
            None.
        """
        if not isinstance (self.__Extensions, str):
            raise TypeError (f'Extensions shall be str type.')
        elif not isinstance (self.__HashAlgorithm, str):
            raise TypeError (f'HashAlgorithm shall be str type.')
        elif not isinstance (self.__Validity, (type (None), str)):
            raise TypeError (f'Validity shall be None or str type.')
        elif not isinstance (self.__CaKeyPassPhrase, (type (None), str)):
            raise TypeError (f'CaKeyPassphrase shall be None or str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__CsrPath,
            self.__CaKeyPairPath,
            self.__CaKeyCrtPath,
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
            OPENSSL_CMD_CA,
            OPENSSL_CA_FLAG_VERBOSE,
            OPENSSL_CA_FLAG_BATCH,
            OPENSSL_CA_FLAG_EXTENSIONS, f'{self.__Extensions}',
            OPENSSL_CA_FLAG_MD,         f'{self.__HashAlgorithm}',
            OPENSSL_CA_FLAG_CERT,       f'{self.__CaKeyCrtPath}',
            OPENSSL_CA_FLAG_KEY_FILE,   f'{self.__CaKeyPairPath}',
            OPENSSL_CA_FLAG_IN,         f'{self.__CsrPath}',
            OPENSSL_CA_FLAG_OUT,        f'{self.__OutputPath}',
            ])

        if self.__Validity:
            CmdList.extend ([
                OPENSSL_CA_FLAG_DAYS, f'{self.__Validity}',
                ])

        if self.__CaKeyPassPhrase:
            CmdList.extend ([
                OPENSSL_CA_FLAG_PASS_IN, f'{self.__CaKeyPassPhrase}',
                ])

        return CmdList
