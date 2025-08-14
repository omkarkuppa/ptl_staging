/**@file
  Header file for Touch Panel Driver

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2013 Intel Corporation.

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

#ifndef _EFI_I2C_TOUCH_PANEL_H_
#define _EFI_I2C_TOUCH_PANEL_H_

#include <Uefi.h>

//
// Libraries
//
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>
#include <Library/DevicePathLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/IoLib.h>
#include <Library/PerformanceLib.h>
#include <Library/TimerLib.h>

//
// UEFI Driver Model Protocols
//
#include <Protocol/DriverBinding.h>
#include <Protocol/DriverSupportedEfiVersion.h>
#include <Protocol/ComponentName2.h>
#include <Protocol/ComponentName.h>

//
// Consumed Protocols
//
#include <Protocol/I2cIo.h>

//
// Produced Protocols
//
#include <Protocol/AbsolutePointer.h>

#include <Library/HidI2cPlatformSupportLib.h>

//
// Driver Version
//
#define TOUCH_DRIVER_VERSION  0x00000000
#define MAX_HID_COLLECTION    256

//
// Protocol instances
//
extern EFI_DRIVER_BINDING_PROTOCOL  gTouchPanelDriverDriverBinding;
extern EFI_COMPONENT_NAME2_PROTOCOL gTouchPanelDriverComponentName2;
extern EFI_COMPONENT_NAME_PROTOCOL  gTouchPanelDriverComponentName;

//
// Include files with function prototypes
//
//#include "ComponentName.h"
//#include "AbsolutePointer.h"



#define DEBUG_I2C_OPERATION   0x40000000
//#define DEBUG_ENTRY_EXIT      0x20000000
#define DEBUG_INT_OUTPUT      0x01000000
#define DEBUG_ENTRY_EXIT      DEBUG_INFO

#define TOUCH_SIGNATURE  0x544F4348 //"TOCH"
#define TOUCH_CONTEXT_FROM_PROTOCOL(a)   CR (a, ABSOLUTE_POINTER_DEV, AbsolutePointerProtocol, TOUCH_SIGNATURE)
typedef struct _EFI_TOUCH_INIT_TAB EFI_TOUCH_INIT_TABLE;

#define THC_HID_ELAN_VID 0x04F3
#define DEFAULT_HID_DESCRIPTOR_LENGTH   0x1E

#define USE_PAGE       0x01
#define DIGITIZER      0x0D
#define DESKTOP        0x01
#define USAGE          0x02
#define TOUCHPANEL     0x04
#define TIP_SWITCH     0x42
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

//
// I2C Touch Panel command set
//
#define OPCODE_SET_POWER       0x8
#define POWER_STATE_ON         0x0
#define POWER_STATE_SLEEP      0x1
#define OPCODE_RESET           0x1
#define RESET_REPORT_ID        0x0
#define WAIT_FOR_RESET_TIMEOUT 5000
#define RESET_RESPONSE_BYTE_0  0x0
#define RESET_RESPONSE_BYTE_1  0x0

#define SET_IDLE                   0x05
#define I2C_HID_LENGTH_PREFIX      2
#define THC_ELAN_HID_SKIP_LENGTH   16
#define THC_ELAN_HID_MAX_LENGTH    32
#define RELATIVE_COORDINATES_MAX_X 1024
#define RELATIVE_COORDINATES_MAX_Y 1024

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
} INPUT_REPORT_COLLECTION;

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

//
// dictionary for parsing incoming data from touchpanel
//
typedef struct {
  UINT32                   Id;                // ReportID. 0 means panel doesn't send ReportIDs, and it also means there is only 1 format
  UINT32                   CollectionCount;
  INPUT_REPORT_COLLECTION  Collection[MAX_HID_COLLECTION];
} INPUT_REPORT_FORMAT;

//
// set of dictionaries for parsing incoming data from touchpanel
//
typedef struct {
  UINT32                 Quantity;   // number of different report formats a panel might send
  UINT32                 TouchPanel; // flag to determine if this device is a touchpanel and not e.g. touchpad
  INPUT_REPORT_FORMAT*   Report;     // a pointer to array of dictionaries for report formats
} INPUT_REPORT_TABLE;

typedef struct {
  UINTN                         Signature;               //needed for CONTEXT_FROM_PROTOCOL
  EFI_I2C_IO_PROTOCOL          *I2cIoProtocol;           //needed I2cBus or I2cIo
  EFI_ABSOLUTE_POINTER_PROTOCOL AbsolutePointerProtocol;
  EFI_ABSOLUTE_POINTER_STATE    State;                   //storage space for AbsPtr's State
  EFI_ABSOLUTE_POINTER_MODE     Mode;                    //storage for AbsPtr's Mode
  BOOLEAN                       NewStateAvailable;
  EFI_UNICODE_STRING_TABLE      *ControllerNameTable;
  INPUT_REPORT_TABLE            InputReportTable;        //dictionary for parsing data from panel into x/y coordinates
  SINGLE_FINGER_REPORT          Report;
  HID_DESC                      HidDescriptor;
  EFI_HANDLE                    ControllerHandle;
  HID_I2C_PLATFORM_SUPPORT      HidI2cPlatformSupport;   //platform-specific functions
  EFI_EVENT                     PollingEvent;
  UINT8                         HidSolutionFlag;
} ABSOLUTE_POINTER_DEV;

typedef struct {
  UINTN ReportId;
  UINTN MaxX;
  UINTN StartX;
  UINTN StopX;
  UINTN MaxY;
  UINTN StartY;
  UINTN StopY;
  UINTN Button;
} REPORT_DESCRIPTOR;

typedef enum {
  WacomHidProtocol      = 1,
  ElanHidProtocol       = 2,
} THC_HID_PROTOCOL;

typedef struct {
  UINT32 ID;
  UINT32 Value;
} TOKEN;

typedef struct {
  UINT32               GlobalUsage;
  UINT32               Usages;
  UINT32               UsageB;
  UINT32               UsageX;
  UINT32               UsageY;
  UINT32               LogMax;
  UINT32               LogMin;
  UINT32               ReportSize;
  UINT32               ReportCount;
  UINT32               GlobalUsageOnStack;
  UINT32               ReportSizeOnStack;
  UINT32               ReportCountOnStack;
  UINT32               TouchPanelUsage;
  INPUT_REPORT_FORMAT  TempReport;
} PARSER_STACK;

typedef struct {
  UINT32 B;
  UINT32 X;
  UINT32 Y;
} TOUCH_OUTPUT;

typedef struct {
  UINTN MinX;
  UINTN MaxX;
  UINTN MinY;
  UINTN MaxY;
} TOUCH_XY_BOUNDARY;

typedef struct {
  UINT32 ID;
  UINT32 Value;
} REPORT_DESCRIPTOR_TOKEN;

/**
  Looks for controllers which declare the EFI_I2C_IO_PROTOCOL and match the device path
  supplied by the silicon vendor or third party I2C driver writer to the platform integrator.

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to test. This handle
                                   must support a protocol interface that supplies
                                   an I/O abstraction to the driver.
  @param[in]  RemainingDevicePath  A pointer to the remaining portion of a device path.  This
                                   parameter is ignored by device drivers, and is optional for bus
                                   drivers. For bus drivers, if this parameter is not NULL, then
                                   the bus driver must determine if the bus controller specified
                                   by ControllerHandle and the child controller specified
                                   by RemainingDevicePath are both supported by this
                                   bus driver.

  @retval EFI_SUCCESS              The device specified by ControllerHandle and
                                   RemainingDevicePath is supported by the driver specified by This.
  @retval EFI_ALREADY_STARTED      The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by the driver
                                   specified by This.
  @retval EFI_ACCESS_DENIED        The device specified by ControllerHandle and
                                   RemainingDevicePath is already being managed by a different
                                   driver or an application that requires exclusive access.
                                   Currently not implemented.
  @retval EFI_UNSUPPORTED          The device specified by ControllerHandle and
                                   RemainingDevicePath is not supported by the driver specified by This.
**/
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingSupported (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Starts touchpanel device controller.
  If initialization completes successfully, this driver will install AbsolutePointer protocol
  on this device. This protocol can be further used for features such as virtual keyboard.

  Initialization steps:
  - Call init function supplied from platform-specific lib to initialize hardware
  - Read device's HID descriptor, according to Microsoft's HID over I2C specification
  - Using data from HID descriptor, build a table of input descriptors. A single HID device may
    produce data in more than 1 format and each needs to be parsed differently.

  Device context caches one touch event. Cache gets invalidated when its data is consumed by calling
  GetState() from AbsolutePointer protocol.
  Driver checks device's interrupt and potentially retrieves new touch data only if
  cache is empty and either GetState() was called or a periodic timer event was triggered.
  When new data is retrieved from device, absolute pointer's WaitForInput event gets signalled
  to inform users of this protocol that new data is available.
  Such construction allows the following uses of Absolute Pointer protocol:
  - consumer calls gBS->WaitForEvent on AbsPtr's WaitForEvent event, then calls GetState which is
    guaranteed to contain valid data (the suggested way of using AbsolutePointer protocol)
  - consumer calls gBS->checkEvent on AbsPtr's WaitForevent event, and if it returned success then
    consumer may call Getstate() which is guaranteed to contain valid data
  - consumer calls GetState() without bothering with events; GetState() either returns valid data
    or an error

  @param[in]  This                 A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle     The handle of the controller to start. This handle
                                   must support I2C protocol interface.
  @param[in]  RemainingDevicePath  ignored

  @retval EFI_SUCCESS              The device was started.
  @retval EFI_DEVICE_ERROR         The device could not be started due to a device error.Currently not implemented.
  @retval EFI_OUT_OF_RESOURCES     The request could not be completed due to a lack of resources.
  @retval Others                   The driver failded to start the device.

**/
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingStart (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN EFI_HANDLE                   ControllerHandle,
  IN EFI_DEVICE_PATH_PROTOCOL     *RemainingDevicePath OPTIONAL
  );

/**
  Stops a device controller or a bus controller.

  The Stop() function is designed to be invoked from the EFI boot service DisconnectController().
  As a result, much of the error checking on the parameters to Stop() has been moved
  into this common boot service. It is legal to call Stop() from other locations,
  but the following calling restrictions must be followed, or the system behavior will not be deterministic.
  1. ControllerHandle must be a valid EFI_HANDLE that was used on a previous call to this
     same driver's Start() function.
  2. The first NumberOfChildren handles of ChildHandleBuffer must all be a valid
     EFI_HANDLE. In addition, all of these handles must have been created in this driver's
     Start() function, and the Start() function must have called OpenProtocol() on
     ControllerHandle with an Attribute of EFI_OPEN_PROTOCOL_BY_CHILD_CONTROLLER.

  @param[in]  This              A pointer to the EFI_DRIVER_BINDING_PROTOCOL instance.
  @param[in]  ControllerHandle  A handle to the device being stopped. The handle must
                                support a bus specific I/O protocol for the driver
                                to use to stop the device.
  @param[in]  NumberOfChildren  The number of child device handles in ChildHandleBuffer.
  @param[in]  ChildHandleBuffer An array of child handles to be freed. May be NULL
                                if NumberOfChildren is 0.

  @retval EFI_SUCCESS           The device was stopped.
  @retval EFI_DEVICE_ERROR      The device could not be stopped due to a device error.

**/
EFI_STATUS
EFIAPI
TouchPanelDriverDriverBindingStop (
  IN EFI_DRIVER_BINDING_PROTOCOL  *This,
  IN  EFI_HANDLE                  ControllerHandle,
  IN  UINTN                       NumberOfChildren,
  IN  EFI_HANDLE                  *ChildHandleBuffer OPTIONAL
  );

/**
  Check the touch device do send interrupt event.

  @param[in] Event    Event whose notification function is being invoked.
  @param[in] Context  Pointer to the notification function's context,
                      which is implementation-dependent.  Context corresponds
                      to NotifyContext in CreateEventEx().
**/
VOID
EFIAPI
CheckDataAvailableCallback (
  IN EFI_EVENT Event,
  IN VOID      *Context
  );

/**
  Helper function that concatenates two 8-bit variables into 16bit value
  @param[in]  Lsb               LSB data
  @param[in]  Msb               Msb Data

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
UINT16
GetLength (
  IN UINT8 Lsb,
  IN UINT8 Msb
  );

/**
  Gets HID descriptor and initializes dictionary for parsing touch input data

  @param  DriverContext         The Code context.

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
EFIAPI
TouchDevInit(
  IN ABSOLUTE_POINTER_DEV  *TouchDev
  );

/**
  A HID-compliant device may have more than 1 functionality, for example
  digitizer/touchpanel/pen/button etc. Each functionality reports data in
  a different format. In order to correctly parse that data, a set of dictionaries
  must be built.
  This function reads HID device's InputReportDescriptor and uses its data to
  create dictionaries for all types of InputReports this device supports.
  Such set of dictionaries is held in InputReportTable.
  InputDataDescriptor is parsed sequentially. In a loop, GetToken() extracts single
  data token from the descriptor stream. Those tokens are passed to UpdateStack()
  which updates ParseStack, which is a not-yet-complete dictionary. Once
  a dictionary is complete, it is either added to InputReportTable or discarded.

  @param TouchDev               A pointer to the touch controller

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
  */
EFI_STATUS
ReadInputReportDescriptor (
  IN ABSOLUTE_POINTER_DEV *TouchDev
  );

/**
  This function uses dictionaries to parse incoming InputReport and convert it into X/Y coordinates plus Button info.

  @param[in]  ReportTable             Report Table with all supported HID reports
  @param[in]  InputStream             Pointer to the HID report
  @param[in]  Output                  Result X/Y/B data
  @param[in]  MinMax                  X/Y Min and Max data
  @param[in]  HidSolutionFlag         Flag for HID protocol

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.

**/
EFI_STATUS
ParseInput (
  INPUT_REPORT_TABLE ReportTable,
  UINT8              HidSolution,
  UINT8              *InputStream,
  TOUCH_OUTPUT       *Output,
  TOUCH_XY_BOUNDARY  *MinMax
  );

/**
  This function limits touchpanel's interrupt frequency to
  prevent traffic congestion on I2C

  @param TouchDev               A pointer to the touch controller

  @retval EFI_SUCCESS           The operation completed successfully.
  @retval Others                An unexpected error occurred.
**/
EFI_STATUS
SetInterruptFrequency (
  IN ABSOLUTE_POINTER_DEV *TouchDev
  );

/**
  For serial debugger used, it will show the buffer message line by line to serial console.

  @param[in] Message              The address point of buffer message
  @param[in] Length               Message length

**/
VOID
ShowBuffer (
  IN  UINT8                       *Message,
  IN  UINT32                      Length
  );

#endif
