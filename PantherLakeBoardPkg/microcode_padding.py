## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2019 Intel Corporation.
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
#
# This script is called to auto generate MicrocodeVersion.data
# parsing Boardpkg.fdf to build slot mode microcode array
#
# FwVersion, Lsv, FwVersionString, SlotSize are inputed by Prep.bat
# or passed from NewGenCap.py script when generating microcode capsule
#
import os
import struct
import re
import sys
import argparse

#
# Globals for help information
#
__prog__        = sys.argv[0]
__copyright__   = 'Copyright (c) 2019 -2021 , Intel Corporation. All rights reserved.'
__description__ = '\n'

def DoAddPaddingData(File, SlotSize):
  with open(File, "rb") as fd:
    Binary = fd.read()
    if len(Binary) > SlotSize:
      print (("Microcode patch %s is larger than slot size(%d)\n") % (File, SlotSize))
      assert (False)
    elif len(Binary) < SlotSize:
      FileBackUp = File.replace(os.path.splitext(File)[1], "_backup" + os.path.splitext(File)[1])
      with open(FileBackUp, "wb") as backup:
        backup.write(Binary)
      Binary = Binary + b'\xff' * (SlotSize - len(Binary))

  with open(File, "wb") as fd:
    fd.write(Binary)

def GetRealFilePath(Path):
  if len(Path.split()) > 1:
    FilePath = Path.split()[-1]
  else:
    FilePath = Path
  index = 0
  while True:
    index1 = FilePath.find("$(", index)
    if index1 == -1:
      break
    index2 = FilePath.find(")",index1)
    FilePath = FilePath.replace(FilePath[index1:index2+1], os.environ[FilePath[index1+2:index2]])
    index = index2 + 1
  return FilePath

def GenMicrocodeVersionRawData(FwVersion, Lsv, VersionString, Output):
  RawData = b''
  RawData = RawData + struct.pack('<I', FwVersion)
  RawData = RawData + struct.pack('<I', Lsv)
  RawData = RawData + VersionString.encode('utf-16-le')
  RawData = RawData + b'\0\0'

  fd = open(Output, "wb")
  fd.write(RawData)
  fd.close()

def GetActivePlatformDsc():
  with open(os.path.join (os.environ["WORKSPACE"], "Conf", "target.txt"), "r") as fd:
    for line in fd.readlines():
      line = line.strip("\n").strip()
      if len(line) != 0 and len(line.split("=")) == 2 and line.split("=")[0].strip() == "ACTIVE_PLATFORM":
        return line.split("=")[1].strip().strip("\n")
  print ("Find ACTIVE_PLATFORM from Conf\\target.txt fail!\n")
  assert(False)

def GetFdfFromDsc(File):
  with open(File, "r") as fd:
    for line in fd.readlines():
      if line.find("FLASH_DEFINITION") != -1:
        return line.split("=")[1].strip().strip("\n")
  print ("Find FLASH_DEFINITION from % fail!\n" % (File))
  assert(False)

def GetMicrocodeFvSectionFromFdf(Fdf):
  with open(Fdf) as fd:
    data = fd.read()

  #
  # Find FILE RAW section according 197DB236-F856-4924-90F8-CDF12FB875F3 GUID
  # Assumption: just one ffs for microcode
  #
  result = re.findall(r"FILE *RAW *= *gIntelMicrocodeArrayFfsBinGuid", data)
  if result == []:
    result = re.findall(r"FILE *RAW *= *197DB236-F856-4924-90F8-CDF12FB875F3", data)

  assert(len(result) == 1)

  #
  # index1, start of ffs section
  # index2, end of ffs section
  #
  index1 = data.find(result[0])
  index2 = -1
  walker = index1
  left_brace_count = 0

  #
  # We are trying to find the whole ffs section
  # make sure the count of '{' is equal to the count of '}'
  #
  while True:
    if data[walker] == "{":
      left_brace_count = left_brace_count + 1
    elif data[walker] == "}":
      left_brace_count = left_brace_count - 1
      if left_brace_count == 0:
        index2 = walker + 1
        break
    walker = walker + 1

  assert (index2 != -1)
  return data[index1:index2].split("\n")

if __name__ == "__main__":
  def Validate32BitInteger (Argument):
    try:
      Value = int (Argument, 0)
    except:
      Message = '{Argument} is not a valid integer value.'.format (Argument = Argument)
      raise argparse.ArgumentTypeError (Message)
    if Value < 0:
      Message = '{Argument} is a negative value.'.format (Argument = Argument)
      raise argparse.ArgumentTypeError (Message)
    if Value > 0xffffffff:
      Message = '{Argument} is larger than 32-bits.'.format (Argument = Argument)
      raise argparse.ArgumentTypeError (Message)
    return Value

  def ValidateSlotSize (Argument):
    try:
      Value = int (Argument, 0)
    except:
      Message = '{Argument} is not a valid integer value.'.format (Argument = Argument)
      raise argparse.ArgumentTypeError (Message)
    if Value % (4096) != 0:
      Message = '{Argument} must be a multiple of 4KB'.format (Argument = Argument)
      raise argparse.ArgumentTypeError (Message)
    return Value

  def ValidateFdf (Argument):
    if os.path.isfile(Argument):
      return Argument
    if os.path.isfile(os.path.join(os.getcwd(), Argument)):
      return os.path.join(os.getcwd(), Argument)
    Message = "[" + Argument + "] is an invalid file path"
    raise argparse.ArgumentTypeError (Message)

  #
  # Create command line argument parser object
  #
  parser = argparse.ArgumentParser (
                      prog = __prog__,
                      description = __description__ + __copyright__,
                      )

  parser.add_argument ("--fw-version", dest = 'FwVersion', type = Validate32BitInteger,
                       help = 'The 32-bit version of the binary payload (e.g. 0x11223344 or 5678).')
  parser.add_argument ("--lsv", dest = 'LowestSupportedVersion', type = Validate32BitInteger,
                       help = 'The 32-bit lowest supported version of the binary payload (e.g. 0x11223344 or 5678).')
  parser.add_argument ("--fw-version-string", dest = 'FwVersionString',
                       help = 'The version string of the binary payload (e.g. "Version 0.1.2.3").')
  parser.add_argument ("--slotsize", dest = 'SlotSize', type = ValidateSlotSize,
                       help = 'The 32-bit slot size must be 4KB alignment!')
  parser.add_argument ("--fdf", dest = 'Fdf', type = ValidateFdf,
                       help = 'Please input .fdf for build MicrocodeFv')
  parser.add_argument ("--opt", dest = 'Opt', choices = ["padding", "revert"], required = True, help = 'Operation Type')

  args, remaining = parser.parse_known_args ()

  FwVersion = int(os.environ["FW_VERSION"],16)
  LowestSupportedVersion = int(os.environ["LSV"],16)
  FwVersionString = os.environ["FW_VERSION_STRING"].strip('"')
  SlotSize = int(os.environ["SLOT_SIZE"],16)
  Fdf = os.path.basename(GetRealFilePath(GetFdfFromDsc(GetActivePlatformDsc())))

  if args.FwVersion != None:
    FwVersion = args.FwVersion
  if args.LowestSupportedVersion != None:
    LowestSupportedVersion = args.LowestSupportedVersion
  if args.FwVersionString != None:
    FwVersionString = args.FwVersionString
  if args.SlotSize != None:
    SlotSize = args.SlotSize
  if args.Fdf != None:
    Fdf = args.Fdf

  for line in GetMicrocodeFvSectionFromFdf(Fdf):
    line = line.strip("\n").strip()

    #
    # Trim comments in one line.
    #
    if line.find("#") != -1:
      index1 = line.find("#")
      index2 = len(line)
      line = line.replace(line[index1:index2],"")

    if len(line) == 0:
      continue

    line = line.strip("\n").strip()
    if line[-4:] not in [".pdb", ".mcb", ".bin"]:
      continue

    if args.Opt == "padding":
      if SlotSize != 0:
        DoAddPaddingData(os.path.join(os.environ["WORKSPACE_BINARIES"], GetRealFilePath(line)), SlotSize)
    elif args.Opt == "revert":
      File1 = os.path.join(os.environ["WORKSPACE_BINARIES"], GetRealFilePath(line))
      File2 = File1.replace(os.path.splitext(File1)[1], "_backup" + os.path.splitext(File1)[1])
      if os.path.isfile(File1) and os.path.isfile(File2):
        os.remove(File1)
        os.rename(File2, File1)
    else:
      assert (False)

  if args.Opt == "revert":
    FwVersion = 0x0001
    LowestSupportedVersion = 0x0001
    FwVersionString = "Version 0.0.0.1"

  #
  # generate MicrocodeVersion.data
  #
  GenMicrocodeVersionRawData (
    FwVersion,
    LowestSupportedVersion,
    FwVersionString,
    os.path.join(os.environ["WORKSPACE_BINARIES"], os.environ["TARGET_PLATFORM"] + "BinPkg", "Binaries", "Microcode", "MicrocodeVersion.data")
    )