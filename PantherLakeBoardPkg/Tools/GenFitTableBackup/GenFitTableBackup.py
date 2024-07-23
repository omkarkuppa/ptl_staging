## @file
#  This script is called to auto generate PostIbbDigest.hash
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

import os
import struct
import sys
import argparse
import ctypes

#
# Globals for help information
#
__prog__        = sys.argv[0]
__copyright__   = 'Copyright (c) 2024, Intel Corporation. All rights reserved.'
__description__ = '\n'

MICROCODE_FIT_ENTRY_TYPE = 0x1
FIT_TABLE_BACKUP_OFFSET  = 0x200
FIT_TABLE_BACKUP_SIZE    = 0x100
FIT_PAYLOAD_REGION_SIZE  = 0x40000

class FIRMWARE_INTERFACE_TABLE_ENTRY (ctypes.Structure):
    _pack_ = 1
    _fields_ = [
        ("Address",  ctypes.c_uint64),
        ("Size",     ctypes.c_uint8 * 3),
        ("Rsvd",     ctypes.c_uint8),
        ("Version",  ctypes.c_uint16),
        ("Type",     ctypes.c_uint8, 7),
        ("C_V",      ctypes.c_uint8, 1),
        ("Checksum", ctypes.c_uint8),
    ]

    def GetSize (self) -> int:
        size = (self.Size[2] << 16) | (self.Size[1] << 8) | self.Size[0]
        return size

    def InitFromBuffer (self, Buffer) -> None:
        size = ctypes.sizeof (self)
        if len (Buffer) < size:
            print ("Input buffer size is smaller than FIRMWARE_INTERFACE_TABLE_ENTRY size")
            exit (1)
        ctypes.memmove (ctypes.addressof(self), Buffer, size)


if __name__ == "__main__":

    def ValidateFvAddress (Argument) -> int:
        try:
            Value = int (Argument, 0)
        except:
            print ('{Argument} is not a valid integer value.'.format (Argument = Argument))
            exit (1)
        return Value

    def CalculateChecksum(buffer, offset) -> bytes:
        checksum = 0
        for i in range(len(buffer)):
            if i != offset:
                checksum += buffer[i]

        checksum = (256 - (checksum % 256)) % 256
        output = buffer[: offset] + bytes ([checksum]) + buffer[offset + 1 :]
        return output

    #
    # Create command line argument parser object
    #
    parser = argparse.ArgumentParser (
                        prog = __prog__,
                        description = __description__ + __copyright__,
                        )

    parser.add_argument ("-i", "--input", dest = 'InputFile', type = argparse.FileType ('rb'), required = True,
                            help = "Input binary filename.")
    parser.add_argument ("-o", "--output", dest = 'OutputFile', type = str, required = True,
                            help = "Output filename")
    parser.add_argument ("-T", "--fixed_fit_location", dest = 'FitLocation', type = ValidateFvAddress, required = True,
                            help = "Fixed Fit table location (hex)")

    #
    # Parse command line arguments
    #
    args = parser.parse_args ()
    print ("FitLocation: ", hex (args.FitLocation))

    #
    # Read all binary input files
    #
    File = args.InputFile
    try:
        Buffer = File.read ()
        File.close ()
    except:
        print ('GenFitTableBackup: error: can not read binary input file {File} !'.format (File = File))
        exit (1)

    #
    # Read Fit table header
    #
    FitHeader = FIRMWARE_INTERFACE_TABLE_ENTRY ()
    FitEntrySize = ctypes.sizeof (FIRMWARE_INTERFACE_TABLE_ENTRY)
    FitHeader.InitFromBuffer (Buffer[args.FitLocation:])

    if FitHeader.Address != struct.unpack('<Q', b'_FIT_   ')[0]:
        print ('FIT table not found!')
        exit (1)
    FitEntryNum = FitHeader.GetSize ()
    FitTableSize = FitEntryNum * FitEntrySize
    if FitTableSize > FIT_TABLE_BACKUP_SIZE:
        print ('FIT table backup space is not enough!')
        exit (1)
    FitTableBuffer = Buffer[args.FitLocation : args.FitLocation + FitTableSize]
    FitTableBackupBuffer = bytes ()

    #
    # Modify Microcode Fit entry address
    #
    for Index in range (FitEntryNum):
        Offset = Index * FitEntrySize
        FitEntry = FIRMWARE_INTERFACE_TABLE_ENTRY ()
        FitEntryBuffer = FitTableBuffer[Offset : Offset + FitEntrySize]
        FitEntry.InitFromBuffer (FitEntryBuffer)
        if FitEntry.Type == MICROCODE_FIT_ENTRY_TYPE:
            FitEntry.Address += 0xC0000
        ctypes.memmove(FitEntryBuffer, ctypes.pointer(FitEntry), ctypes.sizeof(FitEntry))
        FitTableBackupBuffer += FitEntryBuffer

    FitTableBackupBuffer = CalculateChecksum (FitTableBackupBuffer, 0xF)

    with open (args.OutputFile, 'rb+') as F:
        F.seek (args.FitLocation + FIT_TABLE_BACKUP_OFFSET)
        F.write (FitTableBuffer)
        F.seek (args.FitLocation + FIT_TABLE_BACKUP_OFFSET + FIT_TABLE_BACKUP_SIZE)
        F.write (FitTableBackupBuffer)
