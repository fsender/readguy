/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file ex03_buttons.ino
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-10-20
 * @brief ReadGuy 按键功能演示. ReadGuy自带的按键驱动程序是非常好用的

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

void setup(){
  Serial.begin(115200); //初始化串口
  guy.init(); //初始化readguy_driver 基础驱动库. 尽管初始化过程会刷屏, 但此示例不会用到屏幕.
  //if(guy.width()<guy.height()) guy.setRotation(1); //横向
  
  Serial.println(F("[readguy] Button demo")); //显示文本 默认是不支持中文显示的.
  guy.println("Button demo"); //显示文本 默认是不支持中文显示的.
  guy.display();//刷新墨水屏.
}

void loop(){
  int val = guy.getBtn(); //此函数用于获取按键状态 没有按键按下时 返回0.
  //1个按键 返回 1=点按     2=双击    3=长按     4=三击
  //2个按键 返回 1=左键点按 2=左键长按 3=右键点按 4=右键长按
  //3个按键 返回 1=左键点按 2=右键点按 3=中键点按 4=中键长按

  if(val>0){
    int c = guy.getButtonsCount(); //此函数用于返回设备有多少个按键. [最近更新的函数]
    switch (val){
      case 1: 
        if(c==1) guy.println("key single clicked!");
        else if(c==2) guy.println("Left key clicked!");
        else if(c==3) guy.println("Left key clicked!");
      break;
      case 2:
        if(c==1) guy.println("key double clicked!");
        else if(c==2) guy.println("Left key long pressed!");
        else if(c==3) guy.println("Right key clicked!");
      break;
      case 4:
        if(c==1) guy.println("key long pressed!");
        else if(c==2) guy.println("Right key clicked!");
        else if(c==3) guy.println("Centre key clicked!");
      break;
      case 8:
        if(c==1) guy.println("key triple clicked!");
        else if(c==2) guy.println("Right key long pressed!");
        else if(c==3) guy.println("Centre key long pressed!");
      break;
    }
    guy.display();
  }
  delay(10);
}/* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */