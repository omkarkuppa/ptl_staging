## @file
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
#@par Specification
##

import sys
import argparse
from FirmwareStorageFormat.FfsFileHeader import *

EFI_FV_TOTAL_SIZE = 'EFI_FV_TOTAL_SIZE'
EFI_FV_TAKEN_SIZE = 'EFI_FV_TAKEN_SIZE'
EFI_FV_SPACE_SIZE = 'EFI_FV_SPACE_SIZE'

def LoadFvUplMapFile(FilePath):
    FvSpaceInfo = {}
    MapFile = open(FilePath, 'r')
    for Line in MapFile:
        LineSpilt = Line.split(' ')
        if len (LineSpilt) == 3:
            FvSpaceInfo[LineSpilt[0]] = int(LineSpilt[2], 16)
    MapFile.close()
    return FvSpaceInfo[EFI_FV_SPACE_SIZE]

def LoadUniversalPayloadFfsFile(FilePath):
    FfsFile = open(FilePath, 'rb')
    BinData = bytearray(FfsFile.read())
    FfsBuffer = EFI_FFS_FILE_HEADER.from_buffer(BinData, 0)
    UplFfsFileSize = FfsBuffer.FFS_FILE_SIZE
    FfsFile.close()
    return UplFfsFileSize

def Main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-c", "--CheckFvSize", dest="CheckFvSize", nargs=2,
                    help="Check the size between FvUpl available space and UniversalPayload FFS:'-c InputBuildMapFile NewFfsFile'")
    args=parser.parse_args()
    FvUplRemainSpace = LoadFvUplMapFile(args.CheckFvSize[0])
    UplFfsFileSize = LoadUniversalPayloadFfsFile(args.CheckFvSize[1])

    if (FvUplRemainSpace >= UplFfsFileSize):
        EmptySpaceInBytes = FvUplRemainSpace - UplFfsFileSize
        print ('Empty space for FvUpl in bytes : ' + hex(EmptySpaceInBytes))
        return 0
    else:
        RequiredSpaceInBytes = UplFfsFileSize - FvUplRemainSpace
        print ('Required space for FvUpl in bytes : ' + hex(RequiredSpaceInBytes))
        return 1

if __name__ == '__main__':
    sys.exit(Main())