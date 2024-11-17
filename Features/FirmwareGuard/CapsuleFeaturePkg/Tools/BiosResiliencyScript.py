## @file
#  Script to support BIOS Resiliency functionality.
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

#
# Native packages
#
import argparse
import copy
import os
#
# AFP packages
#
from CapsuleCommon import *

MAJ_VER: int = 1
MIN_VER: int = 0

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJ_VER}.{MIN_VER}'
__copyright__  : str = 'Copyright (c) 2024 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to support the BIOS Resiliency miscellaneous operation.\n'

#
# Sub-commands for argument parser.
#
CMD_IMAGE_CMD_ACTION   : str       = 'Image'
CMD_IMAGE_ENCODE_ACTION: str       = 'Encode'
CMD_IMAGE_ACTION_LIST  : List[str] = [
    CMD_IMAGE_ENCODE_ACTION,
    ]

#
# Exit code of this script.
#
EXIT_SUCCESS              : int = 0x00
EXIT_TOP_SWAP_SIZE_INVALID: int = 0x01

class BiosResiliencyMisc (object):
    def __init__ (self) -> None:
        """ Class to perform BIOS Resiliency miscellaneous operations.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        DEBUG (DEBUG_TRACE, f'{__version__}')

    def __IsTopSwapSizeValid (self, TopSwapSize: int) -> bool:
        """ Check the Top Swap size validity.

        Args:
            TopSwapSize (int):
                Size of the Top Swap in platform.

        Raises:
            None.

        Returns:
            bool:
                False - Top Swap size is invalid.
                True  - Top Swap size is valid.
        """
        IsValid: bool = True

        if not isinstance (TopSwapSize, int):
            DEBUG (DEBUG_ERROR, f'TopSwapSize shall be int type.')
            return False

        if TopSwapSize % SIZE_1MB != 0:
            IsValid = False
            DEBUG (DEBUG_ERROR, f'Top Swap is not 1MB aligned.')
        elif TopSwapSize > SIZE_8MB:
            IsValid = False
            DEBUG (DEBUG_ERROR, f'Top Swap size is out of platform supported.')

        return IsValid

    #
    # CMD_IMAGE_CMD_ACTION command.
    #
    def EncodeBiosResImage (
        self,
        InputPath  : str,
        OutputPath : str,
        TopSwapSize: int,
        ) -> int:
        """ Exposed function to generate the Resiliency BIOS ROM image.

        Note:
            (1) This would copy the IBB region into IBB-R region.
            (2) The offset of the BIOS image is dependent on the size of
                BIOS image and size of Top Swap.
            (3) Below configuration is the sample based on the 16MB
                Resiliency BIOS with 5MB Top Swap.

                (4G-16M) +---------------------+
                         |         NVS         |
                         +---------------------+
                         |         OBB         |
                         +---------------------+
                         |        IBB-R        | 5MB (Top Swap Size)
                         +---------------------+
                         |         IBB         | 5MB (Top Swap Size)
                    (4G) +---------------------+

        Args:
            InputPath (str):
                Path to the input BIOS ROM image file.
            OutputPath (str):
                Path to the generated BIOS ROM image file.
            TopSwapSize (int):
                Size of the Top Swap in platform.

        Raises:
            None.

        Returns:
            int:
                Exit code of this function.
        """
        InputBuffer : ByteBuffer = None
        OutputBuffer: ByteBuffer = None
        IbbBuffer   : ByteBuffer = None
        IbbOffset   : int        = None
        IbbROffset  : int        = None

        #
        # Check the input argument validity.
        #
        CheckPathList: List[str] = [
            InputPath,
            ]
        CheckFileListExist (CheckPathList)

        if not self.__IsTopSwapSizeValid (TopSwapSize):
            DEBUG (DEBUG_ERROR, f'Invalid Top Swap size - [0x{TopSwapSize:08X}]')
            return EXIT_TOP_SWAP_SIZE_INVALID

        #
        # Get the image offset information.
        #
        InputBuffer = ByteBuffer (FilePath = AbsPath (InputPath))
        IbbOffset   = InputBuffer.Size - 1 * TopSwapSize
        IbbROffset  = InputBuffer.Size - 2 * TopSwapSize
        IbbBuffer   = ByteBuffer (
                        Buffer = (copy.deepcopy (InputBuffer.Buffer))[IbbOffset:]
                        )

        DEBUG (DEBUG_TRACE, f'BIOS Size      : 0x{InputBuffer.Size:08X}')
        DEBUG (DEBUG_TRACE, f'Top Swap Size  : 0x{TopSwapSize:08X}')
        DEBUG (DEBUG_TRACE, f'IBB Offset     : 0x{IbbOffset:08X}')
        DEBUG (DEBUG_TRACE, f'IBB-R Offset   : 0x{IbbROffset:08X}')
        DEBUG (DEBUG_TRACE, f'IBB Buffer Size: 0x{IbbBuffer.Size:08X}')

        #
        # Generate the Resiliency BIOS ROM image file.
        #
        OutputBuffer = ByteBuffer (
                         Buffer = (copy.deepcopy (InputBuffer.Buffer))
                         )

        for Idx in range (IbbBuffer.Size):
            OutputBuffer.Buffer[IbbROffset + Idx] = IbbBuffer.Buffer[Idx]

        OutputBuffer.Save (AbsPath (OutputPath))

        return EXIT_SUCCESS

def _HandleImageCmd (Args: argparse.Namespace) -> int:
    """ Internal function to handle the CMD_IMAGE_CMD_ACTION command.

    Args:
        Args (argparse.Namespace):
            The argument object parsing from user input.

    Raises:
        None.

    Returns:
        int:
            Exit code of this function.
    """
    ExitCode: int                = EXIT_SUCCESS
    Misc    : BiosResiliencyMisc = BiosResiliencyMisc ()

    if Args.Action == CMD_IMAGE_ENCODE_ACTION:
        #
        # Handle the action for "Encode" the Resiliency BIOS image.
        #
        ExitCode = Misc.EncodeBiosResImage (
                          InputPath   = AbsPath (Args.InputPath),
                          OutputPath  = AbsPath (Args.OutputPath),
                          TopSwapSize = ToInt (Args.TopSwapSize),
                          )

    return ExitCode

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
    ImageParser   : argparse.ArgumentParser    = None

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
    # Image sub-parser.
    #
    ImageParser = Subparsers.add_parser (
                               CMD_IMAGE_CMD_ACTION,
                               help = 'Operate the BIOS Resiliency image.',
                               )

    ImageParser.add_argument (
                     '-A',
                     '-a',
                     '--action',
                     dest     = 'Action',
                     help     = f'Action to be performed. (One of {CMD_IMAGE_ACTION_LIST})',
                     required = True,
                     )

    ImageParser.add_argument (
                     '-I',
                     '-i',
                     '--input',
                     dest     = 'InputPath',
                     help     = 'Input the BIOS image.',
                     required = True,
                     )

    ImageParser.add_argument (
                     '-O',
                     '-o',
                     '--output',
                     dest     = 'OutputPath',
                     help     = 'Path to save the operated BIOS image.',
                     required = True,
                     )

    ImageParser.add_argument (
                     '-TS',
                     '-ts',
                     '--top-swap-size',
                     dest     = 'TopSwapSize',
                     help     = 'Size of the Top Swap in platform.',
                     required = True,
                     )

    return Parser.parse_args ()

if __name__ == '__main__':
    Args    : argparse.Namespace = GetArgs ()
    ExitCode: int                = EXIT_SUCCESS

    #
    # Set the debug level to DEBUG_TRACE to dump all information.
    #
    SetDebugLevel (DEBUG_TRACE)

    if Args.Command == CMD_IMAGE_CMD_ACTION:
        ExitCode = _HandleImageCmd (Args = Args)

    exit (ExitCode)
