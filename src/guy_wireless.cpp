/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_wireless.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy 无线网(WiFi) 功能源代码文件. 
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

static const PROGMEM char NOT_SUPPORTED[] = "(不支持此屏幕)";
static const PROGMEM char TEXT_HTML[] = "text/html";
static const PROGMEM char TEXT_PLAIN [] = "text/plain";
static const PROGMEM char args_name[23][8]={
  "share","epdtype","EpdMOSI","EpdSCLK","Epd_CS","Epd_DC","Epd_RST","EpdBusy",
  "SD_MISO","SD_MOSI","SD_SCLK","SD_CS","I2C_SDA","I2C_SCL",
  "btn_cnt","btn1","btn1c","btn2","btn2c","btn3","btn3c","bklight","rtc"
};
#ifdef READGUY_DEV_154A
  static const PROGMEM char NAME_guyDev154[]="1.54寸标准";
#else 
  static const char *NAME_guyDev154=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_154B
  static const PROGMEM char NAME_guyDev154_V2[]="1.54寸Lilygo";
#else 
  static const char *NAME_guyDev154_V2=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_213A
  static const PROGMEM char NAME_guyDev213[]="2.13寸汉朔价签";
#else 
  static const char *NAME_guyDev213=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_266A
  static const PROGMEM char NAME_guyDev266[]="2.66寸Vusion价签";
#else 
  static const char *NAME_guyDev266=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_213B
  static const PROGMEM char NAME_guyDev213M21[]="2.13寸三色";
#else 
  static const char *NAME_guyDev213M21=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_290A
  static const PROGMEM char NAME_guyDev290[]="2.9寸A01";
#else 
  static const char *NAME_guyDev290=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_290B
  static const PROGMEM char NAME_guyDev290_V2[]="2.9寸T94背光";
#else 
  static const char *NAME_guyDev290_V2=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_420A
  static const PROGMEM char NAME_guyDev420Hink[]="4.2寸HINK黑白价签";
#else 
  static const char *NAME_guyDev420Hink=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_420B
  static const PROGMEM char NAME_guyDev420WF[]="4.2寸WF丝印";
#else 
  static const char *NAME_guyDev420WF=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_370A
  static const PROGMEM char NAME_guyDev370[]="3.7寸高DPI墨水屏";
#else 
  static const char *NAME_guyDev370=NOT_SUPPORTED;
#endif
#ifdef MEPD_DEBUG_DISPLAY
  static const PROGMEM char NAME_epdLcd[]="测试用LCD屏 请勿选此项";
#else 
  static const char *NAME_epdLcd=NOT_SUPPORTED;
#endif
#ifdef READGUY_DEV_270B
  static const PROGMEM char NAME_guyDev270_V2[]="2.7寸佳显触摸墨水屏";
#else 
  static const char *NAME_guyDev270_V2=NOT_SUPPORTED;
#endif
const char *ReadguyDriver::epd_drivers_list[EPD_DRIVERS_NUM_MAX]={
  NAME_guyDev154, //此处的顺序必须和 guy_epaper_config.h 里面的一样
  NAME_guyDev154_V2,
  NAME_guyDev213,
  NAME_guyDev213M21,
  NAME_guyDev266,
  NAME_guyDev270_V2,
  NAME_guyDev290,
  NAME_guyDev290_V2,
  NAME_guyDev370,
  NAME_guyDev420Hink,
  NAME_guyDev420WF,
  NAME_epdLcd
};
#ifdef READGUY_ESP_ENABLE_WIFI
//static const char *index_cn_html;
//static const uint8_t faviconData[1150];

//x==63 -> *
//x==62 -> _
//#define R2CHAR(x) (((x)==63)?42:(((x)==62)?95:(((x)>=36)?((x)+61):(((x)>=10)?((x)+55):((x)+48)))))
void ReadguyDriver::ap_setup(){
  //初始化WiFi AP模式, 用于将来的连接WiFi 处于已连接状态下会断开原本的连接
  WiFi.mode(WIFI_AP);
  IPAddress local_IP(192,168,4,1);
  IPAddress gateway(192,168,4,1);
  IPAddress subnet(255,255,255,0);
  WiFi.softAPConfig(local_IP, gateway, subnet);
  WiFi.softAP("readguy","12345678");
  Serial.println(F("ap_setup SSID: readguy, Pass: 12345678"));
}
void ReadguyDriver::server_setup(const String &notify, const serveFunc *serveFuncs, int funcs){
  //启动WiFi服务器端, 这样就可以进行配网工作
  if(sfuncs!=-1) server_end(); //避免重复服务器setup
  httpUpdater.setup(&sv);
  sv.on("/",         HTTP_GET,  std::bind(&ReadguyDriver::handleInit     ,this));
  sv.on("/verify",   HTTP_POST, std::bind(&ReadguyDriver::handleInitPost ,this)); //此时已经完成了引脚初始化
  sv.on("/pinsetup", HTTP_GET,  std::bind(&ReadguyDriver::handlePinSetup ,this));
  sv.on("/final",    HTTP_POST, std::bind(&ReadguyDriver::handleFinalPost,this)); //此时验证已经正确
  //sv.on("/wifi",     HTTP_GET,  std::bind(&ReadguyDriver::handleWiFi     ,this)); //此时验证已经正确
  guy_notify=notify;
  sfuncs=funcs; //设置服务函数列表
  if(sfnames!=nullptr) { delete [] sfnames; delete [] sfevents; } //严防内存泄露
  if(sfuncs>0){
    sfnames=new String[sfuncs];
    sfevents=new String[sfuncs];
    for(int i=0;i<sfuncs;i++){   //set-up 第三方库内容, 初始化后即可使用
      sv.on(serveFuncs[i].event,HTTP_GET,std::bind(serveFuncs[i].func,&sv));
      sfnames[i]  = serveFuncs[i].linkname;
      sfevents[i] = serveFuncs[i].event;
    }
  }
  else { sfnames=nullptr; sfevents=nullptr; }


  /*sv.on("/favicon.ico", HTTP_GET, [&](){
    sv.client().write_P(PSTR("HTTP/1.1 200 OK\r\n"
                             "Content-Type: image/x-icon\r\n"
                             "Content-Length: 1150\r\n"
                             "Connection: close\r\n\r\n"),89);
    sv.client().write_P((const char *)faviconData,sizeof(faviconData));
  });*/
  sv.onNotFound(std::bind(&ReadguyDriver::handleNotFound,this)); //处理404的情况
  sv.begin();   
  MDNS.begin("readguy");
  //MDNS.addService("http","tcp",80);
  Serial.println(F("server_setup done! visit "));
  if(WiFi.getMode() == WIFI_AP) Serial.println(F("192.168.4.1"));
  else Serial.println(WiFi.localIP());
}
bool ReadguyDriver::server_loop(){ //此时等待网页操作完成响应...
  sv.handleClient();
  //delay(50);
  if((READGUY_cali&1)&&((READGUY_cali&0x1f)!=0x1f)){
    uint8_t got=getBtn_impl();
    uint8_t refFlag=127;
    if(got==1 && !(READGUY_cali&2)){
      READGUY_cali|=2;
      refFlag=0;
    }
    else if(got==2 && !(READGUY_cali&4)){
      READGUY_cali|=4;
      refFlag=1;
    }
    else if(got==4 && !(READGUY_cali&8)){
      READGUY_cali|=8;
      refFlag=2;
    }
    else if(got==8 && !(READGUY_cali&16)){
      READGUY_cali|=16;
      refFlag=3;
    }
    if(refFlag!=127) {
      Serial.printf("randch: %d %c\n",randomch[refFlag],(char)(randomch[refFlag]));
      drawChar((width()>>1)-46+refFlag*24,(height()>>1)-14,randomch[refFlag],true,false,4);
      guy_dev->drv_fullpart(1);
      guy_dev->_display((const uint8_t*)getBuffer());
    }
  }
  delay(1); //防止触发看门狗
  return (READGUY_cali==127);
}
void ReadguyDriver::server_end(){
  sv.stop();
  MDNS.end();
  sfuncs=-1;
  delete [] sfnames;
  delete [] sfevents;
  sfnames=nullptr;
  sfevents=nullptr;
}
void ReadguyDriver::handleInit(){
  if(READGUY_cali!=127) sv.send_P(200, TEXT_HTML, PSTR(
    "<meta http-equiv=\"refresh\" content=\"0;url=/pinsetup\">"));
  else handleFinal();
}
void ReadguyDriver::handleInitPost(){
  // 在此初始化你的数字引脚信息了, 之后就可以尝试初始化了.
  // 此时返回一个文本输入框, 定位到 handleFinalPost 函数
  uint8_t btn_count_=0;
  if(READGUY_cali){ //再次初始化已经初始化的东西, 此时需要关闭一些外设什么的
    Serial.println(F("Reconfig pins and hardwares..."));
    READGUY_cali=0;
    READGUY_sd_ok=0;
#if defined(ESP8266)
    //Esp8266无视SPI的设定, 固定为唯一的硬件SPI (D5=SCK, D6=MISO, D7=MOSI)
    SDFS.end();//关闭SD卡
    btnTask.detach();
#else
    SD.end();//关闭SD卡
    if(sd_spi != epd_spi) { //共线时, 不要删除SD
      delete sd_spi;
      sd_spi=nullptr;
    }
    vTaskDelete(btn_handle);
#endif
  }
  config_data[0]=1; //默认只要运行到此处, 就已经初始化好了的
  for(int i=0;i<23;i++){
    Serial.print(F("Argument "));
    Serial.print(FPSTR(args_name[i]));
    Serial.write(':');
    if(sv.hasArg(FPSTR(args_name[i]))) {
      Serial.println(sv.arg(FPSTR(args_name[i])));
      if(i<14){ //这12个引脚是不可以重复的, 如果有重复, config_data[0]设为0
        config_data[i+1] = sv.arg(FPSTR(args_name[i])).toInt();
      }
      else if(i==14) btn_count_=sv.arg(FPSTR(args_name[14])).toInt();
      else if(i==15&&btn_count_>0) config_data[15]=sv.arg(FPSTR(args_name[15])).toInt()+1;
      else if(i==16&&btn_count_>0) config_data[15]=-config_data[15];
      else if(i==17&&btn_count_>1) config_data[16]=sv.arg(FPSTR(args_name[17])).toInt()+1;
      else if(i==18&&btn_count_>1) config_data[16]=-config_data[16];
      else if(i==19&&btn_count_>2) config_data[17]=sv.arg(FPSTR(args_name[19])).toInt()+1;
      else if(i==20&&btn_count_>2) config_data[17]=-config_data[17];
      else if(i==21) config_data[18] = sv.arg(FPSTR(args_name[21])).toInt();
      else if(i==22) config_data[19] = sv.arg(FPSTR(args_name[22])).toInt(); //保留RTC功能
    }
    else {
      Serial.write('\n');
      if(i==0) READGUY_shareSpi = 0; //有的html响应是没有的.共享SPI默认值为true.
      else if(i<14) config_data[i+1] = -1;//这12个引脚是不可以重复的, 如果有重复, config_data[0]设为0
    }
  }
  //尝试初始化各个硬件, 可能失败, 然后显示一些东西
#if (defined(ESP8266) || defined(CONFIG_IDF_TARGET_ESP32C3))
  if(config_data[3]==-1) config_data[3] = config_data[10];
  else config_data[10] = config_data[3];
  if(config_data[4]==-1) config_data[4] = config_data[11];
  else config_data[11] = config_data[4];
  READGUY_shareSpi = true;
#else
  if(config_data[10] == config_data[3] && config_data[11] == config_data[4]) //检测到SPI共线
    READGUY_shareSpi = true;
  else if(READGUY_shareSpi){
    if(config_data[3]==-1) config_data[3] = config_data[10];
    else config_data[10] = config_data[3];
    if(config_data[4]==-1) config_data[4] = config_data[11];
    else config_data[11] = config_data[4];
  }
#endif
  uint8_t ck=checkEpdDriver();
  if(btn_count_<2) config_data[16]=0;
  if(btn_count_<3) config_data[17]=0;
  Serial.println(F("Config OK. Now init devices."));
  if(ck>=125) {
    const char *pNotify[3]={ PSTR("Necessary pin NOT connected."),\
    PSTR("Pin conflicted."),PSTR("Display not supported.") };
    sv.send_P(200, TEXT_PLAIN, pNotify[ck-125]);
    return;
  }
  String s=FPSTR(html_header); //这样省栈空间
  s += FPSTR(verify_html);
  s += FPSTR(verifybtn_html[btn_count_-1]);
  s += FPSTR(verify2_html);
  s += FPSTR(index_cn_html16);
  sv.send_P(200, TEXT_HTML, (s+FPSTR(end_html)).c_str());
  //发送数据已经录入的网页, 下次点击这个submit按键即将跳转到 handleFinalPost
  //此时显示一个随机字符串, 然后显示出来, 最后验证屏幕驱动状态
#if (defined(ESP8266))
  uint32_t rdm = os_random();  ///生成随机字符串
#else
  uint32_t rdm = esp_random(); ///生成随机字符串
#endif
  randomch[0] = 48+((rdm>>18)%10);//R2CHAR((rdm>>18)%10);
  randomch[1] = 48+((rdm>>12)%10);//R2CHAR((rdm>>12)%10);
  randomch[2] = 48+((rdm>> 6)%10);//R2CHAR((rdm>> 6)%10);
  randomch[3] = 48+((rdm    )%10);//R2CHAR((rdm    )%10);
  Serial.print(F("rand string: "));
  for(int i=0;i<4;i++) Serial.write(randomch[i]);
  Serial.write('\n');
  Serial.println(F("Init EPD..."));  //此时引脚io数据已经录入, 如果没有问题, 此处屏幕应当可以显示
  setEpdDriver(); //尝试初始化屏幕
  Serial.println(F("Init details..."));
  drawCenterString(setSDcardDriver()?"SD Init OK!":"SD Init failed!",width()>>1,(height()>>1)+20);
  setButtonDriver(); //初始化按钮..
  //} //尝试初始化按键, 调用后, 若SD卡初始化成功, READGUY_sd_ok的值会变成1
  drawRect((width()>>1)-46   ,(height()>>1)-14,20,28,0);
  drawRect((width()>>1)-46+24,(height()>>1)-14,20,28,0);
  drawRect((width()>>1)-46+48,(height()>>1)-14,20,28,0);
  drawRect((width()>>1)-46+72,(height()>>1)-14,20,28,0);
  spibz++;
  guy_dev->drv_fullpart(1);
  guy_dev->_display((const uint8_t*)getBuffer());
  spibz--;
  Serial.println(F("Display done!"));
  READGUY_cali=1; //显示初始化完成
}
void ReadguyDriver::handlePinSetup(){
  //JS技术不行见谅, 哈哈, 直接, 把网页文件切片
  String s  = FPSTR(html_header);
  s += FPSTR(index_cn_html); //s时最终的网页HTML, 存储到字符串内.
#ifdef ESP8266
#define DRIVER_TEMPLATE_N 1
#define DRIVER_TEMPLATE_ARRAY_L 13
  static const char *dem_args_name[DRIVER_TEMPLATE_ARRAY_L]={
    "et",args_name[4],args_name[5],args_name[6],args_name[7],args_name[11],args_name[14],
    args_name[15],args_name[17],args_name[19],args_name[21],args_name[12],args_name[13]
  };
  static const PROGMEM int dem_args_val[DRIVER_TEMPLATE_N][DRIVER_TEMPLATE_ARRAY_L]={
    { 6,15, 0, 2, 4, 5, 2, 3, 0,-1,-1,13,14},
    //{ 0,10, 9, 8, 7, 4, 3, 2, 3, 5, 6,SDA,SCL},
    //{ 0,15, 4, 2, 5,-1, 1, 0,-1,-1,-1,-1,-1} //微雪官方例程板子不支持SD卡, 也不支持I2C. 按钮为boot按钮
  };
  static const PROGMEM char driver_template_name[DRIVER_TEMPLATE_N][22]={
    "甘草半糖开发板"
  };
#elif (defined(CONFIG_IDF_TARGET_ESP32))
#define DRIVER_TEMPLATE_N 5
#define DRIVER_TEMPLATE_ARRAY_L 18
  static const char *dem_args_name[DRIVER_TEMPLATE_ARRAY_L]={
    "et",args_name[2],args_name[3],args_name[4],args_name[5],args_name[6],args_name[7],
    args_name[8],args_name[9],args_name[10],args_name[11],args_name[14],
    args_name[15],args_name[17],args_name[19],args_name[21],args_name[12],args_name[13]
  };
  static const PROGMEM int dem_args_val[DRIVER_TEMPLATE_N][DRIVER_TEMPLATE_ARRAY_L]={
//型号,MO,SCK,CS,DC,RST,BUSY,SDMI,SDMO,SDCK,SDCS,BTN,1,2,3,BLK,SDA,SCL
    { 0,23,18,19,16, 4,13, 2,15,14,26, 3,32,33,25,-1,21,22}, //甘草将 io0用作back按键, 此处不用了
    {11,23,18,14,27,33,-1,19,23,18, 4, 3,39,38,37,32,21,22}, //M5Stack使用测试用LCD显示
    { 0,14,13,15,27,26,25,-1,-1,-1,-1, 1, 0,-1,-1,-1,-1,-1}, //微雪开发板的唯一交互按钮为boot按键
    { 1,23,18, 5,19,27,38, 2,15,14,13, 2,32,36,-1,12,25,26}, //T-Watch墨水屏,32引脚上的按钮是高电平有效
    { 5,23,18, 5,17,16, 4, 2,15,14,13, 3,37,38,39,-1,-1,-1}  //T5墨水屏开发板无I2C总线
  };
  static const PROGMEM char driver_template_name[DRIVER_TEMPLATE_N][24]={
    "甘草MP3播放器","M5Stack-Core","微雪官方开发板","LilyGo T-Watch墨水屏","LilyGo T5开发板"
  };
#elif (defined(CONFIG_IDF_TARGET_ESP32S2))
#define DRIVER_TEMPLATE_N 0
#define DRIVER_TEMPLATE_ARRAY_L 18
/*
  static const char *dem_args_name[DRIVER_TEMPLATE_ARRAY_L]={
    "et",args_name[2],args_name[3],args_name[4],args_name[5],args_name[6],args_name[7],
    args_name[8],args_name[9],args_name[10],args_name[11],args_name[14],
    args_name[15],args_name[17],args_name[19],args_name[21],args_name[12],args_name[13]
  };
  static const PROGMEM int dem_args_val[DRIVER_TEMPLATE_N][DRIVER_TEMPLATE_ARRAY_L]={
    { 1, 2, 3, 4, 5, 6, 3, 8, 9,10,11,12,13},
    {10,12,11,10, 9, 8, 3, 6, 5, 4, 3, 2, 1}
  };
  static const PROGMEM char driver_template_name[DRIVER_TEMPLATE_N][28]={
    "甘草半糖板","微雪例程"
  };
  */
#elif (defined(CONFIG_IDF_TARGET_ESP32S3))
#define DRIVER_TEMPLATE_N 0
#define DRIVER_TEMPLATE_ARRAY_L 18
/*
  static const char *dem_args_name[DRIVER_TEMPLATE_ARRAY_L]={
    "et",args_name[2],args_name[3],args_name[4],args_name[5],args_name[6],args_name[7],
    args_name[8],args_name[9],args_name[10],args_name[11],args_name[14],
    args_name[15],args_name[17],args_name[19],args_name[21],args_name[12],args_name[13]
  };
  static const PROGMEM int dem_args_val[DRIVER_TEMPLATE_N][DRIVER_TEMPLATE_ARRAY_L]={
    { 1, 2, 3, 4, 5, 6, 3, 8, 9,10,11,12,13},
    {10,12,11,10, 9, 8, 3, 6, 5, 4, 3, 2, 1}
  };
  static const PROGMEM char driver_template_name[DRIVER_TEMPLATE_N][28]={
    "甘草半糖板","微雪例程"
  };
  */
#elif (defined(CONFIG_IDF_TARGET_ESP32C3))
#define DRIVER_TEMPLATE_N 0
#define DRIVER_TEMPLATE_ARRAY_L 16
/*
  static const char *dem_args_name[DRIVER_TEMPLATE_ARRAY_L]={
    "et",args_name[4],args_name[5],args_name[6],args_name[7],
    args_name[8],args_name[9],args_name[10],args_name[11],args_name[14],
    args_name[15],args_name[17],args_name[19],args_name[21],args_name[12],args_name[13]
  };
  static const PROGMEM int dem_args_val[DRIVER_TEMPLATE_N][DRIVER_TEMPLATE_ARRAY_L]={
    { 1, 2, 3, 4, 5, 6, 3, 8, 9,10,11,12,13},
    {10,12,11,10, 9, 8, 3, 6, 5, 4, 3, 2, 1}
  };
  static const PROGMEM char driver_template_name[DRIVER_TEMPLATE_N][28]={
    "甘草半糖板","微雪例程"
  };
  */
#endif
#if DRIVER_TEMPLATE_N
  s += F("<script>function bt(){var a=document.getElementById(\"e\").value;");
  for(int i=0;i<DRIVER_TEMPLATE_N;i++){
    s += String(F("if(a==")) + (i+1) + ')' + '{';
    for(int j=0;j<DRIVER_TEMPLATE_ARRAY_L;j++){
      s+=F("document.getElementById(\"");
      s+=FPSTR(dem_args_name[j]);
      s+=F("\").value=");
      s+=int(pgm_read_dword(dem_args_val[i] + j));
      s+=';';
    }
    s += '}';
  }
  s += F("}function ct(){document.getElementById(\"e\").value=0;}</script><select id=\"e\" n"
  "ame=\"epdTemplate\" onchange=\"bt()\"><option value=\"0\" selected>自定义线序</option>");
  for(int i=0;i<DRIVER_TEMPLATE_N;i++){
    s += F("<option value=\"");
    s += (i+1);
    s += F("\">");
    s += FPSTR(driver_template_name[i]);
    s += F("</option>");
  }
  s += F("</select><br/>");
#endif
  s += FPSTR(index_cn_html2); //epd型号部分嵌入的js代码
  for(int i=0;i<EPD_DRIVERS_NUM_MAX;i++){
    s += F("<option value=\"");
    s += i;
    s += '\"';
    if(READGUY_cali && READGUY_epd_type == i) s += F(" selected");
    s += '>';
    s += FPSTR(epd_drivers_list[i]);
    s += F("</option>");
  }
  s += F("</select>");
#if defined(ESP8266)
  for(int i=2;i<12;i++){
    if(i>=6 && i<=8) continue;
#elif defined(CONFIG_IDF_TARGET_ESP32C3)
  for(int i=2;i<12;i++){
#else
  for(int i=0;i<12;i++){
#endif
    s += F("<br/>");
    s += FPSTR(args_name[i+2]);
    s += F("<input type=\"number\" id=\"");
    s += FPSTR(args_name[i+2]);
    s += FPSTR(index_cn_html3);
    s += FPSTR(args_name[i+2]);
    s += ("\" min=\"-1\" max=\"99\" step=\"1\" value=\"");
    s += (READGUY_cali?(int)config_data[i+3] :-1);
    s += F("\"/>");
  }
  s += F("<br/>几个按键\?<input type=\"number\" id=\"btn_cnt");
  s += FPSTR(index_cn_html3);
  s += F("btn_cnt\" min=\"1\" max=\"3\" step=\"1\" value=\"");
  if(!(READGUY_cali && READGUY_btn2)) s += 1;
  else            if(!(READGUY_btn3)) s += 2;
  else                                s += 3;
  for(int i=0;i<4;i++){
    if(i){
      s += F("\"/><input type=\"checkbox");
      s += FPSTR(index_cn_html3);
      s += FPSTR(args_name[14+(i<<1)]);
      s += F("\" value=\"1\">高电平触发<br/>");
    } else s += F("\"/><br/>");
    if(i==3) s += F("背光 PWM");
    else { s += F("按键 "); s += i+1; }
    s += F(" 引脚<input type=\"number\" id=\"");
    s += FPSTR(args_name[15+(i<<1)]);
    s += FPSTR(index_cn_html3);
    s += FPSTR(args_name[15+(i<<1)]);
    s += F("\" min=\"-1\" max=\"99\" step=\"1\" value=\"");
    if(i==3) s += (READGUY_cali?(int)READGUY_bl_pin :-1);
    else s += ((READGUY_cali && config_data[15+i])?(int)abs(config_data[15+i])-1:-1);
  }
  s += FPSTR(index_cn_html16); //s += (READGUY_cali?(int)0        :-1);
  sv.send_P(200, TEXT_HTML, (s+FPSTR(end_html)).c_str());
}
void ReadguyDriver::handleFinal(){
  if(READGUY_cali<63){ //看来这里是验证成功了.
    sv.send_P(200, TEXT_PLAIN, PSTR("Not verified."));//验证成功. 验证失败会显示verify failed
    return;
  }
  String s = FPSTR(html_header);
  s += FPSTR(final_html);
  s += (READGUY_cali == 63)?FPSTR(afterConfig_html):FPSTR(home_html); //正常应当显示后面这个home_html
  s += guy_notify;
  s += F("</h3><hr/>");
  if(sfuncs>0){
    s+=F("以下链接来自<b>应用程序</b><br/>"); //换行
    for(int i=0;i<sfuncs && sfnames[i]!=emptyString;i++){
      s+=F("<a href=\"");
      s+=sfevents[i];
      s+=F("\">");
      s+=sfnames[i];
      s+=F("</a> ");
    }
    s+=F("<br/><hr/>"); //换行
  }
  if(!READGUY_sd_ok)  s+=F("SD卡不可用!!!<br/>");
#if (defined(ESP8266)) //此函数速度太慢了, 因此删掉不用了
  /*else{
    fs::FSInfo64 *sdInfo = new fs::FSInfo64; //获取SD数据大小
    //Serial.println("start get_info");
    SDFS.info64(*sdInfo);
    //Serial.println("end get_info");
    s+=F("SD card size: ");
    s+=(uint32_t)(sdInfo->totalBytes/1024);
    s+=F("KB, used size: ");
    s+=(uint32_t)(sdInfo->usedBytes/1024);
    s+=F("KB.<br/>");
    delete sdInfo;
  }*/
  else s+=F("SD卡已插入.<br/>"); //对于大容量(>2GB)卡, SDFS.info64(*sdInfo)函数调用速度太慢(17秒)
#else
  else{
    auto cardType = SD.cardType();
    s+=F("SD Card Type: ");
         if(cardType == CARD_MMC)  s+=F("MMC");
    else if(cardType == CARD_SD)   s+=F("SDSC");
    else if(cardType == CARD_SDHC) s+=F("SDHC");
    else s+=F("UNKNOWN");
    s+=F(", SD card size: ");
    s+=(uint32_t)(SD.cardSize()/1024);
    s+=F(", used size: ");
    s+=(uint32_t)(SD.usedBytes()/1024);
    s+=F("KB.<br/>");
  }
#endif
  s+=F("当前WiFi模式: ");
  s+=(WiFi.getMode()==WIFI_STA)?F("正常联网模式"):F("AP配网模式");
  s+=F(", IP地址: ");
  s+=WiFi.localIP().toString();
  s+=F("<br/>芯片型号: ");
#if (defined(ESP8266))
  s+=F("ESP-12E or 12F");
#else
  s+=ESP.getChipModel();
  s+=F(" Rev");
  s+=ESP.getChipRevision();
#endif
  s+=F(", 芯片闪存ID: ");
  char cbuf[20]="";
#if (defined(ESP8266))
  sprintf(cbuf, "0x%08x", ESP.getChipId());
#else
  uint64_t gotID;
  esp_flash_read_unique_chip_id(esp_flash_default_chip,&gotID);
  sprintf(cbuf, "%016llx", gotID);
#endif
  s+=cbuf;
  s+=F("<br/>闪存容量: ");
  s+=ESP.getFlashChipSize()/1024;
  s+=F("KB, MAC地址: ");
  s+=WiFi.macAddress();
#if (!defined(ESP8266))
  s+=F(", 当前可用内存: ");
  s+=esp_get_free_heap_size();
  s+=F("<br/>ESP-IDF版本: ");
  s+=esp_get_idf_version();
#endif
  s+=F("<br/><b>引脚定义:</b><br/>");
//在此显示屏幕型号和引脚定义信息
  for(int i=0;i<12;i++){
    s+=FPSTR(args_name[i+2]);
    s+=':';
    s+=config_data[i+3];
    if(i==6) s+=F("<br/>");
    else s+=' ';
  }
  s+=F("<br/>按键定义:");
  if(READGUY_btn1 && READGUY_buttons>=1){ s+=FPSTR(args_name[15]); s+=':'; s+=abs(READGUY_btn1-1); s+=' '; }
  if(READGUY_btn2 && READGUY_buttons>=2){ s+=FPSTR(args_name[17]); s+=':'; s+=abs(READGUY_btn2-1); s+=' '; }
  if(READGUY_btn3 && READGUY_buttons==3){ s+=FPSTR(args_name[19]); s+=':'; s+=abs(READGUY_btn3-1); s+=' '; }
  //s+=F("<br/>"); //换行 
  sv.send_P(200, TEXT_HTML, (s+FPSTR(end_html)).c_str());
  if(READGUY_cali == 63){
    Serial.println(F("Data saved to NVS."));
    READGUY_cali = 127;
    nvs_init();
    nvs_write();
    nvs_deinit();
  }
}
void ReadguyDriver::handleFinalPost(){
  //此时硬件初始化完毕了, 然后网页表单也填写完了, 就等此时验证
  if(sv.hasArg("t_verify")){ //验证字符  
    String s = sv.arg("t_verify");
    if(s[0]==randomch[0] && s[1]==randomch[1] && s[2]==randomch[2] && s[3]==randomch[3]){
      READGUY_cali |= 32;
      handleFinal(); //看来这里是验证成功了.
    }
    else{
      sv.send_P(200, TEXT_PLAIN, PSTR("Verify failed."));//验证成功. 验证失败会显示verify failed
      return;
    }
    //else sv.send_P(200, TEXT_PLAIN, PSTR("verify failed."));//验证成功. 验证失败会显示verify failed
  }
  else sv.send_P(200, TEXT_PLAIN, PSTR("No string input."));//验证成功. 验证失败会显示verify failed
}
void ReadguyDriver::handleNotFound(){
  sv.send_P(404, TEXT_PLAIN, PSTR("404 Not Found."));
} //设置WiFi和聚合天气的相关函数不再内置到库中, 请自己在示例中设置

//JS技术不行见谅
const PROGMEM char ReadguyDriver::html_header[] = 
"<!DOCTYPE html><html lang=\"zh-cn\"><head><meta charset=\"utf-8\"><title>";
const PROGMEM char ReadguyDriver::index_cn_html[] = // then write EpdMOSI pin
"readguy 设置</title></head><body><h1>readguy 设置页面</h1><p>感谢您使用ReadGuy. 现在"
"将引脚配置输入到框内, 即可成功点亮屏幕.</p><hr/><h2>引脚定义设定</h2><form "
"name=\"input\" action=\"/verify\" method=\"POST\">";
const PROGMEM char ReadguyDriver::index_cn_html2[] =
#if (!defined(ESP8266) && !defined(CONFIG_IDF_TARGET_ESP32C3))
"<input type=\"checkbox\" name=\"share\" value=\"1\">墨水屏和SD卡共享SPI<br/>"
#endif
"E-paper 型号<select id=\"et\" onchange=\"ct()\" name=\"epdtype\">";
const PROGMEM char ReadguyDriver::index_cn_html3[] = "\" onchange=\"ct()\" name=\"";
const PROGMEM char ReadguyDriver::index_cn_html16[] =// then write RTC type
"\"/><br/><input type=\'submit\' value=\'OK!\'/><br/></form><br/>";

const PROGMEM char ReadguyDriver::verify_html[] =
"readguy 初始化</title></head><body><h1>readguy 验证页面,马上就好</h1>"
"<p>屏幕即将显示.如果屏幕或者按键无响应请返回到<a href=\"/\">设置页面</a><br/>";
const PROGMEM char ReadguyDriver::verify2_html[] =
"<br/><hr/>完成上述4个操作之后屏幕上将会展现出验证码,输入验证码即可完成硬件配置.<br/></p><form action=\"/fin"
"al\" method=\"POST\"><input type=\'text\' name=\'t_verify\' maxlength=\"6";
const PROGMEM char ReadguyDriver::verifybtn_html[3][200] = {
"一个按键, 操作可能比较繁琐, 但功能还都可以的.<br/>"
"点按: 下一个/向下翻页<br/>双击: 上一个/向上翻页<br/>三连击: 返回/切换输入法<br/>长按: 确定/选择",
"两个按键, 操作可以满足需求.<br/>"
"按键1点按: 下一个/向下翻页<br/>按键1长按: 上一个/向上翻页<br/>按键2点按: 确定/选择<br/>按键2长按: 返回/切换输入法",
"三个按键, 操作非常流畅.<br/>"
"按键1: 上一个/向上翻页<br/>按键2点按: 确定/选择<br/>按键2长按: 返回/切换输入法<br/>按键3: 下一个/向下翻页"
};
const PROGMEM char ReadguyDriver::final_html[] =
"欢迎使用 readguy</title></head><body><h1>readguy ";
const PROGMEM char ReadguyDriver::afterConfig_html[] =
"配置完成</h1><p>您已完成了初始化工作.现在可以配置WiFi和天气密钥相关内容.<br/></p>"
"返回<a href=\"/pinsetup\">引脚设置</a><h3>";
const PROGMEM char ReadguyDriver::home_html[]=
"欢迎页面</h1>在这里您可以配置属于应用app的内容. <a href=\"/pinsetup\">重新配置引脚</a><h3>";
/*
const PROGMEM char ReadguyDriver::final2_html[] =
"<form action=\"/wifi\" method=\"POST\">WiFi 名称<input type=\'text\' name=\'ssid\' "
"maxlength=\"31\"/><br/>WiFi 密码<input type=\'text\' name=\'psk\' maxlength=\"31\"/>"
"<br/>天气所在地<input type=\'text\' name=\'ssid\' maxlength=\"31\"/><br/>聚合数据API密"
"钥<input type=\'text\' name=\'psk\' maxlength=\"63";
*/
const PROGMEM char ReadguyDriver::end_html[] = 
"<p>ReadGuy on device " _READGUY_PLATFORM "<br/>Copyright © FriendshipEnder <a href=\""
_GITHUB_LINK "\">GitHub</a> <a href=\"" _BILIBILI_LINK "\">Bilibili</"
"a><br/>版本: " READGUY_VERSION " ,编译日期: " __DATE__ " " __TIME__ "</p></body></html>";
/*const PROGMEM uint8_t ReadguyDriver::faviconData[1150]={
  0x0,0x0,0x1,0x0,0x1,0x0,0x10,0x10,0x0,0x0,0x1,0x0,0x20,0x0,0x68,0x4,0x0,0x0,0x16,0x0,0x0,0x0,0x28,0x0,
  0x0,0x0,0x10,0x0,0x0,0x0,0x20,0x0,0x0,0x0,0x1,0x0,0x20,0x0,0x0,0x0,0x0,0x0,0x40,0x4,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9d,0xaa,0xe8,0xff,0x74,0x73,0x77,0xff,0x74,0x73,
  0x77,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,
  0x77,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,0xe8,0xff,0x74,0x73,
  0x77,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,
  0xe8,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,
  0xe8,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,
  0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,
  0xe8,0xff,0x74,0x73,0x77,0xff,0x74,0x73,0x77,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,
  0xfe,0xff,0x74,0x73,0x77,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,
  0xe8,0xff,0x0,0x0,0x0,0x0,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,
  0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0xe8,0xed,0xfe,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,
  0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9d,0xaa,0xe8,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0x9,0x6b,0xd6,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,
  0xfe,0xff,0x9,0x6b,0xd6,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,
  0xfc,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,
  0xfe,0xff,0xe8,0xed,0xfe,0xff,0x5,0x43,0x86,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0x5,0x43,0x86,0xff,0xe8,0xed,
  0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,
  0x0,0x0,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,
  0xfe,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xe8,0xed,0xfe,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,
  0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0xc6,0xce,
  0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,
  0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0xc6,0xce,0xfc,0xff,0xc6,0xce,0xfc,0xff,0x9d,0xaa,0xe8,0xff,0x9d,0xaa,0xe8,0xff,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x3,0x0,0x0,0x0,0x1,0x0,0x0,0x0,0x1,
  0x0,0x0,0x0,0x1,0x0,0x0,0x80,0x1,0x0,0x0,0x80,0x0,0x0,0x0,0x80,0x1,0x0,0x0,0x80,0x1,0x0,0x0,0xc0,0x1,
  0x0,0x0,0xc0,0x3,0x0,0x0,0xe0,0x3,0x0,0x0,0xf0,0x7,0x0,0x0,0xfc,0x1f,0x0,0x0,0xf0,0xff,0x0,0x0
};*/
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */