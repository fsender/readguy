/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file 2_wifi_config.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-10-14
 * @brief ReadGuy配网服务器 配置并连接附近的WiFi网络演示程序.
 编译烧录后, 本程序将使用AP方式配网并在连接到网络时访问NTP服务器来在墨水屏上显示时间.
 同时开启在STA上的服务器, 供这个WiFi上的用户访问此墨水屏阅读器.

// 注意, 为了避免此项目占用的flash空间过大, 故库内中不再提供配网的相关功能函数.
// 此示例程序提供了配网相关的方法, 在实际程序中, 请自行编写.

// ******** 在进行此示例之前, 不要将 DYNAMIC_PIN_SETTINGS 和 READGUY_ENABLE_WIFI 注释掉. ********
// ******************************** 此示例需要用到 WiFi 的特性. ********************************
 * 
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

//在这里包含程序需要用到的库函数

#include <Arduino.h> //arduino功能基础库. 在platformIO平台上此语句不可或缺
#include "readguy.h" //包含readguy_driver 基础驱动库

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

typedef ReadguyDriver::ReadguyWebServer* server_t; //类型名太长太繁琐, 使用typedef减短
typedef ReadguyDriver::serveFunc         event_t ; //存储一个WiFi功能事件.

void f1(server_t sv); //服务器响应回调函数. 当启动AP配网服务器时, 这些函数将会被调用
void f2(server_t sv);
time_t getNTPTime();  //NTP获取时间的函数

int conf_status = 0;  //标记WiFi配网状态: 当此值为1时, 说明配网程序收到了WiFi SSID和密码信息, 尝试连接.
                      //此变量为2 说明配网成功了. 连接到了WiFi并显示当前时间.
int scanres = 0;      //WiFi扫描结果数量
String recv_ssid = "";//接收到的WiFi SSID
String recv_psk = ""; //接收到的WiFi 密码

void setup(){
  Serial.begin(115200); //初始化串口
  guy.init(); //初始化readguy_driver 基础驱动库. 尽管初始化过程会刷屏, 但此示例不会用到屏幕.
  
  Serial.println(F("[readguy] WiFi Configure demo")); //显示文本 默认是不支持中文显示的.
  guy.println(F("WiFi Configure demo")); //显示文本 默认是不支持中文显示的.
  guy.display();//刷新墨水屏.

  event_t server_event[3]={ //设置一个服务器响应回调函数的容器. 使用方法详见wifi_ap_server示例
    {"配置WiFi","/wificfg",f1}, //配置WiFi的入口链接. 在AP模式下点击此链接将会进入配网功能页面.
    {"","/wifiok",f2}           //收到wifi连接请求后 发送的响应. 此函数调用之后会尝试切换到STA模式, 连接到WiFi.
  }; //本驱动库不会单独保存用户的WiFi SSID和WiFi密码信息

  WiFi.mode(WIFI_AP_STA); //扫描网络需要切换到STA模式进行扫描. 扫描完成之后将会切换到AP模式
  
  guy.server_setup(String(F("WiFi配网示例")),server_event,2); //初始化服务器.
  //这些服务器响应回调函数会打包进入初始化参数列表中.
  //上方的字符串可以在用户访问主页时, 显示在主页的第二行.(作为通知显示, 但并不是通知)

  do{ //没有连接到目标SSID时 一直循环连接.

    conf_status=0; //重置配置状态变量

    scanres = WiFi.scanNetworks(); //开始扫描网络
    
    Serial.println("[readguy] WiFi Scan OK."); //关闭服务器, 尝试连接, 连接成功之后将会在屏幕上显示
    guy.println("WiFi Scan OK.");    //连接失败则会重新进入循环
    guy.display();

    IPAddress local_IP(192,168,4,1); //设置本地AP的IP地址, 网关和子网掩码.
    IPAddress gateway(192,168,4,1);
    IPAddress subnet(255,255,255,0);
    WiFi.softAPConfig(local_IP, gateway, subnet); //注册一个WiFi AP(类比无线路由器), 可以被手机等设备连接
    WiFi.softAP("readguy","12345678"); //初始化WiFi AP模式 (可以理解为路由器模式)

    while(conf_status==0){
      guy.server_loop(); //让服务器一直运行 此函数总是返回true. 因此配网何时完成, 应在程序里设定, 适可而止
    }

    //guy.server_end(); //看来是接收到啊WiFi名称和密码消息了, 现在关闭服务器, 尝试连接到WiFi...

    Serial.println("[readguy] received SSID and PSK info"); //关闭服务器, 尝试连接, 连接成功之后将会在屏幕上显示
    guy.println("received SSID and PSK info");              //连接失败则会重新进入循环
    guy.display(); //显示到墨水屏

    WiFi.scanDelete(); //删除WiFi扫描数据
    scanres = 0; //设置扫描到的网络个数为0

    if(recv_ssid != "-"){ //检测SSID是否为空. 一般的网络SSID应该不会是这个名称吧...
      //WiFi.mode(WIFI_STA);
      WiFi.begin(recv_ssid,recv_psk); //启动STA模式, 然后尝试连接网络(当前维WIFI_AP_STA模式, 可以连接到网络)

      for(int i=0;i<1500;i++){ //等待15秒, 没连上就退出.
        if(WiFi.status() == WL_CONNECTED){ //一旦连接成功就退出循环
          conf_status=2; //标记为连接成功
          guy.print("Connected! IP:"); //连接成功! 显示消息然后退出循环
          guy.println(WiFi.localIP()); 
          Serial.print("[readguy] Connected!IP:"); //连接成功! 显示消息然后退出循环
          Serial.println(WiFi.localIP());
          guy.display();
          break;
        }
        delay(10);
      }
    }
    if(conf_status!=2){ //经过循环之后发现wifi并没连接上 显示连接失败的信息.
      guy.println("WiFi failed! AP and server restarted."); //连接成功! 显示消息然后退出循环
      Serial.println("[readguy] WiFi failed! AP and server restarted."); //连接成功! 显示消息然后退出循环
      guy.display();
    }

  }while(conf_status!=2); // conf_status==2说明连接上了
  
  WiFi.mode(WIFI_STA); //从WIFI_AP_STA模式切换到WIFI_STA模式, 不再提供readguy热点.

  guy.println("Getting NTP time..."); //连接成功之后尝试获取NTP时间
  Serial.println("[readguy] Getting NTP time..."); //连接成功之后尝试获取NTP时间
  guy.display();

  time_t now = getNTPTime(); //下方的函数演示了如何使用NTP来对时
  guy.println(ctime(&now));
  Serial.println(ctime(&now));
  guy.display();

  guy.server_setup("现在是联网的STA模式."); //如果没有调用server_end函数 连续调用server_setup将自动结束之前的服务器
}

void loop(){
  guy.server_loop();
}

// 以下演示了如何向配网服务器添加回调函数.
//其中, sv 参数指向了一个服务器类型的变量. 当有来自客户端的请求时, 需要通过sv来发送响应消息.

void f1(server_t sv){ //使用PSTR来减少对内存的消耗(不加PSTR()或者F()则字符串会存到.rodata,占用宝贵的内存)
  String webpage_html = F(
  "<!DOCTYPE html>"
  "<html lang='zh-cn'>"
  "<head>"
    "<meta charset='UTF-8'>"
    "<title>配置WiFi连接</title>"
  "</head>"
  "<body>"
    "<form name='input' action='/wifiok' method='GET'>"
      "<h2>wifi配置页面</h2>"
      "如需刷新WiFi列表, 请选择选项``重新扫描WiFi``并点击保存, 而不是刷新此页, 刷新此页不会刷新WiFi列表<hr/>"
      "wifi名称:<br/>"
  //    "<input type='text' name='ssid'>"
      "<select name='ssid'>"
  //      "<option value='0'>测试WiFi名称</option>" //在此放置你扫描到的所有WiFi名称
  //      "<option value='1'>1.54寸Lilygo</option>"
  );
  String webpage_html2 = F(
      "</select>"
      "<br/>"
      "wifi密码:<br/>"
      "<input type='text' name='psk'><br/>"
      "<input type='submit' value='保存'>"
    "</form>"
  "</body>"
  "</html>");
  
  for(int i=0;i<=scanres;i++){
    webpage_html += "<option value='";
    webpage_html += i;
    webpage_html += "'>";
    if(i<scanres) webpage_html += WiFi.SSID(i);
    else webpage_html += "重新扫描WiFi";
    webpage_html += "</option>";
  }
  sv->send_P(200, PSTR("text/html"), (webpage_html+webpage_html2).c_str());
} //于此相应, 使用send_P函数而不是send函数来发送数据

void f2(server_t sv){
  if(sv->hasArg("ssid")) {
    if((sv->arg("ssid")).toInt() == scanres){
      recv_ssid = "-"; //空白字符串 退出 重新扫描
    }
    else recv_ssid=WiFi.SSID((sv->arg("ssid")).toInt());
    if(sv->hasArg("psk")) {
      recv_psk=sv->arg("psk");
      if(recv_ssid!="") conf_status=1; //接收到了WiFi SSID信息和密码信息
    }
  }
  if(recv_ssid == "-"){
    sv->send_P(200, PSTR("text/html"), PSTR("<html><body><meta charset=\"utf-8\" http-equiv=\"refresh\""
    " content=\"8;url=/wificfg\">正在扫描WiFi, 8秒后自动跳转.</body></html>"));
  }
  else if(conf_status>=1){
    String s=F("<html><body><meta charset=\"utf-8\">配置成功,正在连接...<br/>WiFi名称:");
    s += recv_ssid;
    s += F("<br/>WiFi密码:");
    s += recv_psk;
    s += F("<br/>连接成功的消息会显示在墨水屏上.</body></html>");
    sv->send_P(200, PSTR("text/html"), s.c_str());
  }
  else sv->send_P(200, PSTR("text/html"), //没有给定SSID, 无法连接到WiFi.
    PSTR("<html><body><meta charset=\"utf-8\">配置失败,缺少信息</body></html>"));
}


    /*----------------- NTP code ------------------*/
WiFiUDP udp;
uint8_t packetBuffer[48];
const int16_t timeZone = 8; //Beijing
const int16_t localPort = 1337;
time_t get_ntp_time_impl(uint8_t _server)
{
  const char * ntpServerName[4] = {
    "ntp1.aliyun.com","time.windows.com","cn.ntp.org.cn","cn.pool.ntp.org"
  };
  char ntpHost[32];
  IPAddress ntpServerIP; // NTP server's ip address
  
  while (udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println(F("Transmit NTP Request"));
  // get a random server from the pool
  strncpy_P(ntpHost,ntpServerName[_server],31);
  ntpHost[31] = '\0';
  WiFi.hostByName(ntpHost, ntpServerIP);
  Serial.print(FPSTR(ntpServerName[_server]));
  Serial.write(':');
  Serial.println(ntpServerIP);

  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, 48);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  udp.beginPacket(ntpServerIP, 123); //NTP requests are to port 123
  udp.write(packetBuffer, 48);
  udp.endPacket();

  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = udp.parsePacket();
    if (size >= 48) {
      Serial.println("Receive NTP Response");
      udp.read(packetBuffer, 48);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * 3600;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

time_t getNTPTime(){
  time_t _now = 0;
  if(!WiFi.isConnected()) return 0;
  udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(localPort);
    for(int i=0;i<4;i++){//最多尝试10次对时请求
        _now=get_ntp_time_impl(i);
        if(_now) break; //成功后立即退出
        yield();
    }
    return _now;
}

/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */