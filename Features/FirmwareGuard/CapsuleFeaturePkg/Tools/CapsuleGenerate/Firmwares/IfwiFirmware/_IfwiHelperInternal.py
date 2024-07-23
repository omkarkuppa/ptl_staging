## @file
#  Internal library support to generate the IFWI capsule payload.
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

from CapsuleCommon import *

from CapsuleGenerate.ApiPrivate import *

from CapsuleGenerate.Firmwares.CommonLib import *

#
# Directory to split to IFWI image.
#
IFWI_REGION_DIR_NAME: str = 'Ifwi'
IFWI_REGION_DIR_PATH: str = \
    JoinPath (
      TEMPORARY_DIR_PATH,
      IFWI_REGION_DIR_NAME,
      )

#
# BGSL template file.
#
IFWI_BGSL_OFFSET_KEY: str = 'RegionOffset'
IFWI_BGSL_SIZE_KEY  : str = 'RegionSize'

IFWI_FLASH_TRUNK_SIZE: int = SIZE_8MB

class _IfwiImgTrunkGenerator (object):
    def __init__ (
        self,
        IfwiBuffer: ByteBuffer,
        ) -> None:
        """ Class to generate the information for IFWI trunk.

        Args:
            IfwiBuffer (ByteBuffer):
                Updated image buffer for specific FMP device.

        Raises:
            None.

        Returns:
            None.
        """
        self.__IfwiBuffer: ByteBuffer = copy.deepcopy (IfwiBuffer)

        self.__PreCheck ()

        self.__ImgInfo         : Dict[str, ImageRegion] = dict ()
        self.__TrunkCnt        : int                    = \
            int (self.__IfwiBuffer.Size / IFWI_FLASH_TRUNK_SIZE)
        self.__FlashMapInfoDict: dict                   = dict ()

        TouchDir (IFWI_REGION_DIR_PATH)

        self.__Split ()

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

    @property
    def FlashMapInfo (self) -> dict:
        """ Return the flash map information for each trunk.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                Flash map information for specific slot.
        """
        return self.__FlashMapInfoDict

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                IfwiBuffer not ByteBuffer type.
            ValueError:
                IfwiBuffer is not aligned with IFWI_FLASH_TRUNK_SIZE.

        Returns:
            None.
        """
        if not isinstance (self.__IfwiBuffer, ByteBuffer):
            raise TypeError ('IfwiBuffer should be ByteBuffer type.')

        if (self.__IfwiBuffer.Size % IFWI_FLASH_TRUNK_SIZE) != 0:
            raise ValueError ('Input image is not aligned.')

    def __Split (self) -> None:
        """ Split the updated image based on assigned trunk size.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        RegionInfo   : ImageRegion = None
        RegionOffset : int         = None
        RegionSize   : int         = None

        for Idx in range (self.__TrunkCnt):
            RegionName   = f'Ifwi{Idx}'
            RegionOffset = IFWI_FLASH_TRUNK_SIZE * Idx
            RegionSize   = IFWI_FLASH_TRUNK_SIZE

            RegionInfo = self.__SplitRegion (
                                RegionName,
                                RegionOffset,
                                RegionSize,
                                )

            self.__ImgInfo.update ({RegionInfo.Region: RegionInfo})

    def __SplitRegion (
        self,
        RegionName: str,
        Offset    : int,
        Size      : int,
        ) -> ImageRegion:
        """ Split the region based on offset, size and return the info.

        Args:
            RegionName (str):
                Name of this region. (Should be unique)
            Offset (int):
                Offset of this region within the updated image.
            Size (int):
                Size of this region within the updated image.

        Raises:
            None.

        Returns:
            ImageRegion:
                Updated image information for specific region.
        """
        Buffer    : ByteBuffer  = \
            ByteBuffer (Buffer = copy.deepcopy (self.__IfwiBuffer.Buffer))
        FileName  : str         = None
        FilePath  : str         = None
        RegionInfo: ImageRegion = None

        FileName = CombineFileName (RegionName, Extension = EXTENSION_BIN_FILE)
        FilePath = JoinPath (IFWI_REGION_DIR_PATH, FileName)

        Buffer = ByteBuffer (Buffer = Buffer.Buffer[Offset:Offset+Size])
        Buffer.Save (FilePath)

        RegionInfo = ImageRegion (
                       Region   = RegionName,
                       FileName = FileName,
                       Path     = FilePath,
                       Offset   = Offset,
                       Size     = Size,
                       Buffer   = Buffer,
                       )

        #
        # Add the trunk information mapping into flash map.
        #
        self.__FlashMapInfoDict.update ({
            RegionName: {
                FM_OFFSET_KEY: FormatHex (Offset, IsPadding = True),
                FM_SIZE_KEY  : FormatHex (Size,   IsPadding = True),
                }
            })

        return RegionInfo

class _IfwiBgupGenerator (object):
    def __init__ (
        self,
        IfwiImgInfoDict : Dict[str, ImageRegion],
        BgslTemplatePath: Union[str, os.PathLike],
        BiosSvn         : str,
        ) -> None:
        """ Class support to generate the signed BGUP for update region.

        Args:
            IfwiImgInfoDict (Dict[str, ImageRegion]):
                IFWI image information dictionary.
            BgslTemplatePath (Union[str, os.PathLike]):
                The path of BGSL template file.
                (It is region information responsibility to provide the
                necessary information)
            BiosSvn (str):
                The value of BIOS SVN for this signed BGUP.

        Raises:
            None.

        Returns:
            None.
        """
        self.__IfwiImgInfoDict : Dict[str, ImageRegion] = IfwiImgInfoDict
        self.__BgslTemplatePath: str                    = \
            AbsPath (BgslTemplatePath)
        self.__BiosSvn         : str                    = BiosSvn

        self.__PreCheck ()

        self.__IfwiBgupInfo: Dict[str, BgupRegion] = dict ()
        self.__IfwiBgupImg : ByteBuffer            = self.__GetBgupBuffer ()

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
        return self.__IfwiBgupInfo

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
        return self.__IfwiBgupImg

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) BiosSvn not str type.
                (2) IfwiImgInfoDict not dict type.
                (3) IfwiImgInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgslTemplatePath,
            ]

        CheckFileListExist (CheckPathList)

        if not isinstance (self.__BiosSvn, str):
            raise TypeError ('BiosSvn should be str type.')
        elif not isinstance (self.__IfwiImgInfoDict, dict):
            raise TypeError ('IfwiImgInfoDict should be dict type.')

        for Region, IfwiImgInfo in self.__IfwiImgInfoDict.items ():
            if not isinstance (IfwiImgInfo, ImageRegion):
                raise TypeError (
                        f'Region {Region} should be ImageRegion type.'
                        )

    def __GenerateBgslFile (self, ImgInfo: ImageRegion) -> File:
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
            IFWI_BGSL_OFFSET_KEY:
                FormatHex (
                  DecToHex (ImgInfo.Offset),
                  IsPadding = True,
                  ),
            IFWI_BGSL_SIZE_KEY  :
                FormatHex (
                  DecToHex (ImgInfo.Size),
                  IsPadding = True,
                  ),
            }

        BgslFile: File = File (FilePath = self.__BgslTemplatePath)
        BgslFile.Fill (**Config)

        return BgslFile

    def __GetIfwiBgupImg (self) -> ByteBuffer:
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
        IfwiBgupImg: bytearray = bytearray ()
        BgupInfo   : BgupInfo  = None
        Offset     : int       = 0x0

        for RegionName, BgupRegion in self.__IfwiBgupInfo.items ():
            self.__IfwiBgupInfo[RegionName].Offset = Offset

            IfwiBgupImg += BgupRegion.Image.Buffer
            Offset      += BgupRegion.Size

        return ByteBuffer (Buffer = IfwiBgupImg)

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

        for RegionName, ImgInfo in self.__IfwiImgInfoDict.items ():
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
            BgslFile = self.__GenerateBgslFile (ImgInfo = ImgInfo)
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
                  Prefix       = 'Ifwi',
                  DataPath     = ImgInfo.Path,
                  )
            Generator.Build ()

            #
            # Add the BGUP information.
            #
            self.__IfwiBgupInfo.\
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
        BgupBuffer = self.__GetIfwiBgupImg ()

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

class _IfwiCfgIniGenerator (object):
    def __init__ (
        self,
        FlashMapDict    : dict,
        IfwiImgInfoDict : Dict[str, ImageRegion],
        IfwiBgupInfoDict: Dict[str, BgupRegion],
        ) -> None:
        """ Class to support to generate the flash config INI file.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            IfwiImgInfoDict (Dict[str, ImageRegion]):
                IFWI image region information dictionary.
                Key - region name, Value - region information.
            IfwiBgupInfoDict (Dict[str, BgupRegion]):
                IFWI image mapping BGUP information dictionary.
                Key - region name, Value - region BGUP information.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMapDict    : dict                   = copy.deepcopy (FlashMapDict)
        self.__IfwiImgInfoDict : Dict[str, ImageRegion] = IfwiImgInfoDict
        self.__IfwiBgupInfoDict: Dict[str, BgupRegion]  = IfwiBgupInfoDict

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) IfwiImgInfoDict not dict type.
                (3) IfwiBgupInfoDict not dict type.
                (4) IfwiImgInfoDict member not ImageRegion type.
                (5) IfwiBgupInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__IfwiImgInfoDict, dict):
            raise TypeError ('IfwiImgInfoDict should be dict type.')
        elif not isinstance (self.__IfwiBgupInfoDict, dict):
            raise TypeError ('IfwiBgupInfoDict should be dict type.')

        for Region, IfwiImgInfo in self.__IfwiImgInfoDict.items ():
            if not isinstance (IfwiImgInfo, ImageRegion):
                raise TypeError (f'Region {Region} should be ImageRegion type.')

        for Region, BgupImgInfo in self.__IfwiBgupInfoDict.items ():
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
              ImageGuid     = uuid.UUID (IFWI_IMG_FILE_GUID),
              ImageInfoDict = self.__IfwiImgInfoDict,
              BgupGuid      = uuid.UUID (IFWI_BGUP_FILE_GUID),
              BgupInfoDict  = self.__IfwiBgupInfoDict,
              )

        CfgIniFile: FlashConfig = \
            Generator.ConfigFile (Prefix = 'Ifwi')

        CfgIniFile.Save (OutputPath)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    '_IfwiImgTrunkGenerator',
    '_IfwiBgupGenerator',
    '_IfwiCfgIniGenerator',
    ]
