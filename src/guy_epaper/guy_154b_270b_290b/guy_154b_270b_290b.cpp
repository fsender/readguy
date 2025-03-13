/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_154b_270b_290b.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 1.54英寸B型 2.7英寸B型和2.9英寸B型墨水屏 驱动库函数
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
#include "guy_154b_270b_290b.h"

#if (defined(READGUY_DEV_154B) || defined(READGUY_DEV_270B) || defined(READGUY_DEV_290B))
namespace guydev_154B_270B_290B{
/*const unsigned char drvSSD168x::_WF_PARTIAL_2IN9[48] ={
  0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x80,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x00,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0xff,0x00,0x0,0x0,0x0,0x0,0x0,  
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x0,0x0,0x0,0x0,0x0,0x0,0x0,
  0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x0,
  0x22,0x17,0x41,0xB0,0x32,0x28
};*/

const unsigned char drvSSD168x::WS_20_30[42] =
{											/*
  0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
  0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
  0x80,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x40,	0x0,	0x0,	0x0,
  0x10,	0x66,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x20,	0x0,	0x0,	0x0,
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,
  0x14,	0x8,	0x0,	0x0,	0x0,	0x0,	0x1,					
  0xA,	0xA,	0x0,	0xA,	0xA,	0x0,	0x1,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x14,	0x8,	0x0,	0x1,	0x0,	0x0,	0x1,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x1,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x44,	0x44,	0x44,	0x44,	0x44,	0x44,	0x0,	0x0,	0x0,			
  0x22,	0x17,	0x41,	0x0,	0x32,	0x36
  */
  0x80,	0x48,	0x40,	/*0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,*/
  0x40,	0x48,	0x80,	/*0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,*/
  0x80,	0x48,	0x40,	/*0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,*/
  0x40,	0x48,	0x80,	/*0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,*/
  0x0,	0x0,	0x0,	/*0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,*/
  0xA,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0xf,	0x1,	0x0,	0xf,	0x1,	0x0,	0x1,					
  0xA,	0x0,	0x0,	0x0,	/*0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					
  0x0,	0x0,	0x0,	0x0,	0x0,	0x0,	0x0,					*/
  0x22,	0x22,	0x22,	0x22,	0x22,	0x22,	0x0,	0x0,	0x0,			
};	
const unsigned char drvSSD168x::VSH_table[32]=
{/*0x00,0xbc,0xc1,0xc6,0xcb,0x23,0x26,0x28,0x2a,0x2c,0x2e,0x32,0x35,0x39,0x3d,0x41,*/
  0x00,0x24,0x28,0x2c,0x30,0x33,0x36,0x38,0x3a,0x3c,0x3e,0x40,0x42,0x44,0x45,0x46,
  0x00,0x41,0x3c,0x37,0x32,0x2d,0x28,0x23,0xcb,0xc6,0xc1,0xbc,0xb7,0xb2,0xb0,0xae};


//以下代码均为我 FriendshipEnder 原创, 呵呵哒~~
void drvSSD168x::SetLut() {       
	unsigned int i;
	guy_epdCmd(0x3f);
	guy_epdParam(greyScaling?0x07:0x02); //greyScaling?0x07:0x22
	guy_epdCmd(0x03);	// gate voltage
	guy_epdParam(0x17); //0x17 orig
	guy_epdCmd(0x04);	// source voltage
	guy_epdParam(greyScaling?pgm_read_byte(VSH_table+16+iLut):pgm_read_byte(VSH_table+iLut));
	guy_epdParam(0xA8);	// VSH2
	guy_epdParam(0x32);	// VSL
	guy_epdCmd(0x2c);		// VCOM
	guy_epdParam(0x20); //0x28 orig
	guy_epdCmd(0x32);
  if(_part){
    for(i=0;i<5;i++){
      guy_epdParam(i==1?0x80:(i==(greyScaling?0:2)?0x40:0x00));
      for(int j=0;j<11;j++) guy_epdParam(0);
    }
    guy_epdParam(greyScaling?1:iLut);
    for(i=0;i<83;i++) guy_epdParam(0);
    guy_epdParam(0x22);
    for(i=0;i<8;i++) guy_epdParam(0);
  }
  else{
    //for(i=0; i<153; i++) guy_epdParam(lut[i]==0xff?iLut:lut[i]); 
    for(i=0;i<5;i++){
      guy_epdParam(pgm_read_byte(WS_20_30+i*3));
      guy_epdParam(pgm_read_byte(WS_20_30+i*3+1));
      guy_epdParam(pgm_read_byte(WS_20_30+i*3+2));
      for(int j=0;j<9;j++) guy_epdParam(0);
    }
    for(i=0;i<18;i++) guy_epdParam(pgm_read_byte(WS_20_30+i+15));
    for(i=0;i<66;i++) guy_epdParam(0);
    for(i=0;i<9;i++) guy_epdParam(pgm_read_byte(WS_20_30+i+33));
  }
}

void drvSSD168x::drv_init(){
  _part=0;
  epd_PowerOn=0;
  //drv_color(0xffu);
}
void drvSSD168x::drv_fullpart(bool part){ //切换慢刷/快刷功能
  if(lastRefresh) return;
  //if(!epd_PowerOn) part=0; //未上电 无法局刷
  if(!part) { iLut=15; greyScaling=0; }
  _part=part;
}
void drvSSD168x::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(m&1){//stage 1
  if(lastRefresh) drv_dispWriter(f,2);
  BeginTransfer();
  if(_part){
    //Reset();
    SetLut();
    guy_epdCmd(0x37); 
    for(int i=0;i<10;i++)  guy_epdParam(i==5?0x40:0x00);  
    guy_epdCmd(0x3C); //BorderWavefrom
    guy_epdParam(0x80);	
    guy_epdCmd(0x18); //BorderWavefrom
    guy_epdParam(0x80);	

    guy_epdCmd(0x22); 
    guy_epdParam(0xC0);   
    guy_epdCmd(0x20); 
    guy_epdBusy(140);  
  }
  else{
    if(epd_PowerOn==0) {
      Reset();
      epd_PowerOn=1;
      _part=0;
      iLut=15;
      guy_epdBusy(20);   
    }
    guy_epdCmd(0x12);  //SWRESET
    guy_epdBusy(20);   
    
    guy_epdCmd(0x01); //Driver output control      
    guy_epdParam((epdHeight-1)&0xff);
    guy_epdParam(((epdHeight-1)>>8)&0xff);
    guy_epdParam(0x00);
    
    guy_epdCmd(0x11); //data entry mode       
    guy_epdParam(0x03);
    SetMemory();
    SetLut();
  }
  SetMemory();
  guy_epdCmd(0x24);
  for (int i = 0; i < epdHeight*epdWidth / 8; i++) 
    SpiTransfer(f(i));
  if(!_part){
    guy_epdCmd(0x26);
    for (int i = 0; i < epdHeight*epdWidth / 8; i++) 
      SpiTransfer(f(i));
  }
  guy_epdCmd(0x22);
  guy_epdParam(_part?0x0f:0xc7);
  guy_epdCmd(0x20);
  EndTransfer();
  lastRefresh=millis();
  }
  if(m&2){//stage 2
    uint32_t ms=millis()-lastRefresh;
    uint32_t u=_part?600:2300;
    if(ms<u) guy_epdBusy(u-ms);
    lastRefresh=0;
  }
  //guy_epdBusy(_part?600:2300);
}
void drvSSD168x::drv_sleep() { //开始屏幕睡眠
  if(RST_PIN>=0){ //无法唤醒
    BeginTransfer();
    guy_epdCmd(0x10);
    guy_epdParam(0x01);
    EndTransfer();
  }
  epd_PowerOn=0;
}
void drvSSD168x::drv_setDepth(uint8_t i){ //设置显示颜色深度, 不支持的话什么都不做
  if(i>0 && i<16) {
    iLut = i;
    drv_fullpart(1);
  }
  else iLut=15;
}
void drvSSD168x::drv_draw16grey_step(std::function<uint8_t(int)> f, int step){
  if(_quality&1) return readguyEpdBase::drv_draw16grey_step(f,step);
  if(step==1){
    drv_fullpart(1);
    greyScaling=1;
  }
  iLut=step-1;
  drv_dispWriter(f);
  if(step==15){
    iLut=15;
    drv_dispWriter(f);
    greyScaling=0;
  }
}

}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */
