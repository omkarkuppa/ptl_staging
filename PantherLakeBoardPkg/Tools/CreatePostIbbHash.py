## @file
#  This script is called to auto generate PostIbbDigest.hash
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
import struct
import sys
import argparse
import uuid
import subprocess
import shutil

#
# Globals for help information
#
__prog__        = os.path.basename (__file__)
__copyright__   = 'Copyright (c) 2023, Intel Corporation. All rights reserved.'
__description__ = '\n'

SHA256_DIGEST_SIZE = 0x20
SHA384_DIGEST_SIZE = 0x30
ALIGNMENT_4K       = 0x1000

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I', bytearray (A + B + C + D, 'ascii'))[0]

def _SIGNATURE_32_TO_STRING (Signature):
    return struct.pack ("<I", Signature).decode ()

class FirmwareVolumeHeaderClass (object):
    _StructFormat                   = '<QQ16sQIIHHHBBII'
    _StructSize                     = struct.calcsize (_StructFormat)

    _EFI_FVH_SIGNATURE              = _SIGNATURE_32 ('_', 'F', 'V', 'H')
    _EFI_FIRMWARE_FILE_SYSTEM2_GUID = uuid.UUID ('8C8CE578-8A3D-4F1C-9935-896185C32DD3')

    def __init__ (self):
        self.FileSystemGuid         = uuid.UUID ('00000000-0000-0000-0000-000000000000')
        self.FvLength               = 0x0000000000000000
        self.Signature              = self._EFI_FVH_SIGNATURE
        self.Attributes             = 0x00000000
        self.HeaderLength           = 0x0000
        self.Checksum               = 0x0000
        self.ExtHeaderOffset        = 0x0000
        self.Revision               = 0x00

    def Decode (self, Buffer):
        if len (Buffer) < self._StructSize:
            raise Exception ("Buffer size smaller than header!")

        (_, _, FileSystemGuid, FvLength, Signature, Attributes, HeaderLength, Checksum, ExtHeaderOffset, _, Revision, NumBlocks, Length) = \
            struct.unpack (
                     self._StructFormat,
                     Buffer[:self._StructSize]
                     )

        self.FileSystemGuid         = uuid.UUID (bytes_le=FileSystemGuid)
        self.FvLength               = FvLength
        self.Signature              = Signature
        self.Attributes             = Attributes
        self.HeaderLength           = HeaderLength
        self.Checksum               = Checksum
        self.ExtHeaderOffset        = ExtHeaderOffset
        self.Revision               = Revision
        self.NumBlocks              = NumBlocks
        self.Length                 = Length

        try:
            assert (self.Signature == self._EFI_FVH_SIGNATURE)
        except AssertionError:
            raise Exception ("No FV header!")

def CreateDigest (Payload, PayloadDigest, ToolPath, Verbose = False):
    CmdList = list ()
    Command = str ()

    #
    # Build signtool command
    #
    if ToolPath is None:
        ToolPath = ''

    if not os.path.exists (os.path.dirname (PayloadDigest)) :
        os.mkdir (os.path.dirname (PayloadDigest))

    SignTool = os.path.join (ToolPath, 'openssl')

    CmdList.extend ([SignTool, 'dgst', '-binary', '-sha384', '-out', PayloadDigest, Payload])

    Command = ' '.join (CmdList)

    if Verbose:
        print (Command)

    #
    # Create digest
    #
    try:
        Process = subprocess.Popen (Command, stdin = subprocess.PIPE, stdout = subprocess.PIPE, stderr = subprocess.PIPE, shell = True)
        Result = Process.communicate ('')
    except:
        raise ValueError (f'{__prog__}: error: can not run OpenSSL tool.')

    if Process.returncode != 0:
        print (Result[1].decode ())
        raise ValueError (f'{__prog__}: error: OpenSSL tool failed.')

def CreateDummyDigest (OutputFileName):
    DummySha384Hash = b'\xFF' * SHA384_DIGEST_SIZE
    with open (OutputFileName, 'wb') as F:
        F.write (DummySha384Hash)

def GetAddressFromFlashmap (FileTxt, SearchTxt):
    for line in FileTxt:
        if (line.find (SearchTxt) != -1):
            # Text line should be like this:
            # SET gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSOffset   = 0x00A60000  # Flash addr (0xFFC60000)
            # address value is at index 3 when this line is split
            address = int (line.split () [3], 16)
            return address
    raise Exception (f'The {SearchTxt} does not exist in Flashmap')

if __name__ == "__main__":

    def ValidateFvAddress (Argument):
        try:
            Value = int (Argument, 0)
        except:
            Message = f'{Argument} is not a valid integer value.'
            raise argparse.ArgumentTypeError (Message)
        if Value % (ALIGNMENT_4K) != 0:
            Message = f'{Argument} must be a multiple of 4 KB'
            raise argparse.ArgumentTypeError (Message)
        return Value

    #
    # Create command line argument parser object
    #
    parser = argparse.ArgumentParser (
                        prog = __prog__,
                        description = __description__ + __copyright__,
                        )

    parser.add_argument ("-i", "--input", dest = 'InputFile', type = argparse.FileType ('rb'), required = True,
                            help = "Input binary filename.")
    parser.add_argument ("-o", "--output", dest = 'OutputFile', type = str, required = True,
                            help = "Output filename")
    parser.add_argument ("--fdf", dest = 'FlashmapFile', type = argparse.FileType ('r'), required = True,
                            help = "Parse this Flashmap to get FV address and size")
    parser.add_argument ("--fv", dest = 'FvName', type = str, action='append',
                            help = "FvAddress")
    parser.add_argument ("-t", "--temp", dest = 'TempFolder', type = str, required = True,
                            help = "Temp Folder to place generated hash file")
    parser.add_argument ("--openssl-tool-path", dest = 'OpensslToolPath', type = str,
                         help = "Path to OpenSSL tool. Optional if path to tools are already in PATH.")

    #
    # Parse command line arguments
    #
    args = parser.parse_args ()

    File            = args.InputFile
    OutputFilePath  = os.path.realpath (args.OutputFile)
    FlashmapFile    = args.FlashmapFile
    FvNameList      = args.FvName
    TempFolder      = os.path.realpath (args.TempFolder)
    OpenSSLToolPath = os.path.realpath (args.OpensslToolPath)

    #
    # Read all binary input files
    #
    try:
        Buffer = File.read ()
        File.close ()
    except:
        raise Exception (f'CreatePostIbbHash: error: can not read binary input file {File} !')

    FlashmapLines = []
    try:
        for line in FlashmapFile.readlines ():
            line = line.strip ()
            FlashmapLines.append (line)
    except:
        raise Exception (f'CreatePostIbbHash: error: can not read Flashmap file {FlashmapFile} !')

    if not os.path.exists (TempFolder):
        os.makedirs (TempFolder)
    POST_IBB_HASH_TMP_FOLDER = TempFolder

    if not os.path.exists (OpenSSLToolPath):
        raise Exception (f'CreatePostIbbHash: error: {OpenSSLToolPath} does not exist!')
    OPENSSL_FOLDER = OpenSSLToolPath

    #
    # Get FV address from Flashmap
    #
    FvAddressList = []
    for FvName in FvNameList:
        try:
            FvAddress = GetAddressFromFlashmap (FlashmapLines, FvName)
            #
            # Strip the external address in extended BIOS
            #
            FvAddress = FvAddress & 0xFFFFFF
            FvAddressList.append (FvAddress)
        except Exception as err:
            print (f'Error: {err}, create dummy SHA384 hash file!')
            CreateDummyDigest (OutputFilePath)
            shutil.rmtree (POST_IBB_HASH_TMP_FOLDER, ignore_errors=True)
            sys.exit (1)

    #
    # Generate digest for each FV
    #
    FvDigestPathList = []
    for address in FvAddressList:
        try:
            FirmwareVolumeHeader = FirmwareVolumeHeaderClass ()
            FirmwareVolumeHeader.Decode (Buffer[address:])
            FvSize = FirmwareVolumeHeader.FvLength
        except Exception as err:
            CreateDummyDigest (OutputFilePath)
            print (f"Error: {err}, create dummy SHA384 hash file!")
            shutil.rmtree (POST_IBB_HASH_TMP_FOLDER, ignore_errors=True)
            sys.exit (1)

        FvName       = "FV_" + hex (address) + ".bin"
        FvDigestName = "FV_" + hex (address) + "_Digest" + ".bin"
        FvPath       = os.path.join (POST_IBB_HASH_TMP_FOLDER, FvName)
        FvDigestPath = os.path.join (POST_IBB_HASH_TMP_FOLDER, FvDigestName)
        FvDigestPathList.append (FvDigestPath)

        with open (FvPath, 'wb') as F:
            FvBuffer = Buffer[address:address + FvSize]
            F.write (FvBuffer)

        CreateDigest (FvPath, FvDigestPath, OPENSSL_FOLDER, True)

    #
    # Combine digests for each FV together
    #
    DigestBuffer = bytes ()
    for FvDigest in FvDigestPathList:
        with open (FvDigest, 'rb') as F:
            DigestBuffer += F.read ()

    #
    # Generate the digest of the combined digest
    #
    PostIbbPath       = os.path.join (POST_IBB_HASH_TMP_FOLDER, "PostIbb.bin")
    PostIbbDigestPath = OutputFilePath
    with open (PostIbbPath, 'wb') as F:
        F.write (DigestBuffer)
    CreateDigest (PostIbbPath, PostIbbDigestPath, OPENSSL_FOLDER, True)

    shutil.rmtree (POST_IBB_HASH_TMP_FOLDER, ignore_errors=True)

    sys.exit (0)
