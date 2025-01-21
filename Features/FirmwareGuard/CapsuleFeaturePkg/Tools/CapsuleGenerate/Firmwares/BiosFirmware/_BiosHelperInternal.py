## @file
#  Internal library support to generate the BIOS capsule payload.
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

import copy
import uuid
from typing import List

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate import *

from CapsuleGenerate.Firmwares.CommonLib import *

#
# List of all possible BIOS region.
#
BIOS_REGION_LIST: List[str] = [
    BIOS_REGION_EXT,
    BIOS_REGION_OBB1,
    BIOS_REGION_NVS,
    BIOS_REGION_OBB,
    BIOS_REGION_IBBR,
    BIOS_REGION_IBB,
    ]

#
# Directory to split to BIOS image.
#
BIOS_REGION_DIR_NAME: str = 'Bios'
BIOS_REGION_DIR_PATH: str = \
    JoinPath (
      TEMPORARY_DIR_PATH,
      BIOS_REGION_DIR_NAME,
      )

#
# BGSL template file.
#
BIOS_BGSL_OFFSET_KEY: str = 'RegionOffset'
BIOS_BGSL_SIZE_KEY  : str = 'RegionSize'

class _BiosImgSplitter (object):
    def __init__ (
        self,
        BiosBuffer  : ByteBuffer,
        FlashMapDict: dict,
        BiosType    : str,
        ) -> None:
        """ Class support to split the BIOS image.

        Args:
            BiosBuffer (ByteBuffer):
                Updated image buffer for specific FMP device.
            FlashMapDict (dict):
                Flash map information of target platform.
            BiosType (str):
                Type of the BIOS image.

        Raises:
            None.

        Returns:
            None.
        """
        self.__BiosBuffer  : ByteBuffer = copy.deepcopy (BiosBuffer)
        self.__FlashMapDict: dict       = FlashMapDict
        self.__BiosType    : str        = BiosType

        self.__PreCheck ()

        self.__ImgInfo: Dict[str, ImageRegion] = dict ()

        TouchDir (BIOS_REGION_DIR_PATH)

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

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) BiosBuffer not ByteBuffer type.
                (2) FlashMapDict not dict type.
                (3) BiosType not str type.

        Returns:
            None.
        """
        if not isinstance (self.__BiosBuffer, ByteBuffer):
            raise TypeError ('BiosBuffer should be ByteBuffer type.')
        elif not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosType, str):
            raise TypeError ('BiosType should be str type.')

    def __Split (self) -> None:
        """ Split the updated image based on flash map.

        Note:
            BKC BIOS IBBr is the same as the IBB, need update it based
            on the IBB information.

        Args:
            None.

        Raises:
            ValueError:
                Flash map not assigned the IBB information.

        Returns:
            None.
        """
        RegionInfo: ImageRegion = None
        RegionName: str         = None
        IsAssigned: bool        = None
        Offset    : int         = None
        Size      : int         = None

        for RegionName, _ in self.__FlashMapDict.items ():
            IsAssigned, _ = IsRegionInfoAssigned (
                              DataDict = self.__FlashMapDict,
                              Region   = RegionName,
                              )
            if not IsAssigned:
                DEBUG (DEBUG_TRACE, f'[{RegionName}] - is not assigned, no need this region.')
                continue

            _, Offset, Size = GetFvRegionInfo (self.__FlashMapDict, RegionName)
            RegionInfo = self.__SplitRegion (RegionName, Offset, Size)
            self.__ImgInfo.update ({RegionInfo.Region: RegionInfo})

        #
        # Handle the case for Normal BIOS without IBBr
        #
        if self.__BiosType == BIOS_NORMAL_TYPE:
            IsFound, RegionName = \
                SearchKeyInDict (self.__ImgInfo, BIOS_REGION_IBB)
            if not IsFound:
                raise ValueError ('BKC BIOS should contain the IBB info.')

            RegionInfo = self.__ImgInfo[RegionName]
            self.__AppendIbbAsIbbr (RegionInfo)

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
            ByteBuffer (Buffer = copy.deepcopy (self.__BiosBuffer.Buffer))
        FileName  : str         = None
        FilePath  : str         = None
        RegionInfo: ImageRegion = None

        FileName = CombineFileName (RegionName, Extension = EXTENSION_BIN_FILE)
        FilePath = JoinPath (BIOS_REGION_DIR_PATH, FileName)

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

        return RegionInfo

    def __AppendIbbAsIbbr (self, IbbImgInfo: ImageRegion) -> None:
        """ Update the IBBR region information based on IBB region.

        Args:
            IbbImgInfo (ImageRegion):
                Updated image information for IBB region.

        Raises:
            None.

        Returns:
            None.
        """
        IbbRFileName: str = \
            CombineFileName (BIOS_REGION_IBBR, Extension = EXTENSION_BIN_FILE)
        IbbRFilePath: str = \
            JoinPath (TEMPORARY_DIR_PATH, BIOS_REGION_DIR_NAME, IbbRFileName)

        CopyFile (IbbImgInfo.Path, IbbRFilePath)

        self.__ImgInfo.update ({
                         BIOS_REGION_IBBR:
                             ImageRegion (
                               Region   = BIOS_REGION_IBBR,
                               FileName = IbbRFileName,
                               Path     = IbbRFilePath,
                               Offset   = IbbImgInfo.Offset,
                               Size     = IbbImgInfo.Size,
                               Buffer   = copy.deepcopy (IbbImgInfo.Buffer),
                               )
                         })

class _BiosBgupGenerator (object):
    def __init__ (
        self,
        FlashMapDict    : dict,
        BiosImgInfoDict : Dict[str, ImageRegion],
        BgupRegionOrder : List[str],
        BgslTemplatePath: Union[str, os.PathLike],
        BiosSvn         : str,
        ) -> None:
        """ Class support to generate the signed BGUP for update region.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            BiosImgInfoDict (Dict[str, ImageRegion]):
                BIOS image information dictionary.
            BgupRegionOrder (List[str]):
                BGUP region order.
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
        self.__FlashMapDict    : dict                   = FlashMapDict
        self.__BiosImgInfoDict : Dict[str, ImageRegion] = BiosImgInfoDict
        self.__BgupRegionOrder : List[str]              = BgupRegionOrder
        self.__BgslTemplatePath: str                    = \
            AbsPath (BgslTemplatePath)
        self.__BiosSvn         : str                    = BiosSvn

        self.__PreCheck ()

        self.__BiosBgupInfo: Dict[str, BgupRegion] = dict ()
        self.__BiosBgupImg : ByteBuffer            = self.__GetBgupBuffer ()

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
        return self.__BiosBgupInfo

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
        return self.__BiosBgupImg

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) BiosSvn not str type.
                (3) BiosBgupInfoDict not dict type.
                (4) BiosImgInfoDict member not ImageRegion type.
                (5) BgupRegionOrder not list type.

        Returns:
            None.
        """
        CheckPathList: List[str] = [
            self.__BgslTemplatePath,
            ]

        CheckFileListExist (CheckPathList)

        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosSvn, str):
            raise TypeError ('BiosSvn should be str type.')
        elif not isinstance (self.__BiosImgInfoDict, dict):
            raise TypeError ('BiosImgInfoDict should be dict type.')
        elif not isinstance (self.__BgupRegionOrder, list):
            raise TypeError ('BgupRegionOrder should be list type.')

        for Region, BiosImgInfo in self.__BiosImgInfoDict.items ():
            if not isinstance (BiosImgInfo, ImageRegion):
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
        _, BaseAddress, _ = GetFvRegionInfo (
                              self.__FlashMapDict,
                              ImgInfo.Region,
                              )

        Config: dict = {
            BIOS_BGSL_OFFSET_KEY:
                FormatHex (
                  DecToHex (BaseAddress),
                  IsPadding = True,
                  ),
            BIOS_BGSL_SIZE_KEY  :
                FormatHex (
                  DecToHex (ImgInfo.Size),
                  IsPadding = True,
                  ),
            }

        BgslFile: File = File (FilePath = self.__BgslTemplatePath)
        BgslFile.Fill (**Config)

        return BgslFile

    def __GetBiosBgupImg (self) -> ByteBuffer:
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
        BiosBgupImg: bytearray = bytearray ()
        Offset     : int       = 0x0
        IsAssigned : bool      = None
        IsFound    : bool      = None
        Value      : str       = None

        for RegionName in self.__BgupRegionOrder:
            IsAssigned, _ = IsRegionInfoAssigned (
                              DataDict = self.__BiosBgupInfo,
                              Region   = RegionName,
                              )
            if not IsAssigned:
                DEBUG (DEBUG_TRACE, f'[{RegionName}] - is not assigned, no need this region.')
                continue

            IsFound, Value = SearchKeyInDict (self.__BiosBgupInfo, RegionName)
            if not IsFound:
                raise ValueError (
                        f'Can not find the necessary BGUP for {RegionName}'
                        )

            self.__BiosBgupInfo[Value].Offset = Offset

            BiosBgupImg += self.__BiosBgupInfo[Value].Image.Buffer
            Offset      += self.__BiosBgupInfo[Value].Size

        return ByteBuffer (Buffer = BiosBgupImg)

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

        for RegionName, ImgInfo in self.__BiosImgInfoDict.items ():
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
                  Prefix       = 'Bios',
                  DataPath     = ImgInfo.Path,
                  )
            Generator.Build ()

            #
            # Add the BGUP information.
            #
            self.__BiosBgupInfo.\
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
        BgupBuffer = self.__GetBiosBgupImg ()

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

class _BiosCfgIniGenerator (object):
    def __init__ (
        self,
        FlashMapDict    : dict,
        BiosUpdateOrder : List[str],
        BiosImgInfoDict : Dict[str, ImageRegion],
        BiosBgupInfoDict: Dict[str, BgupRegion],
        ) -> None:
        """ Class to support to generate the flash config INI file.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            BiosUpdateOrder (List[str]):
                BIOS region update order.
            BiosImgInfoDict (Dict[str, ImageRegion]):
                BIOS image region information dictionary.
                Key - region name, Value - region information.
            BiosBgupInfoDict (Dict[str, BgupRegion]):
                BIOS image mapping BGUP information dictionary.
                Key - region name, Value - region BGUP information.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMapDict    : dict                   = FlashMapDict
        self.__BiosUpdateOrder : List[str]              = BiosUpdateOrder
        self.__BiosImgInfoDict : Dict[str, ImageRegion] = BiosImgInfoDict
        self.__BiosBgupInfoDict: Dict[str, BgupRegion]  = BiosBgupInfoDict

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) BiosUpdateOrder not list type.
                (3) BiosImgInfoDict not dict type.
                (4) BiosBgupInfoDict not dict type.
                (5) BiosImgInfoDict member not ImageRegion type.
                (6) BiosBgupInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__BiosUpdateOrder, list):
            raise TypeError ('BiosUpdateOrder should be list type.')
        elif not isinstance (self.__BiosImgInfoDict, dict):
            raise TypeError ('BiosImgInfoDict should be dict type.')
        elif not isinstance (self.__BiosBgupInfoDict, dict):
            raise TypeError ('BiosBgupInfoDict should be dict type.')

        for Region, BiosImgInfo in self.__BiosImgInfoDict.items ():
            if not isinstance (BiosImgInfo, ImageRegion):
                raise TypeError (f'Region {Region} should be ImageRegion type.')

        for Region, BgupImgInfo in self.__BiosBgupInfoDict.items ():
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
              ImageGuid     = uuid.UUID (BIOS_IMG_FILE_GUID),
              ImageInfoDict = self.__BiosImgInfoDict,
              BgupGuid      = uuid.UUID (BIOS_BGUP_FILE_GUID),
              BgupInfoDict  = self.__BiosBgupInfoDict,
              )

        CfgIniFile: FlashConfig = \
            Generator.ConfigFile (
                        Prefix = 'Bios',
                        Order  = self.__BiosUpdateOrder,
                        )

        CfgIniFile.Save (OutputPath)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    '_BiosImgSplitter',
    '_BiosBgupGenerator',
    '_BiosCfgIniGenerator',
    ]
