/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file guy_epdbase.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief readguy EPD 驱动程序 公共基类. 所有驱动都要依赖此文件.
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

#include "guy_epdbase.h"
#include <spi.h>

readguyEpdBase::readguyEpdBase() {
};

readguyEpdBase::~readguyEpdBase() {
};

void readguyEpdBase::DigitalWrite(int pin, int value) {
  if(pin>=0)  digitalWrite(pin, value);
}

int readguyEpdBase::DigitalRead(int pin) {
  return (pin>=0)?digitalRead(pin):1;
}

void readguyEpdBase::DelayMs(unsigned int delaytime) {
    delay(delaytime);
}
void readguyEpdBase::BeginTransfer(){
  if(!in_trans && CS_PIN>=0) {
    digitalWrite(CS_PIN, LOW);
    if(spi_tr_press!=nullptr) spi_tr_press();
  }
  in_trans++;
}
void readguyEpdBase::EndTransfer(){
  if(in_trans) in_trans--;
  if(!in_trans && CS_PIN>=0) {
    digitalWrite(CS_PIN, HIGH);
    if(spi_tr_release!=nullptr) spi_tr_release();
  }
}
void readguyEpdBase::SpiTransfer(unsigned char data) {
  if(in_trans) {
    _spi->transfer(data);
    return;
  }
  if(CS_PIN>=0)  digitalWrite(CS_PIN, LOW);
  _spi->transfer(data);
  if(CS_PIN>=0)  digitalWrite(CS_PIN, HIGH);
}
int readguyEpdBase::IfInit(SPIClass &c,int8_t cs,int8_t dc,int8_t rst,int8_t busy) {
  //static uint8_t IfHadInit=0;
  //if(IfHadInit) return;
  CS_PIN = cs;
  DC_PIN = dc;
  RST_PIN = rst;
  BUSY_PIN = busy;
  if(CS_PIN>=0)   pinMode(CS_PIN  , OUTPUT);
  if(RST_PIN>=0)  pinMode(RST_PIN , OUTPUT);
  DigitalWrite(RST_PIN,HIGH);
  if(DC_PIN>=0)   pinMode(DC_PIN  , OUTPUT);
  if(BUSY_PIN>=0) pinMode(BUSY_PIN, INPUT); 
  _spi = &c;

  //_spi->begin();
  //_spi->beginTransaction(SPISettings(2000000, MSBFIRST, SPI_MODE0));
  //IfHadInit = 1;
  return 0;
}
void readguyEpdBase::guy_epdCmd(unsigned char command)
{
  #ifdef MEPD_DEBUG_WAVE
  if(dat_combo) {
    if(dat_combo>MEPD_DEBUG_WAVE) Serial.printf("\nTotal %d data written.",dat_combo);
    Serial.write('\n');
  }
  dat_combo=0;
  Serial.printf("Cmd %02x: ", command);
  #endif
	DigitalWrite(DC_PIN, LOW);
	SpiTransfer(command);
	DigitalWrite(DC_PIN, HIGH);
}
void readguyEpdBase::guy_epdParam(unsigned char data)
{
  #ifdef MEPD_DEBUG_WAVE
  if(dat_combo==MEPD_DEBUG_WAVE){
    Serial.print("...");
  } 
  else if(dat_combo<MEPD_DEBUG_WAVE){
    Serial.printf("%02x ",data);
  }
  dat_combo++;
  #endif
	SpiTransfer(data);
}
void readguyEpdBase::guy_epdBusy(int32_t maxt) {
  #if (defined(MEPD_DEBUG_WAVE) || defined(MEPD_DEBUG_WAITTIME))
  Serial.printf("\nWaitUntilIdle: max %d ms.\n",maxt);
  #endif
  //if(spi_tr_release!=nullptr) spi_tr_release();
  uint32_t mls = millis();
  if(maxt<0){      //HIGH: idle, LOW: busy
    while((BUSY_PIN<0 || !DigitalRead(BUSY_PIN)) && millis()-mls<=uint32_t(-maxt))
      DelayMs(1);
  }
  else{      //LOW: idle, HIGH: busy
    while(DigitalRead(BUSY_PIN) && millis()-mls<=uint32_t(maxt))
      DelayMs(1);
  }
  #if (defined(MEPD_DEBUG_WAVE) || defined(MEPD_DEBUG_WAITTIME))
  Serial.printf("Wait %lu ms.\n",millis()-mls);
  #endif
  //if(spi_tr_press!=nullptr) spi_tr_press();
}
void readguyEpdBase::SetMemory() {
    guy_epdCmd(0x44);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    guy_epdParam(0);
    guy_epdParam(((drv_width()-1) >> 3) & 0xFF);
    guy_epdCmd(0x45);
    guy_epdParam(0);
    guy_epdParam(0);
    guy_epdParam((drv_height()-1) & 0xFF);
    guy_epdParam(((drv_height()-1) >> 8) & 0xFF);
    
    guy_epdCmd(0x4E);
    /* x point must be the multiple of 8 or the last 3 bits will be ignored */
    guy_epdParam(0);
    guy_epdCmd(0x4F);
    guy_epdParam(0);
    guy_epdParam(0);
}
void readguyEpdBase::Reset(uint32_t minTime)
{
  if(RST_PIN<0) return;
	//DigitalWrite(RST_PIN, HIGH);
	//DelayMs(minTime);
	DigitalWrite(RST_PIN, LOW);                //module reset
	DelayMs(minTime);
	DigitalWrite(RST_PIN, HIGH);
	DelayMs(minTime);
}
//void readguyEpdBase::drv_draw16grey(const uint8_t *d16bit){        //不支持的话什么都不做

void readguyEpdBase::drv_drawImage(LGFX_Sprite &sprbase,LGFX_Sprite &spr,uint16_t x,uint16_t y){
#ifndef FLOYD_STEINBERG_DITHERING
  static const uint8_t bayer_tab [64]={
     0, 32,  8, 40,  2, 34, 10, 42,
    48, 16, 56, 24, 50, 18, 58, 26,
    12, 44,  4, 36, 14, 46,  6, 38,
    60, 28, 52, 20, 62, 30, 54, 22,
     3, 35, 11, 43,  1, 33,  9, 41,
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47,  7, 39, 13, 45,  5, 37,
    63, 31, 55, 23, 61, 29, 53, 21
  };
#endif
  const uint32_t w = (spr.width()+7)>>3;
  if((!w) || (!spr.height())) return;
  uint16_t *readBuff = new uint16_t[spr.width()];
  uint8_t *writeBuff = new uint8_t[w];
#ifdef FLOYD_STEINBERG_DITHERING
  int16_t *floyd_tab[2];
  floyd_tab[0] = new int16_t [spr.width()];
  floyd_tab[1] = new int16_t [spr.width()];
  for(int j=0;j<spr.width();j++){ floyd_tab[0][j] = 0; floyd_tab[1][j] = 0; }
#endif
  sprbase.fillRect(x,y,spr.width(),spr.height(),1);
  for(int32_t i=0;i<(int32_t)spr.height();i++){
    spr.readRect(0,i,spr.width(),1,readBuff);
#ifdef FLOYD_STEINBERG_DITHERING
    uint_fast8_t buff8bit=0;
    for(int32_t j=0;j<(int32_t)spr.width();j++){
      int32_t flodelta = floyd_tab[i&1][j]+(int32_t)((greysc(readBuff[j])<<8)|greysc(readBuff[j]));
      if(flodelta>=0x8000) { 
        //spr.drawPixel(j,i,1);
        buff8bit |= 1<<((~j)&7);
        flodelta -= 0xffff;
      }
      if((j&7)==7 || j==((int32_t)spr.width()-1)){
        writeBuff[j>>3]=buff8bit;
        buff8bit=0;
      }
      //计算出读取到的颜色, 然后与128比较, 如果小于128, 显示黑色,否则显示白色
      //else              { spr.drawPixel(j,i,0); }
      /*if(j!=(int32_t)spr.width()-1) {
              floyd_tab[  i&1 ][j+1] += (flodelta*7)>>4;
              floyd_tab[!(i&1)][j+1] += (flodelta  )>>4;
      }
      if(j) { floyd_tab[!(i&1)][j-1] += (flodelta*3)>>4; }
            { floyd_tab[!(i&1)][j  ] += (flodelta*5)>>4; }*/
      if(j!=(int32_t)spr.width()-1) 
            { floyd_tab[i&1]   [j+1] += (flodelta*7)>>4; }
      if(j) { floyd_tab[!(i&1)][j-1] += (flodelta*3)>>4; }
            { floyd_tab[!(i&1)][j  ] += (flodelta*5)>>4; }
      if(j!=(int32_t)spr.width()-1) 
            { floyd_tab[!(i&1)][j+1] += (flodelta  )>>4; }
    }
    for(int floi=0;floi<spr.width();floi++) floyd_tab[i&1][floi]=0;
#else
    for(int32_t j=0;j<w;j++){
      uint_fast8_t buff8bit=0;
      for(uint_fast8_t b=0;b<8;b++)
        buff8bit |= (bayer_tab[(b<<3)|(i&7)]<(greysc(readBuff[(j<<3)+b])>>2))<<(7-b);
      writeBuff[j]=buff8bit;
    }
#endif
    sprbase.drawBitmap(x,y+i,writeBuff,spr.width(),1,1,0);
  }
  _display((const uint8_t*)sprbase.getBuffer());
  delete []readBuff;
  delete []writeBuff;
#ifdef FLOYD_STEINBERG_DITHERING
  delete [] floyd_tab[0] ;
  delete [] floyd_tab[1] ;
#endif
}
//不支持的话使用单色抖动刷屏
void readguyEpdBase::drv_draw16grey(LGFX_Sprite &sprbase,LGFX_Sprite &spr,uint16_t x,uint16_t y){
  //Serial.println("drv_draw16grey fx");
  const uint32_t w = (spr.width()+7)>>3;
  if((!w) || (!spr.height())) return;
  uint16_t *readBuff = new uint16_t[spr.width()];
  uint8_t *writeBuff = new uint8_t[w];
#ifdef FLOYD_DITHERING_16GREY
  int16_t *floyd_tab[2];
  floyd_tab[0] = new int16_t [spr.width()];
  floyd_tab[1] = new int16_t [spr.width()];
#endif
  sprbase.fillRect(x,y,spr.width(),spr.height(),1);
  bool negativeOrder=(drv_supportGreyscaling()==-16);
  drv_fullpart(0);
  _display((const uint8_t*)sprbase.getBuffer()); //先对区域慢刷白屏确保颜色正确
  drv_fullpart(1);
  for(uint_fast8_t k=1;k<16;k++){ //亮度为15的不用绘制,因为本来就是白色
#ifdef FLOYD_DITHERING_16GREY
    for(int j=0;j<spr.width();j++){ floyd_tab[0][j] = 0; floyd_tab[1][j] = 0; }
#endif
    for(int i=0;i<spr.height();i++){
      spr.readRect(0,i,spr.width(),1,readBuff);
      for(uint32_t j=0;j<w;j++){
        uint_fast8_t buff8bit=0;
        for(uint_fast8_t b=0;b<8;b++){
#ifdef FLOYD_DITHERING_16GREY
          uint_fast8_t cg=0;
          int32_t fd = floyd_tab[i&1][(j<<3)+b]+(int32_t)((greysc(readBuff[(j<<3)+b])<<8)|greysc(readBuff[(j<<3)+b]));
          while(fd>=0x800) { 
            cg++;
            if(fd>=0) fd -= 0x1000;
          }
          if(fd<0) fd++;
          if((j<<3)+b!=(uint32_t)spr.width()-1) 
                       { floyd_tab[i&1]   [(j<<3)+b+1] += (fd*7)>>4; }
          if((j<<3)+b) { floyd_tab[!(i&1)][(j<<3)+b-1] += (fd*3)>>4; }
                       { floyd_tab[!(i&1)][(j<<3)+b  ] += (fd*5)>>4; }
          if((j<<3)+b!=(uint32_t)spr.width()-1) 
                       { floyd_tab[!(i&1)][(j<<3)+b+1] += (fd  )>>4; }
#else
          uint_fast8_t cg=greysc(readBuff[(j<<3)+b])>>4;
#endif
          if(negativeOrder)
            buff8bit |= (cg<k)<<((~b)&7);
          else{
            if(cg==15) continue; //白色不考虑
            buff8bit |= (cg>=((~k)&15))<<((~b)&7);
          }
        }
        //sprbase.drawPixel(x+j,y+i,(greysc(readBuff[j])/16)==(15-k));
        writeBuff[j]=buff8bit^0xff;
      }
#ifdef FLOYD_DITHERING_16GREY
      for(int floi=0;floi<spr.width();floi++) floyd_tab[i&1][floi]=0;
#endif
      sprbase.drawBitmap(x,y+i,writeBuff,spr.width(),1,1,0);
    }
    drv_draw16grey_step((const uint8_t*)sprbase.getBuffer(),k); //使用灰度显示函数
  }
  delete []readBuff;
  delete []writeBuff;
#ifdef FLOYD_DITHERING_16GREY
  delete [] floyd_tab[0] ;
  delete [] floyd_tab[1] ;
#endif
} /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */