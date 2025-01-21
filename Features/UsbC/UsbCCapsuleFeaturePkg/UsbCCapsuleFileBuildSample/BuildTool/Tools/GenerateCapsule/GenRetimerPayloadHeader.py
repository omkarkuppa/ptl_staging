# @file
#  Generate Thunderbolt Retimer firmware payload header.
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
# Data structures used in Retimer image payload
#
# #define RETIMER_PAYLOAD_HEADER_SIGNATURE SIGNATURE_32 ('R', 'T', 'M', 'R')

# #pragma pack(1)
# typedef struct {
#   UINT32  Signature;
#   UINT32  HeaderSize;
#   UINT32  RetimerCount;
#   UINT32  Reserved;
#   ///
#   /// Variable length array of dimension [EmbeddedDriverCount + PayloadItemCount]
#   /// containing offsets of each of the drivers and payload items contained within the capsule
#   ///
#   // RETIMER_ITEM RetimerItem[];
# } RETIMER_PAYLOAD_HEADER;
#
# typedef struct {
#   RETIMER_DEV_ADDRESS      RetimerDevAddress;
#   UINT32                   ImageOffset;
#   UINT32                   ImageSize;
#   UINT8                    FirmwareType;      // 0:Integrated Tbt retimer, 1:Discrete TBT retimer, Others: Reserved
#   UINT8                    PcieRpType;        // 2:PCH 4:CPU
#   UINT8                    PcieRootPort;
#   UINT8                    Reserve[5];
# } RETIMER_ITEM;
#
# typedef struct {
#   UINT16  Bus;
#   UINT16  Device;
#   UINT16  Function;
#   UINT8   Port;           // DMA retimer
#   UINT8   CascadedRetimerIndex;
#   UINT8   Reserved[8];
# } RETIMER_DEV_ADDRESS;
#
# #pragma pack()
#

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I',bytearray (A + B + C + D, 'ascii'))[0]

HeaderSignature = _SIGNATURE_32 ('R', 'T', 'M', 'R')
HeaderStruct    = '<IIII'
ItemStruct      = '<HHHBB8sIIBBB5s'

def gen_retimer_header_from_ini (inifile):

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
  RetimerFwBuffer = b''

  for Section in config.sections():
    BusNum     = config.getint(Section, 'BusNumber')
    DevNum     = config.getint(Section, 'DevNumber')
    FunNum     = config.getint(Section, 'FunNumber')
    Port       = config.getint(Section, 'PortNumber')
    Index      = config.getint(Section, 'CascadedRetimerIndex')
    FwType     = config.getint(Section, 'FirmwareType')
    PcieRpType = config.getint(Section, 'PcieRpType')
    PcieRpNum  = config.getint(Section, 'PcieRpNumber')
    File       = config.get(Section, 'FileName')

    RetimerFwFile = os.path.join(IniDir,File)
    try:
      fin = open(RetimerFwFile, "rb")
      ImageSize = os.path.getsize(RetimerFwFile)
      # Read file to buffer
      RetimerFw = fin.read()
      fin.close()

    except:
      print ("Failed to open %s" % RetimerFwFile)
      sys.exit (1)

    RetimerItemHeader = struct.pack (ItemStruct, BusNum, DevNum, FunNum, Port, Index, B'', ImageOffset, ImageSize, FwType, PcieRpType, PcieRpNum, B'')
    ImageOffset = ImageOffset + ImageSize
    ItemHeaderBuffer = ItemHeaderBuffer + RetimerItemHeader
    RetimerFwBuffer = RetimerFwBuffer + RetimerFw

  RetimerPayloadHeader = struct.pack (HeaderStruct, HeaderSignature, HeaderSize, UpdateNum, 0)
  Result = RetimerPayloadHeader + ItemHeaderBuffer + RetimerFwBuffer

  OutputFileName  = "ITbtRetimerPayload.bin"
  if (FwType == 1):
    OutputFileName = "DTbtRetimerPayload.bin"

  # Write output
  fout = open(os.path.join(IniDir,OutputFileName), "wb")
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

  ret = gen_retimer_header_from_ini(sys.argv[1])

  if ret != 0:
    return 1
  return 0

if __name__ == '__main__':
    sys.exit(main())
