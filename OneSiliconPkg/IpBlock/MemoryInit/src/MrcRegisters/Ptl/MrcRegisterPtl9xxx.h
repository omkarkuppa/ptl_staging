#ifndef __MrcRegisterPtl9xxx_h__
#define __MrcRegisterPtl9xxx_h__

/**
@file
  This file was automatically generated. Modify at your own risk.
  Note that no error checking is done in these functions so ensure that the correct values are passed.

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

#pragma pack(push, 1)


#define CH1DDR5_CR_DDRCRCLKCOMP_REG                                    (0x00009000)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5_CR_DDRCRCTLCOMP_REG                                    (0x00009004)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5_CR_DDRCRCACOMP_REG                                     (0x00009008)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5_CR_DDRCRCLKCTLCACOMPOFFSET_REG                         (0x0000900C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH1DDR5_CR_DDRCRCCCLANE0_REG                                   (0x00009010)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE1_REG                                   (0x00009014)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE2_REG                                   (0x00009018)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE3_REG                                   (0x0000901C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE4_REG                                   (0x00009020)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE5_REG                                   (0x00009024)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH1DDR5_CR_DDRCRCCCLANE6_REG                                   (0x00009028)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE7_REG                                   (0x0000902C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE8_REG                                   (0x00009030)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCLANE9_REG                                   (0x00009034)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5_CR_DDRCRCCCPICODES_REG                                 (0x00009038)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH1DDR5_CR_DDRCRPERBITDESKEWRISEFALL_REG                       (0x0000903C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH1DDR5_CR_DCCFSMCONTROL_REG                                   (0x00009040)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH1DDR5_CR_DCCCALCCONTROL_REG                                  (0x00009044)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH1DDR5_CR_REFPICONTROL_REG                                    (0x00009048)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH1DDR5_CR_DDRCRBSCANDATA_REG                                  (0x00009050)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH1DDR5_CR_REFPIFSMCONTROL_REG                                 (0x00009054)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH1DDR5_CR_DCCFSMSTATUS_REG                                    (0x00009058)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH1DDR5_CR_DCCLANESTATUS0_REG                                  (0x0000905C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH1DDR5_CR_DCCLANESTATUS1_REG                                  (0x00009060)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH1DDR5_CR_DDRCRMARGINMODECONTROL_REG                          (0x00009064)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH1DDR5_CR_DDRCRMARGINMODEDEBUGMSB_REG                         (0x00009068)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH1DDR5_CR_DDRCRMARGINMODEDEBUGLSB_REG                         (0x0000906C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH1DDR5_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                      (0x00009070)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH1DDR5_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                 (0x00009074)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH1DDR5_CR_DDRCRADC_REG                                        (0x00009078)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH1DDR5_CR_DFXCCMON_REG                                        (0x0000907C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH1DDR5_CR_DDRCRMARGINMODECONTROL1_REG                         (0x00009080)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH1DDR5_CR_CBB_BONUS0_REG                                      (0x00009084)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH1DDR5_CR_BONUS0_REG                                          (0x00009088)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH1DDR5_CR_REFPICLKOFFSET_REG                                  (0x0000908C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH1DDR5_CR_LPMODE4_FLYBY_REG                                   (0x00009090)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH1DDR5_CR_CCC_OVRDCTL_REG                                     (0x00009094)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH1DDR5_DDRTXDLL_CR_PICODELUT0_REG                             (0x00009098)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH1DDR5_DDRTXDLL_CR_PICODELUT1_REG                             (0x0000909C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH1DDR5_DDRTXDLL_CR_PICODELUT2_REG                             (0x000090A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH1DDR5_DDRTXDLL_CR_PICODELUT3_REG                             (0x000090A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH1DDR5_CR_TERDCCCONTROL0_REG                                  (0x000090A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH1DDR5_CR_CS_GEARDOWN_REG                                     (0x000090AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH1DDR5_CR_DDRCRCCCDCCCLKLANE5_REG                             (0x000090B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH1DDR5_CR_COMPUPDATEOVERRIDE_REG                              (0x000090B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH1DDR5_CR_SPARE_REG                                           (0x000090B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CH2DDR5_CR_DDRCRCLKCOMP_REG                                    (0x00009100)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5_CR_DDRCRCTLCOMP_REG                                    (0x00009104)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5_CR_DDRCRCACOMP_REG                                     (0x00009108)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5_CR_DDRCRCLKCTLCACOMPOFFSET_REG                         (0x0000910C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH2DDR5_CR_DDRCRCCCLANE0_REG                                   (0x00009110)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE1_REG                                   (0x00009114)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE2_REG                                   (0x00009118)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE3_REG                                   (0x0000911C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE4_REG                                   (0x00009120)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE5_REG                                   (0x00009124)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH2DDR5_CR_DDRCRCCCLANE6_REG                                   (0x00009128)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE7_REG                                   (0x0000912C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE8_REG                                   (0x00009130)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCLANE9_REG                                   (0x00009134)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5_CR_DDRCRCCCPICODES_REG                                 (0x00009138)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH2DDR5_CR_DDRCRPERBITDESKEWRISEFALL_REG                       (0x0000913C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH2DDR5_CR_DCCFSMCONTROL_REG                                   (0x00009140)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH2DDR5_CR_DCCCALCCONTROL_REG                                  (0x00009144)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH2DDR5_CR_REFPICONTROL_REG                                    (0x00009148)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH2DDR5_CR_DDRCRBSCANDATA_REG                                  (0x00009150)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH2DDR5_CR_REFPIFSMCONTROL_REG                                 (0x00009154)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH2DDR5_CR_DCCFSMSTATUS_REG                                    (0x00009158)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH2DDR5_CR_DCCLANESTATUS0_REG                                  (0x0000915C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH2DDR5_CR_DCCLANESTATUS1_REG                                  (0x00009160)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH2DDR5_CR_DDRCRMARGINMODECONTROL_REG                          (0x00009164)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH2DDR5_CR_DDRCRMARGINMODEDEBUGMSB_REG                         (0x00009168)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH2DDR5_CR_DDRCRMARGINMODEDEBUGLSB_REG                         (0x0000916C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH2DDR5_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                      (0x00009170)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH2DDR5_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                 (0x00009174)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH2DDR5_CR_DDRCRADC_REG                                        (0x00009178)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH2DDR5_CR_DFXCCMON_REG                                        (0x0000917C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH2DDR5_CR_DDRCRMARGINMODECONTROL1_REG                         (0x00009180)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH2DDR5_CR_CBB_BONUS0_REG                                      (0x00009184)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH2DDR5_CR_BONUS0_REG                                          (0x00009188)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH2DDR5_CR_REFPICLKOFFSET_REG                                  (0x0000918C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH2DDR5_CR_LPMODE4_FLYBY_REG                                   (0x00009190)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH2DDR5_CR_CCC_OVRDCTL_REG                                     (0x00009194)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH2DDR5_DDRTXDLL_CR_PICODELUT0_REG                             (0x00009198)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH2DDR5_DDRTXDLL_CR_PICODELUT1_REG                             (0x0000919C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH2DDR5_DDRTXDLL_CR_PICODELUT2_REG                             (0x000091A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH2DDR5_DDRTXDLL_CR_PICODELUT3_REG                             (0x000091A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH2DDR5_CR_TERDCCCONTROL0_REG                                  (0x000091A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH2DDR5_CR_CS_GEARDOWN_REG                                     (0x000091AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH2DDR5_CR_DDRCRCCCDCCCLKLANE5_REG                             (0x000091B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH2DDR5_CR_COMPUPDATEOVERRIDE_REG                              (0x000091B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH2DDR5_CR_SPARE_REG                                           (0x000091B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CH3DDR5_CR_DDRCRCLKCOMP_REG                                    (0x00009200)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5_CR_DDRCRCTLCOMP_REG                                    (0x00009204)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5_CR_DDRCRCACOMP_REG                                     (0x00009208)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5_CR_DDRCRCLKCTLCACOMPOFFSET_REG                         (0x0000920C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH3DDR5_CR_DDRCRCCCLANE0_REG                                   (0x00009210)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE1_REG                                   (0x00009214)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE2_REG                                   (0x00009218)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE3_REG                                   (0x0000921C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE4_REG                                   (0x00009220)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE5_REG                                   (0x00009224)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH3DDR5_CR_DDRCRCCCLANE6_REG                                   (0x00009228)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE7_REG                                   (0x0000922C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE8_REG                                   (0x00009230)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCLANE9_REG                                   (0x00009234)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5_CR_DDRCRCCCPICODES_REG                                 (0x00009238)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH3DDR5_CR_DDRCRPERBITDESKEWRISEFALL_REG                       (0x0000923C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH3DDR5_CR_DCCFSMCONTROL_REG                                   (0x00009240)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH3DDR5_CR_DCCCALCCONTROL_REG                                  (0x00009244)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH3DDR5_CR_REFPICONTROL_REG                                    (0x00009248)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH3DDR5_CR_DDRCRBSCANDATA_REG                                  (0x00009250)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH3DDR5_CR_REFPIFSMCONTROL_REG                                 (0x00009254)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH3DDR5_CR_DCCFSMSTATUS_REG                                    (0x00009258)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH3DDR5_CR_DCCLANESTATUS0_REG                                  (0x0000925C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH3DDR5_CR_DCCLANESTATUS1_REG                                  (0x00009260)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH3DDR5_CR_DDRCRMARGINMODECONTROL_REG                          (0x00009264)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH3DDR5_CR_DDRCRMARGINMODEDEBUGMSB_REG                         (0x00009268)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH3DDR5_CR_DDRCRMARGINMODEDEBUGLSB_REG                         (0x0000926C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH3DDR5_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                      (0x00009270)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH3DDR5_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                 (0x00009274)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH3DDR5_CR_DDRCRADC_REG                                        (0x00009278)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH3DDR5_CR_DFXCCMON_REG                                        (0x0000927C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH3DDR5_CR_DDRCRMARGINMODECONTROL1_REG                         (0x00009280)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH3DDR5_CR_CBB_BONUS0_REG                                      (0x00009284)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH3DDR5_CR_BONUS0_REG                                          (0x00009288)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH3DDR5_CR_REFPICLKOFFSET_REG                                  (0x0000928C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH3DDR5_CR_LPMODE4_FLYBY_REG                                   (0x00009290)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH3DDR5_CR_CCC_OVRDCTL_REG                                     (0x00009294)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH3DDR5_DDRTXDLL_CR_PICODELUT0_REG                             (0x00009298)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH3DDR5_DDRTXDLL_CR_PICODELUT1_REG                             (0x0000929C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH3DDR5_DDRTXDLL_CR_PICODELUT2_REG                             (0x000092A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH3DDR5_DDRTXDLL_CR_PICODELUT3_REG                             (0x000092A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH3DDR5_CR_TERDCCCONTROL0_REG                                  (0x000092A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH3DDR5_CR_CS_GEARDOWN_REG                                     (0x000092AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH3DDR5_CR_DDRCRCCCDCCCLKLANE5_REG                             (0x000092B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH3DDR5_CR_COMPUPDATEOVERRIDE_REG                              (0x000092B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH3DDR5_CR_SPARE_REG                                           (0x000092B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CCC_CR_DDRCRCLKCOMP_REG                                        (0x00009300)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CCC_CR_DDRCRCTLCOMP_REG                                        (0x00009304)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CCC_CR_DDRCRCACOMP_REG                                         (0x00009308)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG                             (0x0000930C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CCC_CR_DDRCRCCCLANE0_REG                                       (0x00009310)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE1_REG                                       (0x00009314)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE2_REG                                       (0x00009318)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE3_REG                                       (0x0000931C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE4_REG                                       (0x00009320)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE5_REG                                       (0x00009324)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CCC_CR_DDRCRCCCLANE6_REG                                       (0x00009328)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE7_REG                                       (0x0000932C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE8_REG                                       (0x00009330)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCLANE9_REG                                       (0x00009334)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CCC_CR_DDRCRCCCPICODES_REG                                     (0x00009338)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG                           (0x0000933C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CCC_CR_DCCFSMCONTROL_REG                                       (0x00009340)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CCC_CR_DCCCALCCONTROL_REG                                      (0x00009344)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CCC_CR_REFPICONTROL_REG                                        (0x00009348)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CCC_CR_DDRCRBSCANDATA_REG                                      (0x00009350)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CCC_CR_REFPIFSMCONTROL_REG                                     (0x00009354)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CCC_CR_DCCFSMSTATUS_REG                                        (0x00009358)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CCC_CR_DCCLANESTATUS0_REG                                      (0x0000935C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CCC_CR_DCCLANESTATUS1_REG                                      (0x00009360)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CCC_CR_DDRCRMARGINMODECONTROL_REG                              (0x00009364)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CCC_CR_DDRCRMARGINMODEDEBUGMSB_REG                             (0x00009368)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CCC_CR_DDRCRMARGINMODEDEBUGLSB_REG                             (0x0000936C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CCC_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                          (0x00009370)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CCC_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG                     (0x00009374)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CCC_CR_DDRCRADC_REG                                            (0x00009378)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CCC_CR_DFXCCMON_REG                                            (0x0000937C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CCC_CR_DDRCRMARGINMODECONTROL1_REG                             (0x00009380)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CCC_CR_CBB_BONUS0_REG                                          (0x00009384)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CCC_CR_BONUS0_REG                                              (0x00009388)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CCC_CR_REFPICLKOFFSET_REG                                      (0x0000938C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CCC_CR_LPMODE4_FLYBY_REG                                       (0x00009390)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CCC_CR_CCC_OVRDCTL_REG                                         (0x00009394)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CCC_DDRTXDLL_CR_PICODELUT0_REG                                 (0x00009398)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CCC_DDRTXDLL_CR_PICODELUT1_REG                                 (0x0000939C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CCC_DDRTXDLL_CR_PICODELUT2_REG                                 (0x000093A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CCC_DDRTXDLL_CR_PICODELUT3_REG                                 (0x000093A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CCC_CR_TERDCCCONTROL0_REG                                      (0x000093A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CCC_CR_CS_GEARDOWN_REG                                         (0x000093AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CCC_CR_DDRCRCCCDCCCLKLANE5_REG                                 (0x000093B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CCC_CR_COMPUPDATEOVERRIDE_REG                                  (0x000093B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CCC_CR_SPARE_REG                                               (0x000093B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CCCSHARED_CR_DDRCRVCCCLKCOMPOFFSET_REG                         (0x00009580)
//Duplicate of DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_REG

#define CCCSHARED_CR_DDRCRBWSELCOMPDATACCC_REG                         (0x00009584)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_REG

#define CCCSHARED_CR_DDRCRVCCCLKCOMPDATACCC_REG                        (0x00009588)
//Duplicate of DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_REG

#define CCCSHARED_CR_LVRCURRENTSENSOR_REG                              (0x0000958C)
//Duplicate of DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_REG

#define CCCSHARED_CR_LVRCURRENTSENSORCOUNT_REG                         (0x00009590)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CCCSHARED_CR_DDRCRDLLOFFSETCONTROL_REG                         (0x00009594)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_REG

#define CCCSHARED_CR_DDRCRVOLTAGEUSEDCLKCONTROL_REG                    (0x0000959C)
//Duplicate of DDRCCC_SHARED0_CR_DDRCRVOLTAGEUSEDCLKCONTROL_REG

#define CCCSHARED_CR_DDRCRCCCPINCONTROLS_REG                           (0x000095A0)
//Duplicate of DDRCCC_SHARED0_CR_DDRCRCCCPINCONTROLS_REG

#define CCCSHARED_CR_DCCCONTROL_REG                                    (0x000095A4)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_REG

#define CCCSHARED_CR_PHDCCLANESTATUS0_REG                              (0x000095A8)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_REG

#define CCCSHARED_CR_PHDCCLANESTATUS1_REG                              (0x000095AC)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_REG

#define CCCSHARED_CR_PHCLOCKREPEATER0_REG                              (0x000095B0)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_REG

#define CCCSHARED_CR_PHCLOCKREPEATER1_REG                              (0x000095B4)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_REG

#define CCCSHARED_CR_DDRCRTXDLLCONTROL1_REG                            (0x000095B8)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG

#define CCCSHARED_CR_SPARE_REG                                         (0x000095BC)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CCCSHARED_CR_DDRCRTXDLLCONTROL2_REG                            (0x000095C0)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_REG

#define CCCSHARED_CR_DDRCRTXDLLCONTROL0_REG                            (0x000095C4)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_REG

#define CCCSHARED_CR_DDRCRTXDLLSTATUS_REG                              (0x000095C8)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define CCCSHARED_CR_DDR_PMBONUS0_REG                                  (0x000095CC)
//Duplicate of DDRCCC_SHARED0_CR_DDR_PMBONUS0_REG

#define CCCSHARED_CR_DDRCRCCCDLLWAKEUPCONTROLS_REG                     (0x000095D0)
//Duplicate of DDRCCC_SHARED0_CR_DDRCRCCCDLLWAKEUPCONTROLS_REG

#define CCCSHARED_CR_DDRCRVCCDLLCOMPCCC_REG                            (0x000095D4)
//Duplicate of DDRPHY_DDRCOMP_CR_VCCDLLCOMPCCC_REG

#define CCCSHARED_CR_DDRCRVCCDLLCOMPCCCOFFSET_REG                      (0x000095D8)
//Duplicate of DDRCCC_SHARED0_CR_DDRCRVCCDLLCOMPCCCOFFSET_REG

#define CCCSHARED_CR_CBB_BONUS1_REG                                    (0x000095DC)
//Duplicate of DDRCCC_SHARED0_CR_CBB_BONUS1_REG

#define CCCSHARED_CR_NTLTEST_REG                                       (0x000095E0)
//Duplicate of DDRDATA_SHARED0_CR_NTLTEST_REG

#define CCCSHARED_CR_NTLRSLT_REG                                       (0x000095E4)
//Duplicate of DDRCCC_SHARED0_CR_NTLRSLT_REG

#define CCCSHARED_CR_DDRCRDLLCBTUNEOVRD_REG                            (0x000095E8)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_REG

#define CCCSHARED_CR_PHCLOCKREPEATER2_REG                              (0x000095EC)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_REG

#define CCCSHARED_CR_PHCLOCKREPEATER3_REG                              (0x000095F0)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_REG

#define CCCSHARED_CR_PHDCCLANESTATUS2_REG                              (0x000095F4)
//Duplicate of DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_REG

#define CCCSHARED_CR_DDRWRONGPHASE_CTRL_REG                            (0x000095F8)
//Duplicate of DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_REG

#define CCCSHARED_CR_DLLTIMERCONTROL_REG                               (0x000095FC)
//Duplicate of DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_REG

#define PGCRSHARED_CR_LVRVOLTAGESENSOR_REG                             (0x00009600)
//Duplicate of DDRPGTERMCR0_CR_LVRVOLTAGESENSOR_REG

#define PGCRSHARED_CR_LVRVOLTAGESENSORCOUNT_REG                        (0x00009604)
//Duplicate of DDRPGTERMCR0_CR_LVRVOLTAGESENSORCOUNT_REG

#define PGCRSHARED_CR_SPARE_REG                                        (0x00009608)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define PGCRSHARED_CR_TLINETERMINATION_REG                             (0x00009628)
//Duplicate of DDRPGTERMCR0_CR_TLINETERMINATION_REG

#define DATASHARED_CR_DCCCONTROL_REG                                   (0x00009680)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_DCCCONTROL_REG

#define DATASHARED_CR_DDRCRDLLCONTROL1_REG                             (0x00009684)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG

#define DATASHARED_CR_DDRCRBSCANDATA_REG                               (0x00009688)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRBSCANDATA_REG

#define DATASHARED_CR_PHCLOCKREPEATER0_REG                             (0x0000968C)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER0_REG

#define DATASHARED_CR_PHCLOCKREPEATER1_REG                             (0x00009690)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHCLOCKREPEATER1_REG

#define DATASHARED_CR_VCCCLKCOMPDATACCC_REG                            (0x00009694)
//Duplicate of DDRDATA_SHARED0_CR_VCCCLKCOMPDATACCC_REG

#define DATASHARED_CR_VCCCLKCOMPDATA_REG                               (0x00009698)
//Duplicate of DDRDATA_SHARED0_CR_VCCCLKCOMPDATA_REG

#define DATASHARED_CR_NTLTEST_REG                                      (0x0000969C)
//Duplicate of DDRDATA_SHARED0_CR_NTLTEST_REG

#define DATASHARED_CR_NTLRSLT_REG                                      (0x000096A0)
//Duplicate of DDRDATA_SHARED0_CR_NTLRSLT_REG

#define DATASHARED_CR_DDRCRCOMPDQSDELAYCONTROL_REG                     (0x000096A4)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCOMPDQSDELAYCONTROL_REG

#define DATASHARED_CR_LVRCURRENTSENSOR_REG                             (0x000096A8)
//Duplicate of DDRPHY_DDRCOMP_CR_LVRCURRENTSENSOR_REG

#define DATASHARED_CR_LVRCURRENTSENSORCOUNT_REG                        (0x000096AC)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define DATASHARED_CR_DDRCRDLLOFFSETCONTROL_REG                        (0x000096B0)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRDLLOFFSETCONTROL_REG

#define DATASHARED_CR_PHCLOCKREPEATER2_REG                             (0x000096B4)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER2_REG

#define DATASHARED_CR_PHCLOCKREPEATER3_REG                             (0x000096B8)
//Duplicate of DDRDATA_SHARED0_CR_PHCLOCKREPEATER3_REG

#define DATASHARED_CR_RCOMPDATA_REG                                    (0x000096BC)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRDATACOMP_REG

#define DATASHARED_CR_DDRCRDATACONTROL6_REG                            (0x000096C0)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRDATACONTROL6_REG

#define DATASHARED_CR_DDRCRWCKCONTROL_REG                              (0x000096C4)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRWCKCONTROL_REG

#define DATASHARED_CR_DDRCRCCCCH0LANE0_REG                             (0x000096C8)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_REG

#define DATASHARED_CR_DDRCRCCCCH1LANE0_REG                             (0x000096CC)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCCCCH0LANE0_REG

#define DATASHARED_CR_DDRCRCCCPICODES_REG                              (0x000096D0)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCCCPICODES_REG

#define DATASHARED_CR_DDRCRWCKPERBITDESKEWRISEFALL_REG                 (0x000096D4)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRWCKPERBITDESKEWRISEFALL_REG

#define DATASHARED_CR_WCKDCCFSMCONTROL_REG                             (0x000096D8)
//Duplicate of DDRDATA_SHARED0_CR_WCKDCCFSMCONTROL_REG

#define DATASHARED_CR_WCKDCCCALCCONTROL_REG                            (0x000096DC)
//Duplicate of DDRDATA_SHARED0_CR_WCKDCCCALCCONTROL_REG

#define DATASHARED_CR_WCKDCCFSMSTATUS_REG                              (0x000096E0)
//Duplicate of DDRDATA_SHARED0_CR_WCKDCCFSMSTATUS_REG

#define DATASHARED_CR_WCKDCCLANESTATUS0_REG                            (0x000096E4)
//Duplicate of DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_REG

#define DATASHARED_CR_WCKDCCLANESTATUS1_REG                            (0x000096E8)
//Duplicate of DDRDATA_SHARED0_CR_WCKDCCLANESTATUS0_REG

#define DATASHARED_CR_DDRCRCLKCOMP_REG                                 (0x000096EC)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCLKCOMP_REG

#define DATASHARED_CR_DDRCRCLKCOMPOFFSET_REG                           (0x000096F0)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCLKCOMPOFFSET_REG

#define DATASHARED_CR_DDRCRMARGINMODECONTROL_REG                       (0x000096F4)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define DATASHARED_CR_DDRCRMARGINMODEDEBUGMSB_REG                      (0x000096F8)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATASHARED_CR_DDRCRMARGINMODEDEBUGLSB_REG                      (0x000096FC)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define DATASHARED_CR_PHDCCLANESTATUS0_REG                             (0x00009700)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS0_REG

#define DATASHARED_CR_PHDCCLANESTATUS1_REG                             (0x00009704)
//Duplicate of DDRPHY_DDRCOMP_DDRPLL_CR_PHDCCLANESTATUS1_REG

#define DATASHARED_CR_DDRCRTXDLLCONTROL1_REG                           (0x00009708)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRDLLCONTROL1_REG

#define DATASHARED_CR_DDRCRTXDLLCONTROL2_REG                           (0x0000970C)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL2_REG

#define DATASHARED_CR_DDRCRTXDLLCONTROL0_REG                           (0x00009710)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRTXDLLCONTROL0_REG

#define DATASHARED_CR_DDRCRTXDLLSTATUS_REG                             (0x00009714)
//Duplicate of DATA0CH0_CR_DDRCRDLLSTATUS_REG

#define DATASHARED_CR_DFXCCMONBYTE0_REG                                (0x00009718)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define DATASHARED_CR_DFXCCMONBYTE1_REG                                (0x0000971C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define DATASHARED_CR_DDR_PMBONUS0_REG                                 (0x00009720)
//Duplicate of DDRDATA_SHARED0_CR_DDR_PMBONUS0_REG

#define DATASHARED_CR_DDRCRBWSELCOMPDATACCC_REG                        (0x00009724)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRBWSELCOMPDATACCC_REG

#define DATASHARED_CR_VCCCLKCOMPTXOFFSET_REG                           (0x00009728)
//Duplicate of DDRDATA_SHARED0_CR_VCCCLKCOMPTXOFFSET_REG

#define DATASHARED_CR_DDRCRCLOCKINGBONUSOVRD0_REG                      (0x0000972C)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCLOCKINGBONUSOVRD0_REG

#define DATASHARED_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG             (0x00009730)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG

#define DATASHARED_CR_DDRCRBYTE1DATABUFFERBONUSPERBIT0_REG             (0x00009734)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRBYTE0DATABUFFERBONUSPERBIT0_REG

#define DATASHARED_CR_DDRCRWCKDQSBUFFERBONUS0_REG                      (0x00009738)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRWCKDQSBUFFERBONUS0_REG

#define DATASHARED_CR_DDRCRCBBBONUS0_REG                               (0x0000973C)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRCBBBONUS0_REG

#define DATASHARED_CR_REFPIDQSWCKOFFSET_REG                            (0x00009740)
//Duplicate of DDRDATA_SHARED0_CR_REFPIDQSWCKOFFSET_REG

#define DATASHARED_CR_DDRCRDLLCBTUNEOVRD_REG                           (0x00009744)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRDLLCBTUNEOVRD_REG

#define DATASHARED_CR_DDRCRRXFIFODLYCONTROL0_REG                       (0x00009748)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_REG

#define DATASHARED_CR_DDRCRRXFIFODLYCONTROL1_REG                       (0x0000974C)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL0_REG

#define DATASHARED_CR_DDRCRRXFIFODLYCONTROL2_REG                       (0x00009750)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRRXFIFODLYCONTROL2_REG

#define DATASHARED_DDRTXDLL_CR_PICODELUT0_REG                          (0x00009754)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define DATASHARED_DDRTXDLL_CR_PICODELUT1_REG                          (0x00009758)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define DATASHARED_DDRTXDLL_CR_PICODELUT2_REG                          (0x0000975C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define DATASHARED_DDRTXDLL_CR_PICODELUT3_REG                          (0x00009760)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define DATASHARED_DDRRXDLL_CR_PICODELUT0_REG                          (0x00009764)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define DATASHARED_DDRRXDLL_CR_PICODELUT1_REG                          (0x00009768)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define DATASHARED_DDRRXDLL_CR_PICODELUT2_REG                          (0x0000976C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define DATASHARED_DDRRXDLL_CR_PICODELUT3_REG                          (0x00009770)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define DATASHARED_CR_DLLTIMERCONTROL_REG                              (0x00009774)
//Duplicate of DDRPHY_DDRCOMP_CR_DLLTIMERCONTROL_REG

#define DATASHARED_CR_PHDCCLANESTATUS2_REG                             (0x00009778)
//Duplicate of DDRDATA_SHARED0_CR_PHDCCLANESTATUS2_REG

#define DATASHARED_CR_DDRWRONGPHASE_CTRL_REG                           (0x0000977C)
//Duplicate of DDRDATA_SHARED0_CR_DDRWRONGPHASE_CTRL_REG

#define CH0DDR5IL_CR_DDRCRCLKCOMP_REG                                  (0x00009800)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH0DDR5IL_CR_DDRCRCTLCOMP_REG                                  (0x00009804)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH0DDR5IL_CR_DDRCRCACOMP_REG                                   (0x00009808)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH0DDR5IL_CR_DDRCRCLKCTLCACOMPOFFSET_REG                       (0x0000980C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE0_REG                                 (0x00009810)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE1_REG                                 (0x00009814)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE2_REG                                 (0x00009818)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE3_REG                                 (0x0000981C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE4_REG                                 (0x00009820)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE5_REG                                 (0x00009824)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE6_REG                                 (0x00009828)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE7_REG                                 (0x0000982C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE8_REG                                 (0x00009830)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCLANE9_REG                                 (0x00009834)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH0DDR5IL_CR_DDRCRCCCPICODES_REG                               (0x00009838)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH0DDR5IL_CR_DDRCRPERBITDESKEWRISEFALL_REG                     (0x0000983C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH0DDR5IL_CR_DCCFSMCONTROL_REG                                 (0x00009840)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH0DDR5IL_CR_DCCCALCCONTROL_REG                                (0x00009844)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH0DDR5IL_CR_REFPICONTROL_REG                                  (0x00009848)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH0DDR5IL_CR_DDRCRBSCANDATA_REG                                (0x00009850)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH0DDR5IL_CR_REFPIFSMCONTROL_REG                               (0x00009854)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH0DDR5IL_CR_DCCFSMSTATUS_REG                                  (0x00009858)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH0DDR5IL_CR_DCCLANESTATUS0_REG                                (0x0000985C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH0DDR5IL_CR_DCCLANESTATUS1_REG                                (0x00009860)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH0DDR5IL_CR_DDRCRMARGINMODECONTROL_REG                        (0x00009864)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH0DDR5IL_CR_DDRCRMARGINMODEDEBUGMSB_REG                       (0x00009868)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH0DDR5IL_CR_DDRCRMARGINMODEDEBUGLSB_REG                       (0x0000986C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH0DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                    (0x00009870)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH0DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG               (0x00009874)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH0DDR5IL_CR_DDRCRADC_REG                                      (0x00009878)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH0DDR5IL_CR_DFXCCMON_REG                                      (0x0000987C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH0DDR5IL_CR_DDRCRMARGINMODECONTROL1_REG                       (0x00009880)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH0DDR5IL_CR_CBB_BONUS0_REG                                    (0x00009884)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH0DDR5IL_CR_BONUS0_REG                                        (0x00009888)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH0DDR5IL_CR_REFPICLKOFFSET_REG                                (0x0000988C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH0DDR5IL_CR_LPMODE4_FLYBY_REG                                 (0x00009890)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH0DDR5IL_CR_CCC_OVRDCTL_REG                                   (0x00009894)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH0DDR5IL_DDRTXDLL_CR_PICODELUT0_REG                           (0x00009898)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH0DDR5IL_DDRTXDLL_CR_PICODELUT1_REG                           (0x0000989C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH0DDR5IL_DDRTXDLL_CR_PICODELUT2_REG                           (0x000098A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH0DDR5IL_DDRTXDLL_CR_PICODELUT3_REG                           (0x000098A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH0DDR5IL_CR_TERDCCCONTROL0_REG                                (0x000098A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH0DDR5IL_CR_CS_GEARDOWN_REG                                   (0x000098AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH0DDR5IL_CR_DDRCRCCCDCCCLKLANE5_REG                           (0x000098B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH0DDR5IL_CR_COMPUPDATEOVERRIDE_REG                            (0x000098B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH0DDR5IL_CR_SPARE_REG                                         (0x000098B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CH1DDR5IL_CR_DDRCRCLKCOMP_REG                                  (0x00009900)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5IL_CR_DDRCRCTLCOMP_REG                                  (0x00009904)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5IL_CR_DDRCRCACOMP_REG                                   (0x00009908)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH1DDR5IL_CR_DDRCRCLKCTLCACOMPOFFSET_REG                       (0x0000990C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE0_REG                                 (0x00009910)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE1_REG                                 (0x00009914)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE2_REG                                 (0x00009918)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE3_REG                                 (0x0000991C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE4_REG                                 (0x00009920)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE5_REG                                 (0x00009924)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE6_REG                                 (0x00009928)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE7_REG                                 (0x0000992C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE8_REG                                 (0x00009930)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCLANE9_REG                                 (0x00009934)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH1DDR5IL_CR_DDRCRCCCPICODES_REG                               (0x00009938)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH1DDR5IL_CR_DDRCRPERBITDESKEWRISEFALL_REG                     (0x0000993C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH1DDR5IL_CR_DCCFSMCONTROL_REG                                 (0x00009940)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH1DDR5IL_CR_DCCCALCCONTROL_REG                                (0x00009944)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH1DDR5IL_CR_REFPICONTROL_REG                                  (0x00009948)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH1DDR5IL_CR_DDRCRBSCANDATA_REG                                (0x00009950)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH1DDR5IL_CR_REFPIFSMCONTROL_REG                               (0x00009954)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH1DDR5IL_CR_DCCFSMSTATUS_REG                                  (0x00009958)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH1DDR5IL_CR_DCCLANESTATUS0_REG                                (0x0000995C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH1DDR5IL_CR_DCCLANESTATUS1_REG                                (0x00009960)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH1DDR5IL_CR_DDRCRMARGINMODECONTROL_REG                        (0x00009964)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH1DDR5IL_CR_DDRCRMARGINMODEDEBUGMSB_REG                       (0x00009968)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH1DDR5IL_CR_DDRCRMARGINMODEDEBUGLSB_REG                       (0x0000996C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH1DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                    (0x00009970)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH1DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG               (0x00009974)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH1DDR5IL_CR_DDRCRADC_REG                                      (0x00009978)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH1DDR5IL_CR_DFXCCMON_REG                                      (0x0000997C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH1DDR5IL_CR_DDRCRMARGINMODECONTROL1_REG                       (0x00009980)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH1DDR5IL_CR_CBB_BONUS0_REG                                    (0x00009984)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH1DDR5IL_CR_BONUS0_REG                                        (0x00009988)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH1DDR5IL_CR_REFPICLKOFFSET_REG                                (0x0000998C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH1DDR5IL_CR_LPMODE4_FLYBY_REG                                 (0x00009990)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH1DDR5IL_CR_CCC_OVRDCTL_REG                                   (0x00009994)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH1DDR5IL_DDRTXDLL_CR_PICODELUT0_REG                           (0x00009998)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH1DDR5IL_DDRTXDLL_CR_PICODELUT1_REG                           (0x0000999C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH1DDR5IL_DDRTXDLL_CR_PICODELUT2_REG                           (0x000099A0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH1DDR5IL_DDRTXDLL_CR_PICODELUT3_REG                           (0x000099A4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH1DDR5IL_CR_TERDCCCONTROL0_REG                                (0x000099A8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH1DDR5IL_CR_CS_GEARDOWN_REG                                   (0x000099AC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH1DDR5IL_CR_DDRCRCCCDCCCLKLANE5_REG                           (0x000099B0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH1DDR5IL_CR_COMPUPDATEOVERRIDE_REG                            (0x000099B4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH1DDR5IL_CR_SPARE_REG                                         (0x000099B8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CH2DDR5IL_CR_DDRCRCLKCOMP_REG                                  (0x00009A00)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5IL_CR_DDRCRCTLCOMP_REG                                  (0x00009A04)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5IL_CR_DDRCRCACOMP_REG                                   (0x00009A08)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH2DDR5IL_CR_DDRCRCLKCTLCACOMPOFFSET_REG                       (0x00009A0C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE0_REG                                 (0x00009A10)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE1_REG                                 (0x00009A14)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE2_REG                                 (0x00009A18)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE3_REG                                 (0x00009A1C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE4_REG                                 (0x00009A20)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE5_REG                                 (0x00009A24)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE6_REG                                 (0x00009A28)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE7_REG                                 (0x00009A2C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE8_REG                                 (0x00009A30)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCLANE9_REG                                 (0x00009A34)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH2DDR5IL_CR_DDRCRCCCPICODES_REG                               (0x00009A38)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH2DDR5IL_CR_DDRCRPERBITDESKEWRISEFALL_REG                     (0x00009A3C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH2DDR5IL_CR_DCCFSMCONTROL_REG                                 (0x00009A40)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH2DDR5IL_CR_DCCCALCCONTROL_REG                                (0x00009A44)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH2DDR5IL_CR_REFPICONTROL_REG                                  (0x00009A48)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH2DDR5IL_CR_DDRCRBSCANDATA_REG                                (0x00009A50)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH2DDR5IL_CR_REFPIFSMCONTROL_REG                               (0x00009A54)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH2DDR5IL_CR_DCCFSMSTATUS_REG                                  (0x00009A58)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH2DDR5IL_CR_DCCLANESTATUS0_REG                                (0x00009A5C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH2DDR5IL_CR_DCCLANESTATUS1_REG                                (0x00009A60)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH2DDR5IL_CR_DDRCRMARGINMODECONTROL_REG                        (0x00009A64)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH2DDR5IL_CR_DDRCRMARGINMODEDEBUGMSB_REG                       (0x00009A68)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH2DDR5IL_CR_DDRCRMARGINMODEDEBUGLSB_REG                       (0x00009A6C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH2DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                    (0x00009A70)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH2DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG               (0x00009A74)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH2DDR5IL_CR_DDRCRADC_REG                                      (0x00009A78)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH2DDR5IL_CR_DFXCCMON_REG                                      (0x00009A7C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH2DDR5IL_CR_DDRCRMARGINMODECONTROL1_REG                       (0x00009A80)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH2DDR5IL_CR_CBB_BONUS0_REG                                    (0x00009A84)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH2DDR5IL_CR_BONUS0_REG                                        (0x00009A88)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH2DDR5IL_CR_REFPICLKOFFSET_REG                                (0x00009A8C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH2DDR5IL_CR_LPMODE4_FLYBY_REG                                 (0x00009A90)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH2DDR5IL_CR_CCC_OVRDCTL_REG                                   (0x00009A94)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH2DDR5IL_DDRTXDLL_CR_PICODELUT0_REG                           (0x00009A98)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH2DDR5IL_DDRTXDLL_CR_PICODELUT1_REG                           (0x00009A9C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH2DDR5IL_DDRTXDLL_CR_PICODELUT2_REG                           (0x00009AA0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH2DDR5IL_DDRTXDLL_CR_PICODELUT3_REG                           (0x00009AA4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH2DDR5IL_CR_TERDCCCONTROL0_REG                                (0x00009AA8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH2DDR5IL_CR_CS_GEARDOWN_REG                                   (0x00009AAC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH2DDR5IL_CR_DDRCRCCCDCCCLKLANE5_REG                           (0x00009AB0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH2DDR5IL_CR_COMPUPDATEOVERRIDE_REG                            (0x00009AB4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH2DDR5IL_CR_SPARE_REG                                         (0x00009AB8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define CH3DDR5IL_CR_DDRCRCLKCOMP_REG                                  (0x00009B00)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5IL_CR_DDRCRCTLCOMP_REG                                  (0x00009B04)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5IL_CR_DDRCRCACOMP_REG                                   (0x00009B08)
//Duplicate of DDRPHY_DDRCOMP_CR_DDRCRCACOMP_REG

#define CH3DDR5IL_CR_DDRCRCLKCTLCACOMPOFFSET_REG                       (0x00009B0C)
//Duplicate of CH0CCC_CR_DDRCRCLKCTLCACOMPOFFSET_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE0_REG                                 (0x00009B10)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE1_REG                                 (0x00009B14)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE2_REG                                 (0x00009B18)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE3_REG                                 (0x00009B1C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE4_REG                                 (0x00009B20)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE5_REG                                 (0x00009B24)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE5_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE6_REG                                 (0x00009B28)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE7_REG                                 (0x00009B2C)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE8_REG                                 (0x00009B30)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCLANE9_REG                                 (0x00009B34)
//Duplicate of CH0CCC_CR_DDRCRCCCLANE0_REG

#define CH3DDR5IL_CR_DDRCRCCCPICODES_REG                               (0x00009B38)
//Duplicate of CH0CCC_CR_DDRCRCCCPICODES_REG

#define CH3DDR5IL_CR_DDRCRPERBITDESKEWRISEFALL_REG                     (0x00009B3C)
//Duplicate of CH0CCC_CR_DDRCRPERBITDESKEWRISEFALL_REG

#define CH3DDR5IL_CR_DCCFSMCONTROL_REG                                 (0x00009B40)
//Duplicate of CH0CCC_CR_DCCFSMCONTROL_REG

#define CH3DDR5IL_CR_DCCCALCCONTROL_REG                                (0x00009B44)
//Duplicate of CH0CCC_CR_DCCCALCCONTROL_REG

#define CH3DDR5IL_CR_REFPICONTROL_REG                                  (0x00009B48)
//Duplicate of DATA0CH0_CR_REFPICONTROL_REG

#define CH3DDR5IL_CR_DDRCRBSCANDATA_REG                                (0x00009B50)
//Duplicate of CH0CCC_CR_DDRCRBSCANDATA_REG

#define CH3DDR5IL_CR_REFPIFSMCONTROL_REG                               (0x00009B54)
//Duplicate of DATA0CH0_CR_REFPIFSMCONTROL_REG

#define CH3DDR5IL_CR_DCCFSMSTATUS_REG                                  (0x00009B58)
//Duplicate of CH0CCC_CR_DCCFSMSTATUS_REG

#define CH3DDR5IL_CR_DCCLANESTATUS0_REG                                (0x00009B5C)
//Duplicate of CH0CCC_CR_DCCLANESTATUS0_REG

#define CH3DDR5IL_CR_DCCLANESTATUS1_REG                                (0x00009B60)
//Duplicate of CH0CCC_CR_DCCLANESTATUS1_REG

#define CH3DDR5IL_CR_DDRCRMARGINMODECONTROL_REG                        (0x00009B64)
//Duplicate of DDRDATA_SHARED0_CR_DDRCRMARGINMODECONTROL_REG

#define CH3DDR5IL_CR_DDRCRMARGINMODEDEBUGMSB_REG                       (0x00009B68)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH3DDR5IL_CR_DDRCRMARGINMODEDEBUGLSB_REG                       (0x00009B6C)
//Duplicate of DATA0CH0_CR_DDRCRMARGINMODEDEBUGMSB0_REG

#define CH3DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG                    (0x00009B70)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSOR_REG

#define CH3DDR5IL_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG               (0x00009B74)
//Duplicate of DATA0CH0_CR_DDRCRVCCCLKFFCURRENTSENSORCOUNT_REG

#define CH3DDR5IL_CR_DDRCRADC_REG                                      (0x00009B78)
//Duplicate of DATA0CH0_CR_DDRCRADC_REG

#define CH3DDR5IL_CR_DFXCCMON_REG                                      (0x00009B7C)
//Duplicate of DDRPHY_DDRCOMP_CR_DFXCCMON_REG

#define CH3DDR5IL_CR_DDRCRMARGINMODECONTROL1_REG                       (0x00009B80)
//Duplicate of CH0CCC_CR_DDRCRMARGINMODECONTROL1_REG

#define CH3DDR5IL_CR_CBB_BONUS0_REG                                    (0x00009B84)
//Duplicate of CH0CCC_CR_CBB_BONUS0_REG

#define CH3DDR5IL_CR_BONUS0_REG                                        (0x00009B88)
//Duplicate of CH0CCC_CR_BONUS0_REG

#define CH3DDR5IL_CR_REFPICLKOFFSET_REG                                (0x00009B8C)
//Duplicate of CH0CCC_CR_REFPICLKOFFSET_REG

#define CH3DDR5IL_CR_LPMODE4_FLYBY_REG                                 (0x00009B90)
//Duplicate of CH0CCC_CR_LPMODE4_FLYBY_REG

#define CH3DDR5IL_CR_CCC_OVRDCTL_REG                                   (0x00009B94)
//Duplicate of CH0CCC_CR_CCC_OVRDCTL_REG

#define CH3DDR5IL_DDRTXDLL_CR_PICODELUT0_REG                           (0x00009B98)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT0_REG

#define CH3DDR5IL_DDRTXDLL_CR_PICODELUT1_REG                           (0x00009B9C)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT1_REG

#define CH3DDR5IL_DDRTXDLL_CR_PICODELUT2_REG                           (0x00009BA0)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT2_REG

#define CH3DDR5IL_DDRTXDLL_CR_PICODELUT3_REG                           (0x00009BA4)
//Duplicate of DDRDATA_SHARED0_DDRTXDLL_CR_PICODELUT3_REG

#define CH3DDR5IL_CR_TERDCCCONTROL0_REG                                (0x00009BA8)
//Duplicate of DATA0CH0_CR_TERDCCCONTROL0_REG

#define CH3DDR5IL_CR_CS_GEARDOWN_REG                                   (0x00009BAC)
//Duplicate of CH0CCC_CR_CS_GEARDOWN_REG

#define CH3DDR5IL_CR_DDRCRCCCDCCCLKLANE5_REG                           (0x00009BB0)
//Duplicate of CH0CCC_CR_DDRCRCCCDCCCLKLANE5_REG

#define CH3DDR5IL_CR_COMPUPDATEOVERRIDE_REG                            (0x00009BB4)
//Duplicate of CH0CCC_CR_COMPUPDATEOVERRIDE_REG

#define CH3DDR5IL_CR_SPARE_REG                                         (0x00009BB8)
//Duplicate of DATA0CH0_CR_SPARE_REG

#define DDRBROADCAST_CR_DIGVIEW_REG                                    (0x00009F70)
#define DDRBROADCAST_CR_DIGVIEW_DEF                                    (0x00000000)
#define DDRBROADCAST_CR_DIGVIEW_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_OFF                     ( 0)
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_WID                     ( 8)
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_MSK                     (0x000000FF)
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_MIN                     (0)
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_MAX                     (255) // 0x000000FF
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_DEF                     (0x00000000)
  #define DDRBROADCAST_CR_DIGVIEW_DigView0_Sel_HSH                     (0x08009F70)

  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_OFF                     ( 8)
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_WID                     ( 8)
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_MSK                     (0x0000FF00)
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_MIN                     (0)
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_MAX                     (255) // 0x000000FF
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_DEF                     (0x00000000)
  #define DDRBROADCAST_CR_DIGVIEW_DigView1_Sel_HSH                     (0x08109F70)

  #define DDRBROADCAST_CR_DIGVIEW_Spare_OFF                            (16)
  #define DDRBROADCAST_CR_DIGVIEW_Spare_WID                            (16)
  #define DDRBROADCAST_CR_DIGVIEW_Spare_MSK                            (0xFFFF0000)
  #define DDRBROADCAST_CR_DIGVIEW_Spare_MIN                            (0)
  #define DDRBROADCAST_CR_DIGVIEW_Spare_MAX                            (65535) // 0x0000FFFF
  #define DDRBROADCAST_CR_DIGVIEW_Spare_DEF                            (0x00000000)
  #define DDRBROADCAST_CR_DIGVIEW_Spare_HSH                            (0x10209F70)

#define DDRBROADCAST_CR_ANAVIEW_REG                                    (0x00009F74)
#define DDRBROADCAST_CR_ANAVIEW_DEF                                    (0x00000000)
#define DDRBROADCAST_CR_ANAVIEW_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_OFF                   ( 0)
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_WID                   ( 1)
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_MSK                   (0x00000001)
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_MIN                   (0)
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_MAX                   (1) // 0x00000001
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_DEF                   (0x00000000)
  #define DDRBROADCAST_CR_ANAVIEW_View_Analog_En_HSH                   (0x01009F74)

  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_OFF                  ( 1)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_WID                  ( 3)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_MSK                  (0x0000000E)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_MIN                  (0)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_MAX                  (7) // 0x00000007
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_DEF                  (0x00000000)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl0muxsel_HSH                  (0x03029F74)

  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_OFF                  ( 4)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_WID                  ( 5)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_MSK                  (0x000001F0)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_MIN                  (0)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_MAX                  (31) // 0x0000001F
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_DEF                  (0x00000000)
  #define DDRBROADCAST_CR_ANAVIEW_anaviewl1muxsel_HSH                  (0x05089F74)

  #define DDRBROADCAST_CR_ANAVIEW_Spare_OFF                            ( 9)
  #define DDRBROADCAST_CR_ANAVIEW_Spare_WID                            (23)
  #define DDRBROADCAST_CR_ANAVIEW_Spare_MSK                            (0xFFFFFE00)
  #define DDRBROADCAST_CR_ANAVIEW_Spare_MIN                            (0)
  #define DDRBROADCAST_CR_ANAVIEW_Spare_MAX                            (8388607) // 0x007FFFFF
  #define DDRBROADCAST_CR_ANAVIEW_Spare_DEF                            (0x00000000)
  #define DDRBROADCAST_CR_ANAVIEW_Spare_HSH                            (0x17129F74)

#define DDRBROADCAST_CR_DDRMULTICASTCONTROL_REG                        (0x00009F78)
#define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DEF                        (0x00000000)
#define DDRBROADCAST_CR_DDRMULTICASTCONTROL_VOLATILE_BITFIELDS_MSK     (0x00000000)

  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_OFF         ( 0)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_WID         ( 1)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_MSK         (0x00000001)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_MIN         (0)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_MAX         (1) // 0x00000001
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_DEF         (0x00000000)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_DdrGVCrBlock_HSH         (0x01009F78)

  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_OFF                ( 1)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_WID                (31)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_MSK                (0xFFFFFFFE)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_MIN                (0)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_MAX                (2147483647) // 0x7FFFFFFF
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_DEF                (0x00000000)
  #define DDRBROADCAST_CR_DDRMULTICASTCONTROL_Spare_HSH                (0x1F029F78)

#define DDRBROADCAST_CR_DDRVIEW_REG                                    (0x00009F7C)
#define DDRBROADCAST_CR_DDRVIEW_DEF                                    (0x00000000)
#define DDRBROADCAST_CR_DDRVIEW_VOLATILE_BITFIELDS_MSK                 (0x00000000)

  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_OFF                    ( 0)
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_WID                    ( 7)
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_MSK                    (0x0000007F)
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_MIN                    (0)
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_MAX                    (127) // 0x0000007F
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_DEF                    (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View1_FubAddr_HSH                    (0x07009F7C)

  #define DDRBROADCAST_CR_DDRVIEW_View_En_OFF                          ( 7)
  #define DDRBROADCAST_CR_DDRVIEW_View_En_WID                          ( 1)
  #define DDRBROADCAST_CR_DDRVIEW_View_En_MSK                          (0x00000080)
  #define DDRBROADCAST_CR_DDRVIEW_View_En_MIN                          (0)
  #define DDRBROADCAST_CR_DDRVIEW_View_En_MAX                          (1) // 0x00000001
  #define DDRBROADCAST_CR_DDRVIEW_View_En_DEF                          (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View_En_HSH                          (0x010E9F7C)

  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_OFF                        ( 8)
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_WID                        ( 8)
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_MSK                        (0x0000FF00)
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_MIN                        (0)
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_MAX                        (255) // 0x000000FF
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_DEF                        (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View1_Sel_HSH                        (0x08109F7C)

  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_OFF                    (16)
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_WID                    ( 7)
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_MSK                    (0x007F0000)
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_MIN                    (0)
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_MAX                    (127) // 0x0000007F
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_DEF                    (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View0_FubAddr_HSH                    (0x07209F7C)

  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_OFF                   (23)
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_WID                   ( 1)
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_MSK                   (0x00800000)
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_MIN                   (0)
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_MAX                   (1) // 0x00000001
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_DEF                   (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View_Analog_En_HSH                   (0x012E9F7C)

  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_OFF                        (24)
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_WID                        ( 8)
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_MSK                        (0xFF000000)
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_MIN                        (0)
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_MAX                        (255) // 0x000000FF
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_DEF                        (0x00000000)
  #define DDRBROADCAST_CR_DDRVIEW_View0_Sel_HSH                        (0x08309F7C)



#pragma pack(pop)
#endif /* __MrcRegisterPtl9xxx_h__ */







