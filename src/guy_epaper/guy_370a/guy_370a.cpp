/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_370a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 3.7英寸墨水屏 驱动库函数
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
#include "guy_370a.h"
#ifdef READGUY_DEV_370A
namespace guydev_370A{
static const PROGMEM unsigned char lut_1Grey_GC[] ={
  0x2A,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//1
  0x05,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//2
  0x2A,0x15,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//3
  0x05,0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//4
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,//5
  0x00,0x02,0x03,0x0A,0x00,0x02,0x06,0x0A,0x05,0x00
};  
static const PROGMEM unsigned char lut_1Grey_A2[] ={
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //1
  0x0A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //2
  0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //3
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //4
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00, //5
  0x00,0x00,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00
}; 

int drv::Init(void) {
  if(sleeping) {
    Reset();
    sleeping=false;
    part_mode=false;
  }
  BeginTransfer();
  guy_epdCmd(0x12); DelayMs(300); //Power on display

  guy_epdCmd(0x46); guy_epdParam(0xF7); guy_epdBusy(20);
  guy_epdCmd(0x47); guy_epdParam(0xF7); guy_epdBusy(20); 
  guy_epdCmd(0x01); guy_epdParam(0xDF); guy_epdParam(0x01); guy_epdParam(0x00); // setting gaet number
  guy_epdCmd(0x03); guy_epdParam(0x00);                                         // set gate voltage
  guy_epdCmd(0x04); guy_epdParam(0x41); guy_epdParam(0xA8); guy_epdParam(0x32); // set source voltage
  guy_epdCmd(0x11); guy_epdParam(0x03);                                         // set data entry sequence
  guy_epdCmd(0x3C); guy_epdParam(0x01);                                         // set border 
  guy_epdCmd(0x0C);                                                             // set booster strength
  guy_epdParam(0xAE); guy_epdParam(0xC7); guy_epdParam(0xC3); guy_epdParam(0xC0); guy_epdParam(0xC0);  
  guy_epdCmd(0x18); guy_epdParam(0x80);                                         // set internal sensor on
  guy_epdCmd(0x2C); guy_epdParam(0x44);                                         // set vcom value

  guy_epdCmd(0x37); // set display option, these setting turn on previous function
  guy_epdParam(0x00);
  for(int i=0;i<4;i++) guy_epdParam(0xff); 
  guy_epdParam(0x4f);
  for(int i=0;i<4;i++) guy_epdParam(0xff); 
  guy_epdCmd(0x44); // setting X direction start/end position of RAM
  guy_epdParam(0x00); guy_epdParam(0x00); guy_epdParam(0x17); guy_epdParam(0x01);
  guy_epdCmd(0x45); // setting Y direction start/end position of RAM
  guy_epdParam(0x00); guy_epdParam(0x00); guy_epdParam(0xDF); guy_epdParam(0x01);
  guy_epdCmd(0x22); guy_epdParam(0xCF); // Display Update Control 2
  EndTransfer();
  return 0;
}
//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~
void drv::Load_LUT(unsigned char mode) {
  unsigned int i;
  guy_epdCmd(0x32);
  for (i = 0; i < 60; i++) {
    if(mode == 1)
      guy_epdParam(pgm_read_byte(lut_1Grey_GC+i));
    else if(mode == 0){
      if(20==i && greyScaling<8) guy_epdParam(0x03);
      else if(53==i && greyScaling<3) guy_epdParam(greyScaling);
      else if(53==i && greyScaling==15) guy_epdParam(5);
      else guy_epdParam(pgm_read_byte(lut_1Grey_A2+i));
    }
  }   
  for (i = 0; i < 45; i++) {
    if(i<40) guy_epdParam(0x00);
    else guy_epdParam(0x22);
  }
}

void drv::drv_init(){ //初始化屏幕
  sleeping = 1;
  Init();
  sleeping = 1;
  part_mode=0;
  //drv_color(0xff);
}
void drv::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(sleeping) return;
  if(!part) {
    greyScaling=15; //恢复默认的灰度模式
    BeginTransfer();
    guy_epdCmd(0x04); // set source voltage
    guy_epdParam(0x41);    guy_epdParam(0xA8);    guy_epdParam(0x32);
    EndTransfer();
  }
  part_mode=part;
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新
  if(sleeping) Init();
  BeginTransfer();
  guy_epdCmd(0x4E);  guy_epdParam(0x00);  guy_epdParam(0x00);
  guy_epdCmd(0x4F);  guy_epdParam(0x00);  guy_epdParam(0x00);

  guy_epdCmd(0x24);
  for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
    SpiTransfer(f(i)); //按照给定的RAM写入数据
  if(!part_mode) {
    guy_epdCmd(0x26);
    for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
      SpiTransfer(f(i)); //按照给定的RAM写入数据
  }

  Load_LUT(!part_mode);
  guy_epdCmd(0x20);
  EndTransfer();
  guy_epdBusy(part_mode?500:1300);  
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){
    BeginTransfer();
    guy_epdCmd(0X10);   //deep sleep
    guy_epdParam(0x03);
    EndTransfer();
  }
  sleeping = true;
  part_mode=0;
}

void drv::drv_setDepth(uint8_t i){ //设置显示颜色深度, 不支持的话什么都不做
  if(i<=0 || i>15) i=15; //形参 无所谓随便更改!
  greyScaling=i;
  if(i>7) i-=7;
  // Depth:   1    2    3    4    5    6    7    8    9   10   11   12   13   14  >=15
  //VSH(V): 2.4  3.6  4.7  5.6  6.4  7.3  8.1  9.0  9.8 10.7 11.6 12.4 13.3 14.1 15.0
  //HEX  : 0x8E,0x9A,0xA5,0xAE,0xB6,0xBF,0xC7,0x23,0x27,0x2C,0x30,0x34,0x38,0x3D,0x41
  static const uint8_t dep[]={0x8E,0x9A,0xA5,0xAE,0xB6,0xBF,0xC7,0x23,0x27,0x2C,0x30,0x34,0x38,0x3D};
  BeginTransfer();
    guy_epdCmd(0x04); // set source voltage
    if(greyScaling==15){ guy_epdParam(0x41); guy_epdParam(0xA8); }
    else{
      guy_epdParam(dep[i+6]); // VSH1,default is 0x41 (15.0V)
      guy_epdParam(dep[i-1]); // VSH2,default is 0xA8 (5.0V)
    }
    guy_epdParam(0x32);
  EndTransfer();
  part_mode=1;
}

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
