## @file
#  Wrapper to execute the OpenSSL "pkcs12" command.
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
from CapsuleCommon.OpenSSL.Command.CmdPkcs12 import *

#
# Default empty password for PKCS#12.
# It is mandatory to encrypt the certificate and private key.
#
PKCS12_EMPTY_PASSWD: str = 'pass:'

class OpenSSLPkcs12ToPfx (object):
    def __init__ (
        self,
        KeyPairPath  : Union[str, os.PathLike],
        CrtPath      : Union[str, os.PathLike],
        OutputPath   : Union[str, os.PathLike],
        KeyPassphrase: str = None,
        PfxPassphrase: str = None,
        ) -> None:
        """ Class to convert the key-pair into PKCS#12 format.

        Args:
            KeyPairPath (Union[str, os.PathLike]):
                Path to the key-pair file.
            CrtPath (Union[str, os.PathLike]):
                Path to the certificate file.
            OutputPath (Union[str, os.PathLike]):
                Path to the output PFX file.
            KeyPassphrase (str, optional):
                String of password to be protected the private key.
                (For input key-pair file.)
                Defaults to None.
            PfxPassphrase (str, optional):
                String of password to be protected the private key.
                (For output PFX file.)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__KeyPairPath  : str = AbsPath (KeyPairPath)
        self.__KeyPassphrase: str = KeyPassphrase
        self.__CrtPath      : str = AbsPath (CrtPath)
        self.__PfxPassphrase: str = PfxPassphrase
        self.__OutputPath   : str = AbsPath (OutputPath)

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
                (1) KeyPassphrase is not None nor str type.
                (2) PfxPassphrase is not None nor str type.

        Returns:
            None.
        """
        if not isinstance (self.__KeyPassphrase, (type (None), str)):
            raise TypeError (f'KeyPassphrase is shall be None or str type.')
        elif not isinstance (self.__PfxPassphrase, (type (None), str)):
            raise TypeError (f'PfxPassphrase is shall be None or str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__KeyPairPath,
            self.__CrtPath,
            ]
        CheckFileListExist (CheckPathList)

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
        PfxPass: str       = PKCS12_EMPTY_PASSWD

        if self.__PfxPassphrase:
            PfxPass = f'{self.__PfxPassphrase}'

        CmdList.extend ([
            OPENSSL_CMD_PKCS12,
            OPENSSL_PKCS12_FLAG_EXPORT,
            OPENSSL_PKCS12_FLAG_IN_KEY,   f'{self.__KeyPairPath}',
            OPENSSL_PKCS12_FLAG_IN,       f'{self.__CrtPath}',
            OPENSSL_PKCS12_FLAG_PASS_OUT, f'{PfxPass}',
            OPENSSL_PKCS12_FLAG_OUT,      f'{self.__OutputPath}',
            ])

        if self.__KeyPassphrase:
            CmdList.extend ([
                OPENSSL_PKCS12_FLAG_PASS_IN, f'{self.__KeyPassphrase}',
                ])

        return CmdList

class OpenSSLPkcs12ParsePrivateKey (object):
    def __init__ (
        self,
        PfxPath   : Union[str, os.PathLike],
        OutputPath: Union[str, os.PathLike],
        Passphrase: str = None,
        ) -> None:
        """ Class to extract the private key without password.

        Args:
            PfxPath (Union[str, os.PathLike]):
                Path to the PFX file used to extract the private key.
            OutputPath (Union[str, os.PathLike]):
                Path to the output private key file without password.
            Passphrase (str, optional):
                String of password to be protected the private key.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__PfxPath   : str = AbsPath (PfxPath)
        self.__Passphrase: str = Passphrase
        self.__OutputPath: str = AbsPath (OutputPath)

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
                Passphrase is not None nor str type.

        Returns:
            None.
        """
        if not isinstance (self.__Passphrase, (type (None), str)):
            raise TypeError (f'Passphrase is shall be None or str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__PfxPath,
            ]
        CheckFileListExist (CheckPathList)

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
        CmdList   : List[str] = list ()
        Passphrase: str       = PKCS12_EMPTY_PASSWD

        if self.__Passphrase:
            Passphrase = f'{self.__Passphrase}'

        CmdList.extend ([
            OPENSSL_CMD_PKCS12,
            OPENSSL_PKCS12_FLAG_NO_ENCRYPT,
            OPENSSL_PKCS12_FLAG_IN,         f'{self.__PfxPath}',
            OPENSSL_PKCS12_FLAG_PASS_IN,    f'{Passphrase}',
            OPENSSL_PKCS12_FLAG_OUT,        f'{self.__OutputPath}',
            ])

        return CmdList
