/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_driver.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 基础功能源代码文件. 
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

#include "readguy.h"
#if (!defined(INPUT_PULLDOWN))
#define INPUT_PULLDOWN INPUT
#endif

guy_button ReadguyDriver::btn_rd[3];
int8_t ReadguyDriver::pin_cmx=-1;

const PROGMEM char ReadguyDriver::projname[8] = "readguy";
const PROGMEM char ReadguyDriver::tagname[7] = "hwconf";
volatile uint8_t ReadguyDriver::spibz=0;
#ifndef DYNAMIC_PIN_SETTINGS
const int8_t ReadguyDriver::config_data[22] = {
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
  READGUY_sd_miso  ,// 目标sd卡的 MISO 引脚, sd_share_spi == 1 时无效
  READGUY_sd_mosi  ,// 目标sd卡的 MOSI 引脚, sd_share_spi == 1 时无效
  READGUY_sd_sclk  ,// 目标sd卡的 SCLK 引脚, sd_share_spi == 1 时无效
  READGUY_sd_cs    ,// 目标sd卡的 CS   引脚.
  READGUY_i2c_sda  ,// 目标i2c总线的SDA引脚, 当且仅当启用i2c总线时才生效
  READGUY_i2c_scl  ,// 目标i2c总线的SCL引脚, 当且仅当启用i2c总线时才生效
  //按键驱动部分, 为负代表高触发, 否则低触发,
  //注意, 这里的io编号是加1的, 即 1或-1 代表 gpio0 的低触发/高触发
  READGUY_btn1     , 
  READGUY_btn2     , 
  READGUY_btn3     , 
  READGUY_bl_pin   ,//前置光接口引脚IO
  READGUY_rtc_type ,//使用的RTC型号(待定, 还没用上)
  0                ,//READGUY_sd_ok   SD卡已经成功初始化
  0                 //READGUY_buttons 按钮个数, 0-3都有可能
};
#endif
#ifndef ESP8266
SPIClass *ReadguyDriver::sd_spi =nullptr;
SPIClass *ReadguyDriver::epd_spi=nullptr;
TaskHandle_t ReadguyDriver::btn_handle;
#endif

ReadguyDriver::ReadguyDriver(){
  READGUY_cali = 0; // config_data[0] 的初始值为0
  READGUY_sd_ok = 0; //初始默认SD卡未成功初始化
  READGUY_buttons = 0; //初始情况下没有按钮
} //WiFiSet: 是否保持AP服务器一直处于打开状态
uint8_t ReadguyDriver::init(uint8_t WiFiSet, bool initepd, bool initSD){
  if(READGUY_cali==127) //已经初始化过了一次了, 为了防止里面一些volatile的东西出现问题....还是退出吧
    return 0;
#ifdef DYNAMIC_PIN_SETTINGS
  nvs_init();
#if (!defined(INDEV_DEBUG))
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
#endif
#if (!defined(INDEV_DEBUG))
  }
  else{ //看来NVS有数据, //从NVS加载数据, 哪怕前面的数据刚刚写入, 还没读取
    if(WiFiSet>=2) WiFi.begin(); //连接到上次存储在flash NVS中的WiFi.
    else if(WiFiSet==1) ap_setup();
    if(checkEpdDriver()!=127) setEpdDriver(initepd/* ,g_width,g_height */);  //初始化屏幕
    else for(;;); //此处可能添加程序rollback等功能操作(比如返回加载上一个程序)
    if(initSD) setSDcardDriver();
    setButtonDriver();
  }
#endif
  nvs_deinit();
#else
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
  Serial.println(F("[Guy init] init done."));
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
  Serial.printf_P(PSTR("[Guy SPI] shareSpi? %d\n"),READGUY_shareSpi);
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
      Serial.println(F("[GUY ERR] EPD DRIVER IC NOT SUPPORTED!\n"));
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
  Serial.println(F("[Guy SPI] drvBase Init OK"));
  return READGUY_epd_type;
}
void ReadguyDriver::setEpdDriver(bool initepd, bool initGFX){
  guy_dev->spi_tr_release = in_release;
  guy_dev->spi_tr_press   = in_press;
  if(initepd) guy_dev->drv_init(); //初始化epd驱动层
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
  Serial.printf_P(PSTR("[Guy EPD] EPD init OK(%d): w: %d, h: %d\n"),guy_dev->drv_ID(),guy_dev->drv_width(),guy_dev->drv_height());
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
#if defined(ESP8266)
    //Esp8266无视SPI的设定, 固定为唯一的硬件SPI (D5=SCK, D6=MISO, D7=MOSI)
    SDFS.setConfig(SDFSConfig(READGUY_sd_cs));
    READGUY_sd_ok = SDFS.begin();
#else
    if(sd_spi == nullptr) {
#if (defined(CONFIG_IDF_TARGET_ESP32))
      sd_spi = new SPIClass(VSPI);
#else
      sd_spi = new SPIClass(FSPI); //ESP32S2和S3和C3 不支持VSPI, C3不支持FSPI
#endif
      sd_spi->begin(READGUY_sd_sclk,READGUY_sd_miso,READGUY_sd_mosi); //初始化SPI
    }
    READGUY_sd_ok = SD.begin(READGUY_sd_cs,*sd_spi,ESP32_SD_SPI_FREQUENCY); //初始化频率为20MHz
#endif
  }
  else READGUY_sd_ok=0; //引脚不符合规则,或冲突或不可用
  if(!READGUY_sd_ok){
    Serial.println(F("[Guy SD] SD Init Failed!"));
    //guyFS().begin(); //初始化内部FS
#ifdef READGUY_USE_LITTLEFS
    LittleFS.begin();
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
  LittleFS.begin();
#else
  SPIFFS.begin();
#endif
  return false;
}
#endif
void ReadguyDriver::setButtonDriver(){
  if(READGUY_btn1) { //初始化按键. 注意高电平触发的引脚在初始化时要设置为下拉
    int8_t btn_pin=abs(READGUY_btn1)-1;
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
  //初始化按钮, 原计划要使用Button2库, 后来发现实在是太浪费内存, 于是决定自己写
  if(READGUY_btn1) btn_rd[0].begin(abs(READGUY_btn1)-1,rd_btn_f,(READGUY_btn1>0));
  if(READGUY_btn2) btn_rd[1].begin(abs(READGUY_btn2)-1,rd_btn_f,(READGUY_btn2>0));
  if(READGUY_btn3) btn_rd[2].begin(abs(READGUY_btn3)-1,rd_btn_f,(READGUY_btn3>0));
  //if(READGUY_buttons==1){
    btn_rd[0].setLongRepeatMode(1); //允许连按
  //}
  if(READGUY_buttons==2){
    btn_rd[0].setMultiBtn(1); //设置为多个按钮,不识别双击或三击
    //btn_rd[0].setLongRepeatMode(1); //双按键 选择按键 设置为允许连按
    btn_rd[1].setMultiBtn(1); //设置为多个按钮,不识别双击或三击
    btn_rd[1].setLongRepeatMode(0);  //双按键 确定按键 设置为不允许连按
  }
  else if(READGUY_buttons==3){
    btn_rd[0].long_press_ms = 20; //不识别双击三击, 只有按一下或者长按, 并且开启连按
    //btn_rd[0].setLongRepeatMode(1);
  //btn_rd[1].setMultiBtn(1); //设置为多个按钮,不识别双击或三击    2024/2/25更新:需要支持连按适配拨轮
    btn_rd[1].setLongRepeatMode(0); //三按键 确定按键 设置为不允许连按
    btn_rd[2].long_press_ms = 20; //不识别双击三击, 只有按一下或者长按, 并且开启连按
    btn_rd[2].setLongRepeatMode(1);
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
    btn_rd[1].setMultiBtn(1); //设置为多个按钮,不识别双击或三击
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
      ledcSetup(0, 8000, 8);
      ledcAttachPin(READGUY_bl_pin, 0);//稍后加入PWM驱动...
      ledcWrite(0, currentBright);
#endif
    }
    else {
      currentBright=-1; //引脚不支持PWM,设置为亮起
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
#ifdef ESP8266
    analogWrite(READGUY_bl_pin,d);
#else
    ledcWrite(0, d);
#endif
  }
  else if(currentBright>=-2 && currentBright<0){ //-1为不支持PWM的亮起,-2为不支持PWM的熄灭
    currentBright=d?-1:-2;
    digitalWrite(READGUY_bl_pin,d?HIGH:LOW);
  }
}
void ReadguyDriver::display(uint8_t part){
  //真的是我c++的盲区了啊....搜索了半天才找到可以这么玩的
  //......可惜'dynamic_cast' not permitted with -fno-rtti
  // static bool _part = 0; 记忆上次到底是full还是part, 注意启动时默认为full
  if(READGUY_cali==127){
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    guy_dev->_display((const uint8_t*)getBuffer(),((part>>1)?part>>1:3));
    //in_release(); //恢复
  }
}
void ReadguyDriver::displayBuffer(const uint8_t *buf, uint8_t part){
  if(READGUY_cali==127){
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    guy_dev->_display(buf,((part>>1)?part>>1:3));
    //in_release(); //恢复
  }
}
void ReadguyDriver::display(std::function<uint8_t(int)> f, uint8_t part){
  if(READGUY_cali==127){
    //in_press(); //暂停, 然后读取按键状态 spibz
    guy_dev->drv_fullpart(part&1);
    guy_dev->drv_dispWriter(f,((part>>1)?part>>1:3));
    //in_release(); //恢复
  }
}
void ReadguyDriver::drawImage(LGFX_Sprite &base, LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw, int32_t zoomh) { 
  if(READGUY_cali==127) guy_dev->drv_drawImage(base, spr, x, y, 0, zoomw, zoomh); 
}
void ReadguyDriver::drawImageStage(LGFX_Sprite &sprbase,LGFX_Sprite &spr,int32_t x,int32_t y,uint8_t stage,
  uint8_t totalstage,int32_t zoomw,int32_t zoomh) {
  if(READGUY_cali!=127 || stage>=totalstage) return;
  //Serial.printf("stage: %d/%d\n",stage+1,totalstage);
  guy_dev->drv_drawImage(sprbase, spr, x, y, (totalstage<=1)?0:(stage==0?1:(stage==(totalstage-1)?3:2)),zoomw,zoomh);
}
void ReadguyDriver::setDepth(uint8_t d){ 
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling()) guy_dev->drv_setDepth(d); 
}
void ReadguyDriver::draw16grey(LGFX_Sprite &spr,int32_t x,int32_t y,int32_t zoomw,int32_t zoomh){
  if(READGUY_cali!=127) return;
  if(guy_dev->drv_supportGreyscaling() && (spr.getColorDepth()&0xff)>1)
    return guy_dev->drv_draw16grey(*this,spr,x,y,zoomw,zoomh);
  guy_dev->drv_drawImage(*this, spr, x, y, 0, zoomw, zoomh);
}
void ReadguyDriver::draw16greyStep(int step){
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling() && step>0 && step<16 ){
    if(step==1) guy_dev->drv_fullpart(1);
    guy_dev->drv_draw16grey_step((const uint8_t *)this->getBuffer(),step);
  }
}
void ReadguyDriver::draw16greyStep(std::function<uint8_t(int)> f, int step){
  if(READGUY_cali==127 && guy_dev->drv_supportGreyscaling() && step>0 && step<16 ){
    if(step==1) guy_dev->drv_fullpart(1);
    guy_dev->drv_draw16grey_step(f,step);
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
  if(READGUY_cali==127) guy_dev->drv_sleep();
}

#if (defined(INDEV_DEBUG))
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
  EEPROM.begin(32);
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
    int8_t rd=(int8_t)EEPROM.read(2+i);
#ifdef DYNAMIC_PIN_SETTINGS
    if(i>=8) config_data[i-8] = rd;
    else
#endif
      s[i]=(char)rd;
  }
  Serial.printf("[Guy NVS] Get EEPROM...%d\n", config_data[0]);
  return !(strcmp_P(s,projname));
}
void ReadguyDriver::nvs_write(){
  for(unsigned int i=0;i<sizeof(config_data)+8;i++){
    EEPROM.write(2+i,(uint8_t)(i<8?pgm_read_byte(projname+i):config_data[i-8]));
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

uint8_t ReadguyDriver::rd_btn_f(uint8_t btn){
  static uint8_t lstate=0; //上次从dc引脚读到的电平
#ifdef ESP8266
  if(btn==ReadguyDriver::pin_cmx && spibz) return lstate;
  if(btn==D5||btn==D6||btn==D7||btn==ReadguyDriver::pin_cmx) 
    pinMode(btn,INPUT_PULLUP);//针对那些复用引脚做出的优化
  uint8_t readb = digitalRead(btn);
  if(btn==ReadguyDriver::pin_cmx) {
    //Serial.printf("rd D1.. %d\n",spibz);
    pinMode(btn,OUTPUT); //如果有复用引脚, 它们的一部分默认需要保持输出状态, 比如复用的DC引脚
    digitalWrite(btn,HIGH);    //这些引脚的默认电平都是高电平
    lstate = readb;
  }
  else if(btn==D5||btn==D6||btn==D7) pinMode(btn,SPECIAL); //针对SPI引脚进行专门的优化
  return readb;
#else //ESP32不再允许SPI相关引脚复用
  if(btn!=ReadguyDriver::pin_cmx)
    return digitalRead(btn);
  if(spibz) return lstate;
  pinMode(btn,INPUT_PULLUP);
  uint8_t readb = digitalRead(btn);
  pinMode(btn,OUTPUT);
  digitalWrite(btn,HIGH);
  lstate = readb;
  return readb;
#endif
} /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
