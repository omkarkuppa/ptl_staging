## @file
#  Library provided basic function to get the user environment.
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

import sys
import platform
from typing import List

class UserEnvironment (object):
    def __init__ (self) -> None:
        """ Class to get the user system information.

        Note:
            This is used to support to debug the tool.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Argv      : List[str] = sys.argv
        self.__OSSystem  : str       = platform.system ()
        self.__OSRelease : str       = platform.release ()
        self.__OSVersion : str       = platform.version ()
        self.__OSPlatform: str       = platform.platform ()

        self.__PythonVersion: str = sys.version

    def Dump (self) -> None:
        """ Dump the user environment on standard output(terminal).

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        print (f'---------- Dump the user system environment Start ----------')
        print (f'User input arguments: {self.__Argv}')
        print (f'OS Name             : {self.__OSSystem} {self.__OSRelease}')
        print (f'OS Version          : {self.__OSVersion}')
        print (f'OS Platform         : {self.__OSPlatform}')
        print (f'Python Path         : {sys.executable}')
        print (f'Python Version      : {self.__PythonVersion}')
        print (f'----------- Dump the user system environment End -----------')
