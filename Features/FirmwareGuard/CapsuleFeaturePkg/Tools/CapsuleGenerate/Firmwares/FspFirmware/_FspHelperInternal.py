## @file
#  Internal library support to generate the FSP capsule payload.
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

import uuid
from typing import List

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate.BgupGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2DscGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdfGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdGeneratorApiLib import *

from CapsuleGenerate.Firmwares.FspFirmware._SplitFspBin import *
from CapsuleGenerate.Firmwares.CommonLib import *

FSP_REVISION: int = 3
FSP_RV      : str = 'FspRV'

#
# Region flash order of different FSP image type.
#
FSP_UPDATE_FLASH_ORDER: List[str] = [
    FSP_S,
    FSP_M,
    FSP_T,
    FSP_O,
    FSP_RV,
    FBM,
    ]

#
# List of all target BGUP flash region.
#
BGUP_REGION_ORDER: List[str] = [
    FSP_S,
    FSP_M,
    FSP_T,
    FSP_O,
    FSP_RV,
    FBM,
    ]

#
# Directory to split to FSP image.
#
FSP_REGION_DIR_NAME: str = 'Fsp'
FSP_REGION_DIR_PATH: str = \
    JoinPath (
        TEMPORARY_DIR_PATH,
        FSP_REGION_DIR_NAME,
        )

FSP_COMPRESS_PKG_NAME  : str = 'FspCompressPkg'
FSP_COMPRESS_PKG_GUID  : str = '9C0F7E5A-4B6D-4C0E-8F1D-3B9E7A6F4E2B'

FSPCOMPRESS_FV_DIR_PATH: str = \
    JoinPath (
        EDKII_WORKSPACE_PATH,
        EDKII_BLD_DIR_NAME,
        FSP_COMPRESS_PKG_NAME,
        EDKII_BLD_TARGET_DIR_NAME,
        EDKII_BLD_FV_DIR_NAME,
        )

COMPRESSION_GUID_KEY    : str = 'CompressionGuid'
COMPRESS_REGIONSIZE_KEY : str = 'RegionSize'
COMPRESS_FVNAMEGUID_KEY : str = 'FvNameGuid'
COMPRESS_FVIMAGEGUID_KEY: str = 'FvImageGuid'

FSP_COMPRESS_FDF_TEMPLATE_FILE_NAME: str = 'PayloadCompressPkg.fdf.template'
FSP_COMPRESS_FDF_FILE_NAME         : str = 'FspCompressPkg.fdf'
FSP_COMPRESS_DSC_FILE_NAME         : str = 'FspCompressPkg.dsc'

FSP_COMPRESS_FDF_TEMPLATE_PATH     : str = \
    JoinPath (
        TEMPLATE_DIR_PATH,
        FSP_COMPRESS_FDF_TEMPLATE_FILE_NAME
        )
FSP_COMPRESS_FDF_PATH              : str = \
    JoinPath (
        TEMPORARY_DIR_PATH,
        FSP_COMPRESS_FDF_FILE_NAME
        )
FSP_COMPRESS_DSC_PATH              : str = \
    JoinPath (
        TEMPORARY_DIR_PATH,
        FSP_COMPRESS_DSC_FILE_NAME
        )

#
# BGSL template file.
#
FSP_BGSL_OFFSET_KEY: str = 'RegionOffset'
FSP_BGSL_SIZE_KEY  : str = 'RegionSize'

class _FspCompress (object):
    def __init__ (self) -> None:
        """ Create FSP compress target folder.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """

        TouchDir (FSPCOMPRESS_FV_DIR_PATH)

    def CompressImage (
            self,
            FvName         : str,
            CompressionGuid: str,
            RegionSize     : int,
            BlockSize      : int,
            FvNameGuid     : str,
            FvImageGuid    : str,
            FspFilePath    : Union[str, os.PathLike]
            ) -> None:
        """ Compress FSP image.

        Args:
            FvName (str):
                Fv Name.
            CompressionGuid (str):
                Compression algorithm Guid.
            RegionSize (int):
                FV size.
            BlockSize (int):
                Flash block size.
            FvNameGuid (str):
                Fv Name Guid.
            FvImageGuid (str):
                Fv Image Guid.
            FspFilePath (Union[str, os.PathLike]):
                FSP raw file for compression.

        Raises:
            None.

        Returns:
            None.
        """
        Edk2FvBuilder         : Edk2FdGeneratorApi      = None
        self.__FvName         : str                     = FvName
        self.__CompressionGuid: str                     = CompressionGuid
        self.__RegionSize     : int                     = RegionSize
        self.__BlockSize      : int                     = BlockSize
        self.__FvNameGuid     : str                     = FvNameGuid
        self.__FvImageGuid    : str                     = FvImageGuid
        self.__FspFilePath    : Union[str, os.PathLike] = FspFilePath

        self.__PreCheckInput ()

        self.__GenerateFdfFile ()

        self.__GenerateDscFile ()

        Edk2FvBuilder = Edk2FdGeneratorApi (
                            FvName,
                            DscFilePath = FSP_COMPRESS_DSC_PATH,
                            FdfFilePath = FSP_COMPRESS_FDF_PATH
                            )
        Edk2FvBuilder.Execute ()

        CompressedFvName = CombineFileName (
                               FvName,
                               Extension = EXTENSION_FV_FILE
                               )
        CompressedFvPath = JoinPath (FSPCOMPRESS_FV_DIR_PATH, CompressedFvName)

        CopyFile (CompressedFvPath, FspFilePath)

    def __PreCheckInput (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1)  FvName type is not str.
                (2)  CompressionGuid type is not str.
                (3)  RegionSize type is not int.
                (4)  BlockSize type is not int.
                (5)  FvNameGuid type is not str.
                (6)  FvImageGuid type is not str.
                (7)  FspFilePath type is not str or os.PathLike.

            ValueError:
                (1) The FspFile assigned path is not exist.
                (2) The FspFile assigned path is not file.

        Returns:
            None.
        """
        if not isinstance (self.__FvName, str):
            raise TypeError (f'Please assign the str type input for FvName.')
        elif not isinstance (self.__CompressionGuid, str):
            raise TypeError (f'Please assign the str type input for CompressionGuid.')
        elif not isinstance (self.__RegionSize, int):
            raise TypeError (f'Please assign the int type input for RegionSize.')
        elif not isinstance (self.__BlockSize, int):
            raise TypeError (f'Please assign the int type input for BlockSize.')
        elif not isinstance (self.__FvNameGuid, str):
            raise TypeError (f'Please assign the str type input for FvNameGuid.')
        elif not isinstance (self.__FvImageGuid, str):
            raise TypeError (f'Please assign the str type input for FvImageGuid.')
        elif not isinstance (self.__FspFilePath, (str, os.PathLike)):
            raise TypeError (f'Please assign the str or os.PathLike type input for FspFilePath.')

        if not IsPathExist (self.__FspFilePath):
            raise ValueError (f'Assigned path is not exist: {self.__FilePath}')
        elif not (IsFile (self.__FspFilePath)):
            raise ValueError (f'Assigned path is not file {self.__FilePath}')

    def __GenerateFdfFile (self) -> None:
        """ Generate FspCompress.fdf.

        Args:
            None

        Raises:
            None.

        Returns:
            None.
        """

        Config: dict = {
            'RegionSize'     :
                FormatHex (
                    self.__RegionSize,
                    IsPadding = True,
                    ),
            'BlockSize'      :
                FormatHex (
                    self.__BlockSize,
                    IsPadding = True,
                    ),
            'BlockNum'       : \
                FormatHex ((int)(self.__RegionSize / self.__BlockSize)),
            'CompressionGuid': self.__CompressionGuid,
            'FvName'         : self.__FvName,
            'FvNameGuid'     : self.__FvNameGuid,
            'FvImageGuid'    : self.__FvImageGuid,
            'FileName'       : self.__FspFilePath
            }

        FdfFile: File = File (FilePath = FSP_COMPRESS_FDF_TEMPLATE_PATH)
        FdfFile.Fill (**Config)
        FdfFile.Save (FSP_COMPRESS_FDF_PATH)

    def __GenerateDscFile (self) -> None:
        """ Generate the DSC file for FSP compression.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        Edk2DscGenerator: Edk2DscGeneratorApi = Edk2DscGeneratorApi ()

        Edk2DscGenerator.FileDescStr  = 'DSC file to create the compressed FV.'
        Edk2DscGenerator.PlatformName = FSP_COMPRESS_PKG_NAME
        Edk2DscGenerator.PlatformGuid = FSP_COMPRESS_PKG_GUID
        Edk2DscGenerator.FdfFile      = FSP_COMPRESS_FDF_FILE_NAME

        Edk2DscGenerator.Save (FSP_COMPRESS_DSC_PATH)

class _FspImgSplitter (object):
    def __init__ (
        self,
        FspFile     : Union[str, os.PathLike],
        FbmFile     : Union[str, os.PathLike],
        FlashMapDict: dict,
        BiosType    : str,
        BlockSize   : int,
        CompressInfo: dict,
        ) -> None:
        """ Class support to split the FSP image.

        Args:
            FspFile (Union[str, os.PathLike]):
                FSP File path.
            FbmFile (Union[str, os.PathLike]):
                Fbm File path.
            FlashMapDict (dict):
                Flash map information of target platform.
            BiosType (str):
                Type of the BIOS image.
            BlockSize (int):
                Flash block size.
            CompressInfo (dict):
                Compression info to generate fdf file.
        Raises:
            None.

        Returns:
            None.
        """
        self.__FspFile     : Union[str, os.PathLike] = FspFile
        self.__FbmFile     : Union[str, os.PathLike] = FbmFile
        self.__FlashMapDict: dict                    = FlashMapDict
        self.__BiosType    : str                     = BiosType
        self.__BlockSize   : int                     = BlockSize
        self.__CompressInfo: dict                    = CompressInfo

        self.__PreCheck ()

        self.__ImgInfo: Dict[str, ImageRegion] = dict ()

        TouchDir (FSP_REGION_DIR_PATH)

        self.__Split ()
        self.__CombineImage ()

    @property
    def ImgInfo (self) -> Dict[str, ImageRegion]:
        """ Return the updated image information.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, ImageRegion]:
                str:
                    Region name.
                BgupRegion:
                    Updated image information for specific region.
        """
        return self.__ImgInfo

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FspFile is not str or os.PathLike type.
                (2) FbmFile is not str or os.PathLike type.
                (3) FlashMapDict is not dict type.
                (4) BiosType is not str type.
                (5) BlockSize is not int type.
                (6) CompressFvInfo is dict int type.

        Returns:
            None.
        """
        if not isinstance (self.__FspFile, (str, os.PathLike)):
            raise TypeError ('FspFile should be str or os.PathLike type.')
        elif not isinstance (self.__FbmFile, (str, os.PathLike)):
            raise TypeError ('FbmFile should be str or os.PathLike type.')
        elif not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosType, str):
            raise TypeError ('BiosType should be str type.')
        elif not isinstance (self.__BlockSize, int):
            raise TypeError ('BlockSize should be int type.')
        elif not isinstance (self.__CompressInfo, dict):
            raise TypeError ('CompressFvInfo should be dict type.')

    def __Split (self) -> None:
        """ Split the updated image based on flash map.

        Args:
            None.

        Returns:
            None.
        """
        FbmFilePath: Union[str, os.PathLike] = None
        RegionInfo : ImageRegion             = None
        RegionName : str                     = None
        Offset     : int                     = 0
        RegionSize : int                     = 0

        self.__SplitFsp ()
        self.__SplitFspO ()

        FbmFilePath = JoinPath (FSP_REGION_DIR_PATH, FBM_IMG_FILE_NAME)
        if self.__FbmFile:
            CopyFile (self.__FbmFile, FbmFilePath)

        for RegionName, _ in self.__FlashMapDict.items ():
            _, _, RegionSize = GetFvRegionInfo (self.__FlashMapDict, RegionName)

            self.__PatchImage (RegionName, RegionSize)

            RegionInfo = self.__GenerateRegionInfo (RegionName, Offset)
            Offset     = Offset + RegionInfo.Size
            self.__ImgInfo.update ({RegionInfo.Region: RegionInfo})

    def __SplitFsp (self) -> None:
        """ Split FSP to FSP S / T / M / O

        Args:
            None.

        Raises:
            Exception:
                FSP revision is not supported.

        Returns:
            None.
        """
        FspCompress: _FspCompress   = _FspCompress ()
        Fd         : FirmwareDevice = FirmwareDevice(0, self.__FspFile)
        FspBuffer  : bytearray      = bytearray ()
        Fsp        : FirmwareVolume = None
        FspName    : str            = None
        ext        : str            = None
        FspfileName: str            = None
        FspfilePath: str            = None
        RegionSize : int            = None

        Fd.ParseFd  ()
        Fd.ParseFsp ()

        for Fsp in Fd.FspList:
            if Fsp.Fih.HeaderRevision < FSP_REVISION:
                raise Exception('ERROR: FSP 1.x is not supported by the split command !')

            FspName, ext = os.path.splitext(os.path.basename(FSP_IMG_FILE_NAME))
            FspName      = FspName + Fsp.Type
            FspfileName  = CombineFileName (FspName, Extension = ext)
            FspfilePath  = JoinPath (FSP_REGION_DIR_PATH, FspfileName)

            FspBuffer.clear ()

            for fvidx in Fsp.FvIdxList:
                fv = Fd.FvList[fvidx]
                FspBuffer += fv.FvData
            ByteBuffer (Buffer = FspBuffer).Save (FspfilePath)

            DEBUG (
                DEBUG_INFO,
                f'Create FSP component file {FspfilePath}'
            )

            _, _, RegionSize = GetFvRegionInfo (self.__FlashMapDict, FspName)

            if self.__CompressInfo[FspName] != NOT_APPLICABLE:
                FspCompress.CompressImage (
                    FvName          = FspName,
                    CompressionGuid = \
                        self.__CompressInfo[FspName][COMPRESSION_GUID_KEY],
                    RegionSize      = RegionSize,
                    BlockSize       = self.__BlockSize,
                    FvNameGuid      = \
                        self.__CompressInfo[FspName][COMPRESS_FVNAMEGUID_KEY],
                    FvImageGuid     = \
                        self.__CompressInfo[FspName][COMPRESS_FVIMAGEGUID_KEY],
                    FspFilePath     = FspfilePath
                    )

    def __SplitFspO (self) -> None:
        """ Get ResetVector and remove OEM config region from Fsp_O.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        FspOFileName : str = CombineFileName (
                                FSP_O,
                                Extension = EXTENSION_BIN_FILE
                                )
        FspOFilePath : str = JoinPath (FSP_REGION_DIR_PATH, FspOFileName)

        FspRvFileName: str = CombineFileName (
                                FSP_RV,
                                Extension = EXTENSION_BIN_FILE
                                )
        FspRvFilePath: str = JoinPath (FSP_REGION_DIR_PATH, FspRvFileName)

        FspOBuffer   : ByteBuffer = ByteBuffer (FspOFilePath)
        FspRvBuffer  : ByteBuffer = \
            ByteBuffer (
                Buffer = FspOBuffer.Buffer[-16:]
                )
        FspRvBuffer.Save (FspRvFilePath)

        NewFspOBuffer : ByteBuffer = \
            ByteBuffer (
                Buffer = FspOBuffer.Buffer[0 : FspOBuffer.Size - SIZE_4KB]
                )

        NewFspOBuffer.Save (FspOFilePath)

    def __PatchImage (
            self,
            RegionName: str,
            RegionSize: int
            ) -> None:
        """ Patch FSP components to expected size.

        Args:
            RegionName (str):
                Fsp component name.
            RegionSize (int):
                Expected file size after patch.

        Raises:
            Exception:
                Patch file is larger than expected.

        Returns:
            None.
        """
        FileName   : str        = None
        FilePath   : str        = None
        FspBuffer  : ByteBuffer = None
        PatchBuffer: bytearray  = bytearray ()

        FileName = CombineFileName (RegionName, Extension = EXTENSION_BIN_FILE)
        FilePath = JoinPath (FSP_REGION_DIR_PATH, FileName)

        FspBuffer  : ByteBuffer = ByteBuffer (FilePath = FilePath)

        if FspBuffer.Size == RegionSize:
            return

        if FspBuffer.Size > RegionSize:
            raise Exception (
                    f'Patch file {FilePath} is larger than expected.'
                    )

        PatchBuffer += FspBuffer.Buffer
        PatchBuffer += bytearray (b'\xff' * (RegionSize - FspBuffer.Size))

        FspBuffer = ByteBuffer (Buffer = PatchBuffer)
        FspBuffer.Save (FilePath)

    def __GenerateRegionInfo (
        self,
        RegionName: str,
        Offset    : int
        ) -> ImageRegion:
        """ Generate the region based on offset, size and return the info.

        Args:
            RegionName (str):
                Name of this region. (Should be unique)
            Offset (int):
                Offset of this region within the updated image.

        Raises:
            None.

        Returns:
            ImageRegion:
                Updated image information for specific region.
        """
        Buffer    : ByteBuffer  = None
        FileName  : str         = None
        FilePath  : str         = None
        RegionInfo: ImageRegion = None

        FileName = CombineFileName (RegionName, Extension = EXTENSION_BIN_FILE)
        FilePath = JoinPath (FSP_REGION_DIR_PATH, FileName)

        Buffer = ByteBuffer (FilePath)

        RegionInfo = ImageRegion (
                       Region   = RegionName,
                       FileName = FileName,
                       Path     = FilePath,
                       Offset   = Offset,
                       Size     = Buffer.Size,
                       Buffer   = Buffer,
                       )

        return RegionInfo

    def __CombineImage (self) -> None:
        """ Combine all FSP components and FBM image as new Fsp.bin.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """

        FspImg: bytearray = bytearray ()

        for RegionName in FSP_UPDATE_FLASH_ORDER:
            FileName = CombineFileName (
                RegionName,
                Extension = EXTENSION_BIN_FILE
                )
            FilePath = JoinPath (FSP_REGION_DIR_PATH, FileName)

            FspImg += ByteBuffer (FilePath = FilePath).Buffer

        ByteBuffer (Buffer = FspImg).Save (self.__FspFile)

class _FspBgupGenerator (object):
    def __init__ (
        self,
        FlashMapDict             : dict,
        FspImgInfoDict           : Dict[str, ImageRegion],
        BgslTemplateaAlignPath   : Union[str, os.PathLike],
        BgslTemplateaNonAlignPath: Union[str, os.PathLike],
        BiosSvn                  : str,
        ) -> None:
        """ Class support to generate the signed BGUP for update region.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            FspImgInfoDict (Dict[str, ImageRegion]):
                FSP image information dictionary.
            BgslTemplateAlignPath (Union[str, os.PathLike]):
                The path of BGSL template file for 4KB align update.
                (It is region information responsibility to provide the
                necessary information)
            BgslTemplateNonAlignPath (Union[str, os.PathLike]):
                The path of BGSL template file for Non-4KB align update.
                (It is region information responsibility to provide the
                necessary information)
            BiosSvn (str):
                The value of BIOS SVN for this signed BGUP.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMapDict  : dict                   = FlashMapDict
        self.__FspImgInfoDict: Dict[str, ImageRegion] = FspImgInfoDict
        self.__BiosSvn       : str                    = BiosSvn

        self.__BgslTemplateAlignPath    : Union[str, os.PathLike] \
              = BgslTemplateaAlignPath
        self.__BgslTemplateNonAlignPath : Union[str, os.PathLike] \
              = BgslTemplateaNonAlignPath

        self.__PreCheck ()

        self.__FspBgupInfo: Dict[str, BgupRegion] = dict ()
        self.__FspBgupImg : ByteBuffer            = self.__GetBgupBuffer ()

    @property
    def ImgInfo (self) -> Dict[str, BgupRegion]:
        """ Return the singed BGUP image information.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, BgupRegion]:
                str:
                    Region name.
                BgupRegion:
                    BGUP image information for specific region.
        """
        return self.__FspBgupInfo

    @property
    def Image (self) -> ByteBuffer:
        """ Return the merged singed BGUP images buffer.

        Note:
            Each BGUP regions is merged into one.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                Byte buffer for all in one signed BGUP image.
        """
        return self.__FspBgupImg

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) BiosSvn not str type.
                (3) FspBgupInfoDict not dict type.
                (4) FspImgInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgslTemplateAlignPath,
            self.__BgslTemplateNonAlignPath,
            ]

        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosSvn, str):
            raise TypeError ('BiosSvn should be str type.')
        elif not isinstance (self.__FspImgInfoDict, dict):
            raise TypeError ('FspImgInfoDict should be dict type.')

        for Region, FspImgInfo in self.__FspImgInfoDict.items ():
            if not isinstance (FspImgInfo, ImageRegion):
                raise TypeError (
                        f'Region {Region} should be ImageRegion type.'
                        )

        CheckFileListExist (CheckPathList)

    def __GenerateBgslFile (self, Region: str, ImgInfo: ImageRegion) -> File:
        """ Return the BGSL for specific region.

        Note:
            This function is used to fill in the base address and
            size into BGSL template file.

        Args:
            ImgInfo (ImageRegion):
                The image information for specific region.

        Raises:
            None.

        Returns:
            File:
                File object for BGSL script content.
        """

        Config: dict = {
            FSP_BGSL_OFFSET_KEY:
                FormatHex (
                  self.__FlashMapDict[Region][FM_OFFSET_KEY],
                  IsPadding = True,
                  ),
            FSP_BGSL_SIZE_KEY  :
                FormatHex (
                  DecToHex (ImgInfo.Size),
                  IsPadding = True,
                  ),
            }
        if ImgInfo.Size & (ALIGNMENT_4K - 1):
            BgslFile: File = File (FilePath = self.__BgslTemplateNonAlignPath)
        else:
            BgslFile: File = File (FilePath = self.__BgslTemplateAlignPath)
        BgslFile.Fill (**Config)

        return BgslFile

    def __GetFspBgupImg (self) -> ByteBuffer:
        """ Return the merged singed BGUP images buffer.

        Args:
            None.

        Raises:
            ValueError:
                Mandatory update region is not exist.

        Returns:
            ByteBuffer:
                Merged all BGUP images into one buffer.
        """
        FspBgupImg: bytearray = bytearray ()
        Offset     : int       = 0x0
        IsFound    : bool      = None
        Value      : str       = None

        for RegionName in BGUP_REGION_ORDER:
            IsFound, Value = SearchKeyInDict (self.__FspBgupInfo, RegionName)
            if not IsFound:
                raise ValueError (
                        f'Can not find the necessary BGUP for {RegionName}'
                        )

            self.__FspBgupInfo[Value].Offset = Offset

            FspBgupImg += self.__FspBgupInfo[Value].Image.Buffer
            Offset      += self.__FspBgupInfo[Value].Size

        return ByteBuffer (Buffer = FspBgupImg)

    def __GetBgupBuffer (self) -> ByteBuffer:
        """ Internal function to build the BGUP.

        Note:
            (1) Iteration all the regions.
            (2) Create the correspond BGSL for specific region.
            (3) Build the signed BGUP for specific region.
            (4) Add the BGUP information into dictionary.
            (5) Merge all the BGUP into one and update the offset.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        TouchDir (BIOSGUARD_BGSL_DIR)
        TouchDir (BIOSGUARD_BGUP_DIR)

        ImgInfo   : ImageRegion     = None
        BgslFile  : File            = None
        BgslPath  : str             = None
        Generator : BgupGenerateApi = None
        BgupBuffer: ByteBuffer      = None

        for RegionName, ImgInfo in self.__FspImgInfoDict.items ():
            #
            # Create BGSL
            #
            BgslPath = \
                JoinPath (
                  BIOSGUARD_BGSL_DIR,
                  CombineFileName (
                    RegionName,
                    Extension = EXTENSION_BGSL_FILE,
                    ),
                  )
            BgslFile = self.__GenerateBgslFile (
                Region = RegionName,
                ImgInfo = ImgInfo)
            BgslFile.Save (BgslPath)
            #
            # Build BGUP
            #
            Generator = \
                BgupGenerateApi (
                  FileName     = ImgInfo.Region,
                  BgslPath     = BgslPath,
                  PlatformName = BIOSGUARD_PLATFORM_ID,
                  BiosSvn      = self.__BiosSvn,
                  Prefix       = 'Fsp',
                  DataPath     = ImgInfo.Path,
                  )
            Generator.Build ()

            #
            # Add the BGUP information.
            #
            self.__FspBgupInfo.\
                update ({
                  ImgInfo.Region:
                      BgupRegion (
                        Region   = ImgInfo.Region,
                        FileName = Generator.FileName,
                        Path     = Generator.FilePath,
                        Buffer   = ByteBuffer (FilePath = Generator.FilePath),
                        )
                  })

        #
        # Merge the BGUP image into one and update offset/length.
        #
        BgupBuffer = self.__GetFspBgupImg ()

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

class _FspCfgIniGenerator (object):
    def __init__ (
        self,
        FlashMapDict   : dict,
        BiosType       : str,
        FspImgInfoDict : Dict[str, ImageRegion],
        FspBgupInfoDict: Dict[str, BgupRegion],
        ) -> None:
        """ Class to support to generate the flash config INI file.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            BiosType (str):
                Type of BIOS image.
            FspImgInfoDict (Dict[str, ImageRegion]):
                FSP image region information dictionary.
                Key - region name, Value - region information.
            FspBgupInfoDict (Dict[str, BgupRegion]):
                FSP image mapping BGUP information dictionary.
                Key - region name, Value - region BGUP information.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMapDict   : dict                   = FlashMapDict
        self.__BiosType       : str                    = BiosType
        self.__FspImgInfoDict : Dict[str, ImageRegion] = FspImgInfoDict
        self.__FspBgupInfoDict: Dict[str, BgupRegion]  = FspBgupInfoDict

        self.__PreCheck ()

        self.__UpdateOrder: List[str] = FSP_UPDATE_FLASH_ORDER

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) BiosType not str type.
                (3) FspImgDict not dict type.
                (4) FspBgupInfoDict not dict type.
                (5) FspImgInfoDict member not ImageRegion type.
                (6) FspBgupInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosType, str):
            raise TypeError ('BiosType should be str type.')
        elif not isinstance (self.__FspImgInfoDict, dict):
            raise TypeError ('FspImgInfoDict should be dict type.')
        elif not isinstance (self.__FspBgupInfoDict, dict):
            raise TypeError ('FspBgupInfoDict should be dict type.')

        for Region, FspImgInfo in self.__FspImgInfoDict.items ():
            if not isinstance (FspImgInfo, ImageRegion):
                raise TypeError (f'Region {Region} should be ImageRegion type.')

        for Region, BgupImgInfo in self.__FspBgupInfoDict.items ():
            if not isinstance (BgupImgInfo, BgupRegion):
                raise TypeError (f'Region {Region} should be BgupRegion type.')

    def Save (self, OutputPath: str) -> None:
        """ Save the flash config INI file into storage.

        Args:
            OutputPath (str):
                Output path of the flash config INI file.

        Raises:
            None.

        Returns:
            None.
        """
        Generator: FlashConfigIniGenerator = \
            FlashConfigIniGenerator (
              FlashMap      = self.__FlashMapDict,
              ImageGuid     = uuid.UUID (FSP_IMG_FILE_GUID),
              ImageInfoDict = self.__FspImgInfoDict,
              BgupGuid      = uuid.UUID (FSP_BGUP_FILE_GUID),
              BgupInfoDict  = self.__FspBgupInfoDict,
              )

        CfgIniFile: FlashConfig = \
            Generator.ConfigFile (
                        Prefix = '',
                        Order  = self.__UpdateOrder,
                        )

        CfgIniFile.Save (OutputPath)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    '_FspImgSplitter',
    '_FspBgupGenerator',
    '_FspCfgIniGenerator'
    ]
