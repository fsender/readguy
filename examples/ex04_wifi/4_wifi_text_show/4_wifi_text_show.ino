/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file 4_wifi_text_show.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2024-02-28
 * @brief ReadGuy通过wifi传输文本并显示.
// 注意, 为了避免此项目占用的flash空间过大, 故库内中不再提供配网的相关功能函数.
// 此示例程序提供了文本传输的功能, 可以通过网页端输入文本并显示到墨水屏上. 自适应字体大小.

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
#include "ctg_u8g2_wqy12.h" //中文字体库

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

//extern const uint8_t ctg_u8g2_wqy12_chinese1[]; //声明中文字体文件, 本字体只包含很少的字, 不建议用
extern const uint8_t ctg_u8g2_wqy12_gb2312[]; //声明中文字体文件, 本字体包含常用字, 体积较大
//extern const uint8_t ctg_u8g2_wqy12[]; //声明中文字体文件, 本字体包含绝大多数的字, 但体积也更大

const lgfx::U8g2font cn_font(ctg_u8g2_wqy12_gb2312); //U8G2格式中文字体转化为LGFX格式字体

typedef ReadguyDriver::ReadguyWebServer* server_t; //类型名太长太繁琐, 使用typedef减短
typedef ReadguyDriver::serveFunc         event_t ; //存储一个WiFi功能事件.

const PROGMEM char textShowHtml[]= R"EOF(<!DOCTYPE html>
<html lang=\"zh-cn\">
<head>
  <meta charset=\"utf-8\">
  <title>WiFi传文字</title>
</head>
<body>
  <h1>WiFi传文字 文本输入</h1>
  <p>输入想要显示到屏幕上的文字<br /></p>
  <form action="/showtext" method="GET"><input type='text' name='txt' placeholder="ReadGuy" maxlength="63" />
    <br /><input type='submit' value='确定显示!' /><br /></form><br />
  <p>ReadGuy<br />Copyright © FriendshipEnder
    <a href="https://github.com/fsender/readguy">GitHub</a>
    <a href="https://space.bilibili.com/180327370/">Bilibili</a></p>
</body>
</html>
)EOF"; //网页文本

void textShow(server_t sv); //服务器响应回调函数. 当启动AP配网服务器时, 这些函数将会被调用
void textShowGet(server_t sv);

void setup(){
  Serial.begin(115200); //初始化串口
  guy.init(); //初始化readguy_driver 基础驱动库. 尽管初始化过程会刷屏, 但此示例不会用到屏幕.
  if(guy.width()<guy.height()) guy.setRotation(1);
  guy.setFont(&cn_font);
  guy.println("Web一键传文本 正在启用热点...");
  guy.display();
  event_t server_event[2]={
    {"一键传文字","/textshow",textShow},
    {"","/showtext",textShowGet},
  };
  guy.ap_setup(); //初始化WiFi AP模式 (可以理解为路由器模式)

  guy.server_setup(String(F("配网服务器演示:可以放置自己的链接和回调函数")),server_event,2); //初始化服务器.
  //这些服务器响应回调函数会打包进入初始化参数列表中.
  //上方的字符串可以在用户访问主页时, 显示在主页的第二行.(作为通知显示, 但并不是通知)
  guy.println("名称:readguy 密码:12345678");
  guy.display();
}
void loop(){
  guy.server_loop(); //让服务器一直运行
}

// 以下演示了如何向配网服务器添加回调函数.
//其中, sv 参数指向了一个服务器类型的变量. 当有来自客户端的请求时, 需要通过sv来发送响应消息.

void textShow(server_t sv){ //点击链接即可出现发送文本框, 点击发送按钮即可将输入的文本显示到屏幕上
  sv->send_P(200, PSTR("text/html"), textShowHtml);
}
void textShowGet(server_t sv){ //注册Web服务函数回调 (就是显示接口)
  const String ok_str_1=F("<html><body><meta charset=\"utf-8\">"); //网页前半部分
  const String ok_str_2=F("<a href=\"/textshow\">重新传文字</a></body></html>"); //网页后半部分
  if(sv->hasArg("txt")){ //检查请求的报文 是否包含键值txt (详见前面的网页声明)
    String txt=sv->arg("txt"); //找到字段
    //-----------------showTextEpd(txt)------------------ //显示到墨水屏幕上
    guy.setTextSize(1); //先设置为默认字体大小, 方便后续计算
    int twidth = guy.textWidth(txt); //获取字符串在当前字体的宽度
    if(!twidth) { //宽度数值必须为非0
      sv->send(200, String(F("text/html")), ok_str_1+"只包含空格, 不显示. "+ok_str_2);
      Serial.println("Arg width == 0."); //字符串为空 或者总宽度为零
      return;
    }
    sv->send(200, String(F("text/html")), ok_str_1+"文字显示完成: "+txt+ok_str_2); //报告显示完成
    float tsize = ((float)guy.width())/twidth;    //计算字体大小, 此大小的目的是填满屏幕
    float fsize = ((float)guy.height())/guy.fontHeight(); //计算垂直方向的字体大小, 制定合适的显示方法
    if(tsize>fsize){ //字符太短, 字体大小取决于屏幕垂直高度
      guy.setTextSize(fsize);
    }
    else{ //字符可以顶到宽度
      guy.setTextSize(tsize, std::max(1.0f,tsize)); //显示的字体大小会根据文本动态变化
    } //水平方向太小的话, 垂直方向大小设置为1.0倍(字体原高度)
    guy.fillScreen(1);//清屏
    guy.setTextDatum(MC_DATUM); //居中显示
    guy.drawString(txt,guy.width()/2,guy.height()/2);//居中显示
    guy.display(READGUY_SLOW); //慢刷
    Serial.print("Show successful:"); //显示成功
    Serial.println(txt);
  }
  else{
    Serial.println("No arg."); //找不到txt字段参数
    sv->send(200, String(F("text/html")), ok_str_1+"显示失败:缺少参数 "+ok_str_2);
  }
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */