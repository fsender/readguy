/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_420b.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 4.2英寸WF丝印墨水屏 驱动库函数
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
#include "guy_420b.h"
#ifdef READGUY_DEV_420B
namespace guydev_420B{
drv::drv(){
  guy_lutArray[0] = lut_vcom0;
  guy_lutArray[1] = lut_ww;
  guy_lutArray[2] = lut_bw;
  guy_lutArray[3] = lut_wb;
  guy_lutArray[4] = lut_bb;
  guy_lutArray[5] = lutFast_vc0;
  guy_lutArray[6] = lutFast_w_w;
  guy_lutArray[7] = lutFast_b_w;
  guy_lutArray[8] = lutFast_w_b;
  guy_lutArray[9] = lutFast_b_b;
  customLut=CUSTOM_LUT_DISABLE;
  customGreyscale=0;
}
void drv::Init(uint8_t pt) {
  Reset();
  guy_epdCmd(0x00); //Software reset
  guy_epdParam(0x00);
  delayMicroseconds(40);
  guy_epdCmd(0x00);
  guy_epdParam(0x1f);
  guy_epdParam(0x0d);
  delayMicroseconds(40);
  guy_epdCmd(0x50);
  guy_epdParam(0x97);
  delayMicroseconds(40);
  guy_epdCmd(0x01);
  guy_epdParam(0x03);
  guy_epdParam(0x00);
  guy_epdParam(0x2b);
  guy_epdParam(0x2b);
  delayMicroseconds(40);
  guy_epdCmd(0x06);
  guy_epdParam(0x17);
  guy_epdParam(0x17);
  guy_epdParam(0x17);
  delayMicroseconds(40);
  guy_epdCmd(0x00);
  guy_epdParam(0x3f);
  delayMicroseconds(40);
  guy_epdCmd(0x30);
  guy_epdParam(0x3a);
  delayMicroseconds(40);
  guy_epdCmd(0x61);
  guy_epdParam(0x01);
  guy_epdParam(0x90);
  guy_epdParam(0x01);
  guy_epdParam(0x2c);
  delayMicroseconds(40);
  guy_epdCmd(0x82); //vcom setting: 0x00:-0.1V, 0x3a: -3.0V
  guy_epdParam(0x1a);
  delayMicroseconds(40);
  guy_epdCmd(0x50);
  guy_epdParam(0xd7);
  delayMicroseconds(40);
  //不知为何, 此处需要快刷lut
  for(uint8_t i=0;i<=4;i++){ //for(uint8_t i=0;i<=(lutOption==2?5:4);i++){
    guy_epdCmd(i+0x20);
    for(int j=0;j<(pt?6:(i+pt==0?44:42));j++){
      if(j==2 && customGreyscale) guy_epdParam(63);
      else if(j==3 && i!=2) guy_epdParam(customLut);
      else guy_epdParam(j<18?pgm_read_byte(guy_lutArray[i+(pt?1:0)*5]+j):0x00);
      delayMicroseconds(40);
    }
  }
  if(pt!=2) guy_epdCmd(0x04);
  delayMicroseconds(40);
}

void drv::sendArea(){
  guy_epdCmd(0x91);
  sendAreaRaw();
}
void drv::sendAreaRaw(){
  guy_epdCmd(0x90);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam(0x01);
  guy_epdParam(0x8f);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam(0x01);
  guy_epdParam(0x2b);
  guy_epdParam(0x01);
}
const PROGMEM unsigned char drv::lut_vcom0[] ={
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02,  
  0x00, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x00, 0x08, 0x08, 0x00, 0x00, 0x02
};
const PROGMEM unsigned char drv::lut_ww[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02
};
const PROGMEM unsigned char drv::lut_bw[] ={
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02
};
const PROGMEM unsigned char drv::lut_bb[] ={
  0x20, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x10, 0x08, 0x08, 0x00, 0x00, 0x02
};
const PROGMEM unsigned char drv::lut_wb[] ={
  0xA0, 0x08, 0x08, 0x00, 0x00, 0x02, 
  0x90, 0x0F, 0x0F, 0x00, 0x00, 0x01, 
  0x50, 0x08, 0x08, 0x00, 0x00, 0x02
};
const PROGMEM unsigned char drv::lutFast_vc0[] ={ 0x00,2,0,48,0,1 };
const PROGMEM unsigned char drv::lutFast_w_w[] ={ 0x02,2,0,48,0,1 };
const PROGMEM unsigned char drv::lutFast_b_w[] ={ 0x5a,2,0,63,0,1 };
const PROGMEM unsigned char drv::lutFast_w_b[] ={ 0x84,2,0,48,0,1 };
const PROGMEM unsigned char drv::lutFast_b_b[] ={ 0x01,2,0,48,0,1 };
//void drv::epd_display(){
void drv::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(m&1){//stage 1
  if(lastRefresh) drv_dispWriter(f,2);
  BeginTransfer();
  Init(part_mode);
  if(part_mode){
    sendArea();
    guy_epdCmd(0x13);
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
      SpiTransfer(f(i));
    //data
    guy_epdCmd(0x92);
    sendAreaRaw();
    //[EPDrg_BW<>] refresh fx
  }
  else{
    Power_is_on=1;
    guy_epdCmd(0x10);
    //刷新数据
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
      SpiTransfer(f(i));
    guy_epdCmd(0x13);
    //刷新数据
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
      SpiTransfer(f(i));
    guy_epdCmd(0x92);
    //[EPDrg_BW<>] refresh fx
    guy_epdCmd(0x00);
    guy_epdParam(0x1f);
    guy_epdParam(0x0d);
    guy_epdCmd(0x50);
    guy_epdParam(0x97);
    //[EPDrg_EPD] wait_until_idle fx: 5000
  }
  guy_epdCmd(0x12);
  EndTransfer();
  lastRefresh=millis();
  }
  if(m&2){//stage 2
    uint32_t ms=millis()-lastRefresh;
    uint32_t u=part_mode?800:3600;
    if(ms<u) guy_epdBusy(ms-u);
    lastRefresh=0;
  BeginTransfer();
  if(part_mode){
    sendArea();
    guy_epdCmd(0x13);
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
      SpiTransfer(f(i));
    guy_epdCmd(0x92);
  }
  else{
    Init(2);
    sendArea();
    guy_epdCmd(0x13);
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
      SpiTransfer(f(i));
    guy_epdCmd(0x92);
    guy_epdCmd(0x02);
  }
    EndTransfer();
    guy_epdBusy(-20);
  }
}
const PROGMEM uint8_t drv::greyTable[16]={
  32,23,18,15,12,10,9,8,
  7,6,5,5,5,5,5,0
};
void drv::drv_init(){
  part_mode=0;
  Power_is_on=0;
  BeginTransfer();
  Init(0);
  EndTransfer();
  //drv_color(0xffu);
}
void drv::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(Power_is_on) {
    if(!part) customLut = CUSTOM_LUT_DISABLE;
    part_mode = part;
    //Init(part);
  }
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){ //未定义RST_PIN时无法唤醒
    BeginTransfer();
    guy_epdCmd(0x02);          //power off
    EndTransfer();
    guy_epdBusy(-30);
    BeginTransfer();
    guy_epdCmd(0x07);         //deep sleep
    guy_epdParam(0xA5);
    EndTransfer();
  }
  Power_is_on=0;
  part_mode=0;
}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
