## @file
#  FMP device instance for FSP firmware.
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
from CapsuleGenerate.Firmwares.FspFirmware._FspHelperInternal import *
from CapsuleGenerate.Firmwares.FspFirmware.FspCapsuleVersion import *

#
# Pre-defined variable of the FMP instance.
#
FSP_DEVICE_NAME      : str  = 'Fsp'
FSP_DEVICE_NAME_UPPER: str  = FSP_DEVICE_NAME.upper ()
FSP_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[FSP_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

FSP_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if FSP_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = FSP_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = FSP_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = FSP_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = FSP_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class Fsp (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'FSP FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == FSP_BUILD_OP_VER_1):
    COMPRESSINFO_KEY           : str = 'CompressInfo'
    BGSL_TEMPLATE_ALIGN_KEY    : str = 'BgslTemplateFileAlign'
    BGSL_TEMPLATE_NON_ALIGN_KEY: str = 'BgslTemplateFileNonAlign'
    FSP_FDF_FV_NAME            : str = 'CapsulePayloadFsp'
    COMPRESS_INFO              : dict = FSP_FIRMWARE_CONFIG[COMPRESSINFO_KEY]
    FSP_FLASH_MAP_DATA         : dict = FSP_FIRMWARE_CONFIG[FLASH_MAP_OFFSET_SECTION]

    FSP_BGSL_TEMPLATE_ALIGN_PATH   : str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          FSP_FIRMWARE_CONFIG[BGSL_TEMPLATE_ALIGN_KEY],
          )
    FSP_BGSL_TEMPLATE_NON_ALIGN_PATH: str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          FSP_FIRMWARE_CONFIG[BGSL_TEMPLATE_NON_ALIGN_KEY],
          )
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_FSP_IMG  : str = '-fsp'
    KEYWORD_CMD_BIOS_SVN : str = '-biossvn'
    KEYWORD_CMD_FSP_BUILD: str = '-build'
    KEYWORD_CMD_FBM_IMG  : str = '-fbm'
    KEYWORD_CMD_FSP_LSV  : str = '-fsplsv'
    POSITION_CMD_BIOS_RES: str = 'res'

    class Fsp (FmpDevice):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (FSP ver.1)

            Args:
                Params (Dict[str, Any]):
                    The necessary parameter of this FMP device instance
                    to build capsule file.

            Raises:
                None.

            Returns:
                None.
            """
            self.__Params      : Dict[str, Any] = Params
            self.__IsBgupBuild : bool = True
            self.__FspPath     : str  = AbsPath (self.__Params[KEYWORD_CMD_FSP_IMG])
            self.__FbmPath     : str  = AbsPath (self.__Params[KEYWORD_CMD_FBM_IMG])
            self.__FspLsv      : str  = Params[KEYWORD_CMD_FSP_LSV]
            self.__BiosSvn     : str  = \
                GetBiosSvn (
                  self.__Params[KEYWORD_CMD_BIOS_SVN],
                  IsPostCheck = False,
                  )
            self.__BuildType   : str  = self.__Params[KEYWORD_CMD_FSP_BUILD]
            self.__ResFlag     : bool = bool (self.__Params[POSITION_CMD_BIOS_RES])
            self.__CompressInfo: dict = COMPRESS_INFO

            self.__PreCheck ()

            self.__CopyFspImage ()

            self.__FbmBuffer   : ByteBuffer   = self.__GetFbmImageBuffer ()
            self.__HdrParser   : FbmHdrParser = FbmHdrParser (self.__FbmBuffer)
            self.__VerInfo     : FbmVersion   = self.__HdrParser.FbmVersion

            #
            # Get the BIOS image configuration.
            #
            self.__BiosBoarId  : str  = DEFAULT
            self.__BiosType    : str  = self.__GetBiosType ()
            self.__FlashMapInfo: dict = \
                GetFlashMapInfo (
                  FSP_FLASH_MAP_DATA,
                  BiosType    = self.__BiosType,
                  BuildTarget = self.__BuildType,
                  BoardId     = self.__BiosBoarId,
                  )

            self.__BlockSize   : int  = \
                GetSpiFlashBlockSize (BoardId = self.__BiosBoarId)

            #
            # Get the version information for FSP.
            #
            self.__FspCapVer   : FspCapsuleVersion = \
                FspCapsuleVersion (
                    VersionInfo = self.__VerInfo,
                    )

            self.__FspCapLsv   : FspCapsuleVersion =\
                FspCapsuleVersion (
                  DotVerStr = self.__FspLsv
                  )

            self.__Payload     : ByteBuffer = self.__GetCapsulePayload ()

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
                    (1) Build type is not assigned.
                    (2) FSP path is empty.
                    (3) FBM path is empty.
                    (4) BIOS SVN is not assigned.
                    (6) FSP is assigned to be ignored.
                    (7) FBM is assigned to be ignored.

            Returns:
                None.
            """
            CheckPathList: List[str] = [
                self.__FspPath,
                self.__FbmPath
                ]

            if not isinstance (self.__BuildType, str):
                raise TypeError ('Build should be str type.')
            elif not (self.__FspPath):
                raise ValueError ('FSP path can not be empty.')
            elif not (self.__FbmPath):
                raise ValueError ('Fbm path can not be empty.')
            elif (self.__IsBgupBuild is True) and \
                 (self.__BiosSvn is NOT_APPLICABLE):
                raise ValueError (
                        f'Please input "-biossvn" since it is mandatory.'
                        )

            if IsFwImageIgnored (self.__FspPath):
                raise ValueError (
                        f'FSP FW can not ignore '
                        f'when building FSP capsule.'
                        )
            if IsFwImageIgnored (self.__FbmPath):
                raise ValueError (
                        f'Fbm FW can not ignore '
                        f'when building FSP capsule.'
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

        def __CopyFspImage (self) -> None:
            """ Copy FSP and FBM image to Temporary dir.

            Args:
                None.

            Raises:
                None.

            Returns:
                None.
            """
            CopyFile (
                self.__FspPath,
                FSP_IMG_FILE_PATH
                )

            CopyFile (
                self.__FbmPath,
                FBM_IMG_FILE_PATH
                )

        def __GetFbmImageBuffer (self) -> ByteBuffer:
            """ Read the FBM image from storage to buffer.

            Args:
                None.

            Raises:
                None.

            Returns:
                ByteBuffer:
                    FBM image buffer within the ByteBuffer wrapper.
            """
            Buffer: Union[None, ByteBuffer] = ByteBuffer (FBM_IMG_FILE_PATH)

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
            FspSplitter         : _FspImgSplitter               = None
            FspBgupGenerator    : _FspBgupGenerator             = None
            ConfigIniGenerator  : _FspCfgIniGenerator           = None
            FdfGenerator        : CapsulePayloadFdfGeneratorApi = None
            Edk2FvBuilder       : Edk2FdGeneratorApi            = None
            FspImgInfoDict      : dict                          = dict ()
            FspBgupInfoDict     : dict                          = dict ()
            CapsulePayloadPath  : str                           = None
            CapsulePayloadBuffer: ByteBuffer                    = None

            #
            # 1. Split the image buffer based on the FlashMap info.
            #
            FspSplitter    = \
                _FspImgSplitter (
                  FspFile      = FSP_IMG_FILE_PATH,
                  FbmFile      = FBM_IMG_FILE_PATH,
                  FlashMapDict = self.__FlashMapInfo,
                  BiosType     = self.__BiosType,
                  BlockSize    = self.__BlockSize,
                  CompressInfo = self.__CompressInfo
                  )
            FspImgInfoDict = FspSplitter.ImgInfo

            #
            # 2. Generate the BiosGuard Update Package (BGUP).
            #
            if self.__IsBgupBuild:
                FspBgupGenerator = \
                    _FspBgupGenerator (
                      FlashMapDict              = self.__FlashMapInfo,
                      FspImgInfoDict            = FspImgInfoDict,
                      BgslTemplateaAlignPath    = \
                        FSP_BGSL_TEMPLATE_ALIGN_PATH,
                      BgslTemplateaNonAlignPath = \
                        FSP_BGSL_TEMPLATE_NON_ALIGN_PATH,
                      BiosSvn                   = self.__BiosSvn,
                      )
                FspBgupGenerator.Save (FSP_BGUP_FILE_PATH)

                FspBgupInfoDict = FspBgupGenerator.ImgInfo

            #
            # 3. Generate the flash config INI file.
            #
            ConfigIniGenerator = \
                _FspCfgIniGenerator (
                  FlashMapDict    = self.__FlashMapInfo,
                  BiosType        = self.__BiosType,
                  FspImgInfoDict  = FspImgInfoDict,
                  FspBgupInfoDict = FspBgupInfoDict,
                  )
            ConfigIniGenerator.Save (FSP_CFG_FILE_PATH)

            #
            # 4. Generate the FDF file
            #
            FdfGenerator = CapsulePayloadFdfGeneratorApi (
                               FSP_FDF_FV_NAME,
                               self.__BlockSize
                               )
            FdfGenerator.AddRawFile (
                             uuid.UUID (FSP_CFG_FILE_GUID),
                             FSP_CFG_FILE_NAME,
                             None
                             )
            FdfGenerator.AddRawFile (
                             uuid.UUID (FSP_IMG_FILE_GUID),
                             FSP_IMG_FILE_NAME,
                             None
                             )
            FdfGenerator.AddRawFile (
                             uuid.UUID (FSP_BGUP_FILE_GUID),
                             FSP_BGUP_FILE_NAME,
                             None
                             )
            FdfGenerator.Save (EDKII_FDF_FILE_PATH)

            #
            # 5. Build the EDK-II FV for FSP capsule payload
            #
            Edk2FvBuilder = Edk2FdGeneratorApi (FSP_FDF_FV_NAME)
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
            return FMP_SUCCESS, FSP_DEVICE_NAME

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
                self.__FspCapVer.GetDecVersion (),
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
            return FMP_SUCCESS, ToInt (self.__FspCapVer.GetDecVersion ())

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
            return FMP_SUCCESS, ToInt (self.__FspCapLsv.GetDecVersion ())

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
            return FMP_UNSUPPORTED, 0

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
            return FMP_SUCCESS, self.__FspCapVer.GetDecDotVersion ()

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
                                      FSP_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
