## @file
#  Library for the object used on flash map.
#
#  Note:
#    Object is used to provide the necessary info for specific region.
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

from typing import Tuple

from CapsuleCommon import *

from CapsuleGenerate._Global import *

class FlashMap (object):
    def __init__ (
        self,
        FlashMapData: dict,
        BiosType    : str = DEFAULT,
        BuildTarget : str = DEFAULT,
        BoardId     : str = DEFAULT,
        ):
        """ Class to handle the trivial flash map operation.

        Args:
            FlashMapData (dict):
                The flash map offset data for all of the config.
            BiosType (str, optional):
                The type of the BIOS.
                (Defined in CapsuleCommon/Intel/Bios/ConstantLib.py)
                Defaults to DEFAULT.
            BuildTarget (str, optional):
                The build target of the BIOS.
                (Should be one of BUILD_TARGET_TYPE_LIST)
                Defaults to DEFAULT.
            BoardId (str, optional):
                The board ID (SKU) of the BIOS.
                Defaults to DEFAULT.

        Raises:
            None.

        Return:
            None.
        """
        self.__FlashMapData: dict = FlashMapData
        self.__BiosType    : str  = BiosType
        self.__BuildTarget : str  = BuildTarget
        self.__BoardId     : str  = BoardId

        self.__PreCheck ()

        self.__FlashMapConfig: dict = self.__GetFlashMapConfig ()

    @property
    def Config (self) -> dict:
        """ Get the correspond flash map data by specific config.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                The correspond flash map data for specific config.
        """
        return self.__FlashMapConfig

    @property
    def RegionList (self) -> List[str]:
        """ Get the list of region name presented in flash map.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[str]:
                List of region name presented in flash map.
        """
        return list (self.__FlashMapConfig.keys ())

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FlashMapData not dict type.
                (2) BiosType not str type.
                (3) BuildTarget not str type.
                (4) BoardId not str type.

        Returns:
            None.
        """

        if not isinstance (self.__FlashMapData, dict):
            raise TypeError ('FlashMapData shall be dict type.')
        elif not isinstance (self.__BiosType, str):
            raise TypeError ('BiosType shall be str type.')
        elif not isinstance (self.__BuildTarget, str):
            raise TypeError ('BuildTarget shall be str type.')
        elif not isinstance (self.__BoardId, str):
            raise TypeError ('BoardId shall be str type.')

    def __GetFlashMapConfig (self) -> dict:
        """ Return the correspond flash map data by specific config.

        Note:
            (1) The search order is defined in KEY_ORDER.
                (BiosType > BuildTarget > BoardId)
            (2) If all the order is not found, attempt to find the default.

        Args:
            None.

        Raises:
            ValueError:
                Failed to get specific config from the input conditions.

        Returns:
            dict:
                The correspond flash map data for specific config.
        """
        IsFound     : bool      = None
        Value       : dict      = None
        KeyParseList: List[str] = list ()

        KEY_ORDER   : List[str] = [
            self.__BiosType,
            self.__BuildTarget,
            self.__BoardId,
            ]

        Data   : dict = copy.deepcopy (self.__FlashMapData)
        IsFound: bool = None
        Value  : dict = None

        #
        # Search the config by the specific order.
        #
        for Key in KEY_ORDER:
            IsFound, Value = SearchKeyInDict (Data, Key)
            if IsFound:
                Data = Data[Value]
                KeyParseList.append (Key)

        #
        # Handle the case when assigned config is not found.
        #
        if not IsFound:
            IsFound, Value = SearchKeyInDict (Data, DEFAULT)
            if IsFound:
                Data = Data[Value]
                KeyParseList.append (DEFAULT)

        DEBUG (
          DEBUG_TRACE,
          'Parse the flashmap with to following '
          f'section: [{".".join (KeyParseList)}] Data: [{Data}]'
          )

        #
        # Raise the exception when assigned config and DEFAULT is not found.
        #
        if not IsFound:
            raise ValueError (f'!!! Failed to parse the flashmap info. !!!')

        return Data

    def IsRegionInfoAssigned (self, Region: str) -> Tuple[bool, str]:
        """ Check if specific region assigned in flash map.

        Note:
            (1) If value of this region is str type, check if assign with
            NOT_APPLICABLE.
            (2) If value of this region is None assume it is not assigned.
            (3) If value of this region is other types,
                assume it is assigned.

        Args:
            Region (str):
                The name of the region.

        Raises:
            TypeError:
                Region not str type.
            ValueError:
                Region str type only allow NOT_APPLICABLE.

        Returns:
            Tuple[bool, str]:
                bool:
                    True  - Specific region is assigned.
                    False - Specific region is not assigned.
                str:
                    Specific region name presented within flash map.
        """
        IsFound   : bool = None
        RegionName: str  = None
        RegionInfo: Any  = None

        if not isinstance (Region, str):
            raise TypeError (f'Region shall be str type.')

        IsFound, RegionName = SearchKeyInDict (self.__FlashMapConfig, Region)
        if not IsFound:
            DEBUG (DEBUG_TRACE, f'Failed to find the [{Region}] region key.')
            return False, None

        RegionInfo = self.__FlashMapConfig[RegionName]
        if isinstance (RegionInfo, type (None)):
            return False, RegionName
        elif isinstance (RegionInfo, str):
            if not CheckStringMatch (RegionInfo, NOT_APPLICABLE):
                raise ValueError (
                        f'Region info str type should be only {NOT_APPLICABLE}.'
                        )

            DEBUG (
              DEBUG_TRACE,
              f'Region [{RegionName}] present but set {NOT_APPLICABLE}.'
              )

            return False, RegionName

        return True, RegionName

    def GetRegionAssignedList (
        self,
        RegionList: List[str]
        ) -> List[str]:
        """ Get the list of region assigned in flash map.

        Args:
            RegionList (List[str]):
                List of region to be checked if assigned.

        Raises:
            TypeError:
                (1) RegionList shall be list type.
                (2) Member of RegionList shall be str type.

        Returns:
            List[str]:
                List of region assigned in flash map.
        """
        Region            : str       = None
        IsFound           : bool      = None
        RegionName        : str       = None
        RegionAssignedList: List[str] = list ()

        if not isinstance (RegionList, list):
            raise TypeError (f'RegionList shall be list type.')

        for Region in RegionList:
            if not isinstance (Region, str):
                raise TypeError ('Member of RegionList shall be str type.')

            IsFound, RegionName = self.IsRegionInfoAssigned (Region = Region)
            if not IsFound:
                DEBUG (DEBUG_TRACE, f'[{RegionName}] - is not assigned.')
                continue

            RegionAssignedList.append (RegionName)

        return RegionAssignedList

    def GetRegionInfo (self, Region: str) -> Tuple[str, int, int]:
        """ Return the correspond region information within the flashmap.

        Args:
            Region (str):
                The name of the region.

        Raises:
            TypeError:
                Region not str type.
            ValueError:
                (1) Region not defined within flashmap.
                (2) Region assigned the NOT_APPLICABLE value.
                (3) Region in incorrect format.
                (4) FM_OFFSET_KEY not assigned within the region data.
                (5) FM_OFFSET_KEY assigned the NOT_APPLICABLE value.
                (6) FM_SIZE_KEY not assigned within the region data.
                (7) FM_SIZE_KEY assigned the NOT_APPLICABLE value.

        Returns:
            Tuple[str, int, int]:
                str:
                    The name of the region in flash map.
                int:
                    Offset value of this region.
                int:
                    Size value of this region.
        """
        IsAssigned: bool = None
        RegionName: str  = None
        Value     : Any  = None
        FvInfo    : dict = None
        Offset    : int  = None
        Size      : int  = None

        if not isinstance (Region, str):
            raise TypeError (f'Region shall be str type.')

        IsAssigned, RegionName = self.IsRegionInfoAssigned (Region = Region)
        if not IsAssigned:
            raise ValueError (f'Failed to find the [{Region}] region key.')

        #
        # Check the region within flash map is assigned.
        #
        FvInfo = self.__FlashMapConfig[RegionName]
        if isinstance (FvInfo, str):
            if CheckStringMatch (FvInfo, NOT_APPLICABLE):
                raise ValueError (
                        f'Region [{RegionName}] not assign the region data.'
                        )
            else:
                raise ValueError (
                        f'Region [{RegionName}] not assign the valid format.'
                        )
        elif not isinstance (FvInfo, dict):
            raise TypeError (f'Region data shall be dict type.')

        #
        # Attempt to get the region offset data.
        #
        IsFound, Value = SearchKeyInDict (FvInfo, FM_OFFSET_KEY)
        if not IsFound:
            raise ValueError (
                    f'Need to assign {FM_OFFSET_KEY} key within flashmap data.'
                    )
        elif CheckStringMatch (FvInfo[Value], NOT_APPLICABLE):
            raise ValueError (
                    f'{FM_OFFSET_KEY} for {RegionName} region is {NOT_APPLICABLE}.'
                    )

        Offset = ToInt (FvInfo[Value])

        #
        # Attempt to get the region size data.
        #
        IsFound, Value = SearchKeyInDict (FvInfo, FM_SIZE_KEY)
        if not IsFound:
            raise ValueError (
                    f'Need to assign {FM_SIZE_KEY} key within flashmap data.'
                    )
        elif CheckStringMatch (FvInfo[Value], NOT_APPLICABLE):
            raise ValueError (
                    f'{FM_SIZE_KEY} for {RegionName} region is {NOT_APPLICABLE}.'
                    )

        Size = ToInt (FvInfo[Value])

        return RegionName, Offset, Size

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'FlashMap',
    ]
