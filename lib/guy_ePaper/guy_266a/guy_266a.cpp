/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_266a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.66英寸墨水屏 驱动库函数
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
#include "guy_266a.h"
// #define MEPD_DEBUG_WAVE
#ifdef READGUY_DEV_266A
namespace guydev_266A{
static const PROGMEM unsigned char lutSlow_vcom0[] ={
  0x00, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x60, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x00, 0x10, 0x00, 0x00, 0x00, 0x01,
  0x00, 0x1a, 0x1a, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_w_w[] ={
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x10, 0x00, 0x00, 0x00, 0x01,
  0xA0, 0x1a, 0x1a, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_b_w[] ={
  0x40, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x40, 0x10, 0x00, 0x00, 0x00, 0x01,
  0xA0, 0x1a, 0x1a, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_w_b[] ={
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x10, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x1a, 0x1a, 0x00, 0x00, 0x01
};
static const PROGMEM unsigned char lutSlow_b_b[] ={
  0x80, 0x08, 0x00, 0x00, 0x00, 0x02,
  0x90, 0x28, 0x28, 0x00, 0x00, 0x01,
  0x80, 0x10, 0x00, 0x00, 0x00, 0x01,
  0x50, 0x1a, 0x1a, 0x00, 0x00, 0x01
};
// partial update waveform
//参考灰度数据与屏幕相关. 测试用屏来自 vusion 黑白双色价签.
////以下代码均为我 FriendshipEnder 原创, 呵呵哒~~
static const PROGMEM unsigned char lutFast_[]={
  0x00,0x18,0x5a,0xa5,0x24
};
drv::drv(){
  guy_lutArray[0] = lutSlow_vcom0;
  guy_lutArray[1] = lutSlow_w_w;
  guy_lutArray[2] = lutSlow_b_w;
  guy_lutArray[3] = lutSlow_w_b;
  guy_lutArray[4] = lutSlow_b_b;
  guy_lutArray[5] = lutFast_;
}
void drv::pre(){
  guy_epdCmd(0x91);
  send_zoneInfo();
  guy_epdCmd(0x13);
}
void drv::epd_init(){
  if(!Power_is_on) Reset();
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
    guy_epdParam(0x29); //0x3a:100Hz, 0x29:150Hz
    guy_epdCmd(0x61);
    guy_epdParam(0x98);
    guy_epdParam(0x01);
    guy_epdParam(0x28);
    guy_epdCmd(0x82);
    guy_epdParam(0x17);
    guy_epdCmd(0x50);
    guy_epdParam(0x17);
}
void drv::send_zoneInfo(){
  guy_epdCmd(0x90);
  guy_epdParam(0x00);
  guy_epdParam(0x97);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam(0x01);
  guy_epdParam(0x27);
  guy_epdParam(0x00);
}
void drv::SendLuts(bool part_lut){
  if(!Power_is_on) part_lut=false; //形参嘛
  for(uint8_t i=0;i<5;i++){
    guy_epdCmd(i+0x20);
    for(int j=0;j<(i==0?44:42);j++){
      if(part_lut){
        if(j==4 && ((i==2) || (greyHQ==3 && i==4))) guy_epdParam(15);
        else if(j==greyHQ) guy_epdParam(greyLut);
        else if(j==0) guy_epdParam(pgm_read_byte(guy_lutArray[5]+(i)));
        else if(j==5) guy_epdParam(1);
        else guy_epdParam(0x0);
      }
      else {
        guy_epdParam(j<24?pgm_read_byte(guy_lutArray[i]+j):0x00);
      }
    }
  }
  if(!Power_is_on){
    guy_epdCmd(0x04);
    guy_epdBusy(-60);
    Power_is_on = 1;
  }
}
void drv::drv_init(){
  part_mode=0;
  drv_color(0xff);
}
void drv::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(!part) greyLut=15; //恢复默认的灰度模式
  part_mode = part;
}
void drv::drv_setDepth(uint8_t i){ 
  part_mode=1; 
  greyLut = i?(i>15?15:i):15; 
  BeginTransfer();
  SendLuts(1); 
  EndTransfer();
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f){ //单色刷新
  BeginTransfer();
  pre();
  //send pixel data -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
  //Total 5624 data written.
  //for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++)
  //  guy_epdParam(c);
  for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
    guy_epdParam(f(i)); //按照给定的RAM写入数据

  guy_epdCmd(0x92);
  if(part_mode){
    guy_epdCmd(0x30);
    guy_epdParam(0x3a); //0x3a:100Hz, 0x29:150Hz
    //[EPDrg_BW<>] refresh fx
    send_zoneInfo();

    guy_epdCmd(0x12);
    EndTransfer();
    //[EPDrg_EPD] wait_until_idle fx: 1300
    guy_epdBusy(-200);
      //[EPDrg_BW<>] writeImageAgain fx
    //guy_epdCmd(0x91);
    //send_zoneInfo();

    //guy_epdCmd(0x13);
      //send image data -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
      //Total 5624 data written.
    //guy_epdCmd(0x92);
  }
  else{
    epd_init();
    SendLuts(0);
    guy_epdCmd(0x12);
    //[EPDrg_EPD] wait_until_idle fx: 1600
    EndTransfer();
    guy_epdBusy(-2000);

    //[EPDrg_BW<>] writeImageAgain fx
    BeginTransfer();
    epd_init();
    SendLuts(1);
    pre();
    //send image data -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_
    //Total 5624 data written.
    for (int i = 0; i < GUY_D_HEIGHT*GUY_D_WIDTH/8; i++)
      guy_epdParam(f(i)); //按照给定的RAM写入数据
    guy_epdCmd(0x92);
    EndTransfer();
    //[EPDrg_BW<>] powerOff fx
    //guy_epdCmd(0x02);
    //[EPDrg_EPD] wait_until_idle fx: 20
    //guy_epdBusy(-20);
  }
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0) { //未定义RST_PIN时无法唤醒
    part_mode = 0;
    BeginTransfer();
    guy_epdCmd(0x02); // power off
    guy_epdBusy(-20);
    guy_epdCmd(0X10);
    guy_epdParam(0x01);
    EndTransfer();
    Power_is_on = 0;
  }
}

void drv::drv_draw16grey_step(std::function<uint8_t(int)> f, int step){
  if(_quality) return readguyEpdBase::drv_draw16grey_step(f,step);
  if(step==1){
    greyHQ=3;
    drv_setDepth(3);
    drv_fullpart(1); //开始局刷
  }
  if(step==3) drv_setDepth(2);
  if(step==5) drv_setDepth(1);
  drv_dispWriter(f);
  if(step==15){
    greyHQ=4;
    drv_setDepth(15);
  }
} //据说可以设置灰度渲染方式. 好像是调用setGreyQuality函数就行来着

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
