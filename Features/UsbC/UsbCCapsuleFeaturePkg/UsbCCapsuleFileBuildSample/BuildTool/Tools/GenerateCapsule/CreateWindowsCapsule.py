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
# @par Specification
##

##
# This is a sample tool and should not be used in production environments.
#
# This tool creates a signed Windows Capsule.
###


import time
import sys
import logging
import os
import shutil
import glob

print ("This is a sample tool and should not be used in production environments\n")

#raw_input('Press any key to continue . . .\n')


### Parse input parameters ###

if len(sys.argv) == 1 or sys.argv[1] == "-h" or sys.argv[1] == "-H" or sys.argv[1] == "-?":
  print ("This tool creates a signed Windows capsule\n")
  print ("usage: CreateWindowsCapsule.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc> <PfxFileName>")
  print ("example: CreateWindowsCapsule.py UEFI 1.2.3.4 ########-####-####-####-############ UEFI_1.2.3.bin 0x01020304 Microsoft Microsoft \"Microsoft UEFI\" OS_DRIVER.pfx")
  sys.exit(-1)

if len(sys.argv) != 10:
  print ("Error: Incorrect number of parameters")
  print ("usage: CreateWindowsCapsule.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc> <PfxFileName>")
  sys.exit(-1)

ProductName = sys.argv[1];
CapsuleVersion_DotString = sys.argv[2];
ProductFmpGuid = sys.argv[3];
CapsuleFileName = sys.argv[4];
CapsuleVersion_HexString = sys.argv[5];
ProductFwProvider = sys.argv[6];
ProductFwMfgName = sys.argv[7];
ProductFwDesc = sys.argv[8];
PfxFile = sys.argv[9];

logging.debug("CapsulePackage: Create Windows Capsule Files")
InfFileName = ProductName + ".inf"
CatFileName = ProductName + ".cat"

capsuleFolderPath = "WindowsCapsule"
if(os.path.exists(capsuleFolderPath)):
  shutil.rmtree(capsuleFolderPath)
  time.sleep(2)

os.mkdir(capsuleFolderPath)
shutil.copy2(CapsuleFileName, capsuleFolderPath)
os.chdir(capsuleFolderPath)

#Make INF
cmd = os.getenv('PYTHON_COMMAND')
cmd = cmd + " " + "..\CreateWindowsInf.py"
cmd = cmd + " " + "\"" + ProductName + "\"" + " " + "\"" + CapsuleVersion_DotString + "\""
cmd = cmd + " " + "\"" + ProductFmpGuid + "\"" + " " + "\"" + CapsuleFileName + "\""
cmd = cmd + " " + "\"" + CapsuleVersion_HexString + "\"" + " " + "\"" + ProductFwProvider + "\""
cmd = cmd + " " + "\"" + ProductFwMfgName + "\"" + " " + "\"" + ProductFwDesc + "\""
ret = os.system(cmd)
if(ret != 0):
  raise Exception("CreateWindowsInf Failed with errorcode %d" % ret)

#Find Signtool
SignToolPath = os.path.join(os.getenv("ProgramFiles(x86)"), "Windows Kits", "10", "bin", "x64", "signtool.exe")
if not os.path.exists(SignToolPath):
  SignToolPath = SignToolPath.replace('10', '8.1')
if not os.path.exists(SignToolPath):
  # use pattern match
  SignToolPathPattern = os.path.join (os.getenv("ProgramFiles(x86)"), "Windows Kits", "10", "bin", "*", "x64", "signtool.exe")
  SignToolPathList = glob.glob (SignToolPathPattern)
  if len (SignToolPathList) != 0:
    SignToolPath = (SignToolPathList)[0]
if not os.path.exists(SignToolPath):
  raise Exception("Can't find signtool on this machine.")

#Find Inf2Cat tool
Inf2CatToolPath = os.path.join(os.getenv("ProgramFiles(x86)"), "Windows Kits", "10", "bin", "x86", "Inf2Cat.exe")
if not os.path.exists(Inf2CatToolPath):
  # use pattern match
  Inf2CatToolPathPattern = os.path.join (os.getenv("ProgramFiles(x86)"), "Windows Kits", "10", "bin", "*", "x86", "Inf2Cat.exe")
  Inf2CatToolPathList = glob.glob (Inf2CatToolPathPattern)
  if len (Inf2CatToolPathList) != 0:
    Inf2CatToolPath = (Inf2CatToolPathList)[0]
if not os.path.exists(Inf2CatToolPath):
  raise Exception("Can't find Inf2Cat on this machine.  Please install the Windows 10 WDK - https://developer.microsoft.com/en-us/windows/hardware/windows-driver-kit")

#Make Cat file
cmd = "\"" + Inf2CatToolPath + "\"" + " /driver:. /os:10_X64 /verbose /uselocaltime"
ret = os.system(cmd)
if(ret != 0):
  raise Exception("Creating Cat file Failed with errorcode %d" % ret)

if(PfxFile is not None):
  #dev sign the cat file
  cmd = "\"" + SignToolPath + "\"" + " sign /a /v /fd SHA256 /f " + "..\\" + PfxFile + " " + CatFileName
  ret = os.system(cmd)
  if(ret != 0):
    raise Exception("Signing Cat file Failed with errorcode %d" % ret)

print ("\nWindows capsule created successfully in folder WindowsCapsule!!!")

sys.exit()
