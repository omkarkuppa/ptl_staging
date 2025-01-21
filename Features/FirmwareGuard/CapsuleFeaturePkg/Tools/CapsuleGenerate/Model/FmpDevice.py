## @file
#  Description a FMP device object should expose what methods.
#
#  Note:
#    All return should combine with 'Status' and 'Data'.
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
from abc import ABCMeta, abstractmethod
from typing import Tuple

from CapsuleCommon import *

from CapsuleGenerate.Model.SignCert import *

#
# Define the FMP device function supported state.
#
FMP_SUCCESS    : int = 0x0000_0000
FMP_UNSUPPORTED: int = 0x0000_0001
FMP_UNKNOWN    : int = 0xFFFF_FFFF

#
# Define the macro strings.
#
FMP_NULL_STR_VALUE: str = ''

#
# Default value for FMP device.
#
FMP_VER_DEFAULT_VALUE    : int       = 0x0000_0001
FMP_LSV_DEFAULT_VALUE    : int       = 0x0000_0001
FMP_GUID_DEFAULT_VALUE   : uuid.UUID = \
    uuid.UUID ('00000000-0000-0000-0000-000000000000')
FMP_HW_INST_DEFAULT_VALUE: int       = 0x0000000000000000
FMP_VENDOR_DEFAULT_VALUE : str       = 'Intel'

class FmpDevice (metaclass = ABCMeta):
    @abstractmethod
    def GetSignCert (self) -> Tuple[int, SignCert]:
        """ Return the sign certificate object used to sign the capsule.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, SignCert]:
                int:
                    The result of this function get executed.
                SignCert:
                    Object to represent the sign command.
        """
        return NotImplementedError

    @abstractmethod
    def GetDeviceName (self) -> Tuple[int, str]:
        """ Return the FMP firmware device name.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, str]:
                int:
                    The result of this function get executed.
                str:
                    The FMP firmware device name.
        """
        return NotImplementedError

    @abstractmethod
    def GetCapsuleFileName (self) -> Tuple[int, str]:
        """ Return the capsule filename.

        Note:
            (1) Should not include any extension.
            (2) Caller responsibility to provide the needed extension.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, str]:
                int:
                    The result of this function get executed.
                str:
                    The filename of this capsule file.
        """
        return NotImplementedError

    @abstractmethod
    def GetFwImagePayload (self) -> Tuple[int, ByteBuffer]:
        """ Return the completed Payload buffer.

        Note:
            (1) Return ByteBuffer should not need to modify anymore.
            (2) Any FmpDevice instance should have inner methods to
                do any necessary modify.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, ByteBuffer]:
                int:
                    The result of this function get executed.
                ByteBuffer:
                    ByteBuffer object that used to build the capsule.
        """
        return NotImplementedError

    @abstractmethod
    def GetFwImageVersion (self) -> Tuple[int, int]:
        """ Return the firmware image version in capsule. (32-bit)

        Note:
            Recommend caller to check the value is UINT32.

        Required:
            Yes.

        Args:
            None

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                int:
                    The UINT32 value to represent the firmware version.
        """
        return NotImplementedError

    @abstractmethod
    def GetFwImageLsv (self) -> Tuple[int, int]:
        """ Return the firmware image LSV value within capsule. (32-bit)

        Note:
            (1) Recommend caller to check the value is UINT32.
            (2) This is a method to anti-rollback the firmware.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                int:
                    The UINT32 value to represent the firmware LSV.
        """
        return NotImplementedError

    @abstractmethod
    def GetFwImageGuid (self) -> Tuple[int, uuid.UUID]:
        """ Return the GUID(ImageTypeId) number of firmware image.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, uuid.UUID]:
                int:
                    The result of this function get executed.
                uuid.UUID:
                    The GUID number of this capsule file for specific
                    FMP device.
        """
        return NotImplementedError

    @abstractmethod
    def GetHardwareInstance (self) -> Tuple[int, int]:
        """ Return the capsule hardware instance number. (64-bit)

        Note:
            Recommend caller to check the value is UINT64.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                int:
                    The hardware instance number of this capsule file
                    for specific FMP device. (UINT64)
        """
        return NotImplementedError

    ####################################################################
    #     The following functions are prepared for Windows Capsule     #
    ####################################################################
    @abstractmethod
    def GetVendor (self) -> Tuple[int, str]:
        """ Return the vendor name of this capsule file.

        Note:
            Here should be "Intel".

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                str:
                    The vendor name that provide this capsule file.
        """
        return NotImplementedError

    @abstractmethod
    def GetCapsuleString (self) -> Tuple[int, str]:
        """ Return the firmware image version in dot format.

        Note:
            Format:
                - With 4 segments, Major.Minor.Micro.Build.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                str:
                    The firmware image version in dot format.
        """
        return NotImplementedError

    @abstractmethod
    def GetCapsuleDescription (self) -> Tuple[int, str]:
        """ Return the description of firmware image.

        Required:
            Yes.

        Args:
            None.

        Raises:
            NotImplementedError:
                Object instance not implemented this function.

        Returns:
            Tuple[int, int]:
                int:
                    The result of this function get executed.
                str:
                    The description of firmware image.
        """
        return NotImplementedError

class FmpDeviceNull (FmpDevice):
    def GetSignCert (self) -> Tuple[int, SignCert]:
        """ Return the sign certificate object used to sign the capsule.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[int, SignCert]:
                int:
                    The result of this function get executed.
                SignCert:
                    Object to represent the sign command.
        """
        return FMP_UNSUPPORTED, None

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
        return FMP_UNSUPPORTED, FMP_NULL_STR_VALUE

    def GetCapsuleFileName (self) -> Tuple[int, str]:
        """ Return the capsule filename.

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
        return FMP_UNSUPPORTED, FMP_NULL_STR_VALUE

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
        return FMP_UNSUPPORTED, None

    def GetFwImageVersion (self) -> Tuple[int, int]:
        """ Return the firmware image version in capsule. (32-bit)

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
        return FMP_UNSUPPORTED, FMP_VER_DEFAULT_VALUE

    def GetFwImageLsv (self) -> Tuple[int, int]:
        """ Return the firmware image LSV value within capsule. (32-bit)

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
        return FMP_UNSUPPORTED, FMP_LSV_DEFAULT_VALUE

    def GetFwImageGuid (self) -> Tuple[int, uuid.UUID]:
        """ Return the GUID(ImageTypeId) number of firmware image.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[int, uuid.UUID]:
                int:
                    The result of this function get executed.
                uuid.UUID:
                    The GUID number of this capsule file for specific
                    FMP device.
        """
        return FMP_UNSUPPORTED, FMP_GUID_DEFAULT_VALUE

    def GetHardwareInstance (self) -> Tuple[int, int]:
        """ Return the capsule hardware instance number. (64-bit)

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

    ####################################################################
    #     The following functions are prepared for Windows Capsule     #
    ####################################################################
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
        return FMP_UNSUPPORTED, FMP_VENDOR_DEFAULT_VALUE

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
        return FMP_UNSUPPORTED, FMP_NULL_STR_VALUE

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
        return FMP_UNSUPPORTED, FMP_NULL_STR_VALUE
