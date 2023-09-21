/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_213a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.13英寸墨水屏 驱动库函数
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
#include "guy_213a.h"
#ifdef READGUY_DEV_213A
namespace guydev_213A{
//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~

const PROGMEM uint8_t drv::_ed_lut_full[] = {   // command //慢刷lut
  0x22, 0x55, 0xaa, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x13, 0x13, 0x13, 0x13, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const PROGMEM uint8_t drv::_ed_lut_part[] = {   // command //快刷lut
  0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00  };
  
void drv::epd_Init(void){
  if(!epd_PowerOn) Reset(); //存疑的选项
  //BeginTransfer();
  guy_epdCmd(0x01); // Panel configuration, Gate selection
  guy_epdParam((GUY_D_HEIGHT - 1) % 256);
  guy_epdParam((GUY_D_HEIGHT - 1) / 256);
  guy_epdParam(0x00);
  guy_epdCmd(0x03); // Set Gate driving voltage
  guy_epdParam(0x15);
  guy_epdParam(0x0e);
  guy_epdCmd(0x04); // Set Source output voltage.
  guy_epdParam(0x1d);

  guy_epdCmd(0x0c); // softstart
  guy_epdParam(0xd7);
  guy_epdParam(0xd6);
  guy_epdParam(0x9d);
  guy_epdCmd(0x2c); // VCOM setting
  guy_epdParam(0x55);    // * different
  guy_epdCmd(0x3a); // DummyLine
  guy_epdParam(0x1a);    // 4 dummy line per gate
  guy_epdCmd(0x3b); // Gatetime
  guy_epdParam(0x08);    // 2us per line
  //设置内部显示内存
  guy_epdCmd(0x11); // set ram entry mode
  guy_epdParam(0x03);    // x increase, y increase : normal mode
  guy_epdCmd(0x44); //此处的指令参数已经是确定好的了, 不适合用那个放到epdBase里的setMemory
  guy_epdParam(0);
  guy_epdParam(15);
  guy_epdCmd(0x45);
  guy_epdParam(0);
  guy_epdParam(255);
  guy_epdParam(249);
  guy_epdParam(255);
  guy_epdCmd(0x4e);
  guy_epdParam(0);
  guy_epdCmd(0x4f);
  guy_epdParam(0);
  guy_epdParam(255);
  //guy_epdParam((GUY_D_HEIGHT-1)&255);
  //guy_epdParam((GUY_D_HEIGHT-1)>>8);

  guy_epdCmd(0x32); //发送lut
  DigitalWrite(DC_PIN, HIGH); //切换到data
  if(/* epd_PowerOn &&  */!epdFull) {
    for(uint32_t i=0;i<sizeof(_ed_lut_part);i++){
      if(i==16) SpiTransfer(iLut);
      else SpiTransfer(pgm_read_byte(_ed_lut_part+i));
    }
  }
  else  {
    for(uint32_t i=0;i<sizeof(_ed_lut_full);i++){
      SpiTransfer(pgm_read_byte(_ed_lut_full+i));
    }
  }
  DigitalWrite(DC_PIN, LOW); //切换到data
  //if(!epd_PowerOn){
    epd_PowerOn = 1;
    guy_epdCmd(0x22);
    guy_epdParam(0xc0);
    guy_epdCmd(0x20);
    guy_epdBusy(100);
  //}
}
void drv::drv_init(){ //初始化屏幕
  epdFull = 1;
  BeginTransfer();
  epd_Init();
  EndTransfer();
  drv_color(0xff);
}
void drv::drv_fullpart(bool part){ //初始化慢刷功能
  //if(part==epdFull) return;
  if(!part) iLut=15; //恢复默认的灰度模式
  epdFull = !part;
  //epd_Init();
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新
  BeginTransfer();
  if(epdFull) { //当刷新模式从快刷切换为慢刷时, 需要发送一次init
    epdFull=0;
    epd_Init();    //初始状态的慢刷也是要发送一次init
    epdFull=1;
    guy_epdCmd(0x26);
    //for (int j = GUY_D_HEIGHT-1; j >= 0; j--)
    //  for (int k = 0; k < GUY_D_WIDTH/8; k++)
    //    guy_epdParam(f(j*(GUY_D_WIDTH/8)+k)); //按照给定的RAM写入数据
    for (int i = 0; i < GUY_D_WIDTH/8*GUY_D_HEIGHT; i++)
      guy_epdParam(f(i));
  }
	guy_epdCmd(0x24);
	//for (int j = GUY_D_HEIGHT-1; j >= 0; j--)
	//	for (int k = 0; k < GUY_D_WIDTH/8; k++)
	//		guy_epdParam(f(j*(GUY_D_WIDTH/8)+k)); //按照给定的RAM写入数据
  for (int i = 0; i < GUY_D_WIDTH/8*GUY_D_HEIGHT; i++)
    guy_epdParam(f(i));
  epd_Init();
  guy_epdCmd(0x22);
  guy_epdParam(epdFull?0xc4:0x04);
  guy_epdCmd(0x20);
  EndTransfer();
  guy_epdBusy(epdFull?1600:310);
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN<0) return; //无法唤醒
  BeginTransfer();
  if(epd_PowerOn){
    guy_epdCmd(0x22);
    guy_epdParam(0xc3);
    guy_epdCmd(0x20);
    guy_epdBusy(200);
    epd_PowerOn = 0;
    epdFull = 1; //强制设置为慢刷新模式
  }
	guy_epdCmd(0x10); //enter deep sleep
	guy_epdParam(0x01);
  EndTransfer();
	DelayMs(200);
	DigitalWrite(RST_PIN, LOW);
}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


