## @file
#  Library to check if caller's input data meet the requirement.
#
#  Provide a service to deal with the optional parameters and toggle the
#  flag parameter.
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

import copy
from typing import Any, Dict, List, MutableMapping

from CapsuleCommon.Base.BaseLib import *
from CapsuleCommon.Base.ExceptionLib import *

from CapsuleCommon.Argument.ArgumentBaseLib import *
from CapsuleCommon.Argument.ArgumentLib import *
from CapsuleCommon.Argument.ArgumentParserLib import *

class ArgumentChecker (object):
    def __init__ (
        self,
        ArgsRules : List[Argument],
        ArgsParser: ArgumentParser,
        ) -> None:
        """ Class used to check the user input meet the requirement.

        All available arguments including the optional key-value pair
        should in it.

        Args:
            ArgsRules (List[Argument]):
                List of necessary argument rules used to check the user
                input meet the criteria.
            ArgsParser (ArgumentParser):
                The object to parse the user input arguments from
                command line/terminal.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Params: MutableMapping[str, Any] = dict ()

        self.__ArgsRules : List[Argument] = ArgsRules
        self.__ArgsParser: ArgumentParser = ArgsParser

        self.__PreCheck ()

        self.__FlagArgv    : List[str]       = self.__ArgsParser.FlagArgv
        self.__KeywordArgv : Dict[str, Any]  = self.__ArgsParser.KeywordArgv
        self.__PositionArgv: Dict[str, bool] = self.__ArgsParser.PositionArgv

        self.__Params = self.__Check ()

    @property
    def Params (self) -> MutableMapping[str, Any]:
        """ Get the all required parameters after do the argument check.

        Args:
            None.

        Raises:
            None.

        Returns:
            MutableMapping[str, Any]:
                The key-value pair result of the require parameters.
                str:
                    The key of this parameter name. (Force lowercase)
                Any:
                    The value of this parameter should be list of str,
                    str, or bool etc.
        """
        return self.__Params

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) ArgsRule not the Argument object.
                (2) ArgsParser not the ArgumentParser object.

        Returns:
            None.
        """
        for ArgsRule in self.__ArgsRules:
            if not isinstance (ArgsRule, Argument):
                raise TypeError (f'ArgsRule should be Argument object.')

        if not isinstance (self.__ArgsParser, ArgumentParser):
            raise TypeError (f'ArgsParser should be ArgumentParser object.')

    def __Check (self) -> MutableMapping[str, Any]:
        """ Check user input argument meet the list of argument rules.

        If user not input and not mandatory argument, would use the
        default value.
        If user not input but mandatory argument, would raise the
        exception.

        All of them are lowercase and would be convert into
        MutableMapping with key-value.
        A. Flag    : If exist would invert the boolean value based on
                     the default value.
                     If not exist would be used the default value.
        B. Keyword : With secondary parameter. (Key-Value pair)
        C. Position: If exist would invert the boolean value based on
                     the default value.

        Args:
            None.

        Raises:
            ValueError:
                Mandatory attribute in Argument object should be bool
                type.
            TypeError:
                Type attribute in Argument object not matched any
                supported type.
            Exception:
                The mandatory command not assigned by user input.

        Returns:
            MutableMapping[str, Any]:
                str:
                    User input argument/command. (Lowercase)
                Any:
                    Might be the list of str, bool, str.
                    Based on the user input and argument type.
        """
        Result: MutableMapping[str, Any] = dict ()

        ArgsType  : int  = int ()
        Command   : str  = str ()
        Mandatory : bool = bool ()
        DefaultVal: Any  = None

        for Rule in self.__ArgsRules:
            ArgsType   = Rule.Type
            Command    = Rule.Command
            Mandatory  = Rule.Compulsory
            DefaultVal = Rule.Default

            #
            # Position argument case.
            #
            if ArgsType == POSITION_ARGUMENT_VARIABLE:
                if Command in self.__PositionArgv:
                    Result.update ({Command: not bool (DefaultVal)})
                else:
                    Result.update ({Command: bool (DefaultVal)})
            #
            # Keyword argument case.
            #
            elif ArgsType == KEYWORD_ARGUMENT_VARIABLE:
                if Command in self.__KeywordArgv:
                    Result.update ({Command: self.__KeywordArgv[Command]})
                else:
                    if Mandatory == COMPULSORY:
                        raise Exception (
                                f'The argument \"{Command}\" is '
                                f'required please assign it.'
                                )
                    elif Mandatory == OPTIONAL:
                        Result.update ({Command: DefaultVal})
                        DEBUG (
                          DEBUG_INFO,
                          f'Not assign the "{Command}" would '
                          f'use default value "{DefaultVal}".'
                          )
                    else:
                        raise ValueError (f'Unsupported value: {Mandatory}')
            #
            # Flag argument case.
            #
            elif ArgsType == FLAG_ARGUMENT_VARIABLE:
                if Command in self.__FlagArgv:
                    Result.update ({Command: not bool (DefaultVal)})
                else:
                    Result.update ({Command: bool (DefaultVal)})
            #
            # Unsupported case.
            #
            else:
                DEBUG (DEBUG_ERROR, f'{Rule}')
                raise TypeError (f'Unsupported argument type.')

        return copy.deepcopy (Result)
