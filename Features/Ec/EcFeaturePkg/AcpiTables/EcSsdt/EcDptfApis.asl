/** @file
  ACPI EC Dptf APIs that consumed by Dptf feature.

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

/**
  CPDC        Change power delovery capability.

  @param      Arg0  - 2 bytes value to change the power delivery capability.
  @retval     None.

**/
Method (CPDC, 1, Serialized)
{
  ECWT (Arg0, ^UVTH)
  ECMD (EC_C_UPDATE_UVTH)
}

/**
  UPCR        Update the charge rate

  @param      Arg0  - 2 bytes value to change the charge rate.
  @retval     None.

**/
Method (UPCR, 1, Serialized)
{
  ECWT (Arg0, RefOf (^CHGR))
  ECMD (EC_C_CHARGE_RATE_CHANGE_NOTIFY)
}

/**
  UPPM          Update the Programable Power Supply Mode.

  @param        1 byte data represent the PPS mode.
  @retval       0x0           - Success.
  @retval       None.
**/
Method (UPPM, 1, Serialized)
{
  ECWT (Arg0, RefOf (^PPSM))
  ECMD (EC_C_SET_PROGRAMMABLE_POWER_SUPPLY_MODE)
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
  Store (ECRD (RefOf (^RBHF)), Index (BPPK, 0))
  Store (ECRD (RefOf (^VBNL)), Index (BPPK, 1))
  Store (ECRD (RefOf (^CMPP)), Index (BPPK, 2))
  Return (BPPK)
}

/**
  RPBS        Get power battery steady state.

  @param      None.
  @retval     Return power battery steady state.

**/
Method (RPBS)
{
  Return (ECRD (RefOf (^PBSS)))
}

/**
  RCTP        Get the charge type.

  @param      None.
  @retval     Return 1 byte of charge type, Traditional or Hybrid.

**/
Method (RCTP)
{
  Return (ECRD (RefOf (^CTYP)))
}

/**
  RBPM        Battery 1 Pmax in mW (LSB).

  @param      None.
  @retval     Return Battery 1 Pmax in mW (LSB).

**/
Method (RBPM)
{
  Return (ECRD (RefOf (BMAX)))
}

/**
  RFCS        Get fast charging enable state.

  @param      None.
  @retval     Return the enable state of fast charging.

**/
Method (RFCS)
{
  Return (ECRD (RefOf (^FCHG)))
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
  ECWT (Arg0, RefOf (^PPSL))
  ECWT (Arg2, RefOf (^PINV))
  ECWT (Arg1, RefOf (^PENV))
  ECWT (100, RefOf (^PSTP))
  ECMD (EC_C_UPDATE_PWM)
}

/**
  RFTH        Get fan speed threshold in RPM.

  @param      None.
  @retval     Return    FAN RPM high threshold
**/
Method (RFTH)
{
  Return (ECRD (RefOf (^FSTH)))
}

/**
  RFTL        Get fan speed threshold in RPM.

  @param      None.
  @retval     Return    FAN RPM low threshold.
**/
Method (RFTL)
{
  Return (ECRD (RefOf (^FSTL)))
}

/**
  RSTH        Get fan speed threshold Hysterisis.

  @praram     None.
  @retvale    Return    Fan speed threshold Hysterisis.
**/
Method (RSTH)
{
  Return (ECRD (RefOf (^FSHS)))
}

/**
  UPFH        Update the fan speed threshold in RPM.
  @param      Arg0  - conveys to the platform the fan speed High threshold to be set.
  @retval     None.
**/
Method (UPFH, 1, Serialized)
{
  ECWT (Arg0, RefOf (^FSTH))
  ECMD (EC_C_UPDATE_RPM_TRIP_THRESHOLDS)
}

/**
  UPFL        Update the fan speed threshold in RPM.
  @param      Arg0  - conveys to the platform the fan speed Low threshold to be set.
  @retval     None.
**/
Method (UPFL, 1, Serialized)
{
  ECWT (Arg0, RefOf (^FSTL))
  ECMD (EC_C_UPDATE_RPM_TRIP_THRESHOLDS)
}

/**
  RSFC        Get Supported Fan Capability.

  @param      None.
  @retval     Return the supported fan capability.
**/
Method (RSFC)
{
  Return (ECRD (RefOf (^CFCS)))
}

/**
  RFPM        Read Fan Policy Mode.

  @param      None.
  @retval     Return currecnt fan policy mode.
**/
Method (RFPM)
{
  Return (ECRD (RefOf (^CFPM)))
}

/**
  UPFP        Update Fan Policy Mode.

  @param      Arg0  - 1 byte value to update CFPM
  @retval     None.
**/
Method (UPFP, 1, Serialized)
{
  Store (ECRD (RefOf (^CFCS)), Local0)
  If (And (Local0, 0x2)) {
    ECWT (Arg0, RefOf (^CFPM))
    ECMD (EC_C_SET_FAN_POLICY_MODE)
  }
}

/**
  RCFS        Get CPU fan speed.

  @param      None.
  @retval     Return CPU fan speed.

**/
Method (RCFS)
{
  Return (ECRD (RefOf (^CFSP)))
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
  Store (ECRD (RefOf (B1DC)), Index (BTPK, 0))
  Store (ECRD (RefOf (^B1RC)), Index (BTPK, 1))
  Store (ECRD (RefOf (^B1FC)), Index (BTPK, 2))
  Store (ECRD (RefOf (B1FV)), Index (BTPK, 3))
  Store (ECRD (RefOf (B1DI)), Index (BTPK, 4))
  Store (ECRD (RefOf (B1CI)), Index (BTPK, 5))
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
  RRPT        Get ram PWRT value.

  @Param      None.
  @retval     Return the PWRT value in ram.

**/
Method (RRPT)
{
  Return (ECRD (RefOf (^PWRT)))
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
  Return (ECRD (RefOf (^ARTG)))
}

/**
  RWPP        Get the worst case rest of platform power in (W).

  @param      None.
  @retval     Return 2 bytes value of worst case rst of platform power in (W).

**/
Method (RWPP)
{
  Return (ECRD (RefOf (^PROP)))
}

/**
  STSN        Store sequence number.

  @param      Arg0  - 1 byte value to store the sequece number
  @retval     None.

**/
Method (STSN, 1, Serialized)
{
  ECWT (Arg0, RefOf (^PBOK))
  ECMD (EC_C_PWR_BOSS_OK)
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
  Store (ECRD (RefOf (^TSR1)), Index (TSPK, 0))
  Store (ECRD (RefOf (^TSR2)), Index (TSPK, 1))
  Store (ECRD (RefOf (^TSR3)), Index (TSPK, 2))
  Store (ECRD (RefOf (^TSR4)), Index (TSPK, 3))
  Store (ECRD (RefOf (^TSR5)), Index (TSPK, 4))

  Return (TSPK)
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
  ECWT (Arg0, RefOf (^TSI))
  ECWT (Arg2, RefOf (^HYST))
  If (LEqual (Arg3, 0x00)) {
    ECWT (Arg1, RefOf (^TSLT)) // Sensor Low Trip Point
  } Else {
    ECWT (Arg1, RefOf (^TSHT)) // Sensor High Trip Point
  }
  ECMD (EC_C_SET_TMP_THRESHOLD)
}
