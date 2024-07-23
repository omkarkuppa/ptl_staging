/** @file
  A wrapper library consuming Trace Hub Debug library in Edk2.

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

#include <Base.h>
#include <Library/TraceHubDebugSysTLib.h>


/**
  Write Statuscode to NPK with GUID information.

  This API would be enabled for Release BIOS Only

  @param  SeverityType     Severity type of the written data buffer.
  @param  Id               Catalog ID
  @param  Guid             Driver Guid

**/
VOID
EFIAPI
TraceHubWriteCataLog64StatusCode (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN GUID                       *Guid
  )
{
  TraceHubSysTWriteCataLog64StatusCode (
    SeverityType,
    Id,
    Guid
    );
}

/**
  Write data from buffer and Severity to TraceHub.

  Writes NumberOfBytes data bytes from Buffer and Severity to TraceHub.
  If Buffer is NULL and NumberOfBytes is larger than zero, then ASSERT().
  If NumberOfBytes is zero, no buffer is written.

  @param  SeverityType     Severity type of the written data buffer.
  @param  Buffer           Pointer to the data buffer to be written.
  @param  NumberOfBytes    Number of bytes to written to TraceHub device.

  @retval RETURN_SUCCESS   The number of bytes written to TraceHub.
  @retval Other            Fail to write the buffer to TraceHub.

**/
RETURN_STATUS
EFIAPI
TraceHubDebugWrite (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT8                      *Buffer,
  IN UINTN                      NumberOfBytes
  )
{
  return TraceHubSysTDebugWrite (
           SeverityType,
           Buffer,
           NumberOfBytes
           );
}

/**
  Write catalog64 message mipi_syst event with 0 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID

**/
VOID
EFIAPI
TraceHubWriteCataLog64_0 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    0
    );
}

/**
  Write catalog64 message mipi_syst event with 1 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_1 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    1,
    Param1
    );
}

/**
  Write catalog64 message mipi_syst event with 6 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_2 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    2,
    Param1,
    Param2
    );
}

/**
  Write catalog64 message mipi_syst event with 3 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.
  @param  Param3         Parameter 3 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_3 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2,
  IN UINT32                     Param3
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    3,
    Param1,
    Param2,
    Param3
    );
}

/**
  Write catalog64 message mipi_syst event with 4 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.
  @param  Param3         Parameter 3 passed to catid message event.
  @param  Param4         Parameter 4 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_4 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2,
  IN UINT32                     Param3,
  IN UINT32                     Param4
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    4,
    Param1,
    Param2,
    Param3,
    Param4
    );
}

/**
  Write catalog64 message mipi_syst event with 5 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.
  @param  Param3         Parameter 3 passed to catid message event.
  @param  Param4         Parameter 4 passed to catid message event.
  @param  Param5         Parameter 5 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_5 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2,
  IN UINT32                     Param3,
  IN UINT32                     Param4,
  IN UINT32                     Param5
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    5,
    Param1,
    Param2,
    Param3,
    Param4,
    Param5
    );
}

/**
  Write catalog64 message mipi_syst event with 6 parameters to TraceHub

  @param  SeverityType   Severity type of the catid message event.
  @param  Id             Catalog ID
  @param  Param1         Parameter 1 passed to catid message event.
  @param  Param2         Parameter 2 passed to catid message event.
  @param  Param3         Parameter 3 passed to catid message event.
  @param  Param4         Parameter 4 passed to catid message event.
  @param  Param5         Parameter 5 passed to catid message event.
  @param  Param6         Parameter 6 passed to catid message event.

**/
VOID
EFIAPI
TraceHubWriteCataLog64_6 (
  IN TRACE_HUB_SEVERITY_TYPE    SeverityType,
  IN UINT64                     Id,
  IN UINT32                     Param1,
  IN UINT32                     Param2,
  IN UINT32                     Param3,
  IN UINT32                     Param4,
  IN UINT32                     Param5,
  IN UINT32                     Param6
  )
{
  TraceHubSysTWriteCataLog64 (
    SeverityType,
    Id,
    6,
    Param1,
    Param2,
    Param3,
    Param4,
    Param5,
    Param6
    );
}

