## @file
# Post build FitGen script.
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

BIOS_INFO_GUID="4A4CA1C6-871C-45bb-8801-6910A7AA5807"
STARTUP_AC_MODULE_GUID="26fdaa3d-b7ed-4714-8509-eecf1593800d"

STARTUP_ACM_FOUND=0

grep $STARTUP_AC_MODULE_GUID $BUILD_DIR/FV/FW_BINARIES_FV_SIGNED.inf > /dev/null
if [ $? -eq 0 ]; then
  STARTUP_ACM_FOUND=1
fi

if [ $STARTUP_ACM_FOUND -eq 1 ]; then
  STARTUP_AC_PARA="-S $STARTUP_AC_MODULE_GUID"
fi

cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_ORG.fd

#
# FitGen parameter usage reference:
#   -D It is FD file instead of FV file.
#   -F <FitTablePointerOffset>
#   -I <BiosInfoGuid>
#   -NA No 0x800 aligned Microcode requirement. No -NA means Microcode is 0x800 aligned.
#   -O RecordType <RecordDataAddress RecordDataSize>|<RESERVE RecordDataSize>
#   -P RecordType <IndexPort DataPort Width Bit Index>
#

if [ "$COMPILER" = "" ]; then
  FitGen -D $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_FIT.fd -F 0x40 -NA -I $BIOS_INFO_GUID $STARTUP_AC_PARA -O 0x0C RESERVE 0x600 -O 0x0B RESERVE 0x400 -P 0xA 0x70 0x71 0x1 0x4 0x2A
else
  FitGen -D $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_FIT.fd -F 0x40 -NA -I $BIOS_INFO_GUID
fi

if [ $? -ne 0 ]; then
  exit $?
fi

cp -f $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS_FIT.fd $WORKSPACE/$BUILD_DIR/FV/CLIENTBIOS.fd
