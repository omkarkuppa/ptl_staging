## @file
#  Library to generate the argument helper on terminal based on
#  assigned operation(OP) code and argument rules.
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
from typing import List

from CapsuleCommon.Base.PaddingLib import *
from CapsuleCommon.Wrapper.FileWrapperLib import *

from CapsuleCommon.Argument.ArgumentBaseLib import *
from CapsuleCommon.Argument.ArgumentLib import *

#
# Definition of length to display.
#
DEFAULT_ONE_LINE_MAX_LENGTH: int = 80
DEFAULT_CMD_MAX_LENGTH     : int = 15
DEFAULT_TITLE_SYMBOL       : str = '*'

#
# Type of argument helper.
#
DEFAULT_HELPER_TYPE : int       = 0x0000_0000
NEW_LINE_HELPER_TYPE: int       = 0x0000_0001
HELPER_TYPE_LIST    : List[int] = [
    DEFAULT_HELPER_TYPE,
    NEW_LINE_HELPER_TYPE
    ]

#
# Version control of Print layout.
#
PRINT_LAYOUT_VER1: int = 0x0000_0001

class ArgumentHelper (object):
    def __init__ (
        self,
        OpCode        : str,
        ArgumentList  : List[Argument],
        Title         : str = '',
        Description   : str = '',
        SubDescription: str = '',
        Copyright     : str = '',
        Version       : str = '',
        Style         : str = DEFAULT_HELPER_TYPE,
        ) -> None:
        """ Class use to generate the argument helper interface.

        Args:
            OpCode (str):
                The operation code of this argument helper.
            ArgumentList (List[Argument]):
                The list of Argument object for all of this OpCode.
            Title (str, optional):
                The title of this argument helper.
                Defaults to ''.
            Description (str, optional):
                The description of this OpCode.
                Defaults to ''.
            SubDescription (str, optional):
                The sub-description of this OpCode.
                Defaults to ''.
            Copyright (str, optional):
                The copyright statement of this program.
                Defaults to ''.
            Version (str, optional):
                The version string of this program.
                Defaults to ''.
            Style (str, optional):
                The type of helper.
                Defaults to DEFAULT_HELPER_TYPE.

        Raises:
            None.

        Returns:
            None.
        """
        self.__LayoutVersion : int            = PRINT_LAYOUT_VER1
        self.__OpCode        : str            = OpCode
        self.__ArgumentList  : List[Argument] = ArgumentList
        self.__Title         : str            = Title
        self.__Description   : str            = Description
        self.__SubDescription: str            = SubDescription
        self.__Copyright     : str            = Copyright
        self.__Version       : str            = Version
        self.__Style         : str            = Style

        self.__PreCheck ()

        self.__MaxCmdLen  : int = self.__GetMaxCmdLen ()
        self.__UsageSyntax: str = self.__GetUsageSegment ()
        self.__ArgsSegment: str = self.__GetArgumentSegment ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) OpCode is not str type.
                (2) ArgumentList is not list type.
                (3) Member of ArgumentList is not Argument type.
                (4) Title is not str type.
                (5) Description is not str type.
                (6) SubDescription is not str type.
                (7) Copyright is not str type.
                (8) Style is not str type.
            ValueError:
                (1) OpCode should not have space.
                (2) Style should one of HELPER_TYPE_LIST.

        Returns:
            None.
        """
        if not isinstance (self.__OpCode, str):
            raise TypeError (f'OpCode should be str type.')
        elif ' ' in self.__OpCode:
            raise ValueError (f'OpCode should not contain the space.')

        if not isinstance (self.__ArgumentList, list):
            raise TypeError (f'ArgumentList should be list type.')
        for Args in self.__ArgumentList:
            if not isinstance (Args, Argument):
                raise TypeError (
                        f'Element in ArgumentList should be Argument type.',
                        )

        if not isinstance (self.__Title, str):
            raise TypeError (f'Title type should be str type.')

        if not isinstance (self.__Description, str):
            raise TypeError (f'Description type should be str type.')

        if not isinstance (self.__Description, str):
            raise TypeError (f'SubDescription type should be str type.')

        if not isinstance (self.__Copyright, str):
            raise TypeError (f'Copyright type should be str type.')

        if not isinstance (self.__Version, str):
            raise TypeError (f'Version type should be str type.')

        if self.__Style not in HELPER_TYPE_LIST:
            raise ValueError (f'Style should be on of {HELPER_TYPE_LIST}.')

    def __GetMaxCmdLen (self) -> int:
        """ Get the maximum size of command.

        Note:
            If size is larger than DEFAULT_CMD_MAX_LENGTH,
            size should be use its value.
            Else use the DEFAULT_CMD_MAX_LENGTH.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The maximum length of command.
        """
        MaxCmdLen : int       = int ()
        CmdLenList: List[int] = [
            len (Args.Command) for Args in self.__ArgumentList
            ]

        MaxCmdLen = max (CmdLenList)

        if MaxCmdLen < DEFAULT_CMD_MAX_LENGTH:
            MaxCmdLen = DEFAULT_CMD_MAX_LENGTH

        return MaxCmdLen

    def __GetUsageSegment (self) -> str:
        """ Get the usage syntax of this OpCode.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Command syntax of this OpCode
        """
        Prefix  : str            = 'Usage: ' + self.__OpCode
        StrList : List[str]      = [Prefix]
        ArgsList: List[Argument] = copy.deepcopy (self.__ArgumentList)
        Cmd     : str            = str ()
        Syntax  : str            = str ()
        ArgsData: str            = str ()
        Length  : int            = int ()

        if self.__Style == NEW_LINE_HELPER_TYPE:
            StrList.append ('\n')
            Length = 0
        else:
            Length = len (Prefix)

        for Args in ArgsList:
            Cmd    = Args.Command
            Syntax = Args.Syntax if Args.Syntax else 'Input'

            #
            # Add the syntax
            #
            if Args.Type == KEYWORD_ARGUMENT_VARIABLE:
                ArgsData = f'[{Cmd} <{Syntax}>]'
            else:
                ArgsData = f'[{Cmd}]'

            #
            # Change to newline if length out of the maximum value.
            #
            if (Length + len (ArgsData)) > DEFAULT_ONE_LINE_MAX_LENGTH:
                StrList.append ('\n')
                if self.__Style == NEW_LINE_HELPER_TYPE:
                    pass
                else:
                    StrList.append (' ' * (len (Prefix) - 1))
                Length = 0

            StrList.append (ArgsData)
            Length += len (ArgsData)

        return ' '.join (StrList)

    def __GetArgsInfoByType (self, IsCompulsory: bool) -> List[str]:
        """ Get the information of argument by type.

        Note:
            This function is used to iteration all the ArgsList.

        Args:
            IsCompulsory (bool):
                True  - Get the compulsory argument information.
                False - Get the optional argument information.

        Raises:
            None.

        Returns:
            List[str]:
                The list of string for argument information by order.
        """
        Description: List[str] = list ()

        CmdPart       : str       = str ()
        DescList      : List[str] = list ()
        DescPart      : str       = str ()
        DefaultValPart: str       = str ()

        for Args in self.__ArgumentList:
            if Args.Compulsory != IsCompulsory:
                continue

            #
            # Command part
            #
            CmdPart = Padding (
                        Args.Command,
                        self.__MaxCmdLen,
                        IsPaddingRight= True
                        )

            #
            # Description of command part
            #
            DescList = Args.Description.split (' ')
            DescPart = GetStrWithFixedWidth (
                         DescList,
                         MaxWidth   = DEFAULT_ONE_LINE_MAX_LENGTH,
                         PaddingPos = (len (CmdPart) + 1),
                         Sep        = '\n'
                         )

            #
            # Default value part
            #
            if IsCompulsory:
                pass
            elif Args.Default:
                if Args.Type == KEYWORD_ARGUMENT_VARIABLE:
                    DefaultValPart = f'<Default: {Args.Default}>'
                else:
                    DefaultValPart = f'<Default: {bool (Args.Default)}>'
            else:
                if Args.Type == KEYWORD_ARGUMENT_VARIABLE:
                    DefaultValPart = f'<Default: {None}>'
                else:
                    DefaultValPart = f'<Default: {bool (Args.Default)}>'

            Description.append (f'{CmdPart}: {DescPart}\n')
            Description.append (' ' * (self.__MaxCmdLen + 2))
            Description.append (f'{DefaultValPart}\n')

        return ''.join (Description)

    def __GetArgumentSegment (self) -> str:
        """ Get the argument description/default value by category.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The description of all arguments within the OpCode.
        """
        CompulsoryArgsList: List[str] = self.__GetArgsInfoByType (True)
        OptionalArgsList  : List[str] = self.__GetArgsInfoByType (False)
        ResultStrList     : List[str] = list ()

        ResultStrList.append (f'[Compulsory Argument]\n')
        ResultStrList += CompulsoryArgsList

        ResultStrList.append (f'\n[Optional Argument]\n')
        ResultStrList += OptionalArgsList

        return ''.join (ResultStrList)

    def __PrintLayoutVer1 (self):
        """ The layout of output the helper information. (Version 1)

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Title
        #
        Title: str = CentralStrWithFixedWidth (
                       self.__Title,
                       DEFAULT_ONE_LINE_MAX_LENGTH,
                       Prefix  = DEFAULT_TITLE_SYMBOL,
                       Postfix = DEFAULT_TITLE_SYMBOL
                       )
        print (DEFAULT_TITLE_SYMBOL * DEFAULT_ONE_LINE_MAX_LENGTH)
        print (Title)
        print (DEFAULT_TITLE_SYMBOL * DEFAULT_ONE_LINE_MAX_LENGTH)

        #
        # Copyright
        #
        if self.__Copyright:
            print (f'{self.__Copyright}\n')

        #
        # Version
        #
        if self.__Version:
            print (f'Version: {self.__Version}\n')

        #
        # Description
        #
        print (f'{self.__Description}\n')

        #
        # Usage Syntax
        #
        print (f'{self.__UsageSyntax}\n')

        #
        # Argument Segment
        #
        print (self.__ArgsSegment)

        #
        # Sub-Description
        #
        print (self.__SubDescription)

    def Print (self) -> None:
        """ Function to print the helper based on layout version.

        Note:


        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        if self.__LayoutVersion == PRINT_LAYOUT_VER1:
            self.__PrintLayoutVer1 ()
        else:
            DEBUG (
              DEBUG_ERROR,
              f'Not supported layout version. '
              f'Would not print out anything.'
              )
