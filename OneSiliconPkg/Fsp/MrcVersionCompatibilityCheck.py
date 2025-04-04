## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
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

# Full import
import os
import sys
import re
import argparse

#Define error codes
SUCCESS = 0
FAIL = 1

WORKSPACE = os.environ.get("WORKSPACE", "")
WORKSPACE_SILICON = os.path.join(WORKSPACE, "Intel")
WORKSPACE_BINARIES = os.path.join(WORKSPACE, "Binaries")

def compare_binary_versions(file1, file2):
    # Check if the files exist
    if not os.path.isfile(file1):
        sys.stderr.write("Error: {file1} not found.\n")
        return FAIL, ""
    if not os.path.isfile(file2):
        sys.stderr.write("Error: {file2} not found.\n")
        return FAIL, ""
    # Extract the version from each file
    try:
        with open(file1, "rb") as f:
            f.seek(8)
            data1 = f.read(4)
        with open(file2, "rb") as f:
            f.seek(8)
            data2 = f.read(4)
    except Exception as e:
        sys.stderr.write("Error reading files: {e}\n")
        return FAIL, ""

    version1 = " ".join(str(b) for b in data1)
    version2 = " ".join(str(b) for b in data2)
    # Compare the versions
    if version1 == version2:
        return SUCCESS, version1
    else:
        return FAIL, ""

def get_green_mrc_version(file1, file2):
    # Call the function and capture the output
    status, green_mrc_version = compare_binary_versions(file1, file2)
    if status == SUCCESS:
        cleaned_green_mrc_version = ",".join(green_mrc_version.strip().split())
        if cleaned_green_mrc_version:
            return SUCCESS, cleaned_green_mrc_version
        else:
            print("Error in getting the Green MRC version")
            return FAIL, ""
    else:
        return status, green_mrc_version

def get_blue_mrc_version(file_path):
    # Check if the file exists
    if not os.path.isfile(file_path):
        print("DEBUG_ERROR:: {file_path} is not found. Breaking the build.")
        return FAIL, ""

    try:
        with open(file_path, "r") as f:
            lines = f.readlines()
    except Exception as e:
        print("DEBUG_ERROR:: Error reading {file_path}: {e}")
        return FAIL, ""

    if len(lines) < 2:
        print("DEBUG_ERROR:: {file_path} does not have enough lines.")
        return FAIL, ""

    blue_line = lines[-2].strip()
    blue_version_field = blue_line.split("/")[0]
    no_spaces = blue_version_field.replace(" ", "")
    cleaned_blue_mrc_version = re.sub(r",+", ",", no_spaces)
    return SUCCESS, cleaned_blue_mrc_version

def main():
    parser = argparse.ArgumentParser(description="MRC Version Compatibility Check")
    parser.add_argument("platform_name", help="Name of the platform")
    args = parser.parse_args()

    platform_name = args.platform_name
    platform_binpkg = f"{platform_name}BinPkg"

    # Green MRC info
    if os.path.exists(WORKSPACE_BINARIES):
        GREEN_MRC_BIN_DIR = os.path.join(WORKSPACE_BINARIES, platform_binpkg, "MemoryInit")
    else:
        GREEN_MRC_BIN_DIR = os.path.join(WORKSPACE_SILICON, platform_binpkg, "MemoryInit")
    GREEN_MRC_BIN_NAME = "GreenMrc.bin"
    GREEN_MRC_RELEASE_BIN_NAME = "ReleaseGreenMrc.bin"
    GREEN_MRC_BIN_PATH = os.path.join(GREEN_MRC_BIN_DIR, GREEN_MRC_BIN_NAME)
    GREEN_MRC_RELEASE_BIN_PATH = os.path.join(GREEN_MRC_BIN_DIR, GREEN_MRC_RELEASE_BIN_NAME)

    # Blue MRC info
    BLUE_MRC_VERSION_HEADER_DIR = os.path.join(WORKSPACE_SILICON, "OneSiliconPkg", "IpBlock", "MemoryInit", "src", "IncludePrivate", "Ptl")
    BLUE_MRC_VERSION_HEADER_NAME = "MrcVersion.h"
    BLUE_MRC_VERSION_HEADER_PATH = os.path.join(BLUE_MRC_VERSION_HEADER_DIR, BLUE_MRC_VERSION_HEADER_NAME)

    status_green, green_mrc_version = get_green_mrc_version(GREEN_MRC_BIN_PATH, GREEN_MRC_RELEASE_BIN_PATH)
    status_blue, blue_mrc_version = get_blue_mrc_version(BLUE_MRC_VERSION_HEADER_PATH)
    # Compare the blue_mrc_version and cleaned_green_mrc_version
    if status_green == SUCCESS and status_blue == SUCCESS:
        if green_mrc_version == blue_mrc_version:
            print("DEBUG_INFO::  Blue MRC version and Green MRC version match. \nblue_mrc_version =" + blue_mrc_version + "\ngreen_mrc_version = "+ green_mrc_version)
            sys.exit(SUCCESS)
        else:
            print("DEBUG_ERROR:: Blue MRC version and Green MRC version do not match. Breaking the build.\n")
            print("blue_mrc_version = " + blue_mrc_version)
            print("green_mrc_version = " + green_mrc_version)
            sys.exit(FAIL)
    else:
        if status_green != SUCCESS:
            print(green_mrc_version)
            sys.exit(status_green)
        elif status_blue != SUCCESS:
            print(blue_mrc_version)
            sys.exit(status_blue)

if __name__ == "__main__":
    main()
