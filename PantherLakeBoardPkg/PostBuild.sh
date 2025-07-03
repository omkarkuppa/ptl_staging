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


# In FSP@Reset solution, FspO Fv will be located at 4G memory address.
# FIT table should not be in FspO Fv and will be put at 4G - 0x5400.
#
FIXED_FIT_LOCATION=0xFFFFAC00

#
# XmlCli: Post Build Process Begin
#
XML_CLI_COMMON_BUILD_PATH=$BUILD_DIR/X64/XmlCliFeaturePkg/XmlCliCommon
XML_CLI_COMMON_DIR=$WORKSPACE_COMMON/Features/XmlCliFeaturePkg/XmlCliCommon
INPUT_VFR_DIRS=$BUILD_DIR/X64/$PLATFORM_PACKAGE/Setup/Setup/OUTPUT $BUILD_DIR/X64/$PLATFORM_PACKAGE/Features/UiApp/D89A7D8B-D016-4D26-93E3-EAB6B4D3B0A2/OUTPUT $XML_CLI_COMMON_BUILD_PATH/Dxe/OUTPUT
if [ -d $XML_CLI_COMMON_BUILD_PATH ]; then
  echo "Create and insert Bios Knobs Data Bin file for XmlCli"
  if [ ! -d $XML_CLI_COMMON_BUILD_PATH/Dummy ]; then
   mkdir $XML_CLI_COMMON_BUILD_PATH/Dummy
  fi
  #
  #Extract Variable names & expression's, publish it in Build Dir/BiosKnobsData.bin file
  #
  $PYTHON_COMMAND $XML_CLI_COMMON_DIR/Tool/ParseSetup.py post_build -b $BUILD_DIR -xb $XML_CLI_COMMON_BUILD_PATH -iv $INPUT_VFR_DIRS
  echo "==== XmlCliPostBuild: BiosKnobsData.bin File Created ====="
  GenSec -s EFI_SECTION_USER_INTERFACE -n "XmlCliBiosKnobsData" -o $XML_CLI_COMMON_BUILD_PATH/BiosKnobsUISec.ui
  GenSec -s EFI_SECTION_RAW -o $XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.sec $XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.bin
  GenFfs -t EFI_FV_FILETYPE_FREEFORM -g 615E6021-603D-4124-B7EA-C48A3737BACD -o $XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.ffs -i $XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.sec -i $XML_CLI_COMMON_BUILD_PATH/BiosKnobsUISec.ui
  $FMMT_PATH/FMMT -a $BUILD_DIR/FV/CLIENTBIOS.fd A881D567-6CB0-4EEE-8435-2E72D33E45B5 $XML_CLI_COMMON_BUILD_PATH/BiosKnobsData.ffs $BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd
  if [ ! -f $BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd ]; then
    echo "!!! XmlCliPostBuild-ERROR:Build Fail !!!"
    echo "*** XmlCliPostBuild: $BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd does not exist. ***"
    echo "-- XmlCliPostBuild: FMMT insertion process may have failed, please check logs above --"
    return 1
  fi

  cp $BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd $BUILD_DIR/FV/CLIENTBIOS.fd
  rm $BUILD_DIR/FV/CLIENTBIOS_XmlCli.fd

  echo "==== BiosKnobsData.bin File Inserted ====="
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

#
# Creat PostIbbDigest.hash
# The order of FVs get reported need to match the FV hash digest calculated in CreateStoredHashFvPpiInfo .
#
export POST_IBB_HASH_TMP_FOLDER=$BUILD_DIR/FV/PostIbbDigestTmp
export OPENSSL_PATH=$WORKSPACE_BINARIES/Tools/OpenSSL
$PYTHON_COMMAND $WORKSPACE_PLATFORM/$PLATFORM_BOARD_PACKAGE/Tools/CreatePostIbbHash.py \
  -i $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
  --fdf $WORKSPACE/FlashMapInclude_Temp.fdf \
  --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvFspSOffset" \
  --fv "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset" \
  --temp $POST_IBB_HASH_TMP_FOLDER \
  --openssl-tool-path $OPENSSL_PATH \
  -o $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/PostIbbDigest.hash
rm $WORKSPACE/FlashMapInclude_Temp.fdf
rm -rf $POST_IBB_HASH_TMP_FOLDER
if [ $? -ne 0 ]
then
  echo "!!! ERROR: CreatePostIbbHash.py execute failure !!!"
  exit 1
fi

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
    # For slot mode, the SLOT_SIZE envir variable already defined in PriBuild.sh script
    #

# Gerneate Bsis binary for both FSP Signing and non FSP Signing cases.
# In FSP Signing case, Bsis binary includes data for FSP verification;
# In non FSP Signing case, Bsis binary only includes FSP-T UPD and BSP entry.
# FSP Signing feature is not enabeld in GCC build environment yet.
export FSPM_LOADING_POLICY=0x0
if [ "$FSPM_COMPRESSED" = "TRUE" ]; then
  export FSPM_LOADING_POLICY=0x2
fi
$PYTHON_COMMAND $WORKSPACE_COMMON/$PLATFORM_BOARD_PACKAGE/BoardSupport/Tools/BsssGen/BsssGen.py \
      -Fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd \
      -FspmLP $FSPM_LOADING_POLICY \
      -o $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/bsis.bin \
      -d $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen/bsis.bin

if [ "$FSP_RESET" = "TRUE" ]; then
    FitGen -D $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd -F 0x40 -NA -L $SLOT_SIZE $MICROCODE_ARRAY_FFS_GUID -I $BIOS_INFO_GUID $STARTUP_AC_PARA -O 0x0C RESERVE 0x800 -O 0x0B RESERVE 0x400 -P 0xA 0x70 0x71 0x1 0x4 0x2A -T $FIXED_FIT_LOCATION
else
    FitGen -D $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd -F 0x40 -NA -L $SLOT_SIZE $MICROCODE_ARRAY_FFS_GUID -I $BIOS_INFO_GUID $STARTUP_AC_PARA -O 0x0C RESERVE 0x600 -O 0x0B RESERVE 0x400 -P 0xA 0x70 0x71 0x1 0x4 0x2A
fi
if [ $? -ne 0 ]; then
  echo "FitGen failure - fail."
  exit $?
fi

#
# Post-build postbuildBpmGen Start
#
if [ "$WCL_BUILD" = "TRUE" ]; then
  if [ -f $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGenWcl/postbuildBpmGen.sh ]; then
  . $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGenWcl/postbuildBpmGen.sh CLIENTBIOS CLIENTBIOS
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
