# @file
#
#  @copyright
#  INTEL CONFIDENTIAL
#  Copyright (C) 2019 Intel Corporation.
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

import sys


if len(sys.argv) == 4 :
    arg_1=str(sys.argv[1])
    arg_2=str(sys.argv[2])
    type=str(sys.argv[3])
    header = open(arg_1, "rt").readlines()
    header_replace = open(arg_1, "rt").readlines()
    header_temp = open(arg_1, "rt").readlines()
    upd_txt = open(arg_2, "rt").readlines()
else:
    print ("please enter all  argument")

if type=="fspm" or type=="Fspm" or type=="FSPM":
    header_string="Fsp M"
    type="Fspm"
elif type=="fsps" or type=="Fsps" or type=="FSPS":
    header_string="Fsp S"
    type="Fsps"
else:
    print ("Please input correct header type")
output_header = open("new" + type + "Upd.h", "wt")


def Remove_duplicate(input_list):
    final_list = []
    for x in input_list:
        if x not in final_list:
            final_list.append(x)
    return final_list

def scan_all_upd(file):
    temp_list=[]
    start=0
    for num, line in enumerate(file):
        if header_string+" Configuration" in line:
            start=1

        if header_string+" UPD Configuration" in line:
            start=0

        if start==1:
            if "**/" in line:
                if "typedef struct {" not in header_temp[num+1]:
                    #print(header_temp[num+1].split(";\n")[0].replace("   "," ").replace("   "," ")
                    #    .replace("   "," ").replace("   "," ")
                    #    .replace("   "," ").replace("  "," ").split(" ")[2])
                    temp_upd=(header_temp[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("  ", " ").split(" ")[2])
                    if "[" in temp_upd:
                        temp_upd=temp_upd.split("[")[0]
                    temp_list.append(temp_upd)

    return temp_list

def scan_main_upd(file):
    temp_list=[]
    for num, line in enumerate(file):
        #print line.split("\n")[0]
        temp_list.append(line.split("\n")[0])
    return temp_list

def replace_upd(temp):
    start=0
    index=0
    for num, line in enumerate(header):
        if header_string+" Configuration" in line:
            start=1
        if header_string+" UPD Configuration" in line:
            start=0

        if start==1:
            for upd in temp:
                if upd in line:
                    if "**/" in header_temp[num-1]:
                        temp_upd = (line.split(";\n")[0].replace("   ", " ").replace("   ", " ")
                            .replace("   ", " ").replace("   ", " ")
                            .replace("   ", " ").replace("  ", " ").split(" ")[2])
                        if "[" in temp_upd:
                            temp_upd = temp_upd.split("[")[0]
                        if upd == temp_upd:
                            #print(line.replace(upd,"Reserved"+str(index)))
                            header_replace[num]=line.replace(upd,"Reserved"+str(index))
                            index = index + 1
    return

def start_replace(num,temp):
    start=0
    for x in range(25):
        if "**/" in header_temp[num+x]:
            start=1

        if start== 1:
            new_num=num+x+1
            break
    temp_upd = (header_temp[num + x + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
        .replace("   ", " ").replace("   ", " ")
        .replace("   ", " ").replace("  ", " ").split(" ")[2])
    if "[" in temp_upd:
        temp_upd = temp_upd.split("[")[0]

    for upd in temp:
        if upd == temp_upd:
            return False

    return True

def wright_new_header(temp):
    start=0
    end=0
    for num, line in enumerate(header_replace):
        if ("/** Offset" in line) and end==0:
            if start_replace(num,temp):
                start=1
                index = 0

        if header_string+" UPD Configuration" in line:
            end=1

        if start==1:
            if "/** Offset" in line:
                newline=line.split("\n")[0].split(" -")[0]
                output_header.write(newline+" - Reserved\n")

            elif "**/" in line:
                output_header.write(line)
                start=0
            else:
                if index==0:
                    #output_header.write("  Reserved\n")
                    index=1
        else:
            output_header.write(line)
    return


def main():
    error_upd = []
    all_upd=scan_all_upd(header)

    all_upd=Remove_duplicate(all_upd)
    UPD_in_Header = len(all_upd)

    main_upd=scan_main_upd(upd_txt)

    main_upd=Remove_duplicate(main_upd)

    temp_upd_list=all_upd

    for x in main_upd:
        if x in temp_upd_list:
            error_upd.append(x)
            temp_upd_list.remove(x)

    replace_upd(temp_upd_list)
    wright_new_header(main_upd)

    print ("-----------------------------------------")
    print ("------Partial " + type + " Header Generated------")
    print ("-----------------------------------------")
    print ("Number of UPD in Header:" + str(UPD_in_Header))
    print ("Number of UPD used in Coreboot:" + str(len(main_upd)))
    print ("Number of UPD Hidden in New Header:" + str(UPD_in_Header-len(error_upd)))
    print ("Number of UPD Visible in New Header:" + str(len(error_upd)))
    print ("-----------------------------------------")

    if (len(main_upd) - len(error_upd)) != 0:
        error_log = open(type + "_error.log", "wt")
        error_confirm = open("fsp_error_log.txt", "wt")
        error_confirm.write("yes")
        error_log.write("-----------------------------------------\n")
        print ("-----------------ERROR-------------------")
        error_log.write("----------------ERROR------------------\n")
        print ("-----------------------------------------")
        error_log.write("-----------------------------------------\n")
        print ("Number of UPD Not Found in "+type+" header: " + str(len(main_upd) - len(error_upd)))
        error_log.write("Number of UPD Not Found in Header:" + str(len(main_upd) - len(error_upd)) + "\n")
        print ("NOTE: Please check " + type + "_error.log to see")
        print ("      list of UPD which is not there in")
        error_log.write("NOTE: Here are the list of UPD which is not there in header\n")
        print ("      header. Please use correct header")
        print ("      or check UPD name in list and Run")
        error_log.write("      Please use correct header or check UPD name in list\n")
        print ("      this script again")
        error_log.write("      and Run this script again\n")

        # print error_upd
        for y in main_upd:
            if y not in error_upd:
                print("UPD not found in header:" + y)
                error_log.write("UPD not found in header: " + y + "\n")
    else:
        print ("---------------NO ERRORS-----------------")
        print ("-----------------------------------------")
        print ("")
    return


if __name__ == "__main__":
    main()
