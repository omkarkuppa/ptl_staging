## @file
#  Library to generate the Flash configuration INI file.
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
from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.Firmwares.FlashRegionLib import *

#
# Head section.
#
CFG_HEAD_SECTION_NAME : str = 'Head'
HEAD_NUM_OF_UPDATE    : str = 'NumOfUpdate'
HEAD_IMAGE_GUID_NAME  : str = 'ImageFileGuid'
HEAD_IMAGE_HELPER_GUID: str = 'HelperFileGuid'

#
# Update Region section.
# In Head section, it should be UpdateX = YYYY.
# where X is the unsigned integer and YYYY is update region name.
#
KEY_BASE_ADDRESS : str = 'BaseAddress'
KEY_IMAGE_LENGTH : str = 'Length'
KEY_IMAGE_OFFSET : str = 'ImageOffset'
KEY_HELPER_OFFSET: str = 'HelperOffset'
KEY_HELPER_LENGTH: str = 'HelperLength'

#
# Header of this configuration file.
#
CONFIG_FILE_HEADER: str = f'''\
## @file
#  Config file template to describe FV image address and target
#  flash address used for flash update.
#
#  Note: Automated generated file. DO NOT MODIFY THIS FILE.
#
#  @copyright
{INTEL_COPYRIGHT_STR}
#
#  @par Specification
#
##

'''

class FlashConfig (INICfgFile):
    def __init__ (
        self,
        ImageGuid : uuid.UUID,
        HelperGuid: uuid.UUID,
        ) -> None:
        """ Class support to generate the INI configuration file.

        Note:
            This INI file is used to control the flash region and step.

        Args:
            ImageGuid (uuid.UUID):
                The GUID used to represent the updatable image.
            HelperGuid (uuid.UUID):
                The GUID used to represent the BGUP used for BiosGuard.

        Raises:
            None.

        Returns:
            None.
        """
        super ().__init__ ()
        self.__ImageGuid : uuid.UUID = ImageGuid
        self.__HelperGuid: uuid.UUID = HelperGuid

        self.__PreCheck ()

        self.__RegionCnt: int = 0

        self.__InitHeadSection ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) ImageGuid not UUID type.
                (2) HelperGuid not UUID type.

        Returns:
            None.
        """
        if not isinstance (self.__ImageGuid, uuid.UUID):
            raise TypeError ('ImageGuid should be UUID type.')
        elif not isinstance (self.__HelperGuid, uuid.UUID):
            raise TypeError ('HelperGuid should be UUID type.')

    def __InitHeadSection (self) -> None:
        """ Initial the Head section information.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        ImageGuidStr : str = (f'{self.__ImageGuid}').upper ()
        HelperGuidStr: str = (f'{self.__HelperGuid}').upper ()

        MappingData: list = [
            [HEAD_NUM_OF_UPDATE,     f'{self.__RegionCnt}'                        ],
            [HEAD_IMAGE_GUID_NAME,   f'{ImageGuidStr} # gCapsuleBiosImageFileGuid'],
            [HEAD_IMAGE_HELPER_GUID, f'{HelperGuidStr} # gCapsuleBiosBgupFileGuid'],
            ]
        super ().UpdateDataToSection (CFG_HEAD_SECTION_NAME, MappingData)

    def AddUpdateRegion (
        self,
        RegionName : str,
        BaseAddress: int,
        ImageInfo  : ImageRegion,
        BgupInfo   : Union[None, BgupRegion],
        ) -> None:
        """ Expose the function to add the update region.

        Note:
            The order is based the function call.

        Args:
            RegionName (str):
                The name of update region.
            BaseAddress (int):
                The base address of this region.
            ImageInfo (ImageRegion):
                The information of this image for this region.
            BgupInfo (Union[None, BgupRegion]):
                The information of this BGUP image for this region.
                (Assign this as None when no BGUP support)

        Raises:
            TypeError:
                (1) RegionName not str type.
                (2) BaseAddress not int type.
                (2) ImageInfo not ImageRegion type.
                (3) BgupInfo not None or BgupRegion type.
            ValueError:
                (1) BaseAddress not UINT64.
                (2) BaseAddress+Length not UINT64.

        Returns:
            None.
        """
        if not isinstance (RegionName, str):
            raise TypeError ('RegionName should be str type.')
        elif not isinstance (BaseAddress, int):
            raise TypeError ('BaseAddress should be int type.')
        elif not isinstance (ImageInfo, ImageRegion):
            raise TypeError ('ImageInfo should be ImageRegion type.')
        elif not ((BgupInfo is None) or (isinstance (BgupInfo, BgupRegion))):
            raise TypeError ('BgupInfo should be None/BgupRegion type.')

        Length      : int = ImageInfo.Size
        ImageOffset : int = ImageInfo.Offset
        HelperOffset: int = 0x0000_0000_0000_0000
        HelperLength: int = 0x0000_0000_0000_0000

        if BgupInfo is not None:
            HelperOffset = BgupInfo.Offset
            HelperLength = BgupInfo.Size

        if not IsUint64 (BaseAddress):
            raise ValueError ('BaseAddress should be UINT64.')
        elif not IsUint64 (BaseAddress + Length):
            raise ValueError ('BaseAddress+Size should be UINT64.')

        #
        # Add the update region by region name.
        # (Each region is consider as a section within INI file)
        #
        MappingData: list = [
            [KEY_BASE_ADDRESS,  f'{FormatHex (BaseAddress,  IsPadding = True, PaddingSize = 16)}'],
            [KEY_IMAGE_LENGTH,  f'{FormatHex (Length,       IsPadding = True, PaddingSize = 16)}'],
            [KEY_IMAGE_OFFSET,  f'{FormatHex (ImageOffset,  IsPadding = True, PaddingSize = 16)}'],
            [KEY_HELPER_OFFSET, f'{FormatHex (HelperOffset, IsPadding = True, PaddingSize = 16)}'],
            [KEY_HELPER_LENGTH, f'{FormatHex (HelperLength, IsPadding = True, PaddingSize = 16)}'],
            ]
        super ().UpdateDataToSection (RegionName, MappingData)

        #
        # Add the update region counter.
        #
        self.__RegionCnt += 1

        #
        # Update the information within Head section.
        #
        MappingData = [
            #
            # 0-based.
            #
            [f'Update{self.__RegionCnt-1}', f'{RegionName}'      ],
            #
            # 1-based.
            #
            [HEAD_NUM_OF_UPDATE,            f'{self.__RegionCnt}'],
            ]
        super ().UpdateDataToSection (CFG_HEAD_SECTION_NAME, MappingData)

    def Save (
        self,
        FilePath: Union[str, os.PathLike],
        ) -> None:
        """ Save the flash configuration file to storage.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path to save the flash configuration file.

        Raises:
            None.

        Returns:
            None.
        """
        SavePath: str = AbsPath (FilePath)

        #
        # The header of Windows INF file.
        #
        HeaderFile = File (FileContent = CONFIG_FILE_HEADER)
        HeaderFile.Save (SavePath)

        #
        # Export the config file content.
        #
        super ().Save (SavePath, FileMode = 'a')

class FlashConfigIniGenerator (object):
    def __init__ (
        self,
        FlashMap     : Dict[str, dict],
        ImageGuid    : uuid.UUID,
        ImageInfoDict: Dict[str, ImageRegion],
        BgupGuid     : uuid.UUID,
        BgupInfoDict : Dict[str, BgupRegion],
        ) -> None:
        """ Provide trivial flow to generate the flash config INI file.

        Args:
            FlashMap (Dict[str, dict]):
                Flash map information of for this target platform.
            ImageGuid (uuid.UUID):
                The GUID used to represent the updatable image.
            ImageInfoDict (Dict[str, ImageRegion]):
                List of information for this image within this region.
            BgupGuid (uuid.UUID):
                The GUID used to represent the BGUP used for BiosGuard.
            BgupInfoDict (Dict[str, BgupRegion]):
                List of the information for this BGUP image within this
                region.
                (Assign this as None when no BGUP support)

        Raises:
            None.

        Returns:
            None.
        """
        self.__FlashMap     : dict                   = FlashMap
        self.__ImageGuid    : uuid.UUID              = ImageGuid
        self.__ImageInfoDict: Dict[str, ImageRegion] = ImageInfoDict
        self.__BgupGuid     : uuid.UUID              = BgupGuid
        self.__BgupInfoDict : Dict[str, BgupRegion]  = BgupInfoDict

    def ConfigFile (self, Prefix: str, Order: List[str] = None) -> FlashConfig:
        """ Return the well-built flash config INI file.

        Args:
            Prefix (str):
                The prefix string for the update order name.
            Order (List[str], optional):
                The order to be updated.
                Each element should be matched into region name.
                Defaults to None.

        Raises:
            TypeError:
                (1) Prefix not str type.
                (2) Order not lis type.
            ValueError:
                Assigned update region not exist.

        Returns:
            FlashConfig:
                Well built flash config INI file.
        """
        UpdateOrder: List[str]   = list (self.__ImageInfoDict.keys ())
        IniConfig  : FlashConfig = \
            FlashConfig (
              ImageGuid  = self.__ImageGuid,
              HelperGuid = self.__BgupGuid,
              )
        IsFound    : bool        = False
        Value      : Any         = None
        BaseAddress: int         = None
        ImageInfo  : ImageRegion = None
        BgupInfo   : BgupRegion  = None

        if not isinstance (Prefix, str):
            raise TypeError (f'Prefix should be str type.')

        if Order is not None:
            if not isinstance (Order, list):
                raise TypeError (f'Order should be list type.')
            UpdateOrder = Order

        DEBUG (DEBUG_TRACE, f'Update order: {UpdateOrder}')

        for Region in UpdateOrder:
            IsFound, Value = SearchKeyInDict (self.__FlashMap, Region)
            if not IsFound:
                raise ValueError (
                        f'FlashMap is not defined region [{Region}] offset.'
                        )
            BaseAddress = ToInt (self.__FlashMap[Value][FM_OFFSET_KEY])

            if Region not in self.__ImageInfoDict:
                raise ValueError (
                        f'Can not find region {Region} in image info.'
                        )

            ImageInfo = self.__ImageInfoDict[Region]
            if Region not in self.__BgupInfoDict:
                BgupInfo = None
            else:
                BgupInfo = self.__BgupInfoDict[Region]

            IniConfig.AddUpdateRegion (
                        RegionName  = f'{Prefix}{ImageInfo.Region}',
                        BaseAddress = BaseAddress,
                        ImageInfo   = ImageInfo,
                        BgupInfo    = BgupInfo,
                        )

        return IniConfig

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'FlashConfig',
    'FlashConfigIniGenerator',
    ]
