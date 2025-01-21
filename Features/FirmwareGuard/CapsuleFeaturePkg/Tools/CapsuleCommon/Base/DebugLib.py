## @file
#  Library to print the message to console with ANSI escape code.
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
import sys
from typing import List, Dict, Union
from types import FunctionType

from CapsuleCommon.Base.SearchLib import SearchValueInList

#
# Debug level mapping to string
#
DEBUG_FATAL: str = 'FATAL'
DEBUG_ERROR: str = 'ERROR'
DEBUG_WARN : str = 'WARN'
DEBUG_INFO : str = 'INFO'
DEBUG_DEBUG: str = 'DEBUG'
DEBUG_TRACE: str = 'TRACE'

#
# List of all debug level
#
DEBUG_LEVEL_LIST: List[str] = [
    DEBUG_FATAL,
    DEBUG_ERROR,
    DEBUG_WARN ,
    DEBUG_INFO ,
    DEBUG_DEBUG,
    DEBUG_TRACE,
    ]

#
# ANSI escape word
#
COLOR_DEBUG_ESCAPE_WORD: str = '\033[{}m'

#
# ANSI debug level color
#
DEBUG_LEVEL_ANSI_COLOR_DICT: Dict[str, str] = {
    DEBUG_FATAL: COLOR_DEBUG_ESCAPE_WORD.format ('31'),
    DEBUG_ERROR: COLOR_DEBUG_ESCAPE_WORD.format ('91'),
    DEBUG_WARN : COLOR_DEBUG_ESCAPE_WORD.format ('93'),
    DEBUG_INFO : COLOR_DEBUG_ESCAPE_WORD.format ('96'),
    DEBUG_DEBUG: COLOR_DEBUG_ESCAPE_WORD.format ('90'),
    DEBUG_TRACE: COLOR_DEBUG_ESCAPE_WORD.format ('92'),
    }

#
# ANSI message font type
#
DEBUG_FONT_TYPE_BOLD: str = 'BOLD'
DEBUG_FONT_TYPE_ENDL: str = 'ENDL'

DEBUG_MESSAGE_FONT_TYPE: Dict[str, str] = {
    DEBUG_FONT_TYPE_BOLD: COLOR_DEBUG_ESCAPE_WORD.format ('1'),
    DEBUG_FONT_TYPE_ENDL: COLOR_DEBUG_ESCAPE_WORD.format ('0'),
    }

class _Debug (object):
    #
    # Class variable to share within different instance
    #
    DebugLevel: str = DEBUG_WARN

    def __init__ (self) -> None:
        pass

    def SetDebugLevel (self, Level: str) -> None:
        """ Set the global debug level.

        Args:
            Level (str):
                To set the debug level to all instance.
                It should be one of the list of DEBUG_LEVEL_LIST.
                (Case-insensitive)

        Raises:
            TypeError:
                Input debug level type should str type.

        Returns:
            None.
        """
        IsFound   : bool = False
        DebugLevel: str  = str ()

        if not isinstance (Level, str):
            raise TypeError (f'Please input the string.')

        IsFound, DebugLevel = SearchValueInList (
                                DEBUG_LEVEL_LIST,
                                Level,
                                IsIgnoreCase=True
                                )

        if not IsFound:
            print (
              f'Failed to set the global debug level to {Level}.'
              f' Supported list is {DEBUG_LEVEL_LIST}'
              )
        else:
            print (f'Set the debug level to {DebugLevel}')
            _Debug.DebugLevel = DebugLevel

    def Debug (
        self,
        DebugLevel: str,
        *Message  : object,
        Separator : str              = ' ',
        End       : Union[None, str] = '\n',
        ) -> None:
        """ To print the debug message into output.

        Note:
            (1) If debug level is higher or equal global debug level.
                Message would be printed into output.
            (2) If the debug level not in DEBUG_LEVEL_LIST,
                it would not do any action.

        Args:
            DebugLevel (str):
                The current message debug level.
                Caller should use the define, rather uses the string
                manually.
                (Case-sensitive)
            *Message (object):
                The message to be output.
                Same usage as print function.
            Separator (str, optional):
                String used to insert between two strings.
                Defaults to ' '.
            End (Union[None, str], optional):
                String used to append into last value.
                Defaults to '\n'.

        Raises:
            TypeError:
                (1) The input Separator is not str type.
                (2) The input End is not None or str type.

        Returns:
            None.
        """
        #
        # Work-around for Windows not fully support ANSI color.
        #
        os.system ('')

        GlobalDebugLevel: str  = _Debug.DebugLevel
        MessageList     : list = [str (Msg) for Msg in Message]

        if not isinstance (Separator, str):
            raise TypeError ('Type of separator should be str.')

        if not isinstance (End, str) and not None:
            raise TypeError ('Type of end should be None or str.')

        if (GlobalDebugLevel is None) or (not GlobalDebugLevel):
            return
        elif (GlobalDebugLevel not in DEBUG_LEVEL_LIST) or \
             (DebugLevel not in DEBUG_LEVEL_LIST):
            return
        else:
            if DEBUG_LEVEL_LIST.index (GlobalDebugLevel) < \
                DEBUG_LEVEL_LIST.index (DebugLevel):
                return

        if DebugLevel in DEBUG_LEVEL_LIST and sys.stdout.isatty ():
            print (
              ''.join ([
                   DEBUG_LEVEL_ANSI_COLOR_DICT[DebugLevel],
                   DEBUG_MESSAGE_FONT_TYPE[DEBUG_FONT_TYPE_BOLD],
                   '[',
                   DebugLevel,
                   '] ',
                   DEBUG_MESSAGE_FONT_TYPE[DEBUG_FONT_TYPE_ENDL],
                   DEBUG_LEVEL_ANSI_COLOR_DICT[DebugLevel],
                   Separator.join (MessageList),
                   DEBUG_MESSAGE_FONT_TYPE[DEBUG_FONT_TYPE_ENDL],
                   ]),
              end = End,
              )
        else:
            MessageList.insert (0, f'[{DebugLevel}]')
            print (*MessageList, sep = Separator, end = End)

#
# Exposed the class to this module (Singleton)
#
_DebugObj    : _Debug       = _Debug ()
DEBUG        : FunctionType = _DebugObj.Debug
SetDebugLevel: FunctionType = _DebugObj.SetDebugLevel
