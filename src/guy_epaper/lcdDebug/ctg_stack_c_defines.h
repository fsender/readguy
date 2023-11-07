/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ctg_stack_c_defines.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 调试用墨水屏模拟器 驱动库函数 可使用LCD屏幕仿真墨水屏使用或测试
 * @version 1.0
 * @date 2023-09-21

 * @attention
 * Copyright (c) 2022-2023 FriendshipEnder
 * 
 * Apache License, Version 2.0
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/********** FRIENDSHIPENDER **********
 * @note THIS FILE IS FOR ESP8266 OR ESP32-C3-12F ONLY
 * @file ctg_stack_c_defines.cpp
 * @author FriendshipEnder (Q:3253342798)
 * @brief driver for SC3EEN Project
 * @version 1.0
 * @date V1.1 2022-06-17
 *       V1.0 2021-11-21
 * 
 * @copyright FriendshipEnder (c) 2022
 * 
 */
#ifndef _CTG_STACK_C_DEFINES_H
#define _CTG_STACK_C_DEFINES_H

//选择需要的屏幕驱动程序
//墨水屏暂时还不支持使用本库文件驱动(但是可以使用其他的库文件)
//#define DISPLAY_no_CS //屏幕无CS引脚, 请选择此项, 引脚顺序为 GND VCC SCK MOSI RST DC BLK

//#define DISPLAY_TYPE_ST7789_240320 //2.0寸的ST7789 IPS TFT模块
//#define DISPLAY_TYPE_ST7789_240240 //1.3寸或者1.54寸的ST7789 IPS TFT模块
//#define DISPLAY_TYPE_ST7789_135240 //1.14寸的ST7789 模块

//#define DISPLAY_TYPE_ST7735_128160 //1.8寸的ST7735 彩色显示模块
//#define DISPLAY_TYPE_ST7735_128128 //1.44寸的ST7735 彩色显示模块
//#define DISPLAY_TYPE_ST7735_080160 //0.96寸的ST7735 彩色显示模块

//#define DISPLAY_TYPE_SSD1306_MONO_128064 //0.96寸的SSD1306 OLED12864 SPI接口显示模块
//#define DISPLAY_TYPE_SSD1327_128128 //适用于隔壁老王的 1.5寸 16灰阶的 SSD1327 OLED SPI接口

//#define DISPLAY_TYPE_ILI9486_320480 //适用于中景园电子的 3.5寸 ILI9486 8pin LCD SPI接口
//#define DISPLAY_TYPE_ILI9342_320240 //适用于M5Stack

//以下模块 引脚顺序不兼容SC3EEN硬件
//#define DISPLAY_TYPE_SSD1351_128128 //适用于绝版的Adafruit 彩色OLED显示模块
//#define DISPLAY_TYPE_SSD1351_128096 //适用于绝版的Adafruit 彩色OLED显示模块 引脚顺序不兼容SC3EEN硬件


#include <Arduino.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

//#define _DEFINA_SD_CS_PIN 0

// * for NodeMcu ctg stack LCF board
#define WHITE_GAP 2

#ifdef ESP8266
#define DISPLAY_TYPE_ST7789_240320 //2.0寸的ST7789 IPS TFT模块
#define _DEFINA_IPS_CS_PIN 15 //lcdDebug是不支持自定义引脚的
#define _DEFINA_IPS_DC_PIN 5
#define _DEFINA_IPS_RST_PIN -1

#define _DEFINA_IPS_MOSI_PIN -1
#define _DEFINA_IPS_MISO_PIN -1
#define _DEFINA_IPS_SCLK_PIN -1

#else
//for M5Stack Core ESP32
#define DISPLAY_TYPE_ILI9342_320240
#define _DEFINA_IPS_CS_PIN 14
#define _DEFINA_IPS_DC_PIN 27
#define _DEFINA_IPS_RST_PIN 33

#define _DEFINA_IPS_MOSI_PIN 23
#define _DEFINA_IPS_MISO_PIN 19
#define _DEFINA_IPS_SCLK_PIN 18
#endif
//#define _DEFINA_IPS_BL_PIN 4

class LGFX : public lgfx::LGFX_Device
{
#if   (defined(DISPLAY_TYPE_ST7789_240320) || defined(DISPLAY_TYPE_ST7789_240240) || defined(DISPLAY_TYPE_ST7789_135240))
  lgfx::Panel_ST7789     _panel_instance;
#elif (defined(DISPLAY_TYPE_ST7735_128160) || defined(DISPLAY_TYPE_ST7735_080160) || defined(DISPLAY_TYPE_ST7735_128128))
  lgfx::Panel_ST7735S    _panel_instance;
#elif (defined(DISPLAY_TYPE_SSD1351_128128) || defined(DISPLAY_TYPE_SSD1351_128096))
  lgfx::Panel_SSD1351    _panel_instance;
#elif (defined(DISPLAY_TYPE_SSD1306_MONO_128064))
  lgfx::Panel_SSD1306    _panel_instance;
#elif (defined (DISPLAY_TYPE_ILI9486_320480))
  lgfx::Panel_ILI9486    _panel_instance;
#elif (defined (DISPLAY_TYPE_SSD1327_128128))
  lgfx::Panel_SSD1327    _panel_instance;
#else //用户可以自己添加屏幕型号操作
  lgfx::Panel_ILI9342    _panel_instance; //用户可以自己添加屏幕型号操作
#endif
  lgfx::Bus_SPI       _bus_instance;
  //lgfx::Light_PWM     _light_instance;
public:
  LGFX(void){}
  void configLGFX(void){
    { // 设置总线控制。- SPI, I2C, I2S, FSMC, etc.
      auto cfg = _bus_instance.config();    // 获取总线配置的结构。
#ifndef ESP8266
#ifdef CONFIG_IDF_TARGET_ESP32
      cfg.spi_host = SPI3_HOST;     // 选择要使用的 SPI  (VSPI_HOST or HSPI_HOST)
#else
      cfg.spi_host = SPI1_HOST;     // 选择要使用的 SPI  (VSPI_HOST or HSPI_HOST)
#endif
      cfg.use_lock   =  true;        // 使用锁时设置为 True
      cfg.dma_channel = SPI_DMA_CH_AUTO; // Set the DMA channel (1 or 2. 0=disable) 设置要打开的 DMA 通道 (0=DMA关闭)
#endif
      cfg.spi_mode = 0;             // SPI设置通讯模式 (0 ~ 3)
#if   (defined(DISPLAY_TYPE_ST7789_240320) || defined(DISPLAY_TYPE_ST7789_240240) || defined(DISPLAY_TYPE_ST7789_135240))
      cfg.freq_write = 40000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =  8000000;    // 接收时的SPI时钟
#elif (defined(DISPLAY_TYPE_ST7735_128160) || defined(DISPLAY_TYPE_ST7735_080160) || defined(DISPLAY_TYPE_ST7735_128128))
      cfg.freq_write = 27000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =  4000000;    // 接收时的SPI时钟
#elif (defined(DISPLAY_TYPE_SSD1306_MONO_128064))
      cfg.freq_write = 20000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =  2000000;    // 接收时的SPI时钟
#elif (defined (DISPLAY_TYPE_SSD1327_128128))
      cfg.freq_write = 20000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =  2000000;    // 接收时的SPI时钟
#elif (defined (DISPLAY_TYPE_ILI9486_320480))
      cfg.freq_write = 40000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =        0;    // 接收时的SPI时钟
#elif (defined(DISPLAY_TYPE_SSD1351_128128) || defined(DISPLAY_TYPE_SSD1351_128096))
      cfg.freq_write = 16000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =        0;    // 接收时的SPI时钟
#else //用户可以自己添加屏幕型号操作
      cfg.freq_write = 40000000;    // 传输时的SPI时钟（最高80MHz，四舍五入为80MHz除以整数得到的值）
      cfg.freq_read  =  8000000;    // 接收时的SPI时钟
#endif
      cfg.spi_3wire  =  true;        // 使用 MOSI 引脚接收时设置为 True

      cfg.pin_sclk = _DEFINA_IPS_SCLK_PIN;            // 设置 SCLK 引脚号
      cfg.pin_mosi = _DEFINA_IPS_MOSI_PIN;            // 设置 MOSI 引脚号
      cfg.pin_miso = _DEFINA_IPS_MISO_PIN;            // 设置 MISO 引脚号 (-1 = 禁用)
      cfg.pin_dc   = _DEFINA_IPS_DC_PIN;            // 设置 DC 引脚号  (-1 = 禁用)
      _bus_instance.config(cfg);    // 设定值反映在总线上。
      _panel_instance.setBus(&_bus_instance);      // 在面板上设置总线。
    }

    { // 设置显示面板控件。- 显示宽度/高度, 颜色格式, 读写像素规则, etc.
      auto cfg = _panel_instance.config();    // 获取显示面板设置的结构。
#if (defined(DISPLAY_no_CS))
      cfg.pin_cs           =    -1;  // 设置 CS 引脚号   (-1 = 禁用)
#undef _DEFINA_IPS_BL_PIN
#define _DEFINA_IPS_BL_PIN _DEFINA_IPS_CS_PIN
#undef _DEFINA_IPS_CS_PIN
#else
      cfg.pin_cs           =    _DEFINA_IPS_CS_PIN;  // 设置 CS 引脚号   (-1 = 禁用)
#endif
      cfg.pin_rst          =    _DEFINA_IPS_RST_PIN;  // 设置 RST 引脚号  (-1 = 禁用)
      cfg.pin_busy         =    -1;  // 设置 BUSY 引脚号 (-1 = 禁用)



#if defined(DISPLAY_TYPE_ST7789_135240)
      cfg.offset_x         =    52;  // 面板的 X 方向偏移量
#elif (defined(DISPLAY_TYPE_ST7735_128160) || defined(DISPLAY_TYPE_ST7735_128128))
      cfg.offset_x         =     2;  // 面板的 X 方向偏移量
#elif defined(DISPLAY_TYPE_ST7735_080160)
      cfg.offset_x         =     24;  // 面板的 X 方向偏移量
#else 
      cfg.offset_x         =     0;  // 面板的 X 方向偏移量
#endif



#if defined(DISPLAY_TYPE_ST7789_135240)
      cfg.offset_y         =    40;  // 面板的 Y 方向偏移量
#elif (defined(DISPLAY_TYPE_ST7735_128160) || defined(DISPLAY_TYPE_ST7735_128128))
      cfg.offset_y         =     1;  // 面板的 Y 方向偏移量
//#elif defined(DISPLAY_TYPE_SSD1351_128096)
//      cfg.offset_y         =     0;  // 面板的 Y 方向偏移量
#else 
      cfg.offset_y         =     0;  // 面板的 Y 方向偏移量
#endif

#if   defined(DISPLAY_TYPE_ST7789_240320)
      #define HW_SCREEN_WIDTH  240   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 320   // 实际显示高度
#elif defined(DISPLAY_TYPE_ST7789_240240)
      #define HW_SCREEN_WIDTH  240   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 240   // 实际显示高度
#elif defined(DISPLAY_TYPE_ST7789_135240)
      #define HW_SCREEN_WIDTH  135   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 240   // 实际显示高度
#elif defined(DISPLAY_TYPE_ST7735_128160)
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 160   // 实际显示高度
      cfg.memory_width     =   132;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   162;  // 驱动IC支持的最大高度
#elif defined(DISPLAY_TYPE_ST7735_128128)
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 128   // 实际显示高度
      cfg.memory_width     =   132;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   132;  // 驱动IC支持的最大高度
#elif defined(DISPLAY_TYPE_ST7735_080160)
      #define HW_SCREEN_WIDTH   80   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 160   // 实际显示高度
      cfg.memory_width     =   128;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   160;  // 驱动IC支持的最大高度
#elif defined(DISPLAY_TYPE_SSD1351_128128)
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 128   // 实际显示高度
#elif defined(DISPLAY_TYPE_SSD1351_128096)
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT  96   // 实际显示高度
#elif (defined(DISPLAY_TYPE_SSD1306_MONO_128064))
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT  64   // 实际显示高度
      cfg.memory_width     =   128;   // 驱动IC支持的最大宽度
      cfg.memory_height    =    64;  // 驱动IC支持的最大高度
#elif (defined(DISPLAY_TYPE_SSD1327_128128))
      #define HW_SCREEN_WIDTH  128   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 128   // 实际显示高度
      cfg.memory_width     =   128;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   128;  // 驱动IC支持的最大高度
#elif (defined (DISPLAY_TYPE_ILI9486_320480))
      #define HW_SCREEN_WIDTH  320   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 480   // 实际显示高度
      cfg.memory_width     =   320;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   480;  // 驱动IC支持的最大高度
#elif (defined(DISPLAY_TYPE_ILI9342_320240))
      #define HW_SCREEN_WIDTH  320   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 240   // 实际显示高度
      cfg.memory_width     =   320;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   240;  // 驱动IC支持的最大高度
#else //用户可以自己添加屏幕型号操作
      #define HW_SCREEN_WIDTH  240   // 实际显示宽度
      #define HW_SCREEN_HEIGHT 320   // 实际显示高度
      cfg.memory_width     =   240;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   320;  // 驱动IC支持的最大高度
#endif
      //cfg.memory_width   =   cfg.memory_width;   // 驱动IC支持的最大宽度
      //cfg.memory_height  =   cfg.memory_height;  // 驱动IC支持的最大高度
#if   (defined(DISPLAY_TYPE_ST7789_240320) || defined(DISPLAY_TYPE_ST7789_240240) || defined(DISPLAY_TYPE_ST7789_135240))
      cfg.memory_width     =   240;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   320;  // 驱动IC支持的最大高度
#elif (defined(DISPLAY_TYPE_SSD1351_128128) || defined(DISPLAY_TYPE_SSD1351_128096))
      cfg.memory_width     =   128;   // 驱动IC支持的最大宽度
      cfg.memory_height    =   128;  // 驱动IC支持的最大高度
#endif
      cfg.panel_width      = HW_SCREEN_WIDTH;
      cfg.panel_height     = HW_SCREEN_HEIGHT;


#if (defined(DISPLAY_TYPE_SSD1351_128128) || defined(DISPLAY_TYPE_SSD1351_128096))
      cfg.offset_rotation  =     4;  // 设置旋转方向的偏移, 可用的值介于0~7之间（4~7是镜像的）
#elif (defined(DISPLAY_TYPE_SSD1306_MONO_128064))
      cfg.offset_rotation  =     2;  // 设置旋转方向的偏移, 可用的值介于0~7之间（4~7是镜像的）
#else
      cfg.offset_rotation  =     0;  // 设置旋转方向的偏移, 可用的值介于0~7之间（4~7是镜像的）
#endif
      //cfg.dummy_read_pixel =     0;  // 在读取像素之前读取的虚拟位数
      //cfg.dummy_read_bits  =     0;  // 读取像素以外的数据之前的虚拟读取位数
#if (defined(DISPLAY_TYPE_SSD1351_128128) || defined(DISPLAY_TYPE_SSD1351_128096) || defined(DISPLAY_no_CS))
      cfg.readable         = false;  // 如果可以读取数据，则设置为 true
#else
      cfg.readable         =  true;  // 如果可以读取数据，则设置为 true
#endif


#if (defined(DISPLAY_TYPE_ST7789_240320) || defined(DISPLAY_TYPE_ST7789_240240) || defined(DISPLAY_TYPE_ST7789_135240)\
  || defined(DISPLAY_TYPE_ILI9342_320240))
      cfg.invert           =  true;  // 如果面板的明暗反转，则设置为 true
#else
      cfg.invert           = false;  // 如果面板的明暗反转，则设置为 true
#endif

#if defined(DISPLAY_TYPE_ST7735_128160)
      cfg.rgb_order        =  true;  // 如果面板的红色和蓝色被交换，则设置为 true
#else
      cfg.rgb_order        = false;  // 如果面板的红色和蓝色被交换，则设置为 true
#endif
      cfg.dlen_16bit       = false;  // 对于以 16 位单位发送数据长度的面板，设置为 true
      cfg.bus_shared       =  true;  // 如果总线与 SD 卡共享，则设置为 true（使用 drawJpgFile 等执行总线控制）
      _panel_instance.config(cfg);
    }
/*
    { // 设置背光控制。 （不需要的话删掉）
    }
*/
    setPanel(&_panel_instance); // 初始化要使用的面板。
  }
};
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */