## @file
#  Private API to generate the BiosGuard Update Package (BGUP).
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

import os

from CapsuleCommon import *

from CapsuleGenerate._Global import *

BIOSGUARD_PLATFORM_NAME_MAX_LENGTH: int = 16

class BgupGenerateApi (object):
    #
    # Get the program/certificate section configuration.
    #
    PROGRAM_SECTION: dict = gBiosGuardCfgData[BIOSGUARD_PROGRAM_SECTION]
    CERT_SECTION   : dict = gBiosGuardCfgData[BIOSGUARD_CERTIFICATE_SECTION]
    #
    # BiosGuard program
    #
    PROGRAM_BGSL2BIN_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          PROGRAM_SECTION[BGSL2BIN_PROGRAM_KEY],
          )
    PROGRAM_BGPB_PATH    : str = \
        JoinPath (
          WORKSPACE_PATH,
          PROGRAM_SECTION[BGPB_PROGRAM_KEY],
          )
    #
    # Certificate used to sign the BGUP.
    #
    CERT_SIGN_TYPE     : str = CERT_SECTION[BIOSGUARD_SIGN_TYPE_KEY]
    CERT_SIGN_ALGORITHM: str = CERT_SECTION[BIOSGUARD_SIGN_ALGORITHM_KEY]
    CERT_SIGN_TOOL_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          CERT_SECTION[BIOSGUARD_SIGN_TOOL_KEY],
          )
    CERT_PUB_KEY_PATH  : str = \
        JoinPath (
          WORKSPACE_PATH,
          CERT_SECTION[BIOSGUARD_SIGN_PBK_KEY],
          )
    CERT_PRI_KEY_PATH  : str = \
        JoinPath (
          WORKSPACE_PATH,
          CERT_SECTION[BIOSGUARD_SIGN_PVK_KEY],
          )
    #
    #
    # Python script used for this API.
    #
    PY_BGUP_BUILDER_SCRIPT_PATH: str = \
        JoinPath (
          WORKSPACE_PATH,
          gBiosGuardCfgData[BGUP_GENERATOR_SCRIPT_KEY],
          )

    def __init__ (
        self,
        FileName    : str,
        BgslPath    : Union[str, os.PathLike],
        PlatformName: str,
        BiosSvn     : str,
        Prefix      : str                     = '',
        DataPath    : Union[str, os.PathLike] = None,
        IsUseSFam   : bool                    = True,
        IsUseFtu    : bool                    = True,
        ) -> None:
        """ Class for API to build the BiosGuard Update Package (BGUP).

        Args:
            FileName (str):
                The file name of this BGUP without extension part.
            BgslPath (Union[str, os.PathLike]):
                The path of BGSL file to be compiled to BGUP.
            PlatformName (str):
                The platform name of this BGUP. (Maximum 16 characters)
            BiosSvn (str):
                The BIOS SVN. (Should be UINT32)
            Prefix (str, optional):
                The prefix of this BGUP file name.
                Defaults to ''.
            DataPath (Union[str, os.PathLike], optional):
                The path of data buffer for this BGUP.
                Defaults to None.
            IsUseSFam (bool, optional):
                True  - Need to update the SFAM covered range.
                False - No need to update the SFAM covered range.
                Defaults to True.
            IsUseFtu (bool, optional):
                True  - Script use the Fault Tolerant Update (FTU).
                False - Script not use the Fault Tolerant Update (FTU).
                Defaults to True.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FileName    : str  = FileName
        self.__BgslPath    : str  = AbsPath (BgslPath)
        self.__DataPath    : str  = AbsPath (DataPath)
        self.__PlatformName: str  = PlatformName
        self.__Prefix      : str  = Prefix
        self.__IsUseSFam   : bool = IsUseSFam
        self.__IsUseFtu    : bool = IsUseFtu
        self.__BiosSvn     : str  = \
            FormatHex (
              BiosSvn,
              IsPadding      = True,
              IsPaddingRight = True,
              )

        self.__PreCheck ()

        self.__BgupFileName: str = \
            CombineFileName (
              self.__Prefix,
              self.__FileName,
              Extension = EXTENSION_BGUP_FILE,
              )
        self.__BgupFilePath: str = \
            JoinPath (
              BIOSGUARD_BGUP_DIR,
              self.__BgupFileName,
              )

    @property
    def FileName (self) -> str:
        """ Return the file name of BGUP. (Without extension)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The file name of this BGUP without extension part.
        """
        return self.__BgupFileName

    @property
    def FilePath (self) -> str:
        """ Return the file path of this BGUP.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of the signed BGUP.
        """
        return self.__BgupFilePath

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FileName is not str type.
                (2) PlatformName is not str type.
                (3) Prefix is not str type.
                (4) IsUseSfam is not bool type.
                (5) IsUseFtu is not bool type.
            ValueError:
                (1) PlatformName is too long.
                (2) BiosSvn is not UINT32.

        Returns:
            None.
        """
        if not isinstance (self.__FileName, str):
            raise TypeError ('FileName should be str type.')
        elif not isinstance (self.__PlatformName, str):
            raise TypeError ('PlatformName should be str type.')
        elif not isinstance (self.__Prefix, str):
            raise TypeError ('Prefix should be str type.')
        elif not isinstance (self.__IsUseSFam, bool):
            raise TypeError ('IsUseSFam should be bool type.')
        elif not isinstance (self.__IsUseFtu, bool):
            raise TypeError ('IsUseFtu should be bool type.')
        elif len (self.__PlatformName) > BIOSGUARD_PLATFORM_NAME_MAX_LENGTH:
            raise ValueError (
                    f'PlatformName is too long [{len (self.__PlatformName)}]'
                    f' > [{BIOSGUARD_PLATFORM_NAME_MAX_LENGTH}] '
                    )
        elif not IsUint32 (HexToDec (self.__BiosSvn)):
            raise ValueError (f'BIOS SVN should be UINT32. [{self.__BiosSvn}]')

    def Build (self) -> int:
        """ Call the script to build BiosGuard Update Package (BGUP).

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                The exit code of BGUP generator script.
                (Detail exit code should be reference its own)
        """
        DEBUG (DEBUG_INFO, f'Build BGUP with Platform: {self.__PlatformName}')
        DEBUG (DEBUG_INFO, f'Build BGUP with BIOS SVN: {self.__BiosSvn}')

        TouchDir (BIOSGUARD_BGUP_DIR)

        CmdList: List[str] = [
            f'{BgupGenerateApi.PY_BGUP_BUILDER_SCRIPT_PATH}',
            '-i',         f'{self.__BgslPath}',
            '-p',         f'{self.__PlatformName}',
            '-biossvn',   f'{self.__BiosSvn}',
            #
            # Necessary BiosGuard Update Package build.
            #
            '-bgsl2bin',  f'{BgupGenerateApi.PROGRAM_BGSL2BIN_PATH}',
            '-bgpb',      f'{BgupGenerateApi.PROGRAM_BGPB_PATH}',
            #
            # Certificate to sign the BGUP.
            #
            '-signtype',  f'{BgupGenerateApi.CERT_SIGN_TYPE}',
            '-algorithm', f'{BgupGenerateApi.CERT_SIGN_ALGORITHM}',
            '-signtool',  f'{BgupGenerateApi.CERT_SIGN_TOOL_PATH}',
            '-pbk',       f'{BgupGenerateApi.CERT_PUB_KEY_PATH}',
            '-pvk',       f'{BgupGenerateApi.CERT_PRI_KEY_PATH}',
            #
            # Additional configuration.
            #
            '-use_sfam',  f'{str (self.__IsUseSFam)}',
            '-use_ftu',   f'{str (self.__IsUseFtu)}',
            #
            # Output path of signed BGUP file.
            #
            '-o',         f'{self.__BgupFilePath}',
            ]

        if self.__DataPath:
            CmdList.append ('-d')
            CmdList.append (f'{self.__DataPath}')

        ExitCode, _, _ = ExecPythonCmd (CmdList, IsException = False)
        if (ExitCode != STATUS_SUCCESS):
            raise ErrorException (f'Failed to generate BGUP.')

        return ExitCode

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'BgupGenerateApi',
    ]
