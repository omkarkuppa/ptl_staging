## @file
#  Python module package initial files for BtgAcmMisc package.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
#  @par Specification Reference:
#
##

import os
from typing import List

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc._BtgAcmFfsParser import *
from BtgAcmMisc._BtgAcmBinParser import *
from BtgAcmMisc._BtgAcmFitEntryGenerator import *
from BtgAcmMisc._BtgAcmPadding import *

#
# Constant Variable.
#
ADDRESS_4GB: int = 0x0000000100000000

BTG_ACM_SLOT_SIZE_ALIGNMENT: int = 0x1000

#
# Pre-defined String.
#
CMD_JOIN_STR: str = GetCmdJoinStr ()
NEW_LINE_STR: str = '\n'

STATUS_SUCCESS: int = 0x0

def _IsSlotSizeValid (SlotSize: int) -> bool:
    """ Check if slot size is valid.

    Args:
        SlotSize (int):
            Slot size of each BtGACM binary file.

    Raises:
        TypeError:
            SlotSize is not int type.
        ValueError:
            SlotSize is not aligned.

    Returns:
        bool:
            False - Slot size is not valid.
            True  - Slot size is valid.
    """
    if not isinstance (SlotSize, int):
        raise TypeError (f'SlotSize shall be int type.')

    if SlotSize % BTG_ACM_SLOT_SIZE_ALIGNMENT != 0:
        raise ValueError (f'SlotSize is not aligned - [{SlotSize}].')

    return True

def Type2FitEntryGenerator (
    BiosFilePath: str,
    OutputPath  : str,
    SlotSize    : int,
    ) -> int:
    """ API to generate the FitGen input parameter for Type-2 entry.

    Args:
        BiosFilePath (str):
            Path to the BIOS image to be decoded.
        OutputPath (str):
            Path to save the FitGen tool type-2 entry command.
        SlotSize (int):
            Slot size of each BtGACM binary file.

    Raises:
        None.

    Returns:
        int:
            STATUS_SUCCESS - Succeed to execute this function.
    """
    FfsParser  : BtgAcmFfsParser          = None
    BinParser  : BtgAcmBinParser          = None
    BinInfo    : BtgAcmBinInfo            = None
    FitEntryGen: BtgAcmFitEntryGenerator  = None

    BiosBuffer   : ByteBuffer = ByteBuffer (FilePath = BiosFilePath)
    BIOS_MEM_BASE: int        = ADDRESS_4GB - BiosBuffer.Size
    CmdList      : List[str]  = list ()
    CmdContent   : str        = None

    _IsSlotSizeValid (SlotSize)

    FfsParser = BtgAcmFfsParser (Buffer = BiosBuffer)
    BinParser = BtgAcmBinParser (
                  DataBuffer = FfsParser.DataBuffer,
                  SlotSize   = SlotSize,
                  )

    for BinInfo in BinParser.BinInfoList:
        Address = BIOS_MEM_BASE + FfsParser.DataOffset + BinInfo.Offset

        FitEntryGen = BtgAcmFitEntryGenerator (
                        Address  = Address,
                        BinInfo  = BinInfo,
                        SlotSize = SlotSize,
                        )
        CmdList.extend (FitEntryGen.CmdList)

    #
    # Generate the input argument for FitGen tool.
    #
    CmdContent = CMD_JOIN_STR.join (CmdList)

    #
    # Add the new end line for the export file.
    # (Require for POSIX-based system.)
    #
    CmdContent += NEW_LINE_STR

    #
    # Export the output file.
    #
    with open (os.path.abspath (OutputPath), 'wt') as File:
        File.write (CmdContent)

    return STATUS_SUCCESS

def BtgAcmPaddingWrapper (
    BtgAcmBasePath: Union[str, os.PathLike],
    SlotSize      : int,
    OutputDir     : Union[str, os.PathLike],
    ) -> int:
    """ Wrapper function for padding the BtGACM binary to slot size.

    Args:
        BtgAcmBasePath (Union[str, os.PathLike]):
            Path to the directory which place the BtGACM binary files.
        SlotSize (int):
            Slot size of each BtGACM binary file.
        OutputDir (Union[str, os.PathLike]):
            Path to the directory to output the padded BtGACM binary.

    Raises:
        None.

    Returns:
        int:
            Status of this function execution.
    """
    Padding: BtgAcmPadding = None

    _IsSlotSizeValid (SlotSize)

    Padding = BtgAcmPadding (
                BtgAcmBasePath = BtgAcmBasePath,
                SlotSize       = SlotSize,
                )
    Padding.Execute (OutputDir)

    return STATUS_SUCCESS

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'Type2FitEntryGenerator',
    'BtgAcmPaddingWrapper',
    ]
