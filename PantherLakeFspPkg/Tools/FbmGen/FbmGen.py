## @file
#  Gen FBM (FSP Boot Manifest) tool.
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

import argparse
import configparser
import datetime
import os
import struct
import subprocess
import sys
from ctypes import *
from functools import reduce
import copy

#
# Globals for help information
#
__copyright__   = "INTEL CONFIDENTIAL \n Copyright (c) 2023, Intel Corporation. All rights reserved. "
__description__ = 'Fbm gen Tool.\n'
__prog__        = sys.argv[0]

#
# Below fields are set for FBM Signing
# ModuleType (4B): 0x14
# PublicKey Size (384 Bytes - RSA): Should be filled by LT-CSS
# HeaderVersion (4B): 0x00010000
# LT-CSS flow requires the minimal size to be 786 bytes for each data to sign.
#
EXPONENT_SIZE            : int = 0x4
SIGN_MODULE_TYPE         : int = 0x14
SIGN_DATA_HEADER_VERSION : int = 0x10000
SIGN_DATA_HEADER_LEN     : int = 0xC4
SIGN_BIT_SIZE            : int = 3072
PUB_KEY_SIZE             : int = 384
MINIMAL_TO_SIGN_DATA_SIZE: int = 786

#
# Currently FSP region include FSP-M and FSP-S
# The OEM Config space of FSP-O is [4G-4K, 4G - 16] in 4G space
#
FSP_REGIONS_NUM_EXCEPT_FSPT_AND_O: int = 0x2
FSPO_OEM_CONFIG_REGION_UPPER_ADDR: int = 0x10
FSPO_OEM_CONFIG_REGION_LOWER_ADDR: int = 0x1000

#
# Digest list calculated by the first FSP component which must contain FSP-O and FSP-T,
# follows a COMPONENT_HASH_STRUCTURE format structure. These digests are for ACM verify.
# Below hash algorithms must be supported for TPM measurement use
#
HASH_TYPE_VALUE = {
    "sha384": 0xC,
    "sha1"  : 0x4,
    "sha256": 0xB,
    "sha512": 0xD,
    "sm3"   : 0x12,
    }

HASH_DIGEST_SIZE = {
    "sha1"  : 20,
    "sha256": 32,
    "sha384": 48,
    "sha512": 64,
    "sm3"   : 32,
    }

FBM_COMPONENT_ID = {
    "TO": 0x0,
    "M" : 0X1,
    "S" : 0x2,
    }

def AlignPtr (offset, alignment = 8) -> int:
    return (offset + alignment - 1) & ~(alignment - 1)

def Bytes2Val (bytes):
    return reduce(lambda x,y: (x<<8)|y,  bytes[::-1] )

def Val2Bytes (value, blen):
    return [(value>>(i*8) & 0xff) for i in range(blen)]

def ConvertToImageAddress(imageSize : int, address : int) -> int:
    ADDRESS_4G = 0x100000000
    return imageSize - (ADDRESS_4G - address)

def RemoveFile(filePath) -> None:
    try:
        os.remove(filePath)
    except OSError as e:
        print("Error: fail to remove file: " + filePath)

def HexDump(buffer, spaceNumber) -> None:
    binStr = ''
    cnt = 0
    for b in buffer:
        binStr = binStr + " " + hex(b)[2:].zfill(2)
        cnt += 1
        if cnt % 16 == 0:
            print(' ' * spaceNumber + hex(cnt - 16)[2:].zfill(4) + ":" + binStr)
            binStr = ""
    if cnt % 16 > 0:
        print(' ' * spaceNumber + hex((cnt - cnt % 16))[2:].zfill(4) + ":" + binStr)

class EFI_FIRMWARE_VOLUME_HEADER(Structure):
    _fields_ = [
        ('ZeroVector',      ARRAY(c_uint8, 16)),
        ('FileSystemGuid',  ARRAY(c_uint8, 16)),
        ('FvLength',        c_uint64),
        ('Signature',       ARRAY(c_char, 4)),
        ('Attributes',      c_uint32),
        ('HeaderLength',    c_uint16),
        ('Checksum',        c_uint16),
        ('ExtHeaderOffset', c_uint16),
        ('Reserved',        c_uint8),
        ('Revision',        c_uint8)
        ]

class GUID(Structure):
    _fields_ = [
        ('Guid1',           c_uint32),
        ('Guid2',           c_uint16),
        ('Guid3',           c_uint16),
        ('Guid4',           ARRAY(c_uint8, 8)),
        ]

class EFI_FIRMWARE_VOLUME_EXT_HEADER(Structure):
    _fields_ = [
        ('FvName',          GUID),
        ('ExtHeaderSize',   c_uint32),
        ]

class EFI_FFS_INTEGRITY_CHECK(Structure):
    _fields_ = [
        ('Header',          c_uint8),
        ('File',            c_uint8)
        ]

class c_uint24(Structure):
    """
    Little-Endian 24-bit Unsigned Integer
    """
    _pack_   = 1
    _fields_ = [('Data', (c_uint8 * 3))]

    def __init__(self, val=0):
        self.set_value(val)

    def __str__(self, indent=0):
        return '0x%.6x' % self.value

    def __int__(self):
        return self.get_value()

    def set_value(self, val):
        self.Data[0:3] = Val2Bytes(val, 3)

    def get_value(self):
        return Bytes2Val(self.Data[0:3])

    value = property(get_value, set_value)

class EFI_FFS_FILE_HEADER(Structure):
    _fields_ = [
        ('Name',                      ARRAY(c_uint8, 16)),
        ('IntegrityCheck',            EFI_FFS_INTEGRITY_CHECK),
        ('Type',                      c_uint8),
        ('Attributes',                c_uint8),
        ('Size',                      c_uint24),
        ('State',                     c_uint8)
        ]

class EFI_COMMON_SECTION_HEADER(Structure):
    _fields_ = [
        ('Size',                      c_uint24),
        ('Type',                      c_uint8)
        ]

class FSP_INFORMATION_HEADER(Structure):
    _fields_ = [
        ('Signature',                 ARRAY(c_char, 4)),
        ('HeaderLength',              c_uint32),
        ('Reserved1',                 c_uint16),
        ('SpecVersion',               c_uint8),
        ('HeaderRevision',            c_uint8),
        ('ImageRevision',             c_uint32),
        ('ImageId',                   ARRAY(c_char, 8)),
        ('ImageSize',                 c_uint32),
        ('ImageBase',                 c_uint32),
        ('ImageAttribute',            c_uint16),
        ('ComponentAttribute',        c_uint16),
        ('CfgRegionOffset',           c_uint32),
        ('CfgRegionSize',             c_uint32),
        ('Reserved2',                 c_uint32),
        ('TempRamInitEntryOffset',    c_uint32),
        ('Reserved3',                 c_uint32),
        ('NotifyPhaseEntryOffset',    c_uint32),
        ('FspMemoryInitEntryOffset',  c_uint32),
        ('TempRamExitEntryOffset',    c_uint32),
        ('FspSiliconInitEntryOffset', c_uint32)
    ]

class FSP_COMMON_HEADER(Structure):
    _fields_ = [
        ('Signature',                 ARRAY(c_char, 4)),
        ('HeaderLength',              c_uint32)
        ]

class FSP_PATCH_TABLE(Structure):
    _fields_ = [
        ('Signature',                 ARRAY(c_char, 4)),
        ('HeaderLength',              c_uint16),
        ('HeaderRevision',            c_uint8),
        ('Reserved',                  c_uint8),
        ('PatchEntryNum',             c_uint32)
        ]

class FIRMWARE_INTERFACE_TABLE_ENTRY(Structure):
    _pack_ = 1
    _fields_ = [
        ("Address",     c_uint64),
        ("Size",        c_uint24),
        ("Reserved",    c_uint8),
        ("Version",     c_uint16),
        ("Type",        c_uint8, 7),
        ("C_V",         c_uint8, 1),
        ("Chksum",      c_uint8)
    ]

class HashStructHead(Structure):
    _pack_ = 1
    _fields_ = [
        ('HashAlg',                   c_uint16),
        ('Size',                      c_uint16),
        ]

class SignDataHeader(Structure):
    _pack_ = 1
    _fields_ = [
        ('ModuleType',                c_uint32),
        ('HeaderLen',                 c_uint32),
        ('HeaderVersion',             c_uint32),
        ('PublicKey',                 c_uint8 * PUB_KEY_SIZE),
        ('Exponent',                  c_uint32),
        ('Signature',                 c_uint8 * PUB_KEY_SIZE),
        ]

class FspRegionDigestHeader(Structure):
    _pack_ = 1
    _fields_ = [
        ('FspComponentId',            c_uint8),
        ('Size',                      c_uint16),
        ('Count',                     c_uint16)
    ]

class FirmwareFile:
    def __init__(self, offset, filedata):
        self.FfsHdr   = EFI_FFS_FILE_HEADER.from_buffer (filedata, 0)
        self.FfsData  = filedata[0:int(self.FfsHdr.Size)]
        self.Offset   = offset
        self.SecList  = []

    def ParseFfs(self):
        ffssize = len(self.FfsData)
        offset  = sizeof(self.FfsHdr)
        if self.FfsHdr.Name != '\xff' * 16:
            while offset < ffssize:
                sechdr = EFI_COMMON_SECTION_HEADER.from_buffer (self.FfsData, offset)
                sec = Section (offset, self.FfsData[offset:offset + int(sechdr.Size)])
                self.SecList.append(sec)
                offset += int(sechdr.Size)
                offset = AlignPtr(offset, 4)

class Section:
    def __init__(self, offset, secdata):
        self.SecHdr  = EFI_COMMON_SECTION_HEADER.from_buffer (secdata, 0)
        self.SecData = secdata[0:int(self.SecHdr.Size)]
        self.Offset  = offset

class FirmwareVolume:
    def __init__(self, Offset, FvData):
        self.FvHdr  = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (FvData, 0)
        self.FvData = FvData[0 : self.FvHdr.FvLength]
        self.Offset = Offset
        if self.FvHdr.ExtHeaderOffset > 0:
            self.FvExtHdr = EFI_FIRMWARE_VOLUME_EXT_HEADER.from_buffer (self.FvData, self.FvHdr.ExtHeaderOffset)
        else:
            self.FvExtHdr = None
        self.FfsList = []

    def ParseFv(self):
        fvsize = len(self.FvData)
        if self.FvExtHdr:
            offset = self.FvHdr.ExtHeaderOffset + self.FvExtHdr.ExtHeaderSize
        else:
            offset = self.FvHdr.HeaderLength
        offset = AlignPtr(offset)
        while offset < fvsize:
            ffshdr = EFI_FFS_FILE_HEADER.from_buffer (self.FvData, offset)
            if (ffshdr.Name == '\xff' * 16) and (int(ffshdr.Size) == 0xFFFFFF):
                offset = fvsize
            else:
                ffs = FirmwareFile (offset, self.FvData[offset:offset + int(ffshdr.Size)])
                ffs.ParseFfs()
                self.FfsList.append(ffs)
                offset += int(ffshdr.Size)
                offset = AlignPtr(offset)

class EFI_SECTION_TYPE():
    """Enumeration of all valid firmware file section types."""
    ALL                        = 0x00
    COMPRESSION                = 0x01
    GUID_DEFINED               = 0x02
    DISPOSABLE                 = 0x03
    PE32                       = 0x10
    PIC                        = 0x11
    TE                         = 0x12
    DXE_DEPEX                  = 0x13
    VERSION                    = 0x14
    USER_INTERFACE             = 0x15
    COMPATIBILITY16            = 0x16
    FIRMWARE_VOLUME_IMAGE      = 0x17
    FREEFORM_SUBTYPE_GUID      = 0x18
    RAW                        = 0x19
    PEI_DEPEX                  = 0x1b
    SMM_DEPEX                  = 0x1c

class FspImage:
    def __init__(self, offset, fih, fihoff, patch):
        self.Fih       = fih
        self.FihOffset = fihoff
        self.Offset    = offset
        self.FvIdxList = []
        self.Type      = "XTMSXXXXOXXXXXXX"[(fih.ComponentAttribute >> 12) & 0x0F]
        self.PatchList = patch
        self.PatchList.append(fihoff + 0x1C)

    def AppendFv(self, FvIdx):
        self.FvIdxList.append(FvIdx)

class FirmwareDevice:
    def __init__(self, fdfile):
        self.FvList  = []
        self.FspList = []
        self.FdFile  = fdfile
        self.Offset  = 0
        with open (self.FdFile, 'rb') as hfsp:
            self.FdData = bytearray(hfsp.read())

    def ParseFd(self):
        offset = 0
        fdsize = len(self.FdData)
        self.FvList  = []
        while offset < fdsize:
            fvh = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (self.FdData, offset)
            if b'_FVH' != fvh.Signature:
                raise Exception("ERROR: Invalid FV header !")
            fv = FirmwareVolume (offset, self.FdData[offset:offset + fvh.FvLength])
            fv.ParseFv ()
            self.FvList.append(fv)
            offset += fv.FvHdr.FvLength

    def CheckFsp (self):
        if len(self.FspList) == 0:
            return

        fih = None
        for fsp in self.FspList:
            if fsp.Fih.HeaderRevision < 3:
                raise Exception("ERROR: FSP 1.x is not supported by this tool !")
            if not fih:
                fih = fsp.Fih
            else:
                newfih = fsp.Fih
                if (newfih.ImageId != fih.ImageId) or (newfih.ImageRevision != fih.ImageRevision):
                    raise Exception("ERROR: Inconsistent FSP ImageId or ImageRevision detected !")

    def ParseFsp(self):
        flen = 0
        cnt  = 0
        for idx, fv in enumerate(self.FvList):
            # Check if this FV contains FSP header
            if flen == 0:
                if len(fv.FfsList) == 0:
                    continue
                ffs = fv.FfsList[0]
                if len(ffs.SecList) == 0:
                    continue
                sec = ffs.SecList[0]
                if sec.SecHdr.Type != EFI_SECTION_TYPE.RAW:
                    continue
                fihoffset = ffs.Offset + sec.Offset + sizeof(sec.SecHdr)
                fspoffset = fv.Offset
                offset    = fspoffset + fihoffset
                fih = FSP_INFORMATION_HEADER.from_buffer (self.FdData, offset)
                cnt += 1
                if b'FSPH' != fih.Signature:
                    continue

                offset += fih.HeaderLength
                offset = AlignPtr(offset, 4)
                plist  = []
                while True:
                    fch = FSP_COMMON_HEADER.from_buffer (self.FdData, offset)
                    if b'FSPP' != fch.Signature:
                        offset += fch.HeaderLength
                        offset = AlignPtr(offset, 4)
                    else:
                        fspp = FSP_PATCH_TABLE.from_buffer (self.FdData, offset)
                        offset += sizeof(fspp)
                        pdata  = (c_uint32 * fspp.PatchEntryNum).from_buffer(self.FdData, offset)
                        plist  = list(pdata)
                        break

                fsp  = FspImage (fspoffset, fih, fihoffset, plist)
                fsp.AppendFv (idx)
                self.FspList.append(fsp)
                flen = fsp.Fih.ImageSize - fv.FvHdr.FvLength
            else:
                fsp.AppendFv (idx)
                print("2idx = ", idx, "flen", flen)
                flen -= fv.FvHdr.FvLength
                if flen < 0:
                    raise Exception("ERROR: Incorrect FV size in image !")
        self.CheckFsp ()

class HashStruct:
    def __init__(self, HashAlgType, OpenSslCommand, fspData) -> None:
        self.HashStructHead = HashStructHead()
        self.HashStructHead.HashAlg = HASH_TYPE_VALUE[HashAlgType]
        self.HashStructHead.Size = HASH_DIGEST_SIZE[HashAlgType]

        command = '%s dgst -%s -binary' % (OpenSslCommand, HashAlgType)
        Process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, \
                                   stderr=subprocess.PIPE, shell=True)
        res = bytearray()
        res = res + Process.communicate(input=fspData)[0]
        self.HashStructBuffer = res

    def GetOverallLength(self):
        return sizeof(self.HashStructHead) + self.HashStructHead.Size

    def SerializeToBin(self) -> bytes:
        hashStructBin = bytes(self.HashStructHead) + self.HashStructBuffer
        return hashStructBin

def DumpHashStruct(fbmBin, offset, spaceNumber) -> int:
    hashHead = HashStructHead.from_buffer(fbmBin, offset)
    hashAlgStr = ""
    for key in HASH_TYPE_VALUE:
        if HASH_TYPE_VALUE[key] == hashHead.HashAlg:
            hashAlgStr = key
            break

    print(' ' * spaceNumber + "Hash Algorithm:", hashAlgStr, " Algorithm ID:", hex(hashHead.HashAlg))
    print(' ' * spaceNumber + "Hash Size:", hex(hashHead.Size))
    hashValueBuffer = fbmBin[offset + sizeof(HashStructHead):offset + sizeof(HashStructHead) + hashHead.Size]
    print(' ' * spaceNumber + "Digest Content: ")
    HexDump(hashValueBuffer, spaceNumber)
    return offset + sizeof(HashStructHead) + hashHead.Size

def CovertFbmCompIDToStr(id) -> str:
    fspType = ''
    for key in FBM_COMPONENT_ID:
        if FBM_COMPONENT_ID[key] == id:
            fspType = key
            break
    return fspType

def DumpFspRegionDigest(fbmBin, offset, spaceNumber) -> int:
    header = FspRegionDigestHeader.from_buffer(fbmBin, offset)
    fspType = CovertFbmCompIDToStr(header.FspComponentId)
    print(' ' * spaceNumber + "Fsp Component Id:", header.FspComponentId, "(FSP-%s)" % fspType)
    print(' ' * spaceNumber + "Size:", hex(header.Size))
    print(' ' * spaceNumber + "Count:", hex(header.Count))
    offset += sizeof(FspRegionDigestHeader)

    for i in range(header.Count):
        print(' ' * spaceNumber + "Digest List[%d]:" % i)
        offset = DumpHashStruct(fbmBin, offset, spaceNumber + 4)

    return offset

class FspRegionDigest:
    def __init__(self, fspFdDir, OpenSslCommand):
        """
        Initialize the FspRegionDigest object.

        Args:
            fspFdDir (str): The directory of the FSP firmware image file.
            OpenSslCommand (str): The command for running OpenSSL.

        """
        self.digestList = []
        self.fspFdDir = fspFdDir
        self.OpenSslCommand = OpenSslCommand
        self.header = FspRegionDigestHeader()

    def CalFspTAndODigestList(self, fspT, fspO, excludeUpd) -> None:
        """
        Calculate the digest list for FSP-T and FSP-O.

        The digest calculation includes two regions:
        - The first region consists of the entire FSP-T and the region before the OEM config data within FSP-O.
        - The second region includes the region within FSP-O after the OEM config data.

        Args:
            fspT (FspObject): The FSP-T object.
            fspO (FspObject): The FSP-O object.
            excludeUpd (bool): Flag indicating whether to exclude UPD region for calculation.

        """
        self.header.FspComponentId = FBM_COMPONENT_ID["TO"]
        FspTandOData = b''

        fspOSize   = fspO.Fih.ImageSize
        fspOOffset = fspO.Offset
        fspTSize   = fspT.Fih.ImageSize
        fspTOffset = fspT.Offset

        with open(self.fspFdDir, "rb") as fr:
            fr.seek(fspTOffset, 0)

            # Read the whole FSP-T
            FspTandOData += fr.read(fspTSize)

            #
            # Get FSP-O data except the region from (4G-4K)B to (4G-16)B
            # Read FSP-O data from the start of FSP-O to 4G-4KB
            #
            chunkSize = fspOSize - FSPO_OEM_CONFIG_REGION_LOWER_ADDR
            fr.seek(fspOOffset, 0)
            FspTandOData += fr.read(chunkSize)

            # Read the last 16B data of FSP-O
            chunkSize = FSPO_OEM_CONFIG_REGION_UPPER_ADDR
            fr.seek(fspOOffset + fspOSize - chunkSize, 0)
            FspTandOData += fr.read(chunkSize)

            for t in HASH_TYPE_VALUE:
                self.digestList.append(HashStruct(t, self.OpenSslCommand, FspTandOData))

        self.header.Count = len(self.digestList)
        self.header.Size = self.GetFspDigestListLen()

    def CalFspMOrSDigestList(self, fsp, excludeUpd) -> None:
        """
        Calculate the digest list for FSP-M or FSP-S.

        When excludeUpd flag is set to True, the digest calculation includes the region of FSP-M/S before UPD
        and after UPD. When excludeUpd flag is set to False, the entire FSP-M/S region is used to calculate the digest.

        Args:
            fsp (FspObject): The FSP-M or FSP-S object.
            excludeUpd (bool): Flag indicating whether to exclude UPD region for calculation.

        """
        self.header.FspComponentId = FBM_COMPONENT_ID[fsp.Type]
        fspDataToCalDigest = b''
        with open(self.fspFdDir, "rb") as fr:
            if excludeUpd is True:
                fr.seek(fsp.Offset, 0)
                regionSize = fsp.Fih.CfgRegionOffset - fsp.Offset
                fspDataToCalDigest += fr.read(regionSize)

                # regionOffset is the offset in FSP FV
                regionOffset = fsp.Fih.CfgRegionOffset + fsp.Fih.CfgRegionSize - fsp.Offset
                regionSize = fsp.Fih.ImageSize + fsp.Offset - (fsp.Fih.CfgRegionOffset + fsp.Fih.CfgRegionSize)
                fr.seek(fsp.Offset + regionOffset, 0)

                fspDataToCalDigest += fr.read(regionSize)
            else:
                fr.seek(fsp.Offset, 0)
                regionSize = fsp.Fih.ImageSize
                fspDataToCalDigest += fr.read(regionSize)

        for t in HASH_TYPE_VALUE:
            self.digestList.append(HashStruct(t, self.OpenSslCommand, fspDataToCalDigest))

        self.header.Count = len(self.digestList)
        self.header.Size = self.GetFspDigestListLen()

    def GetFspDigestListLen(self) -> int:
        """
        Get the length of the FSP digest list in bytes.

        Returns:
            int: The length of the FSP digest list.

        """
        length = sizeof(FspRegionDigestHeader)
        for digest in self.digestList:
            length += digest.GetOverallLength()
        return length

    def SerializeToBin(self) -> bytes:
        """
        Serialize the FSP region digest to binary data.

        Returns:
            bytes: The serialized binary data of the FSP region digest.

        """
        fspComponentBin = bytes(self.header)

        for digest in self.digestList:
            fspComponentBin += digest.SerializeToBin()

        return fspComponentBin

class FspBootManifestHead(Structure):
    _pack_ = 1
    _fields_ = [
        ('StructureId',         ARRAY(c_uint8, 8)),
        ('StructVersion',       c_uint8),
        ('Reserved1',           c_uint24),
        ('KeySignatureOffset',  c_uint16),
        ('FspVersion',          c_uint16),
        ('FspSvn',              c_uint8),
        ('Reserved2',           ARRAY(c_uint8, 1)),
        ('Flags',               c_uint32),
    ]

    def DumpFbmHead(self) -> None:
        print('#################################')
        print("# Fsp Boot Manifest information #")
        print('#################################')

        StructureIdStr = ""
        for i in range(len(self.StructureId)):
            StructureIdStr += chr(self.StructureId[i])
        if StructureIdStr != '__FBMS__':
            print('FBM Not found!')
            exit(1)

        width = 22
        FspVersionStr = hex(self.FspVersion >> 8)[2:]
        FspVersionStr += '.' + hex(self.FspVersion & 0xff)[2:]
        print("{:<{width}} {}".format("Structure Id:", StructureIdStr, width=width))
        print("{:<{width}} {}".format("Struct Version:", hex(self.StructVersion), width=width))
        print("{:<{width}} {}".format("Key Signature Offset:", hex(self.KeySignatureOffset), width=width))
        print("{:<{width}} {}".format("Fsp Version:", FspVersionStr, width=width))
        print("{:<{width}} {}".format("Fsp SVN:", hex(self.FspSvn), width=width))
        print("{:<{width}} {}".format("Flags:", self.Flags, width=width))

class FspBootManifestFixedField(Structure):
    _pack_ = 1
    _fields_ = [
        ('FBMHeader',       FspBootManifestHead),
        ('CompCnt',         c_uint8),
    ]

    # Convert FBM common Header fields in C format to bytes array
    def serialize(self) -> bytes:
        binData = bytes(self.FBMHeader)
        return binData

def ConvertVersionStrToHex(versionStr) -> int:
    """
    Convert version from "xx.xx" string format to hex format.

    The number of '.' characters in the version string should not exceed 4, and each "xx" segment
    should be within the range of 0-255.

    Args:
        versionStr (str): The version string in "xx.xx" format.

    Returns:
        int: The version number in hex format.

    """
    versionList = versionStr.split('.')
    res = 0
    for item in versionList:
        res = (res << 8) | (int(item, 16) & 0xff)
    return res

class REGION_SEGMENT(Structure):
    """
    Structure representing an IBB Segment.
    Bit 0 of Flags indicates Segment Type, 0: a Hashed IBB Segment, 1: a Non-Hashed IBB Segment
    Bit 2:1 of Flags indicates Caching Type, 0: Write Protect (WP), 1: Write Back (WB), 2, 3: Reserved
    FSP-M is WB caching type, the other FSP components is WP type

    Attributes:
        Reserved (c_uint16): Reserved field.
        Flags (c_uint16): Flags field indicating the segment type and caching type.
        Base (c_uint32): Base address of the segment.
        Size (c_uint32): Size of the segment.

    """
    _pack_ = 1
    _fields_ = [
        ('Reserved',    c_uint16),
        ('Flags',       c_uint16),
        ('Base',        c_uint32),
        ('Size',        c_uint32),
    ]

    def DumpInfo(self, spaceNum) -> None:
        """
        Dump the information of the IBB Segment.

        Args:
            spaceNum (int): The number of spaces for indentation in the output.

        """
        print(' ' * spaceNum + 'Flags:', hex(self.Flags))
        print(' ' * spaceNum + 'Base: ', hex(self.Base))
        print(' ' * spaceNum + 'Size: ', hex(self.Size))

class FspRegionInfoHead(Structure):
    _pack_ = 1
    _fields_ = [
        ('ComponentID',    c_uint8),
        ('SegmentCnt',     c_uint8),
    ]

    def DumpInfo(self, spaceNum) -> None:
        fspType = CovertFbmCompIDToStr(self.ComponentID)
        print(' ' * spaceNum + 'FSP Component ID:', self.ComponentID, "(FSP-%s)" % fspType)
        print(' ' * spaceNum + 'FSP SegmentCnt:', self.SegmentCnt)

class FspRegionInfo():
    def __init__(self):
        self.SegmentArray = []
        self.head = FspRegionInfoHead()

    def Serialize(self):
        binData = bytes(self.head)
        for i in range(self.head.SegmentCnt):
            binData += bytes(self.SegmentArray[i])
        return binData

    def GetLength(self) -> int:
        length = sizeof(FspRegionInfoHead)
        length += self.head.SegmentCnt * sizeof(REGION_SEGMENT)
        return length

def DumpFspRegionInfo(fbmBin: bytes, offset: int, spaceNumber: int) -> int:
    """
    Dump FSP region information from the given FBM binary.

    Args:
        fbmBin (bytes): The FBM binary data.
        offset (int): The starting offset to read the FSP region information.
        spaceNumber (int): The number of spaces for indentation in the output.

    Returns:
        int: The updated offset after reading the FSP region information.

    """
    fspRegionHead = FspRegionInfoHead.from_buffer(fbmBin, offset)
    fspRegionHead.DumpInfo(spaceNumber)
    offset += sizeof(FspRegionInfoHead)
    for i in range(fspRegionHead.SegmentCnt):
        seg = REGION_SEGMENT.from_buffer(fbmBin, offset)
        offset += sizeof(REGION_SEGMENT)
        print(' ' * spaceNumber + "Segment Array[%d]" % i)
        seg.DumpInfo(spaceNumber + 4)
    return offset

class KEY_AND_SIGNATURE_STRUCT():
    def __init__(self) -> None:
        self.Version         = 0x0
        self.KeyAlg          = 0x1
        self.Key_Version     = 0x10
        self.Key_KeySizeBits = 3072
        self.Key_Exponent    = 65537
        self.Key_Modulus     = bytes(PUB_KEY_SIZE)
        self.SigScheme       = 0x14
        self.Sig_Version     = 0x0
        self.Sig_KeySizeBits = 3072
        self.Sig_HashAlg     = 0xB
        self.Sig_Signature   = bytes(PUB_KEY_SIZE)

    def InitFromBuffer(self, Buffer: bytes) -> None:
        unpackOffset = 0
        (Version, KeyAlg, Key_Version, Key_KeySizeBits, Key_Exponent) = struct.unpack("<BHBHI", Buffer[unpackOffset : unpackOffset + struct.calcsize("<BHBHI")])
        unpackOffset += struct.calcsize("<BHBHI")
        Key_Modulus = Buffer[unpackOffset : unpackOffset + PUB_KEY_SIZE]
        unpackOffset += PUB_KEY_SIZE
        (SigScheme, Sig_Version, Sig_KeySizeBits, Sig_HashAlg) = struct.unpack("<HBHH", Buffer[unpackOffset : unpackOffset + struct.calcsize("<HBHH")])
        unpackOffset += struct.calcsize("<HBHH")
        Sig_Signature = Buffer[unpackOffset : unpackOffset + PUB_KEY_SIZE]

        self.Version         = Version
        self.KeyAlg          = KeyAlg
        self.Key_Version     = Key_Version
        self.Key_KeySizeBits = Key_KeySizeBits
        self.Key_Exponent    = Key_Exponent
        self.Key_Modulus     = Key_Modulus
        self.SigScheme       = SigScheme
        self.Sig_Version     = Sig_Version
        self.Sig_KeySizeBits = Sig_KeySizeBits
        self.Sig_HashAlg     = Sig_HashAlg
        self.Sig_Signature   = Sig_Signature

    def InitFromKeyPair(self, PubKey: bytes, Signature: bytes) -> None:
        self.Key_Modulus     = PubKey
        self.Sig_Signature   = Signature

    def GetSize(self) -> int:
        size = struct.calcsize("<BHBHI") + PUB_KEY_SIZE + struct.calcsize("<HBHH") + PUB_KEY_SIZE
        return size

    def serialize(self) -> bytes:
        buffer = struct.pack("<BHBHI", self.Version, self.KeyAlg, self.Key_Version, self.Key_KeySizeBits, self.Key_Exponent)
        buffer += self.Key_Modulus
        buffer += struct.pack("<HBHH", self.SigScheme, self.Sig_Version, self.Sig_KeySizeBits, self.Sig_HashAlg)
        buffer += self.Sig_Signature
        return buffer

    def DumpInfo(self) -> None:
        spaceNumber = 4
        print(' ' * spaceNumber + "{:<16} {}".format("Version:", hex(self.Version)))
        print(' ' * spaceNumber + "{:<16} {}".format("KeyAlg:", hex(self.KeyAlg)))
        print(' ' * spaceNumber + "{:<16} {}".format("Key_Version:", hex(self.Key_Version)))
        print(' ' * spaceNumber + "{:<16} {}".format("Key_KeySizeBits:", hex(self.Key_KeySizeBits)))
        print(' ' * spaceNumber + "{:<16} {}".format("Exponent:", hex(self.Key_Exponent)))
        print(' ' * spaceNumber + "{:<16}".format("Modulus:"))
        HexDump(self.Key_Modulus, spaceNumber)
        print(' ' * spaceNumber + "{:<16} {}".format("SigScheme:", hex(self.SigScheme)))
        print(' ' * spaceNumber + "{:<16} {}".format("Sig_Version:", hex(self.Sig_Version)))
        print(' ' * spaceNumber + "{:<16} {}".format("Sig_KeySizeBits:", hex(self.Sig_KeySizeBits)))
        print(' ' * spaceNumber + "{:<16} {}".format("Sig_HashAlg:", hex(self.Sig_HashAlg)))
        print(' ' * spaceNumber + "{:<16}".format("Signature:"))
        HexDump(self.Sig_Signature, spaceNumber)

class FspBootManifest():
    def __init__(self, fbmDir: str, ltSignPath: str) -> None:
        self.fbmFixedField = FspBootManifestFixedField()
        self.ComponentDigests_0 = None
        self.ComponentDigestsList = []
        self.FspRegions_0 = None
        self.FspRegionsList = []
        self.fdDir = None
        self.fd = None
        self.OpenSslCommand = None
        self.configFile = None
        self.excludeUpd = None
        self.outputFile = None
        self.signFbm = SignFbm(fbmDir, ltSignPath)
        self.keyPairFilePath = None

    def CalFspMOrSRegionInfo(self, fspType : str) -> None:
        """
        Calculate FSP-M or FSP-S region information.

        Args:
            fspType (str): The type of FSP ("M" for FSP-M, "S" for FSP-S).

        """
        for fsp in self.fd.FspList:
            if fsp.Type == fspType:
                fspRegion = FspRegionInfo()
                fspRegion.head.ComponentID = FBM_COMPONENT_ID[fsp.Type]
                if not self.excludeUpd:
                    # Single IBB Segment for entire FSP-M/S region
                    seg = REGION_SEGMENT()
                    seg.Base = 0x0
                    seg.Size = fsp.Fih.ImageSize
                    #
                    # Set the bit 0 of Flags to 0 as the entire FSP-M/S region is covered in digest calculation.
                    # For the details of Flags, refer to the definition of REGION_SEGMENT.
                    #
                    if fspType == "M":
                        seg.Flags = 0x2  # FSP-M caching type is Write Back (WB)
                    else:
                        seg.Flags = 0x0  # Other FSP caching types are Write Protect (WP)
                    fspRegion.SegmentArray.append(seg)
                else:
                    # Three IBB Segments for FSP-M/S region with UPD config region
                    seg0 = REGION_SEGMENT()
                    seg0.Base = 0x0
                    seg0.Size = fsp.Fih.CfgRegionOffset - fsp.Offset
                    if fspType == "M":
                        seg0.Flags = 0x2
                    fspRegion.SegmentArray.append(seg0)

                    #
                    # FSP UPD config region
                    # Set the bit 0 of Flags to 1, as the entire FSP-M/S region is not covered in digest calculation.
                    #
                    seg1 = REGION_SEGMENT()
                    seg1.Base = fsp.Fih.CfgRegionOffset - fsp.Offset
                    seg1.Size = fsp.Fih.CfgRegionSize
                    if fspType == "M":
                        seg1.Flags = 0x3
                    else:
                        seg1.Flags = 0x1
                    fspRegion.SegmentArray.append(seg1)

                    seg2 = REGION_SEGMENT()
                    seg2.Base = fsp.Fih.CfgRegionOffset + fsp.Fih.CfgRegionSize - fsp.Offset
                    seg2.Size = fsp.Fih.ImageSize - fspRegion.SegmentArray[0].Size - \
                                fspRegion.SegmentArray[1].Size
                    if fspType == "M":
                        seg2.Flags = 0x2
                    else:
                        seg2.Flags = 0x0
                    fspRegion.SegmentArray.append(seg2)

                fspRegion.head.SegmentCnt = len(fspRegion.SegmentArray)
                self.FspRegionsList.append(copy.deepcopy(fspRegion))
                break

    def CalFspTAndORegionInfo(self) -> None:
        """
        The function is used to calculate Fsp Region information for FSP-T and FSP-O.
        When excludeUpd is set to False, there are two IBB Segments in Segment Array, the FSP-T and FSP-O.
        When excludeUpd is set to True, there are four IBB Segments in Segment Array.
        The first one is the region of the entire FSP-T.
        The second one is the region of FSP-O before OEM config region.
        The third one is the UPD config region in FSP-O.
        The forth one is the region of FSP-O after UPD config region.
        """
        fspT = None
        fspO = None
        for fsp in self.fd.FspList:
            if fsp.Type == "T":
                fspT = fsp
            elif fsp.Type == "O":
                fspO = fsp

        if fspT is None or fspO is None:
            print("Error! FspO or FspT is none")
            sys.exit(1)

        fspRegion = FspRegionInfo()
        fspRegion.ComponentID = FBM_COMPONENT_ID["TO"]

        #
        # FSP-T region
        #
        seg0 = REGION_SEGMENT()
        seg0.Base = 0x0
        seg0.Size = fspT.Fih.ImageSize
        seg0.Flags = 0x0
        fspRegion.SegmentArray.append(seg0)

        if self.excludeUpd == True:
            #
            # FSP-O region before the OEM config data region
            #
            seg1 = REGION_SEGMENT()
            seg1.Base = fspT.Fih.ImageSize
            seg1.Size = fspO.Fih.ImageSize - FSPO_OEM_CONFIG_REGION_LOWER_ADDR
            seg1.Flags = 0x0
            fspRegion.SegmentArray.append(seg1)

            #
            # OEM config data region
            #
            seg2 = REGION_SEGMENT()
            seg2.Base = fspO.Fih.ImageSize - FSPO_OEM_CONFIG_REGION_LOWER_ADDR + fspT.Fih.ImageSize
            seg2.Size = FSPO_OEM_CONFIG_REGION_LOWER_ADDR - FSPO_OEM_CONFIG_REGION_UPPER_ADDR
            seg2.Flags = 0x1
            fspRegion.SegmentArray.append(seg2)

            #
            # FSP-O region after the OEM config data region
            #
            seg3 = REGION_SEGMENT()
            seg3.Base = fspO.Fih.ImageSize - FSPO_OEM_CONFIG_REGION_UPPER_ADDR + fspT.Fih.ImageSize
            seg3.Size = FSPO_OEM_CONFIG_REGION_UPPER_ADDR
            seg3.Flags = 0x0
            fspRegion.SegmentArray.append(seg3)

        else:
            seg1 = REGION_SEGMENT()
            seg1.Base = fspT.Fih.ImageSize
            seg1.Size = fspO.Fih.ImageSize
            seg1.Flags = 0x0
            fspRegion.SegmentArray.append(seg1)

        fspRegion.head.SegmentCnt = len(fspRegion.SegmentArray)
        self.FspRegions_0 = fspRegion

    def GenerateFbmBin(self, fdDir, openSslCommand, configFile, outputFile) -> None:
        """
        Generate FBM binary without signature.

        Args:
            fdDir (str): The directory path containing the firmware device (FD) files.
            openSslCommand (str): The path to the OpenSSL command line tool.
            configFile (str): The path to the configuration file.
            outputFile (str): The path to the output FBM binary file.

        Returns:
            None

        Raises:
            Any exceptions that may occur during the execution of the function.
        """
        self.fdDir = fdDir
        self.fd = FirmwareDevice(fdDir)
        self.fd.ParseFd()
        self.fd.ParseFsp()
        self.OpenSslCommand = openSslCommand
        self.configFile = configFile
        self.excludeUpd = True
        self.outputFile = outputFile

        self.ParseConfigFile()
        self.CalFspSOrMRegionDigest()
        self.CalFSPTAndORegionDigest()
        self.CalFspMOrSRegionInfo("M")
        self.CalFspMOrSRegionInfo("S")
        self.CalFspTAndORegionInfo()
        self.CalFbmSigOffset()
        self.ConvertFbmToBytesArray()
        self.GenerateFbmWithSig()

    def GenerateFbmWithSig(self) -> None:
        """
        Generate FBM binary file with signature.

        Returns:
            None
        """
        # Generate LT-CSS Opaque Header for FBM binary
        sigFile = self.signFbm.GenFbmWithLtcssOpaqueHeader(self.outputFile, self.keyPairFilePath)
        with open(self.outputFile, "rb") as fr:
            fbmBin = fr.read()

        # Read LT-CSS Opaque Header and extract public key and signature
        with open(sigFile, "rb") as fr:
            sigBuffer = fr.read()
            RsaPubKey = sigBuffer[0xC : 0xC + PUB_KEY_SIZE]
            SignatureRsa = sigBuffer[0xC + PUB_KEY_SIZE + 0x4: 0xC + PUB_KEY_SIZE + 0x4 + PUB_KEY_SIZE]
            KeyAndSignature = KEY_AND_SIGNATURE_STRUCT()
            KeyAndSignature.InitFromKeyPair(RsaPubKey, SignatureRsa)
            fbmBin += KeyAndSignature.serialize()
        RemoveFile(sigFile)

        # Write updated FBM binary to file
        with open(self.outputFile, "wb") as fw:
            fw.write(fbmBin)

    def CalFbmSigOffset(self) -> None:
        """
        Calculate the signature offset in the FBM binary.

        Returns:
            None
        """
        fbmSize = 0
        fbmSize += sizeof(FspBootManifestFixedField)
        fbmSize += self.ComponentDigests_0.GetFspDigestListLen()

        for fspc in self.ComponentDigestsList:
            fbmSize += fspc.GetFspDigestListLen()
        fbmSize += self.FspRegions_0.GetLength()

        for fspRegion in self.FspRegionsList:
            fbmSize += fspRegion.GetLength()

        # If fbmSize plus sign data header size is less than gMinimalToSignDataSize, padding data will be added after fbm binary.
        if fbmSize + sizeof(SignDataHeader) < MINIMAL_TO_SIGN_DATA_SIZE:
            paddingSize = MINIMAL_TO_SIGN_DATA_SIZE - fbmSize - sizeof(SignDataHeader)
            self.fbmFixedField.FBMHeader.KeySignatureOffset = MINIMAL_TO_SIGN_DATA_SIZE - sizeof(SignDataHeader)
            print("FbmSize is less than minimal size of data to be signed, need to add padding data {} Bytes".format(paddingSize))
        else:
            self.fbmFixedField.FBMHeader.KeySignatureOffset = fbmSize
        print("Set Signature Offset to {}".format(self.fbmFixedField.FBMHeader.KeySignatureOffset))

    def CalFSPTAndORegionDigest(self) -> None:
        """
        Calculate digest list for FSP-O and FSP-T components.

        Returns:
            None
        """
        self.ComponentDigests_0 = FspRegionDigest(self.fdDir, self.OpenSslCommand)
        fspO = None
        fspT = None
        for fsp in self.fd.FspList:
            if fsp.Type == 'T':
                fspT = fsp
            elif fsp.Type == 'O':
                fspO = fsp
        if fspO != None and fspT != None:
            self.ComponentDigests_0.CalFspTAndODigestList(fspT, fspO, self.excludeUpd)
            self.fbmFixedField.CompCnt += 1
        else:
            print("Error! FSP-O or FSP-T is none")
            sys.exit(1)

    def CalFspSOrMRegionDigest(self) -> None:
        """
        Calculate digest lists for FSP-M and S

        Returns:
            None
        """
        fspM = None
        fspS = None
        for fsp in self.fd.FspList:
            if fsp.Type == 'M':
                fspM = fsp
            elif fsp.Type == 'S':
                fspS = fsp

        if fspM is not None:
            fspmC = FspRegionDigest(self.fdDir, self.OpenSslCommand)
            fspmC.CalFspMOrSDigestList(fspM, self.excludeUpd)
            self.ComponentDigestsList.append(fspmC)
        else:
            print("Error! FSP-M is None")
            sys.exit(1)

        if fspS is not None:
            fspsC = FspRegionDigest(self.fdDir, self.OpenSslCommand)
            fspsC.CalFspMOrSDigestList(fspS, self.excludeUpd)
            self.ComponentDigestsList.append(fspsC)
        else:
            print("Error! FSP-S is None")
            sys.exit(1)

        self.fbmFixedField.CompCnt += len(self.ComponentDigestsList)

    def ParseConfigFile(self) -> None:
        """
        Parse the configuration file and populate the FBMHeader fields accordingly.

        Returns:
            None
        """
        parser = configparser.ConfigParser()
        parser.read(self.configFile)

        tmpStr = parser.get('FBMConfig', 'StructureId')
        for i in range(len(tmpStr)):
            self.fbmFixedField.FBMHeader.StructureId[i] = ord(tmpStr[i])

        tmpStr = parser.get('FBMConfig', 'StructVersion')
        self.fbmFixedField.FBMHeader.StructVersion = int(tmpStr, 16)

        tmpStr = parser.get('FBMConfig', 'FspVersion')
        self.fbmFixedField.FBMHeader.FspVersion = ConvertVersionStrToHex(tmpStr)

        tmpStr = parser.get('FBMConfig', 'FspSvn')
        self.fbmFixedField.FBMHeader.FspSvn = int(tmpStr, 16)

        tmpStr = parser.get('FBMConfig', 'Flags')
        self.fbmFixedField.FBMHeader.Flags = int(tmpStr, 16)

        excludeUpdStr = parser.get('FBMConfig', 'ExcludeUpd')
        if excludeUpdStr.upper() == 'FALSE':
            self.excludeUpd = False

        self.keyPairFilePath = parser.get('FBMConfig', 'KeypairFilePath')

    def ConvertFbmToBytesArray(self) -> None:
        """
        Convert the FBM object to a byte array and write it to the output file.

        Returns:
            None
        """
        binData = self.fbmFixedField.serialize()
        binData += struct.pack('<B', self.fbmFixedField.CompCnt)
        binData += self.ComponentDigests_0.SerializeToBin()

        for fspComponentDigests in self.ComponentDigestsList:
            binData += fspComponentDigests.SerializeToBin()

        binData += self.FspRegions_0.Serialize()

        for fspRegion in self.FspRegionsList:
            binData += fspRegion.Serialize()

        #
        # Add padding data at the end of FBM
        #
        paddingDataSize = MINIMAL_TO_SIGN_DATA_SIZE - len(binData) - sizeof(SignDataHeader)

        if paddingDataSize > 0:
            print("Add {} bytes padding data at the end of FBM".format(paddingDataSize))
            binData += paddingDataSize * struct.pack('<B', 0)

        with open(self.outputFile, "wb") as fw:
            fw.write(binData)

def DumpInfo(imagePath: str) -> None:
    """
    Dump information of the FBM from the image.

    Args:
        imagePath (str): Path to the FBM image file.

    Returns:
        None
    """
    with open(imagePath, "rb") as fbm:
        fbmFileSize = os.path.getsize(imagePath)
        fbmData = bytearray(fbm.read(fbmFileSize))
    fbmHeader = FspBootManifestHead.from_buffer(fbmData, 0)
    fbmHeader.DumpFbmHead()
    offset = sizeof(FspBootManifestHead)

    CompCnt = c_uint8.from_buffer(fbmData, offset).value
    print("FSP component Number:", '{:>2}'.format(CompCnt))
    offset += sizeof(c_uint8)

    print("FSP-T and FSP-O component Digest List")
    offset = DumpFspRegionDigest(fbmData, offset, 4)
    for i in range(CompCnt - 1):
        print("FSP Component List[{}]:".format(i))
        offset = DumpFspRegionDigest(fbmData, offset, 4)

    print("FspRegions_0:")
    offset = DumpFspRegionInfo(fbmData, offset, 4)

    for i in range(FSP_REGIONS_NUM_EXCEPT_FSPT_AND_O):
        print("FspRegions_{}:".format(i + 1))
        offset = DumpFspRegionInfo(fbmData, offset, 4)

    if fbmHeader.KeySignatureOffset - offset > 0:
        print("Padding data length is {} Bytes".format(fbmHeader.KeySignatureOffset - offset))

    print('Fbm KEY_AND_SIGNATURE_STRUCT:')
    fbmSig = KEY_AND_SIGNATURE_STRUCT()
    fbmSig.InitFromBuffer(fbmData[fbmHeader.KeySignatureOffset:])
    fbmSig.DumpInfo()

def GetFbmFromFitTable(imagePath: str, fbmBinPath: str) -> None:
    """
    Get FBM buffer from the input image.

    Args:
        imagePath (str): Path to the FBM image file.

    Returns:
        None
    """
    FIT_POINTER_ADDRESS = 0x40
    FbmFound = False
    fbmData  = None
    with open(imagePath, "rb") as image:
        buffer = image.read()
        bufferSize = len(buffer)
        fitTableAddress = struct.unpack('<I', buffer[bufferSize - FIT_POINTER_ADDRESS : bufferSize - FIT_POINTER_ADDRESS + 4])[0]
        fitTableOffset = ConvertToImageAddress(bufferSize, fitTableAddress)
        fitHeader = FIRMWARE_INTERFACE_TABLE_ENTRY.from_buffer_copy(buffer[fitTableOffset : fitTableOffset + sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY)])
        if fitHeader.Address != struct.unpack('<Q', b'_FIT_   ')[0]:
            print('FIT Table not found!')
            exit(1)
        entryNum = int(fitHeader.Size)
        for index in range(1, entryNum):
            entryBeginOffset = fitTableOffset + sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY) * index
            entryEndOffset = entryBeginOffset + sizeof(FIRMWARE_INTERFACE_TABLE_ENTRY)
            entryBuffer = buffer[entryBeginOffset : entryEndOffset]
            fitEntry = FIRMWARE_INTERFACE_TABLE_ENTRY.from_buffer_copy(entryBuffer)
            if fitEntry.Type == 0xD:
                FbmFound = True
                fbmAddress = fitEntry.Address
                fbmSize = int(fitEntry.Size)
                fbmOffset = ConvertToImageAddress(bufferSize, fbmAddress)
                fbmData = bytearray(buffer[fbmOffset:fbmOffset + fbmSize])
                with open(fbmBinPath, 'wb') as F:
                    F.write(fbmData)
                DumpInfo(fbmBinPath)
                break
        if not FbmFound:
            print('FBM not found!')
            exit(1)

    if fbmData != None:
        fbmHeader = FspBootManifestHead.from_buffer(fbmData, 0)
        KeySignatureOffset = fbmHeader.KeySignatureOffset
        KeySignatureBuffer = KEY_AND_SIGNATURE_STRUCT()
        KeySignatureBuffer.InitFromBuffer(fbmData[KeySignatureOffset : KeySignatureOffset + KeySignatureBuffer.GetSize()])
        LtCssHeader = SignDataHeader()
        LtCssHeader.ModuleType = SIGN_MODULE_TYPE
        LtCssHeader.HeaderVersion = SIGN_DATA_HEADER_VERSION
        LtCssHeader.HeaderLen = SIGN_DATA_HEADER_LEN
        LtCssHeader.Exponent = KeySignatureBuffer.Key_Exponent
        LtCssHeader.PublicKey[:] = KeySignatureBuffer.Key_Modulus
        LtCssHeader.Signature[:] = KeySignatureBuffer.Sig_Signature
        output = bytes(LtCssHeader) + fbmData[:KeySignatureOffset]
        with open(fbmBinPath, 'wb') as F:
                    F.write(output)

def RunCommand(Cmd: str, Timeout = None) -> str:
    """
    Run a shell command and return the output.

    Args:
        Cmd (str): The shell command to run.
        Timeout (int, optional): Timeout value in seconds. Defaults to None.

    Returns:
        str: The output of the command.

    """
    if not isinstance(Timeout, int):
        Timeout = None
    Process = subprocess.Popen(Cmd,
                               shell = True,
                               stdout = subprocess.PIPE,
                               stdin = subprocess.PIPE,
                               stderr = subprocess.STDOUT,
                               universal_newlines = True
                               )
    try:
        Output = Process.communicate(timeout = Timeout)[0]
    except subprocess.TimeoutExpired:
        subprocess.Popen(["taskkill", "/t", "/f", "/pid", str(Process.pid)], shell=True)
        print("Timeout occurred after {} seconds".format(str(Timeout if Timeout else "zero")))
    return Output

class SignFbm():
    def __init__(self, fbmDir: str, ltSignPath: str) -> None:
        """
        Initialize SignFbm class.

        Args:
            fbmDir (str): Directory path of FBM.bin.

        """
        self.SignDataHeader = SignDataHeader()
        self.workSpace = fbmDir
        self.ltsignTool = ltSignPath

    def CalHeaderLen(self, bitSize : int) -> int:
        """
        Calculate the HeaderLen of the header to sign data.
        HeaderLen equals to headerSize + publicKeyLen + sigLen + exponent Size.
        Note HeaderLen is presented in DWORD as a requirement in LT-CSS flow requirement

        Args:
            bitSize (int): Bit size of the key.

        Returns:
            int: HeaderLen in DWORD.

        """
        headerSize   = 12
        publicKeyLen = bitSize / 8
        exponentSize = EXPONENT_SIZE

        # Set the signature length to the key length
        sigLen = publicKeyLen

        # Convert len in bytes to DWORD
        headerLen = headerSize + publicKeyLen + exponentSize + sigLen
        print("Sign data header length:", int(headerLen))
        if headerLen % 4 != 0:
            print("Error! HeaderLen %d %% 4 != 0" % headerLen)
            sys.exit(1)
        return int(headerLen / 4)

    def SignFbmData(self, toSignFbmFile : str, outputFile : str, keypairFile : str) -> None:
        """
        Sign the FBM data using ltsignTool.

        Args:
            toSignFbmFile (str): Path to the FBM file to sign.
            outputFile (str): Path to save the signed output file.
            keypairFile (str): Path to the keypair file.

        """
        cmd = self.ltsignTool
        cmd += " --sign -i " + toSignFbmFile
        cmd += " -o " + outputFile
        cmd += " -k " + keypairFile
        res = RunCommand(cmd)
        print("Result of signing:\n", res)

    def VerifyFbmSign(self, signedFile : str, keypairFile : str) -> None:
        """
        Verify the FBM signature using ltsignTool.

        Args:
            signedFile (str): Path to the signed FBM file.
            keypairFile (str): Path to the keypair file.

        """
        cmd = self.ltsignTool + " --verify -i " + signedFile + " -k " + keypairFile
        res = RunCommand(cmd)
        print("Result of verification:\n", res)

    def GenFbmWithLtcssOpaqueHeader(self, fbmFile : str, keyPairfilePath : str) -> bytes:
        """
        Generate FBM with LT-CSS Opaque Header.

        # Field Name #   # Offset #  # Size #                                # CSS Validation #                                            # Signed #
          ModuleType       0x00        4B                   Must be 0x14                                                                      No
          HeaderLen        0x04        4B                   Must equal (HeaderSize(12B) + publicKeyLen + sigLen + exponentSize) in DWORDS     No
          HeaderVersion    0x08        4B                   Must be 0x10000                                                                   No
          PublicKey        0x0C        RSA: 384, EC: 48+48  Zero on input, filled by CSS, Must be 384 bytes for                               No
          Exponent         (varies)    4B                   Zero on input, filled by CSS RSA only                                             No
          Signature        (varies)    RSA: 384, EC: 48+48  Zero on input, filled by CSS                                                      No
          Data             (varies)    (varies)             No CSS Validation                                                                 Yes

        Args:
            fbmFile (str): Path to the original FBM file.
            keyPairfilePath (str): Path to the key pair file.

        Returns:
            bytes: The generated FBM with signature.

        """
        with open(fbmFile, "rb") as fr:
            fbmData = fr.read()

        self.SignDataHeader.ModuleType = SIGN_MODULE_TYPE
        self.SignDataHeader.HeaderVersion = SIGN_DATA_HEADER_VERSION
        self.SignDataHeader.HeaderLen = self.CalHeaderLen(SIGN_BIT_SIZE)
        keyPairfilePath = os.path.join(os.getenv('WORKSPACE_ROOT'), keyPairfilePath)

        toSignDataFile = os.path.join(self.workSpace, 'fbmToSign.bin')
        fbmSig = os.path.join(self.workSpace, 'fbmSig.bin')
        with open(toSignDataFile, "wb") as fw:
            fbmSignInputBin = bytes(self.SignDataHeader) + fbmData
            fw.write(fbmSignInputBin)

        self.SignFbmData(toSignDataFile, fbmSig, keyPairfilePath)
        self.VerifyFbmSign(fbmSig, keyPairfilePath)
        RemoveFile(toSignDataFile)
        return fbmSig

def main() -> None:
    StartTime = datetime.datetime.now()
    Parser = argparse.ArgumentParser(
                        prog = __prog__,\
                        description = __description__ + __copyright__
                        )
    Parser.add_argument("-o", "--outputDir", dest = 'OutputDir', help = "designate output directory of FBM")
    Parser.add_argument("-c", "--configFile", dest = 'ConfigFile', help = 'Please input config file path')
    Parser.add_argument("-f", "--fspFile", dest = 'fspFile', help = 'Please input Fsp.fd path')
    Parser.add_argument("-i", "--imageDump", dest = 'ImageDumpPath', help = 'Please input BIOS/IFWI image path to dump Fbm info')
    Parser.add_argument("--ltsign-tool-path", dest = 'LtSignToolPath', type = str,  help = "Path to LT Sign tool.")
    Parser.add_argument("--openssl-tool-path", dest = 'OpensslToolPath', type = str, help = "Path to OpenSSL tool.")
    Args, _  = Parser.parse_known_args()

    if Args.ConfigFile != None and Args.OutputDir != None:
        OpenSslTool = os.getenv('OPENSSL_PATH')
        if OpenSslTool != None:
            OpenSslTool = os.path.join(OpenSslTool, 'openssl.exe')
        elif Args.OpensslToolPath != None and os.path.exists(Args.OpensslToolPath):
            OpenSslTool = Args.OpensslToolPath
        else:
            print("Error: Environment variable OPENSSL_PATH is empty")
            sys.exit(1)

        # Verify if Open SSL Tool is available
        try:
            Process = subprocess.Popen('%s version' % (OpenSslTool), stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
            Process.communicate()
        except:
            print('ERROR: Open SSL Tool is not available. Please verify PATH or set OPENSSL_PATH')
            sys.exit(1)

        # Verify if LT Sign Tool is available
        if Args.LtSignToolPath != None and os.path.exists(Args.LtSignToolPath):
            FbmBinPath = os.path.join(Args.OutputDir, 'Fbm.bin')
            fbm = FspBootManifest(Args.OutputDir, Args.LtSignToolPath)
            fbm.GenerateFbmBin(Args.fspFile, OpenSslTool, Args.ConfigFile, FbmBinPath)
        else:
            print('ERROR: LT Sign Tool is not available.')
            sys.exit(1)

        DumpInfo(FbmBinPath)

    if Args.ImageDumpPath != None:
        FbmBinPath = os.path.join(os.path.dirname(Args.ImageDumpPath), 'Fbm.bin')
        GetFbmFromFitTable(Args.ImageDumpPath, FbmBinPath)

    print("Fbm gen tool start time: ", StartTime)
    print("Fbm gen tool end time: ", datetime.datetime.now())
    print("Fbm gen tool takes:", datetime.datetime.now() - StartTime)

if __name__ == '__main__':
    main()