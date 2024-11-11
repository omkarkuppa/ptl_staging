/** @file
  Internal header of the Setup Component.

  Macros defined here are used to specify VFR FormId or QuestionId in VFR and SD files.
  When VFR files are updated, new macros may be required to specify new FormId or QuestionId.

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

#define MAIN_MAIN                                 4096
#define ADVANCED_MAIN                             4097
#define BOOT_MAIN                                 4098
#define BBS_ORDER_FORM                            4099
#define DEBUG_FORM_ID                             4102
#define PLATFORM_DEBUG_OPTION_KEY                 4103
#define WHEA_SETUP_KEY                            4105
#define ACPI_FORM_ID                              4106
#define SETUP_VOLATILE_DATA_ID                    4107
#define CPU_FEATURES_ID                           4108
#define CPU_FORM_ID                               4109
#define POWER_CONFIG_FORM_ID                      4110
#define CPU_POWER_CONFIG_FORM_ID                  4111
#define VID_OPTIONS_FORM_ID                       4112
#define CPU_TURBO_OPTIONS_FORM_ID                 4113
#define CPU_LOCK_OPTIONS_FORM_ID                  4114
#define PERFORMANCE_MENU_CONFIG_FORM_ID           4115
#define PROCESSOR_OC_CONFIG_FORM_ID               4116
#define CPU_TURBO_RATIO_LIMIT_FORM_ID             4117
#define BCLK_CONFIG_FORM_ID                       4118
#define MANAGEABILITY_FORM_ID                     4120
#define SKIN_TEMP_CONTROL_FORM_ID                 4172
#define SKIN_TEMP_CONTROL_SENSOR_0_FORM_ID        4173
#define SKIN_TEMP_CONTROL_SENSOR_1_FORM_ID        4174
#define SKIN_TEMP_CONTROL_SENSOR_2_FORM_ID        4175
#define TRACEHUB_BUFFER_SIZE_KEY                  4300
#define TRACEHUB_BUFFER_SIZE_KEY1                 4301
#define TRACEHUB_BUFFER_SIZE_KEY2                 4302
#define TRACEHUB_BUFFER_SIZE_KEY3                 4303
#define PROCESSOR_TRACE_MEM_SIZE_KEY              4304

#define KEY_XeVoltage                             4121
#define OVERCLOCK_KEY                             4122
#define GT_OC_CONFIG_FORM_ID                      4123
#define MEMORY_CONFIG_FORM_ID                     4124
#define KEY_MEMINFO                               4125
#define KEY_MEMINFO1                              4126
#define KEY_MEMINFO2                              4127
#define KEY_MEMINFO3                              4128
#define KEY_MEMINFO4                              4129
#define KEY_MEMINFO5                              4130
#define KEY_MEMINFO6                              4131
#define KEY_MEMINFO7                              4132
#define KEY_MEMINFO8                              4133
#define KEY_MEMINFO9                              4134
#define KEY_MEMINFO10                             4135
#define MEMORY_FORM_ID                            4136
#define NB_FORM_ID                                4137
#define IGPU_FORM_ID                              4138
#define COMMON_NB_OPTIONS_FORM_ID                 4139
#define GT_MEDIA_POWER_CONFIG_FORM_ID             4142
#define SB_FORM_ID                                4143
#define PCIE_DEVICE_OPTIONS_FORM_ID               4144
#define USB_OPTIONS_FORM_ID                       4145
#define PCIE_FORM_ID                              5433
#define DMI_OPTIONS_FORM_ID                       6004
#define ADVANCED_DMI_OPTIONS_FORM_ID              6005
#define PCH_PCIE_CLOCK_FORM_ID                    5504

#define PCIE0_EQ_SETTINGS_FORM_ID                 5505
#define PCIE1_EQ_SETTINGS_FORM_ID                 5506
#define PCIE2_EQ_SETTINGS_FORM_ID                 5507
#define PCIE3_EQ_SETTINGS_FORM_ID                 5508
#define PCIE4_EQ_SETTINGS_FORM_ID                 5509
#define PCIE5_EQ_SETTINGS_FORM_ID                 5510
#define PCIE6_EQ_SETTINGS_FORM_ID                 5515
#define PCIE7_EQ_SETTINGS_FORM_ID                 5516
#define PCIE8_EQ_SETTINGS_FORM_ID                 5517
#define PCIE9_EQ_SETTINGS_FORM_ID                 5518
#define PCIE10_EQ_SETTINGS_FORM_ID                5519
#define PCIE11_EQ_SETTINGS_FORM_ID                5520
#define PCIE12_EQ_SETTINGS_FORM_ID                5521
#define PCIE13_EQ_SETTINGS_FORM_ID                5522
#define PCIE14_EQ_SETTINGS_FORM_ID                5523
#define PCIE15_EQ_SETTINGS_FORM_ID                5524
#define PCIE16_EQ_SETTINGS_FORM_ID                5525
#define PCIE17_EQ_SETTINGS_FORM_ID                5526
#define PCIE18_EQ_SETTINGS_FORM_ID                5527
#define PCIE19_EQ_SETTINGS_FORM_ID                5528
#define PCIE20_EQ_SETTINGS_FORM_ID                5529
#define PCIE21_EQ_SETTINGS_FORM_ID                5530
#define PCIE22_EQ_SETTINGS_FORM_ID                5531
#define PCIE23_EQ_SETTINGS_FORM_ID                5532
#define PCIE24_EQ_SETTINGS_FORM_ID                4995
#define PCIE25_EQ_SETTINGS_FORM_ID                4996
#define PCIE26_EQ_SETTINGS_FORM_ID                4997
#define PCIE27_EQ_SETTINGS_FORM_ID                4998
#define PCIE28_EQ_SETTINGS_FORM_ID                4999

#define USB_TEST_OPTIONS_FORM_ID                  5300
#define TEST_USB_PORT_LOCK_FORM_ID                5301
#define TEST_USB_PORT_LOCK_POLICY_MENU_FORM_ID    5302
#define SATA_DEVICE_OPTIONS_FORM_ID               4146
#define PCH_SATA_SFMS_OPTIONS_FORM_ID             4147
#define PCIE_EQ_RP0_SETTINGS_FORM_ID              5700
#define PCIE_EQ_RP1_SETTINGS_FORM_ID              5701
#define PCIE_EQ_RP2_SETTINGS_FORM_ID              5702
#define PCIE_EQ_RP3_SETTINGS_FORM_ID              5703
#define PCIE_EQ_RP4_SETTINGS_FORM_ID              5704
#define PCIE_EQ_RP5_SETTINGS_FORM_ID              5705
#define PCIE_EQ_RP6_SETTINGS_FORM_ID              5706
#define PCIE_EQ_RP7_SETTINGS_FORM_ID              5708
#define PCIE_EQ_RP8_SETTINGS_FORM_ID              5709
#define PCIE_EQ_RP9_SETTINGS_FORM_ID              5710
#define PCIE_EQ_RP10_SETTINGS_FORM_ID             5711
#define PCIE_EQ_RP11_SETTINGS_FORM_ID             5712
#define PCH_PCIERP0_OPTIONS_FORM_ID               5500
#define PCH_PCIERP1_OPTIONS_FORM_ID               4148
#define PCH_PCIERP2_OPTIONS_FORM_ID               4149
#define PCH_PCIERP3_OPTIONS_FORM_ID               4150
#define PCH_PCIERP4_OPTIONS_FORM_ID               4151
#define PCH_PCIERP5_OPTIONS_FORM_ID               4152
#define PCH_PCIERP6_OPTIONS_FORM_ID               4153
#define PCH_PCIERP7_OPTIONS_FORM_ID               4154
#define PCH_PCIERP8_OPTIONS_FORM_ID               4155
#define PCH_PCIERP9_OPTIONS_FORM_ID               5101
#define PCH_PCIERP10_OPTIONS_FORM_ID              5102
#define PCH_PCIERP11_OPTIONS_FORM_ID              5103
#define PCH_PCIERP12_OPTIONS_FORM_ID              5104
#define PCH_PCIERP13_OPTIONS_FORM_ID              5105
#define PCH_PCIERP14_OPTIONS_FORM_ID              5106
#define PCH_PCIERP15_OPTIONS_FORM_ID              5107
#define PCH_PCIERP16_OPTIONS_FORM_ID              5108
#define PCH_PCIERP17_OPTIONS_FORM_ID              5109
#define PCH_PCIERP18_OPTIONS_FORM_ID              5110
#define PCH_PCIERP19_OPTIONS_FORM_ID              5111
#define PCH_PCIERP20_OPTIONS_FORM_ID              5112
#define PCH_PCIERP21_OPTIONS_FORM_ID              5501
#define PCH_PCIERP22_OPTIONS_FORM_ID              5502
#define PCH_PCIERP23_OPTIONS_FORM_ID              5503
#define PCH_PCIERP24_OPTIONS_FORM_ID              5511
#define PCH_PCIERP25_OPTIONS_FORM_ID              5512
#define PCH_PCIERP26_OPTIONS_FORM_ID              5513
#define PCH_PCIERP27_OPTIONS_FORM_ID              5514
#define PCH_PCIERP28_OPTIONS_FORM_ID              4994

#define PCH_ASPM_PCIERP0_QUESTION_ID              5600
#define PCH_ASPM_PCIERP1_QUESTION_ID              5601
#define PCH_ASPM_PCIERP2_QUESTION_ID              5602
#define PCH_ASPM_PCIERP3_QUESTION_ID              5603
#define PCH_ASPM_PCIERP4_QUESTION_ID              5604
#define PCH_ASPM_PCIERP5_QUESTION_ID              5605
#define PCH_ASPM_PCIERP6_QUESTION_ID              5606
#define PCH_ASPM_PCIERP7_QUESTION_ID              5607
#define PCH_ASPM_PCIERP8_QUESTION_ID              5608
#define PCH_ASPM_PCIERP9_QUESTION_ID              5609
#define PCH_ASPM_PCIERP10_QUESTION_ID             5610
#define PCH_ASPM_PCIERP11_QUESTION_ID             5611
#define PCH_ASPM_PCIERP12_QUESTION_ID             5612
#define PCH_ASPM_PCIERP13_QUESTION_ID             5613
#define PCH_ASPM_PCIERP14_QUESTION_ID             5614
#define PCH_ASPM_PCIERP15_QUESTION_ID             5615
#define PCH_ASPM_PCIERP16_QUESTION_ID             5616
#define PCH_ASPM_PCIERP17_QUESTION_ID             5617
#define PCH_ASPM_PCIERP18_QUESTION_ID             5618
#define PCH_ASPM_PCIERP19_QUESTION_ID             5619
#define PCH_ASPM_PCIERP20_QUESTION_ID             5620
#define PCH_ASPM_PCIERP21_QUESTION_ID             5621
#define PCH_ASPM_PCIERP22_QUESTION_ID             5622
#define PCH_ASPM_PCIERP23_QUESTION_ID             5623
#define PCH_ASPM_PCIERP24_QUESTION_ID             5624
#define PCH_ASPM_PCIERP25_QUESTION_ID             5625
#define PCH_ASPM_PCIERP26_QUESTION_ID             5626
#define PCH_ASPM_PCIERP27_QUESTION_ID             5627
#define PCH_L1SUB_PCIERP0_QUESTION_ID             5628
#define PCH_L1SUB_PCIERP1_QUESTION_ID             5629
#define PCH_L1SUB_PCIERP2_QUESTION_ID             5630
#define PCH_L1SUB_PCIERP3_QUESTION_ID             5631
#define PCH_L1SUB_PCIERP4_QUESTION_ID             5632
#define PCH_L1SUB_PCIERP5_QUESTION_ID             5633
#define PCH_L1SUB_PCIERP6_QUESTION_ID             5634
#define PCH_L1SUB_PCIERP7_QUESTION_ID             5635
#define PCH_L1SUB_PCIERP8_QUESTION_ID             5636
#define PCH_L1SUB_PCIERP9_QUESTION_ID             5637
#define PCH_L1SUB_PCIERP10_QUESTION_ID            5638
#define PCH_L1SUB_PCIERP11_QUESTION_ID            5639
#define PCH_L1SUB_PCIERP12_QUESTION_ID            5640
#define PCH_L1SUB_PCIERP13_QUESTION_ID            5641
#define PCH_L1SUB_PCIERP14_QUESTION_ID            5642
#define PCH_L1SUB_PCIERP15_QUESTION_ID            5643
#define PCH_L1SUB_PCIERP16_QUESTION_ID            5644
#define PCH_L1SUB_PCIERP17_QUESTION_ID            5645
#define PCH_L1SUB_PCIERP18_QUESTION_ID            5646
#define PCH_L1SUB_PCIERP19_QUESTION_ID            5647
#define PCH_L1SUB_PCIERP20_QUESTION_ID            5648
#define PCH_L1SUB_PCIERP21_QUESTION_ID            5649
#define PCH_L1SUB_PCIERP22_QUESTION_ID            5650
#define PCH_L1SUB_PCIERP23_QUESTION_ID            5651
#define PCH_L1SUB_PCIERP24_QUESTION_ID            5652
#define PCH_L1SUB_PCIERP25_QUESTION_ID            5653
#define PCH_L1SUB_PCIERP26_QUESTION_ID            5654
#define PCH_L1SUB_PCIERP27_QUESTION_ID            5655
#define PCH_PTM_PCIERP0_QUESTION_ID               5656
#define PCH_PTM_PCIERP1_QUESTION_ID               5657
#define PCH_PTM_PCIERP2_QUESTION_ID               5658
#define PCH_PTM_PCIERP3_QUESTION_ID               5659
#define PCH_PTM_PCIERP4_QUESTION_ID               5660
#define PCH_PTM_PCIERP5_QUESTION_ID               5661
#define PCH_PTM_PCIERP6_QUESTION_ID               5662
#define PCH_PTM_PCIERP7_QUESTION_ID               5663
#define PCH_PTM_PCIERP8_QUESTION_ID               5664
#define PCH_PTM_PCIERP9_QUESTION_ID               5665
#define PCH_PTM_PCIERP10_QUESTION_ID              5666
#define PCH_PTM_PCIERP11_QUESTION_ID              5667
#define PCH_PTM_PCIERP12_QUESTION_ID              5668
#define PCH_PTM_PCIERP13_QUESTION_ID              5669
#define PCH_PTM_PCIERP14_QUESTION_ID              5670
#define PCH_PTM_PCIERP15_QUESTION_ID              5671
#define PCH_PTM_PCIERP16_QUESTION_ID              5672
#define PCH_PTM_PCIERP17_QUESTION_ID              5673
#define PCH_PTM_PCIERP18_QUESTION_ID              5674
#define PCH_PTM_PCIERP19_QUESTION_ID              5675
#define PCH_PTM_PCIERP20_QUESTION_ID              5676
#define PCH_PTM_PCIERP21_QUESTION_ID              5677
#define PCH_PTM_PCIERP22_QUESTION_ID              5678
#define PCH_PTM_PCIERP23_QUESTION_ID              5679
#define PCH_PTM_PCIERP24_QUESTION_ID              5680
#define PCH_PTM_PCIERP25_QUESTION_ID              5681
#define PCH_PTM_PCIERP26_QUESTION_ID              5682
#define PCH_PTM_PCIERP27_QUESTION_ID              5683

#define PCH_HDAUDIO_OPTIONS_FORM_ID               4156
#define PCH_HDAUDIO_ADVANCED_OPTIONS_FORM_ID      4159
#define PCH_HDAUDIO_FEATURES_FORM_ID              4160
#define PCH_HDAUDIO_SNDW0_FORM_ID                 4161
#define PCH_HDAUDIO_SNDW1_FORM_ID                 4185
#define PCH_HDAUDIO_SNDW2_FORM_ID                 4186
#define PCH_HDAUDIO_SNDW3_FORM_ID                 4187
#define PCH_HDAUDIO_SNDW4_FORM_ID                 4188

#define SECURITY_OPTIONS_FORM_ID                  4157
#define ME_FORM_ID                                4158
#define ME_DEBUG_CONFIGURATION_FORM_ID            4162
#define ME_FW_UPGRADE_FORM_ID                     4163
#define ARB_SVN_CONFIGURATION_FORM_ID             4164
#define ME_SMBIOS_FORM_ID                         4165
#define THERMAL_CONFIG_FORM_ID                    4166
#define CPU_THERMAL_CONFIG_FORM_ID                4167
#define MEMORY_THERMAL_CONFIG_FORM_ID             4168
#define PLATFORM_THERMAL_CONFIG_FORM_ID           4169
#define OEM_OBJ_FORM_ID                           4170
#define ME_FW_FIPS_MODE_FORM_ID                   4181
#define ADVANCED_DEBUG_FORM_ID                    4182
#define USB_CONFIG_FORM_ID                        4183
#define ME_FW_UPID_FORM_ID                        4184
#define HHM_FORM_ID                               4222
#define INTERNAL_TEMP_KEY                         4224
#define REMOTE_TEMP_KEY                           4225
#define FAN_1_KEY                                 4226
#define FAN_2_KEY                                 4227
#define FAN_3_KEY                                 4228
#define VOLT_1_5V_KEY                             4229
#define VOLT_3_3V_KEY                             4230
#define VOLT_5V_KEY                               4231
#define VOLT_12V_KEY                              4232
#define VOLT_CORE_KEY                             4233
#define CPU_FAN_KEY                               8236
#define PCH_DTS_TEMP_KEY                          8243
#define HDC_CONFIG_FORM_ID                        4256
#define KEY_RatioLimitNumCore0                    4257
#define KEY_RatioLimitNumCore1                    4258
#define KEY_RatioLimitNumCore2                    4259
#define KEY_RatioLimitNumCore3                    4260
#define KEY_RatioLimitNumCore4                    4261
#define KEY_RatioLimitNumCore5                    4262
#define KEY_RatioLimitNumCore6                    4263
#define KEY_RatioLimitNumCore7                    4264
#define KEY_RatioLimitRatio0                      4265
#define KEY_RatioLimitRatio1                      4266
#define KEY_RatioLimitRatio2                      4267
#define KEY_RatioLimitRatio3                      4268
#define KEY_RatioLimitRatio4                      4269
#define KEY_RatioLimitRatio5                      4270
#define KEY_RatioLimitRatio6                      4271
#define KEY_RatioLimitRatio7                      4272
#define KEY_AtomRatioLimitNumCore0                4273
#define KEY_AtomRatioLimitNumCore1                4274
#define KEY_AtomRatioLimitNumCore2                4275
#define KEY_AtomRatioLimitNumCore3                4276
#define KEY_AtomRatioLimitNumCore4                4277
#define KEY_AtomRatioLimitNumCore5                4278
#define KEY_AtomRatioLimitNumCore6                4279
#define KEY_AtomRatioLimitNumCore7                4280
#define KEY_AtomRatioLimitRatio0                  4281
#define KEY_AtomRatioLimitRatio1                  4282
#define KEY_AtomRatioLimitRatio2                  4283
#define KEY_AtomRatioLimitRatio3                  4284
#define KEY_AtomRatioLimitRatio4                  4285
#define KEY_AtomRatioLimitRatio5                  4286
#define KEY_AtomRatioLimitRatio6                  4287
#define KEY_AtomRatioLimitRatio7                  4288
#define KEY_BootFrequency                         5496
#define KEY_TurboMode                             5497
#define KEY_DLVR_RFIM                             5498

//
// CPU disable control.
//
#define PROCESSOR_DISABLE_FORM_ID                 4289

//
// FVM control
//
#define KEY_CoreVrEnableFastVmode                 4290
#define KEY_GtVrEnableFastVmode                   4291
#define KEY_SaVrEnableFastVmode                   4292
#define KEY_IccLimit0                             4293
#define KEY_IccLimit1                             4294
#define KEY_IccLimit2                             4295
#define OC_TVB_CONFIG_FORM_ID                     4296

#define USB_MASS_DEV_NUM_KEY                      6000
#define USB_MASS_DEV_VALID_KEY                    6001
#define USB_CONTROLLER_NUM_KEY                    6002
#define USB_SUPPORT_KEY                           6003

#define DEBUG_INTERFACE_FORM_ID                   4319
#define REWORK_INFORMATION_FORM_ID                4320
#define KEY_CpuRatioLimit                         4321
#define KEY_TPM_SUPPORT                           4323
#define MEM_TRAINING_FORM_ID                      4324
#define MEM_THERMMGT_FORM_ID                      4328
#define MEMORY_CONFIG_PPR_ADV_FORM_ID             4341
#define MEMORY_CONFIG_PPR_FORM_ID                 4337
#define KEY_PPR_REPAIR_PHYSICAL_ADDRESS           4338

#define MEM_OFFSET_KNOBS_FORM_ID                  5037
#define CPU_MEMORY_CAP_FORM_ID                    4325
#define MEM_TURNAROUNDTIMING_FORM_ID              4326
#define MEM_VALIDATION_MENU_FORM_ID               4327

#define SA_TCSS_OPTIONS_FORM_ID                   4332
#define SA_DISPLAY_OPTIONS_FORM_ID                4333
#define SA_VTD_FORM_ID                            4334

#define RFI_OPTIONS_FORM_ID                       4335

#define CPU_CTDP_OPTIONS_FORM_ID                  4329
#define CPU_CTDP_CUSTOM_OPTIONS_FORM_ID           4330

#define VR_ICCMAX_CURRENT_OVERRIDE_CONFIG_FORM_ID 4336

#define RTD3_FORM_ID                              4339

#define AMT_FORM_ID                               4340
#define ASF_CONFIGURATION_FORM_ID                 4342
#define ONE_CLICK_RECOVERY_FORM_ID                4402


#define MKTME_STATUS_QUESTION_ID                  4405

#define GEN3_EP_PRESET_FORM_ID                    4346
#define DPTF_FORM_ID                              4348

#define SERIAL_IO_OPTIONS_FORM_ID                 4349
#define ISCT_FORM_ID                              4350
#define MEMSS_OC_CONFIG_FORM_ID                   4352
#define RING_OC_CONFIG_FORM_ID                    4353
#define UNCORE_OC_CONFIG_FORM_ID                  4354
#define NGU_OC_CONFIG_FORM_ID                     5638
#define VCC_CONFIG_FORM_ID                        4355

#define GEN3_RX_CTLE_CONTROL_FORM_ID              4356
#define DRAM_POWER_METER_FORM_ID                  4357
#define MEMORY_THERMAL_REPORTING_FORM_ID          4358
#define MEMORY_RAPL_FORM_ID                       4359

#define ME_FEATURES_ID                            4360

#define KEY_TPM2_GOTO                             7899

#define SECURE_BOOT_VARIABLE_KEY                  4363

#define KEY_MEMINFO11                             4364
#define KEY_MEMINFO12                             4365
#define KEY_MEMINFO13                             4366
#define CPU_PL3_OPTIONS_FORM_ID                   4369
#define IGPU_IUER_FORM_ID                         4370

#define PCH_SERIAL_IO_DMA_FORM_ID                 4371
#define PCH_SERIAL_IO_I2C0_FORM_ID                4372
#define PCH_SERIAL_IO_I2C1_FORM_ID                4373
#define PCH_SERIAL_IO_SPI0_FORM_ID                4374
#define PCH_SERIAL_IO_SPI1_FORM_ID                4375
#define PCH_SERIAL_IO_UART0_FORM_ID               4376
#define PCH_SERIAL_IO_UART1_FORM_ID               4377
#define PCH_SERIAL_IO_SDIO_FORM_ID                4378
#define TBT_FORM_ID                               4379
#define KEY_RING_MAX_OC_RATIO_LIMIT               4380
#define KEY_LOW_POWER_S0_IDLE                     4381
#define TBT_COMPILE_TYPE                          1
#define TOUCH_PAD_FORM_ID                         4382
#define TOUCH_PANEL_FORM_ID                       4383

#define PLATFORM_SETTINGS_FORM_ID                 4389
#define DMI_GEN3_RP_PRESET_FORM_ID                4390
#define DMI_GEN3_EP_PRESET_FORM_ID                4391
#define DMI_GEN3_EP_HINT_FORM_ID                  4392
#define DMI_GEN3_RX_CTLE_CONTROL_FORM_ID          4393
#define DMI_GEN3_DS_PORT_PRESET_FORM_ID           4396
#define DMI_GEN3_US_PORT_PRESET_FORM_ID           4401
#define DMI_GEN3_MULTIVC_FORM_ID                  4397
#define DMI_GEN3_LTCO_OVERRIDE_FORM_ID            4399
#define DMI_GEN3_RTCO_PRESET_OVERRIDE_FORM_ID     5007
#define DMI_GEN3_RTCO_COE_OVERRIDE_FORM_ID        5008
#define DMI_GEN4_DS_PORT_PRESET_FORM_ID           5009
#define DMI_GEN4_US_PORT_PRESET_FORM_ID           5010
#define DMI_GEN4_LTCO_OVERRIDE_FORM_ID            5011
#define DMI_GEN4_RTCO_COE_OVERRIDE_FORM_ID        5012
#define PEP_FORM_ID                               4394
#define KEY_MEMINFO_PROFILE                       4395
#define KEY_MEMINFO14                             4500
#define KEY_MEMINFO15                             4501
#define KEY_MEMINFO16                             4502
#define KEY_MEMINFO17                             4503
#define KEY_MEMINFO18                             4504
#define KEY_MEMINFO19                             4505
#define KEY_MEMINFO20                             4506
#define KEY_MEMINFO21                             4507
#define MEMORY_CONFIG_RCOMP_TARGET_FORM_ID        4508
#define MEMORY_CONFIG_DIMM_ODT_FORM_ID            4509
#define MEMORY_CONFIG_DIMM_DFE_FORM_ID            4510
#define KEY_MEMINFO22                             4511
#define EC_PECI_MODE_KEY                          7806

#define GOP_FORM_ID                               5089
#define LFP_CONFIG_FORM_ID                        5090
#define DEVICE1_EFP1_FORM_ID                      5091
#define DEVICE2_EFP2_FORM_ID                      5092
#define DEVICE3_EFP3_FORM_ID                      5093
// MipiCam
#define KEY_SA_IPU_ENABLE                         5039
#define MIPICAM_OPTIONS_FORM_ID                   5042
// Storage and Communication Subsystem (SCS)
#define SCS_OPTIONS_FORM_ID                       5044
// Integrated Sensor Hub (ISH)
#define ISH_OPTIONS_FORM_ID                       5046

// PCH POWERON option menu
#define PCH_PWRON_OPTIONS_FORM_ID                 5045

// PCH Thermal Throttling Options Menu
#define PCH_THERMAL_THROTTLING_OPTIONS_FORM_ID    5054

// CPU VR Config
#define CPU_VR_OPTIONS_FORM_ID                    5047
#define CPU_SA_VR_OPTIONS_FORM_ID                 5048
#define CPU_IA_VR_OPTIONS_FORM_ID                 5049
#define CPU_ACOUSTIC_NOISE_FORM_ID                5050
#define CPU_GT_VR_OPTIONS_FORM_ID                 5052
#define CPU_VCCIN_VR_OPTIONS_FORM_ID              5040
#define CPU_ATOM_VR_OPTIONS_FORM_ID               5576

#define KEY_TjMaxOffset                           5041

#define CPU_SETUP_VOLATILE_DATA_ID                5053
#define HHM_SETUP_REFRESH_KEY                     7893
#define THERMAL_SENSOR_1_KEY                      8054
#define THERMAL_SENSOR_2_KEY                      8055
#define THERMAL_SENSOR_3_KEY                      8056
#define THERMAL_SENSOR_4_KEY                      8057
#define THERMAL_SENSOR_5_KEY                      8058

#define CX_ENABLE_QUESTION_ID                     5126
#define AC_SPLIT_LOCK_QUESTION_ID                 5127
#define HWP_ENABLE_QUESTION_ID                    5128
#define ENABLE_GV_QUESTION_ID                     5130
#define RTD3_SUPPORT_QUESTION_ID                  7891
#define SA_GV_QUESTION_ID                         5132
#define PAGE_IDLE_TIMEOUT_QUESTION_ID             5133
#define POWER_DOWN_MODE_QUESTION_ID               5134
#define VIDEO_RS2_QUESTION_ID                     5135
#define SA_CONFIGURE_GT_QUESTION_ID               5136
#define SA_VTD_QUESTION_ID                        5138
#define SA_WRC_FEATURE_QUESTION_ID                5139
#define PCH_DMI_ASPM_QUESTION_ID                  8140
#define PCH_PCIE_CKG_QUESTION_ID                  5141
#define PCH_LEGACY_IO_LOW_LATENCY_QUESTION_ID     8142
#define PCH_TSN_MULTI_VC_QUESTION_ID              5144
#define VT_QUESTION_ID                            5145
#define SA_GV_MASK_QUESTION_ID                    5123


// PCH FW-Configuration Menu
#define ME_FORM_ACTION_KEY                        7804
#define MNG_STATE_KEY                             7812
#define DAM_STATE_KEY                             7802
#define AMT_FORM_ACTION_KEY                       7803

// IMR Options Menu
#define PCH_PCIE_IMR_OPTIONS_FORM_ID              5075

#define PMC_CONFIGURATION_FORM_ID                 5090
#define PMC_ADR_CONFIGURATION_FORM_ID             5091

// Discrete TBT Menu
#define KEY_DTBT_SUPPORT                          5094
#define DTBT_FORM_ID                              5076
#define DTBT_CONTROLLER0_FORM_ID                  5077
#define DTBT_CONTROLLER1_FORM_ID                  5078
#define TBT_SETUP_VOLATILE_DATA_ID                5084
#define KEY_DTBT_CONTROLLER0_HOSTROUTER           5085
#define KEY_DTBT_CONTROLLER1_HOSTROUTER           5086

#define ITBT_FORM_ID                              5200
#define ITBT_ROOTPORT0_FORM_ID                    5201
#define ITBT_ROOTPORT1_FORM_ID                    5202
#define ITBT_ROOTPORT2_FORM_ID                    5203
#define ITBT_ROOTPORT3_FORM_ID                    5204
#define SA_VMD_OPTIONS_FORM_ID                    5220
#define KEY_SA_VMD_GLOBAL_MAPPING                 5221
#define KEY_ITBT_SUPPORT                          7809
// RFI Option Menu
#define CPU_RFI_VR_OPTIONS_FORM_ID                5251
#define KEY_USB4_CM_MODE                          8252
#define PLL_TRIM_CONFIG_FORM_ID                   5414
#define SYS_FW_UPDATE_CONFIG_FORM_ID              5415

#define PLATFORM_VTIO_FORM_ID                     5420

#define KEY_RING_MIN_RATIO_LIMIT                  5416
#define KEY_RING_MAX_RATIO_LIMIT                  5417
#define KEY_CORE_MAX_OC_RATIO_LIMIT               5418
#define KEY_ATOM_MAX_OC_RATIO_LIMIT               5462

#define KEY_SA_TCSS_XHCI_EN                       5419
#define PLATFORM_TCSS_FORM_ID                     5421
// THC Options Menu
#define THC_OPTIONS_FORM_ID                       5422
#define KEY_ENABLE_THERMAL_FUN                    7805

// STC Configuration Menu
#define PLATFORM_STC_CONFIG_FORM_ID               5424

#define GEN4_RP_PRESET_FORM_ID                    5425
#define GEN4_EP_PRESET_FORM_ID                    5426
#define GEN4_EP_HINT_FORM_ID                      5427

// BCLK Configuration Menu
#define KEY_BCLK_RFI_FREQ0                        8428
#define KEY_BCLK_RFI_FREQ1                        8429
#define KEY_BCLK_RFI_FREQ2                        8430
#define KEY_BCLK_RFI_FREQ3                        8431

// WiFi Connectivity Menu
#define KEY_WIFI_POWER_LIMIT                      5432

//TXT related
#define KEY_TXT_POLICY_FIT                        5449

#define ITBT_PCIERP0_OPTIONS_FORM_ID              5438
#define ITBT_PCIERP1_OPTIONS_FORM_ID              5439
#define ITBT_PCIERP2_OPTIONS_FORM_ID              5440
#define ITBT_PCIERP3_OPTIONS_FORM_ID              5442

#define TCSS_SA_USB_FORM_ID                       5441

// TCC Configuration Menu
#define TCC_MODE_CONFIG_FORM_ID                   5434
#define KEY_TCC_MODE                              5456
#define TCC_MODE_PCH_PCIE_OPTIONS_FORM_ID         5458
// WWAN Options Menu
#define WWAN_OPTIONS_FORM_ID                      5435

#define IMAGING_FLASH0_FORM_ID                    5450
#define IMAGING_FLASH1_FORM_ID                    5451
#define IMAGING_FLASH2_FORM_ID                    5452
#define IMAGING_FLASH3_FORM_ID                    5453
#define IMAGING_FLASH4_FORM_ID                    5454
#define IMAGING_FLASH5_FORM_ID                    5455
#define KEY_PCH_STATE_AFTER_G3                    8244

//FUSA
#define FUSA_FORM_ID                              5443
#define KEY_GLOBAL_FUSA_EN                        5444

#define CPU_ATOM_CORE_FORM_ID                     5460
#define CPU_CORE_FORM_ID                          5461

#define CPU_SSC_CONFIG_FORM_ID                    5462
// DEBUG
#define DEBUG_SETUP_VOLATILE_DATA_ID              5463

//crashLog
#define CRASHLOG_FORM_ID                          5464

//FspV
#define FSPV_CONFIG_FORM_ID                       5465

#define KEY_OC_ENABLE_DEPENDENCY                  5491
#define KEY_PERCORE_DISABLE_DEPENDENCY            5492

#define KEY_CPU_D2D_RATIO                         5093

#define TEST_MENU_PCIERP0_FORM_ID                 5541
#define TEST_MENU_PCIERP1_FORM_ID                 5542
#define TEST_MENU_PCIERP2_FORM_ID                 5543
#define TEST_MENU_PCIERP3_FORM_ID                 5544
#define TEST_MENU_PCIERP4_FORM_ID                 5545
#define TEST_MENU_PCIERP5_FORM_ID                 5547
#define TEST_MENU_PCIERP6_FORM_ID                 5548
#define TEST_MENU_PCIERP7_FORM_ID                 5549
#define TEST_MENU_PCIERP8_FORM_ID                 5550
#define TEST_MENU_PCIERP9_FORM_ID                 5551
#define TEST_MENU_PCIERP10_FORM_ID                5552
#define TEST_MENU_PCIERP11_FORM_ID                5572



#define KEY_IBECC_BASE0_CHECK                     5553
#define KEY_IBECC_BASE1_CHECK                     5554
#define KEY_IBECC_BASE2_CHECK                     5555
#define KEY_IBECC_BASE3_CHECK                     5556
#define KEY_IBECC_BASE4_CHECK                     5557
#define KEY_IBECC_BASE5_CHECK                     5558
#define KEY_IBECC_BASE6_CHECK                     5559
#define KEY_IBECC_BASE7_CHECK                     5560
#define KEY_IBECC_MASK0_CHECK                     5561
#define KEY_IBECC_MASK1_CHECK                     5562
#define KEY_IBECC_MASK2_CHECK                     5563
#define KEY_IBECC_MASK3_CHECK                     5564
#define KEY_IBECC_MASK4_CHECK                     5565
#define KEY_IBECC_MASK5_CHECK                     5566
#define KEY_IBECC_MASK6_CHECK                     5567
#define KEY_IBECC_MASK7_CHECK                     5568

#define IGPU_GT_FORM_ID                           5569
#define IGPU_DISPLAY_FORM_ID                      5570
#define IGPU_MEDIA_FORM_ID                        5571

#define KEY_DEEPEST_USB_SLEEP_WAKE_CAPABILITY     7807

#define MEDIA_RC6_QUESTION_ID                     5574

#define PLATFORM_VTIO_SETUP_DATA_ID               5575
#define PCH_HDAUDIO_MIC_PRIVACY_FORM_ID           5577
#define PCH_HDAUDIO_DSP_FEATURES_FORM_ACTION_KEY  5579
#define PCH_HDAUDIO_DBT_OFFLOAD_ACTION_KEY        5580
#define PCH_HDAUDIO_BT_SIDEBAND_ACTION_KEY        5581

#define OC_SETUP_VAR_ID                           5578
#define I3C0_AUTO_ID                              5800
#define I3C2_AUTO_ID                              5801
#ifndef AUTO_ID
#define AUTO_ID(x) x
#endif
