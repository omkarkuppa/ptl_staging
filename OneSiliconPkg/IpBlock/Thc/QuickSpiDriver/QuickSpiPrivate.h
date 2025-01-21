/**@file
  Private Header file for Touch Host Controller QuickSpi Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

#ifndef _QUICK_SPI_PRIVATE_H_
#define _QUICK_SPI_PRIVATE_H_

#include <Uefi.h>
#include <PiDxe.h>

//
// Libraries
//
#include <Library/IoLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/TimerLib.h>
//
// UEFI Driver Model Protocols
//
#include <Protocol/DriverBinding.h>
#include <Protocol/DriverSupportedEfiVersion.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>
#include <Protocol/PciIo.h>
//
// Produced Protocols
//
#include <Protocol/AbsolutePointer.h>
#include <Protocol/Thc.h>
//
// Guid
//
#include <Guid/EventGroup.h>
#include <Guid/EventLegacyBios.h>

//
// Registers
//
#include <Register/ThcRegs.h>
//
// Other
//
#include <IndustryStandard/Pci30.h>


//#define THC_STANDALONE_DEBUG  0 // Required for stand alone driver Debug
#define THC_LOCAL_DEBUG(Message, ...)

#define THC_POLLING_PERIOD     1     // milliseconds
#define THC_DELAY_PERIOD_1MS   1000
#define THC_RESET_RETRY_COUNT  3 // AR Check with the spec

#define TOUCH_DRIVER_VERSION  0x00000001

//
// Protocol instances
//
extern EFI_DRIVER_BINDING_PROTOCOL  gQuickSpiDriverBinding;
extern EFI_COMPONENT_NAME_PROTOCOL  gQuickSpiDriverComponentName;
extern EFI_COMPONENT_NAME2_PROTOCOL gQuickSpiDriverComponentName2;
extern EFI_GUID                     gEdkiiTouchPanelGuid;

#define QUICK_SPI_SIGNATURE  0x51535049 //"QSPI" Touch Host Controller Quick SPI
#define QUICK_SPI_CONTEXT_FROM_THC_PROTOCOL(a)      CR (a, QUICK_SPI_DEV, ThcProtocol, QUICK_SPI_SIGNATURE)
#define QUICK_SPI_CONTEXT_FROM_ABSPTR_PROTOCOL(a)   CR (a, QUICK_SPI_DEV, AbsPtrProtocol, QUICK_SPI_SIGNATURE)
#define THC_HID_ELAN_VID 0x04F3

#define PRD_MAX_ENTRIES            256
#define PRD_READ_TABLES_SUPPORTED  16
#define PRD_MAX_TABLES_SUPPORTED   PRD_READ_TABLES_SUPPORTED
#define PRD_WRITE_TABLES_SUPPORTED 1
#define POINTER_MASK               0x7F
#define POINTER_WRAPAROUND         0x80
#define WRAPAROUND_VALUE_16        16
#define WRAPAROUND_VALUE_0X90      0x90
#define ADDRESS_SHIFT              10 // right shift destination Address
#define THC_MAX_HID_BUFFER_SIZE    4096 // 4kB -> 1 Page
#define THC_ELAN_HID_MAX_LENGTH    32
#define THC_ELAN_HID_SKIP_LENGTH   16

#define QUICK_SPI_DEFAULT_INPUT_REPORT_HEADER_ADDRESS  0x0000
#define QUICK_SPI_DEFAULT_INPUT_REPORT_BODY_ADDRESS    0x1000
#define QUICK_SPI_DEFAULT_OUTPUT_REPORT_ADDRESS        0x4
#define QUICK_SPI_DEFAULT_READ_OPCODE                  0x0B
#define QUICK_SPI_DEFAULT_DUAL_READ_OPCODE             0xBB
#define QUICK_SPI_DEFAULT_QUAD_READ_OPCODE             0xEB
#define QUICK_SPI_DEFAULT_WRITE_OPCODE                 0x02
#define QUICK_SPI_DEFAULT_DUAL_WRITE_OPCODE            0xB2
#define QUICK_SPI_DEFAULT_QUAD_WRITE_OPCODE            0xE2
#define QUICK_SPI_DEFAULT_QPI_WRITE_OPCODE             0xF2
#define QUICK_SPI_DEFAULT_FREQUENCY                    17000000

#define QUICK_SPI_BIT_OFFSET_INTERRUPT_TYPE      4
#define QUICK_SPI_BIT_LENGTH_INTERRUPT_TYPE      4   // input_report_type is 4 bits for HIDSPI
#define QUICK_SPI_BIT_OFFSET_LAST_FRAGMENT_FLAG  22  // Bit 15 as per HIDSPI spec 0.99
#define QUICK_SPI_BIT_OFFSET_MICROFRAME_SIZE     8
#define QUICK_SPI_BIT_LENGTH_MICROFRAME_SIZE     14  // input_report_length is 14 bits for HIDSPI
#define QUICK_SPI_BITMASK_INTERRUPT_TYPE_DATA    0x0
#define QUICK_SPI_BITMASK_INVALID_TYPE_DATA      0x1
#define QUICK_SPI_UNIT_MICROFRAME_SIZE           2   // MFS unit in power of 2
#define QUICK_SPI_DEFAULT_MAX_PACKET_SIZE_MIN    4   // in multiples of 16. i.e., 64 byte MPS
#define QUICK_SPI_DEFAULT_MAX_PACKET_SIZE        128 // in multiples of 16. i.e., 2048 byte MPS

//
// Quick SPI HID
//
#define QUICK_SPI_RELATIVE_COORDINATES_MAX_X 1024
#define QUICK_SPI_RELATIVE_COORDINATES_MAX_Y 1024

typedef enum {
  QuickSpiInputReportInvalidType0           = 0,
  QuickSpiInputReportData                   = 1,  // indicates data is ready to be read.
  QuickSpiInputReportInvalidType2           = 2,
  QuickSpiInputReportResetResponse          = 3,  // indicates a reset occurred.
  QuickSpiInputReportCommandResponse        = 4,  // indicates the command has completed
  QuickSpiInputReportGetFeatureResponse     = 5,  // indicates completion of get feature report requested by host
  QuickSpiInputReportInvalidType6           = 6,
  QuickSpiInputReportDeviceDescriptor       = 7,  // indicates input report contains device descriptor
  QuickSpiInputReportReportDescriptor       = 8,  // indicates input report contains report descriptor
  QuickSpiInputReportSetFeatureResponse     = 9,  // signals set feature response has completed
  QuickSpiInputReportOutputReportResponse   = 10, // signals set output report has completed
  QuickSpiInputReportGetInputReportResponse = 11, // indicates completion of get input report requested by host
  QuickSpiInputReportInvalidType            = 0xF // in this case TIC reset required
} QUICK_SPI_INPUT_REPORT_TYPE;

typedef enum {
  QuickSpiOutputReportInvalidType0     = 0,
  QuickSpiOutputReportDeviceDescriptor = 1,  // request for device descriptor
  QuickSpiOutputReportReportDescriptor = 2,  // request for report descriptor
  QuickSpiOutputReportSetFeature       = 3,  // HID SET_FEATURE request
  QuickSpiOutputReportGetFeature       = 4,  // HID GET_FEATURE request
  QuickSpiOutputReportOutputReport     = 5,  // HID OUTPUT_REPORT request
  QuickSpiOutputReportGetInputReport   = 6,  // Request for input report (HID Get Report)
  QuickSpiOutputReportCommandContent   = 7,  // Command content
} QUICK_SPI_OUTPUT_REPORT_TYPE;

typedef enum {
  QuickSpiCommandSetPowerReserved    = 0,
  QuickSpiCommandSetPowerOn          = 1,
  QuickSpiCommandSetPowerSleep       = 2,
  QuickSpiCommandSetPowerOff         = 3
} QUICK_SPI_COMMAND_SET_POWER;

#pragma pack(push, 1) //ask compiler not to align struct

typedef union {
  struct {
    UINT32 Reserved          : 13; // [0:12] Reserved
    UINT32 SpiWriteMode      : 1;  // [13] 0 - Single SPI; 1 - Multi Spi mode
    UINT32 MultiSpiMode      : 2;  // [14:15] Multi-SPI Mode: 0b00 - Single SPI Mode, 0b01 - Dual SPI Mode 0b10 - Quad SPI Mode, 0b11 - Reserved
    UINT32 Reserved2         : 16; // [16:31] Reserved
  } Fields;
  UINT32 Data32;
} QUICK_SPI_FLAGS;

typedef union {
  struct {
    UINT32 ProtocolVersion   : 4;  // Must be 0x2 for HIDSPI --> starting v101 this will be 0x3
    UINT32 Reserved0         : 4;  // Reserved starting v101 --> used to be input_report_type
    UINT32 InputReportLength : 14; // Length in number if bytes divided by 4
    UINT32 LastFragmentFlag  : 1;  // Value of 1 indicates last fragment. 0 indicates additional fragments
    UINT32 Reserved          : 1;  // Should be set to zero
    UINT32 SyncConst         : 8;  // Must be 0x5A. Used to validate input report header (ex: data integrity on the SPI bus)
  } Fields;
  UINT32 Data32;
} THC_M_PRT_INPUT_REPORT_HEADER_VAL;

typedef union {
  struct {
    UINT32  ReportType    : 8; // see QUICK_SPI_INPUT_REPORT_TYPE above for possible types
    UINT32  ContentLength : 16;
    UINT32  ContentId     : 8;
  } Fields;
  UINT32 Data32;
} QUICK_SPI_INPUT_REPORT_BODY_HEADER;

typedef union {
  struct {
    UINT32 ReportType    : 8;  // See QUICK_SPI_OUTPUT_REPORT_TYPE for OUTPUT_REPORT_TYPE
    UINT32 ContentLength : 16; // Size of content field (n bytes) + 3 (for size of content length and content id fields)
    UINT32 ContentId     : 8;  // 0x00 - descriptors, Report ID - Set/Feature feature or Input/Output Reports. Command opcode - for commands
  } Fields;
  UINT32 Data32;
} QUICK_SPI_OUTPUT_REPORT_HEADER;

typedef struct {
  QUICK_SPI_INPUT_REPORT_BODY_HEADER  Header;
  UINT8 ReadData[1];
} QUICK_SPI_READ_DATA_BUFF;

typedef union {
  struct {
    UINT32 MaximumReadSize  : 14; // RO
    UINT32 MaximumWriteSize : 10; // RO
    UINT32 Reserved         : 8;  // RO
  } Fields;
  UINT32 Data;
} TOUCH_DATA_SIZE;

typedef union {
  struct {
    UINT32 TssGo           : 1;  // THC SW Sequence Cycle Go  RW/1S/V
    UINT32 QuickSpiSsCdIe  : 1;  // THC SW Sequencing Cycle Done Interrupt Enable  RW
    UINT32 QuickSpiSsBtaEn : 1;  // THC SW Sequencing Bus Turnaround Enable  RW, In SPI mode, this bit is not used.
    UINT32 Rsvd0           : 1;  // RSVD  RO
    UINT32 TsiSsPType      : 1;  // TSI SW Sequencing Bus Packet Type  RW, In SPI mode, this bit is not used.
    UINT32 QuickSpiSsTrCt  : 1;  // THC SW Sequencing Command Type  RW
    UINT32 Rsvd1           : 2;  // RSVD  RO
    UINT32 QuickSpiSsCmd   : 8;  // THC SW Sequencing Bus Command  RW
    UINT32 QuickSpiSsBc    : 16; // THC SW Sequencing Bus Word Byte Count  RW
  } Fields;
  UINT32 Data;
} THC_M_PRT_SW_SEQ_CNTRL;

typedef union {
  struct {
    UINT32 TssDone         : 1;  // 1 when the SW Touch Cycle completes after software previously set the TSSGO bit.
                                 // This bit remains asserted until cleared by software writing a 1 or host partition reset.
    UINT32 ThcSsErr        : 1;  // This bit remains asserted until cleared by software writing a 1 or until a partition reset occurs.
                                 // Software must clear this bit before setting the THC Cycle GO bit in this register.
    UINT32 Rsvd0           : 1;  // RSVD  RO
    /**
      Hardware sets this bit when software sets the THC Cycle Go (TSSGO) bit.
      This bit remains set until the cycle completes on the bus interface.
      Hardware automatically sets and clears this bit so that software can determine when read data is valid.
      And/Or when it is safe to begin programming the next command. Software must only program the next command when this bit is 0.
    **/
    UINT32 ThcSsCip        : 1;
    UINT32 Rsvd1           : 28;  // RSVD  RO
  } Fields;
  UINT32 Data32;
} THC_M_PRT_SW_SEQ_STS;

typedef union {
  struct {
    UINT32 Pcd        : 7;  // RW CB Depth for the 2nd RXDMA
    UINT32 Rsvd0      : 1;  // RO RSVD
    UINT32 Ptec       : 8;  // RW Table Entry Count for the 2nd RXDMA
    UINT32 PreFetchWm : 4;  // RW PRD prefetch Watermark for the 2nd RXDMA
    UINT32 Rsvd1      : 12; // RO Reserved
  } Fields;
  UINT32 Data;
} THC_M_PRT_RPRD_CNTRL_2;

typedef union {
  struct {
    UINT32  SpiQpiIcrrdOpCode : 8; // RW QPI Read OpCode for interrupt cause register or PIO read
    UINT32  SpiQioIcrrdOpCode : 8; // RW Quad IO Read OpCode for interrupt cause register or PIO read
    UINT32  SpiDioIcrrdOpCode : 8; // RW Dual IO Read OpCode for interrupt cause register or PIO read
    UINT32  SpiSioIcrrdOpCode : 8; // RW Single IO Read OpCode for interrupt cause register or PIO read
  } Fields;
  UINT32  Data32;
} THC_M_PRT_SPI_ICRRD_OPCODE;

typedef union {
  struct {
    UINT32  SpiQpiDmaRdOpCode : 8; // RW QPI Read OpCode for RxDMA touch data read
    UINT32  SpiQioDmaRdOpCode : 8; // RW Quad IO Read OpCode for RxDMA touch data read
    UINT32  SpiDioDmaRdOpCode : 8; // RW Dual IO Read OpCode for RxDMA touch data read
    UINT32  SpiSioDmaRdOpCode : 8; // RW Single IO Read OpCode for RxDMA touch data read
  } Fields;
  UINT32  Data32;
} THC_M_PRT_SPI_DMARD_OPCODE;

typedef union {
  struct {
    UINT32  SpiQpiWrOpCode : 8; // RW QPI Write OpCode for TxDMA or PIO
    UINT32  SpiQioWrOpCode : 8; // RW Quad IO Write OpCode for TxDMA or PIO
    UINT32  SpiDioWrOpCode : 8; // RW Dual IO Write OpCode for TxDMA or PIO
    UINT32  SpiSioWrOpCode : 8; // RW Single IO Write OpCode for TxDMA or PIO
  } Fields;
  UINT32 Data32;
} THC_M_PRT_SPI_WR_OPCODE;

typedef union {
  struct {
    UINT32 IntTypOffset  : 5;  // Bit offset for interrupt/input report type. Must be 0x4 for HID-SPI
    UINT32 IntTypLen     : 5;  // Length of interrupt type. Must be 4 for HID-SPI
    UINT32 EofOffset     : 5;  // Bit offset for EOF/Last Fragment. Must be 0xE for HIDSPI
    UINT32 SendIcrUsEn   : 1;  // Enable bit to send Interrupt cause register to memory
    UINT32 IntTypDataVal : 16; // Bit mask to support multiple input report types as data interrupts with RxDMA
                               // Set bit positions as per INPUT_REPORT_TYPE definitions above
  } Fields;
  UINT32 Data32;
} THC_M_PRT_DEVINT_CFG_1;

typedef union {
  struct {
    UINT32 UfSizeOffset    : 5; // Bit offset for Input Report Length/Micro Frame Size. Must be 0x9 for HID-SPI
    UINT32 UfSizeLen       : 5; // Length of Input Report Length field. Must be 0xE for HID-SPI
    UINT32 Rsvd1           : 2; // Should be set to zero
    UINT32 UfSizeUnit      : 4; // Input Report Length unit in power of 2. Must be 0x2 for HIDSPI
    UINT32 FTypeIgnore     : 1; // If set to 1, THC ignores Frame Type. Set to 1 for HIDSPI
    UINT32 FTypeVal        : 1; // Always set to 1 for HIDSPI for fixed frame type of HID Report
    UINT32 Rsvd2           : 6; // Should be set to zero
    UINT32 RxDmaAddrIncDis : 1; // Enable or disable address auto increment for every read packet in single interrupt
    UINT32 TxDmaAddrIncDis : 1; // Enable or disable address auto increment for every TxDMA packet in single write dma
    UINT32 Rsvd3           : 1; // When set to 1, THC sends RxDMA data frame in stream mode without toggling CS line - rsvd for now
    UINT32 TxDmaPktStrmEn  : 1; // When set to 1, THC sends TxDMA data frame in stream mode without toggling CS line
    UINT32 DevIntPol       : 1; // 0 - Enable Active Low Interrupt. 1 - Enable Active High Interrupt
    UINT32 Rsvd4           : 3; // Should be set to zero
  } Fields;
  UINT32 Data32;
} THC_M_PRT_DEVINT_CFG_2;

typedef union {
  struct {
    UINT32 SpiTRDC      : 2; // RW SPI Touch IO Read Dummy Clocks
    UINT32 SpiTrMode    : 2; // RW SPI Touch IO Read mode
    UINT32 SpiTCRF      : 3; // RW SPI Touch Cycle Read Frequency
    UINT32 SpiRdMps     : 9; // RO SPI READ Max Packet Size
    UINT32 RSVD0        : 2; // RO Reserved
    UINT32 SpiTwMode    : 2; // RW SPI Touch IO Write mode
    UINT32 SpiTCWF      : 3; // RW SPI Touch Cycle Write Frequency
    UINT32 SpiLowFreqEn : 1; // RW Enable SPI Clock Divide by 8 to support low freq device.
    UINT32 SpiWrMps     : 8; // RO SPI WRITE Max Packet Size
  } Fields;
  UINT32 Data32;
} THC_M_PRT_SPI_CFG;

typedef union {
  struct {
    UINT32 Start                : 1; // RW/V Start for the 2nd RXDMA
    UINT32 IeError              : 1; // RW Interrupt Enable on Error for the 2nd RXDMA
    UINT32 IeIoC                : 1; // RW Interrupt Enable on Completion for the 2nd RXDMA
    UINT32 IeStall              : 1; // RW Interrupt Enable on Stall for the 2nd RXDMA
    UINT32 Rsvd0                : 1; // RO RSVD
    UINT32 IeEof                : 1; // RW Interrupt Enable at EOF for the 2nd RXDMA
    UINT32 Rsvd1                : 1; // RO RSVD
    UINT32 IeDmaCpl             : 1; // RW Interrupt Enable on DMA Completion for the 2nd RXDMA
    UINT32 QuickSpiPrdCbReadPointer  : 8; // RO/V THC PRD CB Read Pointer for the 2nd RXDMA
    UINT32 QuickSpiPrdCbWritePointer : 8; // RW/V THC PRD CB Write Pointer for the 2nd RXDMA
    UINT32 Rsvd2                : 4; // RO Reserved
    UINT32 IntSwDmaEn           : 1; // RW/L Interrupt SW Enable on DMA Device Interrupt for the 2nd RXDMA
    UINT32 StopOverFlow         : 1; // RW Stop on Overflow for the 2nd RXDMA
    UINT32 UpdateHwStatus       : 1; // RO Update HwStatus for the 2nd RXDMA
    UINT32 QuickSpiPrdCbPointerReset : 1; // RW/1S/V THC PRD CB Pointer Reset for the 2nd RXDMA
  } Fields;
  UINT32 Data;
} THC_M_PRT_READ_DMA_CNTRL_2;

typedef union {
   struct {
    UINT32 DmaCplSts             : 1; // RW/1C/V This bit is set upon successful completion of the DMA operation.
                                      // If the IE bit is also 1, then an interrupt is generated. SW clears the bit by writing 1 to the bit.
    UINT32 ErrorSts              : 1; // RW/1C/V Error encountered during DMA operation. An interrupt is generated if the interrupt is enabled.
                                      // SW clears the bit by writing 1 to the bit.
    UINT32 IocSts                : 1; // RW/1C/V An PRD entry with IOC bit set has been completed during DMA operation.
                                      // f the IE_IOC bit is also 1, then an interrupt is generated. SW clears the bit by writing 1 to the bit.
    UINT32 StallSts              : 1; // RW/1C/V Interrupt status when the CB read and write pointers are the same.
                                      // This condition could be used to detect a stall of the GPU and subsequent buffer overrun internal to the Touch IC.
                                      // If the IE_STALL bit is also 1, then an interrupt is generated. SW clears the bit by writing 1 to the bit.
    UINT32 Rsvd0                 : 1; // RO Reserved
    UINT32 EofIntSts             : 1; // RW/1C/V Interrupt status when an EOF is encountered. If the IE_EOF bit is also 1, then an interrupt is generated. SW clears the bit by writing 1 to the bit.
    UINT32 Rsvd1                 : 2; // RO Reserved
    UINT32 Active                : 1; // RW/1C/V DMA is active and not completed. Set by HW when the Start bit is set by SW and auto-cleared by HW in the following conditions:
                                      // - The entire DMA operation has been completed or
                                      // - The DMA operation has been halted by software clearing the Start bit (and no read completion pending)
    UINT32 Rsvd2                 : 23;// RO Reserved
  } Fields;
  UINT32 Data32;
} THC_M_PRT_READ_DMA_INT_STS_2;

typedef union {
  struct {
    UINT32 QuickSpiWrDmaStart      : 1;  // RW/V Start WRDMA
    UINT32 QuickSpiWrDmaIeIocError : 1;  // RW Interrupt Enable on Error for WRDMA
    UINT32 QuickSpiWrDmaIeIoc      : 1;  // RW Interrupt Enable on IOC for WRDMA
    UINT32 QuickSpiWrDmaIeDmaCpl   : 1;  // RW Interrupt Enable on WRDMA Completion
    UINT32 Rsvd0                   : 19; // RO RSVD
    UINT32 QuickSpiWrDmaUhs        : 1;  // RO Update HW Status for WRDMA
    UINT32 QuickSpiWrDmaPtec       : 8;  // RW PRD Table Entry Count for WRDMA
  } Fields;
  UINT32 Data;
} THC_M_PRT_WRITE_DMA_CNTRL;

typedef union {
  struct {
    /**
      RW/V
      Set upon successful completion of the Write DMA operation or by the rising edge of the Error bit.
      If the THC_WRDMA_IE_IOC_DMACPL bit is also 1, then an interrupt is generated.
      In polling mode, when THC_WRDMA_IE_IOC_DMACPL=0, SW clears the bit by writing 1 to the bit.
    **/
    UINT32 ThcRdDmaCmplStatus      : 1;
    UINT32 ThcRdDmaErrorStatus     : 1;  // RW Error encountered during write DMA operation.
    UINT32 ThcRdDmaIocStatus       : 1;  // RO A PRD entry with IOC bit set has been completed during WRDMA operation.
    UINT32 ThcRdDmaStallStatus     : 1;  // RW/1C/V Interrupt Status of PRD table stalls for the 1st RXDMA
    UINT32 ThcRdDmaNonDmaIntStatus : 1;  // RW/1C/V Interrupt Status of non DMA device interrupt
    UINT32 ThcRdDmaEofIntStatus    : 1;  // RW/1C/V Interrupt Status of EOF Interrupt for the 1st RXDMA
    UINT32 Rsvd0                   : 2;  // RO RSVD
    UINT32 ThcRdActiveStatus       : 1;  // RO/V Active status for the 1st RXDMA
    UINT32 Rsvd1                   : 23; // RO RSVD
  } Fields;
  UINT32 Data32;
} THC_M_PRT_READ_INT_STS;


typedef union {
  struct {
    /**
      RW/V
      Set upon successful completion of the Write DMA operation or by the rising edge of the Error bit.
      If the THC_WRDMA_IE_IOC_DMACPL bit is also 1, then an interrupt is generated.
      In polling mode, when THC_WRDMA_IE_IOC_DMACPL=0, SW clears the bit by writing 1 to the bit.
    **/
    UINT32 ThcWrDmaCmplStatus  : 1;
    UINT32 ThcWrDmaErrorStatus : 1;  // RW Error encountered during write DMA operation.
    UINT32 ThcWrDmaIocStatus   : 1;  // RO A PRD entry with IOC bit set has been completed during WRDMA operation.
    UINT32 ThcWrDmaActive      : 1;  // RW Write DMA is active.
    UINT32 Rsvd0               : 28; // RO RSVD
  } Fields;
  UINT32 Data;
} THC_M_PRT_DMA_INT_STS;


//128bit - 16 bytes
typedef struct {
  /**
  This field indicates the destination of the DMA in 64 bit host memory, Hardware supports a
  minimum alignment of 1KB. Software may choose a different fragmentation, so long as it is
  greater than 1KB (ex: 4KB).
  **/
  UINT64  DestinationAddress    : 54; //53:0
  UINT64  Reserved1             : 9;  //54:62
  UINT64  InterruptOnCompletion : 1;  //63
  /**
  The length of the DMA for this PRD Entry must be byte aligned with a maximum transfer length of 16MB-1.
  HW updates this field when the data corresponding to this entry is received. The length field is 1-
  based. Value 1 means 1 byte. Zero length PRD entry is not valid. A PRD entry's length must be
  multiple of 4KBytes except for the last entry in a PRD table. The last entry can be any size from 1
  Byte to 1M-1 Bytes.
  **/
  UINT64  Length                : 24; //87:64
  /**
  End of PRD (EOP)
  This bit indicates that this PRD entry is the final entry in the PRD table.
  It is updated by HW when it receives the last packet of a frame.
  **/
  UINT64  EndOfPrd              : 1;  //88
  /**
  HW Status: These bits are set by HW if the UHS bit is set after completing a PRD. It is cleared by
  SW after being consumed.
  Values:
  0: SW Reset Value
  1: Completed Successfully. This bit is set in the first PRD Entry with the EOP bit set upon successful
  PRD table completion.
  2: Completed with error. This bit is set in the PRD Entry where an error occurs such that the read
  DMA stops processing PRD entries.
  3: Rx Buffer overflow before this frame is received. There could be multiple frames that got dropped
  before this status is set.
  **/
  UINT64  HwStatus              : 2;  //90:89
  UINT64  Reserved2             : 37; //127:91
} PRD_ENTRY;

typedef struct {
  PRD_ENTRY Entries[PRD_MAX_ENTRIES];
} PRD_TABLE;

typedef struct {
  EFI_PHYSICAL_ADDRESS    Buffer;
  EFI_PHYSICAL_ADDRESS    DevAddr;
  VOID                    *Mapping;
  PRD_ENTRY               Entries[PRD_MAX_ENTRIES];
  UINT32                  NumOfConfiguredEntries;
} DRIVER_PRD_TABLE;

typedef struct {
  EFI_PHYSICAL_ADDRESS    CommonPrdBuffer;
  EFI_PHYSICAL_ADDRESS    CommonPrdBufferDevAddr;
  VOID                    *CommonPrdBufferMapping;
  DRIVER_PRD_TABLE        DriverPrdTable[PRD_MAX_TABLES_SUPPORTED];
  UINT32                  NumOfPrdTables;
  UINT32                  BufferSize;
} THC_DMA;

typedef enum {
  IoReadyTimeout      = 100,
  StartQuiesceTimeout = 300,
  EndQuiesceTimeout   = 700,
  OutOfResetTimeout   = 300, // AR change up to 1 sec
  WaitForResetTimeout = 250, // multiplied by the reset attempt count
  CycleTimeout        = 1000,
  WritePollingTimeout = 100,
  PollingLoopTimeout  = 100000, // MicroSeconds
  HidResponseTimeout  = 1000,
  GetDeviceDescriptorTimeout   = 1000 // AR TBD
} THC_TIMERS;

typedef enum {
  QuickSpiPioRead  = 0x4,
  QuickSpiPioWrite = 0x6
} PIO_OP;

typedef enum {
  WacomHidProtocol      = 1,
  ElanHidProtocol       = 2,
} THC_HID_PROTOCOL;

  //
  // Lower Right Corner X,Y Coordinates will be (32767, 32767) and Top
  // left corner will be (0,0)
  //
  typedef struct {
    UINT8 ReportId;    // Always 0x40.
    UINT8 TouchSts;    // Bit 0: 0 - No Touch, 1 - Touch. Bits 1-7 are ignored.
    UINT8 LowerXByte;  // Lower Byte value of X Coordinate. The Max of X coordinate will be 32767.
    UINT8 HigherXByte; // Higher Byte value of X Coordinate.
    UINT8 LowerYByte;  // Lower Byte value of Y Coordinate. The Max of Y coordinate will be 32767.
    UINT8 HigherYByte; // Higher Byte value of Y Coordinate.
  } SINGLE_FINGER_REPORT;


typedef struct {
  UINT32 ID;
  UINT32 Value;
} HID_REPORT_DESCRIPTOR_TOKEN;

typedef struct {
  UINTN ReportId;
  UINTN MaxX;
  UINTN StartX;
  UINTN StopX;
  UINTN MaxY;
  UINTN StartY;
  UINTN StopY;
  UINTN Button;
} HID_REPORT_DESCRIPTOR;

#define MAX_HID_COLLECTION    256

#define USE_PAGE       0x01
#define DIGITIZER      0x0D
#define DESKTOP        0x01
#define USAGE          0x02
#define MOUSE          0x02
#define TOUCHPANEL     0x04
#define TOUCHPPAD      0x05
#define TIP_SWITCH     0x42
#define BUTTON         0x09
#define X_AXIS         0x30
#define Y_AXIS         0x31
#define REPORT_ID      0x21
#define LOG_MIN        0x05
#define LOG_MAX        0x09
#define REP_SIZE       0x1d
#define REP_COUNT      0x25
#define INPUT          0x20
#define OUTPUT         0x24
#define COLLECTION     0x28
#define APPLICATION    0x01
#define LOGICAL        0x02
#define END_COLLECTION 0x30

typedef struct {
  UINT32 ID;
  UINT32 Value;
} HID_TOKEN;

typedef struct {
  UINT32 B;
  UINT32 X;
  UINT32 Y;
} HID_TOUCH_OUTPUT;

typedef struct {
  UINTN MinX;
  UINTN MaxX;
  UINTN MinY;
  UINTN MaxY;
} HID_XY_BOUNDARY;

typedef struct {
  UINT8   HIDDescLengthLsb;
  UINT8   HIDDescLengthMsb;
  UINT8   BcdVersionLsb;
  UINT8   BcdVersionMsb;
  UINT8   ReportDescRegLengthLsb;
  UINT8   ReportDescRegLengthMsb;
  UINT8   ReportDescRegisterLsb;
  UINT8   ReportDescRegisterMsb;
  UINT8   InputRegisterLsb;
  UINT8   InputRegisterMsb;
  UINT8   MaxInputLengthLsb;
  UINT8   MaxInputLengthMsb;
  UINT8   OutputRegisterLsb;
  UINT8   OutputRegisterMsb;
  UINT8   MaxOutputLengthLsb;
  UINT8   MaxOutputLengthMsb;
  UINT8   CommandRegisterLsb;
  UINT8   CommandRegisterMsb;
  UINT8   DataRegisterLsb;
  UINT8   DataRegisterMsb;
  UINT16  VendorId;
  UINT16  ProductId;
  UINT16  VersionId;
  UINT16  Reserved;
  UINT16  Reserved2;
} HID_DESC;

// dictionary for parsing incoming data from touchpanel
typedef struct {
  UINT32  LogMaxX;    // max and min values for X and Y axes
  UINT32  LogMinX;
  UINT32  LogMaxY;
  UINT32  LogMinY;
  UINT32  BitStartB;  // bit positions of Buttons state /X coordinates / Y coordinates in input stream
  UINT32  BitStopB;   // bit positions are calculated in pure input data (after cutting Length and reportID)
  UINT32  BitStartX;
  UINT32  BitStopX;
  UINT32  BitStartY;
  UINT32  BitStopY;
  UINT32  BitsTotal;  // total number of bits with declared purpose
  BOOLEAN ValidCollection;
} HID_INPUT_REPORT_COLLECTION;

// dictionary for parsing incoming data from touchpanel
typedef struct {
  UINT32  Id;  // ReportID. 0 means panel doesn't send ReportIDs, and it also means there is only 1 format
  UINT32  CollectionCount;
  UINT32  UsageTouchPanel;
  UINT32  UsageTouchPad;
  UINT32  UsageMouse;
  HID_INPUT_REPORT_COLLECTION  Collection[MAX_HID_COLLECTION];
} HID_INPUT_REPORT_FORMAT;

// set of dictionaries for parsing incoming data from touchpanel
typedef struct {
  UINT32  Quantity;   // number of different report formats
  UINT32  TouchPanel;
  UINT32  TouchPad;
  HID_INPUT_REPORT_FORMAT   *Report; // pointer to array of dictionaries for report formats
} HID_INPUT_REPORT_TABLE;

typedef struct {
  UINT32  GlobalUsage;
  UINT32  Usages;
  UINT32  UsageB;
  UINT32  UsageX;
  UINT32  UsageY;
  UINT32  LogMax;
  UINT32  LogMin;
  UINT32  ReportSize;
  UINT32  ReportCount;
  UINT32  GlobalUsageOnStack;
  UINT32  ReportSizeOnStack;
  UINT32  ReportCountOnStack;
  UINT32  UsageTouchPanel;
  UINT32  UsageTouchPad;
  UINT32  UsageMouse;
  HID_INPUT_REPORT_FORMAT  TempReport;
} HID_PARSER_STACK;

typedef union {
    struct {
      UINT16  NoOutputReportAck : 1; // When set, device does not acknowledge output reports (output report type=0x05).
      UINT16  Reserved         : 15; // RO Reserved
    } Fields;
    UINT16 Data16;
} QUICK_SPI_DEVICE_DESCRIPTOR_FLAGS;

typedef struct {
  UINT16  wDeviceDescLength;
  UINT16  bcdVersion;
  UINT16  wReportDescLength;
  UINT16  wMaxInputLength;
  UINT16  wMaxOutputLength;
  UINT16  wMaxFragmentLength;
  UINT16  wVendorID;
  UINT16  wProductID;
  UINT16  wVersionID;
  QUICK_SPI_DEVICE_DESCRIPTOR_FLAGS wFlags;
  UINT32  Reserved;
} QUICK_SPI_DEVICE_DESCRIPTOR;

#pragma pack(pop)

typedef struct {
  UINTN                          Signature;
  EFI_HANDLE                     ControllerHandle;
  EFI_UNICODE_STRING_TABLE       *ControllerNameTable;
  EFI_PCI_IO_PROTOCOL            *PciIo;
  UINT64                         PciAttributes;
  UINT64                         PciBar0;
  UINT8                          PciPwrst;
  THC_DMA                        DmaRead2; // Only RxDma2 used in HID AbsPtrMode
  THC_DMA                        DmaWrite;
  THC_PROTOCOL                   ThcProtocol;
  EFI_ABSOLUTE_POINTER_PROTOCOL  AbsPtrProtocol;
  EFI_ABSOLUTE_POINTER_MODE      AbsPtrMode;
  EFI_ABSOLUTE_POINTER_STATE     AbsPtrTouchData;
  BOOLEAN                        AbsPtrDataAvailable;
  UINT8                          *HidBuffer;
  BOOLEAN                        HidActive;
  BOOLEAN                        HidDataAvailable;
  EFI_EVENT                      PollingEvent;
  EFI_EVENT                      ExitEvent;
  SINGLE_FINGER_REPORT           Report;
  BOOLEAN                        ReadDone;
  BOOLEAN                        Initialized;
  HID_INPUT_REPORT_TABLE         InputReportTable;
  QUICK_SPI_DEVICE_DESCRIPTOR    DeviceDescriptor;
  UINT8                          InstanceId;
  UINT8                          LastWritePointer;
  UINT8                          HidSolutionFlag;
} QUICK_SPI_DEV;

#endif
