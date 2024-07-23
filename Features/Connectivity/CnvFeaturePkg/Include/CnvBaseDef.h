/**@file
  The header file of CNV Base Definitions.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification
**/

#ifndef _CNV_BASE_DEF_H_
#define _CNV_BASE_DEF_H_

#define CONCAT_(x, y)  x ## y
#define CONCAT(x, y)   CONCAT_(x, y)
#define ACPI_STR_(x)   #x
#define ACPI_STR(x)    ACPI_STR_(x)

#define ACPI_CONCAT1(i)                 \_SB.i
#define ACPI_CONCAT2(i, j)              \_SB.i.j
#define ACPI_CONCAT3(i, j, k)           \_SB.i.j.k
#define ACPI_CONCAT4(i, j, k, x)        \_SB.i.j.k.x
#define ACPI_CONCAT5(i, j, k, x, y)     \_SB.i.j.k.x.y
#define ACPI_CONCAT6(i, j, k, x, y, z)  \_SB.i.j.k.x.y.z

#ifndef EP_DEV_NAME
#define EP_DEV_NAME  PXSX
#endif

#ifndef EP_ADR_OBJ_TYPE
#define EP_ADR_OBJ_TYPE  MethodObj
#endif

#ifndef RP_ADR_OBJ_TYPE
#define RP_ADR_OBJ_TYPE  MethodObj
#endif

#ifndef RP_PATH1
#define RP_PATH1  PC00
#else
  #undef RP_PATH_LENGTH
#define RP_PATH_LENGTH  1
#endif

#ifndef RP_PATH2
#define RP_PATH2  RP
#else
  #undef RP_PATH_LENGTH
#define RP_PATH_LENGTH  2
#endif

#ifndef RP_PATH3
#define RP_PATH3
#else
  #undef RP_PATH_LENGTH
#define RP_PATH_LENGTH  3
#endif

#ifndef RP_PATH4
#define RP_PATH4
#else
  #undef RP_PATH_LENGTH
#define RP_PATH_LENGTH  4
#endif

#ifndef RP_PATH5
#define RP_PATH5
#else
  #undef RP_PATH_LENGTH
#define RP_PATH_LENGTH  5
#endif

#ifndef RP_PATH_LENGTH
#define RP_PATH_LENGTH  2
#endif

#if RP_PATH_LENGTH == 1
#define WIFI_RP_PATH(RpNum)  ACPI_CONCAT1(CONCAT(RP_PATH1, RpNum))
#define WIFI_EP_PATH(RpNum)  ACPI_CONCAT2(CONCAT(RP_PATH1, RpNum), EP_DEV_NAME)
#elif RP_PATH_LENGTH == 2
#define WIFI_RP_PATH(RpNum)  ACPI_CONCAT2(RP_PATH1, CONCAT(RP_PATH2, RpNum))
#define WIFI_EP_PATH(RpNum)  ACPI_CONCAT3(RP_PATH1, CONCAT(RP_PATH2, RpNum), EP_DEV_NAME)
#elif RP_PATH_LENGTH == 3
#define WIFI_RP_PATH(RpNum)  ACPI_CONCAT3(RP_PATH1, RP_PATH2, CONCAT(RP_PATH3, RpNum))
#define WIFI_EP_PATH(RpNum)  ACPI_CONCAT4(RP_PATH1, RP_PATH2, CONCAT(RP_PATH3, RpNum), EP_DEV_NAME)
#elif RP_PATH_LENGTH == 4
#define WIFI_RP_PATH(RpNum)  ACPI_CONCAT4(RP_PATH1, RP_PATH2, RP_PATH3, CONCAT(RP_PATH4, RpNum))
#define WIFI_EP_PATH(RpNum)  ACPI_CONCAT5(RP_PATH1, RP_PATH2, RP_PATH3, CONCAT(RP_PATH4, RpNum), EP_DEV_NAME)
#elif RP_PATH_LENGTH == 5
#define WIFI_RP_PATH(RpNum)  ACPI_CONCAT5(RP_PATH1, RP_PATH2, RP_PATH3, RP_PATH4, CONCAT(RP_PATH5, RpNum))
#define WIFI_EP_PATH(RpNum)  ACPI_CONCAT6(RP_PATH1, RP_PATH2, RP_PATH3, RP_PATH4, CONCAT(RP_PATH5, RpNum), EP_DEV_NAME)
#endif

#ifndef USB2_PATH1
#define USB2_PATH1  PC00
#else
  #undef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  1
#endif

#ifndef USB2_PATH2
#define USB2_PATH2  XHCI
#else
  #undef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  2
#endif

#ifndef USB2_PATH3
#define USB2_PATH3  RHUB
#else
  #undef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  3
#endif

#ifndef USB2_PATH4
#define USB2_PATH4  HS
#else
  #undef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  4
#endif

#ifndef USB2_PATH5
#define USB2_PATH5
#else
  #undef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  5
#endif

#ifndef USB2_PATH_LENGTH
#define USB2_PATH_LENGTH  4
#endif

#if USB2_PATH_LENGTH == 1
#define USB2_PORT_PATH(PortNum)  ACPI_CONCAT1(CONCAT(USB2_PATH1, PortNum))
#elif USB2_PATH_LENGTH == 2
#define USB2_PORT_PATH(PortNum)  ACPI_CONCAT2(USB2_PATH1, CONCAT(USB2_PATH2, PortNum))
#elif USB2_PATH_LENGTH == 3
#define USB2_PORT_PATH(PortNum)  ACPI_CONCAT3(USB2_PATH1, USB2_PATH2, CONCAT(USB2_PATH3, PortNum))
#elif USB2_PATH_LENGTH == 4
#define USB2_PORT_PATH(PortNum)  ACPI_CONCAT4(USB2_PATH1, USB2_PATH2, USB2_PATH3, CONCAT(USB2_PATH4, PortNum))
#elif USB2_PATH_LENGTH == 5
#define USB2_PORT_PATH(PortNum)  ACPI_CONCAT5(USB2_PATH1, USB2_PATH2, USB2_PATH3, USB2_PATH4, CONCAT(USB2_PATH5, PortNum))
#endif

#endif // _CNV_BASE_DEF_H_
