/** @file
  User Consent bitmap conversion and display.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2009 Intel Corporation.

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

#include <Uefi.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/AmthiMsgsLib.h>
#include <Library/DxeMeLib.h>
#include <Protocol/GraphicsOutput.h>
#include <Protocol/MebxDisplayProtocol.h>

#define WIDTH_OFFSET          0x12
#define HEIGHT_OFFSET         0x16
#define PALETTE_OFFSET        0x36
#define PALETTE_SIZE          0x40
#define NUM_COLORS_IN_PALETTE 16

#define GET_HIGH_BITS(x) (x >> 4)
#define GET_LOW_BITS(x)  (x & 0xF)

typedef EFI_GRAPHICS_OUTPUT_BLT_PIXEL PALETTE[NUM_COLORS_IN_PALETTE];

/**
  Decompress RLE4 packed indexed image (BMP compliant) into raw BGR image.

  @param[out] DecompressedBuffer     Output buffer with decompressed image
  @param[in]  CompressedBuffer       Buffer that contains RLE4 packed data
  @param[in]  CompressedBufferSize   Length of CompressedBuffer
  @param[in]  Width                  Width of image
  @param[in]  Height                 Height of image
  @param[in]  Palette                Pointer to array of 16 RAW_PIXEL entries

  @retval EFI_SUCCESS                Image successfully decompressed
  @retval EFI_INVALID_PARAMETER      Invalid parameter
  @retval EFI_ABORTED                An error occurs during decompression
**/
STATIC
EFI_STATUS
Rle4Decompress (
     OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL  *DecompressedBuffer,
  IN     UINT8                          *CompressedBuffer,
  IN     UINT32                         CompressedBufferSize,
  IN     UINT32                         Width,
  IN     UINT32                         Height,
  IN     PALETTE                        Palette
  )
{
  UINT32  IndexX;
  UINT32  IndexY;
  UINT32  IndexI;
  UINT8   Cmd;
  UINT8   *BufferPtr;
  UINT8   Indices[2];
  UINT32  BaseOffset;
  BOOLEAN EndOfLine;
  UINT8   Counter;
  UINT8   SequenceStart;

  IndexX     = 0;
  Counter    = 0;
  BufferPtr  = CompressedBuffer;
  EndOfLine  = FALSE;

  if (DecompressedBuffer == NULL || CompressedBuffer == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  for (IndexY = 0; IndexY < Height; IndexY++) {
    IndexX  = 0;
    Counter = 0;
    EndOfLine = FALSE;
    while (!EndOfLine) {
      if ((BufferPtr + 2) > (CompressedBuffer + CompressedBufferSize)) {
        //overrun in CompressedBuffer
        return EFI_ABORTED;
      }

      Counter = *(BufferPtr++);
      if (Counter == 0) {
        //if special command met
        Cmd = *(BufferPtr++);
        if (Cmd == 0) {
          //EndOfLine marker?
          if (IndexX < Width) {
            //line width is not equal to the image width
            return EFI_ABORTED;
          } else {
            EndOfLine = TRUE;
          }

        } else if (Cmd == 1) {
          //End-Of-Bitmap  marker?
          if (IndexY != (Height - 1)) {
            //end-of-data marker wrong placement
            return EFI_ABORTED;
          }
        } else {
          //delta and absolute mode are unsupported
          return EFI_ABORTED;
        }
      } else {
        // handle packed pixel sequence
        if ((IndexX + Counter) > Width) {
          //check line length constraint
          return EFI_ABORTED;
        }
        //read color Indices
        Indices[0] = GET_HIGH_BITS(*BufferPtr);
        Indices[1] = GET_LOW_BITS(*BufferPtr);
        BufferPtr++;

        //index of start of sequence
        BaseOffset = IndexY * Width + IndexX;
        //write uncompressed sequence
        for (IndexI = 0; IndexI < Counter; IndexI++) {
          SequenceStart = Indices[IndexI & 1];
          DecompressedBuffer[BaseOffset + IndexI] = Palette[SequenceStart];
        }
        IndexX += Counter;
      }
    }
  }

  return EFI_SUCCESS;
}

/**
  This function inverts order of lines of input buffer.

  @param[in,out] DataBuf  Data to be inverted, the result will be in the same buffer
  @param[out]    Width    Image width
  @param[out]    Height   Image height

  @retval EFI_SUCCESS           The operation succeeded.
  @retval EFI_OUT_OF_RESOURCES  Can't allocate memory resources.
 */
STATIC
EFI_STATUS
InvertOrderOfLines (
  IN OUT UINT32 *DataBuf,
  IN     UINT32 Width,
  IN     UINT32 Height
  )
{
  UINT32  *TempRow;
  UINT32  RowByteLen;
  UINT32  *Row1;
  UINT32  *Row2;
  UINT32  Index;

  RowByteLen = Width * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL);

  TempRow = AllocateZeroPool (RowByteLen);
  if (TempRow == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  for (Index = 0; Index < Height / 2; Index++) {
    Row1 = DataBuf + Index * Width;
    Row2 = DataBuf + (Height - Index - 1) * Width;
    CopyMem (TempRow, Row1, RowByteLen);
    CopyMem (Row1, Row2, RowByteLen);
    CopyMem (Row2, TempRow, RowByteLen);
  }

  FreePool (TempRow);

  return EFI_SUCCESS;
}

/**
  This function returns buffer containing bitmap to be displayed on-screen.
  On success, buffer is allocated by this function and must be freed by caller.

  @param[in]  ScreenResolution  Current screen resolution
  @param[in]  ImageId           Specifies the requested Image Id, values per AMT_IMAGE_ID definition
  @param[out] Width             Image width
  @param[out] Height            Image height
  @param[out] Bitmap            Received bitmap, it must by freed by caller.

  @retval EFI_SUCCESS           Bitmap buffer contains new picture to be displayed. Caller must free the buffer afterwards
  @retval anything else         There was an error, buffer is empty and should not be deallocated
 */
EFI_STATUS
GetUserConsentBitmap (
  IN     AMT_SCREEN_RESOLUTION         ScreenResolution,
  IN     AMT_IMAGE_ID                  ImageId,
     OUT UINT32                        *Width,
     OUT UINT32                        *Height,
     OUT EFI_GRAPHICS_OUTPUT_BLT_PIXEL **Bitmap
  )
{
  UINT32                        ImageDataLength;
  UINT16                        LeftXPos;
  UINT16                        LeftYPos;
  UINT8                         *Buffer;
  UINT32                        ByteOffset;
  UINT32                        ByteSizeReq;
  UINT32                        ByteSizeResp;
  UINT8                         *CompressedBuffer;
  UINT32                        CompressedBufferSize;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *DecompressedBuffer;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *ColorPalette;
  UINT32                        ImageWidth;
  UINT32                        ImageHeight;
  EFI_STATUS                    Status;

  ByteOffset         = 0;
  Buffer             = NULL;
  DecompressedBuffer = NULL;

  if (Width == NULL || Height == NULL || Bitmap == NULL) {
    return EFI_INVALID_PARAMETER;
  }

  Status = AmthiGetGfxUiInfo (ScreenResolution, ImageId, &ImageDataLength, &LeftXPos, &LeftYPos);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Buffer = AllocateZeroPool (ImageDataLength);

  if (Buffer == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }

  while (ImageDataLength != 0) {
    ByteSizeReq = MIN (ImageDataLength, AMT_UI_MAX_DATA_SIZE);
    Status = AmthiGetGfxFragment (ImageId, ByteOffset, ByteSizeReq, &ByteSizeResp, Buffer + ByteOffset);
    if (EFI_ERROR (Status)) {
      FreePool (Buffer);
      return Status;
    }
    ByteOffset += ByteSizeResp;
    ImageDataLength -= ByteSizeResp;
  }

  if (ByteOffset < (PALETTE_OFFSET + PALETTE_SIZE)) {
    FreePool (Buffer);
    return EFI_INVALID_PARAMETER;
  }

  ImageWidth           = *(UINT32*) (Buffer + WIDTH_OFFSET);
  ImageHeight          = *(UINT32*) (Buffer + HEIGHT_OFFSET);
  CompressedBuffer     = Buffer + (PALETTE_OFFSET + PALETTE_SIZE);
  CompressedBufferSize = ByteOffset - (PALETTE_OFFSET + PALETTE_SIZE);
  ColorPalette         = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL*) (Buffer + PALETTE_OFFSET);

  DecompressedBuffer = AllocateZeroPool (ImageWidth * ImageHeight * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

  if (DecompressedBuffer == NULL) {
    FreePool (Buffer);
    return EFI_OUT_OF_RESOURCES;
  }

  Status = Rle4Decompress (
             DecompressedBuffer,
             CompressedBuffer,
             CompressedBufferSize,
             ImageWidth,
             ImageHeight,
             ColorPalette
             );

  FreePool (Buffer);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "Rle4Decompress, status = %r\n", Status));
    FreePool (DecompressedBuffer);
    return Status;
  }

  // invert order of lines
  Status = InvertOrderOfLines ((UINT32*) DecompressedBuffer, ImageWidth, ImageHeight);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InvertOrderOfLines, status = %r\n", Status));
    FreePool (DecompressedBuffer);
    return EFI_DEVICE_ERROR;
  }

  *Bitmap = DecompressedBuffer;
  *Height = ImageHeight;
  *Width  = ImageWidth;

  return EFI_SUCCESS;
}

/**
  This function retrieves bitmap from AMT and displays it on screen.

  @param[in] ScreenId     Id of image to be displayed

  @retval EFI_SUCCESS     An image is being displayed on screen, flow continues
  @retval anything else   There was an error
**/
STATIC
EFI_STATUS
DisplayImage (
  IN UINT32 ScreenId
  )
{
  EFI_STATUS                    Status;
  EFI_GRAPHICS_OUTPUT_BLT_PIXEL *Bitmap;
  MEBX_DISPLAY_PROTOCOL         *MebxDisplayProtocol;
  UINT32                        ImgWidth;
  UINT32                        ImgHeight;
  AMT_SCREEN_RESOLUTION         Screen;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MebxDisplayProtocol->GetScreenResolution (MebxDisplayProtocol, &Screen.Width, &Screen.Height);
  DEBUG ((DEBUG_ERROR, "GetScreenResolution, status = %r \n", Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  DEBUG ((DEBUG_ERROR, "MEBX: resolution %d:%d\n", Screen.Height, Screen.Width));

  Status = GetUserConsentBitmap (Screen, ScreenId, &ImgWidth, &ImgHeight, &Bitmap);
  DEBUG ((DEBUG_ERROR, "GetUserConsentBitmap, status = %r \n", Status));
  if (EFI_ERROR (Status)) {
    return Status;
  }

  Status = MebxDisplayProtocol->DisplayImage (MebxDisplayProtocol, (UINT8*) Bitmap, ImgWidth, ImgHeight);
  DEBUG ((DEBUG_ERROR, "DisplayImage, status = %r \n", Status));

  FreePool (Bitmap);
  return Status;
}

/**
  The function sends current state of MEBx and last user input to AMT and basing on an answer displays image or
  stops displaying by returning EFI_ABORTED or EFI_DEVICE_ERROR

  This function should be called in a loop until it returns EFI_ABORTED or EFI_DEVICE_ERROR

  @param[in]  UserInput       Indicates last user input

  @retval EFI_SUCCESS         An image is being displayed on screen, flow continues
  @retval EFI_ABORTED         User Consent flow has finished, BIOS may erase screen and continue booting
  @retval EFI_DEVICE_ERROR    There was an unexpected error in HECI, graphics or keyboard. User Consent flow
                              must be skipped.
**/
STATIC
EFI_STATUS
UserConsentFlow (
  IN UINT32 UserInput
  )
{
  EFI_STATUS          Status;
  STATIC AMT_IMAGE_ID ScreenId = NullImageId;
  AMT_IMAGE_ID        NewScreenId;

  Status = AmthiGetUiAction (ScreenId, UserInput, &NewScreenId);
  DEBUG ((DEBUG_ERROR, "AmthiGetUiAction %r \n", Status));
  if (EFI_ERROR (Status)) {
    return EFI_DEVICE_ERROR;
  }
  if (NewScreenId == ImageIdExitUi) {
    return EFI_ABORTED;
  }
  if (NewScreenId != ScreenId) {
    Status = DisplayImage (NewScreenId);
    DEBUG ((DEBUG_ERROR, "DisplayImage %r \n", Status));
    if (EFI_ERROR (Status)) {
      return EFI_DEVICE_ERROR;
    }
  }
  ScreenId = NewScreenId;
  // safety check in case something goes wrong inside AMT: if user pressed Esc then exit regardless of AMT response
  if (UserInput == UserInputEsc) {
    DEBUG ((DEBUG_ERROR, "%a: EFI_ABORTED\n", __FUNCTION__));
    return EFI_ABORTED;
  }
  DEBUG ((DEBUG_ERROR, "%a: EFI_SUCCESS\n", __FUNCTION__));
  return EFI_SUCCESS;
}

/**
  This function checks if User Consent flow should be executed

  @retval TRUE    User Consent flow should be executed
  @retval FALSE   User Consent flow shouldn't be executed, this includes also a situation where query failed due to HECI error
**/
BOOLEAN
IsUserConsentRequired (
  VOID
  )
{
  EFI_STATUS Status;
  BOOLEAN    UserConsentRequired;

  UserConsentRequired = FALSE;

  Status = HeciGetIsUserConsentRequired (&UserConsentRequired);
  if (EFI_ERROR (Status)) {
    return FALSE;
  }
  return UserConsentRequired;
}

/**
  Function reads bitmap from AMT and displays it as USER CONSENT. It continuously scans keyboard for user input
  and reads HECI to see if it should update the picture on display.

  @retval EFI_SUCCESS         An user aborted user consent flow
  @retval EFI_PROTOCOL_ERROR  Cannot read user input
  @retval EFI_DEVICE_ERROR    User consent flow ended with an error in HECI, KBD or graphics
**/
EFI_STATUS
PerformUserConsent (
  VOID
  )
{
  EFI_STATUS            Status;
  AMT_MEBX_USER_INPUT   UserInput;
  BOOLEAN               UserConsentFlowInProgress;
  MEBX_DISPLAY_PROTOCOL *MebxDisplayProtocol;

  UserInput                 = UserInputNoInput;
  UserConsentFlowInProgress = TRUE;

  Status = gBS->LocateProtocol (&gMebxDisplayProtocolGuid, NULL, (VOID**) &MebxDisplayProtocol);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  while (UserConsentFlowInProgress) {
    Status = UserConsentFlow (UserInput);
    switch (Status) {
      case EFI_SUCCESS:
        DEBUG ((DEBUG_ERROR, "UserConsentFlow SUCCESS\n"));
        Status = MebxDisplayProtocol->GetUserInput (MebxDisplayProtocol, (MEBX_USER_INPUT*) &UserInput);
        if (EFI_ERROR (Status)) {
          UserConsentFlowInProgress = FALSE;
        }
        break;
      case EFI_ABORTED:
        DEBUG ((DEBUG_ERROR, "UserConsentFlow ABORTED\n"));
        // User Consent flow ended successfully. Clear screen if needed, then return
        Status = EFI_SUCCESS;
        UserConsentFlowInProgress = FALSE;
        break;
      case EFI_DEVICE_ERROR:
      case EFI_PROTOCOL_ERROR:
      default:
        DEBUG ((DEBUG_ERROR, "UserConsentFlow ERROR\n"));
        // User consent flow ended with an error in HECI, KBD or graphics
        Status = EFI_DEVICE_ERROR;
        UserConsentFlowInProgress = FALSE;
        break;
    }
  }

  DEBUG ((DEBUG_ERROR, "%a Exit, status - %\n", __FUNCTION__, Status));

  return Status;
}
