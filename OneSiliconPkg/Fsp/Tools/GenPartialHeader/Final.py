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

global reserved_index

reseved_index = 0

upd_name_change_list = OrderedDict()

if len(sys.argv) == 4:
    arg_1=str(sys.argv[1])
    arg_2 = str(sys.argv[2])
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

output_header = open("Final" + type + "Upd.h", "wt")

def Remove_duplicate(input_list):
    final_list = []
    for x in input_list:
        if x not in final_list:
            final_list.append(x)
    return final_list

def scan_main_upd(file):
    temp_list=[]
    for num, line in enumerate(file):
        #print line.split("\n")[0]
        temp_list.append(line.split("\n")[0])
    return temp_list

def test_conf_exist(file):

    for num, line in enumerate(file):
        if header_string+" Restricted Configuration" in line:
            return True
    return False

def scan_all_upd(file):
    temp_list = []
    start = 0
    for num, line in enumerate(file):
        if header_string + " Configuration" in line:
            start = 1

        if header_string + " UPD Configuration" in line:
            start = 0

        if start == 1:
            if "**/" in line:
                if "typedef struct {" not in header_temp[num + 1]:
                    # print(header_temp[num+1].split(";\n")[0].replace("   "," ").replace("   "," ")
                    #    .replace("   "," ").replace("   "," ")
                    #    .replace("   "," ").replace("  "," ").split(" ")[2])
                    temp_upd = (header_temp[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("  ", " ").split(" ")[2])
                    if "[" in temp_upd:
                        temp_upd = temp_upd.split("[")[0]
                    temp_list.append(temp_upd)

    return temp_list

def scan_all_upd_conf(file,temp_file,test_conf):
    temp_list = []
    start = 0
    for num, line in enumerate(file):
        if header_string+" Configuration" in line:
            start = 1
        if test_conf:
            if header_string+" Restricted Configuration" in line:
                start = 0
        else:
            if header_string + " UPD Configuration" in line:
                start = 0

        if start == 1:
            if "**/" in line:
                if "typedef struct {" not in temp_file[num + 1]:
                     #print(temp_file[num+1].split(";\n")[0].replace("   "," ").replace("   "," ")
                     #   .replace("   "," ").replace("   "," ")
                     #   .replace("   "," ").replace("  "," ").split(" ")[2])
                    temp_upd = (temp_file[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("  ", " ").split(" ")[2])
                    temp_upd_type= (temp_file[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("  ", " ").split(" ")[1])

                    #print (temp_upd+": "+temp_upd_type)
                    temp_list.append(temp_upd+": "+temp_upd_type)

    return temp_list

def scan_all_upd_test_conf(file,temp_file):
    temp_list=[]
    start=0
    for num, line in enumerate(file):
        if header_string+" Restricted Configuration" in line:
            start=1

        if header_string+" UPD Configuration" in line:
            start=0

        if start==1:
            if "**/" in line:
                if "typedef struct {" not in temp_file[num + 1]:
                     #print(temp_file[num+1].split(";\n")[0].replace("   "," ").replace("   "," ")
                     #   .replace("   "," ").replace("   "," ")
                     #   .replace("   "," ").replace("  "," ").split(" ")[2])
                    temp_upd = (temp_file[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("  ", " ").split(" ")[2])
                    temp_upd_type= (temp_file[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("   ", " ")
                       .replace("   ", " ").replace("  ", " ").split(" ")[1])

                    #print (temp_upd+": "+temp_upd_type)
                    temp_list.append(temp_upd + ": " + temp_upd_type)

    return temp_list

def upd_type_change(upd_list,main_upd):
    temp_list=[]
    for upd in upd_list:
        key = upd.split(": ")[0]
        value = upd.split(": ")[1]
        if key not in main_upd:
            type=int(value.split("UINT")[1])
            array=type//8

            if array!=1:
                if "[" in key:
                    index=int(key.split("[")[1].split("]")[0]) * array
                    key = key.split("[")[0]
                    key = key + "[" + str(index) + "]"
                    value = "UINT8"
                else:
                    key=key+"["+str(array)+"]"
                    value="UINT8"
            temp_list.append(key+": "+value)
            #print (key+": "+value)
        else:
            temp_list.append(key + ": " + value)
            #print key + ": " + value

    return temp_list

def get_key_value(upd_conf_list,upd,upd_type):
    for x in upd_conf_list:
        if upd.split("[")[0] == x.split(": ")[0].split("[")[0]:
            if upd_type != x.split(": ")[1]:
                return x
    return None

def merge_reserved(struct,num,index):
    start = 1
    count = 0
    main_num = num
    while(start == 1):
        if "/** Offset" in header_replace[num] and " - Reserved\n" not in header_replace[num]:
            start = 0
        if "} " in header_replace[num]:
            start = 0
        if "UINT8" in header_replace[num]:
            #print (header_replace[num])
            temp_upd = (header_replace[num].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                .replace("   ", " ").replace("   ", " ")
                .replace("   ", " ").replace("  ", " ").split(" ")[2])
            if "[" in temp_upd:
                #print(temp_upd +":"+ temp_upd.split("[")[1].split("]")[0])
                count = count + int(temp_upd.split("[")[1].split("]")[0])
            else:
                count = count + 1
        num = num +1
    #print("Count: " + str(count))
    if count == 1:
        org_upd = (header_replace[main_num].split(";\n")[0].replace("   ", " ").replace("   ", " ")
            .replace("   ", " ").replace("   ", " ")
            .replace("   ", " ").replace("  ", " ").split(" ")[2])
        #print("Org: " + org_upd)
        #print("replaced: Reserved" + str(index))
        struct.append(header_replace[main_num].replace(org_upd, "Reserved" + str(index)))
        #print(header_replace[main_num].replace(org_upd, "Reserved" + str(index)))
    else:
        org_upd = (header_replace[main_num].split(";\n")[0].replace("   ", " ").replace("   ", " ")
            .replace("   ", " ").replace("   ", " ")
            .replace("   ", " ").replace("  ", " ").split(" ")[2])
        #print("Org: " + org_upd)
        #print("replaced: Reserved" + str(index) + "[" + str(count) + "]")
        struct.append(header_replace[main_num].replace(org_upd,"Reserved"+str(index)+"["+str(count)+"]"))
        #print(header_replace[main_num].replace(org_upd,"Reserved"+str(index)+"["+str(count)+"]"))

    return struct

def  merge_conf_reserved(conf_struct,test_conf):
    start = 0
    count_start = 0
    global reseved_index
    for num, line in enumerate(header):
        if header_string+" Configuration" in line:
            start = 1
        if test_conf:
            if header_string+" Restricted Configuration" in line:
                conf_struct.pop()
                conf_struct.append(header_replace[num - 2])
                conf_struct.append(header_replace[num - 1])
                start = 0
        else:
            if header_string + " UPD Configuration" in line:
                conf_struct.pop()
                conf_struct.append(header_replace[num-2])
                conf_struct.append(header_replace[num-1])
                start = 0
        if start == 1:
            if "/** Offset" in header_replace[num]:
                if len(header_replace[num].split(" - ")) >= 2:
                    if header_replace[num].split("\n")[0].split(" - ")[1] == "Reserved":
                        if count_start == 0:
                            conf_struct.append(header_replace[num])
                            conf_struct.append(header_replace[num+1])
                            #print(header_replace[num].split("\n")[0])
                            #print(header_replace[num+1].split("\n")[0])
                            for x in range (10):
                                if "UINT" in header_replace[num+x+2].split("\n")[0]:
                                    #print("line 2:"+header_replace[num+x+2].split("\n")[0])
                                    conf_struct = merge_reserved(conf_struct,num+x+2,reseved_index)
                                    #print("line 3:"+header_replace[num+x+3].split("\n")[0])
                                    conf_struct.append(header_replace[num+x+3])
                                    break
                                else:
                                    conf_struct.append(header_replace[num+x+2])
                            reseved_index = reseved_index + 1
                            count_start = 1
                    else:
                        count_start = 0
                        conf_struct.append(header_replace[num])
                        #print(header_replace[num].split("\n")[0])
                else:
                    if count_start == 0:
                        conf_struct.append(header_replace[num])
                        #print(header_replace[num].split("\n")[0])
            else:
                if count_start == 0:
                    conf_struct.append(header_replace[num])
                    #print(header_replace[num].split("\n")[0])

    return conf_struct

def  merge_test_conf_reserved(test_conf_struct):
    start = 0
    count_start = 0
    global reseved_index

    for num, line in enumerate(header):
        if header_string+" Restricted Configuration" in line:
            start = 1
        if header_string + " UPD Configuration" in line:
            test_conf_struct.pop()
            test_conf_struct.append(header_replace[num - 2])
            test_conf_struct.append(header_replace[num - 1])
            start = 0

        if start == 1:
            if "/** Offset" in header_replace[num]:
                if len(header_replace[num].split(" - ")) == 2:
                    if header_replace[num].split("\n")[0].split(" - ")[1] == "Reserved":
                        if count_start == 0:
                            test_conf_struct.append(header_replace[num])
                            test_conf_struct.append(header_replace[num+1])
                            #print(header_replace[num].split("\n")[0])
                            #print(header_replace[num+1].split("\n")[0])
                            for x in range (10):
                                if "UINT" in header_replace[num+x+2].split("\n")[0]:
                                    #print("line 2:"+header_replace[num+x+2].split("\n")[0])
                                    test_conf_struct = merge_reserved(test_conf_struct,num+x+2,reseved_index)
                                    #print("line 3:"+header_replace[num+x+3].split("\n")[0])
                                    test_conf_struct.append(header_replace[num+x+3])
                                    break
                                else:
                                    test_conf_struct.append(header_replace[num+x+2])
                            reseved_index = reseved_index + 1
                            count_start = 1
                    else:
                        count_start = 0
                        test_conf_struct.append(header_replace[num])
                        #print(header_replace[num].split("\n")[0])
                else:
                    if count_start == 0:
                        test_conf_struct.append(header_replace[num])
                        #print(header_replace[num].split("\n")[0])
            else:
                if count_start == 0:
                    test_conf_struct.append(header_replace[num])
                    #print(header_replace[num].split("\n")[0])

    return test_conf_struct

def replace_reserved_type(upd_test_conf_list):
    start = 0
    for num, line in enumerate(header):
        if header_string + " Configuration" in line:
            start=1

        if header_string+" UPD Configuration" in line:
            start=0

        if start == 1:
            if "**/" in line:
                if "typedef struct {" not in header_temp[num + 1]:
                    temp_upd = (header_temp[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("  ", " ").split(" ")[2])
                    temp_upd_type = (header_temp[num + 1].split(";\n")[0].replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("   ", " ")
                        .replace("   ", " ").replace("  ", " ").split(" ")[1])
                    if "Reserved" in temp_upd.split("[")[0]:
                        key_value = get_key_value(upd_test_conf_list, temp_upd, temp_upd_type)
                        if key_value != None:
                            header_replace[num + 1] = header_replace[num + 1].replace(temp_upd,
                                    key_value.split(": ")[0]).replace(temp_upd_type, key_value.split(": ")[1])
    return

def wright_header_file(conf_struct,test_conf_struct,test_conf):
    start_conf = 0
    start_test_conf = 0
    conf_struct_index = 0
    test_conf_struct_index = 0

    for num, line in enumerate(header):
        if header_string+" Configuration" in line:
            start_conf = 1

        if header_string+" Restricted Configuration" in line:
                start_test_conf = 1
                start_conf = 0

        if header_string + " UPD Configuration" in line:
                start_test_conf = 0
                start_conf = 0

        if start_conf == 1:
            if len(conf_struct)-1 >= conf_struct_index:
                output_header.write(conf_struct[conf_struct_index])
                conf_struct_index = conf_struct_index + 1
        elif start_test_conf == 1:
            if len(test_conf_struct)-1 >= test_conf_struct_index:
                output_header.write(test_conf_struct[test_conf_struct_index])
                test_conf_struct_index = test_conf_struct_index + 1
        else:
            output_header.write(header_replace[num])
    return

def main():
    conf_struct = []
    test_conf_struct = []

    all_upd = scan_all_upd(header)

    all_upd = Remove_duplicate(all_upd)

    main_upd = scan_main_upd(upd_txt)

    main_upd = Remove_duplicate(main_upd)

    upd_conf_list = scan_all_upd_conf(header, header_temp,test_conf_exist(header))

    upd_conf_list = Remove_duplicate(upd_conf_list)

    #print "upd_conf"
    upd_conf_list = upd_type_change(upd_conf_list, main_upd)
    replace_reserved_type(upd_conf_list)
    #print(upd_conf_list)

    conf_struct=merge_conf_reserved(conf_struct,test_conf_exist(header))

    if test_conf_exist(header):
        upd_test_conf_list = scan_all_upd_test_conf(header, header_temp)

        upd_test_conf_list = Remove_duplicate(upd_test_conf_list)

        #print "upd_conf_test"
        upd_test_conf_list = upd_type_change(upd_test_conf_list, main_upd)
        replace_reserved_type(upd_test_conf_list)
        #print(upd_test_conf_list)
        test_conf_struct=merge_test_conf_reserved(test_conf_struct)

    wright_header_file(conf_struct, test_conf_struct,test_conf_exist(header))

    return


if __name__ == "__main__":
    main()
