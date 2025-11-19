### @file
# This script processes a BMP file and converts it to VGA Mode 12h planar data format.
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2025 Intel Corporation.
#
#  This software and the related documents are Intel copyrighted materials,
#  and your use of them is governed by the express license under which they
#  were provided to you ("License"). Unless the License provides otherwise,
#  you may not use, modify, copy, publish, distribute, disclose or transmit
#  this software or the related documents without Intel's prior written
#  permission.
#
#  This software and the related documents are provided as is, with no
#  express or implied warranties, other than those that are expressly stated
#  in the License.
#
# @par Specification Reference:
#
# @par Glossary:
###
__version__ = 1.1

# Import necessary libraries
from PIL import Image
from pathlib import Path
import numpy as np
import math
import argparse
import os
import sys

import logging
from colorama import Fore, Style, init

# Configuration for the BMP file and VGA settings
VGA_PLANES = 4  # Number of VGA planes in Mode 12h (4 planes for 16 colors)

# Define a global logger variable
logger = None

# Define the GOP palette (16-color VGA palette)
CUSTOM_PALETTE = [
    (0x00, 0x00, 0x00),  # Black
    (0x00, 0x00, 0xAA),  # Blue
    (0x00, 0xAA, 0x00),  # Green
    (0x00, 0xAA, 0xAA),  # Cyan
    (0xAA, 0x00, 0x00),  # Red
    (0xAA, 0x00, 0xAA),  # Magenta
    (0xAA, 0x55, 0x00),  # Brown
    (0xAA, 0xAA, 0xAA),  # Light Gray
    (0x55, 0x55, 0x55),  # Dark Gray
    (0x55, 0x55, 0xFF),  # Light Blue
    (0x55, 0xFF, 0x55),  # Light Green
    (0x55, 0xFF, 0xFF),  # Light Cyan
    (0xFF, 0x55, 0x55),  # Light Red
    (0xFF, 0x55, 0xFF),  # Light Magenta
    (0xFF, 0xFF, 0x55),  # Yellow
    (0xFF, 0xFF, 0xFF)   # White
]

def setup_colored_logger():
    """
    Set up a logger with colored output for different log levels.
    This function initializes a global logger with colored output for different
    log levels using the `colorama` library. The logger is configured to display
    messages in different colors based on their severity level (DEBUG, INFO,
    WARNING, ERROR, CRITICAL). The function ensures that the logger is only
    initialized once and prevents reinitialization.
    Returns:
        logging.Logger: A configured logger instance with colored output.
    """
    global logger  # Make the logger global so it can be used everywhere

    if logger is not None:  # Prevent reinitialization
        return logger

    # Initialize colorama for Windows support
    init(autoreset=True)

    # Define color mapping for each log level
    log_colors = {
        logging.DEBUG: Fore.BLUE,
        logging.INFO: Fore.GREEN + Style.BRIGHT,
        logging.WARNING: Fore.YELLOW,
        logging.ERROR: Fore.RED,
        logging.CRITICAL: Fore.MAGENTA + Style.BRIGHT,
    }

    class ColoredFormatter(logging.Formatter):
        def format(self, record):
            log_color = log_colors.get(record.levelno, Fore.WHITE)
            log_message = super().format(record)
            return f"{log_color}{log_message}{Style.RESET_ALL}"

    # Create a logger
    logger = logging.getLogger("ColoredLogger")
    logger.setLevel(logging.DEBUG)

    # Avoid duplicate handlers
    if not logger.handlers:
        console_handler = logging.StreamHandler()
        console_handler.setLevel(logging.DEBUG)

        formatter = ColoredFormatter("[%(levelname)s] %(message)s")
        console_handler.setFormatter(formatter)

        logger.addHandler(console_handler)

    return logger

# Initialize logger at the module level
setup_colored_logger()

def match_palette_color(rgb_color, palette):
    """
    Find the closest color in the palette using Euclidean distance.

    Args:
        rgb_color (tuple): The RGB color to match (R, G, B).
        palette (list): The list of RGB colors in the palette.

    Returns:
        int: The index of the closest color in the palette.
    """
    # Convert np.uint8 to normal int
    rgb_color = tuple(map(int, rgb_color))

    # Calculate the Euclidean distance between the given color and each color in the palette
    distances = [sum((c1 - c2) ** 2 for c1, c2 in zip(rgb_color, pal)) for pal in palette]

    # Find the index of the closest color in the palette
    closest_index = distances.index(min(distances))

    # Log a warning if the color is not in the palette
    if rgb_color not in palette:
        logger.warning(f"Color {rgb_color} not in palette. Replacing with {palette[closest_index]}")

    return closest_index

def convert_to_8bit_palette(image_path, output_path):
    """
    Convert a 24 BMP image to an 8-bit indexed BMP with a custom 16-color palette.

    Args:
        image_path (str): Path to the input 24 BMP image.
        output_path (str): Path to save the output 8-bit indexed BMP.

    Returns:
        np.ndarray: 2D array of indexed pixels.
    """
    # Open the image and convert it to RGB
    img = Image.open(image_path).convert("RGB")
    width, height = img.size

    # Create a new image with mode 'P' (palette-based)
    indexed_img = Image.new("P", (width, height))

    # Print the custom palette information
    logger.info("Custom Palette:")
    for i, color in enumerate(CUSTOM_PALETTE):
        logger.info(f"Index {i:2}: RGB({color[0]:3}, {color[1]:3}, {color[2]:3})")

    # Create a flat palette for PIL (RGB triplets)
    flat_palette = [value for rgb in CUSTOM_PALETTE for value in rgb] + [0] * (256 - len(CUSTOM_PALETTE)) * 3
    indexed_img.putpalette(flat_palette)

    # Convert the image to a numpy array for pixel manipulation
    pixels = np.array(img)
    indexed_pixels = np.zeros((height, width), dtype=np.uint8)

    # Match each pixel to the closest color in the custom palette
    for y in range(height):
        for x in range(width):
            indexed_pixels[y, x] = match_palette_color(tuple(pixels[y, x]), CUSTOM_PALETTE)

    # Put the indexed pixel data into the new image and save it
    indexed_img.putdata(indexed_pixels.flatten())
    indexed_img.save(output_path, format="BMP")
    logger.info(f"Conversion complete! The 8-bit indexed BMP has been saved as {output_path}")

    return indexed_pixels

def generate_planar_buffer(indexed_pixels, width, height):
    """
    Generate a VGA Mode 12h planar buffer from indexed pixel data.
    This function converts a 2D array of indexed pixel values into a planar buffer
    suitable for VGA Mode 12h, ensuring proper 8-pixel alignment. The output buffer
    consists of 4 planes, each representing a bit plane of the image.
    Args:
        indexed_pixels (np.ndarray): A 2D numpy array of shape (height, width) containing
                                     indexed pixel values.
        width (int): The width of the image in pixels.
        height (int): The height of the image in pixels.
    Returns:
        np.ndarray: A 3D numpy array of shape (4, height, bytes_per_row) representing the
                    planar buffer, where bytes_per_row is the number of bytes per row
                    calculated as ceil(width / 8).
    """
    # Calculate the number of bytes per row, ensuring we account for all pixels
    bytes_per_row = math.ceil(width / 8)
    logger.info(f"Bytes per row: {bytes_per_row}")
    # Initialize the planar buffer with zeros (4 planes, height rows, bytes_per_row columns)
    planar_buffer = np.zeros((4, height, bytes_per_row), dtype=np.uint8)

    # Iterate over each pixel in the image
    for y in range(height):
        for x in range(width):
            # Get the index of the current pixel in the palette
            pixel_index = indexed_pixels[y, x]

            # Calculate the bit position within the byte (reverse order, MSB first)
            bit_position = 7 - (x % 8)

            # Iterate over each of the 4 planes
            for plane_num in range(4):
                # Check if the current bit in the pixel index is set
                if pixel_index & (1 << plane_num):
                    # Set the corresponding bit in the planar buffer
                    planar_buffer[plane_num, y, x // 8] |= (1 << bit_position)

    return planar_buffer

def save_planar_buffer_to_header(planar_buffer, width, height, output_path):
    """
    Save the planar buffer to a C header file (.h).
    This function takes a planar buffer representing an image, along with its width and height,
    and writes it to a specified output file in the form of a C header file. The header file
    contains the image data as a 3-dimensional array and a custom palette.
    Args:
        planar_buffer (numpy.ndarray): A 3-dimensional numpy array of shape (4, height, width // 8)
                                       representing the planar buffer of the image.
        width (int): The width of the image in pixels.
        height (int): The height of the image in pixels.
        output_path (str): The file path where the C header file will be saved.
    Returns:
        None
    """
    """Save the planar buffer to a C header file (.h)."""
    bytes_per_row = math.ceil(width / 8)
    with open(output_path, "w") as f:
        f.write("#ifndef _IMAGE_DATA_H_\n#define _IMAGE_DATA_H_\n\n")
        f.write(f"#define BMP_WIDTH {bytes_per_row * 8}\n")
        f.write(f"#define BMP_HEIGHT {height}\n\n")
        f.write(f"CONST UINT8 ImageData[4][{height}][{bytes_per_row}] = {{\n")

        for plane in range(4):
            f.write("  {\n")
            for y in range(height):
                f.write("    { " + ", ".join(f"0x{byte:02X}" for byte in planar_buffer[plane, y]) + " },\n")
            f.write("  },\n")

        VGA_PALETTE = [[int(c * 63 / 255) for c in rgb] for rgb in CUSTOM_PALETTE]

        f.write("};\n\n")

        f.write("CONST UINT8 CustomPalette[16][3] = {\n")
        for color in VGA_PALETTE:
            f.write(f"    {{ {color[0]}, {color[1]}, {color[2]} }},\n")
        f.write("};\n\n")

        f.write("#endif // _IMAGE_DATA_H_\n")

    logger.info(f"Planar buffer saved as {output_path}")

def generate_bw_buffer(image_path, width, height):
    """
    Generate a 1-bit (black & white) planar buffer.
    """
    img = Image.open(image_path).convert("L")  # grayscale
    threshold = 128
    pixels = np.array(img)
    bw_pixels = (pixels > threshold).astype(np.uint8)

    bytes_per_row = math.ceil(width / 8)
    logger.info(f"Bytes per row (BW): {bytes_per_row}")
    buffer = np.zeros((height, bytes_per_row), dtype=np.uint8)

    for y in range(height):
        for x in range(width):
            bit_position = 7 - (x % 8)
            if bw_pixels[y, x]:
                buffer[y, x // 8] |= (1 << bit_position)

    return buffer

def save_bw_buffer_to_header(bw_buffer, width, height, output_path):
    """
    Save the 1-bit BW buffer to a C header file (.h).
    """
    bytes_per_row = math.ceil(width / 8)
    with open(output_path, "w") as f:
        f.write("#ifndef _BW_IMAGE_DATA_H_\n#define _BW_IMAGE_DATA_H_\n\n")
        f.write(f"#define BMP_WIDTH {bytes_per_row * 8}\n")
        f.write(f"#define BMP_HEIGHT {height}\n\n")
        f.write(f"CONST UINT8 BWImageData[{height}][{bytes_per_row}] = {{\n")

        for y in range(height):
            f.write("    { " + ", ".join(f"0x{byte:02X}" for byte in bw_buffer[y]) + " },\n")

        f.write("};\n\n")
        f.write("#endif // _BW_IMAGE_DATA_H_\n")

    logger.info(f"BW buffer saved as {output_path}")

def save_bw_bmp(bw_buffer, width, height, output_path):
    """
    Save the BW buffer as a BMP image for visual verification.
    """
    img = Image.new("1", (width, height))  # 1-bit mode
    pixels = img.load()

    for y in range(height):
        for byte_index, byte in enumerate(bw_buffer[y]):
            for bit in range(8):
                bit_position = 7 - bit
                x = byte_index * 8 + bit
                if x < width:
                    pixels[x, y] = 1 if (byte & (1 << bit_position)) else 0

    img.save(output_path, format="BMP")
    logger.info(f"BW BMP image saved as {output_path}")

def main():
    """
    Main function to process a BMP file and convert it to VGA Mode 12h planar data format.
    This function performs the following steps:

    1. Parses command-line arguments to get the input BMP file path and output C header file name.
    2. Checks if the input BMP file exists.
    3. Converts the 24-bit BMP file to an 8-bit indexed BMP file.
    4. Generates a planar buffer from the indexed BMP file.
    5. Saves the planar buffer to the specified C header file.

    Command-line arguments:
    -i, --input: Path to the BMP file to be processed (required).
    -o, --output: Name of the output C header file (required).
    """
    logger.info(f"BmpToBufferConverter version: {__version__}")

    # Parse command-line arguments
    parser = argparse.ArgumentParser(description="Process a BMP file and convert it to VGA Mode 12h or BW planar data format.")
    parser.add_argument("-i", "--input", required=True, help="Path to the BMP file to be processed.")
    parser.add_argument("-o", "--output", required=True, help="Name of the output C header file.")
    parser.add_argument("-m", "--mode", choices=["vga16", "bw"], default="vga16", help="Conversion mode: vga16 (16-color VGA) or bw (black & white). Default is vga16.")
    args = parser.parse_args()

    if not os.path.isfile(args.input):
        logger.error(f"Error: Input file '{args.input}' not found.")
        return

    input_file_name = Path(args.input).stem
    output_bmp = f"{input_file_name}_output_8bit.bmp"

    img = Image.open(args.input)
    width, height = img.size
    logger.info(f"Image dimensions: Width = {width}, Height = {height}")

    if args.mode == "vga16":
        # VGA 16-color mode
        logger.info("Converting to 8-bit indexed BMP for VGA Mode 12h...")
        indexed_pixels = convert_to_8bit_palette(args.input, output_bmp)

        logger.info("Generating VGA planar buffer...")
        planar_buffer = generate_planar_buffer(indexed_pixels, width, height)

        logger.info(f"Saving VGA planar buffer to {args.output}...")
        save_planar_buffer_to_header(planar_buffer, width, height, args.output)

    elif args.mode == "bw":
        # Black & White mode
        logger.info("Generating BW buffer...")
        bw_buffer = generate_bw_buffer(args.input, width, height)

        logger.info(f"Saving BW buffer to {args.output}...")
        save_bw_buffer_to_header(bw_buffer, width, height, args.output)

        output_bmp = f"{Path(args.input).stem}_bw_output.bmp"
        save_bw_bmp(bw_buffer, width, height, output_bmp)

    logger.info("#" * 100)
    logger.info(f"Processing complete! Output saved to {args.output}")
    logger.info("#" * 100)

    if not width % 8 == 0:
        logger.critical("Width should be a multiple of 8. The header file has been updated accordingly.")

if __name__ == "__main__":
    main()