/*
 TCSS ACPI NVS infomation dump function library.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2024 Intel Corporation.

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
*/
#include <Protocol/TcssNvsArea.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

/**

  This function get ACPI TCSS variable from TCSS NVS protocol and print data.

  @retval EFI_SUCCESS               Print the TCSS NVS data successfully.
  @retval EFI_NOT_FOUND             Not found the statistics information.

**/
EFI_STATUS
EFIAPI
DumpTcssVariable (
  VOID
  )
{
  EFI_STATUS                     Status;
  TCSS_NVS_AREA_PROTOCOL         *TcssNvsAreaProtocol;

  TcssNvsAreaProtocol = NULL;
  Status = EFI_SUCCESS;

  Status = gBS->LocateProtocol (
                  &gTcssNvsAreaProtocolGuid,
                  NULL,
                  (VOID **) &TcssNvsAreaProtocol
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "The gTcssNvsAreaProtocol can't be located!!\n"));
    return Status;
  }
  //
  // Dumping TCSS NVS Area operation region.
  //
  DEBUG ((DEBUG_INFO, "[Dumping TCSS NVS Area operation regio - Start]\n"));
  DEBUG ((DEBUG_INFO, "ItbtXhciEn                   : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtXhciEn));
  DEBUG ((DEBUG_INFO, "ItbtDmaEn [0]                : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtDmaEn[0]));
  DEBUG ((DEBUG_INFO, "ItbtDmaEn [1]                : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtDmaEn[1]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpEn [0]             : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpEn[0]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpEn [1]             : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpEn[1]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpEn [2]             : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpEn[2]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpEn [3]             : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpEn[3]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpAddress [0]        : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpAddress[0]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpAddress [1]        : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpAddress[1]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpAddress [2]        : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpAddress[2]));
  DEBUG ((DEBUG_INFO, "ItbtPcieRpAddress [3]        : 0x%X\n", TcssNvsAreaProtocol->Area->ItbtPcieRpAddress[3]));
  DEBUG ((DEBUG_INFO, "IomReady                     : 0x%X\n", TcssNvsAreaProtocol->Area->IomReady));
  DEBUG ((DEBUG_INFO, "TcssRtd3                     : 0x%X\n", TcssNvsAreaProtocol->Area->TcssRtd3));
  DEBUG ((DEBUG_INFO, "LtrEnable [0]                : 0x%X\n", TcssNvsAreaProtocol->Area->LtrEnable[0]));
  DEBUG ((DEBUG_INFO, "LtrEnable [1]                : 0x%X\n", TcssNvsAreaProtocol->Area->LtrEnable[1]));
  DEBUG ((DEBUG_INFO, "LtrEnable [2]                : 0x%X\n", TcssNvsAreaProtocol->Area->LtrEnable[2]));
  DEBUG ((DEBUG_INFO, "LtrEnable [3]                : 0x%X\n", TcssNvsAreaProtocol->Area->LtrEnable[3]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxSnoopLatency [0]   : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxSnoopLatency[0]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxSnoopLatency [1]   : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxSnoopLatency[1]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxSnoopLatency [2]   : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxSnoopLatency[2]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxSnoopLatency [3]   : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxSnoopLatency[3]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxNoSnoopLatency [0] : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxNoSnoopLatency[0]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxNoSnoopLatency [1] : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxNoSnoopLatency[1]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxNoSnoopLatency [2] : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxNoSnoopLatency[2]));
  DEBUG ((DEBUG_INFO, "PcieLtrMaxNoSnoopLatency [3] : 0x%X\n", TcssNvsAreaProtocol->Area->PcieLtrMaxNoSnoopLatency[3]));
  DEBUG ((DEBUG_INFO, "IomReady                     : 0x%X\n", TcssNvsAreaProtocol->Area->IomReady));
  DEBUG ((DEBUG_INFO, "TcssIomVccSt                 : 0x%X\n", TcssNvsAreaProtocol->Area->TcssIomVccSt));
  DEBUG ((DEBUG_INFO, "RegBarOffset                 : 0x%X\n", TcssNvsAreaProtocol->Area->RegBarOffset));
  DEBUG ((DEBUG_INFO, "IomBase                      : 0x%X\n", TcssNvsAreaProtocol->Area->IomBase));
  DEBUG ((DEBUG_INFO, "RetimerMap                   : 0x%X\n", TcssNvsAreaProtocol->Area->RetimerMap));

  DEBUG ((DEBUG_INFO, "[Dumping TCSS NVS Area operation regio - End]\n"));

  return Status;
}
