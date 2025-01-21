## @file
#  Script to replace specific FV within input image by given FV image.
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

import os
import sys
import copy
import uuid
import argparse
from typing import Any, List

from ctypes import ARRAY, POINTER
from ctypes import cast, pointer, sizeof
from ctypes import c_char, c_uint8, c_uint16, c_uint32, c_uint64
from ctypes import Structure, LittleEndianStructure

MAJOR_VER: int = 1
MINOR_VER: int = 0

__prog__       : str = os.path.basename (__file__)
__version__    : str = f'{__prog__} Version {MAJOR_VER}.{MINOR_VER}'
__copyright__  : str = 'Copyright (C) 2024 Intel Corporation. All rights reserved.\n'
__description__: str = 'Script to replace specific FV within input image.\n'

EFI_FVH_SIGNATURE: bytes = b'_FVH'

EFI_FVH_ZERO_VECTOR: uuid.UUID = \
    uuid.UUID ('00000000-0000-0000-0000-000000000000')

EFI_FFS_VER_2_GUID : uuid.UUID = \
    uuid.UUID ('8c8ce578-8a3d-4f1c-9935-896185c32dd3')

EFI_FFS_VER_3_GUID : uuid.UUID = \
    uuid.UUID ('5473C07A-3DCB-4dca-BD6F-1E9689E7349A')

#
# Exit code of this script.
#
EXIT_SUCCESS          : int = 0x00
EXIT_INPUT_FV_INVALID : int = 0x01
EXIT_PATCHED_FD_FAILED: int = 0x02

def Struct2Stream (Struct: Structure) -> bytes:
    """ Convert the ctype structure into bytes.

    Args:
        Struct (Structure):
            Input ctype Structure to be converted.

    Raises:
        TypeError:
            Struct is not part of ctype Structure.

    Returns:
        bytes:
            Content of the structure data in bytes.
    """
    Length: int = 0
    Ptr   : Any = None

    if not isinstance (Struct, Structure):
        raise TypeError (f'Struct shall be part of ctype Structure.')

    Length = sizeof (Struct)
    Ptr    = cast (pointer (Struct), POINTER (c_char * Length))

    return Ptr.contents.raw

def IsGuidMatched (Source: bytes, Target: uuid.UUID) -> bool:
    """ Check if the GUID value is matched.

    Args:
        Source (bytes):
            Source GUID buffer in bytes type.
        Target (uuid.UUID):
            Target GUID value in uuid.UUID type.

    Raises:
        TypeError:
            (1) Source is not bytes type.
            (2) Target is not uuid.UUID type.

    Returns:
        bool:
            False - GUID value is not matched.
            True  - GUID value is matched.
    """
    SourceGuid: uuid.UUID = None
    TargetGuid: uuid.UUID = None

    if not isinstance (Source, bytes):
        raise TypeError (f'Source shall be bytes type.')
    elif not isinstance (Target, uuid.UUID):
        raise TypeError (f'Target shall be uuid.UUID type.')

    SourceGuid = uuid.UUID (bytes_le = Source)
    TargetGuid = Target

    return (SourceGuid == TargetGuid)

class GUID (LittleEndianStructure):
    #
    # typedef struct {
    #   UINT32    Data1;
    #   UINT16    Data2;
    #   UINT16    Data3;
    #   UINT8     Data4[8];
    # } GUID;
    #
    _fields_ = [
        ('Guid1', c_uint32),
        ('Guid2', c_uint16),
        ('Guid3', c_uint16),
        ('Guid4', ARRAY (c_uint8, 8)),
        ]

class EFI_FIRMWARE_VOLUME_HEADER (LittleEndianStructure):
    #
    # typedef struct {
    #   UINT8                     ZeroVector[16];
    #   EFI_GUID                  FileSystemGuid;
    #   UINT64                    FvLength;
    #   UINT32                    Signature;
    #   EFI_FVB_ATTRIBUTES_2      Attributes;
    #   UINT16                    HeaderLength;
    #   UINT16                    Checksum;
    #   UINT16                    ExtHeaderOffset;
    #   UINT8                     Reserved[1];
    #   UINT8                     Revision;
    #   EFI_FV_BLOCK_MAP_ENTRY    BlockMap[1];
    # } EFI_FIRMWARE_VOLUME_HEADER;
    #
    _fields_ = [
        ('ZeroVector',      GUID),
        ('FileSystemGuid',  GUID),
        ('FvLength',        c_uint64),
        ('Signature',       ARRAY (c_char, 4)),
        ('Attributes',      c_uint32),
        ('HeaderLength',    c_uint16),
        ('Checksum',        c_uint16),
        ('ExtHeaderOffset', c_uint16),
        ('Reserved',        c_uint8),
        ('Revision',        c_uint8),
        ]

class EFI_FIRMWARE_VOLUME_EXT_HEADER (LittleEndianStructure):
    #
    # typedef struct {
    #   EFI_GUID  FvName;
    #   UINT32    ExtHeaderSize;
    # } EFI_FIRMWARE_VOLUME_EXT_HEADER;
    #
    _fields_ = [
        ('FvName',        GUID),
        ('ExtHeaderSize', c_uint32),
        ]

class FirmwareVolume (object):
    def __init__ (self, Offset: int, FvData: bytearray) -> None:
        """ Class to represent the firmware volume (FV).

        Args:
            Offset (int):
                Offset to this input firmware volume.
            FvData (bytearray):
                Input FV buffer in bytearray type.

        Raises:
            None.

        Returns:
            None.
        """
        self.FvHdr    = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (FvData, 0)
        self.FvData   = None
        self.FvOffset = None
        self.FvLength = None
        self.FvExtHdr = None
        self.FvName   = None

        self.FvValid: bool = self.__IsFvValid ()
        if not self.FvValid:
            return

        self.FvData   = FvData[0:self.FvHdr.FvLength]
        self.Offset   = Offset
        self.FvLength = self.FvHdr.FvLength

        #
        # Check ExtHeader
        #
        if self.FvHdr.ExtHeaderOffset > 0:
            self.FvExtHdr = EFI_FIRMWARE_VOLUME_EXT_HEADER.from_buffer (self.FvData, self.FvHdr.ExtHeaderOffset)
            self.FvName = self.FvExtHdr.FvName

    def __IsFvValid (self) -> bool:
        """ Check the validity of the FV.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                False - FV is invalid.
                True  - FV is valid.
        """
        # Check ZeroVector
        if not IsGuidMatched (Struct2Stream (self.FvHdr.ZeroVector), EFI_FVH_ZERO_VECTOR):
            return False

        # Check _FVH signature
        if self.FvHdr.Signature != EFI_FVH_SIGNATURE:
            return False

        # Check FileSystemGuid
        if (not IsGuidMatched (Struct2Stream (self.FvHdr.FileSystemGuid), EFI_FFS_VER_2_GUID)) and \
           (not IsGuidMatched (Struct2Stream (self.FvHdr.FileSystemGuid), EFI_FFS_VER_3_GUID)):
            return False

        return True

class FirmwareDevice (object):
    def __init__ (self, FdData: bytearray) -> None:
        """ Class to represent the firmware device (FD).

        Args:
            FdData (bytearray):
                Input FD buffer in bytearray type..

        Raises:
            None.

        Returns:
            None.
        """
        self.FdData    : bytearray            = FdData
        self.GuidFvList: List[FirmwareVolume] = list ()

        self.__ParseFd ()

    def __ParseFd (self) -> None:
        """ Parsing the FV inside the FD file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        Offset = 0
        FdSize = len (self.FdData)

        while Offset < FdSize:
            Fvh = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (self.FdData, Offset)

            #
            # Find _FVH signature first.
            #
            if Fvh.Signature != EFI_FVH_SIGNATURE:
                # Assume there might be gaps between one FV and the next.
                Offset += 1
                continue

            #
            # Further check if FV header is valid or not.
            #
            Fv = FirmwareVolume (Offset, self.FdData[Offset:(Offset+Fvh.FvLength)])
            if Fv.FvValid == False:
                Offset += 1
                continue

            #
            # Skip to scan child FV.
            #
            Offset += Fv.FvHdr.FvLength

            #
            # Only append FV which has ExtHeader/FvName to list.
            #
            if Fv.FvName != None:
                self.GuidFvList.append (Fv)

def _GetPatchingFvInfo (FvBuffer: bytearray) -> FirmwareVolume:
    """ Return the patching FV information.

    Args:
        FvBuffer (bytearray):
            Input FV buffer in bytearray type.

    Raises:
        None.

    Returns:
        FirmwareVolume:
            Patching FV information in FirmwareVolume type.
            (If the FV is invalid, return it as None.)
    """
    PatchingFv : FirmwareVolume = FirmwareVolume (0, FvBuffer)
    FvFileSize : int            = len (FvBuffer)

    print ('Checking patching FV...')

    if PatchingFv.FvValid != True:
        print ('Not a valid FV')
        return None

    if PatchingFv.FvLength != FvFileSize:
        print ('Not a single FV, FV length should be the same to file size')
        return None

    if PatchingFv.FvName == None:
        print ('This FV does not have ExtHeader that contains FvName')
        return None

    print ('Patching FV:')
    print (f'FvLength = {hex (FvFileSize)}')
    print (f'FvName   = {uuid.UUID (bytes_le = Struct2Stream (PatchingFv.FvName))}')

    return PatchingFv

def _GetPatchedFd (
    FdBuffer: bytearray,
    FvBuffer: bytearray,
    FvInfo  : FirmwareVolume,
    ) -> bytearray:
    """ Return the patched FD file with assigned FV.

    Args:
        FdBuffer (bytearray):
            Input FD buffer in bytearray type.
        FvBuffer (bytearray):
            Input FV buffer in bytearray type.
        FvInfo (FirmwareVolume):
            Input FV information in FirmwareVolume type.

    Raises:
        None.

    Returns:
        bytearray:
            Patched FD buffer in bytearray type.
            (If patched failed, return it as None.)
    """
    SourceFdBuffer: bytearray      = FdBuffer
    SourceFdInfo  : FirmwareDevice = FirmwareDevice (SourceFdBuffer)
    TargetFvBuffer: bytearray      = copy.deepcopy (FvBuffer)
    TargetFvName  : uuid.UUID      = uuid.UUID (bytes_le = Struct2Stream (FvInfo.FvName))
    TargetFd      : bytearray      = None

    for GuidFv in SourceFdInfo.GuidFvList:
        if not IsGuidMatched (Struct2Stream (GuidFv.FvName), TargetFvName):
            continue

        if TargetFd is None:
            TargetFd = copy.deepcopy (SourceFdBuffer)

        print (f'Start patching FV at offset {hex (GuidFv.Offset)} with length {hex (GuidFv.FvLength)}')
        TargetFd[GuidFv.Offset:(GuidFv.Offset+GuidFv.FvLength)] = TargetFvBuffer

    return TargetFd

def Main () -> int:
    """ Main function to replace the specific FV within input image.

    Args:
        None.

    Raises:
        None.

    Returns:
        int:
            Exit code of this script.
                EXIT_SUCCESS:
                    Script is executed successfully.
                EXIT_INPUT_FV_INVALID:
                    Input FV is invalid.
                EXIT_PATCHED_FD_FAILED
                    Failed to patch the FV into input FD file.
    """
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
        Parser: argparse.ArgumentParser = \
            argparse.ArgumentParser (
                       prog            = __prog__,
                       description     = __copyright__ + __description__ + __version__,
                       formatter_class = argparse.RawTextHelpFormatter
                       )

        Parser.add_argument (
                 '-I',
                 '-i',
                 '--input',
                 dest     = 'Input',
                 type     = argparse.FileType ('rb'),
                 required = True,
                 help     = 'Input image file (BIOS) to be patched by input FV file.',
                 )
        Parser.add_argument (
                 '-O',
                 '-o',
                 '--output',
                 dest     = 'Output',
                 type     = str,
                 required = True,
                 help     = 'Output image file name of FV patched image',
                 )
        Parser.add_argument (
                 '-F',
                 '-f',
                 '--fv',
                 dest     = 'FvFile',
                 type     = argparse.FileType ('rb'),
                 required = True,
                 help     = 'The FV file (single FV) patch',
                 )

        return Parser.parse_args()

    Args          : argparse.Namespace = GetArgs ()
    SourceFdImage : bytearray          = bytearray (Args.Input.read ())
    SourceFvImage : bytearray          = bytearray (Args.FvFile.read ())
    SourceFvInfo  : FirmwareVolume     = None
    TargetFdImage : bytearray          = None
    OutputFilePath: str                = os.path.realpath (Args.Output)

    #
    # 1. Close the input FV and FD file to release file access.
    #
    Args.Input.close ()
    Args.FvFile.close ()

    #
    # 2. Extract the FV information.
    #
    SourceFvInfo = _GetPatchingFvInfo (FvBuffer = SourceFvImage)
    if SourceFvInfo is None:
        return EXIT_INPUT_FV_INVALID

    #
    # 3. Patch the FV into FD file.
    #
    TargetFdImage = _GetPatchedFd (
                        FdBuffer = SourceFdImage,
                        FvBuffer = SourceFvImage,
                        FvInfo   = SourceFvInfo,
                        )
    if TargetFdImage is None:
        print (f'Cannot find match FV in {Args.Input.name} to patch!')
        return EXIT_PATCHED_FD_FAILED

    #
    # 3. Export the patched FD file.
    #
    File = open (OutputFilePath, "wb")
    File.write (TargetFdImage)
    File.close ()
    print (f'Output patched image in {OutputFilePath}')

    return EXIT_SUCCESS

if __name__ == '__main__':
    sys.exit (Main ())
