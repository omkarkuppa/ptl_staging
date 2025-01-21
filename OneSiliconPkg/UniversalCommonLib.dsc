## @file
#  Component description file for libraries that are used by all supported products,
#  both Pei and Dxe libraries DSC file.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2012 Intel Corporation.
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
#@par Specification Reference:
#
##
#
# Silicon Init Common Library
#
  FspErrorInfoLib|$(PLATFORM_SI_PACKAGE)/Library/FspErrorInfoLib/PeiDxeFspErrorInfoLib.inf

#
# Common
#
!if $(SEAMLESS_SDU_SUPPORT) == TRUE && $(CPU_GNRD_SUPPORT) != TRUE
  MmRuntimeUpdateLib|SmmRuntimeUpdateFeaturePkg/Library/MmRuntimeUpdateLib/MmRuntimeUpdateLib.inf
!else
  MmRuntimeUpdateLib|$(PLATFORM_SI_PACKAGE)/Library/MmRuntimeUpdateLibNull/MmRuntimeUpdateLibNull.inf
!endif

#
# Cpu
#
  FixedPointLib|$(PLATFORM_SI_PACKAGE)/Library/FixedPointLib/FixedPointLib.inf

#
# Me
#

#
# Pch
#


#
# SA
#

#
# IpBlock Memory Init
#
 MemoryInstallLib|$(PLATFORM_SI_PACKAGE)/Fsp/Library/MemoryInstallLib/MemoryInstallLib.inf