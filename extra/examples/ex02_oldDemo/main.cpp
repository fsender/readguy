/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * 
 * @file oldDemo/main.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @version 1.0
 * @date 2023-09-19
 * @brief 旧版的功能演示.
 * 将根目录下的data文件夹 上传到LittleFS之后运行效果更佳
 * 或者可以准备一张SD卡,并准备在卡的根目录下放置data文件夹内的文件.
 * 用于演示BMP格式图片灰度显示.
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
#include <Arduino.h>
#include "guy_driver.h"
const char HelloArduino[] = "Hello Arduino!";
const char HelloEpaper[] = "Hello E-Paper!";

readguy_driver guy;
void helloWorld(int i,int j)
{
  bool full=1;
  if(i>=1000){
    i-=1000;
    full=0;
  }
  Serial.printf("[%lu] helloWorld: %d, %d\n",millis(),i,j);
  guy.setFont(&FreeMonoBold9pt7b);
  guy.setTextColor(0,1);
  //display.fillScreen(GxEPD_WHITE);
  guy.setCursor(i,j);
  guy.print("Hello");
  if(!full) guy.print("Full");
  //guy.g().drawString(HelloWorld,i,j);
  guy.display(full); // full update
  Serial.printf("[%lu] helloWorld done\n",millis());
}
//TaskHandle_t press2reset_h=nullptr;
void press2reset_f(void *pv){
  pinMode(36,INPUT_PULLUP);
  for(;;){
    if(!digitalRead(36)) ESP.restart();
    delay(10);
  }
}
void setup()
{
  Serial.begin(115200);
  Serial.println("setup");
  //xTaskCreate(press2reset_f,"press2reset",1024,nullptr,1,&press2reset_h);
  guy.init();
  //pinMode(32,INPUT);
  delay(100);
  // first update should be full refresh
  helloWorld(10,10);
  helloWorld(10,30);
  
  LGFX_Sprite sp(&guy);
  sp.createFromBmpFile(guy.guyFS(),"/test.bmp");
  Serial.printf("sp.w: %d, h: %d, res: %d.\n",sp.width(),sp.height(),ESP.getFreeHeap());
  guy.drawImage(sp,30,10);
  Serial.printf("drawn dithering bmp.\n");
  delay(2500);
  guy.draw16grey(sp,30,10);
  Serial.printf("drawn.\n");
  delay(5000);
  
  guy.fillScreen(1);
  helloWorld(1010,50);
  helloWorld(10,70);
  guy.fillScreen(1);
  helloWorld(10,90);
  guy.fillScreen(1);
  helloWorld(10,140);
  guy.fillScreen(1);
  guy.drawChar(10,40,'a',true,false,2);
  helloWorld(10,160);
  helloWorld(1010,10);
  helloWorld(1040,30);
  guy.sleepEPD();
  delay(2000);
  helloWorld(1010,50);
  helloWorld(20,70);
  helloWorld(30,90);
  guy.fillScreen(1);
  helloWorld(40,110);
  guy.setDepth(10); //设置颜色深度
  helloWorld(38,126);
  guy.setDepth(6);
  helloWorld(36,142);
  guy.setDepth(2);
  helloWorld(38,158);

  guy.setDepth(0); //恢复正常颜色深度
  guy.fillScreen(1);
  guy.display(0); //慢刷清屏
  guy.setFont(&fonts::Font0);
  for(int i=1;i<16;i++){
    guy.setDepth(i); //灰度测试
    guy.fillRect(10,i*10,20,10,0);
    guy.setCursor(32,i*10);
    guy.printf("Grey%d",i);
    guy.display();
  }
  delay(3000);
  guy.fillScreen(0);
  guy.display();
  guy.drawLine(0,0,guy.width(),guy.height(),1);
  guy.display();
  Serial.println("Reading touch sensor...");
  //guy.ap_setup();
  //guy.server_setup();
}
int bright=128;
void loop(){
  //guy.server_loop();
  delay(10);
  if(bright%7==0){
    Serial.printf("getBtn: %d\n",guy.getBtn());
  }
  if(bright==511) bright=0;
  else bright++;
  guy.setBright(bright>=256?511-bright:bright);
}
