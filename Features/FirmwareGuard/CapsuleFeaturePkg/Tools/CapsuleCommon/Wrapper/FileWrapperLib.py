## @file
#  Wrapper to manipulate the line-level File operation.
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
from io import TextIOWrapper
from typing import Any, Dict, List, Tuple, Union

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Base.FileLib import *
from CapsuleCommon.Base.PathLib import AbsPath

FILE_UNSUPPORTED_TYPE: int = 0x0000_0000
FILE_BUFFER_TYPE     : int = 0x0000_0001
FILE_CONTENT_TYPE    : int = 0x0000_0002
FILE_PATH_TYPE       : int = 0x0000_0003

def GetStrWithFixedWidth (
    Input     : List[str],
    MaxWidth  : int,
    PaddingPos: int = 0,
    Sep       : str = '\n',
    ) -> str:
    """ Get the string with fixed width.

    Args:
        Input (List[str]):
            Input string of list to be output as fixed width.
        MaxWidth (int):
            The maximum width value.
        PaddingPos (int, optional):
            The padding offset to make each line with same
            size of padding in the beginning.
            Defaults to 0.
        Sep (str, optional):
            The separator symbol for the newline.
            Defaults to '\n'.

    Raises:
        TypeError:
            (1) Input is not list type.
            (2) Maximum width is not int type.
            (3) PaddingPos is not int type.
            (4) Sep is not int type.
            (5) Element of list is not str type.
        ValueError:
            (1) Maximum width should be positive value.
            (2) Padding position should be positive value.

    Returns:
        str:
            The string with fixed width and separator.
    """
    Result : str = str ()
    CurrPos: int = PaddingPos
    Padding: str = ' ' * (PaddingPos)

    if not isinstance (Input, list):
        raise TypeError (f'The type should be list.')

    if not isinstance (MaxWidth, int):
        raise TypeError (f'Maximum width should be int.')
    elif MaxWidth <= 0:
        raise ValueError (f'Maximum width should be positive value.')

    if not isinstance (PaddingPos, int):
        raise TypeError (f'Padding position should be int.')
    elif MaxWidth <= 0:
        raise ValueError (f'Padding position should be positive value.')

    if not isinstance (Sep, str):
        raise TypeError (f'Separator should be string.')

    for String in Input:
        if not isinstance (String, str):
            raise TypeError (f'Element of list should be string.')

        if CurrPos + len (String) > MaxWidth:
            Result += f'{Sep}{Padding}'
            CurrPos = PaddingPos

        Result  += f' {String}'
        CurrPos += len (String)

    return Result

def CentralStrWithFixedWidth (
    Input  : str,
    MaxLen : int,
    Prefix : str = '',
    Postfix: str = '',
    ) -> str:
    """ Central the input string with fixed width and specific symbol.

    Note:
        The string is consist with Prefix-Space-Input-Space-Postfix.

    Args:
        Input (str):
            The input string to be processed.
        MaxLen (int):
            The maximum length.
        Prefix (str, optional):
            The prefix of this string.
            (Length of prefix would also use the MaxLLen space)
            Defaults to ''.
        Postfix (str, optional):
            The postfix of this string.
            (Length of postfix would also use the MaxLLen space)
            Defaults to ''.

    Raises:
        TypeError:
            (1) Input is not str type.
            (2) MaxLen is not int type.
            (3) Prefix is not str type.
            (4) Postfix is not str type.
        ValueError:
            Not enough space to place the prefix, input, postfix.

    Returns:
        str:
            The processed central string with fixed width.
    """
    RemainSpace: int = int ()

    if not isinstance (Input, str):
        raise TypeError (f'Input should be str type.')
    elif not isinstance (MaxLen, int):
        raise TypeError (f'MaxLen should be int type.')
    elif not isinstance (Prefix, str):
        raise TypeError (f'Prefix should be str type.')
    elif not isinstance (Postfix, str):
        raise TypeError (f'Postfix should be str type.')
    elif len (Input) > (MaxLen - len (Prefix) - len (Postfix)):
        raise ValueError (f'Unable to process since not enough space.')

    RemainSpace = MaxLen - len (Prefix) - len (Postfix) - len (Input)

    Left   = ' ' * int (RemainSpace / 2)
    Middle = Input

    if RemainSpace % 2 != 0:
        Right = ' ' * (int (RemainSpace / 2) + 1)
    else:
        Right = ' ' * int (RemainSpace / 2)

    return f'{Prefix}{Left}{Middle}{Right}{Postfix}'

class FileRangeInfo (object):
    def __init__ (self) -> None:
        """ Class to represent the range information.

        Note:
            Caller have responsibility to ensure the range value
            is valid inside the file content.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__RangeList: List[Tuple[int, int]] = list ()

    @property
    def Data (self) -> List[Tuple[int, int]]:
        """ Return the list of range information in tuple (0-based).

        Args:
            None.

        Raises:
            None.

        Returns:
            List[Tuple[int, int]]:
                The 0-based list of range tuple.
                Format should be [(Start, End), ...]
        """
        return self.__RangeList

    @property
    def Length (self) -> int:
        """ Return the length of the range information.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The length of the range information.
        """
        return len (self.__RangeList)

    def __IsValid (self, Value: int) -> bool:
        """ Check the input value is valid.

        Note:
            This function would check below items.
                (1) Is input value is int type.
                (2) Is input value is positive value.

        Args:
            Value (int):
                Input value to be trialed.

        Raises:
            None.

        Returns:
            bool:
                False - Input value is invalid.
                True  - Input value is valid.
        """
        if not isinstance (Value, int):
            return False

        if Value < 0:
            return False

        return True

    def Add (self, Start: int, End: int) -> None:
        """ Add the new range information.

        Args:
            Start (int):
                The start value of the range.
            End (int):
                The end value of the range.

        Raises:
            ValueError:
                (1) Start value is invalid.
                (2) End value is invalid.

        Returns:
            None.
        """
        StartLoc: int = None
        EndLoc  : int = None

        if not self.__IsValid (Start):
            raise ValueError (f'Start value is invalid.')
        elif not self.__IsValid (End):
            raise ValueError (f'End value is invalid.')

        if Start < End:
            StartLoc, EndLoc = Start, End
        else:
            StartLoc, EndLoc = End, Start

        self.__RangeList.append (tuple ([StartLoc, EndLoc]))

class File (object):
    def __init__ (
        self,
        FileBuffer : Union[None, TextIOWrapper]  = None,
        FileContent: Union[None, str, List[str]] = None,
        FilePath   : Union[str, os.PathLike]     = '',
        Encode     : str                         = 'utf-8',
        IsReadLines: bool                        = False,
        ) -> None:
        """ File class to support common manipulate on line level.

        Note:
            FileBuffer/FileContent/FilePath should choose one of them
            as a input. (Encode and IsReadLines only check and valid
            when use FilePath as input)

        Args:
            FileBuffer (Union[None, TextIOWrapper], optional):
                TextIOWrapper type buffer.
                Defaults to None.
            FileContent (Union[None, str, List[str]], optional):
                List of string lines or one-line string.
                Defaults to None.
            FilePath (Union[str, os.PathLike], optional):
                The path of text-based file to be read.
                Defaults to ''.
            Encode (str, optional):
                The encoding of this text-based file.
                Defaults to 'utf-8'.
            IsReadLines (bool, optional):
                True  - To read the file by one-by-one line in list.
                False - To read the file within one string.
                Defaults to False.
        """
        self.__Content   : List[str] = list ()
        self.__Type      : int       = FILE_UNSUPPORTED_TYPE

        self.__FileBuffer : Union[None, TextIOWrapper]  = FileBuffer
        self.__FileContent: Union[None, str, List[str]] = FileContent
        self.__FilePath   : str                         = FilePath
        self.__Encode     : str                         = Encode
        self.__IsReadLines: bool                        = IsReadLines

        self.__PreCheck ()

        if FileContent is not None:
            self.__Type    = FILE_CONTENT_TYPE
            self.__Content = FileContent
        elif FilePath:
            self.__Type     = FILE_PATH_TYPE
            self.__FilePath = AbsPath (FilePath)
            self.__Content  = ReadTextFile (
                self.__FilePath,
                self.__Encode,
                self.__IsReadLines
                )
        elif FileBuffer:
            self.__Type = FILE_BUFFER_TYPE
            if self.__IsReadLines:
                self.__Content = FileBuffer.readlines ()
            else:
                self.__Content = FileBuffer.read ()

        if isinstance (self.__Content, str):
            self.__Content = [self.__Content]

    @property
    def Path (self) -> Tuple[bool, str]:
        """ Path of input file.

        Note:
            (1) This property only valid when input type is
                FILE_PATH_TYPE.
            (2) Otherwise, the IsValid would return False.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[bool, str]:
                bool:
                    The return value is valid or not.
                    Only the FILE_PATH_TYPE is True.
                str:
                    The path of input file.
                    If the Valid is False, then the empty string would
                    be returned.
        """
        IsValid: bool = False

        if self.Type == FILE_PATH_TYPE:
            IsValid = True

        return IsValid, self.__FilePath

    @property
    def Type (self) -> int:
        """ Type of this file object in memory.

        Note:
            (1) Current Valid Type
                    - FILE_UNSUPPORTED_TYPE
                    - FILE_BUFFER_TYPE
                    - FILE_CONTENT_TYPE
                    - FILE_PATH_TYPE
            (2) All of types have been pre-defined as exact int value.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Type of this file object instance.
        """
        return self.__Type

    @property
    def Line (self) -> int:
        """ Line number of this file.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exact lines of this file.
        """
        return len (self.__Content)

    @property
    def Content (self) -> List[str]:
        """ Get the all list of line string within this file.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of string lines within this file.
        """
        return self.__Content

    @property
    def PlanText (self) -> str:
        """ Merge all the lines into plain string.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The string of all the lines within the file.
        """
        if isinstance (self.__Content, list):
            return ''.join (self.__Content)
        else:
            return self.__Content

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) Not choose one of FileBuffer/FileContent/FilePath
                    as input option. (None of them or more than one)
                (2) Path is not exist.
                (3) Path is not a file.
            TypeError:
                (1) FileBuffer not TextIOWrapper type.
                (2) FileContent not str/List[str] type.
                    Caller responsibility to provide list of string.
                (3) Encoded not str type.
                (4) IsReadLines not bool type.

        Returns:
            None.
        """
        Condition: List[int] = [
            int (not (self.__FileBuffer is None)),
            int (not (self.__FileContent is None)),
            int (not (self.__FilePath == '')),
            ]

        if (sum (Condition) == 0):
            raise ValueError (
                    f'Need choose one FileBuffer/FileContent/FilePath to input'
                    )

        if (self.__FileBuffer is not None) and \
           (not isinstance (self.__FileBuffer, TextIOWrapper)):
            raise TypeError (f'FileBuffer only allow TextIOWrapper')

        elif (self.__FileContent is not None) and \
             (not isinstance (self.__FileContent, (str, list))):
            raise TypeError (f'FileContent only allow str/List[str]')

        elif self.__FilePath:
            if not IsPathExist (self.__FilePath):
                raise ValueError (f'Path is not exist.')
            elif not IsFile (self.__FilePath):
                raise ValueError (f'Path is not a file.')

            if not isinstance (self.__Encode, str):
                raise TypeError (f'Encoded should be str type')
            elif not isinstance (self.__IsReadLines, bool):
                raise TypeError (f'IsReadLine should bool type.')

    def __IsLineInRange (self, Line: Union[int, List[int]]) -> bool:
        """ Check the caller line number is within the valid range.

        Args:
            Line (Union[int, List[int]]):
                The specific integer line number or list of integer
                numbers.

        Raises:
            ValueError:
                The input line is not within the valid range.

        Returns:
            bool:
                True  - It is located within the valid range.
                False - Would not return this value, but would raise the
                        ValueError exception since it is unrecoverable
                        error.
        """
        IdxList: list = list ()

        if isinstance (Line, int):
            IdxList = [Line]
        elif isinstance (Line, list):
            IdxList = Line

        for Idx in IdxList:
            if (Idx < 0) or (Idx >= self.Line):
                raise ValueError (
                        f'Line is not under valid range: {Idx} [{self.Line}]'
                        )

        return True

    def GetLineData (self, Line: int) -> str:
        """ Get specific line data with assigned 0-based line number.

        Args:
            Line (int):
                The specific integer line number to get.

        Raises:
            TypeError:
                Assigned line argument is not int type.
            ValueError:
                Assigned line argument is not located in valid range.

        Returns:
            str:
                The content within specific 0-based line number.
        """
        if not isinstance (Line, int):
            raise TypeError (f'Only support int to get line data.')
        elif not self.__IsLineInRange (Line):
            raise ValueError (f'Assigned line number is not valid')

        return self.__Content[Line]

    def Fill (self, Data: Dict[str, Any] = dict (), **Kwargs) -> None:
        """ Fill the key-value mapping into all the lines within file.

        Args:
            Data (Dict[str, Any], optional):
                The key-value mapping to replaced the pre-process string
                into correspond value.
                Defaults to dict().
            **Kwargs:
                Allow to input the key=value, directly into the call
                function.
                Would merge Data dict () and keyword argument into one
                mapping dict ()

        Raises:
            TypeError:
                Data argument should be dict type.

        Returns:
            None.
        """
        Mapping : Dict[str, Any] = dict ()

        if not isinstance (Data, dict):
            raise TypeError (f'Data argument should be dict type.')

        Mapping.update (Data)
        Mapping.update (Kwargs)
        DEBUG (DEBUG_TRACE, f'Mapping fill in string: {Mapping}')

        for Line in range (self.Line):
            self.__Content[Line] = (self.__Content[Line].format (**Mapping))

    def Replace (
        self,
        Revised: str,
        Source : Union[None, str]       = None,
        Line   : Union[None, int, list] = None,
        ) -> None:
        """ To replace the specific string from one to another one.

        Args:
            Revised (str):
                The revised string.
            Source (Union[None, str], optional):
                The specific the be replaced into revised string.
                If the source is None, all the assigned lines would be
                replaced into revised string directly.
                Defaults to None.
            Line (Union[None, int, list], optional):
                The specific integer line number or list of integer
                numbers.
                If keep this as None, all the lines within this file
                would be replaced.
                Defaults to None.

        Raises:
            TypeError:
                (1) Source argument not the str type.
                (2) Revised argument not the str type.
            ValueError:
                Line argument type is not integer or list of integer.

        Returns:
            None.
        """
        IdxList: List[int] = list ()

        if (Source is not None) and (not isinstance (Source, str)):
            raise TypeError (f'Source argument should be str type.')
        elif (Revised is not None) and (not isinstance (Revised, str)):
            raise TypeError (f'Revised argument should be str type.')

        if Line is not None:
            self.__IsLineInRange (Line)

        if Line is None:
            IdxList = range (self.Line)
        elif isinstance (Line, int):
            IdxList = [Line]
        elif isinstance (Line, list):
            IdxList = Line
        else:
            raise ValueError (f'Line info type is not supported.')

        for Idx in IdxList:
            if Source is None:
                self.__Content[Idx] = Revised
            else:
                self.__Content[Idx] = self.__Content[Idx].\
                    replace (
                      Source,
                      Revised,
                      )

    def Find (
        self,
        Keyword: Any,
        Start: Union[None, int] = None,
        End  : Union[None, int] = None,
        ) -> List[int]:
        """ Find the specific keyword in the specific range.

        Args:
            Keyword (Any):
                The keyword to be search in line.
            Start (Union[None, int], optional):
                The 0-based position start to search.
                Defaults to None. (Means 0)
            End (Union[None, int], optional):
                The 0-based position end to search.
                Defaults to None. (Means EOL)

        Raises:
            ValueError:
                (1) "Start" argument not integer type.
                (2) "End" argument not integer type.
                (3) "Start"/"End" offset should not negative value.
                (4) "Start"/"End" offset should not larger than EOL.

        Returns:
            List[int]:
                List of find the specific keyword in 0-based position.
        """
        ResultList: List[int] = list ()
        StartLoc  : int       = 0
        EndLoc    : int       = self.Line

        if Start is not None:
            if not isinstance (Start, int):
                raise ValueError (f'Start only support int.')
            elif Start < 0:
                raise ValueError (f'Start offset should >= 0.')
            elif Start > self.Line:
                raise ValueError (f'Start offset should < Max length.')
        else:
            Start = 0

        if End is not None:
            if not isinstance (End, int):
                raise ValueError (f'End only support int.')
            elif End < 0:
                raise ValueError (f'End offset should >= 0.')
            elif End > self.Line:
                raise ValueError (f'End offset should < Max length.')
        else:
            End = self.Line

        if (Start is not None) and (End is not None):
            if Start > End:
                Start, End = End, Start
            StartLoc = Start
            EndLoc   = End

        DEBUG (DEBUG_TRACE, f'Find the keyword from [{Start}] to [{End}]')

        for Idx in range (StartLoc, EndLoc):
            if str (Keyword) in self.__Content[Idx]:
                ResultList.append (Idx)

        return ResultList

    def FindInRange (
        self,
        Keyword  : Any,
        RangeInfo: FileRangeInfo,
        ) -> Tuple[List[int], FileRangeInfo]:
        """ Find the specific keyword in the specific range list.

        Args:
            Keyword (Any):
                The keyword to be search in line.
            RangeInfo (FileRangeInfo):
                The 0-based range information.
                (Shall be FileRangeInfo type.)

        Raises:
            None.

        Returns:
            Tuple[List[int], FileRangeInfo]:
                List[int]:
                    List of find the specific keyword in offset.
                FileRangeInfo:
                    Range info of found specific keyword in range.
        """
        OffsetList      : List[int]     = list ()
        SectionRangeInfo: FileRangeInfo = FileRangeInfo ()

        for StartLoc, EndLoc in RangeInfo.Data:
            FindResult = self.Find (Keyword, StartLoc, EndLoc)
            if len (FindResult) == 0:
                continue

            SectionRangeInfo.Add (StartLoc, EndLoc)
            for Loc in FindResult:
                OffsetList.append (Loc)

        return OffsetList, SectionRangeInfo

    def Save (self, FilePath: Union[str, os.PathLike]) -> None:
        """ Save the text-based file from memory to storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                The path of text-based file to be output.

        Raises:
            None.

        Returns:
            None.
        """
        WriteTextFile (self.__Content, FilePath)
