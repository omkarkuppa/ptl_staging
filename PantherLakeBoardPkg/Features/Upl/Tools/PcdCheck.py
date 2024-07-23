#!/usr/bin/env python
# -*- coding: UTF-8 -*-
# @file
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
#
##
import os
from ctypes import *
import argparse
import sys
import glob
import re


gPackageName = ""

#
# Data Structure for GUID
#
class GUID(Structure):
    _fields_ = [
        ('Guid1',            c_uint32),
        ('Guid2',            c_uint16),
        ('Guid3',            c_uint16),
        ('Guid4',            c_uint8 * 8),
    ]

#
# Data structure of Dxe Pcd Database
#
class DXE_PCD_DATABASE(Structure):
    _fields_ = [
        ('Signature',                    GUID),
        ('BuildVersion',                 c_uint32),
        ('Length',                       c_uint32),
        ('SystemSkuId',                  c_uint64),
        ('LengthForAllSkus',             c_uint32),
        ('UninitDataBaseSize',           c_uint32),
        ('LocalTokenNumberTableOffset',  c_uint32),
        ('ExMapTableOffset',             c_uint32),
        ('GuidTableOffset',              c_uint32),
        ('StringTableOffset',            c_uint32),
        ('SizeTableOffset',              c_uint32),
        ('SkuIdTableOffset',             c_uint32),
        ('PcdNameTableOffset',           c_uint32),
        ('LocalTokenCount',              c_uint16),
        ('ExTokenCount',                 c_uint16),
        ('GuidTableCount',               c_uint16),
        ('Pad',                          c_uint8 * 8),
    ]

#
# Data Structure for ExMap Table in DXE PCD Database
#
class EX_MAP_TABLE_STRUCT(Structure):
    _fields_ = [
        ('ExTableMapDataToken',    c_uint32),
        ('ExTableMapDataIdx',      c_uint16),
        ('ExTableMapDataGuidIdx',  c_uint16),
    ]

#
# This function returns the GuidValue as a string of the below format
# {0xXXXXXXXX, 0xXXXX, 0xXXXX, {0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX, 0xXX}}
#
def GuidStringFormat(GuidValue):
    GuidValueData = GUID.from_buffer_copy(GuidValue)
    GuidValueString = "{"+hex(GuidValueData.Guid1) + ","+ hex(GuidValueData.Guid2) + "," +hex(GuidValueData.Guid3) + ",{"
    GuidByteArraySize = int(sizeof(GuidValueData.Guid4)/sizeof(c_uint8))
    for GuidValueByte in range(0,GuidByteArraySize):
        GuidValueString+= hex(GuidValueData.Guid4[GuidValueByte])
        if (GuidValueByte != GuidByteArraySize - 1):
            GuidValueString+= ","
        if (GuidValueByte == GuidByteArraySize - 1):
            GuidValueString+="}}"
    return GuidValueString

#
# This function prints the PCDs that PPL consumes but not listed in
# PlatformPayloadPcdListLibNull.inf
#
def ParseMissingPcdTokens(pcd_database_file, DecTokenGuidPair):
    ExTableMapDataToken = []
    ExTableMapDataGuidIdx = []
    GuidTable = []
    with open(pcd_database_file, "rb") as fd:
        PcdDatabaseBuffer = fd.read()
        # Storing the PCD Database file as an object of the class DXE_PCD_DATABASE.
        pcd_database = DXE_PCD_DATABASE.from_buffer_copy(PcdDatabaseBuffer)
        for ExTokenCount in range(0,pcd_database.ExTokenCount):
            # Moving to the beginning of the ExMap table and looping through all the table entries
            # for extracting the DataToken and corresponding Guid Table index from all the table entries.
            ExMapTableRow = PcdDatabaseBuffer[
                pcd_database.ExMapTableOffset+(ExTokenCount*sizeof(EX_MAP_TABLE_STRUCT)):
                pcd_database.ExMapTableOffset+(ExTokenCount*sizeof(EX_MAP_TABLE_STRUCT)) + sizeof (EX_MAP_TABLE_STRUCT)
            ]
            ExMapTableData = EX_MAP_TABLE_STRUCT.from_buffer_copy(ExMapTableRow)
            ExTableMapDataToken.append(hex(ExMapTableData.ExTableMapDataToken))
            ExTableMapDataGuidIdx.append(ExMapTableData.ExTableMapDataGuidIdx)
        for GuidTableCount in range(0,pcd_database.GuidTableCount):
            # Moving to the beginning of the GuidTable and extracting all the Guid values.
            GuidValue = PcdDatabaseBuffer[
                pcd_database.GuidTableOffset+(GuidTableCount*sizeof(GUID)):
                pcd_database.GuidTableOffset+(GuidTableCount*sizeof(GUID)) + sizeof (GUID)
            ]
            GuidValueString = GuidStringFormat(GuidValue)
            GuidTable.append(GuidValueString)
        for ExTokenCount in range(0,pcd_database.ExTokenCount):
            DecTokenGuidPair.append([ExTableMapDataToken[ExTokenCount],GuidTable[ExTableMapDataGuidIdx[ExTokenCount]]])

#
# Parse GUID and PCD names to Missing PCDs
#
def PrintMissingPcdNames(DecTokenGuidPair, path_file):
    global gPackageName

    ExPcdMiss = []
    ExPcdFull = []
    GuidName  = []
    PcdTable  = []
    DecPcdTable = []

    PackagePathList = glob.glob (f"{path_file}/Build/**/{gPackageName}/**/*Pkg", recursive=True)
    PackageNameList = []
    for PackagePath in PackagePathList:
        PackageNameList.append (re.findall (r"\w+Pkg", PackagePath)[-1])
    #
    # Get missing DynamicExPcd
    #
    files = glob.glob (f"{path_file}/**/*.dec", recursive=True)
    for file in files:
        IsProjectDecFile = False
        for PackageName in PackageNameList:
            if PackageName in file:
                IsProjectDecFile = True
        if IsProjectDecFile == False:
            continue
        with open(file) as f:
            DecContent = f.read().split("\n")
        Step = 0
        for line in DecContent:
            if "[Guids]" in line:
                Step = 1
                continue
            elif "PcdsDynamic" in line:
                Step = 2
                continue
            elif Step in (1, 2)  and (line.startswith('[')):  Step = 0
            # Search GUID name
            if Step == 1:
                # Filter Comment
                line = re.sub(r'#.+', '', line).replace(" ", "")
                if "=" in line:
                    name, value = line.split("=")
                    value = value.replace("{", "").replace("}", "").split(",")
                    value = [int(x, 16) for x in value]
                    for Pcd in DecTokenGuidPair:
                        missGuid = Pcd[1].replace("{", "").replace("}", "").split(",")
                        missGuid = [int(x, 16) for x in missGuid]
                        if missGuid == value:
                            GuidName = name
                            PcdTable.append([GuidName, Pcd[0]])
            # Search PCDs name
            elif Step == 2:
                # Filter Comment
                line = re.sub(r'#.+', '', line).replace(" ", "")
                try:
                    Token = line.split("|")[-1].split()[0].replace ("{", "")
                    Token = int(Token, 16)
                    Guid = line.split(".")[0]
                    Guid = Guid.strip()
                    DecPcdTable.append([Guid, Token, line.split("|")[0].strip()])
                except:
                    continue
    # Find the missing ExPcds
    for DecPcd in DecPcdTable:
        for Pcd in PcdTable:
            if Pcd[0] == DecPcd[0] and int(Pcd[1], 16) == DecPcd[1]:
                ExPcdMiss.append (DecPcd[2])

    #
    # Get All DynamicExPcd
    #
    files = glob.glob (f"{path_file}/Build/**/{gPackageName}/**/*.inf", recursive=True)
    for file in files:
        IsPcdExScope = False
        with open (file, "r") as File:
            for line in File:
                if  "[PcdEx" in line:
                    IsPcdExScope = True
                    continue
                if "[" in line:
                    IsPcdExScope == False
                    continue
                if IsPcdExScope == True and "." in line:
                    ExPcdFull.append (line.split("#")[0].strip())
                    continue
    ExPcdFull = set(ExPcdFull)

    #
    # Filter duplicate guid
    #
    ExPcdMissList = []
    for ExPcdMissItem in ExPcdMiss:
        if ExPcdMissItem in ExPcdFull:
            ExPcdMissList.append (ExPcdMissItem)

    return set(ExPcdMissList)

def main():
    global gPackageName
    parser = argparse.ArgumentParser()
    parser.add_argument(
        '-w',
        '--Workspace',
        required = False,
        dest = 'Workspace',
        help = 'Please input your workspace path',
        default = os.environ['WORKSPACE']
        )
    args = parser.parse_args()
    path_file = args.Workspace
    pcd_database_file = ""
    gPackageName = re.findall ('\w+BoardPkg', __file__)[-1]
    pcd_database_file_list = glob.glob (f"{path_file}/**/{gPackageName}/**/DXEPcdDataBase.raw", recursive=True)
    for pcd_database_item in pcd_database_file_list:
        if (pcd_database_item.find ("UefiPayloadPkg") != -1):
            continue
        pcd_database_file = pcd_database_item

    if not os.path.exists(pcd_database_file):
        print ("Error: PcdDatabase in {} is not generated!!! Make sure platform dsc file should contain MdeModulePkg/Universal/PCD/Dxe/Pcd.inf".format (pcd_database_file))
        return 1
    try:
        with open(pcd_database_file, "rb") as fd:
            buff = fd.read()
            pcd_database = DXE_PCD_DATABASE.from_buffer_copy(buff)
            local_token_count = pcd_database.LocalTokenCount
    except:
        print ("Error: PcdDatabase in {} is not correct format!!!".format(pcd_database_file))
        return 1
    if local_token_count != 0:
        DecTokenGuidPair = []
        print("### Pcd data File ### - {}".format(pcd_database_file))
        ParseMissingPcdTokens(pcd_database_file, DecTokenGuidPair)
        ExPcd = PrintMissingPcdNames(DecTokenGuidPair, path_file)
        print("Error: Below PCD(s) is(are) missed in PEI PCD database\n")
        print("\n".join (ExPcd))
        print("Please add these dynamic PCD into XXXBoardPkg/Features/Upl/Library/PeiPayloadHookLib/PeiPayloadHookLib.inf!!!\n")
        return 1
    return 0

if __name__ == "__main__":
    sys.exit(main())
