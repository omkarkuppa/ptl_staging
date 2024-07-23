## @file
#  Library for the object used on flash image and BGUP region.
#
#  Note:
#    Object is used to provide the necessary info for specific region.
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

from CapsuleCommon import *

class ImageRegion (object):
    def __init__ (
        self,
        Region  : str,
        FileName: str,
        Path    : str,
        Offset  : int,
        Size    : int,
        Buffer  : ByteBuffer,
        ) -> None:
        """ Class to represent the flash region.

        Args:
            Region (str):
                The name of this region.
            FileName (str):
                The file name of this region store in storage.
            Path (str):
                The path of this region store in storage.
            Offset (int):
                The offset of this region within the source image.
            Size (int):
                The size of this region within the source image.
            Buffer (ByteBuffer):
                The ByteBuffer of this region in memory.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Region  : str        = Region
        self.__FileName: str        = FileName
        self.__Path    : str        = AbsPath (Path)
        self.__Offset  : int        = Offset
        self.__Size    : int        = Size
        self.__Buffer  : ByteBuffer = Buffer

        self.__PreCheck ()

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Region not str type.
                (2) FileName not str type.
                (3) Offset not int type.
                (4) Size not int type.
                (5) Buffer not ByteBuffer type.
            ValueError:
                (1) Offset not UINT64.
                (2) Size not UINT64.
                (3) Offset+Size not UINT64.
                (4) Flash size and image size is not matched.

        Returns:
            None.
        """
        if not isinstance (self.__Region, str):
            raise TypeError ('Region should be str type.')
        elif not isinstance (self.__FileName, str):
            raise TypeError ('FileName should be str type.')
        elif not isinstance (self.__Offset, int):
            raise TypeError ('Offset should be int type.')
        elif not isinstance (self.__Size, int):
            raise TypeError ('Size should be int type.')
        elif not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer should be ByteBuffer type.')

        if not IsUint64 (self.__Offset):
            raise ValueError (f'Offset should be UINT64 [{self.__Offset}]')
        elif not IsUint64 (self.__Size):
            raise ValueError (f'Size should be UINT64 [{self.__Size}]')
        elif not IsUint64 (self.__Offset + self.__Size):
            raise ValueError (f'Offset+Size should be UINT64.')

        if self.__Buffer.Size != self.__Size:
            raise ValueError (
                    f'Flash size [{self.__Size}] and '
                    f'image size [{self.__Buffer.Size}] is not matched.'
                    )

    @property
    def Region (self) -> str:
        """ Return name of the region. (Should be unique)

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Name of the region.
        """
        return self.__Region

    @property
    def FileName (self) -> str:
        """ Return the file name of this region. (Should be unique)

        Note:
            This is used to split the region from one image and save it
            into the storage.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                File name of this region store in storage.
        """
        return self.__FileName

    @property
    def Path (self) -> str:
        """ Return path of this region store in storage.

        Args:
            None.

        Raises:
            None.

        Returns:
            str:
                Path of this region store in storage.
        """
        return self.__Path

    @property
    def Offset (self) -> int:
        """ Return the offset of this region within the source image.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Offset of this region within the source image.
        """
        return self.__Offset

    @property
    def Size (self) -> int:
        """ Return the size of this region within the source image.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Size of this region within the source image.
        """
        return self.__Size

    @property
    def Buffer (self) -> ByteBuffer:
        """ Return the ByteBuffer of this region in memory.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                ByteBuffer of this region in memory.
        """
        return self.__Buffer

class BgupRegion (object):
    def __init__ (
        self,
        Region  : str,
        FileName: str,
        Path    : str,
        Buffer  : ByteBuffer,
        ) -> None:
        """ Class to represent the BGUP region.

        Args:
            Region (str):
                The name of this BGUP region.
            FileName (str):
                The file name of this BGUP region store in storage.
            Path (str):
                The path of this BGUP region store in storage.
            Buffer (ByteBuffer):
                The ByteBuffer of this BGUP region in memory.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Region  : str        = Region
        self.__FileName: str        = FileName
        self.__Path    : str        = AbsPath (Path)
        self.__Buffer  : ByteBuffer = Buffer

        self.__PreCheck ()

        self.__Size  : int = Buffer.Size
        #
        # BGUP is standalone for each flash region.
        # Can not know the actual Offset within full BGUP region.
        # It need to be assigned later.
        #
        self.__Offset: int = None

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Region not str type.
                (2) FileName not str type.
                (3) Buffer not ByteBuffer type.
            ValueError:
                Buffer size not UINT64.

        Returns:
            None.
        """
        if not isinstance (self.__Region, str):
            raise TypeError ('Region should be str type.')
        elif not isinstance (self.__FileName, str):
            raise TypeError ('FileName should be str type.')
        elif not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer should be ByteBuffer type.')

        if not IsUint64 (self.__Buffer.Size):
            raise ValueError (
                    f'Buffer size should be UINT64. [{self.__Buffer.Size}]'
                    )

    @property
    def Image (self) -> ByteBuffer:
        """ Return of this BGUP region image store within the memory.

        Args:
            None.

        Raises:
            None.

        Returns:
            ByteBuffer:
                BGUP region image store within the memory.
        """
        return self.__Buffer

    @property
    def Size (self) -> int:
        """ Return the size of this BGUP image.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Size of this BGUP image.
        """
        return self.__Size

    @property
    def Offset (self) -> Union[None, int]:
        """ Return the offset of this BGUP region within full BGUP.

        Args:
            None.

        Raises:
            None.

        Returns:
            Union[None, int]:
                Offset of this BGUP region within one BGUP image.
                (If not set this value, return None)
        """
        if self.__Offset is None:
            DEBUG (
                DEBUG_WARN, f'{self.__Region} Offset for BGUP image '
                'is not assigned.'
                )

        return self.__Offset

    @Offset.setter
    def Offset (self, Offset: int) -> None:
        """ Set the BGUP offset within one BGUP image.

        Args:
            Offset (int):
                The offset of this BGUP region within one BGUP image.

        Raises:
            TypeError:
                Offset not int type.
            ValueError:
                (1) Offset not UINT64.
                (2) (Offset+Size) not UINT64.

        Returns:
            None.
        """
        if not isinstance (Offset, int):
            raise TypeError (f'Offset should be int type.')
        elif not IsUint64 (Offset):
            raise ValueError (f'Offset should be UINT64. [{Offset}]')
        elif not IsUint64 (Offset + self.__Size):
            raise ValueError (
                    f'Offset+Size should be UINT64. [{Offset + self.__Size}]'
                    )

        self.__Offset = Offset

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'ImageRegion',
    'BgupRegion',
    ]
