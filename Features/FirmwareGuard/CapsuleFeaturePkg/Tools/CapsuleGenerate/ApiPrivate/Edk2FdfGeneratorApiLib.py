## @file
#  Private API to generate the EDK-II FDF to build for Capsule Payload.
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
from typing import List

from CapsuleCommon import *

CAPSULE_PAYLOAD_FDF_FILE_HEADER: str = f'''\
## @file
#  FDF file to create the Capsule Payload.
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

#
# Defines section.
#
DEFINES_SECTION_STR : str = 'Defines'
FLASH_BLOCK_SIZE_STR: str = 'FLASH_BLOCK_SIZE'

#
# FV Statements
#
FV_ALIGNMENT_STR: str = 'FvAlignment'

#
# FV Fixed Blocks.
#
BLOCK_SIZE_STR   : str = 'BlockSize'
NUM_OF_BLOCKS_STR: str = 'NumBlocks'

#
# FV Attributes.
#
MEMORY_MAPPED_STR     : str = 'MEMORY_MAPPED'
LOCK_CAP_STR          : str = 'LOCK_CAP'
LOCK_STATUS_STR       : str = 'LOCK_STATUS'
WRITE_LOCK_CAP_STR    : str = 'WRITE_LOCK_CAP'
WRITE_LOCK_STATUS_STR : str = 'WRITE_LOCK_STATUS'
WRITE_ENABLED_CAP_STR : str = 'WRITE_ENABLED_CAP'
WRITE_DISABLED_CAP_STR: str = 'WRITE_DISABLED_CAP'
WRITE_STATUS_STR      : str = 'WRITE_STATUS'
STICKY_WRITE_STR      : str = 'STICKY_WRITE'
READ_LOCK_CAP_STR     : str = 'READ_LOCK_CAP'
READ_LOCK_STATUS_STR  : str = 'READ_LOCK_STATUS'
READ_ENABLED_CAP_STR  : str = 'READ_ENABLED_CAP'
READ_DISABLED_CAP_STR : str = 'READ_DISABLED_CAP'
READ_STATUS_STR       : str = 'READ_STATUS'
ERASE_POLARITY_STR    : str = 'ERASE_POLARITY'

class _Edk2RawFile (object):
    def __init__ (self, Path: str, Alignment: Union[None, str] = None):
        """ Class to represent the raw file within the FDF file.

        Args:
            Path (str):
                Path of the raw file.
                (Expect it should related path of workspace path)
            Alignment (Union[None, str], optional):
                Alignment of this raw file.
                (Should be one of FFS_ALIGNMENT_LIST)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Path     : str              = Path
        self.__Alignment: Union[None, str] = Alignment

        self.__PreCheck ()

    @property
    def Path (self) -> str:
        """ Return the path of the raw file.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of the raw file.
        """
        return self.__Path

    @property
    def Alignment (self) -> Union[None, str]:
        """ Return the alignment of this raw file.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, str]:
                Return the alignment for this raw file.
                (If alignment is not necessary, keep it as None)
        """
        return self.__Alignment

    def __PreCheck (self):
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Path not str type.
                (2) Alignment not str type.
            ValueError:
                Assigned alignment is not valid.

        Returns:
            None.
        """
        if not isinstance (self.__Path, str):
            raise TypeError (f'Path should be str type.')

        DEBUG (
          DEBUG_TRACE,
          f'Assign the alignment [{self.__Alignment}] for [{self.__Path}]'
          )

        if self.__Alignment is None:
            return
        elif not isinstance (self.__Alignment, str):
            raise TypeError (f'Alignment should be str type.')
        elif self.__Alignment not in FFS_ALIGNMENT_LIST:
            raise ValueError (f'Assigned FFS alignment is invalid.')

class _Edk2RawFfs (object):
    def __init__ (self, Guid: uuid.UUID):
        """ Class to represent the raw FFS within the FDF file.

        Args:
            Guid (uuid.UUID):
                The GUID value of this FFS.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Guid: uuid.UUID = Guid

        self.__PreCheck ()

        self.__RawFileList: List[_Edk2RawFile] = list ()

    @property
    def Guid (self) -> uuid.UUID:
        """ Return the GUID value of this raw FFS.

        Args:
            None.

        Raises:
            None.

        Returns:
            uuid.UUID:
                The GUID value of this FFS.
                (Each GUID should be unique)
        """
        return self.__Guid

    @property
    def Info (self) -> str:
        """ Return the string to be output within this FFS.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to output for this FDF file.
        """
        FfsStrList: list = list ()

        FfsStrList.append (f'FILE RAW = {str (self.Guid).upper ()} {{\n')

        for RawFile in self.__RawFileList:
            if RawFile.Alignment:
                FfsStrList.append (
                             Padding (
                               f'  Align={RawFile.Alignment}',
                               Size           = 12,
                               IsPaddingRight = True,
                               )
                             )
                FfsStrList.append (f'  {RawFile.Path}\n')
            else:
                FfsStrList.append (f'  {RawFile.Path}\n')

        FfsStrList.append (f'  }}\n')
        FfsStrList.append (f'\n')

        return ''.join (FfsStrList)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                Guid not UUID type.

        Returns:
            None.
        """
        if not isinstance (self.__Guid, uuid.UUID):
            raise TypeError (f'Guid should be UUID type.')

    def AddRawFile (
        self,
        Path     : str,
        Alignment: Union[None, str] = None,
        ) -> None:
        """ API to add the raw file within this FFS.

        Args:
            Path (str):
                Path of the raw file.
                (Expect it should related path of workspace path)
            Alignment (Union[None, str], optional):
                Alignment of this raw file.
                (Should be one of FFS_ALIGNMENT_LIST)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        RawFile: _Edk2RawFile = _Edk2RawFile (Path, Alignment)
        self.__RawFileList.append (RawFile)

class _Edk2Fv (object):
    def __init__ (self) -> None:
        """ Class to represent the FV within the FDF file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        self.FvAlignment     : int  = 16
        self.NumBlocks       : int  = None
        self.ErasePolarity   : int  = 1
        self.MemoryMapped    : bool = True
        self.StickyWrite     : bool = True
        self.LockCap         : bool = True
        self.LockStatus      : bool = True
        self.WriteDisabledCap: bool = True
        self.WriteEnabledCap : bool = True
        self.WriteStatus     : bool = True
        self.WriteLockCap    : bool = True
        self.WriteLockStatus : bool = True
        self.ReadDisabledCap : bool = True
        self.ReadEnabledCap  : bool = True
        self.ReadStatus      : bool = True
        self.ReadLockCap     : bool = True
        self.ReadLockStatus  : bool = True

class CapsulePayloadFdfGeneratorApi (object):
    def __init__ (
        self,
        FvName        : str,
        FlashBlockSize: int,
        NumBlocks     : int = None,
        ):
        """ Class for API to create EDK-II FDF file for capsule payload.

        Note:
            Current only support one FV within the capsule payload.

        Args:
            FvName (str):
                The FV name for this capsule payload.
            FlashBlockSize (int):
                The flash block size of the target platform.
            NumBlocks (int, optional):
                The number of the blocks.
                (It should be expected FV size / flash block size)
                Defaults to None.

        Raises:
            None.

        Returns:
            None.
        """
        self.__FvName        : str = FvName
        self.__FlashBlockSize: int = FlashBlockSize
        self.__NumBlocks     : int = NumBlocks

        self.__PreCheck ()

        self.__Cfg       : INICfgFile             = INICfgFile ()
        self.__RawFfsDict: Dict[str, _Edk2RawFfs] = dict ()

        self.__UpdateDefinesSection ()
        self.__AddEdk2Fv ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) FvName not str type.
                (2) FlashBlockSize not int type.
                (3) NumBlock not None or int type.

        Returns:
            None.
        """
        if not isinstance (self.__FvName, str):
            raise TypeError (f'FvName should be str type.')
        elif not isinstance (self.__FlashBlockSize, int):
            raise TypeError (f'FlashBlockSize should be int type.')

        if self.__NumBlocks is None:
            pass
        elif not isinstance (self.__NumBlocks, int):
            raise TypeError (f'NumBlock should be int type.')

    def __UpdateDefinesSection (self):
        """ Add the define section within FDF file.

        Note:
            It should be on the top section within the FDF file.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        D_FLASH_BLOCK_SIZE_STR: str = f'DEFINE {FLASH_BLOCK_SIZE_STR}'
        D_FLASH_BLOCK_SIZE_VAL: str = FormatHex (
                                        self.__FlashBlockSize,
                                        IsPadding = True,
                                        )

        MappingData: list = [
            [D_FLASH_BLOCK_SIZE_STR, D_FLASH_BLOCK_SIZE_VAL],
            ]

        self.__Cfg.UpdateDataToSection (DEFINES_SECTION_STR, MappingData)

    def __AddEdk2Fv (self):
        """ Add the FV section within the FDF file.

        Note:
            It is only supported one FV as capsule payload.

        Args:
            None.

        Raises:
            None.

        Returns:
            None.
        """
        FdfFvName  : str     = f'FV.{self.__FvName}'
        FvSection  : _Edk2Fv = _Edk2Fv ()
        MappingData: list    = list ()

        FvSection.NumBlocks = self.__NumBlocks

        MappingData.append ([f'{FV_ALIGNMENT_STR}', f'{FvSection.FvAlignment}'])

        if FvSection.NumBlocks:
            MappingData.append ([
                          f'{BLOCK_SIZE_STR}',
                          f'$({FLASH_BLOCK_SIZE_STR})',
                          ])
            MappingData.append ([
                          f'{NUM_OF_BLOCKS_STR}',
                          f'{FvSection.NumBlocks}',
                          ])

        self.__Cfg.UpdateDataToSection (f'{FdfFvName}', MappingData)

        MappingData = [
            [f'{ERASE_POLARITY_STR}',     f'{str (FvSection.ErasePolarity).upper ()}'   ],
            [f'{MEMORY_MAPPED_STR}',      f'{str (FvSection.MemoryMapped).upper ()}'    ],
            [f'{STICKY_WRITE_STR}',       f'{str (FvSection.StickyWrite).upper ()}'     ],
            [f'{LOCK_CAP_STR}',           f'{str (FvSection.LockCap).upper ()}'         ],
            [f'{LOCK_STATUS_STR}',        f'{str (FvSection.LockStatus).upper ()}'      ],
            [f'{WRITE_DISABLED_CAP_STR}', f'{str (FvSection.WriteDisabledCap).upper ()}'],
            [f'{WRITE_ENABLED_CAP_STR}',  f'{str (FvSection.WriteEnabledCap).upper ()}' ],
            [f'{WRITE_STATUS_STR}',       f'{str (FvSection.WriteStatus).upper ()}'     ],
            [f'{WRITE_LOCK_CAP_STR}',     f'{str (FvSection.WriteLockCap).upper ()}'    ],
            [f'{WRITE_LOCK_STATUS_STR}',  f'{str (FvSection.WriteLockStatus).upper ()}' ],
            [f'{READ_DISABLED_CAP_STR}',  f'{str (FvSection.ReadDisabledCap).upper ()}' ],
            [f'{READ_ENABLED_CAP_STR}',   f'{str (FvSection.ReadEnabledCap).upper ()}'  ],
            [f'{READ_STATUS_STR}',        f'{str (FvSection.ReadStatus).upper ()}'      ],
            [f'{READ_LOCK_CAP_STR}',      f'{str (FvSection.ReadLockCap).upper ()}'     ],
            [f'{READ_LOCK_STATUS_STR}',   f'{str (FvSection.ReadLockStatus).upper ()}'  ],
            ]
        self.__Cfg.UpdateDataToSection (f'{FdfFvName}', MappingData)

    def AddRawFile (
        self,
        Guid       : uuid.UUID,
        PayloadPath: str,
        Alignment  : Union[None, str] = None,
        ):
        """ API to add the raw file within FFS by GUID.

        Args:
            Guid (uuid.UUID):
                GUID value of this raw FFS.
            PayloadPath (str):
                Path of the raw file.
                (Expect it should related path of workspace path)
            Alignment (Union[None, str], optional):
                Alignment of this raw file.
                (Should be one of FFS_ALIGNMENT_LIST)
                Defaults to None.

        Raises:
            TypeError:
                Guid not UUID type.

        Returns:
            None.
        """
        if not isinstance (Guid, uuid.UUID):
            raise TypeError (f'Guid should be UUID type.')

        GuidStr: str = str (Guid).upper ()

        if GuidStr not in self.__RawFfsDict.keys():
            self.__RawFfsDict.update ({GuidStr: _Edk2RawFfs (Guid)})

        self.__RawFfsDict[GuidStr].AddRawFile (PayloadPath, Alignment)

    def Save (self, FilePath: Union[str, os.PathLike]):
        """ Save the FDF file into storage.

        Note:
            The order to generate the FDF file.
            (1) Copyright header
            (2) Define and FV section.
            (3) FFS section.

        Args:
            FilePath (Union[str, os.PathLike]):
                Path of the FDF to output.

        Raises:
            None.

        Returns:
            None.
        """
        Path  : str  = AbsPath (FilePath)
        Header: File = File (FileContent = CAPSULE_PAYLOAD_FDF_FILE_HEADER)

        Header.Save (Path)
        self.__Cfg.Save (Path, 'a')

        with open (Path, 'a') as Fp:
            for Guid, RawFfs in self.__RawFfsDict.items ():
                DEBUG (DEBUG_TRACE, f'Output the FFS {Guid}')
                Fp.write (RawFfs.Info)

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'CapsulePayloadFdfGeneratorApi',
    ]
