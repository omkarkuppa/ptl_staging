## @file
#  Private API to generate the XDR format file.
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
from typing import List, Union

from CapsuleGenerate._Global import *

class XdrGeneratorApi (object):
    #
    # Python script used for this API.
    #
    PY_XDR_BUILDER_SCRIPT_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          gPYScriptPathInfo[XDR_BUILDER_SCRIPT_KEY],
          )

    def __init__ (
        self,
        FilePathList: list,
        OutputPath  : Union[str, os.PathLike],
        ) -> None:
        """ Class for API to build the XDR format file by list order.

        Args:
            FilePathList (list):
                List of path to be converted into XDR format.
                (List order equal place order)
            OutputPath (Union[str, os.PathLike]):
                Path of the XDR format file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FilePathList: List[str] = self.__GetFilePathList (FilePathList)
        self.__OutputPath  : str       = AbsPath (OutputPath)

    def __GetFilePathList (
        self,
        PathList: Union[None, str, os.PathLike],
        ) -> List[str]:
        """ Process the user input file path list.

        Note:
            (1) If input is None, append it as 'None' string.
            (2) If input is str, convert it into absolute path.
            (3) If input is not str type, raise TypeError.

        Args:
            PathList (Union[None, str, os.PathLike]):
                List of input path would be converted into XDR format.

        Raises:
            TypeError:
                Input path not None nor str.

        Returns:
            List[str]:
                List of the path string to be converted into XDR format.
                ('None' is reserved keyword to mark it as 0 length)
        """
        ResultList: List[str] = list ()

        for Path in PathList:
            #
            # Handle the case when input is None.
            #
            if IsNoneType (Path):
                ResultList.append (str (None))
                continue

            #
            # Handle the case when input is not str type.
            #
            if not isinstance (Path, str):
                raise TypeError (f'Input path should be None or str.')

            #
            # Check the path is exist and convert it into absolute path.
            #
            CheckFileListExist ([Path])
            ResultList.append (AbsPath (Path))

        return ResultList

    def Execute (self) -> int:
        """ Call the script to build the XDR format file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of XDR builder script.
                (Detail exit code should be reference its own)
        """
        DEBUG (DEBUG_INFO, f'>> Start to generate the XDR format file.')

        Cmd: List[str] = [
            f'{XdrGeneratorApi.PY_XDR_BUILDER_SCRIPT_PATH}',
            '-o', f'{self.__OutputPath}',
            ]

        for FilePath in self.__FilePathList:
            Cmd.append ('-i')
            Cmd.append (f'{FilePath}')

        ExitCode, _, _ = ExecPythonCmd (Cmd)

        DEBUG (DEBUG_INFO, f'>> Finish to generate the XDR format file.')

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'XdrGeneratorApi',
    ]
