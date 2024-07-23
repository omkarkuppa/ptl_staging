## @file
#  Internal library support to generate the BtGAcm capsule payload.
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
import math
import os
from typing import Union

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate.BgupGeneratorApiLib import *

from CapsuleGenerate.Firmwares.CommonLib import *

#
# BGSL template file.
#
BTG_ACM_BGSL_OFFSET_KEY: str = 'RegionOffset'
BTG_ACM_BGSL_SIZE_KEY  : str = 'RegionSize'

#
# Definition of update region.
#
BTG_ACM_SLOT_PREFIX: str = 'BtGAcmSlot'
BTG_ACM_FILE_PREFIX: str = 'Btgacm'

class _BtGAcmSlotInfoGenerator (object):
    def __init__ (
        self,
        FvOffset    : int,
        FvSize      : int,
        SlotSize    : int,
        BtGAcmImg   : ByteBuffer,
        IsSingleSlot: bool = False,
        ) -> None:
        """ Class to generate the information of BtGAcm slots.

        Args:
            FvOffset (int):
                Offset of BtGAcm slots series.
            FvSize (int):
                Size of BtGAcm slots series.
            SlotSize (int):
                Size of BtGAcm slot.
            BtGAcmImg (ByteBuffer):
                BtGAcm image buffer to be delivered.
            IsSingleSlot (bool, optional):
                True - Only deliver BGUP for primary slot.
                False - Deliver all of the possible slots BGUP.
                Defaults to False.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FvOffset    : int        = FvOffset
        self.__FvSize      : int        = FvSize
        self.__SlotSize    : int        = SlotSize
        self.__BtGAcmImg   : ByteBuffer = BtGAcmImg
        self.__IsSingleSlot: bool       = IsSingleSlot

        self.__PreCheck ()

        self.__NumOfSlot: int = self.__GetNumOfSlot ()

        self.__FlashMapInfoDict: dict                   = dict ()
        self.__ImgInfoDict     : Dict[str, ImageRegion] = dict ()
        self.__GenerateSlotInfo ()

    @property
    def ImgInfo (self) -> Dict[str, ImageRegion]:
        """ Return the BtGAcm image information for each slot.

        Args:
            None.

        Raises:
            None.

        Returns:
            Dict[str, ImageRegion]:
                str:
                    Region name.
                ImageRegion:
                    BtGAcm image information for specific slot.
        """
        return self.__ImgInfoDict

    @property
    def FlashMapInfo (self) -> dict:
        """ Return the flash map information for each slot.

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
                (1) FvOffset is not int type.
                (2) FvSize is not int type.
                (3) SlotSize is not int type.
                (4) BtGAcmImg is not ByteBuffer type.
                (5) IsSingleSlot is not bool type.
            ValueError:
                (1) FvOffset is not UINT32.
                (2) FvSize is not UINT32.
                (3) Address of end of FV is not UINT32.
                (4) SlotSize cannot accommodate the BtGAcm buffer.

        Returns:
            None.
        """
        if not isinstance (self.__FvOffset, int):
            raise TypeError ('FvOffset should be int type.')
        elif not isinstance (self.__FvSize, int):
            raise TypeError ('FvSize should be int type.')
        elif not isinstance (self.__SlotSize, int):
            raise TypeError ('SlotSize should be int type.')
        elif not isinstance (self.__BtGAcmImg, ByteBuffer):
            raise TypeError ('BtGAcmImg should be ByteBuffer type.')
        elif not isinstance (self.__IsSingleSlot, bool):
            raise TypeError ('IsSingleSlot should be bool type.')

        if not IsUint32 (self.__FvOffset):
            raise ValueError ('FvOffset should be UINT32.')
        elif not IsUint32 (self.__FvSize):
            raise ValueError ('FvSize should be UINT32.')
        elif not IsUint32 (self.__FvOffset + self.__FvSize):
            raise ValueError ('Address of end of FV should be UINT32.')
        elif self.__BtGAcmImg.Size > self.__SlotSize:
            raise ValueError ('SlotSize is too small.')

    def __GetNumOfSlot (self) -> int:
        """ Get the total possible number of slots.

        Args:
            None.

        Raises:
            ValueError:
                Number of slots is 0.

        Returns:
            int:
                Maximum number of slots for BtGAcm.
        """
        NumOfSlot: int = None

        NumOfSlot = math.floor (self.__FvSize / self.__SlotSize)
        if NumOfSlot == 0:
            raise ValueError (f'Number of slot cannot as 0.')

        if self.__IsSingleSlot:
            NumOfSlot = 1

        return NumOfSlot

    def __GenerateSlotInfo (self) -> None:
        """ Internal function to generate the BtGAcm slot information.

        All the operation for the slot specific should be done in here.

        Note:
            (1) Generate the ImgInfo on each slot with BtGAcm image.
            (2) Generate the FlashMap information on each slot.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        RegionName  : str       = None
        Offset      : int       = None

        for SlotNum in range (self.__NumOfSlot):
            RegionName = f'{BTG_ACM_SLOT_PREFIX}{SlotNum}'
            Offset = self.__FvOffset + (SlotNum * self.__SlotSize)

            #
            # Add the slot information mapping into flash map.
            #
            self.__FlashMapInfoDict.update ({
                RegionName: {
                    FM_OFFSET_KEY: FormatHex (Offset, IsPadding = True),
                    FM_SIZE_KEY  : FormatHex (self.__SlotSize, IsPadding = True),
                    }
                })

            #
            # Add the image information for each slot.
            #
            self.__ImgInfoDict.update ({
                RegionName: ImageRegion (
                              Region   = RegionName,
                              FileName = BTG_ACM_IMG_FILE_NAME,
                              Path     = BTG_ACM_IMG_FILE_PATH,
                              Offset   = 0x0000_0000,
                              Size     = self.__BtGAcmImg.Size,
                              Buffer   = copy.deepcopy (self.__BtGAcmImg),
                              )
                })

class _BtGAcmBgupGenerator (object):
    def __init__ (
        self,
        FlashMapDict     : dict,
        BtGAcmImgInfoDict: Dict[str, ImageRegion],
        BgslTemplatePath : Union[str, os.PathLike],
        BiosSvn          : str,
        ) -> None:
        """ Class support to generate the signed BGUP for update region.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
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
        self.__FlashMapDict     : dict                   = copy.deepcopy (FlashMapDict)
        self.__BtGAcmImgInfoDict: Dict[str, ImageRegion] = BtGAcmImgInfoDict
        self.__BgslTemplatePath : str                    = AbsPath (BgslTemplatePath)
        self.__BiosSvn          : str                    = BiosSvn

        self.__PreCheck ()

        self.__BgupInfo  : Dict[str, BgupRegion] = dict ()
        self.__BgupBuffer: ByteBuffer            = self.__GetBgupBuffer ()

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
        return self.__BgupInfo

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
        return self.__BgupBuffer

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) BiosSvn not str type.

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
            BTG_ACM_BGSL_OFFSET_KEY:
                FormatHex (
                  DecToHex (BaseAddress),
                  IsPadding = True,
                  ),
            BTG_ACM_BGSL_SIZE_KEY  :
                FormatHex (
                  DecToHex (ImgInfo.Size),
                  IsPadding = True,
                  ),
            }

        BgslFile: File = File (FilePath = self.__BgslTemplatePath)
        BgslFile.Fill (**Config)

        return BgslFile

    def __GetBtGAcmBgupImg (self) -> ByteBuffer:
        """ Return the merged singed BGUP images buffer.

        Args:
            None.

        Raises:
            None

        Returns:
            ByteBuffer:
                Merged all BGUP images into one buffer.
        """
        BtGAcmBgupImg: bytearray = bytearray ()
        Offset       : int       = 0x0

        for RegionName, _ in self.__BgupInfo.items ():
            self.__BgupInfo[RegionName].Offset = Offset

            BtGAcmBgupImg += self.__BgupInfo[RegionName].Image.Buffer
            Offset        += self.__BgupInfo[RegionName].Size

        return ByteBuffer (Buffer = BtGAcmBgupImg)

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

        SlotNum   : int             = 0
        ImgInfo   : ImageRegion     = None
        BgslFile  : File            = None
        BgslPath  : str             = None
        Generator : BgupGenerateApi = None
        BgupBuffer: ByteBuffer      = None

        for RegionName, ImgInfo in self.__BtGAcmImgInfoDict.items ():
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
            BgslFile = self.__GenerateBgslFile (ImgInfo)
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
                  Prefix       = 'BtGAcm',
                  DataPath     = ImgInfo.Path,
                  )
            Generator.Build ()

            #
            # Add the BGUP information.
            #
            self.__BgupInfo.\
                update ({
                  ImgInfo.Region:
                      BgupRegion (
                        Region   = ImgInfo.Region,
                        FileName = Generator.FileName,
                        Path     = Generator.FilePath,
                        Buffer   = ByteBuffer (FilePath = Generator.FilePath),
                        )
                  })

            SlotNum += 1

        #
        # Merge the BGUP image into one and update offset/length.
        #
        BgupBuffer = self.__GetBtGAcmBgupImg ()

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

class _BtGAcmCfgIniGenerator (object):
    def __init__ (
        self,
        FlashMapDict: dict,
        ImgInfoDict : Dict[str, ImageRegion],
        BgupInfoDict: Dict[str, BgupRegion],
        ) -> None:
        """ Class to support to generate the flash config INI file.

        Args:
            FlashMapDict (dict):
                Flash map information of target platform.
            ImgInfoDict (Dict[str, ImageRegion]):
                BtGAcm image region information dictionary.
                Key - region name, Value - region information.
            BgupInfoDict (Dict[str, BgupRegion]):
                BtGAcm image mapping BGUP information dictionary.
                Key - region name, Value - region BGUP information.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMapDict: dict                   = copy.deepcopy (FlashMapDict)
        self.__ImgInfoDict : Dict[str, ImageRegion] = ImgInfoDict
        self.__BgupInfoDict: Dict[str, BgupRegion]  = BgupInfoDict

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapDict not dict type.
                (2) ImgDict not dict type.
                (3) BgupInfoDict not dict type.
                (4) ImgInfoDict member not ImageRegion type.
                (5) BgupInfoDict member not ImageRegion type.

        Returns:
            None.
        """
        if not isinstance (self.__FlashMapDict, dict):
            raise TypeError ('FlashMapDict should be dict type.')
        elif not isinstance (self.__ImgInfoDict, dict):
            raise TypeError ('ImgInfoDict should be dict type.')
        elif not isinstance (self.__BgupInfoDict, dict):
            raise TypeError ('BgupInfoDict should be dict type.')

        for Region, ImgInfo in self.__ImgInfoDict.items ():
            if not isinstance (ImgInfo, ImageRegion):
                raise TypeError (f'Region {Region} should be ImageRegion type.')

        for Region, BgupImgInfo in self.__BgupInfoDict.items ():
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
              ImageGuid     = uuid.UUID (BTG_ACM_IMG_FILE_GUID),
              ImageInfoDict = self.__ImgInfoDict,
              BgupGuid      = uuid.UUID (BTG_ACM_BGUP_FILE_GUID),
              BgupInfoDict  = self.__BgupInfoDict,
              )

        CfgIniFile: FlashConfig = \
            Generator.ConfigFile (
                        Prefix = 'BtGAcm',
                        )

        CfgIniFile.Save (OutputPath)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    '_BtGAcmSlotInfoGenerator',
    '_BtGAcmBgupGenerator',
    '_BtGAcmCfgIniGenerator',
    ]
