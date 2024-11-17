## @file
#  API file support to create the capsule file.
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
from typing import Any, List
from CapsuleCommon import *

from CapsuleGenerate._Global import *
from CapsuleGenerate.Model.FmpDevice import *

CAP_PAYLOAD_FILE_NAME: str = 'CapsulePayload.bin'
CAP_PAYLOAD_FILE_PATH: str = \
    JoinPath (
      TEMPORARY_DIR_PATH,
      CAP_PAYLOAD_FILE_NAME,
      )

#
# Windows Driver Update package support.
#
PY_WIN_INF_GENERATOR_SCRIPT_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gPYScriptPathInfo[WIN_INF_GEN_SCRIPT_KEY],
      )

PY_WIN_CAT_GENERATOR_SCRIPT_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gPYScriptPathInfo[WIN_CAT_GEN_SCRIPT_KEY],
      )

PY_WIN_CAT_SIGNER_SCRIPT_PATH: str = \
    JoinPath (
      WORKSPACE_PATH,
      gPYScriptPathInfo[WIN_CAT_SIGN_SCRIPT_KEY],
      )

#
# Definition of the FMP Device instance information.
#
KEY_CAP_PAYLOAD    : str = 'Payload'
KEY_CAP_SIGN_CERT  : str = 'SignCert'
KEY_CAP_DEV_NAME   : str = 'DeviceName'
KEY_CAP_CAP_GUID   : str = 'Guid'
KEY_CAP_VER        : str = 'Version'
KEY_CAP_LSV        : str = 'LowestSupportVersion'
KEY_CAP_NAME       : str = 'FileName'
KEY_CAP_VENDOR     : str = 'Vendor'
KEY_CAP_VER_STR    : str = 'VersionString'
KEP_CAP_DESCRIPTION: str = 'Description'
KEY_CAP_HW_INSTANCE: str = 'HardwareInstance'

class ApiFmpCapBuild (object):
    def __init__ (self, FmpInst: FmpDevice):
        """ Class support to build the FMP capsule file.

        Args:
            FmpInst (FmpDevice):
                The FMP Device Instance object.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FmpInst: FmpDevice = FmpInst
        #
        # PreCheck and initial the FmpInfo.
        #
        self.__FmpInfo: dict      = self.__PreCheck ()

        self.__CapFileName: str = \
            CombineFileName (
              self.__FmpInfo[KEY_CAP_NAME],
              Extension = EXTENSION_CAP_FILE,
              )
        self.__CapFilePath: str = \
            JoinPath (
              OUTPUT_DIR_PATH,
              self.__CapFileName,
              )

    def BuildCapsule (self) -> None:
        """ Expose the function to build the capsule file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        #
        # Flow to generate and sign the capsule file.
        #
        self.__CapPayloadBufferToFile ()
        self.__GenerateCapFile ()

    def BuildWinUpdatePkg (self) -> None:
        """ Expose the function to build Windows driver update package.

        Note:
            It is caller responsibility to ensure the build environment
            is ready. (SDK/WDK install required)

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        OutputDirPath: str = str ()
        InfFilePath  : str = str ()
        CatFilePath  : str = str ()

        OutputDirPath, InfFilePath, CatFilePath = \
            self.__WinUpdatePreWork ()

        self.__WinUpdateInfGenerate (InfFilePath)
        self.__WinUpdateCatGenerate (OutputDirPath)
        self.__WinUpdateCatSign (CatFilePath)

    def __PreCheck (self) -> dict:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                Specific FMP Device instance information is not valid.

        Returns:
            dict:
                The mapping data for this FMP instance information.
                {KEY_CAP_PAYLOAD: Data, ...}
        """
        Status: int  = FMP_SUCCESS
        Datum : Any  = None
        Result: dict = dict ()

        CheckTableList: list = [
            [KEY_CAP_PAYLOAD,     COMPULSORY, self.__FmpInst.GetFwImagePayload,     ByteBuffer,],
            [KEY_CAP_SIGN_CERT,   COMPULSORY, self.__FmpInst.GetSignCert,           SignCert,  ],
            [KEY_CAP_DEV_NAME,    COMPULSORY, self.__FmpInst.GetDeviceName,         str,       ],
            [KEY_CAP_CAP_GUID,    COMPULSORY, self.__FmpInst.GetFwImageGuid,        uuid.UUID, ],
            [KEY_CAP_VER,         COMPULSORY, self.__FmpInst.GetFwImageVersion,     IsUint32,  ],
            [KEY_CAP_LSV,         COMPULSORY, self.__FmpInst.GetFwImageLsv,         IsUint32,  ],
            [KEY_CAP_NAME,        COMPULSORY, self.__FmpInst.GetCapsuleFileName,    str,       ],
            [KEY_CAP_VENDOR,      COMPULSORY, self.__FmpInst.GetVendor,             str,       ],
            [KEY_CAP_VER_STR,     COMPULSORY, self.__FmpInst.GetCapsuleString,      str,       ],
            [KEP_CAP_DESCRIPTION, COMPULSORY, self.__FmpInst.GetCapsuleDescription, str,       ],
            [KEY_CAP_HW_INSTANCE, OPTIONAL,   self.__FmpInst.GetHardwareInstance,   IsUint64,  ],
            ]

        for Key, Type, CallFunc, Checker in CheckTableList:
            if Type != COMPULSORY:
                DEBUG (DEBUG_TRACE, f'{Key} is optional no need to check.')
                continue

            Status, Datum = CallFunc ()
            if Status != FMP_SUCCESS:
                raise ValueError (f'{Key} Check Failed! [{Status}] - {Datum}')

            if isinstance (Checker, FunctionType):
                DEBUG (DEBUG_TRACE, f'{Key} - {Datum} check function call.')
                assert (Checker (Datum))
            else:
                DEBUG (DEBUG_TRACE, f'{Key} - {Datum} check variable type.')
                assert (isinstance (Datum, Checker))

            Result.update ({Key: Datum})

        return Result

    def __CapPayloadBufferToFile (self) -> None:
        """ Save the capsule payload buffer into file.

        Args:
            None.

        Raises:
            ValueError:
                FMP Device instance return the invalid payload.
                (Status is not FMP_SUCCESS)

        Returns:
            None.
        """
        Status : int        = FMP_SUCCESS
        Payload: ByteBuffer = None

        Status, Payload = self.__FmpInst.GetFwImagePayload ()

        if Status != FMP_SUCCESS:
            raise ValueError (f'FMP Device instance payload is invalid.')

        WriteBufferFile (Payload.Buffer, CAP_PAYLOAD_FILE_PATH)

    def __GenerateCapFile (self) -> None:
        """ Generate the signed capsule file.

        Note:
            This function is used to called the GenerateCapsule script
            within the EDK-II BaseTools.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        SignCertObj: SignCert = self.__FmpInfo[KEY_CAP_SIGN_CERT]
        IsVerbose  : bool     = True
        EnvCfg     : dict     = copy.deepcopy (gEdkIIEnvCfg)

        CmdList: List[str] = [
            JoinPath (EDKII_TOOL_PATH, GetGenCapsuleToolPath ()),
            '--encode',
            '--capflag',        'PersistAcrossReset',
            '--capflag',        'InitiateReset',
            '--hash-algorithm', f'{CAPSULE_HASH_ALGORITHM}',
            '--guid',           f'{self.__FmpInfo[KEY_CAP_CAP_GUID]}',
            '--fw-version',     f'{FormatHex (self.__FmpInfo[KEY_CAP_VER])}',
            '--lsv',            f'{FormatHex (self.__FmpInfo[KEY_CAP_LSV])}',
            '-o',               f'{self.__CapFilePath}',
            f'{CAP_PAYLOAD_FILE_PATH}',
            ]
        CmdList += SignCertObj.Command

        if IsVerbose:
            CmdList.append ('-v')

        #
        # Handle the capsule to sign with EDSS sign file.
        #
        if SignCertObj.SignToolType == SIGN_TOOL_EDSS:
            EnvCfg[EDSS_SIGNED_KEY] = ENV_VAR_TRUE_STR

        ExecOsSystemCmd (CmdList, Env = EnvCfg)

    def __WinUpdatePreWork (self) -> Tuple[str, str, str]:
        """ Initial the environment for Windows driver update package.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[str, str, str]:
                str:
                    Path of the update package output directory.
                str:
                    Path of the INF file.
                str:
                    Path of the CAT file.
                    (Default is lowercase filename of INF file)
        """
        OutputDirPath: str = \
            JoinPath (
              OUTPUT_WIN_DIR_PATH,
              self.__FmpInfo[KEY_CAP_DEV_NAME],
              self.__FmpInfo[KEY_CAP_NAME],
              )

        InfFileName: str = \
            CombineFileName (
              self.__FmpInfo[KEY_CAP_NAME],
              Extension = EXTENSION_WIN_INF_FILE,
              )
        InfFilePath: str = \
            JoinPath (OutputDirPath, InfFileName)

        CatFileName: str = \
            CombineFileName (
              self.__FmpInfo[KEY_CAP_NAME],
              Extension = EXTENSION_WIN_CAT_FILE,
              ).lower ()
        CatFilePath: str = \
            JoinPath (OutputDirPath, CatFileName)

        CapFilePath: str = \
            JoinPath (OutputDirPath, self.__CapFileName)

        TouchDir (OutputDirPath)
        CopyFile (self.__CapFilePath, CapFilePath)

        return OutputDirPath, InfFilePath, CatFilePath

    def __WinUpdateInfGenerate (self, OutputPath: str) -> int:
        """ Call the script to generate the INF file.

        Args:
            OutputPath (str):
                Output path of INF file.

        Raises:
            None.

        Returns:
            int:
                Exit code of the INF generator script.
        """
        HexVer: str = DecToHex (self.__FmpInfo[KEY_CAP_VER])

        CmdList: List[str] = [
            PY_WIN_INF_GENERATOR_SCRIPT_PATH,
            '-c',    f'{self.__CapFilePath}',
            '-hex',  f'{HexVer}',
            '-g',    f'{self.__FmpInfo[KEY_CAP_CAP_GUID]}',
            '-p',    f'{self.__FmpInfo[KEY_CAP_VENDOR]}',
            '-m',    f'{self.__FmpInfo[KEY_CAP_VENDOR]}',
            '-d',    f'{self.__FmpInfo[KEY_CAP_VER_STR]}',
            '-desc', f'{self.__FmpInfo[KEP_CAP_DESCRIPTION]}',
            '-o',    f'{OutputPath}',
            ]

        ExitCode, _, _ = ExecPythonCmd (CmdList, IsException = False)
        if (ExitCode != STATUS_SUCCESS):
            raise ErrorException (f'Failed to generate Windows INF file.')

        return ExitCode

    def __WinUpdateCatGenerate (self, UpdatePkgPath: str) -> int:
        """ Call the script to generate the CAT file.

        Args:
            UpdatePkgPath (str):
                Path of the Windows driver update package.

        Raises:
            None.

        Returns:
            int:
                Exit code of the CAT generator script.
        """
        CmdList: List[str] = [
            PY_WIN_CAT_GENERATOR_SCRIPT_PATH,
            '-i', f'{UpdatePkgPath}',
            ]

        ExitCode, _, _ = ExecPythonCmd (CmdList, IsException = False)
        if (ExitCode != STATUS_SUCCESS):
            raise ErrorException (f'Failed to generate Windows CAT file.')

        return ExitCode

    def __WinUpdateCatSign (self, CatFilePath: str) -> int:
        """ Call the script to sign the CAT file.

        Args:
            CatFilePath (str):
                Path of the CAT file to be signed.

        Raises:
            None.

        Returns:
            int:
                Exit code of the CAT singer script.
        """
        WinDriverUpdatePkgPfxSignKeyPath: str = \
            JoinPath (
              WORKSPACE_PATH,
              gCapSignKeyInfo[WIN_DRIVER_PFX_CERT_KEY],
              )

        CheckPathList = [
            WinDriverUpdatePkgPfxSignKeyPath,
            ]

        CheckFileListExist (CheckPathList)

        CmdList: List[str] = [
            PY_WIN_CAT_SIGNER_SCRIPT_PATH,
            '-c', f'{CatFilePath}',
            '-p', f'{WinDriverUpdatePkgPfxSignKeyPath}',
            ]

        ExitCode, _, _ = ExecPythonCmd (CmdList, IsException = False)
        if (ExitCode != STATUS_SUCCESS):
            raise ErrorException (f'Failed to sign Windows CAT file.')

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'ApiFmpCapBuild',
    ]
