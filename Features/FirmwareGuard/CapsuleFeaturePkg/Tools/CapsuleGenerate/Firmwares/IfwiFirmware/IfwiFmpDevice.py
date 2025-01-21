## @file
#  FMP device instance for IFWI firmware.
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

import uuid

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.ApiPrivate.Edk2FdfGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.Edk2FdGeneratorApiLib import *
from CapsuleGenerate.Const.ConfigFile import *
from CapsuleGenerate.Model.FmpDevice import *

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.BiosFirmware.BiosCapsuleVersion import *

from CapsuleGenerate.Firmwares.IfwiFirmware._IfwiHelperInternal import *

#
# Pre-defined variable of the FMP instance.
#
IFWI_DEVICE_NAME      : str  = 'Ifwi'
IFWI_DEVICE_NAME_UPPER: str  = IFWI_DEVICE_NAME.upper ()
IFWI_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[IFWI_DEVICE_NAME]

IFWI_SIZE_32MB: int       = SIZE_32MB
IFWI_SIZE_64MB: int       = SIZE_64MB
IFWI_SIZE_LIST: List[int] = [
    IFWI_SIZE_32MB,
    IFWI_SIZE_64MB,
    ]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

IFWI_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if IFWI_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = IFWI_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = IFWI_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = IFWI_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = IFWI_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class Ifwi (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'IFWI FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == IFWI_BUILD_OP_VER_1):
    BGSL_TEMPLATE_KEY   : str  = 'BgslTemplateFile'

    IFWI_BGSL_TEMPLATE_PATH: str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          IFWI_FIRMWARE_CONFIG[BGSL_TEMPLATE_KEY],
          )

    IFWI_FDF_FV_NAME: str = 'CapsulePayloadIfwi'
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_BIOS_SVN : str = '-biossvn'

    class Ifwi (FmpDevice):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (IFWI ver.1)

            Args:
                Params (Dict[str, Any]):
                    The necessary parameter of this FMP device instance
                    to build capsule file.

            Raises:
                None.

            Returns:
                None.
            """
            self.__Params  : Dict[str, Any] = Params
            #
            # Initial the parameter
            #
            self.__IfwiPath: str = AbsPath (
                                     self.__Params[KEYWORD_CMD_IFWI_IMG],
                                     )
            self.__BiosSvn : str = \
                GetBiosSvn (
                  self.__Params[KEYWORD_CMD_BIOS_SVN],
                  IsPostCheck = False,
                  )
            self.__PreCheck ()

            self.__Buffer: ByteBuffer = self.__GetImageBuffer ()

            #
            # Check if capsule target platform BiosGuard is enabled.
            #
            self.__IsBgupBuild: bool = True

            #
            # Get the version information for BIOS.
            # We use the BIOS version as IFWI version since it would be
            # part of IFWI always.
            #
            self.__HdrParser : BiosFwHdrParser    = \
                BiosFwHdrParser (self.__Buffer)
            self.__BufferHdr : BiosIdHdr          = self.__HdrParser.Header

            #
            # Get the BIOS image configuration.
            #
            self.__BiosBoarId: str = self.__BufferHdr.BoardId[0:4]
            self.__BlockSize : int = \
                GetSpiFlashBlockSize (BoardId = self.__BiosBoarId)

            self.__BiosCapVer: BiosCapsuleVersion = \
                BiosCapsuleVersion (
                  Header          = self.__BufferHdr,
                  IsRestrictCheck = False,
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
                ValueError:
                    (1) IFWI firmware not assigned.
                    (2) Caller input the IFWI firmware as ignore.

            Returns:
                None.
            """
            CheckPathList: List[str] = [
                self.__IfwiPath,
                ]

            if self.__IfwiPath:
                DEBUG (DEBUG_TRACE, f'Assigned the IFWI FW image.')
                if IsFwImageIgnored (self.__IfwiPath):
                    raise ValueError (
                            f'IFWI can not ignore when building IFWI capsule .'
                            )

                CheckFileListExist (CheckPathList)

        def __PostCheck (self):
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

            if self.__IfwiPath:
                DEBUG (
                  DEBUG_TRACE,
                  f"User assign the IFWI path: {self.__IfwiPath}"
                  )
                CopyFile (self.__IfwiPath, IFWI_IMG_FILE_PATH)

            Buffer = ByteBuffer (IFWI_IMG_FILE_PATH)

            if Buffer.Size not in IFWI_SIZE_LIST:
                raise ValueError (
                        f'IFWI size should be one of {IFWI_SIZE_LIST} bytes, '
                        f'but get the size [{Buffer.Size}]'
                        )

            return Buffer

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
            IfwiImgTrunkGenerator: _IfwiImgTrunkGenerator        = None
            IfwiImgInfoDict      : dict                          = None
            IfwiBgupGenerator    : _IfwiBgupGenerator            = None
            IfwiBgupInfoDict     : dict                          = None
            ConfigIniGenerator   : _IfwiCfgIniGenerator          = None
            FdfGenerator         : CapsulePayloadFdfGeneratorApi = None
            Edk2FvBuilder        : Edk2FdGeneratorApi            = None
            CapsulePayloadPath   : str                           = None
            CapsulePayloadBuffer : ByteBuffer                    = None

            #
            # 1. Split the image buffer by trunk size.
            #
            IfwiImgTrunkGenerator = _IfwiImgTrunkGenerator (self.__Buffer)
            IfwiImgInfoDict       = IfwiImgTrunkGenerator.ImgInfo
            FlashMapDict          = IfwiImgTrunkGenerator.FlashMapInfo

            #
            # 2. Generate the BiosGuard Update Package (BGUP).
            #
            if self.__IsBgupBuild:
                IfwiBgupGenerator = \
                    _IfwiBgupGenerator (
                      IfwiImgInfoDict  = IfwiImgInfoDict,
                      BgslTemplatePath = IFWI_BGSL_TEMPLATE_PATH,
                      BiosSvn          = self.__BiosSvn,
                      )
                IfwiBgupGenerator.Save (IFWI_BGUP_FILE_PATH)

                IfwiBgupInfoDict = IfwiBgupGenerator.ImgInfo

            #
            # 3. Generate the flash config INI file.
            #
            ConfigIniGenerator = \
                _IfwiCfgIniGenerator (
                  FlashMapDict     = FlashMapDict,
                  IfwiImgInfoDict  = IfwiImgInfoDict,
                  IfwiBgupInfoDict = IfwiBgupInfoDict,
                  )
            ConfigIniGenerator.Save (IFWI_CFG_FILE_PATH)

            #
            # 4. Generate the FDF file
            #
            FdfGenerator = CapsulePayloadFdfGeneratorApi (IFWI_FDF_FV_NAME, self.__BlockSize)
            FdfGenerator.AddRawFile (uuid.UUID (IFWI_CFG_FILE_GUID),  IFWI_CFG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (IFWI_IMG_FILE_GUID),  IFWI_IMG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (IFWI_BGUP_FILE_GUID), IFWI_BGUP_FILE_NAME, None)
            FdfGenerator.Save (EDKII_FDF_FILE_PATH)

            #
            # 5. Build the EDK-II FV for IFWI capsule payload
            #
            Edk2FvBuilder = Edk2FdGeneratorApi (IFWI_FDF_FV_NAME)
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
            return FMP_SUCCESS, IFWI_DEVICE_NAME

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
                f'BIOS{self.__BiosCapVer.GetDecVersion ()}',
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
            return FMP_SUCCESS, FMP_LSV_DEFAULT_VALUE

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
                                      IFWI_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
