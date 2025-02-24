/** @file
  GPIO ASL header for PTL_PCD.

  @copyright
  INTEL CONFIDENTIAL
  Copyright (C) 2023 Intel Corporation.

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

#ifndef _GPIOV2_ACPI_DEFINES_PTL_PCD_H_
#define _GPIOV2_ACPI_DEFINES_PTL_PCD_H_

#define GPIO_HID_PTL_PCD_H "INTC10BB" // PTL PCD-H GPIO
#define GPIO_HID_PTL_PCD_P "INTC10BC" // PTL PCD-P GPIO

//
// PCH GPIO Community address
//
#define PTL_PCD_GPIOV2_COM0                               0x590000 // PID: 0xF259
#define PTL_PCD_GPIOV2_COM1                               0x5A0000 // PID: 0xF25A
#define PTL_PCD_GPIOV2_COM3                               0x5B0000 // PID: 0xF25B
#define PTL_PCD_GPIOV2_COM4                               0x5C0000 // PID: 0xF25C
#define PTL_PCD_GPIOV2_COM5                               0x5D0000 // PID: 0xF25D


//
// GPP_V
//
#define GPIOV2_PTL_DRIVER_xxgpp_v_0                       0    // GPP_V xxgpp_v_0
#define GPIOV2_PTL_DRIVER_xxgpp_v_1                       1    // GPP_V xxgpp_v_1
#define GPIOV2_PTL_DRIVER_xxgpp_v_2                       2    // GPP_V xxgpp_v_2
#define GPIOV2_PTL_DRIVER_xxgpp_v_3                       3    // GPP_V xxgpp_v_3
#define GPIOV2_PTL_DRIVER_xxgpp_v_4                       4    // GPP_V xxgpp_v_4
#define GPIOV2_PTL_DRIVER_xxgpp_v_5                       5    // GPP_V xxgpp_v_5
#define GPIOV2_PTL_DRIVER_xxgpp_v_6                       6    // GPP_V xxgpp_v_6
#define GPIOV2_PTL_DRIVER_xxgpp_v_7                       7    // GPP_V xxgpp_v_7
#define GPIOV2_PTL_DRIVER_xxgpp_v_8                       8    // GPP_V xxgpp_v_8
#define GPIOV2_PTL_DRIVER_xxgpp_v_9                       9    // GPP_V xxgpp_v_9
#define GPIOV2_PTL_DRIVER_xxgpp_v_10                      10   // GPP_V xxgpp_v_10
#define GPIOV2_PTL_DRIVER_xxgpp_v_11                      11   // GPP_V xxgpp_v_11
#define GPIOV2_PTL_DRIVER_xxgpp_v_12                      12   // GPP_V xxgpp_v_12
#define GPIOV2_PTL_DRIVER_xxgpp_v_13                      13   // GPP_V xxgpp_v_13
#define GPIOV2_PTL_DRIVER_xxgpp_v_14                      14   // GPP_V xxgpp_v_14
#define GPIOV2_PTL_DRIVER_xxgpp_v_15                      15   // GPP_V xxgpp_v_15
#define GPIOV2_PTL_DRIVER_xxgpp_v_16                      16   // GPP_V xxgpp_v_16
#define GPIOV2_PTL_DRIVER_xxgpp_v_17                      17   // GPP_V xxgpp_v_17
#define GPIOV2_PTL_DRIVER_xxsys_pwrok                     18   // GPP_V xxsys_pwrok
#define GPIOV2_PTL_DRIVER_xxsys_reset_b                   19   // GPP_V xxsys_reset_b
#define GPIOV2_PTL_DRIVER_xxl_bklten                      20   // GPP_V xxl_bklten
#define GPIOV2_PTL_DRIVER_xxl_bkltctl                     21   // GPP_V xxl_bkltctl
#define GPIOV2_PTL_DRIVER_xxl_vdden                       22   // GPP_V xxl_vdden
#define GPIOV2_PTL_DRIVER_xxmlk_rst_b                     23   // GPP_V xxmlk_rst_b

//
// GPP_C
//
#define GPIOV2_PTL_DRIVER_xxgpp_c_0                       0    // GPP_C xxgpp_c_0
#define GPIOV2_PTL_DRIVER_xxgpp_c_1                       1    // GPP_C xxgpp_c_1
#define GPIOV2_PTL_DRIVER_xxgpp_c_2                       2    // GPP_C xxgpp_c_2
#define GPIOV2_PTL_DRIVER_xxgpp_c_3                       3    // GPP_C xxgpp_c_3
#define GPIOV2_PTL_DRIVER_xxgpp_c_4                       4    // GPP_C xxgpp_c_4
#define GPIOV2_PTL_DRIVER_xxgpp_c_5                       5    // GPP_C xxgpp_c_5
#define GPIOV2_PTL_DRIVER_xxgpp_c_6                       6    // GPP_C xxgpp_c_6
#define GPIOV2_PTL_DRIVER_xxgpp_c_7                       7    // GPP_C xxgpp_c_7
#define GPIOV2_PTL_DRIVER_xxgpp_c_8                       8    // GPP_C xxgpp_c_8
#define GPIOV2_PTL_DRIVER_xxgpp_c_9                       9    // GPP_C xxgpp_c_9
#define GPIOV2_PTL_DRIVER_xxgpp_c_10                      10   // GPP_C xxgpp_c_10
#define GPIOV2_PTL_DRIVER_xxgpp_c_11                      11   // GPP_C xxgpp_c_11
#define GPIOV2_PTL_DRIVER_xxgpp_c_12                      12   // GPP_C xxgpp_c_12
#define GPIOV2_PTL_DRIVER_xxgpp_c_13                      13   // GPP_C xxgpp_c_13
#define GPIOV2_PTL_DRIVER_xxgpp_c_14                      14   // GPP_C xxgpp_c_14
#define GPIOV2_PTL_DRIVER_xxgpp_c_15                      15   // GPP_C xxgpp_c_15
#define GPIOV2_PTL_DRIVER_xxgpp_c_16                      16   // GPP_C xxgpp_c_16
#define GPIOV2_PTL_DRIVER_xxgpp_c_17                      17   // GPP_C xxgpp_c_17
#define GPIOV2_PTL_DRIVER_xxgpp_c_18                      18   // GPP_C xxgpp_c_18
#define GPIOV2_PTL_DRIVER_xxgpp_c_19                      19   // GPP_C xxgpp_c_19
#define GPIOV2_PTL_DRIVER_xxgpp_c_20                      20   // GPP_C xxgpp_c_20
#define GPIOV2_PTL_DRIVER_xxgpp_c_21                      21   // GPP_C xxgpp_c_21
#define GPIOV2_PTL_DRIVER_xxgpp_c_22                      22   // GPP_C xxgpp_c_22
#define GPIOV2_PTL_DRIVER_xxgpp_c_23                      23   // GPP_C xxgpp_c_23

//
// GPP_F
//
#define GPIOV2_PTL_DRIVER_xxgpp_f_0                       0    // GPP_F xxgpp_f_0
#define GPIOV2_PTL_DRIVER_xxgpp_f_1                       1    // GPP_F xxgpp_f_1
#define GPIOV2_PTL_DRIVER_xxgpp_f_2                       2    // GPP_F xxgpp_f_2
#define GPIOV2_PTL_DRIVER_xxgpp_f_3                       3    // GPP_F xxgpp_f_3
#define GPIOV2_PTL_DRIVER_xxgpp_f_4                       4    // GPP_F xxgpp_f_4
#define GPIOV2_PTL_DRIVER_xxgpp_f_5                       5    // GPP_F xxgpp_f_5
#define GPIOV2_PTL_DRIVER_xxgpp_f_6                       6    // GPP_F xxgpp_f_6
#define GPIOV2_PTL_DRIVER_xxgpp_f_7                       7    // GPP_F xxgpp_f_7
#define GPIOV2_PTL_DRIVER_xxgpp_f_8                       8    // GPP_F xxgpp_f_8
#define GPIOV2_PTL_DRIVER_xxgpp_f_9                       9    // GPP_F xxgpp_f_9
#define GPIOV2_PTL_DRIVER_xxgpp_f_10                      10   // GPP_F xxgpp_f_10
#define GPIOV2_PTL_DRIVER_xxgpp_f_11                      11   // GPP_F xxgpp_f_11
#define GPIOV2_PTL_DRIVER_xxgpp_f_12                      12   // GPP_F xxgpp_f_12
#define GPIOV2_PTL_DRIVER_xxgpp_f_13                      13   // GPP_F xxgpp_f_13
#define GPIOV2_PTL_DRIVER_xxgpp_f_14                      14   // GPP_F xxgpp_f_14
#define GPIOV2_PTL_DRIVER_xxgpp_f_15                      15   // GPP_F xxgpp_f_15
#define GPIOV2_PTL_DRIVER_xxgpp_f_16                      16   // GPP_F xxgpp_f_16
#define GPIOV2_PTL_DRIVER_xxgpp_f_17                      17   // GPP_F xxgpp_f_17
#define GPIOV2_PTL_DRIVER_xxgpp_f_18                      18   // GPP_F xxgpp_f_18
#define GPIOV2_PTL_DRIVER_xxgpp_f_19                      19   // GPP_F xxgpp_f_19
#define GPIOV2_PTL_DRIVER_xxgpp_f_20                      20   // GPP_F xxgpp_f_20
#define GPIOV2_PTL_DRIVER_xxgpp_f_21                      21   // GPP_F xxgpp_f_21
#define GPIOV2_PTL_DRIVER_xxgpp_f_22                      22   // GPP_F xxgpp_f_22
#define GPIOV2_PTL_DRIVER_xxgpp_f_23                      23   // GPP_F xxgpp_f_23
#define GPIOV2_PTL_DRIVER_thc1_gspi1_i3c2_clk_loopbk      24   // GPP_F thc1_gspi1_i3c2_clk_loopbk
#define GPIOV2_PTL_DRIVER_a_gspi0_clk_loopbk              25   // GPP_F a_gspi0_clk_loopbk

//
// GPP_E
//
#define GPIOV2_PTL_DRIVER_xxgpp_e_0                       0    // GPP_E xxgpp_e_0
#define GPIOV2_PTL_DRIVER_xxgpp_e_1                       1    // GPP_E xxgpp_e_1
#define GPIOV2_PTL_DRIVER_xxgpp_e_2                       2    // GPP_E xxgpp_e_2
#define GPIOV2_PTL_DRIVER_xxgpp_e_3                       3    // GPP_E xxgpp_e_3
#define GPIOV2_PTL_DRIVER_xxgpp_e_4                       4    // GPP_E xxgpp_e_4
#define GPIOV2_PTL_DRIVER_xxgpp_e_5                       5    // GPP_E xxgpp_e_5
#define GPIOV2_PTL_DRIVER_xxgpp_e_6                       6    // GPP_E xxgpp_e_6
#define GPIOV2_PTL_DRIVER_xxgpp_e_7                       7    // GPP_E xxgpp_e_7
#define GPIOV2_PTL_DRIVER_xxgpp_e_8                       8    // GPP_E xxgpp_e_8
#define GPIOV2_PTL_DRIVER_xxgpp_e_9                       9    // GPP_E xxgpp_e_9
#define GPIOV2_PTL_DRIVER_xxgpp_e_10                      10   // GPP_E xxgpp_e_10
#define GPIOV2_PTL_DRIVER_xxgpp_e_11                      11   // GPP_E xxgpp_e_11
#define GPIOV2_PTL_DRIVER_xxgpp_e_12                      12   // GPP_E xxgpp_e_12
#define GPIOV2_PTL_DRIVER_xxgpp_e_13                      13   // GPP_E xxgpp_e_13
#define GPIOV2_PTL_DRIVER_xxgpp_e_14                      14   // GPP_E xxgpp_e_14
#define GPIOV2_PTL_DRIVER_xxgpp_e_15                      15   // GPP_E xxgpp_e_15
#define GPIOV2_PTL_DRIVER_xxgpp_e_16                      16   // GPP_E xxgpp_e_16
#define GPIOV2_PTL_DRIVER_xxgpp_e_17                      17   // GPP_E xxgpp_e_17
#define GPIOV2_PTL_DRIVER_xxgpp_e_18                      18   // GPP_E xxgpp_e_18
#define GPIOV2_PTL_DRIVER_xxgpp_e_19                      19   // GPP_E xxgpp_e_19
#define GPIOV2_PTL_DRIVER_xxgpp_e_20                      20   // GPP_E xxgpp_e_20
#define GPIOV2_PTL_DRIVER_xxgpp_e_21                      21   // GPP_E xxgpp_e_21
#define GPIOV2_PTL_DRIVER_xxgpp_e_22                      22   // GPP_E xxgpp_e_22
#define GPIOV2_PTL_DRIVER_xxboothalt_b                    23   // GPP_E xxboothalt_b
#define GPIOV2_PTL_DRIVER_thc0_gspi0_clk_loopbk           24   // GPP_E thc0_gspi0_clk_loopbk

//
// CPUJTAG
//
#define GPIOV2_PTL_DRIVER_xxepd_on                        0    // CPUJTAG xxepd_on
#define GPIOV2_PTL_DRIVER_xxvdd2_pwrgd                    1    // CPUJTAG xxvdd2_pwrgd
#define GPIOV2_PTL_DRIVER_xxjtag_mbpb0                    2    // CPUJTAG xxjtag_mbpb0
#define GPIOV2_PTL_DRIVER_xxjtag_mbpb1                    3    // CPUJTAG xxjtag_mbpb1
#define GPIOV2_PTL_DRIVER_xxjtag_mbpb2                    4    // CPUJTAG xxjtag_mbpb2
#define GPIOV2_PTL_DRIVER_xxjtag_mbpb3                    5    // CPUJTAG xxjtag_mbpb3
#define GPIOV2_PTL_DRIVER_xxjtag_tdo                      6    // CPUJTAG xxjtag_tdo
#define GPIOV2_PTL_DRIVER_xxprdy_b                        7    // CPUJTAG xxprdy_b
#define GPIOV2_PTL_DRIVER_xxpreq_b                        8    // CPUJTAG xxpreq_b
#define GPIOV2_PTL_DRIVER_xxjtag_tdi                      9    // CPUJTAG xxjtag_tdi
#define GPIOV2_PTL_DRIVER_xxjtag_tms                      10   // CPUJTAG xxjtag_tms
#define GPIOV2_PTL_DRIVER_xxjtag_tck                      11   // CPUJTAG xxjtag_tck
#define GPIOV2_PTL_DRIVER_xxdbg_pmode                     12   // CPUJTAG xxdbg_pmode
#define GPIOV2_PTL_DRIVER_xxjtag_trst_b                   13   // CPUJTAG xxjtag_trst_b
#define GPIOV2_PTL_DRIVER_xxddsp_hpdalv                   14   // CPUJTAG xxddsp_hpdalv

//
// GPP_H
//
#define GPIOV2_PTL_DRIVER_xxgpp_h_0                       0    // GPP_H xxgpp_h_0
#define GPIOV2_PTL_DRIVER_xxgpp_h_1                       1    // GPP_H xxgpp_h_1
#define GPIOV2_PTL_DRIVER_xxgpp_h_2                       2    // GPP_H xxgpp_h_2
#define GPIOV2_PTL_DRIVER_xxgpp_h_3                       3    // GPP_H xxgpp_h_3
#define GPIOV2_PTL_DRIVER_xxgpp_h_4                       4    // GPP_H xxgpp_h_4
#define GPIOV2_PTL_DRIVER_xxgpp_h_5                       5    // GPP_H xxgpp_h_5
#define GPIOV2_PTL_DRIVER_xxgpp_h_6                       6    // GPP_H xxgpp_h_6
#define GPIOV2_PTL_DRIVER_xxgpp_h_7                       7    // GPP_H xxgpp_h_7
#define GPIOV2_PTL_DRIVER_xxgpp_h_8                       8    // GPP_H xxgpp_h_8
#define GPIOV2_PTL_DRIVER_xxgpp_h_9                       9    // GPP_H xxgpp_h_9
#define GPIOV2_PTL_DRIVER_xxgpp_h_10                      10   // GPP_H xxgpp_h_10
#define GPIOV2_PTL_DRIVER_xxgpp_h_11                      11   // GPP_H xxgpp_h_11
#define GPIOV2_PTL_DRIVER_xxgpp_h_12                      12   // GPP_H xxgpp_h_12
#define GPIOV2_PTL_DRIVER_xxgpp_h_13                      13   // GPP_H xxgpp_h_13
#define GPIOV2_PTL_DRIVER_xxgpp_h_14                      14   // GPP_H xxgpp_h_14
#define GPIOV2_PTL_DRIVER_xxgpp_h_15                      15   // GPP_H xxgpp_h_15
#define GPIOV2_PTL_DRIVER_xxgpp_h_16                      16   // GPP_H xxgpp_h_16
#define GPIOV2_PTL_DRIVER_xxgpp_h_17                      17   // GPP_H xxgpp_h_17
#define GPIOV2_PTL_DRIVER_xxgpp_h_18                      18   // GPP_H xxgpp_h_18
#define GPIOV2_PTL_DRIVER_xxgpp_h_19                      19   // GPP_H xxgpp_h_19
#define GPIOV2_PTL_DRIVER_xxgpp_h_20                      20   // GPP_H xxgpp_h_20
#define GPIOV2_PTL_DRIVER_xxgpp_h_21                      21   // GPP_H xxgpp_h_21
#define GPIOV2_PTL_DRIVER_xxgpp_h_22                      22   // GPP_H xxgpp_h_22
#define GPIOV2_PTL_DRIVER_xxgpp_h_23                      23   // GPP_H xxgpp_h_23
#define GPIOV2_PTL_DRIVER_xxgpp_h_24                      24   // GPP_H xxgpp_h_24
#define GPIOV2_PTL_DRIVER_lpi3c1_clk_loopbk               25   // GPP_H lpi3c1_clk_loopbk
#define GPIOV2_PTL_DRIVER_lpi3c0_clk_loopbk               26   // GPP_H lpi3c0_clk_loopbk
#define GPIOV2_PTL_DRIVER_ishi3c1_clk_loopbk              27   // GPP_H ishi3c1_clk_loopbk

//
// GPPASPI0
//
#define GPIOV2_PTL_DRIVER_xxgpp_a_0                       0    // GPPASPI0 xxgpp_a_0
#define GPIOV2_PTL_DRIVER_xxgpp_a_1                       1    // GPPASPI0 xxgpp_a_1
#define GPIOV2_PTL_DRIVER_xxgpp_a_2                       2    // GPPASPI0 xxgpp_a_2
#define GPIOV2_PTL_DRIVER_xxgpp_a_3                       3    // GPPASPI0 xxgpp_a_3
#define GPIOV2_PTL_DRIVER_xxgpp_a_4                       4    // GPPASPI0 xxgpp_a_4
#define GPIOV2_PTL_DRIVER_xxgpp_a_5                       5    // GPPASPI0 xxgpp_a_5
#define GPIOV2_PTL_DRIVER_xxgpp_a_6                       6    // GPPASPI0 xxgpp_a_6
#define GPIOV2_PTL_DRIVER_xxgpp_a_7                       7    // GPPASPI0 xxgpp_a_7
#define GPIOV2_PTL_DRIVER_xxgpp_a_8                       8    // GPPASPI0 xxgpp_a_8
#define GPIOV2_PTL_DRIVER_xxgpp_a_9                       9    // GPPASPI0 xxgpp_a_9
#define GPIOV2_PTL_DRIVER_xxgpp_a_10                      10   // GPPASPI0 xxgpp_a_10
#define GPIOV2_PTL_DRIVER_xxgpp_a_11                      11   // GPPASPI0 xxgpp_a_11
#define GPIOV2_PTL_DRIVER_xxgpp_a_12                      12   // GPPASPI0 xxgpp_a_12
#define GPIOV2_PTL_DRIVER_xxgpp_a_13                      13   // GPPASPI0 xxgpp_a_13
#define GPIOV2_PTL_DRIVER_xxgpp_a_14                      14   // GPPASPI0 xxgpp_a_14
#define GPIOV2_PTL_DRIVER_xxgpp_a_15                      15   // GPPASPI0 xxgpp_a_15
#define GPIOV2_PTL_DRIVER_xxgpp_a_16                      16   // GPPASPI0 xxgpp_a_16
#define GPIOV2_PTL_DRIVER_xxgpp_a_17                      17   // GPPASPI0 xxgpp_a_17
#define GPIOV2_PTL_DRIVER_xxspi0_io_2                     18   // GPPASPI0 xxspi0_io_2
#define GPIOV2_PTL_DRIVER_xxspi0_io_3                     19   // GPPASPI0 xxspi0_io_3
#define GPIOV2_PTL_DRIVER_xxspi0_mosi_io_0                20   // GPPASPI0 xxspi0_mosi_io_0
#define GPIOV2_PTL_DRIVER_xxspi0_miso_io_1                21   // GPPASPI0 xxspi0_miso_io_1
#define GPIOV2_PTL_DRIVER_xxspi0_tpm_cs_b                 22   // GPPASPI0 xxspi0_tpm_cs_b
#define GPIOV2_PTL_DRIVER_xxspi0_flash_0_cs_b             23   // GPPASPI0 xxspi0_flash_0_cs_b
#define GPIOV2_PTL_DRIVER_xxspi0_flash_1_cs_b             24   // GPPASPI0 xxspi0_flash_1_cs_b
#define GPIOV2_PTL_DRIVER_xxspi0_clk                      25   // GPPASPI0 xxspi0_clk
#define GPIOV2_PTL_DRIVER_espi_clk_loopbk                 26   // GPPASPI0 espi_clk_loopbk
#define GPIOV2_PTL_DRIVER_spi0_clk_loopbk                 27   // GPPASPI0 spi0_clk_loopbk

//
// vGPIO_3
//
#define GPIOV2_PTL_DRIVER_vGPIO_USB_0                     0    // vGPIO_3 vGPIO_USB_0
#define GPIOV2_PTL_DRIVER_vGPIO_USB_1                     1    // vGPIO_3 vGPIO_USB_1
#define GPIOV2_PTL_DRIVER_vGPIO_USB_2                     2    // vGPIO_3 vGPIO_USB_2
#define GPIOV2_PTL_DRIVER_vGPIO_USB_3                     3    // vGPIO_3 vGPIO_USB_3
#define GPIOV2_PTL_DRIVER_vGPIO_USB_4                     4    // vGPIO_3 vGPIO_USB_4
#define GPIOV2_PTL_DRIVER_vGPIO_USB_5                     5    // vGPIO_3 vGPIO_USB_5
#define GPIOV2_PTL_DRIVER_vGPIO_USB_6                     6    // vGPIO_3 vGPIO_USB_6
#define GPIOV2_PTL_DRIVER_vGPIO_USB_7                     7    // vGPIO_3 vGPIO_USB_7
#define GPIOV2_PTL_DRIVER_vGPIO_TS0                       8    // vGPIO_3 vGPIO_TS0
#define GPIOV2_PTL_DRIVER_vGPIO_TS1                       9    // vGPIO_3 vGPIO_TS1
#define GPIOV2_PTL_DRIVER_vGPIO_THC0                      10   // vGPIO_3 vGPIO_THC0
#define GPIOV2_PTL_DRIVER_vGPIO_THC1                      11   // vGPIO_3 vGPIO_THC1
#define GPIOV2_PTL_DRIVER_vGPIO_THC2                      12   // vGPIO_3 vGPIO_THC2
#define GPIOV2_PTL_DRIVER_vGPIO_THC3                      13   // vGPIO_3 vGPIO_THC3

//
// GPP_S
//
#define GPIOV2_PTL_DRIVER_xxgpp_s_0                       0    // GPP_S xxgpp_s_0
#define GPIOV2_PTL_DRIVER_xxgpp_s_1                       1    // GPP_S xxgpp_s_1
#define GPIOV2_PTL_DRIVER_xxgpp_s_2                       2    // GPP_S xxgpp_s_2
#define GPIOV2_PTL_DRIVER_xxgpp_s_3                       3    // GPP_S xxgpp_s_3
#define GPIOV2_PTL_DRIVER_xxgpp_s_4                       4    // GPP_S xxgpp_s_4
#define GPIOV2_PTL_DRIVER_xxgpp_s_5                       5    // GPP_S xxgpp_s_5
#define GPIOV2_PTL_DRIVER_xxgpp_s_6                       6    // GPP_S xxgpp_s_6
#define GPIOV2_PTL_DRIVER_xxgpp_s_7                       7    // GPP_S xxgpp_s_7

//
// GPP_B
//
#define GPIOV2_PTL_DRIVER_xxgpp_b_0                       0    // GPP_B xxgpp_b_0
#define GPIOV2_PTL_DRIVER_xxgpp_b_1                       1    // GPP_B xxgpp_b_1
#define GPIOV2_PTL_DRIVER_xxgpp_b_2                       2    // GPP_B xxgpp_b_2
#define GPIOV2_PTL_DRIVER_xxgpp_b_3                       3    // GPP_B xxgpp_b_3
#define GPIOV2_PTL_DRIVER_xxgpp_b_4                       4    // GPP_B xxgpp_b_4
#define GPIOV2_PTL_DRIVER_xxgpp_b_5                       5    // GPP_B xxgpp_b_5
#define GPIOV2_PTL_DRIVER_xxgpp_b_6                       6    // GPP_B xxgpp_b_6
#define GPIOV2_PTL_DRIVER_xxgpp_b_7                       7    // GPP_B xxgpp_b_7
#define GPIOV2_PTL_DRIVER_xxgpp_b_8                       8    // GPP_B xxgpp_b_8
#define GPIOV2_PTL_DRIVER_xxgpp_b_9                       9    // GPP_B xxgpp_b_9
#define GPIOV2_PTL_DRIVER_xxgpp_b_10                      10   // GPP_B xxgpp_b_10
#define GPIOV2_PTL_DRIVER_xxgpp_b_11                      11   // GPP_B xxgpp_b_11
#define GPIOV2_PTL_DRIVER_xxgpp_b_12                      12   // GPP_B xxgpp_b_12
#define GPIOV2_PTL_DRIVER_xxgpp_b_13                      13   // GPP_B xxgpp_b_13
#define GPIOV2_PTL_DRIVER_xxgpp_b_14                      14   // GPP_B xxgpp_b_14
#define GPIOV2_PTL_DRIVER_xxgpp_b_15                      15   // GPP_B xxgpp_b_15
#define GPIOV2_PTL_DRIVER_xxgpp_b_16                      16   // GPP_B xxgpp_b_16
#define GPIOV2_PTL_DRIVER_xxgpp_b_17                      17   // GPP_B xxgpp_b_17
#define GPIOV2_PTL_DRIVER_xxgpp_b_18                      18   // GPP_B xxgpp_b_18
#define GPIOV2_PTL_DRIVER_xxgpp_b_19                      19   // GPP_B xxgpp_b_19
#define GPIOV2_PTL_DRIVER_xxgpp_b_20                      20   // GPP_B xxgpp_b_20
#define GPIOV2_PTL_DRIVER_xxgpp_b_21                      21   // GPP_B xxgpp_b_21
#define GPIOV2_PTL_DRIVER_xxgpp_b_22                      22   // GPP_B xxgpp_b_22
#define GPIOV2_PTL_DRIVER_xxgpp_b_23                      23   // GPP_B xxgpp_b_23
#define GPIOV2_PTL_DRIVER_xxgpp_b_24                      24   // GPP_B xxgpp_b_24
#define GPIOV2_PTL_DRIVER_xxgpp_b_25                      25   // GPP_B xxgpp_b_25
#define GPIOV2_PTL_DRIVER_ishi3c0_clk_loopbk              26   // GPP_B ishi3c0_clk_loopbk

//
// GPP_D
//
#define GPIOV2_PTL_DRIVER_xxgpp_d_0                       0    // GPP_D xxgpp_d_0
#define GPIOV2_PTL_DRIVER_xxgpp_d_1                       1    // GPP_D xxgpp_d_1
#define GPIOV2_PTL_DRIVER_xxgpp_d_2                       2    // GPP_D xxgpp_d_2
#define GPIOV2_PTL_DRIVER_xxgpp_d_3                       3    // GPP_D xxgpp_d_3
#define GPIOV2_PTL_DRIVER_xxgpp_d_4                       4    // GPP_D xxgpp_d_4
#define GPIOV2_PTL_DRIVER_xxgpp_d_5                       5    // GPP_D xxgpp_d_5
#define GPIOV2_PTL_DRIVER_xxgpp_d_6                       6    // GPP_D xxgpp_d_6
#define GPIOV2_PTL_DRIVER_xxgpp_d_7                       7    // GPP_D xxgpp_d_7
#define GPIOV2_PTL_DRIVER_xxgpp_d_8                       8    // GPP_D xxgpp_d_8
#define GPIOV2_PTL_DRIVER_xxgpp_d_9                       9    // GPP_D xxgpp_d_9
#define GPIOV2_PTL_DRIVER_xxgpp_d_10                      10   // GPP_D xxgpp_d_10
#define GPIOV2_PTL_DRIVER_xxgpp_d_11                      11   // GPP_D xxgpp_d_11
#define GPIOV2_PTL_DRIVER_xxgpp_d_12                      12   // GPP_D xxgpp_d_12
#define GPIOV2_PTL_DRIVER_xxgpp_d_13                      13   // GPP_D xxgpp_d_13
#define GPIOV2_PTL_DRIVER_xxgpp_d_14                      14   // GPP_D xxgpp_d_14
#define GPIOV2_PTL_DRIVER_xxgpp_d_15                      15   // GPP_D xxgpp_d_15
#define GPIOV2_PTL_DRIVER_xxgpp_d_16                      16   // GPP_D xxgpp_d_16
#define GPIOV2_PTL_DRIVER_xxgpp_d_17                      17   // GPP_D xxgpp_d_17
#define GPIOV2_PTL_DRIVER_xxgpp_d_18                      18   // GPP_D xxgpp_d_18
#define GPIOV2_PTL_DRIVER_xxgpp_d_19                      19   // GPP_D xxgpp_d_19
#define GPIOV2_PTL_DRIVER_xxgpp_d_20                      20   // GPP_D xxgpp_d_20
#define GPIOV2_PTL_DRIVER_xxgpp_d_21                      21   // GPP_D xxgpp_d_21
#define GPIOV2_PTL_DRIVER_xxgpp_d_22                      22   // GPP_D xxgpp_d_22
#define GPIOV2_PTL_DRIVER_xxgpp_d_23                      23   // GPP_D xxgpp_d_23
#define GPIOV2_PTL_DRIVER_xxgpp_d_24                      24   // GPP_D xxgpp_d_24
#define GPIOV2_PTL_DRIVER_xxgpp_d_25                      25   // GPP_D xxgpp_d_25
#define GPIOV2_PTL_DRIVER_shd_i3c_clk_loopbk              26   // GPP_D shd_i3c_clk_loopbk

//
// vGPIO
//
#define GPIOV2_PTL_DRIVER_vGPIO_0                         0    // vGPIO vGPIO_0
#define GPIOV2_PTL_DRIVER_vGPIO_5                         1    // vGPIO vGPIO_5
#define GPIOV2_PTL_DRIVER_vGPIO_30                        2    // vGPIO vGPIO_30
#define GPIOV2_PTL_DRIVER_vGPIO_31                        3    // vGPIO vGPIO_31
#define GPIOV2_PTL_DRIVER_vGPIO_32                        4    // vGPIO vGPIO_32
#define GPIOV2_PTL_DRIVER_vGPIO_33                        5    // vGPIO vGPIO_33
#define GPIOV2_PTL_DRIVER_vGPIO_34                        6    // vGPIO vGPIO_34
#define GPIOV2_PTL_DRIVER_vGPIO_35                        7    // vGPIO vGPIO_35
#define GPIOV2_PTL_DRIVER_vGPIO_36                        8    // vGPIO vGPIO_36
#define GPIOV2_PTL_DRIVER_vGPIO_37                        9    // vGPIO vGPIO_37
#define GPIOV2_PTL_DRIVER_vGPIO_40                        10   // vGPIO vGPIO_40
#define GPIOV2_PTL_DRIVER_vGPIO_41                        11   // vGPIO vGPIO_41
#define GPIOV2_PTL_DRIVER_vGPIO_42                        12   // vGPIO vGPIO_42
#define GPIOV2_PTL_DRIVER_vGPIO_43                        13   // vGPIO vGPIO_43
#define GPIOV2_PTL_DRIVER_vGPIO_44                        14   // vGPIO vGPIO_44
#define GPIOV2_PTL_DRIVER_vGPIO_45                        15   // vGPIO vGPIO_45
#define GPIOV2_PTL_DRIVER_vGPIO_46                        16   // vGPIO vGPIO_46
#define GPIOV2_PTL_DRIVER_vGPIO_47                        17   // vGPIO vGPIO_47

#endif // _GPIOV2_ACPI_DEFINES_PTL_PCD_H_
