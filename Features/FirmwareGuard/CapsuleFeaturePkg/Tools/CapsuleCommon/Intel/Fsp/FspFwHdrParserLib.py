## @file
#  Parse the FSP firmware header from binary file.
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

from typing import Dict, List

from CapsuleCommon.Base.DebugLib import *
from CapsuleCommon.Format.HexadecimalLib import *
from CapsuleCommon.Wrapper.ByteBufferWrapperLib import *
from CapsuleCommon.Wrapper.StructWrapperLib import *

from CapsuleCommon.Intel.Fsp.ConstantLib import *
from CapsuleCommon.Intel.Fsp.FspFwHdrLib import *

#
# Definition of FSP component.
#
FSP_COMPONENT_MAPPING_TABLE: Dict[int, str] = {
    FSP_T_COMPONENT_ID: FSP_T,
    FSP_M_COMPONENT_ID: FSP_M,
    FSP_S_COMPONENT_ID: FSP_S,
    FSP_I_COMPONENT_ID: FSP_I,
    FSP_O_COMPONENT_ID: FSP_O,
    }

class FspFwVersion (object):
    def __init__ (self, InfoHdr: FspInfoHdr) -> None:
        """ Class to represent the FSP firmware version.

        Note:
            This supported from the header version of 6.

        Args:
            InfoHdr (FspInfoHdr):
                Information header of FSP.
                (Should be FspInfoHdr object)

        Raises:
            None.

        Returns:
            None.
        """
        self.__FspInfoHdr: FspInfoHdr = InfoHdr

        self.__PreCheck ()

    @property
    def Major (self) -> str:
        """ Field of major in FSP version. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Field of major in FSP version in HEX string.
        """
        Value: int = HexToDec (self.__FspInfoHdr.ImageRevision)
        Mask : int = BIT24 | BIT25 | BIT26 | BIT27 | BIT28 | BIT29 | BIT30 | BIT31

        Value = (Value & Mask) >> 24

        return DecToHex (Value)

    @property
    def Minor (self) -> str:
        """ Field of minor in FSP version. (1 byte)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Field of minor in FSP version in HEX string.
        """
        Value: int = HexToDec (self.__FspInfoHdr.ImageRevision)
        Mask : int = BIT16 | BIT17 | BIT18 | BIT19 | BIT20 | BIT21 | BIT22 | BIT23

        Value = (Value & Mask) >> 16

        return DecToHex (Value)

    @property
    def Revision (self) -> str:
        """ Field of revision in FSP version. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Field of revision in FSP version in HEX string.
        """
        Value   : int = HexToDec (self.__FspInfoHdr.ImageRevision)
        ExtValue: int = HexToDec (self.__FspInfoHdr.ExtendedImageRevision)
        Mask    : int = BIT8 | BIT9 | BIT10 | BIT11 | BIT12 | BIT13 | BIT14 | BIT15

        Value    = (Value & Mask) >> 8
        ExtValue = (ExtValue & Mask) >> 8

        return DecToHex ((ExtValue << 8) | Value)

    @property
    def Build (self) -> str:
        """ Field of build in FSP version. (2 bytes)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Field of build in FSP version in HEX string.
        """
        Value   : int = HexToDec (self.__FspInfoHdr.ImageRevision)
        ExtValue: int = HexToDec (self.__FspInfoHdr.ExtendedImageRevision)
        Mask    : int = BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7

        Value    = (Value & Mask)
        ExtValue = (ExtValue & Mask)

        return DecToHex ((ExtValue << 8) | Value)

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                InfoHdr not FspInfoHdr type.
            ValueError:
                Information header version lower than 6.

        Returns:
            None.
        """
        InfoHdrVer: int = None

        if not isinstance (self.__FspInfoHdr, FspInfoHdr):
            raise TypeError (f'InfoHdr shall be FspInfoHdr type.')

        InfoHdrVer = HexToDec (self.__FspInfoHdr.HeaderRevision)
        if InfoHdrVer < FSP_INFO_HEADER_HEADER_REVISION_6:
            raise ValueError (f'FSP information header version lower than 6.')

class FspFwHdrParser (object):
    def __init__ (
        self,
        Buffer: ByteBuffer,
        ) -> None:
        """ Class used to parse the FSP firmware header from buffer.

        Args:
            Buffer (ByteBuffer):
                Wrapper of ByteBuffer input.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Buffer       : ByteBuffer            = Buffer
        self.__InfoHdrSigLoc: List[int]             = self.__GetInfoHdrSigLoc ()
        self.__InfoHdrDict  : Dict[str, FspInfoHdr] = self.__GetInfoHdrDict ()
        self.__FspVersion   : FspFwVersion          = self.__GetFspVersion ()

    @property
    def InfoHeaderDict (self) -> Dict[str, FspInfoHdr]:
        """ Information header of component in FSP firmware.

        Args:
            None.

        Raises:
            None.

        Returns:
            dict:
                Key:
                    Name of the component.
                Value:
                    The header of FSP information.
                    (Should be FspInfoHdr object)
        """
        return self.__InfoHdrDict

    @property
    def Version (self) -> FspFwVersion:
        """ Version information of FSP.

        Args:
            None.

        Raises:
            None.

        Returns:
            FspFwVersion:
                The FSP version information of this buffer.
                (Should be FspFwVersion object)
        """
        return self.__FspVersion

    def __GetInfoHdrSigLoc (self) -> List[int]:
        """ Find the FSP information header offset from the buffer image.

        Args:
            None.

        Raises:
            ValueError:
                (1) FSP information header signature is not found.
                (2) FSP component more than MAX_FSP_COMPONENT_NUMBER.

        Returns:
            List[int]:
                List of FSP information header signature offset.
        """
        Loc: List[int] = self.__Buffer.Find (FSP_INFO_HEADER_SIGNATURE)

        if len (Loc) == 0:
            raise ValueError (
                    f'FSP information header signature is not found.'
                    )

        if len (Loc) > MAX_FSP_COMPONENT_NUMBER:
            raise ValueError (
                    f'FSP component number found [{len (Loc)}].'
                    )

        return Loc

    def __GetComponentName (self, ComponentType: int) -> str:
        """ Get the component of name based on component type.

        Args:
            ComponentType (int):
                Value of component type.

        Raises:
            TypeError:
                ComponentType is not int type.
            ValueError:
                ComponentType is invalid.

        Returns:
            str:
                Name of the component.
        """
        if not isinstance (ComponentType, int):
            raise TypeError (
                    f'ComponentType shall be int type.'
                    )

        if ComponentType not in FSP_COMPONENT_MAPPING_TABLE:
            raise ValueError (
                    f'Unrecognized component type - [{ComponentType}]'
                    )

        return FSP_COMPONENT_MAPPING_TABLE[ComponentType]

    def __GetInfoHdrDict (self) -> Dict[str, FspInfoHdr]:
        """ Get the information header of component in FSP firmware.

        Args:
            None.

        Raises:
            ValueError:
                Unsupported FSP information header version.

        Returns:
            dict:
                Key:
                    Name of the component.
                Value:
                    The header of FSP information.
                    (Should be FspInfoHdr object)
        """
        BeginOffset  : int                   = None
        EndOffset    : int                   = None
        InfoHdrBuffer: bytearray             = None
        InfoHdrInfo  : Struct                = None
        InfoHdr      : FspInfoHdr            = None
        ComponentName: str                   = None
        InfoHdrDict  : Dict[str, FspInfoHdr] = dict ()

        for BeginOffset in self.__InfoHdrSigLoc:
            EndOffset     = BeginOffset + FSP_INFO_HDR_BYTE_SIZE
            InfoHdrBuffer = self.__Buffer.Buffer[BeginOffset:EndOffset]

            InfoHdrInfo   = Struct (
                              Buffer     = InfoHdrBuffer,
                              FormatDict = FSP_INFO_HDR_FORMAT_DICT,
                              ByteOrder  = BYTE_ORDER_LITTLE_ENDIAN,
                              )
            InfoHdr       = FspInfoHdr (**InfoHdrInfo.Data)
            ComponentName = self.__GetComponentName (InfoHdr.ComponentType)

            InfoHdrDict.update ({ComponentName: InfoHdr})

        return InfoHdrDict

    def __GetFspVersion (self) -> FspFwVersion:
        """ Get the version information of FSP.

        Args:
            None.

        Raises:
            None.

        Returns:
            FspFwVersion:
                The FSP version information of this buffer.
                (Should be FspFwVersion object)
        """
        InfoHdr   : FspInfoHdr   = None
        FspVersion: FspFwVersion = None

        #
        # Each component inside FSP buffer have same version information.
        #
        for _, InfoHdr in self.InfoHeaderDict.items ():
            FspVersion = FspFwVersion (InfoHdr = InfoHdr)
            break

        return FspVersion
