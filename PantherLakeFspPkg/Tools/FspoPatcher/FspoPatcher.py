## @file
#  Patch crypto services in FspSecCoreO to reuse APIs provided in FspLoaderPeim.
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
# @par Specification
##


import struct
import sys
import argparse
import os
import uuid

#
# Globals for help information
#
__copyright__   = "INTEL CONFIDENTIAL \n Copyright (c) 2023, Intel Corporation. All rights reserved. "
__description__ = 'FspO binary patching tool.\n'
__prog__        = sys.argv[0]

EFI_FIRMWARE_FILE_SYSTEM2_GUID = uuid.UUID ('8C8CE578-8A3D-4F1C-9935-896185C32DD3')
EFI_FIRMWARE_FILE_FSPO_GUID    = uuid.UUID ('01530A64-BD0F-4CD8-80B1-5AC6B83EE3B8')
FSP_SEC_CORE_O_FFS_GUID        = uuid.UUID ('78C674DF-FC05-3302-29C2-143B7E0067AC')
FSP_LOADER_PEIM_FFS_GUID       = uuid.UUID ('E6934D25-5B09-4A06-9C7E-D3D8B4114B42')

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I', bytearray (A + B + C + D, 'ascii'))[0]

def Align (Address, Alignment):
    remainder = Address % Alignment
    if remainder == 0:
        return Address
    else:
        return Address + (Alignment - remainder)

class FspFirmwareVolume (object):
    _FirmwareVolumeStructFormat     = '<QQ16sQIIHHHBBII'
    _FirmwareVolumeStructSize       = struct.calcsize (_FirmwareVolumeStructFormat)
    _ExtFirmwareVolumeStructFormat  = '<16sI'
    _ExtFirmwareVolumeStructSize    = struct.calcsize (_ExtFirmwareVolumeStructFormat)

    _EFI_FVH_SIGNATURE              = _SIGNATURE_32 ('_', 'F', 'V', 'H')
    _EFI_FSPH_SIGNATURE             = _SIGNATURE_32 ('F', 'S', 'P', 'H')

    def __init__ (self, Buffer):
        if len (Buffer) < self._FirmwareVolumeStructSize:
            raise Exception ("Buffer size smaller than FV header!")

        (_, _, FileSystemGuid, FvLength, Signature, Attributes, HeaderLength, Checksum, ExtHeaderOffset, _, Revision, NumBlocks, Length) = \
            struct.unpack (
                    self._FirmwareVolumeStructFormat,
                    Buffer[:self._FirmwareVolumeStructSize]
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

        if self.Signature != self._EFI_FVH_SIGNATURE or self.ExtHeaderOffset == 0:
            raise Exception ("Invalid FV header!")

        (ExtFvGuid, ExtFvLength) = \
            struct.unpack (
                    self._ExtFirmwareVolumeStructFormat,
                    Buffer[self.ExtHeaderOffset:self.ExtHeaderOffset + self._ExtFirmwareVolumeStructSize]
                    )
        self.ExtFvGuid              = uuid.UUID (bytes_le=ExtFvGuid)
        self.ExtFvLength            = ExtFvLength

        if self.ExtFvLength != self._ExtFirmwareVolumeStructSize:
            raise Exception ("Invalid FV header!")

    def GetFirmwareVolumeSize (self) -> int:
        return self.FvLength

    def GetFirmwareVolumeHeaderSize (self) -> int:
        return self.ExtHeaderOffset + self._ExtFirmwareVolumeStructSize

    def CompareGuid (self, Guid) -> bool:
        return self.ExtFvGuid == Guid

class FirmwareFile (object):
    _FirmwareFileStructFormat     = '<16sHBB3BB'
    _FirmwareFileStructSize       = struct.calcsize (_FirmwareFileStructFormat)

    def __init__ (self, Buffer):
        (Name, IntegrityCheck, Type, Attributes, Size0, Size1, Size2, State) = \
            struct.unpack (self._FirmwareFileStructFormat, Buffer[:self._FirmwareFileStructSize])
        self.Name               = uuid.UUID (bytes_le=Name)
        self.IntegrityCheck     = IntegrityCheck
        self.Type               = Type
        self.Attributes         = Attributes
        self.Size               = Size0 + Size1 * 0x100 + Size2 * 0x10000
        self.State              = State

    def CompareGuid (self, Guid) -> bool:
        return self.Name == Guid

#
# search FspO 4KB align address in Fsp.fd by locating the last FSP Header
#
def SearchFspOLocation (FspFdFile) -> int:
    if not os.path.exists(FspFdFile):
        print(FspFdFile + " doesn't exist, fails to continue FspoFvGen.py")
        exit (-1)

    with open(FspFdFile, "rb") as FspFd:
        FspData = FspFd.read()
        FileSize = len(FspData)

        FvOffset = 0
        while (FvOffset < FileSize):
            FspFv = FspFirmwareVolume (FspData[FvOffset:])
            if (FspFv.CompareGuid (EFI_FIRMWARE_FILE_FSPO_GUID)):
                return FvOffset
            FvOffset += FspFv.GetFirmwareVolumeSize ();

    print("There is no FSPO in " + FspFdFile)
    exit (-1)

def FindAndPatchFSPO (Buffer):
    FspoFv = FspFirmwareVolume (Buffer)
    FfsOffset = Align (FspoFv.GetFirmwareVolumeHeaderSize (), 0x8)

    FspSecCoreO_Offset = 0
    FspSecCoreO_Size = 0
    FspLoaderPeim_Offset = 0
    FspLoaderPeim_Size = 0

    # Get FspLoaderPeim and FspSecCoreO FFS offset and size
    while FfsOffset < len (Buffer):
        ffs = FirmwareFile (Buffer[FfsOffset:])
        if ffs.CompareGuid (FSP_SEC_CORE_O_FFS_GUID):
            FspSecCoreO_Offset = FfsOffset
            FspSecCoreO_Size = ffs.Size
        elif ffs.CompareGuid (FSP_LOADER_PEIM_FFS_GUID):
            FspLoaderPeim_Offset = FfsOffset
            FspLoaderPeim_Size = ffs.Size
        FfsOffset += Align (ffs.Size, 8)
    #
    # Copy the ApiWrapper from FspLoaderPeim to FspSecCoreO
    # Structure is identified by FSP Verification API signature '__FVA__'
    #
    ApiWrapperSignature = b'__FVA__'
    ApiWrapperSize = struct.calcsize ('<4Q')
    FspLoaderPeim = Buffer[FspLoaderPeim_Offset : FspLoaderPeim_Offset + FspLoaderPeim_Size]
    index = FspLoaderPeim.find (ApiWrapperSignature)
    if index == -1:
        raise Exception ("Signature __FVA__ not found in FspLoaderPeim!")
    ApiWrapperBuffer = FspLoaderPeim[index : index + ApiWrapperSize]

    FspSecCoreO = Buffer[FspSecCoreO_Offset : FspSecCoreO_Offset + FspSecCoreO_Size]
    index = FspSecCoreO.find (ApiWrapperSignature)
    if index == -1:
        raise Exception ("Signature __FVA__ not found in FspSecCoreO")
    NewFSPO = Buffer[:FspSecCoreO_Offset + index] + ApiWrapperBuffer + Buffer[FspSecCoreO_Offset + index + ApiWrapperSize:]
    return NewFSPO

def main():
    Parser = argparse.ArgumentParser(
                        prog = __prog__,\
                        description = __description__ + __copyright__
                        )
    Parser.add_argument("-f", dest = 'Input', type = str, required = True, help = 'FSP FD file')
    Args = Parser.parse_args()

    #
    # Patch FspO.fd in Fsp.fd
    #
    FspoAddress = SearchFspOLocation(Args.Input)

    with open(Args.Input, "r+b") as FspFd:
        FspFd.seek (FspoAddress)
        FspoBuffer = FspFd.read ()
        NewFSPO = FindAndPatchFSPO (FspoBuffer)
        FspFd.seek (FspoAddress)
        FspFd.write (NewFSPO)

if __name__ == '__main__':
    main()