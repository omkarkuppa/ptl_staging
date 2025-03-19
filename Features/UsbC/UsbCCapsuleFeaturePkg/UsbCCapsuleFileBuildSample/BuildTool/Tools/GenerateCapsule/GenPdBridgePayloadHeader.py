# @file
#  Generate PD Bridge firmware payload header.
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
# Data structures used in PD Bridge image payload
#
# #define PD_BRIDGE_PAYLOAD_HEADER_SIGNATURE SIGNATURE_32 ('P', 'D', 'B', 'G')

# #pragma pack(1)
# typedef struct {
#   UINT32  Signature;
#   UINT32  HeaderSize;
#   UINT32  PayloadCount;
#   UINT32  Reserved;
#   ///
#   /// Variable length array of dimension [EmbeddedDriverCount + PayloadItemCount]
#   /// containing offsets of each of the drivers and payload items contained within the capsule
#   ///
#   /// PAYLOAD_ITEM PayloadItem[];
# } PAYLOAD_HEADER;
#
# typedef union {
#   struct {
#     UINT8  PdCntrlIndex;
#     UINT8  ShareFlashMode;  // 0: Disable, 1:Enabled
#     UINT8  Reserved[14];
#   } PdBridge;  // FirmwareType 3: PdBridge

#   UINT8 Data8[16];

# } FIRMWARE_PRIVATE_DATA;

# typedef struct {
#   FIRMWARE_PRIVATE_DATA  PrivateData;
#   UINT32                 ImageOffset;
#   UINT32                 ImageSize;
#   UINT8                  FirmwareType;   // 3: PD Bridge
#   UINT8                  Reserve[3];
# } PAYLOAD_ITEM;
#
# #pragma pack()
#

def _SIGNATURE_32 (A, B, C, D):
    return struct.unpack ('=I',bytearray (A + B + C + D, 'ascii'))[0]

HeaderSignature = _SIGNATURE_32 ('P', 'D', 'B', 'G')
HeaderStruct    = '<IIII'
ItemStruct      = '<BBB13sIIB3s'

def gen_pd_bridge_header_from_ini (inifile):

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
  PdBridgeFwBuffer = b''

  for Section in config.sections():
    FwType          = config.getint(Section, 'FirmwareType')
    PdCntrlIndex    = config.getint(Section, 'PdCntrlIndex')
    ShareFlashMode  = config.getint(Section, 'ShareFlashMode')
    PdBridgeType    = config.getint(Section, 'PdBridgeType')
    File            = config.get(Section, 'FileName')

    PdBridgeFwFile = os.path.join(IniDir,File)
    try:
      fin = open(PdBridgeFwFile, "rb")
      ImageSize = os.path.getsize(PdBridgeFwFile)
      # Read file to buffer
      PdBridgeFw = fin.read()
      fin.close()

    except:
      print ("Failed to open %s" % PdBridgeFwFile)
      sys.exit (1)

    PdBridgeItemHeader = struct.pack (ItemStruct, PdCntrlIndex, ShareFlashMode, PdBridgeType, B'', ImageOffset, ImageSize, FwType, B'')
    ImageOffset = ImageOffset + ImageSize
    ItemHeaderBuffer = ItemHeaderBuffer + PdBridgeItemHeader
    PdBridgeFwBuffer = PdBridgeFwBuffer + PdBridgeFw

  PdBridgePayloadHeader = struct.pack (HeaderStruct, HeaderSignature, HeaderSize, UpdateNum, 0)
  Result = PdBridgePayloadHeader + ItemHeaderBuffer + PdBridgeFwBuffer

  # Write output
  fout = open(os.path.join(IniDir,"PdBridgePayload.bin"), "wb")
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

  ret = gen_pd_bridge_header_from_ini(sys.argv[1])

  if ret != 0:
    return 1
  return 0

if __name__ == '__main__':
    sys.exit(main())
