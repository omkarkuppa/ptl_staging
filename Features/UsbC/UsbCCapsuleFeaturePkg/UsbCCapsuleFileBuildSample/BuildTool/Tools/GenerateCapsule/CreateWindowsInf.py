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
# This tool generates a Windows INF for the capsule.
###


import time
import sys
import re


### Parse input parameters ###

if len(sys.argv) == 1 or sys.argv[1] == "-h" or sys.argv[1] == "-H" or sys.argv[1] == "-?":
  print ("This tool creates a Windows INF for the capsule\n")
  print ("usage: CreateWindowsInf.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc>")
  print ("example: CreateWindowsInf.py UEFI 1.2.3.4 ########-####-####-####-############ UEFI_1.2.3.bin 0x01020304 Microsoft Microsoft \"Microsoft UEFI\"")
  sys.exit(-1)

if len(sys.argv) != 9:
  print ("Error: Incorrect number of parameters")
  print ("usage: CreateWindowsInf.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc>")
  sys.exit(-1)

SearchObj = re.search( r'\s', sys.argv[1], re.I)

if SearchObj:
  print ("Error: <fw_name> cannot contain any spaces")
  print ("usage: CreateWindowsInf.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc>")
  sys.exit(-1)

SearchObj = re.search( r'^0[Xx][0-9a-fA-F]{1,8}$', sys.argv[5], re.I)

if not SearchObj:
  print ("Error: Incorrect <hex_version> or it is longer than 32 bits")
  print ("usage: CreateWindowsInf.py <ProductFwName> <CapsuleVersionDotString> <ProductFmpGuid> <CapsuleFileName> <CapsuleVersionHexString> <ProductFwProvider> <ProductFwMfgName> <ProductFwDesc>")
  sys.exit(-1)


### INF Template ###

Template = """;
; {fwName}.inf
;
; Copyright (C) 2016 Microsoft Corporation.  All Rights Reserved.
;
[Version]
Signature="$WINDOWS NT$"
Class=Firmware
ClassGuid={{f2e7dd72-6468-4e36-b6f1-6488f42c1b52}}
Provider=%Provider%
DriverVer={date},{driverVer}
PnpLockdown=1
CatalogFile={fwName}.cat

[Manufacturer]
%MfgName% = Firmware,NTamd64

[Firmware.NTamd64]
%FirmwareDesc% = Firmware_Install,UEFI\RES_{{{EsrtGuid}}}

[Firmware_Install.NT]
CopyFiles = Firmware_CopyFiles

[Firmware_CopyFiles]
{OutputEfiName}

[Firmware_Install.NT.Hw]
AddReg = Firmware_AddReg

[Firmware_AddReg]
HKR,,FirmwareId,,{{{EsrtGuid}}}
HKR,,FirmwareVersion,%REG_DWORD%,{VersionIntHexString}
HKR,,FirmwareFilename,,{OutputEfiName}

[SourceDisksNames]
1 = %DiskName%

[SourceDisksFiles]
{OutputEfiName} = 1

[DestinationDirs]
DefaultDestDir = %DIRID_WINDOWS%,Firmware ; %SystemRoot%\Firmware

[Strings]
; localizable
Provider     = "{fwProvider}"
MfgName      = "{fwMfgName}"
FirmwareDesc = "{fwDesc}"
DiskName     = "Firmware Update"

; non-localizable
DIRID_WINDOWS = 10
REG_DWORD     = 0x00010001
"""


### Replace tokens in the Template ###

Date = time.strftime("%m/%d/%Y")
VersionString = sys.argv[2]
while(VersionString.count(".") < 3):
  VersionString += ".0"

Template = Template.format(fwName=sys.argv[1], date=Date, driverVer=VersionString, \
                           EsrtGuid=sys.argv[3], OutputEfiName=sys.argv[4], \
                           VersionIntHexString=sys.argv[5], fwProvider=sys.argv[6], \
                           fwMfgName=sys.argv[7], fwDesc=sys.argv[8])


### Write Template to the output file ###

# Open output file
File = open("%s.inf" % sys.argv[1], "w")

File.write(Template)

# Close output file
File.close()

print ("%s.inf created successfully!!!\n" % (sys.argv[1]))

sys.exit()


