## @file
#  Private API to generate the EDK-II FV for capsule payload.
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

from CapsuleGenerate._Global import *

class Edk2FdGeneratorApi (object):
    #
    # Python script used for this API.
    #
    PY_BUILDER_SCRIPT_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          gPYScriptPathInfo[EDKII_FD_BUILDER_SCRIPT_KEY],
          )

    def __init__ (
        self,
        FvName     : str,
        DscFilePath: Union[str, os.PathLike] = EDKII_DSC_FILE_PATH,
        FdfFilePath: Union[str, os.PathLike] = EDKII_FDF_FILE_PATH,
        ) -> None:
        """ Class for API to build the EDK-II FV via BaseTools.

        Note:
            FvName should be defined in FDF file.

        Args:
            FvName (str):
                The name of this firmware volume (FV).
                Caller have responsibility to provide the FDF file
                in EDK-II WORKSPACE.
            DscFilePath (Union[str, os.PathLike], optional):
                Path of the DSC file to be used.
                Defaults to EDKII_DSC_FILE_PATH.
            FdfFilePath (Union[str, os.PathLike], optional):
                Path of the FDF file to be used.
                Defaults to EDKII_FDF_FILE_PATH.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FvName     : str = FvName
        self.__DscFilePath: str = AbsPath (DscFilePath)
        self.__FdfFilePath: str = AbsPath (FdfFilePath)

        self.__PreCheck ()

        self.__FvNameUpper: str = self.__FvName.upper ()
        self.__FvPath     : str = \
            JoinPath (
              EDKII_BLD_FV_DIR_PATH,
              CombineFileName (
                self.__FvNameUpper,
                Extension = EXTENSION_FV_FILE,
                )
              )

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                FvName is not str type.
            ValueError:
                (1) FvName is empty.
                (2) FvName contain space character.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__DscFilePath,
            self.__FdfFilePath,
            ]

        CheckFileListExist (CheckPathList)

        if not isinstance (self.__FvName, str):
            raise TypeError (f'FvName should be str type.')
        elif not self.__FvName:
            raise ValueError (f'FvName can not empty.')
        elif ' ' in self.__FvName:
            raise ValueError (f'FvName can not contain space.')

    @property
    def FvPath (self) -> str:
        """ Return the path of the built FV.

        Note:
            The file name is all upper-case.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of the built FV.
        """
        return self.__FvPath

    @property
    def DscPath (self) -> str:
        """ Return the path of the DSC file to be used.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Absolute path of the DSC file to be used.
        """
        return self.__DscFilePath

    @property
    def FdfPath (self) -> str:
        """ Return the path of the FDF file to be used.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Absolute path of the FDF file to be used.
        """
        return self.__FdfFilePath

    def Execute (self) -> int:
        """ Call the script to generate the EDK-II FV.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of EDK-II FV generator script.
                (Detail exit code should be reference its own)
        """
        CmdList      : List[str] = [
            f'{Edk2FdGeneratorApi.PY_BUILDER_SCRIPT_PATH}',
            '-t',  f'{EDKII_TOOL_PATH}',
            '-w',  f'{EDKII_WORKSPACE_PATH}',
            '-d',  f'{self.__DscFilePath}',
            '-f',  f'{self.__FdfFilePath}',
            '-fv', f'{self.__FvName}',
            ]

        DEBUG (DEBUG_TRACE, f'DSC File Path: {self.__DscFilePath}')
        DEBUG (DEBUG_TRACE, f'FDF File Path: {self.__FdfFilePath}')
        DEBUG (DEBUG_TRACE, f'FV Name      : {self.__FvName}')

        ExitCode, _, _ = ExecPythonCmd (CmdList)

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'Edk2FdGeneratorApi',
    ]
