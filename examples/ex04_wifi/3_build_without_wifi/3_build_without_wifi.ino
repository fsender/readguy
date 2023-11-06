/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file 3_build_without_wifi.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-11-06
 * @brief ReadGuy最基础的HelloWorld显示, 但不编译WiFi库.
 * 
 * @note 打开方式: 
 * 1. 深入库文件夹, 找到guy_driver_config.h文件
 * 2. 找到宏定义 READGUY_ENABLE_WIFI
 * 3. 注释掉这一行, 就像这样
 * ```C++
 * /// 启用WIFI配网功能.必须先启用 #define DYNAMIC_PIN_SETTINGS. 此选项对 ESP32xx 会减少大量flash.
 * //#define READGUY_ENABLE_WIFI
 * ```
 * 4. 回到这里编译运行.
 * 5. 其实那里的很多功能都能被禁用掉, 直接注释对应的功能定义即可
 * 但我懒得测试哪些注释掉之后会导致编译不过去, 或是, 鬼找上门.
 * 6. 运行完本示例之后想要跑其他示例, 记得注释回去, 免得有些功能用不了
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

ReadguyDriver guy;//新建一个readguy对象, 用于显示驱动.

void setup(){

//                                           --------------------- 1 - 初始化和启动ReadGuy -------<
  Serial.begin(115200); //初始化串口

  //注意: 首次烧录此程序时, 这一步需要你打开手机联esp8266/32的 WiFi, 用于配网.
  //名称是 readguy 密码 12345678. 连接后访问 192.168.4.1 再在网页中完成后续步骤
  //后续启动就可以不用这个配置了
  guy.init(); //初始化readguy_driver 基础驱动库.

  //首次初始化完成之后, 以后再初始化就不需要配网了, 除非你抹除了芯片的flash

  guy.setFont(&FreeMonoBold9pt7b);  //设置显示的字体

  guy.setTextColor(0,1);  //设置显示的颜色. 0代表黑色, 1代表白色

  guy.drawString("Hello Readguy!",10,10); //用此函数将字符串显示到屏幕缓存内

  guy.drawString("Without WiFi!",10,30); //用此函数将字符串显示到屏幕缓存内

  //guy.setCursor(10,10);        //设置显示的坐标
  //guy.print("Hello Readguy!"); //使用这个函数也能显示出字符串, 但是需要提前使用setCursor确定显示坐标

  guy.display(true);   // 快速刷新. 将屏幕缓存内的内容显示到墨水屏幕上
  //guy.display(false); // 慢速刷新. 

  //想知道更多内容, 欢迎移步到其他示例.
}

void loop(){
  
  //什么也不做, 毕竟刷新墨水屏要消耗墨水屏的阳寿.
  //盖姐说它们也是有阳寿的. 刷多了会老化.

}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */