## @file
#  FspTop binary generation tool.
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

import os
import re
import subprocess
import sys
import argparse
import struct

#
# Globals for help information
#
__copyright__   = "INTEL CONFIDENTIAL \n Copyright (c) 2024, Intel Corporation. All rights reserved. "
__description__ = 'FspTop binary generation tool.\n'
__prog__        = sys.argv[0]

#
# Set chunk size as 4K for searching FV header in FSP binary
#
FV_SEARCH_CHUNK_SIZE = 4096

#
# FSP Header signature
#
FSPH_SIGNATURE = "FSPH"

#
# FBM should be patched in Fsp Top padding file, at least 8KB need be left in padding file
# FBM is located at the GFbmOffsetToFspt before the start of FSP-T FV
#
FBM_OFFSET_TO_FSPT = 0x2000

def RunCommand(Cmd: str, Timeout = None) -> str:
    if not isinstance(Timeout, int):
        Timeout = None
    Process = subprocess.Popen(Cmd,
                               shell = True,
                               stdout = subprocess.PIPE,
                               stdin = subprocess.PIPE,
                               stderr = subprocess.STDOUT,
                               universal_newlines = True)
    try:
        Output = Process.communicate(timeout = Timeout)[0]
    except subprocess.TimeoutExpired as e:
        subprocess.Popen("task kill /t /f /pid %s" % Process.pid, shell = True)
        Output = e
        print("Timeout occurred after %s seconds" % str(timeout if timeout else "zero"))
    return Output

#
# Search the last FV by locating the last FV Header
#
def GetLastFvBase(FdFile):
    FspFd = open(FdFile, "rb")
    FileSize = os.path.getsize(FdFile)
    Pos = FileSize - FV_SEARCH_CHUNK_SIZE
    FsphSignature = bytes(FSPH_SIGNATURE, 'utf-8')
    #
    # Search Last FV base
    #
    while Pos >= 0:
        FspFd.seek(Pos)
        Chunk = FspFd.read(FV_SEARCH_CHUNK_SIZE)
        if Chunk.find(FsphSignature) != -1:
            print("The last FV base address is:", hex(Pos))
            return Pos
        Pos -= FV_SEARCH_CHUNK_SIZE
    print("Error! There is no FV in " + FdFile)
    exit(-1)

#
# This class is used to parse flash map settings
#
class Flashmap():
    def __init__(self, FlashmapFile, PrepRelease):
        FileRead = open(FlashmapFile, "r")
        Lines = FileRead.readlines()
        FileRead.close()
        ReleaseLinePos = -1
        DebugLinePos = -1
        Index = 0
        while (ReleaseLinePos == -1 or DebugLinePos == -1) and Index < len(Lines):
            if "DEBUG" in Lines[Index]:
                DebugLinePos = Index
            if "Release" in Lines[Index]:
                ReleaseLinePos = Index
            Index += 1
        if ReleaseLinePos == -1 or DebugLinePos == -1:
            print("Error! In file: " + FlashmapFile + ", string: 'DEBUG' and 'Release' don't exist!")
            exit(-1)

        if PrepRelease == 'RELEASE':
            self.Map = Lines[ReleaseLinePos:]
        elif PrepRelease == 'DEBUG':
            self.Map = Lines[DebugLinePos:ReleaseLinePos]

        self.FilePath       =  FlashmapFile
        #
        #  This script parses the following PCDs.
        #
        self.FspTopSizeStr = "gBoardModuleTokenSpaceGuid.PcdFspTopSize"
        self.FspTopSize    = 0xFFFFFFFF

    #
    # Assuming Flash Map FDF has no !if/!else between the Lines but only one set of mapping PCDs.
    # This call returns the last hit of the matches by parsing the file. If no hit, raise exception.
    #
    def ReadSingleSetting(self, Setting):
        SettingList = []
        PatternPcd = r"(?m)^\s*SET\s*" + Setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        for Line in self.Map:
            for m in re.finditer(PatternPcd, Line):
                SettingList.append(int(m.group("value"), base = 16))
        try:
            ReadValue = SettingList[-1]
        except IndexError as err:
            print("Error! {} is not found in {}".format(Setting, self.FilePath))
            raise
        finally:
            return ReadValue

    def ReadAllSettings(self):
        self.FspTopSize = "{:#08x}".format(self.ReadSingleSetting(self.FspTopSizeStr))
        print("FspTopSize:", self.FspTopSize)

def main():
    Parser = argparse.ArgumentParser(
                        prog = __prog__,\
                        description = __description__ + __copyright__
                        )
    Parser.add_argument("-SplitBin", dest = 'SplitBin', required = True,
                            help = 'Please input split tool file')
    Args, _  = Parser.parse_known_args()

    FspBinDir = os.getenv('WORKSPACE_FSP_BIN')
    Fsp_path = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "Fsp.fd")
    Fsp_Tmp_path = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "Fsp_Tmp.fd")
    FspT_path = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "Fsp_T.fd")
    FspO_path = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "Fsp_O.fd")
    FspTopAt4G_path = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "FspTopAt4G.fd")

    #
    # Split Fsp_O.fd from Fsp.fd
    #
    FspoAddress = GetLastFvBase(Fsp_path)
    Cmd = os.getenv('PYTHON_COMMAND')
    Cmd += " " + Args.SplitBin
    Cmd += " -f " + Fsp_path
    Cmd += " -s " + hex(FspoAddress)
    Cmd += " -t " + FspO_path
    Cmd += " -o " + Fsp_Tmp_path
    RunCommand(Cmd)

    #
    # Copy Fsp_T.fd from Fsp.fd, we need keep Fsp.fd for rebase operation
    #
    FsptAddress = GetLastFvBase(Fsp_Tmp_path)
    with open(Fsp_Tmp_path, 'rb') as source_file, open(FspT_path, 'wb') as destination_file:
        source_file.seek(FsptAddress)
        content = source_file.read()
        destination_file.write(content)

    #
    # Read flashmap to get FSP Top FV size
    #
    FlashMapFile = os.getenv('FLASHMAP_FDF')
    PrepRelease = os.getenv('PrepRelease')
    Fm = Flashmap(FlashMapFile, PrepRelease)
    Fm.ReadAllSettings()

    #
    # Generate FspTopAt4G.fd which include padding data, FSP-T and FSP-O
    #
    PadSize = int(Fm.FspTopSize, 16) - os.path.getsize(FspT_path) - os.path.getsize(FspO_path)
    with open(FspTopAt4G_path, "wb") as FileWrite:
        BinaryData = PadSize * struct.pack('B', 0xff)
        FileWrite.write(BinaryData)

        with open(FspT_path, "rb") as F:
            ReadData = F.read()
            FileWrite.write(ReadData)

        with open(FspO_path, "rb") as F:
            ReadData = F.read()
            FileWrite.write(ReadData)
    
    FreeSpaceSize = os.path.getsize(FspTopAt4G_path) - os.path.getsize(FspT_path) - os.path.getsize(FspO_path)
    if FreeSpaceSize < FBM_OFFSET_TO_FSPT:
        print("Error! There is not enough free space (at least 8KB)")
        exit(-1)

    if os.getenv('FSP_SIGNED') == 'TRUE':
        #
        # If FSP_SIGNED is True, Patch FBM to FSP Top binary.
        #
        fbmFilePath = os.path.join(os.getenv('WORKSPACE_ROOT'),
                                   'Build',
                                   os.getenv('FSP_PKG_NAME'),
                                   os.getenv('FSP_TARGET') + '_' + os.getenv('TOOL_CHAIN_TAG'),
                                   'FV',
                                   'Fbm.bin')
        with open(fbmFilePath, "rb") as FileRead:
            FbmBin = FileRead.read()

        with open(FspTopAt4G_path, "r+b") as Fw:
            Fw.seek(FreeSpaceSize - FBM_OFFSET_TO_FSPT, 1)
            Fw.write(FbmBin)
        print("Successfully patch Fbm Binary %d bytes to FSP Top." % len(FbmBin))

    #
    # Remove Temp files
    #
    os.remove(FspO_path)
    os.remove(FspT_path)
    print("Generate FspTopAt4G.fd success.")

if __name__ == '__main__':
    main()