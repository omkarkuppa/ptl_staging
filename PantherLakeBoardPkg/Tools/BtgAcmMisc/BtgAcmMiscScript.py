## @file
#  Script for BtGACM miscellaneous operation.
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

#
# Native packages
#
import argparse
import os
#
# Self modules
#
from BtgAcmMisc import *

MAJ_VER: int = 1
MIN_VER: int = 3

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2025 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to support the BtGACM miscellaneous operation.\n'

#
# Sub-commands for argument parser.
#
CMD_FIT_ENTRY_CMD_ACTION: str = 'FitEntry'
CMD_PADDING_CMD_ACTION  : str = 'Padding'

#
# Definition of exit code.
#
EXIT_SUCCESS: int = 0x00

def GetArgs () -> argparse.Namespace:
    """ Get the user input arguments.

    Args:
        None.

    Raises:
        None.

    Returns:
        Args [argparse.Namespace]:
            The argument object parsing from user input.
    """
    Parser        : argparse.ArgumentParser    = argparse.\
        ArgumentParser (
            prog            = __prog__,
            description     = __copyright__ + __description__ + __version__,
            formatter_class = argparse.RawTextHelpFormatter,
            )
    Subparsers    : argparse._SubParsersAction = \
        Parser.add_subparsers (dest='Command')
    FitEntryParser: argparse.ArgumentParser    = None

    #
    # Main parser.
    #
    Parser.add_argument (
             '-V',
             '-v',
             '--Version',
             action  = 'version',
             version = __version__,
             )

    #
    # FitEntry sub-parser.
    #
    FitEntryParser = Subparsers.add_parser (
                                  CMD_FIT_ENTRY_CMD_ACTION,
                                  help = 'Generate the FIT entry command for FitGen.',
                                  )
    FitEntryParser.add_argument (
                     '-I',
                     '-i',
                     '--input',
                     dest     = 'InputPath',
                     help     = 'Input the BIOS image.',
                     required = True,
                     )
    FitEntryParser.add_argument (
                     '-S',
                     '-s',
                     '--slotsize',
                     dest     = 'SlotSize',
                     help     = 'Slot size of each BtGACM binary.',
                     required = True,
                     )
    FitEntryParser.add_argument (
                     '-O',
                     '-o',
                     '--output',
                     dest     = 'OutputPath',
                     help     = 'Path to save the parameter file.',
                     required = True,
                     )

    #
    # Padding sub-parser.
    #
    PaddingParser = Subparsers.add_parser (
                                  CMD_PADDING_CMD_ACTION,
                                  help = 'Padding the BtGACM binary to assigned slot size.',
                                  )
    PaddingParser.add_argument (
                    '-I',
                    '-i',
                    '--input',
                    dest     = 'InputPath',
                    help     = 'Path of the directory with a series of BtGACM binary files.',
                    required = True,
                    )
    PaddingParser.add_argument (
                    '-S',
                    '-s',
                    '--slotsize',
                    dest     = 'SlotSize',
                    help     = 'Slot size of each BtGACM binary.',
                    required = True,
                    )
    PaddingParser.add_argument (
                    '-O',
                    '-o',
                    '--output',
                    dest     = 'OutputPath',
                    help     = 'Path of the directory to save buffer into the storage.',
                    required = True,
                    )

    return Parser.parse_args ()

if __name__ == '__main__':
    Args    : argparse.Namespace = GetArgs ()
    ExitCode: int                = EXIT_SUCCESS

    if Args.Command == CMD_FIT_ENTRY_CMD_ACTION:
        ExitCode = Type2FitEntryGenerator (
                     BiosFilePath = os.path.abspath (Args.InputPath),
                     OutputPath   = os.path.abspath (Args.OutputPath),
                     SlotSize     = int (Args.SlotSize, 0),
                     )
    elif Args.Command == CMD_PADDING_CMD_ACTION:
        ExitCode = BtgAcmPaddingWrapper (
                     BtgAcmBasePath = os.path.abspath (Args.InputPath),
                     SlotSize       = int (Args.SlotSize, 0),
                     OutputDir      = os.path.abspath (Args.OutputPath),
                     )

    exit (ExitCode)
