## @file
#  FMP device instance for uCode firmware.
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
from CapsuleGenerate.ApiPrivate.Edk2FdGeneratorApiLib import *
from CapsuleGenerate.ApiPrivate.XdrGeneratorApiLib import *
from CapsuleGenerate.Const.ConfigFile import *
from CapsuleGenerate.Model.FmpDevice import *

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.UcodeFirmware._UcodeHelperInternal import *
from CapsuleGenerate.Firmwares.UcodeFirmware.UcodeCapsuleVersion import *

#
# Pre-defined variable of the FMP instance.
#
UCODE_DEVICE_NAME      : str  = 'Ucode'
UCODE_DEVICE_NAME_UPPER: str  = UCODE_DEVICE_NAME.upper ()
UCODE_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[UCODE_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

UCODE_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if UCODE_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = UCODE_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = UCODE_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = UCODE_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = UCODE_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# FMP instance customize define.
#
UCODE_BGUP_MODE: str = 'ucodebgup'
UCODE_FULL_MODE: str = 'ucodefull'
UCODE_SLOT_MODE: str = 'ucodeslot'

UCODE_CAP_FILE_LABEL: dict = {
    UCODE_BGUP_MODE: 'Bgup',
    UCODE_FULL_MODE: 'Full',
    UCODE_SLOT_MODE: 'Slot',
    }

#
# Default FMP instance should define as the Null one.
#
class Ucode (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'uCode FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == UCODE_BUILD_OP_VER_1):
    BGSL_TEMPLATE_KEY : str = 'BgslTemplateFile'
    SLOT_SIZE_DATA_KEY: str = 'SlotSize'
    UCODE_FV_KEY      : str = 'Fv'

    UCODE_FLASH_MAP_DATA    : dict = \
        UCODE_FIRMWARE_CONFIG[FLASH_MAP_OFFSET_SECTION]
    UCODE_SLOT_SIZE_DATA    : dict = \
        UCODE_FIRMWARE_CONFIG[SLOT_SIZE_DATA_KEY]
    UCODE_BGSL_TEMPLATE_PATH: str  = \
        JoinPath (
          TEMPLATE_DIR_PATH,
          UCODE_FIRMWARE_CONFIG[BGSL_TEMPLATE_KEY],
          )

    UCODE_FDF_FV_NAME: str = 'CapsulePayloadUcode'
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_UCODE_IMG    : str = '-ucode'
    KEYWORD_CMD_UCODE_MODE   : str = '-ucodemode'
    KEYWORD_CMD_UCODE_VER    : str = '-ucodever'
    KEYWORD_CMD_UCODE_LSV    : str = '-ucodelsv'
    KEYWORD_CMD_UCODE_VER_STR: str = '-ucodeverstr'
    KEYWORD_CMD_BIOS_SVN     : str = '-biossvn'
    KEYWORD_CMD_UCODE_BUILD  : str = '-build'
    KEYWORD_CMD_UCODE_SKU    : str = '-sku'
    POSITION_CMD_BIOS_RES    : str = 'res'

    class Ucode (FmpDevice):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (uCode ver.1)

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
            self.__UcodePathList: list = \
                self.__GetUcodePathList (self.__Params[KEYWORD_CMD_UCODE_IMG])

            self.__UcodeVer   : Union[str, int]  = \
                self.__Params[KEYWORD_CMD_UCODE_VER]
            self.__UcodeLsv   : Union[str, int]  = \
                self.__Params[KEYWORD_CMD_UCODE_LSV]
            self.__UcodeVerStr: str              = \
                str (self.__Params[KEYWORD_CMD_UCODE_VER_STR]).strip('"')
            self.__UcodeMode  : str              = \
                self.__GetUcodeMode (self.__Params[KEYWORD_CMD_UCODE_MODE])

            self.__BiosBoarId: str  = self.__Params[KEYWORD_CMD_UCODE_SKU]
            self.__BuildType : str  = self.__Params[KEYWORD_CMD_UCODE_BUILD]
            self.__BiosSvn   : str  = \
                GetBiosSvn (
                  self.__Params[KEYWORD_CMD_BIOS_SVN],
                  IsPostCheck = False,
                  )
            self.__ResFlag   : bool = \
                bool (self.__Params[POSITION_CMD_BIOS_RES])

            self.__PreCheck ()

            #
            # Get the target platform BIOS image configuration.
            #
            self.__BlockSize   : int  = \
                GetSpiFlashBlockSize (BoardId = self.__BiosBoarId)
            self.__BiosType    : str  = self.__GetBiosType ()
            self.__SlotSize    : int  = self.__GetSlotSize ()
            self.__FlashMapInfo: dict = \
                GetFlashMapInfo (
                  UCODE_FLASH_MAP_DATA,
                  BiosType    = self.__BiosType,
                  BuildTarget = self.__BuildType,
                  BoardId     = self.__BiosBoarId,
                  )

            #
            # Get the uCode capsule version and LSV.
            #
            self.__UcodeCapVer: UcodeCapsuleVersion = \
                UcodeCapsuleVersion (Version = self.__UcodeVer)
            self.__UcodeCapLsv: UcodeCapsuleVersion = \
                UcodeCapsuleVersion (Version = self.__UcodeLsv)

            self.__ImgBufferList: List[ByteBuffer] = None
            self.__ImgInfoList  : List[UcodeFwHdr] = None
            self.__ImgBufferList, self.__ImgInfoList = self.__Prepare ()

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
                    (1) UcodeVerStr not str type.
                    (2) Sku not str type.
                    (3) Build not str type.
                ValueError:
                    (1) BiosSvn is not input when mode is BGUP.
                    (2) Build is not input when mode is BGUP or FULL.

            Returns:
                None.
            """
            if not isinstance (self.__UcodeVerStr, str):
                raise TypeError ('UcodeVerStr should be str type.')
            elif not isinstance (self.__BiosBoarId, str):
                raise TypeError ('Sku should be str type.')
            elif not isinstance (self.__BuildType, str):
                raise TypeError ('Build should be str type.')

            #
            # BGUP mode BIOS SVN is mandatory.
            #
            if self.__UcodeMode == UCODE_BGUP_MODE:
                if self.__BiosSvn is NOT_APPLICABLE:
                    raise ValueError (
                            f'uCode BGUP mode capsule build "-biossvn" '
                            'is mandatory.'
                            )

            #
            # BGUP mode and FULL mode BIOS build type is mandatory.
            #
            if self.__UcodeMode in [UCODE_BGUP_MODE, UCODE_FULL_MODE]:
                IsFound, Key = SearchValueInList (
                                 BUILD_TARGET_TYPE_LIST,
                                 self.__BuildType,
                                 )

                if not IsFound:
                    raise ValueError (
                            'uCode BGUP or FULL mode capsule build '
                            '"-build" is mandatory.'
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

            IsAssigned, _ = IsRegionInfoAssigned (
                              DataDict = self.__FlashMapInfo,
                              Region   = UCODE_FV_KEY,
                              )

            if not IsAssigned:
                ErrorException (
                  'uCode capsule build is not supported on '
                  f'[{self.__BiosType}] - [{self.__BuildType}] BIOS.'
                  )

        def __GetUcodePathList (self, UcodePath: Union[str, list]) -> list:
            """ Convert the input uCode path into absolute path.

            Note:
                (1) When type is str, consider user input one uCode.
                    Place the path into list.
                (2) When type is list, consider user input more than one
                    uCode.
                (3) uCode path can not ignore.

            Args:
                UcodePath (Union[str, list]):
                    Path of input uCode firmware image.

            Raises:
                TypeError:
                    UcodePath not str nor list type.
                ValueError:
                    UcodePath is set as ignore.

            Returns:
                list:
                    List of absolute path of input uCode files.
            """
            CurrPath  : str  = None
            PathList  : list = ()
            ResultList: list = list ()

            if isinstance (UcodePath, str):
                PathList = [UcodePath]
            elif isinstance (UcodePath, list):
                PathList = UcodePath
            else:
                raise TypeError ('UcodePath should be str to list type.')

            for Path in PathList:
                if IsFwImageIgnored (Path):
                    raise ValueError ('UcodePath can not ignore.')

                CurrPath = AbsPath (Path)
                ResultList.append (CurrPath)

                CheckFileListExist ([CurrPath])

            return PathList

        def __GetUcodeMode (self, Mode: str) -> str:
            """ Return the type of uCode capsule file.

            Args:
                Mode (str):
                    Type of the uCode capsule file.
                    It should be one of the UCODE_CAP_FILE_LABEL keys.

            Raises:
                ValueError:
                    Unsupported target type of uCode capsule file.

            Returns:
                str:
                    Type of uCode capsule file.
            """
            IsFound: bool = None
            Key    : str  = None

            IsFound, Key = SearchKeyInDict (UCODE_CAP_FILE_LABEL, Mode)

            if not IsFound:
                raise ValueError (
                        'Unsupported uCode capsule update mode. '
                        f'Only support {list (UCODE_CAP_FILE_LABEL.keys ())}.'
                        )

            return Key

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

        def __GetSlotSize (self) -> int:
            """ Return the uCode slot size for specific platform.

            Args:
                None.

            Raises:
                ValueError:
                    Unsupported board ID and DEFAULT is not found.

            Returns:
                int:
                    uCode slot size for specific platform.
            """
            IsFound : bool = None
            Key     : str  = None
            SlotSize: int  = None

            IsFound, Key = SearchKeyInDict (
                             UCODE_SLOT_SIZE_DATA,
                             self.__BiosBoarId,
                             )

            if not IsFound:
                #
                # Not found set it as default.
                #
                DEBUG (
                  DEBUG_WARN,
                  f'Sku assigned [{Key}] is not found. '
                  f'Try to find the [{DEFAULT}] key.'
                  )
                IsFound, Key = SearchKeyInDict (UCODE_SLOT_SIZE_DATA, DEFAULT)

            if not IsFound:
                raise ValueError (f'Sku {DEFAULT} is not found.')

            SlotSize = ToInt (UCODE_SLOT_SIZE_DATA[Key])
            DEBUG (
              DEBUG_INFO,
              f'Find the [{self.__BiosBoarId}] slot size - [{hex (SlotSize)}].'
              )

            return SlotSize

        def __Prepare (self) -> Tuple[list, list]:
            """ Load the list of uCode path buffer and parse the info.

            Args:
                None.

            Raises:
                None.

            Returns:
                Tuple[list, list]:
                    list:
                        List of ByteBuffer mapping to one input uCode.
                    list:
                        List of UcodeFwHdr mapping to one input uCode.
            """
            Buffer      : ByteBuffer       = None
            Info        : UcodeFwHdr       = None
            BufferList  : List[ByteBuffer] = list ()
            InfoList    : List[UcodeFwHdr] = list ()

            for Path in self.__UcodePathList:
                Buffer = ByteBuffer (FilePath = Path)
                Info   = UcodeFwHdrParser (Buffer).Header

                DEBUG (DEBUG_TRACE, f'Now process the uCode: {Path}')

                if Buffer.Size > self.__SlotSize:
                    raise ValueError (
                            f'uCode size [{hex (Buffer.Size)}] is larger than '
                            f'slot size [{hex (self.__SlotSize)}].'
                            )

                Buffer.Padding (
                         TargetSize = self.__SlotSize,
                         Char       = b'\xFF',
                         )

                BufferList.append (Buffer)
                InfoList.append (Info)

            return BufferList, InfoList

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
            UcodeVerHdrGenerator: _UcodeVerHdrGeneratorApi = None
            FvOffset            : int                      = None
            FvSize              : int                      = None
            MaxNumOfSlot        : int                      = None
            FdfGenerator        : _UcodeFdfGeneratorApi    = None
            CapsulePayloadBuffer: ByteBuffer               = None
            UcodeFvPath         : str                      = None
            UcodeFvBuffer       : ByteBuffer               = None
            BgupGenerator       : _UcodeBgupGeneratorApi   = None
            XdrInputList        : list                     = None
            XdrGenerator        : XdrGeneratorApi          = None

            #
            # 1. Generate the uCode version header file.
            #
            UcodeVerHdrGenerator = \
                _UcodeVerHdrGeneratorApi (
                  ToInt (self.__UcodeCapVer.GetDecVersion ()),
                  ToInt (self.__UcodeCapLsv.GetDecVersion ()),
                  self.__UcodeVerStr,
                  )
            UcodeVerHdrGenerator.Save (UCODE_VER_FILE_PATH)

            #
            # 2-1. Generate the FDF file. (For BGUP or FULL mode)
            #
            if self.__UcodeMode in [UCODE_BGUP_MODE, UCODE_FULL_MODE]:
                if CheckStringMatch (
                     self.__FlashMapInfo,
                     NOT_APPLICABLE,
                     IsTypeCheck = False,
                     ):
                    raise ValueError (
                            'FV information is not provided, not supported for '
                            'BGUP or FULL mode update.'
                            )

                _, FvOffset, FvSize = \
                    GetFvRegionInfo (self.__FlashMapInfo, UCODE_FV_KEY)
                MaxNumOfSlot = int (FvSize / self.__SlotSize)

                #
                # Create the FDF file.
                #
                FdfGenerator = _UcodeFdfGeneratorApi (
                                 FvName         = UCODE_FDF_FV_NAME,
                                 FvSize         = FvSize,
                                 FlashBlockSize = self.__BlockSize,
                                 )

                for Idx in range (len (self.__ImgBufferList)):
                    FdfGenerator.AddUcodeFile (
                                   Buffer = self.__ImgBufferList[Idx],
                                   Info   = self.__ImgInfoList[Idx],
                                   )

                    if Idx >= MaxNumOfSlot:
                        raise ValueError (
                                f'uCode FV only support [{MaxNumOfSlot}] '
                                'uCode.'
                                )

                FdfGenerator.Save (EDKII_FDF_FILE_PATH)

                #
                # Build the EDK-II FV for BIOS capsule payload
                #
                Edk2FvBuilder = Edk2FdGeneratorApi (UCODE_FDF_FV_NAME)
                Edk2FvBuilder.Execute ()

                UcodeFvPath   = Edk2FvBuilder.FvPath
                UcodeFvBuffer = ByteBuffer (FilePath = UcodeFvPath)
                UcodeFvBuffer.Save (UCODE_IMG_FILE_PATH)

            #
            # 2-2. Generate BGUP file. (For BGUP mode)
            #
            if self.__UcodeMode in [UCODE_BGUP_MODE]:
                BgupGenerator = \
                    _UcodeBgupGeneratorApi (
                      FvOffset         = FvOffset,
                      FvSize           = FvSize,
                      FvDataPath       = UcodeFvPath,
                      BgslTemplatePath = UCODE_BGSL_TEMPLATE_PATH,
                      BiosSvn          = self.__BiosSvn,
                      )
                BgupGenerator.Save (UCODE_BGUP_FILE_PATH)

            #
            # 2-3. Generate the uCode Array Raw Data (For Slot Mode)
            #
            if self.__UcodeMode == UCODE_SLOT_MODE:
                if len (self.__ImgBufferList) != 1:
                    raise ValueError (
                            'Slot Mode Only support one uCode input but get '
                            f'[{len (self.__ImgBufferList)}].'
                            )
                CopyFile (
                  self.__ImgBufferList[0].Path,
                  UCODE_IMG_FILE_PATH,
                  )

            #
            # 3. Package into XDR format.
            #
            # Note:
            #   (1) RFC 4506 External Data Representation Standard (XDR)
            #       each element is a variable-length opaque data.
            #   (2) uCode capsule generator need to mark length n = 0 in
            #       variable-length opaque data element.
            #   (3) XDR format: 4 bytes as file sizes + Data
            #
            #  +-------------------+
            #  |      uCodeFv      | -> BGUP and FULL mode only.
            #  +-------------------+
            #  |    uCodeFvBgup    | -> BGUP mode only.
            #  +-------------------+
            #  |   uCode Version   | -> Slot mode only.
            #  +-------------------+
            #  |    uCode Array    | -> Slot mode only.
            #  +-------------------+
            #
            XdrInputList = [
                UCODE_IMG_FILE_PATH  if self.__UcodeMode in [UCODE_FULL_MODE, UCODE_BGUP_MODE] else None,
                UCODE_BGUP_FILE_PATH if self.__UcodeMode in [UCODE_BGUP_MODE] else None,
                UCODE_VER_FILE_PATH  if self.__UcodeMode in [UCODE_SLOT_MODE] else None,
                UCODE_IMG_FILE_PATH  if self.__UcodeMode in [UCODE_SLOT_MODE] else None,
                ]
            DEBUG (
              DEBUG_TRACE,
              f'uCode build mode: [{self.__UcodeMode}], '
              f'XDR consist with {XdrInputList}.'
              )

            XdrGenerator = XdrGeneratorApi (
                             FilePathList = XdrInputList,
                             OutputPath   = UCODE_XDR_FILE_PATH,
                             )
            XdrGenerator.Execute ()

            CapsulePayloadBuffer = ByteBuffer (FilePath = UCODE_XDR_FILE_PATH)

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
            return FMP_SUCCESS, UCODE_DEVICE_NAME

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
                self.__BiosType  if self.__UcodeMode != UCODE_SLOT_MODE else '',
                self.__BuildType if self.__UcodeMode != UCODE_SLOT_MODE else '',
                DecToHex (self.__SlotSize),
                UCODE_CAP_FILE_LABEL[self.__UcodeMode],
                FormatHex (self.__UcodeCapVer.GetHexVersion (), IsPrefix = True),
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
            return FMP_SUCCESS, ToInt (self.__UcodeCapVer.GetDecVersion ())

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
            return FMP_SUCCESS, ToInt (self.__UcodeCapLsv.GetDecVersion ())

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
            return FMP_SUCCESS, self.__UcodeCapVer.GetDecDotVersion ()

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

            Status, Vendor = self.GetVendor ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get the vendor name.')

            return FMP_SUCCESS, ' '.join ([
                                      Vendor,
                                      PLATFORM_FULL_NAME,
                                      UCODE_DEVICE_NAME_UPPER,
                                      self.__UcodeVerStr,
                                      ])
