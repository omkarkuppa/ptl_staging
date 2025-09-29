/** @file
  DDR5 PMIC Register Definitions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

  This software and the related documents are Intel copyrighted materials,
  and your use of them is governed by the express license under which they
  were provided to you ("License"). Unless the License provides otherwise,
  you may not use, modify, copy, publish, distribute, disclose or transmit
  this software or the related documents without Intel's prior written
  permission.

  This software and the related documents are provided as is, with no
  express or implied warranties, other than those that are expressly stated
  in the License.

@par Specification Reference: PMIC5100 (JEDEC JESD301-2) and PMIC5200 (JEDEC JESD301-2) Specifications
**/

#ifndef  _MRC_PMIC_H
#define  _MRC_PMIC_H

//
// PMIC5100 Spec Rev 0.99c, the header file only contains PMIC registers that are used by MRC
//

#define PMIC_REG_04 0x04
typedef union {
  struct {
    UINT8 reserved : 4;
    /* R04 [3:0]: Reserved */

    UINT8 global_error_log_critical_temperature : 1;
    /*
     * R04 [4]: GLOBAL_ERROR_LOG_CRITICAL_TEMPERATURE
     * Global Error Log History for Critical Temperature
     * 0 = No Error Occurred
     * 1 = Error Occurred
     */

    UINT8 global_error_log_vin_bulk_over_votlage : 1;
    /*
     * R04 [5]: GLOBAL_ERROR_LOG_VIN_BULK_OVER_VOLTAGE
     * Global Error Log History for VIN_Bulk Over Voltage
     * 0 = No Error Occurred
     * 1 = Error Occurred
     */

    UINT8 global_error_log_buck_ov_or_uv : 1;
    /*
     * R04 [6]: GLOBAL_ERROR_LOG_BUCK_OV_OR_UV
     * Global Error Log History for Buck Regulator Output Over or Under Voltage
     * 0 = No Error Occurred
     * 1 = Error Occurred
     */

    UINT8 global_error_count : 1;
    /*
     * R04 [7]: GLOBAL_ERROR_COUNT
     * Global Error Count Since Last Erase Operation
     * 0 = No Error or Only 1 Error since last Erase operation
     * 1 = > 1 Error Count since last Erase operation
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_04_STRUCT;

#define PMIC_REG_05 0x05
typedef union {
  struct {
    UINT8 pmic_error_log : 3;
    /*
     * R05 [2:0]: PMIC_ERROR_LOG
     * PMIC Power On - High Level Status Bit to Indicate Last Known Power Cycle or System Reset
     * 000 = Normal Power On
     * 001 = Reserved
     * 010 = Buck Regulator Output Over or Under Voltage
     * 011 = Critical Temperature
     * 100 = VIN_Bulk Input Over Voltage
     * 101 = Reserved
     * 110 = Reserved
     * 111 = Reserved
     */

    UINT8 swc_power_good : 1;
    /*
     * R05 [3]: SWC_POWER_GOOD
     * PMIC Power On - SWC Power Not Good
     * 0 = Normal Power On
     * 1 = Power On - SWC Power Not Good
     */

    UINT8 swb_power_good : 1;
    /*
     * R05 [4]: SWB_POWER_GOOD
     * PMIC Power On - SWB Power Not Good
     * 0 = Normal Power On
     * 1 = Power On - SWB Power Not Good
     */

    UINT8 reserved5 : 1;
    /* R05 [5]: Reserved */

    UINT8 swa_power_good : 1;
    /*
     * R05 [6]: SWA_POWER_GOOD
     * PMIC Power On - SWA Power Not Good
     * 0 = Normal Power On
     * 1 = Power On - SWA Power Not Good
     */

    UINT8 reserved7 : 1;
    /* R05 [7]: Reserved */
  } Bits;
  UINT8 Data;
} PMIC_REG_05_STRUCT;

#define PMIC_REG_08 0x08
typedef union {
  struct {
    UINT8 vin_bulk_input_over_voltage_status : 1;
    /*
     * R08 [0]: VIN_BULK_INPUT_OVER_VOLTAGE_STATUS
     * VIN_Bulk Input Supply Over Voltage Status
     * 0 = No Over Voltage
     * 1 = Over Voltage
     */

    UINT8 reserved1 : 1;
    /* R08 [1]: Reserved */

    UINT8 swc_output_power_good_status: 1;
    /*
     * R08 [2]: SWC_OUTPUT_POWER_GOOD_STATUS
     * Switch Node C Output Power Good Status
     * 0 = Power Good
     * 1 = Power Not Good
     */

    UINT8 swb_output_power_good_status : 1;
    /*
     * R08 [3]: SWB_OUTPUT_POWER_GOOD_STATUS
     * Switch Node B Output Power Good Status
     * 0 = Power Good
     * 1 = Power Not Good
     */

    UINT8 reserved4 : 1;
    /* R08 [4]: Reserved */


    UINT8 swa_output_power_good_status : 1;
    /*
     * R08 [5]: SWA_OUTPUT_POWER_GOOD_STATUS
     * Switch Node A Output Power Good Status
     * 0 = Power Good
     * 1 = Power Not Good
     */

    UINT8 critical_temp_shutdown_status : 1;
    /*
     * R08 [6]: CRITICAL_TEMP_SHUTDOWN_STATUS
     * Critical Temperature Shutdown Status
     * 0 = No Critical Temperature Shutdown
     * 1 = Critical Temperature Shutdown
     */

    UINT8 reserved7 : 1;
    /* R08 [7]: Reserved */
  } Bits;
  UINT8 Data;
} PMIC_REG_08_STRUCT;

#define PMIC_REG_14 0x14
typedef union {
  struct {
    UINT8 global_clear_status : 1;
    /* R14 [0]: GLOBAL_CLEAR_STATUS
       Clear all status bits.
       1 = Clear all status bits
    */
    UINT8 reserved_1 : 1;
    UINT8 clear_vout_1v_output_power_good_status : 1;
    /* R14 [2]: CLEAR_VOUT_1.01V_OUTPUT_POWER_GOOD_STATUS
       Clear VOUT_1.01V Output Power Good Status.
       1 = Clear Register Table 143, "Register 0x33" [2]
    */
    UINT8 reserved_3 : 5;
  } Bits;
  UINT8 Data;
} PMIC_REG_14_STRUCT;

#define PMIC_REG_21 0x21
typedef union {
  struct {
    UINT8 swa_power_good_threshold_low_side_voltage_setting : 1;
    /*
     * R21 [0]:
     * SWA_POWER_GOOD_THRESHOLD_LOW_SIDE_VOLTAGE_SETTING
     * Switch Node A Output Threshold Low Side Voltage For Power Good Status
     * 0 = -5% from the setting in "Register 0x21," [7:1]
     * 1 = -7.5% from the setting in "Register 0x21, [7:1]
     */

    UINT8 swa_voltage_setting : 7;
    /*
     * R21 [7:1]: SWA_VOLTAGE_SETTING
     * Switch Node A Output Regulator Voltage Setting
     * 000 0000 = 800 mV
     * 000 0001 = 805 mV
     * 000 0010 = 810 mV
     * ...
     * 011 1100 = 1100 mV
     * ...
     * 111 1101 = 1425 mV
     * 111 1110 = 1430 mV
     * 111 1111 = 1435 mV
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_21_STRUCT;

#define PMIC_REG_23 0x23
typedef union {
  struct {
    UINT8 swd_power_good_threshold_low_side_voltage_setting : 1;
    /*
     * R21 [0]:
     * SWA_POWER_GOOD_THRESHOLD_LOW_SIDE_VOLTAGE_SETTING
     * Switch Node A Output Threshold Low Side Voltage For Power Good Status
     * 0 = -5% from the setting in "Register 0x21," [7:1]
     * 1 = -7.5% from the setting in "Register 0x21, [7:1]
     */

    UINT8 swd_voltage_setting : 7;
    /*
     * R21 [7:1]: SWD_VOLTAGE_SETTING
     * Switch Node A Output Regulator Voltage Setting
     * 000 0000 = 800 mV
     * 000 0001 = 805 mV
     * 000 0010 = 810 mV
     * ...
     * 011 1100 = 1100 mV
     * ...
     * 111 1101 = 1425 mV
     * 111 1110 = 1430 mV
     * 111 1111 = 1435 mV
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_23_STRUCT;

#define PMIC_REG_25 0x25
typedef union {
  struct {
    UINT8 swb_power_good_threshold_low_side_voltage_setting : 1;
    /*
     * R25 [0]: SWB_POWER_GOOD_THRESHOLD_LOW_SIDE_VOLTAGE_SETTING
     * Switch Node B Output Threshold Low Side Voltage For Power Good Status
     * 0 = -5% from the setting in Table 125, "Register 0x25," [7:1]
     * 1 = -7.5% from the setting in Table 125, "Register 0x25," [7:1]
     */

    UINT8 swb_voltage_setting : 7;
    /*
     * R25 [7:1]: SWB_VOLTAGE_SETTING
     * Switch Node B Output Regulator Voltage Setting
     * 000 0000 = 800 mV
     * 000 0001 = 805 mV
     * 000 0010 = 810 mV
     * ...
     * 011 1100 = 1100 mV
     * ...
     * 111 1101 = 1425 mV
     * 111 1110 = 1430 mV
     * 111 1111 = 1435 mV
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_25_STRUCT;

#define PMIC_REG_27 0x27
typedef union {
  struct {
    UINT8 swc_power_good_threshold_low_side_voltage_setting : 1;
    /*
     * R27 [0]: SWC_POWER_GOOD_THRESHOLD_LOW_SIDE_VOLTAGE_SETTING
     * Switch Node C Output Threshold Low Side Voltage For Power Good Status
     * 0 = -5% from the setting in Table 127, "Register 0x27," [7:1]
     * 1 = -7.5% from the setting in Table 127, "Register 0x27," [7:1]
    */

    UINT8 swc_voltage_setting : 7;
    /*
     * R27 [7:1]: SWC_VOLTAGE_SETTING
     * Switch Node C Output Regulator Voltage Setting
     * 000 0000 = 1500 mV
     * 000 0001 = 1505 mV
     * 000 0010 = 1510 mV
     * ...
     * 011 1100 = 1800 mV
     * ...
     * 111 1101 = 2125 mV
     * 111 1110 = 2130 mV
     * 111 1111 = 2135 mV
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_27_STRUCT;

#define PMIC_REG_2B 0x2B
typedef union {
  struct {

    UINT8 SWCStepSize                 : 1;
    /* R2B [0]: SWC_VOLTAGE_STEP_SIZE */

    UINT8 vout_1000mV_voltage_setting : 2;
    /*
     * R2B [2:1]: VOUT_1.0V_VOLTAGE_SETTING
     * VOUT 1.0 V LDO Voltage Setting
     * 00 = 0.9 V
     * 01 = 1.0 V
     * 10 = 1.1 V
     * 11 = 1.2 V
     */

    UINT8 SWBStepSize                 : 1;
    /* R2B [3]: SWB_VOLTAGE_STEP_SIZE */
    UINT8 SWDStepSize                 : 1;
    /* R2B [4]: SWD_VOLTAGE_STEP_SIZE */
    UINT8 SWAStepSize                 : 1;
    /* R2B [5]: SWA_VOLTAGE_STEP_SIZE */
    /*
     * 0: Step size is 5mv
     * 1: Step size is 10mv
     */

    UINT8 vout_1800mV_voltage_setting : 2;
    /*
     * R2B [7:6]: VOUT_1.8V_VOLTAGE_SETTING
     * VOUT 1.8 V LDO Output Voltage Setting
     * 00 = 1.7 V
     * 01 = 1.8 V
     * 10 = 1.9 V
     * 11 = 2.0 V
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_2B_STRUCT;

#define PMIC_REG_2C 0x2C
typedef union {
  struct {
    UINT8 reserved_0 : 1;
    /* R2C [0:0]: Reserved */

    UINT8 swd_output_soft_start_time : 3;
    /*
     * R2C [3:1]: SWD_OUTPUT_SOFT_START_TIME
     * SWD Output Regulator Soft Start Time After VR Enable
     * 000 = 1 ms
     * 001 = 2 ms
     * 010 = 4 ms
     * 011 = 6 ms
     * ...
     * 111 = 14 ms
     */

    UINT8 reserved_4 : 1;
    /* R2C [4:4]: Reserved */

    UINT8 swa_output_soft_start_time : 3;
    /*
     * R2C [7:5]: SWA_OUTPUT_SOFT_START_TIME
     * SWA Output Regulator Soft Start Time After VR Enable
     * 000 = 1 ms
     * 001 = 2 ms
     * 010 = 4 ms
     * 011 = 6 ms
     * ...
     * 111 = 14 ms
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_2C_STRUCT;

#define PMIC_REG_2D 0x2D
typedef union {
  struct {
    UINT8 reserved_0 : 1;
    /* R2D [0]: Reserved */

    UINT8 swc_output_soft_start_time : 3;
    /*
     * R2D [3:1]: SWC_OUTPUT_SOFT_START_TIME
     * SWC Output Regulator Soft Start Time After VR Enable
     * 000 = 1 ms
     * 001 = 2 ms
     * 010 = 4 ms
     * 011 = 6 ms
     * ...
     * 111 = 14 ms
     */

    UINT8 reserved_4 : 1;
    /* R2D [4]: Reserved */

    UINT8 swb_output_soft_start_time : 3;
    /*
     * R2D [7:5]: SWB_OUTPUT_SOFT_START_TIME
     * SWB Output Regulator Soft Start Time After VR Enable
     * 000 = 1 ms
     * 001 = 2 ms
     * 010 = 4 ms
     * 011 = 6 ms
     * ...
     * 111 = 14 ms
     */
  } Bits;
  UINT8 Data;
} PMIC_REG_2D_STRUCT;

#define PMIC_REG_2F 0x2F
typedef union {
  struct {
    UINT8 mask_bits_register_control : 2;
    /*
     * R2F [1:0]: MASK_BITS_REGISTER_CONTROL
     * Mask Bits Register Control6
     * 00 = Mask GSI_n Signal Only (PWR_GOOD Signal will assert)
     * 01 = Mask PWR_GOOD Signal Only (GSI_n Signal will assert)
     * 10 = Mask GSI_n and PWR_GOOD Signals (neither PWR_GOOD assert or GSI_n signal will
     * assert)
     * 11 = Reserved
     */

    UINT8 secure_mode : 1;
    /*
     * R2F [2]: SECURE_MODE
     * PMIC Mode Operation
     * 0 = Secure Mode Operation
     * 1 = Programmable Mode Operation
    */

    UINT8 swc_regulator_control : 1;
    /*
     * R2F [3]: SWC_REGULATOR_CONTROL
     * Disable SWC Regulator Output1,2
     * 0 = Disable Switch Node C Output Regulator
     * 1 = Enable Switch Node C Output Regulator
     */

    UINT8 swb_regulator_control : 1;
    /*
     * R2F [4]: SWB_REGULATOR_CONTROL
     * Disable SWB Regulator Output
     * 0 = Disable Switch Node B Output Regulator
     * 1 = Enable Switch Node B Output Regulator
     */

    UINT8 swd_regulator_control : 1;
    /* R2F [5]: SWD_REGULATOR_CONTROL
     * Disable SWD Regulator Output
     * 0 = Disable Switch Node D Output Regulator
     * 1 = Enable Switch Node D Output Regulator
     */

    UINT8 swa_regulator_control : 1;
    /*
     * R2F [6]: SWA_REGULATOR_CONTROL
     * Disable SWA Regulator Output1,2
     * 0 = Disable Switch Node A Output Regulator
     * 1 = Enable Switch Node A Output Regulator
     */

    UINT8 reserved_7 : 1;
    /* R2F [7]: Reserved */
  } Bits;
  UINT8 Data;
} PMIC_REG_2F_STRUCT;

#define PMIC_REG_32 0x32
typedef union {
  struct {
    UINT8 reserved : 3;
    /* R32 [2:0]: Reserved */

    UINT8 pmic_pwr_good_Output_signal_control : 2;
    /*
     * R32 [4:3]: PWR_GOOD_OUTPUT_SIGNAL_CONTROL
     * PMIC PWR_GOOD Output Signal Control
     * 0x = PMIC controls PWR_GOOD on its own based on internal status
     * 10 = PWR_GOOD Output Low
     * 11 = PWR_GOOD Output Floats
     */

    UINT8 pwr_good_io_type : 1;
    /*
     * R32 [5]: PWR_GOOD_IO_TYPE
     * PMIC PWR_GOOD Output Signal Type
     * 0 = Output Only
     * 1 = Input & Output
     */

    UINT8 management_interface_selection : 1;
    /*
     * R32 [6]: MANAGEMENT_INTERFACE_SELECTION
     * PMIC Management Bus Interface Protocol Selection
     * 0 = I2C Protocol (Max speed 1 MHz)
     * 1 = I3C Basic Protocol
     */

    UINT8 vr_enable : 1;
    /* R32 [7]: VR_ENABLE
     * PMIC Enable
     * 0 = PMIC Disable
     * 1 = PMIC Enable
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_32_STRUCT;

#define PMIC_REG_34 0x34
typedef union {
  struct {
    UINT8 reserved : 1;
    /* R34 [0]: Reserved */

    UINT8 hid_code : 3;
    /*
     * R34 [3:1]: HID_CODE
     * PMIC's 3-bit HID Code
     * 000
     * 001
     * 010
     * 011
     * 100
     * 101
     * 110
     * 111
     */

    UINT8 reserved_4 : 1;
    /* R34 [4]: Reserved */

    UINT8 parity_disable : 1;
    /*
     * R34 [5]: PARITY_DISABLE
     * T Bit Parity Code Disable (Applicable Only if Table 138, "Register 0x32" [6] = '1'.)
     * 0 = Enable
     * 1 = Disable4
    */

    UINT8 ibi_enable : 1;
    /*
     * R34 [6]: IBI_ENABLE
     * In Band Interrupt Enable (Applicable Only if Table 138, "Register 0x32" [6] = '1')
     * 0 = Disable
     * 1 = Enable
     */

    UINT8 pec_enable : 1;
    /*
     * R34 [7]: PEC_ENABLE
     * Packet Error Code Enable (Applicable Only if Table 138, "Register 0x32" [6] = '1')
     * 0 = Disable
     * 1 = Enable
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_34_STRUCT;

#define PMIC_REG_3B 0x3B
typedef union {
  struct {
    UINT8 reserved_0 : 1;
    /* R3B [0]: Reserved */

    UINT8 revision_id_minor_stepping : 3;
    /*
     * R3B [3:1]: REVISION_ID_MINOR_STEPPING
     * Minor Revision Stepping
     * 000 = Revision 0
     * 001 = Revision 1
     * 010 = Revision 2
     * 011 = Revision 3
     * All other encodings are reserved.
     */

    UINT8 revision_id_major_stepping : 2;
    /*
     * R3B [5:4]: REVISION_ID_MAJOR_STEPPING
     * Major Revision Stepping
     * 00 = Revision 1
     * 01 = Revision 2
     * 10 = Revision 3
     * 11 = Revision 4
     */

    UINT8 reserved_6 : 2;
    /* R3B [7:6]: Reserved */
  } Bits;
  UINT8 Data;
} PMIC_REG_3B_STRUCT;

#define PMIC_REG_3C 0x3C
typedef union {
  struct {
    UINT8 vendor_id_byte0 : 8;
    /* R3C [7:0]: VENDOR_ID_BYTE0
       Vendor Identification Register Byte 0
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_3C_STRUCT;

#define PMIC_REG_3D 0x3D
typedef union {
  struct {
    UINT8 vendor_id_byte1 : 8;
    /* R3D [7:0]: VENDOR_ID_BYTE1
       Vendor Identification Register Byte 1
    */
  } Bits;
  UINT8 Data;
} PMIC_REG_3D_STRUCT;

#define PMIC_REG_3E 0x3E
typedef union {
  struct {
    UINT8 Reset           : 1;
    /* R3E [0]: Reset to factory default setting
    */
    UINT8 reserved_1      : 7;
  } Bits;
  UINT8 Data;
} PMIC_REG_3E_STRUCT;

#define PMIC_REG_SWA_ADDR                         (PMIC_REG_21)
#define PMIC_REG_SWB_ADDR                         (PMIC_REG_25)
#define PMIC_REG_SWC_ADDR                         (PMIC_REG_27)
#define PMIC_REG_SWABC_CTL                        (PMIC_REG_2F)

// SWA/SWB/SWC voltage limitation in step size 5mv
#define PMIC_SWA_SWB_START_VOLTAGE                (800)
#define PMIC_SWA_SWB_LOWEST_VOLTAGE               (800)
#define PMIC_SWA_SWB_5MV_HIGHEST_VOLTAGE          (1435)
#define PMIC_SWA_SWB_10MV_HIGHEST_VOLTAGE         (2000)
#define PMIC_SWC_START_VOLTAGE                    (1500)
#define PMIC_SWC_LOWEST_VOLTAGE                   (1500)
#define PMIC_SWC_5MV_HIGHEST_VOLTAGE              (2135)
#define PMIC_SWC_10MV_HIGHEST_VOLTAGE             (2300) // It can be 2770, but just use 2300 for safety

/*
 * The max config idle is 24.
 * The value need be read from Reg 0x40, but
 * it's protected by password.
 * We use 2, and might tune it later on.
 * */
#define PMIC_POWER_ON_SEQUENCE_CONFIG_IDLE_MS     (2)

/**
  This function enables DDR5 DIMM's PMIC's programmable mode and send VR_ENABLE to PMIC

  @param[in, out] MrcData            - The MRC "global data" area.

  @retval VOID
**/
VOID
MrcEnableDimmPmic (
  IN OUT MrcParameters      *MrcData
  );

/**
  This function configures PMIC voltages.

  @param[in, out] MrcData - MRC global data.
**/
VOID
MrcPmicVoltageConfiguration (
  IN OUT MrcParameters      *MrcData
  );

#endif   // _MRC_PMIC_H

