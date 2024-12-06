# @file
#  Generate Discrete Thunderbolt firmware payload header.
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
# @par Specification Reference:
#
import sys
import os
import fileinput
import array
import struct
if sys.version_info.major == 2:
  import ConfigParser
else:
  import configparser

#
#
# Data structures used in dTBT image payload
#
# #define DISCRETE_TBT_PAYLOAD_HEADER_SIGNATURE SIGNATURE_32 ('D', 'T', 'B', 'T')

# #pragma pack(1)
# typedef struct {
#   UINT32  Signature;
#   UINT32  HeaderSize;
#   UINT32  DiscreteTbtCount;
#   UINT32  Reserved;
#   ///
#   /// Variable length array of dimension [EmbeddedDriverCount + PayloadItemCount]
#   /// containing offsets of each of the drivers and payload items contained within the capsule
#   ///
#   // DISCRETE_TBT_ITEM DiscreteTbtItem[];
# } DISCRETE_TBT_PAYLOAD_HEADER;
#
# typedef struct {
#   UINT32                   ImageOffset;
#   UINT32                   ImageSize;
#   UINT8                    FirmwareType;      // 2:Discrete TBT, Others: Reserved
#   UINT8                    PcieRpType;        // 2:PCH 4:CPU
#   UINT8                    PcieRootPort;
#   UINT8                    Reserve[5];
# } DISCRETE_TBT_ITEM;
#
# #pragma pack()
#

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I',bytearray (A + B + C + D, 'ascii'))[0]

HeaderSignature = _SIGNATURE_32 ('D', 'T', 'B', 'T')
HeaderStruct    = '<IIII'
ItemStruct      = '<IIBBB5s'

def gen_discrete_tbt_header_from_ini (inifile):

  IniDir = os.path.dirname(inifile)
  if sys.version_info.major == 2:
    config = ConfigParser.ConfigParser()
  else:
    config = configparser.ConfigParser(inline_comment_prefixes=(';',))
  config.read(inifile)
  UpdateNum = len(config.sections())
  print ("UpdateNum is %d" % UpdateNum)

  if (UpdateNum == 0):
    print ("No sections found")
    sys.exit (1)

  HeaderSize = struct.calcsize (HeaderStruct) + (UpdateNum * struct.calcsize (ItemStruct))
  ImageOffset = HeaderSize
  ItemHeaderBuffer = b''
  DiscreteTbtFwBuffer = b''

  for Section in config.sections():
    FwType     = config.getint(Section, 'FirmwareType')
    PcieRpType = config.getint(Section, 'PcieRpType')
    PcieRpNum  = config.getint(Section, 'PcieRpNumber')
    File       = config.get(Section, 'FileName')

    DiscreteTbtFwFile = os.path.join(IniDir,File)
    try:
      fin = open(DiscreteTbtFwFile, "rb")
      ImageSize = os.path.getsize(DiscreteTbtFwFile)
      # Read file to buffer
      DiscreteTbtFw = fin.read()
      fin.close()

    except:
      print ("Failed to open %s" % DiscreteTbtFwFile)
      sys.exit (1)

    DiscreteTbtItemHeader = struct.pack (ItemStruct, ImageOffset, ImageSize, FwType, PcieRpType, PcieRpNum, B'')
    ImageOffset = ImageOffset + ImageSize
    ItemHeaderBuffer = ItemHeaderBuffer + DiscreteTbtItemHeader
    DiscreteTbtFwBuffer = DiscreteTbtFwBuffer + DiscreteTbtFw

  DiscreteTbtPayloadHeader = struct.pack (HeaderStruct, HeaderSignature, HeaderSize, UpdateNum, 0)
  Result = DiscreteTbtPayloadHeader + ItemHeaderBuffer + DiscreteTbtFwBuffer

  # Write output
  fout = open(os.path.join(IniDir,"DTbtPayload.bin"), "wb")
  fout.write(Result)
  fout.close()

  return 0

###################################################################################################
# Main
###################################################################################################

def main():
  if len(sys.argv) != 2:
    print ("incorrent number of arguments")
    return 1

  ret = gen_discrete_tbt_header_from_ini(sys.argv[1])

  if ret != 0:
    return 1
  return 0

if __name__ == '__main__':
    sys.exit(main())
