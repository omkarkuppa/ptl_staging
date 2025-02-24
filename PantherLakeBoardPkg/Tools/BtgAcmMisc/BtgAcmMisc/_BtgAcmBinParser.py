## @file
#  API to parse the BtGACM FFS from input binary.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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
#  @par Specification Reference:
#
##

import copy
from typing import List, Union

from BtgAcmMisc.Base._ByteBufferWrapper import *
from BtgAcmMisc.BtgAcm._AcmHdrParser import *
from BtgAcmMisc.BtgAcm._AcmInfoTblParser import *
from BtgAcmMisc.BtgAcm._AcmProcessorIdListParser import *

class BtgAcmBinInfo (object):
    def __init__ (
        self,
        Offset: int,
        Size  : int,
        Buffer: ByteBuffer,
        ) -> None:
        """ Class to parse the BtGACM binary information.

        Args:
            Offset (int):
                Offset of this buffer in source BtGACM FFS.
            Size (int):
                Size of this buffer in source BtGACM FFS.
            Buffer (ByteBuffer):
                BtGACM binary buffer to be decoded.

        Raises:
            None.

        Returns:
            None.
        """
        self.__Offset: int        = Offset
        self.__Size  : int        = Size
        self.__Buffer: ByteBuffer = Buffer

        self.__PreCheck ()

        self.__Header         : AcmHdrParser             = None
        self.__InfoTable      : AcmInfoTblParser         = None
        self.__ProcessorIdList: AcmProcessorIdListParser = None
        self.__IsValid        : bool                     = self.__Parse ()

    @property
    def Valid (self) -> bool:
        """ Return the validity of the BtGACM binary info.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - BtGACM binary info is valid.
                False - BtGACM binary info is not valid.
        """
        return self.__IsValid

    @property
    def Offset (self) -> int:
        """ Return the offset of this BtGACM binary buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Offset of this BtGACM binary buffer.
        """
        return self.__Offset

    @property
    def Size (self) -> int:
        """ Return the size of this BtGACM binary buffer.

        Args:
            None.

        Raises:
            None.

        Returns:
            int:
                Size of this BtGACM binary binary buffer.
        """
        return self.__Size

    @property
    def Header (self) -> Union[None, AcmHdrParser]:
        """ Return the header of ACM.

        Args:
            None

        Raises:
            None.

        Returns:
            Union[None, List[AcmHdrParser]]:
                Header of ACM. (AcmHdrParser type.)
                If BtGACM binary info is not valid, return as None.
        """
        if not self.Valid:
            return None

        return self.__Header

    @property
    def ProcessorIdList (self) -> Union[None, List[AcmProcessorId]]:
        """ Return the list of processor ID.

        Args:
            None

        Raises:
            None.

        Returns:
            Union[None, List[AcmProcessorId]]:
                List of ACM processor ID. (AcmProcessorId type.)
                If ACM binary information is not valid, return as None.
        """
        if not self.Valid:
            return None

        return self.__ProcessorIdList.Data

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) Offset is not int type.
                (2) Size is not int type.
                (3) Buffer is not ByteBuffer type.
            ValueError:
                (1) Offset is not positive value.
                (2) Size is not positive value.

        Returns:
            None.
        """
        if not isinstance (self.__Offset, int):
            raise TypeError ('Offset shall be int type.')
        elif not isinstance (self.__Size, int):
            raise TypeError ('Size shall be int type.')
        elif not isinstance (self.__Buffer, ByteBuffer):
            raise TypeError ('Buffer shall be ByteBuffer type.')

        if self.__Offset < 0:
            raise ValueError ('Offset shall be positive value.')
        elif self.__Size < 0:
            raise ValueError ('Size shall be positive value.')

    def __GetAcmHdr (self) -> Tuple[bool, Union[None, AcmHdrParser]]:
        """ Decode the ACM header.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[bool, Union[None, AcmHdrParser]]:
                Parser for the ACM header. (AcmHdrParser type.)
                If ACM header is not valid, return as None.
        """
        IsValid: bool         = False
        Header : AcmHdrParser = AcmHdrParser (self.__Buffer)

        IsValid = Header.Valid

        if not IsValid:
            Header = None

        return IsValid, Header

    def __GetAcmInfoTbl (self) -> Tuple[bool, Union[None, AcmInfoTblParser]]:
        """ Get the ACM info table parser.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[bool, Union[None, AcmInfoTblParser]]:
                Parser for the ACM info table. (AcmInfoTblParser type.)
                If ACM info table is not valid, return as None.
        """
        IsValid: bool             = False
        InfoTbl: AcmInfoTblParser = AcmInfoTblParser (self.__Buffer)

        IsValid = InfoTbl.Valid

        if not IsValid:
            InfoTbl = None

        return IsValid, InfoTbl

    def __GetProcessorIdList (
        self,
        Offset: int,
        ) -> Tuple[bool, Union[None, AcmProcessorIdListParser]]:
        """ Get the ACM processor ID list parser.

        Args:
            None.

        Raises:
            None.

        Returns:
            Tuple[bool, Union[None, AcmProcessorIdListParser]]:
                Parser for the ACM processor ID list.
                (AcmProcessorIdListParser type.)
                If ACM processor ID list is not valid, return as None.
        """
        IsValid        : bool                     = False
        ProcessorIdList: AcmProcessorIdListParser = \
            AcmProcessorIdListParser (self.__Buffer, Offset)

        IsValid = ProcessorIdList.Valid

        if not IsValid:
            ProcessorIdList = None

        return IsValid, ProcessorIdList

    def __Parse (self) -> bool:
        """ Internal wrapper function to parse the BtGACM binary.

        Args:
            None.

        Raises:
            None.

        Returns:
            bool:
                True  - Succeed to decode the BtGACM binary.
                False - Failed to decode the BtGACM binary.
        """
        IsValid        : bool                     = None
        Header         : AcmHdrParser             = None
        InfoTable      : AcmInfoTblParser         = None
        ProcessorIdList: AcmProcessorIdListParser = None

        #
        # Decode the ACM header.
        #
        IsValid, Header = self.__GetAcmHdr ()
        if not IsValid:
            return False

        #
        # Decode the ACM information table.
        #
        IsValid, InfoTable = self.__GetAcmInfoTbl ()
        if not IsValid:
            return False

        #
        # Decode the ACM processor ID list.
        #
        IsValid, ProcessorIdList = \
            self.__GetProcessorIdList (InfoTable.ProcessorIdListOffset)
        if not IsValid:
            return False

        self.__Header          = Header
        self.__InfoTable       = InfoTable
        self.__ProcessorIdList = ProcessorIdList

        return True

class BtgAcmBinParser (object):
    def __init__ (
        self,
        DataBuffer: ByteBuffer,
        SlotSize  : int,
        ) -> None:
        """ Class to parse BtGACM binary information from BtGACM FFS.

        Args:
            DataBuffer (ByteBuffer):
                Data buffer containing list of the binary BtGACM.
            SlotSize (int):
                Slot size of each BtGACM binary file.

        Raises:
            None.

        Returns:
            None.
        """
        self.__DataBuffer: ByteBuffer = DataBuffer
        self.__SlotSize  : int        = SlotSize

        self.__PreCheck ()

        self.__SlotInfoList: List[Tuple[int, int]] = self.__GetSlotInfoList ()
        self.__BinInfoList : List[BtgAcmBinInfo]   = self.__GetBinInfoList ()

    @property
    def BinInfoList (self) -> List[BtgAcmBinInfo]:
        """ Return the list of the BtGACM binary info in this FFS.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[BtgAcmBinInfo]:
                List of the BtGACM binary info in this FFS.
                (BtgAcmBinInfo type.)
        """
        return self.__BinInfoList

    def __PreCheck (self) -> None:
        """ Check the input argument is valid.

        Args:
            None.

        Raises:
            TypeError:
                (1) DataBuffer is not ByteBuffer type.
                (2) SlotSize is not int type.

        Returns:
            None.
        """
        if not isinstance (self.__DataBuffer, ByteBuffer):
            raise TypeError ('DataBuffer shall be ByteBuffer type.')
        elif not isinstance (self.__SlotSize, int):
            raise TypeError ('SlotSize shall be int type.')

    def __GetSlotInfoList (self) -> List[Tuple[int, int]]:
        """ Get BtGACM binary offset and size information list.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[Tuple[int, int]]:
                List of tuple with BtGACM binary offset and size info.
        """
        OffsetRange : range                 = None
        Offset      : int                   = None
        AcmHdr      : AcmHdrParser          = None
        SlotInfoList: List[Tuple[int, int]] = list ()

        OffsetRange = range (0, self.__DataBuffer.Size, self.__SlotSize)

        for Offset in OffsetRange:
            BinBuffer = copy.deepcopy (self.__DataBuffer)
            BinBuffer.Trim (Offset = Offset, Size = BinBuffer.Size - Offset)

            #
            # Offset 0 of each slot must be the ACM header.
            #
            AcmHdr = AcmHdrParser (BinBuffer)

            if AcmHdr.Valid:
                SlotInfoList.append ((Offset, AcmHdr.Size))

        return SlotInfoList

    def __GetBinInfoList (self) -> List[BtgAcmBinInfo]:
        """ Decode the BtGACM binary based on the slot information.

        Args:
            None.

        Raises:
            None.

        Returns:
            List[BtgAcmBinInfo]:
                List of decoded BtGACM binary information.
        """
        BinBuffer  : ByteBuffer          = None
        BinInfo    : BtgAcmBinInfo       = None
        BinInfoList: List[BtgAcmBinInfo] = list ()
        Offset     : int                 = None
        Size       : int                 = None

        for Offset, Size in self.__SlotInfoList:
            BinBuffer = copy.deepcopy (self.__DataBuffer)
            BinBuffer.Trim (Offset = Offset, Size = Size)

            BinInfo = BtgAcmBinInfo (
                        Offset = Offset,
                        Size   = Size,
                        Buffer = BinBuffer,
                        )

            if BinInfo.Valid:
                BinInfoList.append (BinInfo)

        return BinInfoList

#
# Expose variables / methods / objects.
#
__all__: List[str] = [
    'BtgAcmBinInfo',
    'BtgAcmBinParser',
    ]
