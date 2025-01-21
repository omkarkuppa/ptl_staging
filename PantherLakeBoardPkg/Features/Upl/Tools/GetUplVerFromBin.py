## @file
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
#@par Specification
##

import argparse
from   ctypes import *

__prog__        = 'GetUplVerFromBin.py'
__version__     = '%s Version %s' % (__prog__, '0.1')
__copyright__   = 'Copyright (c) 2023, Intel Corporation. All rights reserved.\n'
__description__ = 'Get UPL Version.\n'

class UPLD_INFO_HEADER(LittleEndianStructure):
  _pack_ = 1
  _fields_ = [
    ('Identifier',           ARRAY(c_char, 4)),
    ('HeaderLength',         c_uint32),
    ('SpecRevision',         c_uint16),
    ('Reserved',             c_uint16),
    ('Revision',             c_uint32),
    ('Attribute',            c_uint32),
    ('Capability',           c_uint32),
    ('ProducerId',           ARRAY(c_char, 16)),
    ('ImageId',              ARRAY(c_char, 16)),
    ]

def GetArgs ():
  #
  # Prepare argument parser
  #
  parser = argparse.ArgumentParser()
  parser.add_argument (
           '-V',
           '-v',
           '--Version',
           action = 'version',
           version = __version__
           )

  parser.add_argument(
           '--UplBinFile',
           required = True,
           dest = 'UplBinFile',
           help = 'Choose your Upl binary file path')

  args = parser.parse_args()
  return args

def GetVersionInformation (FileName):
  VersionList = []
  with open (FileName, "rb") as UplFile:
    UplFileArray   = bytearray (UplFile.read ())
    UplInfoPosition = UplFileArray.find (b'PLDH')
    if (UplInfoPosition != -1):
      UpldInfoHeader = UPLD_INFO_HEADER.from_buffer (UplFileArray[UplInfoPosition:])
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplSpecRevision={}".format (UpldInfoHeader.SpecRevision))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplMajorRevision=0x{}".format ("{:08x}".format (UpldInfoHeader.Revision)[0:2]))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplMinorRevision=0x{}".format ("{:08x}".format (UpldInfoHeader.Revision)[2:4]))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplRevision=0x{}".format ("{:08x}".format (UpldInfoHeader.Revision)[4:6]))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplBuildNumber=0x{}".format ("{:08x}".format (UpldInfoHeader.Revision)[6:8]))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplProducerId=L\"{}\"".format (UpldInfoHeader.ProducerId.decode ("UTF-8")))
      VersionList.append ("gBoardModuleTokenSpaceGuid.PcdUplImageId=L\"{}\"".format (UpldInfoHeader.ImageId.decode ("UTF-8")))

  return VersionList

def main ():
  opts = GetArgs ()
  for VersionString in GetVersionInformation (FileName=opts.UplBinFile):
    print (VersionString)

if __name__ == "__main__":
  exit(main())
