/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file readguy.h
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 基础功能 头文件. 
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

#ifndef _GUY_DRIVER_H_FILE
#define _GUY_DRIVER_H_FILE

//                                           ------------------------- includings - 包含 ----<<<<

#include <Arduino.h>
#include <SPI.h>
#include <FS.h>

//#include <esp-fs-webserver.h>
#define LGFX_USE_V1
#include <LovyanGFX.hpp>

#include "guy_epaper/guy_epdbase.h"
#if (defined(READGUY_DEV_154A) || defined(READGUY_DEV_290A))
#include "guy_epaper/guy_154a_290a/guy_154a_290a.h"
#endif
#if (defined(READGUY_DEV_154B) || defined(READGUY_DEV_270B) || defined(READGUY_DEV_290B))
#include "guy_epaper/guy_154b_270b_290b/guy_154b_270b_290b.h"
#endif
#ifdef READGUY_DEV_213A
#include "guy_epaper/guy_213a/guy_213a.h"
#endif
#if (defined(READGUY_DEV_213B) || defined(READGUY_DEV_266A))
#include "guy_epaper/guy_213b_266a/guy_213b_266a.h"
#endif
#ifdef READGUY_DEV_370A
#include "guy_epaper/guy_370a/guy_370a.h"
#endif
#ifdef READGUY_DEV_420A
#include "guy_epaper/guy_420a/guy_420a.h"
#endif
#ifdef READGUY_DEV_420B
#include "guy_epaper/guy_420b/guy_420b.h"
#endif
#ifdef MEPD_DEBUG_DISPLAY
#include "guy_epaper/lcdDebug/lcdDebug.h"
#endif

#include "guy_button.h" //改自Button2精简而来
#include "guy_version.h"
#include "guy_driver_config.h" //config
#ifdef READGUY_USE_LITTLEFS
#include <LittleFS.h>
#else
#ifndef ESP8266
#include <SPIFFS.h>
#endif
#endif

#if defined(ESP8266) //for ESP8266
#ifdef DYNAMIC_PIN_SETTINGS
#include <EEPROM.h> //ESP32需要NVS才可以读取引脚信息
#endif
#ifdef READGUY_ESP_ENABLE_WIFI
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "ESP8266HTTPUpdateServer.h"
#endif
#include <SDFS.h>
#include <Ticker.h>
#else //for ESP32
#ifdef DYNAMIC_PIN_SETTINGS
#include <Preferences.h> //ESP32需要NVS才可以读取引脚信息
#endif
#ifdef READGUY_ESP_ENABLE_WIFI
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include "HTTPUpdateServer.h"
#endif
#include <SD.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#endif

#ifdef DYNAMIC_PIN_SETTINGS
#define READGUY_cali     (config_data[0])
#define READGUY_shareSpi (config_data[1])
#define READGUY_epd_type (config_data[2]) // 对应的epd驱动程序代号, -1为未指定
    //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
#define READGUY_epd_mosi (config_data[3]) // 目标显示器的 MOSI 引脚
#define READGUY_epd_sclk (config_data[4]) // 目标显示器的 SCLK 引脚
#define READGUY_epd_cs   (config_data[5]) // 目标显示器的 CS   引脚
#define READGUY_epd_dc   (config_data[6]) // 目标显示器的 DC   引脚
#define READGUY_epd_rst  (config_data[7]) // 目标显示器的 RST  引脚
#define READGUY_epd_busy (config_data[8]) // 目标显示器的 BUSY 引脚
    //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
#define READGUY_sd_miso  (config_data[9]) // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_mosi  (config_data[10])// 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_sclk  (config_data[11])// 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_cs    (config_data[12])// 目标sd卡的 CS   引脚.
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
#define READGUY_btn1     (config_data[13]) 
#define READGUY_btn2     (config_data[14]) 
#define READGUY_btn3     (config_data[15]) 
#define READGUY_bl_pin   (config_data[16])//前置光接口引脚IO
#define READGUY_rtc_type (config_data[17])//使用的RTC型号(待定, 还没用上)
#define READGUY_sd_ok    (config_data[18]) //SD卡已经成功初始化
#define READGUY_buttons  (config_data[19]) //按钮个数, 0-3都有可能
#endif

class readguy_driver: public LGFX_Sprite{ // readguy 基础类
  public:
    readguy_driver();
    /** @brief 初始化readguy
     *  @return SD卡是否就绪
     */
    uint8_t init();
    //LGFX_Sprite g() const { return gfx; }; //使用 gfx 绘制内容.
    /// @brief 设置显示亮度
    void setBright(int d);
    /// @brief 返回显示亮度
    int getBright() const { return currentBright; }
    /// @brief 刷新显示到屏幕上
    void display(bool part = true);
    /** @brief 刷新显示到屏幕上, 可以自定义读取指定位置像素的函数
     *  @param f 自定义的函数. 此函数将在读取像素并输出到墨水屏时被调用.
     *  每次调用需要返回 "参数对应位置" 的8个像素的颜色信息(凑成一字节). 其中左侧应在高位,右侧应在低位.
     *  例如, 调用f(0)将会返回屏幕最左上角的一横排的8个像素的颜色值.
     *  
     *  * 可以用下述方法来将此 "参数对应位置" 还原为 X和Y坐标. 返回的X,Y坐标位置所在像素点以及其后面的
     *  7个像素点共同组成了需要返回位置的8个像素点. 将这8个像素点拼合为一字节之后返回该函数即可.
     *  @code C++
     *  int w = ( ( drv_width() + 7 ) >> 3 ); //其中 k 为此自定义函数的参数, w为临时变量
     *  int x = ( ( k % w ) << 3 );           // x 为具体像素的横坐标
     *  int y = k / w;                        // y 为具体像素的纵坐标
     *  @endcode
     *  该函数会将参数从0开始,每次逐渐增加1的顺序来被调用. 即先调用f(0),再f(1),f(2),f(3)... 以此类推.
     */
    void display(std::function<uint8_t(int)> f, bool part = true);
    /// @brief 显示图片, 使用抖动算法. 可以用省内存的方法显示
    void drawImage(LGFX_Sprite &spr,uint16_t x,uint16_t y);
    /// @brief 设置显示对比度(灰度)
    void setDepth(uint8_t d);
    /** @brief 返回目标屏幕是否支持16级灰度 返回非0代表支持.
     *  @note 返回负整数则代表调用draw16greyStep需要从深色到浅色刷新, 而不是从浅色到深色刷新 */
    int supportGreyscaling() const{return READGUY_cali==127?guy_dev->drv_supportGreyscaling():0;}
    /// @brief 设置灰度的渲染画质. 高画质模式在某些屏幕某些情况下可能表现不好.
    void setGreyQuality(bool q) { if(READGUY_cali==127) guy_dev->setGreyQuality(q); }
    /// @brief 显示灰度图片,如果支持,否则就不显示灰度图片了. 可以用省内存的方法显示
    void draw16grey(LGFX_Sprite &spr,uint16_t x,uint16_t y);
    /** @brief 按照自定义分步显示灰度图片,如果支持,否则就不显示灰度图片了. 可以用省内存的方法显示
     *  @param step 步骤代号. 从1开始到15,依次调用此函数来自定义的灰度显示显存内容. 没有16.
     *  @note 必须按照 "慢刷全屏->绘图->设置参数1->绘图->设置参数2... 调用15次 来完成一次自定义灰度刷屏
     *  连续调用多次此函数之间, 可以修改显存内的像素颜色, 但只能从白色改为黑色.
     *  @attention 需要先调用 supportGreyscaling() 来确定是否支持灰度分步刷新.为负数时需要从深到浅刷新
     */
    void draw16greyStep(int step);
    /** @brief 分步刷新显示灰度, 详见 display(f,part) 和 draw16grey(spr,x,y) 的注释.
     *  @note 此函数不读取显存,而是通过调用该函数来确定像素颜色.                                   */
    void draw16greyStep(std::function<uint8_t(int)> f, int step);
    /// @brief 对缓冲区内所有像素进行反色.只对现在的缓冲区有效,之后的颜色该怎样就怎样. 灰度信息会被扔掉.
    void invertDisplay();
    /// @brief 进入EPD的低功耗模式
    void sleepEPD(void);
    /// @brief ap配网设置页面
    typedef struct {
      String event;
      int method; //其实这里的method的类型应当为HTTPMethod, 但是为了兼容无wifi的功能, 使用了int格式
      std::function<void()> func;
    } serveFunc;
#ifdef READGUY_ESP_ENABLE_WIFI
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void ap_setup();
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void server_setup(const serveFunc *serveFuncs = nullptr, int funcs = 0);
    bool server_loop();
    void server_end() { sv.stop(); MDNS.end(); }
#else
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void ap_setup(){}
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void server_setup(const serveFunc *serveFuncs = nullptr, int funcs = 0){}
    bool server_loop(){ return true; }
    void server_end(){}
#endif
    /// @brief 检查初始化屏幕硬件, 若检查失败返回0,否则返回硬件代号
    uint8_t checkEpdDriver();
    /// @brief 初始化屏幕, 设置驱动代号, 引脚排列顺序. 过程会检验引脚可用性.
    void setEpdDriver();
    /** @brief 初始化SD卡, 设置驱动代号, 引脚排列顺序. 过程会检验引脚可用性.
     *  @return SD卡初始化成功与否 */
    bool setSDcardDriver();
    /// @brief 初始化按钮, 背光, 设置驱动代号, 引脚排列顺序
    void setButtonDriver();
    /** @brief 检查SD卡是否插入
     *  @param check 为true时, 如果SD卡不可用则初始化SD卡. 为false时, 当SD卡不可用则返回LittleFS. */
    bool SDinside(bool check=true) { return check?setSDcardDriver():READGUY_sd_ok; };
    /// @brief 检查按钮. 当配置未完成时,按钮不可用, 返回0.
    uint8_t getBtn() { return (READGUY_cali==127)?getBtn_impl():0; }
    /** @brief 返回可用的文件系统. 当SD卡可用时, 返回SD卡. 否则根据情况返回最近的可用文件系统
     *  @param initSD 2:总是重新初始化SD卡; 1:若SD卡不可用则初始化; 0:SD卡不可用则返回LittleFS. */
    fs::FS &guyFS(uint8_t initSD = 0);

    //friend class EpdIf; //这样EpdIf就可以随意操作这个类的成员了
  private:
    //以下是支持的所有屏幕型号 Add devices here!
    //添加屏幕驱动范例: 直接添加对应屏幕的类就可以用了
    static const char projname[8];
    static const char tagname[9];
    //数据是否已经校准
#ifdef DYNAMIC_PIN_SETTINGS
    int8_t config_data[20];
    char randomch[4]; //校验用字符串
    void nvs_init();  //初始化持久存储器.
    void nvs_deinit();//保存持久存储器的内容
    bool nvs_read();  //从持久存储器读取, 返回是否读取成功
    void nvs_write(); //写入到持久存储器
#else
    int8_t READGUY_sd_ok = 0;
    int8_t READGUY_cali = 0;
    int8_t READGUY_buttons = 0;  //按钮个数, 0-3都有可能
#endif
    int epd_OK=0; //墨水屏可用
    int currentBright = -3; //初始亮度
    //.........敬请期待更多屏幕ic...........
    
    //LGFX_Sprite gfx; // 图形引擎类指针, 可以用这个指针去操作屏幕缓冲区
    readguyEpdBase *guy_dev = nullptr;

    //内部调用函数: 初始化epd
    //template <class T> T t_init(T t, bool initial = true);
    //template <class T> void t_display(T t);

#if defined(ESP8266)
#ifdef READGUY_ESP_ENABLE_WIFI
    ESP8266WebServer sv;
    ESP8266HTTPUpdateServer httpUpdater;
    String w_ssid;
    String w_psk;
#endif
  //对于esp8266, 需要注册到ticker
    Ticker btnTask;
#else
#ifdef READGUY_ESP_ENABLE_WIFI
    WebServer sv;
    HTTPUpdateServer httpUpdater;
#endif
#ifdef DYNAMIC_PIN_SETTINGS
    //NVS数据操作函数, 无NVS的使用EEProm的最后几个字节块
    Preferences nvsData;
#endif
    static SPIClass *sd_spi;
    static SPIClass *epd_spi;
    static TaskHandle_t btn_handle;
#endif
#ifdef READGUY_ESP_ENABLE_WIFI
    void handleInit();      //服务器初始化系统(初次访问时, 跳转至引脚设定函数)
    void handleInitPost();  //服务器响应初始化请求
    void handlePinSetup();  //服务器-引脚设定函数
    void handleFinal();     //服务器-校验屏幕是否正常
    void handleFinalPost(); //服务器-校验屏幕是否正常回调函数
    void handleWiFiPost();  //服务器-处理WiFi连接相关内容和API接口密钥功能
    void handleNotFound();  //服务器-404响应
#endif
    //按键驱动部分
    static guy_button btn_rd[3];
    /// @brief 复用输出引脚1: 适用于按键引脚与屏幕DC引脚复用的情形
    /// @note 只能解决屏幕DC引脚复用的情况, 其他引脚最好不要复用, 复用了我也解决不了
    static int8_t pin_cmx;
    static volatile uint8_t spibz;
  private:
#ifdef READGUY_ESP_ENABLE_WIFI
    //static constexpr size_t EPD_DRIVERS_NUM_MAX = READGUY_SUPPORT_DEVICES;
    static const char *epd_drivers_list[EPD_DRIVERS_NUM_MAX];
    static const PROGMEM char index_cn_html[];
    /*static const PROGMEM char index_cn_html1[];
    static const PROGMEM char index_cn_html2[];
    static const PROGMEM char index_cn_html3[];
    static const PROGMEM char index_cn_html4[];
    static const PROGMEM char index_cn_html5[];
    static const PROGMEM char index_cn_html6[];
    static const PROGMEM char index_cn_html7[];
    static const PROGMEM char index_cn_html8[];
    static const PROGMEM char index_cn_html9[];
    static const PROGMEM char index_cn_html10[];
    static const PROGMEM char index_cn_html11[];
    static const PROGMEM char index_cn_html12[];
    static const PROGMEM char index_cn_html13[];
    static const PROGMEM char index_cn_html14[];
    static const PROGMEM char index_cn_html15[]; */
    static const PROGMEM char index_cn_html16[];
    static const PROGMEM char verify_html[];
    static const PROGMEM char verify2_html[];
    static const PROGMEM char verifybtn_html[3][200];
    static const PROGMEM char final_html[];
    static const PROGMEM char final2_html[];
    static const PROGMEM char end_html[];
    //static const PROGMEM uint8_t faviconData[1150];
#endif
    static void looptask(); //按键服务函数
    static uint8_t rd_btn_f(uint8_t btn);
    uint8_t getBtn_impl(); //按钮不可用, 返回0.
    static void in_press(){ //SPI开始传输屏幕数据
#ifndef ESP8266
      if(!spibz) epd_spi->beginTransaction(SPISettings(ESP32_DISP_FREQUENCY, MSBFIRST, SPI_MODE0));
#endif
      spibz ++; 
    }
    static void in_release(){//SPI结束传输屏幕数据
      spibz --;
#ifndef ESP8266
      if(!spibz) epd_spi->endTransaction();
#endif
    }
};
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */