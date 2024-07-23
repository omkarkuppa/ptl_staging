## @file
#  Internal library support to generate the uCode capsule payload.
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
import uuid
from ctypes import *
from typing import List

from CapsuleCommon import *

from CapsuleGenerate.ApiPrivate.BgupGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdfGeneratorApiLib import *

from CapsuleGenerate.Firmwares.CommonLib import *

#
# BGSL template file.
#
UCODE_BGSL_OFFSET_KEY: str = 'RegionOffset'
UCODE_BGSL_SIZE_KEY  : str = 'RegionSize'

UCODE_FILE_PREFIX: str = 'Ucode'

class _UcodeFdfGeneratorApi (object):
    def __init__ (self, FvName: str, FvSize: int, FlashBlockSize: int) -> None:
        """ Class support to generate the FDF file for uCode FV.

        Args:
            FvName (str):
                Name of the FV.
            FvSize (int):
                Expected size of this FV.
            FlashBlockSize (int):
                Flash block size of the target platform.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FvName        : str = FvName
        self.__FvSize        : int = FvSize
        self.__FlashBlockSize: int = FlashBlockSize

        self.__PreCheck ()

        self.__UcodeCnt    : int                           = 0
        self.__FdfGenerator: CapsulePayloadFdfGeneratorApi = None

        self.__InitFdfFile ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FvName not str type.
                (2) FvSize not int type.
                (3) FlashBlockSize not int type.
            ValueError:
                FvSize should be multiply of FlashBlockSize.

        Returns:
            None.
        """
        if not isinstance (self.__FvName, str):
            raise TypeError ('FvName should be str type.')
        elif not isinstance (self.__FvSize, int):
            raise TypeError ('FvSize should be int type.')
        elif not isinstance (self.__FlashBlockSize, int):
            raise TypeError ('FlashBlockSize should be int type.')
        elif (self.__FvSize % self.__FlashBlockSize != 0):
            raise ValueError ('FvSize should be multiply of FlashBlockSize.')

    def __InitFdfFile (self) -> None:
        """ Initial the FDF file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FdfGenerator = \
            CapsulePayloadFdfGeneratorApi (
              FvName         = self.__FvName,
              FlashBlockSize = self.__FlashBlockSize,
              NumBlocks      = int (self.__FvSize / self.__FlashBlockSize),
              )

        self.__FdfGenerator.AddRawFile (
                              uuid.UUID (UCODE_VER_FILE_GUID),
                              UCODE_VER_FILE_NAME,
                              None,
                              )

    def AddUcodeFile (self, Buffer: ByteBuffer, Info: UcodeFwHdr) -> None:
        """ Expose the function to add the microcode into FDF file.

        Note:
            (1) It is caller responsibility to make sure the microcode
                FV have enough capacity.
            (2) First uCode is 4K alignment, remains is 16 alignment.

        Args:
            Buffer (ByteBuffer):
                Buffer of the uCode.
            Info (UcodeFwHdr):
                Header information of the uCode.

        Raises:
            None.

        Returns:
            None.
        """
        Alignment: str = None
        FileName : str = os.path.basename (Buffer.Path)

        #
        # uCode alignment.
        #
        if self.__UcodeCnt == 0:
            Alignment = FFS_ALIGNMENT_4K
        else:
            Alignment = FFS_ALIGNMENT_16

        FileName = f'{self.__UcodeCnt}_{FileName}'
        Buffer.Save (JoinPath (EDKII_WORKSPACE_PATH, FileName))

        self.__FdfGenerator.AddRawFile (
                              uuid.UUID (UCODE_IMG_FILE_GUID),
                              FileName,
                              Alignment = Alignment,
                              )

        self.__UcodeCnt += 1

    def Save (self, FilePath: Union[str, os.PathLike]):
        """ Expose the function to save the FDF file into storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path of the file to be saved into the storage.

        Raises:
            None.

        Returns:
            None.
        """
        Path: str = AbsPath (FilePath)
        self.__FdfGenerator.Save (Path)

class _UcodeBgupGeneratorApi (object):
    def __init__ (
        self,
        FvOffset        : int,
        FvSize          : int,
        FvDataPath      : Union[str, os.PathLike],
        BgslTemplatePath: Union[str, os.PathLike],
        BiosSvn         : str,
        ) -> None:
        """ Class support to generate the signed BGUP for update region.

        Args:
            FvOffset (int):
                Offset of the region to be program.
            FvSize (int):
                Length of the region to be program.
            FvDataPath (Union[str, os.PathLike]):
                The path of uCode capsule payload to be delivered into
                BiosGuard to use for programming the region.
            BgslTemplatePath (Union[str, os.PathLike]):
                The path of BGSL template file.
            BiosSvn (str):
                The value of BIOS SVN for this signed BGUP.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FvOffset        : int = FvOffset
        self.__FvSize          : int = FvSize
        self.__FvDataPath      : str = AbsPath (FvDataPath)
        self.__BgslTemplatePath: str = AbsPath (BgslTemplatePath)
        self.__BiosSvn         : str = BiosSvn

        self.__PreCheck ()

        self.__BgupBuffer: ByteBuffer = self.__GetBgupBuffer ()

    @property
    def Image (self) -> ByteBuffer:
        """ Return the BGUP image buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Buffer of the BGUP image.
        """
        return self.__BgupBuffer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FvOffset not int type.
                (2) FvSize not int type.
                (3) BiosSvn not str type.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__FvDataPath,
            self.__BgslTemplatePath,
            ]

        CheckFileListExist (CheckPathList)

        if not isinstance (self.__FvOffset, int):
            raise TypeError ('FvOffset should be int type.')
        elif not isinstance (self.__FvSize, int):
            raise TypeError ('FvSize should be int type.')
        elif not isinstance (self.__BiosSvn, str):
            raise TypeError ('BiosSvn should be str type.')

    def __GenerateBgslFile (self) -> File:
        """ Return the BGSL for uCode capsule payload.

        Note:
            This function is used to fill in the base address and
            size into BGSL template file.

        Args:
            None.

        Raises:
            None.

        Returns:
            File:
                File object for BGSL script content.
        """
        BgslFile: File = File (FilePath = self.__BgslTemplatePath)
        Config  : dict = {
            UCODE_BGSL_OFFSET_KEY:
                FormatHex (self.__FvOffset, IsPadding = True),
            UCODE_BGSL_SIZE_KEY  :
                FormatHex (self.__FvSize, IsPadding = True),
            }

        BgslFile.Fill (**Config)

        return BgslFile

    def __GenerateBgupImg (self, BgslPath: str) -> ByteBuffer:
        """ Return the BGUP buffer for uCode capsule payload.

        Args:
            BgslPath (str):
                Path of the BGSL script to be used to build.

        Raises:
            None.

        Returns:
            ByteBuffer:
                BGUP buffer for uCode capsule payload.
        """
        Generator = BgupGenerateApi (
                      FileName     = CombineFileName (
                                       UCODE_FILE_PREFIX,
                                       Extension = EXTENSION_BGUP_FILE,
                                       ),
                      BgslPath     = BgslPath,
                      PlatformName = BIOSGUARD_PLATFORM_ID,
                      BiosSvn      = self.__BiosSvn,
                      Prefix       = UCODE_FILE_PREFIX,
                      DataPath     = self.__FvDataPath,
                      )
        Generator.Build ()

        return ByteBuffer (FilePath = Generator.FilePath)

    def __GetBgupBuffer (self) -> ByteBuffer:
        """ Internal function to build the BGUP.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Buffer for the BGUP image.
        """
        TouchDir (BIOSGUARD_BGSL_DIR)
        TouchDir (BIOSGUARD_BGUP_DIR)

        BgslFile: File = self.__GenerateBgslFile ()
        BgslPath: str  = JoinPath (
                           BIOSGUARD_BGSL_DIR,
                           CombineFileName (
                             UCODE_FILE_PREFIX,
                             Extension = EXTENSION_BGSL_FILE,
                             ),
                           )
        BgslFile.Save (BgslPath)

        BgupBuffer: ByteBuffer = self.__GenerateBgupImg (BgslPath)

        return BgupBuffer

    def Save (self, FilePath: Union[str, os.PathLike]) -> None:
        """ Expose the function to save the buffer into storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path of the file to be saved into the storage.

        Raises:
            None.

        Returns:
            None.
        """
        Path: str = AbsPath (FilePath)
        self.Image.Save (Path)

class _UcodeVerHdrGeneratorApi (object):
    def __init__ (
        self,
        Version: int,
        Lsv    : int,
        VerStr : str,
        ) -> None:
        """ Class support to generate the uCode version string header.

        Args:
            Version (int):
                Version of this uCode capsule. (UINT32)
            Lsv (int):
                LSV of this uCode capsule. (UINT32)
            VerStr (str):
                Unicode encoded string for version string.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Version: int = Version
        self.__Lsv    : int = Lsv
        self.__VerStr : str = VerStr

        self.__PreCheck ()

        #
        # Add one '00' '00' as the terminate word.
        #
        self.__VerStrLen: int        = len (self.__VerStr) + 1
        self.__HdrBuffer: ByteBuffer = self.__GetHdrBuffer ()

    @property
    def Image (self) -> ByteBuffer:
        """ Return the uCode version string header buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Buffer of the uCode version string header.
        """
        return self.__HdrBuffer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Version not int type.
                (2) LSV not int type.
                (3) VersionString not str type.
            ValueError:
                (1) Version not UINT32.
                (2) LSV not UINT32.

        Returns:
            None.
        """
        if not isinstance (self.__Version, int):
            raise TypeError ('Version should be int type.')
        elif not isinstance (self.__Lsv, int):
            raise TypeError ('LSV should be int type.')
        elif not isinstance (self.__VerStr, str):
            raise TypeError ('VersionString should be str type.')
        elif not IsUint32 (self.__Version):
            raise ValueError ('Version should be UINT32.')
        elif not IsUint32 (self.__Lsv):
            raise ValueError ('LSV should be UINT32.')

    def __XdrHeaderGen (self, LenOfStr: int):
        """ Return the uCode version header.

        Note:
            Since the length of VersionString is unknown, need to keep
            it as an input parameter for dynamic purpose.

        Args:
            LenOfStr (int):
                The length of the version string within the header.

        Raises:
            None.

        Returns:
            _UcodeVersionHeader:
                Header of the uCode.
                (VersionString length is depend on the input string)
        """
        class _UcodeVersionHeader (LittleEndianStructure):
            _pack_  : int  = 1
            _fields_: list = [
                ('Version',                c_uint32),
                ('LowestSupportedVersion', c_uint32),
                ('VersionString',          c_wchar * LenOfStr),
                ]

        return _UcodeVersionHeader ()

    def __GetHdrBuffer (self) -> ByteBuffer:
        """ Internal function to build uCode version header buffer.

        Note:
            The header buffer is little endian order.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Buffer of the uCode version string header.
        """
        Buffer: ByteBuffer = None
        Header             = self.__XdrHeaderGen (LenOfStr = self.__VerStrLen)

        #
        # Fill in the header information.
        #
        Header.Version                = self.__Version
        Header.LowestSupportedVersion = self.__Lsv
        Header.VersionString          = self.__VerStr

        #
        # Convert into ByteBuffer.
        #
        Buffer = ByteBuffer (Buffer = bytearray (Header))

        return Buffer

    def Save (self, FilePath: Union[str, os.PathLike]) -> None:
        """ Expose the function to save the buffer into storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path of the file to be saved into the storage.

        Raises:
            None.

        Returns:
            None.
        """
        Path: str = AbsPath (FilePath)
        self.Image.Save (Path)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    '_UcodeFdfGeneratorApi',
    '_UcodeBgupGeneratorApi',
    '_UcodeVerHdrGeneratorApi',
    ]
