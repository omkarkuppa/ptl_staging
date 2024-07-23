## @file
#  File to generate the FIT Table Type 2 Entry.
#   * Do not support the BtGAcm FFS is EFI_FFS_FILE_HEADER2.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
##
import argparse
import os
import sys
from AcmFfsParser import *
from AcmInfoTableParser import *
from AcmProcessorIdListParser import *
from AcmFMSParser import *

__prog__        = os.path.basename(__file__)
__version__     = '%s Version %s' % (__prog__, '0.1')
__copyright__   = 'Copyright (c) 2022, Intel Corporation. All rights reserved.\n'
__description__ = 'Get the StartupAcm FIT table Entry Data from Binary.\n'

FitEntryData   = list ()
FitToolCmdList = list ()
FitToolCmd     = str ()

def GetArgument ():
    """ Get the user input arguments.

    Args:
        None.

    Returns:
        Args [args]: The argument object parsing from user input.

    Raises:
        None.
    """
    Parser = argparse.ArgumentParser ()
    Parser.add_argument (
                 '-V',
                 '-v',
                 '--Version',
                 action = 'version',
                 version = __version__
                 )
    Parser.add_argument (
                '-f',
                '-F',
                '--fv',
                type = argparse.FileType('rb'),
                required = True,
                dest = 'BiosImage',
                help = 'Input the BIOS image.'
                )
    Parser.add_argument (
                '-a',
                '-A',
                '--align',
                dest = 'Align',
                help = "AlignmentValue."
                )
    Parser.add_argument (
                '-o',
                '-O',
                '--output',
                dest = 'Output',
                help = "Output the parameter file."
                )
    Args = Parser.parse_args ()
    return Args

if __name__ == '__main__':
    Args = GetArgument ()
    Idx  = 0

    BiosImageBuffer  = Args.BiosImage.read ()
    OutputFilePath   = os.path.realpath (Args.Output)
    ACM_ALIGN        = int (Args.Align, 0)
    FLASH_BASE       = ADDRESS_4GB - len (BiosImageBuffer)  # BIOS ROM would be mapping into from 4G - BIOS_IMAGE_SIZE.

    #
    # Find the StartupAcm FFS in binary buffer image.
    #
    AcmBufferList = AcmFfsParser (FLASH_BASE, BiosImageBuffer, ACM_ALIGN).Get ()

    #
    # Check if get the StartupAcm in input binary image buffer.
    #
    if len (AcmBufferList) == 0:
        print ('Cannot find any StartupAcm in BIOS.')
        sys.exit (1)

    #
    # Parsing each StartupAcm binary buffer.
    #
    for AcmBuffer in AcmBufferList:
        Idx     += 1
        AcmBase  = AcmBuffer['Base']
        AcmSize  = AcmBuffer['Size']

        print (f"Attempt to parse the [{Idx}] {FormatHex (AcmBase)} (Size: {FormatHex (AcmSize)})")

        ProcessorIdListPtr  = AcmInfoTableParser (AcmBuffer['Buffer']).GetProcessorIdListPtr ()

        if ProcessorIdListPtr == None:
            continue

        ProcessorIdListPtr  = int (ProcessorIdListPtr, 0)
        ProcessorIdListData = AcmProcessorIdListParser (AcmBuffer['Buffer'], ProcessorIdListPtr).Get ()

        for ProcessorId in ProcessorIdListData:
            Fms     = AcmFMSParser (FormatHex (ProcessorId['FMS'])).Get ()
            FmsMask = AcmFMSParser (FormatHex (ProcessorId['FMSMask'])).Get ()

            SizeByte0  = FormatHex (Fms['Family'] + Fms['Model'])
            SizeByte1  = FormatHex (Fms['Ext_Model'] + Fms['Type'])
            SizeByte2  = FormatHex (FmsMask['Family'] + FmsMask['Model'])
            RsvdByte   = FormatHex (FmsMask['Ext_Model'] + FmsMask['Type'])
            ChksumByte = FormatHex (FmsMask['Ext_Family'][0:1] + Fms['Ext_Family'][0:1])

            AcmInfo = {
                'AcmBase'   : AcmBase,
                'AcmSize'   : AcmSize,
                'FMS'       : FormatHex (ProcessorId['FMS']),
                'FMSMask '  : FormatHex (ProcessorId['FMSMask'])
            }

            print (AcmInfo)
            FitEntryData.append (AcmInfo)

            Cmd = f"-S {AcmBase} {AcmSize} -I {FormatHex (ProcessorId['FMS'])} {FormatHex (ProcessorId['FMSMask'])}"
            FitToolCmdList.append (Cmd)

    #
    # Generate the input argument for FitGen tool.
    #
    if os.name == 'nt':
        # Windows environment
        FitToolCmd = ' ^\n'.join (FitToolCmdList)
    else:
        FitToolCmd = ' \n'.join (FitToolCmdList)

    #
    # Add the new end line for the export file. (Require for Linux-Based bash file)
    #
    FitToolCmd += "\n"

    #
    # Write the output file.
    #
    File   = open (OutputFilePath, "wt")
    Status = File.write (FitToolCmd)
    File.close ()

    sys.exit (0)
