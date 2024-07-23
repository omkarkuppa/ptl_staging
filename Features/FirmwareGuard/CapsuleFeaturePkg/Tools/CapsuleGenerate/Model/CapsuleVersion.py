## @file
#  Description a Capsule Version object should expose what methods.
#
#  Note:
#    The main purpose of this abstract object is used to unify the
#    version expression for capsule build.
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

from abc import ABCMeta, abstractmethod
from typing import List

SEPARATE_VERSION_INPUT_TYPE: int = 0x0
DOT_VERSION_INPUT_TYPE     : int = 0x1
DOT_VERSION_DEC_INPUT_TYPE : int = 0x2
DOT_VERSION_HEX_INPUT_TYPE : int = 0x3
FULL_VERSION_INPUT_TYPE    : int = 0x4
FULL_DEC_VERSION_INPUT_TYPE: int = 0x5
FULL_HEX_VERSION_INPUT_TYPE: int = 0x6
FW_HEADER_INPUT_TYPE       : int = 0x7

CAPSULE_VERSION_INPUT_TYPE_LIST: List[int] = [
    SEPARATE_VERSION_INPUT_TYPE,
    DOT_VERSION_INPUT_TYPE,
    DOT_VERSION_DEC_INPUT_TYPE,
    DOT_VERSION_HEX_INPUT_TYPE,
    FULL_VERSION_INPUT_TYPE,
    FULL_DEC_VERSION_INPUT_TYPE,
    FULL_HEX_VERSION_INPUT_TYPE,
    FW_HEADER_INPUT_TYPE,
    ]

class CapsuleVersion (metaclass = ABCMeta):
    @abstractmethod
    def GetDecVersion (self) -> str:
        """ Return the firmware version format in DEC style. (32-bit)

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            str:
                String to represent the DEC version.
        """
        return NotImplementedError

    @abstractmethod
    def GetHexVersion (self) -> str:
        """ Return the firmware version format in HEX style. (32-bit)

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            str:
                String to represent the HEX version.
        """
        return NotImplementedError

    @abstractmethod
    def GetDecDotVersion (self) -> str:
        """ Return the firmware version format in DEC dot style.

        Note:
            (1) Represent as MMMMM.mmmmm.nnnnn.RRRRR (DEC format)
            (2) Each segment is UINT16. (From 0 to 65535)

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            str:
                String to represent the DEC dot format version.
        """
        return NotImplementedError

    @abstractmethod
    def GetHexDotVersion (self) -> str:
        """ Return the firmware version format in HEX dot style.

        Note:
            (1) Represent as MMMM.mmmm.nnnn.RRRR (HEX format)
            (2) Each segment is UINT16. (From 0x0000 to 0xFFFF)

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            str:
                String to represent the HEX dot format version.
        """
        return NotImplementedError

class CapsuleVersionNull (CapsuleVersion):
    def GetDecVersion (self) -> str:
        """ Return the firmware version format in DEC style. (32-bit)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the DEC version.
        """
        return '0'

    def GetHexVersion (self) -> str:
        """ Return the firmware version format in HEX style. (32-bit)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the HEX version.
        """
        return '0'

    def GetDecDotVersion (self) -> str:
        """ Return the firmware version format in DEC dot style.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the DEC dot format version.
        """
        return '0.0.0.0'

    def GetHexDotVersion (self) -> str:
        """ Return the firmware version format in HEX dot style.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                String to represent the HEX dot format version.
        """
        return '0.0.0.0'
