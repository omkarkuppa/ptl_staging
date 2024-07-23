## @file
#  FMP device instance for ISH PDT firmware.
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
from CapsuleGenerate.Firmwares.IshPdtFirmware.IshPdtCapsuleVersion import *

#
# Pre-defined variable of the FMP instance.
#
ISH_PDT_DEVICE_NAME      : str  = 'IshPdt'
ISH_PDT_DEVICE_NAME_UPPER: str  = ISH_PDT_DEVICE_NAME.upper ()
ISH_PDT_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[ISH_PDT_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

ISH_PDT_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if ISH_PDT_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = ISH_PDT_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = ISH_PDT_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = ISH_PDT_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = ISH_PDT_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class IshPdt (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'ISH PDT FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == ISH_PDT_BUILD_OP_VER_1):
    WIN_DESC_FW_NAME: str = 'ISH PDT'
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_ISH_PDT_VER : str = '-pdtver'
    KEYWORD_CMD_ISH_PDT_LSV : str = '-pdtlsv'

    class IshPdt (object):
        def __init__ (self, Params: Dict[str, Any]) -> None:
            """ Class for the FMP device instance. (ISH PDT)

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
            self.__IshPdtPath: str = AbsPath (Params[KEYWORD_CMD_ISH_PDT_IMG])
            self.__IshPdtVer : str = self.__Params[KEYWORD_CMD_ISH_PDT_VER]
            self.__IshPdtLsv : str = self.__Params[KEYWORD_CMD_ISH_PDT_LSV]
            self.__PreCheck ()

            self.__Buffer      : ByteBuffer           = \
                self.__GetImageBuffer ()
            self.__IshPdtCapVer: IshPdtCapsuleVersion = \
                IshPdtCapsuleVersion (self.__IshPdtVer)
            self.__IshPdtCapLsv: IshPdtCapsuleVersion = \
                IshPdtCapsuleVersion (self.__IshPdtLsv)

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
                    (1) ISH PDT firmware not assigned.
                    (2) Caller input the ISH PDT firmware as ignore.

            Returns:
                None.
            """
            CheckPathList: List[str] = [
                self.__IshPdtPath,
                ]

            if not ((IsIfwiDecomposed (self.__Params)) or (self.__IshPdtPath)):
                raise ValueError (
                        'Please at least input '
                        '(1) -ifwi & -fit or '
                        '(2) -ishpdt '
                        'to assign the ISH PDT FW image.'
                        )

            if self.__IshPdtPath:
                DEBUG (DEBUG_TRACE, f'Assigned the ISH PDT FW image.')
                if IsFwImageIgnored (self.__IshPdtPath):
                    raise ValueError (
                            'ISH PDT FW can not ignore when building '
                            'ISH PDT capsule.'
                            )

                CheckFileListExist (CheckPathList)

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

            if self.__IshPdtPath:
                DEBUG (
                  DEBUG_TRACE,
                  f'User assign the ISH PDT path: {self.__IshPdtPath}'
                  )
                CopyFile (self.__IshPdtPath, ISH_PDT_IMG_FILE_PATH)

            Buffer = ByteBuffer (ISH_PDT_IMG_FILE_PATH)

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
            return FMP_SUCCESS, ISH_PDT_DEVICE_NAME

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
                self.__IshPdtCapVer.GetDecDotVersion (),
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
            return FMP_SUCCESS, ToInt (self.__IshPdtCapVer.GetDecVersion ())

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
            return FMP_SUCCESS, ToInt (self.__IshPdtCapLsv.GetDecVersion ())

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
            return FMP_SUCCESS, self.__IshPdtCapVer.GetDecDotVersion ()

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
                                      WIN_DESC_FW_NAME,
                                      VerStr,
                                      ])
