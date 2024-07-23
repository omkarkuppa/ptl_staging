/** @file
  Header file for DPTF Config Variable

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
#ifndef _DPTF_CONFIG_H_
#define _DPTF_CONFIG_H_

#define DPTF_CONFIG_VARIABLE_NAME  L"DptfConfig"

#pragma pack (1)
/**
 Making any setup structure change after code frozen
 will need to maintain backward compatibility, bump up
 structure revision and update below history table\n
  <b>Revision 1</b>:  - Initial version.
  <b>Revision 2</b>:  - Deprecated TmpObject's as they were obsolete/unused
**/

#define DPTF_CONFIG_REVISION       2

typedef struct {
  UINT8   Revision;
  UINT8   EnableDptf; ///< Enable Dptf: <b>0: Disable</b>; 1: Enable
  UINT32  EnableDCFG;
  UINT32  PpccStepSize;
  UINT8   EnableFan1Device;
  UINT8   EnableFan2Device;
  UINT8   EnableFan3Device;
  UINT8   EnableChargerParticipant;
  UINT8   EnablePowerParticipant;
  UINT16  PowerParticipantPollingRate;
  UINT8   EnableBatteryParticipant;
  UINT8   EnableSen1ParticipantSupport;
  UINT8   EnableSen1Participant;
  UINT8   EnableSen2Participant;
  UINT8   EnableSen3Participant;
  UINT8   EnableSen4Participant;
  UINT8   EnableSen5Participant;
  UINT8   EnablePchFivrParticipant;
  UINT8   PpccObject;
  UINT8   ArtgObject;
  UINT8   PmaxObject;
  UINT8   OptionalObjects;
  UINT8   OemDesignVariable0;
  UINT8   OemDesignVariable1;
  UINT8   OemDesignVariable2;
  UINT8   OemDesignVariable3;
  UINT8   OemDesignVariable4;
  UINT8   OemDesignVariable5;
  UINT8   EnableFanAutoMode;
} DPTF_CONFIG;
#pragma pack ()

extern DPTF_CONFIG    mDptfSetupData;

#define DPTF_CONFIG_VARSTORE\
    efivarstore DPTF_CONFIG,\
        attribute = 0x7, name = DptfConfig, guid = gDptfConfigVariableGuid;

#endif // _DPTF_CONFIG_H_
