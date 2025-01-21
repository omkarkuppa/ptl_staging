/** @file
  Main implementation source file for the support of caching the "Setup" variable.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2014 Intel Corporation.

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

#include <Library/DebugLib.h>
#include <Library/IoLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PostCodeMapLib.h>
#include <Ppi/ReportStatusCodeHandler.h>
#include <Library/PeiServicesLib.h>
#include <Library/TimerLib.h>
#include <PlatformNvRamHookLib.h>
#include <Library/CpuPlatformLib.h>
#include <Library/CpuMailboxLib.h>
#include <Library/PeiHostBridgeIpStatusLib.h>
#define NCDECS_CR_SCRATCHPAD_NCU_0_REG 0x5428
#define NCDECS_CR_SCRATCHPAD_NCU_2_REG 0x542C

/**
  Convert status code value and GUID,  write data to Scratch register.

  @param[in]  PeiServices  An indirect pointer to the EFI_PEI_SERVICES table published by the PEI Foundation.
  @param[in]  CodeType     Indicates the type of status code being reported.
  @param[in]  Value        Describes the current status of a hardware or
                           software entity. This includes information about the class and
                           subclass that is used to classify the entity as well as an operation.
                           For progress codes, the operation is the current activity.
                           For error codes, it is the exception.For debug codes,it is not defined at this time.
  @param[in]  Instance     The enumeration of a hardware or software entity within
                           the system. A system may contain multiple entities that match a class/subclass
                           pairing. The instance differentiates between them. An instance of 0 indicates
                           that instance information is unavailable, not meaningful, or not relevant.
                           Valid instance numbers start with 1.
  @param[in]  CallerId     This optional parameter may be used to identify the caller.
                           This parameter allows the status code driver to apply different rules to
                           different callers.
  @param[in]  Data         This optional parameter may be used to pass additional data.

  @retval EFI_SUCCESS      Status code reported to port code successfully.

**/
EFI_STATUS
EFIAPI
GuidForwardWorker (
  IN CONST  EFI_PEI_SERVICES        **PeiServices,
  IN EFI_STATUS_CODE_TYPE           CodeType,
  IN EFI_STATUS_CODE_VALUE          Value,
  IN UINT32                         Instance,
  IN CONST EFI_GUID                 *CallerId,
  IN CONST EFI_STATUS_CODE_DATA     *Data OPTIONAL
  )
{
  UINT32           PostCodeValue;
  UINT8            Port80;
  UINT8            Port81;
  EFI_STATUS       Status;
  UINT64           MchBar;
  UINT64           *ScratchPad;
  EFI_FV_FILE_INFO FileInfo;

  MchBar = GetHostBridgeRegisterData (HostBridgeCfgReg, MchBarCfgBase);

  PostCodeValue = GetPostCodeFromStatusCode (CodeType, Value);
  if (PostCodeValue != 0) {
    DEBUG ((DEBUG_INFO, "Port80 forward SMM POSTCODE=<%02x>\n", PostCodeValue));

    ///
    /// Saving Post Code value to Scratch Pad
    ///
    ScratchPad  = (UINT64 *) (UINTN) (MchBar + NCDECS_CR_SCRATCHPAD_NCU_0_REG);
    *ScratchPad = PostCodeValue;
  }

  do {
    GetPostCodeBreak( &Port80, &Port81 );
    if (Port80 != DEFAULT_VALUE || Port81 != DEFAULT_VALUE) {
      MicroSecondDelay (100000);
    }
  } while ((PostCodeValue != 0) && ((UINT32)(Port80 | (Port81 << 8)) == PostCodeValue));

  if ((CodeType==EFI_PROGRESS_CODE)&&(Value==(EFI_SOFTWARE_PEI_CORE | EFI_SW_PC_INIT_BEGIN))&&(Data != NULL)) {
      ScratchPad  = (UINT64 *)((UINTN) (Data) + Data->HeaderSize);
      Status = PeiServicesFfsGetFileInfo (*((EFI_PEI_FILE_HANDLE *)ScratchPad), &FileInfo);
      if (!EFI_ERROR (Status)) {
        DEBUG ((DEBUG_INFO,"Guid Forward PEI: %g\n", &FileInfo.FileName));
        ScratchPad  = (UINT64 *) (UINTN) (MchBar + NCDECS_CR_SCRATCHPAD_NCU_2_REG);
        *ScratchPad = FileInfo.FileName.Data1;
      }
  }

  return EFI_SUCCESS;
}


/**
  Entry point of Guid Forward PEIM Library.

  This function is the entry point of this Status Code PEIM.

  @retval EFI_SUCCESS  The entry point of DXE IPL PEIM executes successfully.

**/
EFI_STATUS
EFIAPI
PeiGuidForward (
  VOID
  )
{
  EFI_STATUS                Status;
  EFI_PEI_RSC_HANDLER_PPI   *RscHandlerPpi;

  Status = PeiServicesLocatePpi (
             &gEfiPeiRscHandlerPpiGuid,
             0,
             NULL,
             (VOID **) &RscHandlerPpi
             );

  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = RscHandlerPpi->Register (GuidForwardWorker);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  return EFI_SUCCESS;
}
