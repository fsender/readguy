/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_154a_290a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 1.54英寸和2.9英寸墨水屏 驱动库函数
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
#include "guy_154a_290a.h"

#if (defined(READGUY_DEV_154A) || defined(READGUY_DEV_290A))
namespace guydev_154A_290A{
//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~
int drvBase::Init(const unsigned char* lut) {
  // * EPD hardware init start /
  this->lut = lut;
  if(sleeping) {
    Reset();
    sleeping=0;
  }
  BeginTransfer();
  guy_epdCmd(0x01);
  guy_epdParam((epdHeight - 1) & 0xFF);
  guy_epdParam(((epdHeight - 1) >> 8) & 0xFF);
  guy_epdParam(0x00);                     // GD = 0; SM = 0; TB = 0;
  guy_epdCmd(0x0c);
  guy_epdParam(0xD7);
  guy_epdParam(0xD6);
  guy_epdParam(0x9D);
  guy_epdCmd(0x2c);
  guy_epdParam(0x9B);                     // VCOM 7C
  guy_epdCmd(0x3a);
  guy_epdParam(0x1A);                     // 4 dummy lines per gate
  guy_epdCmd(0x3b);
  guy_epdParam((iLut==1)?0x02:((iLut==3 || iLut==5)?0x05:0x08)); // 2us per line
  guy_epdCmd(0x11);
  guy_epdParam(0x03);                     // X increment; Y increment
  SetLut(this->lut);
  // * EPD hardware init end /
  EndTransfer();
  return 0;
}

void drvBase::SetLut(const unsigned char* lut) {
  this->lut = lut;
  guy_epdCmd(0x32);
  // * the length of look-up table is 30 bytes /
  for (int i = 0; i < 30; i++) {
    if(iLut>0 && iLut<15 && i>19 && i<23){
      guy_epdParam(pgm_read_byte(lut_grey_update+iLut*2+i-(i==20?22:23)));
    }
    else
      guy_epdParam(pgm_read_byte(this->lut+i));
  }
}

const PROGMEM unsigned char lut_slow[] =
{
  /*  0x02, 0x02, 0x01, 0x11, 0x12, 0x12, 0x22, 0x22, 
    0x66, 0x69, 0x69, 0x59, 0x58, 0x99, 0x99, 0x88, 
    0x00, 0x00, 0x00, 0x00, 0xF8, 0xB4, 0x13, 0x51, 
    0x35, 0x51, 0x51, 0x19, 0x01, 0x00*/
  0x50, 0xAA, 0x55, 0xAA, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const PROGMEM unsigned char lut_fast[] =
{
    0x10, 0x18, 0x18, 0x08, 0x18, 0x18, 0x08, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x13, 0x14, 0x44, 0x12, //0x13,0x14,0x44,0x12
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const PROGMEM unsigned char lut_grey_update[]={ //从上到下是依次加深
  0x10, 0x00, 0x10, 0x00, 0x10, 0x01, 0x10, 0x01,
  0x10, 0x02, 0x10, 0x02, 0x10, 0x03, 0x10, 0x04,
  0x10, 0x14, 0x10, 0x24, 0x10, 0x34, 0x11, 0x44,
  0x12, 0x44, 0x13, 0x44
};

void drvBase::drv_init(){
  Init(lut_slow);
  drv_color(0xffu); //睡眠模式下始终需要慢刷
}
void drvBase::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(!part) iLut=15; //恢复默认的灰度模式
  Init(part?lut_fast:lut_slow);
}
void drvBase::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新等功能
  if(sleeping) Init(lut_slow);
  BeginTransfer();
  SetMemory();   // bit set = white, bit reset = black
  guy_epdBusy(100);
  guy_epdCmd(0x24);  /* will send the color data */
  for (int i = 0; i < epdHeight*epdWidth / 8; i++) 
    SpiTransfer(f(i));
  guy_epdCmd(0x22);
  guy_epdParam(0xC4);
  guy_epdCmd(0x20);
  guy_epdCmd(0xff);
  EndTransfer();
  guy_epdBusy((this->lut == (const uint8_t*)lut_fast)?idleFastRf:idleSlowRf);
  BeginTransfer();
  SetMemory();   // bit set = white, bit reset = black
  guy_epdBusy(90);
  guy_epdCmd(0x26);  /* will send the color data */
  for (int i = 0; i < epdHeight*epdWidth / 8; i++) 
    SpiTransfer(f(i));
  EndTransfer();
}
void drvBase::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0) { //未定义RST_PIN时无法唤醒
    BeginTransfer();
    guy_epdCmd(0x10);
    EndTransfer();
    guy_epdBusy(150);
    DigitalWrite(RST_PIN, LOW);
    sleeping=1;
  }
}
void drvBase::drv_setDepth(uint8_t i){
  iLut = i?(i>15?15:i):15;
  Init(lut_fast);
}

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


