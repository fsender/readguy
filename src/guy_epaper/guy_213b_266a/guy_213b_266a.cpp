/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_213b_266a.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 2.13英寸B型墨水屏和 2.66英寸墨水屏 驱动库函数 头文件
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
#include "guy_213b_266a.h"
// #define MEPD_DEBUG_WAVE
#if (defined(READGUY_DEV_213B) || defined(READGUY_DEV_213B3C) \
|| defined(READGUY_DEV_266A) || defined(READGUY_DEV_266A3C))
namespace guydev_213B_266A{
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
static const PROGMEM unsigned char greyLutLevel[]={
  0, 3, 6, 9,12,15,18,21,24,28,31,34,37,42,48,54, //for BWR displays
  0, 3, 5, 7,10,13,16,19,22,25,29,32,35,38,43,49,
  0, 3, 5, 6, 8,11,14,17,20,23,26,29,32,35,39,44,
  0, 2, 4, 5, 6, 8,10,13,15,18,21,24,27,31,35,39,
  0, 2, 3, 5, 6, 7, 8,10,12,15,17,19,22,24,27,30,
  0, 2, 3, 4, 5, 6, 7, 8,10,12,14,16,18,20,22,24,

  0, 3, 3, 3, 4, 4, 5, 6, 7, 9,12,16,21,28,36,48,
};
drv_base::drv_base(){
  guy_lutArray[0] = lutSlow_vcom0;
  guy_lutArray[1] = lutSlow_w_w;
  guy_lutArray[2] = lutSlow_b_w;
  guy_lutArray[3] = lutSlow_w_b;
  guy_lutArray[4] = lutSlow_b_b;
}
void drv_base::epd_init(){
  //if(!Power_is_on) Reset();
    guy_epdCmd(0x01);
    guy_epdParam(0x03);
    guy_epdParam(0x10);
    guy_epdParam(isBWR?0x3f:0x2b);
    guy_epdParam(isBWR?0x3f:0x2b);
    guy_epdParam(0x03);
    guy_epdCmd(0x06);
    guy_epdParam(0x17);
    guy_epdParam(0x17);
    guy_epdParam(0x17);
    guy_epdCmd(0x00);
    guy_epdParam(0xbf);
    guy_epdParam(0x0d);
    guy_epdCmd(0x30);
    guy_epdParam(isBWR?0x3b:0x29); //0x3a:100Hz, 0x29:150Hz
    guy_epdCmd(0x61);
    guy_epdParam(0x98);
    guy_epdParam(0x01);
    guy_epdParam(0x28);
    guy_epdCmd(0x82);
    guy_epdParam(0x17);
    guy_epdCmd(0x50);
    guy_epdParam(0x17);
}
void drv_base::send_zoneInfo(){
  guy_epdCmd(0x90);
  guy_epdParam(0x00);
  guy_epdParam(epdWidth-1);
  guy_epdParam(0x00);
  guy_epdParam(0x00);
  guy_epdParam((epdHeight-1)>>8);
  guy_epdParam((epdHeight-1)&0xff);
  guy_epdParam(0x00);
}
void drv_base::SendLuts(bool part_lut){
  if(!Power_is_on) part_lut=false; //形参嘛
  for(uint8_t i=0;i<5;i++){
    guy_epdCmd(i+0x20);
    for(int j=0;j<(i==0?44:42);j++){
      if(part_lut){
        if(j==4 && ((i==2) || (greyHQ==3 && i==4))) guy_epdParam(isBWR?0x2f:0x0f); //刷黑->白
        else if(j==greyHQ) guy_epdParam(isBWR?pgm_read_byte(greyLutLevel+(greyLut+refTime*16)):greyLut);
        else if(j==0) guy_epdParam(pgm_read_byte(lutFast_+(i)));
        else if(j==5) guy_epdParam(1);
        else guy_epdParam(0x0);
      }
      else {
        guy_epdParam(j<24?pgm_read_byte(guy_lutArray[i]+j):0x00);
      }
    }
  }
  if(!Power_is_on || Power_is_on==2){
    guy_epdCmd(0x04);
    guy_epdBusy(-60);
    Power_is_on = 1;
  }
}
void drv_base::drv_init(){
  part_mode = 0;
  Power_is_on = 0; //初始为未上电
  //drv_color(0xff);
}
void drv_base::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(lastRefresh) return;
  if(!Power_is_on) part=0;
  if(!part) greyLut=15; //恢复默认的灰度模式
  part_mode = part;
}
void drv_base::drv_setDepth(uint8_t i){ 
  part_mode=1; 
  greyLut = i?(i>15?15:i):15; 
  BeginTransfer();
  SendLuts(1); 
  EndTransfer();
}
void drv_base::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(m&1){//stage 1
  if(lastRefresh) drv_dispWriter(f,2);
  BeginTransfer();
  epd_init();
  SendLuts(part_mode);
  guy_epdCmd(0x91);
  send_zoneInfo();
  guy_epdCmd(0x13);
  for (int i = 0; i < epdHeight*epdWidth/8; i++)
    SpiTransfer(f(i)); //按照给定的RAM写入数据

  guy_epdCmd(0x92);
  if(part_mode){
    guy_epdCmd(0x30);
    //guy_epdParam(0x3a); //黑白色用此行, 三色用下一行
    guy_epdParam(isBWR?0x19:(refTime?0x21:0x3a)); //0x3a:100Hz, 0x29:150Hz
    send_zoneInfo();
  }
  guy_epdCmd(0x12);
  EndTransfer();
  lastRefresh=millis();
  }
  if(m&2){//stage 2
    uint32_t ms=millis()-lastRefresh;
    if(part_mode){
      if((int32_t)ms<fastRefTime) guy_epdBusy((int32_t)ms-fastRefTime);
      refTime+=(refTime<5);
    }
    else{
      if((int32_t)ms<slowRefTime) guy_epdBusy((int32_t)ms-slowRefTime);
      BeginTransfer();
      epd_init();
      SendLuts(1);
      guy_epdCmd(0x92);
      EndTransfer();
      refTime=0;
    }
    lastRefresh=0;
  }
}
void drv_base::drv_sleep() { //开始屏幕睡眠
  //if(RST_PIN>=0) { //未定义RST_PIN时无法唤醒
  BeginTransfer();
  guy_epdCmd(0x02); // power off
  guy_epdBusy(-20);
  guy_epdCmd(0X10);
  guy_epdParam(0x01);
  EndTransfer();
  //}
  part_mode = 0;
  Power_is_on = 0;
}

void drv_base::drv_draw16grey_step(std::function<uint8_t(int)> f, int step){
  if(_quality&1) return readguyEpdBase::drv_draw16grey_step(f,step);
  if(step==1){
    greyHQ=3;
    refTime=6;
    drv_setDepth(3);
    drv_fullpart(1); //开始快刷
  }
  if(step==3) drv_setDepth(2);
  if(step==5) drv_setDepth(1);
  drv_dispWriter(f);
  if(step==15){
    greyHQ=4;
    refTime=5;
    drv_setDepth(15);
  }
} //据说可以设置灰度渲染方式. 好像是调用setGreyQuality函数就行来着

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
