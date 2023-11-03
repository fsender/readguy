/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_420a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 4.2英寸Hink墨水屏 驱动库函数
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
#include "guy_420a.h"
#ifdef READGUY_DEV_420A
namespace guydev_420A{
//////////// @brief 以下的部分代码来自半糖(HalfSweet)的程序
//////////// github.com/HalfSweetStudio/EPaperDrive
//////////// @copyright HalfSweet

const PROGMEM unsigned char drv::epd42_lut_full[]={
    0x08,0x00,0x48,0x40,0x00,0x00,0x00,//L0 B low-high-low-high
    0x20,0x00,0x12,0x20,0x00,0x00,0x00,//L1 W low-high-low
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,//L4 VCOM
    0x05,0x20,0x20,0x05,0x00,
    0x0f,0x00,0x00,0x00,0x00,
    0x20,0x40,0x20,0x20,0x00,
    0x20,0x00,0x00,0x00,0x00,
    0x05,0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00, 
};
const PROGMEM unsigned char drv::lut_213_B72_Full[]={
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x80, 0x60, 0x40, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x10, 0x60, 0x20, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x03, 0x03, 0x00, 0x00, 0x02, // TP0 A~D RP0
  0x09, 0x09, 0x00, 0x00, 0x02, // TP1 A~D RP1
  0x03, 0x03, 0x00, 0x00, 0x02, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};
const PROGMEM unsigned char drv::lut_213_B72[]={
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT0: BB:     VS 0 ~7
  0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT1: BW:     VS 0 ~7
  0x40, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT2: WB:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT3: WW:     VS 0 ~7
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //LUT4: VCOM:   VS 0 ~7
  0x0f, 0x00, 0x00, 0x00, 0x00, // TP0 A~D RP0
  0x00, 0x00, 0x00, 0x00, 0x00, // TP1 A~D RP1
  0x00, 0x00, 0x00, 0x00, 0x00, // TP2 A~D RP2
  0x00, 0x00, 0x00, 0x00, 0x00, // TP3 A~D RP3
  0x00, 0x00, 0x00, 0x00, 0x00, // TP4 A~D RP4
  0x00, 0x00, 0x00, 0x00, 0x00, // TP5 A~D RP5
  0x00, 0x00, 0x00, 0x00, 0x00, // TP6 A~D RP6
};
const PROGMEM unsigned char drv::lut_213_B72_16grey[]={ 
    //0x05,0x00,0x10,0x10,0x10,0x00,0x01,//L0 BB R0 B/W 0 
    //0x00,0x00,0x00,0x00,0x00,0x00,0x00,   //L1 BW R0 B/W 1
    //0x05,0x00,0x05,0x10,0x10,0x00,0x01, //L2 WB R1 B/W 0
    //0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //L3 WW R0 W/W 0
    //0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 VCOM
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 bb
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 bw
    0x01,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 wb
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 ww
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,  //L4 VCOM

    0x00,0x00,0x00,0x01,0x01,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,     
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,  
};

void drv::epd_Init(void){
  const uint16_t GreyDrvFrameFreq[]={
    0x1108, /*  75Hz */
    0x1108, /*  75Hz */
    0x180c, /*  35Hz */
    0x2c0a, /*  50Hz */
    0x0709, /*  65Hz */
    0x2f07, /*  80Hz */
    0x0807, /*  90Hz */
    0x2106, /* 100Hz */
    0x0306, /* 110Hz */
    0x1405, /* 120Hz */
    0x0705, /* 125Hz */
    0x3104, /* ~130Hz */
    0x2404, /* 135Hz */
    0x1804, /* 140Hz */
    0x0d04, /* 145Hz */
    0x0304, /* 150Hz */
  };
  //_InitDisplay fx
  if(epdFull==2) {
    Reset();
    epdFull--;
  }
  //guy_epdCmd(0x12); //wake from deepsleep?
  guy_epdCmd(0x74);
  guy_epdParam(0x54);
  guy_epdCmd(0x7e);
  guy_epdParam(0x3b);
  guy_epdCmd(0x01);
  guy_epdParam(0x2b); //300 columns, will send 299.
  guy_epdParam(0x01);
  guy_epdParam(0x00);

  /*guy_epdCmd(0x0c); //Booster Enable with Phase 1, Phase 2 
  //and Phase 3 for soft start current and duration setting.
  guy_epdParam(0x8b);
  guy_epdParam(0x9c);
  guy_epdParam(0xd6);
  guy_epdParam(0x0f);
  guy_epdCmd(0x3a); //set to 100 Hz(0x3a--0x21, 0x3b--0x06)
  guy_epdParam(0x21);
  guy_epdCmd(0x3b);
  guy_epdParam(0x06);*/
  guy_epdCmd(0x3c);
  guy_epdParam(0x03);
  guy_epdCmd(0x2c); //vcom
  guy_epdParam(0x70);  //-1.6v
  guy_epdCmd(0x03);
  guy_epdParam(0x15);
  guy_epdCmd(0x04); //set src volt (SEE DOC)
  guy_epdParam(0x41);
  guy_epdParam(0xa8);
  guy_epdParam(0x32);
  guy_epdCmd(0x3a); //Now trying 100Hz
  guy_epdParam((GreyDrvFrameFreq[GreyScalingHighQuality]>>8)&0xff); //raw 30
  guy_epdCmd(0x3b);
  guy_epdParam(GreyDrvFrameFreq[GreyScalingHighQuality]&0xff); //raw 0a
  guy_epdCmd(0x11); //Data Entry mode
  guy_epdParam(0x03);    //0x03 or 0x01
}
void drv::power_up(){
  if(!Power_is_on){
      //Power is not On
    guy_epdCmd(0x22);
    guy_epdParam(0xc0);
    guy_epdCmd(0x20);
    guy_epdBusy(120);
    Power_is_on=1;
  }
}
void drv::power_down(){
  Power_is_on=0;
  BeginTransfer();
  guy_epdCmd(0x22);
  guy_epdParam(0xc3);
  guy_epdCmd(0x20);
    EndTransfer();
  guy_epdBusy(210);
}

void drv::SetLut(const unsigned char* lut){
  guy_epdCmd(0x32);
  for(int i=0;i<70;i++){
    if(!epdFull && GreyScaling && !GreyScalingHighQuality && i==35) guy_epdParam(GreyScaling);
    else guy_epdParam(pgm_read_byte(lut+i));
  }
}
void drv::drv_init(){ //初始化屏幕
  epdFull = 2;
  drv_color(0xff);
}
void drv::drv_fullpart(bool part){ //初始化慢刷功能
  if(!part) GreyScaling=0;
  if(epdFull<=1) epdFull = !part; //epdFull==2代表睡眠中, 不能快刷
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新
  BeginTransfer();
  epd_Init();
  SetMemory();
  power_up();
  SetMemory();
  if(epdFull){ //慢刷
    guy_epdCmd(0x26);
    for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
      SpiTransfer(f(i)); //按照给定的RAM写入数据
  }
  guy_epdCmd(0x24);
  for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
    SpiTransfer(f(i)); //按照给定的RAM写入数据
  if(epdFull){ //慢刷
    epd_Init();
    SetMemory();
    SetLut(lut_213_B72_Full);
    guy_epdCmd(0x22);
    guy_epdParam(0xc4);
    guy_epdCmd(0x20);
    EndTransfer();
    guy_epdBusy(1600); //等待刷完
  }
  else{ //快刷
    guy_epdCmd(0x2c); //may a mistake? 此处不需要设置vcom
    guy_epdParam(0x20);
    SetLut(GreyScalingHighQuality?lut_213_B72_16grey:lut_213_B72);
    guy_epdCmd(0x22);
    guy_epdParam(0x04);
    guy_epdCmd(0x20);
    EndTransfer();
    guy_epdBusy(260); //等待屏幕刷新完成
  }
  BeginTransfer(); //write again
  SetMemory();
  guy_epdCmd(0x26);
  for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
    SpiTransfer(f(i)); //按照给定的RAM写入数据
  EndTransfer();
  if(epdFull) power_down();
}
void drv::drv_draw16grey_step(std::function<uint8_t(int)> f, int step){
  if(_quality&1) return readguyEpdBase::drv_draw16grey_step(f,step);
  if(step==1) drv_fullpart(1);//初始阶段,完成准备工作 //设置为快刷模式
  GreyScalingHighQuality=step; //开启高品质灰度模式
  drv_dispWriter(f);
  //DelayMs(10); //未知原因()
  if(step==15) GreyScalingHighQuality=0;//初始阶段,完成准备工作 //设置为快刷模式
}
/*void drv::drv_draw16grey(uint8_t *d16bit){ //绘制16阶段灰度示例图片, 注意这需要4倍的内存
  (void) d16bit;
  //drv_color(0xffu); 建议在开始前调用
  drv_fullpart(1); //设置为快刷模式
  for(int i=0;i<16;i++){
    GreyScalingHighQuality=i; //开启高品质灰度模式
    for(int j=0;j<15000;j++){ //从最深色开始发送, 逐步淡化, 重复15次(白色就不用刷了)
      //if(j%50<5) guy_epdParam(0xff);
      //guy_p[j] = (j%50-5<i*3?0:0xff); //For test only
    }
    //drv_display(guy_p);
  }
  GreyScalingHighQuality=0; //关闭灰度模式
}*/
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0) { //未定义RST_PIN时无法唤醒
    epdFull=2; //睡眠
    power_down();
    BeginTransfer();
    guy_epdCmd(0x10); // deep sleep mode
    guy_epdParam(0x01);     // enter deep sleep
    EndTransfer();
  }
}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
