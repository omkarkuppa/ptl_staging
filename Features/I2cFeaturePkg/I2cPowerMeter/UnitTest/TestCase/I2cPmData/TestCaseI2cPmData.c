/**@file
  Unit test case of the I2cPmData function.

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

@par Specification
**/

#include "TestCaseI2cPmData.h"
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>

extern  EFI_ACPI_DESCRIPTION_HEADER *gIC2PmDataTable;
extern  POWER_METER_PKG              *PowerMtrData;
extern  UINT8                        PowerMeterEnable;

/**
  Unit Test case for I2cPmData function.

  @param[in] Context                   The pointer for the Unit Test context.

  @retval UNIT_TEST_PASSED             Pass the test case.
  @retval UNIT_TEST_ERROR_TEST_FAILED  Fail in test case.
**/
UNIT_TEST_STATUS
EFIAPI
TestCaseI2cPmData (
  IN UNIT_TEST_CONTEXT  Context
  )
{
  UINT8*                        Buffer;
  UINT8                         Index;
  UINT8*                        TablePtr;
  UINT8*                        TableEndPtr;
  CHAR8*                        RailNameStrPtr;
  UINT8*                        DataPtr;
  UINT8*                        PtrBuffer8;
  UINT16*                       PtrBuffer16;
  UINT32*                       Signature;
  UINT16                        Rsense;
  UINT8                         *Operation;
  UINT16                        DevCount;

  //
  // Initialize default variable.
  //
  TablePtr        = NULL;
  TableEndPtr     = NULL;
  RailNameStrPtr  = NULL;
  DataPtr         = NULL;
  PtrBuffer8      = NULL;
  PtrBuffer16     = NULL;

  //
  // Check the table ID to modify the table
  //
  if (gIC2PmDataTable->OemTableId == SIGNATURE_64 ('I', '2', 'P', 'm', '_', 'R', 'v', 'p')) {
    DEBUG ((DEBUG_ERROR, "I2cPm ACPI Table found.\n"));
    Buffer = (UINT8*) &gIC2PmDataTable->Signature;
    DEBUG ((DEBUG_ERROR, "Table Signature : %c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3]
      ));
    DEBUG ((DEBUG_ERROR, "Table Length : 0x%X\n", gIC2PmDataTable->Length));
    DEBUG ((DEBUG_ERROR, "Table Revision : 0x%X\n", gIC2PmDataTable->Revision));
    DEBUG ((DEBUG_ERROR, "Table Checksum : 0x%X\n", gIC2PmDataTable->Checksum));
    Buffer = (UINT8*) &gIC2PmDataTable->OemId;
    DEBUG ((DEBUG_ERROR, "Table OemId : %c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5]
      ));
    Buffer = (UINT8*) &gIC2PmDataTable->OemTableId;
    DEBUG ((DEBUG_ERROR, "Table OemTableId : %c%c%c%c%c%c%c%c\n",
      Buffer[0],
      Buffer[1],
      Buffer[2],
      Buffer[3],
      Buffer[4],
      Buffer[5],
      Buffer[6],
      Buffer[7]
      ));
    DEBUG ((DEBUG_ERROR, "Table OemRevision : 0x%X\n", gIC2PmDataTable->OemRevision));
    DEBUG ((DEBUG_ERROR, "Table CreatorId : 0x%X\n", gIC2PmDataTable->CreatorId));
    DEBUG ((DEBUG_ERROR, "Table CreatorRevision : 0x%X\n", gIC2PmDataTable->CreatorRevision));
  } else {
    DEBUG ((DEBUG_ERROR, "I2cPm ACPI Table isn't find.\n"));
    UT_ASSERT_FALSE (TRUE);
  }

  //
  // sync ACPI table's value
  //
  TablePtr = (UINT8*) gIC2PmDataTable;
  TableEndPtr = TablePtr + gIC2PmDataTable->Length;

  while (TablePtr < TableEndPtr) {
    TablePtr++;
    Signature = (UINT32 *)TablePtr;

    switch (*Signature) {
      case (SIGNATURE_32 ('_', 'S', 'B', '_')):
        ///
        /// If we find the _SB_PC00I2C3 scope, save a pointer to the package length
        ///
        Operation = TablePtr - 7;
        DataPtr = TablePtr + 11;
        if ((*Operation == AML_SCOPE_OP) &&
            (*(Signature) == SIGNATURE_32 ('_', 'S', 'B', '_')) &&
            (*(Signature + 1) == SIGNATURE_32 ('P', 'C', '0', '0')) &&
            (*(Signature + 2) == (UINT32) SIGNATURE_32 ('I', '2', 'C', PowerMtrData->I2cBusNumber))
            ) {
          PtrBuffer8 = DataPtr;
          if (*PtrBuffer8 != PowerMtrData->I2cBusNumber) {
            DEBUG ((DEBUG_ERROR, "I2C bus number is not expected!!, [I2Cx] = 0x%x\n", *PtrBuffer8));
            UT_ASSERT_FALSE (TRUE);
          }
        }
        break;

      case (SIGNATURE_32 ('P','O','M','E')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 5;
        if (*Operation == AML_NAME_OP) {
          PtrBuffer8 = DataPtr;
          if (*PtrBuffer8 != PowerMeterEnable) {
            DEBUG ((DEBUG_ERROR, "POME value is not expected!!, [POME] = 0x%x\n", *PtrBuffer8));
            UT_ASSERT_FALSE (TRUE);
          }
        }
        break;

      case (SIGNATURE_32 ('P','A','C','T')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 5;
        if (*Operation == AML_NAME_OP) {
          PtrBuffer8 = DataPtr;
          if (*PtrBuffer8 != PowerMtrData->DeviceSize) {
            DEBUG ((DEBUG_ERROR, "PACT value is not expected!!, [PACT] = 0x%x\n", *PtrBuffer8));
            UT_ASSERT_FALSE (TRUE);
          }
        }
        break;

      case (SIGNATURE_32 ('P','A','B','N')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 5;
        if (*Operation == AML_NAME_OP) {
          PtrBuffer8 = DataPtr;
          if (*PtrBuffer8 != PowerMtrData->I2cBusNumber) {
            DEBUG ((DEBUG_ERROR, "PABN value is not expected!!, [PABN] = 0x%x\n", *PtrBuffer8));
            UT_ASSERT_FALSE (TRUE);
          }
        }
        break;

      case (SIGNATURE_32 ('P','A','S','1')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 8;
        if (*Operation == AML_NAME_OP) {
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          for (DevCount = 0; DevCount < PowerMtrData->DeviceSize; DevCount++) {
            PtrBuffer8 = DataPtr;
            if (*PtrBuffer8 != PowerMtrData->PmConfigPkgArray[DevCount].DeviceAddress) {
              DEBUG ((DEBUG_ERROR, "PAS1 value is not expected!!, [PAS1] = 0x%x\n", *PtrBuffer8));
              UT_ASSERT_FALSE (TRUE);
            }
            DataPtr = (DataPtr + 2);
          }
        }
        break;

      case (SIGNATURE_32 ('P','M','P','0')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 13;
        if (*Operation == AML_NAME_OP) {
          //
          // Conditional match.  For Name Objects, the Operator will always be the byte
          // immediately before the specific name. Therefore, subtract 1 to check the
          // Operator.
          //
          for (DevCount = 0; DevCount < PowerMtrData->DeviceSize; DevCount++) {
            for (Index = 0; Index < 4; Index++) {
              RailNameStrPtr = PowerMtrData->PmConfigPkgArray[DevCount].RailDataPkg[Index].RailName;
              if ((CompareMem (DataPtr, RailNameStrPtr, AsciiStrLen (RailNameStrPtr))) != 0) {
                DEBUG ((DEBUG_ERROR, "PMP0 string is not expected!!, [PMP0] Dev %u Channel %u = %a\n", DevCount, Index, DataPtr));
                UT_ASSERT_FALSE (TRUE);
              }
              DataPtr = (DataPtr + MAX_PM_DEVICE_STRING_LENGTH + 2);
            }
            DataPtr = (DataPtr+4);
          }
        }
        break;

      case (SIGNATURE_32 ('P','M','P','1')):
        //
        // Conditional match.  For Name Objects, the Operator will always be the byte
        // immediately before the specific name. Therefore, subtract 1 to check the
        // Operator.
        //
        Operation = TablePtr - 1;
        DataPtr = TablePtr + 12;
        if (*Operation == AML_NAME_OP) {
          PtrBuffer16 = (UINT16*)(DataPtr);
          //
          // To keep the PMAX table in fixed size, fill the vacancy of the string buffer with AML NO OP.
          // This will makes PMAX table shows numerous "NoOp" after device name.
          //
          for ( DevCount = 0; DevCount < (PowerMtrData->DeviceSize); DevCount++)
          {
            for (Index=0; Index<4; Index++) {
              PtrBuffer16 = (UINT16*) (DataPtr);
              Rsense = PowerMtrData->PmConfigPkgArray[DevCount].RailDataPkg[Index].ResistorValue * 1000;
              if (*PtrBuffer16 != Rsense) {
                DEBUG ((DEBUG_ERROR, "PMP1 value is not expected!!, [PMP1] Rsense = 0x%x\n", *PtrBuffer16));
                UT_ASSERT_FALSE (TRUE);
              }
              DataPtr = (DataPtr+3);
            }
            DataPtr = (DataPtr+3);
          }
        }
        break;

      default:
        break;
    }
  }

  return UNIT_TEST_PASSED;
}
