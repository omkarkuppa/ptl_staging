## @file
#  FMP device instance for BIOS firmware.
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
from CapsuleGenerate.ApiPrivate.Edk2FdfGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdGeneratorApiLib import *
from CapsuleGenerate.Const.ConfigFile import *
from CapsuleGenerate.Model.FmpDevice import *

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.BiosFirmware.BiosCapsuleVersion import *
from CapsuleGenerate.Firmwares.BiosFirmware._BiosHelperInternal import *

#
# Pre-defined variable of the FMP instance.
#
BIOS_DEVICE_NAME      : str  = 'Bios'
BIOS_DEVICE_NAME_UPPER: str  = BIOS_DEVICE_NAME.upper ()
BIOS_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[BIOS_DEVICE_NAME]

#
# List of all target BGUP flash region.
#
BGUP_REGION_ORDER: List[str] = [
    BIOS_REGION_OBB,
    BIOS_REGION_IBBR,
    BIOS_REGION_IBB,
    ]

#
# Region flash order of different BIOS image type.
#
BIOS_UPDATE_FLASH_ORDER: dict = {
    BIOS_NORMAL_TYPE    : [
        BIOS_REGION_IBBR,
        BIOS_REGION_IBB,
        BIOS_REGION_OBB,
        ],
    BIOS_RESILIENCY_TYPE: [
        BIOS_REGION_IBB,
        BIOS_REGION_OBB,
        ],
    }

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

BIOS_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if BIOS_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = BIOS_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = BIOS_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = BIOS_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = BIOS_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class Bios (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'BIOS FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == BIOS_BUILD_OP_VER_1):
    BGSL_TEMPLATE_KEY: str = 'BgslTemplateFile'

    BIOS_FLASH_MAP_DATA    : dict = BIOS_FIRMWARE_CONFIG[FLASH_MAP_OFFSET_SECTION]
    BIOS_BGSL_TEMPLATE_PATH: str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          BIOS_FIRMWARE_CONFIG[BGSL_TEMPLATE_KEY],
          )

    BIOS_FDF_FV_NAME: str = 'CapsulePayloadBios'
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_BIOS_LSV : str = '-bioslsv'
    KEYWORD_CMD_BIOS_SVN : str = '-biossvn'
    POSITION_CMD_BIOS_RES: str = 'res'

    class Bios (FmpDevice):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (BIOS ver.1)

            Args:
                Params (Dict[str, Any]):
                    The necessary parameter of this FMP device instance
                    to build capsule file.

            Raises:
                None.

            Returns:
                None.
            """
            self.__Params: Dict[str, Any] = Params
            #
            # Initial the parameter
            #
            self.__BiosPath: str  = AbsPath (Params[KEYWORD_CMD_BIOS_IMG])
            self.__BiosLsv : int  = \
                ToInt (str (self.__Params[KEYWORD_CMD_BIOS_LSV]))
            self.__BiosSvn : str  = \
                GetBiosSvn (
                  self.__Params[KEYWORD_CMD_BIOS_SVN],
                  IsPostCheck = False,
                  )
            self.__ResFlag : bool = bool (self.__Params[POSITION_CMD_BIOS_RES])
            self.__PreCheck ()

            self.__Buffer   : ByteBuffer      = self.__GetImageBuffer ()
            self.__HdrParser: BiosFwHdrParser = BiosFwHdrParser (self.__Buffer)
            self.__BufferHdr: BiosIdHdr       = self.__HdrParser.Header

            #
            # Check if capsule target platform BiosGuard is enabled.
            #
            self.__IsBgupBuild: bool = True

            #
            # Get the BIOS image configuration.
            #
            self.__BiosBoarId  : str  = self.__BufferHdr.BoardId[0:4]
            self.__BuildType   : str  = self.__BufferHdr.BuildType
            self.__TopSwapSize : int  = \
                GetSpiTopSwapSize (BoardId = self.__BiosBoarId)
            self.__BlockSize   : int  = \
                GetSpiFlashBlockSize (BoardId = self.__BiosBoarId)
            self.__BiosType    : str  = \
                self.__GetBiosType (IsAutoDetect = False)
            self.__FlashMapInfo: dict = \
                GetFlashMapInfo (
                  BIOS_FLASH_MAP_DATA,
                  BiosType    = self.__BiosType,
                  BuildTarget = self.__BuildType,
                  BoardId     = self.__BiosBoarId,
                  )

            #
            # Get the version information for BIOS.
            #
            self.__BiosCapVer: BiosCapsuleVersion = \
                BiosCapsuleVersion (
                  Header          = self.__BufferHdr,
                  IsRestrictCheck = False,
                  )
            self.__BiosCapLsv: BiosCapsuleVersion = \
                BiosCapsuleVersion (
                  DecVer          = self.__BiosLsv,
                  IsRestrictCheck = False,
                  )

            self.__PostCheck ()

            self.__Payload: ByteBuffer = self.__GetCapsulePayload ()

        #######################################################################
        #                   Expose Property for FMP Instance                  #
        #######################################################################
        @property
        def BlockSize (self) -> int:
            """ Return the block size for specific platform.

            Args:
                None.

            Raises:
                None.

            Returns:
                int:
                    Value of block size for specific platform.
            """
            return self.__BlockSize

        @property
        def BiosSvn (self) -> str:
            """ Return the BIOS SVN value. (HEX with 0x prefix)

            Args:
                None.

            Raises:
                None.

            Returns:
                str:
                    Value of BIOS SVN HEX value with 0x prefix.
            """
            return self.__BiosSvn

        @property
        def BiosType (self) -> str:
            """ Return the type of the BIOS image.

            Args:
                None.

            Raises:
                None.

            Returns:
                str:
                    Type of BIOS image should be defined in
                    CapsuleCommon/Intel/Bios/ConstantLib file.
            """
            return self.__BiosType

        @property
        def BuildType (self) -> str:
            """ Return the build type of this BIOS image.

            Args:
                None.

            Raises:
                None.

            Returns:
                str:
                    Build type of this BIOS image, it should be defined
                    in one of BUILD_TARGET_TYPE_LIST.
            """
            return self.__BuildType

        @property
        def BiosCapVer (self) -> BiosCapsuleVersion:
            """ Return the capsule version object for this image.

            Args:
                None.

            Raises:
                None.

            Returns:
                BiosCapsuleVersion:
                    The BIOS version converted into capsule version.
            """
            return self.__BiosCapVer

        @property
        def BiosCapLsv (self) -> BiosCapsuleVersion:
            """ Return the capsule version object for capsule LSV.

            Args:
                None.

            Raises:
                None.

            Returns:
                BiosCapsuleVersion:
                    The capsule version to represent the LSV.
            """
            return self.__BiosCapLsv

        #######################################################################
        #                  Private Function for FMP Instance                  #
        #######################################################################
        def __PreCheck (self) -> None:
            """ Check the input argument is valid.

            Args:
                None.

            Raises:
                TypeError:
                    res flag is not bool type.
                ValueError:
                    (1) BIOS firmware not assigned.
                    (2) Caller input the BIOS firmware as ignore.

            Returns:
                None.
            """
            CheckPathList: List[str] = [
                self.__BiosPath,
                ]

            if not ((IsIfwiDecomposed (self.__Params)) or (self.__BiosPath)):
                raise ValueError (
                        'Please at least input '
                        '(1) -ifwi & -fit or '
                        '(2) -bios '
                        'to assign the BIOS FW image.'
                        )
            elif not isinstance (self.__ResFlag, bool):
                raise TypeError ('res flag is not bool type.')

            if self.__BiosPath:
                DEBUG (DEBUG_TRACE, f'Assigned the BIOS FW image.')
                if IsFwImageIgnored (self.__BiosPath):
                    raise ValueError (
                            f'BIOS FW can not ignore when building BIOS capsule .'
                            )

                CheckFileListExist (CheckPathList)

        def __PostCheck (self) -> None:
            """ Post to check after initial the object finished.

            Note:
                (1) Check the BIOS SVN and image version relationship.

            Args:
                None.

            Raises:
                None.

            Returns:
                None.
            """
            #
            # Handle the BiosSvn case.
            #
            BiosVer: str =  self.__BiosCapVer.GetDecVersion ()
            BiosVer = ZeroPadding (BiosVer, 8, IsPaddingRight = True)

            if CheckStringMatch (self.__BiosSvn, NOT_APPLICABLE):
                self.__BiosSvn = BiosVer

            if int (self.__BiosSvn) < int (BiosVer):
                DEBUG (
                  DEBUG_WARN,
                  f'BIOS SVN {self.__BiosSvn} is lower than '
                  f'BIOS version {BiosVer}.'
                  )
            else:
                DEBUG (
                  DEBUG_INFO,
                  f'BIOS SVN is {self.__BiosSvn}, '
                  f'BIOS major version is {BiosVer}.'
                  )

        def __GetImageBuffer (self) -> ByteBuffer:
            """ Read the firmware image from storage to buffer.

            Args:
                None.

            Raises:
                None.

            Returns:
                ByteBuffer:
                    Firmware image buffer within the ByteBuffer wrapper.
            """
            Buffer: Union[None, ByteBuffer] = None

            if self.__BiosPath:
                DEBUG (
                  DEBUG_TRACE,
                  f'User assign the BIOS path: {self.__BiosPath}'
                  )
                CopyFile (
                  self.__BiosPath,
                  BIOS_IMG_FILE_PATH
                  )

            Buffer = ByteBuffer (BIOS_IMG_FILE_PATH)

            return Buffer

        def __GetBiosType (self, IsAutoDetect: bool) -> str:
            """ Identify the type of this BIOS image.

            Args:
                IsAutoDetect (bool):
                    If let program logic to decide the BIOS type.
                    True  - Based on the Top Swap or others condition.
                    False - Based on the caller input.

            Raises:
                None.

            Returns:
                str:
                    Type of BIOS image should be defined in
                    CapsuleCommon/Intel/Bios/ConstantLib file.
            """
            SigLocation: List[int] = self.__HdrParser.SigLocation
            BiosType   : int = BIOS_NORMAL_TYPE
            BufferSize : int = self.__Buffer.Size

            IBBrStart: int = BufferSize - 2 * self.__TopSwapSize
            IBBrEnd  : int = BufferSize - 1 * self.__TopSwapSize

            DEBUG (
              DEBUG_TRACE,
              f'TopSwap size: 0x{DecToHex (self.__TopSwapSize)}'
              )
            DEBUG (
              DEBUG_TRACE,
              f'Buffer size : 0x{DecToHex (BufferSize)}'
              )

            #
            # Check the BIOS ID signature located in IBBr.
            #
            if IsAutoDetect:
                for Offset in SigLocation:
                    if Offset not in range (IBBrStart, IBBrEnd):
                        DEBUG (
                          DEBUG_TRACE,
                          'Found the signature in {Offset} but not between '
                          f'{IBBrStart} to {IBBrEnd}'
                          )
                        continue

                    BiosType = BIOS_RESILIENCY_TYPE
                    DEBUG (
                      DEBUG_TRACE,
                      'Found the signature in IBBr {Offset} between '
                      f'{IBBrStart} to {IBBrEnd}')
            else:
                if self.__ResFlag:
                    BiosType = BIOS_RESILIENCY_TYPE

            return BiosType

        def __GetCapsulePayload (self) -> ByteBuffer:
            """ Generate the capsule payload.

            Note:
                All customize for the firmware image should be proceed
                in here.

            Args:
                None.

            Raises:
                None.

            Returns:
                ByteBuffer:
                    Buffer include the required capsule payload.
            """
            BiosSplitter        : _BiosImgSplitter              = None
            BiosBgupGenerator   : _BiosBgupGenerator            = None
            ConfigIniGenerator  : _BiosCfgIniGenerator          = None
            FdfGenerator        : CapsulePayloadFdfGeneratorApi = None
            Edk2FvBuilder       : Edk2FdGeneratorApi            = None
            BiosImgInfoDict     : dict                          = dict ()
            BiosBgupInfoDict    : dict                          = dict ()
            CapsulePayloadPath  : str                           = None
            CapsulePayloadBuffer: ByteBuffer                    = None

            #
            # 1. Split the image buffer based on the FlashMap info.
            #
            BiosSplitter    = \
                _BiosImgSplitter (
                  BiosBuffer   = self.__Buffer,
                  FlashMapDict = self.__FlashMapInfo,
                  BiosType     = self.__BiosType,
                  )
            BiosImgInfoDict = BiosSplitter.ImgInfo

            #
            # 2. Generate the BiosGuard Update Package (BGUP).
            #
            if self.__IsBgupBuild:
                BiosBgupGenerator = \
                    _BiosBgupGenerator (
                      FlashMapDict     = self.__FlashMapInfo,
                      BiosImgInfoDict  = BiosImgInfoDict,
                      BgupRegionOrder  = BGUP_REGION_ORDER,
                      BgslTemplatePath = BIOS_BGSL_TEMPLATE_PATH,
                      BiosSvn          = self.__BiosSvn,
                      )
                BiosBgupGenerator.Save (BIOS_BGUP_FILE_PATH)

                BiosBgupInfoDict = BiosBgupGenerator.ImgInfo

            #
            # 3. Generate the flash config INI file.
            #
            ConfigIniGenerator = \
                _BiosCfgIniGenerator (
                  FlashMapDict     = self.__FlashMapInfo,
                  BiosUpdateOrder  = BIOS_UPDATE_FLASH_ORDER[self.__BiosType],
                  BiosImgInfoDict  = BiosImgInfoDict,
                  BiosBgupInfoDict = BiosBgupInfoDict,
                  )
            ConfigIniGenerator.Save (BIOS_CFG_FILE_PATH)

            #
            # 4. Generate the FDF file
            #
            FdfGenerator = CapsulePayloadFdfGeneratorApi (BIOS_FDF_FV_NAME, self.__BlockSize)
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_CFG_FILE_GUID),  BIOS_CFG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_IMG_FILE_GUID),  BIOS_IMG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_BGUP_FILE_GUID), BIOS_BGUP_FILE_NAME, None)
            FdfGenerator.Save (EDKII_FDF_FILE_PATH)

            #
            # 5. Build the EDK-II FV for BIOS capsule payload
            #
            Edk2FvBuilder = Edk2FdGeneratorApi (BIOS_FDF_FV_NAME)
            Edk2FvBuilder.Execute ()

            CapsulePayloadPath   = Edk2FvBuilder.FvPath
            CapsulePayloadBuffer = ByteBuffer (FilePath = CapsulePayloadPath)

            return CapsulePayloadBuffer

        #######################################################################
        #               Common Public Function for FMP Instance               #
        #######################################################################
        def GetSignCert (self) -> Tuple[int, SignCert]:
            """ Return the sign certificate object to sign the capsule.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, SignCert]:
                    int:
                        The result of this function get executed.
                    SignCert:
                        The sign certificate object to sign the capsule.
            """
            return FMP_SUCCESS, PLATFORM_CERT_SIGN

        def GetDeviceName (self) -> Tuple[int, str]:
            """ Return the FMP firmware device name.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, str]:
                    int:
                        The result of this function get executed.
                    str:
                        The FMP firmware device name.
            """
            return FMP_SUCCESS, BIOS_DEVICE_NAME

        def GetCapsuleFileName (self) -> Tuple[int, str]:
            """ Return the capsule filename. (Without extension)

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, str]:
                    int:
                        The result of this function get executed.
                    str:
                        The filename of this capsule file.
            """
            CapFileNameList: List[str] = [
                PLATFORM_PREFIX_NAME,
                CAP_FILE_PREFIX,
                self.__BiosType,
                self.__BuildType,
                self.__BiosCapVer.GetDecVersion (),
                CAP_FILE_POSTFIX,
                ]

            CapFileNameList = list (filter (None, CapFileNameList))

            return FMP_SUCCESS, '_'.join (CapFileNameList)

        def GetFwImagePayload (self) -> Tuple[int, ByteBuffer]:
            """ Return the completed Payload buffer.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, ByteBuffer]:
                    int:
                        The result of this function get executed.
                    ByteBuffer:
                        ByteBuffer object that used to build the capsule.
            """
            return FMP_SUCCESS, self.__Payload

        def GetFwImageVersion (self) -> Tuple[int, int]:
            """ Return the firmware image version in capsule.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    int:
                        The UINT32 value to represent the firmware version.
            """
            return FMP_SUCCESS, ToInt (self.__BiosCapVer.GetDecVersion ())

        def GetFwImageLsv (self) -> Tuple[int, int]:
            """ Return the firmware image LSV value within capsule.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    int:
                        The UINT32 value to represent the firmware LSV.
            """
            return FMP_SUCCESS, ToInt (self.__BiosCapLsv.GetDecVersion ())

        def GetFwImageGuid (self) -> Tuple[int, uuid.UUID]:
            """ Return the GUID(ImageTypeId) number of firmware image.

            Args:
                None.

            Raises:
                None

            Returns:
                Tuple[int, uuid.UUID]:
                    int:
                        The result of this function get executed.
                    uuid.UUID:
                        The GUID number of this capsule file for specific
                        FMP device.
            """
            IsFound: bool = False
            Value  : Any  = None

            IsFound, Value = SearchKeyInDict (GUID_DATA, DEFAULT)
            if not IsFound:
                raise ValueError (f'Not assigned the {DEFAULT} key.')

            return FMP_SUCCESS, uuid.UUID (GUID_DATA[Value])

        def GetHardwareInstance (self) -> Tuple[int, int]:
            """ Return the capsule hardware instance number.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    int:
                        The hardware instance number of this capsule file
                        for specific FMP device. (UINT64)
            """
            return FMP_UNSUPPORTED, FMP_HW_INST_DEFAULT_VALUE

        #######################################################################
        #       The following functions are prepared for Windows Capsule      #
        #######################################################################
        def GetVendor (self) -> Tuple[int, str]:
            """ Return the vendor name of this capsule file.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    str:
                        The vendor name that provide this capsule file.
            """
            return FMP_SUCCESS, VENDOR_NAME

        def GetCapsuleString (self) -> Tuple[int, str]:
            """ Return the firmware image version in dot format.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    str:
                        The firmware image version in dot format.
            """
            return FMP_SUCCESS, self.__BiosCapVer.GetDecDotVersion ()

        def GetCapsuleDescription (self) -> Tuple[int, str]:
            """ Return the description of firmware image.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[int, int]:
                    int:
                        The result of this function get executed.
                    str:
                        The description of firmware image.
            """
            Status: int = 0
            Vendor: str = str ()
            VerStr: str = str ()

            Status, Vendor = self.GetVendor ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get the vendor name.')

            Status, VerStr = self.GetCapsuleString ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get the capsule string.')

            return FMP_SUCCESS, ' '.join ([
                                      Vendor,
                                      PLATFORM_FULL_NAME,
                                      BIOS_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
