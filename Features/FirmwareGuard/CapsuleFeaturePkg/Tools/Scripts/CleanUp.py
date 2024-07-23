## @file
#  Script use to clean up the toolkit temporary/cache/output files
#  and directories.
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
import argparse
from typing import List

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2023 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to clean up the folders/files by rule.\n'

#
# The list of directory name to be clean up.
#
CLEANUP_TARGET_FOLDERS_LIST: List[str] = [
    '__pycache__',
    ]

#
# The list of extension name to be clean up.
#
CLEANUP_TARGET_FILE_EXTENSIONS_LIST: List[str] = [
    'pyc',
    ]

class CleanUp (object):
    def __init__ (
        self,
        RootPath      : Union[str, os.PathLike],
        FolderPathList: List[Union[str, os.PathLike]],
        ExtensionList : List[str],
        ) -> None:
        """ Class used to clean up the directory/file by rule.

        Args:
            RootPath (Union[str, os.PathLike]):
                The root of path to be cleaned up. (Include sub-folder)
            FolderPathList (List[Union[str, os.PathLike]]):
                List of directory path name to be removed.
            ExtensionList (List[str]):
                List of extension to be removed.

        Raises:
            None.

        Returns:
            None.
        """
        self.__RootPath      : str       = AbsPath (RootPath)
        self.__FolderPathList: List[str] = \
            self.__ConquerList (
                   CLEANUP_TARGET_FOLDERS_LIST,
                   FolderPathList,
                   )
        self.__ExtensionList : List[str] = \
            self.__ConquerList (
                   CLEANUP_TARGET_FILE_EXTENSIONS_LIST,
                   ExtensionList,
                   )

    def __ConquerList (self, *Args: Union[List, str]) -> List[str]:
        """ To merge the list of string/string into one list.

        Args:
            *Args (Union[List, str]):
                Input of string or list to be merged into one list.

        Raises:
            TypeError:
                (1) Input is not string/list.
                (2) The member of list is not string.

        Returns:
            List[str]:
                The list of string after conquer.
                (The list would bypass the set () function)
        """
        Result: List[str] = list ()

        for Arg in Args:
            if isinstance (Arg, str):
                Result.append (Arg)
            elif isinstance (Arg, list):
                for SubArg in Arg:
                    if isinstance (SubArg, str):
                        Result.append (SubArg)
                    else:
                        raise TypeError (
                                f'Input should be str. but {type (SubArg)}.'
                                )
            else:
                raise TypeError (
                        f'Input should be list/str. but {type (SubArg)}.'
                        )

        return list (set (Result))

    def Clean (self) -> None:
        """ The main function to do the clean up operation.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        CleanUpFolderPathList: List[str] = list ()
        CleanUpFilePathList  : List[str] = list ()
        Root                 : str       = None
        Folders              : List[str] = None
        Files                : List[str] = None
        Folder               : str       = None
        File                 : str       = None
        Path                 : str       = None
        Result               : bool      = None

        DEBUG (DEBUG_TRACE, f'Cleanup Root Path     : {self.__RootPath}')
        DEBUG (DEBUG_TRACE, f'Cleanup Folder Path   : {self.__FolderPathList}')
        DEBUG (DEBUG_TRACE, f'Cleanup Extension Path: {self.__ExtensionList}')

        for Root, Folders, Files in os.walk (self.__RootPath, topdown = False):
            for Folder in Folders:
                if Folder in self.__FolderPathList:
                    CleanUpFolderPathList.append (JoinPath (Root, Folder))

            for File in Files:
                if File.endswith (tuple (self.__ExtensionList)):
                    CleanUpFilePathList.append (JoinPath (Root, File))

        for Path in CleanUpFilePathList:
            DEBUG (DEBUG_TRACE, f'Now cleanup the file [{Path}]')
            Result = RemoveFile (Path, IsSafeRemove = True)
            if not Result:
                DEBUG (DEBUG_INFO, f'No need take action: {Path}')

        for Path in CleanUpFolderPathList:
            DEBUG (DEBUG_TRACE, f'Now cleanup the folder [{Path}]')
            Result = RemoveDir (Path, IsSafeRemove = True)
            if not Result:
                DEBUG (DEBUG_INFO, f'No need take action: {Path}')

if __name__ == '__main__':
    def GetArgs () -> argparse.Namespace:
        """ Get the user input arguments.

        Args:
            None.

        Raises:
            None.

        Returns:
            Args [argparse.Namespace]:
                The argument object parsing from user input.
        """
        Parser: argparse.ArgumentParser = argparse.\
            ArgumentParser (
              prog            = __prog__,
              description     = __copyright__ + __description__ + __version__,
              formatter_class = argparse.RawTextHelpFormatter,
              )
        Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action  = 'version',
                 version = __version__,
                 )
        Parser.add_argument (
                 '-r',
                 '-R',
                 '--root',
                 dest     = 'RootPath',
                 help     = 'The start of root path to be checked.',
                 default  = '.',
                 required = False,
                 )
        Parser.add_argument (
                 '-d',
                 '-d',
                 '--dir',
                 dest     = 'Dir',
                 help     = 'The list of directory name.',
                 action   = 'append',
                 default  = list (),
                 required = False,
                 )
        Parser.add_argument (
                 '-e',
                 '-E',
                 '--ext',
                 dest     = 'Ext',
                 help     = 'The list of extension name.',
                 action   = 'append',
                 default  = list (),
                 required = False,
                 )

        return Parser.parse_args ()

    DEBUG (DEBUG_INFO, f'--------------- CleanUp Script Start ---------------')

    Args: argparse.Namespace = GetArgs ()

    RootPath: str       = AbsPath (Args.RootPath)
    DirList : List[str] = Args.Dir
    ExtList : List[str] = Args.Ext

    Cleaner: CleanUp = CleanUp (
                         RootPath       = RootPath,
                         FolderPathList = DirList,
                         ExtensionList  = ExtList,
                         )

    Cleaner.Clean ()

    DEBUG (DEBUG_INFO, f'---------------- CleanUp Script End ----------------')
