## @file
# RebaseFspmBinBaseAddress.py
#
# This script rebases FSP-M to a known address where it will be decompressed.
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
# @par Specification
#
##

import os
import sys
import re
import subprocess

fspBinPath        = sys.argv[1]
fspBinFile        = sys.argv[2]
fspBinFileRebased = "Fsp_Rebased.fd"
MapFile       = sys.argv[3]
splitFspBinPath   = os.path.join("edk2","IntelFsp2Pkg","Tools","SplitFspBin.py")

if len(sys.argv) == 5:
  splitFspBinPath   = sys.argv[4]

#
# Make sure argument passed or valid
#
fspBinFilePath = fspBinPath + os.sep + fspBinFile
print (str(fspBinFilePath))
print (str(splitFspBinPath))
print (str(MapFile))
if not os.path.exists(fspBinFilePath):
  print ("WARNING!  " + str(fspBinFilePath) + " is not found.")
  exit(1)
if not os.path.exists(splitFspBinPath):
  print ("WARNING!  " + str(splitFspBinPath) + " is not found.")
  exit(1)
if not os.path.exists(MapFile):
  print ("WARNING!  " + str(MapFile) + " is not found.")
  exit(1)

#
# Get the FSP-S / FSP-M-T FV Base Address from Flash Map
#
file = open (MapFile, "r")
data = file.read ()
for line in data.split("\n"):
  if "PcdSecondaryDataStackBase" in line:
    fspMNemBase = int(line.split("|")[1].split()[0], 16)
file.close()

#
# FSP-M will be decompressed at PcdSecondaryDataStackBase + 4K to adjust decompression
# header and align with 4K boundary.
#
fspMBaseAddress = fspMNemBase + 0x1000

#
# Get FSP-M Size, in order to calculate the FSP-T Base. Used SplitFspBin.py script
# to dump the header, and get the ImageSize in FSP-M section
#
pythontool = sys.executable

#
# Re-base FSP bin file to new address and save it as fspBinFileRebased using SplitFspBin.py
#
rebaseArguments = fspBinFilePath + " -c m -b " + str(hex(fspMBaseAddress).rstrip("L")) + "  -o" + fspBinPath + " -n " + fspBinFileRebased
os.system('"' + pythontool + '"' + " " + splitFspBinPath + " rebase -f" + rebaseArguments)

#
# Split FSP bin to FSP-S/M/T segments
#
splitArguments = fspBinPath + os.sep + fspBinFileRebased + " -o " + fspBinPath + " -n Fsp_Rebased.fd"
os.system('"' + pythontool + '"' + " " + splitFspBinPath + " split -f" + splitArguments)

exit(0)
