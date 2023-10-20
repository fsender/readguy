/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_driver_config.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 基础配置文件. 用户可以根据自己对库的编译需求来修改此文件.
 * 如果你希望在程序代码内包含引脚定义, 请参考此文件下方的注释来确定并编译
 * !!!但是这样编译得到的程序不具备跨平台特性.!!!
 * 可以禁用WiFi功能来减少ESP32的flash消耗
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
#ifndef _READGUY_DRIVER_CONFIG_H_FILE
#define _READGUY_DRIVER_CONFIG_H_FILE

// *** Do not include any file here!

/*
#define _EPD_CS_PIN 15 //D8, 显示屏的片选引脚
#define _EPD_DC_PIN 5 //D1, 显示屏的DC引脚
#define _EPD_RST_PIN -1 //reserved 保留的重置引脚
#define _EPD_BUSY_PIN 4 //D2 显示屏的Busy引脚
#define _SD_CS_PIN 0 //D3, SD卡的片选引脚
#define _BtnL 5 //D1, 左键
#define _BtnM 12 //中间键 D6引脚
#define _BtnR 2 //D4 右键

//对于甘草酸不酸的新版本板子:
//busy 4 rst 2 dc 0 cs 15 sck / mosi / sdcs 5 btnL rx(3) btnM 0

//对于四个按钮的4.2屏开发板:
//clk 14 cs 15 mosi 13 dc 27 rst 26 busy 25 
//sd cs 4 miso 12 (共线) 按键 k1 5 k2 17 k3 16 k4 0 前置光 19/18/23

//对于lilygo t-watch epaper 开发板:
//cs 5 dc 19 rst 27 busy 38 clk 18 mosi 23
//SD卡 cs 13 miso 2 mosi 15 sck 14 按键1 36 电容 32(高电平使能) blk 12

*/

//                                  ------------------ definations - 定义 -

/// @brief 使用静态的数据 !!!注意:注释此选项编写的程序是不支持跨平台运行的!!!
/// @note 相比于禁用WiFi配网功能, 禁用此功能减少的flash并不多, 为保证程序可在不同屏幕上运行, 请不要注释此选项
#define DYNAMIC_PIN_SETTINGS

/// @brief 启用WIFI配网功能.必须先启用 #define DYNAMIC_PIN_SETTINGS. 此选项对 ESP32xx 会减少大量flash.
#define READGUY_ENABLE_WIFI

/// @brief 启用I2C功能. 可用于联网时钟, 温度计, 陀螺仪等外设. 目前暂不支持库内使用类似函数. 仅可以提供引脚定义
#define READGUY_ENABLE_I2C

/// @brief 使用LittleFS作为片上文件系统, 注释此行则用SPIFFS(功能少, 不好用)
#define READGUY_USE_LITTLEFS 1

/// @brief ESP32按键服务任务的栈空间大小, 不建议普通用户更改. 默认值1024字节. 小于此大小会使程序栈溢出.
#define BTN_LOOPTASK_STACK 1024

/// @brief ESP32按键服务任务运行在哪个核心. 通常运行在核心0上.
#define BTN_LOOPTASK_CORE_ID 0

/// @brief ESP32按键服务任务的优先级. 高于arduino默认任务即可
#define BTN_LOOPTASK_PRIORITY 3

/// @brief 按键任务间隔多久调用一次, 默认为20毫秒. 对于需要精确读取按钮的程序, 请酌情减小此数值
#define BTN_LOOPTASK_DELAY 20

/// @brief 使用ESP8266时, 屏幕和SD卡的SPI速率. 默认为20MHz.
#define ESP8266_SPI_FREQUENCY 20000000

/// @brief 使用ESP32且屏幕与SD卡用不同的SPI总线时, 驱动屏幕的SPI速率
#define ESP32_DISP_FREQUENCY 20000000

/// @brief ESP32驱动SD卡的速率. 当ESP32在与SD卡共享SPI时, 屏幕依据此处的速率.
#define ESP32_SD_SPI_FREQUENCY 20000000

/// @brief debug专用, 请保持处于注释状态. 正常开机从NVS读取引脚配置数据, 取消注释则每次开机需要重新配置
//#define INDEV_DEBUG 1

#ifndef DYNAMIC_PIN_SETTINGS
#ifdef ESP8266
#define READGUY_shareSpi  1 //EPD和SD卡是否共享SPI,对于ESP8266必须设置为1
#define READGUY_epd_type MEPD_DEBUG_DISPLAY // 对应的epd驱动程序代号, -1为未指定.
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
#define READGUY_epd_mosi -1 // 目标显示器的 MOSI 引脚, ESP8266必须设置为-1
#define READGUY_epd_sclk -1 // 目标显示器的 SCLK 引脚, ESP8266必须设置为-1
#define READGUY_epd_cs   15 // 目标显示器的 CS   引脚
#define READGUY_epd_dc    5 // 目标显示器的 DC   引脚
#define READGUY_epd_rst  -1 // 目标显示器的 RST  引脚,不用则设为-1.
#define READGUY_epd_busy -1 // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
#define READGUY_sd_miso  -1 // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_mosi  -1 // 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_sclk  -1 // 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_cs     0 // 目标sd卡的 CS   引脚
#define READGUY_i2c_sda  -1 // 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
#define READGUY_i2c_scl  -1 // 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
#define READGUY_btn1    ( 5+1) //按键1,注意需要+1,这里示例已经加了 设置为负的来允许高电平触发
#define READGUY_btn2    (12+1) //按键2,注意需要+1
#define READGUY_btn3    ( 2+1) //按键3,注意需要+1 
#define READGUY_bl_pin    4 //前置光接口引脚IO

#else //对于非ESP8266
#define READGUY_shareSpi  0 //EPD和SD卡是否共享SPI, 此处不共享
#define READGUY_epd_type READGUY_DEV_154B // 对应的epd驱动程序代号, -1为未指定

    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
#define READGUY_epd_mosi 23 // 目标显示器的 MOSI 引脚
#define READGUY_epd_sclk 18 // 目标显示器的 SCLK 引脚
#define READGUY_epd_cs    5 // 目标显示器的 CS   引脚
#define READGUY_epd_dc   19 // 目标显示器的 DC   引脚
#define READGUY_epd_rst  27 // 目标显示器的 RST  引脚,不用则设为-1.
#define READGUY_epd_busy 38 // 目标显示器的 BUSY 引脚

    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
#define READGUY_sd_miso   2 // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_mosi  15 // 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_sclk  14 // 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_cs    13 // 目标sd卡的 CS   引脚
#define READGUY_i2c_sda  -1 // 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
#define READGUY_i2c_scl  -1 // 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效

    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
#define READGUY_btn1    (36+1) //按键1,注意需要+1,这里示例已经加了
#define READGUY_btn2   -(32+1) //按键2,注意需要+1,此处前面的负号表示允许高电平触发
#define READGUY_btn3      0    //按键3,注意需要+1, 不用的按钮应当设置为0
#define READGUY_bl_pin   12 //前置光接口引脚IO
#endif

#define READGUY_rtc_type 0  //使用的RTC型号(待定, 还没用上)
#elif defined(READGUY_ENABLE_WIFI)
#define READGUY_ESP_ENABLE_WIFI //使用WIFI进行配网等功能
#endif
#undef READGUY_ENABLE_WIFI

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */