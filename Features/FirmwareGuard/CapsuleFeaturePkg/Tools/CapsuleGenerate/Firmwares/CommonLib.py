## @file
#  FMP device common function share across different FMP instance.
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
from typing import List, Tuple

from CapsuleCommon import *

from CapsuleGenerate.Firmwares.ConstLib import *
from CapsuleGenerate.Firmwares.CertSignLib import *
from CapsuleGenerate.Firmwares.FlashRegionLib import *
from CapsuleGenerate.Firmwares.FlashConfigLib import *

def IsIfwiDecomposed (Params: Dict[str, Any]) -> bool:
    """ Check if IFWI is decomposed via FIT tool.

    Note:
        (1) Decompose process should be executed before the check.
        (2) Only check if IFWI and FIT tool path is valid.

    Args:
        Params (Dict[str, Any]):
            The parameter that user input.
            (If the parameter is optional and user not given, fill it
             with default value)

    Raises:
        None.

    Returns:
        bool:
            False - IFWI is not decomposed via FIT tool.
            True  - IFWI and FIT tool path is valid and decomposed.
    """
    IsDecomposed: bool = False
    IfwiPath    : str  = AbsPath (Params[KEYWORD_CMD_IFWI_IMG])
    FitToolPath : str  = AbsPath (Params[KEYWORD_CMD_FIT_TOOL])

    if not IfwiPath:
        DEBUG (DEBUG_TRACE, "IFWI path is not assigned.")
    elif not FitToolPath:
        DEBUG (DEBUG_TRACE, "FIT tool path is not assigned.")
    elif not IsFile (IfwiPath):
        DEBUG (DEBUG_TRACE, "Input IFWI path is not file. (Invalid)")
    elif not IsFile (FitToolPath):
        DEBUG (DEBUG_TRACE, "Input FIT tool path is not file. (Invalid)")
    else:
        IsDecomposed = True

    return IsDecomposed

def IsFwImageIgnored (FilePath: str) -> bool:
    """ Check if caller input the IGNORE keyword.

    Args:
        FilePath (str):
            Path of the file.

    Raises:
        None.

    Returns:
        bool:
            True  - The input string is matched the IGNORE keyword.
            False - The input string is not matched the IGNORE keyword.
    """
    FileName: str  = os.path.basename (FilePath)
    IsIgnore: bool = False

    if CheckStringMatch (FileName, IGNORE, IsIgnoreCase = True):
        IsIgnore = True

    return IsIgnore

def IsRegionInfoAssigned (
    DataDict: dict,
    Region  : str
    ) -> Tuple[bool, str]:
    """ Check if specific region in input information data is assigned.

    Note:
        (1) If value of this region is str type, check if assign with
            NOT_APPLICABLE.
        (2) If value of this region is None assume it is not assigned.
        (3) If value of this region is other types,
            assume it is assigned.

    Args:
        DataDict (dict):
            The information data for the target platform configuration.
        Region (str):
            The name of the region.

    Raises:
        TypeError:
            (1) DataDict not dict type.
            (2) Region not str type.
        ValueError:
            Region str type only allow NOT_APPLICABLE.

    Returns:
        Tuple[bool, str]:
            bool:
                True  - Specific region is assigned.
                False - Specific region is not assigned.
            str:
                Specific region name presented within config file.
    """
    IsFound   : bool = None
    RegionName: str  = None
    RegionInfo: Any  = None

    if not isinstance (DataDict, dict):
        raise TypeError ('DataDict should be dict type.')
    elif not isinstance (Region, str):
        raise TypeError ('Region should be str type.')

    IsFound, RegionName = SearchKeyInDict (DataDict, Region)
    if not IsFound:
        DEBUG (DEBUG_TRACE, f'Failed to find the [{RegionName}] region key.')
        return False, None

    RegionInfo = DataDict[RegionName]
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

def GetBiosSvn (InputStr: str, IsPostCheck: bool = False) -> str:
    """ Return the BIOS SVN value. (Without 0x prefix)

    Note:
        (1) If input is NA, means parse from BIOS automatically.
        (2) BIOS SVN is consist with 0xYWWDmmRR,
            Y  - The year of this project for this build.
            WW - The workweek of this project for this build.
            D  - The workday of this project for this build.
            mm - The dot build number of this project for this build.
        (3) The valid value should be as the following.
            A. XXX      - BIOS major. (0 year)
            B. XXXX     - BIOS major.
            C. XXXXX    - BIOS major + minor version. (0 year)
            D. XXXXXX   - BIOS major + minor version.
            E. XXXXXXXX - Assign the SVN directly.

    Args:
        InputStr (str):
            Caller input SVN string.
            (Should be aligned with BIOS version)
            Not matter it is included the HEX prefix, consider the value
            is HEX.
        IsPostCheck (bool, optional):
            Decide if BIOS SVN need to check it numeric logic.
            (Restrict Mode)
            True  - Check the value is rational.
            False - Not to check the value in the last step.
            Defaults to False.

    Raises:
        TypeError:
            Caller input value not str type.

        ValueError:
            (1) Input value not UINT32.
            (2) BIOS SVN not valid length.
            (3) BIOS version year part not in 0-9.
            (4) BIOS version workweek part not in 1-53.
            (5) BIOS version workday part not in 1-7.
            (6) BIOS version dot build version part not in 0-99.

    Returns:
        str:
            The BIOS SVN value. (Without 0x prefix)
    """
    Year    : int = int ()
    Workweek: int = int ()
    Workday : int = int ()
    Build   : int = int ()
    Rsvd    : int = int ()
    BiosSvn : str = str ()

    #
    # If BiosSvn is NA means no input, return directly.
    # It is caller responsibility to handle the no input case.
    #
    if CheckStringMatch (InputStr, NOT_APPLICABLE):
        return NOT_APPLICABLE

    if not isinstance (InputStr, str):
        raise TypeError (f'InputStr should be str type.')
    elif not IsUint32 (HexToDec (InputStr)):
        raise ValueError (f'InputStr should be UINT32')

    BiosSvn = DecToHex (HexToDec (InputStr)) # Standardize the format.
    #
    # Assume the Year is mark as 0, do the padding on left.
    #
    if len (BiosSvn) not in [3, 4, 5, 6, 8]:
        raise ValueError (f'BIOS SVN format is not correct.')
    if len (InputStr) == 3:
        BiosSvn = ZeroPadding (BiosSvn, 4)
    elif len (InputStr) == 5:
        BiosSvn = ZeroPadding (BiosSvn, 6)

    BiosSvn = ZeroPadding (BiosSvn, 8, IsPaddingRight = True)

    DEBUG (DEBUG_INFO, f'Assign the BIOS SVN: 0x{BiosSvn}')

    #
    # Post-Check the component of BIOS SVN is rational.
    #
    if not IsPostCheck:
        return BiosSvn

    Year     = int (BiosSvn[0:1])
    Workweek = int (BiosSvn[1:3])
    Workday  = int (BiosSvn[3:4])
    Build    = int (BiosSvn[4:6])
    Rsvd     = int (BiosSvn[6:8])

    if Year not in range (0, 10):
        raise ValueError (f'The year part [{Year}] should be 0-9.')
    elif Workweek not in range (1, 54):
        raise ValueError (f'The work week part [{Workweek}] should be 1-53.')
    elif Workday not in range (1, 8):
        raise ValueError (f'The work day part [{Workday}] should be 1-7.')
    elif Build not in range (0, 100):
        raise ValueError (f'The build number part [{Build}] should be 0-99.')
    elif Rsvd != 0:
        raise ValueError (f'The reserved part [{Rsvd}] should be 0.')
    else:
        DEBUG (DEBUG_INFO, f'BIOS SVN check passed!')

    return BiosSvn

def GetConfigDataByBoard (
    ConfigData : dict,
    BoardId    : str,
    IsException: bool = True,
    ) -> Any:
    """ Return the specific configuration data by board ID (SKU).

    Note:
        If assigned board ID is not found, would find the default key.

    Args:
        ConfigData (dict):
            The mapping configuration data.
            Key is SKU (or Default), Value is correspond data.
        BoardId (str):
            Assign the board ID for different config on different SKU.
        IsException (bool, optional):
            The way to handle the not found case.
            True  - Raise the exception to block the program execution.
            False - Keep the program execute, return None.
            Defaults to True.

    Raises:
        TypeError:
            Input board ID not str type.
        ValueError:
            (1) BoardId is empty.
            (2) Failed to find the mapping.

    Returns:
        Any:
            The mapping configuration data of specific SKU.
    """
    BoardName: str  = BoardId
    IsFound  : bool = None
    Value    : str  = None

    DEBUG (DEBUG_TRACE, f'Get the config data for {BoardId}')

    if not isinstance (BoardId, str):
        raise TypeError (f'Board ID is not str type.')
    elif not BoardId:
        raise ValueError (f'Board ID can not empty.')

    #
    # Try to find the with the correspond key.
    #
    IsFound, Value = SearchValueInList (list (ConfigData.keys ()), BoardName)
    if IsFound:
        return ConfigData[Value]

    #
    # Attempt to find the default value data.
    #
    DEBUG (
        DEBUG_WARN,
        f'Not found the board specific value, set it as {DEFAULT}'
        )
    BoardName      = DEFAULT
    IsFound, Value = SearchValueInList (list (ConfigData.keys ()), BoardName)
    if IsFound:
        return ConfigData[Value]

    #
    # Handle the not found case.
    #
    if IsException:
        raise ValueError (f'Can not find the mapping data: {BoardName}')

    return None

def GetSpiTopSwapSize (BoardId: str = DEFAULT) -> int:
    """ Return the SPI top swap configuration.

    Args:
        BoardId (str, optional):
            Assign the board ID for different config on different SKU.
            Defaults to DEFAULT.

    Raises:
        None.

    Returns:
        int:
            Top swap size configuration on specific platform.
    """
    Value     : str  = GetConfigDataByBoard (SPI_TOP_SWAP_SIZE_DATA, BoardId)
    TopSwapVal: int  = HexToDec (Value)

    DEBUG (
      DEBUG_TRACE,
      f'Set the Top Swap size as [0x{DecToHex (TopSwapVal)}]'
      )

    return TopSwapVal

def GetSpiFlashBlockSize (BoardId: str = DEFAULT) -> int:
    """ Return the SPI flash block size configuration.

    Args:
        BoardId (str, optional):
            Assign the board ID for different config on different SKU.
            Defaults to DEFAULT.

    Raises:
        None.

    Returns:
        int:
            Flash block size configuration on specific platform.
    """
    Value     : str  = GetConfigDataByBoard (SPI_FLASH_BLOCK_SIZE_DATA, BoardId)
    TopSwapVal: int  = HexToDec (Value)

    DEBUG (
      DEBUG_TRACE,
      f'Set the flash block size size as [0x{DecToHex (TopSwapVal)}]'
      )

    return TopSwapVal

def GetFlashMapInfo (
    OffsetData : dict,
    BiosType   : str,
    BuildTarget: str,
    BoardId    : str
    ) -> dict:
    """ Return the correspond flash map data by specific config.

    Note:
        (1) The search order is defined in KEY_ORDER.
            (BiosType > BuildTarget > BoardId)
        (2) If all the order is not found, attempt to find the default.

    Args:
        OffsetData (dict):
            The flash map offset data for different SKU, build target,
            or BIOS type.
        BiosType (str):
            The type of the BIOS.
            (Defined in CapsuleCommon/Intel/Bios/ConstantLib.py)
        BuildTarget (str):
            The build target of the BIOS.
            (Should be one of BUILD_TARGET_TYPE_LIST)
        BoardId (str):
            The board ID (SKU) of the BIOS.

    Raises:
        TypeError:
            OffsetData should be dict type.
        ValueError:
            The assigned

    Returns:
        dict:
            The correspond flash map data for specific config.
    """
    IsFound     : bool      = None
    Value       : dict      = None
    KeyParseList: List[str] = list ()

    KEY_ORDER   : List[str] = [
        BiosType,
        BuildTarget,
        BoardId,
        ]

    if not isinstance (OffsetData, dict):
        raise TypeError (f'OffsetData should be dict type.')

    Data   : dict = copy.deepcopy (OffsetData)
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

def GetFvRegionInfo (FlashMap: dict, Region: str) -> Tuple[str, int, int]:
    """ Return the correspond FV information within the flashmap.

    Args:
        FlashMap (dict):
            The flashmap data for the target platform configuration.
        Region (str):
            The name of the region.

    Raises:
        ValueError:
            (1) Region is not defined within flashmap.
            (2) Region assigned the NOT_APPLICABLE value.
            (3) Region is incorrect format.
            (4) FM_OFFSET_KEY is not assigned within the region data.
            (5) FM_OFFSET_KEY assigned the NOT_APPLICABLE value.
            (6) FM_SIZE_KEY is not assigned within the region data.
            (7) FM_SIZE_KEY assigned the NOT_APPLICABLE value.

    Returns:
        Tuple[str, int, int]:
            str:
                The region name.
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

    IsAssigned, RegionName = IsRegionInfoAssigned (
                               DataDict = FlashMap,
                               Region   = Region,
                               )

    if not IsAssigned:
        raise ValueError (f'Failed to find the [{Region}] region key.')

    #
    # Check the region within flash map is assigned.
    #
    FvInfo = FlashMap[RegionName]
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
        raise TypeError (f'Region data should be dict type.')

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
