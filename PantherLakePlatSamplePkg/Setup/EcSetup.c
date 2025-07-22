/** @file
  Extracts system info from EC.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 1999 Intel Corporation.

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


#include <SetupPrivate.h>
#include <Library/EcMiscLib.h>
#include "Setup.h"
#include <Library/EcLib.h>
#include <Library/DebugLib.h>
#include <Library/TimerLib.h>

#define STALL_TIME                          1000000    // 1,000,000 microseconds = 1 second

/**
  Returns system monitor values from the EC.

  @param[in]  KeyValue
  @param[out] MonitorValues  Pointer to a structure to return the monitor values in.

  @retval     EFI_INVALID_PARAMETER
  @retval     EFI_SUCCESS
**/
EFI_STATUS
GetSystemMonitorValues (
    IN  UINT16                             KeyValue,
    OUT MOBILE_SYSTEM_MONITOR_INFO*        MonitorValues
  )
{
  // Check input parameters
  if (MonitorValues == NULL) {
    return EFI_INVALID_PARAMETER;
  }
#if FixedPcdGetBool (PcdEcEnable) == 0
  return EFI_UNSUPPORTED;
#else

  if (PcdGetBool (PcdEcPresent) == FALSE) {
    return EFI_UNSUPPORTED;
  }

  switch(KeyValue)
  {
    case THERMAL_SENSOR_1_KEY:
    {
      //
      // Read Thermal Sensor from EC
      //
      GetSensorTemp (1, &MonitorValues->EC_TSR1);
      break;
    }
    case THERMAL_SENSOR_2_KEY:
    {
      //
      // Read Thermal Sensor from EC
      //
      GetSensorTemp (2, &MonitorValues->EC_TSR2);
      break;
    }
    case THERMAL_SENSOR_3_KEY:
    {
      //
      // Read Thermal Sensor from EC
      //
      GetSensorTemp (3, &MonitorValues->EC_TSR3);
      break;
    }
    case THERMAL_SENSOR_4_KEY:
    {
      //
      // Read Thermal Sensor from EC
      //
      GetSensorTemp (4, &MonitorValues->EC_TSR4);
      break;
    }
    case THERMAL_SENSOR_5_KEY:
    {
      //
      // Read Thermal Sensor from EC
      //
      GetSensorTemp (5, &MonitorValues->EC_TSR5);
      break;
    }
    case CPU_FAN_KEY :
    {
      //
      // Read CPU Fan Speed from EC
      //
      EcGetFanSpeed (&MonitorValues->CPUFanSpeed);
      break;
    }
    case PCH_DTS_TEMP_KEY :
    {
      //
      // Read PCH DTS Temp from EC
      //
      GetPchDtsTemp (&MonitorValues->PCHDTSTemp);
      break;
    }
    default:
    break;
  }

  return EFI_SUCCESS;
#endif
}


/**
  CallBack to update the initial thermal string to HiiHandle

  @param  HiiHandle              The HII Handle of the form to update.
  @param  Class                  Indicates the setup class
**/
VOID
InitHhmMobileStrings (
  IN EFI_HII_HANDLE             HiiHandle,
  IN UINT16                     Class
  )
{
  UINT16                        Value1;
  UINT16                        Value2;
  MOBILE_SYSTEM_MONITOR_INFO    MonitorValues = { 0 };

  if (Class != ADVANCED_FORM_SET_CLASS) {
    return ;
  }
  if (PcdGetBool (PcdEcPresent) == TRUE) {
    // Get system monitor values from the EC.
    GetSystemMonitorValues (THERMAL_SENSOR_1_KEY, &MonitorValues);
    GetSystemMonitorValues (THERMAL_SENSOR_2_KEY, &MonitorValues);
    GetSystemMonitorValues (THERMAL_SENSOR_3_KEY, &MonitorValues);
    GetSystemMonitorValues (THERMAL_SENSOR_4_KEY, &MonitorValues);
    GetSystemMonitorValues (THERMAL_SENSOR_5_KEY, &MonitorValues);
    GetSystemMonitorValues (CPU_FAN_KEY, &MonitorValues);
    GetSystemMonitorValues (PCH_DTS_TEMP_KEY, &MonitorValues);

    Value1 = (UINT16)(MonitorValues.EC_TSR1 / 10);
    Value2 = (UINT16)(MonitorValues.EC_TSR1 % 10);
    InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_1_VALUE), L"%d.%01d C", Value1, Value2);

    Value1 = (UINT16)(MonitorValues.EC_TSR2 / 10);
    Value2 = (UINT16)(MonitorValues.EC_TSR2 % 10);
    InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_2_VALUE), L"%d.%01d C", Value1, Value2);

    Value1 = (UINT16)(MonitorValues.EC_TSR3 / 10);
    Value2 = (UINT16)(MonitorValues.EC_TSR3 % 10);
    InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_3_VALUE), L"%d.%01d C", Value1, Value2);

    Value1 = (UINT16)(MonitorValues.EC_TSR4 / 10);
    Value2 = (UINT16)(MonitorValues.EC_TSR4 % 10);
    InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_4_VALUE), L"%d.%01d C", Value1, Value2);

    Value1 = (UINT16)(MonitorValues.EC_TSR5 / 10);
    Value2 = (UINT16)(MonitorValues.EC_TSR5 % 10);
    InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_5_VALUE), L"%d.%01d C", Value1, Value2);

    InitString(HiiHandle, STRING_TOKEN(STR_CPU_FAN_VALUE), L"%d rpm", MonitorValues.CPUFanSpeed);
    InitString(HiiHandle, STRING_TOKEN(STR_PCH_DTS_TEMP_VALUE), L"%d C", MonitorValues.PCHDTSTemp);
  }
}

/**
  CallBack to update StateAfterG3 to EC while StateAfterG3 value changed

  @param  This                   Points to the EFI_HII_CONFIG_ACCESS_PROTOCOL.
  @param  Action                 Specifies the type of action taken by the browser.
  @param  KeyValue               A unique value which is sent to the original
                                 exporting driver so that it can identify the type
                                 of data to expect.
  @param  Type                   The type of value for the question.
  @param  Value                  A pointer to the data being sent to the original
                                 exporting driver.
  @param  ActionRequest          On return, points to the action requested by the
                                 callback function.

  @retval EFI_SUCCESS            The callback successfully handled the action.
  @retval EFI_OUT_OF_RESOURCES   Not enough storage is available to hold the
                                 variable and its data.
  @retval EFI_NOT_FOUND          The specified SetupData is not found.

**/
EFI_STATUS
EFIAPI
StateAfterG3CallBackFunction (
  IN CONST EFI_HII_CONFIG_ACCESS_PROTOCOL *This,
  IN EFI_BROWSER_ACTION                   Action,
  IN EFI_QUESTION_ID                      KeyValue,
  IN UINT8                                Type,
  IN EFI_IFR_TYPE_VALUE                   *Value,
  OUT EFI_BROWSER_ACTION_REQUEST          *ActionRequest
  )
{
  EFI_STATUS              Status;
  PCH_SETUP               PchSetup;
  UINTN                   DataSize;

  if (Action != EFI_BROWSER_ACTION_SUBMITTED) {
    return EFI_UNSUPPORTED;
  }

  DataSize = sizeof (PCH_SETUP);
  Status = gRT->GetVariable (
                  (CHAR16 *) L"PchSetup",
                  &gPchSetupVariableGuid,
                  (UINT32 *) NULL,
                  &DataSize,
                  &PchSetup
                  );
  if (EFI_ERROR (Status)) {
    ASSERT_EFI_ERROR (Status);
    return Status;
  }

  Status = EcSetG3State (PchSetup.StateAfterG3);
  return Status;
}

/**
  Callback function for setup key to switch the System monitors values

  @param HiiHandle              The HII Handle of the form to update.
  @param Class                  Indicates the setup class
  @param SubClass               Indicates setup Subclass
  @param Key                    Key of Setup token

**/
VOID HhmMobileCallBack (
  IN EFI_HII_HANDLE             HiiHandle,
  IN UINT16                     Class,
  IN UINT16                     SubClass,
  IN UINT16                     Key
  )
{
  MOBILE_SYSTEM_MONITOR_INFO    MonitorValues = { 0 };
  UINT16                        Value1;
  UINT16                        Value2;
  UINT8                         Index;
  UINT16                        KeyList[] = {
    THERMAL_SENSOR_1_KEY,
    THERMAL_SENSOR_2_KEY,
    THERMAL_SENSOR_3_KEY,
    THERMAL_SENSOR_4_KEY,
    THERMAL_SENSOR_5_KEY,
    CPU_FAN_KEY,
    PCH_DTS_TEMP_KEY,
  };

  if (Key != HHM_SETUP_REFRESH_KEY) {
    return;
  }

  if (PcdGetBool(PcdEcPresent) == FALSE) {
    return;
  }

  for (Index = 0; Index < sizeof (KeyList) / sizeof (UINT16); Index++) {
    Key = KeyList[Index];

    GetSystemMonitorValues (Key, &MonitorValues);

    switch(Key)
    {
      case THERMAL_SENSOR_1_KEY:
        Value1 = (UINT16)(MonitorValues.EC_TSR1 / 10);
        Value2 = (UINT16)(MonitorValues.EC_TSR1 % 10);
        InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_1_VALUE), L"%d.%01d C", Value1, Value2);
        break;

      case THERMAL_SENSOR_2_KEY:
        Value1 = (UINT16)(MonitorValues.EC_TSR2 / 10);
        Value2 = (UINT16)(MonitorValues.EC_TSR2 % 10);
        InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_2_VALUE), L"%d.%01d C", Value1, Value2);
        break;

      case THERMAL_SENSOR_3_KEY:
        Value1 = (UINT16)(MonitorValues.EC_TSR3 / 10);
        Value2 = (UINT16)(MonitorValues.EC_TSR3 % 10);
        InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_3_VALUE), L"%d.%01d C", Value1, Value2);
        break;

      case THERMAL_SENSOR_4_KEY:
        Value1 = (UINT16)(MonitorValues.EC_TSR4 / 10);
        Value2 = (UINT16)(MonitorValues.EC_TSR4 % 10);
        InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_4_VALUE), L"%d.%01d C", Value1, Value2);
        break;

      case THERMAL_SENSOR_5_KEY:
        Value1 = (UINT16)(MonitorValues.EC_TSR5 / 10);
        Value2 = (UINT16)(MonitorValues.EC_TSR5 % 10);
        InitString(HiiHandle, STRING_TOKEN(STR_THERMAL_SENSOR_5_VALUE), L"%d.%01d C", Value1, Value2);
        break;

      case CPU_FAN_KEY :
        InitString(HiiHandle, STRING_TOKEN(STR_CPU_FAN_VALUE), L"%d rpm", MonitorValues.CPUFanSpeed);
        break;

      case PCH_DTS_TEMP_KEY :
        InitString(HiiHandle, STRING_TOKEN(STR_PCH_DTS_TEMP_VALUE), L"%d C", MonitorValues.PCHDTSTemp);
        break;
    }
  }
}
