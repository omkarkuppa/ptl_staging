## @file
# FvAlignment.py
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2020 Intel Corporation.
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
# @par Specification
#
#
# This script supports
# -  Parse Flash Map FDF file
# -  Check if FV alignment meets requirements in pre build process,
# -  Check if ACM alignment meets requirements in pre build process,
# -  Replace a region with input image in post build process,
#
#  Note : This script does not support the flash map FDF files
#         which have !if/!else in between the setting lines.
#         The last setting in the file is taken by default.
#         The first setting in the file is taken by "--firstmap" option
#

"""
usage: FvAlignment.py [optional arguments] [positional arguments]

Firmware Volume Alignment Check and Edit Script

positional arguments:
  {acm_alignment_check,fv_alignment_check,fsp_size_check}
    acm_alignment_check
                        check ACM alignment
    fv_alignment_check  check FV alignment
    fsp_size_check      check FSP binary size


optional arguments:
  -h, --help            show this help message and exit
  -cl CHECKLIST, --checklist CHECKLIST
                        a hexadecimal number to run check items. Setting bit N lets the check routine for requirement
                        ID N to run. No check routines run if not specified. [all] runs all check routines
  -rl REQLIST, --reqlist REQLIST
                        a hexadecimal number to show requirements for each check item. Setting bit N shows
                        requirement ID N. No requirements are shown if not specified. [all] shows requirements for all
                        check items
  --flashmap FLASHMAP   FlashMapInclude FDF file path
  --firstmap            If this option is set, the tool takes the first hit of PCD setting on parsing FlashMapInclude
                        FDF. If this option is not set, the tool takes the last hit of PCD setting on parsing
                        FlashMapInclude FDF.
  --topswap_size TOPSWAP_SIZE
                        top swap size (hex)
  --replace_image_filepath REPLACE_IMAGE_FILEPATH
                        replace image file path
  -i INPUT, --input INPUT
                        input image file path
  -o OUTPUT, --output OUTPUT
                        output image file path
  --fsp_bin_pkg FSP_BIN_PKG
                        FSP Bin Pkg folder path
  --fspm_size_limit FSPM_SIZE_LIMIT
                        FSP M size limit (hex) per platform requirement


"""

import os
import shutil
import re
import sys
import argparse

__prog__          = sys.argv[0]
__description__   = "Firmware Volume Alignment Check and Edit Script"


FULL_BIT_MASK =  0xFFFFFFFF
FOUR_GB       =  0xFFFFFFFF

#
# This class supports flash map settings
#
class Flashmap ():
    def __init__ (self, flashmap_fdf):
        with open (flashmap_fdf, "r") as fdf:
            self.map    =  fdf.read ()
        self.file_path       =  flashmap_fdf

        #
        #  This script parses the following PCD and macro names.
        #
        self.premem_offset    = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPreMemoryOffset"
        self.premem_size      = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPreMemorySize"
        self.bin_offset       = "gBoardModuleTokenSpaceGuid.PcdFlashFvFirmwareBinariesOffset"
        self.bin_size         = "gBoardModuleTokenSpaceGuid.PcdFlashFvFirmwareBinariesSize"
        self.postmem_offset   = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemoryOffset"
        self.postmem_size     = "gMinPlatformPkgTokenSpaceGuid.PcdFlashFvPostMemorySize"
        self.ibb_offset       = "gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashIbbOffset"
        self.ibb_size         = "gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashIbbSize"
        self.ibbr_offset      = "gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashIbbROffset"
        self.ibbr_size        = "gCapsuleFeaturePkgTokenSpaceGuid.PcdFlashIbbRSize"
        self.flash_base       = "FLASH_BASE"
        self.flash_size       = "FLASH_SIZE"
        self.acm_align_option = "ACM_ALIGNMENT_ON_FV_BASE"

    #
    # Assuming Flash Map FDF has no !if/!else between the lines but only one set of mapping PCDs.
    # This call returns the last hit of the matches by parsing the file. If no hit, raise exception.
    #
    def read (self, setting):
        setting_list = []
        pattern     = r"(?m)^\s*DEFINE\s*" + setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        pattern_pcd = r"(?m)^\s*SET\s*" + setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        pattern_acm_option = r"(?m)^\s*DEFINE\s*" + setting + "\s*=\s*(?P<value>[0-9]*K).*"
        for m in re.finditer (pattern_pcd, self.map):
            setting_list.append (int (m.group ("value"), base = 16))
        if len (setting_list) == 0:
            for m in re.finditer (pattern, self.map):
                setting_list.append (int (m.group ("value"), base = 16))
        if len (setting_list) == 0:
            for m in re.finditer (pattern_acm_option, self.map):
                setting_list.append (m.group ("value"))
        try:
            read_value = setting_list [-1]
        except IndexError as err:
            print ("\n\n Error!!! {} is not found in {} \n\n".format (setting, self.file_path))
            raise
        finally:
            return read_value

    def show_settings (self):
        read_value  = "  |  {:#010x}".format (self.read (self.flash_base))
        read_value += "  |  {:#010x}".format (self.read (self.flash_size))
        read_value += "  |  {}".format (self.read (self.acm_align_option))
        read_value += "  |  {:#010x}".format (self.read (self.premem_offset))
        read_value += "  |  {:#010x}".format (self.read (self.premem_size))
        read_value += "  |  {:#010x}".format (self.read (self.postmem_offset))
        read_value += "  |  {:#010x}".format (self.read (self.postmem_size))
        read_value += "  |  {:#010x}".format (self.read (self.bin_offset))
        read_value += "  |  {:#010x}".format (self.read (self.bin_size))
        read_value += "  |\n"
        print (read_value)


#
# This class supports extension or override of the flash map settings
#
class FlashmapEx (Flashmap):
    def __init__(self, flashmp_fdf):
        super().__init__(flashmp_fdf)

    #
    # Assuming Flash Map FDF has no !if/!else between the lines but only one set of mapping PCDs.
    # This call returns the first hit of the matches by parsing the file. If no hit, raise exception.
    #
    def read (self, setting):
        setting_list = []
        pattern     = r"(?m)^\s*DEFINE\s*" + setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        pattern_pcd = r"(?m)^\s*SET\s*" + setting + "\s*=\s*0x(?P<value>[0-9a-fA-F]*).*"
        for m in re.finditer (pattern_pcd, self.map):
            setting_list.append (int (m.group ("value"), base = 16))
        if len (setting_list) == 0:
            for m in re.finditer (pattern, self.map):
                setting_list.append (int (m.group ("value"), base = 16))
        try:
            read_value = setting_list [0]
        except IndexError as err:
            print ("\n\n Error!!! {} is not found in {} \n\n".format (setting, self.file_path))
            raise
        finally:
            return read_value

#
# This class supports FSP binary size
#
class Binaries ():
    def __init__ (self):
        self.fspt_bin_name    = "Fsp_Rebased_T.fd"
        self.fspm_bin_name    = "Fsp_Rebased_M.fd"
        self.fsps_bin_name    = "Fsp_Rebased_S.fd"

    def get_bin_size (self, dir, bin_name):
        return os.path.getsize (os.path.join (dir, bin_name))

#
# This class supports check items
#
class RequirementList ():
    def __init__ (self):
        self.__result    = 0
        self.__checklist = 0
        self.__reqlist   = 0

    @property
    def checklist (self):
        return self.__checklist
    @checklist.setter
    def checklist (self, checklist):
        self.__checklist = self.__checklist | checklist

    @property
    def reqlist (self):
        return self.__reqlist
    @reqlist.setter
    def reqlist (self, reqlist):
        self.__reqlist = self.__reqlist | reqlist

    @property
    def result (self):
        return self.__result
    @result.setter
    def result (self, result):
        self.__result = self.__result | result

    def show_req (self, req_message):
        print (req_message)

    def report (self, error_message, req_message, fail):
        self.result = fail
        if fail:
            print ("  ERROR {}".format (error_message))
            self.show_req ("  Requirement" + req_message + "\n\n")

    def to_run (self, id):
        return self.__checklist & 1 << id

    def to_show (self, id):
        return self.__reqlist & 1 << id

    #
    # Requirement list
    #
    def req_bb_topswap_size (self, fm, tsize):
        id = 0
        req_message    = "  ({})  boot block size < TopSwap size".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None and tsize is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} exceeds topswap size limitation\n".format (fm.ibb_size.split(".")[1])
            error_message += "  {} : {:#010x}\n".format (fm.ibb_size, fm.read (fm.ibb_size))
            error_message += "  topswap HW size req : {:#010x}".format (tsize)
            if tsize - fm.read (fm.ibb_size) < 0:
                self.report (error_message, req_message, 1)

    def req_premem_flashbase_4g_address (self, fm):
        id = 1
        req_message    = "  ({})  PreMem FV offset + PreMem FV size + flash base address = 4GB".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} and/or {} has wrong settings\n".format (fm.premem_offset.split(".")[1], fm.premem_size.split(".")[1])
            error_message += "  {} : {:#010x}\n".format (fm.premem_offset, fm.read (fm.premem_offset))
            error_message += "  {} : {:#010x}\n".format (fm.premem_size, fm.read (fm.premem_size))
            if FOUR_GB  - (fm.read (fm.premem_offset) + fm.read (fm.premem_size) + fm.read (fm.flash_base)) + 0x1 != 0:
                self.report (error_message, req_message, 1)

    def req_bb_abs_address (self, fm): # absolute address check from 4GB
        id = 2
        req_message    = "  ({})  boot block offset + boot block size + flash base address = 4GB".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} does not match the region size from the boot block offset to 4GB\n".format (fm.ibb_size.split(".")[1])
            error_message += "  {} : {:#010x}\n".format (fm.ibb_offset, fm.read (fm.ibb_offset))
            error_message += "  {} : {:#010x}\n".format (fm.ibb_size, fm.read (fm.ibb_size))
            error_message += "  {} : {:#010x}".format (fm.flash_base, fm.read (fm.flash_base))
            if FOUR_GB - fm.read (fm.ibb_offset) - fm.read (fm.ibb_size) - fm.read (fm.flash_base) + 0x1 != 0:
                self.report (error_message, req_message, 1)

    def req_bb_rel_address (self, fm): # relative address check from flash base (double check)
        id = 3
        req_message    = "  ({})  PreMem FV offset + PreMem FV size - boot block offset = boot block size".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} does not match the region size from the boot block offset {}\n".format (fm.ibb_size.split(".")[1], format (fm.ibb_offset.split(".")[1]))
            error_message += "  to the address {} + {}\n".format (fm.premem_offset.split(".")[1], fm.premem_size.split(".")[1])
            error_message += "  {} : {:#010x}\n".format (fm.premem_offset, fm.read (fm.premem_offset))
            error_message += "  {} : {:#010x}\n".format (fm.premem_size, fm.read (fm.premem_size))
            error_message += "  {} : {:#010x}\n".format (fm.ibb_offset, fm.read (fm.ibb_offset))
            error_message += "  {} : {:#010x}".format (fm.ibb_size, fm.read (fm.ibb_size))
            if (fm.read (fm.premem_offset) + fm.read (fm.premem_size) - fm.read (fm.ibb_offset) - fm.read (fm.ibb_size)) != 0:
                self.report (error_message, req_message, 1)

    def req_bb_offset_match (self, fm):
        id = 4
        req_message    = "  ({})  Firmware Binary FV offset = boot block offset".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} and {} should match\n".format (fm.ibb_offset.split(".")[1], fm.bin_offset.split(".")[1])
            error_message += "  {} : {:#010x}\n".format (fm.ibb_offset, fm.read (fm.ibb_offset))
            error_message += "  {} : {:#010x}".format (fm.bin_offset, fm.read (fm.bin_offset))
            if fm.read (fm.ibb_offset) - fm.read (fm.bin_offset) != 0:
                self.report (error_message, req_message, 1)

    def req_acm (self, fm):
        id = 5
        req_message      = "  ({})  FirmwareBinaries Base = 256KB align - ACM alignment based on FV base\n".format (id)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and fm is not None:
            msg0     = "  ({})  FirmwareBinaries Base = 256KB align  (ie 0xFFN00000, 0xFFN40000, 0xFFN80000, 0xFFNC0000) \n".format (id)
            msg0    += "                       Workable offset example : 0x00800000, 0x00840000, 0x00880000, 0x008C0000"
            msg1     = "  ({})  FirmwareBinaries Base = 256KB align - 64KB (ie 0xFFN30000, 0xFFN70000, 0xFFNB0000, 0xFFNF0000) \n".format (id)
            msg1    += "                             Workable offset example : 0x00830000, 0x00870000, 0x008B0000, 0x008F0000"
            msg2     = "  ({})  FirmwareBinaries Base = 256KB align - 128KB (ie 0xFFN20000, 0xFFN60000, 0xFFNA0000, 0xFFNE0000) \n".format (id)
            msg2    += "                              Workable offset example : 0x00820000, 0x00860000, 0x008A0000, 0x008E0000"
            alignment_dict     =  {"256K":{0:msg0, 1:0x00000}, "64K":{0:msg1, 1:0x10000}, "128K":{0:msg2, 1:0x20000}}
            acm_alignment_mask =  0x0003FFFF   # ACM absolute address alignment requirement mask (256kB)
            error_message  = "({})\n".format (id)
            error_message += "  {} does not meet the alignment requirement.\n".format (fm.bin_offset)
            error_message += "  Current address = {0:#010x} + {1:#010x}".format (fm.read (fm.flash_base), fm.read (fm.bin_offset))
            if (fm.read (fm.bin_offset) + alignment_dict [fm.read (fm.acm_align_option)][1] + fm.read (fm.flash_base)) & acm_alignment_mask != 0:
                self.report (error_message, alignment_dict [fm.read (fm.acm_align_option)][0], 1)


    def req_fspm_binary_size (self, pkg, bin, fspm_size_limit):
        id = 6
        req_message    = "  ({})  FspM binary size < {:#010x}\n".format (id, fspm_size_limit)
        if self.to_show (id):
            self.show_req (req_message)
        if self.to_run (id) and bin is not None:
            error_message  = "({})\n".format (id)
            error_message += "  {} should not exceed the limit size\n".format (bin.fspm_bin_name)
            error_message += "  {} size : {:#010x}\n".format (bin.fspm_bin_name, bin.get_bin_size (pkg, bin.fspm_bin_name))
            if fspm_size_limit - bin.get_bin_size (pkg, bin.fspm_bin_name) < 0:
                self.report (error_message, req_message, 1)

#
# This class supports extension or override of the check items
#
class RequirementListEx (RequirementList):
    def __init__(self):
        super ().__init__()

    #
    # Requirement List
    #


#
# This class supports binary file manipulation
#
class Fd ():
    def __split (self, inbin, offset, split0, split1):
        with open (inbin, "rb") as f:
            read0 = f.read (offset)
            f.seek (offset)
            read1 = f.read (os.path.getsize (inbin) - offset)

        with open (split0, "wb") as bin_split0:
            bin_split0.write (read0)

        with open (split1, "wb") as bin_split1:
            bin_split1.write (read1)

    def __join (self, inbin_list, outbin):
        with open (outbin, "wb") as f:
            for l in inbin_list:
                data = open (l, "rb").read ()
                f.write (data)
                f.flush ()

    def __pad (self, region_size, inbin, outbin):
        with open (inbin, "rb") as f:
            read_in = f.read ()
            size_in = os.path.getsize (inbin)
            if region_size < size_in:
                print ("Error!!! region size is smaller than the input binary size")
                raise
            else:
                binary_out = read_in + b"\xff" * (region_size - size_in)
                with open (outbin, "wb") as f_out:
                    f_out.write (binary_out)

    #
    #  call for replacing the region from <offset> to <offset> + <size> of the input binary <in_bin_filepath>
    #  with the binary <replace_bin_filepath> and saves as <out_bin_filepath>.
    #
    def replace (self, in_bin_filepath, out_bin_filepath, offset, size, replace_bin_filepath):
        if os.path.getsize (replace_bin_filepath) > size:
            print ("Error!!! region size {} is too small to be replaced with {}".format (size, replace_bin_filepath))
            return 1

        temp_dir = os.path.join (os.environ ["BUILD_DIR_PATH"], "fvalignment_tmp")
        if os.path.exists (temp_dir):
            shutil.rmtree (temp_dir)
        os.makedirs (temp_dir)
        self.__pad (size, replace_bin_filepath, os.path.join (temp_dir, "tmp_replace.bin"))
        self.__split (in_bin_filepath, offset, os.path.join (temp_dir, "tmp0.bin"), os.path.join (temp_dir, "tmp1.bin"))
        self.__split (in_bin_filepath, offset + size, os.path.join (temp_dir, "tmp2.bin"), os.path.join (temp_dir, "tmp3.bin"))
        self.__join ([os.path.join (temp_dir, "tmp0.bin"), os.path.join (temp_dir, "tmp_replace.bin"), os.path.join (temp_dir, "tmp3.bin")], os.path.join (temp_dir, "tmp_join.bin"))

        if os.path.exists (out_bin_filepath):
            shutil.copy2 (out_bin_filepath, out_bin_filepath + ".bk")
        shutil.copy2 (os.path.join (temp_dir, "tmp_join.bin"), out_bin_filepath)

        if not os.path.exists (out_bin_filepath):
            return 1
        else:
            shutil.rmtree (temp_dir)
            return 0


def main ():
    def is_valid_file_path (Argument):
        if os.path.isfile (Argument):
            return Argument
        else:
            message = "[" + Argument + "] is not found"
            raise argparse.ArgumentTypeError (message)

    def is_valid_folder_path (Argument):
        if os.path.isdir (Argument):
            return Argument
        else:
            message = "[" + Argument + "] is not found"
            raise argparse.ArgumentTypeError (message)

    def is_valid_32bit_all (Argument):
        try:
            value = int (Argument, base = 16)
        except:
            if Argument == "all":
                return Argument
            else:
                message = "[" + Argument + "] is not a valid 32bit number"
                raise argparse.ArgumentTypeError (message)
        if value < 0 or value > 0xffffffff:
            message = "[" + Argument + "] is not a valid 32bit number"
            raise argparse.ArgumentTypeError (message)
        return Argument

#
# call to run ACM alignment check routines
#
    def check_acm_alignment (Argument):
        print ("\nChecking on ACM alignment ... [{}]".format(args.checklist))

        try:
            checklist = int (args.checklist, base = 16)
        except:
            if args.checklist == "all":
                checklist = FULL_BIT_MASK
            else:
                checklist = 0

        try:
            reqlist = int (args.reqlist, base = 16)
        except:
            if args.reqlist == "all":
                reqlist = FULL_BIT_MASK
            else:
                reqlist = 0

        fm = None
        if checklist == 0:
            print ("No check items to run")
        else:
            if args.flashmap is None:
                print ("Error!!! flashmap option is not set")
                return 1
            fm = Flashmap (args.flashmap)
            if args.firstmap:
                del fm
                fm = FlashmapEx (args.flashmap)
            fm.show_settings ()

        check = RequirementList ()
        # ID check items to run
        check.checklist = checklist
        # ID requirements to show
        check.reqlist   = reqlist
        # run routines
        check.req_acm (fm)

        if checklist != 0:
            if check.result:
                print ("\n  Update flash map file ({}) for adjustment\n".format (args.flashmap))
            else:
                print ("Pass ACM alignment requirement check")

        return check.result

#
# call to run FV alignment check routines
#
    def check_fv_alignment (Argument):
        print ("\nChecking on FV alignment ... [{}]".format(args.checklist))

        try:
            checklist = int (args.checklist, base = 16)
        except:
            if args.checklist == "all":
                checklist = FULL_BIT_MASK
            else:
                checklist = 0

        try:
            reqlist = int (args.reqlist, base = 16)
        except:
            if args.reqlist == "all":
                reqlist = FULL_BIT_MASK
            else:
                reqlist = 0

        fm    = None
        tsize = None
        if checklist == 0:
            print ("No check items to run")
        else:
            if args.flashmap is None:
                print ("Error!!! flashmap option is not set")
                return 1
            else:
                fm = Flashmap (args.flashmap)
                if args.firstmap:
                    del fm
                    fm = FlashmapEx (args.flashmap)
                fm.show_settings ()

            if checklist & 0x1:
                if args.topswap_size is None:
                    print ("Error!!! valid topswap_size option is not set")
                    return 1
                else:
                    tsize = int (args.topswap_size, base = 16)

        check = RequirementList ()
        # ID check items to run
        check.checklist = checklist
        # ID requirements to show
        check.reqlist   = reqlist
        # run routines
        check.req_bb_topswap_size (fm, tsize)
        check.req_premem_flashbase_4g_address (fm)
        check.req_bb_abs_address (fm)
        check.req_bb_rel_address (fm)
        check.req_bb_offset_match (fm)

        if checklist != 0:
            if check.result:
                print ("\n  Update flash map file ({}) for adjustment\n".format (args.flashmap))
            else:
                print ("Pass FV alignment requirement check")

        return check.result

#
# call to run FSP binary size check routines
#
    def check_fsp_size (Argument):
        print ("\nChecking on FSP size ... [{}]".format(args.checklist))

        try:
            checklist = int (args.checklist, base = 16)
        except:
            if args.checklist == "all":
                checklist = FULL_BIT_MASK
            else:
                checklist = 0

        try:
            reqlist = int (args.reqlist, base = 16)
        except:
            if args.reqlist == "all":
                reqlist = FULL_BIT_MASK
            else:
                reqlist = 0

        bin    = None
        mlimit = None
        if checklist == 0:
            print ("No check items to run")
        else:
            bin = Binaries ()
            if checklist & 0x40:    # check BIT6 for ID 6
                if args.fsp_bin_pkg is None:
                    print ("Error!!! valid fsp_bin_pkg option is not set")
                    return 1
                elif args.fspm_size_limit is None:
                    print ("Error!!! valid fspm_size_limit option is not set")
                    return 1
                else:
                    mlimit = int (args.fspm_size_limit, base = 16)

        check = RequirementList ()
        # ID check items to run
        check.checklist = checklist
        # ID requirements to show
        check.reqlist   = reqlist
        # run routines
        check.req_fspm_binary_size (args.fsp_bin_pkg, bin, mlimit)

        if checklist != 0:
            if check.result:
                print ("\n  Update FspPkg FDF for adjustment\n")
            else:
                print ("Pass FSP-M binary size requirement check \n")

        return check.result


#
# call to replace IBB with an image
#
    def replace_ibb_with (Argument):
        print ("Replacing IBB ...")
        print (" input FD      : {}".format (args.input))
        print (" replace image : {}".format (args.replace_image_filepath))

        if args.flashmap is None:
            print ("Error!!! flashmap option is not set")
            return 1
        if args.input is None:
            print ("Error!!! input option is not set")
            return 1
        if args.replace_image_filepath is None:
            print ("Error!!! replace_image_filepath option is not set")
            return 1
        if os.path.getsize (args.replace_image_filepath) > os.path.getsize (args.input):
            print ("Error!!! replace image size is larger than input image")
            return 1

        fm = Flashmap (args.flashmap)
        if args.firstmap:
            del fm
            fm = FlashmapEx (args.flashmap)
        fd = Fd ()
        replace_status = fd.replace (args.input, args.output, fm.read (fm.ibb_offset), fm.read (fm.ibb_size), args.replace_image_filepath)

        if replace_status != 0:
            print (" Error!!! Fail to generate {}".format(args.output))
        else:
            print (" output FD  : {}".format(args.output))
        return replace_status

#
# call to replace IBBR with an image
#
    def replace_ibbr_with (Argument):
        print ("Replacing IBBR ...")
        print (" input FD      : {}".format (args.input))
        print (" replace image : {}".format (args.replace_image_filepath))

        if args.flashmap is None:
            print ("Error!!! flashmap option is not set")
            return 1
        if args.input is None:
            print ("Error!!! input option is not set")
            return 1
        if args.replace_image_filepath is None:
            print ("Error!!! replace_image_filepath option is not set")
            return 1
        if os.path.getsize (args.replace_image_filepath) > os.path.getsize (args.input):
            print ("Error!!! replace image size is larger than input image")
            return 1

        fm = Flashmap (args.flashmap)
        if args.firstmap:
            del fm
            fm = FlashmapEx (args.flashmap)
        fd = Fd ()
        replace_status = fd.replace (args.input, args.output, fm.read (fm.ibbr_offset), fm.read (fm.ibbr_size), args.replace_image_filepath)

        if replace_status != 0:
            print (" Error!!! Fail to generate {}".format(args.output))
        else:
            print (" output FD  : {}".format(args.output))
        return replace_status

#
# command line support
#
    parser = argparse.ArgumentParser (prog = __prog__, description =__description__, usage = "%(prog)s [optional arguments] [positional arguments]")

    parser.add_argument ("-cl", "--checklist", type = is_valid_32bit_all, default = 0, \
                        help = "a hexadecimal number to run check items. \
                                Setting bit N lets the check routine for requirement ID N to run.\
                                No check routines run if not specified.\
                                [all] runs all check routines")

    parser.add_argument ("-rl", "--reqlist", type = is_valid_32bit_all, \
                        help = "a hexadecimal number to show requirements for each check item. \
                                Setting bit N shows requirement ID N.\
                                No requirements are shown if not specified.\
                                [all] shows requirements for all check items")

    parser.add_argument ("--flashmap", type = is_valid_file_path, \
                        help = "FlashMapInclude FDF file path")

    parser.add_argument ("--firstmap", action = "store_true",\
                        help = "If this option is set, the tool takes the first hit of PCD setting\
                                on parsing FlashMapInclude FDF. If this option is not set,\
                                the tool takes the last hit of PCD setting on parsing FlashMapInclude FDF.")

    parser.add_argument ("--topswap_size", type = is_valid_32bit_all, action = "store", \
                        help = "top swap size (hex)")

    parser.add_argument ("--replace_image_filepath", type = is_valid_file_path, \
                        help = "replace image file path")

    parser.add_argument ("-i", "--input", type = is_valid_file_path, help = "input image file path")
    parser.add_argument ("-o", "--output", default = "tmp_out.bin", help = "output image file path")
    parser.add_argument ("-ts", "--topswap", action = "store_true", help = "flag for running the script for top swap image")

    parser.add_argument ("--fsp_bin_pkg", type = is_valid_folder_path, \
                        help = "FSP Bin Pkg folder path")

    parser.add_argument ("--fspm_size_limit", type = is_valid_32bit_all, \
                        help = "FSP M size limit (hex) per platform requirement")

    subparsers = parser.add_subparsers ()
    parser_acm_alignment = subparsers.add_parser ("acm_alignment_check", help = "check ACM alignment")
    parser_acm_alignment.set_defaults (handler = check_acm_alignment)

    parser_fv_alignment = subparsers.add_parser ("fv_alignment_check", help = "check FV alignment")
    parser_fv_alignment.set_defaults (handler = check_fv_alignment)

    parser_fv_alignment = subparsers.add_parser ("fsp_size_check", help = "check FSP binary size")
    parser_fv_alignment.set_defaults (handler = check_fsp_size)

    parser_ibb_replace = subparsers.add_parser ("replace_ibb_with", help = "replace IBB region with the image specified by replace_image_filepath")
    parser_ibb_replace.set_defaults (handler = replace_ibb_with)

    parser_ibbr_replace = subparsers.add_parser ("replace_ibbr_with", help = "replace IBBR region with the image specified by replace_image_filepath")
    parser_ibbr_replace.set_defaults (handler = replace_ibbr_with)

    args = parser.parse_args ()

    returncode = args.handler (args) if hasattr (args, "handler") else 1
    return returncode

if __name__ == "__main__":
    sys.exit (main())

