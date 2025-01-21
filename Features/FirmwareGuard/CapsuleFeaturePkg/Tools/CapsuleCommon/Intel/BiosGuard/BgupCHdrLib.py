## @file
#  Definition of BiosGuard Update Package Certificate (BGUPC) Header.
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
#  @par Specification
#
##

#
# BGUPC Version
#
BGUPC_VERSION_1: int = 0x0001
BGUPC_VERSION_2: int = 0x0002
BGUPC_VERSION_3: int = 0x0003

#
# BGUPC Algorithm
#
BGUPC_RSA2048_SHA256_PKCS1_1P5 : int = 0x0000_0001
BGUPC_RSA2048_SHA256_PKCS1_V2P1: int = 0x0000_0002
BGUPC_RSA3072_SHA256_PKCS1_1P5 : int = 0x0000_0003
BGUPC_RSA3072_SHA256_PKCS1_V2P1: int = 0x0000_0004
BGUPC_RSA3072_SHA384_PKCS1_1P5 : int = 0x0000_0005
BGUPC_RSA3072_SHA384_PKCS1_V2P1: int = 0x0000_0006
