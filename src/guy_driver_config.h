/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_driver_config.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 基础配置文件. 用户可以根据自己对库的编译需求来修改此文件.
 * 
 * 1.5.0 新增!
 * 在项目目录里拷贝本文件并重命名为 "readguy_user_config.h" 即可使本配置文件仅在当前项目有效
 * 
 * 如果你希望在程序代码内包含引脚定义, 请参考此文件下方的注释来确定并编译
 * 这样就可以只针对你设计的这一个硬件来设定引脚功能.
 * 关于屏幕配置, 请参考最下方(284行到文件末尾)来配置到底哪些屏幕型号的屏幕会被包含进来

 * 可以禁用WiFi功能来减少程序的flash消耗

 * @date create: 2023-09-21
 * last modify:  2025/4/7

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

/**************************** 请不要取消注释这里! *****************************
 ********* 这里的设置项是无效的, 仅仅用于标注一些开发板的引脚预定义数据. *********
 ********** 部分热门的, 创新的带墨水屏开发板的引脚定义可能会罗列在这里. **********
 *****************************************************************************
(无效设置项) #define _EPD_CS_PIN 15 //D8, 显示屏的片选引脚
(无效设置项) #define _EPD_DC_PIN 5 //D1, 显示屏的DC引脚
(无效设置项) #define _EPD_RST_PIN -1 //reserved 保留的重置引脚
(无效设置项) #define _EPD_BUSY_PIN 4 //D2 显示屏的Busy引脚
(无效设置项) #define _SD_CS_PIN 0 //D3, SD卡的片选引脚
(无效设置项) #define _BtnL 5 //D1, 左键
(无效设置项) #define _BtnM 12 //中间键 D6引脚
(无效设置项) #define _BtnR 2 //D4 右键

//对于甘草酸不酸的新版本板子:
//busy 4 rst 2 dc 0 cs 15 sck / mosi / sdcs 5 btnL rx(3) btnM 0

//对于四个按钮的4.2屏开发板: @十里画廊 
//(https://oshwhub.com/shilihualang/han-jia-qian-gai-zhuang-ji-jian-kai-fa-ban)
//clk 14 cs 15 mosi 13 dc 27 rst 26 busy 25 
//sd cs 4 miso 12 (共线) 按键 k1 5 k2 17 k3 16 k4 0 前置光 19/18/23 (RGB)

//对于lilygo t-watch epaper 开发板:
//cs 5 dc 19 rst 27 busy 38 clk 18 mosi 23
//SD卡 cs 13 miso 2 mosi 15 sck 14 按键1 36 电容 32(高电平使能) blk 12

*/

//                                  ------------------ definations - 定义 -

/** @brief 使用静态的数据 !!!注意:注释此选项编写的程序是不支持跨平台运行的!!!
/   @note 相比于禁用WiFi配网功能, 禁用此功能减少的flash并不多, 为保证程序可在不同屏幕上运行, 请不要注释此选项
/         关闭此选项自动禁用wifi功能. 如需wifi功能需要自己在程序里加. 但也可以大幅减少flash的占用 */
#define DYNAMIC_PIN_SETTINGS

/// @brief NVS存储配置数据时用的项目名. 必须小于8个字符, 为节省内存, 项目依赖此处的内存块仅分配了8字节.
#define READGUY_NVS_PROJECTNAME "readguy"

/// @brief NVS存储配置数据时用的配置数据键名. 必须小于8个字符, 为节省内存, 项目依赖此处的内存块仅分配了8字节.
#define READGUY_NVS_CONFIGKEY   "hwconf"

/// @brief 当使用自定义驱动 (customDriver) 时会通过NVS访问驱动代码. 必须小于8个字符.
/// @note  有关自定义驱动, 可以参考 readme 中提到的在 2.0 版本即将到来的特性.
#define READGUY_NVS_DRIVERKEY   "driver"

/// @brief 启用WIFI配网功能.必须先启用 #define DYNAMIC_PIN_SETTINGS. 此选项对 ESP32xx 会减少大量可用flash.
/// @note  注释掉它会破坏跨平台特性
#define READGUY_ENABLE_WIFI

/// @brief AP配网热点的名称和密码. 不可注释
#define READGUY_CONF_AP_SSID "readguy"
#define READGUY_CONF_AP_PASS "12345678"

/** @brief 使用服务器或者WIFI配网时, 在网页上显示项目默认的图标, 默认图标会额外占用1150字节的flash空间
/          如果想用自己的图标数据库, 请参考配网示例, 在配置时候加入下方的代码. 此处提供了代码.
    ``` C++
    typedef ReadguyDriver::ReadguyWebServer* server_t; //类型名太长太繁琐, 使用typedef减短
    typedef ReadguyDriver::serveFunc         event_t ; //存储一个WiFi功能事件.
    const uint8_t icon[] = {}; //你的图标数据. 可直接把 *.ico 格式图标文件转化为字节数组, 拷贝到此处
    void sendIcon(server_t sv){ sv->send_P(200, "image/x-icon", (const char *)icon,sizeof(icon)); }
    event_t server_event={"","/favicon",sendIcon};
    guy.server_setup(String("Custom Icon"),&server_event,1); //初始化服务器.
    ``` */
//#define READGUY_USE_DEFAULT_ICON

/** @brief 使用MDNS域名服务. 在设备联网或者打开热点之后, 可以通过域名 readguy.local 来访问配网主页 
/          关闭本功能可节省少许flash. 如需自定义服务名称, 则您可更改此处的字符串, 访问 <自定义名称>.local */
//#define READGUY_MDNS_SERVICE "readguy"

/// @brief 使用固件更新服务. 关闭本功能可节省少许flash. 如需自定义链接名称, 则您可更改此处的字符串.
//#define READGUY_UPDATE_SERVER "固件更新"

/** @brief 启用I2C功能. 可用于联网时钟, 温度计, 陀螺仪等外设. 目前暂不支持库内使用类似函数. 仅可以提供引脚定义
/   @note 现在库提供了获取已存的I2C引脚的接口, 使用时请使用 getI2cSda() 和 getI2cScl() 函数获取I2C的引脚.
/       本库内不提供任何I2C驱动, 只提供引脚定义的存储和读取, 这几乎不增加多少代码. 因此本宏不再使用 */
//#define READGUY_ENABLE_I2C //已经弃用

/** @brief 启用SD卡功能. 开启此功能将会使用内置SD卡管理功能. 关闭后仅可保存SD卡用到的引脚.
/   @note 会破坏兼容性. 若没有启用通用的SD卡驱动程序, 那么那些跨屏台编译的程序将无法用guyFS读取到SD卡.
/   若用户程序希望能从外部加载SD卡, 可以使用getSdMiso()等函数获取SD卡的Miso等引脚, 再由用户程序初始化SD卡. */
#define READGUY_ENABLE_SD

/// @brief 使用LittleFS作为片上文件系统, 注释此行则用SPIFFS(功能少, 不好用)
#define READGUY_USE_LITTLEFS 1

/** @brief 使用esp8266时, EEPROM(类似NVS)的存储位置起点 (从起点开始的40字节被readguy所使用) 可选值: 0~4045
/   @note 对于单一项目来说, 此选项不建议更改, 请在项目初期就确定此变量的值. */
#define READGUY_ESP8266_EEPROM_OFFSET 2

/** @brief (实验性) 允许 EPD_DC 引脚复用作按键引脚. 当DC引脚直连主控且直连按键时 (按下按键后DC引脚电平被锁定)
/   启用此项可以允许这样的按键复用 (如用于早期版本甘草酸不酸2.9阅读器板子) 但复用的引脚作为按钮长按时体验会变差
/   @note 即使注释此选项依旧可以允许DC引脚复用, 但需要在主控直连屏幕DC引脚, 并在按键一端和主控引脚之间接入一个
/   220Ω~1kΩ的电阻 (推荐470Ω), 按键另一端接地, 这样才可以. 注释该选项后, 即使按键和DC引脚复用了, 也不会影响功
/   能和长按体验. 注意如果按键有内阻(不按时按键两端电阻为∞, 按下后按键电阻较高) 可以酌情减小一些按键接入电阻.
/   对于新版的甘草酸不酸2.9阅读器板子就可以注释此选项. 
/   没有需要兼容「复用DC引脚的硬件」的需求的用户, 建议注释此项. 一般仅建议ESP8266启用此选项*/
//#define READGUY_ALLOW_DC_AS_BUTTON

/** @brief (实验性) 允许复用 EPD 的 CS 引脚作为按钮. 这会让读取按钮电平的速度些许减慢.
/   @note 需要在按键一端和主控引脚之间接入一个220Ω~1kΩ的电阻 (推荐470Ω), 并要求EPD的CS引脚直连主控对应引脚. */
//#define READGUY_ALLOW_EPDCS_AS_BUTTON

/** @brief (不建议, 实验性) 允许复用 SD 卡的 CS 引脚作为按钮.
/   @details 这不仅会减慢读按钮速度, 而且十分不建议对「确定」按钮 (通常为按钮2) 启用此功能函数. 因为一些程序会
/   在你按下「确定」后立即读取 SD 卡 (与按键循环读取功能冲突) 导致总线紊乱! 在ESP32上会引起无法预知的崩溃复位!
/   @attention 启用后每次读写SD卡时,都必须调用 setSDbusy(1) 来锁定引脚, 屏蔽按键读取, 调用 setSDbusy(0)解锁.
/   @note 需要在按键一端和主控引脚之间接入一个220Ω~1kΩ的电阻 (推荐470Ω), 并要求SD卡的CS引脚直连主控对应引脚. 
/   所有示例程序未针对此选项重写, 建议重新规划硬件再使用示例. 若硬件无法更改, 请手动对需要用 ReadGuy 库未提供的
/   SD卡IO操作的部分, 手动添加 setSDbusy() 函数. 由 ReadGuy 提供的函数已适配好了, 如ReadGuy::Screenshot() */
//#define READGUY_ALLOW_SDCS_AS_BUTTON

/// @brief 按键去抖时间
#define READGUY_DEFAULT_MIN_DEBOUNCE_MS 25

/// @brief 按键长按持续时间
#define READGUY_LONG_PRESS_MS 300

/// @brief 按键双击识别间隔最大时间
#define READGUY_DOUBLE_PRESS_MS 300

/// @brief 按键长按连按间隔时间
#define READGUY_LONG_REPEAT_MS 200

/// @brief ESP32按键服务任务的栈空间大小, 不建议普通用户更改. 默认值1024字节. 小于此大小会使程序栈溢出.
#ifdef CONFIG_IDF_TARGET_ESP32S3
#define BTN_LOOPTASK_STACK 1536
#else
#define BTN_LOOPTASK_STACK 1024
#endif

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
#define ESP32_SD_MMC_FREQUENCY 40000

/// @brief debug专用, 请保持处于注释状态. 正常开机从NVS读取引脚配置数据, 取消注释则每次开机需要重新配置
//#define READGUY_INDEV_DEBUG 1

/// @brief 串口显示刷屏功能等的信息. 如果是对flash大小要求十分敏感, 或者希望减少串口数据, 可以关闭
#define READGUY_SERIAL_DEBUG

/// @brief 对于单缓存的5.83屏幕,启用双缓存支持. 相当不建议注释掉,否则不能刷白色
#define READGUY_583A_DUAL_BUFFER

/// @brief 取消此行注释来监视SPI的通信数据 (用于debug), 可以查看主控和屏幕的通信数据
//#define MEPD_DEBUG_WAVE 16

/// @brief 显示墨水屏的刷新时间, 单位是毫秒
//#define MEPD_DEBUG_WAITTIME

/// @brief 默认使用更好的floyd steinberg抖动算法,取消注释则用bayer图案抖动算法
#define FLOYD_STEINBERG_DITHERING

/// @brief 使用更好的floyd steinberg抖动算法显示16阶灰度,取消注释则使用阈值填充
#define FLOYD_DITHERING_16GREY

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
#define READGUY_btn1    ( 5+1) //按键1,注意需要+1,这里示例已经加了 设置为负的来允许高电平触发 如 (-(5+1))
#define READGUY_btn2    (12+1) //按键2,注意需要+1
#define READGUY_btn3    ( 2+1) //按键3,注意需要+1 
#define READGUY_bl_pin    4 //前置光接口引脚IO

#define READGUY_user1  -1 //useless
#define READGUY_user2  -1 //useless
#define READGUY_user3  -1 //useless
#define READGUY_user4  -1 //useless
#define READGUY_user5  -1 //useless
#define READGUY_user6  -1 //useless
#define READGUY_user7  -1 //useless
#define READGUY_user8  -1 //useless
#define READGUY_user9  -1 //useless
#define READGUY_user10 -1 //useless

#else //对于非ESP8266 (如 ESP32xx)
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

#define READGUY_user1  -1 //useful with ESP32S3 SDMMC sd mode DAT2 pin
#define READGUY_user2  -1 //useful with ESP32S3 SDMMC sd mode DAT3 pin
#define READGUY_user3  -1 //useless
#define READGUY_user4  -1 //useless
#define READGUY_user5  -1 //useless
#define READGUY_user6  -1 //useless
#define READGUY_user7  -1 //useless
#define READGUY_user8  -1 //useless
#define READGUY_user9  -1 //useless
#define READGUY_user10 -1 //useless
#endif

#define READGUY_rtc_type 0  //使用的RTC型号. RTC 将在2.0实装 保留是为了兼容性 让代码更简单维护

// ******************************************************************
// ********************** 以下内容不建议用户更改 **********************
// ******************************************************************
#elif defined(READGUY_ENABLE_WIFI) // READGUY_ENABLE_WIFI 依赖 DYNAMIC_PIN_SETTINGS
#define READGUY_ESP_ENABLE_WIFI //使用WIFI进行配网等功能
#endif
#undef READGUY_ENABLE_WIFI

#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */