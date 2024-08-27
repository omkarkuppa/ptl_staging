/** @file
  EC Private RAM offset definition.

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

@par Specification Reference:
**/

#ifndef _EC_RAM_OFFSET_H_
#define _EC_RAM_OFFSET_H_

#define MAX_PLATFORM_TEMPERATURE                      0x01    // Max Platform temprature
#define SENSOR_TEMPERATURE                            0x02    // Sensor Temperature
#define DEVICE_STATE                                  0x03    // Device State:
                                                              // BIT0   - Real AC Power (AC Present = 1)
                                                              // BIT2:1 - Reserved
                                                              // BIT3   - CPU Fan (Fan On = 1)
                                                              // BIT5:4 - Reserved
                                                              // BIT6   - Lid State (Lid Open = 1)
                                                              // BIT7   - Reserved
#define MESSAGE_IN_00                                 0x04    // PPM->OPM Message In, 160 bits, 20 bytes
#define MESSAGE_IN_01                                 0x05
#define MESSAGE_IN_02                                 0x06
#define MESSAGE_IN_03                                 0x07
#define MESSAGE_IN_04                                 0x08
#define MESSAGE_IN_05                                 0x09
#define MESSAGE_IN_06                                 0x0A
#define MESSAGE_IN_07                                 0x0B
#define MESSAGE_IN_08                                 0x0C
#define MESSAGE_IN_09                                 0x0D
#define MESSAGE_IN_0A                                 0x0E
#define MESSAGE_IN_0B                                 0x0F
#define MESSAGE_IN_0C                                 0x10
#define MESSAGE_IN_0D                                 0x11
#define MESSAGE_IN_0E                                 0x12
#define MESSAGE_IN_0F                                 0x13
#define MESSAGE_IN_10                                 0x14
#define MESSAGE_IN_11                                 0x15
#define MESSAGE_IN_12                                 0x16
#define MESSAGE_IN_13                                 0x17
#define MESSAGE_OUT_00                                0x18   // OPM->PPM Message Out, 128 bits, 16 bytes
#define MESSAGE_OUT_01                                0x19
#define MESSAGE_OUT_02                                0x1A
#define MESSAGE_OUT_03                                0x1B
#define MESSAGE_OUT_04                                0x1C
#define MESSAGE_OUT_05                                0x1D
#define MESSAGE_OUT_06                                0x1E
#define MESSAGE_OUT_07                                0x1F
#define MESSAGE_OUT_08                                0x20
#define MESSAGE_OUT_09                                0x21
#define MESSAGE_OUT_0A                                0x22
#define MESSAGE_OUT_0B                                0x23
#define MESSAGE_OUT_0C                                0x24
#define MESSAGE_OUT_0D                                0x25
#define MESSAGE_OUT_0E                                0x26
#define MESSAGE_OUT_0F                                0x27
#define CCI_INDICATOR_00                              0x28    // PPM->OPM CCI indicator, 4 bytes
#define CCI_INDICATOR_01                              0x29
#define CCI_INDICATOR_02                              0x2A
#define CCI_INDICATOR_03                              0x2B

#define EC_CRITICAL_TEMPERATURE                       0x2F    // EC Critical Temperature
#define DEVICE_STATE_02                               0x30    // Device State 02:
                                                              // BIT0   - Virtual Dock (Attached = 1)
                                                              // BIT2:1 - Reserved
                                                              // BIT3   - Power Button State (0 - Pressed)
                                                              // BIT4   - Virtual AC Power (AC Present = 1)
                                                              // BIT7:5 - Reserved
#define HOT_KEY_SCAN_CODE                             0x31    // Hot-Key Scan Code
#define BATTERY_1_STATUS                              0x32    // Battery 1 Status
                                                              // BIT0   - Battery is discharging
                                                              // BIT1   - Battery is charging
                                                              // BIT2   - Battery is in critical energy state
                                                              // BIT3   - Battery is present
                                                              // BIT4   - Virtual AC power is present
#define BATTERY_1_CURRENT_RATE                        0x33    // Battery 1 current rate
#define BATTERY_1_REMAINING_PERCENTAGE                0x34    // Battery 1 remaining percentage
#define BATTERY_1_VOLTAGE                             0x35    // Battery 1 voltage
#define BATTERY_2_STATUS                              0x36    // Battery 2 Status

#define KSC_COMMAND_REGISTER                          0x3A    // KSC command register

#define FAN_CAPABILITY_SUPPORTED                      0x40    // Fan capability supported
#define PWM_PORT_SELECT                               0x41    // PWM Port Select
#define PROGRAMMABLE_POWER_SUPPLY                     0x42    // Programmable Power Supply Mode
#define PWM_INITIAL_VALUE                             0x43    // PWM initial value
#define PWM_ENDING_VALUE                              0x44    // PWM ending value
#define PWM_DELAY                                     0x45    // PWM ms delay between steps
#define CURRENT_FAN_POLICY_MODE                       0x46    // Current fan policy mode
#define FAN_SPEED_HIGH                                0x47    // Fan speed high threshold in RPM, 2 bytes
#define FAN_SPEED_LOW                                 0x49    // Fan speed low threshold in RPM, 2 bytes
#define FAN_RPM_TRIP_STATUS                           0x4B    // Fan RPM trip status register:
                                                              // Responsible for triggering _QF2:
                                                              // BIT0   - TFN1
                                                              // BIT1   - TFN2
                                                              // BIT2   - TFN3
#define FAN_SPEED_THRESHOLD_HYSTERISIS                0x4C    // Fan speed threshold Hysterisis, 2 bytes
#define POWER_SOURCE_TYPE                             0x4E    // Power Source Type:
                                                              // BIT0   - AC
                                                              // BIT1   - USB-PD
                                                              // BIT2   - Programmable Power Supply
#define POWER_SOURCE_NUMBER                           0x4F    // PBOK offset where BIOS will update the Power Delivery State Change Sequence Number
#define THERMAL_SENSOR_BIT_INDEX                      0x50    // TemSnsrSelct Select Thermal Sensor
                                                              // 0      - SEN1
                                                              // 1      - SEN2
                                                              // 2      - SEN3
                                                              // 3      - SEN4
                                                              // 4      - SEN5
                                                              // 5      - SEN6
                                                              // 6      - SEN7
                                                              // 7      - SEN8
                                                              // 8      - SEN9
                                                              // 9      - SEN10
                                                              // A      - SEN11
                                                              // B      - SEN12
                                                              // C      - SEN13
                                                              // D      - SEN14
                                                              // E      - SEN15
                                                              // F      - SEN16
                                                              // Hysteresis selection is global and meant for all sensors
#define SENSOR_HIGH_TRIP_POINT                        0x51    // Sensor High Trip Point, 2 bytes
#define SENSOR_LOW_TRIP_POINT                         0x53    // Sensor Low Trip Point, 2 bytes
#define THERMAL_SENSOR_STATUS                         0x55    // Thermal sensor status register: 2 bytes
                                                              // Responsible for triggering _QF1:
                                                              // BIT0   - SEN1
                                                              // BIT1   - SEN2
                                                              // BIT2   - SEN3
                                                              // BIT3   - SEN4
                                                              // BIT4   - SEN5
#define BATTERY_1_DESIGN_CAPACITY                     0x57    // Battery 1 Design Capacity (mAh), 2 bytes
#define BATTERY_1_REMAINING_CAPACITY                  0x59    // Battery 1 Remaining Capacity (mAh), 2 bytes
#define BATTERY_1_FULL_CHARGE_CAPACITY                0x5B    // Battery 1 Full Charge Capacity (mAh), 2 bytes
#define BATTERY_1_FULL_RESOLUTION_VOLTAGE             0x5D    // Battery 1 Full Resolution Voltage (mV), 2 bytes
#define BATTERY_1_DISCHARGE_CURRENT                   0x5F    // Battery 1 Full Resolution Discharge Current (mA), 2 bytes
#define BATTERY_1_CHARGE_CURRENT                      0x61    // Battery 1 Full Resolution Charge Current (mA), 2 bytes

#define MIN_VOLTAGE                                   0x6D    // Min voltage below which platform activates OC protection and shuts down (mV), 2 bytes
#define WORST_CASE_REST_POWER                         0x6F    // Worst case rest of platform power in (W), 2 bytes
#define DDR_FAN_SPEED                                 0x71    // DDR Fan speed in rpm, 2 bytes
#define CPU_FAN_SPEED                                 0x73    // CPU Fan speed in rpm, 2 bytes
#define GPU_FAN_SPEED                                 0x75    // GPU Fan speed in rpm, 2 bytes

#define ACPI_CONCEPT_FLAG                             0x78    // ACPI Concept Flag:
                                                              // BIT0   - EC Connected Standby Debug LED (CAPS LOCK)
                                                              // BIT1   - 10 second power button
                                                              // BIT2   - Reset after AC removal
                                                              // BIT3   - Set to indicated PG3 exit after counter expiry
                                                              // BIT4   - Set to indicate PG3 entered successfully
                                                              // BIT7:5 - Reserved

#define AC_SOURCE_NOMINAL_VOLTAGE                     0x7A    // Returns the AC source nominal voltage in (mV), 2 bytes

#define AC_SOURCE_OPERATIONAL_CURRENT                 0x7D    // Returns the AC source operational current in (mA), 2 bytes

#define PCH_DIGITAL_THERMAL_SENSOR                    0x81    // PCH DTS(Digital Thermal Sensor) reading from PCH
#define CPU_PECI_READING_FRACTIONAL_VALUE             0x82    // CPU PECI reading fractional value (1/64 Celcius)
#define CPU_PECI_READING_INTEGER_VALUE                0x83    // CPU PECI reading integer value (unit Celicus)

#define LONG_TERM_BATTERY_CHARGE                      0x86    // Long term battery charge is based on c/20 SoC in %
#define AC_ADAPTER_RATING                             0x87    // AC adapter rating in 10 mW. Max adapter power supported.
                                                              // Because of EC-space constraint EC can only send 2bytes=15000mW
                                                              // but system uses 150000mW hence bios will multiply this value by 10 and return, 2 bytes
#define CHARGE_TYPE                                   0x89    // Charger type: Traditional or Hybrid
#define AC_SOURCE_1MS_PERIOD                          0x8A    // Returns AC source 1ms period percentage overload in 1% unit
#define AC_SOURCE_2MS_PERIOD                          0x8B    // Returns AC source 2ms period percentage overload in 1% unit
#define AC_SOURCE_10MS_PERIOD                         0x8C    // Returns AC source 10ms period percentage overload in 1% unit
#define BATTERY_MAX_SUSTAINED_POWER                   0x8D    // Max sustained power for battery (mW), 2 bytes
#define BIX_CYCLE_COUNT                               0x8F    // _BIX.Cycle Count, 2 bytes
#define THERMISTOR_SENSOR_1                           0x91    // Sensor SEN1, 2 bytes
#define THERMISTOR_SENSOR_2                           0x93    // Sensor SEN2, 2 bytes
#define THERMISTOR_SENSOR_3                           0x95    // Sensor SEN3, 2 bytes
#define THERMISTOR_SENSOR_4                           0x97    // Sensor SEN4, 2 bytes
#define THERMISTOR_SENSOR_5                           0x99    // Sensor SEN5, 2 bytes
#define PECI_REPEAT_CYCLE_1                           0x9B    // Peci Repeat Cycle, 4 bytes
#define PECI_REPEAT_CYCLE_2                           0x9C
#define PECI_REPEAT_CYCLE_3                           0x9D
#define PECI_REPEAT_CYCLE_4                           0x9E
#define PECI_REPEAT_PERIOD_INTERVAL                   0x9F    // Peci Repeat Period Interval
#define PECI_STOP_ON_ERROR                            0xA0    // Peci Error Count Number
#define PECI_PACKAGE                                  0xA1    // Peci Packages: 20 bytes
                                                              // BYTE0  - Peci Client Address
                                                              // BYTE1  - Write Length
                                                              // BYTE2  - Read Length
                                                              // BYTE3  - Command Code
                                                              // BYTE4  - Host ID
                                                              // BYTE5  - Index
                                                              // BYTE6  - Parameter (LSB)
                                                              // BYTE7  - Parameter (MSB)
                                                              // BYTE8  - Write FCS
                                                              // BYTE9  - Completion code
                                                              // BYTE10 - Data 0
                                                              // BYTE11 - Data 1
                                                              // BYTE12 - Data 2
                                                              // BYTE13 - Data 3
                                                              // BYTE14 - Read FCS
                                                              // BYTE15 - Reserved
                                                              // ...
                                                              // BYTE19 - Reserved

#define PECI_REPEAT_COMMAND_STATUS                    0xBB    // Peci Repeat Command Status
#define PECI_ERROR_COUNT                              0xBC    // Peci Error Count, 4 bytes
#define USBC_SOURCE_POWER_STATE                       0xC0    // USBC Source Power State, 2 bytes
                                                              // BIT0     - PSON Entry/Exit Acknowledge
                                                              // BIT1     - USBC Power SCI Pending
                                                              // BIT15:2  - USBC Total Source Power: Operationg wattage in 10mW granularity

#define WAKE_TIMER_MODE                               0xC3    // Wake timer mode (C3)
#define WAKE_TIMER_VALUE                              0xC4    // Wake time value (C4), 4 bytes
#define BUTTON_STATE                                  0xC9    // Button Enable/Disable
                                                              // BIT0   - Power Button
                                                              // BIT1   - Windows Button
                                                              // BIT2   - Volume Up Button
                                                              // BIT3   - Volume Down Button
                                                              // BIT4   - Rotation Lock Button
                                                              // BIT8:5 - Reserved
#define PPS_CHARGE_CURRENT                            0xCA    // PPS Charge Current (mA), 2 bytes
#define PPS_CHARGE_VOLTAGE                            0xCC    // PPS Charge Voltage (mV), 2 bytes

#define BATTERY_1_DESIGN_VOLTAGE                      0xD3    // Battery 1 Design Voltage (mV), 2 bytes

#define BATTERY_1_PMAX                                0xD7    // Battery 1 Pmax, 2 bytes

#define BATTERY_THRESHOLD                             0xDB    // Battery 1 Trip Point in %
#define BATTERY_TRIP_POINT_LOW                        0xDC    // Battery 1 Trip Point lower byte in mAh
#define BATTERY_TRIP_POINT_HIGH                       0xDD    // Battery 1 Trip Point higher byte in mAh
#define KEYBOARD_BRIGHTNESS                           0xDE    // Keyboard Brightness Percentage
#define BATTERY_CHARGE_RATE                           0xDF    // Battery Charge Rate, 2 bytes

#define CAS_HOT_KEY                                   0xE7    // Ctrl+Alt+Shit(CAS) + Key Scan Code

#define FAST_CHARGING_ENABLED                         0xE9    // Fast Charging Enabled
#define CONTROL_MESSAGE_00                            0xEA    // OPM->PPM Control message, 8 bytes
#define CONTROL_MESSAGE_01                            0xEB
#define CONTROL_MESSAGE_02                            0xEC
#define CONTROL_MESSAGE_03                            0xED
#define CONTROL_MESSAGE_04                            0xEE
#define CONTROL_MESSAGE_05                            0xEF
#define CONTROL_MESSAGE_06                            0xF0
#define CONTROL_MESSAGE_07                            0xF1
#define USBC_MAILBOX_COMMAND                          0xF2    // USB typeC mailbox command
#define USBC_MAILBOX_DATA                             0xF3    // USB typeC mailbox data
#define XDCI_STATE                                    0xF4    // XDCI State:
                                                              // BIT0   - VBUS Sense XDCI device attach/detach(1/0)
#define BATTERY_HIGH_FREQUENCY_IMPEDANCE              0xF5    // Battery high frequency impedance, 2 bytes
#define BATTERY_NO_LOAD_VOLTAGE                       0xF7    // Battery No-Load Voltage, 2 bytes
#define BATTERY_MAX_PEAK_CURRENT                      0xF9    // Battery Maximum peak current, 2 bytes
#define UNDER_VOLTAGE_THRESHOLD                       0xFB    // Under Voltage Threshold

#endif
