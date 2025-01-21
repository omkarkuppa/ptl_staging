## @file
#  Definition of FSP firmware header.
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
#    - Intel Firmware Support Package EAS, Version 2.4 (Errata A),
#      section 5.1.1
#
##

from typing import Any, Dict, List

from CapsuleCommon.Constant.BitLib import *
from CapsuleCommon.Format.HexadecimalLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.Fsp.ConstantLib import *

#
# FSP information header structure.
#
#
# typedef struct {
#   UINT32    Signature;
#   UINT32    HeaderLength;
#   UINT8     Reserved1[2];
#   UINT8     SpecVersion;
#   UINT8     HeaderRevision;
#   UINT32    ImageRevision;
#   CHAR8     ImageId[8];
#   UINT32    ImageSize;
#   UINT32    ImageBase;
#   UINT16    ImageAttribute;
#   UINT16    ComponentAttribute;
#   UINT32    CfgRegionOffset;
#   UINT32    CfgRegionSize;
#   UINT8     Reserved2[4];
#   UINT32    TempRamInitEntryOffset;
#   UINT8     Reserved3[4];
#   UINT32    NotifyPhaseEntryOffset;
#   UINT32    FspMemoryInitEntryOffset;
#   UINT32    TempRamExitEntryOffset;
#   UINT32    FspSiliconInitEntryOffset;
#   UINT32    FspMultiPhaseSiInitEntryOffset;
#   UINT16    ExtendedImageRevision;
#   UINT8     Reserved4[2];
#   UINT32    FspMultiPhaseMemInitEntryOffset;
#   UINT32    FspSmmInitEntryOffset;
# } FSP_INFO_HEADER;
#

#
# Define of FSP information header structure member.
#
FSP_INFO_HDR_SIGNATURE                           : str = 'Signature'
FSP_INFO_HDR_HEADER_LENGTH                       : str = 'HeaderLength'
FSP_INFO_HDR_RESERVED1                           : str = 'Reserved1'
FSP_INFO_HDR_SPEC_VERSION                        : str = 'SpecVersion'
FSP_INFO_HDR_HEADER_REVISION                     : str = 'HeaderRevision'
FSP_INFO_HDR_IMAGE_REVISION                      : str = 'ImageRevision'
FSP_INFO_HDR_IMAGE_ID                            : str = 'ImageId'
FSP_INFO_HDR_IMAGE_SIZE                          : str = 'ImageSize'
FSP_INFO_HDR_IMAGE_BASE                          : str = 'ImageBase'
FSP_INFO_HDR_IMAGE_ATTRIBUTE                     : str = 'ImageAttribute'
FSP_INFO_HDR_COMPONENT_ATTRIBUTE                 : str = 'ComponentAttribute'
FSP_INFO_HDR_CFG_REGION_OFFSET                   : str = 'CfgRegionOffset'
FSP_INFO_HDR_CFG_REGION_SIZE                     : str = 'CfgRegionSize'
FSP_INFO_HDR_RESERVED2                           : str = 'Reserved2'
FSP_INFO_HDR_TEMP_RAM_INIT_ENTRY_OFFSET          : str = 'TempRamInitEntryOffset'
FSP_INFO_HDR_RESERVED3                           : str = 'Reserved3'
FSP_INFO_HDR_NOTIFY_PHASE_ENTRY_OFFSET           : str = 'NotifyPhaseEntryOffset'
FSP_INFO_HDR_FSP_MEMORY_INIT_ENTRY_OFFSET        : str = 'FspMemoryInitEntryOffset'
FSP_INFO_HDR_TEMP_RAM_EXIT_ENTRY_OFFSET          : str = 'TempRamExitEntryOffset'
FSP_INFO_HDR_FSP_SILICON_INIT_ENTRY_OFFSET       : str = 'FspSiliconInitEntryOffset'
FSP_INFO_HDR_FSP_MULTI_PHASE_SI_INIT_ENTRY_OFFSET: str = 'FspMultiPhaseSiInitEntryOffset'
FSP_INFO_HDR_EXTENDED_IMAGE_REVISION             : str = 'ExtendedImageRevision'
FSP_INFO_HDR_RESERVED4                           : str = 'Reserved4'
FSP_INFO_HDR_FSP_MULTI_PHASE_INIT_ENTRY_OFFSET   : str = 'FspMultiPhaseMemInitEntryOffset'
FSP_INFO_HDR_FSP_SMM_INIT_ENTRY_OFFSET           : str = 'FspSmmInitEntryOffset'

#
# Pre-defined string.
#
FSP_INFO_HEADER_SIGNATURE        : str = 'FSPH'
FSP_INFO_HEADER_HEADER_REVISION_6: int = 0x06
FSP_INFO_HEADER_HEADER_REVISION_7: int = 0x07

#
# FSP information header.
#
FSP_INFO_HDR_FORMAT_DICT: Dict[str, Any] = {
    FSP_INFO_HDR_SIGNATURE                           : { FIELD_FORMAT: '4c', FIELD_SIZE: 4 },
    FSP_INFO_HDR_HEADER_LENGTH                       : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_RESERVED1                           : { FIELD_FORMAT: '2B', FIELD_SIZE: 2 },
    FSP_INFO_HDR_SPEC_VERSION                        : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    FSP_INFO_HDR_HEADER_REVISION                     : { FIELD_FORMAT: 'B',  FIELD_SIZE: 1 },
    FSP_INFO_HDR_IMAGE_REVISION                      : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_IMAGE_ID                            : { FIELD_FORMAT: '8c', FIELD_SIZE: 8 },
    FSP_INFO_HDR_IMAGE_SIZE                          : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_IMAGE_BASE                          : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_IMAGE_ATTRIBUTE                     : { FIELD_FORMAT: 'H' , FIELD_SIZE: 2 },
    FSP_INFO_HDR_COMPONENT_ATTRIBUTE                 : { FIELD_FORMAT: 'H',  FIELD_SIZE: 2 },
    FSP_INFO_HDR_CFG_REGION_OFFSET                   : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_CFG_REGION_SIZE                     : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_RESERVED2                           : { FIELD_FORMAT: '4B', FIELD_SIZE: 4 },
    FSP_INFO_HDR_TEMP_RAM_INIT_ENTRY_OFFSET          : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_RESERVED3                           : { FIELD_FORMAT: '4B', FIELD_SIZE: 4 },
    FSP_INFO_HDR_NOTIFY_PHASE_ENTRY_OFFSET           : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_FSP_MEMORY_INIT_ENTRY_OFFSET        : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_TEMP_RAM_EXIT_ENTRY_OFFSET          : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_FSP_SILICON_INIT_ENTRY_OFFSET       : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_FSP_MULTI_PHASE_SI_INIT_ENTRY_OFFSET: { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_EXTENDED_IMAGE_REVISION             : { FIELD_FORMAT: 'H',  FIELD_SIZE: 2 },
    FSP_INFO_HDR_RESERVED4                           : { FIELD_FORMAT: '2B', FIELD_SIZE: 2 },
    FSP_INFO_HDR_FSP_MULTI_PHASE_INIT_ENTRY_OFFSET   : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    FSP_INFO_HDR_FSP_SMM_INIT_ENTRY_OFFSET           : { FIELD_FORMAT: 'L',  FIELD_SIZE: 4 },
    }

FSP_INFO_HDR_STRUCT   : str = str ()
FSP_INFO_HDR_BYTE_SIZE: int = int ()
FSP_INFO_HDR_STRUCT, FSP_INFO_HDR_BYTE_SIZE = \
    GetStructFormatAndByteSize (FSP_INFO_HDR_FORMAT_DICT)

class FspInfoHdr (object):
    Signature: str = FSP_INFO_HEADER_SIGNATURE
    #
    # List of items need to check for input key within dict.
    #
    FSP_INFO_HDR_MANDATORY: List[str] = [
        FSP_INFO_HDR_SIGNATURE,
        FSP_INFO_HDR_HEADER_LENGTH,
        FSP_INFO_HDR_RESERVED1,
        FSP_INFO_HDR_SPEC_VERSION,
        FSP_INFO_HDR_HEADER_REVISION,
        FSP_INFO_HDR_IMAGE_REVISION,
        FSP_INFO_HDR_IMAGE_ID,
        FSP_INFO_HDR_IMAGE_SIZE,
        FSP_INFO_HDR_IMAGE_BASE,
        FSP_INFO_HDR_IMAGE_ATTRIBUTE,
        FSP_INFO_HDR_COMPONENT_ATTRIBUTE,
        FSP_INFO_HDR_CFG_REGION_OFFSET,
        FSP_INFO_HDR_CFG_REGION_SIZE,
        FSP_INFO_HDR_RESERVED2,
        FSP_INFO_HDR_TEMP_RAM_INIT_ENTRY_OFFSET,
        FSP_INFO_HDR_RESERVED3,
        FSP_INFO_HDR_NOTIFY_PHASE_ENTRY_OFFSET,
        FSP_INFO_HDR_FSP_MEMORY_INIT_ENTRY_OFFSET,
        FSP_INFO_HDR_TEMP_RAM_EXIT_ENTRY_OFFSET,
        FSP_INFO_HDR_FSP_SILICON_INIT_ENTRY_OFFSET,
        FSP_INFO_HDR_FSP_MULTI_PHASE_SI_INIT_ENTRY_OFFSET,
        FSP_INFO_HDR_EXTENDED_IMAGE_REVISION,
        FSP_INFO_HDR_RESERVED4,
        FSP_INFO_HDR_FSP_MULTI_PHASE_INIT_ENTRY_OFFSET,
        FSP_INFO_HDR_FSP_SMM_INIT_ENTRY_OFFSET
        ]

    def __init__ (self, **Kwargs) -> None:
        """ Class used to represent the FSP information header.

        Args:
            **Kwargs: Key-Value pair.
                Key   - Member of FSP information header structure.
                Value - Correspond key value.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Kwargs: Dict[str, Any] = Kwargs

        self.__PreCheck ()

        self.__HeaderLength         : str = Kwargs[FSP_INFO_HDR_HEADER_LENGTH]
        self.__SpecVersion          : str = Kwargs[FSP_INFO_HDR_SPEC_VERSION]
        self.__HeaderRevision       : str = Kwargs[FSP_INFO_HDR_HEADER_REVISION]
        self.__ImageRevision        : str = Kwargs[FSP_INFO_HDR_IMAGE_REVISION]
        self.__ComponentAttribute   : str = Kwargs[FSP_INFO_HDR_COMPONENT_ATTRIBUTE]
        self.__ExtendedImageRevision: str = Kwargs[FSP_INFO_HDR_EXTENDED_IMAGE_REVISION]

    @property
    def HeaderLength (self) -> str:
        """ Get the header length of FSP information header. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The header length of FSP information header.
        """
        return self.__HeaderLength

    @property
    def SpecVersion (self) -> str:
        """ Get the spec version of FSP information header. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The spec version of FSP information header.
        """
        return self.__SpecVersion

    @property
    def HeaderRevision (self) -> str:
        """ Get the revision of FSP information header. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The revision of FSP information header..
        """
        return self.__HeaderRevision

    @property
    def ImageRevision (self) -> str:
        """ Get the image revision of FSP firmware. (4 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The image revision of FSP firmware.
        """
        return self.__ImageRevision

    @property
    def ComponentAttribute (self) -> str:
        """ Get the component attribute of FSP firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The component attribute of FSP firmware.
        """
        return self.__ComponentAttribute

    @property
    def ComponentType (self) -> int:
        """ Get the component type of FSP from component attribute. (4 bits)

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Component type of FSP.
        """
        Attribute: int = HexToDec (self.ComponentAttribute)
        Type     : int = None

        Type = (Attribute & (BIT12 | BIT13 | BIT14 | BIT15)) >> 12

        return Type

    @property
    def ExtendedImageRevision (self) -> str:
        """ Get the extended image revision of FSP firmware. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                The extended image revision of FSP firmware.
        """
        return self.__ExtendedImageRevision

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            ValueError:
                (1) FSP information header structure member is missing.
                (2) FSP information header signature is incorrect.
                    (FSP_INFO_HDR_SIGNATURE)

        Returns:
            None.
        """
        InputSig: str = str ()

        #
        # To check header information meets the mandatory list.
        #
        for Member in FspInfoHdr.FSP_INFO_HDR_MANDATORY:
            if Member not in self.__Kwargs:
                raise ValueError (
                        f'Member {Member} not in input information.'
                        )

        #
        # To check the parsed signature is correct.
        #
        InputSig = ByteToStr (
                     self.__Kwargs[FSP_INFO_HDR_SIGNATURE],
                     IsReverse = False
                     )

        if not (InputSig == FspInfoHdr.Signature):
            raise ValueError (
                    f'Input signature ({InputSig}) is not '
                    f'{FspInfoHdr.Signature}'
                    )
