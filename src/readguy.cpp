/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_driver.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 基础功能源代码文件. 
 * @version 1.0
 * @date created: 2023-09-21    last modify: 2024-03-11

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

#include "readguy.h"
/* #if (!defined(ESP8266)) //for ESP32, ESP32S2, ESP32S3, ESP32C3
#include "esp32-hal.h"
#endif */
#if (!defined(INPUT_PULLDOWN)) //not supported pinMode
#define INPUT_PULLDOWN INPUT
#endif

guy_button ReadguyDriver::btn_rd[3];
int8_t     ReadguyDriver::pin_cmx=-1;
#ifdef READGUY_ALLOW_DC_AS_BUTTON
bool       ReadguyDriver::refresh_state=1; //1: free; 0: busy(refreshing)
uint8_t    ReadguyDriver::refresh_press=0x7f; //0x7f: no button pressed other:button pressed pin
#endif
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
uint8_t    ReadguyDriver::static_epd_cs=0x7f;
#endif
#ifdef READGUY_ALLOW_SDCS_AS_BUTTON
uint8_t    ReadguyDriver::static_sd_cs=0x7f;
volatile uint8_t ReadguyDriver::sd_cs_busy=0;
#endif

const PROGMEM char ReadguyDriver::projname[8] = "readguy";
const PROGMEM char ReadguyDriver::tagname[7] = "hwconf";
volatile uint8_t ReadguyDriver::spibz=0;
#ifndef DYNAMIC_PIN_SETTINGS
const int8_t ReadguyDriver::config_data[32] = {
  127              , //READGUY_cali
  READGUY_shareSpi ,
  READGUY_epd_type ,// 对应的epd驱动程序代号, -1为未指定
  //显示驱动部分, 显示默认使用vspi (vspi也是默认SPI库的通道)
  READGUY_epd_mosi ,// 目标显示器的 MOSI 引脚
  READGUY_epd_sclk ,// 目标显示器的 SCLK 引脚
  READGUY_epd_cs   ,// 目标显示器的 CS   引脚
  READGUY_epd_dc   ,// 目标显示器的 DC   引脚
  READGUY_epd_rst  ,// 目标显示器的 RST  引脚
  READGUY_epd_busy ,// 目标显示器的 BUSY 引脚
  //sd卡驱动部分, 默认使用hspi (sd卡建议用hspi)
  READGUY_sd_miso  ,// 目标sd卡的 MISO 引脚, 或esp32s3使用SDIO的 DAT0 引脚, sd_share_spi == 1 时无效
  READGUY_sd_mosi  ,// 目标sd卡的 MOSI 引脚, 或esp32s3使用SDIO的 CMD 引脚, sd_share_spi == 1 时无效
  READGUY_sd_sclk  ,// 目标sd卡的 SCLK 引脚, 或esp32s3使用SDIO的 CLK 引脚, sd_share_spi == 1 时无效
  READGUY_sd_cs    ,// 目标sd卡的 CS   引脚, 或esp32s3使用SDIO的 DAT3 引脚,
  READGUY_i2c_sda  ,// 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
  READGUY_i2c_scl  ,// 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效
  //按键驱动部分, 为负代表高触发, 否则低触发,
  //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
  READGUY_btn1     , 
  READGUY_btn2     , 
  READGUY_btn3     , 
  READGUY_bl_pin   ,//前置光接口引脚IO
  READGUY_rtc_type ,//使用的RTC型号. 现已弃用 RTC 功能. 保留是为了兼容性 让代码更简单维护
  0                ,//READGUY_sd_ok   SD卡已经成功初始化
  0                ,//READGUY_buttons 按钮个数, 0-3都有可能
  -1, //用户自定义变量 同时用于esp32s3使用SDIO卡数据的DAT1 为-1代表不使用SDIO
  -1, //用户自定义变量 同时用于esp32s3使用SDIO卡数据的DAT2
  -1,-1,-1,-1,-1,-1,-1,-1 //user data 区域, 此功能没啥用就暂时全设定为-1了
};
#endif
#ifndef ESP8266
SPIClass *ReadguyDriver::sd_spi =nullptr;
SPIClass *ReadguyDriver::epd_spi=nullptr;
TaskHandle_t ReadguyDriver::btn_handle;
#endif

ReadguyDriver::ReadguyDriver(){
  READGUY_cali = 0; // config_data[0] 的初始值为0
#ifdef DYNAMIC_PIN_SETTINGS
  for(unsigned int i=1;i<sizeof(config_data);i++) config_data[i] = -1;
#endif
  READGUY_sd_ok = 0; //初始默认SD卡未成功初始化
  READGUY_buttons = 0; //初始情况下没有按钮
} //WiFiSet: 是否保持AP服务器一直处于打开状态
uint8_t ReadguyDriver::init(uint8_t WiFiSet, bool initepd, bool initSD){
  if(READGUY_cali==127) //已经初始化过了一次了, 为了防止里面一些volatile的东西出现问题....还是退出吧
    return READGUY_sd_ok;
#ifdef DYNAMIC_PIN_SETTINGS
  nvs_init();
#if (!defined(READGUY_INDEV_DEBUG))
  if(!nvs_read()){  //如果NVS没有录入数据, 需要打开WiFiAP模式初始化录入引脚数据
#endif
#ifdef READGUY_ESP_ENABLE_WIFI
    ap_setup();    //开启WiFi和服务器, 然后网页获取数据. 如果保持WiFi AP模式打开, 则显示此标语.
    if(WiFiSet) server_setup(F("引脚配置完成。请稍后访问<a href=\"/\">此页面</a>浏览更多内容。"));
    else server_setup(F("引脚配置完成, WiFi即将关闭。")); //直接进入主页.
    for(uint32_t i=UINT32_MAX;millis()<i;){
      if(server_loop()){
        if(i==UINT32_MAX) i=millis()+500;
      }
      yield();
    }
    //delay(300); //等待网页加载完再关掉WiFi. (有没有用存疑)
    server_end();
    if(!WiFiSet) WiFi.mode(WIFI_OFF);
    fillScreen(1);
#else //DYNAMIC_PIN_SETTINGS 已定义 and READGUY_ESP_ENABLE_WIFI 已定义 but NVS无数据, 此时无法配置引脚. 视为halt
    (void)WiFiSet; //avoid warning
    nvs_deinit(); //invalid
    Serial.printf_P(PSTR("[Guy NVS] INVALID pin settings on " _READGUY_PLATFORM " (Readguy lib version " READGUY_VERSION ")!\n"\
    "Please open guy_driver_config.h and uncomment #define READGUY_ESP_ENABLE_WIFI!\n"\
    "You can also flash example binary with Wi-Fi, configure pins and re-flash this program.\n"\
    "System will restart in 5 seconds..."));
    delay(5000);
    ESP.restart();
#endif
#if (!defined(READGUY_INDEV_DEBUG))
  }
  else{ //看来NVS有数据, //从NVS加载数据, 哪怕前面的数据刚刚写入, 还没读取
#ifdef READGUY_ESP_ENABLE_WIFI
    if(WiFiSet>=2) WiFi.begin(); //连接到上次存储在flash NVS中的WiFi.
    else
#endif
    if(WiFiSet==1) ap_setup();
    if(checkEpdDriver()!=127) setEpdDriver(initepd/* ,g_width,g_height */);  //初始化屏幕
    else for(;;); //此处可能添加程序rollback等功能操作(比如返回加载上一个程序)
    if(initSD) setSDcardDriver();
    setButtonDriver();
  }
#endif
  nvs_deinit();
#else
  (void)WiFiSet; //avoid warning
  nvs_init();
  if(checkEpdDriver()!=127) setEpdDriver(initepd/* ,g_width,g_height */);  //初始化屏幕
  else for(;;); //此处可能添加程序rollback等功能操作(比如返回加载上一个程序)
  if(initSD) setSDcardDriver();
  setButtonDriver();
  if(!nvs_read()){
    nvs_write(); //全部成功之后, 写入引脚信息到NVS.
  }
  nvs_deinit();
#endif
#ifdef READGUY_SERIAL_DEBUG
  Serial.println(F("[Guy init] init done."));
#endif
  READGUY_cali=127;
  return READGUY_sd_ok;
}
uint8_t ReadguyDriver::checkEpdDriver(){
#ifdef DYNAMIC_PIN_SETTINGS
#ifdef ESP8266
#define TEST_ONLY_VALUE 5
#else 
#define TEST_ONLY_VALUE 3
#endif
#ifdef READGUY_SERIAL_DEBUG
  Serial.printf_P(PSTR("[Guy SPI] shareSpi? %d\n"),READGUY_shareSpi);
#endif
  for(int i=TEST_ONLY_VALUE;i<8;i++){
    if(i<7 && config_data[i]<0) return 125;//必要的引脚没连接
    for(int j=1;j<=8-i;j++)
      if(config_data[i]>=0 && config_data[i]==config_data[i+j]) return 126; //有重复引脚
  }
#undef TEST_ONLY_VALUE
#endif
  if(guy_dev != nullptr && READGUY_epd_type!=guy_dev->drv_ID()) delete guy_dev; //释放多余的内存
  //Serial.printf("initing epd %s...\n",epd_drivers_list[READGUY_epd_type]);
#ifdef DYNAMIC_PIN_SETTINGS
  switch (READGUY_epd_type){
#ifdef READGUY_DEV_154A
    case READGUY_DEV_154A:    guy_dev = new guydev_154A_290A       ::dev154A;   break; //适用于一般的价签黑白屏
#endif
#ifdef READGUY_DEV_154B
    case READGUY_DEV_154B:    guy_dev = new guydev_154B_270B_290B  ::dev154B;   break; //适用于
#endif
#ifdef READGUY_DEV_213A
    case READGUY_DEV_213A:    guy_dev = new guydev_213A            ::drv;       break;
#endif
#ifdef READGUY_DEV_266A
    case READGUY_DEV_266A:    guy_dev = new guydev_213B_266A       ::dev266A;   break;
#endif
#ifdef READGUY_DEV_213B
    case READGUY_DEV_213B:    guy_dev = new guydev_213B_266A       ::dev213B;   break;
#endif
#ifdef READGUY_DEV_290A
    case READGUY_DEV_290A:    guy_dev = new guydev_154A_290A       ::dev290A;   break;
#endif
#ifdef READGUY_DEV_290B
    case READGUY_DEV_290B:    guy_dev = new guydev_154B_270B_290B  ::dev290B;   break;
#endif
#ifdef READGUY_DEV_420A
    case READGUY_DEV_420A:    guy_dev = new guydev_420A            ::drv;       break;
#endif
#ifdef READGUY_DEV_420B
    case READGUY_DEV_420B:    guy_dev = new guydev_420B            ::drv;       break;
#endif
#ifdef READGUY_DEV_370A
    case READGUY_DEV_370A:    guy_dev = new guydev_370A            ::drv;       break;
#endif
#ifdef MEPD_DEBUG_DISPLAY
    case MEPD_DEBUG_DISPLAY:  guy_dev = new EpdLcdDebug            ::drv;       break;
#endif
#ifdef READGUY_DEV_270B
    case READGUY_DEV_270B:    guy_dev = new guydev_154B_270B_290B  ::dev270B;   break;
#endif
#ifdef READGUY_DEV_213B3C
    case READGUY_DEV_213B3C:  guy_dev = new guydev_213B_266A       ::dev213B3C; break;
#endif
#ifdef READGUY_DEV_266A3C
    case READGUY_DEV_266A3C:  guy_dev = new guydev_213B_266A       ::dev266A3C; break;
#endif

#ifdef READGUY_DEV_154C
    case READGUY_DEV_154C:    guy_dev = new guydev_154C            ::drv;       break;
#endif
#ifdef READGUY_DEV_370B
    case READGUY_DEV_370B:    guy_dev = new guydev_370B            ::drv;       break;
#endif
#ifdef READGUY_DEV_426A
    case READGUY_DEV_426A:    guy_dev = new guydev_426A            ::drv;       break;
#endif
#ifdef READGUY_DEV_583A
    case READGUY_DEV_583A:    guy_dev = new guydev_583A            ::drv;       break;
#endif
#ifdef READGUY_DEV_583B
    case READGUY_DEV_583B:    guy_dev = new guydev_583B            ::drv;       break;
#endif
#ifdef READGUY_DEV_750A
    case READGUY_DEV_750A:    guy_dev = new guydev_750A            ::drv;       break;
#endif
#ifdef READGUY_DEV_1020A
    case READGUY_DEV_1020A:   guy_dev = new guydev_1020A           ::drv;       break;
#endif
  //添加新屏幕型号 add displays here
    default: 
#ifdef READGUY_SERIAL_DEBUG
      Serial.println(F("[Guy Error] EPD DRIVER IC NOT SUPPORTED!\n"));
#endif
      return 127;
  }
#else
#if   (defined(READGUY_DEV_154A)    && (READGUY_epd_type==READGUY_DEV_154A))
    guy_dev = new guydev_154A_290A       ::dev154A; //适用于一般的价签黑白屏
#elif (defined(READGUY_DEV_154B)    && (READGUY_epd_type==READGUY_DEV_154B))
    guy_dev = new guydev_154B_270B_290B  ::dev154B; //适用于
#elif (defined(READGUY_DEV_213A)    && (READGUY_epd_type==READGUY_DEV_213A))
    guy_dev = new guydev_213A            ::drv;    
#elif (defined(READGUY_DEV_266A)    && (READGUY_epd_type==READGUY_DEV_266A))
    guy_dev = new guydev_213B_266A       ::dev266A;
#elif (defined(READGUY_DEV_213B)    && (READGUY_epd_type==READGUY_DEV_213B))
    guy_dev = new guydev_213B_266A       ::dev213B;
#elif (defined(READGUY_DEV_290A)    && (READGUY_epd_type==READGUY_DEV_290A))
    guy_dev = new guydev_154A_290A       ::dev290A;
#elif (defined(READGUY_DEV_290B)    && (READGUY_epd_type==READGUY_DEV_290B))
    guy_dev = new guydev_154B_270B_290B  ::dev290B;
#elif (defined(READGUY_DEV_420A)    && (READGUY_epd_type==READGUY_DEV_420A))
    guy_dev = new guydev_420A            ::drv;    
#elif (defined(READGUY_DEV_420B)    && (READGUY_epd_type==READGUY_DEV_420B))
    guy_dev = new guydev_420B            ::drv;    
#elif (defined(READGUY_DEV_370A)    && (READGUY_epd_type==READGUY_DEV_370A))
    guy_dev = new guydev_370A            ::drv;    
#elif (defined(MEPD_DEBUG_DISPLAY)  && (READGUY_epd_type==MEPD_DEBUG_DISPLAY))
    guy_dev = new EpdLcdDebug            ::drv;    
#elif (defined(READGUY_DEV_270B)    && (READGUY_epd_type==READGUY_DEV_270B))
    guy_dev = new guydev_154B_270B_290B  ::dev270B;
#elif (defined(READGUY_DEV_213B3C)  && (READGUY_epd_type==READGUY_DEV_213B3C))
    guy_dev = new guydev_213B_266A       ::dev213B3C;
#elif (defined(READGUY_DEV_266A3C)  && (READGUY_epd_type==READGUY_DEV_266A3C))
    guy_dev = new guydev_213B_266A       ::dev266B3C;

#elif (defined(READGUY_DEV_154C)    && (READGUY_epd_type==READGUY_DEV_154C))
    guy_dev = new guydev_154C            ::drv;
#elif (defined(READGUY_DEV_370B)    && (READGUY_epd_type==READGUY_DEV_370B))
    guy_dev = new guydev_370B            ::drv;
#elif (defined(READGUY_DEV_426A)    && (READGUY_epd_type==READGUY_DEV_426A))
    guy_dev = new guydev_426A            ::drv;
#elif (defined(READGUY_DEV_583A)    && (READGUY_epd_type==READGUY_DEV_583A))
    guy_dev = new guydev_583A            ::drv;
#elif (defined(READGUY_DEV_583B)    && (READGUY_epd_type==READGUY_DEV_583B))
    guy_dev = new guydev_583B            ::drv;
#elif (defined(READGUY_DEV_750A)    && (READGUY_epd_type==READGUY_DEV_750A))
    guy_dev = new guydev_750A            ::drv;
#elif (defined(READGUY_DEV_1020A)   && (READGUY_epd_type==READGUY_DEV_1020A))
    guy_dev = new guydev_1020A           ::drv;
#endif
  //添加新屏幕型号 add displays here
#endif
#if (defined(ESP8266))
  SPI.begin();
  SPI.setFrequency(ESP8266_SPI_FREQUENCY); ///< 1MHz
  guy_dev->IfInit(SPI, READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy);
#else
  if(epd_spi == nullptr) epd_spi = &SPI; //ESP32S2和S3和C3 不支持VSPI, C3只支持HSPI. 使用SPI库默认提供的HSPI.
  else epd_spi->end();
  //epd_spi ->setFrequency(ESP32_DISP_FREQUENCY);
  //Serial.println("deleting guy_dev");
  if(READGUY_shareSpi) sd_spi = epd_spi;
  else {
    if(sd_spi!=nullptr && sd_spi!=&SPI) { sd_spi->end(); delete sd_spi; } //防止SPI被delete掉
    sd_spi=nullptr;
  }
  epd_spi->begin(READGUY_epd_sclk,READGUY_shareSpi?READGUY_sd_miso:-1,READGUY_epd_mosi);
  guy_dev->IfInit(*epd_spi, READGUY_epd_cs, READGUY_epd_dc, READGUY_epd_rst, READGUY_epd_busy);
#endif
#ifdef READGUY_SERIAL_DEBUG
  Serial.println(F("[Guy SPI] drvBase Init OK"));
#endif
  return READGUY_epd_type;
}
void ReadguyDriver::setEpdDriver(bool initepd, bool initGFX){
  guy_dev->spi_tr_release = in_release;
  guy_dev->spi_tr_press   = in_press;
  guy_dev->drv_init(); //初始化epd驱动层
  //if(g_width) guy_width = g_width;
  //else guy_width = guy_dev->drv_width(); //宽度必须是8的倍数, 但这个可以由GFX自动计算
  //if(g_height) guy_height = g_height;
  //else guy_height = guy_dev->drv_height();
  //以下依赖于你的图形驱动
  if(initGFX){
    setColorDepth(1); //单色模式
    createPalette();  //初始化颜色系统
    //创建画布. 根据LovyanGFX的特性, 如果以前有画布会自动重新生成新画布
    //此外, 即使画布宽度不是8的倍数(如2.13寸单色),也支持自动补全8的倍数 ( 250x122 => 250x128 )
    //为了保证图片显示功能的正常使用, 高度也必须是8的倍数.
    createSprite(guy_dev->drv_width(),guy_dev->drv_height());
    //这里发现如果用自定义的内存分配方式会更好一些. 不会导致返回的height不对. 但是因为LovyanGFX库未更新 暂时不能这么用.
    setRotation(0); //默认的旋转方向就是0咯
    setFont(&fonts::Font0);
    setCursor(0,0);
    setTextColor(0);
    fillScreen(1); //开始先全屏白色
  }
#ifdef READGUY_SERIAL_DEBUG
  Serial.printf_P(PSTR("[Guy EPD] EPD init OK(%d): w: %d, h: %d\n"),guy_dev->drv_ID(),guy_dev->drv_width(),guy_dev->drv_height());
#endif
  if(initepd) display(READGUY_SLOW); //25/3/13:feat: 如果initepd为true说明墨水屏需要从上电or休眠初始化, 必定慢刷
}
#ifdef READGUY_ENABLE_SD
bool ReadguyDriver::setSDcardDriver(){
  /*重要信息: 有些引脚冲突是难以避免的, 比如8266 尤其需要重写这部分代码
    对于esp32也要注意这个引脚是否是一个合法的引脚
    对于esp8266真的要重写, 比如esp8266需要允许某些引脚是可以复用的
    此处的函数必须是可以反复调用的, 即 "可重入函数" 而不会造成任何线程危险 */
  if(READGUY_sd_cs>=0
#ifndef ESP8266
  &&READGUY_sd_miso!=READGUY_sd_mosi&&READGUY_sd_miso!=READGUY_sd_sclk&&READGUY_sd_miso!=READGUY_sd_cs&&READGUY_sd_mosi!=READGUY_sd_sclk
  && READGUY_sd_mosi!=READGUY_sd_cs && READGUY_sd_sclk!=READGUY_sd_cs && READGUY_sd_miso>=0 && READGUY_sd_mosi>=0 && READGUY_sd_sclk>=0
#endif
  && READGUY_sd_cs!=READGUY_epd_cs && READGUY_sd_cs!=READGUY_epd_dc && READGUY_sd_cs!=READGUY_epd_rst && READGUY_sd_cs!=READGUY_epd_busy
  ){ //SD卡的CS检测程序和按键检测程序冲突, 故删掉 (可能引发引脚无冲突但是显示冲突的bug)
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON))
    setSDbusy(1);
#endif
#if defined(ESP8266) //Esp8266无视SPI的设定, 固定为唯一的硬件SPI (D5=SCK, D6=MISO, D7=MOSI)
    SDFS.end();
    SDFS.setConfig(SDFSConfig(READGUY_sd_cs));
    READGUY_sd_ok = SDFS.begin();
#else
    if(sd_spi == nullptr) {
#if (defined(READGUY_IDF_TARGET_WITH_VSPI))
      sd_spi = new SPIClass(VSPI);
#else
      sd_spi = new SPIClass(FSPI); //ESP32S2和S3和C3 不支持VSPI, C3不支持FSPI
#endif
      sd_spi->begin(READGUY_sd_sclk,READGUY_sd_miso,READGUY_sd_mosi); //初始化SPI
    }
    READGUY_sd_ok = SD.begin(READGUY_sd_cs,*sd_spi,ESP32_SD_SPI_FREQUENCY); //初始化频率为20MHz
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON))
    setSDbusy(0);
#endif
  }
  else READGUY_sd_ok=0; //引脚不符合规则,或冲突或不可用
  if(!READGUY_sd_ok){
#ifdef READGUY_SERIAL_DEBUG
    Serial.println(F("[Guy SD] SD Init Failed!"));
#endif
    //guyFS().begin(); //初始化内部FS
#ifdef READGUY_USE_LITTLEFS
    if(!LittleFS.begin())
      LittleFS.format(); //初始化失败时, 格式化文件系统
#else
    SPIFFS.begin();
#endif
  }
  return READGUY_sd_ok;
}
#else
bool ReadguyDriver::setSDcardDriver(){
  READGUY_sd_ok=0;
#ifdef READGUY_USE_LITTLEFS
  if(!LittleFS.begin())
    LittleFS.format(); //初始化失败时, 格式化文件系统
#else
  SPIFFS.begin();
#endif
  return false;
}
#endif
void ReadguyDriver::setButtonDriver(){
  if(READGUY_btn1) { //初始化按键. 注意高电平触发的引脚在初始化时要设置为下拉
    int8_t btn_pin=abs((int)READGUY_btn1)-1;
#if defined(ESP8266) //只有ESP8266是支持16引脚pulldown功能的, 而不支持pullup
    if(btn_pin == 16) pinMode(16,(READGUY_btn1>0)?INPUT:INPUT_PULLDOWN_16);
    else if(btn_pin < 16 && btn_pin != D5 && btn_pin != D6 && btn_pin != D7)
#endif
    {
      if(btn_pin!=READGUY_epd_dc) pinMode(btn_pin,(READGUY_btn1>0)?INPUT_PULLUP:INPUT_PULLDOWN);
    }
    READGUY_buttons=1;
  }
  if(READGUY_btn2) {
    int8_t btn_pin=abs(READGUY_btn2)-1;
#if defined(ESP8266) //只有ESP8266是支持16引脚pulldown功能的, 而不支持pullup
    if(btn_pin == 16) pinMode(16,(READGUY_btn2>0)?INPUT:INPUT_PULLDOWN_16);
    else if(btn_pin < 16 && btn_pin != D5 && btn_pin != D6 && btn_pin != D7)
#endif
    {
      if(btn_pin!=READGUY_epd_dc) pinMode(btn_pin,(READGUY_btn2>0)?INPUT_PULLUP:INPUT_PULLDOWN);
    }
    READGUY_buttons=2;
  }
  if(READGUY_btn3) {
    int8_t btn_pin=abs(READGUY_btn3)-1;
#if defined(ESP8266) //只有ESP8266是支持16引脚pulldown功能的, 而不支持pullup
    if(btn_pin == 16) pinMode(16,(READGUY_btn3>0)?INPUT:INPUT_PULLDOWN_16);
    else if(btn_pin < 16 && btn_pin != D5 && btn_pin != D6 && btn_pin != D7)
#endif
    {
      if(btn_pin!=READGUY_epd_dc) pinMode(btn_pin,(READGUY_btn3>0)?INPUT_PULLUP:INPUT_PULLDOWN);
    }
    READGUY_buttons=3;
  }

  if(abs(READGUY_btn1)-1==READGUY_epd_dc || abs(READGUY_btn2)-1==READGUY_epd_dc
  || abs(READGUY_btn3)-1==READGUY_epd_dc ) {
    pin_cmx=READGUY_epd_dc; //DC引脚复用
  }
#if ((defined (READGUY_ALLOW_EPDCS_AS_BUTTON)) || ((defined(READGUY_ALLOW_SDCS_AS_BUTTON)) && (defined(READGUY_ENABLE_SD))))
  for(int j=15;j<=17;j++){ //btn1~3
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
    if(READGUY_epd_cs == abs(config_data[j])-1) static_epd_cs = READGUY_epd_cs;
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
    if(READGUY_sd_cs == abs(config_data[j])-1 && READGUY_sd_cs!=-1) static_sd_cs = READGUY_sd_cs;
#endif
  }
#endif
  //初始化按钮, 原计划要使用Button2库, 后来发现实在是太浪费内存, 于是决定自己写
  if(READGUY_btn1) btn_rd[0].begin(abs(READGUY_btn1)-1,rd_btn_f,(READGUY_btn1>0));
  if(READGUY_btn2) btn_rd[1].begin(abs(READGUY_btn2)-1,rd_btn_f,(READGUY_btn2>0));
  if(READGUY_btn3) btn_rd[2].begin(abs(READGUY_btn3)-1,rd_btn_f,(READGUY_btn3>0));
  //if(READGUY_buttons==1){
    btn_rd[0].setLongRepeatMode(1); //允许连按
  //}
  if(READGUY_buttons==2){
    btn_rd[0].enScanDT(0); //不识别双击或三击
    //btn_rd[0].setLongRepeatMode(1); //双按键 选择按键 设置为允许连按
    btn_rd[1].enScanDT(0); //不识别双击或三击
    btn_rd[1].setLongRepeatMode(0);  //双按键 确定按键 设置为不允许连按
  }
  else if(READGUY_buttons==3){
    btn_rd[0].long_press_ms = 20; //只有长按, 按一下也是长按,
    btn_rd[0].double_press_ms = 20; //不识别双击三击,
    btn_rd[0].setLongRepeatMode(1); //并且开启连按
    //2024/2/25更新:需要支持连按适配拨轮
    btn_rd[1].enScanDT(1); //识别双击或三击(默认)    2025/3/12更新:默认开启识别, 但按下确定键的响应会稍微变慢
    btn_rd[1].setLongRepeatMode(0); //三按键 确定按键 设置为不允许连按
    btn_rd[2].long_press_ms = 20; //只有长按, 按一下也是长按, 并且开启连按
    btn_rd[2].double_press_ms = 20; //不识别双击三击,
    btn_rd[2].setLongRepeatMode(1); //并且开启连按
  }
#ifdef ESP8266 //对于esp8266, 需要注册到ticker. 这是因为没freertos.
  btnTask.attach_ms(BTN_LOOPTASK_DELAY,looptask);
#else // ESP32将会使用FreeRTOS //////此处非常有可能会有bug
  xTaskCreatePinnedToCore([](void *){
    /*
    pinMode(21,OUTPUT);
    digitalWrite(21,LOW);
    pinMode(33,INPUT_PULLUP);
    pinMode(32,INPUT_PULLUP);
    pinMode(25,INPUT_PULLUP);
    btn_rd[0].setLongPressMs(1); //不识别双击三击, 只有按一下或者长按, 并且开启连按
    btn_rd[0].begin(33,rd_btn_f);
    btn_rd[0].setLongRepeatMode(1);
    btn_rd[1].enScanDT(1); //设置为多个按钮,不识别双击或三击
    btn_rd[1].begin(32,rd_btn_f);
    btn_rd[1].setLongRepeatMode(0);
    btn_rd[2].setLongPressMs(1); //不识别双击三击, 只有按一下或者长按, 并且开启连按
    btn_rd[2].begin(25,rd_btn_f);
    btn_rd[2].setLongRepeatMode(1);
    */
    for(;;){
      looptask();
      vTaskDelay(BTN_LOOPTASK_DELAY);
    }
  },"Button loopTask",BTN_LOOPTASK_STACK,NULL,BTN_LOOPTASK_PRIORITY,&btn_handle,BTN_LOOPTASK_CORE_ID);
#endif
  
  if(READGUY_bl_pin>=0) {
    pinMode(READGUY_bl_pin,OUTPUT); //这个是背光灯, 默认也支持PWM调节
    if(digitalPinHasPWM(READGUY_bl_pin)){
      currentBright = 128;
#if defined(ESP8266)
      analogWriteFreq(400); //8266不宜太快
      analogWriteResolution(8); //其实完全可以再快点的
      analogWrite(READGUY_bl_pin,currentBright);
#else
#if (defined ( ESP_IDF_VERSION_VAL ) && (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)))
      //ledcAttach(READGUY_bl_pin,8000,8);
      analogWrite(READGUY_bl_pin, currentBright);
      analogWriteFrequency(READGUY_bl_pin, 8000); //esp32宜快一些, 减少频闪影响
      analogWriteResolution(READGUY_bl_pin, 8); //其实完全可以再快点的
#else
      ledcSetup(0, 8000, 8);
      ledcAttachPin(READGUY_bl_pin, 0);//稍后加入PWM驱动...
      ledcWrite(0, currentBright);
#endif
#endif
    }
    else {
      currentBright=-1; //引脚不支持PWM,设置为亮起
      pinMode(READGUY_bl_pin, OUTPUT);
      digitalWrite(READGUY_bl_pin,HIGH);
    }
  }  //关于按键策略, 我们在此使用多个Button2的类, 然后在一个task共享变量来确定上一个按键状态
}
#ifdef READGUY_ENABLE_SD
fs::FS &ReadguyDriver::guyFS(uint8_t initSD){
  if(initSD==2 || (!READGUY_sd_ok && initSD)) setSDcardDriver();
  if(READGUY_sd_ok){
#ifdef ESP8266
    return SDFS;
#else
    return SD;
#endif
  }
#ifdef READGUY_USE_LITTLEFS
  return LittleFS;
#else
  return SPIFFS;
#endif
}
#else
fs::FS &ReadguyDriver::guyFS(uint8_t initSD){
  (void)initSD; //avoid GCC warning
#ifdef READGUY_USE_LITTLEFS
  return LittleFS;
#else
  return SPIFFS;
#endif
}
#endif
void ReadguyDriver::setBright(int d){
  if(currentBright>=0 && d>=0 && d<=255){
    currentBright=d;
#if ((defined (ESP8266)))
    analogWrite(READGUY_bl_pin,d);
#else
#if (defined ( ESP_IDF_VERSION_VAL ) )
#if ( (ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(5, 0, 0)))
    analogWrite(READGUY_bl_pin,d);
#else
    ledcWrite(0, d);
#endif
#endif
#endif
  }
  else if(currentBright>=-2 && currentBright<0){ //-1为不支持PWM的亮起,-2为不支持PWM的熄灭
    currentBright=d?-1:-2;
    pinMode(READGUY_bl_pin, OUTPUT);
    digitalWrite(READGUY_bl_pin,d?HIGH:LOW);
  }
}
void ReadguyDriver::display(uint8_t part){
  //真的是我c++的盲区了啊....搜索了半天才找到可以这么玩的
  //......可惜'dynamic_cast' not permitted with -fno-rtti
  // static bool _part = 0; 记忆上次到底是full还是part, 注意启动时默认为full
  if(READGUY_cali==127){
    part = refresh_begin(part);
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    guy_dev->_display((const uint8_t*)getBuffer(),((part>>1)?part>>1:3));
    //in_release(); //恢复
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
  }
}
void ReadguyDriver::displayBuffer(const uint8_t *buf, uint8_t part){
  if(READGUY_cali==127){
    part = refresh_begin(part);
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    epdPartRefresh++;
    guy_dev->_display(buf,((part>>1)?part>>1:3));
    //in_release(); //恢复
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
  }
}
void ReadguyDriver::display(std::function<uint8_t(int)> f, uint8_t part){
  if(READGUY_cali==127){
    part = refresh_begin(part);
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    guy_dev->drv_dispWriter(f,((part>>1)?part>>1:3));
    //in_release(); //恢复
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
  }
}
void ReadguyDriver::drawImage(LGFX_Sprite &base, LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw, int32_t zoomh) { 
  if(READGUY_cali==127) {
    refresh_begin(0);
    guy_dev->drv_drawImage(base, spr, x, y, 0, zoomw, zoomh); 
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
  }
}
void ReadguyDriver::drawImageStage(LGFX_Sprite &sprbase,LGFX_Sprite &spr,int32_t x,int32_t y,uint8_t stage,
  uint8_t totalstage,int32_t zoomw,int32_t zoomh) {
  if(READGUY_cali!=127 || stage>=totalstage) return;
#ifdef READGUY_SERIAL_DEBUG
  Serial.printf_P(PSTR("[Guy Draw] stage: %d/%d\n"),stage+1,totalstage);
#endif 
  refresh_begin(0);
  guy_dev->drv_drawImage(sprbase, spr, x, y, (totalstage<=1)?0:(stage==0?1:(stage==(totalstage-1)?3:2)),zoomw,zoomh);
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
}
void ReadguyDriver::setDepth(uint8_t d){ 
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling()) {
//    refresh_begin(0);
    if(d==0 || d>15) { d=15; } //invalid. set to default value 15.
    guy_dev->drv_setDepth(d); 
    current_depth = d;
//#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
//    refresh_end();
//#endif
  }
}
void ReadguyDriver::draw16grey(LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw,int32_t zoomh){
  if(READGUY_cali!=127) return;
  refresh_begin(0);
  if(guy_dev->drv_supportGreyscaling() && (spr.getColorDepth()&0xff)>1)
    return guy_dev->drv_draw16grey(*this,spr,x,y,zoomw,zoomh);
  guy_dev->drv_drawImage(*this, spr, x, y, 0, zoomw, zoomh);
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    refresh_end();
#endif
}
void ReadguyDriver::draw16greyStep(int step){
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling() && step>0 && step<16 ){
    if(step==1) {
      refresh_begin(0);
      guy_dev->drv_fullpart(1);
    }
    guy_dev->drv_draw16grey_step((const uint8_t *)this->getBuffer(),step);
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    if(step>=15) refresh_end();
#endif
  }
}
void ReadguyDriver::draw16greyStep(std::function<uint8_t(int)> f, int step){
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling() && step>0 && step<16 ){
    if(step==1) {
      refresh_begin(0);
      guy_dev->drv_fullpart(1);
    }
    guy_dev->drv_draw16grey_step(f,step);
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
    if(step>=15) refresh_end();
#endif
  }
}
void ReadguyDriver::invertDisplay(){
  if(READGUY_cali==127){
    const int pixels=((guy_dev->drv_width()+7)>>3)*guy_dev->drv_height();
    for(int i=0;i<pixels;i++)
      ((uint8_t*)(getBuffer()))[i]=uint8_t(~(((uint8_t*)(getBuffer()))[i]));
  }
}
void ReadguyDriver::sleepEPD(){
  if(READGUY_cali==127) {
    if(READGUY_bl_pin>=0) {
      pinMode(READGUY_bl_pin, OUTPUT);
      digitalWrite(READGUY_bl_pin, LOW); //关闭背光灯, 节省电量
    }
    guy_dev->drv_sleep();
  }
}
uint8_t ReadguyDriver::refresh_begin(uint8_t freshType){
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
  if(refresh_state){
    refresh_state=0;
    refresh_press = 0x7f; //clear state
    for(uint8_t i=0;i<3;i++){ //修复老旧硬件 按键和DC引脚冲突
      bool flag = 0;
      if(config_data[15+i] == 0) break;
      int ipin = abs((int)config_data[15+i])-1;
      if(ipin == (int)READGUY_epd_dc){ //旨在解决部分老旧硬件共用DC引脚
        while(btn_rd[i].isPressedRaw() == ((int)config_data[15+i] > 0)) { //这里需要按下的状态
          flag = 1;
          yield();
        }
        if(flag) {
  #ifdef READGUY_SERIAL_DEBUG
          Serial.printf_P(PSTR("[Guy Pin] refresh_begin pin %d mode output\n"), READGUY_epd_dc);
  #endif //等待恢复到按键空闲电平 btn>0 代表低电平按下, 此时右边为1 btn<0 代表高电平按下
          delay(btn_rd[i].min_debounce); //去抖动
//#ifdef ESP8266
//        btnTask.detach();//暂时关闭任务, 避免引脚被设置为非法模式
//#else
//        vTaskSuspend(btn_handle);//暂时关闭任务, 避免引脚被设置为非法模式
//#endif
          spibz|=0x40; //set bit at 0x40
          refresh_press = i;
          //pinMode((uint8_t)READGUY_epd_dc, OUTPUT);
        }
      }
    }
  }
#endif
  if((freshType&1)==1){ //隶属于快刷的范畴, 计数+1
    if(epdPartRefresh >= epdForceFull) {
      epdPartRefresh = 0;
      return (freshType & 0xfe);
    }
    else {
      if(!epdPartRefresh) guy_dev->drv_setDepth(current_depth); //慢刷完成后的首次快刷, 保存上次的刷新颜色深度
      if(epdPartRefresh<0x7ffe) epdPartRefresh++;
    }
  }
  else epdPartRefresh = 0; //全刷, 重置计数器
  return freshType;
}
#if (defined(READGUY_ALLOW_DC_AS_BUTTON))
void ReadguyDriver::refresh_end(){
#ifdef READGUY_ALLOW_DC_AS_BUTTON
  //for(uint8_t i=15;i<=17;i++){ //修复老旧硬件 按键和DC引脚冲突
  //  if(config_data[i] == 0) break;
  //  int ipin = abs((int)config_data[i])-1;
  if(refresh_press != 0x7f && !refresh_state){ //旨在解决部分老旧硬件共用DC引脚的bug
#ifdef READGUY_SERIAL_DEBUG
    Serial.printf_P(PSTR("[Guy Pin] refresh_end pin %d\n"), READGUY_epd_dc);
#endif //pinMode((uint8_t)READGUY_epd_dc, INPUT_PULLUP);
    spibz&=0x3f; //reset bit at 0x40
//#ifdef ESP8266
//    btnTask.attach_ms(BTN_LOOPTASK_DELAY,looptask);
//#else
//    vTaskResume(btn_handle);
//#endif //开启任务后, 延时确保按键任务可以活跃而不是一直处于被暂停又刷屏的无限循环
    delay((BTN_LOOPTASK_DELAY+btn_rd[refresh_press].min_debounce)*2);
  }
  refresh_state = 1;
#endif
}
#endif
void ReadguyDriver::setAutoFullRefresh(int16_t frames){
  //epdPartRefresh = frames<0?-frames:0;
  if(frames<0) epdPartRefresh = -frames;
  if(frames) {
    epdForceFull = (frames<0?-frames:frames);
  }
  else {
    epdForceFull = 0x7fff;
  }
}
#ifdef ESP8266
void ReadguyDriver::recoverI2C(){
  if(READGUY_cali!=127) return;
  for(int i=13;i<=14;i++){ // READGUY_i2c_scl == config_data[14]; READGUY_i2c_sda == config_data[13];
    if(config_data[i] == 12 || config_data[i] == 13 || config_data[i] == 14) pinMode(config_data[i], SPECIAL);
    for(int j=15;j<=17;j++){ // READGUY_btn1 == config_data[15]; b2 == config_data[15]; b3== config_data[17];
      if(config_data[i] == abs((int)config_data[j])-1) pinMode(config_data[i], config_data[j]>0?INPUT_PULLUP:INPUT_PULLDOWN);
    }
  }
}
#endif
#if (defined(READGUY_INDEV_DEBUG))
void ReadguyDriver::nvs_init(){
}
void ReadguyDriver::nvs_deinit(){
}
bool ReadguyDriver::nvs_read(){
  return 1;
}
void ReadguyDriver::nvs_write(){
}
#elif (defined(ESP8266))
void ReadguyDriver::nvs_init(){
  EEPROM.begin(sizeof(config_data)+8+READGUY_ESP8266_EEPROM_OFFSET);
}
void ReadguyDriver::nvs_deinit(){
  EEPROM.commit();
  EEPROM.end();
}
bool ReadguyDriver::nvs_read(){
  char s[8];
  for(unsigned int i=0;i<
#ifdef DYNAMIC_PIN_SETTINGS
  sizeof(config_data)+
#endif
  8;i++){
    int8_t rd=(int8_t)EEPROM.read(READGUY_ESP8266_EEPROM_OFFSET+i);
#ifdef DYNAMIC_PIN_SETTINGS
    if(i>=8) config_data[i-8] = rd;
    else
#endif
      s[i]=(char)rd;
  }
#ifdef READGUY_SERIAL_DEBUG
  Serial.printf_P(PSTR("[Guy NVS] ReadGuy Ver " READGUY_VERSION " on " _READGUY_PLATFORM " Get EEPROM...%d\n"), config_data[0]);
#endif
  return !(strcmp_P(s,projname));
}
void ReadguyDriver::nvs_write(){
  for(unsigned int i=0;i<sizeof(config_data)+8;i++){
    EEPROM.write(READGUY_ESP8266_EEPROM_OFFSET+i,(uint8_t)(i<8?pgm_read_byte(projname+i):config_data[i-8]));
  }
}
#else
void ReadguyDriver::nvs_init(){
  nvsData.begin(projname);  //初始化NVS
}
void ReadguyDriver::nvs_deinit(){
  nvsData.end(); //用完NVS记得关闭, 省内存
}
bool ReadguyDriver::nvs_read(){ //此处需要处理一些有关I2C的内容
  if(!nvsData.isKey(tagname)) return 0; //没有这个键值
#ifdef DYNAMIC_PIN_SETTINGS
  size_t len=nvsData.getBytes(tagname,config_data,sizeof(config_data)); //读取的数据长度
  /*if(len<sizeof(config_data)){ //旧版本格式无法获取I2C相关数据, 设置为-1.
    for(int i=sizeof(config_data)-1;i>=15;i--)    //使用新版本格式来存储相关数据
      config_data[i]=config_data[i-2];
    READGUY_i2c_sda=-1;
    READGUY_i2c_scl=-1;
    nvsData.putBytes(tagname,config_data,sizeof(config_data)); //用新版本格式保存
  }*/
  return len==sizeof(config_data);
#else
  return 1;
#endif
}
void ReadguyDriver::nvs_write(){
  if(nvsData.isKey(tagname)) nvsData.remove(tagname);
  nvsData.putBytes(tagname,config_data,sizeof(config_data)); //正式写入NVS
}
#endif

uint8_t ReadguyDriver::getBtn_impl(){ //按钮不可用, 返回0.
  static unsigned long last=0;
  static unsigned long last2=0;
  uint8_t res1,res2,res3,res4=0;
  switch(READGUY_buttons){
    case 1:
      res1=btn_rd[0].read();
      if(res1 == 1) res4 |= 1; //点按
      else if(res1 == 2) res4 |= 4; //双击-确定
      else if(res1 == 3) res4 |= 8; //三击-返回
      else if(res1 == 4) res4 |= 2; //长按-向上翻页
      else if(res1 == 5) res4 |= 3; //单击后长按-新增操作(可以连按)
      break;
    case 2:
      res1=btn_rd[0].read(); //选项上下键 两个按钮引脚都读取
      res2=btn_rd[1].read(); //确定/返回键
//#if 1
      {
        bool newval=btn_rd[0].isPressedRaw();
        if(newval && last2) last2=0;
        else if(!(newval || last2)) last2=millis();  //捕获按钮松开的行为
        if(res1 && (millis()-last>=btn_rd[1].long_press_ms) && (!btn_rd[1].isPressedRaw())){
          //Serial.printf("[%9d] res 1 state: %d %d\n",millis(),longpresstest,pressedRawtest);
          res4 = (res1 == 1)?1:2;      //左键点按-向下翻页
        }
      }
//#endif
      /*
      uint32_t nowm = millis();
      if(res1 && nowm-last >= btn_rd[1].long_press_ms && (!btn_rd[1].isPressedRaw())){
        res4 = (res1 == 1)?1:2;      //左键点按-向下翻页
        last=nowm;
      }
      if(res2) {
        if(btn_rd[0].isPressedRaw()) res4 |= 3; //避免GCC警告(我常年喜欢-Werror=all
        else if(res2 == 1 && nowm>last) res4 |= 4; //右键点按-确定
        else if(res2 == 4 && nowm>last) res4 |= 8; //右键长按-返回
        last=nowm;
      }
      */
      if(res2) {
        unsigned long ts=millis();
        //Serial.printf("[%9lu] now last2: %lu, threshold %lu\n",ts,last2,ts-last2);
        if(btn_rd[0].isPressedRaw() || ts-last2<=20) { //2024.2.25新增:  20毫秒的去抖时间 防误判
          res4 |= 3; //避免GCC警告(我常年喜欢-Werror=all
        }
        else if(res2 == 1) res4 |= 4; //右键点按-确定
        else if(res2 == 4) res4 |= 8; //右键长按-返回
        last=ts;
      }
      if(res4==5 || res4==6) res4=3;
      break;
    case 3:
      res1=btn_rd[0].read();
      res2=btn_rd[1].read();
      res3=btn_rd[2].read();
      if(res1 && millis()-last >= btn_rd[1].long_repeat_ms && (!btn_rd[2].isPressedRaw())) res4 |= 2;
      if(res3) {
        res4 |= ((btn_rd[0].isPressedRaw()<<1)|1);
        last=millis();
      }
      //if(res3 && ((millis()-last)<btn_rd[0].long_repeat_ms)) res4 |=3;
      if(res2 == 1) res4 |= 4;
      else if(res2 == 4) res4 |= 8;
      else if(res2 == 2) res4 |= 3; //新增: 双击进入操作5
      break;
  }
  return res4;
}
void ReadguyDriver::looptask(){ //均为类内静态数据
  btn_rd[0].loop();
  btn_rd[1].loop();
  btn_rd[2].loop();
}
bool ReadguyDriver::screenshot(const char *filename){
#ifdef READGUY_ENABLE_SD
  if(!SDinside(false)) return 0;
  uint16_t ibytes = ((width()+31)>>3)&0x7ffcu; //必须是4的倍数
  uint16_t bmpHeader[] = { //0x3e == 62 字节
    0x4d42,       //[ 0]字符BM, 固定文件头
    0x0f7e,0,0,0, //[ 1]File Size 文件总大小   文件大小0x0f7e == 3966; 位图数据大小32*122 == 3904
    0x003e,0,     //[ 5]Bitmap Data Offset (BDO) 信息头到数据段的字节数 (索引到0x3e后的第一个数据就是位图字节数据)
    0x0028,0,     //[ 7]Bitmap Header Size (BHS) 信息头长度
    0x00fa,0,     //[ 9]宽度 0xfa == 250
    0x007a,0,     //[11]高度 0x7a == 122
    0x01,         //[13]Planes 位面数(图层数) 锁定为1
    0x01,         //[14]Bits Per Pixel (BPP) 每像素位数 (1bit/pixel)
    0,0,          //[15]Compression 压缩方法 压缩说明：0-无压缩
    0x0f40,0,     //[17]Bitmap Data Size	 位图数据的字节数。该数必须是4的倍数    0x0f40 == 3904 == 32*122
    0x0ec4,0,     //[19]Horizontal Resolution 水平分辨率 0x0ec4==3780 (pixel/metre)==96.012 (pixel/inch)
    0x0ec4,0,     //[21]Vertical Resolution   水平分辨率 0x0ec4==3780 (pixel/metre)==96.012 (pixel/inch)
    0,0,          //[23]Colours 位图使用的所有颜色数。为0表示使用 2^比特数 种颜色 如8-比特/像素表示为100h或者 256.
    0,0,          //[25]Important Colours 指定重要的色彩数。当该域的值等于色彩数或者等于0时，表示所有色彩都一样重要
    0,0,          //[27]重要颜色 0: 黑色 (#000000) R0 G0 B0 X0
    0xffff,0xff   //[29]重要颜色 1: 白色 (#ffffff) R255 G255 B255 X0
  };
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON))
  setSDbusy(1);
#endif
  File bmpf = guyFS().open(filename,"w");
  if(!bmpf) { 
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON))
    setSDbusy(0); 
#endif
    return 0; 
  }
  uint32_t datsz = ibytes * height();
  bmpHeader[ 1] = (datsz + sizeof(bmpHeader)) & 0xffffu;
  bmpHeader[ 2] = (datsz + sizeof(bmpHeader)) >> 16;
  bmpHeader[ 9] = width();
  bmpHeader[11] = height();
  bmpHeader[17] = datsz & 0xffffu;
  bmpHeader[18] = datsz >> 16;
  bmpf.write((uint8_t*)bmpHeader,sizeof(bmpHeader));
  uint8_t *byteWrite = new uint8_t [ibytes];
  for(int h=bmpHeader[11]-1;h>=0;h--){           //bmpHeader[11] == tft->height()
    for(uint16_t i=0;i<ibytes;i++) byteWrite[i] = 0;  //ibytes == (tft->width()+7)>>3
    for(uint16_t w=0;w<=bmpHeader[9];w++){            //bmpHeader[ 9] == tft->width()
      byteWrite[w>>3] |= !!(readPixel(w,h))<<((w&7)^7);
    }
    bmpf.write(byteWrite,ibytes);
    yield();
  }
  bmpf.close();
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON))
  setSDbusy(0); 
#endif
  delete [] byteWrite; //释放内存
  //bmpf = guyFS().open(filename,"r");
  //if(!bmpf || bmpf.size()!=datsz + sizeof(bmpHeader) || bmpf.peek() != 0x42) return 0; //检查
  //bmpf.close();
  return 1;
#else
  return 0;
#endif
}

uint8_t ReadguyDriver::rd_btn_f(uint8_t btn, bool activeLow){
  static uint8_t lstate=0; //上次从dc引脚读到的电平
#ifdef ESP8266
  if(btn==ReadguyDriver::pin_cmx && ReadguyDriver::spibz) return lstate;
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
  static uint8_t epdcsstate=0; //上次从epd_cs引脚读到的电平
  if(btn==ReadguyDriver::static_epd_cs && ReadguyDriver::spibz) return epdcsstate;
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
  static uint8_t sdcsstate=0; //上次从sd_cs引脚读到的电平
  if(btn==ReadguyDriver::static_sd_cs && ReadguyDriver::sd_cs_busy) return sdcsstate;
#endif
  if(btn==D5||btn==D6||btn==D7||btn==ReadguyDriver::pin_cmx
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
    || btn==ReadguyDriver::static_epd_cs
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
    || btn==ReadguyDriver::static_sd_cs
#endif
  ) 
    pinMode(btn,activeLow?INPUT_PULLUP:INPUT_PULLDOWN);//针对那些复用引脚做出的优化
  uint8_t readb = digitalRead(btn);
  if(btn==ReadguyDriver::pin_cmx
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
    || btn==ReadguyDriver::static_epd_cs
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
    || btn==ReadguyDriver::static_sd_cs
#endif
  ) {
    //Serial.printf("rd D1.. %d\n",spibz);
    pinMode(btn,OUTPUT); //如果有复用引脚, 它们的一部分默认需要保持输出状态, 比如复用的DC引脚
    digitalWrite(btn,HIGH);    //这些引脚的默认电平都是高电平
    if(btn==ReadguyDriver::pin_cmx) lstate = readb;
#ifdef READGUY_ALLOW_EPDCS_AS_BUTTON
    else if(btn==ReadguyDriver::static_epd_cs) epdcsstate = readb;
#endif
#if (defined(READGUY_ALLOW_SDCS_AS_BUTTON) && defined(READGUY_ENABLE_SD))
    else if(btn==ReadguyDriver::static_sd_cs) sdcsstate = readb;
#endif
  }
  else if(btn==D5||btn==D6||btn==D7) pinMode(btn,SPECIAL); //针对SPI引脚进行专门的优化
  return readb;
#else //ESP32不再允许SPI相关引脚复用
  if(btn!=ReadguyDriver::pin_cmx)
    return digitalRead(btn);
  if(spibz) return lstate;
  pinMode(btn,activeLow?INPUT_PULLUP:INPUT_PULLDOWN);
  uint8_t readb = digitalRead(btn);
  pinMode(btn,OUTPUT);
  digitalWrite(btn,HIGH);
  lstate = readb;
  return readb;
#endif
} /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
