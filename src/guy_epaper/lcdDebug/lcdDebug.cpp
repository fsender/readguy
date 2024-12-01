/******************** F r i e n d s h i p E n d e r ********************
 * 本程序隶属于 Readguy 开源项目, 请尊重开源开发者, 也就是我FriendshipEnder.
 * 如果有条件请到 extra/artset/reward 中扫描打赏,否则请在 Bilibili 上支持我.
 * 项目交流QQ群: 926824162 (萌新可以进来问问题的哟)
 * 郑重声明: 未经授权还请不要商用本开源项目编译出的程序.
 * @file lcdDebug.cpp
 * @author FriendshipEnder (f_ender@163.com), Bilibili: FriendshipEnder
 * @brief 调试用墨水屏模拟器 驱动库函数 可使用LCD屏幕仿真墨水屏使用或测试
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
#include "lcdDebug.h"

#ifdef MEPD_DEBUG_DISPLAY
namespace EpdLcdDebug{
void drv::drv_init(){
  ips.configLGFX();
  ips.init();
  ips.fillScreen(0xffff);
}
void drv::drv_fullpart(bool part){
  if(lastRefresh) return;
  partMode=part;
}
void drv::drv_dispWriter(std::function<uint8_t(int)> f,uint8_t m){ //单色刷新
  if(!(m&1)) return; //stage 1
  uint16_t dat[8];
  unsigned short xbits=(drv_width()+7)/8;
  ips.startWrite();
  if(partMode==0){
    ips.invertDisplay(1);
    DelayMs(SIMULATE_SLOW_REFRESH_DELAY);
#if ((SIMULATE_BLINK) > 1)
    ips.invertDisplay(0);
    DelayMs(SIMULATE_SLOW_REFRESH_DELAY);
    ips.invertDisplay(1);
    DelayMs(SIMULATE_SLOW_REFRESH_DELAY);
#endif
#if ((SIMULATE_BLINK) > 0)
    ips.invertDisplay(0);
    DelayMs(SIMULATE_SLOW_REFRESH_DELAY);
    ips.invertDisplay(1);
    DelayMs(SIMULATE_SLOW_REFRESH_DELAY);
#endif
    ips.invertDisplay(0);
  }
  for(int j=0;j<drv_height();j++){
    for(int i=0;i<xbits;i++){
      uint_fast8_t readf=f(j*xbits+i);
#ifdef SIMULATE_GREYSCALE_COLOUR
      if(readf == 0xff && i!=xbits-1)
#endif
        ips.drawFastHLine(WHITE_GAP+i*8,WHITE_GAP+j,8,0xffff);
#ifdef SIMULATE_GREYSCALE_COLOUR
      else
#endif
      {
        int lineOK=0;
#ifdef SIMULATE_GREYSCALE_COLOUR
        if(partMode)//注意这里 readrect函数已经自动化实现边界处理了
          ips.readRect(WHITE_GAP+i*8,WHITE_GAP+j,8,1,dat); 
        else
#endif
          memset(dat,0xff,sizeof(dat));
        if(readf == 0x00 && i!=xbits-1){
          for(int k=0;k<8;k++)
            if((dat[k]&0x1f)==0x1f) lineOK++;
          if(lineOK==8) {
            ips.drawFastHLine(WHITE_GAP+i*8,WHITE_GAP+j,8,0x1082*(15-depth));
            continue;
          }
        }
        for(int k=0;k<8;k++){
          if(i==xbits-1 && i*8+k>=drv_width()) break;
#ifdef SIMULATE_GREYSCALE_COLOUR
          if((readf&(0x80>>k)))
            ips.drawPixel(WHITE_GAP+i*8+k,WHITE_GAP+j,0xffff);
          else if((dat[k]&0x1f)==0x1f)
            ips.drawPixel(WHITE_GAP+i*8+k,WHITE_GAP+j,0x1082*(15-depth));
#else
          if(!(readf&(0x80>>k))) {
            ips.drawPixel(WHITE_GAP+i*8+k,WHITE_GAP+j,0x1082*(15-depth)); 
          }
#endif
        }
      }
    }
    yield();
  }
  ips.endWrite();
  DelayMs(SIMULATE_FAST_REFRESH_DELAY);
}
void drv::drv_sleep() {}
}
#endif /* END OF FILE. ReadGuy project.
Copyright (C) 2023 FriendshipEnder. */