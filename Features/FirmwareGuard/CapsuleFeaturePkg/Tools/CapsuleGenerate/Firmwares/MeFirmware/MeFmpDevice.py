## @file
#  FMP device instance for ME firmware.
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

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.MeFirmware.MeCapsuleVersion import *

#
# Pre-defined variable of the FMP instance.
#
ME_DEVICE_NAME      : str  = 'Me'
ME_DEVICE_NAME_UPPER: str  = ME_DEVICE_NAME.upper ()
ME_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[ME_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

ME_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if ME_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = ME_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = ME_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = ME_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = ME_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class Me (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'ME FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == ME_BUILD_OP_VER_1):
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_ME_VER : str = '-mever'
    KEYWORD_CMD_ME_LSV : str = '-melsv'
    KEYWORD_CMD_ME_SKU : str = '-mesku'
    KEYWORD_CMD_ME_SIGN: str = '-sign'

    class Me (object):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (ME)

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
            self.__MePath: str = AbsPath (Params[KEYWORD_CMD_ME_IMG])
            self.__MeVer : str = self.__Params[KEYWORD_CMD_ME_VER]
            self.__MeLsv : str = self.__Params[KEYWORD_CMD_ME_LSV]
            self.__MeSku : str = self.__Params[KEYWORD_CMD_ME_SKU]
            self.__MeSign: str = self.__Params[KEYWORD_CMD_ME_SIGN]
            self.__PreCheck ()

            self.__Buffer  : ByteBuffer       = self.__GetImageBuffer ()
            self.__MeCapVer: MeCapsuleVersion = MeCapsuleVersion (self.__MeVer)
            self.__MeCapLsv: MeCapsuleVersion = MeCapsuleVersion (self.__MeLsv)

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
                    (1) ME firmware not assigned.
                    (2) Caller input the ME firmware as ignore.
                    (3) Caller input the ME sku is not supported.
                    (4) Caller input sign type is not supported.

            Returns:
                None.
            """
            SKU_LIST : list = list (GUID_DATA.keys ())
            SIGN_LIST: list = ['', SIGN_PRE_PRODUCTION, SIGN_PRODUCTION]
            IsFound  : bool = False
            Value    : str  = str ()

            CheckPathList: List[str] = [
                self.__MePath,
                ]

            if not ((IsIfwiDecomposed (self.__Params)) or (self.__MePath)):
                raise ValueError (
                        'Please at least input '
                        '(1) -ifwi & -fit or '
                        '(2) -me '
                        'to assign the ME FW image.'
                        )

            if self.__MePath:
                DEBUG (DEBUG_TRACE, f'Assigned the ME FW image.')
                if IsFwImageIgnored (self.__MePath):
                    raise ValueError (
                            f'ME FW can not ignore when building ME capsule .'
                            )

                CheckFileListExist (CheckPathList)

            #
            # Check the ME SKU. (Case-insensitive)
            #
            IsFound, Value = SearchValueInList (SKU_LIST, self.__MeSku)
            if not IsFound:
                raise ValueError (
                        f'Input the SKU is "{self.__MeSku}", '
                        f'but not meet in the list {SKU_LIST}.'
                        )
            self.__MeSku = Value

            #
            # Check the ME Sign is valid. (Case-insensitive)
            #
            IsFound, Value = SearchValueInList (SIGN_LIST, self.__MeSign)
            if not IsFound:
                raise ValueError (
                        f'Input the sign type is "{self.__MeSign}", '
                        f'but not meet in the list {SIGN_LIST}.'
                        )
            self.__MeSign = Value

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

            if self.__MePath:
                DEBUG (
                  DEBUG_TRACE,
                  f'User assign the ME path: {self.__MePath}'
                  )
                CopyFile (self.__MePath, ME_IMG_FILE_PATH)

            Buffer = ByteBuffer (ME_IMG_FILE_PATH)

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
            return self.__Buffer

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
            SignType: SignCert = SignCert ()

            #
            # Based on sign type return different SignCert object.
            #
            if self.__MeSign == SIGN_PRE_PRODUCTION:
                SignType = EDSS_CERT_PRE_PROD_SIGN
            elif self.__MeSign == SIGN_PRODUCTION:
                SignType = EDSS_CERT_PROD_SIGN
            else:
                SignType = PLATFORM_CERT_SIGN

            DEBUG (
              DEBUG_INFO,
              f'Sign with {SignType.CertName} keystore. [{self.__MeSign}]',
              )

            return FMP_SUCCESS, SignType

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
            return FMP_SUCCESS, ME_DEVICE_NAME

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
                self.__MeCapVer.GetDecDotVersion (),
                self.__MeSku.upper (),
                self.__MeSign,
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
            return FMP_SUCCESS, ToInt (self.__MeCapVer.GetDecVersion ())

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
            return FMP_SUCCESS, ToInt (self.__MeCapLsv.GetDecVersion ())

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

            IsFound, Value = SearchKeyInDict (GUID_DATA, self.__MeSku)
            if not IsFound:
                raise ValueError (f'Not assigned the {self.__MeSku} key.')

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
            return FMP_SUCCESS, self.__MeCapVer.GetDecDotVersion ()

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
                                      ME_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
