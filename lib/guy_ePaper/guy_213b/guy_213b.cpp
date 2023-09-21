/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_213b.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.13英寸B型墨水屏 驱动库函数
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

#include <stdlib.h>
#include "guy_213b.h"
#ifdef READGUY_DEV_213B
//#define MEPD_DEBUG_WAVE
namespace guydev_213B{
//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~
static const PROGMEM unsigned char lutSlow_vcom0[] ={
  0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x00, 0x0c, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x28, 0x00, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_w_w[] ={
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x0c, 0x00, 0x00, 0x00, 0x01,
  0x80, 0x20, 0x00, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_b_w[] ={
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x0c, 0x00, 0x00, 0x00, 0x02,
  0x80, 0x28, 0x00, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_w_b[] ={
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x0c, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x28, 0x00, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_b_b[] ={
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x0c, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x28, 0x00, 0x00, 0x00, 0x01
};

//partial screen update LUT
//#define Fx19 0x19 // original value is 25 (phase length)
#define Fx19 40   // new value for test is 40 (phase length) //Me:60
static const PROGMEM unsigned char lutFast_vcomDC[] ={
  0x00, Fx19, 0x01, 0x00, 0x00, 0x01 };
static const PROGMEM unsigned char lutFast_w_w[] ={
  0x00, Fx19, 0x01, 0x00, 0x00, 0x01 };
static const PROGMEM unsigned char lutFast_b_w[] ={
  0x80, 0x1c, 0x01, 0x00, 0x00, 0x01 };
static const PROGMEM unsigned char lutFast_w_b[] ={
  0x40, Fx19, 0x01, 0x00, 0x00, 0x01 };
static const PROGMEM unsigned char lutFast_b_b[] ={
  0x00, Fx19, 0x01, 0x00, 0x00, 0x01 };

drv::drv(){
  guy_lutArray[0] = lutSlow_vcom0;
  guy_lutArray[1] = lutSlow_w_w;
  guy_lutArray[2] = lutSlow_b_w;
  guy_lutArray[3] = lutSlow_w_b;
  guy_lutArray[4] = lutSlow_b_b;
  guy_lutArray[5] = lutFast_vcomDC;
  guy_lutArray[6] = lutFast_w_w;
  guy_lutArray[7] = lutFast_b_w;
  guy_lutArray[8] = lutFast_w_b;
  guy_lutArray[9] = lutFast_b_b;
}
void drv::InitDisplay(){
    guy_epdCmd(0x01);
    guy_epdParam(0x03);
    guy_epdParam(0x00);
    guy_epdParam(0x2b);
    guy_epdParam(0x2b);
    guy_epdParam(0x03);
    guy_epdCmd(0x06);
    guy_epdParam(0x17);
    guy_epdParam(0x17);
    guy_epdParam(0x17);
    guy_epdCmd(0x00);
    guy_epdParam(0xbf);
    guy_epdParam(0x0d);
    guy_epdCmd(0x30);
    guy_epdParam(0x3a);
    guy_epdCmd(0x61);
    guy_epdParam(0x68);
    guy_epdParam(0x00);
    guy_epdParam(0xd4);
    
    guy_epdCmd(0x82);
    guy_epdParam(0x08);
    guy_epdCmd(0x50);
}
void drv::epd_Init(uint8_t lastCmd){
  if(!epd_PowerOn) Reset(); //存疑的选项
  guy_epdCmd(0x91);
  guy_epdCmd(0x90);
  guy_epdParam(0x00);
  guy_epdParam(0x67);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam(0xd3);
  guy_epdParam(0x01);
  guy_epdCmd(lastCmd);
}

void drv::SendLuts(bool part){
  if(!epd_PowerOn) part=false; //形参嘛
  for(uint8_t i=0;i<5;i++){
    guy_epdCmd(i+0x20);
    for(int j=0;j<(i==0?44:42);j++){
      if(part){
        if(j==1) guy_epdParam(i==2?pgm_read_byte(lutFast_b_w+1):iLutFx19);
        else guy_epdParam(j<6?pgm_read_byte(guy_lutArray[i+5]+j):0);
      }
      else {
        guy_epdParam(j<24?pgm_read_byte(guy_lutArray[i]+j):0x00);
      }
    }
  }
  if(!epd_PowerOn){
    guy_epdCmd(0x04);
    epd_PowerOn=1;
  }
}
void drv::post(){
  guy_epdCmd(0x92); //数据发送完毕了
  if(epdFull) { //慢刷
    InitDisplay();
    guy_epdParam(0x97);
    SendLuts();
    //write lut here...
    guy_epdCmd(0x12);
    EndTransfer();
    guy_epdBusy(-2800); //慢刷等待
  }
  else{ //快刷
    InitDisplay();
    guy_epdParam(0x17);
    SendLuts(1); //快刷
    epd_Init();
    guy_epdCmd(0x12);
    EndTransfer();
    guy_epdBusy(-700);
  }
  //guy_epdCmd(0x92); //end refreshing
}

void drv::drv_init(){ //初始化屏幕
  epdFull = 1;
  drv_color(0xff);
}
void drv::drv_fullpart(bool part){ //初始化慢刷功能
  //if(part==epdFull) return;
  if(!part) iLutFx19=Fx19; //恢复默认的灰度模式
  epdFull = !part;
  //epd_Init();
}
    /* for (int j = 0; j < GUY_D_HEIGHT*GUY_D_WIDTH/8; j++)
      guy_epdParam(d[j]); //按照给定的RAM写入数据 */
void drv::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新
  BeginTransfer();
  epd_Init();
  //for (int j = 0; j < GUY_D_HEIGHT*GUY_D_WIDTH/8; j++)
  //  guy_epdParam(c); //按照给定的RAM写入数据
  for (int j = 0; j < GUY_D_HEIGHT*GUY_D_WIDTH/8; j++)
    guy_epdParam(f(j)); //按照给定的RAM写入数据
  post();
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){; //未定义RST_PIN时无法唤醒
    BeginTransfer();
    guy_epdCmd(0x02); //powerOff
    guy_epdBusy(-250);
    guy_epdCmd(0x07); //enter deep sleep
    guy_epdParam(0xa5);
    EndTransfer();
    DelayMs(200);
    epd_PowerOn=false;
    epdFull=1;
    iLutFx19=Fx19;
  }
}
void drv::drv_setDepth(uint8_t i){ //设置显示颜色深度
  static const uint8_t strength[14]={
    2,4,6,8,10,12,15,18,21,24,28,32,37,42
  };
  if(i) iLutFx19 = i>=15?Fx19:strength[i-1];
  else iLutFx19=Fx19;
  epdFull=0;
}

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


