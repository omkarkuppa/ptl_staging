/** @file

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

#ifndef _SETUP_VARIABLE_H_
#define _SETUP_VARIABLE_H_

#include <Library/PchLimits.h>
#include <TcssInfo.h>
#include <OcLimits.h>
#include <CommonDefinitions.h>
#include <CpuPowerMgmt.h>

#define VMD_MAX_DEVICES 31
#ifndef MAX_CUSTOM_P_STATES
#define MAX_CUSTOM_P_STATES 40
#endif // MAX_CUSTOM_P_STATES
#ifndef MAX_16_CUSTOM_P_STATES
#define MAX_16_CUSTOM_P_STATES 16
#endif // MAX_16_CUSTOM_P_STATES

#define MAX_IIO_PCI_EXPRESS_ROOT_PORTS    4
#define MAX_PCI_ROOT_BRIDGES              1
#define SATA_MODE_AHCI                    0
#define SATA_MODE_RAID                    1
#define SATA_TEST_MODE_ENABLE             1
#define NON_CS_DEVICES                    9
#define I2C0_SENSOR_DEVICES               7
#define I2C1_SENSOR_DEVICES               13
#define SERIAL_IO_I2C0                    0x0
#define SERIAL_IO_I2C1                    0x1
#define SERIAL_IO_I2C2                    0x2
#define SERIAL_IO_I2C3                    0x3
#define SERIAL_IO_I2C4                    0x4
#define SERIAL_IO_I2C5                    0x5
#define SERIAL_IO_I2C6                    0x6
#define SERIAL_IO_I2C7                    0x7

#define SERIAL_IO_SPI0                    0x0
#define SERIAL_IO_SPI1                    0x1
#define SERIAL_IO_SPI2                    0x2
#define SERIAL_IO_SPI3                    0x3
#define SERIAL_IO_SPI4                    0x4
#define SERIAL_IO_SPI5                    0x5
#define SERIAL_IO_SPI6                    0x6

#define SERIAL_IO_UART0                   0x0
#define SERIAL_IO_UART1                   0x1
#define SERIAL_IO_UART2                   0x2
#define SERIAL_IO_UART3                   0x3
#define SERIAL_IO_UART4                   0x4
#define SERIAL_IO_UART5                   0x5
#define SERIAL_IO_UART6                   0x6

//
// This is 8 Bits Map of I2C Devices for combined use in BoardConfigPatchTable.h and in PchSetup.hfr
//
#define SERIAL_IO_I2C_TOUCHPAD            0x1
#define SERIAL_IO_I2C_TOUCHPANEL          0x2
//#define 3TH_SERIAL_IO_I2C_DEVICE        0x4
//#define 4TH_SERIAL_IO_I2C_DEVICE       0x08
//#define 5TH_SERIAL_IO_I2C_DEVICE       0x10
//#define 6TH_SERIAL_IO_I2C_DEVICE       0x20
//#define 7TH_SERIAL_IO_I2C_DEVICE       0x40
//#define 8TH_SERIAL_IO_I2C_DEVICE       0x80

//
// This is 8 Bits Map of SPI Devices for combined use in BoardConfigPatchTable.h and in PchSetup.hfr
//
//#define 2nd_SERIAL_IO_SPI_DEVICE       0x02
//#define 3rd_SERIAL_IO_SPI_DEVICE       0x04
//#define 4TH_SERIAL_IO_SPI_DEVICE       0x08
//#define 5TH_SERIAL_IO_SPI_DEVICE       0x10
//#define 6TH_SERIAL_IO_SPI_DEVICE       0x20
//#define 7TH_SERIAL_IO_SPI_DEVICE       0x40
//#define 8TH_SERIAL_IO_SPI_DEVICE       0x80

#define HDAUDIO_FEATURES                    12
#define HDAUDIO_PP_MODULES                  32

#define HDAUDIO_DMIC_CLOCK_SELECT_BOTH      0
#define HDAUDIO_DMIC_CLOCK_SELECT_CLKA      1
#define HDAUDIO_DMIC_CLOCK_SELECT_CLKB      2

#define GUID_CHARS_NUMBER                   37 // 36 GUID chars + null termination
#define FIVR_RAIL_S0IX_SX_STATES_MAX       5  // Number of availabe S0ix/Sx policy bits in FIVR Rail configuration
#define FIVR_RAIL_EXT_VOLTAGE_STATES_MAX   4  // Number of supported voltage settings for FIVR Rail configuration

#define SETUP_MAX_USB2_PORTS              16
#define SETUP_MAX_USB3_PORTS              10

// DTT EC supported Fans & Sensors
#define MAX_EC_SENSORS                    5
#define MAX_EC_FANS                       3

#define OPROM_EFI                         1
#define OPROM_LEGACY                      0

#define MIPICAM_I2C_DEVICES_COUNT         12
#define MIPICAM_GPIO_COUNT                6
#define MIPICAM_MODULE_NAME_LENGTH        16
#define MIPICAM_HID_MIN_LENGTH            6
#define MIPICAM_HID_LENGTH_PTLS                20

#define DTBT_CONTROLLER_SUPPORTED         2
#define ITBT_ROOT_PORTS_SUPPORTED         4
#define ITBT_HOST_DMA_SUPPORTED           2
#define TCSS_PORT0                        0
#define TCSS_PORT1                        1
#define TCSS_PORT2                        2
#define TCSS_PORT3                        3

#define CPU_FM_ADL_ULT_ULX 0x00000670

#define IBECC_REGIONS_MAX                 8

// Need to keep in sync with values in OneSiliconPkg\IpBlock\MemoryInit\src\IncludePublic\Ptl\CMrcExtTypes.h
#define MRC_MAX_RCOMP_TARGETS  5
#define MRC_PPR_REQUEST_MAX    2

#define MRC_TOTAL_DIMMODT_ENTRIES         22

#define MRC_CONTROLLER_DIMM_TAP1_NUM      8
#define MRC_CONTROLLER_DIMM_TAP2_NUM      8

#define MAX_VR_NUM             6

#ifndef TURBO_RATIO_LIMIT_ARRAY_SIZE
#define TURBO_RATIO_LIMIT_ARRAY_SIZE 8
#endif

#ifndef MAX_PCIE_CONTROLLERS
#define MAX_PCIE_CONTROLLERS                10
#endif

#define SERIAL_NUMBER_SIZE                  17

#pragma pack(1)
/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Add StcPeciInterfaceEnable,
                        StcTargetTemp[3], StcEnable[3],
                        StcGain[3], StcMinPerfLevel[3].
  <b>Revision 3</b>:  - Add IpfEnable
  <b>Revision 4</b>:  - Remove ECBaseCsPwrPolicy.
  <b>Revision 5</b>:  - Add OSSE Pep constraint
  <b>Revision 6</b>:  - Added StorageDynamicLinkManagement
  <b>Revision 7</b>:  - Added TccMode
  <b>Revision 8</b>:  - Deprecated wwan related variable
  <b>Revision 9</b>:  - Add IoI2c and IoSpi WittDevice
  <b>Revision 10</b>  - Added platform serial number variable
**/

#define SETUP_DATA_REVISION 10

typedef struct {
  UINT8   Revision;
  UINT8   FastBoot;
  UINT8   FastBootWdt;
  UINT8   SkipHIIUpdate;
  UINT8   ConInBehavior;
  UINT8   DisplayBootMode;
  UINT8   Numlock;
  UINT8   InteractiveText;
  UINT8   LazyConIn;
  UINT8   VirtualKeyboard;
  UINT8   FirmwareConfiguration;
  // General PCI Settings: [] - default
  UINT8   PciLatency; //[32]\ 64 \ 96 \ 128 \ 160 \ 192 \ 224 \ 248
  UINT8   AcpiAuto;
  UINT8   AcpiSleepState;
  UINT8   AcpiHibernate;
  UINT8   AcpiTableRevision;
  UINT8   PciExpNative;
  UINT8   NativeAspmEnable;
  UINT8   PtidSupport;
  // AcpiDebug Setup Options
  UINT8   AcpiDebug;
  UINT32  AcpiDebugAddress;
  UINT8   SerialPortAcpiDebug;
  UINT8   PciDelayOptimizationEcr;
  // S5 RTC wakeup setup options
  UINT8   WakeOnRTCS5;
  UINT8   PeciAccessMethod;
  UINT8   LowPowerS0Idle;
  UINT8   PepCpu;
  UINT8   PepGfx;
  UINT8   PepSataContraints;
  UINT8   PepUart;
  UINT8   PepI2c0;
  UINT8   PepI2c1;
  UINT8   PepI2c2;
  UINT8   PepI2c3;
  UINT8   PepI2c4;
  UINT8   PepI2c5;
  UINT8   PepSpi;
  UINT8   PepXhci;
  UINT8   PepAudio;
  UINT8   PepEmmc;
  UINT8   PepSdxc;
  UINT8   PepIpu;
  UINT8   PepGna;
  UINT8   PepCsme;
  UINT8   PepGbe;
  UINT8   PepThc0;
  UINT8   PepThc1;
  UINT8   PepTcss;
  UINT8   CSNotifyEC;
  UINT8   CSDebugLightEC;
  UINT8   EcLowPowerMode;
  UINT8   EcPeciMode;
  UINT8   EcDebugLed;
  UINT8   Reserved_ECBaseCsPwrPolicy;
  UINT8   SensorStandby;
  UINT8   PL1LimitCS;
  UINT16  PL1LimitCSValue;
  UINT8   TenSecondPowerButtonSupport;
  UINT8   TenSecondPowerButtonEnable;
  UINT8   LpitResidencyCounter;
  UINT8   PseudoG3State;
  // Reading SSDT table from file
  UINT8   LoadSSDTFromFile;
  //
  // On Screen Branding
  //
  UINT16  OperatingSystem;
  UINT16  OemBadgingBrand;
  UINT8   BootFirstToShell;

  UINT8   SensorHubType;
  UINT8   DebugUsbUartSupport;
  UINT8   DebugUsbUart;
  UINT8   EfiNetworkSupport;
  UINT8   TcssPdPsOnEnable;
  UINT8   UsbSensorHub;

  UINT8   HddAcousticPowerManagement;
  UINT8   HddAcousticMode;

  //
  // Recovery Configuration
  //
  UINT8   IsRecoveryJumper;

  UINT8   PchI2cSensorDevicePort[PCH_MAX_SERIALIO_I2C_CONTROLLERS];

  UINT8   PchI2cTouchPadType;
  UINT8   PchI2cTouchPadIrqMode;
  UINT8   PchI2cTouchPadBusAddress;
  UINT16  PchI2cTouchPadHidAddress;
  UINT8   PchI2cTouchPadSpeed;

  UINT8   PchI2cTouchPanelType;
  UINT8   PchI2cTouchPanelIrqMode;
  UINT8   PchI2cTouchPanelBusAddress;
  UINT16  PchI2cTouchPanelHidAddress;
  UINT8   PchI2cTouchPanelSpeed;

  UINT8   PchSpiSensorDevicePort[PCH_MAX_SERIALIO_SPI_CONTROLLERS];

  UINT8   PchUart1SensorDevice;
  UINT8   PchUart2SensorDevice;
  UINT8   PchI2cWittDevice;
  UINT8   PchI2cWittVersion;
  UINT8   PchUartUtkDevice;

  UINT8   PchSerialIoUseTimingParameters;

  UINT16  PchSerialIoTimingSSH0;  // [SSH0] SSCN-HIGH for I2C0
  UINT16  PchSerialIoTimingSSL0;  // [SSL0] SSCN-LOW  for I2C0
  UINT16  PchSerialIoTimingSSD0;  // [SSD0] SSCN-HOLD for I2C0
  UINT16  PchSerialIoTimingFMH0;  // [FMH0] FMCN-HIGH for I2C0
  UINT16  PchSerialIoTimingFML0;  // [FML0] FMCN-LOW  for I2C0
  UINT16  PchSerialIoTimingFMD0;  // [FMD0] FMCN-HOLD for I2C0
  UINT16  PchSerialIoTimingFPH0;  // [FPH0] FPCN-HIGH for I2C0
  UINT16  PchSerialIoTimingFPL0;  // [FPL0] FPCN-LOW  for I2C0
  UINT16  PchSerialIoTimingFPD0;  // [FPD0] FPCN-HOLD for I2C0
  UINT16  PchSerialIoTimingHSH0;  // [HSH0] HSCN-HIGH for I2C0
  UINT16  PchSerialIoTimingHSL0;  // [HSL0] HSCN-LOW  for I2C0
  UINT16  PchSerialIoTimingHSD0;  // [HSD0] HSCN-HOLD for I2C0

  UINT16  PchSerialIoTimingSSH1;  // [SSH1] SSCN-HIGH for I2C1
  UINT16  PchSerialIoTimingSSL1;  // [SSL1] SSCN-LOW  for I2C1
  UINT16  PchSerialIoTimingSSD1;  // [SSD1] SSCN-HOLD for I2C1
  UINT16  PchSerialIoTimingFMH1;  // [FMH1] FMCN-HIGH for I2C1
  UINT16  PchSerialIoTimingFML1;  // [FML1] FMCN-LOW  for I2C1
  UINT16  PchSerialIoTimingFMD1;  // [FMD1] FMCN-HOLD for I2C1
  UINT16  PchSerialIoTimingFPH1;  // [FPH1] FPCN-HIGH for I2C1
  UINT16  PchSerialIoTimingFPL1;  // [FPL1] FPCN-LOW  for I2C1
  UINT16  PchSerialIoTimingFPD1;  // [FPD1] FPCN-HOLD for I2C1
  UINT16  PchSerialIoTimingHSH1;  // [HSH1] HSCN-HIGH for I2C1
  UINT16  PchSerialIoTimingHSL1;  // [HSL1] HSCN-LOW  for I2C1
  UINT16  PchSerialIoTimingHSD1;  // [HSD1] HSCN-HOLD for I2C1

  UINT16  PchSerialIoTimingSSH2;  // [SSH2] SSCN-HIGH for I2C2
  UINT16  PchSerialIoTimingSSL2;  // [SSL2] SSCN-LOW  for I2C2
  UINT16  PchSerialIoTimingSSD2;  // [SSD2] SSCN-HOLD for I2C2
  UINT16  PchSerialIoTimingFMH2;  // [FMH2] FMCN-HIGH for I2C2
  UINT16  PchSerialIoTimingFML2;  // [FML2] FMCN-LOW  for I2C2
  UINT16  PchSerialIoTimingFMD2;  // [FMD2] FMCN-HOLD for I2C2
  UINT16  PchSerialIoTimingFPH2;  // [FPH2] FPCN-HIGH for I2C2
  UINT16  PchSerialIoTimingFPL2;  // [FPL2] FPCN-LOW  for I2C2
  UINT16  PchSerialIoTimingFPD2;  // [FPD2] FPCN-HOLD for I2C2
  UINT16  PchSerialIoTimingHSH2;  // [HSH2] HSCN-HIGH for I2C2
  UINT16  PchSerialIoTimingHSL2;  // [HSL2] HSCN-LOW  for I2C2
  UINT16  PchSerialIoTimingHSD2;  // [HSD2] HSCN-HOLD for I2C2

  UINT16  PchSerialIoTimingSSH3;  // [SSH3] SSCN-HIGH for I2C3
  UINT16  PchSerialIoTimingSSL3;  // [SSL3] SSCN-LOW  for I2C3
  UINT16  PchSerialIoTimingSSD3;  // [SSD3] SSCN-HOLD for I2C3
  UINT16  PchSerialIoTimingFMH3;  // [FMH3] FMCN-HIGH for I2C3
  UINT16  PchSerialIoTimingFML3;  // [FML3] FMCN-LOW  for I2C3
  UINT16  PchSerialIoTimingFMD3;  // [FMD3] FMCN-HOLD for I2C3
  UINT16  PchSerialIoTimingFPH3;  // [FPH3] FPCN-HIGH for I2C3
  UINT16  PchSerialIoTimingFPL3;  // [FPL3] FPCN-LOW  for I2C3
  UINT16  PchSerialIoTimingFPD3;  // [FPD3] FPCN-HOLD for I2C3
  UINT16  PchSerialIoTimingHSH3;  // [HSH3] HSCN-HIGH for I2C3
  UINT16  PchSerialIoTimingHSL3;  // [HSL3] HSCN-LOW  for I2C3
  UINT16  PchSerialIoTimingHSD3;  // [HSD3] HSCN-HOLD for I2C3

  UINT16  PchSerialIoTimingSSH4;  // [SSH4] SSCN-HIGH for I2C4
  UINT16  PchSerialIoTimingSSL4;  // [SSL4] SSCN-LOW  for I2C4
  UINT16  PchSerialIoTimingSSD4;  // [SSD4] SSCN-HOLD for I2C4
  UINT16  PchSerialIoTimingFMH4;  // [FMH4] FMCN-HIGH for I2C4
  UINT16  PchSerialIoTimingFML4;  // [FML4] FMCN-LOW  for I2C4
  UINT16  PchSerialIoTimingFMD4;  // [FMD4] FMCN-HOLD for I2C4
  UINT16  PchSerialIoTimingFPH4;  // [FPH4] FPCN-HIGH for I2C4
  UINT16  PchSerialIoTimingFPL4;  // [FPL4] FPCN-LOW  for I2C4
  UINT16  PchSerialIoTimingFPD4;  // [FPD4] FPCN-HOLD for I2C4
  UINT16  PchSerialIoTimingHSH4;  // [HSH4] HSCN-HIGH for I2C4
  UINT16  PchSerialIoTimingHSL4;  // [HSL4] HSCN-LOW  for I2C4
  UINT16  PchSerialIoTimingHSD4;  // [HSD4] HSCN-HOLD for I2C4

  UINT16  PchSerialIoTimingSSH5;  // [SSH5] SSCN-HIGH for I2C5
  UINT16  PchSerialIoTimingSSL5;  // [SSL5] SSCN-LOW  for I2C5
  UINT16  PchSerialIoTimingSSD5;  // [SSD5] SSCN-HOLD for I2C5
  UINT16  PchSerialIoTimingFMH5;  // [FMH5] FMCN-HIGH for I2C5
  UINT16  PchSerialIoTimingFML5;  // [FML5] FMCN-LOW  for I2C5
  UINT16  PchSerialIoTimingFMD5;  // [FMD5] FMCN-HOLD for I2C5
  UINT16  PchSerialIoTimingFPH5;  // [FPH5] FPCN-HIGH for I2C5
  UINT16  PchSerialIoTimingFPL5;  // [FPL5] FPCN-LOW  for I2C5
  UINT16  PchSerialIoTimingFPD5;  // [FPD5] FPCN-HOLD for I2C5
  UINT16  PchSerialIoTimingHSH5;  // [HSH5] HSCN-HIGH for I2C5
  UINT16  PchSerialIoTimingHSL5;  // [HSL5] HSCN-LOW  for I2C5
  UINT16  PchSerialIoTimingHSD5;  // [HSD5] HSCN-HOLD for I2C5

  UINT16  PchSerialIoTimingM0C0;  // [M0C0] M0D3 for I2C0
  UINT16  PchSerialIoTimingM1C0;  // [M1C0] M1D3 for I2C0
  UINT16  PchSerialIoTimingM0C1;  // [M0C1] M0D3 for I2C1
  UINT16  PchSerialIoTimingM1C1;  // [M1C1] M1D3 for I2C1
  UINT16  PchSerialIoTimingM0C2;  // [M0C2] M0D3 for I2C2
  UINT16  PchSerialIoTimingM1C2;  // [M1C2] M1D3 for I2C2
  UINT16  PchSerialIoTimingM0C3;  // [M0C3] M0D3 for I2C3
  UINT16  PchSerialIoTimingM1C3;  // [M1C3] M1D3 for I2C3
  UINT16  PchSerialIoTimingM0C4;  // [M0C4] M0D3 for I2C4
  UINT16  PchSerialIoTimingM1C4;  // [M1C4] M1D3 for I2C4
  UINT16  PchSerialIoTimingM0C5;  // [M0C5] M0D3 for I2C5
  UINT16  PchSerialIoTimingM1C5;  // [M1C5] M1D3 for I2C5
  UINT16  PchSerialIoTimingM0C6;  // [M0C6] M0D3 for SPI0
  UINT16  PchSerialIoTimingM1C6;  // [M1C6] M1D3 for SPI0
  UINT16  PchSerialIoTimingM0C9;  // [M0C9] M0D3 for UART0
  UINT16  PchSerialIoTimingM1C9;  // [M1C9] M1D3 for UART0
  UINT16  PchSerialIoTimingM0CA;  // [M0CA] M0D3 for UART1
  UINT16  PchSerialIoTimingM1CA;  // [M1CA] M1D3 for UART1
  UINT16  PchSerialIoTimingM0CB;  // [M0CB] M0D3 for UART2
  UINT16  PchSerialIoTimingM1CB;  // [M1CB] M1D3 for UART2

  //
  // I3C-0 Timing Parameters.
  //
  UINT32  I3c0QueueThld;
  UINT32  I3c0DataBufferThld;
  UINT32  I3c0OdTiming;
  UINT32  I3c0PpTiming;
  UINT32  I3c0SclI2cFmTiming;
  UINT32  I3c0SclI2cFmpTiming;
  UINT32  I3c0SclI2cSsTiming;
  UINT32  I3c0ExtLcntTiming;
  UINT32  I3c0ExtTermLcntTiming;
  UINT32  I3c0SdaSwitchDelayTiming;
  UINT32  I3c0BusDelayTiming;

  //
  // I3C-2 Timing Parameters.
  //
  UINT32  I3c2QueueThld;
  UINT32  I3c2DataBufferThld;
  UINT32  I3c2OdTiming;
  UINT32  I3c2PpTiming;
  UINT32  I3c2SclI2cFmTiming;
  UINT32  I3c2SclI2cFmpTiming;
  UINT32  I3c2SclI2cSsTiming;
  UINT32  I3c2ExtLcntTiming;
  UINT32  I3c2ExtTermLcntTiming;
  UINT32  I3c2SdaSwitchDelayTiming;
  UINT32  I3c2BusDelayTiming;

  UINT8   ScanMatrixEnable;
  UINT8   TouchPanelMuxSelector;

  UINT8   Ac1TripPoint;
  UINT8   Ac0TripPoint;
  UINT8   Ac1FanSpeed;
  UINT8   Ac0FanSpeed;
  UINT8   PassiveThermalTripPoint;
  UINT8   CriticalThermalTripPoint;
  UINT8   PassiveTc1Value;
  UINT8   PassiveTc2Value;
  UINT8   PassiveTspValue;
  UINT8   DisableActiveTripPoints;
  UINT8   DisablePassiveTripPoints;
  UINT8   DisableCriticalTripPoints;
  UINT8   PmicVccLevel;
  UINT8   PmicVddqLevel;
  UINT8   TwoComponents;

  UINT8   CPUTempReadEnable;
  UINT8   CPUEnergyReadEnable;
  UINT8   PCHTempReadEnable;
  UINT8   AlertEnableLock;
  UINT8   PchAlert;
  UINT8   DimmAlert;
  UINT8   CpuTemp;
  UINT8   CpuFanSpeed;

  UINT8   DetectEcFan[MAX_EC_FANS];
  UINT8   DetectEcSensors[MAX_EC_SENSORS];

  UINT8   Rtd3Support;
  UINT8   Rtd3P0dl;
  UINT8   Rtd3UsbPt1;
  UINT8   Rtd3UsbPt2;
  UINT16  Rtd3AudioDelay;
  UINT16  Rtd3TouchPadDelay;
  UINT16  Rtd3TouchPanelDelay;
  UINT16  Rtd3SensorHub;
  UINT16  VRRampUpDelay;
  UINT8   PstateCapping;
  UINT8   Rtd3WWAN; // Deprecated
  UINT8   Rtd3SataPort0;
  UINT8   Rtd3SataPort1;
  UINT8   Rtd3SataPort2;
  UINT8   Rtd3SataPort3;
  UINT8   Rtd3SataPort4;
  UINT8   Rtd3SataPort5;
  UINT8   Rtd3SataPort6;
  UINT8   Rtd3SataPort7;
  UINT8   Rtd3WaGpio;
  UINT8   Rtd3Pcie;
  UINT8   Rtd3I2CTouchPanel;

  UINT8   CoExistenceManager; // Deprecated
  UINT8   UsbFnEnable;

  UINT8   FanControl;
  UINT8   CpuFanControl;
  UINT8   LowestFanSpeed;
  UINT8   EnableMsiInFadt;

  //
  // Thunderbolt(TM)
  //
  UINT8   TbtSetupFormSupport;
  UINT8   DiscreteTbtSupport;
  UINT8   IntegratedTbtSupport;
  UINT8   DTbtController[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbtControllerType[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbthostRouterPortNumber[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbtPcieExtraBusRsvd[DTBT_CONTROLLER_SUPPORTED];
  UINT16  DTbtPcieMemRsvd[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbtPcieIoRsvd[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbtPcieMemAddrRngMax[DTBT_CONTROLLER_SUPPORTED];
  UINT32  DTbtPciePMemRsvd[DTBT_CONTROLLER_SUPPORTED];
  UINT8   DTbtPciePMemAddrRngMax[DTBT_CONTROLLER_SUPPORTED];
  //
  // USB4 DTBT Setup options page support.
  //
  UINT8   Usb4CmMode;
  UINT8   Usb4DebugLevel;
  UINT8   Usb4ClassOption;

  UINT8   DTbtRtd3;
  UINT16  DTbtRtd3OffDelay;
  UINT32  DTbtRtd3ClkReq;
  UINT16  DTbtRtd3ClkReqDelay;

  // Disable TBT PCIE RP and child device Tree BME. It was earlier known as TbtVtdBaseSecurity
  UINT8   DisableTbtPcieTreeBme;

  // ITBT Specific Setup Options
  UINT8   OsNativeResourceBalance;
  UINT8   ITbtPcieExtraBusRsvd[ITBT_ROOT_PORTS_SUPPORTED];
  UINT16  ITbtPcieMemRsvd[ITBT_ROOT_PORTS_SUPPORTED];
  UINT8   ITbtPcieMemAddrRngMax[ITBT_ROOT_PORTS_SUPPORTED];
  UINT32  ITbtPciePMemRsvd[ITBT_ROOT_PORTS_SUPPORTED];
  UINT8   ITbtPciePMemAddrRngMax[ITBT_ROOT_PORTS_SUPPORTED];
  UINT16  ITbtForcePowerOnTimeoutInMs;
  UINT16  ITbtConnectTopologyTimeoutInMs;
  UINT16  ReserveMemoryPerSlot;
  UINT16  ReservePMemoryPerSlot;
  UINT8   ReserveIoPerSlot;
  UINT8   UsbCRetimerNumber;

  UINT8   IccLockRegisters;
  UINT8   IccProfile;
  UINT8   ShowProfile;
  UINT8   NumProfiles;

  UINT8   CsModeFirst;
  UINT8   CsMode;
  UINT8   CsModeChanged;

  UINT8   PlatformUnstable;
  UINT8   IsOcDefaultsInitalized;

  UINT8   HidEventFilterDriverEnable;

  UINT8   GuidForward;

  UINT8   HebcValueSupport;
  UINT32  HebcValue;

  UINT8   I2cSarResetDelay; // Deprecated

  UINT8   WwanEnable;       // Deprecated

  UINT8   UsbcBiosTcssHandshake;
  // EC - BIOS handshake timeouts
  UINT32  UsbConnStatusTimeout;


  UINT8   VtioSupport;
  UINT8   SdevCio2Entry;
  UINT8   SdevIspEntry;
  UINT8   SdevHeciEntry;
  UINT8   SdevSpi1Entry;
  UINT8   SdevSpi2Entry;
  UINT8   SdevXhciEntry;
  UINT16  SdevNumberOfSensors[5];
  UINT16  SdevSensorEntry1[5];
  UINT16  SdevSensorEntry2[5];

  UINT8   EcChargingMethod;
  UINT8   AlternateModeSynchDelay;
  UINT8   WakeOnWiGigSupport;

  UINT8   SystemTimeAndAlarmSource; // Select source for system time and alarm

  UINT8   TestLtDprLock;
  UINT8   TestUsbUnlockForNoa;

  UINT8   EnableCrashLog;
  UINT8   CrashLogOnAllReset;
  UINT8   CrashLogSiliconClear;
  UINT8   CrashLogSiliconRearm;
  UINT8   CrashLogSiliconGblRst;
  UINT32  CrashLogSiliconGblRstMask0;
  UINT32  CrashLogSiliconGblRstMask1;
  UINT8   CrashLogCpuClear;
  UINT8   CrashLogCpuRearm;
  UINT8   CrashLogCpuStorageOff;

  UINT8   SysFwUpdateLoadDefault;
  UINT8   SysFwUpdateSkipPowerCheck;

  UINT8   BoardReworkComplainceEnable;
  UINT8   ItpxdpMuxSupport;
  UINT8   ItpxdpMux;

  UINT8   OsDnx;

  UINT8   PuisEnable;
  // Option to slect UCMC/UCSI device
  UINT8   TcssUcmDevice;
  UINT8   PepPcieStorage;
  UINT8   KernelDebugPatch;
  UINT8   ITbtRtd3;
  UINT16  ITbtRtd3ExitDelay;
  UINT8   AuxOriOverrideSupport;
  UINT8   AuxOriOverride;

  UINT8   PowermeterDeviceEnable; // powermeter enabling
  UINT8   WwanResetWorkaround;    // Deprecated
  UINT8   TelemetryDeviceEnable;

  UINT8   SdevXhciNumberOfDevices;
  UINT8   SdevXhciDeviceAttributes[2];
  UINT8   SdevXhciRootPortNumber[2];
  UINT16  SdevXhciVendorId[2];
  UINT16  SdevXhciProductId[2];
  UINT16  SdevXhciRevision[2];
  UINT8   SdevXhciInterfaceNumber[2];
  UINT8   SdevXhciClass[2];
  UINT8   SdevXhciSubClass[2];
  UINT8   SdevXhciProtocol[2];
  UINT16  SdevXhciAcpiPathStringOffset[2];
  UINT16  SdevXhciAcpiPathStringLength[2];
  UINT64  SdevXhciFirmwareHashDevice1[4];
  UINT64  SdevXhciFirmwareHashDevice2[4];
  UINT16  SdevXhciAcpiPathNameDevice1[49]; // 48 characters + NULL termination
  UINT16  SdevXhciAcpiPathNameDevice2[49]; // 48 characters + NULL termination
  UINT8   ControlIommu;

  UINT8   SdevFlags[5];
  UINT8   SdevXhciFlags;
  UINT8   DevicePasswordSupport;
  UINT8   PepVmd;
  UINT8   EnablePcieTunnelingOverUsb4;

  UINT8   MipiCam_Flash0_Mode;
  UINT8   MipiCam_Flash1_Mode;
  UINT8   MipiCam_Flash2_Mode;
  UINT8   MipiCam_Flash3_Mode;
  UINT8   MipiCam_Flash4_Mode;
  UINT8   MipiCam_Flash5_Mode;
  UINT8   TypecRetimerTxComplianceModeEn;
  UINT8   TypecRetimerPD0;
  UINT8   TypecRetimerPD1;
  UINT8   TypecRetimerPD2;
  UINT8   TypecRetimerPD3;
  UINT8   PepNpu;
  UINT8   PepHeci3;
  UINT8   PepSataEnumeration;
  UINT8   PlnEnable;

  UINT8   PmaxDevice;
  UINT8   PmaxAudioCodec;
  UINT8   PmaxWfCamera;
  UINT8   PmaxUfCamera;
  UINT8   PmaxFlashDevice;

  UINT8   StorageRtd3Support;

  UINT16 PcieDevOnOffDelay;

  UINT8   PepPcieLan;
  UINT8   PepPcieWlan;
  UINT8   PepPcieGfx;
  UINT8   PepPcieOther;

  UINT16  WwanTOn2ResDelayMs;           // Deprecated
  UINT16  WwanTOnRes2PerDelayMs;        // Deprecated
  UINT16  WwanTOnPer2PdsDelayMs;        // Deprecated
  UINT16  WwanTRes2OffDelayMs;          // Deprecated
  UINT16  WwanTOffDisDelayMs;           // Deprecated
  UINT16  WwanTResTogDelayMs;           // Deprecated
  UINT16  WwanTRes2PdsDelayMs;          // Deprecated
  UINT16  WwanTPer2ResDelayMs;          // Deprecated
  UINT8   WwanFwFlashDevice;            // Deprecated
  UINT8   MPdtSupport;

  //
  // LCH Setup Option
  //
  UINT8   LchSupport;
  UINT8   LchTestDevice;

  UINT8   UsbcDataRoleSwapPlatformDisable; // Platform Setup option to disable USBC DataRole Swap
  UINT8   UsbcCapsuleDebugLevel;

  UINT16  WwanOemSvid;     // Deprecated
  UINT16  WwanSvidTimeout; // Deprecated
  UINT8   DeepestUSBSleepWakeCapability;

  UINT8   WirelessCnvConfigDevice;         // Deprecated

  UINT8   Gpe1BlockEnable;

  UINT16  PchSerialIoTimingSSH6;  // [SSH6] SSCN-HIGH for I2C6
  UINT16  PchSerialIoTimingSSL6;  // [SSL6] SSCN-LOW  for I2C6
  UINT16  PchSerialIoTimingSSD6;  // [SSD6] SSCN-HOLD for I2C6
  UINT16  PchSerialIoTimingFMH6;  // [FMH6] FMCN-HIGH for I2C6
  UINT16  PchSerialIoTimingFML6;  // [FML6] FMCN-LOW  for I2C6
  UINT16  PchSerialIoTimingFMD6;  // [FMD6] FMCN-HOLD for I2C6
  UINT16  PchSerialIoTimingFPH6;  // [FPH6] FPCN-HIGH for I2C6
  UINT16  PchSerialIoTimingFPL6;  // [FPL6] FPCN-LOW  for I2C6
  UINT16  PchSerialIoTimingFPD6;  // [FPD6] FPCN-HOLD for I2C6
  UINT16  PchSerialIoTimingHSH6;  // [HSH6] HSCN-HIGH for I2C6
  UINT16  PchSerialIoTimingHSL6;  // [HSL6] HSCN-LOW  for I2C6
  UINT16  PchSerialIoTimingHSD6;  // [HSD6] HSCN-HOLD for I2C6
  UINT16  PchSerialIoTimingM0CC;  // [M0CC] M0D3 for I2C6
  UINT16  PchSerialIoTimingM1CC;  // [M1CC] M1D3 for I2C6
  UINT8   PepI2c6;

  UINT16  PchSerialIoTimingSSH7;  // [SSH7] SSCN-HIGH for I2C7
  UINT16  PchSerialIoTimingSSL7;  // [SSL7] SSCN-LOW  for I2C7
  UINT16  PchSerialIoTimingSSD7;  // [SSD7] SSCN-HOLD for I2C7
  UINT16  PchSerialIoTimingFMH7;  // [FMH7] FMCN-HIGH for I2C7
  UINT16  PchSerialIoTimingFML7;  // [FML7] FMCN-LOW  for I2C7
  UINT16  PchSerialIoTimingFMD7;  // [FMD7] FMCN-HOLD for I2C7
  UINT16  PchSerialIoTimingFPH7;  // [FPH7] FPCN-HIGH for I2C7
  UINT16  PchSerialIoTimingFPL7;  // [FPL7] FPCN-LOW  for I2C7
  UINT16  PchSerialIoTimingFPD7;  // [FPD7] FPCN-HOLD for I2C7
  UINT16  PchSerialIoTimingHSH7;  // [HSH7] HSCN-HIGH for I2C7
  UINT16  PchSerialIoTimingHSL7;  // [HSL7] HSCN-LOW  for I2C7
  UINT16  PchSerialIoTimingHSD7;  // [HSD7] HSCN-HOLD for I2C7
  UINT16  PchSerialIoTimingM0CD;  // [M0CD] M0D3 for I2C7
  UINT16  PchSerialIoTimingM1CD;  // [M1CD] M1D3 for I2C7
  UINT8   PepI2c7;
  UINT8   PepUfs0;
  UINT8   PepUfs1;

  UINT8   ClosedLidWovLightingSupport;

  //
  // Flash ID support for Link 0-5
  //
  UINT8   MipiCam_Link0_DriverData_FlashID;
  UINT8   MipiCam_Link1_DriverData_FlashID;
  UINT8   MipiCam_Link2_DriverData_FlashID;
  UINT8   MipiCam_Link3_DriverData_FlashID;
  UINT8   MipiCam_Link4_DriverData_FlashID;
  UINT8   MipiCam_Link5_DriverData_FlashID;
  UINT8   SkipObbHashVerification;

  //
  // TCSS Port Disable UCSI/UCMX Driver Support. 0: Platform POR (For EclessPd it is UCMX, For For EcPd it is UCSI), 2: Force Disable the Driver support.
  //
  UINT8   TestDisableTcssPortUcxxSupport[10]; // MAX 10 Type-C Port. Reserve Extra also.

  UINT8   DgPlatformSupport;
  UINT8   SafeSetting;

  UINT8   CnvDlvrRfim;

  UINT16  M21aAssembled;
  UINT16  WifiPlatCurrentConsumption;
  UINT8   MtlPSDetected;
  UINT8   GbeFwUpdateEnable;

  //
  // DMI EQ Remote Transmitter Coefficient/Preset Override
  //
  UINT8   Gen3RtcoRtpoEnable;
  UINT8   DmiGen3RtcoCpre[8];
  UINT8   DmiGen3RtcoCpo[8];
  UINT8   Gen4RtcoRtpoEnable;
  UINT8   DmiGen4RtcoCpre[8];
  UINT8   DmiGen4RtcoCpo[8];
  UINT8   IsArlMobile;

  UINT8   ITbtPcieTunnelingForUsb4;  // Deprecated from Revision 2

  UINT8   PepIsh;
  //
  // TCSS XHCI Enable Convert of Type-C to Type-A Port
  //
  UINT8   OrientationSensor;
  UINT8   OsDebugPort;
  UINT8   TerminalOverDebug;

  //
  // IUER Configuration
  //
  UINT8   IuerButtonEnable;
  UINT8   IuerConvertibleEnable;
  UINT8   SlateIndicatorSx;
  UINT8   IuerDockEnable;
  UINT8   DockIndicatorSx;
  UINT8   SlateIndicatorRT;
  UINT8   DockIndicatorRT;
  UINT8   Rsvd[1];

  //
  // SA StreamTracer options.
  //
  UINT32  StreamTracerSize;
  UINT32  StreamTracerMode;
  UINT16  PcieRtd3LinkActiveTimeout;

  //
  // Retrieve Mac address from PSS chip
  //

  //
  // Skin Thermal Control:
  // Sensor1, 2, 3 config
  //
  UINT8   StcPeciInterfaceEnable;
  UINT8   StcTargetTemp[3];
  UINT8   StcEnable[3];
  UINT8   StcGain[3];
  UINT8   StcMinPerfLevel[3];
  UINT8   IpfEnable;

  UINT8   SafeLoadingBiosEnable;
  UINT8   TsegReallocateEnableCount;
  UINT8   PepOsse;

  UINT8   StorageDynamicLinkManagement;

  //
  // Static Content
  //
  UINT16  StaticContentSizeAt4Gb;

  UINT8   TccMode;
  UINT8   PchIoI2cWittDeviceEnable;
  UINT8   PchIoI2cWittDevice[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  UINT8   PchIoI3cWittDeviceEnable;
  UINT8   PchIoI3cWittDevice[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  UINT8   PchIoSpiWittDeviceEnable;
  UINT8   PchIoSpiWittDevice[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
  UINT16  PlatformSN[SERIAL_NUMBER_SIZE];
} SETUP_DATA;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added VccsaShutdown.
**/
#define CPU_SETUP_REVISION  2

typedef struct {
  UINT8   Revision;
  UINT8   CpuRatio;
  UINT8   CpuDefaultRatio;
  UINT8   CpuRatioOverride;
//
// CPU: Number of active Big core.
//
  UINT8   ActiveCoreCount;
  UINT8   BistOnReset;
  UINT8   EnableGv;
  UINT8   RaceToHalt;
  UINT8   EnableHwp;
  UINT8   BootMaxFrequency;

  UINT8   EnableHwpAutoPerCorePstate;
  UINT8   EnableHwpAutoEppGrouping;
  UINT8   EnableDynamicEfficiencyControl;

  UINT8   BootFrequency;
  UINT8   EnableCx;
  UINT8   TurboMode;
  UINT32  PowerLimit1;
  UINT8   LongDurationPwrLimitOverride;
  UINT8   PowerLimit1Time;
  UINT8   PowerLimit2;
  UINT32  PowerLimit2Power;
  UINT8   PowerLimit3Override;
  UINT32  PowerLimit3;
  UINT8   PowerLimit3Time;
  UINT8   PowerLimit3DutyCycle;
  UINT8   PowerLimit3Lock;
  UINT8   ResponseMode;
  UINT8   PlatformAtxTelemetryUnit;
  UINT8   PowerLimit4Override;
  UINT32  PowerLimit4;
  UINT8   PowerLimit4Lock;
  UINT16  PowerLimit4Boost;
  UINT8   TurboPowerLimitLock;
  UINT8   PlatformPowerLimit1Enable;
  UINT32  PlatformPowerLimit1Power;
  UINT8   PlatformPowerLimit1Time;
  UINT8   PlatformPowerLimit2Enable;
  UINT32  PlatformPowerLimit2Power;
  UINT8   EnergyEfficientPState;
  UINT8   CStatePreWake;
  UINT8   CStateAutoDemotion;
  UINT8   CStateUnDemotion;
  UINT8   PkgCStateDemotion;
  UINT8   PkgCStateUnDemotion;
  UINT8   EnableThermalMonitor;
  UINT8   PmgCstCfgCtrlLock;
  UINT8   ConfigTdpLevel;
  UINT8   ConfigTdpLock;
  UINT8   ConfigTdpBios;
  UINT8   TimedMwait;
  UINT8   InterruptRedirectMode;

  UINT8   PkgCStateLimit;

  UINT32  CustomPowerLimit1Power;
  UINT32  CustomPowerLimit2Power;
  UINT8   CustomPowerLimit1Time;
  UINT8   CustomTurboActivationRatio;
  UINT8   TStatesEnable;
  UINT8   DisableVrThermalAlert;
  UINT8   PowerFloorManagement;
  UINT8   PowerFloorDisplayDisconnect;
  UINT8   PowerFloorPcieGenDowngrade;
  UINT8   TCCActivationOffset;
  UINT8   NumOfCustomPStates;
  UINT8   StateRatio[MAX_CUSTOM_P_STATES];
  UINT8   StateRatioMax16[MAX_16_CUSTOM_P_STATES];
  UINT8   VT;
  UINT8   AES;
  UINT8   MachineCheck;
  UINT8   MonitorMwait;
  UINT16  DprSize;
  UINT8   FitRecTxt;
  UINT8   Txt;
  UINT8   LtDprProgramming;
  UINT8   ResetAux;
  UINT8   AcheckRequest;
  UINT8   MsegOpt;

  UINT8   EcTurboControlMode;
  UINT8   AcBrickCapacity;
  UINT8   EcPollingPeriod;
  UINT8   EcGuardBandValue;
  UINT8   EcAlgorithmSel;

  UINT8   IsTurboRatioDefaultsInitialized;
  UINT8   FlexRatioOverrideDefault;
  UINT8   RatioLimitRatioDefault[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   RatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   RatioLimitNumCoreDefault[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   RatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   AtomRatioLimitRatioDefault[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   AtomRatioLimitRatio[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   AtomRatioLimitNumCoreDefault[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   AtomRatioLimitNumCore[TURBO_RATIO_LIMIT_ARRAY_SIZE];
  UINT8   OverclockingLock;

  UINT8   DebugInterfaceEnable;
  UINT8   DebugInterfaceLockEnable;
  UINT8   DebugCpuDisabled;

  UINT8   ApIdleManner;

  UINT8   ProcessorTraceTimingPacket;
  UINT8   ProcessorTraceBspOnly;
  UINT8   ProcessorTraceOutputScheme;
  UINT8   ProcessorTraceMemSize;
  UINT8   ProcessorTraceEnable;

  UINT8   SkipStopPbet;
  UINT8   ThreeStrikeCounter;
  UINT8   EpocFclkFreq;

  //
  // VR Config. IA = 0, GT = 1, SA = 2, ATOM = 3
  //
  UINT8   PsysSlope;
  UINT32  PsysOffset;
  UINT8   PsysOffsetPrefix;
  UINT16  PsysPmax;
  UINT8   VrConfigEnable[MAX_VR_NUM];
  UINT16  AcLoadline[MAX_VR_NUM];
  UINT16  DcLoadline[MAX_VR_NUM];
  UINT16  Ps1Threshold[MAX_VR_NUM];
  UINT16  Ps2Threshold[MAX_VR_NUM];
  UINT16  Ps3Threshold[MAX_VR_NUM];
  UINT8   Ps3Enable[MAX_VR_NUM];
  UINT8   Ps4Enable[MAX_VR_NUM];
  UINT16  ImonSlope[MAX_VR_NUM];
  UINT32  ImonOffset[MAX_VR_NUM];
  UINT8   ImonOffsetPrefix[MAX_VR_NUM];
  UINT16  IccMax[MAX_VR_NUM];
  UINT16  TdcCurrentLimit[MAX_VR_NUM];
  UINT8   TdcEnable[MAX_VR_NUM];
  UINT8   IrmsSupported[MAX_VR_NUM];
  UINT8   TdcMode[MAX_VR_NUM];
  UINT32  TdcTimeWindow[MAX_VR_NUM];
  UINT8   TdcLock[MAX_VR_NUM];
  UINT16  VrVoltageLimit[MAX_VR_NUM];
  UINT8   TccOffsetLock;
  UINT8   ThETAIbattEnable;

  //
  // CEP (Current Excursion Protection) Enable.
  //
  UINT8   CepEnable[MAX_VR_NUM];
  UINT8   Reserved1[4];

  //
  // CPU related
  //
  UINT8   FlexOverrideEnable;
  UINT8   MaxEfficiencyRatio;
  UINT8   MaxNonTurboRatio;
  UINT8   FlexRatioOverride;

  UINT8   BusSpeedEnable;
  UINT8   ProcessorBusSpeedOverride;

  UINT8   PciePll;
  UINT8   EnergyEfficientTurbo;

  UINT8   WDTSupportforNextOSBoot;
  UINT16  TimeforNextOSBoot;
  UINT8   X2ApicEnable;
  //
  // VR Core Load Line override
  //
  UINT8   CoreVrDcLLOverrideEnable;
  UINT8   CoreDcLoadline;

  UINT8   AvxDisable;

  //
  // CPU: Number of active LP atom core.
  //
  UINT8   ActiveLpAtomCoreCount;
  UINT8   AllLpAtomCoreCount;
  //
  // CPU: Record number of all cores including enabled & disabled.
  //
  UINT8   AllSmallCoreCount;
  UINT8   AllBigCoreCount;

  UINT8   DualTauBoost;

  //
  // CPU: DLVR clock frequency and encoded spectrum spread percentage.
  //
  UINT8   DlvrSpreadSpectrumPercentage;
  UINT16  DlvrRfiFrequency;
  UINT8   DlvrRfiEnable;

  //
  // Intel Speed Optimizer
  //
  UINT8   IntelSpeedOptimizer;

  //
  // GPRs setting of UCODE_CR_CRASHLOG_CONTROL
  //
  UINT8   CrashLogGprs;

  //
  // Vsys
  //
  UINT8   EnableVsysCritical;
  UINT8   VsysAssertionDeglitchMantissa;
  UINT8   VsysAssertionDeglitchExponent;
  UINT8   VsysDeassertionDeglitchMantissa;
  UINT8   VsysDeassertionDeglitchExponent;

  UINT8   DfdEnable;

  //
  // VR FastVmode control overrides
  //
  UINT8   EnableFastVmode[MAX_VR_NUM];
  UINT8   IsBootFrequencyDefaultsInitialized;
  UINT8   BootFrequencyDefault;
  UINT8   TurboModeDefault;
  UINT32  VsysFullScale;
  UINT32  VsysCriticalThreshold;
  UINT32  PsysFullScale;
  UINT32  PsysCriticalThreshold;

  UINT8   EnableAllThermalFunctions;
  //
  // Acoustic Noise Mitigation
  //
  UINT8   AcousticNoiseMitigation;
  UINT8   FastPkgCRampDisable[MAX_VR_NUM];
  UINT8   SlowSlewRate[MAX_VR_NUM];
  //
  // Hysteresis window for Acoustic Noise Mitigation
  //
  UINT8   PcoreHysteresisWindow;
  UINT8   EcoreHysteresisWindow;

  UINT8   HwpInterruptControl;

  UINT8   PpinSupport;
  UINT8   PpinEnableMode;

  UINT8   MinRingRatioLimit;
  UINT8   MaxRingRatioLimit;

  UINT8   ApplyConfigTdp;
  UINT8   HwpLock;
  UINT8   TmeEnable;
  UINT8   SmmProcTraceEnable;
  //
  // CPU: Number of active small core.
  //
  UINT8   ActiveSmallCoreCount;
  UINT16  IccLimit[MAX_VR_NUM];

  UINT8   ForcePrDemotion;
  UINT8   VrAlertDemotion;

  UINT8   Peci;
  UINT8   EnableRp;
  UINT16  IsysCurrentLimitL1;
  UINT8   IsysCurrentLimitL1Enable;
  UINT8   IsysCurrentL1Tau;
  UINT16  IsysCurrentLimitL2;
  UINT8   IsysCurrentLimitL2Enable;
  UINT16  VsysMax;

  UINT8   AcSplitLock;
  UINT8   TseEnable;
  UINT8   CompareCtdpRatio;
  UINT8   CompareCtdpRatio2;
  UINT8   Reserved[14];
  //
  // Select Core(s) and RING DLVR Mode
  //
  UINT8   DlvrPhaseSsc;

  UINT8   MtlArlSocDetected;
  // TME bypass enable
  UINT8   TmeBypassCapability;
  //
  // Discrete Clock
  //
  UINT16  CpuBclkFreq;
  UINT8   PegDmiClkFreq;
  UINT8   DccClkCtrl;
  UINT16  CurrentCpuBclkFreq;
  UINT16  CurrentPegDmiClkFreq;
  UINT8   SkinTargetTemp[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinTempControlEnable[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinControlLoopGain[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinTempOverrideEnable[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinMinPerformanceLevel[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinTempOverride[SKIN_TEMP_CONTROL_SENSOR];
  UINT8   SkinTempControl;
  UINT8   TdxEnable;
  UINT8   ReduceXecores;
  UINT8   VccsaShutdown;
} CPU_SETUP;

#define CPU_SETUP_VOLATILE_DATA_REVISION       2
typedef struct {
  UINT16  CpuFamilyModel;
  UINT16  CpuExtendedFamilyModel;
  UINT8   CpuStepping;
  UINT8   EdramSupported;

  //
  // Vr Topology
  //
  UINT8  CoreVrLocked;
  UINT8  GtVrLocked;
  UINT8  SaVrLocked;
  UINT8  AtomVrLocked;
  UINT8  TccSupported;
} CPU_SETUP_VOLATILE_DATA;

#define TBT_SETUP_VOLATILE_DATA_REVISION       1
typedef struct {
  UINT8  DTbtContollersNumber;
  UINT8  BoardITbtPcieRootPortSupported[MAX_ITBT_PCIE_PORT];
  UINT8  TcssPortEnable[MAX_TCSS_USB3_PORTS];
} TBT_SETUP_VOLATILE_DATA;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/

#define ME_SETUP_REVISION   1

typedef struct {
  UINT8   Revision;
  //
  // Intel ME
  //
  UINT8   OperationMode;
  UINT8   MeImageType;
  UINT8   MeFwDowngrade;

  UINT8   HeciTimeouts;
  UINT8   DidInitStat;
  UINT8   DisableCpuReplacedPolling;
  UINT8   DisableMessageCheck;
  UINT8   SkipMbpHob;
  UINT8   HeciCommunication;
  UINT8   HeciCommunication2;
  UINT8   HeciCommunication3;
  UINT8   KtDeviceEnable;
  UINT8   DisableD0I3SettingForHeci;
  UINT8   MctpBroadcastCycle;

  UINT8   UnconfigOnRtcAvailable;
  UINT8   MeUnconfigOnRtcClear;
  UINT8   CoreBiosDoneEnabled;
  UINT8   EndOfPostMessage;

  //
  // Intel AMT
  //
  UINT8   Amt;
  UINT8   UnConfigureMe;
  UINT8   AmtCiraRequest;
  UINT8   UsbProvision;

  //
  // ASF
  //
  UINT8   AsfSensorsTable;

  //
  // WatchDog
  //
  UINT8   WatchDogEnabled;
  UINT16  WatchDogTimerOs;
  UINT16  WatchDogTimerBios;

  //
  // Extend ME/CSME Measurements to TPM-PCR
  //
  UINT8   ExtendMeMeasuredBoot;

  //
  // Anti-Rollback SVN
  //
  UINT8   AutoArbSvnCommit;

  //
  // ME-SMBIOS 130 capabilities
  //
  UINT8   BiosReflash;
  UINT8   BiosSetup;
  UINT8   BiosPause;
  UINT8   SecureBootExposureToFw;
  UINT8   vProTbtDock;
  UINT8   vProTbtDockSupported;

  //
  // One Click Recovery
  //
  UINT8   OcrBootHttps;
  UINT8   OcrBootPba;
  UINT8   OcrBootWinRe;
  UINT8   OcrAmtDisSecBoot;

  //
  // CSE Data Resilience
  //
  UINT8   CseDataResilience;

  //
  // SSE Policy
  //
  UINT8   SseCommunication;

  //
  // PSE EOM Flow
  //
  UINT8   PseEomFlowEnable;
} ME_SETUP;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Added CnviDdrRfim.
  <b>Revision 3</b>:  - CI Fix.
  <b>Revision 4</b>:  - Add MrcFailureOnUnsupportedDimm.
  <b>Revision 5</b>:  - Add DCCDDR5READDCA.
  <b>Revision 6</b>:  - Add PPR-related variables.
  <b>Revision 7</b>:  - Add RDTCIDLE.
  <b>Revision 8</b>:  - Add PprRetryLimit variable.
  <b>Revision 9</b>:  - Add Use1p5ReadPostamble variable.
  <b>Revision 10</b>: - Add IbeccEccInjControl, IbeccEccInjAddrBase, IbeccEccInjCount
  <b>Revision 11</b>: - Add MsHashOverride, MsHashInterleaveBit, MsHashMask
  <b>Revision 12</b>: - Add Fusa Cfi Parity variables and FusaConfigEnable
  <b>Revision 13</b>: - Add Lockstep variables
  <b>Revision 14</b>: - Add StreamTracer support variables
  <b>Revision 15</b>: - Add IsWckIdleExitEnabled variable
  <b>Revision 16</b>: - Add WREQT variable
  <b>Revision 17</b>: - Remove FUSA CFI variables
  <b>Revision 18</b>: - Add Nest Support and Posted Interrupt Support variables for Vt-d
  <b>Revision 19</b>: - Add TbtPerfBoostEn for TBT Performance Boost Enable/Disable.
  <b>Revision 20</b>: - Add Fabric GV support.
  <b>Revision 21</b>: - Add Lp5SafeSpeed
  <b>Revision 22</b>: - Add Vdd2HVoltage, Vdd1Voltage, Vdd2LVoltage, VddqVoltage
  <b>Revision 23</b>: - Add ForceInternalClkOn
  <b>Revision 24</b>: - Add DIMMRXOFFSET
  <b>Revision 25</b>: - Add FlexibleAnalogSettings
  <b>Revision 26</b>: - Add ForceWRDSEQT2400
  <b>Revision 27</b>: - Add NnFlex override knobs
  <b>Revision 28</b>: - Add ForceDIMMRXOFFSET
**/

#define SA_SETUP_REVISION   28

typedef struct {
  UINT8   Revision;
  UINT8   BdatEnable;

  //
  // Memory related
  //
  UINT8   XmpProfileEnable;
  UINT8   DllBwEn0;
  UINT8   DllBwEn1;
  UINT8   DllBwEn2;
  UINT8   DllBwEn3;

  UINT8   DdrRefClk;
  UINT16  DdrRatio;
  UINT8   DdrOddRatioMode;

  UINT16  MemoryVddVoltage;
  UINT16  MemoryVddqVoltage;
  UINT16  MemoryVppVoltage;

  UINT8   DFETap1[MRC_CONTROLLER_DIMM_TAP1_NUM]; //2 Channels' 2 Dimm's Tap1
  UINT8   DFETap2[MRC_CONTROLLER_DIMM_TAP2_NUM]; //2 Channels' 2 Dimm's Tap2

  UINT16  RxVrefOffset;

  UINT16  GtDid;                    // not a SETUP item, used by BIOS to pass GT SKU DID to SETUP items
  UINT8   EnableRenderStandby;
  UINT8   EnableMediaStandby;
  UINT8   AlsEnable;
  UINT8   PanelPowerEnable;
  UINT8   ConfigureGT;
  UINT8   ConfigureMedia;
  UINT8   PeiGraphicsPeimInit;
  UINT8   PavpEnable;
  UINT8   GopConfigEnable;
  UINT8   RC1pGtFreqEnable;
  UINT8   RC1pMediaFreqEnable;
  //
  // ICC Related
  //
  UINT8   BclkOverride;

  //
  // PEG
  //
  UINT8   Peg0Enable;
  UINT8   Peg1Enable;
  UINT8   Peg2Enable;
  UINT8   Peg3Enable;
  UINT8   Peg0PowerDownUnusedLanes;
  UINT8   Peg1PowerDownUnusedLanes;
  UINT8   Peg2PowerDownUnusedLanes;
  UINT8   Peg3PowerDownUnusedLanes;

  UINT8   PegMaxPayload[4];
  UINT8   InitAspmAfterOprom;
  UINT8   PegRootPortHPE[4];

  //
  // DVMT5.0 Graphic memory setting
  //
  UINT8   IgdDvmt50PreAlloc;
  //
  // SA Device Control
  //
  UINT8   EnableVtd;
  UINT8   EnableAbove4GBMmio;
  UINT8   SaIpuEnable;
  UINT8   SaIpuImrConfiguration;
  UINT8   EdramTestMode;
  //
  // VT-d
  //
  UINT8   DmaControlGuarantee;

  // Primary Display Select
  UINT8   PrimaryDisplay;
  UINT8   InternalGraphics;
  // TCSS
  UINT8   TcssPortConf[MAX_TCSS_USB3_PORTS];
  UINT8   TcssXhciEn;

  // VCCST Status
  UINT8   TcssVccstStatus;
  //Enable D3 Cold in TCSS
  UINT8   TcssD3ColdEnable;
  //Enable PTM in TCSS
  UINT8   PtmEnabled[MAX_ITBT_PCIE_PORT];
  //
  // PCIe LTR Configuration
  //
  UINT8   SaPcieItbtLtrEnable[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtSnoopLatencyOverrideMode[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtSnoopLatencyOverrideMultiplier[MAX_ITBT_PCIE_PORT];
  UINT16  SaPcieItbtSnoopLatencyOverrideValue[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtNonSnoopLatencyOverrideMode[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtNonSnoopLatencyOverrideMultiplier[MAX_ITBT_PCIE_PORT];
  UINT16  SaPcieItbtNonSnoopLatencyOverrideValue[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtForceLtrOverride[MAX_ITBT_PCIE_PORT];
  UINT8   SaPcieItbtLtrConfigLock[MAX_ITBT_PCIE_PORT];
  //
  // Cpu Telemetry Aggregator (CrashLog)
  //
  UINT8   CpuCrashLogDevice;

  //
  // VMD:Settings
  //
  UINT8   VmdSupported;
  UINT8   VmdEnable;
  UINT8   VmdGlobalMapping;
  UINT8   VmdPort[VMD_MAX_DEVICES];
  UINT8   VmdPortBus[VMD_MAX_DEVICES];
  UINT8   VmdPortDev[VMD_MAX_DEVICES];
  UINT8   VmdPortFunc[VMD_MAX_DEVICES];
  UINT8   VmdPortPresent[VMD_MAX_DEVICES];
  UINT8   RaidR0;
  UINT8   RaidR1;
  UINT8   RaidR10;
  UINT8   RaidR5;
  UINT8   MaxTolud;
  UINT8   DynamicMemoryBoostTrainingFailed;
  UINT8   StrongWkLeaker;
  UINT8   LpddrMemWriteLatencySet;
  UINT8   BdatTestType;
  UINT8   MemTestOnWarmBoot;

  UINT8   BoardStackUp;

  UINT8   ForceColdReset;
  UINT8   SkipExtGfxScan;

  //
  // CRID
  //
  UINT8   CridEnable;

  UINT8   RealtimeMemoryTiming;         ///< Realtime memory timing will allow system to perform realtime memory timing changes.
  UINT8   BclkChangeWarmReset;
  UINT8   BclkChangeContinues;

  //
  // FUSA
  //
  UINT8   DisplayFusaConfigEnable;      //deprecated
  UINT8   GraphicFusaConfigEnable;      //deprecated
  UINT8   OpioFusaConfigEnable;         //deprecated
  UINT8   DpmemSupport;
  UINT8   NpuEnable;
  UINT8   OverrideDriverType;
  UINT8   Lp5BypassVddqLimits;
  //Tc C-State limit
  UINT8   TcStateLimit;
  UINT8   TcNotifyIgd;


  UINT8   tCLDefault;
  UINT8   tCWLDefault;
  UINT16  tFAWDefault;
  UINT16  tRASDefault;
  UINT8   tRCDtRPDefault;
  UINT16  tREFIDefault;
  UINT16  tRFCDefault;
  UINT8   tRRDDefault;
  UINT8   tRTPDefault;
  UINT8   tWRDefault;
  UINT8   tWTRDefault;
  UINT8   NModeDefault;
  UINT8   MemoryBandwidthCompression;
  UINT8   FpbEnable;
  UINT8   WrDqDqsReTraining;
  UINT8   PllOffsetCal;
  //
  // DdrRfim
  //
  UINT8   CnviDdrRfim;

  //
  // MRC settings
  //
  // ExtInputs
  UINT8   HobBufferSize;
  UINT8   SpdProfileSelected;
  UINT16  tCL;
  UINT16  tRCDtRP;
  UINT16  tRAS;
  UINT16  tWR;
  UINT16  tRFC;
  UINT16  tRRD;
  UINT16  tWTR;
  UINT16  tRTP;
  UINT16  tFAW;
  UINT16  tCWL;
  UINT32  tREFI;
  UINT16  MemoryVoltage;
  UINT16  Vdd2Mv;
  UINT8   EccSupport;
  UINT8   RemapEnable;
  UINT8   ScramblerSupport;
  UINT8   ProbelessTrace;
  UINT8   PerBankRefresh;
  UINT8   ExtendedBankHashing;
  UINT8   RmtPerTask;
  UINT8   TrainTrace;
  UINT8   DisPgCloseIdleTimeout;
  UINT8   Ibecc;
  UINT8   IbeccParity;
  UINT8   IbeccOperationMode;
  UINT8   ChHashOverride;
  UINT8   RetrainOnFastFail;
  UINT8   DvfsqEnabled;
  UINT8   DvfscEnabled;
  UINT8   VoltageReadout;
  UINT8   CccHalfFrequency;
  UINT8   Ddr5AutoPrechargeEnable;
  UINT8   Lp5SplitACTEnable;
  UINT8   RetrainToWorkingChannel;
  UINT8   SubChHashOverride;
  UINT8   DisableMc0Ch0;
  UINT8   DisableMc0Ch1;
  UINT8   DisableMc0Ch2;
  UINT8   DisableMc0Ch3;
  UINT8   DisableMc1Ch0;
  UINT8   DisableMc1Ch1;
  UINT8   DisableMc1Ch2;
  UINT8   DisableMc1Ch3;

  UINT8   ChHashInterleaveBit;
  //
  // MRC Training Algorithms
  //
  UINT8   ECT;
  UINT8   SOT;
  UINT8   ERDMPRTC2D;
  UINT8   RDMPRT;
  UINT8   RCVET;
  UINT8   JWRL;
  UINT8   EWRTC2D;
  UINT8   ERDTC2D;
  UINT8   UNMATCHEDWRTC1D;
  UINT8   WRTC1D;
  UINT8   WRVC1D;
  UINT8   RDTC1D;
  UINT8   RDVC1D;
  UINT8   WRTC2D;
  UINT8   RDTC2D;
  UINT8   WRVC2D;
  UINT8   RDVC2D;
  UINT8   WRDSEQT;
  UINT8   DQSRF;
  UINT8   RDDQSODTT;
  UINT8   RDEQT;
  UINT8   DUNITC;
  UINT8   CMDVC;
  UINT8   LCT;
  UINT8   RTL;
  UINT8   TAT;
  UINT8   RMT;
  UINT8   RMTEVENODD;
  UINT8   ALIASCHK;
  UINT8   RCVENC1D;
  UINT8   RMC;
  UINT8   PRETRAIN;

  UINT8   DCCPICODELUT;
  UINT8   DIMMODTT;
  UINT8   DIMMRONT;
  UINT8   TXTCO;
  UINT8   CLKTCO;
  UINT8   CMDSR;
  UINT8   CMDDSEQ;
  UINT8   DIMMODTCA;
  UINT8   DDR5ODTTIMING;
  UINT8   DBI;
  UINT8   DLLDCC;
  UINT8   DLLBWSEL;
  UINT8   RDVREFDC;
  UINT8   RMTBIT;
  UINT8   DQDQSSWZ;
  UINT8   REFPI;
  UINT8   DCCLP5READDCA;
  UINT8   VCCCLKFF;
  UINT8   FUNCDCCDQS;
  UINT8   FUNCDCCCLK;
  UINT8   FUNCDCCWCK;
  UINT8   FUNCDCCDQ;
  UINT8   DATAPILIN;
  UINT8   DDR5XTALK;
  UINT8   DCCLP5WCKDCA;
  UINT8   RXUNMATCHEDCAL;
  UINT8   WRTDIMMDFE;

  UINT8   RXDQSDCC;
  UINT8   DIMMNTODT;
  UINT8   TXDQSDCC;
  UINT8   RXVREFPERBIT;
  UINT8   PPR;
  UINT8   LVRAUTOTRIM;
  UINT8   OPTIMIZECOMP;
  UINT8   WRTRETRAIN;
  UINT8   TLINECLKCAL;
  UINT8   DCCPISERIALCAL;
  UINT8   PHASECLKCAL;
  UINT8   WCKPADDCCCAL;
  UINT8   RDCTLET;
  UINT8   RDDQODTT;
  UINT8   EMPHASIS;
  UINT8   RXDQSVOCC;

  UINT8   MrcTimeMeasure;
  UINT8   MrcFastBoot;
  UINT8   RankInterleave;
  UINT8   EnhancedInterleave;
  UINT8   ChHashEnable;
  UINT8   EnablePwrDn;
  UINT8   EnablePwrDnLpddr;
  UINT8   SrefCfgEna;
  UINT8   ThrtCkeMinDefeatLpddr;
  UINT8   ThrtCkeMinDefeat;
  UINT8   AllowOppRefBelowWriteThrehold;
  UINT8   ForceSingleRank;
  UINT8   DynamicMemoryBoost;
  UINT8   RealtimeMemoryFrequency;
  UINT8   ExitOnFailure;
  UINT8   ForceRetrainPath;
  UINT8   SaGv;
  UINT8   IsDdr5MR7WicaSupported;
  UINT8   CccPinsInterleaved;
  UINT8   EnPeriodicComp;
  UINT8   AsyncOdtDis;
  UINT8   OpportunisticRead;
  UINT8   Disable2CycleBypass;
  UINT8   OCSafeMode;

  UINT16  SrefCfgIdleTmr;
  UINT16  ChHashMask;
  UINT16  DdrFreqLimit;
  UINT8   ThrtCkeMinTmr;
  UINT8   ThrtCkeMinTmrLpddr;
  UINT8   SaGvWpMask;
  UINT8   NModeSupport;
  UINT8   McRefreshRate;
  UINT8   PowerDownMode;
  UINT8   PwdwnIdleCounter;
  UINT8   RMTLoopCount;
  UINT8   GearRatio;
  UINT8   DdrOneDpc;
  UINT16  VddqVoltageOverride;
  UINT16  VccIogVoltageOverride;
  UINT16  VccClkVoltageOverride;
  UINT8   LpddrRttWr;
  UINT8   LpddrRttCa;
  UINT16  SaGvFreq[4];
  UINT8   SaGvGear[4];
  UINT8   IbeccProtectedRegionEnable[IBECC_REGIONS_MAX];
  UINT16  IbeccProtectedRegionBase[IBECC_REGIONS_MAX];
  UINT16  IbeccProtectedRegionOverallBase[IBECC_REGIONS_MAX];
  UINT16  IbeccProtectedRegionMask[IBECC_REGIONS_MAX];
  UINT16  MarginLimitL2;
  UINT8   WriteThreshold;
  UINT8   MarginLimitCheck;
  UINT8   Lfsr0Mask;
  UINT8   Lfsr1Mask;
  UINT8   RefreshWm;
  UINT8   DramRfmMode;
  UINT8   TargetedRowRefreshMode;
  UINT8   DrfmBrc;
  UINT8   SafeModeOverride;
  UINT32  DdrSafeMode;
  UINT8   McSafeMode;
  UINT8   LpMode;
  UINT8   LpMode4;
  UINT16  tRFCpb;
  UINT16  tRFC2;
  UINT16  tRFC4;
  UINT16  tRRD_L;
  UINT16  tRRD_S;
  UINT16  tWTR_L;
  UINT16  tCCD_L;
  UINT16  tWTR_S;
  UINT16  DimmOdt[MRC_TOTAL_DIMMODT_ENTRIES];
  UINT8   DFETap1StepSize;
  UINT8   DFETap2StepSize;
  UINT8   CsVrefLow;
  UINT8   CsVrefHigh;
  UINT8   CaVrefLow;
  UINT8   CaVrefHigh;
  UINT8   EccCorrectionMode;
  UINT8   EccGranularity32BEn;
  UINT8   CAVrefCtlOffset;
  UINT8   VrefCtlOffset;
  UINT8   ClkPiCodeOffset;
  UINT8   RcvEnOffset;
  UINT8   RxDqsOffset;
  UINT8   TxDqOffset;
  UINT8   TxDqsOffset;
  UINT8   VrefOffset;
  UINT8   CntrlrMask;
  UINT8   ChMask;
  UINT8   LowerBasicMemTestSize;
  UINT8   MCREGOFFSET;
  UINT8   PcuDdrVoltage;
  //
  // TurnAround Timing
  //
  // Read-to-Read
  UINT8   tRRSG;
  UINT8   tRRDG;
  UINT8   tRRDR;
  UINT8   tRRDD;
  // Write-to-Read
  UINT8   tWRSG;
  UINT8   tWRDG;
  UINT8   tWRDR;
  UINT8   tWRDD;
  // Write-to-Write
  UINT8   tWWSG;
  UINT8   tWWDG;
  UINT8   tWWDR;
  UINT8   tWWDD;
  // Read-to-Write
  UINT8   tRWSG;
  UINT8   tRWDG;
  UINT8   tRWDR;
  UINT8   tRWDD;

  UINT8   Interpreter;

  UINT16  RcompResistor;
  UINT16  RcompTarget[MRC_MAX_RCOMP_TARGETS];
  UINT16  tCCD_L_WR;

  //
  // PPR
  //
  UINT8   PprRunWCHMATS8;
  UINT8   PprRunRetention;
  UINT8   PprRunXMarch;
  UINT8   PprRunXMarchG;
  UINT8   PprRunYMarchShort;
  UINT8   PprRunYMarchLong;
  UINT8   PprRepairType;
  UINT8   PprRunOnce;
  UINT8   PprRunAtFastboot;
  UINT8   PprErrorInjection;
  UINT64  PprRepairPhysicalAddress;
  UINT32  PprRepairRow;
  UINT8   PprRepairController;
  UINT8   PprRepairChannel;
  UINT8   PprRepairDimm;
  UINT8   PprRepairRank;
  UINT8   PprRepairBankGroup;
  UINT8   PprRepairBank;
  UINT8   PprForceRepair;
  UINT8   BoardTopology;
  UINT16  SubChHashMask;
  UINT8   SubChHashInterleaveBit;
  UINT8   ForceCkdBypass;

  UINT8   MimicWcDisaplayInIpq;
  UINT8   FakeSagv;
  UINT8   DisableZq;
  UINT8   ReplicateSagv;
  UINT8   AdjustWckMode;
  UINT8   TelemetryControl;
  UINT8   SpineAndPhclkGateControl;
  UINT8   SpineGatePerLpmode;
  UINT8   PhclkGatePerLpmode;
  UINT8   DisableSwitchDfiToMc;
  UINT8   RowPressEn;
  UINT8   WeaklockEn;
  UINT8   RxDqsDelayCompEn;
  UINT8   IGpuGsm2Size;
  UINT8   SolFeatureEnabled;
  UINT8   MrcFailureOnUnsupportedDimm;
  UINT8   DCCDDR5READDCA;
  UINT8   PprRunMmrw;
  UINT8   PprRequestEnable[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestController[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestChannel[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestRank[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestBankGroup[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestBank[MRC_PPR_REQUEST_MAX];
  UINT32  PprRequestRow[MRC_PPR_REQUEST_MAX];
  UINT8   PprRequestDevice[MRC_PPR_REQUEST_MAX];
  UINT8   RDTCIDLE;
  UINT8   PprRetryLimit;
  UINT8   Use1p5ReadPostamble;
  UINT8   IbeccEccInjControl;
  UINT32  IbeccEccInjAddrBase;
  UINT8   IbeccEccInjCount;
  UINT8   MsHashOverride;
  UINT8   MsHashInterleaveBit;
  UINT16  MsHashMask;
  UINT8   FusaConfigEnable;

  UINT8   Module0Lockstep;
  UINT8   Module1Lockstep;
  UINT8   LpcLockstep;

  UINT32  PmtStreamTracerSize;
  UINT8   PmtStreamTracerMode;

  UINT8   IsWckIdleExitEnabled;
  UINT8   WREQT;

  UINT8   NestedSupport;
  UINT8   PostedInterruptSupport;
  UINT8   TbtPerfBoostEn;

  UINT8   FabricGVDisable;
  UINT8   Lp5SafeSpeed;
  UINT16  Vdd2HVoltage;
  UINT16  Vdd1Voltage;
  UINT16  Vdd2LVoltage;
  UINT16  VddqVoltage;
  UINT8   ForceInternalClkOn;
  UINT8   DIMMRXOFFSET;
  UINT8   FlexibleAnalogSettings;
  UINT8   ForceWRDSEQT2400;
  UINT8   NnFlexPhyRxEqTap0;
  UINT8   NnFlexPhyRxEqTap1;
  UINT8   NnFlexPhyDqTcoComp;
  UINT8   NnFlexPhyRxCtleR;
  UINT8   NnFlexPhyRxCtleC;
  UINT8   NnFlexPhyRxCtleRcmn;
  UINT8   NnFlexPhyRxCtleEq;
  UINT8   NnFlexPhyRxCtleTailCtl;
  UINT8   NnFlexLpddr5Dfeq;
  UINT8   NnFlexLpddr5PdDrvStr;
  UINT8   NnFlexLpddr5SocOdt;
  UINT8   NnFlexLpddr5PreEmpDn;
  UINT8   NnFlexLpddr5PreEmpUp;
  UINT8   NnFlexLpddr5WckDcaWr;
  UINT8   NnFlexLpddr5WckDcaRd;
  UINT8   NnFlexLpddr5RttNT;
  UINT8   NnFlexDramOvrdMask;
  UINT8   NnFlexDdr5DfeTap1;
  UINT8   NnFlexDdr5DfeTap2;
  UINT8   NnFlexDdr5RttWr;
  UINT8   NnFlexDdr5RttNomWr;
  UINT8   NnFlexDdr5RttNomRd;
  UINT8   NnFlexDdr5RonUp;
  UINT8   NnFlexDdr5RonDn;
  UINT8   NnFlexPhyOvrdMask;
  UINT8   ForceDIMMRXOFFSET;
  UINT8   Rsvd[31];
} SA_SETUP;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n

  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Add PchHdAudioPostProcessingModCustomGuid4,
                        PchHdAudioPostProcessingModCustomGuid5
  <b>Revision 3</b>:  - Add Usb31PortSpeed.
  <b>Revision 4</b>:  - Add PchXhciDwb.
  <b>Revision 5</b>:  - Add GlobalPcieAer.
  <b>Revision 6</b>:  - Add ThcHidI2cMaxFrameSize,
                        ThcHidI2cMaxFrameSizeValue,
                        ThcHidI2cIntDelay,
                        ThcHidI2cIntDelayValue.
**/

#define PCH_SETUP_REVISION  7

typedef struct {
  UINT8   Revision;
  UINT8   ExternalDma;
  UINT16  RootPortDid;              // not a SETUP item, used by BIOS to pass Root Port DID to SETUP items

  UINT8   PmcReadDisable;
  UINT8   OsDebugPort; // deprecated, moved to SETUP_DATA
  UINT8   PchLan;
  UINT8   PchWakeOnLan;
  UINT8   SlpLanLowDc;
  UINT8   PchWakeOnWlan;
  UINT8   PchEnergyReport;
  UINT8   EnableTcoTimer;
  UINT8   PchCrid;
  UINT8   PmcLpmS0i2p0En;
  UINT8   PmcLpmS0i2p1En;
  UINT8   PmcLpmS0i2p2En;
  UINT8   PmcLpmS0i3p0En;
  UINT8   PmcLpmS0i3p1En;
  UINT8   PmcLpmS0i3p2En;
  UINT8   PmcLpmS0i3p3En;
  UINT8   PmcLpmS0i3p4En;
  UINT8   PchPmcLpmS0i2p0En;
  UINT8   DeepSxMode;
  UINT8   LanWakeFromDeepSx;
  UINT8   DisableDsxAcPresentPulldown;
  UINT8   PchWakeOnWlanDeepSx;

  UINT8   PchRtcMemoryLock;
  UINT8   PchBiosLock;
  UINT8   UnlockGpioPads;
  UINT8   PsOnEnable;

  UINT8   StateAfterG3;
  UINT8   IchPort80Route;
  UINT8   EnhancePort8xhDecoding;
  UINT8   PciePllSsc; //deprecated
  UINT8   WdtEnable;
  UINT8   PchS0ixAutoDemotion;
  UINT8   PchLatchEventsC10Exit;
  //
  // Usb Config
  //
  UINT8   PchUsbPortDisable;
  UINT8   PchUsbHsPort[SETUP_MAX_USB2_PORTS];
  UINT8   PchUsbSsPort[SETUP_MAX_USB3_PORTS];
  UINT8   PchUsbPdoProgramming;
  UINT8   PchUsbOverCurrentEnable;
  UINT8   PchXhciOcLock;
  UINT8   PchXhciAcLock;
  UINT8   PchUsb2SusWellPgEnable;
  UINT8   PchUsb31Speed; //deprecated
  UINT8   PchUsbSwDevModeEnable[SETUP_MAX_USB2_PORTS];

  //
  // xDCI Config
  //
  UINT8   PchXdciSupport;

  //
  // Sata CONFIG
  //
  UINT8   PchSata;
  UINT8   SataInterfaceMode;
  UINT8   SataPort[PCH_MAX_SATA_PORTS];
  UINT8   SataHotPlug[PCH_MAX_SATA_PORTS];
  UINT8   SataMechanicalSw[PCH_MAX_SATA_PORTS];
  UINT8   SataSpinUp[PCH_MAX_SATA_PORTS];
  UINT8   SataExternal[PCH_MAX_SATA_PORTS];
  UINT8   SataType[PCH_MAX_SATA_PORTS];
  UINT8   SataTopology[PCH_MAX_SATA_PORTS];
  UINT8   SataRaidR0;
  UINT8   SataRaidR1;
  UINT8   SataRaidR10;
  UINT8   SataRaidR5;
  UINT8   SataRaidIrrt;
  UINT8   SataRaidOub;
  UINT8   SataHddlk;
  UINT8   SataLedl;
  UINT8   SataRaidIooe;
  UINT8   SataRaidSrt;
  UINT8   SataRaidOromDelay;
  UINT8   SataRstForceForm;
  UINT8   SataLegacyOrom;
  UINT8   SataRstOptaneMemory;
  UINT8   SataRstCpuAttachedStorage;
  UINT8   RaidDeviceId;
  UINT8   SataSalp;
  UINT8   SataTestMode;
  UINT8   PxDevSlp[PCH_MAX_SATA_PORTS];
  UINT8   EnableDitoConfig[PCH_MAX_SATA_PORTS];
  UINT16  DitoVal[PCH_MAX_SATA_PORTS];
  UINT8   DmVal[PCH_MAX_SATA_PORTS];

  //
  // PCI_EXPRESS_CONFIG, MAX 16 root ports
  //
  UINT8   PcieRootPort8xhDecode;
  UINT8   Pcie8xhDecodePortIndex;
  UINT8   PcieRootPortPeerMemoryWriteEnable[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieComplianceTestMode;

  // @TODO:
  //  "GetPchMaxPcieClockNum(18)" is for ADL-S specific, and the value is larger than PCH_MAX_PCIE_CLOCKS(16)
  //  For long term, we need to have a good way to deal with the condition.
  UINT8   PchPcieClockUsageOverride[22];
  UINT8   PchPcieClkReqSupport[22];

  UINT8   PcieRootPortEn[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAspm[MAX_PCIE_ROOT_PORTS];
  UINT8   HostL0sTxDis[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortURE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortFEE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortNFE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortCEE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortCTD[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPIE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSFE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSNE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSCE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPMCE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortHPE[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortAER[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSpeed[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortTHS[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortL1SubStates[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortACS[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortPTM[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortSI[MAX_PCIE_ROOT_PORTS];
  UINT16  PcieDetectTimeoutMs[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortMaxPayload[MAX_PCIE_ROOT_PORTS];
  UINT8   RpFunctionSwap;
  UINT8   PcieResizableBarSupport;

  //
  // PCIe EQ
  //
  UINT8   PcieEqOverrideDefault[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortEqPhBypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortGen3EqPh3Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortGen3EqPh23Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3PcetTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3TsLockTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqMethod[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqMode[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh2LocalTxOverrideEn[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh1DpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh1UpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3NoOfPresetOrCoeff[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PreCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3PostCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh3Preset10List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen3EqPh2LocalTxOverridePreset[MAX_PCIE_ROOT_PORTS];

  UINT8   PcieRootPortGen4EqPh3Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortGen4EqPh23Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4PcetTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4TsLockTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqMethod[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqMode[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh2LocalTxOverrideEn[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh1DpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh1UpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3NoOfPresetOrCoeff[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PreCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3PostCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh3Preset10List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen4EqPh2LocalTxOverridePreset[MAX_PCIE_ROOT_PORTS];

  UINT8   PcieRootPortGen5EqPh3Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieRootPortGen5EqPh23Bypass[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5PcetTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5TsLockTimer[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqMethod[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqMode[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh2LocalTxOverrideEn[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh1DpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh1UpTxPreset[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3NoOfPresetOrCoeff[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PreCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3PostCursor9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset0List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset1List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset2List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset3List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset4List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset5List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset6List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset7List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset8List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset9List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh3Preset10List[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieGen5EqPh2LocalTxOverridePreset[MAX_PCIE_ROOT_PORTS];

  UINT8   PcieSetSecuredRegisterLock;
  UINT8   PcieAspmOc[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieDump[MAX_PCIE_ROOT_PORTS];

  //
  // PCIe LTR Configuration
  //
  UINT8   PchPcieLtrEnable[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieLtrConfigLock[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieLtrOverrideSpecCompliant[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideMode[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieSnoopLatencyOverrideMultiplier[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMode[MAX_PCIE_ROOT_PORTS];
  UINT8   PchPcieNonSnoopLatencyOverrideMultiplier[MAX_PCIE_ROOT_PORTS];
  UINT16  PchPcieSnoopLatencyOverrideValue[MAX_PCIE_ROOT_PORTS];
  UINT16  PchPcieNonSnoopLatencyOverrideValue[MAX_PCIE_ROOT_PORTS];


  //
  // Completion Timeout value
  //
  UINT8   CompletionTimeout[MAX_PCIE_ROOT_PORTS];

  //
  // DMI Configuration
  //
  UINT8   DmiMaxLinkSpeed;
  UINT8   DmiGen3EqPh2Enable;
  UINT8   DmiGen3EqPh3Method;
  UINT8   DmiGen3ProgramStaticEq;
  UINT8   DmiGen3RootPortPreset[8];
  UINT8   DmiGen3EndPointPreset[8];
  UINT8   DmiGen3EndPointHint[8];
  UINT8   DmiAspm;
  UINT8   DmiAspmL1ExitLatency;
  UINT8   DmiGen3PcetTimer;
  UINT8   DmiGen4PcetTimer;
  UINT8   DmiGen3Ltcpre[8];
  UINT8   DmiGen3Ltcpo[8];
  UINT8   DmiGen4Ltcpre[8];
  UINT8   DmiGen4Ltcpo[8];
  UINT8   Gen3LtcoEnable;
  UINT8   Gen3RtcoRtpoEnable;
  UINT8   Gen4LtcoEnable;
  UINT8   Gen4RtcoRtpoEnable;
  UINT8   DmiGen3Hapcsb;
  UINT8   DmiGen4Hapcsb;
  UINT8   PchTestDmiMeUmaRootSpaceCheck;
  UINT8   PchTestDmiTranCoOverEn[8];
  UINT8   DmiSrl;
  UINT8   DmiL1ssEnable;

  //
  // THC Configuration
  //
  UINT8   ThcAssignment[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcClock[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcWakeOnTouch[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcMode[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcDsyncPad[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcActiveLtr[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcIdleLtr[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcPerformanceLimitation[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcResetPad[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcResetPadTrigger[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiFlags[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiConnectionSpeed[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiInputReportBodyAddress[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiInputReportHeaderAddress[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiOutputReportAddress[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiReadOpcode[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiWriteOpcode[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidSpiLimitPacketSize[PCH_MAX_THC_CONTROLLERS];
  //
  // IEH Configuration
  //
  UINT8  IehMode;

  //
  // PCI Bridge Resources
  //
  UINT8   PcieExtraBusRsvd[MAX_PCIE_ROOT_PORTS];
  UINT16  PcieMemRsvd[MAX_PCIE_ROOT_PORTS];
  UINT8   PcieIoRsvd[MAX_PCIE_ROOT_PORTS];

  //
  // HD-Audio Configuration
  //
  UINT8   PchHdAudio;
  UINT8   PchHdAudioDsp;
  UINT8   PchHdAudioDspUaaCompliance;
  UINT8   PchHdaIDisplayCodecDisconnect;
  UINT8   PchHdAudioPme;
  UINT8   PchHdAudioHdaLinkEnable;
  UINT8   PchHdAudioDmicLinkEnable[PCH_MAX_HDA_DMIC_LINK_NUM];
  UINT8   PchHdAudioDmicClockSelect[PCH_MAX_HDA_DMIC_LINK_NUM];
  UINT8   PchHdAudioSndwLinkEnable[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdAudioSspLinkEnable[PCH_MAX_HDA_SSP_LINK_NUM];
  UINT8   PchHdAudioSndwMultilaneEnable[PCH_MAX_HDA_SNDW_MULTILANE_NUM];
  UINT8   PchHdaHdAudioLinkFreq;
  UINT8   PchHdaIDisplayLinkFreq;
  UINT8   PchHdaIDisplayLinkTmode;
  UINT8   PchHdAudioCodecSxWakeCapability;
  UINT8   PchHdaAutonomousClockStopSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnActiveIntervalSelectSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnActiveIntervalExtSelectSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnActiveIntervalExt2SelectSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnDelaySelectSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnDelaySelectExtSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaDataOnDelaySelectExt2Sndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaClockLoopbackDelaySelectSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaClockLoopbackEnableSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdaClockLoopbackSourceSndw[PCH_MAX_HDA_SNDW_LINK_NUM];
  UINT8   PchHdAudioI2sCodecSelect;
  UINT8   HdaDiscBtOffEnabled;
  UINT8   HdaDiscBtOffSspLink;
  UINT8   PchHdAudioFeature[HDAUDIO_FEATURES];
  UINT8   PchHdAudioPostProcessingMod[HDAUDIO_PP_MODULES];
  CHAR16  PchHdAudioPostProcessingModCustomGuid1[GUID_CHARS_NUMBER];
  CHAR16  PchHdAudioPostProcessingModCustomGuid2[GUID_CHARS_NUMBER];
  CHAR16  PchHdAudioPostProcessingModCustomGuid3[GUID_CHARS_NUMBER];
  UINT8   PchHdAudioSdiLaneEnable[PCH_MAX_HDA_SDI];
  UINT8   PchHdaAlc245DmicConfiguration;
  UINT8   PchHdaMicPrivacyMode;
  UINT8   PchHdaMicPrivacyDeglitch;
  UINT8   PchHdaMicPrivacyHwModeSoundWire0;
  UINT8   PchHdaMicPrivacyHwModeSoundWire1;
  UINT8   PchHdaMicPrivacyHwModeSoundWire2;
  UINT8   PchHdaMicPrivacyHwModeSoundWire3;
  UINT8   PchHdaMicPrivacyHwModeSoundWire4;
  UINT8   PchHdaMicPrivacyHwModeDmic;
  UINT32  PchHdaMicPrivacyTimeout;
  UINT8   PchHdaSndwClockSrcSelect;

  //
  // Interrupt Configuration
  //
  UINT8   PchIoApic24119Entries;
  UINT8   Enable8254ClockGating;

  //
  // SCS Configuration
  //
  UINT8   PchScsUfsEnable[PCH_MAX_UFS_DEV_NUM];
  UINT8   PchScsUfsInlineEncryption[PCH_MAX_UFS_DEV_NUM];

  //
  // Integrated Sensor Hub (ISH) configuration
  //
  UINT8   PchIshEnable;
  UINT8   PchIshSpiEnable[PCH_MAX_ISH_SPI_CONTROLLERS];
  UINT8   PchIshSpiCs0Enable[PCH_MAX_ISH_SPI_CONTROLLERS];
  UINT8   PchIshUartEnable[PCH_MAX_ISH_UART_CONTROLLERS];
  UINT8   PchIshI2cEnable[PCH_MAX_ISH_I2C_CONTROLLERS];
  UINT8   PchIshI3cEnable[PCH_MAX_ISH_I3C_CONTROLLERS];
  UINT8   PchIshGpEnable[PCH_MAX_ISH_GP_PINS];
  UINT8   PchIshPdtUnlock;
  UINT8   PchIshMsiInterrupt;

  //
  // Pch Serial Io configuration
  //
  UINT8   PchSpiCsPolarity[14]; // [PCH_MAX_SERIALIO_SPI_CONTROLLERS * PCH_MAX_SERIALIO_SPI_CHIP_SELECTS]
  UINT8   PchSerialIoSpi[PCH_MAX_SERIALIO_SPI_CONTROLLERS];
  UINT8   PchSpiCsPolaritySelect[PCH_MAX_SERIALIO_SPI_CHIP_SELECTS];
  UINT8   PchSerialIoUart[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartDmaEnable[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartHwFlowCtrl[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchUartPowerGating[PCH_MAX_SERIALIO_UART_CONTROLLERS];
  UINT8   PchSerialIoI2c[PCH_MAX_SERIALIO_I2C_CONTROLLERS];
  UINT8   PchSerialIoDeviceI2C4Support;
  UINT8   PchSerialIoI3c[PCH_MAX_SERIALIO_I3C_CONTROLLERS];
  UINT8   PchGpio;
  UINT8   PchGpioIrqRoute;
  UINT16  PchGpioDebounce;
  UINT8   PchGpioTestDevices;
  UINT8   PchAdditionalSerialIoDevices;

  //
  // TraceHub Setup Options
  //
  UINT8   SocTraceHubMode;
  UINT16  SocTraceHubMemReg0Size;
  UINT16  SocTraceHubMemReg1Size;
  UINT8   PchTraceHubMode;
  UINT16  PchTraceHubMemReg0Size;
  UINT16  PchTraceHubMemReg1Size;
  UINT8   AetTraceHubMode;      //mutual exclusive option for AET to select TraceHub destination, {0:SOC TH, 1:PCH TH}
  UINT8   BiosTraceSinkMode;    //mutual exclusive option to select BIOS trace destination, {0:SOC TH, 1:PCH TH}

  //
  // Thermal Throttling Level options
  //
  UINT8   SocTtLevelSuggestSet;
  UINT16  SocThrmT0Level;
  UINT16  SocThrmT1Level;
  UINT16  SocThrmT2Level;
  UINT8   SocThrmTtEnable;
  UINT8   SocThrmTtState13Enable;
  UINT8   SocThrmTtLock;

  UINT8   PchTtLevelSuggestSet;
  UINT16  PchThrmT0Level;
  UINT16  PchThrmT1Level;
  UINT16  PchThrmT2Level;
  UINT8   PchThrmTtEnable;
  UINT8   PchThrmTtState13Enable;
  UINT8   PchThrmTtLock;

  UINT8   IoeTtLevelSuggestSet;
  UINT16  IoeThrmT0Level;
  UINT16  IoeThrmT1Level;
  UINT16  IoeThrmT2Level;
  UINT8   IoeThrmTtEnable;
  UINT8   IoeThrmTtLock;
  //
  // DMI Thermal Throttling
  //
  UINT8   PchDmiTsSuggestSet;
  UINT8   PchTs0Width;
  UINT8   PchTs1Width;
  UINT8   PchTs2Width;
  UINT8   PchTs3Width;
  UINT8   PchDmiTsawEn;
  //
  // SATA Thermal Throttling
  //
  UINT8   PchSataTsSuggestSet;
  UINT8   PchP0T1M;
  UINT8   PchP0T2M;
  UINT8   PchP0T3M;
  UINT8   PchP0TDisp;
  UINT8   PchP0Tinact;
  UINT8   PchP0TDispFinit;
  UINT8   PchP1T1M;
  UINT8   PchP1T2M;
  UINT8   PchP1T3M;
  UINT8   PchP1TDisp;
  UINT8   PchP1Tinact;
  UINT8   PchP1TDispFinit;
  //
  // CNVi
  //
  UINT8   CnviMode;
  UINT8   CnviWifiCore;
  UINT8   CnviBtCore;
  UINT8   CnviBtAudioOffload;
  //
  // eSPI Setup Options
  //
  UINT8   PchEspiLgmrEnable;
  UINT8   PchEspiHostC10ReportEnable;

  //
  // SMBUS
  //
  UINT8   SmbusSpdWriteDisable;
  //
  // P2SB
  //
  UINT8   PchSidebandLock;
  //
  // Flash Protection configuration
  //
  UINT8   FprrEnable;

  //
  // Debug Panel
  //
  UINT8   DciEn;
  UINT8   DciDbcMode;
  UINT8   DciClkEnable;
  UINT8   KeepEarlyTrace;
  UINT8   IoeDebugEn;
  UINT8   PmodeClkEn;

  //
  // FIVR Configuration;
  //
  UINT8   ExternalV1p05StateEnable[FIVR_RAIL_S0IX_SX_STATES_MAX];
  UINT8   ExternalVnnStateEnable[FIVR_RAIL_S0IX_SX_STATES_MAX];
  UINT8   ExternalVnnSxStateEnable[FIVR_RAIL_S0IX_SX_STATES_MAX];
  UINT8   ExternalV1p05SupportedVoltageStates[FIVR_RAIL_EXT_VOLTAGE_STATES_MAX];
  UINT8   ExternalVnnSupportedVoltageStates[FIVR_RAIL_EXT_VOLTAGE_STATES_MAX];

  UINT16  ExternalV1p05Voltage;
  UINT16  ExternalVnnVoltage;
  UINT16  ExternalVnnSxVoltage;

  UINT8   PchRetToLowCurModeVolTranTime;
  UINT8   PchRetToHighCurModeVolTranTime;
  UINT8   PchLowToHighCurModeVolTranTime;
  UINT16  PchOffToHighCurModeVolTranTime;
  UINT8   PchFivrDynPm;

  //
  // OS IDLE MODE
  //
  UINT8   PchPmcOsIdleModeEnable;
  UINT8   PchLegacyIoLowLatency;

  //
  // PSF
  //
  UINT8   PsfTccEnable;

  //
  // TSN
  //
  UINT8   PchTsnEnable[PCH_MAX_TSN_PORT];
  UINT8   TsnPcsEnabled;

  // FUSA
  //
  UINT8   PsfFusaConfigEnable;  //deprecated

  UINT16  ExternalV1p05IccMaximum;
  UINT16  ExternalVnnIccMaximum;
  UINT16  ExternalVnnSxIccMaximum;
  UINT8   PmcC10DynamicThresholdAdjustment;

  // Extended BIOS Decode Range
  UINT8   ExtendedBiosDecodeRangeEnable;

  //
  // PMC configuration
  //
  UINT8   PmcAdrEn;
  UINT8   PmcAdrTimer1Val;
  UINT8   PmcAdrMultiplier1Val;
  UINT8   PmcAdrHostPartitionReset;
  UINT8   SocBclkPllOn;
  UINT8   CpuBclkPllOn;
  UINT8   PmcPrivacyConsentSupport;
  UINT8   PmcPrivacyConsent;
  UINT8   PmcWdtTimerEn;

  UINT8   PcieRootPortClockGating[MAX_PCIE_CONTROLLERS];
  UINT8   PcieRootPortPowerGating[MAX_PCIE_CONTROLLERS];
  UINT8   PcieRootPortAutoPowerGating[MAX_PCIE_CONTROLLERS];
  UINT8   PcieRootPortVisaClockGating[MAX_PCIE_CONTROLLERS];

  //
  // UAOL setup option
  //
  UINT8   PchXhciUaol;
  UINT8   PcieFomsCp[MAX_PCIE_ROOT_PORTS];

  //
  // Serial IO UART - Kernel support
  //
  UINT8   SerialIoDebugUartPowerGating;

  UINT8   PcieFiaProgramming;
  UINT8   PchXhciHsiiEnable;
  UINT8   PchPmErDebugMode;
  UINT8   PciePhyAutoPowerGating;

  //
  // PCH LAN: WOL Fast Startup Support
  //
  UINT8   PchWOLFastSupport;

  UINT8   DmiUpPortTranPresetEn;
  UINT8   PchDmiUpPortTranPreset[PCH_MAX_DMI_LANES];
  UINT8   PchDmiTranCoOverEn[PCH_MAX_DMI_LANES];
  UINT8   PchDmiTranCoOverPreCur[PCH_MAX_DMI_LANES];
  UINT8   PchDmiTranCoOverPostCur[PCH_MAX_DMI_LANES];
  UINT8   PchDmiRtlepceb;

  UINT32  ThcDisplayFrameSyncPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcTimestampTimerMode[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cDeviceAddress[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cConnectionSpeed[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcHidI2cAddressingMode[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cDeviceDescriptorAddress[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cStandardModeSerialClockLineHighPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cStandardModeSerialClockLineLowPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cStandardModeSerialDataLineTransmitHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cStandardModeSerialDataLineReceiveHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModeSerialClockLineHighPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModeSerialClockLineLowPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModeSerialDataLineTransmitHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModeSerialDataLineReceiveHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cMaxSuppressedSpikesSMFMFMP[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModePlusSerialClockLineHighPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModePlusSerialClockLineLowPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModePlusSerialDataLineTransmitHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cFastModePlusSerialDataLineReceiveHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cHighSpeedModePlusSerialClockLineHighPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cHighSpeedModePlusSerialClockLineLowPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cHighSpeedModePlusSerialDataLineTransmitHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cHighSpeedModePlusSerialDataLineReceiveHoldPeriod[PCH_MAX_THC_CONTROLLERS];
  UINT32  ThcHidI2cMaximumLengthOfSuppressedSpikesInHighSpeedMode[PCH_MAX_THC_CONTROLLERS];


  // Hammock Harbor
  UINT8   EnableTimedGpio0;
  UINT8   EnableTimedGpio1;
  UINT8   PcieRootPortLinkDownGpios[PCH_MAX_PCIE_ROOT_PORTS];

  // THC Wake On Touch GPIO Pin configurations for ACPI GPIO resource
  UINT8  ThcWotEdgeLevel[PCH_MAX_THC_CONTROLLERS];
  UINT8  ThcWotActiveLevel[PCH_MAX_THC_CONTROLLERS];
  UINT8  ThcWotPinConfig[PCH_MAX_THC_CONTROLLERS];

  // THC Customized SubSystem Device ID and SubSystem Vendor ID
  UINT16  ThcCustomizedSsdid[PCH_MAX_THC_CONTROLLERS];
  UINT16  ThcCustomizedSsvid[PCH_MAX_THC_CONTROLLERS];


  UINT8   PchUsb31PortSpeed[SETUP_MAX_USB3_PORTS];

  // Usb Config
  UINT8   PchXhciDwb;
  UINT8   GlobalPcieAer;
  UINT8   VisaDebug;

  UINT8   Reserved1[19];
  CHAR16  PchHdAudioPostProcessingModCustomGuid4[GUID_CHARS_NUMBER];
  CHAR16  PchHdAudioPostProcessingModCustomGuid5[GUID_CHARS_NUMBER];
  UINT8   ThcHidI2cMaxFrameSize[PCH_MAX_THC_CONTROLLERS];
  UINT16  ThcHidI2cMaxFrameSizeValue[PCH_MAX_THC_CONTROLLERS];
  UINT8   ThcHidI2cIntDelay[PCH_MAX_THC_CONTROLLERS];
  UINT16  ThcHidI2cIntDelayValue[PCH_MAX_THC_CONTROLLERS];

} PCH_SETUP;


/**
  Making any setup structure change after code frozen
  will need to maintain backward compatibility, bump up
  structure revision and update below history table.\n
  <b>Revision 1</b>:  - Initial version.
**/

#define SI_SETUP_REVISION  1

typedef struct {
  UINT8   Revision;
  UINT8   PlatformDebugOption;
  //
  // PCIe IMR
  //
  UINT8   PcieImrEnable;
  UINT16  PcieImrSize;
  UINT8   PcieImrRpLocation;
  UINT8   PcieImrRpSelection;
} SI_SETUP;

#define DEBUG_SETUP_VOLATILE_DATA_REVISION 1
typedef struct {
  UINT8   StreamingTraceSink;
  UINT8   PchIsAttached;
  UINT8   ProcessorTraceEnable;
} DEBUG_SETUP_VOLATILE_DATA;

#define TOKEN_BIOS_PAYLOAD_REVISION 1

typedef struct {
  UINT8                   PlatformDebugOption;
} DEBUG_SETUP_BACKUP;

typedef struct {
  UINT8                   TokenExist;         // Token existence status for previous boot
  DEBUG_SETUP_BACKUP      DebugSetupBackup;   // Backup Debug Setp before Token injection
} BIOS_PAYLOAD;

#define BIOS_PAYLOAD_VARIABLE_NAME             L"BiosPayload"

#define BOARD_INFO_SETUP_REVISION  3

typedef struct {
  // @todo decouple Board related variables from SETUP_DATA
  UINT16 FabId;
  UINT16 BoardBomId;
//  UINT8  PchGeneration;
  BOOLEAN EcEnable;
  UINT8   EcEspiFlashSharingMode;
  UINT8   EcMajorRevision;
  UINT8   EcMinorRevision;
  UINT8   EcPlatformMajorRevision;
  UINT8   EcBuildRevision;
  UINT16  BoardId;
  BOOLEAN SpdPresent;
  UINT8   PlatformGeneration;
  UINT16  DisplayId;
  BOOLEAN EcPresent;
  UINT32  RevisionId;
} BOARD_INFO_SETUP;

/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
**/
#define OC_SETUP_REVISION  1
typedef struct {
  UINT8   Revision;
  //
  // OverClocking setup options
  //
  UINT8   OverclockingSupport;
  UINT8   FllOverclockMode;
  UINT8   PvdRatioThreshold[2];
  UINT8   PvdMode[2];
  UINT8   Avx2RatioOffset;

  //
  // PLL Vlotage Offset related
  //
  UINT8   CorePllVoltageOffset;
  UINT8   RingPllVoltageOffset;
  UINT8   IaAtomPllVoltageOffset;

  //
  // HyerThreading Disable
  //
  UINT16  PerCoreHtDisable;

  //
  // Thermal Velocity Boost options
  //
  UINT8   TvbRatioClipping;
  UINT8   TvbVoltageOptimization;
  //
  // CPU BCLK OC Frequency
  //
  UINT32  CpuBclkOcFrequency;
  //
  // SOC BCLK OC Frequency
  //
  UINT32  SocBclkOcFrequency;

  UINT8   PerCoreDisableConfiguration;
  //
  // IA/GT/SA ICC MAX settings
  //
  UINT8   IaIccUnlimitedMode;
  UINT8   GtIccUnlimitedMode;
  UINT8   SaIccUnlimitedMode;

  UINT16  IaIccMax;
  UINT16  GtIccMax;
  UINT16  SaIccMax;
  //
  // VCCSA Boot Voltage
  //
  UINT8   VccsaBootVoltageSel;
  //
  // CPU D2D Ratio
  //
  UINT8   CpuD2dRatio;
  //
  // CPU Bandgap Reference Mode
  //
  UINT8   CpuBandgapRefMode;
  //
  // VCCIA Boot Voltage
  //
  UINT8   VcciaBootVoltageSel;
  //
  // Granular Ratio Override Support
  //
  UINT8   GranularRatioSupported;
  UINT8   GranularRatioOverride;
  UINT8   UnderVoltProtection;
  UINT8   OverclockingMsrStatus;
  UINT8   PerCoreGranularityBins[MAX_OC_BIG_CORES];
  UINT8   PerAtomClusterGranularityBins[MAX_OC_ATOM_CLUSTERS];

  UINT8   BclkAdaptiveVoltageEnable;

  UINT8   Avx2VoltageScaleFactor;
  //
  // TjMax
  //
  UINT8   TjMaxOffset;

  //
  // External voltage rails overrides
  //
  UINT8   Vcc1p8CpuOverrideEnable;
  UINT16  Vcc1p8CpuOverrideVoltage;
  UINT8   Vcc1p8DdrOverrideEnable;
  UINT16  Vcc1p8DdrOverrideVoltage;
  UINT8   VccDd2OverrideEnable;
  UINT16  VccDd2OverrideVoltage;
  UINT8   VnnaonOverrideEnable;
  UINT16  VnnaonOverrideVoltage;
  UINT8   VccIoOverrideEnable;
  UINT16  VccIoOverrideVoltage;
  UINT8   V0p82OverrideEnable;
  UINT16  V0p82OverrideVoltage;
  UINT8   V3p3OverrideEnable;
  UINT16  V3p3OverrideVoltage;
  UINT8   V1p8OverrideEnable;
  UINT16  V1p8OverrideVoltage;
  UINT8   RichtekVccIaEnable;
  UINT8   RichtekVccGtEnable;
  UINT8   RichtekVccSaEnable;
  UINT8   RichtekVccIaFixedVidMode;
  UINT8   RichtekVccGtFixedVidMode;
  UINT8   RichtekVccSaFixedVidMode;
  UINT16  RichtekVccIaVoltage;
  UINT16  RichtekVccGtVoltage;
  UINT16  RichtekVccSaVoltage;

  UINT8   PcoreTvbTempThreshold0;
  UINT8   PcoreTvbTempThreshold1;
  UINT8   EcoreTvbTempThreshold0;
  UINT8   EcoreTvbTempThreshold1;
  UINT8   PerPcoreRatioDownBinAboveT0[MAX_OC_BIG_CORES];
  UINT8   PerPcoreRatioDownBinAboveT1[MAX_OC_BIG_CORES];
  UINT8   PerPcoreGrRatioDownBinAboveT0[MAX_OC_BIG_CORE_TRL_GROUPS];
  UINT8   PerPcoreGrRatioDownBinAboveT1[MAX_OC_BIG_CORE_TRL_GROUPS];
  UINT8   PerEcoreCcpRatioDownBinAboveT0[MAX_OC_ATOM_CLUSTERS];
  UINT8   PerEcoreCcpRatioDownBinAboveT1[MAX_OC_ATOM_CLUSTERS];

  UINT8   OcTvb;
  UINT8   TvbConfigLimitSelect;
  //
  // TSC HW Fixup disable Configuration
  //
  UINT8   TscDisableHwFixup;

  UINT8   CpuDlvrMode;

  UINT8   CoreOpPointReportingEn; // Enables Core Operating point reporting
  //
  // CPU: Enable SSC config for Compute-Die and Soc-Die
  //
  UINT8   ComputeDieSscEnable;
  UINT8   SocDieSscEnable;

  UINT8   CoreVfConfigScope;

  //
  // Per Core related
  //
  BOOLEAN IsPerCoreVotage;
  UINT8   PerCoreRatioOverride;
  UINT8   RingDownBin;
  //
  // Processor Disable control
  //
  UINT64  PerCoreDisableOriMask;
  UINT8   PerCoreDisable[MAX_OC_CORES];
  UINT8   PerCoreVoltageMode[MAX_OC_BIG_CORES];                  ///< Array used to specifies the selected Core Voltage Mode.
  UINT16  PerCoreVoltageOverride[MAX_OC_BIG_CORES];              ///< Array used to specifies the selected Core Voltage Override.
  UINT16  PerCoreAdaptiveVoltage[MAX_OC_BIG_CORES];              ///< Array used to specifies the selected Core Adaptive Voltage.
  UINT8   PerCoreMaxRatio[MAX_OC_BIG_CORES];
  UINT16  PerCoreVoltageOffset[MAX_OC_BIG_CORES];
  UINT8   PerCoreVoltageOffsetPrefix[MAX_OC_BIG_CORES];
  UINT8   PerCoreRatio[MAX_OC_BIG_CORES];

  //
  // Per Atom related
  //
  UINT8   PerAtomClusterVoltageMode[MAX_OC_ATOM_CLUSTERS];       ///< Array used to specifies the selected Atom Core Voltage Mode.
  UINT16  PerAtomClusterVoltageOverride[MAX_OC_ATOM_CLUSTERS];   ///< Array used to specifies the selected Atom Core Voltage Override.
  UINT16  PerAtomClusterAdaptiveVoltage[MAX_OC_ATOM_CLUSTERS];   ///< Array used to specifies the selected Atom Core Adaptive Voltage.
  UINT8   PerAtomClusterMaxRatio[MAX_OC_ATOM_CLUSTERS];
  UINT16  PerAtomClusterVoltageOffset[MAX_OC_ATOM_CLUSTERS];
  UINT8   PerAtomClusterVoltageOffsetPrefix[MAX_OC_ATOM_CLUSTERS];
  UINT8   AtomClusterRatio[MAX_OC_ATOM_CLUSTERS];

  //
  // Overclocking OcDomain VF Config
  //
  UINT8   MaxOcRatio[MAX_OC_DOMAINS];                 ///< Maximum turbo ratio override
  UINT8   VoltageMode[MAX_OC_DOMAINS];                ///< Specifies whether voltage is operating in Adaptive or Override mode - 0: Adaptive, 1: Override
  UINT16  VoltageOffset[MAX_OC_DOMAINS];              ///< The voltage offset applied to. Valid range from -1000mv to 1000mv
  UINT8   VoltageOffsetPrefix[MAX_OC_DOMAINS];
  UINT16  VoltageOverride[MAX_OC_DOMAINS];            ///< The voltage override which is applied to the entire range of frequencies
  UINT16  AdaptiveVoltage[MAX_OC_DOMAINS];            ///< The adaptive voltage applied during turbo frequencies. Valid range from 0 to 2000mV
  UINT8   VfPointOffsetMode[MAX_OC_DOMAINS];
  UINT8   VfPointCount[MAX_OC_DOMAINS];
  //
  // VF Point Selection Mode Support
  //
  UINT16  CoreVfPointOffset[MAX_OC_VF_POINTS];
  UINT8   CoreVfPointOffsetPrefix[MAX_OC_VF_POINTS];
  UINT16  GtVfPointOffset[MAX_OC_VF_POINTS];
  UINT8   GtVfPointOffsetPrefix[MAX_OC_VF_POINTS];
  UINT16  RingVfPointOffset[MAX_OC_VF_POINTS];
  UINT8   RingVfPointOffsetPrefix[MAX_OC_VF_POINTS];
  UINT16  MemSSVfPointOffset[MAX_OC_VF_POINTS];
  UINT8   MemSSVfPointOffsetPrefix[MAX_OC_VF_POINTS];
  UINT16  NguVfPointOffset[MAX_OC_VF_POINTS];
  UINT8   NguVfPointOffsetPrefix[MAX_OC_VF_POINTS];
} OC_SETUP;

#pragma pack()

#ifndef PLATFORM_SETUP_VARIABLE_NAME
#define PLATFORM_SETUP_VARIABLE_NAME             L"Setup"
#endif

#define SETUP_GUID \
 { 0xEC87D643, 0xEBA4, 0x4BB5, { 0xA1, 0xE5, 0x3F, 0x3E, 0x36, 0xB2, 0x0D, 0xA9 } }

#define SA_SETUP_GUID \
 { 0x72c5e28c, 0x7783, 0x43a1, { 0x87, 0x67, 0xfa, 0xd7, 0x3f, 0xcc, 0xaf, 0xa4 } }

#define ME_SETUP_GUID \
 { 0x5432122d, 0xd034, 0x49d2, { 0xa6, 0xde, 0x65, 0xa8, 0x29, 0xeb, 0x4c, 0x74 } }

#define CPU_SETUP_GUID \
 { 0x75e3088b, 0x88bb, 0x490f, { 0xaa, 0x29, 0xfa, 0xa8, 0x32, 0x44, 0xe8, 0xe3 } }

#define PCH_SETUP_GUID \
 { 0x4570b7f1, 0xade8, 0x4943, { 0x8d, 0xc3, 0x40, 0x64, 0x72, 0x84, 0x23, 0x84 } }

#define SI_SETUP_GUID \
 { 0xAAF8E719, 0x48F8, 0x4099, { 0xA6, 0xF7, 0x64, 0x5F, 0xBD, 0x69, 0x4C, 0x3D } }

#define DEBUG_CONFIG_GUID \
 { 0xDE0A5E74, 0x4E3E, 0x3D96, { 0xA4, 0x40, 0x2C, 0x96, 0xEC, 0xBD, 0x3C, 0x97 } }

#define BOARD_INFO_SETUP_GUID \
 { 0x1E785E1A, 0x8EC4, 0x49E4, { 0x82, 0x75, 0xFB, 0xBD, 0xED, 0xED, 0x18, 0xE7 } }

#define LPE_SETUP_GUID \
 { 0xb5fbe0c8, 0xa72a, 0x408d, { 0x85, 0x9c, 0xf,  0xd7, 0x53, 0x7a, 0xaa, 0x5f } }

#define OC_SETUP_GUID \
 { 0x3524af07, 0x4548, 0x7374, { 0xc9, 0xcb, 0xa3, 0x92, 0xf2, 0x25, 0x12, 0x5d } }
#endif
