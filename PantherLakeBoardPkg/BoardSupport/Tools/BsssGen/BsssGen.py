## @file
#  Bsss Gen tool.
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
# Intel® Converged Boot Guard and Intel® Trusted Execution Technology (Intel® TXT)
##

import argparse
import sys
import re
import os
import struct
from ctypes import *
from functools import reduce
import subprocess
import copy
from GetSecCoreEntry import GetSecCoreEntry

__copyright__   = "INTEL CONFIDENTIAL \n Copyright (c) 2023, Intel Corporation. All rights reserved. "
__description__ = 'Bsis Gen Tool.\n'
__prog__        = sys.argv[0]

FSP_PKG_REVISION   = 0x02
SEARCH_CHUNK_SIZE  = 4096
BSIS_VERSION       = 0x20
BSSS_VERSION       = 0x10
BSIS_MAX_LENGTH    = 1024
TPM_BASE_ADDRESS   = 0xFED40000
FSPT_UPD_SIGNATURE = "PTLUPD_T"
BSIS_ID            = "__BSIS__"
BSSS_ID            = "__BSPM__"

#
# The length of FSP-T UPD can be got from the Fspt upd structure(FSPT_UPD) defined in the file
# Intel\\OneSiliconPkg\\Fsp\\Include\\FsptUpd.h
# Length of FSP-T UPD equals to the offset of the last field of FSPT_UPD add the size of the last field.
# For example: Currently, the offset of UpdTerminator is 0x00FE, and sizeof(UpdTerminator) is 2 Bytes.
# So the length of FSP-T UPD is 0x100 Bytes.
#
FSPT_UPD_LEN          = 0x100
BSP_SEGMENT_MAX_COUNT = 8

#
# As the max allowed PCDE data size is 512B, only choose the digest needed
#
HASH_TYPE_VALUE = {
    "sha384": 0xC,
    "sha1"  : 0x4,
    "sha256": 0xB,
    # "sha512": 0xD,
    "sm3": 0x12,
    }

HASH_DIGEST_SIZE = {
    "sha256": 32,
    "sha512": 64,
    "sm3": 32,
    "sha1": 20,
    "sha384": 48,
    }

#
# The FlashMap is used only in FSP RESET feature.
#
class Flashmap():
    def __init__(self, FlashmapFile, PrepRelease, ExtendedRegionBuildFlag):
        with open(FlashmapFile, "r") as FileRead:
            Lines = FileRead.readlines()
        self.FilePath = FlashmapFile
        self.ExtendedRegionBuildFlag = ExtendedRegionBuildFlag

        #
        # Locate release or debug flashmap settings
        #
        ReleaseLinePos = -1
        DebugLinePos = -1
        Index = 0
        while (ReleaseLinePos == -1 or DebugLinePos == -1) and Index < len(Lines):
            if "DEBUG" in Lines[Index]:
                DebugLinePos = Index
            elif "Release" in Lines[Index]:
                ReleaseLinePos = Index
            Index += 1
        if ReleaseLinePos == -1 or DebugLinePos == -1:
            print("Error! In file: " + FlashmapFile + ", string: 'DEBUG' and 'Release' don't exist!")
            sys.exit(1)
        if PrepRelease == 'RELEASE':
            self.Map = Lines[0:DebugLinePos] + Lines[ReleaseLinePos:]
        elif PrepRelease == 'DEBUG':
            self.Map = Lines[0:ReleaseLinePos]
        else:
            print("Error! PrepRelease is not 'RELEASE' or 'DEBUG'")
            sys.exit(1)

        #
        # Locate fsp reset flashmap settings
        #
        Start = 0
        self.FspResetMap = []
        while Start < len(self.Map):
            if "PcdFspWrapperResetVectorInFsp" in self.Map[Start]:
                if "TRUE" in self.Map[Start]:
                    Flag = True
                elif "FALSE" in self.Map[Start]:
                    Flag = False
                else:
                    print("There is no 'TRUE' or 'FALSE' in" + self.Map[Start])
                    sys.exit(1)
                End = Start + 1
                while End < len(self.Map):
                    if "!else" in self.Map[End]:
                       break
                    End += 1
                if Flag:
                    self.FspResetMap = self.Map[Start:End]
                else:
                    self.FspResetMap = self.Map[End:]
                break
            Start += 1

        if self.FspResetMap == []:
           print("Error! In file: " + FlashmapFile + ", PCD 'PcdFspWrapperResetVectorInFsp' or related '!else' is not exist!")
           sys.exit(1)

        #
        #  This script parses the following PCDs.
        #
        self.PrememOffsetStr       = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPreMemoryOffset"
        self.PrememSizeStr         = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPreMemorySize"
        self.FspmOffsetStr         = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMOffset"
        self.FspmSizeStr           = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspMSize"
        self.ExtendedRegionSizeStr = "EXTENDED_REGION_SIZE"
        self.FlashBaseStr          = "FLASH_BASE"

    #
    # This call returns the last hit of the matches by parsing the file. If no hit, raise exception.
    #
    def ReadSingleSetting(self, Setting, TargetMap):
        SettingList = []
        PatternPcd = r"(?m)^\s*SET\s*" + Setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        for Line in TargetMap:
            for M in re.finditer(PatternPcd, Line):
                SettingList.append(int(M.group("value"), base = 16))
        try:
            ReadValue = SettingList[-1]
        except IndexError as err:
            print("Error! {} is not found in {}".format(Setting, self.FilePath))
            raise
        finally:
            return ReadValue

    def ReadDefineSetting(self, Setting, TargetMap):
        SettingList = []
        PatternDefine = r"(?m)^\s*DEFINE\s*" + Setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        for Line in TargetMap:
            for M in re.finditer(PatternDefine, Line):
                SettingList.append(int(M.group("value"), base = 16))
        try:
            ReadValue = SettingList[-1]
        except IndexError as err:
            print("Error! {} is not found in {}".format(Setting, self.FilePath))
            raise
        finally:
            return ReadValue

    def ReadAllSettings(self):
        self.FvPreMemoryOffset = "{:#08x}".format(self.ReadSingleSetting(self.PrememOffsetStr, self.FspResetMap))
        self.FvPreMemorySize = "{:#08x}".format(self.ReadSingleSetting(self.PrememSizeStr, self.FspResetMap))
        self.FvFspmOffset = "{:#08x}".format(self.ReadSingleSetting(self.FspmOffsetStr, self.Map))
        self.FvFspmSize = "{:#08x}".format(self.ReadSingleSetting(self.FspmSizeStr, self.Map))
        self.ExtendedRegionSize = hex(0x0)
        self.BiosFlashBaseAddr = "{:#08x}".format(self.ReadDefineSetting(self.FlashBaseStr, self.Map))
        if self.ExtendedRegionBuildFlag == 'TRUE':
            self.ExtendedRegionSize = "{:#08x}".format(self.ReadDefineSetting(self.ExtendedRegionSizeStr, self.Map))
            print("ExtendedRegionSize:", self.ExtendedRegionSize)
            #
            # Change FV offset when there is Extended Region in BIOS
            #
            self.FvPreMemoryOffset = hex(int(self.FvPreMemoryOffset, 16) - int(self.ExtendedRegionSize, 16))
            self.BiosFlashBaseAddr = hex(int(self.BiosFlashBaseAddr, 16) + int(self.ExtendedRegionSize, 16))
            self.FvFspmOffset      = hex(int(self.FvFspmOffset, 16) - int(self.ExtendedRegionSize, 16))

        print("Get flash map setting in file: ", self.FilePath)
        print("FvPreMemoryOffset:", self.FvPreMemoryOffset)
        print("FvPreMemorySize:", self.FvPreMemorySize)
        print("FvFspmOffset:", self.FvFspmOffset)
        print("BiosFlashBaseAddr:", self.BiosFlashBaseAddr)

#
# Convert the address in CLIENTBIOS.fd to the address of 4G space
#
def ConvertTo4GSpaceAddr(AddressInBios, BiosBaseAddrIn4GSpace):
    return int(AddressInBios, 16) + int(BiosBaseAddrIn4GSpace, 16)

class FSPT_UPD(Structure):
    _pack_ = 1
    _fields_ = [
        ('fsptUpdBin',         ARRAY(FSPT_UPD_LEN, c_uint8)),
    ]

class REGION_SEGMENT(Structure):
    _pack_ = 1
    _fields_ = [
        ('Reserved', c_uint16),
        ('Flags',    c_uint16),
        ('Base',     c_uint32),
        ('Size',     c_uint32),
    ]

    def dumpInfo(self):
        print("Flags:", self.Flags)
        print("Base:",  hex(self.Base))
        print("Size:",  hex(self.Size))

class FspRegionDigestHeader(Structure):
    _pack_ = 1
    _fields_ = [
        ('FspComponentId',            c_uint8),
        ('Size',                      c_uint16),
        ('Count',                     c_uint16)
    ]

FBM_COMPONENT_ID = {
    "TO": 0x0,
    "M" : 0X1,
    "S" : 0x2,
    "VS": 0xF,
    }

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
        ('FspSiliconInitEntryOffset', c_uint32),
        ('FspMultiPhaseSiInitEntryOffset', c_uint32),
        ('ExtendedImageRevision',     c_uint16)
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

def AlignPtr (offset, alignment = 8) -> int:
    return (offset + alignment - 1) & ~(alignment - 1)

def Bytes2Val (bytes):
    return reduce(lambda x,y: (x<<8)|y,  bytes[::-1] )

def Val2Bytes (value, blen):
    return [(value>>(i*8) & 0xff) for i in range(blen)]

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

class BSSSHeader(Structure):
    _pack_ = 1
    _fields_ = [
        ('StructureID',   ARRAY(8, c_uint8)),
        ('StructVersion', c_uint8),
        ('SizeOfElement', c_uint16),
        ('Reserved0',     c_uint8),
    ]

    def genBSSSHeader(self, dataSize):
        for i in range(len(BSSS_ID)):
            self.StructureID[i] = ord(BSSS_ID[i])
        self.StructVersion = BSSS_VERSION
        self.SizeOfElement = dataSize

    def dumpInfo(self):
        print("BSSS header:")
        StructureIdStr = ""
        for i in range(len(self.StructureID)):
            StructureIdStr += chr(self.StructureID[i])
        print("Structure ID:", StructureIdStr)
        print("Struct Version:", self.StructVersion)
        print("Size of Element:", self.SizeOfElement)
        print("Reserved value:", hex(self.Reserved0))
#
# Fields of BSP_CONFIG_STRUCTURE except IBB Segment array and BspDigestInfo
#
class BspConfigHead(Structure):
    _pack_ = 1
    _fields_ = [
        ('BspEntryPoint',          c_uint32),
        ('FsptUpd',                FSPT_UPD),
        ('FspmLoadingPolicy',      c_uint8),
        ('FspmBaseAddress',        c_uint32),
        ('TpmBaseAddress',         c_uint64),
        ('BspSegmentCount',        c_uint32),
        ('FspConfigRegSegmentCount',   c_uint32)
        ]

    def dumpInfo(self):
        print("BspEntryPoint:", hex(self.BspEntryPoint))
        print("FsptUpd:")
        cnt = 0
        fsptUpdStr = ""
        for i in range(sizeof(FSPT_UPD)):
            fsptUpdStr += " " + hex(self.FsptUpd.fsptUpdBin[i])[2:].zfill(2)
            cnt += 1
            if cnt % 16 == 0:
                print(hex(cnt - 16)[2:].zfill(2) + ":" + fsptUpdStr)
                fsptUpdStr = ""

        if cnt % 16 != 0:
            print(hex((cnt - cnt % 16))[2:].zfill(2) + ":" + fsptUpdStr)

        print("FspmLoadingPolicy:", hex(self.FspmLoadingPolicy))
        print("FspmBaseAddress:", hex(self.FspmBaseAddress))
        print("BspSegmentCount:", self.BspSegmentCount)
        print("FspConfigRegSegmentCount:", self.FspConfigRegSegmentCount)
        print("TpmBaseAddress:", hex(self.TpmBaseAddress))

    #
    # Get FSP-T Upd by searching FspUpdSearchKeyword and revision in PreMemory FV of client FD
    #
    def getFspTUpd(self, FdDir, FvPreMemoryOffset, FvPreMemorySize, BiosFlashBaseAddr):
        with open(FdDir, "rb") as Fd:
            FspUpdSearchKeyword = bytes(FSPT_UPD_SIGNATURE, 'utf-8') + bytes(c_uint8(FSP_PKG_REVISION))
            Cnt = int(FvPreMemoryOffset, 16)
            Fd.seek(Cnt, 1)
            ChunkSize = SEARCH_CHUNK_SIZE
            Chunk = Fd.read(ChunkSize)
            FsptUpdAddress = 0xFFFFFFFF
            MaxChunkAddr = int(FvPreMemorySize, 16) + Cnt

            #
            # Search FspUpdSearchKeyword per Chunk in PreMemory FV
            #
            while Chunk and Cnt < MaxChunkAddr:
                FsptUpdPos = Chunk.find(FspUpdSearchKeyword)
                if FsptUpdPos != -1:
                    FsptUpdAddress = Cnt + FsptUpdPos
                    print("Fspt Upd address in Client Bios is:", hex(FsptUpdAddress))
                Chunk = Fd.read(ChunkSize)
                Cnt = Cnt + ChunkSize

            fsptUpd = b''
            if FsptUpdAddress == 0xFFFFFFFF:
                print("Failed to find Fspt Upd Address in FvPreMemory, set Fspt Upd Address to 0x0")
                FsptUpdAddress = 0x0
            else:
                Fd.seek(FsptUpdAddress)
                fsptUpd = bytearray(Fd.read(FSPT_UPD_LEN))
                self.FsptUpd = FSPT_UPD.from_buffer(fsptUpd, 0)

class HashStructHead(Structure):
    _pack_ = 1
    _fields_ = [
        ('HashAlg', c_uint16),
        ('Size',    c_uint16),
        ]

class HashStruct:
    def __init__(self, HashAlgType, OpenSslCommand, fspData):
        self.HashStructHead = HashStructHead()
        self.HashStructHead.HashAlg = HASH_TYPE_VALUE[HashAlgType]
        self.HashStructHead.Size = HASH_DIGEST_SIZE[HashAlgType]

        print("self.HashStrHead.HashAlg", self.HashStructHead.HashAlg)
        command = '%s dgst -%s -binary' % (OpenSslCommand, HashAlgType)
        print("command:", command)
        Process = subprocess.Popen(command, stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
        res = bytearray()
        res = res + Process.communicate(input=fspData)[0]
        self.HashStructBuffer = res

    def getOverallLength(self):
        return sizeof(self.HashStructHead) + self.HashStructHead.Size

    def serializeToBin(self):
        hashStructBin = bytes(self.HashStructHead) + self.HashStructBuffer
        return hashStructBin

#
# BIOS Specific IBB Structure (BSIS) Header
#
class BSISHeader(Structure):
    _pack_ = 1
    _fields_ = [
        ('StructureID',   ARRAY(8, c_uint8)),
        ('StructVersion', c_uint8),
        ('Reserved',      c_uint8),
        ('SizeofData',    c_uint16),
    ]

    def genBSISHeader(self, dataSize):
        for i in range(len(BSIS_ID)):
            self.StructureID[i] = ord(BSIS_ID[i])
        self.StructVersion = BSIS_VERSION
        self.SizeofData = dataSize

    def dumpInfo(self):
        print("BSIS header:")
        StructureIdStr = ""
        for i in range(len(self.StructureID)):
            StructureIdStr += chr(self.StructureID[i])
        print("Structure ID:", StructureIdStr)
        print("Struct Version:", self.StructVersion)
        print("Size of Data:", self.SizeofData)
        print("Reserved value:", hex(self.Reserved))

class BspConfig():
    def __init__(self, flashMap, fdDir, fspmLP):
        self.BSISHeader = BSISHeader()
        self.BsssHeader = BSSSHeader()
        self.BspConfigHead = BspConfigHead()
        self.BspDigestList = []
        self.IbbSegmentList = []
        self.fdDir = fdDir
        self.FspRegionsList = []
        self.OpenSslCommand = None
        self.Fspfd = ''
        self.FspMFspSConfigRegDigestList = []
        self.FspMFspSConfigRegSegList = []
        self.header = FspRegionDigestHeader()

        self.BspConfigHead.TpmBaseAddress = TPM_BASE_ADDRESS
        self.BspConfigHead.FspmBaseAddress = ConvertTo4GSpaceAddr(flashMap.FvFspmOffset, flashMap.BiosFlashBaseAddr)
        self.BspConfigHead.FspmLoadingPolicy = int(fspmLP, 16)
        print("FspmLoadingPolicy", self.BspConfigHead.FspmLoadingPolicy)

        self.BspBaseAddress = ConvertTo4GSpaceAddr(flashMap.FvPreMemoryOffset, flashMap.BiosFlashBaseAddr)
        self.BspSize = int(flashMap.FvPreMemorySize, 16)

        #
        # Get the Sec Core Entry address of BSP
        #
        BspSecCoreEntryAddr = GetSecCoreEntry(fdDir, flashMap.FvPreMemoryOffset, flashMap.FvPreMemorySize)
        print("BspSecCoreEntryAddr:", BspSecCoreEntryAddr)
        if BspSecCoreEntryAddr == "":
            print("Error! Failed to find Bsp Sec Core Entry Address")
            sys.exit(1)
        else:
            self.BspConfigHead.BspEntryPoint = ConvertTo4GSpaceAddr(BspSecCoreEntryAddr, flashMap.BiosFlashBaseAddr)
            print("Bsp Sec Core Entry Address is:", hex(self.BspConfigHead.BspEntryPoint))

        self.BspConfigHead.getFspTUpd(fdDir, flashMap.FvPreMemoryOffset, flashMap.FvPreMemorySize, flashMap.BiosFlashBaseAddr)

    def genBsssBin(self, flashMap, fdDir, output):
        BsssHeader = BSSSHeader()
        BspConfigData = BspConfigHead()
        BsssHeader.genBSSSHeader(sizeof(BSSSHeader) + sizeof(BspConfigHead))
        #
        # Get the Sec Core Entry address of BSP
        #
        BspSecCoreEntryAddr = GetSecCoreEntry(fdDir, flashMap.FvPreMemoryOffset, flashMap.FvPreMemorySize)
        print("BspSecCoreEntryAddr:", BspSecCoreEntryAddr)
        if BspSecCoreEntryAddr == "":
            print("Error! Failed to find Bsp Sec Core Entry Address")
            sys.exit(1)
        else:
            BspConfigData.BspEntryPoint = ConvertTo4GSpaceAddr(BspSecCoreEntryAddr, flashMap.BiosFlashBaseAddr)
            print("Bsp Sec Core Entry Address is:", hex(BspConfigData.BspEntryPoint))

        BspConfigData.getFspTUpd(fdDir, flashMap.FvPreMemoryOffset, flashMap.FvPreMemorySize, flashMap.BiosFlashBaseAddr)

        bsssBin = bytes(BsssHeader) + bytes(BspConfigData)
        with open(output, "wb") as fw:
            fw.write(bsssBin)

        with open(fdDir, 'r+b') as fd:
            buffer = fd.read()
            fd.seek(len(buffer) - 0x1000)
            fd.write(bsssBin)

    ##################  Order of Components in BSIS.bin file ##############
    ##############            BSIS Header               ###############
    ##############            BSSS Header               ###############
    ##############            BSPM ELEMENT              ###############
    ##############            BSP SEGMENT LIST          ###############
    ##############            BSP DIGEST LIST           ###############
    ##############            FSP-M UPD REGION SEGMENT  ###############
    ##############            FSP-S UPD REGION SEGMENT  ###############
    ##############            FSP-M UPD REGION DIGEST   ###############
    ##############            FSP-S UPD REGION DIGEST   ###############
    ########################################################################
    def genBsisBin(self, BiosFlashBaseAddr, OpenSslCommand, output, FspSigned):
        if FspSigned:
            self.OpenSslCommand = OpenSslCommand
            self.genBspDgstList(BiosFlashBaseAddr, OpenSslCommand)
            self.genFspSAndMConfigRegionDigest()
            
        bsisBin = bytes(self.BspConfigHead)

        for i in range(self.BspConfigHead.BspSegmentCount):
            bsisBin += bytes(self.IbbSegmentList[i])

        for bspDigest in self.BspDigestList:
            bsisBin += bspDigest.serializeToBin()
        
        #
        # Add FSP-M and S Configuration Region Segment List and respective Digests at the END of Bsis.bin file
        #
        for i in range(self.BspConfigHead.FspConfigRegSegmentCount):
            bsisBin += bytes(self.FspMFspSConfigRegSegList[i])
        
        for fspCfgRegDigest in self.FspMFspSConfigRegDigestList:
            bsisBin += fspCfgRegDigest.serializeToBin()

        #
        # Add BSSS header
        #
        self.BsssHeader.genBSSSHeader(len(bsisBin))
        bsisBin = bytes(self.BsssHeader) + bsisBin

        #
        # Add BSIS header at the head of bsis.bin
        #
        self.BSISHeader.genBSISHeader(len(bsisBin))
        bsisBin = bytes(self.BSISHeader) + bsisBin

        if len(bsisBin) > BSIS_MAX_LENGTH:
            print("Error! BSIS binary length is larger than %d, which will cause BpmGen tool error" % BSIS_MAX_LENGTH)

        with open(output, "wb") as fw:
            fw.write(bsisBin)

    def genBspDgstList(self, flashBase, openSslCommand):
        bspDataToCmpDgst = b''
        with open(self.fdDir, "rb") as BiosFd:
            #
            # Generate digests according to segment in IbbSegmentList
            #
            ibbSeg = REGION_SEGMENT()
            ibbSeg.Base = self.BspBaseAddress
            ibbSeg.Size = self.BspSize
            self.IbbSegmentList.append(ibbSeg)
            self.BspConfigHead.BspSegmentCount = len(self.IbbSegmentList)

            idx = 0
            while idx < len(self.IbbSegmentList) and self.IbbSegmentList[idx].Size != 0:
                offset = self.IbbSegmentList[idx].Base - int(flashBase, 16)
                BiosFd.seek(offset)
                bspDataToCmpDgst += bytearray(BiosFd.read(self.IbbSegmentList[idx].Size))
                idx += 1

            for t in HASH_TYPE_VALUE:
                self.BspDigestList.append(HashStruct(t, openSslCommand, bspDataToCmpDgst))

    def genFspSAndMConfigRegionDigest(self):
        FspBinDir = os.getenv('WORKSPACE_FSP_BIN')
        # Fsp.fd file path
        self.Fspfd = os.path.join(FspBinDir, "PantherLakeFspBinPkg", "Fsp.fd")
        self.fd = FirmwareDevice(self.Fspfd)
        self.fd.ParseFd()
        self.fd.ParseFsp()
        self.CalFspSOrMRegionDigest()

    def CalFspSOrMRegionDigest(self) -> None:
        """
        Calculate digest lists for FSP-M and S Configuration Region

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
            self.getFspMOrSConfigRegionDigest(fspM)
        else:
            print("Error! FSP-M is None")
            sys.exit(1)

        if fspS is not None:
            self.getFspMOrSConfigRegionDigest(fspS)
        else:
            print("Error! FSP-S is None")
            sys.exit(1)
    
    def getFspMOrSConfigRegionDigest(self, fsp) -> None:
        """
        Calculate the digest list for FSP-M or FSP-S Configuartion Region.

        The digest calculation includes only the UPD region of FSP-M/S.

        Args:
            fsp (FspObject): The FSP-M or FSP-S object.

        """
        fspDataToCalDigest = b''
        with open(self.Fspfd, "rb") as fr:
            print("FSP-M/S CfgRegionOffset", fsp.Fih.CfgRegionOffset - fsp.Offset)
            print("FSP-M/S CfgRegionSize", fsp.Fih.CfgRegionSize)
            ibbSeg = REGION_SEGMENT()
            ibbSeg.Base = fsp.Fih.CfgRegionOffset - fsp.Offset
            ibbSeg.Size = fsp.Fih.CfgRegionSize
            self.FspMFspSConfigRegSegList.append(ibbSeg)
            self.BspConfigHead.FspConfigRegSegmentCount = len(self.FspMFspSConfigRegSegList)
            fr.seek(fsp.Fih.CfgRegionOffset, 0)
            fspDataToCalDigest += bytearray(fr.read(fsp.Fih.CfgRegionSize))

        for t in HASH_TYPE_VALUE:
            self.FspMFspSConfigRegDigestList.append(HashStruct(t, self.OpenSslCommand, fspDataToCalDigest))

def dumpHashStruct(bsisBin, offset):
    hashHead = HashStructHead.from_buffer(bsisBin, offset)
    hashAlgStr = ""
    for key in HASH_TYPE_VALUE:
        if HASH_TYPE_VALUE[key] == hashHead.HashAlg:
            hashAlgStr = key
            break
    print("Hash Algorithm:", hashAlgStr, " Algorithm ID:", hex(hashHead.HashAlg))
    print("Hash Size:", hex(hashHead.Size))

    hashBinStr = ""
    cnt = 0
    hashValueBuffer = bsisBin[offset + sizeof(HashStructHead):offset + sizeof(HashStructHead) + hashHead.Size]
    for b in hashValueBuffer:
        hashBinStr = hashBinStr + " " + hex(b)[2:].zfill(2)
        cnt += 1
        if cnt % 16 == 0:
            print(hex(cnt - 16)[2:].zfill(4) + ":" + hashBinStr)
            hashBinStr = ""
    if cnt % 16 > 0:
        print(hex((cnt - cnt % 16))[2:].zfill(4) + ":" + hashBinStr)
    return offset + sizeof(HashStructHead) + hashHead.Size

def dumBsis(bsisDir):
    print('##################################################')
    print("# BSIS (BIOS Specific IBB Structure) information #")
    print('##################################################')

    with open(bsisDir, "rb") as BsisFile:
        bsisBinSize = os.path.getsize(bsisDir)
        bsisData = bytearray(BsisFile.read(bsisBinSize))
    bsisHeader = BSISHeader.from_buffer(bsisData, 0)
    bsisHeader.dumpInfo()
    offset = sizeof(BSISHeader)
    bsssHeader = BSSSHeader.from_buffer(bsisData, offset)
    bsssHeader.dumpInfo()
    offset += sizeof(BSSSHeader)
    #
    # Bsis that only consists of headers will end at this point.
    #
    if offset == bsisBinSize:
        print("All BSIS info is printed")
        return

    bspConfigHead = BspConfigHead.from_buffer(bsisData, offset)
    bspConfigHead.dumpInfo()
    offset += sizeof(BspConfigHead)

    for i in range(bspConfigHead.BspSegmentCount):
        print("Segment Array[%d]" % i)
        ibbSeg = REGION_SEGMENT.from_buffer(bsisData, offset)
        ibbSeg.dumpInfo()
        offset += sizeof(REGION_SEGMENT)
        
    for i in range(bspConfigHead.BspSegmentCount):
        for t in HASH_TYPE_VALUE:
            offset = dumpHashStruct(bsisData, offset)

    for i in range(bspConfigHead.FspConfigRegSegmentCount):
        print("Segment Array[%d]" % i)
        ibbSeg = REGION_SEGMENT.from_buffer(bsisData, offset)
        ibbSeg.dumpInfo()
        offset += sizeof(REGION_SEGMENT)
    
    for i in range(bspConfigHead.FspConfigRegSegmentCount):
        for t in HASH_TYPE_VALUE:
            offset = dumpHashStruct(bsisData, offset)

#
# Bsis that only consists of headers
#
class EmptyBsis():
    def __init__(self):
        self.BSISHeader = BSISHeader()
        self.BsssHeader = BSSSHeader()

    def genBsis(self, output):
        bsisBin = b''

        #
        # Add BSSS header
        #
        self.BsssHeader.genBSSSHeader(len(bsisBin))
        bsisBin = bytes(self.BsssHeader) + bsisBin

        #
        # Add BSIS header at the head of bsis.bin
        #
        self.BSISHeader.genBSISHeader(len(bsisBin))
        bsisBin = bytes(self.BSISHeader) + bsisBin
        with open(output, "wb") as fw:
            fw.write(bsisBin)

def main():
    Parser = argparse.ArgumentParser(
                        prog = __prog__,\
                        description = __description__ + __copyright__
                        )
    Parser.add_argument("-Fd", "--FdDir", dest = 'FdDir',
                            help = 'Please input fd file directory')
    Parser.add_argument("-FspmLP", "--FspmLoadingPolicy", dest = 'FspmLoadingPolicy',
                            help = 'Please input Value of FspmLoadingPolicy in hex format(e.g. 0x1), \
                                    0: Load FSP-M, \
                                    1: Skip loading FSP-M')
    Parser.add_argument("-o", "--output", dest = 'Output',
                            help = "Please input the output file of BSIS.bin")
    Parser.add_argument("-p", "--patch", dest = 'PatchOutput',
                            help = "Please input the output file of FSPE.bin")
    Parser.add_argument("-d", "--dump", dest = 'Dump',
                            help = "Dump information of BSIS Binary")
    Parser.add_argument("-s", "--signed", dest = 'FspSigned',
                            help = "Indicate FSP Signed build")

    FlashmapFile = os.getenv('FLASHMAP_FDF')
    PrepRelease = os.getenv('PrepRelease')
    ExtendedRegionBuildFlag = os.getenv('EXTENDEDREGION_BUILD')
    FspReset = os.getenv('FSP_RESET')
    Args, _  = Parser.parse_known_args()

    if Args.FspSigned != None and Args.FspSigned == 'TRUE':
        FspSigned = True
    else:
        FspSigned = False

    if Args.Output != None:
        if FspReset.upper() == "FALSE":
            bsis = EmptyBsis()
            bsis.genBsis(Args.Output)
        else:
            Fm = Flashmap(FlashmapFile, PrepRelease, ExtendedRegionBuildFlag)
            Fm.ReadAllSettings()
            bspConfig = BspConfig(Fm, Args.FdDir, Args.FspmLoadingPolicy)
            OpenSslCommand = os.path.join(os.getenv('OPENSSL_PATH'), 'openssl.exe')
            bspConfig.genBsisBin(Fm.BiosFlashBaseAddr, OpenSslCommand, Args.Output, FspSigned)

    if Args.PatchOutput != None:
        Fm = Flashmap(FlashmapFile, PrepRelease, ExtendedRegionBuildFlag)
        Fm.ReadAllSettings()
        bspConfig = BspConfig(Fm, Args.FdDir, Args.FspmLoadingPolicy)
        bspConfig.genBsssBin(Fm, Args.FdDir, Args.PatchOutput)

    if Args.Dump != None:
        dumBsis(Args.Dump)

if __name__ == '__main__':
    main()