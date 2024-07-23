## @file
#  Library provided basic function for parsing the user input arguments
#  from command line/terminal.
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
from typing import Any, Dict, List, Tuple

from CapsuleCommon.Base.DebugLib import *

from CapsuleCommon.Argument.ArgumentBaseLib import *

class ArgumentParser (object):
    def __init__ (self) -> None:
        """ Basic class object to parse the user input from terminal.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Argv: List[str] = sys.argv[1:]

        self.__PositionArgv: List[str]      = list () # Like XXX
        self.__KeywordArgv : Dict[str, Any] = dict () # Like -XXX YYY
        self.__FlagArgv    : Dict[str, Any] = dict () # Like --XXX

        self.__Decode ()
        self.__Print ()

    @property
    def PositionArgv (self) -> List[str]:
        """ Get the list of user input position argument with lowercase.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                User input position argument with lowercase.
        """
        return self.__PositionArgv

    @property
    def KeywordArgv (self) -> Dict[str, Any]:
        """ Get the list of user input keyword argument with lowercase.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, Any]:
                str:
                    User input keyword argument with lowercase.
                    Command name part.
                Any:
                    User input value.
                    If user call multiple times, would become list of
                    str.
        """
        return self.__KeywordArgv

    @property
    def FlagArgv (self) -> Dict[str, bool]:
        """ Get the list of user input flag argument with lowercase.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, bool]:
                str:
                    User input flag argument with lowercase.
                    Command name part.
                bool:
                    This would be True if user input.
        """
        return self.__FlagArgv

    def Get (self) -> Tuple[list, dict, dict]:
        """ Return the decode result.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[list, dict, dict]:
                PositionArgv (List[str]):
                    Record the position argument.
                    All are the string.
                KeywordArgv (Dict[str, Any]):
                    Record the keyword argument with the following
                    parameter/secondary parameter. (Key-Value pair)
                    If this keyword exist, convert it into
                    key-list[value, value, ...].
                Flag (Dict[str, bool]):
                    Record the flag argument.
                    If use set up this variable would be set to True.
        """
        return self.__PositionArgv, self.__KeywordArgv, self.__FlagArgv

    def __Print (self) -> None:
        """ To output the all information to terminal.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DEBUG (DEBUG_TRACE, f'User input args  : {sys.argv}')
        DEBUG (DEBUG_TRACE, f'Stored args      : {self.__Argv}')
        DEBUG (DEBUG_TRACE, f'Position argument: {self.__PositionArgv}')
        DEBUG (DEBUG_TRACE, f'Keyword argument : {self.__KeywordArgv}')
        DEBUG (DEBUG_TRACE, f'Flag argument    : {self.__FlagArgv}')

    def __Decode (self) -> None:
        """ To decode user input parameters on terminal.

        (1) Flag    : Set up this variable would be set to True.
        (2) Keyword : With secondary parameter. (Key-Value pair)
            - If keyword not exist, create a key-value pair.
            - If keyword exist, convert it to key-list[val, val, ...]
        (3) Position: Without any secondary parameter.

        Note: This parsing library is case-insensitive.
              All position args and would be converted into lowercase.

        Args:
            None.

        Raises:
            ValueError:
                User input the KEYWORD_ARGUMENT_VARIABLE but without
                along with secondary input.

        Returns:
            None.
        """
        Argc    : int  = len (self.__Argv)
        CurrIdx : int  = 0
        CurrArgv: str  = str ()
        Data    : dict = dict ()
        ArgType : int  = -1

        while (CurrIdx < Argc):
            #
            # Convert it to lower case first.
            #
            CurrArgv = self.__Argv[CurrIdx].lower ()
            ArgType  = GetArgsType (CurrArgv)

            if ArgType == FLAG_ARGUMENT_VARIABLE:
                Data     = {CurrArgv: True}
                self.__FlagArgv.update (Data)
                CurrIdx += 1
            elif ArgType == KEYWORD_ARGUMENT_VARIABLE:
                if (CurrIdx + 1) >= Argc:
                    raise ValueError (
                            f'Expect the secondary input along with '
                            f'keyword argument.'
                            )

                if CurrArgv in self.__KeywordArgv:
                    if not isinstance (self.__KeywordArgv[CurrArgv], list):
                        self.__KeywordArgv[CurrArgv] = \
                            [self.__KeywordArgv[CurrArgv]]

                    self.__KeywordArgv[CurrArgv].append (
                                                   self.__Argv[CurrIdx+1],
                                                   )
                else:
                    Data = {CurrArgv: self.__Argv[CurrIdx + 1]}
                    self.__KeywordArgv.update (Data)

                CurrIdx += 2
            elif ArgType == POSITION_ARGUMENT_VARIABLE:
                self.__PositionArgv.append (CurrArgv)
                CurrIdx += 1
            else:
                raise ValueError (f'Unsupported Command type: {CurrArgv}')
