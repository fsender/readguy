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
#include <functional>

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
#if (defined(READGUY_DEV_213B) || defined(READGUY_DEV_213B3C) \
  || defined(READGUY_DEV_266A) || defined(READGUY_DEV_266A3C))
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

#ifdef READGUY_DEV_154C
#include "guy_epaper/guy_154C/guy_154C.h"
#endif
#ifdef READGUY_DEV_370B
#include "guy_epaper/guy_370B/guy_370B.h"
#endif
#ifdef READGUY_DEV_426A
#include "guy_epaper/guy_426A/guy_426A.h"
#endif
#ifdef READGUY_DEV_583A
#include "guy_epaper/guy_583A/guy_583A.h"
#endif
#ifdef READGUY_DEV_583B
#include "guy_epaper/guy_583B/guy_583B.h"
#endif
#ifdef READGUY_DEV_750A
#include "guy_epaper/guy_750A/guy_750A.h"
#endif
#ifdef READGUY_DEV_1020A
#include "guy_epaper/guy_1020A/guy_1020A.h"
#endif
  //添加新屏幕型号 add displays here

#include "guy_button.h" //改自Button2精简而来
#include "guy_version.h"
#include "guy_config_host.h" //config
#ifdef READGUY_USE_LITTLEFS
#include <LittleFS.h>
#else
#ifndef ESP8266
#include <SPIFFS.h>
#endif
#endif

#if defined(ESP8266) //for ESP8266
#include <EEPROM.h> //ESP32需要NVS才可以读取引脚信息,
#ifdef READGUY_ESP_ENABLE_WIFI
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#ifdef READGUY_MDNS_SERVICE
#include <ESP8266mDNS.h>
#endif
#ifdef READGUY_UPDATE_SERVER
#include "ESP8266HTTPUpdateServer.h"
#endif
#endif
#ifdef READGUY_ENABLE_SD
#include <SDFS.h>
#endif
#include <Ticker.h>
#else //for ESP32
#include <Preferences.h> //ESP32需要NVS才可以读取引脚信息
#ifdef READGUY_ESP_ENABLE_WIFI
#include <WiFi.h>
#include <WebServer.h>
#ifdef READGUY_MDNS_SERVICE
#include <ESPmDNS.h>
#endif
#ifdef READGUY_UPDATE_SERVER
#include "HTTPUpdateServer.h"
#endif
#endif
#ifdef READGUY_ENABLE_SD
#if (defined(READGUY_IDF_TARGET_MATRIX_SDIO))
#include <SD_MMC.h>
#endif
#include <SD.h>
#endif
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
#define READGUY_sd_miso  (config_data[9]) // 目标sd卡的 MISO 引脚, 或esp32s3使用SDIO的 DAT0 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_mosi  (config_data[10])// 目标sd卡的 MOSI 引脚, 或esp32s3使用SDIO的 CMD 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_sclk  (config_data[11])// 目标sd卡的 SCLK 引脚, 或esp32s3使用SDIO的 CLK 引脚, sd_share_spi == 1 时无效
#define READGUY_sd_cs    (config_data[12])// 目标sd卡的 CS   引脚, 或esp32s3使用SDIO的 DAT3 引脚
#define READGUY_i2c_sda  (config_data[13])// 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
#define READGUY_i2c_scl  (config_data[14])// 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效
    //按键驱动部分, 为负代表高触发, 否则低触发,
    //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
#define READGUY_btn1     (config_data[15]) 
#define READGUY_btn2     (config_data[16]) 
#define READGUY_btn3     (config_data[17]) 
#define READGUY_bl_pin   (config_data[18])//前置光接口引脚IO
#define READGUY_rtc_type (config_data[19])//RTC 将在2.0实装 保留是为了兼容性 让代码更简单维护
#define READGUY_sd_ok    (config_data[20]) //SD卡已经成功初始化
#define READGUY_buttons  (config_data[21]) //按钮个数, 0-3都有可能

#define READGUY_user1    (config_data[22]) //用户自定义变量 同时用于esp32s3使用SDIO卡数据的DAT1 为-1代表不使用SDIO
#define READGUY_user2    (config_data[23]) //用户自定义变量 同时用于esp32s3使用SDIO卡数据的DAT2
#define READGUY_user3    (config_data[24]) //用户自定义变量
#define READGUY_user4    (config_data[25]) //用户自定义变量
#define READGUY_user5    (config_data[26]) //用户自定义变量
#define READGUY_user6    (config_data[27]) //用户自定义变量
#define READGUY_user7    (config_data[28]) //用户自定义变量
#define READGUY_user8    (config_data[29]) //用户自定义变量
#define READGUY_user9    (config_data[30]) //用户自定义变量
#define READGUY_user10   (config_data[31]) //用户自定义变量
#endif

#define READGUY_SLOW       0
#define READGUY_FAST       1
#define READGUY_SLOW_START 2
#define READGUY_FAST_START 3
#define READGUY_SLOW_END   4
#define READGUY_FAST_END   5

  //按键行为     下一个     上一个     确定      返回/退出    特殊操作(如切换输入法)
  //1个按键 返回 1=点按     2=双击    4=长按     8=三击      3=点按后接长按
  //2个按键 返回 1=左键点按 2=左键长按 4=右键点按 8=右键长按  3=按住左键点按右键
  //3个按键 返回 1=右键点按 2=左键点按 4=中键点按 8=中键长按  3=中间按键双击(需手动开启)
#define GUY_BTN_PREV     1 //上一个
#define GUY_BTN_NEXT     2 //下一个
#define GUY_BTN_SPECIAL  3 //确定 选择
#define GUY_BTN_OK       4 //返回/退出
#define GUY_BTN_BACK     8 //特殊操作(如切换输入法)

#if __cplusplus >= 201304L
#define RG_IL_CONEXP constexpr inline
#else
#define RG_IL_CONEXP inline
#endif

class ReadguyDriver: public LGFX_Sprite{ // readguy 基础类
  public:
#ifdef READGUY_ESP_ENABLE_WIFI
#ifdef ESP8266
    typedef ESP8266WebServer ReadguyWebServer;
  #ifdef READGUY_UPDATE_SERVER
    typedef ESP8266HTTPUpdateServer ReadguyUpdateServer;
  #endif
#else
    typedef WebServer ReadguyWebServer;
  #ifdef READGUY_UPDATE_SERVER
    typedef HTTPUpdateServer ReadguyUpdateServer;
  #endif
#endif
#endif
    ReadguyDriver();
    /** @brief 初始化readguy
     *  @param WiFiSet 是否保持AP模式关闭. 0:配网完成自动关WiFi, 1:需要手动调用 WiFi.mode(WIFI_OFF) 关闭WiFi.
     * 2:自动连接到已存的WiFi, 但不等待连接成功
     *  @param initepd 是否初始化墨水屏. 初始化后的首次刷屏必为慢刷. 如果是不断电复位, 可以不初始化墨水屏直接刷屏
     *  @param initSD 是否初始化文件系统. 选是-初始化SD失败则初始化LittleFs; 选否-不初始化SD也不初始化littlefs.
     *  @return SD卡是否就绪
     */
    uint8_t init(uint8_t WiFiSet = 0, bool initepd = 1, bool initSD = 1);
    /// @brief 设置显示亮度
    void setBright(int d);
    /// @brief 返回显示亮度
    int getBright() const { return currentBright; }
    /// @brief 刷新显示到屏幕上
    void display(uint8_t part = READGUY_FAST);
    /// @brief 刷新显示到屏幕上
    void displayBuffer(const uint8_t *buf, uint8_t part);
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
    void display(std::function<uint8_t(int)> f, uint8_t part);
    /// @brief 显示图片, 使用抖动算法. 可以用省内存的方法显示, 可以缩放到指定的宽度和高度
    void drawImage(LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw=0, int32_t zoomh=0){
      if(READGUY_cali==127) drawImage(*this,spr,x,y,zoomw,zoomh);
    }
    /// @brief 显示图片, 将图片(任意颜色格式)显示到一个黑白色的sprite(必须是黑白二值型)上 (未经测试)
    void drawImage(LGFX_Sprite &base,LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw=0,int32_t zoomh=0);
    /// @brief 设置显示对比度(灰度)
    void setDepth(uint8_t d);
    /// @brief 返回上次设置的显示对比度(灰度)
    int16_t getDepth() const {return current_depth;}
    /** @brief 返回目标屏幕是否支持16级灰度 返回非0代表支持.
     *  @note 返回负整数则代表调用draw16greyStep需要从深色到浅色刷新, 而不是从浅色到深色刷新 */
    int supportGreyscaling() const{return READGUY_cali==127?guy_dev->drv_supportGreyscaling():0;}

    /** @brief 设置灰度的渲染画质. 高画质模式在某些屏幕某些情况下可能表现不好.
     *  @param q 0-关闭连续刷屏 开启16阶灰度抖动  1-开启连续刷屏 开启16阶灰度抖动
     *           2-关闭连续刷屏 关闭16阶灰度抖动  3-开启连续刷屏 关闭16阶灰度抖动 */
    void setGreyQuality(uint8_t q) { if(READGUY_cali==127) guy_dev->setGreyQuality(q); }
    /// @brief 显示灰度图片,如果支持,否则就不显示灰度图片了. 可以用省内存的方法显示
    void draw16grey(LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw=0,int32_t zoomh=0);
    /** @brief 按照自定义分步显示灰度图片,如果支持,否则就不显示灰度图片了. 可以用省内存的方法显示
     *  @param step 步骤代号. 从1开始到15,依次调用此函数来自定义的灰度显示显存内容. 没有0和16.
     *  @note 必须按照 "慢刷全屏->绘图->设置参数1->绘图->设置参数2... 调用15次 来完成一次自定义灰度刷屏
     *  连续调用多次此函数之间, 可以修改显存内的像素颜色, 但只能从白色改为黑色.
     *  @attention 先调用 supportGreyscaling() 来确定是否支持灰度分步刷新. 为负时需要从深到浅刷新. 参见示例.
     *  方法较为复杂用法参见示例. */
    void draw16greyStep(int step);
    /** @brief 分步刷新显示灰度, 详见 display(f,part) 和 draw16grey(spr,x,y) 的注释.
     *  @note 此函数不读取显存,而是通过调用该函数来确定像素颜色, 不建议新手使用该函数.  */
    void draw16greyStep(std::function<uint8_t(int)> f, int step);
    /// @brief 对缓冲区内所有像素进行反色.只对现在的缓冲区有效,之后的颜色该怎样就怎样. 灰度信息会被扔掉.
    void invertDisplay();
    /// @brief 进入EPD的低功耗模式
    void sleepEPD(void);
    /// @brief 设置自动全刷慢刷(连续快刷一定次数之后自动调用慢刷, 保护屏幕
    /// @param frames 连续快刷多少次之后自动慢刷, 传入0来禁用自动慢刷
    void setAutoFullRefresh(int16_t frames);
    /// @brief 截屏并保存到SD卡上. 不支持灰度. 当SD卡不可用或者截屏失败时返回false
    bool screenshot(const char *filename);
#ifdef READGUY_ESP_ENABLE_WIFI
    /// @brief ap配网设置页面
    typedef struct {
      String linkname; 
      String event;    //链接名称 事件URI
      std::function<void(ReadguyWebServer*)> func; //触发时执行的函数
    } serveFunc;
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void ap_setup();
    /*** @brief 初始化WiFi AP模式, 用于将来的连接WiFi 可以自定义模式切换的函数
     * @param ssid 设置ap的名称
     * @param pass 设置ap的密码
     * @param m WiFi模式        */
    void ap_setup(const char *ssid, const char *pass, int m=(int)WIFI_AP);
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void server_setup(const String &notify=emptyString, const serveFunc *serveFuncs = nullptr, int funcs = 0);
    bool server_loop();
    void server_end();
#else
    /// @brief ap配网设置页面
    typedef struct {
      String linkname; 
      String event;    //链接名称 事件URI
      std::function<void(void*)> func; //触发时执行的函数
    } serveFunc;
    /// @brief 初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void ap_setup(){}
    void ap_setup(const char *ssid, const char *pass, int m=2){(void)ssid; (void)pass; (void)m;} //avoid warning
    /// @brief 初始化服务器模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
    void server_setup(const String &notify=emptyString, const serveFunc *serveFuncs = nullptr, int funcs = 0)
      {(void)notify;(void)serveFuncs;(void)funcs;} //avoid warning
    bool server_loop(){ return true; }
    void server_end(){}
#endif
    /// @brief 检查初始化屏幕硬件, 若检查失败返回0,否则返回硬件代号
    uint8_t checkEpdDriver();
    /** @brief 初始化屏幕, 设置驱动代号, 引脚排列顺序. 过程会检验引脚可用性.
     *  @param g_width, g_height 显示区域的宽度和高度. 为0表示直接使用屏幕的宽度和高度
     *  @note 这两个参数转专为指定分辨率的程序画面设计, 其他分辨率的画面会自动拉伸. [1.2新增] */
    void setEpdDriver(bool initepd = 1, bool initGFX = 1);
    /** @brief 初始化SD卡, 设置驱动代号, 引脚排列顺序. 过程会检验引脚可用性.
     *  @return SD卡初始化成功与否 */
    bool setSDcardDriver();
    /// @brief 初始化按钮, 背光, 设置驱动代号, 引脚排列顺序
    void setButtonDriver();
    /** @brief 检查SD卡是否插入
     *  @param check 为true时, 如果SD卡不可用则初始化SD卡. 为false时, 当SD卡不可用则返回LittleFS. */
    bool SDinside(bool check=true) { return check?setSDcardDriver():READGUY_sd_ok; };
    void SDdeinit() { READGUY_sd_ok = 0; }; //当SD卡被检测到拔出或不可用时, 调用此函数标记
    /// @brief 检查按钮. 当配置未完成时,按钮不可用, 返回0.
    uint8_t getBtn() { return (READGUY_cali==127)?getBtn_impl():0; }
    /// @brief [此函数已弃用 非常不建议使用] 根据按钮ID来检查按钮. 注意这里如果按下返回0, 没按下或者按钮无效返回1
    //uint8_t getBtn(int btnID){return btnID<getButtonsCount()?(!(btn_rd[btnID].isPressedRaw())):1;}
    /// @brief 该函数用于设置按键是否允许扫描连按
    void setButtonSpecial(bool en = 1) { if(READGUY_buttons==3) btn_rd[1].enScanDT(en); }
    /** @brief 返回可用的文件系统. 当SD卡可用时, 返回SD卡. 否则根据情况返回最近的可用文件系统
     *  @param initSD 2:总是重新初始化SD卡; 1:若SD卡不可用则初始化; 0:SD卡不可用则返回LittleFS. */
    fs::FS &guyFS(uint8_t initSD = 0);
#ifdef ESP8266
    /// @brief 恢复I2C复用SPI时的引脚功能, 仅ESP8266可用
    void recoverI2C();
#else
    /// @brief 恢复I2C引脚功能
    void recoverI2C(){}
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
    /// @brief 设置SD卡为忙状态或空闲状态
    void setSDbusy(bool busy){ if(static_sd_cs!=0x7f) sd_cs_busy += (busy?1:(sd_cs_busy?-1:0)); }
#else
    void setSDbusy(bool busy){ (void)busy; }  //sd_cs as a btn pin
#endif

#if 0 // disabled because of useless
    /// @brief 暂停按键扫描 (比如即将要开启中断或者中断, 定时器等硬件外设资源不足)
    void stopKeyScan(){
#ifdef ESP8266
        btnTask.detach();//暂时关闭任务, 避免引脚被设置为非法模式
#else
        vTaskSuspend(btn_handle);//暂时关闭任务, 避免引脚被设置为非法模式
#endif
    }
    /// @brief 恢复按键扫描 (中断...等需要定时器的硬件外设用完了)
    void keyScan(){
#ifdef ESP8266
        btnTask.attach_ms(BTN_LOOPTASK_DELAY,looptask);
#else
        vTaskResume(btn_handle); //开启任务后, 延时确保按键任务可以活跃而不是一直处于被暂停又刷屏的无限循环
#endif
    }
#endif
    //friend class EpdIf; //这样EpdIf就可以随意操作这个类的成员了
  private:
    //以下是支持的所有屏幕型号 Add devices here!
    //添加屏幕驱动范例: 直接添加对应屏幕的类就可以用了
    static const char projname[8];
    static const char tagname[8];
    //uint8_t config_wifi=0; //是否强行在初始化期间设置WiFi.
    void nvs_init();  //初始化持久存储器.
    void nvs_deinit();//保存持久存储器的内容
    bool nvs_read();  //从持久存储器读取, 返回是否读取成功
    void nvs_write(); //写入到持久存储器
#ifdef DYNAMIC_PIN_SETTINGS//数据是否已经校准
    int8_t config_data[32];
    char randomch[4]; //校验用字符串
#else
    static const int8_t config_data[32];
    int8_t READGUY_sd_ok = 0;
    int8_t READGUY_cali = 127;
    int8_t READGUY_buttons = 0;  //按钮个数, 0-3都有可能
#endif
    int16_t epdPartRefresh = 0; //连续快刷次数
    int16_t epdForceFull   = 0x7fff;  //连续快刷达到指定次数后, 强制全刷, 内部变量
    int16_t currentBright  = -3; //初始亮度
    int16_t current_depth  = 15; //初始灰度
    uint8_t refresh_begin(uint8_t freshType); //设置快速刷新 频率调控
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    void refresh_end();
#endif
    
    //LGFX_Sprite gfx; // 图形引擎类指针, 可以用这个指针去操作屏幕缓冲区
    readguyEpdBase *guy_dev = nullptr;

    //内部调用函数: 初始化epd
    //template <class T> T t_init(T t, bool initial = true);
    //template <class T> void t_display(T t);

#if defined(ESP8266)
  //对于esp8266, 需要注册到ticker
    Ticker btnTask;
#else
    //NVS数据操作函数, 无NVS的使用EEProm的最后几个字节块
    Preferences nvsData;
    static SPIClass *sd_spi;
    static SPIClass *epd_spi;
    static TaskHandle_t btn_handle;
#endif
#ifdef READGUY_ESP_ENABLE_WIFI
    ReadguyWebServer sv;
#ifdef READGUY_UPDATE_SERVER
    ReadguyUpdateServer httpUpdater;
#endif
    String guy_notify=emptyString; //嵌入在网页中的自定义标语
    int sfuncs=-1;
    String* sfnames=nullptr;
    String* sfevents=nullptr;
    void handleInit();      //服务器初始化系统(初次访问时, 跳转至引脚设定函数)
    void handleInitPost();  //服务器响应初始化请求
    void handlePinSetup();  //服务器-引脚设定函数
    void handleFinal();     //服务器-校验屏幕是否正常
    void handleFinalPost(); //服务器-校验屏幕是否正常回调函数
    //void handleWiFi();//[已弃用]服务器-处理WiFi连接相关内容和API接口密钥功能
    void handleNotFound();  //服务器-404响应
#endif
    //按键驱动部分
    static guy_button btn_rd[3];
    /// @brief 复用输出引脚1: 适用于按键引脚与屏幕DC引脚复用的情形
    /// @note 只能解决屏幕DC引脚复用的情况, 其他引脚最好不要复用, 复用了我也解决不了
    static int8_t  pin_cmx;
#ifdef READGUY_ALLOW_DC_AS_BUTTON
    static bool    refresh_state; //1: free; 0: busy(refreshing)
    static uint8_t refresh_press; //0x7f: epd_dc btn released; others: epd_dc btn pressed
#endif
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
    static uint8_t static_epd_cs; //epd_cs as a btn pin
#endif
#ifdef READGUY_ALLOW_SDCS_AS_BUTTON
    static uint8_t static_sd_cs;  //sd_cs as a btn pin
    static volatile uint8_t sd_cs_busy;  //sd_cs as a btn pin
#endif
    static volatile uint8_t spibz;
#ifdef READGUY_ESP_ENABLE_WIFI
    //static constexpr size_t EPD_DRIVERS_NUM_MAX = READGUY_SUPPORT_DEVICES;
    static const char *epd_drivers_list[EPD_DRIVERS_NUM_MAX];
    static const PROGMEM char html_header[]; //HTML头的数据. 省内存, 能省一点是一点
    static const PROGMEM char index_cn_html[];
    static const PROGMEM char index_cn_html2[];
    static const PROGMEM char index_cn_html3[];
    static const PROGMEM char index_cn_html16[];
    static const PROGMEM char verify_html[];
    static const PROGMEM char verify2_html[];
    static const PROGMEM char *verifybtn_html[3];
    static const PROGMEM char verifybtn_html1[];
    static const PROGMEM char verifybtn_html2[];
    static const PROGMEM char verifybtn_html3[];
    static const PROGMEM char final_html[];
    static const PROGMEM char afterConfig_html[];
    static const PROGMEM char home_html[];
    static const PROGMEM char end_html[];
#ifdef READGUY_USE_DEFAULT_ICON
    static const PROGMEM uint8_t faviconData[1150];
#endif
#endif
    static void looptask(); //按键服务函数
    static uint8_t rd_btn_f(uint8_t btn, bool activeLow);
    uint8_t getBtn_impl(); //按钮不可用, 返回0.
    static void in_press(){ //SPI开始传输屏幕数据
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
      if(!(spibz&0x3f))
#else
      if(!spibz)
#endif
#ifdef ESP8266
        SPI.beginTransaction(SPISettings(ESP8266_SPI_FREQUENCY, MSBFIRST, SPI_MODE0));
#else
        epd_spi->beginTransaction(SPISettings(ESP32_DISP_FREQUENCY, MSBFIRST, SPI_MODE0));
#endif
      spibz ++; 
    }
    static void in_release(){//SPI结束传输屏幕数据
      spibz --;
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
      if(!(spibz&0x3f))
#else
      if(!spibz)
#endif
#ifdef ESP8266
        SPI.endTransaction();
#else
        epd_spi->endTransaction();
#endif
    }
  public: //增加了一些返回系统状态变量的函数, 它们是静态的, 而且不会对程序造成任何影响.
    RG_IL_CONEXP int getShareSpi() const { return READGUY_shareSpi; }
    RG_IL_CONEXP int getEpdType () const { return READGUY_epd_type; } // 对应的epd驱动程序代号, -1为未指定
        //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
    RG_IL_CONEXP int getEpdMosi () const { return READGUY_epd_mosi; } // 目标显示器的 MOSI 引脚
    RG_IL_CONEXP int getEpdSclk () const { return READGUY_epd_sclk; } // 目标显示器的 SCLK 引脚
    RG_IL_CONEXP int getEpdCs   () const { return READGUY_epd_cs; } // 目标显示器的 CS   引脚
    RG_IL_CONEXP int getEpdDc   () const { return READGUY_epd_dc; } // 目标显示器的 DC   引脚
    RG_IL_CONEXP int getEpdRst  () const { return READGUY_epd_rst; } // 目标显示器的 RST  引脚
    RG_IL_CONEXP int getEpdBusy () const { return READGUY_epd_busy; } // 目标显示器的 BUSY 引脚
        //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
    RG_IL_CONEXP int getSdMiso  () const { return READGUY_sd_miso; } // 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
    RG_IL_CONEXP int getSdMosi  () const { return READGUY_sd_mosi; }// 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
    RG_IL_CONEXP int getSdSclk  () const { return READGUY_sd_sclk; }// 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
    RG_IL_CONEXP int getSdCs    () const { return READGUY_sd_cs; }// 目标sd卡的CS引脚. 对ESP32S3, 返回127代表使用SDMMC
    RG_IL_CONEXP int getI2cSda  () const { return READGUY_i2c_sda; }// 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
    RG_IL_CONEXP int getI2cScl  () const { return READGUY_i2c_scl; }// 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效
        //按键驱动部分, 为负代表高触发, 否则低触发,
        //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
    RG_IL_CONEXP int getBtn1Pin () const { return abs((int)READGUY_btn1)-1; } 
    RG_IL_CONEXP int getBtn2Pin () const { return abs((int)READGUY_btn2)-1; } 
    RG_IL_CONEXP int getBtn3Pin () const { return abs((int)READGUY_btn3)-1; } 
    RG_IL_CONEXP int getBtn1Info() const { return READGUY_btn1; } 
    RG_IL_CONEXP int getBtn2Info() const { return READGUY_btn2; } 
    RG_IL_CONEXP int getBtn3Info() const { return READGUY_btn3; } 
    RG_IL_CONEXP int getBlPin   () const { return READGUY_bl_pin; } //前置光接口引脚IO
    RG_IL_CONEXP int getRtcType () const { return READGUY_rtc_type; } //RTC 将在2.0实装 保留是为了兼容性 让代码更简单维护
    RG_IL_CONEXP int getButtonsCount() const { return READGUY_buttons; } //按钮个数, 0-3都有可能
    RG_IL_CONEXP int getReadguy_user1 () const { return READGUY_user1; } //用户变量
    RG_IL_CONEXP int getReadguy_user2 () const { return READGUY_user2; } //用户变量
    RG_IL_CONEXP int getReadguy_user3 () const { return READGUY_user3; } //用户变量
    RG_IL_CONEXP int getReadguy_user4 () const { return READGUY_user4; } //用户变量
    RG_IL_CONEXP int getReadguy_user5 () const { return READGUY_user5; } //用户变量
    RG_IL_CONEXP int getReadguy_user6 () const { return READGUY_user6; } //用户变量
    RG_IL_CONEXP int getReadguy_user7 () const { return READGUY_user7; } //用户变量
    RG_IL_CONEXP int getReadguy_user8 () const { return READGUY_user8; } //用户变量
    RG_IL_CONEXP int getReadguy_user9 () const { return READGUY_user9; } //用户变量
    RG_IL_CONEXP int getReadguy_user10() const { return READGUY_user10;} //用户变量
    RG_IL_CONEXP bool getReadguyUseSdio() const { //返回程序调用SD卡时 是否使用了SDIO
#ifdef CONFIG_IDF_TARGET_ESP32S3              //仅对ESP32S3可用
      return (READGUY_user1 != -1) && (READGUY_user2 != -1) && (READGUY_sd_cs != -1);
#else
      return 0;                               //非ESP32S3平台不可用SDIO
#endif
    } //用于esp32s3使用SDIO卡数据的DAT2
#ifdef CONFIG_IDF_TARGET_ESP32S3              //仅对ESP32S3可用
    RG_IL_CONEXP int getSdio_dat0 () const { return getReadguyUseSdio()?READGUY_sd_miso:-1; } //用于esp32s3使用SDIO卡数据的DAT0
    RG_IL_CONEXP int getSdio_dat1 () const { return getReadguyUseSdio()?READGUY_user1  :-1; } //用于esp32s3使用SDIO卡数据的DAT1
    RG_IL_CONEXP int getSdio_dat2 () const { return getReadguyUseSdio()?READGUY_user2  :-1; } //用于esp32s3使用SDIO卡数据的DAT2
    RG_IL_CONEXP int getSdio_dat3 () const { return getReadguyUseSdio()?READGUY_sd_cs  :-1; } //用于esp32s3使用SDIO卡数据的DAT3
    RG_IL_CONEXP int getSdio_clk  () const { return getReadguyUseSdio()?READGUY_sd_sclk:-1; } //用于esp32s3使用SDIO卡数据的CLK
    RG_IL_CONEXP int getSdio_cmd  () const { return getReadguyUseSdio()?READGUY_sd_mosi:-1; } //用于esp32s3使用SDIO卡数据的CMD
#else
    RG_IL_CONEXP int getSdio_dat0 () const { return -1; } //用于esp32s3使用SDIO卡数据的DAT0
    RG_IL_CONEXP int getSdio_dat1 () const { return -1; } //用于esp32s3使用SDIO卡数据的DAT1
    RG_IL_CONEXP int getSdio_dat2 () const { return -1; } //用于esp32s3使用SDIO卡数据的DAT2
    RG_IL_CONEXP int getSdio_dat3 () const { return -1; } //用于esp32s3使用SDIO卡数据的DAT3
    RG_IL_CONEXP int getSdio_clk  () const { return -1; } //用于esp32s3使用SDIO卡数据的CLK
    RG_IL_CONEXP int getSdio_cmd  () const { return -1; } //用于esp32s3使用SDIO卡数据的CMD
#endif
    //RG_IL_CONEXP int memWidth   () const { return guy_width ;  } //返回显存宽度(不是画幅宽度),不会随着画布旋转改变
    //RG_IL_CONEXP int memHeight  () const { return guy_height ; } //返回显存高度(不是画幅高度),不会随着画布旋转改变
    int drvWidth () const { return READGUY_cali==127?guy_dev->drv_width():0;  } //返回显示屏硬件宽度(不是画幅宽度)
    int drvHeight() const { return READGUY_cali==127?guy_dev->drv_height():0; } //返回显示屏硬件高度(不是画幅高度)
  //int width () const { return (getRotation()&1)?drvHeight():drvWidth(); }
  //int height() const { return (getRotation()&1)?drvWidth():drvHeight(); }
    size_t getFreeMem() const { return 
#ifdef ESP8266
      ESP.getFreeHeap();
#else
      esp_get_free_heap_size();
#endif
    }
//  private:
    void implBeginTransfer() { guy_dev->BeginTransfer(); } //此函数用于开启SPI传输, 只能在自定义刷屏函数中使用!!
    void implEndTransfer()   { guy_dev->EndTransfer();   } //此函数用于开启SPI传输, 只能在自定义刷屏函数中使用!!
    /// @brief 分阶段显示图片, 使用抖动算法. 更加的省内存.目前函数
    void drawImageStage(LGFX_Sprite &spr,int32_t x,int32_t y,uint8_t stage,uint8_t totalstage,
      int32_t zoomw=0,int32_t zoomh=0){ drawImageStage(*this,spr,x,y,stage,totalstage,zoomw,zoomh); }
    void drawImageStage(LGFX_Sprite &sprbase,LGFX_Sprite &spr,int32_t x,int32_t y,
      uint8_t stage,uint8_t totalstage,int32_t zoomw=0,int32_t zoomh=0);
};
#undef RG_IL_CONEXP
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */