## @file
#  Private API to generate the EDK-II DSC to build for Capsule Payload.
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

#
# Placeholder string for DSC file description.
#
DSC_FILE_DESC_MAX_LENGTH : int = 60
DSC_FILE_DESC_KEY_STR    : str = '%DSC_FILE_DESC%'
DSC_FILE_DESC_DEFAULT_STR: str = 'DSC file to create the Capsule Payload.'

EDKII_DSC_FILE_HEADER    : str = f'''\
## @file
#  {DSC_FILE_DESC_KEY_STR}
#
#  DO NOT MODIFY THIS FILE.
#
#  @copyright
{INTEL_COPYRIGHT_STR}
#
#  @par Specification
#
##
'''

class Edk2DscGeneratorApi (object):
    def __init__ (self) -> None:
        """ Class for API to generate the EDK-II DSC file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FileDescStr  : str = DSC_FILE_DESC_DEFAULT_STR
        self.__PlatformName : str = EDKII_CAP_PKG_NAME
        self.__PlatformGuid : str = EDKII_CAP_PKG_GUID.upper ()
        self.__PlatformVer  : str = '1.0'
        self.__FdfFile      : str = EDKII_FDF_FILE_NAME
        self.__SupportedArch: str = f'{"|".join (EDKII_BLD_ARCH_LIST)}'
        self.__BuildTarget  : str = f'{"|".join (BUILD_TARGET_TYPE_LIST)}'

    @property
    def FileDescStr (self) -> None:
        """ Get the DSC file description set in DSC object.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                DSC file description get set in DSC object.
        """
        return self.__FileDescStr

    @FileDescStr.setter
    def FileDescStr (self, Desc: str) -> None:
        """ Set the customized file description for DSC object.

        Args:
            Desc (str):
                String of file description to be set.

        Raises:
            TypeError:
                Desc is not str type.
            ValueError:
                Desc is too long. (Out of DSC_FILE_DESC_MAX_LENGTH)

        Returns:
            None.
        """
        if not isinstance (Desc, str):
            raise TypeError (f'Desc should be str type.')
        elif len (Desc) > DSC_FILE_DESC_MAX_LENGTH:
            raise ValueError (
                    f'Length of Desc is out of [{DSC_FILE_DESC_MAX_LENGTH}]',
                    )

        self.__FileDescStr = Desc

    @property
    def PlatformName (self) -> str:
        """ Get the platform name set in DSC object.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Platform name get set in DSC object.
        """
        return self.__PlatformName

    @PlatformName.setter
    def PlatformName (self, Name: str) -> None:
        """ Set the customized platform name for DSC object.

        Args:
            Name (str):
                Platform name to be set.

        Raises:
            TypeError:
                Input is not str type.
            ValueError:
                Space is contained in the input string.

        Returns:
            None.
        """
        if not isinstance (Name, str):
            raise TypeError (f'Platform name should be str type.')
        elif Name.isspace ():
            raise ValueError (f'Space is not allowed in platform name.')

        self.__PlatformName = Name

    @property
    def PlatformGuid (self) -> str:
        """ Get the platform GUID set in DSC object.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Platform GUID get set in DSC object.
        """
        return self.__PlatformGuid

    @PlatformGuid.setter
    def PlatformGuid (self, Guid: Union[str, uuid.UUID]) -> None:
        """ Set the customized platform GUID for DSC object.

        Note:
            Function would convert the GUID into uppercase implied.

        Args:
            Guid (Union[str, uuid.UUID]):
                GUID to be set.

        Raises:
            TypeError:
                Guid is neither str nor uuid.UUID.

        Returns:
            None.
        """
        if not isinstance (Guid, (str, uuid.UUID)):
            raise TypeError (f'Guid should be str or uuid.UUID.')

        GuidValue: uuid.UUID = uuid.UUID (Guid)

        self.__PlatformGuid = str (GuidValue).upper ()

    @property
    def FdfFile (self) -> str:
        """ Get the FDF file set in DSC object.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                FDF file get set in DSC object.
        """
        return self.__FdfFile

    @FdfFile.setter
    def FdfFile (self, FdfFileInput: str) -> None:
        """ Set the customized FDF file name for DSC object.

        Args:
            FdfFileInput (str):
                FDF file name to be set.

        Raises:
            TypeError:
                Input is not str type.
            ValueError:
                Space is contained in the input string.

        Returns:
            None.
        """
        if not isinstance (FdfFileInput, str):
            raise TypeError (f'FDF file should be str type.')
        elif FdfFileInput.isspace ():
            raise ValueError (f'Space is not allowed in FDF file.')

        self.__FdfFile = FdfFileInput

    def Save (self, OutputPath: Union[str, os.PathLike]) -> None:
        """ Save the DSC file from memory object to storage.

        Args:
            OutputPath (Union[str, os.PathLike]):
                The path of DSC file to be output.

        Raises:
            None.

        Returns:
            None.
        """
        FilePath: str = AbsPath (OutputPath)

        HeaderContent: str = \
            EDKII_DSC_FILE_HEADER.replace (
                                    DSC_FILE_DESC_KEY_STR,
                                    self.__FileDescStr,
                                    )

        Header     : File       = File (FileContent = HeaderContent)
        DscFile    : INICfgFile = INICfgFile ()
        MappingData: list       = [
            ['DSC_SPECIFICATION'      , EDKII_DSC_SPEC_LATEST_VER_STR                ],
            ['PLATFORM_NAME'          , self.__PlatformName                          ],
            ['PLATFORM_GUID'          , self.__PlatformGuid                          ],
            ['PLATFORM_VERSION'       , self.__PlatformVer                           ],
            ['FLASH_DEFINITION'       , self.__FdfFile                               ],
            ['OUTPUT_DIRECTORY'       , f'{EDKII_BLD_DIR_NAME}/{self.__PlatformName}'],
            ['SUPPORTED_ARCHITECTURES', self.__SupportedArch                         ],
            ['BUILD_TARGETS'          , self.__BuildTarget                           ],
            ]

        Header.Save (FilePath)
        DscFile.UpdateDataToSection ('Defines', MappingData)
        DscFile.Save (FilePath, 'a')
