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
from collections import OrderedDict

upd_name_change_list = OrderedDict()

if len(sys.argv) == 3 :
    arg_1=str(sys.argv[1])
    type=str(sys.argv[2])
    header = open(arg_1, "rt").readlines()
    header_replace = open(arg_1, "rt").readlines()
    header_temp = open(arg_1, "rt").readlines()
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

output_header = open("NC" + type + "Upd.h", "wt")
upd_list_file = open(type+"_name_change_list.txt", "wt")

def replace_upd():
    start=0
    index=0
    for num, line in enumerate(header):
        if header_string+" Configuration" in line:
            start=1
        if header_string+" UPD Configuration" in line:
            start=0

        if start==1:
            for upd in upd_name_change_list.keys():
                if upd in line:
                    if "**/" in header_temp[num-1]:
                        temp_upd = (line.split(";\n")[0].replace("   ", " ").replace("   ", " ")
                            .replace("   ", " ").replace("   ", " ")
                            .replace("   ", " ").replace("  ", " ").split(" ")[2])
                        if "[" in temp_upd:
                            temp_upd = temp_upd.split("[")[0]
                        if upd == temp_upd:
                            header_replace[num]=line.replace(upd,upd_name_change_list[upd])
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
            return [True,upd]

    return [False,temp_upd]

def comment_wright_new_header():
    start=0
    end=0
    for num, line in enumerate(header_replace):
        if ("/** Offset" in line) and end==0:
            start_upd=start_replace(num,upd_name_change_list.keys())
            if start_upd[0]:
                start=1
                index = 0

        if header_string+" UPD Configuration" in line:
            end=1

        if start==1:
            if "/** Offset" in line:
                newline=line.split("\n")[0].split(" - ")[0]
                output_header.write(newline+" - "+ upd_name_change_list[start_upd[1]] + "\n")

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

def get_upd_name_change_list(file):

    for num, line in enumerate(file):
        if " | " in line:
            #print(line.split("\n")[0].split(" | ")[0] + " | " + line.split("\n")[0].split(" | ")[1])
            upd_name_change_list[line.split("\n")[0].split(" | ")[0]]=line.split("\n")[0].split(" | ")[1]
    return

def get_comment(num):
    comment=""
    while ("**/" not in header_temp[num]):
        #print(header_temp[num])
        comment = comment + header_temp[num].split("\n")[0]
        num = num + 1

    comment = comment.split("/** ")[1]
    upd = header_temp[num+1].split(";\n")[0].replace("   ", " ").replace("   ", " ").replace("   ", " ").replace("   ", " ").replace("   ", " ").replace("  ", " ").split(" ")[2]
    if "[" in upd:
        upd = upd.split("[")[0]

    return [comment,upd]

def write_upd_name_change_list():
    temp=[]
    start = 0
    for num, line in enumerate(header):
        if header_string + " Configuration" in line:
            start = 1
        if header_string + " UPD Configuration" in line:
            start = 0

        if start == 1:
            if "/** Offset" in line:
                comment_and_upd=get_comment(num)
                #print(comment_and_upd[1] +": "+ comment_and_upd[0])
                if "[ALIAS_NAME" in comment_and_upd[0]:
                    alias_name = comment_and_upd[0].split("[ALIAS_NAME")[1].split("]")[0].replace(" ", "")
                    #print(comment_and_upd[1] + " | " +alias_name)
                    upd_list_file.write(comment_and_upd[1] + " | " + alias_name + "\n")
                    temp.append(comment_and_upd[1] + " | " + alias_name + "\n")

    return temp

def main():

    upd_txt=write_upd_name_change_list()
    get_upd_name_change_list(upd_txt)
    replace_upd()
    comment_wright_new_header()
    return


if __name__ == "__main__":
    main()