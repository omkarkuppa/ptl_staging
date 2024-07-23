/** @file
  This module configures the memory controller power modes.

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

///
/// Include files
///
#include "CMrcTypes.h"
#include "MrcCommon.h"
#include "MrcGeneral.h"
#include "MrcPowerMetering.h"
#include "MrcDebugPrint.h"

#define MAX_FREQ  7

MrcPowerWeightMapping DdrPhyPowerTableLp5[MAX_FREQ] = {
// Freq    ReadPower  WritePower  IdlePower     CkePower
//         (fj/byte)   (fj/byte)  (microWatts) (microWatts)
  {f2400,  {21700,     24500,       442500,       304000}},
  {f3200,  {20700,     23000,       480300,       312100}},
  {f4800,  {18700,     20200,       555900,       328200}},
  {f6400,  {15200,     15000,       637800,       348500}},
  {f7200,  {15700,     15900,       669200,       352400}},
  {f8533,  {14000,     13500,       732200,       365800}},
  {f9600,  {15100,     14000,       834100,       396500}}
};

MrcPowerWeightMapping DdrPhyPowerTableDdr5[MAX_FREQ] = {
// Freq    ReadPower  WritePower  IdlePower     CkePower
//         (fj/byte)   (fj/byte)  (microWatts) (microWatts)
  {f3200,  {37600,     14000,       241000,       202900}},
  {f4200,  {34600,     14000,       286900,       235100}},
  {f6000,  {29100,     13800,       369400,       293000}},
  {f6400,  {27900,     13800,       387700,       305900}},
  {f6800,  {26700,     13800,       406100,       318700}},
  {f7200,  {25500,     13800,       424400,       331600}},
  {f8800,  {22900,     13600,       493200,       379800}}
};

/**
  This function calculates power weights at non-POR frequencies using linear interpolation.

  @param[in]  MrcData     - Include all MRC global data.
  @param[in]  Freq        - Non-POR Frequency.
  @param[out] ReadPower   - DDRPHY Read energy weight.
  @param[out] WritePower  - DDRPHY Write energy weight.
  @param[out] IdlePower   - DDRPHY Idle energy weight.
  @param[out] CkePower    - DDRPHY Cke energy weight.

  @retval Nothing.
**/
static VOID CaculateDdrPhyPower (
  IN  MrcParameters   *const MrcData,
  IN  MrcFrequency           Freq,
  OUT UINT32          *const ReadPower,
  OUT UINT32          *const WritePower,
  OUT UINT32          *const IdlePower,
  OUT UINT32          *const CkePower
  )
{
  INT32 x1, x2, y1, y2;
  UINT32 Index1, Index2;
  BOOLEAN IsDdr5;
  MrcOutput *Outputs;
  MrcPowerWeightMapping *DdrPhyPowerTable;

  Outputs = &MrcData->Outputs;
  IsDdr5 = Outputs->IsDdr5;
  DdrPhyPowerTable = (IsDdr5) ? DdrPhyPowerTableDdr5 : DdrPhyPowerTableLp5;

  if (DdrPhyPowerTable[0].Freq > Freq) {
    Index1 = 0;
    Index2 = 1;
  } else {
    for (Index1 = 0; Index1 < MAX_FREQ - 2; Index1++) {
      if (DdrPhyPowerTable[Index1].Freq < Freq && DdrPhyPowerTable[Index1 + 1].Freq > Freq)
        break;
    }
    Index2 = Index1 + 1;
  }

  x1 = DdrPhyPowerTable[Index1].PowerValue.ReadPwr;
  x2 = DdrPhyPowerTable[Index2].PowerValue.ReadPwr;
  y1 = DdrPhyPowerTable[Index1].Freq;
  y2 = DdrPhyPowerTable[Index2].Freq;
  *ReadPower = (UINT32)(x2 + (INT32)((Freq - y2) * (x1 - x2)) / (y1 - y2));

  x1 = DdrPhyPowerTable[Index1].PowerValue.WritePwr;
  x2 = DdrPhyPowerTable[Index2].PowerValue.WritePwr;
  *WritePower = (UINT32)(x2 + (INT32)((Freq - y2) * (x1 - x2)) / (y1 - y2));

  x1 = DdrPhyPowerTable[Index1].PowerValue.IdlePwr;
  x2 = DdrPhyPowerTable[Index2].PowerValue.IdlePwr;
  *IdlePower = (UINT32)(x2 + (INT32)((Freq - y2) * (x1 - x2)) / (y1 - y2));

  x1 = DdrPhyPowerTable[Index1].PowerValue.CkePwr;
  x2 = DdrPhyPowerTable[Index2].PowerValue.CkePwr;
  *CkePower = (UINT32)(x2 + (INT32)((Freq - y2) * (x1 - x2)) / (y1 - y2));

  return;
}

/**
  This function calculates the Power Metering weights.

  @param[in]  MrcData     - Include all MRC global data.
  @param[out] ReadPower   - DDRPHY Read energy weight.
  @param[out] WritePower  - DDRPHY Write energy weight.
  @param[out] IdlePower   - DDRPHY Idle energy weight.
  @param[out] CkePower    - DDRPHY Cke energy weight.

  @retval Nothing.
**/
VOID MrcGetDdrPhyWeights (
  IN  MrcParameters   *const MrcData,
  OUT UINT32          *const ReadPower,
  OUT UINT32          *const WritePower,
  OUT UINT32          *const IdlePower,
  OUT UINT32          *const CkePower
  )
{
  MrcOutput            *Outputs;
  MrcFrequency          Freq;
  UINT32                Count;
  BOOLEAN               Found;
  MrcPowerWeightMapping *PowerMeasurePtr;
  BOOLEAN               IsDdr5;
  MrcPowerWeightMapping *DdrPhyPowerTable;

  Outputs           = &MrcData->Outputs;
  Freq              = Outputs->Frequency;
  Found             = FALSE;
  IsDdr5            = Outputs->IsDdr5;
  DdrPhyPowerTable  = (IsDdr5) ? DdrPhyPowerTableDdr5 : DdrPhyPowerTableLp5;

  for (Count = 0; Count < MAX_FREQ; Count++) {
    PowerMeasurePtr = &DdrPhyPowerTable[Count];
    if(Freq == PowerMeasurePtr->Freq) {
      *ReadPower = PowerMeasurePtr->PowerValue.ReadPwr;
      *WritePower = PowerMeasurePtr->PowerValue.WritePwr;
      *IdlePower = PowerMeasurePtr->PowerValue.IdlePwr;
      *CkePower = PowerMeasurePtr->PowerValue.CkePwr;
      Found = TRUE;
    }
  }
  //If don't found the value on table, use linear interpolation to calculate those.
  if (!Found) {
    CaculateDdrPhyPower (MrcData, Freq, ReadPower, WritePower, IdlePower, CkePower);
  }
  *ReadPower = UDIVIDEROUND(*ReadPower, 200); //Read power in pJ per Byte in units of 0.2pJ/byte
  *WritePower = UDIVIDEROUND(*WritePower, 200); //Write power in pJ per Byte in units of 0.2pJ/byte
  *IdlePower = UDIVIDEROUND(*IdlePower, 4000); //Active idle (LPMode0 and LPMode0.5) power in units of 4mW.
  *CkePower = UDIVIDEROUND(*CkePower, 4000); //CKE (LPMode2) power in units of 4mW.

  return;
}



