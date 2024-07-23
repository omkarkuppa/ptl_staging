/** @file
  ACPI EC Common APIs under \_SB scope.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference:
**/

#include <EcCommands.h>

#ifndef PARENT_OF_LPCB
#define PARENT_OF_LPCB PC00
#endif

// PPOE is the BIOS setting for PD PS_ON Enable
External (PPOE, IntObj)
External (UAXL, MethodObj)
External (\_SB.PC00.LPCB.NRTS, MethodObj)
Mutex (HLDI, 0) // Hold indicator: 0- No one accessing the Power State 1- Someone else is accessing the Power State

/**
  RLDS        Get Lid status.
  @param      None.
  @retval     Return the LID status: 1 - LID open, 0 - LID close.
**/
Method (RLDS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.LSTE)))
}

/**
  RVPS        Get virtual power state.
  @param      None.
  @retval     Return the virtual AC power: 1 - AC present, 0 - AC not present.
**/
Method (RVPS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.VPWR)))
}

/**
  RBED        Get button enable/disable.
  @param      None.
  @retval     Return button enable/disable:
              bit0    - Power button N/A to disable
              bit1    - Windows button
              bit2    - volume up button
              bit3    - volume down button
              bit4    - rotation lock button
              bit5-7  - reserved
**/
Method (RBED)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BTEN)))
}

/**
  RAPS        Get AC power state.
  @param      None.
  @retval     Return AC power state: 1 - AC present, 0 - AC not present.
**/
Method (RAPS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPWR)))
}

/**
  RMPT        Get max platform temprature.
  @param      None.
  @retval     Return max platform temprature value.
**/
Method (RMPT)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PLMX)))
}

/**
  RSSV        Get sensors value.
  @param      None.
  @retval     Return sensors value in package object:
              Index0      - TSR1 Sensor SEN1
              Index1      - TSR2 Sensor SEN2
              Index2      - TSR3 Sensor SEN3
              Index3      - TSR4 Sensor SEN4
              Index4      - TSR5 Sensor SEN5
**/
Method (RSSV)
{
  Name (TSPK, Buffer (5) {})
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSR1)), Index (TSPK, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSR2)), Index (TSPK, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSR3)), Index (TSPK, 2))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSR4)), Index (TSPK, 3))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSR5)), Index (TSPK, 4))

  Return (TSPK)
}

/**
  RBPM        Battery 1 Pmax in mW (LSB).
  @param      None.
  @retval     Return Battery 1 Pmax in mW (LSB).
**/
Method (RBPM)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BMAX)))
}

/**
  RPDP        Get PCH DTS reading from PCH.
  @param      None.
  @retval     Return PCH DTS value from PCH.
**/
Method (RPDP)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PPDT)))
}

/**
  RCPH        Get CPU PECI reading integer value (unit Celicus).
  @param      None.
  @retval     Return CPU PECI reading integer value (unit Celicus).
**/
Method (RCPH)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECH)))
}

/**
  RBPV        Get battery package value.
  @param      None.
  @retval     Return the battery value in package object:
              Index0        - B1DC
              Index1        - B1RC
              Index2        - B1FC
              Index3        - B1FV
              Index4        - B1DI
              Index5        - B1CI
              Index6        - B2RC - 0
              Index7        - B2FC - 0
              Index8        - B2FV - 0
              Index9        - B2DI - 0
              Index10       - B2CI - 0
**/
Method (RBPV)
{
  Name (BTPK, Package () {
    0x80000000, 0x80000000, 0x80000000, 0x80000000,
    0x80000000, 0x80000000, 0x80000000, 0x80000000,
    0x80000000, 0x80000000, 0x80000000
  })
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1DC)), Index (BTPK, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1RC)), Index (BTPK, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1FC)), Index (BTPK, 2))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1FV)), Index (BTPK, 3))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1DI)), Index (BTPK, 4))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1CI)), Index (BTPK, 5))
  //
  // BAT2 is unsupported, return 0 for BAT2 field
  Store (0, Index (BTPK, 6))
  Store (0, Index (BTPK, 7))
  Store (0, Index (BTPK, 8))
  Store (0, Index (BTPK, 9))
  Store (0, Index (BTPK, 10))
  Return (BTPK)
}

/**
  RPNV        Get PWM ending value.
  @param      None.
  @retval     Return PWM ending value.
**/
Method (RPNV)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PENV)))
}

/**
  ERXX        Read DDR fan in rpm.
  @param      None.
  @retval     Return DDR fan in rpm.
**/
Method (ERDF)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.DFSP)))
}

/**
  ERXX        Read GPU fan in rpm.
  @param      None.
  @retval     Return GPU fan in rpm.
**/
Method (ERGF)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.GFSP)))
}

/**
  RCFS        Get CPU fan speed.
  @param      None.
  @retval     Return CPU fan speed.
**/
Method (RCFS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFSP)))
}

/**
  RPRC        Get PECI repeat pacakges.
  @param      None.
  @retval     Return PECI repeat in buffer object:
              Index0      - PRC1
              Index1      - PRC2
              Index2      - PRC3
              Index3      - PRC4
              Index4      - PRPI
              Index5      - PECN
              Index6      - PCAD
              Index7      - PEWL
              Index8      - PWRL
              Index9      - PECD
              Index10     - PEHI
              Index11     - PECI
              Index12     - PEPL
              Index13     - PEPM
              Index14     - PWFC
              Index15     - PECC
              Index16     - PDT0
              Index17     - PDT1
              Index18     - PDT2
              Index19     - PDT3
              Index20     - PRFC
              Index21     - PRS0
              Index22     - PRS1
              Index23     - PRS2
              Index24     - PRS3
              Index25     - PRS4
**/
Method (RPRC)
{
  Name (PEPK, Buffer (26) {})
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC1)), Index (PEPK, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC2)), Index (PEPK, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC3)), Index (PEPK, 2))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC4)), Index (PEPK, 3))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRPI)), Index (PEPK, 4))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECN)), Index (PEPK, 5))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PCAD)), Index (PEPK, 6))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEWL)), Index (PEPK, 7))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PWRL)), Index (PEPK, 8))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECD)), Index (PEPK, 9))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEHI)), Index (PEPK, 10))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECI)), Index (PEPK, 11))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEPL)), Index (PEPK, 12))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEPM)), Index (PEPK, 13))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PWFC)), Index (PEPK, 14))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECC)), Index (PEPK, 15))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT0)), Index (PEPK, 16))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT1)), Index (PEPK, 17))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT2)), Index (PEPK, 18))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT3)), Index (PEPK, 19))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRFC)), Index (PEPK, 20))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS0)), Index (PEPK, 21))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS1)), Index (PEPK, 22))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS2)), Index (PEPK, 23))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS3)), Index (PEPK, 24))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS4)), Index (PEPK, 25))
  Return (PEPK)
}

/**
  RPES        Get PECI error counts.
  @param      None.
  @retval     Return PECI error counts in buffer object:
              Index0      - PEC0: Peci Error Count (LSB)
              Index1      - PEC1: Peci Error Count
              Index2      - PEC2: Peci Error Count
              Index3      - PEC3: Peci Error Count (MSB)
**/
Method (RPES)
{
  Name (PEPK, Buffer (4) {})
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEC0)), Index (PEPK, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEC1)), Index (PEPK, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEC2)), Index (PEPK, 2))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEC3)), Index (PEPK, 3))
  Return (PEPK)
}

/**
  RPBS        Get power battery steady state.
  @param      None.
  @retval     Return power battery steady state.
**/
Method (RPBS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PBSS)))
}

/**
  RBPS        Get battery packages.
  @param      None.
  @retval     Return battery packages:
              Index0    - RBHF: Battery high frequency impedance
              Index1    - VBNL: Battery No-Load Voltage
              Index2    - CMPP: Battery Maximum peak current
**/
Method (RBPS)
{
  Name (BPPK, Package () {
    0x80000000, 0x80000000, 0x80000000
  })
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RBHF)), Index (BPPK, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.VBNL)), Index (BPPK, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CMPP)), Index (BPPK, 2))
  Return (BPPK)
}

/**
  RFCS        Get fast charging enable state.
  @param      None.
  @retval     Return the enable state of fast charging.
**/
Method (RFCS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FCHG)))
}

/**
  ERSS        Read thermal sensor status.
  @param      None.
  @retval     Return thermal sensor status registers:
              BIT0:   SEN1
              BIT1:   SEN2
              BIT2:   SEN3
              BIT3:   SEN4
              BIT4:   SEN5
**/
Method (ERSS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSSR)))
}

/**
  RRPT        Get ram PWRT value.
  @Param      None.
  @retval     Return the PWRT value in ram.
**/
Method (RRPT)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PWRT)))
}

/**
  RWPP        Get the worst case rest of platform power in (W).
  @param      None.
  @retval     Return 2 bytes value of worst case rst of platform power in (W).
**/
Method (RWPP)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PROP)))
}

/**
  RCTP        Get the charge type.
  @param      None.
  @retval     Return 1 byte of charge type, Traditional or Hybrid.
**/
Method (RCTP)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTYP)))
}

/**
  RADR        Get the AC adpter rating in 10 mW.
  @param      None.
  @retval     Return 2 bytes value of AC adapter rating in 10 mW.
              Max adapter power supported. Because of space constraint
              can only send 2bytes=15000mW but system uses 150000mW
              hence bios will multiply this value by 10 and return.
**/
Method (RADR)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ARTG)))
}

/**
  RPOI        Get PPM->OPM message in and CCI indicator.
  @param      None.
  @retval     Return the PPM->OPM message in and CCI indicator in buffer object (20 bytes):
              Index0      - MGI0
              Index1      - MGI1
              Index2      - MGI2
              Index3      - MGI3
              Index4      - MGI4
              Index5      - MGI5
              Index6      - MGI6
              Index7      - MGI7
              Index8      - MGI8
              Index9      - MGI9
              Index10     - MGIA
              Index11     - MGIB
              Index12     - MGIC
              Index13     - MGID
              Index14     - MGIE
              Index15     - MGIF
              Index16     - CCI0
              Index17     - CCI1
              Index18     - CCI2
              Index19     - CCI3
**/
Method (RPOI)
{
  Name (PMBF, Buffer (20) {})
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI0)), Index (PMBF, 0))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI1)), Index (PMBF, 1))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI2)), Index (PMBF, 2))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI3)), Index (PMBF, 3))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI4)), Index (PMBF, 4))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI5)), Index (PMBF, 5))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI6)), Index (PMBF, 6))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI7)), Index (PMBF, 7))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI8)), Index (PMBF, 8))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGI9)), Index (PMBF, 9))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGIA)), Index (PMBF, 10))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGIB)), Index (PMBF, 11))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGIC)), Index (PMBF, 12))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGID)), Index (PMBF, 13))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGIE)), Index (PMBF, 14))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGIF)), Index (PMBF, 15))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CCI0)), Index (PMBF, 16))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CCI1)), Index (PMBF, 17))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CCI2)), Index (PMBF, 18))
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CCI3)), Index (PMBF, 19))
  Return (PMBF)
}

/**
  RDB1        Read the Battery1 state value from EC RAM.
  @param      None.
  @retval     Return Battery 1 state:
              bit2:0    - the battery state defined in ACPI spec for _BST.
              bit3      - the battery 1 device presentation state.
**/
Method (RDB1)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1ST)))
}

/**
  RSFC        Get Supported Fan Capability.
  @param      None.
  @retval     Return the supported fan capability.
**/
Method (RSFC)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFCS)))
}

/**
  RFPM        Read Fan Policy Mode.
  @param      None.
  @retval     Return currecnt fan policy mode.
**/
Method (RFPM)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFPM)))
}

/**
  RFTH        Get fan speed threshold in RPM.
  @param      None.
  @retval     Return    FAN RPM high threshold
**/
Method (RFTH)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FSTH)))
}

/**
  RFTL        Get fan speed threshold in RPM.
  @param      None.
  @retval     Return    FAN RPM low threshold.
**/
Method (RFTL)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FSTL)))
}

/**
  RSTH        Get fan speed threshold Hysterisis.
  @praram     None.
  @retvale    Return    Fan speed threshold Hysterisis.
**/
Method (RSTH)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FSHS)))
}

//
// write and commands.
//
/**
  STSN        Store sequence number.
  @param      Arg0  - 1 byte value to store the sequece number
  @retval     None.
**/
Method (STSN, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PBOK))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_PWR_BOSS_OK)
}

/**
  UPFH        Update the fan speed threshold in RPM.
  @param      Arg0  - conveys to the platform the fan speed High threshold to be set.
  @retval     None.
**/
Method (UPFH, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FSTH))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_RPM_TRIP_THRESHOLDS)
}

/**
  UPFL        Update the fan speed threshold in RPM.
  @param      Arg0  - conveys to the platform the fan speed Low threshold to be set.
  @retval     None.
**/
Method (UPFL, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.FSTL))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_RPM_TRIP_THRESHOLDS)
}

/**
  CPDC        Change power delovery capability.
  @param      Arg0  - 2 bytes value to change the power delivery capability.
  @retval     None.
**/
Method (CPDC, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, \_SB.PARENT_OF_LPCB.LPCB.H_EC.UVTH)
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_UVTH)
}

//
// Local variable under H_EC scope to optimize the PWM updating.
// FNAL - Fan setting already set.
// FANI - Fan Index record.
// LSFS - Last fan speed record.
// RQFS - Requested fan speed value.
//
Name (FNAL, 0)
Name (FANI, 0)
Name (LSFS, 0)
Name (RQFS, 0)
/**
  UPFS        Update the Fan speed in PWM.
  @param      Arg0  - 1 byte value to select the PWM port (low byte).
  @param      Arg1  - 1 byte value to change the FAN speed in percentage.
  @retval     None.
**/
Method (UPFS, 2, Serialized)
{
  If (LEqual (FNAL, 0)) {
    Store (1, FNAL)
    Store (Arg0, FANI)
    Store (RPNV (), LSFS)
    Store (Arg1, RQFS)
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PPSL))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (LSFS, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PINV))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PENV))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (100, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PSTP))
    ADBG (Concatenate ("Update fan PWM Index:", ToHexString (Arg0)))
    ADBG (Concatenate ("Update fan PWM Speed:", ToHexString (Arg1)))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_PWM)
  } Else {
    Store (0, Local0)   // Represent the fan ID change: 1 - fan id changed, 0 - fan id not changed.
    //
    // Update Fan Index if required.
    //
    If (LNotEqual (Arg0, FANI)) {
      ADBG (Concatenate ("Fan Id change to ", ToHexString (Arg0)))
      Store (1, Local0)
      Store (Arg0, FANI)
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PPSL))
    }
    //
    // Update PWM initial value if required.
    // RQFS now is not update, it is still the value of last request.
    //
    If (LNotEqual (RQFS, LSFS)) {
      ADBG (Concatenate ("Pre Fan PWM Update to ", ToHexString (RQFS)))
      Store (RQFS, LSFS)
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (RQFS, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PINV))
    }
    //
    // Update the PMW and send Ec command if new PWM requested.
    // Update the RQFS to the latest request.
    //
    If (LOr (LNotEqual (Arg1, RQFS), LEqual (Local0, 1))) {
      ADBG (Concatenate ("Update fan speed to ", ToHexString (Arg1)))
      Store (Arg1, RQFS)
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PENV))
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_PWM)
    }
  }
}

/**
  UPF2        Update the Fan speed in PWM.
  @param      Arg0  - 1 byte value to select the PWM port (low byte).
  @param      Arg1  - 1 byte value to change the FAN speed in percentage.
  @param      Arg2  - 1 byte value to update the previous speed level.
  @retval     None.
**/
Method (UPF2, 3, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PPSL))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg2, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PINV))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PENV))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (100, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PSTP))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UPDATE_PWM)
}

/**
  UPFP        Update Fan Policy Mode.
  @param      Arg0  - 1 byte value to update CFPM
  @retval     None.
**/
Method (UPFP, 1, Serialized)
{
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFCS)), Local0)
  If (And (Local0, 0x2)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFPM))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_SET_FAN_POLICY_MODE)
  }
}

/**
  UPCR        Update the charge rate
  @param      Arg0  - 2 bytes value to change the charge rate.
  @retval     None.
**/
Method (UPCR, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CHGR))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_CHARGE_RATE_CHANGE_NOTIFY)
}

/**
  CBTS        Chanbe button status, enable/disable the button.
  @param      Arg0  - 1 byte state value to change the button state:
                BIT0      - Power Button
                BIT1      - Windows Button
                BIT2      - Volume Up Button
                BIT3      - Volume Down Button
                BIT4      - Rotation Lock  Button
                BIT5-7    - Reserved
  @retval     None.
**/
Method (CBTS, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BTEN))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_BTN_SCI_CTL)
}

/**
  USTP        Update the sensor trip point.
  @param      Arg0  - 1 byte value to select the thermal sensor
  @param      Arg1  - 1 byte value to select Hysteresis.
  @param      Arg2  - 1 byte value to set sensor low trip point
  @param      Arg3  - Input to indicate whether it is Low trip point or High trip point
                       0 - Low trip point
                       1 - High trip point
  @retval     None.
**/
Method (USTP, 4, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSI))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg2, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.HYST))
  If (LEqual (Arg3, 0x00)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSLT)) // Sensor Low Trip Point
  } Else {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSHT)) // Sensor High Trip Point
  }
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_SET_TMP_THRESHOLD)
}

/**
  SPET        Start PECI test.
  @param      None.
  @retval     None.
**/
Method (SPET)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_START_PECI_TEST)
}

/**
  EPET        Stop/End PECI test.
  @param      None.
  @param      None.
**/
Method (EPET)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_STOP_PECI_TEST)
}

/**
  WPED        Write PECI data.
  @param      Arg0  - 26 bytes buffer data to write PECI data.
                Index0    - to PRC1
                Index1    - to PRC2
                Index2    - to PRC3
                Index3    - to PRC4
                Index4    - to PRPI
                Index5    - to PECN
                Index6    - to PCAD
                Index7    - to PEWL
                Index8    - to PWRL
                Index9    - to PECD
                Index10   - to PEHI
                Index11   - to PECI
                Index12   - to PEPL
                Index13   - to PEPM
                Index14   - to PWFC
                Index15   - to PECC
                Index16   - to PDT0
                Index17   - to PDT1
                Index18   - to PDT2
                Index19   - to PDT3
                Index20   - to PRFC
                Index21   - to PRS0
                Index22   - to PRS1
                Index23   - to PRS2
                Index24   - to PRS3
                Index25   - to PRS4
  @retval     None.
**/
Method (WPED, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 0)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC1))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 1)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC2))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 2)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC3))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 3)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRC4))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 4)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRPI))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 5)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECN))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 6)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PCAD))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 7)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEWL))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 8)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PWRL))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 9)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECD))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 10)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEHI))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 11)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECI))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 12)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEPL))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 13)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PEPM))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 14)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PWFC))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 15)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PECC))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 16)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT0))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 17)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT1))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 18)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT2))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 19)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PDT3))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 20)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRFC))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 21)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS0))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 22)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS1))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 23)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS2))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 24)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS3))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 25)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRS4))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_ACPI_PORT_WRT_PECI_DATA)
}

/**
  DPBN      Disable power button notify.
  @param    None.
  @retval   None.
**/
Method (DPBN)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_DIS_PWR_BTN_NOTIFY)
}

/**
  WOPM      Write to ECOPM.
  @param    Arg0  - 24 bytes OPM data to write to EC.
              Index0    - MGO0
              Index1    - MGO1
              Index2    - MGO2
              Index3    - MGO3
              Index4    - MGO4
              Index5    - MGO5
              Index6    - MGO6
              Index7    - MGO7
              Index8    - MGO8
              Index9    - MGO9
              Index10   - MGOA
              Index11   - MGOB
              Index12   - MGOC
              Index13   - MGOD
              Index14   - MGOE
              Index15   - MGOF
              Index16   - CTL0
              Index17   - CTL1
              Index18   - CTL2
              Index19   - CTL3
              Index20   - CTL4
              Index21   - CTL5
              Index22   - CTL6
              Index23   - CTL7
  @retval   None.
**/
Method (WOPM, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 0)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO0))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 1)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO1))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 2)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO2))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 3)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO3))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 4)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO4))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 5)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO5))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 6)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO6))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 7)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO7))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 8)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO8))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 9)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGO9))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 10)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOA))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 11)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOB))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 12)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOC))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 13)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOD))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 14)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOE))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 15)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.MGOF))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 16)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL0))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 17)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL1))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 18)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL2))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 19)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL3))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 20)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL4))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 21)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL5))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 22)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL6))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (DeRefOf (Index (Arg0, 23)), RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CTL7))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_UCSI_CMD_HNDLR)
}

/**
  PG3 Watch Dog Timer Setting.
  @param  Arg0  - Timer Index
  @param  Arg1  - Timer Value
  @retval None.
**/
Method (PGWD, 2)
{
  If(LNotEqual(Arg1,0xFFFFFFFF)) {
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf(\_SB.PARENT_OF_LPCB.LPCB.H_EC.WTMM))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg1, RefOf(\_SB.PARENT_OF_LPCB.LPCB.H_EC.WTMV))
    \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_P_G3_PROGRAM_COUNTER)
  }
}

/**
  Cleaning EC PG3 entry/exit bit witn PGED variable enable and battery connected.
**/
Method (PGCL, 0, Serialized)
{
  If (LEqual (PGED, 1)) {
    If (LEqual (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.RPWR)), 0)) {
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PG3E))
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PG3X))
    }
  }
}
// Notify Sleep State
//
// Handle all commands sent to EC by BIOS
//
//  Arguments: (1)
//    Arg0  -5 = Sx Exit Notify
//           4 = Sx Entry Notify
//           3 = Idle Resiliency Entry Notify
//           2 = Idle Resiliency Exit Notify
//           1 = CS Entry Notify
//           0 = CS Exit Notify
//  Return Value:
//    0x00 = Success
//    0xFF = Failure
//
Method (NTSS, 1, Serialized) {

  Switch (ToInteger (Arg0)) {

    Case (0) { // exit CS
      D8XH (0, 0xC5)
      D8XH (1, 0xAA)
      ADBG ("EC Exit CS")
      If (LEqual (ECNO, 0x01)) { // Check EC notification enabled in Setup
        ADBG ("EC Notify")
        Store (Acquire (HLDI, 0xFFFF), Local0) // Wait for Mutex for telling to exit Low Power Mode
        If (LEqual (Local0, Zero)) {
          \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_LOW_POWER_EXIT) // Notify EC of CS exit
          If (LEqual (ECLP, 0x1)) {
            Store (One, \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECUP)
          }
          Release (HLDI)
        }
      }
      Return (0x00)
    }

    Case (1) { // enter CS
      D8XH (0, 0xC5)
      D8XH (1, 0x01)
      ADBG ("Enter CS")

      If (LEqual (ECNO, 0x01)) { // Check EC notification enabled in Setup
        ADBG ("EC Notify")
        \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_LOW_POWER_ENTRY) // Notify EC of CS entry
        If (LEqual (ECLP, 0x1)) {
          Store (Zero, \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECUP)
        }

        If (CondRefOf (PPOE)) {
          If (LOr (LEqual (PPOE, 1), LEqual (PPOE, 2))) {
            Store (0, Local0) // Counter
            Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.EXAC)), Local1)  // Local 1 = EXAC
            While (LEqual (Local1, 0))
            {
              ADBG (Concatenate ("EXAC", ToHexString (Local1)))
              If (LGreater (Local0, 20)) {
                ADBG ("EC PS_ON ACK timeout")
                Break
              }
              Sleep (10)
              Increment (Local0) // Increment Timeout.
              Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.EXAC)), Local1)
            }
            If (LEqual (Local1, 1)) {
              Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.SCIP)), Local2) // Local2 = SCIP
              If (LEqual (Local2, 1)) {
                ADBG (Concatenate ("SCIP", ToHexString (Local2)))
                ADBG ("Checking pending PS_ON limit update")
                UAXL ()
              }
            }
          }
        }
      }
      Return (0x00)
    }

    Case (4) { // enter Sx
      ADBG ("EC Enter Sx")

      If (CondRefOf (PPOE)) {
        If (LEqual (PPOE, 2)) {
          If (CondRefOf (\_SB.PC00.LPCB.NRTS)) {
            \_SB.PC00.LPCB.NRTS (0)
          }
        }

        If (LOr (LEqual (PPOE, 1), LEqual (PPOE, 2))) {

          ADBG ("Notify")
          \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_SX_ENTER) // Notify of Sx entry
          If (LEqual (ECLP, 0x1)) {
            Store (Zero, \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECUP)
          }
          Store (0, Local0) // Counter
          Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.EXAC)), Local1)  // Local 1 = EXAC
          While (LEqual (Local1, 0))
          {
            ADBG (Concatenate ("EXAC", ToHexString (Local1)))
            If (Lgreater (Local0, 20)) {
              ADBG ("EC PS_ON ACK timeout")
              Break
            }
            Sleep (10)
            Increment (Local0) // Increment Timeout.
            Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.EXAC)), Local1)
          }
        }
      }
      Return (0x00)
    }

    Case (5) { // exit Sx
      ADBG ("EC Exit Sx")
      If (CondRefOf (PPOE)) {
        If (LOr (LEqual (PPOE, 1), LEqual (PPOE, 2))) {
          ADBG ("EC Notify")
          \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_SX_EXIT) // Notify of Sx Exit
          If (LEqual (ECLP, 0x1)) {
            Store (Zero, \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECUP)
          }
        }
        If (LEqual (PPOE, 2)) {
          If (CondRefOf (\_SB.PC00.LPCB.NRTS)) {
            \_SB.PC00.LPCB.NRTS (1)
          }
        }
      }
      Return (0x00)
    }

    Default {
      Return (0xFF)  // Error invalid argument
    }
  }
}

/**
  UPPM          Update the Programable Power Supply Mode.
  @param        1 byte data represent the PPS mode.
  @retval       0x0           - Success.
  @retval       None.
**/
Method (UPPM, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PPSM))
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECMD (EC_C_SET_PROGRAMMABLE_POWER_SUPPLY_MODE)
}

//
// EC write to update the EC ram.
//
/**
  UPCF      Update the cpu fan status in EC ram.
  @param    Arg0  - 1 byte status value to update the cpu fan in EC ram.
              1 - Fan on
              0 - Fan off
  @retval   None.
**/
Method (UPCF, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.CFAN))
}

/**
  UPTS      Update the thermal sensor status in EC ram.
  @param    Arg0  - 2 bytes value to update the thermal sensor status register in EC ram:
              BIT0:   SEN1
              BIT1:   SEN2
              BIT2:   SEN3
              BIT3:   SEN4
              BIT4:   SEN5
  @retval   None.
**/
Method (UPTS, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.TSSR))
}

/**
  UPLD      Update the Lid status in EC ram.
  @param    Arg0  - 1 byte date to update the Lid status.
  @retval   None.
**/
Method (UPLD, 1, Serialized)
{
  \_SB.PARENT_OF_LPCB.LPCB.H_EC.ECWT (Arg0, RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.LSTE))
}

/**
  NTLD           Notify LID Device.
**/
Method (NTLD, 1, Serialized)
{
  Notify (\_SB.PARENT_OF_LPCB.LPCB.H_EC.LID0, Arg0)
}

/**
  NTBT          Notify Battery Devices.
**/
Method (NTBT, 1, Serialized)
{
  Notify (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BAT0, Arg0)
  Notify (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BAT1, Arg0)
}

/**
  XDAT          XDCI Attached.
  @retval       1 - device attached. 0 - device detach.
**/
Method (XDAT, 0, Serialized)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.XDAT ())
}

#ifndef CONVERTIBLE_BUTTON
#define CONVERTIBLE_BUTTON   6
#endif
#ifndef DOCK_INDICATOR
#define DOCK_INDICATOR       7
#endif
/**
  UPBT      Update the button state
  @param    Arg0  - GBSX value. Virtual GPIO button Notify Sleep State Change.
  @param    Arg1  - PB1E value. 10sec Power button support.
                    Bit0: 10 sec P-button Enable/Disable
                    Bit1: Internal Flag
                    Bit2: Rotation Lock flag, 0:unlock, 1:lock
                    Bit3: Slate/Laptop Mode Flag, 0: Slate, 1: Laptop
                    Bit4: Undock / Dock Flag, 0: Undock, 1: Dock
                    Bit5: VBDL Flag. 0: VBDL is not called, 1: VBDL is called, Virtual Button Driver is loaded.
                    Bit7-6: Reserved for future use.
  @retval   None.
**/
Method (UPBT, 2, Serialized)
{
  If (And (Arg0, 0x40)) {
    If (And (Arg1, 0x08)) {
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPBT (CONVERTIBLE_BUTTON, One)
    } Else {
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPBT (CONVERTIBLE_BUTTON, Zero)
    }
  }

  If (And (Arg0, 0x80)) {
    If (And (Arg1, 0x10)) {
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPBT (DOCK_INDICATOR, One)
    } Else {
      \_SB.PARENT_OF_LPCB.LPCB.H_EC.UPBT (DOCK_INDICATOR, Zero)
    }
  }

  //
  // Prevent reinitializing Virtual GPIO Button status
  //
  Store (1, \_SB.PARENT_OF_LPCB.LPCB.H_EC.ONTM)
}

/**
  UPBP      Update the platform power participant.

  @param    Arg0  - Power State.
  @retval   Return the updated Power State.
**/
Method (UPBP, 1, Serialized)
{
  Store (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.B1ST)), Local0)
  Store (0, Local1)
  Store (ShiftRight (And (Local0, 0x08), 3), \_SB.PARENT_OF_LPCB.LPCB.H_EC.BNUM)
  ADBG (Concatenate ("BNUM resume from Sx: ", ToHexString (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BNUM)))
  If (LEqual (\_SB.PARENT_OF_LPCB.LPCB.H_EC.BNUM, 0)) {
    If (LNotEqual (\_SB.RVPS (), Arg0)) {
      Store (\_SB.RVPS (), Local1)
      // Perform needed ACPI Notifications.
      SPNT ()
      BPNT ()
    }
  } Else {
    If (LNotEqual (\_SB.RAPS (), Arg0)) {
      Store (\_SB.RAPS (), Local1)
      // Perform needed ACPI Notifications.
      SPNT ()
      BPNT ()
    }
  }

  Return (Local1)
}

/**
  NTPR      Notify the power button device to release.

  @param    Arg0  - Power Button State.
  @retval   None.
**/
Method (NTPR, 1, Serialized)
{
  //
  // If Using Control Method Power Button, notify PWRD device with 0x2
  //
  If (LEqual (\_SB.PWRB.PBST, 0x1)) {
    If (LEqual (Arg0, 1)) { //Power Button woke the system
      Notify (\_SB.PWRB, 0x02) // Send release notification to Power Button device 0x02
    }
  }
}

//
// PTID Methods start
//
External (\_TZ.TZ00._TMP, MethodObj)

/**
  Temperature Sensor Data Dump.

  @param  Arg0  - TSDB value.
  @param  Arg1  - Temperature Sersor value pacakge object.
  @retval The updated temperature sensor value package.
**/
Method (TSDD, 2, Serialized)
{
  Store (Add (Multiply (\_SB.RCPH (), 10), 2732), Local0)
  Store (\_SB.RSSV (), Local1)
  Store (\_SB.RBPM (), Local2)
  Store (\_SB.RPDP (), Local3)
  Store (\_SB.RCPH (), Local4)

  If (LEqual (Arg0, 1)) {
    Store (Local0, Index (Arg1, 0))
    Store (Local0, Index (Arg1, 1))
    Store (Local0, Index (Arg1, 2))
    Store (Local0, Index (Arg1, 3))
    Store (Local0, Index (Arg1, 4))
    Store (Add (DeRefOf (Index (Local1, 1)), 2732), Index (Arg1, 6))
    Store (Add (DeRefOf (Index (Local1, 2)), 2732), Index (Arg1, 7))
    Store (Add (DeRefOf (Index (Local1, 3)), 2732), Index (Arg1, 8))
    Store (Add (DeRefOf (Index (Local1, 4)), 2732), Index (Arg1, 9))
    Store (Add (Multiply (Local2, 10), 2732), Index (Arg1, 9))
    Store (Add (Multiply (Local3, 10), 2732), Index (Arg1, 10))
    Store (Add (Multiply (Local4, 10), 2732), Index (Arg1, 11))
  } Else {
    Store (Local0, Index (Arg1, 0))
    Store (Local0, Index (Arg1, 1))
    Store (Local0, Index (Arg1, 2))
    Store (Local0, Index (Arg1, 3))
    Store (Local0, Index (Arg1, 4))
    Store (Add (DeRefOf (Index (Local1, 0)), 2732), Index (Arg1, 5))
    Store (Add (DeRefOf (Index (Local1, 1)), 2732), Index (Arg1, 6))
    Store (Add (DeRefOf (Index (Local1, 2)), 2732), Index (Arg1, 7))
    Store (Add (DeRefOf (Index (Local1, 3)), 2732), Index (Arg1, 8))
    Store (Add (DeRefOf (Index (Local1, 4)), 2732), Index (Arg1, 9))
    Store (Add (Multiply (Local2, 10), 2732), Index (Arg1, 10))
    Store (Add (Multiply (Local3, 10), 2732), Index (Arg1, 11))
    Store (Add (Multiply (Local4, 10), 2732), Index (Arg1, 12))
  }

  If (LEqual (Arg0, 1)) {
    Store (\_TZ.TZ00._TMP, Index (Arg1, 13))
  }
  Else {
    Store (\_TZ.TZ00._TMP, Index (Arg1, 14))
  }
  Return (Arg1)
}

/**
  Power Sensor Data Dump.

  @param  Arg0  - Platform Power value package object.
  @retval The updated platform power value package.
**/
Method (PSDD, 1, Serialized)
{
  Store (0, Index (Arg0, 0))
  Multiply (0, 10, Index (Arg0, 1))
  Multiply (0, 10, Index (Arg0, 2))
  Store (\_SB.RBPV (), Local0)
  Store (DeRefOf (Index (Local0, 0)), Index (Arg0, 6))
  Store (DeRefOf (Index (Local0, 1)), Index (Arg0, 7))
  Store (DeRefOf (Index (Local0, 2)), Index (Arg0, 8))
  Store (DeRefOf (Index (Local0, 3)), Index (Arg0, 9))
  Store (DeRefOf (Index (Local0, 4)), Index (Arg0, 10))
  Store (DeRefOf (Index (Local0, 5)), Index (Arg0, 11))
  Store (DeRefOf (Index (Local0, 6)), Index (Arg0, 12))
  Store (DeRefOf (Index (Local0, 7)), Index (Arg0, 13))
  Store (DeRefOf (Index (Local0, 8)), Index (Arg0, 14))
  Store (DeRefOf (Index (Local0, 9)), Index (Arg0, 15))
  Store (DeRefOf (Index (Local0, 10)), Index (Arg0, 16))
  // convert PMAX from signed to unsigned integer
  // PMAX Value = (NOT(OR(FFFF0000, ECValue))+1) * 10.
  Store (\_SB.RBPM (), Local0)
  If (Local0){ // don't convert a zero
    Not (Or (0xFFFF0000, Local0, Local0), Local0) // make it a 32 bit value before inverting the bits
    Multiply (Add (1, Local0, Local0), 10, Local0) // add 1 and multiply by 10
  }
  Store (Local0, Index (Arg0, 17))
  // PMAX Value = (NOT(OR(FFFF0000, ECValue))+1) * 10.
  // PMAX is unsupported, return 0 directly.
  Store (0, Index (Arg0, 18))

  Return (Arg0)
}

/**
  Other Sensor Data Dump.

  @param  Arg0  - TSDB value.
  @param  Arg1  - Ohter Sensor Data pacakge object.
  @retval The updated other sensor data package.
**/
Method (OSDD, 2, Serialized)
{
  If (LEqual (Arg0, 1)) {
    Store (\_SB.RPNV(), Index (Arg1, 0))
    Store (\_SB.RCFS(), Index (Arg1, 1))
    Store (DeRefOf (Index (\_SB.RSSV (), 2)), Index (Arg1, 2))
  }
  Else {
    Store (\_SB.RPNV(), Index (Arg1, 0))
    Store (\_SB.RCFS(), Index (Arg1, 1))
  }
  Return (Arg1)
}

/**
  Read PECI Mailbox Data.

  @param  None.
  @retval 26 bytes PECI data.
**/
Method (RPMD, 0, Serialized)
{
  Return (\_SB.RPRC ())
}

/**
  Write PECI Mailbox Data.

  @param  Arg0 - 26 bytes PECI data to write.
  @retval None.
**/
Method (WPMD, 1, Serialized)
{
  \_SB.WPED (Arg0)
}

/**
  Issue PECI Command.

  @param  None.
  @retval None.
**/
Method (ISPC, 0, Serialized)
{
  \_SB.SPET ()
}

/**
  End PECI Command.

  @param  None.
  @retval None.
**/
Method (ENPC)
{
  \_SB.EPET ()
}

/**
  Read PECI Command Status.

  @param  None.
  @retval Return the PECI command status.
**/
Method (RPCS)
{
  Return (\_SB.PARENT_OF_LPCB.LPCB.H_EC.ECRD (RefOf (\_SB.PARENT_OF_LPCB.LPCB.H_EC.PRCS)))
}

/**
  Read PECI Error Count.

  @param  None.
  @retval Return the PECI error count.
**/
Method (RPEC)
{
  Store (0, Local0)

  Store (\_SB.RPES (), Local1)
  Or(Local0, DeRefOf (Index (Local1, 0)), Local0)
  Or(Local0, ShiftLeft (DeRefOf (Index (Local1, 1)), 8), Local0)
  Or(Local0, ShiftLeft (DeRefOf (Index (Local1, 2)), 16), Local0)
  Or(Local0, ShiftLeft (DeRefOf (Index (Local1, 3)), 24), Local0)

  Return (Local0)
}

//
// PTID Methods end
//
