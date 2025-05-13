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

#include <Base.h>
#include <Uefi.h>
#include <Uefi/UefiBaseType.h>
#include <IndustryStandard/Pci.h>
#include <Library/BaseLib.h>
#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PchPciBdfLib.h>
#include <Library/TimerLib.h>
#include <LpssSpiPrivateLib.h>
#include <Register/LpssSpiRegs.h>

/**
  0x0 Enhanced SSP Control 0 register
**/
typedef union {
  struct {
    /**
    Data Size Select With EDSS as MS
    **/
    UINT32 DataSizeSelect  : 4; //[RW][0:3]
    /**
    Frame Format
    00 - Motorola Serial Peripheral Interface (SPI) Motorola interface supported
    01 = reserved
    10 = reserved
    11 = reserved
    **/
    UINT32 FrameFormat     : 2; //[RW][4:5]
    /**
    External Clock Select
    1 = Reserved
    0 = use On-chip clock (output of M/N Divider) to produce the SSP's serial clock (SSPSCLK).
    Selects the use of the the output of the M/N Divider (MBAR0 + 0x800, CLOCK_PARAMS) to create the SSP's serial clock (SSPCLK)
    Note: Setting M=N=1 will provide a pass through of the M/N Divider of the serial clock.
    See SCR for Serial Clock Rate generation. Note: The input (SSPEXTCLK) is treated as SSPSCLKEN, a clock enable used to gate the SSPSCLK output.
    When the external signal SSPSCLKEN changes, there will be a 1 -2 clock lag before the SSPSCLK is started or stopped
    due to the internal synchronization of this signal.
    **/
    UINT32 ExtClockSelect  : 1; //[RW][6]
    /**
    Synchronous Serial Port Enable
    0 = SSP operation disabled
    1 = SSP operation enabled.
    The SSP enable bit, SSCR0.SSE, enables and disables all SSP operations.
    When SSCR0.SSE=0, the Enhanced SSP is disabled; when SSCR0.SSE=1, it is enabled.
    When the Enhanced SSP is disabled, all of its clocks can be stopped by programmers to minimize power consumption.
    On reset, the Enhanced SSP is disabled. When the SSCR0.SSE bit is cleared during active operation,
    the Enhanced SSP is disabled immediately, terminating the current frame being transmitted or received.
    Clearing SSCR0.SSE resets the Enhanced SSP FIFOs and the Enhanced SSP status bits;
    however, the Enhanced SSP Control registers are not reset.
    Note: After reset or after clearing the SSCR0.SSE, users must ensure that the SSCR1, SSITR and SSTO
    control registers are properly re-configured and that the SSSR register is reset before re- enabling the Enhanced SSP with the SSCR0.SSE.
    Also, the SSCR0.SSE bit must be cleared before reconfiguring the SSCR0, SSCR1, registers;
    other control bits in SSCR0 can be written at the same time as the SSCR0.SSE.
    When any SSP is disabled, its five pins can be used as GPIOs.
    **/
    UINT32 SynchronousSerialPortEnable  : 1; //[RW][7]
    /**
    Serial Clock Rate Value used to generate transmission rate of SSP.
    Note: The SPI Interface Controller (SSP) baud rate (or Serial bit-rate clock SPI_CLK_OUT) can be generated either by
    the M/N divider or internally to the SSP (SSCR0.SCR) by dividing the on-chip SSP_CLK (output of M/N) to generate baud rates up to : 25 Megabits per second
    **/
    UINT32 SerialClockRateValue        : 12; //[RW][8:19]
    /**
    Extended Data Size Select
    0 - A zero is prepended to the DSS value which sets the DSS range from 4 -16 bits
    1 - A one is pre-appended to the DSS value which sets the DSS range from 17-32 bits
    The 1-bit extended field is used in conjunction with the data size select SSCR0.DSS bits to select the size of the data transmitted and received by the Enhanced SSP.
    **/
    UINT32 ExtDataSizeSelect           : 1; //[RW][20]
    /**
    Network Clock Select
    0 - Clock selection is determined by ECS bit
    1 - Reserved
    The SSCR.NCS bit in conjunction with SSCR.ECS determines which clock is used.
    **/
    UINT32 NetworkClockSelect    : 1; //[RW][21]
    /**
    Receive FIFO Over Run Interrupt Mask
    0 - Receive FIFO Over Run(ROR) events will generate an SSP interrupt
    1 - ROR events will be masked
    When set, this bit will mask the Receive FIFO Over Run (ROR) event from generating an SSP interrupt.
    The SSSR status register will still indicate that an ROR event has occurred.
    This bit can be written to at any time (before or after SSP is enabled)
    **/
    UINT32 RxFifoOverrunIntMask  : 1; //[RW][22]
    /**
    Transmit FIFO Under Run Interrupt Mask
    0 = Transmit FIFO Under Run(TUR) events will generate an SSP interrupt
    1 = TUR events will be masked
    When set, this bit will mask the Transmit FIFO Under Run (TUR) event from generating an SSP interrupt.
    The SSSR status register will still indicate that an TUR event has occurred.
    This bit can be written to at any time (before or after SSP is enabled).
    **/
    UINT32 TxFifoOverrunIntMask  : 1; //[RW][23]
    UINT32 Reserved0             : 5; //[N/A][24:28]
    /**
    Chicken bit for LNW ECO 716771.
    Fix for cross clock communication where clk_apb is slower than clk_ssp
    **/
    UINT32 ChickenBit            : 1; //[RW][29]
    /**
    Set to 0 for Clock selection which is determined by the NCS and ECS bits in this register.
    1 = reserved
    **/
    UINT32 ACS                  : 1; //[RW][30]
    /**
    0 - Normal SSP Mode : Full Duplex Serial peripheral interface.
    1 - reserved
    **/
    UINT32 Mode         : 1; //[RW][31]
  } Fields;
  UINT32 Data32;
} LPSS_SPI_CONTROL0_REG;

/**
  0x4 Enhanced SSP Control 1 register
**/
typedef union {
  struct {
    /**
    Receive FIFO Interrupt Enable
    0 - Receive FIFO level interrupt is disabled
    1 - Receive FIFO level interrupt is enabled
    The SSCR1.RIE bit enables the Receive FIFO Service Request Interrupt.
    When SSCR1.RIE=0, the Interrupt is masked, and the state of the receive FIFO service request SSSR.RFS bit within the Enhanced SSP Status register is ignored.
    When SSCR1.RIE=1, the Interrupt is enabled, and whenever SSSR.RFS is set to one, an Interrupt request is made to the Interrupt controller.
    Note that programming SSCR1.RIE=0 does not affect the current state of SSSR.RFS or the ability of the Receive FIFO logic to set and clear SSSR.RFS
    it only blocks the generation of the Interrupt request. Also, the state of SSCR1.RIE does not affect the generation of the Receive FIFO DMA service request,
    which is asserted whenever the SSSR.RFS bit is set to 1.
    **/
    UINT32 RxFifoIntEnable   : 1; //[RW][0]
    /**
    Transmit FIFO Interrupt Enable
    0 - Transmit FIFO level interrupt is disabled
    1 - Transmit FIFO level interrupt is enabled
    The SSCR1.TIE bit enables the Transmit FIFO Service Request Interrupt.
    When SSCR1.TIE=0, the Interrupt is masked, and the state of the transmit FIFO service request (SSSR.TFS) bit within the Enhanced SSP Status register is ignored.
    When SSCR1.TIE=1, the Interrupt is enabled, and whenever SSSR.TFS is set to one, an Interrupt request is made to the Interrupt controller.
    Note that programming SSCR1.TIE=0 does not affect the current state of SSSR.TFS or the ability of the Transmit FIFO logic to set and clear SSSR.TFS it only blocks the generation of the Interrupt request.
    Also, the state of SSCR1.TIE does not affect the generation of the Transmit FIFO DMA service request,
    which is asserted whenever the SSSR.TFS is set to 1.
    **/
    UINT32 TxFifoIntEnable   : 1; //[RW][1]
    /**
    Loop-Back Mode (test mode bit)
    The SSCR1.LBM bit is a test mode bit that enables and disables the ability of the Enhanced SSP Transmit and Receive logic to communicate.
    0 = Normal serial port operation enabled
    1 = Output of transmit serial shifter connected to input of receive serial shifter, internally.
    When SSCR1.LBM=0, the Enhanced SSP operates normally.
    The Transmit and Receive data paths are independent and communicate via their respective pins. When SSCR1.LBM=1,
    the output of the Transmit serial shifter is connected directly to the input of the Receive serial shifter internally.
    During Loop-Back mode, the Transmit pin (SSPTXD) continues to function as normal.
    **/
    UINT32 LoopBackMode     : 1; //[RW][2]
    /**
    SSPSCLK polarity setting
    0 - The inactive or idle state of SSPSCLK is low
    1 - The inactive or idle state of SSPSCLK is high
    The SSCR1.SPO bit selects the polarity of the inactive state of the SSPSCLK pin when the Motorola SPI format is selected (SSCR0.FRF=00).
    For S SCR1.SPO=0, the SSPSCLK is held low in the Inactive or Idle state when the Enhanced SSP is not transmitting/receiving data. For SSCR1.SPO=1,
    the SSPSCLK is held high during the Inactive/Idle state.
    The programmed setting of the SSCR1.SPO alone does not determine which SSPSCLK edge transmits or receives data the SSCR1.SPO setting in combination
    with the SSPSCLK phase bit SSCR1.SPH does.
    **/
    UINT32 ClockIdlePolarity   : 1; //[RW][3]
    /**
    SSPSCLK phase setting
    0 - SSPSCLK is inactive one cycle at the start of a frame and cycle at the end of a frame
    1 - SSPSCLK is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
    The SSCR1.SPH bit determines the phase relationship between the SSPSCLK and the serial frame (SSPSFRM) pins when the Motorola SPI format is selected (SSCR0.FRF=00).
    When SSCR1.SPH=0, SSPSCLK remains in its Inactive/Idle state (as determined by the SSCR1.SPO setting)
    for one full cycle after SSPSFRM is asserted low at the beginning of a frame.
    SSPSCLK continues to transition for the rest of the frame and is then held in its Inactive state for one-half of an SSPSCLK period before
    SSPSFRM is de-asserted high at the end of the frame. When SSCR1.SPH=1, SSPSCLK remains in its Inactive/Idle state (as determined by the SSCR1.SPO setting)
    or one-half cycle after SSPSFRM is asserted low at the beginning of a frame. SSPSCLK continues to transition for the remainder of the frame,
    and is then held in its Inactive state for one full SSPSCLK period before SSPSFRM is de-asserted high at the end of the frame.
    The combination of the SSCR1.SPO and SSCR1.SPH settings determines when SSPSCLK is active during the assertion of SSPSFRM,
    and which SSPSCLK edge transmits and receives data on the SSPTXD and SSPRXD pins.
    When SSCR1.SPO and SSCR1.SPH are programmed to the same value (both 0 or both 1),
    Transmit data is driven on the falling edge of SSPSCLK, and Receive data is latched on the rising edge of SSPSCLK.
    When SSCR1.SPO and SSCR1.SPH are programmed to opposite values (one 0 and the other 1),
    Transmit data is driven on the rising edge of SSPSCLK, and Receive data is latched on the falling edge of SSPSCLK.
    **/
    UINT32 ClockPhase      : 1; //[RW][4]
    UINT32 Reserved0       : 1; //[RW][5:13]
    /**
    Enable FIFO Write/Read (test mode bit)
    0 = FIFO write/read special function is disabled (normal SSP operational mode)
    1 = FIFO write/read special function is enabled
    This bit enables a Test mode for the Enhanced SSP. When SSCR1.EFWR = 0, the Enhanced SSP operates in the Normal mode described in this document.
    When SSCR1.EFWR = 1, the Enhanced SSP enters a mode that whenever the CPU reads or writes to the Enhanced SSP Data register,
    it actually reads and writes directly to either the Transmit FIFO or the Receive FIFO, depending on the programmed state of the select FIFO for enable FIFO write/read (SSCR1.STRF) bit.
    With SSCR1.STRF = 0, all Writes to the SSDR are performed on the Transmit FIFO, and Reads to the SSDR will read back the data written to the Transmit FIFO in first-in-first-out order.
    With SSCR1.STRF = 1, all Writes to the SSDR are performed on the Receive FIFO, and Reads to the SSDR will read back the data written to the Receive FIFO in first-in-firstout order.
    In EFWR Test mode, data will not be transmitted on the SSPTXD pin, data input on the SSPRXD pin will not be stored, and the busy and ROR bits will no t work.
    The Interrupt Test Register, however, will still be functional.
    Using software, this mode can test whether or not the Transmit FIFO or the Receive FIFO operates properly as a FIFO memory stack.
    Software should verify that the SSSR.CSS bit has gone from a 1 to a 0 before reading the TX FIFO.
    **/
    UINT32 EnableFifoWriteRead   : 1; //[RW][14]
    /**
    Select FIFO for Enable FIFO Write/Read (STRF)
    0 = Transmit FIFO is selected for both writes and reads through the SSP Data Register (SSDR)
    1 = Receive FIFO is selected for both writes and reads through the SSP Data Register (SSDR)
    This is a test mode bit that selects whether the Transmit or Receive FIFO is enabled for Writes and Reads whenever the SSCR1.EFWR is programmed to 1,
    which puts the Enhanced SSP in a Test mode.
    **/
    UINT32 SelectTxRxFifo        : 1; //[RW][15]
    /**
    Invert Frame Signal
    0 - Frame signal (Chip Select) is active low
    1 - Frame signal (Chip Select) is active high
    **/
    UINT32 InvertFrameSignal      : 1; //[RW][16]
    UINT32 Reserved1              : 1; //[N/A][17]
    /**
    Peripheral Trailing Byte Interrupts Enable
    0 = Peripheral Trailing Byte Interrupts are disabled
    1 = Peripheral Trailing Byte Interrupts are enabled
    This feature is not recommended for use in the LPSS implementation.
    **/
    UINT32 PeripheralTrailingByteIntEnable   : 1; //[RW][18]
    /**
    Receive FIFO Time-out Interrupt Enable
    0 = Receiver Time-out interrupts are disabled
    1 = Receiver Time-out interrupts are enabled
    The SSCR1.TINTE is a read-write bit used to mask or enable the Receiver Time-out Interrupt.
    When SSCR1.TINTE=0, the Interrupt is masked and the state of the SSSR.TINT bit is ignored by the Interrupt controller.
    When SSCR1.TINTE=1, the Interrupt is enabled, and whenever the SSSR.TINT bit is set to one an Interrupt request is made to the Interrupt controller.
    Note that programming SSCR1.TINTE=0 does not affect the current state of the SSSR.TINT or the ability of logic to set and clear the SSSR.TINT;
    it only blocks the generation of the Interrupt request.
    **/
    UINT32 ReceiveFifoTimeoutintEnable       : 1; //[RW][19]
    /**
    Receive FIFO Service Request Enable
    0 = DMA Service Request is disabled
    1 = DMA Service Request is enabled
    The SSCR1.RSRE bit enables the Receive FIFO DMA Service Request.
    When SSCR1.RSRE=0, the DMA Service Request is masked, and the state of the receive FIFO service request SSSR.RFS bit within the Enhanced SSP Status register is ignored.
    When SSCR1.RSRE=1, the DMA Service Request is enabled, and whenever SSSR.RFS is set to one, a DMA Service Request is made to the DMA.
    Note that programming SSCR1.RSRE=0 does not affect the current state of SSSR.RFS or the ability of the Receive FIFO logic to set and clear SSSR.RFS
    it only blocks the generation of the DMA Service Request.
    Also, the state of SSCR1.RFRS does not affect the generation of the Interrupt, which is asserted whenever the SSSR.RFS is set to 1.
    **/
    UINT32 ReceiveFifoServiceRequestEnable   : 1; //[RW][20]
    /**
    Transmit FIFO Service Request Enable
    The SSCR1.TSRE bit enables the Transmit FIFO DMA Service Request.
    When SSCR1.TSRE=0, the DMA Service Request is masked, and the state of the transmit FIFO service request (SSSR.TFS) bit
    within the Enhanced SSP Status register is ignored.
    When SSCR1.TSRE=1, the DMA Service Request is enabled, and whenever SSSR.TFS is set to one,
    a DMA Service Request is made to the DMA. Note that programming SSCR1.TSRE=0 does not affect the current state of
    SSSR.TFS or the ability of the Transmit FIFO logic to set and clear SSSR.TFS it only blocks the generation of the DMA Service Request.
    Also, the state of SSCR1.TSRE does not affect the generation of the Interrupt, which is asserted whenever the SSSR.TFS is set to 1.
    **/
    UINT32 TransmitFifoServiceRequestEnable   : 1; //[RW][21]
    /**
    0 = Processor based, trailing bytes are handled by processor
    1 = DMA based, trailing bytes are handled by DMA
    **/
    UINT32 Trail    : 1; //[RW][22]
    /**
    Receive Without Transmit
    0 - Transmit/Receive mode
    1 - Receive without transmit mode
    The SSCR1.RWOT bit is a read-write bit used to put the Enhanced SSP into a mode similar to half duplex.
    When the Enhanced SSP is in Transmit/Receive mode as determined SSCR1.RWOT=0,
    the Enhanced SSP simultaneously transmits and receives data (as supported by the individual protocols, i.e.,
    normally all modes are full duplex except microwire) and the serial clock SSPSCLK only toggles while an active data transfer is underway.
    When in Receive-without-Transmit mode as determined by SSCR1.RWOT=1, the Enhanced SSP will continue to clock in receive data despite
    data existing or not in the Transmit FIFO. Data is sent/received according to the selected format immediately after the Enhanced SSP enable bit (SSCR0.SSE) is set.
    This allows the Enhanced SSP to receive data without transmitting data (half-duplex only).
    During this mode, if there is no data to send, programmers should disable the DMA service requests and Interrupts for the Transmit FIFO
    (clear the SSCR1.TSRE and SSCR1.TIE bits).
    If the Transmit FIFO is empty, the SSPTXD line will be driven to 0.
    The Transmit FIFO underrun condition will not occur when SSCR1.RWOT=1.
    When RWOT is enabled, the SSSR.BUSY bit will remain active (set to 1) until software clears the RWOT bit.
    If the Enhanced SSP has been in RWOT mode, and software disables this by clearing the RWOT bit,
    an extra frame cycle may occur due to the synchronization between clock domains. This bit must not be used when the SSCR0.MOD bit is set.
    Warning: When RWOT =1, the first entry of the TX FIFO must be initialized with all zeros.
    Note: RWOT is not used in Windows drivers at all. When you enable RWOT you have no control on the data flow (the clock is free-running).
    Even if we have half-duplex operation support from windows we need to control how many data we need to read without transmit.
    We use full-duplex operation to do that putting dummy data into the TX FIFO. Registers SIRFL, TX_BIT_COUNT and RX_BIT_COUNT cannot be
    referred when using RWOT mode
    **/
    UINT32 ReceiveWithoutTransmit   : 1; //[RW][23]
    UINT32 Reserved2                : 8; //[RW][24:31]
  } Fields;
  UINT32 Data32;
} LPSS_SPI_CONTROL1_REG;

/**
  0x8 Enhanced SSP Status register
**/
typedef union {
  struct {
    UINT32 Reserved0     : 2; //[N/A][0:1] Reserved
    /**
    Transmit FIFO Not Full
    0 = Transmit FIFO is full
    1 = Transmit FIFO is not full
    The receive FIFO not empty flag SSSR.RNE is a read-only bit that is set whenever
    the Receive FIFO contains one or more entries of valid data, and is cleared when it no longer contains any valid data.
    This bit can be polled when using programmed I/O to remove remaining bytes of data from the Receive FIFO since CPU Interrupt
    requests are made only when the Receive FIFO Trigger threshold has been met or exceeded.
    This bit does not generate an Interrupt.
    **/
    UINT32 TxFifoNotFull : 1; // [RO][2]
    /**
    Receive FIFO NotEmpty The receive FIFO not empty flag SSSR.
    0 = Receive FIFO is empty
    1 = Receive FIFO is not empty
    RNE is a read-only bit that is set whenever the Receive FIFO contains one or more entries of valid data,
    and is cleared when it no longer contains any valid data.
    This bit can be polled when using programmed I/O to remove remaining bytes of data from the Receive FIFO since CPU Interrupt requests
    are made only when the Receive FIFO Trigger threshold has been met or exceeded.
    This bit does not generate an Interrupt.
    **/
    UINT32 RxFifoNotEmpty : 1; //[RO][3]
    /**
    The Enhanced SSP busy SSSR.
    0 = SSP is idle or disabled
    1 = SSP currently transmitting or receiving a frame
    BSY flag is a read-only bit that is set when the SSP is actively transmitting and/or receiving data,
    and is cleared when the SSP is idle or disabled (SSCR0.SSE=0).
    This bit does not generate an Interrupt.
    **/
    UINT32 Busy           : 1; //[RO][4]
    /**
    Transmit FIFO Service Request
    0 = Transmit FIFO level exceeds the Low Water Mark Transmit FIFO (SITF.LWMTF), or SSP disabled
    1 = Transmit FIFO level is at or below the Low Water Mark Transmit FIFO (SITF.LWMTF), request interrupt
    The transmit FIFO service request flag SSSR.TFS is a read-only bit that is set to generate an Interrupt when the Transmit FIFO
    requires service to prevent an underrun. SSSR.TFS is set any time the
    Transmit FIFO has the same or fewer entries of valid data than indicated by the Transmit FIFO Trigger threshold,
    and it is cleared when it has more entries of valid data than the threshold value (the service request is triggered when the number of FIFO entries is less than or equal to 1 + LWMTF).
    When the SSSR.TFS bit is set, an Interrupt is generated unless the transmit FIFO interrupt request enable (SSCR1.TIE) bit is cleared.
    Also, the setting of the SSSR.TFS bit will generate a DMA service request if the SSCR.TSRE bit was set.
    After the CPU or the DMA fills the FIFO such that it exceeds the threshold, the SSSR.TFS flag (and the service request and/or Interrupt) is automatically cleared.
    If the threshold has not been exceeded, another request will be made. Software should not set both the SSCR1.TSRE and the SSCR1.TIE bits.
    **/
    UINT32 TxFifoService  : 1; //[RO][5]
    /**
    Receive FIFO Service Request
    0 = Receive FIFO level is at or below the water mark for the SPI receive FIFO (WMRF) or SSP disabled
    1 = Receive FIFO level exceeds the water mark for the SPI receive FIFO (WMRF) , request interrupt
    The receive FIFO service request flag SSSR.RFS is a read-only bit that is set to generate an Interrupt when the
    Receive FIFO requires service to prevent an overrun.
    SSSR.RFS is set any time the Receive FIFO has more entries of valid data than the number indicated by the Receive FIFO Trigger threshold,
    and it is cleared when it has the same or fewer entries than the threshold value.
    When the SSSR.RFS bit is set, an Interrupt is generated unless the receive FIFO interrupt request enable (SSCR1.RIE) bit is cleared. Also,
    the setting of the SSSR.RFS bit will signal a DMA service request if the SSCR1.RSRE bit is set.
    After the CPU or DMA reads the FIFO such that it has the same or fewer entries than the SIRF.WMRF value,
    the SSSR.RFS flag (and the service request and/or Interrupt) is automatically cleared.
    Software should not set both the SSCR1.RSRE and the SSCR1.RIE bits.
    **/
    UINT32 RxFifoService  : 1; //[RO][6]
    /**
    Receive FIFO Overrun
    0 = Receive FIFO has not experienced an overrun
    1 = Attempted data write to full receive FIFO, request interrupt
    The receiver overrun status bit SSSR.ROR is a read-write bit that is set when the Receive logic attempts
    to place data into the Receive FIFO after it has been completely filled.
    If the Receive FIFO is full and new data is received, the SSSR.ROR bit is set,
    and the newly received data is discarded. This process is repeated for each new data
    - chunk received until at least one empty FIFO entry exists.
    When the SSSR.ROR bit is set, an Interrupt is generated to the CPU that can be locally masked by the SSCR0.RIM bit.
    The setting of the SSSR.ROR bit does not, however, generate any DMA service request.
    The SSSR.ROR bit remains set until cleared by software writing a 1 to this bit which will also reset its Interrupt request;
    writing a 0 to this bit does not affect SSSR.ROR status.
    **/
    UINT32 RxFifoOverrun  : 1; //[RO][7]
    UINT32 Reserved1      : 10; //[N/A][8:17]
    /**
    Peripheral Trailing Byte Interrupt
    0 = No peripheral trailing byte interrupt pending
    1 = Peripheral trailing byte interrupt pending
    This feature is not recommended for use in the LPSS implementation.
    **/
    UINT32 PeripheralTrailingByteInt : 1; //[RW/1C][18]
    /**
    Receiver Time-out Interrupt
    0 = No receiver time-out pending
    1 = Receive FIFO has been idle for period defined by SSTO
    The SSSR.TINT bit is a read-write bit that is set to 1 when the Receive FIFO has been idle
    (no samples received) for a period of time defined by the value programmed within the Time-Out register (SSTO).
    This interrupt can be masked by the SSCR1.TINTE bit.
    The SSSR.TINT bit is cleared by programmers by writing a 1 to it.
    **/
    UINT32 RxTimeoutInt   : 1; //[RO][19]
    UINT32 Reserved2      : 1; //[N/A][20]
    /**
    Transmit FIFO Under Run
    0 = Transmit FIFO has not experienced an under run
    1 = Attempted read from the transmit FIFO when the FIFO was empty, request interrupt.
    The SSSR.TUR bit is a read-write, one-to-clear status bit that indicates that the transmitter tried to send data from the
    Transmit FIFO when the Transmit FIFO was empty.When the SSSR.TUR bit is set,
    an Interrupt is generated to the CPU that can be locally masked by the SSCR0.TIM bit
    The setting of the SSSR.TUR bit does n ot, however, generate any DMA service request.
    The SSSR.TUR bit remains set until cleared by software writing a 1 to this bit which will also reset its Interrupt request;
    writing a 0 to this bit does not affect SSSR.TUR status.
    This bit can only be set when the Enhanced SSP is a receiver to the FRAME signal (SSCR1.SFRMDIR = 1),
    or if the Enhanced SSP is a transmitter to the FRAME signal and the Enhanced SSP is in Network mode,
    and will not be set if the Enhanced SSP is in Receive-Without- Transmit mode (SSCR1.RWOT =1).
    **/
    UINT32 TxFifoUnderRun  : 1; //[RW/1C][21]
    UINT32 Reserved3       : 10; //[N/A][22:31]
  } Fields;
  UINT32 Data32;
} LPSS_SPI_STATUS_REG;

/**
  0xC Read-Write SSP Interrupt Test
**/
typedef union {
  struct {
    UINT32 Reserved0                  : 5; //[N/A][0:4] Reserved
    /**
      Test Transmit FIFO service request
      0 - No transmit FIFO service request pending
      1 - Generates non-maskable interrupt to CPU and a DMA request for transmit FIFO
    **/
    UINT32 TestTxFifoServiceRequest   : 1; //[RW][6]
    /**
      Test Receive FIFO service request
      0 - No receive
    **/
    UINT32 TestRxFifoServiceRequest   : 1; //[RW][7]
    /**
      Test Receive FIFO overrun
      0 = No receive FIFO overrun service request
      1 = Generates non-maskable interrupt to CPU. No DMA request is generated
      Writing a 1 to the test receive FIFO overrun bit SSITR.TROR, bit 7,
      will generate a non-maskable Interrupt strobe signal to the Interrupt controller only, no DMA request will be made.
      Setting any of these bits will also cause the corresponding status bit(s) to be set in the Enhanced SSP Status register (SSSR).
      The Interrupt and/or service request, caused by the setting of one of these test bits, will remain active
      until the test bit is cleared by writing a 0 to it.
    **/
    UINT32 TestReceiveFxOverrun    : 1; //[RW][8]
    UINT32 Reserved1               : 24; //[N/A][8:31] Reserved
  } Fields;
  UINT32 Data32;
} LPSS_SPI_RW_INTERRUPT_TEST_REG;

/**
  0x10 The Enhanced SSP Data register (SSDR)
  The Enhanced SSP Data registers are single address locations that Read-Write data transfers can access. The SSDR represents
  two physical registers: the first is temporary storage for data on its way out through the Transmit FIFO, the other is temporary
  storage for data coming in through the Receive FIFO. As the system accesses the register, FIFO Control logic transfers data
  automatically between register and FIFO as fast as the system moves it. Data in the FIFO shifts up or down to accommodate the
  new word (unless it's an attempted Write to a full Transmit FIFO). Status bits (such as SSSR.RNE, and SSSR.TNF) show users
  whether the FIFO is full, above/below a programmable FIFO trigger threshold, or empty. For Transmit data transfers (Write from
  system to SSP peripheral), the register can be loaded (written) by the system processor anytime it falls below its threshold level
  when using programmed I/O. When a data size of less than 32-bits is selected, users should not left-justify data written to the
  Transmit FIFO. Transmit logic left-justifies the data and ignores any unused bits. Received data of less than 32 -bits is
  automatically right-justified in the Receive FIFO.
**/

/**
  0x200
**/
typedef union {
  struct {
    UINT32 ClockEnable : 1;     //[RW][0] 0 - No clock Update, 1 - Clock gets updated. Note: For new M and N values to be used by the divider, the
                                //      clk_update bit should be set to 0 & then set to 1.
    UINT32 M           : 15;    //[RW][1:15] M over N divider logic that creates the CLK_OUT. Used to generate the input clk to the SPI
    UINT32 N           : 15;    //[RW][16:31] M over N divider logic that creates the CLK_OUT. Used to generate the input clk to the SPI
    UINT32 ClockUpdate : 1;     //[RW][32]  0 - Clock disabled 1 - Clock Enabled.
  } Fields;
  UINT32 Data32;
} LPSS_SPI_CLOCK;

// 0x204, SPI Soft Reset Register
typedef union {
  struct {
    UINT32  ResetSpi       :2;    //[RW][0:1] SPI Host Controller reset.
                                  //    00 : SPI Host Controller is in reset (Reset Asserted)
                                  //    11 : SPI Host Controller is NOT at reset (Reset Released)
    UINT32  ResetDma       :1;    //[RW][2] iDMA Software Reset Control
                                  //    0 : IP is in reset (Reset Asserted)
                                  //    1 : IP is NOT at reset (Reset Released)
    UINT32  Reserved       :29;   //[N/A][3:31] Reserved
  } Fields;
  UINT32 Data32;
} LPSS_SPI_RESETS;

// 0x84, Power Management Control And Status Register
typedef union {
  struct {
    UINT32  PwrState            :2;     //[RW][0-1] Power State Field
    UINT32  Reserved0           :1;     //[RW][2] Reserved
    UINT32  NoSoftReset         :1;     //[RW][3] No Soft Reset Field
    UINT32  Reserved1           :4;     //[RW][4-7] Reserved
    UINT32  PmeEn               :1;     //[RW][8] Pme Enable Field
    UINT32  Reserved2           :6;     //[RW][9-14] Reserved
    UINT32  PmeSts              :1;     //[R][15] Pme Status Field
    UINT32  Reserved3           :16;    //[RW][16-31] Reserved
  } Fields;
  UINT32 Data32;
} LPSS_SPI_PWRMG_CONTROL_AND_STATUS;

/**
  Wait For LPSS SPI controller to be in Idle state by checking the Busy bit

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
VOID
LpssSpiWaitIdle (
  IN UINTN SpiBaseAddress
  )
{
  UINT8                     Timeout;
  LPSS_SPI_STATUS_REG       SpiStatusRegister;

  Timeout = LpssSpiWaitIdleTimeout;
  SpiStatusRegister.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSSR);
  while (SpiStatusRegister.Fields.Busy && (Timeout > 0)) {
    MicroSecondDelay (LpssSpiSingleOperationDelay);
    Timeout--;
    SpiStatusRegister.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSSR);
  }
}

/**
  Return LPSS SPI controller Tx Fifo Not Full status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
LpssSpiTxFifoNotFull (
  IN UINTN SpiBaseAddress
  )
{
  LPSS_SPI_STATUS_REG  SpiStatusRegister;

  SpiStatusRegister.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSSR);
  return (BOOLEAN) SpiStatusRegister.Fields.TxFifoNotFull;
}

/**
  Return LPSS SPI controller Rx Fifo Not Empty status

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
BOOLEAN
LpssSpiRxFifoNotEmpty (
  IN UINTN SpiBaseAddress
  )
{
  LPSS_SPI_STATUS_REG  SpiStatusRegister;

  SpiStatusRegister.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSSR);
  return (BOOLEAN) SpiStatusRegister.Fields.RxFifoNotEmpty;
}

/**
  Wait For LPSS SPI controller to have no interrupts pending

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
**/
VOID
STATIC
LpssSpiWaitNoInt (
  IN UINTN SpiBaseAddress
  )
{
  UINT8                                Count;
  LPSS_SPI_RW_INTERRUPT_TEST_REG       SpiRWInterruptTest;
  //
  // Clear all interrupts
  //
  SpiRWInterruptTest.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSITR);
  //
  // Ensure there are no interrupts in progress.
  //
  Count = 0;
  while (SpiRWInterruptTest.Fields.TestTxFifoServiceRequest &&
         SpiRWInterruptTest.Fields.TestRxFifoServiceRequest &&
         SpiRWInterruptTest.Fields.TestReceiveFxOverrun &&
         (Count < LpssSpiNoIntCounter)) {
    SpiRWInterruptTest.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSITR);
    Count ++;
  }
}

/**
  This function does the basic init work for SPI controller.

  @param[in]  SpiBaseAddress  - Base Address of SPI Controller
  @param[in]  Frequency         Bus clock frequency (in kHz)
  @param[in]  ClockPhase        0 - Clock is inactive one cycle at the start of a frame and cycle at the end of a frame
                                1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]  ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]  CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
**/
VOID
STATIC
LpssSpiControllerBusInit (
  IN UINTN           SpiBaseAddress,
  IN UINT32          Frequency,
  IN UINT8           ClockPhase,
  IN UINT8           ClockIdlePolarity,
  IN UINT8           CsActivePolarity
  )
{
  LPSS_SPI_CONTROL0_REG  SpiControl0;
  LPSS_SPI_CONTROL1_REG  SpiControl1;
  LPSS_SPI_CLOCK         SpiClock;
  UINT16                 SpiMClock;

  SpiControl0.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0);
  SpiControl0.Fields.DataSizeSelect = V_LPSS_MEM_SPI_SSCR0_DSS_8;
  SpiControl0.Fields.FrameFormat = 0x0; //00 - Motorola Serial Peripheral Interface (SPI)
  SpiControl0.Fields.ExtClockSelect = 0x0;
  SpiControl0.Fields.SynchronousSerialPortEnable = 0x1;
  SpiControl0.Fields.SerialClockRateValue = 0x0; // Use the M/N divider
  SpiControl0.Fields.ExtDataSizeSelect = 0x0; // 4 - 16 bits
  SpiControl0.Fields.NetworkClockSelect = 0x0;
  SpiControl0.Fields.RxFifoOverrunIntMask = 0x1;
  SpiControl0.Fields.TxFifoOverrunIntMask = 0x1;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0, SpiControl0.Data32);

  SpiControl1.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR1);
  SpiControl1.Fields.RxFifoIntEnable = 0x0;
  SpiControl1.Fields.TxFifoIntEnable = 0x0;
  SpiControl1.Fields.LoopBackMode = 0x0;
  SpiControl1.Fields.ClockIdlePolarity = ClockIdlePolarity;
  SpiControl1.Fields.ClockPhase = ClockPhase;
  SpiControl1.Fields.EnableFifoWriteRead = 0x0;
  SpiControl1.Fields.SelectTxRxFifo = 0x0;
  SpiControl1.Fields.InvertFrameSignal = CsActivePolarity;
  SpiControl1.Fields.PeripheralTrailingByteIntEnable = 0x0;
  SpiControl1.Fields.ReceiveFifoTimeoutintEnable = 0x0;
  SpiControl1.Fields.ReceiveFifoServiceRequestEnable = 0x0;
  SpiControl1.Fields.TransmitFifoServiceRequestEnable = 0x0;
  SpiControl1.Fields.Trail = 0x0;
  SpiControl1.Fields.ReceiveWithoutTransmit = 0x0;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR1, SpiControl1.Data32);

  //
  // Configure M/N dividers
  //
  SpiClock.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_CLOCKS);
  //M = (spi_clock * 32767) / base_clock;
  SpiMClock = (UINT16) DivU64x64Remainder (MultU64x32 (Frequency, 32767), LPSS_SPI_DEFAULT_CLOCK_FREQUENCY, NULL);
  SpiClock.Fields.M = SpiMClock;
  SpiClock.Fields.N = 32767;
  SpiClock.Fields.ClockUpdate = 0x0;
  SpiClock.Fields.ClockEnable = 0x1;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (LpssSpiClockUpdate);
  SpiClock.Fields.ClockUpdate = 0x1;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (LpssSpiClockUpdate);
  SpiClock.Fields.ClockUpdate = 0x0;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_CLOCKS, SpiClock.Data32);
  MicroSecondDelay (LpssSpiClockUpdate);
  SpiClock.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_CLOCKS);
  LpssSpiWaitNoInt (SpiBaseAddress);
}

/**
  Set LPSS SPI controller Active Chip Select

  @param[in]  SpiBaseAddress - Base Address of SPI Controller
  @param[in]  CsOutput       - Default CS used by the SPI HC. 0 = CS0, 1 = CS1, CS2, CS3.
  @param[in]  CsMode         - Sets Chip Select Control mode Hardware or Software. 0 = HW Control, 1 = SW Control
  @param[in]  CsState        - 0 = CS is set to low; 1 = CS is set to high
  @param[in]  CsIdlePolarity   Sets Inactive/Idle polarity of CSn (CsOutput)
**/
VOID
LpssSpiSetCsControl (
  IN UINTN  SpiBaseAddress,
  IN UINT8  CsOutput,
  IN UINT8  CsMode,
  IN UINT8  CsState,
  IN UINT8  CsIdlePolarity
  )
{
  LPSS_SPI_CS_CONTROL_REG  SpiCsControl;

  SpiCsControl.Data32 = MmioRead32 ((SpiBaseAddress + R_LPSS_SPI_MEM_SPI_CS_CONTROL));
  //
  // Set CS mode
  //
  SpiCsControl.Fields.CsMode = CsMode;
  //
  // Set CS Output Select
  //
  SpiCsControl.Fields.CsOutputSelect = CsOutput;
  //
  // Set CS state
  //
  SpiCsControl.Fields.CsState = CsState;
  if (CsOutput == 0) {
    SpiCsControl.Fields.Cs0Polarity = CsIdlePolarity;
  }
  if (CsOutput == 1) {
    SpiCsControl.Fields.Cs1Polarity = CsIdlePolarity;
  }
  MmioWrite32 ((SpiBaseAddress + R_LPSS_SPI_MEM_SPI_CS_CONTROL), SpiCsControl.Data32);
}

/**
  Deactivates SPI device
  - Revert CS to Idle state
  - Disable SSP

  @param[in] SpiBaseAddress  - Base Address of SPI Controller
  @param[in] CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in] CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
**/
VOID
LpssSpiDeactivate (
  IN UINTN  SpiBaseAddress,
  IN UINT8  CsOutputSelect,
  IN UINT8  CsActivePolarity
  )
{
  LPSS_SPI_CONTROL0_REG  SpiControl0;
  //
  // Deactivate CS
  //
  LpssSpiSetCsControl (SpiBaseAddress, CsOutputSelect, LpssSpiCsModeSoftware, LpssSpiCsStateDeAsserted, !(CsActivePolarity & BIT0));
  //
  // Disable SPI Operation
  //
  SpiControl0.Data32 = MmioRead32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0);
  SpiControl0.Fields.SynchronousSerialPortEnable = 0;
  MmioWrite32 (SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0, SpiControl0.Data32);
}

/**
  Enable Spi device when in PCI only if it has assigned Memory resources

  - Enable Memory space decoding
  - Place device in D0
  - Get it out of reset

  @param[in]       PciCfgBase       Pci Config Base Address of specified SPI controller
  @param[in][out]  SpiBaseAddress   MMIO Base Address of specified SPI controller

  @retval   TRUE    SPI successfully enabled
  @retval   FALSE   SPI does not have memory assigned or SDA/SCL are not high
**/
BOOLEAN
STATIC
LpssSpiPciEnable (
  IN UINT64         PciCfgBase,
  IN OUT UINTN      *SpiBaseAddress
  )
{
  LPSS_SPI_RESETS                SpiResets;
  LPSS_SPI_PWRMG_CONTROL_AND_STATUS  SpiPwrMgmControlStsReg;

  *SpiBaseAddress = (UINTN) ((PciSegmentRead32 ((PciCfgBase + 0x10)) & 0xFFFFF000) + LShiftU64 (PciSegmentRead32 ((PciCfgBase + 0x10 + 4)), 32));
  if ((*SpiBaseAddress == 0xFFFFFFFFFFFFF000) || (*SpiBaseAddress == 0x0)) { //Device is still not enabled, or there is no BAR assigned
    DEBUG ((DEBUG_WARN, "%a () - No BAR assigned 0x%X\n",  __FUNCTION__, *SpiBaseAddress));
    return FALSE;
  }

  PciSegmentOr8 (PciCfgBase + PCI_COMMAND_OFFSET, EFI_PCI_COMMAND_MEMORY_SPACE); // Enable MSE

  SpiPwrMgmControlStsReg.Data32 = PciSegmentRead32 (PciCfgBase + R_LPSS_SPI_CFG_PMECTRLSTATUS);
  SpiPwrMgmControlStsReg.Fields.PwrState = 0x0; // D0 State
  PciSegmentWrite32 (PciCfgBase + R_LPSS_SPI_CFG_PMECTRLSTATUS, SpiPwrMgmControlStsReg.Data32);
  PciSegmentRead8 (PciCfgBase + R_LPSS_SPI_CFG_PMECTRLSTATUS); // Dummy Read after modifying PME

  SpiResets.Data32 = MmioRead32 (*SpiBaseAddress + R_LPSS_SPI_MEM_RESETS);
  SpiResets.Fields.ResetSpi = 3;
  SpiResets.Fields.ResetDma = 0;
  MmioWrite32 (*SpiBaseAddress + R_LPSS_SPI_MEM_RESETS, SpiResets.Data32);

  return TRUE;
}

/**
  Set up the SPI Controller for reading and/or writing

  @param[in]       PciCfgBase        Pci Config Base Address of specified SPI controller
  @param[in][out]  SpiBaseAddress    MMIO Base Address of specified I2C controller
  @param[in]       Frequency         Bus clock frequency (in kHz)
  @param[in]       ClockIdlePolarity Clock Idle Polarity 0 = Low, 1 = High
  @param[in]       ClockPhase        0 - Clock is inactive one cycle at the start of a frame and one cycle at the end of a frame
                                     1 - Clock is inactive for one halfcycle at the start of a frame and one cycle at the end of a frame
  @param[in]       CsOutputSelect    Select which SPI CS Signal is to be driven by the SPI Frame (0 = CS0; 1=CS1 etc.)
  @param[in]       CsActivePolarity  Selects SPI ChipSelect signal polarity, 0 = Low, 1 = High
  @param[in]       AssertCs          FALSE - CS will remain in De-Asserted case. Used for scenario when external CS is used (ex GPIO)
                                     TRUE - CS either CS0/CS1 will be asserted once SPI transaction starts

  @retval   EFI_TIMEOUT        Timeout occurred in SPI disable/enable
  @retval   EFI_SUCCESS        W/R successfully initialized
  @retval   EFI_DEVICE_ERROR   Serial Io Spi Device is still not enabled or there is no BAR assigned
**/
EFI_STATUS
LpssSpiInitializeWriteRead (
  IN UINT64       PciCfgBase,
  IN OUT UINTN    *SpiBaseAddress,
  IN UINTN        Frequency,
  IN UINT8        ClockIdlePolarity,
  IN UINT8        ClockPhase,
  IN UINT8        CsOutputSelect,
  IN UINT8        CsActivePolarity,
  IN BOOLEAN      AssertCs
  )
{
  UINT8                  Count;
  LPSS_SPI_CONTROL0_REG  SpiControl0;

  if (!LpssSpiPciEnable (PciCfgBase, SpiBaseAddress)) {
    DEBUG ((DEBUG_WARN, "%a () - Device is still not enabled or there is no BAR assigned.\n", __FUNCTION__));
    return EFI_DEVICE_ERROR;
  }
  //
  // 1.Set CS mode to SW, deassert CS line before changing CPL, CPH, IFS settings
  //
  LpssSpiSetCsControl (*SpiBaseAddress, CsOutputSelect, LpssSpiCsModeSoftware, LpssSpiCsStateDeAsserted, !(CsActivePolarity & BIT0));
  //
  // 2. Disable controller
  //
  SpiControl0.Data32 = MmioRead32 (*SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0);
  SpiControl0.Fields.SynchronousSerialPortEnable = 0;
  MmioWrite32 (*SpiBaseAddress + R_LPSS_SPI_MEM_SSCR0, SpiControl0.Data32);
  //
  // 3. Spi initalization
  //
  LpssSpiControllerBusInit (*SpiBaseAddress, (UINT32) Frequency, ClockIdlePolarity, ClockPhase, CsActivePolarity);
  //
  // 4. Make sure SPI is not running
  //
  LpssSpiWaitIdle (*SpiBaseAddress);
  Count = 0;
  while ((MmioRead32 (*SpiBaseAddress + R_LPSS_SPI_MEM_SIRF) > 0) && (Count < LpssSpiNoIntCounter)) {
    MmioRead32 (*SpiBaseAddress + R_LPSS_SPI_MEM_SIRF);
    Count++;
  }
  if (Count == LpssSpiNoIntCounter) {
    return EFI_TIMEOUT;
  }
  //
  // 5. Activate CS
  //
  if (AssertCs == TRUE) {
    LpssSpiSetCsControl (*SpiBaseAddress, CsOutputSelect, LpssSpiCsModeSoftware, LpssSpiCsStateAsserted, !(CsActivePolarity & BIT0));
  }
  return EFI_SUCCESS;
}
