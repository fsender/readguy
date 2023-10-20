/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file 1_wifi_ap_server.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-09-29
 * @brief ReadGuy配网服务器演示.
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
void f3(server_t sv);

void setup(){
  Serial.begin(115200); //初始化串口
  guy.init(); //初始化readguy_driver 基础驱动库. 尽管初始化过程会刷屏, 但此示例不会用到屏幕.
  
  event_t server_event[3]={ //设置一个服务器响应回调函数的容器.
    {"按钮1","/btn1",f1}, //需要向容器的第一个元素中添加链接名称, 链接响应和链接回调函数
    {"按钮2","/btn2",f2}, //链接响应必须以斜线字符开头
    {"","/btn3",f3} //将HTML响应的事件名改为空白字符串, 就不会显示进入此链接的按钮, 只能通过其他页面跳转
  }; //这里不太好理解, 不过看完示例应该就会了

  guy.ap_setup(); //初始化WiFi AP模式 (可以理解为路由器模式)

  guy.server_setup(String(F("配网服务器演示:可以放置自己的链接和回调函数")),server_event,3); //初始化服务器.
  //这些服务器响应回调函数会打包进入初始化参数列表中.
  //上方的字符串可以在用户访问主页时, 显示在主页的第二行.(作为通知显示, 但并不是通知)
}
void loop(){
  guy.server_loop(); //让服务器一直运行
}

// 以下演示了如何向配网服务器添加回调函数.
//其中, sv 参数指向了一个服务器类型的变量. 当有来自客户端的请求时, 需要通过sv来发送响应消息.

void f1(server_t sv){ //使用PSTR来减少对内存的消耗(不加PSTR()或者F()则字符串会存到.rodata,占用宝贵的内存)
  sv->send_P(200, PSTR("text/html"), PSTR("<html><body><meta charset=\"utf-8\">按钮1服务函数</body></html>"));
} //于此相应, 使用send_P函数而不是send函数来发送数据

void f2(server_t sv){ //使用 R"EOF()EOF" 括起来的字符串, (不含括号本身) 其中的字符不需要转义, 包括回车换行
  sv->send_P(200, PSTR("text/html"), PSTR(R"EOF(<html>
  <body>
  <meta charset="utf-8">按钮2服务函数 <a href="/btn3">按钮3</a></body></html>)EOF"));
} //即使是缩进的空格也会被包含到字符串内, 此外, 引号等符号也不需要转义符.

void f3(server_t sv){ //此函数不会直接在首页中显示链接, 只能通过btn2中的链接跳转到本页面.
  sv->send_P(200, PSTR("text/html"), PSTR("<html><body><meta charset=\"utf-8\">按钮3服务函数</body></html>"));
}

/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */