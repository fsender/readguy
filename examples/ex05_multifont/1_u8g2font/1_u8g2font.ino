/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file 1_u8g2font.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-09-19
 * @brief ReadGuy功能演示.
 * 将根目录下的data文件夹 上传到LittleFS之后运行效果更佳
 * 或者可以准备一张SD卡,并准备在卡的根目录下放置data文件夹内的文件.
 * 用于演示BMP格式图片灰度显示.
 * 
 * @note 食用方法:
 * 首先, 你需要有一颗ESP8266或者ESP32开发板, 一个墨水屏, 以及1~3个按钮.
 * 如果有一张SD卡或者是屏幕前置光, 那就更好啦, 哈哈哈哈~
 * 不过如果你有一个成品阅读器, 那你可以社牛的问一下硬件的作者(一般在oshwhub能找到)
 * 问到哪个引脚对应哪个GPIO.
 * 其实看原理图就可以知道哪个GPIO连接到哪个外设了.
 * 
 * 其次, 你需要把开发板连接到电脑, 烧录这个程序. 克隆这个项目之后, 在platformIO里打开,
 * 直接编译! 上传! (用arduino IDE的, 把lib文件夹内的三个子文件夹拷贝到你的arduino库
 * 目录里, 一般在C:\Users\$username$\OneDrive\文档\Arduino\libraries)
 * 不成功的和不会的, 建议再看一遍ESP8266和ESP32项目的构建流程.
 * 
 * 烧录完成之后, 用手机连接设备的WiFi网络 "readguy". 密码12345678. 
 * 连接上之后访问 http://192.168.4.1 最后根据网页的提示操作就可以
 * 你需要知道你的哪个引脚对应哪个GPIO, 才能使用这个库 (带来的不便请谅解nia~)
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
#include <Arduino.h>    //arduino功能基础库. 在platformIO平台上此语句不可或缺
#include "readguy.h" //包含readguy_driver 基础驱动库

#include "ctg_u8g2_wqy12.h" //中文字体库

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

extern const uint8_t ctg_wqy9pt_chinese1[]; //声明中文字体文件

const lgfx::U8g2font cn_font(ctg_wqy9pt_chinese1); //U8G2格式中文字体转化为LGFX格式字体

void setup(){

  Serial.begin(115200); //初始化串口

  guy.init();

  guy.setFont(&cn_font);

  guy.setCursor(10,10);

  guy.print("中文");

  guy.display();
}

void loop(){

}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */