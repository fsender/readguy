/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex03_buttons.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.2 增加了新的手势功能
 * 
 * @date created: 2023-10-20    modify: 2024-02-25   last modify: 2024-03-11
 * @brief ReadGuy 按键功能演示. ReadGuy自带的按键驱动程序是非常好用的
  以下内容为按键个数与手势操作的对应关系.
  //行为         下一个     上一个     确定      返回/退出    特殊操作(如切换输入法)
  //1个按键 返回 1=点按     2=双击    4=长按     8=三击      3=点按后接长按
  //2个按键 返回 1=左键点按 2=左键长按 4=右键点按 8=右键长按  3=按住左键点按右键
  //3个按键 返回 1=右键点按 2=左键点按 4=中键点按 8=中键长按  3=中间按键双击(需手动开启)

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
int c=1;
void setup(){
  Serial.begin(115200); //初始化串口
  guy.init(); //初始化readguy_driver 基础驱动库. 尽管初始化过程会刷屏, 但此示例不会用到屏幕.
  if(guy.width()<200) guy.setRotation(1); //对于小屏幕, 设置横向
  c = guy.getButtonsCount(); //此函数用于返回设备有多少个按键.
  
  Serial.println(F("[readguy] Button demo")); //显示文本 默认是不支持中文显示的.
  guy.println(F("Button demo."));
  guy.println(F("Press the buttons in any way!"));
  guy.println(F("Information will show"));
  guy.println(F("on the EPD display."));
  guy.println();
  guy.print(F("Your device supports "));
  guy.printf_P(PSTR("%d button%c.\r\n"),c,c==1?' ':'s'); //显示文本
  guy.display();//刷新墨水屏.
  //guy.setButtonSpecial(true); //对于拨轮, 无法做到按住按钮1按下按钮2
}

void loop(){
  auto val = guy.getBtn(); //此函数用于获取按键状态 没有按键按下时 返回0.

  if(val!=guyBNone){
    guy.fillRect(0,guy.height()-10, guy.width(),10,1);
    guy.setCursor(2,guy.height()-10);
    switch (val){
      case guyBNext: //下一个  手势
        if(c==1)      guy.println(F("key single clicked!"));
        else if(c==2) guy.println(F("Left key clicked!"));
        else if(c==3) guy.println(F("Right key clicked!"));
      break;
      case guyBPrev: //上一个  手势
        if(c==1)      guy.println(F("key long pressed!"));
        else if(c==2) guy.println(F("Left key long pressed!"));
        else if(c==3) guy.println(F("Left key clicked!"));
      break;
      case guyBSpecial: //特殊  手势
        if(c==1)      guy.println(F("key clicked and pressed!"));
        else if(c==2) guy.println(F("Right clicked at left pressing!"));
        else if(c==3) guy.println(F("Centre key double clicked!"));
      break;
      case guyBOK: //确定 手势
        if(c==1)      guy.println(F("key double clicked!"));
        else if(c==2) guy.println(F("Right key clicked!"));
        else if(c==3) guy.println(F("Centre key clicked!"));
      break;
      case guyBCancel: //返回  手势
        if(c==1)      guy.println(F("key triple clicked!"));
        else if(c==2) guy.println(F("Right key long pressed!"));
        else if(c==3) guy.println(F("Centre key long pressed!"));
      break;
      default: //未知手势
        guy.println(F("Unknown gesture!"));
      break;
    }
    guy.display();
    guy.scroll(0,-8);
  }
  delay(10);
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */