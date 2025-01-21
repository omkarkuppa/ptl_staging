## @file
#  Wrapper to execute the OpenSSL "genrsa" command.
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
from typing import Any, List, Union

from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.Base.SearchLib import *
from CapsuleCommon.OpenSSL.Command.CmdGenRsa import *
from CapsuleCommon.OpenSSL.Constant.Cipher import *
from CapsuleCommon.OpenSSL.Constant.KeyLength import *

class OpenSSLGenRsa (object):
    def __init__ (
        self,
        KeyLength : str,
        OutputPath: Union[str, os.PathLike],
        Cipher    : str = OPENSSL_CIPHER_AES_256_STR,
        Passphrase: str = None,
        ) -> None:
        """ Class to generate RSA key-pair via OpenSSL program.

        Args:
            KeyLength (str):
                Bit length of the key.
                Shall be one of OPENSSL_KEY_LENGTH_STR_LIST.
            OutputPath (Union[str, os.PathLike]):
                Path to the output RSA key-pair file.
            Cipher (str, optional):
                Algorithm for the cipher of this key.
                Shall be one of OPENSSL_GEN_RSA_CIPHER_SUPPORTED_LIST.
                This value is matter when passphrase is set along only.
                Defaults to OPENSSL_CIPHER_AES_256_STR.
            Passphrase (str, optional):
                String of password to be protected the private key.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__KeyLength : str = KeyLength
        self.__Cipher    : str = Cipher
        self.__Passphrase: str = Passphrase
        self.__OutputPath: str = AbsPath (OutputPath)

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
                (1) KeyLength is not str type.
                (2) Cipher is not str type.
                (3) Passphrase is not None nor str type.
            ValueError:
                (1) KeyLength is not in OPENSSL_KEY_LENGTH_STR_LIST.
                (2) Cipher is not in OPENSSL_GEN_RSA_CIPHER_SUPPORTED_LIST.

        Returns:
            None.
        """
        IsValid: bool = None
        Value  : Any  = None

        if not isinstance (self.__KeyLength, str):
            raise TypeError (f'KeyLength shall be str type.')
        elif not isinstance (self.__Cipher, str):
            raise TypeError (f'Cipher shall be str type.')
        elif not isinstance (self.__Passphrase, (type (None), str)):
            raise TypeError (f'Passphrase is shall be None or str type.')

        #
        # Check the requested key length is valid.
        #
        if self.__KeyLength not in OPENSSL_KEY_LENGTH_STR_LIST:
            raise ValueError (
                    f'Key length - [{self.__KeyLength}] is not valid. '
                    f'Shall be one of {OPENSSL_KEY_LENGTH_STR_LIST}.'
                    )

        #
        # Check the requested cipher algorithm is supported.
        #
        if self.__Passphrase:
            IsValid, Value = SearchValueInList (
                               InputList   = OPENSSL_GEN_RSA_CIPHER_SUPPORTED_LIST,
                               TargetValue = self.__Cipher,
                               )
            if not IsValid:
                raise ValueError (
                        f'Cipher - [{self.__Cipher}] is not valid. '
                        f'Shall be one of {OPENSSL_GEN_RSA_CIPHER_SUPPORTED_LIST}.'
                        )

            self.__Cipher = Value

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
            OPENSSL_CMD_GEN_RSA,
            OPENSSL_GEN_RSA_FLAG_VERBOSE,
            ])

        if self.__Passphrase:
            CmdList.extend ([
                f'-{self.__Cipher}',
                OPENSSL_GEN_RSA_FLAG_PASS_OUT, f'{self.__Passphrase}',
                ])

        CmdList.extend ([
            OPENSSL_GEN_RSA_FLAG_OUT, f'{self.__OutputPath}',
            self.__KeyLength,
            ])

        return CmdList
