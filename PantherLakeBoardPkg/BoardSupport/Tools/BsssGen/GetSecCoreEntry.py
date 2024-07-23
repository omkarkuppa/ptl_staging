## @file
#  Get Sec Core Entry tool.
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

from ctypes import *
from functools import reduce

class GUID(Structure):
    _fields_ = [
        ('Guid1',            c_uint32),
        ('Guid2',            c_uint16),
        ('Guid3',            c_uint16),
        ('Guid4',            ARRAY(c_uint8, 8)),
        ]

class EFI_FV_BLOCK_MAP_ENTRY(Structure):
    _pack_ = 1
    _fields_ = [
        ('NumBlocks',            c_uint32),
        ('Length',               c_uint32),
    ]

class EFI_FIRMWARE_VOLUME_HEADER(Structure):
    _fields_ = [
        ('ZeroVector',           ARRAY(c_uint8, 16)),
        ('FileSystemGuid',       GUID),
        ('FvLength',             c_uint64),
        ('Signature',            c_uint32),
        ('Attributes',           c_uint32),
        ('HeaderLength',         c_uint16),
        ('Checksum',             c_uint16),
        ('ExtHeaderOffset',      c_uint16),
        ('Reserved',             c_uint8),
        ('Revision',             c_uint8),
        ]

class c_uint24(Structure):
    """Little-Endian 24-bit Unsigned Integer"""
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

class EFI_COMMON_SECTION_HEADER(Structure):
    _fields_ = [
        ('Size',                 c_uint24),
        ('Type',                 c_uint8)
        ]

#
# File Types Definitions
#
EFI_FV_FILETYPE_ALL                   = 0x00
EFI_FV_FILETYPE_RAW                   = 0x01
EFI_FV_FILETYPE_FREEFORM              = 0x02
EFI_FV_FILETYPE_SECURITY_CORE         = 0x03
EFI_FV_FILETYPE_PEI_CORE              = 0x04
EFI_FV_FILETYPE_DXE_CORE              = 0x05
EFI_FV_FILETYPE_PEIM                  = 0x06
EFI_FV_FILETYPE_DRIVER                = 0x07
EFI_FV_FILETYPE_COMBINED_PEIM_DRIVER  = 0x08
EFI_FV_FILETYPE_APPLICATION           = 0x09
EFI_FV_FILETYPE_SMM                   = 0x0A
EFI_FV_FILETYPE_FIRMWARE_VOLUME_IMAGE = 0x0B
EFI_FV_FILETYPE_COMBINED_SMM_DXE      = 0x0C
EFI_FV_FILETYPE_SMM_CORE              = 0x0D
EFI_FV_FILETYPE_OEM_MIN               = 0xc0
EFI_FV_FILETYPE_OEM_MAX               = 0xdf
EFI_FV_FILETYPE_DEBUG_MIN             = 0xe0
EFI_FV_FILETYPE_DEBUG_MAX             = 0xef
EFI_FV_FILETYPE_FFS_MIN               = 0xf0
EFI_FV_FILETYPE_FFS_MAX               = 0xff
EFI_FV_FILETYPE_FFS_PAD               = 0xf0

#
# Leaf section Type values.
#
EFI_SECTION_PE32                  = 0x10
EFI_SECTION_PIC                   = 0x11
EFI_SECTION_TE                    = 0x12
EFI_SECTION_DXE_DEPEX             = 0x13
EFI_SECTION_VERSION               = 0x14
EFI_SECTION_USER_INTERFACE        = 0x15
EFI_SECTION_COMPATIBILITY16       = 0x16
EFI_SECTION_FIRMWARE_VOLUME_IMAGE = 0x17
EFI_SECTION_FREEFORM_SUBTYPE_GUID = 0x18
EFI_SECTION_RAW                   = 0x19
EFI_SECTION_PEI_DEPEX             = 0x1B
EFI_SECTION_SMM_DEPEX             = 0x1C

class EFI_FIRMWARE_VOLUME_HEADER(Structure):
    _fields_ = [
        ('ZeroVector',           ARRAY(c_uint8, 16)),
        ('FileSystemGuid',       ARRAY(c_uint8, 16)),
        ('FvLength',             c_uint64),
        ('Signature',            ARRAY(c_char, 4)),
        ('Attributes',           c_uint32),
        ('HeaderLength',         c_uint16),
        ('Checksum',             c_uint16),
        ('ExtHeaderOffset',      c_uint16),
        ('Reserved',             c_uint8),
        ('Revision',             c_uint8)
        ]

class Section:
    def __init__(self, offset, secdata):
        self.SecHdr   = EFI_COMMON_SECTION_HEADER.from_buffer (secdata, 0)
        self.SecData  = secdata[0:int(self.SecHdr.Size)]
        self.Offset   = offset

class EFI_FIRMWARE_VOLUME_EXT_HEADER(Structure):
    _fields_ = [
        ('FvName',               ARRAY(c_uint8, 16)),
        ('ExtHeaderSize',        c_uint32)
        ]

class EFI_FFS_INTEGRITY_CHECK(Structure):
    _fields_ = [
        ('Header',               c_uint8),
        ('File',                 c_uint8)
        ]

class EFI_FFS_FILE_HEADER(Structure):
    _fields_ = [
        ('Name',                 ARRAY(c_uint8, 16)),
        ('IntegrityCheck',       EFI_FFS_INTEGRITY_CHECK),
        ('Type',                 c_uint8),
        ('Attributes',           c_uint8),
        ('Size',                 c_uint24),
        ('State',                c_uint8)
        ]

class EFI_IMAGE_DATA_DIRECTORY(Structure):
    _fields_ = [
        ('VirtualAddress',       c_uint32),
        ('Size',                 c_uint32)
        ]

class EFI_TE_IMAGE_HEADER(Structure):
    _fields_ = [
        ('Signature',            ARRAY(c_char, 2)),
        ('Machine',              c_uint16),
        ('NumberOfSections',     c_uint8),
        ('Subsystem',            c_uint8),
        ('StrippedSize',         c_uint16),
        ('AddressOfEntryPoint',  c_uint32),
        ('BaseOfCode',           c_uint32),
        ('ImageBase',            c_uint64),
        ('DataDirectoryBaseReloc',  EFI_IMAGE_DATA_DIRECTORY),
        ('DataDirectoryDebug',      EFI_IMAGE_DATA_DIRECTORY)
        ]

class EFI_IMAGE_DOS_HEADER(Structure):
    _fields_ = [
        ('e_magic',              c_uint16),
        ('e_cblp',               c_uint16),
        ('e_cp',                 c_uint16),
        ('e_crlc',               c_uint16),
        ('e_cparhdr',            c_uint16),
        ('e_minalloc',           c_uint16),
        ('e_maxalloc',           c_uint16),
        ('e_ss',                 c_uint16),
        ('e_sp',                 c_uint16),
        ('e_csum',               c_uint16),
        ('e_ip',                 c_uint16),
        ('e_cs',                 c_uint16),
        ('e_lfarlc',             c_uint16),
        ('e_ovno',               c_uint16),
        ('e_res',                ARRAY(c_uint16, 4)),
        ('e_oemid',              c_uint16),
        ('e_oeminfo',            c_uint16),
        ('e_res2',               ARRAY(c_uint16, 10)),
        ('e_lfanew',             c_uint16)
        ]

class EFI_IMAGE_FILE_HEADER(Structure):
    _fields_ = [
        ('Machine',               c_uint16),
        ('NumberOfSections',      c_uint16),
        ('TimeDateStamp',         c_uint32),
        ('PointerToSymbolTable',  c_uint32),
        ('NumberOfSymbols',       c_uint32),
        ('SizeOfOptionalHeader',  c_uint16),
        ('Characteristics',       c_uint16)
        ]

class EFI_IMAGE_OPTIONAL_HEADER32(Structure):
    _fields_ = [
        ('Magic',                         c_uint16),
        ('MajorLinkerVersion',            c_uint8),
        ('MinorLinkerVersion',            c_uint8),
        ('SizeOfCode',                    c_uint32),
        ('SizeOfInitializedData',         c_uint32),
        ('SizeOfUninitializedData',       c_uint32),
        ('AddressOfEntryPoint',           c_uint32),
        ('BaseOfCode',                    c_uint32),
        ('BaseOfData',                    c_uint32),
        ('ImageBase',                     c_uint32),
        ('SectionAlignment',              c_uint32),
        ('FileAlignment',                 c_uint32),
        ('MajorOperatingSystemVersion',   c_uint16),
        ('MinorOperatingSystemVersion',   c_uint16),
        ('MajorImageVersion',             c_uint16),
        ('MinorImageVersion',             c_uint16),
        ('MajorSubsystemVersion',         c_uint16),
        ('MinorSubsystemVersion',         c_uint16),
        ('Win32VersionValue',             c_uint32),
        ('SizeOfImage',                   c_uint32),
        ('SizeOfHeaders',                 c_uint32),
        ('CheckSum'     ,                 c_uint32),
        ('Subsystem',                     c_uint16),
        ('DllCharacteristics',            c_uint16),
        ('SizeOfStackReserve',            c_uint32),
        ('SizeOfStackCommit' ,            c_uint32),
        ('SizeOfHeapReserve',             c_uint32),
        ('SizeOfHeapCommit' ,             c_uint32),
        ('LoaderFlags'     ,              c_uint32),
        ('NumberOfRvaAndSizes',           c_uint32),
        ('DataDirectory',                 ARRAY(EFI_IMAGE_DATA_DIRECTORY, 16))
        ]

class EFI_IMAGE_OPTIONAL_HEADER32_PLUS(Structure):
    _fields_ = [
        ('Magic',                         c_uint16),
        ('MajorLinkerVersion',            c_uint8),
        ('MinorLinkerVersion',            c_uint8),
        ('SizeOfCode',                    c_uint32),
        ('SizeOfInitializedData',         c_uint32),
        ('SizeOfUninitializedData',       c_uint32),
        ('AddressOfEntryPoint',           c_uint32),
        ('BaseOfCode',                    c_uint32),
        ('ImageBase',                     c_uint64),
        ('SectionAlignment',              c_uint32),
        ('FileAlignment',                 c_uint32),
        ('MajorOperatingSystemVersion',   c_uint16),
        ('MinorOperatingSystemVersion',   c_uint16),
        ('MajorImageVersion',             c_uint16),
        ('MinorImageVersion',             c_uint16),
        ('MajorSubsystemVersion',         c_uint16),
        ('MinorSubsystemVersion',         c_uint16),
        ('Win32VersionValue',             c_uint32),
        ('SizeOfImage',                   c_uint32),
        ('SizeOfHeaders',                 c_uint32),
        ('CheckSum'     ,                 c_uint32),
        ('Subsystem',                     c_uint16),
        ('DllCharacteristics',            c_uint16),
        ('SizeOfStackReserve',            c_uint64),
        ('SizeOfStackCommit' ,            c_uint64),
        ('SizeOfHeapReserve',             c_uint64),
        ('SizeOfHeapCommit' ,             c_uint64),
        ('LoaderFlags'     ,              c_uint32),
        ('NumberOfRvaAndSizes',           c_uint32),
        ('DataDirectory',                 ARRAY(EFI_IMAGE_DATA_DIRECTORY, 16))
        ]

class EFI_IMAGE_OPTIONAL_HEADER(Union):
    _fields_ = [
        ('PeOptHdr',             EFI_IMAGE_OPTIONAL_HEADER32),
        ('PePlusOptHdr',         EFI_IMAGE_OPTIONAL_HEADER32_PLUS)
        ]

class EFI_IMAGE_NT_HEADERS32(Structure):
    _fields_ = [
        ('Signature',            c_uint32),
        ('FileHeader',           EFI_IMAGE_FILE_HEADER),
        ('OptionalHeader',       EFI_IMAGE_OPTIONAL_HEADER)
        ]

class EFI_IMAGE_DIRECTORY_ENTRY:
    EXPORT                     = 0
    IMPORT                     = 1
    RESOURCE                   = 2
    EXCEPTION                  = 3
    SECURITY                   = 4
    BASERELOC                  = 5
    DEBUG                      = 6
    COPYRIGHT                  = 7
    GLOBALPTR                  = 8
    TLS                        = 9
    LOAD_CONFIG                = 10

def ParsePeTeImage(Data):
    TeHdr          = EFI_TE_IMAGE_HEADER.from_buffer (Data, 0)
    if TeHdr.Signature == b'VZ': # TE image
        EntryPoint   = TeHdr.AddressOfEntryPoint + sizeof (EFI_TE_IMAGE_HEADER) - TeHdr.StrippedSize;

    elif TeHdr.Signature == b'MZ': # PE image
        DosHdr  = EFI_IMAGE_DOS_HEADER.from_buffer (Data, 0)
        PeHdr   = EFI_IMAGE_NT_HEADERS32.from_buffer (Data, DosHdr.e_lfanew)
        if PeHdr.Signature != 0x4550:
            raise Exception("ERROR: Invalid PE32 header !")
        if PeHdr.OptionalHeader.PeOptHdr.Magic == 0x10b: # PE32 image
            if PeHdr.FileHeader.SizeOfOptionalHeader < EFI_IMAGE_OPTIONAL_HEADER32.DataDirectory.offset:
                raise Exception("ERROR: Unsupported PE32 image !")
            if PeHdr.OptionalHeader.PeOptHdr.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY.BASERELOC:
                raise Exception("ERROR: No relocation information available !")

        elif PeHdr.OptionalHeader.PeOptHdr.Magic == 0x20b: # PE32+ image
            if PeHdr.FileHeader.SizeOfOptionalHeader < EFI_IMAGE_OPTIONAL_HEADER32_PLUS.DataDirectory.offset:
                raise Exception("ERROR: Unsupported PE32+ image !")
            if PeHdr.OptionalHeader.PePlusOptHdr.NumberOfRvaAndSizes <= EFI_IMAGE_DIRECTORY_ENTRY.BASERELOC:
                raise Exception("ERROR: No relocation information available !")
        else:
            raise Exception("ERROR: Invalid PE32 optional header !")
        EntryPoint   = PeHdr.OptionalHeader.PeOptHdr.AddressOfEntryPoint;

    else:
        raise Exception("Unknown magic number in the DOS header")
    return EntryPoint

class FirmwareFile:
    def __init__(self, offset, filedata):
        self.FfsHdr   = EFI_FFS_FILE_HEADER.from_buffer (filedata, 0)
        self.FfsData  = filedata[0:int(self.FfsHdr.Size)]
        self.Offset   = offset
        self.Cnt      = 0

    #
    # Get Section EFI_SECTION_PE32 or EFI_SECTION_TE Section in Firmware file
    # PE and TE section is a leaf section in Firmware file
    #
    def GetSectionPeOrTe(self, SectionFileType, Instance):
        FfsSize = len(self.FfsData)
        Offset  = sizeof(self.FfsHdr)
        if self.FfsHdr.Name != '\xff' * 16:
            while Offset < (FfsSize - sizeof (EFI_COMMON_SECTION_HEADER)):
                SecHdr = EFI_COMMON_SECTION_HEADER.from_buffer (self.FfsData, Offset)
                sec = Section (Offset, self.FfsData[Offset:Offset + int(SecHdr.Size)])
                if SecHdr.Type == SectionFileType:
                    self.Cnt += 1
                if self.Cnt == Instance:
                    x = ParsePeTeImage(sec.SecData[sizeof(EFI_COMMON_SECTION_HEADER):])
                    return x + sizeof(EFI_COMMON_SECTION_HEADER) + Offset
                Offset += int(SecHdr.Size)
                Offset  = AlignPtr(Offset, 4)
        return None

class FirmwareVolume:
    def __init__(self, Offset, FvData):
        self.FvHdr    = EFI_FIRMWARE_VOLUME_HEADER.from_buffer (FvData, 0)
        self.FvData   = FvData[0 : self.FvHdr.FvLength]
        self.Offset   = Offset
        if self.FvHdr.ExtHeaderOffset > 0:
            self.FvExtHdr = EFI_FIRMWARE_VOLUME_EXT_HEADER.from_buffer (self.FvData, self.FvHdr.ExtHeaderOffset)
        else:
            self.FvExtHdr = None

    def GetFileByType(self, FfsFileType, Instance):
        FvSize = len(self.FvData)
        if self.FvExtHdr:
            Offset = self.FvHdr.ExtHeaderOffset + self.FvExtHdr.ExtHeaderSize
        else:
            Offset = self.FvHdr.HeaderLength
        Offset = AlignPtr(Offset)
        Cnt = 0
        while Offset < (FvSize - sizeof (EFI_FFS_FILE_HEADER)):
            FfsHdr = EFI_FFS_FILE_HEADER.from_buffer (self.FvData, Offset)
            if (FfsHdr.Name == '\xff' * 16) and (int(FfsHdr.Size) == 0xFFFFFF):
                Offset = FvSize
            else:
                Ffs = FirmwareFile (Offset, self.FvData[Offset:Offset + int(FfsHdr.Size)])
                if FfsHdr.Type == FfsFileType:
                    Cnt += 1
                if Cnt == Instance:
                    OffsetInSection = Ffs.GetSectionPeOrTe(EFI_SECTION_PE32, 1)
                    if OffsetInSection == None:
                        OffsetInSection = Ffs.GetSectionPeOrTe(EFI_SECTION_TE, 1)
                    if OffsetInSection == None:
                        raise Exception("ERROR: couldn't find PE32+ image in FFS!")
                    #
                    # Physical address is FV base + offset of PE32 + offset of the entry point
                    #
                    return OffsetInSection + Offset
                Offset += int(FfsHdr.Size)
                Offset = AlignPtr(Offset)
        raise Exception("ERROR: couldn't find Sec Core in FV!")

def AlignPtr (Offset, Alignment = 8):
    return (Offset + Alignment - 1) & ~(Alignment - 1)

def Bytes2Val (bytes):
    return reduce(lambda x,y: (x<<8)|y,  bytes[::-1] )

def GetSecCoreEntry(FdDir, FvAddress, FvLen):
    Fd = open(FdDir, "rb")
    Fd.seek(int(FvAddress, 16), 1)
    Len = int(FvLen, 16)
    FvData = Fd.read(Len)
    Fd.close()
    FvBytes = bytearray(FvData)
    Fv = FirmwareVolume (0, FvBytes)
    Res = Fv.GetFileByType(EFI_FV_FILETYPE_SECURITY_CORE, 1)
    SecCoreEntryAddress = hex(Res + int(FvAddress, 16))
    print ("Sec Core entry address in FD is:", SecCoreEntryAddress)
    return SecCoreEntryAddress