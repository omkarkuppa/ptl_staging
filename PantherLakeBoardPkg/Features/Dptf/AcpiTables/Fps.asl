/** @file
  Intel ACPI Reference Code for Intel Dynamic Tuning Technology Fan Perfomance State Table

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2021 Intel Corporation.

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

    Name(FPS0, Package()
    {
          0,    // Revision:Integer
          //        Control,  TripPoint,    Speed,  NoiseLevel, Power
          Package(){100,      0xFFFFFFFF,   12000,  500,        5000},
          Package(){ 95,      0xFFFFFFFF,   11600,  475,        4750},
          Package(){ 90,      0xFFFFFFFF,   11200,  450,        4500},
          Package(){ 85,      0xFFFFFFFF,   10500,  425,        4250},
          Package(){ 80,      0xFFFFFFFF,    9800,  400,        4000},
          Package(){ 70,      0xFFFFFFFF,    9300,  350,        3500},
          Package(){ 60,      0xFFFFFFFF,    7400,  300,        3000},
          Package(){ 50,      0xFFFFFFFF,    6300,  250,        2500},
          Package(){ 40,      0xFFFFFFFF,    5100,  200,        2000},
          Package(){ 30,      0xFFFFFFFF,    3800,  150,        1500},
          Package(){ 25,      0xFFFFFFFF,    3200,  125,        1250},
          Package(){  0,      0xFFFFFFFF,       0,    0,           0} // OFF
    })
