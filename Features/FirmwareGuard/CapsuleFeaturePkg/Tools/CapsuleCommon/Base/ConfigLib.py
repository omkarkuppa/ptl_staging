## @file
#  Library provided basic function to parse the configuration file.
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

import os
import toml
from typing import Any, List, MutableMapping, Union

from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import AbsPath, IsPathExist
from CapsuleCommon.Base.SearchLib import SearchValueInList

TOML_FILE_TYPE: str = 'TOML'

SUPPORTED_FILE_TYPE_LIST: List[str] = [
    TOML_FILE_TYPE,
    ]

class Config (object):
    def __init__ (
        self,
        FilePath: Union[str, os.PathLike],
        FileType: str = TOML_FILE_TYPE,
        ) -> None:
        """ Class to read the configuration file into MutableMapping.

        Args:
            FilePath (Union[str, os.PathLike]):
                The path of configuration file to do the parsing.
            FileType (str, optional):
                The input configuration structure type.
                This should be in one of SUPPORTED_FILE_TYPE_LIST list.
                Defaults to TOML_FILE_TYPE.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FilePath: str = AbsPath (FilePath)
        self.__FileType: str = FileType
        self.__PreCheckInput ()

        self.__File: str                      = ReadTextFile (self.__FilePath)
        self.__Data: MutableMapping[str, Any] = self.__Decode ()

    @property
    def Data (self) -> MutableMapping[str, Any]:
        """ To return the MutableMapping type configuration data.

        Note:
            It should be the key-value format.

        Args:
            None.

        Raises:
            None.

        Returns:
            MutableMapping[str, Any]:
                The configuration file data in memory.
        """
        return self.__Data

    def __PreCheckInput (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) The assigned path is not exist.
                (2) The assigned path is not file.
                (3) The assigned FileType is not supported.
            TypeError:
                The assigned FileType is not str.

        Returns:
            None.
        """
        if not IsPathExist (self.__FilePath):
            raise ValueError (f'Assigned path is not exist: {self.__FilePath}')
        elif not (IsFile (self.__FilePath)):
            raise ValueError (f'Assigned path is not file {self.__FilePath}')

        if not isinstance (self.__FileType, str):
            raise TypeError (f'Please assign the correct str type input.')

        IsFound, Result = SearchValueInList (
                            SUPPORTED_FILE_TYPE_LIST,
                            self.__FileType,
                            )

        if not IsFound:
            raise ValueError (
                    f'Please select the supported file type'
                    f'{SUPPORTED_FILE_TYPE_LIST}'
                    )

        self.__FileType = Result

        DEBUG (DEBUG_TRACE, f'CONFIG type to be decoded: {self.__FileType}')
        DEBUG (DEBUG_TRACE, f'CONFIG path to be decoded: {self.__FilePath}')

    def __Decode (self) -> MutableMapping[str, Any]:
        """ Decode the configuration file string by specific FileType.

        Args:
            None.

        Raises:
            ValueError:
                The input FileType is not supported.

        Returns:
            MutableMapping[str, Any]:
                The configuration file data in memory.
        """
        Result: Union[None, MutableMapping[str, Any]] = None

        if self.__FileType == TOML_FILE_TYPE:
            Result = toml.loads (self.__File)
        #
        # Here could extend to have more file type supported.
        #
        else:
            raise ValueError (f'Input is not supported type.')

        return Result
