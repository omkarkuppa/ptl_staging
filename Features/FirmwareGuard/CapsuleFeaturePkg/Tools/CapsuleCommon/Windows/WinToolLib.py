## @file
#  Library supported to get the Windows utility tool.
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

import glob
import os
from typing import List

from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import *
from CapsuleCommon.Base.SystemLib import *

from CapsuleCommon.Windows.ConstantLib import *

WIN_PROGRAM_FILE_X86_DIR_PATH: str = str ()
if IsWindowsPlatform ():
    WIN_PROGRAM_FILE_X86_DIR_PATH = os.getenv ('ProgramFiles(x86)')

#
# Pre-defined WDK path
#
WDK_DIR_NAME    : str = 'Windows Kits'
WDK_DIR_PATH    : str = JoinPath (
    WIN_PROGRAM_FILE_X86_DIR_PATH,
    WDK_DIR_NAME,
    )

#
# Pre-defined WDK binary path
#
WDK_BIN_DIR_NAME     : str       = 'bin'
WDK_BIN_DIR_PATH_LIST: List[str] = [
    JoinPath (WDK_BIN_DIR_NAME),
    JoinPath (WDK_BIN_DIR_NAME, '*'),
    ]

#
# Pre-defined WDK architecture
# Note: Place as new to old.
#
WDK_X86_ARCH_DIR_NAME: str       = 'x86'
WDK_X64_ARCH_DIR_NAME: str       = 'x64'
WDK_ARCH_LIST        : List[str] = [
    WDK_X64_ARCH_DIR_NAME,
    WDK_X86_ARCH_DIR_NAME,
    ]

#
# Pre-defined OS version
# Note: Place as new to old.
#
WDK_OS_VER_LIST   : List[str] = [
    OS_WINDOWS_10,
    OS_WINDOWS_8_1,
    ]

#
# Possible path
#   - Windows Kits\<OS_VER>\<bin>\<OS_ARCH>
#   - Windows Kits\<OS_VER>\<bin>\<WDK_VER>\<OS_ARCH>
# Note: Search order from top to bottom.
#
WDK_TOOL_PATH_LIST: List[str] = [
    #
    # Default WDK Installed Path
    #
    JoinPath (WDK_DIR_PATH, '10', WDK_BIN_DIR_NAME, WDK_X64_ARCH_DIR_NAME),
    JoinPath (WDK_DIR_PATH, '8.1', WDK_BIN_DIR_NAME, WDK_X64_ARCH_DIR_NAME),
    JoinPath (WDK_DIR_PATH, '10', WDK_BIN_DIR_NAME, '*', WDK_X64_ARCH_DIR_NAME),
    JoinPath (WDK_DIR_PATH, '10', WDK_BIN_DIR_NAME, WDK_X86_ARCH_DIR_NAME),
    JoinPath (WDK_DIR_PATH, '8.1', WDK_BIN_DIR_NAME, WDK_X86_ARCH_DIR_NAME),
    JoinPath (WDK_DIR_PATH, '10', WDK_BIN_DIR_NAME, '*', WDK_X86_ARCH_DIR_NAME),
    #
    # Possible Installed Path
    #
    JoinPath (WIN_OS_DISK, WDK_BIN_DIR_NAME, '*', WDK_X64_ARCH_DIR_NAME),
    JoinPath (WIN_OS_DISK, WDK_BIN_DIR_NAME, '*', WDK_X86_ARCH_DIR_NAME),
    ]

#
# Pre-defined tool program name
#
WIN_INF2CAT_TOOL_NAME : str = 'Inf2Cat.exe'
WIN_SIGNTOOL_TOOL_NAME: str = 'signtool.exe'

class WinWdkTool (object):
    def __init__ (self) -> None:
        """ Class to find the Windows platform WDK tool.

        Note:
            Provide the generic method to search/fetch the tool path.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Inf2CatToolPath: str = self.__FindTool (WIN_INF2CAT_TOOL_NAME)
        self.__SignToolPath   : str = self.__FindTool (WIN_SIGNTOOL_TOOL_NAME)

        self.__PrintInfo ()

    @property
    def Inf2Cat (self) -> str:
        """ Path of Inf2Cat program on Windows platform WDK.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of Inf2Cat program.
        """
        return self.__Inf2CatToolPath

    @property
    def SignTool (self) -> str:
        """ Path of signtool program on Windows platform WDK.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of signtool program.
        """
        return self.__SignToolPath

    def __PrintInfo (self) -> None:
        """ Print the debug related message on terminal.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DEBUG (DEBUG_INFO, f'Inf2Cat Path : {self.Inf2Cat}')
        DEBUG (DEBUG_INFO, f'SignTool Path: {self.SignTool}')

    def __FindTool (self, ToolName: str) -> str:
        """ Find the assign tool name from list of possible paths.

        Args:
            ToolName (str):
                The tool name to be search.
                (Need extension, case-sensitive)

        Raises:
            ValueError:
                The tool is not found within list of paths.

        Returns:
            str:
                Path of assigned Windows platform WDK tool.
        """
        CandidateList: List[str] = list ()
        ToolPath     : str       = str ()
        CandidatePath: str       = str ()

        #
        # Glob all paths from list of possible tool paths.
        #
        for Path in WDK_TOOL_PATH_LIST:
            CandidatePath = JoinPath (Path, ToolName)
            CandidateList = glob.glob (CandidatePath)

            DEBUG (DEBUG_TRACE, f'Candidate path: {CandidatePath}')
            DEBUG (DEBUG_TRACE, f'Candidate list: {CandidateList}')

            #
            # Check candidate path valid.
            #
            for Candidate in CandidateList:
                DEBUG (DEBUG_TRACE, f'Check the path: {Candidate}')

                if not IsFile (Candidate):
                    DEBUG(DEBUG_TRACE, f'{Candidate} is not file.')
                    continue

                ToolPath = Candidate
                break

            #
            # If find the tool, break immediately.
            #
            if ToolPath:
                break

        #
        # If WDK tool not found
        #
        if not ToolPath:
            DEBUG (
              DEBUG_ERROR,
              f'Please install the latest Windows WDK - '
              f'https://developer.microsoft.com/en-us/'
              f'windows/hardware/windows-driver-kit'
              )
            raise ValueError (f'Failed to locate the {ToolName}')

        return ToolPath
