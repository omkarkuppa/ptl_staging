## @file
#  Post build BpmGen script.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2017 Intel Corporation.
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

SCRIPT_ERROR=0
KM_REVOCATION=0
KM_ID=0x01

if [ ! -d $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen ]; then
  echo "#### Can not find BpmGen folder, bypass BpmGen ####"
  exit 0
fi

if [ ! -f $WORKSPACE_BINARIES/Tools/InternalOnly/BpmGen2/BpmGen2.exe ]; then
  echo "#### Can not find BpmGen2.exe, bypass BpmGen ####"
  exit 0
fi

echo "#### BpmGen2:  Generating KeyManifest.bin ####"

BPMGEN2=$WORKSPACE_BINARIES/Tools/InternalOnly/BpmGen2/bpmgen2
BPMGEN2_PARAM=bpmgen2_fsp.params


cd $WORKSPACE_PLATFORM/$PLATFORM_PACKAGE/InternalOnly/ToolScripts/BpmGen
BPMGEN2_ARG="KMGEN -KEY pubkey.pem BPM -KM $WORKSPACE/$BUILD_DIR/FV/KeyManifest.bin -SIGNKEY 3k_key_private.pem  -SIGHASHALG SHA384 -SCHEME RSAPSS -KMKHASH SHA384 -KMID $KM_ID -SVN $KM_REVOCATION -d:2"

chmod +x $BPMGEN2
$BPMGEN2 $BPMGEN2_ARG  > bpmgen2_km.txt

if [ $? -ne 0 ]; then
  echo "#### Error generating KM file #####"
  exit
fi

echo "#### BpmGen2:  Generating Manifest.bin ####"
BPMGEN2_ARG="GEN $WORKSPACE/$BUILD_DIR/FV/$1.fd $BPMGEN2_PARAM -BPM $WORKSPACE/$BUILD_DIR/FV/Manifest.bin -U $WORKSPACE/$BUILD_DIR/FV/$1_MBIOS.fd -KM $WORKSPACE/$BUILD_DIR/FV/KeyManifest.bin -d:2"

$BPMGEN2 $BPMGEN2_ARG  > bpmgen2_bpm.txt

if [ $? -ne 0 ]; then
  echo "#### Error generating BPM file #####"
  exit
fi

cp -f $WORKSPACE/$BUILD_DIR/FV/$1_MBIOS.fd $WORKSPACE/$BUILD_DIR/FV/$2.fd

cd $WORKSPACE