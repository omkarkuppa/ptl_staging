## @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2016 Intel Corporation.
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

# !!! This postbuild.sh must run under workspace root using $WORKSPACE/$PLATFORM_BOARD_PACKAGE/PostBuild.sh !!!"
#cd $WORKSPACE
echo "PostBuild."

PATH=$PATH:$WORKSPACE/$BUILD_DIR/FV
cp $BUILD_DIR/FV/GuidedSectionTools.txt $BUILD_DIR/FV/GuidToolDefinitionConf.ini
FMMT_PATH=$WORKSPACE/Edk2/BaseTools/BinWrappers/PosixLike

#
# FMMT tool search the "FmmtConf.ini" file in PATH environment variable.
# Build folder is one of the possible path.
#
cp $BUILD_DIR/FV/GuidedSectionTools.txt $BUILD_DIR/FV/FmmtConf.ini

#
# UPL Post Build Process Begin
#
if [ "$UNIVERSAL_PAYLOAD" = "TRUE" ]; then
  # +-----------------+
  # | NV_Storage      |
  # +-----------------+
  # | FvPostMemory    |
  # +-----------------+
  # | FvSecurity      |
  # +-----------------+
  # | FvAdvanced      |
  # +-----------------+
  # | FvOsBoot        |                                                               +------------------+
  # +-----------------+                                                        +------| PayloadEntry.elf |
  # | FvUefiBoot      |                                                        |      +------------------+
  # +-----------------+  FMMT   +---------+           +---------+ objcopy      |      +------------------+
  # | FvUpl (Empty)   |<--------| UPL.ffs |<----------| UPL.elf |<-------------+------| uefi_fv.fv       |
  # +-----------------+         +---------+           +---------+              |      +------------------+
  # | FvFwBinaries    |                                                        |            ...
  # +-----------------+                                                        |      +------------------+
  # | FvMicrocode     |                                                        +------| other.fv         |
  # +-----------------+                                                               +------------------+
  # | FvPostMemory    |
  # +-----------------+
  # | FSP-S           |
  # +-----------------+
  # | FSP-M           |
  # +-----------------+
  # | FSP-T           |
  # +-----------------+
  # | FvPreMemory     |
  # +-----------------+
  export UPL_BUILD_DIR=Build/UefiPayloadPkgX64

  python3 $WORKSPACE_CORE/UefiPayloadPkg/UniversalPayloadBuild.py \
    -s $UPL_SPEC_REVISION \
    --SkipBuild \
    --AddFv bds_fv=$WORKSPACE/$BUILD_DIR/FV/FVPPLBDSUNCOMPACT.Fv \
    -pb $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.elf
  if [ $? -ne 0 ]; then
    return 1
  fi

  GenSec \
    -s EFI_SECTION_RAW \
    -o $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.raw \
       $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.elf
  if [ $? -ne 0 ]; then
    return 1
  fi

  GenSec \
    --sectionalign 16 \
    -o $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.align \
       $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.raw
  if [ $? -ne 0 ]; then
    return 1
  fi

  LzmaCompress \
    -e $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.align \
    -o $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.lzma
  if [ $? -ne 0 ]; then
    return 1
  fi

  GenSec \
    -s EFI_SECTION_GUID_DEFINED \
    -g EE4E5898-3914-4259-9D6E-DC7BD79403CF \
    -r PROCESSING_REQUIRED \
    -o $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.guid \
       $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.lzma
  if [ $? -ne 0 ]; then
    return 1
  fi

  GenFfs \
    -t EFI_FV_FILETYPE_DXE_CORE \
    -g 728c3e86-88e7-447a-a146-8f99915ebafa \
    -o $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.ffs \
    -i $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.guid
  if [ $? -ne 0 ]; then
    return 1
  fi

  $PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Features/Upl/Tools/FvUplSizeCheck.py -c \
    $WORKSPACE/$BUILD_DIR/FV/FVUPL.Fv.map \
    $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.ffs
  if [ $? -ne 0 ]; then
    return 1
  fi

  $FMMT_PATH/FMMT -a \
    $BUILD_DIR/FV/CLIENTBIOS.fd \
    F69584D4-56F0-4D8E-A387-072A836CDD4E \
    $WORKSPACE/$UPL_BUILD_DIR/UniversalPayload.ffs \
    $BUILD_DIR/FV/CLIENTBIOS.fd
  if [ $? -ne 0 ]; then
    return 1
  fi
fi
#
# UPL Post Build Process End
#


#
# Payload Management Post Build Process Begin
#
export PAYLOAD_BUILD_TOOL_PATH=$WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/PayloadManagement
export PAYLOAD_BUILD_FLAGS=IntegrateBuild

# Set generic arguments for NotFit/Fit Payload build
if [ "$TARGET" = "RELEASE" ]; then
  export PAYLOAD_BUILD_FLAGS="$PAYLOAD_BUILD_FLAGS r"
fi

#
# Generate Non-FIT Payload
#
. $PAYLOAD_BUILD_TOOL_PATH/NonFitPayloads/NonFitPayloadBuild.sh $PAYLOAD_BUILD_FLAGS
echo "Non Fit Payload build end..."
if [ $? -ne 0 ]; then
  echo "!!! ERROR: Non FitPayloadBuild execute failure !!!"
  exit 1
fi

# Add more arguments for FitPayload build
if [ "$RESILIENCY_BUILD" = "TRUE" ]; then
  export PAYLOAD_BUILD_FLAGS="$PAYLOAD_BUILD_FLAGS res"
fi
if [ "$WCL_BUILD" = "TRUE" ]; then
  export PAYLOAD_BUILD_FLAGS="$PAYLOAD_BUILD_FLAGS wcl"
fi

#
# Generate FIT Payload
#
. $PAYLOAD_BUILD_TOOL_PATH/FitPayloads/FitPayloadBuild.sh $PAYLOAD_BUILD_FLAGS
echo "Fit Payload build end..."
if [ $? -ne 0 ]; then
  echo "!!! ERROR: FitPayloadBuild execute failure !!!"
  exit 1
fi

#
# Patch Payloads to ClientBios
#
echo "Patch Payloads to ClientBios"
export NON_FIT_PAYLOAD_BUILD_DIR=$WORKSPACE/Build/NonFitPayloadsPkg/${TARGET}_${TOOL_CHAIN_TAG}
export FIT_PAYLOAD_BUILD_DIR=$WORKSPACE/Build/FitPayloadsPkg/${TARGET}_${TOOL_CHAIN_TAG}
cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADPRE.fd

#
# Patch NotFitPayload
#
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/PayloadManagement/ReplaceFvInImage.py \
  -I $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
  -F $NON_FIT_PAYLOAD_BUILD_DIR/FV/NONFITPAYLOADS.fd \
  -O $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADTEMP.fd
if [ $? -ne 0 ]
then
  echo "Error: Patch NonFitPayload failure"
  exit 1
fi

#
# Patch FitPayload
#
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/PayloadManagement/ReplaceFvInImage.py \
  -I $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADTEMP.fd \
  -F $FIT_PAYLOAD_BUILD_DIR/FV/FITPAYLOADS.fd \
  -O $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADPOST.fd
if [ $? -ne 0 ]
then
  echo "Error: Patch FitPayload failure"
  exit 1
fi

rm $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADTEMP.fd
cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_PAYLOADPOST.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd
#
# Payload Management Post Build Process End
#


#
# XmlCli: Post Build Process Begin
#

# Set EDK2 tool paths for XmlCli processing
EDK_TOOLS_BIN=$WORKSPACE/Edk2/BaseTools/Source/C/bin
EDK_TOOLS_BINWRAPPERS=$WORKSPACE/Edk2/BaseTools/BinWrappers/PosixLike


WORKSPACE_BUILD_DIR="$WORKSPACE/$BUILD_DIR"
XML_CLI_COMMON_BUILD_PATH="$WORKSPACE_BUILD_DIR/X64/XmlCliFeaturePkg/XmlCliCommon"
XML_CLI_COMMON_DIR="$WORKSPACE_COMMON/Features/XmlCliFeaturePkg/XmlCliCommon"
INPUT_VFR_DIRS="$WORKSPACE_BUILD_DIR/X64/$PLATFORM_FULL_PACKAGE/Setup/Setup/OUTPUT $WORKSPACE_BUILD_DIR/X64/$PLATFORM_FULL_PACKAGE/Features/UiApp/D89A7D8B-D016-4D26-93E3-EAB6B4D3B0A2/OUTPUT $XML_CLI_COMMON_BUILD_PATH/Dxe/OUTPUT"
if [ -d "$XML_CLI_COMMON_BUILD_PATH" ]; then
  echo "Create and insert Bios Knobs Data Bin file for XmlCli"
  if [ ! -d "$XML_CLI_COMMON_BUILD_PATH/Dummy" ]; then
    mkdir -p "$XML_CLI_COMMON_BUILD_PATH/Dummy"
  fi
  #
  # Extract Variable names & expression's, publish it in Build Dir/BiosKnobsData.bin file
  #
  "$PYTHON_COMMAND" "$XML_CLI_COMMON_DIR/Tool/ParseSetup.py" post_build -b "$WORKSPACE_BUILD_DIR" -xb "$XML_CLI_COMMON_BUILD_PATH" -iv $INPUT_VFR_DIRS -edk "$EDK_TOOLS_BIN" -ets "$EDK_TOOLS_BINWRAPPERS"
  GenSec -s EFI_SECTION_USER_INTERFACE -n "XmlCliBiosKnobsData" -o "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsUISec.ui"
  GenSec -s EFI_SECTION_RAW -o "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.sec" "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.bin"
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g 615E6021-603D-4124-B7EA-C48A3737BACD -o "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.ffs" -i "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.sec" -i "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsUISec.ui"
  "$FMMT_PATH/FMMT" -a "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd" A881D567-6CB0-4EEE-8435-2E72D33E45B5 "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.ffs" "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd"
  
  if [ ! -f "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd" ]; then
    echo "=== XmlCliPostBuild: Trying to generate fd file once again... ==="
    "$FMMT_PATH/FMMT" -a "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd" A881D567-6CB0-4EEE-8435-2E72D33E45B5 "$XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.ffs" "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd"
    if [ ! -f "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd" ]; then
      echo "!!! XmlCliPostBuild-ERROR:Build Fail !!!"
      echo "*** XmlCliPostBuild: $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd does not exist. ***"
      echo "-- XmlCliPostBuild: FMMT insertion process may have failed, please check logs above --"
      return 1
    fi
  fi
  
  # Backing up CLIENTBIOS.fd before overwriting
  if [ ! -f "$WORKSPACE/$BUILD_DIR/FV/ClientBios.fd.bak" ]; then
    cp "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd" "$WORKSPACE/$BUILD_DIR/FV/ClientBios.fd.bak"
  fi

  cp "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd" "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd"
  # Clean up backup file
  rm -f "$WORKSPACE/$BUILD_DIR/FV/ClientBios.fd.bak"
  rm -f "$WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd"

  echo "==== BiosKnobsData.bin File Inserted ====="
else
  echo "DEBUG: Directory does not exist: $XML_CLI_COMMON_BUILD_PATH"
  echo "DEBUG: Skipping XmlCli Post Build Process"
fi
#
# XmlCli: Post Build Process End
#


#
# Post-build FitGen
#
BIOS_INFO_GUID="4A4CA1C6-871C-45bb-8801-6910A7AA5807"
STARTUP_AC_MODULE_GUID="26FDAA3D-B7ED-4714-8509-EECF1593800D"
MICROCODE_ARRAY_FFS_GUID="197DB236-F856-4924-90F8-CDF12FB875F3"

STARTUP_ACM_FOUND=0

grep $STARTUP_AC_MODULE_GUID $BUILD_DIR/FV/FVFWBINARIES.inf > /dev/null
if [ $? -eq 0 ]; then
  STARTUP_ACM_FOUND=1
fi

if [ $STARTUP_ACM_FOUND -eq 1 ]; then
  STARTUP_AC_PARA="-S $STARTUP_AC_MODULE_GUID"
fi

if [ ! -d "$WORKSPACE/RomImages" ]; then
  mkdir -p $WORKSPACE/RomImages
fi

echo --- Analyze ASL runtime behavior using acpiexec ---
cd $BUILD_X64
if [ ! -d AslAnalyze ]; then
  mkdir -p AslAnalyze
fi
if [ ! -d AslAnalyze/TglU  ]; then
  mkdir -p AslAnalyze/TglU
fi
if [ ! -d AslAnalyze/TglY ]; then
  mkdir -p AslAnalyze/TglY
fi

cd AslAnalyze
find ../ -name '*.aml' -exec cp -f '{}' . \;
cd TglU
mv -f ../*TglU*.aml .
mv -f ../Dptf.aml .
cd ../TglY
mv -f ../*TglY*.aml .
cd ..
if [ "$COMPILER" != "XCODE" ]; then
echo q | acpiexec -di -l *.aml TglU/*.aml > $WORKSPACE/$BUILD_DIR/AslAnalyzeTglURvp.log
echo q | acpiexec -di -l *.aml TglY/*.aml > $WORKSPACE/$BUILD_DIR/AslAnalyzeTglYRvp.log

echo --- ASL analysis completed ---
fi
cd $WORKSPACE
#
# ---run TXTSX tool to generate FIT table---
#

export OPENSSL_PATH=$WORKSPACE_BINARIES/Tools/OpenSSL
export BUILD_FV_FOLDER=$WORKSPACE/$BUILD_DIR/FV
export HASH_TMP_FOLDER=$BUILD_FV_FOLDER/FvIbbnp1Tmp
FV_IBBNP1_HASH_FILE_GUID="87A71A9A-327F-4117-B73C-2B31D7748435"
if [ ! -d $HASH_TMP_FOLDER ]; then
  mkdir $HASH_TMP_FOLDER
fi
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Features/MultiIbbConfig/Tools/FvIbbnp1Hash.py \
  -t $HASH_TMP_FOLDER \
  --openssl-tool-path $OPENSSL_PATH
GenSec -s EFI_SECTION_USER_INTERFACE -n "FvIbbnp1Digest" -o $HASH_TMP_FOLDER/FvIbbnp1Digest.ui
GenSec -s EFI_SECTION_RAW -o $HASH_TMP_FOLDER/FvIbbnp1Digest.raw $HASH_TMP_FOLDER/FvIbbnp1Digest.bin
GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $FV_IBBNP1_HASH_FILE_GUID -o $HASH_TMP_FOLDER/FvIbbnp1Digest.ffs -i $HASH_TMP_FOLDER/FvIbbnp1Digest.raw -i $HASH_TMP_FOLDER/FvIbbnp1Digest.ui
$FMMT_PATH/FMMT -r $BUILD_FV_FOLDER/CLIENTBIOS.fd $FV_IBBNP1_HASH_FILE_GUID $HASH_TMP_FOLDER/FvIbbnp1Digest.ffs $HASH_TMP_FOLDER/CLIENTBIOS_POST.fd
cp $HASH_TMP_FOLDER/CLIENTBIOS_POST.fd $BUILD_FV_FOLDER/CLIENTBIOS.fd
rm -rf $HASH_TMP_FOLDER
if [ $? -ne 0 ]
then
  echo "!!! ERROR: FvIbbnp1Hash.py execute failure !!!"
  exit 1
fi

# Cutting off unused part of Extended BIOS Region out of the FD image
if [ "$EXTENDEDREGION_BUILD" != "TRUE" ] || [ -z "$FLASHMAP_FDF" ]; then
  echo "Skipping Extended Region Post Build Process"
else
  EXTENDED_REGION_SIZE=""
  EXTENDED_REGION_IN_USE=""

  # Extract values from FLASHMAP_FDF
  EXTENDED_REGION_SIZE=$(grep -oP 'DEFINE EXTENDED_REGION_SIZE\s+=\s*\K[\dx]+' "$FLASHMAP_FDF")
  EXTENDED_REGION_IN_USE=$(grep -oP 'DEFINE EXTENDED_REGION_IN_USE\s+=\s*\K[\dx]+' "$FLASHMAP_FDF")

  echo "Extended BIOS Region size        : $EXTENDED_REGION_SIZE"
  echo "Extended BIOS Region size in use : $EXTENDED_REGION_IN_USE"

  # Calculate cutoff size
  CUTOFF_SIZE=$((EXTENDED_REGION_SIZE - EXTENDED_REGION_IN_USE))
  echo "Cutting off unused $CUTOFF_SIZE in size"

  # Copy and split operations
  cp -f $BUILD_FV_FOLDER/CLIENTBIOS.fd $BUILD_FV_FOLDER/ClientBios_ExtSplitPre.fd

  Split -f $BUILD_FV_FOLDER/CLIENTBIOS.fd -s $CUTOFF_SIZE -t $BUILD_FV_FOLDER/ClientBios_ExtSplitPost.fd -o $BUILD_FV_FOLDER/Garbage.bin
  

  cp -f $BUILD_FV_FOLDER/ClientBios_ExtSplitPost.fd $BUILD_FV_FOLDER/CLIENTBIOS.fd
  rm -f $BUILD_FV_FOLDER/Garbage.bin

  # Cut off extended region from ClientBios.fd
  # We will append extended region to ClientBios.fd in last step of PostBuild.sh
  Split -f $BUILD_FV_FOLDER/CLIENTBIOS.fd -s $EXTENDED_REGION_IN_USE -o $BUILD_FV_FOLDER/ExtendedRegion.bin -t $BUILD_FV_FOLDER/CLIENTBIOS.fd
fi

#
# [BEGIN] BIOS Capsule Update Fault Tolerance Support
#

export TEMP_FLASHMAP_FDF=$WORKSPACE/FlashMapInclude_Temp.fdf
export OPENSSL_PATH=$WORKSPACE_BINARIES/Tools/OpenSSL
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/StripFlashmap.py "$TARGET" $FLASHMAP_FDF $TEMP_FLASHMAP_FDF

#
# Calculate FIT location, which is located FIT_OFFSET_TO_FSP_TOP size before FSP-T and FSP-O
# Prevent values in excess of 2^31 by cutting off the first "FF" before calculation, and inserting "FF" after that
#
FIXED_FIT_LOCATION=
if [ "$FSP_RESET" = "TRUE" ]; then
  FIT_OFFSET_TO_FSP_TOP=0x200
  FLASH_FSPO_SIZE=$(awk '/gBoardModuleTokenSpaceGuid\.PcdFlashFvFspOSize/ {print $4}' $TEMP_FLASHMAP_FDF)
  FLASH_FSPT_SIZE=$(awk '/gMinPlatformPkgTokenSpaceGuid\.PcdFlashFvFspTSize/ {print $4}' $TEMP_FLASHMAP_FDF)
  FLASH_FSPO_SIZE=0x${FLASH_FSPO_SIZE: -6}
  FLASH_FSPT_SIZE=0x${FLASH_FSPT_SIZE: -6}
  FIXED_FIT_LOCATION=$((0x1000000 - $FLASH_FSPO_SIZE - $FLASH_FSPT_SIZE - $FIT_OFFSET_TO_FSP_TOP))
  FIXED_FIT_LOCATION=$(printf "0xFF%X\n" $FIXED_FIT_LOCATION)
  echo "FIXED_FIT_LOCATION: $FIXED_FIT_LOCATION"
fi

#
# Create PostIbbDigest.hash
# The order of FVs get reported need to match the FV hash digest calculated in CreateStoredHashFvPpiInfo.
#
export POST_IBB_HASH_TMP_FOLDER=$BUILD_DIR/FV/PostIbbDigestTmp

if [ "$FSP_SIGNED" = "TRUE" ]; then
  $PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/CreatePostIbbHash.py \
    -i $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
    --fdf $TEMP_FLASHMAP_FDF \
    --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset" \
    --temp $POST_IBB_HASH_TMP_FOLDER \
    --openssl-tool-path $OPENSSL_PATH \
    -o $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/PostIbbDigest.hash
else
  $PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/CreatePostIbbHash.py \
    -i $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
    --fdf $TEMP_FLASHMAP_FDF \
    --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSOffset" \
    --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset" \
    --temp $POST_IBB_HASH_TMP_FOLDER \
    --openssl-tool-path $OPENSSL_PATH \
    -o $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/PostIbbDigest.hash
fi
if [ $? -ne 0 ]
then
  echo "!!! ERROR: CreatePostIbbHash.py execute failure !!!"
  exit 1
fi

rm -rf $POST_IBB_HASH_TMP_FOLDER

#
# Below scripts calculate the digest of OBB region and Non Fit Payload region to replace the dummy one in FvPreMemory.
# The purpose of OBB/Non Fit Payload digest is to examine the integrity when loading OBB/Non Fit Payload from external storage for recovery.
# So this section must be executed when:
#    1. After all operations on OBB FVs are done.
#    2. Before BootGuard BPM/KM is created.
#
FMMT -v $WORKSPACE/$BUILD_DIR/FV/FVPREMEMORY.Fv > $WORKSPACE/$BUILD_DIR/FV/FVPREMEMORY.Fv.Info

if [ -z "$(grep -m1 "ObbDigest" $WORKSPACE/$BUILD_DIR/FV/FVPREMEMORY.Fv.Info)" ]; then
  echo "No ObbDigest found in FvPreMemory. Skip BIOS fault tolerance support process in post build."
elif [ -z "$(grep -m1 "NonFitPayloadDigest" $WORKSPACE/$BUILD_DIR/FV/FVPREMEMORY.Fv.Info)" ]; then
  echo "No Non Fit Payload digest found in FvPreMemory. Skip BIOS fault tolerance support process in post build."
else
  echo "Start to calculate digest for BIOS fault tolerance support."
  #
  # Create the ObbDigest.bin and NonFitPayloadDigest.bin.
  #
  export OBB_HASH_FILE_GUID=169BB326-C8E4-4588-A742-6808D7499B47
  export PAYLOAD_HASH_FILE_GUID=7B0B85CA-8A43-4520-BA8B-33370F305210
  export OBB_HASH_TMP_FOLDER=$WORKSPACE/$BUILD_DIR/FV/ObbDigestTmp
  export FLASH_OBB_OFFSET=$(awk '/SET gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashObbOffset/ { for(i=1;i<=NF;i++) if($i ~ /^0x/) print $i }' $TEMP_FLASHMAP_FDF)
  export FLASH_OBB_SIZE=$(awk '/SET gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashObbSize/ { for(i=1;i<=NF;i++) if($i ~ /^0x/) print $i }' $TEMP_FLASHMAP_FDF)
  if [ "$EXTENDEDREGION_BUILD" == "TRUE" ]; then
    #hard code, need to refine.
    NvsOffset=0x02000000
    FLASH_OBB_OFFSET=$(($FLASH_OBB_OFFSET - $NvsOffset))
  fi
  export FLASH_NVS_OBB_SIZE=$(printf "0x%08X" $(($FLASH_OBB_OFFSET + $FLASH_OBB_SIZE)))

  echo "OBB Offset   = $FLASH_OBB_OFFSET"
  echo "OBB Size     = $FLASH_OBB_SIZE"
  echo "NVS+OBB Size = $FLASH_NVS_OBB_SIZE"

  if [ ! -d $OBB_HASH_TMP_FOLDER ]; then
    mkdir $OBB_HASH_TMP_FOLDER
  fi

  cp $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd

  #
  # Split CLIENTBIOS.fd to get Obb.bin
  #
  Split -f $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd -s $FLASH_NVS_OBB_SIZE -o $OBB_HASH_TMP_FOLDER/NvsObb.bin -t $OBB_HASH_TMP_FOLDER/IBBx.bin
  Split -f $OBB_HASH_TMP_FOLDER/NvsObb.bin -s $FLASH_OBB_OFFSET -o $OBB_HASH_TMP_FOLDER/Nvs.bin -t $OBB_HASH_TMP_FOLDER/Obb.bin

  #
  # Calculate OBB and Non-FIT Payload SHA384 digest to replace the dummy FFS.
  #
  $OPENSSL_PATH/openssl dgst -binary -sha384 $OBB_HASH_TMP_FOLDER/Obb.bin > $OBB_HASH_TMP_FOLDER/ObbDigest.bin
  $OPENSSL_PATH/openssl dgst -binary -sha384 $NON_FIT_PAYLOAD_BUILD_DIR/FV/NONFITPAYLOADS.fd > $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.bin

  #
  # Prepare ObbDigest.ffs.
  #
  GenSec -s EFI_SECTION_USER_INTERFACE -n "ObbDigest" -o $OBB_HASH_TMP_FOLDER/ObbDigest.ui
  GenSec -s EFI_SECTION_RAW $OBB_HASH_TMP_FOLDER/ObbDigest.bin -o $OBB_HASH_TMP_FOLDER/ObbDigest.raw
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $OBB_HASH_FILE_GUID -o $OBB_HASH_TMP_FOLDER/ObbDigest.ffs -i $OBB_HASH_TMP_FOLDER/ObbDigest.raw -i $OBB_HASH_TMP_FOLDER/ObbDigest.ui

  #
  # Prepare NonFitPayloadDigest.ffs.
  #
  GenSec -s EFI_SECTION_USER_INTERFACE -n "NonFitPayloadDigest" -o $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.ui
  GenSec -s EFI_SECTION_RAW $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.bin -o $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.raw
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $PAYLOAD_HASH_FILE_GUID -o $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.ffs -i $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.raw -i $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.ui

  #
  # Use FMMT tool to replace the dummy FFS into actual one.
  #
  FMMT -r $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ObbDigest $OBB_HASH_TMP_FOLDER/ObbDigest.ffs $OBB_HASH_TMP_FOLDER/CLIENTBIOS_TEMP.fd
  FMMT -r $OBB_HASH_TMP_FOLDER/CLIENTBIOS_TEMP.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D NonFitPayloadDigest $OBB_HASH_TMP_FOLDER/NonFitPayloadDigest.ffs $OBB_HASH_TMP_FOLDER/CLIENTBIOS_TEMP.fd

  if [ "$EXTENDEDREGION_BUILD" == "TRUE" ]; then
    EXTENDED_HASH_FILE_GUID=151CB3B2-87B0-4A37-8353-ECB59945E9A0
    cp -f $BUILD_FV_FOLDER/ExtendedRegion.bin $OBB_HASH_TMP_FOLDER/ExtendedRegion.bin
    $OPENSSL_PATH/openssl dgst -binary -sha384 $OBB_HASH_TMP_FOLDER/ExtendedRegion.bin > $OBB_HASH_TMP_FOLDER/ExtendedDigest.bin
    GenSec -s EFI_SECTION_USER_INTERFACE -n "ExtendedDigest" -o $OBB_HASH_TMP_FOLDER/ExtendedDigest.ui
    GenSec -s EFI_SECTION_RAW $OBB_HASH_TMP_FOLDER/ExtendedDigest.bin -o $OBB_HASH_TMP_FOLDER/ExtendedDigest.raw
    GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $EXTENDED_HASH_FILE_GUID -o $OBB_HASH_TMP_FOLDER/ExtendedDigest.ffs -i $OBB_HASH_TMP_FOLDER/ExtendedDigest.raw -i $OBB_HASH_TMP_FOLDER/ExtendedDigest.ui
    FMMT -r $OBB_HASH_TMP_FOLDER/CLIENTBIOS_TEMP.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ExtendedDigest $OBB_HASH_TMP_FOLDER/ExtendedDigest.ffs $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd
  else
    cp -f $OBB_HASH_TMP_FOLDER/CLIENTBIOS_TEMP.fd $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd
  fi

  #
  # Keep Obb.bin for BIOS Resiliency OBB BGUP build below.
  #
  cp -f $OBB_HASH_TMP_FOLDER/Obb.bin $WORKSPACE/$BUILD_DIR/FV/Obb.bin
  cp -f $OBB_HASH_TMP_FOLDER/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd

  rm -rf $OBB_HASH_TMP_FOLDER

  echo "End to calculate digest for BIOS fault tolerance support."
fi

rm $TEMP_FLASHMAP_FDF

#
# [END] BIOS Capsule Update Fault Tolerance Support
#

#
# [START] BIOS Resiliency Support
#

if [ "$RESILIENCY_BUILD" != "TRUE" ]; then
  echo "Resiliency Build is not enabled. Skip BIOS Resiliency support process in post build."
else
  export IBB_BGSL_FILE_GUID=F53FC14B-025C-4477-9B48-7A1B19F80F30
  export OBB_BGSL_FILE_GUID=318D30B7-F669-4AF2-ADE1-E3F84D097BB3
  export PAYLOAD_BGSL_FILE_GUID=CB25FE33-8B2A-4770-85E7-1EE130E22789
  export BGSL_TMP_FOLDER=$WORKSPACE/$BUILD_DIR/FV/BgslTmp
  export BIOS_GUARD_UPDATE_PACKAGE_PATH=$WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/Tools/ToolScripts/BiosGuard/UpdatePackage
  export BIOS_GUARD_SVN=$(awk '/^VERSION_MAJOR/ { printf $3 }' $BIOS_ID_FILE | tr -d '\r')

  echo "BIOS Guard SVN                 = $BIOS_GUARD_SVN"
  echo "BIOS Guard Update Package Path = $BIOS_GUARD_UPDATE_PACKAGE_PATH"

  if [ ! -d $BGSL_TMP_FOLDER ]; then
    mkdir $BGSL_TMP_FOLDER
  fi

  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/GenerateBGSL.py \
    genbgsl \
    -t $TARGET \
    -m $FLASHMAP_FDF \
    -o $BIOS_GUARD_UPDATE_PACKAGE_PATH

  #
  # Generate IBB <-> IBBR sync up BGUP.
  #
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BuildBGUP_SPI.py -p script_ibb_ibbr.bgsl -v $BIOS_GUARD_SVN -use_ftu false
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  cat capsule_update_package.BIOS_Guard update_package_bgupc.biosguard > $BGSL_TMP_FOLDER/IbbBgsl.bin
  rm script.bin *.BIOS_Guard *.biosguard

  #
  # Generate OBB_to_SPI BGUP.
  #
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BuildBGUP_SPI.py -d $WORKSPACE/$BUILD_DIR/FV/Obb.bin -p script_obb.bgsl -v $BIOS_GUARD_SVN -use_ftu false
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  cat capsule_update_package.BIOS_Guard update_package_bgupc.biosguard > $BGSL_TMP_FOLDER/ObbRBgsl.bin
  rm script.bin *.BIOS_Guard *.biosguard

  #
  # Generate NONFITPAYLOAD_to_SPI BGUP.
  #
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BuildBGUP_SPI.py -d $NON_FIT_PAYLOAD_BUILD_DIR/FV/NONFITPAYLOADS.fd -p script_BuildBGUP_NonFitPayload.bgsl -v $BIOS_GUARD_SVN -use_ftu false
  $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
  cat capsule_update_package.BIOS_Guard update_package_bgupc.biosguard > $BGSL_TMP_FOLDER/NonFitPayloadBgsl.bin
  rm script.bin *.BIOS_Guard *.biosguard
  if [ "$EXTENDEDREGION_BUILD" == "TRUE" ]; then
    #
    # Generate Extended_to_SPI BGUP.
    #
    $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BuildBGUP_SPI.py -d $WORKSPACE/$BUILD_DIR/FV/ExtendedRegion.bin -p script_extendedbios.bgsl -v $BIOS_GUARD_SVN -use_ftu false
    echo "Create BGUP for ExtendBios"
    $PYTHON_COMMAND $BIOS_GUARD_UPDATE_PACKAGE_PATH/BiosGuardCapsule.py -i update_package.BIOS_Guard -o capsule_update_package.BIOS_Guard
    cat capsule_update_package.BIOS_Guard update_package_bgupc.biosguard > $BGSL_TMP_FOLDER/ExtendedBgsl.bin
    rm script.bin *.BIOS_Guard *.biosguard

    #
    # Prepare ExtendedBgsl.ffs
    #
    EXTENDED_HASH_FILE_GUID=6F43F2C6-D0C7-40F2-AB33-3A907CFE7ECB
    GenSec -s EFI_SECTION_USER_INTERFACE -n "ExtendedBgsl" -o $BGSL_TMP_FOLDER/ExtendedBgsl.ui
    GenSec -s EFI_SECTION_RAW $BGSL_TMP_FOLDER/ExtendedBgsl.bin -o $BGSL_TMP_FOLDER/ExtendedBgsl.raw
    GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $EXTENDED_HASH_FILE_GUID -o $BGSL_TMP_FOLDER/ExtendedBgsl.ffs -i $BGSL_TMP_FOLDER/ExtendedBgsl.raw -i $BGSL_TMP_FOLDER/ExtendedBgsl.ui
  fi

  #
  # Prepare IbbBgsl.ffs.
  #
  GenSec -s EFI_SECTION_USER_INTERFACE -n "IbbBgsl" -o $BGSL_TMP_FOLDER/IbbBgsl.ui
  GenSec -s EFI_SECTION_RAW  $BGSL_TMP_FOLDER/IbbBgsl.bin -o $BGSL_TMP_FOLDER/IbbBgsl.raw
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $IBB_BGSL_FILE_GUID -o $BGSL_TMP_FOLDER/IbbBgsl.ffs -i $BGSL_TMP_FOLDER/IbbBgsl.raw -i $BGSL_TMP_FOLDER/IbbBgsl.ui

  #
  # Prepare ObbRBgsl.ffs.
  #
  GenSec -s EFI_SECTION_USER_INTERFACE -n "ObbRBgsl" -o $BGSL_TMP_FOLDER/ObbRBgsl.ui
  GenSec -s EFI_SECTION_RAW  $BGSL_TMP_FOLDER/ObbRBgsl.bin -o $BGSL_TMP_FOLDER/ObbRBgsl.raw
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $OBB_BGSL_FILE_GUID -o $BGSL_TMP_FOLDER/ObbRBgsl.ffs -i $BGSL_TMP_FOLDER/ObbRBgsl.raw -i $BGSL_TMP_FOLDER/ObbRBgsl.ui

  #
  # Prepare NonFitPayloadBgsl.ffs.
  #
  GenSec -s EFI_SECTION_USER_INTERFACE -n "NonFitPayloadBgsl" -o $BGSL_TMP_FOLDER/NonFitPayloadBgsl.ui
  GenSec -s EFI_SECTION_RAW  $BGSL_TMP_FOLDER/NonFitPayloadBgsl.bin -o $BGSL_TMP_FOLDER/NonFitPayloadBgsl.raw
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g $PAYLOAD_BGSL_FILE_GUID -o $BGSL_TMP_FOLDER/NonFitPayloadBgsl.ffs -i $BGSL_TMP_FOLDER/NonFitPayloadBgsl.raw -i $BGSL_TMP_FOLDER/NonFitPayloadBgsl.ui

  #
  # Replace the dummy BGSL FFS in CLIENTBIOS.fd with the actual one.
  #
  if [ "$EXTENDEDREGION_BUILD" == "TRUE" ]; then
    cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/ClientBios_Extended.fd
    FMMT -r $WORKSPACE/$BUILD_DIR/FV/ClientBios_Extended.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ExtendedBgsl $BGSL_TMP_FOLDER/ExtendedBgsl.ffs $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd
  fi

  FMMT -r $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D IbbBgsl $BGSL_TMP_FOLDER/IbbBgsl.ffs $BGSL_TMP_FOLDER/CLIENTBIOS_TEMP.fd
  FMMT -r $BGSL_TMP_FOLDER/CLIENTBIOS_TEMP.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D NonFitPayloadBgsl $BGSL_TMP_FOLDER/NonFitPayloadBgsl.ffs $BGSL_TMP_FOLDER/CLIENTBIOS_TEMP.fd
  FMMT -r $BGSL_TMP_FOLDER/CLIENTBIOS_TEMP.fd FC8FE6B5-CD9B-411E-BD8F-31824D0CDE3D ObbRBgsl $BGSL_TMP_FOLDER/ObbRBgsl.ffs $BGSL_TMP_FOLDER/CLIENTBIOS.fd

  cp -f $BGSL_TMP_FOLDER/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd

  #
  # Clean-up the intermediate files.
  #
  rm -rf $BGSL_TMP_FOLDER

  rm -f $BIOS_GUARD_UPDATE_PACKAGE_PATH/script_ibb_ibbr.bgsl
  rm -f $BIOS_GUARD_UPDATE_PACKAGE_PATH/script_obb.bgsl
  rm -f $BIOS_GUARD_UPDATE_PACKAGE_PATH/script_BuildBGUP_NonFitPayload.bgsl
  rm -f $BIOS_GUARD_UPDATE_PACKAGE_PATH/script_extendedbios.bgsl
fi

#
# [END] BIOS Resiliency Support
#

# Support the Type 2 FIT entry version 0x200 format.
export STARTUP_ACM_PARA=
export ACM_FIT_ENTRY_DATA_FILE=$WORKSPACE/$BUILD_DIR/FV/AcmFitGenPara.txt

# Parsing the BtGAcm binary in BIOS rom image.
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BtgAcmMisc/BtgAcmMiscScript.py \
  FitEntry \
  -I $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
  -S $BTG_ACM_SLOT_SIZE \
  -O $ACM_FIT_ENTRY_DATA_FILE
if [ $? -ne 0 ]
then
  echo "!!! Error: Failed to generate the FIT type-2 entry parameter file !!!"
  exit 1
fi

# Get the parameter used for FitGen tool.
while read -r line
do
  export STARTUP_ACM_PARA="$STARTUP_ACM_PARA $line"
done < "$ACM_FIT_ENTRY_DATA_FILE"

export STARTUP_AC_PARA="$STARTUP_AC_PARA $STARTUP_ACM_PARA"

    #
    # Run FitGen tool to generate FIT
    #
    cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_FITPRE.fd

    # FitGen parameter usage reference:
    #   -D It is FD file instead of FV file.
    #   -F <FitTablePointerOffset>
    #   -L <MicrocodeSlotSize> <MicrocodeFfsGuid>
    #   -I <BiosInfoGuid>
    #   -NA No 0x800 aligned Microcode requirement. No -NA means Microcode is 0x800 aligned.
    #   -O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>
    #   -P RecordType <IndexPort DataPort Width Bit Index>
    #
    # For slot mode, the SLOT_SIZE environment variable already defined in PriBuild.sh script
    #

# Generate Bsis binary for both FSP Signing and non FSP Signing cases.
# In FSP Signing case, Bsis binary includes data for FSP verification;
# In non FSP Signing case, Bsis binary only includes FSP-T UPD and BSP entry.
# FSP Signing feature is not enabled in GCC build environment yet.
export FSPM_LOADING_POLICY=0x0
if [ "$FSPM_COMPRESSED" = "TRUE" ]; then
  export FSPM_LOADING_POLICY=$(($FSPM_LOADING_POLICY + 0x2))
fi
if [ "$FSP_SIGNED" = "TRUE" ]; then
  if [ "$MULTI_IBB_BUILD" != "TRUE" ]; then
    export FSPM_LOADING_POLICY=$(($FSPM_LOADING_POLICY + 0x1))
  fi
fi

$PYTHON_COMMAND $WORKSPACE_COMMON/$PLATFORM_BOARD_PACKAGE/BoardSupport/Tools/BsssGen/BsssGen.py \
      -Fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
      -FspmLP $FSPM_LOADING_POLICY \
      -s $FSP_SIGNED \
      -o $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/bsis.bin \
      -d $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/bsis.bin

BuildFitArgs="-D $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd"
BuildFitArgs="$BuildFitArgs -F 0x40 -NA -L $SLOT_SIZE $MICROCODE_ARRAY_FFS_GUID"
BuildFitArgs="$BuildFitArgs -I $BIOS_INFO_GUID $STARTUP_AC_PARA -O 0x0B RESERVE 0x400"
if [ "$FSP_RESET" = "TRUE" ]; then
  if [ "$FSP_SIGNED" = "TRUE" ]; then
    FBM_OFFSET_TO_FIT=0x1E00
    FIXED_FBM_LOCATION=0x${FIXED_FIT_LOCATION: -6}
    FIXED_FBM_LOCATION=$((FIXED_FBM_LOCATION - FBM_OFFSET_TO_FIT))
    FIXED_FBM_LOCATION=$(printf "0xFF%X\n" $FIXED_FBM_LOCATION)
    echo "FIXED_FBM_LOCATION: $FIXED_FBM_LOCATION"
    BuildFitArgs="$BuildFitArgs -O 0x0C RESERVE 0x800 -O 0x0D $FIXED_FBM_LOCATION"
    BuildFitArgs="$BuildFitArgs 0x800 -P 0xA 0x70 0x71 0x1 0x4 0x2a -T $FIXED_FIT_LOCATION"
  else
    BuildFitArgs="$BuildFitArgs -O 0x0C RESERVE 0x800 -P 0xA 0x70 0x71 0x1 0x4 0x2A -T $FIXED_FIT_LOCATION"
  fi
else
  BuildFitArgs="$BuildFitArgs -O 0x0C RESERVE 0x600 -P 0xA 0x70 0x71 0x1 0x4 0x2A"
fi
FitGen $BuildFitArgs
if [ $? -ne 0 ]; then
  echo "FitGen failure - fail."
  exit $?
fi

#
# Post-build postbuildBpmGen Start
#
if [ "$WCL_BUILD" = "TRUE" ]; then
  if [ -f $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BpmGenWcl/postbuildBpmGen.sh ]; then
  . $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BpmGenWcl/postbuildBpmGen.sh CLIENTBIOS CLIENTBIOS
  fi
else
  if [ -f $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BpmGen/postbuildBpmGen.sh ]; then
    . $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/BpmGen/postbuildBpmGen.sh CLIENTBIOS CLIENTBIOS
  fi
fi
if [ $? -ne 0 ]; then
  echo "postbuildBpmGen.sh - fail."
  exit $?
fi

rm $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/bsis.bin
#
# Post-build postbuildBpmGen End
#

#
# ---Create ROM and Simics images---
#
# Get BIOS MAJOR_VERSION and MIN_VERSION
BIOS_MAJOR_VERSION=`grep ^VERSION_MAJOR $WORKSPACE/$BIOS_ID_FILE`
IFS=' ' read -ra WORDS <<< "$BIOS_MAJOR_VERSION"
BIOS_MAJOR_VERSION=${WORDS[${#WORDS[@]} - 1]}
unset WORDS
BIOS_MAJOR_VERSION=$(printf "${BIOS_MAJOR_VERSION}" | tr -d '\r\n')

BIOS_MIN_VERSION=`grep ^VERSION_MINOR $WORKSPACE/$BIOS_ID_FILE`
IFS=' ' read -ra WORDS <<< "$BIOS_MIN_VERSION"
BIOS_MIN_VERSION=${WORDS[${#WORDS[@]} - 1]}
unset WORDS
BIOS_MINOR_VERSION=$(printf "${BIOS_MINOR_VERSION}" | tr -d '\r\n')

BIOS_ROM_FILENAME_INTEXT=External
BUILD_TYPE=$TARGET

if [ "$COMPILER" = "GCC" ]; then
  ROM_FILENAME_COMPILER=GCC
elif [ "$COMPILER" = "CLANG" ]; then
  ROM_FILENAME_COMPILER=CLANGPDB_LINUX
else
  ROM_FILENAME_COMPILER=UNKNOWN
fi

BIOS_PREFIX=PTL

if [ "$PTL_BUILD" = "TRUE" ]; then
  BIOS_PREFIX=PTL_U_P_H
fi


# ---Create ROM images---
. $WORKSPACE_BINARIES/$PLATFORM_BIN_PACKAGE/Tools/ToolScripts/RomImage/SetupRomDirs.sh

