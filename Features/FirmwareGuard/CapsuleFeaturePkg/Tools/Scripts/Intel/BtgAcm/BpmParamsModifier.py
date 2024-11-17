## @file
#  Script to modify the BPM (Boot Policy Manifest) parameters file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2024 Intel Corporation.
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

import argparse
import copy
from typing import Dict, List, Union

from CapsuleCommon import *

MAJ_VER: int = 0
MIN_VER: int = 1

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (C) 2024 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to modify BPM (Boot Policy Manifest) parameters file.\n'

#
# BPM parameters file definition.
#
BPM_PARAMS_SPLITTER: str = ':'

#
# Set of IBB section.
#
IBB_SET_TYPE_STR          : str = 'IbbSetType'
IBB_SET_TYPE_COLD_BOOT_STR: str = '0:ColdBoot'
IBB_SET_TYPE_S3_RESUME_STR: str = '1:S3Resume'

IBB_SET_TYPE_LIST: List[str] = [
    IBB_SET_TYPE_COLD_BOOT_STR,
    IBB_SET_TYPE_S3_RESUME_STR,
    ]

#
# Flags of IBB.
#
IBB_FLAGS_STR                  : str = 'IbbFlags'
IBB_FLAG_TOP_SWAP_SUPPORTED_BIT: int = BIT4

#
# Script exit code
#
EXIT_SUCCESS: int = 0x00

class _IbbSetInfo (object):
    def __init__ (self, SetType: str, RangeInfo: FileRangeInfo):
        """ Class to represent the IBB set information.

        Args:
            SetType (str):
                Type of IBB set.
            RangeInfo (FileRangeInfo):
                The 0-based range information.
                (Shall be FileRangeInfo type.)

        Raises:
            None.

        Returns:
            None.
        """
        self.__SetType  : str           = SetType
        self.__RangeInfo: FileRangeInfo = RangeInfo

        self.__PreCheck ()

        self.__StartOffset: int = None
        self.__EndOffset  : int = None
        self.__StartOffset, self.__EndOffset = self.__GetOffsetInfo ()

        self.__Dump ()

    @property
    def SetType (self) -> str:
        """ Return the type of IBB set.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String of type of IBB set.
        """
        return self.__SetType

    @property
    def StartOffset (self) -> int:
        """ Return the start offset of IBB set. (0-based)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Start offset of IBB set. (0-based)
        """
        return self.__StartOffset

    @property
    def EndOffset (self) -> int:
        """ Return the end offset of IBB set. (0-based)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                End offset of IBB set. (0-based)
        """
        return self.__EndOffset

    @property
    def RangeInfo (self) -> FileRangeInfo:
        """ Return the range information.

        Args:
            None.

        Raises:
            None.

        Returns:
            FileRangeInfo:
                The 0-based range information.
                (Shall be FileRangeInfo type.)
        """
        return self.__RangeInfo

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) SetType is not str type.
                (2) RangeInfo is not FileRangeInfo type.
            ValueError:
                (1) SetType is not a valid value.
                (1) SetType instance number is not valid.

        Returns:
            None.
        """
        if not isinstance (self.__SetType, str):
            raise TypeError (f'Type of IBB set shall be str type.')
        elif not isinstance (self.__RangeInfo, FileRangeInfo):
            raise TypeError (f'Range information shall be FileRangeInfo type.')

        if self.__SetType not in IBB_SET_TYPE_LIST:
            raise ValueError (f'Invalid type of IBB set - {self.__SetType}')

        if self.__RangeInfo.Length != 1:
            raise ValueError (
                    f'Number of [{self.__SetType}] is not valid - '
                    f'{self.__RangeInfo.Length}'
                    )

    def __GetOffsetInfo (self) -> Union[int, int]:
        """ Extract the offset information.

        Note:
            The range information of IBB set must be 1 pair only.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[int, int]:
                int:
                    Start offset of IBB set. (0-based)
                int:
                    End offset of IBB set. (0-based)
        """
        StartOffset: int = self.__RangeInfo.Data[0][0]
        EndOffset  : int = self.__RangeInfo.Data[0][1]

        return StartOffset, EndOffset

    def __Dump (self) -> None:
        """ Dump the debug information of this object.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DEBUG (DEBUG_TRACE, f'IBB Set Type: {self.SetType}')
        DEBUG (DEBUG_TRACE, f'Start Offset: {self.StartOffset}')
        DEBUG (DEBUG_TRACE, f'End Offset  : {self.EndOffset}')
        DEBUG (DEBUG_TRACE, f'Range Info  : {self.RangeInfo.Data}')

class BpmParamsFile (object):
    def __init__ (self, FilePath: str) -> None:
        """ Class to represent the BPM parameters file.

        Args:
            FilePath (str):
                Path to the BPM parameters file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FilePath: str = AbsPath (FilePath)

        self.__PreCheck ()

        self.__BpmParamsFile: File = \
            File (
              FilePath    = self.__FilePath,
              IsReadLines = True,
              )

        self.__IbbSetInfoDict: Dict[str, _IbbSetInfo] = \
            self.__GetIbbSetInfoDict ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) BPM parameters file path is not exist.
                (2) BPM parameters file path is not a file.

        Returns:
            None.
        """
        if not IsPathExist (self.__FilePath):
            raise ValueError (
                    f'BPM params path [{self.__FilePath}] is not exist!'
                    )
        elif not IsFile (self.__FilePath):
            raise ValueError (
                    f'BPM params path [{self.__FilePath}] is not file!'
                    )

    def __GetRangeInfo (
        self,
        InputList: List[int],
        Start    : int = None,
        End      : int = None,
        ) -> FileRangeInfo:
        """ Return the range information of the BPM parameters file.

        Args:
            InputList (List[int]):
                List of offset information.
            Start (int, optional):
                Start offset to be added.
                If it is not None, add it into the list.
                Defaults to None.
            End (int, optional):
                End offset to be added.
                If it is not None, add it into the list.
                Defaults to None.

        Raises:
            TypeError:
                (1) Start value is not int type.
                (2) End value is not int type.
                (3) Start value is not positive value.
                (4) End value is not positive value.

        Returns:
            FileRangeInfo:
                The 0-based range information.
                (Shall be FileRangeInfo type.)
        """
        Value     : int           = None
        OffsetList: List[int]     = copy.deepcopy (InputList)
        RangeInfo : FileRangeInfo = FileRangeInfo ()

        #
        # Append the Start and End offset into list.
        # This would help to create the interval range.
        #
        for Value in ([Start, End]):
            if Value is None:
                continue

            if not isinstance (Value, int):
                raise TypeError (f'[{Value}] shall be int type.')

            if Value < 0:
                raise ValueError (f'[{Value}] shall be positive value.')

            OffsetList.append (Value)

        #
        # Sort the members of list.
        #
        OffsetList.sort ()

        #
        # Build the range information.
        #
        for Idx in range (len (OffsetList)):
            if Idx < (len (OffsetList) - 1):
                RangeInfo.Add (OffsetList[Idx], OffsetList[Idx + 1])

        return RangeInfo

    def __GetIbbSetInfoDict (self) -> Dict[str, _IbbSetInfo]:
        """ Return the IBB set information dictionary.

        Args:
            None.

        Raises:
            ValueError:
                Failed to find the IBB_SET_TYPE_STR string.

        Returns:
            Dict[str, _IbbSetInfo]:
                str:
                    Type of IBB set.
                _IbbSetInfo:
                    Information of correspond IBB set.
                    (Shall be _IbbSetInfo type.)
        """
        OffsetList     : List[int]              = None
        IbbSetTypeStr  : str                    = None
        InputRangeInfo : FileRangeInfo          = None
        OutputRangeInfo: FileRangeInfo          = None
        IbbSetInfo     : _IbbSetInfo            = None
        IbbSetInfoDict : Dict[str, _IbbSetInfo] = dict ()

        #
        # Find the keyword of IBB_SET_TYPE_STR.
        #
        OffsetList = self.__BpmParamsFile.Find (Keyword = IBB_SET_TYPE_STR)
        if len (OffsetList) == 0:
            raise ValueError (
                    f'Fail to find the type [{IBB_SET_TYPE_STR}] '
                    f'from input file {self.__FilePath}'
                    )

        #
        # Get the range information based on offset list.
        #
        InputRangeInfo = self.__GetRangeInfo (
                                InputList = OffsetList,
                                Start     = 0,
                                End       = self.__BpmParamsFile.Line,
                                )

        #
        # Iterate all of the possible type of IBB set.
        #   - Collect all the range information.
        #   - Build the mapping dictionary of different IBB set type.
        #
        for IbbSetTypeStr in IBB_SET_TYPE_LIST:
            _, OutputRangeInfo = \
                self.__BpmParamsFile.FindInRange (
                                       Keyword   = IbbSetTypeStr,
                                       RangeInfo = InputRangeInfo,
                                       )

            IbbSetInfo = _IbbSetInfo (
                           SetType   = IbbSetTypeStr,
                           RangeInfo = OutputRangeInfo,
                           )
            IbbSetInfoDict.update ({ IbbSetTypeStr: IbbSetInfo })

        return IbbSetInfoDict

    def __IsIbbSetTypeValid (self, IbbSetType: str) -> bool:
        """ Check if the type of IBB set is valid.

        Args:
            IbbSetType (str):
                Type of IBB set string to be trialed.

        Raises:
            None.

        Returns:
            bool:
                False - Type of IBB set is invalid.
                True  - Type of IBB set is valid.
        """
        if IbbSetType not in IBB_SET_TYPE_LIST:
            return False

        return True

    def __GetValueFromFile (self, String: str) -> str:
        """ Retrieve the value from specific line string.

        Note:
            This function is targeted for "Key:Value" format string.

        Args:
            String (str):
                String to be decoded.

        Raises:
            ValueError:
                (1) Splitter symbol of BPM parameters file is not exist.

        Returns:
            str:
                Value part of the string.
        """
        if BPM_PARAMS_SPLITTER not in String:
            raise ValueError (
                    f'Value splitter - [{BPM_PARAMS_SPLITTER}] is not found '
                    f'- [{String}].'
                    )

        return (String.split (BPM_PARAMS_SPLITTER)[-1]).strip ()

    def ToggleIbbFlags (
        self,
        IbbSetType: str,
        BitNum    : int,
        Supported : bool,
        ) -> None:
        """ Exposed function to toggle IBB flags resident in IBB set.

        Args:
            IbbSetType (str):
                Type of IBB set.
            BitNum (int):
                Specific bit number to be operated.
            Supported (bool):
                Boolean value to toggle specific bit number.
                False - Disabled.
                True  - Enabled.

        Raises:
            TypeError:
                (1) IbbSetType is not str type.
                (2) BitNum is not int type.
                (3) Supported is not bool type.
            ValueError:
                (1) IbbSetType is invalid.

        Returns:
            None.
        """
        IsFound   : bool        = None
        Key       : str         = None
        IbbSetInfo: _IbbSetInfo = None
        OffsetList: List[int]   = None
        Offset    : int         = None
        IbbFlagVal: Bit         = None
        OriValStr : str         = None
        RevValStr : str         = None

        if not isinstance (IbbSetType, str):
            raise TypeError (f'IbbSetType shall be str type.')
        elif not isinstance (BitNum, int):
            raise TypeError (f'BitNum shall be int type.')
        elif not isinstance (Supported, bool):
            raise TypeError (f'Supported shall be bool type.')

        if not self.__IsIbbSetTypeValid (IbbSetType):
            raise ValueError (f'Invalid type of IBB set - [{IbbSetType}].')

        IsFound, Key = SearchKeyInDict (self.__IbbSetInfoDict, IbbSetType)
        if not IsFound:
            raise ValueError (f'Not found assigned IBB set type - [{IbbSetType}]')

        #
        # Get the IBB set set type offset.
        #
        IbbSetInfo    = self.__IbbSetInfoDict[Key]
        OffsetList, _ = self.__BpmParamsFile.FindInRange (
                                               Keyword   = IBB_FLAGS_STR,
                                               RangeInfo = IbbSetInfo.RangeInfo,
                                               )
        if len (OffsetList) != 1:
            raise ValueError (
                    f'Invalid number of {IBB_FLAGS_STR} - [{len (OffsetList)}]'
                    )

        #
        # Retrieve the origin value.
        #
        Offset     = OffsetList[0]
        OriValStr  = self.__GetValueFromFile (
                            self.__BpmParamsFile.GetLineData (Offset),
                            )
        IbbFlagVal = Bit (Value = OriValStr)

        #
        # Toggle the assigned bit based on supported flag.
        #
        if Supported:
            IbbFlagVal.SetBit (BitNum)
        else:
            IbbFlagVal.ClearBit (BitNum)

        #
        # Update the IBB flags information.
        #
        RevValStr = hex (IbbFlagVal.Target)
        self.__BpmParamsFile.Replace (
                               Revised = RevValStr,
                               Source  = OriValStr,
                               Line    = Offset,
                               )

    def ToggleTopSwapSupportedFlag (self, Supported: bool) -> None:
        """ Exposed function to toggle Top Swap supported bit.

        Args:
            Supported (bool):
                Boolean value to toggle the Top Swap supported bit.
                False - Disabled.
                True  - Enabled.

        Raises:
            None.

        Returns:
            None.
        """
        self.ToggleIbbFlags (
               IBB_SET_TYPE_COLD_BOOT_STR,
               IBB_FLAG_TOP_SWAP_SUPPORTED_BIT,
               Supported,
               )

    def Save (self, FilePath: str) -> None:
        """ Save the modified BPM parameters file into storage.

        Args:
            FilePath (str):
                Path to save the modified BPM parameters file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BpmParamsFile.Save (FilePath)

if __name__ == '__main__':
    def GetArgs ():
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
                 action   = 'version',
                 version  = __version__
                 )
        Parser.add_argument (
                 '-I',
                 '-i',
                 '--input',
                 dest     = 'InputFilePath',
                 help     = 'Path to BPM parameters file to be modified.',
                 required = True,
                 )
        Parser.add_argument (
                 '-O',
                 '-o',
                 '--output',
                 dest     = 'OutputFilePath',
                 help     = 'Path to modified BPM parameters file to be saved.',
                 required = True,
                 )
        Parser.add_argument (
                 '-TS',
                 '-ts',
                 '--TopSwap',
                 dest     = 'TopSwapSupported',
                 type     = ToBool,
                 help     = 'Toggle the TopSwap Supported bit.',
                 default  = None,
                 required = False,
                 )
        return Parser.parse_args ()

    Args     : argparse.Namespace = GetArgs ()
    BpmParams: BpmParamsFile      = BpmParamsFile (FilePath = Args.InputFilePath)

    if Args.TopSwapSupported is not None:
        #
        # Toggle the Top Swap supported bit if needed.
        #
        BpmParams.ToggleTopSwapSupportedFlag (Args.TopSwapSupported)

    BpmParams.Save (Args.OutputFilePath)
