## @file
#  FMP device instance for BtGAcm firmware.
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
from CapsuleGenerate.Const.ConfigFile import *
from CapsuleGenerate.Model.FmpDevice import *

from CapsuleGenerate.ApiPrivate.Edk2FdfGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdGeneratorApiLib import *

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.BtGAcmFirmware.BtGAcmCapsuleVersion import *
from CapsuleGenerate.Firmwares.BtGAcmFirmware._BtGAcmHelperInternal import *

#
# Pre-defined variable of the FMP instance.
#
BTG_ACM_DEVICE_NAME      : str  = 'Btgacm'
BTG_ACM_DEVICE_NAME_UPPER: str  = BTG_ACM_DEVICE_NAME.upper ()
BTG_ACM_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[BTG_ACM_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

BTG_ACM_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if BTG_ACM_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = BTG_ACM_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = BTG_ACM_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = BTG_ACM_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = BTG_ACM_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class BtGAcm (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'BtGAcm FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == BTG_ACM_BUILD_OP_VER_1):
    #
    # Pre-defined value
    #
    BGSL_TEMPLATE_KEY  : str = 'BgslTemplateFile'
    BTG_ACM_FDF_FV_NAME: str = 'CapsulePayloadBtGAcm'

    BTG_ACM_FLASH_MAP_DATA    : dict = \
        BTG_ACM_FIRMWARE_CONFIG[FLASH_MAP_OFFSET_SECTION]
    BTG_ACM_BGSL_TEMPLATE_PATH: str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          BTG_ACM_FIRMWARE_CONFIG[BGSL_TEMPLATE_KEY],
          )

    BTG_ACM_FV_KEY: str = 'Fv'

    if PLATFORM_GENERATION >= 14:
        BTG_ACM_AIT_HDR_VERSION = AIT_VERSION_9
    else:
        BTG_ACM_AIT_HDR_VERSION = AIT_VERSION_7

    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_BTG_ACM_IMG  : str = '-btgacm'
    KEYWORD_CMD_BTG_ACM_LSV  : str = '-btgacmlsv'
    KEYWORD_CMD_BIOS_SVN     : str = '-biossvn'
    KEYWORD_CMD_BTG_ACM_BUILD: str = '-build'
    POSITION_CMD_BIOS_RES    : str = 'res'
    FLAG_CMD_BTG_SINGLE_SLOT : str = '--single'

    class BtGAcm (FmpDevice):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (BtGAcm ver.1)

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
            self.__IsBgupBuild   : bool = True
            self.__BtGAcmPath    : str  = AbsPath (self.__Params[KEYWORD_CMD_BTG_ACM_IMG])
            self.__BtGAcmLsv     : str  = Params[KEYWORD_CMD_BTG_ACM_LSV]
            self.__BiosSvn       : str  = \
                GetBiosSvn (
                  self.__Params[KEYWORD_CMD_BIOS_SVN],
                  IsPostCheck = False,
                  )
            self.__BuildType     : str  = self.__Params[KEYWORD_CMD_BTG_ACM_BUILD]
            self.__ResFlag       : bool = bool (self.__Params[POSITION_CMD_BIOS_RES])
            self.__SingleSlotFlag: bool = bool (self.__Params[FLAG_CMD_BTG_SINGLE_SLOT])

            self.__PreCheck ()

            self.__Buffer   : ByteBuffer        = self.__GetImageBuffer ()
            self.__HdrParser: BtGAcmFwHdrParser = BtGAcmFwHdrParser (self.__Buffer)
            self.__VerInfo  : BtGAcmVersion     = self.__HdrParser.AcmVersion

            #
            # Get the BIOS image configuration.
            #
            self.__BiosBoarId  : str  = DEFAULT
            self.__BiosType    : str  = self.__GetBiosType ()
            self.__FlashMapInfo: dict = \
                GetFlashMapInfo (
                  BTG_ACM_FLASH_MAP_DATA,
                  BiosType    = self.__BiosType,
                  BuildTarget = self.__BuildType,
                  BoardId     = self.__BiosBoarId,
                  )
            self.__BlockSize   : int  = \
                GetSpiFlashBlockSize (BoardId = self.__BiosBoarId)

            #
            # Get the version information for BIOS.
            #
            self.__BtGAcmCapVer: BtGAcmCapsuleVersion = \
                BtGAcmCapsuleVersion (
                  VersionInfo = self.__VerInfo,
                  HeaderVer   = self.__HdrParser.AitHdrVersion,
                  )
            self.__BtGAcmCapLsv: BtGAcmCapsuleVersion = \
                BtGAcmCapsuleVersion (
                  DecDotVerStr = self.__BtGAcmLsv,
                  HeaderVer    = BTG_ACM_AIT_HDR_VERSION,
                  )

            self.__PostCheck ()

            self.__Payload: ByteBuffer = self.__GetCapsulePayload ()

        #######################################################################
        #                  Private Function for FMP Instance                  #
        #######################################################################
        def __PreCheck (self) -> None:
            """ Check the input argument is valid.

            Args:
                None.

            Raises:
                TypeError:
                    Build should be str type.
                ValueError:
                    (1) BtGAcm path is empty.
                    (2) BIOS SVN is not assigned.
                    (3) BtGAcm is assigned to be ignored.
                    (4) Build type is not assigned.

            Returns:
                None.
            """
            CheckPathList: List[str] = [
                self.__BtGAcmPath,
                ]

            if not isinstance (self.__BuildType, str):
                raise TypeError ('Build should be str type.')
            elif not (self.__BtGAcmPath):
                raise ValueError ('BtGAcm path can not be empty.')
            elif (self.__IsBgupBuild is True) and \
                 (self.__BiosSvn is NOT_APPLICABLE):
                raise ValueError (
                        f'Please input "-biossvn" since it is mandatory.'
                        )

            if IsFwImageIgnored (self.__BtGAcmPath):
                raise ValueError (
                        f'BtGAcm FW can not ignore '
                        f'when building BtGAcm capsule.'
                        )

            CheckFileListExist (CheckPathList)

            IsFound, Key = SearchValueInList (
                             BUILD_TARGET_TYPE_LIST,
                             self.__BuildType,
                             )

            if not IsFound:
                raise ValueError (
                        'Please assign the flag "-build" is mandatory.'
                        )

            self.__BuildType = Key

        def __PostCheck (self) -> None:
            """ Post check function after all variables initialized.

            Args:
                None.

            Raises:
                ErrorException:
                    Flash map is not assigned for target platform.

            Returns:
                None.
            """
            IsAssigned: bool = None

            IsAssigned, _ = IsFlashMapRegionInfoAssigned (
                              self.__FlashMapInfo,
                              BTG_ACM_FV_KEY,
                              )

            if not IsAssigned:
                ErrorException (
                  'BtGAcm capsule build is not supported on '
                  f'[{self.__BiosType}] - [{self.__BuildType}] BIOS.'
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
            SrcBuffer: Union[None, ByteBuffer] = None
            Buffer   : Union[None, ByteBuffer] = None

            DEBUG (
              DEBUG_TRACE,
              f'User assign the BtGAcm path: {self.__BtGAcmPath}'
              )
            SrcBuffer = ByteBuffer (self.__BtGAcmPath)
            SrcBuffer.Padding (
                        TargetSize = BTG_ACM_ALIGNMENT,
                        Char       = b'\xFF',
                        )
            SrcBuffer.Save (BTG_ACM_IMG_FILE_PATH)

            Buffer = ByteBuffer (BTG_ACM_IMG_FILE_PATH)

            return Buffer

        def __GetBiosType (self) -> str:
            """ Identify the type for the target platform BIOS image.

            Args:
                None.

            Raises:
                None.

            Returns:
                str:
                    Type of BIOS image should be defined in
                    CapsuleCommon/Intel/Bios/ConstantLib file.
            """
            if self.__ResFlag:
                return BIOS_RESILIENCY_TYPE
            else:
                return BIOS_NORMAL_TYPE

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
            FvOffset            : int                           = None
            FvSize              : int                           = None
            SlotInfoGenerator   : _BtGAcmSlotInfoGenerator      = None
            ImgInfoDict         : Dict[str, ImageRegion]        = None
            SlotFlashMapInfo    : dict                          = None
            BtGAcmBgupGenerator : _BtGAcmBgupGenerator          = None
            BgupInfoDict        : Dict[str, BgupRegion]         = None
            ConfigIniGenerator  : _BtGAcmCfgIniGenerator        = None
            FdfGenerator        : CapsulePayloadFdfGeneratorApi = None
            Edk2FvBuilder       : Edk2FdGeneratorApi            = None
            CapsulePayloadPath  : str                           = None
            CapsulePayloadBuffer: ByteBuffer                    = None

            #
            # 1. Get the BtGAcm slot information.
            #
            _, FvOffset, FvSize = GetFvRegionInfo (
                                    self.__FlashMapInfo,
                                    BTG_ACM_FV_KEY,
                                    )
            DEBUG (
              DEBUG_TRACE,
              f'Get the BtGAcm region information '
              f'Offset: [{FormatHex (FvOffset)}] Size: [{FormatHex (FvSize)}]'
              )

            SlotInfoGenerator = \
                _BtGAcmSlotInfoGenerator (
                    FvOffset     = FvOffset,
                    FvSize       = FvSize,
                    SlotSize     = BTG_ACM_ALIGNMENT,
                    BtGAcmImg    = self.__Buffer,
                    IsSingleSlot = self.__SingleSlotFlag,
                    )
            ImgInfoDict      = SlotInfoGenerator.ImgInfo
            SlotFlashMapInfo = SlotInfoGenerator.FlashMapInfo

            #
            # 2. Generate the BiosGuard Update Package (BGUP).
            #
            if self.__IsBgupBuild:
                BtGAcmBgupGenerator = \
                    _BtGAcmBgupGenerator (
                      FlashMapDict      = SlotFlashMapInfo,
                      BtGAcmImgInfoDict = ImgInfoDict,
                      BgslTemplatePath  = BTG_ACM_BGSL_TEMPLATE_PATH,
                      BiosSvn           = self.__BiosSvn,
                      )
                BgupInfoDict        = BtGAcmBgupGenerator.ImgInfo
                BtGAcmBgupGenerator.Save (BTG_ACM_BGUP_FILE_PATH)

            #
            # 3. Generate the flash config INI file.
            #
            ConfigIniGenerator = _BtGAcmCfgIniGenerator (
                                   FlashMapDict = SlotFlashMapInfo,
                                   ImgInfoDict  = ImgInfoDict,
                                   BgupInfoDict = BgupInfoDict,
                                   )
            ConfigIniGenerator.Save (BTG_ACM_CFG_FILE_PATH)

            #
            # 4. Generate the FDF file
            #
            FdfGenerator = CapsulePayloadFdfGeneratorApi (
                             BTG_ACM_FDF_FV_NAME,
                             self.__BlockSize,
                             )
            FdfGenerator.AddRawFile (uuid.UUID (BTG_ACM_CFG_FILE_GUID),  BTG_ACM_CFG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (BTG_ACM_IMG_FILE_GUID),  BTG_ACM_IMG_FILE_NAME,  FFS_ALIGNMENT_256K)
            FdfGenerator.AddRawFile (uuid.UUID (BTG_ACM_BGUP_FILE_GUID), BTG_ACM_BGUP_FILE_NAME, None)
            FdfGenerator.Save (EDKII_FDF_FILE_PATH)

            #
            # 5. Build the EDK-II FV for BtGAcm capsule payload
            #
            Edk2FvBuilder = Edk2FdGeneratorApi (BTG_ACM_FDF_FV_NAME)
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
            return FMP_SUCCESS, BTG_ACM_DEVICE_NAME

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
                self.__BtGAcmCapVer.GetDecDotVersion (),
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
            return FMP_SUCCESS, ToInt (self.__BtGAcmCapVer.GetDecVersion ())

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
            return FMP_SUCCESS, ToInt (self.__BtGAcmCapLsv.GetDecVersion ())

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
            return FMP_SUCCESS, self.__BtGAcmCapVer.GetDecDotVersion ()

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
                                      BTG_ACM_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
