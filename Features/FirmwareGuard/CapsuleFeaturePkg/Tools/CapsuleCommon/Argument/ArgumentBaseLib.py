## @file
#  Library provided base define/function for argument package.
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

POSITION_ARGUMENT_VARIABLE: int = 0x0000_0000 # XXX
KEYWORD_ARGUMENT_VARIABLE : int = 0x0000_0001 # -XXX YYY
FLAG_ARGUMENT_VARIABLE    : int = 0x0000_0002 # --XXX

ARGUMENT_VARIABLE_LIST: list = [
    POSITION_ARGUMENT_VARIABLE,
    KEYWORD_ARGUMENT_VARIABLE,
    FLAG_ARGUMENT_VARIABLE,
    ]

def IsFlagArgs (Command: str) -> bool:
    """ Check if user input command is flag argument. (--XXX)

    Args:
        Command (str):
            User input command.

    Raises:
        None.

    Returns:
        bool:
            True  - Command is flag argument.
            False - Command is not flag argument.
    """
    if Command.startswith ('--'):
        return True
    else:
        return False

def IsPositionArgs (Command: str) -> bool:
    """ Check if user input command is position argument. (XXX)

    Args:
        Command (str):
            User input command.

    Raises:
        None.

    Returns:
        bool:
            True  - Command is position argument.
            False - Command is not position argument.
    """
    if Command.startswith ('--'):
        return False
    elif Command.startswith ('-'):
        return False
    else:
        return True

def IsKeywordArgs (Command: str) -> bool:
    """ Check if user input command is keyword argument. (-XXX YYY)

    Args:
        Command (str):
            User input command.

    Raises:
        None.

    Returns:
        bool:
            True  - Command is keyword argument.
            False - Command is not keyword argument.
    """
    if Command.startswith ('--'):
        return False
    elif Command.startswith ('-'):
        return True
    else:
        return False

def GetArgsType (Command: str) -> int:
    """ Get the user input argument type.

    Args:
        Command (str):
            User input command.

    Raises:
        None.

    Returns:
        int:
            Type of user input argument/command.
            Should be one of ARGUMENT_VARIABLE_LIST.
    """
    if Command.startswith ('--'):
        return FLAG_ARGUMENT_VARIABLE
    elif Command.startswith ('-'):
        return KEYWORD_ARGUMENT_VARIABLE
    else:
        return POSITION_ARGUMENT_VARIABLE
