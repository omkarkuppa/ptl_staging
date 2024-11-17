## @file
#  Wrapper to execute the OpenSSL "x509" command.
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

from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.Base.SearchLib import *
from CapsuleCommon.OpenSSL.Command.CmdX509 import *
from CapsuleCommon.OpenSSL.Constant.KeyForm import *

class OpenSSLX509KeyFormConvert (object):
    def __init__ (
        self,
        InputPath : Union[str, os.PathLike],
        InputForm : str,
        OutputPath: Union[str, os.PathLike],
        OutputForm: str,
        ) -> None:
        """ Class to convert the key form via OpenSSL program.

        Args:
            InputPath (Union[str, os.PathLike]):
                Path to the input file to be converted.
            InputForm (str):
                Key form of input file.
                Shall be one of OPENSSL_X509_KEY_IN_FORM_SUPPORTED_LIST.
            OutputPath (Union[str, os.PathLike]):
                Path to the output file to be stored.
            OutputForm (str):
                Key form of output file.
                Shall be one of OPENSSL_X509_KEY_OUT_FORM_SUPPORTED_LIST.

        Raises:
            None.

        Returns:
            None.
        """
        self.__InputPath : str = AbsPath (InputPath)
        self.__InputForm : str = InputForm
        self.__OutputPath: str = AbsPath (OutputPath)
        self.__OutputForm: str = OutputForm

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
                (1) InputForm is not str type.
                (2) OutputForm is not str type.
            ValueError:
                (1) InputForm is not in OPENSSL_X509_KEY_IN_FORM_SUPPORTED_LIST.
                (2) OutputForm is not in OPENSSL_X509_KEY_OUT_FORM_SUPPORTED_LIST.
                (3) InputForm and OutputForm is the same.

        Returns:
            None.
        """
        IsValid      : bool      = None
        Value        : Any       = None
        CheckPathList: List[str] = None

        if not isinstance (self.__InputForm, str):
            raise TypeError (f'InputForm shall be str type.')
        elif not isinstance (self.__OutputForm, str):
            raise TypeError (f'OutputForm shall be str type.')

        #
        # Check the input file exist.
        #
        CheckPathList = [
            self.__InputPath
            ]
        CheckFileListExist (CheckPathList)

        #
        # Check the key form of input is valid.
        #
        IsValid, Value = SearchValueInList (
                           InputList   = OPENSSL_X509_KEY_IN_FORM_SUPPORTED_LIST,
                           TargetValue = self.__InputForm,
                           )
        if not IsValid:
            raise ValueError (
                    f'InputForm - [{self.__InputForm}] is not valid. '
                    f'Shall be one of {OPENSSL_X509_KEY_IN_FORM_SUPPORTED_LIST}'
                    )
        self.__InputForm = Value

        #
        # Check the key form of output is valid.
        #
        IsValid, Value = SearchValueInList (
                           InputList   = OPENSSL_X509_KEY_OUT_FORM_SUPPORTED_LIST,
                           TargetValue = self.__OutputForm,
                           )
        if not IsValid:
            raise ValueError (
                    f'OutputForm - [{self.__OutputForm}] is not valid. '
                    f'Shall be one of {OPENSSL_X509_KEY_OUT_FORM_SUPPORTED_LIST}'
                    )
        self.__OutputForm = Value

        #
        # Check both of key form is not identical.
        #
        if self.__InputForm == self.__OutputForm:
            raise ValueError (f'InputForm is the same as OutputForm')

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
            OPENSSL_CMD_X509,
            OPENSSL_X509_FLAG_IN_FORM,  f'{self.__InputForm}',
            OPENSSL_X509_FLAG_IN,       f'{self.__InputPath}',
            OPENSSL_X509_FLAG_OUT_FORM, f'{self.__OutputForm}',
            OPENSSL_X509_FLAG_OUT,      f'{self.__OutputPath}',
            ])

        return CmdList
