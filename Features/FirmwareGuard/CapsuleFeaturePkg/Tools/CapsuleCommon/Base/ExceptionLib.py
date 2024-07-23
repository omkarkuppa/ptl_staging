## @file
#  Library supported to raise the customize exception.
#
#  Note:
#    (1) This library work with DebugLib/UserEnvLib.
#    (2) Any error exception should be used outside of the package.
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
from typing import NoReturn, Union

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.UserEnvLib import *

class _Exception (Exception):
    def __init__ (
        self,
        *Message         : object,
        DebugLevel       : str              = DEBUG_FATAL,
        Separator        : str              = ' ',
        End              : Union[None, str] = '\n',
        IsNativeException: bool             = True,
        IsDumpUserEnv    : bool             = True,
        ) -> NoReturn:
        """ Class raise the exception force to terminate the execution.

        Args:
            *Message (object):
                The message to be output.
                Same usage as print function.
            DebugLevel (str, optional):
                The current message debug level.
                Caller should use the define, rather uses the string
                manually.
                (Case-sensitive)
                Defaults to DEBUG_FATAL.
            Separator (str, optional):
                String used to insert between two strings.
                Defaults to ' '.
            End (Union[None, str], optional):
                String used to append into last value.
                Defaults to '\n'.
            IsNativeException (bool, optional):
                If need to raise the Python native exception.
                True  - Raise the Python native exception.
                False - Not raise the Python native exception.
                Defaults to True.
            IsDumpUserEnv (bool, optional):
                True  - Dump the current user environment.
                False - Not dump the current user environment.

        Raises:
            None.

        Returns:
            NoReturn.
        """
        self.__DebugLevel       : str              = DebugLevel
        self.__Message          : object           = Message
        self.__Separator        : str              = Separator
        self.__End              : Union[None, str] = End
        self.__IsNativeException: bool             = IsNativeException
        self.__IsDumpUserEnv    : bool             = IsDumpUserEnv

        self.__PreCheck ()

        DEBUG (
          self.__DebugLevel,
          *self.__Message,
          Separator = self.__Separator,
          End       = self.__End,
          )

        if self.__IsDumpUserEnv:
            UserEnvironment ().Dump ()

        if self.__IsNativeException:
            raise Exception (*self.__Message)
        else:
            sys.exit (1)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) IsNativeException is not bool type.
                (2) IsDumpUserEnv is not bool type.

        Returns:
            None.
        """
        if not isinstance (self.__IsNativeException, bool):
            raise TypeError (f'IsNativeException should be bool type.')
        elif not isinstance (self.__IsDumpUserEnv, bool):
            raise TypeError (f'IsDumpUserEnv should be bool type.')

class ErrorException (_Exception):
    def __init__ (
        self,
        *Message         : object,
        Separator        : str              = ' ',
        End              : Union[None, str] = '\n',
        IsNativeException: bool             = True,
        ) -> None:
        """ Class to raise the customize error exception.

        Note:
            Force the debug level into DEBUG_ERROR.

        Args:
            *Message (object):
                The message to be output.
                Same usage as print function.
            Separator (str, optional):
                String used to insert between two strings.
                Defaults to ' '.
            End (Union[None, str], optional):
                String used to append into last value.
                Defaults to '\n'.
            IsNativeException (bool, optional):
                If need to raise the Python native exception.
                True  - Raise the Python native exception.
                False - Not raise the Python native exception.
                Defaults to True.

        Raises:
            None.

        Returns:
            None.
        """
        self.__DebugLevel: str              = DEBUG_ERROR
        self.__Message   : object           = Message
        self.__Separator : str              = Separator
        self.__End       : Union[None, str] = End

        super ().__init__ (
                   *self.__Message,
                   DebugLevel        = self.__DebugLevel,
                   Separator         = self.__Separator,
                   End               = self.__End,
                   IsNativeException = IsNativeException,
                   )
