## @file
#  FMP device instance for Monolithic firmware.
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
import uuid

from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.Const.ConfigFile import *
from CapsuleGenerate.Model.FmpDevice import *

from CapsuleGenerate.Firmwares.CommonLib import *
from CapsuleGenerate.Firmwares.BiosFirmware.BiosFmpDevice import *

#
# Pre-defined variable of the FMP instance.
#
MONOLITHIC_DEVICE_NAME      : str  = 'Monolithic'
MONOLITHIC_DEVICE_NAME_UPPER: str  = MONOLITHIC_DEVICE_NAME.upper ()
MONOLITHIC_FIRMWARE_CONFIG  : dict = gFmpFirmwareInfo[MONOLITHIC_DEVICE_NAME]

#
# Build operation version control.
#
BUILD_OP_VER: Union[None, int] = None

MONOLITHIC_BUILD_OP_VER_1: int = 1

#
# Generic-defined.
#
if MONOLITHIC_FIRMWARE_CONFIG is not None:
    CAP_FILE_PREFIX : str  = MONOLITHIC_FIRMWARE_CONFIG[CAP_FILE_PREFIX_KEY]
    CAP_FILE_POSTFIX: str  = MONOLITHIC_FIRMWARE_CONFIG[CAP_FILE_POSTFIX_KEY]
    GUID_DATA       : dict = MONOLITHIC_FIRMWARE_CONFIG[FMP_GUID_SECTION_KEY]
    BUILD_OP_VER    : int  = MONOLITHIC_FIRMWARE_CONFIG[FW_VERSION_KEY]

#
# Default FMP instance should define as the Null one.
#
class Monolithic (FmpDeviceNull):
    def __init__ (self, *Args, **KwArgs):
        DEBUG (DEBUG_WARN, f'Monolithic FMP instance is not implemented.')

#
# Override with different build operation.
#
if (BUILD_OP_VER == MONOLITHIC_BUILD_OP_VER_1):
    MONOLITHIC_FDF_FV_NAME: str = 'CapsulePayloadMonolithic'
    #
    # FMP Device specific build command
    #
    KEYWORD_CMD_ME_IMG     : str = '-me'
    KEYWORD_CMD_EC_IMG     : str = '-ec'
    KEYWORD_CMD_ISH_PDT_IMG: str = '-pdt'
    KEYWORD_CMD_GBE_IMG    : str = '-gbe'

    class Monolithic (object):
        def __init__ (self, Params: Dict[str, Any], BiosFmpInst: Bios) -> None:
            """ Class for the FMP device instance. (Monolithic ver.1)

            Args:
                Params (Dict[str, Any]):
                    The necessary parameter of this FMP device instance
                    to build capsule file.
                BiosFmpInst (Bios):
                    BIOS FMP instance.
                    (Monolithic had dependency on BIOS FMP)

            Raises:
                None.

            Returns:
                None.
            """
            self.__Params     : Dict[str, Any] = Params
            self.__BiosFmpInst: Bios           = BiosFmpInst
            #
            # Initial the parameter
            # Note: Path cannot convert to AbsPath () at this moment,
            #       it might be IGNORE keyword.
            #
            self.__BiosPath  : str = self.__Params[KEYWORD_CMD_BIOS_IMG]
            self.__MePath    : str = self.__Params[KEYWORD_CMD_ME_IMG]
            self.__EcPath    : str = self.__Params[KEYWORD_CMD_EC_IMG]
            self.__IshPdtPath: str = self.__Params[KEYWORD_CMD_ISH_PDT_IMG]
            self.__GbePath   : str = self.__Params[KEYWORD_CMD_GBE_IMG]

            self.__FwImageInfo: dict = {
                FW_BIOS   : self.__BiosPath,
                FW_CSME   : self.__MePath,
                FW_EC     : self.__EcPath,
                FW_ISH_PDT: self.__IshPdtPath,
                FW_GBE    : self.__GbePath,
                }

            self.__PreCheck ()
            self.__Prepare ()

            self.__Payload: ByteBuffer = self.__GetCapsulePayload ()

        #######################################################################
        #                  Private Function for FMP Instance                  #
        #######################################################################
        def __PreCheck (self) -> None:
            """ Check the input argument is valid.

            Note:
                Check the user input FW component is valid.
                    - If it input is empty, set the path as None.
                    - If it input is ignore, set the path as IGNORE.
                    - If it input is valid, convert it absolute path.

            Args:
                None.

            Raises:
                TypeError:
                    Path of the firmware component not str type.

            Returns:
                None.
            """
            #
            # Note:
            #   (1) '-bios', '-bioslsv', '-biossvn', '-ifwi', '-fit' check
            #       had been covered by BiosFmpDevice instance.
            #   (2) '-bios' cannot ignore checked as well.
            #
            FwImgInfo: dict = copy.deepcopy (self.__FwImageInfo)

            for FwName, FwPath in FwImgInfo.items ():
                if not isinstance (FwPath, str):
                    raise TypeError (f'FwPath should be str type.')
                elif not FwPath:
                    DEBUG (
                      DEBUG_INFO,
                      f'Caller not assign the [{FwName}] path.'
                      )
                    self.__FwImageInfo[FwName] = None
                elif IsFwImageIgnored (FwPath):
                    DEBUG (
                      DEBUG_INFO,
                      f'Caller set [{FwName}] as {IGNORE} skip to check.'
                      )
                    self.__FwImageInfo[FwName] = IGNORE
                else:
                    DEBUG (DEBUG_INFO, f'Assigned the [{FwName}] {FwPath}')
                    CheckFileListExist ([FwPath])
                    self.__FwImageInfo[FwName] = AbsPath (FwPath)

        def __Prepare (self) -> None:
            """ Prepare the firmware component image into workspace.

            Note:
                (1) If FwPath is None, do nothing to keep as-it.
                (2) If FwPath is IGNORE, replace it as dummy file.
                (3) If FwPath is assigned, copy it.

            Args:
                None.

            Raises:
                ValueError:
                    Unsupported firmware component not list on
                    FW_COMPONENT_DICT.

            Returns:
                None.
            """
            IsFound   : bool = None
            FwKey     : str  = None
            TargetPath: str  = None

            for FwName, FwPath in self.__FwImageInfo.items ():
                IsFound, FwKey = SearchKeyInDict (FW_COMPONENT_DICT, FwName)
                if not IsFound:
                    raise ValueError (f'Unsupported FW component [{FwName}].')

                TargetPath = FW_COMPONENT_DICT[FwKey][FW_FILE_PATH_KEY]
                DEBUG (
                  DEBUG_INFO,
                  f'Prepare [{FwName}] component to [{TargetPath}]'
                  )

                if FwPath is None:
                    DEBUG (DEBUG_INFO, 'No need to take action, it is empty.')
                elif FwPath is IGNORE:
                    DEBUG (DEBUG_INFO, 'Replace it as dummy file.')
                    CreateDummyFile (TargetPath)
                else:
                    DEBUG (DEBUG_INFO, f'Assign it as [{FwPath}], copy it.')
                    CopyFile (FwPath, TargetPath)

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
            FdfGenerator        : CapsulePayloadFdfGeneratorApi = None
            Edk2FvBuilder       : Edk2FdGeneratorApi            = None
            CapsulePayloadPath  : str                           = None
            CapsulePayloadBuffer: ByteBuffer                    = None

            #
            # 1. Generate the FDF file
            #
            FdfGenerator = CapsulePayloadFdfGeneratorApi (
                             MONOLITHIC_FDF_FV_NAME,
                             self.__BiosFmpInst.BlockSize
                             )
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_CFG_FILE_GUID),  BIOS_CFG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_IMG_FILE_GUID),  BIOS_IMG_FILE_NAME,  None)
            FdfGenerator.AddRawFile (uuid.UUID (BIOS_BGUP_FILE_GUID), BIOS_BGUP_FILE_NAME, None)

            for FwName, _ in self.__FwImageInfo.items ():
                if CheckStringMatch (FwName, FW_BIOS):
                    continue

                IsFound, FwKey = SearchKeyInDict (FW_COMPONENT_DICT, FwName)
                if not IsFound:
                    raise ValueError (f'Unsupported FW component [{FwName}].')

                FwGuid = uuid.UUID (FW_COMPONENT_DICT[FwKey][FW_FILE_GUID_KEY])
                FwName = FW_COMPONENT_DICT[FwKey][FW_FILE_NAME_KEY]
                FdfGenerator.AddRawFile (FwGuid, FwName, None)

            FdfGenerator.Save (EDKII_FDF_FILE_PATH)

            #
            # 2. Build the EDK-II FV for Monolithic capsule payload
            #
            Edk2FvBuilder = Edk2FdGeneratorApi (MONOLITHIC_FDF_FV_NAME)
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
            return FMP_SUCCESS, MONOLITHIC_DEVICE_NAME

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
                self.__BiosFmpInst.BiosType,
                self.__BiosFmpInst.BuildType,
                self.__BiosFmpInst.BiosCapVer.GetDecVersion (),
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
            Status : int = None
            Version: int = None

            Status, Version = self.__BiosFmpInst.GetFwImageVersion ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get Monolithic version.')

            return FMP_SUCCESS, Version

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
            Status : int = None
            Version: int = None

            Status, Version = self.__BiosFmpInst.GetFwImageLsv ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get Monolithic LSV.')

            return FMP_SUCCESS, Version

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
            Status: int = None
            VerStr: str = None

            Status, VerStr = self.__BiosFmpInst.GetCapsuleString ()
            if Status != FMP_SUCCESS:
                ErrorException (f'Failed to get Monolithic Capsule String.')

            return FMP_SUCCESS, VerStr

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
                                      MONOLITHIC_DEVICE_NAME_UPPER,
                                      VerStr,
                                      ])
