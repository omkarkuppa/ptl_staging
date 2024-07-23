/** @file
  SMM IO/MSR bitmap support functions

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2022 Intel Corporation.

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

#include "DgrSmm.h"
#include "SmmResourceConfig.h"

#define HIGH_MSR_BASE              0xC0000000
#define IO_BITMAP_BUFFER_LENGTH    0x2000
#define MSR_BITMAP_BUFFER_LENGTH   0x1000
#define TSS_SEGMENT                0x70
#define TSS_IOMAP_OFFSET           102

EFI_HANDLE  mSmmDgrResourceProtectHandle = NULL;
BOOLEAN     mSmmDgrLockSmmResourceControl = FALSE;
UINTN       *mSmmDgrIoBitMapBase;
BOOLEAN     mSmmDgrResourceConfigApiInitialized = FALSE;

extern UINTN  gSmmDgrMsrBitMapBase;

UINT32  mMsrIndex[] = {
  MSR_IA32_SYSENTER_CS,
  MSR_IA32_SYSENTER_ESP,
  MSR_IA32_SYSENTER_EIP,
  MSR_IA32_STAR,
  MSR_IA32_LSTAR,
  MSR_IA32_FMASK,
  MSR_IA32_PERF_GLOBAL_CTRL,
};

//
// SMM Resource (IO, MSR) Config protocol Interfaces.
//
GLOBAL_REMOVE_IF_UNREFERENCED
SMM_RESOURCE_CONFIG_PROTOCOL mSmmResourceConfigProtocol = {
  ConfigIoBitmap,
  ConfigMsrBitmap,
  InstallIoBitmap,
  InstallMsrBitmap
};

/**
  Allocate pages for code.

  @param[in]  Pages Number of pages to be allocated.

  @return Allocated memory.
**/
VOID *
AllocateCodePages (
  IN UINTN           Pages
  );

/**
  SMM End Of Dxe event notification handler.

  Setting Global variable to notify End Of Dxe Event to control SMM Resource access

  @param[in] Protocol   Points to the protocol's unique identifier
  @param[in] Interface  Points to the interface instance
  @param[in] Handle     The handle on which the interface was installed

  @retval EFI_SUCCESS   Notification handler runs successfully.
**/
EFI_STATUS
EFIAPI
SpsSmmEndOfDxeEventNotify (
  IN CONST EFI_GUID  *Protocol,
  IN VOID            *Interface,
  IN EFI_HANDLE      Handle
  )
{
  DEBUG ((DEBUG_INFO, "SpsSmmEndOfDxeEventNotify\n"));

  mSmmDgrLockSmmResourceControl = TRUE;

  return EFI_SUCCESS;
}

/**
  This function installs protocol interface for IO, MSR Configuration.

  @retval EFI_SUCCESS   Successfully installed SMM Resource Config APIs.
**/
EFI_STATUS
EFIAPI
SpsSmmResourceConfigApiInit (
  VOID
  )
{
  EFI_STATUS  Status;
  VOID        *Registration;

  //
  // Install Protocol for SMM Resource config APIs
  //
  Status = gSmst->SmmInstallProtocolInterface (
    &mSmmDgrResourceProtectHandle,
    &gSmmResourceConfigProtocolGuid,
    EFI_NATIVE_INTERFACE,
    &mSmmResourceConfigProtocol
  );
  ASSERT_EFI_ERROR(Status);

  //
  // Register SMM End of DXE Event
  //
  Status = gSmst->SmmRegisterProtocolNotify (
    &gEfiSmmEndOfDxeProtocolGuid,
    SpsSmmEndOfDxeEventNotify,
    &Registration
  );
  ASSERT_EFI_ERROR(Status);

  return Status;
}

/**
  Sub function for Configuring IO Port access for selected CPU.

  @param[in] IoBitMapBase      IO Bit map base address
  @param[in] Port              Port address
  @param[in] DenyRwAccess  TRUE = Deny access to port; FALSE = Allow access.
**/
VOID
InternalProtectIo (
  IN UINTN   IoBitMapBase,
  IN UINT16  Port,
  IN BOOLEAN DenyRwAccess
  )
{
  UINT8 *IoBitmap;
  UINTN Index;
  UINTN Offset;

  if (Port >= 0x8000) {
    IoBitmap = (UINT8 *) (UINTN) (IoBitMapBase + SIZE_4KB);
    Port -= 0x8000;
  } else {
    IoBitmap = (UINT8 *) (UINTN) (IoBitMapBase);
  }

  Index = Port / 8;
  Offset = Port % 8;

  if (DenyRwAccess) {
    IoBitmap[Index] |= (UINT8)(1 << Offset);
  } else {
    IoBitmap[Index] &= (UINT8)~(1 << Offset);
  }

  return;
}

/**
  This function configures IO port access through bitmap for all CPUs within SMM.
  ie Using this API, IO port can be configured as read write access or restricted access
  within SMM.
  For example: To revoke access to IO port 0xCF8 to 0xCFB, invoke this API with
  PortBase = 0xCF8, PortLength = 4 and DenyRwAccess = TRUE

  @param[in] PortBase       IO Port base address.
  @param[in] PortLength     Length of IO Port bitmap that needs access configuration.
  @param[in] DenyRwAccess   0 = Allow RW Access to port; 1 = Deny RW Access to port.

  @retval      EFI_SUCCESS  Successfully enabled protection for specified IO range.
**/
EFI_STATUS
EFIAPI
ConfigIoBitmap (
  IN UINT16  PortBase,
  IN UINT16  PortLength,
  IN BOOLEAN DenyRwAccess
  )
{
  UINTN  CpuIndex;
  UINTN  Index;

  if (mSmmDgrLockSmmResourceControl) {
    return EFI_ACCESS_DENIED;
  }

  for (CpuIndex = 0; CpuIndex < gSmst->NumberOfCpus; CpuIndex++) {
    for (Index = 0; Index < PortLength; Index++) {
      InternalProtectIo (mSmmDgrIoBitMapBase[CpuIndex], (UINT16)(PortBase + Index), DenyRwAccess);
    }
  }

  return EFI_SUCCESS;
}

/**
  This function configures MSR access through bitmap for all CPUs within SMM.
  Using this API, any MSR can be configured as Read only, Write only, Read/Write
  or no access with in SMM.

  For example:
    To configure MSR 0xE2, Invoke this API with MSR = 0xE2 and

      Case1: To revoke both read and write access
        DenyReadAccess = TRUE, DenyWriteAccess = TRUE.

      Case2: To revoke read access and allow Write access
        DenyReadAccess = TRUE, DenyWriteAccess = FALSE.

      Case3: To allow read access and revoke write access
        DenyReadAccess = FALSE, DenyWriteAccess = TRUE.

      Case4: To allow both read and Write access
        DenyReadAccess = FALSE, DenyWriteAccess = FALSE.

  DGR reference code allows access for only specified MSRs and denies
  access to rest of all MSRs. Refer mAllowedListMsr table structure at
  Intel\<Platform Sample Pkg>\Features\NiftyRock\PpamPlatformSmm\SmmIoMsrAccess.h for
  list of MSRs allowed for R/W access.

  OEM platform SMM driver can use this API to modify access to any MSR before
  SMM End of Dxe event (gEfiSmmEndOfDxeProtocolGuid).

  @param[in]  Msr          MSR base address.
  @param[in]  DenyReadAccess   FALSE - Allow read access, TRUE - Deny read access to MSR.
  @param[in]  DenyWriteAccess  FALSE - Allow write access, TRUE - Deny write access to MSR.

  @retval  EFI_SUCCESS            MSR Bitmap has been successfully configured.
  @retval  EFI_INVALID_PARAMETER  If MSR is not in the range of 0 to 0x1FFF and
                                  0xC0000000 to 0xC0001FFF.
  @retval  EFI_ACCESS_DENIED      If this protocol is invoked after SMM End of Dxe event.
**/
EFI_STATUS
EFIAPI
ConfigMsrBitmap (
  IN UINT32  Msr,
  IN BOOLEAN DenyReadAccess,
  IN BOOLEAN DenyWriteAccess
  )
{
  UINT8 *MsrBitmap;
  UINTN Index;
  UINTN Offset;

  if (mSmmDgrLockSmmResourceControl) {
    return EFI_ACCESS_DENIED;
  }

  MsrBitmap = (UINT8 *) (UINTN) (gSmmDgrMsrBitMapBase);
  if (Msr >= HIGH_MSR_BASE) {
    Msr = Msr - HIGH_MSR_BASE + 0x2000;
  }

  //
  // Configure MSR Read access
  //
  Index = Msr / 8;
  Offset = Msr % 8;

  if (DenyReadAccess) {
    MsrBitmap[Index] |= (UINT8)(1 << Offset);
  }
  else {
    MsrBitmap[Index] &= (UINT8)~(1 << Offset);
  }

//
// Configure MSR Write access
//
  Msr = Msr + 0x4000;
  Index = Msr / 8;
  Offset = Msr % 8;

  if (DenyWriteAccess) {
    MsrBitmap[Index] |= (UINT8)(1 << Offset);
  }
  else {
    MsrBitmap[Index] &= (UINT8)~(1 << Offset);
  }

  return EFI_SUCCESS;
}

/**
  This function allows a caller to install new IO Bitmap on top of the
  default IO Bitmap.

  @param[in] Buffer          Buffer address of IO Bitmap.
  @param[in] BufferLength    Length of the buffer. This should be 0x2000.

  @retval  EFI_SUCCESS            IO bitmap has been successfully installed.
  @retval  EFI_INVALID_PARAMETER  Buffer length is not equal to 0x2000 (8KB).
  @retval  EFI_ACCESS_DENIED      This protocol is invoked after SMM End of Dxe event.
**/
EFI_STATUS
EFIAPI
InstallIoBitmap (
  IN UINTN  *Buffer,
  IN UINT16  BufferLength
  )
{
  UINTN  CpuIndex;
  UINT8 *IoBitmap;

  if (BufferLength != IO_BITMAP_BUFFER_LENGTH) {
    DEBUG ((DEBUG_ERROR, "Invalid Buffer Length \n"));
    return EFI_INVALID_PARAMETER;
  }

  if (mSmmDgrLockSmmResourceControl) {
    return EFI_ACCESS_DENIED;
  }

  for (CpuIndex = 0; CpuIndex < gSmst->NumberOfCpus; CpuIndex++) {
    IoBitmap = (UINT8 *) (UINTN) (mSmmDgrIoBitMapBase [CpuIndex]);
    CopyMem (IoBitmap, (UINT8 *) Buffer, BufferLength);
  }

  return EFI_SUCCESS;
}

/**
  This function allows a caller to install new MSR Bitmap on top of the
  default MSR Bitmap.

  @param[in] Buffer          Buffer address of IO Bitmap.
  @param[in] BufferLength    Length of the buffer. This should be 0x1000.

  @retval  EFI_SUCCESS            MSR bitmap has been successfully installed.
  @retval  EFI_INVALID_PARAMETER  Buffer length is not equal to 0x1000 (4KB).
  @retval  EFI_ACCESS_DENIED      This protocol is invoked after SMM End of Dxe event.
**/
EFI_STATUS
EFIAPI
InstallMsrBitmap (
  IN UINTN  *Buffer,
  IN UINT16  BufferLength
  )
{
  UINT8 *MsrBitmap;

  if (BufferLength != MSR_BITMAP_BUFFER_LENGTH) {
    DEBUG((DEBUG_ERROR, "Invalid Buffer Length \n"));
    return EFI_INVALID_PARAMETER;
  }

  if (mSmmDgrLockSmmResourceControl) {
    return EFI_ACCESS_DENIED;
  }

  MsrBitmap = (UINT8 *) (UINTN) (gSmmDgrMsrBitMapBase);
  CopyMem (MsrBitmap, (UINT8 *) Buffer, BufferLength);

  return EFI_SUCCESS;
}

/**
  Initialize IO MSR Bitmap

  Processor Extended State Enumeration Main Leaf (EAX = 0DH, ECX = 0):
    EAX Bits 31 - 00: Reports the supported bits of the lower 32 bits of XCR0. XCR0[n] can be set to 1 only if EAX[n] is 1.
         Bit 00: x87 state.
         Bit 01: SSE state.
         Bit 02: AVX state.
         Bits 04 - 03: MPX state.
         Bits 07 - 05: AVX-512 state.
         Bit 08: Used for IA32_XSS.
         Bit 09: PKRU state.
         Bits 31 - 10: Reserved.
    EBX Bits 31 - 00: Maximum size (bytes, from the beginning of the XSAVE/XRSTOR save area) required by
                      enabled features in XCR0. May be different than ECX if some features at the end of the XSAVE save area
                      are not enabled.
    ECX Bit 31 - 00: Maximum size (bytes, from the beginning of the XSAVE/XRSTOR save area) of the
                     XSAVE/XRSTOR save area required by all supported features in the processor, i.e., all the valid bit fields in
                     XCR0.
    EDX Bit 31 - 00: Reports the supported bits of the upper 32 bits of XCR0. XCR0[n+32] can be set to 1 only if EDX[n] is 1.
         Bits 31 - 00: Reserved.

    Processor Extended State Enumeration Sub-leaf (EAX = 0DH, ECX = 1)
    EAX Bit 00: XSAVEOPT is available.
         Bit 01: Supports XSAVEC and the compacted form of XRSTOR if set.
         Bit 02: Supports XGETBV with ECX = 1 if set.
         Bit 03: Supports XSAVES/XRSTORS and IA32_XSS if set.
         Bits 31 - 04: Reserved.
    EBX Bits 31 - 00: The size in bytes of the XSAVE area containing all states enabled by XCRO | IA32_XSS.
    ECX Bits 31 - 00: Reports the supported bits of the lower 32 bits of the IA32_XSS MSR. IA32_XSS[n] can be
                      set to 1 only if ECX[n] is 1.
         Bits 07 - 00: Used for XCR0.
         Bit 08: PT state.
         Bit 09: Used for XCR0.
         Bits 31 - 10: Reserved.
    EDX Bits 31 - 00: Reports the supported bits of the upper 32 bits of the IA32_XSS MSR. IA32_XSS[n+32] can
                      be set to 1 only if EDX[n] is 1.
         Bits 31 - 00: Reserved.

    Processor Extended State Enumeration Sub-leaves (EAX = 0DH, ECX = n, n > 1)
    NOTES:
           Leaf 0DH output depends on the initial value in ECX.
           Each sub-leaf index (starting at position 2) is supported if it corresponds to a supported bit in either the
           XCR0 register or the IA32_XSS MSR.
           * If ECX contains an invalid sub-leaf index, EAX/EBX/ECX/EDX return 0. Sub-leaf n (0 <= n <= 31) is invalid
           if sub-leaf 0 returns 0 in EAX[n] and sub-leaf 1 returns 0 in ECX[n]. Sub-leaf n (32 <= n <= 63) is invalid if
           sub-leaf 0 returns 0 in EDX[n-32] and sub-leaf 1 returns 0 in EDX[n-32].
    EAX Bits 31 - 0: The size in bytes (from the offset specified in EBX) of the save area for an extended state
                     feature associated with a valid sub-leaf index, n.
    EBX Bits 31 - 0: The offset in bytes of this extended state component's save area from the beginning of the
                     XSAVE/XRSTOR area.
                     This field reports 0 if the sub-leaf index, n, does not map to a valid bit in the XCR0 register*.
    ECX Bit 00 is set if the bit n (corresponding to the sub-leaf index) is supported in the IA32_XSS MSR; it is clear
               if bit n is instead supported in XCR0.
         Bit 01 is set if, when the compacted format of an XSAVE area is used, this extended state component
                located on the next 64-byte boundary following the preceding state component (otherwise, it is located
                immediately following the preceding state component).
         Bits 31 - 02 are reserved. This field reports 0 if the sub-leaf index, n, is invalid*.
    EDX This field reports 0 if the sub-leaf index, n, is invalid*; otherwise it is reserved.

  @param[in] CpuIndex  CPU Index no.
  @param[in] GdtBase   GDT Base address
  @param[in] GdtSize   GDT size
**/
EFI_STATUS
EFIAPI
InitializeIoMsrBitmap (
  IN UINTN   CpuIndex,
  IN UINTN   GdtBase,
  IN UINTN   GdtSize
  )
{
  EFI_STATUS               Status = EFI_SUCCESS;
  IA32_SEGMENT_DESCRIPTOR  *GdtDescriptor;
  UINTN                    TssBase;
  UINTN                    IoMapOffset;
  UINT32                   Eax;
  UINT32                   Ebx;
  UINT32                   Ecx;
  UINT32                   Edx;
  UINT32                   Index;
  UINT64                   Xcr0;

  TssBase      = 0;
  IoMapOffset  = 0;

  if (gSmmDgrMsrBitMapBase == 0) {
    gSmmDgrMsrBitMapBase = (UINTN) AllocateCodePages (1);
    ASSERT(gSmmDgrMsrBitMapBase != 0);
  }

  if (gSmmDgrMsrBitMapBase != 0) {
    //
    // Fill all bits in the MSR bitmap field with 1 to Deny access for R/W.
    // Only required MSR access will be enabled by clearing its corresponding bit from Platform Pkg.
    //
    SetMem ((VOID *)gSmmDgrMsrBitMapBase, EFI_PAGES_TO_SIZE(1), 0xFF);
  }

  if (mSmmDgrIoBitMapBase == NULL) {
    mSmmDgrIoBitMapBase = AllocateCodePages (EFI_SIZE_TO_PAGES(sizeof(UINTN) * PcdGet32(PcdCpuMaxLogicalProcessorNumber)));

    if (mSmmDgrIoBitMapBase != NULL) {
      ZeroMem ((VOID *) mSmmDgrIoBitMapBase, sizeof (UINTN)* PcdGet32 (PcdCpuMaxLogicalProcessorNumber));
    } else {
      ASSERT(mSmmDgrIoBitMapBase != NULL);
      return EFI_OUT_OF_RESOURCES;
    }
  }

  DEBUG ((DEBUG_INFO, "GDTBase(%d) - 0x%x, ", CpuIndex, GdtBase));
  GdtDescriptor = (IA32_SEGMENT_DESCRIPTOR *)(GdtBase + TSS_SEGMENT);
  TssBase = (UINTN) (GdtDescriptor->Bits.BaseLow + LShiftU64((UINT64) GdtDescriptor->Bits.BaseMid, 16) + LShiftU64((UINT64) GdtDescriptor->Bits.BaseHigh, 24));

  IoMapOffset = *(UINTN *) (TssBase + TSS_IOMAP_OFFSET);

  mSmmDgrIoBitMapBase[CpuIndex] = TssBase + IoMapOffset;

  //
  // Fill all bits in the bitmap field with 1 to Deny access for R/W.
  // Only required IO Port access will be enabled by clearing its corresponding bit from Platform Pkg.
  //
  SetMem ((VOID *) (TssBase + IoMapOffset), 0x2000, 0xFF);
  DEBUG ((DEBUG_INFO, "mIoBitMapBase(%d) - 0x%x\n", CpuIndex, mSmmDgrIoBitMapBase[CpuIndex]));

  if (CpuIndex == 0) {
    Xcr0 = 3;
    for (Index = 0; Index < 64; Index++) {
      if ((LShiftU64 (1, Index) & Xcr0) == 0) {
        continue;
      }
      AsmCpuidEx (
        0xD,
        Index,
        &Eax,
        &Ebx,
        &Ecx,
        &Edx
        );
      if (Index == 0) {
        Xcr0 = Eax | LShiftU64 (Edx, 32);
      }
      DEBUG ((DEBUG_INFO, "CPUID  (0xD, %d) - 0x%08x, 0x%08x, 0x%08x, 0x%08x\n", Index, Eax, Ebx, Ecx, Edx));
    }
  }

  if (!mSmmDgrResourceConfigApiInitialized) {
    Status = SpsSmmResourceConfigApiInit ();
    mSmmDgrResourceConfigApiInitialized = TRUE;
  }

  return Status;
}
