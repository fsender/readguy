/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_583A.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 墨水屏 驱动库函数 源代码模板
 *        使用前, 先查找替换 583A 改为你需要的驱动名称
 *        比如3英寸的屏幕驱动程序应该叫 guy_dev_300a
 *        此时需要将 583A 替换为300A.
 *        替换好之后, 改文件名, 大小写也要注意改 (文件名最好不要出现大写)
 *        最后把这个brief替换掉
 * @version 1.0
 * @date 2023-11-11

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
#include "guy_583A.h"
#ifdef READGUY_DEV_583A
namespace guydev_583A{
/*LUT格式：
  重复次数，xnnnxnnn...(000=0,001=VSH,010=VSL,011=VSH_LV,100=VSL_LV,101=VSH_LVX,110=VSL_LVX,111=浮动),持续时间.......
  0x01, 0x00, 0x00, 0x1B, 0x10, 0x0F, 0x0A, 0x0A, 0x0F, 0x10, 0x1B
  0x01, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  0x01, 0x21, 0x21, 0x21, 0x21, 0x1B, 0x10, 0x0F, 0x0A, 0x0A, 0x0F, 0x10, 0x1B
  0x01, 0x12, 0x12, 0x12, 0x10, 0x14, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x00
  0x01, 0x11, 0x21, 0x21, 0x22, 0x1B, 0x10, 0x0F, 0x0A, 0x0A, 0x0F, 0x10, 0x1B
  0x01, 0x21, 0x21, 0x21, 0x20, 0x14, 0x01, 0x04, 0x01, 0x04, 0x01, 0x04, 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  0x01, 0x12, 0x12, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02
  0x01, 0x21, 0x21, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
*/
const PROGMEM unsigned char drv::lut_20_LUTC[] =
{  0x01, 0x00, 0x00, 0x1B, 0x10, 0x0F, 0x0A, 0x0A, 0x0F, 0x10, 0x1B};
const PROGMEM unsigned char drv::lut_20_LUTCFAST[] =
{  0x01, 0x00, 0x00, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::lut_21_LUTB[] =//000
{  0x01, 0x21, 0x21, 0x21, 0x21, 0x1B, 0x14, 0x13, 0x12, 0x00, 0x00, 0x10, 0x1E};
const PROGMEM unsigned char drv::lut_21_LUTBFAST[] =
{  0x01, 0x12, 0x12, 0x12, 0x10, 0x1f, 0x01, 0x02, 0x01, 0x02, 0x01, 0x02, 0x00};
const PROGMEM unsigned char drv::lut_22_LUTW[] =//011
{  0x01, 0x11, 0x21, 0x21, 0x22, 0x1B, 0x14, 0x13, 0x12, 0x00, 0x00, 0x10, 0x1E};
const PROGMEM unsigned char drv::lut_22_LUTWFAST[] =
{  0x01, 0x21, 0x21, 0x21, 0x20, 0x1f, 0x01, 0x04, 0x01, 0x04, 0x01, 0x04, 0x00};
const PROGMEM unsigned char drv::lut_23_LUTG1[] =//001
{  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::lut_24_LUTG2[] =//010
{  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::lut_25_LUTR0[] =//100//WW
{  0x01, 0x12, 0x12, 0x02, 0x02, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03};
const PROGMEM unsigned char drv::lut_26_LUTR1[] =//101//BB
{  0x00/*0x01*/, 0x21, 0x21, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02};
const PROGMEM unsigned char drv::lut_27_LUTR2[] =//110
{  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::lut_28_LUTR3[] =//111//局刷
{  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::lut_29_LUTXON[] =
{  0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
const PROGMEM unsigned char drv::customGrey[] =
{  0x00, 0x01, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0a, 0x0c, 0x0e, 0x11, 0x14, 0x17, 0x1a, 0x1e,
   0x00, 0x0f, 0x0e, 0x0d, 0x0c, 0x0b, 0x0a, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01};
const PROGMEM unsigned char *drv::luts[10]={
  lut_20_LUTC,lut_21_LUTB,lut_22_LUTW,lut_23_LUTG1,lut_24_LUTG2,
  lut_25_LUTR0,lut_26_LUTR1,lut_27_LUTR2,lut_28_LUTR3,lut_29_LUTXON
};
const unsigned char *drv::luts_fast[10]={
  lut_20_LUTCFAST,lut_21_LUTBFAST,lut_22_LUTWFAST,lut_23_LUTG1,lut_24_LUTG2,
  lut_25_LUTR0,lut_26_LUTR1,lut_27_LUTR2,lut_28_LUTR3,lut_29_LUTXON
};

#ifdef READGUY_583A_DUAL_BUFFER
drv::drv(){ //构造时, 分配内存.
  buf_2nd=(uint8_t *)malloc(GUY_D_WIDTH*GUY_D_HEIGHT/8);
  if(buf_2nd) {
    Serial.println(F("[GUY MEM] allocate 33600B for 5.83' EPD."));
    //memset(buf_2nd,0,GUY_D_WIDTH*GUY_D_HEIGHT/8);
  }
}
drv::~drv(){ //构造时, 分配内存.
  if(buf_2nd) free(buf_2nd);
  Serial.println(F("[GUY MEM] unallocate 33600B."));
}
#endif
void drv::sendLut(int lutid){
  int lutlen=13,lutlen2=247;
  if(lutid==0){lutlen=11;lutlen2=209;}
  else if(lutid==9){lutlen=10;lutlen2=190;}
  guy_epdCmd(0x20+lutid);
  if(epdFull){
    for(int i=0;i<lutlen;i++){
      guy_epdParam(pgm_read_byte(luts[lutid]+i));
    }
  }
  else{
    for(int i=0;i<lutlen;i++){
      if(lutid==1 && i==5) guy_epdParam(pgm_read_byte(customGrey+iLut+greyscalingHighQuality));
      else
      guy_epdParam(pgm_read_byte(luts_fast[lutid]+i));
    }
  }
  for(int i=0;i<lutlen2;i++){
    guy_epdParam(0);
  }
}
void drv::epd_init(){
  if(!epd_PowerOn) Reset(); //设定为已上电
  guy_epdCmd(0x01);
  guy_epdParam(0x37);
  guy_epdParam(0x00);
  guy_epdParam(0x07);
  guy_epdParam(0x07);
  guy_epdCmd(0x00);
  guy_epdParam(0xcf);
  guy_epdParam(0x88);
  guy_epdCmd(0x06);
  guy_epdParam(0xc7);
  guy_epdParam(0xcc);
  guy_epdParam(0x28);
  guy_epdCmd(0x30);
  guy_epdParam(0x3a);
  guy_epdCmd(0x41);
  guy_epdParam(0x00);
  guy_epdCmd(0x50);
  guy_epdParam(0x77);
  guy_epdCmd(0x60);
  guy_epdParam(0x22);
  guy_epdCmd(0x61);
  guy_epdParam(0x02);
  guy_epdParam(0x58);
  guy_epdParam(0x01);
  guy_epdParam(0xc0);
  guy_epdCmd(0x82);
  guy_epdParam(0x28);
  for(int i=0;i<10;i++){
    sendLut(i);
  }
  guy_epdCmd(0xe5);
  guy_epdParam(0x03);
}
void drv::epd_sendZoneInfo(){
    guy_epdCmd(0x91);
    //send area info
    guy_epdCmd(0x90);
    guy_epdParam(0x00);
    guy_epdParam(0x00);
    guy_epdParam(0x02);
    guy_epdParam(0x57);
    guy_epdParam(0x00);
    guy_epdParam(0x00);
    guy_epdParam(0x01);
    guy_epdParam(0xbf);
    guy_epdParam(0x01);
}
void drv::epd_sendWriter(std::function<uint8_t(int)> f){
    guy_epdCmd(0x10);  //Then write image data 显示数据
#ifdef READGUY_583A_DUAL_BUFFER
    if(epdFull || buf_2nd==nullptr || greyscalingHighQuality){
#endif
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++){ //fill 0x03 for test
        uint8_t dat=0,dat_r=f(i);
        dat |= ((dat_r&0x80)?0x30:0);
        dat |= ((dat_r&0x40)?0x03:0);
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x20)?0x30:0);
        dat |= ((dat_r&0x10)?0x03:0);
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x08)?0x30:0);
        dat |= ((dat_r&0x04)?0x03:0);
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x02)?0x30:0);
        dat |= ((dat_r&0x01)?0x03:0);
        guy_epdParam(dat);
#ifdef READGUY_583A_DUAL_BUFFER
        buf_2nd[i]=dat_r;
      }
    }
    else{
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/8;i++){ //fill 0x03 for test
        //guy_epdParam(0x03); //for data test
        uint8_t dat=0,dat_r=f(i);
        dat |= ((dat_r&0x80)?((buf_2nd[i]&0x80)?0x40:0x30):((buf_2nd[i]&0x80)?0x00:0x50));
        //0x3?黑->白色, 0x0?白->黑色 0x4? 白->白色 0x5? 黑->黑色
        dat |= ((dat_r&0x40)?((buf_2nd[i]&0x40)?0x4:0x3):((buf_2nd[i]&0x40)?0x0:0x5));
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x20)?((buf_2nd[i]&0x20)?0x40:0x30):((buf_2nd[i]&0x20)?0x00:0x50));
        dat |= ((dat_r&0x10)?((buf_2nd[i]&0x10)?0x4:0x3):((buf_2nd[i]&0x10)?0x0:0x5));
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x8)?((buf_2nd[i]&0x8)?0x40:0x30):((buf_2nd[i]&0x8)?0x00:0x50));
        dat |= ((dat_r&0x4)?((buf_2nd[i]&0x4)?0x4:0x3):((buf_2nd[i]&0x4)?0x0:0x5));
        guy_epdParam(dat);
        dat&=0;
        dat |= ((dat_r&0x2)?((buf_2nd[i]&0x2)?0x40:0x30):((buf_2nd[i]&0x2)?0x00:0x50));
        dat |= ((dat_r&0x1)?((buf_2nd[i]&0x1)?0x4:0x3):((buf_2nd[i]&0x1)?0x0:0x5));
        guy_epdParam(dat);
        buf_2nd[i]=dat_r;
      }
#endif
    }
}
void drv::drv_init(){ //初始化屏幕
  epdFull=1;
  epd_PowerOn=0;
}
void drv::drv_fullpart(bool part){ //初始化慢刷/快刷功能
  if(lastRefresh) return;
  if(!epd_PowerOn) part=0;
  epdFull=!part;
  if(epdFull) iLut=15;
}
void drv::drv_setDepth(uint8_t i){
  if(!epd_PowerOn) return; //不支持切换到快刷的情形
  epdFull=0; iLut = i?(i>15?15:i):15;  //如果需要, 改成自己的代码
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(m&1){//stage 1
    if(lastRefresh) drv_dispWriter(f,2);
    //add driver code...
      BeginTransfer();
    if(epdFull){
      if(!epd_PowerOn){
        epdFull=0; //发送快刷的lut
        epd_init();
        epdFull=1; //发送快刷的lut
        guy_epdCmd(0x04); //power on
        EndTransfer();
        guy_epdBusy(-60);
        BeginTransfer();
        guy_epdCmd(0x10);
        for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x33
          guy_epdParam(0x33);
        }
        epd_PowerOn=1; //设定为已上电
      }
      else{ //full refresh need power on
        guy_epdCmd(0x04); //power on
        EndTransfer();
        guy_epdBusy(-60);
        BeginTransfer();
      }
      epd_sendZoneInfo();
      guy_epdCmd(0x10);  //Then write image data
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
        guy_epdParam(0x33); //for test
      }
      guy_epdCmd(0x92);
      epd_init();
      //epd_init(); //repeat call in GxEPD2
      epd_sendWriter(f);  //write image data
    }
    else{
      epd_init();
      //Power on fx
      guy_epdCmd(0x04);
      EndTransfer();
      guy_epdBusy(-60);
      BeginTransfer();
      epd_sendZoneInfo();
      epd_sendWriter(f);
      guy_epdCmd(0x92);
      epd_sendZoneInfo();
    }
    guy_epdCmd(0x12);  //开始刷屏
    EndTransfer();
    lastRefresh=millis();
  }
  if(m&2){//stage 2
    int ms=millis()-lastRefresh;
    Serial.printf("epdFull? %d, epd_PowerOn? %d \n",epdFull,epd_PowerOn);
    int u=epdFull?slowRefTime:fastRefTime;  //全刷:局刷 busy时间
    if(ms<u) guy_epdBusy(ms-u);   //对于busy电平为低电平忙碌,高电平正常的屏幕则改为ms-u
    lastRefresh=0;
    //add driver code...
    BeginTransfer();
    if(epdFull){
      epdFull=0; //发送快刷的lut
      epd_init(); //repeat init in part mode
      epdFull=1; //发送快刷的lut
      epd_sendZoneInfo();
      guy_epdCmd(0x10);  //Then write image data
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
        guy_epdParam(0x33); //for test
      }
      guy_epdCmd(0x02); //power off for test...
      //guy_epdBusy(-30);
    }
    else{
      guy_epdCmd(0x92);
      epd_sendZoneInfo();
      guy_epdCmd(0x10);  //Then write image data 显示数据
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
        guy_epdParam(0x33); //for test:全白
      }
      guy_epdCmd(0x92);
    }
    EndTransfer();
  }
/* test code ,  with stageless ------------------------------------------------------
  if(epdFull){ //慢刷
    BeginTransfer();
    if(!epd_PowerOn){
      epdFull=0; //发送快刷的lut
      epd_init();
      epdFull=1; //发送快刷的lut
      guy_epdCmd(0x04); //power on
      EndTransfer();
      guy_epdBusy(-60);
      BeginTransfer();
      guy_epdCmd(0x10);
      for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x33
        guy_epdParam(0x33);
      }
      epd_PowerOn=1; //设定为已上电
    }
    else{ //full refresh need power on
      guy_epdCmd(0x04); //power on
      EndTransfer();
      guy_epdBusy(-60);
      BeginTransfer();
    }
    epd_sendZoneInfo();
    guy_epdCmd(0x10);  //Then write image data
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
      guy_epdParam(0x33); //for test
    }
    guy_epdCmd(0x92);
    epd_init();
    //epd_init(); //repeat call in GxEPD2
    epd_sendWriter(f);  //write image data
    guy_epdCmd(0x12);
    EndTransfer();
    guy_epdBusy(-4000);
    BeginTransfer();
    epdFull=0; //发送快刷的lut
    epd_init(); //repeat init in part mode
    epdFull=1; //发送快刷的lut
    epd_sendZoneInfo();
    guy_epdCmd(0x10);  //Then write image data
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
      guy_epdParam(0x33); //for test
    }
    guy_epdCmd(0x02); //power off for test...
    EndTransfer();
    //guy_epdBusy(-30);
  }
  else{ //partmode
    BeginTransfer();
    epd_init();
    //Power on fx
    guy_epdCmd(0x04);
    EndTransfer();
    guy_epdBusy(-60);
    BeginTransfer();
    epd_sendZoneInfo();
    epd_sendWriter(f);
    guy_epdCmd(0x92);
    epd_sendZoneInfo();
    guy_epdCmd(0x12);  //快刷
    EndTransfer();
    guy_epdBusy(-1000); //1秒钟刷屏时间
    BeginTransfer();
    guy_epdCmd(0x92);
    epd_sendZoneInfo();
    guy_epdCmd(0x10);  //Then write image data 显示数据
    for(int i=0;i<GUY_D_WIDTH*GUY_D_HEIGHT/2;i++){ //fill 0x03 for test
      guy_epdParam(0x33); //for test:全白
    }
    guy_epdCmd(0x92);
    EndTransfer();
  }
// test code ,  with stageless ------------------------------------------------------ */
}
void drv::drv_draw16grey_step(std::function<uint8_t(int)> f, int step){
  if(_quality&1) return readguyEpdBase::drv_draw16grey_step(f,step);
  if(step==1) drv_fullpart(1);//初始阶段,完成准备工作 //设置为快刷模式
  greyscalingHighQuality=16; //开启高品质灰度模式
  iLut=step;
  drv_dispWriter(f);
  //DelayMs(10); //未知原因()
  if(step==15) greyscalingHighQuality=0;//初始阶段,完成准备工作 //设置为快刷模式
}
void drv::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){ //RST_PIN<0 无法唤醒
    //add driver code...
    guy_epdCmd(0x07); // deep sleep
    guy_epdParam(0xA5);    // check code
  }
  epd_PowerOn = 0;
  epdFull = 1; //强制设置为慢刷新模式
}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */


