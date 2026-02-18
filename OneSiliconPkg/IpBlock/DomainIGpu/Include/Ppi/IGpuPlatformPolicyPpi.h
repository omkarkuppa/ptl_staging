/** @file
  The PEI_IGPU_PLATFORM_POLICY_PPI provides platform information to PEI Graphics PEIM.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2019 Intel Corporation.

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

#ifndef _IGPU_PLATFORM_POLICY_PPI_H_
#define _IGPU_PLATFORM_POLICY_PPI_H_

/**
  Globally unique identifier for PEI platform policy PPI.
**/
#define PEI_IGPU_PLATFORM_POLICY_PPI_GUID \
{ \
  0x4eabcd09, 0x43d3, 0x4b4d, { 0xb7, 0x3d, 0x43, 0xc8, 0xd9, 0x89, 0x99, 0x5 } \
}

#define PEI_IGPU_PLATFORM_POLICY_REVISION  2

/**
Pre-declaration of PEI IGPU platform policy PPI.
**/
typedef struct _PEI_IGPU_PLATFORM_POLICY_PPI  PEI_IGPU_PLATFORM_POLICY_PPI;
typedef struct _EFI_PREMEM_GRAPHICS_PPI       EFI_PREMEM_GRAPHICS_PPI;

/**
  Enum defining the different lid status values
**/
typedef enum {
  LidClosed,
  LidOpen,
  LidStatusMax
} LID_STATUS;

/**
 * Enum defining text alignment for VGA
 */
typedef enum {
  VGA_TEXT_LEFT,
  VGA_TEXT_CENTER,
  VGA_TEXT_RIGHT,
} VGA_TEXT_ALIGNMENT;

typedef enum {
  GopPreMemPhase  = 0,
  GopPostMemPhase = 1
} GOP_BOOT_PHASE;

typedef enum {
  VGA_TEXT            = 0, // Only for premem
  VGA_GRAPHICS        = 1, // Only for premem
  HIRES_GRAPHICS_MODE = 2
} GOP_MODE;

typedef struct _STATUS_FLAGS {
  UINT8    LidStatus    :1; // 1 - Open, 0 - Closed
  UINT8    BootPhase    :1;
  UINT8    ReservedBits :6;
} STATUS_FLAGS;

typedef struct {
  UINT8           Version; // For future changes and compatibility.
  UINT8           Size;    // Size of structure.
  VOID            *VbtPtr; // Pointer to vbt data
  UINT32          VbtSize; // Size of vbt.
  STATUS_FLAGS    Flags;
  GOP_MODE        GfxMode;
} GOP_POLICY_PTR;

/**
  This function gets the platform lid status for LFP displays.

  @param[out] CurrentLidStatus  Output variable to store the lid status.

  @retval EFI_SUCCESS           Correct lid status is returned.
  @retval EFI_UNSUPPORTED       Platform does not support lid.
**/
typedef
EFI_STATUS
(EFIAPI *GET_PLATFORM_LID_STATUS)(
  OUT LID_STATUS  *CurrentLidStatus
  );

/**
  This function gets the base address of loaded VBT.

  @param[out] VbtAddress    Starting address of the VBT is returned in this parameter.
  @param[out] VbtSize       Size of the VBT is returned in this parameter.

  @retval     EFI_SUCCESS   If the VBT is loaded and parameters contain valid values.
  @return     Other error codes meaning VBT is not loaded and parameters contain invalid values.
**/
typedef
EFI_STATUS
(EFIAPI *GET_VBT_DATA)(
  OUT EFI_PHYSICAL_ADDRESS *VbtAddress,
  OUT UINT32               *VbtSize
  );


/**
  This defines the PEI Graphics Platform Policy PPI structure.
**/
struct _PEI_IGPU_PLATFORM_POLICY_PPI {
  UINT32                     Revision;                  ///< Revision of current implementation.
  GET_PLATFORM_LID_STATUS    GetPlatformLidStatus;      ///< Function Pointer for get platform lid status.
  GET_VBT_DATA               GetVbtData;                ///< Function pointer for get vbt data.
  UINT32                     GetMaxActiveDisplays;      ///< Max Displays to be enabled by PEIM, Max Value : 2, 0 :default VBT..
};

/**
  The GraphicsPreMemPpiInit initializes the graphics subsystem in phases.
  @param[in] GraphicsPolicyPtr    GraphicsPolicyPtr points to a configuration data
                                  block of policy settings required by Graphics PEIM.
  @retval EFI_SUCCESS             The invocation was successful.
  @retval EFI_INVALID_PARAMETER   The phase parameter is not valid.
  @retval EFI_NOT_ABORTED         The stages was not called in the proper order.
  @retval EFI_NOT_FOUND           The PeiGraphicsPlatformPolicyPpi is not located.
  @retval EFI_DEVICE_ERROR        The initialization failed due to device error.
  @retval EFI_NOT_READY           The previous init stage is still in progress and not
                                  ready for the current initialization phase yet. The
                                  platform code should call this again sometime later.
**/
typedef
EFI_STATUS
(EFIAPI *PREMEM_PEI_GRAPHICS_INIT)(
  IN VOID                            *GraphicsPolicyPtr
  );

typedef
EFI_STATUS
(EFIAPI *PREMEM_PEI_GRAPHICS_EXIT)(
  VOID
  );

typedef
VOID
(EFIAPI *PREMEM_PEI_GRAPHICS_VGA_WRITE)(
  IN VGA_TEXT_ALIGNMENT Alignment,
  IN UINTN              Line,
  IN CHAR8              *String
  );

typedef
EFI_STATUS
(EFIAPI *PREMEM_PEI_GRAPHICS_VGA_GRAPHICS_VIDEO_FILL)(
  IN VOID *Blt
  );

struct _EFI_PREMEM_GRAPHICS_PPI {
  PREMEM_PEI_GRAPHICS_INIT                     GraphicsPreMemPpiInit;
  PREMEM_PEI_GRAPHICS_EXIT                     GraphicsPreMemPpiExit;
  PREMEM_PEI_GRAPHICS_VGA_WRITE                GraphicsPreMemPpiVgaWrite;
  PREMEM_PEI_GRAPHICS_VGA_GRAPHICS_VIDEO_FILL  VGAFillBuffer;
};

extern EFI_GUID  gPeiGraphicsPlatformPpiGuid;
extern EFI_GUID  gIntelPeiPreMemGraphicsPpiGuid;

#endif // _IGPU_PLATFORM_POLICY_PPI_H_
