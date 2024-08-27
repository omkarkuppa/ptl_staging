## @file
#  File to modify the BPM manifest parameter file.
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
# @par Specification
##
import argparse
import os
import sys

from BitOperation import BitOperation
from FileBuffer import FileBuffer

__prog__        = os.path.basename (__file__)
__version__     = "%s Version %s" % (__prog__, '0.1')
__copyright__   = "Copyright (c) 2024, Intel Corporation. All rights reserved.\n"
__description__ = "Script to modify the BPM manifest parameter file.\n"

IBB_SET_TYPE_STR = "IbbSetType"
COLD_BOOT_TYPE   = "0:ColdBoot"
S3_RESUME_TYPE   = "1:S3Resume"

IBB_FLAG_STR = "IbbFlags"
IBB_FLAGS_BOOT_COMPONENT_BIT = 0xA

def BoolType (Value):
    """ Check the input type is bool.

    Args:
        Value [str]: check the input Value is True/False.

    Returns:
        True : The input value if True.
        False: The input value is False.

    Raises:
        None.
    """
    if Value.lower () == "false":
        return False
    elif Value.lower () == "true":
        return True
    else:
        print (f"[ERROR] Please enter True/False as a valid boolean value. ({Value})")
        sys.exit (1)

def GenerateIntervalTuple (InputList, MaxLen, StartPos=0):
    """ Generate the range

    Args:
        InputList [str]    : input keyword list to sort.
        MaxLen    [number] : input for Max Length value.
        StartPos  [number] : input for starting position.

    Returns:
        Result [List]: List of section interval

    Raises:
        None.
    """
    Result = list ()
    KeyPointList = list (set ([StartPos] + InputList + [MaxLen]))
    KeyPointList.sort ()
    for Idx in range (len (KeyPointList)):
        if Idx < (len (KeyPointList) - 1):
            Result.append ((KeyPointList[Idx], KeyPointList[Idx+1]))
    return Result

def ToggleIbbFlag (FileBuffer, BitPosition, Supported, BootType):
    """ To toggle the IBB flag bit position in ColdBoot or S3Resume section.

    Args:
        FileBuffer  [FileBuffer]: To do file operation.
        BitPosition [number]    : IBB Flag Bit Position
        Supported   [bool]      : True means to toggle bit value to 1 (Support).
                                  False means to toggle bit value to 0.
        BootType    [str]       : To identify Cold Boot or S3 Resume

    Returns:
        None

    Raises:
        None.
    """
    #
    # Find the "IbbSetType" string.
    #
    IBBSetTypeLocList = File.FindKeywordInLine (IBB_SET_TYPE_STR)
    if len (IBBSetTypeLocList) == 0:
        print (f"Fail to find the flag: {IBB_SET_TYPE_STR} from input file {FileBuffer.Path}")
        sys.exit (1)
    ManifestSectionList = GenerateIntervalTuple (IBBSetTypeLocList, MaxLen=File.Length, StartPos=0)
    print (f"Toggle IBB Flag for {BootType}")
    #
    # Find the BootType string.
    #
    BootTypeStrRange, BootTypeStrPos = FileBuffer.FindKeywordInRange (ManifestSectionList, BootType)
    if len (BootTypeStrRange) == 0:
        print (f"[ERROR] Not found the '{BootType}' string in template file !!!")
        sys.exit (1)

    #
    # Find the "IbbFlags" string.
    #
    IBBFlagsStrRange, IBBFlagsStrPos = File.FindKeywordInRange (BootTypeStrRange, IBB_FLAG_STR)
    if len (IBBFlagsStrPos) == 0:
        print ("[ERROR] Not found the IbbFlags in this section !!!")
        sys.exit (1)
    elif len (IBBFlagsStrPos) > 1:
        print ("[ERROR] Found more than one IbbFlags in this section !!!")
        sys.exit (1)

    Position = IBBFlagsStrPos[0]
    OriString = (File.GetLineData (Position).split (":")[-1]).strip ()
    IBBFlags = BitOperation (OriString)
    if Supported:
        print ("Set the input Ibb flag to TRUE")
        IBBFlags.SetBit (BitPosition)
    else:
        print ("Set the input Ibb flag to FALSE")
        IBBFlags.ClearBit (BitPosition)
    RevString = hex (IBBFlags.Value)
    File.Replace (Position, OriString, RevString)

if __name__ == "__main__":
    def GetArgs ():
        """ Get the user input arguments.

        Args:
            None.

        Returns:
            Args [args]: The argument object parsing from user input.

        Raises:
            None.
        """
        Parser = argparse.ArgumentParser(
            prog=__prog__,
            description=__copyright__ + __description__ + __version__,
            formatter_class=argparse.RawTextHelpFormatter
            )
        Parser.add_argument (
                 "-V",
                 "-v",
                 "--Version",
                 action="version",
                 version=__version__
                 )
        Parser.add_argument (
                 "-F",
                 "-f",
                 "--File",
                 help="Input the BtGAcm manifest file.",
                 type=argparse.FileType ("r"),
                 required=True
                 )
        Parser.add_argument (
                 "-BC",
                 "-bc",
                 "--BootComponent",
                 type=BoolType,
                 help="Toggle the BootComponent bit.",
                 )
        return Parser.parse_args ()

    Args          = GetArgs ()
    File          = FileBuffer (Args.File)
    BootComponent = Args.BootComponent

    if BootComponent is not None:
        ToggleIbbFlag (File, IBB_FLAGS_BOOT_COMPONENT_BIT, BootComponent, COLD_BOOT_TYPE)
        ToggleIbbFlag (File, IBB_FLAGS_BOOT_COMPONENT_BIT, BootComponent, S3_RESUME_TYPE)
    File.Save ()
