## @file
#  A wrapper to manipulate the bytearray type and read the binary
#  file to bytearray.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2022 Intel Corporation.
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
# @par Specification
##
from .BaseLib import *
from .ByteBufferIo import *

class ByteBuffer (object):
    def __init__ (self, FilePath=None, Permission='rb', ByteArray=None):
        self.__FilePath                   = FilePath
        self.__FileName, self.__Extension = GetFileNameAndExt (self.__FilePath)
        self.__Permission                 = Permission
        self.__FileValid                  = False
        if ByteArray is None:
            self.__FileBuffer = self.__Read ()
        else:
            self.__FileBuffer = ByteArray
        self.__BufferSize     = len (self.__FileBuffer)
        self.__PrintInfo()

    def __PrintInfo (self):
        """ Print the debug related message on terminal.

        Args:
            None.

        Returns:
            None.

        Raises:
            None.
        """
        SizeInKiloByte = self.__BufferSize / KILOBYTE_TO_BYTE
        SizeInMegaByte = self.__BufferSize / MEGABYTE_TO_BYTE
        print (f'Binary file path: {self.__FilePath}')

        if SizeInMegaByte >= 1.0:
            print (f'Binary file size: {SizeInMegaByte} MB')
        elif SizeInKiloByte >= 1.0:
            print (f'Binary file size: {SizeInKiloByte} KB')
        else:
            print (f'Binary file size: {self.__BufferSize} Byte')

    def __Read (self):
        """ Read the file from storage to memory buffer.

        Args:
            None.

        Returns:
            Buffer [bytearray]: The default type of bytearray (without any wrapper).

        Raises:
            FileNotFoundError: If file not found in storage would raise this exception.
        """
        Buffer = bytearray ()
        try:
            FilePointer = open (self.__FilePath, self.__Permission)             # _io.BufferedReader
            Buffer = bytearray (FilePointer.read ())
            FilePointer.close ()
            self.__FileValid = True
        except FileNotFoundError:
            raise FileNotFoundError

        return Buffer

    def GetFilePath (self):
        """ To get the file path of this bytebuffer.

        Args:
            None.

        Returns:
            __FilePath [str]: The file path of this bytebuffer.

        Raises:
            None.
        """
        return self.__FilePath

    def GetFileName (self):
        """ To get the file path of this bytebuffer.

        Args:
            None.

        Returns:
            __FileName [str]: The filename of this file.

        Raises:
            None.
        """
        return self.__FileName

    def GetExtension (self):
        """ To get the file extension of this file.

        Args:
            None.

        Returns:
            __FileName [str]: The file extension of this file.

        Raises:
            None.
        """
        return self.__Extension

    def Get (self):
        """ Fetch the bytearray without any wrapper.

        Args:
            None.

        Returns:
            __FileBuffer [bytearray]: The default type of bytearray (without any wrapper).

        Raises:
            None.
        """
        return self.__FileBuffer

    def GetSize (self):
        """ Get the size of this bytebuffer in byte.

        Args:
            None.

        Returns:
            Size [int]: The size of this bytebuffer.

        Raises:
            None.
        """
        return len (self.__FileBuffer)

    def Find (self, Input):
        """ Find the specific input matching in the bytearray.
            Now support ANSI character in str.
            Others need to input with bytes or bytearray.

        Args:
            Input  [str|bytes|bytearray]: The input data need to be found in the bytearray.

        Returns:
            Result list[int]: The offset start from in this bytearray. Return empty list if not found.

        Raises:
            None.
        """
        Signature = ByteBufferIo (Input).Process ()
        Result    = list()
        Start     = 0
        Location  = 0

        while True:
            Location = self.__FileBuffer.find (Signature, Start)
            Result.append (Location)

            if (Location == -1):
                break
            else:
                Start = Location + len (Signature)

        if len (Result) == 1 and Result[0] == -1:
            Result = []
            print (f'Could not find the signature in buffer.')
        elif len (Result) != 1:
            Result = Result[0:-1]
            print (f'Find the signature in buffer location {Result}')

        return Result

    def Write (self, Offset, Replace):
        """ To overwrite the bytearray start from specific offset.
            If replaced bytearray is over all the source bytearray length, the over size part would not take action.

        Args:
            Offset  [int]                : The offset start to do the replacement action.
            Replace [str|bytes|bytearray]: The data need to be replaced in the bytearray.

        Returns:
            None.

        Raises:
            TypeError: If input offset not int would raise this exception.
        """
        if type (Offset) is not int:
            raise TypeError

        ReplaceByteData = ByteBufferIo (Replace).process ()

        for i in range (len (ReplaceByteData)):
            if Offset+i > self.__BufferSize:
                print (f'Byte 0x%08{Offset+i} replace out of buffer size {self.__BufferSize} would not take action!')
            else:
                self.__FileBuffer[Offset+i] = ReplaceByteData[i]
                print (f'Byte 0x%08{Offset+i} replace finish!')

    def Save (self, Path):
        """ Save this bytebuffer to disk.

        Args:
            Path [str]: The path to store this bytebuffer into disk.

        Returns:
            None.

        Raises:
            None.
        """
        fp = open (Path, "wb")
        fp.write (self.__FileBuffer)
        fp.close ()
