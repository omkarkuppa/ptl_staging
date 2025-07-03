# @file
#   Setup files parser
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2021 Intel Corporation.
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
from __future__ import division
from __future__ import nested_scopes
from __future__ import with_statement
from __future__ import print_function


__version__ = "1.0.0"
__author__ = ["ashinde", "gsaraiya"]


import os
import re
import sys
import glob
import copy
import time
import shutil
import string
import binascii
import argparse
import subprocess
from collections import namedtuple


PY2 = bool(sys.version_info.major == 2)

BIOS_KNOBS_DATA_BIN_HDR_SIZE = 0x50
# Version 0x4    Supports BitWise Setup Knobs
BIOS_KNOB_BIN_REVISION = 0x04  # Version 0.4
BIOS_KNOB_BIN_REVISION_HEX = hex(BIOS_KNOB_BIN_REVISION)[2:].strip().strip('L').zfill(2)
NVAR_NAME_OFFSET_IN_HDR = 0x22
NVAR_NAME_OFFSET_IN_HDR_HEX = hex(NVAR_NAME_OFFSET_IN_HDR)[2:].strip().strip('L').zfill(2)
NVAR_GUID_OFFSET_IN_HDR = 0x12
NVAR_BIT_KNOB_COUNT_OFFSET_IN_HDR = 0x4E
BIOS_KNOB_BIN_NVAR_NAME_RESERVED = 0x2C
INVERT_DICT = {'_EQU_': '_NEQ_', '_NEQ_': '_EQU_', '_LTE_': '_GT_', '_GTE_': '_LT_', '_LT_': '_GTE_', '_GT_': '_LTE_'}
MATH_OPERATION_MAP = (
    ('==', '_EQU_'),
    ('!=', '_NEQ_'),
    ('<=', '_LTE_'),
    ('>=', '_GTE_'),
    ('<', '_LT_'),
    ('>', '_GT_')
)
MATH_OPERATION = ('AND', 'OR', 'NOT', '_EQU_', '_NEQ_', '_LTE_', '_GTE_', '_LT_', '_GT_', '_LIST_')
OPERATION_DICT = {'disableif': 'Dif', 'grayoutif': 'Gif', 'suppressif': 'Sif'}
DATA_TYPE_DICT = {'BOOLEAN': 1, 'UINT8': 1, 'UINT16': 2, 'UINT32': 4, 'UINT64': 8, 'UINTN': 8, 'CHAR8': 1, 'CHAR16': 2}
SETUP_TYPE_DICT = {'oneof': 5, 'numeric': 0x7, 'numric': 0x7, 'checkbox': 0x6, 'string': 0x8}

FFS_FILE_HEADER_SIZE = 0x18
gEfiFirmwareFileSystemGuid = [0x7A9354D9, 0x0468, 0x444a, 0x81, 0xCE, 0x0B, 0xF6, 0x17, 0xD8, 0x90, 0xDF]
gEfiFirmwareFileSystem2Guid = [0x8c8ce578, 0x8a3d, 0x4f1c, 0x99, 0x35, 0x89, 0x61, 0x85, 0xc3, 0x2d, 0xd3]
gEfiFirmwareFileSystem3Guid = [0x5473c07a, 0x3dcb, 0x4dca, 0xbd, 0x6f, 0x1e, 0x96, 0x89, 0xe7, 0x34, 0x9a]
ZERO_GUID = [0x00000000, 0x0000, 0x0000, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00]
AllFsGuid = [0xFFFFFFFF, 0xFFFF, 0xFFFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF]
BIOS_KNOBS_BIN_FFS_GUID = "615E6021-603D-4124-B7EA-C48A3737BACD"  # Guid of section to be generated
BIOS_KNOBS_BIN_FV_GUID = "A881D567-6CB0-4EEE-8435-2E72D33E45B5"  # Guid of FV to be generated

FV_SIGNATURE = 0x4856465F  # "_FVH" = 0x4856465F

# Executables
GEN_SEC_BIN = "GenSec"
GEN_FFS_BIN = "GenFfs"
if sys.platform == "win32":
    # Append .exe extension to executables if windows OS
    GEN_SEC_BIN = "{}.exe".format(GEN_SEC_BIN)
    GEN_FFS_BIN = "{}.exe".format(GEN_FFS_BIN)

# Scripts
FMMT_SCRIPT ="FMMT"

# Global Values used while parsing
KNOB_OFFSET = 0
KNOB_SIZE = 1
KNOB_EXP = 2
KNOB_IN_XML = 4
FIX_KNOB_NAME = 5
KNOB_TYPE = 6
BIT_MASK = 7
DEPEX_MASK = 8
MASK_VALUES = {1: 0xFFFF, 2: 0xFFFE, 4: 0xFFFC, 8: 0xFFF8}
STRING_KNOB_SIZE = 0x80

InfoList = namedtuple("InfoList", [
    "Offset", "Size", "Depex", "SetupPgPtr",
    "PublishKnobInXml",  # for Publishing Knob in XML
    "FixDuplicateKnobName",  # for Fixing Knob Name (False = there was no knob with the same name in other NVAR struct)
    "SetupType",
    "KnobNameSuffix"  # for Knob Name suffix
])


# ========================
# Common Utility functions
# ========================
def timeit(method):
    def timed(*args, **kw):
        start_time = time.time()
        result = method(*args, **kw)
        end_time = time.time()
        print('[ParseSetup.py:{}] {:.2f} seconds'.format(method.__name__, (end_time - start_time)))
        if 'log_time' in kw:
            name = kw.get('log_name', method.__name__.upper())
            kw['log_time'][name] = int((end_time - start_time) * 1000)
        return result
    return timed


def hex_string(val, zfill=0):
    """Hex string with zeroes filled to occupy specified number of characters

    :param val: input integer value
    :param zfill: total return characters, if less then it will be filled with zeroes
                    zeroes are padded as prefix to the string value
    :return:
    """
    result = "{:x}".format(val).strip().rstrip("L")
    return result.zfill(zfill)


def hex_to_int(buffer_lis):
    """Convert specified hex buffer list to integer

    :param buffer_lis: buffer list of hex values
    :return: integer value
    """
    _bytes = "".join(buffer_lis) if PY2 else bytes(buffer_lis)
    return int(binascii.hexlify(_bytes), 16)


def hexlify(val):
    """Wrapper to binascii.hexlify for python 2 and 3
    compatibility

    :param val: binary data (bytes)
    :return: hex representation of binary data
    """
    if PY2:  # For python 2 string and bytes are not separate class
        return binascii.hexlify(val)
    else:
        return binascii.hexlify(val.encode("utf-8")).decode("utf-8")


def read_list(buffer, offset, size):
    """Read integer value from byte buffer

    :param buffer: byte buffer
    :param offset: offset to start reading from buffer
    :param size: size to be read from buffer
    :return: integer value of specified byte size
    """
    if sys.version_info.major == 2:
        return int(binascii.hexlify(string.join(buffer[offset:offset + size][::-1], '')), 16)
    else:
        return int(binascii.hexlify(bytes(buffer[offset:offset + size][::-1])), 16)


def make_directory(dir_path):
    """Make directory which is to be created

    :param dir_path: directory which is to be created
    :return:
    """
    if not os.path.exists(dir_path):
        os.makedirs(dir_path)
    return dir_path


def fetch_guid(buffer_lis, offset):
    """Read GUID from input buffer

    :param buffer_lis: buffer list
    :param offset: offset/index location where guid is stored
    :return: GUID as a list of integer
    """
    if len(buffer_lis) > (offset + 0x10):
        guid_lis = [
            hex_to_int(buffer_lis[(offset + 0x0):(offset + 0x4)][::-1]),
            hex_to_int(buffer_lis[(offset + 0x4):(offset + 0x6)][::-1]),
            hex_to_int(buffer_lis[(offset + 0x6):(offset + 0x8)][::-1])
        ]
        guid_lis = guid_lis + [hex_to_int(buffer_lis[(offset + i):(offset + i)]) for i in range(0x8, 0x10 + 1)]
    else:
        guid_lis = ZERO_GUID
    return guid_lis


def replace_not_expression(match):
    """Replace matched regex DEPEX with negative
    inverted value

    :param match: regex match
    :return: replace value string for match
    """
    return match.group(1) + INVERT_DICT.get(match.group(2).strip(), '')


def replace_index_value(match):
    """Replace matched regex as Index value

    :param match: regex match
    :return:
    """
    return '_' + str(eval(match.group(1)))


def process_ro_expression(raw_expression, nvar_details, nvar_table, all_nvar_table, missing_nvars, nvar_ro_lis):
    """Process DEPEX for Read-only nvars

    :param raw_expression: Raw Expression
    :param nvar_details: dictionary of Nvar information
    :param nvar_table: dictionary consisting of table of all Nvars
    :param all_nvar_table: dictionary of all Nvars
    :param missing_nvars: dictionary of missing Nvars
    :param nvar_ro_lis: list of read only Nvars
    :return: processed_expression: Processed Expression for read only raw expression
    """
    processed_expression = raw_expression.replace('(', '( ').replace(')', ' )')
    nvar_name_combination = ''
    for match in re.finditer(r"\s*(\S*?)\s*\.\s*(\S*)\s*", processed_expression):
        if match:
            nvar_struct_name = match.group(1).strip()
            nvar_struct_name = nvar_details[nvar_struct_name]
            knob_name = match.group(2).strip().replace('.', '_')
            if nvar_struct_name not in nvar_table:
                if nvar_struct_name not in missing_nvars:
                    missing_nvars[nvar_struct_name] = []
                    missing_nvars[nvar_struct_name].append(knob_name)
                else:
                    if knob_name not in missing_nvars[nvar_struct_name]:
                        missing_nvars[nvar_struct_name].append(knob_name)
            else:
                if nvar_struct_name not in nvar_ro_lis:
                    nvar_ro_lis[nvar_struct_name] = []
                    nvar_ro_lis[nvar_struct_name].append(knob_name)
                else:
                    if knob_name not in nvar_ro_lis[nvar_struct_name]:
                        nvar_ro_lis[nvar_struct_name].append(knob_name)
            nvar_name_combination = nvar_name_combination + '|' + nvar_struct_name
    if nvar_name_combination and nvar_name_combination[0] == '|':
        nvar_name_combination = nvar_name_combination[1:]
    for match in re.finditer(r"\s*(\w+)\s", processed_expression):
        var = match.group(1).strip()
        if var not in MATH_OPERATION:
            try:
                int(var, 16)
                continue
            except ValueError or TypeError:
                pass
            if (var in all_nvar_table) and (var not in missing_nvars):
                missing_nvars[var] = []
                missing_nvars[var].append(var)
    return processed_expression


def replace_depex(match, duplicate_knobs, all_nvars, nvar_details, missing_nvars, all_knobs):
    """Replace Dependency Expression (DEPEX)

    :param match: regex matched DEPEX
    :param duplicate_knobs: dictionary of duplicate knobs
    :param all_nvars: dictionary of all Nvars
    :param nvar_details: dictionary of Nvar information
    :param missing_nvars: dictionary of missing Nvars
    :param all_knobs: dictionary of all knobs
    :return: Replace value for matched DEPEX
    """
    result = None
    if match:
        nvar_struct_name = nvar_details[match.group(1).strip()]
        knob_name = match.group(2).strip()
        result = ' %s ' % knob_name
        if knob_name in duplicate_knobs and nvar_struct_name in duplicate_knobs[knob_name]:
            result = ' Nvar%d_%s ' % (all_nvars[nvar_struct_name]['bin_var_id'], knob_name)
        elif nvar_struct_name in missing_nvars and len(all_knobs[knob_name]) > 1:
            result = ' Nvar%d_%s ' % (all_nvars[nvar_struct_name]['bin_var_id'], knob_name)
    return result.replace('.', '_')


def process_depex(raw_expression, nvar_table, nvar_details, duplicate_knobs, missing_nvars, all_knobs):
    """Process Dependency Expression (DEPEX)

    :param raw_expression: Raw Expression
    :param nvar_table: dictionary consisting of table of all Nvars
    :param nvar_details: dictionary of Nvar information
    :param duplicate_knobs: dictionary of duplicate knobs
    :param missing_nvars: dictionary of missing Nvars
    :param all_knobs: dictionary of all knobs
    :return: Processed DEPEX
    """
    return re.sub(
        r"\s*(\S*?)\s*\.\s*(\S*)\s*",
        lambda match: replace_depex(match, duplicate_knobs, nvar_table, nvar_details, missing_nvars, all_knobs),
        raw_expression)


def process_print_expression(raw_expression):
    """Process Print Expression

    :param raw_expression: Raw Expression
    :return:
    """
    processed_expression = raw_expression.replace('ideqval ', '').replace('ideqvallist ', '_LIST_ ')
    for sign, string_representation in MATH_OPERATION_MAP:
        processed_expression = processed_expression.replace(sign, " {} ".format(string_representation))
    processed_expression = re.sub(r"\s+", ' ', processed_expression)
    processed_expression = re.sub(r"NOT\s*(.*?)(_EQU_|_NEQ_|_LTE_|_GTE_|_LT_|_GT_)",
                                  replace_not_expression, processed_expression)
    return processed_expression


@timeit
def clean_vfr_out_file(vfr_out_path, input_vfr_directories, build_path=None):
    """Clean vfr out files from output directory

    :param vfr_out_path: location to vfr output files
    :param input_vfr_directories: list of directories from where *.i files are to be parsed
    :param build_path: bios output Build directory path
    :return: None
    """
    status = 0
    if input_vfr_directories:
        shutil.rmtree(vfr_out_path)
        if not os.path.exists(vfr_out_path):
            os.mkdir(vfr_out_path)
        for input_dir in input_vfr_directories:
            if os.path.exists(input_dir) and vfr_out_path:
                for filename in glob.glob(os.path.join(input_dir, '*.i')):
                    shutil.copy(filename, vfr_out_path)
    if build_path:
        for root, directory, files in os.walk(build_path):
            for file in glob.glob(os.path.join(root, "*.i")):
                with open(file, "r") as f:
                    line = f.readline().lower().strip()
                    if not line:
                        continue
                    match = re.search(r"\"(.*.vfr)\"", line)
                    if match and (not re.search(r"library", match.group(1))):
                        out_file = os.path.join(vfr_out_path, os.path.splitext(os.path.basename(file))[0] + '.iout')
                        if not os.path.isfile(out_file):
                            shutil.copy(file, out_file)
        status += 1
    for file in glob.iglob(vfr_out_path + "/*.iout"):
        outfile = os.path.splitext(file)[0] + '.int'
        with open(outfile, 'w') as out:
            for line in open(file, 'r').readlines():
                if re.search(r"\s*#\s*pragma\s\s*pack\s*\(\s*(.*?)\s*\)\s*", line):
                    out.write(line)
                    continue
                line = line.split('#')[0]
                line = line.split('//')[0]
                # line = line.strip(' \t')
                if not line.strip():
                    continue
                out.write(line.replace(';', ';\n'))
    status += 1
    for file in glob.iglob(vfr_out_path + "/*.int"):
        outfile = os.path.splitext(file)[0] + '.i'
        with open(outfile, 'w') as out:
            skip_if_true = False
            expression_level = 0
            for line in open(file, 'r').readlines():
                if (not line.strip()) and skip_if_true:
                    continue
                match = re.search(r"\s*(disableif|grayoutif|suppressif)\s\s*(.*?);", line)
                if match:
                    if match.group(2).strip() == 'TRUE':
                        skip_if_true = True
                        expression_level = expression_level + 1
                        continue
                if skip_if_true:
                    if re.search(r"\s*(disableif|grayoutif|suppressif)\s\s*", line):
                        expression_level = expression_level + 1
                        continue
                    if (re.search(r"\s*endif\s*", line)) and (expression_level != 0):
                        expression_level = expression_level - 1
                        if expression_level == 0:
                            skip_if_true = False
                    continue
                out.write(line)
    status += 1
    return status


# Setup File Parsing
def parse_header_file(header_file, nvar_list, nvar_table):
    """Parses Header file for Nvars

    :param header_file: input header file location
    :param nvar_list: dictionary of Nvars
    :param nvar_table: dictionary of Nvar Table
    :return:
    """
    nvar_combination_expression = ""
    if not os.path.isfile(header_file):
        print("All Nvars Enabled for this flow")
    else:
        print("Filtered Knobs Enabled for this flow")
        with open(header_file, "r") as hdr_file:
            table_start = False
            nvar_name = None
            nvar_struct = None
            for line in hdr_file.readlines():
                line = line.split('//')[0].strip()
                if not line:
                    continue
                if not table_start:
                    if re.search(r"\s*VARSTORE_LIST\s\s*mVarstoreList\s*\[\s*\S*\s*\]\s*=", line):
                        table_start = True
                else:
                    if not nvar_struct:
                        match = re.search(r"\s*\"\$(ST|SN)=\s*(.*?)\s*\",", line)
                        if match:
                            nvar_struct = match.group(2)
                            continue
                    if nvar_struct and (not nvar_name):
                        match = re.search(r"\s*\"(.*?)\"\s*,", line)
                        if match:
                            nvar_name = match.group(1)
                            continue
                    if nvar_name:
                        match = re.search(r"\s*{\s*(.*?)\s*}\s*,", line)
                        if match:
                            nvar_guid_str = match.group(1)
                            continue

                    if nvar_name and re.search(r"\s*}\s*,", line):
                        nvar_struct = nvar_struct if nvar_struct else nvar_name
                        nvar_list[nvar_name] = nvar_struct
                        nvar_list[nvar_struct] = nvar_struct
                        nvar_table[nvar_struct] = {'NvarName': nvar_name, 'NvarGuidStr': nvar_guid_str,
                                                   'NvarSize': 0, 'KnobCount': 0, 'ExpCount': 0, 'DupKnobCount': 0,
                                                   'VarNamesDict': {}, 'DupExp': {}, 'NvarStructDone': False,
                                                   'bin_var_id': 0xFF}
                        if nvar_combination_expression:
                            nvar_combination_expression = "{}|{}|{}".format(nvar_combination_expression, nvar_name,
                                                                           nvar_struct)
                        else:
                            nvar_combination_expression = "{}|{}".format(nvar_name, nvar_struct)
                        print("Enabled Nvar for XML CLI knobs = L'{}'".format(nvar_table[nvar_struct]['NvarName']))
                        nvar_name = ""
                        nvar_struct = ""
                    if re.search(r"\s*}\s*;", line):
                        table_start = False

    return nvar_combination_expression


def construct_nvar_buffer(sorted_lis, skip_depex, nvar_details, nvar_table, all_nvars, duplicate_knobs, missing_nvars, all_knobs):
    nvar_id_bin = 0
    result_buffer = b""
    for varstore_struct_name in sorted_lis:
        buffer_str = ""
        if skip_depex:
            nvar_table[varstore_struct_name]['DupKnobCount'] = 0
        bit_knobs = 0
        for bios_knob_name in nvar_table[varstore_struct_name]['VarNamesDict']:
            if bios_knob_name == 'Temp__Struct__Size':
                continue
            if nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_IN_XML]:
                bin_knob_name = bios_knob_name
                knob_offset = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_OFFSET]
                knob_size = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_SIZE]
                knob_type = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_TYPE]
                try:
                    depex_mask = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][DEPEX_MASK]
                except:
                    depex_mask = 0x7
                if knob_type == SETUP_TYPE_DICT.get("string", 0):
                    type_size_bin = hex(((knob_type & 0x8) << 4) + (int(knob_size / 2) & 0x7F))[2:].strip()[0:2]
                    bin_knob_name = bin_knob_name.replace("_0", "")
                else:
                    bit_mask = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][BIT_MASK]
                    if bit_mask != "":
                        bit_knobs = bit_knobs + 1
                        bit_mask = bit_mask.split('_')
                        bit_offset = int(bit_mask[0], 16)
                        bit_size = int(bit_mask[1], 16)
                        knob_offset = knob_offset + int(bit_offset/8)
                        bit_data = (((bit_size & 0x1F) << 3) + ((bit_offset % 8) & 0x7)) & 0xFF  # bit_size[7:3] bit_offset[2:0]
                        knob_size = 0xC + ((bit_size >> 5) & 1)  # bit_size_BIT05[1:0]
                        type_size_bin = (hex(((knob_type & depex_mask) << 4) + (knob_size & 0xF))[2:].strip()+'%02X'%bit_data)[0:4]
                    else:
                        type_size_bin = hex(((knob_type & depex_mask) << 4) + (knob_size & 0xF))[2:].strip()[0:2]
                bin_offset = hex_string(knob_offset, zfill=4)
                if nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][FIX_KNOB_NAME]:
                    bin_knob_name = "Nvar%d_%s" % (nvar_id_bin, bin_knob_name)
                depex = nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_EXP].strip()
                if (depex != "TRUE") and (not skip_depex):
                    depex = process_depex(depex, all_nvars, nvar_details, duplicate_knobs, missing_nvars, all_knobs)
                if sys.version_info.major == 2:
                    bin_name = binascii.hexlify(bin_knob_name).strip()
                    if (depex == "TRUE") or skip_depex:
                        buffer_str = buffer_str + bin_offset[2:4] + bin_offset[0:2] + type_size_bin + bin_name + '00' + '00'
                    else:
                        ExpBin = binascii.hexlify(depex).strip()
                        buffer_str = buffer_str + bin_offset[2:4] + bin_offset[0:2] + type_size_bin + bin_name + '00' + ExpBin + '00'
                else:
                    bin_name = binascii.hexlify(bin_knob_name.strip().encode('utf-8'))
                    if (depex == "TRUE") or skip_depex:
                        buffer_str = buffer_str + bin_offset[2:4] + bin_offset[0:2] + type_size_bin + bin_name.decode("utf-8") + '00' + '00'
                    else:
                        ExpBin = binascii.hexlify(depex.strip().encode('utf-8'))
                        buffer_str = buffer_str + bin_offset[2:4] + bin_offset[0:2] + type_size_bin + bin_name.decode("utf-8") + '00' + ExpBin.decode("utf-8") + '00'

        knob_hex_buffer = binascii.unhexlify(buffer_str)
        buffer_str = ""
        for DupCnt in range(0, nvar_table[varstore_struct_name]['DupKnobCount']):
            depex = nvar_table[varstore_struct_name]['DupExp'][DupCnt][1]
            if (depex != "TRUE") and (not skip_depex):
                depex = process_depex(depex, all_nvars, nvar_details, duplicate_knobs, missing_nvars, all_knobs)
            bin_name = hexlify(nvar_table[varstore_struct_name]['DupExp'][DupCnt][0])
            ExpBin = hexlify(depex)
            buffer_str = buffer_str + bin_name.strip() + '00' + ExpBin.strip() + '00'
        if nvar_table[varstore_struct_name]['DupKnobCount'] > 1:
            duplicate_knob_hex_buffer = binascii.unhexlify(buffer_str)
            duplicate_knob_hex_buffer_size = len(duplicate_knob_hex_buffer)
        else:
            duplicate_knob_hex_buffer_size = 0
        knob_hex_buffer_size = len(knob_hex_buffer)
        knob_count = hex(nvar_table[varstore_struct_name]['KnobCount'])[2:].strip().strip('L').zfill(4)
        bit_knob_count = hex(bit_knobs)[2:].strip().strip('L').zfill(4)
        duplicate_knob_count = hex(knob_hex_buffer_size + BIOS_KNOBS_DATA_BIN_HDR_SIZE)[2:].strip().strip('L').zfill(6)
        nvar_packet_size = hex(knob_hex_buffer_size + duplicate_knob_hex_buffer_size + BIOS_KNOBS_DATA_BIN_HDR_SIZE)[2:].strip().strip('L').zfill(6)
        nvar_size = hex(nvar_table[varstore_struct_name]['NvarSize'])[2:].strip().strip('L').zfill(4)
        if len(nvar_table[varstore_struct_name]['NvarName']) >= BIOS_KNOB_BIN_NVAR_NAME_RESERVED:
            print("Nvar Name L\"{}\" too long, please request your Xmlcli engineer to reserve more space in the "
                  "Nvar Header \"KNOB_BIN_HDR\"".format(nvar_table[varstore_struct_name]['NvarName']))
        else:
            nvar_name = hexlify(nvar_table[varstore_struct_name]['NvarName']) + hex(0)[2:].strip().zfill((BIOS_KNOB_BIN_NVAR_NAME_RESERVED - len(nvar_table[varstore_struct_name]['NvarName']))*2)
            temp_guid = nvar_table[varstore_struct_name]['NvarGuidStr'].replace('{', '').replace('}', '').strip().split(',')
            temp_guid[0] = temp_guid[0].strip()[2:].zfill(8)
            temp_guid[1] = temp_guid[1].strip()[2:].zfill(4)
            temp_guid[2] = temp_guid[2].strip()[2:].zfill(4)
            nvar_guid = (temp_guid[0][6:8] + temp_guid[0][4:6] + temp_guid[0][2:4] + temp_guid[0][0:2]
                         + temp_guid[1][2:4] + temp_guid[1][0:2] + temp_guid[2][2:4] + temp_guid[2][0:2]
                         + temp_guid[3].strip()[2:].zfill(2) + temp_guid[4].strip()[2:].zfill(2)
                         + temp_guid[5].strip()[2:].zfill(2) + temp_guid[6].strip()[2:].zfill(2)
                         + temp_guid[7].strip()[2:].zfill(2) + temp_guid[8].strip()[2:].zfill(2)
                         + temp_guid[9].strip()[2:].zfill(2) + temp_guid[10].strip()[2:].zfill(2))
            if nvar_id_bin != nvar_table[varstore_struct_name]['bin_var_id']:
                print("This is a Nvar Id problem %s" % varstore_struct_name)
            nvar_buffer = (hexlify("$NVAR") + hex_string(nvar_id_bin, 2)
                           + knob_count[2:4] + knob_count[0:2]
                           + duplicate_knob_count[4:6] + duplicate_knob_count[2:4] + duplicate_knob_count[0:2]
                           + nvar_packet_size[4:6] + nvar_packet_size[2:4] + nvar_packet_size[0:2]
                           + NVAR_NAME_OFFSET_IN_HDR_HEX[0:2] + BIOS_KNOB_BIN_REVISION_HEX[0:2]
                           + nvar_size[2:4] + nvar_size[0:2] + nvar_guid + nvar_name + bit_knob_count[2:4] + bit_knob_count[0:2])
            nvar_id_bin = nvar_id_bin + 1
            result_buffer = result_buffer + binascii.unhexlify(nvar_buffer)
            if knob_hex_buffer_size > 4:
                result_buffer = result_buffer + knob_hex_buffer
            if (duplicate_knob_hex_buffer_size > 2) and (not skip_depex):
                result_buffer = result_buffer + duplicate_knob_hex_buffer
    return result_buffer, nvar_id_bin


def depex_evaluate(read_only_knobs, missing_nvars, nvar_id_bin, nvar_details, nvar_table, all_nvars, all_knobs):
    result_buffer = b""
    for read_only_var_count in range(0, 2):
        read_only_nvars = read_only_knobs if read_only_var_count == 0 else missing_nvars
        for current_nvar in sorted(read_only_nvars, reverse=True):
            buffer_str = ""
            if len(read_only_nvars[current_nvar]) == 0:
                # print ("skipping since its Empty, Nvar = %s" %all_nvars[current_nvar]['var_name'])
                continue
            if current_nvar in nvar_table:
                nvar_bin_var_id = nvar_table[current_nvar]['bin_var_id']
            else:
                nvar_bin_var_id = nvar_id_bin
                nvar_id_bin = nvar_id_bin + 1
            if all_nvars[nvar_details[current_nvar]]['bin_var_id'] != nvar_bin_var_id:
                print("Something is wrong here for %s" % current_nvar)
            bit_knobs = 0
            for knob in read_only_nvars[current_nvar]:
                knob_prefix = "Nvar{}_".format(nvar_bin_var_id) if read_only_var_count == 1 and knob in all_knobs and len(all_knobs[knob]) > 1 else ""
                this_knob = "{}{}".format(knob_prefix, knob)
                knob_offset = all_nvars[current_nvar]['structure'][knob][KNOB_OFFSET]
                knob_size = all_nvars[current_nvar]['structure'][knob][KNOB_SIZE]
                bit_mask = all_nvars[current_nvar]['structure'][knob][BIT_MASK]
                if bit_mask != "":
                    bit_knobs = bit_knobs + 1
                    bit_mask = bit_mask.split('_')
                    bit_offset = int(bit_mask[0], 16)
                    bit_size = int(bit_mask[1], 16)
                    knob_offset = knob_offset + int(bit_offset/8)
                    bit_data = (((bit_size & 0x1F) << 3) + ((bit_offset % 8) & 0x7)) & 0xFF  # bit_size[7:3] bit_offset[2:0]
                    knob_size = 0xC + ((bit_size >> 5) & 1)  # Bitsize_BIT05[1:0]
                    type_size_bin = (hex(0x70 + (knob_size & 0xF))[2:].strip()+'%02X'%bit_data)[0:4]
                else:
                    type_size_bin = hex(0x70 + knob_size)[2:].strip()[0:2]
                knob_offset = hex(knob_offset)[2:].strip().zfill(4)
                name_binary = hexlify("{}".format(this_knob)).strip()
                buffer_str = (buffer_str + knob_offset[2:4] + knob_offset[0:2] + type_size_bin + name_binary + '00' + '00')

            knob_hex_buffer = binascii.unhexlify(buffer_str)
            nvar_name_padding = (BIOS_KNOB_BIN_NVAR_NAME_RESERVED - len(all_nvars[current_nvar]['VarName'])) * 2
            nvar_name_str = hexlify(all_nvars[current_nvar]['VarName']) + hex(0)[2:].strip().zfill(nvar_name_padding)
            temp_guid = all_nvars[current_nvar]['Guid'].replace('{', '').replace('}', '').strip().split(',')
            temp_guid[0] = temp_guid[0].strip()[2:].zfill(8)
            temp_guid[1] = temp_guid[1].strip()[2:].zfill(4)
            temp_guid[2] = temp_guid[2].strip()[2:].zfill(4)
            nvar_guid = (temp_guid[0][6:8] + temp_guid[0][4:6] + temp_guid[0][2:4] + temp_guid[0][0:2]
                         + temp_guid[1][2:4] + temp_guid[1][0:2] + temp_guid[2][2:4] + temp_guid[2][0:2]
                         + temp_guid[3].strip()[2:].zfill(2) + temp_guid[4].strip()[2:].zfill(2)
                         + temp_guid[5].strip()[2:].zfill(2) + temp_guid[6].strip()[2:].zfill(2)
                         + temp_guid[7].strip()[2:].zfill(2) + temp_guid[8].strip()[2:].zfill(2)
                         + temp_guid[9].strip()[2:].zfill(2) + temp_guid[10].strip()[2:].zfill(2))
            knob_buffer_size = len(knob_hex_buffer)
            knob_count_str = hex_string(len(read_only_nvars[current_nvar]), zfill=4)
            bit_knob_count = hex(bit_knobs)[2:].strip().strip('L').zfill(4)
            duplicate_knob_count = hex_string(knob_buffer_size + BIOS_KNOBS_DATA_BIN_HDR_SIZE, zfill=6)
            nvar_packet_size = hex_string(knob_buffer_size + BIOS_KNOBS_DATA_BIN_HDR_SIZE, zfill=6)
            nvar_size = hex_string(all_nvars[current_nvar]['Size'], zfill=4)
            if nvar_bin_var_id != all_nvars[nvar_details[current_nvar]]['bin_var_id']:
                print("This is a Nvro Id problem {}".format(current_nvar))
            nvar_buffer_str = (hexlify("$NVRO")
                               + hex(nvar_bin_var_id)[2:].strip().zfill(2)
                               + knob_count_str[2:4] + knob_count_str[0:2]
                               + duplicate_knob_count[4:6] + duplicate_knob_count[2:4] + duplicate_knob_count[0:2]
                               + nvar_packet_size[4:6] + nvar_packet_size[2:4] + nvar_packet_size[0:2]
                               + NVAR_NAME_OFFSET_IN_HDR_HEX[0:2] + BIOS_KNOB_BIN_REVISION_HEX[0:2]
                               + nvar_size[2:4] + nvar_size[0:2] + nvar_guid + nvar_name_str + bit_knob_count[2:4] + bit_knob_count[0:2])

            result_buffer = result_buffer + binascii.unhexlify(nvar_buffer_str)
            if knob_buffer_size > 4:
                result_buffer = result_buffer + knob_hex_buffer
    return result_buffer, nvar_id_bin


def get_pragma_packed_alignment(pragma_match):
    """Get alignment to perform based on pragma pack matched

    :param pragma_match: pragma regex matched to be processed
    :return: tuple of whether it is pragma packed or not and pack alignment
             i.e. (bool, integer) ==> (False, 8)
    """
    pragma_packed = False
    pack_alignment = 8
    arg_string = pragma_match.group(1).strip()
    if arg_string == "1":
        pragma_packed = True
    elif arg_string in ("2", "4", "8"):
        pragma_packed = False
        pack_alignment = int(arg_string)
    else:
        arg_match = re.search(r"push\s*,\s*(\S*)", arg_string)
        if arg_match and arg_match.group(1) == "1":
            pragma_packed = True
        elif arg_match and arg_match.group(1) in ("2", "4", "8"):
            pragma_packed = False
            pack_alignment = int(arg_match.group(1))

    return pragma_packed, pack_alignment


def handle_struct(struct_match, is_pragma_packed, pack_alignment, structure, var_offset, header_file, all_nvars, nvar_table, all_knobs):
    """Handle Structure parsing

    :param struct_match: regex match for structure
    :param is_pragma_packed: specify whether structure is pragma packed or not
    :param pack_alignment: specify pack alignment
    :param structure: dictionary of structure
    :param var_offset: Variable Offset
    :param header_file: path to the header file location
    :param all_nvars: dictionary of all the nvars
    :param nvar_table:dictionary of the nvar table
    :param all_knobs: dictionary of all the knobs
    :return: whether structure is as nvar or not

        Just a hint for a beginner to Python: `structure` variable is dictionary which is mutable data in python
        which means python uses same memory reference for the dictionary when you pass it to the function;
        same applies for `all_nvars` and `nvar_table`
        ==> For simplicity one can return all these 3 variables and handle as output result
    """
    struct_name = struct_match.group(1)
    struct_as_nvar = False
    if not is_pragma_packed:
        var_offset = (var_offset + (pack_alignment - 1)) & MASK_VALUES[pack_alignment]
    if struct_name not in all_nvars:
        for ThisMember in structure:
            all_knobs.setdefault(ThisMember, []).append(struct_name)
        all_nvars[struct_name] = {'structure': structure, 'Size': var_offset, 'var_name': '', 'Guid': '', 'bin_var_id': 0xFF}
    if os.path.isfile(header_file) and struct_name in nvar_table and not nvar_table[struct_name]['NvarStructDone']:
        nvar_table[struct_name]['VarNamesDict'] = structure
        nvar_table[struct_name]['NvarStructDone'] = True
        nvar_table[struct_name]['NvarSize'] = var_offset
        struct_as_nvar = True
    if not struct_as_nvar:
        structure["Temp__Struct__Size"] = var_offset
    return struct_as_nvar


def handle_struct_member(line, structure, var_offset, bit_wise, bit_offset, bitwise_varsize, temp_struct, is_pragma_packed, pack_alignment):
    """Parse Structure for Structure members and update the dictionary of structure

    :param line: line to be parsed
    :param structure: structure dictionary (input as well as output).. will be updated as python uses same
                        reference to dict object
    :param var_offset: offset value of variable
    :param bit_wise: specify whether the operation is bit wise or not
    :param bit_offset: offset bit
    :param temp_struct: temporary structure dictionary
    :param is_pragma_packed: specify whether pragma packed structure or not
    :param pack_alignment: pragma pack alignment of structure
    :return: Updated values of `structure`, `var_offset`, `bit_wise`, `bit_offset`
    """
    standard_data_type = False
    bit_size = 0
    var_size = 1
    for match in re.finditer(r"\s*(BOOLEAN|UINT\S*|CHAR\S*)\s\s*(.*?);", line):
        if match:
            var_size = (DATA_TYPE_DICT[match.group(1)] & 0xF)
            var_name = match.group(2).strip()
            total_elements = 1
            is_array_var = False
            array_match = re.search(r"(\S*)\s*:\s*(\S*)\s*", var_name)
            if array_match:
                var_name = array_match.group(1)
                bit_size = int(array_match.group(2))
                bit_wise = True
                if ((bitwise_varsize != var_size) and (bit_offset != 0)):
                    var_offset = var_offset + bitwise_varsize
                    bit_offset = 0
                bitwise_varsize = var_size
            else:
                if bit_wise:
                    bit_wise = False
                    bit_offset = 0
                    var_offset = var_offset + bitwise_varsize
            if (not is_pragma_packed) and (var_size != 1):
                var_offset = (var_offset + (var_size - 1)) & MASK_VALUES[var_size]
            array_match = re.search(r"(\S*)\s*\[\s*(\S*)\s*\]", var_name)
            if array_match:
                var_name = array_match.group(1)
                total_elements = eval(array_match.group(2))
                is_array_var = True
            for count in range(0, total_elements):
                if bit_wise:
                    if ((var_size*8) < bit_offset + bit_size):
                        bit_offset = 0
                        var_offset = var_offset + var_size
                    bit_mask = "%X_%X" %(bit_offset, bit_size)
                    bit_offset = bit_offset + bit_size
                else:
                    bit_mask = ""
                info_list = InfoList(var_offset, var_size, "", "", False, False, 0, bit_mask)
                if is_array_var:
                    structure["{}_{}".format(var_name, count)] = list(info_list)
                else:
                    structure[var_name] = list(info_list)
                if not bit_wise:
                    var_offset = var_offset + var_size
            standard_data_type = True
    if not standard_data_type:
        match = re.search(r"\s*(\S*)\s\s*(.*?);", line)
        if match:
            parent = match.group(1)
            child_name = match.group(2)
            if temp_struct.get(parent, None):
                if not is_pragma_packed:
                    var_offset = (var_offset + (pack_alignment - 1)) & MASK_VALUES[pack_alignment]
                is_array_var = False
                total_elements = 1
                array_match = re.search(r"(\S*)\s*\[\s*(\S*)\s*\]", child_name)
                if array_match:
                    total_elements = eval(array_match.group(2))
                    is_array_var = True
                for count in range(0, total_elements):
                    if is_array_var:
                        child_name = "%s_%s" % (array_match.group(1), count)
                    for member in temp_struct[parent]:
                        if member == "Temp__Struct__Size":
                            continue
                        info_list = InfoList(temp_struct[parent][member][KNOB_OFFSET] + var_offset,
                                             temp_struct[parent][member][KNOB_SIZE], "", "", False, False, 0,
                                             temp_struct[parent][member][BIT_MASK])
                        nested_member_name = "%s_%s" % (child_name, member)
                        structure[nested_member_name] = list(info_list)
                    var_offset = var_offset + temp_struct[parent]["Temp__Struct__Size"]

    return structure, var_offset, bit_wise, bit_offset, var_size, bitwise_varsize


def handle_expression(line, expression, nvar_details, expression_level, published_knobs, nvar_table, all_knobs, final_expression_name, duplicate_knobs, previous_line):
    """Parse the Expression line and update the dictionaries for nvar, knobs and final expression

    :param line: input line to be parse
    :param expression: nvar combination expression for line to be parsed for
    :param nvar_details: dictionary consisting of details of nvar
    :param expression_level: level of expression
    :param published_knobs: dictionary consisting of published knobs
    :param nvar_table: nvar table dictionary
    :param all_knobs: dictionary containing all the knobs
    :param final_expression_name: final expression name
    :param duplicate_knobs: dictionary consisting duplicate knobs
    :param previous_line: previously parsed line
    :return: None
            but python updates the dictionaries:
            `nvar_details`, `nvar_table`, `all_knobs`, `duplicate_knobs`, `previous_line`

    """
    for match in re.finditer(r"(\s*(\S*)\s*varid\s*=\s*)({})\s*\.\s*(\S*)\s*,".format(expression), line):
        if match:
            _type = match.group(2)
            nvar_type = match.group(3)
            if nvar_type in nvar_details:
                varstore_struct_name = nvar_details[nvar_type]
            else:
                continue
            org_variable = match.group(4).replace(".", "_")
            if _type == "":
                match_type = re.search(r"\s*(numric|numeric|oneof|checkbox|string)\s*", previous_line)
                if match_type:
                    _type = match_type.group(1)
                else:
                    continue
            elif _type in ("password", "date", "time"):
                continue
            if _type == "string":
                variable = org_variable + '_0'
            else:
                variable = re.sub(r"\[\s*(.*?)\s*\]", replace_index_value, org_variable)
            final_expression = "TRUE"
            if expression_level > 0:
                final_expression = process_print_expression(final_expression_name[1])
                if (expression_level > 1) and (final_expression.replace(" ", "") != "(FALSE)"):
                    for ExpCnt in range(2, (expression_level + 1)):
                        current_expression = process_print_expression(final_expression_name[ExpCnt])
                        if current_expression.replace(" ", "") == "(FALSE)":
                            final_expression = "( FALSE )"
                            break
                        if current_expression not in final_expression:
                            final_expression = final_expression + " _AND_ " + current_expression
                if ((final_expression.replace(" ", "") == "(FALSE)")
                    or (final_expression.replace(" ", "") == "Sif(TRUE)")):
                    continue
                final_expression = re.sub(r"\[\s*(.*?)\s*\]", replace_index_value, final_expression)
            if (varstore_struct_name, variable) in published_knobs:
                knob_published = published_knobs[varstore_struct_name, variable]
                if expression_level > 0:
                    nvar_table[varstore_struct_name]['ExpCount'] = nvar_table[varstore_struct_name]['ExpCount'] + 1
                else:
                    final_expression = "TRUE"
                duplicate_idx = nvar_table[varstore_struct_name]['DupKnobCount']
                nvar_table[varstore_struct_name]['DupExp'][duplicate_idx] = [variable, final_expression]
                nvar_table[varstore_struct_name]['DupKnobCount'] = duplicate_idx + 1
                published_knobs[varstore_struct_name, variable] = knob_published + 1
            else:
                published_knobs[varstore_struct_name, variable] = 1
                if expression_level > 0:
                    nvar_table[varstore_struct_name]['ExpCount'] = nvar_table[varstore_struct_name]['ExpCount'] + 1
                else:
                    final_expression = "TRUE"
                nvar_table[varstore_struct_name]['KnobCount'] = nvar_table[varstore_struct_name]['KnobCount'] + 1
                nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_EXP] = final_expression
                nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_IN_XML] = True
                nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_TYPE] = SETUP_TYPE_DICT.get(_type, 0)
                if _type == "string":
                    nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_SIZE] = (0x7F * 2)
                    for string_knob_size in range(1, STRING_KNOB_SIZE):
                        _variable = "{}_{}".format(org_variable, string_knob_size)
                        if _variable not in nvar_table[varstore_struct_name]['VarNamesDict']:
                            nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_SIZE] = (
                                    string_knob_size * 2)
                            break
                if len(all_knobs[variable]) > 1:
                    if variable in duplicate_knobs:
                        nvar_table[varstore_struct_name]['VarNamesDict'][variable][FIX_KNOB_NAME] = True
                    else:
                        duplicate_knobs[variable] = copy.deepcopy(all_knobs[variable])
                        duplicate_knobs[variable].remove(varstore_struct_name)
    return expression_level


@timeit
def parse_setup_files(vfr_output_file_path, header_file_path, knobs_bin_file, skip_depex=False):
    """Parse Setup Files from Build Folder

    :param vfr_output_file_path: vfr out put files
    :param header_file_path: header file location
    :param knobs_bin_file: bios knob data bin file location
    :param skip_depex: specify whether to skip depex check or not
    :return:
    """
    final_expression_name = {}
    temp_struct = {}
    nvar_table = {}
    all_nvars = {}
    all_knobs = {}
    duplicate_knobs = {}
    nvar_details = {}
    missing_nvars = {}
    read_only_nvars = {}
    published_knobs = {}
    varstore_start = False
    expression_level = 0
    output_path = os.path.dirname(header_file_path)
    var_offset = 0
    var_size = 1
    structure_start = False
    typedef_start = False
    is_pragma_packed = False
    pack_alignment = 8

    nvar_combination_expression = parse_header_file(header_file=header_file_path, nvar_list=nvar_details, nvar_table=nvar_table)

    final_expression_name[expression_level] = "TRUE"
    for vfr_output_file in glob.iglob(vfr_output_file_path + "/*.i"):
        match = re.search(r".*[\\\/](\S*).i", vfr_output_file)
        if match:
            print("Parsing Hii elements from %s.i" % match.group(1))
        current_line = ""
        remove_expression = False
        expression_level = 0
        goto_expression_prefix = {}
        varstore_struct_name = ''
        structure = {}
        bit_wise = False
        bit_offset = 0
        bitwise_varsize = 1
        for line in open(vfr_output_file, "r").readlines():
            if not structure_start:  # Validate Pragma match
                match = re.search(r"\s*#\s*pragma\s\s*pack\s*\(\s*(.*?)\s*\)\s*", line)
                if match:
                    is_pragma_packed, pack_alignment = get_pragma_packed_alignment(pragma_match=match)
                    continue

            # Start Processing lines
            line = line.split('#')[0].split('//')[0].strip()
            if not line:  # Skip empty lines
                continue
            previous_line = current_line
            current_line = line

            # Parse Structure
            # i.e. typedef struct {
            match = re.search(r"\s*typedef\s\s*struct\s*", line)
            if match:  # structure found
                structure_start = True
                bit_wise = False
                bit_offset = 0
                structure = {}
                continue
            else:
                if re.search(r"\s*typedef\s*", line):
                    typedef_start = True
                    continue
                if typedef_start and re.search(r"\s*struct\s*", line):
                    typedef_start = False
                    structure_start = True
                    bit_wise = False
                    bit_offset = 0
                    structure = {}
                    continue

            if structure_start:
                struct_match = re.search(r"\s*}\s*(\S*)\s*;", line)
                if struct_match:
                    structure_start = False
                    if bit_wise:
                        bit_wise = False
                        bit_offset = 0
                        var_offset = var_offset + var_size
                    # dictionaries, structure, all_nvars, nvar_table and all_knobs will be updated
                    struct_as_nvar = handle_struct(struct_match, is_pragma_packed, pack_alignment, structure, var_offset,
                                                   header_file=header_file_path, all_nvars=all_nvars,
                                                   nvar_table=nvar_table, all_knobs=all_knobs)
                    if struct_match.group(1) and not struct_as_nvar:
                        temp_struct[struct_match.group(1)] = structure
                    var_offset = 0
                    continue
                else:
                    structure, var_offset, bit_wise, bit_offset, var_size, bitwise_varsize = handle_struct_member(line, structure, var_offset, bit_wise, bit_offset,bitwise_varsize, temp_struct, is_pragma_packed, pack_alignment)

            # Parse Var Store Example lines listed below:
            # efivarstore SA_SETUP, varid = 2, attribute = 0x7, name = SaSetup, guid = { 0x72c5e28c, 0x7783, 0x43a1, { 0x87, 0x67, 0xfa, 0xd7, 0x3f, 0xcc, 0xaf, 0xa4 } };
            # varstore SYSTEM_ACCESS, varid = 0xF000, name = SystemAccess, guid = { 0xE770BB69, 0xBCB4, 0x4D04, { 0x9E, 0x97, 0x23, 0xFF, 0x94, 0x56, 0xFE, 0xAC }};
            match = re.search(r"\s*(efivarstore|varstore)\s*(.*?)\s*,", line)
            if match:
                current_struct_var_name = match.group(2).strip()
                if current_struct_var_name in all_nvars:
                    varstore_start = True
                    varstore_struct_name = current_struct_var_name
                elif current_struct_var_name in DATA_TYPE_DICT:
                    varstore_struct_name = current_struct_var_name
                    varstore_start = True

            if varstore_start:
                match = re.search(r"\s*name\s*=\s*(.*?)\s*,\s*guid\s*=\s*(.*?)\s*;", line)
                if match:
                    if varstore_struct_name in DATA_TYPE_DICT:
                        all_nvars[match.group(1).strip()] = {}
                        all_nvars[match.group(1).strip()]['Size'] = DATA_TYPE_DICT[varstore_struct_name]
                        varstore_struct_name = match.group(1).strip()
                        info_list = InfoList(0, all_nvars[varstore_struct_name]['Size'], "", "", False, False, 0, '')
                        structure[varstore_struct_name] = list(info_list)
                        all_nvars[varstore_struct_name]['structure'] = {}
                        all_nvars[varstore_struct_name]['structure'][varstore_struct_name] = list(info_list)
                    all_nvars[varstore_struct_name]['VarName'] = match.group(1).strip()
                    all_nvars[varstore_struct_name]['Guid'] = match.group(2).strip()
                    if varstore_struct_name not in nvar_table:
                        nvar_details[all_nvars[varstore_struct_name]['VarName']] = varstore_struct_name
                        nvar_details[varstore_struct_name] = varstore_struct_name
                        if not os.path.isfile(header_file_path):
                            nvar_table[varstore_struct_name] = {
                                'NvarName': all_nvars[varstore_struct_name]['VarName'],
                                'NvarGuidStr': all_nvars[varstore_struct_name]['Guid'],
                                'NvarSize': all_nvars[varstore_struct_name]['Size'], 'KnobCount': 0, 'ExpCount': 0,
                                'DupKnobCount': 0, 'VarNamesDict': all_nvars[varstore_struct_name]['structure'],
                                'DupExp': {}, 'NvarStructDone': True, 'bin_var_id': 0xFF}
                            nvar_combination_expression = "{}{}{}|{}".format(
                                nvar_combination_expression, "|" if nvar_combination_expression else "",
                                nvar_table[varstore_struct_name]['NvarName'], varstore_struct_name)
                    varstore_start = False
                    continue
                else:
                    match = re.search(r"\s*name\s*=\s*(.*?)\s*,", line)
                    if match:
                        if varstore_struct_name in DATA_TYPE_DICT:
                            all_nvars[match.group(1).strip()] = {}
                            all_nvars[match.group(1).strip()]['Size'] = DATA_TYPE_DICT[varstore_struct_name]
                            varstore_struct_name = match.group(1).strip()
                            info_list = InfoList(0, all_nvars[varstore_struct_name]['Size'], "", "", False, False, 0, "")
                            structure[varstore_struct_name] = list(info_list)
                            all_nvars[varstore_struct_name]['structure'] = {}
                            all_nvars[varstore_struct_name]['structure'][varstore_struct_name] = list(info_list)
                        all_nvars[varstore_struct_name]['VarName'] = match.group(1).strip()
                        continue
                    match = re.search(r"\s*guid\s*=\s*(.*?)\s*;", line)
                    if match:
                        all_nvars[varstore_struct_name]['Guid'] = match.group(1).strip()
                        varstore_start = False
                        if varstore_struct_name not in nvar_table:
                            nvar_details[all_nvars[varstore_struct_name]['VarName']] = varstore_struct_name
                            nvar_details[varstore_struct_name] = varstore_struct_name
                            if not os.path.isfile(header_file_path):
                                nvar_table[varstore_struct_name] = {
                                    'NvarName': all_nvars[varstore_struct_name]['VarName'],
                                    'NvarGuidStr': all_nvars[varstore_struct_name]['Guid'],
                                    'NvarSize': all_nvars[varstore_struct_name]['Size'], 'KnobCount': 0,
                                    'ExpCount': 0, 'DupKnobCount': 0,
                                    'VarNamesDict': all_nvars[varstore_struct_name]['structure'], 'DupExp': {},
                                    'NvarStructDone': True, 'bin_var_id': 0xFF}
                                nvar_combination_expression = "{}{}{}|{}".format(
                                    nvar_combination_expression, "|" if nvar_combination_expression else "",
                                    nvar_table[varstore_struct_name]['NvarName'], varstore_struct_name)
                        continue

            if remove_expression:
                match = re.search(r"\s*(.*?);", line)
                if match:
                    final_expression_name[expression_level] = final_expression_name[expression_level] + ' ' + match.group(1).strip() + ')'
                    remove_expression = False
                    continue
                else:
                    match = re.search(r"\s*(.*)\s*", line)
                    if match:
                        final_expression_name[expression_level] = final_expression_name[expression_level] + ' ' + match.group(1)
                        continue
            else:
                match = re.search(r"\s*(disableif|grayoutif|suppressif)\s\s*(.*?);", line)
                if match:
                    expression_level = expression_level + 1
                    operation = OPERATION_DICT.get(match.group(1).strip(), '??')
                    final_expression_name[expression_level] = operation + '(' + match.group(2).strip() + ')'
                    remove_expression = False
                    continue
                else:
                    match = re.search(r"\s*(disableif|grayoutif|suppressif)\s\s*(.*)\s*", line)
                    if match:
                        expression_level = expression_level + 1
                        operation = OPERATION_DICT.get(match.group(1).strip(), '??')
                        final_expression_name[expression_level] = operation + '(' + match.group(2).strip()
                        remove_expression = True
                        continue

            for match in re.finditer(r"\s*endif\s*", line):
                if match and (expression_level != 0):
                    final_expression_name.pop(expression_level)
                    expression_level = expression_level - 1

            if expression_level == 0:
                final_expression_name = {0: "TRUE"}

            match = re.search(r"\s*goto\s*(.*?)\s*,", line)
            if match:
                goto_form_id = match.group(1).strip()
                if expression_level > 0:
                    goto_expression_prefix[goto_form_id] = final_expression_name.copy()

            match = re.search(r"\s*form\s*formid\s*=\s*(.*?)\s*,", line)
            if match:
                current_form_id = match.group(1).strip()
                expression_level = 0
                final_expression_name = {0: "TRUE"}
                if current_form_id in goto_expression_prefix:
                    expression_level = len(goto_expression_prefix[current_form_id]) - 1
                    if expression_level > 0:
                        final_expression_name = goto_expression_prefix[current_form_id].copy()

            if re.search(r"\s*endform\s*;\s*", line):
                expression_level = 0
                final_expression_name = {0: "TRUE"}

            handle_expression(line, nvar_combination_expression, nvar_details, expression_level, published_knobs,
                              nvar_table, all_knobs, final_expression_name, duplicate_knobs, previous_line)

    # print(nvar_combination_expression)
    for varstore_struct_name in sorted(nvar_table):
        # print(varstore_struct_name)
        # print(nvar_table[varstore_struct_name]['NvarName'])
        # print(nvar_table[varstore_struct_name]['NvarGuidStr'])
        # print(nvar_table[varstore_struct_name]['NvarSize'])
        # print(nvar_table[varstore_struct_name]['KnobCount'])
        if nvar_table[varstore_struct_name]['KnobCount'] == 0:
            # delete entries for NVAR's that don't hold any Bios Knobs
            nvar_table.pop(varstore_struct_name)
            continue

    # Adding Boot order Variable manually
    nvar_table['BootOrder'] = {'NvarName': 'BootOrder',
                               'NvarGuidStr': '0x8BE4DF61, 0x93CA, 0x11d2, 0xAA, 0x0D, 0x00, 0xE0, 0x98, 0x03, 0x2B, 0x8C',
                               'NvarSize': 0, 'KnobCount': 0, 'ExpCount': 0, 'DupKnobCount': 0, 'VarNamesDict': {},
                               'DupExp': {}, 'NvarStructDone': False, 'bin_var_id': 0xFF}

    for varstore_struct_name in nvar_table:
        for variable in nvar_table[varstore_struct_name]['VarNamesDict']:
            if variable == 'Temp__Struct__Size':
                continue
            visual_expression = nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_EXP]
            if visual_expression != "TRUE":
                visual_expression = process_ro_expression(visual_expression, nvar_details, nvar_table, all_nvars, missing_nvars,
                                           read_only_nvars)
                nvar_table[varstore_struct_name]['VarNamesDict'][variable][KNOB_EXP] = visual_expression
        for DupCountId in nvar_table[varstore_struct_name]['DupExp']:
            visual_expression = nvar_table[varstore_struct_name]['DupExp'][DupCountId][1]
            if visual_expression != "TRUE":
                visual_expression = process_ro_expression(visual_expression, nvar_details, nvar_table, all_nvars, missing_nvars,
                                           read_only_nvars)
                nvar_table[varstore_struct_name]['DupExp'][DupCountId][1] = visual_expression

    temp_vars = {varstore_name: nvar_table[varstore_name]['KnobCount'] for varstore_name in nvar_table}
    temp_vars = sorted(temp_vars.items(), key=lambda x: x[1], reverse=True)
    sorted_lis = [_var[0] for _var in temp_vars]
    bin_var_id = 0
    # print ("-------------------------------------------")
    for varstore_struct_name in sorted_lis:
        if varstore_struct_name in all_nvars:
            all_nvars[varstore_struct_name]['bin_var_id'] = bin_var_id
        nvar_table[varstore_struct_name]['bin_var_id'] = bin_var_id
        # print(varstore_struct_name, bin_var_id, nvar_table[varstore_struct_name]['KnobCount'])
        bin_var_id = bin_var_id + 1
        if varstore_struct_name in read_only_nvars:
            temp_lis = copy.deepcopy(read_only_nvars[varstore_struct_name])
            for bios_knob_name in temp_lis:
                if nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name][KNOB_IN_XML]:
                    read_only_nvars[varstore_struct_name].remove(bios_knob_name)
                    nvar_table[varstore_struct_name]['VarNamesDict'][bios_knob_name].append(0x3)
    # print ("-------------------------------------------")
    if not skip_depex:
        for varstore_struct_name in list(reversed(sorted(read_only_nvars))):
            if (len(read_only_nvars[varstore_struct_name]) == 0) or (nvar_table[varstore_struct_name]['KnobCount'] != 0):
                continue
            if varstore_struct_name in nvar_table:
                all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'] = nvar_table[varstore_struct_name]['bin_var_id']
            else:
                all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'] = bin_var_id
                bin_var_id = bin_var_id + 1
            # print (nvar_details[varstore_struct_name], all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'])
        for varstore_struct_name in list(reversed(sorted(missing_nvars))):
            if len(missing_nvars[varstore_struct_name]) == 0:
                continue
            if varstore_struct_name in nvar_table:
                all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'] = nvar_table[varstore_struct_name]['bin_var_id']
            else:
                all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'] = bin_var_id
                bin_var_id = bin_var_id + 1
            # print (nvar_details[varstore_struct_name], all_nvars[nvar_details[varstore_struct_name]]['bin_var_id'])
    # print ("-------------------------------------------")

    # Done with all required processing and DB gathering, lets create the binary now.

    knobs_bin_file = os.path.join(output_path, 'BiosKnobsData.bin') if ((not knobs_bin_file) or (knobs_bin_file == "dummy")) else knobs_bin_file
    result_buffer, nvar_id_bin = construct_nvar_buffer(sorted_lis, skip_depex, nvar_details, nvar_table, all_nvars, duplicate_knobs, missing_nvars, all_knobs)

    # print ("=========================================================================")
    # for VarStructName in read_only_nvars:
    # print (VarStructName)
    # for knob_count in read_only_nvars[VarStructName]:
    # print ('\t'+knob_count)
    # print ("=========================================================================")
    # for VarStructName in missing_nvars:
    # print (VarStructName)
    # for knob_count in missing_nvars[VarStructName]:
    # print ('\t'+knob_count)
    # print ("=========================================================================")
    if not skip_depex:
        _result_buffer, nvar_id_bin = depex_evaluate(read_only_knobs=read_only_nvars, missing_nvars=missing_nvars,
                                                     nvar_id_bin=nvar_id_bin, nvar_details=nvar_details,
                                                     nvar_table=nvar_table, all_nvars=all_nvars, all_knobs=all_knobs)
        result_buffer = result_buffer + _result_buffer

    # Writing buffer to file...
    knobs_bin_file = os.path.join(output_path, 'BiosKnobsData.bin') if ((not knobs_bin_file) or (knobs_bin_file == "dummy")) else knobs_bin_file
    with open(knobs_bin_file, 'wb') as KnobsBin:
        KnobsBin.write(result_buffer)

    # for VarStructName in all_nvars:
    #     print(VarStructName)
    #     print('\t'+all_nvars[VarStructName]['VarName'])
    #     print('\t'+all_nvars[VarStructName]['Guid'])
    #     print(all_nvars[VarStructName]['Size'])
    #     exit(1)

    # DebugOut.close()
    # return nvar_table


def get_fv_main_component(bios_rom_file, ffs_guid, output_file):
    """

    :param bios_rom_file: BIOS Rom Image location
    :param ffs_guid: FFS Guid
    :param output_file: Output file location
    :return:
    """
    with open(bios_rom_file, "rb") as bios_bin:
        bios_bin_buffer_lis = list(bios_bin.read())
    bios_fv_base = 0
    for _ in range(0, len(bios_bin_buffer_lis)):
        fv_zero_vector = fetch_guid(bios_bin_buffer_lis, bios_fv_base)
        fv_guid = fetch_guid(bios_bin_buffer_lis, (bios_fv_base + 0x10))
        if (fv_zero_vector == ZERO_GUID) and (fv_guid != ZERO_GUID):  # Every valid FV needs to have this zero vector.
            fv_size = read_list(bios_bin_buffer_lis, (bios_fv_base + 0x20), 8)
            if (fv_guid == AllFsGuid) and (fv_size == 0xFFFFFFFFFFFFFFFF):
                bios_fv_base = ((bios_fv_base & 0xFFFFF000) + 0x1000)
                continue  # InValid FV, skip this iteration
            if fv_guid == gEfiFirmwareFileSystemGuid:
                file_system_type = 1
            elif fv_guid == gEfiFirmwareFileSystem2Guid:
                file_system_type = 2
            elif fv_guid == gEfiFirmwareFileSystem3Guid:
                file_system_type = 3
            else:
                file_system_type = 0
            fv_signature = read_list(bios_bin_buffer_lis, (bios_fv_base + 0x28), 4)
            if fv_signature != FV_SIGNATURE:  # "_FVH" = 0x4856465F
                bios_fv_base = ((bios_fv_base & 0xFFFFF000) + 0x1000)
                continue  # InValid FV, skip this iteration
            fv_header_length = read_list(bios_bin_buffer_lis, (bios_fv_base + 0x30), 2)
            extended_header_offset = read_list(bios_bin_buffer_lis, (bios_fv_base + 0x34), 2)
            if extended_header_offset:
                fv_header_length = extended_header_offset + 0x10 + 4
            bios_fv_ffs_base = bios_fv_base + fv_header_length
            file_system_base = (bios_fv_ffs_base + 7) & 0xFFFFFFF8  # because FileSystem sits on a 8 byte boundary
            if file_system_base >= (bios_fv_base + fv_size):
                bios_fv_base = (bios_fv_base + fv_size)
                continue
            for FfsCount in range(0, 1):
                if file_system_type == 0:
                    break
                bios_fv_ffs_base = (bios_fv_ffs_base + 7) & 0xFFFFFFF8  # this is because FFS sits on a 8 byte boundary
                if bios_fv_ffs_base >= (bios_fv_base + fv_size):
                    break
                _ffs_guid = fetch_guid(bios_bin_buffer_lis, bios_fv_ffs_base)
                if _ffs_guid == ZERO_GUID:
                    break
                if _ffs_guid == ffs_guid:
                    with open(output_file, 'wb') as ffs_file:
                        if sys.version_info.major == 2:
                            ffs_file.write(string.join(bios_bin_buffer_lis[bios_fv_base:bios_fv_base + fv_size], ''))
                        else:
                            ffs_file.write(bytes(bios_bin_buffer_lis[bios_fv_base:bios_fv_base + fv_size]))
                    return
            bios_fv_base = (bios_fv_base + fv_size)
        else:
            bios_fv_base = ((bios_fv_base & 0xFFFFF000) + 0x1000)  # InValid FV, Adjust FvBaseAccordingly


@timeit
def post_build(bios_build_dir, vfr_output_file_path, header_file_path, knobs_bin_file, skip_depex, xmlcli_build_dir=None, input_vfr_directories=[], process_vfr=True):
    """

    :param bios_build_dir: Build output directory of Bios
    :param vfr_output_file_path: if not provided then default considered as <xmlcli_build_dir>/Dummy
    :param header_file_path:
    :param knobs_bin_file: bios knob data bin file location
    :param skip_depex: specify whether to skip depex or not
    :param xmlcli_build_dir: Build output directory of XmlCli
    :param process_vfr: Specifies whether to process vfr file or not
    :return:
    """
    build_dir_x64 = os.path.join(bios_build_dir, "X64")
    if not xmlcli_build_dir:
        xmlcli_build_dir = os.path.join(build_dir_x64, "XmlCliFeaturePkg", "XmlCliCommon")
    if os.path.exists(xmlcli_build_dir):
        print("Create and insert Bios Knobs Data Bin file for XmlCli")
        dummy_dir = make_directory(os.path.join(xmlcli_build_dir, "Dummy"))
        if process_vfr:
            ret_status = clean_vfr_out_file(dummy_dir, input_vfr_directories=input_vfr_directories,
                                            build_path=build_dir_x64)

            # remove temp files
            for temp_file in os.listdir(dummy_dir):
                if os.path.splitext(temp_file)[-1] in (".int", ".iout"):
                    os.remove(os.path.join(dummy_dir, temp_file))

        parse_setup_files(vfr_output_file_path=vfr_output_file_path if vfr_output_file_path else dummy_dir,
                          header_file_path=header_file_path,
                          knobs_bin_file=knobs_bin_file,
                          skip_depex=skip_depex)


def parse_setup_files_handler(args):
    print("Parse Setup Files XmlCli Steps with arguments: \n\t{}".format(args))

    parse_setup_files(vfr_output_file_path=args.vfr_output_file_path,
                      header_file_path=args.header_file_path,
                      knobs_bin_file=args.knobs_bin_file,
                      skip_depex=args.skip_depex)


def clean_vfr_out_file_handler(args):
    print("Clean VFR Out XmlCli Steps with arguments: \n\t{}".format(args))

    clean_vfr_out_file(vfr_out_path=args.vfr_out_path,
                       input_vfr_directories=args.input_vfr_directories,
                       build_path=args.build_path)

    # remove temp files
    for temp_file in os.listdir(args.vfr_out_path):
        if os.path.splitext(temp_file)[-1] in (".int", ".iout"):
            os.remove(os.path.join(args.vfr_out_path, temp_file))


def get_fv_main_component_handler(args):
    print("Get FV main Component XmlCli Steps with arguments: \n\t{}".format(args))

    guid_str = args.ffs_guid.split(',')
    ffs_guid = [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]
    for index in range(0, len(guid_str)):
        ffs_guid[index] = int(guid_str[index].strip(), 16)

    get_fv_main_component(bios_rom_file=args.bios_image_file,
                          ffs_guid=ffs_guid,
                          output_file=args.output_file)


def post_build_handler(args):
    print("Post Build XmlCli Steps with arguments: \n\t{}".format(args))

    bios_knobs_ui = os.path.join(args.xmlcli_build_dir, "BiosKnobsUISec.ui")
    bios_knobs_data_sec = os.path.join(args.xmlcli_build_dir, "BiosKnobsData.sec")
    bios_knobs_data = os.path.join(args.xmlcli_build_dir, "BiosKnobsData.bin")
    bios_knobs_data_ffs = os.path.join(args.xmlcli_build_dir, "BiosKnobsData.ffs")
    client_bios_fd = os.path.join(args.build_dir, "FV", "ClientBios.fd")
    # The exact filename is needed on case-sensitive systems (Ex: Linux)
    for file in os.listdir(os.path.join(args.build_dir, "FV")):
        if file.lower() == "clientbios.fd":
            client_bios_fd = os.path.join(args.build_dir, "FV", file)
    client_bios_fd_bak = os.path.join(args.build_dir, "FV", "ClientBios.fd.bak")
    xmlcli_client_bios_fd = os.path.join(args.build_dir, "FV", "ClientBios_XmlCli.fd")
    if os.path.exists(client_bios_fd) and not os.path.exists(client_bios_fd_bak):
        shutil.copy(client_bios_fd, client_bios_fd_bak)

    if not args.stitch_only:
        start_time = time.time()
        post_build(bios_build_dir=args.build_dir,
                   xmlcli_build_dir=args.xmlcli_build_dir,
                   input_vfr_directories=args.input_vfr_directories,
                   process_vfr=args.process_vfr,
                   vfr_output_file_path=args.vfr_output_file_path,
                   header_file_path=args.header_file_path if args.header_file_path else "all_knobs",
                   knobs_bin_file=args.knobs_bin_file if args.knobs_bin_file else bios_knobs_data,
                   skip_depex=args.skip_depex)
        print("XmlCli Parsing Time to generate BiosKnobsData.bin = {:.1f} secs".format(time.time() - start_time))

    # Perform Section/FFS generation
    if args.edk_tools_bin and args.edk_tools_scripts:
        gen_sec_binary = os.path.join(args.edk_tools_bin, GEN_SEC_BIN)
        gen_ffs_binary = os.path.join(args.edk_tools_bin, GEN_FFS_BIN)
        fmmt_tool = os.path.join(args.edk_tools_scripts, FMMT_SCRIPT)
        fmmt_input_fd = client_bios_fd if not os.path.exists(client_bios_fd_bak) else client_bios_fd_bak

        bios_knobs_ui_cmd = [gen_sec_binary, "-s", "EFI_SECTION_USER_INTERFACE", "-n", "XmlCliBiosKnobsData",
                             "-o", bios_knobs_ui]
        bios_knobs_data_cmd = [gen_sec_binary, "-s", "EFI_SECTION_RAW",
                               "-o", bios_knobs_data_sec, bios_knobs_data]
        bios_knobs_data_ffs_cmd = [gen_ffs_binary, "-t", "EFI_FV_FILETYPE_FREEFORM", "-g", BIOS_KNOBS_BIN_FFS_GUID,
                                   "-o", bios_knobs_data_ffs,
                                   "-i", bios_knobs_data_sec, "-i", bios_knobs_ui]
        fmmt_cmd = [fmmt_tool, "-a", fmmt_input_fd,
            BIOS_KNOBS_BIN_FV_GUID, bios_knobs_data_ffs, xmlcli_client_bios_fd]

        try:
            print(">> Generating Bios Knobs section...\nExecuting: {}".format(bios_knobs_ui_cmd))
            result = subprocess.check_call(bios_knobs_ui_cmd, stderr=subprocess.STDOUT)
            print(result)
            print(">> Generating BiosKnobsData.Bin section...\nExecuting: {}".format(bios_knobs_data_cmd))
            result = subprocess.check_call(bios_knobs_data_cmd, stderr=subprocess.STDOUT)
            print(result)
            print(">> Generating FFS...\nExecuting: {}".format(bios_knobs_data_ffs_cmd))
            result = subprocess.check_call(bios_knobs_data_ffs_cmd, stderr=subprocess.STDOUT)
            print(result)
            print(">> Generating new FD with FMMT...\nExecuting: \n{}".format(" ".join(fmmt_cmd)))
            result = os.popen(" ".join(fmmt_cmd))
            print(result.read())
        except Exception as e:
            print("Exception in subprocess call: {}!!\n".format(e))

        if not os.path.exists(xmlcli_client_bios_fd):
            print("!!! ERROR:Build Fail !!!")
            print("*** {} does not exist. ***".format(xmlcli_client_bios_fd))
            print("-- FMMT insertion process may have failed, please check logs above --")
        else:
            shutil.copy(xmlcli_client_bios_fd, client_bios_fd)
            print("==== {} File Inserted =====".format(bios_knobs_data))


def main():
    """Entrypoint method for Parsing setup knobs

    The file accepts commandline argument:
    argument 1:
        post_build - Build the XmlCli Process from python file itself
        cleanout - Clean All Output files
        getfvmaincmp - Get FV Main component for given FFS Guid
        (otherwise) -
    """
    print("XmlCli ParseSetup Version {}".format(__version__))

    arg_parser = argparse.ArgumentParser("XmlCli ParseSetup v{}".format(__version__),
                                         description="XmlCli Post Build Steps")
    subparsers = arg_parser.add_subparsers()

    # Post Build
    post_build_parser = subparsers.add_parser("post_build", help="Perform Complete Post Build steps")
    post_build_parser.add_argument("-b", "--build_dir",
                                   required=True, type=str,
                                   help="Enter BIOS Build directory where output of all files are generated",
                                   )
    post_build_parser.add_argument("-xb", "--xmlcli_build_dir",
                                   required=True, type=str,
                                   help="Enter Build directory where XmlCli output is generated",
                                   )
    post_build_parser.add_argument("-iv", "--input_vfr_directories",
                                   required=False, type=str, nargs="*",
                                   help="zero or more vfr files",
                                   )
    post_build_parser.add_argument("-pv", "--process_vfr",
                                   required=False, type=bool, default=True,
                                   help="Enter boolean value whether to process vfr file or not. (Default is True)",
                                   )
    post_build_parser.add_argument("-ov", "--vfr_output_file_path",
                                   required=False, type=str,
                                   help="Specify VFR output file location",
                                   )
    post_build_parser.add_argument("-oh", "--header_file_path",
                                   required=False, type=str,
                                   help="Specify output header file location",
                                   )
    post_build_parser.add_argument("-ob", "--knobs_bin_file",
                                   required=False, type=str,
                                   help="Specify output header file location",
                                   )
    post_build_parser.add_argument("-sd", "--skip_depex",
                                   required=False, type=bool, default=False,
                                   help="Specify whether to Skip DEPEX check or not",
                                   )
    post_build_parser.add_argument("-st", "--stitch_only",
                                   required=False, type=bool, default=False,
                                   help="Specify whether to Stitch fd only from already generated bin file or not",
                                   )
    post_build_parser.add_argument("-edk", "--edk_tools_bin",
                                   required=False, type=str,
                                   help="Edk Tools Binaries location",
                                   )
    post_build_parser.add_argument("-ets", "--edk_tools_scripts",
                                   required=False, type=str,
                                   help="Edk Tools Python Scripts location",
                                   )
    post_build_parser.set_defaults(func=post_build_handler)

    # FV Main Component
    fv_component_parser = subparsers.add_parser("get_fv_main_cmp", help="Perform Complete Post Build steps")
    fv_component_parser.add_argument("-ib", "--bios_image_file",
                                     required=True, type=str,
                                     help="BIOS Image file",
                                     )
    fv_component_parser.add_argument("-fg", "--ffs_guid",
                                     required=True, type=str,
                                     help="FFS Guid",
                                     )
    fv_component_parser.add_argument("-o", "--output_file",
                                     required=False, type=str,
                                     help="Output file location",
                                     )
    fv_component_parser.set_defaults(func=get_fv_main_component_handler)

    # Parse Setup Files Only
    parse_setup_parser = subparsers.add_parser("parse_setup", help="Perform Parsing only setup files")
    parse_setup_parser.add_argument("-ov", "--vfr_output_file_path",
                                    required=True, type=str,
                                    help="Output Vfr path",
                                    )
    parse_setup_parser.add_argument("-oh", "--header_file_path",
                                    required=False, type=str,
                                    help="Specify output header file location",
                                    )
    parse_setup_parser.add_argument("-ob", "--knobs_bin_file",
                                    required=True, type=str,
                                    help="Output Path for generating Bios Knobs Binary",
                                    )
    parse_setup_parser.add_argument("-sd", "--skip_depex",
                                    required=False, type=bool, default=False,
                                    help="Specify whether to Skip DEPEX check or not",
                                    )
    parse_setup_parser.set_defaults(func=parse_setup_files_handler)

    # Clean Out File
    clean_out_parser = subparsers.add_parser("clean_out", help="Perform Complete Post Build steps")
    clean_out_parser.add_argument("-ov", "--vfr_out_path",
                                  required=True, type=str,
                                  help="Output Vfr path",
                                  )
    clean_out_parser.add_argument("-iv", "--input_vfr_directories",
                                  required=False, type=str, nargs="*",
                                  help="zero or more vfr files",
                                  )
    clean_out_parser.add_argument("-ob", "--build_path",
                                  required=True, type=str,
                                  help="Output Build Path",
                                  )
    clean_out_parser.set_defaults(func=clean_vfr_out_file_handler)

    start_time = time.time()

    args = arg_parser.parse_args()
    args.func(args)

    print("XmlCli Overall Parsing Time = %.1f secs" % (time.time() - start_time))


if __name__ == '__main__':
    main()
