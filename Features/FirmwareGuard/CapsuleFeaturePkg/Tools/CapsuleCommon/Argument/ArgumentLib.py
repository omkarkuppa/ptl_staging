## @file
#  Library provided basic class for each Argument object.
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

from typing import Any

from CapsuleCommon.Argument.ArgumentBaseLib import *

ARGS_COMMAND_KEY    : str = 'Command'
ARGS_COMPULSORY_KEY : str = 'Compulsory'
ARGS_DESCRIPTION_KEY: str = 'Description'
ARGS_SYNTAX_KEY     : str = 'Syntax'
ARGS_DEFAULT_KEY    : str = 'Default'

class Argument (object):
    def __init__ (
        self,
        Command    : str,
        Compulsory : bool = False,
        Description: str  = 'Description',
        Syntax     : str  = 'SYNTAX',
        Default    : Any  = None,
        ) -> None:
        """ Class used to represent the each argument.

        Args:
            Command (str):
                Command of this argument.
            Compulsory (bool, optional):
                True  - This argument is mandatory for user to input.
                False - The argument is not mandatory for user to input.
                Defaults to False.
            Description (str, optional):
                Description of this argument for usage/purpose etc.
                Defaults to 'Description'.
            Syntax (str, optional):
                Part of description to show on the argument helper.
                Defaults to 'SYNTAX'.
            Default (Any, optional):
                The default value when user not input.
                Note: This only take effect when Compulsory is False.
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Command    : str  = Command.lower () # Force lowercase.
        self.__Compulsory : bool = Compulsory
        self.__Description: str  = Description
        self.__Syntax     : str  = Syntax
        self.__Default    : Any  = Default

        self.__PreCheck ()

    @property
    def Command (self) -> str:
        """ Command of this argument.

        Note: (1) --XXX: Flag args     (True/False)
              (2) -XXX : Keyword args  (Key-Value)
              (3) XXX  : Position args (True/False)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The command of this argument.
        """
        return self.__Command

    @property
    def Compulsory (self) -> bool:
        """ Identify if this argument is required/mandatory.

        Note: If this argument is mandatory, default value would not
              take any effect.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - This argument is mandatory for user to input.
                False - The argument is not mandatory for user to input.
        """
        return self.__Compulsory

    @property
    def Description (self) -> str:
        """ Description of this argument for usage/purpose etc.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The description of this argument.
        """
        return self.__Description

    @property
    def Syntax (self) -> str:
        """ Part of description to show on the argument helper.

        e.g. PATH, <Option A|Option B|Option C>, ...

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                To return syntax string. (Allowed input string)
        """
        return self.__Syntax

    @property
    def Default (self) -> Any:
        """ The default value when user not input.

        Note: This only take effect when Compulsory is False.

        Args:
            None.

        Raises:
            None.

        Returns:
            Any:
                The default value of this argument.
        """
        return self.__Default

    @property
    def Type (self) -> int:
        """ Type of the argument.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The type of this argument.
                It should be one of ARGUMENT_VARIABLE_LIST
        """
        return GetArgsType (self.Command)

    def __repr__ (self) -> str:
        """ To represent the object in str type.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The representation of this object.
        """
        Result: dict = {
            str (self.Command): {
                ARGS_COMMAND_KEY    : str (self.Command),
                ARGS_COMPULSORY_KEY : str (self.Compulsory),
                ARGS_DESCRIPTION_KEY: str (self.Description),
                ARGS_SYNTAX_KEY     : str (self.Syntax),
                ARGS_DEFAULT_KEY    : str (self.Default),
                }
            }
        return str (Result)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Arg - Command is not str type.
                (2) Arg - Compulsory is not bool type.
                (3) Arg - Description is not str type.
                (4) Arg - Syntax is not str type.

        Returns:
            None.
        """
        if not isinstance (self.__Command, str):
            raise TypeError (f'Command should be str type.')

        if not isinstance (self.__Compulsory, bool):
            raise TypeError (f'Compulsory should be bool type.')

        if not isinstance (self.__Description, str):
            raise TypeError (f'Description should be str type.')

        if not isinstance (self.__Syntax, str):
            raise TypeError (f'Syntax should be str type.')
